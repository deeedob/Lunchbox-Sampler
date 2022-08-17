#pragma once
#include <Adafruit_SSD1327.h>

class Graphics : public Adafruit_SSD1327
{
public:
	Graphics( u_int8_t w, u_int8_t h, TwoWire* twi = &Wire1,
	          int8_t rst_pin = -1, u_int32_t preclk = 400000,
	          u_int32_t postclk = 100000 );
	Graphics( const Graphics& other ) = default;
	Graphics( Graphics&& other ) = default;
	Graphics& operator=( const Graphics& other ) = default;
	Graphics& operator=( Graphics&& other ) = default;
	~Graphics() = default;
private:
};