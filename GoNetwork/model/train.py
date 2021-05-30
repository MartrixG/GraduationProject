import os
import sys
import time

import numpy as np
import logging
import random
import struct

import torch
from torch import cuda, manual_seed, nn
from utils.util import log_config, prepare, get_opt_scheduler, AverageMeter, accuracy, save
from utils.dataset_process import get_src_dataset, get_data_loader
from torch.backends import cudnn
from model.train_model import NetWork
from model.flop_benchmark import get_model_infos
from socket import *


def model_train(train_loader, model, criterion, optimizer, epoch_str, print_freq, grad_clip):
    model.train()
    base_top1 = AverageMeter()
    base_top5 = AverageMeter()
    base_loss = AverageMeter()
    for step, (base_feature, base_label) in enumerate(train_loader):
        batch = base_feature.size(0)
        base_feature = base_feature.cuda()
        base_label = base_label.cuda()
        optimizer.zero_grad()
        label = model(base_feature)
        loss = criterion(label, base_label)
        loss.backward()
        nn.utils.clip_grad_norm(model.get_weights(), grad_clip)
        optimizer.step()
        pre1, pre5 = accuracy(label.data, base_label.data, top_k=(1, 5))
        base_top1.update(pre1.item(), batch)
        base_top5.update(pre5.item(), batch)
        base_loss.update(loss.item(), batch)

        if step % print_freq == 0 or step + 1 == len(train_loader):
            str1 = 'train - epoch:' + epoch_str + ' batch:[' + '{:3d}/{:}]  '.format(step, len(train_loader))
            str2 = '[Loss:{:.6f}  Pre@1:{:.5f}%  Pre@5:{:.5f}%]'.format(base_loss.avg, base_top1.avg,
                                                                        base_top5.avg)
            logging.info(str1 + str2)

    return base_top1.avg, base_top5.avg, base_loss.avg


def mode_val(valid_loader, model, criterion, epoch_str, print_freq):
    model.eval()
    val_top1 = AverageMeter()
    val_top5 = AverageMeter()
    val_loss = AverageMeter()
    for step, (val_feature, val_label) in enumerate(valid_loader):
        batch = val_feature.size(0)
        val_feature = val_feature.cuda()
        val_label = val_label.cuda()

        label = model(val_feature)
        loss = criterion(label, val_label)

        pre1, pre5 = accuracy(label.data, val_label.data, top_k=(1, 5))
        val_top1.update(pre1.item(), batch)
        val_top5.update(pre5.item(), batch)
        val_loss.update(loss.item(), batch)

        if step % print_freq == 0 or step + 1 == len(valid_loader):
            str1 = 'valid - epoch:' + epoch_str + ' batch:[' + '{:3d}/{:}]  '.format(step, len(valid_loader))
            str2 = '[Loss:{:.6f}  Pre@1:{:.5f}%  Pre@5:{:.5f}%]'.format(val_loss.avg, val_top1.avg,
                                                                        val_top5.avg)
            logging.info(str1 + str2)

    return val_top1.avg, val_top5.avg, val_loss.avg


def train(args):
    logging.info('Start load dataset.')
    train_data, val_data, test_data = get_src_dataset(args.data_path, args.train_size, args.val_size, args.test_size)
    train_loader, val_loader, test_loader = get_data_loader(train_data, val_data, test_data, args.batch_size, args.workers)
    logging.info('Data set loaded.')

    model = NetWork('policy', [1, args.input_channel, 19, 19], 361)
    model = model.cuda()
    flop, param = get_model_infos(model, [1, args.input_channel, 19, 19])
    logging.info('Params={:.2f} M, FLOPs={:.2f} M'.format(param, flop))

    optimizer, scheduler, criterion = get_opt_scheduler(
        model.get_weights(), args.optimizer, args.lr, args.weight_decay, args.scheduler, args.epoch)
    criterion = criterion.cuda()

    logging.info('optimizer : {:}'.format(optimizer))
    logging.info('scheduler : {:}'.format(scheduler))
    logging.info('criterion   : {:}'.format(criterion))

    need_time = AverageMeter()
    time_str = ''
    best_acc = 0

    for epoch in range(1, args.epoch + 1):
        logging.info('epoch:{:} LR:{:.6f} need time {:}'.format(epoch, scheduler.get_lr()[0], time_str))
        epoch_str = '[{:03d}/{:03d}]'.format(epoch, args.epoch)

        s_time = time.time()
        train_top1, train_top5, train_loss = model_train(train_loader, model, criterion, optimizer, epoch_str, args.print_frequency, args.grad_clip)
        val_top1, val_top5, val_loss = mode_val(val_loader, model, criterion, epoch_str, args.print_frequency)
        need_time.update(time.time() - s_time)

        m, s = divmod(need_time.avg * (args.epoch - epoch), 60)
        h, m = divmod(m, 60)
        time_str = "%d:%02d:%02d" % (h, m, s)
        train_str = 'train set - epoch:' + epoch_str + ' result  Loss:'
        vla_str = ' val  set - epoch:' + epoch_str + ' result  Loss:'
        logging.info(train_str + '{:.6f}  Pre@1 : {:.5f}%  Pre@5:{:.5f}%'.format(train_loss, train_top1, train_top5))
        logging.info(vla_str + '{:.6f}  Pre@1 : {:.5f}%  Pre@5:{:.5f}%'.format(val_loss, val_top1, val_top5))

        if val_top1 > best_acc:
            best_acc = val_top1
            logging.info('find the best model. best acc is {:.5f}%'.format(best_acc))
            logging.info('Save it to {:}'.format(args.save + 'best.pt'))
            save(model, os.path.join(args.save, 'best.pt'))

        scheduler.step()

    logging.info('best acc is {:.5f}%'.format(best_acc))


def play(args):
    files = os.listdir('.')
    model_file = ''
    for file in files:
        if file == 'model.pt':
            model_file = file
    if model_file == '':
        return

    logging.info("find model and loading.")
    model = NetWork('forward', [1, 31, 19, 19], 361)
    model.load_state_dict(torch.load(model_file))

    if torch.cuda.is_available():
        model = model.cuda()
        logging.info("using GPU.")
    else:
        logging.info("using CPU.")

    server_name = '127.0.0.1'
    server_port = 23334
    buf_size = 120000
    ADDR = (server_name, server_port)

    server_socket = socket(AF_INET, SOCK_STREAM)
    server_socket.bind(ADDR)

    logging.info("binding with 127.0.0.1:23334")
    logging.info("listening...")
    server_socket.listen(1)

    client_socket, client_addr = server_socket.accept()
    logging.info('client accept')
    while True:
        try:
            data_byte = client_socket.recv(buf_size)
        except IOError as e:
            logging.info(str(e))
            logging.info('client close')
            client_socket.close()
            break
        if not data_byte:
            break
        data_src = data_byte.decode('utf8').strip().split('\n')
        prob = forward(data_src, model)
        for one_state_res in prob:
            client_socket.send(one_state_res)


def forward(data_src, model):
    layer_base = [0, 3, 11, 27, 19]
    input_data = torch.zeros((len(data_src), 31, 19, 19), dtype=torch.float32)
    for k in range(len(data_src)):
        data = data_src[k]
        color = int(data[-1])

        feature = torch.tensor(np.array(list(map(int, data))[:-1], dtype=np.int8).reshape(5, 19, 19))

        train_feature = torch.zeros((31, 19, 19), dtype=torch.float32)

        for i in range(19):
            for j in range(19):
                if feature[0][i][j] == 0:
                    train_feature[2][i][j] = 1
                else:
                    train_feature[1 - (int(feature[0][i][j]) == color)][i][j] = 1

        train_feature[3] = torch.ones((19, 19))

        for layer_num in range(1, 5):
            layer = feature[layer_num]
            for i in range(19):
                for j in range(19):
                    if layer[i][j] != 0:
                        train_feature[layer_base[layer_num] + int(layer[i][j])][i][j] = 1

        if color == 1:
            train_feature[30] = torch.ones((19, 19))

        input_data[k] = train_feature

    if torch.cuda.is_available():
        input_data = input_data.cuda()

    out = model.forward(input_data)
    out = out.cpu().detach().numpy()

    pro = np.exp(out) / np.sum(np.exp(out))
    for i in pro:
        for j in i:
            print("{:.6f}".format(j), end=',')
        print()
    res = []
    for i in pro:
        one_state_res = b''
        for j in i:
            float_str = "{:.6f}".format(j)[::-1]
            one_state_res += float_str[0:-2].encode('utf8')
        one_state_res += b'\0'
        res.append(one_state_res)
    return res


def main(args):
    seed = prepare(args)
    np.random.seed(seed)
    random.seed(seed)
    manual_seed(seed)
    cuda.manual_seed(seed)
    cuda.set_device(args.gpu)
    cudnn.benchmark = False
    cudnn.deterministic = True

    log_config(args)
    start = time.time()
    if args.type == 'train':
        train(args)
    else:
        play(args)
    tot_time = time.time() - start
    m, s = divmod(tot_time, 60)
    h, m = divmod(m, 60)
    logging.info("total time %d:%02d:%02d" % (h, m, s))
