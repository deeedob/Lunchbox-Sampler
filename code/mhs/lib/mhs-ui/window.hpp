#pragma once
#include <Adafruit_SSD1327.h>
#include <vector>
#include <window_state.hpp>

class WindowState;

class WindowConfigs {
public:
    static WindowConfigs& get();
    WindowConfigs(WindowConfigs const&) = delete;
    WindowConfigs(WindowConfigs &&) = delete;
    WindowConfigs& operator=(WindowConfigs const&) = delete;
    WindowConfigs& operator=(WindowConfigs &&) = delete;
    static WindowConfigs *getInstance();

protected:
    WindowConfigs(u_int8_t winPadx = 2, u_int8_t winPady = 2, u_int8_t txtPadx = 0, u_int8_t txtPady = 1);
    static WindowConfigs configs;

    std::vector<u_int8_t> currentWindowSize;
    std::pair<u_int8_t , u_int8_t> winPadding, txtPadding;
};


class Window : public Adafruit_SSD1327
{
public:
    Window( uint8_t w, uint8_t h, TwoWire *twi, int8_t rst_pin,
            u_int32_t preclk, uint32_t postclk);


    size_t printToWindow( const String &s, const std::vector<u_int8_t> &p);

    // windows are always rect
    struct Positions {
        std::vector<u_int8_t> first;
        std::vector<u_int8_t> second;
    };

    Positions splitHorizontal( const u_int8_t &pad, const float &split_ratio,
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
    u_int8_t txt_padding;
    std::vector<u_int8_t> currentPos;
    u_int8_t windowPadding_x, windowPadding_y, textPadding_y, textPadding_x;

private:
    size_t write( uint8_t uint8);
    /*TODO remove*/
    size_t _write(const uint8_t *buff, size_t size);
    size_t _print(const String &s);




};
