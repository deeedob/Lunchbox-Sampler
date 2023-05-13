#pragma once
#include <Audio.h>
#include <Wire.h>
#include <memory>
#include <note.hpp>
#include <vector>
#include <numeric>
#include <main_memory.hpp>
#include <TeensyThreads.h>

namespace lbs
{
	class Audio
	{
		struct RoutingInfo
		{
			uint8_t oldNote { 0 };
			uint8_t mixer_pos { 0 };
			uint8_t channel_pos { 0 };
		};
		using Patch = std::unique_ptr<AudioConnection>;
		using AudioFiles = std::vector<String>;
		using AudioMidiRouting = std::vector<uint8_t>;
		using RawFilePlayer = std::vector<std::pair<AudioPlaySerialflashRaw, RoutingInfo>>;
		using PatchBank = std::vector<Patch>;
		using MixerBank = std::vector<AudioMixer4>;
	public:
		enum class POLYPHONY
		{
			SMALL = 4,
			MEDIUM = 8,
			BIG = 16,
		};
		explicit Audio( POLYPHONY poly = POLYPHONY::SMALL );
		
		static bool playNote( const Note& n );
		static bool playSingleRaw( const String& file, float gain = 1.0f );
		static void stopNote( const Note& n );
		
		void setMasterVolume( float volume );
		void setBitCrush( float crush_bits );
		void setRoomSize( float size );
		void setLowPassFreq( float freq );
	private:
		void initialize_connections( POLYPHONY p );
		void sort_audio_files_ascending();
		uint8_t get_amount_of_mixer( POLYPHONY p );
		uint8_t get_amount_of_patches( POLYPHONY p );
	private:
		std::mutex m_mutex;
		AudioControlSGTL5000 m_audioControlSgtl5000;
		AudioOutputI2S m_outputI2S;
		
		AudioFiles m_audioFiles;
		AudioMidiRouting m_audioMidiRouting;
		
		RawFilePlayer m_rawPlayer;
		PatchBank m_patchBank;
		MixerBank m_mixBank;
		
		AudioMixer4 m_fxMixer;
		AudioEffectBitcrusher m_bitcrusher;
		AudioEffectFreeverb m_freeverb;
		AudioFilterLadder m_filter;
		AudioEffectEnvelope m_masteEnv;
		/* what is right or wrong i dont know anymore ...
		 * This somehow works because we call the audio instance from
		 * a static function in the MidiListener wrapped inside a member function called from a seperate thread -- pure madness ---
		 * Maybe there is a more optimal way but for now this will again be used... good ol glue routine again :]
		 * */
		static Audio* m_glue;
	};
}