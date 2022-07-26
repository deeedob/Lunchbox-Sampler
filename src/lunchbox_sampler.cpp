#include "lunchbox_sampler.hpp"

LunchboxSampler::LunchboxSampler()
	: m_system( std::make_shared< EventSystem >())
{
	setup();
	m_digitalInterrupts = std::make_unique< DigitalInterrupts >( m_system );
	m_analogInterrupts = std::make_unique< AnalogInterrupts >( m_system );
	setupDigitalEvents();
	setupAnalogEvents();
#ifdef VERBOSE
	Serial.println( "::: finished setup :::" );
#endif

}

LunchboxSampler::~LunchboxSampler() = default;

LunchboxSampler& LunchboxSampler::getInstance()
{
	static auto* singleton = new LunchboxSampler();
	return *singleton;
}

[[noreturn]] void LunchboxSampler::run()
{
	auto& rP = m_analogInterrupts->getPots();
	auto& rF = m_analogInterrupts->getFSR();
	rP->setDelta( 100 );
	while( true ) {
		rP->update();
		rP->next();
		delay( 200 );
		rF->update();
		rF->next();
	}
}

/* TODO: extend properly */
void LunchboxSampler::setup()
{
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
}

void LunchboxSampler::setupEventSystem()
{
	setupDigitalEvents();
}

void LunchboxSampler::setupDigitalEvents()
{
	m_digitalInterrupts->enableAll();
	
	m_system->attachDigital( Events::DIGITAL::ROTARY_L, []() {
		Serial.println( "Rotary Left" );
	} );
	m_system->attachDigital( Events::DIGITAL::ROTARY_R, []() {
		Serial.println( "Rotary Right" );
	} );
	m_system->attachDigital( Events::DIGITAL::BTN_RETURN, []() {
		Serial.println( "Button Return" );
	} );
	m_system->attachDigital( Events::DIGITAL::BTN_ENTER, []() {
		Serial.println( "Button Enter" );
	} );
	m_system->attachDigital( Events::DIGITAL::BTN_TOGGLE, []() {
		Serial.println( "Button Toggle" );
	} );
	
}

void LunchboxSampler::setupAnalogEvents()
{
	
	m_analogInterrupts->getPots()->enableISR();
	
	m_system->attachAnalog( Events::Analog::POTS::POT_0, []( AnalogData d ) {
		Serial.print( "Analog Pos: " );
		Serial.print( d.m_pos );
		Serial.println( " , Data: " );
		Serial.println( d.m_data );
	} );
	m_system->attachAnalog( Events::Analog::POTS::POT_1, []( AnalogData d ) {
		Serial.print( "Analog Pos: " );
		Serial.print( d.m_pos );
		Serial.println( " , Data: " );
		Serial.println( d.m_data );
	} );
	m_system->attachAnalog( Events::Analog::POTS::POT_2, []( AnalogData d ) {
		Serial.print( "Analog Pos: " );
		Serial.print( d.m_pos );
		Serial.println( " , Data: " );
		Serial.println( d.m_data );
	} );
	m_system->attachAnalog( Events::Analog::POTS::POT_3, []( AnalogData d ) {
		Serial.print( "Analog Pos: " );
		Serial.print( d.m_pos );
		Serial.println( " , Data: " );
		Serial.println( d.m_data );
	} );
}

void LunchboxSampler::setupFSREvents()
{
	
	m_analogInterrupts->getFSR()->enableISR();
	
	m_system->attachAnalog( Events::Analog::FSR::FSR_0, []( AnalogData d ) {
		Serial.print( "FSR Pos: " );
		Serial.print( d.m_pos );
		Serial.println( " , Data: " );
		Serial.println( d.m_data );
	} );
	m_system->attachAnalog( Events::Analog::FSR::FSR_1, []( AnalogData d ) {
		Serial.print( "FSR Pos: " );
		Serial.print( d.m_pos );
		Serial.println( " , Data: " );
		Serial.println( d.m_data );
	} );
	m_system->attachAnalog( Events::Analog::FSR::FSR_2, []( AnalogData d ) {
		Serial.print( "FSR Pos: " );
		Serial.print( d.m_pos );
		Serial.println( " , Data: " );
		Serial.println( d.m_data );
	} );
	m_system->attachAnalog( Events::Analog::FSR::FSR_3, []( AnalogData d ) {
		Serial.print( "FSR Pos: " );
		Serial.print( d.m_pos );
		Serial.println( " , Data: " );
		Serial.println( d.m_data );
	} );
}