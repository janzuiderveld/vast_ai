import rtmidi
from light_abstr import MidiOutWrapper
import os 

script_dir = os.path.dirname(__file__)

playback = 6

# set up midi out
playback_out = rtmidi.MidiOut()
playback_out.open_port(playback)
# maybe try if without wrapper outs are omni?
playback_out = MidiOutWrapper(playback_out)  # passing in a rtmidi.MidiOut instance

playback_out.all_notes_off()