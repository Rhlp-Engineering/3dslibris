# Dmacorelinkdmaregistersummary

> Source: https://problemkaputt.de/gbatek.htm
> Section: Dmacorelinkdmaregistersummary

3DS DMA - Corelink DMA Register Summary 
| |
| --- |

 **Corelink DMA Engines**
 
```

| 1000C000h ARM9 XDMA CoreLink DMA-330 r0p0 (four channels)
 10200000h ARM11 CDMA CoreLink DMA-330 r0p0 (eight channels)
 10206000h ARM11 CDMA CoreLink DMA-330 r1p1 (eight channels) (New3DS only)
 10300000h ARM11 DMA region (aka FIFOs are here?) |
| --- |

The New3DS has two DMA controllers on ARM11 side (the r0p0 is used by bootrom,
and, after bootrom, everything is usually done by r1p1).

**DMAC Control Registers**

```

| 10xxx000h DSR DMA Manager Status Register (R)
 10xxx004h DPC DMA Manager Program Counter (R)
 10xxx008h-01Ch - Reserved
 10xxx020h INTEN Interrupt Enable Register (R/W)
 10xxx024h INT_EVENT_RIS Event-Interrupt Raw Status Register (R)
 10xxx028h INTMIS Interrupt Status Register (R)
 10xxx02Ch INTCLR Interrupt Clear Register (W)
 10xxx030h FSRD DMA Manager Fault Status Register (R)
 10xxx034h FSRC DMA Channel(s) Fault Status Register (R)
 10xxx038h FTRD DMA Manager Fault Type Register (R)
 10xxx03Ch - Reserved
 10xxx040h+n*4 FTRn DMA channel n Fault type (R)
 10xxx060h-0FCh - Reserved |
| --- |

**DMAC Channel Thread Status Registers**

```

| 10xxx100h+n*8 CSRn DMA channel n Channel status (R)
 10xxx104h+n*8 CPCn DMA channel n Channel PC (R)
 10xxx140h-3FCh - Reserved |
| --- |

**DMAC AXI Status and Loop Counter Registers**

```

| 10xxx400h+n*20h SARn DMA channel n Source address (R)
 10xxx404h+n*20h DARn DMA channel n Destination address (R)
 10xxx408h+n*20h CCRn DMA channel n Channel control (R)
 10xxx40Ch+n*20h LC0_n DMA channel n Loop counter 0 (R)
 10xxx410h+n*20h LC1_n DMA channel n Loop counter 1 (R)
 10xxx414h+n*20h - Reserved
 10xxx418h+n*20h - Reserved
 10xxx41Ch+n*20h - Reserved
 10xxx500h-CFCh - Reserved |
| --- |

**DMAC Debug Registers (debugging is meant to be synonym for "start execution")**

```

| 10xxxD00h DBGSTATUS Debug Status Register (R)
 10xxxD04h DBGCMD Debug Command Register (W) (start INST code)
 10xxxD08h DBGINST0 Debug Instruction-0 (W) (code[2]+ch+thread)
 10xxxD0Ch DBGINST1 Debug Instruction-1 (W) (byte[4])
 10xxxD10h-DFCh - Reserved |
| --- |

**DMAC Configuration Registers**

```

| 10xxxE00h CR0 Config Register 0, Misc Info (R)
 10xxxE04h CR1 Config Register 1, Cache Info (R)
 10xxxE08h CR2 Config Register 2, Boot Address (R)
 10xxxE0Ch CR3 Config Register 3, Boot Event/Irq Security (R)
 10xxxE10h CR4 Config Register 4, Boot Peripheral Security (R)
 10xxxE14h CRD DMA Configuration Register (R)
 10xxxE18h-E7Ch - Reserved
 10xxxE80h WD Watchdog Register (R/W) (rev1 only)
 10xxxE84h-FDCh - Reserved |
| --- |

**Peripheral and component identification register**

```

| 10xxxFE0h-FECh periph_id_n Peripheral Identification Registers 0-3 (R)
 10xxxFF0h-FFCh pcell_id_n Component Identification Registers 0-3 (R) |
| --- |

**CR0 values on 3DS**

```

| [1000CE00h]=000167035h ;ARM9 (12 events, 8 peripherals, 4 channels)
 [10200E00h]=0001f1075h ;Old3DS/ARM11 (16 events, 18 peripherals, 8 channels)
 [10206E00h]=0003ff075h ;New3DS/ARM11 (32 events, 32 peripherals, 8 channels) |
| --- |