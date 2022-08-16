#pragma once
#include "abstract_module.hpp"

using namespace lbs;

class ModuleMainMenu : public AbstractModule
{
public:
	ModuleMainMenu();
	void enter( Graphics* g ) override;
	void update( Graphics* g ) override;
	void exit() override;
	~ModuleMainMenu() override;
};