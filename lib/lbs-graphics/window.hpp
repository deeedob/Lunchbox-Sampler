#pragma once
#include <Adafruit_GFX.h>
#include <memory>
#include "ui_settings.hpp"

namespace lbs
{
	
	class Window : public Adafruit_GFX
	{
		using SpSettings = std::shared_ptr<UISettings>;
	public:
		explicit Window( UISettings::WindowSize size );
		~Window();
		
		// CORE DRAW API
		void drawPixel( int16_t x, int16_t y, uint16_t color ) override;
		void startWrite( void ) override;
		void writePixel( int16_t x, int16_t y, uint16_t color ) override;
		void writeFillRect( int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color ) override;
		void writeFastVLine( int16_t x, int16_t y, int16_t h, uint16_t color ) override;
		void writeFastHLine( int16_t x, int16_t y, int16_t w, uint16_t color ) override;
		void writeLine( int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color ) override;
		void endWrite( void ) override;
		
		// CONTROL API
		void setRotation( uint8_t r ) override;
		
		void drawFastVLine( int16_t x, int16_t y, int16_t h, uint16_t color ) override;
		void drawFastHLine( int16_t x, int16_t y, int16_t w, uint16_t color ) override;
		void fillRect( int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color ) override;
		void fillScreen( uint16_t color ) override;
		void drawLine( int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color ) override;
		void drawRect( int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color ) override;
		
		// Print API
		void printlnCentered( const char s[] );
		size_t write( uint8_t c ) override;
		size_t write( const uint8_t* buffer, size_t size ) override;
		int availableForWrite( void ) override;
		void flush() override;
		
		// CUSTOMIZED API
		void resize( u_int16_t w, uint16_t h );
		void drawWindowBorder( UISettings::Spacer padding, u_int8_t roundness, u_int16_t color, u_int8_t iterations );
		
		const UISettings::Spacer& getTextSpacing() const;
		void setTextSpacing( const UISettings::Spacer& text_spacing );
		const UISettings::Spacer& getTextPadding() const;
		void setTextPadding( const UISettings::Spacer& text_padding );
		const UISettings::WindowSize& getWindowSize() const;
		void setWindowOffsets( uint16_t x0, uint16_t y0 ) noexcept;
		uint8_t getPixel( int16_t x, int16_t y ) const;
		uint8_t* getBuffer( void ) const;
	
	protected:
		void setWindowSize( const UISettings::WindowSize& window_size );
		uint8_t getRawPixel( int16_t x, int16_t y ) const;
		void drawFastRawVLine( int16_t x, int16_t y, int16_t h, uint16_t color );
		void drawFastRawHLine( int16_t x, int16_t y, int16_t w, uint16_t color );
	
	private:
		uint8_t* m_buffer;
		UISettings::WindowSize m_windowSize;
		UISettings::Spacer m_textSpacing;
		UISettings::Spacer m_textPadding;
		UISettings::Spacer m_paddedWindow;

#ifdef __AVR__
		// Bitmask tables of 0x80>>X and ~(0x80>>X), because X>>Y is slow on AVR
	static const uint8_t PROGMEM GFXsetBit[], GFXclrBit[];
#endif
	};
	
}