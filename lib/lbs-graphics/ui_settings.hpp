#pragma once
#include <cstdint>
#include <vector>

namespace lbs
{
	
	class Settings
	{
	public:
		struct Window
		{
			uint16_t x0 { 0 };
			uint16_t y0 { 0 };
			uint16_t width { 0 };
			uint16_t height { 0 };
			u_int8_t z { 0 };
		};
		struct Spacer
		{
			uint16_t top { 0 };
			uint16_t right { 0 };
			uint16_t bottom { 0 };
			uint16_t left { 0 };
		};
		using Windows = std::vector< Window >;
		
		Settings() = default;
		Settings( const Settings& other ) = default;
		Settings( Settings&& other ) = default;
		Settings& operator=( const Settings& other ) = default;
		Settings& operator=( Settings&& other ) = default;
		~Settings() = default;
		
		template< typename R, typename... >
		struct fst
		{
			typedef R type;
		};
		
		template< typename... Ts >
		typename fst< void, typename std::enable_if< std::is_convertible< Ts, Window >::value >::type... >::type
		addWindows( Ts&& ... args ) noexcept
		{
			int x[] = { 0, ( m_windows.push_back( std::forward< Window >( args )), 0 ) ... };
			static_cast<void>(x);
		}
		
		void addWindow( Window&& win ) noexcept
		{
			m_windows.emplace_back( std::forward< Window >( win ));
		}
		
		const Windows& getWindows() const noexcept
		{
			return m_windows;
		}
	
	private:
		u_int8_t m_winCnt { 0 };
		Windows m_windows;
	};
	
}