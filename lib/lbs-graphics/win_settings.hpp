#pragma once
#include <cstdint>
#include <vector>
#include "util.hpp"

namespace lbs
{
	class WindowSettings
	{
	public:
		
		enum class COLORS
		{
			BACKGROUND = 0x44,
			ACTIVE = 0xff,
			PASSIVE = 0x66
		};
		//using Windows = std::vector<WindowSize>;
		
		WindowSettings()
			: m_textSpacing(), m_textPadding(), m_textSize()
		{ }
		
		WindowSettings( const WindowSettings& other ) = default;
		WindowSettings( WindowSettings&& other ) = default;
		WindowSettings& operator=( const WindowSettings& other ) = default;
		WindowSettings& operator=( WindowSettings&& other ) = default;
		~WindowSettings() = default;
		
		/*
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
		*/
		virtual const Spacer& getTextSpacing() const
		{
			return m_textSpacing;
		}
		
		virtual void setTextSpacing( const Spacer& text_spacing )
		{
			m_textSpacing = text_spacing;
		}
		
		virtual const Spacer& getTextPadding() const
		{
			return m_textPadding;
		}
		
		virtual void setTextPadding( const Spacer& text_margin )
		{
			m_textPadding = text_margin;
		}
		
		virtual const WindowSize& getWindowSize() const
		{
			return m_windowSize;
		}
		
		virtual const WindowSize& getActiveRegion() const
		{
			return m_activeRegion;
		}
	
	protected:
		Spacer m_textSpacing;
		Spacer m_textPadding;
		WindowSize m_windowSize;
		WindowSize m_activeRegion;
		TextSize m_textSize;
	};
	
}