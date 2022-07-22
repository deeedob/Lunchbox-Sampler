# MHS Hardware Test

Please test the hardware setup if it's working.

## How To
- Clone this repo or switch to hw-test branch :

```bash
    git clone https://github.com/MobileHS/MHS.git 
# or
    git checkout hw-test
```

- then re-init platformio and make sure the [Adafruit SSD1327](https://github.com/adafruit/Adafruit_SSD1327) lib is 
linking and building correctly!

- make sure you have a working microSD Card formatted as fat32
and have the [audio files](https://github.com/MobileHS/MHS/tree/hw-test/code/mhs/res) loaded!

- connect all the inputs , outputs and display as followed :

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

If you need help wiring the Rotary please refer to [this](https://github.com/MobileHS/MHS/blob/main/docs/parts/rotary%20wiring.png). The Rotary button is in **PULLUP** mode.
