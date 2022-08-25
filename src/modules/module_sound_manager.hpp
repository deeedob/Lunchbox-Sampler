#pragma once
#include "abstract_module.hpp"

using namespace lbs;

class ModuleSoundManager : public AbstractModule
{
	enum class STATE
	{
		CHOOSESAMPLE,
		REVERSE,
		CUT,
		VOLUME,
		
		
		//...
	};
public:
	ModuleSoundManager();
	void enter( Graphics* g ) override;
	void update( Graphics* g, Events::DIGITAL e ) override;
	void exit() override;
private:
	void decrement_state();
	void increment_state();
	void draw_choose_Sample(Graphics* g);
	void draw_editor_Menu(Graphics* g);
private:
	std::vector<STATE> m_editor;
	std::vector<STATE> m_chooseSample;
	std::vector<STATE>::iterator m_current;
	Window m_topNav;
	Window m_bottom;
};