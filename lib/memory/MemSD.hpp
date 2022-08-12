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
	public:
		static MemSD& getInstance();
	
	public:
		bool exists( std::string file );
		std::vector<std::tuple<std::string, int8_t, std::string, std::string>> *readSettings(std::string packName);

        friend bool transferToFlash(std::string filepath);
	
	private:
		static inline String finishCSVLine( String line );
	};
}