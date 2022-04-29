#pragma once

#include <vector>
#include <Arduino.h>

class WindowConfigs {
public:
    enum class CLRS {
        CLR_ACTIVE,
        CLR_PASSIVE,
        CLR_BG
    };
    WindowConfigs(WindowConfigs const&) = delete;
    WindowConfigs(WindowConfigs &&) = delete;
    WindowConfigs& operator=(WindowConfigs const&) = delete;
    WindowConfigs& operator=(WindowConfigs &&) = delete;
    static WindowConfigs& getInstance();

    const std::pair<u_int8_t, u_int8_t> &getWinPadding() const;
    void setWinPadding( const std::pair<u_int8_t, u_int8_t> &winPadding );
    const std::pair<u_int8_t, u_int8_t> &getTxtPadding() const;
    void setTxtPadding( const std::pair<u_int8_t, u_int8_t> &txtPadding );

private:
    explicit WindowConfigs(std::pair<u_int8_t, u_int8_t> winPad = {2, 2 }, std::pair<u_int8_t, u_int8_t> txtPad = {0, 0 });

    std::pair<u_int8_t, u_int8_t> winPadding, txtPadding;
};
