# Filessoundbankcbnkformat

> Source: https://problemkaputt.de/gbatek.htm
> Section: Filessoundbankcbnkformat

3DS Files - Sound Bank (CBNK Format) 
| |
| --- |

 **CBNK Header**
 
```

| 000h 4 ID "CBNK"
 004h 2 Byte Order (FEFFh)
 006h 2 Header Size (usually 20h)
 008h 4 Version? (can be 01000100h)
 00Ch 4 Total Filesize
 010h 4 Number of Blocks (usually 1 = INFO)
 014h 4+4+4 INFO Block (RefID=5800h, Offset from CBNK+0, Size) |
| --- |

**INFO Block**

```

| 000h 4 ID "INFO"
 004h 4 INFO Block Size (same as in CBNK Header)
 008h 4+4 Footer Entry (RefID=0100h, Offset from Info+8) ;Offs=Near end
 010h 4+4 List Entry (RefID=0101h, Offset from Info+8) ;Offs=10h
 018h 4 Number of following List Entries
 01Ch (4+4)*N List Entries (RefID=59xxh, Offs from INFO+18h or Offs=FFFFFFFFh)
 ... .. Entries for above list
 ... 4+8*X Footer Entry |
| --- |

**List Entry with RefID=5903h:**

```

| Nothing (these entries have Offset=FFFFFFFFh) |
| --- |

**List Entry with RefID=5900h:**

```

| 000h 4+4 1st Child Entry (RefID=6000h, Offset=8) ;\these are simply
 008h 4+4 2nd Child Entry (RefID=5901h, Offset=8) ; pointing to the next
 010h 4+4 3rd Child Entry (RefID=6000h, Offset=8) ; child, until reaching
 018h 4+4 4th Child Entry (RefID=5902h, Offset=8) ;/below data...
 020h 4 Bank Entry Number (0, 1, 2, etc.)
 024h 4 Unknown (21Fh) ;RefID ?
 028h 4 Unknown (03Ch or 48h) ;Offset? or Volume or so?
 02Ch 4 Unknown (7Fh)
 030h 4 Unknown (40h)
 034h 4 Unknown (3F800000h)
 038h 4 Unknown (0)
 03Ch 4 Unknown (20h)
 040h 4 Unknown (0)
 044h 4 Unknown (08h)
 048h 4 Unknown (7F7F7Fh)
 04Ch 4 Unknown (7Fh) |
| --- |

**Footer Entry with RefID=0101h:**

```

| 000h 4 Number of following 8-byte entries
 004h+N*8 4 Resource ID ? (05000005h, or 05000002h in another CBNK file)
 008h+N*8 4 Footer Index (0, 1, 2, 3, etc.) |
| --- |