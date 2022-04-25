
#include <wx/wx.h>
#include <midifile.h>
#include "sequence-editor.h"
#include "system-exclusive-event.h"

SystemExclusiveEventType* SystemExclusiveEventType::GetInstance()
{
	static SystemExclusiveEventType* instance = new SystemExclusiveEventType();
	return instance;
}

SystemExclusiveEventType::SystemExclusiveEventType()
{
	this->name = wxString("System exclusive");
	this->short_name = wxString("Sysex");
}

bool SystemExclusiveEventType::MatchesEvent(MidiFileEvent_t event)
{
	return (MidiFileEvent_getType(event) == MIDI_FILE_EVENT_TYPE_SYSEX);
}

Row* SystemExclusiveEventType::GetRow(SequenceEditor* sequence_editor, long step_number, MidiFileEvent_t event)
{
	return new SystemExclusiveEventRow(sequence_editor, step_number, event);
}

SystemExclusiveEventRow::SystemExclusiveEventRow(SequenceEditor* sequence_editor, long step_number, MidiFileEvent_t event): Row(sequence_editor, step_number, event)
{
	this->event_type = SystemExclusiveEventType::GetInstance();
	this->cells[0] = new EventTypeCell(this);
	this->cells[1] = new SystemExclusiveEventTimeCell(this);
	this->cells[2] = new SystemExclusiveEventTrackCell(this);
	this->cells[3] = new Cell(this);
	this->cells[4] = new Cell(this);
	this->cells[5] = new Cell(this);
	this->cells[6] = new Cell(this);
	this->cells[7] = new Cell(this);
}

SystemExclusiveEventTimeCell::SystemExclusiveEventTimeCell(Row* row): Cell(row)
{
	this->label = wxString("Time");
}

wxString SystemExclusiveEventTimeCell::GetValueText()
{
	return this->row->sequence_editor->step_size->GetTimeStringFromTick(MidiFileEvent_getTick(this->row->event));
}

SystemExclusiveEventTrackCell::SystemExclusiveEventTrackCell(Row* row): Cell(row)
{
	this->label = wxString("Track");
}

wxString SystemExclusiveEventTrackCell::GetValueText()
{
	return wxString::Format("%d", MidiFileTrack_getNumber(MidiFileEvent_getTrack(this->row->event)));
}

