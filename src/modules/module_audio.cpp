#include "module_audio.hpp"


using namespace lbs;

struct MidiData {
	uint8_t type;
	uint8_t channel;
	uint8_t data1;
	uint8_t data2;
};

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
	drawMidiVisualisation({144, 1, 66, 75}, g);
	g->drawWindow( m_topNav );
	g->drawWindow( m_midiVisualisation );
	g->drawWindow( m_information );
	//g->drawWindow( temp_window );
	g->display();
}
void ModuleAudio::drawMidiVisualisation(MidiData m_midi_data, Graphics* g)
{
	Window temp_window;
	temp_window.fillScreen( 0x44 );
	temp_window.drawWindowBorder( { 4, 4 }, 2, 0xbb, 2 );
	temp_window.setTextPadding( { 4, 4 } );
	temp_window.setTextColor( 0xff );
	
	m_topNav.fillScreen(0xff);
	m_topNav.setTextColor(0x44);
	m_topNav.printlnCentered("MIDI");
	
	int whiteKeys [14]={1,3,5,6,8,10,12,13,15,17,18,20,22,24};
	int blackKeys [10]={2,4,7,9,11,14,16,19,21,23};
	int Key;
	for(int i=24; i<100; i=i+24)
	{
		Serial.println("e");
		if(m_midi_data.data1>=i &&m_midi_data.data1<i+24)
		{
			Key=m_midi_data.data1-i+1;
			Serial.println(Key);
		}
	}
	int wCount=0;
	for(int i=8; i<=116; i=i+8)
	{
		if(whiteKeys[wCount]==Key)
		{
			m_midiVisualisation.drawRect(i,5,8,50,0xbb);
			m_midiVisualisation.fillRect(i+1,6,6,48,0xbb);
		}
		else{
			m_midiVisualisation.drawRect(i,5,8,50,0x44);
			m_midiVisualisation.fillRect(i+1,6,6,48,0xff);
		}
		    wCount++;
	}
	int count=0;
	int bcount=0;
	for(int i=8 ; i<116; i=i+8)
	{
		if(count!=2 && count!=6 && count!=9 &&count!=13)
		{
			if(blackKeys[bcount]==Key)
			{
				m_midiVisualisation.drawRect(i+5,5,6,30,0x44);
				m_midiVisualisation.fillRect(i+6,6,4,28,0xbb);
			}
			else
			{
				m_midiVisualisation.drawRect(i+5,5,6,30,0x44);
				m_midiVisualisation.fillRect(i+6,6,4,28,0x11);
			}
			bcount++;
	
		}
		count++;
	}
	m_information.fillScreen(0x44);
	m_information.drawWindowBorder({ 2, 2 }, 0, 0xbb, 1 );
	m_information.setTextColor(0xff);
	m_information.setTextSize(0.4);
	//std::string s = std::to_string(m_midi_data.data1);
	//m_information.printlnHCentered(std::to_string(m_midi_data.data1).cstr());
	//m_information.printlnHCentered(std::to_string(m_midi_data.data2));
	
	temp_window.fillRect(4,80,120,44,0x44);
	temp_window.drawRect(4,80,120,44,0xbb);
	temp_window.drawRect(8,84,112,36,0xbb);
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