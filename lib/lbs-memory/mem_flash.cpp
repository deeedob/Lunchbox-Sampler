#include "mem_flash.hpp"

namespace lbs {

    inline std::string getBasename(const std::string &filepath) {
        return filepath.substr(filepath.find_last_of("/\\") + 1);
    }

    std::vector<std::string> MemFlash::getAllFromFlash() {
        unsigned int count = 0;
        char filename[64];
        uint32_t filesize;
        std::vector<std::string> filelist;

        /* announce reading filelist (prob setting file iterator to 0) */
        SerialFlash.opendir();

        while (SerialFlash.readdir(filename, sizeof(filename), filesize)) {
            filelist.emplace_back(filename);

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


    void MemFlash::transferSingleToFlash(const std::string &filepath) {

        SPI.setMOSI(C_SDCARD_MOSI_PIN);
        SPI.setSCK(C_SDCARD_SCK_PIN);

        if (!SD.begin(C_SDCARD_CS_PIN)) {
#ifdef VERBOSE
            Serial.println("SD Card: initialization failed!");
#endif
        }

        File f = SD.open(filepath.c_str());

        if (!f) {
#ifdef VERBOSE
            Serial.print("transferToFlash: error reading file ");
            Serial.print(filepath.c_str());
            Serial.println(" from SD");
#endif
        }

        std::string basename = lbs::getBasename(const_cast<std::string &>(filepath));

        if (!SerialFlash.create(basename.c_str(), f.size())) {
#ifdef VERBOSE
            Serial.print("transferToFlash: error creating file");
            Serial.print(basename.c_str());
            Serial.println(" on Flash");
#endif
        }

        SerialFlashFile ff = SerialFlash.open(basename.c_str());

        char byte;

        for (uint i = 0; i < f.size(); i++) {
            f.readBytes(&byte, 1);
            ff.write(&byte, 1);
        }
    }

    MemFlash::MemFlash() {

        SerialFlash.begin(C_FLASH_PIN)

    }
}