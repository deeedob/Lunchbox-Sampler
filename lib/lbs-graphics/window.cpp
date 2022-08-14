#include "window.hpp"
#include "fonts/MLReg_5pt.h"

using namespace lbs;

#ifndef _swap_int16_t
#define _swap_int16_t( a, b )                                                    \
  {                                                                            \
    int16_t t = a;                                                             \
    a = b;                                                                     \
    b = t;                                                                     \
  }
#endif

inline GFXglyph* pgm_read_glyph_ptr( const GFXfont* gfxFont, uint8_t c )
{
#ifdef __AVR__
	return &(((GFXglyph *)pgm_read_pointer(&gfxFont->glyph))[c]);
#else
	// expression in __AVR__ section may generate "dereferencing type-punned
	// pointer will break strict-aliasing rules" warning In fact, on other
	// platforms (such as STM32) there is no need to do this pointer magic as
	// program memory may be read in a usual way So expression may be simplified
	return gfxFont->glyph + c;
#endif //__AVR__
}

Window::Window( WindowSize size )
	: Adafruit_GFX( size.width, size.height ), WindowSettings()
{
	m_windowSize = size;
	uint32_t bytes = size.getX2() * size.getY2();
	if( bytes > 0 ) {
		if(( m_buffer = (uint8_t*) malloc( bytes ))) {
			memset( m_buffer, 0, bytes );
			setActiveRegion( size );
			setFont( &MLReg5pt7b );
			setTextSpacing( { 0, -2 } );
			setTextColor( 0x00 );
		}
	}
}

Window::~Window()
{
	if( m_buffer )
		free( m_buffer );
}

size_t Window::write( const uint8_t* buffer, size_t size )
{
	if( buffer == nullptr ) return 0;
	size_t count = 0;
	while ( size-- ) { count += write( *buffer++ ); }
	return count;
}

// TODO implement?
void Window::flush()
{
	Print::flush();
}

void Window::drawPixel( int16_t x, int16_t y, uint16_t color )
{
	if( !m_buffer )
		return;
	
	if(( x < 0 ) || ( y < 0 ) || ( x >= m_windowSize.width ) || ( y >= m_windowSize.height ))
		return;
	
	m_buffer[ x + y * WIDTH ] = color;
}

/* start a routine! */
void Window::startWrite( void )
{ }

void Window::writePixel( int16_t x, int16_t y, uint16_t color )
{
	drawPixel( x, y, color );
}

void Window::writeFillRect( int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color )
{
	fillRect( x, y, w, h, color );
}

void Window::writeFastVLine( int16_t x, int16_t y, int16_t h, uint16_t color )
{
	drawFastVLine( x, y, h, color );
}

void Window::writeFastHLine( int16_t x, int16_t y, int16_t w, uint16_t color )
{
	drawFastHLine( x, y, w, color );
}

void Window::writeLine( int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color )
{
	int16_t steep = abs( y1 - y0 ) > abs( x1 - x0 );
	if( steep ) {
		_swap_int16_t( x0, y0 );
		_swap_int16_t( x1, y1 );
	}
	
	if( x0 > x1 ) {
		_swap_int16_t( x0, x1 );
		_swap_int16_t( y0, y1 );
	}
	
	int16_t dx, dy;
	dx = x1 - x0;
	dy = abs( y1 - y0 );
	
	int16_t err = dx / 2;
	int16_t ystep;
	
	if( y0 < y1 ) {
		ystep = 1;
	} else {
		ystep = -1;
	}
	
	for( ; x0 <= x1; x0++ ) {
		if( steep ) {
			writePixel( y0, x0, color );
		} else {
			writePixel( x0, y0, color );
		}
		err -= dy;
		if( err < 0 ) {
			y0 += ystep;
			err += dx;
		}
	}
}

void Window::endWrite( void )
{
}

void Window::setRotation( uint8_t r )
{
	rotation = 0;
}

void Window::drawFastVLine( int16_t x, int16_t y, int16_t h, uint16_t color )
{
	if( h < 0 ) { // Convert negative heights to positive equivalent
		h *= -1;
		y -= h - 1;
		if( y < 0 ) {
			h += y;
			y = 0;
		}
	}
	
	// Edge rejection (no-draw if totally off canvas)
	if(( x < 0 ) || ( x >= width()) || ( y >= height()) || (( y + h - 1 ) < 0 )) {
		return;
	}
	
	if( y < 0 ) { // Clip top
		h += y;
		y = 0;
	}
	if( y + h > height()) { // Clip bottom
		h = height() - y;
	}
	
	drawFastRawVLine( x, y, h, color );
	
}

void Window::drawFastHLine( int16_t x, int16_t y, int16_t w, uint16_t color )
{
	if( w < 0 ) { // Convert negative widths to positive equivalent
		w *= -1;
		x -= w - 1;
		if( x < 0 ) {
			w += x;
			x = 0;
		}
	}
	
	// Edge rejection (no-draw if totally off canvas)
	if(( y < 0 ) || ( y >= height()) || ( x >= width()) || (( x + w - 1 ) < 0 )) {
		return;
	}
	
	if( x < 0 ) { // Clip left
		w += x;
		x = 0;
	}
	if( x + w >= width()) { // Clip right
		w = width() - x;
	}
	
	drawFastRawHLine( x, y, w, color );
}

void Window::fillRect( int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color )
{
	startWrite();
	for( int16_t i = x; i < x + w; i++ ) {
		writeFastVLine( i, y, h, color );
	}
	endWrite();
}

void Window::fillScreen( uint16_t color )
{
	if( m_buffer ) {
		memset( m_buffer, color, WIDTH * HEIGHT );
	}
}

void Window::drawLine( int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color )
{
	// Update in subclasses if desired!
	if( x0 == x1 ) {
		if( y0 > y1 ) _swap_int16_t( y0, y1 );
		drawFastVLine( x0, y0, y1 - y0 + 1, color );
	} else if( y0 == y1 ) {
		if( x0 > x1 ) _swap_int16_t( x0, x1 );
		drawFastHLine( x0, y0, x1 - x0 + 1, color );
	} else {
		startWrite();
		writeLine( x0, y0, x1, y1, color );
		endWrite();
	}
}

void Window::drawRect( int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color )
{
	startWrite();
	writeFastHLine( x, y, w, color );
	writeFastHLine( x, y + h - 1, w, color );
	writeFastVLine( x, y, h, color );
	writeFastVLine( x + w - 1, y, h, color );
	endWrite();
}

/* print / println -> write(u_int8, u_int8) -> write(u_int8) */
size_t Window::write( uint8_t c )
{
	if( !gfxFont ) { // 'Classic' built-in font
		if( c == '\n' ) {              // Newline?
			cursor_x = m_activeRegion.offset_x + m_textPadding.xAxis;               // Reset x to zero,
			cursor_y += ( textsize_y * 8 ) + m_textSpacing.yAxis; // advance y one line
		} else if( c != '\r' ) {       // Ignore carriage returns
			if( wrap && (( cursor_x + textsize_x * 6 ) > ( m_activeRegion.getX2() - m_textPadding.xAxis ))) { // Off right?
				cursor_x = m_activeRegion.offset_x + m_textPadding.xAxis; // Reset x to zero,
				cursor_y += ( textsize_y * 8 ) + m_textSpacing.yAxis; // advance y one line
			}
			if( cursor_y + ( textsize_y * 8 ) > ( m_activeRegion.getY2() - m_textPadding.yAxis ))
				return 1;
			drawChar( cursor_x, cursor_y, c, textcolor, textbgcolor, textsize_x,
			          textsize_y );
			cursor_x += ( textsize_x * 6 ) + m_textSpacing.xAxis; // Advance x one char
		}
	} else { // Custom font
		if( c == '\n' ) {
			cursor_x = m_activeRegion.offset_x + m_textPadding.xAxis;
			cursor_y += ((int16_t) textsize_y * (uint8_t) pgm_read_byte( &gfxFont->yAdvance )) + m_textSpacing.yAxis;
		} else if( c != '\r' ) {
			uint8_t first = pgm_read_byte( &gfxFont->first );
			if(( c >= first ) && ( c <= (uint8_t) pgm_read_byte( &gfxFont->last ))) {
				GFXglyph* glyph = pgm_read_glyph_ptr( gfxFont, c - first );
				uint8_t w = pgm_read_byte( &glyph->width ),
					h = pgm_read_byte( &glyph->height );
				if(( w > 0 ) && ( h > 0 )) { // Is there an associated bitmap?
					int16_t xo = (int8_t) pgm_read_byte( &glyph->xOffset ); // sic
					if( wrap && (( cursor_x + textsize_x * ( xo + w )) > ( m_activeRegion.getX2() - m_textPadding.xAxis ))) {
						cursor_x = m_activeRegion.offset_x + m_textPadding.xAxis;
						cursor_y += ((int16_t) textsize_y * (uint8_t) pgm_read_byte( &gfxFont->yAdvance )) + m_textSpacing.yAxis;
					}
					if( cursor_y + ( textsize_y * 8 ) > ( m_activeRegion.getY2() - m_textPadding.yAxis ))
						return 1;
					drawChar( cursor_x, cursor_y, c, textcolor, textbgcolor, textsize_x,
					          textsize_y );
				}
				cursor_x += ((uint8_t) pgm_read_byte( &glyph->xAdvance ) * (int16_t) textsize_x ) + m_textSpacing.xAxis;
			}
		}
	}
	return 1;
}

void Window::printlnCentered( const char* s )
{
	auto temp = m_textPadding;
	m_textPadding = { 0, temp.yAxis };
	int16_t x1, y1;
	uint16_t w, h;
	getTextBounds( s, getCursorX(), getCursorY(), &x1, &y1, &w, &h );
	int16_t cent_screen = ( getActiveRegion().width / 2 ) + getActiveRegion().offset_x;
	int16_t cent_box = w / 2;
	auto offset = cent_screen - cent_box;
	setCursor( offset, getCursorY());
	println( s );
	m_textPadding = temp;
}

void Window::printlnRight( const char* s )
{
	int16_t x1, y1;
	uint16_t w, h;
	getTextBounds( s, getCursorX(), getCursorY(), &x1, &y1, &w, &h );
	
	w += m_textPadding.xAxis;
	if( gfxFont ) {
		w += 6;
	}
	setCursor( m_activeRegion.getX2() - w, getCursorY());
	println( s );
}

void Window::resize( u_int16_t w, uint16_t h )
{
	uint32_t bytes = w * h;
	if( bytes > 0 ) {
		if( m_buffer ) {
			// reset old window to 0
			memset( m_buffer, 0, WIDTH * HEIGHT );
			free( m_buffer );
		}
		
		if(( m_buffer = (uint8_t*) malloc( bytes ))) {
			memset( m_buffer, 0, bytes );
			WIDTH = w;
			HEIGHT = w;
			// TODO: dont use _width / _height anymore?
			_width = WIDTH;
			_height = HEIGHT;
			m_windowSize.width = w;
			m_windowSize.height = h;
			setActiveRegion( m_windowSize );
		}
	}
}

uint8_t Window::getPixel( int16_t x, int16_t y ) const
{
	return getRawPixel( x, y );
}

uint8_t Window::getRawPixel( int16_t x, int16_t y ) const
{
	if(( x < 0 ) || ( y < 0 ) || ( x >= WIDTH ) || ( y >= HEIGHT ))
		return 0;
	if( m_buffer ) {
		return m_buffer[ x + y * WIDTH ];
	}
	return 0;
}

void Window::drawFastRawVLine( int16_t x, int16_t y, int16_t h, uint16_t color )
{
	// x & y already in raw (rotation 0) coordinates, no need to transform.
	uint8_t* buffer_ptr = m_buffer + y * WIDTH + x;
	for( int16_t i = 0; i < h; i++ ) {
		( *buffer_ptr ) = color;
		buffer_ptr += WIDTH;
	}
}

void Window::drawFastRawHLine( int16_t x, int16_t y, int16_t w, uint16_t color )
{
	// x & y already in raw (rotation 0) coordinates, no need to transform.
	memset( m_buffer + y * WIDTH + x, color, w );
}

uint8_t* Window::getBuffer( void ) const
{
	return m_buffer;
}

int Window::availableForWrite( void )
{
	return Print::availableForWrite();
}

void Window::drawWindowBorder( Spacer padding, u_int8_t roundness, u_int16_t color, u_int8_t iterations )
{
	u_int8_t x_0, y_0, w, h;
	auto glob_pad = padding;
	for( int i = 0; i < iterations; i++ ) {
		x_0 = padding.xAxis;
		y_0 = padding.yAxis;
		w = m_windowSize.width - ( 2 * padding.xAxis );
		h = m_windowSize.height - ( 2 * padding.yAxis );
		if( roundness <= 0 )
			drawRect( x_0, y_0, w, h, color );
		else
			drawRoundRect( x_0, y_0, w, h, roundness, color );
		padding += glob_pad;
	}
	setActiveRegion( WindowSize { x_0, y_0, w, h, 0 } );
}

void Window::setWindowOffsets( uint16_t x0, uint16_t y0 ) noexcept
{
	m_windowSize.offset_x = x0;
	m_windowSize.offset_y = y0;
}

void Window::setTextPadding( const Spacer& text_padding )
{
	m_textPadding = text_padding;
	setCursor( getCursorX() + m_textPadding.xAxis, getCursorY() + m_textPadding.yAxis );
}

Window::SplitScreen Window::createSplitScreen( float split_val, SPLIT s, u_int16_t screen_x, u_int16_t screen_y )
{
	if( split_val < 0 || split_val > 1.0 ) { split_val = 0.5; }
	--screen_x;
	--screen_y;
	
	if( s == SPLIT::HORIZONTAL ) {
		auto split = static_cast<u_int16_t>((float) screen_y * split_val);
		Window top( WindowSize( 0, 0, screen_x, split, 0 ));
		Window bottom( WindowSize( 0, split, screen_x, split, 0 ));
		return { top, bottom };
	}
	if( s == SPLIT::VERTICAL ) {
		auto split = static_cast<u_int16_t>((float) screen_x * split_val);
		Window left( WindowSize( 0, 0, split, screen_y, 0 ));
		Window right( WindowSize( split, 0, screen_x - split, screen_y, 0 ));
		return { left, right };
	}
	
}