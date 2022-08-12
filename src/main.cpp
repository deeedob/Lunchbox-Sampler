#include <lunchbox_sampler.hpp>

/* Arduino ... why u do this shit?! */
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