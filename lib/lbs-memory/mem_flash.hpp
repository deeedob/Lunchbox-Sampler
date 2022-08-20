#pragma once

#include <LittleFS.h>   //Flash memory on audio shield
#include <SerialFlash.h>
#include <SD.h>
#include <vector>
#include <string>
#include "define_t40.hpp"

namespace lbs {

    class MemFlash {
    public:
        MemFlash();

        static std::vector<std::string> getAllFromFlash();

        //int getFreeSpaceinMB();
        static void eraseFlash();

        static void transferSingleToFlash(const std::string &filepath);
    };
}