#include "module_audio.hpp"

#include <utility>

using namespace lbs;

ModuleAudio::ModuleAudio( std::shared_ptr<Audio> audio )
	: AbstractModule( "Audio" ), m_audio( std::move( audio ))
{
	auto split_screens = WindowSize::createSplitScreen( UTIL::HORIZONTAL, 0.15f );
	m_top = Window( split_screens.first );
	m_bottom = Window( split_screens.second );
	auto split_screens2 = WindowSize::createSplitScreen( UTIL::VERTICAL, 0.5 );
	m_left = Window( split_screens2.first );
	m_right = Window( split_screens2.second );
	
	m_mainStates.push_back( STATE::AUDIO_MODULE_MAIN );
	m_mainStates.push_back( STATE::AUDIO_MODULE_SUB_1 );
	m_mainStates.push_back( STATE::AUDIO_MODULE_SUB_2 );
	m_mainStates.push_back( STATE::AUDIO_MODULE_SUB_3 );
	
	m_channels.push_back( STATE::CHANNEL_MASTER );
	m_channels.push_back( STATE::CHANNEL_1 );
	m_channels.push_back( STATE::CHANNEL_2 );
	m_channels.push_back( STATE::CHANNEL_3 );
	m_channels.push_back( STATE::CHANNEL_4 );
	
	m_effects.push_back( STATE::EFFECT_VOLUME );
	m_effects.push_back( STATE::EFFECT_FILTER );
	m_effects.push_back( STATE::EFFECT_DELAY );
	m_effects.push_back( STATE::EFFECT_COMPRESSOR );
	
	m_detailFilter.push_back( STATE::IS_HIGHPASS );
	m_detailFilter.push_back( STATE::FREQUENCY );
	
	m_detailDelay.push_back( STATE::TIME );
	m_detailDelay.push_back( STATE::FEEDBACK );
	
	m_detailCompressor.push_back( STATE::GAIN );
	m_detailCompressor.push_back( STATE::RATIO );
	m_detailCompressor.push_back( STATE::TRESHOLD );
	
	highpass = true;
	
	m_transitionTable.setName( STATE::MAIN, m_moduleName.c_str());
	m_transitionTable.setName( STATE::MASTER, "MASTER" );
	m_transitionTable.setName( STATE::CHANNEL_1, "CHANNEL 1" );
	m_transitionTable.setName( STATE::CHANNEL_2, "CHANNEL 2" );
	m_transitionTable.setName( STATE::CHANNEL_3, "CHANNEL 3" );
	m_transitionTable.setName( STATE::CHANNEL_4, "CHANNEL 4" );
	
	m_transitionTable.setFunction( STATE::MAIN, [ = ]() {
		this->draw_main();
	} );
	
	m_transitionTable.setFunction( STATE::MASTER, [ = ]() {
		this->draw_master( Events::Analog::POTS::LAST );
	} );
	
	m_transitionTable.setFunction( STATE::CHANNEL_1, [ = ]() {
		//this->draw_channel1();
	} );
	
	m_transitionTable.setFunction( STATE::CHANNEL_2, [ = ]() {
		//this->draw_channel2();
	} );
	
	m_transitionTable.setFunction( STATE::CHANNEL_3, [ = ]() {
		//this->draw_channel3();
	} );
	
	m_transitionTable.setFunction( STATE::CHANNEL_4, [ = ]() {
		//this->draw_channel4();
	} );
	
	m_transitionTable.get()[ STATE::MAIN ] = {
		{ Events::DIGITAL::BTN_ENTER, STATE::MASTER },
		{ Events::DIGITAL::BTN_ENTER, STATE::CHANNEL_1 },
		{ Events::DIGITAL::BTN_ENTER, STATE::CHANNEL_2 },
		{ Events::DIGITAL::BTN_ENTER, STATE::CHANNEL_3 },
		{ Events::DIGITAL::BTN_ENTER, STATE::CHANNEL_4 },
	};
	m_transitionTable.get()[ STATE::MASTER ] = {
		{ Events::DIGITAL::BTN_RETURN, STATE::MAIN },
	};
	m_transitionTable.setCurrent( STATE::MAIN );
	
}

ModuleAudio::~ModuleAudio()
{ }

void ModuleAudio::enter( Graphics* g )
{
	g->clearDisplay();
	draw_main();
	g->drawWindow( m_top );
	g->drawWindow( m_bottom );
	g->display();
}

void ModuleAudio::update( Graphics* g, Events::DIGITAL e )
{
	/* decide if to use internal vec size or external size like a list. */
	switch( m_transitionTable.current()) {
		default:
			m_transitionTable.setSelectionSize( m_transitionTable.getCurrentVecSize());
			break;
	}
	g->clearDisplay();
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
	
	if( m_transitionTable.current() == STATE::MASTER ) {
		g->drawWindow( m_left );
		g->drawWindow( m_right );
	} else {
		g->drawWindow( m_top );
		g->drawWindow( m_bottom );
	}
	g->display();
}

void ModuleAudio::update( Graphics* g, Events::Analog::POTS pots, const AnalogData& data )
{
	g->clearDisplay();
	switch( m_transitionTable.current()) {
		case STATE::MASTER:
			float volume, bitcrush, lowpass, reverb;
			switch( pots ) {
				case Events::Analog::POTS::POT_0:
					bitcrush = static_cast<float>(data.m_data) / 1023.0f;
					m_oldPot0 = (int8_t) ( bitcrush * 100.0f );
					m_audio->setBitCrush( bitcrush );
					break;
				case Events::Analog::POTS::POT_1:
					lowpass = static_cast<float>(data.m_data) / 1023.0f;
					m_oldPot1 = (int8_t) ( lowpass * 100.0f );
					m_audio->setLowPassFreq( lowpass );
					break;
				case Events::Analog::POTS::POT_2:
					reverb = static_cast<float>(data.m_data) / 1023.0f;
					m_oldPot2 = (int8_t) ( reverb * 100.0f );
					m_audio->setRoomSize( reverb );
					break;
				case Events::Analog::POTS::POT_3:
					volume = static_cast<float>(data.m_data) / 1023.0f;
					m_oldPot3 = (int8_t) ( volume * 100.0f );
					m_audio->setMasterVolume( volume );
					break;
			}
			draw_master( pots );
			break;
	}
	
	g->drawWindow( m_left );
	g->drawWindow( m_right );
	g->display();
}

void ModuleAudio::exit()
{ notify( *this ); }

void ModuleAudio::draw_main()
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

void ModuleAudio::draw_master( Events::Analog::POTS e )
{
	uint16_t color1_rot0 = 0xFF;
	uint16_t color1_rot1 = 0xFF;
	uint16_t color1_rot2 = 0xFF;
	uint16_t color1_rot3 = 0xFF;
	
	uint16_t color2_rot0 = 0x33;
	uint16_t color2_rot1 = 0x33;
	uint16_t color2_rot2 = 0x33;
	uint16_t color2_rot3 = 0x33;
	
	uint16_t color3_rot0 = 0x33;
	uint16_t color3_rot1 = 0x33;
	uint16_t color3_rot2 = 0x33;
	uint16_t color3_rot3 = 0x33;
	switch( e ) {
		case Events::Analog::POTS::POT_0:
			color1_rot0 = 0x22;
			color2_rot0 = 0xFF;
			color3_rot0 = 0xEE;
			break;
		case Events::Analog::POTS::POT_1:
			color1_rot1 = 0x22;
			color2_rot1 = 0xFF;
			color3_rot1 = 0xEE;
			break;
		case Events::Analog::POTS::POT_2:
			color1_rot2 = 0x22;
			color2_rot2 = 0xFF;
			color3_rot2 = 0xEE;
			break;
		case Events::Analog::POTS::POT_3:
			color1_rot3 = 0x22;
			color2_rot3 = 0xFF;
			color3_rot3 = 0xEE;
			break;
	}
	//ROT0
	m_left.fillRect( 0, 0, 64, 64, color3_rot0 );
	m_left.setTextColor( color1_rot0 );
	m_left.setCursor( 8, 5 );
	m_left.setFont();
	m_left.println( "Bit-crush" );
	
	m_left.drawRotary( 32, 32, 15, 0xAA, color1_rot0 );
	m_left.drawCircle( 32, 32, 15, color1_rot0, color1_rot0, m_oldPot0 );
	//ROT1
	m_right.fillRect( 0, 0, 64, 64, color3_rot1 );
	m_right.setTextColor( color1_rot1 );
	m_right.setCursor( 8, 5 );
	m_right.setFont();
	m_right.println( "Low-pass" );
	
	m_right.drawRotary( 32, 32, 15, 0xAA, color1_rot1 );
	m_right.drawCircle( 32, 32, 15, color1_rot1, color1_rot1, m_oldPot1 );
	
	//ROT2
	m_left.fillRect( 0, 64, 64, 64, color3_rot2 );
	m_left.setTextColor( color1_rot2 );
	m_left.setCursor( 15, 70 );
	m_left.setFont();
	m_left.println( "Reverb" );
	m_left.drawRotary( 32, 100, 15, 0xAA, color1_rot2 );
	m_left.drawCircle( 32, 100, 15, color1_rot2, color1_rot2, m_oldPot2 );
	//ROT3
	m_right.fillRect( 0, 64, 64, 64, color3_rot3 );
	m_right.setTextColor( color1_rot3 );
	m_right.setCursor( 15, 70 );
	m_right.setFont();
	m_right.println( "Volume" );
	m_right.drawRotary( 32, 100, 15, 0xAA, color1_rot3 );
	m_right.drawCircle( 32, 100, 15, color1_rot3, color1_rot3, m_oldPot3 );
}

void ModuleAudio::draw_channel1()
{

}

void ModuleAudio::draw_channel2()
{

}

void ModuleAudio::draw_channel3()
{

}

void ModuleAudio::draw_channel4()
{

}

void ModuleAudio::draw_audio_module_main( Graphics* g )
{
	m_top.fillScreen( 0xff );
	m_top.drawWindowBorder( { 2, 2 }, 0, 0x00, 1 );
	m_top.printlnCentered( m_moduleName.c_str());
	
	m_bottom.fillScreen( 0x44 );
	m_bottom.drawWindowBorder( { 4, 4 }, 2, 0xbb, 2 );
	m_bottom.setTextPadding( { 4, 4 } );
	if( m_channels.begin() == m_currentChannel ) {
		m_bottom.setTextColor( 0x00 );
		m_bottom.printlnHoverCenter( "MASTER", 0xff );
	} else {
		m_bottom.setTextColor( 0xff );
		m_bottom.printlnHCentered( "MASTER" );
	}
	if( m_channels.begin() + 1 == m_currentChannel ) {
		m_bottom.setTextColor( 0x00 );
		m_bottom.printlnHoverCenter( "CHANNEL 1", 0xff );;
	} else {
		m_bottom.setTextColor( 0xff );
		m_bottom.printlnHCentered( "CHANNEL 1" );
	}
	if( m_channels.begin() + 2 == m_currentChannel ) {
		m_bottom.setTextColor( 0x00 );
		m_bottom.printlnHoverCenter( "CHANNEL 2", 0xff );;
	} else {
		m_bottom.setTextColor( 0xff );
		m_bottom.printlnHCentered( "CHANNEL 2" );
	}
	if( m_channels.begin() + 3 == m_currentChannel ) {
		m_bottom.setTextColor( 0x00 );
		m_bottom.printlnHoverCenter( "CHANNEL 3", 0xff );;
	} else {
		m_bottom.setTextColor( 0xff );
		m_bottom.printlnHCentered( "CHANNEL 3" );
	}
	if( m_channels.begin() + 4 == m_currentChannel ) {
		m_bottom.setTextColor( 0x00 );
		m_bottom.printlnHoverCenter( "CHANNEL 4", 0xff );;
	} else {
		m_bottom.setTextColor( 0xff );
		m_bottom.printlnHCentered( "CHANNEL 4" );
	}
	
	g->clearDisplay();
	g->drawWindow( m_top );
	g->drawWindow( m_bottom );
	g->display();
}

void ModuleAudio::draw_audio_module_sub1( Graphics* g )
{
	g->clearDisplay();
	m_top.fillScreen( 0xff );
	m_top.drawWindowBorder( { 2, 2 }, 0, 0x00, 1 );
	m_top.printlnCentered( "MASTER" );
	m_left.fillScreen( 0x00 );
	m_right.fillScreen( 0x00 );
	m_left.drawLine( 0, 74, 64, 74, 0xFF );
	m_right.drawLine( 0, 74, 64, 74, 0xFF );
	
	//ROT0
	uint16_t color1_rot0 = 0xFF;
	uint16_t color2_rot0 = 0x00;
	if( m_currentEffect == m_effects.begin()) {
		m_left.fillRect( 0, 0, 74, 74, 0xEE );
		color1_rot0 = 0x33;
		color2_rot0 = 0xFF;
	}
	m_last_value_rot0 = analogRead( 14 );
	float p = m_last_value_rot0 / 1023 * 100;
	Serial.println( analogRead( 14 ));
	
	m_left.setTextColor( color1_rot0 );
	m_left.setCursor( 14, 22 );
	m_left.setFont();
	m_left.println( "Volume" );
	
	m_left.drawRotary( 32, 46, 15, 0xAA, color1_rot0 );
	m_left.drawCircle( 32, 46, 15, color1_rot0, color2_rot0, p );
	
	m_left.setTextColor( color1_rot0 );
	m_left.setCursor( 22, 64 );
	m_left.setFont();
	m_left.println( "MIX" );
	
	//ROT1
	uint16_t color1_rot1 = 0xFF;
	uint16_t color2_rot1 = 0x00;
	if( m_currentEffect == m_effects.begin() + 1 ) {
		m_right.fillRect( 0, 0, 74, 74, 0xEE );
		color1_rot1 = 0x33;
		color2_rot1 = 0xFF;
	}
	m_right.setTextColor( color1_rot1 );
	m_right.setCursor( 14, 22 );
	m_right.setFont();
	m_right.println( "Filter" );
	
	m_last_value_rot1 = analogRead( 15 );
	float p_rot1 = m_last_value_rot1 / 1023 * 100;
	
	m_right.drawRotary( 32, 46, 15, 0xAA, color1_rot1 );
	m_right.drawCircle( 32, 46, 15, color1_rot1, color2_rot1, p_rot1 );
	m_right.setTextColor( color1_rot1 );
	m_right.setCursor( 22, 64 );
	m_right.setFont();
	m_right.println( "MIX" );
	
	//ROT2
	uint16_t color1_rot2 = 0xFF;
	uint16_t color2_rot2 = 0x00;
	if( m_currentEffect == m_effects.begin() + 2 ) {
		m_left.fillRect( 0, 74, 74, 74, 0xEE );
		color1_rot2 = 0x33;
		color2_rot2 = 0xFF;
	}
	m_left.setTextColor( color1_rot2 );
	m_left.setCursor( 16, 77 );
	m_left.setFont();
	m_left.println( "Delay" );
	
	m_left.drawRotary( 32, 103, 15, 0xAA, color1_rot2 );
	m_left.drawCircle( 32, 103, 15, color1_rot2, color2_rot2, 20 );
	
	m_left.setTextColor( color1_rot2 );
	m_left.setCursor( 22, 119 );
	m_left.setFont();
	m_left.println( "MIX" );
	
	//ROT3
	uint16_t color1_rot3 = 0xFF;
	uint16_t color2_rot3 = 0x00;
	if( m_currentEffect == m_effects.begin() + 3 ) {
		m_right.fillRect( 0, 74, 74, 74, 0xEE );
		color1_rot3 = 0x33;
		color2_rot3 = 0xFF;
	}
	m_right.setTextColor( color1_rot3 );
	m_right.setCursor( 21, 77 );
	m_right.setFont();
	m_right.println( "Comp" );
	
	m_right.drawRotary( 32, 103, 15, 0xAA, color1_rot3 );
	m_right.drawCircle( 32, 103, 15, color1_rot3, color2_rot3, 20 );
	m_right.setTextColor( color1_rot3 );
	m_right.setCursor( 22, 119 );
	m_right.setFont();
	m_right.println( "MIX" );
	
	g->drawWindow( m_left );
	g->drawWindow( m_right );
	g->drawWindow( m_top );
	
	g->display();
}

void ModuleAudio::draw_audio_module_sub2( Graphics* g )
{
	m_top.fillScreen( 0xff );
	m_top.drawWindowBorder( { 2, 2 }, 0, 0x00, 1 );
	m_bottom.fillScreen( 0x44 );
	m_bottom.drawWindowBorder( { 4, 4 }, 2, 0xbb, 2 );
	m_bottom.setTextPadding( { 4, 4 } );
	if( m_currentEffect == m_effects.begin() + 1 ) {
		m_top.printlnCentered( "DETAIL FILTER" );
		m_bottom.setCursor( 10, 40 );
		if( m_detailFilter.begin() == m_currentDetailFilter ) {
			m_bottom.setTextColor( 0x00 );
			if( highpass ) {
				m_bottom.printlnHoverCenter( "HIGHPASS", 0xff );
			} else {
				m_bottom.printlnHoverCenter( "LOWPASS", 0xff );
			}
		} else {
			m_bottom.setTextColor( 0xff );
			if( highpass ) {
				m_bottom.printlnHCentered( "HIGHPASS" );
			} else {
				m_bottom.printlnHCentered( "LOWPASS" );
			}
		}
		if( m_detailFilter.begin() + 1 == m_currentDetailFilter ) {
			m_bottom.setTextColor( 0x00 );
			m_bottom.fillRect( 8, 42, 112, 25, 0xff );
			m_bottom.printlnHCentered( "FREQUENCY: " );
			m_bottom.setCursor( 50, 65 );
			m_bottom.print( frequency );
			m_bottom.println( "Hz" );
		} else {
			m_bottom.setTextColor( 0xff );
			m_bottom.printlnHCentered( "FREQUENCY" );
			m_bottom.setCursor( 50, 65 );
			m_bottom.print( frequency );
			m_bottom.println( "Hz" );
		}
	} else if( m_currentEffect == m_effects.begin() + 2 ) {
		m_top.printlnCentered( "DETAIL DELAY" );
		m_bottom.setCursor( 10, 50 );
		if( m_detailDelay.begin() == m_currentDetailDelay ) {
			m_bottom.setTextColor( 0x00 );
			m_bottom.printlnHoverCenter( "TIME", 0xff );
		} else {
			m_bottom.setTextColor( 0xff );
			m_bottom.printlnHCentered( "TIME" );
		}
		if( m_detailDelay.begin() + 1 == m_currentDetailDelay ) {
			m_bottom.setTextColor( 0x00 );
			m_bottom.printlnHoverCenter( "FEEDBACK", 0xff );
		} else {
			m_bottom.setTextColor( 0xff );
			m_bottom.printlnHCentered( "FEEDBACK" );
		}
	} else if( m_currentEffect == m_effects.begin() + 3 ) {
		m_top.printlnCentered( "DETAIL COMPRESSOR" );
		if( m_detailCompressor.begin() == m_currentDetailCompressor ) {
			m_bottom.setTextColor( 0x00 );
			m_bottom.printlnHoverCenter( "GAIN", 0xff );
		} else {
			m_bottom.setTextColor( 0xff );
			m_bottom.printlnHCentered( "GAIN" );
		}
		if( m_detailCompressor.begin() + 1 == m_currentDetailCompressor ) {
			m_bottom.setTextColor( 0x00 );
			m_bottom.printlnHoverCenter( "RATIO", 0xff );
		} else {
			m_bottom.setTextColor( 0xff );
			m_bottom.printlnHCentered( "RATIO" );
		}
		if( m_detailCompressor.begin() + 2 == m_currentDetailCompressor ) {
			m_bottom.setTextColor( 0x00 );
			m_bottom.printlnHoverCenter( "TRESHOLD", 0xff );
		} else {
			m_bottom.setTextColor( 0xff );
			m_bottom.printlnHCentered( "TRESHOLD" );
		}
	}
	g->drawWindow( m_bottom );
	g->drawWindow( m_top );
	
	g->display();
}