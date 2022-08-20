#pragma once

#include "define_t40.hpp"
#include <SD.h>
#include <SerialFlash.h>
#include <string>
#include <vector>

namespace lbs
{

	class MainMemory
	{

	public:
		MainMemory();
		static std::vector< std::string > getAllFromFlash();

		//int getFreeSpaceinMB();
		static void eraseFlash();

		static void transferSingleToFlash( const std::string& filepath );
		//static void transferFolderToFlash( const std::string& filepath );
		//std::vector< String > getAllSamplepacks();
		//std::vector<String> getAllFiles();
		//std::vector<String> getAllSamples( const String& samplepack );
		//void initializeSamplepack( const String& samplepack );
	};
}// namespace lbs