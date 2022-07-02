#pragma once
#include "events.hpp"
#include "define_t40.hpp"
#include <map>
#include <atomic>
#include <ADC.h>

namespace lbs {
    /* globals to access the adc values
     * We are mapping 8 analog values to 2 adc's
     * Do not use this directly!!!
     * */
    volatile std::atomic<std::array<u_int16_t, 8>> _analogOldValue;
    volatile std::atomic<u_int8_t> _analogReadPosition;
    class AnalogInterrupts
    {
    public:
        /* every pin has at least one of these properties
         * thanks @https://github.com/KurtE/TeensyDocuments/blob/master/Teensy4%20MicroMod%20Pins.pdf
         * Note that the AudioInputAnalog class uses adc0!
         */
        enum class ADC_STATE {
            ADC0 = 0,
            ADC1 = 1,
            BOTH = 2,
        };
        using anlg_lookup = std::map<Events::Analog::POTS, std::pair<u_int8_t, ADC_STATE>>;

        void enableAllIsr();
        void enableADC0Isr();
        void enableADC1Isr();

        void disableAllIsr();
        void disableADC0Isr();
        void disableADC1Isr();

        static const anlg_lookup& getLookup();

    private:
        ADC adc;
    };
}
