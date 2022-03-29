# [LittleFS Library (for Teensy)](https://github.com/PaulStoffregen/LittleFS)
 
## Supported Chips

### NOR Flash

####
MFG | PART # | Size
------------ | ------------- |------------ 
Winbond | W25Q16JV*IQ/W25Q16FV | 16Mb
... | W25Q32JV*IQ/W25Q32FV | 32Mb
... | W25Q64JV*IQ/W25Q64FV | 64Mb
... | W25Q128JV*IQ/W25Q128FV | 128Mb
... | W25Q256JV*IQ | 256Mb
... | Winbond W25Q512JV*IQ | 512Mb
... | W25Q64JV*IM (DTR) | 64Mb
... | W25Q128JV*IM (DTR) | 128Mb
... | W25Q256JV*IM (DTR) | 256Mb
... | W25Q512JV*IM (DTR) | 512Mb
Adesto/Atmel | AT25SF041 | 4Mb
Spansion | S25FL208K | 8Mb

### NAND Flash

####
MFG | PART # | Size
------------ | ------------- |------------ 
Winbond  | W25N01G | 1Gb
... | W25N02G | 2Gb
... | W25M02 | 2Gb


### FRAM

####
MFG | PART # | Size
------------ | ------------- |------------ 
Cypress | CY15B108QN-40SXI | 8Mb
... | FM25V10-G | 1Mb
... | FM25V10-G rev1 | 1Mb
... | CY15B104Q-SXI | 4Mb
... | CY15B102Q-SXI | 2Mb
ROHM | MR45V100A | 1Mb
Fujitsu | MB85RS2MTAPNF | 2Mb
)

# [SerialFlash library](https://github.com/PaulStoffregen/SerialFlash)

## Hardware Compatibility

These chips have been tested with SerialFlash:

    Winbond W25Q80BV    (http://www.adafruit.com/product/1564)
    Winbond W25Q64FV
    Winbond W25Q128FV
    Winbond W25Q256FV
    Micron N25Q512A
    Micron N25Q00AA
    Spansion S25FL127S
    Spansion S25FL256S
    Spansion S25FL512S

SerialFlash automatically detects SPI Flash chip type and capacity to automatically handle differences between supported chips.