#include "module_audio.hpp"

using namespace lbs;

ModuleAudio::ModuleAudio()
	: AbstractModule( "Audio" )
{
	auto split_screens = WindowSize::createSplitScreen( UTIL::HORIZONTAL, 0.15f );
	auto split_screens2 = WindowSize::createSplitScreen( UTIL::HORIZONTAL, 0.65f );
	m_topNav = Window( split_screens.first );
	m_midiVisualisation=Window( split_screens.second );
	m_information=Window( split_screens2.second );
}

ModuleAudio::~ModuleAudio()
{

}

void ModuleAudio::enter( Graphics* g )
{
	Window temp_window;
	temp_window.fillScreen( 0x44 );
	temp_window.drawWindowBorder( { 4, 4 }, 2, 0xbb, 2 );
	temp_window.setTextPadding( { 4, 4 } );
	temp_window.setTextColor( 0xff );
	
	m_topNav.fillScreen(0xff);
	m_topNav.setTextColor(0x44);
	m_topNav.printlnCentered("MIDI");
	
	for(int i=8; i<=116; i=i+8)
	{
		m_midiVisualisation.drawRect(i,5,8,50,0x44);
		m_midiVisualisation.fillRect(i+1,6,6,48,0xff);
	}
	int count=0;
	for(int i=8 ; i<116; i=i+8)
	{
		if(count!=2 && count!=6 && count!=9 &&count!=13)
		{
			m_midiVisualisation.drawRect(i+5,5,6,30,0x44);
			m_midiVisualisation.fillRect(i+6,6,4,28,0x11);
	
		}
		Serial.println(count);
		count++;
	}
	m_information.fillScreen(0x44);
	m_information.drawWindowBorder({ 2, 2 }, 0, 0xbb, 1 );
	m_information.setTextColor(0xff);
	m_information.setTextSize(0.4);
	m_information.printlnHCentered("Note:");
	m_information.printlnHCentered("Velocity:");
	
	
	temp_window.fillRect(4,80,120,44,0x44);
	temp_window.drawRect(4,80,120,44,0xbb);
	temp_window.drawRect(8,84,112,36,0xbb);
	
	
	g->drawWindow( m_topNav );
	g->drawWindow( m_midiVisualisation );
	g->drawWindow( m_information );
	//g->drawWindow( temp_window );
	g->display();
}
void ModuleAudio::update( Graphics* g, Events::DIGITAL e )
{
	if( e == Events::DIGITAL::BTN_RETURN ) {
		exit();
		return;
	}
}

void ModuleAudio::exit()
{
	notify( *this );
}