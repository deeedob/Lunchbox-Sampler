#include "fsr.hpp"
#include "analog_interrupts.hpp"

using namespace lbs;

FSR::FSR( u_int8_t mpxPin0, u_int8_t mpxPin1, u_int8_t mpxPin2,u_int8_t mpxPin3, u_int16_t delta ) {
    m_pads[0] = Multiplex(mpxPin0);
    m_pads[1] = Multiplex(mpxPin1);
    m_pads[2] = Multiplex(mpxPin2);
    m_pads[3] = Multiplex(mpxPin3);
    m_delta = delta;
}

void FSR::isr() {
    int val = _glob_adc.adc1->analogReadContinuous();
    _glob_FSRValues[_glob_FSRPosition] = val;
    /* TODO: construct MIDI event and enqueue? */
}

void FSR::enableISR(u_int8_t prio) {
    _glob_adc.adc1->enableInterrupts(isr, prio);
}

void FSR::disableISR() {
    _glob_adc.adc1->disableInterrupts();
}

void FSR::startScan() {
    stopScan();
    m_pads[_glob_FSRPosition].setActive();      // select different mpx out
    int oldVal = _glob_FSRValues[_glob_FSRPosition];
    _glob_adc.adc1->enableCompareRange(static_cast<int16_t>(oldVal - m_delta), static_cast<int16_t>(oldVal + m_delta), false, true);
    _glob_adc.adc1->startContinuous(FSR_POLL_);
}

void FSR::stopScan() {
    _glob_adc.adc1->stopContinuous();
    _glob_adc.adc1->disableCompare();
}

u_int8_t FSR::next() {
    if( _glob_FSRPosition >= 4)
        _glob_FSRPosition = 0;
    return _glob_FSRPosition++;
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
        _glob_FSRValues[i] = _glob_adc.adc1->analogRead(FSR_POLL_);
#ifdef VERBOSE
        Serial.print("FSR: "); Serial.print(i); Serial.print(" VAL: "); Serial.println(_glob_FSRValues[i]);
#endif
    }
}
