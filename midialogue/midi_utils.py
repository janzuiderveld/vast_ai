import pretty_midi
import os
import tempfile
def load_midi(fp):
    # os.system(f"/Users/janzuiderveld/Documents/GitHub/vast_ai/midialogue/midi-utilities/bin/normalizesmf {fp}")
    
    pretty_midi.pretty_midi.MAX_TICK = 1e16
    with open(fp, "rb") as fh:
        fp = fh.read()

    # Load MIDI file
    with tempfile.NamedTemporaryFile('wb') as mf:
        mf.write(fp)
        mf.seek(0)
        print(mf.name)
        while True:
            try:
                midi = pretty_midi.PrettyMIDI(mf.name)
                break
            except:
                continue

    # midi = pretty_midi.PrettyMIDI(fp)
    # note_count = midi.get_piano_roll().sum()
    # if note_count:
    #     print(note_count)
    #     return midi 
    # else:
    #     return 0

in_file = "/workspace/vast_ai/midialogue/midi_in/20220426144438.mid"
# out_file = "/Users/janzuiderveld/Documents/GitHub/vast_ai/midialogue/midi_edit/20220425235234.mid"
# in_file = "/Users/janzuiderveld/Documents/GitHub/vast_ai/midialogue/midi_in/20220426000304.mid"
# out_file = "/Users/janzuiderveld/Documents/GitHub/vast_ai/midialogue/midi_edit/20220426000304.mid"


# os.system(f"bash test_midi_edit.sh {in_file} {out_file}")

import time
# load_midi(out_file)
start = time.time()
load_midi(in_file)
end = time.time()
print(end - start)




    # # Load MIDI file
    # with open(fp, "rb") as fh:
    #     data = fh.read()
    # with tempfile.NamedTemporaryFile('wb') as mf:
    #     mf.write(data)
    #     mf.seek(0)
    #     midi_data = pretty_midi.PrettyMIDI(mf.name)
    #     # count notes
    #     note_count = midi_data.get_piano_roll().sum(axis=0)
    #     if note_count: break