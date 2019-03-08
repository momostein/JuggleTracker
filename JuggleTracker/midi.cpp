#include "midi.h"

namespace midi
{
	MIDIManager::MIDIManager(int midiport, int channel) : channel(channel)
	{
		int flag = midiOutOpen(&device, midiport, 0, 0, CALLBACK_NULL);

		if (flag != MMSYSERR_NOERROR) {
			open = false;
			return;
		}
		else
		{
			open = true;
		}



	}
	MIDIManager::~MIDIManager()
	{
		if (open)
		{
			// turn any MIDI notes currently playing:
			midiOutReset(device);

			// Remove any data in MIDI device and close the MIDI Output port
			midiOutClose(device);
		}
	}

	bool MIDIManager::isOpen()
	{
		return open;
	}

	int MIDIManager::sendMsg(Command command, unsigned char p1, unsigned char p2, unsigned char p3)
	{
		MIDIMessage message;
		message.data[0] = command + channel;
		message.data[1] = p1;
		message.data[2] = p2;
		message.data[3] = p3;

		return sendMsg(message);
	}

	int MIDIManager::sendMsg(MIDIMessage message)
	{
		return midiOutShortMsg(device, message.word);
	}
}