#pragma once
#include "abstract_module.hpp"
#include <string>

using namespace lbs;

class ModuleAudio : public AbstractModule
{
public:
	enum class STATE
	{
		//MainStates
		MAIN,
		MASTER,
		CHANNEL_1,
		CHANNEL_2,
		CHANNEL_3,
		CHANNEL_4,
		
		AUDIO_MODULE_MAIN,
		AUDIO_MODULE_SUB_1,
		AUDIO_MODULE_SUB_2,
		AUDIO_MODULE_SUB_3,
		
		//CHANNELS
		CHANNEL_MASTER,
		
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
	explicit ModuleAudio( std::shared_ptr<Audio> audio );
	void enter( Graphics* g ) override;
	void update( Graphics* g, Events::DIGITAL e ) override;
	void draw_audio_module_main( Graphics* g );
	void draw_audio_module_sub1( Graphics* g );
	void draw_audio_module_sub2( Graphics* g );
	void exit() override;
	~ModuleAudio() override;
	void update( Graphics* g, Events::Analog::POTS pots, const AnalogData& data ) override;
private:
	void draw_main();
	void draw_master( Events::Analog::POTS e );
	void draw_channel1();
	void draw_channel2();
	void draw_channel3();
	void draw_channel4();
private:
	TransitionTable<STATE, Events::DIGITAL, void()> m_transitionTable;
	bool m_return { false };
	int8_t m_oldPot0 { 0 }, m_oldPot1 { 0 }, m_oldPot2 { 0 }, m_oldPot3 { 0 };
	std::shared_ptr<Audio> m_audio;
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