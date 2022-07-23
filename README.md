# Lunchbox Sampler
This device plays back audio files, records them, loads samples packs, and visualizes everything on a screen while trying to be **blazingly fast**.
We tried to incorporate as many features as we could from classic samplers like the MPC, Korg Volca, and many more.
The Lunchbox Sampler lives up to its name and puts an emphasis on ***mobility*** and ***extensibility***. 
There's everything you need to rebuild this device yourself - and maybe even extend it. *Give it a shot!*

Features :
1. Midi USB Host
2. Midi Jack IN & OUT
3. Midi with 4 custom build Force Sensors ((FSR))[https://en.wikipedia.org/wiki/Force-sensing_resistor]
4. 128x128 Display
    - Horizontal, Vertical, Fullscreen
5. Condenser Microphone
6. Stereo TS 6.35mm IN & OUT
7. Stereo TRS 3.5mm OUT
8. SD Card for loading Settings & Soundpacks
9. Interrupt driven Event System
    - Multithreaded
    - Digital Interrupts
    - Analog Interrupts on change

Overview :
1. [Pin Layout](#pin-layout)
2. [Building](#building)
3. [Contribution Policy](#contribution-policy)
4. [Coding Standard](#coding-standard)

## Pin Layout
The following pins are used :

| **Teensy Pins** |    **Mode**     | **descriptions**         | 
|:---------------:|:---------------:|--------------------------|
|        0        |       RX1       | Midi IN,  Serial         |
|        1        |       TX1       | Midi OUT, Serial         |
|        2        |      Dig 2      | Rotary A                 |
|        3        |      Dig 3      | Rotary B                 | 
|        4        |      Dig 4      | Button Enter             |
|        5        |      Dig 5      | Button Return            |
|       *6*       |   *RESERVED*    | *Audio-Shield, MEMCS*    |
|       *7*       |   *RESERVED*    | *Audio-Shield, DIN*      |
|       *8*       |   *RESERVED*    | *Audio-Shield, DOUT*     |
|        9        |      Dig 9      | Display Reset            |
|      *10*       |   *RESERVED*    | *Audio-Shield, SD::CS*   |
|      *11*       |   *RESERVED*    | *Audio-Shield, SD::MOSI* |
|      *12*       |   *RESERVED*    | *Audio-Shield, MISO*     |
|      *13*       |   *RESERVED*    | *Audio-Shield, SD::SCK*  |
|       14        |       A0        | POT0, ADC0               |
|       15        |       A1        | POT1, ADC0               |
|       16        |      SCL1       | Display, I2C Clock       |
|       17        |      SDA1       | Display, I2C Data        |
|      *18*       |   *RESERVED*    | *Audio-Shield, SDA*      |
|      *19*       |   *RESERVED*    | *Audio-Shield, SCL*      |
|      *20*       |   *RESERVED*    | *Audio-Shield, LRCLK*    |
|      *21*       |   *RESERVED*    | *Audio-Shield, BCLK*     |
|       22        |       A8        | FSR Poll, ADC1           |
|      *23*       |   *RESERVED*    | *Audio-Shield, MCLK*     |
|                 | **Bottom Pins** |                          |
|       24        |       A10       | POT2, ADC0               |
|       25        |       A11       | POT3, ADC0               |
|       26        |    **FREE**     |                          |
|       27        |       A8        |                          |
|       28        |     Dig 28      | FSR Select 0             |
|       29        |    **FREE**     |                          |
|       30        |     Dig 30      | FSR Select 1             |
|       31        |    **FREE**     |                          |
|       32        |     Dig 32      | FSR Select 2             |
|       33        |    **FREE**     |                          |

## Building
The following describes the build process for the [Teensy Platform](https://www.pjrc.com/) using platformio.
Please install this program and the teensy-loader on your system and add them to your System Paths variable.

Clone this repo :
```bash
    $ git clone https://github.com/MobileHS/Lunchbox-Sampler.git
    # or via ssh
    $ git clone git@github.com:MobileHS/Lunchbox-Sampler.git 
    # cd into the repo
    $ cd Lunchbox-Sampler
```

Run platformio :
```bash
    $ pio run
```
This will generate a hidden folder called **.pio**. In this directory, all the compiled libraries and sources are linked and the final hex file is produced.

Then simply upload it to the teensy board :
```bash
    $ pio run --target upload
```

## Contribution Policy
Please follow these commandments :

1. Make **small commits**.
2. Explain the why, not the what, in your commit message
3. Create a **new branch** with the following pattern : feature-\<name\>
4. Do not commit or push directly to the master branch

When working on features, it is necessary to work in a subbranch, e.g. :

```bash
    git checkout -b feature-xyz main #create a new feature branch from main
```

The master branch gets merged regularly from the development branch! To update your feature branch 
when working on bigger features use a **rebase technique** to do so!

```bash
    git checkout development
    git pull
    git checkout feature-xyz
    git rebase development   # merge conflicts may happen. Better do this step in an IDE!
```

When a feature is finished - merge it back into development.

```bash
    git checkout development
    git pull
    git merge feature-xyz   # merge conflicts may happen. Better do this step in an IDE!
```

## Coding Standard
- Work with **Modules** inside the lib folder
  - every module sits in namespace **lbs**
- **main** is the active branch! Don't touch it and use **development** instead!
- Don't get lost in your feature branch! Commit regularly into development if working.
