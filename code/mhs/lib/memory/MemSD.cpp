#include <Arduino.h>
#include <SD.h>
#include <SerialFlash.h>
#include <Bounce.h>
#include <iostream>

#include "MemSD.hpp"


MemGeneric
*MemGeneric::getInstance()
{
    static MemGeneric *instance;

    if (!instance)
        instance = new MemGeneric();

    return instance;
}

MemGeneric::MemGeneric()
{

    while(!Serial) {}
    Serial.println(F("listFlash: All Files on SPI Flash chip:"));

    /* Opening SerialPin? */
    if (!SerialFlash.begin(FLASH_PIN)) {
        Serial.println("Error initializing Flash chip");
    }

}

void
MemGeneric::listFlash()
{
    /* could cause infinity loop, reason ? */
    while (!Serial);


    unsigned int count = 0;

    /* announce reading filelist (prob setting file iterator to 0) */
    SerialFlash.opendir();

    while (1) {
        char filename[64];
        uint32_t filesize;

        if (SerialFlash.readdir(filename, sizeof(filename), filesize)) {
            Serial.print(F("  "));
            Serial.print(filename);
            Serial.print(F("  "));
            Serial.print(filesize);
            Serial.print(F(" bytes"));
            Serial.println();
            count++;
        }
        else {
            if(count == 0)
            {
                Serial.println("listFlash: SPI Flash chip is empty");
            }
            break;
        }
    }

}

void
MemGeneric::purgeFlash()
{
    if (!SerialFlash.begin(FLASH_PIN)) {
        while (1) {
            Serial.println(F("purgeFlash: Unable to access SPI Flash chip"));
            return;
        }
    }
    Serial.println("purgeFlash: deleting all Files. Please wait until 'ready' (ca.40s)");
    SerialFlash.eraseAll();
    while (SerialFlash.ready() == false);
    Serial.println("ready");
}



bool
MemGeneric::compare(File &file, SerialFlashFile &ffile) {
    file.seek(0);
    ffile.seek(0);
    unsigned long count = file.size();
    while (count > 0) {
        char buf1[128], buf2[128];
        unsigned long n = count;
        if (n > 128) n = 128;
        file.read(buf1, n);
        ffile.read(buf2, n);
        if (memcmp(buf1, buf2, n) != 0) return false; // differ
        count = count - n;
    }
    return true;  // all data identical
}