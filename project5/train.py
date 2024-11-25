import torch
from torchvision import datasets, transforms
from torch import optim, nn
from my_network import MyNetwork
import matplotlib.pyplot as plt


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
    accuracy = correct / total
    return accuracy

def train_network(model, train_loader, test_loader, epochs, optimizer, criterion):
    train_losses = []
    test_losses = []
    train_accuracies = []
    test_accuracies = []
    training_examples_seen = []  # 记录每次累积的样本数
    num_seen = 0  # 当前累积的训练样本数

    for epoch in range(epochs):
        model.train()
        epoch_loss = 0
        correct_train = 0
        total_train = 0

        for data, target in train_loader:
            # 前向传播
            optimizer.zero_grad()
            output = model(data)
            loss = criterion(output, target)
            loss.backward()
            optimizer.step()

            # 记录损失
            epoch_loss += loss.item()
            _, predicted = torch.max(output, 1)
            correct_train += (predicted == target).sum().item()
            total_train += target.size(0)

            # 累积样本数
            num_seen += data.size(0)
            training_examples_seen.append(num_seen)
            train_losses.append(loss.item())

        # 计算训练准确率
        train_accuracy = correct_train / total_train
        train_accuracies.append(train_accuracy)

        # 计算测试损失和准确率
        model.eval()
        test_loss = 0
        correct_test = 0
        total_test = 0
        with torch.no_grad():
            for data, target in test_loader:
                output = model(data)
                test_loss += criterion(output, target).item()
                _, predicted = torch.max(output, 1)
                correct_test += (predicted == target).sum().item()
                total_test += target.size(0)

        test_loss /= len(test_loader)
        test_accuracy = correct_test / total_test

        test_losses.append(test_loss)
        test_accuracies.append(test_accuracy)

        print(f'Epoch {epoch + 1}: Training Loss = {epoch_loss / len(train_loader):.4f}, '
              f'Training Accuracy = {train_accuracy * 100:.2f}%, '
              f'Test Loss = {test_loss:.4f}, Test Accuracy = {test_accuracy * 100:.2f}%')

    # 生成训练和测试损失图
    plt.figure(figsize=(10, 6))
    plt.plot(training_examples_seen, train_losses, label='Train loss', color='blue', linewidth=1)
    plt.scatter(
        [training_examples_seen[i * len(train_loader) - 1] for i in range(1, epochs + 1)],
        test_losses,
        label='Test loss',
        color='red'
    )
    plt.xlabel("Number of training examples seen")
    plt.ylabel("Negative Log Likelihood Loss")
    plt.title("Training and Test Loss over Training Examples")
    plt.legend()
    plt.grid()
    plt.show()

    # 生成训练和测试准确率图
    plt.figure(figsize=(10, 6))
    plt.plot(range(epochs), train_accuracies, label='Train Accuracy', color='blue', linewidth=1)
    plt.plot(range(epochs), test_accuracies, label='Test Accuracy', color='orange', linewidth=1)
    plt.xlabel("Epochs")
    plt.ylabel("Accuracy")
    plt.title("Training and Testing Accuracy")
    plt.legend()
    plt.grid()
    plt.show()

def main():
    transform = transforms.Compose([
        transforms.ToTensor(),
        transforms.Normalize((0.1307,), (0.3081,))
    ])
    
    train_set = datasets.MNIST(root='./data', train=True, download=True, transform=transform)
    test_set = datasets.MNIST(root='./data', train=False, download=True, transform=transform)
    
    train_loader = torch.utils.data.DataLoader(train_set, batch_size=64, shuffle=True)
    test_loader = torch.utils.data.DataLoader(test_set, batch_size=64, shuffle=False)
    
    model = MyNetwork()
    optimizer = optim.SGD(model.parameters(), lr=0.01, momentum=0.9)
    criterion = nn.CrossEntropyLoss()
    
    train_network(model, train_loader, test_loader, epochs=10, optimizer=optimizer, criterion=criterion)
    torch.save(model.state_dict(), "./data/mnist_5.pth")

if __name__ == "__main__":
    main()
