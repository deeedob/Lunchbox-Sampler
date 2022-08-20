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
	void initializeConnections();
private:
	AudioControlSGTL5000 m_audioControlSgtl5000;
	AudioPlaySerialflashRaw m_rawFlash0;
	AudioPlaySerialflashRaw m_rawFlash1;
	
	AudioMixer4 m_mixer01;
	
	AudioFilterLadder m_filterLadderLeft;
	AudioFilterLadder m_filterLadderRight;
	
	AudioOutputI2S m_outputI2S;
	Patch p_f0LTm01;
	Patch p_f0RTm01;
	Patch p_f1LTm01;
	Patch p_f1RTm01;
	Patch p_m01ToutL;
	Patch p_m01ToutR;
};