# Some seems to use this ..

> Source: https://problemkaputt.de/gbatek.htm
> Section: Some seems to use this ..

3DS Config - ARM7 Registers (GBA/NDS/DSi Mode) 
| |
| --- |

 The somewhat misnamed "ARM7" registers are actually ARM9 registers, used to configure GBA/NDS/DSi mode.
 
```

| 10018000h 1 ARM7_CNT ;\GBA/NDS/DSi mode
 10018080h 20h ARM7_BOOTCODE ;/
 10018100h 2 ARM7_SAVE_TYPE ;\GBA savedata
 10018104h 2 ARM7_SAVE_CNT ;/
 10018108h 2 ARM7_RTC_CNT ;\
 10018110h 4 ARM7_RTC_BCD_DATE ;
 10018114h 4 ARM7_RTC_BCD_TIME ; GBA real time clock
 10018118h 4 ARM7_RTC_HEX_TIME ;
 1001811Ch 4 ARM7_RTC_HEX_DATE ;/
 10018120h 4 ARM7_SAVE_CFG_? ;\
 10018124h 4 ARM7_SAVE_CFG_? ; GBA savedata write/erase timings?
 10018128h 4 ARM7_SAVE_CFG_? ;
 1001812Ch 4 ARM7_SAVE_CFG_? ;/
 04700000h 4 <-- on ARM7 side, disable bootcode overlay |
| --- |

For specs on GBA/NDS/DSi mode:

GBA Reference

NDS Reference

DSi Reference

The ARM7_RTC_xxx registers are emulating the GBA cartridge RTC. The RTC for
NDS/DSi mode is emulated separately via registers in the 3DS GPIO register
space:

3DS GPIO Registers

The GBA/NDS/DSi video must be forwarded to 3DS framebuffers via ARM11 DMA:

3DS Video LGY Registers (Legacy GBA/NDS Video to Framebuffer)

GBA Audio requires enabling amplifiers via TSC register, and enabling GBA sound
via CODEC_SNDEXCNT.

NDS/DSi sound probably requires something similar, and more extensive TSC
initialization for NDS or DSi touchscreen mode; and adjusting the TSC
calibration points depending on whether using LGYFB scaling.

**10018000h - ARM7_CNT (R/W)**

```

| 0-1 Console Mode (0=3DS, 1=NDS/DSi, 2=GBA, 3=Auto-replaced by 0) (R/W)
 2-31 Unused (0) |
| --- |

After writing ARM7_CNT, apply the mode value by writing [10141100h].bit15=1
(CFG11_TWLMODE_BOOT). GBA/NDS/DSi mode will start ARM7, executing the
ARM7_BOOTCODE.

NDS/DSi mode changes the ARM9 memory map (to survive the changed memory map,
ARM9 should execute ITCM code during the mode change). GBA mode does keep ARM9
in 3DS mode. ARM11 is always kept running in 3DS mode.

**10018080h..1001809Fh - ARM7_BOOTCODE (32 bytes) (R/W)**

These 32 bytes do overlay the exception vectors at 00000000h..0000001Fh in the
ARM7 BIOS ROMs (for GBA/NDS/DSi mode). The ARM7 reset vector opcode(s) are
executed after switching to GBA/NDS/DSi mode (see ARM7_CNT
CFG11_TWLMODE_BOOT.bit15).

After booting, ARM7 can disable the BOOTCODE overlay by writing [4700000h]=1.

**ARM7:04700000h - on ARM7 side, disable bootcode overlay (W)**

```

| 0 Disable ARM7_BOOTCODE overlay (0=No, 1=Disable) (SET-ONCE)
 1-31 Unused (0) |
| --- |

```

| ____________________ GBA Mode Cartridge Savedata and RTC ____________________ |
| --- |

**10018100h - ARM7_SAVE_TYPE (R/W)**

```

| 0-3 GBA Cartridge Type (00h-0Fh, see below) (R/W)
 4-15 Unused (0) |
| --- |

Type values (same as used in the footer of the GBA ROM-image):

```

| 00h = ROM 16.0Mbyte, EEPROM 0.5Kbyte (in upper 16Mbyte of ROM area)
 01h = ROM 31.9Mbyte, EEPROM 0.5Kbyte (in upper 100h byte of ROM area)
 02h = ROM 16.0Mbyte, EEPROM 8Kbyte (in upper 16Mbyte of ROM area)
 03h = ROM 31.9Mbyte, EEPROM 8Kbyte (in upper 100h byte of ROM area)
 04h = ROM 32Mbyte, FLASH 64Kbyte, RTC ;\(FLASH ID=3D1Fh, Atmel)
 05h = ROM 32Mbyte, FLASH 64Kbyte ;/
 06h = ROM 32Mbyte, FLASH 64Kbyte, RTC ;\(FLASH ID=D4BFh, SST)
 07h = ROM 32Mbyte, FLASH 64Kbyte ;/
 08h = ROM 32Mbyte, FLASH 64Kbyte, RTC ;\(FLASH ID=1B32h, Panasonic)
 09h = ROM 32Mbyte, FLASH 64Kbyte ;/
 0Ah = ROM 32Mbyte, FLASH 128Kbyte, RTC ;\(FLASH ID=09C2h, Macronix)
 0Bh = ROM 32Mbyte, FLASH 128Kbyte ;/
 0Ch = ROM 32Mbyte, FLASH 128Kbyte, RTC ;\(FLASH ID=1362h, Sanyo)
 0Dh = ROM 32Mbyte, FLASH 128Kbyte ;/
 0Eh = ROM 32Mbyte, SRAM 32Kbyte ;-SRAM
 0Fh = ROM 32Mbyte ;-Raw ROM |
| --- |

**10018104h - ARM7_SAVE_CNT (R/W)**

```

| 0 Savedata mapping (0=GBA:0E000000h, 1=3DS:08080000h) (R/W)
 1-15 Unused (0) |
| --- |

Below registers are for emulating the GBA cartridge RTC

GBA Cart Real-Time Clock (RTC)

**10018108h - ARM7_RTC_CNT (W and R)**

```

| 0 Write (0=No change, 1=Apply RTC_BCD and RTC_HEX) (W)
 1 Read (0=No change, 1=Latch RTC_BCD and RTC_HEX) (W)
 2-13 Unused (0)
 14 Write Error Flag (0=Okay, 1=Error, invalid data) (R)
 15 Write/Read Busy Flag (0=Ready, 1=Busy) (R) |
| --- |

To get the current time:

```

| Set Read flag, wait until busy=0, then read RTC_HEX and/or RTC_BCD registers |
| --- |

To set the current time:

```

| Write RTC_HEX and RTC_BCD registers, set Write flag, then wait until busy=0 |
| --- |

The initial time on power-up in 01 Jan 2000, 00:00:00. The actual battery
backed time can be obtained from MCU[30h..36h].

The GBA software can access the RTC via port 080000C4h, 080000C6h, 080000C8h.

**10018110h - ARM7_RTC_BCD_DATE (R=Latched Read value, W=Written value)**

```

| 0-7 Year BCD (00h..99h)
 8-15 Month BCD (01h..12h)
 16-23 Day BCD (01h..31h)
 24-31 Day of Week (00h..06h) (WHAT=Monday?) |
| --- |

**10018114h - ARM7_RTC_BCD_TIME (R=Latched Read value, W=Written value)**

```

| 0-7 Hour BCD (00h..23h) (always 24-hours, even in AM/PM mode)
 8-15 Minute BCD (00h..59h)
 16-23 Second BCD (00h..59h)
 24-31 Zero (00h) |
| --- |

The HEX registers below contain some control flags, and some weird signed
offsets for the time/date values (the hardware does automatically
increase/decrease the BCD time/date using that offsets; set the offsets to zero
to avoid that).

**10018118h - ARM7_RTC_HEX_TIME (R=Latched Read value, W=Written value)**

```

| 0-6 Second (signed -40h..+3Fh, usually 00h..3Bh)
 7 Ctrl bit7 (0=Normal, 1=Time lost, force 1st Jan 2000, 00:00:00)
 8-14 Minute (signed -40h..+3Fh, usually 00h..3Bh)
 15 Ctrl bit6 (0=12 hour, 1=24 hour) (for GBA side, both with AM/PM flag)
 16-21 Hour (signed -20h..+1Fh, usually 00h..17h)
 22-23 Unused (0)
 24-27 Day of Week (signed -08h..+07h, usually 00h..06h)
 28 Ctrl bit1 (IRQ duty/hold related?)
 29 Ctrl bit3 (Per Minute IRQ)
 30 Ctrl bit5 (Unknown?)
 31 Error ? (0=Normal, 1=Triggers error) |
| --- |

**1001811Ch - ARM7_RTC_HEX_DATE (R=Latched Read value, W=Written value)**

```

| 0-15 Days (unsigned, 0000h..8EACh=100 years, Bigger=Triggers error)
 16-31 Unknown (can be 0000h..FFFFh) |
| --- |

**10018120h - ARM7_SAVE_CFG_? - R/W mask 00FFFFFFh (reset=007FD000h=8376320)**

**10018124h - ARM7_SAVE_CFG_? - R/W mask 00FFFFFFh (reset=007FD000h=8376320)**

**10018128h - ARM7_SAVE_CFG_? - R/W mask 000FFFFFh (reset=000000E0h=224)**

**1001812Ch - ARM7_SAVE_CFG_? - R/W mask 000FFFFFh (reset=00051000h=331776)**

Maybe write/erase timings, or so?

```

| ______________________ Memory Maps in GBA/NDS/DSi Modes ______________________ |
| --- |

**Memory map in GBA Mode**

```

| 3DS --> GBA
 08080000h --> 0E000000h, GBA Cart FLASH/SRAM/EEPROM (max 128Kbyte)
 080A0000h --> 06000000h, GBA 2D-Engine VRAM (64K+32Kbyte)
 080B8000h --> 03000000h, GBA Fast WRAM (32Kbyte)
 080C0000h --> 02000000h, GBA Slow WRAM (256Kbyte)
 20000000h --> 08000000h, GBA Cart ROM (32Mbyte max) |
| --- |

Cart ROM is unstable: About 99% reads are reading from addr+20h (within
800h-byte pages), and about 1% are randomly reading as wanted from address+0.

```

| see FCRAM_MODE.bit0 for workaround |
| --- |

When in GBA mode, ARM9 seems to be no longer able to access the remapped ARM9
memory blocks (ARM9 only sees 00h's)? And, ARM9/ARM11 seem to HANG (without any
exception) when trying to access Main RAM at 20000000h?

**Memory map in NDS/DSi Mode**

```

| 3DS --> NDS/DSi
 08000000h --> 06800000h, NDS VRAM A (128Kbyte)
 08020000h --> 06820000h, NDS VRAM B (128Kbyte)
 08040000h --> 06840000h, NDS VRAM C (128Kbyte)
 08060000h --> 03800000h, NDS ARM7 WRAM (64Kbyte) (ARM7 only)
 08070000h --> 06898000h, NDS VRAM H (32Kbyte)
 08078000h --> 068A0000h, NDS VRAM I (16Kbyte)
 08080000h --> 06860000h, NDS VRAM D (128Kbyte)
 080A0000h --> 06880000h, NDS VRAM E (64Kbyte)
 080B0000h --> 06890000h, NDS VRAM F (16Kbyte)
 080B4000h --> 06894000h, NDS VRAM G (16Kbyte)
 080B8000h --> 03000000h, NDS Shared RAM (32Kbyte) (initially mapped to ARM9)
 080C0000h --> 03xxxxxxh, DSi New Shared WRAM A (256Kbyte) (Misc)
 10174000h --> 04804000h, NDS Wifi RAM (8Kbyte)
 1FF00000h --> 03xxxxxxh, DSi New Shared WRAM B (256Kbyte) (DSP Code)
 1FF40000h --> 03xxxxxxh, DSi New Shared WRAM C (256Kbyte) (DSP Data)
 20000000h --> 02000000h, NDS Main RAM (max 16MByte) ;\only each 4th
 20000000h --> 0C000000h, DSi Main RAM (max 32Mbyte) ;/halfword used
 ITCM/DTCM --> ITCM/DTCM, NDS ITCM/DTCM (32K+16K, same mapping as in 3DS mode)
 FF-filled --> 08000000h, GBA Cart ROM/SRAM (32MB+64K) (empty, FFh-filled) |
| --- |

Main RAM needs enable in EXMEMCNT, Main RAM uses only the lower 16bit of the
64bit FCRAM data bus (ie. only each 4th halfword is used). OAM/Palette need
enable in POWCNT1. VRAM needs enable in VRAMCNT. New Shared WRAM needs MBK.
Wifi RAM needs POWCNT2.

Booting NDS/DSi titles requires initializing various ARM and TSC registers, and
initial RAM content:

BIOS RAM Usage

**GBA Titles**

Nintendo did never actually release GBA titles for 3DS (except, they did
release ten "ambassador" GBA titles as a surprise gift for people who had
connected to the eshop during the first some months after the Old3DS release
date; that was before 3DS XL and 2DS and New3DS were released, so those GBA
titles seem to exist for original Old3DS only; the System Transfer tool might
allow to transfer them from Old3DS to New3DS though).

**GBA Footers**

GBA ROM-images are reportedly stored in .code files (in NCCH .app files, which
can also contain the usual NCCH icon/banner/logo).

Nintendo is using 360h-byte footers in GBA ROM-images (at the end of the .code
file?).

```

| Config Data:
 000h 4 Unknown (usually 0) (maybe ROM-image Offset, or version)
 004h 4 ROM-image Size (usually Romsize)
 008h 4 Cartridge Type (Port 10018100h, ARM7_SAVE_TYPE)
 00Ch 4 Unknown (usually 0000FFFFh) (guess: savedata fillvalue?)
 010h 4 Unknown (Port 10018120h, reportedly 1561662 or 2607238) ;\maybe
 014h 4 Unknown (Port 10018124h, reportedly 156166 or 577077 ) ; write
 018h 4 Unknown (Port 10018128h, reportedly 134 or 388 ) ; erase
 01Ch 4 Unknown (Port 1001812Ch, reportedly 187667 or 201072 ) ;/timings?
 020h 4 LCD Ghosting (01h..FFh) (uh, what is that?)
 024h 300h LCD Video LUT (guess: maybe for Port 10400484h/10400584h or so?)
 324h 0Ch Padding (0)
 1st Descriptor:
 330h 4 Descriptor Type (00h=ROM-Image)
 334h 4 ROM-image Offset (usually 0)
 338h 4 ROM-image Size (usually Romsize)
 33Ch 4 Padding (0)
 2nd Descriptor:
 340h 4 Descriptor Type (01h=Config Data)
 344h 4 Config Offset (usually Romsize+0)
 348h 4 Config Size (usually 324h)
 34Ch 4 Padding (0)
 Footer Entrypoint (in last 10h-byte of .code file):
 350h 4 GBA Footer ID (".CAA")
 354h 4 Maybe Version (must be 1)
 358h 4 Descriptor List Offset (usually Romsize+330h)
 35Ch 4 Descriptor List Size (usually 20h) (2*10h) |
| --- |

**lcd_ghosting_enum:**

```

| 0xFFFF: no # Some seems to use this ..
 0xFF: no
 0xF0: little
 0xC0: official_standard
 0x80: official_more
 0x90: official_more_plus # ?
 0x20: insane
 0x01: max |
| --- |

Ghosting seems to be an alpha value for blending the picture with the previous
frame, which was itself blended with the previous frames, thus generating a
horribly smeared picture resembling old 8bit monochrome gameboys (unknown why
Nintendo has used that effect that for GBA titles).

**GBA Savedata**

Upon power-off, GBA savedata is usually temporarily stored in a small eMMC
partition, and later copied to the actual savedata file upon reboot. The file
format and filename for that savedata are unknown...?

**ARM7 Registers Misc...**

ARM7 has the GBA BIOS implemented in hardware. The BIOS is completely identical
to the GBA BIOS on NDS consoles (ie. with [3F0Ch]=01h).

The system is booted silently by calling SWI 01h (aka RegisterRamReset),
followed by jumping to the code that does SWI 00h (aka SoftReset) to finish
booting. The boot splash is still in BIOS, however, and can be seen by calling
or replacing one of the previous interrupts with SWI 26h (aka HardReset).

ARM7_BOOTCODE: "This is the first code that will be run after execution begins.
TwlProcess9 uses this to put ARM7 in a loop (TWL), and to set the POSTFLG and
branch to more copied code (GBA)."