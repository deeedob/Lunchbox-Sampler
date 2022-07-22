#include <Arduino.h>
#include <SD.h>
#include <SPI.h>
#include <SerialFlash.h>
#include <Bounce.h>
#include <iostream>

#include "memory.hpp"

void Memory::listFlash()
{
    /* could cause infinity loop ? */
    while (!Serial);
    delay(100);

    Serial.println(F("All Files on SPI Flash chip:"));

    /* Opening SerialPin? */
    if (!SerialFlash.begin(FLASH_PIN)) {
        Serial.println("Flash Chip is empty!");
        return;
    }

    int count=0;
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
            if(count==0)
            {
                Serial.println("SPI Flash chip is empty");
            }
            break;
        }
    }

}

// load Sample Pack from SD to Flash
bool Memory::loadSamplePack(const char* path)
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
                if (compare(f, ff) == true) {
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
Memory::deleteSamplePack(const char *path)
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
        unsigned long length = f.size();
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
Memory::loadSample(const char* path)
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
Memory::getSample(const char* path)
{
    if(!SerialFlash.begin(FLASH_PIN))
    {
        Serial.println("Couldnt open SerialFlash");
    }
    SerialFlashFile ff = SerialFlash.open(path);
    return ff;

}

bool
Memory::replaceSample(const char* oldPath, const char* newPath)
{

    return true;
}

bool
Memory::deleteSample(const char* name)
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


void
Memory::purgeFlash()
{
    if (!SerialFlash.begin(FLASH_PIN)) {
        while (1) {
            Serial.println(F("Unable to access SPI Flash chip"));
            return;
        }
    }
    Serial.println("deleting all Files. Please wait until 'ready' (ca.40s)");
    SerialFlash.eraseAll();
    while (SerialFlash.ready() == false)
    {

    }
    Serial.println("ready");
}

void
Memory::readSettings()
{
    if (!SD.begin(SDCARD_CS_PIN )) {
        Serial.println("initialization failed!");
        return;
    }
    File myFile;
    myFile = SD.open("settings.txt", FILE_READ);
    if(myFile) {
        Serial.println("settings.txt");
        while (myFile.available()) {
            Serial.write(myFile.read());
        }
        // close the file:
        myFile.close();
    }
    else {
        // if the file didn't open, print an error:
        Serial.println("error opening test.txt");
    }
}

void
Memory::updateSettings()
{
    if (!SD.begin(SDCARD_CS_PIN )) {
        Serial.println("initialization failed!");
        return;
    }
    File myFile;
    SD.remove("settings.txt");
    myFile = SD.open("settings.txt", FILE_WRITE);
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
        Serial.println("error opening test.txt");
    }
}

void
Memory::configSettings(){

    if (!SD.begin(SDCARD_CS_PIN )) {
        Serial.println("initialization failed!");
        return;
    }

    Memory::settings = new String[128];
    File settingsFile;
    settingsFile = SD.open("settings.txt", FILE_READ);
    if(settingsFile) {
        int count = 0;
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
        Serial.println("error opening test.txt");
    }
}

int
Memory::searchFreeMidi(){
    for(int i=0;i<128;i++){
        if (settings[i].equals("not defined")){
            return i;
        }
    }
    return 128;
}


bool
Memory::compare(File &file, SerialFlashFile &ffile) {
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

void
Memory::setSettingsFile(String *settings)
{
    return;
}