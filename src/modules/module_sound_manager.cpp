/**************************************************************************************************
 * Copyright (c) 2022. Dennis Oberst                                                              *
 **************************************************************************************************/

#include "module_sound_manager.hpp"

ModuleSoundManager::ModuleSoundManager() : AbstractModule( "Sound Manager" ) {
	auto split_screens = WindowSize::createSplitScreen( UTIL::HORIZONTAL, 0.15 );
	m_top = Window( split_screens.first );
	m_bottom = Window( split_screens.second );
	m_mainStates.push_back( STATE::SOUND_MANAGER_MODULE_MAIN );
	m_mainStates.push_back( STATE::SOUND_MANAGER_MODULE_SUB_1 );
	m_mainStates.push_back( STATE::SOUND_MANAGER_MODULE_SUB_2 );
	m_mainStates.push_back( STATE::SOUND_MANAGER_MODULE_SUB_3 );

	m_subStates.push_back( STATE::VOLUME );
	m_subStates.push_back( STATE::FILTER1 );
	m_subStates.push_back( STATE::FILTER2 );
	m_subStates.push_back( STATE::REVERSE );
	m_subStates.push_back( STATE::CUT );
}

void ModuleSoundManager::enter( Graphics* g ) {
	m_currentMainState = m_mainStates.begin();
	draw_sound_manager_module_main( g );
}
void ModuleSoundManager::draw_sound_manager_module_main( Graphics* g ) {
	m_top.fillScreen( 0xff );
	m_top.drawWindowBorder( { 2, 2 }, 0, 0x00, 1 );
	m_top.printlnCentered( "Choose Samplepack" );
	m_bottom.fillScreen( 0x44 );
	m_bottom.drawWindowBorder( { 4, 4 }, 2, 0xbb, 2 );
	m_bottom.setTextPadding( { 4, 4 } );
	if( sizeof bsp_data_Flash2 / sizeof bsp_data_Flash2[ 0 ] >= 7 ) {
		for( int i = m_first_visible_sample; i < m_first_visible_sample + 7; i++ ) {
			if( i == m_selected_pack ) {
				m_bottom.setTextColor( 0x00 );
				m_bottom.printlnHoverCenter( bsp_data_Flash2[ i ][ 0 ].c_str(), 0xff );
			} else {
				m_bottom.setTextColor( 0xff );
				m_bottom.printlnHCentered( bsp_data_Flash2[ i ][ 0 ].c_str() );
			}
		}
	} else {
		for( int i = 0; i < sizeof bsp_data_Flash2 / sizeof bsp_data_Flash2[ 0 ]; i++ ) {
			if( i == m_selected_pack ) {
				m_bottom.setTextColor( 0x00 );
				m_bottom.printlnHoverCenter( bsp_data_Flash2[ i ][ 0 ].c_str(), 0xff );
			} else {
				m_bottom.setTextColor( 0xff );
				m_bottom.printlnHCentered( bsp_data_Flash2[ i ][ 0 ].c_str() );
			}
		}
	}
	g->drawWindow( m_top );
	g->drawWindow( m_bottom );
	g->display();
}
void ModuleSoundManager::draw_sound_manager_module_sub1( Graphics* g ) {
	g->clearDisplay();

	m_top.fillScreen( 0xff );
	m_top.drawWindowBorder( { 2, 2 }, 0, 0x00, 1 );
	m_top.printlnCentered( "Choose Sample" );

	m_bottom.fillScreen( 0x44 );
	m_bottom.drawWindowBorder( { 4, 4 }, 2, 0xbb, 2 );
	m_bottom.setTextPadding( { 4, 4 } );

	if( sizeof bsp_data_Flash2[ m_selected_pack ] / sizeof bsp_data_Flash2[ m_selected_pack ][ 0 ] >= 7 ) {
		for( int i = m_first_visible_sample; i < m_first_visible_sample + 7; i++ ) {
			if( i == m_selected_sample ) {
				m_bottom.setTextColor( 0x00 );
				m_bottom.printlnHoverCenter( bsp_data_Flash2[ m_selected_pack ][ i ].c_str(), 0xff );
			} else {
				m_bottom.setTextColor( 0xff );
				m_bottom.printlnHCentered( bsp_data_Flash2[ m_selected_pack ][ i ].c_str() );
			}
		}
	} else {
		for( int i = 1; i < sizeof bsp_data_Flash2[ m_selected_pack ] / sizeof bsp_data_Flash2[ m_selected_pack ][ 0 ];
		     i++ ) {
			if( i == m_selected_sample ) {
				m_bottom.setTextColor( 0x00 );
				m_bottom.printlnHoverCenter( bsp_data_Flash2[ m_selected_pack ][ i ].c_str(), 0xff );
			} else {
				m_bottom.setTextColor( 0xff );
				m_bottom.printlnHCentered( bsp_data_Flash2[ m_selected_pack ][ i ].c_str() );
			}
		}
	}
	g->drawWindow( m_top );
	g->drawWindow( m_bottom );
	g->display();
}
void ModuleSoundManager::draw_sound_manager_module_sub2( Graphics* g ) {
	m_top.fillScreen( 0xff );
	m_top.drawWindowBorder( { 2, 2 }, 0, 0x00, 1 );
	m_top.printlnCentered( "CHOOSE EFFECT" );

	m_bottom.fillScreen( 0x44 );
	m_bottom.drawWindowBorder( { 4, 4 }, 2, 0xbb, 2 );
	m_bottom.setTextPadding( { 4, 4 } );
	if( m_subStates.begin() == m_currentSubState ) {
		m_bottom.setTextColor( 0x00 );
		m_bottom.printlnHoverCenter( "CHANGE VOLUME", 0xff );
	} else {
		m_bottom.setTextColor( 0xff );
		m_bottom.printlnHCentered( "CHANGE VOLUME" );
	}
	if( m_subStates.begin() + 1 == m_currentSubState ) {
		m_bottom.setTextColor( 0x00 );
		m_bottom.printlnHoverCenter( "FILTER1", 0xff );
		;
	} else {
		m_bottom.setTextColor( 0xff );
		m_bottom.printlnHCentered( "FILTER1" );
	}
	if( m_subStates.begin() + 2 == m_currentSubState ) {
		m_bottom.setTextColor( 0x00 );
		m_bottom.printlnHoverCenter( "FILTER2", 0xff );
		;
	} else {
		m_bottom.setTextColor( 0xff );
		m_bottom.printlnHCentered( "FILTER2" );
	}
	if( m_subStates.begin() + 3 == m_currentSubState ) {
		m_bottom.setTextColor( 0x00 );
		m_bottom.printlnHoverCenter( "REVERSE", 0xff );
		;
	} else {
		m_bottom.setTextColor( 0xff );
		m_bottom.printlnHCentered( "REVERSE" );
	}
	if( m_subStates.begin() + 4 == m_currentSubState ) {
		m_bottom.setTextColor( 0x00 );
		m_bottom.printlnHoverCenter( "CUT", 0xff );
		;
	} else {
		m_bottom.setTextColor( 0xff );
		m_bottom.printlnHCentered( "CUT" );
	}
	g->drawWindow( m_top );
	g->drawWindow( m_bottom );
	g->display();
}
void ModuleSoundManager::draw_sound_manager_module_sub3( Graphics* g ) {
	g->clearDisplay();
	m_top.fillScreen( 0xff );
	m_top.drawWindowBorder( { 2, 2 }, 0, 0x00, 1 );
	m_bottom.fillScreen( 0x44 );
	m_bottom.drawWindowBorder( { 4, 4 }, 2, 0xbb, 2 );
	m_bottom.setTextPadding( { 4, 4 } );
	if( m_currentSubState == m_subStates.begin() ) {
		m_top.printlnCentered( "CHANGE VOLUME" );
		m_bottom.drawRect( 12, 12, 8, 86, 0xFF );
		m_bottom.drawRect( 24, 12, 90, 86, 0xFF );
		m_lastValue = analogRead( 14 );
		float p = m_lastValue / 1023 * 100;
		int x = 86 * p / 100;
		Serial.println( m_lastValue );
		Serial.println( x );
		m_bottom.fillRect( 12, 12 + ( 86 - x ), 6, x, 0xFF );
	} else if( m_currentSubState == m_subStates.begin() + 1 ) {
		m_top.printlnCentered( "FILTER 1" );
	} else if( m_currentSubState == m_subStates.begin() + 2 ) {
		m_top.printlnCentered( "FILTER 2" );
	} else if( m_currentSubState == m_subStates.begin() + 3 ) {
		m_top.printlnCentered( "REVERSE SAMPLE" );
	}
	else if( m_currentSubState == m_subStates.begin() + 4 ) {
		m_top.printlnCentered( "CUT" );
		
	}
	g->drawWindow( m_top );
	g->drawWindow( m_bottom );
	g->display();
}
void ModuleSoundManager::update( Graphics* g, Events::DIGITAL e ) {
	//MAIN --> CHOOSE SAMPLEPACK
	if( m_currentMainState == m_mainStates.begin() ) {
		if( e == Events::DIGITAL::ROTARY_L ) {
			if( m_selected_pack == 0 ) {
				m_selected_pack = sizeof bsp_data_Flash2 / sizeof bsp_data_Flash2[ 0 ];
				m_first_visible_sample = ( sizeof bsp_data_Flash2 / sizeof bsp_data_Flash2[ 0 ] ) - 4;
			} else {
				if( m_selected_sample == m_first_visible_sample ) { m_first_visible_sample--; }
			}
			Serial.println( m_selected_pack );
			m_selected_pack--;
			draw_sound_manager_module_main( g );
			return;
		}
		if( e == Events::DIGITAL::ROTARY_R ) {
			if( m_selected_pack < sizeof bsp_data_Flash2 / sizeof bsp_data_Flash2[ 0 ] - 1 ) {
				Serial.println( m_selected_pack );
				++m_selected_pack;
				if( m_selected_pack == m_first_visible_sample + 7 ) { m_first_visible_sample++; }
			} else {
				m_first_visible_sample = 0;
				m_selected_pack = 0;
			}
			draw_sound_manager_module_main( g );
			return;
		}
		if( e == Events::DIGITAL::BTN_RETURN ) {
			exit();
			return;
		}
		if( e == Events::DIGITAL::BTN_ENTER ) {
			m_currentMainState++;
			m_selected_sample = 1;
			m_first_visible_sample = 1;
			draw_sound_manager_module_sub1( g );
			return;
		}
	}
	//SUB1 --> CHOOSE SAMPLE
	if( m_currentMainState == m_mainStates.begin() + 1 ) {
		if( e == Events::DIGITAL::ROTARY_R ) {
			if( m_selected_sample <
			    sizeof bsp_data_Flash2[ m_selected_pack ] / sizeof bsp_data_Flash2[ m_selected_pack ][ 0 ] - 1 ) {
				++m_selected_sample;
				if( sizeof bsp_data_Flash2[ m_selected_pack ] / sizeof bsp_data_Flash2[ m_selected_pack ][ 0 ] >= 7 ) {
					if( m_selected_sample == m_first_visible_sample + 7 ) { m_first_visible_sample++; }
				}
			} else {
				m_first_visible_sample = 1;
				m_selected_sample = 1;
			}
			draw_sound_manager_module_sub1( g );
			return;
		}
		if( e == Events::DIGITAL::ROTARY_L ) {
			if( m_selected_sample == 1 ) {
				m_selected_sample =
				    sizeof bsp_data_Flash2[ m_selected_pack ] / sizeof bsp_data_Flash2[ m_selected_pack ][ 0 ];
				m_first_visible_sample =
				    ( sizeof bsp_data_Flash2[ m_selected_pack ] / sizeof bsp_data_Flash2[ m_selected_pack ][ 0 ] ) - 7;
			} else {
				if( m_selected_sample == m_first_visible_sample ) { m_first_visible_sample--; }
			}
			m_selected_sample--;
			draw_sound_manager_module_sub1( g );
			return;
		}
		if( e == Events::DIGITAL::BTN_ENTER ) {
			m_currentMainState++;
			m_currentSubState = m_subStates.begin();
			draw_sound_manager_module_sub2( g );
			return;
		}
		if( e == Events::DIGITAL::BTN_RETURN ) {
			m_currentMainState--;
			draw_sound_manager_module_sub1( g );
			m_selected_sample = 0;
			return;
		}
	}
	//SUB2 --> CHOOSE EFFECT
	if( m_currentMainState == m_mainStates.begin() + 2 ) {
		if( e == Events::DIGITAL::ROTARY_R ) {
			m_currentSubState++;
			if( m_currentSubState == m_subStates.end() ) { m_currentSubState = m_subStates.begin(); }
			draw_sound_manager_module_sub2( g );
			return;
		}
		if( e == Events::DIGITAL::ROTARY_L ) {
			if( m_currentSubState == m_subStates.begin() ) { m_currentSubState = m_subStates.end(); }
			m_currentSubState--;
			draw_sound_manager_module_sub2( g );
			return;
		}
		if( e == Events::DIGITAL::BTN_ENTER ) {
			m_currentMainState++;
			draw_sound_manager_module_sub3( g );
			return;
		}
		if( e == Events::DIGITAL::BTN_RETURN ) {
			m_currentSubState = m_subStates.begin();
			m_currentMainState--;
			draw_sound_manager_module_sub1( g );
			return;
		}
	}
	//SUB3 --> EFFECTS
	if( m_currentMainState == m_mainStates.begin() + 3 ) {
		if( e == Events::DIGITAL::BTN_RETURN ) {
			m_currentSubState = m_subStates.begin();
			m_currentMainState--;
			draw_sound_manager_module_sub2( g );
			return;
		}
		if( analogRead( 14 ) != m_lastValue ) {
			m_lastValue = analogRead( 14 );
			draw_sound_manager_module_sub3( g );
			return;
		}
	}
}

void ModuleSoundManager::exit() { notify( *this ); }
