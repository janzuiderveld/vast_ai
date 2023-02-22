import rtmidi
import signal
import random
from light_abstr import set_laser_mode, set_all_lasers_mode, x_by_x, all_on_off
import os
import sys

script_dir = os.path.dirname(__file__)

teensy = 1

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

try:
    # set all lasers to off
    set_all_lasers_mode(0, midi_out=midi_out)

    # flash 
    for i in range(2):
        all_on_off(step_sleep=0.2, random_sleep=False, midi_out=midi_out)
        i += 1
    
    # set all lasers to on
    set_all_lasers_mode(1, midi_out=midi_out)

except KeyboardInterrupt:
    print('Received KeyboardInterrupt signal. Exiting...')
    exit_pattern()
    sys.exit(0)
        