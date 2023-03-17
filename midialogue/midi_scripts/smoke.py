import rtmidi
import signal
import random
from light_abstr import smoke, in_port
import os
import sys
import time

# arg: 1 is on, 0 is off

script_dir = os.path.dirname(__file__)
teensy = in_port

# set up midi out
midi_out = rtmidi.MidiOut()
midi_out.open_port(teensy)

mode = int(sys.argv[1])
smoke(mode, midi_out=midi_out)