import os
from PIL import Image
import torch
from torchvision import transforms
import matplotlib.pyplot as plt
from my_network import MyNetwork  # 替换为你的网络定义文件

# 定义加载图像的函数
def load_images(folder):
    """
    从文件夹中加载 PNG 图像并转换为 PyTorch 张量。
    """
    transform = transforms.Compose([
        transforms.ToTensor(),
        transforms.Normalize((0.1307,), (0.3081,))  # MNIST 的归一化参数
    ])

    images = []
    filenames = sorted(os.listdir(folder))  # 按文件名排序
    for filename in filenames:
        if filename.endswith(".png"):
            img_path = os.path.join(folder, filename)
            img = Image.open(img_path).convert("L")  # 确保是灰度图
            img_tensor = transform(img)
            images.append((img_tensor, filename))  # 返回张量和文件名

    return images

# 定义预测函数
def predict_images(model, images):
    """
    用深度学习模型对加载的图像进行预测。
    """
    model.eval()  # 设置为评估模式
    predictions = []
    with torch.no_grad():  # 禁用梯度计算
        for img_tensor, filename in images:
            img_tensor = img_tensor.unsqueeze(0)  # 增加 batch 维度
            output = model(img_tensor)  # 前向传播
            predicted_label = output.argmax(dim=1).item()  # 获取预测标签
            predictions.append((predicted_label, filename))
    return predictions

# 定义可视化函数
def plot_predictions(images, predictions):
    """
    可视化图像及其预测结果，每行显示3个图像。
    
    Args:
        images: List of image tensors (with optional filenames).
        predictions: List of tuples (predicted_label, confidence).
    """
    num_images = len(images)
    rows = (num_images + 2) // 3  # 每行3个，计算总行数
    fig, axes = plt.subplots(rows, 3, figsize=(15, 5 * rows))
    
    # Flatten axes array for easier iteration if there's more than one row
    axes = axes.flatten() if rows > 1 else [axes]
    
    for ax, (img_tensor, filename), (predicted_label, _) in zip(axes, images, predictions):
        img = img_tensor.squeeze().numpy()  # 将张量转换为 NumPy 数组
        ax.imshow(img, cmap="gray")
        ax.set_title(f"Pred: {predicted_label}")
        ax.axis("off")
    
    # Hide any unused axes
    for ax in axes[len(images):]:
        ax.axis("off")
    
    plt.tight_layout()
    plt.show()
# 加载模型
def load_model(model_path, model_class):
    """
    加载保存的模型权重。
    """
    model = model_class()
    model.load_state_dict(torch.load(model_path))
    return model

# 主函数
def main():
    input_folder = "./data/handwritten_processed"  # 手写图像文件夹
    model_path = "./data/mnist_5.pth"  # 已训练的模型路径

    # 替换为你的模型类
    model = load_model(model_path, MyNetwork)

    # 加载手写图像
    images = load_images(input_folder)

    # 用模型预测
    predictions = predict_images(model, images)

    # 可视化预测结果
    plot_predictions(images, predictions)

if __name__ == "__main__":
    main()
