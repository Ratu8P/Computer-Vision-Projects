import os
from PIL import Image
import torch
from torchvision import transforms
import matplotlib.pyplot as plt
from my_network import MyNetwork

def load_images(folder):
    
    transform = transforms.Compose([
        transforms.ToTensor(),
        transforms.Normalize((0.1307,), (0.3081,))
    ])

    images = []
    filenames = sorted(os.listdir(folder))
    for filename in filenames:
        if filename.endswith(".png"):
            img_path = os.path.join(folder, filename)
            img = Image.open(img_path).convert("L")
            img_tensor = transform(img)
            images.append((img_tensor, filename))
    return images

# define a function to predict images
def predict_images(model, images):
    model.eval()
    predictions = []
    with torch.no_grad():
        for img_tensor, filename in images:
            img_tensor = img_tensor.unsqueeze(0) 
            output = model(img_tensor)
            predicted_label = output.argmax(dim=1).item()
            predictions.append((predicted_label, filename))
    return predictions

# visualize the predictions
def plot_predictions(images, predictions):
    num_images = len(images)
    rows = (num_images + 2) // 3 
    fig, axes = plt.subplots(rows, 3, figsize=(15, 5 * rows))
    
    # Flatten axes array for easier iteration if there's more than one row
    axes = axes.flatten() if rows > 1 else [axes]
    
    for ax, (img_tensor, filename), (predicted_label, _) in zip(axes, images, predictions):
        img = img_tensor.squeeze().numpy()
        ax.imshow(img, cmap="gray")
        ax.set_title(f"Pred: {predicted_label}")
        ax.axis("off")
    
    # Hide any unused axes
    for ax in axes[len(images):]:
        ax.axis("off")
    
    plt.tight_layout()
    plt.show()

def load_model(model_path, model_class):
    model = model_class()
    model.load_state_dict(torch.load(model_path))
    return model

if __name__ == "__main__":
    input_folder = "./data/handwritten_processed"  
    model_path = "./data/mnist_5.pth" 
    model = load_model(model_path, MyNetwork)
    images = load_images(input_folder)
    predictions = predict_images(model, images)

    plot_predictions(images, predictions)
