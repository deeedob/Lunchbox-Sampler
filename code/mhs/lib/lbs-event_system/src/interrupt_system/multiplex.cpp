#include "multiplex.hpp"

using namespace lbs;

Multiplex::Multiplex( u_int8_t mpxPin ) {
    std::array<bool, 3> tmp = getTable().find(mpxPin)->second;
    fn_pinSelect = [pos = tmp]() {
        if(pos[0])
            digitalWriteFast(FSR_SEL2_, HIGH);
        else
            digitalWriteFast(FSR_SEL2_, LOW);
        if(pos[1])
            digitalWriteFast(FSR_SEL1_, HIGH);
        else
            digitalWriteFast(FSR_SEL1_, LOW);
        if(pos[2])
            digitalWriteFast(FSR_SEL0_, HIGH);
        else
            digitalWriteFast(FSR_SEL0_, LOW);
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