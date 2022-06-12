# MHS
This is a project actively developed as part of the Pam 3 Course of the TH Koeln. <br>
We plan to develop and build a **mobile hardware Sampler** with the [Teensy](https://www.pjrc.com/store/teensy40.html) Microcontroller.

## Prerequisite
Ensure that the hardware is functioning. You can do this by running the [hardware test](https://github.com/MobileHS/MHS/tree/hw-test) and checking your device.

Connect all the inputs , outputs and display as followed :
| **Teensy Pins**  | **Mode**  | **description**  |
|---|---|---|
| 0  | RX1  | Midi IN, serial  |
| 1 | TX1  | Midi OUT, serial  |
|   |   |   |
| 2 | Dig 2  | Rotary, Out A  |
| 3 | Dig 3  | Rotary, Out B  |
| 4 | Dig 4  | Rotary, Switch  |
| 5 | Dig 5  | Button, Return |
| 22 | Dig 22 | Button, Play/Pause
|   |   |   |
| 16  | SCL1  | Display, I2C Clock  |
| 17  | SDA1  | Display, I2C Data  |
| 9  | Dig 9  | Display, Reset  |
|   |   |   |
| 14  | A0  | Poti 0  |
| 15  | A1  | Poti 1  |
| 24  | A10  | Poti 2  |
| 26  | A12  | Poti 3  |

## Building
The following describes the build process for the [i.MX RT1060 MCU](https://github.com/MobileHS/MHS/blob/main/docs/teensy40/IMXRT_RT1060-Block_diagram.png) using platformio. Please install this program and the teensy-loader on your system and add them to your System Paths variable.

Clone this repo :
```bash
    # git clone https://github.com/MobileHS/MHS.git 
```

Navigate to the sources : 
```bash
    # cd MHS/code/mhs
```

Run platformio : 
```bash
    # pio run
```
This will generate a hidden folder called **.pio**. In this directory, all the compiled libraries and sources are linked and the final hex file is produced.

Then simply upload it to the teensy board : 
```bash
    # pio run --target upload
```

## Development

**Pull the repo before working!!**
```bash
    # git pull
```

```bash
    # git add <files> or --all
    # git commit -m <msg> 
    # git push origin main
```
## Dependency
This Project will be developed with the [PlatformIO](https://platformio.org/) Teensy4.0-Toolchain.
It is recommended to use the [Clion](https://www.jetbrains.com/clion/) IDE with the PlatformIO Plugin.
A working [Serial monitor](https://plugins.jetbrains.com/plugin/8031-serial-port-monitor) and the library dependencies specified in the [platformio.ini](https://github.com/MobileHS/MHS/blob/main/code/mhs/platformio.ini) file are required.
