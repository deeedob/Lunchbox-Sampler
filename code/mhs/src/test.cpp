#include <Arduino.h>
#include <SD.h>
#include <SPI.h>
#include <SerialFlash.h>

#include "MemGeneric.hpp"
#include "MemSample.hpp"

using namespace std;

void setup() {
    SPI.setSCK(SDCARD_SCK_PIN);
    SPI.setMOSI(SDCARD_MOSI_PIN);

    Serial.begin(9600);
    Serial.println("Starting Memory Class");

    // Zuweisung Midi sind in der txt Datei "settings.txt" und im String [] "settings" gespeichert. Die Beiden tauschen sich untereinander aus.
    // Settings zieht sich in der Setup Methode über configSettings() die Midi Zuweisungen aus der txt Datei, welche wiederum in der Laufzeit von settings aktualisiert wird über
    // updateSettingsTxt(). Werden neue Samples/Samplepacks hinzugefuegt werden sie zunächst ins Array eingelesen und dann in der Textdatei aktualisiert.
    // Gleiches gilt für das Löschen von Samples/SamplePacks. Über readSettingsTxt() kann der Inhalt der TextDatei gelesen werden.
    MemSample *msamp = MemSample::getInstance();
    MemGeneric *mgen = MemGeneric::getInstance();

    msamp->loadSamplePack("SamplePack01");
    msamp->deleteSamplePack("SamplePack01");
    msamp->loadSample("SamplePack01/Rim.wav");
    msamp->deleteSample("Rim.wav");
    mgen->purgeFlash();
    mgen->listFlash();
    msamp->readSettings();
}

void loop() {

}
