#pragma once

#include <vector>
#include <Arduino.h>

class WindowConfigs {
public:
    enum class CLRS {
        CLR_ACTIVE = 0xcc,
        CLR_PASSIVE = 0xee,
        CLR_BG = 0x44
    };
    /* make true singleton */
    WindowConfigs(WindowConfigs const&) = delete;
    WindowConfigs(WindowConfigs &&) = delete;
    WindowConfigs& operator=(WindowConfigs const&) = delete;
    WindowConfigs& operator=(WindowConfigs &&) = delete;
    static WindowConfigs& getInstance();

    /* getters and setters */
    const std::pair<u_int8_t, u_int8_t> &getWinPadding() const;
    void setWinPadding( const std::pair<u_int8_t, u_int8_t> &winPadding );

    const std::pair<u_int8_t, u_int8_t> &getTxtPadding() const;
    void setTxtPadding( const std::pair<u_int8_t, u_int8_t> &txtPadding );

    const std::pair<CLRS, CLRS> &getActiveWinClrs() const;
    void setActiveWinClrs( const std::pair<CLRS, CLRS> &activeWinClrs );

    const std::pair<std::pair<u_int8_t, u_int8_t>, std::pair<u_int8_t, u_int8_t>> &getActiveWindow() const;
    void setActiveWindow(const std::pair<std::pair<u_int8_t, u_int8_t>, std::pair<u_int8_t, u_int8_t>> &activeWindow );

private:
    explicit WindowConfigs(
            const std::pair<u_int8_t, u_int8_t> &winPad = {2, 2},
            const std::pair<u_int8_t, u_int8_t> &txtPad = {0, 0},
            const std::pair<CLRS, CLRS> &activeWinClrs = {CLRS::CLR_ACTIVE, CLRS::CLR_PASSIVE},
            const std::pair<std::pair<u_int8_t, u_int8_t>, std::pair<u_int8_t,u_int8_t>> &activeWindow = {{0,0}, {0,0}} );

    std::pair<u_int8_t, u_int8_t> winPadding, txtPadding; //set this if you want more padding in the window

    std::pair<CLRS, CLRS> activeWinClrs; // set this to decide how clrs are treated in split windows
    std::pair<std::pair<u_int8_t, u_int8_t>, std::pair<u_int8_t, u_int8_t>> activeWindow; //the current activeWindow. Important for drawing text without reimplementing Adafruit GFX completely...

};
