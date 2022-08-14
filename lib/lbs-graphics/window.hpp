#pragma once
#include <Adafruit_GFX.h>
#include <memory>
#include "win_settings.hpp"

namespace lbs
{
	
	class Window : public Adafruit_GFX, public WindowSettings
	{
	public:
		using SplitScreen = std::pair<Window, Window>;
		enum class SPLIT
		{
			HORIZONTAL,
			VERTICAL
		};
		
		explicit Window( WindowSize size );
		static SplitScreen createSplitScreen( float split_val, SPLIT s, u_int16_t screen_x, u_int16_t screen_y );
		~Window();
		
		// CORE DRAW API
		void drawPixel( int16_t x, int16_t y, uint16_t color ) override;
		void writePixel( int16_t x, int16_t y, uint16_t color ) override;
		void writeFillRect( int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color ) override;
		void writeFastVLine( int16_t x, int16_t y, int16_t h, uint16_t color ) override;
		void writeFastHLine( int16_t x, int16_t y, int16_t w, uint16_t color ) override;
		void writeLine( int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color ) override;
		void startWrite( void ) override;
		void endWrite( void ) override;
		
		// CONTROL API
		
		void drawFastVLine( int16_t x, int16_t y, int16_t h, uint16_t color ) override;
		void drawFastHLine( int16_t x, int16_t y, int16_t w, uint16_t color ) override;
		void fillRect( int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color ) override;
		void fillScreen( uint16_t color ) override;
		void drawLine( int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color ) override;
		void drawRect( int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color ) override;
		
		// Print API
		void printlnCentered( const char s[] );
		void printlnRight( const char s[] );
		
		size_t write( uint8_t c ) override;
		size_t write( const uint8_t* buffer, size_t size ) override;
		int availableForWrite( void ) override;
		void flush() override;
		
		// CUSTOMIZED API
		void resize( u_int16_t w, uint16_t h );
		void drawWindowBorder( Spacer padding, u_int8_t roundness, u_int16_t color, u_int8_t iterations );
		
		void setTextPadding( const Spacer& text_margin ) override;
		void setWindowOffsets( uint16_t x0, uint16_t y0 );
		uint8_t getPixel( int16_t x, int16_t y ) const;
		uint8_t* getBuffer( void ) const;
	
	protected:
		uint8_t getRawPixel( int16_t x, int16_t y ) const;
		void drawFastRawVLine( int16_t x, int16_t y, int16_t h, uint16_t color );
		void drawFastRawHLine( int16_t x, int16_t y, int16_t w, uint16_t color );
	private:
		void setRotation( uint8_t r ) override;
		
		template<typename T>
		typename std::enable_if<std::is_assignable<WindowSize, T>::value>::type
		setActiveRegion( T&& t )
		{
			m_activeRegion = std::forward<T>( t );
			cursor_x = m_activeRegion.offset_x;
			if( gfxFont )
				cursor_y = m_activeRegion.offset_y + ( textsize_y * 8 );
			else
				cursor_y = m_activeRegion.offset_y;
		}
	
	private:
		uint8_t* m_buffer;
		WindowSettings m_settings;
#ifdef __AVR__
		// Bitmask tables of 0x80>>X and ~(0x80>>X), because X>>Y is slow on AVR
	static const uint8_t PROGMEM GFXsetBit[], GFXclrBit[];
#endif
	};
	
}