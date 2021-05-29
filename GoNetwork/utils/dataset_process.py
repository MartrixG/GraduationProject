import numpy as np
import torch

from torch.utils.data import Dataset, DataLoader


class NormalDataSet(Dataset):
    def __init__(self, name, src_data):
        super(NormalDataSet, self).__init__()
        self.name = name
        self.feature = src_data[0]
        self.label = src_data[1]
        self.length = len(self.feature)

    def __len__(self):
        return self.length

    def __getitem__(self, index):
        assert 0 <= index < self.length, 'invalid index = {:}'.format(index)
        return self.feature[index], self.label[index]


def get_src_dataset(root, train_size, val_size, test_size):
    # 1824026
    y_train_file = [line.strip() for line in open('{:}/train_data/label.txt'.format(root)).readlines()]
    x_train_data = np.zeros((train_size, 7, 19, 19), dtype=np.int8)
    y_train_data = np.zeros((train_size, 2), dtype=np.int32)
    x_val_data = np.zeros((val_size, 7, 19, 19), dtype=np.int8)
    y_val_data = np.zeros((val_size, 2), dtype=np.int32)
    x_test_data = np.zeros((test_size, 7, 19, 19), dtype=np.int8)
    y_test_data = np.zeros((test_size, 2), dtype=np.int32)
    count = 0
    with open('{:}/train_data/feature.txt'.format(root)) as f:
        while True:
            line = list(map(int, f.readline().strip()))
            board_information = np.array(line, dtype=np.int8).reshape(7, 19, 19)
            color, pos = y_train_file[count].split(' ')
            pos = (ord(pos[0]) - ord('a')) * 19 + (ord(pos[1]) - ord('a'))
            if count < train_size:
                x_train_data[count] = board_information
                if color == 'B':
                    y_train_data[count][0] = 1
                else:
                    y_train_data[count][0] = 2
                y_train_data[count][1] = pos
            elif count < train_size + val_size:
                x_val_data[count - train_size] = board_information
                if color == 'B':
                    y_val_data[count - train_size][0] = 1
                else:
                    y_val_data[count - train_size][0] = 2
                y_val_data[count - train_size][1] = pos
            elif count < train_size + val_size + test_size:
                x_test_data[count - (train_size + val_size)] = board_information
                if color == 'B':
                    y_test_data[count - (train_size + val_size)][0] = 1
                else:
                    y_test_data[count - (train_size + val_size)][0] = 2
                y_test_data[count - (train_size + val_size)][1] = pos
            else:
                break
            count += 1

    train_data = get_unzip_data(x_train_data, y_train_data)
    val_data = get_unzip_data(x_val_data, y_val_data)
    test_data = get_unzip_data(x_test_data, y_test_data)
    return train_data, val_data, test_data


def get_unzip_data(x_data_src, y_data_src):
    layer_base = [0, 3, 11, 19, 27, 43, 35]
    x_data = np.zeros((len(x_data_src), 47, 19, 19))
    y_data = np.zeros(len(y_data_src))
    for index in range(len(x_data_src)):
        feature = x_data_src[index]
        label = y_data_src[index]
        train_feature = np.zeros((47, 19, 19), dtype=np.float32)
        train_label = label[1]
        # 0 - 2 player
        for i in range(19):
            for j in range(19):
                if feature[0][i][j] == 0:
                    train_feature[2][i][j] = 1
                else:
                    train_feature[1 - (int(feature[0][i][j]) == int(label[0]))][i][j] = 1
        # 3 one
        train_feature[3] = np.ones((19, 19))
        # 4 - 11 turn
        # 12 - 19 qi
        # 20 - 27 capture
        # 28 - 35 self capture
        # 36 - 43 qi after move
        # 44 legal
        for layer_num in range(1, 7):
            layer = x_data_src[index][layer_num]
            for i in range(19):
                for j in range(19):
                    if layer[i][j] != 0:
                        train_feature[layer_base[layer_num] + layer[i][j]][i][j] = 1
        # 45 zeros
        # 46
        if label[0] == 1:
            train_feature[46] = np.ones((19, 19))

        x_data[index] = train_feature
        y_data[index] = label[1]

    return torch.tensor(x_data, dtype=torch.float32), torch.tensor(y_data, dtype=torch.long)


def get_data_loader(train_data, val_data, test_data, batch_size, num_workers):
    train_loader = DataLoader(NormalDataSet('train', train_data),
                              batch_size=batch_size,
                              num_workers=num_workers,
                              shuffle=True,
                              pin_memory=True)

    val_loader = DataLoader(NormalDataSet('val', val_data),
                            batch_size=batch_size,
                            num_workers=num_workers,
                            shuffle=False,
                            pin_memory=True)

    test_loader = DataLoader(NormalDataSet('test', test_data),
                             batch_size=batch_size,
                             num_workers=num_workers,
                             shuffle=False,
                             pin_memory=True)

    return train_loader, val_loader, test_loader
