# Configconfig9Registers

> Source: https://problemkaputt.de/gbatek.htm
> Section: Configconfig9Registers

3DS Config - CONFIG9 Registers 
| |
| --- |

 **CONFIG9 Registers (ARM9 only)**
 
```

| Address Width Old3DS Name Used by
 10000000h 1 Yes CFG9_SYSPROT9 Boot9
 10000001h 1 Yes CFG9_SYSPROT11 Boot9
 10000002h 1 Yes CFG9_RST11 Boot9
 10000004h 4 Yes CFG9_DEBUG_CTL
 10000008h 1 Yes CFG9_AES Boot9, Process9, TwlProcess9
 1000000Ch 2 Yes CFG9_CARD_CTL Process9
 10000010h 1 Yes CFG9_CARD_POWER Process9
 10000012h 2 Yes CFG9_CARD_INSERT_DELAY Boot9, Process9
 10000014h 2 Yes CFG9_CARD_PWROFF_DELAY Boot9, Process9
 10000020h 2 Yes CFG9_SDMMC_CTL Process9
 10000100h 2 Yes CFG9_UNKNOWN
 10000200h 1 No CFG9_EXTMEMCNT9 (New3DS) NewKernel9
 10000FFCh 4 Yes CFG9_MPCORECFG
 ---------
 10010000h 4 Yes CFG9_BOOTENV
 10010010h 1 Yes CFG9_UNITINFO Process9
 10010014h 1 Yes CFG9_TWLUNITINFO Process9 |
| --- |

**10000000h - CFG9_SYSPROT9**

```

| 0 Disables ARM9 bootrom "(+8000h)" when set to 1, and enables access to
 FCRAM. Cannot be cleared to 0 once set to 1. Boot9
 1 Disables OTP area when set to 1. Cannot be cleared to 0 once set to 1.
 NewKernel9Loader, Process9
 2-7 Unused (0) |
| --- |

On Old 3DS, NATIVE_FIRM reads CFG9_SYSPROT9 to know whether it has previously
initialized the TWL console-unique keys using the OTP data. After setting the
TWL console-unique keys, NATIVE_FIRM sets CFG9_SYSPROT9 bit 1 to disable the
OTP area. In subsequent FIRM launches prior to the next reset, NATIVE_FIRM will
see that the OTP area is disabled, and skip this step.

On New 3DS, the above is instead done by the Kernel9 loader. In addition to
using the OTP data for initializing the TWL console-unique keys, the Kernel9
loader will generate the decryption key for NATIVE_FIRM. The final keyslot for
NATIVE_FIRM is preserved, so that at a non-reset FIRM launch, the keyslot can
be reused, since the OTP would then be inaccessible.

**10000001h - CFG9_SYSPROT11**

```

| 0 Disables ARM11 bootrom "(+8000h)" when set to 1, and enables access
 to FCRAM. Cannot be cleared to 0 once set to 1. Boot9
 1-7 Unused (0) |
| --- |

**10000002h - CFG9_RST11**

```

| 0 Presumably takes ARM11 out of reset. Cannot be set to 1 once it has
 been cleared.
 Reportedly: Bit0 is actually for "write-protecting the bootrom area"?
 1-7 Unused (0) |
| --- |

**10000004h - CFG9_DEBUG_CTL**

```

| 0-31 Whatever, debug related (readonly, always zero on retail consoles) |
| --- |

**10000008h - CFG9_AES (R/W)**

```

| 0-1 Unknown (R/W)
 2-3 AES related? Value 3 written after write to AES_CTL (R/W)
 4-7 Unused (0) |
| --- |

**1000000Ch - CFG9_CARD_CTL - Gamecard Controller Select (R/W)**

```

| 0-1 Gamecard ROM controller (0=NTRCARD, 1=?, 2=CTRCARD0, 3=CTRCARD1)
 2-3 Unused (0)
 4 Gamecard SPI_CARD mode (0=Manual, 1=FIFO)
 5-7 Unused (0)
 8 Gamecard SPI controller (0=NTRCARD, 1=SPI_CARD)
 9-11 Unused (0)
 12 Unknown...? (R/W)
 13-15 Unused (0) |
| --- |

There are three controllers for ROM cartridge commands:

```

| xxx0h/xxx1h? NTRCARD (8-byte commands) (bit1=0) (Port 10164000h)
 xxx2h CTRCARD0 (16-byte commands) (bit1=1, bit0=0) (Port 10004000h)
 xxx3h CTRCARD1 (16-byte commands) (bit1=1, bit0=1) (Port 10005000h) |
| --- |

And three controllers for SPI-bus cartridge savedata:

```

| x0x0h/x0x1h NTRCARD Manual NDS-style (bit8=0, bit1=0) (Port 10164000h)
 x0x2h/x0x3h None (bit8=0, bit1=1) (N/A)
 x10xh SPI_CARD in Manual Mode (bit8=1, bit4=0) (Port 1000D000h)
 x11xh SPI_CARD in FIFO Mode (bit8=1, bit4=1) (Port 1000D800h) |
| --- |

The deselected controllers are disconnected from the cartridge bus (and tend to
return data=FFh when trying to read from the cartridge).

**10000010h - CFG9_CARD_POWER (same as SCFG_MC on DSi) (R/W)**

```

| 0 NDS Slot Game Cartridge (0=Inserted, 1=Ejected) (R)
 1 NDS Slot Unknown/Unused (0)
 2-3 NDS Slot Power State (0=Off, 1=On+Reset, 2=On, 3=RequestOff) (R/W)
 4-15 Unused (0) |
| --- |

Same as SCFG_MC on DSi (but with the bits for 2nd cartridge slot removed). See
DSi specs for cartridge power on/off sequences.

DSi Control Registers (SCFG)

**10000012h - CFG9_CARD_INSERT_DELAY (usually 1988h = 100ms) (R/W)**

**10000014h - CFG9_CARD_PWROFF_DELAY (usually 264Ch = 150ms) (R/W)**

Same as 4004012h/4004014h on DSi, see DSi SCFG registers for details.

```

| 0-15 Delay in 400h cycle units (at 67.027964MHz) ;max FFFFh=ca. 1 second |
| --- |

There are related IRQs on ARM9 and ARM11 side (CGC aka Change Gamecard or so):

```

| ARM9 IF.bit25, and ARM11 Interrupt 74h ;at begin of PWROFF delay
 ARM9 IF.bit26, and ARM11 Interrupt 75h ;at end of INSERT delay |
| --- |

**10000020h - CFG9_SDMMC_CTL**

This register controls power of multiple ports/controllers and allows to map
controller 3 to ARM9 or ARM11. The SD card can be accessed on ARM11 by setting
bit8 and clearing bit9.

```

| 0 Controller 1/3 port 0 power (0=On, 1=Off) ;10xxx000h.port0 SD slot TP13
 1 Controller 1 port 1 power (0=On, 1=Off) ;10006000h.port1 eMMC
 2 Controller 2 port 0 power (0=On, 1=Off) ;10122000h.port0 Wifi SDIO
 3 ...Unknown... (initially 1 on Old3DS, initially 0 on New3DS)
 4-5 Unused (0)
 6 ...Unknown... Wifi port related? Pull up? Set at cold boot
 7 ...Unknown... New3DS: initially 1 (R/W), Old3DS: Unused (0)
 8 Controller 3 mapping (0=ARM9 10007000h, 1=ARM11 10100000h)
 9 SD card controller select (0=10007000h/10100000h, 1=10006000h)
 10-15 Unused (0) |
| --- |

**10000100h - CFG9_UNKNOWN**

```

| 0-1 Unknown (R/W)
 2 Unused (0)
 3 Unknown (R/W)
 4-7 Unused (0)
 8-13 Unknown (R/W)
 14-15 Unused (0) |
| --- |

**10000200h - New3DS - CFG9_EXTMEMCNT9**

```

| 0 New3DS: Extended ARM9 memory at 08100000h..0817FFFFh (0=Off, 1=On)
 1-31 Unused (0) |
| --- |

**10000FFCh - CFG9_MPCORECFG (R)**

Same as CFG11_SOCINFO (Port 10140FFCh).

**10010000h - CFG9_BOOTENV**

```

| 0-31 Value (0..FFFFFFFFh) (R/W) |
| --- |

This register is used to determine what the previous running FIRM was. Its
value is kept following an MCU reboot.

```

| 00000000h Coldboot
 00000001h Warmboot from 3DS mode
 00000003h Warmboot from DSi mode (need checking TLNC)
 00000007h Warmboot from GBA mode (need forwarding .sav) |
| --- |

NATIVE_FIRM will only launch titles if this is not value 0, and will only save
the AGB_FIRM savegame to SD if this is value 7.

**10010010h - CFG9_UNITINFO (R)**

This 8bit register is value zero for retail, non-zero for dev/debug units.

**10010014h - CFG9_TWLUNITINFO (R/W)**

```

| 0-1 Value (to be copied from CFG9_UNITINFO)
 2-7 Unused (0) |
| --- |

In the console-unique TWL key-init/etc function the ARM9 copies the u8 value
from REG_UNITINFO to this register.