# Filesvideoarchivecgfxheaderblocksdicts

> Source: https://problemkaputt.de/gbatek.htm
> Section: Filesvideoarchivecgfxheaderblocksdicts

3DS Files - Video Archive (CGFX Header, Blocks, DICTs) 
| |
| --- |

 **CGFX Header**
 
```

| 000h 4 ID "CGFX"
 004h 2 Byte Order (FEFFh)
 006h 2 CGFX header size (14h)
 008h 4 Revision (can be 05000000h) (or 01000000h=Old, incompatible)
 00Ch 4 Total Filesize (bytes)
 010h 4 Number of blocks (usually 1=DATA, or 2=DATA+IMAG)
 ... .. DATA Block (DICT's, CMDL/TXOB/etc., and DICT Symbols)
 ... .. IMAG Block (optional, eg. for TXOB bitmaps) |
| --- |

The DATA block contains various small data structures. It's more or less
required to load the whole DATA block to memory when parsing those structs.

The IMAG block isn't required in memory (or, when needed, one may want to store
its content directly in VRAM).

**DATA Block (for CGFX Version 05000000h)**

```

| 000h 4 ID "DATA"
 004h 4 DATA Block Size (including DICT,CMDL,etc.)
 008h 4+4 DICT 00h for CMDL Models (NumEntries, Offset)
 010h 4+4 DICT 01h for TXOB Textures (NumEntries, Offset)
 018h 4+4 DICT 02h for LUTS Look-up tables? (NumEntries, Offset)
 020h 4+4 DICT 03h for ? Materials (NumEntries, Offset)
 028h 4+4 DICT 04h for SHDR Shaders (+DVLB) (NumEntries, Offset)
 030h 4+4 DICT 05h for CCAM Cameras (NumEntries, Offset)
 038h 4+4 DICT 06h for CFLT/CHLT/CALT Lights (NumEntries, Offset)
 040h 4+4 DICT 07h for CFOG? Fogs (NumEntries, Offset)
 048h 4+4 DICT 08h for CENV Environments (NumEntries, Offset)
 050h 4+4 DICT 09h for CANM Skeleton animations (NumEntries, Offset)
 058h 4+4 DICT 0Ah for CANM Texture animations (NumEntries, Offset)
 060h 4+4 DICT 0Bh for ? Visibility animations (NumEntries, Offset)
 068h 4+4 DICT 0Ch for ? Camera animations (NumEntries, Offset)
 070h 4+4 DICT 0Dh for CANM Light animations (NumEntries, Offset)
 078h 4+4 DICT 0Eh for PEMT Emitters (NumEntries, Offset)
 Below entry doesn't exist in all files (ie. abort here if [80h]="DICT").
 (080h) 4+4 DICT 0Fh for PEMT Particles (NumEntries, Offset)
 ... .. DICT's and CMDL/TXOB/etc. and Symbols... |
| --- |

The above Offsets are self-relative pointers to the corresponding DICT.

Unused DICTs have NumEntries=0 and Offset=0 (ie. the Offset doesn't point
anywhere in that case).

**IMAG Block (if present)**

```

| 000h 4 ID "IMAG"
 004h 4 IMAG Block Size
 008h ... Data (eg. texture bitmaps, referenced from TXOB's) |
| --- |

**DATA Block (for CGFX Version 01000000h)**

3DS Activity Log does have some old CGFX file in RomFS:\simple.bcmdl, its the
data block lacks the "DATA" ID and Block Size, and the DICT's merely have
Offsets (without NumEntries).

```

| 000h 4*9 DICT's (nine Offsets) |
| --- |

The CMDL/MTOB/etc. blocks in that file are probably also incompatible with
newer CGFX files?

**DICT - Directory with Symbol/Offset for each file**

```

| 000h 4 ID "DICT"
 004h 4 DICT size (1Ch+N*10h)
 008h 4 Number of entries (same as NumEntries in DATA's DICT list)
 00Ch 10h Tree entry 0 (Patricia Tree Root entry)
 01Ch 10h*N Tree entry 1..N (Patricia Tree and actual Symbols/Offsets) |
| --- |

Patricia Tree Root entry:

```

| 000h 4 Patricia Tree First Bit-number (always FFFFFFFFh=dummy/last)
 002h 2 Patricia Tree First entry (1..N) (usually points to rightmost bit)
 004h 0Ah Patricia Tree Unused (zerofilled) |
| --- |

Patricia Tree entry 1..N format:

```

| 000h 4 Patricia Tree Bit-number to be tested (0=Bit0 of 1st char) ?
 004h 2 Patricia Tree Next entry when test=0 ?
 006h 2 Patricia Tree Next entry when test=1 ?
 008h 4 Offset to symbol (self-relative) --> ASCII name
 00Ch 4 Offset to object (self-relative) --> flags followed by "CMDL" etc? |
| --- |

The Patricia Tree is intended for searching Symbols by testing a few bits
(without comparing all characters). The tree does somewhat unforunately start
with the highest bit in rightmost character (so the search string must be
assumed to be zeropadded). Unknown when the final entry is reached... when the
bit-test number is same/bigger than previous bit-test?

Note on DICT names: There can be several root entries with same name (eg. 3DS
Download Play ExeFS:\banner has name "COMMON" used for CFGX_00h and CFGX_09h
and CFGX_0Ah root entries).