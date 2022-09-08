/**************************************************************************************************
 * Copyright (c) 2022. Dennis Oberst                                                              *
 **************************************************************************************************/

#pragma once
#include "abstract_module.hpp"
#include <string>

using namespace lbs;

class ModuleAudio : public AbstractModule
{
public:
	struct MidiData{
		uint8_t type;
		uint8_t channel;
		uint8_t data1;
		uint8_t data2;
	};
	enum class STATE
	{
		//MainStates
		AUDIO_MODULE_MAIN,
		AUDIO_MODULE_SUB_1,
		AUDIO_MODULE_SUB_2,
		AUDIO_MODULE_SUB_3,
		
		//CHANNELS
		CHANNEL_MASTER,
		CHANNEL_1,
		CHANNEL_2,
		CHANNEL_3,
		CHANNEL_4,
		
		//EFFECTS
		EFFECT_VOLUME,
		EFFECT_FILTER,
		EFFECT_DELAY,
		EFFECT_COMPRESSOR,
		
		//FILTER DETAIL
		IS_HIGHPASS,
		FREQUENCY,
		
		//DELAY DETAIL
		TIME,
		FEEDBACK,
		
		//DETAIL COMPRESSOR
		TRESHOLD,
		RATIO,
		GAIN,
	};
	ModuleAudio();
	void enter( Graphics* g ) override;
	void update( Graphics* g, Events::DIGITAL e ) override;
	void draw_audio_module_main(Graphics* g);
	void draw_audio_module_sub1(Graphics* g);
	void draw_audio_module_sub2(Graphics* g);
	void exit() override;
	~ModuleAudio() override;
private:
	Window m_top;
	Window m_bottom;
	Window m_left;
	Window m_right;
	std::vector<STATE>::iterator m_currentMainState;
	std::vector<STATE>::iterator m_currentChannel;
	std::vector<STATE>::iterator m_currentEffect;
	std::vector<STATE>::iterator m_currentDetailFilter;
	std::vector<STATE>::iterator m_currentDetailDelay;
	std::vector<STATE>::iterator m_currentDetailCompressor;
	std::vector<STATE> m_mainStates;
	std::vector<STATE> m_channels;
	std::vector<STATE> m_effects;
	std::vector<STATE> m_detailFilter;
	std::vector<STATE> m_detailDelay;
	std::vector<STATE> m_detailCompressor;
	bool highpass;
	float m_last_value_rot0;
	float m_last_value_rot1;
	int frequency;
};