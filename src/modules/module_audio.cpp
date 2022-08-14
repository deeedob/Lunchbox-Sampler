#include "module_audio.hpp"

using namespace lbs;

void ModuleAudio::enter( Graphics* g )
{
	Window::SplitScreen screens = Window::createSplitScreen( 0.25f, Window::SPLIT::HORIZONTAL, g->width(), g->height());
	auto& left = screens.first;
	auto& right = screens.second;
	
	left.drawWindowBorder( { 2, 2 }, 0, 0x00, 2 );
	right.drawWindowBorder( { 2, 2 }, 0, 0x00, 2 );
	
	g->drawWindow( screens );
}

void ModuleAudio::update( Graphics* g )
{

}

void ModuleAudio::exit()
{

}

ModuleAudio::~ModuleAudio()
{

}