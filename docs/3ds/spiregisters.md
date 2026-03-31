# Spiregisters

> Source: https://problemkaputt.de/gbatek.htm
> Section: Spiregisters

3DS SPI Registers 
| |
| --- |

 **SPI Registers**
 There are four SPI buses (most of them can have up to 3 devices attached):
 
```

| 1000D000h ARM9 SPI_CARD (savedate in game card SPI FLASH memory)
 10160000h ARM9/ARM11 SPI_BUS0 (0=Powerman, 1=Wifi-FLASH, 2=Touchscr/sound)
 10142000h ARM9/ARM11 SPI_BUS1 (0=Touchscr/sound with extra 3ds registers)
 10143000h ARM9/ARM11 SPI_BUS2 (unused) |
| --- |

The separate registers (at the above base addresses) are:

```

| 10xxx000h 2 SPI_MANUAL_CNT (R/W) ;\Manual Access
 10xxx002h 1/2 SPI_MANUAL_DATA (R/W) ;/
 10xxx800h 4 SPI_FIFO_CNT (R/W) ;\
 10xxx804h 4 SPI_FIFO_DONE (R/W) ;
 10xxx808h 4 SPI_FIFO_BLKLEN (R/W) ; FIFO Access
 10xxx80Ch 4 SPI_FIFO_DATA (R/W) ; (and AUTOPOLL for
 10xxx810h 4 SPI_FIFO_STATUS (R) ; status reads)
 10xxx814h 4 SPI_AUTOPOLL (R/W) ;
 10xxx818h 4 SPI_FIFO_INT_MASK (R/W) ;
 10xxx81Ch 4 SPI_FIFO_INT_STAT (R/ack);/ |
| --- |

Interrupt IDs and DMA/FIFO startup/peripheral IDs are:

```

| SPI_CARD ARM9 IF.bit23 Various (ARM9 NDMA and ARM9 XDMA)
 SPI_BUS0 ARM11 IRQ 56h ARM11 CDMA 0Fh
 SPI_BUS1 ARM11 IRQ 57h ARM11 CDMA 10h
 SPI_BUS2 ARM11 IRQ 24h ARM11 CDMA 11h |
| --- |

**Config registers**

```

| CFG11_SPI_CNT - mode select for SPI_BUS0,1,2 (Manual or Fifo)
 CFG9_CARD_CTL - mode select for SPI_CARD (Manual or Fifo or NTRCARD) |
| --- |

3DS Config - CONFIG11 Registers

3DS Config - CONFIG9 Registers

```

| ___________________________ SPI Manual Access Mode ___________________________ |
| --- |

**10xxx000h - SPI_MANUAL_CNT**

**10xxx002h - SPI_MANUAL_DATA**

Manual access mode works same as on DSi. Max transfer rate is 8MHz, 1bit mode
only, and the manual byte-by-byte transfer adds some software overload to the
transfer time.

DS Serial Peripheral Interface Bus (SPI)

One advantage is that the manual mode supports true bi-directional SPI
transfers (which aren't really needed because most SPI chips use only one
direction at a time, with dummy data in opposite direction).

```

| ____________________________ SPI FIFO Access Mode ____________________________ |
| --- |

**10xxx800h - SPI_FIFO_CNT (R/W)**

```

| 0-2 Baudrate (0=512KHz, 1=1MHz, 2=2MHz, 3=4MHz, 4=8MHz, 5..7=16MHz)
 3-5 Unused (0)
 6-7 SPI_BUS0-1: Device Select (0..2, or 3=Hangs/Reads as 2)
 SPI_BUS2: Unused (0)
 SPI_CARD: Unused (0)
 8-11 Unused (0)
 12 Bus Mode (0=1bit, 1=4bit)
 13 Transfer Direction (0=Read, 1=Write)
 14 Unused (0)
 15 Start Transfer (0=Idle/Ready, 1=Start/Busy)
 16-31 Unused (0) |
| --- |

CARD: This register seems to have a bug where the lower 8 bits are shifted up
by 16 when reading this register. Uh, really?

**10xxx804h - SPI_FIFO_DONE (R/W) (or R when busy?)**

```

| 0 Chip Select (0=Not Selected, 1=Selected)
 1-31 Unused (0) |
| --- |

Select occurs automatically (when starting a transfer via SPI_FIFO_CNT).
However, deselect must be done manually (by writing 0 to SPI_FIFO_DONE after
last data block). This allows to keep the chip selected during multiple
transfer blocks (including blocks with different data direction, eg. WriteCmd +
ReadData).

**10xxx808h - SPI_FIFO_BLKLEN (R/W) (or R when busy?)**

```

| 0-20 Transfer length (1..1FFFFFh bytes, 0=?)
 21-31 Unused (0) |
| --- |

**10xxx80Ch - SPI_FIFO_DATA (R/W)**

```

| 0-31 32bit FIFO for reading/writing data |
| --- |

**10xxx810h - SPI_FIFO_STATUS (R)**

```

| 0 Whatever FIFO busy... or FIFO full (0=Not full, 1=Full)
 1-31 unknown/unspecified |
| --- |

"At transfer start and every 32 bytes the FIFO becomes busy."

Uh, unknown if this refers to Send FIFO or Receive FIFO. For the latter having
a Empty flag would be most useful.

Might be also a generic "data request" flag, rather than a "full/busy" flag?

**10xxx814h - SPI_AUTOPOLL (R/W)**

```

| 0-7 Command byte (eg. 05h=RDSR, aka FLASH read status)
 8-15 Unused (0)
 16-19 Timeout (0..10 = 1ms,2ms,4ms,8ms,..,512ms,1024ms, or 11..15=Never)
 20-23 Unused (0)
 24-26 Bit number (0..6=Bit0..6, or 7=Bugged?, always "ready" after 1st poll?)
 27-29 Unused (0)
 30 Bit value (0=WaitUntilZero, 1=WaitUntilSet)
 31 Start Poll (0=Idle/Ready, 1=Start/Busy) |
| --- |

Autopoll does repeatedly transfer command+reply bytes, until reaching the
desired reply bit state (or until reaching timeout). Autopoll uses the device
number and baudrate selected in SPI_FIFO_CNT.

**10xxx818h - SPI_FIFO_INT_MASK (R/W) (or R when busy?)**

```

| 0 Transfer Finished Interrupt Disable (0=Enable, 1=Disable)
 1 Autopoll Success Interrupt Disable (0=Enable, 1=Disable)
 2 Autopoll Timeout Interrupt Disable (0=Enable, 1=Disable)
 3 Unknown (R/W) (?)
 4-31 Unused (0) |
| --- |

**10xxx81Ch - SPI_FIFO_INT_STAT (R/ack)**

```

| 0 Transfer Finished Flag (0=No, 1=Yes/IRQ) (write 1 to clear)
 1 Autopoll Success Flag (0=No, 1=Yes/IRQ) (write 1 to clear)
 2 Autopoll Timeout Flag (0=No, 1=Yes/IRQ) (write 1 to clear)
 3 Unknown (usually 0, but might be something related to INT_MASK.bit3 ?)
 4-31 Unused (0) |
| --- |

Bit0 also fires on each autopoll try? No: it does not fire on autopoll.