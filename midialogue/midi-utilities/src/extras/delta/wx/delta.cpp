
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wx/wx.h>
#include <rtmidi_c.h>
#include <expat.h>
#include <midiutil-common.h>
#include <midiutil-rtmidi.h>
#include <midiutil-expat.h>
#include <midiutil-wx.h>

#define ACTION_NOOP 10000
#define ACTION_EXIT 10001

static wxTextCtrl* text_box;
static int startup_error = 0;
static RtMidiOutPtr midi_out = NULL;
static int channel_number = 0;
static int program_number = -1;
static int velocity = 64;
static int map[512];
static int down[512];
static int current_note = 63;
static int current_key_code = -1;

static void handle_xml_start_element(void *user_data, const XML_Char *name, const XML_Char **attributes)
{
	if (strcmp(name, "map") == 0)
	{
		int key = -1;
		wxString action = wxEmptyString;

		for (int i = 0; attributes[i] != NULL; i += 2)
		{
			if (strcmp(attributes[i], "key") == 0)
			{
				key = MidiUtil_getWxKeyCodeFromName(attributes[i + 1]);
			}
			else if (strcmp(attributes[i], "action") == 0)
			{
				action = attributes[i + 1];
			}
		}

		if ((key >= 0) && (action != wxEmptyString))
		{
			if (action == "noop")
			{
				map[key] = ACTION_NOOP;
			}
			else if (action == "exit")
			{
				map[key] = ACTION_EXIT;
			}
			else
			{
				map[key] = wxAtoi(action);
			}
		}
	}
}

static void send_note_on(int channel, int note, int velocity)
{
	if (midi_out != NULL)
	{
		unsigned char message[MIDI_UTIL_MESSAGE_SIZE_NOTE_ON];
		MidiUtilMessage_setNoteOn(message, channel, note, velocity);
		rtmidi_out_send_message(midi_out, message, MIDI_UTIL_MESSAGE_SIZE_NOTE_ON);
	}
}

static void send_note_off(int channel, int note)
{
	if (midi_out != NULL)
	{
		unsigned char message[MIDI_UTIL_MESSAGE_SIZE_NOTE_OFF];
		MidiUtilMessage_setNoteOff(message, channel, note, 0);
		rtmidi_out_send_message(midi_out, message, MIDI_UTIL_MESSAGE_SIZE_NOTE_OFF);
	}
}

static void send_program_change(int channel, int number)
{
	if (midi_out != NULL)
	{
		unsigned char message[MIDI_UTIL_MESSAGE_SIZE_PROGRAM_CHANGE];
		MidiUtilMessage_setProgramChange(message, channel, number);
		rtmidi_out_send_message(midi_out, message, MIDI_UTIL_MESSAGE_SIZE_PROGRAM_CHANGE);
	}
}

static void handle_key_down(wxKeyEvent& event)
{
	int key_code = event.GetKeyCode();

	if (!down[key_code])
	{
		wxString key_name = MidiUtil_getNameFromWxKeyCode((wxKeyCode)(key_code));
		int action = map[key_code];
		down[key_code] = 1;

		switch (action)
		{
			case ACTION_NOOP:
			{
				text_box->SetValue(wxString::Format("<map key=\"%s\" action=\"noop\" />", key_name));
				break;
			}
			case ACTION_EXIT:
			{
				wxExit();
				break;
			}
			default:
			{
				char note_name[128];

				send_note_off(channel_number, current_note);
				current_note = MidiUtil_clamp(current_note + action, 0, 127);
				send_note_on(channel_number, current_note, velocity);
				current_key_code = key_code;

				MidiUtil_setNoteNameFromNumber(action, note_name);
				text_box->SetValue(wxString::Format("<map key=\"%s\" action=\"%d\" />", key_name, action));
				break;
			}
		}
	}
}

static void handle_key_up(wxKeyEvent& event)
{
	int key_code = event.GetKeyCode();
	int action = map[key_code];
	down[key_code] = 0;

	switch (action)
	{
		case ACTION_NOOP:
		case ACTION_EXIT:
		{
			break;
		}
		default:
		{
			if (key_code == current_key_code) send_note_off(channel_number, current_note);
			break;
		}
	}
}

static void handle_kill_focus(wxFocusEvent& event)
{
	send_note_off(channel_number, current_note);
}

static void usage(wxString program_name)
{
	text_box->SetValue(wxString::Format("Usage:  %s [ --out <port> ] [ --channel <n> ] [ --program <n> ] [ --velocity <n> ] [ --map <filename.xml> ]\n", program_name));
	startup_error = 1;
}

class Application: public wxApp
{
public:
	bool OnInit();
	int OnExit();
};

bool Application::OnInit()
{
	int i;
	wxFrame *window = new wxFrame(NULL, wxID_ANY, "Delta", wxDefaultPosition, wxSize(640, 480));
	text_box = new wxTextCtrl(window, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE | wxTE_READONLY | wxBORDER_NONE);

	for (i = 0; i < 512; i++)
	{
		map[i] = ACTION_NOOP;
		down[i] = 0;
	}

	for (i = 1; i < this->argc; i++)
	{
		if (this->argv[i] == "--out")
		{
			if (++i == this->argc) usage(this->argv[0]);

			if ((midi_out = rtmidi_open_out_port((char *)("delta"), this->argv[i].char_str(), (char *)("delta"))) == NULL)
			{
				text_box->SetValue(wxString::Format("Error:  Cannot open MIDI output port \"%s\".\n", this->argv[i]));
				startup_error = 1;
			}
		}
		else if (this->argv[i] == "--channel")
		{
			if (++i == this->argc) usage(this->argv[0]);
			channel_number = wxAtoi(this->argv[i]);
		}
		else if (this->argv[i] == "--program")
		{
			if (++i == this->argc) usage(this->argv[0]);
			program_number = wxAtoi(this->argv[i]);
		}
		else if (this->argv[i] == "--velocity")
		{
			if (++i == this->argc) usage(this->argv[0]);
			velocity = wxAtoi(this->argv[i]);
		}
		else if (this->argv[i] == "--map")
		{
			XML_Parser xml_parser = XML_ParserCreate(NULL);
			char error_message[1024];
			if (++i == this->argc) usage(this->argv[0]);
			XML_SetStartElementHandler(xml_parser, handle_xml_start_element);

			if (XML_ParseFile(xml_parser, argv[i].char_str(), error_message, 1024) < 0)
			{
				text_box->SetValue(wxString::Format("Error:  %s\n", error_message));
				startup_error = 1;
			}

			XML_ParserFree(xml_parser);
		}
		else
		{
			usage(this->argv[0]);
		}
	}

	if (!startup_error)
	{
		if (program_number >= 0) send_program_change(channel_number, program_number);
		text_box->Bind(wxEVT_KEY_DOWN, handle_key_down);
		text_box->Bind(wxEVT_KEY_UP, handle_key_up);
		text_box->Bind(wxEVT_KILL_FOCUS, handle_kill_focus);
	}

	this->SetTopWindow(window);
	window->Show(true);
	return true;
}

int Application::OnExit()
{
	if (midi_out != NULL)
	{
		send_note_off(channel_number, current_note);
		rtmidi_close_port(midi_out);
	}

	return 0;
}

wxIMPLEMENT_APP(Application);

