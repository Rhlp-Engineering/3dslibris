# Gpuexternalregistersmemorycontrolstatusregisters

> Source: https://problemkaputt.de/gbatek.htm
> Section: Gpuexternalregistersmemorycontrolstatusregisters

3DS GPU External Registers - Memory Control/Status Registers 
| |
| --- |

 The 3DS bootrom error screen initializes only 10400004h and 10400030h (VRAM control/power). The 3DS OS does reportedly also initialize 10400050h and 10400054h (timings?).
 
 **10400000h - GPU_FIXED_00010002h (always 00010002h, maybe ChipID/Version) (R)**
 
```

| 0-31 Always 00010002h (maybe ChipID/Version) |
| --- |

Note: Register 104010D4h seems to contain the same readonly value.

**10400004h - GPU_VRAM_CNT (reset=0, bootrom=300h) (R/W)**

```

| 0-31 Unknown (0..FFFFFFFFh) |
| --- |

Seems to have no effect on LCD output. However, value FFFFFFFFh does mess up
the polygon renderer (or MEMCOPY unit), causing each 4 scanlines to be drawn
twice.

**10400008h - GPU_UNKNOWN_2BIT (reset=0) (R/W)**

```

| 0-1 Unknown (0..03h)
 2-31 Unused (0) |
| --- |

**10400030h - GPU_VRAM_POWER (R/W)**

```

| 0-7 Unknown (0..FFh)
 8 Power off VRAM_A data.bit0-63 (0=On, 1=Power Off)
 9 Power off VRAM_A data.bit64-127 (0=On, 1=Power Off)
 10 Power off VRAM_B data.bit0-63 (0=On, 1=Power Off) ;\later uses VRAM_A
 11 Power off VRAM_B data.bit64-127 (0=On, 1=Power Off) ;/when VRAM_B=off?
 12-31 Unknown (0..FFFFFh) |
| --- |

Seems to disable VRAM power, VRAM_A/B are 3Mbyte each, and each of them seems
to have separate sections for data bit0-63 and 64-127 (ie. the first and second
8 bytes within 16 byte snippets).

After writing to 10400030h, one should issue a dummy read from 10400030h.

Even when disabled only for a short moment, the VRAM does immediately forget
it's data and gets filled with random values; this can be seen after
re-enabling the memory (the CPU hangs when reading VRAM while it is disabled).

**10400034h - GPU_STAT_IRQ_FLAGS (R)**

Contains IRQ flags (normally IRQ handling/polling should work fine without
needing this register; except, polling GPU_MEMCOPY_CNT can apparently hang the
CPU, whilst polling GPU_STAT_IRQ_FLAGS.bit30 does work better).

```

| 0 Unknown, usually set ;\get cleared when setting GPU_MAKE_SOMETHING.bit0
 1 Unknown, usually set ;/(unknown if/when the flags can get set again)
 2-25 Unused (0)
 26 IRQ 28h PSC0 aka GPU_MEMFILL 0 ready ;mirror of GPU_MEMFILL_CNT0.bit1
 27 IRQ 29h PSC1 aka GPU_MEMFILL 1 ready ;mirror of GPU_MEMFILL_CNT1.bit1
 28 IRQ 2Ah PDC0 aka GPU H/V-IRQ for top screen
 29 IRQ 2Bh PDC1 aka GPU H/V-IRQ for bottom screen
 30 IRQ 2Ch PPF aka GPU_MEMCOPY ready ;mirror of GPU_MEMCOPY_CNT.bit8
 31 IRQ 2Dh P3D aka GPUREG_IRQ_CMP/REQ match (commonly at end of cmdlist) |
| --- |

Bit26,27,30 can be cleared in the corresponding mirrored registers.

Bit28,29 are automatically set/cleared during LCD output (usually once per
frame).

Bit31 can be cleared by setting IRQ_REQ and IRQ_CMP to different values.

**10400038h - GPU_STAT_SOMETHING (R)**

```

| 0-31 Can be 00000000h, 10400800h, 10402000h, or 10410000h |
| --- |

Unknown, initially 10402000h on reset, 10400800h after LCD init or so, and
becomes 00000000h after setting GPU_MAKE_SOMETHING.bit0, and (thereafter?)
changes to 10410000h. No idea if 104xxxxxh is related to the address of the GPU
I/O area.

**1040003Ch - GPU_MAKE_SOMETHING (R/W)**

```

| 0 Unknown (0=Normal, 1=GPU_STAT_SOMETHING changes after a while)
 1-31 Unknown (0..7FFFFFFFh) |
| --- |

Unknown, setting bit0 (even only for a short moment) will later on cause
GPU_STAT_SOMETHING to become 10410000h. Also affects GPU_STAT_IRQ_FLAGS.bit0-1.

**10400040h - GPU_STAT_BACKLIGHT_OR_SO (R)**

```

| 0-1 Unknown (usually 0, but can be 3 when [104000C0h]=0 or [104000CCh]=0)
 2-31 Unused (0) |
| --- |

**10400044h - GPU_UNKNOWN_CAN_BE_7F80h (R)**

```

| 0-31 Texture related...? (usually 0, but sometimes 7F80h, and once 1200h) |
| --- |

Unknown. Usually zero, but can contain other values after rendering with
GPUREG_TEXUNIT_CONFIG=nonzero (even then, it does often stay zero, but
sometimes changes to 7F80h, and once changed to 1200h). Whether the register
does change seems to depend on power-up state of other uninitialized
registers/memory and/or whatever timings (if nonzero values did occur then they
do usually reappear upon next rendering even after reset, but that behaviour
can change on next power-up).

Note: Value 1200h occurred after changing GPUREG_COLORBUFFER_LOC and
GPUREG_DEPTHBUFFER_LOC to dummy address zero (but that couldn't be reproduced).

**10400048h - GPU_UNKNOWN_32BIT (reset=0) (R/W)**

```

| 0-31 Unknown (0..FFFFFFFFh) |
| --- |

**10400050h - GPU_TIMING_CNT1 (R/W=FFFFFFFFh) (res=1111EF00h) ;init=22221200h**

```

| 0-31 Unknown (0..FFFFFFFFh) (affects GPU_MEMCOPY timings and maybe others) |
| --- |

Somehow affects timings. Different values can make timings about 10% faster or
slower (or, some values seem to make no difference at all). For example,

```

| 10101010h or 20202020h --> slower MEMCOPY
 1111EF00h or 22221200h --> normal MEMCOPY
 FFFFFFFFh --> faster MEMCOPY |
| --- |

**10400054h - GPU_TIMING_CNT2 (R/W=FFFFFFFFh) (res=00000112h) ;init=00000FF2h**

```

| 0-31 Unknown (0..FFFFFFFFh) (probably affects whatever timings) |
| --- |

**10400058h - GPU_STAT_BUSY_FLAGS (R)**

```

| 10 Unknown, seems to be set on power-up, but later cleared?
 17 Often set after writing GPU_VRAM_POWER
 18 Sometimes set after writing GPU_VRAM_POWER
 19 GPU_MEMFILL_0/1 Busy (0=No, 1=Busy)
 20 GPU_MEMCOPY Busy (0=No, 1=Busy; gets set some cycles after start) |
| --- |

**1040005Ch - GPU_MAKE_WHATEVER_MESS (R/W)**

```

| 0 Whatever, gets copied to bit8 (W)
 8 Whatever, contains value written to bit0 (R)
 16-21 Whatever, 00h..3Fh (R/W)
 24-29 Whatever, 00h..3Fh (R/W) |
| --- |

Affects the 30h-word pattern returned when reading below register.

**10400060h - GPU_STAT_WHATEVER_MESS (R)**

```

| 0-6 Unused? (0)
 7-22 Whatever Mess
 23 Unused? (0)
 24-29 Whatever Mess
 30-31 Unused? (0) |
| --- |

Repeatly reading returns a mess, repeated every 30h words, for example:

```

| 08002400h, 01404180h, 04002000h, 00020000h, 20000200h, 02080000h, 00080000h,
 0041A000h, 11006100h, 00041000h, 00242000h, 00100880h, 1200A000h, 08000000h,
 10042080h, 04082400h, 00002400h, 00000000h, 1040C500h, 03000000h, ... |
| --- |

The same values as above are also returned when inserting delays in the read
loop. So it's either random generated... or a FIFO that reads junk from an
uninitialized array?

Changing 1040005Ch (eg. writing FFFFFFFFh) causes above to return different
garbage values (also reapeating each 30h words, but containing a more regular
pattern with fewer bits set).

**10400068h GPU_UNKNOWN_INITIALLY_00A80000h (res=00A80000h) (R/W)**

```

| 0-31 Unknown (0..FFFFFFFFh) (initially 00A80000h on reset) |
| --- |

**10400070h - Read-only memory traffic counters (R)**

```

| 10400070h GPU_STAT_TOTAL_NON_VRAM_READS ;\Non-VRAM (eg. AXI,QTM,FCRAM)
 10400074h GPU_STAT_TOTAL_NON_VRAM_WRITES ;/
 10400078h GPU_STAT_TOTAL_VRAM_A_READS ;\1st 3Mbyte VRAM block
 1040007Ch GPU_STAT_TOTAL_VRAM_A_WRITES ;/
 10400080h GPU_STAT_TOTAL_VRAM_B_READS ;\2nd 3Mbyte VRAM block
 10400084h GPU_STAT_TOTAL_VRAM_B_WRITES ;/
 10400088h GPU_STAT_POLYGON_ARRAY_READS ;-ATTR_BUF, INDEX_LIST
 1040008Ch GPU_STAT_POLYGON_TEXTURE_READS ;-cache misses only
 10400090h GPU_STAT_POLYGON_DEPTHBUFFER_READS
 10400094h GPU_STAT_POLYGON_DEPTHBUFFER_WRITES
 10400098h GPU_STAT_POLYGON_COLORBUFFER_READS
 1040009Ch GPU_STAT_POLYGON_COLORBUFFER_WRITES
 104000A0h GPU_STAT_LCD_UPPER_SCREEN_READS ;\for both left+right buffer
 104000A4h GPU_STAT_LCD_LOWER_SCREEN_READS ;/
 104000A8h GPU_STAT_MEMCOPY_SRC_READS ;\GPU_MEMCOPY
 104000ACh GPU_STAT_MEMCOPY_DST_WRITES ;/
 104000B0h GPU_STAT_MEMFILL_0_DST_WRITES ;\GPU_MEMFILL 0/1
 104000B4h GPU_STAT_MEMFILL_1_DST_WRITES ;/
 104000B8h GPU_STAT_CPU_READS_FROM_VRAM_A_B ;\counted by GPU because the
 104000BCh GPU_STAT_CPU_WRITES_TO_VRAM_A_B ;/GPU must forward vram to cpu |
| --- |

**104000C4h - GPU_BASE_ADDR_VRAM_A (reset=18000000h) (R/W)**

**104000C8h - GPU_BASE_ADDR_VRAM_B (reset=18300000h) (R/W)**

```

| 0-31 Base address for VRAM_A/B blocks (0..FFFFFFFFh) (any alignment needed?) |
| --- |

The 3DS contains two 3MByte VRAM blocks (ie. 6Mbyte in total). These registers
allow to swap the base address of those two blocks, or to map them to other
locations within the following 12Mbyte region:

```

| 18000000h..182FFFFFh Usually VRAM_A (3Mbyte)
 18300000h..185FFFFFh Usually VRAM_B (3Mbyte)
 18600000h..18BFFFFFh Usually empty/zerofilled (6Mbyte) |
| --- |

Trying to map VRAM outside of that 12Mbyte region might work, but the ARM CPU
will trigger data aborts when trying to access that memory.

**104000C0h - GPU_BACKLIGHT_OR_SO_1 (reset=20000000h) (R/W)**

**104000CCh - GPU_BACKLIGHT_OR_SO_2 (reset=20000000h) (R/W)**

```

| 0-31 Unknown (0..FFFFFFFFh) |
| --- |

Reportedly 104000C0h does "Writes 0 to allow backlights to turn off, 20000000h
to force them always on."

Unknown what that means. Default is 20000000h (and backlights CAN be switched
off in that state). Maybe "allow" means that they can switch off
automatically... upon GPU inactivity... or upon blank screen... or upon screen
saver timeouts... or whatever? Or maybe 20000000h is FCRAM base, completely
unrelated to backlights?

Note: Setting [104000C0h] or [104000CCh] to zero can cause [10400040h] to get
nonzero.

**104000D0h - GPU_UNKNOWN_4BIT (reset=0) (R/W)**

```

| 0-3 Unknown (0..0Fh)
 4-31 Unused (0) |
| --- |