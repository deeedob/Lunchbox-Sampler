#include <define_t40.hpp>
#include <Arduino.h>
#include <ui.hpp>

void setup() {
    Serial.begin(9600);
    Serial.println("<<<<<<<<<<<<Begin program>>>>>>>>>>>>>>>");

    Wire1.begin();
    Window w(128, 128, &Wire1, DISPLAY_RST);
    if(!w.begin(SSD1327_I2C_ADDRESS)) {
        Serial.println("unable to init OLED");
        exit(1);
    }
    Serial.println("Starting Window");
    w.setTextWrap(true);

    w.configs->setWinPadding({8,4});
    w.draw(4,2);
}

void loop() {
}