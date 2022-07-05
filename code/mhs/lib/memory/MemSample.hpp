#pragma once

#include "frozen/string.h"

#define SDCARD_CS_PIN   10
#define SDCARD_MOSI_PIN 11
#define SDCARD_SCK_PIN  13
#define FLASH_PIN 6

#define FLASHSIZE 16

class MemSample {

/* static part */
public: static MemSample *getInstance();

public:
    bool loadSamplePack(const char* path);
    String getCurrentSamplePack();
    SerialFlashFile getSample(uint8_t note);
    bool loadSample(const char*);
    bool replaceSample(const char* oldPath, const char* newPath);
    bool deleteSamplePack(const char* path);
    bool deleteSample(const char*);

private:

    class MidiMapping {

    public:
        String* getSample();
        uint8_t getNote();
        uint8_t getIntFromNote(String note);

    private:
        explicit MidiMapping();
        enum PlaybackMode { ONESHOT, LOOP, TOGGLE };
        static constexpr frozen::unordered_map<frozen::string, uint8_t, 12> pitches = {{"c", 0}, {"csh", 1}, {"d", 2}, {"dsh", 3}, {"e", 4}, {"f", 5}, {"fsh", 6}, {"g", 7}, {"gsh", 8}, {"a", 9}, {"ash", 10}, {"b", 11}};
        String *mapping;
        PlaybackMode *mode;
    };

    String currentSamplePack;
    explicit MemSample();
    bool testSize(const char* path);

};
