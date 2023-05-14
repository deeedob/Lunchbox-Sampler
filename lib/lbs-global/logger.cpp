/**************************************************************************************************
 * Copyright (c) 2023. Dennis Oberst                                                              *
 **************************************************************************************************/
#include "logger.hpp"

std::mutex lbs::Logger::m_mutex;

const std::chrono::system_clock::time_point lbs::Logger::start_time = std::chrono::system_clock::now();
