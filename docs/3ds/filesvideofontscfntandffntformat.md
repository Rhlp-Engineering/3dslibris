# Filesvideofontscfntandffntformat

> Source: https://problemkaputt.de/gbatek.htm
> Section: Filesvideofontscfntandffntformat

3DS Files - Video Fonts (CFNT and FFNT Format) 
| |
| --- |

 **CTR Font Format**
 .bcfnt files are bitmap fonts made for the 3ds. The system font uses this format too except the header is changed from CFNT to CFNU and all file offsets are changed to absolute in memory offsets.
 
```

| CFNT - 3DS Font version 3 file
 CFNU - 3DS Font version 3 stored in memory (System Font)
 FFNT - 3DS Font version 4 file |
| --- |

The format is based on the Nitro Font format for DS/DSi:

DS Cartridge Nitro Font Resource Format

**CFNT Header**

```

| 000h 4 ID ("CFNT" or "CFNU" or "FFNT")
 004h 2 Byte Order (FEFFh)
 006h 2 Header Size
 008h 4 Version (usually 03000000h) (also so for FFNT ?)
 00Ch 4 File size (the total)
 010h 4 Number of "blocks" to read |
| --- |

**Font Info Chunk**

```

| CFNT FFNT Siz Content
 000h 000h 4 ID ("FINF")
 004h 004h 4 Section Size
 008h 008h 1 Font Type
 009h - 1 Line Feed (8bit)
 - 00Ch 2 Line Feed (16bit)
 00Ah 00Eh 2 Alter Char Index (maybe tileno for undefined chars?)
 00Ch 010h 3 Default Width's (3 bytes: Left, Glyph Width, Char Width)
 00Fh 013h 1 Encoding (1=Unicode?)
 010h 014h 4 TGLP Offset
 014h 018h 4 CWDH Offset
 018h 01Ch 4 CMAP Offset
 01Ch 009h 1 Height
 01Dh 00Ah 1 Width
 01Eh 00Bh 1 Ascent
 01Fh - 1 Reserved |
| --- |

**Character Glyph Chunk (Tile Bitmaps)**

```

| CFNT FFNT Siz Content
 000h 000h 4 ID ("TGLP")
 004h 004h 4 Section Size
 008h 008h 1 Cell Width
 009h 009h 1 Cell Height
 00Ah - 1 Baseline Position (8bit)
 - 010h 2 Baseline Position (16bit)
 00Bh 00Bh 1 Max Character Width
 00Ch 00Ch 4 Sheet Size
 010h - 2 Number of Sheets (16bit)
 - 00Ah 1 Number of Sheets (8bit)
 012h 012h 2 Sheet Image Format 0-13: (RGBA8, RGB8, RGBA5551, RGB565, RGBA4,
 014h 014h 2 Number of columns LA8, HILO8, L8, A8, LA4, L4, A4,
 016h 016h 2 Number of rows ETC1, ETC1A4)
 018h 018h 2 Sheet Width
 01Ah 01Ah 2 Sheet Height
 01Ch 01Ch 4 Sheet Data Offset |
| --- |

3DS GPU Texture Formats

**Character Map(s) - Translation Tables for ASCII/JIS/etc to Tile Numbers?**

```

| 000h 4 ID ("CMAP")
 004h 4 Section Size
 008h 2 Code Begin
 00Ah 2 Code End
 00Ch 2 Mapping Method (0=Direct, 1=Table, 2=Scan)
 00Eh 2 Reserved?
 010h 4 Next CMAP Offset |
| --- |

Depending on the mapping method the next bytes are read like so:

```

| Direct: IndexOffset = read uint16
 Table: IndexTable = read uint16 array (CodeEnd - CodeBegin + 1)
 Scan: Number of Scan Entries = read uint16 for every scan
 entry: read uint16 & read uint16 |
| --- |

**Character Width Chunk**

```

| 000h 4 ID ("CWDH")
 004h 4 Section Size
 008h 2 Start Index ;\implies NumEntries = EndIndex-StartIndex+1
 00Ah 2 End Index ;/
 00Ch 4 Next CWDH Offset
 010h 3*N Char Widths (3 bytes: Left, Glyph Width, Char Width) |
| --- |

**Tools**

BFFNT Extractor/Creator. Creates PNG files from TGLP atlas data and a JSON
manifest.

See also: [http://www.3dbrew.org/wiki/System_Font](http://www.3dbrew.org/wiki/System_Font)

**Truetype Fonts (TTF)**

Some 3DS titles are alsu using Truetype fonts:

```

| 3DS System Data Archives 1, web-kit/OSS CROs (RomFS:\font\nintendo.ttf)
 3DS Browser applet (RomFS:\browser\font\nintendo_NTLG-DB_001.ttf) |
| --- |

That fileformat is also used in the Windows operating system:

```

| used in Windows 3.1 (Windows\System\*.ttf)
 used in Windows 95 (Windows\Fonts\*.ttf) |
| --- |

Fileformat specs can be found at:

```
[https://docs.microsoft.com/en-us/typography/opentype/spec/](https://docs.microsoft.com/en-us/typography/opentype/spec/)
| |
| --- |