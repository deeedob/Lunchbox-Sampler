#include <define_t40.hpp>
#include <SD.h>
#include <SPI.h>
#include <SerialFlash.h>
#include "lunchbox_sampler.hpp"

using namespace lbs;

int main()
{
#ifdef VERBOSE
	Serial.begin( 9600 );
	Serial.println( ":::Lunchbox Sampler:::" );
#endif
	LunchboxSampler& sampler = LunchboxSampler::getInstance();
	sampler.run();
}