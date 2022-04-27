#include "ui.hpp"


mhs::UI::UI( uint8_t w, uint8_t h, TwoWire *twi, int8_t rst_pin,
             u_int32_t preclk, uint32_t postclk) : Window(w, h, twi, rst_pin, preclk, postclk) {
    Wire1.begin();
    if(!this->begin(SSD1327_I2C_ADDRESS)) {
        Serial.println("unable to init OLED");
        exit(1);
    }
    Serial.println("Starting UI");
    this->setTextWrap(true);
}

void mhs::UI::drawMenu() {
    this->fillScreen(static_cast<uint16_t>(CLRS::CLR_BG));

    float val = static_cast<float>(analogRead(A0)) / 1024.f;
    Positions p = this->splitHorizontal(2, val, 4, 2,
                                        static_cast<u_int8_t>(CLRS::CLR_ACTIVE),
                                        static_cast<u_int8_t>(CLRS::CLR_PASSIVE));
    this->printToWindow("kskhjdkshdksjhdksjhdkjshdkshjdkshkdhsdsljadflskdjflksdjflskdjflksdjflskdjflksdjflskdjflskjd", p.first);
    this->printToWindow("kskhjdkshdksjhdksjhdkjshdkshjdkshkdhsdsljadflskdjflksdjflskdjflksdjflskdjflksdjflskdjflskjd", p.second);

    this->display();
}

mhs::UI::~UI() {
    Wire1.end();
}
