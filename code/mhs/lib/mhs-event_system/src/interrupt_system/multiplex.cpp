#include "multiplex.hpp"

using namespace lbs;

Multiplex::Multiplex( u_int8_t mpxPin ) {
    assert(("pin out of range", (mpxPin <= 7)));
    std::array<bool, 3> tmp = getTable().find(mpxPin)->second;
    fn_pinSelect = [pos = tmp]() {
        if(pos[0])
            digitalWriteFast(_FSR_SEL2, HIGH);
        else
            digitalWriteFast(_FSR_SEL2, LOW);
        if(pos[1])
            digitalWriteFast(_FSR_SEL1, HIGH);
        else
            digitalWriteFast(_FSR_SEL1, LOW);
        if(pos[2])
            digitalWriteFast(_FSR_SEL0, HIGH);
        else
            Serial.println("2 0");
    };
}

void Multiplex::setActive() {
    fn_pinSelect();
}

const Multiplex::mpx_lookup& Multiplex::getTable() {
    static const auto* table = new mpx_lookup({
        {0, {false, false, false}},
        {1, {false, false, true }},
        {2, {false, true, false }},
        {3, {false, true, true  }},
        {4, {true, false, false }},
        {5, {true, false, true  }},
        {6, {true, true, false  }},
        {7, {true, true, true   }},
    });
    return *table;
}