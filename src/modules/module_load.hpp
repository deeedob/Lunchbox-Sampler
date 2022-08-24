#pragma once
#include "abstract_module.hpp"
#include <list>

using namespace lbs;

class ModuleLoad : public AbstractModule
{
	enum class STATE
	{
		SAMPLE,
		SAMPLEPACK
	};
public:
	ModuleLoad();
	void enter( Graphics* g ) override;
	void update( Graphics* g, Events::DIGITAL e ) override;
	void exit() override;
private:
	void draw_load_menu1(Graphics* g);
	void decrement_state();
	void increment_state();
	void loadSamples(Graphics* g);
	void loadSamplePacks(Graphics* g);
private:
	std::vector<STATE> m_state;
	std::vector<STATE>::iterator m_current;
	Window m_topNav;
	Window m_bottom;
};