import torch
import cv2
import numpy as np
import matplotlib.pyplot as plt
from torchvision.datasets import MNIST
from torchvision.transforms import Compose, ToTensor, Normalize
from my_network import MyNetwork  # Replace with your model's class name

# Function to visualize both filters and their effect on an input image
def visualize_filters_and_effects(model_path, input_image):
    """
    Visualize filters from the first convolutional layer and their effects on the input image.
    """
    # Load the trained model
    model = MyNetwork()  # Replace with your model's class name
    model.load_state_dict(torch.load(model_path))
    model.eval()

    # Access the first convolutional layer's weights
    conv1_weights = model.conv1.weight.detach().numpy()  # Shape: [10, 1, 5, 5]

    # Convert the input image to numpy array and normalize it to match the model input
    input_image = input_image.squeeze().numpy()  # Assuming a single-channel input image

    # Prepare a combined plot for both filters and their effects
    num_filters = conv1_weights.shape[0]  # Number of filters (e.g., 10)
    plt.figure(figsize=(24, 15))

    for i, kernel in enumerate(conv1_weights):
        # Extract the filter weights for the i-th filter
        filter_weights = kernel[0]  # Shape: [5, 5]

        # Apply the filter using OpenCV's filter2D function
        filtered_image = cv2.filter2D(input_image, -1, filter_weights)

        # Plot the filter itself
        plt.subplot(num_filters, 4, 2 * i + 1)
        plt.imshow(filter_weights, cmap='gray')
        plt.title(f"Filter {i+1}")
        plt.axis('off')

        # Plot the effect of the filter on the input image
        plt.subplot(num_filters, 4, 2 * i + 2)
        plt.imshow(filtered_image, cmap='gray')
        plt.title(f"Filtered Output {i+1}")
        plt.axis('off')

    plt.suptitle("Conv1 Filters and Their Effects on Input Image", fontsize=16)
    plt.tight_layout()
    plt.show()

# Example usage
if __name__ == "__main__":
    # Path to the trained model
    trained_model_path = "./data/mnist_5.pth"  # Replace with actual path

    # Load an example training image from MNIST
    transform = Compose([ToTensor(), Normalize((0.1307,), (0.3081,))])
    train_set = MNIST(root='./data', train=True, transform=transform, download=True)
    first_image, _ = train_set[0]  # Load the first training image (tensor)

    # Visualize filters and their effects on the first image
    visualize_filters_and_effects(trained_model_path, first_image)
