import rtmidi

script_dir = os.path.dirname(__file__)

playback = 6

# set up midi out
midi_out = rtmidi.MidiOut()
midi_out.open_port(playback)
# maybe try if without wrapper outs are omni?
midi_out = MidiOutWrapper(midi_out)  # passing in a rtmidi.MidiOut instance

# send note off for all notes
for ch in range(1,5):
    for note in range(128):
        # midi_out.send_message([0x80, note, 0]
        midi_out.note_off(note, ch=ch) 
