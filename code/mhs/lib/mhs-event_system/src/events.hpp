#pragma once

namespace lbs {
    class Events
    {
    public:
        enum class DIGITAL
        {
            ROTARY_L,
            ROTARY_R,
            BTN_ENTER,
            BTN_RETURN,
            BTN_TOGGLE,
            MIDI_IN, // maybe not?
        };
        class Analog {
        public:
            enum class POTS
            {
                POT0 = 0,
                POT1 = 1,
                POT2 = 2,
                POT3 = 3,
            };
            enum class FSR
            {
                FSR0 = 4,
                FSR1 = 5,
                FSR2 = 6,
                FSR3 = 7
            };
        };
    };
}
