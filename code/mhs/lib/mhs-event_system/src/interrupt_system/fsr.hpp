#pragma once
#include "multiplex.hpp"
#include "analog_interrupts.hpp"

namespace lbs {
    /* globals to access the adc values. Do not use this directly!!! */
    std::array<int , 4> _glob_FSRValues = {};
    volatile u_int8_t _glob_FSRPosition = 0;

    /*! @class Manages the interrupts for the FSR Pads.
     *  ADC1 is used exclusively to make things simpler.
     * */
    class FSR
    {
    public:
        explicit FSR(u_int8_t mpxPin0, u_int8_t mpxPin1, u_int8_t mpxPin2, u_int8_t mpxPin3, u_int16_t delta );

        static void isr();
        /*! enables the interrupt for the multiplexer poll out.
         *  ADC1 is used exclusively for this conversion.
         */
        void enableISR(u_int8_t prio);
        void disableISR();
        void startScan();
        void stopScan();
        u_int8_t next();

        u_int16_t getDelta() const;
        void setDelta( u_int16_t mDelta );
    private:
        u_int16_t m_delta;
        std::array<Multiplex, 4> m_pads;
    };
}
