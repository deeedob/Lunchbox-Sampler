#pragma once
#include "abstract_module.hpp"

using namespace lbs;

class ModuleRecorder : public AbstractModule
{
public:
	ModuleRecorder();
	void enter( Graphics* g ) override;
	void update( Graphics* g, Events::DIGITAL e ) override;
	void exit() override;
};