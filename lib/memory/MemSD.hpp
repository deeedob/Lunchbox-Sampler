#pragma once
#include <SD.h>
#include <SerialFlash.h>
#include <Bounce.h>
#include <iostream>
#include <memory>
#include <vector>
#include <tuple>
#include <CSV_Parser.h>

namespace lbs
{
	
	class MemSD
	{
	private:
		
		class OpenFile
		{
		public:
			uint64_t size();
			uint8_t readByte();
			uint64_t remaining();
			bool notAtEnd();
			bool isOpen();
			bool reset();
			std::string getFilename();
			OpenFile( std::string filepath );
			~OpenFile();
		
		private:
			std::string filename;
			uint64_t count = 0;
			File file;
			
		};
	
	public:
		static MemSD& getInstance();
	
	public:
		bool exists( std::string file );
		bool openFile( std::string file );
		OpenFile& fileDo();
		std::vector< std::tuple< std::string, int8_t, std::string, std::string>>* readSettings( std::string packName );
	
	private:
		OpenFile ofile;
		static inline String finishCSVLine( String line );
		MemSD();
	};
}