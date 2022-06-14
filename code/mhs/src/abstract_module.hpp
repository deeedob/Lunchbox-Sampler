#pragma once
#include <memory>
#include <window.hpp>

class AbstractModule {
private:
    virtual void enter() = 0;
    virtual void exit() = 0;
};