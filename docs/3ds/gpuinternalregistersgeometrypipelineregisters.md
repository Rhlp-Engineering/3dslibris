# Gpuinternalregistersgeometrypipelineregisters

> Source: https://problemkaputt.de/gbatek.htm
> Section: Gpuinternalregistersgeometrypipelineregisters

3DS GPU Internal Registers - Geometry Pipeline registers 
| |
| --- |

 **10401978h - PICA(025Eh) - GPUREG_PRIMITIVE_CONFIG (R/W)**
 
```

| 0-3 Number of vertex shader output map registers - 1 ;see OUTMAP
 8-9 Primitive mode (0=Tri's, 1=Tri-Strip, 2=Tri-Fan, 3=Geometry primitive)
 16 UNKNOWN (R/W)
 __ __ ___ __ __ __
 /\ | / /| /\ /\ /\ | / .'\ ?????????
 /__\ |/ /_|/__\/__\ /__\|/.'___\ ?????????
 0=Triangles, 1=TriangleStrip, 2=TriangleFan, 3=Geometry primitive |
| --- |

**1040197Ch - PICA(025Fh) - GPUREG_RESTART_PRIMITIVE (R/W)**

```

| 0-31 UNKNOWN, usually 1 (writing any value works... but value is R/W) |
| --- |

Used to terminate a triangle strip/fan (eg. to start a new strip, instead of
appending further vertices to the old strip) (note: the restart must be issued
at BEGIN of each strip/fan, not at the end).

Restart occurs on writing any value... but the written value is R/W, so it
might have some purpose?

```

| __________________________ Vertex Attribute Arrays ___________________________ |
| --- |

ATTR_BUF arrays contain vertex attributes (such like coordinates and colors).
The GPU can automatically transfer data from these arrays to the shader unit
(which does then forward the data to the drawing hardware).

There are up to 12 arrays for up to 12 attributes (one can either store
multiple attributes in one single array, or store different attributes in
separate arrays).

**10401804h - PICA(0201h) - GPUREG_ATTR_BUF_FORMAT_LOW (R/W)**

**10401808h - PICA(0202h) - GPUREG_ATTR_BUF_FORMAT_HIGH (R/W)**

```

| 0-3 Vertex attribute 0 type/size ;\these 4bit values contain type/size
 4-7 Vertex attribute 1 type/size ; type in bit0-1:
 8-11 Vertex attribute 2 type/size ; 0 = fixed1.0.7 ;Signed byte
 12-15 Vertex attribute 3 type/size ; 1 = fixed0.0.8 ;Unsigned byte
 16-19 Vertex attribute 4 type/size ; 2 = fixed1.0.15 ;Signed halfword
 20-23 Vertex attribute 5 type/size ; 3 = float1.8.23 ;Float32
 24-27 Vertex attribute 6 type/size ; size in bit2-3:
 28-31 Vertex attribute 7 type/size ; 0 = X ;1D
 32-35 Vertex attribute 8 type/size ; 1 = X,Y ;2D
 36-39 Vertex attribute 9 type/size ; 2 = X,Y,Z ;3D
 40-43 Vertex attribute 10 type/size ; 3 = X,Y,Z,W ;4D
 44-47 Vertex attribute 11 type/size ;/
 48-59 Fixed vertex attribute 0..11 mask flags (0=Array, 1=Fixed?) ???
 60-63 Number of vertex attributes-1 (0..11 = 1..12 attributes) |
| --- |

**10401800h - PICA(0200h) - GPUREG_ATTR_BUF_BASE (R/W)**

```

| 0 Unused (0)
 1-28 Base address for ATTR_BUF's and INDEX_LIST, in 16-byte units
 29-31 Unused (0) |
| --- |

**1040180Ch+i*0Ch - PICA(0203h+3*(0..11)) - GPUREG_ATTR_BUFi_OFFSET (R/W)**

```

| 0-27 Vertex array address, with base GPUREG_ATTR_BUF_BASE, in 1-byte units |
| --- |

Addressing uses Base+Offset+Index (with Index from FIRST_INDEX or INDEX_LIST):

```

| GPUREG_ATTR_BUF_BASE*10h + GPUREG_ATTR_BUFi_OFFSET*1 + Index*ArrayEntrySize |
| --- |

**10401810h+i*0Ch - PICA(0204h+3*(0..11)) - GPUREG_ATTR_BUFi_CONFIG_LOW (R/W)**

**10401814h+i*0Ch - PICA(0205h+3*(0..11)) - GPUREG_ATTR_BUFi_CONFIG_HIGH (R/W)**

```

| 0-3 Array Component 0 ;\
 4-7 Array Component 1 ; These are mapping the array component(s)
 8-11 Array Component 2 ; to vertex attribute numbers:
 12-15 Array Component 3 ; 00h..0Bh = Vertex attribute 0..11
 16-19 Array Component 4 ; 0Ch = 4-byte padding
 20-23 Array Component 5 ; 0Dh = 8-byte padding
 24-27 Array Component 6 ; 0Eh = 12-byte padding
 28-31 Array Component 7 ; 0Fh = 16-byte padding
 32-35 Array Component 8 ; Attribute 0..11 do usually refer to vertex
 36-39 Array Component 9 ; shader registers v0..v11 (unless they are
 40-43 Array Component 10 ; renumbered via "PERMUTATION"...?)
 44-47 Array Component 11 ;/
 48-55 Array Entry Size in bytes (0..FFh) ;total size of above component(s)
 56-59 Unused (0)
 60-63 Number of components in this array (0..12) (0=Disable this array?) |
| --- |

Padding: Does "skip" array entries? (maybe useful when having texturing
disabled, whilst still having texture info in the array?).

Reportedly there is some alignment issue/feature for padding. And also an
alignment issue for components or so; maybe halfword/float32 need to be
aligned? There is no alignment restriction for components that consist of bytes
(eg. 3x8bit RGB does work).

**104018A0h - PICA(0228h) - GPUREG_ATTR_BUF_NUMVERTICES (R/W)**

```

| 0-31 Number of vertices to render (MUL2 for DRAWELEMENTS) |
| --- |

Don't care for FIXEDATTRIB, needed only for ATTR_BUF's.

Counted TWICE for DRAWELEMENTS (once for index-read, and once for array-read)!

XXX are multiple arrays also counted as multiple reads?

**104018B8h - PICA(022Eh) - GPUREG_ATTR_BUF_DRAWARRAYS - Increasing Indices (W)**

**104018BCh - PICA(022Fh) - GPUREG_ATTR_BUF_DRAWELEMENTS - With INDEX_LIST (W)**

Writing to either of these registers starts sending the amount of data
specified in NUMVERTICES from ATTR_BUF's to the vertex shader.

```

| 0-31 Don't care (writing any value starts drawing from ATTR_BUF) |
| --- |

The only difference between DRAWARRAYS and DRAWELEMENTS are the array indices:

```

| DRAWARRAYS uses increasing indices, starting at GPUREG_ATTR_BUF_FIRST_INDEX.
 DRAWELEMENTS uses array indices from GPUREG_ATTR_BUF_INDEX_LIST. |
| --- |

**104018A8h - PICA(022Ah) - GPUREG_ATTR_BUF_FIRST_INDEX (R/W)**

```

| 0-31 Starting index in ATTR_BUF arrays (usually 0, to start with 1st entry) |
| --- |

Used only for for DRAWARRAYS (not used for DRAWELEMENTS, which does instead
read indices from the INDEX_LIST).

**1040189Ch - PICA(0227h) - GPUREG_ATTR_BUF_INDEX_LIST (R/W)**

```

| 0-27 Index List address, with base GPUREG_ATTR_BUF_BASE, in 1-byte units
 31 Index List entry size (0=8bit indices, 1=16bit indices) |
| --- |

Used only for DRAWELEMENTS. The index list allows to re-use ATTR_BUF entries
for multiple vertices (triangle strips are automatically re-using vertices from
the previous triangle, however, with the index list, one can also re-use
vertices from the previous strips; which can save about 50% of memory).

```

| __ __ __ __ __ __ __ __ __
 /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ <-- First Strip
 /__\/__\/__\/__\/__\/__\/__\/__\/__\/__\...
 \ /\ /\ /\ /\ /\ /\ /\ /\ <-- Second Strip
 \/__\/__\/__\/__\/__\/__\/__\/__\...
 /\ /\ /\ /\ /\ /\ /\ <-- Third Strp
 /__\/__\/__\/__\/__\/__\/__\... |
| --- |

```

| __________________________ Fixed Vertex Attributes ___________________________ |
| --- |

The Fixed Attribute registers can be used in two ways:

```

| 1) Setting attribute(s) to fixed settings (similar to FLOATUNIFORM
 registers). For example, one could re-use a shader that was
 programmed to support variable colors with fixed colors.
 Unknown if fixed settings are also working for partial attributes
 (eg. 2D graphics with X,Y coordinates and fixed Z,W values).
 2) Using [GPUREG_FIXEDATTRIB_INDEX]=0Fh does directly send vertex
 attribute(s) to the shader unit (this could be "easier" than setting
 up ATTR_BUF arrays in memory, however, in practice it is extremely
 uncomfortable (due to the weird data format with 4x24bit float values
 sent in reversed 32bit word order). |
| --- |

**104018C8h - PICA(0232h) - GPUREG_FIXEDATTRIB_INDEX (W)**

```

| 0-3 Fixed attribute index (00h-0Bh=Index, 0Fh=immediate-mode submission) |
| --- |

Selects the index of the fixed attribute to be input with
GPUREG_FIXEDATTRIB_DATAi.

**104018CCh - PICA(0233h) - GPUREG_FIXEDATTRIB_DATA(0) (HIGH) (W)**

**104018D0h - PICA(0234h) - GPUREG_FIXEDATTRIB_DATA(1) (MID) (W)**

**104018D4h - PICA(0235h) - GPUREG_FIXEDATTRIB_DATA(2) (LOW) (W)**

```

| 0-23 float1.7.16, Vertex attribute X
 24-47 float1.7.16, Vertex attribute Y
 48-71 float1.7.16, Vertex attribute Z
 72-95 float1.7.16, Vertex attribute W |
| --- |

The above 96bit value is split into three 32bit words, and, weirdly,

```

| the word order for writing is HIGH,MID,LOW (aka big endian style)
 the byte order per 32bit word is little endian |
| --- |

More uncomfortably, fixed attributes must be in 4x24bit float format (with
X,Y,Z,W, even when needing only X,Y, and with 24bit float, instead of more
comfortable formats like 8bit/16bit integer or 32bit float)

Note: The three DATA registers are mirrors of each other, one could write the
HIGH,MID,LOW words all to DATA0, or to DATA0..2.

```

| ________________________ Shader/Attribute/Mode Config ________________________ |
| --- |

Below is still rather nebulous, some registers are required to be initialized
as described... and some seem to be bogus (somehow having wrong descriptions
and/or no function at all).

**10401908h - PICA(0242h) - GPUREG_VSH_NUM_ATTR (required) (R/W)**

```

| 0-3 Number of vertex shader input attributes - 1 |
| --- |

**10401928h - PICA(024Ah) - GPUREG_VSH_OUTMAP_TOTAL1 (required) (R/W)**

**10401944h - PICA(0251h) - GPUREG_VSH_OUTMAP_TOTAL2 (no function?) (R/W)**

```

| 0-3 Number of vertex shader output map registers - 1 |
| --- |

Uh, why are there TWO such registers? Is that for each command buffer? Or for
GSH and VSH? Or are that something like total+remaining counts?

**104018A4h - PICA(0229h) - GPUREG_GEOSTAGE_CONFIG (unknown purpose) (R/W)**

```

| 0-1 Geometry shader in use (0=Not in use, 2=In use)
 8 Drawing triangle elements (0=Not, 1=Drawing triangle elements)
 9 UNKNOWN "0x0" (R/W)
 16-19 UNKNOWN (R/W)
 31 Use reserved geometry shader subdivision (0=Don't use, 1=Use) |
| --- |

"When using vertex buffers and drawing elements in triangles mode, bit8 is set
to 1, else it is set to 0."

Uh, what means drawing triangle elements... does the gpu even support drawing
anything else than triangles?

**1040194Ch - PICA(0253h) - GPUREG_GEOSTAGE_CONFIG2 (parts required) (R/W)**

```

| 0 Function indicator
 with vertex buffers: 0=Draw elements, 1=Draw arrays <-- blah?
 without: 0=Not inputting, 1=Inputting vertex attribute data <--?
 8 Drawing triangle elements (0=Not, 1=Drawing triangle elements) |
| --- |

"When using vertex buffers, bit0 is set to 1 before drawing arrays, and cleared
to 0 immediately after.

When using immediate mode to directly input vertex attribute data, bit0 is set
to 1 before inputting vertex attribute data, and cleared to 0 immediately
after. While bit0 is set to 1, some register writes outside of the 0x200-0x254
and 0x280-0x2DF ranges may be processed incorrectly.

When using vertex buffers and drawing elements in triangles mode, bit8 is set
to 1, else it is set to 0."

**10401948h - PICA(0252h) - GPUREG_GSH_MISC0 (R/W)**

```

| 0-1 Mode? (0=Normal, 1=WhateverSubdivision?, 2=WhateverParticle?)
 8-22 Something? (0..7FFFh, for the Particle stuff?)
 24 Flag? (0..1, for the Particle stuff?) |
| --- |

Reportedly 00000000h=Normal, 00000001h=Reserved geometry shader subdivision in
use, 01004302h=Particle system in use.

**10401950h - PICA(0254h) - GPUREG_GSH_MISC1 (R/W)**

```

| 0-4 Reserved geometry shader subdivision type (2=Loop, 3=Catmull-Clark) |
| --- |

"Configures the type of reserved geometry shader subdivision in use. The value
is ignored when a subdivision is not in use."

**104018B4h - PICA(022Dh) - GPUREG_POST_VERTEX_CACHE_NUM (R/W)**

```

| 0-7 Number of entries in post-vertex cache (unsigned, usually 04h or 84h)
 8-15 UNKNOWN (R/W)
 16-23 UNKNOWN (R/W) ;\these two bytes are swapped when reading
 24-31 UNKNOWN (R/W) ;/(reading mirrors also returns swapped bytes) |
| --- |

"Configures the post-vertex cache." Uh, what is that?

**104018C4h - PICA(0231h) - GPUREG_VTX_FUNC (W)**

```

| 0-31 Trigger (0=Idle???, Non-zero=Clear post-vertex cache) |
| --- |

"Triggers clearing the post-vertex cache."

Uh, then, maybe should be called "clear cache" instead "vtx func". And what is
a "post-vertex"? And is it really clearing anything, drawing seems to work
without doing so.

**10401910h - PICA(0244h) - GPUREG_VSH_COM_MODE (R/W)**

```

| 0 Geometry shader configuration enable (0=Disable, 1=Enable) |
| --- |

"Sets whether to use the geometry shader configuration or reuse the vertex
shader configuration for the geometry shader shading unit."

**10401914h - PICA(0245h) - GPUREG_START_DRAW_FUNC0 (R/W)**

```

| 0 Mode (0=Drawing, 1=Configuration)
 1-7 UNKNOWN "0x0" (not R/W) |
| --- |

"Related to drawing. When the mode value is set to 1, rendering is not
performed properly. When set to 0, changes to the vertex shader configuration
registers are not applied correctly. Because of this, it is usually initialized
to 1, set to 0 immediately before triggering a draw, and set back to 1
immediately after triggering a draw."

**1040190Ch - PICA(0243h) - GPUREG_undoc_1040190Ch (R/W=00000037h)**

```

| 0-2 UNKNOWN (0..7) (R/W)
 4-6 UNKNOWN (0..3) (R/W) |
| --- |

Power-up default is 37h. GPU hangs on several other settings, in some cases
also depending on other registers (for example, 00h hangs only when also
clearing other undocumented registers).

**10401954h - PICA(0255h) - GPUREG_undoc_10401954h (R/W=00000001h)**

```

| 0 UNKNOWN (0..1) (R/W) |
| --- |