#include "pots.hpp"

using namespace lbs;
Pots* Pots::isr_instance = nullptr;

Pots::Pots( const std::shared_ptr<ADC>& adc, u_int8_t pot0, u_int8_t pot1, u_int8_t pot2, u_int8_t pot3, u_int16_t delta)
    : m_values({}), m_pots({pot0, pot1, pot2, pot3}), m_delta(delta)
{
    m_adc = adc;
    m_position = 0;
    rescanAll();
    isr_instance = this;
}

void Pots::isr() {
    auto i = Pots::isr_instance;
    int val = i->m_adc->adc0->analogReadContinuous();
    i->m_values[i->m_position] = val;
#ifdef VERBOSE
    Serial.print("IRS::POT: "); Serial.print(i->m_position); Serial.print(" VAL: "); Serial.println(val);
#endif
    /* TODO: enqueue */
}

void Pots::enableISR( u_int8_t prio ) {
    m_adc->adc0->enableInterrupts(isr, prio);
}

void Pots::disableISR() {
    m_adc->adc0->disableInterrupts();
}

void Pots::startScan() {
    stopScan();
    int oldVal = m_values[m_position];
    m_adc->adc0->enableCompareRange(static_cast<int16_t>(oldVal - m_delta), static_cast<int16_t>(oldVal + m_delta), false, true);
    m_adc->adc0->startContinuous(m_pots[m_position]);
}

void Pots::stopScan() {
    m_adc->adc0->stopContinuous();
    m_adc->adc0->disableCompare();
}

u_int8_t Pots::next() {
    /* todo access without instance? */
    if( m_position >= 4 )
        m_position = 0;
    return m_position++;
}

u_int16_t Pots::getDelta() const {
    return m_delta;
}

void Pots::setDelta( u_int16_t mDelta ) {
    m_delta = mDelta;
}

void Pots::rescanAll() {
    /* todo does it work to change the "normal value" in static contect */
    for(int i = 0; i < 4; i++) {
        m_values[i] = m_adc->analogRead(m_pots[i]);
#ifdef VERBOSE
        Serial.print("POT: "); Serial.print(i); Serial.print(" VAL: "); Serial.println(m_values[i]);
#endif
    }
}
