import pretty_midi 

# open midi file /home/p/vast_ai/midialogue/midi_out/midi_0023.mid
mf = pretty_midi.PrettyMIDI("/home/p/vast_ai/midialogue/midi_out/15_midii.mid")
# mf = pretty_midi.PrettyMIDI("/home/p/vast_ai/midialogue/midi_out/7_midi.mid")

# sort instruments by name
mf.instruments.sort(key=lambda x: x.name)

mf.instruments.append(pretty_midi.Instrument(program=0))

for i in range(4):
    mf.instruments[i].notes.append(pretty_midi.Note(
        velocity=0, pitch=60, start=0, end=1))

# count the number of notes on every channel
for i, instrument in enumerate((mf.instruments)):
    print("Channel {}: {}".format(i, len(instrument.notes)))
    print(instrument.name)


# change the notes on channel 0 to channel 1
mf.instruments[1] = mf.instruments[0]

# change the notes on channel 1 to channel 2


# print the velocity of every note on channel 0
print("Channel 0")
for note in mf.instruments[0].notes:
    print(note.velocity)
    print(note.pitch)


# print midi notes on channel 4
print("Channel 4")
for note in mf.instruments[3].notes:
    print(note)
    
print("Channel 3")
for note in mf.instruments[2].notes:
    print(note)

print("Channel 2")
for note in mf.instruments[1].notes:
    print(note)

print("Channel 1")
for note in mf.instruments[0].notes:
    print(note)
    

# change all notes on channel 4 to 60
for note in mf.instruments[3].notes:
    note.pitch = 62

# change all notes on channel 3 to 0 velocity
for note in mf.instruments[2].notes:
    note.velocity = 0

# change all notes on channel 2 to 0 velocity
for note in mf.instruments[1].notes:
    note.velocity = 0

# change all notes on channel 1 to 0 velocity
for note in mf.instruments[0].notes:
    note.velocity = 0


# save midi file
new_fp = '/home/p/vast_ai/midi_0023b.mid'
mf.write(new_fp)

import os
# play midi file
os.chdir("/home/p/vast_ai/midialogue/midi-utilities/bin")
os.system("./playsmf --out {} {}".format(6, new_fp))