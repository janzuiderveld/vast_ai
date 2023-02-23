import rtmidi
from light_abstr import MidiOutWrapper, all_synths_port, model_port
import signal
import time
import os
import random 
import sys

script_dir = os.path.dirname(__file__)

playback = all_synths_port
model_port = model_port

ambient_ch = 5
no_input_ambient_start = 10

min_note_duration = 5
max_random_note_d = 30

min_silence_duration = 1
max_random_silence_d = 10

# set up midi out
midi_out = rtmidi.MidiOut()
midi_out.open_port(playback)
# maybe try if without wrapper outs are omni?
midi_out = MidiOutWrapper(midi_out, ch=ambient_ch)  # passing in a rtmidi.MidiOut instance

def handle_input(event, data=None):
    message, deltatime = event
    # note = message[1]
    # note_on = message[2] > 0

    global last_input_time

    if last_input_time + no_input_ambient_start < time.time():
        midi_out.all_notes_off(ch=ambient_ch)
    
    last_input_time = time.time()
    
# set up midi in
midi_in = rtmidi.MidiIn()
midi_in.open_port(model_port)
midi_in.set_callback(handle_input)

# define notes to play
notes = [48, 50, 52, 53, 55, 57, 59, 60, 62, 64, 65]
# extend with lower octave
notes.extend([n - 12 for n in notes[:12]])

def sig_exit(sig, frame):
    print('Received SIGTERM signal. Exiting...')
    midi_out.note_off_all(ch=ambient_ch)
    sys.exit(0)

signal.signal(signal.SIGTERM, sig_exit)

# send midi for ambient background sounds
try:
    last_input_time = time.time()
    while True:
        # check time since last input
        if time.time() - last_input_time > no_input_ambient_start:
            note = random.choice(notes)
            velocity = random.randint(50, 127)
            duration = (max_random_note_d * random.random()) + min_note_duration

            print(f"playing note {note} for {duration} seconds")
            midi_out.note_on(note)
            time.sleep(duration)

            print(f"turning off note {note}")
            midi_out.note_off(note)

            silence_duration = (max_random_silence_d * random.random()) + min_silence_duration
            print(f"silence for {silence_duration} seconds")
            time.sleep(silence_duration)

except KeyboardInterrupt:
    print('Received KeyboardInterrupt signal. Exiting...')
    midi_out.note_off(note)
    midi_out.all_notes_off(ch=ambient_ch)
    sys.exit(0)

