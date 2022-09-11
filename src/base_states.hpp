#pragma once
#include <graphics.hpp>
#include <list>
#include <memory>
#include <observer.hpp>

#include "modules/module_audio.hpp"
#include "modules/module_load.hpp"
#include "modules/module_main_settings.hpp"
#include "modules/module_recorder.hpp"
#include "modules/module_sound_manager.hpp"

using namespace lbs;

class BaseStates : public Observer<AbstractModule>
{
	
	enum class STATE
	{
		MAIN_MENU,// this class
		LOAD,
		MANAGESOUND,
		AUDIO,
		RECORD,
		MAINSETTINGS
	};
	
	using Transition = std::pair<STATE, std::unique_ptr<AbstractModule> >;
	using TransitionList = std::vector<Transition>;

public:
	BaseStates();
	void baseUpdate( Events::DIGITAL e );

private:
	/*!
	 * @brief Gets emitted when a derived module exits!
	 * @param src The derived module
	 */
	void emit( const AbstractModule& src ) override;
	void increment_state();
	void decrement_state();
	void draw_main_menu();
	void update_main_menu( Events::DIGITAL e );

private:
	std::unique_ptr<Graphics> m_graphics;
	TransitionList m_transitionList;
	uint8_t m_current;
	Window m_top;
	Window m_bottom;
	bool m_inModule;
};