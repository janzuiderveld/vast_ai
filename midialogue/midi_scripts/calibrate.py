import rtmidi
import signal
import random
from light_abstr import calibrate_laser, set_laser_mode, set_all_lasers_mode, x_by_x, all_on_off, in_port
import os
import sys
import time

# arg: None is manual, 0-7 is is laser, 8 is all

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

try:
    auto = int(sys.argv[1])
except:
    auto = None

while True:
    try:
        if auto == None:
            laser_num = int(input('Enter laser_num to calibrate: '))
        else: 
            laser_num = auto 

        if laser_num != 8:

            # turn off laser on channel
            set_laser_mode(laser_num, 0, midi_out=midi_out)

            time.sleep(1)

            calibrate_laser(laser_num, midi_out=midi_out)

            # turn on laser on channel
            set_laser_mode(laser_num, 1, midi_out=midi_out)
        
        else:
            for laser_num in range(7):
                # turn off laser on channel
                set_laser_mode(laser_num, 0, midi_out=midi_out)

                calibrate_laser(laser_num, midi_out=midi_out)

                # turn on laser on channel
                set_laser_mode(laser_num, 1, midi_out=midi_out)
            
        if auto != None:
            break


    except KeyboardInterrupt:
        print('Received KeyboardInterrupt signal. Exiting...')
        exit_pattern()
        sys.exit(0)
            