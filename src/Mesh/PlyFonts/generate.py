import os
import re
import sys

from fontTools.ttLib import TTFont
from fontTools.pens.freetypePen import FreeTypePen
from fontTools.pens.boundsPen import BoundsPen
from fontTools.misc.transform import Transform

from PIL import Image
import numpy as np

TARGET_WIDTH = 8
TARGET_HEIGHT = 16
REMOVE_DOT_RE = re.compile(r"\..*$")

def get_max_bounds(font):
    glyf_table = font["glyf"]
    glyph_set = font.getGlyphSet()
    cmap = font.getBestCmap()

    max_bounds = [0, 0, 0, 0]

    for glyph_name in cmap.values():
        if not glyf_table[glyph_name].isComposite():
            continue
        pen = BoundsPen(glyph_set)
        glyph_set[glyph_name].draw(pen)

        xmin, ymin, xmax, ymax = pen.bounds

        max_bounds[0] = min(max_bounds[0], xmin)
        max_bounds[1] = min(max_bounds[1], ymin)
        max_bounds[2] = max(max_bounds[2], xmax)
        max_bounds[3] = max(max_bounds[3], ymax)

    return tuple(max_bounds)


def byte_array_to_int(arr: bytes, size: tuple[int, int]) -> list[list[int]]:
    result: list[list[int]] = []

    for y in range(size[1]):
        line: list[int] = []
        for x in range(size[0]):
            line.append(arr[x + y * size[0]])
        result.append(line)

    return result


def get_matrix_by_ord(font_path: str) -> dict[int, list[list[int]]]:
    result = {}

    with TTFont(font_path) as font:
        cmap = font.getBestCmap()

        glyph_set = font.getGlyphSet()

        xmin, ymin, xmax, ymax = get_max_bounds(font)

        w = xmax - xmin
        h = ymax - ymin

        ratio = min(TARGET_WIDTH / w, TARGET_HEIGHT / h)

        transform = Transform().scale(ratio, ratio).translate(0, h / 4)

        for glyph_ord, glyph_name in cmap.items():
            if glyph_ord > 255:
                continue

            pen = FreeTypePen(glyph_set)
            glyph_set[glyph_name].draw(pen)

            colors, size = pen.buffer(
                width=TARGET_WIDTH, height=TARGET_HEIGHT, transform=transform
            )
            result[glyph_ord] = byte_array_to_int(colors, size)
    return result


def raster_to_stdout(raster: dict[int, list[list[bool]]]):
    for char, m in raster.items():
        print(f"--- {char} ({chr(char)}) ---")

        for line in m:
            for b in line:
                if b:
                    print("█", end="")
                else:
                    print(" ", end="")
            print()

def raster_dict_to_image(raster: dict[int, list[list[int]]]):
    width = len(list(raster.values())[0][0])
    height = len(list(raster.values())[0])

    output = np.zeros((height * 0x8, width * 0x10), dtype=np.uint8)

    for c, m in raster.items():
        if c & 0x80:
            continue

        x = (c & 0x0f) * width
        y = ((c & 0x70) >> 4) * height

        for i in range(height):
            for j in range(width):
                output[y + i, x + j] = m[i][j]

    return output



def main():
    args = sys.argv[1:]
    if len(args) < 2:
        print('Usage: generate.py <font file> <output file>')
        return

    font = args[0]
    output = args[1]
    debug = os.getenv("SHOW_FONT_RASTER") != None

    rasterized = get_matrix_by_ord(font)
    if debug:
        raster_to_stdout(rasterized)
        return

    v = raster_dict_to_image(rasterized)
    i = Image.fromarray(v, mode='L')
    with open(output, 'wb') as f:
        i.save(f, "png")
    

if __name__ == "__main__":
    main()
