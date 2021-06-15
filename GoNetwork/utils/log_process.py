train_loss = []
train_pre1 = []
train_pre5 = []
cache_num = []
with open('../data/data_log/train.log') as f:
    lines = f.readlines()
    for line in lines:
        if line.__contains__(' val  set'):
            pos = line.find('Loss') + len('Loss:')
            train_loss.append(float(line[pos: pos + 8]))
            pos = line.find('Pre@1 : ') + len('Pre@1 : ')
            train_pre1.append(float(line[pos: pos + 8]))
            pos = line.find('Pre@5:') + len('Pre@5:')
            train_pre5.append(float(line[pos: pos + 8]))

    print(train_loss)
    print(len(train_loss))
    print(train_pre1)
    print(len(train_pre1))
    print(train_pre5)
    print(len(train_pre5))
