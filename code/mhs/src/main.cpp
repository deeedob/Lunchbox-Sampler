#include <ui.hpp>
#include <define_t40.hpp>



void setup() {
    Serial.begin(9600);
    Serial.println("<<<<<<<<<<<<Begin program>>>>>>>>>>>>>>>");
}

void loop() {
    Serial.print("POT0 : ");
    Serial.println(analogRead(POT0));
    Serial.print("POT1 : ");
    Serial.println(analogRead(POT1));
    Serial.print("POT2 : ");
    Serial.println(analogRead(POT2));
    Serial.print("POT3 : ");
    Serial.println(analogRead(POT3));
}