# Dsxboo

> Source: https://problemkaputt.de/gbatek.htm
> Section: Dsxboo

DS Xboo 
| |
| --- |

 The DS Xboo cable allows to upload NDS ROM-Images (max 3.9MBytes) to the console via parallel port connection. Should be the best, simpliest, easiest, and fastest way to test code on real hardware. And, at a relative decent price of 11 cents per diode it should be by far the least expensive way. You'll have to touch classic tools (screwdrivers, knifes, saws, tweezers, and solder) which will probably scare most of you to hell.
 
 **DS XBOO Connection Schematic**
 
```

| Console Pin/Names Parallel Port Pin/Names
 RFU.9 FMW.1 D ---|>|--- DSUB.14 CNTR.14 AutoLF
 RFU.6 FMW.2 C ---|>|--- DSUB.1 CNTR.1 Strobe
 RFU.10 FMW.3 /RES ---|>|--- DSUB.16 CNTR.31 Init
 RFU.7 FMW.4 /S ---|>|--- DSUB.17 CNTR.36 Select
 RFU.5 FMW.5 /W --. SL1A - - N.C.
 RFU.28 FMW.6 VCC __| SL1B - - N.C.
 RFU.2,12 FMW.7 VSS --------- DSUB.18-25 CNTR.19-30 Ground
 RFU.8 FMW.8 Q --------- DSUB.11 CNTR.11 Busy
 P00 Joypad-A ---|>|--- DSUB.2 CNTR.2 D0
 P01 Joypad-B ---|>|--- DSUB.3 CNTR.3 D1
 P02 Joypad-Select ---|>|--- DSUB.4 CNTR.4 D2
 P03 Joypad-Start ---|>|--- DSUB.5 CNTR.5 D3
 P04 Joypad-Right ---|>|--- DSUB.6 CNTR.6 D4
 P05 Joypad-Left ---|>|--- DSUB.7 CNTR.7 D5
 P06 Joypad-Up ---|>|--- DSUB.8 CNTR.8 D6
 P07 Joypad-Down ---|>|--- DSUB.9 CNTR.9 D7
 RTC.1 INT aka SI --------- DSUB.10 CNTR.10 /Ack |
| --- |

Parts List: 15 wires, four (DS) or twelve (DS-Lite) "BAT 85" diodes, 1 parallel
port socket.

**DS XBOO Connection Notes**

The Firmware chip (FMW.Pins) hides underneath of the RFU shielding plate, so
it'd be easier to connect the wires to the RFU.Pins (except DS-Lite: The RFU
pins are terribly small (and have different pin-numbers), so either using
FMW.Pins, or using mainboard vias (see below GIF) would be easier). The easiest
way for the /W-to-VCC connection is to shortcut SL1 by putting some solder onto
it.

The P00..P07 and INT signals are labeled on the switch-side of the mainboard,
however, there should be more room for the cables when connecting them to via's
at the bottom-side (except DS-Lite: P01 is found only at switch-side) image
below may help to locate that pins,

```
[http://problemkaputt.de/nds-pins.gif](http://problemkaputt.de/nds-pins.gif)
| (GIF-Image, 7.5KBytes) |
| --- |

At the parallel port side, DSUB.Pins or CNTR.Pins can be used for 25pin DSUB or
36pin Centronics sockets, the latter one allowing to use a standard printer
cable.

The ring printed on the diodes is pointing towards parallel port side, the 4
diodes are required to prevent the parallel port to pull-up LOW levels on the
NDS side, be sure to use BAT85 diodes, cheaper ones like 1N4148 are loosing too
much voltage and won't gain stable LOW levels.

The power managment chip in the DS-Lite simply refuses to react to the Power-On
button when P00..P07 are dragged high by the parallel port (even if it is in
HighZ state), the 8 diodes in the data-lines are solving that problem (they are
required on DS-Lite only, not on original DS).

**DS XBOO Operation Notes**

The main Upload function is found in no$gba Utility menu, together with further
functions in Remote Access sub-menu.

Before uploading anything: download the original firmware, the file is saved as
FIRMnnnn.BIN, whereas "nnnn" is equal to the last 16bit of the consoles 48bit
MAC address, so Firmware-images from different consoles are having unique
filenames. If you don't already have, also download the NDS BIOS, the BIOS
contains encryption seed data required to encrypt/decrypt secure area; without
having downloaded the BIOS, no$gba will be working only with unencrypted
ROM-images. Next, select Patch Firmware to install the nocash firmware.

**DS XBOO Troubleshooting**

Be sure that the console is switched on, and that the XBOO cable is connected,
and that you have selected the correct parallel port in no$gba setup (the
"multiboot" options in Various Options screen), and, of course, try avoid to be
fiddling with the joypad during uploads.

I've tested the cable on two computers, the overall upload/download stuff
should work stable. The firmware access functions - which are required only for
(un-)installation - worked only with one of the two computers; try using a
different computer/parallel port in case of problems.

**Nocash Firmware**

The primary purpose is to receive uploaded NDS-images via parallel port
connection, additionally it's containing bootmenu and setup screens similar to
the original firmware. The user interface is having less cryptic symbols and
should be alltogether faster and easier to use. Important Information about
Whatever is supported (but it can be disabled). The setup contains a couple of
additional options like automatic daylight saving time adjustment.

The bootmenu allows to boot normal NDS and GBA carts, it does additionally
allow to boot NDS-images (or older PassMe-images) from flashcards in GBA slot.
Furthermore, benefits of asm coding, the nocash firmware occupies less than
32KBytes, allowing to store (and boot) smaller NDS-images in the unused portion
of the firmware memory (about 224KBytes), the zero-filled region between cart
header and secure area, at 200h..3FFFh, is automatically excluded, so the image
may be slightly bigger than the available free memory space.

**Missing**

Unlike the original firmware, the current version cannot yet boot via WLAN.

| DSi Reference |
| --- |

**Basic Hardware Features (mostly same as NDS)**

NDS Reference

DSi Basic Differences to NDS

**New Hardware Features**

DSi I/O Map

DSi Control Registers (SCFG)

DSi XpertTeak (DSP)

DSi New Shared WRAM (for ARM7, ARM9, DSP)

DSi New DMA (NDMA)

DSi Microphone and SoundExt

DSi Advanced Encryption Standard (AES)

DSi Cartridge Header

DSi Touchscreen/Sound Controller

DSi I2C Bus

DSi Cameras

DSi SD/MMC Protocol and I/O Ports

DSi SD/MMC Filesystem

DSi Atheros Wifi SDIO Interface

DSi Atheros Wifi Internal Hardware

DSi GPIO Registers

DSi Console IDs

DSi Unknown Registers

DSi Notes

DSi Exploits

DSi Regions

**General Info**

ARM CPU Reference

BIOS Functions

External Connectors

Credits: [http://www.dsibrew.org/wiki/Special:AllPages](http://www.dsibrew.org/wiki/Special:AllPages)
 (now spammed)