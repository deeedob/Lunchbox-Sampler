#pragma once
#include "multiplex.hpp"
#include <memory>
#include <ADC.h>

namespace lbs {


    /*! @class Manages the interrupts for the FSR Pads.
     *  ADC1 is used exclusively to make things simpler.
     * */
    class FSR
    {
    public:
        explicit FSR(const std::shared_ptr<ADC>& adc, u_int8_t mpxPin0, u_int8_t mpxPin1, u_int8_t mpxPin2, u_int8_t mpxPin3, u_int16_t delta );

        static void isr();
        /*! enables the interrupt for the multiplexer poll out.
         *  ADC1 is used exclusively for this conversion.
         */
        void enableISR(u_int8_t prio = 255);
        void disableISR();
        void startScan();
        void stopScan();
        u_int8_t next();

        u_int16_t getDelta() const;
        void setDelta( u_int16_t mDelta );
    private:
        void rescanAll();
    private:
        std::shared_ptr<ADC> m_adc;
        std::array<int , 4> m_values;
        volatile u_int8_t m_position;
        std::array<Multiplex, 4> m_pads;
        u_int16_t m_delta;
        static FSR* isr_instance;
    };
}
