#pragma once
#include <memory>
#include <Arduino.h>
#include <list>
#include "graphics.hpp"
#include "transition_table.hpp"

namespace lbs
{
	class AbstractModule;
	
	class UI
	{
		enum class State
		{
			LOAD,
			AUDIO,
			MANAGESOUND,
			RECORD,
			MAINSETTINGS
		};
	public:
		UI();
		void start();
		std::list<String>& getTransitionList() const;
		void setTransitionList( const std::list<String>& transition_list );
	private:
		std::unique_ptr<Graphics> m_graphics;
		std::list<std::pair<String, const AbstractModule&>> m_transitionList;
		State m_currentState;
	};
}