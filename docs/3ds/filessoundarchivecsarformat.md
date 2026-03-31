# Filessoundarchivecsarformat

> Source: https://problemkaputt.de/gbatek.htm
> Section: Filessoundarchivecsarformat

3DS Files - Sound Archive (CSAR Format) 
| |
| --- |

 CSAR files are usually found in "RomFS:\sound\" folder, with extension ".bcsar" (Binary CTR Sound Archive).
 The CSAR archive contains CSEQ, CWSD, CBNK, CWAR files (and optionally CGRP). Whereof, the CWAR file contains CWAV files.
 
 **CSAR Header**
 
```

| 000h 4 ID "CSAR"
 004h 2 Byte Order (FEFFh)
 006h 2 Header Size (usually 40h)
 008h 4 Version (usually 02000000h) (02030200h in Miiverse-posting)
 00Ch 4 Total Filesize
 010h 4 Number of Blocks (usually 3 = STRG+INFO+FILE)
 014h 4+4+4 STRG Block (RefID=2000h, Offset from CSAR+0, Size)
 020h 4+4+4 INFO Block (RefID=2001h, Offset from CSAR+0, Size)
 02Ch 4+4+4 FILE Block (RefID=2002h, Offset from CSAR+0, Size)
 038h 8 Padding to 20h-byte boundary (0) |
| --- |

```

| _________________________________ STRG Block _________________________________ |
| --- |

**String Block Header**

```

| 000h 4 ID "STRG"
 004h 4 STRG Block Size (same as in CSAR header)
 008h 4+4 Filename String List (RefID=2400h, Offset from STRG+8)
 010h 4+4 Filename Search Tree (RefID=2401h, Offset from STRG+8) |
| --- |

**Filename String List**

```

| 000h 4 Number of Filenames (eg. 40h)
 004h (4+4+4)*N Filenames (RefID=1F01h, Offset from this Table, Size)
 ... .. Filename strings (uppercase ASCII, terminated by 00h) |
| --- |

**Filename Search Tree (Patricia Tree)**

This is intended for fast file name lookup. For example, if the String List
contains only two entries with name "Fuck" and "Funeral", then one could find
the desired string with a single bit-test in the 3rd char (to avoid false
matches for strings that do not exist in the list, one could do a string
compare on the final result).

```

| 000h 4 First Tree index for search (for testing the leftmost char)
 004h 4 Number of Tree entries (usually NumNames*2-1)
 008h+N*14h 2 Entry Type (0=Test, 1=Name)
 00Ah+N*14h 2 Bit number to be tested (0=Bit7 of 1st char) ;\FFh-filled
 00Ch+N*14h 4 Next Tree index when test Result=0? ; in Name entries
 010h+N*14h 4 Next Tree index when test Result=1? ;/
 014h+N*14h 4 Name Index in STRG Block (0..NumNames-1) ;\FFh-filled
 018h+N*14h 4 Resource ID in INFO Block (NNxxxxxxh) ;/in Test entries |
| --- |

To view the whole directory, simply ignore the Test entries, and extract the
Name+ID pairs from the Name entries.

**Resource IDs**

The Resource IDs refer to entries in the INFO Block (the upper 8bit indicate a
List number, and lower 24bit contain the entry number in that list):

```

| ID List File Types Common Name prefix
 01xxxxxxh = Audio List (CSEQ, CWSD files) ;name SE,BGM
 02xxxxxxh = Set List (folders for above Audio entries) ;name WSDSET,SEQSET
 03xxxxxxh = Bank List (CBNK files) ;name BANK
 04xxxxxxh = Player List (...whatever?) ;name PLAYER
 05xxxxxxh = Wav Archive (CWAR files) <-- unless nameless ;name WARC ;or none
 06xxxxxxh = Group List (CGRP files) <-- if any ;name GROUP |
| --- |

Groups exist only in some files. Wav Archives do exist in all files, but they
are often completely omitted in the string tree (or have only one of them
listed in the string tree).

```

| _________________________________ INFO Block _________________________________ |
| --- |

**Info Block Header**

```

| 000h 4 ID "INFO"
 004h 4 INFO Block Size (same as in CSAR header)
 008h 4+4 Audio List (RefID=2100h, Offset relative to INFO+8) CSEQ,CWSD
 010h 4+4 Set List (RefID=2104h, Offset relative to INFO+8) Folder?
 018h 4+4 Bank List (RefID=2101h, Offset relative to INFO+8) CBNK
 020h 4+4 WavArchive List (RefID=2103h, Offset relative to INFO+8) CWAR
 028h 4+4 Group List (RefID=2105h, Offset relative to INFO+8) CGRP
 030h 4+4 Player List (RefID=2102h, Offset relative to INFO+8) ?
 038h 4+4 File List (RefID=2106h, Offset relative to INFO+8) Files
 040h 4+4 Final Entry (RefID=220Bh, Offset relative to INFO+8) ?
 ... .. Lists+Entries (at Offsets from above INFO header)
 ... 14h Final Entry (at Offset from above INFO header)
 ... .. Padding to 20h-byte boundary (0) |
| --- |

**Audio List (RefID=2100h)**

```

| 000h 4 Number of List entries
 004h (4+4)*N Audio Entries (RefID=2200h, Offset from List+0) |
| --- |

**Audio Entry (RefID=2200h) (eg. 2Fh entries) (60h bytes per entry)**

```

| 000h 4 Index in File List (File CSEQ or CWSD) (eg. 0,0,1,2,2,3,etc.)
 (above does often use the same file index for 1-4 audio entries)
 004h 4 Sound player Resource ID (040000xxh)
 008h 4 Volume or so (10h,12h,14h,18h,1Ch,1Eh,28h,38h,40h,48h,60h,64h,7Fh)
 00Ch 4+4 Extended entry (RefID=2202h/2203h, Offset=44h, relative to Audio)
 014h 4 Unknown (80020107h)
 018h 4 Name Index in STRG Block (Names SE_xx,JGL_xx,BGM_xx) (0,1,2,etc.)
 01Ch 4 Unknown (0)
 020h 4 Unknown (40h)
 024h 4 Unknown (30h)
 028h 4 Unknown (0 or 1)
 02Ch 4 Unknown (0=norm, or FFFFh,10000000h,01FF0000h,FE000000h,FFE6EE99h)
 030h 4 Unknown (0Fh)
 034h 4 Unknown (3F000000h)
 038h 4 Unknown (1)
 03Ch 4 Unknown (0)
 040h 4 Unknown (0 or 1) |
| --- |

Extended Entry RefID=2202h: (used for CWSD files)

```

| 044h 4 Unknown (0, 1, 2, 3) ;often same for 1-2 entries
 048h 4 Unknown (1)
 04Ch 4 Unknown (101h)
 050h 4 Unknown (40h)
 054h 4 Unknown (14h)
 058h 4 Unknown (27Fh)
 05Ch 4 Unknown (0) |
| --- |

Extended Entry RefID=2203h: (used for CSEQ files)

```

| 044h 4 Unknown (100h)
 048h 4 Unknown (18h)
 04Ch 4 Unknown (1, 3, 7, 0Fh, 1Fh, 3Fh, or FFFFh) ;some bitmask?
 050h 4 Unknown (3)
 054h 4 Unknown (3, 30h, 4Ch, 102h, 26Ah, 283h, ..., 3C2h, 3F7h)
 058h 4 Unknown (40h)
 05Ch 4 Number of below 4-byte entries (usually 1, sometimes 2)
 060h 4*N Resource ID (0300000xh) ;often same for 1-N entries |
| --- |

**Set List (RefID=2104h)**

```

| 000h 4 Number of List entries
 004h (4+4)*N Set Entries (RefID=2204h, Offset from List+0) |
| --- |

**Set Entry (RefID=2204h) (eg. 5 entries) (38h bytes per entry)**

This seems to have no file assigned, instead it's assigned to Sound Resource
IDs (for above Audio Entries), in so far, the Set could be considered being a
Folder instead of a File.

```

| 000h 4 First Sound Resource ID in this sequence set (eg. 010000xxh)
 004h 4 Last Sound Resource ID in this sequence set (eg. 010000xxh)
 008h 4+4 Extended entry (RefID=0100h, Offset=20h, relative to Set Entry+0)
 010h 4+4 Extended entry (RefID=2205h, Offset=28h, relative to Set Entry+0)
 Extended entry (RefID=0000h, Offs=FFFFFFFFh) ;alternately to 2205h
 018h 4 Unknown (1)
 01Ch 4 Name Index in STRG Block (Names SEQSET_xxx or WSDSET_xxx) |
| --- |

Extended Entry RefID=0100h:

```

| 020h 4 Number of below 4-byte values (usually 1, sometimes 2)
 024h 4*n Unknown (2, 3, 4, 5, or 6) |
| --- |

Extended Entry RefID=2205h: (if present)

```

| .. 4 Unknown (100h) ;\exists only for WSDSET_xx,
 .. 4 Unknown (0Ch) ; not for SEQSET_xx
 .. 4 Unknown (0) ;
 .. 4 Unknown (0) ;/ |
| --- |

**Bank List (RefID=2101h)**

```

| 000h 4 Number of List entries
 004h (4+4)*N Bank Entries (RefID=2206h, Offset from List+0) |
| --- |

**Bank Entry (RefID=2206h) (eg. 5 entries) (18h bytes per entry)**

```

| 000h 4 Index in File List (File CBNK)
 004h 4+4 Extended entry (RefID=0100h, Offset=14h, relative to Bank Entry+0)
 00Ch 4 Unknown (1)
 010h 4 Name Index in STRG Block (Name BANK_xxx) |
| --- |

Extended Entry RefID=0100h:

```

| 014h 4 Number of below 4-byte entries (can be 0=None)
 018h 4*n Resource ID (05000000h) (if any, ie. if above is nonzero) |
| --- |

Above list contains one entry in AR Games, which also has CGRP, maybe that's
related?

**WavArchive List (RefID=2103h)**

```

| 000h 4 Number of List entries
 004h (4+4)*N WAV Archive Entries (RefID=2207h, Offset from List+0) |
| --- |

**WavArchive Entry (RefID=2207h) (eg. 7 entries) (0Ch bytes per entry)**

```

| 000h 4 Index in File List (File CWAR)
 004h 8 Unknown (0)
 008h 8 Unknown (0 or 1) ;set when NEXT file is CGRP? or unrelated? |
| --- |

**Group List (RefID=2105h)**

```

| 000h 4 Number of List entries (can be 0 if there aren't any groups)
 004h (4+4)*N Group Entries (RefID=2208h, Offset from List+0) |
| --- |

**Group Entry (RefID=2208h) (exists in ArGames and MiiMaker, for example)**

```

| 000h 4 Index in File List (File CGRP)
 004h 4 Unknown (1)
 008h 4 Name Index in STRG Block (Name GROUP_xxx) |
| --- |

**Player List (RefID=2102h)**

```

| 000h 4 Number of List entries
 004h (4+4)*N Player Entries (RefID=2209h, Offset from List+0) |
| --- |

**Player Entry (RefID=2209h) (eg. 7 entries) (10h bytes per entry)**

```

| 000h 4 Varies 2,8,1,1,4,1,2 ;Index in File List? (would be CSEQ/CSTM)
 004h 4 Unknown (3)
 008h 4 Name Index in STRG Block (Name PLAYER_xxx)
 00Ch 4 Unknown (0) |
| --- |

**File List (RefID=2106h)**

```

| 000h 4 Number of List entries
 004h (4+4)*N File Entries (RefID=220Ah, Offset from List+0) |
| --- |

**File Entry (RefID=220Ah) (eg. only 13h entries, despite of 40h filenames)**

```

| 000h 4+4 ExtEntry (RefID=220Ch/220Dh, Offset=0Ch, relative to File Entry)
 008h 4 Zero |
| --- |

ExtEntry RefID=220Ch: (eg. CSEQ file, in internal FILE section):

```

| 00Ch 4+4+4 File (RefID=1F00h/0000h, Offset relative to FILE+8, Size)
 Note: Above RefID=1F00h is used for Normal Files, whilst Group Files are
 instead using RefID=0000h (the Offset/Size do exist nonetheless, pointing
 to the CGRP file). |
| --- |

ExtEntry RefID=220Dh: (eg. CSTM file, in external .bcstm file):

```

| 00Ch ... External Filename (eg. "stream/xxx.bcstm",00h) |
| --- |

**Final Entry (RefID=220Bh)**

```

| 000h 2 Unknown (18h, 20h, 40h) x
 002h 2 Unknown (30h, 40h, 60h) x
 004h 2 Unknown (00h, 04h)
 006h 2 Unknown (00h, 04h)
 008h 2 Unknown (00h, 08h)
 00Ah 2 Unknown (00h, 40h) x
 00Ch 2 Unknown (00h, 04h, 40h) x
 00Eh 2 Unknown (0, 1) (1 in Miiverse-posting)
 010h 2 Unknown (0)
 012h 2 Unknown (0) |
| --- |

```

| _________________________________ FILE Block _________________________________ |
| --- |

**File Block Header**

```

| 000h 4 ID "FILE"
 004h 4 FILE Block Size (same as in CSAR header)
 008h 18h Padding to 20h-byte boundary (0)
 020h ... Data Area (contains CSEQ, CWSD, CBNK, CWAR, CGRP files) |
| --- |

Note: Some of the files may also contain chunks with name INFO or FILE, that
chunks are unrelated to the INFO/FILE chunks in the CSAR header).

**Tools**

- vgmtoolbox's Advanced Cutter/Offset Finder (extract BCWAVs without filenames)

- 3DSUSoundArchiveTool (reference implementation of CSAR extraction)