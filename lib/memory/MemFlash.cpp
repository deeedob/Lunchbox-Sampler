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

MemFlash::WriteFile& MemFlash::fileDo()
{
	return this->wfile;
}

bool MemFlash::openFileW( std::string filepath, uint64_t length )
{
	this->wfile = WriteFile(filepath, length);
	if (this->wfile.isOpen()) {
		return true;
	}
	return false;
}

void MemFlash::playSample( std::string filename )
{

}

/**
 * @brief TO BE FILLED
 * @param
 * @return
 */
MemFlash::WriteFile::WriteFile( std::string filename, uint64_t length )
{
	if (!SerialFlash.create(filename.c_str(), length)) {
		return;
	}
	this->filename = filename;
	this->file = SerialFlash.open(filename.c_str());
}

uint64_t MemFlash::WriteFile::size()
{
	return file.size();
}

bool MemFlash::WriteFile::writeByte( uint8_t byte )
{
	return file.write(&byte, 1);
}

uint64_t MemFlash::WriteFile::remaining()
{
	return file.available();
}

bool MemFlash::WriteFile::notAtEnd()
{
	return file.available() > 0;
}

bool MemFlash::WriteFile::isOpen()
{
	return file != NULL;
}

bool MemFlash::WriteFile::reset()
{
	return false;
}

std::string MemFlash::WriteFile::getFilename()
{
	return filename;
}

