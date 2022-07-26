#include <Arduino.h>
#include <vector>
#include "Notes.h"

const int buttonPin1 = 24;
const int buttonPin2 = 26;

Notes::Notes(int BPM, int BPB, int Bars) {
  this->BPB = BPB;
  this->BPM = BPM;
  this->Bars = Bars;
};
std::vector <Notes::Note> notes;
void Notes::setBPM(int bpm) { BPM = bpm; }
void Notes::setBars(int bars) { Bars = bars; }
void Notes::setBPB(int bpb) { BPB = bpb; }
int Notes::getBars(){ return Bars; }
int Notes::getBpm() { return BPM; }
int Notes::getBpb() { return BPB; }
void Notes::playLive() {
  while (true) {
    delayMicroseconds(60000000 / (BPM * 24));
    if (digitalRead(buttonPin1) == HIGH) {
      buttonPressed(buttonPin1, 41);
    }
    if (digitalRead(buttonPin2) == HIGH) {
      buttonPressed(buttonPin2, 36);
    }
  }
}
void Notes::buttonPressed(int buttonPin, int note) {
  usbMIDI.sendNoteOn(note, 70, 1, 0);
  while (digitalRead(buttonPin) == HIGH) {
    delayMicroseconds(60000000 / (BPM * 24));
  }
  if (digitalRead(buttonPin) == LOW) {
    usbMIDI.sendNoteOff(note, 70, 1, 0);
    return;
  }
}
void Notes::recordIntern(uint32_t startTime, boolean isMetronom) {
  int metro = 0;
  boolean isNote1 = false;
  boolean isNote2 = false;
  int clock = 0;
  while (clock <= (Bars * BPB * 24)) {
    if (micros() - startTime >= (60000000 / (BPM * 24))) {
      clock++;
      metro++;
      startTime = micros();
    }
    if (isMetronom && metro == 24) {
      Serial.println("click");
      metro = 0;
    }
    if (digitalRead(buttonPin1) == HIGH && !isNote1) {
      isNote1 = true;
      saveNote(41, 70, midi::NoteOn, 1, clock);
    }
    if (digitalRead(buttonPin1) == LOW && isNote1) {
      isNote1 = false;
      saveNote(41, 70, midi::NoteOff, 1, clock);
    }
    if (digitalRead(buttonPin2) == HIGH && !isNote2) {
      isNote2 = true;
      saveNote(36, 70, midi::NoteOn, 1, clock);
    }
    if (digitalRead(buttonPin2) == LOW && isNote2) {
      isNote2 = false;
      saveNote(36, 70, midi::NoteOff, 1, clock);
    }
  }
}
void Notes::saveNote(int data1, int data2, int type, int channel, int timing) {
  Note note;
  note.timing = timing;
  note.midiData.data1 = data1;
  note.midiData.data2 = data2;
  note.midiData.type = type;
  note.midiData.channel = channel;
  for (Note n : notes) {
    auto itPos = notes.begin();
    if (n.timing < note.timing) {
      itPos++;
      continue;
    }
    if (n.timing >= note.timing) {
      auto newIt = notes.insert(itPos, note);
    }
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
  while (clock <= (Bars * BPB * 24)) {
    while (usbMIDI.read()) {
      byte type = usbMIDI.getType();
      if (type == midi::NoteOn || type == midi::NoteOff) {
        // Midi OFF Noten müssen auch auf der nächsten Beat gespeichert werden,
        // da der Ton sonst kein off Signal bekommt und "für immer" weiter läuft
        // Midi On Noten hingegen dürfen nicht mehr akzeptiert werden, wenn sie auf dem nächsten Beat liegen,
        // da sonst Töne gedoppelt vorliegen, bzw. am Ende des Beats ebenfalls nur eine Note on ohne entsprechnede Note off existiert
        if (type == midi::NoteOn && clock >= (Bars * BPB * 24)) {
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
void Notes::sendMidiToDaw() {
  long prevTiming = 0;
  for (Note note : notes) {
    int timing = note.timing;
    delayMicroseconds(((60000000 / (24 * BPM)) * (timing - prevTiming)));
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
void Notes::loopMidi(int count) {
  while (count > 0) {
    sendMidiToDaw();
    count--;
  }
}
void Notes::readRecord() {
  for (Note n : notes) {
    Serial.print("Type: ");
    Serial.println(n.midiData.type);
    Serial.print("timing: ");
    Serial.println(n.timing);
    Serial.print("note: ");
    Serial.println(n.midiData.data1);
  }
}
std::vector<Notes::Note> getNotes() {
  return notes;
}

