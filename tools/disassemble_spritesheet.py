"""
Tool to split a spritesheet into individual images using IMAGECOUNT from a meta file.
"""

import os
import re
from tkinter import Tk, filedialog
from PIL import Image


def select_file(title, filetypes):
    root = Tk()
    root.withdraw()
    return filedialog.askopenfilename(title=title, filetypes=filetypes)


def read_image_count(meta_file):
    with open(meta_file, "r", encoding="utf-8") as f:
        content = f.read()

    match = re.search(r"IMAGECOUNT\s*=\s*(\d+)", content)

    if not match:
        raise ValueError("Could not find IMAGECOUNT in meta file")

    return int(match.group(1))


def split_spritesheet(image_path, meta_path):
    image_count = read_image_count(meta_path)

    sheet = Image.open(image_path)

    sheet_width, sheet_height = sheet.size

    if sheet_width % image_count != 0:
        raise ValueError(
            f"Image width ({sheet_width}) is not divisible by IMAGECOUNT ({image_count})"
        )

    frame_width = sheet_width // image_count

    sheet_name = os.path.splitext(os.path.basename(image_path))[0]
    output_dir = os.path.join(os.path.dirname(image_path), sheet_name)

    os.makedirs(output_dir, exist_ok=True)

    SCALE = 4


    for i in range(image_count):
        left = i * frame_width
        right = left + frame_width

        frame = sheet.crop((left, 0, right, sheet_height))

        # Upscale
        upscaled = frame.resize(
            (frame.width * SCALE, frame.height * SCALE),
            Image.Resampling.NEAREST  # preserves pixel art
        )

        output_file = os.path.join(output_dir, f"{i}.png")
        upscaled.save(output_file)

        print(f"Split {i} images")
        print(f"Output directory: {output_dir}")





image_path = select_file(
    "Select spritesheet image",
    [
        ("Image files", "*.png *.jpg *.jpeg *.bmp *.gif"),
        ("All files", "*.*"),
    ],
)

if image_path:
    meta_path = select_file(
        "Select meta file",
        [
            ("Text files", "*.txt"),
            ("Meta files", "*.meta"),
            ("All files", "*.*"),
        ],
    )

    if meta_path:
        split_spritesheet(image_path, meta_path)