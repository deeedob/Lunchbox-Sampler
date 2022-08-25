#pragma once
#include "abstract_module.hpp"
#include <list>

using namespace lbs;

class ModuleLoad : public AbstractModule
{
	enum class STATE
	{
		SAMPLE,
		SAMPLEPACK,
		SAMPLEPACKX,
		SAMPLEX,
		NOTE,
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
	void chooseStartNote(Graphics* g);
private:
	std::vector<STATE> m_state1;
	std::vector<STATE> m_state2;
	std::vector<STATE> m_state3;
	std::vector<STATE>::iterator m_current;
	Window m_topNav;
	Window m_bottom;
	Window m_midiVisualisation;
	int Key=1;
};