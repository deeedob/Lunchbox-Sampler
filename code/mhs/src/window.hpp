#pragma once
#include <Adafruit_SSD1327.h>
#include <vector>

class Window : public Adafruit_SSD1327
{
public:
    // windows are always rect
    struct Positions {
        std::vector<u_int8_t> first;
        std::vector<u_int8_t> second;
    };
    Window( uint8_t w, uint8_t h, TwoWire *twi,
            int8_t rst_pin, u_int32_t preclk,
            uint32_t postclk, u_int8_t p );
    size_t write( uint8_t uint8);
    size_t printToWindow( const String &s, const std::vector<u_int8_t> &p);
    Positions splitHorizontal( const u_int8_t &padding, const float &split_ratio,
                               const u_int8_t &round,
                               const u_int8_t &deepness, u_int8_t clr_left,
                               u_int8_t clr_right);

    void printCurrentPos() {
        Serial.println("current position of active window : ");
        for(unsigned char currentPo : currentPos) {
            Serial.println(currentPo);
        }
    }

protected:
    /* window is always a rectangle*/
    u_int8_t txt_padding;

private:
    std::vector<u_int8_t> currentPos;
    u_int8_t padding;
};