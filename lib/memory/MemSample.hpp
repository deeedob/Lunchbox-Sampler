#pragma once;
#include <define_t40.hpp>
#include "MemFlash.hpp"
#include "MemSD.hpp"

namespace lbs
{
	
	class MemSample
	{
	
	private:
		class MidiMapping
		{
		
		public:
			bool add( std::string pitch, int8_t octave, std::string sample, std::string mode );
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
		static MemSample& getInstance();
		bool loadSamplePack( const std::string packName );
	
	private:
		std::string currentSamplePack;
		MidiMapping mapping;
		lbs::MemFlash& mf = MemFlash::getInstance();
		lbs::MemSD& ms = MemSD::getInstance();
		MemSample();
		
	};
	
}