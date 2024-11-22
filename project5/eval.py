import torch
from torchvision import datasets, transforms
import matplotlib.pyplot as plt
import numpy as np

# Import your trained model class
from my_network import MyNetwork  # Replace with your actual network file

model_path = "./data/mnist_5.pth"

# Load the trained model
def load_model(model_class, path):
    model = model_class()
    model.load_state_dict(torch.load(path))
    model.eval()  # Set to evaluation mode
    return model

# Plotting helper function
def plot_predictions(images, predictions, result):
    """
    Plots the first 9 images in a 3x3 grid with predictions above each image.
    """
    fig, axes = plt.subplots(3, 3, figsize=(8, 8))
    for i, ax in enumerate(axes.flat):
        if i >= len(images):
            break
        img = images[i].squeeze()  # Remove channel dimension
        ax.imshow(img, cmap="gray")
        ax.set_title(f"Pred: {predictions[i]}\nResult: {result[i]}")
        ax.axis("off")
    plt.tight_layout()
    plt.show()

# Main function
def main():
    # Load the trained network
    model = load_model(MyNetwork, model_path)
    
    # Load the test dataset
    transform = transforms.Compose([transforms.ToTensor(), transforms.Normalize((0.1307,), (0.3081,))])
    test_dataset = datasets.MNIST(root='./data', train=False, download=True, transform=transform)
    test_loader = torch.utils.data.DataLoader(test_dataset, batch_size=10, shuffle=False)

    # Get the first 10 test examples
    images, results = next(iter(test_loader))

    # Perform inference
    with torch.no_grad():  # Disable gradient computation
        outputs = model(images)
        predictions = outputs.argmax(dim=1).tolist()

    # Print results
    print(f"{'Image Index':<12}{'Network Outputs':<60}{'Prediction':<12}{'Result'}")
    for i, (output, pred, label) in enumerate(zip(outputs, predictions, results)):
        output_str = ", ".join(f"{val:.2f}" for val in output.tolist())
        print(f"{i:<12}{output_str:<60}{pred:<12}{label.item()}")

    # Plot the first 9 test images with predictions
    plot_predictions(images[:9], predictions[:9], results[:9])

if __name__ == "__main__":
    main()
