//
// Created by lenaw on 21.07.2022.
//
#include <Arduino.h>
#include <vector>
#include <map>
#include <MIDI.h>
#include <MIDIUSB.h>

#undef assert
#undef main

u_int8_t type;
u_int8_t channel;
u_int8_t data1;
u_int8_t data2;
float timing;
float length;
int BPM;
int bars;

class Notes {
    float timing;
    float length;
    int BPM;
};
struct Note{
    struct MidiData {
        u_int8_t type;
        u_int8_t channel;
        u_int8_t data1;
        u_int8_t data2;
    };
    Note::MidiData midiData;
    float timing;
};
std::vector<Note> notes(0);
void startRecord(long startTime);
void sendMidi();
void readRecord();
int main()
{
    BPM=120;
    Serial.begin(115200);
    Serial.println("isMain");
    delay(5000);
    startRecord(micros());
    readRecord();
}
void setBPM(int bpm)
{
    BPM=bpm;
}
void startRecord(long startTime) {
    Serial.println("start recording");
    float bars=1;
    float millisPerBeat = 60000000 / BPM;
    float millisPerBar = millisPerBeat * 4;
    float millisPer16th = millisPerBeat / 16;
    int clock=0;
    //while (micros()-startTime<=millisPerBar*bars) {
        /*if(millis()-startTime>=millisPer16th*counter)
        {
            counter++;
        }*/
        while(clock<=bars*4*24)
        {
         while (usbMIDI.read()) {
             if(usbMIDI.getType()==midi::Clock)
                {
                     clock++;
                     break;
                }
            Note note;
            byte type = usbMIDI.getType();
            note.midiData.type=type;
            switch (type) {
                case midi::NoteOn:
                    note.timing=clock;
                    note.midiData.data1 = usbMIDI.getData1();
                    note.midiData.data2 = usbMIDI.getData2();
                    note.midiData.channel =usbMIDI.getChannel();
                case midi::NoteOff:
                    note.timing=clock;
                    note.midiData.data1 = usbMIDI.getData1();
                    note.midiData.data2 = usbMIDI.getData2();
                    note.midiData.channel = usbMIDI.getChannel();
                    break;
            }
            notes.push_back(note);
        }
    }
}

void sendMidi(int starttime)
{
    MIDI_CREATE_INSTANCE(HardwareSerial,Serial1,MIDI)
    int timing=0;
    int clock=0;
  for(int i=0; i<notes.size(); i++)
  {
      timing=notes[i].timing;
      if(micros()-starttime>= 60000000/(24*BPM))
      {
          starttime=micros();
          clock++;
      }
      if(timing==clock)
      {
          usbMIDI.send(notes[i].midiData.type,notes[i].midiData.data1, notes[i].midiData.data2, notes[i].midiData.channel,)
      }
        byte type = MIDI.getType();
        switch (type) {
            case midi::NoteOn:
                note = MIDI.getData1();
                velocity = MIDI.getData2();
                channel = MIDI.getChannel();
                if (velocity > 0) {
                    usbMIDI.sendNoteOn(note, velocity, channel);
                } else {
                    usbMIDI.sendNoteOff(note, velocity, channel);

                }
                break;
            case midi::NoteOff:
                note = MIDI.getData1();
                velocity = MIDI.getData2();
                channel = MIDI.getChannel();
                usbMIDI.sendNoteOff(note, velocity, channel);
                break;
        }

    }
    usbMIDI.send(midi::Clock,0,0,1,0)
}
void readRecord()
{
    for(Note n: notes)
    {
        Serial.print("Type: ");
        Serial.println(n.midiData.type);
        Serial.print("timing: ");
        Serial.println(n.timing);
        Serial.print("note: ");
        Serial.println(n.midiData.data1);

    }
}

