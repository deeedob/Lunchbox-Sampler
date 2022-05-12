#include <ui.hpp>

void setup() {
    Serial.begin(9600);
    Serial.println("<<<<<<<<<<<<Begin program>>>>>>>>>>>>>>>");
    UI ui;
    ui.start();
}

void loop() {
}