# Dsmemorymaps

> Source: https://problemkaputt.de/gbatek.htm
> Section: Dsmemorymaps

DS Memory Maps 
| |
| --- |

 **NDS9 Memory Map**
 
```

| 00000000h Instruction TCM (32KB) (not moveable) (mirror-able to 1000000h)
 0xxxx000h Data TCM (16KB) (moveable)
 02000000h Main Memory (4MB)
 03000000h Shared WRAM (0KB, 16KB, or 32KB can be allocated to ARM9)
 04000000h ARM9-I/O Ports
 05000000h Standard Palettes (2KB) (Engine A BG/OBJ, Engine B BG/OBJ)
 06000000h VRAM - Engine A, BG VRAM (max 512KB)
 06200000h VRAM - Engine B, BG VRAM (max 128KB)
 06400000h VRAM - Engine A, OBJ VRAM (max 256KB)
 06600000h VRAM - Engine B, OBJ VRAM (max 128KB)
 06800000h VRAM - "LCDC"-allocated (max 656KB)
 07000000h OAM (2KB) (Engine A, Engine B)
 08000000h GBA Slot ROM (max 32MB)
 0A000000h GBA Slot RAM (max 64KB)
 FFFF0000h ARM9-BIOS (32KB) (only 3K used) |
| --- |

The ARM9 Exception Vectors are located at FFFF0000h. The IRQ handler redirects
to [DTCM+3FFCh].

**NDS7 Memory Map**

```

| 00000000h ARM7-BIOS (16KB)
 02000000h Main Memory (4MB)
 03000000h Shared WRAM (0KB, 16KB, or 32KB can be allocated to ARM7)
 03800000h ARM7-WRAM (64KB)
 04000000h ARM7-I/O Ports
 04800000h Wireless Communications Wait State 0 (8KB RAM at 4804000h)
 04808000h Wireless Communications Wait State 1 (I/O Ports at 4808000h)
 06000000h VRAM allocated as Work RAM to ARM7 (max 256K)
 08000000h GBA Slot ROM (max 32MB)
 0A000000h GBA Slot RAM (max 64KB) |
| --- |

The ARM7 Exception Vectors are located at 00000000h. The IRQ handler redirects
to [3FFFFFCh aka 380FFFCh].

**Further Memory (not mapped to ARM9/ARM7 bus)**

```

| 3D Engine Polygon RAM (52KBx2)
 3D Engine Vertex RAM (72KBx2)
 Firmware (256KB) (built-in serial flash memory)
 GBA-BIOS (16KB) (not used in NDS mode)
 NDS Slot ROM (serial 8bit-bus, max 4GB with default protocol)
 NDS Slot FLASH/EEPROM/FRAM (serial 1bit-bus) |
| --- |

**Shared-RAM**

Even though Shared WRAM begins at 3000000h, programs are commonly using mirrors
at 37F8000h (both ARM9 and ARM7). At the ARM7-side, this allows to use 32K
Shared WRAM and 64K ARM7-WRAM as a continous 96K RAM block.

**Undefined I/O Ports**

On the NDS (at the ARM9-side at least) undefined I/O ports are always zero.

**Undefined Memory Regions**

16MB blocks that do not contain any defined memory regions (or that contain
only mapped TCM regions) are typically completely undefined.

16MB blocks that do contain valid memory regions are typically containing
mirrors of that memory in the unused upper part of the 16MB area (only
exceptions are TCM and BIOS which are not mirrored).