# Configaxiregisters

> Source: https://problemkaputt.de/gbatek.htm
> Section: Configaxiregisters

3DS Config - AXI Registers 
| |
| --- |

 **PrimeCell High-Performance AXI Bus Matrix (HPM) (PL301) Revision: r1p2**
 This is something for configuring interactions between Master Interfaces (MIs) and Slave Interfaces (SIs). The datasheet contains only meaningless blurb in a too-big-too-fail language. Guessing between the lines, Master does probably refer to CPUs and DMA controllers, and Slave might refer to Memory Chips.
 
```

| 1020F000h-3FCh Unused (0) (-)
 1020F400h+MI*20h QoS Tidemark for Master MI=00h (R/W)
 1020F404h+MI*20h QoS Access Control for Master MI=00h (R/W)
 1020F408h+MI*20h AR Channel Arbitration value for MI=00h..NumMI-1 (R/W)
 1020F40Ch+MI*20h AW Channel Arbitration value for MI=00h..NumMI-1 (R/W)
 1020F800h-FBCh Reserved (0) (-)
 1020FFC0h PrimeCell Configuration Register 0 NumSI's (07h/0Ah) (R)
 1020FFC4h PrimeCell Configuration Register 1 NumMI's (11h/16h) (R)
 1020FFC8h PrimeCell Configuration Register 2 Zero (00h) (R)
 1020FFCCh PrimeCell Configuration Register 3 Zero (00h) (R)
 1020FFD0h-FDCh Reserved (0) (-)
 1020FFE0h-FECh PrimeCell Peripheral Register 0,1,2,3 (01h,13h,x4h,00h) (R)
 1020FFF0h-FFCh PrimeCell ID Register 0,1,2,3 (0Dh,F0h,05h,B1h) (R) |
| --- |

These registers allow to read some fixed settings, and to change a few variable
settings (there must be a lot more fixed internal settings assigned at
manufacturing time, but one cannot read or change them).

The 3DS seems to support 1020F400h/1020F404h for Master 0 only.

Whilst 1020F408h/1020F40Ch are supported for Master 0 and up.

**1020F400h+MI*20h - QoS Tidemark for Master MI=00h (R/W)**

```

| 0-6 Max number of outstanding transactions before activating QoS (0..7Fh)
 7-31 Unused (0) |
| --- |

A value of 00h does completely disable QoS (instead of instantly triggering it
upon 0 outstanding transactions). QoS is short for Quality of Service (whatever
than means).

**1020F404h+MI*20h - QoS Access Control for Master MI=00h (R/W)**

```

| 0-6 Permit Slave 0-6 to use reserved slots (1=Yes) ;\Old3DS mode
 7-31 Unused (0) ;/
 0-9 Permit Slave 0-9 to use reserved slots (1=Yes) ;\New3DS mode
 10-31 Unused (0) ;/ |
| --- |

**1020F408h+MI*20h AR Channel Arbitration value for MI=00h..NumMI-1 (R/W)**

**1020F40Ch+MI*20h AW Channel Arbitration value for MI=00h..NumMI-1 (R/W)**

Arbitration for AXI read (AR) and AXI write (AW) address channel signals.

The meaning of these register depends on the chip-configuration. There are
three possible modes for each MI (of which, the 3DS uses LRG for MI=00h, and
Fixed RR for MI=01h and up).

```

| Programmable Least Recently Granted (LRG) arbitration (3DS: used for MI=00h)
 Write ii00pp00h ;set priority for interface ;-write
 Write FF0000iih ;select interface for reading ;\read
 Read 0000ppiih ;read priority for previously selected interface ;/
 Fixed Round-robin (RR) arbitration scheme (3DS: used for MI=01h..NumMI-1)
 Write xxxxxxxxh ;ignored (values are fixed) ;-write
 Write FF0000ssh ;select slot for reading ;\read
 Read 000000iih ;read interface for previously selected slot ;/
 Programmable Round-robin (RR) arbitration scheme (3DS: not used)
 Write ss0000iih ;set interface for slot ;-write
 Write FF0000ssh ;select slot for reading ;\read
 Read 000000iih ;read interface for previously selected slot ;/ |
| --- |

Whereas, the parameter bits are:

```

| pp = Priority (00h..FFh; 00h=Highest, FFh=Lowest) ;for LRG
 ss = Slot number (00h..unknown max value) ;for RR
 ii = Slave interface number (00h..NumSI-1)
 FF = Fixed code for reading (FFh)
 00 = Unused/reserved (00h) |
| --- |

The following settings exist on New3DS (the fixed values are same for AR+AW):

```

| Master Old3DS Mode New3DS Mode
 MI=00h pp pp pp pp pp pp pp -- -- -- -- pp pp pp pp pp pp pp pp pp pp --
 MI=01h -- 01 02 03 04 05 06 -- -- -- -- -- 01 02 03 04 05 06 07 08 09 --
 MI=02h 01 02 05 06 -- -- -- -- -- -- -- 01 02 05 06 08 09 -- -- -- -- --
 MI=03h 01 02 04 05 06 -- -- -- -- -- -- 01 02 04 05 06 08 09 -- -- -- --
 MI=04h 01 02 04 -- -- -- -- -- -- -- -- 01 02 04 08 -- -- -- -- -- -- --
 MI=05h 01 02 04 -- -- -- -- -- -- -- -- 01 02 04 08 -- -- -- -- -- -- --
 MI=06h 01 02 04 -- -- -- -- -- -- -- -- 01 02 04 08 -- -- -- -- -- -- --
 MI=07h 01 02 -- -- -- -- -- -- -- -- -- 01 02 08 -- -- -- -- -- -- -- --
 MI=08h 01 02 -- -- -- -- -- -- -- -- -- 01 02 08 -- -- -- -- -- -- -- --
 MI=09h 01 02 -- -- -- -- -- -- -- -- -- 01 02 08 -- -- -- -- -- -- -- --
 MI=0Ah 01 02 05 -- -- -- -- -- -- -- -- 01 02 05 08 -- -- -- -- -- -- --
 MI=0Bh -- -- -- -- -- -- -- -- -- -- -- 02 08 -- -- -- -- -- -- -- -- --
 MI=0Ch -- -- -- -- -- -- -- -- -- -- -- 08 02 -- -- -- -- -- -- -- -- --
 MI=0Dh -- -- -- -- -- -- -- -- -- -- -- 01 08 -- -- -- -- -- -- -- -- --
 MI=0Eh -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
 MI=0Fh -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
 MI=10h -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
 MI=11h -- -- -- -- -- -- -- -- -- -- -- 01 08 -- -- -- -- -- -- -- -- --
 MI=12h -- -- -- -- -- -- -- -- -- -- -- 01 08 -- -- -- -- -- -- -- -- --
 MI=13h -- -- -- -- -- -- -- -- -- -- -- 01 08 -- -- -- -- -- -- -- -- --
 MI=14h -- -- -- -- -- -- -- -- -- -- -- -- 01 02 03 04 05 06 07 08 09 --
 MI=15h -- -- -- -- -- -- -- -- -- -- -- -- 01 02 03 04 05 06 07 08 09 -- |
| --- |

Entries with value "01..09" are fixed (and have same values for AR and AW).

Entries marked "--" are reading as zero (but there seems to be no way to
distinguish between SI=00h and SI=None).

The priority values (pp) for MI=00h are ininitally 00h, but can be changed,
New3DS has 34 priority values in total (7+7 for AR+AW in Old3DS mode, and
another 10+10 for AR+AW in New3DS mode).

**1020FFC0h PrimeCell Configuration Register 0 Num SIs (R)**

**1020FFC4h PrimeCell Configuration Register 1 Num MIs (R)**

```

| 0-7 Number of Master/Slave Interfaces (MIs/SIs) (01h..20h)
 8-31 - |
| --- |

The values here change depending on whether running in Old3DS or New3DS mode.

```

| Old3DS Mode: NumMI=11h, NumSI=07h
 New3DS Mode: NumMI=16h, NumSI=0Ah |
| --- |

**1020FFC8h PrimeCell Configuration Register 2 Zero (R)**

**1020FFCCh PrimeCell Configuration Register 3 Zero (R)**

```

| 0-31 Zero |
| --- |

**1020FFE0h-FECh - PrimeCell Peripheral Register 0-3 (R)**

This region contains four 32bit registers (01h,13h,x4h,00h). One is supposed to
extract the lower 8bit of these 32bit values, and then to merge them into a
"conceptual-32bit-value" (00x41301h). And then interprete it as so:

```

| 0-11 Part number (301h=HPM)
 12-19 Designer (41h=ARM)
 20-23 Revision (1=r1p0, 2=r1p1, 3=r1p2)
 24-31 Reserved (undef) |
| --- |

This value is fixed=00341301h on New3DS (ie. r1p2, no matter if running in
Old3DS or New3DS mode).

The value on actual Old3DS is unknown (although, some consoles are reportedly
using r1p0?).

**1020FFF0h-FFCh - PrimeCell ID Register 0-3 (R)**

This region contains four 32bit registers (0Dh,F0h,05h,B1h). One is supposed to
extract the lower 8bit of these 32bit values, and then to merge them into a
"conceptual-32bit-value" (B105F00Dh). And then interprete it as so:

```

| 0-31 Component ID (B105F00Dh) (same ID as for Corelink DMA controller) |
| --- |

**Guesses on possible Master Interfaces (MIs)**

Old3DS = 11h MI's

```

| 1xARM11 (with 2 CPU cores)
 1xCDMA (with 8 channels)
 1xCSND (with 32+2 sound+capture channels)
 1xGPU (internal rendering, external to 2 LCD's, and memcopy/memfill)
 1xDSP
 1xARM9 ;\
 1xXDMA (with 4 channels) ; ARM9
 1xNDMA (with 8 channels) ;
 1xDMA (with 4 channels) ;/
 1xARM7 ;\
 1xNDMA (with 4 channels) ; ARM7
 1xDMA (with 4 channels) ; (can't really share ARM11 bus though)
 1xNDS/GBA GPU (2x 2D and 1x 3D) ;
 1xNDS/GBA Sound (with 15+2 channels) ;/ |
| --- |

New3DS = 16h MIs (five more than Old3DS)

```

| 0xNewARM11 (but with 2 more CPU cores)
 1xNewCDMA (with 8 channels)
 1xMVD
 1xLevel 2 Cache Controller |
| --- |

**Guesses on possible Slave Interfaces (SIs)**

Old3DS = 07h SI's

```

| FCRAM
 VRAM
 ARM9 RAM
 DSP RAM
 AXI RAM
 BIOS ROM(s)
 I/O Area(s) |
| --- |

New3DS = 0Ah SIs (three more than Old3DS)

```

| Extended VRAM? aka QTM?
 Extended ARM9 RAM
 Extended FCRAM |
| --- |

**Datasheets**

Reportedly "CoreLink NIC-301 r1p0" (aka NIC-301 Network Interconnect)

```
[http://infocenter.arm.com/help/topic/com.arm.doc.ddi0422a/CHDGHIID.html](http://infocenter.arm.com/help/topic/com.arm.doc.ddi0422a/CHDGHIID.html)
| DDI0422.pdf |
| --- |

But, the ID values don't match up (at least not on New3DS, it's actually: r1p2)

```

| DDI0422D_hpm_pl301_r1p2_ts.pdf |
| --- |