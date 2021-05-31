import os
import time

import numpy as np
import logging
import random

from torch import cuda, manual_seed, nn
from utils.util import log_config, prepare, get_opt_scheduler, AverageMeter, accuracy, save
from utils.dataset_process import get_src_dataset, get_data_loader
from torch.backends import cudnn
from model.train_model import NetWork
from model.flop_benchmark import get_model_infos


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
        play()
    tot_time = time.time() - start
    m, s = divmod(tot_time, 60)
    h, m = divmod(m, 60)
    logging.info("total time %d:%02d:%02d" % (h, m, s))
