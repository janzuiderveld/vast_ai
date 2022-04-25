#ifndef LANE_INCLUDED
#define LANE_INCLUDED

class Lane;

#include <QtWidgets>
#include "midifile.h"
#include "window.h"

enum LaneMouseOperation
{
	LANE_MOUSE_OPERATION_NONE,
	LANE_MOUSE_OPERATION_RECT_SELECT,
	LANE_MOUSE_OPERATION_DRAG_EVENTS
};

class Lane: public QWidget
{
	Q_OBJECT

public:
	static const QString NOTE_LANE_TYPE;
	static const QString VELOCITY_LANE_TYPE;
	static const QString CONTROLLER_LANE_TYPE;
	static const QString TEMPO_LANE_TYPE;
	static const QString MARKER_LANE_TYPE;
	static const QString ALL_EVENTS_LANE_TYPE;

	QString type;
	Window* window;
	bool sequence_updated = true;
	int track_number = 1;
	int channel = 0;
	int cursor_y = 0;
	LaneMouseOperation mouse_operation;
	int mouse_down_x = 0;
	int mouse_down_y = 0;
	int mouse_drag_x = 0;
	int mouse_drag_y = 0;
	bool mouse_drag_x_allowed = false;
	bool mouse_drag_y_allowed = false;
	QColor background_color;
	QPen unselected_event_pen;
	QBrush unselected_event_brush;
	QPen unselected_event_text_pen;
	QPen selected_event_pen;
	QBrush selected_event_brush;
	QPen selected_event_text_pen;
	QPen unselected_background_event_pen;
	QBrush unselected_background_event_brush;
	QPen unselected_background_event_text_pen;
	QPen selected_background_event_pen;
	QBrush selected_background_event_brush;
	QPen selected_background_event_text_pen;
	QPen cursor_pen;
	QBrush cursor_brush;
	QPen selection_rect_pen;
	QPen grid_line_pen;
	int mouse_drag_threshold;

	Lane(Window* window, QString type);
	void paintEvent(QPaintEvent* event);
	void mousePressEvent(QMouseEvent* event);
	void mouseReleaseEvent(QMouseEvent* event);
	void mouseMoveEvent(QMouseEvent* event);
	void wheelEvent(QWheelEvent* event);
	void sequenceUpdated();
	void cut();
	void copy_();
	void paste();
	void zoomIn();
	void zoomOut();
	void editEvent();
	void selectEvent();
	void cursorUp();
	void cursorDown();

	virtual void paintBackground(QPainter* painter) = 0;
	virtual void paintEvents(QPainter* painter, int selected_events_x_offset, int selected_events_y_offset) = 0;
	virtual MidiFileEvent_t getEventFromXY(int x, int y) = 0;
	virtual QPoint getPointFromEvent(MidiFileEvent_t midi_event) = 0;
	virtual MidiFileEvent_t addEventAtXY(int x, int y) = 0;
	virtual void moveEventsByXY(int x_offset, int y_offset) = 0;
	virtual void selectEventsInRect(int x, int y, int width, int height) = 0;
	virtual void scrollYBy(int offset) = 0;
	virtual void zoomYBy(float factor) = 0;
	virtual int getCursorGap() = 0;
	virtual void setDragOriginFromXY(int x, int y);

	static Lane* newLane(Window* window, QString type);
};

#endif
