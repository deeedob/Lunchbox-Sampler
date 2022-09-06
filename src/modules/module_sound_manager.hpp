#pragma once
#include "abstract_module.hpp"

using namespace lbs;

class ModuleSoundManager : public AbstractModule
{
	enum class STATE
	{
		//MainStates
		SOUND_MANAGER_MODULE_MAIN,     //CHOOSE SAMPLEPACK
		SOUND_MANAGER_MODULE_SUB_1,    //CHOOSE SAMPLE
		SOUND_MANAGER_MODULE_SUB_2,    //CHOOSE EFFECT
		SOUND_MANAGER_MODULE_SUB_3,    //DETAIL EFFECT
		
		//SubStates
		REVERSE,
		FILTER1,
		FILTER2,
		VOLUME,
		CUT,
		
		
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
	void draw_sound_manager_module_main(Graphics* g);
	void draw_sound_manager_module_sub1(Graphics* g);
	void draw_sound_manager_module_sub2(Graphics* g);
	void draw_sound_manager_module_sub3(Graphics* g);
	
private:
	std::vector<STATE> m_mainStates;
	std::vector<STATE> m_subStates;
	std::vector<STATE>::iterator m_currentMainState;
	std::vector<STATE>::iterator m_currentSubState;
	std::string bsp_data_Flash[12]={"Sample1","Sample2", "Sample3", "Sample4", "Sample5", "Sample6", "Sample7", "Sample8", "Sample9", "Sample10", "Sample11", "Sample12"};
	std::string bsp_data_Flash2[6][12]={{"SamplePack1","Sample1","Sample2", "Sample3", "Sample4", "Sample5", "Sample6", "Sample7", "Sample8", "Sample9", "Sample10", "Sample11"},{"SamplePack2"}, {"SamplePack3"}, {"SamplePack4"}, {"SamplePack5"},{"SamplePack6"}};
	float m_lastValue;
	Window m_bottom;
	Window m_top;
	
	int m_selected_sample =0;
	int m_selected_pack =0;
	int m_first_visible_sample =0;
};