#include "main_memory.hpp"

using namespace lbs;

const String MainMemory::m_packRootDir = "/samplepacks/";

AUDIOTYPE getAudioType( const String& sample_name ) {
	String ext = toLowerCase( sample_name.lastIndexOf( "." ) );
	if( ext.compareTo( ".wav" ) == 0 ) {
		return AUDIOTYPE::WAV;
	} else if( ext.compareTo( ".raw" ) == 0 ) {
		return AUDIOTYPE::RAW;
	}

	return AUDIOTYPE::INVALID;
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

std::vector< String > MainMemory::getFilelistFromFlash() {
	unsigned int count = 0;
	char filename[ 64 ];
	uint32_t filesize;
	std::vector< String > filelist;

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

void MainMemory::transferSingleToFlash( const String& filepath ) {

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
	String basename = filepath.substring( filepath.lastIndexOf( '/' ) );

	uint skip_bytes = 0;
	if( getAudioType( basename ) == AUDIOTYPE::WAV ) {
		char byte[ 4 ];
		for( uint i = 0; i < f.size(); ) {
			f.read( &byte, 1 );
			i++;
			if( byte[ 0 ] == 'd' && i < f.size() - 3 ) {
				f.read( &byte, 3 );
				i += 3;
				byte[ 3 ] = '\0';
				if( strcmp( byte, "ata" ) == 0 ) {
					skip_bytes = i;
#ifdef VERBOSE
					Serial.print( "Skipped " );
					Serial.print( i );
					Serial.print( " byte header on " );
					Serial.println( basename );
#endif
					break;
				}
			}
		}
#ifdef VERBOSE
		Serial.println( "WARNING: WAV file will be loaded with header!" );
#endif
	}

	if( !SerialFlashChip::createErasable( basename.c_str(), f.size() - skip_bytes ) ) {
#ifdef VERBOSE
		Serial.print( "transferToFlash: error creating file" );
		Serial.print( basename.c_str() );
		Serial.println( " on Flash" );
#endif
	}

	SerialFlashFile ff = SerialFlashChip::open( basename.c_str() );

	//char* buf = new char[ ff.size() ];
	//f.read( buf, ff.size() );
	//ff.write( buf, ff.size() );

	char byte;
	for( int i = 0; i < ff.size(); i++ ) {
		f.read( &byte, 1 );
		ff.write( &byte, 1 );
	}

	f.close();
	ff.close();
}


void MainMemory::eraseFlash() {
#ifdef VERBOSE
	Serial.println( "purgeFlash: deleting all Files. Please wait until 'ready' (ca.40s)" );
#endif
	SerialFlashChip::eraseAll();
	while( !SerialFlashChip::ready() ) {}

#ifdef VERBOSE
	Serial.println( "ready" );
#endif
}

void MainMemory::loadSamplepack( const String& pack_name ) {
	String fullpath;
	auto list = getSampleNamesFromPack( pack_name );
	for( auto& i : list ) {
		// Clang Tidy because of unnecessary string concatenation
		fullpath = m_packRootDir + pack_name;
		fullpath += "/" + i;
		transferSingleToFlash( fullpath );
	}
}

std::vector<String> MainMemory::getSampleNamesFromPack( const String& pack_name ) {
	std::vector<String> filelist;
	String path = m_packRootDir + pack_name;

	File sample_dir = SD.open( path.c_str());
	File entry;
	while( ( entry = sample_dir.openNextFile()  ) ) {
		String name = entry.name();
		if( !entry.isDirectory() && getAudioType( entry.name() ) != AUDIOTYPE::INVALID ) {
#ifdef VERBOSE
			Serial.print( "Audio file " );
			Serial.print( entry.name() );
			Serial.println( " found" );
#endif
			filelist.push_back( name );
		}
	}

	entry.close();
	sample_dir.close();

	return filelist;
}

/* helper functions to print list all files recursively */
void printSpaces( int num )
{
	for( int i = 0; i < num; i++ ) {
		Serial.print( " " );
	}
}

void printTime( const DateTimeFields tm )
{
	const char* months[12] = {
		"January", "February", "March", "April", "May", "June",
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

void printDirectory( File dir, int num_spaces ) {
	while( true ) {
		File entry = dir.openNextFile();
		if( !entry ) {
			//Serial.println("** no more files **");
			break;
		}
		printSpaces( num_spaces );
		Serial.print( entry.name() );
		if( entry.isDirectory() ) {
			Serial.println( "/" );
			printDirectory( entry, num_spaces + 2 );
		} else {
			// files have sizes, directories do not
			unsigned int n = log10( entry.size());
			if( n > 10 ) n = 10;
			printSpaces( 50 - num_spaces - strlen( entry.name() ) - n );
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
	Serial.println( "_____________END  PRINTDIR_______________" );
}