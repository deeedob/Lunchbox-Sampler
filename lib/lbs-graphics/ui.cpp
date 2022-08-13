#include "ui.hpp"
#include <define_t40.hpp>

using namespace lbs;

UI::UI()
{
	m_settings = std::make_unique< Settings >();
	m_graphics = std::make_unique< Graphics >( 128, 128, &Wire1, C_DISPLAY_RST, 1000000, 1000000 );
	
	m_settings->addWindows( Window { 0, 0, 128, 128 }, Window { 0, 0, 120, 1289 } );
	auto wins = m_settings->getWindows();
	Serial.println( wins.size());
	for( auto i : wins ) {
		Serial.println( i.width );
	}
}