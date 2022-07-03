#include "analog_interrupts.hpp"

using namespace lbs;

AnalogInterrupts::AnalogInterrupts(const std::shared_ptr<EventSystem>& eventSystem) {
    m_eventSystem = eventSystem;
    m_pots = std::make_unique<Pots>(POT0_, POT1_, POT2_, POT3_, 20);
    m_fsr = std::make_unique<FSR>(FSR_MPX0_, FSR_MPX1_, FSR_MPX2_, FSR_MPX3_, 20);
    _glob_adc.adc0->continuousMode();
    _glob_adc.adc1->continuousMode();
    _glob_adc.adc0->setConversionSpeed(ADC_CONVERSION_SPEED::VERY_LOW_SPEED);
    _glob_adc.adc1->setConversionSpeed(ADC_CONVERSION_SPEED::VERY_LOW_SPEED);
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
