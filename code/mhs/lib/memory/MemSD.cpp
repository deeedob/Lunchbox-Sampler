#include <define_t40.hpp>
#include "MemSD.hpp"
using namespace lbs;

/**
 * @brief add comma separators to string if not enough commas as specified
 * @param line: input string
 * @return line with added commas (goal: 3 commas per line)
 */
inline String
MemSD::finishCSVLine(String line)
{
    uint sepcount = 0;
    for (int i = 0; i < line.length(); i++) {
        if (line[i] == ',') {
            sepcount++;
        }
    }

    while (sepcount < 3) {
        line.append(',');
        sepcount++;
    }

    //finish string with \n for CSV parser
    line.append('\n');

    return line;
}

MemSD
&MemSD::getInstance()
{
    static MemSD *instance = new MemSD();

    if (!SD.begin(SDCARD_CS_PIN)) {
#ifdef VERBOSE
        Serial.println("loadSamplePack: Unable to access SPI Flash chip");
#endif
        //TODO: Error handling
    }

    return *instance;
}

String
MemSD::listFlash()
{
    unsigned int count = 0;
    char filename[64];
    uint32_t filesize;
    String filelist;

    /* announce reading filelist (prob setting file iterator to 0) */
    SerialFlash.opendir();

    while (SerialFlash.readdir(filename, sizeof(filename), filesize)) {
            filelist.append("File " + count);
            filelist.append(": ");
            filelist.append(filename);
            filelist.append("  ");
            filelist.append(filesize);
            filelist.append(" bytes\n");
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

void
MemSD::purgeFlash()
{
#ifdef VERBOSE
    Serial.println("purgeFlash: deleting all Files. Please wait until 'ready' (ca.40s)");
#endif
    SerialFlash.eraseAll();
    while (!SerialFlash.ready());

#ifdef VERBOSE
    Serial.println("ready");
#endif
}

std::vector<std::tuple<std::string, int8_t, std::string, std::string>>
*MemSD::readSettings(const std::string packName)
{
    const std::string pack = lbs::packdir + packName;
    const std::string path = pack + lbs::settingsfile;

#ifdef VERBOSE
    Serial.println("VERBOSE MODE");
#endif

   if (!SD.exists(path.c_str())) {
#ifdef VERBOSE
       Serial.print("ERROR in readSettings: filepath ");
       Serial.print(path.c_str());
       Serial.println(" does not exist!");
#endif
        //TODO: Error handling
   }

   File settings = SD.open(path.c_str(), FILE_READ);

   if (!settings) {
#ifdef VERBOSE
       Serial.println("ERROR in readSettings: could not read file!");
#endif
       //TODO: Error handling
   }

   CSV_Parser cp("scss", false);

   String tmp;
   uint sepcount = 0;
    while (settings.available()) {
      tmp = settings.readStringUntil('\n');
      tmp = MemSD::finishCSVLine(tmp);
      cp << tmp.c_str();
   }

#ifdef VERBOSE
   cp.print();
#endif

   char **notes = (char **) cp[0];
   int8_t *octaves = (int8_t *) cp[1];
   char **samples = (char **) cp[2];
   char **modes = (char **) cp[3];

   auto *result = new std::vector<std::tuple<std::string, int8_t,  std::string, std::string>>;

    std::string sample;
    for (int i = 0; i < cp.getRowsCount(); i++) {
       sample = std::string(samples[i]);
       // omit adding to result if specified Sample does not exist
       if (!SD.exists((pack + sample).c_str())) {
           continue;
       }
      result->push_back(std::make_tuple(std::string(notes[i]), octaves[i], sample, std::string(modes[i])));
   }

   return result;
}

MemSD::MemSD() {
}

bool MemSD::exists(std::string file) {
    return SD.exists((lbs::packdir + file).c_str());
}


