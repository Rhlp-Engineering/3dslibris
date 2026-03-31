# Filesvideoarchivecgfx00Hcmdlmodels

> Source: https://problemkaputt.de/gbatek.htm
> Section: Filesvideoarchivecgfx00Hcmdlmodels

3DS Files - Video Archive (CGFX_00h - CMDL Models) 
| |
| --- |

 CMDL is found in most CGFX files.
 
 **CMDL - 3D Model**
 
```

| 000h 4 Flags (bit7: hasSkeletonSobj) (eg. 40000092h)
 004h 4 ID "CMDL"
 008h 4 Revision (07000000h)
 00Ch 4 Name Offset (self-relative, same name as in parent DICT)
 010h 4 Unknown (0)
 014h 4 Unknown (0)
 018h 4 Unknown (1)
 01Ch 4 Unknown (1)
 020h 4 Unknown (0)
 024h 4 Unknown (0)
 028h 4+4 DICT for Animation Types (NumEntries, Offset)
 030h 0Ch Scale (3 float32: x, y, z) ;-usually (1,1,1)
 03Ch 0Ch Rotation (3 float32) ;-usually (0,0,0)
 048h 0Ch Translation (3 float32) ;-usually (0,0,0)
 054h 30h Local Matrix (4x3 float32) ;-usually unit matrix
 084h 30h World Matrix (4x3 float32) ;-usually unit matrix
 0B4h 4+4 Vertex Info SOBJ List (Meshes) (NumEntries, Offset)
 0BCh 4+4 DICT for MTOB (Materials) (NumEntries, Offset)
 0C4h 4+4 Vertex Info SOBJ List (Shapes) (NumEntries, Offset)
 0CCh 4+4 DICT for Unknown (Mesh Nodes) (NumEntries, Offset)
 0D4h 4 Unknown (1)
 0D8h 4 Unknown (0)
 0DCh 4 Unknown (0)
 0E0h 4 Skeleton Info SOBJ offset (self-relative) (present if flag bit7=1) |
| --- |

A CMDL section refers to outside data; it can not be considered separately from
the rest of the CGFX file. The second DICT in the CMDL section contains offsets
to MTOB objects.

**SOBJ List, at CMDL([B8h]+B8h)**

```

| 000h 4*N Vertex Info SOBJ self-relative offset list |
| --- |

**MTOB - Materials - some kind of Texture Info, linked to TXOB**

```

| 000h 4 Flags (eg. 08000000h)
 004h 4 ID "MTOB"
 008h 4 Revision (06000000h)
 00Ch 4 Name Offset (self-relative, same name as in parent DICT)
 010h 4+4 Unknown (0,0)
 018h 4+4 Unknown (0,0)
 020h 4 Unknown (1)
 024h 4+4 Unknown (0,0)
 02Ch 4+4 Unknown (0,0)
 034h ... Vectors/Matrices? (lots of float32's)
 0D8h .. Unknown (00FFFFFFh, FFFFFFFFh, FFFFFFFFh, 00000000h)
 0E8h .. Unknown (FFF2FFFFh, FF99CCCCh, FF000000h, FF000000h)
 0F8h .. Unknown (FF000000h, FF000000h, 00000000h, 00000000h)
 108h 4 Unknown (3)
 ... ... ?
 .. .. ?
 324h 40h TXOB (Flags,"TXOB",etc.) (40h bytes)
 364h .. SHDR (Flags,"SHDR",etc.) (12Ch bytes? or less?) |
| --- |

Then somewhere inside/after MTOB comes something called TXOB with 40h-byte size
and SHDR (which are probably something else than the TXOB's and SHDR's in the
CGFX's "root directory") (though they might be meant to <refer> to those
root entries).

```

| 000h 4 Flags (eg. 20000004h)
 004h 4 ID "TXOB"
 008h 4 Revision (05000000h)
 00Ch 4 Name Offset (self-relative) (eg. points to empty string)
 010h 4+4 Unknown (0,0)
 018h 4 Name Offset (self-relative) (eg. points to "COMMON3",0)
 01Ch 4+4 Unknown (0,0)
 024h 4 Unknown (FFFFFF90h)
 028h 4 Unknown (1)
 02Ch 4+4+4 Unknown (0,0,0)
 038h 4 Unknown (3F800000h) (float32)
 03Ch 4 Unknown (0)
 -------------------------
 040h 4 Flags (00000001h) for below shader stuff?
 044h 4 ID "SHDR"
 048h 4 Revision (05000000h)
 04Ch 4 Name Offset (self-relative) (eg. points to ... what?)
 050h 4+4 Unknown (0,0)
 058h 4 Name Offset (self-relative) (eg. points to ... what?)
 05Ch 4 Unknown ?
 ... ... ?
 .. .. |
| --- |

**SOBJ**

SOBJ structures can be used to describe 3D objects that are part of the model.
If such is the case then they will follow this structure:

```

| 000h 4 Flags (bit4: model, bit1: skeleton)
 004h 4 ID "SOBJ"
 008h 4 ?
 00Ch 4 Unknown symbol offset (self-relative)
 010h 0Ch ?
 01Ch 4 Offset (self-relative) to Unknown1 (array of floats?)
 020h 0Ch Mesh position vector (X/Y/Z floats)
 02Ch 4+4 Face Group List (NumEntries, Offset) (list contains offsets)
 034h 4 ?
 038h 4+4 Vertex Group List (NumEntries, Offset) (list contains offsets)
 040h 4 Unknown offset (self-relative) ? |
| --- |

Face groups:

```

| 000h 4+4 Bone Group ID List (NumEntries, Offset) (list contains 32bit IDs)
 008h 4 ?
 00Ch 4+4 Unknown2 List (NumEntries, Offset) (list contains offsets) |
| --- |

Unknown2:

```

| 000h 4+4 Face Group Descriptor List (NumEntries, Offset) (contains offsets)
 008h 4+4 Unknown3 List (NumEntries, Offset) (contains 32bit's)
 010h 8 ? |
| --- |

Face array descriptor:

```

| 000h 4 Flags (bit 1: vertex index format: 0=byte, 1=short)
 004h 4 ?
 008h 4+4 Vertex Index Array (SizeInBytes, Offset) |
| --- |

Vertex groups come in a number of different formats. Typically the first vertex
group entry is of format 40000002h and contains the actual vertex array.

Vertex group format 40000002h:

```

| 000h 4 Flags (40000002h)
 004h 4 ?
 008h 4 ?
 00Ch 4 ?
 010h 4 ?
 014h 4+4 Vertex Array (SizeInBytes, Offset)
 01Ch 4 ?
 020h 4 ?
 024h 4 Vertex stride/size in bytes (see below)
 028h 4 Unknown3 count
 02Ch 4 Offset (self-relative) to component declaration offset array |
| --- |

Each mesh's primary vertex group contains an array of vertex component
declaration objects, defining the order and parameters for each of a vertex's
components.

Vertex component declaration:

```

| 000h 4 Flags (40000001h)
 004h 4 Vertex component type (see below)
 008h 4 ?
 00Ch 4 ?
 010h 4 ?
 014h 4 ?
 018h 4 ?
 01Ch 4 ?
 020h 4 ?
 024h 1 Vertex component data type (see below)
 025h 1 ?
 026h 1 ?
 027h 1 ?
 028h 4 Number of values in this component (eg. XYZ=3, UV=2)
 02Ch 4 Multiplier for this component's values (float)
 030h 4 Position of this component within vertex stride |
| --- |

Vertex formats with bone data support multiple bone assignment. In this case,
the sum of all bone weights is 64h.

```

| Vertex component types:
 00h=Position
 01h=Normal
 02h=? (unobserved)
 03h=Color
 04h=UV0
 05h=UV1
 06h=? (unobserved, possibly UV2)
 07h=Weight
 08h=Index
 Vertex component data types:
 00h=sbyte
 01h=byte
 02h=short
 03h=? (unobserved, possibly ushort)
 04h=? (unobserved, possibly int)
 05h=? (unobserved, possibly uint)
 06h=float |
| --- |

Vertex components are stored as one of the above data types, and the vertex
component declaration contains a multiplier that adapts the values to the float
version which the game will use. For example, color RGBA values are stored as
bytes, and the multiplier converts them from 0-255 to 0-1.0, and position
components using short values are normalized via the multiplier to take
advantage of the entire short value range.