#include "ui.hpp"

UI::UI()
{
	m_settings = std::make_unique<Settings>();
	m_graphics = std::make_unique<Graphics>( 128, 128 );
	
	m_settings->addWindows( WinSize { 0, 0, 128, 128 } );
	auto wins = m_settings->getWindows();
	Serial.println( wins.size());
	for( auto i : wins ) {
		Serial.println( i.width );
	}
}