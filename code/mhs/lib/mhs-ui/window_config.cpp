#include "window_config.hpp"

WindowConfigs::WindowConfigs(
        const std::pair<u_int8_t, u_int8_t> &winPad,
        const std::pair<u_int8_t, u_int8_t> &txtPad,
        const std::pair<CLRS, CLRS> &activeWinClrs,
        const std::pair<std::pair<u_int8_t, u_int8_t>, std::pair<u_int8_t, u_int8_t>> &activeWindow )
        : winPadding(std::move(winPad)), txtPadding(std::move(txtPad)), activeWinClrs(std::move(activeWinClrs)), activeWindow(std::move(activeWindow))
        { }

WindowConfigs &WindowConfigs::getInstance() {
    static WindowConfigs* instance = new WindowConfigs();
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

const std::pair<WindowConfigs::CLRS, WindowConfigs::CLRS> &WindowConfigs::getActiveWinClrs() const {
    return activeWinClrs;
}

void WindowConfigs::setActiveWinClrs( const std::pair<CLRS, CLRS> &activeWinClrs ) {
    WindowConfigs::activeWinClrs = activeWinClrs;
}

const std::pair<std::pair<u_int8_t, u_int8_t>, std::pair<u_int8_t, u_int8_t>> &WindowConfigs::getActiveWindow() const {
    return activeWindow;
}

void WindowConfigs::setActiveWindow(const std::pair<std::pair<u_int8_t, u_int8_t>, std::pair<u_int8_t, u_int8_t>> &activeWindow ) {
    WindowConfigs::activeWindow = activeWindow;
}
