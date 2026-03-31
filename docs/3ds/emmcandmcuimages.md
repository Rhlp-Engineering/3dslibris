# Emmcandmcuimages

> Source: https://problemkaputt.de/gbatek.htm
> Section: Emmcandmcuimages

3DS eMMC and MCU Images 
| |
| --- |

 **eMMC Images**
 no$gba can load 3DS eMMC images from following files in no$gba folder (currently that merely allows to view eMMC images in Window/Filesystem, ie. there isn't any actual 3DS emulation supported in no$gba):
 
```

| 3DS-#.mmc ;\with "#" being the machine number ("1".."C")
 New3DS-#.mmc ;/ |
| --- |

For decrypting the image, CID and OTP needs to be stored at following eMMC
offsets:

```

| DC00h 20h ID '3DS CID/OTP/NCSD/FIRM BackupData'
 DC20h 10h eMMC CID (from eMMC command CMD10)
 DC30h D0h Reserved
 DD00h 100h OTP (from ARM9 I/O address 10012000h)
 DE00h 200h Copy of original NCSD (from eMMC offset 00000000h)
 E000h 1000h Copy of original FIRM0 (from eMMC offset 0B130000h)
 F000h 1000h Copy of original FIRM1 (from eMMC offset 0B530000h) |
| --- |

The copy of original NCSD/FIRM0/FIRM1 allows to uninstall sighax-based stuff
(or to install such stuff; doing that works even without having OTP dump, as
long as the unencrypted content of the NCSD/FIRM0/FIRM1 sectors is known).

**MCU Firmware Images**

The MCU image can be viewed in no$gba's RL78 disassembler (via Alt+W, R). The
image is loaded from following files in no$gba directory:

```

| 3DS.mcu
 New3DS.mcu |
| --- |

The file should contain a copy of the whole 8000h-byte MCU flash memory:

```

| 0000h 1000h Part 1 current version
 1000h 1000h Part 1 old version backup
 2000h 3000h Part 2 current version
 5000h 3000h Part 2 old version backup |
| --- |

If the old version areas aren't dumped, best leave them empty.

**Atheros Wifi ROM image**

The Wifi ROM image can be viewed in no$gba's Xtensa disassembler (via Alt+W,
X). The image is loaded from following file (in no$gba directory):

```

| AR6014G.ROM ;3DS wifi chip (256Kbytes) |
| --- |