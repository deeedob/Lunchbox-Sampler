/**************************************************************************************************
 * Copyright (c) 2023. Dennis Oberst                                                              *
 **************************************************************************************************/

#include "graphics.hpp"
#include "WireIMXRT.h"

#include <core_pins.h>
#include <logger.hpp>

namespace lbs
{
    Graphics::Graphics(uint16_t w, uint16_t h)
        : Adafruit_SSD1327(w, h, &Wire1)
    {
        LOG_DEBUG("Starting the graphics engine.");
        while (!begin(SSD1327_I2C_ADDRESS)) {
            yield();
            LOG_ERROR("Display not found!");
            threads.delay(500);
        }
        LOG_DEBUG("Graphics initialization successfully");

        clearDisplay();
        fillScreen(0xff);
        Adafruit_SSD1327::display();
    }

} // namespace lbs
