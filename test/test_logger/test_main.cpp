/**************************************************************************************************
 * Copyright (c) 2023. Dennis Oberst                                                              *
 **************************************************************************************************/
#include <unity.h>
#include <Arduino.h>
#include "../../lib/lbs-global/logger.hpp"
#include <atomic>

using namespace lbs;

std::atomic<int> finish(0);

void log_in_thread(int sleep_time)
{
    int count = 0;
    int maxval = 25;
    while (count < maxval) {
        if (threads.id() == 1)
            LOG_INFO("TID: ", threads.id(), " ", count + 1, "/", maxval);
        else if (threads.id() == 2)
            LOG_WARNING("TID: ", threads.id(), " ", count + 1, "/", maxval);
        else
            LOG_ERROR("TID: ", threads.id(), " ", count + 1, "/", maxval);
        threads.delay(sleep_time);
        ++count;
    }
    LOG_ERROR("FINISHED EXECUTION WITH TID: ", threads.id());
    finish++;
}

int main()
{
    Serial.begin(921600);
    delay(2000);
    UNITY_BEGIN();

    LOG_INFO("STARTING LOG TEST");
    threads.addThread(log_in_thread, 75);
    threads.addThread(log_in_thread, 150);
    threads.addThread(log_in_thread, 165);

    do {
        LOG_DEBUG("MAIN TID: ", threads.id(), "FINISH: ", finish);
        threads.delay(500);
    } while (finish.load() < 3);

    LOG_DEBUG("FINISHED LOGGING TEST");
    UNITY_END();
}
