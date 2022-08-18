#include "mem_sd.hpp"

using namespace lbs;

MemSD::MemSD()
{
	SPI.setMOSI( C_SDCARD_MOSI_PIN );
	SPI.setSCK( C_SDCARD_SCK_PIN );
	
	if( !SD.begin( C_SDCARD_CS_PIN )) {
		Serial.println( "SD Card: initialization failed!" );
	}
	printAllFiles( SD.open( "/" ), 0 );
}

void MemSD::printAllFiles( File dir, int numSpaces )
{
	auto printSpaces = []( int num ) {
		for( int i = 0; i < num; i++ ) {
			Serial.print( " " );
		}
	};
	auto printTime = []( const DateTimeFields tm ) {
		const char* months[12] = {
			"January", "February", "March", "April", "May", "June",
			"July", "August", "September", "October", "November", "December"
		};
		if( tm.hour < 10 ) Serial.print( '0' );
		Serial.print( tm.hour );
		Serial.print( ':' );
		if( tm.min < 10 ) Serial.print( '0' );
		Serial.print( tm.min );
		Serial.print( "  " );
		Serial.print( tm.mon < 12 ? months[ tm.mon ] : "???" );
		Serial.print( " " );
		Serial.print( tm.mday );
		Serial.print( ", " );
	};
	while ( true ) {
		File entry = dir.openNextFile();
		if( !entry ) {
			//Serial.println("** no more files **");
			break;
		}
		printSpaces( numSpaces );
		Serial.print( entry.name());
		if( entry.isDirectory()) {
			Serial.println( "/" );
			printAllFiles( entry, numSpaces + 2 );
		} else {
			// files have sizes, directories do not
			unsigned int n = log10( entry.size());
			if( n > 10 ) n = 10;
			printSpaces( 50 - numSpaces - strlen( entry.name()) - n );
			Serial.print( "  " );
			Serial.print( entry.size(), DEC );
			DateTimeFields datetime;
			if( entry.getModifyTime( datetime )) {
				printSpaces( 4 );
				printTime( datetime );
			}
			Serial.println();
		}
		entry.close();
	}
}