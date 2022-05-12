#include <define_t40.hpp>
#include <ADC.h>
#include <ADC_Module.h>
#include <Bounce.h>
#include <TeensyThreads.h>
#include <ui.hpp>

Bounce b(ROTARY_SW, 2);
ADC adc;
int pinAstateCurrent = LOW;
int pinAStateLast = pinAstateCurrent;

FLASHMEM void flashfunc() {};
DMAMEM void otherMem(){};


u_int16_t delta = 10;
u_int16_t old_pot0, old_pot1, old_pot2, old_pot3;

void setup() {
    Serial.begin(9600);
    Serial.println("<<<<<<<<<<<<Begin program>>>>>>>>>>>>>>>");
    pinMode(ROTARY_SW, INPUT_PULLUP);
    pinMode(ROTARY_A, INPUT_PULLUP);
    pinMode(ROTARY_B, INPUT_PULLUP);

    attachInterrupt(digitalPinToInterrupt(ROTARY_SW), []() {
        b.update();
        if(b.fallingEdge()) {
            Serial.println("Interrup press");
            Serial.println(adc.analogRead(POT0));
        }

        if(b.risingEdge()) {
            Serial.println("Interrup release");
            Serial.println();
        }
    }, CHANGE);

    attachInterrupt(digitalPinToInterrupt(ROTARY_B), []() {
        pinAstateCurrent = digitalRead(ROTARY_A);
        if((pinAStateLast == LOW) && (pinAstateCurrent == HIGH)) {
            if( digitalReadFast(ROTARY_B) == HIGH) {
                Serial.println("Left");
            } else {
                Serial.println("Right");
            }
        }
        pinAStateLast = pinAstateCurrent;
    }, CHANGE);


    adc.adc1->enableInterrupts([]() {
        Serial.println(adc.adc1->analogReadContinuous());
        old_pot0 = adc.adc1->analogReadContinuous();
    });

    adc.adc0->enableInterrupts([]() {
        Serial.println(adc.adc0->analogReadContinuous());
    });

    adc.adc1->enableCompareRange( 100, 800 , true, true); // ready if value lies out of [1.0,2.0] V
    adc.adc0->enableCompareRange( 100, 800 , true, true); // ready if value lies out of [1.0,2.0] V
    //adc.adc1->enableCompare(500, true);

    //old_pot0 = analogRead(POT0);
    //old_pot1 = analogRead(POT1);
    //old_pot2 = analogRead(POT2);
    //old_pot3 = analogRead(POT3);
    /* continues */
    adc.adc1->analogReadContinuous();
    adc.adc0->analogReadContinuous();
    /* single */
    adc.adc1->startSingleRead(POT0);
    old_pot0 = adc.adc1->readSingle();
    Serial.print("POT 0 INIT VAL : ");
    Serial.println(old_pot0);
}

int value;
void loop() {
    adc.adc1->enableCompareRange( old_pot0 - delta, old_pot0 + delta , false, true);
    Serial.println("POT0");
    adc.adc1->startContinuous(POT0);
    delay(1000);
    adc.adc1->disableCompare();
/*
    Serial.println("POT1");
    adc.adc1->startContinuous(POT1);
    delay(1000);
    adc.adc1->stopContinuous();

    Serial.println("POT2");
    adc.adc0->startContinuous(POT2);
    delay(1000);

    Serial.println("POT3");
    adc.adc0->startContinuous(POT3);
    delay(1000);
    adc.adc0->stopContinuous();
    */
}