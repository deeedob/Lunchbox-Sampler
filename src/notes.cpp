#include "notes.hpp"
Notes::Notes( int bpm, int bpb, int bars )
	: m_bpm(bpm), m_bars(bars), m_bpb(bpb){}

void Notes::setBpm(int bpm) { m_bpm = bpm; }
void Notes::setBars(int bars) { m_bars = bars; }
void Notes::setBpb(int bpb) { m_bpb = bpb; }
int Notes::getBars(){ return m_bars; }
int Notes::getBpm() { return m_bpm; }
int Notes::getBpb() { return m_bpb; }
void Notes::recordInternWithMidiClockGrid(uint32_t startTime, bool isMetronom, bool overdub) {
	int metro = 0;
	Serial.println( "0" );
	bool isNote1 = false;
	bool isNote2 = false;
	int clock = 0;
	while( clock <= ( m_bars * m_bpb * 24 ) ) {
		if( micros() - startTime >= ( 60000000 / ( m_bpm * 24 ) ) ) {
			clock++;
			metro++;
			startTime = micros();
		}
		if( isMetronom && metro == 24 ) {
			Serial.println( "click" );
			metro = 0;
		}
		if( digitalRead( m_buttonPin1 ) == HIGH && !isNote1 ) {
			isNote1 = true;
			buttonPressed( m_buttonPin1, 41 );
			saveNote( { midi::NoteOn, 1, 41, 70, clock }, overdub );
		}
		if( digitalRead( m_buttonPin1 ) == LOW && isNote1 ) {
			isNote1 = false;
			saveNote( { midi::NoteOff, 1, 41, 70, clock }, overdub );
		}
		if( digitalRead( m_buttonPin2 ) == HIGH && !isNote2 ) {
			isNote2 = true;
			buttonPressed( m_buttonPin2, 36 );
			saveNote( { midi::NoteOn, 1, 36, 70, clock }, overdub );
		}
		if( digitalRead( m_buttonPin2 ) == LOW && isNote2 ) {
			isNote2 = false;
			saveNote( { midi::NoteOff, 1, 36, 70, clock }, overdub );
		}
	}
}
void Notes::playLive() {
	while (true) {
		delayMicroseconds(60000000 / ( m_bpm * 24));
		if (digitalRead( m_buttonPin1 ) == HIGH) {
			buttonPressed( m_buttonPin1, 41);
		}
		if (digitalRead( m_buttonPin2 ) == HIGH) {
			buttonPressed( m_buttonPin2, 36);
		}
	}
}
void Notes::buttonPressed(int buttonPin, int note) {
	usbMIDI.sendNoteOn(note, 70, 1, 0);
	while (digitalRead(buttonPin) == HIGH) {
		delayMicroseconds(60000000 / ( m_bpm * 24));
	}
	if (digitalRead(buttonPin) == LOW) {
		usbMIDI.sendNoteOff(note, 70, 1, 0);
		return;
	}
}
void Notes::RecordFromDaw() {
	Serial.println("start recording");
	// Starte Aufnahme, wenn MidiStartSignal sendet;
	while (true) {
		usbMIDI.read();
		if (usbMIDI.getType() == midi::Start) {
			break;
		}
	}
	int clock = -1;
	// Bevor eine Note geschickt wird, wird immer eine clock geschickt.
	
	// Nehme auf Für Anzahl eingestellter Bars, 24 Midi Clocks pro Beat (so oft wird die Clock message gesendet); Beats per Bar;
	while (clock <= ( m_bars * m_bpb * 24)) {
		while (usbMIDI.read()) {
			byte type = usbMIDI.getType();
			if (type == midi::NoteOn || type == midi::NoteOff) {
				// Midi OFF Noten müssen auch auf der nächsten Beat gespeichert werden,
				// da der Ton sonst kein off Signal bekommt und "für immer" weiter läuft
				// Midi On Noten hingegen dürfen nicht mehr akzeptiert werden, wenn sie auf dem nächsten Beat liegen,
				// da sonst Töne gedoppelt vorliegen, bzw. am Ende des Beats ebenfalls nur eine Note on ohne entsprechnede Note off existiert
				if (type == midi::NoteOn && clock >= ( m_bars * m_bpb * 24)) {
					break;
				}
				saveNote({usbMIDI.getData1(), usbMIDI.getData2(), type, usbMIDI.getChannel(), clock}, false);
				break;
			}
			if (type == midi::Clock) {
				clock++;
				break;
			}
		}
	}
}
void Notes::saveNote(Note data, bool isOverdub) {
	if(isOverdub) {
		int pos;
		for( Note n : m_notes ) {
			Serial.print("+");
			if( n.m_timing < data.m_timing ) {
				pos++;
			}
			else if( n.m_timing > data.m_timing ) {
				m_notes.insert( (m_notes.begin() + pos), data );
				    return;
			}
			 else if( n.m_timing == data.m_timing ) {
				m_notes.insert( (m_notes.begin() + pos+1), data );
				return;
			}
		}
	}
	else
	{
		m_notes.push_back(data);
	}
}
void Notes::readRecord() {
	if(m_notes.empty())
	{
		Serial.println("Notes empty");
	}
	for (Note n : m_notes) {
		Serial.print("Type: ");
		Serial.println(n.m_midiData.type);
		Serial.print("timing: ");
		Serial.println(n.m_timing);
		Serial.print("note: ");
		Serial.println(n.m_midiData.data1);
	}
}
void Notes::sendMidiToDaw() {
	Note prevnote=Note();
	prevnote.m_timing=0;
	for (Note note : m_notes) {
		int timing = note.m_timing;
		if(note.m_timing!=prevnote.m_timing)
		{
			delayMicroseconds(((60000000 / (24 * m_bpm )) * (timing - prevnote.m_timing)));
		}
		byte type = note.m_midiData.type;
	
		switch (type) {
			case midi::NoteOn:
				if (note.m_midiData.data2 > 0) {
					usbMIDI.sendNoteOn(note.m_midiData.data1, note.m_midiData.data2, note.m_midiData.channel);
				} else {
					usbMIDI.sendNoteOff(note.m_midiData.data1, note.m_midiData.data2, note.m_midiData.channel);
				}
				break;
			case midi::NoteOff:
				usbMIDI.sendNoteOff(note.m_midiData.data1, note.m_midiData.data2, note.m_midiData.channel);
				break;
		}
		prevnote = note;
	}
}
void Notes::recordInternNoGrid(uint32_t startTime, bool isMetronom, bool overdub) {
	bool isNote1 = false;
	bool isNote2 = false;
	while( micros() - startTime <= (m_bars * m_bpb *60000000/m_bpm) ) {
		if( digitalRead( m_buttonPin1 ) == HIGH && !isNote1 ) {
			isNote1 = true;
			buttonPressed( m_buttonPin1, 41 );
			saveNote( { midi::NoteOn, 1, 41, 70, micros() - startTime }, overdub );
		}
		if( digitalRead( m_buttonPin1 ) == LOW && isNote1 ) {
			isNote1 = false;
			saveNote( { midi::NoteOff, 1, 41, 70, micros() - startTime }, overdub );
		}
		if( digitalRead( m_buttonPin2 ) == HIGH && !isNote2 ) {
			isNote2 = true;
			buttonPressed( m_buttonPin2, 36 );
			saveNote( { midi::NoteOn, 1, 36, 70, micros() - startTime }, overdub );
		}
		if( digitalRead( m_buttonPin2 ) == LOW && isNote2 ) {
			isNote2 = false;
			saveNote( { midi::NoteOff, 1, 36, 70, micros() - startTime }, overdub );
		}
		
	}
}