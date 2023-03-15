import rtmidi
import signal
import random
from light_abstr import set_laser_mode, set_all_lasers_mode, x_by_x, all_on_off, in_port
import os
import sys

script_dir = os.path.dirname(__file__)

teensy = in_port

# set up midi out
midi_out = rtmidi.MidiOut()
midi_out.open_port(teensy)

def exit_pattern():
    set_all_lasers_mode(0, midi_out=midi_out)

def sig_exit(sig, frame):
    print('Received SIGTERM signal. Exiting...')
    exit_pattern()
    sys.exit(0)

signal.signal(signal.SIGTERM, sig_exit)

try:
    # set all lasers to off
    set_all_lasers_mode(0, midi_out=midi_out)

    # do 5 x by x rounds
    for i in range(1,6):
        x_by_x(x=i, step_sleep=0.7/i**0.1, random_sleep=True, random_order=True, midi_out=midi_out)

    # do on_off rounds until interrupted
    i = 6
    while True:
        all_on_off(step_sleep=0.7/i**0.1, random_sleep=False, midi_out=midi_out)
        i += 1
        
except KeyboardInterrupt:
    print('Received KeyboardInterrupt signal. Exiting...')
    exit_pattern()
    sys.exit(0)
        