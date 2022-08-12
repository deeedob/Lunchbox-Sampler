#include "graphics.hpp"

Graphics::Graphics( u_int8_t w, u_int8_t h, TwoWire* twi,
                    int8_t rst_pin, u_int32_t preclk, u_int32_t postclk )
	: Adafruit_SSD1327( w, h, twi, rst_pin, preclk, postclk )
{ }