from PIL import Image, ImageOps
import os

def main(input_folder, output_folder):
    """
    Preprocess JPEG images in the input folder:
    - Convert to grayscale
    - Crop and resize to 28x28
    - Invert colors (black background, white digits)
    - Save the processed images with sequential names (img_1.png, img_2.png, ...)
    """
    if not os.path.exists(output_folder):
        os.makedirs(output_folder)
    
    files = [f for f in os.listdir(input_folder) if f.endswith(".png")]
    for idx, filename in enumerate(sorted(files), start=1):
        img_path = os.path.join(input_folder, filename)
        img = Image.open(img_path).convert("L")  # Convert to grayscale

        # Resize to 28x28 using LANCZOS resampling
        resized_img = ImageOps.fit(img, (133, 133), Image.Resampling.LANCZOS)
        inverted_img = ImageOps.invert(resized_img)  # Invert colors

        # Save the new image
        output_path = os.path.join(output_folder, f"img_{idx}.png")
        resized_img.save(output_path)
        print(f"Processed and saved: {output_path}")


if __name__ == "__main__":
    input_folder = "./data/greek_handwritten"  
    output_folder = "./data/greek_handwritten_processed" 
    main(input_folder, output_folder)
