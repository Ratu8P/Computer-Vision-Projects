import torch.nn as nn
import torch

class MyNetwork(nn.Module):
    def __init__(self, num_filters, dropout_rate, num_hidden_nodes):
        super(MyNetwork, self).__init__()
        self.conv1 = nn.Conv2d(1, num_filters, kernel_size=5)
        self.pool = nn.MaxPool2d(2, 2)
        self.conv2 = nn.Conv2d(num_filters, num_filters * 2, kernel_size=5)
        self.dropout = nn.Dropout(dropout_rate)
        self.fc1 = nn.Linear(num_filters * 2 * 4 * 4, num_hidden_nodes)
        self.fc2 = nn.Linear(num_hidden_nodes, 10)

    def forward(self, x):
        x = self.pool(torch.relu(self.conv1(x)))
        x = self.pool(torch.relu(self.conv2(x)))
        x = self.dropout(x)
        x = x.view(-1, self.fc1.in_features)
        x = torch.relu(self.fc1(x))
        x = torch.log_softmax(self.fc2(x), dim=1)
        return x
