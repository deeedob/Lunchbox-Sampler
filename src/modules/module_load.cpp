#include "module_load.hpp"

using namespace lbs;

ModuleLoad::ModuleLoad()
	: AbstractModule( "Load Samples" )
{
	auto split_screens = WindowSize::createSplitScreen( UTIL::HORIZONTAL, 0.2f );
	m_top = Window( split_screens.first );
	m_bottom = Window( split_screens.second );
}

void ModuleLoad::enter( Graphics* g )
{
	m_top.fillScreen( 0xff );
	m_top.drawWindowBorder( { 2, 2 }, 0, 0x00, 1 );
	m_top.printlnCentered( m_moduleName.c_str());
	
	m_bottom.fillScreen( 0x44 );
	m_bottom.drawWindowBorder( { 4, 4 }, 2, 0xbb, 2 );
	m_bottom.setTextPadding( { 4, 4 } );
	
	g->drawWindow( m_top );
	g->drawWindow( m_bottom );
	g->display();
}

void ModuleLoad::update( Graphics* g, Events::DIGITAL e )
{

}

void ModuleLoad::exit()
{

}