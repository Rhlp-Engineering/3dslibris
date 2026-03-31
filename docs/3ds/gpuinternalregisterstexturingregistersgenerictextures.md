# Gpuinternalregisterstexturingregistersgenerictextures

> Source: https://problemkaputt.de/gbatek.htm
> Section: Gpuinternalregisterstexturingregistersgenerictextures

3DS GPU Internal Registers - Texturing registers (Generic Textures) 
| |
| --- |

 **10401200h - PICA(0080h) - GPUREG_TEXUNIT_CONFIG (R/W)**
 
```

| 0 Texture 0 enable (0=Disable, 1=Enable) (R/W)
 1 Texture 1 enable (0=Disable, 1=Enable) (R/W)
 2 Texture 2 enable (0=Disable, 1=Enable) (R/W)
 3 UNKNOWN "0x0" (R/W) (R/W)
 8-9 Texture 3 coordinates (0=Texture0, 1=Texture1, 2=Texture2) (R/W)
 10 Texture 3 enable (0=Disable, 1=Enable) (R/W)
 12 UNKNOWN "0x1" (R/W) (R/W)
 13 Texture 2 coordinates (0=Texture2, 1=Texture1) (R/W)
 16 Clear texture cache (0=No change, 1=Clear) (W)
 17-31 0x0, uh, what??? (??)
 24-25 UNKNOWN (R/W) (R/W) |
| --- |

**10401204h - PICA(0081h) - GPUREG_TEXUNIT0_BORDER_COLOR (R/W)**

**10401244h - PICA(0091h) - GPUREG_TEXUNIT1_BORDER_COLOR (R/W)**

**10401264h - PICA(0099h) - GPUREG_TEXUNIT2_BORDER_COLOR (R/W)**

```

| 0-7 Border color Red (unsigned, 00h..FFh)
 8-15 Border color Green (unsigned, 00h..FFh)
 16-23 Border color Blue (unsigned, 00h..FFh)
 24-31 Border color Alpha (unsigned, 00h..FFh) |
| --- |

**10401208h - PICA(0082h) - GPUREG_TEXUNIT0_DIM (R/W)**

**10401248h - PICA(0092h) - GPUREG_TEXUNIT1_DIM (R/W)**

**10401268h - PICA(009Ah) - GPUREG_TEXUNIT2_DIM (R/W)**

```

| 0-10 Texture dimension Height (unsigned, 0..7FFh)
 11-15 UNKNOWN (R/W)
 16-26 Texture dimension Width (unsigned, 0..7FFh) |
| --- |

**1040120Ch - PICA(0083h) - GPUREG_TEXUNIT0_PARAM (R/W)**

**1040124Ch - PICA(0093h) - GPUREG_TEXUNIT1_PARAM (R/W)**

**1040126Ch - PICA(009Bh) - GPUREG_TEXUNIT2_PARAM (R/W)**

```

| 0 UNKNOWN (R/W)
 1 Magnification filter (0=Nearest, 1=Linear)
 2 Minification filter (0=Nearest, 1=Linear)
 3 UNKNOWN (R/W)
 4-5 ETC1 (0=NotETC1, 2=ETC1, 1/3=?) ;note: still 0 for ETC1A4
 6-7 UNKNOWN (R/W)
 8-10 Wrap T (0=ClampToEdge, 1=ClampToBorder, 2=Repeat, 3=MirroredRepeat)
 11 UNKNOWN (R/W)
 12-14 Wrap S (0=ClampToEdge, 1=ClampToBorder, 2=Repeat, 3=MirroredRepeat)
 15 UNKNOWN (R/W)
 16-17 UNKNOWN "0x0" (R/W)
 18-19 UNKNOWN (R/W)
 20 TEXUNIT0 only: Shadow (0=No, 1=Shadow)
 20 TEXUNIT1/2: UNKNOWN (R/W)
 21-23 UNKNOWN (R/W)
 24 Mipmap filter (0=Nearest, 1=Linear)
 28-30 TEXUNIT0 only: Type (see below)
 28-30 TEXUNIT1/2: Unused (0) |
| --- |

Type values:

```

| 0=2D, 1=CubeMap, 2=Shadow2D, 3=Projection, 4=ShadowCube, 5=Disabled |
| --- |

**10401210h - PICA(0084h) - GPUREG_TEXUNIT0_LOD (R/W)**

**10401250h - PICA(0094h) - GPUREG_TEXUNIT1_LOD (R/W)**

**10401270h - PICA(009Ch) - GPUREG_TEXUNIT2_LOD (R/W)**

```

| 0-12 fixed1.4.8, Bias
 13-15 UNKNOWN (R/W)
 16-19 Max Level of Detail (max LOD) (unsigned, 0..0Fh = ?)
 20-23 UNKNOWN (R/W)
 24-27 Min Level of Detail (min LOD) (unsigned, 0..0Fh = ?) |
| --- |

**10401214h - PICA(0085h) - GPUREG_TEXUNIT0_ADDR1 (R/W)**

**10401218h - PICA(0086h) - GPUREG_TEXUNIT0_ADDR2 (R/W)**

**1040121Ch - PICA(0087h) - GPUREG_TEXUNIT0_ADDR3 (R/W)**

**10401220h - PICA(0088h) - GPUREG_TEXUNIT0_ADDR4 (R/W)**

**10401224h - PICA(0089h) - GPUREG_TEXUNIT0_ADDR5 (R/W)**

**10401228h - PICA(008Ah) - GPUREG_TEXUNIT0_ADDR6 (R/W)**

**10401254h - PICA(0095h) - GPUREG_TEXUNIT1_ADDR (R/W)**

**10401274h - PICA(009Dh) - GPUREG_TEXUNIT2_ADDR (R/W)**

First ADDR register, PICA(0085h,0095h,009Dh):

```

| 0-27 Texture physical memory address, in 8-byte units (full 28bits) |
| --- |

Subsequent ADDR registers, PICA(0086h..008Ah):

```

| 0-21 Texture physical memory address, in 8-byte units (lower 22bits)
 (upper 6bits are reused from first ADDR register) |
| --- |

Individual texels in a texture are laid out in memory as a Z-order curve.
Mipmap data is stored directly following the main texture data.

If the texture is a cube:

```

| Register Description
 ADDR1 Positive X
 ADDR2 Negative X
 ADDR3 Positive Y
 ADDR4 Negative Y
 ADDR5 Positive Z
 ADDR6 Negative Z |
| --- |

Otherwise, ADDR(1) points to a 2D texture, and the rest are empty.

**1040122Ch - PICA(008Bh) - GPUREG_TEXUNIT0_SHADOW (R/W)**

```

| 0 Perspective (0=Perspective, 1=Not perspective)
 1-23 fixed0.0.24, Z bias (upper 23 bits) (uh, how can that have 24bit?)
 24-31 UNKNOWN (R/W) |
| --- |

"Texture unit's shadow texture properties."

**10401230h - PICA(008Ch) - GPUREG_undoc_10401230h (R/W=FFFF00FFh)**

```

| 0-7 UNKNOWN (0..FFh) (R/W)
 16-31 UNKNOWN (0..FFFFh) (R/W) |
| --- |

**10401234h - PICA(008Dh) - GPUREG_undoc_10401234h (R/W=000000FFh)**

```

| 0-7 UNKNOWN (0..FFh) (R/W) |
| --- |

**10401238h - PICA(008Eh) - GPUREG_TEXUNIT0_TYPE (R/W)**

**10401258h - PICA(0096h) - GPUREG_TEXUNIT1_TYPE (R/W)**

**10401278h - PICA(009Eh) - GPUREG_TEXUNIT2_TYPE (R/W)**

```

| 0-3 Texture Data Format |
| --- |

Format values:

```

| 00h = RGBA8888 GL_RGBA GL_UNSIGNED_BYTE
 01h = RGB888 GL_RGB GL_UNSIGNED_BYTE
 02h = RGBA5551 GL_RGBA GL_UNSIGNED_SHORT_5_5_5_1
 03h = RGB565 GL_RGB GL_UNSIGNED_SHORT_5_6_5
 04h = RGBA4444 GL_RGBA GL_UNSIGNED_SHORT_4_4_4_4
 05h = LA8 GL_LUMINANCE_ALPHA GL_UNSIGNED_BYTE
 06h = HILO8
 07h = L8 GL_LUMINANCE GL_UNSIGNED_BYTE
 08h = A8 GL_ALPHA GL_UNSIGNED_BYTE
 09h = LA44 GL_LUMINANCE_ALPHA GL_UNSIGNED_BYTE_4_4_EXT
 0Ah = L4
 0Bh = A4 GL_ALPHA GL_UNSIGNED_NIBBLE_EXT
 0Ch = ETC1 GL_ETC1_RGB8_OES
 0Dh = ETC1A4 |
| --- |

3DS GPU Texture Formats