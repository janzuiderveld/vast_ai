import rtmidi
import signal
import random
from light_abstr import set_laser_mode, set_all_lasers_mode, x_by_x, all_on_off, in_port
import os
import sys
import time

script_dir = os.path.dirname(__file__)

teensy = in_port

# set up midi out
midi_out = rtmidi.MidiOut()
midi_out.open_port(teensy)

def exit_pattern():
    set_all_lasers_mode(1, midi_out=midi_out)

def sig_exit(sig, frame):
    print('Received SIGTERM signal. Exiting...')
    exit_pattern()
    sys.exit(0)

signal.signal(signal.SIGTERM, sig_exit)

# read sys.argv[1] to determine which laser to flash
laser_num = int(sys.argv[1])

for i in range(5):
    try:
        # set laser to off
        set_laser_mode(laser_num, 0, midi_out=midi_out)
        
        time.sleep(0.3)

        # set laser to on
        set_laser_mode(laser_num, 1, midi_out=midi_out)

        time.sleep(0.3)

    except KeyboardInterrupt:
        print('Received KeyboardInterrupt signal. Exiting...')
        exit_pattern()
        sys.exit(0)
            