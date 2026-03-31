# Gpuinternalregisterstexturingregistersproceduraltexture

> Source: https://problemkaputt.de/gbatek.htm
> Section: Gpuinternalregisterstexturingregistersproceduraltexture

3DS GPU Internal Registers - Texturing registers (Procedural Texture) 
| |
| --- |

 These registers are for "Procedural Texture", whatever that means. Probably something for automatically generating or animating plasma/fire/water textures.
 As far as known, these are controlled solely by the parameters in the registers below (without actually using/supporting custom program code on the shader units or so).
 See GPUREG_TEXUNIT_CONFIG.bit8,9,10 for enable flag... and for selecting coordinates from texture unit 0, 1, or 2. Unknown what those coordinates are used for... as additional incoming PROCTEX parameters, or maybe the PROCTEX unit does output coordinates to the selected texture unit?
 
 **104012A0h - PICA(00A8h) - GPUREG_TEXUNIT3_PROCTEX0 (R/W)**
 
```

| 0-2 U-direction clamp ;\clamp, see below
 3-5 V-direction clamp ;/
 6-9 RGB mapping function ;\function, see below
 10-13 Alpha mapping function ;/
 14 Handle alpha separately (0=Don't separate, 1=Separate)
 15 Noise enable (0=Disable, 1=Enable)
 16-17 U-direction shift (0=None, 1=Odd, 2=Even, 3=?)
 18-19 V-direction shift (0=None, 1=Odd, 2=Even, 3=?)
 20-27 Texture bias (lower 8bit of float1.5.10) (upper 8bit are in PROCTEX4) |
| --- |

Clamp values:

```

| 00h = Clamp to zero
 01h = Clamp to edge
 02h = Symmetrical repeat
 03h = Mirrored repeat
 04h = Pulse |
| --- |

Mapping function values:

```

| 00h = U
 01h = U^2
 02h = V
 03h = V^2
 04h = (U + V) / 2
 05h = (U^2 + V^2) / 2
 06h = sqrt(U^2 + V^2)
 07h = Minimum
 08h = Maximum
 09h = Rmax |
| --- |

**104012A4h - PICA(00A9h) - GPUREG_TEXUNIT3_PROCTEX1 (R/W)**

```

| 0-15 fixed1.3.12, U-direction noise amplitude
 16-31 float1.5.10, U-direction noise phase |
| --- |

Configures the procedural texture unit's U-direction noise amplitude/phase.

**104012A8h - PICA(00AAh) - GPUREG_TEXUNIT3_PROCTEX2 (R/W)**

```

| 0-15 fixed1.3.12, V-direction noise amplitude
 16-31 float1.5.10, V-direction noise phase |
| --- |

Configures the procedural texture unit's V-direction noise amplitude/phase.

**104012ACh - PICA(00ABh) - GPUREG_TEXUNIT3_PROCTEX3 (R/W)**

```

| 0-15 float1.5.10, U-direction noise frequency
 16-31 float1.5.10, V-direction noise frequency |
| --- |

Configure the procedural texture unit's U-direction and V-direction noise
frequency.

**104012B0h - PICA(00ACh) - GPUREG_TEXUNIT3_PROCTEX4 (R/W)**

```

| 0-2 Minification filter (see below)
 3-6 Min Level of Detail (Min LOD) (usually 0)
 7-10 Max Level of Detail (Max LOD) (usually 6)
 11-18 Texture width (unsigned, 00h..FFh)
 19-26 Texture bias (upper 8bit of float1.5.10) (lower 8bit are in PROCTEX0) |
| --- |

Minification filter values:

```

| 00h = Nearest
 01h = Linear
 02h = Nearest, Mipmap Nearest
 03h = Linear, Mipmap Nearest
 04h = Nearest, Mipmap Linear
 05h = Linear, Mipmap Linear |
| --- |

**104012B4h - PICA(00ADh) - GPUREG_TEXUNIT3_PROCTEX5_LOW (R/W)**

**104012B8h - PICA(00AEh) - GPUREG_TEXUNIT3_PROCTEX5_HIGH (R/W)**

```

| 0-7 Mipmap level 0 base level (usually 00h) (Texture offset)
 8-15 Mipmap level 1 offset (usually 80h)
 16-23 Mipmap level 2 offset (usually C0h)
 24-31 Mipmap level 3 offset (usually E0h)
 32-39 Mipmap level 4 offset (usually F0h)
 40-47 Mipmap level 5 offset (usually F8h)
 48-55 Mipmap level 6 offset (usually FCh)
 56-63 Mipmap level 7 offset (usually FEh) |
| --- |

Usually set to LOW=E0C08000h, HIGH=FEFCF8F0h.

"Sets the procedural texture unit's offset."

**104012BCh - PICA(00AFh) - GPUREG_PROCTEX_LUT_INDEX (R/W)**

```

| 0-7 Index (0..255 or 0..127, depending on selected table)
 8-11 Reference table (0=Noise, 1=?, 2=RGB, 3=Alpha, 4=Color, 5=ColorDiff) |
| --- |

Selects look-up table and index for writing via GPUREG_PROCTEX_LUT_DATA(i).

**104012C0h+i*4 - PICA(00B0h+i) - GPUREG_PROCTEX_LUT_DATA(0..7) (R/W)**

```

| 0-31 LUT data |
| --- |

These registers behave as a FIFO queue. Each write to these registers writes
the provided value to the table selected with GPUREG_PROCTEX_LUT, starting at
the selected index.

Noise Table (128 elements):

```

| 0-11 fixed0.0.12, Value
 12-23 fixed0.0.12 with two's complement
 ( [0.5,1.0) mapped to [-1.0,0) ), Difference from next element |
| --- |

RGB Mapping Function Table (128 elements):

```

| 0-11 fixed0.0.12, Value
 12-23 fixed0.0.12 with two's complement, Difference from next element |
| --- |

Alpha Mapping Function Table (128 elements):

```

| 0-11 fixed0.0.12, Value
 12-23 fixed0.0.12 with two's complement, Difference from next element |
| --- |

Color Table (256 elements):

```

| 0-7 Red (unsigned, 00h..FFh)
 8-15 Green (unsigned, 00h..FFh)
 16-23 Blue (unsigned, 00h..FFh)
 24-31 Alpha (unsigned, 00h..FFh) |
| --- |

Color Difference Table (256 elements):

```

| 0-7 Half of red (signed, -80h..+7Fh) ;\
 8-15 Half of green (signed, -80h..+7Fh) ; difference between current
 16-23 Half of blue (signed, -80h..+7Fh) ; and next color table elements
 24-31 Half of alpha (signed, -80h..+7Fh) ;/ |
| --- |

```

| ____________________ Procedural Texture Generation Notes _____________________ |
| --- |

**Overview**

Procedural texture generation has four stages:

```

| Noise Module (outputs u',v')
 Repeat Module (outputs u'',v'')
 Base Shape (also notated as G(u'',v''), output g)
 F(g) and Lookup Table |
| --- |

**Noise Module**

This stage applies noise on the input coordinates. Little is known about this
other than that there are three noise parameters: Amplitude, Frequency, and
Phase.

**Repeat Module**

This stage performs basic texture coordinate wrapping on the noised
coordinates. It supports symmetric and mirrored wrapping. They don't seem to be
configurable beyond that.

**Base Shape**

The U'' and V'' coordinates are used to generate a scalar value in the range
[0;1] from the wrapped coordinates using one of six functions.

The output of this function is named "g".

**F(g) and Lookup Table**

F is a selectable function which transforms g to another scalar value. There
are two known options for F:

```

| the identity function
 a triangle function |
| --- |

The final texel color is determined by using the value of F(g) as an index into
a configurable lookup table.