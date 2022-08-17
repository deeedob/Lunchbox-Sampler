#pragma once
#include "abstract_module.hpp"

using namespace lbs;

class ModuleMainSettings : public AbstractModule
{
public:
	ModuleMainSettings();
	void enter( Graphics* g ) override;
	void update( Graphics* g, Events::DIGITAL e ) override;
	void exit() override;
};