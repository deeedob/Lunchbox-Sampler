#include "fsr.hpp"

using namespace lbs;

FSR::FSR( u_int8_t mpxPin0, u_int8_t mpxPin1, u_int8_t mpxPin2,u_int8_t mpxPin3, u_int16_t delta ) {
    m_pads[0] = Multiplex(mpxPin0);
    m_pads[1] = Multiplex(mpxPin1);
    m_pads[2] = Multiplex(mpxPin2);
    m_pads[3] = Multiplex(mpxPin3);
    m_delta = delta;
    m_pos = 0;
}

// 0 - 4, 1 - 5, 2 - 6, 3 - 7
u_int8_t FSR::next() {
    if(m_pos >= 4)
        m_pos = 0;
    return (m_pos++)+4;
}

u_int16_t FSR::getDelta() const {
    return m_delta;
}

void FSR::setDelta( u_int16_t mDelta ) {
    m_delta = mDelta;
}

