#include "window_config.hpp"

#include <utility>

std::pair<WindowConfigs::CLRS, WindowConfigs::CLRS> WindowConfigs::activeWinClrs = {CLRS::CLR_ACTIVE, CLRS::CLR_PASSIVE};

WindowConfigs::WindowConfigs(
        std::pair<u_int8_t, u_int8_t> winPad,
        std::pair<u_int8_t, u_int8_t> txtPad,
        std::pair<u_int8_t, u_int8_t> txtSpacing,
        std::pair<std::pair<u_int8_t, u_int8_t>, std::pair<u_int8_t, u_int8_t>> activeWindow )
        : winPadding(std::move(winPad)), txtPadding(std::move(txtPad)), txtSpacing(std::move(txtSpacing)), activeWindow(std::move(activeWindow))
{ }

WindowConfigs &WindowConfigs::getInstance() {
    static WindowConfigs* instance = new WindowConfigs({2, 2});
    return *instance;
}

const std::pair<u_int8_t, u_int8_t>& WindowConfigs::getWinPadding() const {
    return winPadding;
}

void WindowConfigs::setWinPadding(const std::pair<u_int8_t, u_int8_t> &winPadding ) {
    WindowConfigs::winPadding = winPadding;
}

const std::pair<u_int8_t, u_int8_t>& WindowConfigs::getTxtPadding() const {
    return txtPadding;
}

void WindowConfigs::setTxtPadding(const std::pair<u_int8_t, u_int8_t> &txtPadding ) {
    WindowConfigs::txtPadding = txtPadding;
}

const std::pair<WindowConfigs::CLRS, WindowConfigs::CLRS> &WindowConfigs::getActiveWinClrs() {
    return activeWinClrs;
}

void WindowConfigs::setActiveWinClrs( const std::pair<CLRS, CLRS> &activeWinClrs ) {
    WindowConfigs::activeWinClrs = activeWinClrs;
}

const WindowConfigs::winArea &WindowConfigs::getActiveWindow() const {
    return activeWindow;
}

void WindowConfigs::setActiveWindow(const WindowConfigs::winArea &activeWindow ) {
    WindowConfigs::activeWindow = activeWindow;
}

const WindowConfigs::winArea &WindowConfigs::getActiveTxtWindow() const {
    return activeTxtWindow;
}

void WindowConfigs::setActiveTxtWindow(const WindowConfigs::winArea &activeTxtWindow ) {
    WindowConfigs::activeTxtWindow = activeTxtWindow;
}

const std::pair<u_int8_t, u_int8_t> &WindowConfigs::getTxtSpacing() const {
    return txtSpacing;
}

void WindowConfigs::setTxtSpacing( const std::pair<u_int8_t, u_int8_t> &txtSpacing ) {
    WindowConfigs::txtSpacing = txtSpacing;
}


