# Filessoundwavearchivecwarformat

> Source: https://problemkaputt.de/gbatek.htm
> Section: Filessoundwavearchivecwarformat

3DS Files - Sound Wave Archive (CWAR Format) 
| |
| --- |

 CWAR files can contain CWAV files. The CWAR files are located inside of CSAR archives.
 Note: The CWARs are often left nameless (the CSAR may contain only one name entry in its STRG block (or none at all), despite of have multiple CWARs in its INFO block.
 
 **CWAR Header**
 
```

| 000h 4 ID "CWAR"
 004h 2 Byte Order (FEFFh)
 006h 2 Header Size (usually 40h)
 008h 4 Version? (can be 01000000h)
 00Ch 4 Total Filesize
 010h 4 Number of Blocks (usually 2 = INFO+FILE)
 014h 4+4+4 INFO Block (RefID=6800h, Offset from CWAR+0, Size)
 020h 4+4+4 FILE Block (RefID=6801h, Offset from CWAR+0, Size)
 02Ch 14h Padding to 20h-byte boundary (0) |
| --- |

**INFO Block**

```

| 000h 4 ID "INFO"
 004h 4 INFO Block Size (same as in CWAR Header)
 008h 4 Number of following List entries
 00Ch (4+4+4)*N List Entries (RefID=1F00h, Offset from FILE+8, Size)
 ... .. Padding to 20h-byte boundary (0) |
| --- |

**FILE Block**

```

| 000h 4 ID "FILE"
 004h 4 FILE Block Size (same as in CWAR Header)
 008h 18h Padding to 20h-byte boundary (0)
 020h ... Data Area (contains CWAV files) |
| --- |