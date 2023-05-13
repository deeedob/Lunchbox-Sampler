#include "fsr.hpp"
#include "analog_interrupts.hpp"

using namespace lbs;

FSR* FSR::m_isrInstance = nullptr;

FSR::FSR( AnalogInterrupts* const parent, u_int8_t mpxPin0, u_int8_t mpxPin1, u_int8_t mpxPin2, u_int8_t mpxPin3, u_int16_t delta )
	: m_parent( parent ), m_values( { 0, 0, 0, 0 } ), m_delta( delta )
{
	m_position = 0;
	m_pads[ 0 ] = std::make_tuple( Multiplex( mpxPin0 ), false, 480 );
	m_pads[ 1 ] = std::make_tuple( Multiplex( mpxPin1 ), false, 550 );
	m_pads[ 2 ] = std::make_tuple( Multiplex( mpxPin2 ), false, 530 );
	m_pads[ 3 ] = std::make_tuple( Multiplex( mpxPin3 ), false, 420 );
	rescanAll();
	m_isrInstance = this;
}

FSR::~FSR()
{
	delete m_parent;
}

void FSR::isr()
{
	static unsigned long last_interrupt_time = 0;
	unsigned long interrupt_time = millis();
	auto& i = FSR::m_isrInstance;
	auto val = (u_int16_t) i->m_parent->getAdc()->adc1->analogReadContinuous();
	//i->m_values[ i->m_position ] = val;
	
	if( last_interrupt_time - interrupt_time > 5 ) {
		/* TODO: construct MIDI event and enqueue? */
		auto& note = std::get<STATE>( i->m_pads[ i->m_position ] );
		if( !note ) {
			note = true; // Trigger is ON
		} else {
			note = false; // Trigger is OFF
		}
		i->m_parent->getEventSystem()
		 ->enqueueAnalog( static_cast<Events::Analog::FSR>(i->m_position), { note, val } );
		i->update();
	}
	
	last_interrupt_time = interrupt_time;
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
	noInterrupts();
	stopScan();
	std::get<MPX>( m_pads[ m_position ] ).setActive();      // set the mpx pin to read active
	if( !std::get<STATE>( m_pads[ m_position ] )) { // if the pad is Note Off then wait for a trigger
		m_parent->getAdc()->adc1->enableCompareRange( std::get<LOW_BORDER>( m_pads[ m_position ] ), 1024, true, true );
	} else { //else wait till the value is back to normal state
		m_parent->getAdc()->adc1->enableCompareRange( std::get<LOW_BORDER>( m_pads[ m_position ] ) - 170, 1024, false, true );
	}
	m_parent->getAdc()->adc1->startContinuous( C_FSR_POLL );
	interrupts();
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
	noInterrupts();
	if( ++m_position >= 4 )
		m_position = 0;
	
	interrupts();
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
	std::array<u_int16_t, 4> deltas { 0, 0, 0, 0 };
	
	for( int i = 0; i < 4; i++ ) {       //cycle through the 4 pots
		std::get<MPX>( m_pads[ i ] ).setActive();      // select different mpx out
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

//TODO: fix only rescanning 3rd
void FSR::rescanAll()
{
	stopScan();
	for( int i = 0; i < m_pads.size(); i++ ) {
		std::get<MPX>( m_pads[ i ] ).setActive();
		m_values[ i ] = m_parent->getAdc()->analogRead( C_FSR_POLL );
#ifdef VERBOSE
		Serial.print( "FSR: " );
		Serial.print( i );
		Serial.print( " VAL: " );
		Serial.println( m_values[ i ] );
#endif
	}
}