#include "module_load.hpp"

using namespace lbs;

ModuleLoad::ModuleLoad() : AbstractModule( "Load Samples" ) {
	auto split_screens = WindowSize::createSplitScreen( UTIL::HORIZONTAL, 0.15 );
	m_topNav = Window( split_screens.first );
	m_bottom = Window( split_screens.second );
	m_mainStates.push_back( STATE::LOAD_MODULE_MAIN );
	m_mainStates.push_back( STATE::LOAD_MODULE_SUB_1 );
	m_mainStates.push_back( STATE::LOAD_MODULE_SUB_2 );
	m_mainStates.push_back( STATE::LOAD_MODULE_SUB_2b );
	m_mainStates.push_back( STATE::LOAD_MODULE_SUB_3 );
	m_current_mainStates = m_mainStates.begin();

	m_loadMenu.push_back( STATE::SAMPLE );
	m_loadMenu.push_back( STATE::SAMPLEPACK );
	m_current_subStates = m_loadMenu.begin();

	m_AddOrRemove.push_back( STATE::ADD_SAMPLE_TO_FLASH );
	m_AddOrRemove.push_back( STATE::REMOVE_SAMPLE_FROM_FLASH );
}
void ModuleLoad::enter( Graphics* g ) { draw_load_module( g ); }
void ModuleLoad::draw_load_module( Graphics* g ) {
	m_topNav.fillScreen( 0xff );
	m_topNav.drawWindowBorder( { 2, 2 }, 0, 0x00, 1 );
	m_topNav.printlnCentered( "LOAD SAMPLES" );

	m_bottom.fillScreen( 0x44 );
	m_bottom.drawWindowBorder( { 4, 4 }, 2, 0xbb, 2 );
	m_bottom.setTextPadding( { 4, 30 } );
	if( m_loadMenu.begin() == m_current_subStates ) {
		m_bottom.setTextColor( 0x00 );
		m_bottom.printlnHoverCenter( "SAMPLE", 0xff );
	} else {
		m_bottom.setTextColor( 0xff );
		m_bottom.printlnHCentered( "SAMPLE" );
	}
	if( m_loadMenu.begin() + 1 == m_current_subStates ) {
		m_bottom.setTextColor( 0x00 );
		m_bottom.printlnHoverCenter( "SAMPLEPACK", 0xff );
		;
	} else {
		m_bottom.setTextColor( 0xff );
		m_bottom.printlnHCentered( "SAMPLEPACK" );
	}
	g->drawWindow( m_topNav );
	g->drawWindow( m_bottom );
	g->display();
}
void ModuleLoad::update( Graphics* g, Events::DIGITAL e ) {
	//LOAD_MODULE_MAIN
	//RETURN TO MAIN MENU
	if( e == Events::DIGITAL::BTN_RETURN && m_current_mainStates == m_mainStates.begin() ) {
		Serial.println( "MAIN MENU" );
		exit();
		return;
	}
	// CHOSE SAMPLE OR SAMPLEPACK WITH ROTARY
	if( ( e == Events::DIGITAL::ROTARY_R || e == Events::DIGITAL::ROTARY_L ) &&
	    m_current_mainStates == m_mainStates.begin() ) {
		++m_current_subStates;
		if( m_current_subStates == m_loadMenu.end() ) m_current_subStates = m_loadMenu.begin();
	}
	//GO TO LOAD_MODULE_SUB1 with LOAD SAMPLE
	if( e == Events::DIGITAL::BTN_ENTER && m_current_mainStates == m_mainStates.begin() &&
	    m_current_subStates == m_loadMenu.begin() ) {
		Serial.println( "LOAD SAMPLE" );
		m_current_mainStates++;
		m_current_subStates = m_AddOrRemove.begin();
		m_isSample = true;
		draw_load_module_sub1( g );
		return;
	}
	//GO TO LOAD_MODULE_SUB1 with LOAD SAMPLEPACK
	if( e == Events::DIGITAL::BTN_ENTER && m_current_mainStates == m_mainStates.begin() &&
	    m_current_subStates == m_loadMenu.begin() + 1 ) {
		m_current_mainStates++;
		m_current_subStates = m_AddOrRemove.begin();
		m_isSample = false;
		Serial.println( "LOAD SAMPLEPACK" );
		draw_load_module_sub1( g );
		return;
	}

	//LOAD_MODULE_SUB_1
	// RETURN TO LOAD_MODULE_MAIN
	if( e == Events::DIGITAL::BTN_RETURN && m_current_mainStates == m_mainStates.begin() + 1 ) {
		m_current_mainStates--;
		m_current_subStates = m_loadMenu.begin();
		draw_load_module( g );
		return;
	}
	//CHOSE ADD OR REMOVE SAMPLE WITH ROTARY
	if( ( e == Events::DIGITAL::ROTARY_R || e == Events::DIGITAL::ROTARY_L ) &&
	    m_current_mainStates == m_mainStates.begin() + 1 ) {
		++m_current_subStates;
		if( m_current_subStates == m_AddOrRemove.end() ) { m_current_subStates = m_AddOrRemove.begin(); }
		draw_load_module_sub1( g );
	}
	if( e == Events::DIGITAL::BTN_ENTER && m_current_mainStates == m_mainStates.begin() + 1 &&
	    m_current_subStates == m_AddOrRemove.begin() ) {
		m_current_mainStates++;
		m_isAdd = true;
		draw_load_module_sub2( g );
		return;
	}
	if( e == Events::DIGITAL::BTN_ENTER && m_current_mainStates == m_mainStates.begin() + 1 &&
	    m_current_subStates == m_AddOrRemove.begin() + 1 ) {
		m_current_mainStates++;
		m_isAdd = false;
		m_first_visible_sample = 0;
		m_selected_sample = 0;
		draw_load_module_sub2( g );
		return;
	}
	//SUB 2   CHOOSE SAMPLEPACK TO ADD/REMOVE WITH ROTARY
	if( e == Events::DIGITAL::ROTARY_R && m_current_mainStates == m_mainStates.begin() + 2 ) {
		if( m_selected_pack < sizeof bsp_data_Flash2 / sizeof bsp_data_Flash2[ 0 ] - 1 ) {
			Serial.println( sizeof bsp_data_Flash2 / sizeof bsp_data_Flash2[ 0 ] );
			++m_selected_pack;
			if( m_selected_pack == m_first_visible_sample + 7 ) { m_first_visible_sample++; }
		} else {
			m_first_visible_sample = 0;
			m_selected_pack = 0;
		}
		draw_load_module_sub2( g );
		return;
	}
	if( e == Events::DIGITAL::ROTARY_L && m_current_mainStates == m_mainStates.begin() + 2 ) {
		if( m_selected_pack == 0 ) {
			m_selected_pack = sizeof bsp_data_Flash2 / sizeof bsp_data_Flash2[ 0 ];
			m_first_visible_sample = ( sizeof bsp_data_Flash2 / sizeof bsp_data_Flash2[ 0 ] ) - 4;
		} else {
			if( m_selected_pack == m_first_visible_sample ) { m_first_visible_sample--; }
		}
		m_selected_pack--;
		draw_load_module_sub2( g );
		return;
	}
	if( e == Events::DIGITAL::BTN_RETURN && m_current_mainStates == m_mainStates.begin() + 2 ) {
		m_current_mainStates--;
		if(m_isAdd) {
			m_current_subStates = m_AddOrRemove.begin();
		} else {
			m_current_subStates = m_AddOrRemove.begin()+1;
		}
		draw_load_module_sub1( g );
		return;
	}
	if( e == Events::DIGITAL::BTN_ENTER && m_current_mainStates == m_mainStates.begin() + 2 ) {
			if( m_isSample ) {
				m_current_mainStates++;
				m_selected_sample = 1;
				m_first_visible_sample = 1;
				Serial.println( "CHOOSE SAMPLE OUT OF SAMPLEPACK" );
				draw_load_module_sub2b( g );
			}
		    else {
			    if(m_isAdd)
			    {
				    m_current_mainStates += 2;
				    Serial.println( "CHOOSE START CHOOSE_NOTE" );
				    draw_load_module_sub3( g );
			    }
			    else //REMOVE
			    {
				    m_selected_sample=0;
				    m_selected_pack=0;
				    m_first_visible_sample=0;
				    m_current_mainStates = m_mainStates.begin();
				    m_current_subStates = m_loadMenu.begin();
				    draw_load_module( g );
			    }
			}
		return;
	}

	//SUB 2B   CHOOSE SAMPLE TO ADD/REMOVE WITH ROTARY
	if( e == Events::DIGITAL::ROTARY_R && m_current_mainStates == m_mainStates.begin() + 3 ) {
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
		draw_load_module_sub2b( g );
		return;
	}
	if( e == Events::DIGITAL::ROTARY_L && m_current_mainStates == m_mainStates.begin() + 3 ) {
		if( m_selected_sample == 1 ) {
			m_selected_sample =
			    sizeof bsp_data_Flash2[ m_selected_pack ] / sizeof bsp_data_Flash2[ m_selected_pack ][ 0 ];
			m_first_visible_sample =
			    ( sizeof bsp_data_Flash2[ m_selected_pack ] / sizeof bsp_data_Flash2[ m_selected_pack ][ 0 ] ) - 7;
		} else {
			if( m_selected_sample == m_first_visible_sample ) { m_first_visible_sample--; }
		}
		m_selected_sample--;
		draw_load_module_sub2b( g );
		return;
	}
	if( e == Events::DIGITAL::BTN_RETURN && m_current_mainStates == m_mainStates.begin() + 3 ) {
		m_current_mainStates--;
		m_selected_sample = 0;
		m_first_visible_sample = 0;
		draw_load_module_sub2( g );
		return;
	}
	if( e == Events::DIGITAL::BTN_ENTER && m_current_mainStates == m_mainStates.begin() + 3 ) {
		if( m_isAdd) {
			m_current_mainStates++;
			Serial.println( "CHOOSE START CHOOSE_NOTE" );
			draw_load_module_sub3( g );
		}
		else {
			m_current_subStates = m_loadMenu.begin();
			m_current_mainStates = m_mainStates.begin();
			draw_load_module( g );
		}
		return;
	}
	//LOAD_MODULE_SUB_3:
	//CHOSE NOTE POSITION FOR SAMPLE
	//MOVE KEY WITH ROTARY
	if( e == Events::DIGITAL::ROTARY_R && m_current_mainStates == m_mainStates.begin() + 4 ) {
		m_key--;
		if( m_key == 0 ) {
			m_key = 24;
			return;
		}
		draw_load_module_sub3( g );
		return;
	}
	if( e == Events::DIGITAL::ROTARY_L && m_current_mainStates == m_mainStates.begin() + 4 ) {
		m_key++;
		if( m_key == 24 ) { m_key = 0; }
		draw_load_module_sub3( g );
		return;
	}
	if( e == Events::DIGITAL::BTN_ENTER && m_current_mainStates == m_mainStates.begin() + 4 ) {
		Serial.println("IS ADDED");
		m_current_mainStates = m_mainStates.begin();
		m_current_subStates = m_loadMenu.begin();
		draw_load_module( g );
		return;
	}
	if( e == Events::DIGITAL::BTN_RETURN && m_current_mainStates == m_mainStates.begin() + 4 ) {
		if( !m_isSample ) {
			m_current_mainStates -= 2;
			m_selected_sample = 0;
			m_first_visible_sample = 0;
			draw_load_module_sub2( g );
		} else {
			m_current_mainStates--;
			draw_load_module_sub2b( g );
		}
		return;
	}
	if( m_current_mainStates == m_mainStates.begin() ) draw_load_module( g );
}
void ModuleLoad::exit() { notify( *this ); }
void ModuleLoad::draw_load_module_sub1( Graphics* g ) {
	g->clearDisplay();
	m_topNav.fillScreen( 0xff );
	m_topNav.drawWindowBorder( { 2, 2 }, 0, 0x00, 1 );
	if( m_isSample ) {
		m_topNav.printlnCentered( "LOAD SAMPLE" );
	} else {
		m_topNav.printlnCentered( "LOAD SAMPLEPACK" );
	}
	auto split_screens2 = WindowSize::createSplitScreen( UTIL::HORIZONTAL, 0.5 );
	Window m_Add = Window( split_screens2.first );
	Window m_Remove = Window( split_screens2.second );

	m_bottom.fillScreen( 0x44 );
	//Mit Rotary eine der Boxen auswählen
	if( m_current_subStates == m_AddOrRemove.begin() ) {
		m_Add.fillScreen( 0XFF );
		m_Add.drawWindowBorder( { 4, 4 }, 2, 0x00, 2 );
		m_Add.setTextPadding( { 4, 4 } );
		m_Add.setTextColor( 0x00 );
		m_Add.printlnHCentered( "ADD TO FLASH" );
	} else {
		m_Add.fillScreen( 0X00 );
		m_Add.drawWindowBorder( { 4, 4 }, 2, 0xFF, 2 );
		m_Add.setTextPadding( { 4, 4 } );
		m_Add.setTextColor( 0xFF );
		m_Add.printlnHCentered( "ADD TO FLASH" );
	}
	if( m_current_subStates == m_AddOrRemove.begin() + 1 ) {
		m_Remove.fillScreen( 0XFF );
		m_Remove.drawWindowBorder( { 4, 4 }, 2, 0x00, 2 );
		m_Remove.setTextPadding( { 8, 4 } );
		m_Remove.setTextColor( 0x00 );
		m_Remove.printlnHCentered( "REMOVE FROM FLASH" );
	} else {
		m_Remove.fillScreen( 0X00 );
		m_Remove.drawWindowBorder( { 4, 4 }, 2, 0xFF, 2 );
		m_Remove.setTextPadding( { 4, 4 } );
		m_Remove.setTextColor( 0xFF );
		m_Remove.printlnHCentered( "REMOVE FROM FLASH" );
	}

	g->drawWindow( m_topNav );
	g->drawWindow( m_Add );
	g->drawWindow( m_Remove );
	g->display();
}
void ModuleLoad::draw_load_module_sub2( Graphics* g ) {
	g->clearDisplay();

	m_topNav.fillScreen( 0xff );
	m_topNav.drawWindowBorder( { 2, 2 }, 0, 0x00, 1 );
	if( m_isAdd ) {
		m_topNav.printlnCentered( "ADD TO FLASH" );
	} else {
		m_topNav.printlnCentered( "REMOVE FROM FLASH" );
	}
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

	g->drawWindow( m_topNav );
	g->drawWindow( m_bottom );
	g->display();
}
void ModuleLoad::draw_load_module_sub2b( Graphics* g ) {
	g->clearDisplay();

	m_topNav.fillScreen( 0xff );
	m_topNav.drawWindowBorder( { 2, 2 }, 0, 0x00, 1 );
	if( m_isAdd ) {
		m_topNav.printlnCentered( "ADD TO FLASH" );
	} else {
		m_topNav.printlnCentered( "REMOVE FROM FLASH" );
	}
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
	g->drawWindow( m_topNav );
	g->drawWindow( m_bottom );
	g->display();
}
void ModuleLoad::draw_load_module_sub3( Graphics* g ) {
	m_topNav.fillScreen( 0xff );
	m_topNav.setTextColor( 0x44 );
	m_topNav.printlnCentered( "MIDI" );
	m_bottom.fillScreen( 0x00 );

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
	g->drawWindow( m_topNav );
	g->drawWindow( m_bottom );
	g->display();
}
