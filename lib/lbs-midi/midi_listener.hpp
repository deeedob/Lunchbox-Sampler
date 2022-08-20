#pragma once
#include <active_object/runnable.hpp>
#include <atomic>
#include <MIDI.h>
#include "note.hpp"

class MidiListener : public Runnable
{
public:
	/*!
	 * @brief The background Midi listener.
	 * Listens on Midi Input from the Host via USB and Midi In via 5Pin connector.
	 * Gets instantiated inside the scheduler
	 */
	explicit MidiListener( )
		: m_done(false)
	{
		MIDI_CREATE_DEFAULT_INSTANCE();
		MIDI.begin(MIDI_CHANNEL_OMNI);
		m_runnable = std::make_unique<std::thread>(&Runnable::runThread, this);
	}
	~MidiListener() override
	{
		m_done = true;
		m_runnable->join();
	}
	MidiListener(const MidiListener&) = delete;
	MidiListener& operator=(const MidiListener&) = delete;
	MidiListener(MidiListener&&) = delete;
	MidiListener& operator=(MidiListener&&) = delete;
	
	Note processNoteOn() {
		auto type = midi::NoteOn;
		auto channel = usbMIDI.getChannel();
		auto note = usbMIDI.getData1();
		auto velocity = usbMIDI.getData2();
		return Note{type, channel, note, velocity, 0};
	}

	void runTarget( void* arg ) override {
		while(!m_done) {
			if(usbMIDI.read()) {
				auto type = usbMIDI.getType();
				switch( type ) {
					case midi::NoteOn :
						Note n = processNoteOn();
						n.printMidiData();
						break;
				}
			}
		}
	}

private:
	std::atomic<bool> m_done;
	std::unique_ptr<std::thread> m_runnable;
};