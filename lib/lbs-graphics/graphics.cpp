#include "graphics.hpp"
#include "fonts/MLReg_5pt.h"
#include <define_t40.hpp>
#include <string>

using namespace lbs;

Graphics::Graphics( u_int8_t w, u_int8_t h, TwoWire* twi, int8_t rst_pin, u_int32_t preclk, u_int32_t postclk )
	: Adafruit_SSD1327( w, h, twi, rst_pin, preclk, postclk )
{
	pinMode( C_DISPLAY_RST, OUTPUT );
	
	while( !begin( 0x3d )) {
		yield();
	}
	clearDisplay();
	Adafruit_SSD1327::display();
	
	setFont( &MLReg5pt7b );
	GFXcanvas1 canvas( 64, 64 );
	canvas.println( "Mono Lisa is a very nice Font! - . , + # * ' # ! § $ % & / ( ) = ? ` ´ " );
	drawBitmap( 63, 20, canvas.getBuffer(), 64, 64, 0xff, 0x55 );
	
	//String msg( "Mono Lisa is a very nice Font! - . , + # * ' # ! § $ % & / ( ) = ? ` ´ " );
	//int16_t x1, y1;
	//uint16_t width, height;
	//getTextBounds( msg, 30, 30, &x1, &y1, &width, &height );
	//setTextSize( 1 );
	//Adafruit_SSD1327::println( "Mono Lisa is a nice Font!" );
	
	window_x1 = 0;
	window_y1 = 0;
	window_x2 = 127;
	window_y2 = 127;
	Adafruit_SSD1327::display();
}

void Graphics::drawPixelFast( u_int8_t x, u_int8_t y, uint8_t color ) noexcept
{
	if( x > width() || y > height())
		return;
	// adjust dirty window
	window_x1 = min( window_x1, x );
	window_y1 = min( window_y1, y );
	window_x2 = max( window_x2, x );
	window_y2 = max( window_y2, y );
	
	uint8_t* pixelptr = &buffer[ x / 2 + ( y * WIDTH / 2 ) ];
	if( x % 2 == 0 ) { // even, left nibble
		uint8_t t = pixelptr[ 0 ] & 0x0F;
		t |= ( color & 0xF ) << 4;
		pixelptr[ 0 ] = t;
	} else { // odd, right lower nibble
		uint8_t t = pixelptr[ 0 ] & 0xF0;
		t |= color & 0xF;
		pixelptr[ 0 ] = t;
	}
}