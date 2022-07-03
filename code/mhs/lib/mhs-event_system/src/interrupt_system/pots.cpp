#include "pots.hpp"
#include "analog_interrupts.hpp"

using namespace lbs;

Pots::Pots( u_int8_t pot0, u_int8_t pot1, u_int8_t pot2, u_int8_t pot3, u_int16_t delta )
    : m_pots({pot0, pot1, pot2, pot3}), m_delta(delta)
{
    rescanAll();
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

void Pots::rescanAll() {
    for(int i = 0; i < 4; i++) {
        _glob_PotValues[i] = _glob_adc.adc0->analogRead(m_pots[i]);
#ifdef VERBOSE
        Serial.print("POT: "); Serial.print(i); Serial.print(" VAL: "); Serial.println(_glob_PotValues[i]);
#endif
    }
}
