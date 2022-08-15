#pragma once
#include <list>
#include <Arduino.h>
#include <events.hpp>
#include "abstract_module.hpp"

namespace lbs
{
	class TransitionTable
	{
	public:
		using State = std::pair<String, AbstractModule*>;
		using MultiList = std::list<State>;
		
		TransitionTable() = default;
		
		void addState( const State& s )
		{
			m_transitionList.push_back( s );
		}
		
		void update( const Events::DIGITAL& e )
		{
			if( e == Events::DIGITAL::ROTARY_L ) {
				if( m_currentState != m_transitionList.begin()) {
					m_currentState--;
				}
			}
			if( e == Events::DIGITAL::ROTARY_R ) {
				if( m_currentState != m_transitionList.end()) {
					m_currentState++;
				}
			}
			if( e == Events::DIGITAL::BTN_ENTER ) {
				if( m_currentState == m_transitionList.begin())
					m_currentState->second->enter();
			}
		}
	
	private:
		MultiList m_transitionList;
		std::list<State>::iterator m_currentState;
	};
}