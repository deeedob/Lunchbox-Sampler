#pragma once

#include <SD.h>
#include <SPI.h>
#include <vector>
#include <define_t40.hpp>

namespace lbs {
    class MemSD {
    public:
        MemSD();

        //std::vector<String> getAllFiles();
        std::vector<String> getAllSamplepacks();
        //std::vector<String> getAllSamples( const String& samplepack );
        //void initializeSamplepack( const String& samplepack );
    private:
    };
}