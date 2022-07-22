#pragma once

#define SDCARD_CS_PIN   10
#define SDCARD_MOSI_PIN 11
#define SDCARD_SCK_PIN  13
#define FLASH_PIN 6

class Memory {

public:
    static int searchFreeMidi();

    static void listFlash();

    static bool loadSamplePack(const char*);
    static bool deleteSamplePack(const char* path);

    static bool loadSample(const char*);
    static SerialFlashFile getSample(const char* path);
    static bool replaceSample(const char* oldPath, const char* newPath);
    static bool deleteSample(const char*);

    static void purgeFlash();

    static void readSettings();
    static void updateSettings();
    static void configSettings();

    static bool compare(File &file, SerialFlashFile &ffile);

    static void setSettingsFile(String *settings);

private:
    static SerialFlashFile file;
    static String *settings;

};