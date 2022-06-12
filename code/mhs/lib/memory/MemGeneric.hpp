#pragma once

#define SDCARD_CS_PIN   10
#define SDCARD_MOSI_PIN 11
#define SDCARD_SCK_PIN  13
#define FLASH_PIN 6

class MemGeneric {

public:
    static MemGeneric *getInstance();
    int searchFreeMidi();
    void listFlash();
    void purgeFlash();
    bool compare(File &file, SerialFlashFile &ffile);

    void setSettingsFile(String *settings);
    void readSettings();
    void updateSettings();
    void configSettings();

private:
    static MemGeneric *instance;
    String *settings;
    MemGeneric();

};