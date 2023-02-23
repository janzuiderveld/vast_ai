import time
import random
from rtmidi.midiconstants import (ALL_NOTES_OFF, ALL_SOUND_OFF, BALANCE, BANK_SELECT_LSB,
                                  BANK_SELECT_MSB, BREATH_CONTROLLER, CHANNEL_PRESSURE,
                                  CHANNEL_VOLUME, CONTROL_CHANGE, DATA_ENTRY_LSB, DATA_ENTRY_MSB,
                                  END_OF_EXCLUSIVE, EXPRESSION_CONTROLLER, FOOT_CONTROLLER,
                                  LOCAL_CONTROL, MIDI_TIME_CODE, MODULATION, NOTE_OFF, NOTE_ON,
                                  NRPN_LSB, NRPN_MSB, PAN, PITCH_BEND, POLY_PRESSURE,
                                  PROGRAM_CHANGE, RESET_ALL_CONTROLLERS, RPN_LSB, RPN_MSB,
                                  SONG_POSITION_POINTER, SONG_SELECT, TIMING_CLOCK)
num_steps = 7

import os

in_port = int(os.environ['in_port'])
model_port = int(os.environ['model_port'])
all_synths_port = int(os.environ['all_synths_port'])


def set_laser_mode(i, mode, midi_out=None):
    if mode == 1:
        midi_out.send_message([0x90, i, 112])  # note on, middle C, velocity 112
    elif mode == 0:
        midi_out.send_message([0x80, i, 0])  # note off, middle C, velocity 0

def set_all_lasers_mode(mode, step_sleep=0, random_sleep=False, random_order=False, midi_out=None):
    i_list = list(range(7))
    if random_order:
        random.shuffle(i_list)
    for i in i_list:
        set_laser_mode(i, mode, midi_out=midi_out)
        time.sleep(step_sleep * random.random() if random_sleep else step_sleep)

def x_by_x(x=1, step_sleep=0.1, random_sleep=False, random_order=False, midi_out=None):
    i_list = list(range(num_steps))
    if random_order:
        random.shuffle(i_list)
    for i in i_list:
        set_laser_mode(i, 0, midi_out=midi_out)
        set_laser_mode((i + x) % num_steps, 1, midi_out=midi_out)
        time.sleep(step_sleep * random.random() if random_sleep else step_sleep)

def all_on_off(step_sleep=0.1, random_sleep=False, midi_out=None):
    set_all_lasers_mode(1, step_sleep=0, random_sleep=False, midi_out=midi_out)
    time.sleep(step_sleep * random.random() if random_sleep else step_sleep)
    set_all_lasers_mode(0, step_sleep=0, random_sleep=False, midi_out=midi_out)
    time.sleep(step_sleep * random.random() if random_sleep else step_sleep)

# MIDI Abstractionz

class MidiOutWrapper:
    def __init__(self, midi, ch=1):
        self.channel = ch
        self._midi = midi

    def channel_message(self, command, *data, ch=None):
        """Send a MIDI channel mode message."""
        command = (command & 0xf0) | ((ch if ch else self.channel) - 1 & 0xf)
        msg = [command] + [value & 0x7f for value in data]
        self._midi.send_message(msg)

    def note_off(self, note, velocity=0, ch=None):
        """Send a 'Note Off' message."""
        self.channel_message(NOTE_OFF, note, velocity, ch=ch)

    def note_on(self, note, velocity=127, ch=None):
        """Send a 'Note On' message."""
        self.channel_message(NOTE_ON, note, velocity, ch=ch)

    def program_change(self, program, ch=None):
        """Send a 'Program Change' message."""
        self.channel_message(PROGRAM_CHANGE, program, ch=ch)

    def all_notes_off(self, ch=None):
        """Send a 'All Notes Off' message."""
        # send note off for all notes
        if ch is None:
            for ch in range(1,6):
                for note in range(128):
                    # midi_out.send_message([0x80, note, 0]
                    self.note_off(note, ch=ch) 
        else:
            for note in range(128):
                # midi_out.send_message([0x80, note, 0]
                self.note_off(note, ch=ch)

        # if ch is None:
        #     for ch in range(1,6):
        #         self.channel_message(ALL_NOTES_OFF, ch=ch)
        # else:
        #     self.channel_message(ALL_NOTES_OFF, ch=ch)

# midiout = MidiOutWrapper(midiout)  # passing in a rtmidi.MidiOut instance
# midiout.channel_message(NOTE_ON, 36, 127, ch=10)
# midiout.channel_message(NOTE_ON, 40, 127)  # sent on default channel 1

# midiout.program_change(9)  # programs counted from zero too!
# midiout.note_on(36, ch=10)  # default velocity 127
# midiout.note_on(40, 64)  # default channel 1
# midiout.note_on(60)  # default velocity and channel
# midiout.note_off(36, ch=10)   # default release velocity 0
# midiout.note_off(40, 64)  # default channel 1
# midiout.note_off(60)  # default release velocity and channel