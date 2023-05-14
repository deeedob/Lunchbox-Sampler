/**************************************************************************************************
 * Copyright (c) 2023. Dennis Oberst                                                              *
 **************************************************************************************************/
#include <Arduino.h>
#include <graphics.hpp>
#include <logger.hpp>

using namespace lbs;

#undef main

int main()
{
    Serial.begin(21600);
    delay(1000);
    LOG_INFO("Starting Lunchbox Sampler");

    Graphics g;

    return 0;
}
