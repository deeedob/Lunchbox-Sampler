#pragma once
#ifndef MHS_MEMSD_H
#define MHS_MEMSD_H

#define SDCARD_CS_PIN   10
#define SDCARD_MOSI_PIN 11
#define SDCARD_SCK_PIN  13
#define FLASH_PIN 6

class MemSD {

public:
    MemSD *getInstance();
    String listFlash();
    void purgeFlash();
    bool compare(File &file, SerialFlashFile &ffile);
    void readSettings();
    void updateSettings();
    void configSettings();
    int searchFreeMidi();

private:
    explicit MemSD();

};

#endif
