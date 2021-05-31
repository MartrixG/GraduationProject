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

        self.layer_base = [0, 3, 11, 0, 0, 27, 19]

    def transform(self, feature, label):
        train_feature = torch.zeros((31, 19, 19), dtype=torch.float32)
        train_label = label[1]
        for i in range(19):
            for j in range(19):
                if feature[0][i][j] == 0:
                    train_feature[2][i][j] = 1
                else:
                    train_feature[1 - (int(feature[0][i][j]) == int(label[0]))][i][j] = 1

        train_feature[3] = torch.ones((19, 19))

        for layer_num in [1, 2, 5, 6]:
            layer = feature[layer_num]
            for i in range(19):
                for j in range(19):
                    if layer[i][j] != 0:
                        train_feature[self.layer_base[layer_num] + int(layer[i][j])][i][j] = 1

        if label[0] == 1:
            train_feature[30] = torch.ones((19, 19))

        return train_feature, train_label

    def __len__(self):
        return self.length

    def __getitem__(self, index):
        assert 0 <= index < self.length, 'invalid index = {:}'.format(index)
        return self.transform(self.feature[index], self.label[index])


def get_src_dataset(root, train_size, val_size, test_size):
    # 1824026
    val_base = 1000000
    y_train_file = [line.strip() for line in open('{:}/train_data/label.txt'.format(root)).readlines()]
    x_train_data = np.zeros((train_size, 7, 19, 19), dtype=np.int8)
    y_train_data = np.zeros((train_size, 2), dtype=np.int32)
    x_val_data = np.zeros((val_size, 7, 19, 19), dtype=np.int8)
    y_val_data = np.zeros((val_size, 2), dtype=np.int32)
    x_test_data = np.zeros((test_size, 7, 19, 19), dtype=np.int8)
    y_test_data = np.zeros((test_size, 2), dtype=np.int32)
    count = -1
    with open('{:}/train_data/feature.txt'.format(root)) as f:
        while True:
            count += 1
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
            elif train_size <= count < val_base:
                continue
            elif val_base <= count < val_base + val_size:
                x_val_data[count - val_base] = board_information
                if color == 'B':
                    y_val_data[count - val_base][0] = 1
                else:
                    y_val_data[count - val_base][0] = 2
                y_val_data[count - val_base][1] = pos
            elif val_base + val_size <= count < val_base + val_size + test_size:
                x_test_data[count - (val_size + val_base)] = board_information
                if color == 'B':
                    y_test_data[count - (val_size + val_base)][0] = 1
                else:
                    y_test_data[count - (val_size + val_base)][0] = 2
                y_test_data[count - (val_size + val_base)][1] = pos
            else:
                break

    train_data = (torch.tensor(x_train_data, dtype=torch.float32), torch.tensor(y_train_data, dtype=torch.long))
    val_data = (torch.tensor(x_val_data, dtype=torch.float32), torch.tensor(y_val_data, dtype=torch.long))
    test_data = (torch.tensor(x_test_data, dtype=torch.float32), torch.tensor(y_test_data, dtype=torch.long))
    return train_data, val_data, test_data


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
