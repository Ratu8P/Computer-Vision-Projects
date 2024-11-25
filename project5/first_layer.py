import torch
import matplotlib.pyplot as plt
from my_network import MyNetwork  # Replace with the file where your network is defined

# Function to examine the network
def examine_network(model_path):
    """
    Examine a trained PyTorch network, visualize the filters,
    and display their shapes.
    """
    # Initialize the model architecture
    model = MyNetwork()  # Replace with the actual class name of your network
    model.load_state_dict(torch.load(model_path))  # Load weights into the model
    model.eval()  # Set the model to evaluation mode

    # Print the model structure
    print("Model Structure:")
    print(model)

    # Access the first convolutional layer's weights
    conv1_weights = model.conv1.weight  # Assuming the first layer is named 'conv1'
    
    # Print the shape of the weights
    print("\nShape of conv1 weights:", conv1_weights.shape)  # Expected: [10, 1, 5, 5]

    # Visualize the filters in conv1
    num_filters = conv1_weights.shape[0]
    plt.figure(figsize=(8, 6))
    
    for i in range(num_filters):
        plt.subplot(3, 4, i+1)
        filter_weights = conv1_weights[i, 0].detach().numpy()  # Detach from computation graph
        plt.imshow(filter_weights, cmap='gray')
        plt.title(f"Filter {i+1}")
        plt.axis('off')  # Remove axes for clarity

    plt.suptitle("Visualization of Conv1 Filters")
    plt.tight_layout()
    plt.show()

# Example usage
if __name__ == "__main__":
    # Provide the path to your saved model's state_dict
    trained_model_path = "./data/mnist_5.pth"  # Replace with your actual file path
    examine_network(trained_model_path)
