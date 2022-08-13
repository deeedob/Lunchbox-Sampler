#include "window.hpp"

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

Window::Window( UISettings::WindowSize size )
	: Adafruit_GFX( size.width, size.height )
{
	uint32_t bytes = size.width * size.height;
	if( bytes > 0 ) {
		if(( m_buffer = (uint8_t*) malloc( bytes ))) {
			memset( m_buffer, 0, bytes );
		}
		cursor_y = textsize_y * 8;
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

void Window::flush()
{
	Print::flush();
}

void Window::drawPixel( int16_t x, int16_t y, uint16_t color )
{
	if( !m_buffer )
		return;
	
	if(( x < 0 ) || ( y < 0 ) || ( x >= _width ) || ( y >= _height ))
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
	UISettings::Spacer pad = m_textPadding + m_paddedWindow;
	UISettings::Spacer spacing = m_textSpacing;
	if( !gfxFont ) { // 'Classic' built-in font
		if( c == '\n' ) {              // Newline?
			cursor_x = pad.xAxis;               // Reset x to zero,
			cursor_y += ( textsize_y * 8 ) + spacing.yAxis; // advance y one line
		} else if( c != '\r' ) {       // Ignore carriage returns
			if( wrap && (( cursor_x + textsize_x * 6 ) > ( _width - pad.xAxis ))) { // Off right?
				cursor_x = pad.xAxis;                                       // Reset x to zero,
				cursor_y += ( textsize_y * 8 ) + spacing.yAxis; // advance y one line
			}
			if( cursor_y + ( textsize_y * 8 ) > _height - pad.yAxis )
				return 1;
			drawChar( cursor_x, cursor_y, c, textcolor, textbgcolor, textsize_x,
			          textsize_y );
			cursor_x += ( textsize_x * 6 ) + spacing.xAxis; // Advance x one char
		}
		
	} else { // Custom font
		if( c == '\n' ) {
			cursor_x = pad.xAxis;
			cursor_y +=
				((int16_t) textsize_y * (uint8_t) pgm_read_byte( &gfxFont->yAdvance )) + spacing.yAxis;
		} else if( c != '\r' ) {
			uint8_t first = pgm_read_byte( &gfxFont->first );
			if(( c >= first ) && ( c <= (uint8_t) pgm_read_byte( &gfxFont->last ))) {
				GFXglyph* glyph = pgm_read_glyph_ptr( gfxFont, c - first );
				uint8_t w = pgm_read_byte( &glyph->width ),
					h = pgm_read_byte( &glyph->height );
				if(( w > 0 ) && ( h > 0 )) { // Is there an associated bitmap?
					int16_t xo = (int8_t) pgm_read_byte( &glyph->xOffset ); // sic
					if( wrap && (( cursor_x + textsize_x * ( xo + w )) > _width - pad.xAxis )) {
						cursor_x = pad.xAxis;
						cursor_y += ((int16_t) textsize_y * (uint8_t) pgm_read_byte( &gfxFont->yAdvance )) + spacing.yAxis;
					}
					if( cursor_y + ( textsize_y * 8 ) > _height - pad.yAxis )
						return 1;
					drawChar( cursor_x, cursor_y, c, textcolor, textbgcolor, textsize_x,
					          textsize_y );
				}
				cursor_x += ((uint8_t) pgm_read_byte( &glyph->xAdvance ) * (int16_t) textsize_x ) + spacing.xAxis;
			}
		}
	}
	return 1;
}

void Window::resize( u_int16_t w, uint16_t h )
{
	uint32_t bytes = w * h;
	if( bytes > 0 ) {
		if( m_buffer ) {
			free( m_buffer );
		}
		
		if(( m_buffer = (uint8_t*) malloc( bytes ))) {
			memset( m_buffer, 0, bytes );
			
			WIDTH = w;
			HEIGHT = h;
			_width = WIDTH;
			_height = HEIGHT;
			textsize_x = textsize_y = 1;
			cursor_x = 0;
			cursor_y = textsize_y * 8;
			m_windowSize.width = w;
			m_windowSize.height = h;
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

void Window::printlnCentered( const char* s )
{
	auto temp = m_textPadding;
	m_textPadding = { 0, temp.yAxis };
	int16_t x1, y1;
	uint16_t w, h;
	getTextBounds( s, getCursorX(), getCursorY(), &x1, &y1, &w, &h );
	int16_t cent_screen = width() / 2;
	int16_t cent_box = w / 2;
	setCursor( cent_screen - cent_box, getCursorY());
	println( s );
	m_textPadding = temp;
}

void Window::drawWindowBorder( UISettings::Spacer padding, u_int8_t roundness, u_int16_t color, u_int8_t iterations )
{
	u_int8_t x_0, y_0, x_1, y_1;
	
	for( int i = 0; i < iterations; i++ ) {
		x_0 = m_windowSize.x0 + padding.xAxis;
		y_0 = m_windowSize.y0 + padding.yAxis;
		x_1 = m_windowSize.width - ( 2 * padding.xAxis );
		y_1 = m_windowSize.height - ( 2 * padding.yAxis );
		Serial.print( "padx:" );
		Serial.println( padding.xAxis );
		Serial.print( "width:" );
		Serial.println( m_windowSize.width );
		Serial.print( "x0:" );
		Serial.println( x_0 );
		Serial.print( "y0:" );
		Serial.println( y_0 );
		Serial.print( "x1:" );
		Serial.println( x_1 );
		Serial.print( "y1:" );
		Serial.println( y_1 );
		drawRoundRect( x_0, y_0, x_1, y_1, roundness, color );
		padding += padding;
	}
	m_paddedWindow = padding;
	m_paddedWindow.xAxis -= 4;
	m_paddedWindow.yAxis -= 4;
	setCursor( getCursorX() + m_paddedWindow.xAxis, getCursorY() + m_paddedWindow.yAxis );
}

const UISettings::WindowSize& Window::getWindowSize() const
{
	return m_windowSize;
}

void Window::setWindowSize( const UISettings::WindowSize& window_size )
{
	m_windowSize = window_size;
}

void Window::setWindowOffsets( uint16_t x0, uint16_t y0 ) noexcept
{
	m_windowSize.x0 = x0;
	m_windowSize.y0 = y0;
}

const UISettings::Spacer& Window::getTextSpacing() const
{
	return m_textSpacing;
}

void Window::setTextSpacing( const UISettings::Spacer& text_spacing )
{
	m_textSpacing = text_spacing;
}

const UISettings::Spacer& Window::getTextPadding() const
{
	return m_textPadding;
}

void Window::setTextPadding( const UISettings::Spacer& text_padding )
{
	m_textPadding = text_padding;
}