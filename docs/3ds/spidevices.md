# Spidevices

> Source: https://problemkaputt.de/gbatek.htm
> Section: Spidevices

3DS SPI Devices 
| |
| --- |

 **SPI Device List**
 
```

| Card, Device 0 Cartridge SPI (eg. FLASH savedata) ;-similar as NDS/DSi
 Bus0, Device 0 Power Managment Device ;\
 Bus0, Device 1 Wifi FLASH ; same as NDS/DSi
 Bus0, Device 2 DSi Touchscreen/Sound/Microphone ;/
 Bus1, Device 0 3DS Touchscreen/Sound/Microphone ;-extra 3ds registers
 Bus1, Device 1,2 Unused
 Bus2, Device 0 Unused
 Bus2, Device 1,2 Don't exist (there are no device-select bits for Bus2)
 Card, Device 1,2 Don't exist (there are no device-select bits for Card) |
| --- |

**SPI Bus/Device 0:0 - Power Managment Device**

The old power managment SPI registers are mostly for NDS mode:

DS Power Management Device

On 3DS, backlight and power-down are controlled via MCU.

**SPI Bus/Device 0:1 - Wifi FLASH**

This chip did contain the firmware in NDS consoles. On 3DS it does merely
contain some wifi/user settings, mostly for NDS titles.

DS Firmware Serial Flash Memory

DS Firmware Header

DS Firmware Wifi Calibration Data

DS Firmware Wifi Internet Access Points

DS Firmware User Settings

The 3DS does additionally store wifi/user settings in a eMMC file:

3DS Files - Config Savegame

**SPI Bus/Device 0:2 - DSi Touchscreen/Sound (DSi mode)**

This SPI bus is used for accessing the TSC chip in DSi mode.

DSi Touchscreen/Sound Controller

Touchscreen calibration points for NDS/DSi titles are in Wifi FLASH (matched to
the current NDS/DSi screen resolution, which can be 256x192pix or fullscreen;
selected by holding Start+Select when starting NDS/DSi title).

**SPI Bus/Device 1:0 - 3DS Touchscreen/Sound (3DS mode)**

This is another SPI bus wired to the TSC chip, this bus can be accessed in 3DS
mode only, giving access to the normal DSi registers, plus some additional 3DS
registers in TSC page 64h,65h,67h,FBh. These new registers are required for
activating sound output, and for reading the Circle Pad.

3DS TSC, Register Summary

Touchscreen calibration points for 3DS titles are stored in HWCAL0/1.dat files,
that files do also include calibration data for Circle Pad, and various initial
TSC register settings.

**Cartridge SPI**

Cartridge SPI can be wired to FLASH savedata (or theoretically anything else).

This can use 4bit SPI mode (if the chip is 4bit compatible, and if 4bit related
signals are wired up accordingly, and if the specific commands support 4bit
transfers).