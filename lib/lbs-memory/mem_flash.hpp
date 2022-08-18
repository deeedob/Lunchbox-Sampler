#pragma once

#include <LittleFS.h>   //Flash memory on audio shield
#include <SerialFlash.h>
#include <SD.h>
#include <vector>
#include <string>
#include <mem_generic.h>
#include "define_t40.hpp"

namespace lbs {

    class MemFlash {
    public:
        MemFlash();

        std::vector<std::string> getAllFromFlash();

        //int getFreeSpaceinMB();
        //void eraseAll();
        void transferSingleToFlash(const std::string &filepath);
    };
}