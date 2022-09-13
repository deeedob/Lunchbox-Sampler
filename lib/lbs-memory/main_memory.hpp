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
        //general functions
        static bool initMemory();

        static void printAllFilesFromSD();

        static uint64_t getFreeSpacefromSD();

        // flash chip specific functions
        static std::vector<String> getFilelistFromFlash();

        static bool flashEmpty();

        static void eraseFlash();

        static uint getFreeSpacefromFlash();

        // return midiNote Name as String
        static String getNoteName(uint8_t midiNote);

        // sample pack functions
        static std::vector<String> getAvailableSamplepacks();

        static std::vector<String> getSampleNamesFromPack(const String &packName);

        static void loadSamplepack(const String &packName);

        static String getLoadedPackName();

        static String getSampleFromNote(uint8_t note);

        static bool setSampleForNote(const String &sampleName, uint8_t midiNote);

        // mapping file functions
        static bool createStdMappingFile(const String &packName);

        static bool deleteMappingFile(const String &packName);

        static void createAllStdMappingFiles();

        static bool saveCurrentMappingToFile();

        static void printMapping();

    private:
        static uint freeSpaceFlash;
        const static String mPackRootDir;
        static const std::map<String, PitchVal> pitches;
        static String sampleMapping[128];
        static playbackMode modeMapping[128];
        static String currentPack;

        static void loadMappingFile(const String &packName);

        static uint transferSampleToFlash(const String &filepath, size_t sampleSize);
    };
}