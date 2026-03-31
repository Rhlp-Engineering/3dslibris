# Filessoundseqcseqformat

> Source: https://problemkaputt.de/gbatek.htm
> Section: Filessoundseqcseqformat

3DS Files - Sound SEQ (CSEQ Format) 
| |
| --- |

 CSEQ is usually found inside of CSAR (but can be also a standalone file, eg. in 3DS Camera Applet, RomFS:\sound\csnd.LZ).
 
 **CSEQ Header**
 
```

| 000h 4 ID "CSEQ"
 004h 2 Byte Order (FEFFh)
 006h 2 Header Size (usually 40h)
 008h 4 Version? (can be 01000000h)
 00Ch 4 Total Filesize
 010h 4 Number of Blocks (usually 2 = DATA+LABL)
 014h 4+4+4 DATA Block (RefID=5000h, Offset from CSEQ+0, Size)
 020h 4+4+4 LABL Block (RefID=5001h, Offset from CSEQ+0, Size)
 02Ch 14h Padding to 20h-byte boundary (0) |
| --- |

**DATA Block**

```

| 000h 4 ID "DATA"
 004h 4 DATA Block Size (same as in CSEQ Header)
 008h ... Data (addressed via Offsets in LABL block)
 ... .. Padding to 20h-byte boundary (0) |
| --- |

The content of the Data area is unknown, probably some midi-like notes?

**LABL Block**

```

| 000h 4 ID "LABL"
 004h 4 LABL Block Size (same as in CSEQ Header)
 008h 4 Number of following List entries (eg. 26h)
 00Ch (4+4)*N List Entries (RefID=5100h, Offset from LABL+8)
 ... .. Padding to 20h-byte boundary (0) |
| --- |

List Entry Format:

```

| 000h (4+4)*N Data Entry (RefID=1F00h, Offset from DATA+8)
 008h 4 String Length, excluding ending 00h
 00Ch LEN+1 String (ASCII, plus ending 00h) (eg. "SMF_TWL_function_xxx")
 ... .. Padding to 4-byte boundary (0) |
| --- |