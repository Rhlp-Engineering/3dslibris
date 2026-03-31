# Filessavedataextdata

> Source: https://problemkaputt.de/gbatek.htm
> Section: Filessavedataextdata

3DS Files - Savedata Extdata 
| |
| --- |

 Extdata files in FAT filesystem:
 
```

| ..\00000000\00000001 ;-DIFF:VSXE directory
 ..\00000000\00000002 ;-DIFF:raw_data (usually 1-byte icon?)
 ..\00000000\00000003 ;\
 ..\00000000\00000004 ; DIFF:raw_data (usually user/boss files, if any)
 ..\00000000\ (etc.) ;/
 ..\Quota.dat ;-DIFF:raw_data (QUOT, whatever) |
| --- |

Example content of the above DIFF:VSXE filesystem:

```

| icon ;with 64bit VSXE.ID same as DIFI.ID of 00000000\00000002
 boss\ ;usually empty folder (used in SpotPass data)
 user\somefile.dat ;with 64bit VSXE.ID same as DIFI.ID of 00000000\000000xx
 user\otherfile.bin ;with 64bit VSXE.ID same as DIFI.ID of 00000000\000000xx |
| --- |

"This page describes the format and encryption of extdata, "extra data" stored
on SD card and NAND, at":

```

| 3ds:\data\<ID0>\extdata\00048000\
 sd:\Nintendo 3DS\<ID0>\<ID1>\extdata\00000000\ |
| --- |

"Extdata ID High is always 00000000 for SD, and always 00048000 for eMMC.
Regular apps can only mount SD extdata using the same Extdata ID which is
stored in the CXI exheader. Therefore, regular apps which have the exheader
Extdata ID set to zero can't use extdata. This restriction doesn't apply for
shared extdata with Extdata ID high "bitmask"(=?) 48000h stored on NAND. System
apps with a certain(=?) access right can mount arbitrary extdata. All NAND
extdata is shared extdata, while all SD extdata is normal extdata."

**Quota.dat File (DIFF container with 48h-byte raw data)**

```

| 000h 4 ID "QUOT"
 004h 4 Version (00030000h)
 008h 4 Always 1000h, maybe DIFF/VSXE cluster/block size?
 00Ch 4 Always 7Eh, maybe \00000000\ directory capacity?
 010h 4 Zero
 014h 4 Unknown (eg. 4000h, 134h, 8D9h, or 2A8h, or C8h) ;maybe max size
 018h 4 Zero
 01Ch 4 Unknown (eg. 3FC0h, 123h, 121h, or 0EEh, or B0h) ;maybe max-xx
 020h 4 Zero
 024h 4 Zero
 028h 4 Unknown (eg. 3FD7h, 128h, 128h, or 17Dh, or B0h) ;maybe max-x
 02Ch 4 Zero
 030h 4 Most recent "00000000\000000xx" file number? (2 and up)
 034h 4 Zero
 038h 4 Zero
 03Ch 4 Zero
 040h 4 Most recent "00000000\000000xx" file size? (or 0=deleted?)
 044h 4 Zero |
| --- |

Quota.dat exists for eMMC shared extdata only (not for SD extdata). The purpose
of the Quota.dat file is unknown (it might be some kind of log file, and/or
containing some settings/limits or so).

**VSXE File System**

The extdata\00048000\f00000xx\00000000/00000001 files contain a VSXE filesystem
(inside of the DIFF container).

3DS Files - Savedata SAVE/BDRI/VSXE Formats

The VSXE format does have file & directory names, but without actually
assigning data clusters to files (instead, VSXE does merely assign ID values to
each "file" - and that ID refers to the DIFF[154h] ID in one of the
extdata\00048000\f00000xx\00000000/000000xx files).

**Virtual File System Structure**

When extdata is created, these are always created regardless of whether the
title actually uses them.

```

| /icon This virtual file contains the extdata icon displayed in data
 management. This icon can only be written to by titles when
 creating extdata, titles would have to recreate extdata to
 change the icon. This file can't be read directly, instead it
 is read via FS:ReadExtSaveDataIcon.
 /user/ This virtual directory contains the title's actual extdata files.
 /boss/ This virtual directory can contain SpotPass content. SpotPass
 content can only be downloaded to this /boss virtual directory. |
| --- |

User extdata and SpotPass extdata use separate mount points at /user and /boss.
Therefore one mount can't access the other virtual directory, and also can't
access /icon (the title's SpotPass extdata can be mounted by the title itself,
if it uses SpotPass).

**Extended Banner (optional)**

Folder "\user\ExBanner\" can contain optional extended banner files:

```

| \user\ExBanner\COMMON.bin ;-common data for all regions
 \user\ExBanner\JPN_JP.bin ;\
 \user\ExBanner\USA_EN.bin ; per region/language
 etc. ;/ |
| --- |

If present, this banner is displayed instead of the CXI ExeFS banner.

Uh, displayed instead of? Or does it merely replace textures?

3DS Files - Video Banner (Extended Banner)

**SD Extdata ID's**

```

| JPN USA EUR Description Extdata images
 00000082 0000008f 00000098 Home Menu extdata, this contains home-menu
 savedata and cached icons for applications.
 00000200 00000210 00000220 System Settings extdata added with 2.0.0-2.
 00000207 00000217 00000227 Mii Maker, contains an ExBanner cleartext
 00000208 00000218 00000228 Streetpass Mii Plaza 11 mb big!
 00000209 00000219 00000229 eShop, contains store music in AAC format.
 0000020b 0000021b 0000022b Nintendo Zone
 0000020d 0000021d 0000022d Face Raiders, likely contains an ExBanner
 000002cc 000002cd 000002ce Home Menu theme
 ? 000004aa 000004ab Nintendo Video Extra Data |
| --- |

This is where the video files are stored, and includes the thumbnail, the
description, and possibly some checksum info in each video file stored in the
extdata images. There are always 9 files within the subdirectory "00000000" of
this folder, even without any videos downloaded. The files are "00000001" -
"00000009", and "00000003" - "00000008" have the same filesize of 50.7 MB. It
is possible to restore the older videos by overwriting all the files within
this directory. Provided of course you have made a backup of the files before
hand, by copying all the files within this directory to your computer. As far
I'm aware its not possible to mix and match the files in order to get certain
videos in one grouping, ie. having all 3 Zelda orchestral recordings in one
group of 4 Nintendo videos.

```

| 00000306 00000308 00000307 Mario Kart 7
 0000030b 0000030d 0000030c Nintendogs + Cats
 00000326 00000326 00000326 Pokdex 3D
 00000305 0000032d 0000033c Super Street Fighter IV 3D
 00000328 00000358 0000033b Ridge Racer 3D
 ? 0000034d 00000402 Samurai Warriors Chronicles
 ? 0000034f 0000038a Dead or Alive Dimensions
 00000481 N/A N/A Monster Hunter Tri G (Download-Quests)
 ? 00000517 00000518 Swapnote
 0000055d 0000055d 0000055d P-Letter X, P-Letter Y
 ? 00000725 00000724 Ambassador Certificate
 ? ? 000007af New Super Mario Bros. 2
 ? 00000863 00000864 Animal Crossing: New Leaf
 ? 00000a85 00000a86 Professor Layton and the Miracle Mask
 ? ? 00000a87 Professor Layton and the Azran Legacy (German)
 ? ? 00000b4f Fullblox / Crashmo
 ? ? 00000ba9 Pokmon Mystery Dungeon: Gates to Infinity
 ? ? 00000c24 Denpa men
 00000c73 00000c73 00000c73 Save Data Transfer Tool
 ? ? 00000d9a Donkey Kong Country Returns 3D: Trailer
 ? ? 00000ea6 Etrian Odyssey IV
 ? 00000edf 00000ee0 Super Smash Bros. for Nintendo 3DS
 ? 00000f14 00000f1e Phoenix Wright: Ace Attorney - Dual Destinies
 ? 00001007 00001005 Professor Layton vs Phoenix Wright: Ace Attorney
 ? ? 00001062 Nintendo Pocket Football Club
 ? ? 0000111c Yoshi's New Island
 ? ? 00001131 Fantasy Life
 000011c5 000011c5 000011c5 Pokmon Omega Ruby, Pokmon Alpha Sapphire
 ? ? 000012ca Mario vs. Donkey Kong: Tipping Stars
 ? ? 00001499 Korg DSN-12
 ? ? 000014f2 Animal Crossing: Happy Home Designer
 000014d1 000014d1 000014d1 Home Menu badge
 ? ? 00001632 Fullblox / Stretchmo
 ? ? 00001646 Pokmon Rumble World
 00001648 00001648 00001648 Pokmon Sun, Pokmon Moon
 0000165c 0000165c 0000165c Home Menu saved theme layouts
 ? ? 00001678 Yo-kai Watch
 ? ? 000018fa Phoenix Wright: Ace Attorney - Spirit of Justice
 ? ? 0000198f Animal Crossing: New Leaf - Welcome amiibo
 ? ? 00001a05 Super Mario Maker
 ? ? 00001a2e Swapdoodle |
| --- |

**eMMC Shared Extdata ID's**

```

| e0000000 Request system transfer (if present)
 f0000001 Camera application NAND JPEG/MPO files, phtcache.bin, UploadData.dat
 f0000002 Sound application NAND M4A files
 f0000009 SpotPass content storage for notifications
 f000000b Miis and Play/Usage Records
 Contains idb.dat, idbt.dat, gamecoin.dat, ubll.lst, CFL_DB.dat,
 and CFL_OldDB.dat. These files contain cleartext Miis and some
 data relating (including cached ICN data) to Play/Usage Records
 f000000c Contains bashotorya.dat and bashotorya2.dat
 f000000d Home Menu SpotPass content data storage
 f000000e Update notification versionlist.dat (added in 7.0.0-13) |
| --- |

**Shared Extdata "f000000b" gamecoin.dat**

```

| 000h 4 Fixed ID or so (4F00h)
 004h 2 Total Play Coins
 006h 2 Total Play Coins obtained on the date stored below. When the below
 date does not match the current date, this field is reset to zero,
 then the date (and other fields) are updated. Once this value
 is >=10, no more Play Coins can be obtained until the current date
 changes.
 008h 4 Total step count at the time a new Play Coin was obtained.
 00Ch 4 Step count for the day the last Play Coin was obtained, for that
 day's step count (same as the step count displayed by home-menu
 when this file was updated).
 010h 2 Year
 012h 1 Month
 013h 1 Day |
| --- |

The above date stores the last time new Play Coin(s) were obtained. The
contents of this file is updated by home-menu. PTM:GetTotalStepCount is not
checked constantly, after home-menu boot this is only checked when waking from
sleep-mode. Each time home-menu updates the contents of this file, home-menu
will set the Play Coin total to 300 if it's higher than the 300 Play Coin
limit.

**Shared Extdata "f000000b" ubll.lst**

List of blocked users.

Empty space is filled with 0Ch-long sequences of 00 00 ... 07

**Tools**

3ds-save-tool - Extract/verifies extdata