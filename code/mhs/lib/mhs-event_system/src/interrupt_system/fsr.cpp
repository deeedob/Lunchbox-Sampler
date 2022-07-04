#include "fsr.hpp"

using namespace lbs;
FSR* FSR::isr_instance = nullptr;

FSR::FSR(const std::shared_ptr<ADC>& adc, u_int8_t mpxPin0, u_int8_t mpxPin1, u_int8_t mpxPin2,u_int8_t mpxPin3, u_int16_t delta )
    : m_values({}), m_delta(delta)
{
    m_adc = adc;
    m_position = 0;
    m_pads[0] = Multiplex(mpxPin0);
    m_pads[1] = Multiplex(mpxPin1);
    m_pads[2] = Multiplex(mpxPin2);
    m_pads[3] = Multiplex(mpxPin3);
    rescanAll();
    isr_instance = this;
}

void FSR::isr() {
    auto i = FSR::isr_instance;
    int val = i->m_adc->adc1->analogReadContinuous();
    i->m_values[i->m_position] = val;
    /* TODO: construct MIDI event and enqueue? */
}

void FSR::enableISR(u_int8_t prio) {
    m_adc->adc1->enableInterrupts(isr, prio);
}

void FSR::disableISR() {
    m_adc->adc1->disableInterrupts();
}

void FSR::startScan() {
    stopScan();
    m_pads[m_position].setActive();      // select different mpx out
    int oldVal = m_values[m_position];
    m_adc->adc1->enableCompareRange(static_cast<int16_t>(oldVal - m_delta), static_cast<int16_t>(oldVal + m_delta), false, true);
    m_adc->adc1->startContinuous(FSR_POLL_);
}

void FSR::stopScan() {
    m_adc->adc1->stopContinuous();
    m_adc->adc1->disableCompare();
}

u_int8_t FSR::next() {
    if( m_position >= 4)
        m_position = 0;
    return m_position++;
}

u_int16_t FSR::getDelta() const {
    return m_delta;
}

void FSR::setDelta( u_int16_t mDelta ) {
    m_delta = mDelta;
}

void FSR::rescanAll() {
    for(int i = 0; i < 4; i++) {
        m_pads[i].setActive();
        m_values[i] = m_adc->adc1->analogRead(FSR_POLL_);
#ifdef VERBOSE
        Serial.print("FSR: "); Serial.print(i); Serial.print(" VAL: "); Serial.println(m_values[i]);
#endif
    }
}
