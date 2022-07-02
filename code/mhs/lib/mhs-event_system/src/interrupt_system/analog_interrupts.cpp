#include "analog_interrupts.hpp"

const lbs::AnalogInterrupts::anlg_lookup &
lbs::AnalogInterrupts::getLookup() {
    static const auto* table = new anlg_lookup({
       { Events::Analog::POTS::POT0,        {_POT0, ADC_STATE::BOTH } },
       { Events::Analog::POTS::POT1,        {_POT1, ADC_STATE::BOTH } },
       { Events::Analog::POTS::POT2,        {_POT2, ADC_STATE::ADC0 } },
       { Events::Analog::POTS::POT3,        {_POT3, ADC_STATE::ADC1 } },
    });
}
