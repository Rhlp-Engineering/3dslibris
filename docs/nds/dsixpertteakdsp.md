# Dsixpertteakdsp

> Source: https://problemkaputt.de/gbatek.htm
> Section: Dsixpertteakdsp

DSi XpertTeak (DSP) 
| |
| --- |

 The DSi includes an XpertTeak Digital Signal Processor (DSP); which is consisting of a TeakLite II processor, plus some "expert" features (like DMA support). The thing appears to be intended for audio/video decoding, but it's left unused by most DSi games. However, it's used by the "Nintendo DSi Sound" and "Nintendo Zone" system utilities, and by the "Cooking Coach" cartridge.
 
 DSi Teak Misc
 
 **XpertTeak I/O Registers (on ARM side)**
 DSi Teak I/O Ports (on ARM9 Side)
 
 **XpertTeak I/O Registers (on Teak side)**
 DSi Teak MMIO - Register Summary
 DSi Teak MMIO[8000h] - Misc Registers (JAM/GLUE)
 DSi Teak MMIO[8020h] - Timers (TMR)
 DSi Teak MMIO[8050h] - Serial Port (SIO)
 DSi Teak MMIO[8060h] - Debug (OCEM, On-chip Emulation Module)
 DSi Teak MMIO[8080h] - PLL and Power (PMU, Power Management Unit)
 DSi Teak MMIO[80C0h] - Host Port Interface (APBP aka HPI)
 DSi Teak MMIO[80E0h] - AHBM - Advanced High Performance Bus Master
 DSi Teak MMIO[8100h] - Memory Interface Unit (MIU)
 DSi Teak MMIO[8140h] - Code Replacement Unit (CRU)
 DSi Teak MMIO[8180h] - Direct Memory Access (DMA)
 DSi Teak MMIO[8200h] - Interrupt Control Unit (ICU)
 DSi Teak MMIO[8280h] - Audio (Buffered Time Division Multiplexing Port)
 
 **TeakLite II Processor**
 DSi Teak CPU Registers
 DSi Teak CPU Control/Status Registers
 DSi Teak CPU Address Config/Step/Modulo
 DSi TeakLite II Instruction Set Encoding
 DSi TeakLite II Operand Encoding
 
 
 
| DSi Teak Misc |
| --- |

 **Teak Instruction Set References**
 There aren't any official references for the Teak instruction set. However, there's one document that has leaked into internet (plus some docs for older Oak instruction set):
 
```

| TeakLite Architecture Specification Revision 4.41 (DSP Group Inc.)
 OakDSPCore Technical Manuals for CWDSP1640 or CWDSP167x (LSI Logic)
 OakDSPCore DSP Subsystem AT75C (Atmel) |
| --- |

TeakLite II supports lots of additional opcodes, the only available info has
leaked in form of .DLLs which were (apparently by mistake) bundled with a
specific RVDS release version:

```

| TeakLite II disassembler dll in RVDS (RealView Developer Suite) 4.0 Pro |
| --- |

There's no known way to use the RVDS disassembler GUI to decipher Teak
binaries. However, Normmatt found a way to get the .DLLs to disassemble code
manually (via LoadLibrary and GetProcAddr), which in turn allowed to
disassemble all possible 65536 combinations for all opcodes.

BUG: That RVDS tool disassembles "R0425" operands to "r6" (instead of "r5"),
that's definetely wrong for the old "movr" opcode (TeakLite I didn't support
any "r6" register at all), and, when looking at disassembled code, it does also
look wrong for newer TL2 opcodes.

Actually, the registers CAN be redefined via ar0/ar1/arp0/arp1/arp2/arp3, so it
isn't actually a bug.

**Teak MMIO Registers**

There aren't official docs, but wwylele has discovered a debugger at that
contains definitions for most of the MMIO bits:

```

| searching for "teak" in the "search for chip" box on "www.lauterbach.com"
 should lead to this file: trace32_ceva-teak_r_2019_02_000108303_win64.zip |
| --- |

The zip contains a "perxteak.per" file that contains the MMIO bit definitions
(in txt format). The definitions for OCEM do accidentally refer to an older
version (without 18bit addresses). Definitions for GLUE, ICU, APBP, DMA, CRU,
GPIO, and BTDMP are missing (whereof a similar/older ICU variant is defined in
"peroak.per" in the same zip).

**Teak COFF Files**

The DSi Sound utility contains a COFF file called "aac.a" (inside of its nitro
filesystem), and aside from the binary, the file is also including a COFF
symbol table with labels in ASCII format.

**Teak Undefined Opcodes**

There are several "Undefined" opcodes: Any opcodes that have no instruction
assigned in the opcode encoding table (or that are excplicitely assigned as
"undefined" in the table). Opcodes with invalid parameters (eg. ArArp set to
6..7).

Some opcodes are also having "Unused" operand bits; these bits should be
usually zero (nonzero would supposedly mirror to the same instruction, but one
shouldn't do that).

Moreover, there are various special cases saying that certain opcodes may not
be used with certain registers, eg. "addh" shall not be used with operands
Ax,Bx,p (with unknown results when violating that rule).

**Teak Memory**

Memory is addressed in 16bit WORD units (not in bytes) with separate
Instruction and Data busses. Before starting the Teak, store the Teak program
code in New Shared WRAM, and then map that memory to Teak side via MBK
registers:

DSi New Shared WRAM (for ARM7, ARM9, DSP)

At Teak side, 16bit is the smallest addressable unit (so there's no
"byte-order" on Teak side - however, 16bit values should be stored in little
endian format on ARM side).

Confusingly, the "movpdw" opcode is doing a 32bit read with two 16bit words
ordered in big-endian (and, on ARM side, byte-fractions ordered in
little-endian). There are a few more opcodes that can read/write 32bits, with
optional address increment/decrement for the 2nd word, so endianness is
selectable in that cases; it's also common to use the SAME address for both
words (probably intended for scaling a 16bit memory value to 32bits).

**Teak Code Memory**

TeakLite II supports 18bit program memory addressing (unlike Teak/Oak which
supported only 16bit addresses). The 18bits allow to address max 256Kwords
(=512Kbytes) of code, whereas, the DSi can map only half that much memory to
the DSP (ie. max 256Kbytes code).

Call/Ret are always pushing/popping 32bit return addresses (even when doing
"near" calls within the same 16bit page).

**Teak Data Memory (RAM and Memory Mapped I/O)**

Teak Data Memory is addressed via 16bit address bus (via registers r0..r7),
allowing to access max 64Kwords (2Kwords of MMIO, plus 62Kwords of RAM). The
memory is divided into three sections (X/Z/Y-spaces), the size/location of that
sections can be changed via Port 8114h (in 1Kword units), and alongsides, the
MMIO base can be adjusted via Port 811Eh. The default areas are:

```

| 0000h..7FFFh X Space (for RAM, with 1-stage write-buffer) ;min zero
 8000h..87FFh Z Space (for Memory-mapped I/O, no write-buffer) ;min zero
 8800h..FFFFh Y Space (for RAM, with 1-stage write-buffer)) ;min 1Kword |
| --- |

Confusingly, the DSi Sound utility is mapping 128Kwords of RAM as Teak Data
memory, but it's unknown how to access all of that memory. The CPU opcodes, CPU
registers, and MMIO registers don't seem to allow to access more than 64Kwords
of Data. Maybe the extra memory is accessible via DMA, and maybe the CPU's
[r7+imm16] operands might allow to exceed the 64Kbyte range (though they might
as well wrap within 64Kbyte range, actually that's more likely, especially for
"signed" immediates).

**Teak Call Conventions (as done in "aac.a" from DSi Sound)**

Functions are called with up to four parameters in a0,a1,b0,b1 (or a0l, a1l,
b0l, b1l when needing only 16bits). Any further parameters are pushed on stack
before the function call (and are deallocated via "rets Imm8" opcode upon
return). Register r7 is often used as stack frame (for accessing pushed
incoming parameters & locally allocated variables).

Functions may smash a0, a1, b0, b1. A return value (if any) is stored in a0 (or
a0l). All other registers like r0..r7, sv, etc. should be left unchanged (or
pushed/popped when needed).

**Teak Speed**

Cycles per opcode are defined in the TeakLite document (not covering TeakLite
II opcodes though). Most instructions (even Multiply opcodes) can complete in a
single clock cycle. The main bottleneck appear to be memory access cycles: Code
and Data memory can be accessed in parallel, so the overall rule would be:

```

| NumCycles = max(NumberOfOpcodeWords, NumberOfDataReadsWrites) |
| --- |

Some exceptions with extra cycles are opcodes that are changing PC, or that do
read/write program memory (movd and movp). Opcodes exp, max, maxd, min are
having restrictions saying that the result may not be used by the "following
instruction".

The overall clock speed in the DSi is unknown; some years ago somebody seems to
have claimed it to be around 130MHz, but it's unclear where that info came
from.

```

| Teak actual CPU clock(s) are...
 134.055928MHz (aka 134MHz) <-- for Timer 0, SIO, DMA (no "waitstates")
 107.244742MHz (aka 134MHz/1.25) <-- for Timer 1, CPU (with "waitstates") |
| --- |

The ARM9 can access WRAM at 33MHz, whilst Teak seems to be capable of fetching
16bit opcodes and 16bit data at 107MHz from the same WRAM (apparently without
even using a cache).

| DSi Teak I/O Ports (on ARM9 Side) |
| --- |

**4004300h - DSi9 - DSP_PDATA - DSP Transfer Data Read FIFO (R)**

```

| 0-15 Data (one stage of the 16-stage Read FIFO) |
| --- |

**4004300h - DSi9 - DSP_PDATA - DSP Transfer Data Write FIFO (W)**

```

| 0-15 Data (one stage of the 16-stage Write FIFO) |
| --- |

**4004304h - DSi9 - DSP_PADR - DSP Transfer Address (W)**

```

| 0-15 Lower 16bit of Address in DSP Memory (in 16bit units) |
| --- |

For MMIO, only lower 10bit are used (ie. MMIO[8000h-87FFh] can be accessed via
8000h-87FFh or 0000h-07FFh or other mirrors).

For Data, the 16bit address allows to access 64K-words.

For Code, the 16bit address allows to access 64K-words, but Code works only in
write-direction?

For Data/Code, the upper 16bit of Address must be configured on DSP side, in
DMA channel 0 registers:

```

| MMIO[81BEh] - DMA Select Channel (must be 0 for below DMA 0 regisrers)
 MMIO[81C2h:0] - DMA Channel 0: Source Address, bit16-31 (R/W)
 MMIO[81C6h:0] - DMA Channel 0: Destination Address, bit16-31 (R/W) |
| --- |

The Auto-increment flag in DSP_PCFG.bit1 will increment the 16bit address after
each DSP_PDATA unit (but doesn't seem to increment the upper bits after address
xxFFFFh).

**4004308h - DSi9 - DSP_PCFG - DSP Configuration (R/W)**

```

| 0 DSP Reset (0=Release, 1=Reset) ;should be held "1" for 8 DSP clks
 1 DSP Transfer Address Auto-Increment (0=Off, 1=On)
 2-3 DSP Read Data Length (0=1 word, 1=8 words, 2=16 words, 3=FreeRun)
 4 DSP Read Start Flag (mem transfer via Read FIFO) (1=Start)
 5 Interrupt Enable Read FIFO Full (0=Off, 1=On)
 6 Interrupt Enable Read FIFO Not-Empty (0=Off, 1=On)
 7 Interrupt Enable Write FIFO Full (0=Off, 1=On)
 8 Interrupt Enable Write FIFO Empty (0=Off, 1=On)
 9 Interrupt Enable Reply Register 0 (0=Off, 1=On)
 10 Interrupt Enable Reply Register 1 (0=Off, 1=On)
 11 Interrupt Enable Reply Register 2 (0=Off, 1=On)
 12-15 DSP Memory Transfer (0=DSP/Data, 1=DSP/MMIO, 5=DSP/Code, 7=ARM/AHBM) |
| --- |

**400430Ch - DSi9 - DSP_PSTS - DSP Status (R)**

```

| 0 Read Transfer Underway Flag (0=No, 1=Yes/From DSP Memory)
 1 Write Transfer Underway Flag (0=No, 1=Yes/To DSP Memory)
 2 Peripheral Reset Flag (0=No/Ready, 1=Reset/Busy)
 3-4 Unused (0)
 5 Read FIFO Full Flag (0=No, 1=Yes/Full)
 6 Read FIFO Not-Empty Flag (0=No, 1=Yes, ARM9 may read PDATA)
 7 Write FIFO Full Flag (0=No, 1=Yes/Full)
 8 Write FIFO Empty Flag (0=No, 1=Yes/Empty)
 9 Semaphore IRQ Flag (0=None, 1=IRQ)
 10 Reply Register 0 Update Flag (0=Was Written by DSP, 1=No)
 11 Reply Register 1 Update Flag (0=Was Written by DSP, 1=No)
 12 Reply Register 2 Update Flag (0=Was Written by DSP, 1=No)
 13 Command Register 0 Read Flag (0=Was Read by DSP, 1=No)
 14 Command Register 1 Read Flag (0=Was Read by DSP, 1=No)
 15 Command Register 2 Read Flag (0=Was Read by DSP, 1=No) |
| --- |

Unknown if/when bit10-15 get reset... maybe after reading the status... or when
reading a reply or writing a new command?

**4004310h - DSi9 - DSP_PSEM - ARM9-to-DSP Semaphore (R/W)**

```

| 0-15 ARM9-to-DSP Semaphore 0..15 Flags (0=Off, 1=On) |
| --- |

Reportedly these flags are sent in ARM9-to-DSP direction (=seems correct).

Confusingly, the other DSP_Pxxx registers are for opposite direction?

**4004314h - DSi9 - DSP_PMASK - DSP-to-ARM9 Semaphore Mask (R/W)**

```

| 0-15 DSP-to-ARM9 Semaphore 0..15 Interrupt Disable (0=Enable, 1=Disable) |
| --- |

**4004318h - DSi9 - DSP_PCLEAR - DSP-to-ARM9 Semaphore Clear (W)**

```

| 0-15 DSP-to-ARM9 Semaphore 0..15 Clear (0=No Change, 1=Clear/Ack) |
| --- |

Reportedly clears bits in DSP_PSEM/4004310h. [that's probably nonsense,
clearing bits in DSP_SEM/400431Ch would make more sense?]

**400431Ch - DSi9 - DSP_SEM - DSP-to-ARM9 Semaphore Data (R)**

```

| 0-15 DSP-to-ARM9 Semaphore 0..15 Flags (0=Off, 1=On) |
| --- |

Reportedly these flags are received in DSP-to-ARM9 direction.

**4004320h - DSi9 - DSP_CMD0 - DSP Command Register 0 (R/W) (ARM9 to DSP)**

**4004328h - DSi9 - DSP_CMD1 - DSP Command Register 1 (R/W) (ARM9 to DSP)**

**4004330h - DSi9 - DSP_CMD2 - DSP Command Register 2 (R/W) (ARM9 to DSP)**

```

| 0-15 Command/Data to DSP |
| --- |

**4004324h - DSi9 - DSP_REP0 - DSP Reply Register 0 (R) (DSP to ARM9)**

**400432Ch - DSi9 - DSP_REP1 - DSP Reply Register 1 (R) (DSP to ARM9)**

**4004334h - DSi9 - DSP_REP2 - DSP Reply Register 2 (R) (DSP to ARM9)**

```

| 0-15 Reply/Data from DSP |
| --- |

**Further Teak related registers**

SCFG_CLK, SCFG_RST, SCFG_EXT registers, MBK registers, and SNDEXCNT register.

DSi Control Registers (SCFG)

DSi New Shared WRAM (for ARM7, ARM9, DSP)

DSi Microphone and SoundExt

And, for the final audio output and microphone input,

DSi Touchscreen/Sound Controller

| DSi Teak MMIO - Register Summary |
| --- |

I/O ports are mapped in "data memory" at 8000h-87FFh (2Kwords). All ports are
16bit wide, located at even word addresses (words at odd addresses seem to be
always 0000h, except, there ARE some odd ports used at 806xh).

**8000h Mirrors**

```

| 8000h..8002h 3300 3300 3300 R Mirror of Port 80D6h |
| --- |

**8004h JAM**

```

| 8004h 0000 0000 87FF R/W JAM Unknown
 8006h ? ? ? ?? JAM Unknown/Crash, DANGER (crashes on read)
 8008h..800Eh 3300 3300 3300 R Mirror of Port 80D6h |
| --- |

**8010h GLUE**

```

| 8010h 0000 0000 0003 R/W GLUE CFG0
 8012h 0000 0000 0003 R/W GLUE Unknown 2bit
 8014h 0000 0000 FFFF R/W GLUE Unknown 16bit
 8016h 0000 0000 0000 R GLUE Unknown (DSi=0000h, New3DS=00BAh)
 8018h 0000 0000 BDEF R/W GLUE Whatever Parity/Shuffle
 801Ah C902 C902 C902 R GLUE Chip config ID (for xpert_offsets_tbl)
 801Ch..801Eh 0003 0003 0003 R Mirror of port 8010h |
| --- |

**8020h Timers**

```

| 8020h 0000 0000 7xDF R/W Timer 0 Control (bit11=DANGER) ;\
 8022h 0000 0000 0000 W Timer 0 Trigger Event/Watchdog ;
 8024h 0000 0000 FFFF R/W Timer 0 Reload value, bit0-15 ; Timer 0
 8026h 0000 0000 FFFF R/W Timer 0 Reload value, bit16-31 ;
 8028h 0000 0000 0000 R Timer 0 Counter value, bit0-15 ;
 802Ah 0000 0000 0000 R Timer 0 Counter value, bit16-31 ;
 802Ch 0000 0000 FFFF R/W Timer 0 PWM Reload value, bit0-15 ;
 802Eh 0000 0000 FFFF R/W Timer 0 PWM Reload value, bit16-31 ;/
 8030h 0200 0200 7xDF R/W Timer 1 Control (bit11=DANGER) ;\
 8032h 0000 0000 0000 W Timer 1 Trigger Event/Watchdog ;
 8034h 0000 0000 FFFF R/W Timer 1 Reload value, bit0-15 ; Timer 1
 8036h 0000 0000 FFFF R/W Timer 1 Reload value, bit16-31 ;
 8038h 0000 0000 0000 R Timer 1 Counter value, bit0-15 ;
 803Ah 0000 0000 0000 R Timer 1 Counter value, bit16-31 ;
 803Ch 0000 0000 FFFF R/W Timer 1 PWM Reload value, bit0-15 ;
 803Eh 0000 0000 FFFF R/W Timer 1 PWM Reload value, bit16-31 ;/
 8040h..804Eh 3300 3300 3300 R Mirror of Port 80D6h |
| --- |

**8050h Serial Port (SIO)**

```

| 8050h 7000 0000 F03F R/W SIO Control
 8052h 0000 0000 7F7F R/W SIO Clock Divider
 8054h 0000 0000 0000 R+W SIO Data (R) and (W)
 8056h 0000 0000 0001 R/W SIO Enable
 8058h 0000 0000 0000 R SIO Status
 805Ah..805Eh F03F F03F F03F R Mirror of port 8050h |
| --- |

**8060h Debug (OCEM, On-chip Emulation Module)**

```

| 8060h 0105 0105 0105 R OCEM Program Flow Trace Buffer, bit0-15
 8061h 0000 0000 0000 R OCEM Program Flow Trace Buffer, bit16-31
 8062h FFFF 0000 FFFF R/W OCEM Program Break Address 1, bit0-15
 8063h 0F03 0000 0F03 R/W OCEM Program Break Address 1, bit16-31
 8064h FFFF 0000 FFFF R/W OCEM Program Break Address 2, bit0-15
 8065h 0F03 0000 0F03 R/W OCEM Program Break Address 2, bit16-31
 8066h FFFF 0000 FFFF R/W OCEM Program Break Address 3, bit0-15
 8067h 0F03 0000 0F03 R/W OCEM Program Break Address 3, bit16-31
 8068h 00FF 0000 00FF R/W OCEM Program Break Counter 1
 8069h 00FF 0000 00FF R/W OCEM Program Break Counter 2
 806Ah 00FF 0000 00FF R/W OCEM Program Break Counter 3
 806Bh FFFF 0000 FFFF R/W OCEM Data Break Mask
 806Ch FFFF 0000 FFFF R/W OCEM Data Break Address
 806Dh 0000 0000 R/W OCEM Breakpoint Enable Flags (DANGER)
 806Eh 3001 0000 FFFF R/W OCEM Mode/Indication?
 806Fh 0000 0000 BFFF R/W OCEM Breakpoint Status Flags
 8070h 0000 0000 0001 R/W OCEM Program Flow Trace Update Disable
 8072h 0000 0000 FFFF R/W Unknown 16bit?
 8074h C000 C000 C000 R OCEM Boot/Debug Mode
 8076h..807Eh 0105 0105 0105 R Mirror of port 8060h |
| --- |

**8080h PLL and Power (PMU, Power Management Unit)**

```

| 8080h C00E 0000 FFFF R/W PMU PLL Multiplier
 8082h 0001 0000 0001 R/W PMU PLL Power-on config
 8084h 8000 R/W PMU PLL Divider/Bypass (DANGER)
 8086h 0000 R/W PMU Wake/Shutdown Module(s)
 8088h 0000 0000 07BF R/W PMU Recover Module(s) on interrupt 0
 808Ah 0000 0000 07BF R/W PMU Recover Module(s) on interrupt 1
 808Ch 0000 0000 07BF R/W PMU Recover Module(s) on interrupt 2
 808Eh 0000 0000 07BF R/W PMU Recover Module(s) on vectored interrupt
 8090h 0000 0000 06BF R/W PMU Recover Module(s) on Timer 0 (no bit8)
 8092h 0000 0000 05BF R/W PMU Recover Module(s) on Timer 1 (no bit9)
 8094h 0000 0000 07BF R/W PMU Recover Module(s) on NMI
 8096h 0000 0000 0002 R/W PMU Recover DMA on external signal (bit1)
 8098h 0000 0000 0302 R/W PMU Breakpoint mask module(s) (bit1,8,9 only)
 809Ah 0000 0000 0003 R/W PMU Wake/Shutdown BTDMP(s)
 809Ch 0000 0000 0003 R/W PMU Recover BTDMP(s) on interrupt 0
 809Eh 0000 0000 0003 R/W PMU Recover BTDMP(s) on interrupt 1
 80A0h 0000 0000 0003 R/W PMU Recover BTDMP(s) on interrupt 2
 80A2h 0000 0000 0003 R/W PMU Recover BTDMP(s) on vectored interrupt
 80A4h 0000 0000 0003 R/W PMU Recover BTDMP(s) on Timer 0
 80A6h 0000 0000 0003 R/W PMU Recover BTDMP(s) on Timer 1
 80A8h 0000 0000 0003 R/W PMU Recover BTDMP(s) on NMI (undoc?)
 80AAh 0000 0000 FFFF R/W Unknown 16bit
 80ACh 0000 0000 FFFF R/W Unknown 16bit
 80AEh 0000 0000 FFFF R/W Unknown 16bit
 80B0h..80BEh FFFF FFFF FFFF R Mirror of port 8080h |
| --- |

**80C0h APBP (aka HPI Host Port Interface)**

```

| 80C0h xxxx xxxx xxxx R/W APBP DSP-to-ARM Reply 0
 80C2h 4300 4300 4300 R APBP ARM-to-DSP Command 0
 80C4h 0000 0000 FFFF R/W APBP DSP-to-ARM Reply 1
 80C6h 3123 3123 3123 R APBP ARM-to-DSP Command 1
 80C8h 0000 0000 FFFF R/W APBP DSP-to-ARM Reply 2
 80CAh 3223 3223 3223 R APBP ARM-to-DSP Command 2
 80CCh 0000 0000 FFFF R/W APBP DSP-to-ARM Semaphore Set Flags
 80CEh 0000 R/W APBP ARM-to-DSP Semaphore Interrupt Mask
 80D0h 0000 W? APBP ARM-to-DSP Semaphore Ack Flags
 80D2h AFFE AFFE AFFE R APBP ARM-to-DSP Semaphore Get Flags
 80D4h 0000 R/W APBP Control (DANGER: can crash cpu)
 80D6h 03C0 03C0 03C0 R APBP DSP-side Status
 80D8h 3B00 3B00 3B00 R APBP ARM-side Status (mirror of 400430Ch)
 80DAh..80DEh 0000 0000 0000 R Fixed 0000h |
| --- |

**80E0h Bus Config for DMA to external ARM-bus (AHBM)**

```

| 80E0h 0000 0000 0000 R AHBM Status
 80E2h+N*6 0000 0000 0FBF R/W AHBM Channel 0..2 Configure Burst/Data
 80E4h+N*6 0000 0000 03FF R/W AHBM Channel 0..2 Configure Whatever
 80E6h+N*6 0000 0000 00FF R/W AHBM Channel 0..2 Configure DMA
 80F4h 0000 0000 FC00 R/W Unknown 6bit? bit10-15 are used
 80F6h 0000 0000 0000 R? AHBM Internal FIFO (R) and maybe also (W?)
 80F8h 0000 0000 0000 R? Unknown always zero?
 80FAh 0000 0000 FFFF R/W Read/write-able(!) mirror of MMIO[80FCh]
 80FCh FFFF 0000 FFFF R/W Unknown 16bit?
 80FEh 0000 0000 FFFF R/W Unknown 16bit? |
| --- |

**8100h MIU (memory limits for X-,Y-,Z-Space, and Memory Mapped I/O base)**

```

| 8100h FFFF 0000 FFFF R/W MIU Waitstate Settings, bit0-15
 8102h 0FFF 0000 0FFF R/W MIU Waitstate Settings, bit16-31
 8104h 0000 0000 FFFF R/W MIU Waitstate Area Z0
 8106h 0000 0000 FFFF R/W MIU Waitstate Area Z1
 8108h 0000 0000 FFFF R/W MIU Waitstate Area Z2
 810Ah 0000 0000 FFFF R/W MIU Waitstate Area Z3
 810Ch 0014 0014 0014 R Mirror of port 811Ah
 810Eh 0000 0000 FFFF R/W MIU X Page (16bit) (or unused)
 8110h 0000 0000 00FF R/W MIU Y Page (8bit) (or unused)
 8112h 0000 R/W MIU Z Page (16bit) (or absolute page)(DANGER)
 8114h 1E20 R/W MIU X/Y Page Size for Page 0 (or all pages)
 8116h 1E20 0100 403F R/W MIU X/Y Page Size for Page 1 (or unused)
 8118h 1E20 0100 403F R/W MIU X/Y Page Size for Off-chip (or unused)
 811Ah 0014 00x4 R/W MIU Config for Misc stuff (DANGER)
 811Ch 0004 0000 007F R/W MIU Config for Program Page and Download Mem
 811Eh 8000 R/W MIU Base Address for MMIO Registers (DANGER)
 8120h 0000 0000 000F R/W MIU Observability Mode
 8122h 0000 0000 007F R/W MIU Pin Config?
 8124h..813Eh 0014 0014 0014 R Mirror of port 811Ah |
| --- |

**8140h Patch (CRU, Code Replacement Unit)**

```

| 8140h+N*4 0000 0000 FFFF R/W CRU Entry 0..14 Offset, bit0-15
 8142h+N*4 0000 0000 803F R/W CRU Entry 0..14 Offset, bit16-31
 817Ch 0000 0000 FFFF R/W CRU Entry 15 Offset, bit0-15 ;\with control
 817Eh 0000 0000 C03F R/W CRU Entry 15 Offset, bit16-31 ;/status bits |
| --- |

**8180h DMA (eight channels, port 81C0h..81Exh are bank-switched via 81BEh)**

```

| 8180h 0000 0000 0000 R DMA Internal: Channel Size0 Busy or so?
 8182h 0000 0000 0000 R DMA Internal: Channel Size1 Busy or so?
 8184h 0001 0000 00FF R/W DMA Channel Start Flags (1=Start/Busy)
 8186h 0000 0000 00FF R/W DMA Channel Pause Flags (1=Pause)
 8188h 0000 0000 0000 R DMA Channel End Flags for Size0
 818Ah 0000 0000 0000 R DMA Channel End Flags for Size1
 818Ch 0000 0000 0000 R DMA Channel End Flags for Size2 (all done)
 818Eh 3210 0000 7777 R/W DMA Whatever Slot Config, bit0-15
 8190h 7654 0000 7777 R/W DMA Whatever Slot Config, bit16-31
 8192h 0000 0000 7C03 R/W Unknown, R/W mask 7C03h
 8194h 0000 0000 0000 R DMA Internal: contains SRC_ADDR_L after DMA
 8196h 0000 0000 0000 R DMA Internal: contains DST_ADDR_L after DMA
 8198h..81B4h 0000 0000 0000 R Fixed 0000h
 81B6h 0000 0000 FFFF R/W Unknown, 16bit
 81B8h 0000 0000 FFFF R/W Unknown, 16bit
 81BAh 0000 0000 FFFF R/W Unknown, 16bit
 81BCh 0000 0000 FFFF R/W Unknown, 16bit
 81BEh 0000 0000 0007 R/W DMA Select Channel (bank for 81C0h-81Exh)
 81C0h:0..7 0000 0000 FFFF R/W DMA Channel: Source Address, bit0-15
 81C2h:0..7 0000 0000 FFFF R/W DMA Channel: Source Address, bit16-31
 81C4h:0..7 0000 0000 FFFF R/W DMA Channel: Destination Address, bit0-15
 81C6h:0..7 0000 0000 FFFF R/W DMA Channel: Destination Address, bit16-31
 81C8h:0..7 FFFF 0001 FFFF R/W DMA Channel: Size0 (usually total len)
 81CAh:0..7 0001 0001 FFFF R/W DMA Channel: Size1 (usually 1)
 81CCh:0..7 0001 0001 FFFF R/W DMA Channel: Size2 (usually 1)
 81CEh:0..7 0001 0000 FFFF R/W DMA Channel: Source Step0 ;-2,4,2,1
 81D0h:0..7 0001 0000 FFFF R/W DMA Channel: Source Step1 ;-4,2,2,1
 81D2h:0..7 0001 0000 FFFF R/W DMA Channel: Source Step2 ;-2,4,0,1
 81D4h:0..7 0001 0000 FFFF R/W DMA Channel: Destination Step0 ;-4,2,0,1
 81D6h:0..7 0001 0000 FFFF R/W DMA Channel: Destination Step1 ;-0,0,0,1
 81D8h:0..7 0001 0000 FFFF R/W DMA Channel: Destination Step2 ;-0,0,0,1
 81DAh:0..7 F200 0000 F7FF R/W DMA Channel: Memory Area Config
 81DCh:0..7 0000 0000 1FF7 R/W DMA Channel: Unknown, usually set to 0300h?
 81DEh:0..7 0000 0000 00FF R/W DMA Channel: Start/Stop/Control
 81E0h:0..7 0000 0000 0000 R DMA Internal: contains SRC_ADDR_L after DMA
 81E2h:0..7 0000 0000 0000 R DMA Internal: contains DST_ADDR_L after DMA
 81E4h:0..7 0000 0000 0000 R DMA Internal: contains SRC_ADDR_H after DMA
 81E6h:0..7 0000 0000 0000 R DMA Internal: contains DST_ADDR_H after DMA
 81E8h..81FEh 0000 0000 0000 R Fixed 0000h |
| --- |

**8200h ICU (interrupts)**

```

| 8200h 4020 4020 4020 R ICU Interrupt Pending Flags (1=Pending)
 8202h 0000 0000 0000 W ICU Interrupt Acknowledge (1=Clear)
 8204h 0000 0000 FFFF R/W ICU Interrupt Manual Trigger (1=Set)
 8206h 0000 0000 FFFF R/W ICU Enable Interrupt as int0 (1=Enable)
 8208h 0000 0000 FFFF R/W ICU Enable Interrupt as int1 (1=Enable)
 820Ah 0000 0000 FFFF R/W ICU Enable Interrupt as int2 (1=Enable)
 820Ch 0000 0000 FFFF R/W ICU Enable Interrupt as vint (1=Enable)
 820Eh 2000 0000 FFFF R/W ICU Interrupt Trigger mode (0=Level, 1=Edge)
 8210h 2000 0000 FFFF R/W ICU Interrupt Polarity (0=Normal, 1=Invert)
 8212h+N*4 0003 0000 8003 R/W ICU Vectored Interrupt 0..15 Addr, bit16-31
 8214h+N*4 FC00 0000 FFFF R/W ICU Vectored Interrupt 0..15 Addr, bit0-15
 8252h 0000 0000 FFFF R/W ICU Interrupt Master Disable (1=Off/undoc)
 8254h 0000 0000 5555 R/W Unknown, R/W mask 5555h
 8256h 0000 0000 5555 R/W Unknown, R/W mask 5555h
 8258h..827Eh 0000 0000 0000 R Mirror of Port 8200h |
| --- |

**8280h BTDMP Audio (two channels N=0..1, each with speaker out and mic in)**

```

| 8280h+N*80h 0005 0000 FFFF R/W BTDMP Receive Control ;\
 8282h+N*80h 0000 0000 7FE7 R/W BTDMP Receive Period ;
 8284h+N*80h 0000 0000 0FE7 R/W BTDMP Receive Usually 0004h ;
 8286h+N*80h 0000 0000 0003 R/W BTDMP Receive Usually 0021h ; RX
 8288h+N*80h 1FFF 0000 1FFF R/W BTDMP Receive Usually 0000h ; (microphone)
 828Ah+N*80h 0000 0000 0FFF R/W BTDMP Receive Usually 0000h ;
 828Ch+N*80h 0000 0000 3FFF R/W BTDMP Receive Usually 0000h ;
 828Eh+N*80h 0000 0000 FFFF R/W BTDMP Receive Usually unused ;
 8290h+N*80h 0000 0000 FFFF R/W BTDMP Receive Usually unused ;
 8292h+... 0000 0000 0000 R Fixed 0000h ;
 829Eh+N*80h 0000 0000 8000 R/W BTDMP Receive Enable ;/
 82A0h+N*80h 0005 0000 FFFF R/W BTDMP Transmit Control ;\
 82A2h+N*80h 0000 0000 7FE7 R/W BTDMP Transmit Period ;
 82A4h+N*80h 0000 0000 0FE7 R/W BTDMP Transmit Usually 0004h ;
 82A6h+N*80h 0000 0000 0003 R/W BTDMP Transmit Usually 0021h ; TX
 82A8h+N*80h 1FFF 0000 1FFF R/W BTDMP Transmit Usually 0000h ; (audio out)
 82AAh+N*80h 0000 0000 0FFF R/W BTDMP Transmit Usually 0000h ;
 82ACh+N*80h 0000 0000 3FFF R/W BTDMP Transmit Usually 0000h ;
 82AEh+N*80h 0000 0000 FFFF R/W BTDMP Transmit Usually unused ;
 82B0h+N*80h 0000 0000 FFFF R/W BTDMP Transmit Usually unused ;
 82B2h+... 0000 0000 0000 R Fixed 0000h ;
 82BEh+N*80h 0000 0000 8000 R/W BTDMP Transmit Enable ;/
 82C0h+N*80h 001x 001F 001F R BTDMP Receive FIFO Status ;\
 82C2h+N*80h 0057 005x 0057 R BTDMP Transmit FIFO Status ;
 82C4h+N*80h E0A1 FFFF E0A1 R BTDMP Receive FIFO Data ; RX/TX
 82C6h+N*80h 0000 0000 0000 W BTDMP Transmit FIFO Data ;
 82C8h+N*80h 0000 0000 0003 R/W BTDMP Receive FIFO Control ;
 82CAh+N*80h 0000 0000 0003 R/W BTDMP Transmit FIFO Control ;/
 82CCh+... 0000 0000 0000 R Fixed 0000h
 8380h..867Eh 03C0 03C0 03C0 R Mirror of Port 80D6h |
| --- |

**8680h ? (listed in "xpert_offsets_tbl", but there are just mirrors)**

```

| 8680h..87FEh 03C0 03C0 03C0 R Mirror of Port 80D6h |
| --- |

**8800h ? (listed in "perxteak.per", but seems to be always 0) (dbg aka H2C)**

```

| (this is called "Host-to-Core JAM protocol" and consists of 11bit values)
 (not sure if that are MMIO registers, or some 11bit data transfer protocol)
 8800h..8807h 0000 0000 0000 R? Fixed 0 (reportedly H2C aka dbg stuff?) |
| --- |

**xpert_offsets_tbl: (with baseIO=8000h)**

The DSi's aac.a file contains a "xpert_offsets_tbl" with three sets of I/O
regions, which are apparently related to different hardware versions:

```

| ? JAM GLUE TMR SIO OCEM PMU APBP AHBM MIU CRU DMA ICU AUDIO ?
 #0 3333 0000 0010 0020 0050 0060 0080 00A0 3333 00C0 3333 0100 0180 0200 3333
 #1 0000 0004 0010 0020 0050 0060 0080 00C0 00E0 0100 0140 0180 0200 0280 0680
 #2 3333 0004 0010 3333 3333 0020 0040 3333 3333 0060 3333 3333 0120 3333 3333 |
| --- |

All addresses are relative to the MMIO base (usually 8000h), 3333h is
apparently some dummy value for unsupported I/O areas.

The DSi does use set #1. The other two sets are probably relicts for older Teak
hardware that was never used in DSi consoles (for example, set #2 doesn't event
support the APBP region for cmd/reply/semaphore).

| DSi Teak MMIO[8000h] - Misc Registers (JAM/GLUE) |
| --- |

**MMIO[8004h] - JAM Unknown (R/W)**

```

| 0-10 Unknown (R/W) (0..7FFh=?)
 11-14 Unused (0)
 15 Unknown (R/W) (0..1=?) |
| --- |

Maybe this is for the 11bit "Host-to-Core JAM protocol"?

**MMIO[8006h] - JAM Unknown/Crash, DANGER (crashes on read)**

Crashes on read.

**MMIO[8010h] - GLUE CFG0 (R/W)**

GLUE_CFG0 - Number of wait-states for Z-space transactions in predefined zones

Uh, the above name does not match up with below descriptions?

```

| 0 Timer 1 clock source (0=107MHz/Core, 1=Timer0_TOUT)
 1 Timer 0 force restart upon Timer 1 output (0=No, 1=Yes)
 2-15 Unused (0) |
| --- |

**MMIO[8012h] - GLUE Unknown 2bit (R/W)**

```

| 0-1 Unknown (R/W) (0..3=?)
 2-15 Unused (0) |
| --- |

**MMIO[8014h] - GLUE Unknown 16bit (R/W)**

```

| 0-15 Unknown (R/W) (0..FFFFh=?) |
| --- |

**MMIO[8016h] - GLUE Unknown, DSi=0000h or New3DS=00BAh (R)**

```

| 0-15 Unknown (DSi: always 0000h, New3DS: always 00BAh) |
| --- |

**MMIO[8018h] - GLUE Whatever Parity/Shuffle (R/W)**

```

| 0-3 Value A (R/W)
 4 All four bits in Value A XORed together (R)
 5-8 Value B (R/W)
 9 All four bits in Value B XORed together (R)
 10-13 Value C (R/W)
 14 All four bits in Value C XORed together (R)
 15 Value D (R/W) |
| --- |

Unknown purpose, might be a randomizer/maths feature, or an interactive
chip-detection feature, or something completely different, like memory-control
function, or whatever.

**MMIO[801Ah] - GLUE Chip config ID (R)**

```

| 0-15 Fixed, always C902h on DSi and New3DS |
| --- |

Used for chip detect (for xpert_offsets_tbl).

**More Unknown Registers**

XpertTeak was announced to support GPIO, unknown where those registers are
located. Probably consist of 16bit direction and 16bit data registers (if it's
similar as in older Oak chips). A few GPIO-style bits seem to be in
st2/mod0/mod3 CPU registers.

Unknown what below is... maybe MMIO[8800h..8807h] or maybe 11bit values for JAM
registers at MMIO[8004h..8006h]... and/or Input-only direction GPIO?

**[dbg:800h] - H2C Host-to-Core JAM protocol h2c_0**

```

| 0 Reset (0=No, 1=Yes)
 1 Boot (0=No, 1=Yes)
 2 Debug (0=No, 1=Yes)
 4 URST (user reset) (0=No, 1=Yes)
 5 -
 6 Internal Program (Load code to on-chip memory) (0=No, 1=Yes)
 7-10 - |
| --- |

**[dbg:801h] - H2C Host-to-Core JAM protocol h2c_1**

```

| 0-6 -
 7 Continue core's clock after stopped by software (0=No, 1=Yes)
 8 Stop (Stop core's clock) (0=No, 1=Yes)
 9 NMI (0=No, 1=Yes)
 10 Abort (0=No, 1=Yes) |
| --- |

**[dbg:802h] - H2C Host-to-Core JAM protocol h2c_2**

```

| 0-10 Interrupt 0..10 (0=No, 1=Yes) |
| --- |

**[dbg:803h] - H2C Host-to-Core JAM protocol h2c_3**

```

| 0-10 GPI (General Purpose Input) 0..10 (0=Low, 1=High) |
| --- |

**[dbg:804h] - H2C Host-to-Core JAM protocol h2c_4**

```

| 0-4 GPI (General Purpose Input) 11..15 (0=Low, 1=High)
 5-8 -
 9-10 UI (User Input) 0..1 (0=Low, 1=High) |
| --- |

**[dbg:805h] - H2C Host-to-Core JAM protocol h2c_5**

```

| 0-10 Interrupt external/internal control 0..10 (0=Ext, 1=Int) |
| --- |

**[dbg:806h] - H2C Host-to-Core JAM protocol h2c_6**

```

| 0-10 GPI Enable Control 0..10 (0=Disable, 1=Enable) |
| --- |

**[dbg:807h] - H2C Host-to-Core JAM protocol h2c_7**

```

| 0-4 GPI Enable Control 11..15 (0=Disable, 1=Enable)
 5-8
 9-10 User Input Enable Control 0..1 (0=Disable, 1=Enable) |
| --- |

| DSi Teak MMIO[8020h] - Timers (TMR) |
| --- |

**MMIO[8020h/8030h] - Timer 0/1 Control (R/W)**

```

| 0-1 Time prescaler (0=Div1, 1=Div2, 2=Div4, 3=Div16)
 2-4 Count mode
 0h: single count Stop at zero
 1h: auto restart Wrap from zero to Reload value
 2h: free running Wrap from zero to FFFFFFFFh
 3h: event count Decrement manually, and stop at zero
 4h: watchdog mode 1 Trigger Teak Reset at zero
 5h: watchdog mode 2 Trigger Teak NMI at zero
 6h: watchdog mode 3 Trigger Unacknowledgeable-Timer-IRQ at zero?
 7h: reserved Same as mode 0 (stop at zero)
 5 Unused (0)
 6 Output signal polarity (0=Normal, 1=Invert/Buggy?)
 7 Clear output signal; when bit14-15=0 (0=No change, 1=Clear) (W)
 8 Pause the counter (0=Unpause, 1=Pause)
 9 Freeze COUNTER_L/H register value (0=Freeze, 1=Update)
 Note: Bit8/Bit9 can be forced to always 1 via other Timer's Bit13
 10 Restart/Reload the counter (0=No change, 1=Restart) (W)
 11 Breakpoint requests enable (0=Disable, 1=Enable) (DANGER/TRAP)
 12 Clock source (0=InternalClk=134MHz/107MHz, 1=ExternalClk=None)
 13 General Purpose (somehow interact between Timer 0 and 1)
 Timer0: Force Timer1.Control.Bit9=1 (0=No, 1=Yes/ForceUpdate)
 Timer1: Force Timer0.Control.Bit8=1 (0=No, 1=Yes/ForcePause)
 14-15 Clear output signal automatically (0=No, 1/2/3=After 2/4/8 cycles) |
| --- |

The InternalClk is different for Timer 0 and 1, and depends on GLUE setting:

```

| For Timer 0 (no "waitstates") --> 134.055928MHz
 For Timer 1 (with "waitstates") --> 107.244742MHz (aka 134.055928MHz/1.25)
 For Timer 1 (if GLUE_CFG0.bit0) --> Timer0_TOUT (bugs if Timer0_Reload<3)
 Unknown what "waitstates" refers to, probably not the Z0/Z1/Z2/Z3 waits? |
| --- |

Bit10 is automatically cleared after writing - however, when using prescaler
other than Div1, it may take a few cycles until bit10 gets cleared (and perhaps
also takes time until it is processed??? if so, one shouldn't clear bit10 in
that timeslot).

Bit7 is usually cleared after writing - but not always, still unknown what/when
that happens?

**MMIO[8022h/8032h] - Timer 0/1 Trigger Event/Watchdog (W)**

```

| 0 In Event Mode: Decrement Counter (0=No change, 1=Decrement)
 In Watchdog Mode: Reload Counter (0=No change, 1=Reload)
 1-15 Unused (0) |
| --- |

**MMIO[8024h/8034h] - Timer 0/1 Reload value, bit0-15 (R/W)**

**MMIO[8026h/8036h] - Timer 0/1 Reload value, bit16-31 (R/W)**

```

| 0-31 Start/Reload value for decrementing counter |
| --- |

Used as start/reload value; copied to counter when setting Control.bit10, or
after reaching 0 in auto-restart mode, or when writting trigger bit in watchdog
mode.

Not used in free-run mode (which starts at current counter value, and wraps
from 0 to FFFFFFFFh).

**MMIO[8028h/8038h] - Timer 0/1 Counter value, bit0-15 (R)**

**MMIO[802Ah/803Ah] - Timer 0/1 Counter value, bit16-31 (R)**

```

| 0-31 Current (or frozen) decrementing counter value |
| --- |

When using freeze, counter keeps decreasing in background, but reading the
counter register returns the frozen value (latched when changing Control.bit9
from 1-to-0).

**MMIO[802Ch/803Ch] - Timer 0/1 PWM Reload value, bit0-15 (R/W)**

**MMIO[802Eh/803Eh] - Timer 0/1 PWM Reload value, bit16-31 (R/W)**

```

| 0-31 Restart value for PWM counter (uh, maybe PWM duty?) |
| --- |

Unknown if/when/how this is used (maybe just goes to an unimplemented PWM
output pin).

| DSi Teak MMIO[8050h] - Serial Port (SIO) |
| --- |

This is a SPI-style serial I/O interface, allowing to connect some peripheral
or debugging hardware (unused in DSi/3DS).

**MMIO[8050h] - SIO Control (R/W)**

```

| 0 Chip Select Polarity (0=Active High, 1=Active Low)
 1 Chip Select Output (0=Disable/Hangs, 1=Enable) (for Master)
 2 Master/Slave Clock (0=FromClkDivider, 1=ExternalClk/Hangs)
 3 Clock Polarity (0=Idle Low, 1=Idle High)
 4 Clock Edge Phase (0=InputOnRising, 1=OutputOnRising)
 5 Transfer End Interrupt (0=Enable, 1=Disable/Hangs/NoStatusDone)
 6-11 Unused (0)
 12-15 Num data bits per transfer (0=Hangs, 1..15=2bit..16bit) |
| --- |

**MMIO[8052h] - SIO Clock Divider (R/W)**

```

| 0-6 Clock Divider 1 (1..7Fh = Div1..Div127) (0=Div1, too)
 7 Unused (0)
 8-14 Clock Divider 2 (1..7Fh = Div1..Div127) (0=Div1, too)
 15 Unused (0) |
| --- |

The transfer shift clock is 134MHz/(Divider1*Divider2).

**MMIO[8054h] - SIO Data (R) and (W)**

```

| 0-15 Transfer data (probably using only LSBs when NumBits<16) |
| --- |

Writing the transmit data starts the transfer, the reply data can be read when
Transfer Done status flag gets set.

**MMIO[8056h] - SIO Enable (R/W)**

```

| 0 Enable SIO operation (0=Disable/Hangs, 1=Enable)
 1-15 Unused (0) |
| --- |

**MMIO[8058h] - SIO Status (R)**

```

| 0 Transfer done (0=No, 1=Done, Data can be read now)
 1 Overrun error (0=No, 1=New data arrived before reading old data)
 2-15 Unused (0) |
| --- |

Bit0,1 are automatically cleared after reading the status register.

**SIO Transfer Timing Bugs**

After transfer end, one must apparently wait at least "Divider1*Divider2/2"
cycles before starting another transfer, else the new transfer won't start.

There are two dummy transfer clocks inserted (so transferring 2..16 bits does
actually take "(4..18)*Divider1*Divider2" cycles).

And, transfer start doesn't seem to take place until next clock boundary (that
may take between "0 and 0.999*Divider1*Divider2" extra cycles).

| DSi Teak MMIO[8060h] - Debug (OCEM, On-chip Emulation Module) |
| --- |

**MMIO[8060h] - OCEM Program Flow Trace Buffer (PFT), bit0-15 (R)**

**MMIO[8061h] - OCEM Program Flow Trace Buffer (PFT), bit16-31 (R)**

This buffer contains addresses of the 16 most recent jump opcodes, reading
returns the oldest of those 16 addresses (or 16 random address/page values if
read after reset). One can disable buffer updating via MMIO[8070h] (eg. to
avoid jumps inside of a debug handler to be written to the buffer). There is
"buffer full" condition that gets set 16 jumps after reset, and that can trap
an optional buffer full exception.

```

| 0-17 Program Flow Trace Address
 18-23 Unused (0)
 24-27 Program Flow Trace Page
 28-31 Unused (0) |
| --- |

Unknown if there is any way to "make the buffer empty", either by clearing the
buffer, or by removing entries until it gets empty. Also, there is no known way
to read the next buffer entry, except for one trick: temporarily re-enable
buffer updates via MMIO[8070h] and issue a dummy jump; this will add a new
entry, and reading will then return the next oldest entry; by that, the buffer
won't get empty (but, when ignoring the address of the dummy jump, one can
extract the actual non-dummy data from the buffer).

**MMIO[8062h/8064h/8066h] - OCEM Program Break Address 1/2/3, bit0-15 (R/W)**

**MMIO[8063h/8065h/8067h] - OCEM Program Break Address 1/2/3, bit16-31 (R/W)**

```

| 0-17 Program Break Address
 18-23 Unused (0)
 24-27 Program Break Page (should be usually 00h)
 28-31 Unused (0) |
| --- |

**MMIO[8068h/8069h/806Ah] - OCEM Program Break Counter 1/2/3 (R/W)**

```

| 0-7 Program Address Break Counter (decrements upon PC=break.addr)
 8-15 Unused (0) |
| --- |

Break triggers upon count zero. Writing count=NNh will trap after program
counter had hit the break address NNh times (writing count=00h will trigger
immediately, without even hitting the break address).

**MMIO[806Bh] - OCEM Data Break Mask (R/W)**

**MMIO[806Ch] - OCEM Data Break Address (R/W)**

```

| 0-15 Mask/Address |
| --- |

**MMIO[806Dh] - OCEM Breakpoint Enable Flags DANGER (0=Disable, 1=Enable/Trap)**

```

| 0 Data value break point on data write transaction
 1 Data value break point on data read transaction
 2 Data address break point as a result on data write transaction
 3 Data address break point as a result on data read transaction
 4 Simultaneous data address and data value match
 5 External register write transaction ;\(aka ext0/1/2/3?)
 6 External register read transaction ;/
 7 Program Address break 1 count zero
 8 Program Address break 2 count zero
 9 Program Address break 3 count zero
 10 Break on any program jumps instead of executing the next address
 11 Break on detection of interrupt service routine
 12 Break as a result of program flow trace buffer full
 13 Break when returning to the beginning of block repeat loop
 14 Break on illegal condition (uh, are that... illegal opcodes?)
 15 Single Step |
| --- |

Note: Data value refers to the "dvm" CPU-core register. Data write matches can
occur on things like "mov r0,1234h" or "addv r0,1" (when r0 becomes equal to
dvm).

**MMIO[806Eh] - OCEM Mode/Indication? (R/W)**

```

| 0 Program Flow Trace Buffer full (0=Not full/OldestIsGarbage, 1=Full)
 1-11 Unknown (R/W)
 12 MOVD instruction detected (uh, usually 1, even when not using movd?)
 13 User reset activated while in break point service routine
 14 Boot mode (0=No, 1=Yes)
 15 Debug mode (0=No, 1=Yes) |
| --- |

Unknown if this is a mode or status register, or mixup thereof?

**MMIO[806Fh] - OCEM Breakpoint Status Flags (R/W)**

```

| 0 Break caused by Data value match
 1 Break caused by Data address match
 2 Break caused by Data value and data address match
 3 Break caused by User defined register transaction (aka ext0/1/2/3?)
 4 Break caused by an external event (aka what?)
 5 Break caused by Program address break 1 count zero
 6 Break caused by Program address break 2 count zero
 7 Break caused by Program address break 3 count zero
 8-10 Unknown (R/W)
 11 Break caused by Branch break point
 12 Break caused by Interrupt break point
 13 Break caused by Program Flow Trace Buffer full
 14 Break caused by Illegal break point
 15 Break caused by Software trap |
| --- |

Uh, R/W mask is BFFFh, ie. bit14 is always 0, that doesn't match up with above
description?

**MMIO[8070h] - OCEM Program Flow Trace Update Disable (R/W)**

```

| 0 Disable Program Flow Buffer Updating (0=Enable, 1=Disable)
 1-15 Unused (0) |
| --- |

**MMIO[8072h] - Unknown 16bit? (R/W)**

```

| 0-15 Unknown (R/W) |
| --- |

Maybe be "programming model signature" mentioned in perxteak.per (though that
is claimed to use only upper 8bit)? That signature might be some general
purpose value to notify the debugger what kind of code is executed?

**MMIO[8074h] - OCEM Boot/Debug Mode (R)**

```

| 0-13 Unused? (0)
 14 Boot mode (0=No, 1=Yes)
 15 Debug mode (0=No, 1=Yes) |
| --- |

Usually contains the same mode bits as written to MMIO[806Eh].bit14/bit15
(maybe one of the registers contains the OLD mode bits; when entering a debug
handler or so).

| DSi Teak MMIO[8080h] - PLL and Power (PMU, Power Management Unit) |
| --- |

**MMIO[8080h] - PMU PLL Multiplier (R/W)**

```

| 0-15 Configuration of the PLL clock multiplication (0..FFFFh=what?) |
| --- |

Power-up: C00Eh. Unknown what that does... multiplying clock by 16bit seems
unlikely. Maybe alike CCR2 register in files peroak.per and pertklc.per?

Or maybe 0Ch and 0Eh translate to (14+1):12 ratio (ie. the 134MHz/1.25 divider;
though changing this register doesn't seem to affect that)?

**MMIO[8082h] - PMU PLL Power-on config (R/W)**

```

| 0 PLL power-on configuration value for PLL use (0..1=what)
 1-15 Unused (0) |
| --- |

Power-up: 0001h. Unknown what that is. Default value for PLB bit in next
register? Or maybe something needed for powering-up the PLL (when clearing
bypass flag)?

**MMIO[8084h] - PMU PLL Divider/Bypass (R/W?)**

```

| 0-6 Clock Divider (0 or 1=Div1) (2..7Fh=Crashes?)
 7-14 Unused (0)
 15 Bypass PLL (0=Use PLL/Crashes, 1=Bypass; works only if Div1) |
| --- |

Power-up: 8000h. Ie. PLL bypassed, and using 134MHz clock from DSi as is.

Unknown how to change/enable the PLL without crashing.

**MMIO[8086h,809Ah] - PMU Wake/Shutdown module(s) (0=Wake-up, 1=Shutdown) (R/W)**

**MMIO[8088h,809Ch] - PMU Recover module(s) on interrupt 0 (R/W)**

**MMIO[808Ah,809Eh] - PMU Recover module(s) on interrupt 1 (R/W)**

**MMIO[808Ch,80A0h] - PMU Recover module(s) on interrupt 2 (R/W)**

**MMIO[808Eh,80A2h] - PMU Recover module(s) on vectored interrupt (R/W)**

**MMIO[8090h,80A4h] - PMU Recover module(s) on Timer 0 (except bit8) (R/W)**

**MMIO[8092h,80A6h] - PMU Recover module(s) on Timer 1 (except bit9) (R/W)**

**MMIO[8094h,80A8h] - PMU Recover module(s) on non-maskable interrupt (R/W)**

**MMIO[8096h] - PMU Recover DMA module on external signal (bit1 only) (R/W)**

```

| For registers MMIO[8086h..8096h]:
 0 Core ;aka cpu?
 1 DMA
 2 SIO
 3 GLUE
 4 APBP/HPI
 5 AHBM
 6 Unused (0)
 7 OCEM
 8 Timer 0
 9 Timer 1
 10 JAM
 11-15 Unused (0)
 For registers MMIO[809Ah..80A8h]:
 0 BTDMP 0
 1 BTDMP 1
 2-15 Unused (0) |
| --- |

Register 8090h.bit8 and 8092h.bit9 are unused, always 0 (ie. a disabled timer
cannot wakeup itself).

Register 8096h supports bit1 only (DMA), all other bits are unused, always 0.

**MMIO[8098h] - BM: PMU Breakpoint mask module(s) (bit1,8,9 only) (R/W)**

```

| 0 Unused (0)
 1 DMA
 2-7 Unused (0)
 8 Timer 0
 9 Timer 1
 10-15 Unused (0) |
| --- |

Unknown what this does. It looks similar to the "Recover" registers, but, going
by the description, it is "breakpoink masking" something instead of
"recovering".

**MMIO[80AAh] - 16bit R/W ?**

**MMIO[80ACh] - 16bit R/W ?**

**MMIO[80AEh] - 16bit R/W ?**

| DSi Teak MMIO[80C0h] - Host Port Interface (APBP aka HPI) |
| --- |

The APBP (aka HPI Host Port Interface) registers are used to communicate
between ARM and DSP.

**MMIO[80C0h/80C4h/80C8h] - APBP DSP-to-ARM Reply 0/1/2 (R/W)**

**MMIO[80C2h/80C6h/80CAh] - APBP ARM-to-DSP Command 0/1/2 (R)**

```

| 0-15 Command/Reply Data |
| --- |

**MMIO[80CCh] - APBP DSP-to-ARM Semaphore Set Flags (0=Clear, 1=Set) (R/W)**

**MMIO[80CEh] - APBP ARM-to-DSP Semaphore Interrupt Mask (?=On/Off) (R/W)**

**MMIO[80D0h] - APBP ARM-to-DSP Semaphore Ack Flags (0=No change, 1=Clear) (W?)**

**MMIO[80D2h] - APBP ARM-to-DSP Semaphore Get Flags (R)**

```

| 0-15 Semaphore Flag 0..15 |
| --- |

**MMIO[80D4h] - APBP Control (R/W)**

```

| 0-1 Unused (0)
 2 ARM-side register endianness (0=Normal, 1=Big-Endian/DANGER)
 3-7 Unused (0)
 8 Interrupt when CMD0 is written by ARM (0=Enable, 1=Disable)
 9-11 Unused (0)
 12 Interrupt when CMD1 is written by ARM (0=Enable, 1=Disable)
 13 Interrupt when CMD2 is written by ARM (0=Enable, 1=Disable)
 14-15 Unused (0) |
| --- |

**MMIO[80D6h] - APBP DSP-side Status (R)**

```

| 0-4 Unused? (usually 0)
 5 Reply Register 0 Read Flag (0=Was Read by ARM?, 1=No)
 6 Reply Register 1 Read Flag (0=Was Read by ARM?, 1=No)
 7 Reply Register 2 Read Flag (0=Was Read by ARM?, 1=No)
 8 Command Register 0 Update Flag (0=Was Written by ARM?, 1=No)
 9 Semaphore IRQ Flag (0=No, 1=[80D2h] AND NOT [80CEh])
 10-11 Unused? (usually 0)
 12 Command Register 1 Update Flag (0=Was Written by ARM?, 1=No)
 13 Command Register 2 Update Flag (0=Was Written by ARM?, 1=No)
 14-15 Unused? (usually 0) |
| --- |

**MMIO[80D8h] - APBP ARM-side Status (mirror of ARM9 Port 400430Ch) (R)**

```

| 0 Read Transfer Underway Flag (0=No, 1=Yes/From DSP Memory)
 1 Write Transfer Underway Flag (0=No, 1=Yes/To DSP Memory)
 2 Peripheral Reset Flag (0=No/Ready, 1=Reset/Busy)
 3-4 Unused (0)
 5 Read FIFO Full Flag (0=No, 1=Yes/Full)
 6 Read FIFO Not-Empty Flag (0=No, 1=Yes, ARM9 may read PDATA)
 7 Write FIFO Full Flag (0=No, 1=Yes/Full)
 8 Write FIFO Empty Flag (0=No, 1=Yes/Empty)
 9 Semaphore IRQ Flag (0=None, 1=IRQ)
 10 Reply Register 0 Update Flag (0=Was Written by DSP, 1=No)
 11 Reply Register 1 Update Flag (0=Was Written by DSP, 1=No)
 12 Reply Register 2 Update Flag (0=Was Written by DSP, 1=No)
 13 Command Register 0 Read Flag (0=Was Read by DSP, 1=No)
 14 Command Register 1 Read Flag (0=Was Read by DSP, 1=No)
 15 Command Register 2 Read Flag (0=Was Read by DSP, 1=No) |
| --- |

| DSi Teak MMIO[80E0h] - AHBM - Advanced High Performance Bus Master |
| --- |

AHBM does allow the Teak CPU to access the ARM-side memory (in combination with
DMA interface at MMIO[8180h]).

**Supported ARM-side memory areas**

DSi allows to access most of the ARM9 address space:

```

| 02000000h/Main RAM --> works
 03000000h/Shared RAM --> works (maybe also New Shared RAM, if any mapped?)
 04000000h/ARM9 I/O --> works
 05000000h/Palette --> works
 06000000h/VRAM --> works
 07000000h/OAM --> works
 08000000h/GBA SLOT ROM --> works (with dummy FFFFFFFFh values)
 0A000000h/GBA SLOT RAM --> works (with dummy FFFFFFFFh values)
 FFFF0000h/ARM9 BIOS --> works (lower 32K only, upper 32K is zerofilled)
 DTCM/ITCM --> probably ignored?
 Any other --> returns zero (but without MMIO[80E0h] error flag) |
| --- |

3DS in 3DS mode is more restrictive:

```

| 20000000h/FCRAM --> works
 1FF80000h/AXI --> works
 1FF40000h/DSP/DATA --> oddly mirrors to 1FF80000h/AXI
 Any other --> rejected (and sets MMIO[80E0h].bit4 error flag) |
| --- |

**MMIO[80E0h] - AHBM Status (R)**

```

| 0-1 Usually/always 0 ?
 2 Burst queue not empty (0=Empty, 1=Not-empty)
 3 Usually/always 0 ?
 4 Busy/stuck/error? (0=Normal, 1=Invalid ARM address) ;3DS only?
 5-15 Usually/always 0 ? |
| --- |

"Applications wait for all bits to be 0 before connecting AHBM to DMA."

**MMIO[80E2h+(0..2)*6] - AHBM Channel 0..2 Configure Burst/Data (R/W)**

```

| 0 Applications set this to 1 if BURST is non-zero, uh?
 1-2 Burst type (0=x1, 1=x4, 2=x8, 3=?)
 3 Unknown (R/W) (0=Normal, 1=Dunno/NoTransfer?)
 4-5 Data type (0=8bit, 1=16bit, 2=32bit, 3=?)
 6 Unused (0)
 7 Unknown (R/W) (0=Normal, 1=Dunno/TransferHangs/crashes?)
 8-11 Unknown (R/W) (0..Fh=?) (usually 0)
 12-15 Unused (0) |
| --- |

Used values: 0010h=?,0020h=?,0025h=dma?

**MMIO[80E4h+(0..2)*6] - AHBM Channel 0..2 Configure Whatever (R/W)**

```

| 0-7 Unknown (R/W) (0..FFh=?) (usually 0)
 8 Transfer direction (0=Read external memory, 1=Write external memory)
 9 Applications always set this (usually 1=set) (but also works when 0)
 10-15 Unused (0) |
| --- |

Used values: 0200h=read, 0300h=write

**MMIO[80E6h+(0..2)*6] - AHBM Channel 0..2 Configure DMA (R/W)**

```

| 0-7 Connect to DMA channel 0..7 (0=No, 1=Connect)
 8-15 Unused (0) |
| --- |

Used values: bit0-7=dma0..7, 0000h=All off.

**MMIO[80F4h] - Unknown 6bit? bit10-15 are used (R/W)**

**MMIO[80F6h] - AHBM Internal FIFO (R) and maybe also (W?)**

**MMIO[80F8h] - Unknown always zero? (R?)**

**MMIO[80FAh] - Read/write-able(!) mirror of MMIO[80FCh] (R/W)**

**MMIO[80FCh] - Unknown 16bit? (R/W)**

**MMIO[80FEh] - Unknown 16bit? (R/W)**

Unknown registers, normally left unused. The FIFO register seems to contain the
most recent 8x16bit values that were transferred via AHBM. Unknown if that FIFO
(and whatever address setting) could be used for manual (non-DMA) AHBM
transfers.

| DSi Teak MMIO[8100h] - Memory Interface Unit (MIU) |
| --- |

**MMIO[8100h] - MIU Waitstate Settings, bit0-15 (R/W)**

**MMIO[8102h] - MIU Waitstate Settings, bit16-31 (R/W)**

```

| 0-3 Number of wait-states for off-chip Z0 block
 4-7 Number of wait-states for off-chip Z1 block
 8-11 Number of wait-states for off-chip Z2 block
 12-15 Number of wait-states for off-chip Z3 block
 16-19 Number of wait-states for off-chip Z blocks outside Z0/Z1/Z2/Z3
 20-23 Number of wait-states for off-chip X/Y memory transactions
 24-27 Number of wait-states for off-chip Program-memory transactions
 28-31 Unused (0) |
| --- |

Uh, what means "off-chip" exactly, what kind of memory, on what "chip"?

**MMIO[8104h] - MIU Waitstate Area Z0 (R/W)**

**MMIO[8106h] - MIU Waitstate Area Z1 (R/W)**

**MMIO[8108h] - MIU Waitstate Area Z2 (R/W)**

**MMIO[810Ah] - MIU Waitstate Area Z3 (R/W)**

```

| 0-5 Wait-state block Start address in 1K-word units
 6-11 Wait-state block End address in 1K-word units
 12-15 Wait-state block Page (lower 4bit of page) |
| --- |

**MMIO[810Eh] - MIU X Page (or unused) (16bit) (R/W)**

**MMIO[8110h] - MIU Y Page (or unused) (8bit) (R/W)**

**MMIO[8112h] - MIU Z Page (or "absolute" page) (16bit) (R/W)**

The X/Z pages are 16bit wide:

```

| 0-15 Memory Page (...base or so, in WHAT-units?) |
| --- |

The Y page is only 8bit wide:

```

| 0-7 Memory Page (...base or so, in WHAT-units?)
 8-15 Unused (0) |
| --- |

The register meaning depends on the PGM paging mode:

```

| When PGM=0 --> MMIO[8112h] is the "absolute" data memory page
 When PGM=1 --> MMIO[810Eh/8110h/8112h] are the X/Y/Z pages |
| --- |

**MMIO[8114h] - MIU X/Y Page Size for Page 0 (or all pages) (R/W)**

**MMIO[8116h] - MIU X/Y Page Size for Page 1 (or unused) (R/W)**

**MMIO[8118h] - MIU X/Y Page Size for Off-chip Pages (or unused) (R/W)**

```

| 0-5 X memory size (0..3Fh)
 6-7 Unused (0)
 8-14 Y memory size (1..40h)
 15 Unused (0) |
| --- |

The register meaning depends on the PGM paging mode:

```

| When PGM=0?--> MMIO[8114h,8116h,8118h] used for Page 0, 1, and Off-chip pages
 When PGM=1?--> MMIO[8114h] used for all pages |
| --- |

For Y memory size, the hardware does automatically replace the written value by
min=01h or max=40h when trying to to write 00h or 41h-7Fh.

**MMIO[811Ah] - MIU Config for Misc stuff (R/W)**

```

| 0 Program protection mechanism, uh, what is that? (0=Disable, 1=Enable)
 1 Program page for entire program space (from Test pin) (1=offchip)
 2 Program page for breakpoint handler (0=Page1/offchip, 1=Page0/onchip)
 3 Unknown (R/W) (0..1=?)
 4 Core/DMA data use only Z-even address bus (single? PGM=0?) (1=Enable)
 5 Unknown (R/W) (0..1=?)
 6 Paging mode (PGM) (for X/Y/Z pages) (0=Normal, 1=DANGER)
 7-15 Unused (0) |
| --- |

Power-on default is 0014h. Bit1-2 are read-only (always bit1=0, bit2=1).

**MMIO[811Ch] - MIU Config for Program Page and Download Memory (R/W)**

```

| 0 Select Z-space data memory (0=Regular Memory, 1=Download memory)
 1 Download mem is/was selected (bit1 can be cleared during Trap only)
 2-5 Alternative Program Page for movd/movp opcodes (PDLPAGE) (4bit)
 6 Select program page for movd/movp (0=2bit/movpd, 1=4bit/PDLPAGE)
 7-15 Unused (0) |
| --- |

Bit1 gets set when setting bit0=1 (bit1 is usually readonly, except, trap
handler may clear bit1 by writing to it). Unknown what Download means, maybe
off-chip data memory?

**MMIO[811Eh] - MIU Base Address for MMIO Registers (R/W)**

```

| 0-9 Unused (0)
 10-15 MMIO Base Address (in 400h-word units) |
| --- |

Power-on default is 8000h, ie. mapping MMIO[8000h..87FFh] to 8000h..87FFh.

**MMIO[8120h] - MIU Observability Mode (R/W)**

```

| 0 Observability Enable (0=Disable, 1=Enable)
 1-3 Observability Mode (0..4=Mode, see below)
 4-15 Unused (0) |
| --- |

Observability modes (defines which Core/DMA address/data buses are reflected on
the off-chip XZ buses):

```

| 00h: Core XZ address/data buses
 01h: Core Y address/data buses
 02h: Core P address/data buses
 03h: DMA DST address/data buses
 04h: DMA SRC address/data buses |
| --- |

**MMIO[8122h] - MIU Pin Config? (R/W)**

```

| 0 Z Read Polarity Bit - for DRZON/DRZEN
 1 Z Write Polarity Bit - for DWZON/DWZEN
 2 Z Strobe Polarity Bit - for ZSTRB
 3 X Strobe Polarity Bit - for XSTRB
 4 X Select Polarity Bit - for XS
 5 Z Select Polarity Bit - for ZS
 6 Signal Polarity Bit - for RD_WR
 7-15 Unused (0) |
| --- |

| DSi Teak MMIO[8140h] - Code Replacement Unit (CRU) |
| --- |

The CRU is intended to fix bugs in ROM-based programs (rather useless for
RAM-based DSi code).

**MMIO[8140h+(0..15)*4] - CRU Entry 0..15 Offset, bit0-15 (R/W)**

**MMIO[8142h+(0..15)*4] - CRU Entry 0..15 Offset, bit16-31 (R/W)**

```

| 0-17 Program Address (0..3FFFFh) (R/W)
 18-21 Program Page (usually 0) (0..Fh) (R/W)
 22-30 Unused (0) (except bit25-30 in entry 15)
 25-28 Entry 15: Match entry number (0..15=Entry 0..15) (R)
 29 Entry 15: Match flag (cleared after read) (0=None, 1=Yes/match) (R)
 30 Entry 15: Master enable for all entries (0=Disable, 1=Enable) (R/W)
 31 Enable Entry (0=Disable, 1=Enable) (R/W) |
| --- |

There are sixteen identical entries, resembling OCEM program breaks, with some
extra control/status bits in last entry.

The feature is working in so far that the read-only status bits get set upon
matches, but the match should reportedly insert "a branch instruction" or
trigger a "trap" exception, which doesn't happen.

Unknown if there are further config/mode/enable bits elsewhere for activating
that behaviour. To bypass ROM code, it might redirect to data RAM, or external
AHBM memory?

| DSi Teak MMIO[8180h] - Direct Memory Access (DMA) |
| --- |

```

| _____________________________ DMA Control/Status _____________________________ |
| --- |

**MMIO[8184h] - DMA Channel Start Flags (0=Stop/Done, 1=Start/Busy) (R/W)**

**MMIO[8186h] - DMA Channel Pause Flags (0=Normal, 1=Pause) (R/W)**

```

| 0-7 Channel 0..7 flags
 8-15 Unused (0) |
| --- |

Transfer start/stop should be done via [81DEh].bit14/15 (using RMW to change
[8184h] directly could mess up other channels; if they finish during the RMW).

Channel 0 is automatically enabled on power up, because it is used for ARM-side
data transfers via DSP_PDATA (Port 4004300h).

**MMIO[8188h] - DMA Channel End Flags for Size0 (R)**

**MMIO[818Ah] - DMA Channel End Flags for Size1 (R)**

**MMIO[818Ch] - DMA Channel End Flags for Size2 (R) (all done)**

```

| 0-7 Channel 0..7 end flags (0=No, 1=End)
 8-15 Unused (0) |
| --- |

The COFF labels refer to these three registers as "seox", whatever that means.

The 3rd register can be used to check for transfer completion (transfer end
does also clear the Start/Enable bit in MMIO[8184h]).

**MMIO[818Eh] - DMA Whatever Slot Config, bit0-15 (R/W)**

**MMIO[8190h] - DMA Whatever Slot Config, bit16-31 (R/W)**

```

| 0-2 Whatever (0..7) (initially 0 on reset)
 4-6 Whatever (0..7) (initially 1 on reset)
 8-10 Whatever (0..7) (initially 2 on reset)
 12-14 Whatever (0..7) (initially 3 on reset)
 16-18 Whatever (0..7) (initially 4 on reset)
 20-22 Whatever (0..7) (initially 5 on reset)
 24-24 Whatever (0..7) (initially 6 on reset)
 28-30 Whatever (0..7) (initially 7 on reset)
 Bit3,7,11,15,19,23,25,31 are unused (always 0) |
| --- |

Reportedly "some sort of slots for channel 0..7" maybe priority for DMA 0..7?

transfer can hang (eg. when changing 1st word to 4444h or higher)?

```

| ________________________________ DMA Channels ________________________________ |
| --- |

**MMIO[81BEh] - DMA Select Channel (R/W)**

```

| 0-2 Select the channel to be mapped to MMIO[81C0h..81Exh] (0..7)
 3-15 Unused (0) |
| --- |

**MMIO[81C0h:0..7] - DMA Channel: Source Address, bit0-15 (R/W)**

**MMIO[81C2h:0..7] - DMA Channel: Source Address, bit16-31 (R/W)**

**MMIO[81C4h:0..7] - DMA Channel: Destination Address, bit0-15 (R/W)**

**MMIO[81C6h:0..7] - DMA Channel: Destination Address, bit16-31 (R/W)**

```

| 0-31 Address (within the selected memory area, see MMIO[81DAh]) |
| --- |

Unknown how/what works exactly...

DSP/data probably uses only lower 18bit & probably ignores X/Y/Z and
MMIOBASE

DSP/code probably uses only lower 18bit & probably ignores prpage/movpd

DSP/mmio probably uses only lower 11bit & probably ignores MMIOBASE

ARM/ahbm uses full 32bit address, counted in BYTE-units (not 16bit-units)

**MMIO[81C8h:0..7] - DMA Channel: Size0 (inner dimension) (R/W)**

**MMIO[81CAh:0..7] - DMA Channel: Size1 (middle dimension) (R/W)**

**MMIO[81CCh:0..7] - DMA Channel: Size2 (outer dimension) (R/W)**

```

| 0-15 Length (for each array dimension) (0001h..FFFFh) |
| --- |

Trying to write size=0000h is automatically replaced by size=0001h.

Total transfer length is Size0*Size1*Size2 (that is always counted in 16bit
units; Size0 should contain an even number when using 32bit mode).

Normal 1-dimensional transfer would use Size0=Len, Size1=Size2=1. Size1 can be
useful for 2-dimensional arrays. Size2 was announced to be useful for
re-ordering things like RGB values (uh, but that may require 8bit-addressing,
and negative step values, or separate step-direction flags?).

**MMIO[81CEh:0..7] - DMA Channel: Source Step0 (R/W)**

**MMIO[81D0h:0..7] - DMA Channel: Source Step1 (R/W)**

**MMIO[81D2h:0..7] - DMA Channel: Source Step2 (R/W)**

**MMIO[81D4h:0..7] - DMA Channel: Destination Step0 (R/W)**

**MMIO[81D6h:0..7] - DMA Channel: Destination Step1 (R/W)**

**MMIO[81D8h:0..7] - DMA Channel: Destination Step2 (R/W)**

```

| 0-15 Step (... in 8bit/16bit/32bit units?) (signed or unsigned?) |
| --- |

**MMIO[81DAh:0..7] - DMA Channel: Memory Area Config (R/W)**

```

| 0-3 Source Memory Area (0..0Fh, see below)
 4-7 Destination Memory Area (0..0Fh, see below)
 8 Unknown? (0=Normal, 1=No Irq, No end, maybe repeat?)
 9 Different Memory Areas (0=No/Slow, 1=Yes/Simultaneous Read+Write)
 10 Transfer Unit size (0=16bit/Slow, 1=32bit/Fast)
 11 Unused (0)
 12-13 Unknown? (0..3=?)
 12-15 Transfer Speed (0=Slow, 1/2=Medium, 3=Fast) (or burst size?) |
| --- |

Source/Destination Memory Areas can be:

```

| 00h DSP/Data memory ;\
 01h DSP/MMIO registers ; 16bit-address units
 05h DSP/Code memory (only for DST_SPACE) (untested) ;/
 07h ARM/AHBM external memory (via AHBM registers) ;-8bit-address units |
| --- |

Used values: 670h,607h,400h,250h. Reset value is F200h (and kept so for channel
0).

**MMIO[81DCh:0..7] - DMA Channel: Unknown, usually set to 0300h? (R/W)**

```

| 0-2 Unknown (R/W) (0..07h=?) (usually 0)
 3 Unused (0)
 4-7 Unknown (R/W) (0..0Fh=?) (usually 0h)
 8-9 Unknown (R/W) (0/1/2=Hangs?, 3=Normal)
 10-12 Unknown (R/W) (0..07h=?) (usually 0h)
 13-15 Unused (0) |
| --- |

Usually set to 0300h or 0700h (or, set to 0011h for DSP_PDATA transfer for
channel 0, though that does also work with reset value 0000h).

**MMIO[81DEh:0..7] - DMA Channel: Start/Stop/Control (R/W)**

```

| 0-2 Interrupt upon Size 0..2 End (0=Disable, 1=Enable) (R/W)
 3-5 Never set Size2 End flag? (0=Normal, 1=No end, maybe repeat?) (R/W)
 6-7 Unknown (0..3) (R/W)
 8-13 Unused? (0)
 14-15 Start/Stop Transfer (0=No change, 1=Start, 2=Stop, 3=Same as 1) (W) |
| --- |

```

| ______________________ DMA Unknown/Internal Registers ________________________ |
| --- |

**MMIO[8192h] - Unknown, one register with RW-mask 7C03h (R/W)**

Unknown/internal stuff. Writing 0000h, 0001h, 0002h, 0800h, 1000h, 4000h, FFFFh
passes okay, however, writing 0400h or 2000h causes TRAP exception. Note: Don't
remember how that had happened; it might have been caused indirectly by dvm
data matches.

**MMIO[81B6h..81BCh] - Unknown, four registers with RW-mask FFFFh (R/W)**

Unknown/internal stuff. The four 16bit R/W registers might be general purpose
fill values (if they have no other purpose)?

**MMIO[8180h] - DMA Internal: Channel Size0 Busy or so? (R)**

**MMIO[8182h] - DMA Internal: Channel Size1 Busy or so? (R)**

Unknown, can contain similar busy flags as in [8184h], although in read-only
form, and not exactly the same value (eg. bit0 tends to be zero despite of
channel being enabled for DSP_PATA transfers). Maybe related to Size0/1, or
maybe indicating which channel is RIGHT NOW transferring data (if so, only one
bit should be seen set at a time; or maybe bits go off one after another during
processing of multiple channels).

**MMIO[8194h] - DMA Internal: initially 0, contains SRC_ADDR_L after DMA (R)**

**MMIO[8196h] - DMA Internal: initially 0, contains DST_ADDR_L after DMA (R)**

**MMIO[8198h..81B4h] - Unknown, fixed 0 (R)**

**MMIO[81E0h:0..7] - DMA Internal: initially 0, but SRC_ADDR_L(n) after DMA (R)**

**MMIO[81E2h:0..7] - DMA Internal: initially 0, but DST_ADDR_L(n) after DMA (R)**

**MMIO[81E4h:0..7] - DMA Internal: initially 0, but SRC_ADDR_H(n) after DMA (R)**

**MMIO[81E6h:0..7] - DMA Internal: initially 0, but DST_ADDR_H(n) after DMA (R)**

**MMIO[81E8h..81FEh] - Unknown, fixed 0 (R)**

Unknown/internal stuff.

```

| _________________________________ DMA Notes _________________________________ |
| --- |

STEP0, STEP1 and STEP2 specify the address stepping between elements for each
dimension. Below is an example showing how it works.

With the source address configuration:

```

| SRC_ADDR = 0
 SIZE0 = 3
 SIZE1 = 5
 SIZE2 = 2
 SRC_STEP0 = 2
 SRC_STEP1 = 1
 SRC_STEP2 = 7 |
| --- |

The data transfer copies data from the following addresses:

```

| <--------------------------size1-------------------------->
 <--size0--> <--size0--> <--size0--> <--size0--> <--size0-->
 0, 2, 4, 5, 7, 9, 10, 12, 14, 15, 17, 19, 20, 22, 24 <-- size2 (1st)
 31, 33, 35, 36, 38, 40, 41, 43, 45, 46, 48, 50, 51, 53, 55 <-- size2 (2nd) |
| --- |

The 32bit double word mode also automatically aligns down the addresses to
32bit boundaries.

When the memory space is specified as 7 (AHBM), it performs data transfer
from/to external ARM memory.

The external memory has 8-bit addressing. Keep in mind that the STEP value is
also added to the address as-is, so STEP = 1 means stepping 8-bit for FCRAM,
while stepping 16-bit for DSP memory.

There are also more alignment requirement on the external memory address, but
it is handled by AHBM.

| DSi Teak MMIO[8200h] - Interrupt Control Unit (ICU) |
| --- |

**MMIO[8200h] - ICU Interrupt Pending Flags (0=No, 1=IRQ Pending) (R)**

**MMIO[8202h] - ICU Interrupt Acknowledge (0=No change, 1=Set) (W)**

**MMIO[8204h] - ICU Interrupt Manual Trigger (0=Release, 1=Set) (R/W)**

**MMIO[8206h] - ICU Enable Interrupt routing to core interrupt 0 (R/W)**

**MMIO[8208h] - ICU Enable Interrupt routing to core interrupt 1 (R/W)**

**MMIO[820Ah] - ICU Enable Interrupt routing to core interrupt 2 (R/W)**

**MMIO[820Ch] - ICU Enable Interrupt routing to vectored interrupt (R/W)**

**MMIO[820Eh] - ICU Interrupt Trigger mode (0=Level, 1=Edge) (R/W)**

**MMIO[8210h] - ICU Interrupt Polarity (0=Normal, 1=Invert) (R/W)**

```

| 0-8 No hardware IRQs (but can be used as Software IRQs via Manual Trigger)
 9 Timer 1
 10 Timer 0
 11 BTDMP 0
 12 BTDMP 1
 13 SIO
 14 APBP
 15 DMA |
| --- |

**MMIO[8212h+(0..15)*4] - ICU Vectored Interrupt 0..15 Address, bit16-31 (R/W)**

**MMIO[8214h+(0..15)*4] - ICU Vectored Interrupt 0..15 Address, bit0-15 (R/W)**

```

| 0-17 Address of interrupt handler for vectored interrupt 0..15
 18-30 Unused (0)
 31 Context switch on vectored interrupt 0..15 (0=Disable, 1=Enable) |
| --- |

Observe that upper/lower 16bit are arranged in BIG-ENDIAN style (unlike other
registers with lower/upper 16bit).

Default values on reset are 0003FC00h.

Interrupt vector address for Interrupt 0..15, when using "vectored interrupt".

The core interrupts and NMI have fixed vector addresses.

**MMIO[8252h] - ICU Interrupt Master Disable (0=Normal, 1=Off/undoc) (R/W)**

```

| 0-15 Master Disable for interrupt 0..15 (0=Normal, 1=Off, don't set pending) |
| --- |

**MMIO[8254h] - Unknown, R/W mask 5555h (R/W)**

**MMIO[8256h] - Unknown, R/W mask 5555h (R/W)**

Unknown.

**Interrupt related CPU registers**

DSi Teak CPU Control/Status Registers

**teak exception vectors**

```

| code:00000h start (from reset or timer watchdog)
 code:00002h trap_handler (trap/break) (from OCEM or Timers)
 code:00004h nmi_handler (non-maskable interrupt) (from timer watchdog)
 code:00006h int0_handler
 code:0000Eh int1_handler
 code:00016h int2_handler
 code:variable vint_handler(s) (with context switch, instead of push/pop?) |
| --- |

**Interrupt Behaviour**

Pending flags are set somewhat as so:

```

| new_state=incoming_hw_signal ;always 0 for interrupt 0..8
 if polarity=1 then new_state=new_state xor 1
 new_state=new_state OR manual_trigger ;done AFTER above polarity invert
 new_state=new_state AND NOT master_disable
 if new_state=1 and old_state=0 then pending=1
 old_state=new_state |
| --- |

Acknowlege is required for clearing pending bits, this is required in both
Egde- and Level-triggered modes (the difference is that Level-triggered mode
will ignore the acknowledge if new_state is still on).

Manual Trigger won't appear in Pending until after two NOP opcodes (or similar
delay).

**IRQ-to-interrupt translator**

The main job of ICU is to translate 16 IRQ signals to 4 processor interrupt
signals (int0, int1, int2 and vint), specified by I0x, I1x, I2x and IVx
registers. When an IRQ is signaled, ICU will generate interrupt signal and the
processor starts to execute interrupt handling procedure if the core interrupt
is enabled. The procedure is supposed check IPx reigster to know the exact IRQ
index, and to set IAx registers to clear the IRQ signal.

**Software interrupt**

The processor can writes to ITx to generate a software interrupt manually. A
use case for this in many 3DS games is that IRQ 0 is used as the reschedule
procedure for multithreading. When a thread wants to yield, it triggers IRQ 0,
which switches thread context and resume another thread.

| DSi Teak MMIO[8280h] - Audio (Buffered Time Division Multiplexing Port) |
| --- |

There are two BTDMP units. The weird name "Buffered Time Division Multiplexing
Port" does essentially mean "FIFO with sample rate timer".

```

| BTDMP 0 is used for Receive (microphone) and Transmit (audio out).
 BTDMP 1 isn't actually used for anything. |
| --- |

**MMIO[8280h/82A0h+(0..1)*80h] - BTDMP Receive/Transmit Control (R/W)**

```

| 0-3 Unknown (0..Fh) (usually 0Dh/0Fh)
 4-7 Unknown (0..Fh) (usually 00h)
 8-11 Enable BTDMP Interrupt when non-zero (0=Off, AnyOther=On?)
 12-15 Unknown (0..Fh) (usually 00h) |
| --- |

DSi sets this to 020Dh (receive) or 010Fh (transmit).

**MMIO[8282h/82A2h+(0..1)*80h] - BTDMP Receive/Transmit Period (R/W)**

```

| 0-2 Clock Divider? (0..7) (usually 4) (affects timing when bit13=1)
 3-4 Unused (0)
 5-12 Clock Divider? (0..FFh) (usually 80h) (affects timing when bit13=1)
 13 Clock Select (0=ExternalDSiAudioClk, 1=InternalClkDivider?)
 14 Unknown (0..1) (usually 0)
 15 Unused (0) |
| --- |

Usually set to 1004h, which does select ExternalClk (32.73kHz or 47.61kHz, as
selected in SNDEXCNT on ARM side; per 2x16bit stereo data).

Unknown how InternalClk works exactly. Increasing the "dividers" doesn't
consistently increase FIFO transfer time; it seems as if the max time gets
truncated somehow; maybe that's caused by InternalClk being ANDed with still
running ExternalClk.

**MMIO[8284h/82A4h+(0..1)*80h] - BTDMP Receive/Transmit Usually 0004h (R/W)**

```

| 0-2 Unknown (0..7) (R/W) (R/W)
 3-4 Unused? (0)
 5-11 Unknown (0..7Fh) (R/W) (R/W)
 12-15 Unused? (0) |
| --- |

Hmmmm, the write-ability of bit0-11 resembles the Period register...?

**MMIO[8286h/82A6h+(0..1)*80h] - BTDMP Receive/Transmit Usually 0021h (R/W)**

```

| 0-1 Unknown (0..3) (R/W) (R/W)
 2-4 Unused? (0)
 5 Unknown/writeonly? (read=0) (code writes 1 here) (W?)
 6-15 Unused? (0) |
| --- |

**MMIO[8288h/82A8h+(0..1)*80h] - BTDMP Receive/Transmit Usually 0000h'a (R/W)**

```

| 0-12 Unknown (0..1FFFh) (R/W) (initially=1FFFh on reset)
 13-15 Unused (0) |
| --- |

**MMIO[828Ah/82AAh+(0..1)*80h] - BTDMP Receive/Transmit Usually 0000h'b (R/W)**

```

| 0-11 Unknown (0..0FFFh) (R/W)
 12-15 Unused (0) |
| --- |

**MMIO[828Ch/82ACh+(0..1)*80h] - BTDMP Receive/Transmit Usually 0000h'c (R/W)**

```

| 0-13 Unknown (0..3FFFh) (R/W)
 14-15 Unused (0) |
| --- |

**MMIO[828Eh/82AEh+(0..1)*80h] - BTDMP Receive/Transmit Usually unused'a (R/W)**

**MMIO[8290h/82B0h+(0..1)*80h] - BTDMP Receive/Transmit Usually unused'b (R/W)**

```

| 0-15 Unknown (0..FFFFh) (R/W) |
| --- |

Some of the above unknown stuff might be... volume, bias, sample.size,
mono/stereo, dummy-value-when-fifo-empty, beep generator, filter/equalizer,
sample rate interpolation, whatever...?

**MMIO[829Eh/82BEh+(0..1)*80h] - BTDMP Receive/Transmit Enable (R/W)**

```

| 0-14 Unused (0)
 15 Enable Transfer (0=Off, 1=On, allow Transfer+IRQ's) |
| --- |

```

| ________________________________ BTDMP FIFOs _________________________________ |
| --- |

**MMIO[82C0h/82C2h+(0..1)*80h] - BTDMP Receive/Transmit FIFO Status (R)**

```

| 0 usually 0
 1 If transfer ENABLED usually set, sometimes 0
 2 If transfer ENABLED usually set
 3 FIFO Full (0=No, 1=Full, 16x16bit words)
 4 FIFO Empty (0=No, 1=Empty, 0x16bit words)
 5 If transfer ENABLED usually set
 6 usually 0
 7 For TX: gets set when FIFO contains ONE word?
 8-15 usually 0 |
| --- |

**MMIO[82C4h/82C6h+(0..1)*80h] - BTDMP Receive/Transmit FIFO Data (R) and (W)**

```

| 0-15 Signed 16bit audio sample. |
| --- |

FIFO size is 16x16bit. The data is stereo, ie. one needs to write 2x16bit for a
full stereo sample (the write-order for left/right is still unknown).

Software seems to be directly writing to the FIFO (rather than using DMA). If
DMA is also supported, that would probably require whatever special settings in
the BTDMP and/or DMA registers (ie. so that the DMA knows when it needs to
transfer new FIFO data).

**MMIO[82C8h/82CAh+(0..1)*80h] - BTDMP Receive/Transmit FIFO Control (R/W)**

```

| 0-1 Unknown (0..3=?) (R/W)
 2 Flush FIFO (0=No change, 1=Clear FIFO) (W)
 3-15 Unused (0) |
| --- |

| DSi Teak CPU Registers |
| --- |

**36bit/40bit Accumulators: a0,a1,b0,b1**

```

| a0e:a0h:a0l (4:16:16 bits) = a0 (36bit) ;TL2: 40bit (8:16:16)
 a1e:a1h:a1l (4:16:16 bits) = a1 (36bit) ;TL2: 40bit (8:16:16)
 b0e:b0h:b0l (4:16:16 bits) = b0 (36bit) ;TL2: 40bit (8:16:16)
 b1e:b1h:b1l (4:16:16 bits) = b1 (36bit) ;TL2: 40bit (8:16:16) |
| --- |

4bit snippets (bit32-35) of a0/a1 can be found in status registers (st0,st1).
On TL2, the whole upper 8bit (bit32-39) of a0/a1/b0/b1 can be additionally
accessed via push/pop (a0e,a1e,b0e,b1e).

**Teak General Registers: r0,r1,r2,r3,r4,r5,r6,r7**

```

| r0 ;TL ;16bit ;\
 r1 ;TL ;16bit ;
 r2 ;TL ;16bit ; old TL1 registers
 r3 ;TL ;16bit ;
 r4 ;TL ;16bit ;
 r5 ;TL ;16bit ;/
 r6 ;TL2 ;16bit ;<-- new TL2 register
 r7 ;TL ;16bit ;<-- aka rb (with optional immediate, MemR7Imm) |
| --- |

**32bit Multiply Result and 16bit Muliply Parameters: p0,p1, and x0,y0,x1,y1**

```

| x0 ;TL ;16bit ;-
 y0 ;TL ;16bit ;-
 x1 ;TL2 ;16bit ;-
 y1 ;TL2 ;16bit ;-
 p0 ;TL ;33bit! ;\Px ;TL2: 33bit p0e:p0 ? ;TL1: 32bit?
 p1 ;TL2 ;33bit! ;/ ;TL2: 33bit p1e:p1 ? ;TL1: N/A
 p0h ;TL ;16bit ; ;<-- aka ph ;<-- called "p0" (aka "p") in "RegisterP0" |
| --- |

The "load ps" and "load ps01" opcodes allow to specify a multiply shifter, this
is useful when dealing with signed/unsigned parameters:

```

| Unsigned = Unsigned * Unsigned ;use shift 0
 Unsigned = Unsigned * Signed ;use shift +1
 Unsigned = Signed * Signed ;use shift +2
 Signed = Unsigned * Unsigned ;use shift -1
 Signed = Unsigned * Signed ;use shift 0
 Signed = Signed * Signed ;use shift +1 |
| --- |

**TeakLite Misc**

```

| pc ;TL ;18bit! ;-program counter (TL2: 18bit, TL1: 16bit)
 sp ;TL ;16bit ;-stack pointer (decreasing on push/call)
 sv ;TL ;16bit ;-shift value (negative=right) (for shift-by-register)
 mixp ;TL ;16bit ;-related to min/max/mind/maxd
 lc ;TL ;16bit ;-Loop Counter (of block repeat)
 repc ;TL ;16bit ;-Repeat Counter (for "rep" opcode)
 dvm ;TL ;16bit ;-Data Value Match (OCEM data breakpoints) (and for trap) |
| --- |

**TeakLiteII Misc: vtr0,vtr1,prpage**

```

| vtr0 ;TL2 16bit ;\related to vtrshr,vtrmov,vtrclr
 vtr1 ;TL2 16bit ;/(saved C/C1 carry flags for Viterby decoding)
 prpage ;TL2 4bit ;-??? (bit0-3 used/dangerous, bit4-15 always 0) |
| --- |

vtr0,vtr1 are related to vtrshr,vtrmov,vtrclr opcodes (and multi-function
opcodes with "vtrshr" suffix).

prpage isn't used by existing DSi code, setting the four write-able bits to
nonzero seems to screw-up opcode fetching, causing code to crash (unless one of
the next 1-2 prefetched opcodes restores prpage=0, which causes opcode fetching
to recover; after skipping some following prefetched opcodes, until prpage=0 is
applied). Maybe it's related to code access rights or waitstates... it doesn't
seem to be related to upper 2bit of the 18bit program counter (prpage is zero
even when executing code above address 0FFFFh).

**Old Control/Status registers (TeakLite): st0,st1,st2,icr**

**New Control/Status registers (TeakLiteII): stt0,stt1,stt2,mod0,mod1,mod2,mod3**

DSi Teak CPU Control/Status Registers

**Address Config (TeakLiteII): ar0,ar1,arp0,arp1,arp2,arp3**

**Address Step/Modulo: cfgi,cfgj (and TL2 stepi0,stepj0)**

DSi Teak CPU Address Config/Step/Modulo

**User-defined registers (optional off-core): ext0,ext1,ext2,ext3**

The four ext registers are intended for custom hardware extensions (where they
could be used as I/O ports, with faster & more direct access than memory
mapped I/O).

```

| ext0 ;TL ;16bit
 ext1 ;TL ;16bit
 ext2 ;TL ;16bit
 ext3 ;TL ;16bit |
| --- |

In the DSi, the four register do exist (they are fully read/write-able), but
unknown if they do have any special functions - or if they are just
general-purpose data registers (existing DSi software isn't using the ext
registers, and hardware is solely accessed via memory mapped I/O).

**Bitfields for Control/Status registers and cfgi/cfgj registers**

```

| page ;TL ;8bit "load" st1.bit0-7 (page for MemImm8) ;aka "lpg"
 ps ;TL ;2bit "load" st1.bit10-11 (product shifter for multiply?)
 ps01 ;TL2 ;4bit "load" mod0...? (maybe separate "ps" for p0 and p1 ?)
 movpd ;TL2 ;2bit "load" stt2.bit6-7 (page for reading DATA from ProgMem)
 modi ;TL ;9bit "load" cfgi.bit7-15 =imm9
 modj ;TL ;9bit "load" cfgj.bit7-15 =imm9
 stepi ;TL ;7bit "load" cfgi.bit0-6 =imm7
 stepj ;TL ;7bit "load" cfgj.bit0-6 =imm7 |
| --- |

**Shadow Registers**

Some registers (or in case of st0-st2: fractions thereof) exist as "shadows"...
related to "cntx", "swap", "banke" (and maybe "bankr"?) opcodes, and
"reti/retid" opcodes with "context" suffix, and interrupts with context
switching enabled.

```

| st0 bit0,2-11 ;\control/status (cntx)
 st1 bit10-11 (and "swap": bit0-7) ; (TL2: probably also SttMod)
 st2 bit0-7 ;/
 a0 <--> b0 manualswap only? ;\accumulators (swap)
 a1 <--> b1 autoswapped? ;/
 r0 <--> r0b ;\
 r1 <--> r1b ;
 r4 <--> r4b ; BankFlags (banke)
 r7 <--> r7b ;TL2 ;
 cfgi <--> cfgib ;
 cfgj <--> cfgjb ;TL2 ;/
 Ar,Arp <--> ? ;TL2 ;-? (bankr and/or cntx) |
| --- |

Registers b0/b1 can be used as normal opcode operands, the other shadow
registers are used only when doing bank/cntx stuff.

**Suffix codes: dmod,dmodi,dmodj,dmodij,context,eu,dbrv,ebrv,s,r**

Non-register assembler keywords.

```

| dmod ;TL ;suffix ;\
 dmodi ;TL2 ;suffix ;
 dmodj ;TL2 ;suffix ;
 dmodij ;TL2 ;suffix ;/
 context;TL ;suffix ;<-- (related to "cntx")
 eu ;TL ;suffix ;<-- (aka "Axheu", now "Axh,eu")
 dbrv ;TL2 ;suffix ;\for "bitrev"
 ebrv ;TL2 ;suffix ;/
 s ;TL ;suffix ;\param for "cntx" opcode ;"s" also for opcode 88D1h
 r ;TL ;suffix ;/ |
| --- |

**Condition codes: true,eq,neq,gt,ge,lt,le,nn,c,v,e,l,nr,niu0,iu0,iu1**

The 16 condition codes can be used for all opcodes with "Cond" operand, whereas
"true" can be omitted (as it means always/non-conditional), the four conditions
"gt,ge,lt,le" can be also used with "min/max/maxd/cbs" opcodes.

**Old pre-TeakLiteII keyword names (renamed in TeakLiteII)**

```

| TL: x y p ph rb lpg a0heu a1heu
 TL2: x0 y0 p0 p0h r7 page a0h,eu a1h,eu |
| --- |

| DSi Teak CPU Control/Status Registers |
| --- |

There are two sets of Control/Status registers

```

| Old registers (for TeakLite): st0/st1/st2, and icr
 New registers (for TeakLiteII): stt0/stt1/stt2, and mod0/mod1/mod2/mod3 |
| --- |

The new registers do contain only a few new bits, apart from that they are
basically same as the old registers (with the old bits rearranged to different
locations in the new registers).

The old registers do still exist on TeakLiteII, so one could use either old or
new registers (all reads/writes will be mirrored to both register sets).

However, there are a few cases where writing the old registers may smash bits
in new registers (writing the old "limit" bit will change BOTH of the new
"limit" bits, changing "ps" versus "ps01" may also involve strange effects,
and... changing "a0e/a1e" bits seems to have "weird" effects on
a0l/a0h/a1l/a1h, or maybe that's some "wanted" saturation effect?).

**CPU Flags (for Cond opcodes)**

CPU Flags are stored in st0 register (with mirrors in stt0/stt1). The flags can
be used for conditional opcodes (with "Cond" operand). According to TeakLite
datasheet, the flags are affected somewhat like this:

```

| ZMNVCEL- add, addh, addl, cmp, cmpu, sub, subh, subl, inc, dec, neg
 ZMNVCEL- maa, maasu, mac, macsu, macus, macuu, msu, sqra, rnd, pacr, movr
 ZMN-C--- or
 ZM--C--- addv, cmpv, subv, and
 ZMN--E-- clr, clrr, copy, divs, swap, not, xor
 ZMN--0L- lim
 ZMNVCELR norm
 ZMN-CE-- rol, ror
 ZMN-CE-- movs, movsi, shfc, shfi, shl, shl4, shr, shr4 ;for logical shift
 ZMNVCEL- movs, movsi, shfc, shfi, shl, shl4, shr, shr4 ;for arithmetic shift
 ZMN--E-- mov, movp, pop ;when dst=ac,bc (whut?) ;\
 xxxxxxxx mov, movp, pop ;when dst=st0 ; mov etc.
 ------L- mov, push ;when src=aXL,aXH,bXL,bXH ;
 -------- mov, movp, pop, push ;when src/dst neither of above ;/
 ZMN--E-- cntx s ;store shadows (new flags for a1) ;\cntx
 ZMNVCELR cntx r ;restore shadows (old flags) ;/
 ZM------ set, rst, chng
 Z------- tst0, tst1, tstb
 -M------ max, maxd, min
 -------R modr
 -------- mpy, mpyi, mpysu, sqr, exp
 -------- banke, dint, eint, load, nop, bkrep, rep, break, trap, movd
 -------- br, brr, call, calla, callr, ret, retd, reti, retid, rets |
| --- |

Flags for new TL2 opcodes aren't officially documented; some might follow the
above rules (eg. the new "r6" register should act as old "r0-r5"), but other
new opcodes might do this or that.

```

| __________________________ Old registers (TeakLite) __________________________ |
| --- |

**st0 - Old TL1 Status/Control Register st0**

```

| 0 SAT R/W Saturation Mode (0=Off, 1=Saturate "Ax to data") ;mod0.0
 1 IE R/W Interrupt Enable (0=Disable, 1=Enable) ;dint/eint ;mod3.7
 2 IM0 R/W Interrupt INT0 Mask (0=Disable, 1=Enable if IE=1) ;mod3.8
 3 IM1 R/W Interrupt INT1 Mask (0=Disable, 1=Enable if IE=1) ;mod3.9
 4 R R/W Flag: rN is Zero ;see Cond nr ;stt1.4
 5 L R/W Flag: Limit ;see Cond l ;L=(LM or VL) ;stt0.0+1
 6 E R/W Flag: Extension ;see Cond e ;stt0.2
 7 C R/W Flag: Carry ;see Cond c ;stt0.3
 8 V R/W Flag: Overflow ;see Cond v ;stt0.4
 9 N R/W Flag: Normalized ;see Cond nn ;stt0.5
 10 M R/W Flag: Minus ;see Cond gt,ge,lt,le ;stt0.6
 11 Z R/W Flag: Zero ;see Cond eq,neq,gt,le ;stt0.7
 12-15 a0e R/W Accumulator 0 Extension Bits ;a0.32-35 |
| --- |

**st1 - Old TL1 Status/Control Register st1**

```

| 0-7 PAGE R/W Data Memory Page (for MemImm8) (see "load page") ;mod1.0-7
 8-9 - - Reserved (read: always set) ;-
 10-11 PS R/W Product Shifter for P0 (see "load ps")(multiply?) ;mod0.10-11
 (0=No Shift, 1=SHR1, 2=SHL1, 3=SHL2)
 12-15 a1e R/W Accumulator 1 Extension Bits ;a1.32-35 |
| --- |

**st2 - Old TL1 Status/Control Register st2**

```

| 0-3 MDn R/W Enable cfgi.modi modulo for R0..R3 (0=Off, 1=On) ;mod2.0-3
 4-5 MDn R/W Enable cfgj.modj modulo for R4..R5 (0=Off, 1=On) ;mod2.4-5
 6 IM2 R/W Interrupt INT2 Mask (0=Disable, 1=Enable if IE=1) ;mod3.10
 7 S R/W Shift Mode (0=Arithmetic, 1=Logic) ;mod0.7
 8 OU0 R/W OUSER0 User Output Pin ;mod0.8
 9 OU1 R/W OUSER1 User Output Pin ;mod0.9
 10 IU0 R IUSER0 User Input Pin (zero) ;see Cond iu0,niu0 ;stt1.??
 11 IU1 R IUSER1 User Input Pin (zero) ;see Cond iu1 ;stt1.??
 12 - - Reserved (read: always set) ;-
 13 IP2 R Interrupt Pending INT2 (0=No, 1=IRQ) ;stt2.2
 14 IP0 R Interrupt Pending INT0 (0=No, 1=IRQ) ;stt2.0
 15 IP1 R Interrupt Pending INT1 (0=No, 1=IRQ) ;stt2.1 |
| --- |

**icr - Old TL1 Interrupt Context and Repeat Nesting**

```

| 0 NMIC R/W NMI Context switching enable (0=Off, 1=On) ;mod3.0
 1 IC0 R/W INT0 Context switching enable (0=Off, 1=On) ;mod3.1
 2 IC1 R/W INT1 Context switching enable (0=Off, 1=On) ;mod3.2
 3 IC2 R/W INT2 Context switching enable (0=Off, 1=On) ;mod3.3
 4 LP R InLoop (when inside one or more "bkrep" loops) ;stt2.15
 5-7 BCn R Block repeat nest. counter ;see "bkrep" ;stt2.12-14
 8-15 - - Reserved (read: always set) ;- |
| --- |

```

| _________________________ New registers (TeakLiteII) _________________________ |
| --- |

**stt0 - New TL2 Status/Control Register stt0 (CPU Flags)**

```

| 0 LM R/W Flag: Limit, set if saturation has/had occured ;st0.5
 1 VL R/W Flag: LatchedV, set if overflow has/had occurred ;st0.5, too
 2 E R/W Flag: Extension ;see Cond e ;st0.6
 3 C R/W Flag: Carry ;see Cond c ;st0.7
 4 V R/W Flag: Overflow ;see Cond v ;st0.8
 5 N R/W Flag: Normalized ;see Cond nn ;st0.9
 6 M R/W Flag: Minus ;see Cond gt,ge,lt,le ;st0.10
 7 Z R/W Flag: Zero ;see Cond eq,neq,gt,le ;st0.11
 8-10 - - Unknown (reads as zero)
 11 C1 R/W Flag: Carry1 (2nd carry, for dual-operation opcodes)
 12-15 - - Unknown (reads as zero) |
| --- |

**stt1 - New TL2 Status/Control Register stt1 (whatever)**

```

| 0-3 - - Unknown (reads as zero)
 4 R R/W Flag: rN is Zero ;see Cond nr ;st0.4
 5-13 - - Unknown (reads as zero) (IU1 and IU0 should be here!)
 14 P0E R/W Upper bit of 33bit P0 register ;\shifted-in on ;p0.32
 15 P1E R/W Upper bit of 33bit P1 register ;/arith right shifts ;p1.32 |
| --- |

Note: bit14/bit15 are automatically sign-expanded when moving data to
p0/p0h/p1.

**stt2 - New TL2 Status/Control Register stt2 (Interrupt/ProgBank/Bkrep)**

```

| 0 IP0 R Interrupt Pending INT0 (0=No, 1=IRQ) ;st2.14
 1 IP1 R Interrupt Pending INT1 (0=No, 1=IRQ) ;st2.15
 2 IP2 R Interrupt Pending INT2 (0=No, 1=IRQ) ;st2.13
 3 IPV R Interrupt Pending VINT ;-
 4-5 - - Unknown (reads as zero) ;-
 6-7 PCMhi R/W Program Memory Bank (for ProgMemRn/ProgMemAxl) ("load movpd")
 8-11 - - Unknown (reads as zero) ;-
 12-14 BCn R Block repeat nest. counter ;see "bkrep" ;icr.5-7
 15 LP R InLoop (when inside one or more "bkrep" loops) ;icr.4 |
| --- |

**mod0 - New TL2 Status/Control Register mod0 (Misc)**

```

| 0 SAT R/W Saturation Mode (0=Off, 1=Saturate "Ax to data"?) ;st0.0
 1 SATA R/W Saturation Mode on store (0=Off, 1="(Ax op data) to Ax"?)
 2 ? R Unknown (reads as one)
 3 - - Unknown (reads as zero)
 4 - - Unknown (reads as zero)
 5-6 HWM R/W Halfword Multiply ... Modify y0 (and y1?)
 0=read y0/y1 directly (full 16bit words)
 1=Takes y0>>8 and y1>>8 (logic shift)
 2=Takes y0&0xFF and y1&0xFF
 3=Takes y0>>8 and y1&&0xFF
 7 S R/W Shift Mode (0=Arithmetic, 1=Logic) ;st2.7
 8 OU0 R/W OUSER0 User Output Pin ;st2.8
 9 OU1 R/W OUSER1 User Output Pin ;st2.9
 10-11 PS0 R/W Product Shifter for P0 (see "load ps")(multiply?) ;st1.10-11
 12 - - Unknown (reads as zero)
 13-14 PS1 R/W Product Shifter for P1 (see "load ps")(multiply?)
 15 - - Unknown (reads as zero) |
| --- |

**mod1 - New TL2 Status/Control Register mod1 (Data Page)**

```

| 0-7 PAGE R/W Data Memory Page (for MemImm8) (see "load page") ;st1.0-7
 8-11 - - Unknown (reads as zero)
 12 STP16 R/W banke opcode (0=exchange cfgi/cfgj, 1=cfgi/cfgj+stepi0/stepj0)
 1=use stepi0/j0 instead of stepi/j for stepping Rn registers
 13 CMD R/W Change Modulo mode (0=New TL2 style, 1=TL1 style)
 14 EPI R/W Unknown (1=Set R3=0 after any "modr R3" or "access[R3]"?)
 15 EPJ R/W Unknown (1=Set R7=0 after any "modr R7" or "access[R7]"?) |
| --- |

**mod2 - New TL2 Status/Control Register mod2 (Modulo Enable)**

```

| 0-3 MDn R/W Enable cfgi.modi modulo for R0..R3 (0=Off, 1=On) ;st2.0-3
 4-5 MDn R/W Enable cfgj.modj modulo for R4..R5 (0=Off, 1=On) ;st2.4-5
 6-7 MDn R/W Enable cfgj.modj modulo for R6..R7 (0=Off, 1=On) ;TL2 only
 8-11 BRn R/W Step +s for R0..R3 (0=cfgi.stepi, 1=stepi0)
 12-15 BRn R/W Step +s for R4..R7 (0=cfgj.stepi, 1=stepj0) |
| --- |

XXX... bit8-9 seem to mess up my code (that uses r0/r1, but only with +0 step).

"When BRn=1, memory access through Rn will use the bit-reversed value of Rn as
the address. Note that this also implies that stepi0/j0 will be used,
regardless of what STP16 says."

**mod3 - New TL2 Status/Control Register mod3 (Interrupt Control)**

```

| 0 NMIC R/W NMI Context switching enable (0=Off, 1=On) ;icr.0
 1 IC0 R/W INT0 Context switching enable (0=Off, 1=On) ;icr.1
 2 IC1 R/W INT1 Context switching enable (0=Off, 1=On) ;icr.2
 3 IC2 R/W INT2 Context switching enable (0=Off, 1=On) ;icr.3
 4 OU2 R/W Unknown (R/W)
 5 OU3 R/W Unknown (R/W)
 6 OU4 ? ---DANGER BIT--- (1=hangs/crashes when set)
 7 IE R/W Interrupt Enable (0=Disable, 1=Enable) ;dint/eint ;st0.1
 8 IM0 R/W Interrupt INT0 Mask (0=Disable, 1=Enable if IE=1) ;st0.2
 9 IM1 R/W Interrupt INT1 Mask (0=Disable, 1=Enable if IE=1) ;st0.3
 10 IM2 R/W Interrupt INT2 Mask (0=Disable, 1=Enable if IE=1) ;st2.6
 11 IMV R/W Interrupt VINT Mask (0=Disable, 1=Enable if IE=1?)
 12 - - Unknown (reads as zero)
 13 CCNTA R/W Unknown (R/W)
 14 CPC R/W Stack word order for PC on call/ret (0=Normal, 1=Reversed)
 15 CREP R/W Unknown (R/W) |
| --- |

Bit14=0: push lowword then push highword on call; pop highword then pop lowword
on ret.

| DSi Teak CPU Address Config/Step/Modulo |
| --- |

```

| _______________________________ Address Config _______________________________ |
| --- |

These registers allow to reconfigure opcodes. For example, opcodes with operand
MemR45 can normally use only R4/R5 operands; if desired that can be changed to
anything else like R3/R0. The selection if Offset/Step operands can be changed
in similar way.

```

| Unknown which settings affect which opcodes exactly. |
| --- |

The DSi Sound code is using a default configuration for most of its code, but
it does also have some functions with alternate configurations.

**ar0/ar1**

```

| 0-2 R/W PM1/PM3 Post Modify Step (0..7 = +0,+1,-1,+s,+2,-2,+2,-2)
 3-4 R/W CS1/CS3 Offset (0..3 = +0,+1,-1,-1)
 5-7 R/W PM0/PM2 Post Modify Step (0..7 = +0,+1,-1,+s,+2,-2,+2,-2)
 8-9 R/W CS0/CS2 Offset (0..3 = +0,+1,-1,-1)
 10-12 R/W RN1/RN3 Register (0..7 = R0..R7)
 13-15 R/W RN0/RN2 Register (0..7 = R0..R7) |
| --- |

**arp0/arp1/arp2/arp3**

```

| 0-2 R/W PIn Post Modify Step I (0..7 = +0,+1,-1,+s,+2,-2,+2,-2)
 3-4 R/W CIn Offset I (0..3 = +0,+1,-1,-1)
 5-7 R/W PJn Post Modify Step J (0..7 = +0,+1,-1,+s,+2,-2,+2,-2)
 8-9 R/W CJn Offset J (0..3 = +0,+1,-1,-1)
 10-11 R/W RIn Register I (0..3 = R0..R3)
 12 - - Unused (always zero)
 13-14 R/W RJn Register J (0..3 = R4..R7)
 15 - - Unused (always zero) |
| --- |

```

| ________________________________ Step/Modulo ________________________________ |
| --- |

**cfgi - Step and Mod I (for R0..R3)**

**cfgj - Step and Mod J (for R4..R7)**

```

| 0-6 stepi/stepj (7bit) (see "load stepi/stepj") ;step "Rn+s" ?
 7-15 modi/modj (9bit) (see "load modi/modj") |
| --- |

The modulos can be enabled in Control/Status registers. Some opcodes do also
allow to disable modulos via "dmod" suffix.

On TL2, the above 7bit stepi/stepj can be optionally replaced by new 16bit
stepi0/stepj0 registers (via flags in mod2 register).

**stepi0 ;TL2 16bit**

**stepj0 ;TL2 16bit**

```

| 0-16 stepi0/stepj0 |
| --- |

more steps, probably for "modr" with "+s0" (stepII2D2S0)

and for STP16 and BRn?

| DSi TeakLite II Instruction Set Encoding |
| --- |

The opcodes are 16bits wide (some followed by an additional 16bit parameter
word, namely those with "@16" operands). The encoding is very messy (fixed
opcode bits randomly mixed/interleaved with variable parameter bits, and with
new TL2 opcodes squeezed in formerly unused locations), making it pretty much
impossible to decode that unpleasant stuff by software/logic.

The only reasonable decoding way is using a huge table with 65536 entries
(which could be generated temporarily from the information in below table,
using the Base number plus all variable bit combinations, for example, "6100h
TL mov MemImm8@0, Ab@11" has variable bits in bit0-7 and bit11-12, so the
opcode would be mapped at 6100h-61FFh, 6900h-69FFh, 7100h-71FFh, 7900h-79FFh).

**TeakLite I (TL) and TeakLite II (TL2) Opcodes**

```

| Base Ver Opcode (with parameter bits located at @bitnumber and up)
 D4FBh TL add MemImm16@16, Ax@8
 A600h TL add MemImm8@0, Ax@8
 86C0h TL add Imm16@16, Ax@8
 C600h TL add Imm8u@0, Ax@8
 D4DBh TL add MemR7Imm16@16, Ax@8
 4600h TL add MemR7Imm7s@0, Ax@8
 8680h TL add MemRn@0, Ax@8 || Rn@0stepZIDS@3
 86A0h TL add RegisterP0@0, Ax@8
 D2DAh TL2 add Ab@10, Bx@0
 5DF0h TL2 add Bx@1, Ax@0
 9070h TL2 add MemR01@8, sv, Abh@2 || sub MemR01@8offsZI@0, sv, Abl@2
 || mov Abl@2, MemR45@8 || R01@8stepII2@0, R45@8stepII2@1
 5DB0h TL2 add MemR04@1, sv, Abh@2 || sub MemR04@1offsZI@0, sv, Abl@2
 || R04@1stepII2@0
 6F80h TL2 add MemR45@2, MemR01@2, Abh@3
 || add MemR45@2offsZI@1, MemR01@2offsZI@0, Abl@3
 || R01@2stepII2@0, R45@2stepII2@1
 6FA0h TL2 add MemR45@2, MemR01@2, Abh@3
 || sub MemR45@2offsZI@1, MemR01@2offsZI@0, Abl@3
 || R01@2stepII2@0, R45@2stepII2@1
 5E30h TL2 add MemR45@8, sv, Abh@2 || sub MemR45@8offsZI@1, sv, Abl@2
 || mov Abl@2, MemR01@8 || R01@8stepII2@0, R45@8stepII2@1
 5DC0h TL2 add p0, p1, Ab@2
 D782h TL2 add p1, Ax@0
 5DF8h TL2 add Px@1, Bx@0
 D38Bh TL2 add r6, Ax@4
 4590h TL2 add3 p0, p1, Ab@2
 4592h TL2 add3a p0, p1, Ab@2
 4593h TL2 add3aa p0, p1, Ab@2
 5DC1h TL2 adda p0, p1, Ab@2
 B200h TL addh MemImm8@0, Ax@8
 9280h TL addh MemRn@0, Ax@8 || Rn@0stepZIDS@3
 92A0h TL addh Register@0, Ax@8
 9464h TL2 addh r6, Ax@0
 90E0h TL2 addhp MemR0425@2, Px@4, Ax@8 || R0425@2stepII2D2S@0 ;p=ProgMem? Px?
 B400h TL addl MemImm8@0, Ax@8
 9480h TL addl MemRn@0, Ax@8 || Rn@0stepZIDS@3
 94A0h TL addl Register@0, Ax@8
 9466h TL2 addl r6, Ax@0
 906Ch TL2 addsub p0, p1, Ab@0
 49C2h TL2 addsub p1, p0, Ab@4
 916Ch TL2 addsuba p0, p1, Ab@0
 49C3h TL2 addsuba p1, p0, Ab@4
 E700h TL addv Imm16@16, MemImm8@0
 86E0h TL addv Imm16@16, MemRn@0 || Rn@0stepZIDS@3
 87E0h TL addv Imm16@16, Register@0
 47BBh TL2 addv Imm16@16, r6
 D4F9h TL and MemImm16@16, Ax@8
 A200h TL and MemImm8@0, Ax@8
 82C0h TL and Imm16@16, Ax@8
 C200h TL and Imm8u@0, Ax@8
 D4D9h TL and MemR7Imm16@16, Ax@8
 4200h TL and MemR7Imm7s@0, Ax@8
 8280h TL and MemRn@0, Ax@8 || Rn@0stepZIDS@3
 82A0h TL and RegisterP0@0, Ax@8
 6770h TL2 and Ab@2, Ab@0, Ax@12 ;TL2 only
 D389h TL2 and r6, Ax@4
 4B80h TL banke BankFlags6@0 ;{r0}{,r1}{,r4}{,cfgi}{,r7}{,cfgj}
 8CDFh TL2 bankr ;without operand ?
 8CDCh TL2 bankr Ar@0
 8CD0h TL2 bankr Ar@2, Arp@0
 8CD8h TL2 bankr Arp@0
 5EB8h TL2 bitrev Rn@0
 D7E8h TL2 bitrev Rn@0, dbrv
 D7E0h TL2 bitrev Rn@0, ebrv
 5C00h TL bkrep NoReverse, Imm8u@0, Address16@16
 5D00h TL bkrep NoReverse, Register@0, Address18@16and5
 8FDCh TL2 bkrep NoReverse, r6, Address18@16and0
 DA9Ch TL2 bkreprst MemR0425@0
 5F48h TL2 bkreprst MemSp, Unused2@0
 DADCh TL2 bkrepsto MemR0425@0, Unused1@10
 9468h TL2 bkrepsto MemSp, Unused3@0
 4180h TL br Address18@16and4, Cond@0
 D3C0h TL break ;break
 5000h TL brr RelAddr7@4, Cond@0
 41C0h TL call Address18@16and4, Cond@0
 D480h TL calla Axl@8
 D381h TL2 calla Ax@4
 1000h TL callr RelAddr7@4, Cond@0
 9068h TL2 cbs Axh@0, Axh@not0, r0, ge
 9168h TL2 cbs Axh@0, Axh@not0, r0, gt
 D49Eh TL2 cbs Axh@8, Bxh@5, r0, ge
 D49Fh TL2 cbs Axh@8, Bxh@5, r0, gt
 D5C0h TL2 cbs MemR01@2, MemR45@2, ge || R01@2stepII2@0, R45@2stepII2@1
 D5C8h TL2 cbs MemR01@2, MemR45@2, gt || R01@2stepII2@0, R45@2stepII2@1
 E500h TL chng Imm16@16, MemImm8@0
 84E0h TL chng Imm16@16, MemRn@0 || Rn@0stepZIDS@3
 85E0h TL chng Imm16@16, Register@0
 47BAh TL2 chng Imm16@16, r6
 0038h TL2 chng Imm16@16, SttMod@0
 6760h TL clr Implied ConstZero, Ax@12, Cond@0 ;aX=0
 6F60h TL clr Implied ConstZero, Bx@12, Cond@0 ;bX=0
 8ED0h TL2 clr Implied ConstZero, Ab@2, Ab@0
 5DFEh TL2 clrp p0
 5DFFh TL2 clrp p0, p1
 5DFDh TL2 clrp p1
 67C0h TL clrr Implied Const8000h, Ax@12, Cond@0 ;aX=8000h
 6F70h TL2 clrr Implied Const8000h, Bx@12, Cond@0 ;bX=8000h
 8DD0h TL2 clrr Implied Const8000h, Ab@2, Ab@0
 D4FEh TL cmp MemImm16@16, Ax@8
 AC00h TL cmp MemImm8@0, Ax@8
 8CC0h TL cmp Imm16@16, Ax@8
 CC00h TL cmp Imm8u@0, Ax@8
 D4DEh TL cmp MemR7Imm16@16, Ax@8
 4C00h TL cmp MemR7Imm7s@0, Ax@8
 8C80h TL cmp MemRn@0, Ax@8 || Rn@0stepZIDS@3
 8CA0h TL cmp RegisterP0@0, Ax@8
 4D8Ch TL2 cmp Ax@1, Bx@0
 D483h TL2 cmp b0, b1
 D583h TL2 cmp b1, b0
 DA9Ah TL2 cmp Bx@10, Ax@0
 8B63h TL2 cmp p1, Ax@4
 D38Eh TL2 cmp r6, Ax@4
 BE00h TL cmpu MemImm8@0, Ax@8
 9E80h TL cmpu MemRn@0, Ax@8 || Rn@0stepZIDS@3
 9EA0h TL cmpu Register@0, Ax@8
 8A63h TL2 cmpu r6, Ax@3
 ED00h TL cmpv Imm16@16, MemImm8@0
 8CE0h TL cmpv Imm16@16, MemRn@0 || Rn@0stepZIDS@3
 8DE0h TL cmpv Imm16@16, Register@0
 47BEh TL2 cmpv Imm16@16, r6
 D390h TL cntx r ;restore shadows
 D380h TL cntx s ;store shadows
 67F0h TL copy Implied Ax@not12, Ax@12, Cond@0 ;aX=aY
 67E0h TL dec Implied Const1, Ax@12, Cond@0 ;aX=aX-1
 43C0h TL dint ;IE=0, interrupt disable
 0E00h TL divs MemImm8@0, Ax@8
 4380h TL eint ;IE=1, interrupt enable
 9460h TL exp Bx@0, Implied sv
 9060h TL exp Bx@0, Implied sv, Ax@8
 9C40h TL exp MemRn@0, Implied sv || Rn@0stepZIDS@3
 9840h TL exp MemRn@0, Implied sv, Ax@8 || Rn@0stepZIDS@3
 9040h TL exp RegisterP0@0, Implied sv, Ax@8
 9440h TL exp RegisterP0@0, Implied sv
 D7C1h TL2 exp r6, Implied sv
 D382h TL2 exp r6, Implied sv, Ax@4
 67D0h TL inc Implied Const1, Ax@12, Cond@0 ;aX=aX+1
 49C0h TL lim a0 ;aka a0,a0
 49D0h TL lim a0, a1
 49F0h TL lim a1 ;aka a1,a1
 49E0h TL lim a1, a0
 4D80h TL load Imm2u@0, ps ;st1.bit11-10=imm2
 DB80h TL load Imm7s@0, stepi ;cfgi.LSB=imm7
 DF80h TL load Imm7s@0, stepj ;cfgj.LSB=imm7
 0400h TL load Imm8u@0, page ;st1.LSBs=imm8 ;aka "lpg"
 0200h TL load Imm9u@0, modi ;cfgi.MSB=imm9
 0A00h TL load Imm9u@0, modj ;cfgj.MSB=imm9
 D7D8h TL2 load Imm2u@1, movpd, Unused1@0 ;stt2.bit6.7 (page for ProgMem)
 0010h TL2 load Imm4u@0, ps01 ;mod0.bit10-11,13-14 and st1.10-11 ?
 D400h TL maa MemR45@2, MemR0123@0, Ax@11
 || R0123@0stepZIDS@3, R45@2stepZIDS@5
 8400h TL maa MemRn@0, Imm16@16, Ax@11 || Rn@0stepZIDS@3
 8420h TL maa y0, MemRn@0, Ax@11 || Rn@0stepZIDS@3
 8440h TL maa y0, Register@0, Ax@11
 E400h TL maa y0, MemImm8@0, Ax@11
 5EA8h TL2 maa y0, r6, Ax@0
 D700h TL maasu MemR45@2, MemR0123@0, Ax@11
 || R0123@0stepZIDS@3, R45@2stepZIDS@5
 8700h TL maasu MemRn@0, Imm16@16, Ax@11 || Rn@0stepZIDS@3
 8720h TL maasu y0, MemRn@0, Ax@11 || Rn@0stepZIDS@3
 8740h TL maasu y0, Register@0, Ax@11
 5EAEh TL2 maasu y0, r6, Ax@0
 D200h TL mac MemR45@2, MemR0123@0, Ax@11
 || R0123@0stepZIDS@3, R45@2stepZIDS@5
 8200h TL mac MemRn@0, Imm16@16, Ax@11 || Rn@0stepZIDS@3
 8220h TL mac y0, MemRn@0, Ax@11 || Rn@0stepZIDS@3
 8240h TL mac y0, Register@0, Ax@11
 E200h TL mac y0, MemImm8@0, Ax@11
 5EA4h TL2 mac y0, r6, Ax@0
 4D84h TL2 mac y0, x1, Ax@1, Unused1@0
 5E28h TL2 mac1 MemR45@2, MemR01@2, Ax@8 || R01@2stepII2@0, R45@2stepII2@1
 D600h TL macsu MemR45@2, MemR0123@0, Ax@11
 || R0123@0stepZIDS@3, R45@2stepZIDS@5
 8600h TL macsu MemRn@0, Imm16@16, Ax@11 || Rn@0stepZIDS@3
 E600h TL macsu y0, MemImm8@0, Ax@11
 8620h TL macsu y0, MemRn@0, Ax@11 || Rn@0stepZIDS@3
 8640h TL macsu y0, Register@0, Ax@11
 5EACh TL2 macsu y0, r6, Ax@0
 D300h TL macus MemR45@2, MemR0123@0, Ax@11
 || R0123@0stepZIDS@3, R45@2stepZIDS@5
 8300h TL macus MemRn@0, Imm16@16, Ax@11 || Rn@0stepZIDS@3
 8320h TL macus y0, MemRn@0, Ax@11 || Rn@0stepZIDS@3
 8340h TL macus y0, Register@0, Ax@11
 5EA6h TL2 macus y0, r6, Ax@0
 D500h TL macuu MemR45@2, MemR0123@0, Ax@11
 || R0123@0stepZIDS@3, R45@2stepZIDS@5
 8500h TL macuu MemRn@0, Imm16@16, Ax@11 || Rn@0stepZIDS@3
 8520h TL macuu y0, MemRn@0, Ax@11 || Rn@0stepZIDS@3
 8540h TL macuu y0, Register@0, Ax@11
 5EAAh TL2 macuu y0, r6, Ax@0
 8460h TL max NoReverse, Ax@8, Implied Ax@not8, Bogus MemR0, ge,
 Implied mixp, Implied r0 || R0stepZIDS@3 ;when aY >= aX
 8660h TL max NoReverse, Ax@8, Implied Ax@not8, Bogus MemR0, gt,
 Implied mixp, Implied r0 || R0stepZIDS@3 ;when aY > aX
 5E21h TL2 max a0h, a1h || max a0l, a1l || vtrshr
 5F21h TL2 max a1h, a0h || max a1l, a0l || vtrshr
 D784h TL2 max Axh@1, Bxh@0 || max Axl@1, Bxl@0 || vtrshr
 4A40h TL2 max Axh@3, Bxh@4 || max Axl@3, Bxl@4 || mov Axl@not3, MemR04@1
 || vtrshr || R04@1stepII2@0
 4A44h TL2 max Axh@3, Bxh@4 || max Axl@3, Bxl@4 || mov Axh@not3, MemR04@1
 || vtrshr || R04@1stepII2@0
 45A0h TL2 max Axh@4, Bxh@3 || max Axl@4, Bxl@3 || mov Axh@not4, MemR45@2
 || mov Axl@not4, MemR01@2 || vtrshr
 || R01@2stepII2@0, R45@2stepII2@1
 D590h TL2 max Axh@6, Bxh@5 || max Axl@6, Bxl@5 || mov Axh@not6, MemR01@2
 || mov Axl@not6, MemR45@2 || vtrshr
 || R01@2stepII2@0, R45@2stepII2@1
 4A60h TL2 max Bxh@4, Axh@3 || max Bxl@4, Axl@3 || mov Bxl@not4, MemR04@1
 || vtrshr || R04@1stepII2@0
 4A64h TL2 max Bxh@4, Axh@3 || max Bxl@4, Axl@3 || mov Bxh@not4, MemR04@1
 || vtrshr || R04@1stepII2@0
 8060h TL maxd NoReverse, Ax@8, MemR0, ge, Implied mixp, Implied r0
 || R0stepZIDS@3 ;when (r0) >= aX
 8260h TL maxd NoReverse, Ax@8, MemR0, gt, Implied mixp, Implied r0
 || R0stepZIDS@3 ;when (r0) > aX
 8860h TL min NoReverse, Ax@8, Implied Ax@not8, Bogus MemR0, le,
 Implied mixp, Implied r0 || R0stepZIDS@3 ;when aY <= aX
 8A60h TL min NoReverse, Ax@8, Implied Ax@not8, Bogus MemR0, lt,
 Implied mixp, Implied r0 || R0stepZIDS@3 ;when aY < aX
 43C2h TL2 min Axh@0, Axh@not0 || min Axl@0, Axl@not0 || vtrshr
 D2B8h TL2 min Axh@11, Bxh@10 || min Axl@11, Bxl@10
 || mov Axh@not11, MemR01@2 || mov Axl@not11, MemR45@2
 || vtrshr || R01@2stepII2@0, R45@2stepII2@1
 4A00h TL2 min Axh@3, Bxh@4 || min Axl@3, Bxl@4 || mov Axl@not3, MemR04@1
 || vtrshr || R04@1stepII2@0
 4A04h TL2 min Axh@3, Bxh@4 || min Axl@3, Bxl@4 || mov Axh@not3, MemR04@1
 || vtrshr || R04@1stepII2@0
 45E0h TL2 min Axh@4, Bxh@3 || min Axl@4, Bxl@3 || mov Axh@not4, MemR45@2
 || mov Axl@not4, MemR01@2 || vtrshr
 || R01@2stepII2@0, R45@2stepII2@1
 D4BAh TL2 min Axh@8, Bxh@0 || min Axl@8, Bxl@0 || vtrshr
 4A20h TL2 min Bxh@4, Axh@3 || min Bxl@4, Axl@3 || mov Bxl@not4, MemR04@1
 || vtrshr || R04@1stepII2@0
 4A24h TL2 min Bxh@4, Axh@3 || min Bxl@4, Axl@3 || mov Bxh@not4, MemR04@1
 || vtrshr || R04@1stepII2@0
 47A0h TL2 mind NoReverse, Ax@3, MemR0, le, Implied mixp, Implied r0
 || R0stepZIDS@0
 47A4h TL2 mind NoReverse, Ax@3, MemR0, lt, Implied mixp, Implied r0
 || R0stepZIDS@0
 0080h TL modr MemRn@0stepZIDS@3
 00A0h TL modr MemRn@0stepZIDS@3, dmod ;Disable modulo
 D294h TL2 modr MemR0123@10stepII2D2S0@0 || modr MemR4567@10stepII2D2S0@5
 0D80h TL2 modr MemR0123@5stepII2D2S0@1 || modr MemR4567@5stepII2D2S0@3, dmod
 0D81h TL2 modr MemR0123@5stepII2D2S0@1, dmod
 || modr MemR4567@5stepII2D2S0@3, dmod
 8464h TL2 modr MemR0123@8stepII2D2S0@0, dmod || modr MemR4567@8stepII2D2S0@3
 5DA0h TL2 modr MemRn@0stepD2
 5DA8h TL2 modr MemRn@0stepD2, dmod
 4990h TL2 modr MemRn@0stepI2
 4998h TL2 modr MemRn@0stepI2, dmod
 D290h TL mov Ab@10, Ab@5
 D298h TL mov Abl@10, dvm
 D2D8h TL mov Abl@10, x0
 3000h TL mov Ablh@9, MemImm8@0
 D4BCh TL mov Axl@8, MemImm16@16
 D49Ch TL mov Axl@8, MemR7Imm16@16
 DC80h TL mov Axl@8, MemR7Imm7s@0
 D4B8h TL mov MemImm16@16, Ax@8
 6100h TL mov MemImm8@0, Ab@11
 6200h TL mov MemImm8@0, Ablh@10
 6500h TL mov MemImm8@0, Axh@12, eu ;aka Axheu
 6000h TL mov MemImm8@0, R0123457y0@10
 6D00h TL mov MemImm8@0, sv
 D491h TL mov dvm, Ab@5
 D492h TL mov icr, Ab@5
 5E20h TL mov Imm16@16, Bx@8
 5E00h TL mov Imm16@16, Register@0
 4F80h TL mov Imm5u@0, icr ;uh, but icr is 8bit wide (only 4bit are R/W)?
 2500h TL mov Imm8s@0, Axh@12 ;signed!
 2900h TL mov Imm8s@0, ext0
 2D00h TL mov Imm8s@0, ext1
 3900h TL mov Imm8s@0, ext2
 3D00h TL mov Imm8s@0, ext3
 2300h TL mov Imm8s@0, R0123457y0@10 ;signed!
 0500h TL mov Imm8s@0, sv
 2100h TL mov Imm8u@0, Axl@12 ;unsigned!
 D498h TL mov MemR7Imm16@16, Ax@8
 D880h TL mov MemR7Imm7s@0, Ax@8
 98C0h TL mov MemRn@0, Bx@8 || Rn@0stepZIDS@3
 1C00h TL mov MemRn@0, Register@5 || Rn@0stepZIDS@3
 47E0h TL mov MemSp, Register@0
 47C0h TL mov mixp, Register@0
 2000h TL mov R0123457y0@9, MemImm8@0
 4FC0h TL mov Register@0, icr
 5E80h TL mov Register@0, mixp
 1800h TL mov Register@5, MemRn@0 || Rn@0stepZIDS@3
 5EC0h TL mov RegisterP0@0, Bx@5
 5800h TL mov RegisterP0@0, Register@5
 D490h TL mov repc, Ab@5
 7D00h TL mov sv, MemImm8@0
 D493h TL mov x0, Ab@5
 D49Bh TL2 mov a0h, stepi0
 D59Bh TL2 mov a0h, stepj0
 4390h TL2 mov a0h, MemR0425@2 || mov y0, MemR0425@2offsZIDZ@0
 || R0425@2stepII2D2S@0
 43D0h TL2 mov a1h, MemR0425@2 || mov y0, MemR0425@2offsZIDZ@0
 || R0425@2stepII2D2S@0
 8FD4h TL2 mov Ab@0, p0
 43A0h TL2 mov Abh@3, MemR01@2 || mov Abl@3, MemR45@2
 || R01@2stepII2@0, R45@2stepII2@1
 43E0h TL2 mov Abh@3, MemR45@2 || mov Abl@3, MemR01@2
 || R01@2stepII2@0, R45@2stepII2@1
 9D40h TL2 mov Abh@4, MemR04@1 || mov Abh@2, MemR04@1offsZI@0
 || R04@1stepII2@0
 9164h TL2 mov Abl@0, prpage
 9064h TL2 mov Abl@0, repc
 D394h TL2 mov Abl@0, x1
 D384h TL2 mov Abl@0, y1
 9540h TL2 mov Abl@3, ArArp@0
 9C60h TL2 mov Abl@3, SttMod@0
 9560h TL2 mov ArArp@0, Abl@3
 D488h TL2 mov ArArp@0, MemR04@8 || R04@8stepII2@5
 5F50h TL2 mov ArArpSttMod@0, MemR7Imm16@16
 886Bh TL2 mov Ax@8, pc
 8C60h TL2 mov Axh@4, MemR4567@8 || mov MemR0123@8, Axh@4
 || R0123@8stepII2D2S@0, R4567@8stepII2D2S@2
 4800h TL2 mov Axh@6, MemR0123@4 || movr MemR4567@4, Axh@6
 || R0123@4stepII2D2S@0, R4567@4stepII2D2S@2
 4900h TL2 mov Axh@6, MemR0123@4 || mov MemR4567@4, Axh@6
 || R0123@4stepII2D2S@0, R4567@4stepII2D2S@2
 7F80h TL2 mov Axh@6, MemR4567@4 || movr MemR0123@4, Axh@6
 || R0123@4stepII2D2S@0, R4567@4stepII2D2S@2
 8863h TL2 mov Bx@8, pc
 0008h TL2 mov Imm16@16, ArArp@0
 0023h TL2 mov Imm16@16, r6
 0001h TL2 mov Imm16@16, repc
 8971h TL2 mov Imm16@16, stepi0
 8979h TL2 mov Imm16@16, stepj0
 0030h TL2 mov Imm16@16, SttMod@0
 5DD0h TL2 mov Imm4u@0, prpage
 80C4h TL2 mov MemR01@9, Abh@10 || mov MemR45@9, Abl@10
 || R01@9stepII2@0, R45@9stepII2@8
 D292h TL2 mov MemR0425@10_MemR0425@10offsZIDZ@5, Px@0
 || R0425@10stepII2D2S@5
 D7D4h TL2 mov MemR04@1, repc || R04@1stepII2@0
 5F4Ch TL2 mov MemR04@1, sv || sub3 MemR04@1, p0, p1, b0 || R04@1stepII2@0
 D4B4h TL2 mov MemR04@1, sv || sub3rnd MemR04@1, p0, p1, b1 || R04@1stepII2@0
 DE9Ch TL2 mov MemR04@1, sv || sub3rnd MemR04@1, p0, p1, b0 || R04@1stepII2@0
 4B40h TL2 mov MemR04@3, sv || addsub MemR04@3, p1, p0, Bx@0
 || R04@3stepII2@2
 4B42h TL2 mov MemR04@3, sv || addsubrnd MemR04@3, p1, p0, Bx@0
 || R04@3stepII2@2
 8062h TL2 mov MemR04@4, ArArp@8 || R04@4stepII2@3
 8063h TL2 mov MemR04@4, SttMod@8 || R04@4stepII2@3
 9960h TL2 mov MemR04@4, sv || addsub MemR04@4, p1, p0, Bx@2
 || R04@4stepD2S@3 ;<-- ordered p1, p0 here !
 99E0h TL2 mov MemR04@4, sv || addsubrnd MemR04@4, p1, p0, Bx@2
 || R04@4stepD2S@3 ;<-- ordered p1, p0 here !
 9860h TL2 mov MemR04@4, sv || sub3 MemR04@4, p0, p1, Bx@2
 || R04@4stepD2S@3
 98E0h TL2 mov MemR04@4, sv || sub3rnd MemR04@4, p0, p1, Bx@2
 || R04@4stepD2S@3
 8873h TL2 mov MemR04@8, sv || sub3 MemR04@8, p0, p1, b1 || R04@8stepII2@3
 D4C0h TL2 mov MemR45@5, Abh@2 || mov MemR01@5, Abl@2
 || R01@5stepII2@0, R45@5stepII2@1
 4D90h TL2 mov MemR7Imm16@16, ArArpSttMod@0
 D2DCh TL2 mov MemR7Imm16@16, repc, Unused2@0, Unused1@10
 1B20h TL2 mov MemRn@0, r6 || Rn@0stepZIDS@3 ;override 1800h (mov a1,MemRn@0)
 D29Ch TL2 mov MemSp, r6, Unused2@0, Unused1@10
 8A73h TL2 mov mixp, Bx@3
 4381h TL2 mov mixp, r6
 4382h TL2 mov p0h, Bx@0
 D3C2h TL2 mov p0h, r6
 4B60h TL2 mov p0h, Register@0 ;<-- here "p0h" as source
 8FD8h TL2 mov p1, Ab@0
 88D0h TL2 mov Px@1, MemR0425@8_MemR0425@8offsZIDZ@2 || R0425@8stepII2D2S@2
 88D1h TL2 mov Px@1, MemR0425@8_MemR0425@8offsZIDZ@2,s || R0425@8stepII2D2S@2
 D481h TL2 mov r6, Bx@8
 1B00h TL2 mov r6, MemRn@0 || Rn@0stepZIDS@3 ;override 1800h (mov a0,MemRn@0)
 43C1h TL2 mov r6, mixp
 5F00h TL2 mov r6, Register@0
 5F60h TL2 mov Register@0, r6
 D2D9h TL2 mov repc, Abl@10
 D7D0h TL2 mov repc, MemR04@1 || R04@1stepII2@0
 D3C8h TL2 mov repc, MemR7Imm16@16, Unused3@0
 D482h TL2 mov stepi0, a0h
 D582h TL2 mov stepj0, a0h
 D2F8h TL2 mov SttMod@0, Abl@10
 49C1h TL2 mov x1, Ab@4
 D299h TL2 mov y1, Ab@10
 5EB0h TL2 mov prpage, Abl@0
 49A0h TL2 mov SttMod@0, MemR04@4 || R04@4stepII2@3
 4DC0h TL2 mova Ab@4, MemR0425@2_MemR0425@2offsZIDZ@0 || R0425@2stepII2D2S@0
 4BC0h TL2 mova MemR0425@2_MemR0425@2offsZIDZ@0, Ab@4 || R0425@2stepII2D2S@0
 5F80h TL movd MemR0123@0,ProgMemR45@2 || R0123@0stepZIDS@3, R45@2stepZIDS@5
 0040h TL movp ProgMemAxl@5, Register@0
 0D40h TL2 movp ProgMemAx@5, Register@0
 0600h TL movp ProgMemRn@0, MemR0123@5 || R0123@5stepZIDS@7, Rn@0stepZIDS@3
 D499h TL2 movpdw ProgMemAx@8_ProgMemAx@8offsI, pc
 8864h TL movr MemR0425@3, Abh@8 || R0425@3stepII2D2S@0 ;op*10000h+8000h
 9CE0h TL movr MemRn@0, Ax@8 || Rn@0stepZIDS@3
 9CC0h TL movr RegisterP0@0, Ax@8
 5DF4h TL2 movr Bx@1, Ax@0
 8961h TL2 movr r6, Ax@3
 6300h TL movs Implied sv, MemImm8@0, Ab@11
 0180h TL movs Implied sv, MemRn@0, Ab@5 || Rn@0stepZIDS@3
 0100h TL movs Implied sv, RegisterP0@0, Ab@5
 5F42h TL2 movs Implied sv, r6, Ax@0
 4080h TL movsi Implied Imm5s@0, R0123457y0@9, Ab@5, Bogus Imm5s@0
 D000h TL mpy MemR45@2, MemR0123@0 || R0123@0stepZIDS@3, R45@2stepZIDS@5
 8000h TL mpy MemRn@0, Imm16@16 || Rn@0stepZIDS@3
 8020h TL mpy y0, MemRn@0 || Rn@0stepZIDS@3
 8040h TL mpy y0, Register@0
 E000h TL mpy y0, MemImm8@0
 5EA0h TL2 mpy y0, r6
 CB00h TL2 mpy MemR45@5, MemR01@5 || mpysu MemR45@5offsZI@4, MemR01@5offsZI@3
 || sub3 p0, p1, Ab@6 || R01@5stepII2@3, R45@5stepII2@4
 CB01h TL2 mpy MemR45@5, MemR01@5 || mpyus MemR45@5offsZI@4, MemR01@5offsZI@3
 || sub3 p0, p1, Ab@6 || R01@5stepII2@3, R45@5stepII2@4
 CB02h TL2 mpy MemR45@5, MemR01@5 || mpysu MemR45@5offsZI@4, MemR01@5offsZI@3
 || sub3a p0, p1, Ab@6 || R01@5stepII2@3, R45@5stepII2@4
 CB03h TL2 mpy MemR45@5, MemR01@5 || mpyus MemR45@5offsZI@4, MemR01@5offsZI@3
 || sub3a p0, p1, Ab@6 || R01@5stepII2@3, R45@5stepII2@4
 CB04h TL2 mpy MemR45@5, MemR01@5 || mpysu MemR45@5offsZI@4, MemR01@5offsZI@3
 || add3 p0, p1, Ab@6 || R01@5stepII2@3, R45@5stepII2@4
 CB05h TL2 mpy MemR45@5, MemR01@5 || mpyus MemR45@5offsZI@4, MemR01@5offsZI@3
 || add3 p0, p1, Ab@6 || R01@5stepII2@3, R45@5stepII2@4
 CB06h TL2 mpy MemR45@5, MemR01@5 || mpysu MemR45@5offsZI@4, MemR01@5offsZI@3
 || add3a p0, p1, Ab@6 || R01@5stepII2@3, R45@5stepII2@4
 CB07h TL2 mpy MemR45@5, MemR01@5 || mpyus MemR45@5offsZI@4, MemR01@5offsZI@3
 || add3a p0, p1, Ab@6 || R01@5stepII2@3, R45@5stepII2@4
 D5E0h TL2 mpy MemR04@1, x1 || mpy y1, x0 || sub3 p0, p1, Ax@3
 || R04@1stepII2@0
 D5E4h TL2 mpy MemR04@1, x1 || mpy y1, x0 || add3 p0, p1, Ax@3
 || R04@1stepII2@0
 C800h TL2 mpy MemR4567@4, MemR0123@4
 || mpy MemR4567@4offsZIDZ@2, MemR0123@4offsZIDZ@0
 || add3 p0, p1, Ab@6 || R0123@4stepII2D2S@0, R4567@4stepII2D2S@2
 C900h TL2 mpy MemR4567@4, MemR0123@4
 || mpy MemR4567@4offsZIDZ@2, MemR0123@4offsZIDZ@0
 || sub3 p0, p1, Ab@6 || R0123@4stepII2D2S@0, R4567@4stepII2D2S@2
 80C2h TL2 mpy MemR45@0, MemR01@0 || mpy MemR45@0offsZI@9, MemR01@0offsZI@8
 || add3a p0, p1, Ab@10 || R01@0stepII2@8, R45@0stepII2@9
 49C8h TL2 mpy MemR45@2, MemR01@2 || mpy MemR45@2offsZI@1, MemR01@2offsZI@0
 || sub3a p0, p1, Ab@4 || R01@2stepII2@0, R45@2stepII2@1
 80C8h TL2 mpy MemR45@2, MemR01@2 || mpy MemR45@2offsZI@1, MemR01@2offsZI@0
 || addsub p0, p1, Ab@10 || R01@2stepII2@0, R45@2stepII2@1
 81C8h TL2 mpy MemR45@2, MemR01@2 || mpy MemR45@2offsZI@1, MemR01@2offsZI@0
 || addsuba p0, p1, Ab@10 || R01@2stepII2@0, R45@2stepII2@1
 82C8h TL2 mpy MemR45@2, MemR01@2 || mpy MemR45@2offsZI@1, MemR01@2offsZI@0
 || add p0, p1, Ab@10 || R01@2stepII2@0, R45@2stepII2@1
 83C8h TL2 mpy MemR45@2, MemR01@2 || mpy MemR45@2offsZI@1, MemR01@2offsZI@0
 || adda p0, p1, Ab@10 || R01@2stepII2@0, R45@2stepII2@1
 00C0h TL2 mpy MemR45@3, MemR01@3 || mpy MemR45@3offsZI@2, MemR01@3offsZI@1
 || sub p0, p1, Ab@4 || R01@3stepII2@1, R45@3stepII2@2
 00C1h TL2 mpy MemR45@3, MemR01@3 || mpy MemR45@3offsZI@2, MemR01@3offsZI@1
 || suba p0, p1, Ab@4 || R01@3stepII2@1, R45@3stepII2@2
 0D20h TL2 mpy MemR45@3, MemR01@3 || mpyus MemR45@3offsZI@2, MemR01@3offsZI@1
 || add3a p0, p1, Ax@0, dmodi || R01@3stepII2@1, R45@3stepII2@2
 0D30h TL2 mpy MemR45@3, MemR01@3 || mpyus MemR45@3offsZI@2, MemR01@3offsZI@1
 || add3a p0, p1, Ax@0, dmodj || R01@3stepII2@1, R45@3stepII2@2
 4B50h TL2 mpy MemR45@3, MemR01@3 || mpyus MemR45@3offsZI@2, MemR01@3offsZI@1
 || add3a p0, p1, Ax@0, dmodij || R01@3stepII2@1, R45@3stepII2@2
 D7A0h TL2 mpy MemR45@3, MemR01@3 || mpy MemR45@3offsZI@2, MemR01@3offsZI@1
 || add3 sv, p0, p1, Ax@4 || R01@3stepII2@1, R45@3stepII2@2
 D7A1h TL2 mpy MemR45@3, MemR01@3 || mpy MemR45@3offsZI@2, MemR01@3offsZI@1
 || add3rnd sv, p0, p1, Ax@4 || R01@3stepII2@1, R45@3stepII2@2
 9861h TL2 mpy MemR45@4, MemR01@4 || mpy MemR45@4offsZI@3, MemR01@4offsZI@2
 || add3 p0, p1, Ax@8, dmodj || R01@4stepII2@2, R45@4stepII2@3
 9862h TL2 mpy MemR45@4, MemR01@4 || mpy MemR45@4offsZI@3, MemR01@4offsZI@2
 || add3 p0, p1, Ax@8, dmodi || R01@4stepII2@2, R45@4stepII2@3
 9863h TL2 mpy MemR45@4, MemR01@4 || mpy MemR45@4offsZI@3, MemR01@4offsZI@2
 || add3 p0, p1, Ax@8, dmodij || R01@4stepII2@2, R45@4stepII2@3
 98E1h TL2 mpy MemR45@4, MemR01@4 || mpy MemR45@4offsZI@3, MemR01@4offsZI@2
 || add3a p0, p1, Ax@8, dmodj || R01@4stepII2@2, R45@4stepII2@3
 98E2h TL2 mpy MemR45@4, MemR01@4 || mpy MemR45@4offsZI@3, MemR01@4offsZI@2
 || add3a p0, p1, Ax@8, dmodi || R01@4stepII2@2, R45@4stepII2@3
 98E3h TL2 mpy MemR45@4, MemR01@4 || mpy MemR45@4offsZI@3, MemR01@4offsZI@2
 || add3a p0, p1, Ax@8, dmodij || R01@4stepII2@2, R45@4stepII2@3
 4DA0h TL2 mpy y0, MemR04@3 || mpyus y1, MemR04@3offsZI@2
 || sub3 p0, p1, Ax@4 || R04@3stepII2@2
 4DA1h TL2 mpy y0, MemR04@3 || mpyus y1, MemR04@3offsZI@2
 || sub3a p0, p1, Ax@4 || R04@3stepII2@2
 4DA2h TL2 mpy y0, MemR04@3 || mpyus y1, MemR04@3offsZI@2
 || add3 p0, p1, Ax@4 || R04@3stepII2@2
 4DA3h TL2 mpy y0, MemR04@3 || mpyus y1, MemR04@3offsZI@2
 || add3a p0, p1, Ax@4 || R04@3stepII2@2
 94E0h TL2 mpy y0, MemR04@4 || mpy y1, MemR04@4offsZI@3
 || sub3 p0, p1, Ax@8 || R04@4stepII2@3
 94E2h TL2 mpy y0, MemR04@4 || mpy y1, MemR04@4offsZI@3
 || sub3a p0, p1, Ax@8 || R04@4stepII2@3
 94E4h TL2 mpy y0, MemR04@4 || mpy y1, MemR04@4offsZI@3
 || add3 p0, p1, Ax@8 || R04@4stepII2@3
 94E6h TL2 mpy y0, MemR04@4 || mpy y1, MemR04@4offsZI@3
 || add3a p0, p1, Ax@8 || R04@4stepII2@3
 94E1h TL2 mpy y0, MemR04@4 || mpysu y1, MemR04@4offsZI@3
 || sub3 p0, p1, Ax@8 || R04@4stepII2@3
 94E3h TL2 mpy y0, MemR04@4 || mpysu y1, MemR04@4offsZI@3
 || sub3a p0, p1, Ax@8 || R04@4stepII2@3
 94E5h TL2 mpy y0, MemR04@4 || mpysu y1, MemR04@4offsZI@3
 || add3 p0, p1, Ax@8 || R04@4stepII2@3
 94E7h TL2 mpy y0, MemR04@4 || mpysu y1, MemR04@4offsZI@3
 || add3a p0, p1, Ax@8 || R04@4stepII2@3
 8862h TL2 mpy y0, x1 || mpy MemR04@4, x0 || sub3 p0, p1, Ax@8
 || R04@4stepII2@3
 8A62h TL2 mpy y0, x1 || mpy MemR04@4, x0 || add3 p0, p1, Ax@8
 || R04@4stepII2@3
 4D88h TL2 mpy y0, x1 || mpy y1, x0 || sub p0, p1, Ax@1
 5E24h TL2 mpy y0, x1 || mpy y1, x0 || add p0, p1, Ab@0
 8061h TL2 mpy y0, x1 || mpy y1, x0 || add3 p0, p1, Ab@8
 8071h TL2 mpy y0, x1 || mpy y1, x0 || add3a p0, p1, Ab@8
 8461h TL2 mpy y0, x1 || mpy y1, x0 || sub3 p0, p1, Ab@8
 8471h TL2 mpy y0, x1 || mpy y1, x0 || sub3a p0, p1, Ab@8
 D484h TL2 mpy y0, x1 || mpy y1, x0 || add3aa p0, p1, Ab@0
 D49Dh TL2 mpy y0, x1 || mpy y1, x0 || sub p0, p1, Bx@5
 D4A0h TL2 mpy y0, x1 || mpy y1, x0 || addsub p0, p1, Ab@0
 4FA0h TL2 mpy y0, x1 || mpy y1, x0 || add3 p0, p1, Ab@3
 || mov Axh@6, MemR04@1 || mov Bxh@2, MemR04@1offsZI@0
 || R04@1stepII2@0
 5818h TL2 mpy y0, x1 || mpy y1, x0 || addsub sv, p0, p1, Ax@0
 || mov Axh@0, MemR0425@7 || mov Axh@not0, MemR0425@7offsZI@6
 || R0425@7stepII2@6 ;override 5800h+18h (mov a0, Register)
 5838h TL2 mpy y0, x1 || mpy y1, x0 || addsubrnd sv, p0, p1, Ax@0
 || mov Axh@0, MemR0425@7 || mov Axh@not0, MemR0425@7offsZI@6
 || R0425@7stepII2@6 ;override 5800h+38h (mov a1, Register)
 80D0h TL2 mpy y0, x1 || mpy y1, x0 || addsub sv, p0, p1, Ax@10
 || mov Axh@9, MemR04@3 || mov Bxh@8, MemR04@3offsZI@2
 || R04@3stepII2@2
 80D1h TL2 mpy y0, x1 || mpy y1, x0 || addsubrnd sv, p0, p1, Ax@10
 || mov Axh@9, MemR04@3 || mov Bxh@8, MemR04@3offsZI@2
 || R04@3stepII2@2
 80D2h TL2 mpy y0, x1 || mpy y1, x0 || add3 sv, p0, p1, Ax@10
 || mov Axh@9, MemR04@3 || mov Bxh@8, MemR04@3offsZI@2
 || R04@3stepII2@2
 80D3h TL2 mpy y0, x1 || mpy y1, x0 || add3rnd sv, p0, p1, Ax@10
 || mov Axh@9, MemR04@3 || mov Bxh@8, MemR04@3offsZI@2
 || R04@3stepII2@2
 D3A0h TL2 mpy y0, x1 || mpy y1, x0 || addsub p0, p1, Ab@3
 || mov Axh@6, MemR04@1 || mov Bxh@2, MemR04@1offsZI@0
 || R04@1stepII2@0
 4D89h TL2 mpy y0, x1 || mpyus y1, x0 || sub p0, p1, Ax@1
 5F24h TL2 mpy y0, x1 || mpyus y1, x0 || add p0, p1, Ab@0
 8069h TL2 mpy y0, x1 || mpyus y1, x0 || add3 p0, p1, Ab@8
 8079h TL2 mpy y0, x1 || mpyus y1, x0 || add3a p0, p1, Ab@8
 8469h TL2 mpy y0, x1 || mpyus y1, x0 || sub3 p0, p1, Ab@8
 8479h TL2 mpy y0, x1 || mpyus y1, x0 || sub3a p0, p1, Ab@8
 D584h TL2 mpy y0, x1 || mpyus y1, x0 || add3aa p0, p1, Ab@0
 D59Dh TL2 mpy y0, x1 || mpyus y1, x0 || sub p0, p1, Bx@5
 D5A0h TL2 mpy y0, x1 || mpyus y1, x0 || addsub p0, p1, Ab@0
 0800h TL mpyi NoReverse, Implied p0, y0, Imm8s@0 ;multiply ;aka "mpys"
 D100h TL mpysu MemR45@2, MemR0123@0 || R0123@0stepZIDS@3, R45@2stepZIDS@5
 8100h TL mpysu MemRn@0, Imm16@16 || Rn@0stepZIDS@3
 8120h TL mpysu y0, MemRn@0 || Rn@0stepZIDS@3
 8140h TL mpysu y0, Register@0
 CA00h TL2 mpysu MemR45@5, MemR01@5
 || mpysu MemR45@5offsZI@4, MemR01@5offsZI@3
 || sub3a p0, p1, Ab@6 || R01@5stepII2@3, R45@5stepII2@4
 CA01h TL2 mpysu MemR45@5, MemR01@5
 || mpyus MemR45@5offsZI@4, MemR01@5offsZI@3
 || sub3a p0, p1, Ab@6 || R01@5stepII2@3, R45@5stepII2@4
 CA02h TL2 mpysu MemR45@5, MemR01@5
 || mpysu MemR45@5offsZI@4, MemR01@5offsZI@3
 || sub3aa p0, p1, Ab@6 || R01@5stepII2@3, R45@5stepII2@4
 CA03h TL2 mpysu MemR45@5, MemR01@5
 || mpyus MemR45@5offsZI@4, MemR01@5offsZI@3
 || sub3aa p0, p1, Ab@6 || R01@5stepII2@3, R45@5stepII2@4
 CA04h TL2 mpysu MemR45@5, MemR01@5
 || mpysu MemR45@5offsZI@4, MemR01@5offsZI@3
 || add3a p0, p1, Ab@6 || R01@5stepII2@3, R45@5stepII2@4
 CA05h TL2 mpysu MemR45@5, MemR01@5
 || mpyus MemR45@5offsZI@4, MemR01@5offsZI@3
 || add3a p0, p1, Ab@6 || R01@5stepII2@3, R45@5stepII2@4
 CA06h TL2 mpysu MemR45@5, MemR01@5
 || mpysu MemR45@5offsZI@4, MemR01@5offsZI@3
 || add3aa p0, p1, Ab@6 || R01@5stepII2@3, R45@5stepII2@4
 CA07h TL2 mpysu MemR45@5, MemR01@5
 || mpyus MemR45@5offsZI@4, MemR01@5offsZI@3
 || add3aa p0, p1, Ab@6 || R01@5stepII2@3, R45@5stepII2@4
 5EA2h TL2 mpysu y0, r6
 D080h TL msu MemR45@2,MemR0123@0,Ax@8 || R0123@0stepZIDS@3, R45@2stepZIDS@5
 90C0h TL msu MemRn@0, Imm16@16, Ax@8 || Rn@0stepZIDS@3 ;multiply, subtract
 9080h TL msu y0, MemRn@0, Ax@8 || Rn@0stepZIDS@3
 90A0h TL msu y0, Register@0, Ax@8
 B000h TL msu y0,MemImm8@0, Ax@8
 9462h TL2 msu y0, r6, Ax@0
 8264h TL2 msusu y0, MemR0425@3, Ax@8 || R0425@3stepII2D2S@0
 6790h TL neg Ax@12, Cond@0 ;aX=0-aX
 0000h TL nop
 94C0h TL norm Ax@8, Bogus MemRn@0 || Rn@0stepZIDS@3 ;if N=0 (aX=aX*2,rN+/-)
 6780h TL not Ax@12, Cond@0 ;aX=not aX
 D4F8h TL or MemImm16@16, Ax@8
 A000h TL or MemImm8@0, Ax@8
 80C0h TL or Imm16@16, Ax@8
 C000h TL or Imm8u@0, Ax@8
 D4D8h TL or MemR7Imm16@16, Ax@8
 4000h TL or MemR7Imm7s@0, Ax@8
 8080h TL or MemRn@0, Ax@8 || Rn@0stepZIDS@3
 80A0h TL or RegisterP0@0, Ax@8
 D291h TL2 or Ab@10, Ax@6, Ax@5
 D4A4h TL2 or Ax@8, Bx@1, Ax@0
 D3C4h TL2 or b0, Bx@1, Ax@0
 D7C4h TL2 or b1, Bx@1, Ax@0
 D388h TL2 or r6, Ax@4
 67B0h TL pacr Implied Const8000h, Implied p0, Ax@12, Cond@0 ;aX=shfP+8000h
 D7C2h TL2 pacr1 Implied Const8000h, Implied p1, Ax@0
 5E60h TL pop Register@0
 47B4h TL2 pop Abe@0
 80C7h TL2 pop ArArpSttMod@8
 0006h TL2 pop Bx@5, Unused1@0
 D7F4h TL2 pop prpage, Unused2@0
 D496h TL2 pop Px@0
 0024h TL2 pop r6, Unused1@0
 D7F0h TL2 pop repc, Unused2@0
 D494h TL2 pop x0
 D495h TL2 pop x1
 0004h TL2 pop y1, Unused1@0
 47B0h TL2 popa Ab@0
 5F40h TL push Imm16@16
 5E40h TL push Register@0
 D7C8h TL2 push Abe@1, Unused1@0
 D3D0h TL2 push ArArpSttMod@0
 D7FCh TL2 push prpage, Unused2@0
 D78Ch TL2 push Px@1, Unused1@0
 D4D7h TL2 push r6, Unused1@5
 D7F8h TL2 push repc, Unused2@0
 D4D4h TL2 push x0, Unused1@5
 D4D5h TL2 push x1, Unused1@5
 D4D6h TL2 push y1, Unused1@5
 4384h TL2 pusha Ax@6, Unused2@0
 D788h TL2 pusha Bx@1, Unused1@0
 0C00h TL rep Imm8u@0 ;repeat next opcode N+1 times
 0D00h TL rep Register@0 ;repeat next opcode N+1 times
 0002h TL2 rep r6, Unused1@0
 4580h TL ret Cond@0 ;=pop pc
 D780h TL retd ;delayed return (after 2 clks)
 45C0h TL reti Cond@0 ;Don't context switch
 45D0h TL reti Cond@0, context ;Do context switch
 D7C0h TL retid ;delayed, from interrupt
 D3C3h TL2 retid context
 0900h TL rets Imm8u@0 ;ret+dealloc sp (for INCOMING pushed params)
 67A0h TL rnd Implied Const8000h, Ax@12, Cond@0 ;aX=aX+8000h
 6750h TL rol Implied Const1, Ax@12, Cond@0 ;aX=aX rcl 1 (37bit rotate)
 6F50h TL rol Implied Const1, Bx@12, Cond@0 ;bX=bX rcl 1 (37bit rotate)
 6740h TL ror Implied Const1, Ax@12, Cond@0 ;aX=aX rcr 1 (37bit rotate)
 6F40h TL ror Implied Const1, Bx@12, Cond@0 ;bX=bX rcr 1 (37bit rotate)
 E300h TL rst Imm16@16, MemImm8@0
 82E0h TL rst Imm16@16, MemRn@0 || Rn@0stepZIDS@3
 83E0h TL rst Imm16@16, Register@0
 47B9h TL2 rst Imm16@16, r6
 4388h TL2 rst Imm16@16, SttMod@0
 E100h TL set Imm16@16, MemImm8@0
 80E0h TL set Imm16@16, MemRn@0 || Rn@0stepZIDS@3
 81E0h TL set Imm16@16, Register@0
 47B8h TL2 set Imm16@16, r6
 43C8h TL2 set Imm16@16, SttMod@0
 D280h TL shfc Implied sv, Ab@10, Ab@5, Cond@0
 9240h TL shfi Implied Imm6s@0, Ab@10, Ab@7, Bogus Imm6s@0
 6720h TL shl Implied Const1, Ax@12, Cond@0 ;aX=aX*2
 6F20h TL shl Implied Const1, Bx@12, Cond@0 ;bX=bX*2
 6730h TL shl4 Implied Const4, Ax@12, Cond@0 ;aX=aX*10h
 6F30h TL shl4 Implied Const4, Bx@12, Cond@0 ;bX=bX*10h
 6700h TL shr Implied Const1, Ax@12, Cond@0 ;aX=aX/2
 6F00h TL shr Implied Const1, Bx@12, Cond@0 ;bX=bX/2
 6710h TL shr4 Implied Const4, Ax@12, Cond@0 ;aX=aX/10h
 6F10h TL shr4 Implied Const4, Bx@12, Cond@0 ;bX=bX/10h
 BA00h TL sqr MemImm8@0
 9A80h TL sqr MemRn@0 || Rn@0stepZIDS@3
 9AA0h TL sqr Register@0
 D790h TL2 sqr Abh@2 || sqr Abl@2 || add3 p0, p1, Ab@0
 49C4h TL2 sqr Abh@4 || mpysu Abh@4, Abl@4 || add3a p0, p1, Ab@0
 4B00h TL2 sqr MemR0425@4 || sqr MemR0425@4offsZIDZ@2 || add3 p0, p1, Ab@0
 || R0425@4stepII2D2S@2
 5F41h TL2 sqr r6
 BC00h TL sqra MemImm8@0, Ax@8
 9C80h TL sqra MemRn@0, Ax@8 || Rn@0stepZIDS@3
 9CA0h TL sqra Register@0, Ax@8
 9062h TL2 sqra r6, Ax@8, Unused1@0
 D4FFh TL sub MemImm16@16, Ax@8
 AE00h TL sub MemImm8@0, Ax@8
 8EC0h TL sub Imm16@16, Ax@8
 CE00h TL sub Imm8u@0, Ax@8
 D4DFh TL sub MemR7Imm16@16, Ax@8
 4E00h TL sub MemR7Imm7s@0, Ax@8
 8E80h TL sub MemRn@0, Ax@8 || Rn@0stepZIDS@3
 8EA0h TL sub RegisterP0@0, Ax@8
 8A61h TL2 sub Ab@3, Bx@8
 8861h TL2 sub Bx@4, Ax@3
 8064h TL2 sub MemR01@8, sv, Abh@3 || add MemR01@8offsZI@0, sv, Abl@3
 || mov MemR45@8, sv || R01@8stepII2@0, R45@8stepII2@1
 5DE0h TL2 sub MemR04@1, sv, Abh@2 || add MemR04@1offsZI@0, sv, Abl@2
 || R04@1stepII2@0
 6FC0h TL2 sub MemR45@2, MemR01@2, Abh@3
 || add MemR45@2offsZI@1, MemR01@2offsZI@0, Abl@3
 || R01@2stepII2@0, R45@2stepII2@1
 6FE0h TL2 sub MemR45@2, MemR01@2, Abh@3
 || sub MemR45@2offsZI@1, MemR01@2offsZI@0, Abl@3
 || R01@2stepII2@0, R45@2stepII2@1
 5D80h TL2 sub MemR45@2, sv, Abh@3 || add MemR45@2offsZI@1, sv, Abl@3
 || mov MemR01@2, sv || R01@2stepII2@0, R45@2stepII2@1
 5DC2h TL2 sub p0, p1, Ab@2
 D4B9h TL2 sub p1, Ax@8
 8FD0h TL2 sub Px@1, Bx@0
 D38Fh TL2 sub r6, Ax@4
 80C6h TL2 sub3 p0, p1, Ab@10
 82C6h TL2 sub3a p0, p1, Ab@10
 83C6h TL2 sub3aa p0, p1, Ab@10
 5DC3h TL2 suba p0, p1, Ab@2
 B600h TL subh MemImm8@0, Ax@8
 9680h TL subh MemRn@0, Ax@8 || Rn@0stepZIDS@3
 96A0h TL subh Register@0, Ax@8
 5E23h TL2 subh r6, Ax@8
 B800h TL subl MemImm8@0, Ax@8
 9880h TL subl MemRn@0, Ax@8 || Rn@0stepZIDS@3
 98A0h TL subl Register@0, Ax@8
 5E22h TL2 subl r6, Ax@8
 EF00h TL subv Imm16@16, MemImm8@0
 8EE0h TL subv Imm16@16, MemRn@0 || Rn@0stepZIDS@3
 8FE0h TL subv Imm16@16, Register@0
 47BFh TL2 subv Imm16@16, r6
 4980h TL swap SwapTypes4@0
 0020h TL trap ;software interrupt
 A800h TL tst0 Axl@8, MemImm8@0
 8880h TL tst0 Axl@8, MemRn@0 || Rn@0stepZIDS@3
 88A0h TL tst0 Axl@8, Register@0
 E900h TL tst0 Imm16@16, MemImm8@0
 88E0h TL tst0 Imm16@16, MemRn@0 || Rn@0stepZIDS@3
 89E0h TL tst0 Imm16@16, Register@0
 D38Ch TL2 tst0 Axl@4, r6
 47BCh TL2 tst0 Imm16@16, r6
 9470h TL2 tst0 Imm16@16, SttMod@0
 AA00h TL tst1 Axl@8, MemImm8@0 Implied Not
 8A80h TL tst1 Axl@8, MemRn@0 Implied Not || Rn@0stepZIDS@3
 8AA0h TL tst1 Axl@8, Register@0 Implied Not
 EB00h TL tst1 Imm16@16, MemImm8@0 Implied Not
 8AE0h TL tst1 Imm16@16, MemRn@0 Implied Not || Rn@0stepZIDS@3
 8BE0h TL tst1 Imm16@16, Register@0 Implied Not
 D38Dh TL2 tst1 Axl@4, r6 Implied Not
 47BDh TL2 tst1 Imm16@16, r6 Implied Not
 9478h TL2 tst1 Imm16@16, SttMod@0 Implied Not
 80C1h TL2 tst4b a0l, MemR0425@10 || R0425@10stepII2D2S@8
 4780h TL2 tst4b a0l, MemR0425@2, Ax@4 || R0425@2stepII2D2S@0
 F000h TL tstb NoReverse, Implied Not MemImm8@0, Imm4bitno@8
 9020h TL tstb NoReverse, Implied Not MemRn@0, Imm4bitno@8 || Rn@0stepZIDS@3
 9000h TL tstb NoReverse, Implied Not Register@0, Imm4bitno@8
 9018h TL2 tstb NoReverse, Implied Not r6, Imm4bitno@8 ;override tstb a0,Imm4
 0028h TL2 tstb NoReverse, Implied Not SttMod@0, Imm4bitno@16, Unused12@20
 5F45h TL2 vtrclr vtr0 ;vtr0=0 ;for Viterbi decoding...
 5F47h TL2 vtrclr vtr0, vtr1 ;vtr0=0, vtr1=0 ;(saved C/C1 carry flags)
 5F46h TL2 vtrclr vtr1 ;vtr1=0
 D383h TL2 vtrmov Axl@4 ;Axl=(vtr1 and FF00h)+(vtr0/100h)
 D29Ah TL2 vtrmov vtr0, Axl@0 ;Axl=vtr0
 D69Ah TL2 vtrmov vtr1, Axl@0 ;Axl=vtr1
 D781h TL2 vtrshr ;vtr0=vtr0/2+C*8000h, vtr1=vtr1/2+C1*8000h
 D4FAh TL xor MemImm16@16, Ax@8
 A400h TL xor MemImm8@0, Ax@8
 84C0h TL xor Imm16@16, Ax@8
 C400h TL xor Imm8u@0, Ax@8
 D4DAh TL xor MemR7Imm16@16, Ax@8
 4400h TL xor MemR7Imm7s@0, Ax@8
 8480h TL xor MemRn@0, Ax@8 || Rn@0stepZIDS@3
 84A0h TL xor RegisterP0@0, Ax@8
 D38Ah TL2 xor r6, Ax@4
 8800h TL undefined Unused5@0, Unused1@8 ;(mpy/mpys without A in bit11)
 8820h TL undefined Unused5@0, Unused1@8 ;(mpy/mpys without A in bit11)
 8840h TL undefined Unused5@0, Unused1@8 ;(mpy/mpys without A in bit11)
 D800h TL undefined Unused7@0, Unused1@8 ;(mpy/mpys without A in bit11)
 9B80h TL undefined Unused6@0 ;(sqr without A in bit8)
 BB00h TL undefined Unused8@0 ;(sqr without A in bit8)
 E800h TL undefined Unused8@0 ;(mpy without A in bit11)
 5EA1h TL2 undefined Unused1@1 ;(mpy/mpys without A in bit11)
 5DFCh TL2 undefined
 8CDEh TL2 undefined
 D3C1h TL2 undefined
 5EB4h TL2 undefined Unused2@0 |
| --- |

| DSi TeakLite II Operand Encoding |
| --- |

**Syntax Notes**

The official Teak syntax specifies all operands in "source,dest" order, that's
opposite of most other ASM languages which use "dest,source" order (except
68000 processors, which the Teak is apparently inspired on). One exception are
instructions tagged "NoReverse" in the opcode list: These ones do have the
operands ordered as how one would usually expect them.

Operands tagged as "Implied xx" are effectively used by the hardware, although
the official syntax doesn't specify them in source code. On the other hand,
operands tagged as "Bogus xx" are specified in official source code syntax,
although the hardware doesn't actually use that operands in that form.

The nocash (dis-)assembler syntax reverses the operand order (except those
flagged as NoReverse), removes Bogus operands, and inserts Implied operands.
Moreover, immediates and memory operands are specified differently...

**Memory Operands**

```

| name native nocash
 MemRn (Rn) [Rn]
 MemSp (sp) [sp]
 ProgMemRn (Rn) [code:movpd:Rn]
 ProgMemAxl (Axl) [code:movpd:Axl]
 ProgMemAx (Ax) [code:Ax]
 ProgMemAx_.. (Ax),(Ax+) [code:Ax]:[code:Ax+]
 MemImm8 0xNN [page:NNh]
 MemImm16 [##0xNNNN] [NNNNh]
 MemR7Imm7s (r7+#0xNN), (r7+#-NNN) [r7+/-NNh]
 MemR7Imm16 (r7+##0xNNNN) [r7+NNNNh] |
| --- |

**Immediates/Addresses**

```

| Address18 0xNNNNN NNNNNh ;for bkrep/br/call
 Address16 0xNNNN NNNNh ;for bkrep
 RelAddr7 0xNNNN NNNNh ;for jmp
 ImmN: #0xNNNN NNNNh
 ImmNs: #0xNN, #-NNN +/-NNh
 Imm16: ##0xNNNN NNNNh
 Imm4bitno: ... 1 shl N
 ConstZero <implied> 0000h
 Const1 <implied> 0001h
 Const4 <implied> 0004h
 Const8000h <implied> 8000h |
| --- |

**Operand Encoding**

Below shows the binary encoding for registers/conditions. For example, "Ab@10"
in the opcode list can be "b0,b1,a0,a1" encoded in bit10-11. Some instructions
re-use the same bitfield for multiple operands (eg. when performing two
operations on the SAME operand, or when expecting DIFFERENT operands: for
"Ax@12,Ax@not12" one of the registers must "a0", and the other must be "a1").

```

| Register: RegisterP0: Ax: Axl: Axh: Px:
 00: r0 00: r0 0: a0 0: a0l 0: a0h 0: p0
 01: r1 01: r1 1: a1 1: a1l 1: a1h 1: p1
 02: r2 02: r2
 03: r3 03: r3 Bx: Bxl: Bxh: Ablh:
 04: r4 04: r4 0: b0 0: b0l 0: b0h 0: b0l
 05: r5 05: r5 1: b1 1: b1l 1: b1h 1: b0h
 06: r7 06: r7 2: b1l
 07: y0 07: y0 Ab: Abl: Abh: Abe: 3: b1h
 08: st0 08: st0 0: b0 0: b0l 0: b0h 0: b0e 4: a0l
 09: st1 09: st1 1: b1 1: b1l 1: b1h 1: b1e 5: a0h
 0A: st2 0A: st2 2: a0 2: a0l 2: a0h 2: a0e 6: a1l
 0B: p0h !! 0B: p0 !! 3: a1 3: a1l 3: a1h 3: a1e 7: a1h
 0C: pc 0C: pc
 0D: sp 0D: sp Cond:
 0E: cfgi 0E: cfgi 0: true ;Always ;always
 0F: cfgj 0F: cfgj 1: eq ;Equal to zero ;Z=1
 10: b0h 10: b0h 2: neq ;Not equal to zero ;Z=0
 11: b1h 11: b1h 3: gt ;Greater than zero ;M=0 and Z=0
 12: b0l 12: b0l 4: ge ;Greater or equal to zero ;M=0
 13: b1l 13: b1l 5: lt ;Less than zero ;M=1
 14: ext0 14: ext0 6: le ;Less or equal to zero ;M=1 or Z=1
 15: ext1 15: ext1 7: nn ;Normalize flag is cleared ;N=0
 16: ext2 16: ext2 8: c ;Carry flag is set ;C=1
 17: ext3 17: ext3 9: v ;Overflow flag is set ;V=1
 18: a0 18: a0 A: e ;Extension flag is set ;E=1
 19: a1 19: a1 B: l ;Limit flag is set ;L=1
 1A: a0l 1A: a0l C: nr ;R flag is cleared ;R=0
 1B: a1l 1B: a1l D: niu0 ;Input user pin 0 cleared ;IUSER0=0
 1C: a0h 1C: a0h E: iu0 ;Input user pin 0 set ;IUSER0=1
 1D: a1h 1D: a1h F: iu1 ;Input user pin 1 set ;IUSER1=1
 1E: lc 1E: lc
 1F: sv 1F: sv |
| --- |

```

| R0123457y0: Rn: ArArpSttMod: ArArp: SttMod:
 0: r0 0: r0 0: ar0 0: ar0 0: stt0
 1: r1 1: r1 1: ar1 1: ar1 1: stt1
 2: r2 2: r2 2: arp0 2: arp0 2: stt2
 3: r3 3: r3 3: arp1 3: arp1 3: reserved
 4: r4 4: r4 4: arp2 4: arp2 4: mod0
 5: r5 5: r5 5: arp3 5: arp3 5: mod1
 6: r7 ;aka rb 6: r6 ;TL2 only 6: reserved 6: reserved 6: mod2
 7: y0 ;aka y 7: r7 ;TL2 only 7: reserved 7: reserved 7: mod3
 8: stt0
 R01: R04: R45: 9: stt1 Ar: BankFlags:
 0: r0 0: r0 0:r4 A: stt2 0: ar0 01h: cfgi
 1: r1 1: r4 1:r5 B: reserved 1: ar1 02h: r4
 C: mod0 04h: r1
 R0123: R0425: R4567: D: mod1 Arp: 08h: r0
 0: r0 0: r0 0: r4 E: mod2 0: arp0 10h: r7 ;TL2
 1: r1 1: r4 1: r5 F: mod3 1: arp1 20h: cfgj ;TL2
 2: r2 2: r2 2: r6 2: arp2
 3: r3 3: r5 3: r7 3: arp3 |
| --- |

```

| SwapTypes:
 val native nocash ;meaning
 0: (a0,b0) a0,b0 ;a0 <--> b0 ;flags(a0)
 1: (a0,b1) a0,b1 ;a0 <--> b1 ;flags(a0)
 2: (a1,b0) a1,b0 ;a1 <--> b0 ;flags(a1)
 3: (a1,b1) a1,b1 ;a1 <--> b1 ;flags(a1)
 4: (a0,b0),(a1,b1) a0:a1,b0:b1 ;a0 <--> b0 and a1 <--> b1 ;flags(a0)
 5: (a0,b1),(a1,b0) a0:a1,b1:b0 ;a0 <--> b1 and a1 <--> b0 ;flags(a0)
 6: (a0,b0,a1) a1,b0,a0 ;a0 --> b0 --> a1 ;flags(a1)
 7: (a0,b1,a1) a1,b1,a0 ;a0 --> b1 --> a1 ;flags(a1)
 8: (a1,b0,a0) a0,b0,a1 ;a1 --> b0 --> a0 ;flags(a0)
 9: (a1,b1,a0) a0,b1,a1 ;a1 --> b1 --> a0 ;flags(a0)
 A: (b0,a0,b1) b1,a0,b0 ;b0 --> a0 --> b1 ;flags(a0)!
 B: (b0,a1,b1) b1,a1,b0 ;b0 --> a1 --> b1 ;flags(a1)!
 C: (b1,a0,b0) b0,a0,b1 ;b1 --> a0 --> b0 ;flags(a0)!
 D: (b1,a1,b0) b0,a1,b1 ;b1 --> a1 --> b0 ;flags(a1)!
 E: reserved reserved ;- ;-
 F: reserved reserved ;- ;- |
| --- |

**offs and step**

Memory operands with "offs" allow to read from [Rn], [Rn+1], or [Rn-1].
Operands with "step" allow to increment/decrement registers (for old TL opcodes
this is usually specified alongsides with memory operands, for new TL2 opcodes
it's typically specified as separate "||" instruction (eg. "|| Rn@0stepZIDS@3";
which can be omitted if the step is zero).

The official syntax wants "+1,-1" abbreviated to "+,-" in some cases (but not
abbreviated in other cases). step "+s" does probably refer to "stepi or stepj",
but it's rather unclear which one, maybe stepi is used for r0..r3, and stepj
for r4..r7, or vice versa... or maybe it depends on each opcode (particulary
opcodes that allow to use "Rn" (r0..r7) might use the same step in ALL cases).

```

| offsZI: ;maybe offsAr01 ?
 0: '' ;Z (zero)
 1: '+' ;I (increment)
 offsI:
 0: '+' ;I (increment)
 offsZIDZ: ;aka offsAr0123
 0: '' ;Z (zero)
 1: '+' ;I (increment)
 2: '-' ;D (decrement)
 3: '' ;Z (zero)
 stepZIDS:
 0: '' ;Z (zero)
 1: '+1' ;I (increment)
 2: '-1' ;D (decrement)
 3: '+s' ;S (add step) ;XXX ? see "stepi" and "stepj"
 modrstepZIDS:
 0: '' ;Z (zero)
 1: '+' ;I (increment)
 2: '-' ;D (decrement)
 3: '+s' ;S (add step) ;XXX ? see "stepi" and "stepj"
 stepII2D2S: ;aka stepAr0123@
 0: '+1' ;I (increment)
 1: '+2' ;I2 (increment twice)
 2: '-2' ;D2 (decrement twice)
 3: '+s' ;S (add step) ;XXX ? see "stepi" and "stepj"
 stepD2S:
 0: '-2' ;D2 (decrement twice)
 1: '+s' ' ;S (add step) ;XXX ? see "stepi" and "stepj"
 modrstepII2D2S0:
 0: '+' ;I (increment)
 1: '+2' ;I2 (increment twice)
 2: '-2' ;D2 (decrement twice)
 3: '+s0' ;S0 (add step0 ?) ;XXX ?? see "stepi0" and "stepj0"
 stepII2:
 0: '+1' ;I (increment)
 1: '+2' ;I2 (increment twice)
 modrstepI2:
 0: '+2' ;I2 (increment twice)
 modrstepD2:
 0: '-2' ;D2 (decrement twice) |
| --- |

Note: The "modr" opcodes are probably just incrementing/decrementing registers
(with optional "modulo"), although the official syntax specifies their operands
in brackets, ie. as if they were doing memory accesses.