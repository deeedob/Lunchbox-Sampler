#include <define_t40.hpp>
#include "MemSD.hpp"

using namespace lbs;


/**
 * @brief add comma separators to string if not enough commas as specified
 * @param line: input string
 * @return line with added commas (goal: 3 commas per line)
 */
inline String MemSD::finishCSVLine( String line )
{
	uint sepcount = 0;
	for( int i = 0; i < line.length(); i++ ) {
		if( line[ i ] == ',' ) {
			sepcount++;
		}
	}
	
	while( sepcount < 3 ) {
		line.append( ',' );
		sepcount++;
	}
	
	//finish string with \n for CSV parser
	line.append( '\n' );
	
	return line;
}

/**
 * @brief TO BE FILLED
 * @param
 * @return
 */
MemSD& MemSD::getInstance()
{
	static MemSD* instance = new MemSD();
	
	if( !SD.begin( C_SDCARD_CS_PIN )) {
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
std::vector< std::tuple< std::string, int8_t, std::string, std::string>>* MemSD::readSettings( const std::string packName )
{
	const std::string pack = C_PACK_DIR + packName;
	const std::string path = pack + C_SETTINGS_FILE;

#ifdef VERBOSE
	Serial.println("VERBOSE MODE");
#endif
	
	if( !SD.exists( path.c_str())) {
#ifdef VERBOSE
		Serial.print("ERROR in readSettings: filepath ");
		Serial.print(path.c_str());
		Serial.println(" does not exist!");
#endif
		//TODO: Error handling
	}
	
	File settings = SD.open( path.c_str(), FILE_READ );
	
	if( !settings ) {
#ifdef VERBOSE
		Serial.println("ERROR in readSettings: could not read file!");
#endif
		//TODO: Error handling
	}
	
	CSV_Parser cp( "scss", false );
	String tmp;
	uint sepcount = 0;
	while( settings.available()) {
		tmp = settings.readStringUntil( '\n' );
		tmp = MemSD::finishCSVLine( tmp );
		cp << tmp.c_str();
	}

#ifdef VERBOSE
	cp.print();
#endif
	
	char** notes = ( char** ) cp[ 0 ];
	int8_t* octaves = ( int8_t* ) cp[ 1 ];
	char** samples = ( char** ) cp[ 2 ];
	char** modes = ( char** ) cp[ 3 ];
	auto* result = new std::vector< std::tuple< std::string, int8_t, std::string, std::string>>;
	std::string sample;
	for( int i = 0; i < cp.getRowsCount(); i++ ) #ifdef VERBOSE
			Serial.println("loadSamplePack: Failed to open File " + *it + " from SD Card");
#endif{
		sample = std::string( samples[ i ] );
		// omit adding to result if specified Sample does not exist
		if( !SD.exists(( pack + sample ).c_str())) {
			continue;
		}
		result->push_back( std::make_tuple( std::string( notes[ i ] ), octaves[ i ], sample, std::string( modes[ i ] )));
	}
	
	return result;
}

/**
 * @brief TO BE FILLED
 * @param
 * @return
 */
MemSD::MemSD()
{

}

/**
 * @brief TO BE FILLED
 * @param
 * @return
 */
bool MemSD::exists( std::string file )
{
	return SD.exists(( C_PACK_DIR + file ).c_str());
}

MemSD::OpenFile& MemSD::fileDo()
{
	return ofile;
}

bool MemSD::openFile( std::string file )
{
	this->ofile = OpenFile(file);
	if ( this->ofile.isOpen()) {
		return true;
	}
	return false;
}

MemSD::OpenFile::OpenFile( std::string filepath )
{
	filename = filepath;
	file = SD.open( filepath.c_str(), FILE_READ );
}

bool MemSD::OpenFile::isOpen()
{
	return file != NULL;
}

MemSD::OpenFile::~OpenFile()
{
	if( isOpen()) {
		file.close();
	}
}

uint64_t MemSD::OpenFile::size()
{
	if( isOpen()) {
		return file.size();
	}
	return 0;
}

uint8_t MemSD::OpenFile::readByte()
{
	if( isOpen()) {
		uint8_t b;
		if( count < size()) {
			file.read( &b, 1 );
			count++;
			return b;
		}
	}
	return 0;
}

uint64_t MemSD::OpenFile::remaining()
{
	if( isOpen()) {
		return size() - count;
	}
	return 0;
}

bool MemSD::OpenFile::notAtEnd()
{
	if( isOpen()) {
		return count < size();
	}
	return false;
}

bool MemSD::OpenFile::reset()
{
	if( isOpen()) {
		file.close();
	}
	
	file = SD.open( filename.c_str(), FILE_READ );
	return true;
}

std::string MemSD::OpenFile::getFilename()
{
	return filename;
}