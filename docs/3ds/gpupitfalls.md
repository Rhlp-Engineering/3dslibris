# Gpupitfalls

> Source: https://problemkaputt.de/gbatek.htm
> Section: Gpupitfalls

3DS GPU Pitfalls 
| |
| --- |

 **Vertex attribute alignment**
 Vertex components which are defined through GPUREG_ATTR_BUFi_CONFIG_LOW will be accessed aligned by the GPU.
 Vertex attributes will be aligned to their component element size.
 Padding attributes (Component type > 11) will always aligned to 4 byte offets into the buffer.
 The stride which is passed to the GPU should be passed unaligned.
 
 **Vertex stride in GPUREG_ATTR_BUFi_CONFIG_HIGH**
 The vertex stride set in GPUREG_ATTR_BUFi_CONFIG_HIGH must match the actual size of the vertex contained in the buffer or the PICA will freeze or it won't draw anything.
 If you want to use a different stride you have to pad the data accordingly with padding attributes.
 
 **Output mapping in GPUREG_SH_OUTMAP_MASK**
 The output masking in GPUREG_SH_OUTMAP_MASK influences how the registers starting at GPUREG_SH_OUTMAP_Oi map to outputs in the shader.
 
 If an output is disabled in GPUREG_SH_OUTMAP_MASK it means that no slot in the GPUREG_SH_OUTMAP_Oi registers is consumed. GPUREG_SH_OUTMAP_TOTAL configures the number of used consecutive slots in the outmap. Example:
 
```

| GPUREG_SH_OUTMAP_TOTAL = 0x00000002 ;2 outputs enabled
 GPUREG_SH_OUTMAP_MASK = 0x00000011 ;o0 enabled, o4 enabled
 GPUREG_SH_OUTMAP_O0 = 0x03020100 ;o0 = pos.xyzw
 GPUREG_SH_OUTMAP_O1 = 0x0B0A0908 ;o4 = color.rgba ;<-- o4 in "O1"!
 GPUREG_SH_OUTMAP_O2 = ... ;(unused) |
| --- |

**Shaders - Configued Output components must be written exactly once**

Each configured output component has to be written exactly once or the PICA
freezes.

**Shaders - MOVA instructions can't be adjacent**

Having 2 consecutive MOVA instructions will freeze the PICA. This can be
relaxed by placing a NOP between 2 MOVAs or by rearranging the code.

**Shader - Special cases**

JMP overwrites the PC *after* the stacks checks (and only if no stack was
popped).

Executing a BREAK on an empty LOOP stack hangs the GPU.

A stack overflow discards the oldest element, so you could think of it as a
queue or a ring buffer.

If the CALL stack is popped four times in a row, the fourth update to its copy
of the PC is missed (the third PC update will be propagated). Probably a
hardware bug.

**Shader - Floating-Point Behavior**

The PICA200 is not IEEE-compliant. It has positive and negative infinities and
NaN, but does not seem to have negative 0. Input and output subnormals are
flushed to +0. The internal floating point format seems to be the same as used
in shader binaries: 1 sign bit, 7 exponent bits, 16 (explicit) mantissa bits.
Several instructions also have behavior that differs from the IEEE functions.
Here are the results from some tests done on hardware (s = largest subnormal, n
= smallest positive normal):

```

| Computation Result Notes
 inf * 0 0 Including inside MUL, MAD, DP4, etc.
 NaN * 0 NaN
 +inf - +inf NaN Indicates +inf is real inf, not FLT_MAX
 rsq(rcp(-inf)) +inf Indicates that there isn't -0.0.
 rcp(-0) +inf no -0 so differs from IEEE where rcp(-0) = -inf
 rcp(0) +inf
 rcp(+inf) 0
 rcp(NaN) NaN
 rsq(-0) +inf no -0 so differs from IEEE where rsq(-0) = -inf
 rsq(-2) NaN
 rsq(+inf) 0
 rsq(-inf) NaN
 rsq(NaN) NaN
 max(0, +inf) +inf
 max(0, -inf) -inf
 max(0, NaN) NaN max violates IEEE but match GLSL spec
 max(NaN, 0) 0
 max(-inf, +inf) +inf
 min(0, +inf) 0
 min(0, -inf) -inf
 min(0, NaN) NaN min violates IEEE but match GLSL spec
 min(NaN, 0) 0
 min(-inf, +inf) -inf
 cmp(s, 0) false cmp does not flush input subnormals
 max(s, 0) s max does not flush input or output subnormals
 mul(s, 2) 0 input subnormals are flushed in arithmetic
 instructions
 mul(n, 0.5) 0 output subnormals are flushed in arithmetic
 instructions |
| --- |

1.0 can be multiplied 63 times by 0.5 until the result compares equal zero.
This is consistent with a 7-bit exponent and output subnormal flushing.

**Command list related**

Invalid GPU command parameters including NaN floats can cause the GPU to hang,
which then causes the "GSP module" to hang as well.