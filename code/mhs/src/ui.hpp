#pragma once
#include <Arduino.h>
#include <Wire.h> // for i2c
#include <Adafruit_SSD1327.h>

namespace mhs {
    class UI
    {
    public:
        explicit UI( const Adafruit_SSD1327& mDisplay );
        ~UI();
        void drawBase();

    private:
        Adafruit_SSD1327 m_display;
    };
}
