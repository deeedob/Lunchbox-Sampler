#include <define_t40.hpp>
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
	static auto *instance = new MemFlash();
	
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
= default;

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
        filelist += "  ";
        filelist += std::string(String(filesize).c_str());
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

    if (count == 0)
        Serial.println("Flash is empty");

    return filelist;
}


/**
 * @brief TO BE FILLED
 * @param
 * @return
 */
void MemFlash::playSample(const std::string &filename) {
    Serial.println("PlaySample trigger");
    AudioConnection patchCord1(MemFlash::playFlashRaw1,
    0, i2s1, 0);
    AudioConnection patchCord2(MemFlash::playFlashRaw1,
    0, i2s1, 1);
    AudioMemory(10);
    audioShield.enable();
    audioShield.volume(0.5);
    if (this->playFlashRaw1.play(filename.c_str())) {
#ifdef VERBOSE
        Serial.println("Success playing sample");
#endif
    } else {
#ifdef VERBOSE
        Serial.println("Error playing sample");
#endif
    }
}

bool lbs::transferToFlash(const std::string &filepath) {
    File f = SD.open(filepath.c_str());

    if (!f) {
#ifdef VERBOSE
        Serial.print("transferToFlash: error reading file ");
        Serial.print(filepath.c_str());
        Serial.println(" from SD");
#endif
        return false;
    }

    std::string basename = lbs::getBasename(const_cast<std::string &>(filepath));

    if (!SerialFlash.create(basename.c_str(), f.size())) {
#ifdef VERBOSE
        Serial.print("transferToFlash: error creating file");
        Serial.print(basename.c_str());
        Serial.println(" on Flash");
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
