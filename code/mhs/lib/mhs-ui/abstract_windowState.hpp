#pragma once
#include "window.hpp"
#include <vector>
#include <Arduino.h>

class Window;

class AbstractWindowState
{
public:
    using winArea = std::pair<std::pair<u_int8_t, u_int8_t>, std::pair<u_int8_t, u_int8_t>>; //custom type for this pairing madness
    /* create variadic functions */

    virtual void enter(Window* win, ...) = 0;
    virtual void draw(Window* win, ...) = 0;
    virtual void exit(Window* win, ...) = 0;
    virtual bool equals(const AbstractWindowState& b) = 0;
    virtual winArea getWindowTextArea(bool b) const = 0;
    virtual ~AbstractWindowState() {}
};
