#include <ui.hpp>
#include <define_t40.hpp>

using namespace mhs;
UI ui(128,128, &Wire1, DISPLAY_RST, 1000000);

void setup() {
    Serial.begin(9600);
    Serial.println("<<<<<<<<<<<<Begin program>>>>>>>>>>>>>>>");
    ui.drawMenu();
}

void loop() {
}