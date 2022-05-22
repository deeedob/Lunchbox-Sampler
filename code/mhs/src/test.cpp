#include <Arduino.h>
#include <Audio.h>
#include <Wire.h>
#include <SD.h>
#include <SPI.h>
#include <SerialFlash.h>
#include <Adafruit_SSD1327.h>
#include <Encoder.h>
#include <Bounce.h>
#include <iostream>
#include <fstream>


using namespace std;

#define SDCARD_CS_PIN   10
#define SDCARD_MOSI_PIN 11
#define SDCARD_SCK_PIN  13
#define FLASH_PIN 6


String *settings;

int searchFreeMidi();
bool loadSamplePack(const char*);
bool loadSample(const char*);
bool deleteSampleFromFlash(const char*);
bool deleteSamplePackFromFlash(const char* path);
void deleteAllFilesOnFlash();
void readSettingsTxt();
void updateSettingsTxt();
bool replaceSample(const char* oldPath, const char* newPath);
SerialFlashFile triggerSample(const char* path);
void configSettings();
void directoryListingFlash();
bool compareFiles(File &file, SerialFlashFile &ffile);
SerialFlashFile file;

void setup() {
    SPI.setSCK(SDCARD_SCK_PIN);
    SPI.setMOSI(SDCARD_MOSI_PIN);

    Serial.begin(9600);
    Serial.println("Starting Memory Class");

    // Zuweisung Midi sind in der txt Datei "settings.txt" und im String [] "settings" gespeichert. Die Beiden tauschen sich untereinander aus.
    // Settings zieht sich in der Setup Methode über configSettings() die Midi Zuweisungen aus der txt Datei, welche wiederum in der Laufzeit von settings aktualisiert wird über
    // updateSettingsTxt(). Werden neue Samples/Samplepacks hinzugefuegt werden sie zunächst ins Array eingelesen und dann in der Textdatei aktualisiert.
    // Gleiches gilt für das Löschen von Samples/SamplePacks. Über readSettingsTxt() kann der Inhalt der TextDatei gelesen werden.

    settings = new String[128];
    configSettings();
    //loadSamplePack("SamplePack01");
    //deleteSamplePackFromFlash("SamplePack01");
    //loadSample("SamplePack01/Rim.wav");
    //deleteSampleFromFlash("Rim.wav");
    //deleteAllFilesOnFlash();
    directoryListingFlash();
    readSettingsTxt();
}
void loop() {

}
void readSettingsTxt()
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
void updateSettingsTxt()
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
int searchFreeMidi(){
    for(int i=0;i<128;i++){
        if (settings[i].equals("not defined")){
            return i;
        }
    }
    return 128;
}
void configSettings(){
    if (!SD.begin(SDCARD_CS_PIN )) {
        Serial.println("initialization failed!");
        return;
    }
    File myFile;
    myFile = SD.open("settings.txt", FILE_READ);
    if(myFile) {
         int count = 0;
            char buf1[myFile.size()];
            myFile.read(buf1, myFile.size());
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
                    if(count==myFile.size())break;
            }
            myFile.close();
            return;
    }
    else {
        // if the file didn't open, print an error:
        Serial.println("error opening test.txt");
    }
}
void deleteAllFilesOnFlash()
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
            updateSettingsTxt();
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
            if (compareFiles(f, ff) == true) {
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
            updateSettingsTxt();
        }
        ff.close();
        return true;
    }
    return false;
}
bool deleteSampleFromFlash(const char* name)
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
                updateSettingsTxt();
                break;
            }
        }
        return true;
    }
    Serial.println("unable to delete ");
    Serial.println(name);
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
                for(int i=0; i<128; i++)
                {
                    if(settings[i].equals(filename))
                    {
                        settings[i]="not defined";
                        updateSettingsTxt();
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
bool replaceSample(const char* oldPath, const char* newPath)
{

    return true;
}
void directoryListingFlash()
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