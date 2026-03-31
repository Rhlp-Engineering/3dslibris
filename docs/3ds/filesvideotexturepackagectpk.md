# Filesvideotexturepackagectpk

> Source: https://problemkaputt.de/gbatek.htm
> Section: Filesvideotexturepackagectpk

3DS Files - Video Texture Package (CTPK) 
| |
| --- |

 CTPK archives can contain one or more texture files. The archive does usually contain filenames with .tga extension (although the CTPK bitmaps are raw data, unrelated to the Targa .tga format) (confusingly, games that use CTPK files do also have other LZss-compressed texture .lz77 files in the same folder, and THOSE files are actually having Targa .tga headers/footers).
 
 CTPK is used in "3DS AR Games" (RomFS:\i_ar\tex\Tx_fishing.ctpk), and reportedly in 3DS games "Persona Q" and "Persona Q2". Anyways, more common 3DS texture formats seem to be CLIM files, and TXOB's inside of CFGX files.
 
 **CTPK Header**
 
```

| 000h 4 ID "CTPK" ;\
 004h 2 Version (0001h) ;
 006h 2 Number of Texture Info Entries (N) ;
 008h 4 Texture Data section offset (aka size of header+info) ; Header
 00Ch 4 Texture Data section size ;
 010h 4 Texture Filename CRC32 Table ;
 014h 4 Conversion Info section offset ;
 018h 8 Padding (0) ;/
 020h N*20h Texture Info Entries ;\
 ... N*4 Texture Bitmap Sizes (same as [004h] in Info Entries?) ;
 ... N*var Texture Filename Strings (ASCII, terminated by 00h) ;
 ... .. Padding (0) (to 8-byte boundary) ; Info
 ... N*8 Texture Filename CRC32 Table ;
 ... N*4 Conversion Info Block Entries ;
 ... ... Padding (0) (to 80h-byte boundary) ;/
 ... N*var Texture Data section (raw bitmaps) ;-Data |
| --- |

**Texture Info Entries (starting at offset 020h)**

Contains Filename, Size, Type, etc. (sorted aphabetically by filename).

```

| 000h 4 Filename Offset
 004h 4 Texture Data Size (eg. 200h,800h,2000h,4000h,8000h?)
 008h 4 Texture Data Offset (relative to Texture Data section offset)
 00Ch 4 Texture Format (is that same/other as in "Conversion Info"?)
 010h 2 Width
 012h 2 Height
 014h 1 Mip Level (eg. 01h) (usually 1)
 015h 1 Type (0=Cubemap, 1=1D, 2=2D) (usually 2)
 016h 2 Cubemap Related ... ? (usually 0)
 018h 4 Bitmap Size Array Offset/4 (what for??)
 01Ch 4 Unix Timestamp (4Cxxxxxxh) |
| --- |

**Texture Filename CRC32 Table**

CRC32s on the filenames (sorted by CRC32 values, starting with smallest value).

```

| 000h 4 CRC32 of Filename (excluding trailing 00h)
 004h 4 File Number, index in Texture Info Entries (0..(N-1)) |
| --- |

**Conversion Info Block Entries (unknown purpose, maybe info on source data?)**

```

| 000h 1 Texture Format (usually 4,5,9,0Dh) (see below)
 001h 1 Unknown (usually 1=what?)
 002h 1 Compressed (usually 1=what?)
 003h 1 ETC1 Related (Quality?) (usually 3 or 6=what?) |
| --- |

Texture Formats:

```

| 00h = RGBA8888
 01h = RGB888
 02h = RGBA5551
 03h = RGB565
 04h = RGBA4444
 09h = ?
 0Ch = ETC1
 0Dh = ETC1 + Alpha |
| --- |

3DS GPU Texture Formats