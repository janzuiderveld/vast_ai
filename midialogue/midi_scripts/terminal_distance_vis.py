import rtmidi
import signal
import random
from light_abstr import query_dists, calibrate_laser, set_laser_mode, set_all_lasers_mode, x_by_x, all_on_off, in_port
import os
import sys
import time

# arg: None is manual, 0-7 is is laser, 8 is all

script_dir = os.path.dirname(__file__)

counter = 0
dists = [0, 0, 0, 0, 0, 0, 0]

def handle_input(event, data=None):
    event, deltatime = event
    if event[1] != 1:
        return

    global counter
    global dists

    if counter % 2 == 0:
        dists[counter//2] = event[2]*10
    else:
        dists[counter//2] += event[2]
    # print(event)
    
    counter += 1
    counter = counter % 14

# set up midi in
midi_in = rtmidi.MidiIn()
midi_in.open_port(in_port)
midi_in.set_callback(handle_input)

# set up midi out
midi_out = rtmidi.MidiOut()
midi_out.open_port(in_port)


def sig_exit(sig, frame):
    print('Received SIGTERM signal. Exiting...')
    sys.exit(0)

signal.signal(signal.SIGTERM, sig_exit)

# Function to clear the screen
def clear_screen():
    if os.name == 'posix':  # Unix/Linux/MacOS/BSD/etc.
        os.system('clear')
    elif os.name in ('nt', 'dos', 'ce'):  # DOS/Windows
        os.system('cls')
    else:
        print("\n" * 80)  # Fallback for other operating systems

# Function to print the bars
def print_bars(var1, var2, var3, var4, var5, var6, var7, max_value=1000):
    clear_screen()

    bar1 = int(var1 / max_value * 200)
    bar2 = int(var2 / max_value * 200)
    bar3 = int(var3 / max_value * 200)
    bar4 = int(var4 / max_value * 200)
    bar5 = int(var5 / max_value * 200)
    bar6 = int(var6 / max_value * 200)
    bar7 = int(var7 / max_value * 200)

    print("Laser 0: " + "|" * bar1 + f" ({var1}/{max_value})")
    print("Laser 1: " + "|" * bar2 + f" ({var2}/{max_value})")
    print("Laser 2: " + "|" * bar3 + f" ({var3}/{max_value})")
    print("Laser 3: " + "|" * bar4 + f" ({var4}/{max_value})")
    print("Laser 4: " + "|" * bar5 + f" ({var5}/{max_value})")
    print("Laser 5: " + "|" * bar6 + f" ({var6}/{max_value})")
    print("Laser 6: " + "|" * bar7 + f" ({var7}/{max_value})")
        

while True:
    try:
        time.sleep(0.1)
        # get distances
        query_dists(midi_out=midi_out)
        print_bars(dists[0], dists[1], dists[2], dists[3], dists[4], dists[5], dists[6])

    except KeyboardInterrupt:
        print('Received KeyboardInterrupt signal. Exiting...')
        sys.exit(0)
            