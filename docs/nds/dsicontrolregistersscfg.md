# Dsicontrolregistersscfg

> Source: https://problemkaputt.de/gbatek.htm
> Section: Dsicontrolregistersscfg

DSi Control Registers (SCFG) 
| |
| --- |

 **4004000h - DSi9 - SCFG_A9ROM - ROM Status (R) [0000h]**
 
```

| 0 ARM9 BIOS Upper 32K half of DSi BIOS (0=Enabled, 1=Disabled)
 1 ARM9 BIOS for NDS Mode (0=DSi BIOS, 1=NDS BIOS)
 2-15 Unused (0)
 16-31 Unspecified (0) |
| --- |

Possible values are:

```

| 00h DSi ROM mapped at FFFFxxxxh, full 64K enabled (during bootstage 1 only)
 01h DSi ROM mapped at FFFFxxxxh, lower 32K only
 03h NDS ROM mapped at FFFFxxxxh (internal setting)
 00h NDS ROM mapped at FFFFxxxxh (visible setting due to SCFG_EXT9.bit31=0) |
| --- |

Checking for A9ROM=01h is common for detecting if the console is a "DSi console
running in DSi mode".

**4004000h - DSi7 - SCFG_ROM - ROM Control (R/W, Set-Once)**

```

| 0 ARM9 BIOS Upper 32Kbyte of DSi BIOS (0=Enabled, 1=Disabled) (FFFF8xxxh)
 1 ARM9 BIOS for NDS Mode (0=DSi BIOS, 1=NDS BIOS)(FFFF0xxxh)
 2-7 Unused (0)
 8 ARM7 BIOS Upper 32Kbyte of DSi BIOS (0=Enabled, 1=Disabled) (0008xxxh)
 9 ARM7 BIOS for NDS Mode (0=DSi BIOS, 1=NDS BIOS) (0000xxxh)
 10 Access to Console ID registers (0=Enabled, 1=Disabled) (4004Dxxh)
 11-15 Unused (0)
 16 Unknown, used by bootrom, set to 0 (0=Maybe start ARM9 ?)
 17-31 Unused (0) |
| --- |

Bits in this register can be set once (cannot be changed back from 1 to 0).

Don't change bit1 while executing IRQs or SWI functions on ARM9 side.

The System Menu sets bit10 shortly before starting any Cartridges or DSiware
files (except System Base Tools) (for NDS mode, after having set bit10, it's
also setting bit1 and bit9).

**4004004h - DSi9 - SCFG_CLK - New Block Clock Control (R/W) [0084h]**

```

| 0 ARM9 CPU Clock (0=NITRO/67.03MHz, 1=TWL/134.06MHz) (TCM/Cache)
 1 Teak DSP Block Clock (0=Stop, 1=Run)
 2 Camera Interface Clock (0=Stop, 1=Run)
 3-6 Unused (0)
 7 New Shared RAM Clock (0=Stop, 1=Run) (set via ARM7) (R)
 8 Camera External Clock (0=Disable, 1=Enable) ("outputs at 16.76MHz")
 9-15 Unused (0)
 16-31 See below (Port 4004006h, SCFG_RST) |
| --- |

Change ARM9 clock only from code within ITCM (and wait at least 8 cycles before
accessing any non-ITCM memory).

Disable the corresponding modules before stopping their clocks.

**4004004h - DSi7 - SCFG_CLK7 (R/W)**

```

| 0 SD/MMC Clock (0=Stop, 1=Run) (should be same as SCFG_EXT7.bit18)
 1 Unknown/used (0=Stop, 1=Run) (backlight goes off when cleared?)
 2 Unknown/used (0=Stop, 1=Run) (unknown effect?)
 3-6 Unused (0)
 7 New Shared RAM Clock (0=Stop, 1=Run)
 8 Touchscreen Clock (0=Stop, 1=Run) (needed for touchscr input)
 9-15 Unused (0)
 16-31 See below (Port 4004006h, SCFG_JTAG) |
| --- |

**4004006h - DSi9 - SCFG_RST - New Block Reset (R/W) [0000h]**

```

| 0 Teak DSP Block Reset (0=Apply Reset, 1=Release Reset)
 1-15 Unused (0) |
| --- |

**4004006h - DSi7 - SCFG_JTAG - Debugger Control (R/W? or Write-ONCE-only?)**

```

| 0 ARM7SEL (set when debugger can do ARM7 debugging)
 1 CPU JTAG Enable
 2-7 Unused (0)
 8 DSP JTAG Enable
 9-15 Unused (0) |
| --- |

Initialized as so: if SCFG_OP=2 then SCFG_JTAG=0102h, elseif SCFG_OP=1 then
SCFG_JTAG=0103h, entrypoint=0, endif.

**4004008h - DSi9 - SCFG_EXT9 - Extended Features (R/W) [8307F100h]**

```

| 0 Revised ARM9 DMA Circuit (0=NITRO, 1=Revised)
 1 Revised Geometry Circuit (0=NITRO, 1=Revised)
 2 Revised Renderer Circuit (0=NITRO, 1=Revised)
 3 Revised 2D Engine Circuit (0=NITRO, 1=Revised)
 4 Revised Divider Circuit (0=NITRO, 1=Revised)
 5-6 Unused (0)
 7 Revised Card Interface Circuit (0=NITRO, 1=Revised)
 8 Extended ARM9 Interrupts (0=NITRO, 1=Extended)
 9-11 Unused (0)
 12 Extended LCD Circuit (0=NITRO, 1=Extended)
 13 Extended VRAM Access (0=NITRO, 1=Extended)
 14-15 Main Memory RAM Limit (0..1=4MB/DS, 2=16MB/DSi, 3=32MB/DSiDebugger)
 16 Access to New DMA Controller (0=Disable, 1=Enable) (40041xxh)
 17 Access to Camera Interface (0=Disable, 1=Enable) (40042xxh)
 18 Access to Teak DSP Block (0=Disable, 1=Enable) (40043xxh)
 19-23 Unused (0)
 24 Access to 2nd NDS Cart Slot (0=Disable, 1=Enable) (set via ARM7) (R)
 25 Access to New Shared WRAM (0=Disable, 1=Enable) (set via ARM7) (R)
 26-30 Unused (0)
 31 Access to SCFG/MBK registers (0=Disable, 1=Enable) (4004000h-4004063h) |
| --- |

Bit24-25 are READ-ONLY (showing state set via ARM7 side). Note: Official specs
for Bit24-25 are nonsense.

Default settings seem to be:

```

| 8307F100h for DSi firmware, DSi cartridges and DSiware
 03000000h for NDS cartridges (and DSiware in NDS mode, eg. Pictochat) |
| --- |

Main RAM mapping depending on bit14-15:

```

| Mode 2000000h-2FFFFFFh C000000h-CFFFFFFh D000000h-DFFFFFFh
 4MB (0 or 1) 1st 4MB (+mirrors) Zerofilled Zerofilled
 16MB (2) 1st 16MB 1st 16MB (mirror) 1st 16MB (mirror)
 32MB (3) 1st 16MB 1st 16MB (mirror) Open bus (or 2nd 16MB) |
| --- |

SCFG_EXT9.bit14-15 affect the Main RAM mapping on <both> ARM9 and ARM7
side (that, at least AFTER games have been booted, however, there's a special
case DURING boot process: For NDS games, the firmware switches to 4MB mode on
ARM9 side, whilst ARM7 is still relocating memory from the 16MB area at the
same time - unknown how that is working exactly, maybe ARM7 isn't affected by
SCFG_EXT9 setting until ARM7 has configured/disabled its own SCFG_EXT7
register).

The 32MB mode requires an extra RAM chip (present in DSi debug version only;
DSi retail consoles return 16bit open bus values instead of extra memory). RAM
Size/Openbus detection is conventionally done by trying to read/write a BYTE at
[0DFFFFFAh].

**4004008h - DSi7 - SCFG_EXT7 - Extended Features (R/W)**

```

| 0 Revised ARM7 DMA Circuit (0=NITRO, 1=Revised)
 1 Revised Sound DMA (0=NITRO, 1=Revised)
 2 Revised Sound (0=NITRO, 1=Revised)
 3-6 Unused (0)
 7 Revised Card Interface Circuit (0=NITRO, 1=Revised) (set via ARM9) (R)
 8 Extended ARM7 Interrupts (0=NITRO, 1=Extended) (4000218h)
 9 Extended SPI Clock (8MHz) (0=NITRO, 1=Extended) (40001C0h)
 10 Extended Sound DMA ? (0=NITRO, 1=Extended) (?)
 11 Undocumented/Unknown ?? (0=NITRO, 1=Extended) (?)
 12 Extended LCD Circuit (0=NITRO, 1=Extended) (set via ARM9) (R)
 13 Extended VRAM Access (0=NITRO, 1=Extended) (set via ARM9) (R)
 14-15 Main Memory RAM Limit (0..1=4MB, 2=16MB, 3=32MB) (set via ARM9) (R)
 16 Access to New DMA Controller (0=Disable, 1=Enable) (40041xxh)
 17 Access to AES Unit (0=Disable, 1=Enable) (40044xxh)
 18 Access to SD/MMC registers (0=Disable, 1=Enable) (40048xxh-40049xxh)
 19 Access to SDIO Wifi registers (0=Disable, 1=Enable) (4004Axxh-4004Bxxh)
 20 Access to Microphone regs (0=Disable, 1=Enable) (40046xxh)
 21 Access to SNDEXCNT register (0=Disable, 1=Enable) (40047xxh)
 22 Access to I2C registers (0=Disable, 1=Enable) (40045xxh)
 23 Access to GPIO registers (0=Disable, 1=Enable) (4004Cxxh)
 24 Access to 2nd NDS Cart Slot (0=Disable, 1=Enable) (40021xxh)
 25 Access to New Shared WRAM (0=Disable, 1=Enable) (3xxxxxxh)
 26-27 Unused (0)
 28 Undocumented/Unknown (0=???, 1=Normal) (?)
 29-30 Unused (0)
 31 Access to SCFG/MBK registers (0=Disable, 1=Enable) (4004000h-4004063h) |
| --- |

Bit7,12-15 are READ-ONLY (showing state set via ARM9 side).

Default settings seem to be:

```

| 93FFFB06h for DSi Firmware (Bootcode and SysMenu/Launcher)
 13FFFB06h for DSiware (eg. SysSettings, Flipnote, PaperPlane)
 13FBFB06h for DSi Cartridges (eg. System Flaw) (bit18=0=sdmmc off)
 12A03000h for NDS cartridges (and DSiware in NDS mode, eg. Pictochat) |
| --- |

Bits 0,1,2,10,18,31 are taken from carthdr[1B8h].

**4004010h - DSi9 - SCFG_MC - NDS Slot Memory Card Interface Status (R)**

**4004010h - DSi7 - SCFG_MC - NDS Slot Memory Card Interface Control (R/W)**

```

| 0 1st NDS Slot Game Cartridge (0=Inserted, 1=Ejected) (R)
 1 1st NDS Slot Unknown/Unused (0)
 2-3 1st NDS Slot Power State (0=Off, 1=On+Reset, 2=On, 3=RequestOff) (R/W)
 4 2nd NDS Slot Game Cartridge (always 1=Ejected) ;\DSi (R)
 5 2nd NDS Slot Unknown/Unused (0) ; prototype
 6-7 2nd NDS Slot Power State (always 0=Off) ;/relict (R/W)
 8-14 Unknown/Undocumented (0)
 15 Swap NDS Slots (0=Normal, 1=Swap) (R/W)
 16-31 ARM7: See Port 4004012h, ARM9: Unspecified (0) |
| --- |

Note: Additionally, the NDS slot Reset pin can be toggled (via ROMCTRL.Bit29;
that bit is writeable on ARM7 side on DSi; which wasn't supported on NDS).

Power state values:

```

| 0=Power is Off
 1=Power On and force Reset (shall be MANUALLY changed to state=2)
 2=Power On
 3=Request Power Off (will be AUTOMATICALLY changed to state=0) |
| --- |

cart_power_on: (official/insane 1+10+27+120ms, but also works with 1+1+0+1ms)

```

| wait until state<>3 ;wait if pwr off busy
 exit if state<>0 AND no_reset_wanted ;exit if already on & no reset wanted
 wait 1ms, then set state=1 ;pwr on & force reset
 wait 10ms, then set state=2 ;pwr on normal state ;better: 1ms
 wait 27ms, then set ROMCTRL=20000000h ;release reset pin ;better: 0ms
 wait 120ms (or 270ms on 3DS) ;more insane delay? ;better: 1ms/20ms
 ;note: the last delay (after releasing reset) can be 1ms for most carts,
 ;except DSi NAND carts do require 20ms (eg. Face Training)
 ;XXX other day: needs MORE than 20ms (30ms works), temperature related?? |
| --- |

cart_power_off: (official 150ms, when using default [4004014h]=264Ch)

```

| wait until state<>3 ;wait if pwr off busy
 exit if state<>2 ;exit if already off
 set state=3 ;request pwr off
 exit unless you want to know when below pointless delay has ellapsed
 wait until state=0 ;default=150ms ;wait until pwr off ;better: skip |
| --- |

Power Off is also done automatically by hardware when ejecting the cartridge.

The Power On sequence does reset ROMCTRL.bit29=0 (reset signal).

Bit15 swaps ports 40001A0h-40001BFh and 4100010h with 40021A0h-40021BFh? and
4102010h?, the primary purpose is mapping the 2nd Slot to the 4xx0xxxh
registers (for running carts in 2nd slot in NDS mode; which of course doesn't
work because the 2nd slot connector isn't installed), theoretically it would
also allow to access the 1st slot via 4xx2xxxh registers (however, that doesn't
seem to be fully implemented, cart reading does merely reply FFh's (cart
inserted) or 00h's (no cart)). 4102010h can be read by manually polling DRQ in
40021A4h.bit23, and probably by NDMA (but not by old DMA which has no known DRQ
mode for 2nd slot).

**4004012h - DSi7 - SCFG_CARD_INSERT_DELAY (usually 1988h = 100ms) (R/W)**

**4004014h - DSi7 - SCFG_CARD_PWROFF_DELAY (usually 264Ch = 150ms) (R/W)**

```

| 0-15 Delay in 400h cycle units (at 67.027964MHz) ;max FFFFh=ca. 1 second |
| --- |

Usually set to 1988h/264Ch by firmware. Power up default is FFFFh/FFFFh, with
that setting it takes about 1 second to sense inserted carts (after power-up,
or after cart-insert).

Insert delay defines the time until SCFG_MC.bit0 reacts upon cart insert, and
until cart access works (cart eject works instantly regardless of the delay
setting). The 3DS bootrom uses 051Dh=20ms, which may be good to avoid switch
bounce. The DSi/3DS firmwares use 1988h=100ms, which might be reasonable if the
switch triggers too early.

Power Off delay defines how fast SCGF_MC.bit2-3 are changing from state=3
(request power off) to state=0 (power off). The 3DS bootrom uses 051Dh=20ms,
the DSi/3DS firmwares use 264Ch=150ms, which is both kinda pointless. If the
VCC pin is kept powered during that time (?) then the delay might help to
finish FLASH writes (which would make sense only if there was data written to
cartridge).

**4004020h - DSi7 - SCFG_WL - Wireless Disable (R/W)**

```

| 0 OFFB, related to Wifi Enable flag from TWLCFGn.dat and HWINFO_S files
 1-15 Unknown/unused (0) |
| --- |

Unknown what that does. SDIO Wifi seems to work regardless of that bit.

**4004024h - DSi7 - SCFG_OP - Debugger Type (R)**

```

| 0-1 Debug Hardware Type (0=Retail, other=debug variants)
 2-3 Unknown/unused (0)
 4 Unknown (maybe used, since it isn't masked & copied to RAM)
 5-15 Unknown/unused (0) |
| --- |

Changing this register would theoretically allow to install the debug firmware
on retail consoles, however, it's unknown where bit0-1 come from (possibly from
an internal PROM in the TWL CPU, solder pads underneath of the TWL CPU, or even
from a physically different TWL CPU chip version).