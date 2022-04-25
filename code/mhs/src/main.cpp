#include <ui.hpp>
#include <define_t40.hpp>

using namespace mhs;

void setup() {
    UI ui(Adafruit_SSD1327(128,128, &Wire1, DISPLAY_RST, 1000000));
    ui.drawBase();
}

void loop() {

}