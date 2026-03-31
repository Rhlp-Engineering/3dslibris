# Dssystemandbuiltinperipherals

> Source: https://problemkaputt.de/gbatek.htm
> Section: Dssystemandbuiltinperipherals

DS System and Built-in Peripherals 
| |
| --- |

 DS DMA Transfers
 DS Timers
 DS Interrupts
 DS Maths
 DS Inter Process Communication (IPC)
 DS Keypad
 DS Absent Link Port
 DS Real-Time Clock (RTC)
 DS Serial Peripheral Interface Bus (SPI)
 DS Touch Screen Controller (TSC)
 DS Power Management Device
 DS Power Control
 DS Backwards-compatible GBA-Mode
 DS Debug Registers (Emulator/Devkits)
 
 
 
| DS DMA Transfers |
| --- |

 The DS includes four DMA channels for each CPU (ie. eight channels in total), which are working more or less the same as on GBA:
 GBA DMA Transfers
 All NDS9 and NDS7 DMA Registers are R/W. The gamepak bit (Bit 27) has been removed (on the NDS9 the bit is used to expand the mode setting to 3bits).
 
 **NDS9 DMA**
 Word count of all channels is expanded to 21bits (max 1..1FFFFFh units, or 0=200000h units), and SAD/DAD registers for all channels support ranges of 0..0FFFFFFEh. The transfer modes (DMACNT Bit27-29) are:
 
```

| 0 Start Immediately
 1 Start at V-Blank
 2 Start at H-Blank (paused during V-Blank)
 3 Synchronize to start of display
 4 Main memory display
 5 DS Cartridge Slot
 6 GBA Cartridge Slot
 7 Geometry Command FIFO |
| --- |

**NDS7 DMA**

Word Count, SAD, and DAD are R/W, aside from that they do have the same
restrictions as on GBA (max 4000h or 10000h units, some addresses limited to
0..07FFFFFEh). DMACNT Bit27 is unused on NDS7. The transfer modes (DMACNT
Bit28-29) are:

```

| 0 Start Immediately
 1 Start at V-Blank
 2 DS Cartridge Slot
 3 DMA0/DMA2: Wireless interrupt, DMA1/DMA3: GBA Cartridge Slot |
| --- |

**40000E0h - NDS9 only - DMA0FILL - DMA 0 Filldata (R/W)**

**40000E4h - NDS9 only - DMA1FILL - DMA 1 Filldata (R/W)**

**40000E8h - NDS9 only - DMA2FILL - DMA 2 Filldata (R/W)**

**40000ECh - NDS9 only - DMA3FILL - DMA 3 Filldata (R/W)**

```

| Bit0-31 Filldata |
| --- |

The DMA Filldata registers contain 16 bytes of general purpose WRAM, intended
to be used as fixed source addresses for DMA memfill operations.

This is useful because DMA cannot read from TCM, and reading from Main RAM
would require to recurse cache & write buffer.

The DMA Filldata is used with Src=Fixed and SAD=40000Exh (which isn't optimal
because it's doing repeated reads from SAD, and, for that reason, a memfill via
STMIA opcodes can be faster than DMA; the DSi's new NDMA channels are providing
a faster fill method with Src=Fill and SAD=Unused).

**NDS7 Sound DMA**

The NDS additionally includes 16 Sound DMA channels, plus 2 Sound Capture DMA
channels (see Sound chapter). The priority of these channels is unknown.

**NDS9 Cache, Writebuffer, DTCM, and ITCM**

Cache and tightly coupled memory are connected directly to the NDS9 CPU,
without using the system bus. So that, DMA cannot access DTCM/ITCM, and access
to cached memory regions must be handled with care: Drain the writebuffer
before DMA-reads, and invalidate the cache after DMA-writes. See,

ARM CP15 System Control Coprocessor

The CPU can be kept running during DMA, provided that it is accessing only TCM
(or cached memory), otherwise the CPU is halted until DMA finishes.

Respectively, interrupts executed during DMA will usually halt the CPU (unless
the IRQ handler uses only TCM and cache; the IRQ vector at FFFF00xxh must be
cached, or relocated to ITCM at 000000xxh, and the IRQ handler may not access
IE, IF, or other I/O ports).

**NDS Sequential Main Memory DMA**

Main RAM has different access time for sequential and non-sequential access.
Normally DMA uses sequential access (except for the first word), however, if
the source and destination addresses are both in Main RAM, then all accesses
become non-sequential. In that case it would be faster to use two DMA
transfers, one from Main RAM to a scratch buffer in WRAM, and one from WRAM to
Main RAM.

| DS Timers |
| --- |

Same as GBA, except F = 33.513982 MHz (for both NDS9 and NDS7).

GBA Timers

Both NDS9 and NDS7 have four Timers each, eight Timers in total.

The NDS sound controller is having its own frequency generators (unlike GBA,
which needed to use Timers to drive channel A/B sounds).

| DS Interrupts |
| --- |

**4000208h - NDS9/NDS7 - IME - Interrupt Master Enable (R/W)**

```

| 0 Disable all interrupts (0=Disable All, 1=See IE register)
 1-31 Not used |
| --- |

**4000210h - NDS9/NDS7 - IE - 32bit - Interrupt Enable (R/W)**

**4000214h - NDS9/NDS7 - IF - 32bit - Interrupt Request Flags (R/W)**

Bits in the IE register are 0=Disable, 1=Enable.

Reading IF returns 0=No request, 1=Interrupt Request.

Writing IF acts as 0=No change, 1=Acknowledge (clears that bit).

```

| 0 LCD V-Blank
 1 LCD H-Blank
 2 LCD V-Counter Match
 3 Timer 0 Overflow
 4 Timer 1 Overflow
 5 Timer 2 Overflow
 6 Timer 3 Overflow
 7 NDS7 only: SIO/RCNT/RTC (Real Time Clock)
 8 DMA 0
 9 DMA 1
 10 DMA 2
 11 DMA 3
 12 Keypad
 13 GBA-Slot (external IRQ source) / DSi: None such
 14 Not used / DSi9: NDS-Slot Card change?
 15 Not used / DSi: dito for 2nd NDS-Slot?
 16 IPC Sync
 17 IPC Send FIFO Empty
 18 IPC Recv FIFO Not Empty
 19 NDS-Slot Game Card Data Transfer Completion
 20 NDS-Slot Game Card IREQ_MC
 21 NDS9 only: Geometry Command FIFO
 22 NDS7 only: Screens unfolding
 23 NDS7 only: SPI bus
 24 NDS7 only: Wifi / DSi9: XpertTeak DSP
 25 Not used / DSi9: Camera
 26 Not used / DSi9: Undoc, IF.26 set on FFh-filling 40021Axh
 27 Not used / DSi: Maybe IREQ_MC for 2nd gamecard?
 28 Not used / DSi: NewDMA0
 29 Not used / DSi: NewDMA1
 30 Not used / DSi: NewDMA2
 31 Not used / DSi: NewDMA3
 ? DSi7: any further new IRQs on ARM7 side... in bit13-15,21,25-26? |
| --- |

Raw TCM-only IRQs can be processed even during DMA ?

Trying to set all IE bits gives FFFFFFFFh (DSi7) or FFFFFF7Fh (DSi9).

**4000218h - DSi7 - IE2 - DSi7 Extra Interrupt Enable Bits**

**400021Ch - DSi7 - IF2 - DSi7 Extra Interrupt Flags**

```

| 0 DSi7: GPIO18[0] ;\
 1 DSi7: GPIO18[1] ; maybe 1.8V signals?
 2 DSi7: GPIO18[2] ;/
 3 DSi7: Unused (0)
 4 DSi7: GPIO33[0] unknown (related to "GPIO330" testpoint on mainboard?)
 5 DSi7: GPIO33[1] Headphone connect (HP#SP) (static state)
 6 DSi7: GPIO33[2] Powerbutton interrupt (short pulse upon key-down)
 7 DSi7: GPIO33[3] Sound Enable Output (ie. not a useful irq-input)
 8 DSi7: SD/MMC Controller ;-Onboard eMMC and External SD Slot
 9 DSi7: SD Slot Data1 pin ;-For SDIO hardware in External SD Slot
 10 DSi7: SDIO Controller ;\Atheros Wifi Unit
 11 DSi7: SDIO Data1 pin ;/
 12 DSi7: AES interrupt
 13 DSi7: I2C interrupt
 14 DSi7: Microphone Extended interrupt
 15-31 DSi7: Unused (0) |
| --- |

Trying to set all IE2 bits gives 00007FF7h (DSi7) or 00000000h (DSi9).

**DTCM+3FFCh - NDS9 - IRQ Handler (hardcoded DTCM address)**

**380FFFCh - NDS7 - IRQ Handler (hardcoded RAM address)**

```

| Bit 0-31 Pointer to IRQ Handler |
| --- |

NDS7 Handler must use ARM code, NDS9 Handler can be ARM/THUMB (Bit0=Thumb).

**DTCM+3FF8h - NDS9 - IRQ Check Bits (hardcoded DTCM address)**

**380FFF8h - NDS7 - IRQ Check Bits (hardcoded RAM address)**

```

| Bit 0-31 IRQ Flags (same format as IE/IF registers) |
| --- |

When processing & acknowleding interrupts via IF register, the user
interrupt handler should also set the corresponding bits of the IRQ Check value
(required for BIOS IntrWait and VBlankIntrWait SWI functions).

**380FFC0h - DSi7 only - Extra IRQ Check Bits for IE2/IF2 (hardcoded RAM addr)**

Same as the above 380FFF8h value, but for new IE2/IF2 registers, intended for
use with IntrWait and VBlankIntrWait functions. However, that functions are
BUGGED on DSi and won't actually work in practice (they do support only the new
380FFC0h bits, but do accidently ignore the old 380FFF8h bits).

--- Below for other (non-IRQ) exceptions ---

**27FFD9Ch - RAM - NDS9 Debug Stacktop / Debug Vector (0=None)**

**380FFDCh - RAM - NDS7 Debug Stacktop / Debug Vector (0=None)**

These addresses contain a 32bit pointer to the Debug Handler, and, memory below
of the addresses is used as Debug Stack. The debug handler is called on
undefined instruction exceptions, on data/prefetch aborts (caused by the
protection unit), on FIQ (possibly caused by hardware debuggers). It is also
called by accidental software-jumps to the reset vector, and by unused SWI
numbers within range 0..1Fh.

| DS Maths |
| --- |

**4000280h - NDS9 - DIVCNT - Division Control (R/W)**

```

| 0-1 Division Mode (0-2=See below) (3=Reserved; same as Mode 1)
 2-13 Not used
 14 Division by zero (0=Okay, 1=Division by zero error; 64bit Denom=0)
 15 Busy (0=Ready, 1=Busy) (Execution time see below)
 16-31 Not used |
| --- |

Division Modes and Busy Execution Times

```

| Mode Numer / Denom = Result, Remainder ; Cycles
 0 32bit / 32bit = 32bit , 32bit ; 18 clks
 1 64bit / 32bit = 64bit , 32bit ; 34 clks
 2 64bit / 64bit = 64bit , 64bit ; 34 clks |
| --- |

Division is started when writing to any of the DIVCNT/NUMER/DENOM registers.

**4000290h - NDS9 - DIV_NUMER - 64bit Division Numerator (R/W)**

**4000298h - NDS9 - DIV_DENOM - 64bit Division Denominator (R/W)**

Signed 64bit values (or signed 32bit values in 32bit modes, the upper 32bits
are then unused, with one exception: the DIV0 flag in DIVCNT is set only if the
full 64bit DIV_DENOM value is zero, even in 32bit mode).

**40002A0h - NDS9 - DIV_RESULT - 64bit Division Quotient (=Numer/Denom) (R)**

**40002A8h - NDS9 - DIVREM_RESULT - 64bit Remainder (=Numer MOD Denom) (R)**

Signed 64bit values (in 32bit modes, the values are sign-expanded to 64bit).

**Division Overflows**

Overflows occur on "DIV0" and "-MAX/-1" (eg. -80000000h/-1 in 32bit mode):

```

| DIV0 --> REMAIN=NUMER, RESULT=+/-1 (with sign opposite of NUMER)
 -MAX/-1 --> RESULT=-MAX (instead +MAX) |
| --- |

On overflows in 32bit/32bit=32bit mode: the upper 32bit of the sign-expanded
32bit result are inverted. This feature produces a correct 64bit (+MAX) result
in case of the incorrect 32bit (-MAX) result. The feature also applies on DIV0
errors (which makes the sign-expanded 64bit result even more messed-up than the
normal 32bit result).

The DIV0 flag in DIVCNT.14 indicates DENOM=0 errors (it does not indicate
"-MAX/-1" errors). The DENOM=0 check relies on the full 64bit value (so, in
32bit mode, the flag works only if the unused upper 32bit of DENOM are zero).

**40002B0h - NDS9 - SQRTCNT - Square Root Control (R/W)**

```

| 0 Mode (0=32bit input, 1=64bit input)
 1-14 Not used
 15 Busy (0=Ready, 1=Busy) (Execution time is 13 clks, in either Mode)
 16-31 Not used |
| --- |

Calculation is started when writing to any of the SQRTCNT/PARAM registers.

**40002B4h - NDS9 - SQRT_RESULT - 32bit - Square Root Result (R)**

**40002B8h - NDS9 - SQRT_PARAM - 64bit - Square Root Parameter Input (R/W)**

Unsigned 64bit parameter, and unsigned 32bit result.

**IRQ Notes**

Push all DIV/SQRT values (parameters and control registers) when using DIV/SQRT
registers on interrupt level, and, after restoring them, be sure to wait until
the busy flag goes off, before leaving the IRQ handler.

**BIOS Notes**

The NDS9 and NDS7 BIOSes additionally contain software based division and
square root functions, which are NOT using above hardware registers (even the
NDS9 functions are raw software).

**Timing Notes**

The Div/Sqrt timings are counted in 33.51MHz units. Although the calculations
are quite fast, mind that reading/writing the result/parameter registers takes
up additional clock cycles (especially due to the PENALTY cycle glitch for
non-sequential accesses; parts of that problem can be eventually bypassed by
using sequential STMIA/LDMIA opcodes) (nethertheless, in some cases, software
may be actually faster than the hardware registers; eg. for small 8bit numbers;
that of course NOT by using the BIOS software functions which are endless
inefficient).

| DS Inter Process Communication (IPC) |
| --- |

Allows to exchange status information between ARM7 and ARM9 CPUs.

The register can be accessed simultaneously by both CPUs (without violating
access permissions, and without generating waitstates at either side).

**4000180h - NDS9/NDS7 - IPCSYNC - IPC Synchronize Register (R/W)**

```

| Bit Dir Expl.
 0-3 R Data input from IPCSYNC Bit8-11 of remote CPU (00h..0Fh)
 4-7 - Not used
 8-11 R/W Data output to IPCSYNC Bit0-3 of remote CPU (00h..0Fh)
 12 - Not used
 13 W Send IRQ to remote CPU (0=None, 1=Send IRQ)
 14 R/W Enable IRQ from remote CPU (0=Disable, 1=Enable)
 15-31 - Not used |
| --- |

**4000184h - NDS9/NDS7 - IPCFIFOCNT - IPC Fifo Control Register (R/W)**

```

| Bit Dir Expl.
 0 R Send Fifo Empty Status (0=Not Empty, 1=Empty)
 1 R Send Fifo Full Status (0=Not Full, 1=Full)
 2 R/W Send Fifo Empty IRQ (0=Disable, 1=Enable)
 3 W Send Fifo Clear (0=Nothing, 1=Flush Send Fifo)
 4-7 - Not used
 8 R Receive Fifo Empty (0=Not Empty, 1=Empty)
 9 R Receive Fifo Full (0=Not Full, 1=Full)
 10 R/W Receive Fifo Not Empty IRQ (0=Disable, 1=Enable)
 11-13 - Not used
 14 R/W Error, Read Empty/Send Full (0=No Error, 1=Error/Acknowledge)
 15 R/W Enable Send/Receive Fifo (0=Disable, 1=Enable)
 16-31 - Not used |
| --- |

**4000188h - NDS9/NDS7 - IPCFIFOSEND - IPC Send Fifo (W)**

```

| Bit0-31 Send Fifo Data (max 16 words; 64bytes) |
| --- |

**4100000h - NDS9/NDS7 - IPCFIFORECV - IPC Receive Fifo (R)**

```

| Bit0-31 Receive Fifo Data (max 16 words; 64bytes) |
| --- |

**IPCFIFO Notes**

When IPCFIFOCNT.15 is disabled: Writes to IPCFIFOSEND are ignored (no data is
stored in the FIFO, the error bit doesn't get set though), and reads from
IPCFIFORECV return the oldest FIFO word (as usually) (but without removing the
word from the FIFO).

When the Receive FIFO is empty: Reading from IPCFIFORECV returns the most
recently received word (if any), or ZERO (if there was no data, or if the FIFO
was cleared via IPCFIFOCNT.3), and, in either case the error bit gets set.

The Fifo-IRQs are edge triggered, IF.17 gets set when the condition
"(IPCFIFOCNT.2 AND IPCFIFOCNT.0)" changes from 0-to-1, and IF.18 gets set when
"(IPCFIFOCNT.10 AND NOT IPCFIFOCNT.8)" changes from 0-to-1. The IRQ flags can
be acknowledged even while that conditions are true.

| DS Keypad |
| --- |

For the GBA-buttons: Same as GBA, both ARM7 and ARM9 have keyboard input
registers, and each its own keypad IRQ control register.

GBA Keypad Input

For Touchscreen (and Microphone) inputs, see

DS Touch Screen Controller (TSC)

**4000136h - NDS7 - EXTKEYIN - Key X/Y Input (R)**

```

| 0 Button X (0=Pressed, 1=Released)
 1 Button Y (0=Pressed, 1=Released)
 2 Unknown / set
 3 DEBUG button (0=Pressed, 1=Released/None such)
 4,5 Unknown / set
 6 Pen down (0=Pressed, 1=Released/Disabled) (always 0 in DSi mode)
 7 Hinge/folded (0=Open, 1=Closed)
 8..15 Unknown / zero |
| --- |

The Hinge stuff is a magnetic sensor somewhere underneath of the Start/Select
buttons (NDS) or between A/B/X/Y buttons (DSi), it will be triggered by the
magnet field from the right speaker when the console is closed. The hinge
generates an interrupt request (there seems to be no way to disable this,
unlike as for all other IRQ sources), however, the interrupt execution can be
disabled in IE register (as for other IRQ sources).

The Pen Down is the /PENIRQ signal from the Touch Screen Controller (TSC), if
it is enabled in the TSC control register, then it will notify the program when
the screen pressed, the program should then read data from the TSC (if there's
no /PENIRQ then doing unneccassary TSC reads would just waste CPU power).
However, the user may release the screen before the program performs the TSC
read, so treat the screen as not pressed if you get invalid TSC values (even if
/PENIRQ was LOW).

Not sure if the TSC /PENIRQ is actually triggering an IRQ in the NDS?

The Debug Button should be connected to R03 and GND (on original NDS, R03 is
the large soldering point between the SL1 jumper and the VR1 potentiometer)
(there is no R03 signal visible on the NDS-Lite board).

Interrupts are reportedly not supported for X,Y buttons.

| DS Absent Link Port |
| --- |

The DS doesn't have a Serial Link Port Socket, however, internally, the NDS7
contains the complete set of Serial I/O Ports, as contained in the GBA:

GBA Communication Ports

In GBA mode, the ports are working as on real GBA (as when no cable is
connected). In NDS mode, the ports are even containing some additional bits:

**NDS7 SIO Bits (according to an early I/O map from Nintendo)**

```

| NDS7 4000128h SIOCNT Bit15 "CKUP" New Bit in NORMAL/MULTI/UART mode (R/W)
 NDS7 4000128h SIOCNT Bit14 "N/A" Removed IRQ Bit in UART mode (?)
 NDS7 400012Ah SIOCNT_H Bit14 "TFEMP" New Bit (R/W)
 NDS7 400012Ah SIOCNT_H Bit15 "RFFUL" New Bit (always zero?)
 NDS7 400012Ch SIOSEL Bit0 "SEL" New Bit (always zero?)
 NDS7 4000140h JOYCNT Bit7 "MOD" New Bit (R/W) |
| --- |

The "CKUP" bit duplicates the internal clock transfer rate (selected in
SIOCNT.1) (tested in normal mode) (probably works also in multi/uart mode?).

**NDS7 DS-Lite 4001080h (W) (?)**

DS-Lite Firmware writes FFFFh to this address (prior to accessing SIOCNT), so
it's probably SIO or debugging related (might be as well a bug or so). Reading
from the port always returns 0000h on both DS and DS-Lite.

**NDS9 SIO Bits (according to an early I/O map from Nintendo)**

```

| NDS9 4000120h SIODATA32 Bit0-31 Data (always zero?)
 NDS9 4000128h SIOCNT Bit2 "TRECV" New Bit (always zero?)
 NDS9 4000128h SIOCNT Bit3 "TSEND" New Bit (always zero?)
 NDS9 400012Ch SIOSEL Bit0 "SEL" New Bit (always zero?) |
| --- |

Not sure if these ports really exist in the release-version, or if it's been
prototype stuff?

**RCNT**

RCNT (4000134h) should be set to 80xxh (general purpose mode) before accessing
EXTKEYIN (4000136h) or RTC (4000138h). No idea why (except when using
RTC/SI-interrupt).

**DS Serial Port**

The SI line is labeled "INT" on the NDS mainboard, it is connected to Pin 1 of
the RTC chip (ie. the /INT interrupt pin).

I have no idea where to find SO, SC, and SD. I've written a test proggy that
pulsed all four RCNT bits - but all I could find was the SI signal. However,
the BIOS contains some code that uses SIO normal mode transfers (for the debug
version), so at least SI, SO, SC should exist...?

MAYBE that three signals are somehow replaced by EXTKEYIN bit0,1,3?

| DS Real-Time Clock (RTC) |
| --- |

**NDS**

Seiko Instruments Inc. S-35180 (compatible with S-35190A)

Miniature 8pin RTC with 3-wire serial bus

**DSi**

Seiko S-35199A01 (12pin BGA, with some extra functions like FOUT and Alarm
Date)

**4000138h - NDS7 - Real Time Clock Register**

```

| Bit Expl.
 0 Data I/O (0=Low, 1=High)
 1 Clock Out (0=Low, 1=High)
 2 Select Out (0=Low, 1=High/Select)
 4 Data Direction (0=Read, 1=Write)
 5 Clock Direction (should be 1=Write)
 6 Select Direction (should be 1=Write)
 3,8-11 Unused I/O Lines
 7,12-15 Direction for Bit3,8-11 (usually 0)
 16-31 Not used |
| --- |

**Serial Transfer Flowchart**

Chipselect and Command/Parameter Sequence:

```

| Init CS=LOW and /SCK=HIGH, and wait at least 1us
 Switch CS=HIGH, and wait at least 1us
 Send the Command byte (see bit-transfer below)
 Send/receive Parameter byte(s) associated with the command (see below)
 Switch CS to LOW |
| --- |

Bit transfer (repeat 8 times per cmd/param byte) (bits transferred LSB first):

```

| Output /SCK=LOW and SIO=databit (when writing), then wait at least 5us
 Output /SCK=HIGH, wait at least 5us, then read SIO=databit (when reading)
 In either direction, data is output on (or immediately after) falling edge. |
| --- |

Ideally, <both> commands and parameters should be transmitted LSB-first
(unlike the original Seiko document, which recommends LSB-first for data, and
MSB-first for commands) (actually, later Seiko datasheets are going so far to
recommend MSB-first for everything, eg. to use bit-reversed Data=C8h for
Year=13h).

**Command Register**

```

| Command Register
 Fwd Rev
 0 7 Fixed Code (must be 0)
 1 6 Fixed Code (must be 1)
 2 5 Fixed Code (must be 1)
 3 4 Fixed Code (must be 0, or, DSi only: 1=Extended Command)
 4-6 3-1 Command
 Fwd Rev Parameter bytes (read/write access)
 0 0 1 byte, status register 1
 4 1 1 byte, status register 2
 2 2 7 bytes, date & time (year,month,day,day_of_week,hh,mm,ss)
 6 3 3 bytes, time (hh,mm,ss)
 1* 4* 1 byte, int1, frequency duty setting
 1* 4* 3 bytes, int1, alarm time 1 (day_of_week, hour, minute)
 5 5 3 bytes, int2, alarm time 2 (day_of_week, hour, minute)
 3 6 1 byte, clock adjustment register
 7 7 1 byte, free register
 Extended command (when above "fourth bit" was set, DSi only)
 Fwd Rev Parameter bytes (read/write access)
 0 0 3 byte, up counter (msw,mid,lsw) (read only)
 4 1 1 byte, FOUT register setting 1
 2 2 1 byte, FOUT register setting 2
 6 3 reserved
 1 4 3 bytes, alarm date 1 (year,month,day)
 5 5 3 bytes, alarm date 2 (year,month,day)
 3 6 reserved
 7 7 reserved
 7 0 Parameter Read/Write Access (0=Write, 1=Read) |
| --- |

* INT1: Type and number of parameters depend on INT1 setting in stat reg2.

The "Fwd" bit numbers and command values for LSB-first command transfers (ie.
both commands and parameters use the same bit-order).

The "Rev" numbers/values are for MSB-first command transfers (ie. commands
using opposite bit-order than parameters, as being suggested by Seiko).

**Control and Status Registers**

```

| Status Register 1
 0 W Reset (0=Normal, 1=Reset)
 1 R/W 12/24 hour mode (0=12 hour, 1=24 hour)
 2-3 R/W General purpose bits
 4 R Interrupt 1 Flag (1=Yes) ;auto-cleared on read
 5 R Interrupt 2 Flag (1=Yes) ;auto-cleared on read
 6 R Power Low Flag (0=Normal, 1=Power is/was low) ;auto-cleared on read
 7 R Power Off Flag (0=Normal, 1=Power was off) ;auto-cleared on read
 Power off indicates that the battery was removed or fully discharged,
 all registers are reset to 00h (or 01h), and must be re-initialized.
 Status Register 2
 0-3 R/W INT1 Mode/Enable
 0000b Disable
 0x01b Selected Frequency steady interrupt
 0x10b Per-minute edge interrupt
 0011b Per-minute steady interrupt 1 (duty 30.0 seconds)
 0100b Alarm 1 interrupt
 0111b Per-minute steady interrupt 2 (duty 0.0079 seconds)
 1xxxb 32kHz output
 4-5 R/W General purpose bits
 6 R/W INT2 Enable
 0b Disable
 1b Alarm 2 interrupt
 7 R/W Test Mode (0=Normal, 1=Test, don't use) (cleared on Reset)
 Clock Adjustment Register (to compensate oscillator inaccuracy)
 0-7 R/W Adjustment (00h=Normal, no adjustment)
 Free Register
 0-7 R/W General purpose bits |
| --- |

**Date Registers**

```

| Year Register
 0-7 R/W Year (BCD 00h..99h = 2000..2099)
 Month Register
 0-4 R/W Month (BCD 01h..12h = January..December)
 5-7 - Not used (always zero)
 Day Register
 0-5 R/W Day (BCD 01h..28h,29h,30h,31h, range depending on month/year)
 6-7 - Not used (always zero)
 Day of Week Register (septenary counter)
 0-2 R/W Day of Week (00h..06h, custom assignment, usually 0=Monday?)
 3-7 - Not used (always zero) |
| --- |

**Time Registers**

```

| Hour Register
 0-5 R/W Hour (BCD 00h..23h in 24h mode, or 00h..11h in 12h mode)
 6 * AM/PM (0=AM before noon, 1=PM after noon)
 * 24h mode: AM/PM flag is read only (PM=1 if hour = 12h..23h)
 * 12h mode: AM/PM flag is read/write-able
 * 12h mode: Observe that 12 o'clock is defined as 00h (not 12h)
 7 - Not used (always zero)
 Minute Register
 0-6 R/W Minute (BCD 00h..59h)
 7 - Not used (always zero)
 Second Register
 0-6 R/W Minute (BCD 00h..59h)
 7 - Not used (always zero) |
| --- |

**Alarm 1 and Alarm 2 Registers**

```

| Alarm1 and Alarm2 Day of Week Registers (INT1 and INT2 each)
 0-2 R/W Day of Week (00h..06h)
 3-6 - Not used (always zero)
 7 R/W Compare Enable (0=Alarm every day, 1=Alarm only at specified day)
 Alarm1 and Alarm2 Hour Registers (INT1 and INT2 each)
 0-5 R/W Hour (BCD 00h..23h in 24h mode, or 00h..11h in 12h mode)
 6 R/W AM/PM (0=AM, 1=PM) (must be correct even in 24h mode?)
 7 R/W Compare Enable (0=Alarm every hour, 1=Alarm only at specified hour)
 Alarm1 and Alarm2 Minute Registers (INT1 and INT2 each)
 0-6 R/W Minute (BCD 00h..59h)
 7 R/W Compare Enable (0=Alarm every min, 1=Alarm only at specified min)
 Selected Frequency Steady Interrupt Register (INT1 only) (when Stat2/Bit2=0)
 0 R/W Enable 1Hz Frequency (0=Disable, 1=Enable)
 1 R/W Enable 2Hz Frequency (0=Disable, 1=Enable)
 2 R/W Enable 4Hz Frequency (0=Disable, 1=Enable)
 3 R/W Enable 8Hz Frequency (0=Disable, 1=Enable)
 4 R/W Enable 16Hz Frequency (0=Disable, 1=Enable)
 The signals are ANDed when two or more frequencies are enabled,
 ie. the /INT signal gets LOW when either of the signals is LOW.
 5-7 R/W General purpose bits |
| --- |

Note: There is only one register shared as "Selected Frequency Steady
Interrupt" (accessed as single byte parameter when Stat2/Bit2=0) and as "Alarm1
Minute" (accessed as 3rd byte of 3-byte parameter when Stat2/Bit2=1), changing
either value will also change the other value.

**Up Counter (DSi only)**

```

| Up Counter Msw
 0-7 R Up Counter bit16-23 (non-BCD, 00h..FFh)
 Up Counter Mid
 0-7 R Up Counter bit8-15 (non-BCD, 00h..FFh)
 Up Counter Lsw
 0-7 R Up Counter bit0-7 (non-BCD, 00h..FFh) |
| --- |

The 24bit Up Counter is incremented when seconds=00h (that is, once per minute;
unless the Time is getting changed by write commands, which may cause some
stuttering). The Up Counter starts at 000000h upon power-up, and, if the
battery lasts that long: wraps from FFFFFFh to 000000h after about 30 years.

**Alarm 1 and Alarm 2 Date Registers (DSi only)**

```

| Alarm 1 and Alarm 2 Year Register
 0-7 R/W Year (BCD 00h..99h = 2000..2099)
 Alarm 1 and Alarm 2 Month Register
 0-4 R/W Month (BCD 01h..12h = January..December)
 5 - Not used (always zero)
 6 R/W Year Compare Enable (0=Ignore, 1=Enable)
 7 R/W Month Compare Enable (0=Ignore, 1=Enable)
 Alarm 1 and Alarm 2 Day Register
 0-5 R/W Day (BCD 01h..28h,29h,30h,31h, range depending on month/year)
 6 - Not used (always zero)
 7 R/W Day Compare Enable (0=Ignore, 1=Enable) |
| --- |

XXX unspecified if above Alarm Date stuff is really R/W (or write only)

**FOUT Register (DSi only)**

```

| FOUT Register Setting 1
 0-7 R/W Enable bits (bit0=256Hz, bit1=512Hz, ..., bit7=32768Hz)
 FOUT Register Setting 2
 0-7 R/W Enable bits (bit0=1Hz, bit1=2Hz, ..., bit7=128Hz)
 The above sixteen FOUT signals are ANDed when two or more frequencies are
 enabled, ie. the FOUT signal gets LOW when either of the signals is LOW. |
| --- |

Note: The FOUT pin goes to the DSi's wifi daughterboard: FOUT must be set to
32768Hz (FOUT1=80h, FOUT2=00h), that setting is required for exchanging Atheros
WMI commands/events, the firmware does take care of initializing that setting
(eg. needed after battery removal, or after overwriting the FOUT registers by
software).

**Interrupt**

There's only one /INT signal, shared for both INT1 and INT2.

In the NDS, it is connected to the SI-input of the SIO unit (and so, also
shared with SIO interrupts). To enable the interrupt, RCNT should be set to
8144h (Bit14-15=General Purpose mode, Bit8=SI Interrupt Enable, Bit6,2=SI
Output/High).

The Output/High settings seems to be used as pullup (giving faster reactions on
low-to-high transitions) (nethertheless, in most cases it seems to be also
working okay as Input, ie. with RCNT=8100h).

The RCNT interrupt is generated on high-to-low transitions on the SI line (but
only if the IRQ is enabled in RCNT.8, and only if RCNT is set to general
purpose mode) (note: changing RCNT.8 from off-to-on does NOT generate IRQs,
even when SI is LOW).

**Pin-Outs**

```

| 1 /INT 8 VDD
 2 XOUT 7 SIO
 3 XIN 6 /SCK
 4 GND 5 CS |
| --- |

| DS Serial Peripheral Interface Bus (SPI) |
| --- |

**Serial Peripheral Interface Bus**

SPI Bus is a 4-wire (Data In, Data Out, Clock, and Chipselect) serial bus.

The NDS supports the following SPI devices (each with its own chipselect).

DS Firmware Serial Flash Memory

DS Touch Screen Controller (TSC)

DS Power Management Device

**40001C0h - NDS7 - SPICNT - SPI Bus Control/Status Register**

```

| 0-1 Baudrate (0=4MHz/Firmware, 1=2MHz/Touchscr, 2=1MHz/Powerman., 3=512KHz)
 2 DSi: Baudrate MSB (4=8MHz, 5..7=None/0Hz) (when SCFG_EXT7.bit9=1)
 2 NDS: Not used (Zero)
 3-6 Not used (Zero)
 7 Busy Flag (0=Ready, 1=Busy) (presumably Read-only)
 8-9 Device Select (0=Powerman., 1=Firmware, 2=Touchscr, 3=Reserved)
 10 Transfer Size (0=8bit/Normal, 1=16bit/Bugged)
 11 Chipselect Hold (0=Deselect after transfer, 1=Keep selected)
 12-13 Not used (Zero)
 14 Interrupt Request (0=Disable, 1=Enable)
 15 SPI Bus Enable (0=Disable, 1=Enable) |
| --- |

The "Hold" flag should be cleared BEFORE transferring the LAST data unit, the
chipselect will be then automatically cleared after the transfer, the program
should issue a WaitByLoop(3) manually AFTER the LAST transfer.

**40001C2h - NDS7 - SPIDATA - SPI Bus Data/Strobe Register (R/W)**

The SPI transfer is started on writing to this register, so one must
<write> a dummy value (should be zero) even when intending to
<read> from SPI bus.

```

| 0-7 Data
 8-15 Not used (always zero, even in bugged-16bit mode) |
| --- |

During transfer, the Busy flag in SPICNT is set, and the written SPIDATA value
is transferred to the device (via output line), simultaneously data is received
(via input line). Upon transfer completion, the Busy flag goes off (with
optional IRQ), and the received value can be then read from SPIDATA, if
desired.

**Notes/Glitches**

SPICNT Bits 12,13 appear to be unused (always zero), although the BIOS
(attempts to) set Bit13=1, and Bit12=Bit11 when accessing the firmware.

The SPIDATA register is restricted to 8bit, so that only each 2nd byte will
appear in SPIDATA when attempting to use the bugged-16bit mode.

**Cartridge Backup Auxiliar SPI Bus**

The NDS Cartridge Slot uses a separate SPI bus (with other I/O Ports), see

DS Cartridge Backup

| DS Touch Screen Controller (TSC) |
| --- |

**Texas Instruments TSC2046 (NDS)**

**Asahi Kasei Microsystems AK4148AVT (NDS-Lite)**

The Touch Screen Controller (for lower LCD screen) is accessed via SPI bus,

DS Serial Peripheral Interface Bus (SPI)

**Control Byte (transferred MSB first)**

```

| 0-1 Power Down Mode Select
 2 Reference Select (0=Differential, 1=Single-Ended)
 3 Conversion Mode (0=12bit, max CLK=2MHz, 1=8bit, max CLK=3MHz)
 4-6 Channel Select (0-7, see below)
 7 Start Bit (Must be set to access Control Byte) |
| --- |

**Channel**

```

| 0 Temperature 0 (requires calibration, step 2.1mV per 1'C accuracy)
 1 Touchscreen Y-Position (somewhat 0B0h..F20h, or FFFh=released)
 2 Battery Voltage (not used, connected to GND in NDS, always 000h)
 3 Touchscreen Z1-Position (diagonal position for pressure measurement)
 4 Touchscreen Z2-Position (diagonal position for pressure measurement)
 5 Touchscreen X-Position (somewhat 100h..ED0h, or 000h=released)
 6 AUX Input (connected to Microphone in the NDS)
 7 Temperature 1 (difference to Temp 0, without calibration, 2'C accuracy) |
| --- |

All channels can be accessed in Single-Ended mode.

In differential mode, only channel 1,3,4,5 (X,Z1,Z2,Y) can be accessed.

On AK4148AVT, channel 6 (AUX) is split into two separate channels, IN1 and IN2,
separated by Bit2 (Reference Select). IN1 is selected when Bit2=1, IN2 is
selected when Bit2=0 (despite of the Bit2 settings, both IN1 and IN2 are using
single ended more). On the NDS-Lite, IN1 connects to the mircrophone (as on
original NDS), and the new IN2 input is simply wired to VDD3.3 (which is equal
to the external VREF voltage, so IN2 is always FFFh).

**Power Down Mode**

```

| Mode /PENIRQ VREF ADC Recommended use
 0 Enabled Auto Auto Differential Mode (Touchscreen, Penirq)
 1 Disabled Off On Single-Ended Mode (Temperature, Microphone)
 2 Enabled On Off Don't use
 3 Disabled On On Don't use |
| --- |

Allows to enable/disable the /PENIRQ output, the internal reference voltage
(VREF), and the Analogue-Digital Converter.

For AK4148AVT, Power Down modes are slightly different (among others, /PENIRQ
is enabled in Mode 0..2).

**Reference Voltage (VREF)**

VREF is used as reference voltage in single ended mode, at 12bit resolution one
ADC step equals to VREF/4096. The TSC generates an internal VREF of 2.5V
(+/-0.05V), however, the NDS uses as external VREF of 3.33V (sinks to 3.31V at
low battery charge), the external VREF is always enabled, no matter if internal
VREF is on or off. Power Down Mode 1 disables the internal VREF, which may
reduce power consumption in single ended mode. After conversion, Power Down
Mode 0 should be restored to re-enable the Penirq signal.

**Sending the first Command after Chip-Select**

Switch chipselect low, then output the command byte (MSB first).

**Reply Data**

The following reply data is received (via Input line) after the Command byte
has been transferred: One dummy bit (zero), followed by the 8bit or 12bit
conversion result (MSB first), followed by endless padding (zero).

Note: The returned ADC value may become unreliable if there are longer delays
between sending the command, and receiving the reply byte(s).

**Sending further Commands during/after receiving Reply Data**

In general, the Output line should be LOW during the reply period, however,
once when Data bit6 has been received (or anytime later), a new Command can be
invoked (started by sending the HIGH-startbit, ie. Command bit7),
simultaneously, the remaining reply-data bits (bit5..0) can be received.

In other words, the new command can be output after receiving 3 bits in 8bit
mode (the dummy bit, and data bits 7..6), or after receiving 7 bits in 12bit
mode (the dummy bit, and data bits 11..6).

In practice, the NDS SPI register always transfers 8 bits at once, so that one
would usually receive 8 bits (rather than above 3 or 7 bits), before outputting
a new command.

**Touchscreen Position**

Read the X and Y positions in 12bit differential mode, then convert the
touchscreen values (adc) to screen/pixel positions (scr), as such:

```

| scr.x = (adc.x-adc.x1) * (scr.x2-scr.x1) / (adc.x2-adc.x1) + (scr.x1-1)
 scr.y = (adc.y-adc.y1) * (scr.y2-scr.y1) / (adc.y2-adc.y1) + (scr.y1-1) |
| --- |

The X1,Y1 and X2,Y2 calibration points are found in Firmware User Settings,

DS Firmware User Settings

scr.x1,y1,x2,y2 are originated at 1,1 (converted to 0,0 by above formula).

**Touchscreen Pressure (not supported on DSi)**

To calculate the pressure resistance, in respect to X/Y/Z positions and X/Y
plate resistances, either of below formulas can be used,

```

| Rtouch = (Rx_plate*Xpos*(Z2pos/Z1pos-1))/4096
 Rtouch = (Rx_plate*Xpos*(4096/Z1pos-1)-Ry_plate*(1-Ypos))/4096 |
| --- |

The second formula requires less CPU load (as it doesn't require to measure
Z2), the downside is that one must know both X and Y plate resistance (or at
least their ratio). The first formula doesn't require that ratio, and so
Rx_plate can be set to any value, setting it to 4096 results in

```

| touchval = Xpos*(Z2pos/Z1pos-1) |
| --- |

Of course, in that case, touchval is just a number, not a resistance in Ohms.

**Touchscreen Notes**

It may be impossible to press locations close to the screen borders.

When pressing two or more locations the TSC values will be somewhere in the
middle of these locations.

The TSC values may be garbage if the screen becomes newly pressed or released,
to avoid invalid inputs: read TSC values at least two times, and ignore BOTH
positions if ONE position was invalid.

**Microphone / AUX Channel**

Observe that the microphone amplifier is switched off after power up, see:

DS Power Management Device

DS Power Control

**Temperature Calculation (not supported on DSi)**

TP0 decreases by circa 2.1mV per degree Kelvin. The voltage difference between
TP1 minus TP0 increases by circa 0.39mV (1/2573 V) per degree Kelvin. At
VREF=3.33V, one 12bit ADC step equals to circa 0.8mV (VREF/4096).

Temperature can be calculated at best resolution when using the current TP0
value, and two calibration values (an ADC value, and the corresponding
temperature in degrees kelvin):

```

| K = (CAL.TP0-ADC.TP0) * 0.4 + CAL.KELVIN |
| --- |

Alternately, temperature can be calculated at rather bad resolution, but
without calibration, by using the difference between TP1 and TP0:

```

| K = (ADC.TP1-ADC.TP0) * 8568 / 4096 |
| --- |

To convert Kelvin to other formats,

```

| Celsius: C = (K-273.15)
 Fahrenheit: F = (K-273.15)*9/5+32
 Reaumur: R = (K-273.15)*4/5
 Rankine: X = (K)*9/5 |
| --- |

The Temperature Range for the TSC 2046 chip is -40'C..+85'C (for AK4181AVT only
-20'C..+70'C). According to Nintendo, the DS should not be exposed to "extreme"
heat or cold, the optimal battery charging temperature is specified as
+10'C..+40'C.

The original firmware does not support temperature calibration, calibration is
supported by nocash firmware (if present). See Extended Settings,

DS Firmware Extended Settings

**Pin-Outs**

```

| ________
 VCC 1|o |16 DCLK
 X+ 2| |15 /CS
 Y+ 3| TSC |14 DIN
 X- 4| 2046 |13 BUSY
 Y- 5| |12 DOUT
 GND 6| |11 /PENIRQ
 VBAT 7| |10 IOVDD
 AUX 8|________|9 VREF |
| --- |

For AK4181AVT, same pins as above, except that IOVDD replaced by the new IN2
input, the pin is wired to VDD3.3 (so IN2 is always equal to VREF, which is
wired to VDD3.3, too) (and AUX is renamed to IN1, and is kept used for MIC
input).

**DSi Touchscreen Controller (in NDS mode)**

DSi in NDS mode does support only X, Y, and MIC (all other channels do return
FFFh in 12bit mode, and FFh in 8bit mode, ie. no pressure, no temperature, and
no GNDed battery sensor). On DSi, MIC does return data in both single-ended and
differential mode (unlike as on real NDS).

**DSi Touchscreen Controller (in DSi mode)**

The DSi touchscreen controller supports a NDS backwards compatibility mode.
But, in DSi mode, it is working entirely different (it's still accessed via SPI
bus, but with some new MODE/INDEX values).

DSi Touchscreen/Sound Controller

The NDS Touchscreen controller did additionally allow to read Temperature and
Touchscreen Pressure - unknown if the DSi is also supporting such stuff (via
whatever DSi-specific registers).

The touchscreen hardware can be switched to NDS compatibility mode (for older
games), but unknown how to do that.

| DS Power Control |
| --- |

The DS contains several Power Managment functions, some accessed via I/O ports
(described below), and some accessed via SPI bus:

DS Power Management Device

**4000304h - NDS9 - POWCNT1 - Graphics Power Control Register (R/W)**

```

| 0 Enable Flag for both LCDs (0=Disable) (Prohibited, see notes)
 1 2D Graphics Engine A (0=Disable) (Ports 008h-05Fh, Pal 5000000h)
 2 3D Rendering Engine (0=Disable) (Ports 320h-3FFh)
 3 3D Geometry Engine (0=Disable) (Ports 400h-6FFh)
 4-8 Not used
 9 2D Graphics Engine B (0=Disable) (Ports 1008h-105Fh, Pal 5000400h)
 10-14 Not used
 15 Display Swap (0=Send Display A to Lower Screen, 1=To Upper Screen)
 16-31 Not used |
| --- |

Use SwapBuffers command once after enabling Rendering/Geometry Engine.

Improper use of Bit0 may damage the hardware?

When disabled, corresponding Ports become Read-only, corresponding (palette-)
memory becomes read-only-zero-filled.

**4000304h - NDS7 - POWCNT2 - Sound/Wifi Power Control Register (R/W)**

```

| Bit Expl.
 0 Sound Speakers (0=Disable, 1=Enable) (Initial setting = 1)
 1 Wifi (0=Disable, 1=Enable) (Initial setting = 0)
 2-31 Not used |
| --- |

Note: Bit0 disables the internal Speaker only, headphones are not disabled.

Bit1 disables Port 4000206h, and Ports 4800000h-480FFFFh.

**4000206h - NDS7 - WIFIWAITCNT - Wifi Waitstate Control**

```

| Bit Expl.
 0-1 WS0 nonsequential time (0-3 = 10, 8, 6, 18 cycles) ;\4800000h-4807FFFh
 2 WS0 sequential time (0-1 = 6, 4 cycles) ;/ (used for RAM)
 3-4 WS1 nonsequential time (0-3 = 10, 8, 6, 18 cycles) ;\4808000h-480FFFFh
 5 WS1 sequential time (0-1 = 10, 4 cycles) ;/ (used for I/O)
 6-15 Not used (zero) |
| --- |

This register is initialized by firmware on power-up (set to 0030h, other
software shouldn't change that setting). The timings are per 16bit halfword
access.

Note: WIFIWAITCNT can be accessed only when enabled in POWCNT2.

**4000301h - NDS7 - HALTCNT - Low Power Mode Control (R/W)**

In Halt mode, the CPU is paused as long as (IE AND IF)=0.

In Sleep mode, most of the hardware including sound and video are paused, this
very-low-power mode could be used much like a screensaver.

```

| Bit Expl.
 0-5 Not used (zero)
 6-7 Power Down Mode (0=No function, 1=Enter GBA Mode, 2=Halt, 3=Sleep) |
| --- |

The HALTCNT register should not be accessed directly. Instead, the BIOS Halt,
Sleep, CustomHalt, IntrWait, or VBlankIntrWait SWI functions should be used.

BIOS Halt Functions

ARM CP15 System Control Coprocessor

The NDS9 does not have a HALTCNT register, instead, the Halt function uses the
co-processor opcode "mcr p15,0,r0,c7,c0,4" - this opcode locks up if interrupts
are disabled via IME=0 (unlike NDS7 HALTCNT method which doesn't check IME).

**4000300h - NDS7/NDS9 - POSTFLG - BYTE - Post Boot Flag (R/W)**

The NDS7 and NDS9 post boot flags are usually set upon BIOS/Firmware boot
completion, once when set the reset vector is redirected to the debug handler
of Nintendo's hardware debugger. That allows the NDS7 debugger to capture
accidental jumps to address 0, that appears to be a common problem with
HLL-programmers, asm-coders know that (and why) they should not jump to 0.

```

| Bit Expl.
 0 Post Boot Flag (0=Boot in progress, 1=Boot completed)
 1 NDS7: Not used (always zero), NDS9: Bit1 is read-writeable
 2-7 Not used (always zero) |
| --- |

There are some write-restrictions: The NDS7 register can be written to only
from code executed in BIOS (done by NDS boot ROM, or by DSi firmware, whereas
the DSi firmware is using the CpuSet SWI function to issue the POSTFLG write
from within ROM). Bit0 of both NDS7 and NDS9 registers cannot be cleared
(except by Reset) once when it is set. DSi games seem to run regardless of
POSTFLG, whilst NDS games somewhat refuse to run when POSTFLG=0.

**Memory Power Down Functions**

DS Main Memory Control

DS Firmware Serial Flash Memory

| DS Power Management Device |
| --- |

**Power Management Device - Mitsumi 3152A (NDS) / Mitsumi 3205B (NDS-LITE)**

The Power Management Device is accessed via SPI bus,

DS Serial Peripheral Interface Bus (SPI)

To access the device, write the Index Register, then read or write the data
register, and release the chipselect line when finished.

```
********************************
| Index Register
 Bit0-6 Register Select (0..3) (0..4 for DS-Lite) (0..7Fh for DSi)
 Bit7 Register Direction (0=Write, 1=Read)
 Register 0 - Powermanagement Control (R/W)
 Bit0 Sound Amplifier Enable (0=Disable, 1=Enable)
 (Old-DS: Disabled: Sound is very silent, but still audible)
 (DS-Lite: Disabled: Sound is NOT audible)
 (DSi in NDS Mode: R/W, but effect is unknown yet)
 (DSi in DSi Mode: Not used, Bit0 is always 1)
 Bit1 Sound Amplifier Mute (0=Normal, 1=Mute) (Old-DS Only, not DS-Lite)
 (Old-DS: Muted: Sound is NOT audible (that works only if Bit0=1)
 (DS-Lite: Not used, Bit1 is always zero)
 (DSi in NDS Mode: R/W, but effect is unknown yet)
 (DSi in DSi Mode: R/W, but effect is unknown yet)
 Bit2 Lower Backlight (0=Disable, 1=Enable)
 Bit3 Upper Backlight (0=Disable, 1=Enable)
 Bit4 Power LED Blink Enable (0=Always ON, 1=Blinking OFF/ON)
 Bit5 Power LED Blink Speed (0=Slow, 1=Fast) (only if Blink enabled)
 (DSi: Power LED Blinking isn't supported, neither in NDS nor DSi mode)
 Bit6 DS System Power (0=Normal, 1=Shut Down)
 Bit7 Not used (always 0)
 Register 1 - Battery Status (R)
 Bit0 Battery Power LED Status (0=Power Good/Green, 1=Power Low/Red)
 (DSi: Usually 0, not tested if it changes upon Power=Low)
 Bit1-7 Not used
 Register 2 - Microphone Amplifier Control (R/W)
 Bit0 Amplifier (0=Disable, 1=Enable)
 Bit1-7 Not used (always 0)
 (DSi in NDS Mode: looks same as NDS, ie. only bit0 is R/W)
 (DSi in DSi Mode: Not used, always FFh)
 Register 3 - Microphone Amplifier Gain Control (R/W)
 Bit0-1 Gain (0..3=Gain 20, 40, 80, 160)
 Bit2-7 Not used (always 0)
 (DSi in NDS Mode: looks same as NDS, ie. only bit0-1 are R/W)
 (DSi in DSi Mode: Not used, always FFh)
 Register 4 - DS-Lite and DSi Only - Backlight Levels/Power Source (R/W)
 Bit0-1 Backlight Brightness (0..3=Low,Med,High,Max) (R/W)
 (when bit2+3 are both set, then reading bit0-1 always returns 3)
 Bit2 Force Max Brightness when Bit3=1 (0=No, 1=Yes) (R/W)
 Bit3 External Power Present (0=No, 1=Yes) (Read-Only)
 Bit4-7 Unknown (Always 4) (Read-Only)
 (DSi in NDS Mode: looks same as in DSi mode)
 (DSi in DSi Mode: Bit0-1 are R/W, but ignored, bit2-3 are always 0)
 Register 10h - DSi Only - Backlight Mirrors & Reset (R/W)
 Bit0 Reset (0=No, 1=Reboot DSi) (same/similar as BPTWL reset feature?)
 Bit1 Unknown (R/W) (note: whatever it is, it isn't warmboot flag)
 Bit2-3 Mirror of Register 0, bit2-3 (backlight enable bits) (R/W)
 Bit4-7 Not used (always 0)
 Bit5 Not used (always 0) - but DSi bootrom sets that bit on boot error?
 (This register works in NDS mode and DSi mode, though it's mainly intended
 for NDS mode, eg. DS Download Play uses the Reset bit to return to DSi menu)
 (note: writing bit2 seems to affect BOTH bit1 and bit2 in register 0)
 Register 1Fh and 20h - DSi Only (?)
 DSi bootrom sets register 1Fh and 20h bit0-4 to value 1Fh on boot error,
 unknown purpose, seems to have no effect, maybe prototype backlight level? |
| --- |

On Old-DS, registers 4..7Fh are mirrors of 0..3. On DS-Lite, registers 5,6,7
are mirrors of 4, register 8..7Fh are mirrors of 0-7.

On DSi (in DS mode), index 0,1,2,3,4,10h are used (reads as
0Fh,00h,00h,01h,41h,0Fh - regardless of backlight level, and power source),
index 5..0Fh and 11h..7Fh return 00h (ie. unlike DS and DS-Lite, there are no
mirrors; aside from the mirrored bits in register 10h).

**Backlight Dimming / Backlight caused Shut-Down(s)**

The above bits are essentially used to switch Backlights on or off. However,
there a number of strange effects. Backlight dimming is possible by pulse width
modulation, ie. by using a timer interrupt to issue pulse widths of N% ON, and
100-N% OFF. Too long pulses are certainly resulting in flickering. Too short
pulses are ignored, the backlights will remain OFF, even if the ON and OFF
pulses are having the same length. Much too short pulses cause the power supply
to shut-down; after changing the backlight state, further changes must not
occur within the next (circa) 2500 clock cycles. The mainboard can be operated
without screens & backlights connected, however, if so, the power supply
will shut-down as soon as backlights are enabled.

Pulse width modulated dimming does also work on the DS-Lite, allowing to use
smoother fade in/out effects as when using the five "hardware" levels
(Off,Low,Med,High,Max).

| DS Main Memory Control |
| --- |

**Main Memory**

The DS Main Memory is 2Mx16bit (4MByte), 1.8V Pseudo SRAM (PSRAM); all Dynamic
RAM refresh is handled internally, the chip doesn't require any external
refresh signals, and alltogether behaves like Static RAM. Non-sequential access
time is 70ns, sequential (burst) access time is 12ns.

**Main Memory Control**

The memory chips contain built-in Control functions, which can be accessed via
Port 27FFFFEh and/or by EXMEMCNT Bit 14. Nintendo is using at least two
different types of memory chips in DS consoles, Fujitsu 82DBS02163C-70L, and ST
M69AB048BL70ZA8, both appear to have different control mechanisms, other chips
(with 8MB size) are used in the semi-professional DS hardware debuggers, and
further chips may be used in future, so using the memory control functions may
lead into compatibitly problems.

**Power Consumption / Power Control**

Power Consumption during operation (read/write access) is somewhat 30mA, in
standby mode (no read/write access) consumption is reduced to 100uA.

Furthermore, a number of power-down modes are supported: In "Deep" Power Down
mode the refresh is fully disabled, consumption is 10uA (and all data will be
lost), in "Partial" Power Down modes only fragment of memory is refreshed, for
smallest fragments, consumption goes to down to circa 50uA. The chip cannot be
accessed while it is in Deep or Partial Power Down mode.

**Fujitsu 82DBS02163C-70L**

The Configuration Register (CR) can be written to by the following sequence:

```

| LDRH R0,[27FFFFEh] ;read one value
 STRH R0,[27FFFFEh] ;write should be same value as above
 STRH R0,[27FFFFEh] ;write should be same value as above
 STRH R0,[27FFFFEh] ;write any value
 STRH R0,[27FFFFEh] ;write any value
 LDRH R0,[2400000h+CR*2] ;read, address-bits are defining new CR value |
| --- |

Do not access any other Main Memory addresses during above sequence (ie.
disable interrupts, and do not execute the sequence by code located in Main
Memory). The CR value is write-only. The CR bits are:

```

| Bit Expl.
 0-6 Reserved (Must be 7Fh)
 7 Write Control
 0=WE Single Clock Pulse Control without Write Suspend Function
 1=WE Level Control with Write Suspend Function)
 Burst Read/Single Write is not supported at WE Single Clock Mode.
 8 Reserved (Must be 1)
 9 Valid Clock Edge (0=Falling Edge, 1=Rising Edge)
 10 Single Write (0=Burst Read/Burst Write, 1=Burst Read/Single Write)
 11 Burst Sequence (0=Reserved, 1=Sequential)
 12-14 Read Latency (1=3 clocks, 2=4 clocks, 3=5 clocks, other=Reserved)
 15 Mode
 0=Synchronous: Burst Read, Burst Write
 1=Asynchronous: Page Read, Normal Write
 In Mode 1 (Async), only the Partial Size bits are used,
 all other bits, CR bits 0..18, must be "1".
 16-18 Burst Length (2=8 Words, 3=16Words, 7=Continous, other=Reserved)
 19-20 Partial Size (0=1MB, 1=512KB, 2=Reserved, 3=Deep/0 bytes) |
| --- |

The Power Down mode is entered by setting CE2=LOW, this can be probably done by
setting EXMEMCNT Bit14 to zero.

**ST Microelectronics M69AB048BL70ZA8**

The chip name decodes as PSRAM (M96), Asynchronous (A), 1.8V Burst (B), 2Mx16
(048), Two Chip Enables (B), Low Leakage (L), 70ns (70), Package (ZA),
-30..+85'C (8).

There are three data sheets for different PSRAM chips available at www.st.com
(unfortunately none for M69AB048BL70ZA8), each using different memory control
mechanisms.

**NDS9 BIOS**

The NDS9 BIOS contains the following Main Memory initialization code, that
method doesn't match up with any ST (nor Fujitsu) data sheets that I've seen.
At its best, it looks like a strange (and presumably non-functional) mix-up of
different ST control methods.

```

| STRH 2000h,[4000204h] ;EXMEMCNT, enable RAM, async mode
 LDRH R0,[27FFFFEh]
 STRH R0,[27FFFFEh]
 STRH R0,[27FFFFEh]
 STRH FFDFh,[27FFFFEh]
 STRH E732h,[27FFFFEh]
 LDRH R0,[27E57FEh]
 STRH 6000h,[4000204h] ;EXMEMCNT, enable RAM, normal mode |
| --- |

| DS Backwards-compatible GBA-Mode |
| --- |

When booting a 32pin GBA cartridge, the NDS is automatically switched into GBA
mode, in that mode all NDS related features are disabled, and the console
behaves (almost) like a GBA.

**GBA Features that are NOT supported on NDS in GBA Mode.**

Unlike real GBA, the NDS does not support 8bit DMG/CGB cartridges.

The undocumented Internal Memory Control register (Port 800h) isn't supported,
so the NDS doesn't allow to use 'overclocked' RAM.

The NDS doesn't have a link-port, so GBA games can be played only in single
player mode, link-port accessories cannot be used, and the NDS cannot run GBA
code via multiboot.

**GBA Features that are slightly different on NDS in GBA Mode.**

The CPU, Timers, and Sound Frequencies are probably clocked at 16.76MHz;
33.51MHz/2; a bit slower than the original GBA's 16.78MHz clock?

In the BIOS, a single byte in a formerly 00h-filled area has been changed from
00h to 01h, resulting in SWI 0Dh returning a different BIOS checksum.

The GBA picture can be shown on upper or lower screen (selectable in
boot-menu), the backlight for the selected screen is always on, resulting in
different colors & much better visibility than original GBA. Unlike GBA-SP,
the NDS doesn't have a backlight-button.

**Screen Border in GBA mode**

The GBA screen is centered in the middle of the NDS screen. The surrounding
pixels are defined by 32K-color bitmap data in VRAM Block A and B. Each frame,
the GBA picture is captured into one block, and is displayed in the next frame
(while capturing new data to the other block).

To get a flicker-free border, both blocks should be initialized to contain the
same image before entering GBA mode (usually both are zero-filled, resulting in
a plain black border).

Note: When using two different borders, the flickering will be irregular - so
there appears to be a frame inserted or skipped once every some seconds in GBA
mode?!

**Switching from NDS Mode to GBA Mode**

```

| --- NDS9: ---
 ZEROFILL VRAM A,B ;init black screen border (or other color/image)
 POWCNT=8003h ;enable 2D engine A on upper screen (0003h=lower)
 EXMEMCNT=... ;set Async Main Memory mode (clear bit14)
 IME=0 ;disable interrupts
 SWI 06h ;halt with interrupts disabled (lockdown)
 --- NDS7: ---
 POWERMAN.REG0=09h ;enable sound amplifier & upper backlight (05h=lower)
 IME=0 ;disable interrupts
 wait for VCOUNT=200 ;wait until VBlank
 SWI 1Fh with R2=40h ;enter GBA mode, by CustomHalt(40h) |
| --- |

After that, the GBA BIOS will be booted, the GBA Intro will be displayed, and
the GBA cartridge (if any) will be started.

| DS Debug Registers (Emulator/Devkits) |
| --- |

**No$gba Emulator Pseudo I/O Ports (no$gba) (GBA,NDS9,NDS7)**

```

| 4FFFA00h..A0Fh R Emulation ID (16 bytes, eg. "no$gba v2.7", padded with 20h)
 4FFFA10h W String Out (raw)
 4FFFA14h W String Out (with %param's)
 4FFFA18h W String Out (with %param's, plus linefeed)
 4FFFA1Ch W Char Out (nocash)
 4FFFA20h..A27h R Clock Cycles (64bit)
 4FFFA28h..A3Fh - N/A |
| --- |

Note: Above ports can be disabled via the "Debug I/O" option in no$gba setup.

**Ensata Emulator Pseudo I/O Ports (NDS9)**

```

| 4000640h (32bit) ;aka CLIPMTX_RESULT (mis-used to invoke detection)
 4000006h (16bit) ;aka VCOUNT (mis-used to get detection result)
 4FFF010h (32bit) ;use to initialize/unlock/reset something
 4FFF000h (8bit) ;debug message character output (used when Ensata detected) |
| --- |

The Ensata detection works by mis-using CLIPMTX_RESULT and VCOUNT registers:

```

| [4000640h]=2468ACE0h ;CLIPMTX_RESULT (on real hardware it's read-only)
 if ([4000006h] AND 1FFh)=10Eh ;VCOUNT (on real hardware it's 000h..106h)
 [4FFF010h]=13579BDFh ;\initialize/reset something
 [4FFF010h]=FDB97531h ;/
 Ensata=true
 else
 Ensata=false
 endif |
| --- |

Once when a commercial game has detected Ensata, it stops communicating with
the ARM7, and instead it does seem to want to communicate with the Ensata
executable (which has little to do with real NDS hardware). Ie. aside from
"unlocking" port 4FFF000h, it does also "lock" access to the ARM7 hardware
(like sound, touchscreen, RTC, etc).

**ISD (Intelligent Systems Debugger or so) I/O Ports**

The ISD ports seem to be real (non-emulated) debugging ports, mapped to the GBA
Slot region at 8000000h-9FFFFFFh, and used to output text messages, and
possible also other debugging stuff.

There are appear to be two variants: nitroemu and cgbemu (the latter appears to
be dating back to old 8bit CGB hardware; which was apparently still used for
the NDS two hardware generations later).

**NDS Devkit**

In Nintendo's devkit, debug messages are handled in file "os_printf.c", this
file detects the available hardware/software based debug I/O ports, and
redirects the [OS_PutString] vector to the corresponding string_out function
(eg. to OS_PutStringAris for writing a 00h-terminated string to port 4FFF000h).
With some minimal efforts, this could be redirected to the corresponding no$gba
debug I/O ports.