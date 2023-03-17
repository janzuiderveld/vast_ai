# # bin/python3
import subprocess
import os
import random
import rtmidi
import time
import signal
import sys
# pip install python-rtmidi

script_dir = os.path.dirname(__file__)

model_port = int(sys.argv[1])
timeout = int(sys.argv[2])
verbose = True

# ONMIDI
# onmidi runs programs or scripts in response to MIDI input. It's particularly useful for turning pages in on-screen sheet music.
# Usage: onmidi --in <port> [ --out <port> ] [ --hold-length <msecs> ] [ --note-command <note> <command> | --controller-command <controller number> <command> | --controller-hold-command <controller number> <command> | --pitch-wheel-up-command <command> | --pitch-wheel-down-command <command> ]

def get_ports_arec():
        str = check_output([arec, '-l'], universal_newlines=True)
        res = [line.split()[0] for line in str.split('\n') if 'MIDI' in line]
        return res

midi_in = rtmidi.MidiIn()
print(midi_in.get_ports())  # ['Arturia MiniLab mkII 0', 'loopMIDI Port 1']
midi_in.open_port(model_port)

try:
    ROOT_DIR = os.environ["ROOT_DIR"]
except:
    ROOT_DIR = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))

os.makedirs(f"{ROOT_DIR}/midialogue/midi_in", exist_ok=True)
os.makedirs(f"{ROOT_DIR}/midialogue/midi_in/midi_in_unfinished", exist_ok=True)
os.makedirs(f"{ROOT_DIR}/midialogue/midi_out", exist_ok=True)

def handle_input(event, data=None):
    # message, deltatime = event
    # print(f'message: {message}, deltatime: {deltatime}, data: {data}')

    global recording
    
    # check if timer is still running
    with open(f"{script_dir}/countdown.txt", "r") as f:
        count = f.read()

        if count == "":
             recording = True
        else:
            time = int(count)
            if time == 0:
                recording = False

    if not recording:
        recording = True
        print("Start recording")
        os.system(f"echo {timeout} > {script_dir}/countdown.txt")
        fn = f"{ROOT_DIR}/midialogue/midi_in/midi_in_unfinished/{random.getrandbits(64)}.mid"
        script = [f"arecordmidi --port 14:0 {fn}"]

        process = subprocess.Popen(script, shell=True)
        
        # start countdown timer
        os.system(f"bash {script_dir}/countdown.sh {timeout} 'kill {process.pid}'")
        os.system(f"mv {fn} {ROOT_DIR}/midialogue/midi_in")

        # start countdown timer # TO REPLACE =============================
        # os.system("bash {script_dir}/countdown.sh {timeout} 'kill {process.pid}' 'mv {fn} {ROOT_DIR}/midialogue/midi_in' &")
        
        subprocess.Popen(["bash", f"{script_dir}/countdown.sh", f"{timeout}", f"'kill {process.pid}'", f"'mv {fn} {ROOT_DIR}/midialogue/midi_in'"])
        ##################################################################

    else:
        # reset countdown timer
        os.system(f"echo {timeout} > {script_dir}/countdown.txt")


def handle_input2(event, data=None):
    # set last_input to current time
    global rec_proces
    global last_input
    global recording
    global fn

    last_input = time.time()

    # start recording if not already recording
    if not recording:
        recording = True
        print("Start recording")
        fn = f"{ROOT_DIR}/midialogue/midi_in/midi_in_unfinished/{random.getrandbits(64)}.mid"
        script = [f"arecordmidi", "--port", "14:0", f"{fn}"]
        rec_proces = subprocess.Popen(script, shell=False)

midi_in.set_callback(handle_input2)

recording = False
last_input = None
rec_proces = None

while True:
    try:
        if last_input is not None:
            if time.time() - last_input > timeout:
                # stop recording
                print("Stop recording")
                # rec_proces.kill()
                os.kill(rec_proces.pid, signal.SIGTERM)
                recording = False
                last_input = None

                # process midi file
                # os.system(f"python3 {ROOT_DIR}/midialogue/midi_scripts/midi_process.py {fn}")

                os.system(f"mv {fn} {ROOT_DIR}/midialogue/midi_in")
        
            elif verbose:
                print(f"Stopping recording in {timeout - (time.time() - last_input)} seconds")

        time.sleep(1)
    except KeyboardInterrupt:
        print('')
        break