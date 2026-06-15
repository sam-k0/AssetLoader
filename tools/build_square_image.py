"""
Convert a horizontal spritesheet strip into a rectangular grid.

Reads IMAGECOUNT from a meta file.
"""

import math
import os
import re
from tkinter import Tk, filedialog

from PIL import Image


def select_file(title, filetypes):
    root = Tk()
    root.withdraw()
    return filedialog.askopenfilename(
        title=title,
        filetypes=filetypes
    )


def read_image_count(meta_file):
    with open(meta_file, "r", encoding="utf-8") as f:
        content = f.read()

    match = re.search(r"IMAGECOUNT\s*=\s*(\d+)", content)

    if not match:
        raise ValueError("Could not find IMAGECOUNT in meta file")

    return int(match.group(1))


def find_grid_dimensions(count):
    best_cols = 1
    best_rows = count
    best_diff = count

    for cols in range(1, int(math.sqrt(count)) + 1):
        if count % cols == 0:
            rows = count // cols
            diff = abs(rows - cols)

            if diff < best_diff:
                best_diff = diff
                best_cols = cols
                best_rows = rows

    # Return wider orientation
    return max(best_cols, best_rows), min(best_cols, best_rows)
def build_grid(sheet_path, meta_path, output_path):
    image_count = read_image_count(meta_path)

    strip = Image.open(sheet_path)

    strip_width, strip_height = strip.size

    if strip_width % image_count != 0:
        raise ValueError(
            f"Width {strip_width} not divisible by IMAGECOUNT {image_count}"
        )

    frame_width = strip_width // image_count
    frame_height = strip_height

    cols, rows = find_grid_dimensions(image_count)

    print(f"Grid: {cols} x {rows}")

    result = Image.new(
        strip.mode,
        (cols * frame_width, rows * frame_height)
    )

    for i in range(image_count):
        frame = strip.crop(
            (
                i * frame_width,
                0,
                (i + 1) * frame_width,
                frame_height
            )
        )

        x = (i % cols) * frame_width
        y = (i // cols) * frame_height

        result.paste(frame, (x, y))

    result.save(output_path)

    print(f"Saved: {output_path}")


sheet_path = select_file(
    "Select spritesheet strip",
    [
        ("Image files", "*.png *.jpg *.jpeg *.bmp *.gif"),
        ("All files", "*.*"),
    ],
)

if sheet_path:
    meta_path = select_file(
        "Select meta file",
        [
            ("Meta files", "*.meta *.txt"),
            ("All files", "*.*"),
        ],
    )

    if meta_path:
        output_path = filedialog.asksaveasfilename(
            defaultextension=".png",
            filetypes=[("PNG files", "*.png")]
        )

        if output_path:
            build_grid(sheet_path, meta_path, output_path)