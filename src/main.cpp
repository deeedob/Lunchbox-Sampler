#include <lunchbox_sampler.hpp>

#undef main

int main()
{
#ifdef VERBOSE
	Serial.begin( 9600 );
	Serial.println( ":::Lunchbox Sampler:::" );
#endif
	LunchboxSampler& sampler = LunchboxSampler::getInstance();
	sampler.run();
}