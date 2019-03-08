#pragma once
#include <windows.h>   /* required before including mmsystem.h */
#include <mmsystem.h>  /* multimedia functions (such as MIDI) for Windows */

namespace midi
{
	typedef union { unsigned long word; unsigned char data[4]; } MIDIMessage;

	enum Command : unsigned char { NOTE_OFF = 128, NOTE_ON = 144, POLY_PRES = 160, CTRL_CHANGE = 176, PROG_CHANGE = 192, CHAN_PRES = 208, PITCH_WHEEL = 224 };

	class MIDIManager
	{
	private:
		bool open = false;
		
		HMIDIOUT device;

	public:
		int channel = 0;

		MIDIManager(int midiport, int _default_channel = 0);
		~MIDIManager();

		bool isOpen();

		int sendMsg(Command command, unsigned char p1, unsigned char p2 = 0, unsigned char p3 = 0);
		int sendMsg(MIDIMessage message);
	};

}