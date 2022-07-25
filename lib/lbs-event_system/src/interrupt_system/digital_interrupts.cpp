#include "digital_interrupts.hpp"

using namespace lbs;
DigitalInterrupts* DigitalInterrupts::m_instance = nullptr;

DigitalInterrupts::DigitalInterrupts( const std::shared_ptr< EventSystem >& eventSystem, u_int8_t bounceTime )
	: m_btnEnter( C_BTN_ENTER, bounceTime ),
	  m_btnReturn( C_BTN_RETURN, bounceTime ),
	  m_btnToggle( C_BTN_TOGGLE, bounceTime )
{
	m_eventSystem = eventSystem;
	m_instance = this;
};

/* Disable all pins if class gets out of scope */
DigitalInterrupts::~DigitalInterrupts()
{
	disableAll();
}

void DigitalInterrupts::enableAll()
{
	for( auto& i : getTable()) {
		attachInterrupt( std::get< 0 >( i.second ), std::get< 1 >( i.second ), std::get< 2 >( i.second ));
	}
}

void DigitalInterrupts::enablePin( Events::DIGITAL e )
{
	auto tmp = getTable().find( e )->second;
	attachInterrupt( std::get< 0 >( tmp ), std::get< 1 >( tmp ), std::get< 2 >( tmp ));
}

void DigitalInterrupts::enableCustomizedPin( Events::DIGITAL e, void (* function)(), int mode )
{
	auto tmp = getTable().find( e )->second;
	attachInterrupt( std::get< 0 >( tmp ), function, mode );
}

void DigitalInterrupts::disablePin( Events::DIGITAL e )
{
	auto tmp = getTable().find( e )->second;
	detachInterrupt( std::get< 0 >( tmp ));
}

/* Events :: {Pin :: ISR :: Mode } */
const DigitalInterrupts::DigLookup& DigitalInterrupts::getTable()
{
	static const auto* table = new DigLookup( {
		                                          //{ Events::DIGITAL::ROTARY_R,   std::make_tuple( ROTARY_B_     , isr_rotaryB    , LOW     )},
		                                          { Events::DIGITAL::ROTARY_L,   std::make_tuple( C_ROTARY_A, isrRotaryA, LOW ) },
		                                          { Events::DIGITAL::BTN_ENTER,  std::make_tuple( C_BTN_ENTER, isrBtnEnter, CHANGE ) },
		                                          { Events::DIGITAL::BTN_RETURN, std::make_tuple( C_BTN_RETURN, isrBtnReturn, CHANGE ) },
		                                          { Events::DIGITAL::BTN_TOGGLE, std::make_tuple( C_BTN_TOGGLE, isrBtnToggle, CHANGE ) }} );
	return *table;
}

void DigitalInterrupts::disableAll()
{
	for( auto& i : getTable()) {
		detachInterrupt( std::get< 0 >( i.second ));
	}
}

void DigitalInterrupts::isrRotaryA()
{
	static unsigned long lastInterruptTime = 0;
	unsigned long interruptTime = millis();
	/* if the interrupt comes faster then 5ms assume it's a bounce */
	if( interruptTime - lastInterruptTime > 5 ) {
		if( digitalRead( C_ROTARY_B ) == HIGH ) {
			DigitalInterrupts::m_instance->m_eventSystem
			                             ->enqueueDigital( Events::DIGITAL::ROTARY_L );
#ifdef VERBOSE
			Serial.print( "ISR::ROTARY:: " );
			Serial.print( C_ROTARY_B );
			Serial.println( " LEFT" );
#endif
		} else {
			DigitalInterrupts::m_instance->m_eventSystem
			                             ->enqueueDigital( Events::DIGITAL::ROTARY_R );
#ifdef VERBOSE
			Serial.print( "ISR::ROTARY:: " );
			Serial.print( C_ROTARY_B );
			Serial.println( " RIGHT" );
#endif
		}
	}
	lastInterruptTime = interruptTime;
}

void DigitalInterrupts::isrRotaryB()
{
	static unsigned long lastInterruptTime = 0;
	unsigned long interruptTime = millis();
	/* if the interrupt comes faster then 5ms assume it's a bounce */
	if( interruptTime - lastInterruptTime > 5 ) {
	}
	lastInterruptTime = interruptTime;
}

void DigitalInterrupts::isrBtnEnter()
{
	DigitalInterrupts::m_instance->m_btnEnter.update();
	/* on button enter */
	if( DigitalInterrupts::m_instance->m_btnEnter.fallingEdge()) {
		DigitalInterrupts::m_instance->m_eventSystem
		                             ->enqueueDigital( Events::DIGITAL::BTN_ENTER );
#ifdef VERBOSE
		Serial.print( "ISR::BTN:: " );
		Serial.print( C_BTN_ENTER );
		Serial.println( " ENTER" );
#endif
	}
}

void DigitalInterrupts::isrBtnReturn()
{
	DigitalInterrupts::m_instance->m_btnReturn.update();
	/* on button enter */
	if( DigitalInterrupts::m_instance->m_btnReturn.fallingEdge()) {
		DigitalInterrupts::m_instance->m_eventSystem
		                             ->enqueueDigital( Events::DIGITAL::BTN_RETURN );
#ifdef VERBOSE
		Serial.print( "ISR::BTN:: " );
		Serial.print( C_BTN_RETURN );
		Serial.println( " RETURN" );
#endif
	}
}

void DigitalInterrupts::isrBtnToggle()
{
	DigitalInterrupts::m_instance->m_btnToggle.update();
	/* on button enter */
	if( DigitalInterrupts::m_instance->m_btnToggle.fallingEdge()) {
		DigitalInterrupts::m_instance->m_eventSystem
		                             ->enqueueDigital( Events::DIGITAL::BTN_TOGGLE );
#ifdef VERBOSE
		Serial.print( "ISR::BTN:: " );
		Serial.print( C_BTN_TOGGLE );
		Serial.println( " TOGGLE" );
#endif
	}
}