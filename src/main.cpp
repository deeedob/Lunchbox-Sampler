#include "main_memory.hpp"
#include <Audio.h>
#include <AudioControl.h>
#include <SD.h>
#include <SPI.h>
#include <SerialFlash.h>
#include <Wire.h>

/* Arduino ... why u do this shit?! */
#undef main

AudioPlaySerialflashRaw g_playFlashRaw1;//xy=103.19999694824219,318.1999969482422
AudioOutputI2S g_i2S1;                  //xy=301.1999969482422,282.1999969482422
AudioConnection g_patchCord1( g_playFlashRaw1, 0, g_i2S1, 0 );
AudioConnection g_patchCord2( g_playFlashRaw1, 0, g_i2S1, 1 );
AudioControlSGTL5000 g_audioShield;


int main() {
#ifdef VERBOSE
    Serial.begin(9600);
    Serial.println(":::Lunchbox Sampler:::");
#endif
    //Reserve memory for audio connections (taken from SamplePlayer example in audiolibrary)
    AudioMemory(10);

    //mf.eraseFlash();
    //auto samplelist = mf.getSampleNamesFromPack( "SamplePack03" );
    //Serial.println( "Files from Samplepack03:" );
    //for( auto& i : samplelist ) { Serial.println( i.c_str() ); }

    //auto flashlist = mf.getFilelistFromFlash();
    //Serial.println( "Files on Flash:" );
    //for( auto& i : flashlist ) { Serial.println( i.c_str() ); }

    //Serial.println( "Load Samplepack to Flash" );
    //mf.loadSamplepack("SamplePack03");

    //flashlist = mf.getFilelistFromFlash();
    //Serial.println( "Files on Flash:" );
    //for( auto& i : flashlist ) { Serial.println( i.c_str() ); }

    //mf.loadSamplepack("SamplePack01");
    //auto packs = mf.getAvailableSamplepacks();
    lbs::MainMemory::init();
    lbs::MainMemory::deleteMappingFile("SamplePack01");
    lbs::MainMemory::createAllStdMappingFiles();
    lbs::MainMemory::loadSamplepack("SamplePack01");
    Serial.println(lbs::MainMemory::getLoadedPackName());
    lbs::MainMemory::setSampleForNote("0.wav", 12);
    lbs::MainMemory::setSampleForNote("0.wav", 13);
    lbs::MainMemory::setSampleForNote("0.wav", 14);
    lbs::MainMemory::setSampleForNote("0.wav", 15);
    lbs::MainMemory::setSampleForNote("0.wav", 16);
    lbs::MainMemory::setSampleForNote("0.wav", 60);
    lbs::MainMemory::setSampleForNote("0.wav", 61);
    lbs::MainMemory::printMapping();
    lbs::MainMemory::saveCurrentMappingToFile();

/*
    for (auto &pack: packs) {
        Serial.println(pack);
    }
*/
    Serial.println("Play Samples");
    g_audioShield.enable();
    g_audioShield.volume(0.5);
    g_playFlashRaw1.play(lbs::MainMemory::getSampleFromNote(12).c_str());
    while (g_playFlashRaw1.isPlaying()) {}
    g_playFlashRaw1.play(lbs::MainMemory::getSampleFromNote(13).c_str());
    while (g_playFlashRaw1.isPlaying()) {}
    g_playFlashRaw1.play(lbs::MainMemory::getSampleFromNote(60).c_str());
    while (g_playFlashRaw1.isPlaying()) {}
    g_playFlashRaw1.play(lbs::MainMemory::getSampleFromNote(61).c_str());
    while (g_playFlashRaw1.isPlaying()) {}
    g_playFlashRaw1.play(lbs::MainMemory::getSampleFromNote(62).c_str());
    while (g_playFlashRaw1.isPlaying()) {}
    //Serial.println( "Playing sample finished" );
//auto res = mf.loadMappingFile("SamplePack02");
    //mf.eraseFlash();
    //mf.printMapping();


/*
    for (auto& i : res) {
        Serial.print("Col 1: ");
        Serial.println(std::get<0>(i));
        Serial.print("Col 2: ");
        Serial.println(std::get<1>(i));
        Serial.print("Col 3: ");
        Serial.println(std::get<2>(i));
    }
*/

}