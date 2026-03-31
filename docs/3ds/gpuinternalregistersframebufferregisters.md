# Gpuinternalregistersframebufferregisters

> Source: https://problemkaputt.de/gbatek.htm
> Section: Gpuinternalregistersframebufferregisters

3DS GPU Internal Registers - Framebuffer registers 
| |
| --- |

 **10401400h - PICA(0100h) - GPUREG_COLOR_OPERATION (R/W)**
 
```

| 0-1 Fragment operation mode (0=Default, 1=Gas, 2=?, 3=Shadow)
 8 Blend mode (0=Logic op, 1=Blend)
 16-23 Unused, reportedly E4h ? (isn't R/W... maybe write-only?)
 24 Render only each 2nd line (0=All lines, 1=Only each 2nd line)
 25 Render nothing? (0=Render, 1=Nothing) |
| --- |

**10401404h - PICA(0101h) - GPUREG_BLEND_FUNC (R/W)**

```

| 0-2 RGB equation (0=Add, 1=Sub, 2=ReverseSub, 3=Min, 4=Max, 5/6/7=Add)
 8-10 Alpha equation (0=Add, 1=Sub, 2=ReverseSub, 3=Min, 4=Max, 5/6/7=Add)
 16-19 RGB source blending function (00h..0Fh, see below)
 20-23 RGB destination blending function (00h..0Fh, see below)
 24-27 Alpha source blending function (00h..0Fh, see below)
 28-31 Alpha destination blending function (00h..0Fh, see below) |
| --- |

Blending Function values:

```

| 0h = Zero
 1h = One
 2h = Source color
 3h = One minus source color
 4h = Destination color
 5h = One minus destination color
 6h = Source alpha ;<-- use for source func's?
 7h = One minus source alpha ;<-- use for dest func's?
 8h = Destination alpha
 9h = One minus destination alpha
 Ah = Constant color ;<-- aka GPUREG_BLEND_COLOR maybe?
 Bh = One minus constant color
 Ch = Constant alpha
 Dh = One minus constant alpha
 Eh = Source alpha saturate ;<-- uh?
 Fh = ? |
| --- |

Unknown what source/dest means, maybe source=polygon, dest=colorbuf?

Unknown if there's a way to disable blending (other than using ADD+Zero).

Selecting things like "Source alpha" as RGB func seems to mean "color*alpha"?

Unknown if this (or other registers) allow anti-aliasing.

**10401408h - PICA(0102h) - GPUREG_LOGIC_OP (R/W)**

```

| 0-3 Logic op (0..0Fh, see below) |
| --- |

Logic op values:

```

| 0h = Clear
 1h = AND
 2h = Reverse AND
 3h = Copy
 4h = Set
 5h = Inverted copy
 6h = Noop
 7h = Invert
 8h = NAND
 9h = OR
 Ah = NOR
 Bh = XOR
 Ch = Equivalent
 Dh = Inverted AND
 Eh = Reverse OR
 Fh = Inverted OR |
| --- |

Uh, what does that crap mean, and when is it used what for?

**1040140Ch - PICA(0103h) - GPUREG_BLEND_COLOR (R/W)**

```

| 0-7 Red (unsigned, 00h..FFh)
 8-15 Green (unsigned, 00h..FFh)
 16-23 Blue (unsigned, 00h..FFh)
 24-31 Alpha (unsigned, 00h..FFh) |
| --- |

**10401410h - PICA(0104h) - GPUREG_FRAGOP_ALPHA_TEST (R/W)**

```

| 0 Alpha Testing Enable (0=Disable, 1=Enable)
 4-6 Alpha Testing Function (0-7, see below)
 8-15 Alpha Reference value (00h..FFh, unsigned) |
| --- |

Alpha Testing Function values:

```

| 0h = Never
 1h = Always
 2h = Equal
 3h = Not equal
 4h = Less than
 5h = Less than or equal
 6h = Greater than
 7h = Greater than or equal |
| --- |

**10401414h - PICA(0105h) - GPUREG_STENCIL_TEST (R/W)**

```

| 0 Stencil Testing Enable (0=Disable, 1=Enable)
 4-6 Stencil Testing Function (0-7, see below)
 8-15 Buffer mask (unsigned, 00h..FFh)
 16-23 Reference value (signed, -80h..+7Fh)
 24-31 Mask (unsigned, 00h..FFh) |
| --- |

Stencil Testing Function values:

```

| 0h = Never
 1h = Always
 2h = Equal
 3h = Not equal
 4h = Less than
 5h = Less than or equal
 6h = Greater than
 7h = Greater than or equal |
| --- |

**10401418h - PICA(0106h) - GPUREG_STENCIL_OP (R/W)**

```

| 0-2 Stencil Fail operation (0..7, see below)
 4-6 Stencil Z-fail operation (0..7, see below)
 8-10 Stencil Z-pass operation (0..7, see below) |
| --- |

Operation values:

```

| 0h = Keep
 1h = Zero
 2h = Replace
 3h = Increment
 4h = Decrement
 5h = Invert
 6h = Increment and wrap
 7h = Decrement and wrap |
| --- |

**1040141Ch - PICA(0107h) - GPUREG_DEPTH_COLOR_MASK (R/W)**

```

| 0 Depth test enable (0=Disable, 1=Enable) ;\Depth testing
 4-6 Depth function (0..7, see below) ;/
 8 Red write enable (0=Disable, 1=Enable) ;\
 9 Green write enable (0=Disable, 1=Enable) ; Color Buffer writing
 10 Blue write enable (0=Disable, 1=Enable) ;
 11 Alpha write enable (0=Disable, 1=Enable) ;/
 12 Depth write enable (0=Disable, 1=Enable) ;-Depth Buffer writing |
| --- |

Depth function values:

```

| 0h = Never
 1h = Always (same as when depth test is disabled)
 2h = Equal
 3h = Not equal
 4h = Less than
 5h = Less than or equal
 6h = Greater than
 7h = Greater than or equal |
| --- |

**10401440h - PICA(0110h) - GPUREG_RENDERBUFFER_INVALIDATE (forget cache) (W)**

**10401444h - PICA(0111h) - GPUREG_RENDERBUFFER_FLUSH (cache writeback) (W)**

```

| 0 Trigger (0=No change, 1=Trigger) (W)
 1-31 Unused (0) |
| --- |

INVALIDATE: Forgets Color/Depth Cache contents (should be used before changing
buffer base addresses, or before externally MEMFILL'ing the Color/Depth
buffers).

FLUSH: Copies Color/Depth Cache to memory (should be used before MEMCOPY'ing
the Color Buffer to LCD Framebuffer).

**10401448h - PICA(0112h) - GPUREG_COLORBUFFER_READING - Allow Read (R/W)**

**1040144Ch - PICA(0113h) - GPUREG_COLORBUFFER_WRITING - Allow Write (R/W)**

```

| 0-3 Allow read/write (00h=Disable, 0Fh=Enable, 01h..0Eh=Same as 0Fh?) |
| --- |

Write disable prevents polygon drawing. Unknown what happens on read disable.

Note: Disable stops the counters at 10400098h/1040009Ch.

**10401450h - PICA(0114h) - GPUREG_DEPTHBUFFER_READING - Allow READ (R/W)**

**10401454h - PICA(0115h) - GPUREG_DEPTHBUFFER_WRITING - Allow WRITE (R/W)**

```

| 0 Allow stencil read/write (0=Disable, 1=Enable)
 1 Allow depth read/write (0=Disable, 1=Enable) |
| --- |

Note: Disable stops the counters at 10400090h/10400094h.

**10401458h - PICA(0116h) - GPUREG_DEPTHBUFFER_FORMAT (R/W)**

```

| 0-1 Format (0=16bitDepth, 1=?, 2=24bitDepth, 3=24bitDepth+8bitStencil) |
| --- |

Configures the depth buffer data format.

**1040145Ch - PICA(0117h) - GPUREG_COLORBUFFER_FORMAT (R/W)**

```

| 0-1 Pixel size (0=16bitColor, 2=32bitColor, 1/3=?)
 16-18 Format (0=RGBA8/Gas, 2=RGB5A1, 3=RGB565, 4=RGBA4, 1/5/6/7=?) |
| --- |

Configures the color buffer data format. Color components are laid out in
reverse byte order in memory, with the most significant bits used first. uh,
maybe that refers to two 16bit pixels within one 32bit word?

**1040146Ch - PICA(011Bh) - GPUREG_RENDERBUFFER_BLOCK32 (R/W)**

```

| 0 Render buffer block size (0=Normal/8x8 pixels, 1=32x32 pixels) |
| --- |

Should be set to 8x8 (the 32x32 pixel mode can be reportedly used with
GPU_MEMCOPY_FLAGS.bit16=1, but 32x32 won't match up with the 400x240 and
320x240 pixel 3DS screens)

**10401470h - PICA(011Ch) - GPUREG_DEPTHBUFFER_LOC (R/W)**

**10401474h - PICA(011Dh) - GPUREG_COLORBUFFER_LOC (R/W)**

```

| 0-2 UNKNOWN, seems to have no effect (R/W)
 3-27 Buffer physical address, in 64-byte units |
| --- |

These registers configure the depth/color buffer physical addresses. The
buffers are reportedly divided into 8x8 pixel blocks (or optionally 32x32
pixel), that tiled format does supposedly improve cache hits when drawing small
polygons.

The color buffer can contain RGBA values. After rendering, these should be
copied to LCD framebuffer(s) using GPU_MEMCOPY registers (which can convert the
color format, and convert tiled memory to linear order.

The depth buffer can contain Depth and Stencil values. Unknown if the Depth
values are float or integer values, and unknown if they are Z or W values (or
if either one is selectable).

**10401478h - PICA(011Eh) - GPUREG_RENDERBUFFER_DIM_0 (R/W)**

**104011B8h - PICA(006Eh) - GPUREG_RENDERBUFFER_DIM_1 (R/W)**

```

| 0-10 Height (unsigned, 0..7FFh) (usually 240 on 3DS)
 12-21 Width - 1 (unsigned, 0..3FFh) (usually 320-1 or 400-1 on 3DS)
 24 Negate Y-coordinates (0=Mirror; horizontally on 3DS, 1=Default) |
| --- |

Configures the size of the COLORBUFFER and DEPTHBUFFER. The DIM_0 and DIM_1
registers should be usually set to the same value (although DIM_0 seems to be
needed, DIM_1 seems to have no effect).

**10401434h - PICA(010Dh) - GPUREG_undoc_10401434h (R/W=00000001h)**

```

| 0 UNKNOWN (0..1) (R/W) |
| --- |

**10401438h - PICA(010Eh) - GPUREG_undoc_10401438h (R/W=FFFFFFFFh)**

**1040143Ch - PICA(010Fh) - GPUREG_undoc_1040143Ch (R/W=FFFFFFFFh)**

```

| 0-31 UNKNOWN (0..FFFFFFFFh) (R/W) |
| --- |

**10401464h - PICA(0119h) - GPUREG_undoc_10401464h (R/W=FFFFFFFFh)**

**10401468h - PICA(011Ah) - GPUREG_undoc_10401468h (R/W=FFFFFFFFh)**

```

| 0-31 UNKNOWN (0..FFFFFFFFh) (R/W) |
| --- |

**1040147Ch - PICA(011Fh) - GPUREG_undoc_1040147Ch (R/W=7FFFFFFFh)**

```

| 0-30 UNKNOWN (0..7FFFFFFFh) (R/W) |
| --- |

Power-up default is 00020200h, should be set to 00010140h. GPU hangs when set
to 7FFFFFFFh. Value 1FFFFFFFh can cause a few dirt pixels upon polygon
rendering. Guess: Maybe some PLL timing or memory waitstate/cache stuff?

**10401480h - PICA(0120h) - GPUREG_GAS_LIGHT_XY - Planar Shading (R/W)**

**10401484h - PICA(0121h) - GPUREG_GAS_LIGHT_Z - View Shading (R/W)**

```

| 0-7 shading minimum intensity (unsigned, 00h..FFh)
 8-15 shading maximum intensity (unsigned, 00h..FFh)
 16-23 shading density attenuation (unsigned, 00h..FFh) |
| --- |

**10401488h - PICA(0122h) - GPUREG_GAS_LIGHT_Z_COLOR (R/W)**

```

| 0-7 View shading effect in line-of-sight direction (unsigned, 00h..FFh)
 8 Gas color LUT input (0=Gas density, 1=Light factor) |
| --- |

Configures gas light shading in the line-of-sight direction, and the input to
the gas color LUT.

**1040148Ch - PICA(0123h) - GPUREG_GAS_LUT_INDEX (W)**

```

| 0-15 Index... uh does that really have 16bit range? (W) |
| --- |

Index for writing GPUREG_GAS_LUT_DATAi.

**10401490h - PICA(0124h) - GPUREG_GAS_LUT_DATA (R/W)**

```

| 0-31 LUT data |
| --- |

These registers behave as a FIFO queue. Each write to these registers writes
the provided value to the gas look-up table, starting at the index selected
with GPUREG_GAS_LUT_INDEX. Uh, how many registers are "these registers"?

Gas Look-Up Table (16 elements):

```

| First 8 elements:
 0-7 Red (signed, -80h..+7Fh)
 8-15 Green (signed, -80h..+7Fh)
 16-23 Blue (signed, -80h..+7Fh)
 Last 8 elements:
 0-7 Red (unsigned, 00h..FFh)
 8-15 Green (unsigned, 00h..FFh)
 16-23 Blue (unsigned, 00h..FFh) |
| --- |

**10401494h - PICA(0125h) - GPUREG_undoc_10401494h (R/W=0000FFFFh)**

```

| 0-15 UNKNOWN (0..FFFFh) (R/W) |
| --- |

**10401498h - PICA(0126h) - GPUREG_GAS_DELTAZ_DEPTH (R/W)**

```

| 0-23 Depth direction attenuation proportion (fixed0.16.8)
 24-25 Depth function (0..3, see below) |
| --- |

Configures the gas depth direction attenuation proportion, as well as the gas
depth function.

```

| Gas depth function values:
 0h = Never
 1h = Always
 2h = Greater than/Greater than or equal ;uh?
 3h = Less than/Less than or equal/Equal/Not equal ;uh?? |
| --- |

**104014C0h - PICA(0130h) - GPUREG_FRAGOP_SHADOW (R/W)**

```

| 0-15 float1.5.10, Sum of penumbra scale and penumbra bias
 16-31 float1.5.10, Penumbra scale with reversed sign |
| --- |

Configures shadow properties.

**104014FCh - PICA(013Fh) - GPUREG_undoc_104014FCh (R/W=0000000Fh)**

```

| 0-15 UNKNOWN (0..0Fh) (R/W) |
| --- |