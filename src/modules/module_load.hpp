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
		CHOOSE_SAMPLEPACK,
		CHOOSE_SAMPLE,
		CHOOSE_NOTE,
	};
public:
	ModuleLoad();
	void enter( Graphics* g ) override;
	void update( Graphics* g, Events::DIGITAL e ) override;
	void exit() override;
private:
	void draw_load_module(Graphics* g);
	void decrement_state();
	void increment_state();
	void draw_load_Samples(Graphics* g);
	void draw_load_SamplePacks(Graphics* g);
	void draw_choose_MidiNote(Graphics* g);
private:
	std::vector<STATE> m_loadMenu;
	std::vector<STATE> m_chooseFile;
	std::vector<STATE> m_chooseNote;
	std::vector<STATE>::iterator m_current;
	Window m_topNav;
	Window m_bottom;
	int Key=1;
};