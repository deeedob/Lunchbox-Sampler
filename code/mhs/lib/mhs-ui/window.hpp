#pragma once

#include "abstract_windowState.hpp"
#include "window_config.hpp"
#include <Adafruit_GrayOLED.h>
#include <gfxfont.h>
#include <cstdarg>
#include <typeinfo>

class AbstractWindowState;

class Window : public Adafruit_GrayOLED
{
public:
    WindowConfigs* configs;

    Window( const uint8_t &bpp, const uint16_t &w,
            const u_int16_t &h, TwoWire *twi,
            const int8_t &rst_pin, const uint32_t &clkDuring,
            const uint32_t &clkAfter );

    inline AbstractWindowState* getCurrentState() const { return currentState; }
    void draw();
    void printToWindow( const String &s...);
    void setState(AbstractWindowState& newState);

    size_t write( uint8_t uint8 ) override;

    /* TODO: is this possible when using in concrete_windowState? */
    //friend std::pair<u_int8_t, u_int8_t> operator+( const std::pair<u_int8_t,u_int8_t>& lhs, const std::pair<uint8_t, uint8_t>& rhs);
    //friend std::pair<u_int8_t, u_int8_t>& operator+=( std::pair<u_int8_t,u_int8_t>& lhs, const std::pair<uint8_t, uint8_t>& rhs);


private:
    AbstractWindowState* currentState;
};
