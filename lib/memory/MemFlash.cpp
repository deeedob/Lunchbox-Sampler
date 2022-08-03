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

/**
 * @brief TO BE FILLED
 * @param
 * @return
 */
bool MemFlash::loadFile( std::string name, File& file )
{
	if( SerialFlash.create( name.c_str(), file.size())) {
		SerialFlashFile flashfile = SerialFlash.open( name.c_str());
		char[256]
		buf;
		uint count = 0;
		uint n = 0;
		
		while( count < file.size()) {
			n = file.read( &buf, 256 );
			flashfile.write( &buf, n );
			count += n;
		}
	}
}