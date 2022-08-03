#include "MemSample.hpp"

using namespace lbs;

const std::unordered_map< std::string, uint8_t > MemSample::MidiMapping::pitches = {{ "c",   0 },
                                                                                    { "csh", 1 },
                                                                                    { "d",   2 },
                                                                                    { "dsh", 3 },
                                                                                    { "e",   4 },
                                                                                    { "f",   5 },
                                                                                    { "fsh", 6 },
                                                                                    { "g",   7 },
                                                                                    { "gsh", 8 },
                                                                                    { "a",   9 },
                                                                                    { "ash", 10 },
                                                                                    { "b",   11 }};

std::string& toLower( std::string& str )
{
	std::transform( str.begin(), str.end(), str.begin(), ::tolower );
	return str;
}

MemSample::MemSample()
{
}

MemSample& MemSample::getInstance()
{
	static MemSample* instance = new MemSample();
	return *instance;
}

bool MemSample::loadSamplePack( const std::string packName )
{
	
	auto settings = this->ms.readSettings( packName );
	
	//put output from CSV Parser into mapping
	int count = 0;
	for( auto i = settings->begin(); i < settings->end(); i++ ) {
		std::string& pitch = std::get< 0 >( *i );
		int8_t& oct = std::get< 1 >( *i );
		std::string& sample = std::get< 2 >( *i );
		std::string& mode = std::get< 3 >( *i );
		this->mapping.add( pitch, oct, sample, mode );
	}
	
	currentSamplePack = packName;
	
	mf.purgeFlash();
	
	// load mapping onto flash memory
	File* sample;
	for( auto it = this->mapping.getSampleList()->begin(); it < this->mapping.getSampleList()->end(); it++ ) {
		ms.fileDo().readByte();
		mf.loadFile( *it, *sample );
	}
	
	//TODO: adjustSize() (strips samplesizes down if size of sample pack too large
	
}

bool MemSample::loadSample( std::string name )
{
	
	if( ms.openFile( C_PACK_DIR + currentSamplePack + "/" + name )) {
		while( ms.fileDo().notAtEnd()) {
		}
	}
}

MemSample::MidiMapping::MidiMapping()
{
	for( int i = 0; i < 128; i++ ) {
		this->samples[ i ] = nullptr;
		this->mode[ i ] = MidiMapping::PlaybackMode::ONESHOT;
	}
}

bool MemSample::MidiMapping::add( std::string pitch, int8_t octave, std::string sample, std::string mode )
{
	
	if( octave < -1 || octave > 9 ) {
		return false;
	}
	
	// TODO: catch pitchNo if hashtable fails
	auto pitchNo = this->pitches.at( pitch );

#ifdef VERBOSE
	Serial.print("check pitchNo: ");
	Serial.println(pitchNo);
#endif
	
	int16_t note = pitchNo + ( octave + 1 ) * 12;
	
	if( note < 0 || note > 127 ) {
		return false;
	}
	
	this->samples[ ( uint8_t ) note ] = sample;
	this->mode[ ( uint8_t ) note ] = MemSample::MidiMapping::getModeFromString( mode );
	return true;
	
}

inline MemSample::MidiMapping::PlaybackMode MemSample::MidiMapping::getModeFromString( std::string mode )
{
	if( mode == "TOGGLE" ) {
		return MemSample::MidiMapping::PlaybackMode::TOGGLE;
	} else if( mode == "LOOP" ) {
		return MemSample::MidiMapping::PlaybackMode::LOOP;
	} else {
		return MemSample::MidiMapping::PlaybackMode::ONESHOT;
	}
}

std::vector< std::string >* MemSample::MidiMapping::getSampleList()
{
	auto* res = new std::vector< std::string >;
	for( int i = 0; i < 127; i++ ) {
		if( this->samples[ i ] != "" ) {
			res->push_back( samples[ i ] );
		}
	}
	return res;
}


