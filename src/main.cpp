#include <Arduino.h>
#include "notes.hpp"

#undef main

int BPM;
int Bars;
int BPB;
int buttonPin1=24;
int buttonPin2=26;

int main() {
  pinMode(buttonPin1, INPUT);
  pinMode(buttonPin2, INPUT);
  BPM = 120;
  Bars = 2;
  BPB = 4;
  Serial.begin(115200);
  Notes notes(BPM,BPB,Bars);
  //notes.recordIntern(micros(), false);
  //notes.playLive();
  notes.recordInternWithMidiClockGrid(micros(),false, false);
  notes.readRecord();
  //notes.getNotes();
}