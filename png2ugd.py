#!/usr/bin/env python
"""
png2udg.py

Copyright (C) 2018 Jordi Sesmero
Based on the original idea from Juan J. Martinez
Uses UDG format z88dk SP1: ZXSpectrumZSDCCnewlib_SP1_04_BiggerSprites.md

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.

"""
__version__ = "1.0.1"

from argparse import ArgumentParser
from PIL import Image
from os.path import basename
import sys

INK = (255, 255, 255)
PAPER = (205, 0, 0)
MASK = (0, 0, 0)

COLORS = [INK, PAPER, MASK,]


def get_value(rgb, animated=False):
    """
    Obtains pixel value if it is enabled. Color is not supported yet here.
    :param rgb:
    :param animated: It is a WTF from SP1. If I do animation it requires me to invert the values...
    :return:
    """

    if rgb[0] > 100 and rgb[1] > 100 and rgb[2] > 100:
        return "1" if not animated else "0"
    else:
        return "0" if not animated else "1"


def do_formatted(column, asm=False):
    """
    Generates binary formatted value from column array
    :param column:
    :return:
    """
    if not asm:
        return hex(int('{}'.format(''.join(column)), 2))
    else:
        return '@{}'.format(''.join(column))


def main():
    animated = False

    parser = ArgumentParser(description="png2sp1sprite",
                            epilog="Copyright (C) 2018 Jordi Sesmero",
                            )

    parser.add_argument("--version", action="version", version="%(prog)s "  + __version__)
    parser.add_argument("-i", "--id", dest="id", default="sprite", type=str,
                        help="variable name (default: sprite)")

    parser.add_argument("-a", "--asm", dest="asm",
                        help="generate asm file", action='store_true', default=False)

    parser.add_argument("image", help="image to convert", nargs="?")

    args = parser.parse_args()

    if not args.image:
        parser.error("required parameter: image")
    asm = args.asm
    name = basename(args.image).split('.')[0]

    try:
        image = Image.open(args.image)
    except IOError:
        parser.error(f"failed to open the image {args.image}")
        exit(1)

    (w, h) = image.size

    if (w % 8) != 0 or (h % 8) != 0:
        parser.error("%r size should be 8x8 8 but it is %sx%s" % (args.image, w, h))

    rows = []
    blocks = []
    fmt = ""
    print("SIZE: ", w, h, file=sys.stderr)
    for bloque in range(0, w, 8):
        for hy in range(0, h, 8):
            for y in range(hy, hy + 8):
                col = []
                # vamos al bloque de 8 que toca (ej: 0 al 8, 8 al 16, 16 al 24, 24 al 32)
    
                for x in range(bloque, bloque + 8):
                    pixel = image.getpixel((x, y))
                    v = get_value(pixel, animated=False)
                    col.append(v)
                rows.append(do_formatted(col, asm=asm))
    
            print("BLOCK: ", bloque, hy, file=sys.stderr)
        blocks.append(rows)
    
    udgs = ["{}".format(', '.join(udg)) for udg in blocks]
    fmt = "{}".format(', '.join(udgs))

    if not asm:
        print("// len(ugds) == ", sum(len(ugd) for ugd in blocks))
        print("const uint8_t " + args.id + "[] = {" + fmt + "};")
    else:
        print("SECTION rodata_user")
        print("PUBLIC _{}".format(args.id.replace('-', "_")))
        print("._{}".format(args.id.replace('-', "_")))

        print("defb {}".format(fmt))

if __name__ == "__main__":
    main()
