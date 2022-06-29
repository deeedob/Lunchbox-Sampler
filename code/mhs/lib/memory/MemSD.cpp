#include <Arduino.h>
#include <SD.h>
#include <SerialFlash.h>
#include <Bounce.h>
#include <iostream>

#include "MemSD.hpp"

MemSD
*MemSD::getInstance()
{
    static MemSD *instance;

    if (!SD.begin(SDCARD_CS_PIN)) {
#ifdef VERBOSE
        Serial.println("loadSamplePack: Unable to access SPI Flash chip");
#endif
        //TODO: Error handling
    }

    if (!instance)
        instance = new MemSD();

    return instance;
}

String
MemSD::listFlash()
{
    unsigned int count = 0;
    char filename[64];
    uint32_t filesize;
    String filelist;

    /* announce reading filelist (prob setting file iterator to 0) */
    SerialFlash.opendir();

    while (SerialFlash.readdir(filename, sizeof(filename), filesize)) {
            filelist.append("File " + count);
            filelist.append(": ");
            filelist.append(filename);
            filelist.append("  ");
            filelist.append(filesize);
            filelist.append(" bytes\n");
#ifdef VERBOSE
            Serial.print("File ");
            Serial.print(count);
            Serial.print(F(": "));
            Serial.print(filename);
            Serial.print(F("  "));
            Serial.print(filesize);
            Serial.print(F(" bytes"));
            Serial.println();
#endif
            count++;
    }

    if (count == 0)
        Serial.println("Flash is empty");

    return filelist;
}

void
MemSD::purgeFlash()
{
#ifdef VERBOSE
    Serial.println("purgeFlash: deleting all Files. Please wait until 'ready' (ca.40s)");
#endif
    SerialFlash.eraseAll();
    while (!SerialFlash.ready());

#ifdef VERBOSE
    Serial.println("ready");
#endif
}

bool
MemSD::compare(File &file, SerialFlashFile &ffile) {
    file.seek(0);
    ffile.seek(0);

    unsigned long count = file.size();

    while (count > 0) {
        char buf1[128], buf2[128];
        unsigned long n = count;
        if (n > 128) n = 128;
        file.read(buf1, n);
        ffile.read(buf2, n);
        if (memcmp(buf1, buf2, n) != 0)
            return false; // differ
        count = count - n;
    }

    return true;  // all data identical
}