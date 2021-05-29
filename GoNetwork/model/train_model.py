from torch import nn


class NetWork(nn.Module):
    def __init__(self, name, x_shape, num_class):
        super(NetWork, self).__init__()
        self.name = name
        self.x_shape = x_shape
        self.num_class = num_class
        self.first_layer = nn.Sequential(
            nn.Conv2d(in_channels=47, out_channels=192, kernel_size=(5, 5), stride=(1, 1), padding=(2, 2), bias=True),
            nn.BatchNorm2d(192),
            nn.ReLU(),
        )

        self.blocks = nn.ModuleList()
        for i in range(19):
            self.blocks.append(
                nn.Sequential(
                    nn.Conv2d(in_channels=192, out_channels=192, kernel_size=(3, 3), stride=(1, 1), padding=(1, 1)),
                    nn.BatchNorm2d(192),
                    nn.ReLU(),
                    nn.Conv2d(in_channels=192, out_channels=192, kernel_size=(3, 3), stride=(1, 1), padding=(1, 1)),
                    nn.BatchNorm2d(192)
                )
            )
        self.relu = nn.ReLU()
        self.classify_layer = nn.Conv2d(in_channels=192, out_channels=1, kernel_size=(1, 1), stride=(1, 1), padding=(0, 0))

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
