
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <rtmidi_c.h>
#include <midifile.h>
#include <midiutil-common.h>
#include <midiutil-system.h>
#include <midiutil-rtmidi.h>

static int should_shutdown = 0;
static MidiUtilLock_t lock = NULL;

static void usage(char *program_name)
{
	fprintf(stderr, "Usage:  %s --out <port> [ --from <time> ] [ --to <time> ] [ ( --solo-track <n> ) ... | ( --mute-track <n> ) ... ] [ --extra-time <seconds> ] <filename.mid>\n", program_name);
	exit(1);
}

static void handle_interrupt(void *arg)
{
	MidiUtilLock_lock(lock);
	should_shutdown = 1;
	MidiUtilLock_notify(lock);
	MidiUtilLock_unlock(lock);
}

int main(int argc, char **argv)
{
	int i;
	char *midi_out_port = NULL;
	char *from_string = NULL;
	char *to_string = NULL;
	int number_of_solo_tracks = 0;
	int solo_tracks[1024];
	int number_of_mute_tracks = 0;
	int mute_tracks[1024];
	float extra_time = 0.0;
	char *filename = NULL;
	MidiFile_t midi_file;
	RtMidiOutPtr midi_out = NULL;
	RtMidiOutPtr track_midi_outs[1024];
	unsigned long start_time = 0;
	MidiFileEvent_t midi_file_event;
	long from_tick;
	long to_tick;

	for (i = 1; i < argc; i++)
	{
		if (strcmp(argv[i], "--help") == 0)
		{
			usage(argv[0]);
		}
		else if (strcmp(argv[i], "--out") == 0)
		{
			if (++i == argc) usage(argv[0]);
			midi_out_port = argv[i];
		}
		else if (strcmp(argv[i], "--from") == 0)
		{
			if (++i == argc) usage(argv[0]);
			from_string = argv[i];
		}
		else if (strcmp(argv[i], "--to") == 0)
		{
			if (++i == argc) usage(argv[0]);
			to_string = argv[i];
		}
		else if (strcmp(argv[i], "--solo-track") == 0)
		{
			if (++i == argc) usage(argv[0]);
			solo_tracks[number_of_solo_tracks++] = atoi(argv[i]);
			number_of_solo_tracks++;
		}
		else if (strcmp(argv[i], "--mute-track") == 0)
		{
			if (++i == argc) usage(argv[0]);
			mute_tracks[number_of_mute_tracks++] = atoi(argv[i]);
			number_of_mute_tracks++;
		}
		else if (strcmp(argv[i], "--extra-time") == 0)
		{
			if (++i == argc) usage(argv[0]);
			extra_time = (float)(atof(argv[i]));
		}
		else
		{
			filename = argv[i];
		}
	}

	if ((midi_out_port == NULL) || (filename == NULL))
	{
		usage(argv[0]);
	}

	if ((midi_file = MidiFile_load(filename)) == NULL)
	{
		fprintf(stderr, "Error:  Cannot open \"%s\".\n", filename);
		return 1;
	}

	from_tick = MidiFile_getTickFromTimeString(midi_file, from_string);
	if (from_tick < 0) from_tick = 0;
	to_tick = MidiFile_getTickFromTimeString(midi_file, to_string);
	if (to_tick < 0) to_tick = MidiFileEvent_getTick(MidiFile_getLastEvent(midi_file));

	if ((midi_out = rtmidi_open_out_port("playsmf", midi_out_port, "playsmf")) == NULL)
	{
		printf("Error:  Cannot open MIDI output port \"%s\".\n", midi_out_port);
		return 1;
	}

	{
		int number_of_tracks = MidiFile_getNumberOfTracks(midi_file);
		int track_number;

		if (number_of_solo_tracks > 0)
		{
			int solo_track_number;

			for (track_number = 0; track_number < number_of_tracks; track_number++) track_midi_outs[track_number] = NULL;

			for (solo_track_number = 0; solo_track_number < number_of_solo_tracks; solo_track_number++)
			{
				if (solo_tracks[solo_track_number] < number_of_tracks) track_midi_outs[solo_tracks[solo_track_number]] = midi_out;
			}
		}
		else
		{
			int mute_track_number;

			for (track_number = 0; track_number < number_of_tracks; track_number++) track_midi_outs[track_number] = midi_out;

			for (mute_track_number = 0; mute_track_number < number_of_mute_tracks; mute_track_number++)
			{
				if (mute_tracks[mute_track_number] < number_of_tracks) track_midi_outs[mute_tracks[mute_track_number]] = NULL;
			}
		}
	}

	lock = MidiUtilLock_new();
	MidiUtil_setInterruptHandler(handle_interrupt, NULL);

	for (midi_file_event = MidiFile_getFirstEvent(midi_file); midi_file_event != NULL; midi_file_event = MidiFileEvent_getNextEventInFile(midi_file_event))
	{
		if (MidiFileEvent_getType(midi_file_event) != MIDI_FILE_EVENT_TYPE_META)
		{
			long tick = MidiFileEvent_getTick(midi_file_event);
			int in_range = ((tick >= from_tick) && (tick <= to_tick));
			RtMidiOutPtr midi_out = track_midi_outs[MidiFileTrack_getNumber(MidiFileEvent_getTrack(midi_file_event))];

			if ((!should_shutdown) && in_range)
			{
				unsigned long event_time = (unsigned long)(MidiFile_getTimeFromTick(midi_file, tick) * 1000);

				while (!should_shutdown)
				{
					unsigned long current_time = MidiUtil_getCurrentTimeMsecs();

					if (start_time == 0)
					{
						start_time = current_time - event_time; /* fake start time based on range */
					}

					if (current_time - start_time < event_time)
					{
						MidiUtilLock_lock(lock);
						MidiUtilLock_wait(lock, event_time - (current_time - start_time));
						MidiUtilLock_unlock(lock);
					}
					else
					{
						break;
					}
				}
			}

			if (midi_out != NULL)
			{
				if (MidiFileEvent_getType(midi_file_event) == MIDI_FILE_EVENT_TYPE_SYSEX)
				{
					rtmidi_out_send_message(midi_out, (const unsigned char *)(MidiFileSysexEvent_getData(midi_file_event)), MidiFileSysexEvent_getDataLength(midi_file_event));
				}
				else if ((should_shutdown && !MidiFileEvent_isNoteStartEvent(midi_file_event)) || (!should_shutdown && (in_range || ((MidiFileEvent_getType(midi_file_event) != MIDI_FILE_EVENT_TYPE_NOTE_ON) && (MidiFileEvent_getType(midi_file_event) != MIDI_FILE_EVENT_TYPE_NOTE_OFF)))))
				{
					unsigned long data = MidiFileVoiceEvent_getData(midi_file_event);
					rtmidi_out_send_message(midi_out, (const unsigned char *)(&data), MidiFileVoiceEvent_getDataLength(midi_file_event));
				}
			}
		}
	}

	if ((extra_time > 0) && !should_shutdown)
	{
		MidiUtilLock_lock(lock);
		MidiUtilLock_wait(lock, (long)(extra_time * 1000.0));
		MidiUtilLock_unlock(lock);
	}

	MidiUtil_setInterruptHandler(NULL, NULL);
	MidiUtilLock_free(lock);
	rtmidi_close_port(midi_out);
	MidiFile_free(midi_file);
	return 0;
}

