# Dsinewdmandma

> Source: https://problemkaputt.de/gbatek.htm
> Section: Dsinewdmandma

DSi New DMA (NDMA) 
| |
| --- |

 The DSi has four New DMA channels for ARM7 and ARM9 each (eight New DMA channels in total). The old NDS-style DMA channels do still exist, too [though unknown which priority they have in relation to new channels].
 
 **4004100h - DSi - NDMAGCNT NewDMA Global Control (R/W) [00000000h]**
 
```

| 0-15 Unused (0)
 16-19 Cycle Selection (0=None, 1..15=1..16384 clks) ;1 SHL (N-1)
 20-30 Unused (0)
 31 DMA Arbitration Mode (0=NDMA0=HighestPriority, 1=RoundRobinPriority) |
| --- |

CycleSelection is used ONLY in RoundRobin mode; if so... then it does specify
the number of cycles that can be executed by ARM9 and DSP <CPUs?> during
NDMA?

**4004104h+x*1Ch - NDMAxSAD - NewDMAx Source Address (R/W) [00000000h]**

**4004108h+x*1Ch - NDMAxDAD - NewDMAx Destination Address (R/W) [00000000h]**

```

| 0-1 Unused (0)
 2-31 DMA Source/Destination Address, in 4-byte steps |
| --- |

**400410Ch+x*1Ch - NDMAxTCNT - NewDMAx Total Length for Repeats (R/W) [0]**

```

| 0-27 Total Number of Words to Transfer (1..0FFFFFFFh, or 0=10000000h)
 28-31 Unused (0) |
| --- |

Not used in "Start immediately" mode (which doesn't repeat).

Not used in "Repeat infinitely" mode (which repeats forever).

Used only in "Repeat until NDMAxTCNT" mode (for example, to define the total
size of the Camera picture).

Total Length isn't required to be a multiple of the Logical or Physical Block
Sizes (for example the DSi launcher uses Total=64h with Log=8/Phys=8; in that
case only 4 words (instead of 8 words) are transferred for the last block).

**4004110h+x*1Ch - NDMAxWCNT - NewDMAx Logical Block Size**

```

| 0-23 Number of Words to Transfer (1..00FFFFFFh, or 0=01000000h)
 24-31 Unused (0) |
| --- |

Should be a multiple of the Physical Block Size specified in NDMAxCNT.Bit16-19.

The bus will be monopolized until the selected number of words for (physical)
block transfers has completed, a single (physical) block transfer cycle will
never be split up.

**4004114h+x*1Ch - NDMAxBCNT - NewDMAx Block Transfer Timing/Interval**

```

| 0-15 Interval Timer (1..FFFFh, or 0=Infinite/TillTransferEnd)
 16-17 Prescaler (33.514MHz SHR (n*2)) ;0=33MHz, 1=8MHz, 2=2MHz, 3=0.5MHz
 18-31 Unused (0) |
| --- |

Allows to insert a delay after each (Physical?) Block.

**4004118h+x*1Ch - NDMAxFDATA - NewDMAx Fill Data**

```

| 0-31 Fill Data (can be used as Fixed Source Data for memfill's) |
| --- |

This value is used when setting NDMAxCNT.Bit13-14=3, which causes the source
data to be read directly (within 0 clock cycles) from the NDMAxFDATA (instead
of from the address specified in NDMAxSAD; in this case, the NDMAxSAD is
ignored/don't care).

**400411Ch+x*1Ch - NDMAxCNT - NewDMAx Control**

```

| 0-9 Unused (0)
 10-11 Dest Address Update (0=Increment, 1=Decrement, 2=Fixed, 3=Reserved)
 12 Dest Address Reload (0=No, 1=Reload at (logical blk?) transfer end)
 13-14 Source Address Update (0=Increment, 1=Decrement, 2=Fixed, 3=FillData)
 15 Source Address Reload (0=No, 1=Reload at (logical blk?) transfer end)
 16-19 Physical Block Size (0..0Fh=1..32768 words, aka (1 SHL n) words)
 20-23 Unused (0)
 24-28 DMA Startup Mode (00h..1Fh, see ARM7/ARM9 startup lists below)
 29 DMA Repeat Mode (0=Repeat until NDMAxTCNT, 1=Repeat infinitely)
 30 DMA Interrupt Enable (0=Disable, 1=Enable)
 31 DMA Enable/Busy (0=Disable, 1=Enable/Busy) |
| --- |

**Startup Modes for ARM9:**

```

| 00h Timer0 ;\
 01h Timer1 ; new NDMA-specific modes
 02h Timer2 ;
 03h Timer3 ;/
 04h DS Cartridge Slot 4100010h
 05h Reserved (maybe 2nd DS-Cart Slot 4102010h, or GBA slot relict?)
 06h V-Blank
 07h H-Blank (but not during V-blank)
 08h Display Sync (sync to H-blank drawing) ;Uh, what is BLANK-DRAWING ??
 09h Work RAM (what?) (=probably Main memory display, as on NDS)
 0Ah Geometry Command FIFO
 0Bh Camera ;-new NDMA-specific mode
 0Ch..0Fh Reserved
 10h..1Fh Start immediately (without repeat) |
| --- |

**Startup Modes for ARM7:**

```

| 00h Timer0 ;\
 01h Timer1 ; new NDMA-specific modes
 02h Timer2 ;
 03h Timer3 ;/
 04h DS Cartridge Slot 4100010h
 05h Reserved? (maybe 2nd DS-Cart Slot 4102010h, or GBA slot relict?)
 06h V-Blank
 07h NDS-Wifi
 08h SD/MMC (SD_DATA32_FIFO) ;\
 09h DSi-Wifi (SDIO_DATA32_FIFO) ;
 0Ah AES in (AES_WRFIFO) ; new NDMA-specific modes
 0Bh AES out (AES_RDFIFO) ;
 0Ch Microphone (MIC_DATA) ;/
 0Dh..0Fh Reserved?
 10h..1Fh Start immediately (without repeat) |
| --- |

**Start/repeat modes**

There are three different transfer modes.

**1) Start immediately (without repeat):**

the transfer ends after one Logical Block, without repeat. With single IRQ
(after last/only block).

**2) Start by Hardware events, Repeat until NDMAxTCNT:**

the transfer repeats Logical Blocks until reaching the Total Length. With
single IRQ (after last block).

**3) Start by Hardware events, Repeat infinitely:**

the transfer repeats Logical Blocks infinitely. With multiple IRQs (one IRQ
after EACH logical block).

**Read-only Effect**

There is something that can make port 4004104h..4004173h read-only. For
example, when FFh-filling all DSi registers, and then 00h-filling them, then
most DMA bits stay set (00h-filling them another time does clear them).

Maybe, during enabled transfers, ONLY the enable/busy bit is writeable?