# Gpuinternalregistersshaderregisters

> Source: https://problemkaputt.de/gbatek.htm
> Section: Gpuinternalregistersshaderregisters

3DS GPU Internal Registers - Shader registers 
| |
| --- |

 There are four shader units, each having its own set of I/O registers:
 
```

| 10401A00h - PICA(0280h..) - Shader 0 (Geometry Shader, aka GSH)
 10401AC0h - PICA(02B0h..) - Shader 1 (Vertex Shader, aka VSH)
 10401B80h - PICA(02E0h..) - Shader 2 (Unknown purpose)
 10401C40h - PICA(0310h..) - Shader 3 (Unknown purpose) |
| --- |

The vertex shader can reportedly use three shader units at once (or all four
units, when not using the geometry shader). Even when using multiple units as
vertex shader, one does only need to initialize the Shader 1 registers.

Unknown if the Shader 2 and 3 registers are having any special purpose for yet
unknown special effects (maybe for the undescribed "reserved geometry shader
subdivision" feature, or maybe for the "procedural texture" unit, or maybe they
are just dummy registers).

```

| ____________________________ Input/Output Config _____________________________ |
| --- |

**10401A24h - PICA(0289h) - GPUREG_GSH_INPUTBUFFER_CONFIG (R/W)**

**10401AE4h - PICA(02B9h) - GPUREG_VSH_INPUTBUFFER_CONFIG (R/W)**

**10401BA4h - PICA(02E9h) - GPUREG_VSH2_INPUTBUFFER_CONFIG (R/W)**

**10401C64h - PICA(0319h) - GPUREG_VSH3_INPUTBUFFER_CONFIG (R/W)**

```

| 0-3 Number of Input vertex attributes - 1
 4-7 Unused (0)
 8 Use reserved geometry shader subdivision (0=Don't use, 1=Use) (uh?)
 9-26 Unused (0)
 27 Whatever, should be 1 for geometry shader (R/W)
 28 Whatever, should be 0 (R/W)
 29 Whatever, should be 1 for vertex shader (R/W)
 30 Unused (0)
 31 Whatever, should be 1 for vertex shader (R/W) |
| --- |

Configures the shader unit's input buffer.

**10401A2Ch - PICA(028Bh) - GPUREG_GSH_ATTR_PERMUTATION_LOW (R/W)**

**10401A30h - PICA(028Ch) - GPUREG_GSH_ATTR_PERMUTATION_HIGH (R/W)**

**10401AECh - PICA(02BBh) - GPUREG_VSH_ATTR_PERMUTATION_LOW (R/W)**

**10401AF0h - PICA(02BCh) - GPUREG_VSH_ATTR_PERMUTATION_HIGH (R/W)**

**10401BACh - PICA(02EBh) - GPUREG_VSH2_ATTR_PERMUTATION_LOW (R/W)**

**10401BB0h - PICA(02ECh) - GPUREG_VSH2_ATTR_PERMUTATION_HIGH (R/W)**

**10401C6Ch - PICA(031Bh) - GPUREG_VSH3_ATTR_PERMUTATION_LOW (R/W)**

**10401C70h - PICA(031Ch) - GPUREG_VSH3_ATTR_PERMUTATION_HIGH (R/W)**

```

| 0-3 Vertex attribute 0 input register index (0..15=v0..v15)
 4-7 Vertex attribute 1 input register index
 8-11 Vertex attribute 2 input register index
 12-15 Vertex attribute 3 input register index
 16-19 Vertex attribute 4 input register index
 20-23 Vertex attribute 5 input register index
 24-27 Vertex attribute 6 input register index
 28-31 Vertex attribute 7 input register index
 32-35 Vertex attribute 8 input register index
 36-39 Vertex attribute 9 input register index
 40-43 Vertex attribute 10 input register index
 44-47 Vertex attribute 11 input register index
 48-51 Vertex attribute 12 input register index ;\uh, how does one
 52-55 Vertex attribute 13 input register index ; set attr 12-15?
 56-59 Vertex attribute 14 input register index ; maybe FROM
 60-63 Vertex attribute 15 input register index ;/geometry shader? |
| --- |

Sets the shader unit input register index which will correspond to each
attribute contained by the input buffer (which in the case of geometry shaders
is the vertex shader output buffer (uh, that says that there is ONE vertex
shader, and MULTIPLE geometry shaders?)) for attributes 0 through 15.

For example, having LOW.bit0-3 set to 5 means that, in the shader program, v5
will contain the input buffer's 1st attribute.

For example, having HIGH.bit0-3 set to 5 means that, in the shader program, v5
will contain the input buffer's 9th attribute.

somewhat semantic?

Uh, ATTR_PERMUTATION is a scary homebrew slang for ATTR_IDs?

**10401A34h - PICA(028Dh) - GPUREG_GSH_OUTMAP_MASK (R/W)**

**10401AF4h - PICA(02BDh) - GPUREG_VSH_OUTMAP_MASK (R/W)**

**10401BB4h - PICA(02EDh) - GPUREG_VSH2_OUTMAP_MASK (R/W)**

**10401C74h - PICA(031Dh) - GPUREG_VSH3_OUTMAP_MASK (R/W)**

```

| 0 Output register o0 (0=Disable, 1=Enable)
 1 Output register o1 (0=Disable, 1=Enable)
 2 Output register o2 (0=Disable, 1=Enable)
 3 Output register o3 (0=Disable, 1=Enable)
 4 Output register o4 (0=Disable, 1=Enable)
 5 Output register o5 (0=Disable, 1=Enable)
 6 Output register o6 (0=Disable, 1=Enable)
 7 Output register o7 (0=Disable, 1=Enable) (vertex shader only, uh?)
 8 Output register o8 (0=Disable, 1=Enable) (vertex shader only, uh?)
 9 Output register o9 (0=Disable, 1=Enable) (vertex shader only, uh?)
 10 Output register o10 (0=Disable, 1=Enable) (vertex shader only, uh?)
 11 Output register o11 (0=Disable, 1=Enable) (vertex shader only, uh?)
 12 Output register o12 (0=Disable, 1=Enable) (vertex shader only, uh?)
 13 Output register o13 (0=Disable, 1=Enable) (vertex shader only, uh?)
 14 Output register o14 (0=Disable, 1=Enable) (vertex shader only, uh?)
 15 Output register o15 (0=Disable, 1=Enable) (vertex shader only, uh?)
 16-26 UNKNOWN (R/W)
 16-31 UNKNOWN "0x0" (uh, but only bit27-31 are always 0) |
| --- |

Toggles a shader unit's output registers.

See also: GPUREG_SH_OUTMAP_Oi

```

| _____________________________ Uniform Registers ______________________________ |
| --- |

Uniform registers can contain general purpose constants. That is, they are
usually kept set to constant values throughout sending multiple vertices
(unlike position/texture coordinates or other vertex attributes that differ on
each vertex).

**10401A00h - PICA(0280h) - GPUREG_GSH_BOOLUNIFORM (R/W)**

**10401AC0h - PICA(02B0h) - GPUREG_VSH_BOOLUNIFORM (R/W)**

**10401B80h - PICA(02E0h) - GPUREG_VSH2_BOOLUNIFORM (R/W)**

**10401C40h - PICA(0310h) - GPUREG_VSH3_BOOLUNIFORM (R/W)**

```

| 0-15 Boolean register b0..b15 value (0=False, 1=True)
 16-27 For GSH: UNKNOWN (0..0FFFh) (R/W) ;\unknown, but R/W mask resembles
 16-24 For VSH: UNKNOWN (0..01FFh) (R/W) ;/GPUREG_xSH_ENTRYPOINT !
 16-31 UNKNOWN "0x7FFF" (uh, but bit28-31 are always 0?) (for VSH: 25-31) |
| --- |

The sixteen 1bit boolean registers (B0..B15) can be used by conditional shader
opcodes. Uh, elsewhere it's claimed that there are only eight bool registers?

Bit15 seems to be automatically set after executing shader code?

Bit16 and up seem to contain an opcode address?

**10401A04h+i*4 - PICA(0281h+i) - GPUREG_GSH_INTUNIFORM_I0..I3 (R/W)**

**10401AC4h+i*4 - PICA(02B1h+i) - GPUREG_VSH_INTUNIFORM_I0..I3 (R/W)**

**10401B84h+i*4 - PICA(02E1h+i) - GPUREG_VSH2_INTUNIFORM_I0..I3 (R/W)**

**10401C44h+i*4 - PICA(0311h+i) - GPUREG_VSH3_INTUNIFORM_I0..I3 (R/W)**

```

| 0-7 unsigned, Integer register ii X value aka INT.x Loop count-1
 8-15 unsigned, Integer register ii Y value aka INT.y Loop starting index
 16-23 unsigned, Integer register ii Z value aka INT.z Loop index step
 24-31 Unused (0) |
| --- |

Each of the four integer registers (I0..I3) contains three 8bit integer values
(X,Y,Z). The integers are used to initalize loop counters and indexing.

**10401A40h - PICA(0290h) - GPUREG_GSH_FLOATUNIFORM_INDEX (W)**

**10401B00h - PICA(02C0h) - GPUREG_VSH_FLOATUNIFORM_INDEX (W)**

**10401BC0h - PICA(02F0h) - GPUREG_VSH2_FLOATUNIFORM_INDEX (W)**

**10401C80h - PICA(0320h) - GPUREG_VSH3_FLOATUNIFORM_INDEX (W)**

```

| 0-7 Transfer index for FLOATUNIFORM_DATA writes (0..95 = C0..C95)
 31 Transfer mode (0=float1.7.16, 1=float1.8.23) |
| --- |

**10401A44h+i*4 - PICA(0291h+i) - GPUREG_GSH_FLOATUNIFORM_DATA(0..7) (W)**

**10401B04h+i*4 - PICA(02C1h+i) - GPUREG_VSH_FLOATUNIFORM_DATA(0..7) (W)**

**10401BC4h+i*4 - PICA(02F1h+i) - GPUREG_VSH2_FLOATUNIFORM_DATA(0..7) (W)**

**10401C84h+i*4 - PICA(0321h+i) - GPUREG_VSH3_FLOATUNIFORM_DATA(0..7) (W)**

Each of the 96 float uniform registers (C0..C95) contains four float values
(X,Y,Z,W). The floats can contain general purpose constants, colors, vectors,
or matrices (eg. when using four such registers as matrix with 4x4 float
values).

"The data format which should be written to it depends on the transfer mode set
with GPUREG_SH_FLOATUNIFORM_INDEX. These registers function as a FIFO queue:
after each time a 4-component uniform register is successfully set, the target
register index is incremented, meaning that groups of uniforms with contiguous
register IDs can be set with only one initial write to
GPUREG_SH_FLOATUNIFORM_INDEX."

**float24 (float1.7.16) transfer mode:**

"Data should be sent by writing three words which are the concatenation of the
float24 value of the uniform register's 4 components, in the "reverse order".
Assuming each letter corresponds to 4 bits, the format becomes:"

```

| (uh, is that the aforementioned "reverse order", or is it unreversed here?)
 1st word : ZZWWWWWW ;uh, reverse of GPUREG_FIXEDATTRIB_DATA ??
 2nd word : YYYYZZZZ
 3rd word : XXXXXXYY |
| --- |

**float32 (float1.8.23) transfer mode:**

"Data should be sent by writing four words which are each the float32 value of
the uniform register's 4 components, in the reverse order... uh, who/what/how
is that reverse order??? probably alike as shown below (or reverse thereof?):"

```

| 1st word : WWWWWWWW
 2nd word : ZZZZZZZZ
 3rd word : YYYYYYYY
 4th word : XXXXXXXX |
| --- |

```

| ____________________________ Opcodes and Opdesc's ____________________________ |
| --- |

**10401A28h - PICA(028Ah) - GPUREG_GSH_ENTRYPOINT (R/W)**

**10401AE8h - PICA(02BAh) - GPUREG_VSH_ENTRYPOINT (R/W)**

**10401BA8h - PICA(02EAh) - GPUREG_VSH2_ENTRYPOINT (R/W)**

**10401C68h - PICA(031Ah) - GPUREG_VSH3_ENTRYPOINT (R/W)**

```

| 0-15 Code entry point offset, in 32-bit words
 16-31 0x7FFF ;maybe max/end address for error-checks? |
| --- |

uh, but, for GSH, R/W mask is 0FFF0FFFh.

uh, but, for VSH, R/W mask is 01FF01FFh.

**10401A6Ch - PICA(029Bh) - GPUREG_GSH_CODETRANSFER_INDEX (W)**

**10401B2Ch - PICA(02CBh) - GPUREG_VSH_CODETRANSFER_INDEX (W)**

**10401BECh - PICA(02FBh) - GPUREG_VSH2_CODETRANSFER_INDEX (W)**

**10401CACh - PICA(032Bh) - GPUREG_VSH3_CODETRANSFER_INDEX (W)**

```

| 0-11 Target shader code offset, in WHAT units? 4-byte maybe? |
| --- |

Offset at which upcoming shader code data transferred through
GPUREG_SH_CODETRANSFER_DATAi should be written.

**10401A70h+i*4 - PICA(029Ch+i) - GPUREG_GSH_CODETRANSFER_DATA(0..7) (W)**

**10401B30h+i*4 - PICA(02CCh+i) - GPUREG_VSH_CODETRANSFER_DATA(0..7) (W)**

**10401BF0h+i*4 - PICA(02FCh+i) - GPUREG_VSH2_CODETRANSFER_DATA(0..7) (W)**

**10401CB0h+i*4 - PICA(032Ch+i) - GPUREG_VSH3_CODETRANSFER_DATA(0..7) (W)**

```

| 0-31 Shader instruction data |
| --- |

Used to transfer shader code data. These registers behave as a FIFO queue: each
write to these registers writes the provided value to the GPU shader code
memory bank at the offset initially set by GPUREG_SH_CODETRANSFER_INDEX. The
offset in question is incremented after each write to this register.

**10401A3Ch - PICA(028Fh) - GPUREG_GSH_CODETRANSFER_END (W)**

**10401AFCh - PICA(02BFh) - GPUREG_VSH_CODETRANSFER_END (W)**

**10401BBCh - PICA(02EFh) - GPUREG_VSH2_CODETRANSFER_END (W)**

**10401C7Ch - PICA(031Fh) - GPUREG_VSH3_CODETRANSFER_END (W)**

```

| 0-31 Don't care (writing any value works) |
| --- |

Unknown what that does. It seems to be needed at least once after first code
upload (but is usually done after each shader code upload). Maybe enables code
execution, or invalidates some cache?

**10401A94h - PICA(02A5h) - GPUREG_GSH_OPDESCS_INDEX (W)**

**10401B54h - PICA(02D5h) - GPUREG_VSH_OPDESCS_INDEX (W)**

**10401C14h - PICA(0305h) - GPUREG_VSH2_OPDESCS_INDEX (W)**

**10401CD4h - PICA(0335h) - GPUREG_VSH3_OPDESCS_INDEX (W)**

```

| 0-11 Target shader operand descriptor offset, in WHAT units? |
| --- |

Uh, why 12bit wide? There are only max 128 descriptors per shader.

Offset at which upcoming shader operand descriptor data transferred through
GPUREG_SH_OPDESCS_DATAi should be written.

**10401A98h+i*4 - PICA(02A6h+i) - GPUREG_GSH_OPDESCS_DATA(0..7) (W)**

**10401B58h+i*4 - PICA(02D6h+i) - GPUREG_VSH_OPDESCS_DATA(0..7) (W)**

**10401C18h+i*4 - PICA(0306h+i) - GPUREG_VSH2_OPDESCS_DATA(0..7) (W)**

**10401CD8h+i*4 - PICA(0336h+i) - GPUREG_VSH3_OPDESCS_DATA(0..7) (W)**

```

| 0-31 Shader operand descriptor data |
| --- |

Used to transfer shader operand descriptor data. These registers behave as a
FIFO queue: each write to these registers writes the provided value to the GPU
shader operand descriptor memory bank at the offset initially set by
GPUREG_SH_OPDESCS_INDEX. The offset in question is incremented after each write
to this register.