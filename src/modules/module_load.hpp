/**************************************************************************************************
 * Copyright (c) 2022. Dennis Oberst                                                              *
 **************************************************************************************************/

#pragma once
#include <abstract_module.hpp>

using namespace lbs;

class ModuleLoad : public AbstractModule
{
public:
	ModuleLoad();
	void enter( Graphics* g ) override;
	void update( Graphics* g ) override;
	void exit() override;
	~ModuleLoad() override;
};