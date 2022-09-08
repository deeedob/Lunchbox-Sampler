#include "pots.hpp"
#include "analog_interrupts.hpp"
//#include "event_sytem.hpp"

using namespace lbs;

Pots* Pots::m_isrInstance = nullptr;

Pots::Pots( const AnalogInterrupts* const parent, u_int8_t pot0, u_int8_t pot1, u_int8_t pot2, u_int8_t pot3, u_int16_t delta )
	: m_parent( parent ), m_values( { 0, 0, 0, 0 } ), m_pots( { pot0, pot1, pot2, pot3 } ), m_delta( delta )
{
	m_position = 0;
	for( auto i : m_pots ) {
		pinMode( i, INPUT );
	}
	rescanAll();
	m_isrInstance = this;
}

Pots::~Pots()
{
	delete m_parent;
}

void Pots::isr()
{
	auto& i = Pots::m_isrInstance;
	auto val = (u_int16_t) i->m_parent->getAdc()->adc0->analogReadContinuous();
	i->m_values[ i->m_position ] = val;
	i->update();
	i->m_parent->getEventSystem()
	 ->enqueueAnalog( static_cast<Events::Analog::POTS>(i->m_position), { i->m_position, val } );
	
}

void Pots::enableISR( u_int8_t prio )
{
	m_parent->getAdc()->adc0->enableInterrupts( isr, prio );
}

void Pots::disableISR()
{
	m_parent->getAdc()->adc0->disableInterrupts();
}

void Pots::update()
{
	noInterrupts();
	stopScan();
	u_int16_t oldVal = m_values[ m_position ];
	m_parent->getAdc()->adc0->enableCompareRange( oldVal - m_delta, oldVal + m_delta, false, true );
	m_parent->getAdc()->adc0->startContinuous( m_pots[ m_position ] );
	interrupts();
}

void Pots::stopScan()
{
	m_parent->getAdc()->adc0->stopContinuous();
	m_parent->getAdc()->adc0->disableCompare();
}

u_int8_t Pots::next()
{
	noInterrupts();
	if( ++m_position >= 4 )
		m_position = 0;
	interrupts();
	return m_position;
}

u_int16_t Pots::recalibrateDelta( u_int16_t padding, u_int16_t samples )
{
	noInterrupts();
	stopScan();
	m_parent->getAdc()->adc0->setConversionSpeed( ADC_CONVERSION_SPEED::VERY_HIGH_SPEED );
	m_parent->getAdc()->adc0->setSamplingSpeed( ADC_SAMPLING_SPEED::VERY_HIGH_SPEED );
	u_int16_t lowest;
	u_int16_t highest;
	std::array<u_int16_t, 4> deltas { 0, 0, 0, 0 };
	
	for( int i = 0; i < 4; i++ ) {       //cycle through the 4 pots
		lowest = m_parent->getAdc()->adc0->analogRead( m_pots[ i ] );
		highest = m_parent->getAdc()->adc0->analogRead( m_pots[ i ] );
		for( int j = 0; j < samples; j++ ) {   //average out of 20
			u_int16_t tmp = m_parent->getAdc()->adc0->analogRead( m_pots[ i ] );
			delayMicroseconds( 50 );
			if( tmp < lowest )
				lowest = tmp;
			else if( tmp > highest )
				highest = tmp;
		}
		deltas[ i ] = highest - lowest;
	}
	/* find the highest delta value*/
	u_int16_t highestDelta = 0;
	for( int i = 0; i < 4; i++ ) {
		if( deltas[ i ] > highestDelta )
			highestDelta = deltas[ i ];
	}
#ifdef VERBOSE
	Serial.print( "Recalibrate::POTS, VAL: " );
	Serial.println( highestDelta );
#endif
	interrupts();
	return highestDelta + padding;
}

u_int16_t Pots::getDelta() const
{
	return m_delta;
}

void Pots::setDelta( u_int16_t mDelta )
{
	m_delta = mDelta;
}

void Pots::rescanAll()
{
	/* todo does it work to change the "normal value" in static contect */
	for( int i = 0; i < 4; i++ ) {
		m_values[ i ] = m_parent->getAdc()->analogRead( m_pots[ i ] );
#ifdef VERBOSE
		Serial.print( "POT: " );
		Serial.print( i );
		Serial.print( " VAL: " );
		Serial.println( m_values[ i ] );
#endif
	}
}