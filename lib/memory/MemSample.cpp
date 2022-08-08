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
	for( auto it = this->mapping.getSampleList()->begin(); it < this->mapping.getSampleList()->end(); it++ ) {
		if (!ms.openFile(*it)) {
			this->mapping.removeSample(*it);
#ifdef VERBOSE
			Serial.println("loadSamplePack: Failed to open File " + *it + " from SD Card");
#endif
			continue;
		}
		
		std::string basename = it->substr(it->find_last_of("/\\") + 1);
		
		if (!mf.openFileW(basename, ms.fileDo().size())) {
#ifdef VERBOSE
			Serial.println("loadSamplePack: Failed to create File " + *it + " on Flash for writing");
#endif
			this->mapping.removeSample(*it);
			continue;
		}
		
		for (int i = 0; i < ms.fileDo().size(); i++) {
			mf.fileDo().writeByte(ms.fileDo().readByte());
		}
		
	}
	
	return true;
	//TODO: adjustSize() (strips samplesizes down if size of sample pack too large
}

void lbs::playSample( uint8_t midiNote )
{
	auto mf = MemFlash::getInstance();
	auto memsample  = MemSample::getInstance();
	std::string sample = memsample.mapping.getSampleName(midiNote);
	mf.playbackFile = SerialFlash.open(sample.c_str());
	if (!mf.playbackFile) {
#ifdef VERBOSE
		Serial.println("playSample: could not load playbackFile");
#endif
		return;
	}
	
	WaveHC wave;
	if(!wave.create(mf.playbackFile)) {
#ifdef VERBOSE
		Serial.println("playSample: could not create wave object");
#endif
		return;
	}
	
	if (wave.isplaying) {
		wave.stop();
	}
	
	wave.play();
}

void MemSample::playSample( uint8_t note)
{
	lbs::playSample(note);
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
		if( this->samples[ i ].compare("") != 0) {
			res->push_back( samples[ i ] );
		}
	}
	return res;
}

void MemSample::MidiMapping::removeSample( std::string sampleName )
{
	for (int i = 0; i < 128; i++) {
		if (sampleName.compare(samples[i]) == 0) {
			samples[i] = "";
			mode[i] = ONESHOT;
			return;
		}
	}
}

std::string MemSample::MidiMapping::getSampleName( uint8_t midiNote )
{
	return this->samples[midiNote];
}


