#pragma once
#include "abstract_module.hpp"

using namespace lbs;

class ModuleLoad : public AbstractModule
{
public:
	ModuleLoad();
	void enter( Graphics* g ) override;
	void update( Graphics* g, Events::DIGITAL e ) override;
	void exit() override;
private:
	Window m_topNav;
	Window m_left;
	Window m_right;
};