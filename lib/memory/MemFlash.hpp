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

namespace lbs
{
	
	class MemFlash
	{


/* static part */
	public:
		static MemFlash& getInstance();
		void purgeFlash();
		std::string listFlash();
		bool loadFile( std::string name, File& file );
	
	public:
	
	private:
		static const std::string packfolder;
		MemFlash();
	};
}
