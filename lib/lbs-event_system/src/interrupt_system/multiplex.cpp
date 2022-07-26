#include "multiplex.hpp"

using namespace lbs;

Multiplex::Multiplex( u_int8_t mpxPin)
{
	std::array< bool, 3 > pos = getTable().find( mpxPin )->second;
	m_fnPinSelect = [pos]() {
		if( pos[ 0 ] )
			digitalWriteFast( C_FSR_SEL_2, HIGH );
		else
			digitalWriteFast( C_FSR_SEL_2, LOW );
		if( pos[ 1 ] )
			digitalWriteFast( C_FSR_SEL_1, HIGH );
		else
			digitalWriteFast( C_FSR_SEL_1, LOW );
		if( pos[ 2 ] )
			digitalWriteFast( C_FSR_SEL_0, HIGH );
		else
			digitalWriteFast( C_FSR_SEL_0, LOW );
	};
}

void Multiplex::setActive()
{
	m_fnPinSelect();
}

const Multiplex::MpxLookup & Multiplex::getTable() {
    static const auto* table = new MpxLookup ({
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