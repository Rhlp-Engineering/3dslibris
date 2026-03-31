# Filesvideolayoutclytflyt

> Source: https://problemkaputt.de/gbatek.htm
> Section: Filesvideolayoutclytflyt

3DS Files - Video Layout (CLYT/FLYT) 
| |
| --- |

 CLYT (.bclyt) is the layout format used on the 3DS. It stands for (Binary) CTR Layout, and is similar to the RLYT format used on the Wii.
 FLYT is something similar (used in 3DS eShop applet RomFS:\layout\).
 
 **CLYT Header**
 
```

| 000h 4 ID ("CLYT") ;or "FLYT"
 004h 2 Byte Order (FEFFh)
 006h 2 Header Size (0014h)
 008h 4 Version (02020000h)
 00Ch 4 Total Filesize
 010h 4 Number of following Chunks |
| --- |

**lyt1 (Layout)**

```

| 000h 4 Chunk ID "lyt1"
 004h 4 Chunk Size
 008h 4 Origin type (0=Classic, 1=Normal)
 00Ch 8 Vector2 Canvas Size |
| --- |

**txl1 (Texture List)**

```

| 000h 4 Chunk ID "txl1"
 004h 4 Chunk Size
 008h 4 Number of Textures (N)
 00Ch N*4 Texture Name Offsets (relative to the start of this "array")
 .. .. null terminated names |
| --- |

**fnl1 (Font List)**

```

| 000h 4 Chunk ID "fnl1"
 004h 4 Chunk Size
 008h 4 Number of Fonts (N)
 00Ch N*4 Font Name Offsets (relative to the start of this "array")
 .. .. null terminated names |
| --- |

**mat1 (Materials)**

```

| 000h 4 Chunk ID "mat1"
 004h 4 Chunk Size
 008h 4 Number of Materials (N)
 00Ch N*4 Material Entry Offsets (relative to the start of this "section") |
| --- |

After this, the material entries follow.

```

| 00h 14h String Material Name
 14h 4 RGBA8 Tev Color (Buffer Color?)
 18h 18h RGBA8[6] Tev Constant Colors
 30h 4 UInt32 Flags / bitfield
 Bit Flag / data
 0-1 Nr texMap
 2-3 Nr texMatrix
 4-5 Nr texCoordGen
 6-8 Nr tevStage
 9 Has alphaCompare
 10 Has blendMode
 11 Use Texture Only
 12 Separate Blend Mode
 14 Has Indirect Parameter
 15-16 Nr projectionTexGenParameter
 17 Has Font Shadow Parameter |
| --- |

Texture Map Entry:

```

| 00h 2 UInt16 Texture Index
 02h 1 Byte Bitfield
 Bit Data
 0-1 Wrap S (0=Clamp, 1=Repeat, 2=Mirror)
 2-3 Min Filter (0=Near, 1=Linear)
 02h 1 Byte Bitfield ;<-- uh, maybe at offset 03h?
 Bit Data
 0-1 Wrap T (0=Clamp, 1=Repeat, 2=Mirror)
 2-3 Mag Filter (0=Near, 1=Linear) |
| --- |

Texture Matrix Entry:

```

| 00h 8 Vector2 Translation
 08h 4 Single Rotation
 0Ch 8 Vector2 Scale |
| --- |

TODO: texCoordGen, tevStage, alphaCompare, blendMode, etc...

**pan1 (Pane)**

```

| 000h 4 Chunk ID "pan1"
 004h 4 Chunk Size
 008h 1 UInt8 Flags
 Bit Flag
 0 Visible
 1 InfluencedAlpha
 2 LocationAdjust
 009h 1 UInt8 Origin
 00Ah 1 UInt8 Alpha
 00Bh 1 UInt8 Pane magnification flags
 Bit Flag
 0 IgnorePartsMagnify
 1 AdjustToPartsBounds
 00Ch 18h String Pane name
 024h 0Ch Vector3 Translation
 030h 0Ch Vector3 Rotation
 03Ch 8 Vector2 Scale
 048h 8 Vector2 Size |
| --- |

**pic1 (Picture)**

```

| 000h 4 Chunk ID "pic1"
 004h 4 Chunk Size
 008h 4 RGBA8 Top left vertex color
 00Ch 4 RGBA8 Top right vertex color
 010h 4 RGBA8 Bottom left vertex color
 014h 4 RGBA8 Bottom right vertex color
 018h 2 UInt16 Material ID
 01Ah 2 UInt16 Nr texture coordinates = N
 Texture coordinates entry
 01Ch+N*20h 8 Vector2 Top left vertex texture coordinate
 024h+N*20h 8 Vector2 Top right vertex texture coordinate
 02Ch+N*20h 8 Vector2 Bottom left vertex texture coordinate
 034h+N*20h 8 Vector2 Bottom right vertex texture coordinate |
| --- |

**txt1 (Text)**

**wnd1 (Window)**

...?

**bnd1 (Bounding)**

```

| 000h 4 Chunk ID "bnd1"
 004h 4 Chunk Size
 008h 2 UInt16? ?
 00Ah 2 UInt16? ?
 00Ch ? (0x10h up to 0x30h) String Bounding name
 03Ch 8 Vector2 ?
 044h 4 Vector2 ?
 ? ? ? ? |
| --- |

**pts1 (Parts 1)**

...?

**pas1 (Pane Start)**

```

| 000h 4 Chunk ID "pas1"
 004h 4 Chunk Size (08h) |
| --- |

**pae1 (Pane End)**

```

| 000h 4 Chunk ID "pae1"
 004h 4 Chunk Size (08h) |
| --- |

**grp1 (Group)**

3 000h 4 Chunk ID "grp1"

```

| 004h 4 Chunk Size (1Ch+(0..N)*10h)
 008h 10h String Group Name
 018h 4 UInt32 Nr Pane References (=N) (can be zero)
 01Ch 10h*N String[N] Pane References |
| --- |

**grs1 (Group Start)**

```

| 000h 4 Chunk ID "grs1"
 004h 4 Chnunk Size (08h) |
| --- |

**gre1 (Group End)**

```

| 000h 4 Chunk ID "gre1"
 004h 4 Chunk Size (08h) |
| --- |

**Tools**

```

| Every File Explorer has limited support for viewing these |
| --- |

**See also**

[http://florian.nouwt.com/wiki/index.php/CLYT_%28File_Format%29](http://florian.nouwt.com/wiki/index.php/CLYT_%28File_Format%29)

[https://github.com/Gericom/EveryFileExplorer/tree/master/3DS/NintendoWare/LYT1](https://github.com/Gericom/EveryFileExplorer/tree/master/3DS/NintendoWare/LYT1)