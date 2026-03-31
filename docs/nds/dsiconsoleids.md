# Dsiconsoleids

> Source: https://problemkaputt.de/gbatek.htm
> Section: Dsiconsoleids

DSi Console IDs 
| |
| --- |

 The DSi contains several unique per-console numbers:
 
```

| CPU/Console ID - Found in Port 4004D00h, in AES Keys, and in Files
 eMMC CID Register - Found in Main RAM, and in eMMC CID register
 Serial/Barcode - Found in Main RAM, and on stickers, and in HWINFO_S.dat
 Wifi MAC Address - Found in Main RAM, and in Wifi FLASH
 Nintendo WFC ID - Found in Wifi FLASH |
| --- |

**4004D00h - DSi7 - CPU/Console ID Code (64bit) (R)**

```

| 0-63 CPU/Console ID Code |
| --- |

This appears to be a PROM inside of the CPU TWL chip. The value is used to
initialize KEY_X values for eMMC encryption/decryption. Common 64bit settings
are:

```

| 08201nnnnnnnn1nnh for DSi (EUR) and DSi XL (USA)
 08202nnnnnnnn1nnh for DSi XL
 08203nnnnnnnn1nnh for DSi XL
 08204nnnnnnnn1nnh for DSi
 08A15???????????h for DSi
 08A18nnnnnnnn1nnh for DSi (USA) (black)
 08A19nnnnnnnn1nnh for DSi (USA)
 08A20nnnnnnnn1nnh for DSi
 08A21nnnnnnnn1nnh for DSi
 08A22???????????h for DSi (USA)
 08A23???????????h for DSi (EUR)
 08C267B7xxxxxxxxh for 3DS
 6B27D20002000000h for n3DS (unknown how that has happened) |
| --- |

The "n" digits appear to be always in BCD range (0..9), the other digits appear
to be fixed (on known consoles).

The 64bit value is also stored as 16-byte ASCII string in "dev.kp". And, the
ASCII string is also stored in footer of "Tad Files on SD Cards".

Port 4004D00h should be read only if the flag in 4004D08h is zero. Moreover,
Port 4004D00h can be read only by firmware, and get's disabled for all known
games, so most exploits will only see zeroes in 4004D00h..4004D08h.

Easiest way to obtain the 64bit value would be extracting it from SD Card
(using modified "DSi SRL Extract" source code).

DSi SD/MMC DSiware Files on External SD Card (.bin aka Tad Files)

Obtaining the 64bit value by DSi software is working only indirectly:

With sudokuhax it can be simply "read" from 40044E0h (LSW) and 40044ECh (MSW).
Whereas, that ports are write-only, so it needs some small efforts to "read"
them.

With DSi cartridge exploits it's a bit more difficult: The values at
40044D4h..40044FBh are destroyed, but 40044D0h..40044D3h is left intact, which
can be used to compute the original MSW value at 40044ECh, using a bunch of
constants and maths operations (caution: the result may depend on carry-in from
unknown LSBs, eg. the MSW may appear as 08A2nnnnh or 08E2nnnnh). Next, one can
simply brute-force the LSW (there should be only 10 million combinations
(assuming it to be a BCD number with one fixed digit), which could be scanned
within less than 6 minutes using the DSi AES hardware).

Note: JimmyZ made some PC tools ("TWLbf" and "bfCL") that can bruteforce the
Console ID or CID (or both) from encrypted eMMC images.

**4004D08h - DSi7 - CPU/Console ID Flag (1bit) (R)**

```

| 0 CPU/Console ID Flag (0=Okay/Ready, 1=Bad/Busy)
 1-15 Unknown/Unused (0) |
| --- |

Some flag that indicates whether one can read the CPU/Console ID from Port
4004D00h. The flag should be usually zero (unknown when it could be nonzero,
maybe shortly after power-up, or maybe when the internal PROM wasn't programmed
yet; which should never happen in retail units).

**eMMC CID Register**

The CID can be read via SD/MMC commands, and it's also stored at 2FFD7BCh in
RAM; the RAM value is little-endian 120bit (ie. without the CRC7 byte),
zeropadded to 16-bytes (with 00h in MSB); the value looks as so;

```

| MY ss ss ss ss 03 4D 30 30 46 50 41 00 00 15 00 ;DSi Samsung KMAPF0000M-S998
 MY ss ss ss ss 32 57 37 31 36 35 4D 00 01 15 00 ;DSi Samsumg KLM5617EFW-B301
 MY ss ss ss ss 30 36 35 32 43 4D 4D 4E 01 FE 00 ;DSi ST NAND02GAH0LZC5 rev30
 MY ss ss ss ss 31 36 35 32 43 4D 4D 4E 01 FE 00 ;DSi ST NAND02GAH0LZC5 rev31
 MY ss ss ss ss 03 47 31 30 43 4D 4D 00 01 11 00 ;3DS whatever chiptype?
 MY ss ss ss ss 07 43 59 31 47 34 4D 00 01 15 00 ;3DS Samsung KLM4G1YE0C-B301 |
| --- |

The value is used to initialize AES_IV register for eMMC encryption/decryption.

The "MY" byte contains month/year; with Y=0Bh..0Dh for 2008..2010 (Y=0Eh..0Fh
would be 2011..2012, but there aren't any known DSi/3DS consoles using that
values) (unknown how 2013 and up would be assigned; JEDEC didn't seem to mind
to define them yet). The "ss" digits are a 32bit serial number (or actually it
looks more like a 32bit random number, not like an incrementing serial value).

Without a working exploit (for reading RAM at 2FFD7BCh), the CID could be
obtained by connecting wires to the eMMC chip. However, this might require
whatever custom hardware/software setup (unknown if any standard tools like PC
card readers are able to read the CID value).

Note: JimmyZ made some PC tools ("TWLbf" and "bfCL") that can bruteforce the
Console ID or CID (or both) using hex values extracted from encrypted eMMC
images.

To speedup CID bruting, one can extract the MY datecode from "Samsung YWW,
KMAPF0000M-S998" text printed on the eMMC chip: the "YWW" is year/week, eg. 8xx
means 2008, and translates to year "B" in the "MY" field.

For the ST chips, there seems to me a similar year/week (reading "KOR 99 YWW",
but the year/week on ST chips is usually about a month older than the
month/year in CID). Known ST date codes are AC/BC (Oct2009/Nov2009) for the
"rev0" ones, and CC/1D (Dec2009/Jan2010) for the "rev1" ones (ie. the ST chips
seem to have been mostly used in early DSi XL models).

**Serial/Barcode**

The barcode is found on a sticker on the bottom of console (and on an identical
sticker underneath of the battery). It's also stored as ASCII string in
HWINFO_S.dat file, and at 2FFFD71h in RAM.

The serial/barcode consists of 2-3 letters, followed by 8 digits, followed by a
checksum digit.

The first letter indicates the console model:

```

| DSi T (or V for devunits)
 DSi XL/LL W (or unknown for devunits)
 3DS C (or E for devunits)
 3DS XL/LL S (or R for devunits)
 2DS A (or P for devunits)
 New 3DS Y (or Yxx00 for devunits)
 New 3DS XL/LL Q (or Qxx00 for devunits)
 New 2DS XL/LL N (or Nxx01 for devunits) |
| --- |

The next 1-2 letter(s) indicate the region:

```

| JPN Japan JF,JH,JM
 USA North America W
 USA Middle East, Southeast Asia S
 EUR Europe EF,EH,EM
 AUS Australia (for 3DS: part of EUR) AG,AH
 CHN China (iQue) CF,CH,CM
 KOR South Korea KF,KH,KM |
| --- |

The checksum can be calculated across 1st-8th digit (and ignoring the leading
letters):

```

| 9th = (250 - (1st+3rd+5th+7th) - 3*(2nd+4th+6th+8th)) mod 10 |
| --- |

Unknown if the barcode is internally used for any purposes (such like
encryption, or network identification).

**dev.kp**

This file contains another "TWxxxxxxxx" ID (with "xxxxxxxx" being a 32bit
lowercase hex number), this 32bit Console ID is also used in .tik files (except
in tickets for free system titles).

**Wifi MAC Address - Found in Main RAM, Wifi FLASH, and Wifi EEPROM**

The MAC is a unique 48bit number needed for Wifi communications. The MAC is
stored in SPI bus Wifi FLASH, and it's also stored at 2FFFCF4h in RAM. The same
MAC value is also stored in I2C bus Wifi EEPROM. The MAC can be also viewed in
Firmware (see System Settings, Internet, Options, System Information). Common
values for DSi are:

```

| 00 22 4C xx xx xx ;seen in DSi XL
 00 23 CC xx xx xx ;seen in DSi
 00 24 1E xx xx xx ;seen in DSi
 00 27 09 xx xx xx ;seen in DSi |
| --- |

The value isn't used for eMMC encryption (the eMMC is still accessible when
swapping the Wifi daughterboard). However, the MAC value is included in game
".bin" files stored on SD card (unknown if that is causing any issues when
loading those games on a console with swapped Wifi daughterboard).

**Nintendo WFC ID**

This is some unknown purpose value stored in Wifi FLASH. The value can be
viewed in Firmware (see System Settings, Internet, Options, System
Information). The firmware does only show the lower 43bit of the value, in
decimal format, multiplied by 1000, whilst the actual WFC in FLASH seems to be
about 14 bytes (112bit). The firmware does also allow to "delete" and
"transfer" the "WFC Configuration" (whatever that means).

**Flipnote Studio ID**

This ID consists of a 16-digit HEX number (can be viewed by clicking the "tool"
symbol in upper right of Flipnote's main menu).

The first 8-digits have unknown meaning. The last 8-digits are same as the last
8-digits of the wifi MAC address.