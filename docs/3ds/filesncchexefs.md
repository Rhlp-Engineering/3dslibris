# Filesncchexefs

> Source: https://problemkaputt.de/gbatek.htm
> Section: Filesncchexefs

3DS Files - NCCH ExeFS 
| |
| --- |

 **ExeFS Header (max 10 files)**
 
```

| 000h 10h*10 File headers for File 1..10 ;First at 000h
 0A0h 20h Reserved
 0C0h 20h*10 SHA256 Hash for File 10..1 ;First at 1E0h |
| --- |

File hashes are stored in reverse order, i.e. the hash at offset 1E0h
corresponds to the first ExeFS section.

Each file header has a corresponding file hash, which is the SHA256 hash
calculated over the entire file contents.

**File headers**

There are a maximum of 10 file headers in the ExeFS format (the maximum number
of file headers is disputable, with makerom indicating a maximum of 8 sections
and makecia indicating a maximum of 10 (uh, are homebrew tools?). From a
non-SDK point of view, the ExeFS header format can hold no more than 10 file
headers within the currently define size of 200h bytes). The file headers have
the following structure:

```

| 00h 8 File name (ASCII, zeropadded) (all 00h for unused entries)
 08h 4 File offset in bytes (0=Right after the 200h-byte ExeFS Header)
 0Ch 4 File size in bytes |
| --- |

File offsets are non-inclusive of the header's size (200h bytes). Also, file
headers which are not used are filled with all zeros.

The file offsets should be 200h-byte aligned. The file size is usually 4-byte
aligned (unknown if that is actually required).

**ExeFS Files**

ExeFS or Executable Filesystem contains information related to the executable
program, and is the part of the CXI format.

The ExeFS usually contains one or more of the following files:

```

| .code Contains ARM11 code, which can be optionally
 reverse-LZSS compressed via an exheader flag.
 .firm Contains FIRM with ARM9/ARM11 code (usually plus some NCCH's)
 logo Contains distribution licensing Binary data
 banner Contains the banner which homemenu uses for this CXI
 icon Contains the icon which homemenu displays for this CXI |
| --- |

3DS Files - NCCH Logo

**.code (ARM11)**

The .code file does simply contain plain code/data without any file header
(instead, the .code file's load addresses and compression flag are located in
the NCCH Exheader).

For the LZrev decompression function, see:

LZ Decompression Functions

The (decompressed) filesize should be a multiple of 1000h bytes (and the
code/const/data sections should be on 1000h-byte page boundaries, as specified
in the NCCH Exheader).

**.code (GBA)**

The .code files for GBA games do reportedly contain the GBA ROM-image, with an
extra GBA Footer (with info on cartridge type and preferred LCD color scheme).
For details:

3DS Config - ARM7 Registers (GBA/NDS/DSi Mode)

Unknown if GBA games have specific entries in NCCH header/exheader.