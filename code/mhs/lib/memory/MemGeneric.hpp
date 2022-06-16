#pragma once
#ifndef MHS_MEMGENERIC_H
#define MHS_MEMGENERIC_H

#define SDCARD_CS_PIN   10
#define SDCARD_MOSI_PIN 11
#define SDCARD_SCK_PIN  13
#define FLASH_PIN 6

class MemGeneric {

public: static MemGeneric *getInstance();
public: static MemGeneric *instance;

public:
    void listFlash();
    void purgeFlash();
    bool compare(File &file, SerialFlashFile &ffile);

    MemGeneric();
private:


};

#endif MHS_MEMGENERIC_H
