import rtmidi
from light_abst import MidiOutWrapper
import signal
import time

script_dir = os.path.dirname(__file__)

playback = 6
model_in = 0
ambient_ch = 5
no_input_ambient_start = 5
min_note_duration = 3
max_random_note_d = 5

# set up midi out
midi_out = rtmidi.MidiOut()
midi_out.open_port(playback)
# maybe try if without wrapper outs are omni?
midi_out = MidiOutWrapper(midi_out, ch=ambient_ch)  # passing in a rtmidi.MidiOut instance

def handle_input(event, data=None):
    message, deltatime = event
    # note = message[1]
    # note_on = message[2] > 0

    if last_input_time + no_input_ambient_start < time.time():
        midi_out.all_notes_off(ch=ambient_ch)
    
    last_input_time = time.time()
    
# set up midi in
midi_in = rtmidi.MidiIn()
midi_in.open_port(model_in)
midi_in.set_callback(handle_input)

# define notes to play
notes = [48, 50, 52, 53, 55, 57, 59, 60, 62, 64, 65, 67, 69, 71, 72]

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
            midi_out.note_on(note)
            time.sleep(duration)
            midi_out.note_off(note)
except KeyboardInterrupt:
    print('Received KeyboardInterrupt signal. Exiting...')
    midi_out.note_off_all(ch=ambient_ch)
    sys.exit(0)

