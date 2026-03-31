# Gpuinternalregistersrasterizerregisters

> Source: https://problemkaputt.de/gbatek.htm
> Section: Gpuinternalregistersrasterizerregisters

3DS GPU Internal Registers - Rasterizer registers 
| |
| --- |

 
```

| ____________________________ Viewport and Scissor ____________________________ |
| --- |

**10401104h - PICA(0041h) - GPUREG_VIEWPORT_V_SCALE (3DS: 240/2) (R/W)**

**1040110Ch - PICA(0043h) - GPUREG_VIEWPORT_H_SCALE (3DS: 320/2 or 400/2) (R/W)**

```

| 0-23 float1.7.16, size/2 |
| --- |

Used to scale the vertex coordinates (usually in range -1.0 to +1.0) to screen
coordinates (in pixel units). Should be set to "H=width/2" and "V=height/2".
Normally that would be the screen size (or for smaller windows, the scissor
area size).

**10401108h - PICA(0042h) - GPUREG_VIEWPORT_V_STEP (3DS: 2/240) (R/W)**

**10401110h - PICA(0044h) - GPUREG_VIEWPORT_H_STEP (3DS: 2/320 or 2/400) (R/W)**

```

| 0-31 float1.7.24, 2/size |
| --- |

Used as stepping offsets when interpolating colors (and textures?) between
vertex coordinates. Should be set to same settings as in above SCALE registers,
but with "inverse" division, ie. "H=2/width" and "V=2/height" (instead of
"width/2" and "height/2").

For the odd floating point format, one could adjust one of the more common
formats (assuming sign=0 for the latter two cases):

```

| (float24)*100h ;convert float1.7.16 to float1.7.24
 (float32)*2-40000000h ;convert float1.8.23 to float1.7.24
 (float64)/10000000h-0C0000000h ;convert float1.11.52 to float1.7.24 |
| --- |

or it might also work to directly use screen coordinates for vertices (and set
all viewport registers to 1.0).

**10401194h - PICA(0065h) - GPUREG_SCISSORTEST_MODE (R/W)**

```

| 0-1 Mode (0=Disable, 1=Render Outside, 3=Render Inside, 2=Same as 0) |
| --- |

Allows to disable the scissor feature (ie. to render to whole framebuffer), or
to select whether to render only inside, or only outside of the scissor
rectangle. This can be useful if the framebuffer is divided into smaller
windows.

**10401198h - PICA(0066h) - GPUREG_SCISSORTEST_POS1 (R/W)**

**1040119Ch - PICA(0067h) - GPUREG_SCISSORTEST_POS2 (R/W)**

```

| 0-9 Vertical Position (unsigned, 0..3FFh pixels) ;0..240-1
 16-25 Horizontal Position (unsigned, 0..3FFh pixels) ;0..320-1 or 0..400-1 |
| --- |

Defines the scissor area, ranging from POS1=origin to POS2=origin+size-1
(whereas, coordinate 0,0 is lower-right).

**104011A0h - PICA(0068h) - GPUREG_VIEWPORT_XY (R/W)**

```

| 0-9 Vertical Position (signed, -200h..+1FFh pixels)
 16-25 Horizontal Position (signed, -200h..+1FFh pixels) |
| --- |

Default is 0,0 (with coordinate 0,0 being at center of framebuffer). Nonzero
values can cause polygons to wrap at the framebuffer edges (unless scissor test
is used, with position plus scissor size being within framebuffer dimensions).

```

| ____________________________________ Misc ____________________________________ |
| --- |

**10401100h - PICA(0040h) - GPUREG_FACECULLING_CONFIG (R/W)**

```

| 0-1 Culling (0=Show Front+Back, 1=Show Back, 2=Show Front, 3=Same as 0) |
| --- |

Selects whether to show the polygon front/back sides. Normally only the front
side is needed (and it's thus faster not to render the back side).

Note: The hardware detects front/back by checking whether the triangle's
coordinates are arranged clockwise or anti-clockwise on screen.

**1040111Ch - PICA(0047h) - GPUREG_FRAGOP_CLIP (R/W)**

```

| 0 Enable clipping plane(s?) (0=Disable, 1=Enable) |
| --- |

**10401120h+i*4 - PICA(0048h+i) - GPUREG_FRAGOP_CLIP_DATAi (i=0..3) (R/W)**

```

| 0-23 float1.7.16, Clipping plane coefficient i |
| --- |

"Used to configure clipping plane coefficients."

Note: There is a similar poorly described feature in OpenGL, but it's unknown
what it does and how to use it. It does somehow add another clipping plane
(additionally to the left/right, top/bottom, and near/far clipping plane).

**10401134h - PICA(004Dh) - GPUREG_DEPTHMAP_SCALE (R/W)**

```

| 0-23 float1.7.16, Near - Far ;default would be Z=-0.99999999 |
| --- |

Used to configure the depth range scale.

Uh, what means map, hmmm, maybe it just means "bitmap" with depth values?

Maybe defines the visible depth range? In Z or W units? But why called scale?

Maybe also "scales" float values to 24bit integers in depth buffer?

**10401138h - PICA(004Eh) - GPUREG_DEPTHMAP_OFFSET (R/W)**

```

| 0-23 float1.7.16, Near + Polygon Offset ;default would be Z=+0.0 |
| --- |

Used to configure the depth range bias.

Probably defines near clipping plane, in Z or W units? Unknown what "Polygon
Offset" refers to.

**104011B4h - PICA(006Dh) - GPUREG_DEPTHMAP_ENABLE (R/W)**

```

| 0 Enable depth range, uh what? (0=Disable/What??, 1=Enable/Default) |
| --- |

```

| ________________________________ Early Depth _________________________________ |
| --- |

Early Depth is something about drawing only depth values (without computing and
drawing colors/textures) for pre-checking if the polygon is visible... if so,
something is probably required to actually draw the color/texture part...?

**10401184h - PICA(0061h) - GPUREG_EARLYDEPTH_FUNC (R/W)**

```

| 0-1 Early depth function (0=GreaterEqual, 1=Greater, 2=LessEqual, 3=Less) |
| --- |

Configures the early depth test function.

**10401188h - PICA(0062h) - GPUREG_EARLYDEPTH_TEST1 (R/W)**

**10401460h - PICA(0118h) - GPUREG_EARLYDEPTH_TEST2 (R/W)**

```

| 0 Enable early depth test (0=Disable, 1=Enable) |
| --- |

**1040118Ch - PICA(0063h) - GPUREG_EARLYDEPTH_CLEAR (W)**

```

| 0 Trigger (0=Idle???, 1=Clear) (W) ;read=Always 0 |
| --- |

Triggers clearing the early depth data. Whatever that means, probably something
for restarting a new early depth test (whatever, it does NOT memfill the
DEPTHBUFFER).

**104011A8h - PICA(006Ah) - GPUREG_EARLYDEPTH_DATA (R/W)**

```

| 0-23 Early depth Clear value (unsigned) |
| --- |

```

| _______________________________ Shader Output ________________________________ |
| --- |

**1040113Ch - PICA(004Fh) - GPUREG_SH_OUTMAP_TOTAL (R/W)**

```

| 0-2 Number of shader output attributes (1..7) |
| --- |

**10401140h+i*4 - PICA(0050h+i) - GPUREG_SH_OUTMAP_Oi (i=0..6) (R/W)**

```

| 0-4 Semantic for the x component of the register (0..1Fh, see below)
 8-12 Semantic for the y component of the register (0..1Fh, see below)
 16-20 Semantic for the z component of the register (0..1Fh, see below)
 24-28 Semantic for the w component of the register (0..1Fh, see below) |
| --- |

These registers map components of the corresponding vertex shader output
register (o0..o6) to specific fixed-function semantics.

Semantic values:

```

| 00h = position.x ;\
 01h = position.y ; Vertex Position
 02h = position.z ;
 03h = position.w ;/
 04h = normquat.x ;\
 05h = normquat.y ; Quaternion specifying the normal/tangent frame
 06h = normquat.z ; (for fragment lighting)
 07h = normquat.w ;/
 08h = color.r ;\
 09h = color.g ; Vertex color
 0Ah = color.b ;
 0Bh = color.a ;/
 0Ch = texcoord0.u ;\Texture coordinates for texture 0
 0Dh = texcoord0.v ;/
 0Eh = texcoord1.u ;\Texture coordinates for texture 1
 0Fh = texcoord1.v ;/
 10h = texcoord0.w ;-Texture coordinate.w for texture 0
 12h = view.x ;\
 13h = view.y ; View vector (for fragment lighting)
 14h = view.z ;/
 16h = texcoord2.u ;\Texture coordinates for texture 2
 17h = texcoord2.v ;/
 1Fh = Unused component ;-for unused components of the output register |
| --- |

Observe that the X,Y,Z,W positions require Z,W even for simple 2D graphics (Z
should be -1.0..+0.0 to pass near/far clipping, W should be +1.0 to avoid
perspective division of the X,Y,Z values).

**104011BCh - PICA(006Fh) - GPUREG_SH_OUTATTR_CLOCK (R/W)**

```

| 0 'position.z' present (0=Absent, 1=Present)
 1 'color' component present (0=Absent, 1=Present)
 8 'texcoord0' component present (0=Absent, 1=Present)
 9 'texcoord1' component present (0=Absent, 1=Present)
 10 'texcoord2' component present (0=Absent, 1=Present)
 11-15 UNKNOWN (R/W)
 16 'texcoord0.w' present (0=Absent, 1=Present)
 17 UNKNOWN (R/W)
 24 'normquat' or 'view' component present (0=Absent, 1=Present) |
| --- |

Controls the clock supply to parts relating to certain attributes.

somewhat semantic?

**10401190h - PICA(0064h) - GPUREG_SH_OUTATTR_MODE (R/W)**

```

| 0 Use texture coordinates (0=Don't use, 1=Use) |
| --- |

Configures the shader output attribute mode, uh?

```

| ________________________________ Undocumented ________________________________ |
| --- |

**10401114h - PICA(0045h) - GPUREG_undoc_10401114h (R/W=00FFFFFFh)**

**10401118h - PICA(0046h) - GPUREG_undoc_10401118h (R/W=00FFFFFFh)**

```

| 0-23 UNKNOWN (0..FFFFFFh) (R/W) |
| --- |

**10401130h - PICA(004Ch) - GPUREG_undoc_10401130h (R/W=00000001h)**

```

| 0 UNKNOWN (0..1) (R/W) |
| --- |

**10401160h - PICA(0058h) - GPUREG_undoc_10401160h (R/W=00000101h)**

```

| 0 Flat Shading (0=Interpolate Colors, 1=Flat Uni-Color)
 8 UNKNOWN (0..1) (R/W) |
| --- |

The Flat Uni-Color mode uses only the third (last) color of each triangle.

Unknown if that does also apply to colors calculated from light vectors.

**10401164h - PICA(0059h) - GPUREG_undoc_10401164h (R/W=00000001h)**

```

| 0 UNKNOWN (0..1) (R/W) |
| --- |

**10401180h - PICA(0060h) - GPUREG_undoc_10401180h (R/W=00000301h)**

```

| 0 UNKNOWN (0..1) (R/W)
 8-9 Draw pixel(s) (0=All/Normal, 1=Each 2nd, 2=Each 4th, 3=Same as 0) |
| --- |

Allows to draw only each 2nd/4th pixel within 2x2 pixel cells (resulting in
striped/dotted output).

**104011A4h - PICA(0069h) - GPUREG_undoc_104011A4h (R/W=FFFF0001h)**

```

| 0 UNKNOWN (0..1) (R/W)
 16-31 UNKNOWN (0..FFFFh) (R/W) |
| --- |

**104011ACh - PICA(006Bh) - GPUREG_undoc_104011ACh (R/W=00000FFFh)**

```

| 0-11 UNKNOWN (0..FFFh) (R/W) |
| --- |

**104011B0h - PICA(006Ch) - GPUREG_undoc_104011B0h (DANGER, hangs when reading)**

```

| 0-31 UNKNOWN/DANGER, hangs when reading (maybe a fifo, hangs when empty?) |
| --- |

```

| ___________________________________ Status ___________________________________ |
| --- |

**10401168h - PICA(005Ah) - GPUREG_STAT_NUM_VERTICES_RECEIVED (R)**

**1040116Ch - PICA(005Bh) - GPUREG_STAT_NUM_TRIANGLES_RECEIVED (R)**

**10401170h - PICA(005Ch) - GPUREG_STAT_NUM_TRIANGLES_DISPLAYED (R)**

Indicates the number of processed vertices/triangles, the third counter does
increment only on actually displayed triangles (ie. when having passed the
front/back check) (and maybe further checks, like screen/scissor area).

```

| 0-xx? Status counter (incrementing) |
| --- |

Note: Reading from these registers (or any other GPU registers at
10401040h..1040107Fh or 10401100h..10401FFFh) does cause ARM11 to wait until
rendering has finished.