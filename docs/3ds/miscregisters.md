# Miscregisters

> Source: https://problemkaputt.de/gbatek.htm
> Section: Miscregisters

3DS MISC Registers 
| |
| --- |

 
```

| _____________________________ UNKNOWN Registers _____________________________ |
| --- |

**Unknown Registers at 10204000h (ARM11 only) (New3DS only)**

```

| 10204000h 4 Unknown R/W=000000F1h (res=00000000h)
 10204004h 4 Unknown, readonly (00000001h) (R)
 10204008h 08h Unused (0) ?
 10204010h 4 Unknown, readonly (00000202h) (R)
 10204014h 4 Unknown, readonly (12000802h/12000002h) (R) (or data abort!)
 10204018h 08h Unused (0) ?
 10204020h 4 Unknown, readonly (00000020h) (R) ;or res=00000000h?
 10204024h FDCh Unused (0) ? |
| --- |

```

| ______________________________ FCRAM Registers ______________________________ |
| --- |

**FCRAM Control Registers (ARM11 only)**

```

| 10201000h 4 FCRAM_MODE (R/W)
 10201004h 0Ch Unused (0)
 10201010h 4 FCRAM_DELAY (R/W)
 10201014h 0Ch Unused (0)
 10201020h 4 FCRAM_OR_WHATEVER (R/W)
 10201024h FDCh Unused (0) |
| --- |

**10201000h - FCRAM_MODE (R/W)**

```

| 0 FCRAM Mode (burst enable or so) (0=GBA, 1=3DS/Fast) (R/W)
 1-4 Unknown... (00h..0Fh, usually 0) (R/W)
 5-31 Unused (0) |
| --- |

Bit0 must be cleared in GBA mode (where FCRAM is used to emulate the GBA
cartridge ROM), when not clearing bit0, only about 1% of GBA ROM reads will
correctly read from "addr", and the other 99% will accidentally read from
"addr+20h" (wrapped within 800h-byte pages).

Changes to this register have no effect until applying them via
CFG11_FCRAM_CNT.

**10201010h - FCRAM_DELAY (R/W)**

```

| 0-1 Unknown... (0..3, usually 0) (R/W)
 2-15 Unused (0)
 16-18 FCRAM Delay (0..7 = Fast..Slow, usually 2) (R/W)
 19-31 Unused (0) |
| --- |

Changes to this register have no effect until applying them via
CFG11_FCRAM_CNT.

**10201020h - FCRAM_OR_WHATEVER (R/W)**

```

| 0-3 Unknown... (00h..0Fh, usually 8) (R/W)
 4-7 Unused (0)
 8-11 Unknown... (00h..0Fh, usually 8) (R/W)
 12-30 Unused (0)
 31 Unknown... (0..1, usually 0) (R/W) |
| --- |

Changes to this register have no effect until applying them via CFG11_FCRAM_CNT
(probably so... assuming that it works similar as the registers above, and that
changes do have some yet unknown effect at all).

**Internal FCRAM Configuration Registers**

The 3DS uses the following FCRAM chips:

```

| Old3DS: Fujitsu MB82M8080-07L (128Mbyte)
 New3DS: Fujitsu 82MK9A9A, 7L (256Mbyte) |
| --- |

The chip in Old3DS does reportedly internally contain 2 dies:

```

| MB81EDS516545 (4x2Mx64bit, aka 64Mbyte)
 MB82DBS08645 (unknown, supposedly contains the missing 64Mbyte) |
| --- |

The MB81EDS516545 datasheet says that there are some internal config registers,
which must be initialized after power up, the initialization requires changing
/RAS and /CAS and some other pins. Unknown if/how the 3DS hardware or software
can/may/must initialize those registers.

```

| ______________________________ SD/MMC Registers ______________________________ |
| --- |

**SD/MMC Registers**

```

| 10006000h A9 Standard controller for SD/MMC Slot and internal eMMC
 10006200h A9 mirrors of above (each 200h bytes)
 10007000h A9 Alternate SD/MMC Slot controller (see CFG9_SDMMC_CTL.8/9)
 10007200h A9 mirrors of above (each 200h bytes)
 10100000h A11/A9 Alternate SD/MMC Slot controller (see CFG9_SDMMC_CTL.8/9)
 10122000h A11/A9 SDIO Wifi controller
 10123000h A11/A9 Unknown, reportedly SDIO? (actually just data abort?)
 10300000h A11 DATA32 FIFO for registers at 10100000h (Alternate SD/MMC)
 10322000h A11 DATA32 FIFO for registers at 10122000h (SDIO Wifi)
 10323000h A11 DATA32 FIFO for registers at 10123000h (Unknown) |
| --- |

These registers are used to access the system NAND and the inserted SD card.
Both devices use the same interface.

HCLK of the SDMMC controller is 67.027964 MHz (double of the DSi HCLK).

Unknown if eMMC and Wifi are fast enough to support HCLK/2 on 3DS.

Standard SD cards may require HCLK/4 on 3DS (unless the card was detected to
support HCLK/2; don't know if it requires further card initialization to enable
fast mode?).

DSi SD/MMC Protocol and I/O Ports

DSi SD/MMC Filesystem

DSi Atheros Wifi SDIO Interface

DSi Atheros Wifi Internal Hardware

See also: CFG9_SDMMC_CTL

**DATA FIFOs**

The ARM9 controllers (10006000h/10007000h) have DATA32 FIFO at base+10Ch.

The ARM11 controllers (10100000h/10122000h/10123000h) have DATA32 FIFO at
base+200000h (the ARM11 controllers can be also accessed from ARM9 side, but
the DATA32 FIFOs (and DRQs/IRQs) are on ARM11 side only; so ARM9 could only use
DATA16 FIFOs and polling in place of DRQs/IRQs).

**IRQ_STAT**

```

| 3 SD card removal flag (Set to 1 when SD card is removed)
 4 SD card insertion flag (Set to 1 when SD card is inserted)
 5 SD card insertion status (0=Missing, 1=Inserted) SIGSTATE |
| --- |

SD card insertion status

Assertion happens around 250 ms after SD card insertion and/or enabling the
EMMC hardware (delay could possibly be due to an SD bus timeout?).

```

| _______________________________ IPC Registers ________________________________ |
| --- |

**PXI Registers**

```

| Address Width Old3DS Name Used by
 10008000h 4 Yes PXI_SYNC9 Boot9, Process9 ;-SYNC
 10008004h 2 Yes PXI_CNT9 Boot9, Process9 ;\
 10008008h 4 Yes PXI_SEND9 ; FIFO
 1000800Ch 4 Yes PXI_RECV9 ;/
 10163000h 4 Yes PXI_SYNC11 Boot11 ;-SYNC
 10163004h 2 Yes PXI_CNT11 Boot11 ;\
 10163008h 4 Yes PXI_SEND11 ; FIFO
 1016300Ch 4 Yes PXI_RECV11 ;/ |
| --- |

The PXI registers are similar to those on DS. Uh, aka what is known as IPC.

But, except, SYNC is 8bit (instead 4bit)!

The FIFO registers seem to be exactly same as on NDS.

**10008000h - ARM9 - PXI_SYNC9**

**10163000h - ARM11 - PXI_SYNC11**

```

| 0-7 R Data received from remote SYNC bit8-15
 8-15 W Data sent to remote SYNC bit0-7 (CAUTION: write-only, unlike NDS!)
 16-22 - Unused (0)
 23 - Unused (0) ;<-- reportedly "?" whatever that means, if anything?
 24-28 - Unused (0)
 29 - PXI_SYNC11: Unused (0)
 30 W PXI_SYNC11: Send IRQ to ARM9 IF.bit12 (0=No change, 1=Yes)
 29 W PXI_SYNC9: Send IRQ to ARM11 IRQ 50h (0=No change, 1=Yes)
 30 W PXI_SYNC9: Send IRQ to ARM11 IRQ 51h (0=No change, 1=Yes)
 31 R/W Enable IRQ from remote CPU (0=Disable, 1=Enable) |
| --- |

Caution: The send-value (in bit8-15) is write-only on 3DS (reads as zero), this
is different as on NDS/DSi. As workaround: Use 8bit LDRB/STRB instead of 32bit
LDR/STR when changing the IRQ bits.

**10008004h - ARM9 - PXI_CNT**

**10163004h - ARM11 - PXI_CNT**

```

| 0 R Send Fifo Empty Status (0=Not Empty, 1=Empty)
 1 R Send Fifo Full Status (0=Not Full, 1=Full)
 2 R/W Send Fifo Empty IRQ (0=Disable, 1=Enable)
 3 W Send Fifo Clear (0=Nothing, 1=Flush Send Fifo)
 4-7 unknown/unspecified
 8 R Receive Fifo Empty (0=Not Empty, 1=Empty)
 9 R Receive Fifo Full (0=Not Full, 1=Full)
 10 R/W Receive Fifo Not Empty IRQ (0=Disable, 1=Enable)
 11-13 unknown/unspecified
 14 R/W Error, Read Empty/Send Full (0=No Error, 1=Error/Acknowledge)
 15 R/W Enable Send/Receive Fifo (0=Disable, 1=Enable) |
| --- |

**10008008h - ARM9 - PXI_SEND**

**10163008h - ARM11 - PXI_SEND**

**1000800Ch - ARM9 - PXI_RECV**

**1016300Ch - ARM11 - PXI_RECV**

FIFO

DS Inter Process Communication (IPC)

**ARM11-to-ARM11 Messages**

ARM11 cores can use the Software Interrupt Register, Port 17E01F00h (and data
in AXI memory or other RAM locations) to communicate with each other.

```

| ________________________________ HID Registers _______________________________ |
| --- |

**HID Registers (aka Keypad)**

```

| 10146000h 2 HID_PAD
 10146002h 2 HID_PAD_IRQ |
| --- |

**10146000h - HID_PAD (R)**

Same as NDS/DSi, but with X/Y buttons included right in this register.

```

| 0 Button A (0=Pressed, 1=Released)
 1 Button B
 2 Select
 3 Start
 4 DPAD Right
 5 DPAD Left
 6 DPAD Up
 7 DPAD Down
 8 Button R
 9 Button L
 10 Button X
 11 Button Y
 12-15 Unused (0) |
| --- |

**10146002h - HID_PAD_IRQ (R/W)**

```

| 0-11 Button IRQ Source (0=Ignore, 1=Select) (Button A, B, Select, etc.)
 12-13 Unused (0)
 14 Button IRQ Enable (0=Disable, 1=Enable, Interrupt 5Bh)
 15 Button IRQ Condition (0=Logical OR, 1=Logical AND) |
| --- |

Similar as Port 4000132h on GBA/NDS, used as wake-up source in sleep mode.

**Other Inputs**

```

| Circle Pad: Connected to Touchscreen/Sound controller
 Power/Home buttons: see I2C MCU
 Wifi button: see I2C MCU (older 3DS only)
 Volume/3D Sliders: see I2C MCU
 Accelerometer: see I2C MCU (also includes pedometer step counter)
 Gyroscope: see I2C Gyroscope
 Hinge/Shell: see GPIO (and/or reportedly also I2C MCU)
 DebugPad: See I2C (whatever that is)
 MIC, Cameras, IR, QTM, Charge, SD slot, ROM slot
 Old3DS: NFC reader/writer (external adaptor, instead of New3DS built-in)
 Old3DS: Circle Pad Pro (2nd Circle Pad and R/ZL/ZR buttons, on IR port)
 New3DS: C-stick and ZL/ZR: see I2C (instead of Old3DS's circle pad pro)
 New3DS: NFC built-in (instead of Old3DS's external adaptor)
 New3DS: Head-tracking (whatever that is, probably just camera with IR-LED..?)
 New3DS: Invisible camera IR-LED (unknown how to control/test that) |
| --- |

```

| ________________________________ MP? Registers _______________________________ |
| --- |

**MP Registers (local wifi multiplayer?)**

```

| 10165206h 2 NDS-Wifi WIFIWAITCNT
 10170000h 8000h NDS-Wifi WS0 region
 10178000h 8000h NDS-Wifi WS1 region |
| --- |

These registers are used by MP-module. MP-module seems to be used for
3DS<>DSi local-WLAN communications?

```

| ______________________________ PARODY Registers ______________________________ |
| --- |

**Parody Register summary**

After reading through many official datasheets and homebrew wiki pages, some of
the more amazing 3DS hardware related information goes into 1020B000h...

```

| 1020B000h 4 Certain Register
 1020B014h 4 Assert Register
 1020B034h 4 This Register
 1020B096h 2 Figure Register
 1020B1BFh 1 Byte Register
 1020B2E4h ?? Broken Register
 1020B3D0h 4 Snakish Register |
| --- |

**1020B000h - Certain Register**

This is the Certain register. The register contains certain bits that are used
to enable certain functions, each bit must be set to a certain value. The
entire system will hang if used with a certain setting.

**1020B014h - Assert Register**

Bits in the Assert register get asserted when asserting events are asserted and
de-asserted. You are asserted to assert what the asserted information does
assert. Assert your dictionary if you can't assert that.

**1020B034h - This Register**

This register can be used to change the value of this register by writing to
this register.

**1020B096h - Figure Register**

Figure 128 contains information about the Figure register. These register
specifications are outlined in Figure 128. See Figure 128 for details about the
Figure Register in Figure 128.

Figure 128: Figure 128 is left blank intentionally.

1020B0DCh - ?

**1020B1BFh - Byte Register**

8-bit Byte value 0,1: 1-ish enable 2:31 must be <this&0x3<=2'b010 +!
Pdn SoC value>.

**1020B2E4h - Broken Register**

This works exactly as described in the broken link on the hardware page.

**1020B3D0h - Snakish Register**

With Native_firm as of 8.1.0.192 three additional unsigned flags have been
introduced to the old(6.5.0.0+) specification which did have specific bits at a
certain virtual address and that were not formerly asserted in 1.0.2.30 with
*ANY* firm these values must be accumulated with strict binary point
comparisions(in the same format as the other bits) prohibitvely permitting
abstractions of the reorganized architecture in the *actual* snakish sentences
used to describe the entire register space(that CAN be changed in future).