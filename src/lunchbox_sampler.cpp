#include "lunchbox_sampler.hpp"

LunchboxSampler::LunchboxSampler()
	: m_system( std::make_shared<EventSystem>())
{
	m_digitalInterrupts = std::make_unique<DigitalInterrupts>( m_system );
	m_analogInterrupts = std::make_unique<AnalogInterrupts>( m_system );
	m_states = std::make_unique<BaseStates>();
	
	setup();
	setupDigitalEvents();
	setupAnalogEvents();
	setupFSREvents();
	m_analogInterrupts->disableAll();
	m_digitalInterrupts->disableAll();
}

LunchboxSampler::~LunchboxSampler() = default;

LunchboxSampler& LunchboxSampler::getInstance()
{
	static auto* singleton = new LunchboxSampler();
	return *singleton;
}

[[noreturn]] void LunchboxSampler::run()
{
	const auto& rP = m_analogInterrupts->getPots();
	const auto& rF = m_analogInterrupts->getFSR();
	rP->setDelta( 50 );
	rF->setDelta( 20 );
	while ( true ) {
		rP->update();
		rF->update();
		delay( 40 );
		rP->next();
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
		Serial.print( " , Data: " );
		Serial.println( d.m_data );
	} );
	m_system->attachAnalog( Events::Analog::POTS::POT_1, []( AnalogData d ) {
		Serial.print( "Analog Pos: " );
		Serial.print( d.m_pos );
		Serial.print( " , Data: " );
		Serial.println( d.m_data );
	} );
	m_system->attachAnalog( Events::Analog::POTS::POT_2, []( AnalogData d ) {
		Serial.print( "Analog Pos: " );
		Serial.print( d.m_pos );
		Serial.print( " , Data: " );
		Serial.println( d.m_data );
	} );
	m_system->attachAnalog( Events::Analog::POTS::POT_3, []( AnalogData d ) {
		Serial.print( "Analog Pos: " );
		Serial.print( d.m_pos );
		Serial.print( " , Data: " );
		Serial.println( d.m_data );
	} );
}

void LunchboxSampler::setupFSREvents()
{
	
	m_analogInterrupts->getFSR()->enableISR();
	
	m_system->attachAnalog( Events::Analog::FSR::FSR_0, []( AnalogData d ) {
		Serial.print( "FSR Pos: " );
		Serial.print( d.m_pos );
		Serial.print( " , Data: " );
		Serial.println( d.m_data );
	} );
	m_system->attachAnalog( Events::Analog::FSR::FSR_1, []( AnalogData d ) {
		Serial.print( "FSR Pos: " );
		Serial.print( d.m_pos );
		Serial.print( " , Data: " );
		Serial.println( d.m_data );
	} );
	m_system->attachAnalog( Events::Analog::FSR::FSR_2, []( AnalogData d ) {
		Serial.print( "FSR Pos: " );
		Serial.print( d.m_pos );
		Serial.print( " , Data: " );
		Serial.println( d.m_data );
	} );
	m_system->attachAnalog( Events::Analog::FSR::FSR_3, []( AnalogData d ) {
		Serial.print( "FSR Pos: " );
		Serial.print( d.m_pos );
		Serial.print( " , Data: " );
		Serial.println( d.m_data );
	} );
}