import torch
import matplotlib.pyplot as plt
from torchvision import datasets, transforms
from my_network import MyNetwork

transform = transforms.Compose([
    transforms.ToTensor(),
    transforms.Normalize((0.1307,), (0.3081,))
])

train_set = datasets.MNIST(root='./data', train=True, download=True, transform=transform)
test_set = datasets.MNIST(root='./data', train=False, download=True, transform=transform)

train_loader = torch.utils.data.DataLoader(train_set, batch_size=64, shuffle=True)
test_loader = torch.utils.data.DataLoader(test_set, batch_size=64, shuffle=False)

# Display the first 6 images from the training set
examples = enumerate(test_loader)
batch_idx, (example_data, example_targets) = next(examples)

fig, axes = plt.subplots(1, 6, figsize=(10, 2))
for i in range(6):
    axes[i].imshow(example_data[i][0], cmap='gray')
    axes[i].set_title(f"Label: {example_targets[i].item()}")
    axes[i].axis('off')

plt.show()

