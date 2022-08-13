#pragma once
#include <cstdint>
#include <vector>

namespace lbs
{
	
	class UISettings
	{
	public:
		struct WindowSize
		{
			uint16_t x0 { 0 };
			uint16_t y0 { 0 };
			uint16_t width { 0 };
			uint16_t height { 0 };
			u_int8_t z { 0 };
		};
		
		struct Spacer
		{
			int16_t xAxis { 0 };
			int16_t yAxis { 0 };
			
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
		};
		
		enum class COLORS
		{
			BACKGROUND = 0x44,
			ACTIVE = 0xff,
			PASSIVE = 0x66
		};
		using Windows = std::vector<WindowSize>;
		
		UISettings()
			: m_textSpacing(), m_textPadding()
		{ }
		
		UISettings( const UISettings& other ) = default;
		UISettings( UISettings&& other ) = default;
		UISettings& operator=( const UISettings& other ) = default;
		UISettings& operator=( UISettings&& other ) = default;
		~UISettings() = default;
		
		template<typename R, typename...>
		struct fst
		{
			typedef R type;
		};
		
		template<typename... Ts>
		typename fst<void, typename std::enable_if<std::is_convertible<Ts, WindowSize>::value>::type...>::type
		addWindows( Ts&& ... args ) noexcept
		{
			int x[] = { 0, ( m_windows.push_back( std::forward<WindowSize>( args )), 0 ) ... };
			static_cast<void>(x);
		}
		
		void addWindow( WindowSize&& win ) noexcept
		{
			m_windows.emplace_back( std::forward<WindowSize>( win ));
		}
		
		const Windows& getWindows() const noexcept
		{
			return m_windows;
		}
		
		const Spacer& getTextSpacing() const
		{
			return m_textSpacing;
		}
		
		void setTextSpacing( const Spacer& text_spacing )
		{
			m_textSpacing = text_spacing;
		}
		
		const Spacer& getTextPadding() const
		{
			return m_textPadding;
		}
		
		void setTextPadding( const Spacer& text_margin )
		{
			m_textPadding = text_margin;
		}
	
	protected:
	
	private:
		Spacer m_textSpacing;
		Spacer m_textPadding;
		Windows m_windows;
	};
	
}