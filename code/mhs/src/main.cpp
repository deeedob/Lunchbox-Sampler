#include <ui.hpp>
#include <define_t40.hpp>

using namespace mhs;
UI::CLRS customCols;
UI ui(128,128, &Wire1, DISPLAY_RST, 1000000);

void setup() {
    Serial.println("<<<<<<<<<<<<Begin program>>>>>>>>>>>>>>>");
    Serial.begin(9600);
    ui.drawMenu();
}

void loop() {
}