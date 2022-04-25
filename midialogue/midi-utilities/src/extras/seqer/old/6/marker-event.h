#ifndef MARKER_EVENT_INCLUDED
#define MARKER_EVENT_INCLUDED

class MarkerEventType;
class MarkerEventRow;
class MarkerEventTimeCell;
class MarkerEventNameCell;

#include <wx/wx.h>
#include <midifile.h>
#include "sequence-editor.h"

extern MarkerEventType* marker_event_type;

class MarkerEventType: public EventType
{
public:
	static MarkerEventType* GetInstance();

private:
	MarkerEventType();

public:
	bool MatchesEvent(MidiFileEvent_t event);
	Row* GetRow(SequenceEditor* sequence_editor, long step_number, MidiFileEvent_t event);
};

class MarkerEventRow: public Row
{
public:
	MarkerEventRow(SequenceEditor* sequence_editor, long step_number, MidiFileEvent_t event);
	void Delete();
};

class MarkerEventTimeCell: public Cell
{
public:
	MarkerEventTimeCell(Row* row);
	wxString GetValueText();
	void SmallIncrease();
	void SmallDecrease();
	void LargeIncrease();
	void LargeDecrease();
	void Quantize();
};

class MarkerEventNameCell: public Cell
{
public:
	MarkerEventNameCell(Row* row);
	wxString GetValueText();
	void EnterValue();
};

#endif
