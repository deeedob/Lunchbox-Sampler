#pragma once
#include <events.hpp>
#include <graphics.hpp>
#include <utility>
#include <Arduino.h>

namespace lbs
{
	class Graphics;
	
	class AbstractModule
	{
	public:
		explicit AbstractModule( String name )
			: m_moduleName( std::move( name ))
		{ };
		virtual void enter( Graphics* g ) = 0;
		virtual void update( Graphics* g, Events::DIGITAL e ) = 0;
		virtual void exit() = 0;
		
		virtual String& getModuleName()
		{ return m_moduleName; };
		virtual ~AbstractModule() = default;
	protected:
		String m_moduleName;
	};
}