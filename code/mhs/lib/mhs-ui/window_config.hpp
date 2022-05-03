#pragma once

#include <vector>
#include <memory>
#include <Arduino.h>

class WindowConfigs {
public:
    using winArea = std::pair<std::pair<u_int8_t, u_int8_t>, std::pair<u_int8_t, u_int8_t>>; //custom type for this pairing madness
    enum class CLRS {
        CLR_ACTIVE = 0xff,
        CLR_PASSIVE = 0x66,
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

    void setActiveWinClrs( const std::pair<CLRS, CLRS> &activeWinClrs );

    const winArea &getActiveWindow() const;
    void setActiveWindow( const winArea &activeWindow );

    static const std::pair<CLRS, CLRS> &getActiveWinClrs();

    const winArea &getActiveTxtWindow() const;
    void setActiveTxtWindow( const winArea &activeTxtWindow );

    const std::pair<u_int8_t, u_int8_t> &getTxtSpacing() const;
    void setTxtSpacing( const std::pair<u_int8_t, u_int8_t> &txtSpacing );
private:
    explicit WindowConfigs(
            std::pair<u_int8_t, u_int8_t> winPad = {2, 2},
            std::pair<u_int8_t, u_int8_t> txtPad = {0, 0},
            std::pair<u_int8_t, u_int8_t> txtSpacing = {2,2},
            std::pair<std::pair<u_int8_t, u_int8_t>, std::pair<u_int8_t, u_int8_t>> activeWindow = {{0, 0}, {0, 0}} );

private:
    std::pair<u_int8_t, u_int8_t> winPadding, txtPadding, txtSpacing ; //set this if you want more padding in the window

    static std::pair<CLRS, CLRS> activeWinClrs; // set this to decide how clrs are treated in split windows
    winArea activeWindow; // the Active Window to draw in!
    winArea activeTxtWindow; //the current activeWindow. Important for drawing text without reimplementing Adafruit GFX completely...
};
