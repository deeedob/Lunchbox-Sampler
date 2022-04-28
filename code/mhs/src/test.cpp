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

void testDisplay();
void testMicrophone();
void playFile(const char *filename);

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
    Serial.begin(9600);
    Wire.begin();
    SD.begin(SDCARD_CS_PIN);
    pinMode(ROTARY_SW, INPUT_PULLUP);

    testDisplay();
    //testMicrophone();

    Serial.println("Testing the SD-Card");
    SPI.setMOSI(SDCARD_MOSI_PIN);
    SPI.setSCK(SDCARD_SCK_PIN);
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

    long newPos = enc.read();
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
    audioShield.volume(volume/1024.f);

}

void testMicrophone() {
    Serial.println("Testing the AOM-6738P-R mic");
    Serial.println("use your headphones!");
    AudioMemory(8);
    audioShield.enable();
    audioShield.inputSelect(AUDIO_INPUT_MIC);  // select mic or line-in for audio shield input source
    audioShield.volume(0.8);
    float pot2 = (float) analogRead(POT1) / 1024.f;
    audioShield.micGain((int)(                   pot2*45));
    while(btn_rotary.risingEdge()){}
    audioShield.disable();
}

void testDisplay() {

    Serial.println("Testing the SSD1327 OLED in I2C mode");
    if ( ! display.begin(0x3D) ) {
        Serial.println("Unable to initialize OLED");
        exit(1);
        //while (1) yield();
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

