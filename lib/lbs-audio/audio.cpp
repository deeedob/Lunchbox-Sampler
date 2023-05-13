#include "audio.hpp"
#include <define_t40.hpp>

using namespace lbs;

Audio* Audio::m_glue = nullptr;

Audio::Audio( POLYPHONY poly )
	: m_audioMidiRouting( 128 ), m_rawPlayer( static_cast<uint8_t>(poly)),
	  m_patchBank( get_amount_of_patches( poly )), m_mixBank( get_amount_of_mixer( poly ))
{
	m_glue = this;
	
	m_audioControlSgtl5000.enable();
	m_audioControlSgtl5000.volume( 0.4f );
	/* TODO: watch this closely grow or shrink and update appropriate! */
	AudioMemory( 80 );
	initialize_connections( poly );
	
	/* setup the audio files and the midi routing*/
	m_audioFiles = MainMemory::getFilelistFromFlash();
	/* fill the routing with 0 ... 127 basically a 1-1 routing */
	std::iota( std::begin( m_audioMidiRouting ), std::end( m_audioMidiRouting ), 0 );
	sort_audio_files_ascending();
	m_glue->m_masteEnv.noteOn();
	m_glue->m_masteEnv.attack( 5 );
#ifdef VERBOSE
	Serial.print( "Audio Memory Usage: " );
	Serial.println(AudioMemoryUsageMax());
#endif
}

bool Audio::playNote( const Note& n )
{
	std::lock_guard<std::mutex> guard( m_glue->m_mutex );
	auto routing = m_glue->m_audioMidiRouting[ n.note ];
	/* check if the note + routing is in bounds */
	if( routing > m_glue->m_audioFiles.size() - 1 || routing < 0 ) {
		return false;
	}
	for( auto& item : m_glue->m_rawPlayer ) {
		if( !item.first.isPlaying()) {
			m_glue->m_mixBank[ item.second.mixer_pos ].gain( item.second.channel_pos, n.velocity );
			item.second.oldNote = n.note;
			
			m_glue->m_masteEnv.noteOn();
			
			return item.first.play( m_glue->m_audioFiles[ routing ].c_str());
		}
	}
	return false;
}

void Audio::stopNote( const Note& n )
{
	std::lock_guard<std::mutex> guard( m_glue->m_mutex );
	for( auto& item : m_glue->m_rawPlayer ) {
		if( item.first.isPlaying() && item.second.oldNote == n.note ) {
			item.first.stop();
			m_glue->m_masteEnv.noteOff();
		}
	}
}

bool Audio::playSingleRaw( const String& file, float gain )
{
	std::lock_guard<std::mutex> guard( m_glue->m_mutex );
	auto& player = m_glue->m_rawPlayer[ 0 ];
	if( player.first.isPlaying())
		player.first.stop();
	
	m_glue->m_mixBank[ player.second.mixer_pos ].gain( player.second.channel_pos, gain );
	return player.first.play( file.c_str());
}

void Audio::setMasterVolume( float volume )
{
	m_glue->m_audioControlSgtl5000.volume( volume );
}

void Audio::setBitCrush( float crush_bits )
{
	m_glue->m_bitcrusher.sampleRate( 44100 );
	m_glue->m_bitcrusher.bits( static_cast<uint8_t>(crush_bits * 16));
}

void Audio::setRoomSize( float size )
{
	if( size < 0.05f )
		m_glue->m_fxMixer.gain( 2, 0.0f );
	else {
		m_glue->m_fxMixer.gain( 2, 1.0f );
		m_glue->m_freeverb.roomsize( size );
	}
}

void Audio::setLowPassFreq( float freq )
{
	m_glue->m_filter.frequency( freq * 20000 );
}

// TODO: implement BIG polyphony
void Audio::initialize_connections( POLYPHONY p )
{
	//AudioConnection patchCord2( playSdWav1, 1, mixer1, 1 );
	/* connecting all player to a mixer */
	uint8_t mval = 0;
	int i = 0;
	for( int j = 0; j < m_rawPlayer.size(); i += 4, mval++, j += 2 ) {
		uint8_t destin = 0;
		m_rawPlayer[ j ].second.mixer_pos = mval;
		m_rawPlayer[ j + 1 ].second.mixer_pos = mval;
		m_rawPlayer[ j ].second.channel_pos = destin;
		m_rawPlayer[ j + 1 ].second.channel_pos = destin + 2;
		m_patchBank[ i ] = std::make_unique<AudioConnection>( m_rawPlayer[ j ].first, 0, m_mixBank[ mval ], destin++ );
		m_patchBank[ i + 1 ] = std::make_unique<AudioConnection>( m_rawPlayer[ j ].first, 1, m_mixBank[ mval ], destin++ );
		m_patchBank[ i + 2 ] = std::make_unique<AudioConnection>( m_rawPlayer[ j + 1 ].first, 0, m_mixBank[ mval ], destin++ );
		m_patchBank[ i + 3 ] = std::make_unique<AudioConnection>( m_rawPlayer[ j + 1 ].first, 1, m_mixBank[ mval ], destin++ );
	}
	/* connect final mix bank for small and medium polyphony */
	if( p == POLYPHONY::SMALL || p == POLYPHONY::MEDIUM ) {
		uint8_t destin = 0;
		for( int j = 0; j < m_mixBank.size() - 1; j++, i++ ) {
			if( destin > 3 )
				destin = 0;
			m_patchBank[ i ] = std::make_unique<AudioConnection>( m_mixBank[ j ], 0, m_mixBank[ m_mixBank.size() - 1 ], destin++ );
		}
	}
	/*make final mono connection */
	m_patchBank[ i++ ] = std::make_unique<AudioConnection>( m_mixBank[ m_mixBank.size() - 1 ], 0, m_fxMixer, 0 );
	m_patchBank.push_back( std::make_unique<AudioConnection>( m_mixBank[ m_mixBank.size() - 1 ], m_bitcrusher ));
	m_patchBank.push_back( std::make_unique<AudioConnection>( m_mixBank[ m_mixBank.size() - 1 ], m_freeverb ));
	m_patchBank.push_back( std::make_unique<AudioConnection>( m_bitcrusher, 0, m_fxMixer, 1 ));
	m_patchBank.push_back( std::make_unique<AudioConnection>( m_freeverb, 0, m_fxMixer, 2 ));
	m_patchBank.push_back( std::make_unique<AudioConnection>( m_fxMixer, 0, m_filter, 0 ));
	//m_patchBank.push_back( std::make_unique<AudioConnection>( m_fxMixer, 0, m_masteEnv, 0 ));
	
	m_patchBank.push_back( std::make_unique<AudioConnection>( m_filter, 0, m_outputI2S, 0 ));
	m_patchBank.push_back( std::make_unique<AudioConnection>( m_filter, 0, m_outputI2S, 1 ));
}

void Audio::sort_audio_files_ascending()
{
	struct less_than
	{
		inline uint8_t first_of( char s, const String& str )
		{
			for( int i = 1; i < str.length(); i++ ) {
				if( str[ i ] == s )
					return i;
			}
			return -1;
		}
		
		inline bool operator()( const String& a, const String& b )
		{
			auto pos_a = first_of( '.', a );
			auto pos_b = first_of( '.', b );
			if( pos_a < 0 || pos_b < 0 )
				return false;
			
			String first = a.substring( 1, pos_a );
			String second = b.substring( 1, pos_b );
			int a_int = first.toInt();
			int b_int = second.toInt();
			return a_int < b_int;
		}
	};
	std::sort( m_audioFiles.begin(), m_audioFiles.end(), less_than());
}

uint8_t Audio::get_amount_of_mixer( POLYPHONY p )
{
	if( p == POLYPHONY::SMALL )
		return 3;
	else if( p == POLYPHONY::MEDIUM )
		return 5;
	else
		return 11;
}

uint8_t Audio::get_amount_of_patches( Audio::POLYPHONY p )
{
	if( p == POLYPHONY::SMALL )
		return 12;
	else if( p == POLYPHONY::MEDIUM )
		return 22;
	else
		return 44;
}