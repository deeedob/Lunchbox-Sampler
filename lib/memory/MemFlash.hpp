#pragma once
#ifdef VERBOSE

#include <Arduino.h>

#endif

#include <algorithm>
#include <Audio.h>
#include <cstdio>
#include <iostream>
#include <SerialFlash.h>
#include <SD.h>
#include <SPI.h>
#include <string>
#include <unordered_map>
#include <vector>
#include <Wire.h>
#include <map>
#include <tuple>

#include <MemGen.hpp>
#include <memory>

namespace lbs {
    bool transferToFlash(const std::string &filepath);

    class MemFlash {
/* static part */
    public:
        static MemFlash &getInstance();

    public:
        void purgeFlash();

        void playSample(const std::string &filename);

        std::string listFlash();

        friend bool transferToFlash(const std::string &filepath);


    private:
        static const std::string packfolder;
        AudioControlSGTL5000 audioShield;
        AudioPlaySerialflashRaw playFlashRaw1;  //xy=265.1999969482422,1208.1999969482422
        AudioOutputI2S i2s1;           //xy=613.1999969482422,1207.1999969482422
        MemFlash();
    };
}
