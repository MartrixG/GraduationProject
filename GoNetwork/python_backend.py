import os

import numpy as np
import torch

from socket import socket, AF_INET, SOCK_STREAM
from torch import nn


class NetWork(nn.Module):
    def __init__(self, name, x_shape, num_class):
        super(NetWork, self).__init__()
        self.name = name
        self.x_shape = x_shape
        self.num_class = num_class
        self.first_layer = nn.Sequential(
            nn.Conv2d(in_channels=x_shape[1], out_channels=256, kernel_size=(5, 5), stride=(1, 1), padding=(2, 2), bias=True),
            nn.BatchNorm2d(256),
            nn.ReLU(),
        )

        self.blocks = nn.ModuleList()
        for i in range(19):
            self.blocks.append(
                nn.Sequential(
                    nn.Conv2d(in_channels=256, out_channels=256, kernel_size=(3, 3), stride=(1, 1), padding=(1, 1)),
                    nn.BatchNorm2d(256),
                    nn.ReLU(),
                    nn.Conv2d(in_channels=256, out_channels=256, kernel_size=(3, 3), stride=(1, 1), padding=(1, 1)),
                    nn.BatchNorm2d(256)
                )
            )
        self.relu = nn.ReLU()
        self.classify_layer = nn.Conv2d(in_channels=256, out_channels=1, kernel_size=(1, 1), stride=(1, 1), padding=(0, 0))

    def forward(self, feature):
        x = self.first_layer(feature)

        for i in range(len(self.blocks)):
            x = x + self.blocks[i](x)

        x = self.relu(x)

        x = self.classify_layer(x)
        out = x.reshape(-1, 361)

        return out

    def get_weights(self):
        return self.parameters()


def play():
    files = os.listdir()
    model_file = ''
    for file in files:
        if file == 'model.pt':
            model_file = file
    if model_file == '':
        return

    model = NetWork('forward', [1, 31, 19, 19], 361)
    model.load_state_dict(torch.load(model_file))

    if torch.cuda.is_available():
        model = model.cuda()

    server_name = '127.0.0.1'
    server_port = 23334
    buf_size = 120000
    ADDR = (server_name, server_port)

    server_socket = socket(AF_INET, SOCK_STREAM)
    server_socket.bind(ADDR)

    server_socket.listen(1)

    client_socket, client_addr = server_socket.accept()
    while True:
        try:
            data_byte = client_socket.recv(buf_size)
        except IOError:
            client_socket.close()
            break
        if not data_byte:
            break
        length = int.from_bytes(data_byte, signed=False, byteorder='big')
        data_src = []
        for i in range(length):
            data_byte = client_socket.recv(1806)
            data_src.append(data_byte.decode('utf8').strip())
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
    res = []
    for i in pro:
        one_state_res = b''
        for j in i:
            float_str = "{:.6f}".format(j)[::-1]
            one_state_res += float_str[0:-2].encode('utf8')
        res.append(one_state_res)
    return res


if __name__ == '__main__':
    play()
