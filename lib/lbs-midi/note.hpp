#pragma once
#include <cstdint>
#include <Arduino.h>

struct Note
{
	Note( uint8_t type_, uint8_t channel_, uint8_t note_, float velocity_, int time_ = 0 )
		: type( type ), channel( channel_ ), note( note_ ), velocity( velocity_ ), time( time_ )
	{ }
	
	void printMidiData() const
	{
		Serial.println( "______" );
		Serial.print( "type: " );
		Serial.println( type );
		Serial.print( "channel: " );
		Serial.println( channel );
		Serial.print( "note: " );
		Serial.println( note );
		Serial.print( "velocity: " );
		Serial.println( velocity );
		Serial.print( "timing: " );
		Serial.println( time );
	}
	
	uint8_t type;
	uint8_t channel;
	uint8_t note;
	float velocity;
	int time;
};