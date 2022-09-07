/**************************************************************************************************
 * Copyright (c) 2022. Dennis Oberst                                                              *
 **************************************************************************************************/

#include "module_sound_manager.hpp"

ModuleSoundManager::ModuleSoundManager() : AbstractModule( "Sound Manager" ) {
	auto split_screens = WindowSize::createSplitScreen( UTIL::HORIZONTAL, 0.15 );
	m_top = Window( split_screens.first );
	m_bottom = Window( split_screens.second );
	auto split_screens2 = WindowSize::createSplitScreen( UTIL::VERTICAL, 0.5 );
	m_left = Window( split_screens2.first );
	m_right = Window( split_screens2.second );
	m_mainStates.push_back( STATE::SOUND_MANAGER_MODULE_MAIN );
	m_mainStates.push_back( STATE::SOUND_MANAGER_MODULE_SUB_1 );
	m_mainStates.push_back( STATE::SOUND_MANAGER_MODULE_SUB_2 );
	m_mainStates.push_back( STATE::SOUND_MANAGER_MODULE_SUB_3 );

	m_subStates.push_back( STATE::REMOVE_SAMPLES );
	m_subStates.push_back( STATE::CHANGE_ROUTING );

	bsp_data_Flash.push_back( { "0.wav", 48 } );
	bsp_data_Flash.push_back( { "1.wav", 49 } );
	bsp_data_Flash.push_back( { "2.wav", 50 } );
	bsp_data_Flash.push_back( { "3.wav", 51 } );
	bsp_data_Flash.push_back( { "4.wav", 52 } );
	bsp_data_Flash.push_back( { "5.wav", 53 } );
	bsp_data_Flash.push_back( { "6.wav", 54} );
	bsp_data_Flash.push_back( { "7.wav", 55 } );
	bsp_data_Flash.push_back( { "8.wav", 56 } );
	bsp_data_Flash.push_back( { "9.wav", 57 } );
	bsp_data_Flash.push_back( { "10.wav", 58 } );
	bsp_data_Flash.push_back( { "11.wav", 59 } );
	bsp_data_Flash.push_back( { "12.wav", 60 } );
}

void ModuleSoundManager::enter( Graphics* g ) {
	m_currentSubState = m_subStates.begin();
	m_currentMainState = m_mainStates.begin();
	draw_sound_manager_module_main( g );
}
void ModuleSoundManager::draw_sound_manager_module_sub1( Graphics* g ) {
	g->clearDisplay();

	m_top.fillScreen( 0xff );
	m_top.drawWindowBorder( { 2, 2 }, 0, 0x00, 1 );
	m_top.printlnCentered( "Choose Sample" );

	m_left.fillScreen( 0x44 );
	m_left.drawWindowBorder( { 2, 2 }, 2, 0xbb, 1 );
	m_left.setTextPadding( { 2, 20 } );
	m_right.fillScreen( 0x44 );
	m_right.drawWindowBorder( { 2, 2 }, 2, 0xbb, 1 );
	m_right.setTextPadding( { 2, 20 } );

	if( sizeof bsp_data_Flash >= 8 ) {
		for( int i = m_first_visible_sample; i < m_first_visible_sample + 8; i++ ) {
			if( i == m_selected_sample ) {
				m_left.setTextColor( 0x00 );
				m_left.printlnHoverCenter( bsp_data_Flash[ i ].name.c_str(), 0xff );
				m_right.setTextColor( 0x00 );
				m_right.printlnHoverCenter( MidiNotes[bsp_data_Flash[ i ].midiNote].c_str(), 0xff );
			} else {
				m_left.setTextColor( 0xff );
				m_left.printlnHCentered( bsp_data_Flash[ i ].name.c_str() );
				m_right.setTextColor( 0xff );
				m_right.printlnHCentered( MidiNotes[bsp_data_Flash[ i ].midiNote].c_str());
			}
		}
	} else {
		for( int i = 1; i < sizeof bsp_data_Flash; i++ ) {
			if( i == m_selected_sample ) {
				m_left.setTextColor( 0x00 );
				m_left.printlnHoverCenter( bsp_data_Flash[ i ].name.c_str(), 0xff );
				m_right.setTextColor( 0x00 );
				m_right.printlnHoverCenter( MidiNotes[bsp_data_Flash[ i ].midiNote].c_str(), 0xff );
			} else {
				m_left.setTextColor( 0xff );
				m_left.printlnHCentered( bsp_data_Flash[ i ].name.c_str() );
				m_right.setTextColor( 0xff );
				m_right.printlnHCentered( MidiNotes[bsp_data_Flash[ i ].midiNote].c_str() );
			}
		}
	}
	g->drawWindow( m_left );
	g->drawWindow( m_right );
	g->drawWindow( m_top );
	g->display();
}
void ModuleSoundManager::draw_sound_manager_module_main( Graphics* g ) {
	m_top.fillScreen( 0xff );
	m_top.drawWindowBorder( { 2, 2 }, 0, 0x00, 1 );
	m_top.printlnCentered( "SOUND MANAGER" );

	m_bottom.fillScreen( 0x44 );
	m_bottom.drawWindowBorder( { 4, 4 }, 2, 0xbb, 2 );
	m_bottom.setTextPadding( { 4, 32 } );
	if( m_subStates.begin() == m_currentSubState ) {
		m_bottom.setTextColor( 0x00 );
		m_bottom.printlnHoverCenter( "REMOVE SAMPLEPACK", 0xff );
	} else {
		m_bottom.setTextColor( 0xff );
		m_bottom.printlnHCentered( "REMOVE SAMPLEPACK" );
	}
	if( m_subStates.begin() + 1 == m_currentSubState ) {
		m_bottom.setTextColor( 0x00 );
		m_bottom.printlnHoverCenter( "CHANGE ROUTING", 0xff );
		;
	} else {
		m_bottom.setTextColor( 0xff );
		m_bottom.printlnHCentered( "CHANGE ROUTING" );
	}
	g->drawWindow( m_top );
	g->drawWindow( m_bottom );
	g->display();
}
void ModuleSoundManager::update( Graphics* g, Events::DIGITAL e ) {
	//MAIN -->SOUND MANAGER
	if( m_currentMainState == m_mainStates.begin() ) {
		if( e == Events::DIGITAL::ROTARY_R ) {
			m_currentSubState++;
			if( m_currentSubState == m_subStates.end() ) { m_currentSubState = m_subStates.begin(); }
			draw_sound_manager_module_main( g );
			return;
		}
		if( e == Events::DIGITAL::ROTARY_L ) {
			if( m_currentSubState == m_subStates.begin() ) { m_currentSubState = m_subStates.end(); }
			m_currentSubState--;
			draw_sound_manager_module_main( g );
			return;
		}
		if( e == Events::DIGITAL::BTN_ENTER ) {
			if( m_currentSubState == m_subStates.begin() ) {
				m_currentMainState--;
				draw_load_module_loadbar( g );
				return;
				//lade
			}
			if( m_currentSubState == m_subStates.begin() + 1 ) {
				m_currentMainState++;
				draw_sound_manager_module_sub1( g );
				return;
			}
		}
		if( e == Events::DIGITAL::BTN_RETURN ) {
			exit();
			return;
		}
	}
	if( m_currentMainState == m_mainStates.begin() + 1 ) {
		if( e == Events::DIGITAL::ROTARY_R ) {
			if( m_selected_sample < sizeof bsp_data_Flash ) {
				++m_selected_sample;
				if( sizeof bsp_data_Flash >= 8 ) {
					if( m_selected_sample == m_first_visible_sample + 8 ) { m_first_visible_sample++; }
				}
			} else {
				m_first_visible_sample = 0;
				m_selected_sample = 0;
			}
			draw_sound_manager_module_sub1( g );
			return;
		}
		if( e == Events::DIGITAL::ROTARY_L ) {
			if( m_selected_sample == 1 ) {
				m_selected_sample = sizeof bsp_data_Flash;
				m_first_visible_sample = sizeof bsp_data_Flash - 8;
			} else {
				if( m_selected_sample == m_first_visible_sample ) { m_first_visible_sample--; }
			}
			m_selected_sample--;
			draw_sound_manager_module_sub1( g );
			return;
		}
		if( e == Events::DIGITAL::BTN_RETURN ) {
			m_currentMainState--;
			draw_sound_manager_module_main( g );
			return;
		}
		if( e == Events::DIGITAL::BTN_ENTER ) {
			m_currentMainState++;
			m_key=bsp_data_Flash[m_selected_sample].midiNote;
			m_note=m_key;
			draw_key_mapping(g);
			return;
		}
	}
	if( m_currentMainState == m_mainStates.begin() + 2 ) {
		if( e == Events::DIGITAL::ROTARY_R ) {
			m_key--;
			m_note--;
			if( m_key == 0 ) {
				m_key = 24;
				return;
			}
			draw_key_mapping( g );
			return;
		}
		if( e == Events::DIGITAL::ROTARY_L ) {
			m_key++;
			m_note++;
			if( m_key == 24 ) { m_key = 0; }
			draw_key_mapping( g );
			return;
		}
	}
}
void ModuleSoundManager::draw_load_module_loadbar( Graphics* g ) {
	m_bottom.fillScreen( 0x00 );
	m_bottom.drawRect( 12, 50, 100, 8, 0xFF );
	for( int i = 0; i < 20; i++ ) {
		m_bottom.fillRect( 12, 50, i * 5, 8, 0xFF );
		m_bottom.fillRect( 20, 60, 100, 20, 0x00 );
		m_bottom.setTextColor( 0xff );
		m_bottom.setCursor( 58, 70 );
		m_bottom.print( i * 5 );
		m_bottom.print( "%" );
		g->drawWindow( m_bottom );
		g->display();
		delay( 20 );
	}
	exit();
}
void ModuleSoundManager::draw_key_mapping( Graphics* g ) {
	m_top.fillScreen( 0xff );
	m_top.setTextColor( 0x44 );
	m_top.printlnCentered( "MIDI" );
	m_bottom.fillScreen( 0x00 );
	if(m_key>=24) {
		for( int i = 1; i < 4; i++ ) {
			if( m_key >= i * 24 && m_key < (i+1) * 24 -1 ) {
				m_key = m_key - i * 24;
				break;
			}
		}
	}
	int whiteKeys[ 14 ] = { 1, 3, 5, 6, 8, 10, 12, 13, 15, 17, 18, 20, 22, 24 };
	int blackKeys[ 10 ] = { 2, 4, 7, 9, 11, 14, 16, 19, 21, 23 };
	int wCount = 0;
	for( int i = 8; i <= 116; i = i + 8 ) {
		if( whiteKeys[ wCount ] == m_key ) {
			m_bottom.drawRect( i, 5, 8, 50, 0xbb );
			m_bottom.fillRect( i + 1, 6, 6, 48, 0xbb );
		} else {
			m_bottom.drawRect( i, 5, 8, 50, 0x44 );
			m_bottom.fillRect( i + 1, 6, 6, 48, 0xff );
		}
		wCount++;
	}
	int count = 0;
	int bcount = 0;
	for( int i = 8; i < 116; i = i + 8 ) {
		if( count != 2 && count != 6 && count != 9 && count != 13 ) {
			if( blackKeys[ bcount ] == m_key ) {
				m_bottom.drawRect( i + 5, 5, 6, 30, 0x44 );
				m_bottom.fillRect( i + 6, 6, 4, 28, 0xbb );
			} else {
				m_bottom.drawRect( i + 5, 5, 6, 30, 0x44 );
				m_bottom.fillRect( i + 6, 6, 4, 28, 0x11 );
			}
			bcount++;
		}
		count++;
	}
	m_bottom.setCursor(62,70);
	m_bottom.setTextColor(0xff);
	m_bottom.println(MidiNotes[m_note].c_str());
	g->drawWindow( m_top );
	g->drawWindow( m_bottom );
	g->display();
}

/*if( m_currentMainState == m_mainStates.begin()+2 ) {
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
	}*/
void ModuleSoundManager::exit() { notify( *this ); }
