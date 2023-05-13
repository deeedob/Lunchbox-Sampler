#include "module_load.hpp"
#include <main_memory.hpp>

using namespace lbs;

//TODO: add decision to pure purge and find out if already purged
ModuleLoad::ModuleLoad()
	: AbstractModule( "Sample Manager" ), m_transitionTable()
{
	auto split_screens = WindowSize::createSplitScreen( UTIL::HORIZONTAL, 0.15 );
	m_top = Window( split_screens.first );
	m_bottom = Window( split_screens.second );
	
	m_transitionTable.setName( STATE::MAIN, m_moduleName.c_str());
	m_transitionTable.setName( STATE::LOAD, "Load Samplepack" );
	m_transitionTable.setName( STATE::PURGE, "Remove Current" );
	m_transitionTable.setFunction( STATE::MAIN, [ = ]() {
		this->draw_main();
	} );
	m_transitionTable.setFunction( STATE::LOAD, [ = ]() {
		this->draw_load();
	} );
	m_transitionTable.setFunction( STATE::PURGE, [ = ]() {
		this->draw_purge();
	} );
	m_transitionTable.setFunction( STATE::DECIDE, [ = ]() {
		this->draw_decision();
	} );
	m_transitionTable.setFunction( STATE::YES, [ = ]() {
		m_samples = MainMemory::getSampleNamesFromPack( m_samplePacks[ m_samplePackSelection ] );
		m_inPurge = true;
		MainMemory::eraseFlash();
		m_inPurge = false;
		m_inLoading = true;
		MainMemory::loadSamplepack( m_samplePacks[ m_samplePackSelection ] );
		m_inLoading = false;
		m_return = true;
	} );
	m_transitionTable.setFunction( STATE::NO, [ = ]() {
		m_transitionTable.transition( Events::DIGITAL::BTN_RETURN, false );
		m_transitionTable.triggerCurrentFunction();
	} );
	
	m_transitionTable.setFunction( STATE::PURGE, [ = ]() {
		m_inPurge = true;
		MainMemory::eraseFlash();
		m_inPurge = false;
		m_return = true;
	} );
	
	m_transitionTable.get()[ STATE::MAIN ] = {
		{ Events::DIGITAL::BTN_ENTER, STATE::PURGE },
		{ Events::DIGITAL::BTN_ENTER, STATE::LOAD },
	};
	m_transitionTable.get()[ STATE::LOAD ] = {
		{ Events::DIGITAL::BTN_ENTER, STATE::DECIDE },
		{ Events::DIGITAL::BTN_RETURN, STATE::MAIN }
	};
	m_transitionTable.get()[ STATE::DECIDE ] = {
		{ Events::DIGITAL::BTN_ENTER, STATE::YES },
		{ Events::DIGITAL::BTN_ENTER, STATE::NO },
		{ Events::DIGITAL::BTN_RETURN, STATE::LOAD }
	};
	m_transitionTable.get()[ STATE::PURGE ] = {
		{ Events::DIGITAL::BTN_ENTER, STATE::LOAD },
		{ Events::DIGITAL::BTN_RETURN, STATE::MAIN }
	};
	
	m_transitionTable.setCurrent( STATE::MAIN );
}

void ModuleLoad::enter( Graphics* g )
{
	auto mm = MainMemory::instance();
	mm->subscribe( this );
	m_samplePacks = MainMemory::getSamplePacksFromSD();
	
	std::sort( m_samplePacks.begin(), m_samplePacks.end(), []( const String& a, const String& b ) -> bool {
		String first = a.substring( a.indexOf( '_' ) + 1, a.length());
		String second = b.substring( b.indexOf( '_' ) + 1, b.length());
		int a_int = first.toInt();
		int b_int = second.toInt();
		return a_int < b_int;
	} );
	
	draw_main();
	g->drawWindow( m_top );
	g->drawWindow( m_bottom );
	g->display();
	m_graphics = g;
}

void ModuleLoad::update( Graphics* g, Events::DIGITAL e )
{
	/* decide if to use internal vec size or external size like a list. */
	switch( m_transitionTable.current()) {
		case STATE::LOAD:
			m_transitionTable.setSelectionSize( m_samplePacks.size());
			break;
		case STATE::DECIDE:
			m_transitionTable.setSelectionSize( 2 );
			break;
		default:
			m_transitionTable.setSelectionSize( m_transitionTable.getCurrentVecSize());
			break;
	}
	switch( e ) {
		case Events::DIGITAL::ROTARY_L:
			m_transitionTable.moveLeft();
			m_transitionTable.triggerCurrentFunction();
			break;
		case Events::DIGITAL::ROTARY_R:
			m_transitionTable.moveRight();
			m_transitionTable.triggerCurrentFunction();
			break;
		case Events::DIGITAL::BTN_RETURN:
			if( m_transitionTable.current() == STATE::MAIN ) {
				exit();
				return;
			}
			m_transitionTable.transition( e, false );
			m_transitionTable.triggerCurrentFunction();
			break;
		default:
			if( m_transitionTable.current() == STATE::MAIN )
				m_transitionTable.transition( e, true );
			else
				m_transitionTable.transition( e, false );
			
			m_transitionTable.triggerCurrentFunction();
			if( m_return ) {
				m_return = false;
				exit();
				return;
			}
			break;
	}
	g->clearDisplay();
	g->drawWindow( m_top );
	g->drawWindow( m_bottom );
	g->display();
}

void ModuleLoad::draw_main()
{
	m_top.fillScreen( 0xff );
	m_top.drawWindowBorder( { 2, 2 }, 0, 0x00, 1 );
	m_top.printlnCentered( m_moduleName.c_str());
	
	m_bottom.fillScreen( 0x44 );
	m_bottom.drawWindowBorder( { 4, 4 }, 2, 0xbb, 2 );
	m_bottom.setTextPadding( { 4, 4 } );
	
	auto names = m_transitionTable.allNamesCurrent();
	for( int i = 0; i < names.size(); i++ ) {
		if( i == m_transitionTable.selected()) {
			m_bottom.setTextColor( 0x00 );
			m_bottom.printlnHoverCenter( names[ i ].c_str(), 0xff );
		} else {
			m_bottom.setTextColor( 0xff );
			m_bottom.printlnHCentered( names[ i ].c_str());
		}
	}
}

void ModuleLoad::draw_load()
{
	m_top.fillScreen( 0xff );
	m_top.drawWindowBorder( { 2, 2 }, 0, 0x00, 1 );
	m_top.printlnCentered( m_transitionTable.name( STATE::LOAD ).c_str());
	
	m_bottom.fillScreen( 0x44 );
	m_bottom.drawWindowBorder( { 4, 4 }, 2, 0xbb, 2 );
	m_bottom.setTextPadding( { 4, 4 } );
	
	for( int i = m_transitionTable.selected(); i < ( m_transitionTable.selected() + 7 ) && i < m_samplePacks.size(); i++ ) {
		if( i == m_transitionTable.selected()) {
			m_bottom.setTextColor( 0x00 );
			m_bottom.printlnHoverCenter( m_samplePacks[ i ].c_str(), 0xff );
		} else {
			m_bottom.setTextColor( 0xff );
			m_bottom.printlnHCentered( m_samplePacks[ i ].c_str());
		}
	}
	m_samplePackSelection = m_transitionTable.selected();
}

void ModuleLoad::draw_purge()
{

}

void ModuleLoad::draw_decision()
{
	String name = m_samplePacks[ m_samplePackSelection ];
	m_bottom.fillScreen( 0x44 );
	m_bottom.fillRect( 0, 18, 128, 82, 0xff );
	m_bottom.drawRect( 4, 22, 120, 74, 0x00 );
	m_bottom.setTextColor( 0x00 );
	m_bottom.setCursor( 6, 33 );
	m_bottom.println( "Do You really " );
	m_bottom.setCursor( 6, 43 );
	m_bottom.println( "want to add " );
	m_bottom.setCursor( 6, 53 );
	m_bottom.println( name.c_str());
	m_bottom.setCursor( 6, 63 );
	m_bottom.println( "to flash?" );
	
	if( m_transitionTable.selectedState() == STATE::YES ) {
		m_bottom.fillRect( 15, 70, 40, 20, 0x22 );
		m_bottom.setTextColor( 0xff );
		m_bottom.setCursor( 25, 82 );
		m_bottom.print( "yes" );
		
		m_bottom.drawRect( 70, 70, 40, 20, 0x44 );
		m_bottom.setTextColor( 0x00 );
		m_bottom.setCursor( 82, 82 );
		m_bottom.print( "no" );
		
	} else {
		m_bottom.drawRect( 15, 70, 40, 20, 0x44 );
		m_bottom.setTextColor( 0x00 );
		m_bottom.setCursor( 25, 82 );
		m_bottom.print( "yes" );
		
		m_bottom.fillRect( 70, 70, 40, 20, 0x22 );
		m_bottom.setTextColor( 0xff );
		m_bottom.setCursor( 82, 82 );
		m_bottom.print( "no" );
	}
}

void ModuleLoad::draw_purge_progress( std::pair<uint32_t, uint32_t> progress )
{
	m_top.fillScreen( 0xff );
	m_top.drawWindowBorder( { 2, 2 }, 0, 0x00, 1 );
	m_top.printlnCentered( "PURGING" );
	
	m_bottom.fillScreen( 0x44 );
	m_bottom.drawRect( 12, 50, 100, 8, 0xFF );
	m_bottom.setCursor( 0, 30 );
	m_bottom.printlnHCentered( "purging flash..." );
	
	float percentage = static_cast<float>(progress.first) / static_cast<float>(progress.second);
	
	m_bottom.fillRect( 12, 50, static_cast<int16_t>(percentage * 100.0f), 8, 0xFF );
	m_bottom.setTextColor( 0xff );
	m_bottom.setCursor( 58, 70 );
	m_bottom.print( static_cast<int16_t>(percentage * 100.0f));
	m_bottom.print( "%" );
}

void ModuleLoad::draw_load_progress( std::pair<uint32_t, uint32_t> progress )
{
	m_top.fillScreen( 0xff );
	m_top.drawWindowBorder( { 2, 2 }, 0, 0x00, 1 );
	m_top.printlnCentered( "LOADING" );
	
	m_bottom.fillScreen( 0x44 );
	m_bottom.drawRect( 12, 50, 100, 8, 0xFF );
	m_bottom.setCursor( 0, 30 );
	m_bottom.printlnHCentered( "loading samples..." );
	
	float percentage = static_cast<float>(progress.first) / static_cast<float>(progress.second);
	
	m_bottom.fillRect( 12, 50, static_cast<int16_t>(percentage * 100.0f), 8, 0xFF );
	m_bottom.setTextColor( 0xff );
	String msg = progress.first + " / " + progress.second;
	m_bottom.printlnHCentered( msg.c_str());
	m_bottom.setCursor( 0, 90 );
	m_bottom.printlnHCentered( m_samples[ progress.first ].c_str());
}

void ModuleLoad::exit()
{
	free( m_graphics );
	notify( *this );
}

void ModuleLoad::emit( const std::pair<uint32_t, uint32_t>& src )
{
	if( m_inPurge ) {
		draw_purge_progress( src );
	} else if( m_inLoading ) {
		draw_load_progress( src );
	}
	m_graphics->clearDisplay();
	m_graphics->drawWindow( m_top );
	m_graphics->drawWindow( m_bottom );
	m_graphics->display();
}