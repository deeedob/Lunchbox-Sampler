#include <define_t40.hpp>
#include <interrupt_system/analog_interrupts.hpp>
#include <interrupt_system/digital_interrupts.hpp>
#include <event_sytem.hpp>
#include <Bounce.h>
#include <SD.h>
#include <Audio.h>

using namespace lbs;

AudioPlaySdWav playSdWav;
AudioOutputI2S audioOutput;
AudioControlSGTL5000 audioShield;

AudioConnection          patchCord1(playSdWav, 0, audioOutput, 0);
AudioConnection          patchCord2(playSdWav, 1, audioOutput, 1);
#undef main
int main() {
    pinMode(BTN_ENTER_, INPUT_PULLUP);
    pinMode(BTN_RETURN_, INPUT_PULLUP);
    pinMode(BTN_TOGGLE_, INPUT_PULLUP);
    pinMode(ROTARY_A_, INPUT_PULLUP);
    pinMode(ROTARY_B_, INPUT_PULLUP);
    pinMode(LED_BUILTIN, INPUT);
    SPI.setSCK(SDCARD_SCK_PIN_);
    SPI.setMOSI(SDCARD_MOSI_PIN_);
    Serial.begin(9600);
    Serial.println(":::Begin Event System Test:::");
    if (!(SD.begin(SDCARD_CS_PIN_))) {
        // stop here, but print a message
        Serial.println("Unable to access the SD card.  Program halted.");
        delay(500);
    }

    AudioMemory(8);
    audioShield.enable();
    audioShield.volume(0.35);
    playSdWav.play("01.WAV");
    delay(10);



    auto eventSystem = std::make_shared<EventSystem>();
    DigitalInterrupts dig_int(eventSystem);
    AnalogInterrupts an_int(eventSystem);
    dig_int.enableAll();
    an_int.enableAll();

    while( true ) {
        an_int.getPots()->startScan();
        delay(1000);
        an_int.getPots()->next();
    }

}

void todo() {

    attachInterrupt(digitalPinToInterrupt(BTN_RETURN_), []() {
        Bounce b(BTN_RETURN_, 25);
        b.update();

        if(b.fallingEdge()) {
            Serial.println("Interrup press");
        }
    }, CHANGE);
}