import logging
import os
import shutil
import numpy as np
import time
import sys
import json

from collections import namedtuple


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
    logging.info('momentum : {:}'.format(args.momentum))
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
