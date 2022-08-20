#include "main_memory.hpp"

namespace lbs
{
	inline std::string toLower( std::string str ) {
		std::transform( str.begin(), str.end(), str.begin(), ::tolower );
		return str;
	}

	inline std::string getBasename( const std::string& filepath ) {
		return filepath.substr( filepath.find_last_of( "/\\" ) + 1 );
	}

	std::vector< std::string > MainMemory::getFilelistFromFlash() {
		unsigned int count = 0;
		char filename[ 64 ];
		uint32_t filesize;
		std::vector< std::string > filelist;

		/* announce reading filelist (prob setting file iterator to 0) */
		SerialFlash.opendir();

		while( SerialFlash.readdir( filename, sizeof( filename ), filesize ) ) {
			filelist.emplace_back( filename );

#ifdef VERBOSE
			Serial.print( "File " );
			Serial.print( count );
			Serial.print( F( ": " ) );
			Serial.print( filename );
			Serial.print( F( "  " ) );
			Serial.print( filesize );
			Serial.print( F( " bytes" ) );
			Serial.println();
#endif
			count++;
		}

#ifdef VERBOSE
		if( count == 0 ) Serial.println( "Flash is empty" );
#endif

		return filelist;
	}

	void MainMemory::transferSingleToFlash( const std::string& filepath ) {

		Serial.print( "Transfer to Flash function begin upload of" );
		Serial.println( filepath.c_str() );

		File f = SD.open( filepath.c_str() );

		if( !f ) {
#ifdef VERBOSE
			Serial.print( "transferToFlash: error reading file " );
			Serial.print( filepath.c_str() );
			Serial.println( " from SD" );
#endif
		}

		std::string basename = lbs::getBasename( const_cast< std::string& >( filepath ) );

		if( !SerialFlash.createErasable( basename.c_str(), f.size() ) ) {
#ifdef VERBOSE
			Serial.print( "transferToFlash: error creating file" );
			Serial.print( basename.c_str() );
			Serial.println( " on Flash" );
#endif
		}

		SerialFlashFile ff = SerialFlash.open( basename.c_str() );

		//char* buf = new char[ ff.size() ];
		//f.read( buf, ff.size() );
		//ff.write( buf, ff.size() );

		char byte;
		for( int i = 0; i < ff.size(); i++ ) {
			f.read( &byte, 1 );
			ff.write( &byte, 1 );
		}
	}

	MainMemory::MainMemory() {
		SPI.setMOSI( C_SDCARD_MOSI_PIN );
		SPI.setSCK( C_SDCARD_SCK_PIN );

		if( !SD.begin( C_SDCARD_CS_PIN ) ) { Serial.println( "SD Card: initialization failed!" ); }

		if( !SerialFlash.begin( C_FLASH_PIN ) ) {
#ifdef VERBOSE
			Serial.println( "Error initializing Flash Chip!" );
#endif
		}
	}

	void MainMemory::eraseFlash() {
#ifdef VERBOSE
		Serial.println( "purgeFlash: deleting all Files. Please wait until 'ready' (ca.40s)" );
#endif
		SerialFlash.eraseAll();
		while( !SerialFlash.ready() ) {}

#ifdef VERBOSE
		Serial.println( "ready" );
#endif
	}

	void MainMemory::loadSamplepack( const std::string& pack_name ) {
		std::string fullpath;
		auto list = getSampleNamesFromPack( pack_name );
		for( auto& i : list ) {
			// Clang Tidy because of unnecessary string concatenation
			fullpath = C_PACK_DIR + pack_name;
			fullpath += "/" + i;
			transferSingleToFlash( fullpath );
		}
	}

	std::vector< std::string > MainMemory::getSampleNamesFromPack( const std::string& pack_name ) {
		std::string path = C_PACK_DIR + pack_name;
#ifdef VERBOSE
		Serial.print( "Opening path: " );
		Serial.println( path.c_str() );
#endif
		File sample_dir = SD.open( path.c_str() );
		std::vector< std::string > filelist;

		File entry;
		size_t delimiter_pos;
		while( ( entry = sample_dir.openNextFile() ) ) {
			std::string name = std::string( entry.name() );
			delimiter_pos = name.find( '.' );
			if( !entry.isDirectory() && lbs::toLower( name.substr( delimiter_pos, name.length() - 1 ) ) == ".wav" ) {
#ifdef VERBOSE
				Serial.print( "Wav file found: " );
				Serial.println( name.c_str() );
#endif
				filelist.emplace_back( name );
			}
		}

		entry.close();
		sample_dir.close();

		return filelist;
	}

}// namespace lbs