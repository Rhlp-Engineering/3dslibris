# Filesarchivesarc

> Source: https://problemkaputt.de/gbatek.htm
> Section: Filesarchivesarc

3DS Files - Archive SARC 
| |
| --- |

 SARC archives contain only files, not folders. However, the filenames do usually contain slashes (eg. "bsnd/cursor.bcwav"), so software could split those names into folder+filename, if desired.
 SARC is used by 3DS eShop Applet, RomFS:\layout\ and RomFS:\sound\.
 
 **SARC Header**
 
```

| 000h 4 ID "SARC"
 004h 2 Header Size (14h)
 006h 2 Byte Order (FEFFh)
 008h 4 Total Filesize
 00Ch 4 Data Offset (aka Size of SARC+SFAT+SFNT)
 010h 2 Unknown/version? (always 0100h)
 012h 2 Unknown/reserved? (0)
 .. .. SFAT Chunk (see below) (Directory)
 .. .. SFNT Chunk (see below) (Filename strings)
 .. .. Data |
| --- |

**SFAT Chunk (File Allocation Table)**

```

| 000h 4 ID "SFAT"
 004h 2 Header Size (0Ch)
 006h 2 Number of FAT Entries
 008h 4 Filename Hash Multiplier (usually 65h)
 00Ch N*10h FAT Entries (10h bytes each) |
| --- |

FAT Entry Format:

```

| 000h 4 Filename Checksum (sorted, with smallest checksum in 1st FAT entry)
 004h 2 Filename Offset (at SFNT+8+Offset*4)
 006h 2 File Attribute? (always 0100h)
 008h 4 File Data Start (at Data+Start)
 00Ch 4 File Data End (at Data+End, aka Data+Start+Size) |
| --- |

The filename checksums are calculated as follows:

```

| sum=0, for i=0 to namelen-1, sum=(sum*hash_multiplier)+name[i], next i |
| --- |

**SFNT Chunk (Filename Table)**

```

| 00h 4 ID "SFNT"
 04h 2 Header Length (08h)
 06h 2 Unknown (padding?)
 08h .. Filenames (ASCII, terminated by zero, on 4-byte boundaries) |
| --- |

**File Data (at Offset specified in SARC[0Ch])**

Files are at least 4-byte aligned (some SARCs do reportedly use 80h-byte
alignment).

**Tools**

SARC Extractor/Creator with TAR-like command line flags. Can decompress a .zlib
SARC file (4-byte size header + ZLIB data).