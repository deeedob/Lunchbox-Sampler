#pragma once

#include "abstract_windowState.hpp"
#include "window_config.hpp"
#include <Adafruit_SSD1327.h>
#include <gfxfont.h>
#include <cstdarg>
#include <typeinfo>

class AbstractWindowState;

class Window : public Adafruit_SSD1327
{
public:
    WindowConfigs* configs;

    Window( uint8_t w, uint8_t h, TwoWire *twi = &Wire1,
            int8_t rst_pin = -1, uint32_t preclk = 400000,
            uint32_t postclk = 100000 );

    inline AbstractWindowState* getCurrentState() const { return currentState; }
    void draw(u_int8_t roundness = 2, u_int8_t iteration = 1);
    void printToWindow( const String &s...);
    void setState(AbstractWindowState& newState);

    size_t write( uint8_t uint8 ) override;

    /* TODO: is this possible when using in concrete_windowState? */
    //friend std::pair<u_int8_t, u_int8_t> operator+( const std::pair<u_int8_t,u_int8_t>& lhs, const std::pair<uint8_t, uint8_t>& rhs);
    //friend std::pair<u_int8_t, u_int8_t>& operator+=( std::pair<u_int8_t,u_int8_t>& lhs, const std::pair<uint8_t, uint8_t>& rhs);


private:
    AbstractWindowState* currentState;
};
