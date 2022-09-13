#include "main_memory.hpp"

using namespace lbs;

const String MainMemory::m_packRootDir = "/samplepacks/";
MainMemory *MainMemory::m_glue = nullptr;

uint MainMemory::freeSpaceFlash = 0;

/**
 * @brief determines filetype of an audio file by extension
 * @param sampleName: name of sample/audio file
 * @return audiotype (RAW, WAVE or INVALID)
 */
AUDIOTYPE getAudioType(const String &sampleName) {
	noInterrupts();
	String ext = sampleName.substring(sampleName.lastIndexOf(".")).toLowerCase();
	if (ext.compareTo(".wav") == 0) {
		return AUDIOTYPE::WAV;
	} else if (ext.compareTo(".raw") == 0) {
		return AUDIOTYPE::RAW;
	}

	return AUDIOTYPE::INVALID;
	interrupts();
}

/**
 * @brief skips the header of a wave file reference provided
 * @param File &f: reference to a wave file
 * @return the first position of the payload after the header
 */
size_t findPayload(File &f) {
	noInterrupts();
	size_t pos = f.position();
	f.seek(0);

	if (!f) {
		interrupts();
		return 0;
	}

	char byte[4];
	while (f.available()) {
		f.read(&byte, 1);
		if (byte[0] == 'd' && f.available() > 3 && f.peek() == 'a') {
			f.read(&byte, 3);
			byte[3] = '\0';
			if (strcmp(byte, "ata") == 0) {
				size_t res = f.position();
				f.seek(pos);
				interrupts();
				return res;
			}
		}
	}
	interrupts();
	return 0;
}

/**
 * @brief calculated the size of a wave file without the header
 * @param fullpath: the path to a wave file
 * @return the size of the wave payload/audio data
 */
size_t getRawAudioSize(const String &fullpath) {
	noInterrupts();
	File f = SD.open(fullpath.c_str());
	size_t audioPos = findPayload(f);
	if (audioPos > 0) {
		audioPos--;
	}
	interrupts();
	return f.size() - audioPos;
}

MainMemory::MainMemory() {
	init();
	m_glue = this;
}

void MainMemory::init() {
	noInterrupts();
	SPI.setMOSI(C_SDCARD_MOSI_PIN);
	SPI.setSCK(C_SDCARD_SCK_PIN);
	
	if( !SD.begin( C_SDCARD_CS_PIN )) {
		#ifdef VERBOSE
		Serial.println( "SD Card: initialization failed!" );
		#endif
	}
	delay( 200 );
	if( !SerialFlash.begin( C_FLASH_PIN )) {
		#ifdef VERBOSE
		Serial.println( "Error initializing Flash Chip!" );
		#endif
	}
	delay( 200 );
	interrupts();
}
/**
 * @brief  returns a filelist of files on flash
 * @return a vector of strings containing all files on flash
 */
std::vector<String> MainMemory::getFilelistFromFlash()
{
	noInterrupts();
	unsigned int count = 0;
	char filename[64];
	uint32_t filesize;
	std::vector<String> filelist;
	
	SerialFlash.opendir();
	
	while ( SerialFlash.readdir( filename, sizeof( filename ), filesize )) {
		filelist.emplace_back( filename );
		count++;
	}
	interrupts();
	return filelist;
}

/**
 * @brief transfers a given sample to flash
 * @param filepath: absolute path to audio sample
 * @param sampleSize: custom sample size in case of low free space on flash
 * @return unsigned int of bytes transferred to flash chip
 */
uint32_t MainMemory::transferSingleToFlash(const String &filepath, const size_t sampleSize) {
	noInterrupts();
	File f = SD.open(filepath.c_str());

	if (!f) {
#ifdef VERBOSE
		Serial.print("transferToFlash: error reading file ");
		Serial.print(filepath.c_str());
		Serial.println(" from SD");
#endif
		interrupts();
		return 0;
	}
	String basename = filepath.substring(filepath.lastIndexOf('/') + 1);

	if (getAudioType(basename) == AUDIOTYPE::WAV) {
		size_t payloadPos = findPayload(f);
		f.seek(payloadPos);

#ifdef VERBOSE
		if (f.available() < 1) {
			Serial.println("WARNING: WAV file will be loaded with header!");
			f.seek(0);
		}
#endif
	}

	basename = basename.substring(0, basename.lastIndexOf('.'));
	size_t filelength = f.available();

	if (sampleSize) {
		filelength = sampleSize;
#ifdef VERBOSE
		Serial.print("Stripping filelength from ");
		Serial.print(f.available());
		Serial.print(" to ");
		Serial.print(filelength);
		Serial.println(" bytes");
#endif
	}

	if (!SerialFlashChip::createErasable(basename.c_str(), f.size())) {
#ifdef VERBOSE
		Serial.print("transferToFlash: error creating file");
		Serial.print(basename.c_str());
		Serial.println(" on Flash");
#endif
		interrupts();
		return 0;
	}
	
	SerialFlashFile ff = SerialFlashChip::open( basename.c_str());

	char byte;
	for( int i = 0; i < ff.size(); i++ ) {
		f.read( &byte, 1 );
		ff.write( &byte, 1 );
	}
	
	f.close();
	size_t size = ff.size();
	ff.close();
	interrupts();
	return size;
}

void MainMemory::eraseFlash()
{
	noInterrupts();
	uint32_t i, blocksize, capacity;
	blocksize = SerialFlashChip::blockSize();

    uint8_t id[5];
    SerialFlashChip::readID(id);
    capacity = SerialFlashChip::capacity(id);
    for (i = 0; i < capacity; i += blocksize) {
        SerialFlashChip::eraseBlock(i);
        m_glue->notify(std::pair<uint32_t, uint32_t>(i, capacity));
    }
    m_glue->notify(std::pair<uint32_t, uint32_t>(capacity, capacity));
    while (!SerialFlashChip::ready()) {}
    freeSpaceFlash = C_FLASHSIZE;
    currentPack = "";
    interrupts();
}

void MainMemory::loadSamplepack( const String& pack_name ) {
	noInterrupts();
	eraseFlash();
	createStdMappingFile(pack_name);
	String fullpath;
	auto list = getSampleNamesFromPack(pack_name);

	uint sum = 0;
	for (const auto &sample: list) {
		sum += getRawAudioSize(fullpath + sample);
	}

	double stripFactor = 0;
	if (sum > getFreeSpacefromFlash()) {
		stripFactor = (float_t) getFreeSpacefromFlash() / (float_t) sum;
#ifdef VERBOSE
		Serial.print("Stripfactor for samples is ");
		Serial.println(stripFactor, 4);
#endif
	}

	size_t sampleSize = 0;
	uint32_t size = list.size();
	uint32_t current = 0;
	for (auto &i: list) {
		fullpath = m_packRootDir + pack_name;
		fullpath += "/" + i;
		transferSingleToFlash(fullpath);
		m_glue->notify(std::pair<uint32_t, uint32_t>(current, size));
		current++;
	}
	interrupts();
}

std::vector<String> MainMemory::getSampleNamesFromPack( const String& pack_name )
{
	noInterrupts();
	std::vector<String> filelist;
	String path = m_packRootDir + pack_name;
	
	File sample_dir = SD.open( path.c_str());
	File entry;
	while (( entry = sample_dir.openNextFile())) {
		String name = entry.name();
		if (!entry.isDirectory() && getAudioType(name) != AUDIOTYPE::INVALID) {
			filelist.push_back(name);
		}
	}
	
	entry.close();
	sample_dir.close();
	
	interrupts();
	return filelist;
}

std::vector<String> MainMemory::getSamplePacksFromSD()
{
	noInterrupts();
	std::vector<String> list;
	auto sample_dir = SD.open( m_packRootDir.c_str());
	if( !sample_dir || !sample_dir.isDirectory()) {
		list.emplace_back( "Error occurred!" );
		list.emplace_back( "Check the SD Card." );
		interrupts();
		return list;
	}
	while ( auto item = sample_dir.openNextFile()) {
		if( !item.isDirectory()) continue;
		list.emplace_back( item.name());
	}
	sample_dir.close();
	interrupts();
	return list;
}

/* helper functions to print list all files recursively */
void printSpaces( int num )
{
	for( int i = 0; i < num; i++ ) { Serial.print( " " ); }
}

void printTime( const DateTimeFields tm )
{
	const char* months[12] = { "January", "February", "March", "April", "May", "June",
	                           "July", "August", "September", "October", "November", "December"
	};
	if( tm.hour < 10 ) Serial.print( '0' );
	Serial.print( tm.hour );
	Serial.print( ':' );
	if( tm.min < 10 ) Serial.print( '0' );
	Serial.print( tm.min );
	Serial.print( "  " );
	Serial.print( tm.mon < 12 ? months[ tm.mon ] : "???" );
	Serial.print( " " );
	Serial.print( tm.mday );
	Serial.print( ", " );
	Serial.print( tm.year + 1900 );
}

void printDirectory( File dir, int numSpaces )
{
	while ( true ) {
		File entry = dir.openNextFile();
		if( !entry ) {
			//Serial.println("** no more files **");
			break;
		}
		printSpaces( numSpaces );
		Serial.print( entry.name());
		if( entry.isDirectory()) {
			Serial.println( "/" );
			printDirectory( entry, numSpaces + 2 );
		} else {
			// files have sizes, directories do not
			unsigned int n = log10( entry.size());
			if( n > 10 ) n = 10;
			printSpaces( 50 - numSpaces - strlen( entry.name()) - n );
			Serial.print( "  " );
			Serial.print( entry.size(), DEC );
			DateTimeFields datetime;
			if( entry.getModifyTime( datetime )) {
				printSpaces( 4 );
				printTime( datetime );
			}
			Serial.println();
		}
		entry.close();
	}
}

void MainMemory::printAllFilesFromSD()
{
    Serial.println("_____________START PRINTDIR_______________");
    File root = SD.open("/");
    printDirectory(root, 0);
    Serial.println("_____________END  PRINTDIR_______________");
}

MainMemory *MainMemory::instance() {
    return m_glue;
}

/**
 * @brief returns free space on SD card
 * @return free space on SD card
 */
uint64_t MainMemory::getFreeSpacefromSD() {
    return SD.totalSize() - SD.usedSize();
}

/**
 * @brief returnsfree space on flash
 * @return unsigned int of free space on flash
*/
uint MainMemory::getFreeSpacefromFlash() {
    return freeSpaceFlash;
}

/**
 * @brief function to check if flash is empty
 * @return boolean if flash is empty
 */
bool MainMemory::flashEmpty() {
	noInterrupts();
	if (getFreeSpacefromFlash() == C_FLASHSIZE) {
		interrupts();
		return true;
	}
	interrupts();
	return false;
}

/**
 * @brief converts midiNote integer [0, 127] to Note name (scientific pitch notation)
 * @param midiNote: unsigned byte [0, 127]
 * @return string with according note name (scientific pitch notation)
 */
String MainMemory::getNoteName(uint8_t midiNote) {
	noInterrupts();
	uint8_t octave = (midiNote / 12) - 1;
	uint8_t noteVal = midiNote % 12;
	String noteValTxt;

	switch (noteVal) {
		case 0:
			interrupts();
			return "C" + String(octave);
		case 1:
			interrupts();
			return "C#" + String(octave);
		case 2:
			interrupts();
			return "D" + String(octave);
		case 3:
			interrupts();
			return "D#" + String(octave);
		case 4:
			interrupts();
			return "E" + String(octave);
		case 5:
			interrupts();
			return "F" + String(octave);
		case 6:
			interrupts();
			return "F#" + String(octave);
		case 7:
			interrupts();
			return "G" + String(octave);
		case 8:
			interrupts();
			return "G#" + String(octave);
		case 9:
			interrupts();
			return "A" + String(octave);
		case 10:
			interrupts();
			return "A#" + String(octave);
		case 11:
			interrupts();
			return "B" + String(octave);
		default:
			interrupts();
			return "C" + String(octave);
	}
}

/**
 * @brief creates a standard mapping file for samplepack (starting at C4 or C0 if many samples)
 * @param packName: name of sample pack to be processed
 * @return success or failure
 */
bool MainMemory::createStdMappingFile(const String &packName) {
	noInterrupts();
	String settingsPath = m_packRootDir + "/" + packName + "/" + C_SETTINGS_FILE;

	if (!SD.exists(settingsPath.c_str())) {
#ifdef VERBOSE
		Serial.println("createStdMappingFile(): No mapping found, creating new standard mapping");
#endif
		File settings = SD.open(settingsPath.c_str(), FILE_WRITE_BEGIN);
		auto samples = getSampleNamesFromPack(packName);
		//starting at C4
		uint8_t currentNote = 60;
		if (samples.size() > 67) {
#ifdef VERBOSE
			Serial.println("createStdMappingFile(): Many samples in folder, starting at octave 0");
#endif
			currentNote = 12;
		}
		String line;
		for (auto &sample: samples) {
			//stop at highest midi note
			if (currentNote > 127) {
				break;
			}
			line = (getNoteName(currentNote) + "," + sample + ",ONESHOT\n");
			settings.write(line.c_str());
			currentNote++;
		}
		settings.close();
		interrupts();
		return true;
#ifdef VERBOSE
	} else {
		Serial.print("createStdMappingFile(): Mapping ");
		Serial.print(settingsPath.c_str());
		Serial.println(" exists and will not be overwritten, either change manually or delete");
#endif
	}
	interrupts();
	return false;
}