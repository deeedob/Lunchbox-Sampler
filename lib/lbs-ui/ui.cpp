#include "ui.hpp"

UI::UI( u_int8_t w, u_int8_t h, TwoWire* wire, int8_t display_rst, uint32_t preclk, uint32_t postclk )
{
	//{ LOAD, AUDIO, MANAGESOUND, RECORD, MAINSETTINGS };
	//{ setLoad, setAudio, setManagesound, setRecord, setMainsettings };
	display = std::make_unique<Window>( w, h, wire, display_rst, preclk, postclk ); // as Window is quite heavy make this a pointer
	/* set Transitions for the main Menu */
	transitionTable[ State::LOAD ] = {{ Trigger::setAudio, State::AUDIO }};
	transitionTable[ State::AUDIO ] = {{ Trigger::setLoad, State::LOAD },
	                                   { Trigger::setManagesound, State::MANAGESOUND },
	};
	transitionTable[ State::MANAGESOUND ] = {{ Trigger::setAudio, State::AUDIO },
	                                         { Trigger::setRecord, State::RECORD },
	};
	transitionTable[ State::RECORD ] = {{ Trigger::setManagesound, State::MANAGESOUND },
	                                    { Trigger::setMainsettings, State::MAINSETTINGS },
	};
}

void UI::start()
{
	if( !display->begin( SSD1327_I2C_ADDRESS )) {
		Serial.println( "unable to init OLED" );
		exit( 1 );
	}
	Serial.println( "OLED init succesfull!" );
	display->clearDisplay();
	display->setTextWrap( true );
	
	display->configs->setTxtPadding( { 8, 28 } );
	display->configs->setWinPadding( { 4, 4 } );
	
	display->draw( 10, 4 );
	display->setTextSize( 3 );
	display->printToWindow( "MHS", false );
	
	display->display();
}