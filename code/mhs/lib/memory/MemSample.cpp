#include <Arduino.h>
#include <SD.h>
#include <SerialFlash.h>
#include <iostream>

#include "MemGeneric.hpp"

#include "MemSample.hpp"

MemSample *MemSample::instance;

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
        Serial.println("loadSamplePack: Unable to access SPI Flash chip");
        return false;
    }
    if (!SerialFlash.begin(FLASH_PIN)) {
        Serial.println("loadSamplePack: Unable to access SPI Flash chip");
        return false;
    }
    File dir=SD.open(path, FILE_READ);
    if(!dir)
    {
        Serial.println("loadSamplePack: Directory nicht gefunden");
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
            Serial.println(F("loadSamplePack: already exists on the Flash chip"));
            SerialFlashFile ff = SerialFlash.open(filename);
            if (ff && ff.size() == f.size()) {
                Serial.println(F("loadSamplePack: size is the same, comparing data..."));
                if (mgen()->compare(f, ff) == true) {
                    Serial.println(F("loadSamplePack:  files are identical :)"));
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
                int i=searchFreeMidi();
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
        Serial.println("delSamplePack: Unable to access SPI Flash chip");
        return false;
    }
    if (!SerialFlash.begin(FLASH_PIN)) {
        Serial.println("delSamplePack: Unable to access SPI Flash chip");
        return false;
    }
    File dir=SD.open(path);
    while (1) {
        File f = dir.openNextFile();
        if (!f) break;
        const char *filename = f.name();
        if (SerialFlash.exists(filename)) {
            Serial.println("delSamplePack: file exists");
            if(SerialFlash.remove(filename))
            {
                Serial.println(filename);
                Serial.println( "delSamplePack: is deleted");
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
                Serial.println( "delSamplePack: unable to delete ");
                Serial.println(filename);
            }
        }
        else
        {
            Serial.println(filename);
            Serial.println( " does not exist on Flash :delSamplePack");
        }
        f.close();
    }
    return true;
}

bool
MemSample::loadSample(const char* path)
{
    if (!SD.begin(SDCARD_CS_PIN)) {
        Serial.println("loadSample: Unable to access SPI Flash chip");
        return false;
    }
    if (!SerialFlash.begin(FLASH_PIN)) {
        Serial.println("loadSample: Unable to access SPI Flash chip");
        return false;
    }
    File f = SD.open(path);
    if(!f){
        Serial.println("loadSample: Datei nicht gefunden ") ;
        return false;
    }
    const char *filename = f.name();
    unsigned long length = f.size();
    if (SerialFlash.exists(filename)) {
        Serial.println(F("loadSample: already exists on the Flash chip"));
        SerialFlashFile ff = SerialFlash.open(filename);
        if (ff && ff.size() == f.size()) {
            Serial.println(F("loadSample:   size is the same, comparing data..."));
            if (mgen()->compare(f, ff) == true) {
                Serial.println(F("loadSample:   files are identical :)"));
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
        Serial.println("getSample: Couldnt open SerialFlash");
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
        Serial.println("deleteSample: Unable to access SPI Flash chip");
        return false;
    }
    if (!SerialFlash.begin(FLASH_PIN)) {
        Serial.println("deleteSample: Unable to access SPI Flash chip");
        return false;
    }
    if(SerialFlash.remove(name))
    {
        Serial.println(name);
        Serial.println(" is deleted :deleteSample");
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
    Serial.println("deleteSample: unable to delete " + String(name));
    return false;
}

void
MemSample::readSettings()
{
    const char *filename = "settings.txt";
    if (!SD.begin(SDCARD_CS_PIN )) {
        Serial.println("readSettings: initialization failed!");
        return;
    }
    File myFile;
    myFile = SD.open(filename, FILE_READ);
    if(myFile) {
        Serial.println("readSettings: " + String(filename));
        while (myFile.available()) {
            Serial.write(myFile.read());
        }
        // close the file:
        myFile.close();
    }
    else {
        // if the file didn't open, print an error:
        Serial.println("readSettings: error opening " + String(filename));
    }
}

void
MemSample::updateSettings()
{
    char *filename = "settings.txt";
    if (!SD.begin(SDCARD_CS_PIN )) {
        Serial.println("updateSettings: initialization failed!");
        return;
    }
    File myFile;
    SD.remove(filename);
    myFile = SD.open(filename, FILE_WRITE);
    if (myFile) {
        for(int i=0; i<128; i++)
        {
            myFile.print(i);
            myFile.print(",");
            myFile.print(settings[i]);
            myFile.print(";");
        }
        myFile.close();
        Serial.println("done.");
        configSettings();
    }
    else {
        // if the file didn't open, print an error:
        Serial.println("updateSettings: error opening " + String(filename));
    }
}

void
MemSample::configSettings(){

    char *filename = "settings.txt";

    if (!SD.begin(SDCARD_CS_PIN )) {
        Serial.println("configSettings: initialization failed!");
        return;
    }

    this->settings = new String[128];
    File settingsFile;
    settingsFile = SD.open(filename, FILE_READ);
    if(settingsFile) {
        unsigned int count = 0;
        char buf1[settingsFile.size()];
        settingsFile.read(buf1, settingsFile.size());
        while (1) {
            int i = 0;
            String name;
            String midiValue;
            while (buf1[count + i] != ',') {
                midiValue.append(buf1[i + count]);
                i++;
            }
            i++;
            while (buf1[count + i] != ';') {
                name += buf1[i + count];
                i++;
            }
            i++;
            settings[midiValue.toInt()] = name;
            count += i;
            if(count==settingsFile.size())break;
        }
        settingsFile.close();
        return;
    }
    else {
        // if the file didn't open, print an error:
        Serial.println("error opening " + String(filename));
    }
}

void
MemSample::setSettingsFile(String *settings)
{
    return;
}

int
MemSample::searchFreeMidi(){
    for(int i=0;i<128;i++){
        if (settings[i].equals("not defined")){
            return i;
        }
    }
    return 128;
}

MemGeneric
*MemSample::mgen()
{
    return MemGeneric::getInstance();
}