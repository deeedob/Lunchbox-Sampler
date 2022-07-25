#include "event_sytem.hpp"

using namespace lbs;

EventSystem::EventSystem()
{
	/* we can't call nullptr in the consumer so init to real void func! */
	for( int i = static_cast<int>(Events::DIGITAL::ROTARY_L); static_cast<Events::DIGITAL>(i) < Events::DIGITAL::LAST; i++ ) {
		m_digMapping[ static_cast<Events::DIGITAL>(i) ] = []() { };
	}
	
	for( int i = static_cast<int>(Events::Analog::POTS::POT_0); static_cast<Events::Analog::POTS>(i) < Events::Analog::POTS::POT_3; i++ ) {
		m_analogMapping[ static_cast<Events::Analog::POTS>(i) ] = []( AnalogData ) { };
	}
}

void EventSystem::enqueueDigital( Events::DIGITAL e )
{
	m_scheduler.m_digitalListener.send( m_digMapping.find( e )->second );
}

void EventSystem::enqueueAnalog( Events::Analog::POTS e, AnalogData value )
{
	//scheduler.m_analogListener.send(m_analogMapping.find(e)->second);
	auto f = m_analogMapping.find( e )->second;
	std::function< void() > binder = std::bind( getAnalogMapping().find( e )->second, value );
	//m_scheduler.m_analogListener.send( static_cast<std::function< void( unsigned short ) >>(std::bind(&test, value)) );
	m_scheduler.m_analogListener.send( binder );
}

void EventSystem::attachDigital( lbs::Events::DIGITAL e, std::function< void() > f )
{
	m_digMapping[ e ] = std::move( f );
}

void EventSystem::detachDigital( lbs::Events::DIGITAL e )
{
	m_digMapping[ e ] = []() { };
}

void EventSystem::attachAnalog( lbs::Events::Analog::POTS e, std::function< void( AnalogData ) > f )
{
	m_analogMapping[ e ] = std::move( f );
}

EventSystem::AnalogMapping& EventSystem::getAnalogMapping() { return m_analogMapping; }