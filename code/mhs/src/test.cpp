#include <Arduino.h>
#include <Audio.h>
#include <Wire.h>
#include <SD.h>
#include <SPI.h>
#include <SerialFlash.h>
#include <Adafruit_SSD1327.h>
#include <Encoder.h>
#include <Bounce.h>

#define POT0 A0
#define POT1 A1
#define POT2 A10
#define POT3 A12

#define DISPLAY_CLK 16
#define DISPLAY_SDA 17
#define DISPLAY_RST 9

#define ROTARY_A 2
#define ROTARY_B 3
#define ROTARY_SW 4

#define MIDI_IN 0
#define MIDI_OUT 1

#define SDCARD_CS_PIN   10
#define SDCARD_MOSI_PIN 11
#define SDCARD_SCK_PIN  13
#define FLASH_PIN 6

void testDisplay();
void testMicrophone();
void playFile(const char *filename);

//Testing Flash
void DeleteAllFilesOnFlash();
void rawHardwareTestFlash();
void testSDtoSPI();
void ListFiles();

//DeleteAllFilesOnFlash:
float eraseBytesPerSecond(const unsigned char *id);

//rawHardwareTestFlash:
bool test();
const char * id2chip(const unsigned char *id);
bool equal_signatures(const unsigned char *data1, const unsigned char *data2);
void create_signature(unsigned long address, unsigned char *data);
void print_signature(const unsigned char *data);
bool is_erased(const unsigned char *data, unsigned int len);
void printbuf(const void *buf, uint32_t len);

//testSDtoSPI:
void error(const char *message);
bool compareFiles(File &file, SerialFlashFile &ffile);

//List Files:
void spaces(int num);

SerialFlashFile file;
const unsigned long testIncrement = 4096;

AudioInputI2S audioInput;
AudioPlaySdWav playSdWav;
AudioOutputI2S audioOutput;
AudioControlSGTL5000 audioShield;

AudioConnection          patchCord1(playSdWav, 0, audioOutput, 0);
AudioConnection          patchCord2(playSdWav, 1, audioOutput, 1);


Adafruit_SSD1327 display(128, 128, &Wire1, DISPLAY_RST, 1000000);
Encoder enc(ROTARY_A, ROTARY_B);
Bounce btn_rotary = Bounce(ROTARY_SW, 15);

elapsedMillis ms = 0;

char *wavFiles[] = {"01.WAV", "02.WAV", "03.WAV", "04.WAV", "05.WAV", "06.WAV"};
int wavNum = 0;

void setup() {
    SPI.setSCK(SDCARD_SCK_PIN);
    SPI.setMOSI(SDCARD_MOSI_PIN);

    //uncomment for RawHardwareTest,if SDCard is in
    //pinMode(SDCARD_CS_PIN, INPUT_PULLUP);
    //pinMode(ROTARY_SW, INPUT_PULLUP);

    Serial.begin(9600);
    Serial.println("_________begin hw-test___________-");
    //SD.begin(SDCARD_CS_PIN);
    //pinMode(ROTARY_SW, INPUT_PULLUP);


    /* Test Flash:
    Step 1: DeleteAllFilesOnFlash();
    Step 2: rawHardwareTestFlash();
    Step 3: DeleteAllFilesOnFlash again();
    Step 4: testSDtoSPI;    -----> Only able to save WAV01, because of Memory Volume;
    Step 5: ListFiles;
    Step 6: DeleteAllFilesOnFlash */

    //DeleteAllFilesOnFlash();
    //rawHardwareTestFlash();
    //DeleteAllFilesOnFlash();
    testSDtoSPI();
    ListFiles();
    DeleteAllFilesOnFlash();

    //testDisplay();
    //testMicrophone();

    Serial.println("Testing the SD-Card");
    if (!(SD.begin(SDCARD_CS_PIN))) {
        // stop here, but print a message
        Serial.println("Unable to access the SD card.  Program halted.");
        delay(500);
    }
    AudioMemory(8);
    audioShield.enable();
    audioShield.volume(0.5);


    // reset audio resource usage stats.
    // useful if tracking max usage in main program
    AudioProcessorUsageMaxReset();
    AudioMemoryUsageMaxReset();

}
void inBounds(int& id) {
    if(id < 0) id = 5;
    else if(id > 5) id = 0;
}

long oldPos = -999;
void loop() {

    /*long newPos = enc.read();
    delay(100);
    if(newPos != oldPos) {
        if(newPos > oldPos) {
            inBounds(++wavNum);
            Serial.println(wavNum);
        }
        else {
            inBounds(--wavNum);
            Serial.println(wavNum);
        }
        oldPos = newPos;
        playSdWav.stop();
        Serial.print("Start playing: ");
        Serial.println(wavFiles[wavNum]);
        playSdWav.play(wavFiles[wavNum]);
        delay(10);

        display.clearDisplay();
        display.setTextSize(2);
        display.setTextWrap(false);
        display.setTextColor(SSD1327_WHITE);

        display.setCursor(display.width()/2-50,display.height()/2);
        display.write(wavFiles[wavNum]);
        display.display();
    }
    btn_rotary.update();
    if(btn_rotary.fallingEdge()) {
        playSdWav.togglePlayPause();
        testMicrophone();
    }
    if(btn_rotary.risingEdge()) playSdWav.togglePlayPause();

    float volume = (float) analogRead(POT0);
    audioShield.volume(volume/1024.f);*/

}

void testMicrophone() {
    Serial.println("Testing the AOM-6738P-R mic");
    Serial.println("use your headphones!");
    AudioMemory(8);
    audioShield.enable();
    audioShield.inputSelect(AUDIO_INPUT_MIC);  // select mic or line-in for audio shield input source
    audioShield.volume(0.8);
    float pot2 = (float) analogRead(POT1) / 1024.f;
    audioShield.micGain((int)(pot2*45));
    while(btn_rotary.risingEdge()){}
    audioShield.disable();
}
void testDisplay() {

    Serial.println("Testing the SSD1327 OLED in I2C mode");
    if ( ! display.begin(0x3D) ) {
        Serial.println("Unable to initialize OLED");
        while (1) yield();
    }
    display.clearDisplay();
    //display.setFont(&FreeMono12pt7b);

    // test display functionality
    display.setTextSize(2);
    display.setTextWrap(true);
    display.setTextColor(SSD1327_WHITE);

    display.setCursor(display.width()/2-50,display.height()/2);
    display.write("MHS");

    display.drawCircle(display.width()/2, display.height()/2, 50,15);
    display.display();

    // pixel draw test
    for( int i = 0; i < display.height(); i++ ) {
        for( int j = 0; j < display.width(); j++ ) {
            display.drawPixel(j,i,(j*2)%255);
            display.display();
        }
    }
    Serial.print("time >> drawPixel()");
    Serial.print(ms);
    Serial.print(" ms :: fps: ");
    Serial.println(1000.f / (float) ms);
    display.display();
    delay(1000);
    display.clearDisplay();
    display.fillScreen(0xFF);

    ms = 0;
    for( int i = 0; i < display.height() ; i++ ) {
        display.drawLine(0,i,display.width(), i, i*2);
        display.display();
    }

    Serial.print("time >> drawLine()");
    Serial.print(ms);
    Serial.print(" ms :: fps: ");
    Serial.println(1000.f/ (float) ms);
    display.display();
    delay(1000);
    display.clearDisplay();

    ms = 0;
    for( int i = 0; i < display.height() ; i++ ) {
        display.drawFastHLine(0,i,display.width(), (i*2) % 255);
        display.display();
    }
    Serial.print("time >> drawFastHLine()");
    Serial.print(ms);
    Serial.print(" ms :: fps: ");
    Serial.println(1000.f/ (float) ms);
    display.display();
    delay(1000);
    display.clearDisplay();

    ms = 0;
    display.fillRoundRect(0,0,display.width(), display.height(),25,0xFF);
    Serial.print("time >> fillRoundRect()");
    Serial.print(ms);
    Serial.print(" ms :: fps: ");
    Serial.println(1000.f / (float) ms);
    display.display();
    delay(1000);
    display.clearDisplay();

    ms = 0;
    display.writeFillRect(0,85,display.width(), display.height(),0xff);
    Serial.print("time >> writeFillRect()");
    Serial.print(ms);
    Serial.print(" ms :: fps: ");
    Serial.println(1000.f / (float) ms);
    display.display();
    delay(1000);
    display.setCursor(5,105);
    display.setTextSize(1);
    display.setTextColor(0x00);
    display.write("MHSMHSMHSMHSMHSMHSMHSMHSMHS");
    display.display();

}

void ListFiles()
{
    while (!Serial) ;
    delay(100);
    Serial.println(F("All Files on SPI Flash chip:"));

    if (!SerialFlash.begin(FLASH_PIN)) {
    error("Unable to access SPI Flash chip");
    }

    int count=0;
    SerialFlash.opendir();
    while (1) {
        char filename[64];
        uint32_t filesize;

        if (SerialFlash.readdir(filename, sizeof(filename), filesize)) {
            Serial.print(F("  "));
            Serial.print(filename);
            spaces(20 - strlen(filename));
            Serial.print(F("  "));
            Serial.print(filesize);
            Serial.print(F(" bytes"));
            Serial.println();
            count++;
        }
        else {
            if(count==0)
            {
                Serial.println("SPI Flash chip is empty");
            }
            break; // no more files
        }
    }
}

void testSDtoSPI()
{
    Serial.println(F("Copy all files from SD Card to SPI Flash"));

    if (!SD.begin(SDCARD_CS_PIN)) {
        error("Unable to access SD card");
    }
    if (!SerialFlash.begin(FLASH_PIN)) {
        error("Unable to access SPI Flash chip");
    }

    File rootdir = SD.open("/");
    while (1) {
        // open a file from the SD card
        Serial.println();
        File f = rootdir.openNextFile();
        if (!f) break;
        const char *filename = f.name();
        Serial.print(filename);
        Serial.print(F("    "));
        unsigned long length = f.size();
        Serial.println(length);

        // check if this file is already on the Flash chip
        if (SerialFlash.exists(filename)) {
            Serial.println(F("  already exists on the Flash chip"));
            SerialFlashFile ff = SerialFlash.open(filename);
            if (ff && ff.size() == f.size()) {
                Serial.println(F("  size is the same, comparing data..."));
                if (compareFiles(f, ff) == true) {
                    Serial.println(F("  files are identical :)"));
                    f.close();
                    ff.close();
                    continue;  // advance to next file
                } else {
                    Serial.println(F("  files are different"));
                }
            } else {
                Serial.print(F("  size is different, "));
                Serial.print(ff.size());
                Serial.println(F(" bytes"));
            }
            // delete the copy on the Flash chip, if different
            Serial.println(F("  delete file from Flash chip"));
            SerialFlash.remove(filename);
        }

        // create the file on the Flash chip and copy data
        if (SerialFlash.create(filename, length)) {
            SerialFlashFile ff = SerialFlash.open(filename);
            if (ff) {
                Serial.print(F("  copying"));
                // copy data loop
                unsigned long count = 0;
                unsigned char dotcount = 9;
                while (count < length) {
                    char buf[256];
                    unsigned int n;
                    n = f.read(buf, 256);
                    ff.write(buf, n);
                    count = count + n;
                    Serial.print(".");
                    if (++dotcount > 100) {
                        Serial.println();
                        dotcount = 0;
                    }
                }
                ff.close();
                if (dotcount > 0) Serial.println();
            } else {
                Serial.println(F("  error opening freshly created file!"));
            }
        } else {
            Serial.println(F("  unable to create file"));
        }
        f.close();
    }
    rootdir.close();
    delay(10);
    Serial.println(F("Finished All Files"));
}

void DeleteAllFilesOnFlash()
{
    unsigned long startMillis = millis();
    while (!Serial && (millis() - startMillis < 10000)) ;
    delay(100);
    SerialFlash.begin(FLASH_PIN);

    if (!SerialFlash.begin(FLASH_PIN)) {
        while (1) {
            Serial.println(F("Unable to access SPI Flash chip"));
            delay(1000);
        }
    }
    unsigned char id[5];
    SerialFlash.readID(id);
    unsigned long size = SerialFlash.capacity(id);

    if (size > 0) {
        Serial.print(F("Flash Memory has "));
        Serial.print(size);
        Serial.println(F(" bytes."));
        Serial.println(F("Erasing ALL Flash Memory:"));
        // Estimate the (lengthy) wait time.
        Serial.print(F("  estimated wait: "));
        int seconds = (float)size / eraseBytesPerSecond(id) + 0.5;
        Serial.print(seconds);
        Serial.println(F(" seconds."));
        Serial.println(F("  Yes, full chip erase is SLOW!"));
        SerialFlash.eraseAll();
        unsigned long dotMillis = millis();
        unsigned char dotcount = 0;
        while (SerialFlash.ready() == false) {
            if (millis() - dotMillis > 1000) {
                dotMillis = dotMillis + 1000;
                Serial.print(".");
                dotcount = dotcount + 1;
                if (dotcount >= 60) {
                    Serial.println();
                    dotcount = 0;
                }
            }
        }
        if (dotcount > 0) Serial.println();
        Serial.println(F("Erase completed"));
        unsigned long elapsed = millis() - startMillis;
        Serial.print(F("  actual wait: "));
        Serial.print(elapsed / 1000ul);
        Serial.println(F(" seconds."));
    }
}
void rawHardwareTestFlash()
{
    Serial.println(F("Raw SerialFlash Hardware Test"));
    SerialFlash.begin(FLASH_PIN); // proceed even if begin() fails

    if (test()) {
        Serial.println();
        Serial.println(F("All Tests Passed  :-)"));
        Serial.println();
        Serial.println(F("Test data was written to your chip.  You must run"));
        Serial.println(F("EraseEverything before using this chip for files."));
    } else {
        Serial.println();
        Serial.println(F("Tests Failed  :{"));
        Serial.println();
        Serial.println(F("The flash chip may be left in an improper state."));
        Serial.println(F("You might need to power cycle to return to normal."));
    }
}

void spaces(int num) {
    for (int i=0; i < num; i++) {
        Serial.print(' ');
    }
}

bool compareFiles(File &file, SerialFlashFile &ffile) {
    file.seek(0);
    ffile.seek(0);
    unsigned long count = file.size();
    while (count > 0) {
        char buf1[128], buf2[128];
        unsigned long n = count;
        if (n > 128) n = 128;
        file.read(buf1, n);
        ffile.read(buf2, n);
        if (memcmp(buf1, buf2, n) != 0) return false; // differ
        count = count - n;
    }
    return true;  // all data identical
}
void error(const char *message) {
    while (1) {
        Serial.println(message);
        delay(2500);
    }
}
float eraseBytesPerSecond(const unsigned char *id) {
    if (id[0] == 0x20) return 152000.0; // Micron
    if (id[0] == 0x01) return 500000.0; // Spansion
    if (id[0] == 0xEF) return 419430.0; // Winbond
    if (id[0] == 0xC2) return 279620.0; // Macronix
    return 320000.0; // guess?
}

bool test() {
    unsigned char buf[256], sig[256], buf2[8];
    unsigned long address, count, chipsize, blocksize;
    unsigned long usec;
    bool first;

    // Read the chip identification
    Serial.println();
    Serial.println(F("Read Chip Identification:"));
    SerialFlash.readID(buf);
    Serial.print(F("  JEDEC ID:     "));
    Serial.print(buf[0], HEX);
    Serial.print(' ');
    Serial.print(buf[1], HEX);
    Serial.print(' ');
    Serial.println(buf[2], HEX);
    Serial.print(F("  Part Number: "));
    Serial.println(id2chip(buf));
    Serial.print(F("  Memory Size:  "));
    chipsize = SerialFlash.capacity(buf);
    Serial.print(chipsize);
    Serial.println(F(" bytes"));
    if (chipsize == 0) return false;
    Serial.print(F("  Block Size:   "));
    blocksize = SerialFlash.blockSize();
    Serial.print(blocksize);
    Serial.println(F(" bytes"));


    // Read the entire chip.  Every test location must be
    // erased, or have a previously tested signature
    Serial.println();
    Serial.println(F("Reading Chip..."));
    memset(buf, 0, sizeof(buf));
    memset(sig, 0, sizeof(sig));
    memset(buf2, 0, sizeof(buf2));
    address = 0;
    count = 0;
    first = true;
    while (address < chipsize) {
        SerialFlash.read(address, buf, 8);
        //Serial.print("  addr = ");
        //Serial.print(address, HEX);
        //Serial.print(", data = ");
        //printbuf(buf, 8);
        create_signature(address, sig);
        if (is_erased(buf, 8) == false) {
            if (equal_signatures(buf, sig) == false) {
                Serial.print(F("  Previous data found at address "));
                Serial.println(address);
                Serial.println(F("  You must fully erase the chip before this test"));
                Serial.print(F("  found this: "));
                printbuf(buf, 8);
                Serial.print(F("     correct: "));
                printbuf(sig, 8);
                return false;
            }
        } else {
            count = count + 1; // number of blank signatures
        }
        if (first) {
            address = address + (testIncrement - 8);
            first = false;
        } else {
            address = address + 8;
            first = true;
        }
    }
    // Write any signatures that were blank on the original check
    if (count > 0) {
        Serial.println();
        Serial.print(F("Writing "));
        Serial.print(count);
        Serial.println(F(" signatures"));
        memset(buf, 0, sizeof(buf));
        memset(sig, 0, sizeof(sig));
        memset(buf2, 0, sizeof(buf2));
        address = 0;
        first = true;
        while (address < chipsize) {
            SerialFlash.read(address, buf, 8);
            if (is_erased(buf, 8)) {
                create_signature(address, sig);
                //Serial.printf("write %08X: data: ", address);
                //printbuf(sig, 8);
                SerialFlash.write(address, sig, 8);
                while (!SerialFlash.ready()) ; // wait
                SerialFlash.read(address, buf, 8);
                if (equal_signatures(buf, sig) == false) {
                    Serial.print(F("  error writing signature at "));
                    Serial.println(address);
                    Serial.print(F("  Read this: "));
                    printbuf(buf, 8);
                    Serial.print(F("  Expected:  "));
                    printbuf(sig, 8);
                    return false;
                }
            }
            if (first) {
                address = address + (testIncrement - 8);
                first = false;
            } else {
                address = address + 8;
                first = true;
            }
        }
    } else {
        Serial.println(F("  all signatures present from prior tests"));
    }


    // Read all the signatures again, just to be sure
    // checks prior writing didn't corrupt any other data
    Serial.println();
    Serial.println(F("Double Checking All Signatures:"));
    memset(buf, 0, sizeof(buf));
    memset(sig, 0, sizeof(sig));
    memset(buf2, 0, sizeof(buf2));
    count = 0;
    address = 0;
    first = true;
    while (address < chipsize) {
        SerialFlash.read(address, buf, 8);
        create_signature(address, sig);
        if (equal_signatures(buf, sig) == false) {
            Serial.print(F("  error in signature at "));
            Serial.println(address);
            Serial.print(F("  Read this: "));
            printbuf(buf, 8);
            Serial.print(F("  Expected:  "));
            printbuf(sig, 8);
            return false;
        }
        count = count + 1;
        if (first) {
            address = address + (testIncrement - 8);
            first = false;
        } else {
            address = address + 8;
            first = true;
        }
    }
    Serial.print(F("  all "));
    Serial.print(count);
    Serial.println(F(" signatures read ok"));


    // Read pairs of adjacent signatures
    // check read works across boundaries
    Serial.println();
    Serial.println(F("Checking Signature Pairs"));
    memset(buf, 0, sizeof(buf));
    memset(sig, 0, sizeof(sig));
    memset(buf2, 0, sizeof(buf2));
    count = 0;
    address = testIncrement - 8;
    first = true;
    while (address < chipsize - 8) {
        SerialFlash.read(address, buf, 16);
        create_signature(address, sig);
        create_signature(address + 8, sig + 8);
        if (memcmp(buf, sig, 16) != 0) {
            Serial.print(F("  error in signature pair at "));
            Serial.println(address);
            Serial.print(F("  Read this: "));
            printbuf(buf, 16);
            Serial.print(F("  Expected:  "));
            printbuf(sig, 16);
            return false;
        }
        count = count + 1;
        address = address + testIncrement;
    }
    Serial.print(F("  all "));
    Serial.print(count);
    Serial.println(F(" signature pairs read ok"));


    // Write data and read while write in progress
    Serial.println();
    Serial.println(F("Checking Read-While-Write (Program Suspend)"));
    address = 256;
    while (address < chipsize) { // find a blank space
        SerialFlash.read(address, buf, 256);
        if (is_erased(buf, 256)) break;
        address = address + 256;
    }
    if (address >= chipsize) {
        Serial.println(F("  error, unable to find any blank space!"));
        return false;
    }
    for (int i=0; i < 256; i += 8) {
        create_signature(address + i, sig + i);
    }
    Serial.print(F("  write 256 bytes at "));
    Serial.println(address);
    Serial.flush();
    SerialFlash.write(address, sig, 256);
    usec = micros();
    if (SerialFlash.ready()) {
        Serial.println(F("  error, chip did not become busy after write"));
        return false;
    }
    SerialFlash.read(0, buf2, 8); // read while busy writing
    while (!SerialFlash.ready()) ; // wait
    usec = micros() - usec;
    Serial.print(F("  write time was "));
    Serial.print(usec);
    Serial.println(F(" microseconds."));
    SerialFlash.read(address, buf, 256);
    if (memcmp(buf, sig, 256) != 0) {
        Serial.println(F("  error writing to flash"));
        Serial.print(F("  Read this: "));
        printbuf(buf, 256);
        Serial.print(F("  Expected:  "));
        printbuf(sig, 256);
        return false;
    }
    create_signature(0, sig);
    if (memcmp(buf2, sig, 8) != 0) {
        Serial.println(F("  error, incorrect read while writing"));
        Serial.print(F("  Read this: "));
        printbuf(buf2, 256);
        Serial.print(F("  Expected:  "));
        printbuf(sig, 256);
        return false;
    }
    Serial.print(F("  read-while-writing: "));
    printbuf(buf2, 8);
    Serial.println(F("  test passed, good read while writing"));



    // Erase a block and read while erase in progress
    if (chipsize >= 262144 + blocksize + testIncrement) {
        Serial.println();
        Serial.println(F("Checking Read-While-Erase (Erase Suspend)"));
        memset(buf, 0, sizeof(buf));
        memset(sig, 0, sizeof(sig));
        memset(buf2, 0, sizeof(buf2));
        SerialFlash.eraseBlock(262144);
        usec = micros();
        delayMicroseconds(50);
        if (SerialFlash.ready()) {
            Serial.println(F("  error, chip did not become busy after erase"));
            return false;
        }
        SerialFlash.read(0, buf2, 8); // read while busy writing
        while (!SerialFlash.ready()) ; // wait
        usec = micros() - usec;
        Serial.print(F("  erase time was "));
        Serial.print(usec);
        Serial.println(F(" microseconds."));
        // read all signatures, check ones in this block got
        // erased, and all the others are still intact
        address = 0;
        first = true;
        while (address < chipsize) {
            SerialFlash.read(address, buf, 8);
            if (address >= 262144 && address < 262144 + blocksize) {
                if (is_erased(buf, 8) == false) {
                    Serial.print(F("  error in erasing at "));
                    Serial.println(address);
                    Serial.print(F("  Read this: "));
                    printbuf(buf, 8);
                    return false;
                }
            } else {
                create_signature(address, sig);
                if (equal_signatures(buf, sig) == false) {
                    Serial.print(F("  error in signature at "));
                    Serial.println(address);
                    Serial.print(F("  Read this: "));
                    printbuf(buf, 8);
                    Serial.print(F("  Expected:  "));
                    printbuf(sig, 8);
                    return false;
                }
            }
            if (first) {
                address = address + (testIncrement - 8);
                first = false;
            } else {
                address = address + 8;
                first = true;
            }
        }
        Serial.print(F("  erase correctly erased "));
        Serial.print(blocksize);
        Serial.println(F(" bytes"));
        // now check if the data we read during erase is good
        create_signature(0, sig);
        if (memcmp(buf2, sig, 8) != 0) {
            Serial.println(F("  error, incorrect read while erasing"));
            Serial.print(F("  Read this: "));
            printbuf(buf2, 256);
            Serial.print(F("  Expected:  "));
            printbuf(sig, 256);
            return false;
        }
        Serial.print(F("  read-while-erasing: "));
        printbuf(buf2, 8);
        Serial.println(F("  test passed, good read while erasing"));

    } else {
        Serial.println(F("Skip Read-While-Erase, this chip is too small"));
    }

    return true;
}
const char * id2chip(const unsigned char *id)
{
    if (id[0] == 0xEF) {
        // Winbond
        if (id[1] == 0x40) {
            if (id[2] == 0x14) return "W25Q80BV";
            if (id[2] == 0x15) return "W25Q16DV";
            if (id[2] == 0x17) return "W25Q64FV";
            if (id[2] == 0x18) return "W25Q128FV";
            if (id[2] == 0x19) return "W25Q256FV";
        }
    }
    if (id[0] == 0x01) {
        // Spansion
        if (id[1] == 0x02) {
            if (id[2] == 0x16) return "S25FL064A";
            if (id[2] == 0x19) return "S25FL256S";
            if (id[2] == 0x20) return "S25FL512S";
        }
        if (id[1] == 0x20) {
            if (id[2] == 0x18) return "S25FL127S";
        }
    }
    if (id[0] == 0xC2) {
        // Macronix
        if (id[1] == 0x20) {
            if (id[2] == 0x18) return "MX25L12805D";
        }
    }
    if (id[0] == 0x20) {
        // Micron
        if (id[1] == 0xBA) {
            if (id[2] == 0x20) return "N25Q512A";
            if (id[2] == 0x21) return "N25Q00AA";
        }
        if (id[1] == 0xBB) {
            if (id[2] == 0x22) return "MT25QL02GC";
        }
    }
    if (id[0] == 0xBF) {
        // SST
        if (id[1] == 0x25) {
            if (id[2] == 0x02) return "SST25WF010";
            if (id[2] == 0x03) return "SST25WF020";
            if (id[2] == 0x04) return "SST25WF040";
            if (id[2] == 0x41) return "SST25VF016B";
            if (id[2] == 0x4A) return "SST25VF032";
        }
        if (id[1] == 0x25) {
            if (id[2] == 0x01) return "SST26VF016";
            if (id[2] == 0x02) return "SST26VF032";
            if (id[2] == 0x43) return "SST26VF064";
        }
    }
    if (id[0] == 0x1F) {
        // Adesto
        if (id[1] == 0x89) {
            if (id[2] == 0x01) return "AT25SF128A";
        }
    }
    return "(unknown chip)";
}

void print_signature(const unsigned char *data)
{
    Serial.print(F("data="));
    for (unsigned char i=0; i < 8; i++) {
        Serial.print(data[i]);
        Serial.print(' ');
    }
    Serial.println();
}

void create_signature(unsigned long address, unsigned char *data)
{
    data[0] = address >> 24;
    data[1] = address >> 16;
    data[2] = address >> 8;
    data[3] = address;
    unsigned long hash = 2166136261ul;
    for (unsigned char i=0; i < 4; i++) {
        hash ^= data[i];
        hash *= 16777619ul;
    }
    data[4] = hash;
    data[5] = hash >> 8;
    data[6] = hash >> 16;
    data[7] = hash >> 24;
}

bool equal_signatures(const unsigned char *data1, const unsigned char *data2)
{
    for (unsigned char i=0; i < 8; i++) {
        if (data1[i] != data2[i]) return false;
    }
    return true;
}

bool is_erased(const unsigned char *data, unsigned int len)
{
    while (len > 0) {
        if (*data++ != 255) return false;
        len = len - 1;
    }
    return true;
}


void printbuf(const void *buf, uint32_t len)
{
    const uint8_t *p = (const uint8_t *)buf;
    do {
        unsigned char b = *p++;
        Serial.print(b >> 4, HEX);
        Serial.print(b & 15, HEX);
        //Serial.printf("%02X", *p++);
        Serial.print(' ');
    } while (--len > 0);
    Serial.println();
}