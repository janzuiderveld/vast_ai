#ifndef ALL_EVENTS_LANE_INCLUDED
#define ALL_EVENTS_LANE_INCLUDED

class AllEventsLane;

#include <QtWidgets>
#include "label-lane.h"
#include "midifile.h"
#include "window.h"

class AllEventsLane: public LabelLane
{
	Q_OBJECT

public:
	AllEventsLane(Window* window);
	void populateLabels();
	MidiFileEvent_t addEventAtXY(int x, int y);
	void moveEventsByXY(int x_offset, int y_offset);
};

#endif
