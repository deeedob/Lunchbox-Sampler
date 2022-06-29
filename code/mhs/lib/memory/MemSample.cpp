#include <Arduino.h>
#include <SD.h>
#include <SerialFlash.h>
#include <iostream>
#include "frozen/unordered_map.h"

#include "MemSD.hpp"
#include "MemSample.hpp"

constexpr frozen::unordered_map<String, uint8_t, 12> MemSample::MidiMapping::pitches =

MemSample
*MemSample::getInstance()
{
    static MemSample *instance;

    if (!SerialFlash.begin(FLASH_PIN)) {
#ifdef VERBOSE
        Serial.println("loadSamplePack: Unable to access SPI Flash chip");
#endif
        //TODO: Error handling
    }

    if (!instance)
        instance = new MemSample();

    return instance;
}

String
MemSample::getCurrentSamplePack()
{
    return this->currentSamplePack;
}

bool
MemSample::testSize(const char* path)
{
    File dir = SD.open(path, FILE_READ);
    uint64_t packsize = 0;

    if(!dir)
    {
#ifdef VERBOSE
        Serial.println("loadSamplePack: Directory nicht gefunden");
#endif
        //TODO: Error handling
    }

    File f = nullptr;

    while ((f = dir.openNextFile())) {
        packsize += f.size();
    }

    if (packsize > FLASHSIZE)
        return false;

    return true;
}

// load Sample Pack from SD to Flash
bool
MemSample::loadSamplePack(const char* path)
{
    //if samplepack same as loadedsamplepack -> abort
    // Access to SD through MemSD
    File dir = SD.open(path, FILE_READ);

    //loadMapping
    // TODO: loadMapping from filepath into Midimapping object

    //TODO: adjustSize() (strips samplesizes down if size of sample pack too large

    //test if success and size is ok and Mapping loaded
    if (!dir || !this->currentMapping)
        return false;

    // Load every file to flash
    File f = nullptr;
    while ((f = dir.openNextFile())) {

        const char *filename = f.name();
        unsigned long length = f.size();
#ifdef VERBOSE
        Serial.println("name: ");
        Serial.println(filename);
#endif
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
            ff.close();
        }
        f.close();
    }
    dir.close();

    this->currentSamplePack = path;

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
    for(int i=0; i < 128; i++){
        if (settings[i].equals("not defined")){
            return i;
        }
    }
    return 128;
}

MemSample::MidiMapping
*MemSample::MidiMapping::getInstance()
{

}

uint8_t
MemSample::getIntFromNote(String note)
{
    note = note.toLowerCase();
    uint8_t octave = 5;
    String pitchstr;
    String octavestr;
    uint8_t pitch = 0;

    for (int i = 0; i < note.length(); i++) {
       if ((note[i] >= 'a' && note[i] <= 'z')) {
           pitchstr.append(note[i]);
       } else {
           octavestr.append(note[i]);
       }
    }

    octave = octavestr.toInt();
    pitch = MemSample::MidiMapping::pitches.find(pitchstr)->second;

    return octave * 12 + pitch;
}