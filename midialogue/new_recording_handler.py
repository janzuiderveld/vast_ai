import os
import sys
import numpy as np
import glob
import argparse
import pretty_midi
import tempfile

def wait_for_new_midi(midi_folder):
    init_midis = glob.glob("{}/*.mid".format(midi_folder))
    print("Waiting for new *mid in {}".format(midi_folder))
    while True:
        current_midis = glob.glob("{}/*.mid".format(midi_folder))
        if len(current_midis) > len(init_midis):
            new_midi = list(set(current_midis).symmetric_difference(set(init_midis)))[0]
            print("New midi found: {}".format(new_midi))

            return new_midi

def main(args):
    new_fp = wait_for_new_midi(args.midi_in_folder)
    
    # while True:
    #     try:
    #         # wait for new midi
            # new_fp = wait_for_new_midi(args.midi_in_folder)
    #         print(new_fp)

    #         ...

    #     except Exception as e:
    #         print(('Error on line {}'.format(sys.exc_info()[-1].tb_lineno), type(e).__name__, e))
    #         break

if __name__ == "__main__":
  parser = argparse.ArgumentParser()
  parser.add_argument('--midi_in_folder', type=str, default='midi_in')
  args = parser.parse_args()

  os.makedirs(args.midi_in_folder, exist_ok=True)

  main(args)
