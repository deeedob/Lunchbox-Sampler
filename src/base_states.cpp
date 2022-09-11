#include "base_states.hpp"
#include <define_t40.hpp>

BaseStates::BaseStates()
	: m_current( 0 ), m_inModule( false )
{
	m_graphics = std::make_unique<Graphics>( C_DISPLAY_WIDTH, C_DISPLAY_HEIGHT, &Wire1, C_DISPLAY_RST, 1000000, 1000000 );
	
	auto split_screens = WindowSize::createSplitScreen( UTIL::HORIZONTAL, 0.15 );
	m_top = Window( split_screens.first );
	m_bottom = Window( split_screens.second );
	/* init the individual modules */
	auto module_load = std::make_unique<ModuleLoad>();
	auto module_sound_manager = std::make_unique<ModuleSoundManager>();
	auto module_audio = std::make_unique<ModuleAudio>();
	auto module_recorder = std::make_unique<ModuleRecorder>();
	auto module_main_settings = std::make_unique<ModuleMainSettings>();
	/* get notified on exit */
	module_load->subscribe( this );
	module_sound_manager->subscribe( this );
	module_audio->subscribe( this );
	module_recorder->subscribe( this );
	module_main_settings->subscribe( this );
	m_transitionList.reserve( 5 );
	m_transitionList.push_back( { STATE::LOAD, std::move( module_load ) } );
	m_transitionList.push_back( { STATE::MANAGESOUND, std::move( module_sound_manager ) } );
	m_transitionList.push_back( { STATE::AUDIO, std::move( module_audio ) } );
	m_transitionList.push_back( { STATE::RECORD, std::move( module_recorder ) } );
	m_transitionList.push_back( { STATE::MAINSETTINGS, std::move( module_main_settings ) } );
	draw_main_menu();
}

void BaseStates::baseUpdate( Events::DIGITAL e )
{
	if( m_inModule ) {
		m_transitionList[ m_current ].second->update( m_graphics.get(), e );
		return;
	}
	update_main_menu( e );
}

void BaseStates::baseUpdate( Events::Analog::POTS e, const AnalogData& d )
{
	if( m_inModule ) {
		m_transitionList[ m_current ].second->update( m_graphics.get(), e, d );
		return;
	}
}

void BaseStates::increment_state()
{
	++m_current;
	if( m_current == m_transitionList.size())
		m_current = 0;
}

void BaseStates::decrement_state()
{
	if( m_current == 0 ) {
		m_current = m_transitionList.size() - 1;
		return;
	}
	m_current--;
}

void BaseStates::draw_main_menu()
{
	m_top.fillScreen( 0xff );
	m_top.drawWindowBorder( { 2, 2 }, 0, 0x00, 1 );
	m_top.printlnCentered( "Main Menu" );
	
	m_bottom.fillScreen( 0x44 );
	m_bottom.drawWindowBorder( { 4, 4 }, 2, 0xbb, 2 );
	m_bottom.setTextPadding( { 4, 4 } );
	for( const auto& item : m_transitionList ) {
		if( item.second->getModuleName().equals( m_transitionList[ m_current ].second->getModuleName())) {
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
		m_transitionList[ m_current ].second->enter( m_graphics.get());
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
		if( item.second->getModuleName().equals( m_transitionList[ m_current ].second->getModuleName())) {
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

void BaseStates::emit( const AbstractModule& src )
{
	m_inModule = false;
	m_graphics->clearDisplay();
	draw_main_menu();
}