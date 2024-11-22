import torch
from torchvision import transforms
from torch import nn
from PIL import Image
import os
import matplotlib.pyplot as plt
from my_network import MyNetwork
from greek_transform import GreekTransform


def main(model_path, handwritten_folder, classes):
    """
    使用训练好的模型对手写的希腊字母图像进行分类。

    Args:
        model_path (str): 训练好的模型文件路径。
        handwritten_folder (str): 手写图像文件夹路径。
        classes (list): 希腊字母类别列表。
    """
    model = MyNetwork()
    model.fc2 = nn.Linear(50, 3)  # 修改 fc2 层为 3 类
    model.load_state_dict(torch.load(model_path))
    model.eval()  # 设置为评估模式

    # 定义图像预处理
    transform = transforms.Compose([
        transforms.ToTensor(),
        GreekTransform(),
        transforms.Normalize((0.1307,), (0.3081,))
    ])

    # 获取手写图像文件
    image_files = [
        os.path.join(handwritten_folder, f)
        for f in os.listdir(handwritten_folder)
        if f.endswith(('.png', '.jpg', '.jpeg'))
    ]

    # 初始化子图
    num_images = len(image_files)
    cols = 3  # 每行显示 3 张图片
    rows = (num_images + cols - 1) // cols  # 计算总行数
    plt.figure(figsize=(15, 5 * rows))  # 设置画布大小

    # 遍历每个图像文件
    for idx, img_path in enumerate(image_files):
        # 打开并预处理图像
        image = Image.open(img_path)
        input_tensor = transform(image).unsqueeze(0) 

        # 模型预测
        with torch.no_grad():
            output = model(input_tensor)
            _, predicted = torch.max(output, 1)

        # 打印预测结果
        print(f"{img_path}：{classes[predicted.item()]}")

        # 在子图中显示图像和预测结果
        plt.subplot(rows, cols, idx + 1)
        plt.imshow(input_tensor.cpu().squeeze().numpy(), cmap="gray")
        plt.title(f"Prediction: {classes[predicted.item()]}")
        plt.axis("off")

    # 显示所有子图
    plt.tight_layout()
    plt.show()


if __name__ == "__main__":
    # 设置模型路径和图像文件夹路径
    model_path = "./data/greek_trained_model.pth"  # 训练好的模型文件
    handwritten_folder = "./data/greek_handwritten_processed"  # 手写图像文件夹路径
    classes = ["alpha", "beta", "gamma"]  # 定义希腊字母类别

    # 执行评估
    main(model_path, handwritten_folder, classes)