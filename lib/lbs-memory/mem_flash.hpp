#pragma once
#include <LittleFS.h>   //Flash memory on audio shield
#include <SerialFlash.h>
#include <vector>

namespace lbs
{
	
	class MemFlash
	{
	public:
		MemFlash();
		std::vector<String> getAllFromFlash();
		int getFreeSpaceinMB();
		void eraseAll();
		void transferSingleToFlash();
		
		void transferPackToFlash()
		{
		
		};
	private:
	};
}