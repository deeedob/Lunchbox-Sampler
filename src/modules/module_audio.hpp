/**************************************************************************************************
 * Copyright (c) 2022. Dennis Oberst                                                              *
 **************************************************************************************************/

#pragma once
#include "abstract_module.hpp"

using namespace lbs;

class ModuleAudio : public AbstractModule
{
public:
	ModuleAudio();
	void enter( Graphics* g ) override;
	void update( Graphics* g, Events::DIGITAL e ) override;
	void exit() override;
	~ModuleAudio() override;
private:
	Window m_topNav;
	Window m_midiVisualisation;
	Window m_information;
};