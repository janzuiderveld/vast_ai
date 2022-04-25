
#include <QtWidgets>
#include "all-events-lane.h"
#include "colors.h"
#include "controller-lane.h"
#include "lane.h"
#include "midifile.h"
#include "note-lane.h"
#include "marker-lane.h"
#include "tempo-lane.h"
#include "velocity-lane.h"
#include "window.h"

const QString Lane::NOTE_LANE_TYPE = "note-lane";
const QString Lane::VELOCITY_LANE_TYPE = "velocity-lane";
const QString Lane::CONTROLLER_LANE_TYPE = "controller-lane";
const QString Lane::TEMPO_LANE_TYPE = "tempo-lane";
const QString Lane::MARKER_LANE_TYPE = "marker-lane";
const QString Lane::ALL_EVENTS_LANE_TYPE = "all-events-lane";

Lane::Lane(Window* window, QString type)
{
	this->type = type;
	this->window = window;
	this->setFocusPolicy(Qt::StrongFocus);

	QSettings settings;
	this->background_color = settings.value("lane/background-color", Colors::buttonShade(255, 0)).value<QColor>();
	this->unselected_event_pen = QPen(settings.value("lane/unselected-event-border-color", Colors::buttonShade(0, 180)).value<QColor>());
	this->unselected_event_brush = QBrush(settings.value("lane/unselected-event-color", Colors::buttonShade(255, 0)).value<QColor>());
	this->unselected_event_text_pen = QPen(settings.value("lane/unselected-event-text-color", Colors::textShade(0, 255)).value<QColor>());
	this->selected_event_pen = QPen(settings.value("lane/selected-event-border-color", Colors::buttonShade(0, 180)).value<QColor>());
	this->selected_event_brush = QBrush(settings.value("lane/selected-event-color", Colors::buttonShade(200, 100)).value<QColor>());
	this->selected_event_text_pen = QPen(settings.value("lane/selected-event-text-color", Colors::textShade(0, 255)).value<QColor>());
	this->unselected_background_event_pen = QPen(settings.value("lane/unselected-background-event-border-color", Colors::buttonShade(175, 80)).value<QColor>());
	this->unselected_background_event_brush = QBrush(settings.value("lane/unselected-background-event-color", Colors::buttonShade(255, 100)).value<QColor>());
	this->unselected_background_event_text_pen = QPen(settings.value("lane/unselected-background-event-text-color", Colors::textShade(175, 80)).value<QColor>());
	this->selected_background_event_pen = QPen(settings.value("lane/selected-background-event-border-color", Colors::buttonShade(175, 80)).value<QColor>());
	this->selected_background_event_brush = QBrush(settings.value("lane/selected-background-event-color", Colors::buttonShade(220, 100)).value<QColor>());
	this->selected_background_event_text_pen = QPen(settings.value("lane/selected-background-event-text-color", Colors::textShade(175, 80)).value<QColor>());
	this->cursor_pen = QPen(settings.value("lane/cursor-color", Colors::buttonShade(0, 255)).value<QColor>());
	this->cursor_brush = QBrush(settings.value("lane/cursor-color", Colors::buttonShade(255, 0)).value<QColor>());
	this->selection_rect_pen = QPen(settings.value("lane/selection-rect-color", Colors::buttonShade(0, 255)).value<QColor>(), 1, Qt::DashLine);
	this->grid_line_pen = QPen(settings.value("lane/grid-line-color", Colors::buttonShade(230, 60)).value<QColor>());
	this->mouse_drag_threshold = settings.value("lane/mouse-drag-threshold", 12).toInt();
}

void Lane::paintEvent(QPaintEvent* event)
{
	Q_UNUSED(event)

	QPainter painter(this);

	// background

	this->paintBackground(&painter);

	// time lines

	if (this->window->getXFromTick(MidiFile_getTickFromBeat(this->window->sequence->midi_file, 1)) - this->window->getXFromTick(0) > 8)
	{
		painter.setPen(this->grid_line_pen);
		painter.setBrush(Qt::NoBrush);
		long min_tick = this->window->getTickFromX(0);
		long max_tick = this->window->getTickFromX(this->width());
		int min_beat = std::max((int)(MidiFile_getBeatFromTick(this->window->sequence->midi_file, min_tick)) - 1, 0);
		int max_beat = (int)(MidiFile_getBeatFromTick(this->window->sequence->midi_file, max_tick)) + 1;

		for (int beat = min_beat; beat < max_beat; beat++)
		{
			int x = this->window->getXFromTick(MidiFile_getTickFromBeat(this->window->sequence->midi_file, beat));
			painter.drawLine(x, 0, x, this->height());
		}
	}

	// selection rectangle

	int selected_events_x_offset = 0;
	int selected_events_y_offset = 0;

	if (this->mouse_operation == LANE_MOUSE_OPERATION_RECT_SELECT)
	{
 		if ((this->mouse_drag_x != this->mouse_down_x) && (this->mouse_drag_y != this->mouse_down_y))
		{
			painter.setPen(this->selection_rect_pen);
			painter.setBrush(Qt::NoBrush);
			painter.drawRect(this->mouse_down_x, this->mouse_down_y, this->mouse_drag_x - this->mouse_down_x, this->mouse_drag_y - this->mouse_down_y);
		}
	}
	else
	{
		if (this->mouse_drag_x_allowed) selected_events_x_offset = this->mouse_drag_x - this->mouse_down_x;
		if (this->mouse_drag_y_allowed) selected_events_y_offset = this->mouse_drag_y - this->mouse_down_y;
	}

	// events

	this->paintEvents(&painter, selected_events_x_offset, selected_events_y_offset);

	// cursor

	int cursor_x = this->window->getXFromTick(this->window->cursor_tick);
	painter.setPen(this->cursor_pen);
	painter.setBrush(this->cursor_brush);
	painter.drawLine(cursor_x, 0, cursor_x, this->height());

	if (this->hasFocus())
	{
		painter.drawEllipse(cursor_x - 2, this->cursor_y - 4, 4, 4);
		painter.drawEllipse(cursor_x - 2, this->cursor_y + this->getCursorGap(), 4, 4);
	}

	this->sequence_updated = false;
}

void Lane::mousePressEvent(QMouseEvent* event)
{
	int cursor_x = this->window->getXFromTick(this->window->cursor_tick);

	if (event->button() == Qt::LeftButton)
	{
		this->mouse_operation = LANE_MOUSE_OPERATION_NONE;
		this->mouse_down_x = event->pos().x();
		this->mouse_down_y = event->pos().y();
		this->mouse_drag_x = this->mouse_down_x;
		this->mouse_drag_y = this->mouse_down_y;
		this->mouse_drag_x_allowed = false;
		this->mouse_drag_y_allowed = false;

		MidiFileEvent_t midi_event = this->getEventFromXY(this->mouse_down_x, this->mouse_down_y);
		bool shift = ((event->modifiers() & Qt::ShiftModifier) != 0);
		bool create_undo_command = false;

		if (midi_event == NULL)
		{
			this->mouse_operation = LANE_MOUSE_OPERATION_RECT_SELECT;

 			if (!shift)
			{
				this->window->selectNone();

				if ((this->mouse_down_x == cursor_x) && (this->mouse_down_y == this->cursor_y))
				{
					midi_event = this->addEventAtXY(this->mouse_down_x, this->mouse_down_y);
					MidiFileEvent_setSelected(midi_event, 1);
					this->mouse_operation = LANE_MOUSE_OPERATION_NONE;
					create_undo_command = true;
				}
			}
		}
		else
		{
			if (MidiFileEvent_isSelected(midi_event))
			{
				if (shift)
				{
					MidiFileEvent_setSelected(midi_event, 0);
					this->mouse_operation = LANE_MOUSE_OPERATION_NONE;
				}
				else
				{
					this->track_number = MidiFileTrack_getNumber(MidiFileEvent_getTrack(midi_event));
					this->mouse_operation = LANE_MOUSE_OPERATION_DRAG_EVENTS;
					this->setDragOriginFromXY(this->mouse_down_x, this->mouse_down_y);
				}
			}
			else
			{
 				if (shift)
				{
					MidiFileEvent_setSelected(midi_event, 1);
					this->track_number = MidiFileTrack_getNumber(MidiFileEvent_getTrack(midi_event));
					this->mouse_operation = LANE_MOUSE_OPERATION_NONE;
				}
				else
				{
					this->window->selectNone();
					MidiFileEvent_setSelected(midi_event, 1);
					this->track_number = MidiFileTrack_getNumber(MidiFileEvent_getTrack(midi_event));
					this->mouse_operation = LANE_MOUSE_OPERATION_DRAG_EVENTS;
					this->setDragOriginFromXY(this->mouse_down_x, this->mouse_down_y);
				}
			}
		}

		this->window->sequence->update(create_undo_command);
	}
}

void Lane::mouseReleaseEvent(QMouseEvent* event)
{
	if (event->button() == Qt::LeftButton)
	{
		int mouse_up_x = event->pos().x();
		int mouse_up_y = event->pos().y();
		bool create_undo_command = false;

		if (this->mouse_operation == LANE_MOUSE_OPERATION_DRAG_EVENTS)
		{
			if ((this->mouse_drag_x_allowed && (mouse_up_x != this->mouse_down_x)) || (this->mouse_drag_y_allowed && (mouse_up_y != this->mouse_down_y)))
			{
				int x_offset = this->mouse_drag_x_allowed ? (this->mouse_drag_x - this->mouse_down_x) : 0;
				int y_offset = this->mouse_drag_y_allowed ? (this->mouse_drag_y - this->mouse_down_y) : 0;
				this->moveEventsByXY(x_offset, y_offset);
				create_undo_command = true;
			}
			else
			{
				QPoint midi_event_position = this->getPointFromEvent(this->getEventFromXY(this->mouse_down_x, this->mouse_down_y));

				if ((this->window->getXFromTick(this->window->cursor_tick) == midi_event_position.x()) && (this->cursor_y == midi_event_position.y()))
				{
					this->window->focusInspector();
				}
				else
				{
					this->window->cursor_tick = this->window->getTickFromX(midi_event_position.x());
					this->cursor_y = midi_event_position.y();
				}
			}
		}
		else if (this->mouse_operation == LANE_MOUSE_OPERATION_RECT_SELECT)
		{
			if ((mouse_up_x == this->mouse_down_x) && (mouse_up_y == this->mouse_down_y))
			{
				this->window->cursor_tick = this->window->getTickFromX(mouse_up_x);
				this->cursor_y = mouse_up_y;
			}
			else
			{
				this->selectEventsInRect(std::min(this->mouse_down_x, mouse_up_x), std::min(this->mouse_down_y, mouse_up_y), std::abs(mouse_up_x - this->mouse_down_x), std::abs(mouse_up_y - this->mouse_down_y));
			}
		}

		this->mouse_operation = LANE_MOUSE_OPERATION_NONE;
		this->mouse_drag_x_allowed = false;
		this->mouse_drag_y_allowed = false;
		this->window->sequence->update(create_undo_command);
	}
}

void Lane::mouseMoveEvent(QMouseEvent* event)
{
	if (this->mouse_operation != LANE_MOUSE_OPERATION_NONE)
	{
		this->mouse_drag_x = event->pos().x();
		this->mouse_drag_y = event->pos().y();
		if (abs(this->mouse_drag_x - this->mouse_down_x) > this->mouse_drag_threshold) this->mouse_drag_x_allowed = true;
		if (abs(this->mouse_drag_y - this->mouse_down_y) > this->mouse_drag_threshold) this->mouse_drag_y_allowed = true;
		this->update();
	}
}

void Lane::wheelEvent(QWheelEvent* event)
{
	int x_offset = event->angleDelta().x();
	int y_offset = event->angleDelta().y();
	bool control = ((event->modifiers() & Qt::ControlModifier) != 0);

	if (control)
	{
		this->window->zoomXBy(x_offset / 960.0 + 1.0);
		this->zoomYBy(y_offset / 960.0 + 1.0);
	}
	else
	{
		this->window->scrollXBy(x_offset);
		this->scrollYBy(y_offset);
	}

	this->window->update();
}

void Lane::sequenceUpdated()
{
	this->sequence_updated = true;
	this->update();
}

void Lane::cut()
{
	this->copy_();
	this->window->delete_();
}

void Lane::copy_()
{
	MidiFile_t clipboard_midi_file = MidiFile_newFromTemplate(this->window->sequence->midi_file);
	bool has_multiple_selected_tracks = Sequence::midiFileHasMultipleSelectedTracks(this->window->sequence->midi_file);
	long first_selected_event_tick = -1;

	for (MidiFileEvent_t midi_event = MidiFile_getFirstEvent(this->window->sequence->midi_file); midi_event != NULL; midi_event = MidiFileEvent_getNextEventInFile(midi_event))
	{
		if (MidiFileEvent_isSelected(midi_event))
		{
			int track_number = MidiFileTrack_getNumber(MidiFileEvent_getTrack(midi_event));
			if ((track_number > 1) && !has_multiple_selected_tracks) track_number = 1;
			MidiFileEvent_t clipboard_midi_event = MidiFileTrack_copyEvent(MidiFile_getTrackByNumber(clipboard_midi_file, track_number, 1), midi_event);
			if (first_selected_event_tick < 0) first_selected_event_tick = MidiFileEvent_getTick(midi_event);
			MidiFileEvent_setTick(clipboard_midi_event, MidiFileEvent_getTick(midi_event) - first_selected_event_tick);
			MidiFileEvent_setSelected(clipboard_midi_event, 0);
		}
	}

	if (first_selected_event_tick >= 0)
	{
		QMimeData* mime_data = new QMimeData();
		mime_data->setData("audio/midi", Sequence::saveMidiFileToBuffer(clipboard_midi_file));
		QGuiApplication::clipboard()->setMimeData(mime_data);
	}

	MidiFile_free(clipboard_midi_file);
}

void Lane::paste()
{
	const QMimeData* mime_data = QGuiApplication::clipboard()->mimeData();
	if ((mime_data == NULL) || !mime_data->hasFormat("audio/midi")) return;
	MidiFile_t clipboard_midi_file = Sequence::loadMidiFileFromBuffer(mime_data->data("audio/midi"));
	if (clipboard_midi_file == NULL) return;
	this->window->selectNone();
	bool has_multiple_populated_tracks = Sequence::midiFileHasMultiplePopulatedTracks(clipboard_midi_file);

	for (MidiFileEvent_t clipboard_midi_event = MidiFile_getFirstEvent(clipboard_midi_file); clipboard_midi_event != NULL; clipboard_midi_event = MidiFileEvent_getNextEventInFile(clipboard_midi_event))
	{
		int track_number = MidiFileTrack_getNumber(MidiFileEvent_getTrack(clipboard_midi_event));
		if ((track_number > 1) && !has_multiple_populated_tracks) track_number = this->track_number;
		MidiFileEvent_t midi_event = MidiFileTrack_copyEvent(MidiFile_getTrackByNumber(this->window->sequence->midi_file, track_number, 1), clipboard_midi_event);
		MidiFileEvent_setTick(midi_event, MidiFileEvent_getTick(clipboard_midi_event) + this->window->cursor_tick);
		MidiFileEvent_setSelected(midi_event, 1);
	}

	MidiFile_free(clipboard_midi_file);
	this->window->sequence->update(true);
}

void Lane::zoomIn()
{
	this->zoomYBy(1.05f);
}

void Lane::zoomOut()
{
	this->zoomYBy(1 / 1.05f);
}

void Lane::setDragOriginFromXY(int x, int y)
{
	Q_UNUSED(x)
	Q_UNUSED(y)
	// noop by default
}

Lane* Lane::newLane(Window* window, QString type)
{
	if (type == Lane::NOTE_LANE_TYPE) return new NoteLane(window);
	if (type == Lane::VELOCITY_LANE_TYPE) return new VelocityLane(window);
	if (type == Lane::CONTROLLER_LANE_TYPE) return new ControllerLane(window);
	if (type == Lane::TEMPO_LANE_TYPE) return new TempoLane(window);
	if (type == Lane::MARKER_LANE_TYPE) return new MarkerLane(window);
	if (type == Lane::ALL_EVENTS_LANE_TYPE) return new AllEventsLane(window);
	return NULL;
}

void Lane::editEvent()
{
	int cursor_x = this->window->getXFromTick(this->window->cursor_tick);
	MidiFileEvent_t cursor_midi_event = this->getEventFromXY(cursor_x, this->cursor_y);

	if (cursor_midi_event == NULL)
	{
		this->window->selectNone();
		cursor_midi_event = this->addEventAtXY(cursor_x, this->cursor_y);
		MidiFileEvent_setSelected(cursor_midi_event, 1);
		this->window->sequence->update(true);
	}
	else
	{
		if (MidiFileEvent_isSelected(cursor_midi_event))
		{
			this->window->focusInspector();
		}
		else
		{
			this->window->selectNone();
			MidiFileEvent_setSelected(cursor_midi_event, 1);
			this->track_number = MidiFileTrack_getNumber(MidiFileEvent_getTrack(cursor_midi_event));
			this->window->sequence->update(false);
		}
	}
}

void Lane::selectEvent()
{
	MidiFileEvent_t cursor_midi_event = this->getEventFromXY(this->window->getXFromTick(this->window->cursor_tick), this->cursor_y);

	if (cursor_midi_event != NULL)
	{
		MidiFileEvent_setSelected(cursor_midi_event, !MidiFileEvent_isSelected(cursor_midi_event));
		this->window->sequence->update(false);
	}
}

void Lane::cursorUp()
{
	this->cursor_y--;
	this->update();
}

void Lane::cursorDown()
{
	this->cursor_y++;
	this->update();
}

