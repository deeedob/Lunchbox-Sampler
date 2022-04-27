#pragma once
#include <Arduino.h>
#include <Wire.h> // for i2c
#include <window.hpp>

namespace mhs {

    class UI : private Window
    {

    public:
        enum class CLRS {
            CLR_ACTIVE = 0xcc,
            CLR_PASSIVE = 0xee,
            CLR_BG = 0x44
        };
        UI( uint8_t w, uint8_t h, TwoWire *twi = &Wire1, int8_t rst_pin = -1, u_int32_t preclk = 400000, uint32_t postclk = 10000);

        ~UI();
        void drawMenu();

    };

}
