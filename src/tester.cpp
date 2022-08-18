#include <mem_sd.hpp>
#include <mem_flash.hpp>
#include <Arduino.h>

using namespace lbs;

int main()
{
	Serial.begin( 9600 );
	Serial.println( "Memory 2" );
	while ( !Serial ) {
		Serial.println( "Starting Memory Class" );
	}
	
	MemSD mem_sd;
}