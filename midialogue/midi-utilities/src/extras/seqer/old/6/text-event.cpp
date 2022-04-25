
#include <wx/wx.h>
#include <midifile.h>
#include "sequence-editor.h"
#include "text-event.h"

TextEventType* TextEventType::GetInstance()
{
	static TextEventType* instance = new TextEventType();
	return instance;
}

TextEventType::TextEventType()
{
	this->name = wxString("Text");
	this->short_name = wxString("Text");
}

bool TextEventType::MatchesEvent(MidiFileEvent_t event)
{
	return MidiFileEvent_isTextEvent(event);
}

Row* TextEventType::GetRow(SequenceEditor* sequence_editor, long step_number, MidiFileEvent_t event)
{
	return new TextEventRow(sequence_editor, step_number, event);
}

TextEventRow::TextEventRow(SequenceEditor* sequence_editor, long step_number, MidiFileEvent_t event): Row(sequence_editor, step_number, event)
{
	this->event_type = TextEventType::GetInstance();
	this->cells[0] = new EventTypeCell(this);
	this->cells[1] = new TextEventTimeCell(this);
	this->cells[2] = new TextEventTrackCell(this);
	this->cells[3] = new Cell(this);
	this->cells[4] = new TextEventTextCell(this);
	this->cells[5] = new Cell(this);
	this->cells[6] = new Cell(this);
	this->cells[7] = new Cell(this);
}

TextEventTimeCell::TextEventTimeCell(Row* row): Cell(row)
{
	this->label = wxString("Time");
}

wxString TextEventTimeCell::GetValueText()
{
	return this->row->sequence_editor->step_size->GetTimeStringFromTick(MidiFileEvent_getTick(this->row->event));
}

TextEventTrackCell::TextEventTrackCell(Row* row): Cell(row)
{
	this->label = wxString("Track");
}

wxString TextEventTrackCell::GetValueText()
{
	return wxString::Format("%d", MidiFileTrack_getNumber(MidiFileEvent_getTrack(this->row->event)));
}

TextEventTextCell::TextEventTextCell(Row* row): Cell(row)
{
	this->label = wxString("Text");
}

wxString TextEventTextCell::GetValueText()
{
	return wxString(MidiFileTextEvent_getText(this->row->event));
}

