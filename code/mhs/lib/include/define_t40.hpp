#pragma once
#include <Arduino.h>

/* Arduino ... why u do this shit?! */
#undef assert
#undef main

/* General Pins */
#define _DISPLAY_CLK 16
#define _DISPLAY_SDA 17
#define _DISPLAY_RST 9

#define _SDCARD_CS_PIN   10
#define _SDCARD_MOSI_PIN 11
#define _SDCARD_SCK_PIN  13

/* Digital Pin */
#define _ROTARY_A 2
#define _ROTARY_B 3
#define _BTN_ENTER 4
#define _BTN_RETURN 5
#define _BTN_TOGGLE 26

#define _MIDI_IN 0
#define _MIDI_OUT 1


/* every pin has at least one of these properties
 * thanks @https://github.com/KurtE/TeensyDocuments/blob/master/Teensy4%20MicroMod%20Pins.pdf
 * Note that the AudioInputAnalog class uses adc0!
 */

/* Analog Pins */
#define _POT0 A0        //both
#define _POT1 A1        //both
#define _POT2 A10       //adc0
#define _POT3 A11       //adc0

/* FSR Pins */
#define _FSR_POLL A8    //both
#define _FSR_SEL0 28
#define _FSR_SEL1 30
#define _FSR_SEL2 32

/* MPX Positions */
#define _FSR_MPX0 0
#define _FSR_MPX1 1
#define _FSR_MPX2 2
#define _FSR_MPX3 3

/*!
 * Little hack to use asserts on the teensy platform via Serial!
 * */
void assert_3(const char *__file, int __lineno, const char *__sexp );
#define assert( a ) if (!a) { assert_3(__FILE__, __LINE__,  #a); }

// handle diagnostic informations given by assertion and abort program execution:
void assert_3(const char *__file, int __lineno, const char *__sexp) {
    // transmit diagnostic informations through serial link.
    // Serial.println(__func);
    Serial.println(" ___ ASSERT FAILED ___ FILE, LINE#, Expression ");
    Serial.println(__file);
    Serial.println(__lineno, DEC);
    Serial.println(__sexp);
    Serial.flush();
    // abort program execution.
    //xxx abort();
    Serial.println(" ___ ASSERT FAILED ___ STOPPING !!!!");
    while( 1 );
}
