#pragma once

namespace lbs
{
	
	enum class UTIL
	{
		HORIZONTAL,
		VERTICAL
	};
	
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
			: offset_x( x0 ), offset_y( y0 ), width( x1 ), height( y1 ), z( z )
		{ };
		
		static std::pair<WindowSize, WindowSize> createSplitScreen( UTIL split, float split_factor = 0.5f, uint16_t max_screen_x = 128, uint16_t max_screen_y = 128 )
		{
			
			if( split_factor < 0 || split_factor > 1.0 ) { split_factor = 0.5; }
			--max_screen_x;
			--max_screen_y;
			
			if( split == UTIL::HORIZONTAL ) {
				auto splitter = static_cast<u_int16_t>((float) max_screen_y * split_factor);
				WindowSize top( 0, 0, max_screen_x, splitter, 0 );
				WindowSize bottom( 0, splitter, max_screen_x, max_screen_y - splitter, 0 );
				
				return { top, bottom };
			}
			if( split == UTIL::VERTICAL ) {
				auto splitter = static_cast<u_int16_t>((float) max_screen_x * split_factor);
				WindowSize left( 0, 0, splitter, max_screen_y, 0 );
				WindowSize right( splitter, 0, max_screen_x - splitter, max_screen_y, 0 );
				return { left, right };
			}
			
			auto splitter = static_cast<u_int16_t>((float) max_screen_x * split_factor);
			WindowSize left( 0, 0, splitter, max_screen_y, 0 );
			WindowSize right( splitter, 0, max_screen_x - splitter, max_screen_y, 0 );
			return { left, right };
		}
		
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
	
	struct TextSize
	{
		uint8_t text_x { 0 };
		uint8_t text_y { 0 };
	};
	
}