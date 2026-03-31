# Filesvideoshadersdvlbanddvobformat

> Source: https://problemkaputt.de/gbatek.htm
> Section: Filesvideoshadersdvlbanddvobformat

3DS Files - Video Shaders (DVLB and DVOB Format) 
| |
| --- |

 There are two shader file formats:
 
```

| DVLB - Vertex Shader Library (can contain one or more shaders) (common/used)
 DVOB - Vertex Shader Object (single shader) (unknown if/where that's used?) |
| --- |

Shaders are usually stored in folder RomFS:\shaders, with extensions

```

| .shbin Shader Binary in DVLB format (most common)
 .?? Shader Binary in DVOB format (unknown if/where that's used?)
 .bcsdr Shader Binary in DVLB format stored inside of a CGFX container
 .csdr ASCII, with shader binary(ies?) encoded in BASE64 (eg. in AR Games) |
| --- |

**DVLB Format (Vertex Shader Library) (can contain one or more shaders)**

```

| 000h 4 ID "DVLB" (Vertex Shader Library)
 004h 4 N = number of DVLEs in SHBIN
 008h 4*N DVLE offset table (offsets to start of the DVLB(???) sections) |
| --- |

There is one DVLP block, containing the binary opcode/opdesc data for all
shaders (located right after the above header, ie. at offset 008h+4*N).

```

| 000h 4 ID "DVLP" (Vertex Shader Library Program Opcodes/Opdescs)
 004h 2 Type/Version? (can be 0C02h)
 006h 2 Unknown (can be 0000h)
 008h 4 Opcode Table Offset (from DVLP start) (28h)
 00Ch 4 Opcode Table Size (in 4-byte units)
 010h 4 Opdesc Table Offset (from DVLP start)
 014h 4 Opdesc Table Size (in 8-byte units)
 018h 4 Line Number Table Offset ? (from DVLP start)
 01Ch 4 Line Number Table Size ? (usually zero)
 020h 4 Filename Symbol Table Offset (from DVLP start)
 024h 4 Filename Symbol Table Size (in 1-byte units)
 028h .. Data for above Opcodes, Opdescs, etc. |
| --- |

There can be one or more DVLE blocks, with information about entrypoint and
parameters for each shader (located at Header[008h+index*4]).

```

| 000h 4 ID "DVLE" (Vertex Shader Library Function Entrypoints/Params)
 004h 2 Type/Version? (can be 0C02h)
 006h 1 Shader type (0=Vertex shader, 1=Geometry shader, other=?)
 008h 4 Opcode Entrypoint Address (in words, in memory) (main)
 00Ch 4 Opcode End Address (in words, in memory) (endmain)
 010h 4 Unknown (can be 001F0001h)
 014h 4 Unknown (can be 00000000h)
 018h 4 Constant Table Offset (relative to DVLE start) (eg. 40h)
 01Ch 4 Constant Table Size (in 14h-byte units)
 020h 4 Label Table Offset (relative to DVLE start)
 024h 4 Label Table Size (in 10h-byte units)
 028h 4 Output Register Table Offset (relative to DVLE start)
 02Ch 4 Output Register Table Size (in 8-byte units)
 030h 4 Input Register Table Offset (relative to DVLE start)
 034h 4 Input Register Table Size (in 8-byte units)
 038h 4 Symbol Table Offset (relative to DVLE start)
 03Ch 4 Symbol Table Size (in 1-byte units)
 040h .. Data for above Constants, Labels, etc. |
| --- |

**DVOJ (Single Vertex Shader Function Object)**

```

| 000h 4 ID "DVOJ"
 004h 4 Unknown (seems to be related to the DVLE shader type) (uh?)
 008h 4 Unknown
 00Ch 4 Unknown (usually FFFFFFFFh)
 010h 4 Constant table Offset (from DVOJ start)
 014h 4 Constant table Size (in 14h-byte units)
 018h 4 Label table Offset (from DVOJ start)
 01Ch 4 Label table Size (in 10h-byte units)
 020h 4 Opcode Table Offset (from DVOJ start)
 024h 4 Opcode Table Size (in 4-byte units)
 028h 4 Opdesc Table Offset (relative to DVLP??? start)
 02Ch 4 Opdesc Table Size (in 8-byte units)
 030h 4 Line Number Table Offset ? (from DVOJ start)
 034h 4 Line Number Table Size ? (in 8-byte units)
 038h 4 Relocation Table Offset (from DVOJ start)
 03Ch 4 Relocation Table Size (in 0Ch-byte units)
 040h 4 Output register table Offset (from DVOJ start)
 044h 4 Output register table Size (in 8-byte units)
 048h 4 Input Register table Offset (from DVOJ start)
 04Ch 4 Input Register table Size (in 8-byte units)
 050h 4 Symbol table Offset (from DVOJ start)
 054h 4 Symbol table Size (in 1-byte units) |
| --- |

Entrypoint/End Address seem to be implied to be begin/end of the opcode area?

```

| _______________________________ Table Formats ________________________________ |
| --- |

**Opcode Table (with 4-byte entries)**

```

| 000h 4 Shader Opcode |
| --- |

**Opdesc Table (with 8-byte entries)**

```

| 000h 4 Shader Opdesc (bit31: set to 1 in most (but not all) entries, why?)
 004h 4 Unknown, Flags? (usually 0Fh, 07h, or 03h) |
| --- |

**Constant Table (with 14h-byte entries)**

```

| Bool Entries (with 00h in first byte):
 000h 1 Entry Type (Must be 00h for Bool Uniform)
 002h 1 Bool Uniform ID (whatever, maybe register number, 0..0Fh?)
 004h 1 Value (boolean, ie. 00h or 01h ?)
 005h 0Fh Unused
 Integer Uniform Vector Entries (with 01h in first byte):
 000h 1 Entry Type (Must be 01h for Integer Uniform)
 002h 1 Integer Uniform vector ID (whatever, maybe register number, 0..3?)
 004h 1 x (u8) ;Loop count-1
 005h 1 y (u8) ;Loop starting index
 006h 1 z (u8) ;Loop index step
 007h 1 w (u8) ;uh, but integer uniforms have only 3 elements... x,y,z?
 008h 0Ch Unused
 Float Uniform Vector Entries (with 02h in first byte):
 000h 1 Entry Type (Must be 02h for Float Uniform)
 002h 1 Float Uniform vector ID (00h..5Fh = c0..c95)
 004h 4 x (float24) ;24bit float values (paddded with bit24-31=zero)
 008h 4 y (float24) ;(weirdly, these 4x24bit must be manually merged
 00Ch 4 z (float24) ;into 3x32bit before sending them to GPU)
 010h 4 w (float24) |
| --- |

**Output Register Table (with 8-byte entries)**

```

| 000h 2 Output Type (see below) (only lower 4bit used?)
 002h 2 Output Register ID (only lower 4bit used?)
 004h 2 Output Attribute Component Mask (eg. 5=xz) (only lower 4bit used?)
 006h 2 Unknown (unknown?) |
| --- |

Output Types:

```

| 00h = result.position
 01h = result.normalquat
 02h = result.color
 03h = result.texcoord0
 04h = result.texcoord0w
 05h = result.texcoord1
 06h = result.texcoord2
 07h = ?
 08h = result.view |
| --- |

**Input Register Table (with 8-byte entries)**

```

| 000h 4 Input Parameter Name Offset (in Symbol Table) (eg. "vVtxData")
 004h 2 Input Register Index Start (see below)
 006h 2 Input Register Index End (end=start+num-1) |
| --- |

Input Register Indices:

```

| 00h..0Fh = v0..v15 ;Vertex input registers (per vertex)
 10h..6Fh = c0..c95 ;Float uniform registers (per function)
 70h..73h = i0..i3 ;Integer uniform registers (per function)
 78h..87h = b0..b15 ;Bool uniform registers (per function) |
| --- |

**Label Table (with 10h-byte entries)**

```

| 000h 1 Label ID (increasing IDs, starting at 00h) (unknown purpose)
 001h 3 Unknown (000100h)
 004h 4 Opcode Address (in words, in memory)
 008h 4 Unknown (FFFFFFFFh)
 00Ch 4 Label Name Offset (in Symbol Table) (eg. "main", and "endmain") |
| --- |

**Relocation Table (with 0Ch-byte entries) (exists in DVOB files only)**

```

| 000h 4 Opcode Address (in words, in memory) (excluding END, NOP, etc.)
 004h 4 Unknown ;\probably some kind of opcode/opdesc relocation info?
 008h 4 Unknown ;/(for merging several DVOB files in shader memory) |
| --- |

**Line Number Table (with 8-byte entries, one for each Opcode Table entry)**

```

| 000h 4 Source Filename Offset (in Symbol Table) (eg. "font_XxxShader.vsh")
 004h 4 Source Line number? (for the corresponding Opcode Table entry) |
| --- |

**Symbol Table (with ASCII strings, each terminated by 00h)**

DVLB files have separate Symbol Tables in each DVLP/DVLE chunk:

```

| Symbols in DVLP chunk: Source Filenames
 Symbols in DVLE chunk(s): Labels and Names for Input registers |
| --- |

DVOB files have all those symbols in a single Symbol Table.