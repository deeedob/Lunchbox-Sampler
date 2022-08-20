#include "main_memory.hpp"

/* Arduino ... why u do this shit?! */
#undef main

int main() {
#ifdef VERBOSE
	Serial.begin( 9600 );
	Serial.println( ":::Lunchbox Sampler:::" );
#endif

	auto mf = lbs::MainMemory();

	//auto list = mf.getAllFromFlash();

	auto list = mf.getSampleNamesFromPack( "Samplepack01" );
	Serial.println( "Files on Samplepack01" );
	for( const auto& i : list ) { Serial.println( i.c_str() ); }

	mf.eraseFlash();

	auto flashlist = mf.getFilelistFromFlash();
	Serial.println( "Files on Flash:" );
	for( const auto& i : flashlist ) { Serial.println( i.c_str() ); }

	mf.loadSamplepack( "Samplepack01" );

	flashlist = mf.getFilelistFromFlash();
	Serial.println( "Files on Flash:" );
	for( const auto& i : flashlist ) { Serial.println( i.c_str() ); }
}