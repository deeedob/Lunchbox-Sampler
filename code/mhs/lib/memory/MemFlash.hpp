#pragma once
#include <Arduino.h>
#include <SerialFlash.h>
#include <SD.h>
#include <iostream>
#include <string>
#include <cstdio>
#include <memory>
#include <vector>
#include <unordered_map>
#include <algorithm>

#define SDCARD_CS_PIN   10
#define SDCARD_MOSI_PIN 11
#define SDCARD_SCK_PIN  13
#define FLASH_PIN 6

#define FLASHSIZE 16

namespace lbs {

    class MemFlash {

        class MidiMapping {

        public:
            bool add(std::string pitch, int8_t octave, std::string sample, std::string mode);
            std::vector<std::string> *getSampleList();
            MidiMapping();

        private:
            enum PlaybackMode {
                ONESHOT, LOOP, TOGGLE
            };
            static inline PlaybackMode getModeFromString(std::string mode);
            static const std::unordered_map<std::string, uint8_t> pitches;

            std::string samples[128];
            PlaybackMode mode[128];

        };

/* static part */
    public:
        static MemFlash &getInstance();

    public:
        bool loadSamplePack(std::string packName);

    private:
        MidiMapping mapping;
        static const std::string packfolder;
        std::string currentSamplePack;

        MemFlash();
    };
}
