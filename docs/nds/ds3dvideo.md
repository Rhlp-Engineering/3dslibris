# Ds3Dvideo

> Source: https://problemkaputt.de/gbatek.htm
> Section: Ds3Dvideo

DS 3D Video 
| |
| --- |

 DS 3D Overview
 DS 3D I/O Map
 DS 3D Display Control
 DS 3D Geometry Commands
 DS 3D Matrix Load/Multiply
 DS 3D Matrix Types
 DS 3D Matrix Stack
 DS 3D Matrix Examples (Projection)
 DS 3D Matrix Examples (Rotate/Scale/Translate)
 DS 3D Matrix Examples (Maths Basics)
 DS 3D Polygon Attributes
 DS 3D Polygon Definitions by Vertices
 DS 3D Polygon Light Parameters
 DS 3D Shadow Polygons
 DS 3D Texture Attributes
 DS 3D Texture Formats
 DS 3D Texture Coordinates
 DS 3D Texture Blending
 DS 3D Toon, Edge, Fog, Alpha-Blending, Anti-Aliasing
 DS 3D Status
 DS 3D Tests
 DS 3D Rear-Plane
 DS 3D Final 2D Output
 
 3D is more or less (about 92%) understood and described.
 
 
 
| DS 3D Overview |
| --- |

 The NDS 3D hardware consists of a Geometry Engine, and a Rendering Engine.
 
 **Geometry Engine (Precalculate coordinates & assign polygon attributes)**
 Geometry commands can be sent via Ports 4000440h and up (or alternately, written directly to Port 4000400h).
 The commands include matrix and vector multiplications, the purpose is to rotate/scale/translate coordinates (vertices), the resulting coordinates are stored in Vertex RAM.
 Moreover, it allows to assign attributes to the polygons and vertices, that includes vertex colors (or automatically calculated light colors), texture attributes, number of vertices per polygon (three or four), and a number of flags, these attributes are stored in Polygon RAM. Polygon RAM also contains pointers to the corresponding vertices in Vertex RAM.
 
 **Swap Buffers (Pass data from the Geometry Engine to the Rendering Engine)**
 The hardware includes two sets of Vertex/Polygon RAM, one used by the Geometry Engine, one by the Rendering Engine. The SwapBuffers command simply exchanges these buffers (so the new Geometry Data is passed to the Rendering Engine) (and the old buffer is emptied, so the Geometry engine can write new data to it). Additionally, the two parameter bits from the <previous> SwapBuffers command are copied to the Geometry Engine.
 Data that is NOT swapped: SwapBuffers obviously can't swap Texture memory (so software must take care that Texture memory is kept mapped throughout rendering). Moreover, the rendering control registers (ports 4000060h, and 4000330h..40003BFh) are not swapped (so that values must be kept intact during rendering, too).
 
 **Rendering Engine (Display Output)**
 The Rendering Engine draws the various Polygons, and outputs them as BG0 layer to the 2D Video controller (which may then output them to the screen, or to the video capture unit). The Rendering part is done automatically by hardware, so the software has little influence on it.
 Rendering is done scanline-by-scanline, so there's only a limited number of clock cycles per scanline, which is limiting the maximum number of polygons per scanline. However, due to the 48-line cache (see below), some scanlines are allowed to exceed that maximum.
 Rendering starts 48 lines in advance (while still in the Vblank period) (and does then continue throughout the whole display period), the rendered data is written to a small cache that can hold up to 48 scanlines.
 
 **Scanline Cache vs Framebuffer**
 Note: There's only the 48-line cache (not a full 192-line framebuffer to store the whole rendered image). That is perfectly reasonable since animated data is normally drawn only once (so there would be no need to store it). That, assuming that the Geometry Engine presents new data every frame (otherwise, if the Geometry software is too slow, or if the image isn't animated, then the hardware is automatically rendering the same image again, and again).
 
 
 
| DS 3D I/O Map |
| --- |

 **3D I/O Map**
 
```
********
| Address Siz Name Expl.
 Rendering Engine (per Frame settings)
 4000060h 2 DISP3DCNT 3D Display Control Register (R/W)
 4000320h 1 RDLINES_COUNT Rendered Line Count Register (R)
 4000330h 10h EDGE_COLOR Edge Colors 0..7 (W)
 4000340h 1 ALPHA_TEST_REF Alpha-Test Comparision Value (W)
 4000350h 4 CLEAR_COLOR Clear Color Attribute Register (W)
 4000354h 2 CLEAR_DEPTH Clear Depth Register (W)
 4000356h 2 CLRIMAGE_OFFSET Rear-plane Bitmap Scroll Offsets (W)
 4000358h 4 FOG_COLOR Fog Color (W)
 400035Ch 2 FOG_OFFSET Fog Depth Offset (W)
 4000360h 20h FOG_TABLE Fog Density Table, 32 entries (W)
 4000380h 40h TOON_TABLE Toon Table, 32 colors (W)
 Geometry Engine (per Polygon/Vertex settings)
 4000400h 40h GXFIFO Geometry Command FIFO (W)
 4000440h ... ... Geometry Command Ports (see below)
 4000600h 4 GXSTAT Geometry Engine Status Register (R and R/W)
 4000604h 4 RAM_COUNT Polygon List & Vertex RAM Count Register (R)
 4000610h 2 DISP_1DOT_DEPTH 1-Dot Polygon Display Boundary Depth (W)
 4000620h 10h POS_RESULT Position Test Results (R)
 4000630h 6 VEC_RESULT Vector Test Results (R)
 4000640h 40h CLIPMTX_RESULT Read Current Clip Coordinates Matrix (R)
 4000680h 24h VECMTX_RESULT Read Current Directional Vector Matrix (R) |
| --- |

**Geometry Commands (can be invoked by Port Address, or by Command ID)**

Table shows Port Address, Command ID, Number of Parameters, and Clock Cycles.

```

| Address Cmd Pa.Cy.
 N/A 00h - - NOP - No Operation (for padding packed GXFIFO commands)
 4000440h 10h 1 1 MTX_MODE - Set Matrix Mode (W)
 4000444h 11h - 17 MTX_PUSH - Push Current Matrix on Stack (W)
 4000448h 12h 1 36 MTX_POP - Pop Current Matrix from Stack (W)
 400044Ch 13h 1 17 MTX_STORE - Store Current Matrix on Stack (W)
 4000450h 14h 1 36 MTX_RESTORE - Restore Current Matrix from Stack (W)
 4000454h 15h - 19 MTX_IDENTITY - Load Unit Matrix to Current Matrix (W)
 4000458h 16h 16 34 MTX_LOAD_4x4 - Load 4x4 Matrix to Current Matrix (W)
 400045Ch 17h 12 30 MTX_LOAD_4x3 - Load 4x3 Matrix to Current Matrix (W)
 4000460h 18h 16 35* MTX_MULT_4x4 - Multiply Current Matrix by 4x4 Matrix (W)
 4000464h 19h 12 31* MTX_MULT_4x3 - Multiply Current Matrix by 4x3 Matrix (W)
 4000468h 1Ah 9 28* MTX_MULT_3x3 - Multiply Current Matrix by 3x3 Matrix (W)
 400046Ch 1Bh 3 22 MTX_SCALE - Multiply Current Matrix by Scale Matrix (W)
 4000470h 1Ch 3 22* MTX_TRANS - Mult. Curr. Matrix by Translation Matrix (W)
 4000480h 20h 1 1 COLOR - Directly Set Vertex Color (W)
 4000484h 21h 1 9* NORMAL - Set Normal Vector (W)
 4000488h 22h 1 1 TEXCOORD - Set Texture Coordinates (W)
 400048Ch 23h 2 9 VTX_16 - Set Vertex XYZ Coordinates (W)
 4000490h 24h 1 8 VTX_10 - Set Vertex XYZ Coordinates (W)
 4000494h 25h 1 8 VTX_XY - Set Vertex XY Coordinates (W)
 4000498h 26h 1 8 VTX_XZ - Set Vertex XZ Coordinates (W)
 400049Ch 27h 1 8 VTX_YZ - Set Vertex YZ Coordinates (W)
 40004A0h 28h 1 8 VTX_DIFF - Set Relative Vertex Coordinates (W)
 40004A4h 29h 1 1 POLYGON_ATTR - Set Polygon Attributes (W)
 40004A8h 2Ah 1 1 TEXIMAGE_PARAM - Set Texture Parameters (W)
 40004ACh 2Bh 1 1 PLTT_BASE - Set Texture Palette Base Address (W)
 40004C0h 30h 1 4 DIF_AMB - MaterialColor0 - Diffuse/Ambient Reflect. (W)
 40004C4h 31h 1 4 SPE_EMI - MaterialColor1 - Specular Ref. & Emission (W)
 40004C8h 32h 1 6 LIGHT_VECTOR - Set Light's Directional Vector (W)
 40004CCh 33h 1 1 LIGHT_COLOR - Set Light Color (W)
 40004D0h 34h 32 32 SHININESS - Specular Reflection Shininess Table (W)
 4000500h 40h 1 1 BEGIN_VTXS - Start of Vertex List (W)
 4000504h 41h - 1 END_VTXS - End of Vertex List (W)
 4000540h 50h 1 392 SWAP_BUFFERS - Swap Rendering Engine Buffer (W)
 4000580h 60h 1 1 VIEWPORT - Set Viewport (W)
 40005C0h 70h 3 103 BOX_TEST - Test if Cuboid Sits inside View Volume (W)
 40005C4h 71h 2 9 POS_TEST - Set Position Coordinates for Test (W)
 40005C8h 72h 1 5 VEC_TEST - Set Directional Vector for Test (W) |
| --- |

All cycle timings are counted in 33.51MHz units. NORMAL commands takes 9..12
cycles, depending on the number of enabled lights in PolyAttr (Huh, 9..12 (four
timings) cycles for 0..4 (five settings) lights?) Total execution time of
SwapBuffers is Duration until VBlank, plus 392 cycles.

In MTX_MODE=2 (Simultanous Set), MTX_MULT/TRANS take additional 30 cycles.

| DS 3D Display Control |
| --- |

**4000060h - DISP3DCNT - 3D Display Control Register (R/W)**

```

| 0 Texture Mapping (0=Disable, 1=Enable)
 1 PolygonAttr Shading (0=Toon Shading, 1=Highlight Shading)
 2 Alpha-Test (0=Disable, 1=Enable) (see ALPHA_TEST_REF)
 3 Alpha-Blending (0=Disable, 1=Enable) (see various Alpha values)
 4 Anti-Aliasing (0=Disable, 1=Enable)
 5 Edge-Marking (0=Disable, 1=Enable) (see EDGE_COLOR)
 6 Fog Color/Alpha Mode (0=Alpha and Color, 1=Only Alpha) (see FOG_COLOR)
 7 Fog Master Enable (0=Disable, 1=Enable)
 8-11 Fog Depth Shift (FOG_STEP=400h shr FOG_SHIFT) (see FOG_OFFSET)
 12 Color Buffer RDLINES Underflow (0=None, 1=Underflow/Acknowledge)
 13 Polygon/Vertex RAM Overflow (0=None, 1=Overflow/Acknowledge)
 14 Rear-Plane Mode (0=Blank, 1=Bitmap)
 15-31 Not used |
| --- |

**4000540h - Cmd 50h - SWAP_BUFFERS - Swap Rendering Engine Buffer (W)**

SwapBuffers exchanges the two sets of Polygon/Vertex RAM buffers, that is, the
newly defined polygons/vertices are passed to the rendering engine (and will be
displayed in following frame(s)). The other buffer is emptied, and passed to
the Geometry Engine (to be filled with new polygons/vertices by Geometry
Commands).

```

| 0 Translucent polygon Y-sorting (0=Auto-sort, 1=Manual-sort)
 1 Depth Buffering (0=With Z-value, 1=With W-value)
 (mode 1 does not function properly with orthogonal projections)
 2-31 Not used |
| --- |

SwapBuffers isn't executed until next VBlank (Scanline 192) (the Geometry
Engine is halted for that duration). SwapBuffers should not be issued within
Begin/End. The two parameter bits of the SwapBuffers command are used for the
following gxcommands (ie. not for the old gxcommands prior to SwapBuffers).

SwapBuffers does lock-up the 3D hardware if an incomplete polygon list has been
defined (eg. a triangle with only 2 vertices). On lock-up, only 2D video is
kept working, any wait-loops for GXSTAT.27 will hang the program. Once lock-up
has occured, there seems to be no way to recover by software, not by sending
the missing veric(es), and not even by pulsing POWCNT1.Bit2-3.

**4000580h - Cmd 60h - VIEWPORT - Set Viewport (W)**

```

| 0-7 Screen/BG0 Coordinate X1 (0..255) (For Fullscreen: 0=Left-most)
 8-15 Screen/BG0 Coordinate Y1 (0..191) (For Fullscreen: 0=Bottom-most)
 16-23 Screen/BG0 Coordinate X2 (0..255) (For Fullscreen: 255=Right-most)
 24-31 Screen/BG0 Coordinate Y2 (0..191) (For Fullscreen: 191=Top-most) |
| --- |

Coordinate 0,0 is the lower-left (unlike for 2D where it'd be upper-left).

The 3D view-volume (size as defined by the Projection Matrix) is automatically
scaled to match into the Viewport area. Although polygon vertices are clipped
to the view-volume, some vertices may still exceed to X2,Y1 (lower-right)
boundary by one pixel, due to some sort of rounding errors. The Viewport
settings don't affect the size or position of the 3D Rear-Plane. Viewport
should not be issued within Begin/End.

**4000610h - DISP_1DOT_DEPTH - 1-Dot Polygon Display Boundary Depth (W)**

1-Dot Polygons are very small, or very distant polygons, which would be
rendered as a single pixel on screen. Polygons with a depth value greater (more
distant) than DISP_1DOT_DEPTH can be automatically hidden; in order to reduce
memory consumption, or to reduce dirt on the screen.

```

| 0-14 W-Coordinate (Unsigned, 12bit integer, 3bit fractional part)
 15-31 Not used (0000h=Closest, 7FFFh=Most Distant) |
| --- |

The DISP_1DOT_DEPTH comparision can be enabled/disabled per polygon (via
POLYGON_ATTR.Bit13), so "important" polygons can be displayed regardless of
their size and distance.

Note: The comparision is always using the W-coordinate of the vertex (not the
Z-coordinate) (ie. no matter if using Z-buffering, or W-buffering). The polygon
is rendered if at least one of its vertices is having a w-coordinate less or
equal than DISP_1DOT_DEPTH. NB. despite of checking the w-coords of ALL
vertices, the polygon is rendered using the color/depth/texture of its FIRST
vertex.

Note: The hardware does round-up the width and height of all polygons to at
least 1, so polygons of 0x0, 1x0, 0x1, and 1x1 dots will be all rounded-up to a
size of 1x1. Of which, the so-called "1dot" depth check is applied only to the
0x0 dot variant (so "0dot" depth check would be a better name for it).

Caution: Although DISP_1DOT_DEPTH is a Geometry Engine parameter, it is NOT
routed through GXFIFO, ie. changes will take place immediately, and will affect
all following polygons, including such that are still in GXFIFO. Workaround:
ensure that GXFIFO is empty before changing this parameter.

**4000340h - ALPHA_TEST_REF - Alpha-Test Comparision Value (W)**

Alpha Test can be enabled in DISP3DCNT.Bit2. When enabled, pixels are rendered
only if their Alpha value is GREATER than ALPHA_TEST_REF. Otherwise, when
disabled, pixels are rendered only if their Alpha value is GREATER than zero.
Alpha Test is performed on the final polygon pixels (ie. after texture
blending).

```

| 0-4 Alpha-Test Comparision Value (0..31) (Draw pixels if Alpha>AlphaRef)
 5-31 Not used |
| --- |

Value 00h is effectively the same as when Alpha Test is disabled. Value 1Fh
hides all polygons, including opaque ones.

| DS 3D Geometry Commands |
| --- |

**4000400h - GXFIFO - Geometry Command FIFO (W) (mirrored up to 400043Fh?)**

Used to send packed commands, unpacked commands,

```

| 0-7 First Packed Command (or Unpacked Command)
 8-15 Second Packed Command (or 00h=None)
 16-23 Third Packed Command (or 00h=None)
 24-31 Fourth Packed Command (or 00h=None) |
| --- |

and parameters,

```

| 0-31 Parameter data for the previously sent (packed) command(s) |
| --- |

to the Geometry engine.

**FIFO / PIPE Number of Entries**

The FIFO has 256 entries, additionally, there is a PIPE with four entries
(giving a total of 260 entries). If the FIFO is empty, and if the PIPE isn't
full, then data is moved directly into the PIPE, otherwise it is moved into the
FIFO. If the PIPE runs half empty (less than 3 entries) then 2 entries are
moved from the FIFO to the PIPE. The state of the FIFO can be obtained in
GXSTAT.Bit16-26, observe that there may be still data in the PIPE, even if the
FIFO is empty. Check the busy flag in GXSTAT.Bit27 to see if the PIPE or FIFO
contains data (or if a command is still executing).

Each PIPE/FIFO entry consists of 40bits of data (8bit command code, plus 32bit
parameter value). Commands without parameters occupy 1 entry, and Commands with
N parameters occupy N entries.

**Sending Commands by Ports 4000440h..40005FFh**

Geometry commands can be indirectly sent to the FIFO via ports 4000440h and up.

For a command with N parameters: issue N writes to the port.

For a command without parameters: issue one dummy-write to the port.

That mechanism puts the 8bit command + 32bit parameter into the FIFO/PIPE.

If the FIFO is full, then a wait is generated until data is removed from the
FIFO, ie. the STR opcode gets freezed, during the wait, the bus cannot be used
even by DMA, interrupts, or by the NDS7 CPU.

**GXFIFO Access via DMA**

Larger pre-calculated data blocks can be sent directly to the FIFO. This is
usually done via DMA (use DMA in Geometry Command Mode, 32bit units,
Dest=4000400h/fixed, Length=NumWords, Repeat=0). The timings are handled
automatically, ie. the system (should) doesn't freeze when the FIFO is full
(see below Overkill note though). DMA starts when the FIFO becomes less than
half full, the DMA does then write 112 words to the GXFIFO register (or less,
if the remaining DMA transfer length gets zero).

**GXFIFO Access via STR,STRD,STM**

If desired, STR,STRD,STM opcodes can be used to write to the FIFO.

Opcodes that write more than one 32bit value (ie. STRD and STM) can be used to
send ONE UNPACKED command, plus any parameters which belong to that command.
After that, there must be a 1 cycle delay before sending the next command (ie.
one cannot sent more than one command at once with a single opcode, each
command must be invoked by a new opcode). STRD and STM can be used because the
GXFIFO register is mirrored to 4000400h..43Fh (16 words).

As with Ports 4000440h and up, the CPU gets stopped if (and as long as) the
FIFO is full.

**GXFIFO / Unpacked Commands**

```

| - command1 (upper 24bit zero)
 - parameter(s) for command1 (if any)
 - command2 (upper 24bit zero)
 - parameter(s) for command2 (if any)
 - command3 (upper 24bit zero)
 - parameter(s) for command3 (if any) |
| --- |

**GXFIFO / Packed Commands**

```

| - command1,2,3,4 packed into one 32bit value (all bits used)
 - parameter(s) for command1 (if any)
 - parameter(s) for command2 (if any)
 - parameter(s) for command3 (if any)
 - parameter(s) for command4 (top-most packed command MUST have parameters)
 - command5,6 packed into one 32bit value (upper 16bit zero)
 - parameter(s) for command5 (if any)
 - parameter(s) for command6 (top-most packed command MUST have parameters)
 - command7,8,9 packed into one 32bit value (upper 8bit zero)
 - parameter(s) for command7 (if any)
 - parameter(s) for command8 (if any)
 - parameter(s) for command9 (top-most packed command MUST have parameters) |
| --- |

Packed commands are first decompressed and then stored in command the FIFO.

**GXFIFO DMA Overkill on Packed Commands Without Parameters**

Normally, the 112 word limit ensures that the FIFO (256 entries) doesn't get
full, however, this limit is much too high for sending a lot of "Packed
Commands Without Parameters" (ie. PUSH, IDENTITY, or END) - eg. sending 112 x
Packed(00151515h) to GXFIFO would write 336 x Cmd(15h) to the FIFO, which is
causing the FIFO to get full, and which is causing the DMA (and CPU) to be
paused (for several seconds, in WORST case) until enough FIFO commands have
been processed to allow the DMA to finish the 112 word transfer.

Not sure if there's much chance to get Overkills in practice. Normally most
commands DO have parameters, and so, usually even LESS than 112 FIFO entries
are occupied (since 8bit commands with 32bit parameters are merged into single
40bit FIFO entries).

**Invalid GX commands**

Invalid commands (anything else than 10h..1Ch, 20h..2Bh, 30h..33h, 40h..41h,
50h, 60h, or 70h..72h) seem to be simply ignored by the hardware (at least,
testing has confirmed that they do not fetch any parameters from the gxfifo).

| DS 3D Matrix Load/Multiply |
| --- |

**4000440h - Cmd 10h - MTX_MODE - Set Matrix Mode (W)**

```

| 0-1 Matrix Mode (0..3)
 0 Projection Matrix
 1 Position Matrix (aka Modelview Matrix)
 2 Position & Vector Simultaneous Set mode (used for Light+VEC_TEST)
 3 Texture Matrix (see DS 3D Texture Coordinates chapter)
 2-31 Not used |
| --- |

Selects the current Matrix, all following MTX commands (load, multiply, push,
pop, etc.) are applied to that matrix. In Mode 2, all MTX commands are applied
to both the Position and Vector matrices. There are two special cases:

```

| MTX_SCALE in Mode 2: uses ONLY Position Matrix
 MTX_PUSH/POP/STORE/RESTORE in Mode 1: uses BOTH Position AND Vector Matrices |
| --- |

Ie. the four stack commands act like mode 2 (even when in mode 1; keeping the
two stacks somewhat in sync), and scale acts like mode 1 (even when in mode 2;
keeping the light vector length's intact).

**4000454h - Cmd 15h - MTX_IDENTITY - Load Unit Matrix to Current Matrix (W)**

Sets C=I. Parameters: None

The Identity Matrix (I), aka Unit Matrix, consists of all zeroes, with a
diagonal row of ones. A matrix multiplied by the Unit Matrix is left unchanged.

**4000458h - Cmd 16h - MTX_LOAD_4x4 - Load 4x4 Matrix to Current Matrix (W)**

Sets C=M. Parameters: 16, m[0..15]

**400045Ch - Cmd 17h - MTX_LOAD_4x3 - Load 4x3 Matrix to Current Matrix (W)**

Sets C=M. Parameters: 12, m[0..11]

**4000460h - Cmd 18h - MTX_MULT_4x4 - Multiply Current Matrix by 4x4 Matrix (W)**

Sets C=M*C. Parameters: 16, m[0..15]

**4000464h - Cmd 19h - MTX_MULT_4x3 - Multiply Current Matrix by 4x3 Matrix (W)**

Sets C=M*C. Parameters: 12, m[0..11]

**4000468h - Cmd 1Ah - MTX_MULT_3x3 - Multiply Current Matrix by 3x3 Matrix (W)**

Sets C=M*C. Parameters: 9, m[0..8]

**400046Ch - Cmd 1Bh - MTX_SCALE - Multiply Current Matrix by Scale Matrix (W)**

Sets C=M*C. Parameters: 3, m[0..2]

Note: MTX_SCALE doesn't change Vector Matrix (even when in MTX_MODE=2) (that's
done so for keeping the length of the light vector's intact).

**4000470h - Cmd 1Ch - MTX_TRANS - Mult. Curr. Matrix by Translation Matrix (W)**

Sets C=M*C. Parameters: 3, m[0..2] (x,y,z position)

**4000640h..67Fh - CLIPMTX_RESULT - Read Current Clip Coordinates Matrix (R)**

This 64-byte region (16 words) contains the m[0..15] values of the Current Clip
Coordinates Matrix, arranged in 4x4 Matrix format. Make sure that the Geometry
Engine is stopped (GXSTAT.27) before reading from these registers.

The Clip Matrix is internally used to convert vertices to screen coordinates,
and is internally re-calculated anytime when changing the Position or
Projection matrices:

```

| ClipMatrix = PositionMatrix * ProjectionMatrix |
| --- |

To read only the Position Matrix, or only the Projection Matrix: Use Load
Identity on the OTHER matrix, so the ClipMatrix becomes equal to the DESIRED
matrix (multiplied by the Identity Matrix, which has no effect on the result).

**4000680h..6A3h - VECMTX_RESULT - Read Current Directional Vector Matrix (R)**

This 36-byte region (9 words) contains the m[0..8] values of the Current
Directional Vector Matrix, arranged in 3x3 Matrix format (the fourth row/column
may contain any values).

Make sure that the Geometry Engine is stopped (GXSTAT.27) before reading from
these registers.

| DS 3D Matrix Types |
| --- |

Essentially, all matrices in the NDS are 4x4 Matrices, consisting of 16 values,
m[0..15]. Each element is a signed fixed-point 32bit number, with a fractional
part in the lower 12bits.

The other Matrix Types are used to reduce the number of parameters being
transferred, for example, 3x3 Matrix requires only nine parameters, the other
seven elements are automatically set to 0 or 1.0 (whereas "1.0" means "1 SHL
12" in 12bit fixed-point notation).

```
****
| _ 4x4 Matrix _ _ Identity Matrix _
 | m[0] m[1] m[2] m[3] | | 1.0 0 0 0 |
 | m[4] m[5] m[6] m[7] | | 0 1.0 0 0 |
 | m[8] m[9] m[10] m[11] | | 0 0 1.0 0 |
 |_m[12] m[13] m[14] m[15]_| |_ 0 0 0 1.0 _| |
| --- |

```
****
| _ 4x3 Matrix _ _ Translation Matrix _
 | m[0] m[1] m[2] 0 | | 1.0 0 0 0 |
 | m[3] m[4] m[5] 0 | | 0 1.0 0 0 |
 | m[6] m[7] m[8] 0 | | 0 0 1.0 0 |
 |_m[9] m[10] m[11] 1.0 _| |_m[0] m[1] m[2] 1.0 _| |
| --- |

```
****
| _ 3x3 Matrix _ _ Scale Matrix _
 | m[0] m[1] m[2] 0 | | m[0] 0 0 0 |
 | m[3] m[4] m[5] 0 | | 0 m[1] 0 0 |
 | m[6] m[7] m[8] 0 | | 0 0 m[2] 0 |
 |_ 0 0 0 1.0 _| |_ 0 0 0 1.0 _| |
| --- |

| DS 3D Matrix Stack |
| --- |

**Matrix Stack**

The NDS has three Matrix Stacks, and two Matrix Stack Pointers (the Coordinate
Matrix stack pointer is also shared for Directional Matrix Stack).

```

| Matrix Stack________Valid Stack Area____Stack Pointer___________________
 Projection Stack 0..0 (1 entry) 0..1 (1bit) (GXSTAT: 1bit)
 Coordinate Stack 0..30 (31 entries) 0..63 (6bit) (GXSTAT: 5bit only)
 Directional Stack 0..30 (31 entries) (uses Coordinate Stack Pointer)
 Texture Stack One..None? 0..1 (1bit) (GXSTAT: N/A) |
| --- |

Which of the stacks/matrices depends on the current Matrix Mode (as usually,

but with one exception; stack operations MTX_PUSH/POP/STORE/RESTORE in Mode 1
are acting same as in Mode 2):

```

| MTX_MODE = 0 --> Projection Stack
 MTX_MODE = 1 or 2 --> BOTH Coordinate AND Directional Stack
 MTX_MODE = 3 --> Texture Stack |
| --- |

The initial value of the Stack Pointers is zero, the current value of the
pointers can be read from GXSTAT (read-only), that register does also indicate
stack overflows (errors flag gets set on read/write to invalid entries, ie.
entries 1 or 1Fh..3Fh). For all stacks, the upper half (ie. 1 or 20h..3Fh) are
mirrors of the lower half (ie. 0 or 0..1Fh).

**4000444h - Cmd 11h - MTX_PUSH - Push Current Matrix on Stack (W)**

Parameters: None. Sets [S]=C, and then S=S+1.

**4000448h - Cmd 12h - MTX_POP - Pop Current Matrix from Stack (W)**

Sets S=S-N, and then C=[S].

```

| Parameter Bit0-5: Stack Offset (signed value, -30..+31) (usually +1)
 Parameter Bit6-31: Not used |
| --- |

Offset N=(+1) pops the most recently pushed value, larger offsets of N>1
will "deallocate" N values (and load the Nth value into C). Zero or negative
values can be used to pop previously "deallocated" values.

The stack has only one level (at address 0) in projection mode, in that mode,
the parameter value is ignored, the offset is always +1 in that mode.

**400044Ch - Cmd 13h - MTX_STORE - Store Current Matrix on Stack (W)**

Sets [N]=C. The stack pointer S is not used, and is left unchanged.

```

| Parameter Bit0-4: Stack Address (0..30) (31 causes overflow in GXSTAT.15)
 Parameter Bit5-31: Not used |
| --- |

The stack has only one level (at address 0) in projection mode, in that mode,
the parameter value is ignored.

**4000450h - Cmd 14h - MTX_RESTORE - Restore Current Matrix from Stack (W)**

Sets C=[N]. The stack pointer S is not used, and is left unchanged.

```

| Parameter Bit0-4: Stack Address (0..30) (31 causes overflow in GXSTAT.15)
 Parameter Bit5-31: Not used |
| --- |

The stack has only one level (at address 0) in projection mode, in that mode,
the parameter value is ignored.

In Projection mode, the parameter for POP, STORE, and RESTORE is unused - not
sure if the parameter (ie. a dummy value) is - or is not - to be written to the
command FIFO?

There appear to be actually 32 entries in Coordinate & Directional Stacks,
entry 31 appears to exist, and appears to be read/write-able (although the
stack overflow flag gets set when accessing it).

| DS 3D Matrix Examples (Projection) |
| --- |

The most important matrix is the Projection Matrix (to be initialized with
MTX_MODE=0 via MTX_LOAD_4x4 command). It does specify the dimensions of the
view volume.

With Perspective Projections more distant objects will appear smaller, with
Orthogonal Projects the size of the objects is always same regardless of their
distance.

```
****
| Perspective Projection Orthogonal Projection
 __ __________
 top __..--'' | top | |
 | view | | view |
 Eye ----|--------->| Eye ----|--------->|
 |__volume | | volume |
 bottom ''--..__| bottom|__________|
 near far near far |
| --- |

Correctly initializing the projection matrix (as shown in the examples below)
can be quite difficult (mind that fixed point multiply/divide requires to
adjust the fixed-point width before/after calculation). For beginners, it may
be recommended to start with a simple Identity Matrix (MTX_IDENTITY command)
used as Projection Matrix (ie. Ortho with t,b,l,r set to +/-1).

**Orthogonal Projections (Ortho)**

```

| | (2.0)/(r-l) 0 0 0 |
 | 0 (2.0)/(t-b) 0 0 |
 | 0 0 (2.0)/(n-f) 0 |
 | (l+r)/(l-r) (b+t)/(b-t) (n+f)/(n-f) 1.0 | |
| --- |

n,f specify the distance from eye to near and far clip planes. t,b,l,r are the
coordinates of near clip plane (top,bottom,left,right). For a symmetrical view
(ie. the straight-ahead view line centered in the middle of viewport) t,b,l,r
should be usually t=+ysiz/2, b=-ysiz/2, r=+xsiz/2, l=-xsiz/2; the (xsiz/ysiz)
ratio should be usually equal to the viewport's (width/heigh) ratio. Examples
for a asymmetrical view would be b=0 (frog's view), or t=0 (bird's view).

**Left-Right Asymmetrical Perspective Projections (Frustum)**

```

| | (2*n)/(r-l) 0 0 0 |
 | 0 (2*n)/(t-b) 0 0 |
 | (r+l)/(r-l) (t+b)/(t-b) (n+f)/(n-f) -1.0 |
 | 0 0 (2*n*f)/(n-f) 0 | |
| --- |

n,f,t,b,l,r have same meanings as above (Ortho), the difference is that more
distant objects will appear smaller with Perspective Projection (unlike
Orthogonal Projection where the size isn't affected by the distance).

**Left-Right Symmetrical Perspective Projections (Perspective)**

```

| | cos/(asp*sin) 0 0 0 |
 | 0 cos/sin 0 0 |
 | 0 0 (n+f)/(n-f) -1.0 |
 | 0 0 (2*n*f)/(n-f) 0 | |
| --- |

Quite the same as above (Frustum), but with symmetrical t,b values (which are
in this case obtained from a vertical view range specified in degrees), and l,r
are matched to the aspect ratio of the viewport (asp=height/width).

**Moving the Camera**

After initializing the Projection Matrix, you may multiply it with Rotate
and/or Translation Matrices to change camera's position and view direction.

| DS 3D Matrix Examples (Rotate/Scale/Translate) |
| --- |

**Identity Matrix**

The MTX_IDENTITY command can be used to initialize the Position Matrix before
doing any Translation/Scaling/Rotation, for example:

```

| Load(Identity) ;no rotation/scaling used
 Load(Identity), Mul(Rotate), Mul(Scale) ;rotation/scaling (not so efficient)
 Load(Rotate), Mul(Scale) ;rotation/scaling (more efficient) |
| --- |

**Rotation Matrices**

Rotation can be performed with MTX_MULT_3x3 command, simple examples are:

```

| Around X-Axis Around Y-Axis Around Z-Axis
 | 1.0 0 0 | | cos 0 sin | | cos sin 0 |
 | 0 cos sin | | 0 1.0 0 | | -sin cos 0 |
 | 0 -sin cos | | -sin 0 cos | | 0 0 1.0 | |
| --- |

**Scale Matrix**

The MTX_SCALE command allows to adjust the size of the polygon. The x,y,z
parameters should be normally all having the same value, x=y=z (unless if you
want to change only the height of the object, for example). Identical results
can be obtained with MTX_MULT commands, however, when using lighting
(MTX_MODE=2), then scaling should be done ONLY with MTX_SCALE (which keeps the
length of the light's directional vector intact).

**Translation Matrix**

The MTX_TRANS command allows to move polygons to the desired position. The
polygon VTX commands are spanning only a small range of coordinates (near
zero-coordinate), so translation is required to move the polygons to other
locations in the world coordinates. Aside from that, translation is useful for
moved objects (at variable coordinates), and for re-using an object at various
locations (eg. you can create a forest by translating a tree to different
coordinates).

**Matrix Multiply Order**

The Matrix must be set up BEFORE sending the Vertices (which are then
automatically multiplied by the matrix). When using multiple matrices
multiplied with each other: Mind that, for matrix maths A*B is NOT the same as
B*A. For example, if you combine Rotate and Translate Matrices, the object will
be either rotated around it's own zero-coordinate, or around world-space
zero-coordinate, depending on the multiply order.

| DS 3D Matrix Examples (Maths Basics) |
| --- |

Below is a crash-course on matrix maths. Most of it is carried out
automatically by the hardware. So this chapter is relevant only if you are
interested in details about what happens inside of the 3D engine.

**Matrix-by-Matrix Multiplication**

Matrix multiplication, C = A * B, is possible only if the number of columns in
A is equal to the number of rows in B, so it works fine with the 4x4 matrices
which are used in the NDS. For the multiplication, assume matrix C to consist
of elements cyx, and respecitively, matrix A and B to consist of elements ayx
and byx. So that C = A * B looks like:

```

| | c11 c12 c13 c14 | | a11 a12 a13 a14 | | b11 b12 b13 b14 |
 | c21 c22 c23 c24 | = | a21 a22 a23 a24 | * | b21 b22 b23 b24 |
 | c31 c32 c33 c34 | | a31 a32 a33 a34 | | b31 b32 b33 b34 |
 | c41 c42 c43 c44 | | a41 a42 a43 a44 | | b41 b42 b43 b44 | |
| --- |

Each element in C is calculated by multiplying the elements from one row in A
by the elements from the corresponding column in B, and then taking the sum of
the products, ie.

```

| cyx = ay1*b1x + ay2*b2x + ay3*b3x + ay4*b4x |
| --- |

In total, that requires 64 multiplications (four multiplications for each of
the 16 cyx elements), and 48 additions (three per cyx element), the hardware
carries out that operation at a relative decent speed of 30..35 clock cycles,
possibly by performing several multiplications simultaneously with separate
multiply units.

Observe that for matrix multiplication, A*B is NOT the same as B*A.

**Matrix-by-Vector & Vector-by-Matrix Multiplication**

Vectors are Matrices with only one row, or only one column. Multiplication
works as for normal matrices; the number of rows/columns must match up,
repectively, row-vectors can be multiplied by matrices; and matrices can be
multiplied by column-vectors (but not vice-versa). Eg. C = A * B:

```

| | b11 b12 b13 b14 |
 | c11 c12 c13 c14 | = | a11 a12 a13 a14 | * | b21 b22 b23 b24 |
 | b31 b32 b33 b34 |
 | b41 b42 b43 b44 | |
| --- |

The formula for calculating the separate elements is same as above,

```

| cyx = ay1*b1x + ay2*b2x + ay3*b3x + ay4*b4x |
| --- |

Of which, C and A have only one y-index, so one may replace "cyx and ayx" by
"c1x and a1x", or completely leave out the y-index, ie. "cx and ax".

**Matrix-by-Number Multiplication**

Simply multiply all elements of the Matrix by the number, C = A * n:

```

| cyx = ayx*n |
| --- |

Of course, works also with vectors (matrices with only one row/column).

**Matrix-to-Matrix Addition/Subtraction**

Both matrices must have the same number of rows & columns, add/subtract all
elements with corresponding elements in other matrix, C = A +/- B:

```

| cyx = ayx +/- byx |
| --- |

Of course, works also with vectors (two matrices with only one row/column).

**Vectors**

A vector, for example (x,y,z), consists of offsets along x-,y-, and z-axis. The
line from origin to origin-plus-offset is having two characteristics: A
direction, and a length.

The length (aka magnitude) can be calculated as L=sqrt(x^2+y^2+z^2).

**Vector-by-Vector Multiplication**

This can be processed as LineVector*RowVector, so the result is a number (aka
scalar) (aka a matrix with only 1x1 elements). Multiplying two (normalized)
vectors results in: "cos(angle)=vec1*vec2", ie. the cosine of the angle between
the two vectors (eg. used for light vectors). Multiplying a vector with itself,
and taking the square root of the result obtains its length, ie.
"length=sqrt(vec^2)".

That stuff should be done with 3-dimensional vectors (not 4-dimensionals).

**Normalized Vectors**

Normalized Vectors (aka Unit Vectors) are vectors with length=1.0. To normalize
a vector, divide its coordinates by its length, ie. x=x/L, y=y/L, z=z/L, the
direction remains the same, but the length is now 1.0.

On the NDS, normalized vectors should have a length of something less than 1.0
(eg. something like 0.99) because several NDS registers are limited to 1bit
sign, 0bit integer, Nbit fractional part (so vectors that are parallel to the
x,y,z axes, or that become parallel to them after rotation, cannot have a
length of 1.0).

**Fixed-Point Numbers**

The NDS uses fixed-point numbers (rather than floating point numbers). Addition
and Subtraction works as with normal integers, provided that the fractional
part is the same for both numbers. If it is not the same: Shift-left the value
with the smaller fractional part.

For multiplication, the fractional part of result is the sum of the fractional
parts (eg. 12bit fraction * 12bit fraction = 24bit fraction; shift-right the
result by 12 to convert it 12bit fraction). The NDS matrix multiply unit is
maintaining the full 24bit fraction when processing the

```

| cyx = ay1*b1x + ay2*b2x + ay3*b3x + ay4*b4x |
| --- |

formula, ie. the three additions are using full 24bit fractions (with
carry-outs to upper bits), the final result of the additions is then
shifted-right by 12.

For division, it's vice versa, the fractions of the operands are substracted,
24bit fraction / 12bit fraction = 12bit fraction. When dividing two 12bit
numbers, shift-left the first number by 12 before division to get a result with
12bit fractional part.

**Four-Dimensional Matrices**

The NDS uses four-dimensional matrices and vectors, ie. matrices with 4x4
elements, and vectors with 4 elements. The first three elements are associated
with the X,Y,Z-axes of the three-dimensional space. The fourth element is
somewhat a "W-axis".

With 4-dimensional matrices, the Translate matrix can be used to move an object
to another position. Ie. once when you've setup a matrix (which may consists of
pre-multiplied scaling, rotation, translation matrices), then that matrix can
be used on vertices to perform the rotation, scaling, translation all-at-once;
by a single Vector*Matrix operation.

With 3-dimensional matrices, translation would require a separate addition,
additionally to the multiply operation.

| DS 3D Polygon Attributes |
| --- |

**40004A4h - Cmd 29h - POLYGON_ATTR - Set Polygon Attributes (W)**

```

| 0-3 Light 0..3 Enable Flags (each bit: 0=Disable, 1=Enable)
 4-5 Polygon Mode (0=Modulation,1=Decal,2=Toon/Highlight Shading,3=Shadow)
 6 Polygon Back Surface (0=Hide, 1=Render) ;Line-segments are always
 7 Polygon Front Surface (0=Hide, 1=Render) ;rendered (no front/back)
 8-10 Not used
 11 Depth-value for Translucent Pixels (0=Keep Old, 1=Set New Depth)
 12 Far-plane intersecting polygons (0=Hide, 1=Render/clipped)
 13 1-Dot polygons behind DISP_1DOT_DEPTH (0=Hide, 1=Render)
 14 Depth Test, Draw Pixels with Depth (0=Less, 1=Equal) (usually 0)
 15 Fog Enable (0=Disable, 1=Enable)
 16-20 Alpha (0=Wire-Frame, 1..30=Translucent, 31=Solid)
 21-23 Not used
 24-29 Polygon ID (00h..3Fh, used for translucent, shadow, and edge-marking)
 30-31 Not used |
| --- |

Writes to POLYGON_ATTR have no effect until next BEGIN_VTXS command.

Changes to the Light bits have no effect until lighting is re-calculated by
Normal command. The interior of Wire-frame polygons is transparent (Alpha=0),
and only the lines at the polygon edges are rendered, using a fixed Alpha value
of 31.

**4000480h - Cmd 20h - COLOR - Directly Set Vertex Color (W)**

```

| Parameter 1, Bit 0-4 Red
 Parameter 1, Bit 5-9 Green
 Parameter 1, Bit 10-14 Blue
 Parameter 1, Bit 15-31 Not used |
| --- |

The 5bit RGB values are internally expanded to 6bit RGB as follows:
X=X*2+(X+31)/32, ie. zero remains zero, all other values are X=X*2+1.

Aside from by using the Color command, the color can be also changed by
MaterialColor0 command (if MaterialColor0.Bit15 is set, it acts identical as
the Color Command), and by the Normal command (which calculates the color based
on light/material parameters).

**Depth Test**

The Depth Test compares the depth of the pixels of the polygon with the depth
of previously rendered polygons (or of the rear plane if there have been none
rendered yet). The new pixels are drawn if the new depth is Less (closer to the
camera), or if it is Equal, as selected by POLYGON_ATTR.Bit14.

Normally, Depth Equal would work only exact matches (ie. if the overlapping
polygons have exactly the same coordinates; and thus have the same rounding
errors), however, the NDS hardware is allowing "Equal" to have a tolerance of
+/-200h (within the 24bit depth range of 0..FFFFFFh), that may bypass rounding
errors, but it may also cause nearby polygons to be accidently treated to have
equal depth.

| DS 3D Polygon Definitions by Vertices |
| --- |

The DS supports polygons with 3 or 4 edges, triangles and quadliterals.

The position of the edges is defined by vertices, each consisting of (x,y,z)
values.

For Line Segments, use Triangles with twice the same vertex, Line Segments are
rendered always because they do not have any front and back sides.

The Prohibited Quad shapes may produce unintended results, namely, that are
Quads with crossed sides, and quads with angles greater than 180 degrees.

```

| Separate Tri. Triangle Strips Line Segment
 v0 v2___v4____v6
 |\ v3 /|\ |\ /\ v0 v1
 | \ /\ v0( | \ | \ / \ ------
 |__\ /__\ \|__\|__\/____\ v2
 v1 v2 v4 v5 v1 v3 v5 v7 |
| --- |

```

| Separate Quads Quadliteral Strips Prohibited Quads
 v0__v3 v0__v2____v4 v10__ v0__v3 v4
 / \ v4____v7 / \ |\ _____ / /v11 \/ |\
 / \ | \ / \ | |v6 v8| / /\ v5| \
 /______\ |_____\ /______\___|_|_____|/ /__\ /___\
 v1 v2 v5 v6 v1 v3 v5 v7 v9 v2 v1 v6 v7 |
| --- |

The vertices are normally arranged anti-clockwise, except that: in
triangle-strips each second polygon uses clockwise arranged vertices, and
quad-strips are sorts of "up-down" arranged (whereas "up" and "down" may be
anywhere due to rotation). Other arrangements may result in quads with crossed
lines, or may swap the front and back sides of the polygon (above examples are
showing the front sides).

**4000500h - Cmd 40h - BEGIN_VTXS - Start of Vertex List (W)**

```

| Parameter 1, Bit 0-1 Primitive Type (0..3, see below)
 Parameter 1, Bit 2-31 Not used |
| --- |

Indicates the Start of a Vertex List, and its Primitive Type:

```

| 0 Separate Triangle(s) ;3*N vertices per N triangles
 1 Separate Quadliteral(s) ;4*N vertices per N quads
 2 Triangle Strips ;3+(N-1) vertices per N triangles
 3 Quadliteral Strips ;4+(N-1)*2 vertices per N quads |
| --- |

The BEGIN_VTX command should be followed by VTX_-commands to define the
Vertices of the list, and should be then terminated by END_VTX command.

BEGIN_VTX additionally applies changes to POLYGON_ATTR.

**4000504h - Cmd 41h - END_VTXS - End of Vertex List (W)**

Parameters: None. This is a Dummy command for OpenGL compatibility. It should
be used to terminate a BEGIN_VTX, VTX_<values> sequence. END_VTXS is
possibly required for Nintendo's software emulator? On real NDS consoles (and
in no$gba) it does have no effect, it can be left out, or can be issued
multiple times inside of a vertex list, without disturbing the display.

**400048Ch - Cmd 23h - VTX_16 - Set Vertex XYZ Coordinates (W)**

```

| Parameter 1, Bit 0-15 X-Coordinate (signed, with 12bit fractional part)
 Parameter 1, Bit 16-31 Y-Coordinate (signed, with 12bit fractional part)
 Parameter 2, Bit 0-15 Z-Coordinate (signed, with 12bit fractional part)
 Parameter 2, Bit 16-31 Not used |
| --- |

**4000490h - Cmd 24h - VTX_10 - Set Vertex XYZ Coordinates (W)**

```

| Parameter 1, Bit 0-9 X-Coordinate (signed, with 6bit fractional part)
 Parameter 1, Bit 10-19 Y-Coordinate (signed, with 6bit fractional part)
 Parameter 1, Bit 20-29 Z-Coordinate (signed, with 6bit fractional part)
 Parameter 1, Bit 30-31 Not used |
| --- |

Same as VTX_16, with only one parameter, with smaller fractional part.

**4000494h - Cmd 25h - VTX_XY - Set Vertex XY Coordinates (W)**

```

| Parameter 1, Bit 0-15 X-Coordinate (signed, with 12bit fractional part)
 Parameter 1, Bit 16-31 Y-Coordinate (signed, with 12bit fractional part) |
| --- |

The Z-Coordinate is kept unchanged, and re-uses the value from previous VTX.

**4000498h - Cmd 26h - VTX_XZ - Set Vertex XZ Coordinates (W)**

```

| Parameter 1, Bit 0-15 X-Coordinate (signed, with 12bit fractional part)
 Parameter 1, Bit 16-31 Z-Coordinate (signed, with 12bit fractional part) |
| --- |

The Y-Coordinate is kept unchanged, and re-uses the value from previous VTX.

**400049Ch - Cmd 27h - VTX_YZ - Set Vertex YZ Coordinates (W)**

```

| Parameter 1, Bit 0-15 Y-Coordinate (signed, with 12bit fractional part)
 Parameter 1, Bit 16-31 Z-Coordinate (signed, with 12bit fractional part) |
| --- |

The X-Coordinate is kept unchanged, and re-uses the value from previous VTX.

**40004A0h - Cmd 28h - VTX_DIFF - Set Relative Vertex Coordinates (W)**

```

| Parameter 1, Bit 0-9 X-Difference (signed, with 9/12bit fractional part)
 Parameter 1, Bit 10-19 Y-Difference (signed, with 9/12bit fractional part)
 Parameter 1, Bit 20-29 Z-Difference (signed, with 9/12bit fractional part)
 Parameter 1, Bit 30-31 Not used |
| --- |

Sets XYZ-Coordinate relative to the XYZ-Coordinates from previous VTX. In
detail: The 9bit fractional values are divided by 8 (sign expanded to 12bit
fractions, in range +/-0.125), and that 12bit fraction is then added to the old
vtx coordinates. The result of the addition should not overflow 16bit vertex
coordinate range (1bit sign, 3bit integer, 12bit fraction).

**Notes on VTX commands**

On each VTX command, the viewport coordinates of the vertex are calculated and
stored in Vertex RAM,

```

| ( xx, yy, zz, ww ) = ( x, y, z, 1.0 ) * ClipMatrix |
| --- |

The actual screen position (in pixels) is then,

```

| screen_x = (xx+ww)*viewport_width / (2*ww) + viewport_x1
 screen_y = (yy+ww)*viewport_height / (2*ww) + viewport_y1 |
| --- |

Each VTX command that completes the definition of a polygon (ie. each 3rd for
Separate Trangles) does additionally store data in Polygon List RAM.

VTX commands may be issued only between Begin and End commands.

**Clipping**

Polygons are clipped to the 6 sides of the view volume (ie. to the left, right,
top, bottom, near, and far edges). If one or more vertic(es) exceed one of
these sides, then these vertic(es) are replaced by two newly created vertices
(which are located on the intersections of the polygon edges and the view
volume edge).

Depending on the number of clipped vertic(es), this may increase or decrease
the number of entries in Vertex RAM (ie. minus N clipped vertices, plus 2 new
vertices). Also, clipped polygons which are part of polygon strips are
converted to separate polygons (which does increase number of entries in Vertex
RAM). Polygons that are fully outside of the View Volume aren't stored in
Vertex RAM, nor in Polygon RAM (the only exception are polygons that are
located exactly one pixel below of, or right of lower/right edges, which appear
to be accidently stored in memory).

| DS 3D Polygon Light Parameters |
| --- |

The lighting operation is performed by executing the Normal command (which sets
the VertexColor based on the Light/Material parameters) (to the rest of the
hardware it doesn't matter if the VertexColor was set by Color command or by
Normal command). Light is calculated only for the Front side of the polygon
(assuming that the Normal is matched to that side), so the Back side will be
(incorrectly) using the same color.

**40004C8h - Cmd 32h - LIGHT_VECTOR - Set Light's Directional Vector (W)**

Sets direction of the specified light (ie. the light selected in Bit30-31).

```

| 0-9 Directional Vector's X component (1bit sign + 9bit fractional part)
 10-19 Directional Vector's Y component (1bit sign + 9bit fractional part)
 20-29 Directional Vector's Z component (1bit sign + 9bit fractional part)
 30-31 Light Number (0..3) |
| --- |

Upon executing this command, the incoming vector is multiplied by the current
Directional Matrix, the result is then applied as LightVector. This allows to
rotate the light direction. However, normally, to keep the light unrotated, be
sure to use LoadIdentity (in MtxMode=2) before setting the LightVector.

**40004CCh - Cmd 33h - LIGHT_COLOR - Set Light Color (W)**

Sets the color of the specified light (ie. the light selected in Bit30-31).

```

| 0-4 Red (0..1Fh) ;\light color this will be combined with
 5-9 Green (0..1Fh) ; diffuse, specular, and ambient colors
 10-14 Blue (0..1Fh) ;/upon execution of the normal command
 15-29 Not used
 30-31 Light Number (0..3) |
| --- |

**40004C0h - Cmd 30h - DIF_AMB - MaterialColor0 - Diffuse/Ambient Reflect. (W)**

```

| 0-4 Diffuse Reflection Red ;\light(s) that directly hits the polygon,
 5-9 Diffuse Reflection Green ; ie. max when NormalVector has opposite
 10-14 Diffuse Reflection Blue ;/direction of LightVector
 15 Set Vertex Color (0=No, 1=Set Diffuse Reflection Color as Vertex Color)
 16-20 Ambient Reflection Red ;\light(s) that indirectly hits the polygon,
 21-25 Ambient Reflection Green ; ie. assuming that light is reflected by
 26-30 Ambient Reflection Blue ;/walls/floor, regardless of LightVector
 31 Not used |
| --- |

With Bit15 set, the lower 15bits are applied as VertexColor (exactly as when
when executing the Color command), the purpose is to use it as default color
(eg. when outcommenting the Normal command), normally, when using lighting, the
color setting gets overwritten (as soon as executing the Normal command).

**40004C4h - Cmd 31h - SPE_EMI - MaterialColor1 - Specular Ref. & Emission (W)**

```

| 0-4 Specular Reflection Red ;\light(s) reflected towards the camera,
 5-9 Specular Reflection Green ; ie. max when NormalVector is in middle of
 10-14 Specular Reflection Blue ;/LightVector and ViewDirection
 15 Specular Reflection Shininess Table (0=Disable, 1=Enable)
 16-20 Emission Red ;\light emitted by the polygon itself,
 21-25 Emission Green ; ie. regardless of light colors/vectors,
 26-30 Emission Blue ;/and no matter if any lights are enabled
 31 Not used |
| --- |

Caution: Specular Reflection WON'T WORK when the ProjectionMatrix is rotated.

**40004D0h - Cmd 34h - SHININESS - Specular Reflection Shininess Table (W)**

Write 32 parameter words (each 32bit word containing four 8bit entries),
entries 0..3 in the first word, through entries 124..127 in the last word:

```

| 0-7 Shininess 0 (unsigned fixed-point, 0bit integer, 8bit fractional part)
 8-15 Shininess 1 ("")
 16-23 Shininess 2 ("")
 24-31 Shininess 3 ("") |
| --- |

If the table is disabled (by MaterialColor1.Bit15), then reflection will act as
if the table would be filled with linear increasing numbers.

**4000484h - Cmd 21h - NORMAL - Set Normal Vector (W)**

In short, this command does calculate the VertexColor, based on the various
light-parameters.

In detail, upon executing this command, the incoming vector is multiplied by
the current Directional Matrix, the result is then applied as NormalVector
(giving it the same rotation as used for the following polygon vertices).

```

| 0-9 X-Component of Normal Vector (1bit sign + 9bit fractional part)
 10-19 Y-Component of Normal Vector (1bit sign + 9bit fractional part)
 20-29 Z-Component of Normal Vector (1bit sign + 9bit fractional part)
 30-31 Not used |
| --- |

Defines the Polygon's Normal. And, does then update the Vertex Color; by
recursing the View Direction, the NormalVector, the LightVector(s), and
Light/Material Colors. The execution time of the Normal command varies
depending on the number of enabled light(s).

**Additional Light Registers**

Additionally to above registers, light(s) must be enabled in PolygonAttr (mind
that changes to PolygonAttr aren't applied until next Begin command). And, the
Directional Matrix must be set up correctly (in MtxMode=2) for the LightVector
and NormalVector commands.

**Normal Vector**

The Normal vector must point "away from the polygon surface" (eg. for the
floor, the Normal should point upwards). That direction is implied by the
polygon vertices, however, the hardware cannot automatically calculate it, so
it must be set manually with the Normal command (prior to the VTX-commands).

When using lighting, the Normal command must be re-executed after switching
Lighting on/off, or after changing light/material parameters. And, of course,
also before defining polygons with different orientation. Polygons with same
orientation (eg. horizontal polygon surfaces) and same material color can use
the same Normal. Changing the Normal per polygon gives differently colored
polygons with flat surfaces, changing the Normal per vertex gives the illusion
of curved surfaces.

**Light Vector**

Each light consists of parallel beams; similar to sunlight, which appears to us
(due to the great distance) to consist of parallel beams, all emmitted into the
same direction; towards Earth.

In reality, light is emitted into ALL directions, originated from the light
source (eg. a candle), the hardware doesn't support that type of non-parallel
light. However, the light vectors can be changed per polygon, so a polygon that
is located north of the light source may use different light direction than a
polygon that is east of the light source.

And, of course, Light 0..3 may (and should) have different directions.

**Normalized Vectors**

The Normal Vector and the Light Vectors should be normalized (ie. their length
should be 1.0) (in practice: something like 0.99, since the registers have only
fractional parts) (a length of 1.0 can cause overflows).

**Lighting Limitations**

The functionality of the light feature is limited to reflecting light to the
camera (light is not reflected to other polygons, nor does it cast shadows on
other polygons). However, independently of the lighting feature, the DS
hardware does allow to create shadows, see:

DS 3D Shadow Polygons

**Internal Operation on Normal Command**

```

| IF TexCoordTransformMode=2 THEN TexCoord=NormalVector*Matrix (see TexCoord)
 NormalVector=NormalVector*DirectionalMatrix
 VertexColor = EmissionColor
 FOR i=0 to 3
 IF PolygonAttrLight[i]=enabled THEN
 DiffuseLevel = max(0,-(LightVector[i]*NormalVector))
 ShininessLevel = max(0,(-HalfVector[i])*(NormalVector))^2
 IF TableEnabled THEN ShininessLevel = ShininessTable[ShininessLevel]
 ;note: below processed separately for the R,G,B color components...
 VertexColor = VertexColor + SpecularColor*LightColor[i]*ShininessLevel
 VertexColor = VertexColor + DiffuseColor*LightColor[i]*DiffuseLevel
 VertexColor = VertexColor + AmbientColor*LightColor[i]
 ENDIF
 NEXT i |
| --- |

**Internal Operation on Light_Vector Command (for Light i)**

```

| LightVector[i] = (LightVector*DirectionalMatrix)
 HalfVector[i] = (LightVector[i]+LineOfSightVector)/2 |
| --- |

**LineOfSightVector (how it SHOULD work)**

Ideally, the LineOfSightVector should point from the camera to the vertic(es),
however, the vertic(es) are still unknown at time of normal command, so it is
just pointing from the camera to the screen, ie.

```

| LineOfSightVector = (0,0,-1.0) |
| --- |

Moreover, the LineOfSightVector should be multiplied by the Projection Matrix
(so the vector would get rotated accordingly when the camera gets rotated),
and, after multiplication by a scaled matrix, it'd be required to normalize the
resulting vector.

**LineOfSightVector (how it DOES actually work)**

However, the NDS cannot normalize vectors by hardware, and therefore, it does
completely leave out the LineOfSightVector*ProjectionMatrix multiplication. So,
the LineOfSightVector is always (0,0,-1.0), no matter of any camera rotation.
That means,

```

| Specular Reflection WON'T WORK when the ProjectionMatrix is rotated (!) |
| --- |

So, if you want to rotate the "camera" (in MTX_MODE=0), then you must instead
rotate the "world" in the opposite direction (in MTX_MODE=2).

That problem applies only to Specular Reflection, ie. only if Lighting is used,
and only if the Specular Material Color is nonzero.

**Maths Notes**

Note on Vector*Vector multiplication: Processed as LineVector*RowVector, so the
result is a number (aka scalar) (aka a matrix with only 1x1 elements),
multiplying two (normalized) vectors results in: "cos(angle)=vec1*vec2", ie.
the cosine of the angle between the two vectors.

The various Normal/Light/Half/Sight vectors are only 3-dimensional (x,y,z), ie.
only the upper-left 3x3 matrix elements are used on multiplications with the
4x4 DirectionalMatrix.

| DS 3D Shadow Polygons |
| --- |

The DS hardware's Light-function allows to reflect light to the camera, it does
not reflect light to other polygons, and it does not cast any shadows. For
shadows at fixed locations it'd be best to pre-calculate their shape and
position, and to change the vertex color of the shaded polygons.

Additionally, the Shadow Polygon feature can be used to create animated
shadows, ie. moved objects and variable light sources.

**Shadow Polygons and Shadow Volume**

The software must define a Shadow Volume (ie. the region which doesn't contain
light), the hardware does then automatically draw the shadow on all pixels
whose x/y/z-coordinates are inside of that region.

The Shadow Volume must be defined by several Shadow Polygons which are
enclosing the shaded region. The 'top' of the shadow volume should be usually
translated to the position of the object that casts the shadow, if the light
direction changes then the shadow volume should be also rotated to match the
light direction. The 'length' of the shadow volume should be (at least) long
enough to reach from the object to the walls/floor where the shadow is to be
drawn. The shadow volume must be passed TWICE to the hardware:

**Step 1 - Shadow Volume for Mask**

Set Polygon_Attr Mode=Shadow, PolygonID=00h, Back=Render, Front=Hide,
Alpha=01h..1Eh, and pass the shadow volume (ie. the shadow polygons) to the
geometry engine.

The Back=Render / Front=Hide setting causes the 'rear-side' of the shadow
volume to be rendered, of course only as far as it is in front of other
polygons. The Mode=Shadow / ID=00h setting causes the polygon NOT to be drawn
to the Color Buffer - instead, flags are set in the Stencil Buffer (to be used
in Step 2).

**Step 2 - Shadow Volume for Rendering**

Simply repeat step 1, but with Polygon_Attr Mode=Shadow, PolygonID=01h..3Fh,
Back=Render(what/why?), Front=Render, Alpha=01h..1Eh.

The Front=Render setting causes the 'front-side' of the shadow volume to be
rendered, again, only as far as it is in front of other polygons. The
Mode=Shadow / ID>00h setting causes the polygon to be drawn to the Color
Buffer as usually, but only if the Stencil Buffer bits are zero (ie. the
portion from Step 1 is excluded) (additionally, Step 2 resets the stencil bits
after checking them). Moreover, the shadow is rendered only if its Polygon ID
differs from the ID in the Attribute Buffer.

**Shadow Alpha and Shadow Color**

The Alpha=Translucent setting in Step 1 and 2 ensures that the Shadow is drawn
AFTER the normal (opaque) polygons have been rendered. In Step 2 it does
additionally specify the 'intensity' of the shadow. For normal shadows, the
Vertex Color should be usually black, however, the shadow volume may be also
used as 'spotlight volume' when using other colors.

**Rendering Order**

The Mask Volume must be rendered prior to the Rendering Volume, ie. Step 1 and
2 must be performed in that order, and, to keep that order intact, Auto-sorting
must have been disabled in the previous Swap_Buffers command.

The shadow volume must be rendered after the 'target' polygons have been
rendered, for opaque targets this is done automatically (due to the translucent
alpha setting; translucent polygons are always rendered last, even with
auto-sort disabled).

**Translucent Targets**

Casting shadows on Translucent Polygons. First draw the translucent target
(with update depth buffer enabled, required for the shadow z-coordinates), then
draw the Shadow Mask/Rendering volumes.

Due to the updated depth buffer the shadow will be cast only on the translucent
target (not on any other polygons underneath of the translucent polygon). If
you want the shadow to appear on both: Draw draw the Shadow Mask/Rendering
volume TWICE (once before, and once after drawing the translucent target).

**Polygon ID and Fog Enable**

The "Render only if Polygon ID differs" feature (see Step 2) allows to prevent
the shadow to be cast on the object that casts the shadow (ie. the object and
shadow should have the same IDs). The feature also allows to select whether
overlapping shadows (with same/different IDs) are shaded once or twice.

The old Fog Enable flag in the Attribute Buffer is ANDed with the Fog Enable
flag of the Shadow Polygons, this allows to exclude Fog in shaded regions.

**Shadow Volume Open/Closed Shapes**

Normally, the shadow volume should have a closed shape, ie. should have
rear-sides (step 1), and corresponding front-sides (step 2) for all possible
viewing angles. That is required for the shadow to be drawn correctly, and also
for the Stencil Buffer to be reset to zero (in step 2, so that the stencil bits
won't disturb other shadow volumes).

Due to that, drawing errors may occur if the shadow volume's front or rear side
gets clipped by near/far clip plane.

One exception is that the volume doesn't need a bottom-side (with a suitable
volume length, the bottom may be left open, since it vanishes in the
floor/walls anyways).

| DS 3D Texture Attributes |
| --- |

**4000488h - Cmd 22h - TEXCOORD - Set Texture Coordinates (W)**

Specifies the texture source coordinates within the texture bitmap which are to
be associated with the next vertex.

```

| Parameter 1, Bit 0-15 S-Coordinate (X-Coordinate in Texture Source)
 Parameter 1, Bit 16-31 T-Coordinate (Y-Coordinate in Texture Source)
 Both values are 1bit sign + 11bit integer + 4bit fractional part.
 A value of 1.0 (=1 SHL 4) equals to one Texel. |
| --- |

With Position 0.0 , 0.0 drawing starts from upperleft of the Texture.

With positive offsets, drawing origin starts more "within" the texture.

With negative offsets, drawing starts "before" the texture.

"When texture mapping, the Geometry Engine works faster if you issue commands
in the order TexCoord -> Normal -> Vertex."

**40004A8h - Cmd 2Ah - TEXIMAGE_PARAM - Set Texture Parameters (W)**

```

| 0-15 Texture VRAM Offset div 8 (0..FFFFh -> 512K RAM in Slot 0,1,2,3)
 (VRAM must be allocated as Texture data, see Memory Control chapter)
 16 Repeat in S Direction (0=Clamp Texture, 1=Repeat Texture)
 17 Repeat in T Direction (0=Clamp Texture, 1=Repeat Texture)
 18 Flip in S Direction (0=No, 1=Flip each 2nd Texture) (requires Repeat)
 19 Flip in T Direction (0=No, 1=Flip each 2nd Texture) (requires Repeat)
 20-22 Texture S-Size (for N=0..7: Size=(8 SHL N); ie. 8..1024 texels)
 23-25 Texture T-Size (for N=0..7: Size=(8 SHL N); ie. 8..1024 texels)
 26-28 Texture Format (0..7, see below)
 29 Color 0 of 4/16/256-Color Palettes (0=Displayed, 1=Made Transparent)
 30-31 Texture Coordinates Transformation Mode (0..3, see below) |
| --- |

Texture Formats:

```

| 0 No Texture -
 1 A3I5 Translucent Texture 8bpp
 2 4-Color Palette Texture 2bpp
 3 16-Color Palette Texture 4bpp
 4 256-Color Palette Texture 8bpp
 5 4x4-Texel Compressed Texture 3bpp
 6 A5I3 Translucent Texture 8bpp
 7 Direct Texture 16bpp |
| --- |

Texture Coordinates Transformation Modes:

```

| 0 Do not Transform texture coordinates
 1 TexCoord source
 2 Normal source
 3 Vertex source |
| --- |

The S-Direction equals to the horizontal direction of the source bitmap.

The T-Direction, T-repeat, and T-flip are the same in vertical direction.

For a "/" shaped texture, the S-clamp, S-repeat, and S-flip look like so:

```

| Clamp _____ Repeat Repeat+Flip
 _____/ /////////// /\/\/\/\/\/ |
| --- |

With "Clamp", the texture coordinates are clipped to MinMax(0,Size-1), so the
texels at the edges of the texture bitmap are repeated (to avoid that effect,
fill the bitmap edges by texels with alpha=0, so they become invisible).

**40004ACh - Cmd 2Bh - PLTT_BASE - Set Texture Palette Base Address (W)**

```

| 0-12 Palette Base Address (div8 or div10h, see below)
 (Not used for Texture Format 7: Direct Color Texture)
 (0..FFF8h/8 for Texture Format 2: ie. 4-color-palette Texture)
 (0..17FF0h/10h for all other Texture formats)
 13-31 Not used |
| --- |

The palette data occupies 16bit per color:

```

| Bit0-4: Red
 Bit5-9: Green
 Bit10-14: Blue
 Bit15: Not used |
| --- |

VRAM must be allocated as Texture Palette, there can be up to 6 Slots
allocated, ie. the addressable 18000h bytes, see Memory Control chapter.

**TexImageParam and TexPlttBase**

Can be issued per polygon (except within polygon strips).

| DS 3D Texture Formats |
| --- |

**Format 2: 4-Color Palette Texture**

Each Texel occupies 2bit, the first Texel is located in LSBs of 1st byte.

In this format, the Palette Base is specified in 8-byte steps; all other
formats use 16-byte steps (see PLTT_BASE register).

**Format 3: 16-Color Palette Texture**

Each Texel occupies 4bit, the 1st Texel is located in LSBs of 1st byte.

**Format 4: 256-Color Palette Texture**

Each Texel occupies 8bit, the 1st Texel is located in 1st byte.

**Format 7: Direct Color Texture**

Each Texel occupies 16bit, the 1st Texel is located in 1st halfword.

Bit0-4: Red, Bit5-9: Green, Bit10-14: Blue, Bit15: Alpha

**Format 1: A3I5 Translucent Texture (3bit Alpha, 5bit Color Index)**

Each Texel occupies 8bit, the 1st Texel is located in 1st byte.

```

| Bit0-4: Color Index (0..31) of a 32-color Palette
 Bit5-7: Alpha (0..7; 0=Transparent, 7=Solid) |
| --- |

The 3bit Alpha value (0..7) is internally expanded into a 5bit Alpha value
(0..31) as follows: Alpha=(Alpha*4)+(Alpha/2).

**Format 6: A5I3 Translucent Texture (5bit Alpha, 3bit Color Index)**

Each Texel occupies 8bit, the 1st Texel is located in 1st byte.

```

| Bit0-2: Color Index (0..7) of a 8-color Palette
 Bit3-7: Alpha (0..31; 0=Transparent, 31=Solid) |
| --- |

**Format 5: 4x4-Texel Compressed Texture**

Consists of 4x4 Texel blocks in Slot 0 or 2, 32bit per block, 2bit per Texel,

```

| Bit0-7 Upper 4-Texel row (LSB=first/left-most Texel)
 Bit8-15 Next 4-Texel row ("")
 Bit16-23 Next 4-Texel row ("")
 Bit24-31 Lower 4-Texel row ("") |
| --- |

Additional Palette Index Data for each 4x4 Texel Block is located in Slot 1,

```

| Bit0-13 Palette Offset in 4-byte steps; Addr=(PLTT_BASE*10h)+(Offset*4)
 Bit14-15 Transparent/Interpolation Mode (0..3, see below) |
| --- |

whereas, the Slot 1 offset is related to above Slot 0 or 2 offset,

```

| slot1_addr = slot0_addr / 2 ;lower 64K of Slot1 assoc to Slot0
 slot1_addr = slot2_addr / 2 + 10000h ;upper 64K of Slot1 assoc to Slot2 |
| --- |

The 2bit Texel values (0..3) are intepreted depending on the Mode (0..3),

```

| Texel Mode 0 Mode 1 Mode 2 Mode 3
 0 Color 0 Color0 Color 0 Color 0
 1 Color 1 Color1 Color 1 Color 1
 2 Color 2 (Color0+Color1)/2 Color 2 (Color0*5+Color1*3)/8
 3 Transparent Transparent Color 3 (Color0*3+Color1*5)/8 |
| --- |

Mode 1 and 3 are using only 2 Palette Colors (which requires only half as much
Palette memory), the 3rd (and 4th) Texel Colors are automatically set to above
values (eg. to gray-shades if color 0 and 1 are black and white).

Note: The maximum size for 4x4-Texel Compressed Textures is 1024x512 or
512x1024 (which are both occupying the whole 128K in slot 0 or 2, plus 64K in
slot1), a larger size of 1024x1024 cannot be used because of the gap between
slot 0 and 2.

| DS 3D Texture Coordinates |
| --- |

For textured polygons, a texture coordinate must be associated with each vertex
of the polygon. The coordinates (S,T) are defined by TEXCOORD command
(typically issued prior to each VTX command), and can be optionally
automatically transformed, by the Transformation Mode selected in
TEXIMAGE_PARAM register.

**Texture Matrix**

Although the texture matrix is 4x4, with values m[0..15], only the left two
columns of this matrix are actually used. In Mode 2 and 3, the bottom row of
the matrix is replaced by S and T values from most recent TEXCOORD command.

**Texture Coordinates Transformation Mode 0 - No Transform**

The values are set upon executing the TEXCOORD command,

```

| ( S' T' ) = ( S T ) |
| --- |

Simple coordinate association, without using the Texture Matrix at all.

**Texture Coordinates Transformation Mode 1 - TexCoord source**

The values are calculated upon executing the TEXCOORD command,

```

| | m[0] m[1] |
 ( S' T' ) = ( S T 1/16 1/16 ) * | m[4] m[5] |
 | m[8] m[9] |
 | m[12] m[13] | |
| --- |

Can be used to produce a simple texture scrolling, rotation, or scaling, by
setting a translate, rotate, or scale matrix for the texture matrix.

**Texture Coordinates Transformation Mode 2 - Normal source**

The values are calculated upon executing the NORMAL command,

```

| | m[0] m[1] |
 ( S' T' ) = ( Nx Ny Nz 1.0 ) * | m[4] m[5] |
 | m[8] m[9] |
 | S T | |
| --- |

Can be used to produce spherical reflection mapping by setting the texture
matrix to the current directional vector matrix, multiplied by a scaling matrix
that expands the directional vector space from -1.0..+1.0 to one half of the
texture size. For that purpose, translate the origin of the texture coordinate
to the center of the spherical texture by using TexCoord command (spherical
texture means a bitmap that contains some circle-shaped image).

**Texture Coordinates Transformation Mode 3 - Vertex source**

The values are calculated upon executing any VTX commands,

```

| | m[0] m[1] |
 ( S' T' ) = ( Vx Vy Vz 1.0 ) * | m[4] m[5] |
 | m[8] m[9] |
 | S T | |
| --- |

Can be used to produce texture scrolls dependent on the View coordinates by
copying the current position coordinate matrix into the texture matrix. For
example, the PositionMatrix can be obtained via CLIPMTX_RESULT (see there for
details), and that values can be then manually copied to the TextureMatrix.

**Sign+Integer+Fractional Parts used in above Formulas**

```

| Matrix m[..] 1+19+12 (32bit)
 Vertex Vx,Vy,Vz 1+3+12 (16bit)
 Normal Nx,Ny,Nz 1+0+9 (10bit)
 Constant 1.0 0+1+0 (1bit)
 Constant 1/16 0+0+4 (4bit)
 TexCoord S,T 1+11+4 (16bit)
 Result S',T' 1+11+4 (16bit) <-------- clipped to that size ! |
| --- |

Observe that the S',T' values are clipped to 16bit size. Ie. after the
Vector*Matrix calaction, the result is shifted right (to make it having a 4bit
fraction), and the value is then masked to 16bit size.

| DS 3D Texture Blending |
| --- |

Polygon pixels consist of a Vertex Color, and of Texture Colors.

These colors can be blended as described below. Or, to use only either one:

To use only the Vertex Color: Select No Texture in TEXIMAGE_PARAM.

To use only the Texture Color: Select Modulation Mode and Alpha=31 in
POLYGON_ATTR, and set COLOR to 7FFFh (white), or to gray values (to decrease
brightness of the texture color).

**Vertex Color (Rv,Gv,Bv,Av)**

The Vertex Color (Rv,Gv,Bv) can be changed per Vertex (either by Color, Normal,
or Material0 command), pixels between vertices are shaded to medium values of
the surrounding vertices. The Vertex Alpha (Av), can be changed only per
polygon (by PolygonAttr command).

**Texture Colors (Rt,Gt,Bt,At)**

The Texture Colors (Rt,Gt,Bt), and Alpha value (At), are defined by the Texture
Bitmap. For formats without Alpha value, assume At=31 (solid), and for formats
with 1bit Alpha assume At=A*31.

**Shading Table Colors (Rs,Gs,Bs)**

In Toon/Highlight Shading Mode, the red component of the Vertex Color (Rv) is
mis-used as an index in the Shading Table, ie. Rv is used to read Shading
Colors (Rs,Gs,Bs) from the table; the green and blue components of the Vertex
Color (Gv,Bv) are unused in this mode. The Vertex Alpha (Av) is kept used.

Shading is used in Polygon Mode 2, whether it is Toon or Highlight Shading is
selected in DISP3DCNT; this is a per-frame selection, so only either one can be
used.

**Texture Blending - Modulation Mode (Polygon Attr Mode 0)**

```

| R = ((Rt+1)*(Rv+1)-1)/64
 G = ((Gt+1)*(Gv+1)-1)/64
 B = ((Bt+1)*(Bv+1)-1)/64
 A = ((At+1)*(Av+1)-1)/64 |
| --- |

The multiplication result is decreased intensity (unless both factors are 63).

**Texture Blending - Decal Mode (Polygon Attr Mode 1)**

```

| R = (Rt*At + Rv*(63-At))/64 ;except, when At=0: R=Rv, when At=31: R=Rt
 G = (Gt*At + Gv*(63-At))/64 ;except, when At=0: G=Gv, when At=31: G=Gt
 B = (Bt*At + Bv*(63-At))/64 ;except, when At=0: B=Bv, when At=31: B=Bt
 A = Av |
| --- |

The At value is used (only) as ratio for Texture color vs Vertex Color.

**Texture Blending - Toon Shading (Polygon Mode 2, DISP3DCNT=Toon)**

The vertex color Red component (Rv) is used as an index in the toon table.

```

| R = ((Rt+1)*(Rs+1)-1)/64 ;Rs=ToonTableRed[Rv]
 G = ((Gt+1)*(Gs+1)-1)/64 ;Gs=ToonTableGreen[Rv]
 B = ((Bt+1)*(Bs+1)-1)/64 ;Bs=ToonTableBlue[Rv]
 A = ((At+1)*(Av+1)-1)/64 |
| --- |

This is same as Modulation Mode, but using Rs,Gs,Bs instead Rv,Gv,Bv.

**Texture Blending - Highlight Shading (Polygon Mode 2, DISP3DCNT=Highlight)**

```

| R = ((Rt+1)*(Rs+1)-1)/64+Rs ;truncated to MAX=63
 G = ((Gt+1)*(Gs+1)-1)/64+Gs ;truncated to MAX=63
 B = ((Bt+1)*(Bs+1)-1)/64+Bs ;truncated to MAX=63
 A = ((At+1)*(Av+1)-1)/64 |
| --- |

Same as Toon Shading, with additional addition offset, the addition may
increase the intensity, however, it may also change the hue of the color.

Above formulas are for 6bit RGBA values, ie. 5bit values internally expanded to
6bit as such: IF X>0 THEN X=X*2+1.

**Uni-Colored Textures**

Although textures are normally containing "pictures", in some cases it makes
sense to use "blank" textures that are filled with a single color:

Wire-frame polygons are always having Av=31, however, they can be made
transparent by using Translucent Textures (ie. A5I3 or A3I5 formats) with
At<31.

In Toon/Highlight shading modes, the Vertex Color is mis-used as table index,
however, Toon/Highlight shading can be used on uni-colored textures, which is
more or less the same as using Toon/Highlight shading on uni-colored
Vertex-colors.

| DS 3D Toon, Edge, Fog, Alpha-Blending, Anti-Aliasing |
| --- |

**4000380h..3BFh - TOON_TABLE - Toon Table (W)**

This 64-byte region contains the 32 toon colors (16bit per color), used for
both Toon and Highlight Shading. In both modes, the Red (R) component of the
RGBA vertex color is mis-used as index to obtain the new RGB value from the
toon table, vertex Alpha (A) is kept used as is.

```

| Bit0-4: Red, Bit5-9: Green, Bit10-14: Blue, Bit15: Not Used |
| --- |

Shading can be enabled (per polygon) in Polygon_Attr, whether it is Toon or
Highlight Shading is set (per frame) in DISP3DCNT. For more info on shading,
see:

DS 3D Texture Blending

**4000330h..33Fh - EDGE_COLOR - Edge Colors 0..7 (W)**

This 16-byte region contains the 8 edge colors (16bit per color), Edge Color 0
is used for Polygon ID 00h..07h, Color 1 for ID 08h..0Fh, and so on.

```

| Bit0-4: Red, Bit5-9: Green, Bit10-14: Blue, Bit15: Not Used |
| --- |

Edge Marking allows to mark the edges of an object (whose polygons all have the
same ID) in a wire-frame style. Edge Marking can be enabled (per frame) in
DISP3DCNT. When enabled, the polygon edges are drawn at the edge color, but
only if the old ID value in the Attribute Buffer is different than the Polygon
ID of the new polygon, so no edges are drawn between connected or overlapping
polygons with same ID values.

Edge Marking is applied ONLY to opaque polygons (including wire-frames).

Edge Marking increases the size of opaque polygons (see notes below).

Edge Marking doesn't work very well with Anti-Aliasing (see Anti-Aliasing).

Technically, when rendering a polygon, it's edges (ie. the wire-frame region)
are flagged as possible-edges (but it's still rendered normally, without using
the edge-color). Once when all opaque polygons (*) have been rendered, the edge
color is applied to these flagged pixels, under following conditions: At least
one of the four surrounding pixels (up, down, left, right) must have different
polygon_id than the edge, and, the edge depth must be LESS than the depth of
that surrounding pixel (ie. no edges are rendered if the depth is GREATER or
EQUAL, even if the polygon_id differs). At the screen borders, edges seem to be
rendered in respect to the rear-plane's polygon_id entry (see Port 4000350h).

(*) Actually, edge-marking is reportedly performed not until all opaque AND
translucent polygons have been rendered. That brings up some effects/problems
when edges are covered by translucent polys: The edge-color is probably drawn
as is (ie. it'll overwrite the translucent color, rather than being blended
with the translucent color). And, any translucent polygons that do update the
depth buffer will cause total edge-marking malfunction (since edge-marking
involves the comparision of the current/surrounding pixel's depth values).

**4000358h - FOG_COLOR - Fog Color (W)**

Fog can be used to let more distant polygons to disappear in foggy grayness (or
in darkness, or other color). This is particulary useful to "hide" the far clip
plane. Fog can be enabled in DISP3DCNT.Bit7, moreover, when enabled, it can be
activated or deactivated per polygon (POLYGON_ATTR.Bit15), and per Rear-plane
(see there).

```

| 0-4 Fog Color, Red ;\
 5-9 Fog Color, Green ; used only when DISP3DCNT.Bit6 is zero
 10-14 Fog Color, Blue ;/
 15 Not used
 16-20 Fog Alpha ;-used no matter of DISP3DCNT.Bit6
 21-31 Not used |
| --- |

Whether or not fog is applied to a pixel depends on the Fog flag in the
framebuffer, the initial value of that flag can be defined in the rear-plane.
When rendering opaque pixels, the framebuffer's fog flag gets replaced by
PolygonAttr.Bit15. When rendering translucent pixels, the old flag in the
framebuffer gets ANDed with PolygonAttr.Bit15.

**400035Ch - FOG_OFFSET - Fog Depth Offset (W)**

```

| 0-14 Fog Offset (Unsigned) (0..7FFFh)
 15-31 Not used |
| --- |

FogDepthBoundary[0..31] (for FogDensity[0..31]) are defined as:

```

| FogDepthBoundary[n] = FOG_OFFSET + FOG_STEP*(n+1) ;with n = 0..31 |
| --- |

Whereas FOG_STEP is derived from the FOG_SHIFT value in DISP3DCNT.Bit8-11
(FOG_STEP=400h shr FOG_SHIFT) (normally FOG_SHIFT should be 0..10 (bigger shift
amounts of 11..15 would cause FOG_STEP to become zero, so only Density[0] and
Density[31] would be used).

The meaning of the depth values depends on whether z-values or w-values are
stored in the framebuffer (see SwapBuffers.Bit1).

For translucent polygons, the depth value (and therefore: the amount of fog)
depends on the depth update bit (see PolygonAttr.Bit11).

**4000360h..37Fh - FOG_TABLE - Fog Density Table (W)**

This 32-byte region contains FogDensity[0..31] (used at FogDepthBoundary[n]),

```

| 0-6 Fog Density (00h..7Fh = None..Full) (usually increasing values)
 7 Not used |
| --- |

FogDensity[0] is used for all pixels closer than FogDepthBoundary[0],
FogDensity[31] is used for all pixels more distant than FogDepthBoundary[0].

Density is linear interpolated for pixels that are between two Density depth
boundaries. The formula for Fog Blending is:

```

| FrameBuffer[R] = (FogColor[R]*Density + FrameBuffer[R]*(128-Density)) / 128
 FrameBuffer[G] = (FogColor[G]*Density + FrameBuffer[G]*(128-Density)) / 128
 FrameBuffer[B] = (FogColor[B]*Density + FrameBuffer[B]*(128-Density)) / 128
 FrameBuffer[A] = (FogColor[A]*Density + FrameBuffer[A]*(128-Density)) / 128 |
| --- |

If DISP3DCNT.Bit6 is set (=Alpha Only), then only FrameBuffer[A] is updated,
and FrameBuffer[RGB] are kepth unchanged. Density=127 is handled as if
Density=128.

Fog Glitch: The fog_alpha value appears to be ignored (treated as
fog_alpha=1Fh) in the region up to the first density boundary. However,
normally that value will be multiplied by zero (assumung that density[0] is
usually zero), so you won't ever notice that hardware glitch.

**Alpha-Blending (Polygon vs FrameBuffer)**

Alpha-Blending occurs for pixels of translucent polygons,

```

| FrameBuf[R] = (Poly[R]*(Poly[A]+1) + FrameBuf[R]*(31-(Poly[A])) / 32
 FrameBuf[G] = (Poly[G]*(Poly[A]+1) + FrameBuf[G]*(31-(Poly[A])) / 32
 FrameBuf[B] = (Poly[B]*(Poly[A]+1) + FrameBuf[B]*(31-(Poly[A])) / 32
 FrameBuf[A] = max(Poly[A],FrameBuf[A]) |
| --- |

There are three situations in which Alpha-Blending is bypassed (the old
Framebuf[R,G,B,A] value is then simply overwritten by Poly[R,G,B,A]):

```

| 1) Alpha-Blending is disabled (DISP3DCNT.Bit3=0)
 2) The polygon pixel is opaque (Poly[A]=31)
 3) The old framebuffer value is totally transparent (FrameBuf[A]=0) |
| --- |

The third case can happen if the rear-plane was initialized with Alpha=0, which
causes the polygon not to be blended with the rear-plane (which may give better
results when subsequently blending the 3D layer with the 2D engine).

Note: Totally transparent pixels (with Poly[A]=0) are not rendered (ie. neither
FrameBuf[R,G,B,A] nor FrameBuf[Depth,Fog,PolyID,etc.] are updated.

**Anti-Aliasing**

Anti-Aliasing can be enabled in DISP3DCNT, when enabled, the edges of opaque
polygons will be anti-aliased (ie. the pixels at the edges may become
translucent).

Anti-Aliasing is not applied on translucent polygons. And, Anti-Aliasing is not
applied on the interiors of the poylgons (eg. an 8x8 chessboard texture will be
anti-aliased only at the board edges, not at the edges of the 64 fields).

Anti-Aliasing is (accidently) applied to opaque 1dot polygongs, line-segments
and wire-frames (which results in dirty lines with missing pixels, 1dot polys
become totally invisible), workaround is to use translucent dots, lines and
wires (eg. with alpha=30).

Anti-Aliasing is (correctly) not applied to edges of Edge-Marked polygons, in
that special case even opaque line-segments and wire-frames are working even if
anti-aliasing is enabled (provided that they are edge-marked, ie. if their
polygon ID differs from the framebuffer's ID).

Anti-Aliasing is (accidently) making the edges of Edge-Marked polygons
translucent (with alpha=16 or so?), that reduces the contrast of the edge
colors. Moreover, if two of these translucent edges do overlap, then they are
blended twice (even if they have the same polygon_id, and even if the
depth_update bit in polygon_attr is set; both should normally prevent
double-blending), that scatters the brightness of such edges.

**Polygon Size**

In some cases, the NDS hardware doesn't render the lower/right edges of certain
polygons. That feature reduces rendering load, and, when rendering connected
polygons (eg. strips), then it'd be unnecessary to render that edges (since
they'd overlap with the upper/left edges of the other polygon). On the
contrary, if there's no connected polygon displayed, then the polygon may
appear smaller than expected. Small polygons with excluded edges are:

```

| Opaque polygons (except wire-frames) without Edge-Marking and Anti-Aliasing,
 and, all polygons with vertical right-edges (except line-segments).
 Plus, Translucent Polys when Alpha-Blending is disabled in DISP3DCNT.Bit3. |
| --- |

All other polygons are rendered at full size with all edges included (except
vertical right edges). Note: To disable the small-polygon feature, you can
enable edge-marking (which does increase the polygon size, even if no edges are
drawn, ie. even if all polys do have the same ID).

| DS 3D Status |
| --- |

**4000600h - GXSTAT - Geometry Engine Status Register (R and R/W)**

Bit 30-31 are R/W. Writing "1" to Bit15 does reset the Error Flag (Bit15), and
additionally resets the Projection Stack Pointer (Bit13), and probably (?) also
the Texture Stack Pointer. All other GXSTAT bits are read-only.

```

| 0 BoxTest,PositionTest,VectorTest Busy (0=Ready, 1=Busy)
 1 BoxTest Result (0=All Outside View, 1=Parts or Fully Inside View)
 2-7 Not used
 8-12 Position & Vector Matrix Stack Level (0..31) (lower 5bit of 6bit value)
 13 Projection Matrix Stack Level (0..1)
 14 Matrix Stack Busy (0=No, 1=Yes; Currently executing a Push/Pop command)
 15 Matrix Stack Overflow/Underflow Error (0=No, 1=Error/Acknowledge/Reset)
 16-24 Number of 40bit-entries in Command FIFO (0..256)
 (24) Command FIFO Full (MSB of above) (0=No, 1=Yes; Full)
 25 Command FIFO Less Than Half Full (0=No, 1=Yes; Less than Half-full)
 26 Command FIFO Empty (0=No, 1=Yes; Empty)
 27 Geometry Engine Busy (0=No, 1=Yes; Busy; Commands are executing)
 28-29 Not used
 30-31 Command FIFO IRQ (0=Never, 1=Less than half full, 2=Empty, 3=Reserved) |
| --- |

When GXFIFO IRQ is enabled (setting 1 or 2), the IRQ flag (IF.Bit21) is set
while and as long as the IRQ condition is true (and attempts to acknowledge the
IRQ by writing to IF.Bit21 have no effect). So that, the IRQ handler must
either fill the FIFO, or disable the IRQ (setting 0), BEFORE trying to
acknowledge the IRQ.

**4000604h - RAM_COUNT - Polygon List & Vertex RAM Count Register (R)**

```

| 0-11 Number of Polygons currently stored in Polygon List RAM (0..2048)
 12-15 Not used
 16-28 Number of Vertices currently stored in Vertex RAM (0..6144)
 13-15 Not used |
| --- |

If a SwapBuffers command has been sent, then the counters are reset 10 cycles
(at 33.51MHz clock) after next VBlank.

**4000320h - RDLINES_COUNT - Rendered Line Count Register (R)**

Rendering starts in scanline 214, the rendered lines are stored in a buffer
that can hold up to 48 scanlines. The actual screen output begins after
scanline 262, the lines are then read from the buffer and sent to the display.
Simultaneously, the rendering engine keeps writing new lines to the buffer
(ideally at the same speed than display output, so the buffer would always
contain 48 pre-calculated lines).

```

| 0-5 Minimum Number (minus 2) of buffered lines in previous frame (0..46)
 6-31 Not used |
| --- |

If rendering becomes slower than the display output, then the number of
buffered lines decreases. Smaller values in RDLINES indicate that additional
load to the rendering engine may cause buffer underflows in further frames, if
so, the program should reduce the number of polygons to avoid display glitches.

Even if RDLINES becomes zero, it doesn't indicate whether actual buffer
underflows have occured or not (underflows are indicated in DISP3DCNT Bit12).

| DS 3D Tests |
| --- |

**40005C0h - Cmd 70h - BOX_TEST - Test if Cuboid Sits inside View Volume (W)**

The BoxTest result indicates if one or more of the 6 faces of the box are fully
or parts of inside of the view volume. Can be used to reduce unnecessary
overload, ie. if the result is false, then the program can skip drawing of
objects which are inside of the box.

BoxTest verifies only if the faces of the box are inside view volume, and so,
it will return false if the whole view volume is located inside of the box
(still objects inside of the box may be inside of view).

```

| Parameter 1, Bit 0-15 X-Coordinate
 Parameter 1, Bit 16-31 Y-Coordinate
 Parameter 2, Bit 0-15 Z-Coordinate
 Parameter 2, Bit 16-31 Width (presumably: X-Offset?)
 Parameter 3, Bit 0-15 Height (presumably: Y-Offset?)
 Parameter 3, Bit 16-31 Depth (presumably: Z-Offset?)
 All values are 1bit sign, 3bit integer, 12bit fractional part |
| --- |

The result of the "coordinate+offset" additions should not overflow 16bit
vertex coordinate range (1bit sign, 3bit integer, 12bit fraction).

Before using BoxTest, be sure that far-plane-intersecting & 1-dot polygons
are enabled, if they aren't: Send the PolygonAttr command (with bit12,13 set to
enable them), followed by dummy Begin and End commands (required to apply the
new PolygonAttr settings). BoxTest should not be issued within Begin/End.

After sending the BoxTest command, wait until GXSTAT.Bit0 indicates Ready, then
read the result from GXSTAT.Bit1.

**40005C4h - Cmd 71h - POS_TEST - Set Position Coordinates for Test (W)**

```

| Parameter 1, Bit 0-15 X-Coordinate
 Parameter 1, Bit 16-31 Y-Coordinate
 Parameter 2, Bit 0-15 Z-Coordinate
 Parameter 2, Bit 16-31 Not used
 All values are 1bit sign, 3bit integer, 12bit fractional part. |
| --- |

Multiplies the specified line-vector (x,y,z,1) by the clip coordinate matrix.

After sending the command, wait until GXSTAT.Bit0 indicates Ready, then read
the result from POS_RESULT registers. POS_TEST can be issued anywhere (except
within polygon strips, huh?).

Caution: POS_TEST overwrites the internal VTX registers, so the next vertex
should be <fully> defined by VTX_10 or VTX_16, otherwise, when using
VTX_XY, VTX_XZ, VTX_YZ, or VTX_DIFF, then the new vertex will be relative to
the POS_TEST coordinates (rather than to the previous vertex).

**4000620h..62Fh - POS_RESULT - Position Test Results (R)**

This 16-byte region (4 words) contains the resulting clip coordinates (x,y,z,w)
from the POS_TEST command. Each value is 1bit sign, 19bit integer, 12bit
fractional part.

**40005C8h - Cmd 72h - VEC_TEST - Set Directional Vector for Test (W)**

```

| Parameter 1, Bit 0-9 X-Component
 Parameter 1, Bit 10-19 Y-Component
 Parameter 1, Bit 20-29 Z-Component
 Parameter 1, Bit 30-31 Not used
 All values are 1bit sign, 9bit fractional part. |
| --- |

Multiplies the specified line-vector (x,y,z,0) by the directional vector
matrix. Similar as for the NORMAL command, it does require Matrix Mode 2 (ie.
Position & Vector Simultaneous Set mode).

After sending the command, wait until GXSTAT.Bit0 indicates Ready, then read
the result ("the directional vector in the View coordinate space") from
VEC_RESULT registers.

**4000630h..635h - VEC_RESULT - Vector Test Results (R)**

This 6-byte region (3 halfwords) contains the resulting vector (x,y,z) from the
VEC_TEST command. Each value is 4bit sign, 0bit integer, 12bit fractional part.
The 4bit sign is either 0000b (positive) or 1111b (negative).

There is no integer part, so values >=1.0 or <-1.0 will cause overflows.

(Eg. +1.0 aka 1000h will be returned as -1.0 aka F000h due to overflow and
sign-expansion).

| DS 3D Rear-Plane |
| --- |

Other docs seem to refer to this as Clear-plane, rather than Rear-plane,
anyways, the plane can be an image, so it isn't always "cleared".

The view order is as such:

```

| --> 2D Layers --> 3D Polygons --> 3D Rear-plane --> 2D Layers --> 2D Backdrop |
| --- |

The rear-plane can be disabled (by making it transparent; alpha=0), so that the
2D layers become visible as background.

2D layers can be moved in front of, or behind the 3D layer-group (which is
represented as BG0 to the 2D Engine), 2D layers behind BG0 can be used instead
of, or additionally to the rear-plane.

The rear-plane can be initialized via below two registers (so all pixels in the
plane have the same colors and attributes), this method is used when
DISP3DCNT.14 is zero:

**4000350h - CLEAR_COLOR - Clear Color Attribute Register (W)**

```

| 0-4 Clear Color, Red
 5-9 Clear Color, Green
 10-14 Clear Color, Blue
 15 Fog (enables Fog to the rear-plane) (doesn't affect Fog of polygons)
 16-20 Alpha
 21-23 Not used
 24-29 Clear Polygon ID (affects edge-marking, at the screen-edges?)
 30-31 Not used |
| --- |

**4000354h - CLEAR_DEPTH - Clear Depth Register (W)**

```

| 0-14 Clear Depth (0..7FFFh) (usually 7FFFh = most distant)
 15 Not used
 16-31 See Port 4000356h, CLRIMAGE_OFFSET |
| --- |

The 15bit Depth is expanded to 24bit as "X=(X*200h)+((X+1)/8000h)*1FFh".

**Rear Color/Depth Bitmaps**

Alternately, the rear-plane can be initialized by bitmap data (allowing to
assign different colors & attributes to each pixel), this method is used
when DISP3DCNT.14 is set:

Consists of two bitmaps (one with color data, one with depth data), each
containing 256x256 16bit entries, and so, each occupying a whole 128K slot,

```

| Rear Color Bitmap (located in Texture Slot 2)
 0-4 Clear Color, Red
 5-9 Clear Color, Green
 10-14 Clear Color, Blue
 15 Alpha (0=Transparent, 1=Solid) (equivalent to 5bit-alpha 0 and 31)
 Rear Depth Bitmap (located in Texture Slot 3)
 0-14 Clear Depth, expanded to 24bit as X=(X*200h)+((X+1)/8000h)*1FFh
 15 Clear Fog (Initial fog enable value) |
| --- |

This method requires VRAM to be allocated to Texture Slot 2 and 3 (see Memory
Control chapter). Of course, in that case the VRAM is used as Rear-plane, and
cannot be used for Textures.

The bitmap method is restricted to 1bit alpha values (the register-method
allows to use a 5bit alpha value).

The Clear Polygon ID is kept defined in the CLEAR_COLOR register, even in
bitmap mode.

**4000356h - CLRIMAGE_OFFSET - Rear-plane Bitmap Scroll Offsets (W)**

The visible portion of the bitmap is 256x192 pixels (regardless of the viewport
setting, which is used only for polygon clipping). Internally, the bitmap is
256x256 pixels, so the bottom-most 64 rows are usually offscreen, unless
scrolling is used to move them into view.

```

| Bit0-7 X-Offset (0..255; 0=upper row of bitmap)
 Bit8-14 Y-Offset (0..255; 0=left column of bitmap) |
| --- |

The bitmap wraps to the upper/left edges when exceeding the lower/right edges.

| DS 3D Final 2D Output |
| --- |

The final 3D image (consisting of polygons and rear-plane) is passed to 2D
Engine A as BG0 layer (provided that DISPCNT is configured to use 3D as BG0).

**Scrolling**

The BG0HOFS register (4000010h) can be used the scroll the 3D layer
horizontally, the scroll region is 512 pixels, consisting of 256 pixels for the
3D image, followed by 256 transparent pixels, and then wrapped to the 3D image
again. Vertical scrolling (and rotation/scaling) cannot be used on the 3D
layer.

**BG Priority Order**

The lower 2bit of the BG0CNT register (4000008h) control the priority relative
to other BGs and OBJs, so the 3D layer can be in front of or behind 2D layers.
All other bits in BG0CNT have no effect on 3D, namely, mosaic cannot be used on
the 3D layer.

**Special Effects**

Special Effects Registers (4000050h..54h) can be used as such:

```

| Brightness up/down with BG0 as 1st Target via EVY (as for 2D)
 Blending with BG0 as 2nd Target via EVA/EVB (as for 2D)
 Blending with BG0 as 1st Target via 3D Alpha-values (unlike as for 2D) |
| --- |

The latter method probably (?) uses per-pixel 3D alpha values as such: EVA=A/2,
and EVB=16-A/2, without using the EVA/EVB settings in 4000052h.

**Window Feature**

Window Feature (4000040h..4Bh) can be used as for 2D.

"If the 3D screen has highest priority, then alpha-blending is always enabled,
regardless of the Window Control register's color effect enable flag [ie.
regardless of Bit5 of WIN0IN, WIN1IN, WINOBJ, WINOUT registers]"... not sure if
that is true, and if it superseedes the effect selection in Port 4000050h...?