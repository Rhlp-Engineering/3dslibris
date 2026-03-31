# Filesvideotexturebinaryctxb

> Source: https://problemkaputt.de/gbatek.htm
> Section: Filesvideotexturebinaryctxb

3DS Files - Video Texture Binary (CTXB) 
| |
| --- |

 **CTXB (CTR Texture Binary) (.ctxb files)**
 CTXB format is used by Zelda Ocarina of Time 3D, Majora's Mask 3D, Luigi's Mansion 3D, and Ever Oasis.
 The files in Ocarina of Time seem to contain only one texture per file (the format can reportedly contain some extra features: More than one texture, Texture names, Cubemaps, Mipmaps - but unknown if/which games are using those features).
 
```

| 000h 4 Chunk ID "ctxb" ;\
 004h 4 Total Filesize ;
 008h 1 Value 01h (version, or number of texture chunks or so) ; file
 009h 7 Zero ; header
 010h 4 Chunk Size (usually 18h) ;
 014h 4 Texture Base Offset (usually 48h) ;/
 018h 4 Chunk ID "tex " ;\tex
 01Ch 4 Chunk Size (0Ch+N*24h?) (usually 30h) ; header
 020h 4 Number of textures (1) (usually 1) ;/
 024h 4 Texture bitmap size in bytes (filesize-48h) ;\
 028h 2 Mipmap Count (1) (usually 0) ;
 02Ah 1 Compression (0=None, 1=ETC1/ETC1A4) (usually 0 or 1) ; texture
 02Bh 1 Cubemap (0=Normal, 1=Cubemap) (usually 0) ;
 02Ch 2 Width (in pixels, power of two) ;
 02Eh 2 Height (in pixels, power of two) ;
 030h 2 OpenTK Texture Format (67xxh) ;
 032h 2 OpenTK Data Type (0000h,1401h,67xxh,80xxh) ;
 034h 4 Texture offset (from Texture Base Offset) (usually 0) ;
 038h 10h Texture name (if any) (usually zerofilled) ;/
 .. .. Cubemap entries (if any?) (usually none)
 048h ... Texture bitmap (width*height*bpp/8) |
| --- |

CubemapData (exists only if Cubemap flag is set):

```

| 000h 4 Data Offset Right (+X)
 004h 4 Data Offset Left (-X)
 008h 4 Data Offset Bottom (-Y)
 00Ch 4 Data Offset Top (+Y)
 010h 4 Data Offset Front (-Z)
 014h 4 Data Offset Back (+Z) |
| --- |

Unknown how/where that Cubemap stuff is stored exactly, probably in the Data
area, at the "begin of the bitmap"? Cubemaps must probably have Width=Height?

The Texture Format is defined as pair of OpenTK constants. For details, see

3DS GPU Texture Formats