
#include <QtWidgets>
#include "continuous-value-lane.h"
#include "controller-lane.h"
#include "lane.h"
#include "midifile.h"
#include "window.h"

ControllerLane::ControllerLane(Window* window): ContinuousValueLane(window, Lane::CONTROLLER_LANE_TYPE)
{
}

bool ControllerLane::shouldIncludeEvent(MidiFileEvent_t midi_event)
{
	return (MidiFileEvent_getType(midi_event) == MIDI_FILE_EVENT_TYPE_CONTROL_CHANGE) && (MidiFileControlChangeEvent_getNumber(midi_event) == this->controller_number);
}

MidiFileEvent_t ControllerLane::addEvent(long tick, float value)
{
	return MidiFileTrack_createControlChangeEvent(MidiFile_getTrackByNumber(this->window->sequence->midi_file, this->track_number, 1), tick, this->channel, this->controller_number, (int)(value));
}

float ControllerLane::getEventValue(MidiFileEvent_t midi_event)
{
	return (float)(MidiFileControlChangeEvent_getValue(midi_event));
}

void ControllerLane::setEventValue(MidiFileEvent_t midi_event, float value)
{
	MidiFileControlChangeEvent_setValue(midi_event, (int)(value));
}

