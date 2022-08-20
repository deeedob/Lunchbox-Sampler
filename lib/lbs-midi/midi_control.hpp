#pragma once
#include <MIDI.h>
#include <stdint.h>

class MidiControl
{
public:
	struct Note {
		struct MidiData {
			uint8_t type;    //Midi Status tpes e.g. NoteOn NoteOff etc
			uint8_t channel; //Midi channel
			uint8_t note;    //Note as integer
			uint8_t velocity;//Velocity
		};
		MidiData m_midiData;
		int m_timing;
	};

	MidiControl();
	MidiControl( const MidiControl& other ) = default;
	MidiControl( MidiControl&& other ) = default;
	MidiControl& operator=( const MidiControl& other ) = default;
	MidiControl& operator=( MidiControl&& other ) = default;
	~MidiControl() = default;

	void midiReadContinuous() noexcept;

	static float getTimefromBPM( int bpm );
	static float getBPMfromTime( float time );

private:
	void playFromFlash( const Note& data );

private:
};