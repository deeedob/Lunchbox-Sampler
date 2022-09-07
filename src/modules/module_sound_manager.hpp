#pragma once
#include "abstract_module.hpp"

using namespace lbs;

class ModuleSoundManager : public AbstractModule
{
	struct file
	{
		String name;
		int midiNote;
	};
	enum class STATE
	{
		//MainStates
		SOUND_MANAGER_MODULE_MAIN,     //CHOOSE SAMPLEPACK
		SOUND_MANAGER_MODULE_SUB_1,    //CHOOSE SAMPLE
		SOUND_MANAGER_MODULE_SUB_2,    //CHOOSE EFFECT
		SOUND_MANAGER_MODULE_SUB_3,    //DETAIL EFFECT
		
		//SubStates
		CHANGE_ROUTING,
		REMOVE_SAMPLES,
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
	void draw_load_module_loadbar(Graphics* g);
	void draw_key_mapping( Graphics*g);
	void draw_sound_manager_module_sub2(Graphics* g);
	void draw_sound_manager_module_sub3(Graphics* g);
	
private:
	std::vector<STATE> m_mainStates;
	std::vector<STATE> m_subStates;
	std::vector<STATE>::iterator m_currentMainState;
	std::vector<STATE>::iterator m_currentSubState;
	std::vector <file> bsp_data_Flash;
	std::string MidiNotes [128]{
	    "C-1","C#-1", "D-1","D#-1","E-1","F-1","F#-1","G-1","G#-1","A-1","A#-1","B-1",
	    "C0","C#0", "D0","D#0","E0","F0","F#0","G0","G#0","A0","A#0","B0",
	    "C1","C#1", "D1","D#1","E1","F1","F#1","G1","G#1","A1","A#1","B1",
	    "C2","C#2", "D2","D#2","E2","F2","F#2","G2","G#2","A2","A#2","B2",
	    "C3","C#3", "D3","D#3","E3","F3","F#3","G3","G#3","A3","A#3","B3",
	    "C4","C#4", "D4","D#4","E4","F4","F#4","G4","G#4","A4","A#4","B4",
	    "C5","C#5", "D5","D#5","E5","F5","F#5","G5","G#5","A5","A#5","B5",
	    "C6","C#6", "D6","D#6","E6","F6","F#6","G6","G#6","A6","A#6","B6",
	    "C7","C#7", "D7","D#7","E7","F7","F#7","G7","G#7","A7","A#7","B7",
	    "C8","C#8", "D8","D#8","E8","F8","F#8","G8","G#8","A8","A#8","B8",
	    "C9","C#9", "D9","D#9","E9","F9","F#9", "G9"
	};
	float m_lastValue;
	Window m_bottom;
	Window m_top;
	Window m_left;
	Window m_right;
	
	int m_key=0;
	int m_note;
	int m_selected_sample =0;
	int m_selected_pack =0;
	int m_first_visible_sample =0;
};