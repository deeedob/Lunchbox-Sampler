#include "module_load.hpp"

using namespace lbs;

ModuleLoad::ModuleLoad()
	: AbstractModule( "Load Samples" )
{
	auto split_screens = WindowSize::createSplitScreen( UTIL::HORIZONTAL, 0.15f );
	auto left_right = WindowSize::divideWindow( UTIL::VERTICAL, 0.3f, split_screens.second );
	
	m_topNav = Window( split_screens.first );
	m_left = Window( left_right.first );
	m_right = Window( left_right.second );
}

void ModuleLoad::enter( Graphics* g )
{
	m_topNav.fillScreen( 0xff );
	m_topNav.drawWindowBorder( { 2, 2 }, 0, 0x00, 1 );
	m_topNav.printlnCentered( m_moduleName.c_str());
	
	m_left.fillScreen( 0x44 );
	m_left.drawWindowBorder( { 4, 4 }, 2, 0xbb, 2 );
	m_left.setTextPadding( { 4, 4 } );
	
	m_right.fillScreen( 0x44 );
	m_right.drawWindowBorder( { 4, 4 }, 2, 0xbb, 2 );
	
	g->drawWindow( m_topNav );
	g->drawWindow( m_left );
	g->drawWindow( m_right );
	g->display();
}

void ModuleLoad::update( Graphics* g, Events::DIGITAL e )
{
	if( e == Events::DIGITAL::BTN_RETURN ) {
		exit();
		return;
	}
	
}

void ModuleLoad::exit()
{
	notify( *this );
}