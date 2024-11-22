import torch
from torchvision import datasets, transforms
from torch import optim, nn
import matplotlib.pyplot as plt
from greek_transform import GreekTransform
from my_network import MyNetwork


def test_model(model, test_loader):
    model.eval()
    correct = 0
    total = 0
    with torch.no_grad():
        for data, target in test_loader:
            output = model(data)
            _, predicted = torch.max(output, 1)
            correct += (predicted == target).sum().item()
            total += target.size(0)
    return correct / total

def train_network(model, train_loader, test_loader, epochs, optimizer, criterion):
    train_accuracies = []
    test_accuracies = []
    train_losses = []

    for epoch in range(epochs):
        model.train()
        train_loss = 0
        correct = 0
        total = 0

        for data, target in train_loader:
            optimizer.zero_grad()
            output = model(data)
            loss = criterion(output, target)
            loss.backward()
            optimizer.step()
            train_loss += loss.item()

            _, predicted = torch.max(output, 1)
            correct += (predicted == target).sum().item()
            total += target.size(0)

        train_accuracy = correct / total
        test_accuracy = test_model(model, test_loader)

        train_accuracies.append(train_accuracy)
        test_accuracies.append(test_accuracy)
        train_losses.append(train_loss / len(train_loader))

        print(f"Epoch {epoch + 1}/{epochs}, Loss: {train_loss / len(train_loader):.4f}, "
              f"Train Accuracy: {train_accuracy * 100:.2f}%, Test Accuracy: {test_accuracy * 100:.2f}%")

    plt.figure(figsize=(10, 5))
    plt.plot(train_accuracies, label="Train Accuracy")
    plt.plot(test_accuracies, label="Test Accuracy")
    plt.xlabel("Epoch")
    plt.ylabel("Accuracy")
    plt.legend()
    plt.title("Training and Testing Accuracy")
    plt.grid()
    plt.show()

    plt.figure(figsize=(10, 5))
    plt.plot(train_losses, label="Train Loss")
    plt.xlabel("Epoch")
    plt.ylabel("Loss")
    plt.legend()
    plt.title("Training Loss")
    plt.grid()
    plt.show()

def main():
    # # 定义设备为 CPU
    # device = torch.device("cpu")

    # 加载数据集
    greek_dataset = datasets.ImageFolder(
        root="./data/greek_train",
        transform=transforms.Compose([
            transforms.ToTensor(),
            GreekTransform(),
            transforms.Normalize((0.1307,), (0.3081,))
        ])
    )

    # 划分数据集为训练集和测试集 (80% 训练, 20% 测试)
    train_size = int(0.8 * len(greek_dataset))
    test_size = len(greek_dataset) - train_size
    train_dataset, test_dataset = torch.utils.data.random_split(greek_dataset, [train_size, test_size])

    # 创建 DataLoader
    train_loader = torch.utils.data.DataLoader(train_dataset, batch_size=5, shuffle=True)
    test_loader = torch.utils.data.DataLoader(test_dataset, batch_size=5, shuffle=False)

    model = MyNetwork()
    model.load_state_dict(torch.load("./data/mnist_5.pth"))

    # 冻结所有层参数
    for param in model.parameters():
        param.requires_grad = False

    # 替换最后一层分类器
    model.fc2 = nn.Linear(50, 3)  # 输出改为3个类

    # 定义优化器和损失函数
    optimizer = optim.Adam(model.fc2.parameters(), lr=0.001)  # 只优化最后一层
    criterion = nn.CrossEntropyLoss()

    # 训练模型
    train_network(model, train_loader, test_loader, epochs=40, optimizer=optimizer, criterion=criterion)

    # 保存训练后的模型
    torch.save(model.state_dict(), "./data/greek_trained_model.pth")
    print("Model saved as 'greek_trained_model.pth'")

if __name__ == "__main__":
    main()
