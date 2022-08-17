#include "base_states.hpp"
#include <define_t40.hpp>

BaseStates::BaseStates()
	: m_inModule( false )
{
	m_graphics = std::make_unique<Graphics>( C_DISPLAY_WIDTH, C_DISPLAY_HEIGHT, &Wire1, C_DISPLAY_RST, 1000000, 1000000 );
	
	auto split_screens = WindowSize::createSplitScreen( UTIL::HORIZONTAL, 0.2f );
	m_top = Window( split_screens.first );
	m_bottom = Window( split_screens.second );
	
	m_transitionList.push_back( { STATE::LOAD, std::make_unique<ModuleLoad>() } );
	m_transitionList.push_back( { STATE::MANAGESOUND, std::make_unique<ModuleSoundManager>() } );
	m_transitionList.push_back( { STATE::AUDIO, std::make_unique<ModuleAudio>() } );
	m_transitionList.push_back( { STATE::RECORD, std::make_unique<ModuleRecorder>() } );
	m_transitionList.push_back( { STATE::MAINSETTINGS, std::make_unique<ModuleMainSettings>() } );
	m_current = m_transitionList.begin();
	draw_main_menu();
}

void BaseStates::changeState( Events::DIGITAL e )
{
	if( m_inModule ) {
		m_current->second->update( m_graphics.get(), e );
		return;
	}
	update_main_menu( e );
}

void BaseStates::increment_state()
{
	++m_current;
	if( m_current == m_transitionList.end())
		m_current = m_transitionList.begin();
}

void BaseStates::decrement_state()
{
	if( m_current == m_transitionList.begin()) {
		m_current = m_transitionList.end();
	}
	m_current--;
}

void BaseStates::draw_main_menu()
{
	m_top.fillScreen( 0xff );
	m_top.drawWindowBorder( { 2, 2 }, 0, 0x00, 1 );
	m_top.printlnCentered( "MAIN MENU" );
	
	m_bottom.fillScreen( 0x44 );
	m_bottom.drawWindowBorder( { 4, 4 }, 2, 0xbb, 2 );
	m_bottom.setTextPadding( { 4, 4 } );
	for( const auto& item : m_transitionList ) {
		if( item.second->getModuleName().equals( m_current->second->getModuleName())) {
			m_bottom.setTextColor( 0x00 );
			m_bottom.printlnHoverCenter( item.second->getModuleName().c_str(), 0xff );
			continue;
		}
		m_bottom.setTextColor( 0xff );
		m_bottom.printlnHCentered( item.second->getModuleName().c_str());
	}
	
	m_graphics->drawWindow( m_top );
	m_graphics->drawWindow( m_bottom );
	m_graphics->display();
}

void BaseStates::update_main_menu( Events::DIGITAL e )
{
	if( e == Events::DIGITAL::ROTARY_L )
		decrement_state();
	if( e == Events::DIGITAL::ROTARY_R )
		increment_state();
	if( e == Events::DIGITAL::BTN_ENTER ) {
		Serial.println( "btn enter" );
		m_current->second->enter( m_graphics.get());
		m_inModule = true;
		return;
	}
	
	m_top.fillScreen( 0xff );
	m_top.drawWindowBorder( { 2, 2 }, 0, 0x00, 1 );
	m_top.printlnCentered( "MAIN MENU" );
	
	m_bottom.fillScreen( 0x44 );
	m_bottom.drawWindowBorder( { 4, 4 }, 2, 0xbb, 2 );
	m_bottom.setTextPadding( { 4, 4 } );
	for( const auto& item : m_transitionList ) {
		if( item.second->getModuleName().equals( m_current->second->getModuleName())) {
			m_bottom.setTextColor( 0x00 );
			m_bottom.printlnHoverCenter( item.second->getModuleName().c_str(), 0xff );
			continue;
		}
		m_bottom.setTextColor( 0xff );
		m_bottom.printlnHCentered( item.second->getModuleName().c_str());
	}
	
	m_graphics->drawWindow( m_top );
	m_graphics->drawWindow( m_bottom );
	m_graphics->display();
}