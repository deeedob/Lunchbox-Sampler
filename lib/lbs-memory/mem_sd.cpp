#include "mem_sd.hpp"

using namespace lbs;

MemSD::MemSD() {
    SPI.setMOSI(C_SDCARD_MOSI_PIN);
    SPI.setSCK(C_SDCARD_SCK_PIN);

    if (!SD.begin(C_SDCARD_CS_PIN)) {
        Serial.println("SD Card: initialization failed!");
    }
}

/*std::vector<String> MemSD::getAllSamplepacks() {
    SD.open("/packs");
    return std::vector<String>();
}*/



