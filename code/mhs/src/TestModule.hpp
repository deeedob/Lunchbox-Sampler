#pragma once

#include "abstract_module.hpp"
#include <memory>
#include <window.hpp>

class MainWindow : AbstractModule {
    MainWindow();
private:
    std::unique_ptr<Window> display;
    void enter() override;
    void exit() override;

    void customOnRotaryRight();
    void customOnButtonReturn(); //TODO: ButtonReturn classic mode only in UI?
};
