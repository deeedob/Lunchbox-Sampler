#include "lunchbox_sampler.hpp"

LunchboxSampler::LunchboxSampler() {
    setup();
}

LunchboxSampler::~LunchboxSampler() {

}


LunchboxSampler &LunchboxSampler::getInstance() {
    static auto* singleton = new LunchboxSampler();
    return *singleton;
}

/* todo: extend properly*/
void LunchboxSampler::setup() {
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

void LunchboxSampler::run() {
    while( true ) {
         yield();
    }
}
