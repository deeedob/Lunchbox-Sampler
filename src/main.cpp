#include "main_memory.hpp"

/* Arduino ... why u do this shit?! */
#undef main

int main() {
#ifdef VERBOSE
	Serial.begin( 9600 );
	Serial.println( ":::Lunchbox Sampler:::" );
#endif

	auto mf = lbs::MainMemory();

	auto samplelist = mf.getSampleNamesFromPack( "SamplePack01" );
	Serial.println( "Files from Samplepack01:" );
	for( auto& i : samplelist ) { Serial.println( i.c_str() ); }

	Serial.println( "Files on Flash:" );
	auto flashlist = mf.getFilelistFromFlash();
	for( auto& i : flashlist ) { Serial.println( i.c_str() ); }

	Serial.println( "Load Samplepack to Flash" );
	mf.loadSamplepack( "SamplePack01" );

	Serial.println( "Files on Flash:" );
	flashlist = mf.getFilelistFromFlash();
	for( auto& i : flashlist ) { Serial.println( i.c_str() ); }
}