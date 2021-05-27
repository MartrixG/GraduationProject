import numpy as np


def get_src_dataset(root):
    x_train_file = np.array([list(map(int, line.strip())) for line in open('{:}/train_data/feature_text.txt'.format(root)).readlines()])
    y_train_file = np.array([list(line.strip()) for line in open('{:}/train_data/label.txt'.format(root)).readlines()])
    x_train_file = np.reshape(x_train_file, (-1, 7, 19, 19))
    for i in range(10):
        print(i)
        for j in range(7):
            for k in range(19):
                for m in range(19):
                    print(x_train_file[i][j][k][m], end='')
                print()
            print('\n')
