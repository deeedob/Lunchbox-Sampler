#pragma once
#include <Arduino.h>
#include <SerialFlash.h>
#include <SD.h>
#include <iostream>
#include <string>
#include <cstdio>
#include <memory>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <SoundZ.h>

namespace lbs
{
	
	class MemFlash
	{
	
	private:
		class WriteFile
			{
			public:
				uint64_t size();
				bool writeByte(uint8_t byte);
				uint64_t remaining();
				bool notAtEnd();
				bool isOpen();
				bool reset();
				std::string getFilename();
				
				WriteFile( std::string filepath, uint64_t length );
				~WriteFile();
		
		private:
				std::string filename;
				uint64_t count = 0;
				SerialFlashFile file;
			
			};


/* static part */
	public:
		static MemFlash& getInstance();
	
	public:
		void purgeFlash();
		std::string listFlash();
		bool openFileW( std::string file, uint64_t length );
		MemFlash::WriteFile& fileDo();
		SerialFlashFile& getFile(std::string filename);
	
	private:
		friend void playSample(uint8_t midiNote);
		static const std::string packfolder;
		WriteFile wfile;
		SerialFlashFile playbackFile;
		MemFlash();
	};
}
