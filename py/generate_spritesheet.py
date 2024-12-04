"""
Tool to generate a spritesheet by merging multiple images in a directory into a single image.
"""


import os
import re
from tkinter import Tk, filedialog
from PIL import Image

def select_directory():
    # Initialize tkinter root window (hidden)
    root = Tk()
    root.withdraw()  # Hide the root window
    directory_path = filedialog.askdirectory(title="Select the directory containing images")
    return directory_path

def numeric_sort_key(filename):
    # Extract the number from the filename using regular expression
    match = re.search(r'(\d+)', filename)
    if match:
        return int(match.group(1))  # Return the numeric part as an integer
    return 0  # Default to 0 if no number is found

def merge_images_in_directory(directory_path, output_path):
    # List all image files in the directory
    image_files = [f for f in os.listdir(directory_path) if f.lower().endswith(('png', 'jpg', 'jpeg', 'bmp', 'gif'))]

    if not image_files:
        print("No images found in the directory.")
        return

    # Sort the files based on the numeric part in the filename
    image_files.sort(key=numeric_sort_key)

    # Open the first image to get the width and height
    first_image = Image.open(os.path.join(directory_path, image_files[0]))
    width, height = first_image.size

    # Create a new image with the total width (sum of all image widths) and the same height
    total_width = width * len(image_files)
    result_image = Image.new('RGB', (total_width, height))

    # Paste each image into the resulting image
    x_offset = 0
    for image_file in image_files:
        img = Image.open(os.path.join(directory_path, image_file))
        result_image.paste(img, (x_offset, 0))
        x_offset += width

    # Save the result image
    result_image.save(output_path)
    print(f"IMAGECOUNT={len(image_files)}")
    print("You can copy and paste above line into the meta file")
    print(f"Merged image saved at {output_path}")

# Example usage
directory_path = select_directory()  # Open file dialog to select the directory
if directory_path:  # Check if a directory was selected
    output_path = filedialog.asksaveasfilename(defaultextension=".png", filetypes=[("PNG files", "*.png"), ("All files", "*.*")])
    if output_path:
        merge_images_in_directory(directory_path, output_path)
