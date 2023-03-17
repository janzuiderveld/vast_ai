import tempfile
import pretty_midi

def load_midi_fp(fp):
  pretty_midi.pretty_midi.MAX_TICK = 1e16
  with open(fp, "rb") as fh:
      fp = fh.read()

  # Load MIDI file
  with tempfile.NamedTemporaryFile('wb') as mf:
    mf.write(fp)
    mf.seek(0)
    print(mf.name)
    
    print("startup: loading midi")
    midi = pretty_midi.PrettyMIDI(mf.name)

    print(midi.instruments)
    if len(midi.instruments) > 4:
        del midi.instruments[4:]
    if len(midi.instruments) < 4:
        for i in range(4 - len(midi.instruments)):
            midi.instruments.append(pretty_midi.Instrument(program=0))

    instr_map = {}
    # check the velocity of the notes
    for i in range(4):
        # get velocity of the first note
        if len(midi.instruments[i].notes) > 0:
            track_no = midi.instruments[i].notes[0].velocity - 121
            instr_map[i] = track_no
            print(midi.instruments[i].notes[0].velocity)
        else:
            # map to lowest track with no notes
            for j in range(4):
                if j not in instr_map.values():
                    instr_map[i] = j
                    break

    # map instruments according to instr_map
    # cp = midi.instruments.copy()
    # for i in range(4):
    #     midi.instruments[instr_map[i]] = cp[i]

    midi.instruments[0].name = "p1"
    midi.instruments[1].name = "p2"
    midi.instruments[2].name = "tr"
    midi.instruments[3].name = "no"

    for note in midi.instruments[3].notes:
        if note.pitch == 60:
            note.pitch = 2
        elif note.pitch == 62:
            note.pitch = 5
        elif note.pitch == 64:
            note.pitch = 8
        elif note.pitch == 65:
            note.pitch = 11
        elif note.pitch == 67:
            note.pitch = 13
        elif note.pitch == 69:
            note.pitch = 15


    # delete the program change messages
    for i in range(4):
        for j in range(len(midi.instruments[i].control_changes)):
            if midi.instruments[i].control_changes[j].number == 0:
                del midi.instruments[i].control_changes[j]
                break

    return midi

midi = load_midi_fp("/home/p/vast_ai/midi_out copy/0_midii.mid")

with tempfile.NamedTemporaryFile('rb') as mf:
    midi.write(mf.name)
    midi = mf.read()

filepath = "/home/p/vast_ai/midi_out copy/0_midii_c.mid"
with open(filepath, 'wb') as f:
    f.write(midi)