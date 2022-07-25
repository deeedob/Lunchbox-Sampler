#include "multiplex.hpp"

using namespace lbs;

Multiplex::Multiplex( u_int8_t mpxPin, u_int8_t sel0, u_int8_t sel1, u_int8_t sel2 )
{
	std::array< bool, 3 > pos = getTable().find( mpxPin )->second;
	m_fnPinSelect = [pos, sel0, sel1, sel2]() {
		if( pos[ 0 ] )
			digitalWriteFast( sel2, HIGH );
		else
			digitalWriteFast( sel2, LOW );
		if( pos[ 1 ] )
			digitalWriteFast( sel1, HIGH );
		else
			digitalWriteFast( sel1, LOW );
		if( pos[ 2 ] )
			digitalWriteFast( sel0, HIGH );
		else
			digitalWriteFast( sel0, LOW );
	};
}

void Multiplex::setActive()
{
	m_fnPinSelect();
}

const Multiplex::MpxLookup& Multiplex::getTable()
{
	static const auto* table = new MpxLookup( {{ 0, { false, false, false }},
	                                           { 1, { false, false, true }},
	                                           { 2, { false, true,  false }},
	                                           { 3, { false, true,  true }},
	                                           { 4, { true,  false, false }},
	                                           { 5, { true,  false, true }},
	                                           { 6, { true,  true,  false }},
	                                           { 7, { true,  true,  true }},
	                                          } );
	return *table;
}