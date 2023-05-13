#include "lunchbox_sampler.hpp"

LunchboxSampler::LunchboxSampler()
	: m_system( std::make_shared<EventSystem>()), m_memory()
{
	setup();
	MainMemory::init();
	m_digitalInterrupts = std::make_unique<DigitalInterrupts>( m_system );
	m_analogInterrupts = std::make_unique<AnalogInterrupts>( m_system );
	m_states = std::make_unique<BaseStates>( m_audio );
	
	setupDigitalEvents();
	setupAnalogEvents();
	setupFSREvents();
	m_audio = std::make_shared<Audio>( Audio::POLYPHONY::MEDIUM );
	m_midiListener = std::make_unique<MidiListener>( m_audio );
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
	rP->setDelta( 40 );
	rF->setDelta( 25 );
	while ( true ) {
		rP->update();
		rF->update();
		threads.delay( 4 );
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
{ }

void LunchboxSampler::setupDigitalEvents()
{
	m_digitalInterrupts->enableAll();
	
	m_system->attachDigital( Events::DIGITAL::ROTARY_L, [ & ]( Events::DIGITAL e ) { m_states->baseUpdate( e ); } );
	m_system->attachDigital( Events::DIGITAL::ROTARY_R, [ & ]( Events::DIGITAL e ) { m_states->baseUpdate( e ); } );
	m_system->attachDigital( Events::DIGITAL::BTN_ENTER, [ & ]( Events::DIGITAL e ) { m_states->baseUpdate( e ); } );
	m_system->attachDigital( Events::DIGITAL::BTN_RETURN, [ & ]( Events::DIGITAL e ) { m_states->baseUpdate( e ); } );
	m_system->attachDigital( Events::DIGITAL::BTN_TOGGLE, []() { Serial.println( "Button Toggle" ); } );
}

void LunchboxSampler::setupAnalogEvents()
{
	
	m_analogInterrupts->getPots()->enableISR();
	
	m_system->attachAnalog( Events::Analog::POTS::POT_0, [ = ]( AnalogData d ) {
		m_states->baseUpdate( Events::Analog::POTS::POT_0, d );
	} );
	m_system->attachAnalog( Events::Analog::POTS::POT_1, [ = ]( AnalogData d ) {
		m_states->baseUpdate( Events::Analog::POTS::POT_1, d );
	} );
	m_system->attachAnalog( Events::Analog::POTS::POT_2, [ = ]( AnalogData d ) {
		m_states->baseUpdate( Events::Analog::POTS::POT_2, d );
	} );
	m_system->attachAnalog( Events::Analog::POTS::POT_3, [ = ]( AnalogData d ) {
		m_states->baseUpdate( Events::Analog::POTS::POT_3, d );
	} );
}

void LunchboxSampler::setupFSREvents()
{
	
	m_analogInterrupts->getFSR()->enableISR();
	
	m_system->attachAnalog( Events::Analog::FSR::FSR_0, [ & ]( AnalogData d ) {
		if( d.m_pos ) m_audio->playNote( Note( 0, 0, 0, 1.0f ));
		else
			m_audio->stopNote( Note( 0, 0, 0, 1.0f ));
	} );
	m_system->attachAnalog( Events::Analog::FSR::FSR_1, [ & ]( AnalogData d ) {
		if( d.m_pos ) m_audio->playNote( Note( 0, 0, 1, 1.0f ));
		else
			m_audio->stopNote( Note( 0, 0, 1, 1.0f ));
	} );
	m_system->attachAnalog( Events::Analog::FSR::FSR_2, [ & ]( AnalogData d ) {
		if( d.m_pos ) m_audio->playNote( Note( 0, 0, 2, 1.0f ));
		else
			m_audio->stopNote( Note( 0, 0, 2, 1.0f ));
	} );
	m_system->attachAnalog( Events::Analog::FSR::FSR_3, [ & ]( AnalogData d ) {
		if( d.m_pos ) m_audio->playNote( Note( 0, 0, 3, 1.0f ));
		else
			m_audio->stopNote( Note( 0, 0, 3, 1.0f ));
	} );
}