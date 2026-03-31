# Gpuinternalregistersfragmentlightingregisters

> Source: https://problemkaputt.de/gbatek.htm
> Section: Gpuinternalregistersfragmentlightingregisters

3DS GPU Internal Registers - Fragment Lighting registers 
| |
| --- |

 **10401500h+i*40h - PICA(0140h+10h*(0..7)) - GPUREG_LIGHTi_SPECULAR0 (R/W)**
 **10401504h+i*40h - PICA(0141h+10h*(0..7)) - GPUREG_LIGHTi_SPECULAR1 (R/W)**
 **10401508h+i*40h - PICA(0142h+10h*(0..7)) - GPUREG_LIGHTi_DIFFUSE (R/W)**
 **1040150Ch+i*40h - PICA(0143h+10h*(0..7)) - GPUREG_LIGHTi_AMBIENT (R/W)**
 **10401700h - PICA(01C0h) - GPUREG_LIGHTING_AMBIENT (R/W)**
 
```

| 0-7 unsigned, Blue
 8-9 UNKNOWN (maybe Blue MSBs? if so, is it really unsigned?) (R/W)
 10-17 unsigned, Green
 18-19 UNKNOWN (R/W)
 20-27 unsigned, Red
 28-29 UNKNOWN (R/W) |
| --- |

These registers contain the colors of the corresponding lights, usually set to
following values:

```

| GPUREG_LIGHTi_SPECULAR0 = material_specular0*lightX_specular0
 GPUREG_LIGHTi_SPECULAR1 = material_specular1*lightX_specular1
 GPUREG_LIGHTi_DIFFUSE = material_diffuse*lightX_diffuse
 GPUREG_LIGHTi_AMBIENT = material_ambient*lightX_ambient
 GPUREG_LIGHTING_AMBIENT = material_ambient*scene_ambient + material_emission |
| --- |

**10401510h+i*40h - PICA(0144h+10h*(0..7)) - GPUREG_LIGHTi_VECTOR_LOW (R/W)**

**10401514h+i*40h - PICA(0145h+10h*(0..7)) - GPUREG_LIGHTi_VECTOR_HIGH (R/W)**

```

| 0-15 float1.5.10, X coordinate
 16-31 float1.5.10, Y coordinate
 32-47 float1.5.10, Z coordinate |
| --- |

Light position (for a positional light) or the light direction vector (for a
directional light) of the corresponding light.

Uh, does the direction vector need to be normalized? And how does it differ
from SPOTDIR direction vector?

**10401518h+i*40h - PICA(0146h+10h*(0..7)) - GPUREG_LIGHTi_SPOTDIR_LOW (R/W)**

**1040151Ch+i*40h - PICA(0147h+10h*(0..7)) - GPUREG_LIGHTi_SPOTDIR_HIGH (R/W)**

```

| 0-12 fixed1.1.11, X coordinate (negated)
 16-28 fixed1.1.11, Y coordinate (negated)
 32-44 fixed1.1.11, Z coordinate (negated) |
| --- |

Spot direction (unitary) vector of the corresponding light.

**10401524h+i*40h - PICA(0149h+10h*(0..7)) - GPUREG_LIGHTi_CONFIG (R/W)**

```

| 0 Light type (0=Positional light, 1=Directional light)
 1 Two side diffuse (0=One side, 1=Both sides)
 2 Use geometric factor 0 (0=Don't use, 1=Use)
 3 Use geometric factor 1 (0=Don't use, 1=Use) |
| --- |

Configures a light's properties.

**10401528h+i*40h - PICA(014Ah+10h*(0..7)) - GPUREG_LIGHTi_ATTENUATION_BIAS**

**1040152Ch+i*40h - PICA(014Bh+10h*(0..7)) - GPUREG_LIGHTi_ATTENUATION_SCALE**

```

| 0-19 float1.7.12, Distance attenuation value (R/W) |
| --- |

For the odd 20bit floating point format, one could do:

```

| (float24)/10h ;convert float1.7.16 to float1.7.12 |
| --- |

Distance attenuation BIAS and SCALE values of the corresponding light.

The attenuation factor is DA(clip(BIAS + SCALE*distance, 0.0, 1.0)).

```

| _______________________ |
| --- |

**1040123Ch - PICA(008Fh) - GPUREG_LIGHTING_ENABLE (R/W)**

```

| 0 Enable Lighting (0=Disable, 1=Enable) |
| --- |

Uh, that is in texture register area... is that texture specific?

**10401718h - PICA(01C6h) - GPUREG_LIGHTING_DISABLE (R/W)**

```

| 0 Disable (0=Enable, 1=Disable) |
| --- |

**10401708h - PICA(01C2h) - GPUREG_LIGHTING_NUM_LIGHTS (R/W)**

```

| 0-2 Number of active lights - 1 (0..7 = 1..8 Lights) |
| --- |

To use 0 lights, one seems to have to disable lighting... unknown if one can
keep GPUREG_LIGHTING_AMBIENT enabled in that state.

**1040170Ch - PICA(01C3h) - GPUREG_LIGHTING_CONFIG0 (R/W)**

```

| 0 Shadow factor enable (0=Disable, 1=Enable)
 (usually accompanied by bit 16, 17, or 18)
 2-3 Fresnel selector (0=None, 1=Primary, 2=Secondary, 3=Both alpha's)
 4-7 Light environment configuration (see below)
 8-11 UNKNOWN "0x4" (R/W)
 16 Apply shadow attenuation to primary color (0=Don't apply, 1=Apply)
 17 Apply shadow attenuation to secondary color (0=Don't apply, 1=Apply)
 18 Invert shadow attenuation (0=Don't invert, 1=Invert)
 19 Apply shadow attenuation to alpha component (0=Don't apply, 1=Apply)
 20-21 UNKNOWN (R/W)
 22-23 Bump map texture unit
 24-25 Shadow map texture unit
 26 UNKNOWN (R/W)
 27 Clamp highlights (0=Disable, 1=Enable)
 28-29 Bump mode (0=Not used, 1=Use as bump map, 2=Use as tangent map)
 30 Recalculate bump vectors (0=Enable, 1=Disable)
 (usually set to 1 when bump mode is not 0)
 31 UNKNOWN "0x1" (R/W) |
| --- |

Fresnel selector values:

```

| 0h = None
 1h = Primary alpha
 2h = Secondary alpha
 3h = Primary and secondary alpha |
| --- |

Light environment configuration values:

```

| Value Description Available LUTs
 00h = Configuration 0 D0, RR, SP, DA
 01h = Configuration 1 FR, RR, SP, DA
 02h = Configuration 2 D0, D1, RR, DA
 03h = Configuration 3 D0, D1, FR, DA
 04h = Configuration 4 All except for FR
 05h = Configuration 5 All except for D1
 06h = Configuration 6 All except for RB and RG
 08h = Configuration 7 All |
| --- |

The light environment configuration controls which LUTs are available for use.
If a LUT is not available in the selected configuration, its value will always
read a constant 1.0 regardless of the enable state in GPUREG_LIGHTING_CONFIG1.
If RR is enabled but not RG or RB, the output of RR is used for the three
components; Red, Green and Blue.

**10401710h - PICA(01C4h) - GPUREG_LIGHTING_CONFIG1 (R/W)**

```

| 0-7 Fragment light source 0-7 shadows (0=Enable, 1=Disable)
 8-15 Fragment light source 0-7 spot light (0=Enable, 1=Disable)
 16 Term 0 distribution component D0 LUT (0=Enable, 1=Disable)
 17 Term 1 distribution component D1 LUT (0=Enable, 1=Disable)
 18 0x1 (1)
 19 Fresnel FR LUT disabled (0=Enable, 1=Disable)
 20 Term 1 reflection component RB LUT (0=Enable, 1=Disable)
 21 Term 1 reflection component RG LUT (0=Enable, 1=Disable)
 22 Term 1 reflection component RR LUT (0=Enable, 1=Disable)
 24-31 Fragment light source 0-7 distance attenuation (0=Enable, 1=Disable) |
| --- |

Allows to disable various aspects of the light environment.

**10401714h - PICA(01C5h) - GPUREG_LIGHTING_LUT_INDEX (R/W)**

```

| 0-7 Starting index (00h..FFh) ;\for reading/writing via
 8-12 Look-up table number (see below) ;/GPUREG_LIGHTING_LUT_DATA(i) |
| --- |

Lookup table numbers:

```

| 00h = D0 ;\Distribution, whatever that is?
 01h = D1 ;/
 03h = FR ;-Fresnel, whatever that is?
 04h = RB ;\
 05h = RG ; Reflection with separate blue/green/red tables?
 06h = RR ;/
 08h-0Fh = SP0-7 ;-Spotlight? ;\maybe for light 0..7 ?
 10h-17h = DA0-7 ;-Distance attenuation? ;/ |
| --- |

Note: The index in bit0-7 is auto-incremented when writing (or reading)
LUT_DATA registers; there is no carry-out to table number in bit8-12.

**10401720h+i*4 - PICA(01C8h+i) - GPUREG_LIGHTING_LUT_DATA(0..7) (R/W)**

```

| 0-11 fixed0.0.12, Entry value
 12-23 fixed1.0.11, Absolute value of the difference between the next
 entry and this entry, used to implement linear interpolation
 uh, how/why is that a ABSOLUTE value WITH SIGN bit? |
| --- |

A LUT contains data for the input domain [-1.0, 1.0], which is indexed using a
signed 8bit number [-128, 127]. Therefore a LUT contains 256 entries. The index
of a value is (int)(x/127.0f) & 0xFF.

DA: The input domain is [0.0, 1.0], and the index is an unsigned 8bit number
[0, 255] instead.

**10401740h - PICA(01D0h) - GPUREG_LIGHTING_LUTINPUT_ABS (R/W)**

```

| 0 UNKNOWN (R/W)
 1 abs() flag for the input of D0 (0=Enable, 1=Disable)
 4 UNKNOWN (R/W)
 5 abs() flag for the input of D1 (0=Enable, 1=Disable)
 8 UNKNOWN (R/W)
 9 abs() flag for the input of SP (0=Enable, 1=Disable)
 12 UNKNOWN (R/W)
 13 abs() flag for the input of FR (0=Enable, 1=Disable)
 16 UNKNOWN (R/W)
 17 abs() flag for the input of RB (0=Enable, 1=Disable)
 20 UNKNOWN (R/W)
 21 abs() flag for the input of RG (0=Enable, 1=Disable)
 24 UNKNOWN (R/W)
 25 abs() flag for the input of RR (0=Enable, 1=Disable) |
| --- |

Controls whether the absolute value of the input is taken before using a LUT.

**10401744h - PICA(01D1h) - GPUREG_LIGHTING_LUTINPUT_SELECT (R/W)**

```

| 0-2 Input selector for D0 ;\Input selector values:
 4-6 Input selector for D1 ; 0h = N . H ;uh, dot symbol?
 8-10 Input selector for SP ; 1h = V . H ; ... maybe multiply?
 12-14 Input selector for FR ; 2h = N . V ;uh, who is N,H,L,V,P
 16-18 Input selector for RB ; 3h = L . N ; and greek symbol?
 20-22 Input selector for RG ; 4h = -L . P (aka Spotlight aka SP)
 24-26 Input selector for RR ;/ 5h = cos <greek symbol> (aka CP) |
| --- |

Selects the input from LUTs.

```

| N = Normal Vector?
 V = View Vector?
 L = Light Vector?
 H = ?
 P = ?
 cos <greek symbol> = ? |
| --- |

**10401748h - PICA(01D2h) - GPUREG_LIGHTING_LUTINPUT_SCALE (R/W)**

```

| 0-2 Scaler selector for D0 ;\Scaler selector values:
 4-6 Scaler selector for D1 ; 0h = 1x
 8-10 Scaler selector for SP ; 1h = 2x
 12-14 Scaler selector for FR ; 2h = 4x
 16-18 Scaler selector for RB ; 3h = 8x
 20-22 Scaler selector for RG ; 6h = 0.25x
 24-26 Scaler selector for RR ;/ 7h = 0.5x
 28-30 UNKNOWN (R/W) |
| --- |

Controls the scaling that is applied to the output of a LUT.

**1040174Ch - PICA(01D3h) - GPUREG_undoc_1040174Ch (R/W=00000001h)**

```

| 0 UNKNOWN (0..1) (R/W) |
| --- |

**10401750h - PICA(01D4h) - GPUREG_undoc_10401750h (R/W=0FFFFF03h)**

```

| 0-1 UNKNOWN (0..3) (R/W)
 8-31 UNKNOWN (0..FFFFFFh) (R/W) |
| --- |

**10401754h - PICA(01D5h) - GPUREG_undoc_10401754h (R/W=1FFF1FFFh)**

**10401758h - PICA(01D6h) - GPUREG_undoc_10401758h (R/W=1FFF1FFFh)**

```

| 0-12 UNKNOWN (0..1FFFh) (R/W)
 16-28 UNKNOWN (0..1FFFh) (R/W) |
| --- |

**1040175Ch - PICA(01D7h) - GPUREG_undoc_1040175Ch (R/W=000FFFFFh)**

**10401760h - PICA(01D8h) - GPUREG_undoc_10401760h (R/W=000FFFFFh)**

```

| 0-19 UNKNOWN (0..FFFFFh) (R/W) |
| --- |

**10401764h - PICA(01D9h) - GPUREG_LIGHTING_LIGHT_PERMUTATION (R/W)**

```

| 0-2 ID of the 1st enabled light (0..7)
 4-6 ID of the 2nd enabled light (0..7)
 8-10 ID of the 3rd enabled light (0..7)
 12-14 ID of the 4th enabled light (0..7)
 16-18 ID of the 5th enabled light (0..7)
 20-22 ID of the 6th enabled light (0..7)
 24-26 ID of the 7th enabled light (0..7)
 28-30 ID of the 8th enabled light (0..7) |
| --- |

Sets the IDs of enabled light sources (1st..8th light does refer to the number
of lights in GPUREG_LIGHTING_NUM_LIGHTS).

Uh, LIGHT_PERMUTATION is a scary homebrew slang for LIGHT_IDs?