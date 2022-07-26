#include "analog_interrupts.hpp"

using namespace lbs;

AnalogInterrupts::AnalogInterrupts( const std::shared_ptr< EventSystem >& eventSystem )
{
	m_eventSystem = eventSystem;
	m_adc = std::make_unique< ADC >();
	m_adc->adc0->continuousMode();
	m_adc->adc1->continuousMode();
	m_adc->adc0->setAveraging( 16 );
	m_adc->adc1->setAveraging( 16 );
	m_adc->adc0->setConversionSpeed( ADC_CONVERSION_SPEED::VERY_LOW_SPEED );
	m_adc->adc1->setConversionSpeed( ADC_CONVERSION_SPEED::VERY_LOW_SPEED );
	m_adc->adc0->setSamplingSpeed( ADC_SAMPLING_SPEED::MED_SPEED );
	m_adc->adc1->setSamplingSpeed( ADC_SAMPLING_SPEED::MED_SPEED );
	
	m_pots = std::make_unique< Pots >( this, C_POT_0, C_POT_1, C_POT_2, C_POT_3, 50 );
	m_fsr = std::make_unique< FSR >( this, C_FSR_MPX_0, C_FSR_MPX_1, C_FSR_MPX_2, C_FSR_MPX_3, 50 );
}

const std::unique_ptr< Pots >& AnalogInterrupts::getPots() const
{
	return m_pots;
}

const std::unique_ptr< FSR >& AnalogInterrupts::getFSR() const
{
	return m_fsr;
}

// TODO: return optional ADC1 | 2 depending on callee
const std::unique_ptr< ADC >& AnalogInterrupts::getAdc() const
{
	return m_adc;
}

void AnalogInterrupts::enableAll()
{
	m_pots->enableISR();
	m_fsr->enableISR();
}

void AnalogInterrupts::disableAll()
{
	m_pots->disableISR();
	m_fsr->disableISR();
}

const std::shared_ptr< EventSystem >& AnalogInterrupts::getEventSystem() const
{
	return m_eventSystem;
}