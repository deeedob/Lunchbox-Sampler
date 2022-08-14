#include "graphics.hpp"
#include <define_t40.hpp>

using namespace lbs;

Graphics::Graphics( u_int8_t w, u_int8_t h, TwoWire* twi, int8_t rst_pin, u_int32_t preclk, u_int32_t postclk )
	: Adafruit_SSD1327( w, h, twi, rst_pin, preclk, postclk )
{
	pinMode( C_DISPLAY_RST, OUTPUT );
	
	while ( !begin( 0x3d )) {
		yield();
	}
	
	window_x1 = 0;
	window_y1 = 0;
	window_x2 = 127;
	window_y2 = 127;
	
	clearDisplay();
	Adafruit_SSD1327::display();
	
}

void Graphics::drawWindow( const Window& win )
{
	const auto size = win.getWindowSize();
	int16_t y = size.offset_y;
	for( int16_t j = 0; j < size.height; j++, y++ ) {
		for( int16_t i = 0; i < size.width; i++ ) {
			this->writePixel( size.offset_x + i, y, win.getBuffer()[ j * size.width + i ] );
		}
	}
}

void Graphics::drawWindow( const Window::SplitScreen& wins )
{
	drawWindow( wins.first );
	drawWindow( wins.second );
}

void Graphics::drawAll()
{
	for( const auto& item : m_windows ) {
		drawWindow( item.first );
	}
}

void Graphics::removeWindow( const String& win_name )
{
	for( int i = 0; i < m_windows.size(); i++ ) {
		if( m_windows[ i ].second.equals( win_name ))
			m_windows.erase( m_windows.begin() + i );
	}
}