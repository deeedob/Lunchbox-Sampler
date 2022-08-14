#include "module_load.hpp"

using namespace lbs;

ModuleLoad::ModuleLoad()
{

}

ModuleLoad::~ModuleLoad()
{

}

void ModuleLoad::enter( Graphics* g )
{
	Window::SplitScreen screens = Window::createSplitScreen( 0.45f, Window::SPLIT::VERTICAL, g->width(), g->height());
	auto& left = screens.first;
	auto& right = screens.second;
	
	left.drawWindowBorder( { 2, 2 }, 0, 0x00, 2 );
	right.drawWindowBorder( { 2, 2 }, 0, 0x00, 2 );
	
	g->drawWindow( screens );
}

void ModuleLoad::update( Graphics* g )
{

}

void ModuleLoad::exit()
{

}