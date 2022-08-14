#pragma once

namespace lbs
{
	
	struct Spacer
	{
		
		Spacer& operator+=( const Spacer& rhs )
		{
			this->xAxis += rhs.xAxis;
			this->yAxis += rhs.yAxis;
			return *this;
		}
		
		friend Spacer operator+( Spacer lhs, const Spacer& rhs )
		{
			lhs += rhs;
			return lhs;
		}
		
		int16_t xAxis { 0 };
		int16_t yAxis { 0 };
		
	};
	
	struct WindowSize
	{
		WindowSize() = default;
		
		WindowSize( uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint8_t z )
			: offset_x( x0 ), width( x1 ), offset_y( y0 ), height( y1 ), z( z )
		{ };
		
		u_int16_t getX2()
		{
			return width + offset_x;
		}
		
		u_int16_t getY2()
		{
			return height + offset_y;
		}
		
		uint16_t offset_x { 0 };
		uint16_t offset_y { 0 };
		uint16_t width { 0 };
		uint16_t height { 0 };
		u_int8_t z { 0 };
	};
	
}