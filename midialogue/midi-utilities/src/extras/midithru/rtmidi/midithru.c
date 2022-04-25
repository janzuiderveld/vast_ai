
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <rtmidi_c.h>
#include <midiutil-common.h>
#include <midiutil-system.h>
#include <midiutil-rtmidi.h>

static int number_of_midi_ins = 0;
static RtMidiInPtr midi_ins[128];
static int number_of_midi_outs = 0;
static RtMidiOutPtr midi_outs[128];

static void usage(char *program_name)
{
	fprintf(stderr, "Usage:  %s [ --in <port> ] ... [ --virtual-in <port> ] ... [ --out <port> ] ... [ --virtual-out <port> ] ...\n", program_name);
	exit(1);
}

static void handle_midi_message(double timestamp, const unsigned char *message, size_t message_size, void *user_data)
{
	int midi_out_number;
	for (midi_out_number = 0; midi_out_number < number_of_midi_outs; midi_out_number++) rtmidi_out_send_message(midi_outs[midi_out_number], message, message_size);
}

static void handle_exit(void *user_data)
{
	while (number_of_midi_ins > 0) rtmidi_close_port(midi_ins[--number_of_midi_ins]);
	while (number_of_midi_outs > 0) rtmidi_close_port(midi_outs[--number_of_midi_outs]);
}

int main(int argc, char **argv)
{
	int i;

	for (i = 1; i < argc; i++)
	{
		if (strcmp(argv[i], "--help") == 0)
		{
			usage(argv[0]);
		}
		else if (strcmp(argv[i], "--in") == 0)
		{
			if (++i == argc) usage(argv[0]);

			if ((midi_ins[number_of_midi_ins++] = rtmidi_open_in_port("midithru", argv[i], "midithru", handle_midi_message, NULL)) == NULL)
			{
				fprintf(stderr, "Error:  Cannot open MIDI input port \"%s\".\n", argv[i]);
				exit(1);
			}
		}
		else if (strcmp(argv[i], "--virtual-in") == 0)
		{
			if (++i == argc) usage(argv[0]);
			midi_ins[number_of_midi_ins++] = rtmidi_open_in_port("midithru", NULL, argv[i], handle_midi_message, NULL);
		}
		else if (strcmp(argv[i], "--out") == 0)
		{
			if (++i == argc) usage(argv[0]);

			if ((midi_outs[number_of_midi_outs++] = rtmidi_open_out_port("midithru", argv[i], "midithru")) == NULL)
			{
				fprintf(stderr, "Error:  Cannot open MIDI output port \"%s\".\n", argv[i]);
				exit(1);
			}
		}
		else if (strcmp(argv[i], "--virtual-out") == 0)
		{
			if (++i == argc) usage(argv[0]);
			midi_outs[number_of_midi_outs++] = rtmidi_open_out_port("midithru", NULL, argv[i]);
		}
		else
		{
			usage(argv[0]);
		}
	}

	MidiUtil_waitForExit(handle_exit, NULL);
	return 0;
}

