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
	if( m_current == m_state1.end())
		m_current = m_state1.begin();
}
void ModuleLoad::decrement_state()
{
	if( m_current == m_state1.begin()) {
		m_current = m_state1.end();
	}
	m_current--;
}
void ModuleLoad::enter( Graphics* g )
{
	m_state1.push_back(STATE::SAMPLE);
	m_state1.push_back(STATE::SAMPLEPACK);
	m_current=m_state1.begin();
	draw_load_menu1(g);
}
void ModuleLoad::draw_load_menu1(Graphics* g)
{
	m_topNav.fillScreen( 0xff );
	m_topNav.drawWindowBorder( { 2, 2 }, 0, 0x00, 1 );
	m_topNav.printlnCentered( "LOAD SAMPLES");
	
	m_bottom.fillScreen( 0x44 );
	m_bottom.drawWindowBorder( { 4, 4 }, 2, 0xbb, 2 );
	m_bottom.setTextPadding( { 4, 30 } );
	if(m_state1.begin()==m_current) {
		m_bottom.setTextColor( 0x00 );
		m_bottom.printlnHoverCenter("SAMPLE", 0xff );
	}
	else {
		m_bottom.setTextColor( 0xff );
		m_bottom.printlnHCentered( "SAMPLE");
	}
	if(m_state1.begin()+1==m_current) {
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
	if( e == Events::DIGITAL::BTN_RETURN && !m_state1.empty()) {
		Serial.println("MAIN MENU");
		exit();
		return;
	}
	if( e == Events::DIGITAL::BTN_RETURN && m_state1.empty()) {
		m_state1.push_back(STATE::SAMPLE);
		m_state1.push_back(STATE::SAMPLEPACK);
		m_current=m_state1.begin();
		draw_load_menu1(g);
		Serial.println("LOAD SAMPLE MAIN MENU");
		return;
	}
	if( e == Events::DIGITAL::BTN_ENTER && m_current==m_state1.begin()) {
		Serial.println("LOAD SAMPLE");
		loadSamples(g);
		return;
	}
	if( e == Events::DIGITAL::BTN_ENTER && m_current==m_state1.begin()+1) {
		Serial.println("LOAD SAMPLEPACK");
		loadSamplePacks(g);
		return;
	}
	if( e == Events::DIGITAL::BTN_ENTER && m_current==m_state2.begin() && m_state3.empty()) {
		Serial.println("CHOOSE START NOTE");
		chooseStartNote(g);
		return;
	}
	if( e == Events::DIGITAL::BTN_ENTER && m_current==m_state2.begin()+1 && m_state3.empty()) {
		Serial.println("CHOOSE START NOTE");
		chooseStartNote(g);
		return;
	}
	if( e == Events::DIGITAL::ROTARY_L &&!m_state1.empty()){
		decrement_state();
	}
	if( e == Events::DIGITAL::ROTARY_R &&!m_state1.empty())
	{increment_state();}
	if( e == Events::DIGITAL::ROTARY_R &&!m_state3.empty()) {
		Key++;
		if(Key==24)
		{
			Key=0;
			return;
		}
		chooseStartNote(g);
		return;
	}
	if( e == Events::DIGITAL::BTN_ENTER &&!m_state3.empty()) {
		Serial.println("uhu");
		if(m_current==m_state2.begin())
		{
			Serial.println("Back To Load Samples");
			m_state3.clear();
			loadSamples(g);
			return;
		}
		if(m_current==m_state2.begin()+1)
		{
			m_state3.clear();
			loadSamplePacks(g);
			return;
		}
		
	}
	if( e == Events::DIGITAL::ROTARY_L &&!m_state3.empty()) {
		Key--;
		if(Key==0)
		{
			Key=24;
		}
		Serial.println(Key);
		chooseStartNote(g);
	}
	if(!m_state1.empty())
		draw_load_menu1(g);
}
void ModuleLoad::exit()
{
	notify( *this );
}
void ModuleLoad::chooseStartNote(Graphics* g){
	m_state3.push_back(STATE::NOTE);
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
void ModuleLoad::loadSamplePacks(Graphics* g) {
	m_state1.clear();
	m_state2.clear();
	m_state3.clear();
	m_state2.push_back(STATE::SAMPLEX);
	m_state2.push_back(STATE::SAMPLEPACKX);
	m_current=m_state2.begin()+1;
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
void ModuleLoad::loadSamples(Graphics* g) {
	m_state1.clear();
	m_state2.clear();
	m_state3.clear();
	m_state2.push_back(STATE::SAMPLEX);
	m_state2.push_back(STATE::SAMPLEPACKX);
	m_current=m_state2.begin();
	
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
