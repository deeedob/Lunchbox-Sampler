/**************************************************************************************************
 * Copyright (c) 2023. Dennis Oberst                                                              *
 **************************************************************************************************/
#pragma once
#include "WireIMXRT.h"
#include <Adafruit_SSD1327.h>

namespace lbs
{

    class Graphics: public Adafruit_SSD1327
    {
    public:
        Graphics(uint16_t w = 128, uint16_t h = 128);
    };

} // namespace lbs
