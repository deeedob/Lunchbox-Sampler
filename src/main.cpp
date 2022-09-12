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

    auto mf = lbs::MainMemory();
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
    auto packs = mf.getAllSamplepacks();

    for (auto &pack: packs) {
        Serial.println(pack);
    }
/*
    Serial.println("Play Samples");
	g_audioShield.enable();
	g_audioShield.volume( 0.5 );
	g_playFlashRaw1.play( mf.getSampleFromNote(24).c_str() );
	while( g_playFlashRaw1.isPlaying() ) {}
    g_playFlashRaw1.play( mf.getSampleFromNote(25).c_str() );
    while( g_playFlashRaw1.isPlaying() ) {}
    g_playFlashRaw1.play( mf.getSampleFromNote(26).c_str() );
    while( g_playFlashRaw1.isPlaying() ) {}
    g_playFlashRaw1.play( mf.getSampleFromNote(27).c_str() );
    while( g_playFlashRaw1.isPlaying() ) {}
    g_playFlashRaw1.play( mf.getSampleFromNote(28).c_str() );
    while( g_playFlashRaw1.isPlaying() ) {}
    g_playFlashRaw1.play( mf.getSampleFromNote(29).c_str() );
	//Serial.println( "Playing sample finished" );
    Serial.println( "Printing Settings");
*/
//auto res = mf.loadSettings("SamplePack02");
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