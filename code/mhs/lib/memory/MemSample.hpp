#pragma once
#ifndef MHS_MEMSAMPLE_H
#define MHS_MEMSAMPLE_H

#define SDCARD_CS_PIN   10
#define SDCARD_MOSI_PIN 11
#define SDCARD_SCK_PIN  13
#define FLASH_PIN 6

class MemSample {

/* static part */
public: static MemSample *getInstance();
public: static MemSample *instance;

public:
    String *settings;

    String getCurrentSamplePack();
    bool loadSamplePack(const char*);
    bool deleteSamplePack(const char* path);
    bool loadSample(const char*);
    SerialFlashFile getSample(const char* path);
    bool replaceSample(const char* oldPath, const char* newPath);
    bool deleteSample(const char*);
    void setSettingsFile(String *settings);
    void readSettings();
    void updateSettings();
    void configSettings();
    int searchFreeMidi();
    MemGeneric *mgen();

private:
    SerialFlashFile file;
    String currentSamplePack;

    MemSample();

};

#endif MHS_MEMSAMPLE_H
