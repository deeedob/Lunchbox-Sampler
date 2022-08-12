#pragma once


/* General Pins */
#define C_DISPLAY_CLK 16
#define C_DISPLAY_SDA 17
#define C_DISPLAY_RST 9

#define C_SDCARD_CS_PIN   10
#define C_SDCARD_MOSI_PIN 11
#define C_SDCARD_SCK_PIN  13

/* Digital Pin */
#define C_ROTARY_A 2
#define C_ROTARY_B 3
#define C_BTN_ENTER 4
#define C_BTN_RETURN 5
#define C_BTN_TOGGLE 26

#define C_MIDI_IN 0
#define C_MIDI_OUT 1

/* every pin has at least one of these properties
 * thanks @https://github.com/KurtE/TeensyDocuments/blob/master/Teensy4%20MicroMod%20Pins.pdf
 * Note that the AudioInputAnalog class uses adc0!
 */

/* Analog Pins */
#define C_POT_0 A0        //both
#define C_POT_1 A1        //both
#define C_POT_2 A10       //adc0
#define C_POT_3 A11       //adc0

/* FSR Pins */
#define C_FSR_POLL A8    //both
#define C_FSR_SEL_0 28
#define C_FSR_SEL_1 30
#define C_FSR_SEL_2 32

/* MPX Positions */
#define C_FSR_MPX_0 0
#define C_FSR_MPX_1 1
#define C_FSR_MPX_2 2
#define C_FSR_MPX_3 3