#pragma once
#include <window.hpp>
/* forward declare to resolve circular dependencies!*/
class Window;


class WindowState
{
public:
    virtual void enter(Window* window) = 0;
    virtual void drawWindow(Window* window) = 0;
    virtual void exit(Window* window) = 0;
    virtual ~WindowState() {};
};