#include <Arduino.h>
#include <SD.h>
#include <SerialFlash.h>
#include <Bounce.h>
#include <iostream>

#include "MemGeneric.hpp"

#include "MemSample.hpp"

MemSample *MemSample::getInstance()
{
    if (!MemSample::instance)
        MemSample::instance = new MemSample();

    return MemSample::instance;
}

MemSample::MemSample()
{

}

String MemSample::getCurrentSamplePack()
{
    return this->currentSamplePack;
}

// load Sample Pack from SD to Flash
bool
MemSample::loadSamplePack(const char* path)
{
    if (!SD.begin(SDCARD_CS_PIN)) {
        Serial.println("Unable to access SPI Flash chip");
        return false;
    }
    if (!SerialFlash.begin(FLASH_PIN)) {
        Serial.println("Unable to access SPI Flash chip");
        return false;
    }
    File dir=SD.open(path, FILE_READ);
    if(!dir)
    {
        Serial.println("Directory nicht gefunden");
        return false;
    }
    while (1) {
        File f = dir.openNextFile();
        if (!f) break;
        const char *filename = f.name();
        unsigned long length = f.size();
        Serial.println("name: ");
        Serial.println(filename);
        if (SerialFlash.exists(filename)) {
            Serial.println(F("already exists on the Flash chip"));
            SerialFlashFile ff = SerialFlash.open(filename);
            if (ff && ff.size() == f.size()) {
                Serial.println(F("  size is the same, comparing data..."));
                if (MemGeneric::getInstance()->compare(f, ff) == true) {
                    Serial.println(F("  files are identical :)"));
                    f.close();
                    ff.close();
                    continue;
                }
            }
            // delete the copy on the Flash chip, if different
            SerialFlash.remove(filename);
        }
        // create the file on the Flash chip and copy data
        if (SerialFlash.create(filename, length)) {
            SerialFlashFile ff = SerialFlash.open(filename);
            if (ff) {
                unsigned long count = 0;
                while (count < length) {
                    char buf[256];
                    unsigned int n;
                    n = f.read(buf, 256);
                    ff.write(buf, n);
                    count = count + n;
                }
                int i=MemGeneric::getInstance()->searchFreeMidi();
                settings[i]=f.name();
                Serial.println(settings[i]);
            }
            else
            {
                Serial.println("could not create ");
                Serial.println(filename);
                Serial.println(length);
            }
            updateSettings();
            ff.close();
        }
        f.close();
    }
    dir.close();
    return true;
}

bool
MemSample::deleteSamplePack(const char *path)
{
    if (!SD.begin(SDCARD_CS_PIN)) {
        Serial.println("Unable to access SPI Flash chip");
        return false;
    }
    if (!SerialFlash.begin(FLASH_PIN)) {
        Serial.println("Unable to access SPI Flash chip");
        return false;
    }
    File dir=SD.open(path);
    while (1) {
        File f = dir.openNextFile();
        if (!f) break;
        const char *filename = f.name();
        if (SerialFlash.exists(filename)) {
            Serial.println("file exists");
            if(SerialFlash.remove(filename))
            {
                Serial.println(filename);
                Serial.println( "is deleted");
                for(int i=0; i<128; i++)
                {
                    if(settings[i].equals(filename))
                    {
                        settings[i]="not defined";
                        updateSettings();
                        break;
                    }
                }
            }
            else
            {
                Serial.println( "unable to delete ");
                Serial.println(filename);
            }
        }
        else
        {
            Serial.println(filename);
            Serial.println( " does not exist on Flash");
        }
        f.close();
    }
    return true;
}

bool
MemSample::loadSample(const char* path)
{
    if (!SD.begin(SDCARD_CS_PIN)) {
        Serial.println("Unable to access SPI Flash chip");
        return false;
    }
    if (!SerialFlash.begin(FLASH_PIN)) {
        Serial.println("Unable to access SPI Flash chip");
        return false;
    }
    File f = SD.open(path);
    if(!f){
        Serial.println("Datei nicht gefunden ") ;
        return false;
    }
    const char *filename = f.name();
    unsigned long length = f.size();
    if (SerialFlash.exists(filename)) {
        Serial.println(F("already exists on the Flash chip"));
        SerialFlashFile ff = SerialFlash.open(filename);
        if (ff && ff.size() == f.size()) {
            Serial.println(F("  size is the same, comparing data..."));
            if (compare(f, ff) == true) {
                Serial.println(F("  files are identical :)"));
                f.close();
                ff.close();
                return false;
            }
        }
        // delete the copy on the Flash chip, if different
        SerialFlash.remove(filename);
    }
    if (SerialFlash.create(filename, length)) {
        SerialFlashFile ff = SerialFlash.open(filename);
        if (ff) {
            unsigned long count = 0;
            while (count < length) {
                char buf[256];
                unsigned int n;
                n = f.read(buf, 256);
                ff.write(buf, n);
                count = count + n;
            }
            int i=searchFreeMidi();
            settings[i]=f.name();
            Serial.println(settings[i]);
            updateSettings();
        }
        ff.close();
        return true;
    }
    return false;
}

SerialFlashFile
MemSample::getSample(const char* path)
{
    if(!SerialFlash.begin(FLASH_PIN))
    {
        Serial.println("Couldnt open SerialFlash");
    }
    SerialFlashFile ff = SerialFlash.open(path);
    return ff;

}

bool
MemSample::replaceSample(const char* oldPath, const char* newPath)
{

    return true;
}

bool
MemSample::deleteSample(const char* name)
{
    if (!SD.begin(SDCARD_CS_PIN)) {
        Serial.println("Unable to access SPI Flash chip");
        return false;
    }
    if (!SerialFlash.begin(FLASH_PIN)) {
        Serial.println("Unable to access SPI Flash chip");
        return false;
    }
    if(SerialFlash.remove(name))
    {
        Serial.println(name);
        Serial.println(" is deleted");
        for(int i=0; i<128; i++)
        {
            if(settings[i].equals(name))
            {
                settings[i]="not defined";
                updateSettings();
                break;
            }
        }
        return true;
    }
    Serial.println("unable to delete ");
    Serial.println(name);
    return false;
}