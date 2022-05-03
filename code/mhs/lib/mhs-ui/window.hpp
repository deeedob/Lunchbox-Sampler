#pragma once

#include "abstract_windowState.hpp"
#include "window_config.hpp"
#include <Adafruit_SSD1327.h>
#include <gfxfont.h>
#include <cstdarg>
#include <memory>
#include <typeinfo>

class AbstractWindowState;
/*!
 * @ingroup UI
 * The main Window System allowing us to control and draw Windows.
 */
class Window : public Adafruit_SSD1327
{
public:
    WindowConfigs* configs;

    Window( uint8_t w, uint8_t h, TwoWire *twi = &Wire1,
            int8_t rst_pin = -1, uint32_t preclk = 400000,
            uint32_t postclk = 100000 );
    /*!
     * Get a pointer to the current state singleton.
     */
    inline AbstractWindowState* getCurrentState() const { return currentState; }
    /*!
     * Draws the current State,
     */
    void draw(u_int8_t roundness = 2, u_int8_t iteration = 1, float split = 0.5f);
    /*!
     * Print a String to the current Window State
     * @param s The String that should be printed
     * @param ... additional bool argument to decide printing to the first or second Window. false: first Window, true: second Window
     */
    void printToWindow( const String &s...);
    /*!
     * Set the current Window State
     */
    void setState(AbstractWindowState& newState);

    /* TODO: is this possible when using in concrete_windowState? */
    //friend std::pair<u_int8_t, u_int8_t> operator+( const std::pair<u_int8_t,u_int8_t>& lhs, const std::pair<uint8_t, uint8_t>& rhs);
    //friend std::pair<u_int8_t, u_int8_t>& operator+=( std::pair<u_int8_t,u_int8_t>& lhs, const std::pair<uint8_t, uint8_t>& rhs);
private:
    size_t write( uint8_t c ) override;
    AbstractWindowState* currentState;
};
