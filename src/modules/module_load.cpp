#include "module_load.hpp"

using namespace lbs;

ModuleLoad::ModuleLoad()
	: AbstractModule( "Load Samples" )
{
	/*auto split_screens = WindowSize::createSplitScreen( UTIL::HORIZONTAL, 0.15f );
	auto left_right = WindowSize::divideWindow( UTIL::VERTICAL, 0.3f, split_screens.second );
	
	m_topNav = Window( split_screens.first );
	m_left = Window( left_right.first );
	m_right = Window( left_right.second );*/
	auto split_screens = WindowSize::createSplitScreen( UTIL::HORIZONTAL, 0.15 );
	m_topNav = Window( split_screens.first );
	m_bottom = Window( split_screens.second );
}
void ModuleLoad::increment_state()
{
	++m_current;
	if( m_current == m_loadMenu.end())
		m_current = m_loadMenu.begin();
}
void ModuleLoad::decrement_state()
{
	if( m_current == m_loadMenu.begin()) {
		m_current = m_loadMenu.end();
	}
	m_current--;
}
void ModuleLoad::enter( Graphics* g )
{
	m_loadMenu.push_back(STATE::SAMPLE);
	m_loadMenu.push_back(STATE::SAMPLEPACK);
	m_current= m_loadMenu.begin();
	draw_load_module( g );
}
void ModuleLoad::draw_load_module(Graphics* g)
{
	m_topNav.fillScreen( 0xff );
	m_topNav.drawWindowBorder( { 2, 2 }, 0, 0x00, 1 );
	m_topNav.printlnCentered( "LOAD SAMPLES");
	
	m_bottom.fillScreen( 0x44 );
	m_bottom.drawWindowBorder( { 4, 4 }, 2, 0xbb, 2 );
	m_bottom.setTextPadding( { 4, 30 } );
	if( m_loadMenu.begin()==m_current) {
		m_bottom.setTextColor( 0x00 );
		m_bottom.printlnHoverCenter("SAMPLE", 0xff );
	}
	else {
		m_bottom.setTextColor( 0xff );
		m_bottom.printlnHCentered( "SAMPLE");
	}
	if( m_loadMenu.begin()+1==m_current) {
		m_bottom.setTextColor( 0x00 );
		m_bottom.printlnHoverCenter( "SAMPLEPACK", 0xff );;
	}
	else{
		m_bottom.setTextColor( 0xff );
		m_bottom.printlnHCentered( "SAMPLEPACK");
	}
	g->drawWindow( m_topNav );
	g->drawWindow( m_bottom );
	g->display();
}
void ModuleLoad::update( Graphics* g, Events::DIGITAL e )
{
	if( e == Events::DIGITAL::BTN_RETURN && !m_loadMenu.empty()) {
		Serial.println("MAIN MENU");
		m_loadMenu.clear();
		exit();
		return;
	}
	if( e == Events::DIGITAL::BTN_RETURN && m_loadMenu.empty()) {
		m_loadMenu.push_back(STATE::SAMPLE);
		m_loadMenu.push_back(STATE::SAMPLEPACK);
		m_current= m_loadMenu.begin();
		draw_load_module( g );
		Serial.println("LOAD SAMPLE MAIN MENU");
		return;
	}
	if( e == Events::DIGITAL::BTN_ENTER && m_current== m_loadMenu.begin()) {
		Serial.println("LOAD SAMPLE");
		draw_load_Samples(g);
		return;
	}
	if( e == Events::DIGITAL::BTN_ENTER && m_current== m_loadMenu.begin()+1) {
		Serial.println("LOAD SAMPLEPACK");
		draw_load_SamplePacks( g );
		return;
	}
	if( e == Events::DIGITAL::BTN_ENTER && m_current==m_chooseFile.begin() && m_chooseNote.empty()) {
		Serial.println("CHOOSE START CHOOSE_NOTE");
		draw_choose_MidiNote( g );
		return;
	}
	if( e == Events::DIGITAL::BTN_ENTER && m_current==m_chooseFile.begin()+1 && m_chooseNote.empty()) {
		Serial.println("CHOOSE START CHOOSE_NOTE");
		draw_choose_MidiNote( g );
		return;
	}
	if( e == Events::DIGITAL::ROTARY_L &&!m_loadMenu.empty()){
		decrement_state();
	}
	if( e == Events::DIGITAL::ROTARY_R &&!m_loadMenu.empty())
	{increment_state();}
	if( e == Events::DIGITAL::ROTARY_R &&!m_chooseNote.empty()) {
		Key++;
		if(Key==24)
		{
			Key=0;
			return;
		}
		draw_choose_MidiNote( g );
		return;
	}
	if( e == Events::DIGITAL::BTN_ENTER &&!m_chooseNote.empty()) {
		Serial.println("uhu");
		if(m_current==m_chooseFile.begin())
		{
			Serial.println("Back To Load Samples");
			m_chooseNote.clear();
			draw_load_Samples( g );
			return;
		}
		if(m_current==m_chooseFile.begin()+1)
		{
			m_chooseNote.clear();
			draw_load_SamplePacks( g );
			return;
		}
		
	}
	if( e == Events::DIGITAL::ROTARY_L &&!m_chooseNote.empty()) {
		Key--;
		if(Key==0)
		{
			Key=24;
		}
		Serial.println(Key);
		draw_choose_MidiNote( g );
	}
	if(!m_loadMenu.empty()) draw_load_module( g );
}
void ModuleLoad::exit()
{
	notify( *this );
}
void ModuleLoad::draw_choose_MidiNote(Graphics* g){
	m_chooseNote.push_back(STATE::CHOOSE_NOTE );
	m_topNav.fillScreen(0xff);
	m_topNav.setTextColor(0x44);
	m_topNav.printlnCentered("MIDI");
	m_bottom.fillScreen(0x00);
	
	int whiteKeys [14]={1,3,5,6,8,10,12,13,15,17,18,20,22,24};
	int blackKeys [10]={2,4,7,9,11,14,16,19,21,23};
	int wCount=0;
	for(int i=8; i<=116; i=i+8)
	{
		if(whiteKeys[wCount]==Key)
		{
			m_bottom.drawRect(i,5,8,50,0xbb);
			m_bottom.fillRect(i+1,6,6,48,0xbb);
		}
		else{
			m_bottom.drawRect(i,5,8,50,0x44);
			m_bottom.fillRect(i+1,6,6,48,0xff);
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
				m_bottom.drawRect(i+5,5,6,30,0x44);
				m_bottom.fillRect(i+6,6,4,28,0xbb);
			}
			else
			{
				m_bottom.drawRect(i+5,5,6,30,0x44);
				m_bottom.fillRect(i+6,6,4,28,0x11);
			}
			bcount++;
	
		}
		count++;
	}
	g->drawWindow( m_topNav );
	g->drawWindow( m_bottom );
	g->display();
}
void ModuleLoad::draw_load_SamplePacks(Graphics* g) {
	m_loadMenu.clear();
	m_chooseFile.clear();
	m_chooseNote.clear();
	m_chooseFile.push_back(STATE::CHOOSE_SAMPLE );
	m_chooseFile.push_back(STATE::CHOOSE_SAMPLEPACK );
	m_current=m_chooseFile.begin()+1;
	g->clearDisplay();
	m_topNav.fillScreen( 0xff );
	m_topNav.drawWindowBorder( { 2, 2 }, 0, 0x00, 1 );
	m_topNav.printlnCentered( "LOAD SAMPLEPACKS");
	
	m_bottom.fillScreen( 0x44 );
	m_bottom.drawWindowBorder( { 4, 4 }, 2, 0xbb, 2 );
	m_bottom.setTextPadding( { 4, 4 } );
	m_bottom.setTextColor(0xFF);
	m_bottom.fillRect(8,8,110,45,0xFF);
	m_bottom.fillRect(8,55,110,45,0xFF);
	
	g->drawWindow( m_topNav );
	g->drawWindow( m_bottom );
	g->display();
	
}
void ModuleLoad::draw_load_Samples(Graphics* g) {
	m_loadMenu.clear();
	m_chooseFile.clear();
	m_chooseNote.clear();
	m_chooseFile.push_back(STATE::CHOOSE_SAMPLE );
	m_chooseFile.push_back(STATE::CHOOSE_SAMPLEPACK );
	m_current=m_chooseFile.begin();
	
	g->clearDisplay();
	m_topNav.fillScreen( 0xff );
	m_topNav.drawWindowBorder( { 2, 2 }, 0, 0x00, 1 );
	m_topNav.printlnCentered( "LOAD SAMPLE");
	
	m_bottom.fillScreen( 0x44 );
	m_bottom.drawWindowBorder( { 4, 4 }, 2, 0xbb, 2 );
	m_bottom.setTextPadding( { 4, 4 } );
	m_bottom.setTextColor(0xFF);
	m_bottom.fillRect(8,8,110,45,0xFF);
	m_bottom.fillRect(8,55,110,45,0xFF);
	
	g->drawWindow( m_topNav );
	g->drawWindow( m_bottom );
	g->display();
}
