#pragma once
#include <define_t40.hpp>
#include "MemFlash.hpp"
#include "MemSD.hpp"
#ifdef VERBOSE
#include "Arduino.h"
#endif

namespace lbs
{

	class MemSample
	{
	
	private:
		class MidiMapping
		{
		
		public:
			bool add( std::string pitch, int8_t octave, std::string sample, std::string mode );
			void removeSample(std::string sampleName);
			std::string getSampleName(uint8_t midiNote);
			std::vector< std::string >* getSampleList();
			MidiMapping();
		
		private:
			enum PlaybackMode
			{
				ONESHOT = 0,
				LOOP = 1,
				TOGGLE = 2
			};
			static inline PlaybackMode getModeFromString( std::string mode );
			static const std::unordered_map< std::string, uint8_t > pitches;
			
			std::string samples[128];
			PlaybackMode mode[128];
			
		};
	
	public:
        static MemSample &getInstance();

        bool loadSamplePack(std::string packName);

        friend void playSample(uint8_t midiNote);
		void playSample(uint8_t Note);

    private:
        friend void playSample(uint8_t midiNote);

        std::string currentSamplePack;
        MidiMapping mapping;
        lbs::MemFlash &mf = MemFlash::getInstance();
        lbs::MemSD &ms = MemSD::getInstance();

        MemSample();

    };

    void playSample(uint8_t midiNote);
}