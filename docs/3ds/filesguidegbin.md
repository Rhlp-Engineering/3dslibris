# Filesguidegbin

> Source: https://problemkaputt.de/gbatek.htm
> Section: Filesguidegbin

3DS Files - Guide (GBIN) 
| |
| --- |

 GBIN Files are found in 3DS Camera & 3DS Sound (RomFS:\res\Guide*.gbin).
 The files contain several "T_xxx" and "D_xxx_y" strings, bundled with unknown small numbers.
 Purpose is unknown, maybe some Tree structure, or a list of Chapters (T) and Pages (D).
 The files contain no real data, so they do probably refer to whatever other files, ie. translating names to file indices and/or assigning attributes.
 
 **GBIN Format**
 
```

| 000h 4 ID "GBIN"
 004h 4 Number of GUID entries
 008h .. GUID entries |
| --- |

GUID entry format:

```

| 000h 4 ID "GUID"
 004h 4 Number of D-entries
 008h 4 Unknown (0 or 1 or 2)
 00Ch 4 Unknown (0 or 1 or 2)
 010h 4 Unknown (0..0Fh?)
 014h 0Ch Unknown (0)
 020h 10h T-Name ("T_001" and up, zeropadded)
 030h N*14h D-Entries |
| --- |

D-Entry Format:

```

| 000h 10h D-NAME ("D_001_0" and up, zeropadded)
 010h 4 Unknown (0..04h?) |
| --- |