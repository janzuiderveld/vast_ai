import rtmidi
import signal
import random
import time
import sys
import os
from light_abstr import set_laser_mode, set_all_lasers_mode, x_by_x, all_on_off

script_dir = os.path.dirname(__file__)

teensy = 1

# set up midi out
midi_out = rtmidi.MidiOut()
midi_out.open_port(teensy)

try:
    set_all_lasers_mode(0, midi_out=midi_out)
    time.sleep(0.3)
    set_all_lasers_mode(1, step_sleep=0.4, random_sleep=True, midi_out=midi_out)
        
except KeyboardInterrupt:
    sys.exit(0)


        



