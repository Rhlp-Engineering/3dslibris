# Filesncchromfs

> Source: https://problemkaputt.de/gbatek.htm
> Section: Filesncchromfs

3DS Files - NCCH RomFS 
| |
| --- |

 **Overall RomFS file structure**
 
```

| 00000h 5Ch RomFS Header (see below) ;Header
 0005Ch 4 Padding to 20h-byte boundary (0)
 00060h 20h*N SHA256's on the 1000h-byte block(s) at xx000h ;Master Hash
 ... .. Padding to block-byte boundary (0)
 01000h .... RomFS Directory/Files (see below) ;Level 3
 ... .. Padding to block-byte boundary (0)
 xx000h 20h*NN SHA256's on each 1000h-byte block at yy000h and up;Level 1
 ... .. Padding to block-byte boundary (0)
 yy000h 20h*NNN SHA256's on each 1000h-byte block at 1000h and up ;Level 2 |
| --- |

**RomFS Header (5Ch bytes)**

```

| 000h 8 ID "IVFC",00h,00h,01h,00h
 008h 4 Master Hash Size (eg. 20h) (aka Size of SHA256's at 00060h)
 00Ch 8 Level 1 Logical Offset (usually 0) ;\
 014h 8 Level 1 Size of SHA256's at xx000h (eg. 60h aka 24A0h/1000h*20h) ;
 01Ch 4 Level 1 Block size (1 SHL N) (usually 0Ch=1000h) ;/
 020h 4 Reserved (0)
 024h 8 Level 2 Logical Offset (eg. 1000h) (Level 1 size) ;\
 02Ch 8 Level 2 Size of SHA256's at yy000h (eg. 24A0h aka 124A20h/1000h*20h)
 034h 4 Level 2 Block size (1 SHL N) (usually 0Ch=1000h) ;/
 038h 4 Reserved (0)
 03Ch 8 Level 3 Logical Offset (eg. 4000h) (Level 1+2 size) ;\
 044h 8 Level 3 Size of Directory/File area at 001000h (eg. 124A20h) ;
 04Ch 4 Level 3 Block size (1 SHL N) (usually 0Ch=1000h) ;/
 050h 4 Reserved (0)
 054h 4 Header Size or so? (usually 5Ch)
 058h 4 Optional info size (uh?) (usually 0)
 05Ch 4 unknown/padding (usually 0) |
| --- |

The "Logical Offsets" are completely unrelated to the physical offsets in the
RomFS partition. Instead, the "Logical Offsets" might be something about where
to map the Level 1-3 sections in virtual memory (with the physical Level 3,1,2
ordering being re-ordered to Level 1,2,3)?

The various SHA256's are similar to the "Digest" tables in DSi cart header.

**RomFS Directory/File area (aka Level 3) (usually/always? starting at 1000h)**

```

| 000h 4 Header Length (usually 28h)
 004h 4 Directory Hash Table Offset (usually 28h)
 008h 4 Directory Hash Table Length
 00Ch 4 Directory Table Offset (eg. 44h) (first=Root)
 010h 4 Directory Table Length
 014h 4 File Hash Table Offset
 018h 4 File Hash Table Length
 01Ch 4 File Table Offset (eg. 170h) (starting with Root)
 020h 4 File Table Length (eg. 52Ch)
 024h 4 File Data Offset (eg. 6A0h)
 028h .. Directory Hash Table
 .. .. Directory Table
 .. .. File Hash Table
 .. .. File Table
 .. .. File Data (each file aligned to 10h-byte boundary) |
| --- |

Note: The first Directory Table entry is the Root directory. Apart from that,
there is no real requirement for the location of the other directory (and file)
table entries. However, Table entries are commonly held to be sorted
alphabetically. And, Siblings should be stored at continous table offsets (so
all entries for one directory are grouped together; that can speed up sector
loading).

**Directory Table Entries**

```

| 00h 4 Directory Table Offset of Parent Directory (Self for Root)
 04h 4 Directory Table Offset of next Sibling Directory (FFFFFFFFh=None)
 08h 4 Directory Table Offset of first Child Directory (FFFFFFFFh=None)
 0Ch 4 File Table Offset of first File (FFFFFFFFh=None)
 10h 4 Directory Table Offset of next Directory with same Hash
 14h 4 Name Length in bytes (ie. L=NumChars*2) (0 for Root)
 18h L Directory Name (16bit Unicode)
 18h+L 0/2 Alignment padding |
| --- |

**File Table Entries**

```

| 00h 4 Directory Table Offset of Parent Directory
 04h 4 File Table Offset of next Sibling File (FFFFFFFFh=None)
 08h 8 File Data Offset (64bit)
 10h 8 File Data Size (64bit)
 18h 4 File Table Offset of next File with same Hash (FFFFFFFFh=None)
 1Ch 4 Name Length in bytes (ie. L=NumChars*2)
 20h L File Name (16bit Unicode)
 20h+L 0/2 Alignment padding |
| --- |

**File/Directory Hash Tables (for quick lookup)**

```

| 00h N*4 Offset to File/Directory with matching Chksum (FFFFFFFFh=None) |
| --- |

These tables can be used for fast lookup (faster than doing char-by-char string
compares for all names in directory). For searching a specific name in a
specific directory, compute a checksum on the Directory Offset and Name:

```

| chksum = ParentDirectoryOffset XOR 123456789
 for i=0 to NameLength/2-1
 chksum = (chksum ROR 5) XOR Name[i*2] ;32bit rotate, then XOR lower 16bit
 offset = hashtable [(chksum MOD (hashtablesize/4))*4] |
| --- |

Then verify if Name(offset) and ParentDirectory entry(offset) do actually
match, and else retry with offset=NextHashOffset(offset).

Note: The hashtablesize's should be roughly same or bigger than the total
number of files/directories in the RomFS (multiplied by 4, as each entry is
32bit wide). Even then, there can be false matches, including matches in
different directories (which can slowdown things when needing additional sector
reads for obtaining the table entries for such directories).

**RomFS can be used for...**

```

| in conjunction with the ExeFS of a NCCH
 to contain the game manual accessible from the Home Menu
 to contain the DLP Child CIA ;uh, is that "CIA" a homebrew fileformat?
 to contain game cartridge update data |
| --- |