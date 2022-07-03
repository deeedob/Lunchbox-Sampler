#pragma once

/* Arduino ... why u do this shit?! */
#undef main

/* General Pins */
#define DISPLAY_CLK_ 16
#define DISPLAY_SDA_ 17
#define DISPLAY_RST_ 9

#define SDCARD_CS_PIN_   10
#define SDCARD_MOSI_PIN_ 11
#define SDCARD_SCK_PIN_  13

/* Digital Pin */
#define ROTARY_A_ 2
#define ROTARY_B_ 3
#define BTN_ENTER_ 4
#define BTN_RETURN_ 5
#define BTN_TOGGLE_ 26

#define MIDI_IN_ 0
#define MIDI_OUT_ 1


/* every pin has at least one of these properties
 * thanks @https://github.com/KurtE/TeensyDocuments/blob/master/Teensy4%20MicroMod%20Pins.pdf
 * Note that the AudioInputAnalog class uses adc0!
 */

/* Analog Pins */
#define POT0_ A0        //both
#define POT1_ A1        //both
#define POT2_ A10       //adc0
#define POT3_ A11       //adc0

/* FSR Pins */
#define FSR_POLL_ A8    //both
#define FSR_SEL0_ 28
#define FSR_SEL1_ 30
#define FSR_SEL2_ 32

/* MPX Positions */
#define FSR_MPX0_ 0
#define FSR_MPX1_ 1
#define FSR_MPX2_ 2
#define FSR_MPX3_ 3

