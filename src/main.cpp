/**************************************************************************************************
 * Copyright (c) 2023. Dennis Oberst                                                              *
 **************************************************************************************************/
#include <Arduino.h>
#include "logger.hpp"

using namespace lbs;

#undef main

int main()
{
	Serial.begin(921600);
	delay(1000);
	LOG_INFO("Starting Lunchbox Sampler");
	return 0;
}
