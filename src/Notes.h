
#include <Arduino.h>
#include <vector>
#include <MIDI.h>
#include <MIDIUSB.h>

#ifndef LUNCHBOX_SAMPLER_NOTES_H
#define LUNCHBOX_SAMPLER_NOTES_H
class Notes
{
  const int buttonPin1 = 24;
  const int buttonPin2 = 26;
  int BPM;
  int Bars;
  int BPB;
public:
  struct Note{
    struct MidiData
    {
      u_int8_t type;
      u_int8_t channel;
      u_int8_t data1;
      u_int8_t data2;
    };
    Note::MidiData midiData;
    int timing;
  };
  Notes(int BPM, int BPB, int Bars);
  void setBPM(int bpm);
  void setBars(int bars);
  void setBPB(int bpb);
  int getBars();
  int getBpm();
  int getBpb();
  void playLive();
  void buttonPressed(int buttonPin, int note);
  void recordIntern(uint32_t startTime, boolean isMetronom);
  void saveNote(int data1, int data2, int type, int channel, int timing);
  void RecordFromDaw();
  void sendMidiToDaw();
  void loopMidi(int count);
  void readRecord();
  std::vector<Note> getNotes();
};

#endif// LUNCHBOX_SAMPLER_NOTES_H