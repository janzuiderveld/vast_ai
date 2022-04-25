
#include <wx/wx.h>
#include <midifile.h>
#include "lyric-event.h"
#include "sequence-editor.h"

LyricEventType* LyricEventType::GetInstance()
{
	static LyricEventType* instance = new LyricEventType();
	return instance;
}

LyricEventType::LyricEventType()
{
	this->name = wxString("Lyric");
	this->short_name = wxString("Lyric");
}

bool LyricEventType::MatchesEvent(MidiFileEvent_t event)
{
	return MidiFileEvent_isLyricEvent(event);
}

Row* LyricEventType::GetRow(SequenceEditor* sequence_editor, long step_number, MidiFileEvent_t event)
{
	return new LyricEventRow(sequence_editor, step_number, event);
}

LyricEventRow::LyricEventRow(SequenceEditor* sequence_editor, long step_number, MidiFileEvent_t event): Row(sequence_editor, step_number, event)
{
	this->event_type = LyricEventType::GetInstance();
	this->cells[0] = new EventTypeCell(this);
	this->cells[1] = new LyricEventTimeCell(this);
	this->cells[2] = new LyricEventTrackCell(this);
	this->cells[3] = new Cell(this);
	this->cells[4] = new LyricEventLyricCell(this);
	this->cells[5] = new Cell(this);
	this->cells[6] = new Cell(this);
	this->cells[7] = new Cell(this);
}

LyricEventTimeCell::LyricEventTimeCell(Row* row): Cell(row)
{
	this->label = wxString("Time");
}

wxString LyricEventTimeCell::GetValueText()
{
	return this->row->sequence_editor->step_size->GetTimeStringFromTick(MidiFileEvent_getTick(this->row->event));
}

LyricEventTrackCell::LyricEventTrackCell(Row* row): Cell(row)
{
	this->label = wxString("Track");
}

wxString LyricEventTrackCell::GetValueText()
{
	return wxString::Format("%d", MidiFileTrack_getNumber(MidiFileEvent_getTrack(this->row->event)));
}

LyricEventLyricCell::LyricEventLyricCell(Row* row): Cell(row)
{
	this->label = wxString("Lyric");
}

wxString LyricEventLyricCell::GetValueText()
{
	return wxString(MidiFileLyricEvent_getText(this->row->event));
}

