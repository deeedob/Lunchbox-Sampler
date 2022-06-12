#pragma once

#define SDCARD_CS_PIN   10
#define SDCARD_MOSI_PIN 11
#define SDCARD_SCK_PIN  13
#define FLASH_PIN 6

class MemSample {

public:
    static MemSample *getInstance();
    String getCurrentSamplePack();
    bool loadSamplePack(const char*);
    bool deleteSamplePack(const char* path);
    bool loadSample(const char*);
    SerialFlashFile getSample(const char* path);
    bool replaceSample(const char* oldPath, const char* newPath);
    bool deleteSample(const char*);

private:
    SerialFlashFile file;
    String *settings;
    String currentSamplePack;
    static MemSample *instance;
    MemSample();

};