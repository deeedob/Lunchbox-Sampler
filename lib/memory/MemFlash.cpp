#include <define_t40.hpp>
#include <map>
#include <tuple>
#include "MemSD.hpp"
#include "MemFlash.hpp"

using namespace lbs;

const std::unordered_map<std::string, uint8_t> MemFlash::MidiMapping::pitches = {{"c", 0}, {"csh", 1}, {"d", 2}, {"dsh", 3}, {"e", 4}, {"f", 5}, {"fsh", 6}, {"g", 7}, {"gsh", 8}, {"a", 9}, {"ash", 10}, {"b", 11}};

std::string
&toLower(std::string &str)
{
    std::transform(str.begin(), str.end(), str.begin(), ::tolower);
    return str;
}

MemFlash
&MemFlash::getInstance()
{
    static MemFlash *instance = new MemFlash();

    if (!SerialFlash.begin(FLASH_PIN)) {
#ifdef VERBOSE
        Serial.println("loadSamplePack: Unable to access SPI Flash chip");
#endif
        //TODO: Error handling
    }

    return *instance;
}

// load Sample Pack from SD to Flash
bool
MemFlash::loadSamplePack(const std::string packName)
{
    MemSD &sd = MemSD::getInstance();

    auto settings = sd.readSettings(lbs::packdir + packName + lbs::settingsfile);

    int count = 0;
    for (auto i = settings->begin(); i < settings->end(); i++) {
        std::string &pitch = std::get<0>(*i);
        int8_t &oct = std::get<1>(*i);
        std::string &sample = std::get<2>(*i);
        std::string &mode = std::get<3>(*i);
        this->mapping.add(pitch, oct, sample, mode);
    }

    for (auto it = this->mapping.getSampleList()->begin(); it < this->mapping.getSampleList()->end(); it++) {
       SerialFlash.create(it->c_str(), )
    }

    //TODO: adjustSize() (strips samplesizes down if size of sample pack too large

}

MemFlash::MemFlash() {
}


MemFlash::MidiMapping::MidiMapping()
{
    for (int i = 0; i < 128; i++) {
        this->samples[i] = nullptr;
        this->mode[i] = MidiMapping::PlaybackMode::ONESHOT;
    }
}

bool MemFlash::MidiMapping::add(std::string pitch, int8_t octave, std::string sample, std::string mode) {

    if (octave < -1 || octave > 9) {
        return false;
    }

    auto pitchNo = this->pitches.at(pitch);

#ifdef VERBOSE
    Serial.print("check pitchNo: ");
    Serial.println(pitchNo);
#endif

    int16_t note = pitchNo + (octave + 1) * 12;

    if (note < 0 || note > 127) {
        return false;
    }

    this->samples[(uint8_t) note] = sample;
    this->mode[(uint8_t) note] = MemFlash::MidiMapping::getModeFromString(mode);
    return true;
}

inline MemFlash::MidiMapping::PlaybackMode
MemFlash::MidiMapping::getModeFromString(std::string mode)
{
    if (mode == "TOGGLE") {
        return MemFlash::MidiMapping::PlaybackMode::TOGGLE;
    } else if (mode == "LOOP") {
        return MemFlash::MidiMapping::PlaybackMode::LOOP;
    } else {
        return MemFlash::MidiMapping::PlaybackMode::ONESHOT;
    }
}

std::vector<std::string>
*MemFlash::MidiMapping::getSampleList() {
    auto *res = new std::vector<std::string>;
    for (int i = 0; i < 127; i++) {
        if (this->samples[i] != "") {
            res->push_back(samples[i]);
        }
    }
    return res;
}
