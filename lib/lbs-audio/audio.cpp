#include "audio.hpp"

Audio::Audio()
{
	m_audioControlSgtl5000.volume(1.0f);
}

void Audio::initializeBase()
{
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