#include "audio.hpp"
#include <define_t40.hpp>
void enableSD()
{
	SPI.setMOSI(C_SDCARD_MOSI_PIN);
	SPI.setSCK(C_SDCARD_SCK_PIN);
	if( !( SD.begin( C_SDCARD_CS_PIN ))) {
		while( 1 ) {
			Serial.println( "Unable to access the SD card" );
			delay( 500 );
		}
	}
	delay(200);
}

Audio::Audio()
{
	m_audioControlSgtl5000.enable();
	m_audioControlSgtl5000.volume(0.7f);
	m_mixer01.gain(0, 0.2f);
	m_mixer01.gain( 1, 0.2f );
	m_mixer01.gain( 2, 0.5f );
	m_mixer01.gain( 3, 0.5f );
	
	m_filterLadderLeft.frequency(10000);
	m_filterLadderLeft.resonance(0.5f);
	m_filterLadderRight.frequency( 10000 );
	m_filterLadderRight.resonance( 0.5f );
	
	/* TODO: watch this closely grow or shrink and update appropriate! */
	AudioMemory(20);
	initializeConnections();
	
	enableSD();
	m_rawFlash1.play("/18.wav");
	//m_rawFlash0.play("/16.wav");
	//m_rawFlash3.play("/10.wav");
	//m_rawFlash2.play("/13.wav");
	//m_rawFlash3.play("/18.wav");
}

void Audio::initializeConnections()
{
	p_f0LTm01 = std::make_unique<AudioConnection>( m_rawFlash0, 0, m_mixer01, 0 );
	//p_f0RTm01 = std::make_unique<AudioConnection>( m_rawFlash0, 1, m_mixer01, 1 );
	p_f1LTm01 = std::make_unique<AudioConnection>( m_rawFlash1, 0, m_mixer01, 2 );
	//p_f1RTm01 = std::make_unique<AudioConnection>( m_rawFlash1, 1, m_mixer01, 3 );
	
	p_m01ToutL = std::make_unique<AudioConnection>(m_mixer01, 0, m_outputI2S, 0);
	p_m01ToutR = std::make_unique<AudioConnection>( m_mixer01, 0, m_outputI2S, 1 );
}