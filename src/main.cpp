#include "lunchbox_sampler.hpp"
/* Arduino ... why u do this shit?! */
#undef main
using namespace lbs;

int main()
{
#ifdef VERBOSE
	Serial.begin( 115200 );
	Serial.println( ":::Lunchbox Sampler:::" );
#endif
	auto& sampler = LunchboxSampler::getInstance();
	sampler.run();
	
	//pinMode( C_FSR_SEL_0, OUTPUT );
	//pinMode( C_FSR_SEL_1, OUTPUT );
	//pinMode( C_FSR_SEL_2, OUTPUT );
	//pinMode( C_FSR_POLL, INPUT );
	//Multiplex mpx( C_FSR_MPX_3 );
	//while( 1 ) {
	//	mpx.setActive();
	//	Serial.println( analogRead( C_FSR_POLL ) );
	//	delay( 200 );
	//}
}