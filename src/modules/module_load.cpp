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
	m_state.push_back(STATE::SAMPLE);
	m_state.push_back(STATE::SAMPLEPACK);
	m_current=m_state.begin();
	
}
void ModuleLoad::increment_state()
{
	++m_current;
	if( m_current == m_state.end())
		m_current = m_state.begin();
}

void ModuleLoad::decrement_state()
{
	if( m_current == m_state.begin()) {
		m_current = m_state.end();
	}
	m_current--;
}

void ModuleLoad::enter( Graphics* g )
{
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
	if(m_state.begin()==m_current) {
		m_bottom.setTextColor( 0x00 );
		m_bottom.printlnHoverCenter("SAMPLE", 0xff );
	}
	else {
		m_bottom.setTextColor( 0xff );
		m_bottom.printlnHCentered( "SAMPLE");
	}
	if(m_state.begin()+1==m_current) {
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
	if( e == Events::DIGITAL::BTN_RETURN ) {
		exit();
		return;
	}
	if( e == Events::DIGITAL::BTN_ENTER && m_current==m_state.begin()) {
		loadSamples(g);
		return;
	}
	if( e == Events::DIGITAL::BTN_ENTER && m_current==m_state.begin()+1) {
		loadSamplePacks(g);
		return;
	}
	if( e == Events::DIGITAL::ROTARY_L )
		decrement_state();
	if( e == Events::DIGITAL::ROTARY_R )
		increment_state();
	draw_load_menu1(g);
	
}

void ModuleLoad::exit()
{
	notify( *this );
}

void ModuleLoad::loadSamplePacks(Graphics* g) {
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
void ModuleLoad::loadSamples(Graphics* g) {}
