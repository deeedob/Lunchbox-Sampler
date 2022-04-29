#pragma once

#include "abstract_windowState.hpp"
#include "window_config.hpp"
#include <Adafruit_GFX.h>
#include <cstdarg>
#include <typeinfo>

class AbstractWindowState;

class Window : public Adafruit_GFX
{
public:
    WindowConfigs* configs;

    Window(const int16_t& w, const int16_t& h);

    inline AbstractWindowState* getCurrentState() const { return currentState; }
    void draw();
    void print(const String &s...);
    void setState(AbstractWindowState& newState);

    size_t write( uint8_t uint8 ) override;

private:
    AbstractWindowState* currentState;
};
