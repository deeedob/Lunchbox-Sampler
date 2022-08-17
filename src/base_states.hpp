#pragma once
#include <graphics.hpp>
#include <memory>
#include <list>

#include "modules/module_load.hpp"
#include "modules/module_audio.hpp"
#include "modules/module_recorder.hpp"
#include "modules/module_sound_manager.hpp"
#include "modules/module_main_settings.hpp"

using namespace lbs;

class BaseStates
{
	
	enum class STATE
	{
		MAIN_MENU,
		LOAD,
		MANAGESOUND,
		AUDIO,
		RECORD,
		MAINSETTINGS
	};
	
	using Transition = std::pair<STATE, std::unique_ptr<AbstractModule>>;
	using TransitionList = std::list<Transition>;
public:
	BaseStates();
	void changeState( Events::DIGITAL e );
private:
	void increment_state();
	void decrement_state();
	void draw_main_menu();
	void update_main_menu( Events::DIGITAL e );
private:
	std::unique_ptr<Graphics> m_graphics;
	TransitionList m_transitionList;
	std::list<Transition>::iterator m_current;
	Window m_top;
	Window m_bottom;
	bool m_inModule;
};