#pragma once
#include <cstdint>
#include <vector>

class Settings
{
public:
	struct WinSize
	{
		uint16_t x0;
		uint16_t y0;
		uint16_t width;
		uint16_t height;
	};
	using Windows = std::vector<WinSize>;
	Settings();
	Settings( const Settings& other ) = default;
	Settings( Settings&& other ) = default;
	Settings& operator=( const Settings& other ) = default;
	Settings& operator=( Settings&& other ) = default;
	~Settings() = default;
	
	template<typename... Ts>
	void addWindows( Ts&& ... args );
	Windows getWindows() const noexcept;

private:
	Windows m_windows;
};

template<typename... Ts>
void Settings::addWindows( Ts&& ... args )
{
	int x[] = { 0, ( m_windows.push_back( args ), 0 ) ... };
	static_cast<void>(x);
}

Settings::Windows Settings::getWindows() const noexcept
{
	return m_windows;
}