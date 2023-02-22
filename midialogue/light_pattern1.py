#!/bin/python3
# loop until killed

import os
import time
import glob

import mido

port = mido.open_output('Teensy MIDI:Teensy MIDI MIDI 1')
msg = mido.Message('note_off', note=0)

port.send(msg)


num_lasers=7
i = 0
init_speed = 0.1

def set_light(i, mode):
    if mode > 0:
        note = NoteOn(i, 1)
        # os.system(f"midialogue/midi-utilities/bin/sendmidi --out 1 --note-on 0 {i} 1")
    else:
        note = NoteOff(i, 1)
        # os.system(f"midialogue/midi-utilities/bin/sendmidi --out 1 --note-off 0 {i} 1")  
    
    msg = Message(note, channel=1)
    conn.write(msg)

while True:
    # turn on light
    set_light(i, 1)
    # turn off light
    set_light(i+1, 0)
    # increment i
    i = (i+1) % num_lasers
    # wait
    time.sleep(init_speed)


# midi-utilities/bin/sendmidi --out 1 --note-off 0 $i+1 1
