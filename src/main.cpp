#include <lunchbox_sampler.hpp>

#undef main

int main()
{
#ifdef VERBOSE
	Serial.begin( 9600 );
	Serial.println( ":::Lunchbox Sampler:::" );
#endif
	
	pinMode( C_BTN_ENTER, INPUT_PULLUP );
	pinMode( C_BTN_RETURN, INPUT_PULLUP );
	pinMode( C_BTN_TOGGLE, INPUT_PULLUP );
	pinMode( C_ROTARY_A, INPUT_PULLUP );
	pinMode( C_ROTARY_B, INPUT_PULLUP );
	pinMode(LED_BUILTIN, INPUT );
	pinMode( C_FSR_SEL_0, OUTPUT );
	pinMode( C_FSR_SEL_1, OUTPUT );
	pinMode( C_FSR_SEL_2, OUTPUT );
	pinMode( C_FSR_POLL, INPUT );
	LunchboxSampler& sampler = LunchboxSampler::getInstance();
	sampler.run();
}