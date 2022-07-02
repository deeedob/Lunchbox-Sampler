#pragma once
#include "analog_interrupts.hpp"

namespace lbs {
    /* globals to access the adc values. Do not use this directly!!! */
    std::array<int, 4> _glob_PotValues = {};
    volatile u_int8_t _glob_PotPosition = 0;

    /*! @class Manages the interrupts for the potentiometers.
     *  ADC0 is used exclusively to make things simple.
     *  */
    class Pots
    {
    public:
        explicit Pots(u_int8_t potPin0, u_int8_t potPin1, u_int8_t potPin2, u_int8_t potPin3, u_int16_t delta);

        static void isr();
        void enableISR(u_int8_t prio);
        void disableISR();
        void startScan();
        void stopScan();
        u_int8_t next();

        u_int16_t getDelta() const;
        void setDelta( u_int16_t mDelta );
    private:
        std::array<u_int8_t, 4> m_pots{};
        u_int16_t m_delta;
    };
}
