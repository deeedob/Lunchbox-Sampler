/**************************************************************************************************
 * Copyright (c) 2022. Dennis Oberst                                                              *
 **************************************************************************************************/

#include "_Notes.h"

_Notes::_Notes(int BPM, int BPB, int Bars) {
  this->m_bpb = BPB;
  this->m_bpm = BPM;
  this->m_bars = Bars;
};

void _Notes::setBPM(int bpm) { m_bpm = bpm; }
void _Notes::setBars(int bars) { m_bars = bars; }
void _Notes::setBPB(int bpb) { m_bpb = bpb; }
int _Notes::getBars(){ return m_bars; }
int _Notes::getBpm() { return m_bpm; }
int _Notes::getBpb() { return m_bpb; }
void _Notes::playLive() {
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
void _Notes::buttonPressed(int buttonPin, int note) {
  usbMIDI.sendNoteOn(note, 70, 1, 0);
  while (digitalRead(buttonPin) == HIGH) {
    delayMicroseconds(60000000 / ( m_bpm * 24));
  }
  if (digitalRead(buttonPin) == LOW) {
    usbMIDI.sendNoteOff(note, 70, 1, 0);
    return;
  }
}
void _Notes::recordIntern(uint32_t startTime, boolean isMetronom) {
  int metro = 0;
  boolean isNote1 = false;
  boolean isNote2 = false;
  int clock = 0;
  while (clock <= ( m_bars * m_bpb * 24)) {
    if (micros() - startTime >= (60000000 / ( m_bpm * 24))) {
      clock++;
      metro++;
      startTime = micros();
    }
    if (isMetronom && metro == 24) {
      Serial.println("click");
      metro = 0;
    }
    if (digitalRead( m_buttonPin1 ) == HIGH && !isNote1) {
      isNote1 = true;
      saveNote({41, 70, midi::NoteOn, 1, clock});
    }
    if (digitalRead( m_buttonPin1 ) == LOW && isNote1) {
      isNote1 = false;
      saveNote({41, 70, midi::NoteOff, 1, clock});
    }
    if (digitalRead( m_buttonPin2 ) == HIGH && !isNote2) {
      isNote2 = true;
      saveNote({36, 70, midi::NoteOn, 1, clock});
    }
    if (digitalRead( m_buttonPin2 ) == LOW && isNote2) {
      isNote2 = false;
      saveNote({36, 70, midi::NoteOff, 1, clock});
    }
  }
}
void _Notes::saveNote(Note data) {
  for (Note n : m_notes) {
    auto itPos = m_notes.begin();
    if (n.timing < data.timing) {
      itPos++;
      continue;
    }
    if (n.timing >= data.timing) {
      auto newIt = m_notes.insert(itPos, data);
    }
  }
}
void _Notes::RecordFromDaw() {
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
        saveNote(usbMIDI.getData1(), usbMIDI.getData2(), type, usbMIDI.getChannel(), clock);
        break;
      }
      if (type == midi::Clock) {
        clock++;
        break;
      }
    }
  }
}
void _Notes::sendMidiToDaw() {
  long prevTiming = 0;
  for (Note note : notes) {
    int timing = note.timing;
    delayMicroseconds(((60000000 / (24 * m_bpm )) * (timing - prevTiming)));
    byte type = note.midiData.type;
    switch (type) {
    case midi::NoteOn:
      if (note.midiData.data2 > 0) {
        usbMIDI.sendNoteOn(note.midiData.data1, note.midiData.data2, note.midiData.channel);
      } else {
        usbMIDI.sendNoteOff(note.midiData.data1, note.midiData.data2, note.midiData.channel);
      }
      break;
    case midi::NoteOff:
      usbMIDI.sendNoteOff(note.midiData.data1, note.midiData.data2, note.midiData.channel);
      break;
    }
    prevTiming = note.timing;
  }
}
void _Notes::loopMidi(int count) {
  while (count > 0) {
    sendMidiToDaw();
    count--;
  }
}
void _Notes::readRecord() {
  for (Note n : m_notes) {
    Serial.print("Type: ");
    Serial.println(n.midiData.type);
    Serial.print("timing: ");
    Serial.println(n.timing);
    Serial.print("note: ");
    Serial.println(n.midiData.data1);
  }
}

void _Notes::setNotes( const std::vector< Note >& notes ) { m_notes = notes; }
std::vector< _Notes::Note> getNotes() {
  return m_notes;
}