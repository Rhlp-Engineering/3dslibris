# Gpuinternalregisterstexturingregistersenvironment

> Source: https://problemkaputt.de/gbatek.htm
> Section: Gpuinternalregisterstexturingregistersenvironment

3DS GPU Internal Registers - Texturing registers (Environment) 
| |
| --- |

 Below are "texture env(ironment?)" registers for "texture combiner's". Unknown what that means, probably something for merging output from texture units 0,1,2. But unknown why there are six TexEnv's.
 
 **10401300h - PICA(00C0h) - GPUREG_TEXENV0_SOURCE (R/W)**
 **10401320h - PICA(00C8h) - GPUREG_TEXENV1_SOURCE (R/W)**
 **10401340h - PICA(00D0h) - GPUREG_TEXENV2_SOURCE (R/W)**
 **10401360h - PICA(00D8h) - GPUREG_TEXENV3_SOURCE (R/W)**
 **104013C0h - PICA(00F0h) - GPUREG_TEXENV4_SOURCE (R/W)**
 **104013E0h - PICA(00F8h) - GPUREG_TEXENV5_SOURCE (R/W)**
 
```

| 0-3 RGB source 0
 4-7 RGB source 1
 8-11 RGB source 2
 16-19 Alpha source 0
 20-23 Alpha source 1
 24-27 Alpha source 2 |
| --- |

Configures a texture combiner's sources.

Source values:

```

| 00h = Primary color
 01h = Fragment primary color
 02h = Fragment secondary color
 03h = Texture 0
 04h = Texture 1
 05h = Texture 2
 06h = Texture 3
 0xh = ?
 0Dh = Previous buffer
 0Eh = Constant (from GPUREG_TEXENVi_COLOR)
 0Fh = Previous |
| --- |

**10401304h - PICA(00C1h) - GPUREG_TEXENV0_OPERAND (R/W)**

**10401324h - PICA(00C9h) - GPUREG_TEXENV1_OPERAND (R/W)**

**10401344h - PICA(00D1h) - GPUREG_TEXENV2_OPERAND (R/W)**

**10401364h - PICA(00D9h) - GPUREG_TEXENV3_OPERAND (R/W)**

**104013C4h - PICA(00F1h) - GPUREG_TEXENV4_OPERAND (R/W)**

**104013E4h - PICA(00F9h) - GPUREG_TEXENV5_OPERAND (R/W)**

"Configures a texture combiner's operands."

uh, R/W mask is only 00111333h...?

```

| 0-3 RGB operand 0 (0..0Dh, see below) ;\uh, but R/W mask suggest
 4-7 RGB operand 1 (0..0Dh, see below) ; range 0..3 only
 8-11 RGB operand 2 (0..0Dh, see below) ;/
 12-14 Alpha operand 0 (0..7, see below) ;\uh, but R/W mask suggest
 16-18 Alpha operand 1 (0..7, see below) ; range 0..1 only
 20-22 Alpha operand 2 (0..7, see below) ;/ |
| --- |

RGB operand values:

```

| 00h = Source color
 01h = One minus source color
 02h = Source alpha
 03h = One minus source alpha
 04h = Source red ;\
 05h = One minus source red ;
 08h = Source green ; uh, really? R/W mask is only 2bit
 09h = One minus source green ;
 0Ch = Source blue ;
 0Dh = One minus source blue ;/ |
| --- |

Alpha operand values:

```

| 0h = Source alpha
 1h = One minus source alpha
 2h = Source red ;\
 3h = One minus source red ;
 4h = Source green ; uh, really? R/W mask is only 1bit
 5h = One minus source green ;
 6h = Source blue ;
 7h = One minus source blue ;/ |
| --- |

**10401308h - PICA(00C2h) - GPUREG_TEXENV0_COMBINER (R/W)**

**10401328h - PICA(00CAh) - GPUREG_TEXENV1_COMBINER (R/W)**

**10401348h - PICA(00D2h) - GPUREG_TEXENV2_COMBINER (R/W)**

**10401368h - PICA(00DAh) - GPUREG_TEXENV3_COMBINER (R/W)**

**104013C8h - PICA(00F2h) - GPUREG_TEXENV4_COMBINER (R/W)**

**104013E8h - PICA(00FAh) - GPUREG_TEXENV5_COMBINER (R/W)**

```

| 0-3 RGB combine mode (0..9, see below)
 16-19 Alpha combine mode (0..9, see below) |
| --- |

Combine mode values:

```

| 0h = Replace
 1h = Modulate
 2h = Add
 3h = Add signed
 4h = Interpolate
 5h = Subtract
 6h = Dot3 RGB
 7h = Dot3 RGBA
 8h = Multiply then add
 9h = Add then multiply
 Ah..Fh = ? |
| --- |

**1040130Ch - PICA(00C3h) - GPUREG_TEXENV0_COLOR (R/W)**

**1040132Ch - PICA(00CBh) - GPUREG_TEXENV1_COLOR (R/W)**

**1040134Ch - PICA(00D3h) - GPUREG_TEXENV2_COLOR (R/W)**

**1040136Ch - PICA(00DBh) - GPUREG_TEXENV3_COLOR (R/W)**

**104013CCh - PICA(00F3h) - GPUREG_TEXENV4_COLOR (R/W)**

**104013ECh - PICA(00FBh) - GPUREG_TEXENV5_COLOR (R/W)**

```

| 0-7 Constant color Red (unsigned, 00h..FFh)
 8-15 Constant color Green (unsigned, 00h..FFh)
 16-23 Constant color Blue (unsigned, 00h..FFh)
 24-31 Constant color Alpha (unsigned, 00h..FFh) |
| --- |

**10401310h - PICA(00C4h) - GPUREG_TEXENV0_SCALE (R/W)**

**10401330h - PICA(00CCh) - GPUREG_TEXENV1_SCALE (R/W)**

**10401350h - PICA(00D4h) - GPUREG_TEXENV2_SCALE (R/W)**

**10401370h - PICA(00DCh) - GPUREG_TEXENV3_SCALE (R/W)**

**104013D0h - PICA(00F4h) - GPUREG_TEXENV4_SCALE (R/W)**

**104013F0h - PICA(00FCh) - GPUREG_TEXENV5_SCALE (R/W)**

```

| 0-1 RGB scale (0=1x, 1=2x, 2=4x, 3=?)
 16-17 Alpha scale (0=1x, 1=2x, 2=4x, 3=?) |
| --- |

**1040131Ch - PICA(00C7h) - GPUREG_undoc_1040131Ch (R/W=00000007h)**

```

| 0-2 UNKNOWN (0..7) (R/W) |
| --- |

**10401380h - PICA(00E0h) - GPUREG_TEXENV_UPDATE_BUFFER (R/W)**

```

| 0-2 Fog mode (0=Disabled, 5=Fog, 7=Gas, other=?)
 3 Shading density source (0=Plain density, 1=Depth density)
 8 TexEnv 1 RGB buffer input (0=Previous Buffer, 1=Previous)
 9 TexEnv 2 RGB buffer input (0=Previous Buffer, 1=Previous)
 10 TexEnv 3 RGB buffer input (0=Previous Buffer, 1=Previous)
 11 TexEnv 4 RGB buffer input (0=Previous Buffer, 1=Previous)
 12 TexEnv 1 Alpha buffer input (0=Previous Buffer, 1=Previous)
 13 TexEnv 2 Alpha buffer input (0=Previous Buffer, 1=Previous)
 14 TexEnv 3 Alpha buffer input (0=Previous Buffer, 1=Previous)
 15 TexEnv 4 Alpha buffer input (0=Previous Buffer, 1=Previous)
 16 Z flip (0=Don't flip,1=Flip)
 24-25 UNKNOWN "0x0" (R/W) |
| --- |

This register is shared between the gas/fog mode configuration and texture
combiner buffer inputs. Texture combiner buffer inputs are typically written
with a mask of 0x2, and the gas/fog mode configuration is typically written
with a mask of 0x5.

**10401384h - PICA(00E1h) - GPUREG_FOG_COLOR (R/W)**

```

| 0-7 Fog color Red (unsigned, 00h..FFh)
 8-15 Fog color Green (unsigned, 00h..FFh)
 16-23 Fog color Blue (unsigned, 00h..FFh) |
| --- |

**10401388h - PICA(00E2h) - GPUREG_undoc_10401388h (R/W=0000FFFFh)**

**1040138Ch - PICA(00E3h) - GPUREG_undoc_1040138Ch (R/W=0000FFFFh)**

```

| 0-15 UNKNOWN (0..FFFFh) (R/W) |
| --- |

**10401390h - PICA(00E4h) - GPUREG_GAS_ATTENUATION (R/W)**

```

| 0-15 float1.5.10, Gas density attenuation |
| --- |

**10401394h - PICA(00E5h) - GPUREG_GAS_ACCMAX (R/W)**

```

| 0-15 float1.5.10, Gas maximum density accumulation |
| --- |

**10401398h - PICA(00E6h) - GPUREG_FOG_LUT_INDEX (R/W)**

```

| 0-6 Index for FOG_LUT_DATA (0..7Fh) |
| --- |

The index is incremented on FOG_LUT_DATA writes (but not incremented on reads).

**104013A0h+i*4 - PICA(00E8h+i) - GPUREG_FOG_LUT_DATA(0..7) (R/W)**

```

| 0-12 fixed1.1.11, Difference from next entry
 13-23 fixed0.0.11, Value |
| --- |

These registers behave as a FIFO queue. Each write to these registers writes
the provided value to the fog look-up table, starting at the index selected
with GPUREG_FOG_LUT_INDEX.

**104013F4h - PICA(00FDh) - GPUREG_TEXENV_BUFFER_COLOR (R/W)**

```

| 0-7 Texture combiner buffer color Red (unsigned, 00h..FFh)
 8-15 Texture combiner buffer color Green (unsigned, 00h..FFh)
 16-23 Texture combiner buffer color Blue (unsigned, 00h..FFh)
 24-31 Texture combiner buffer color Alpha (unsigned, 00h..FFh) |
| --- |