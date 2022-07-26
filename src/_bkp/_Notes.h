/**************************************************************************************************
 * Copyright (c) 2022. Dennis Oberst                                                              *
 **************************************************************************************************/

#pragma once

#include <Arduino.h>
#include <MIDI.h>
#include <MIDIUSB.h>
#include <vector>

class _Notes
{

public:
	struct Note {
		struct MidiData {
			u_int8_t type;
			u_int8_t channel;
			u_int8_t data1;
			u_int8_t data2;
		};
		MidiData midiData;
		int timing;
	};
	_Notes( int BPM, int BPB, int Bars );
	void setBPM( int bpm );
	void setBars( int bars );
	void setBPB( int bpb );
	int getBars();
	int getBpm();
	int getBpb();
	void playLive();
	void buttonPressed( int buttonPin, int note );
	void recordIntern( uint32_t startTime, boolean isMetronom );
	void saveNote(Note data);
	void RecordFromDaw();
	void sendMidiToDaw();
	void loopMidi( int count );
	void readRecord();
	void setNotes( const std::vector< Note >& notes );

private:
	const int m_buttonPin1 = 24;
	const int m_buttonPin2 = 26;
	std::vector<Note> m_notes;
	int m_bpm;
	int m_bars;
	int m_bpb;
};