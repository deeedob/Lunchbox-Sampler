#include <Arduino.h>
#include <define_t40.hpp>
#include <functional>
#include <map>
#include <MIDIUSB.h>
#include <MIDIcontroller.h>
#include <MIDI.h>

#undef assert
#undef main
void sendMidi();
void DAWTOMidi();
void MidiIn();

typedef struct {
    u_int8_t type;
    u_int8_t channel;
    u_int8_t data1;
    u_int8_t data2;
} midiData;

/*int main() {
    Serial.begin(115200);
    Serial.println(":::Starting MIDI Test :::");
    while(true) {
        DAWTOMidi();
    }

    return 0;
}*/
void DAWTOMidi()
{
    while(usbMIDI.read()) {
        int velocity, channel, note;
        // Is there a MIDI message incoming ?
        int type = usbMIDI.getType();
        Serial.print("type");
        Serial.println(type);
        switch (type) {
            case midi::NoteOn:
                note = usbMIDI.getData1();
                velocity = usbMIDI.getData2();
                channel = usbMIDI.getChannel();
                if (velocity > 0) {
                    Serial.println(note);
                    Serial.println(" ON");
                } else {
                    Serial.println(note);
                    Serial.println(" OFF");

                }
                break;
            case midi::NoteOff:
                note = usbMIDI.getData1();
                velocity = usbMIDI.getData2();
                channel = usbMIDI.getChannel();
                Serial.println(note);
                Serial.println(" OFF");
                break;
        }
    }

}
void MidiToDAW(){
    MIDI_CREATE_INSTANCE(HardwareSerial,Serial1,MIDI)
    int type, velocity, channel, note;

    if (MIDI.read()) {                    // Is there a MIDI message incoming ?
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
}
void MidiIn()
{
    MIDI_CREATE_INSTANCE(HardwareSerial, Serial1,MIDI)
    byte type;
    int velocity, channel,note;

    Serial.println(MIDI.getData1());
    if (MIDI.read()) {
        Serial.println("isMIDI");// Is there a MIDI message incoming ?
        byte type = MIDI.getType();
        switch (type) {
            case midi::NoteOn:
                note = MIDI.getData1();
                velocity = MIDI.getData2();
                channel = MIDI.getChannel();
                if (velocity > 0) {
                    Serial.println("NOTE ON: ");
                    Serial.println(note);
                    Serial.println(velocity);
                    Serial.println(channel);
                    Serial.println();

                } else {
                    Serial.println("NOTE OFF: ");
                    Serial.println(note);
                    Serial.println(velocity);
                    Serial.println(channel);
                    Serial.println();
                }
                break;
            case midi::NoteOff:
                note = MIDI.getData1();
                velocity = MIDI.getData2();
                channel = MIDI.getChannel();
                //usbMIDI.sendNoteOff(note, velocity, channel);
                Serial.println("NOTE OFF: ");
                Serial.println(note);
                Serial.println(velocity);
                Serial.println(channel);
                Serial.println();
                break;
        }
    }

}

