#pragma once
#include "window.hpp"


class Window;

class AbstractWindowState
{
public:
    /* create variadic functions */
    virtual void enter(Window* win, ...) = 0;
    virtual void draw(Window* win, ...) = 0;
    virtual void exit(Window* win, ...) = 0;
    virtual bool equals(const AbstractWindowState& b) = 0;
    virtual ~AbstractWindowState() {}
};
