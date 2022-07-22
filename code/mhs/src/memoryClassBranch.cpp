#include <define_t40.hpp>
#include <SD.h>
#include <SPI.h>
#include <SerialFlash.h>
#include <CSV_Parser.h>

#include <MemFlash.hpp>
#include <MemSD.hpp>

using namespace lbs;

void setup()
{

    SPI.setSCK(SDCARD_SCK_PIN);
    SPI.setMOSI(SDCARD_MOSI_PIN);

    Serial.begin(9600);
    Serial.println("Starting Memory Class");

    MemFlash &ms = MemFlash::getInstance();


    Serial.println("");
    Serial.println("Printing");
    ms.loadSamplePack("SamplePack02");

    //MemFlash *msamp = MemFlash::getInstance();

/*
    msamp->loadSamplePack("SamplePack01");
    msamp->deleteSamplePack("SamplePack01");
    msamp->loadSample("SamplePack01/Rim.wav");
    msamp->deleteSample("Rim.wav");
    mgen->purgeFlash();
    mgen->listFlash();
    msamp->readSettings();
*/
}

void loop()
{

}
