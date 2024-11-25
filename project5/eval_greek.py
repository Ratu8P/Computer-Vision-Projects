import torch
from torchvision import transforms
from torch import nn
from PIL import Image
import os
import matplotlib.pyplot as plt
from my_network import MyNetwork
from greek_transform import GreekTransform


def main(model_path, handwritten_folder, classes):
    # load model
    model = MyNetwork()
    model.fc2 = nn.Linear(50, 3)  # change the output layer to 3 classes
    model.load_state_dict(torch.load(model_path))
    model.eval() 

    transform = transforms.Compose([
        transforms.ToTensor(),
        GreekTransform(),
        transforms.Normalize((0.1307,), (0.3081,))
    ])

    image_files = [
        os.path.join(handwritten_folder, f)
        for f in os.listdir(handwritten_folder)
        if f.endswith(('.png', '.jpg', '.jpeg'))
    ]

    # initialize the plot
    num_images = len(image_files)
    cols = 3  # 3 images per row
    rows = (num_images + cols - 1) // cols 
    plt.figure(figsize=(15, 5 * rows))

    for idx, img_path in enumerate(image_files):
        image = Image.open(img_path)
        input_tensor = transform(image).unsqueeze(0) 

        with torch.no_grad():
            output = model(input_tensor)
            _, predicted = torch.max(output, 1)
        print(f"{img_path}：{classes[predicted.item()]}")

        plt.subplot(rows, cols, idx + 1)
        plt.imshow(input_tensor.cpu().squeeze().numpy(), cmap="gray")
        plt.title(f"Prediction: {classes[predicted.item()]}")
        plt.axis("off")

    plt.tight_layout()
    plt.show()


if __name__ == "__main__":
    model_path = "./data/greek_trained_model.pth"  
    handwritten_folder = "./data/greek_handwritten_processed"  
    classes = ["alpha", "beta", "gamma"] 
    
    main(model_path, handwritten_folder, classes)