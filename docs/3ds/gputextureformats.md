# Gputextureformats

> Source: https://problemkaputt.de/gbatek.htm
> Section: Gputextureformats

3DS GPU Texture Formats 
| |
| --- |

 **3DS GPU Texture Format numbers (and CLIM numbers)**
 
```

| GPU CLIM Name bpp Red Green Blue Alpha
 00h 09h RGBA8888 32bit Bit31-24 Bit23-16 Bit15-8 Bit7-0
 01h 06h RGB888 24bit Bit23-16 Bit15-8 Bit7-0 FFh
 02h 07h RGBA5551 16bit Bit15-11 Bit10-6 Bit5-1 Bit0
 03h 05h RGB565 16bit Bit15-11 Bit10-5 Bit4-0 FFh
 04h 08h RGBA4444 16bit Bit15-12 Bit11-8 Bit7-4 Bit3-0
 05h 03h LA88 16bit <--------Bit15-8-------> Bit7-0
 06h 04h HILO88 16bit Bit15-8 Bit7-0 00h FFh
 07h 00h L8 8bit <--------Bit7-0--------> FFh
 08h 01h A8 8bit <--------00h-----------> Bit7-0
 09h 02h LA44 8bit <--------Bit7-4--------> Bit3-0
 0Ah 0Ch L4 4bit <--------Bit3-0--------> FFh
 0Bh 0Dh A4 4bit <--------00h-----------> Bit3-0
 0Ch 0Ah ETC1 4bit/average <---64bit per 4x4pix---> FFh
 0Dh 0Bh ETC1A4 8bit/average <---64bit per 4x4pix---> Bit3-0 |
| --- |

The RGB/RGBA bit-order for 3DS is conventionally MSBs=Red, LSBs=Alpha (but the
GPU shader unit can swap the input/values, so one could also use anything like
BGRA or ARGB; except, swapping the 1bit-alpha in RGBA5551 may have unintended
results).

Unknown if any of the GPU's "LUT" look-up tables can be used to convert
Luminance values (or other RGBA values) to palette indices?

**OpenTK Format+Type pairs**

Some 3DS files have the "Texture Format" expressed as "OpenTK Format+Type
pairs":

```

| CGFX:TXOB chunks contain OpenTK constants (plus GPU format numbers)
 CTXB:TEX chunks contain OpenTK constants (without GPU format numbers) |
| --- |

Below shows how to translate 3DS GPU Format numbers to/from OpenTK constants:

```
[https://wiki.cloudmodding.com/oot/3D:CTXB_format](https://wiki.cloudmodding.com/oot/3D:CTXB_format)[https://github.com/dptug/FEZ/blob/master/OpenTK/Graphics/OpenGL/PixelType.cs](https://github.com/dptug/FEZ/blob/master/OpenTK/Graphics/OpenGL/PixelType.cs)
| GPU Format OpenTK Texture Format OpenTK Data Type
 00h RGBA8888 <--> 6752h RGBANativeDMP 1401h UnsignedByte (u8)
 01h RGB888 <--> 6754h RGBNativeDMP 1401h UnsignedByte (u8)
 02h RGBA5551 <--> 6752h RGBANativeDMP 8034h UnsignedShort5551
 03h RGB565 <--> 6754h RGBNativeDMP 8363h UnsignedShort565
 04h RGBA4444 <--> 6752h RGBANativeDMP 8033h UnsignedShort4444
 05h LA88 <--> 6758h LuminanceAlphaNativeDMP 1401h UnsignedByte (u8)
 06h HILO88 <--> 6759h ...NativeDMP? 1401h UnsignedByte (u8)
 07h L8 <--> 6757h LuminanceNativeDMP 1401h UnsignedByte (u8)
 08h A8 <--> 6756h AlphaNativeDMP 1401h UnsignedByte (u8)
 09h LA44 <--> 6758h LuminanceAlphaNativeDMP 6760h UnsignedByte44DMP
 0Ah L4 <--> 6757h LuminanceNativeDMP 6761h Unsigned4BitsDMP
 0Bh A4 <--> 6756h AlphaNativeDMP 6761h Unsigned4BitsDMP
 0Ch ETC1 <--> 675Ah ETC1RGB8NativeDMP 0000h (implied in ETC1)
 0Dh ETC1A4 <--> 675Bh ETC1AlphaRGB8A4NativeDMP 0000h (implied in ETC1A4) |
| --- |

**ETC1 (64bit per 4x4 pixels) (iPACKMAN aka Ericsson Texture Compression)**

```

| 0-15 Bit0 of Pixel0..15 (aka 0=Small, 1=Large) ;\clamp to 00h..FFh
 16-31 Bit1 of Pixel0..15 (aka 0=Add, 1=Sub) ;/
 32 Flip bit (0=LeftColor1+RightColor2, 1=TopColor1+BottomColor2)
 33 Diff bit (0=2xRGB444, 1=RGB555+/-RGB333)
 34-36 Color 2 Offset Table (0..7, see below)
 37-39 Color 1 Offset Table (0..7, see below)
 When Diff=0
 40-43 Color 2 Blue (4bit) ;\
 44-47 Color 1 Blue (4bit) ; when Diff=0,
 48-51 Color 2 Green (4bit) ; for color1/2, use 4bit as is
 52-55 Color 1 Green (4bit) ; and expand to 8bit = 4bit*11h
 56-59 Color 2 Red (4bit) ;
 60-63 Color 1 Red (4bit) ;/
 When Diff=1
 40-42 Color 2 Blue (signed 3bit) ;\when Diff=1,
 43-47 Color 1 Blue (5bit) ; for color1, use 5bit as is
 48-50 Color 2 Green (signed 3bit) ; for color2, use 5bit=5bit+signed3bit
 51-55 Color 1 Green (5bit) ; (the result should never exceed 0..1Fh)
 56-58 Color 2 Red (signed 3bit) ; thereafer, expand to 8bit = 5bit*21h/4
 59-63 Color 1 Red (5bit) ;/ |
| --- |

Offset Tables (with "(Small,Large)" step pairs):

```

| (2,8), (5,17), (9,29), (13,42), (18,60), (24,80), (33,106), (47,183) |
| --- |

Pixel ordering (unlike the usual Z-order which is used for other 3DS textures):

```

| 0 4 8 12
 1 5 9 13
 2 6 10 14
 3 7 11 15
 A whole 8x8pix tile consists of four such 4x4pix blocks (that four blocks
 are stored in Z-order, but the pixels inside of the block are ordered as
 shown above, ie. NOT in Z-order).
 The official ETC1 specs suggest to store the 64bit ETC1 values in big-endian,
 however, the 3DS is storing them in little-endian. |
| --- |

ETC1 specs can be found in the TXT document linked below (there are also
distorted HTM documents with endiannes expressed as "backslash times" instead
of "multiply").

khronos.org/registry/OpenGL/extensions/OES/OES_compressed_ETC1_RGB8_texture.txt

**ETC1A4 (128bit per 4x4 pixels)**

```

| 0-63 Alpha for Pixel0..15 (pixel0 in bit0..3, pixel1 in bit4..7, etc.)
 64-127 ETC1 with Colors and Luminance bits for Pixel0..15 |
| --- |

This is some kind of a custom extension of the ETC1 format, with additional
4bit alpha for each pixel (the alpha values are ordered the same way as in the
ETC1 format, eg. Pixel3 is lower-left of the 4x4 pixel area).