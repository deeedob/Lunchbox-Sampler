#include "graphics.hpp"
#include "fonts/MLReg_5pt.h"
#include <define_t40.hpp>
#include <string>

using namespace lbs;

Graphics::Graphics( u_int8_t w, u_int8_t h, TwoWire* twi, int8_t rst_pin, u_int32_t preclk, u_int32_t postclk )
	: Adafruit_SSD1327( w, h, twi, rst_pin, preclk, postclk )
{
	pinMode( C_DISPLAY_RST, OUTPUT );
	
	while ( !begin( 0x3d )) {
		yield();
	}
	
	clearDisplay();
	Adafruit_SSD1327::display();
	
	m_settings = std::make_shared<UISettings>();
	
	Window canvas( UISettings::WindowSize( { 0, 0, 128, 128 } ));
	canvas.setFont( &MLReg5pt7b );
	
	//m_settings->setTextPadding( { 15, 0 } );
	//m_settings->setTextSpacing( { 2, 2 } );
	
	for( int y = 0; y < canvas.height(); y++ ) {
		for( int x = 0; x < canvas.width(); x++ ) {
			canvas.drawPixel( x, y, 0xff );
		}
	}
	canvas.setTextWrap( true );
	canvas.setTextColor( 0x00 );
	canvas.println( "Mono Lisa is a very nice Font!" );
	canvas.fillScreen( 0x00 );
	
	canvas.resize( 128, 128 );
	canvas.fillScreen( 0xff );
	canvas.drawWindowBorder( { 4, 4 }, 4, 0x00, 2 );
	canvas.println( "qwerzweiurzwioerskjdahkjsasdfsdskdalhskjdhfksljadhksjdhfksajldhklsajdhksjdahksldjfhiukfhsdjkfhskaldjhfdskfjghdklfjghkldsfjhlkdsjfhkdsjfhgkljdsfhgkljdshfgkjhdsfklgjhdskjfhkdsjfhgkljdsfhgkjdhsfkgjhdkjfgldkfögkldsfgiweoprtuerDSFGDSFGDFSdhfxymcnvbmyxnb" );
	//canvas.setCursor( m_settings->getTextPadding().xAxis, m_settings->getTextPadding().yAxis + 10 );
	//canvas.printlnCentered( "Item 1" );
	//canvas.printlnCentered( "a" );
	//canvas.printlnCentered( "31" );
	//canvas.printlnCentered( "ABC" );
	
	//drawGrayscaleBitmap( 0, 0, canvas.getBuffer(), canvas.width(), canvas.height());
	canvas.setWindowOffsets( 10, 10 );
	drawWindow( canvas );
	
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

void Graphics::drawWindow( Window& win )
{
	const auto size = win.getWindowSize();
	int16_t y = size.y0;
	for( int16_t j = 0; j < size.height; j++, y++ ) {
		for( int16_t i = 0; i < size.width; i++ ) {
			writePixel( size.x0 + i, y, win.getBuffer()[ j * size.width + i ] );
		}
	}
}