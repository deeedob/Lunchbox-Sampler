/**************************************************************************************************
 * Copyright (c) 2022. Dennis Oberst                                                              *
 **************************************************************************************************/

#include "interrupt_system/analog_interrupts.hpp"
#include "interrupt_system/digital_interrupts.hpp"
#include "event_sytem.hpp"
#include <Arduino.h>
#include <Audio.h>

using namespace lbs;

#undef main

AudioPlaySdWav playSdWav;
AudioOutputI2S audioOutput;
AudioControlSGTL5000 audioShield;

AudioConnection patchCord1( playSdWav, 0, audioOutput, 0 );
AudioConnection patchCord2( playSdWav, 1, audioOutput, 1 );

void setupPins()
{
	pinMode( C_BTN_ENTER, INPUT_PULLUP );
	pinMode( C_BTN_RETURN, INPUT_PULLUP );
	pinMode( C_BTN_TOGGLE, INPUT_PULLUP );
	pinMode( C_ROTARY_A, INPUT_PULLUP );
	pinMode( C_ROTARY_B, INPUT_PULLUP );
	pinMode(LED_BUILTIN, INPUT );
	pinMode( C_FSR_SEL_0, OUTPUT );
	pinMode( C_FSR_SEL_1, OUTPUT );
	pinMode( C_FSR_SEL_2, OUTPUT );
	pinMode( C_FSR_POLL, INPUT );
	SPI.setSCK( C_SDCARD_SCK_PIN );
	SPI.setMOSI( C_SDCARD_MOSI_PIN );
}

void setupDigitalInterrupts()
{

}

int main()
{
	Serial.begin( 9600 );
	Serial.println( "Begin program" );
	
	Serial.println( ":::Begin Event System Test:::" );
	setupPins();
	if( !( SD.begin( C_SDCARD_CS_PIN ))) {
		// stop here, but print a message
		Serial.println( "Unable to access the SD card.  Program halted." );
		delay( 500 );
	}
	
	AudioMemory( 8 );
	audioShield.enable();
	audioShield.volume( 0.35 );
	playSdWav.play( "01.WAV" );
	delay( 10 );
	
	auto eventSystem = std::make_shared< EventSystem >();
	DigitalInterrupts digitalInterrupts( eventSystem );
	digitalInterrupts.enableAll();
	
	eventSystem->attachDigital( Events::DIGITAL::ROTARY_L, []() {
		Serial.println( "Rotary Left" );
	} );
	eventSystem->attachDigital( Events::DIGITAL::ROTARY_R, []() {
		Serial.println( "Rotary Right" );
	} );
	eventSystem->attachDigital( Events::DIGITAL::BTN_RETURN, []() {
		Serial.println( "Button Return" );
	} );
	eventSystem->attachDigital( Events::DIGITAL::BTN_ENTER, []() {
		Serial.println( "Button Enter" );
	} );
	eventSystem->attachDigital( Events::DIGITAL::BTN_TOGGLE, []() {
		Serial.println( "Button Toggle" );
	} );
	
	AnalogInterrupts an_int( eventSystem );
	
	
	
	
	an_int.enableAll();
	auto& pots = an_int.getPots();
	auto& fsr = an_int.getFSR();
	//an_int.getPots()->enableISR();
	//an_int.getPots()->setDelta( 40 );
	//an_int.getPots()->update();
	fsr->enableISR();
	fsr->setDelta( 18 );
	fsr->update();
	
	while( true ) {
		//pots->update();
		fsr->update();
		delay( 200 );
		//pots->next();
		fsr->next();
	}
	
	return 0;
}