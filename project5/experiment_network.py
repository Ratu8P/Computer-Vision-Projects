import torch.nn as nn
import torch

class MyNetwork(nn.Module):
    def __init__(self, conv1_filters=10, conv2_filters=20, dropout_rate=0.5, fc1_nodes=50):
        super(MyNetwork, self).__init__()
        self.conv1 = nn.Conv2d(1, conv1_filters, kernel_size=5)
        self.pool = nn.MaxPool2d(2, 2)
        self.conv2 = nn.Conv2d(conv1_filters, conv2_filters, kernel_size=5)
        self.dropout = nn.Dropout(dropout_rate)
        self.fc1 = nn.Linear(conv2_filters * 4 * 4, fc1_nodes)  # 动态调整输入
        self.fc2 = nn.Linear(fc1_nodes, 10)

    def forward(self, x):
        x = self.pool(torch.relu(self.conv1(x)))
        x = self.pool(torch.relu(self.conv2(x)))
        x = self.dropout(x)
        x = x.view(-1, self.fc1.in_features)  # 获取动态输入大小
        x = torch.relu(self.fc1(x))
        x = torch.log_softmax(self.fc2(x), dim=1)
        return x