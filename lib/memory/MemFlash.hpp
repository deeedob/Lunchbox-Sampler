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
#include <MemGen.hpp>

namespace lbs {
    bool transferToFlash(std::string filepath);

    class MemFlash {
/* static part */
    public:
        static MemFlash &getInstance();

    public:
        void purgeFlash();

        std::string listFlash();

        friend bool transferToFlash(std::string filepath);
	
	private:
		friend void playSample(uint8_t midiNote);
		static const std::string packfolder;
		SerialFlashFile playbackFile;
		MemFlash();
	};
}
