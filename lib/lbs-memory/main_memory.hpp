#pragma once

#include "define_t40.hpp"
#include <SD.h>
#include <SerialFlash.h>
#include <algorithm>
#include <string>
#include <vector>
#include <map>

namespace lbs {
    enum AUDIOTYPE {
        INVALID = 0,
        WAV = 1,
        RAW = 2
    };

    enum playbackMode {
        ONESHOT = 0,
        LOOP = 1,
    };

    struct PitchVal {
        int8_t value = -1;
    };

    struct mapping {
        String sample;
        playbackMode mode;
    };

    class MainMemory {

    public:
        MainMemory();

        void eraseFlash();

        uint getFreeSpacefromFlash();

        uint64_t getFreeSpacefromSD();

        bool flashEmpty()

        static std::vector<String> getFilelistFromFlash();

        static void printMapping();

        static void printAllFilesFromSD();

        String getSampleFromNote(uint8_t note);

        void loadSamplepack(const String &packName);

        static void loadSettings(const String &packName);

        static uint transferSampleToFlash(const String &filepath, size_t sampleSize);

        static std::vector<String> getSampleNamesFromPack(const String &packName);
        //std::vector< String > getAllSamplepacks();
        //std::vector<String> getAllFiles();
        //std::vector<String> getAllSamples( const String& samplepack );
        //void initializeSamplepack( const String& samplepack );

    private:
        uint freeSpaceFlash = 0;

        const static String mPackRootDir;
        static const std::map<String, PitchVal> pitches;
        static String sampleMapping[128];
        static playbackMode modeMapping[128];
    };
}