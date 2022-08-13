#pragma once
#include <Adafruit_SSD1327.h>
#include "window.hpp"
#include <vector>

namespace lbs
{
	
	class Graphics : public Adafruit_SSD1327
	{
		using Windows = std::vector< Window >;
	public:
		Graphics( u_int8_t w, u_int8_t h, TwoWire* twi = &Wire1, int8_t rst_pin = -1, u_int32_t preclk = 400000, u_int32_t postclk = 100000 );
		Graphics( const Graphics& other ) = default;
		Graphics( Graphics&& other ) = default;
		Graphics& operator=( const Graphics& other ) = default;
		Graphics& operator=( Graphics&& other ) = default;
		~Graphics() = default;
		
		void drawPixelFast( u_int8_t x, u_int8_t y, uint8_t color ) noexcept;
	
	private:
		Windows m_windows;
	};
}