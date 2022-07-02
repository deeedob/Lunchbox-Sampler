#include "analog_interrupts.hpp"

using namespace lbs;

AnalogInterrupts::AnalogInterrupts() {
    pots = std::make_unique<Pots>(_POT0, _POT1, _POT2, _POT3, 20);
    fsr = std::make_unique<FSR>(_FSR_MPX0, _FSR_MPX1, _FSR_MPX2, _FSR_MPX3, 20);
    _glob_adc.adc0->continuousMode();
    _glob_adc.adc1->continuousMode();
    _glob_adc.adc0->setConversionSpeed(ADC_CONVERSION_SPEED::VERY_LOW_SPEED);
    _glob_adc.adc1->setConversionSpeed(ADC_CONVERSION_SPEED::VERY_LOW_SPEED);
}

const std::unique_ptr<Pots>& AnalogInterrupts::getPots() const {
    return pots;
}

const std::unique_ptr<FSR>& AnalogInterrupts::getFSR() const {
    return fsr;
}
