#include <Arduino.h>
#include <SD.h>
#include <SerialFlash.h>
#include <Bounce.h>
#include <iostream>

#include "MemGeneric.hpp"

MemGeneric *MemGeneric::instance;

MemGeneric
*MemGeneric::getInstance()
{
    if (!MemGeneric::instance)
        MemGeneric::instance = new MemGeneric();

    return MemGeneric::instance;
}

void
MemGeneric::listFlash()
{
    /* could cause infinity loop, reason ? */
    while (!Serial);
    delay(100);

    Serial.println(F("listFlash: All Files on SPI Flash chip:"));

    /* Opening SerialPin? */
    if (!SerialFlash.begin(FLASH_PIN)) {
        Serial.println("listFlash: Flash Chip is empty!");
        return;
    }

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

MemGeneric::MemGeneric()
{

}