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
	Serial.println("init");
	m_audioControlSgtl5000.enable();
	m_audioControlSgtl5000.volume(0.7f);
	m_mixer0Left.gain(0, 0.2f);
	m_mixer0Right.gain( 0, 0.2f );
	m_mixer0Left.gain( 1, 0.5f );
	m_mixer0Right.gain( 1, 0.5f );
	m_mixer0Left.gain( 2, 0.5f );
	m_mixer0Right.gain( 2, 0.5f );
	m_mixer0Left.gain( 3, 0.5f );
	m_mixer0Right.gain( 3, 0.5f );
	
	m_filterLadderLeft.frequency(10000);
	m_filterLadderLeft.resonance(0.5f);
	m_filterLadderRight.frequency( 10000 );
	m_filterLadderRight.resonance( 0.5f );
	
	/* TODO: watch this closely grow or shrink and update appropriate! */
	AudioMemory(20);
	initializeConnections();
	
	enableSD();
	m_rawFlash1.play("02.WAV");
	Serial.println( "done" );
}

void Audio::initializeConnections()
{
	Serial.println( "base" );
	/* 4 FlashFiles to Mixer*/
	p_rawToMixer1L = std::make_unique< AudioConnection >( m_rawFlash1, 0, m_mixer0Left, 0 );
	p_rawToMixer1R = std::make_unique< AudioConnection >( m_rawFlash1, 1, m_mixer0Right, 0 );
	
	
	p_rawToMixer2L = std::make_unique< AudioConnection >( m_rawFlash2, 0, m_mixer0Left, 1 );
	p_rawToMixer2R = std::make_unique< AudioConnection >( m_rawFlash2, 1, m_mixer0Right, 1 );
	
	p_rawToMixer3L = std::make_unique< AudioConnection >( m_rawFlash3, 0, m_mixer0Left, 2 );
	p_rawToMixer3R = std::make_unique< AudioConnection >( m_rawFlash3, 1, m_mixer0Right, 2 );
	
	p_rawToMixer4L = std::make_unique< AudioConnection >( m_rawFlash4, 0, m_mixer0Left, 3 );
	p_rawToMixer4R = std::make_unique< AudioConnection >( m_rawFlash4, 1, m_mixer0Right, 3 );
	/* Mixer to Ladder Filter */
	p_mixer0L = std::make_unique< AudioConnection >( m_mixer0Left, 0, m_filterLadderLeft, 0 );
	p_mixer0R = std::make_unique< AudioConnection >( m_mixer0Right, 0, m_filterLadderRight, 0 );
	/* Ladder to output */
	p_ladderL = std::make_unique< AudioConnection >( m_filterLadderLeft, 0, m_outputI2S, 0 );
	p_ladderR = std::make_unique< AudioConnection >( m_filterLadderRight, 0, m_outputI2S, 1 );
}