#pragma once
#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>
#include <memory>

class Audio
{
	using Patch = std::unique_ptr<AudioConnection>;
public:
	Audio();
	void initializeBase();
private:
	// AudioPlayQueue m_queue; this queue maybe better ?
	AudioControlSGTL5000 m_audioControlSgtl5000;
	AudioPlaySdWav m_rawFlash1;
	AudioPlaySerialflashRaw m_rawFlash2;
	AudioPlaySerialflashRaw m_rawFlash3;
	AudioPlaySerialflashRaw m_rawFlash4;
	
	AudioMixer4 m_mixer0Left;
	AudioMixer4 m_mixer0Right;
	
	AudioFilterLadder m_filterLadderLeft;
	AudioFilterLadder m_filterLadderRight;
	
	AudioOutputI2S m_outputI2S;
	
	Patch p_rawToMixer1L;
	Patch p_rawToMixer2L;
	Patch p_rawToMixer3L;
	Patch p_rawToMixer4L;
	Patch p_rawToMixer1R;
	Patch p_rawToMixer2R;
	Patch p_rawToMixer3R;
	Patch p_rawToMixer4R;
	
	Patch p_mixer0L;
	Patch p_mixer0R;
	
	Patch p_ladderL;
	Patch p_ladderR;
};