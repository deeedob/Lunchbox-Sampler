#include <lunchbox_sampler.hpp>

#undef main

int main()
{
#ifdef VERBOSE
	Serial.begin( 9600 );
	Serial.println( ":::Lunchbox Sampler:::" );
#endif
	//LunchboxSampler& sampler = LunchboxSampler::getInstance();
	//sampler.run();
	auto es = std::make_shared<EventSystem>();
	AnalogInterrupts ai(es);
	ai.enableAll();
	auto& fsr = ai.getFSR();
	
	es->attachAnalog( Events::Analog::FSR::FSR_0, []( AnalogData d ) {
		Serial.print( "FSR Pos: " );
		Serial.print( d.m_pos );
		Serial.print( " , Data: " );
		Serial.println( d.m_data );
	} );
	es->attachAnalog( Events::Analog::FSR::FSR_1, []( AnalogData d ) {
		Serial.print( "FSR Pos: " );
		Serial.print( d.m_pos );
		Serial.print( " , Data: " );
		Serial.println( d.m_data );
	} );
	es->attachAnalog( Events::Analog::FSR::FSR_2, []( AnalogData d ) {
		Serial.print( "FSR Pos: " );
		Serial.print( d.m_pos );
		Serial.print( " , Data: " );
		Serial.println( d.m_data );
	} );
	es->attachAnalog( Events::Analog::FSR::FSR_3, []( AnalogData d ) {
		Serial.print( "FSR Pos: " );
		Serial.print( d.m_pos );
		Serial.print( " , Data: " );
		Serial.println( d.m_data );
	} );
	
	while( true ) {
		fsr->update();
		delay( 200 );
		fsr->next();
	}
	
	
	
	// end::[]
}