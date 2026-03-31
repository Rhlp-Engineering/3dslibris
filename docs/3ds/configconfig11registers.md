# Configconfig11Registers

> Source: https://problemkaputt.de/gbatek.htm
> Section: Configconfig11Registers

3DS Config - CONFIG11 Registers 
| |
| --- |

 **CONFIG11 Registers (also referred to as "PDN" Registers)**
 
```

| Address Width Old3DS Name Used by
 10140000h 1*8 Yes CFG11_SHAREDWRAM_32K_CODE<0-7> Boot11,Process9,DSP
 10140008h 1*8 Yes CFG11_SHAREDWRAM_32K_DATA<0-7> Boot11,Process9,DSP
 10140100h 4 Yes CFG11_NULLPAGE_CNT
 10140104h 1 Yes CFG11_FIQ_CNT Kernel11
 10140105h 1 Yes ? Kernel11
 10140108h 2 Yes ? Related to HID_PAD_IRQ (??) TwlBg
 1014010Ch 2 Yes CFG11_CDMA_CNT TwlBg, NWM
 10140140h 4 Yes CFG11_GPUPROT Kernel11
 10140180h 1 Yes CFG11_WIFICNT TwlBg, NWM Services
 101401C0h 4 Yes CFG11_SPI_CNT SPI Services, TwlBg
 10140200h 4 Yes ?
 10140400h 1 No CFG11_GPU_NEW3DS_CNT NewKernel11
 10140410h 4 No CFG11_CDMA_PERIPHERALS NewKernel11
 10140420h 1 No CFG11_BOOTROM_OVERLAY_CNT NewKernel11
 10140424h 4 No CFG11_BOOTROM_OVERLAY_VAL NewKernel11
 10140428h 4 No ?
 10140FFCh 2 Yes CFG11_SOCINFO Boot11, Kernel11
 ---------
 10141000h 2 Yes CFG11_PDN_CNT ;??? Kernel11, TwlBg
 10141008h 4 Yes CFG11_PDN_WAKE_ENABLE PTM, PDN
 1014100Ch 4 Yes CFG11_PDN_WAKE_REASON PTM, TwlBg, PDN
 10141100h 2 Yes CFG11_TWLMODE_BOOT TwlProcess9, TwlBg
 10141104h 2 Yes CFG11_TWLMODE_SLEEP TwlBg
 10141108h 2 Yes CFG11_TWLMODE_2 ;IRQ? ;? TwlBg
 1014110Ah 2 Yes CFG11_TWLMODE_HID_IRQ TwlBg
 10141110h 2 Yes CFG11_TWLMODE_HID_MODE TwlBg
 10141112h 2 Yes CFG11_TWLMODE_HID_SET TwlBg
 10141114h 2 Yes CFG11_TWLMODE_GPIO_MODE TwlBg, Codec
 10141116h 2 Yes CFG11_TWLMODE_GPIO_SET TwlBg, Codec
 10141118h 1 Yes CFG11_TWLMODE_CARD_MODE TwlBg
 10141119h 1 Yes CFG11_TWLMODE_CARD_SET TwlBg
 10141120h 1 Yes ? TwlBg
 10141200h 4 Yes CFG11_GPU_CNT Boot11, Kernel11, PDN, TwlBg
 10141204h 4 - CFG11_VRAM_CNT Boot11, Kernel11, TwlBg
 10141208h 1 - CFG11_LCD_CNT Boot11
 10141210h 2 Yes CFG11_FCRAM_CNT Kernel11, TwlBg
 10141220h 1 Yes CFG11_SPEAKER_CNT Boot11, TwlBg, PDN
 10141224h 1 Yes CFG11_CAMERA_CNT PDN Services
 10141230h 1 Yes CFG11_DSP_CNT Process9, PDN
 10141240h ? No CFG11_MVD_CNT
 10141300h 4 No CFG11_MPCORE_CLKCNT NewKernel11
 10141304h 2 No CFG11_MPCORE_CNT NewKernel11
 10141310h 1 No CFG11_MPCORE_BOOTCNT_CPU0 NewKernel11
 10141311h 1 No CFG11_MPCORE_BOOTCNT_CPU1 NewKernel11
 10141312h 1 No CFG11_MPCORE_BOOTCNT_CPU2 NewKernel11
 10141313h 1 No CFG11_MPCORE_BOOTCNT_CPU3 NewKernel11 |
| --- |

**10140000h+0..7 - CFG11_SHAREDWRAM_32K_CODE --- aka MBK**

**10140008h+0..7 - CFG11_SHAREDWRAM_32K_DATA --- aka MBK**

Used for mapping 32K chunks of shared WRAM for Teak DSP code/data.

```

| 0 Master (0=ARM only, 1=ARM and DSP)
 1 Unused (0)
 2-4 Offset (0..7) (slot 0..7) (LSB of address in 32Kbyte units)
 5-6 Unused (0)
 7 Enable (0=Disable, 1=Enable) |
| --- |

ARM11 and ARM9 can access the memory regardless of bit0 (but trigger data abort
if bit7 is cleared, or on missing slot values in bit2-4).

```

| DSP Code is mapped at 1FF00000h..1FF3FFFFh in ARM memory (256Kbytes)
 DSP Data is mapped at 1FF40000h..1FF7FFFFh in ARM memory (256Kbytes) |
| --- |

**10140100h - CFG11_NULLPAGE_CNT (R/W)**

```

| 0 Address 00000000h..00000FFFh Trap (0=Off/Normal, 1=Data Abort) (R/W)
 1-15 Unused (0)
 16 Address 00000000h..00000FFFh Flag (1=Accessed, write 0 to clear)(R/ack)
 17-31 Unused (0) |
| --- |

Allows to trap ARM11 memory access to physical address 00000000h..00000FFFh.
Apparently intended for debugging uninitialzed pointers. The exception vectors
(except reset) should still work because they don't require data in that area.
The bit doesn't behave as anti-dumping feature (the memory mirrors at 00010000h
and FFFF0000h are still accessible, and bit0 can be cleared at any time).

**10140104h - CFG11_FIQ_CNT**

```

| 0-1 FIQ related? (R/W)
 2-3 New3DS only: unknown...? ;initially set, (R/W) in New3DS mode!
 4-7 Unused (0) |
| --- |

"Writing bit1 to this register disables FIQ interrupts.

This bit is set upon receipt of a FIQ interrupt and when svcUnbindInterrupt is
called on the FIQ-abstraction software interrupt for the current core. It is
cleared when binding that software interrupt to an event and just before that
event is signaled."

Maybe bit0-3 are for CPU0-3 (with above mentioned bit1 being for CPU1)? If the
bits are really FIQ related, then they are probably used on debug hardware
only.

**10140105h - ???**

```

| 0-1 Unknown...? (R/W)
 2-3 New3DS only: unknown...? ;initially set, (R/W) in New3DS mode!
 4-7 Unused (0) |
| --- |

Unknown. Maybe bit0-3 are for CPU0-3?

**10140108h - Related to HID_PAD_IRQ (??) TwlBg**

```

| 0 Unknown...? (R/W)
 1-15 Unused (0) |
| --- |

**1014010Ch - CFG11_CDMA_CNT (R/W)**

```

| 0 Enable Microphone DMA (0=Off, 1=Enable CDMA 00h) (R/W)
 1 Enable NTRCARD DMA on ARM11 side (0=Off, 1=Enable CDMA 01h) (R/W)
 2-3 Unused (0)
 4 Wifi-related? Set to 1 very early in NWM-module CDMA 04h ? (R/W)
 5 Unknown...? maybe debug sdio ? CDMA 05h ? (R/W)
 6-15 Unused (0) |
| --- |

**10140140h - CFG11_GPUPROT**

```

| 0-3 Old FCRAM DMA cutoff size, 0 = no protection
 4-7 New FCRAM DMA cutoff size, 0 = no protection ;<--New3DS mode only
 8 AXI WRAM protection, 0 = accessible
 9-10 QTM DMA cutoff size ;<--New3DS mode only
 11-31 Unused (0) |
| --- |

The New3DS bits are writeable only after enabling New3DS mode.

When this register is set to value 0, the GPU can access the entire FCRAM, AXI
WRAM, and on New3DS all QTM-mem. On cold boot this reg is set to 0.

Initialized during kernel boot, and used with SVC 59h which was implemented
with v11.3.

```

| The first 800000h-bytes of Old FCRAM cannot be protected.
 The first 800000h-bytes of New FCRAM are protected if Old FCRAM cutoff<>0
 The first 100000h-bytes of QTM cannot be protected.
 Old FCRAM DMA cutoff protects from 28000000h-(800000h*x) until end of FCRAM
 New FCRAM DMA cutoff protects from 30000000h-(800000h*x) until end of FCRAM
 QTM DMA cutoff protects from 1F400000h-(100000h*x) until end of QTM |
| --- |

**10140180h - CFG11_WIFICNT**

```

| 0 Enable wifi subsystem
 1-7 Unused (0) |
| --- |

**101401C0h - CFG11_SPI_CNT**

```

| 0 SPI_BUS0 Mode (registers at 10160000h) (0=Manual, 1=Fifo/Autopoll)
 1 SPI_BUS1 Mode (registers at 10142000h) (0=Manual, 1=Fifo/Autopoll)
 2 SPI_BUS2 Mode (registers at 10143000h) (0=Manual, 1=Fifo/Autopoll)
 3-31 Unused (0) |
| --- |

The registers for the deselected mode are disconnected from the bus, but they
are still working internally (that is, transfer attempts will finish after the
expected amount of time, but SPI reads return nothing useful: FFh for Manual
reads, and 00h for FIFO/Autopoll reads).

**10140200h - ???**

```

| 0-31 Unknown (R/W) |
| --- |

Unknown.

**10140400h - New3DS - CFG11_GPU_NEW3DS_CNT (R/W, even in Old3DS mode)**

```

| 0 Enable NEW3DS mode? (enables GPU? access to extra FCRAM banks, etc.)
 1 Texture related? (observing texture glitches when disabling this bit)
 2-31 Zero |
| --- |

**10140410h - New3DS - CFG11_CDMA_PERIPHERALS (R/W)**

```

| 0-17 CDMA Peripheral 00h-11h data request target (0=Old CDMA, 1=New CDMA)
 18-31 Zero |
| --- |

**10140420h - New3DS - CFG11_BOOTROM_OVERLAY_CNT (R/W)**

```

| 0 ARM11 Bootrom Overlay Enable (0=Disable, 1=Enable)
 1-31 Unused (0) |
| --- |

When enabled, opcode/data reads from physical(?) address 00010000h-00010FFFh
and FFFF0000h-FFFF0FFFh will return following values:

```

| ARM11 Opcode reads --> E59FF018h (opcode LDR PC,[$+20h]
 ARM11 Data reads --> [10140424h] (CFG11_BOOTROM_OVERLAY_VAL) |
| --- |

This is intended for redirecting the CPU2/3 reset vectors, however, it does
also affect all other exception vectors, including for CPU0/1 (so one should
usually disable IRQs while having the overlay enabled).

See CFG11_MPCORE_BOOTCNT_CPU2/3 for details on starting CPU2/3.

**10140424h - New3DS - CFG11_BOOTROM_OVERLAY_VAL (R/W)**

```

| 0-31 ARM11 Bootrom Overlay Data (aka CPU2/3 entrypoint) |
| --- |

Note: For redirecting to different entrypoints for CPU2/3, one can use opcode
EE10nFB0h (mov Rn,p15,0,c0,c0,5) to obtain the current CPU number.

**10140428h - New3DS - ??? (R/W)**

```

| 0 Unknown (R/W, only in New3DS mode)
 1-31 Unused (0) |
| --- |

Unknown.

**10140FFCh - CFG11_SOCINFO (R)**

Same as CFG9_MPCORECFG (Port 10000FFCh).

```

| 0 Always 1 (bootrom would do extra GPIO if it were cleared) (R)
 1 Console Type (0=Old3DS, 1=New3DS) (R)
 2 Max Clock on New3DS (0=536MHz, 1=804MHz) ;unused 0 on Old3DS (R)
 3-15 Unused (0) |
| --- |

Possible values are:

```

| 0=Old3DS/Prototype (with some GPIO bits used for LCD/backlight enable)
 1=Old3DS/Retail
 3=New3DS/Prototype (with slower clock and without Level 2 Cache)
 7=New3DS/Retail |
| --- |

Unknown if there are any New3DS consoles released with bit2=0.

Bit2 is also related to CFG11_MPCORE_CNT bit8?

**10141000h - CFG11_PDN_CNT (R or W?)**

```

| 0 Request sleep? (0=No, 1=Enter sleep mode?) (W?)
 1-14 Unused (0)
 15 Ready to sleep? (0=No, 1=GPU halted via CFG11_GPU_CNT.bit16=0) (R?) |
| --- |

**10141008h - CFG11_PDN_WAKE_ENABLE (0=Disable, 1=Enable) (R/W)**

**1014100Ch - CFG11_PDN_WAKE_REASON (0=No, 1=Yes/Ack) (R/ack)**

```

| 0 Unused (0) -
 1 HID_PAD_IRQ IRQ 5Bh Controller Buttons
 2 Unused (0) -
 3 GPIO_DATA0_DATA_IN.bit2=0 IRQ 60h Hinge is Open
 4 GPIO_DATA1_IRQ_ENABLE.bit0 IRQ 64h Headphone connect
 5 Unused (0) -
 6 GPIO_DATA1_IRQ_ENABLE.bit1 IRQ 66h ?
 7 Unknown...? IRQ ? ?
 8 Unknown...? IRQ ? ?
 9-15 Unused (0) -
 16 Unknown...? IRQ ? ?
 17 GPIO_DATA3_IRQ_ENABLE.bit0 IRQ 68h C-stick
 18 GPIO_DATA3_IRQ_ENABLE.bit1 IRQ 69h IrDA IRQ
 19 GPIO_DATA3_IRQ_ENABLE.bit2 IRQ 6Ah Gyro IRQ
 20 GPIO_DATA3_IRQ_ENABLE.bit3 IRQ 6Bh ?
 21 GPIO_DATA3_IRQ_ENABLE.bit4 IRQ 6Ch IrDA TX-RC (manual out)
 22 GPIO_DATA3_IRQ_ENABLE.bit5 IRQ 6Dh IrDA RXD (manual in)
 23 GPIO_DATA3_IRQ_ENABLE.bit6 IRQ 6Eh ?
 24 GPIO_DATA3_IRQ_ENABLE.bit7 IRQ 6Fh ?
 25 GPIO_DATA3_IRQ_ENABLE.bit8 IRQ 70h TSC[67h:2Bh] (Headphone connect)
 26 GPIO_DATA3_IRQ_ENABLE.bit9 IRQ 71h MCU[10h-1Fh]
 27 GPIO_DATA3_IRQ_ENABLE.bit10 IRQ 72h NFC?
 28 GPIO_DATA3_IRQ_ENABLE.bit11 IRQ 73h ?
 29 Unknown...? IRQ ? ? ... triggers always/often?
 30 GPIO_DATA0_DATA_IN.bit1=0 IRQ 63h Touchscreen Pen is down
 31 GPIO_DATA0_DATA_IN.bit2=1 IRQ 62h Hinge is Closed |
| --- |

Note: TSC triggers on headphone connect, ack by reading TSC[67h:2Bh].

The REASON bits are getting set only if enabled in CFG11_PDN_WAKE_ENABLE (and
only if the corresponding bits in GPIO_DATA1_IRQ_ENABLE, GPIO_DATA3_IRQ_ENABLE,
and HID_PAD_IRQ are also enabled).

The REASON bits can be cleared by writing 1 (unless the request condition is
still enabled and true).

Caution: Nonzero REASON bits can cause attempts to switch to New3DS mode (via
CFG11_MPCORE_CLKCNT) to hang?

Caution: Above is for New3DS (Old3DS doesn't have NFC and C-stick, and the
other bits are probably all completely different and still unknown).

**10141100h - CFG11_TWLMODE_BOOT (R/W)**

```

| 0-1 Setting from ARM7_CNT (Port 10018000h) (0=3DS, 1=NDS/DSi, 2=GBA) (R)
 2-14 Unused (0) ?
 15 Enable GBA/NDS/DSi hardware (can be set ONLY if bit0-1=nonzero) (R/W) |
| --- |

**10141104h - CFG11_TWLMODE_SLEEP (R/W)**

```

| 0 ARM7 Wakeup (0=No change, 1=Wakeup; clear bit1-2) (W)
 1-2 ARM7 Sleep state (0=Normal/awake, 1-2=?, 3=Sleeping) (R)
 3-14 Unused? (0)
 15 Enable IRQ upon ARM7 Sleep (0=Disable, 1=Enable IRQ 59h) (R/W) |
| --- |

Allows to trigger an IRQ when ARM7 enters lower power sleep mode (via GBA SWI
03h or NDS7/DSi7 SWI 07h). If so, one should forward the ARM7 wakeup condition
from CFG11_TWLMODE_HID_IRQ to ARM11 register HID_PAD_IRQ, enter sleep mode on
ARM11 side, and (after ARM11 wakeup) wakeup ARM7 via CFG11_TWLMODE_SLEEP.bit0.

**10141108h - CFG11_TWLMODE_2 ;IRQ? (R?)**

```

| 0-15 Unknown, reportedly "Bitfield", but looks like readonly, returns zero? |
| --- |

"LGY_IRQ_ENABLE: ARM11 interrupt enable bits for legacy interrupts, same bit
layout as the TWLMODE_GPIO regs below." Uh, what is that layout. And which
ARM11 interrupt(s) gets triggered? And why isn't this R/W?

MAYBE meant to be a readonly mirror of some bits in 4004C03h?

**1014110Ah - CFG11_TWLMODE_HID_IRQ (R)**

```

| 0-9 Button IRQ Source (0=Ignore, 1=Select) (Button A, B, Select, etc.)
 10-13 Not used
 14 Button IRQ Enable (0=Disable, 1=Enable)
 15 Button IRQ Condition (0=Logical OR, 1=Logical AND) |
| --- |

This is a readonly mirror of the ARM7 Key Interrupt Control register (KEYCNT,
GBA/NDS Port 4000132h). The ARM7 uses this as wakeup condition for sleep mode,
the ARM11 could forward that value to HID_PAD_IRQ when entering sleep mode,
too. In homebrew code, one can also mis-use the register to send data from ARM7
to ARM11.

Note: The NDS does also have a separate KEYCNT register on ARM9, but that isn't
forwarded to ARM11 side.

**10141110h - CFG11_TWLMODE_HID_MODE - R/W:0FFFh (0=Auto, 1=Manual)**

```

| 0-11 GBA/NDS Button Mode (0=Auto/HID_PAD, 1=Manual/CFG11_TWLMODE_HID_SET)
 12 Unknown, if any (not R/W, but reportedly used???) (dev hw maybe?)
 13-15 Unused (0) |
| --- |

Set bits will use the corresponding values from CFG11_TWLMODE_HID_SET instead
of allowing the hardware to read it from HID_PAD.

This is set to 0x1FFF (all buttons and the debug key) and CFG11_TWLMODE_HID_SET
is set to 0 when the "Close this software and return to HOME Menu?" dialog is
shown to prevent the button presses from propagating to the DS/GBA CPU.

**10141112h - CFG11_TWLMODE_HID_SET - R/W:0FFFh (Manual state, 0 or 1)**

```

| 0-11 Buttons ... unknown, reportedly 0 when NOT pressed ??????????
 12 Unknown, if any (not R/W, but reportedly used???) (dev hw maybe?)
 13-15 Unused (0) |
| --- |

Works the same way as HID_PAD, but the values set here are only replaced in the
HID_PAD seen by the NDS/GBA CPUs when the corresponding bits in
CFG11_TWLMODE_HID_MODE are set.

**10141114h - CFG11_TWLMODE_GPIO_MODE - R/W:0287h (0=Auto, 1=Manual)**

**10141116h - CFG11_TWLMODE_GPIO_SET - R/W:0387h (Manual state, 0 or 1)**

```

| 0 DEBUG Debug Button for NDS mode (inverted ???)
 1 /PENIRQ Touchscreen Pen Down for NDS mode (0=Pressed, 1=Released)
 2 HINGE Hinge for NDS mode (inverted ???)
 3-6 Unused (0)
 7 GPIO33[1] Headphone connect (HP#SP) (0=None, 1=Connected)
 8 GPIO33[2] Powerbutton interrupt (0=Short Keydown Pulse, 1=Normal)
 9 GPIO33[3] Sound Enable Output (ie. not a useful input)
 10-15 Unused (0) |
| --- |

Allows to remote control some GPIO bits in DSi Port 4004C00h and NDS Port
4000136h. Other bits in GPIO18[0,1,2] and GPIO33[0] seem to be always set and
cannot be changed from 3DS side.

**10141118h - CFG11_TWLMODE_CARD_MODE - R/W:01h (0=Auto, 1=Manual)**

**10141119h - CFG11_TWLMODE_CARD_SET - R/W:01h (Manual state, 0 or 1)**

```

| 0 Slot Game Cartridge (0=Inserted, 1=Ejected)
 1-7 Unused (0) |
| --- |

Allows to remote control Port 4004010h SCFG_MC.bit0.

**10141120h - 1 - R:0 ?**

Unknown. Always zero? Is that register really used by TwlBg?

**10141200h - CFG11_GPU_CNT (R/W)**

```

| 0 Enable GPU registers at 10400000h and up (0=DANGER) (vram=snow?)
 "When this is unset VRAM is not accessible and triggers exceptions."
 1 Enable GPU_MEMFILL 0/1 (0=Disable, 1=Enable) (R/W)
 2 Enable reading GPU Internal registers? (0=Hangs, 1=Enable) (R/W)
 3 Enable reading GPU Internal registers? (0=Hangs, 1=Enable) (R/W)
 4 Enable GPU_MEMCOPY (0=Disable, 1=Enable) (R/W)
 5 unknown, DANGER, hangs when cleared? (0=DANGER, 1=Enable)
 6 Enable LCD Pixel Output (0=Off/fade, 1=Enable) (R/W)
 7-15 Unused (0)
 16 Enable LCD Backlight and GPU/VRAM clock? (0=Off/dark, 1=Enable) (R/W)
 17-31 Unused (0) |
| --- |

**10141204h - CFG11_VRAM_CNT (W)**

**10141208h - CFG11_LCD_CNT (W)**

```

| 0 Clock enable? (1=Enable) (W)
 1-31 Unused (0) |
| --- |

Bootrom seems write these registers for VRAM and LCD clock enable, although the
registers don't seem to be actually implemented in hardware (or they are write
only, always returning 0 when reading).

**10141210h - CFG11_FCRAM_CNT (R/W)**

Used to apply changes to Port 102010xxh FCRAM Registers (which is needed only
when switching to GBA mode; or when using nonstandard FCRAM timings).

```

| 0 Apply FCRAM Config (0=Prepare, 1=Apply; on 0-to-1 transition) (R/W)
 1 Allow 3DS FCRAM access (0=Disable, 1=Enable) (R/W)
 2 Apply Busy flag (0=Ready, 1=Busy; or bit0-1 aren't set to 01h) (R)
 3-31 Unused (0) |
| --- |

After toggling bit0 from 0-to-1, one must wait until the apply has finished.
Weirdly, the Busy flag cannot be read when bit1=1, however, below are some
working apply sequences:

```

| Write 0h, Write 1h, Wait until Bit2=0, Write 3h ;best, for GBA and 3DS mode
 Write 0h, Write 1h, Wait until Bit2=0 ;good, for GBA mode only
 Write 0h, Write 3h, do ugly hardcoded delay ;ugly, for GBA and 3DS mode |
| --- |

Note: ARM11 hangs when accessing FCRAM if bit0 or bit1 are zero, or if apply is
busy (or if GBA mode is active).

**10141220h - CFG11_SPEAKER_CNT (R/W)**

```

| 0 Unknown (R/W)
 1 Audio Clock TP152 (Speaker PWM?) (0=Off/Muted Sound, 1=8MHz/16MHz On)
 2-31 Unused (0) |
| --- |

Should be set to 02h. Bit1=1 is required for Audio and Touchscreen. The Speaker
Clock seems to drive speaker PWM (sound gets muted when bit1=0, although audio
I2S clock/data are kept running).

**10141224h - CFG11_CAMERA_CNT**

```

| 0 Camera 16MHz Enable TP229 (for camera I2C bus) (0=Disable, 1=Enable)
 (1) Unused (0) ;<--- reportedly "turn on/off cameras" uh???
 1-31 Unused (0) |
| --- |

**10141230h - CFG11_DSP_CNT (R/W)**

```

| 0-1 Enable DSP registers at 10203000h (0,1,2=Disable, 3=Enable) (R/W)
 2-7 Unused (0) |
| --- |

Unknown what bit0-1 do exactly, maybe one is reset, and one is stop (they must
be BOTH set though, otherwise registers at 10203000h are always zero).

**10141240h - New3DS - CFG11_MVD_CNT (R/W) (always 0 in Old3DS mode)**

```

| 0 Reset MVD (0=Reset, 1=Normal)
 1-31 Unused (0) |
| --- |

**10141300h - New3DS - CFG11_MPCORE_CLKCNT**

```

| 0-2 Desired Mode (0,1,2,3,4,5,6,7) (see below) (R/W)
 3-14 Unused (0)
 15 Mode Change IRQ Flag (0=None, 1=IRQ 58h) (write 1 to reset) (R/ack)
 16-18 Current Mode (0,1,3,5,7) (never 2,4,6) (see below) (R)
 19-31 Unused (0) |
| --- |

Mode values:

```

| Mode 0: Old3DS Mode, 268MHz (1x), 128MB FCRAM, no L2C cache
 Mode 1: New3DS Mode, 268MHz (1x), 256MB FCRAM, and L2C cache controller
 Mode 3: New3DS Mode, 536MHz (2x), 256MB FCRAM, and L2C cache controller
 Mode 5: New3DS Mode, 804MHz (3x), 256MB FCRAM, and L2C cache controller
 Mode 2/4/6 are same as Mode 0. Mode 7 is same as Mode 5 |
| --- |

Mode changes are applied ONLY once when all ARM11 cores are in Wait for IRQ
state (eg. "WFI" opcode). At that point, the IRQ flag in bit15 is set, and IRQ
58h is triggered.

Mode changes (and IRQs) occur ONLY when changing to different modes:

```

| - Changing between mode 0/1/3/5/7 (because they are different)
 - Changing between mode 5/7 (although they appear to be same) |
| --- |

Mode changes (and IRQs) do NOT occur when:

```

| - Rewriting the current mode value (because mode is already same)
 - Changing between mode 0/2/4/6 (because they are all same as mode 0) |
| --- |

New3DS mode enables some registers (eg. 10141240h, which are always zero in
Old3DS mode), and unlocks some Memory and I/O regions (which trigger data abort
in Old3DS mode):

```

| 10130000h (L2B_0, L2B_1, Y2R_1, and MVD registers)
 10206000h (newer CDMA controller)
 17E10000h (L2C cache controller)
 28000000h (Extra FCRAM) |
| --- |

Other New3DS memory isn't automatically enabled (and keeps triggering data
abort even in New3DS mode, until explicitely enabling it):

```

| 08100000h (Extended ARM9 RAM, needs enable via CFG9_EXTMEMCNT9.bit0)
 1F000000h (New VRAM aka QTM, needs enable via CFG11_MPCORE_CNT.bit0) |
| --- |

Clock changes affect the ARM11 CPU clock, including the MPCore Timer/Watchdog
registers at 17E00600h (if desired, one can change their prescalers to maintain
them running at same speed despite of the higher CPU clock).

The 804MHz mode should be used only if CFG11_SOCINFO.bit2 indicates that it is
supported.

The IRQ flag in bit15 should be acknowledge before (or alongsides with) writing
the desired clock value (needed if it was still set from an older
un-acknowledged clock change).

IRQ 58h should be enabled to recover from WFI (or maybe anything like Hblank
IRQ might work for that, too). CPSR.I flag does not need to be enabled, WFI
does only wait for IRQs (but can recover without executing them). Using the old
CP15 Wait-for-IRQ opcode instead of WFI does work, too.

**10141304h - New3DS - CFG11_MPCORE_CNT**

```

| 0 Enable New VRAM aka QTM at 1F000000h-1F3FFFFFh (0=Off, 1=On) (R/W)
 1-7 Unused (0)
 8 Unknown... should be set only if CFG11_SOCINFO.bit2=1 (R/W)
 9-xx Unused (0) |
| --- |

Kernel11 sets this to 101h when bit2 in CFG11_SOCINFO is set otherwise 001h.

```

| Bit8: "Enables the L2C block (LGR2 only)" == Level 2 Cache Controller? |
| --- |

**10141310h - New3DS - CFG11_MPCORE_BOOTCNT_CPU0 (R)**

**10141311h - New3DS - CFG11_MPCORE_BOOTCNT_CPU1 (R)**

```

| 0-7 Fixed (always 30h, even when executing WFI opcode) (R) |
| --- |

These are readonly dummy registers, somewhat indicating that CPU0/1 do exist
and that they were already started (of which, CPU0 is actually running code,
and CPU1 is initially hanging in a bootrom waitloop; which waits for FIRM code
to send software interrupt 01h via Port 17E01F00h, and then jumps to
[1FFFFFDCh]).

**10141312h - New3DS - CFG11_MPCORE_BOOTCNT_CPU2 (R/W)**

**10141313h - New3DS - CFG11_MPCORE_BOOTCNT_CPU3 (R/W)**

```

| 0 Start/Stop CPU core (0=Stop upon WFI, 1=Start/Enable) (R/W)
 1 Must be 1 when starting CPU (0=Hangs when setting bit0, 1=Works) (R/W)
 2-3 Unused (0)
 4 CPU started (0=No, 1=Yes, bit0 is/was set) (R)
 5 CPU not in WFI state (0=Executes WFI opcode, 1=Normal) (R)
 6-7 Unused (0) |
| --- |

Starting CPU2/3 must be done with CFG11_BOOTROM_OVERLAY_CNT/VAL (the default
reset vector in ARM11 bootrom doesn't support booting CPU2/3).

Bit0 enables CPU2/3 (and alongsides enables corresponding registers at
17E00900h/17E00A00h).

Bit1 must be set alongsides with bit0 (otherwise the CPU hangs somehow), once
when the CPU is started, toggling bit1 won't affect the CPU operation; clearing
bit1 is commonly done to notify CPU0 that the CPU has started.

Note: When starting/stopping CPU2/3, one should also notify the hardware about
the new state (via Port 17E00008h, SCU CPU Status Register).