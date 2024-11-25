import pandas as pd
import matplotlib.pyplot as plt
import os  # 导入 os 模块以检查和创建目录

# Function to generate graphs from a CSV file using subplots
def generate_experiment_graphs(csv_file_path, save_path):
    # 确保保存路径存在
    os.makedirs(save_path, exist_ok=True)

    # Load data from the CSV file
    data = pd.read_csv(csv_file_path)

    # Get unique parameters
    parameters = data["Parameter"].unique()

    # Generate plots for each parameter
    for param in parameters:
        # Filter data for the current parameter
        param_data = data[data["Parameter"] == param]

        # Create subplots: one for accuracy and one for loss
        fig, axes = plt.subplots(2, 1, figsize=(10, 12))

        # Plot accuracy on the first subplot
        axes[0].plot(param_data["Value"], param_data["Final Train Accuracy"], label="Train Accuracy", marker='o')
        axes[0].plot(param_data["Value"], param_data["Final Test Accuracy"], label="Test Accuracy", marker='o')
        axes[0].set_title(f"Effect of {param} on Accuracy")
        axes[0].set_xlabel(f"{param} Value")
        axes[0].set_ylabel("Accuracy (%)")
        axes[0].legend()
        axes[0].grid()

        # Plot loss on the second subplot
        axes[1].plot(param_data["Value"], param_data["Final Train Loss"], label="Train Loss", marker='x')
        axes[1].plot(param_data["Value"], param_data["Final Test Loss"], label="Test Loss", marker='x')
        axes[1].set_title(f"Effect of {param} on Loss")
        axes[1].set_xlabel(f"{param} Value")
        axes[1].set_ylabel("Loss")
        axes[1].legend()
        axes[1].grid()

        # Adjust layout and save the figure
        plt.tight_layout()
        plt.savefig(os.path.join(save_path, f"{param}_experiment_results_subplot.png"))
        plt.show()

    print("Graphs have been generated and saved successfully.")

# Example usage
if __name__ == "__main__":
    generate_experiment_graphs(
        "./data/fashion_mnist_experiment_results_with_loss_accuracy.csv",
        "./data/experiment_results/"
    )
