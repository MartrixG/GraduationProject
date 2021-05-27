import os
import sys
import time

import numpy as np
import logging
import random

from torch import cuda, manual_seed
from utils.util import log_config, prepare
from utils.dataset_process import get_src_dataset
from torch.backends import cudnn


def train(args):
    logging.info('Start load dataset.')
    get_src_dataset(args.data_path)


def main(args):
    seed = prepare(args)
    if not cuda.is_available():
        logging.info('no gpu device available')
        sys.exit(1)
    np.random.seed(seed)
    random.seed(seed)
    manual_seed(seed)
    cuda.manual_seed(seed)
    cuda.set_device(args.gpu)
    cudnn.benchmark = False
    cudnn.deterministic = True

    log_config(args)
    start = time.time()
    train(args)
    tot_time = time.time() - start
    m, s = divmod(tot_time, 60)
    h, m = divmod(m, 60)
    logging.info("total time %d:%02d:%02d" % (h, m, s))
