import rtmidi
import signal
import random
import time
import sys
import os
from light_abstr import set_laser_mode, set_all_lasers_mode, x_by_x, all_on_off, in_port, all_synths_port

script_dir = os.path.dirname(__file__)

teensy = in_port
playback = all_synths_port

def handle_input(event, data=None):
    event, deltatime = event
    
    if event[0] < 0xF0:
        channel = (event[0] & 0xF) + 1
        status = event[0] & 0xF0
    else:
        status = event[0]
        channel = None

    note = event[1]
    note_on = event[2] > 0

    if channel == 1 and status == 144:
        if note <= 64:
            set_laser_mode(0, note_on, midi_out=midi_out)
        else:
            set_laser_mode(1, note_on, midi_out=midi_out)

    elif channel == 2 and status == 144:
        set_laser_mode(2, note_on, midi_out=midi_out)
    
    elif channel == 3 and status == 144:
        set_laser_mode(3, note_on, midi_out=midi_out)

    elif channel == 4 and status == 144:
        if note == 60 or note == 62:
            set_laser_mode(4, note_on, midi_out=midi_out)
        
        elif note == 64 or note == 65:
            set_laser_mode(5, note_on, midi_out=midi_out)
        
        elif note == 67 or note == 69:
            set_laser_mode(6, note_on, midi_out=midi_out)

        

# functions = [handle_input0, handle_input1, handle_input3, handle_input4]

# set up midi out
midi_out = rtmidi.MidiOut()
midi_out.open_port(teensy)

# set up midi in
midi_in = rtmidi.MidiIn()
midi_in.open_port(playback)
midi_in.set_callback(handle_input)

def sig_exit(sig, frame):
    print('Received SIGTERM signal. Exiting...')
    exit_pattern()
    sys.exit(0)

def exit_pattern():
    # global midi_in
    # del midi_in
    # set_all_lasers_mode(0, midi_out=midi_out)
    # time.sleep(0.3)
    # set_all_lasers_mode(1, step_sleep=0.4, random_sleep=True, midi_out=midi_out)
    pass

signal.signal(signal.SIGTERM, sig_exit)

try:
    # set all lasers to off
    set_all_lasers_mode(0, midi_out=midi_out)

    while True:
        time.sleep(1)
        
except KeyboardInterrupt:
    print('Received KeyboardInterrupt signal. Exiting...')
    exit_pattern()
    sys.exit(0)


        



