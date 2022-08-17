/**************************************************************************************************
 * Copyright (c) 2022. Dennis Oberst                                                              *
 **************************************************************************************************/

#pragma once
#include "abstract_module.hpp"

using namespace lbs;

class ModuleSoundManager : public AbstractModule
{
public:
	ModuleSoundManager();
	void enter( Graphics* g ) override;
	void update( Graphics* g, Events::DIGITAL e ) override;
	void exit() override;
};