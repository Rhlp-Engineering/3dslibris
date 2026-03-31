# Filesvideolayoutimagesclimflim

> Source: https://problemkaputt.de/gbatek.htm
> Section: Filesvideolayoutimagesclimflim

3DS Files - Video Layout Images (CLIM/FLIM) 
| |
| --- |

 CLIM/FLIM files contain texture bitmaps, commonly used for GUI layout (rather than for 3D game engines). CLIM is the standard format for 3DS, FLIM appears to be more common on Wii-U (but is also used on 3DS).
 
```

| CLIM is used in Manual NCCH.RomFS textures *.bclim
 CLIM is used in Exe NCCH.header logos *.bclim
 CLIM is used in Exe NCCH.RomFS layout\*.bclim
 FLIM is used in 3DS eShop applet NCCH.RomFS:\layout\*.bflim |
| --- |

**File Format with Footer**

```

| 00000000h .. Texture data (with width/height padded to power-of-2, min 8x8)
 Footer+00h 4 ID "CLIM" ;FLIM: ID "FLIM"
 Footer+04h 2 Byte Order (FEFFh)
 Footer+06h 2 Footer Size (14h)
 Footer+08h 4 Version? (02020000h) ;FLIM: Version (07020000h)
 Footer+0Ch 4 Total Filesize
 Footer+10h 4 Number of Blocks (usually 1 = imag)
 Footer+14h 4 Chunk ID "imag"
 Footer+18h 4 Chunk Size (10h)
 Footer+1Ch 2 Texture Width, can be odd ;FLIM: Height (after rotate: Width)
 Footer+1Eh 2 Texture Height, can be odd;FLIM: Width (after rotate: Height)
 Footer+20h 1 Texture Format (00h..0Dh) ;FLIM: Reportedly Align? (80h)
 Footer+21h 1 Zero ;FLIM: MSB of above? (00h)
 Footer+22h 1 Zero ;FLIM: Texture Format (00h..0Dh)
 Footer+23h 1 Zero ;FLIM: Unknown (04h or 08h?)
 Footer+24h 4 Footer Offset (Filesize-28h) ;<--stored at end of file |
| --- |

The Texture Format values don't use the same numbering as the GPU. The
following table can be used to translate CLIM and FLIM numbers to GPU numbers:

```

| clim_to_gpu_table: 07h,08h,09h,05h,06h,03h,01h,02h,04h,00h,0Ch,0Dh,0Ah,0Bh |
| --- |

3DS GPU Texture Formats

The CLIM/FLIM width/height entries can be odd, the actual sizes are rounded to
powers of two, with at least 8 pixels (for example, 33x4 would become 64x8,
that includes several completely unused 8x8 tiles which are nethertheless
stored in the CLIM/FLIM file).

**FLIM**

3DS eShop FLIM files are somewhat mirrored and rotated (they can be decoded
using pixel order for the height/width entries; but they must be mirrored
and/or rotated left or right after decoding (which will swap height/width to
width/height).

3DS eShop FLIM is using the same Texture Format numbers as CLIM (up to
including 0Ch=L4 and 0Dh=A4). However, there is a webpage claiming other
numbers to be used (which might be nonsense, or maybe there is "another FLIM
format for Wii-U"): [https://mk8.tockdom.com/wiki/BFLIM_(File_Format)](https://mk8.tockdom.com/wiki/BFLIM_(File_Format))