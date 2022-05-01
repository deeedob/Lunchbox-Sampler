#include <define_t40.hpp>
#include <Arduino.h>
#include <ui.hpp>
#include <concrete_windowStates.hpp>

Window w(128, 128, &Wire1, DISPLAY_RST, 1000000);
void setup() {
    Serial.begin(9600);
    Serial.println("<<<<<<<<<<<<Begin program>>>>>>>>>>>>>>>");

    Wire1.begin();
    if(!w.begin(SSD1327_I2C_ADDRESS)) {
        Serial.println("unable to init OLED");
        exit(1);
    }
    w.clearDisplay();
    w.display();

    Serial.println("Starting Window");
    w.setTextWrap(true);

    w.configs->setWinPadding({4,4});
    w.draw(2,2);
    Serial.println("Here");
    //w.setState(WindowHorizSplit::getInstance());
    //w.draw(4,1);
}

void loop() {
}