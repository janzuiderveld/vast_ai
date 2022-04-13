import Python-TCP-Image-Socket.utils
import argparse
import os

def main():
    parser = argparse.ArgumentParser(description='server job script')
    parser.add_argument('--input_image_filepath', type=str, default="Python-TCP-Image-Socket/8080_images0" help='image receive filepath')

    while True:
        input_filepath = utils.wait_for_file(args.input_image_filepath)
        print("New file found: " + str(input_filepath))

        # TODO This will boot script, a booted "server" waiting for new images would cut startup times SIGNIFICANTLY
        os.system(f'train.py --start_image "{input_filepath}" --prompts "a painting in the style of ... | Trending on artstation"')