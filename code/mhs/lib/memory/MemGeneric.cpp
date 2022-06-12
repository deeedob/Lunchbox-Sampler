#include <Arduino.h>
#include <SD.h>
#include <SerialFlash.h>
#include <Bounce.h>
#include <iostream>

#include "MemGeneric.hpp"

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

    Serial.println(F("All Files on SPI Flash chip:"));

    /* Opening SerialPin? */
    if (!SerialFlash.begin(FLASH_PIN)) {
        Serial.println("Flash Chip is empty!");
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
                Serial.println("SPI Flash chip is empty");
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
MemGeneric::readSettings()
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
MemGeneric::updateSettings()
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
MemGeneric::configSettings(){

    if (!SD.begin(SDCARD_CS_PIN )) {
        Serial.println("initialization failed!");
        return;
    }

    this->settings = new String[128];
    File settingsFile;
    settingsFile = SD.open("settings.txt", FILE_READ);
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
        Serial.println("error opening test.txt");
    }
}

int
MemGeneric::searchFreeMidi(){
    for(int i=0;i<128;i++){
        if (settings[i].equals("not defined")){
            return i;
        }
    }
    return 128;
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

void
MemGeneric::setSettingsFile(String *settings)
{
    return;
}