#pragma once
#include <Adafruit_GFX.h>

namespace lbs
{
	
	class Window : Adafruit_GFX
	{
	
	public:
		explicit Window( uint16_t w = 0, uint16_t h = 0, uint8_t z_index = 0 );
		~Window();
		void drawPixel( int16_t x, int16_t y, uint16_t color ) noexcept override;
		void fillScreen( uint16_t color ) noexcept override;
		void drawFastVLine( int16_t x, int16_t y, int16_t h, uint16_t color ) noexcept override;
		void drawFastHLine( int16_t x, int16_t y, int16_t w, uint16_t color ) noexcept override;
		
		uint8_t getPixel( int16_t x, int16_t y ) const noexcept;
		
		uint8_t* getBuffer() const noexcept
		{ return m_buffer; }
	
	protected:
		uint8_t getRawPixel( int16_t x, int16_t y ) const;
		void drawFastRawVLine( int16_t x, int16_t y, int16_t h, uint16_t color );
		void drawFastRawHLine( int16_t x, int16_t y, int16_t w, uint16_t color );
	private:
		uint8_t* m_buffer;
		uint8_t m_zIndex;
	};
	
}