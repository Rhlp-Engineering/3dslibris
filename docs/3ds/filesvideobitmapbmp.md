# Filesvideobitmapbmp

> Source: https://problemkaputt.de/gbatek.htm
> Section: Filesvideobitmapbmp

3DS Files - Video Bitmap (BMP) 
| |
| --- |

 **Windows Bitmap (.bmp)**
 Windows bitmap used in 3DS Browser applet (RomFS:\browser\page\img\*.bmp)
 Adobe variant used in 3DS AR Games (RomFS:\i_ar\tex\stamp*.lz77)
 
```

| 000h 2 ID "BM" (Windows Bitmap, aka .bmp)
 002h 4 Total Filesize
 006h 4 Reserved (usually 0)
 00Ah 4 Bitmap Pixeldata Offset
 00Eh 4 Size of Header bytes at 0Eh and up (28h=Normal, 38h=Adobe)
 012h 4 Width in pixels (positive)
 016h 4 Height in pixels (pos=Windows/bottom up, neg=Adobe/top down)
 01Ah 2 Number of Bitplanes (always 1)
 01Ch 2 Bits per Pixel (bpp) (usually 1,4,8,16,24,32)
 01Eh 4 Compression Type (usually 0=Uncompressed) (3=custom RGB bitmasks?)
 022h 4 Compressed Bitmap Size (can be Dummy=0 if uncompressed?)
 026h 4 Horizontal resolution (pixels per metre) ;\eg. 0B12h or 03E8h
 02Ah 4 Vertical resolution (pixels per metre) ;/
 02Eh 4 Number of colors in palette (or 0=Default 2^bpp) (or 0=None?)
 032h 4 Number of important colors (or 0=All)
 036h (4) Birmask Red ;\Custom color masks for 16bpp..32bpp bitmaps
 036h (4) Birmask Green ; only if [01Eh]=3 or 6
 036h (4) Birmask Blue ;/
 036h (4) Birmask Alpha ;-only if [01Eh]=6, or [01Eh]=3 and [00Eh]=38h
 ... (..) Palette (if any, if bpp<=8)
 ... .. Bitmap Pixels |
| --- |