from torch import nn


class NetWork(nn.Module):
    def __init__(self, name, x_shape, num_class):
        super(NetWork, self).__init__()
        self.name = name
        self.x_shape = x_shape
        self.num_class = num_class
        self.conv = nn.ModuleList()
        self.first_layer = nn.Sequential(
            nn.Conv2d(in_channels=47, out_channels=192, kernel_size=(5, 5), stride=(1, 1), padding=(2, 2), bias=True),
            nn.ReLU()
        )

        for i in range(11):
            self.conv.append(
                nn.Sequential(
                    nn.Conv2d(in_channels=192, out_channels=192, kernel_size=(3, 3), stride=(1, 1), padding=(1, 1), bias=True),
                    nn.ReLU()
                )
            )

        self.classify_layer = nn.Conv2d(in_channels=192, out_channels=1, kernel_size=(1, 1), stride=(1, 1), padding=(0, 0))

    def forward(self, feature):
        x = self.first_layer(feature)

        for conv in self.conv:
            x = conv(x)

        out = self.classify_layer(x)
        out = out.reshape(-1, 19, 19)
        return out

    def get_weights(self):
        return self.parameters()
