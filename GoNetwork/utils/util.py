import logging
import os
import shutil
import numpy as np
import time
import sys
import json
import torch

from torch import optim, nn
from collections import namedtuple


class AverageMeter(object):

    def __init__(self):
        self.avg = 0
        self.sum = 0
        self.cnt = 0

    def update(self, val, n=1):
        self.sum += val * n
        self.cnt += n
        self.avg = self.sum / self.cnt


def convert_param(original_list):
    assert isinstance(original_list, list), 'The type is not correct : {:}'.format(original_list)
    ctype, value = original_list[0], original_list[1]
    is_list = isinstance(value, list)
    if not is_list:
        value = [value]
    re = []
    for x in value:
        if ctype == 'int':
            x = int(x)
        elif ctype == 'str':
            x = str(x)
        elif ctype == 'bool':
            x = bool(int(x))
        elif ctype == 'float':
            x = float(x)
        elif ctype == 'none':
            assert x == 'None', 'for none type, the value must be None instead of {:}'.format(x)
            x = None
        else:
            raise TypeError('Does not know this type : {:}'.format(ctype))
        re.append(x)
    if not is_list:
        re = re[0]
    return re


def load_config(path, if_dict=False):
    path = str(path)
    with open(path, 'r') as f:
        data = json.load(f)
    content = {k: convert_param(v) for k, v in data.items()}
    if if_dict:
        return content
    Arguments = namedtuple('Configure', ' '.join(content.keys()))
    content = Arguments(**content)
    return content


def log_config(args):
    logging.info("mission type : {:}".format(args.type))
    logging.info('load the dataset : {:}'.format(args.name))
    logging.info('batch_size : {:}'.format(args.batch_size))
    logging.info('epoch : {:}'.format(args.epoch))
    logging.info('optimizer : {:}'.format(args.optimizer))
    logging.info('scheduler : {:}'.format(args.scheduler))
    logging.info('learning rate : {:}'.format(args.lr))
    logging.info('weight_decay : {:}'.format(args.weight_decay))
    logging.info('args:{:}'.format(args))


def create_exp_dir(path, scripts_to_save=None):
    if not os.path.exists(path):
        os.mkdir(path)
    print('Experiment dir : {}'.format(path))

    if scripts_to_save is not None:
        os.mkdir(os.path.join(path, 'scripts'))
        for script in scripts_to_save:
            dst_file = os.path.join(path, 'scripts', os.path.basename(script))
            shutil.copyfile(script, dst_file)


def prepare(args):
    if args.seed == -1:
        seed = np.random.randint(0, 10000)
    else:
        seed = args.seed
    args.save = 'log/{}-{}-seed-{:05d}/'.format(args.type, time.strftime("%Y-%m-%d-%H-%M-%S"), seed)
    create_exp_dir(args.save)

    log_format = '%(asctime)s %(message)s'
    logging.basicConfig(stream=sys.stdout, level=logging.INFO,
                        format=log_format, datefmt='%m/%d %I:%M:%S %p')
    fh = logging.FileHandler(os.path.join(args.save, args.type + '.log'))
    fh.setFormatter(logging.Formatter(log_format))
    logging.getLogger().addHandler(fh)
    return seed


def get_opt_scheduler(params, optm, lr, decay, scheduler_name, epoch):
    if optm == 'SGD':
        optimizer = optim.SGD(params, lr, weight_decay=decay, momentum=0.9)
    elif optm == 'Adam':
        optimizer = optim.Adam(params, lr, weight_decay=decay)
    else:
        raise ValueError

    if scheduler_name == 'PolyScheduler':
        scheduler = optim.lr_scheduler.CosineAnnealingLR(optimizer, epoch, eta_min=0.0001)
    else:
        raise ValueError
    criterion = nn.CrossEntropyLoss()
    return optimizer, scheduler, criterion


def accuracy(output, target, top_k=(1,)):
    max_k = max(top_k)
    batch_size = target.size(0)

    _, pred = output.topk(max_k, 1, True, True)
    pred = pred.t()
    correct = pred.eq(target.view(1, -1).expand_as(pred))

    res = []
    for k in top_k:
        correct_k = correct[:k].view(-1).float().sum(0)
        res.append(correct_k.mul_(100.0 / batch_size))
    return res


def save(model, model_path):
    torch.save(model.state_dict(), model_path)
