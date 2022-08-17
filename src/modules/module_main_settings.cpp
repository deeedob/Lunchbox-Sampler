#include "module_main_settings.hpp"

using namespace lbs;

ModuleMainSettings::ModuleMainSettings()
	: AbstractModule( "Main Settings" )
{

}

void ModuleMainSettings::enter( Graphics* g )
{
	Window temp_window;
	temp_window.fillScreen( 0x44 );
	temp_window.drawWindowBorder( { 4, 4 }, 2, 0xbb, 2 );
	temp_window.setTextPadding( { 4, 4 } );
	temp_window.setTextColor( 0xff );
	temp_window.printlnCentered( "Unimplemented!" );
	
	g->drawWindow( temp_window );
	g->display();
}

void ModuleMainSettings::update( Graphics* g, lbs::Events::DIGITAL e )
{
	if( e == Events::DIGITAL::BTN_RETURN ) {
		exit();
		return;
	}
}

void ModuleMainSettings::exit()
{
	notify( *this );
}