import torchvision.transforms.functional as F

class GreekTransform:
    def __init__(self):
        pass

    def __call__(self, x):
        x = F.rgb_to_grayscale(x)
        x = F.affine(x, 0, (0, 0), 36 / 128, 0)
        x = F.center_crop(x, (28, 28))
        return F.invert(x)
