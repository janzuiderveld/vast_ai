import tempfile
import pretty_midi
import sys

fp = sys.argv[1]
# timeout = sys.argv[2]
timeout = 3
max_duration = 10

def load_midi_fp(fp):
  pretty_midi.pretty_midi.MAX_TICK = 1e16
  with open(fp, "rb") as fh:
      fp = fh.read()

  # Load MIDI file
  with tempfile.NamedTemporaryFile('wb') as mf:
    mf.write(fp)
    mf.seek(0)
    print(mf.name)
    
    midi = pretty_midi.PrettyMIDI(mf.name)

    end_time = midi.get_end_time()

    # # print length of midi
    print(f" original midi length: {end_time} seconds")

    print(f" removing last {timeout} seconds of midi")



    # remove the last {timeout} seconds of the midi using pretty_midi
    # iterate over the notes in the midi
    for instrument in midi.instruments:
        to_delete = []
        for i, note in enumerate(instrument.notes):
            if note.end > end_time - float(timeout):
                to_delete.append(i)
            elif note.start > end_time - float(timeout):
                to_delete.append(i)
        for i in reversed(to_delete):
            del instrument.notes[i]

    end_time = midi.get_end_time()

    print(f" new midi length: {end_time} seconds")

    if end_time > max_duration:
        # remove the first end_time - max_duration seconds of the midi
        for instrument in midi.instruments:
            to_delete = []
            for i, note in enumerate(instrument.notes):
                if note.end < end_time - max_duration:
                    to_delete.append(i)
                elif note.start < end_time - max_duration:
                    to_delete.append(i)
            for i in reversed(to_delete):
                del instrument.notes[i]

        # subtract the first second from all notes
        for instrument in midi.instruments:
            for note in instrument.notes:
                note.start -= max_duration
                note.end -= max_duration


    end_time = midi.get_end_time()

    print(f" final midi length: {end_time} seconds")

    # print notes
    # for instrument in midi.instruments:
    #     for note in instrument.notes:
    #         print(note)
     
    # remove tempo changes
    midi.time_signature_changes = []
    midi.key_signature_changes = []
    midi.tempo_changes = []

    return midi

midi = load_midi_fp(fp)

with tempfile.NamedTemporaryFile('rb') as mf:
    midi.write(mf.name)
    midi = mf.read()

with open(fp, 'wb') as f:
    f.write(midi)