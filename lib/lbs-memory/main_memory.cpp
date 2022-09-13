#include <set>
#include "main_memory.hpp"

using namespace lbs;

const String MainMemory::mPackRootDir = "/samplepacks/";

const std::map<String, PitchVal> MainMemory::pitches = {{"c",  PitchVal{0}},
                                                        {"c#", PitchVal{1}},
                                                        {"d",  PitchVal{2}},
                                                        {"d#", PitchVal{3}},
                                                        {"e",  PitchVal{4}},
                                                        {"f",  PitchVal{5}},
                                                        {"f#", PitchVal{6}},
                                                        {"g",  PitchVal{7}},
                                                        {"g#", PitchVal{8}},
                                                        {"a",  PitchVal{9}},
                                                        {"a#", PitchVal{10}},
                                                        {"b",  PitchVal{11}}};

uint MainMemory::freeSpaceFlash = 0;
String MainMemory::currentPack = "";

String MainMemory::sampleMapping[] = {""};
playbackMode MainMemory::modeMapping[] = {ONESHOT};

/**
 * @brief
 * @param
 * @return
 */
size_t findPayload(File &f) {

    size_t pos = f.position();
    f.seek(0);

    if (!f) {
        return 0;
    }

    char byte[4];
    while (f.available()) {
        f.read(&byte, 1);
        if (byte[0] == 'd' && f.available() > 3 && f.peek() == 'a') {
            f.read(&byte, 3);
            byte[3] = '\0';
            if (strcmp(byte, "ata") == 0) {
                size_t res = f.position();
                f.seek(pos);
                return res;
            }
        }
    }
    return 0;
}

/**
 * @brief
 * @param
 * @return
 */
size_t getRawAudioSize(const String &fullpath) {
    File f = SD.open(fullpath.c_str());
    size_t audioPos = findPayload(f);
    if (audioPos > 0) {
        audioPos--;
    }
    return f.size() - audioPos;
}

/**
 * @brief
 * @param
 * @return
 */
playbackMode parsePlaybackMode(const String &string) {
    if (string == "LOOP") {
        return lbs::LOOP;
    }
    return lbs::ONESHOT;
}

/**
 * @brief
 * @param
 * @return
 */
String parseStringFromPlaybackMode(playbackMode mode) {
    return (const String[]) {
            "ONESHOT",
            "LOOP"
    }[mode];
}

/**
 * @brief
 * @param
 * @return
 */
AUDIOTYPE getAudioType(const String &sampleName) {
    String ext = sampleName.substring(sampleName.lastIndexOf(".")).toLowerCase();
    if (ext.compareTo(".wav") == 0) {
        return AUDIOTYPE::WAV;
    } else if (ext.compareTo(".raw") == 0) {
        return AUDIOTYPE::RAW;
    }

    return AUDIOTYPE::INVALID;
}

/* helper functions to print list all files recursively */
void printSpaces(int num) {
    for (int i = 0; i < num; i++) {
        Serial.print(" ");
    }
}

void printTime(const DateTimeFields tm) {
    const char *months[12] = {
            "January", "February", "March", "April", "May", "June",
            "July", "August", "September", "October", "November", "December"
    };
    if (tm.hour < 10) Serial.print('0');
    Serial.print(tm.hour);
    Serial.print(':');
    if (tm.min < 10) Serial.print('0');
    Serial.print(tm.min);
    Serial.print("  ");
    Serial.print(tm.mon < 12 ? months[tm.mon] : "???");
    Serial.print(" ");
    Serial.print(tm.mday);
    Serial.print(", ");
    Serial.print(tm.year + 1900);
}

void printDirectory(File dir, int num_spaces) {
    while (true) {
        File entry = dir.openNextFile();
        if (!entry) {
            //Serial.println("** no more files **");
            break;
        }
        printSpaces(num_spaces);
        Serial.print(entry.name());
        if (entry.isDirectory()) {
            Serial.println("/");
            printDirectory(entry, num_spaces + 2);
        } else {
            // files have sizes, directories do not
            unsigned int n = log10(entry.size());
            if (n > 10) n = 10;
            printSpaces(50 - num_spaces - strlen(entry.name()) - n);
            Serial.print("  ");
            Serial.print(entry.size(), DEC);
            DateTimeFields datetime;
            if (entry.getModifyTime(datetime)) {
                printSpaces(4);
                printTime(datetime);
            }
            Serial.println();
        }
        entry.close();
    }
}

/**
 * @brief
 * @param
 * @return
 */
bool MainMemory::initMemory() {
    SPI.setMOSI(C_SDCARD_MOSI_PIN);
    SPI.setSCK(C_SDCARD_SCK_PIN);

    if (!SD.begin(C_SDCARD_CS_PIN)) {
        Serial.println("SD Card: initialization failed!");
        return false;
    }

    if (!SerialFlashChip::begin(C_FLASH_PIN)) {
#ifdef VERBOSE
        Serial.println("Error initializing Flash Chip!");
#endif
        return false;
    }

    return true;
}

/**
 * @brief
 * @param
 * @return
 */
void MainMemory::printAllFilesFromSD() {
    Serial.println("_____________START PRINTDIR_______________");
    File root = SD.open("/");
    printDirectory(root, 0);
    Serial.println("_____________END  PRINTDIR_______________");
}

/**
 * @brief
 * @param
 * @return
 */
uint64_t MainMemory::getFreeSpacefromSD() {
    return SD.totalSize() - SD.usedSize();
}

/**
 * @brief
 * @param
 * @return
 */
std::vector<String> MainMemory::getFilelistFromFlash() {
    unsigned int count = 0;
    char filename[64];
    uint32_t filesize;
    std::vector<String> filelist;

    /* announce reading filelist (prob setting file iterator to 0) */
    SerialFlashChip::opendir();

    while (SerialFlashChip::readdir(filename, sizeof(filename), filesize)) {
        filelist.emplace_back(filename );

#ifdef VERBOSE
        Serial.print("File " );
        Serial.print(count );
        Serial.print(F(": " ) );
        Serial.print(filename );
        Serial.print(F("  " ) );
        Serial.print(filesize );
        Serial.print(F(" bytes" ) );
        Serial.println();
#endif
        count++;
    }

#ifdef VERBOSE
    if (count == 0) Serial.println("Flash is empty");
#endif

    return filelist;
}

/**
 * @brief
 * @param
 * @return
 */
bool MainMemory::flashEmpty() {
    if (getFreeSpacefromFlash() == C_FLASHSIZE) {
        return true;
    }
    return false;
}

/**
 * @brief
 * @param
 * @return
 */
void MainMemory::eraseFlash() {
#ifdef VERBOSE
    Serial.println("purgeFlash: deleting all Files. Please wait until 'ready' (ca.40s)");
#endif
    SerialFlashChip::eraseAll();
    while (!SerialFlashChip::ready()) {}

#ifdef VERBOSE
    Serial.println("ready");
#endif
    freeSpaceFlash = C_FLASHSIZE;
    currentPack = "";
}

/**
 * @brief
 * @param
 * @return
 */
uint MainMemory::getFreeSpacefromFlash() {
    return freeSpaceFlash;
}

/**
 * @brief
 * @param
 * @return
 */
String MainMemory::getNoteName(uint8_t midiNote) {
    uint8_t octave = (midiNote / 12) - 1;
    uint8_t noteVal = midiNote % 12;
    String noteValTxt;

    switch (noteVal) {
        case 0:
            return "C" + String(octave);
        case 1:
            return "C#" + String(octave);
        case 2:
            return "D" + String(octave);
        case 3:
            return "D#" + String(octave);
        case 4:
            return "E" + String(octave);
        case 5:
            return "F" + String(octave);
        case 6:
            return "F#" + String(octave);
        case 7:
            return "G" + String(octave);
        case 8:
            return "G#" + String(octave);
        case 9:
            return "A" + String(octave);
        case 10:
            return "A#" + String(octave);
        case 11:
            return "B" + String(octave);
        default:
            return "C" + String(octave);
    }
}

/**
 * @brief
 * @param
 * @return
 */
std::vector<String> MainMemory::getAvailableSamplepacks() {
    auto res = std::vector<String>();
    File packdir = SD.open(mPackRootDir.c_str());

    File entry;
    while ((entry = packdir.openNextFile())) {
        res.push_back(entry.name());
    }

    return res;
}

/**
 * @brief
 * @param
 * @return
 */
std::vector<String> MainMemory::getSampleNamesFromPack(const String &packName) {
    std::vector<String> filelist;
    String path = mPackRootDir + packName;
#ifdef VERBOSE
    Serial.print("getSampleNamesFromPack(): Listing Samples from ");
    Serial.println(path);
#endif

    File sampleDir = SD.open(path.c_str());
    File entry;
    while ((entry = sampleDir.openNextFile())) {
        String name = entry.name();
        if (!entry.isDirectory() && getAudioType(name) != AUDIOTYPE::INVALID) {
#ifdef VERBOSE
            Serial.print("getSampleNamesFromPack(): Audio file ");
            Serial.print(entry.name());
            Serial.println(" found");
#endif
            filelist.push_back(name);
        } else {
#ifdef VERBOSE
            Serial.print("getSampleNamesFromPack(): Skipping ");
            Serial.print(entry.name());
            Serial.println(", no audio file");
#endif
        }
    }

    entry.close();
    sampleDir.close();
    std::sort(filelist.begin(), filelist.end());

    return filelist;
}

void MainMemory::loadSamplepack(const String &packName) {
    MainMemory::eraseFlash();
    MainMemory::createStdMappingFile(packName);
    String fullpath = mPackRootDir + "/" + packName + "/";
    uint count = 0;

    loadMappingFile(packName);
    auto samples = getSampleNamesFromPack(packName);

    uint sum = 0;
    for (const auto &sample: samples) {
        sum += getRawAudioSize(fullpath + sample);
    }

    double stripFactor = 0;
    if (sum > getFreeSpacefromFlash()) {
        stripFactor = (float_t) getFreeSpacefromFlash() / (float_t) sum;
#ifdef VERBOSE
        Serial.print("Stripfactor for samples is ");
        Serial.println(stripFactor, 4);
#endif
    }

    size_t sampleSize = 0;
    for (auto &i: samples) {
        if (i.length() > 0) {
            if ((sampleSize = transferSampleToFlash(fullpath + i,
                                                    round(getRawAudioSize(fullpath + i) * stripFactor)))) {
                count++;
                freeSpaceFlash -= sampleSize;
            }
        }
    }

    for (auto &i: sampleMapping) {
        if (i.length() > 0) {
            i = i.substring(0, i.lastIndexOf('.'));
        }
    }

    currentPack = packName;

#ifdef VERBOSE
    Serial.print(packName);
    Serial.print(", ");
    Serial.print(count);
    Serial.println(" Samples loaded!");
    Serial.print("Free Space on Flash: ");
    Serial.println(freeSpaceFlash);
#endif
}

/**
 * @brief
 * @param
 * @return
 */
String MainMemory::getLoadedPackName() {
    return currentPack;
}

/**
 * @brief
 * @param
 * @return
 */
String MainMemory::getSampleFromNote(uint8_t note) {
    String res = sampleMapping[note];
    return res;
}

/**
 * @brief
 * @param
 * @return
 */
bool MainMemory::setSampleForNote(const String &sampleName, uint8_t midiNote) {

    if (currentPack == "") {
#ifdef VERBOSE
        Serial.println("setSampleForNote(): No samplepack loaded, load a samplepack");
#endif
        return false;
    }

    if (!SerialFlash.exists(sampleName.substring(0, sampleName.lastIndexOf('.')).c_str())) {
#ifdef VERBOSE
        Serial.print("setSampleForNote(): sample");
        Serial.print(sampleName);
        Serial.println(" does not exist");
#endif
        return false;
    }

    sampleMapping[midiNote] = sampleName.substring(0, sampleName.lastIndexOf("."));
    return true;
}

/**
 * @brief
 * @param
 * @return
 */
bool MainMemory::createStdMappingFile(const String &packName) {
    String settingsPath = mPackRootDir + "/" + packName + "/" + C_SETTINGS_FILE;

    if (!SD.exists(settingsPath.c_str())) {
#ifdef VERBOSE
        Serial.println("createStdMappingFile(): No mapping found, creating new standard mapping");
#endif
        File settings = SD.open(settingsPath.c_str(), FILE_WRITE_BEGIN);
        auto samples = getSampleNamesFromPack(packName);
        //starting at C4
        uint8_t currentNote = 60;
        if (samples.size() > 67) {
#ifdef VERBOSE
            Serial.println("createStdMappingFile(): Many samples in folder, starting at octave 0");
#endif
            currentNote = 12;
        }
        String line;
        for (auto &sample: samples) {
            //stop at highest midi note
            if (currentNote > 127) {
                break;
            }
            line = (getNoteName(currentNote) + "," + sample + ",ONESHOT\n");
            settings.write(line.c_str());
            currentNote++;
        }
        settings.close();
        return true;
#ifdef VERBOSE
    } else {
        Serial.print("createStdMappingFile(): Mapping ");
        Serial.print(settingsPath.c_str());
        Serial.println(" exists and will not be overwritten, either change manually or delete");
#endif
    }

    return false;
}

/**
 * @brief
 * @param
 * @return
 */
bool MainMemory::deleteMappingFile(const String &packName) {
    String settingspath = mPackRootDir + "/" + packName + "/" + C_SETTINGS_FILE;
    if (SD.exists(settingspath.c_str())) {
#ifdef VERBOSE
        Serial.println("deleteMappingFile(): mapping exists... deleting");
#endif
        return SD.remove(settingspath.c_str());
    }
#ifdef VERBOSE
    Serial.println("deleteMappingFile(): No mapping existing... nothing to do");
#endif
    return false;
}

/**
 * @brief
 * @param
 * @return
 */
void MainMemory::createAllStdMappingFiles() {
    auto packs = getAvailableSamplepacks();
    for (auto &pack: packs) {
        createStdMappingFile(pack);
    }
}

/**
 * @brief
 * @param
 * @return
 */
bool MainMemory::saveCurrentMappingToFile() {
    uint8_t index = 0;

    if (currentPack == "") {
#ifdef VERBOSE
        Serial.println("saveCurrentMappingToFile(): No samplepack loaded, load samplepack");
#endif
        return false;
    }

    deleteMappingFile(currentPack);
    File mapping = SD.open((mPackRootDir + "/" + currentPack + "/" + C_SETTINGS_FILE).c_str(), FILE_WRITE_BEGIN);
    for (auto &i: sampleMapping) {
        if (i != "") {
            mapping.write(
                    (getNoteName(index) + "," + String(i) + "," + parseStringFromPlaybackMode(modeMapping[index]) +
                     "\n").c_str());
        }
        index++;
    }
    mapping.close();
    return false;
}

/**
 * @brief
 * @param
 * @return
 */
void MainMemory::printMapping() {
    for (int i = 0; i < 128; i++) {
        Serial.print("Midi Note ");
        Serial.print(i);
        Serial.print(": ");
        Serial.print(sampleMapping[i]);
        Serial.print(", ");
        Serial.println(modeMapping[i]);
    }
}

/**
 * @brief
 * @param
 * @return
 */
void MainMemory::loadMappingFile(const String &packName) {
    const String pack = mPackRootDir + packName + "/";
    const String path = pack + C_SETTINGS_FILE;

    // Check pack exists
    if (!SD.exists(path.c_str())) {
#ifdef VERBOSE
        Serial.print("ERROR in loadMappingFile: filepath ");
        Serial.print(path.c_str());
        Serial.println(" does not exist!");
#endif
        return;
    }

    // Check file readable
    File settings = SD.open(path.c_str(), FILE_READ);
    if (!settings) {
#ifdef VERBOSE
        Serial.println("ERROR in loadMappingFile: could not read file!");
#endif
        return;
    }

    //Parse CSV file
    String sample;
    String note, mode;
    size_t pos, pos2;
    auto result = std::vector<std::tuple<String, String, String>>();
    auto res = std::tuple<String, String, String>();
    String line;
    int8_t midiNote;
    int8_t octave;

    while (settings.available()) {
        line = settings.readStringUntil('\n');
        pos = line.indexOf(',');
        pos2 = line.indexOf(',', pos + 1);

        sample = line.substring(pos + 1, pos2).replace(" ", "");

        //Check Sample valid
        if (sample.length() <= 0 || !SD.exists((pack + sample).c_str())) {
#ifdef VERBOSE
            Serial.print("CSV PARSE SKIP: invalid sample ");
            Serial.println(sample);
#endif
            continue;
        }

        //PARSE NOTE TO MIDI VALUE AND TEST VALIDITY
        note = line.substring(0, pos).replace(" ", "");

        if (note.length() < 2) {
#ifdef VERBOSE
            Serial.print("CSV PARSE SKIP: invalid note ");
            Serial.println(note);
#endif
            continue;
        }

        octave = note.substring(note.length() - 1, note.length()).toInt();
        note = note.substring(0, note.length() - 1).toLowerCase();

        if (octave < 0 || octave > 9 || MainMemory::pitches.at(note).value < 0) {
#ifdef VERBOSE
            Serial.print("CSV PARSE SKIP: invalid note value or octave ");
            Serial.println(note);
            Serial.print(" ");
            Serial.print(octave);
#endif
            continue;
        }

        //Calculate midi value (C4 = 60)
        midiNote = 12 * (octave + 1) + MainMemory::pitches.at(note).value;

        //Parse playback mode
        mode = line.substring(pos2 + 1, line.length());
        if (mode.length() <= 0) {
            mode = "ONESHOT";
        }

#ifdef VERBOSE
        Serial.print("loadMappingFile(): PARSED: ");
        Serial.print(note);
        Serial.print(octave);
        Serial.print(" ");
        Serial.print(sample);
        Serial.print(" ");
        Serial.print(mode);
        Serial.print(" Note: ");
        Serial.print(" ");
        Serial.println(midiNote);
#endif

        // Save name without wave extension (as named on flash)
        sampleMapping[midiNote] = sample;
        modeMapping[midiNote] = parsePlaybackMode(mode);
    }

#ifdef VERBOSE
    Serial.println("Finished reading settings");
#endif
}

/**
 * @brief
 * @param
 * @return
 */
uint MainMemory::transferSampleToFlash(const String &filepath, const size_t sampleSize = 0) {

    Serial.print("transferSampleToFlash(): begin upload of ");
    Serial.println(filepath.c_str());

    File f = SD.open(filepath.c_str());

    if (!f) {
#ifdef VERBOSE
        Serial.print("transferToFlash: error reading file ");
        Serial.print(filepath.c_str());
        Serial.println(" from SD" );
#endif
        return 0;
    }
    String basename = filepath.substring(filepath.lastIndexOf('/') + 1);

    if(getAudioType(basename ) == AUDIOTYPE::WAV ) {
        size_t payloadPos = findPayload(f);
        f.seek(payloadPos);

#ifdef VERBOSE
        if (f.available() < 1) {
            Serial.println("WARNING: WAV file will be loaded with header!");
            f.seek(0);
        }
#endif
    }

    basename = basename.substring(0, basename.lastIndexOf('.'));

    size_t filelength = f.available();

    if (sampleSize) {
        filelength = sampleSize;
#ifdef VERBOSE
        Serial.print("Stripping filelength from ");
        Serial.print(f.available());
        Serial.print(" to ");
        Serial.print(filelength);
        Serial.println(" bytes");
#endif
    }

    if (!SerialFlashChip::createErasable(basename.c_str(), filelength)) {
#ifdef VERBOSE
        Serial.print("transferToFlash: error creating file \"");
        Serial.print(basename.c_str());
        Serial.println("\" on Flash");
#endif
        return 0;
    }

    SerialFlashFile ff = SerialFlashChip::open(basename.c_str());

    char byte;
    for(int i = 0; i < ff.size(); i++ ) {
        f.read(&byte, 1 );
        ff.write(&byte, 1 );
    }

    f.close();
    size_t size = ff.size();
    ff.close();
    return size;
}