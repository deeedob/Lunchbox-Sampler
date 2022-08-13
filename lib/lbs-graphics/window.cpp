#include "window.hpp"

using namespace lbs;

Window::Window( uint16_t w, uint16_t h, uint8_t z_index )
{

}

Window::~Window()
{

}

void Window::drawPixel( int16_t x, int16_t y, uint16_t color ) noexcept
{

}

void Window::fillScreen( uint16_t color ) noexcept
{
	Adafruit_GFX::fillScreen( color );
}

void Window::drawFastVLine( int16_t x, int16_t y, int16_t h, uint16_t color ) noexcept
{
	Adafruit_GFX::drawFastVLine( x, y, h, color );
}

void Window::drawFastHLine( int16_t x, int16_t y, int16_t w, uint16_t color ) noexcept
{
	Adafruit_GFX::drawFastHLine( x, y, w, color );
}

uint8_t Window::getPixel( int16_t x, int16_t y ) const noexcept
{
	return 0;
}

uint8_t Window::getRawPixel( int16_t x, int16_t y ) const
{
	return 0;
}

void Window::drawFastRawVLine( int16_t x, int16_t y, int16_t h, uint16_t color )
{

}

void Window::drawFastRawHLine( int16_t x, int16_t y, int16_t w, uint16_t color )
{

}