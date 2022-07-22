#pragma once
#include <SD.h>
#include <SerialFlash.h>
#include <Bounce.h>
#include <iostream>
#include <memory>
#include <vector>
#include <tuple>
#include <CSV_Parser.h>


namespace lbs {
    static const std::string settingsfile = "/mapping.csv";
    static const std::string packdir = "/packs/";

    class MemSD {
    public:
        static MemSD &getInstance();

    public:
        bool exists(std::string file);
        File *getSample(std::string sampleName);
        std::vector<std::tuple<std::string, int8_t, std::string, std::string>> *readSettings(std::string path);

    private:
        static inline String finishCSVLine(String line);
        MemSD();
    };
}