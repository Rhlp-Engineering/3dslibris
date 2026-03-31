# Filessoundwsdcwsdformat

> Source: https://problemkaputt.de/gbatek.htm
> Section: Filessoundwsdcwsdformat

3DS Files - Sound WSD (CWSD Format) 
| |
| --- |

 **CWSD Header**
 
```

| 000h 4 ID "CWSD"
 004h 2 Byte Order (FEFFh)
 006h 2 Header Size (usually 20h)
 008h 4 Version? (can be 01000000h)
 00Ch 4 Total Filesize
 010h 4 Number of Blocks (usually 1 = INFO)
 014h 4+4+4 INFO Block (RefID=6800h, Offset from CWSD+0, Size) |
| --- |

**INFO Block**

```

| 000h 4 ID "INFO"
 004h 4 INFO Block Size (same as in CWSD Header)
 008h 4+4 ID List (RefID=0100h, Offset from INFO+8) ;Offs=10h
 010h 4+4 Crazy List (RefID=0101h, Offset from INFO+8) |
| --- |

ID List: (uh, are these Resource ID's pointing to CSAR entries):

```

| 000h 4 Nummber of below 8-byte entries
 004h (4+4)*N Resource ID and Index (ID=050000xxh, Index=0, 1, 2, etc.) |
| --- |

Crazy List:

```

| 000h 4 Nummber of below 8-byte entries
 004h (4+4)*N Crazy Entry N (RefID=4900h, Offset from Crazy List+0) |
| --- |

Crazy Entry:

```

| 000h 4+4 Crazy Part1 (RefID=4901h, Offset from Crazy Entry+0)
 008h 4+4 Crazy Part2 (RefID=0101h, Offset from Crazy Entry+0)
 010h 4+4 Crazy Part3 (RefID=0101h, Offset from Crazy Entry+0) |
| --- |

Crazy Part1:

```

| 000h 4 Unknown (303h)
 004h 4 Unknown (40h)
 008h 4 Unknown (3F800000h, 3FE66666h, or 3F4CCCCDh) ;Int? Float?
 00Ch 4 Unknown (14h)
 010h 4 Unknown (1Ch)
 014h 4 Unknown (27Fh)
 018h 4 Unknown (0)
 01Ch 4 Unknown (0)
 020h 4 Unknown (08h)
 024h 4 Unknown (7F7F7F7Fh)
 028h 4 Unknown (7Fh) |
| --- |

Crazy Part2:

```

| 000h 4 Unknown (1) ;Num entries?
 004h 4+4 Child Block (RefID=4903h, Offset from 000h+0) ;Offs=0Ch
 00Ch 4 Unknown (0)
 010h 4 Unknown (08h)
 014h 4 Unknown (1) ;Num entries?
 018h 4+4 Child Block (RefID=4904h, Offset from 014h+0) ;Offs=0Ch
 020h 4 Unknown (0)
 024h 4 Unknown (0)
 028h 4 Unknown (0)
 02Ch 4 Unknown (0) |
| --- |

Crazy Part3:

```

| 000h 4 Unknown (1) ;Num entries?
 004h 4+4 Child Block (RefID=4902h, Offset from Crazy Part3+0) ;Offs=0Ch
 00Ch 4 Unknown (0 or 2)
 010h 4 Unknown (0) |
| --- |