#include "fsr.hpp"
#include "analog_interrupts.hpp"
//#include "event_sytem.hpp"

using namespace lbs;

FSR* FSR::m_isrInstance = nullptr;

FSR::FSR( AnalogInterrupts* const parent, u_int8_t mpxPin0, u_int8_t mpxPin1, u_int8_t mpxPin2, u_int8_t mpxPin3, u_int16_t delta )
	: m_parent( parent ), m_values( { 0, 0, 0, 0 } ), m_delta( delta )
{
	m_position = 0;
	m_pads[ 0 ] = Multiplex( mpxPin0 );
	m_pads[ 1 ] = Multiplex( mpxPin1 );
	m_pads[ 2 ] = Multiplex( mpxPin2 );
	m_pads[ 3 ] = Multiplex( mpxPin3 );
	rescanAll();
	m_isrInstance = this;
}

FSR::~FSR()
{
	delete m_parent;
}

void FSR::isr()
{
	auto i = FSR::m_isrInstance;
	auto val = ( u_int16_t ) i->m_parent->getAdc()->adc1->analogReadContinuous();
	// TODO: remove updateRange from ISR?
	i->updateRange();
	i->m_values[ i->m_position ] = val;
	/* TODO: construct MIDI event and enqueue? */
	i->m_parent->getEventSystem()
	 ->enqueueAnalog( static_cast<Events::Analog::FSR>(i->m_position), { i->m_position, val } );
	
}

void FSR::enableISR( u_int8_t prio )
{
	m_parent->getAdc()->adc1->enableInterrupts( isr, prio );
}

void FSR::disableISR()
{
	m_parent->getAdc()->adc1->disableInterrupts();
}

void FSR::update()
{
	stopScan();
	m_pads[ m_position ].setActive();      // select different mpx out
	int oldVal = m_values[ m_position ];
	m_parent->getAdc()->adc1->enableCompareRange( oldVal - m_delta, oldVal + m_delta, false, true );
	m_parent->getAdc()->adc1->startContinuous( C_FSR_POLL );
}

void FSR::updateRange()
{
	stopScan();
	int oldVal = m_values[ m_position ];
	m_parent->getAdc()->adc1->enableCompareRange( oldVal - m_delta, oldVal + m_delta, false, true );
	m_parent->getAdc()->adc1->startContinuous( C_FSR_POLL );
}

void FSR::stopScan()
{
	m_parent->getAdc()->adc1->stopContinuous();
	m_parent->getAdc()->adc1->disableCompare();
}

u_int8_t FSR::next()
{
	if( ++m_position >= 4 )
		m_position = 0;
	return m_position;
}

u_int16_t FSR::recalibrateDelta( u_int16_t padding, u_int16_t samples )
{
	noInterrupts();
	stopScan();
	m_parent->getAdc()->adc1->setConversionSpeed( ADC_CONVERSION_SPEED::VERY_HIGH_SPEED );
	m_parent->getAdc()->adc1->setSamplingSpeed( ADC_SAMPLING_SPEED::VERY_HIGH_SPEED );
	u_int16_t lowest;
	u_int16_t highest;
	std::array< u_int16_t, 4 > deltas { 0, 0, 0, 0 };
	
	for( int i = 0; i < 4; i++ ) {       //cycle through the 4 pots
		m_pads[ i ].setActive();      // select different mpx out
		lowest = m_parent->getAdc()->adc1->analogRead( C_FSR_POLL );
		highest = m_parent->getAdc()->adc1->analogRead( C_FSR_POLL );
		for( int j = 0; j < samples; j++ ) {   //average out of 20
			u_int16_t tmp = m_parent->getAdc()->adc1->analogRead( C_FSR_POLL );
			delayMicroseconds( 50 );
			if( tmp < lowest )
				lowest = tmp;
			else if( tmp > highest )
				highest = tmp;
		}
		deltas[ i ] = highest - lowest;
	}
	/* find the highest delta value */
	u_int16_t highestDelta = 0;
	for( int i = 0; i < 4; i++ ) {
		if( deltas[ i ] > highestDelta )
			highestDelta = deltas[ i ];
	}
#ifdef VERBOSE
	Serial.print( "Recalibrate::FSR, VAL: " );
	Serial.println( highestDelta );
#endif
	interrupts();
	return highestDelta + padding;
}

u_int16_t FSR::getDelta() const
{
	return m_delta;
}

void FSR::setDelta( u_int16_t mDelta )
{
	m_delta = mDelta;
}

void FSR::rescanAll()
{
	for( int i = 0; i < 4; i++ ) {
		m_pads[ i ].setActive();
		m_values[ i ] = m_parent->getAdc()->adc1->analogRead( C_FSR_POLL );
#ifdef VERBOSE
		Serial.print( "FSR: " );
		Serial.print( i );
		Serial.print( " VAL: " );
		Serial.println( m_values[ i ] );
#endif
	}
}