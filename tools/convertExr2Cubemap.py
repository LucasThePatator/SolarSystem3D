import PIL
import numpy as np
from PIL import Image
import py360convert
from argparse import ArgumentParser
import OpenEXR
from matplotlib import pyplot as plt

if __name__ == "__main__":
    arg_parser = ArgumentParser(description="Converts an Exr file to Cubemap")
    arg_parser.add_argument("input", help="Input Exr file", type=str)
    arg_parser.add_argument("output", help="Output Cubemap file", type=str)
    args = arg_parser.parse_args()

    print("Reading input file...")
    with OpenEXR.File(args.input) as infile:
        header = infile.header()
        print(f"type={header['type']}")
        print(f"compression={header['compression']}")

        RGB = infile.channels()["RGB"].pixels

    height, width = RGB.shape[0:2]
    print(f"height={height}; width={width}")
    gamma = 1
    RGB = (np.pow(RGB, 1 / gamma) * 255).astype("uint8")

    # You can make conversion between supported cubemap format
    cube = py360convert.e2c(RGB, face_w=2048)
    PIL.Image.fromarray(cube).save(args.output)
