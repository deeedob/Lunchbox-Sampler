#pragma once
#include <graphics.hpp>
#include <memory>
#include <list>
#include <Arduino.h>

#include "modules/module_load.hpp"
#include "modules/module_audio.hpp"
#include "modules/module_sound_manager.hpp"
#include "modules/module_main_menu.hpp"

using namespace lbs;

class BaseStates
{
	enum class State
	{
		MAIN_MENU,
		LOAD,
		MANAGESOUND,
		AUDIO,
		RECORD,
		MAINSETTINGS
	};
	using StateName = std::pair<String, State>;
	using TransitionList = std::list<std::pair<StateName, const AbstractModule&>>;
public:
	BaseStates();
private:
	std::unique_ptr<Graphics> m_graphics;
	TransitionList m_transitionList;
	State m_currentState;
};