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
u_int8_t cnt = 0;
std::array<u_int16_t, 2> old_positions;

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
        old_positions[cnt%2] = adc.adc1->analogReadContinuous();
        Serial.print("ADC1: ");
        Serial.print(adc.adc1->analogReadContinuous());
        Serial.print(" POTI ");
        Serial.println(cnt%2);
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
    old_positions[0] = adc.adc1->readSingle();
    Serial.print("POT 0 INIT VAL : ");
    Serial.println(old_positions[0]);

    adc.adc1->startSingleRead(POT1);
    old_positions[1] = adc.adc1->readSingle();
    Serial.print("POT 1 INIT VAL : ");
    Serial.println(old_positions[1]);
/*
    adc.adc1->startSingleRead(POT2);
    old_positions[2] = adc.adc1->readSingle();
    Serial.print("POT 2 INIT VAL : ");
    Serial.println(old_positions[2]);

    adc.adc1->startSingleRead(POT3);
    old_positions[3] = adc.adc1->readSingle();
    Serial.print("POT 03INIT VAL : ");
    Serial.println(old_positions[3]);
    */
}

void loop() {

    Serial.println("POT0");
    adc.adc1->enableCompareRange( old_positions[0] - delta, old_positions[0] + delta , false, true);
    adc.adc1->startContinuous(POT0);
    delay(1000);
    adc.adc1->disableCompare();
    cnt++;

    Serial.println("POT1");
    adc.adc1->enableCompareRange( old_positions[1] - delta, old_positions[1] + delta , false, true);
    adc.adc1->startContinuous(POT1);
    delay(1000);
    adc.adc1->stopContinuous();
    adc.adc1->disableCompare();
    cnt++;

    Serial.println("POT3");
    adc.adc1->enableCompareRange( old_positions[1] - delta, old_positions[1] + delta , false, true);
    adc.adc1->startContinuous(POT2);
    delay(1000);
    adc.adc1->stopContinuous();
    adc.adc1->disableCompare();
    cnt++;

    Serial.println("POT2");
    adc.adc0->enableCompareRange( old_positions[1] - delta, old_positions[1] + delta , false, true);
    adc.adc0->startContinuous(POT2);
    delay(1000);
    adc.adc0->stopContinuous();
    adc.adc0->disableCompare();
    cnt++;
}