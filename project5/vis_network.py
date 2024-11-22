from torchviz import make_dot
from my_network import MyNetwork
import torch

model = MyNetwork()
# 创建一个输入张量
dummy_input = torch.randn(1, 1, 28, 28)

# 前向传播输出
output = model(dummy_input)

# 可视化网络
graph = make_dot(output, params=dict(model.named_parameters()))
graph.render("network_structure", format="png")  # 保存为 PNG 格式
