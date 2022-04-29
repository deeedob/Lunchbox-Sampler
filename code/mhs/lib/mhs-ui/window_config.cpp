#include "window_config.hpp"

WindowConfigs &WindowConfigs::getInstance() {
    static WindowConfigs* instance = new WindowConfigs();
    return *instance;
}

WindowConfigs::WindowConfigs( std::pair<u_int8_t, u_int8_t> winPad, std::pair<u_int8_t, u_int8_t> txtPad )
        : winPadding(std::move(winPad)), txtPadding(std::move(txtPad)){ }

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
