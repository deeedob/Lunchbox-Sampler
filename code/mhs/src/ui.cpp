#include "ui.hpp"

mhs::UI::UI( const Adafruit_SSD1327& mDisplay ) : m_display(mDisplay) {
    Wire1.begin();
    if(!m_display.begin(SSD1327_I2C_ADDRESS)) {
        Serial.println("unable to init OLED");
        exit(1);
    }

    m_display.clearDisplay();
}

void mhs::UI::drawBase() {
    m_display.writeFillRect(0, 50, 128, 128, 0xff);
    m_display.display();
}

mhs::UI::~UI() {
    Wire1.end();
}
