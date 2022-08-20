#pragma once

#include "define_t40.hpp"
#include <SD.h>
#include <SerialFlash.h>
#include <algorithm>
#include <string>
#include <vector>

namespace lbs
{
	enum AUDIOTYPE
	{
		INVALID = 0,
		WAV = 1,
		RAW = 2
	};

	class MainMemory
	{

	public:
		MainMemory();
		//TODO: implement these functions
		//TODO: handle if samplepack is too big for flash!
		//int getFreeSpacefromFlash();
		//int getFreeSpacefromSD();
		//bool isFitting(constr String& pack_name)
		//bool flashEmpty()
		
		static void eraseFlash();
		static std::vector< String > getFilelistFromFlash();
		static void transferSampleToFlash( const String& filepath );
		static void loadSamplepack( const String& pack_name );
		static std::vector<String> getSampleNamesFromPack( const String& pack_name );
		static void printAllFilesFromSD();
		//static void transferFolderToFlash( const std::string& filepath );
		//std::vector< String > getAllSamplepacks();
		//std::vector<String> getAllFiles();
		//std::vector<String> getAllSamples( const String& samplepack );
		//void initializeSamplepack( const String& samplepack );
	private:
		const static String m_packRootDir;
	};
}