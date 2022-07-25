#include "lunchbox_sampler.hpp"

LunchboxSampler::LunchboxSampler()
	: m_system { }
{
	m_system = std::make_shared< EventSystem >();
	setup();
	setupEventSystem();
}

LunchboxSampler::~LunchboxSampler() = default;

LunchboxSampler& LunchboxSampler::getInstance()
{
	static auto* singleton = new LunchboxSampler();
	return *singleton;
}

[[noreturn]] void LunchboxSampler::run()
{
	while( true ) {
		yield();
	}
}

/* TODO: extend properly */
void LunchboxSampler::setup()
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
	SPI.setSCK( C_SDCARD_SCK_PIN );
	SPI.setMOSI( C_SDCARD_MOSI_PIN );
}

void LunchboxSampler::setupEventSystem()
{
	setupDigitalEvents();
}

void LunchboxSampler::setupDigitalEvents()
{
	DigitalInterrupts digitalInterrupts( m_system );
	digitalInterrupts.enableAll();
	
	m_system->attachDigital( Events::DIGITAL::ROTARY_L, []() { Serial.println( "Rotary Left" ); } );
	m_system->attachDigital( Events::DIGITAL::ROTARY_R, []() { Serial.println( "Rotary Right" ); } );
	m_system->attachDigital( Events::DIGITAL::BTN_RETURN, []() { Serial.println( "Button Return" ); } );
	m_system->attachDigital( Events::DIGITAL::BTN_ENTER, []() { Serial.println( "Button Enter" ); } );
	m_system->attachDigital( Events::DIGITAL::BTN_TOGGLE, []() { Serial.println( "Button Toggle" ); } );
}

void LunchboxSampler::setupAnalogEvents()
{
	
	AnalogInterrupts analogInterrupts( m_system );
	analogInterrupts.enableAll();
	
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