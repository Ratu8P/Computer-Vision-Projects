import torch
import torchvision
import torchvision.transforms as transforms
from torch.utils.data import DataLoader
import torch.nn as nn
import torch.optim as optim
import time
import pandas as pd
import random

# 定义网络结构
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

# 加载 Fashion-MNIST 数据集
def load_data():
    transform = transforms.Compose([
        transforms.ToTensor(),
        transforms.Normalize((0.5,), (0.5,))
    ])

    train_set = torchvision.datasets.FashionMNIST(
        root='./data', train=True, download=True, transform=transform)
    test_set = torchvision.datasets.FashionMNIST(
        root='./data', train=False, download=True, transform=transform)

    train_loader = DataLoader(train_set, batch_size=32, shuffle=True)
    test_loader = DataLoader(test_set, batch_size=32, shuffle=False)

    return train_loader, test_loader

# 定义训练与评估函数
def train_and_evaluate(num_filters, dropout_rate, num_hidden_nodes, train_loader, test_loader):
    model = MyNetwork(num_filters, dropout_rate, num_hidden_nodes)
    criterion = nn.CrossEntropyLoss()
    optimizer = optim.SGD(model.parameters(), lr=0.01, momentum=0.9)

    start_time = time.time()

    for epoch in range(5):  # 训练 5 个 epoch
        model.train()
        for inputs, labels in train_loader:
            optimizer.zero_grad()
            outputs = model(inputs)
            loss = criterion(outputs, labels)
            loss.backward()
            optimizer.step()

    # 测试集准确率
    model.eval()
    correct = 0
    total = 0
    with torch.no_grad():
        for inputs, labels in test_loader:
            outputs = model(inputs)
            _, predicted = torch.max(outputs.data, 1)
            total += labels.size(0)
            correct += (predicted == labels).sum().item()

    accuracy = 100 * correct / total
    elapsed_time = time.time() - start_time

    return accuracy, elapsed_time

# 实验主程序
if __name__ == "__main__":
    train_loader, test_loader = load_data()

    # 参数设置
    filter_options = [10, 20, 30]
    dropout_options = [0.3, 0.5, 0.7]
    hidden_node_options = [50, 100, 150]

    # 保存结果
    results = []

    # Round-robin search strategy
    for _ in range(50):  # 运行 50 次
        # 随机选择其中一个参数进行优化，其余固定
        mode = random.choice(['filters', 'dropout', 'hidden_nodes'])

        if mode == 'filters':
            num_filters = random.choice(filter_options)
            dropout_rate = 0.5  # 固定
            num_hidden_nodes = 100  # 固定
        elif mode == 'dropout':
            num_filters = 20  # 固定
            dropout_rate = random.choice(dropout_options)
            num_hidden_nodes = 100  # 固定
        elif mode == 'hidden_nodes':
            num_filters = 20  # 固定
            dropout_rate = 0.5  # 固定
            num_hidden_nodes = random.choice(hidden_node_options)

        print(f"正在测试: filters={num_filters}, dropout={dropout_rate}, hidden_nodes={num_hidden_nodes}")
        accuracy, elapsed_time = train_and_evaluate(num_filters, dropout_rate, num_hidden_nodes, train_loader, test_loader)
        results.append((num_filters, dropout_rate, num_hidden_nodes, accuracy, elapsed_time))

    # 保存结果到 CSV
    df = pd.DataFrame(results, columns=['Filters', 'Dropout', 'HiddenNodes', 'Accuracy', 'Time'])
    df.to_csv('fashion_mnist_experiment_results.csv', index=False)
