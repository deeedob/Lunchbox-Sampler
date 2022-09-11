#pragma once
#include <Arduino.h>
#include <events.hpp>
#include <graphics.hpp>
#include <observer.hpp>
#include <transition_table.h>
#include <utility>

namespace lbs
{
	class Graphics;

	class AbstractModule : public Observable< AbstractModule >
	{
	public:
		explicit AbstractModule( String name ) : m_moduleName( std::move( name ) ) {};
		virtual void enter( Graphics* g ) = 0;
		virtual void update( Graphics* g, Events::DIGITAL e ) = 0;

		virtual void exit() { notify( *this ); }

		virtual String& getModuleName() { return m_moduleName; };

		virtual ~AbstractModule() = default;

	protected:
		String m_moduleName;
	};
}// namespace lbs