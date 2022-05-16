#include <Arduino.h>
#include <Audio.h>
#include <Wire.h>
#include <SD.h>
#include <SPI.h>
#include <SerialFlash.h>
#include <Adafruit_SSD1327.h>
#include <Encoder.h>
#include <Bounce.h>

#define POT0 A0
#define POT1 A1
#define POT2 A10
#define POT3 A12

#define DISPLAY_CLK 16
#define DISPLAY_SDA 17
#define DISPLAY_RST 9

#define ROTARY_A 2
#define ROTARY_B 3
#define ROTARY_SW 4

#define MIDI_IN 0
#define MIDI_OUT 1

#define SDCARD_CS_PIN   10
#define SDCARD_MOSI_PIN 11
#define SDCARD_SCK_PIN  13
#define FLASH_PIN 6

AudioInputI2S audioInput;
AudioPlaySdWav playSdWav;
AudioOutputI2S audioOutput;
AudioControlSGTL5000 audioShield;
AudioConnection          patchCord1(playSdWav, 0, audioOutput, 0);
AudioConnection          patchCord2(playSdWav, 1, audioOutput, 1);
AudioPlaySerialflashRaw  playFlashRaw1;
AudioPlaySerialflashRaw  playFlashRaw2;


bool loadSamplePack(const char*);
bool loadSample(const char*);
bool deleteSampleFromFlash(const char*);
bool deleteSamplePackFromFlash(const char* path);
void deleteAllFilesOnFlash();
bool replaceSample(const char* oldPath, const char* newPath);
SerialFlashFile triggerSample(const char* path);

void directoryListing();
bool compareFiles(File &file, SerialFlashFile &ffile);
SerialFlashFile file;

void setup() {
    SPI.setSCK(SDCARD_SCK_PIN);
    SPI.setMOSI(SDCARD_MOSI_PIN);

    Serial.begin(9600);
    //loadSamplePack("SamplePack01");
    //deleteSamplePackFromFlash("SamplePack01");
    //loadSample("SamplePack02/01.WAV");
   //deleteSampleFromFlash("Rim.wav");
    //deleteAllFilesOnFlash();
    //triggerSample("01.WAV");
    directoryListing();
}
void loop() {

}
void deleteAllFilesOnFlash()
{
    if (!SerialFlash.begin(FLASH_PIN)) {
        while (1) {
            Serial.println(F("Unable to access SPI Flash chip"));
            return;
        }
    }
    Serial.println("deleting all Files");
    SerialFlash.eraseAll();
    Serial.println("ready");
}
// load Sample Pack from SD to Flash
bool loadSamplePack(const char* path)
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
                if (compareFiles(f, ff) == true) {
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
    return true;
}
bool loadSample(const char* path)
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
    const char *filename = f.name();
    unsigned long length = f.size();
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
        }
        return true;
    }
    return false;
}
bool deleteSampleFromFlash(const char* path)
{
    if (!SD.begin(SDCARD_CS_PIN)) {
        Serial.println("Unable to access SPI Flash chip");
        return false;
    }
    if (!SerialFlash.begin(FLASH_PIN)) {
        Serial.println("Unable to access SPI Flash chip");
        return false;
    }
    if(SerialFlash.remove(path))
    {
        Serial.println(path);
        Serial.println(" is deleted");
        return true;
    }
    Serial.println("unable to delete ");
    Serial.println(path);
    return false;
}
bool deleteSamplePackFromFlash(const char* path)
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
bool replaceSample(const char* oldPath, const char* newPath)
{

    return true;
}
void directoryListing()
{
    while (!Serial) ;
    delay(100);
    Serial.println(F("All Files on SPI Flash chip:"));

    if (!SerialFlash.begin(FLASH_PIN)) {
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
SerialFlashFile triggerSample(const char* path)
{
    if(!SerialFlash.begin(FLASH_PIN))
    {
        Serial.println("Couldnt open SerialFlash");
    }
    SerialFlashFile ff = SerialFlash.open(path);

    while(1)
    {
        playFlashRaw1.play(path);
    }
    return ff;

}
bool compareFiles(File &file, SerialFlashFile &ffile) {
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