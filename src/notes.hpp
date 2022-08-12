#pragma once
#include <MIDI.h>
#include <vector>

class Notes
{
	struct Note {
		struct MidiData {
			u_int8_t type;
			u_int8_t channel;
			u_int8_t data1;
			u_int8_t data2;
		};
		MidiData m_midiData;
		int m_timing;
	};

public:
	Notes( int bpm, int bpb, int bars );
	~Notes() = default;
	//getter & setter:
	void setBpm( int bpm );
	void setBars( int bars );
	void setBpb( int bpb );
	void setNotes( std::vector< Notes > n_notes );
	int getBars();
	int getBpm();
	int getBpb();
	std::vector< Notes > getNotes();
	/* add function and test ist */
	void buttonPressed( int buttonPin, int note );
	void playLive();
	void sendMidiToDaw();
	void saveNote( Note data, bool isOverdub );
	void RecordFromDaw();
	void readRecord();
	void recordInternWithMidiClockGrid( uint32_t startTime, bool isMetronom, bool overdub );
	void recordInternNoGrid( uint32_t startTime, bool isMetronom, bool overdub );

private:
	const int m_buttonPin1 = 24;
	const int m_buttonPin2 = 26;
	std::vector< Note > m_notes;
	int m_bpm;
	int m_bars;
	int m_bpb;
};