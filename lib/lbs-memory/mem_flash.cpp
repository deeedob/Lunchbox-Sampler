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

        Serial.println("Transfer to Flash function begin");

        File f = SD.open(filepath.c_str());

        if (!f) {
#ifdef VERBOSE
            Serial.print("transferToFlash: error reading file ");
            Serial.print(filepath.c_str());
            Serial.println(" from SD");
#endif
        }

        std::string basename = lbs::getBasename(const_cast<std::string &>(filepath));

        if (!SerialFlash.createErasable(basename.c_str(), f.size())) {
#ifdef VERBOSE
            Serial.print("transferToFlash: error creating file");
            Serial.print(basename.c_str());
            Serial.println(" on Flash");
#endif
        }

        SerialFlashFile ff = SerialFlash.open(basename.c_str());

        char *buf = new char[ff.size()];
        f.read(buf, ff.size());
        ff.write(buf, ff.size());

    }

    MemFlash::MemFlash() {

        if (!SerialFlash.begin(C_FLASH_PIN)) {
#ifdef VERBOSE
            Serial.println("Error initializing Flash Chip!");
#endif
        }

    }

    void MemFlash::eraseFlash() {
#ifdef VERBOSE
        Serial.println("purgeFlash: deleting all Files. Please wait until 'ready' (ca.40s)");
#endif
        SerialFlash.eraseAll();
        while (!SerialFlash.ready()) {}

#ifdef VERBOSE
        Serial.println("ready");
#endif
    }

    MemFlash::MemFlash() {

        SerialFlash.begin(C_FLASH_PIN)

    }
}