#include <define_t40.hpp>
#include <map>
#include <tuple>
#include "MemSD.hpp"
#include "MemFlash.hpp"

using namespace lbs;

/**
 * @brief TO BE FILLED
 * @param
 * @return
 */
MemFlash& MemFlash::getInstance()
{
	static MemFlash* instance = new MemFlash();
	
	if( !SerialFlash.begin( C_FLASH_PIN )) {
#ifdef VERBOSE
		Serial.println("loadSamplePack: Unable to access SPI Flash chip");
#endif
		//TODO: Error handling
	}
	
	return *instance;
}

/**
 * @brief TO BE FILLED
 * @param
 * @return
 */
MemFlash::MemFlash()
{
}

/**
 * @brief TO BE FILLED
 * @param
 * @return
 */
void MemFlash::purgeFlash()
{
#ifdef VERBOSE
	Serial.println("purgeFlash: deleting all Files. Please wait until 'ready' (ca.40s)");
#endif
	SerialFlash.eraseAll();
	while( !SerialFlash.ready()) { }

#ifdef VERBOSE
	Serial.println("ready");
#endif
}

/**
 * @brief TO BE FILLED
 * @param
 * @return
 */
std::string MemFlash::listFlash()
{
	unsigned int count = 0;
	char filename[64];
	uint32_t filesize;
	std::string filelist;
	
	/* announce reading filelist (prob setting file iterator to 0) */
	SerialFlash.opendir();
	
	while( SerialFlash.readdir( filename, sizeof( filename ), filesize )) {
		filelist += filename;
		filelist += "  " + filesize;
		filelist += "bytes\n";

#ifdef VERBOSE
		Serial.print("File ");
		Serial.print(count);
		Serial.print(F(": "));
		Serial.print(filename);
		Serial.print(F("  "));
		Serial.print(filesize);
		Serial.print(F(" bytes"));
		Serial.println();
#endif
		count++;
	}
	
	if( count == 0 )
		Serial.println( "Flash is empty" );
	
	return filelist;
}

bool lbs::transferToFlash(std::string filepath) {
    File f = SD.open(filepath.c_str());

    if (!f) {
#ifdef VERBOSE
        Serial.println("transferToFlash: error reading file " + filepath + " from SD");
#endif
        return false;
    }

    std::string basename = lbs::getBasename(filepath);

    if (!SerialFlash.create(basename.c_str(), f.size())) {
#ifdef VERBOSE
        Serial.println("transferToFlash: error creating file" + basename + " on Flash");
#endif
        return false;
    }

    SerialFlashFile ff = SerialFlash.open(basename.c_str());

    char byte;

    for (uint i = 0; i < f.size(); i++) {
        f.readBytes(&byte, 1);
        ff.write(&byte, 1);
    }

    return true;
}

/**
 * @brief TO BE FILLED
 * @param
 * @return
 */