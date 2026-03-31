# Dscartridgesencryptionfirmware

> Source: https://problemkaputt.de/gbatek.htm
> Section: Dscartridgesencryptionfirmware

DS Cartridges, Encryption, Firmware 
| |
| --- |

 **Cartridges**
 DS Cartridge Header
 DS Cartridge Secure Area
 DS Cartridge Icon/Title
 DS Cartridge Protocol
 DS Cartridge Backup
 DS Cartridge NAND
 DS Cartridge I/O Ports
 DS Cartridge NitroROM and NitroARC File Systems
 DS Cartridge Unknown Commands
 DS Cartridge PassMe/PassThrough
 DS Cartridge GBA Slot
 
 **Cartridge File Formats**
 DS File Formats
 
 **Add-Ons**
 DS Cart Rumble Pak
 DS Cart Slider with Rumble
 DS Cart Expansion RAM
 DS Cart Infrared/Pedometers
 DS Cart Unknown Extras
 
 **Special Cartridges**
 DS Cart Cheat Action Replay DS
 DS Cart Cheat Codebreaker DS
 DS Cart DLDI Driver
 
 **Encryption**
 DS Encryption by Gamecode/Idcode (KEY1)
 DS Encryption by Random Seed (KEY2)
 
 **Firmware / Wifi Flash**
 DS Firmware Serial Flash Memory
 DS Firmware Header
 DS Firmware Wifi Calibration Data
 DS Firmware Wifi Internet Access Points
 DS Firmware User Settings
 DS Firmware Extended Settings
 
 
 
| DS Cartridge Header |
| --- |

 **Header Overview (loaded from ROM Addr 0 to Main RAM 27FFE00h on Power-up)**
 
```

| Address Bytes Expl.
 000h 12 Game Title (Uppercase ASCII, padded with 00h)
 00Ch 4 Gamecode (Uppercase ASCII, NTR-<code>) (0=homebrew)
 010h 2 Makercode (Uppercase ASCII, eg. "01"=Nintendo) (0=homebrew)
 012h 1 Unitcode (00h=NDS, 02h=NDS+DSi, 03h=DSi) (bit1=DSi)
 013h 1 Encryption Seed Select (00..07h, usually 00h)
 014h 1 Devicecapacity (Chipsize = 128KB SHL nn) (eg. 7 = 16MB)
 015h 7 Reserved (zero filled)
 01Ch 1 Reserved (zero) (except, used on DSi)
 01Dh 1 NDS Region (00h=Normal, 80h=China, 40h=Korea) (other on DSi)
 01Eh 1 ROM Version (usually 00h)
 01Fh 1 Autostart (Bit2: Skip "Press Button" after Health and Safety)
 (Also skips bootmenu, even in Manual mode & even Start pressed)
 020h 4 ARM9 rom_offset (4000h and up, align 1000h)
 024h 4 ARM9 entry_address (2000000h..23BFE00h)
 028h 4 ARM9 ram_address (2000000h..23BFE00h)
 02Ch 4 ARM9 size (max 3BFE00h) (3839.5KB)
 030h 4 ARM7 rom_offset (8000h and up)
 034h 4 ARM7 entry_address (2000000h..23BFE00h, or 37F8000h..3807E00h)
 038h 4 ARM7 ram_address (2000000h..23BFE00h, or 37F8000h..3807E00h)
 03Ch 4 ARM7 size (max 3BFE00h, or FE00h) (3839.5KB, 63.5KB)
 040h 4 File Name Table (FNT) offset
 044h 4 File Name Table (FNT) size
 048h 4 File Allocation Table (FAT) offset
 04Ch 4 File Allocation Table (FAT) size
 050h 4 File ARM9 overlay_offset
 054h 4 File ARM9 overlay_size
 058h 4 File ARM7 overlay_offset
 05Ch 4 File ARM7 overlay_size
 060h 4 Port 40001A4h setting for normal commands (usually 00586000h)
 064h 4 Port 40001A4h setting for KEY1 commands (usually 001808F8h)
 068h 4 Icon/Title offset (0=None) (8000h and up)
 06Ch 2 Secure Area Checksum, CRC-16 of [[020h]..00007FFFh]
 06Eh 2 Secure Area Delay (in 131kHz units) (051Eh=10ms or 0D7Eh=26ms)
 070h 4 ARM9 Auto Load List Hook RAM Address (?) ;\endaddr of auto-load
 074h 4 ARM7 Auto Load List Hook RAM Address (?) ;/functions
 078h 8 Secure Area Disable (by encrypted "NmMdOnly") (usually zero)
 080h 4 Total Used ROM size (remaining/unused bytes usually FFh-padded)
 084h 4 ROM Header Size (4000h)
 088h 4 Unknown, some rom_offset, or zero? (DSi: slightly different)
 08Ch 8 Reserved (zero filled; except, [88h..93h] used on DSi)
 094h 2 NAND end of ROM area ;\in 20000h-byte units (DSi: 80000h-byte)
 096h 2 NAND start of RW area ;/usually both same address (0=None)
 098h 18h Reserved (zero filled)
 0B0h 10h Reserved (zero filled; or "DoNotZeroFillMem"=unlaunch fastboot)
 0C0h 9Ch Nintendo Logo (compressed bitmap, same as in GBA Headers)
 15Ch 2 Nintendo Logo Checksum, CRC-16 of [0C0h-15Bh], fixed CF56h
 15Eh 2 Header Checksum, CRC-16 of [000h-15Dh]
 160h 4 Debug rom_offset (0=none) (8000h and up) ;only if debug
 164h 4 Debug size (0=none) (max 3BFE00h) ;version with
 168h 4 Debug ram_address (0=none) (2400000h..27BFE00h) ;SIO and 8MB
 16Ch 4 Reserved (zero filled) (transferred, and stored, but not used)
 170h 90h Reserved (zero filled) (transferred, but not stored in RAM)
 200h E00h Reserved (zero filled) (usually not transferred) |
| --- |

DSi Cartridges are using an extended cartridge header,

DSi Cartridge Header

Some of that new/changed DSi header entries are important even in NDS mode:

- On DSi, ARM9/ARM7 areas are restricted to 2.75MB (instead 3.8MB on real NDS)

- New NDS titles must have RSA signatures (and old titles must be in whitelist)

For more info about CRC-16, see description of GetCRC16 BIOS function,

BIOS Misc Functions

For the Logo checksum, the BIOS verifies only [15Ch]=CF56h, it does NOT verify
the actual data at [0C0h-15Bh] (nor it's checksum), however, the data is
verified by the firmware.

**Secure Area Delay**

The Secure Area Delay at header[06Eh] is counted in 130.912kHz units (which can
be clocked via one of the hardware timers with prescaler=F/256 and
reload=(10000h-((X AND 3FFFh)+2)); for some weird reason, in case of Header
checksum it's ANDed with 1FFFh instead of 3FFFh). Commonly used values are
X=051Eh (10ms), and X=0D7Eh (26ms).

The delay is used for all Blowfish encrypted commands, the actual usage/purpose
differs depending on bit31 of the ROM Chip ID:

When ChipID.Bit31=0 (commands are sent ONCE): The delay is issued BEFORE
sending the command:

```

| Delay,Cmd |
| --- |

Older/newer games are using delays of 10ms/26ms (although all known existing
cartridges with Bit31=0 would actually work WITHOUT delays).

When ChipID.Bit31=1 (commands are repeated MULTIPLE times): The delay is issued
AFTER sending the command for the FIRST time:

```

| Cmd,Delay,Cmd ;for 2x repeat
 Cmd,Delay,Cmd,Cmd,Cmd,Cmd,Cmd,Cmd,Cmd,Cmd ;for 9x repeat |
| --- |

Known games are using delays of 26ms (although all known existing cartridges
(=Cooking Coach) with Bit31=1 would actually work with shorter delays of ca.
7ms (but, better use 8ms for safety)).

**NDS Gamecodes**

This is the same code as the NTR-UTTD (NDS) or TWL-UTTD (DSi) code which is
printed on the package and sticker on (commercial) cartridges (excluding the
leading "NTR-" or "TWL-" part).

```

| U Unique Code (usually "A", "B", "C", or special meaning)
 TT Short Title (eg. "PM" for Pac Man)
 D Destination/Language (usually "J" or "E" or "P" or specific language) |
| --- |

The first character (U) is usually "A" or "B", in detail:

```

| A NDS common games
 B NDS common games
 C NDS common games
 D DSi-exclusive games
 H DSiWare (system utilities and browser) (eg. HNGP=browser)
 I NDS and DSi-enhanced games with built-in Infrared port
 K DSiWare (dsiware games and flipnote) (eg. KGUV=flipnote)
 N NDS nintendo channel demo's japan (NTR-NTRJ-JPN)
 T NDS many games
 U NDS and DSi uncommon extra hardware (eg. NAND, ram, microSD, TV, azimuth)
 V DSi-enhanced games
 Y NDS many games |
| --- |

The second/third characters (TT) are:

```

| Usually an abbreviation of the game title (eg. "PM" for "Pac Man") (unless
 that gamecode was already used for another game, then TT is just random) |
| --- |

The fourth character (D) indicates Destination/Language:

```

| A Asian E English/USA I Italian M Swedish Q Danish U Australian
 B N/A F French J Japanese N Nor R Russian V EUR+AUS
 C Chinese G N/A K Korean O Int S Spanish W..Z Europe #3..5
 D German H Dutch L USA #2 P Europe T USA+AUS |
| --- |

| DS Cartridge Secure Area |
| --- |

The Secure Area is located in ROM at 4000h..7FFFh, it can contain normal
program code and data, however, it can be used only for ARM9 boot code, it
cannot be used for ARM7 boot code, icon/title, filesystem, or other data.

**Secure Area Size**

The Secure Area exists if the ARM9 boot code ROM source address (src) is
located within 4000h..7FFFh, if so, it will be loaded (by BIOS via KEY1
encrypted commands) in 4K portions, starting at src, aligned by 1000h, up to
address 7FFFh. The secure area size if thus 8000h-src, regardless of the ARM9
boot code size entry in header.

Note: The BIOS silently skips any NDS9 bootcode at src<4000h.

Cartridges with src>=8000h do not have a secure area.

**Secure Area ID**

The first 8 bytes of the secure area are containing the Secure Area ID, the ID
is required (verified by BIOS boot code), the ID value changes during boot
process:

```

| Value Expl.
 "encryObj" raw ID before encryption (raw ROM-image)
 (encrypted) encrypted ID after encryption (encrypted ROM-image)
 "encryObj" raw ID after decryption (verified by BIOS boot code)
 E7FFDEFFh,E7FFDEFFh destroyed ID (overwritten by BIOS after verify) |
| --- |

If the decrypted ID does match, then the BIOS overwrites the first 8 bytes by
E7FFDEFFh-values (ie. only the ID is destroyed). If the ID doesn't match, then
the first 800h bytes (2K) are overwritten by E7FFDEFFh-values.

**Secure Area First 2K Encryption/Content**

The first 2K of the Secure Area (if it exists) are KEY1 encrypted. In official
games, this 2K region contains data like so (in decrypted form):

```

| 000h..007h Secure Area ID (see above)
 008h..00Dh Fixed (FFh,DEh,FFh,E7h,FFh,DEh)
 00Eh..00Fh CRC16 across following 7E0h bytes, ie. [010h..7FFh]
 010h..7FDh Unknown/random values, mixed with some THUMB SWI calls
 7FEh..7FFh Fixed (00h,00h) |
| --- |

Of which, only the ID in the first 8 bytes is verified. Neither BIOS nor
(current) firmare versions are verifying the data at 008h..7FFh, so the 7F8h
bytes may be also used for normal program code/data.

**Avoiding Secure Area Encryption**

WLAN files are reportedly same format as cartridges, but without Secure Area,
so games with Secure Area cannot be booted via WLAN. No$gba can encrypt and
decrypt Secure Areas only if the NDS BIOS-images are present. And, Nintendo's
devkit doesn't seem to support Secure Area encryption of unreleased games.

So, unencrypted cartridges are more flexible in use. Ways to avoid encryption
(which still work on real hardware) are:

1) Set NDS9 ROM offset to 4000h, and leave the first 800h bytes of the Secure
Area 00h-filled, which can be (and will be) safely destroyed during loading;
due to the missing "encryObj" ID; that method is used by Nintendo's devkit.

2) Set NDS9 ROM offset to 8000h or higher (cartridge has no Secure Area at
all).

3) Set NDS9 ROM offset, RAM address, and size to zero, set NDS7 ROM offset to
200h, and point both NDS9 and NDS7 entrypoints to the loaded NDS7 region. That
method avoids waste of unused memory at 200h..3FFFh, and it should be
compatible with the NDS console, however, it is not comaptible with commercial
cartridges - which do silently redirect address below 4000h to
"addr=8000h+(addr AND 1FFh)". Still, it should work with inofficial flashcards,
which do not do that redirection. No$gba emulates the redirection for regular
official cartridges, but it disables redirection for homebrew carts if NDS7 rom
offset<8000h, and NDS7 size>0.

[One possible problem: Newer "anti-passme" firmware versions reportedly check
that the entrypoint isn't set to 80000C0h, that firmwares might also reject
NDS9 entrypoints within the NDS7 bootcode region?]

| DS Cartridge Icon/Title |
| --- |

The ROM offset of the Icon/Title is defined in CartHdr[68h]. The size was
originally implied by the size of the original Icon/Title structure rounded to
200h-byte sector boundary (ie. A00h bytes for Version 1 or 2), however, later
DSi carts (with CartHdr[1Ch].bit0=1) are having a size entry at CartHdr[208h]
(usually 23C0h).

If present (ie. if CartHdr[68h]=nonzero), then Icon/Title are displayed in the
bootmenu.

```

| 0000h 2 Version (0001h, 0002h, 0003h, or 0103h)
 0002h 2 CRC16 across entries 0020h..083Fh (all versions)
 0004h 2 CRC16 across entries 0020h..093Fh (Version 0002h and up)
 0006h 2 CRC16 across entries 0020h..0A3Fh (Version 0003h and up)
 0008h 2 CRC16 across entries 1240h..23BFh (Version 0103h and up)
 000Ah 16h Reserved (zero-filled)
 0020h 200h Icon Bitmap (32x32 pix) (4x4 tiles, 4bit depth) (4x8 bytes/tile)
 0220h 20h Icon Palette (16 colors, 16bit, range 0000h-7FFFh)
 (Color 0 is transparent, so the 1st palette entry is ignored)
 0240h 100h Title 0 Japanese (128 characters, 16bit Unicode)
 0340h 100h Title 1 English ("")
 0440h 100h Title 2 French ("")
 0540h 100h Title 3 German ("")
 0640h 100h Title 4 Italian ("")
 0740h 100h Title 5 Spanish ("")
 0840h 100h Title 6 Chinese ("") (Version 0002h and up)
 0940h 100h Title 7 Korean ("") (Version 0003h and up)
 0A40h 800h Zerofilled (probably reserved for Title 8..15) |
| --- |

Below for animated DSi icons only (Version 0103h and up):

```

| 1240h 1000h Icon Animation Bitmap 0..7 (200h bytes each, format as above)
 2240h 100h Icon Animation Palette 0..7 (20h bytes each, format as above)
 2340h 80h Icon Animation Sequence (16bit tokens) |
| --- |

Unused/padding bytes:

```

| 0840h 1C0h Unused/padding (FFh-filled) in Version 0001h
 0940h C0h Unused/padding (FFh-filled) in Version 0002h
 23C0h 40h Unused/padding (FFh-filled) in Version 0103h |
| --- |

**Versions**

```

| 0001h = Original
 0002h = With Chinese Title
 0003h = With Chinese+Korean Titles
 0103h = With Chinese+Korean Titles and animated DSi icon |
| --- |

**Title Strings**

Usually, for non-multilanguage games, the same (english) title is stored in all
title entries. The title may consist of ASCII characters 0020h-007Fh, character
000Ah (linefeed), and should be terminated/padded by 0000h.

The whole text should not exceed the dimensions of the DS cart field in the
bootmenu (the maximum number of characters differs due to proportional font).

The title is usually split into a primary title, optional sub-title, and
manufacturer, each separated by 000Ah character(s). For example: "America",
000Ah, "The Axis of War", 000Ah, "Cynicware", 0000h.

**Icon Animation Sequence (DSi)**

The sequence is represented by 16bit tokens, in the following format:

```

| 15 Flip Vertically (0=No, 1=Yes)
 14 Flip Horizontally (0=No, 1=Yes)
 13-11 Palette Index (0..7)
 10-8 Bitmap Index (0..7)
 7-0 Frame Duration (01h..FFh) (in 60Hz units) |
| --- |

Value 0000h indicates the end of the sequence. If the first token is 0000h,
then the non-animated default image is shown.

Uh, actually, a non-animated icon uses values 01h,00h,00h,01h, followed by 7Ch
zerofilled bytes (ie. 0001h, 0100h, 3Eh x 0000h)?

**FAT16:\title\000300tt\4ggggggg\data\banner.sav ;if carthdr[1BFh].bit2=1**

Some DSi games are having a separate "banner.sav" file stored in the eMMC
filesystem, enabled via carthdr[1BFh].bit2 (allowing to indicate the game
progress by overriding the default icon). The banner files are 4000h bytes in
size, the animation data is same as above, but without title strings and
without non-animated icon.

```

| 0000h 2 Version (0103h)
 0002h 6 Reserved (zero-filled)
 0008h 2 CRC16 across entries 0020h..119Fh (with initial value FFFFh)
 000Ah 16h Reserved (zero-filled)
 0020h 1000h Icon Animation Bitmap 0..7 (200h bytes each) ;\same format as
 1020h 100h Icon Animation Palette 0..7 (20h bytes each) ; in Icon/Title
 1120h 80h Icon Animation Sequence (16bit tokens) ;/
 11A0h 2E60h Garbage (random values, maybe due to eMMC decryption) |
| --- |

The feature is used by some Brain Age Express games (for example, Brain Age
Express Sudoku: 'title\00030004\4b4e3945\data\banner.sav').

The feature does probably work only for DSiware titles (unless there are any
DSi carts with SD/MMC access enabled; or unless there is a feature for storing
similar data in cartridge memory).

| DS Cartridge Protocol |
| --- |

Communication with Cartridge ROM relies on sending 8 byte commands to the
cartridge, after the sending the command, a data stream can be received from
the cartridge (the length of the data stream isn't fixed, below descriptions
show the default length in brackets, but one may receive more, or less bytes,
if desired).

**Cartridge Memory Map**

```

| 0000000h-0000FFFh Header (unencrypted)
 0001000h-0003FFFh Not read-able (zero filled in ROM-images)
 0004000h-0007FFFh Secure Area, 16KBytes (first 2Kbytes with extra encryption)
 0008000h-... Main Data Area |
| --- |

DSi cartridges are split into a NDS area (as above), and a new DSi area:

```

| XX00000h XX02FFFh DSi Not read-able (XX00000h=first megabyte after NDS area)
 XX03000h-XX06FFFh DSi ARM9i Secure Area (usually with modcrypt encryption)
 XX07000h-... DSi Main Data Area |
| --- |

Cartridge memory must be copied to RAM (the CPU cannot execute code in ROM).

**Command Summary, Cmd/Reply-Encryption Type, Default Length**

```

| Command/Params Expl. Cmd Reply Len
 -- Unencrypted Load --
 9F00000000000000h Dummy (read HIGH-Z bytes) RAW RAW 2000h
 0000000000000000h Get Cartridge Header RAW RAW 200h DSi:1000h
 00aaaaaaaa000000h Get Cartridge Header (1T-ROM,NAND)RAW RAW 200h
 9000000000000000h 1st Get ROM Chip ID RAW RAW 4
 A000000000000000h Get 3DS encryption type (3DS) RAW RAW 4
 00aaaaaaaa000000h Unencrypted Data (debug ver only) RAW RAW 200h
 3Ciiijjjxkkkkkxxh Activate KEY1 Encryption (NDS) RAW RAW 0
 3Diiijjjxkkkkkxxh Activate KEY1 Encryption (DSi) RAW RAW 0
 3E00000000000000h Activate 16-byte commands (3DS) RAW RAW 0
 -- Secure Area Load --
 4llllmmmnnnkkkkkh Activate KEY2 Encryption Mode KEY1 FIX 910h+0
 1lllliiijjjkkkkkh 2nd Get ROM Chip ID KEY1 KEY2 910h+4
 xxxxxxxxxxxxxxxxh Invalid - Get KEY2 Stream XOR 00h KEY1 KEY2 910h+...
 2bbbbiiijjjkkkkkh Get Secure Area Block (4Kbytes) KEY1 KEY2 910h+10A8h
 6lllliiijjjkkkkkh Optional KEY2 Disable KEY1 KEY2 910h+?
 Alllliiijjjkkkkkh Enter Main Data Mode KEY1 KEY2 910h+0
 -- Main Data Load --
 B7aaaaaaaa000000h Encrypted Data Read KEY2 KEY2 200h
 B800000000000000h 3rd Get ROM Chip ID KEY2 KEY2 4
 xxxxxxxxxxxxxxxxh Invalid - Get KEY2 Stream XOR 00h KEY2 KEY2 ...
 B500000000000000h Whatever NAND related? (DSi?) KEY2 KEY2 0
 D600000000000000h Whatever NAND related? (DSi?) KEY2 KEY2 4 |
| --- |

The parameter digits contained in above commands are:

```

| aaaaaaaa 32bit ROM address (command B7 can access only 8000h and up)
 bbbb Secure Area Block number (0004h..0007h for addr 4000h..7000h)
 x,xx Random, not used in further commands (DSi: always zero)
 iii,jjj,llll Random, must be SAME value in further commands
 kkkkk Random, must be INCREMENTED after FURTHER commands
 mmm,nnn Random, used as KEY2-encryption seed |
| --- |

```

| ____________ Unencrypted Commands (First Part of Boot Procedure) _____________ |
| --- |

**Cartridge Reset**

The /RES Pin switches the cartridge into unencrypted mode. After reset, the
first two commands (9Fh and 00h) are transferred at 4MB/s CLK rate.

**9F00000000000000h (2000h) - Dummy**

Dummy command send after reset, returns endless stream of HIGH-Z bytes (ie.
usually receiving FFh, immediately after sending the command, the first 1-2
received bytes may be equal to the last command byte).

**0000000000000000h (200h) (DSi:1000h) - Get Header (from address 00000000h)**

**00aaaaaaaa000000h (200h) - DSi Get Header Snippet (from address aaaaaaaah)**

Returns RAW unencrypted cartridge header, usually repeated every 1000h bytes.
Some carts allow to read the header with a single 200h-byte or 1000h-byte read,
others require eight separate 200h-byte reads.

```

| NDS/MROM --> Read 200h bytes from address 000h
 NDS/1T-ROM --> Read 200h bytes from address 000h
 NDS/NAND --> Read 200h bytes from address 000h
 DSi/MROM --> Read 1000h bytes from address 000h
 DSi/1T-ROM --> Read 8x200h bytes from address 000h,200h,400h,..,E00h
 DSi/NAND --> Read 8x200h bytes from address 000h,200h,400h,..,E00h |
| --- |

Note: DSi/1T-ROM usually allows to read 1000h bytes or 8x200h, but DSi/NAND
works only when reading 8x200h bytes.

The Gamecode header entry is used later on to initialize the encryption. Also,
the ROM Control entries define the length of the KEY1 dummy periods (typically
910h clocks), and the CLK transfer rate for further commands (typically faster
than the initial 4MB/s after power up).

**9000000000000000h (4) - 1st Get ROM Chip ID**

Returns RAW unencrypted Chip ID (eg. C2h,0Fh,00h,00h), repeated every 4 bytes.

```

| 1st byte - Manufacturer (eg. C2h=Macronix) (roughly based on JEDEC IDs)
 2nd byte - Chip size (00h..7Fh: (N+1)Mbytes, F0h..FFh: (100h-N)*256Mbytes?)
 3rd byte - Flags (see below)
 4th byte - Flags (see below) |
| --- |

The Flag Bits in 3th byte can be

```

| 0 Uses Infrared (but via SPI, unrelated to ROM) (also Jam with the Band)
 1 Unknown (set in some 3DS carts)
 2-6 Zero
 7 Unknown (set in Kingdom Hearts - Re-Coded) |
| --- |

The Flag Bits in 4th byte can be

```

| 0-2 Zero
 3 NAND flag (0=ROM, 1=NAND)
 4 3DS Flag (0=NDS/DSi, 1=3DS)
 5 Unknown (0=Normal, 1=Support cmd B5h/D6h)
 6 DSi flag (0=NDS/3DS, 1=DSi) (but also set in NDS Walk with Me)
 7 Cart Protocol Variant (0=old/smaller MROM, 1=new/bigger 1T-ROM or NAND) |
| --- |

Existing/known ROM IDs are:

```

| C2h,07h,00h,00h NDS Macronix 8MB ROM (eg. DS Vision, with microSD slot)
 AEh,0Fh,00h,00h NDS Noname 16MB ROM (eg. Meine Tierarztpraxis)
 C2h,0Fh,00h,00h NDS Macronix 16MB ROM (eg. Metroid Demo)
 C2h,1Fh,00h,00h NDS Macronix 32MB ROM (eg. Over the Hedge)
 C2h,1Fh,00h,40h DSi Macronix 32MB ROM (eg. Art Academy, TWL-VAAV, SystemFlaw)
 80h,3Fh,01h,E0h NDS SanDisk 64MB ROM+Infrared (eg. Walk with Me, NTR-IMWP)
 AEh,3Fh,00h,E0h DSi Noname 64MB ROM (eg. de Blob 2, TWL-VD2V)
 C2h,3Fh,00h,00h NDS Macronix 64MB ROM (eg. Ultimate Spiderman)
 C2h,3Fh,00h,40h DSi Macronix 64MB ROM (eg. Crime Lab, NTR-VAOP)
 80h,7Fh,00h,80h NDS SanDisk 128MB ROM (DS Zelda, NTR-AZEP-0)
 80h,7Fh,01h,E0h ? SanDisk? 128MB ROM+Infrared (P-letter SoulSilver, IPGE)
 C2h,7Fh,00h,80h NDS Macronix 128MB ROM (eg. Spirit Tracks, NTR-BKIP)
 C2h,7Fh,00h,C0h DSi Macronix 128MB ROM (eg. Cooking Coach, TWL-VCKE)
 ECh,7Fh,00h,88h NDS Samsung 128MB NAND (eg. Warioware D.I.Y., NTR-UORE)
 ECh,7Fh,01h,88h NDS Samsung 128MB NAND (eg. Jam with the Band, NTR-UXBP)
 ECh,7Fh,00h,E8h DSi Samsung 128MB NAND (eg. Face Training, TWL-USKV)
 80h,FFh,80h,E0h NDS SanDisk? 256MB ROM (Kingdom Hearts - Re-Coded, NTR-BK9P)
 C2h,FFh,01h,C0h DSi Macronix 256MB ROM+Infrared (eg. P-Letter White)
 C2h,FFh,00h,80h NDS Macronix 256MB ROM (eg. Band Hero, NTR-BGHP)
 C2h,FEh,01h,C0h DSi Macronix 512MB ROM+Infrared (eg. P-Letter White 2)
 C2h,FEh,00h,90h 3DS Macronix probably 512MB? ROM (eg. Sims 3)
 45h,FAh,00h,90h 3DS SanDisk? maybe... 1GB? ROM (eg. Starfox)
 C2h,F8h,00h,90h 3DS Macronix maybe... 2GB? ROM (eg. Kid Icarus)
 C2h,7Fh,00h,90h 3DS Macronix 128MB ROM CTR-P-AENJ MMinna no Ennichi
 C2h,FFh,00h,90h 3DS Macronix 256MB ROM CTR-P-AFSJ Pro Yakyuu Famista 2011
 C2h,FEh,00h,90h 3DS Macronix 512MB ROM CTR-P-AFAJ Real 3D Bass FishingFishOn
 C2h,FAh,00h,90h 3DS Macronix 1GB ROM CTR-P-ASUJ Hana to Ikimono Rittai Zukan
 C2h,FAh,02h,90h 3DS Macronix 1GB ROM CTR-P-AGGW Luigis Mansion 2 ASiA CHT
 C2h,F8h,00h,90h 3DS Macronix 2GB ROM CTR-P-ACFJ Castlevania - Lords of Shadow
 C2h,F8h,02h,90h 3DS Macronix 2GB ROM CTR-P-AH4J Monster Hunter 4
 AEh,FAh,00h,90h 3DS Noname? 1GB ROM CTR-P-AGKJ Gyakuten Saiban 5
 AEh,FAh,00h,98h 3DS Noname? 1GB NAND CTR-P-EGDJ Tobidase Doubutsu no Mori
 45h,FAh,00h,90h 3DS SanDisk? 1GB ROM CTR-P-AFLJ Fantasy Life
 45h,F8h,00h,90h 3DS SanDisk? 2GB ROM CTR-P-AVHJ Senran Kagura Burst - Guren
 C2h,F0h,00h,90h 3DS Macronix 4GB ROM CTR-P-ABRJ Biohazard Revelations
 ?,?,?,? NDS ? ? (eg. Japanese TV Tuner, NTR-UNSJ)
 00h,00h,00h,00h Cart Reset Busy (Face Training needs 20ms delay after reset)
 FFh,FFh,FFh,FFh None (no cartridge inserted) |
| --- |

The official JEDEC ID for Samsung would be "CEh", but for some reason,
Samsung's NDS chip does spit out "ECh" as Maker ID. SanDisk has two IDs (80h on
NDS/DSi, and 45h on 3DS).

**3Ciiijjjxkkkkkxxh (0) - Activate KEY1 Encryption Mode**

The 3Ch command returns endless stream of HIGH-Z bytes, all following commands,
and their return values, are encrypted. The random parameters iii,jjj,kkkkk
must be re-used in further commands; the 20bit kkkkk value is to be incremented
by one after each <further> command (it is <not> incremented after
the 3Ch command).

**3Diiijjjxkkkkkxxh (0) - Activate KEY1 Encryption Mode and Unlock DSi Mode**

Same as command 3Ch (but with different initial 1048h-byte encryption values),
and works only on DSi carts. Command 3Dh is unlocking two features on DSi
carts:

```

| 1) Command 2bbbbiiijjjkkkkkh loads ARM9i secure area (instead of ARM9 area)
 2) Command B7aaaaaaaa000000h allows to read the 'whole' cartridge space |
| --- |

Without command 3Dh, DSi carts will allow to read only the first some megabytes
(for example, the first 11 Mbyte of the System Flaw cartridge), and the
remaining memory returns mirrors of "addr=8000h+(addr AND 1FFh)").

Note: After reset, the cartridge protocol allows to send only either one of the
3Ch/3Dh commands (DSi consoles can control the cartridge reset pin, so they can
first send 3Ch and read the normal secure area, then issue a reset and 3Dh and
read the DSi secure area) (on a NDS one could do the same by ejecting/inserting
the cartridge instead of toggling the reset pin).

```

| ____________ KEY1 Encrypted Commands (2nd Part of Boot procedure) ____________ |
| --- |

**4llllmmmnnnkkkkkh (910h) - Activate KEY2 Encryption Mode**

KEY1 encrypted command, parameter mmmnnn is used to initialize the KEY2
encryption stream. Returns 910h dummy bytes (which are still subject to old
KEY2 settings; at pre-initialization time, this is fixed: HIGH-Z, C5h, 3Ah,
81h, etc.). The new KEY2 seeds are then applied, and the first KEY2 byte is
then precomputed. The 910h dummy stream is followed by that precomputed byte
value endless repeated (this is the same value as that "underneath" of the
first HIGH-Z dummy-byte of the next command).

Secure1000h: Returns repeated FFh bytes (instead of the leading C5h, 3Ah, 81h,
etc. stuff).

Secure1000h: Returns repeated FFh bytes (instead of the repeated precomputed
value).

**1lllliiijjjkkkkkh (914h) - 2nd Get ROM Chip ID / Get KEY2 Stream**

KEY1 encrypted command. Returns 910h dummy bytes, followed by KEY2 encrypted
Chip ID repeated every 4 bytes, which must be identical as for the 1st Get ID
command. The BIOS randomly executes this command once or twice. Changing the
first command byte to any other value returns an endless KEY2 encrypted stream
of 00h bytes, that is the easiest way to retrieve encryption values and to
bypass the copyprotection.

**2bbbbiiijjjkkkkkh (19B8h) - Get Secure Area Block**

KEY1 encrypted command. Used to read a secure area block (bbbb in range
0004h..0007h for addr 4000h..7000h) (or, after sending command 3Dh on a DSi:
bbbb in range 0004h..0007h for addr XX03000h..XX06000h).

Each block is 4K, so it requires four Get Secure Area commands to receive the
whole Secure Area (ROM locations 4000h-7FFFh), the BIOS is reading these blocks
in random order.

Normally (if the upper bit of the Chip ID is set): Returns 910h dummy bytes,
followed by 200h KEY2 encrypted Secure Area bytes, followed by 18h KEY2
encrypted 00h bytes, then the next 200h KEY2 encrypted Secure Area bytes, again
followed by 18h KEY2 encrypted 00h bytes, and so on. That stream is repeated
every 10C0h bytes (8x200h data bytes, plus 8x18h zero bytes).

Alternately (if the upper bit of the Chip ID is zero): Returns 910h dummy
bytes, followed by 1000h KEY2 encrypted Secure Area bytes, presumably followed
by 18h bytes, too.

Aside from above KEY2 encryption (which is done by hardware), the first 2K of
the NDS Secure Area is additionally KEY1 encrypted; which must be resolved
after transfer by software (and the DSi Secure Area is usually modcrypted, as
specified in the cartridge header).

**6lllliiijjjkkkkkh (0) - Optional KEY2 Disable**

KEY1 encrypted command. Returns 910h dummy bytes (which are still KEY2
affected), followed by endless stream of RAW 00h bytes. KEY2 encryption is
disabled for all following commands.

This command is send only if firmware[18h] matches encrypted string "enPngOFF",
and ONLY if firmware get_crypt_keys had completed BEFORE completion of secure
area loading, this timing issue may cause unstable results.

**Alllliiijjjkkkkkh (910h) - Enter Main Data Mode**

KEY1 encrypted command. Returns 910h dummy bytes, followed by endless KEY2
encrypted stream of 00h bytes. All following commands are KEY2 encrypted.

```

| ________________ KEY2 Encrypted Commands (Main Data Transfer) ________________ |
| --- |

**B7aaaaaaaa000000h (200h) - Get Data**

KEY2 encrypted command. The desired ROM address is specifed, MSB first, in
parameter bytes (a). Returned data is KEY2 encrypted.

There is no alignment restriction for the address. However, the datastream
wraps to the begin of the current 4K block when address+length crosses a 4K
boundary (1000h bytes). Special case: SanDisk ROMs are forcefully 200h-byte
aligned, and can merely read max 200h bytes (padded with unencrypted FFh bytes
when trying to read more data).

The command can be used only for addresses 8000h and up. Addresses 0..7FFFh are
silently redirected to address "8000h+(addr AND 1FFh)". DSi cartridges will
also reject XX00000h..XX06FFFh in the same fashion (and also XX07000h and up if
the DSi cartridge isn't unlocked via command 3Dh).

Addresses that do exceed the ROM size do mirror to the valid address range
(that includes mirroring non-loadable regions like 0..7FFFh to "8000h+(addr AND
1FFh)"; some newer games are using this behaviour for some kind of anti-piracy
checks).

**B800000000000000h (4) - 3rd Get ROM Chip ID**

KEY2 encrypted command. Returns KEY2 encrypted Chip ID repeated every 4 bytes.

**xxxxxxxxxxxxxxxxh - Invalid Command**

Any other command (anything else than above B7h and B8h) in KEY2 command mode
causes communcation failures. The invalid command returns an endless KEY2
encrypted stream of 00h bytes. After the invalid command, the KEY2 stream is
NOT advanced for further command bytes, further commands seems to return KEY2
encrypted 00h bytes, of which, the first returned byte appears to be HIGH-Z.

Ie. the cartridge seems to have switched back to a state similar to the
KEY1-phase, although it doesn't seem to be possible to send KEY1 commands.

```

| ___________________________________ Notes ___________________________________ |
| --- |

**KEY1 Command Encryption / 910h Dummy Bytes**

All KEY1 encrypted commands are followed by 910h dummy byte transfers, these
910h clock cycles are probably used to decrypt the command at the cartridge
side; communication will fail when transferring less than 910h bytes.

The return values for the dummy transfer are: A single HIGH-Z byte, followed by
90Fh KEY2-encrypted 00h bytes. The KEY2 encryption stream is advanced for all
910h bytes, including for the HIGH-Z byte.

Note: Current cartridges are using 910h bytes, however, other carts might use
other amounts of dummy bytes, the 910h value can be calculated based on ROM
Control entries in cartridge header. For the KEY1 formulas, see:

DS Encryption by Gamecode/Idcode (KEY1)

**KEY2 Command/Data Encryption**

DS Encryption by Random Seed (KEY2)

**Cart Protocol Variants (Chip ID.Bit31)**

There are two protocol variants for NDS carts, indicated by Bit31 of the ROM
Chip ID (aka bit7 of the 4th ID byte):

```

| 1) Chip ID.Bit31=0 Used by older/smaller carts with up to 64MB ROM
 2) Chip ID.Bit31=1 Used by newer/bigger carts with 64MB or more ROM |
| --- |

The first variant (for older carts) is described above. The second second
variant includes some differences for KEY1 encrypted commands:

GAPS: The commands have the same 910h-cycle gaps, but without outputting CLK
pulses during those gaps (ie. used with ROMCTRL.Bit28=0) (the absence of the
CLKs implies that there is no dummy data transferred during gaps, and
accordingly, that the KEY2 stream isn't advanced during the 910h gap cycles).

REPEATED COMMANDS and SECURE AREA DELAY: All KEY1 encrypted commands must be
sent TWICE (or even NINE times). First, send the command with 0-byte Data
transfer length. Second, issue the Secure Area Delay (required; use the delay
specified in cart header[06Eh]).

Third, send the command once again with 0-byte or 4-byte data transfer length
(usually 0 bytes, or 4-bytes for Chip ID command), or sent it eight times with
200h-byte data transfer length (for the 1000h-byte secure area load command).

For those repeats, always resend exactly the same command (namely, kkkkk is NOT
incremented during repeats, and there is no extra index needed to select
200h-byte portions within 1000h-byte blocks; the cartridge is automatically
outputting the eight portions one after another).

| DS Cartridge Backup |
| --- |

**SPI Bus Backup Memory**

```

| Type Total Size Page Size Chip/Example Game/Example
 EEPROM 0.5K bytes 16 bytes ST M95040-W (eg. Metroid Demo)
 EEPROM 8K bytes 32 bytes ST M95640-W (eg. Super Mario DS)
 EEPROM 64K bytes 128 bytes ST M95512-W (eg. Downhill Jam)
 EEPROM 128K bytes ? bytes ? (eg. Explorers of Sky)
 FLASH 256K bytes 256 bytes ST M45PE20 (eg. Skateland)
 FLASH 256K bytes Sanyo LE25FW203T (eg. Mariokart)
 FLASH 512K bytes 256 bytes ST M25PE40? (eg. which/any games?)
 FLASH 512K bytes ST 45PE40V6 (eg. DS Zelda, NTR-AZEP-0)
 FLASH 1024K bytes ST 45PE80V6 (eg. Spirit Tracks, NTR-BKIP)
 FLASH 8192K bytes MX25L6445EZNI-10G (Art Academy only, TWL-VAAV)
 FRAM 8K bytes No limit ? (eg. which/any games?)
 FRAM 32K bytes No limit Ramtron FM25L256? (eg. which/any games?) |
| --- |

**Lifetime Stats (might vary per manufacturer)**

```

| Type Max Writes per Page Data Retention
 EEPROM 100,000 40 years
 FLASH 100,000 20 years
 FRAM No limit 10 years |
| --- |

SPI Bus Backup Memory is accessed via Ports 40001A0h and 40001A2h, see

DS Cartridge I/O Ports

**Commands**

For all EEPROM and FRAM types:

```

| 06h WREN Write Enable Cmd, no parameters
 04h WRDI Write Disable Cmd, no parameters
 05h RDSR Read Status Register Cmd, read repeated status value(s)
 01h WRSR Write Status Register Cmd, write one-byte value
 9Fh RDID Read JEDEC ID (not supported on EEPROM/FLASH, returns FFh-bytes) |
| --- |

For 0.5K EEPROM (8+1bit Address):

```

| 03h RDLO Read from Memory 000h-0FFh Cmd, addr lsb, read byte(s)
 0Bh RDHI Read from Memory 100h-1FFh Cmd, addr lsb, read byte(s)
 02h WRLO Write to Memory 000h-0FFh Cmd, addr lsb, write 1..MAX byte(s)
 0Ah WRHI Write to Memory 100h-1FFh Cmd, addr lsb, write 1..MAX byte(s) |
| --- |

For 8K..64K EEPROM and for FRAM (16bit Address):

```

| 03h RD Read from Memory Cmd, addr msb,lsb, read byte(s)
 02h WR Write to Memory Cmd, addr msb,lsb, write 1..MAX byte(s) |
| --- |

For 128K EEPROM (24bit Address):

```

| As above, but with 24bit addr msb,mid,lsb ? |
| --- |

Note: MAX = Page Size (see above chip list) (no limit for FRAM).

For FLASH backup, commands should be same as for Firmware FLASH memory:

DS Firmware Serial Flash Memory

A few NDS/DSi carts are sharing the SPI bus for FLASH and Infrared, this
requires a 00h-prefix byte for FLASH access, with slower 1MHz SPI clock and
delays, see:

DS Cart Infrared/Pedometers

**Status Register**

```

| 0 WIP Write in Progress (1=Busy) (Read only) (always 0 for FRAM chips)
 1 WEL Write Enable Latch (1=Enable) (Read only, except by WREN,WRDI)
 2-3 WP Write Protect (0=None, 1=Upper quarter, 2=Upper Half, 3=All memory) |
| --- |

For 0.5K EEPROM:

```

| 4-7 ONEs Not used (all four bits are always set to "1" each) |
| --- |

For 8K..64K EEPROM and for FRAM:

```

| 4-6 ZERO Not used (all three bits are always set to "0" each)
 7 SRWD Status Register Write Disable (0=Normal, 1=Lock) (Only if /W=LOW) |
| --- |

WEL gets reset on Power-up, WRDI, WRSR, WRITE/LO/HI, and on /W=LOW.

The WRSR command allows to change ONLY the two WP bits, and the SRWD bit (if
any), these bits are non-volatile (remain intact during power-down),
respectively, the WIP bit must be checked to sense WRSR completion.

**Detection (by examining hardware responses)**

The overall memory type and bus-width can be detected by RDSR/RDID commands:

```

| RDSR RDID Type (bus-width)
 FFh, FFh,FFh,FFh None (none)
 F0h, FFh,FFh,FFh EEPROM (with 8+1bit address bus)
 00h, FFh,FFh,FFh EEPROM/FRAM (with 16bit address bus)
 ? ?,?,? EEPROM (with 24bit address bus)
 00h, xxh,xxh,xxh FLASH (usually with 24bit address bus) |
| --- |

And, the RD commands can be used to detect the memory size/mirrors (though that
won't work if the memory is empty).

**Detection (in emulators)**

Nintendo is using different functions for sending cmd+addr and data. The
bus-width can be detected by counting the bytes transferred with same program
counter after chip selection. One could also try to examine code/data in the
ROM-image (but that may envolve self-decompressing code and other obstacles).

Special cases:

Over the Hedge does initially try to access 8Kbyte EEPROM, but does actually
use 0.5Kbyte EEPROM (as workaround: re-detect the bus-width on each transfer).

Rune Factory - A Fantasy Harvest Moon seems to be also difficult to detect (as
workaround: force 64Kbyte EEPROM on gamecode ARFx).

FLASH has same 24bit bus-width as 128Kbyte EEPROM, but isn't compatible on
writing. EEPROM does use 02h=Write+Erase. FLASH does use 0Ah=Write+Erase (or
D8h/DBh=Erase and 02h=Write separatly).

**Pin-Outs for EEPROM and FRAM chips**

```

| Pin Name Expl.
 1 /S Chip Select
 2 Q Data Out
 3 /W Write-Protect (not used in NDS, wired to VCC)
 4 VSS Ground
 5 D Data In
 6 C Clock
 7 /HOLD Transfer-pause (not used in NDS, wired to VCC)
 8 VCC Supply 2.5 to 5.5V for M95xx0-W |
| --- |

FRAM (Ferroelectric Nonvolatile RAM) is fully backwards compatible with normal
EEPROMs, but comes up with faster write/erase time (no delays), and with lower
power consumption, and unlimited number of write/erase cycles. Unlike as for
normal RAM, as far as I understand, the data remains intact without needing any
battery.

**Other special save memory**

```

| DS Vision (NDS cart with microSD slot... and maybe ALSO with EEPROM?)
 NAND carts can store data in a read/write-able portion of the "ROM" chip
 Typing Adventure does have SPI FLASH (but not directly wired to SPI bus) |
| --- |

**DSi Internal eMMC and External SD Card**

DSi cartridges are usually (maybe always) having SD/MMC access disabled, so
they must stick using EEPROM/FLASH chips inside of the cartridges (which is
required for NDS compatibility anyways).

However, DSiware games (downloaded from DSi Shop) are allowed to save data on
eMMC, using "private.sav" or "public.sav" files in their data folder. The size
of that files is preset in cartridge header.

| DS Cartridge NAND |
| --- |

SLC-NAND is used in at least three NDS/DSi games. The ROM Chip ID has bit3 in
4th byte set.

```

| ECh,7Fh,00h,88h NDS Samsung 128MB NAND (eg. Warioware D.I.Y., NTR-UORE)
 ECh,7Fh,01h,88h NDS Samsung 128MB NAND (eg. Jam with the Band, NTR-UXBP)
 ECh,7Fh,00h,E8h DSi Samsung 128MB NAND (eg. Face Training, TWL-USKV) |
| --- |

In the cart header, the 1st byte of the Gamecode is "U" (that "U" is also used
for a few other carts with "uncommon" hardware), and header entries [094h,096h]
indicate the end of the ROM region and start of RW region (in 128Kbyte units
for NDS, or 512Kbyte units for DSi; exception: the oldest NAND title (japanese
version of Jam with the Band) did have [094h,096h] set to all zeroes). The
chips are all 128MByte (122MByte usable), with memory map as so:

```

| 00000000h ROM region (one large region) (R)
 0xxx0000h RW region (split into several 128KByte blocks) (R/W)
 07A00000h Reserved region (R) |
| --- |

The RW space is 8MB for Jam with the Band, 16MB for WarioWare DIY, and 82MB in
Face Training. Unknown if some of the cart memory is reserved for broken sector
handling.

**DS Cartridges with NAND memory**

NAND memory contains both the game and save memory (normal NDS games contain
separate ROM and SPI FLASH/EEPROM chips for that purposes). The advantage is
that NAND allows more storage than the usual FLASH chips. Nintendo also claims
SLC-NAND to be very fast, but that's only half true (it's much slower than
MROM, but might be slightly faster than SPI FLASH).

**Command Summary**

For whatever reason, random access is slightly restricted: One must use command
8Bh/B2h to select ROM region or a 128KByte RW window before reading/writing the
selected area. Writing is done in 2Kbyte units (4x200h bytes).

```

| In ROM access mode:
 9400000000000000h Len=200h NAND Read ID
 B2aaaaaaaa000000h Len=0 NAND Select 128Kbyte RW access mode
 B300000000000000h Len=04h Unknown (returns 00000000h)
 BB00000000000000h Len=200h Unknown (returns 1X 04 09 20 04, plus zeroes)
 In RW access mode (on DSi carts, this works ONLY in DSi mode):
 81aaaaaaaa000000h Len=200h NAND Write to Write Buffer (must be issued 4x)
 8200000000000000h Len=0 NAND Forward Write Buffer to NAND
 8400000000000000h Len=0 NAND Discard Write Buffer
 8500000000000000h Len=0 NAND Write Enable
 8600000000000000h Len=0 Unknown
 8700000000000000h Len=0 NAND Write Disable
 8B00000000000000h Len=0 NAND Select ROM access mode
 In either mode:
 0B00000000000000h Len=200h Returns cart header[000h..1FFh]
 0C00000000000000h Len=200h Returns corrupted cart header[1F8h..3F7h] ??
 58h..5Fh Len=0 Unknown (looks same/similar as in 1T-ROM carts)
 60h..68h Len=800h Unknown (looks same/similar as in 1T-ROM carts)
 B000000000000000h Len=04h Unknown (returns 01010101h)
 B500000000000000h Len=0 Unknown (looks same/similar as in SanDisk carts)
 B7aaaaaaaa000000h Len=200h NAND Read from ROM or RW area
 B800000000000000h Len=04h Read Chip ID
 D600000000000000h Len=04h NAND Read Status
 Further command(s) spotted in Face Training disassembly:
 8800000000000000h Len=0 Unknown (is in disassembly, but fails on HW?) |
| --- |

**81aaaaaaaa000000h - NAND Write to Write Buffer + Data[200h]**

The command must be issued 4 times with the same address (seems to use the
address from the first command), a full write is 800h bytes (4x200h), the chip
won't respond to other commands until all 4 commands are sent. Unknown what
happens when trying to send more than 4 commands.

This command doesn't directly write to NAND, instead it stores the incoming
data in a 800h-byte write buffer (latter committed using command 82h).

The provided address needs to be within the access window specified by command
B2h.

**8200000000000000h - NAND Forward Write Buffer to NAND**

The data in the 800h-byte write buffer is written to the actual NAND, and the
write-enable bit in the status register is cleared.

**8400000000000000h - NAND Discard Write Buffer**

The data in the write buffer is discarded (a subsequent command 82h would have
no effect).

Games seem to always use this after command 82h, so maybe it's required even
after a write.

**8500000000000000h - NAND Write Enable**

The write-enable bit in the status register is set.

Only works in RW access mode.

**8B00000000000000h - NAND Select ROM access mode**

Switch to ROM access mode.

**B2aaaaaaaa000000h - NAND Select 128Kbyte RW access mode**

The provided address defines an accessible 128K window in RW address space,
reads and writes may only take place within that window, the lower 17 bits of
the address are ignored.

Addresses below start of RW area are ignored (the cart stays in ROM mode, and
the cart does then reportedly get stuck permanently busy).

Addresses that go past the end of the RW address space will just read all FF's.

**B7aaaaaaaa000000h - NAND Read from ROM or RW area + Data[200h]**

This command is used for reading data under both ROM and RW modes.

In ROM mode: returns data from the ROM space, pretty much like the 'regular' B7
command, trying to read from RW space in this mode will return all FF's.

In RW mode: returns data from the RW space, the provided address needs to be
within the access window specified by command B2h.

**D600000000000000h - NAND Read Status + Data[4]**

Status register bits:

```

| 0-1 Unknown (usually zero)
 2-3 Unknown (usually zero, but tested by DSi Launcher, not NAND related?)
 4 NAND write enable
 5 NAND status (0=busy, 1=ready)
 6 Unknown (usually zero, but set by DeSmuME)
 7 Unknown (possible error flag?)
 8-15 Same as bit0-7
 16-23 Same as bit0-7
 24-31 Same as bit0-7 |
| --- |

Value on startup is 20h (aka 20202020h when reading 4 bytes).

**BB00000000000000h Len=200h Unknown (returns 1X 04 09 20 04, plus zeroes)**

```

| Values in Jam with the Band (nocash dump): 17 04 09 20 04, plus 1FBh zeroes
 Values in Face Training (nocash dump): 10 04 09 20 04, plus 1FBh zeroes |
| --- |

**9400000000000000h - NAND Read ID + Data[200h]**

Returns NAND ID data of sorts.

```

| Values in Jam with the Band (arisotura dump):
 000h EC F1 00 95 40 00 00 00 00 00 00 00 00 00 00 00 ....@...........
 010h 00 00 00 00 00 00 00 00 EC 00 9E A1 51 65 34 35 ............Qe45
 020h 30 35 30 31 19 19 02 0A 00 00 00 00 00 00 00 00 0501............
 030h FF FF FF .. (1D0h bytes) (why not 00h's ???) ................
 Values in Jam with the Band (nocash dump):
 000h EC F1 00 95 40 00 00 00 00 00 00 00 00 00 00 00 ....@...........
 010h 00 00 00 00 00 00 00 00 EC 00 3B 5A 32 9B 32 30 ..........;Z2.20
 020h 35 35 30 30 19 19 02 0A 00 00 00 00 00 00 00 00 5500............
 030h 00 00 00 .. (1D0h bytes) ................
 Values in Face Training (nocash dump):
 000h EC F1 00 95 40 00 00 00 00 00 00 00 00 00 00 00 ....@...........
 010h 00 00 00 00 00 00 00 00 EC 00 5A 36 5C 14 35 35 ..........Z6\.55
 020h 32 36 30 36 04 04 08 0A 00 00 00 00 00 00 00 00 2606............
 030h 00 00 00 .. (1D0h bytes) ................ |
| --- |

For Jam with Band, the 16 bytes at 018h are also found in the last 800h bytes
of the RW space (see below). The bytes with value ECh might be related to the
Maker ID in Chip ID.

**Last 128Kbyte in RW region (at 079E0000h..079FFFFFh)**

The last 128Kbyte of RW area is normal write-able memory in Face Training
without special content. However, in Jam with the Band, the last 128KByte are
readonly, they can be read like normal RW blocks (select via command B2h, then
read via B7h), but writing isn't supported (although NAND Status bit5 gets
cleared for a moment upon write attempts to this 128Kbyte area; unlike as for
when trying to write 7A00000h and up).

```

| Values in Jam with the Band (arisotura dump):
 079E0000h FF FF FF .. (1F800h bytes) ................
 079FF800h EC 00 9E A1 51 65 34 35 30 35 30 31 19 19 02 0A ....Qe450501....
 079FF810h 00 00 00 00 6D D6 DA 9B B0 24 22 88 79 3B BF EA ....m....$".y;..
 079FF820h E6 AC 5E FA 69 12 0D 52 5D 5B F5 80 FF FF FF FF ..^.i..R][......
 079FF830h FF FF FF .. (7D0h bytes) ................
 Values in Jam with the Band (nocash dump):
 079E0000h FF FF FF .. (1F800h bytes) ................
 079FF800h EC 00 3B 5A 32 9B 32 30 35 35 30 30 19 19 02 0A ..;Z2.205500....
 079FF810h 00 00 00 00 DD 58 84 07 F9 72 19 04 96 8C FF 67 .....X...r.....g
 079FF820h 7F 66 B9 E5 FD F7 3F 1A AE 60 60 00 FF FF FF FF .f....?..``.....
 079FF830h FF FF FF .. (7D0h bytes) ................ |
| --- |

Jam with the Band checks the bytes at 079FF800h (unknown what it does with
them, or what it does when they are missing).

**Reserved Area (at 07A00000h and up) (including 08000000h and up)**

This memory isn't intended to be used, in Jam with the Band it just returns
FFh's. However, in Face Training it does return some interesting internal data
(in RW mode):

The DSi Blowfish key (with the gamecode pre-applied), some NDS ARM code (for
the secure area or so), and most interestingly some more ARM+THUMB code
(apparently containing the firmware running on an ARM CPU inside of the game
cartridge itself).

Unfortunately, that internal memory is returned as "raw" data with faulty bits,
and it seems to be required to apply error correction to convert it to actual
intact data (the "raw" stuff consists of 1E8h-byte data snippets, usually
followed by 8-byte ECC info, or sometimes 16-byte or 20-byte ECC, or no ECC at
all). There are several "backup" copies of the blowfish/firmware blocks.

**Write Example**

```

| B2aaaaaaaa000000h - Select 128Kbyte RW access mode (unlesss already)
 8500000000000000h - NAND Write Enable
 81aaaaaaaa000000h - NAND Write to Write Buffer + Data[200h]
 81aaaaaaaa000000h - NAND Write to Write Buffer + Data[200h]
 81aaaaaaaa000000h - NAND Write to Write Buffer + Data[200h]
 81aaaaaaaa000000h - NAND Write to Write Buffer + Data[200h]
 8200000000000000h - NAND Forward Write Buffer to NAND
 D600000000000000h - NAND Read Status + Data[4]
 (...repeat reading status until bit5=1=ready...)
 8400000000000000h - NAND Discard SRAM write
 8B00000000000000h - NAND Select ROM access mode (if desired) |
| --- |

**Chipsets**

Warioware D.I.Y., NTR-UORE:

```

| PCB "DI X-7 C17-01"
 Chip "SAMSUNG 004, KLC2811ANB-P204, NTR-UORE-0" |
| --- |

Jam with the Band, NTR-UXBP:

```

| PCB (Unknown)
 Chip "SAMSUNG 013, KLC2811UOC-P30A, NTR-UXBP-0, WKA069J2" |
| --- |

Face Training, TWL-USKV:

```

| PCB "DI X-8 C17-01"
 U1 "SAMSUNG 031, KLC2811UOC-P309, TWL-USKV-0, WKE114(80?)"
 (this chip must be slightly different, for DSi mode support) |
| --- |

There are also a bunch of 3DS games with similar chips (with the RW area being
called "CARD2" area on 3DS, and, the 3DS is using 16-byte commands, so the
protocol is different).

**Notes**

Unlike ROM carts, the NAND cart does crash upon invalid commands (and stops to
respond to further commands). That is, upon invalid command numbers, upon ROM
mode commands in RW mode (or vice-versa), upon any data lengths other than
listed above.

| DS Cartridge I/O Ports |
| --- |

The Gamecard bus registers can be mapped to NDS7 or NDS9 via EXMEMCNT, see

DS Memory Control

**40001A0h - NDS7/NDS9 - AUXSPICNT - Gamecard ROM and SPI Control (R/W)**

```

| 0-1 SPI Baudrate (0=4MHz/Default, 1=2MHz, 2=1MHz, 3=512KHz)
 2-5 Not used (always zero)
 6 SPI Hold Chipselect (0=Deselect after transfer, 1=Keep selected)
 7 SPI Busy (0=Ready, 1=Busy) (presumably Read-only)
 8-12 Not used (always zero)
 13 NDS Slot Mode (0=Parallel/ROM, 1=Serial/SPI-Backup)
 14 Transfer Ready IRQ (0=Disable, 1=Enable) (for ROM, not for AUXSPI)
 15 NDS Slot Enable (0=Disable, 1=Enable) (for both ROM and AUXSPI) |
| --- |

The "Hold" flag should be cleared BEFORE transferring the LAST data unit, the
chipselect will be then automatically cleared after the transfer, the program
should issue a WaitByLoop(12) on NDS7 (or longer on NDS9) manually AFTER the
LAST transfer.

**40001A2h - NDS7/NDS9 - AUXSPIDATA - Gamecard SPI Bus Data/Strobe (R/W)**

The SPI transfer is started on writing to this register, so one must
<write> a dummy value (should be zero) even when intending to
<read> from SPI bus.

```

| 0-7 Data
 8-15 Not used (always zero) |
| --- |

During transfer, the Busy flag in AUXSPICNT is set, and the written DATA value
is transferred to the device (via output line), simultaneously data is received
(via input line). Upon transfer completion, the Busy flag goes off, and the
received value can be then read from AUXSPIDATA, if desired.

**40001A4h - NDS7/NDS9 - ROMCTRL - Gamecard Bus ROMCTRL (R/W)**

```

| 0-12 KEY1 gap1 length (0-1FFFh) (forced min 08F8h by BIOS) (leading gap)
 13 KEY2 encrypt data (0=Disable, 1=Enable KEY2 Encryption for Data)
 14 "SE" Unknown? (usually same as Bit13) (does NOT affect timing?)
 15 KEY2 Apply Seed (0=No change, 1=Apply Encryption Seed) (Write only)
 16-21 KEY1 gap2 length (0-3Fh) (forced min 18h by BIOS) (200h-byte gap)
 22 KEY2 encrypt cmd (0=Disable, 1=Enable KEY2 Encryption for Commands)
 23 Data-Word Status (0=Busy, 1=Ready/DRQ) (Read-only)
 24-26 Data Block size (0=None, 1..6=100h SHL (1..6) bytes, 7=4 bytes)
 27 Transfer CLK rate (0=6.7MHz=33.51MHz/5, 1=4.2MHz=33.51MHz/8)
 28 KEY1 Gap CLKs (0=Hold CLK High during gaps, 1=Output Dummy CLK Pulses)
 29 RESB Release Reset (0=Reset, 1=Release) (cannot be cleared once set)
 30 Data Direction "WR" (0=Normal/read, 1=Write, for FLASH/NAND carts)
 31 Block Start/Status (0=Ready, 1=Start/Busy) (IRQ See 40001A0h/Bit14) |
| --- |

The cartridge header is booted at 4.2MHz CLK rate, and following transfers are
then using ROMCTRL settings specified in cartridge header entries [060h] and
[064h], which are usually using 6.7MHz CLK rate for the main data transfer
phase (whereof, older MROM carts can actually transfer 6.7Mbyte/s, but newer
1T-ROM carts default to reading 200h-byte blocks with gap1=657h, thus reaching
only 1.6Mbyte/s).

Transfer length of null, four, and 200h..4000h bytes are supported by the
console, however, retail cartridges cannot cross 1000h-byte boundaries (and,
SanDisk ROM chips and Samsung NAND chips cannot transfer more than 200h bytes).

**Default cart header entries**

```

| hdr[60h] hdr[64h] hdr[6Eh]
 00586000h 001808F8h 051Eh ;older/faster MROM
 00416657h 081808F8h 0D7Eh ;newer/slower 1T-ROM
 00416657h 081808F8h 0D7Eh ;newer/slower NAND |
| --- |

**Older/Faster MROM**

The romctrl values in cartheader[60h,64h] are okay, but the secure delay in
[6Eh] is nonsense (should be zero).

**Misdeclared MROM**

Some carts like SystemFlaw and BiggestLoser are actually containing MROM chips
despite of having 1T-ROM values in cart header (gap1=657h is making loading
insane slow, gap2=01h causes errors on 1000h-byte blocks, and secure.clk=4.2MHz
is slowing down secure area loading, combined with even slower secure area
delay despite of not needing any delay for MROM).

As the cart header entries are wrong, some other detection is needed: This can
be probably done by checking ChipID.bit31 (or otherwise by testing if
1000h-block reading works with gap1=01h, if so, then it's 1T-ROM).

**Newer/Slower 1T-ROM**

Actual 1T-ROM carts can be very slow, especially when using the insane cart
header values and default firmware blocksize of 200h bytes which drops loading
speed from 6.7Mbytes/s to 1.6Mbyte/s (as workaround, use gap1=180h,
blocksize=1000h, also secure area delay should be 400h, not D7Eh)
(tested/working for CookingCoach, unknown if that timings work for all other
carts).

**Superslow Writeable NAND**

This is having the same official insane delays as 1T-ROM, but with less
tolerance for faster timings (fastest working values are gap1=380h,
blocksize=200h, secure delay 400h, plus 20ms delay after releasing reset).

**Cart Reset**

Reset flag in bit29 can be set once only (to release reset), the only way to
clear the bit is power-off. However, there are some ways to issue resets:

1) On NDS: Manually eject/insert the cart (that won't affect bit29, but the
cart will reset itself anyways upon power loss) (eject on DSi will power-off
the cart slot).

2) If one of the two ROMCTRL registers (on ARM7 and ARM9) is still zero:
Temporarily toggle ARM7/ARM9 cart access via EXMEMCNT on ARM9 side.

3) On DSi: If the 2nd cart slot ROMCTRL register (40021A4h) is still zero:
Temporarily swap 1ns/2nd cart slot via SCFG_MC.bit15 on ARM7 side.

4) On DSi: Use SCFG_MC to toggle cart power off/on; this will actually reset
bit29, the DSi firmware is actually using that method, but it's very slow
(takes about 300ms, for the power-off wait, plus (unneccassary) hardcoded
power-on delays).

**40001A8h - NDS7/NDS9 - Gamecard bus 8-byte Command Out**

The separate commands are described in the Cartridge Protocol chapter, however,
once when the BIOS boot procedure has completed, one would usually only need
command "B7aaaaaaaa000000h", for reading data (usually 200h bytes) from address
aaaaaaaah (which should be usually aligned by 200h).

```

| 0-7 1st Command Byte (at 40001A8h) (eg. B7h) (MSB)
 8-15 2nd Command Byte (at 40001A9h) (eg. addr bit 24-31)
 16-23 3rd Command Byte (at 40001AAh) (eg. addr bit 16-23)
 24-31 4th Command Byte (at 40001ABh) (eg. addr bit 8-15) (when aligned=even)
 32-39 5th Command Byte (at 40001ACh) (eg. addr bit 0-7) (when aligned=00h)
 40-47 6th Command Byte (at 40001ADh) (eg. 00h)
 48-57 7th Command Byte (at 40001AEh) (eg. 00h)
 56-63 8th Command Byte (at 40001AFh) (eg. 00h) (LSB) |
| --- |

Observe that the command/parameter MSB is located at the smallest memory
location (40001A8h), ie. compared with the CPU, the byte-order is reversed.

**4100010h - NDS7/NDS9 - Gamecard bus 4-byte Data In (R) (or W)**

```

| 0-7 1st received Data Byte (at 4100010h)
 8-15 2nd received Data Byte (at 4100011h)
 16-23 3rd received Data Byte (at 4100012h)
 24-31 4th received Data Byte (at 4100013h) |
| --- |

After sending a command, data can be read from this register manually (when the
DRQ bit is set), or by DMA (with DMASAD=4100010h, Fixed Source Address,
Length=1, Size=32bit, Repeat=On, Mode=DS Gamecard).

**40001B0h - 32bit - NDS7/NDS9 - Encryption Seed 0 Lower 32bit (W)**

**40001B4h - 32bit - NDS7/NDS9 - Encryption Seed 1 Lower 32bit (W)**

**40001B8h - 16bit - NDS7/NDS9 - Encryption Seed 0 Upper 7bit (bit7-15 unused)**

**40001BAh - 16bit - NDS7/NDS9 - Encryption Seed 1 Upper 7bit (bit7-15 unused)**

These registers are used by the NDS7 BIOS to initialize KEY2 encryption (and
there's normally no need to change that initial settings). Writes to the Seed
registers do not have direct effect on the internal encryption registers, until
the Seed gets applied by writing "1" to ROMCTRL.Bit15.

```

| For more info: |
| --- |

DS Encryption by Random Seed (KEY2)

Note: There are <separate> Seed registers for both NDS7 and NDS9, which
can be applied by ROMCTRL on NDS7 and NDS9 respectively (however, once when
applied to the internal registers, the new internal setting is used for
<both> CPUs).

| DS Cartridge NitroROM and NitroARC File Systems |
| --- |

The DS hardware, BIOS, and Firmware do NOT contain any built-in filesystem
functions. The ARM9/ARM7 boot code (together max 3903KB), and Icon/Title
information are automatically loaded on power-up.

Programs that require to load additional data from cartridge ROM may do that
either by implementing whatever functions to translate filenames to ROM
addresses, or by reading from ROM directly.

**NitroROM**

The NitroROM Filesystem is used by many NDS games (at least those that have
been developed with Nintendo's tools). It's used for ROM Cartridges, and, on
the DSi, it's also used for DSiWare games (in the latter case, NitroROM acts as
a 2nd virtual filesystem inside of the DSi's FAT16 filesystem).

```

| FNT = cart_hdr[040h] ;\origin as defined in ROM cartridge header
 FAT = cart_hdr[048h] ;/
 IMG = 00000000h ;-origin at begin of ROM |
| --- |

Aside from using filenames, NitroROM files can be alternately accessed via
Overlay IDs (see later on below).

**NitroARC (Nitro Archive)**

NARC Files are often found inside of NitroROM Filesystems (ie. NARC is a second
virtual filesystem, nested inside of the actual filesystem). The NARC Format is
very similar to the NitroROM Format, but with additional Chunk Headers (instead
of the Cartridge ROM Header).

```

| ... ... Optional Header (eg. compression header, or RSA signature)
 000h 4 Chunk Name "NARC" (Nitro Archive) ;\
 004h 2 Byte Order (FFFEh) (unlike usually, not FEFFh) ;
 006h 2 Version (0100h) ; NARC
 008h 4 File Size (from "NARC" ID to end of file) ; Header
 00Ch 2 Chunk Size (0010h) ;
 00Eh 2 Number of following chunks (0003h) ;/
 010h 4 Chunk Name "BTAF" (File Allocation Table Block) ;\
 014h 4 Chunk Size (including above chunk name) ; File
 018h 2 Number of Files ; Allocation
 01Ah 2 Reserved (0000h) ; Table
 01Ch ... FAT (see below) ;/
 ... 4 Chunk Name "BTNF" (File Name Table Block) ;\
 ... 4 Chunk Size (including above chunk name) ; File Name
 ... ... FNT (see below) ; Table
 ... .. Padding for 4-byte alignment (FFh-filled, if any) ;/
 ... 4 Chunk Name "GMIF" (File Image Block) ;\
 ... 4 Chunk Size (including above chunk name) ; File Data
 ... ... IMG (File Data) ;/ |
| --- |

**NARCless variant**

There are a few NARC archives with crippled header, without "NARC" string (eg.
rom:\dwc\utility.bin in Over the Hedge, Downhill Jam, and Tony Hawk's
Skateland).

```

| 000h 4 FNT Filename Table Offset (always at 10h)
 004h 4 FNT Filename Table Size
 008h 4 FAT Allocaton Table Offset (at above Offset+Size+Padding)
 00Ch 4 FAT Allocaton Table Size
 010h .. FNT Filename Table Data
 ... .. FAT Allocaton Table Data
 ... .. IMG File Data |
| --- |

The offsets in FAT are relative to IMG=0 (as if IMG would start at begin of
file).

**File Allocation Table (FAT) (base/size defined in cart header)**

Contains ROM addresses for up to 61440 files (File IDs 0000h and up).

```

| Addr Size Expl.
 00h 4 Start address (originated at IMG base) (0=Unused Entry)
 04h 4 End address (Start+Len) (0=Unused Entry) |
| --- |

For NitroROM, addresses must be after Secure Area (at 8000h and up).

For NitroARC, addresses can be anywhere in the IMG area (at 0 and up).

Directories are fully defined in FNT area, and do not require FAT entries.

**File Name Table (FNT) (base/size defined in cart header)**

Consists of the FNT Directory Table, followed by one or more FNT Sub-Tables.

To interprete the directory tree: Start at the 1st Main-Table entry, which is
referencing to a Sub-Table, any directories in the Sub-Table are referencing to
Main-Table entries, which are referencing to further Sub-Tables, and so on.

**FNT Directory Main-Table (base=FNT+0, size=[FNT+06h]*8)**

Consists of a list of up to 4096 directories (Directory IDs F000h and up).

```

| Addr Size Expl.
 00h 4 Offset to Sub-table (originated at FNT base)
 04h 2 ID of first file in Sub-table (0000h..EFFFh) |
| --- |

For first entry (ID F000h, root directory):

```

| 06h 2 Total Number of directories (1..4096) |
| --- |

Further entries (ID F001h..FFFFh, sub-directories):

```

| 06h 2 ID of parent directory (F000h..FFFEh) |
| --- |

**FNT Sub-tables (base=FNT+offset, ends at Type/Length=00h)**

Contains ASCII names for all files and sub-directories within a directory.

```

| Addr Size Expl.
 00h 1 Type/Length
 01h..7Fh File Entry (Length=1..127, without ID field)
 81h..FFh Sub-Directory Entry (Length=1..127, plus ID field)
 00h End of Sub-Table
 80h Reserved
 01h LEN File or Sub-Directory Name, case-sensitive, without any ending
 zero, ASCII 20h..7Eh, except for characters \/?"<>*:;| |
| --- |

Below for Sub-Directory Entries only:

```

| LEN+1 2 Sub-Directory ID (F001h..FFFFh) ;see FNT+(ID AND FFFh)*8 |
| --- |

File Entries do not have above ID field. Instead, File IDs are assigned in
incrementing order (starting at the "First ID" value specified in the Directory
Table).

**ARM9 and ARM7 Overlay Tables (OVT) (base/size defined in cart header)**

Somehow related to Nintendo's compiler, allows to assign compiler Overlay IDs
to filesystem File IDs, and to define additional information such like load
addresses.

```

| Addr Size Expl.
 00h 4 Overlay ID
 04h 4 RAM Address ;Point at which to load
 08h 4 RAM Size ;Amount to load
 0Ch 4 BSS Size ;Size of BSS data region
 10h 4 Static initialiser start address
 14h 4 Static initialiser end address
 18h 4 File ID (0000h..EFFFh)
 1Ch 4 Reserved (zero) |
| --- |

**Cartridge Header**

The base/size of FAT, FNT, OVT areas is defined in cartridge header,

DS Cartridge Header

| DS Cartridge Unknown Commands |
| --- |

The Main Data transfer mode is normally using only two commands (B7h/B8h),
however most cartridges do support one or more undocumented commands (as
opposed to invalid commands, which will cause the cart to stop responding).

```

| Title Chip ID Commands...
 Metroid First Hunt 00000FC2 B7 B8 D8
 Meine Tierarztpraxis 00000FAE B7 B8 D8
 Meine Tierpension 00000FC2 B7 B8 D8
 Nanostray 00000FC2 B7 B8 D8
 Over the Hedge 00001FC2 B7 B8 D8
 Tony Hawk's Skateland 00003FC2 B7 B8
 Tony Hawk's Downhill Jam 00003FC2 B7 B8
 Ultimate Spiderman 00003FC2 B7 B8
 System Flaw (DSi) 40001FC2 B7 B8 F1
 Biggest Loser (DSi) 40001FC2 B7 B8 F1
 Cooking Coach (DSi) C0007FC2 58..5F 60..68 B7 B8
 Walk with Me E0013F80 69..6C B5 B7 B8 D6
 Face Training (DSI NAND) E8007FEC 0x 5x 6x 8x 94 Bx D6 (see NAND chapter) |
| --- |

The presence of those commands was tested on DSi (where one can reset the cart
by software to recover from invalid commands), with all parameter bits set to
zero.

That testing is more difficult on NAND carts because the commands do only work
when transferring the correct number of data bytes (0, 4, 200h, or 800h bytes),
and only when being in the correct mode (ROM or RW mode; whereof, on DSi carts,
the RW mode works ONLY in DSi mode).

```

| ______________________________ Command 58h..68h ______________________________ |
| --- |

**5800000000000000h-5F00000000000000h (..) - Get HighZ**

Unknown purpose, returns HighZ.

**6000000000000000h-6800000000000000h (..) - Get Zeroes**

Unknown purpose, returns at least 4000h encrypted 00h bytes.

```

| ______________________________ Command 69h..6Ch ______________________________ |
| --- |

**6900000000000000h-6C00000000000000h (..) - Get Zeroes**

Unknown purpose, returns at least 4000h encrypted 00h bytes.

```

| ______________________________ Command B5h/D6h _______________________________ |
| --- |

**B500000000000000h (0) - Start/Stop/Reset/Ack something?**

Unknown. Returns only HighZ.

**D600000000000000h (4) - Get Status Byte (also used on NAND carts)**

Returns a status byte (repeated in first four bytes, then followed by HighZ
bytes).

Command B5h/D6h are supported in carts that have bit5 set in 4th byte of Chip
ID (eg. in Walk with Me). Command D6h (and maybe also B5h) works both on power
up (before secure area), and in main data phase (after secure area). The DSi
Launcher contains following code to deal with such carts:

```

| if chip_id AND 20000000h
 get_nand_status(cmd_D6h)
 if (nand_status AND 0Ch)<>0 ;whatever bits
 whatever(cmd_B5h) ;whatever command
 loop:
 get_nand_status(cmd_D6h)
 if (nand_status AND 20h)=0 then goto loop ;wait for ready flag |
| --- |

```

| ________________________________ Command D8h _________________________________ |
| --- |

**D800000000000000h (..) - Get Zeroes**

Unknown purpose, returns at least 4000h encrypted 00h bytes.

```

| ________________________________ Command F1h _________________________________ |
| --- |

**F100000000000000h (1000h) - Get Serial/Manufacturer Info or so**

Returns 1000h bytes of data (repeated each 1000h bytes), the data is mostly
FFh-filled, with some kind of Serial Number or Manufacturer Info at offset
E00h. Unknown what those values are good for, and if they contain unique IDs.

This command works in main data phase only (returns only FFh/HighZ on power up
before secure area loading).

System Flaw cmd F1 response:

```

| 0000..0DFF FF-filled
 0E00 1E 40 05 5A FF FF 0D 01 32 68 38 7A 23 3F FF FF
 0E10 03 0B 00 00 03 09 FF FF FF FF FF FF FF FF FF FF
 0E20 1E 40 05 03 0B 00 00 03 09 00 00 FF FF FF FF FF
 0E30 FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF
 0E40 FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF
 0E50 FF FF FF FF FF 5A FF 5E FF FF FF FF FF FF 5A FF
 0E60 FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF
 0E70 FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF
 0E80..0FFF FF-filled
 1000..3FFF mirrors of 0000-0FFF |
| --- |

Biggest Loser cmd F1 response:

```

| 0000..0DFF FF-filled
 0E00 11 16 08 5A FF FF 0D 0B 39 7C 40 8E 2A 53 FF FF
 0E10 03 0A 07 05 05 04 00 00 07 00 7F FF 00 FF FF FF
 0E20 FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF
 0E30 FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF
 0E40 FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF
 0E50 FF FF FF FF FF 5A FF 5E FF FF FF FF FF FF 5A FF
 0E60 FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF
 0E70 FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF
 0E80..0FFF FF-filled
 1000..3FFF mirrors of 0000-0FFF |
| --- |

| DS Cartridge PassMe/PassThrough |
| --- |

PassMe is an adapter connected between the DS and an original NDS cartridge,
used to boot unencrypted code from a flash cartridge in the GBA slot, it
replaces the following entries in the original NDS cartridge header:

```

| Addr Siz Patch
 004h 4 E59FF018h ;opcode LDR PC,[027FFE24h] at 27FFE04h
 01Fh 1 04h ;set autostart bit
 022h 1 01h ;set ARM9 rom offset to nn01nnnnh (above secure area)
 024h 4 027FFE04h ;patch ARM9 entry address to endless loop
 034h 4 080000C0h ;patch ARM7 entry address in GBA slot
 15Eh 2 nnnnh ;adjust header crc16 |
| --- |

After having verified the encrypted chip IDs (from the original cartridge), the
console thinks that it has successfully loaded a NDS cartridge, and then jumps
to the (patched) entrypoints.

**GBA Flashcard Format**

Although the original PassMe requires only the entrypoint, PassMe programs
should additionally contain one (or both) of the ID values below, allowing
firmware patches to identify & start PassMe games without real PassMe
hardware.

```

| 0A0h GBA-style Title ("DSBooter")
 0ACh GBA-style Gamecode ("PASS")
 0C0h ARM7 Entrypoint (32bit ARM code) |
| --- |

Of course, that applies only to early homebrew programs, newer games should use
normal NDS cartridge headers.

**ARM9 Entrypoint**

The GBA-slot access rights in the EXMEMCNT register are initially assigned to
the ARM7 CPU, so the ARM9 cannot boot from the flashcard, instead it is
switched into an endless loop in Main RAM (which contains a copy of the
cartridge header at 27FFE00h and up). The ARM7 must thus copy ARM9 code to Main
RAM, and then set the ARM9 entry address by writing to [027FFE24h].

| DS Cartridge GBA Slot |
| --- |

Aside from the 17-pin NDS slot, the DS also includes a 32-pin GBA slot. This
slot is used for GBA backwards compatibility mode. Additionally, in DS mode, it
can be as expansion port, or for importing data from GBA games.

```

| NDS: Normal 32pin slot
 DS Lite: Short 32pin slot (GBA cards stick out)
 DSi: N/A (dropped support for GBA carts, and for DS-expansions) |
| --- |

In DS mode, ROM, SRAM, FLASH backup, and whatever peripherals contained in
older GBA cartridges can be accessed (almost) identically as in GBA mode,

GBA Cartridges

**Addressing**

In DS mode, only one ROM-region is present at 8000000h-9FFFFFFh (ie. without
the GBA's mirrored WS1 and WS2 regions at A000000h-DFFFFFFh). The expansion
region (for SRAM/FLASH/etc) has been moved from E000000h-E00FFFFh (GBA-mode) to
A000000h-A00FFFFh (DS-mode).

**Timings**

GBA timings are specified as "waitstates" (excluding 1 access cycle), NDS
timings are specified as (total) "access time". And, the NDS bus-clock is twice
as fast as for GBA. So, for "N" GBA waitstates, the NDS access time should be
"(N+1)*2". Timings are controlled via NDS EXMEMCNT instead GBA WAITCNT,

DS Memory Control - Cartridges and Main RAM

**GBA EEPROM Timings**

EEPROMs in GBA carts can be accessed via DMA in DS mode, although the timings
are a bit too fast. The access time should be 18 clks for 1st/2nd access, but
the NDS supports only 4 or 6 clks for 2nd access; of which, 6 clks seems to
work stable (and 4 clks doesn't work, it seems to ignore writes and returns
garnage on reading).

| DS Cart Rumble Pak |
| --- |

**DS Rumble Option Pak**

The Rumble Pak comes bundled with Metroid Prime Pinball. It contains a small
actuator made by ALPS to make it rumble. The original device (NTR-008) is sized
like a normal GBA cartridge, and there's also shorter variant for the DS-Lite
(USG-006).

The rumble pak is pretty simple internally, it only wires up to a few pins on
the GBA Cartridge Port:

```

| VCC, GND, /WR, AD1, and IRQ (grounded) |
| --- |

AD1 runs into a little 8 pin chip, which is probably just a latch on the rising
edge of /WR. A line runs from this chip to a transistor that is directly
connected to the actuator. The only other chip on the board is a 5 pin jobber,
probably a power component.

For detection, AD1 seems to be pulled low when reading from it, the other AD
lines are open bus (containing the halfword address), so one can do:

```

| for i=0 to 0FFFh
 if halfword[8000000h+i*2]<>(i and FFFDh) then <not_a_ds_rumble_pak>
 next i |
| --- |

The actuator doesn't have an on/off setting like a motor, it rumbles when you
switch it between the two settings. Switch frequently for a fast rumble, and
fairly slowly for more of a 'tick' effect. That should be done via timer irq:

```

| rumble_state = rumble_state xor 0002h
 halfword[8000000h]=rumble_state |
| --- |

Unknown if one of the two states has higher power-consumption than the other,
ie. if it's a "pull/release" mechanism, if so, then disabling rumble should be
done by using the "release" state, which would be AD1=0, or AD1=1...?

Note: The v3 firmware can detect the Rumble Pak as an option pak, but it does
not provide an enable/disable rumble option in the alarm menu.

**Other DS Rumble device**

There's also another DS add-on with rumble. That device uses AD8 (instead AD1)
to control rumble, and, it's using a classic motor (ie. it's rumbling while and
as long as the latched AD8 value is "1").

DS Cart Slider with Rumble

**GBA Rumble Carts**

There are also a few GBA games that contain built-in Rumble, and which could be
used by NDS games as well. To be user friendly, best support both types.

GBA Cart Rumble

| DS Cart Slider with Rumble |
| --- |

Add-on device for the japanese title Magukiddo. The optical sensor is attached
underneath of the console (connected to the GBA slot).

The sensor is an Agilent ADNS-2030 Low Power Optical Mouse Sensor (16pin DIP
chip with built-in optical sensor, and external LED light source) with two-wire
serial bus (CLK and DTA).

**ADNS-2030 Registers (write 1 byte index, then read/write 1 byte data)**

Index (Bit7=Direction; 0=Read, 1=Write):

```

| 00h Product_ID (R) (03h)
 01h Revision_ID (R) (10h=Rev. 1.0) (20h=Used in DS-option-pak)
 02h Motion/Status Flags (R)
 03h Delta_X (R) (signed 8bit) (automatically reset to 00h after reading)
 04h Delta_Y (R) (signed 8bit) (automatically reset to 00h after reading)
 05h SQUAL (R) (surface quality) (unsigned 8bit)
 06h Average_Pixel (R) (unsigned 6bit, upper 2bit unused)
 07h Maximum_Pixel (R) (unsigned 6bit, upper 2bit unused)
 08h Reserved
 09h Reserved
 0Ah Configuration_bits (R/W)
 0Bh Reserved
 0Ch Data_Out_Lower (R)
 0Dh Data_Out_Upper (R)
 0Eh Shutter_Lower (R)
 0Fh Shutter_Upper (R)
 10h Frame_Period_Lower (R/W)
 11h Frame_Period_Upper (R/W) |
| --- |

Motion/Status Flags:

```

| 7 Motion since last report or PD (0=None, 1=Motion occurred)
 6 Reserved
 5 LED Fault detected (0=No fault, 1=Fault detected)
 4 Delta Y Overflow (0=No overflow, 1=Overflow occured)
 3 Delta X Overflow (0=No overflow, 1=Overflow occured)
 2 Reserved
 1 Reserved
 0 Resolution in counts per inch (0=400, 1=800) |
| --- |

Configuration_bits:

```

| 7 Reset Power up defaults (W) (0=No, 1=Reset)
 6 LED Shutter Mode (0=LED always on, 1=LED only on when shutter is open)
 5 Self Test (W) (0=No, 1=Perform all self tests)
 4 Resolution in counts per inch (0=400, 1=800)
 3 Dump 16x16 Pixel bitmap (0=No, 1=Dump via Data_Out ports)
 2 Reserved
 1 Reserved
 0 Sleep Mode (0=Normal/Sleep after 1 second, 1=Always awake)
 _______
 |74273 |
 /WR -----------------> |CLK | _____
 AD1/SIO CLK ---------> |D1 Q1|--------------> CLK |74125|
 AD2 power control ---> |D2 Q2|---> ____ | |
 AD3/SIO DIR ---------> |D3 Q3|------o-|7400\________|/EN |
 AD8 rumble on/off ---> |D? Q?|---> '-|____/ | |
 AD0/SIO DTA ----o----> |D5 Q5|----------------------|A Y|--o--DTA
 | |_______| |- - -| |
 ____ '-------------------------------------|Y A|--'
 /RD ---|7400\______ ____ | |
 /RD ---|____/ |7400\_____________________________|/EN |
 A19 _______________|____/ |_____| |
| --- |

7400 Quad NAND Gate, 74273 8bit Latch

AD0 Optical Sensor Serial Data (0=Low, 1=High)

AD1 Optical Sensor Serial Clock (0=Low, 1=High)

AD2 Optical Sensor Power (0=Off, 1=On)

AD3 Optical Sensor Serial Direction (0=Read, 1=Write)

AD8 Rumble Motor (0=Off, 1=On)

Thanks: Daniel Palmer

| DS Cart Expansion RAM |
| --- |

**DS Memory Expansions**

There are several RAM expansions for the NDS. The RAM cartridge connects to the
GBA slot; can can be then accessed from cartridges in the DS slot.

```

| Opera (8MB RAM) (official RAM expansion for Opera browser)
 EZ3/4/3-in-1 (8-16MB RAM, plus FLASH, plus rumble)
 Supercard (32MB)
 M3 (32MB)
 G6 (32MB) |
| --- |

The recommended access time (waitstates) for all memory types is unknown.
Unknown which programs do use these expansions for which purposes (aside from
the Opera browser).

Thanks to Rick "Lick" Wong for info on detection and unlocking.

**Opera / DS Memory Expansion Pak (NTR-011 or USG-007)**

```

| base=9000000h, size=800000h (8MB)
 unlock=1, lock=0
 STRH [8240000h],lock/unlock |
| --- |

**EZ**

```

| base=8400000h, size=VAR (8MB..16MB)
 locking/unlocking/detection see below |
| --- |

**Supercard**

```

| base=8000000h, size=1FFFFFEh (32MB minus last two bytes?)
 unlock=5 (RAM_RW), lock=3 (MEDIA)
 STRH [9FFFFFEh],A55Ah
 STRH [9FFFFFEh],A55Ah
 STRH [9FFFFFEh],lock/unlock
 STRH [9FFFFFEh],lock/unlock |
| --- |

**M3**

```

| base=8000000h, size=2000000h (32MB)
 unlock=00400006h, lock=00400003h
 LDRH Rd,[8E00002h]
 LDRH Rd,[800000Eh]
 LDRH Rd,[8801FFCh]
 LDRH Rd,[800104Ah]
 LDRH Rd,[8800612h]
 LDRH Rd,[8000000h]
 LDRH Rd,[8801B66h]
 LDRH Rd,[8000000h+(lock/unlock)*2]
 LDRH Rd,[800080Eh]
 LDRH Rd,[8000000h]
 LDRH Rd,[80001E4h]
 LDRH Rd,[80001E4h]
 LDRH Rd,[8000188h]
 LDRH Rd,[8000188h] |
| --- |

**G6**

```

| base=8000000h, size=2000000h (32MB)
 unlock=6, lock=3
 LDRH Rd,[9000000h]
 LDRH Rd,[9FFFFE0h]
 LDRH Rd,[9FFFFECh]
 LDRH Rd,[9FFFFECh]
 LDRH Rd,[9FFFFECh]
 LDRH Rd,[9FFFFFCh]
 LDRH Rd,[9FFFFFCh]
 LDRH Rd,[9FFFFFCh]
 LDRH Rd,[9FFFF4Ah]
 LDRH Rd,[9FFFF4Ah]
 LDRH Rd,[9FFFF4Ah]
 LDRH Rd,[9200000h+(lock/unlock)*2]
 LDRH Rd,[9FFFFF0h]
 LDRH Rd,[9FFFFE8h] |
| --- |

**Detection**

For EZ, detection works as so:

```

| ez_ram_test: ;Based on DSLinux Amadeus' detection
 ez_subfunc(9880000h,8000h) ;-SetRompage (OS mode)
 ez_subfunc(9C40000h,1500h) ;-OpenNorWrite
 [08400000h]=1234h ;\
 if [08400000h]=1234h ; test writability at 8400000h
 [8000000h]=4321h ; and non-writability at 8000000h
 if [8000000h]<>4321h ;
 return true ;/
 ez_subfunc(9C40000h,D200h) ;CloseNorWrite
 ez_subfunc(9880000h,0160h) ;SetRompage (0160h)
 ez_subfunc(9C40000h,1500h) ;OpenNorWrite
 [8400000h]=1234h ;\
 if [8400000h]=1234h ; test writability at 8400000h
 return true ;/
 return false ;-failed
 ez_subfunc(addr,data):
 STRH [9FE0000h],D200h
 STRH [8000000h],1500h
 STRH [8020000h],D200h
 STRH [8040000h],1500h
 STRH [addr],data
 STRH [9FC0000h],1500h |
| --- |

For all other types (everything except EZ), simply verify that you can write
(when unlocked), and that you can't (when locked).

| DS Cart Infrared/Pedometers |
| --- |

**NDS/DSi Cartridges with IR port**

DS Cart Infrared Cartridge SPI Commands

DS Cart Infrared Cartridge Memory Map

**Activity Meter**

Pedometer with two-color LED and button. The step counter results can be
transferred to NDS via IrDA.

DS Cart Infrared Activity Meter IR Commands

DS Cart Infrared Activity Meter Memory Map

**P-Walker**

Pedometer with LCD, speaker, and three buttons. There is no intended way to run
custom program code (though it can be tweaked to do so via CPU Memory Write
command).

DS Cart Infrared P-Walker IR Commands

DS Cart Infrared P-Walker Memory Map

DS Cart Infrared P-Walker Ports LCD Controller

DS Cart Infrared P-Walker Ports Accelerometer BMA150

The purpose is more or less unknown: Apart from communicating with the NDS, the
IrDA can be also used communicate with other P-Walkers (maybe for
trading/fighting?). The GUI supports Teams, Routes, Events, Items (maybe for
some built-in interactice game engine?).

**Component Lists**

DS Cart Infrared Component Lists

**H8/38602 CPU series with H8/300H instruction set**

The NDS cartridges and Activity Meter and P-Walker contain Renesas H8/3860X
CPUs with H8/300H instruction set and on-chip firmware. In the cartridge it's
merely used for forwading IR messages via SPI bus, in the pedometer it's
handling step sensors, step counting, EEPROM logging, buttons, LED/LCD,
RTC/time, IR messages, etc.

H8/386 SFRs

H8/386 Exception Vectors

H8/300H Operands

H8/300H Opcodes

**References**

```

| H8/300H Series Programming Manual (Hitachi, 257 pages) ;-Opcodes
 H8/38602R Group Hardware Manual (Renesas, 554 pages) ;-SFR's
 The addition of H8/38606 Group (Renesas, 6 pages) ;-FLASH/ROM/RAM |
| --- |

For P-Walker:

```
[http://dmitry.gr/?r=05.Projects&proj=28.%20pokewalker](http://dmitry.gr/?r=05.Projects&proj=28.%20pokewalker)[http://forums.nesdev.org/viewtopic.php?f=23&t=21140#p265388](http://forums.nesdev.org/viewtopic.php?f=23&t=21140#p265388)
| BMA150 Triaxial digital acceleration sensor Data sheet (Bosch, 56 pages)
 SSD1850 Advance Information (Solomon System, 56 pages) ;-LCD driver
 ;-Disassembly/Story
 ;-Forum |
| --- |

| DS Cart Infrared Cartridge SPI Commands |
| --- |

There are two NDS cart firmware versions with minor differences:

```

| OLD was used in Walk with Me (maybe also Active Health?)
 NEW was used in the P-Letter game series |
| --- |

The IR-port is accessed via SPI bus commands; that bus is also shared for
accessing FLASH/EEPROM memory (via 00h-prefix).

**Infrared NDS cartridge SPI command summary**

```

| 04h,04h Initial dummy in walk with me (bugged read or wrdi?)
 00h,cmd,params[...] Savedata access
 01h,00h,00h Infrared RX (none, len=0, plus dummy data=0)
 01h,len,data[len] Infrared RX (OLD: max 84h bytes, NEW: max B8h bytes)
 02h,data[...] Infrared TX (OLD: max 84h bytes, NEW: max B8h bytes)
 02h,F2h,data[...] OLD: ignored (refuses to TX data starting with F2h)
 03h,msb,lsb,data Memory Write 8bit ;\MOV.B
 04h,msb,lsb,data Memory Read 8bit ;/
 05h,msb,lsb,data,data Memory Write 16bit ;\MOV.W (fails on 8bit SFRs?)
 06h,msb,lsb,data,data Memory Read 16bit ;/
 07h,00h,num,num,num,... Blah, returns num params from previous spi command
 08h..FFh OLD: Ignored (keeps awaiting a SPI command byte)
 08h,ver NEW: Returns version (ver=AAh)
 09h..FFh OLD: Ignored (returns zeropadding) |
| --- |

**SPI Transfers**

The SPI transfers are working at max 1MHz transfer clock, and they do require a
delay after each byte:

Waiting 800h clks at 33MHz seems to work okay (eg. MOV r0,200h // SWI 03h on
ARM7). The NEW ROM version disables IR polling when doing the SPI transfers for
RX/TX data blocks (so it may work with shorter delays between the data[...]
bytes).

The savedata access is directly passed to the FLASH/EEPROM chip and does work
at 4MHz without delays (except, the leading 00h prefix must be transferred at
1MHz plus delay, and another delay is needed when releasing chipselect after
last byte).

Note: The NDS cart slot IRQ pin is GNDed in Walk with Me (ie. there is no IRQ
for SPI/IR status).

SPI access does require cart power on and reset (via DSi SCFG registers), but
doesn't require any ROM commands like secure area loading. Confusingly, the ROM
Chip ID seems to have an IR flag in bit0 of 3rd byte (although the ROM chip
isn't wired to IR hardware at all).

**IR Transfers**

The IR transfers are using a fixed baudrate: 115200 baud, 8n1, one-directional
(RX gets disabled during TX). The RX/TX commands are transferring "packets"
(with each "packet" being terminated by a "pause" in the IR transmission; that
"packet+pause" mechanism is making it impossible to use streaming for
transferring larger blocks that exceed the buffer size of max 84h or B8h
bytes).

The RX command will return empty data with len=00h until a WHOLE packet has
been received via IR.

The TX command won't start the IR transfer until the WHOLE packet has been
written via SPI, with packet end indicated by releasing chip select.

There is no way to detect TX transfer end (other than computing the expected
tranfer time and using an ARM timer).

However, Nintendo is sending a Reply for most TX commands, so one can simply
wait for RX packets to determine TX completion (if neither Reply nor Checksum
Error are received then one will still need timeout handling).

**Memory Read/Write**

These commands are normally not used. The memory commands do forcefully abort
IR transfers, so they can't be used for polling IR transfer status.

However, they can be used for dumping the firmware ROM, and they could be used
to upload/execute custom code in RAM, which may allow to overcome some of the
above IR transfer restrictions (other baudrates, fewer delays, better
streaming, not ignoring byte F2h, etc).

**IR Cart Detection**

NDS/DSi Carts with IR support are having special game code with "I" in first
letter (NTR-Ixxx or TWL-Ixxx). There are reportedly pirate/bootleg versions of
the P-Letter games without IR hardware, unknown if they do nethertheless have
the "I" in the gamecode.

Emulators can detect the leading 00h prefix for Savedata access, although that
detection may go wrong if preceeded by IR access. Emulators can additionally
detect the slow 1MHz SPI clock used for IR access (and for 00h prefix).

| DS Cart Infrared Cartridge Memory Map |
| --- |

**IR Cartridge SFR Usage**

```

| [0FFD6h].0 Port 3 Data bit0 OUT IrDA PWDOWN (1=disable IrDA RX)
 [0FFD6h].1 Port 3 Data bit1 IN IrDA RXD ;\via serial IrDA registers
 [0FFD6h].2 Port 3 Data bit2 OUT IrDA TXD ;/
 [0FFDBh].3 Port 8 Data bit3 OUT Savedata chipselect (0=select) (cmd 00h)
 [0FFDBh].2 Port 8 Data bit2 OUT LED color ;\used in UNUSED functions,
 [0FFDBh].3 Port 8 Data bit3 OUT LED color ; in OLD ROM only, and
 [0FFDEh].0 Port B Data bit0 IN Button input ;/conflicting with Savedata
 IrDA IR Transfers
 SPI NDS Console (and cmd 00h forwarding to Savedata) |
| --- |

**IR Cartridge (OLD Version)**

```

| FB80h 200h undocumented and unused RAM, is R/W in my 38600R (!)
 FD80h 2 unused ;-unused
 FD82h 2 ir_callback ;\main callbacks for ir/spi polling
 FD84h 2 spi_callback ;/
 FD86h 2 ir_timestamp ;-last ir access (for timeout)?
 FD88h 2 spi_timestamp ;-last spi access (for debug or so)?
 FD8Ah 1 initial_blah ;-initial state of Port 8.bit3 (not really used)
 FD8Bh 1 ir_rxbuf_wrptr ;-ir_rxbuf_wrptr (for incoming IR data)?
 FD8Ch 1 ir_rxbuf_rdptr ;-ir_rxbuf_rdptr (for forwarding to spi)?
 FD8Dh 84h spi_rx_buf ;-spi_rx_buf ;(also ir TX buf)
 FE11h 84h infrared_rx_buf ;-infrared_rx_buf
 FE95h 1 spi_index ;-spi_index
 FE96h 1 ir_tx_index ;-ir_tx_index (from spi buf to TX infrared)
 FE97h 1 ir_timeout_flag ;-ir_timeout_flag (or packet end or so?)
 FE98h 2 button_num_changes ;\
 FE9Ah 2 button_num_pushes ; used only in
 FE9Ch 1 button_new_state ; UNUSED functions
 FE9Dh 1 button_old_state ;
 FE9Eh 1 button_newly_pushed ;
 FE9Fh 1 button_offhold ;/
 FEA0h E0h stack_area (stacktop at FF80h) |
| --- |

**IR Cartridge (NEW Version)**

```

| FB80h 200h undocumented and unused RAM, is R/W in my 38600R (!)
 FD80h 2 unused ;-unused
 FD82h 2 ir_callback ;\main callbacks for ir/spi polling
 FD84h 2 spi_callback ;/
 FD86h 2 ir_timestamp ;-last ir access (for timeout)?
 FD88h 1 ir_rxbuf_wrptr ;-ir_rxbuf_wrptr (for incoming IR data)?
 FD89h 1 ir_rxbuf_rdptr ;-blah, always set to 0, never used
 FD8Ah 1 spi_index ;-spi_index
 FD8Bh 1 ir_tx_index ;-ir_tx_index (from spi buf to TX infrared)
 FD8Ch B8h spi_rx_buf ;-spi_rx_buf ;(also ir TX buf)
 FE44h B8h+1 infrared_rx_buf ;-infrared_rx_buf (plus space for appending 00h)
 FEFDh 1 ir_timeout_flag ;-ir_timeout_flag (or packet end or so?)
 FEFEh 82h stack_area (stacktop at FF80h) |
| --- |

| DS Cart Infrared Activity Meter IR Commands |
| --- |

**Packet Encryption/Checksumming**

Nintendo wants all IR packet bytes to be "encrypted" (XORed by AAh), that
encryption/decryption must be done on ARM side. The checksums are calculated as
so (on decrypted packets):

```

| sum=0, packet[2,3]=00h,00h ;-initial chksum
 for i=0 to size-1
 if (i and 1)=0 then sum=sum+packet[i]*100h ;\add in big-endian fashion
 if (i and 1)=1 then sum=sum+packet[i] ;/
 sum=(sum/10000h)+(sum AND FFFFh) ;\final adjust
 sum=(sum/10000h)+(sum) ;/
 packet[2,3]=sum,sum/100h ;-store in little-endian |
| --- |

The packets are transferred at 115200 baud, 8n1. End of Packet is indicated by
a pause in the IR transmission (that does also indicate the packet size).

Before sending a command packet, one should always wait for incoming data from
the Activity Meter (ie. for the FCh byte, or for Reply/ChecksumError responses
for previous command).

**Activity Meter IR commands (from NDS):**

```

| 08,xx,cc,cc,msb,lsb,data[..] CPU Memory Write (len=3Eh max) ;Reply=08
 0A,xx,cc,cc,msb,lsb,len CPU Memory Read (len=40h max) ;Reply=0A
 0A,xx,cc,cc,FB,9C,len CPU Memory Read FB9Ch with ClrFlag ;Reply=0A
 20,xx,cc,cc,msb,lsb,data[..] Serial EEPROM Write (len=3Eh max) ;Reply=20
 22,xx,cc,cc,msb,lsb,len Serial EEPROM Read (len=40h max) ;Reply=22
 24,00,cc,cc,ss,ss,ss,ss Update Ringbuf_mm ;\ ;Reply=24
 24,01,cc,cc,ss,ss,ss,ss Update Ringbuf_hh ; and set ;Reply=24
 24,02,cc,cc,ss,ss,ss,ss Update Ringbuf_dd ; 32bit ;Reply=24
 24,03,cc,cc,ss,mm,hh Set RTC hh:mm:ss ; seconds ;Reply=24
 24,04,cc,cc,ss,ss,ss,ss Raw Set ssssssss ? ;/ ;Reply=24
 24,xx,cc,cc,ss,ss,ss,ss Invalid (same as 24,04) ;Reply=24
 26,xx,cc,cc Deadlock ;\both same (maybe ;Reply=26
 28,xx,cc,cc Deadlock ;/Watchdog/reboot?) ;Reply=26
 2A,xx,cc,cc,00,nn Stepback Ringbuf_hh ;\go back nn ;Reply=2A
 2A,xx,cc,cc,01,nn Stepback Ringbuf_mm ; entries, ;Reply=2A
 2A,xx,cc,cc,02,nn Stepback Ringbuf_dd ;/see [FCDAh] ;Reply=2A
 2A,xx,cc,cc,xx,.. Invalid ;Reply=2A
 2C,cs,cc,cc Toggle one LED on/off ;Reply=2C
 F4,xx,cc,cc Disconnect ;Reply=None
 F6,xx,cc,cc Force "Bad Chksum" reply ;Reply=FC
 FA,xx,cc,cc Connect ;Reply=F8
 FE,... Noise ;\ignored, noise ;Reply=None
 FF,... Noise ;/ ;Reply=None
 xx,xx,cc,cc Invalid ;-ignored, invalid cmd ;Reply=None
 xx,xx,xx,xx Bad Chksum ;Reply=FC |
| --- |

**Activity Meter IR replies (to NDS):**

```

| 08,sq,cc,cc Reply to Cmd 08 (CPU Memory Write reply)
 0A,sq,cc,cc,data[..] Reply to Cmd 0A (CPU Memory Read reply)
 20,sq,cc,cc Reply to Cmd 20 (Serial EEPROM Write reply)
 22,sq,cc,cc,data[..] Reply to Cmd 22 (Serial EEPROM Read reply)
 26,xx,cc,cc Reply to Cmd 26 and 28 (Deadlock reply)
 24,xx,cc,cc Reply to Cmd 24 (Update, or Set RTC time)
 2A,xx,cc,cc Reply to Cmd 2A (Stepback, with result at [FCDAh])
 2C,cs,cc,cc Reply to Cmd 2C (LED reply)
 80,FF,cc,cc Factory Reset and Hardware Test completed (or failed)
 F8,00,cc,cc Reply to Cmd FA (Connect reply)
 FC,xx,cc,cc Reply to Cmd's with Bad Chksum (and Cmd F6)
 FC Advertising Msg (after pressing button) (single byte) |
| --- |

**Notes**

```

| cc,cc Checksum (LITTLE-ENDIAN)
 msb,lsb Memory Address (big-endian)
 ss,ss,ss,ss Seconds since 2001 (big-endian)
 ss,mm,hh RTC time HH:MM:SS (BCD) (caution: smashes seconds since 2001)
 sq Increasing sequence number in Memory Access replies
 cs LED color/state (c=color red/green, s=state on/off)
 xx Whatever (don't care?) |
| --- |

There aren't any specific commands for reading things like step counters, one
must instead use the Memory Read/Write commands with hardcoded RAM or EEPROM
address, see:

DS Cart Infrared Activity Meter Memory Map

The command/reply are intended to be transferred from/to NDS accordingly, but
things could go wrong if there are multiple consoles or activity meters (all
trying to process the same message, or even mistreating replies as commands).

The infrared range (distance/angle) is unknown. Dumping the whole 64K CPU
memory space worked without checksum errors at about 5-10cm distance (and that
worked without even using the Connect command).

| DS Cart Infrared Activity Meter Memory Map |
| --- |

The most important RAM locations are FCE8h=Total steps, and FB9Ch=Unique ID
(when using multiple Activity Meters), FCF0h=Daily Goal (to change LED color
after N steps). Nintendo is reading/writing a few more RAM locations. And,
there are ring buffers with steps per minute/hour/day in EEPROM.

**Activity Meter SFR Usage**

```

| [0FFD4h].0 Port 1 Data bit0 IN Factory Test (0=Test, 1=Normal)
 [0FFD4h].2 Port 1 Data bit2 OUT Set for sum of eight A/D conversions
 [0FFD6h].0 Port 3 Data bit0 OUT IrDA PWDOWN (1=disable IrDA RX)
 [0FFD6h].1 Port 3 Data bit1 IN IrDA RXD ;\via serial IrDA registers
 [0FFD6h].2 Port 3 Data bit2 OUT IrDA TXD ;/
 [0FFDBh].2 Port 8 Data bit2 OUT LED color?
 [0FFDBh].3 Port 8 Data bit3 OUT LED color?
 [0FFDCh].0 Port 9 Data bit0 OUT SPI EEPROM chipselect (0=select)
 [0FFDEh].0 Port B Data bit0 IN Button input
 IrDA IR Transfers
 SPI SPI 8Kbyte EEPROM
 A/D Used to read two single-axis sensors (for step counting)?
 A/D Also used to read sum of eight A/D conversions (for wakeup from sleep)? |
| --- |

The SPI EEPROM uses same commands as NDS cart savedata:

DS Cartridge Backup

**Activity Meter RAM (1Kbyte, FB80h-FF7Fh)**

```

| FB80h 1 Button flags (bit7=curr.state, bit6=newly.pressed, bit5=old.state)
 FB81h 1 ... cleared if memread src was unique_id (and other cases)
 FB82h 1 ... sys/power mode ?
 FB83h 1 ... adc_mode, or power_saving?
 FB84h 1 ... clock change request
 FB85h 1 ... led_extra_mask (never CLEARED, except on boot, or maybe via IR)
 FB86h 1 adc_array_index (index in ADC array X/Y, wraps in range 00h..3Fh)
 FB87h 1 ... entrysize of current data in ringbuf (per newest TAG) or so?
 FB88h 1 SPI overrun error (probably nonsense, SPI clk can't outrun itself)
 FB89h 1 Unused
 FB8Ah 1 num_steps_curr_minute (00h..FCh) (no conflict with tag FDh,FEh,FFh)
 FB8Bh 1 rtc_event_flags (bit0=minute, bit1=hour, bit2=day, bit3=also.hour)
 FB8Ch 1 ... timing offhold for various stuff
 FB8Dh 1 some_shift_amount ;READ via IR
 FB8Eh 1 Daily goal reached flag (aka LED color) (bit0=reached, bit1=???)
 FB8Fh 1 ... timing for LED step pulses?
 FB90h 1 ... timing for LED step pulses?
 FB91h 1 ... flag for LED step pulse state?
 FB92h 1 Hour when new day starts (BCD, usually/always 03h) ;READ via IR
 FB93h 1 ... some flag for inactivity low-power mode ?
 FB94h 1 LED animation number (1..5, or 0=none) (factory test result)
 FB95h 1 Unused
 FB96h 1 New day flag
 FB97h 1 Fixed LED mask (this is a "fixed" setting from EEPROM)
 FB98h 1 Compare_ctrl_0 ;\for "Compare Control" HW registers (89h,89h)
 FB99h 1 Compare_ctrl_1 ;/
 FB9Ah 1 New Goal flag (apply [FCF4h] as new goal, starting on next day?)
 FB9Bh 1 Unused
 FB9Ch 28h Unique ID ;READ via IR (initally set by NDS via RAM+EEPROM writes?)
 FBC4h 2 adc_current_x
 FBC6h 2 adc_current_y
 FBC8h 80h adc_array_x (40h x 16bit)
 FC48h 80h adc_array_y (40h x 16bit)
 FCC8h 2 adc_scale_factor_x ;\scale factors
 FCCAh 2 adc_scale_factor_y ;/
 FCCCh 2 adc_scale_unused_z ;\semi-unused (written, but never read)
 FCCEh 2 adc_scale_unused_t ;/
 FCD0h 2 ringbuf_mm_index (0020h..16A0h) ;READ via IR
 FCD2h 2 ringbuf_hh_index (16A1h..1C42h) ;READ via IR
 FBD4h 2 Unused
 FCD6h 2 ringbuf_dd_index (1C43h..1CDEh) ;READ via IR
 FCD8h 2 num_steps_curr_hour (16bit step counter for current hour)
 FCDAh 2 ringbuf_stepback_index (result from cmd_2Ah, to be read by cmd_0Ah)
 FCDCh 2 adc_inactivity_timer (time since last pedometer step)
 FCDEh 2 SPI overrun error counter (related to flag at FB88h)
 FCE0h 2 Unused
 FCE2h 2 adc_current_sum (sum of eight A/D conversions)
 FCE4h 4 seconds_counter (seconds since 1st Jan 2001?, initially 0D2B0B80h)
 FCE8h 4 num_steps_lifelong (lifelong TOTAL steps)
 FCECh 4 num_steps_today (step counter, for current day)
 FCF0h 4 Daily_goal (WRITTEN via IR, NDS cart default=3000 decimal)
 FCF4h 4 new_goal_steps (somewhat reload value for daily goal?)
 FCF8h 18h Unused
 FD10h 2 main_callback (main_adc_button_callback, or ir_callback)
 FD12h 40h ir_tx_data (buffer for Memory & EEPROM reads)
 FD52h 2 clk_callback (clk_whatever_callback, or 0=none)
 FD54h 2 ir_callback (ir_active_callback, or ir_dummy_callback)
 FD56h 2 ir_timestamp_last_byte (for sensing SHORT GAPs, aka end-of-packet)
 FD58h 2 RX chksum from hdr[2..3]
 FD5Ah 2 RX chksum from calculation
 FD5Ch 2 ir_timestamp_last_xfer (for sensing LONG GAPs, aka sleep mode)
 FD5Eh 1 Unused
 FD5Fh 1 ir_rx_len
 FD60h 1 ... semi-unused (set to 00h?) (but never read)
 FD61h 44h ir_rxtx_buf, hdr[4]+data[40h]
 FDA5h 1 ir_tx_hdr_len ;\memorized TX len+hdr[4]
 FDA6h 4 ir_tx_hdr_copy ;/(never actually used)
 FDAAh 1 bad_chksum_count, give up sending bad_chksum replies after 3 errors
 FDABh 1 bad_chksum_flag, request reply_FCh (bad_chksum)
 FDACh 80h ... array (40h x 16bit) ;\
 FE2Ch 80h ... array (40h x 16bit) ; analog sine/cosine
 FEACh 4 ... dword ; stuff for converting
 FEB0h 4 ... dword ; adc to step counter?
 FEB4h 1 ... byte ;
 FEB5h 1 ... byte ;/
 FEB6h 2 Incremented in main_adc_button_callback (but not used elsewhere)
 FEB8h 1 Unused ;\maybe meant to be 4-byte tx hdr,
 FEB9h 1 TX sequence number ; but only hdr[1] used (as increasing
 FEBAh 2 Unused ;/seq.no for memory read/write replies)
 FEBCh 4 ... array (2 x 16bit)
 FEC0h 4 ... array (2 x 16bit)
 FEC4h 4 ... array (2 x 16bit)
 FEC8h 4 ... array (2 x 16bit)
 FECEh B2h CPU Stack area, initial SP=FF80h |
| --- |

**Activity Meter EEPROM (8Kbytes, serial SPI bus)**

```

| EEPROM:0000h 9 ID "nintendo",00h (9 bytes)
 EEPROM:0009h 17h Unused (FFh-filled)
 EEPROM:0020h 1681h Ringbuf_mm ;steps per MINUTE for 4 days ;(24*60*4-1)*8bit
 EEPROM:16A1h 5A2h Ringbuf_hh ;steps per HOUR for 30 days ;(24*30+1)*16bit
 EEPROM:1C43h 9Ch Ringbuf_dd ;steps per DAY for 52 days ;(52)*24bit
 EEPROM:1CDFh 1 Unused (FFh) (padding ringbuf's to 20h-byte-boundary)
 EEPROM:1CE0h 200h Unused (FFh-filled)
 EEPROM:1EE0h 8+1 ADC_scale_values (4x16bit) ;RAM:FCC8h ;\
 EEPROM:1EE9h 2+1 ADC sum_limit ;RAM:stack ; these EEPROM
 EEPROM:1EECh 3 Unused ; settings
 EEPROM:1EEFh 4+1 Num_steps_lifelong ;RAM:FCE8h ; have 1-byte
 EEPROM:1EF4h 1+1 Fixed LED Mask ;RAM:FB97h ; checksums
 EEPROM:1EF6h 2 Unused ; appended, and
 EEPROM:1EF8h 1+1 Some_shift_amount ;RAM:FB8Dh ; backups at
 EEPROM:1EFAh 4+1 Daily_goal ;RAM:FCF0h ; 1F40h-1F9Fh
 EEPROM:1EFFh 4+1 New_goal_steps ;RAM:FCF4h ;
 EEPROM:1F04h 28h+1 Unique ID ;RAM:FB9Ch ;
 EEPROM:1F2Dh 13h Unused (00h-filled) ;/
 EEPROM:1F40h 60h Backup copies of above data at 1EE0h..1F3Fh ;-backups
 EEPROM:1FA0h 2 Error code (initially FFFFh)
 EEPROM:1FA2h 1 Reboot counter (initially 00h or 01h ?)
 EEPROM:1FA3h 5Dh Unused (FFh-filled) |
| --- |

The MM/HH/DD ring buffers are steps per minute/hour/day accordingly, mixed with
special tags:

```

| 00xxh Zero steps for N minutes (N=max FCh) ;\in ringbuf_mm
 xxh N steps per minute (N=01h..FCh) ;/
 xxxxh N steps per hour (N=0000h..FFFFh) ;-in ringbuf_hh
 xxxxxxh N steps per day (N=000000h..FFFFFFh) ;-in ringbuf_dd
 FDxxxxxxxxxxh Timestamp, reversed-BCD-digit-order, seconds since 2001 or so?
 FEh Newest entry marker?
 FFh Unused entry marker? |
| --- |

The firmware contains code for searching tags FDh/FEh/FFh forwards and
backwards, that works well with BCD values and 8bit counters, but unknown it
can work with the 16bit/24bit counters. The BCD digits are stored backwards
(eg. 12345 = FD5432100000h). Rinbuf pointers can be read from RAM locations
FCD0h, FCD2h, FCD6h, and FCDAh).

| DS Cart Infrared P-Walker IR Commands |
| --- |

Commands are usually send from NDS (or from other Walkers)

```

| EEPROM Commands (Cmd 02,04,0C,0E,82) ;\From NDS or Walker
 Connect Commands (Cmd F8,FA,FC) ;/
 Peer Commands (Cmd 10...1C) ;-From Walker
 Unused Commands (Cmd's with * marking) ;-From Prototype tests?
 Other Commands (Cmd's other than above) ;-From NDS |
| --- |

**Incoming Commands from NDS (or incoming commands/replies from another Walker)**

```

| 00,hi,..,lzss(..) EEPROM Write [hi00h..hi7Fh] Compressed ;Reply=04
 80,hi,..,lzss(..) EEPROM Write [hi80h..hiFFh] Compressed ;Reply=04
 02,hi,..,data(..) EEPROM Write [hi00h..hi7Fh] Raw ;Reply=04
 82,hi,..,data(..) EEPROM Write [hi80h..hiFFh] Raw ;Reply=04
 04,xx,.. EEPROM Write Reply ;SendMoreCmd(s)
 06,hi,..,lo,data(nn)* CPU Memory Write [hilo+(0..nn-1)] ;Reply=06
 0A,hi,..,lo,data(nn)* EEPROM Write Random Len [hilo+(0..nn-1)] ;Reply=04
 0C,xx,..,hi,lo,nn EEPROM Read Request [hilo+(0..nn-1)] ;Reply=0E
 0E,xx,..,data(nn) EEPROM Read Reply ;SendMoreCmd(s)
 10,xx,..,data(68h) Peer Step 1 Request ;Reply=12
 12,xx,..,data(68h) Peer Step 1 Reply ;SendMoreCmd(s)
 14,xx,..,data(38h) Peer Step 2 Request ;Reply=16
 16,xx,.. Peer Step 2 Reply ;Reply=16 or None
 1C,xx,.. Peer Refuse ;Reply=None+Disconnect
 20,xx,.. Identity Read Request ;Reply=22
 24,xx,.. * Ping Request ;Reply=26
 2A,xx,..,none? Unique ID Read Request ;Reply=2A
 2C,xx,..,none? * Unique ID Read Request slightly other ;Reply=2A
 32,xx,..,data(28h?) * Identity Write Request 1 ;Reply=34
 36,xx,.. * Connection Error 1 ;Reply=None
 38,xx,.. * Walk Start Silent ;Reply=38
 40,xx,..,data(28h?) * Identity Write Request 2 ;Reply=42
 44,xx,.. * Connection Error 2 ;Reply=None
 4E,xx,.. Walk End Request ;Reply=50
 52,xx,..,data(28h?)?? Identity Write Request 3 ;Reply=54
 56,xx,.. * Connection Error 3 ;Reply=None
 5A,xx,.. Walk Start Nonsilent ;Reply=5A
 60,xx,..,data(28h?) * Identity Write Request 4 ;Reply=62
 64,xx,.. * Connection Error 4 ;Reply=None
 66,xx,.. * Walk End Request OTHER ;Reply=68
 9C,xx,.. * Error Whatever ;Reply=9C+Disconnect
 9E,xx,.. * Error Weird Participate ;Reply=9E+Disconnect
 A0,xx,.. * Weird Participate 1 ;Reply=A0 or 9E
 A2,xx,.. * Weird Participate 2 ;Reply=A2 or 9E
 A4,xx,.. * Weird Participate 3 ;Reply=A4 or 9E
 A6,xx,.. * Weird Participate 4 ;Reply=A6 or 9E
 A8,xx,.. * Weird Participate 5 ;Reply=A8 or 9E
 AA,xx,.. * Weird Participate 6 ;Reply=AA or 9E
 AC,xx,.. * Weird Participate 7 ;Reply=AC or 9E
 AE,xx,.. * Weird Participate 8 ;Reply=AE or 9E
 B8,xx,.. * Award Stamp Heart ;Reply=D8
 BA,xx,.. * Award Stamp Spade ;Reply=DA
 BC,xx,.. * Award Stamp Diamond ;Reply=DC
 BE,xx,.. * Award Stamp Club ;Reply=DE
 C0,xx,.. * Award Special Map ;Reply=C0
 C2,xx,.. * Award Event P-Letter ;Reply=C2
 C4,xx,.. * Award Event Item ;Reply=C4
 C6,xx,.. * Award Event Route ;Reply=C6
 D0,xx,.. * Award All Stamps and Special Map ;Reply=C0
 D2,xx,.. * Award All Stamps and Event P-Letter ;Reply=C2
 D4,xx,.. * Award All Stamps and Event Item ;Reply=C4
 D6,xx,.. * Award All Stamps and Event Route ;Reply=C6
 D8,xx,.. * Connection Error 5 ;Reply=None
 F0,xx,..,data(71h) ?? Enroll Data (28h+40h+8+1 bytes) ;Reply=F0
 F4,xx,.. * Disconnect ;Reply=None+Disconnect
 F8,02,.. Connection Reply from Walker ;SendCmd=1002
 FA,01,.. Connection Request from NDS ;Reply=F802
 FA,02,.. Connection Request from Walker ;Reply=F802
 FA,xx,.. Connection Request invalid ;Reply=None+Disconnect
 FC Connection Beacon from Walker ;SendCmd=FA
 FE,01,..,data(8) * EEPROM Write [0008h..000Fh] ;Reply=FE
 xx * Ignored (single byte other than FC) ;Reply=None
 xx,xx,.. * Invalid Cmd ;Reply=None
 xx,xx,xxxx * Ignored (wrong 4-byte ID for Cmd 00-F7) ;Reply=None
 xx,xx,xxxx * Bad Checksum (disconnect if too often) ;Reply=None |
| --- |

**Outgoing Replies to NDS (or outgoing commands/replies to another Walker)**

```

| 02,hi,..,data(nn) EEPROM Write ... ;Cmd=Peer
 82,hi,..,data(nn) EEPROM Write ... ;Cmd=Peer
 04,hi,.. EEPROM Write Reply ;Cmd=00/02/0A/80/82
 06,hi,.. * CPU Memory Write Reply ;Cmd=06h
 0C,02,..,hi,lo,nn EEPROM Read Request ;Peer, EEPROM Read ;Cmd=0Eh
 0E,02,..,data(nn) EEPROM Read Reply ;Cmd=0Ch
 10,02,..,data(68h) Peer Step 1 Request (after Connect Reply);Cmd=F8h
 12,02,..,data(68h) Peer Step 1 Reply ;Cmd=10h
 14,02,..,data(38h) Peer Step 2 Request ;Cmd=0Eh
 16,02,.. Peer Step 2 Reply ;Cmd=14h/16h
 1C,02,.. Peer Refuse ;Cmd=10h/12h
 22,02,..,data(68h) Identitiy Read Reply ;Cmd=20h
 26,02,.. * Ping Reply ;Cmd=24h
 2A,02,..,data(28h) Unique ID Reply ;Cmd=2Ah/2Ch
 34,02,.. * Identitiy Write 1 Reply ;Cmd=32h
 38,02,.. * Walk Start silent Reply ;Cmd=38h
 42,02,.. * Identitiy Write 2 Reply ;Cmd=40h
 50,02,.. Walk End Reply ;Cmd=4Eh
 54,02,.. ?? Identitiy Write 3 Reply ;Cmd=52h
 5A,02,.. Walk Start nonsilent Reply ;Cmd=5Ah
 62,02,.. * Identitiy Write 4 Reply ;Cmd=60h
 68,02,.. * Walk End OTHER Reply ;Cmd=66h
 9C,02,.. * Weird Whatever Reply-to-Reply? ;Cmd=9Ch
 9E,02,..,data(11h) * Weird Participated Reply ;Cmd=A0h..AEh
 9E,02,.. * Weird Participated Reply-to-Reply? ;Cmd=9Eh
 A0,02,..,data(11h) * Weird Participated Reply 1 ;Cmd=A0h
 A2,02,..,data(11h) * Weird Participated Reply 2 ;Cmd=A2h
 A4,02,..,data(11h) * Weird Participated Reply 3 ;Cmd=A4h
 A6,02,..,data(11h) * Weird Participated Reply 4 ;Cmd=A6h
 A8,02,..,data(11h) * Weird Participated Reply 5 ;Cmd=A8h
 AA,02,..,data(11h) * Weird Participated Reply 6 ;Cmd=AAh
 AC,02,..,data(11h) * Weird Participated Reply 7 ;Cmd=ACh
 AE,02,..,data(11h) * Weird Participated Reply 8 ;Cmd=AEh
 C0,02,.. * Award Special Map Reply ;Cmd=C0h/D0h
 C2,02,.. * Award Event P-Letter Reply ;Cmd=C2h/D2h
 C4,02,.. * Award Event Item Reply ;Cmd=C4h/D4h
 C6,02,.. * Award Event Route Reply ;Cmd=C6h/D6h
 C8,02,.. * Award Stamp Heart Reply ;Cmd=B8h
 CA,02,.. * Award Stamp Spade Reply ;Cmd=BAh
 CC,02,.. * Award Stamp Diamond Reply ;Cmd=BCh
 CE,02,.. * Award Stamp Club Reply ;Cmd=BEh
 F0,02,..,data(28h) ?? Enroll Reply ;Cmd=F0h
 F8,02,.. Connect Reply ;Cmd=FAh
 FA,02,.. Connect Request from walker ;Cmd=FCh
 FC Connection Beacon ;Button?
 FE,02,.. * EEPROM Write [0008h..000Fh] Reply ;Cmd=FEh
 -?- Checksum Error... has no reply? or maybe sends Beacons? |
| --- |

**Notes**

```

| .. short for 16bit Checksum at hdr[2..3] and 32bit Session ID at hdr[4..7]
 xx somewhat don't care (usually 01h=From NDS, or 02h=From Walker) |
| --- |

All 16bit checksum and the IR "encryption" (XOR by AAh), seem to be same as for
Activity Meter (see there, except use initial sum=2, not sum=0).

The Connect Request & Reply commands are sending a "random" SessionID each,
all following commands/replies (except F8h and up) must use

```

| SessionID = ConnectRequestRandomID XOR ConnectReplyRandomID. |
| --- |

The Compressed EEPROM commands are writing decompressed 80h-bytes of data to
EEPROM (ie. the compression is only used to speedup the IR transfer, not to
save memory). The compression format is Nintendo's standard LZSS (including the
header value 10h, and little-endian 24bit length; which should be always 80h).
Special case: Compressed writes with exactly 80h bytes of incoming data are
treated as uncompressed writes (to be used when compression ratio is worse than
no compression).

| DS Cart Infrared P-Walker Memory Map |
| --- |

**P-Walker SFR Usage**

```

| [0FFD4h].0 Port 1 Data bit0 OUT SPI LCD chipselect (0=select)
 [0FFD4h].1 Port 1 Data bit1 OUT SPI LCD access mode (0=Cmd, 1=Data)
 [0FFD4h].2 Port 1 Data bit2 OUT SPI EEPROM chipselect (0=select)
 [0FFD6h].0 Port 3 Data bit0 OUT IrDA PWDOWN (1=disable IrDA RX)
 [0FFD6h].1 Port 3 Data bit1 IN IrDA RXD ;\via serial IrDA registers
 [0FFD6h].2 Port 3 Data bit2 OUT IrDA TXD ;/
 [0FFDBh].2 Port 8 Data bit2 ?
 [0FFDBh].3 Port 8 Data bit3 ?
 [0FFDBh].4 Port 8 Data bit4 OUT A/D related ... whatfor LCD? accel? batt?
 [0FFDCh].0 Port 9 Data bit0 OUT SPI Accelerometer chipselect (0=select)
 [0FFDEh].0 Port B Data bit0 IN ? ;\
 [0FFDEh].2 Port B Data bit2 IN ? ; maybe buttons
 [0FFDEh].4 Port B Data bit4 IN ? ;/
 [0FFDEh].5 Port B Data bit5 OUT ?
 Timer W General A/B/C Audio Frequency/Volume
 IrDA IR Transfers
 SPI SPI 64Kbyte EEPROM, LCD Cmd/Data, Accelerometer
 A/D whatfor LCD? accel? batt? |
| --- |

The SPI EEPROM uses same commands as NDS cart savedata:

DS Cartridge Backup

For the other two SPI-like devices, see:

DS Cart Infrared P-Walker Ports LCD Controller

DS Cart Infrared P-Walker Ports Accelerometer BMA150

Note: SPI LCD chipselect is also used for outputting factory test results via
SPI bus (requires special hardware attached instead of the LCD screen).

**RAM Map (2Kbytes, F780h..FF7Fh)**

```

| F780h 60h Misc variables
 F7E0h 2 main_callback ;<--
 F7E2h ECh Misc variables
 F8CEh 8+80h Infrared RX/TX buffer hdr+data (also misc/heap)
 F956h 62Ah Temp buffer, free RAM, and stack ;<--
 FF80h - Stacktop (end of RAM) |
| --- |

**ROM Map**

For some quick hacks, Dmitry recommends these ROM addresses (which won't work
when reflashing the firmware).

```

| 0772h Send IR packet (F8D6h=src, r0l=len, r0h=hdr[0], r1l=hdr[1])
 08D6h Default callback (when in IR transfer mode)
 259Eh Watchdog refresh |
| --- |

**EEPROM Map**

The EEPROM contains some important basic data, plus GUI related bitmaps (mostly
text strings pre-rendered as bitmaps for the local user name & game
language) (and maybe(?) also game specific customizations).

```

| 0000h 8 ID "nintendo" (set after initial power-up eeprom init)
 0008h 8 ID whatever (set via Cmd F0h and FEh) (never read)
 0010h 62h ???
 0072h 1 Number of watchdog resets
 0073h 0Dh ???
 0080h 02h+1 ADC calibration (factory-provided) ;\
 0083h 28h+1 Unique ID (set via Cmd F0h) ; with 1-byte
 00ACh 40h+1 LCD ConfigCmds (set via Cmd F0h) ; checksums
 00EDh 68h+1 Identity Data ("provisioned" at walk start time) ; and backup
 0156h 18h+1 Health Data ("provisioned" at walk start time) ; copies at
 016Fh 01h+1 Copy Flag (00h=Normal, A5h=copy was interrupted) ; 0180h-027Fh
 0171h 0Fh Unused ;/
 0180h 100h Backup copies of entries at 0080h-0017Fh
 0280h ... Various Bitmaps
 8C70h ... Various Garbage, Bitmaps, Items, Team, Route
 CE8Ah 2 current watts written to eeprom by cmd 20h before replying
 (likely so remote can read them directly). u16 BE
 CE8Ch ... Various stuff
 CEF0h 1Ch Historic step count per day. u32 each, BE,
 [0] is yesterday, [1] is day before, etc...
 CF0Ch ... Various stuff |
| --- |

```

| __________________ Data Structures (in EEPROM and Packets) ___________________ |
| --- |

**Unique ID (28h bytes)**

```

| 00h 28h Generated by the DS game at pairing time, unique per walker |
| --- |

**Identity Data (68h bytes) //stored reliably at 0x00ED/0x01ED**

All multi-byte values LE, unless otherwise specified

```

| 00h 4 Unknown (LE, always 1?) ;\written from game packet at walk start
 04h 4 Unknown (LE, always 1?) ; ;<-- 0 at walk end ;copied from [0]
 08h 2 Unknown (LE, always 7?) ;
 0Ah 2 Unknown (LE, always 7?) ;/ ;<-- 0 at walk end ;copied from [8?]
 0Ch 2 TrainerTID
 0Eh 2 TrainerSID
 10h 28h Unique ID
 38h 10h EventBitmap (aka bitfield with 128 event flags?)
 48h 10h Trainer Name (8 chars, using a custom 16bit charset, non-unicode)
 58h 1 Unknown
 59h 1 Unknown
 5Ah 1 Unknown
 5Bh 1 Flags (bit0=PairedToGame, bit1=HasPoke, bit2=PokeJoinedOnAWalk)
 5Ch 1 ProtoVer (02h) (written by DS, refuse peer's with other values)
 5Dh 1 Unknown
 5Eh 1 ProtoSubver (00h) (written by DS, refuse peer's with other values)
 5Fh 1 Unknown (02h) (written by DS at walk start)
 60h 4 LastSyncTime ;Big Endian ;in WHAT... maybe seconds since WHEN?
 64h 4 StepCount ;Big Endian ;since WHEN... today? lifetime? lastsync? |
| --- |

**Peer Play Data (38h bytes) //aka "PEER PLAY DATA"**

All multi-byte values LE, unless otherwise specified

```

| 00h 4 curStepCount (since WHEN?)
 04h 2 curWatts
 06h 2 Unused
 08h 4 Unknown, copied from IdentityData[00h]
 0Ch 2 Unknown, copied from IdentityData[08h]
 0Eh 2 Species
 10h 16h P-Nickname (11 chars) ;\the actual names in bitmap format
 26h 10h Trainer Name (8 chars) ;/are stored elsewhere in EEPROM?
 36h 1 GenderForm
 37h 1 HasSpecialForms (spinda, arceus, unown, etc.) |
| --- |

**LcdConfigCmds (40h bytes)**

Stored reliably at 00ACh/01ACh.

```

| 00h 1 u8 contrastAndFlags (if 00h/FFh? commands at ROM:BEB8h will be used)
 01h 3Fh u8 commands[3fh] (Commands, or FDh,NNh=Delay(NNh), FEh=End of list) |
| --- |

**Enroll Data (71h bytes)**

```

| 00h 28h Unique ID ;always written
 28h 40h LCD Config Data ;written or verified depending on byte[70h]
 68h 8 Whatever ID ;always written to EEPROM:0008h
 70h 1 LCD Action (00h=WriteA, 01h=Compare, 03h=WriteB, 02h/04h-FFh=Nop) |
| --- |

**HealthData (18h bytes)**

Stored reliably at 0156h/0256h. Cached in RAM at F780h.

Big Endian unless otherwise noted.

```

| 00h 4 u32 lifetimeTotalSteps
 04h 4 u32 todaySteps //zeroed at midnight
 08h 4 u32 lastSyncTime
 0Ch 2 u16 totalDays
 0Eh 2 u16 curWatts
 10h 2 u16 unk_0
 12h 1 u8 unk_1
 13h 1 u8 unk_2
 14h 3 u8 padding[3]
 17h 1 u8 settings (bit0=isOnSpecialRoute, bit1-2=Volume, bit3-6=Contrast) |
| --- |

See Dmitry's webpage for more "game-specifc" data structures and memory
addresses.

| DS Cart Infrared P-Walker Ports LCD Controller |
| --- |

**LCD Command Table**

Commands are 1 or 2 bytes long (with D/C=Low, and, unlike normal SPI, one can
release /CS after each byte, even when sending 2-byte commands).

```

| 1st Byte 2nd Byte Description
 00h+(0..Fh) - Set Column Address bit0-3 ;\VRAM xloc in 1-pixel units
 10h+(0..7) - Set Column Address bit4-6 ;/
 18h+(0..7) - Reserved
 20h+(0..7) - Set Internal Regulator Resistor Ratio
 (0..7 = 2.3, 3.0, 3.7, 4.4, 5.1, 5.8, 6.5, 7.2)
 28h+(0..7) - Set Power Control Register
 bit2: Internal Voltage Booster (0=Off, 1=On)
 bit1: Internal Regulator (0=Off, 1=On)
 bit0: Output Op-amp Buffer (0=Off, 1=On)
 30h+(0..0Fh) - Reserved
 40h 00h-xxh Set Display Start Line (0..127?) (ROW) ("scroll yloc")
 41h+(0..2) 00h-xxh Same as above?
 44h 00h-xxh Set Display Offset (0..63) (COM0=ROW0..63) (pinout?)
 45h+(0..2) 00h-xxh Same as above?
 48h 00h-xxh Set Multiplex Ratio (num lines, duty 1/((16..64)+icon))
 49h+(0..2) 00h-xxh Same as above?
 4Ch 00h-3Fh Set N-line Inversion (0=Off, 1..31=Reduce crosstalk?)
 4Ch 20h-FFh Same as above?
 4Dh+(0..2) 00h-FFh Same as above?
 50h+(0..7) - Set LCD Bias (0..5=1/(4..9), 6=1/9, too)
 57h+(0..0Ch) - Reserved
 64h+(0..3) - Set DC-DC Converter Factor (0=2x/3x, 1=4x, 2/3=5x)
 68h+(0..18h) - Reserved
 81h 00h-3Fh Set Contrast (0..3Fh, 3Fh=Darkest)
 82h OTP Set VL6 voltage (00h..0Fh = original+Signed4bit(N))
 83h OTP OTP Programming?
 84h+(0..7) - Reserved
 88h 00h-FFh Set White Mode (bit0-3=FrameA, bit4-7=FrameB)
 89h 00h-FFh Set White Mode (bit0-3=FrameC, bit4-7=FrameD)
 8Ah 00h-FFh Set Light Gray Mode (bit0-3=FrameA, bit4-7=FrameB)
 8Bh 00h-FFh Set Light Gray Mode (bit0-3=FrameC, bit4-7=FrameD)
 8Ch 00h-FFh Set Dark Gray Mode (bit0-3=FrameA, bit4-7=FrameB)
 8Dh 00h-FFh Set Dark Gray Mode (bit0-3=FrameC, bit4-7=FrameD)
 8Eh 00h-FFh Set Black Mode (bit0-3=FrameA, bit4-7=FrameB)
 8Fh 00h-FFh Set Black Mode (bit0-3=FrameC, bit4-7=FrameD)
 Above defines the grayscale palette for color 0-3,
 normally all frames should use the same setting.
 Color 0 is usually white (set to zero), color 3 is
 usually black (set to number of levels selected via
 cmd 90h). Color 1 and 2 are usually light/dark gray,
 set to desired contrast, which may depend on the LCD.
 90h+(0..7) - Set PWM and FRC for gray-scale operation
 bit0-1: Levels (0/1=Nine, 2=Twelve, 3=Fifteen Levels)
 bit2: Frames (0=Four, 1=Three Frames)
 Note: Nintendo uses "9 levels" ranging from "0 to 9"
 (maybe level 0 is treated as off, thus not counted)
 98h+(0..7) - Reserved
 A0h+(0..1) - Set Segment Remap (0=Col00h is SEG0, 1=Col7Fh is SEG0)
 Aka xflip mirror?
 A2h+(0..1) - Set Icon Enable (0=Disable, 1=Enable)
 A4h+(0..1) - Set Entire Display On/Off (0=Show RAM, 1=All Pixels On)
 A6h+(0..1) - Set Inverse Display (0=Normal, 1=Inverse On/Off Pixels)
 A8h+(0..1) - Set Power Save Mode (0=Standby, 1=Sleep)
 AAh - Reserved
 ABh - Start Internal Oscillator (needed after reset)
 ACh+(0..1) ? Reserved
 AEh+(0..1) - Set Display On/Off (0=Off, 1=On)
 B0h+(0..0Fh) - Set Page Address (00h..0Fh) ;VRAM yloc in 8-pixel units
 C0h+(0,8) - Set COM Output Scan Direction (0=Normal, 8=Remapped)
 Remapped: COM[0..(N-1)] becomes COM[(N-1)..0])
 Aka yflip mirror?
 C1h+(0..6) - Same as above (Normal)
 C9h+(0..6) - Same as above (Remapped)
 D0h+(0..10h) - Reserved
 E1h - Exit Power-save Mode (return from Sleep/Standby modes)
 E2h - Software Reset (initialize some internal registers)
 E3h - Reserved
 E4h - Exit N-line Inversion mode
 E5h+(0..2) - Reserved
 E8h LEN+DTA Transfer VRAM Display Data (for 3-wire SPI mode only)
 (LEN=00h-FFh, followed by LEN+1 data bytes)
 E9h+(0..6) - Reserved
 F0h+(0..0Fh) .. Test mode commands and Extended features
 F0h 00h-03h Ext. Set VL6 Noise reduction (0=Enable, 3=Disable)
 F1h 08h-0Fh Ext. Set TC Value per 'C (0=-0.05%, 1=-0.07%, 2..7=?)
 F2h 00h-07h Ext. Oscillator Adjustment
 (0..7 = -9%, -6%, -3%, +0%, +3%, +6%, +9%, +12%)
 F7h 00h-01h Ext. Oscillator Source (0=Internal, 1=External, 2=UNDOC)
 F6h 00h-1Fh Ext. Frame Frequency Adjust
 bit0-2: FrameFQ (0..7 = 0..7)
 bit3-4: Fosc (0-3 = 59kHz, 75kHz, 94kHz, 113kHz)
 FBh X2h,X6h Ext. Lock/Unlock Interface (bit2: 0=Unlock, 1=Lock) |
| --- |

**VRAM Writing**

First set the VRAM address using three commands with D/C=Low:

```

| 00h+(xloc) AND 0Fh ;Set Column Address bit0-3
 10h+(xloc/10h) ;Set Column Address bit4-6
 B0h+(yloc/8) ;Set Page Address (00h..0Ah, other=reserved?) |
| --- |

For the actual data transfer, set D/C=High and write data (each two bytes
contain vertical 8-pixel columns, with bit0=top, bit7=bottom):

```

| 1st byte = bitplane 1 ;\color 0..3 are usually white, lgray, dgray, black
 2nd byte = bitplane 0 ;/(palette can be changed via cmd 88h-8Fh though) |
| --- |

Note: Column Address (xloc) is auto-incremented after each byte pair, the Page
Address (yloc) is NOT incremented when wrapping from x=7Fh to x=00h.

The last page is the "icon", this has only 1 pixel per xloc, maybe intended to
enable special symbols instead of pixels (on displays to combine symbols and
pixels).

**VRAM Reading and Status Byte (both supported in parallel Non-SPI mode only)**

The controller supports four modes (Nintendo uses the 4-wire SPI mode):

```

| 3-wire SPI Serial write-only (/CS, CLK, MOSI, with cmd E8h instead D/C pin)
 4-wire SPI Serial write-only (/CS, CLK, MOSI, D/C=Data/Cmd)
 12-wire 8080 Parallel read/write (/CS, D0-D7, D/C, /RD, /WR)
 12-wire 6800 Parallel read/write (/CS, D0-D7, D/C, E, R/W) |
| --- |

VRAM can be read setting the VRAM address as usually, and then reading with
D/C=High. And, an 8bit status byte can be read with D/C=Low:

```

| 7 BUSY Chip is executing instruction (0=Ready, 1=Busy)
 6 ON Display is On/Off (0=Off, 1=On)
 5 RES Chip is executing reset (0=Ready, 1=Busy)
 4-0 - Fixed Chip ID (08h=SSD1850) |
| --- |

**Note**

Nintendo uses a 96x64 pixel grayscale LCD screen with 2bpp, without backlight
or frontlight. VRAM is double buffered (VRAM size is at least 96x128 pixels).

The display controller seems to be a Solomon System SSD1850 chip... or similar,
there are a number of datasheets that are more or less matching the LCD
commands used in the firmware:

```

| SSD0852 128x128 would allow double-buffer, but extended commands are wrong
 SSD0858 104x65 close, but extended commands are wrong
 SSD0859 128x81 could be correct (almost same as SSD1850)
 SSD1820 128x65 wrong, lacks palette (command 88h-8Fh)
 SSD1820A 128x65 wrong, lacks palette (command 88h-8Fh)
 SSD1821 128x81 wrong, lacks palette (command 88h-8Fh)
 SSD1850 128x65 could be correct (ysiz is good, but no double-buffering)
 SSD1851 128x81 as above, but more lines than needed
 SSD1852 128x128 would allow double-buffer, but extended commands are wrong
 SSD1854 128x160 wrong, uses 2-byte command B0h,YYh lacks extended commands
 (also cmd 18h,20h,4xh,50h,60h-63h,64h,82h,83h,etc. differ)
 SSD1858 104x65 close, but lacks many extended commands
 SSD1859 128x81 could be correct (almost same as SSD1850) |
| --- |

But, Nintendo's firmware contains extended command F7h,02h (which isn't
mentioned in any of the above datasheets).

And, the firmware seems to use two 64-line framebuffers at DisplayStart=0 and
DisplayStart=64, ie. needing 128 lines in total, which leaves no space for the
"icon" scanline (at least not addressable via cmd B0h+y).

| DS Cart Infrared P-Walker Ports Accelerometer BMA150 |
| --- |

**Might do drilling machine, not good honking out paste pretty screenshot?**

Official BMA150 register specs are in a Bosch-Captcha, which appears to be
intended to fool humans and bots alike (text on hatched background, undefined
color codes, increasingly unpleasant medusa-like details at closer look).
Anyways, here's a plaintext-hack of the Bosch-Captcha:

```

| 00h Chip ID (bit7-3=Unused, bit2-0=02h)
 01h Version (bit7-4=al_version, bit3-0=ml_version) (undefined values)
 02h Acc X Low (bit7-6=DataLsb, bit5-1=Unused, bit0=NewDataFlag)
 03h Acc X High (bit7-0=DataMsb)
 04h Acc Y Low (bit7-6=DataLsb, bit5-1=Unused, bit0=NewDataFlag)
 05h Acc Y High (bit7-0=DataMsb)
 06h Acc Z Low (bit7-6=DataLsb, bit5-1=Unused, bit0=NewDataFlag)
 07h Acc Z High (bit7-0=DataMsb)
 08h Temperature (bit7-0=DataTempMsb) (Lsb not existing, except in Trimming?)
 09h Status Flags (see below)
 0Ah Control Flags (see below)
 0Bh Config Flags (see below)
 0Ch LG Threshold (bit7-0)
 0Dh LG Duration (bit7-0)
 0Eh HG Threshold (bit7-0)
 0Fh HG Duration (bit7-0)
 10h Any Motion Threshold (bit7-0)
 11h Misc Stuff (bit7-6=AnyMotionDur, bit5-3=HG Hyst, bit2-0=LG Hyst)
 12h Customer Reserved 1 (bit7-0)
 13h Customer Reserved 2 (bit7-0)
 14h Range/Bandwidth (bit7-5=Reserved, bit4-3=Range, bit2-0=Bandwidth)
 15h Misc Flags (see below)
 16h Trimming X Low (bit7-6=OffsetLsb, bit5-0=Gain)
 17h Trimming Y Low (bit7-6=OffsetLsb, bit5-0=Gain)
 18h Trimming Z Low (bit7-6=OffsetLsb, bit5-0=Gain)
 19h Trimming T Low (bit7-6=OffsetLsb, bit5-0=Gain)
 1Ah Trimming X High (bit7-0=OffsetMsb)
 1Bh Trimming Y High (bit7-0=OffsetMsb)
 1Ch Trimming Z High (bit7-0=OffsetMsb)
 1Dh Trimming T High (bit7-0=OffsetMsb)
 1Eh-22h BST reserved (official blank/green)
 23h BST reserved (official blank/white)
 24h-2Ah Not used (official gray/dither)
 2Bh-3Dh EEPROM Defaults for Registers 0Bh-1Dh
 3Eh-42h BST reserved (official blank/orange)
 43h-49h Not used (official gray/dither)
 4Ah-4Fh Not mentioned (official not here)
 50h-7Fh BST reserved (official blank/cyan) |
| --- |

Obscure Notes: Registers are 00h-06h,21h-22h,43h-7Fh are classifed as NOTHING,
07h-20h as IMAGE, and 23h-42h as EEPROM (whatever that crap means). Also,
Registers 00h-15h are OPERATIONAL, 16h-3Dh are DEFAULT SETTING, 3Eh-7Fh are
BOSCH SENSORTEC RESERVED.

**Register 09h - Status Flags**

```

| 7 ST Result
 6-5 Not used (official piss/dither)
 4 Alert Phase
 3 LG_latched
 2 HG_latched
 1 LG_status
 0 HG_status |
| --- |

**Register 0Ah - Control Flags**

```

| 7 Reserved (official gray/dither)
 6 Reset INT
 5 Update IMAGE
 4 EE_W (uh? maybe eeprom write?)
 3 Self Test 1
 2 Self Test 0
 1 Soft Reset
 0 Sleep |
| --- |

**Register 0Bh - Config Flags**

```

| 7 Alert
 6 Any Motion
 5-4 Counter HG
 3-2 Counter LG
 1 Enable HG
 0 Enable LG |
| --- |

**Register 15h - Misc Flags**

```

| 7 SPI4
 6 enable_adv_INT
 5 new_data_INT
 4 latch_INT
 3 shadow_dis
 2-1 wake_up_pause
 0 wake_up |
| --- |

| DS Cart Infrared Component Lists |
| --- |

**Gamecart for Walk with Me/Laufrhythmus**

```

| Case "Nintendo, NTR-031. PAT. PEND., IMWPN1J12"
 PCB "DA A-4 IRU01-10" (two layers) plus "IRL01-01 "(brown extra film layer)
 U1 32pin "S906748-1, SanDisk, 11014-64B, P0A837.00, 0843, NTR-IMWP-1" (ROM)
 U2 32pin "38600R, A06V, AH00167, 0832" (CPU, ROM 8Kbyte, RAM 0.5KByte)
 U3 5pin "?" (OR-gate? flipflop?) (for forwarding SPI /CS to FLASH /CS)
 U4 8pin "45PE80VG, HPAMZ V5, KOR 833X, ST e3" (SPI FLASH 1024 Kbytes)
 U1' 7pin "5 S.. 9" IR transceiver (on brown film layer)
 X1 6pin "737Wv" ;7.37MHz? /FLASH.CS --|""""|-- GND
 R1,R2,RA1 resistors | U3 |-- /SPI.CS (from NDS)
 C1,C2,C3,C4,C5,C6 capacitors VDD33 --|____|-- U2.pinxxx |
| --- |

Note: The printed part number on the CPU is 38600R, which does officially have
ROM 8Kbyte, RAM 0.5KByte (and the software does use only that much memory, but
it does actually contain twice as much ROM and RAM, ie. it seems to be a 38602R
chip... or, as the part number is wrong, it MIGHT even be a F38602 with FLASH
memory?).

IR transceiver wiring: 1=GND, 2=CPU.pin24=TXD, 3=CPU.pin25=RXD,
4=CPU.pin26=PWDOWN, 5=NC, 6=GND, 7=VCC.

**Gamecart for P-Letter Black**

```

| PCB "DI Y-1 IRC02-01" (two layers, without brown extra film layer)
 U1 32pin "MXIC..." (ROM)
 U2 32pin "..." (CPU, ROM 8Kbyte, RAM 0.5KByte)
 U3 5pin "..." (OR-gate? flipflop?) (for forwarding SPI /CS to FLASH /CS)
 U4 8pin "..." (SPI FLASH)
 U5 7pin "..." IR transceiver
 X1 6pin "..."
 R1,R2,RA1 resistors
 C1,C2,C3,C4,C5,C6 capacitors |
| --- |

More or less same as in Walk with Me, but without film layer, and with
non-legible part numbers on existing photos.

**Activity Meter (Actimeter in german) (Nintendo, 2008-2009)**

NTR-IMWJ Aruite Wakaru Seikatsu Rhythm (JP)

NTR-IMWE Personal Trainer: Walking (US)

NTR-IMWP Walk with Me (EU) (Laufrhythmus in german)

NTR-IA8P Active Health with Carol Vorderman (EU)

```

| Case "Nintendo DS, NTR-027, (C) 2008 Nintendo, NTR-A-HC, Made in Japan"
 Case "CE ./ VCI, ACN 060 566 083, Nintendo"
 PCB "NTR-DHC-01" (in water resistant case)
 Ux 32pin Side-A "38602R, F22V, AH04731, 0834" (CPU, ROM 16Kbyte, RAM 1KByte)
 U2 8pin Side-B "564X, 48H3, 30" (SPI EEPROM 8Kbyte, ST M95640-W or similar)
 U3 7pin Side-B "1 S. 9" IR transceiver
 ?? 2pin Side-A huge smd capacitor shaped thing, maybe analog 1-axis sensor?
 ?? 2pin Side-A huge smd capacitor shaped thing, maybe analog 1-axis sensor?
 Ux/Qx Side-A many small chips with 3-6 pins and few markings
 Xx 3pin Side-A "CB825" ;32.768-kHz or 38.4-kHz Crystal Resonator?
 Yx 6pin Side-A ":i] 3.68t" ;3.68MHz (115.2kHz*32)
 C1..C34 Plenty capacitors
 R1..R28 Plenty resistors
 BTI 2pin Side-B Battery holder (for CR2032 H, 3V)
 Button Side-A Push button (communication button)
 |< 4pin Side-A Two color LED |
| --- |

Activity Meter Instruction Booklet, 310 pages: "Do not disassemble or attempt
to repair the Activity Meter yourself. Doing so could result in injury or
electrocution."

IR transceiver wiring: 1=BAT+, 2=TX-RC=NC?, 3=1K-to-CPU.pin25=RXD,
4=CPU.pin26=PWDOWN, 5=CPU.pin24=TXD, 6=?, 7=GND.

**P-Walker (Nintendo, 2009)**

NTR-IPKx/NTR-IPGx P-letter HeartGold/SoulSilver

TWL-IRBO/TWL-IRAO P-letter Black/White

TWL-IREO/TWL-IRDO P-letter Black/White 2

```

| Case "?"
 PCB "NTR-PHC-01" (with green solder stop & unconventional black text layer)
 U1 32pin Side-B "F38606, F04V, AK04052, 0942" (CPU,FLASH 48Kbyte,RAM 2KByte)
 U2 4pin Side-A "?"
 U3 4pin Side-A "?"
 U4 4pin Side-A "M_RA"
 U5 7pin Side-B IR transceiver
 U6 8pin Side-A "Sxxxx, xxxx" (maybe SPI EEPROM?)
 U7 12pin Side-B "043, A939, 021" (accelerometer?) (Bosch BMA150 ?)
 U8 5pin Side-A "?"
 Q1 6pin Side-A "Z4"
 D1 3pin Side-A "?" dual diode or so
 X1 3pin Side-B "EAJJ" ;32.768-kHz or 38.4-kHz Crystal Resonator?
 Y1 6pin Side-B "3.68" ;3.68MHz (115.2kHz*32)
 BZ1 2pin Side-B wires to piezo speaker (aka buzzer)
 CN1 14pin Side-A LCD connector 14pin? or 2x14pin? (without backlight)
 (with SSD1850 display controller (or similar) inside of LCD screen)
 (96x64 2-bit greyscale screen) (reportedly with SPI bus)
 BT+/- Side-B Battery contacts for removeable battery (for CR2032, 3V)
 C1..C29 Plenty capacitors
 R1..R22 Plenty resistors
 SW's Side-A Three buttons (left, center, right) |
| --- |

**Fit Meter (for Wii Fit U) (2013?)**

```

| PCB "SAMU-01" (with green solder stop & unconventional black text layer)
 U1 40pin Side-B "R5F101EEA, 1242KE415, SINGAPORE" (RL78 CPU)
 U2 7pin Side-B "845G2947" IR transceiver, with metal shield
 U3 16pin Side-B (not installed)
 U4 16pin Side-A --UNKNOWN MARKING, BAD PHOTO-- maybe accelerometer
 U5 4pin Side-A --UNKNOWN MARKING, BAD PHOTO-- maybe SPI EEPROM/FLASH
 U6 4pin Side-A (not installed)
 U7 7pin Side-B "I357, U231, 094" whatever, in metal shielded case
 X1 3pin Side-B "EABL" crystal or so
 BZ1 2pin Side-B wires to piezo speaker (aka buzzer)
 CN1 14pin Side-A LCD connector 14pin? or 2x14pin? (without backlight)
 BT+/- Side-B Battery contacts for removeable battery
 Q1 3pin Side-A Transistor or so
 D1..D2 Side-A Diodes (3pin each)
 C1..C29 Plenty capacitors
 R1..R22 Plenty resistors
 SW's Side-A Three buttons (left, center, right) |
| --- |

Similar as P-Walker, but uses a RL78 CPU (alike 3DS MCU) instead of H8/386.

The IR transceivers do resemble RPM841-H16 or similar (as used in 3DS). Observe
that 7pin transceiver pinouts are NOT same. Walk with Me, Activity Meter, and
3DS all have different 7pin transceiver wirings (and, some have additional
metal shields).

| H8/386 SFRs |
| --- |

**H8/386 Special Function Registers at F000h and up**

```

| F020h FLMCR1 FLASH Memory Control 1
 F021h FLMCR2 FLASH Memory Control 2
 F022h FLPWCR FLASH Memory Power Control
 F023h EBR1 FLASH Erase Block 1
 F02Bh FENR FLASH Memory Enable
 F067h RTCFLG RTC Interrupt Flag
 F068h RSECDR RTC Seconds / Free running counter
 F069h RMINDR RTC Minutes
 F06Ah RHRDR RTC Hours
 F06Bh RWKDR RTC Day-of-week
 F06Ch RTCCR1 RTC Control 1
 F06Dh RTCCR2 RTC Control 2
 F06Fh RTCCSR RTC Clock Source Select
 F078h ICCR1 I2C Bus Control 1
 F079h ICCR2 I2C Bus Control 2
 F07Ah ICMR I2C Bus Mode
 F07Bh ICIER I2C Bus Interrupt Enable
 F07Ch ICSR I2C Bus Status
 F07Dh SAR I2C Slave Address
 F07Eh ICDRT I2C Bus Transmit Data
 F07Fh ICDRR I2C Bus Receive Data
 F085h PFCR System Port Function Control
 F086h PUCR8 Port 8 Pull-up Control
 F087h PUCR9 Port 9 Pull-up Control
 F08Ch PODR9 Port 9 Open-drain Control
 F0D0h TMB1 Timer B1 Mode
 F0D1h TC/LB1 Timer B1 Counter (R) / Load (W)
 F0DCh CMCR0 Compare Control 0
 F0DDh CMCR1 Compare Control 1
 F0DEh CMDR Compare Data
 F0E0h SSCRH SPI Synchronous Serial Control H (AccessState3)
 F0E1h SSCRL SPI Synchronous Serial Control L (AccessState3)
 F0E2h SSMR SPI Synchronous Serial Mode (AccessState3)
 F0E3h SSER SPI Synchronous Serial Enable (AccessState3)
 F0E4h SSSR SPI Synchronous Serial Status (AccessState3)
 F0E9h SSRDR SPI Synchronous Serial Receive Data (AccessState3)
 F0EBh SSTDR SPI Synchronous Serial Transmit Data (AccessState3)
 F0F0h TMRW Timer W Mode
 F0F1h TCRW Timer W Control
 F0F2h TIERW Timer W Interrupt Enable
 F0F3h TSRW Timer W Status
 F0F4h TIOR0 Timer W I/O control 0
 F0F5h TIOR1 Timer W I/O control 1
 F0F6h TCNT Timer W Counter (16bit)
 F0F8h GRA Timer W General A (16bit)
 F0FAh GRB Timer W General B (16bit)
 F0FCh GRC Timer W General C (16bit)
 F0FEh GRD Timer W General D (16bit) |
| --- |

**H8/386 Special Function Registers at FF80h and up, with [FFnn] & [FFnn].bitno**

```

| FF8Ch ECPWCR Async Event Counter PWM Compare (16bit)
 FF8Eh ECPWDR Async Event Counter PWM Data (16bit)
 FF91h SPCR IrDA UART Serial 3 Port Control
 FF92h AEGSR Async Event Input Pin Edge Select
 FF94h ECCR Async Event Counter Control
 FF95h ECCSR Async Event Counter Control/Status
 FF96h ECH Async Event Counter H
 FF97h ECL Async Event Counter L
 FF98h SMR3 IrDA UART Serial 3 Mode (AccessState3)
 FF99h BRR3 IrDA UART Serial 3 Bit Rate (AccessState3)
 FF9Ah SCR3 IrDA UART Serial 3 Control (AccessState3)
 FF9Bh TDR3 IrDA UART Serial 3 Transmit Data (AccessState3)
 FF9Ch SSR3 IrDA UART Serial 3 Status (AccessState3)
 FF9Dh RDR3 IrDA UART Serial 3 Receive Data (AccessState3)
 FFA6h SEMR IrDA UART Serial 3 Extended Mode (AccessState3)
 FFA7h IrCR IrDA Control
 FFB0h TMWD Timer WD Watchdog Mode
 FFB1h TCSRWD1 Timer WD Watchdog Control/Status 1
 FFB2h TCSRWD2 Timer WD Watchdog Control/Status 2
 FFB3h TCWD Timer WD Watchdog Counter
 FFBCh ADRR A/D Converter Result (16bit)
 FFBEh AMR A/D Converter Mode
 FFBFh ADSR A/D Converter Start
 FFC0h PMR1 Port 1 Mode
 FFC2h PMR3 Port 3 Mode
 FFCAh PMRB Port B Mode
 FFD4h PDR1 Port 1 Data
 FFD6h PDR3 Port 3 Data
 FFDBh PDR8 Port 8 Data
 FFDCh PDR9 Port 9 Data
 FFDEh PDRB Port B Data
 FFE0h PUCR1 Port 1 Pull-up Control
 FFE1h PUCR3 Port 3 Pull-up Control
 FFE4h PCR1 Port 1 Control
 FFE6h PCR3 Port 3 Control
 FFEBh PCR8 Port 8 Control
 FFECh PCR9 Port 9 Control
 FFF0h SYSCR1 System Control 1
 FFF1h SYSCR2 System Control 2
 FFF2h IEGR Interrupt Edge Select
 FFF3h IENR1 Interrupt Enable 1
 FFF4h IENR2 Interrupt Enable 2
 FFF5h OSCCR System Oscillator Control
 FFF6h IRR1 Interrupt Flag 1
 FFF7h IRR2 Interrupt Flag 2
 FFFAh CKSTPR1 Clock Stop 1
 FFFBh CKSTPR2 Clock Stop 2 |
| --- |

| H8/386 Exception Vectors |
| --- |

**H8/386 Exception Vectors (Vector 0000h has highest priority, 004Eh lowest)**

Below are for Normal Mode with 16bit addressing (Extended Mode has 32bit
vectors at 0000h..009Fh accordingly).

```

| 0000h Reset/Watchdog
 0002h Reserved
 0004h Reserved
 0006h Reserved
 0008h Reserved
 000Ah Reserved
 000Ch Reserved
 000Eh External NMI interrupt
 0010h Trap 0 opcode
 0012h Trap 1 opcode
 0014h Trap 2 opcode
 0016h Trap 3 opcode
 0018h Reserved
 001Ah CPU Direct transition by executing SLEEP
 001Ch Reserved
 001Eh Reserved
 0020h External IRQ0 interrupt
 0022h External IRQ1 interrupt
 0024h External IRQAEC interrupt
 0026h Reserved
 0028h Reserved
 002Ah Comparator COMP0
 002Ch Comparator COMP1
 002Eh RTC per 0.25 seconds (4Hz) ;0.25-second overflow
 0030h RTC per 0.5 seconds (2Hz) ;0.5-second overflow
 0032h RTC per second (1Hz) ;Second periodic overflow
 0034h RTC per minute ;Minute periodic overflow
 0036h RTC per hour ;Hour periodic overflow
 0038h RTC per day ;Day-of-week periodic overflow
 003Ah RTC per week (7 days) ;Week periodic overflow
 003Ch RTC Free-running overflow
 003Eh WDT overflow (interval timer)
 0040h Asynchronous event counter overflow
 0042h Timer B1 Overflow
 0044h Serial SPI (or IIC2) (aka I2C ?)
 0046h Timer W Overflow or Capture/compare A,B,C,D
 0048h Reserved
 004Ah IrDA UART Serial 3
 004Ch A/D Conversion end
 004Eh Reserved |
| --- |

Note: The SSU (aka SPI) and IIC (aka I2C) share the same vector address. When
using the IIC, shift the SSU to standby mode using CKSTPR2.

| H8/300H Operands |
| --- |

**CPU Registers**

```

| R0..R6 32bit General Purpose ;\can be alternately used as
 R7 (SP) 32bit Stack Pointer ;/8bit/16bit registers (see below)
 PC 24bit Program Counter
 CCR 8bit Flags (occupies 16bit when pushed/stored in memory) |
| --- |

Registers R0..R7 can be split into 8bit/16bit registers (alike 80x86
registers):

```

| .-----------------------.
 | ERx | 32bit (ERx)
 |-----------+-----------|
 | Ex | Rx | 16bit (Rx)
 '-----------+-----+-----|
 ' RxH | RxL | 8bit (RxB)
 '-----'-----' |
| --- |

There are no opcodes for splitting upper 16bit in Ex into 8bit ExL,ExH (except,
the sign/zero-extend opcodes seem to allow to extend ExL to Ex).

**Normal and Extended Mode**

The CPU can be wired to two different addressing modes:

```

| Normal Mode --> 16bit addressing (default)
 Extended Mode --> 24bit addressing |
| --- |

Normal mode is usually used (the CPU has less than 64K ROM/RAM anyways), the
opcodes with [er0..er7] memory addressing are then actually accessing [r0..r7],
ie. the upper 16bit in e0..e7 are ignored (and can be used for general purpose
data).

Extended mode, if it were ever used, uses the lower 24bit of er0..er7 for
[er0..er7] addressing, and CALL/RET and indirect JMP opcodes are a bit slower,
and exception vectors are 32bit wide.

**Memory Addressing**

Data is stored in Big-Endian. 16bit/32bit values must be stored at even
addresses (with bit0 cleared) (there is no need to clear bit1 for 32bit values,
namely, push/pop work regardless of bit1).

```

| Native Nocash
 @aa:8 [FFaa] Memory at FF00h..FFFFh (upper RAM and SFR's)
 @aa:16 [nnnn]
 @aa:24 [nnnnnn]
 @Erm [Erm]
 @(d:16,ERm) [ERm+nnnn]
 @(d:24,ERm) [ERm+nnnnnn]
 @ERm+ [ERm+] Memory access with post-increment
 @-ERm [ERm-] Memory access with pre-decrement
 (implied) [ER6+],[ER5+] Memory block transfer (EEPMOV) |
| --- |

**Bit Addressing**

```

| Native Nocash
 #nn:8,@aa:8 [FFaa].n
 RnB,@aa:8 [FFaa].RnB
 #nn:8,RdB RdB.n
 RnB,RdB RdB.RnB |
| --- |

Note: The "#nn:8" suggests native syntax to use "#0x80" to select bit7,
however, the existing disassembler does instead use "#7".

| H8/300H Opcodes |
| --- |

All opcodes are multiples of 2 bytes (2,4,6,8,10 bytes), all opcodes should be
always located at even addresses (ie. bit0 of jump address/disp operands should
be always 0).

**Register Encoding**

```

| 0..7 8bit Registers R0H..R7H (bit8-15) ;\RxB
 8..F 8bit Registers R0L..R7L (bit0-7) ;/
 0..7 16bit Registers R0..R7 (bit0-15) ;\Rx
 8..F 16bit Registers E0..E7 (bit16-31) ;/
 0..7 32bit Registers ER0..ER7 (bit0-31) ;-ERx (in normal opcodes)
 8..F 32bit Registers ER0..ER7 (bit0-31) ;-ERx (in opcodes marked *m,*s) |
| --- |

**H8/300H Main Opcodes**

```

| Opcode Native Nocash States IxHUNZVC
 0.. --> Misc 0xxx
 1.. --> Misc 1xxx
 2dnn MOV.B @aa:8,RdB MOV.B RdB,[FFaa] 4 ----nz0-
 3snn MOV.B Rs,@aa:8 MOV.B [FFaa],Rs 4 ----nz0-
 4cnn --> Jumps (relative 8bit range)
 5.. --> Jumps (various) and unsigned mul/div
 6.. --> Misx 6xxx
 7.. --> Misc 7xxx
 8dnn ADD.B #nn:8,RdB ADD.B RdB,nn 2 --h-nzvc
 9dnn ADDX #nn:8,RdB ADC.B RdB,nn 2 --h-nzvc
 Adnn CMP.B #nn:8,RdB CMP.B RdB,nn 2 --h-nzvc
 Bdnn SUBX #nn:8,RdB SBC.B RdB,nn 2 --h-nzvc
 Cdnn OR.B #nn:8,RdB OR.B RdB,nn 2 ----nz0-
 Ddnn XOR.B #nn:8,RdB XOR.B RdB,nn 2 ----nz0-
 Ednn AND.B #nn:8,RdB AND.B RdB,nn 2 ----nz0-
 Fdnn MOV.B #nn:8,RdB MOV.B RdB,nn 2 ----nz0- |
| --- |

**H8/300H Misc 0xxx**

```

| 0000 NOP NOP 2 --------
 01.. --> Misc 01xx ;Memory Load/Store (32bit ERn) etc.
 020d STC.B CCR,RdB MOV.B RdB,CCR 2 --------
 030s LDC.B RsB,CCR MOV.B CCR,RsB 2 xxxxxxxx
 04nn ORC #nn:8,CCR OR.B CCR,nn 2 xxxxxxxx
 05nn XORC #nn:8,CCR XOR.B CCR,nn 2 xxxxxxxx
 06nn ANDC #nn:8,CCR AND.B CCR,nn 2 xxxxxxxx
 07nn LDC.B #nn:8,CCR MOV.B CCR,nn 2 xxxxxxxx
 08sd ADD.B RsB,RdB ADD.B RdB,RsB 2 --h-nzvc
 09sd ADD.W Rs,Rd ADD.W Rd,Rs 2 --h-nzvc
 0A.. --> Increment/Add
 0B.. --> Increment/Add
 0Csd MOV.B RsB,RdB MOV.B RdB,RsB 2 ----nz0-
 0Dsd MOV.W Rs,Rd MOV.W Rd,Rs 2 ----nz0-
 0Esd ADDX RsB,RdB ADC.B RdB,RsB 2 --h-nzvc
 0F0d DAA RdB DAA.B RdB 2 --U-nzUc
 0Fsd *s MOV.L ERs,ERd MOV.L ERd,ERs 2 ----nz0- |
| --- |

**H8/300H Misc 01xx ;Memory Load/Store (32bit ERn) etc.**

```

| 010069md MOV.L @ERm,ERd MOV.L ERd,[ERm] 8 ----nz0-
 014069m0 LDC.W @ERm,CCR MOV.W CCR,[ERm] 6 xxxxxxxx
 010069ms *m MOV.L ERs,@ERm MOV.L [ERm],ERs 8 ----nz0-
 014069m0 *m STC.W CCR,@ERm MOV.W [ERm],CCR 6 --------
 01006B0dnnnn MOV.L @aa:16,ERd MOV.L ERd,[nnnn] 10 ----nz0-
 01406B00nnnn LDC.W @aa:16,CCR MOV.W CCR,[nnnn] 8 xxxxxxxx
 01006B2d00nnnnnn MOV.L @aa:24,ERd MOV.L ERd,[nnnnnn] 12 ----nz0-
 01406B2000nnnnnn LDC.W @aa:24,CCR MOV.W CCR,[nnnnnn] 10 xxxxxxxx
 01006B8snnnn MOV.L ERs,@aa:16 MOV.L [nnnn],ERs 10 ----nz0-
 01406B80nnnn STC.W CCR,@aa:16 MOV.W [nnnn],CCR 8 --------
 01006BAs00nnnnnn MOV.L ERs,@aa:24 MOV.L [nnnnnn],ERs 12 ----nz0-
 01406BA000nnnnnn STC.W CCR,@aa:24 MOV.W [nnnnnn],CCR 10 --------
 01006Dmd MOV.L @ERm+,ERd MOV.L ERd,[ERm+] 10 ----nz0-
 01406Dm0 LDC.W @ERm+,CCR MOV.W CCR,[ERm+] 8 xxxxxxxx
 01006Dms *m MOV.L ERs,@-ERm MOV.L [ERm-],ERs 10 ----nz0-
 01406Dm0 *m STC.W CCR,@-ERm MOV.W [ERm-],CCR 8 --------
 01006Fmdnnnn MOV.L @(d:16,ERm),ERd MOV.L ERd,[ERm+nnnn] 10 ----nz0-
 01406Fm0nnnn LDC.W @(d:16,ERm),CCR MOV.W CCR,[ERm+nnnn] 8 xxxxxxxx
 01006Fmsnnnn *m MOV.L ERs,@(d:16,ERm) MOV.L [ERm+nnnn],ERs 10 ----nz0-
 01406Fm0nnnn *m STC.W CCR,@(d:16,ERm) MOV.W [ERm+nnnn],CCR 8 --------
 010078m06B2d00.. MOV.L @(d:24,ERm),ERd MOV.L ERd,[ERm+nnnnnn] 14 ----nz0-
 014078m06B2000.. LDC.W @(d:24,ERm),CCR MOV.W CCR,[ERm+nnnnnn] 12 xxxxxxxx
 010078m06BAs00..*? MOV.L ERs,@(d:24,ERm) MOV.L [ERm+nnnnnn],ERs 14 ----nz0-
 014078m06BA000.. STC.W CCR,@(d:24,ERm) MOV.W [ERm+nnnnnn],CCR 12 --------
 0180 SLEEP HALT 2 --------
 01C050sd MULXS.B RsB,Rd SMUL.B Rd,RsB 16 ----nz--
 01C052sd MULXS.W Rs,ERd SMUL.W ERd,Rs 24 ----nz--
 01D051sd DIVXS.B RsB,Rd SDIV.B Rd,RsB 16 ----nz--
 01D053sd DIVXS.W Rs,ERd SDIV.W ERd,Rs 24 ----nz--
 01F064sd OR.L ERs,ERd OR.L ERd,ERs 4 ----nz0-
 01F065sd XOR.L ERs,ERd XOR.L ERd,ERs 4 ----nz0-
 01F066sd AND.L E?Rs,ERd AND.L ERd,ERs 4 ----nz0- |
| --- |

**H8/300H Misc 1xxx**

```

| 10.. --> Shift/Rotate (shift left)
 11.. --> Shift/Rotate (shift right)
 12.. --> Shift/Rotate (rotate left)
 13.. --> Shift/Rotate (rotate right)
 14sd OR.B RsB,RdB OR.B RdB,RsB 2 ----nz0-
 15sd XOR.B RsB,RdB XOR.B RdB,RsB 2 ----nz0-
 16sd AND.B RsB,RdB AND.B RdB,RsB 2 ----nz0-
 17.. --> Not/Neg/Extend
 18sd SUB.B RsB,RdB SUB.B RdB,RsB 2 --h-nzvc
 19sd SUB.W Rs,Rd SUB.W Rd,Rs 2 --h-nzvc
 1A.. --> Decrement/Subtract
 1B.. --> Decrement/Subtract
 1Csd CMP.B RsB,RdB CMP.B RdB,RsB 2 --h-nzvc
 1Dsd CMP.W Rs,Rd CMP.W Rd,Rs 2 --h-nzvc
 1Esd SUBX RsB,RdB SBC.B RdB,RsB 2 --h-nzvc
 1F0d DAS RdB DAS.B RdB 2 --U-nzU?
 1Fsd *s CMP.L ERs,ERd CMP.L ERd,ERs 2 --h-nzvc |
| --- |

**H8/300H Shift/Rotate (Shift Logical/Arithmetic, Rotate through Carry or not)**

```

| 100d SHLL.B RdB SHL.B RdB 2 ----nz0c
 101d SHLL.W Rd SHL.W Rd 2 ----nz0c
 103d SHLL.L ERd SHL.L ERd 2 ----nz0c
 108d SHAL.B RdB SAL.B RdB 2 ----nzvc
 109d SHAL.W Rd SAL.W Rd 2 ----nzvc
 10Bd SHAL.L ERd SAL.L ERd 2 ----nzvc
 110d SHLR.B RdB SHR.B RdB 2 ----0z0c
 111d SHLR.W Rd SHR.W Rd 2 ----0z0c
 113d SHLR.L ERd SHR.L ERd 2 ----0z0c
 118d SHAR.B RdB SAR.B RdB 2 ----nz0c
 119d SHAR.W Rd SAR.W Rd 2 ----nz0c
 11Bd SHAR.L ERd SAR.L ERd 2 ----nz0c
 120d ROTXL.B RdB RCL.B RdB 2 ----nz0c
 121d ROTXL.W Rd RCL.W Rd 2 ----nz0c
 123d ROTXL.L ERd RCL.L ERd 2 ----nz0c
 128d ROTL.B RdB ROL.B RdB 2 ----nz0c
 129d ROTL.W Rd ROL.W Rd 2 ----nz0c
 12Bd ROTL.L ERd ROL.L ERd 2 ----nz0c
 130d ROTXR.B RdB RCR.B RdB 2 ----nz0c
 131d ROTXR.W Rd RCR.W Rd 2 ----nz0c
 133d ROTXR.L ERd RCR.L ERd 2 ----nz0c
 138d ROTR.B RdB ROR.B RdB 2 ----nz0c
 139d ROTR.W Rd ROR.W Rd 2 ----nz0c
 13Bd ROTR.L ERd ROR.L ERd 2 ----nz0c |
| --- |

**H8/300H Not/Neg/Extend**

```

| 170d NOT.B RdB NOT.B RdB 2 ----nz0-
 171d NOT.W Rd NOT.W Rd 2 ----nz0-
 173d NOT.L Rd NOT.L ERd 2 ----nz0-
 175d EXTU.W Rd UMOV Rd,RdL ;or Ed,EdL? 2 ----0z0-
 177d EXTU.L ERd UMOV ERd,Rd 2 ----0z0-
 178d NEG.B RdB NEG.B RdB 2 --h-nzvc
 179d NEG.W Rd NEG.W Rd 2 --h-nzvc
 17Bd NEG.L Rd NEG.L ERd 2 --h-nzvc
 17Dd EXTS.W Rd SMOV Rd,RdL ;or Ed,EdL? 2 ----nz0-
 17Fd EXTS.L ERd SMOV ERd,Rd 2 ----nz0- |
| --- |

**H8/300H Increment/Add and Decrement/Subtract**

```

| 0A0d INC.B RdB INC.B RdB,1 2 ----nzv-
 1A0d DEC.B RdB DEC.B RdB,1 2 ----nzv-
 0Asd *s ADD.L E?Rs,ERd ADD.L ERd,ERs 2 --h-nzvc
 1Asd *s SUB.L ERs,ERd SUB.L ERd,ERs 2 --h-nzvc
 0B0d ADDS #1,ERd INC.S ERd,1 2 --------
 1B0d SUBS #1,ERd DEC.S ERd,1 2 --------
 0B5d INC.W #1,Rd INC.W Rd,1 2 ----nzv-
 1B5d DEC.W #1,Rd DEC.W Rd,1 2 ----nzv-
 0B7d INC.L #1,ERd INC.L ERd,1 2 ----nzv-
 1B7d DEC.L #1,ERd DEC.L ERd,1 2 ----nzv-
 0B8d ADDS #2,ERd INC.S ERd,2 2 --------
 1B8d SUBS #2,ERd DEC.S ERd,2 2 --------
 0B9d ADDS #4,ERd INC.S ERd,4 2 --------
 1B9d SUBS #4,ERd DEC.S ERd,4 2 --------
 0BDd INC.W #2,Rd INC.W Rd,2 2 ----nzv-
 1BDd DEC.W #2,Rd DEC.W Rd,2 2 ----nzv-
 0BFd INC.L #2,ERd INC.L ERd,2 2 ----nzv-
 1BFd DEC.L #2,ERd DEC.L ERd,2 2 ----nzv- |
| --- |

**H8/300H Jump Opcodes and Unsigned Mul/Div**

```

| 50sd MULXU.B RsB,Rd UMUL.B Rd,RsB 14 ----nz--
 51sd DIVXU.B RsB,Rd UDIV.B Rd,RsB 14 ----nz--
 52sd MULXU.W Rs,ERd UMUL.W ERd,Rs 22 ----nz--
 53sd DIVXU.W Rs,ERd UDIV.W ERd,Rs 22 ----nz--
 5470 RTS RET 8,10 --------
 55nn BSR d:8 CALL $+/-nn 6,8 --------
 5670 RTE RETI 10 xxxxxxxx
 57n0 TRAPA #n:2 TRAP 0..3 ;[0010h+n*2] 14 1x------
 58c0nnnn --> Jumps (relative 16bit range)
 59s0 JMP @ERs JMP ERs 4 --------
 5Annnnnn JMP @aa:24 JMP nnnnnn 6 --------
 5Baa JMP @@aa:8 JMP [FFaa] 8,10 --------
 5C00nnnn BSR d:16 CALL $+/-nnnn 8,10 --------
 5Ds0 JSR @ERs CALL ERs 6,8 --------
 5Ennnnnn JSR @aa:24 CALL nnnnnn 8,10 --------
 5Faa JSR @@aa:8 CALL [FFaa] 8,12 -------- |
| --- |

**H8/300H Relative Jump Opcodes (8bit/16bit range)**

```

| 4cnn Bcc d:8 Jcc $+/-nn 4 --------
 58c0nnnn Bcc d:16 Jcc $+/-nnnn 6 -------- |
| --- |

The 4bit condition code can be:

```

| 0 BRA or BT JMP ;always/true
 1 BRN or BF - ;never/false
 2 BHI JA ;unsigned-above
 3 BLS JBE ;unsigned-below-equal
 4 BCC or BHS JNC or JAE ;unsigned-above-equal
 5 BCS or BLO JC or JB ;unsigned-below
 6 BNE JNZ or JNE ;not equal/zero
 7 BEQ JZ or JE ;equal/zero
 8 BVC JNO ;signed-no overflow
 9 BVS JO ;signed-n-overflow
 A BPL JNS ;signed-n-plus
 B BMI JS ;signed-n-minus
 C BGE JGE ;signed-n-greater-eq
 D BLT JL ;signed-n-less
 E BGT JG ;signed-n-greater
 F BLE JLE ;signed-n-less-equal |
| --- |

Destination address should be equal (although, the opcodes are weirdly using
byte offsets, hence limiting 8bit range to even 7bit range).

The execution time for condition=false is unknown. The time for BRN (always
false) is said to be equal to two NOPs (but unknown why one would use that
opcode, and if it does refer to the BRN opcode with 8bit and/or 16bit range).

**H8/300H Misc 6xxx**

```

| 60nd BSET RnB,RdB SET RdB.RnB 2 --------
 61nd BNOT RnB,RdB NOT RdB.RnB 2 --------
 62nd BCLR RnB,RdB CLR RdB.RnB 2 --------
 63nd BTST RnB,RdB TST RdB.RnB 2 -----z--
 64sd OR.W Rs,Rd OR.W Rd,Rs 2 ----nz0-
 65sd XOR.W Rs,Rd XOR.W Rd,Rs 2 ----nz0-
 66sd AND.W Rs,Rd AND.W Rd,Rs 2 ----nz0-
 67nd *i B{I}ST #nn:8,RdB MOV RdB.n,{not} C 2 --------
 68md MOV.B @ERm,RdB MOV.B RdB,[ERm] 4 ----nz0-
 68ms *m MOV.B RsB,@ERm MOV.B [ERm],RsB 4 ----nz0-
 69md MOV.W @ERm,Rd MOV.W Rd,[ERm] 4 ----nz0-
 69ms *m MOV.W Rs,@ERm MOV.W [ERm],Rs 4 ----nz0-
 6A0dnnnn MOV.B @aa:16,RdB MOV.B RdB,[aaaa] 6 ----nz0-
 6A2d00nnnnnn MOV.B @aa:24,RdB MOV.B RdB,[aaaaaa] 8 ----nz0-
 6A4dnnnn MOVFPE @aa:16,RdB MOV.B RdB,[periph:aaaa] * ----nz0-
 6A8snnnn MOV.B RsB,@aa:16 MOV.B [aaaa],RsB 6 ----nz0-
 6AAs00nnnnnn MOV.B RsB,@aa:24 MOV.B [aaaaaa],RsB 8 ----nz0-
 6ACsnnnn MOVTPE RsB,@aa:16 MOV.B [periph:aaaa],RsB * ----nz0-
 6B0dnnnn MOV.W @aa:16,Rd MOV.W Rd,[aaaa] 6 ----nz0-
 6B2d00nnnnnn MOV.W @aa:24,Rd MOV.W Rd,[aaaaaa] 8 ----nz0-
 6B8snnnn MOV.W Rs,@aa:16 MOV.W [aaaa],Rs 6 ----nz0-
 6BAs00nnnnnn MOV.W Rs,@aa:24 MOV.W [aaaaaa],Rs 8 ----nz0-
 6Cmd MOV.B @ERm+,RdB MOV.B RdB,[ERm+] 6 ----nz0-
 6Cms *m MOV.B RsB,@-ERm MOV.B [ERm-],RsB 6 ----nz0-
 6Dmd MOV.W @ERm+,RdB MOV.W RdB,[ERm+] 6 ----nz0-
 6Dms *m MOV.W RsB,@-ERm MOV.W [ERm-],RsB 6 ----nz0-
 6Emdnnnn MOV.B @(d:16,ERm),RdB MOV.B RdB,[ERm+nnnn] 6 ----nz0-
 6Emsnnnn *m MOV.B RsB,@(d:16,ERm) MOV.B [ERm+nnnn],RsB 6 ----nz0-
 6Fmdnnnn MOV.W @(d:16,ERm),Rd MOV.W Rd,[ERm+nnnn] 6 ----nz0-
 6Fmsnnnn *m MOV.W Rs,@(d:16,ERm) MOV.W [ERm+nnnn],Rs 6 ----nz0- |
| --- |

**H8/300H Misc 7xxx**

```

| 70nd BSET #nn:8,RdB SET RdB.n 2 --------
 71nd BNOT #nn:8,RdB NOT RdB.n 2 --------
 72nd BCLR #nn:8,RdB CLR RdB.n 2 --------
 73nd BTST #nn:8,RdB TST RdB.n 2 -----z--
 74nd *i B{I}OR #nn:8,RdB OR C,{not} RdB.n 2 -------c
 75nd *i B{I}XOR #nn:8,RdB XOR C,{not} RdB.n 2 -------c
 76nd *i B{I}AND #nn:8,RdB AND C,{not} RdB.n 2 -------c
 77nd *i B{I}LD #nn:8,RdB MOV C,{not} RdB.n 2 -------c
 78m06A2d00nnnnnn MOV.B @(d:24,ERm),RdB MOV.B RdB,[ERm+nnnnnn] 10 ----nz0-
 78m06AAs00nnnnnn MOV.B RsB,@(d:24,ERm) MOV.B [ERm+nnnnnn],RsB 10 ----nz0-
 78m06B2d00nnnnnn MOV.W @(d:24,ERm),Rd MOV.W Rd,[ERm+nnnnnn] 10 ----nz0-
 78m06BAs00nnnnnn*? MOV.W Rs,@(d:24,ERm) MOV.W [ERm+nnnnnn],Rs 10 ----nz0-
 79.. --> Immediate (16bit)
 7A.. --> Immediate (32bit)
 7B5C498F EEPMOV.B MOV [ER6+],[ER5+],R4L- 8+4n --------
 7BD4598F EEPMOV.W MOV [ER6+],[ER5+],R4- 8+4n --------
 7C.. --> Bit Operations (Memory at ERm)
 7D.. --> Bit Operations (Memory at ERm)
 7E.. --> Bit Operations (Memory at FFaa)
 7F.. --> Bit Operations (Memory at FFaa) |
| --- |

The EEPMOV opcodes were originally intended to write to an EEPROM, but they can
be used as generic memory transfers with number of bytes in R4L or R4. EEPMOV.B
is blocking IRQs and NMIs. EEPMOV.W is also blocking IRQs, however, EEPMOV.W
gets aborted upon NMI (without resuming the transfer upon return, so software
must manually retry if R4=nonzero after executing EEPMOV.W in combination with
NMI sources).

**H8/300H Immediate 16bit/32bit**

```

| 790dnnnn MOV.W #nnnn:16,Rd MOV.W Rd,nnnn 4 ----nz0-
 791dnnnn ADD.W #nnnn:16,Rd ADD.W Rd,nnnn 4 --h-nzvc
 792dnnnn CMP.W #nnnn:16,Rd CMP.W Rd,nnnn 4 --h-nzvc
 793dnnnn SUB.W #nnnn:16,Rd SUB.W Rd,nnnn 4 --h-nzvc
 794dnnnn OR.W #nnnn:16,Rd OR.W Rd,nnnn 4 ----nz0-
 795dnnnn XOR.W #nnnn:16,Rd XOR.W Rd,nnnn 4 ----nz0-
 796dnnnn AND.W #nnnn:16,Rd AND.W Rd,nnnn 4 ----nz0-
 7A0dnnnnnnnn MOV.L #nnnnnnnn:32,E?Rd MOV.L E?Rd,nnnnnnnn 6 ----nz0-
 7A1dnnnnnnnn ADD.L #nnnnnnnn:32,ERd ADD.L ERd,nnnnnnnn 6 --h-nzvc
 7A2dnnnnnnnn CMP.L #nnnnnnnn:32,ERd CMP.L ERd,nnnnnnnn 6 --h-nzvc
 7A3dnnnnnnnn SUB.L #nnnnnnnn:32,ERd SUB.L ERd,nnnnnnnn 6 --h-nzvc
 7A4dnnnnnnnn OR.L #nnnnnnnn:32,ERd OR.L ERd,nnnnnnnn 6 ----nz0-
 7A5dnnnnnnnn XOR.L #nnnnnnnn:32,ERd XOR.L ERd,nnnnnnnn 6 ----nz0-
 7A6dnnnnnnnn AND.L #nnnnnnnn:32,ERd AND.L ERd,nnnnnnnn 6 ----nz0- |
| --- |

**H8/300H Bit Operations**

```

| 7Cm074n0 *i B{I}OR #nn:8,@ERm OR C,{not} [ERm].n 6 -------c
 7Cm075n0 *i B{I}XOR #nn:8,@ERm XOR C,{not} [ERm].n 6 -------c
 7Cm076n0 *i B{I}AND #nn:8,@ERm AND C,{not} [ERm].n 6 -------c
 7Cm077n0 *i B{I}LD #nn:8,@ERm MOV C,{not} [ERm].n 6 -------c
 7Dm060n0 BSET RnB,@ERm SET [ERm].RnB 8 --------
 7Dm061n0 BNOT RnB,@ERm NOT [ERm].RnB 8 --------
 7Dm062n0 BCLR RnB,@ERm CLR [ERm].RnB 8 --------
 7Dm063n0 BTST RnB,@ERm TST [ERm].RnB 8 -----z--
 7Dm067n0 *i B{I}ST #nn:8,@ERm MOV [ERm].n,{not} C 8 --------
 7Dm070n0 BSET #nn:8,@ERm SET [ERm].n 8 --------
 7Dm071n0 BNOT #nn:8,@ERm NOT [ERm].n 8 --------
 7Dm072n0 BCLR #nn:8,@ERm CLR [ERm].n 8 --------
 7Dm073n0 BTST #nn:8,@ERm TST [ERm].n 8 -----z--
 7Eaa74n0 *i B{I}OR #nn:8,@aa:8 OR C,{not} [FFaa].n 6 -------c
 7Eaa75n0 *i B{I}XOR #nn:8,@aa:8 XOR C,{not} [FFaa].n 6 -------c
 7Eaa76n0 *i B{I}AND #nn:8,@aa:8 AND C,{not} [FFaa].n 6 -------c
 7Eaa77n0 *i B{I}LD #nn:8,@aa:8 MOV C,{not} [FFaa].n 6 -------c
 7Faa60n0 BSET RnB,@aa:8 SET [FFaa].RnB 8 --------
 7Faa61n0 BNOT RnB,@aa:8 NOT [FFaa].RnB 8 --------
 7Faa62n0 BCLR RnB,@aa:8 CLR [FFaa].RnB 8 --------
 7Faa63n0 BTST RnB,@aa:8 TST [FFaa].RnB 8 -----z--
 7Faa67n0 *i B{I}ST #nn:8,@aa:8 MOV [FFaa].n,{not} C 8 --------
 7Faa70n0 BSET #nn:8,@aa:8 SET [FFaa].n 8 --------
 7Faa71n0 BNOT #nn:8,@aa:8 NOT [FFaa].n 8 --------
 7Faa72n0 BCLR #nn:8,@aa:8 CLR [FFaa].n 8 --------
 7Faa73n0 BTST #nn:8,@aa:8 TST [FFaa].n 8 -----z-- |
| --- |

Nintendo uses undocumented opcode 7Eaa73n0 instead of official 7Faa73n0.
Unknown if both are working. Unknown if all other opcodes in 7EaaXXn0 range do
also act like 7FaaXXn0, and perhaps also 7Cm0XXn0 like 7Dm0XXn0 (nintendo uses
the offical 7D/7F for SET/NOT/CLR, so weirdness may apply for TST only).

**H8/300H Encoding Notes**

```

| *i optional inverted source operand (when setting bit3 in the "n" digit)
 *s must have bit3 set in "s" digit
 *m must have bit3 set in "m" digit
 *? must have bit3 set-or-not-set (has conflicting info in official specs)
 E?Rs meant to be ERs (although official specs omit the E in some cases)
 E?Rd meant to be ERd (although official specs omit the E in some cases)
 xxxS meant to be Silent, no flags affected (although specs say Sign Extend)
 xxxX meant to mean Carry, or meant to mean nothing specific in other cases |
| --- |

**H8/300H Pseudo Opcodes**

The official "PUSH/POP.W/L Rn" opcodes are normal MOV.W/L opcodes with ER7 (SP)
and post-increment or pre-decrement.

```

| 6DFn PUSH.W Rn ;MOV.W [ER7-],Rn
 6D7n POP.W Rn ;MOV.W Rn,[ER7+]
 01006DFn PUSH.L ERn ;MOV.L [ER7-],ERn
 01006D7n POP.L ERn ;MOV.L ERn,[ER7+] |
| --- |

Although not officially defined, one could also implement "PUSH/POP.W CCR".

There are no PUSH/POP.B opcodes (because that would misalign the stack).

**H8/300H N/A**

```

| ---N/A--- MOV.L @aa:8,ERd MOV.L ERd,[FFaa] - ----nz0-
 ---N/A--- MOV.L ERs,@aa:8 MOV.L [FFaa],ERs - ----nz0-
 ---N/A--- MOV.W @aa:8,Rd MOV.W Rd,[FFaa] - ----nz0-
 ---N/A--- MOV.W Rs,@aa:8 MOV.W [FFaa],Rs - ----nz0-
 ---N/A--- SUB.B #nn:8,RdB SUB.B RdB,nn - --h-nzvc |
| --- |

| DS Cart Unknown Extras |
| --- |

**DS Cartridges with built-in MicroSD Card Slot**

The DS Vision cartridge contains a built-in microSD card slot. Users can
download videos from internet (against a fee), store the videos on microSD
cards, and then view them on the NDS via DS Vision cartridge.

Unknown how the microSD is accessed; via parallel 'ROM' bus and/or via serial
SPI bus; by which commands? Also unknown if the thing contains built-in video
decoder hardware, or if videos are decoded on ARM cpus.

**DS Cartridge with Bluetooth Keyboard (Typing Adventure)**

Mostly unknown. The components in the cartridge are:

```

| typical Macronix ROM
 STMicroelectronics M25PE10 SPI FLASH memory, presumably 128K
 Broadcom BCM2070 Bluetooth controller
 26MHz crystal oscillator |
| --- |

the FLASH chip isn't connected directly to the cart-SPI bus, instead it's
prolly accessed through the Bluetooth controller.

**NTR-UNSJ - Japanese TV Tuner**

Some huge cartridge with digital TV receiver.

Unknown if the "U" in the gamecode means that it contains NAND, too.

DSi Launcher sets BPTWL[21h].bit0=0 for this cartridge (unknown what for).

**NTR/TWL-Uxxx gamecodes (for carts with uncommon hardware)**

```

| NTR-UNSJ Japanese TV Tuner, with TV receiver
 NTR-UBRP Nintendo DS Brower, with RAM cart in GBA slot
 NTR-UAMA DS Vision Starter Kit, with microSD
 NTR-UEIJ Starry Sky Navigation, with azimuth
 NTR/TWL-Uxxx NAND carts (see NAND chapter) |
| --- |

| DS Cart Cheat Action Replay DS |
| --- |

The first commercial DS cheat code solution, this device was developed by
Datel. It supports swapping out cartridges after loading the AR software. For
updating, the user may either manually enter codes or use the included
proprietary USB cable that comes with the device. The user has been able to
manually update codes since firmware version 1.52.

**Action Replay DS Codes**

```

| ABCD-NNNNNNNN Game ID ;ASCII Gamecode [00Ch] and CRC32 across [0..1FFh]
 00000000 XXXXXXXX manual hook codes (rarely used) (default is auto hook)
 0XXXXXXX YYYYYYYY word[XXXXXXX+offset] = YYYYYYYY
 1XXXXXXX 0000YYYY half[XXXXXXX+offset] = YYYY
 2XXXXXXX 000000YY byte[XXXXXXX+offset] = YY
 3XXXXXXX YYYYYYYY IF YYYYYYYY > word[XXXXXXX] ;unsigned ;\
 4XXXXXXX YYYYYYYY IF YYYYYYYY < word[XXXXXXX] ;unsigned ; for v1.54,
 5XXXXXXX YYYYYYYY IF YYYYYYYY = word[XXXXXXX] ; when X=0,
 6XXXXXXX YYYYYYYY IF YYYYYYYY <> word[XXXXXXX] ; uses
 7XXXXXXX ZZZZYYYY IF YYYY > ((not ZZZZ) AND half[XXXXXXX]) ; [offset]
 8XXXXXXX ZZZZYYYY IF YYYY < ((not ZZZZ) AND half[XXXXXXX]) ; instead of
 9XXXXXXX ZZZZYYYY IF YYYY = ((not ZZZZ) AND half[XXXXXXX]) ; [XXXXXXX]
 AXXXXXXX ZZZZYYYY IF YYYY <> ((not ZZZZ) AND half[XXXXXXX]) ;/
 BXXXXXXX 00000000 offset = word[XXXXXXX+offset]
 C0000000 YYYYYYYY FOR loopcount=0 to YYYYYYYY ;execute Y+1 times
 C4000000 00000000 offset = address of the C4000000 code ;v1.54
 C5000000 XXXXYYYY counter=counter+1, IF (counter AND YYYY) = XXXX ;v1.54
 C6000000 XXXXXXXX [XXXXXXXX]=offset ;v1.54
 D0000000 00000000 ENDIF
 D1000000 00000000 NEXT loopcount
 D2000000 00000000 NEXT loopcount, and then FLUSH everything
 D3000000 XXXXXXXX offset = XXXXXXXX
 D4000000 XXXXXXXX datareg = datareg + XXXXXXXX
 D5000000 XXXXXXXX datareg = XXXXXXXX
 D6000000 XXXXXXXX word[XXXXXXXX+offset]=datareg, offset=offset+4
 D7000000 XXXXXXXX half[XXXXXXXX+offset]=datareg, offset=offset+2
 D8000000 XXXXXXXX byte[XXXXXXXX+offset]=datareg, offset=offset+1
 D9000000 XXXXXXXX datareg = word[XXXXXXXX+offset]
 DA000000 XXXXXXXX datareg = half[XXXXXXXX+offset]
 DB000000 XXXXXXXX datareg = byte[XXXXXXXX+offset] ;bugged on pre-v1.54
 DC000000 XXXXXXXX offset = offset + XXXXXXXX
 EXXXXXXX YYYYYYYY Copy YYYYYYYY parameter bytes to [XXXXXXXX+offset...]
 44332211 88776655 parameter bytes 1..8 for above code (example)
 0000AA99 00000000 parameter bytes 9..10 for above code (padded with 00s)
 FXXXXXXX YYYYYYYY Copy YYYYYYYY bytes from [offset..] to [XXXXXXX...] |
| --- |

IF/ENDIF can be nested up to 32 times. FOR/NEXT cannot be nested, any FOR
statement does forcefully terminate any prior loop. FOR does backup the current
IF condidition flags, and NEXT does restore these flags, so ENDIF(s) aren't
required inside of the loop. The NEXT+FLUSH command does (after finishing the
loop) reset offset=0, datareg=0, and does clear all condition flags, so further
ENDIF(s) aren't required after the loop.

Before v1.54, the DB000000 code did accidently set offset=offset+XXXXXXX after
execution of the code. For all word/halfword accesses, the address should be
aligned accordingly. For the COPY commands, addresses should be aligned by four
(all data is copied with ldr/str, except, on odd lengths, the last 1..3 bytes
do use ldrb/strb).

offset, datareg, loopcount, and counter are internal registers in the action
replay software.

> The condition register is checked, for all code types

> but the D0, D1 and D2 code type

Makes sense.

> and for the C5 code type it's checked AFTER the counter has

> been incremented (so the counter is always incremented

I love that exceptions ;-)

**Hook**

The hook codes consist of a series of nine 00000000 XXXXXXXX codes, and must be
marked as (M) code (for not being confused with normal 0XXXXXXX YYYYYYYY
codes). For all nine codes, the left 32bit are actually don't care (but should
be zero), the meaning of the right 32bit varies from 1st to 9th code.

```

| 1st: Address used prior to launching game (eg. 23xxxxxh)
 2nd: Address to write the hook at (inside the ARM7 executable)
 3rd: Hook final address (huh?)
 4th: Hook mode selection (0=auto, 1=mode1, 2=mode2)
 5th: Opcode that replaces the hooked one (eg. E51DE004h)
 6th: Address to store important stuff (default 23FE000h)
 7th: Address to store the code handler (default 23FE074h)
 8th: Address to store the code list (default 23FE564h)
 9th: Must be 1 (00000001h) |
| --- |

For most games, the AR does automatically hook code on the ARM7. Doing that
automatically is nice, but hooking ARM7 means that there is no access to VRAM,
TCM and Cache, which <might> cause problems since efficient games
<should> store all important data in TCM or Cache (though, in practice,
I'd doubt that any existing NDS games are that efficient).

**Thanks**

To Kenobi and Dualscreenman from Kodewerx for above ARDS cheat info.

| DS Cart Cheat Codebreaker DS |
| --- |

This is Pelican's entry into the DS cheat-device industry. It supports swapping
out the cartridges, and alternately, also gives the user the option of
connecting another gamecard onto it. For updating, the user may either manually
enter codes, or use Wifi to connect to the Codebreaker update site (that
updating will overwrite all manually entered codes though).

**Codebreaker DS Codes**

```

| ---Initialization---
 0000CR16 GAMECODE Specify Game ID, use Encrypted codes
 8000CR16 GAMECODE Specify Game ID, use Unencrypted codes
 BEEFC0DE XXXXXXXX Change Encryption Keys
 A0XXXXXX YYYYYYYY Bootup-Hook 1, X=Address, Y=Value
 A8XXXXXX YYYYYYYY Bootup-Hook 2, X=Address, Y=Value
 F0XXXXXX TYYYYYYY Code-Hook 1 (T=Type,Y=CheatEngineAddr,X=HookAddr)
 F8XXXXXX TPPPPPPP Code-Hook 2 (T=Type,X=CheatEngineHookAddr,P=Params)
 ---General codes---
 00XXXXXX 000000YY [X]=YY
 10XXXXXX 0000YYYY [X]=YYYY
 20XXXXXX YYYYYYYY [X]=YYYYYYYY
 60XXXXXX 000000YY ZZZZZZZZ 00000000 [[X]+Z]=YY
 60XXXXXX 0000YYYY ZZZZZZZZ 10000000 [[X]+Z]=YYYY
 60XXXXXX YYYYYYYY ZZZZZZZZ 20000000 [[X]+Z]=YYYYYYYY
 30XXXXXX 000000YY [X]=[X] + YY
 30XXXXXX 0001YYYY [X]=[X] + YYYY
 38XXXXXX YYYYYYYY [X]=[X] + YYYYYYYY
 70XXXXXX 000000YY [X]=[X] OR YY
 70XXXXXX 001000YY [X]=[X] AND YY
 70XXXXXX 002000YY [X]=[X] XOR YY
 70XXXXXX 0001YYYY [X]=[X] OR YYYY
 70XXXXXX 0011YYYY [X]=[X] AND YYYY
 70XXXXXX 0021YYYY [X]=[X] XOR YYYY
 ---Memory fill/copy---
 40XXXXXX 2NUMSTEP 000000YY 000000ZZ byte[X+(0..NUM-1)*STEP*1]=Y+(0..NUM-1)*Z
 40XXXXXX 1NUMSTEP 0000YYYY 0000ZZZZ half[X+(0..NUM-1)*STEP*2]=Y+(0..NUM-1)*Z
 40XXXXXX 0NUMSTEP YYYYYYYY ZZZZZZZZ word[X+(0..NUM-1)*STEP*4]=Y+(0..NUM-1)*Z
 50XXXXXX YYYYYYYY ZZZZZZZZ 00000000 copy Y bytes from [X] to [Z]
 ---Conditional codes (bugged)---
 60XXXXXX 000000YY ZZZZZZZZ 01c100VV IF [[X]+Z] .. VV THEN [[X]+Z]=YY
 60XXXXXX 000000YY ZZZZZZZZ 01c0VVVV IF [[X]+Z] .. VVVV THEN [[X]+Z]=YY
 60XXXXXX 0000YYYY ZZZZZZZZ 11c100VV IF [[X]+Z] .. VV THEN [[X]+Z]=YYYY
 60XXXXXX 0000YYYY ZZZZZZZZ 11c0VVVV IF [[X]+Z] .. VVVV THEN [[X]+Z]=YYYY
 60XXXXXX YYYYYYYY ZZZZZZZZ 21c100VV IF [[X]+Z] .. VV THEN [[X]+Z]=YYYYYYYY
 60XXXXXX YYYYYYYY ZZZZZZZZ 21c0VVVV IF [[X]+Z] .. VVVV THEN [[X]+Z]=YYYYYYYY
 ---Conditional codes (working)---
 D0XXXXXX NNc100YY IF [X] .. YY THEN exec max(1,NN) lines
 D0XXXXXX NNc0YYYY IF [X] .. YYYY THEN exec max(1,NN) lines |
| --- |

The condition digits (c=0..7), have the following functions:

```

| 0 IF [mem] = imm THEN ... 4 IF ([mem] AND imm) = 0 THEN ...
 1 IF [mem] <> imm THEN ... 5 IF ([mem] AND imm) <> 0 THEN ...
 2 IF [mem] < imm THEN ... (unsigned) 6 IF ([mem] AND imm) = imm THEN ...
 3 IF [mem] > imm THEN ... (unsigned) 7 IF ([mem] AND imm) <> imm THEN ... |
| --- |

Notes

```

| GAMECODE Cartridge Header[00Ch] (32bit in reversed byte-order)
 CR16 Cartridge Header[15Eh] (16bit in normal byte-order)
 XXXXXX 27bit addr (actually 7 digits, XXXXXXX, overlaps 5bit code number) |
| --- |

The "bugged" conditional codes (60XXXXXX) are accidently skipping NN lines when
the condition is false, where NN is taken from the upper 8bit of the code's
last 32bit values (ie. exactly as for the D0XXXXXX codes). For byte-writes,
that would be NN=01h, which can be eventually dealt with, although there may be
compatibility problems which future versions that might fix that bug. For
halfword/word writes, NN would be 11h or 21h, so that codes are about totally
unusable.

**Codebreaker DS / Encrypted Codes**

The overall "address value" decryption works like so:

```

| for i=4Fh to 00h
 y=77628ECFh
 if i>13h then y=59E5DC8Ah
 if i>27h then y=054A7818h
 if i>3Bh then y=B1BF0855h
 address = (Key0-value) xor address
 value = value - Key1 - (address ror 1Bh)
 address = (address xor (value + y)) ror 13h
 if (i>13h) then
 if (i<=27h) or (i>3Bh) then x=Key2 xor Key1 xor Key0
 else x=((Key2 xor Key1) and Key0) xor (Key1 and Key2)
 value=value xor (x+y+address)
 x = Secure[((i*4+00h) and FCh)+000h]
 x = Secure[((i*4+34h) and FCh)+100h] xor x
 x = Secure[((i*4+20h) and FCh)+200h] xor x
 x = Secure[((i*4+08h) and FCh)+300h] xor x
 address = address - (x ror 19h)
 next i |
| --- |

Upon startup, the initial key settings are:

```

| Secure[0..7FFh] = Copy of the ENCRYPTED 1st 2Kbytes of the game's Secure Area
 Key0 = 0C2EAB3Eh, Key1 = E2AE295Dh, Key2 = E1ACC3FFh, Key3 = 70D3AF46h
 scramble_keys |
| --- |

Upon BEEFC0DE XXXXXXXX, the keys get changed like so:

```

| Key0 = Key0 + (XXXXXXXX ror 1Dh)
 Key1 = Key1 - (XXXXXXXX ror 05h)
 Key2 = Key2 xor (Key3 xor Key0)
 Key3 = Key3 xor (Key2 - Key1)
 scramble_keys |
| --- |

The above scramble_keys function works like so:

```

| for i=0 to FFh
 y = byte(xlat_table[i])
 Secure[i*4+000h] = (Secure[i*4+000h] xor Secure[y*4]) + Secure[y*4+100h]
 Secure[i*4+400h] = (Secure[i*4+400h] xor Secure[y*4]) - Secure[y*4+200h]
 next i
 for i=0 to 63h
 Key0 = Key0 xor (Secure[i*4] + Secure[i*4+190h])
 Key1 = Key1 xor (Secure[i*4] + Secure[i*4+320h])
 Key2 = Key2 xor (Secure[i*4] + Secure[i*4+4B0h])
 Key3 = Key3 xor (Secure[i*4] + Secure[i*4+640h])
 next i
 Key0 = Key0 - Secure[7D0h]
 Key1 = Key1 xor Secure[7E0h]
 Key2 = Key2 + Secure[7F0h]
 Key3 = Key3 xor Secure[7D0h] xor Secure[7F0h] |
| --- |

the xlat_table consists of 256 fixed 8bit values:

```

| 34h,59h,00h,32h,7Bh,D3h,32h,C9h,9Bh,77h,75h,44h,E0h,73h,46h,06h
 0Bh,88h,B3h,3Eh,ACh,F2h,BAh,FBh,2Bh,56h,FEh,7Ah,90h,F7h,8Dh,BCh
 8Bh,86h,9Ch,89h,00h,19h,CDh,4Ch,54h,30h,01h,93h,30h,01h,FCh,36h
 4Dh,9Fh,FDh,D7h,32h,94h,AEh,BCh,2Bh,61h,DFh,B3h,44h,EAh,8Bh,A3h
 2Bh,53h,33h,54h,42h,27h,21h,DFh,A9h,DDh,C0h,35h,58h,EFh,8Bh,33h
 B4h,D3h,1Bh,C7h,93h,AEh,32h,30h,F1h,CDh,A8h,8Ah,47h,8Ch,70h,0Ch
 17h,4Eh,0Eh,A2h,85h,0Dh,6Eh,37h,4Ch,39h,1Fh,44h,98h,26h,D8h,A1h
 B6h,54h,F3h,AFh,98h,83h,74h,0Eh,13h,6Eh,F4h,F7h,86h,80h,ECh,8Eh
 EEh,4Ah,05h,A1h,F1h,EAh,B4h,D6h,B8h,65h,8Ah,39h,B3h,59h,11h,20h
 B6h,BBh,4Dh,88h,68h,24h,12h,9Bh,59h,38h,06h,FAh,15h,1Dh,40h,F0h
 01h,77h,57h,F5h,5Dh,76h,E5h,F1h,51h,7Dh,B4h,FAh,7Eh,D6h,32h,4Fh
 0Eh,C8h,61h,C1h,EEh,FBh,2Ah,FCh,ABh,EAh,97h,D5h,5Dh,E8h,FAh,2Ch
 06h,CCh,86h,D2h,8Ch,10h,D7h,4Ah,CEh,8Fh,EBh,03h,16h,ADh,84h,98h
 F5h,88h,2Ah,18h,ACh,7Fh,F6h,94h,FBh,3Fh,00h,B6h,32h,A2h,ABh,28h
 64h,5Ch,0Fh,C6h,23h,12h,0Ch,D2h,BAh,4Dh,A3h,F2h,C9h,86h,31h,57h
 0Eh,F8h,ECh,E1h,A0h,9Ah,3Ch,65h,17h,18h,A0h,81h,D0h,DBh,D5h,AEh |
| --- |

all used operations are unsigned 32bit integer.

**Thanks**

To Kenobi and Dualscreenman from Kodewerx for above CBDS cheat info.

| DS Cart DLDI Driver |
| --- |

DLDI (Dynamically Linked Device Interface for libfat) is a popular yet
undocumented flashcart driver for homebrew NDS software dating back to 2006.
Below was reverse-engineered 11/2018.

file.dldi --> driver file (can be small like 1.5Kbyte, or max 32Kbyte)

file.nds --> ROM image (must contain 32Kbyte space with DLDI ID for
patching)

**Driver patch file standard header**

```

| 00h 4 DLDI ID (EDh,A5h,8Dh,BFh) (aka BF8DA5EDh) ;\patching tools will
 04h 8 DLDI String (20h,"Chishm",00h) ; refuse any other
 0Ch 1 DLDI Version (01h in .dldi, don't care in .nds) ;/values
 0Dh 1 Size of .dldi+BSS (rounded up to 1 SHL N bytes) (max 0Fh=32Kbytes)
 0Eh 1 Sections to fix/destroy (see FIX_xxx)
 0Fh 1 Space in .nds file (1 SHL N) (0Eh..0Fh in .nds, can be 0 in .dldi)
 10h 48 ASCII Full Driver Name (max 47 chars, plus zero padding)
 40h 4 Address of ALL start (text) ;-base address (BF800000h in .dldi)
 44h 4 Address of ALL end (data) ;-for highly-unstable FIX_ALL addr.adjusts
 48h 4 Address of GLUE start ;\for semi-stable FIX_GLUE addr.adjusts
 4Ch 4 Address of GLUE end ;/ ("Interworking glue" for ARM-vs-THUMB)
 50h 4 Address of GOT start ;\for semi-stable FIX_GOT addr.adjusts
 54h 4 Address of GOT end ;/ ("Global Offset Table")
 58h 4 Address of BSS start ;\for zerofilling "BSS" via FIX_BSS
 5Ch 4 Address of BSS end ;/ ("Block Started by Symbol")
 60h 4 ASCII Short Driver/Device Name (4 chars, eg. "MYHW" for MyHardware)
 64h 4 Flags 2 (see FEATURE_xxx) (usually 13h=GbaSlot, or 23h=NdsSlot)
 68h 4 Address of Function startup() ;<-- must be at offset +80h !! ;\
 6Ch 4 Address of Function isInserted() ;out: 0=no/fail, 1=yes/okay ; all
 70h 4 Address of Function readSectors(sector,numSectors,buf) ; return
 74h 4 Address of Function writeSectors(sector,numSectors,buf) ; 0=fail,
 78h 4 Address of Function clearStatus() ; 1=okay
 7Ch 4 Address of Function shutdown() ;/
 80h .. Driver Code (can/must begin with "startup()") ;\max 7F80h
 .. .. Glue section (usually a small snippet within above code) ; bytes (when
 .. .. GOT section (usually after above code) (pointer table) ; having 32K
 .. .. BSS section (usually at end, may exceed .dldi filesize) ; allocated)
 .. .. Optional two garbage NOPs at end of default.dldi ;/ |
| --- |

hdr[0Eh] - Sections to fix/destroy (FIX_xxx):

```

| 0 FIX_ALL ;-installer uses highly-unstable guessing in whole dldi file
 1 FIX_GLUE ;-installer uses semi-stable address guessing in GLUE area
 2 FIX_GOT ;-installer uses semi-stable address guessing in GOT area
 3 FIX_BSS ;-installer will zerofill BSS area
 4-7 Reserved (0) |
| --- |

hdr[64h] - Flags 2 (FEATURE_xxx) (usually 13h=GbaSlot, or 23h=NdsSlot):

```

| 0 FEATURE_MEDIUM_CANREAD 00000001h (usually set)
 1 FEATURE_MEDIUM_CANWRITE 00000002h (a few carts can't write)
 2-3 Reserved (0)
 4 FEATURE_SLOT_GBA 00000010h (need EXMEMCNT bit7 adjusted)
 5 FEATURE_SLOT_NDS 00000020h (need EXMEMCNT bit11 adjusted)
 6-31 Reserved (0) |
| --- |

Note: The allocated driver size in hdr[0Fh] was 0Fh=32Kbytes between 2006 and
2016, however, libnds has changed that to 0Eh=16Kbytes in January 2017 (maybe
intending to free more RAM, especially when using ARM7 WRAM).

However, there's at least one driver exceeding 16K (rpg_nand.dldi in AKAIO
package; the driver disguises itself as 8K driver in hdr[0Dh], but its BSS area
actually needs ways MORE than 16K).

**Required entries in .nds file**

Officially, dldi could be at any 4-byte aligned location, however, for faster
lookup, better use this locations:

```

| dldi area should be located at a 40h-byte aligned address in ROM image.
 dldi area should be located in ARM9 (or ARM7) bootcode area. |
| --- |

An "empty" driver needs to contain:

```

| dldi[00h..0Bh] must contain DLDI ID word/string
 dldi[0Fh] must contain allocated size (0Eh=16Kbyte or 0Fh=32Kbyte)
 dldi[40h..43h] must contain RAM base address of DLDI block
 and other entries should contain valid dummy strings and dummy functions. |
| --- |

An installed driver should contain a copy of the .dldi file, with addresses
adjusted to RAM locations, and BSS area zerofilled (if FIX_BSS was set)

```

| dldi[0Fh] must be kept as in the old .nds file (not as in .dldi file) |
| --- |

Some installers might try to detect homebrew by looking at nds carthdr for
deciding whether or not to try to install dldi (unknown if/which ones are doing
such things and looking at which carthdr entries).

**Functions**

startup, isInserted, clearStatus, shutdown can be dummy functions that do
nothing (other than returning r0=1=okay).

Alternately startup/shutdown can initialize or power down the hardware,
clearStatus is meant to be some sort of soft reset, and isInserted is allowing
to test if the SD card is inserted & working.

read/write sectors are reading/writing one or more sectors. Sector size is 200h
bytes, sector numbers is 0=First 200h bytes, 1=Next 200h bytes, and so on.

buf should be usually 4-byte aligned, however, some drivers do also support
unaligned buffers using slower transfer code (better implement that when making
.dldi drivers, but better don't rely on it being supported when making .nds
files).

The driver functions can support SD and SDHC (or the flashcart manufacturer
might release driver updates if SDHC wasn't supported).

Higher level FAT functions must be contained in the .nds file (so a driver
update won't help if the .nds file lacks FAT32) (and ExFAT most unlikely to be
supported).

Functions should be ARM7 compatible, ie. don't use BLX or POP r15 for mode
switching, so the driver can be used on both ARM9 and ARM7 (or even on GBA).

**SLOT_GBA/NDS**

SLOT_GBA/NDS seem to relate to GBA and NDS slots, the driver can probably have
only one of the SLOT bits set (the functions don't allow to select which slot
to use).

Purpose is unclear to me, maybe just telling the .nds file that the flashcart
is in the given slot (and thereby shouldn't expect other hardware in that
slot). Or maybe telling telling the installer which hardware the driver is
supposed for.

FIX_xxx does maybe relate to address adjustments made by the dldi installer.
Unknown if/how that's working.

DS Cart DLDI Driver - Guessed Address-Adjustments

Some DLDI flashcarts support extra features like Rumble. However, that extra
hardware is accessed via direct I/O, not via DLDI driver. Unknown which I/O
ports are used for that stuff - probably something compatible with official
GBA/NDS rumble cart(s).

| DS Cart DLDI Driver - Guessed Address-Adjustments |
| --- |

The DLDI installer uses some guessing method for address-adjustments (the
FIX_xxx flags are supposed to patch addresses, but not opcodes or other data).

**Unaligned-Word patching and over-shooting bug**

The central mistake in the official DLDI installer is that it is patching all
words at [start..end-1], using 1-byte address increments instead of 4-byte
increments. This includes patching words at non-word aligned locations, or
patching words whose lower bytes were already patched, and over-shooting to
words at [end-1..end+2].

**ddmem Base BF800000h**

Most or all .dldi files are using ddmem base BF800000h (defined in dldi.ld).
That value was chosen because it won't conflict with opcodes (as NDS BIOS
doesn't use SWI function 80h, BF80xxxxh would be an invalid SWI function in
ARM; and BF80h would be an invalid opcode in THUMB).

So far, this would have worked well, but it doesn't work with unaligned-word
patching bug (eg. THUMB opcodes 8000h,BF00h, or ARM opcodes xxBF80xxh or the
like). And, even if it would have worked for opcodes - it might still fail for
data values.

**FIX_GOT**

GOT does usually contain BF80xxxxh address pointers (plus some 00000000h
words). The guessing works quite stable (the maximum for 32K files is
xxxxh=7FFFh, so there's no risk to encounter xxxx=BF80h) (one could encounter
xxxxh=xxBFh, but the previously patched word is usually in RAM area, eg.
02xxxxxxh, so this would form BF02h, without risk to be seen as BF80h).

BUG: The GOT table is usually located at the end of the .dldi file, meaning
that the over-shooting bug will see three uninitialized bytes at
[got_end+0..2], and may go amok if they are BF80xxh or xxBF80h. The value of
those bytes depends on left-over from previously installed .dldi driver(s) and
on the ddmem base used in the .nds file, so the bug may take place randomly
depending on several factors.

**FIX_GLUE**

GLUE does usually contain a handful of opcodes and .pool values for switching
between ARM and THUMB code. The intention is to patch the addresses in the
.pool, and to leave the opcodes intact. This can be potentially stable,
assuming that the used opcodes in the GLUE (and the next three bytes after
glue_end) usually won't contain BF80h).

**FIX_ALL**

This is the mother of all bugs. Fortunately there aren't any .dldi drivers with
FIX_ALL flag - and one should never make drivers that do use it.

ALL is covering the whole dldi space, including the 80h-byte DLDI header, the
code area, including GLUE area, and GOT area, and probably also the yet
uninitialized BSS area, and the next three bytes after end_all.

Patching the whole code area means an increased risk to hit opcodes or data
values that contain BF80h. The over-shooting bug may even destroy the next
three bytes after the 32Kbyte area.

Patching the DLDI header could destroy the header itself, the header in the
.dldi file usually won't contain BF80h at unintended locations, however, the
pointers in that header are adjusted before applying FIX_ALL, for example, RAM
base 0200BF00h (in .nds file) combined with a function at BF800080h (in .dldi
file) would result in 0200BF80h. The nasty thing is that the problem won't
occur with other RAM base values (in other .nds files).

**Avoiding the bugs**

When making .dldi drivers: Never use FIX_ALL. And preferably avoid FIX_GOT and
FIX_GLUE as well (ARM CPU can do relative jumps and relative addressing, eg.
via ADR and ADRL pseudo opcodes, so there's no point in needing address
adjustments). Or otherwise append padding after GOT area, and try to avoid
using opcodes/data with BF80h in/after GLUE area.

When making dldi installers: Best patch only word-aligned words (ARM CPU can't
access unaligned data, so there's little chance that DLDI drivers would ever
contain unaligned words). Or, when maintaining unaligned patching: At the very
least skip the 80h-byte header on FIX_ALL, and after patching a word at other
locations, skip the next three bytes, and don't do the over-shooting at end.

When making .nds files: There isn't too much that could be done here. One could
set ddmem to 64Kbyte aligned addresses (so functions won't end up at
xxxxBF80h). Or one could even set ddmem to BF800000h (so patching will leave
everything unchanged & intact - so one could then do the address
adjustments manually, and hopefully more reliable than other DLDI installers).

| DS Encryption by Gamecode/Idcode (KEY1) |
| --- |

**KEY1 - Gamecode / Idcode Encryption**

The KEY1 encryption relies only on the gamecode (or firmware idcode), it does
not contain any random components. The fact that KEY1 encrypted commands appear
random is just because the <unencrypted> commands contain random values,
so the encryption result looks random.

KEY1 encryption is used for KEY1 encrypted gamecart commands (ie. for loading
the secure area). It is also used for resolving the extra decryption of the
first 2K of the secure area, and for firmware decryption, and to decode some
encrypted values in gamecart/firmware header.

**Initial Encryption Values**

Below formulas can be used only with a copy of the 1048h-byte key tables from
NDS/DSi BIOS. The values can be found at:

```

| NDS.ARM7 ROM: 00000030h..00001077h (values 99 D5 20 5F ..) Blowfish/NDS-mode
 DSi.ARM9 ROM: FFFF99A0h..FFFFA9E7h (values 99 D5 20 5F ..) ""
 DSi.TCM Copy: 01FFC894h..01FFD8DBh (values 99 D5 20 5F ..) ""
 DSi.ARM7 ROM: 0000C6D0h..0000D717h (values 59 AA 56 8E ..) Blowfish/DSi-mode
 DSi.RAM Copy: 03FFC654h..03FFD69Bh (values 59 AA 56 8E ..) ""
 DSi.Debug: (stored in launcher) (values 69 63 52 05 ..) Blowfish/DSi-debug |
| --- |

The DSi ROM sections are disabled after booting, but the RAM/TCM copies can be
dumped (eg. with some complex main memory hardware mods, or via unlaunch
exploit). The DSi.Debug key is stored in launcher, and it's used when SCFG_OP
is nonzero (as so on debugging on hardware).

**encrypt_64bit(ptr) / decrypt_64bit(ptr)**

```

| Y=[ptr+0]
 X=[ptr+4]
 FOR I=0 TO 0Fh (encrypt), or FOR I=11h TO 02h (decrypt)
 Z=[keybuf+I*4] XOR X
 X=[keybuf+048h+((Z SHR 24) AND FFh)*4]
 X=[keybuf+448h+((Z SHR 16) AND FFh)*4] + X
 X=[keybuf+848h+((Z SHR 8) AND FFh)*4] XOR X
 X=[keybuf+C48h+((Z SHR 0) AND FFh)*4] + X
 X=Y XOR X
 Y=Z
 NEXT I
 [ptr+0]=X XOR [keybuf+40h] (encrypt), or [ptr+0]=X XOR [keybuf+4h] (decrypt)
 [ptr+4]=Y XOR [keybuf+44h] (encrypt), or [ptr+4]=Y XOR [keybuf+0h] (decrypt) |
| --- |

**apply_keycode(modulo)**

```

| encrypt_64bit(keycode+4)
 encrypt_64bit(keycode+0)
 [scratch]=0000000000000000h ;S=0 (64bit)
 FOR I=0 TO 44h STEP 4 ;xor with reversed byte-order (bswap)
 [keybuf+I]=[keybuf+I] XOR bswap_32bit([keycode+(I MOD modulo)])
 NEXT I
 FOR I=0 TO 1040h STEP 8
 encrypt_64bit(scratch) ;encrypt S (64bit) by keybuf
 [keybuf+I+0]=[scratch+4] ;write S to keybuf (first upper 32bit)
 [keybuf+I+4]=[scratch+0] ;write S to keybuf (then lower 32bit)
 NEXT I |
| --- |

**init_keycode(idcode,level,modulo,key)**

```

| if key=nds then copy [nds_arm7bios+0030h..1077h] to [keybuf+0..1047h]
 if key=dsi then copy [dsi_arm7bios+C6D0h..D717h] to [keybuf+0..1047h]
 [keycode+0]=[idcode]
 [keycode+4]=[idcode]/2
 [keycode+8]=[idcode]*2
 IF level>=1 THEN apply_keycode(modulo) ;first apply (always)
 IF level>=2 THEN apply_keycode(modulo) ;second apply (optional)
 [keycode+4]=[keycode+4]*2
 [keycode+8]=[keycode+8]/2
 IF level>=3 THEN apply_keycode(modulo) ;third apply (optional) |
| --- |

**firmware_decryption**

```

| init_keycode(firmware_header+08h,1,0Ch,nds) ;idcode (usually "MACP"), level 1
 decrypt_64bit(firmware_header+18h) ;rominfo
 init_keycode(firmware_header+08h,2,0Ch,nds) ;idcode (usually "MACP"), level 2
 decrypt ARM9 and ARM7 bootcode by decrypt_64bit (each 8 bytes)
 decompress ARM9 and ARM7 bootcode by LZ77 function (swi)
 calc CRC16 on decrypted/decompressed ARM9 bootcode followed by ARM7 bootcode |
| --- |

Note: The sizes of the compressed/encrypted bootcode areas are unknown (until
they are fully decompressed), one way to solve that problem is to decrypt the
next 8 bytes each time when the decompression function requires more data.

**gamecart_decryption**

```

| init_keycode(cart_header+0Ch,1,08h,nds) ;gamecode, level 1, modulo 8
 decrypt_64bit(cart_header+78h) ;rominfo (secure area disable)
 init_keycode(cart_header+0Ch,2,08h,nds) ;gamecode, level 2, modulo 8
 encrypt_64bit all NDS KEY1 commands (1st command byte in MSB of 64bit value)
 after loading the secure_area, calculate secure_area crc, then
 decrypt_64bit(secure_area+0) ;first 8 bytes of secure area
 init_keycode(cart_header+0Ch,3,08h,nds) ;gamecode, level 3, modulo 8
 decrypt_64bit(secure_area+0..7F8h) ;each 8 bytes in first 2K of secure
 init_keycode(cart_header+0Ch,1,08h,dsi) ;gamecode, level 1, modulo 8
 encrypt_64bit all DSi KEY1 commands (1st command byte in MSB of 64bit value) |
| --- |

After secure area decryption, the ID field in the first 8 bytes should be
"encryObj", if it matches then first 8 bytes are filled with E7FFDEFFh,
otherwise the whole 2K are filled by that value.

**Gamecart Command Register**

Observe that the byte-order of the command register [40001A8h] is reversed. The
way how the CPU stores 64bit data in memory (and the way how the
"encrypt_64bit" function for KEY1-encrypted commands expects data in memory) is
LSB at [addr+0] and MSB at [addr+7]. This value is to be transferred MSB first.
However, the DS hardware transfers [40001A8h+0] first, and [40001A8h+7] last.
So, the byte order must be reversed when copying the value from memory to the
command register.

**Note**

The KEY1 encryption is based on Bruce Schneier's "Blowfish Encryption
Algorithm".

| DS Encryption by Random Seed (KEY2) |
| --- |

**KEY2 39bit Seed Values**

The pre-initialization settings at cartridge-side (after reset) are:

```

| Seed0 = 58C56DE0E8h
 Seed1 = 5C879B9B05h |
| --- |

The post-initialization settings (after sending command 4llllmmmnnnkkkkkh to
the cartridge, and after writing the Seed values to Port 40001Bxh) are:

```

| Seed0 = (mmmnnn SHL 15)+6000h+Seedbyte
 Seed1 = 5C879B9B05h |
| --- |

The seedbyte is selected by Cartridge Header [013h].Bit0-2, this index value
should be usually in range 0..5, however, possible values for index 0..7 are:
E8h,4Dh,5Ah,B1h,17h,8Fh,99h,D5h.

The 24bit random value (mmmnnn) is derived from the real time clock setting,
and also scattered by KEY1 encryption, anyways, it's just random and doesn't
really matter where it comes from.

**KEY2 Encryption**

Relies on two 39bit registers (x and y), which are initialized as such:

```

| x = reversed_bit_order(seed0) ;ie. LSB(bit0) exchanged with MSB(bit38), etc.
 y = reversed_bit_order(seed1) |
| --- |

During transfer, x, y, and transferred data are modified as such:

```

| x = (((x shr 5)xor(x shr 17)xor(x shr 18)xor(x shr 31)) and 0FFh)+(x shl 8)
 y = (((y shr 5)xor(y shr 23)xor(y shr 18)xor(y shr 31)) and 0FFh)+(y shl 8)
 data = (data xor x xor y) and 0FFh |
| --- |

| DS Firmware Serial Flash Memory |
| --- |

**ST Microelectronics SPI Bus Compatible Serial FLASH Memory**

```

| Chips used as wifi-flash:
 ID 20h,40h,12h - ST M45PE20 - 256 KBytes (Nintendo DS) (in my old DS)
 ID 20h,50h,12h - ST M35PE20 - 256 KBytes (Nintendo DS) (in my DS-Lite)
 ID 20h,80h,13h - ST M25PE40 - 512 KBytes (iQue DS, with chinese charset)
 ID 20h,40h,11h - ST 45PE10V6 - 128 Kbytes (Nintendo DSi) (in my DSi)
 ID 20h,58h,0Ch?- 5A32 - 4 Kbytes (Nintendo DSi, newer models)
 ID ? - 26FV032T - (Nintendo DSi, J27H020) (this has big package)
 ID ? - 5K32 - (3DS?)
 ID 62h,62h,0Ch - 32B, 3XH - 4 Kbytes (New3DS)
 Other similar chips (used in game cartridges):
 ID 20h,40h,13h - ST 45PE40V6 - 512 KBytes (DS Zelda, NTR-AZEP-0)
 ID 20h,40h,14h - ST 45PE80V6 - 1024 Kbytes (eg. Spirit Tracks, NTR-BKIP)
 +ID 62h,11h,00h - Sanyo ? - 512 Kbytes (P-Letter Diamond, ADAE)
 ID 62h,16h,00h - Sanyo LE25FW203T - 256 KBytes (Mariokart backup)
 +ID 62h,26h,11h - Sanyo ? - ? Kbytes (3DS: CTR-P-AXXJ)
 +ID 62h,26h,13h - Sanyo ? - ? Kbytes (3DS: CTR-P-APDJ)
 ID C2h,22h,11h - Macronix MX25L1021E? 128 Kbytes (eg. 3DS Starfox)
 ID C2h,22h,13h - Macronix ...? 512 Kbytes (eg. 3DS Kid Icarus, 3DS Sims 3)
 ID C2h,20h,17h - Macronix MX25L6445EZNI-10G 8192 Kbytes (DSi Art Academy)
 ID 01h,F0h,00h - Garbage/Infrared on SPI-bus? (eg. P-Letter White)
 ID 03h,F8h,00h - Garbage/Infrared on SPI-bus? (eg. P-Letter White 2) |
| --- |

FLASH has more than 100,000 Write Cycles, more than 20 Year Data Retention

The Firmware Flash Memory is accessed via SPI bus,

DS Serial Peripheral Interface Bus (SPI)

**Instruction Codes**

```

| 06h WREN Write Enable (No Parameters)
 04h WRDI Write Disable (No Parameters)
 9Fh RDID Read JEDEC Identification (Read 1..3 ID Bytes)
 (Manufacturer, Device Type, Capacity)
 05h RDSR Read Status Register (Read Status Register, endless repeated)
 Bit7-2 Not used (zero)
 Bit1 WEL Write Enable Latch (0=No, 1=Enable)
 Bit0 WIP Write/Program/Erase in Progess (0=No, 1=Busy)
 03h READ Read Data Bytes (Write 3-Byte-Address, read endless data stream)
 0Bh FAST Read Data Bytes at Higher Speed (Write 3-Byte-Address, write 1
 dummy-byte, read endless data stream) (max 25Mbit/s)
 0Ah PW Page Write (Write 3-Byte-Address, write 1..256 data bytes)
 (changing bits to 0 or 1) (reads unchanged data, erases the page,
 then writes new & unchanged data) (11ms typ, 25ms max)
 02h PP Page Program (Write 3-Byte-Address, write 1..256 data bytes)
 (changing bits from 1 to 0) (1.2ms typ, 5ms max)
 DBh PE Page Erase 100h bytes (Write 3-Byte-Address) (10ms typ, 20ms max)
 D8h SE Sector Erase 10000h bytes (Write 3-Byte-Address) (1s typ, 5s max)
 B9h DP Deep Power-down (No Parameters) (consumption 1uA typ, 10uA max)
 (3us) (ignores all further instructions, except RDP)
 ABh RDP Release from Deep Power-down (No Parameters) (30us) |
| --- |

Write/Program may not cross page-boundaries. Write/Program/Erase are rejected
during first 1..10ms after power up. The WEL bit is automatically cleared on
Power-Up, on /Reset, and on completion of WRDI/PW/PP/PE/SE instructions. WEL is
set by WREN instruction (which must be issued before any write/program/erase
instructions). Don't know how RDSR behaves when trying to write to the
write-protected region?

**Communication Protocol**

```

| Set Chip Select LOW to invoke the command
 Transmit the instruction byte
 Transmit any parameter bytes
 Transmit/receive any data bytes
 Set Chip Select HIGH to finish the command |
| --- |

All bytes (and 3-byte addresses) transferred most significant bit/byte first.

**DSi "5A32" chip (32Kit aka 4Kbyte)**

Early DSi DWM-W015 boards did have 128Kbyte FLASH, but later boards have custum
4Kbyte FLASH chips (these 4K chips are found on later DSi DWM-W015 boards, and
DSi DWM-W024 boards, and 3DS DWM-W028 boards). The chips are having a 24bit
address bus (needed for NDS compatibility), and, a weird non-writeable gap
within a 128Kbyte memory are:

```

| 000000h..0002FFh Writeable only if /WP=HIGH (otherwise writes are ignored)
 000300h..01F2FFh Not writeable (FFh-filled, writes are ignored)
 01F300h..01FFFFh Writeable
 020000h and up Mirrors of 0..01FFFFh (same read/write-ability as above) |
| --- |

There are several part numbers: "5A32" (DSi), "5K32" (3DS), "32B, 3XH" (3DS),
and "26FV032T" (DSi), that chips are probably all the same size &
functionally same; most of those 4Kbyte chips have tiny packages (except
"26FV032T" which comes in classic large package).

**Pin-Outs (Large Package, in NDS, and early DSi boards)**

```

| 1 D Serial Data In (latched at rising clock edge) _________
 2 C Serial Clock (max 25MHz) /|o |
 3 /RES Reset 1 -| | |- 8
 4 /S Chip Select (instructions start at falling edge) 2 -| | |- 7
 5 /W Write Protect (makes first 256 pages read-only) 3 -| |_________|- 6
 6 VCC Supply (2.7V..3.6V typ) (4V max) (DS:VDD3.3) 4 -|/ |- 5
 7 VSS Ground |___________|
 8 Q Serial Data Out (changes at falling clock edge) |
| --- |

**Pin-Outs (Tiny Package, in newer DSi boards, and 3DS)**

```

| 1 /S Chip Select (instructions start at falling edge) ___________
 2 Q Serial Data Out (changes at falling clock edge) 1 -| o |- 8
 3 /W Write Protect (makes first pages read-only) 2 -| |- 7
 4 VSS Ground 3 -| |- 6
 5 D Serial Data In (latched at rising clock edge) 4 -|___________|- 5
 6 C Serial Clock
 7 /RES Reset
 8 VCC Supply (2.7V..3.6V typ) (DSi: VDD33) |
| --- |

| DS Firmware Header |
| --- |

**Firmware Memory Map**

```

| 00000h-00029h Firmware Header
 0002Ah-001FFh Wifi Settings
 00200h-3F9FFh Firmware Code/Data ;-NDS only (not DSi)
 00200h-002FEh 00h-filled ;\
 002FFh 80h ;
 00300h-1F2FFh FFh-filled (not write-able on 4K chips) ; DSi only (not NDS)
 1F300h-1F3FEh FFh-filled (write-able) ;
 1F3FFh Whatever Debug/Bootflags ;
 1F400h-1F5FFh Wifi Access Point 4 ;\with WPA/WPA2 ;
 1F600h-1F7FFh Wifi Access Point 5 ; support ;
 1F800h-1F9FFh Wifi Access Point 6 ;/ ;/
 3FA00h-3FAFFh Wifi Access Point 1 ;\
 3FB00h-3FBFFh Wifi Access Point 2 ; Open/WEP only
 3FC00h-3FCFFh Wifi Access Point 3 ;/
 3FD00h-3FDFFh Not used
 3FE00h-3FEFFh User Settings Area 1
 3FF00h-3FFFFh User Settings Area 2 |
| --- |

On iQue DS (with 512K flash memory), user settings are moved to 7FE00h and up,
and, there seems to be some unknown stuff at 200h..27Fh.

**Firmware Header (00000h-001FFh)**

```

| Addr Size Expl.
 000h 2 part3 romaddr/8 (arm9 gui code) (LZ/huffman compression)
 002h 2 part4 romaddr/8 (arm7 wifi code) (LZ/huffman compression)
 004h 2 part3/4 CRC16 arm9/7 gui/wifi code
 006h 2 part1/2 CRC16 arm9/7 boot code
 008h 4 firmware identifier (usually nintendo "MAC",nn) (or nocash "XBOO")
 the 4th byte (nn) occassionally changes in different versions
 00Ch 2 part1 arm9 boot code romaddr/2^(2+shift1) (LZSS compressed)
 00Eh 2 part1 arm9 boot code 2800000h-ramaddr/2^(2+shift2)
 010h 2 part2 arm7 boot code romaddr/2^(2+shift3) (LZSS compressed)
 012h 2 part2 arm7 boot code 3810000h-ramaddr/2^(2+shift4)
 014h 2 shift amounts, bit0-2=shift1, bit3-5=shift2, bit6-8=shift3,
 bit9-11=shift4, bit12-15=firmware_chipsize/128K
 016h 2 part5 data/gfx romaddr/8 (LZ/huffman compression)
 018h 8 Optional KEY1-encrypted "enPngOFF"=Cartridge KEY2 Disable
 (feature isn't used in any consoles, instead contains timestamp)
 018h 5 Firmware version built timestamp (BCD minute,hour,day,month,year)
 01Dh 1 Console type
 FFh=Nintendo DS
 20h=Nintendo DS-lite
 57h=Nintendo DSi (also iQueDSi)
 43h=iQueDS
 63h=iQueDS-lite
 The entry was unused (FFh) in older NDS, ie. replace FFh by 00h)
 Bit0 seems to be DSi/iQue related
 Bit1 seems to be DSi/iQue related
 Bit2 seems to be DSi related
 Bit3 zero
 Bit4 seems to be DSi related
 Bit5 seems to be DS-Lite related
 Bit6 indicates presence of "extended" user settings (DSi/iQue)
 Bit7 zero
 01Eh 2 Unused (FFh-filled)
 020h 2 User Settings Offset (div8) (usually last 200h flash bytes)
 022h 2 Unknown (7EC0h or 0B51h)
 024h 2 Unknown (7E40h or 0DB3h)
 026h 2 part5 CRC16 data/gfx
 028h 2 unused (FFh-filled)
 02Ah-1FFh Wifi Calibration Data (see next chapter) |
| --- |

**DSi**

```

| 000h 1Dh Zerofilled (bootcode is in new eMMC chip, not on old FLASH chip)
 01Dh 6 Same as on DS (header: Console Type and User Settings Offset)
 022h 6 Zerofilled (bootcode is in new eMMC chip, not on old FLASH chip)
 028h..1FCh Same as on DS (wifi calibration)
 1FDh 1 Wifi Board (01h=DWM-W015, 02h=W024, 03h=W028) ;\this was
 1FEh 1 Wifi Flash (20h=With access point 4/5/6) ; FFh-filled
 1FFh 1 Same as on DS (FFh) ;/on DS
 200h FFh Zerofilled ;\
 2FFh 1 Unknown (80h) ; this was
 300h 1F000h FFh's (not write-able on 4K chips) ; bootcode
 1F300h FFh FFh's (write-able) ;twl-debugger: 00h's ; on DS
 1F3FFh 1 FFh ;twl-debugger: 40h ;/ |
| --- |

The bytes [000h..027h] cannot be changed on DSi because they are part of the
RSA signature in DSi's Boot Info Block (at eMMC offset 200h..3FFh).

| DS Firmware Wifi Calibration Data |
| --- |

**Wifi Calibration/Settings (located directly after Firmware Header)**

```

| Addr Size Expl.
 000h-029h Firmware Header (see previous chapter)
 02Ah 2 CRC16 (with initial value 0) of [2Ch..2Ch+config_length-1]
 02Ch 2 config_length (usually 0138h, ie. entries 2Ch..163h)
 02Eh 1 Unused (00h)
 02Fh 1 Version (0=v1..v4, 3=v5, 5=v6..v7,6=W006,15=W015,24=W024,34=N3DS)
 030h 6 Unused (00h-filled) (DS-Lite and DSi: FF,FF,FF,FF,FF,00)
 036h 6 48bit MAC address (v1-v5: 0009BFxxxxxx, v6-v7: 001656xxxxxx)
 03Ch 2 list of enabled channels ANDed with 7FFE (Bit1..14 = Channel 1..14)
 (usually 3FFEh, ie. only channel 1..13 enabled)
 03Eh 2 Whatever Flags (usually FFFFh)
 040h 1 RF Chip Type (NDS: usually 02h) (DS-Lite and DSi/3DS: usually 03h)
 041h 1 RF Bits per entry at 0CEh (usually 18h=24bit=3byte) (Bit7=?)
 042h 1 RF Number of entries at 0CEh (usually 0Ch)
 043h 1 Unknown (usually 01h)
 044h 2 Initial Value for [4808146h] ;W_CONFIG_146h
 046h 2 Initial Value for [4808148h] ;W_CONFIG_148h
 048h 2 Initial Value for [480814Ah] ;W_CONFIG_14Ah
 04Ah 2 Initial Value for [480814Ch] ;W_CONFIG_14Ch
 04Ch 2 Initial Value for [4808120h] ;W_CONFIG_120h
 04Eh 2 Initial Value for [4808122h] ;W_CONFIG_122h
 050h 2 Initial Value for [4808154h] ;W_CONFIG_154h
 052h 2 Initial Value for [4808144h] ;W_CONFIG_144h
 054h 2 Initial Value for [4808130h] ;W_CONFIG_130h
 056h 2 Initial Value for [4808132h] ;W_CONFIG_132h
 058h 2 Initial Value for [4808140h] ;W_CONFIG_140h ;maybe ACK timeout?
 05Ah 2 Initial Value for [4808142h] ;W_CONFIG_142h
 05Ch 2 Initial Value for [4808038h] ;W_POWER_TX
 05Eh 2 Initial Value for [4808124h] ;W_CONFIG_124h
 060h 2 Initial Value for [4808128h] ;W_CONFIG_128h
 062h 2 Initial Value for [4808150h] ;W_CONFIG_150h
 064h 69h Initial 8bit values for BB[0..68h]
 0CDh 1 Unused (00h) |
| --- |

Below for Type2 (ie. when [040h]=2) (Mitsumi MM3155 and RF9008):

```

| 0CEh 24h Initial 24bit values for RF[0,4,5,6,7,8,9,0Ah,0Bh,1,2,3]
 0F2h 54h Channel 1..14 2x24bit values for RF[5,6]
 146h 0Eh Channel 1..14 8bit values for BB[1Eh] (usually somewhat B1h..B7h)
 154h 0Eh Channel 1..14 8bit values for RF[9].Bit10..14 (usually 10h-filled) |
| --- |

Below for Type3 (ie. when [040h]=3) (Mitsumi MM3218) (and AR6013G):

```

| --- Type3 values are originated at 0CEh, following addresses depend on: ---
 1) number of initial values, found at [042h] ;usually 29h
 2) number of BB indices, found at [0CEh+[042h]] ;usually 02h
 3) number of RF indices, found at [043h] ;usually 02h
 --- Below example addresses assume above values to be set to 29h,02h,02h ---
 0CEh 29h Initial 8bit values for RF[0..28h]
 0F7h 1 Number of BB indices per channel
 0F8h 1 1st BB index
 0F9h 14 1st BB data for channel 1..14
 107h 1 2nd BB index
 108h 14 2nd BB data for channel 1..14
 116h 1 1st RF index
 117h 14 1st RF data for channel 1..14
 125h 1 2nd RF index
 126h 14 2nd RF data for channel 1..14
 134h 46 Unused (FFh-filled) |
| --- |

Below for both Type2 and Type3:

```

| 162h 1 Unknown (usually 19h..1Ch)
 163h 1 Unused (FFh) (Inside CRC16 region, with config_length=138h)
 164h 99h Unused (FFh-filled) (Outside CRC16 region, with config_length=138h)
 1FDh 1 DSi/3DS Wifi Board (01h=W015, 02h=W024, 03h=W028);\this was
 1FEh 1 DSi/3DS Wifi Flash (20h=With access point 4/5/6) ; FFh-filled on DS
 1FFh 1 DSi/3DS Same as on DS (FFh) ;/ |
| --- |

Most of the Wifi settings seem to be always the same values on all currently
existing consoles. Except for:

Values that are (obviously) different are the CRC16, and 4th-6th bytes of the
MAC address. Also, initial values for BB[01h] and BB[1Eh], and channel 1..14
values for BB[1Eh], and unknown entry [162h] contain different calibration
settings on all consoles.

Firmware v5 is having a new wifi ID [2Fh]=03h, and different RF[9] setting.

Firmware v6 (dslite) has wifi ID [2Fh]=05h, and same RF[9] setting as v5,
additionally, v6 and up have different 2nd-3rd bytes of the MAC address.

Moreover, a LOT of values are different with Type3 chips (ie. when [040h]=3).

**Note**

Unlike for Firmware User Settings, the Firmware Header (and Wifi Settings)
aren't stored in RAM upon boot. So the data must be retrieved via SPI bus by
software.

| DS Firmware Wifi Internet Access Points |
| --- |

**Connection data 1 at WifiFlash[00020h]*8-400h (eg. 01FA00h/03FA00h/07FA00h)**

**Connection data 2 at WifiFlash[00020h]*8-300h (eg. 01FB00h/03FB00h/07FB00h)**

**Connection data 3 at WifiFlash[00020h]*8-200h (eg. 01FC00h/03FC00h/07FC00h)**

These three 100h byte regions are used to memorize known internet access
points. The NDS firmware doesn't use these regions, but games that support
internet are allowed to read (and configure/write) them. The DSi firmware also
supports configuring these entries.

```

| Addr Siz Expl.
 000h 64 Unknown (usually 00h-filled) (no Proxy supported on NDS)
 040h 32 SSID (ASCII name of the access point) (padded with 00h's)
 060h 32 SSID for WEP64 on AOSS router (each security level has its own SSID)
 080h 16 WEP Key 1 (for type/size, see entry E6h)
 090h 16 WEP Key 2 ;\
 0A0h 16 WEP Key 3 ; (usually 00h-filled)
 0B0h 16 WEP Key 4 ;/
 0C0h 4 IP Address (0=Auto/DHCP)
 0C4h 4 Gateway (0=Auto/DHCP)
 0C8h 4 Primary DNS Server (0=Auto/DHCP)
 0CCh 4 Secondary DNS Server (0=Auto/DHCP)
 0D0h 1 Subnet Mask (0=Auto/DHCP, 1..1Ch=Leading Ones) (eg. 6 = FC.00.00.00)
 0D1h .. Unknown (usually 00h-filled)
 0E6h 1 WEP Mode (0=None, 1/2/3=5/13/16 byte hex, 5/6/7=5/13/16 byte ascii)
 0E7h 1 Status (00h=Normal, 01h=AOSS, FFh=connection not configured/deleted)
 0E8h 1 Zero (not SSID Length, ie. unlike as entry 4,5,6 on DSi)
 0E9h 1 Unknown (usually 00h)
 0EAh 2 DSi only: MTU (Max transmission unit) (576..1500, usually 1400)
 0ECh 3 Unknown (usually 00h-filled)
 0EFh 1 bit0/1/2 - connection 1/2/3 (1=Configured, 0=Not configured)
 0F0h 6 Nintendo Wifi Connection (WFC) 43bit User ID
 (ID=([F0h] AND 07FFFFFFFFFFh)*1000, shown as decimal string
 NNNN-NNNN-NNNN-N000) (the upper 5bit of the last byte are
 containing additional/unknown nonzero data)
 0F6h 8 Unknown (nonzero stuff !?!)
 0FEh 2 CRC16 for Entries 000h..0FDh (with initial value 0000h) |
| --- |

For connection 3: entries [0EFh..0FDh] - always zero-filled?

The location of the first data block is at the User Settings address minus
400h, ie. Firmware Header [00020h]*8-400h.

**Connection data 4 at WifiFlash[00020h]*8-A00h (eg. 01F400h) (DSi only)**

**Connection data 5 at WifiFlash[00020h]*8-800h (eg. 01F600h) (DSi only)**

**Connection data 6 at WifiFlash[00020h]*8-600h (eg. 01F800h) (DSi only)**

The DSi has three extra 200h-byte regions (for use DSi games, with the new
WPA/WPA2 encryption support, and with additional proxy support), these extra
regions are found under "Advanced Setup" in the DSi firmware's "Internet"
configuration menu.

```

| Addr Siz Expl.
 000h 32 Proxy Authentication Username (ASCII string, padded with 00's)
 000h 32 Proxy Authentication Password (ASCII string, padded with 00's)
 040h 32 SSID (ASCII string, padded with 00's) (see [0E8h] for length)
 060h .. Maybe same as NDS
 080h 16 WEP Key (zerofilled for WPA)
 0xxh .. Maybe same as NDS
 0C0h 4 IP Address (0=Auto/DHCP)
 0C4h 4 Gateway (0=Auto/DHCP)
 0C8h 4 Primary DNS Server (0=Auto/DHCP)
 0CCh 4 Secondary DNS Server (0=Auto/DHCP)
 0D0h 1 Subnet Mask (0=Auto/DHCP, 1..1Ch=Leading Ones) (eg. 6 = FC.00.00.00)
 0D1h .. Unknown (zerofilled)
 0E6h 1 WEP (00h=None/WPA/WPA2, 01h/02h/03h/05h/06h/07h=WEP, same as NDS)
 0E7h 1 WPA (00h=Normal, 10h=WPA/WPA2, 13h=WPS+WPA/WPA2, FFh=unused/deleted)
 0E8h 1 SSID Length in characters (01h..20h, or 00h=unused)
 0E9h 1 Unknown (usually 00h)
 0EAh 2 MTU Value (Max transmission unit) (576..1500, usually 1400)
 0ECh 3 Unknown (usually 00h-filled)
 0EFh 1 bit0/1/2 - connection 4/5/6 (1=Configured, 0=Not configured)
 0F0h 14 Zerofilled (or maybe ID as on NDS, if any such ID exists for DSi?)
 0FEh 2 CRC16 for Entries 000h..0FDh (with initial value 0000h)
 100h 32 Precomputed PSK (based on WPA/WPA2 password and SSID) ;\all zero
 120h 64 WPA/WPA2 password (ASCII string, padded with 00's) ;/for WEP
 160h 33 Zerofilled
 181h 1 WPA (0=None/WEP, 4=WPA-TKIP, 5=WPA2-TKIP, 6=WPA-AES, 7=WPA2-AES)
 182h 1 Proxy Enable (00h=None, 01h=Yes)
 183h 1 Proxy Authentication (00h=None, 01h=Yes)
 184h 48 Proxy Name (ASCII string, max 47 chars, padded with 00's)
 1B4h 52 Zerofilled
 1E8h 2 Proxy Port (16bit)
 1EAh 20 Zerofilled
 1FEh 2 CRC16 for Entries 100h..1FDh (with initial value 0000h) (0=deleted) |
| --- |

The location of the first data block (aka settings number 4) is at the User
Settings address minus A00h, ie. Firmware Header [00020h]*8-A00h.

Observe that NDS consoles do have NDS Firmware bootcode/data in that area, so
those new regions can exist on DSi only (or on homebrew NDS firmwares).
Presence of the new regions is indicated in Firmware Header [001FEh], that byte
is usually FFh=NDS or 20h=DSi, the DSi browser does internally replace FFh by
10h, and does then check if byte>=20h (ie. the new areas exist if the byte
is 20h..FEh).

Note that the Proxy feature can be used to redirect internet access (when using
a custom proxy server, one could redirect commercial games to homebrew servers;
as done by the [http://pbsds.net/](http://pbsds.net/)
 project) (actually the same should be possible with the DNS server entry,
possibly with less traffic).

**Note**

The location of the user settings & connection data varies (eg.
01Fxxxh=DSi, 03Fxxxh=NDS, 07Fxxxh=iQueDS).

**Nintendo Zone Beacons**

DSi games and DSi browser can reportedly also connect to Nintendo's public
access points (those that are announced via Nintendo Zone Beacons).

**Hidden Connection data at WifiFlash[00020h]*8-100h (eg. xxFD00h)**

This seems to hold an extra 100h-byte region (same as Connection data 1-3),
some (or all) NDS/DSi games seem to be capable of using it (although it isn't
shown in config menues). One theory is that it might be used for the Nintendo
Wi-Fi USB Connector?

| DS Firmware User Settings |
| --- |

**Current Settings (RAM 27FFC80h-27FFCEFh, DSi: 2FFFC80h-2FFFCEFh)**

**User Settings 0 (Firmware 3FE00h-3FEFFh) ;(DSi & iQue use different address,**

**User Settings 1 (Firmware 3FF00h-3FFFFh) ;see Firmware Header [020h])**

```

| Addr Size Expl.
 000h 2 Version (5) (Always 5, for all NDS/DSi Firmware versions)
 002h 1 Favorite color (0..15) (0=Gray, 1=Brown, etc.)
 003h 1 Birthday month (1..12) (Binary, non-BCD)
 004h 1 Birthday day (1..31) (Binary, non-BCD)
 005h 1 Not used (zero)
 006h 20 Nickname string in UTF-16 format
 01Ah 2 Nickname length in characters (0..10)
 01Ch 52 Message string in UTF-16 format
 050h 2 Message length in characters (0..26)
 052h 1 Alarm hour (0..23) (Binary, non-BCD)
 053h 1 Alarm minute (0..59) (Binary, non-BCD)
 054h 2
 056h 1 80h=enable alarm (huh?), bit 0..6=enable?
 057h 1 Zero (1 byte)
 058h 2x2 Touch-screen calibration point (adc.x1,y1) 12bit ADC-position
 05Ch 2x1 Touch-screen calibration point (scr.x1,y1) 8bit pixel-position
 05Eh 2x2 Touch-screen calibration point (adc.x2,y2) 12bit ADC-position
 062h 2x1 Touch-screen calibration point (scr.x2,y2) 8bit pixel-position
 064h 2 Language and Flags (see below)
 066h 1 Year (2000..2255) (when having entered date in the boot menu)
 067h 1 Unknown (usually 00h...08h or 78h..7Fh or so)
 068h 4 RTC Offset (difference in seconds when RTC time/date was changed)
 06Ch 4 Not used (FFh-filled, sometimes 00h-filled) (=MSBs of above?) |
| --- |

Below not stored in RAM (found only in FLASH memory)...

```

| 070h 2 Update counter (used to check latest) (must be 0000h..007Fh)
 072h 2 CRC16 of entries 00h..6Fh (70h bytes)
 074h 8Ch Not used (FFh-filled) (or extended data, see below) |
| --- |

Below extended data was invented for iQue DS (for adding the chinese language
setting), and is also included in Nintendo DSi models. Presence of extended
data is indicated in Firmware Header entry [1Dh].Bit6.

```

| 074h 1 Unknown (01h) (maybe version?)
 075h 1 Extended Language (0..5=Same as Entry 064h, plus 6=Chinese)
 (for language 6, entry 064h defaults to english; for compatibility)
 (for language 0..5, both entries 064h and 075h have same value)
 076h 2 Bitmask for Supported Languages (Bit0..6)
 (007Eh for iQue DS, ie. with chinese, but without japanese)
 (0042h for iQue DSi, chinese (and english, but only for NDS mode))
 (003Eh for DSi/EUR, ie. without chinese, and without japanese)
 078h 86h Not used (FFh-filled on iQue DS, 00h-filled on DSi)
 0FEh 2 CRC16 of entries 74h..FDh (8Ah bytes) |
| --- |

Note: The DSi does store the user settings in eMMC files (TWLCFGn.dat), that
files include NDS-style settings (username etc), plus additional DSi-specific
settings (country, parental controls, etc). For backwards compatibilty, the DSi
does also store a copy of those settings in NDS-style format in Wifi FLASH and
Main RAM at 2FFFC80h.

DSi SD/MMC Firmware System Settings Data Files

DSi Backlight level and DSi sound volume seem to be stored in the BPTWL chip
(or possibly in its attached I2C potentiometer).

**Language and Flags (Entry 064h)**

```

| 0..2 Language (0=Japanese, 1=English, 2=French, 3=German,
 4=Italian, 5=Spanish, 6..7=Reserved) (for Chinese see Entry 075h)
 (the language setting also implies time/data format)
 3 GBA mode screen selection (0=Upper, 1=Lower)
 4-5 Backlight Level (0..3=Low,Med,High,Max) (DS-Lite only)
 6 Bootmenu Disable (0=Manual/bootmenu, 1=Autostart Cartridge)
 7-8 ?
 9 Settings Lost (1=Prompt for User Info, and Language, and Calibration)
 10 Settings Okay (0=Prompt for User Info)
 11 Settings Okay (0=Prompt for User Info) (Same as Bit10)
 12 No function
 13 Settings Okay (0=Prompt for User Info, and Language)
 14 Settings Okay (0=Prompt for User Info) (Same as Bit10)
 15 Settings Okay (0=Prompt for User Info) (Same as Bit10) |
| --- |

The Health and Safety message is skipped if Bit9=1, or if one or more of the
following bits is zero: Bits 10,11,13,14,15. However, as soon as entering the
bootmenu, the Penalty-Prompt occurs.

Note: There are two User Settings areas in the firmware chip, at offset 3FE00h
and 3FF00h, if both areas have valid CRCs, then the current/newest area is that
whose Update Counter is one bigger than in the other/older area.

```

| IF count1=((count0+1) AND 7Fh) THEN area1=newer ELSE area0=newer |
| --- |

When changing settings, the older area is overwritten with new data (and
incremented Update Counter). The two areas allow to recover previous settings
in case of a write-error (eg. on a battery failure during write).

**Battery Removal**

Even though the battery is required only for the RTC (not for the firmware
flash memory), most of the firmware user settings are reset when removing the
battery. This appears to be a strange bug-or-feature of the DS bios, at least,
fortunately, it still keeps the rest of the firmware intact.

| DS Firmware Extended Settings |
| --- |

Extended Settings contain some additional information which is not supported by
the original firmware (current century, date/time formats, temperature
calibration, etc.), the settings are supported by Nocash Firmware, by the
no$gba emulator, and may be eventually also supported by other emulators. If
present, the values can be used by games, otherwise games should use either
whatever default settings, or contain their own configuration menu.

**Extended Settings - loaded to 23FEE00h (aka fragments of NDS9 boot code)**

```

| Addr Siz Expl.
 00h 8 ID "XbooInfo"
 08h 2 CRC16 Value [0Ch..0Ch+Length-1]
 0Ah 2 CRC16 Length (from 0Ch and up)
 0Ch 1 Version (currently 01h)
 0Dh 1 Update Count (newer = (older+1) AND FFh)
 0Eh 1 Bootmenu Flags
 Bit6 Important Info (0=Disable, 1=Enable)
 Bit7 Bootmenu Screen (0=Upper, 1=Lower)
 0Fh 1 GBA Border (0=Black, 1=Gray Line)
 10h 2 Temperature Calibration TP0 ADC value (x16) (sum of 16 ADC values)
 12h 2 Temperature Calibration TP1 ADC value (x16) (sum of 16 ADC values)
 14h 2 Temperature Calibration Degrees Kelvin (x100) (0=none)
 16h 1 Temperature Flags
 Bit0-1 Format (0=Celsius, 1=Fahrenheit, 2=Reaumur, 3=Kelvin)
 17h 1 Backlight Intensity (0=0ff .. FFh=Full)
 18h 4 Date Century Offset (currently 20, for years 2000..2099)
 1Ch 1 Date Month Recovery Value (1..12)
 1Dh 1 Date Day Recovery Value (1..31)
 1Eh 1 Date Year Recovery Value (0..99)
 1Fh 1 Date/Time Flags
 Bit0-1 Date Format (0=YYYY-MM-DD, 1=MM-DD-YYYY, 2=DD-MM-YYYY)
 Bit2 Friendly Date (0=Raw Numeric, 1=With Day/Month Names)
 Bit5 Time DST (0=Hide DST, 1=Show DST=On/Off)
 Bit6 Time Seconds (0=Hide Seconds, 1=Show Seconds)
 Bit7 Time Format (0=24 hour, 1=12 hour)
 20h 1 Date Separator (Ascii, usually Slash, or Dot)
 21h 1 Time Separator (Ascii, usually Colon, or Dot)
 22h 1 Decimal Separator (Ascii, usually Comma, or Dot)
 23h 1 Thousands Separator (Ascii, usually Comma, or Dot)
 24h 1 Daylight Saving Time (Nth)
 Bit 0-3 Activate on (0..4 = Last,1st,2nd,3rd,4th)
 Bit 4-7 Deactivate on (0..4 = Last,1st,2nd,3rd,4th)
 25h 1 Daylight Saving Time (Day)
 Bit 0-3 Activate on (0..7 = Mon,Tue,Wed,Thu,Fri,Sat,Sun,AnyDay)
 Bit 4-7 Deactivate on (0..7 = Mon,Tue,Wed,Thu,Fri,Sat,Sun,AnyDay)
 26h 1 Daylight Saving Time (of Month)
 Bit 0-3 Activate DST in Month (1..12)
 Bit 4-7 Deactivate DST in Month (1..12)
 27h 1 Daylight Saving Time (Flags)
 Bit 0 Current DST State (0=Off, 1=On)
 Bit 1 Adjust DST Enable (0=Disable, 1=Enable) |
| --- |

Note: With the original firmware, the memory region at 23FEE00h and up contains
un-initialized, non-zero-filled data (fragments of boot code).

| DS File Formats |
| --- |

DS Files - 2D Video

DS Files - 3D Video

DS Files - Sound (SDAT etc.)

DS Files - Text Messages (MESG)

DS Files - Text Manuals

DS Cartridge Nitro Font Resource Format

DS Cartridge NitroROM and NitroARC File Systems

LZ Decompression Functions

LZ Decompression Functions ASH0

ZIP Decompression

PNG Bitmaps

DS Encrypted Arika Archives with ALZ1 compression

DS Files - Vicarious Vision GFC/GOB Archives with ZLIB compression

| DS Files - Text Messages (MESG) |
| --- |

MESG files contain localized strings (like "Okay" and "Cancel" and longer text
messages). Different languages are stored separate message files, usually with
.bmg extension. MESG files are used in NDS/DSi titles (in little endian), and
also on Wii (in big endian).

DSi titles are: DSi Shop, Launcher, System Settings, and DSi Camera (however,
the DSi-to-3DS Transfer Tool uses the newer MsgPrjBn/MsgStdBn format; despite
of being a DSi program).

**MESG Header**

```

| 000h 8 ID "MESGbmg1" ;or "GSEM1gmb" in Super Mario 64 DS
 008h 4 Total Filesize ;or Filesize+1 in Super Mario 64 DS
 00Ch 4 Number of Chunks (2=INF1+DAT1, 3=INF1+DAT1+MID1)
 010h 1 Encoding (1=CP1252, 2=UTF-16, 3=Shift-JIS, 4=UTF-8)
 011h 15 Padding (0) |
| --- |

Encoding UTF-16 appears to be most common (Super Mario 64 DS uses Shift-JIS).

**INF1 Chunk (Message Info)**

It comes just after the BMG header. It contains information (like pointers)
about the messages.

```

| 000h 4 Chunk ID "INF1" ;or "1FNI" in Super Mario 64 DS
 004h 4 Chunk Size
 008h 2 Number of messages (N)
 00Ah 2 Size of each INF data in bytes ;or in BITs in Super Mario 64 DS
 00Ch 4 "BMG file ID = ID for this BMG file (usually 0)"
 010h N*siz Message Info (32bit offset from DAT1+8, and optional attributes) |
| --- |

For each message, there is a INF data about it. At Wii Fit, there is just the
message offset. At Wii Fit Plus, there is two another fields.

```

| 000h 4 Offset to the message (after DAT1+8 section header)
 004h siz-4 Attributes/flags (if entrysize is bigger than 4 bytes) |
| --- |

The optional Attribute bits could be used to select different font types or
window styles.

**DAT1 Chunk (Message Strings)**

```

| 000h 4 Chunk ID "DAT1" ;or "1TAD" in Super Mario 64 DS
 004h 4 Chunk Size ;or Size+1 in Super Mario 64 DS
 008h .. Message strings (usually UTF-16, depending on Encoding in header) |
| --- |

The DSi Camera uses both char 001Ah and 0025h for escape codes.

UTF-16 string characters:

```

| 0000 End of String (except inside Escape sequences)
 000A Linebreak
 001A,nn,command,parameters Escape Sequences (nn=length in bytes)
 001A,08,00,0000,00xx Set font size (64h=100%=Normal Size)
 001A,08,00,0001,00xx Set text color to xx
 001A,08,01,0000,24xx Draw Unicode char U+2460..246E ;"(1)"..("15)"
 001A,08,01,0000,xxxx Draw Unicode char U+E068..F12B ;custom?
 001A,06,02,0000 Draw Name of current player
 001A,0A,02,0010,000x,000w Draw Integer from index x with w digits
 001A,08,02,0011,00xx Unknown (with xx=0..8)
 001A,08,02,0012,0000 Draw Name of a player
 001A,08,02,0013,0000 Unknown
 001A,08,02,0014,0000 Unknown
 001A,08,02,0015,0000 Unknown
 001A,0A,02,0016,0000,0000 Unknown
 001A,08,02,0017,0000 Unknown
 001A,08,02,0020,0000 Draw Name of a Wii friend
 001A,08,03,0010,0000 Unknown
 001A,0C,04,0000,000x,yyyy,zzzz Unknown (x=0..1, y=0524..14A4, and z=y+1)
 0025,00xx,00yy,00zz Escape codes in form of "%xyz" (or similar)
 00xx ASCII Characters 20h..7Eh
 E0xx Custom button symbols (eg. in DSi Launcher) |
| --- |

Shift-JIS (or whatever) strings in Super Mario 64 DS:

```

| 0D Linebreak?
 10..1F Escape codes?
 xx,xx Unknown (doesn't really look like english Shift-JIS characters)
 FF End of String |
| --- |

**MID1 Chunk (Message IDs) (if any)**

```

| 000h 4 Chunk ID "MID1"
 004h 4 Chunk Size
 008h 2 Number of messages (same as in INF1 block)
 00Ah 2 Unknown (usually 1000h)
 00Ch 4 Padding (0)
 010h 4*N Message IDs |
| --- |

Messages can be repeated in the different files (such like menu and ingame), if
so, all messages with the same message ID of the same language are always
having the same text, no differences between the text files can be found.

"Elements with the same table index are attributes (not IDs?) for the same
string."

**FLW1 and FLT1 Chunks (if any)**

Some games are reportedly having additional "FLW1" and "FLT1" chunks. Unknown
which games, and unknown what for, and unknown if such chunks exist in any DS
games (or only in Wii games or whatever).

Thanks to [http://wiki.tockdom.com/wiki/BMG](http://wiki.tockdom.com/wiki/BMG)

| DS Files - Text Manuals |
| --- |

Manual files are common in DSiware download titles, unknown if DS/DSi
cartridges did also use that files (or if they did stick with print manuals).

Used in DSi Flipnote, Sudoku, Paper Plane, Deep Psyche, Dr. Mario, DSi-to-3DS
Transfer Tool.

Usually found in "rom:\..\manpages_narc.blz" (with two folders in the .blz
file, "arc" for the manual, and "gpArc" with a quick reference on how to read
the manual; the latter containing only two "pages", without "content" file).

**NTLI File (manual language info) (in "ntli" folder)**

```

| 000h 4 ID "NTLI"
 004h 2 Byte Order (FEFFh)
 006h 2 Version (can be 0200h)
 008h 4 Total Filesize
 00Ch 2 Header Size (10h)
 00Eh 4 Number of Chunks (usually 1 = mtl1) |
| --- |

**mtl1 Chunk**

```

| 000h 4 Chunk ID "mtl1"
 004h 4 Chunk Size
 008h 4 Number of supported languages
 00Ch 2*N Language IDs (two-letter ASCII spelled backwards) |
| --- |

Known IDs are ne=English, rf=French, se=Spanish, ti=Italian, ed=German.

EUR games do usually support 5 languages (Deep Psyche supports less).

IDs for Japanese/Chinese/Korean are unknown (also unknown if US has different
IDs for english/spanish etc, and if portugese or so do exist on DSi).

Note: The two-letters do directly translate to the "ntmc\xx\" and "ntpg\xx\"
folder names.

**NTMC File (manual contents) (in "ntmc\xx\" folders)**

The content contains "hyperlinks" to the actual chapters. A chapter can be
divided into subsections (with the subsections being shown at the right of the
main chapter name).

```

| 000h 4 ID "NTMC"
 004h 2 Byte Order (FEFFh)
 006h 2 Version (can be 0200h)
 008h 4 Total Filesize
 00Ch 2 Header Size (10h)
 00Eh 4 Number of Chunks (usually 3 = nap1+txp1+mtc1) |
| --- |

**nap1 Chunk**

```

| 000h 4 Chunk ID "nap1"
 004h 4 Chunk Size
 008h 4 Number of chapters (aka pages) minus 1? (eg. 18h=19h)
 00Ch 4*N Offsets to filenames (from nap+0Ch)
 ... .. Filenames (ASCII, terminated by 00h) |
| --- |

**txp1 Chunk**

```

| 000h 4 Chunk ID "txp1"
 004h 4 Chunk Size
 008h 4 Number of something?? minus 1 (eg. 25h=26h)
 00Ch 4*N Offsets to something?? (from txp1+0Ch)
 ... .. Somewhat corrupt UTF-16 strings (many aborted with char 20xxh) |
| --- |

**mtc1 Chunk**

```

| 000h 4 Chunk ID "mtc1"
 004h 4 Chunk Size
 008h 4 Number of dunno what (eg. 0Dh=Much more?)
 00Ch 2*? 16bit Indices in txp1? (eg. 0000h..0025h) |
| --- |

**NTPG File (manual pages) (or rather chapters?) (in "ntpg\xx\" folders)**

The pages can contain text, tables, and symbols/images/screenshots (there
appears to be no support for hyperlinks inside of the pages; probably because
they default to be displayed on upper screen, without touchscreen support).

```

| 000h 4 ID "NTPC"
 004h 2 Byte Order (FEFFh)
 006h 2 Version (can be 0200h)
 008h 4 Total Filesize
 00Ch 2 Header Size (10h)
 00Eh 4 Number of Chunks (usually 7 = nap1+txp1+pag1+pan1+pas1+txt1+pae1) |
| --- |

**nap1 Chunk**

```

| 000h 4 Chunk ID "nap1"
 004h 4 Chunk Size (10h)
 008h 4 Zero (unlike as in NTMC file)
 00Ch 4 Unknown (4) |
| --- |

**txp1 Chunk**

```

| 000h 4 Chunk ID "txp1"
 004h 4 Chunk Size
 008h 4 Number of something?? minus 1 (eg. 02h=03h)
 00Ch 4*N Offsets to something?? (from txp1+0Ch)
 ... .. UTF-16 strings (Headline, Body, Footer?) |
| --- |

**pag1 Chunk**

```

| 000h 4 Chunk ID "pag1"
 004h 4 Chunk Size (10h)
 008h 2? 0000h text color black?
 00Ah 2? 0160h link color or so?
 00Ch 2? 7FFFh bg color white?
 00Eh 2? 0000h |
| --- |

**pan1 Chunk**

```

| 000h 4 Chunk ID "pan1"
 004h 4 Chunk Size (10h)
 008h 2? 0000h
 00Ah 2? 0000h
 00Ch 2? 0100h
 00Eh 2? 0160h link color or so? |
| --- |

**pas1 Chunk (start?)**

```

| 000h 4 Chunk ID "pan1"
 004h 4 Chunk Size (0Ch)
 008h 4? 00000001h |
| --- |

**txt1 Chunk**

```

| 000h 4 Chunk ID "txt1"
 004h 4 Chunk Size
 008h 2? 0008h
 00Ah 2? 0008h
 00Ch 2? 00F0h
 00Eh 2? 0150h
 010h 2? 0001h
 012h 2? 0015h Number of 8-byte entries? start/end line-wrapping list?
 014h 2? 000Dh
 016h 2? 0010h
 018h 4? 00000000h
 01Ch 4? 00001CE7h
 020h 4? 00000000h
 024h 4? 00000000h
 028h N*8 Unknown 8-byte entries? (00xxh,0010h,0000h,00yyh)
 Or maybe positioning for symbols/images/tables? |
| --- |

**pae1 Chunk (end?)**

```

| 000h 4 Chunk ID "pae1"
 004h 4 Chunk Size (08h) |
| --- |

**NTTF File (manual graphics/symbols) (in "nttf" folder)**

```

| 000h 2 Bitmap Width in bytes (eg. 0Fh, 10h, 14h, 40h, 60h)
 002h 2 Bitmap Height (eg. 12h, 0Eh, 14h, 30h, 47h)
 004h 1 Unknown, maybe Texture Format? (always 04h=256-Color?)
 005h 1 Unknown, maybe Color0.alpha? (00h or 01h) (often same as [007h])
 006h 2 Number of Palette entries (usually 10h, 80h, or 100h)
 008h 8 Zerofilled
 010h .. Palette data (with 16bit values in range 0000h..7FFFh)
 ... .. Bitmap data (seems to be always 8bpp) |
| --- |

Width/height can be odd (the bitmap doesn't contain any padding to even
width/height... except, maybe padding to bytesize in case color depths other
than 8bpp should exist?).

**Notes**

DSi manuals are using the TWLFontTable.dat system font (Flipnote, at least).

3DS manuals consist of CLYT and CLIM files (inside of a manual BCMA darc
archive in a NCCH file).