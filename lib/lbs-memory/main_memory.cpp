#include "main_memory.hpp"

using namespace lbs;

const String MainMemory::m_packRootDir = "/samplepacks/";
MainMemory* MainMemory::m_glue = nullptr;

MainMemory::MainMemory()
{
	init();
	m_glue = this;
}

void MainMemory::init()
{
	noInterrupts();
	SPI.setMOSI( C_SDCARD_MOSI_PIN );
	SPI.setSCK( C_SDCARD_SCK_PIN );
	
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

void MainMemory::transferSingleToFlash( const String& filepath )
{
	noInterrupts();
	File f = SD.open( filepath.c_str());
	
	if( !f ) {
#ifdef VERBOSE
		Serial.print( "transferToFlash: error reading file " );
		Serial.print( filepath.c_str());
		Serial.println( " from SD" );
#endif
	}
	auto last_of = []( char s, const String& str ) {
		for( int i = str.length() - 1; i >= 0; i-- ) {
			if( str[ i ] == s ) return i;
		}
		return -1;
	};
	auto pos = last_of( '/', filepath );
	String basename;
	if( pos < 0 )
		basename = filepath;
	else
		basename = filepath.substring( pos );
	
	if( !SerialFlashChip::createErasable( basename.c_str(), f.size())) {
#ifdef VERBOSE
		Serial.print( "transferToFlash: error creating file" );
		Serial.print( basename.c_str());
		Serial.println( " on Flash" );
#endif
	}
	
	SerialFlashFile ff = SerialFlashChip::open( basename.c_str());
	
	char byte;
	for( int i = 0; i < ff.size(); i++ ) {
		f.read( &byte, 1 );
		ff.write( &byte, 1 );
	}
	
	f.close();
	ff.close();
	interrupts();
}

void MainMemory::eraseFlash()
{
	noInterrupts();
	uint32_t i, blocksize, capacity;
	blocksize = SerialFlashChip::blockSize();
	
	uint8_t id[5];
	SerialFlashChip::readID( id );
	capacity = SerialFlashChip::capacity( id );
	for( i = 0; i < capacity; i += blocksize ) {
		SerialFlashChip::eraseBlock( i );
		m_glue->notify( std::pair<uint32_t, uint32_t>( i, capacity ));
	}
	m_glue->notify( std::pair<uint32_t, uint32_t>( capacity, capacity ));
	while ( !SerialFlashChip::ready()) { }
	interrupts();
}

void MainMemory::loadSamplepack( const String& pack_name )
{
	noInterrupts();
	String fullpath;
	auto list = getSampleNamesFromPack( pack_name );
	uint32_t size = list.size();
	uint32_t current = 0;
	for( auto& i : list ) {
		fullpath = m_packRootDir + pack_name;
		fullpath += "/" + i;
		transferSingleToFlash( fullpath );
		m_glue->notify( std::pair<uint32_t, uint32_t>( current, size ));
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
		if( !entry.isDirectory() && !name.endsWith( ".csv" )) {
			filelist.push_back( name );
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