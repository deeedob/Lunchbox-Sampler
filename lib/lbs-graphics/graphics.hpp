#pragma once
#include <Adafruit_SSD1327.h>
#include <vector>
#include "window.hpp"

namespace lbs
{
	class Graphics : public Adafruit_SSD1327
	{
		using Windows = std::vector<std::pair<Window, String>>;
	public:
		Graphics( u_int8_t w, u_int8_t h, TwoWire* twi = &Wire1, int8_t rst_pin = -1, u_int32_t preclk = 400000, u_int32_t postclk = 100000 );
		Graphics( const Graphics& other ) = default;
		Graphics( Graphics&& other ) = default;
		Graphics& operator=( const Graphics& other ) = default;
		Graphics& operator=( Graphics&& other ) = default;
		~Graphics() = default;
		
		void drawWindow( const Window& win );
		void drawWindow( const Window::SplitScreen& wins );
		
		void drawAll();
		void removeWindow( const String& win_name );
		
		template<typename T, typename U>
		typename std::enable_if<std::is_same<Window, T>::value &&
		                        std::is_assignable<String, U>::value>::type
		addWindow( T&& window, U&& win_name )
		{
			m_windows.push_back( std::forward<std::pair<T, U>>( { window, win_name } ));
		}
	
	private:
		Windows m_windows;
		std::shared_ptr<WindowSettings> m_settings;
	};
}