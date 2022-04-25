from collections import defaultdict
import os
import sys
import torch
import torch.nn.functional as F
import numpy as np
import glob
import argparse

def load_midi_fp(fp):
  pretty_midi.pretty_midi.MAX_TICK = 1e16
  with open(fp, "rb") as fh:
      fp = fh.read()

  # Load MIDI file
  with tempfile.NamedTemporaryFile('wb') as mf:
    mf.write(fp)
    mf.seek(0)
    print(mf.name)
    midi = pretty_midi.PrettyMIDI(mf.name)
    print(midi.instruments)
    if len(midi.instruments) > 4:
        del midi.instruments[4:]
    if len(midi.instruments) < 4:
        for i in range(4 - len(midi.instruments)):
            midi.instruments.append(pretty_midi.Instrument(program=0))
    midi.instruments[0].name = "p1"
    midi.instruments[1].name = "p2"
    midi.instruments[2].name = "tr"
    midi.instruments[3].name = "no"

    print(midi.instruments)
    return midi

def wait_for_new_midi(midi_folder):
    init_midis = glob.glob(f"{midi_folder}/*.mid")
    print(f"Waiting for new *mid in {midi_folder}")
    while True:
        # current_midis = glob.glob(f"{midi_folder}/*.mid")
        # if len(current_midis) > len(init_midis):
        #     new_midi = list(set(current_midis).symmetric_difference(set(init_midis)))[0]
        #     print(f"New midi found: {new_midi}")

        if 1:
            new_midi = init_midis[0]
            # try:
            #     load_midi_fp(new_midi)
                
            # except Exception as e:
            #     error = ('Error on line {}'.format(sys.exc_info()[-1].tb_lineno), type(e).__name__, e)
            #     print(f"Failed to load {new_midi}")
            #     return 0
                
            return new_midi

def main(args):
    while True:
        try:
            # wait for new midi
            new_fp = wait_for_new_midi(args.midi_in_folder)
            os.system(f"./playsmf --out {args.midi_out_port} {new_fp}")
        except Exception as e:
            print(('Error on line {}'.format(sys.exc_info()[-1].tb_lineno), type(e).__name__, e))
            break

if __name__ == "__main__":
  parser = argparse.ArgumentParser()
  parser.add_argument('--midi_in_folder', type=str, default='/Users/janzuiderveld/Documents/GitHub/vast_ai/midialogue/midi_out')
  parser.add_argument('--midi_out_port', type=int, default=0)
  args = parser.parse_args()

  os.makedirs(args.midi_in_folder, exist_ok=True)
  
  os.chdir("/Users/janzuiderveld/Documents/GitHub/vast_ai/midialogue/midi-utilities/bin")
  os.system(f"./lsmidiouts")  

  main(args)
