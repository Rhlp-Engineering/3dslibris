# Dmacorelinkdmafixedconfigurationandidregisters

> Source: https://problemkaputt.de/gbatek.htm
> Section: Dmacorelinkdmafixedconfigurationandidregisters

3DS DMA - Corelink DMA - Fixed Configuration and ID Registers 
| |
| --- |

 
```

| ________________________ DMAC Configuration Registers ________________________ |
| --- |

**10xxxE00h - CR0 - Configuration Register 0 - Misc Info (R)**

```

| 0 Peripheral Requests supported (0=None, 1=Yes, see bit12-16)
 1 boot_from_pc signal state at reset (0=Was Low, 1=Was High)
 2 boot_manager_ns signal state at reset (0=Was Low, 1=Was High)
 3 Reserved (undef)
 4-6 Number of supported DMA channels (0..7 = 1..8 channels)
 7-11 Reserved (undef)
 12-16 Number of supported Peripherals (0..31 = 1..32 peripherals)
 17-21 Number of supported Interrupt outputs (0..31 = 1..32 interrupts)
 22-31 Reserved (undef) |
| --- |

Bit1/2 are probably related to CR2 boot address, excecuting in secure or
non-secure state... but it's unspecified if Low/High means on or off.

```

| CR0 values on 3DS:
 [1000CE00h]=000167035h ;ARM9 (12 events, 8 peripherals, 4 channels)
 [10200E00h]=0001f1075h ;Old3DS/ARM11 (16 events, 18 peripherals, 8 channels)
 [10206E00h]=0003ff075h ;New3DS/ARM11 (32 events, 32 peripherals, 8 channels) |
| --- |

**10xxxE04h - CR1 - Configuration Register 1 - Cache Info (R)**

```

| 0-2 Size of an i-cache line (2,3,4,5 = 4,8,16,32 bytes) (0,1,6,7=Reserved)
 3 Reserved (undef)
 4-7 Number of i-cache lines (0..15 = 1..16 lines)
 8-31 Reserved (undef) |
| --- |

**10xxxE08h - CR2 - Configuration Register 2 - Boot Address (R)**

```

| 0-31 Reset entrypoint (for DMA Manager?) |
| --- |

**10xxxE0Ch - CR3 - Configuration Register 3 - Boot Event/Irq Security (R)**

```

| 0-31 Security state for Event-interrupt 0..31 (0=Secure, 1=Non-secure) |
| --- |

**10xxxE10h - CR4 - Configuration Register 4 - Boot Peripheral Security (R)**

```

| 0-31 Security state for Peripheral 0..31 (0=Secure, 1=Non-secure) |
| --- |

**10xxxE14h - CRD - DMA Configuration Register (R)**

```

| 0-2 AXI master Data width (2,3,4 = 32,64,128 bit) (0,1,5,6,7=Reserved)
 3 Reserved (undef)
 4-6 Write capability, max number outstanding transactions (0..7 = 1..8)
 7 Reserved (undef)
 8-11 Write queue depth (0..15 = 1..16 lines)
 12-14 Read capability, max number outstanding transactions (0..7 = 1..8)
 15 Reserved (undef)
 16-19 Read queue depth (0..15 = 1..16 lines)
 20-29 Data buffer depth (0..3FFh = 1..1024 lines)
 30-31 Reserved (undef) |
| --- |

**10xxxE80h - WD - Watchdog Register (R/W) (rev1 only)**

Supported on rev1 only (ie. r1p0/r1p1/r1p2, but not r0p0).

```

| 0 Action on Lock-up (0=IRQ and Abort contributing channels, 1=IRQ only)
 1-31 Reserved (undef) |
| --- |

```

| ______________ Peripheral and component identification register ______________ |
| --- |

**10xxxFE0h-FECh - periph_id_n - Peripheral Identification Registers 0-3 (R)**

This region contains four 32bit registers (30h,13h,x4h,00h). One is supposed to
extract the lower 8bit of these 32bit values, and then to merge them into a
"conceptual-32bit-value" (00x41330h). And then interprete it as so:

```

| 0-11 Part number (330h=DMAC)
 12-19 Designer (41h=ARM)
 20-23 Revision (0=r0p0, 1=r1p0, 2=r1p1, 3=r1p2)
 24 Integration test logic (0=None, 1=Exists)
 25-31 Reserved (undef) |
| --- |

**10xxxFF0h-FFCh - pcell_id_n - Component Identification Registers 0-3 (R)**

This region contains four 32bit registers (0Dh,F0h,05h,B1h). One is supposed to
extract the lower 8bit of these 32bit values, and then to merge them into a
"conceptual-32bit-value" (B105F00Dh). And then interprete it as so:

```

| 0-31 Component ID (B105F00Dh) |
| --- |