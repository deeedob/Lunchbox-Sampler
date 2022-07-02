#include "pots.hpp"

using namespace lbs;

Pots::Pots( u_int8_t potPin0, u_int8_t potPin1, u_int8_t potPin2, u_int8_t potPin3, u_int16_t delta )
    : m_pots({potPin0, potPin1, potPin2, potPin3}), m_delta(delta)
{
}

void Pots::isr() {
    int val = _glob_adc.adc1->analogReadContinuous();
    _glob_PotValues[_glob_PotPosition] = val;
    /* TODO: enqueue */
}

void Pots::enableISR( u_int8_t prio ) {
    _glob_adc.adc0->enableInterrupts(isr, prio);
}

void Pots::disableISR() {
    _glob_adc.adc0->disableInterrupts();
}

void Pots::startScan() {
    stopScan();
    int oldVal = _glob_PotValues[_glob_PotPosition];
    _glob_adc.adc0->enableCompareRange(static_cast<int16_t>(oldVal - m_delta), static_cast<int16_t>(oldVal + m_delta), false, true);
    _glob_adc.adc0->startContinuous(m_pots[_glob_PotPosition]);
}

void Pots::stopScan() {
    _glob_adc.adc0->stopContinuous();
    _glob_adc.adc0->disableCompare();
}

u_int8_t Pots::next() {
    if( _glob_PotPosition >= 4 )
        _glob_PotPosition = 0;
    return _glob_PotPosition++;
}

u_int16_t Pots::getDelta() const {
    return m_delta;
}

void Pots::setDelta( u_int16_t mDelta ) {
    m_delta = mDelta;
}
