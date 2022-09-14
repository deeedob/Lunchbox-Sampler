#pragma once

#include "define_t40.hpp"
#include <SD.h>
#include <SerialFlash.h>
#include <algorithm>
#include <string>
#include <vector>
#include <map>
#include <observer.hpp>

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

    class MainMemory : public Observable<std::pair<uint32_t, uint32_t>> {

    public:
        MainMemory();

        static void init();

        static MainMemory *instance();

        static void printAllFilesFromSD();

        static uint64_t getFreeSpacefromSD();

        static std::vector<String> getFilelistFromFlash();

        static bool flashEmpty();

        static void eraseFlash();

        static uint getFreeSpacefromFlash();

        static String getNoteName(uint8_t midiNote);

        static std::vector<String> getSamplePacksFromSD();

        static std::vector<String> getSampleNamesFromPack(const String &pack_name);

        static void loadSamplepack(const String &pack_name);

        static String getLoadedPackName();

        static String getSampleFromNote(uint8_t note);

        static bool setSampleForNote(const String &sampleName, uint8_t midiNote);

        static bool createStdMappingFile(const String &packName);

        static bool deleteMappingFile(const String &packName);

        static void createAllStdMappingFiles();

        static bool saveCurrentMappingToFile();

        //static void printMapping();
        static uint32_t transferSingleToFlash(const String &filepath, size_t sampleSize = 0);


    private:
        const static String m_packRootDir;
        static MainMemory *m_glue;
        static String currentPack;
        static uint freeSpaceFlash;
        static const std::map<String, PitchVal> pitches;
        static String sampleMapping[128];
        static playbackMode modeMapping[128];

        static void loadMappingFile(const String &packName);

    };
}// namespace lbs