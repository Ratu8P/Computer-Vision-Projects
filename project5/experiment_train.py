import torchvision
import torchvision.transforms as transforms
from torch.utils.data import DataLoader
import torch.optim as optim
import pandas as pd
import time
from experiment_network import MyNetwork
import torch.nn as nn
import torch
import matplotlib.pyplot as plt
import os

def load_data():
    transform = transforms.Compose([
        transforms.ToTensor(),
        transforms.Normalize((0.5,), (0.5,))
    ])
    train_set = torchvision.datasets.FashionMNIST(root="./data", train=True, download=True, transform=transform)
    test_set = torchvision.datasets.FashionMNIST(root="./data", train=False, download=True, transform=transform)
    return DataLoader(train_set, batch_size=32, shuffle=True), DataLoader(test_set, batch_size=32, shuffle=False)

# 训练与评估
def train_and_evaluate(train_loader, test_loader, conv1_filters, conv2_filters, dropout_rate, fc1_nodes, epochs, device):
    model = MyNetwork(conv1_filters=conv1_filters, conv2_filters=conv2_filters, dropout_rate=dropout_rate, fc1_nodes=fc1_nodes)
    model = model.to(device)

    criterion = nn.CrossEntropyLoss()
    optimizer = optim.SGD(model.parameters(), lr=0.01, momentum=0.9)

    # 存储训练和测试过程中的指标
    train_losses = []
    train_accuracies = []
    test_losses = []
    test_accuracies = []

    start_time = time.time()

    for epoch in range(epochs):
        model.train()
        running_loss = 0.0
        correct_train = 0
        total_train = 0

        # 训练
        for inputs, labels in train_loader:
            inputs, labels = inputs.to(device), labels.to(device)
            optimizer.zero_grad()
            outputs = model(inputs)
            loss = criterion(outputs, labels)
            loss.backward()
            optimizer.step()

            running_loss += loss.item()
            _, predicted = torch.max(outputs, 1)
            correct_train += (predicted == labels).sum().item()
            total_train += labels.size(0)

        train_loss = running_loss / len(train_loader)
        train_accuracy = 100 * correct_train / total_train
        train_losses.append(train_loss)
        train_accuracies.append(train_accuracy)

        # 测试
        model.eval()
        correct_test = 0
        total_test = 0
        test_loss = 0.0
        with torch.no_grad():
            for inputs, labels in test_loader:
                inputs, labels = inputs.to(device), labels.to(device)
                outputs = model(inputs)
                loss = criterion(outputs, labels)
                test_loss += loss.item()
                _, predicted = torch.max(outputs, 1)
                total_test += labels.size(0)
                correct_test += (predicted == labels).sum().item()

        test_loss /= len(test_loader)
        test_accuracy = 100 * correct_test / total_test
        test_losses.append(test_loss)
        test_accuracies.append(test_accuracy)

        print(f"Epoch {epoch + 1}/{epochs} - Train Loss: {train_loss:.4f}, Train Acc: {train_accuracy:.2f}%, "
              f"Test Loss: {test_loss:.4f}, Test Acc: {test_accuracy:.2f}%")

    elapsed_time = time.time() - start_time
    return train_losses, train_accuracies, test_losses, test_accuracies, elapsed_time

# 主程序
if __name__ == "__main__":
    device = torch.device("cuda" if torch.cuda.is_available() else "cpu")
    print(f"Using device: {device}")

    train_loader, test_loader = load_data()

    fixed_conv1_filters = 10
    fixed_conv2_filters = 20
    fixed_dropout_rate = 0.5
    fixed_fc1_nodes = 50
    fixed_epochs = 5
    num_tests = 10

    results = []

    experiments = [
        {"param": "conv1_filters", "start": 5, "step": 5},
        {"param": "conv2_filters", "start": 10, "step": 10},
        {"param": "dropout_rate", "start": 0.1, "step": 0.1},
        {"param": "fc1_nodes", "start": 50, "step": 50},
        {"param": "epochs", "start": 1, "step": 1},
    ]

    for exp in experiments:
        param_name = exp["param"]
        start, step = exp["start"], exp["step"]
        for i in range(num_tests):
            value = start + i * step

            params = {
                "conv1_filters": fixed_conv1_filters,
                "conv2_filters": fixed_conv2_filters,
                "dropout_rate": fixed_dropout_rate,
                "fc1_nodes": fixed_fc1_nodes,
                "epochs": fixed_epochs,
                param_name: value,
            }

            train_losses, train_accuracies, test_losses, test_accuracies, elapsed_time = train_and_evaluate(
                train_loader, test_loader,
                conv1_filters=params["conv1_filters"],
                conv2_filters=params["conv2_filters"],
                dropout_rate=params["dropout_rate"],
                fc1_nodes=params["fc1_nodes"],
                epochs=params["epochs"],
                device=device
            )

            print(f"Testing {param_name}: {value} -> Final Test Acc: {test_accuracies[-1]:.2f}%, Time: {elapsed_time:.2f}s")
            results.append({
                "Parameter": param_name,
                "Value": value,
                "Final Train Loss": train_losses[-1],
                "Final Train Accuracy": train_accuracies[-1],
                "Final Test Loss": test_losses[-1],
                "Final Test Accuracy": test_accuracies[-1],
                "Time": elapsed_time
            })

    df = pd.DataFrame(results)
    df.to_csv("fashion_mnist_experiment_results_with_loss_accuracy.csv", index=False)
    print("Results saved to 'fashion_mnist_experiment_results_with_loss_accuracy.csv'")