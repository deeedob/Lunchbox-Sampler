#include "analog_interrupts.hpp"

using namespace lbs;

AnalogInterrupts::AnalogInterrupts(const std::shared_ptr<EventSystem>& eventSystem) {
    m_eventSystem = eventSystem;
    m_adc = std::make_shared<ADC>();
    m_adc->adc0->continuousMode();
    m_adc->adc1->continuousMode();
    m_adc->adc0->setAveraging(16);
    m_adc->adc1->setAveraging(16);
    m_adc->adc0->setConversionSpeed(ADC_CONVERSION_SPEED::VERY_LOW_SPEED);
    m_adc->adc1->setConversionSpeed(ADC_CONVERSION_SPEED::VERY_LOW_SPEED);
    m_adc->adc0->setSamplingSpeed(ADC_SAMPLING_SPEED::MED_SPEED);
    m_adc->adc1->setSamplingSpeed(ADC_SAMPLING_SPEED::MED_SPEED);

    m_pots = std::make_unique<Pots>(m_adc, POT0_, POT1_, POT2_, POT3_, 110);
    m_fsr = std::make_unique<FSR>(m_adc, FSR_MPX0_, FSR_MPX1_, FSR_MPX2_, FSR_MPX3_, 20);

}

const std::unique_ptr<Pots>& AnalogInterrupts::getPots() const {
    return m_pots;
}

const std::unique_ptr<FSR>& AnalogInterrupts::getFSR() const {
    return m_fsr;
}

void AnalogInterrupts::enableAll() {
    m_pots->enableISR();
    m_fsr->enableISR();
}

void AnalogInterrupts::disableAll() {
    m_pots->disableISR();
    m_fsr->disableISR();
}
