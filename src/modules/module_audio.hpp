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
	ModuleAudio();
	void enter( Graphics* g ) override;
	void update( Graphics* g, Events::DIGITAL e ) override;
	void drawMidiVisualisation(MidiData midi_data, Graphics* g);
	void exit() override;
	~ModuleAudio() override;
private:
	Window m_topNav;
	Window m_midiVisualisation;
	Window m_information;
};