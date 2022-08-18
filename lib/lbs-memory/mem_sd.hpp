#pragma once
#include <SD.h>
#include <SPI.h>
#include <define_t40.hpp>

namespace lbs
{
	class MemSD
	{
	public:
		MemSD();
		void printAllFiles( File dir, int numSpaces );
	private:
	};
}