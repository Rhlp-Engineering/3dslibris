# Gpuexternalregistersmemfillandmemcopy

> Source: https://problemkaputt.de/gbatek.htm
> Section: Gpuexternalregistersmemfillandmemcopy

3DS GPU External Registers - Memfill and Memcopy 
| |
| --- |

 
```

| ________________________________ Memory Fill ________________________________ |
| --- |

**10400010h/10400020h - GPU_MEMFILL_DST_ADDR0/1 - Memfill 0/1 Start Address**

**10400014h/10400024h - GPU_MEMFILL_DST_END0/1 - Memfill 0/1 End Address**

```

| 0 Unused (0)
 1-28 Physical Memory Address, in 16-byte units
 29-31 Unused (0) |
| --- |

**10400018h/10400028h - GPU_MEMFILL_DATA0/1 - Memfill 0/1 Fill Value**

```

| 0-31 Fill Value |
| --- |

**1040001Ch/1040002Ch - GPU_MEMFILL_CNT0/1 - Memfill 0/1 Control**

```

| 0 Start/Busy (0=Idle/Ready, 1=Start/Busy) (R/W)
 1 Ready IRQ (0=No, 1=Ready, IRQ 28h/29h) (write 0 to clear) (R/ack)
 2-7 Unused (0)
 8-9 Fill-width (0=16bit, 1=24bit, 2=32bit, 3=Same as 1) (R/W)
 10-15 Unused (0)
 16-20 Unknown, 5bit, R/W (R/W)
 21-31 Unused (0) |
| --- |

A memory fill is triggered by setting bit0 in the control register. Doing so
aborts any running memory fills on that filling unit. Upon completion, the
hardware clears bit0 and sets bit1 and fires interrupt PSC0/PSC1 (IRQ 28h/29h).

```

| ________________________________ Memory Copy ________________________________ |
| --- |

**10400C00h - GPU_MEMCOPY_SRC_ADDR - Memcopy Input physical address (R/W)**

**10400C04h - GPU_MEMCOPY_DST_ADDR - Memcopy Output physical address (R/W)**

```

| 0 Unused (0)
 1-28 Physical Memory Address, in 16-byte units
 29-31 Unused (0) |
| --- |

**10400C08h - GPU_MEMCOPY_DISPLAY_SIZE - DisplayTransfer width/height(R/W)**

```

| 0-2 Unused (0)
 3-15 Input Height (in 8-pixel units, usually 240/8 for 3DS)
 16-18 Unused (0)
 19-31 Input Width (in 8-pixel units, usually 320/8 or 400/8 for 3DS) |
| --- |

Input size before downscale (the output size is the same, unless downscale was
used).

**10400C0Ch - GPU_MEMCOPY_DISPLAY_GAP - DisplayTransfer Input height+gap (R/W)**

```

| 0-2 Unused (0)
 3-15 Input Height+Gap (in 8-pixel units, usually (240+0)/8 for 3DS)
 16-31 Unused (0) |
| --- |

Allows to skip input pixels, used only if GPU_MEMCOPY_FLAGS.bit2=1.

**10400C10h - GPU_MEMCOPY_FLAGS - Memcopy Transfer flags (R/W)**

```

| 0 Horizontal Flip (0=Normal, 1=Mirror)
 1 Conversion Mode (0=TiledToLinear, 1=LinearToTiled)
 2 Input Gap (0=Use raw height, 1=Use height+gap)
 3 Copy Mode (0=DisplayCopy/Pixels, 1=TextureCopy/RawBytes)
 4 Unused (0)
 5 Conversion Disable (0=Convert as specified in bit1, 1=TiledToTiled)
 6-7 Unused (0)
 8-10 Input RGBA Format (0=8888, 1=8880, 2=5650, 3=5551, 4..7=4444)
 11 Unused (0)
 12-14 Output RGBA Format (0=8888, 1=8880, 2=5650, 3=5551, 4..7=4444)
 15 Unused (0)
 16 Block Tiling Size (0=Normal/8x8 pixels, 1=32x32 pixels)
 17-23 Unused (0)
 24-25 Output Downscale (0=No, 1=Height/2, 2=WidthAndHeight/2, 3=Invalid)
 26-31 Unused (0) |
| --- |

The transfer hangs on some Input/Output RGBA combinations:

```

| 32bit Input (8888) works with all output formats
 24bit Input (8880) works only if output is also 24bit
 16bit Input (5650/5551/4444) works only if output is also 16bit |
| --- |

**10400C14h - GPU_MEMCOPY_UNKNOWN_21BIT - Memcopy (R/W)**

```

| 0-20 Unknown, R/W
 21-31 Unused (0) |
| --- |

"GSP module writes value 0 here prior to writing to 10400C18h, for cmd3."

**10400C18h - GPU_MEMCOPY_CNT - Memcopy Start/Busy (R/W)**

```

| 0 Start/Busy (0=Idle/Ready, 1=Start/Busy) (R/W)
 1-7 Unused (0)
 8 Ready IRQ (0=No, 1=Ready, "PPF" IRQ 2Ch) (write 0 to clear) (R/ack)
 9-31 Unused (0) |
| --- |

Caution: Polling this register can HANG the ARM11 CPU at end of transfer; when
using polling, it's better to wait for GPU_STAT_IRQ_FLAGS.bit30=1.

**10400C1Ch - GPU_MEMCOPY_REMAIN_IRQ - Memcopy Remain IRQ (R/W)**

```

| 0-13 Transfer IRQ position (compare value for below remain counter) (R/W)
 14-15 Unused (0)
 16-29 Transfer width remain, in 4-pixel units (3FFEh..0, or 3FFFh=done) (R)
 30-31 Unused (0) |
| --- |

The IRQ position should be set to 3FFFh for triggering IRQ flag at transfer
end, otherwise the IRQ would trigger before completion (or never at all).

Example: For width=320pix, remain goes through 4Fh..00h, and does then wrap to
3FFFh.

The 4-pixel unit might be due to decreasing the counter upon both reading &
writing 8-pixel chunks.

Used for DisplayTransfer. And maybe also for TextureCopy?

**10400C20h - GPU_MEMCOPY_TEX_SIZE - TextureCopy total amount**

```

| 0-3 Unused (0)
 4-31 Total amount of data to copy, in 16-byte units |
| --- |

**10400C24h - GPU_MEMCOPY_TEX_SRC_WIDTH - TextureCopy input line width/gap**

**10400C28h - GPU_MEMCOPY_TEX_DST_WIDTH - TextureCopy output line width/gap**

```

| For SRC_WIDTH:
 0-15 Width, in unknown units (maybe 16-byte units, same as below?)
 16-31 Gap, in 16-byte units
 For DST_WIDTH:
 0-31 Width and Gap (probably same 2x16bit format as for above SRC_WIDTH) |
| --- |

**10400C2Ch - GPU_MEMCOPY_UNKNOWN_FLAG - Memcopy Unknown (R/W)**

```

| 0 Unknown, R/W
 1-31 Unused (0) |
| --- |

**Notes...**

"The DisplayTransfer registers are only used if FLAGS.bit3=0 and ignored
otherwise. The TextureCopy registers are "likewise" only used if FLAGS.bit3=1,
and ignored otherwise."

**TextureCopy**

When FLAGS.bit3=1, the hardware performs a TextureCopy-mode transfer. In this
mode, all other bits of that register are ignored (except for FLAGS.bit2, which
still needs to be set correctly?), and no format conversions are done.

Instead, it performs a raw data copy from the source to the destination, but
with a configurable gap between lines. The total amount of bytes to copy is
specified in the size register, and the hardware loops reading lines from the
input and writing them to the output until this amount is copied. The "gap"
specified in the input/output dimension register is the number of chunks to
skip after each "width" chunks of the input/output, and is NOT counted towards
the total size of the transfer.

By correctly calculating the input and output gap sizes it is possible to use
this functionality to copy arbitrary sub-rectangles between differently-sized
framebuffers or textures, which is one of its main uses over a regular
no-conversion DisplayTransfer. When copying tiled textures/framebuffers it's
important to remember that the contents of a tile are laid out sequentially in
memory, and so this should be taken into account when calculating the transfer
parameters.

Specifying invalid/junk values for the TextureCopy dimensions can result in the
GPU hanging while attempting to process this TextureCopy.