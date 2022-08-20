#include "lunchbox_sampler.hpp"
/* Arduino ... why u do this shit?! */
#undef main
using namespace lbs;
int main()
{
#ifdef VERBOSE
	Serial.begin( 9600 );
	Serial.println( ":::Lunchbox Sampler:::" );
#endif
	auto& sampler = LunchboxSampler::getInstance();
	
	sampler.run();
}