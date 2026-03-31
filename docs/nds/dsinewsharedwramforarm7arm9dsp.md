# Dsinewsharedwramforarm7Arm9Dsp

> Source: https://problemkaputt.de/gbatek.htm
> Section: Dsinewsharedwramforarm7Arm9Dsp

DSi New Shared WRAM (for ARM7, ARM9, DSP) 
| |
| --- |

 **Shared WRAM (total 800Kbytes)**
 
```

| Old WRAM-0/1 32Kbytes (2x16K), mappable to ARM7, or ARM9
 New WRAM-A 256Kbytes (4x64K), mappable to ARM7, or ARM9
 New WRAM-B 256Kbytes (8x32K), mappable to ARM7, ARM9, or DSP-program memory
 New WRAM-C 256Kbytes (8x32K), mappable to ARM7, ARM9, or DSP-data memory |
| --- |

New WRAM mapping is done in three steps: First, releasing Slot Write Protect
(on ARM7 side). Then, mapping the physical banks to logical slots (on ARM9
side). And finally, mapping those slots to actual memory addresses (on ARM7 and
ARM9 sides). As an extra step, one may set Slot Write Protect flags (on ARM7
side) to prevent ARM9 from applying further changes.

```

| ____________________________ Slot Write Protect ______________________________ |
| --- |

MBK9 is READ/WRITE-ABLE only on ARM7 side (and READ-ONLY on ARM9).

**4004060h - DSi9 - MBK9, WRAM-A/B/C Slot Write Protect (undocumented) (R)**

**4004060h - DSi7 - MBK9, WRAM-A/B/C Slot Write Protect (undocumented) (R/W)**

```

| 0-3 WRAM-A, Port 4004040h-4004043h Write (0=Writeable by ARM9, 1=Read-only)
 4-7 Unknown/Unused (0)
 8-15 WRAM-B, Port 4004044h-400404Bh Write (0=Writeable by ARM9, 1=Read-only)
 16-23 WRAM-C, Port 400404Ch-4004053h Write (0=Writeable by ARM9, 1=Read-only)
 24-31 Unknown/Unused (0) ;but, carthdr has nonzero data for it? |
| --- |

Selects whether ARM9 may write to WRAM slot registers at 4004040h-4004053h (in
Read-only mode neither ARM7 nor ARM9 can write to those registers; that applies
only to that registers, ie. the memory itself isn't write-protected).

```

| ______________________________ Slot Allocation ______________________________ |
| --- |

MBK1-MBK5 are READ-ONLY on ARM7 side, and either READ-ONLY or READ/WRITE-ABLE
on ARM9 side (depending on the MBK9 setting).

**4004040h - DSi - MBK1.0, WRAM-A0 - 64K, mappable to ARM7, or ARM9**

**4004041h - DSi - MBK1.1, WRAM-A1 - 64K, mappable to ARM7, or ARM9**

**4004042h - DSi - MBK1.2, WRAM-A2 - 64K, mappable to ARM7, or ARM9**

**4004043h - DSi - MBK1.3, WRAM-A3 - 64K, mappable to ARM7, or ARM9**

```

| 0 Master (0=ARM9, 1=ARM7)
 1 Not used
 2-3 Offset (0..3) (slot 0..3) (LSB of address in 64Kbyte units)
 4-6 Not used
 7 Enable (0=Disable, 1=Enable) |
| --- |

In Cooking Coach, above four bytes are locked via MBK9 (not write-able, always
81h,85h,89h,8Dh).

**4004044h - DSi - MBK2.0, WRAM-B0 - 32K, mappable to ARM7, ARM9, or DSP/code**

**4004045h - DSi - MBK2.1, WRAM-B1 - 32K, mappable to ARM7, ARM9, or DSP/code**

**4004046h - DSi - MBK2.2, WRAM-B2 - 32K, mappable to ARM7, ARM9, or DSP/code**

**4004047h - DSi - MBK2.3, WRAM-B3 - 32K, mappable to ARM7, ARM9, or DSP/code**

**4004048h - DSi - MBK3.0, WRAM-B4 - 32K, mappable to ARM7, ARM9, or DSP/code**

**4004049h - DSi - MBK3.1, WRAM-B5 - 32K, mappable to ARM7, ARM9, or DSP/code**

**400404Ah - DSi - MBK3.2, WRAM-B6 - 32K, mappable to ARM7, ARM9, or DSP/code**

**400404Bh - DSi - MBK3.3, WRAM-B7 - 32K, mappable to ARM7, ARM9, or DSP/code**

```

| 0-1 Master (0=ARM9, 1=ARM7, 2 or 3=DSP/code)
 2-4 Offset (0..7) (slot 0..7) (LSB of address in 32Kbyte units)
 5-6 Not used (0)
 7 Enable (0=Disable, 1=Enable) |
| --- |

**400404Ch - DSi - MBK4.0, WRAM-C0 - 32K, mappable to ARM7, ARM9, or DSP/data**

**400404Dh - DSi - MBK4.1, WRAM-C1 - 32K, mappable to ARM7, ARM9, or DSP/data**

**400404Eh - DSi - MBK4.2, WRAM-C2 - 32K, mappable to ARM7, ARM9, or DSP/data**

**400404Fh - DSi - MBK4.3, WRAM-C3 - 32K, mappable to ARM7, ARM9, or DSP/data**

**4004050h - DSi - MBK5.0, WRAM-C4 - 32K, mappable to ARM7, ARM9, or DSP/data**

**4004051h - DSi - MBK5.1, WRAM-C5 - 32K, mappable to ARM7, ARM9, or DSP/data**

**4004052h - DSi - MBK5.2, WRAM-C6 - 32K, mappable to ARM7, ARM9, or DSP/data**

**4004053h - DSi - MBK5.3, WRAM-C7 - 32K, mappable to ARM7, ARM9, or DSP/data**

```

| 0-1 Master (0=ARM9, 1=ARM7, 2 or 3=DSP/data)
 2-4 Offset (0..7) (slot 0..7) (LSB of address in 32Kbyte units)
 5-6 Not used (0)
 7 Enable (0=Disable, 1=Enable) |
| --- |

```

| ______________________________ Address Mapping ______________________________ |
| --- |

MBK6-8 exist as separate READ/WRITE-ABLE registers on ARM7 and ARM9 side
(making it six registers in total).

**4004054h - DSi - MBK6, WRAM-A, 64K..256K mapping (R/W)**

```

| 0-3 Not used (0)
 4-11 Start Address (3000000h+N*10000h) ;=3000000h..3FF0000h
 12-13 Image Size (0 or 1=64KB/Slot0, 2=128KB/Slot0+1+2??, 3=256KB/Slot0..3)
 14-19 Not used (0)
 20-28 End Address (3000000h+N*10000h-1) ;=2FFFFFFh..4FEFFFFh
 29-31 Not used (0) |
| --- |

**4004058h - DSi - MBK7, WRAM-B (R/W)**

**400405Ch - DSi - MBK8, WRAM-C (R/W)**

```

| 0-2 Not used (0)
 3-11 Start Address (3000000h+N*8000h) ;=3000000h..3FF8000h
 12-13 Image Size (0=32K/Slot0,1=64KB/Slot0-1,2=128KB/Slot0-3,3=256KB/Slot0-7)
 14-18 Not used (0)
 19-28 End Address (3000000h+N*8000h-1) ;=2FFFFFFh..4FF7FFFh
 29-31 Not used (0) |
| --- |

Uh, but, ARM7 3800000h..3FFFFFFh contains OTHER memory (ARM7-WRAM) !?

```

| ___________________________________ Notes ___________________________________ |
| --- |

**Slots and Image Size vs Start/End Addresses**

When using Image Size of 4 slots, then Memory at 3000000h..3FFFFFFh is:

```

| Slots 0,1,2,3,0,1,2,3,0,1,2,3,0,1,2,3,etc. |
| --- |

When start=6, and End=12, then (with above example), only following is mapped:

```

| Slots -,-,-,-,-,-,2,3,0,1,2,3,-,-,-,-,etc. |
| --- |

Observe that the mapped region starts with Slot 2 (not Slot 0) in that case.

Moreover, some slots may be empty (disabled, or mapped to another CPU), so, if
Slot 3 is empty (disabled or mapped to another CPU), then memory appears to
look somewhat as so:

```

| Slots -,-,-,-,-,-,2,z,0,1,2,z,-,-,-,-,etc. |
| --- |

Whereas, the "z" areas seem to read as zerofilled memory blocks (rather than
mirroring to underlaying WRAM's of lower priority).

**Overlapping WRAM regions**

```

| New Shared-WRAM-A Highest Priority
 New Shared-WRAM-B High Priority
 New Shared-WRAM-C Low Priority
 Old Shared-WRAM-0/1 Lowest Priority
 Old ARM7-WRAM Whatever Priority (unknown...)
 I/O ports 4xxxxxxh Whatever Priority (unknown...) |
| --- |

**Overlapping WRAM slots**

```

| Unknown what happens when selecting multiple WRAM blocks to the same slot? |
| --- |

The initial MBK values are derived from carthdr.

Exploits for DSi cartridges & DSiware usually have ARM7.MBK registers
disabled via SCFG_EXT7, making it impossible to change that ARM7 registers; the
ARM9.MBK registers are usually kept enabled, nethertheless, the ARM7.MBK9
setting can apply some restrictions to ARM9 side (for example, in Cooking
Coach, WRAM-A is controlled via ARM7.MBK1, so ARM9 can control WRAM-B and
WRAM-C via ARM9.MBK2-5 only).