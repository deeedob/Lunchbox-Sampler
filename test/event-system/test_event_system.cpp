#include <interrupt_system/analog_interrupts.hpp>
#include <interrupt_system/digital_interrupts.hpp>
#include <event_sytem.hpp>
#include <Arduino.h>
#include <Audio.h>

using namespace lbs;

#undef main

AudioPlaySdWav playSdWav;
AudioOutputI2S audioOutput;
AudioControlSGTL5000 audioShield;

AudioConnection          patchCord1(playSdWav, 0, audioOutput, 0);
AudioConnection          patchCord2(playSdWav, 1, audioOutput, 1);

void setupPins() {
    pinMode(BTN_ENTER_, INPUT_PULLUP);
    pinMode(BTN_RETURN_, INPUT_PULLUP);
    pinMode(BTN_TOGGLE_, INPUT_PULLUP);
    pinMode(ROTARY_A_, INPUT_PULLUP);
    pinMode(ROTARY_B_, INPUT_PULLUP);
    pinMode(LED_BUILTIN, INPUT);
    pinMode(FSR_SEL0_, OUTPUT);
    pinMode(FSR_SEL1_, OUTPUT);
    pinMode(FSR_SEL2_, OUTPUT);
    pinMode(FSR_POLL_, INPUT);
    SPI.setSCK(SDCARD_SCK_PIN_);
    SPI.setMOSI(SDCARD_MOSI_PIN_);
}

void t(std::function<void(u_int16_t)> f) {
    f(10);
}

int main() {
    Serial.begin(9600);
    Serial.println("Begin program");

    Serial.println(":::Begin Event System Test:::");
    setupPins();
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
    DigitalInterrupts digitalInterrupts(eventSystem);
    digitalInterrupts.enableAll();

    eventSystem->attachDigital(Events::DIGITAL::ROTARY_L, [](){
        Serial.println("Rotary Left");
    });
    eventSystem->attachDigital(Events::DIGITAL::ROTARY_R, [](){
        Serial.println("Rotary Right");
    });
    eventSystem->attachDigital(Events::DIGITAL::BTN_RETURN, [](){
        Serial.println("Button Return");
    });
    eventSystem->attachDigital(Events::DIGITAL::BTN_ENTER, [](){
        Serial.println("Button Enter");
    });
    eventSystem->attachDigital(Events::DIGITAL::BTN_TOGGLE, [](){
        Serial.println("Button Toggle");
    });

    //eventSystem->detachDigital(Events::DIGITAL::ROTARY_L);

    AnalogInterrupts analogInterrupts(eventSystem);
    auto& pots = analogInterrupts.getPots();
    pots->enableISR();

    Worker<std::function<void(u_int16_t)>, u_int16_t> f;


    while(true) {
        pots->update();
        pots->next();
        delay(200);
    }

    return 0;
}
