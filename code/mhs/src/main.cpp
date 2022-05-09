#include <define_t40.hpp>
#include <ADC.h>
#include <ADC_Module.h>
#include <Bounce.h>
#include <TeensyThreads.h>
#include <ui.hpp>

volatile int count = 0;
void thread_func(int data) {
    while(1) count += data;
}

Bounce b(ROTARY_SW, 10);
ADC adc;
int pinAstateCurrent = LOW;
int pinAStateLast = pinAstateCurrent;

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

    adc.adc0->enableInterrupts([]() {
        Serial.println("adc0 interrupted!");
    });
    adc.adc1->enableInterrupts([]() {
        Serial.println("adc1 interrupted!");
    });


    threads.addThread(thread_func, 1);
}

void loop() {

    // A0 = 14 A1= 15,A3=24, A4=26

    //noInterrupts();
    /* interrupts disabled for critical section */
    //Serial.println("uninterruptable code");
    //interrupts(); // re-enable interrupts
    Serial.println(count);
    delay(500);
}