import os
import sys
import numpy as np
import glob
import argparse
# import pretty_midi
import tempfile
import os
import time
import subprocess
import traceback

# def load_midi_fp(fp):
#   pretty_midi.pretty_midi.MAX_TICK = 1e16
#   with open(fp, "rb") as fh:
#       fp = fh.read()

#   # Load MIDI file
#   with tempfile.NamedTemporaryFile('wb') as mf:
#     mf.write(fp)
#     mf.seek(0)
#     print(mf.name)
#     midi = pretty_midi.PrettyMIDI(mf.name)
#     print(midi.instruments)
#     if len(midi.instruments) > 4:
#         del midi.instruments[4:]
#     if len(midi.instruments) < 4:
#         for i in range(4 - len(midi.instruments)):
#             midi.instruments.append(pretty_midi.Instrument(program=0))
#     midi.instruments[0].name = "p1"
#     midi.instruments[1].name = "p2"
#     midi.instruments[2].name = "tr"
#     midi.instruments[3].name = "no"

#     print(midi.instruments)
#     return midi

def wait_for_new_midi(midi_folder, pull=False):
    init_midis = glob.glob("{}/*.mid".format(midi_folder))
    print("Waiting for new *mid in {}".format(midi_folder))
    process = None
    while True:
        if pull:
            ID = os.environ["ID"]
            ROOT_DIR = os.environ["ROOT_DIR"]
            os.system(f"./vast copy {ID}:/workspace/vast_ai/midialogue/midi_out {ROOT_DIR}/midialogue")

            if process != None and process.poll() != None:
                command = ["./vast", "copy", args.midi_in_folder, f"{ID}:/workspace/vast_ai/midialogue"]
                process = subprocess.Popen(command, stdout=subprocess.PIPE, stderr=subprocess.PIPE)

        current_midis = glob.glob("{}/*.mid".format(midi_folder))
        if len(current_midis) > len(init_midis):
            new_midi = list(set(current_midis).symmetric_difference(set(init_midis)))[0]
            print("New midi found: {}".format(new_midi))

            return new_midi

        time.sleep(0.1)

def main(args):
    ID = os.environ["ID"]
    ROOT_DIR = os.environ["ROOT_DIR"]

    command = ["python3", "/home/p/vast_ai/midialogue/midi_scripts/light_pattern_ready.py"]
    process = subprocess.Popen(command, stdout=subprocess.PIPE, stderr=subprocess.PIPE)

    while True:
        try:
            # start ambient out
            os.system(f"echo 'starting ambient out'")
            # os.system(f"/home/p/vast_ai/midialogue/midi_scripts/ambient_out.py")
            command = ["python3", "/home/p/vast_ai/midialogue/midi_scripts/ambient_out.py"]
            ambient_process = subprocess.Popen(command, stdout=subprocess.PIPE, stderr=subprocess.PIPE)

            # wait for new midi being saved
            os.system(f"echo 'waiting for new midi in {args.midi_in_folder}'")
            saved_midi = wait_for_new_midi(args.midi_in_folder, pull=False)

            # send note off on note 10 teensy to stop input
            os.system(f"/home/p/vast_ai/midialogue/midi-utilities/bin/sendmidi --out {args.midi_in_port} --note-off 1 10 0")

            # copy midi to server
            os.system(f"echo 'copying {saved_midi} to {ID}:/workspace/vast_ai/midialogue'")
            command = ["./vast", "copy", args.midi_in_folder, f"{ID}:/workspace/vast_ai/midialogue"]
            process = subprocess.Popen(command, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
            
            # stop ambient out
            os.system(f"echo 'killing ambient out'")
            os.system(f"kill {ambient_process.pid}")

            # launch script for loading light pattern
            process = subprocess.Popen(["python3", "/home/p/vast_ai/midialogue/midi_scripts/light_pattern_load.py"])
            loading_pid = process.pid
            
            os.system(f"echo 'wating for new midi in {args.midi_out_folder}'")
            new_fp = wait_for_new_midi(args.midi_out_folder, pull=True)

            # kill script for light patterns
            os.system(f"kill {loading_pid}")

            # launch script for playing light pattern
            process = subprocess.Popen(["python3", "/home/p/vast_ai/midialogue/midi_scripts/light_pattern_play.py"])
            play_pid = process.pid

            os.system("echo playing {}".format(new_fp))
            # play new midi
            os.system(f"{ROOT_DIR}/midialogue/midi-utilities/bin/playsmf --out {args.midi_send_port} {new_fp}")

            os.system(f"echo 'killing light pattern play script'")
            # kill script for light patterns
            os.system(f"kill {play_pid}")

            # os.system(f"echo 'send note off to all channels'")
            # # send note off to all channels
            # _ = subprocess.Popen(["python3", "/home/p/vast_ai/midialogue/midi_scripts/note_off_all.py"])
            
            # Launch the process
            # process = subprocess.Popen(command)
            # input_PID = process.pid
            # os.environ["input_PID"] = str(input_PID)

            # print("checking if input is running")
            # poll = process.poll()
            # while poll is not None:
            #     # print error message
            #     print("Error in input script")
            #     print(process.stderr.read())
                
            #     # try again
            #     process = subprocess.Popen(command)
            #     input_PID = process.pid
            #     os.environ["input_PID"] = str(input_PID)

            #     time.sleep(0.5)

            #     poll = process.poll()

            # kill light pattern PIDs
            os.system("kill $(ps aux | grep 'light_pattern' | awk '{print $2}')")

            # launch script for playing light pattern
            os.system(f"echo 'launching light pattern after play'")
            os.system(f"python3 {ROOT_DIR}/midialogue/midi_scripts/light_pattern_after_play.py")

            # send note on on note 10 teensy to start input
            os.system(f"/home/p/vast_ai/midialogue/midi-utilities/bin/sendmidi --out {args.midi_in_port} --note-on 1 10 1")

        except Exception as e:
            print(traceback.format_exc())
            print(('Error on line {}'.format(sys.exc_info()[-1].tb_lineno), type(e).__name__, e))
            process.kill()
            break


if __name__ == "__main__":
  parser = argparse.ArgumentParser()
  parser.add_argument('--midi_out_folder', type=str, default='/home/p/vast_ai/midialogue/midi_out')
  parser.add_argument('--midi_in_folder', type=str, default='/home/p/vast_ai/midialogue/midi_in')

#   parser.add_argument('--midi_out_folder', type=str, default='/Users/janzuiderveld/Documents/GitHub/vast_ai/midialogue/midi_out')
  parser.add_argument('--midi_send_port', type=int, default=6)
  parser.add_argument('--midi_in_port', type=int, default=3)
  args = parser.parse_args()

  os.makedirs(args.midi_out_folder, exist_ok=True)
#   os.chdir("/Users/janzuiderveld/Documents/GitHub/vast_ai/midialogue/midi-utilities/bin")

  main(args)
