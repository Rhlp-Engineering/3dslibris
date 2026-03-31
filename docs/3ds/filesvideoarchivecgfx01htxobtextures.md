# Filesvideoarchivecgfx01Htxobtextures

> Source: https://problemkaputt.de/gbatek.htm
> Section: Filesvideoarchivecgfx01Htxobtextures

3DS Files - Video Archive (CGFX_01h - TXOB Textures) 
| |
| --- |

 TXOB is found in most CGFX files.
 
 **TXOB - Texture Objects?**
 TXOBs are contained within MTOBs. They can describe textures; if such is the case, then their structure is as follows:
 
```

| Can be 40h-byte or 5Ch-byte size?
 3dbrew messed blurb claims 4Ch-bytes (with overlapping entries)?
 Or are the 40h-byte ones sub-items in MTOB?
 And only the 5Ch-byte ones are actually DICT 01h entries?
 000h 4 Flags (20000011h=Normal/5Ch-byte. 20000009h=Cubemap/110h-byte)
 004h 4 ID "TXOB"
 008h 4 Revision? (05000000h)
 00Ch 4 Offset (self-relative) to symbol
 010h 4 Unknown (0)
 014h 4 Unknown (0)
 018h 4 OpenTK Texture height (eg. 40h) rounded up ;\
 01Ch 4 OpenTK Texture width (eg. 10h) rounded up ; info #1
 020h 4 OpenTK Texture Format (67xxh) ;
 024h 4 OpenTK Data Type (0000h,1401h,67xxh,80xxh) ;/
 028h 4 Mipmap levels (eg. 1)
 02Ch 4 Unknown (0)
 030h 4 Unknown (0)
 034h 4 GPU Texture format (00h..0Dh, GPU format number)
 038h 4 Offset TexSide0 (04h) (self-relative) ;\normal texture
 03Ch 20h TexSide0 ;/(flags=20000011h)
 038h 6*4 Offset TexSide0..5 (18h,34h,50h,6Ch,88h,A4h) ;\cubemap
 050h 6*20h TexSide0..5 ;/(flags=20000009h)
 TexSide(s):
 000h 4 GPU Texture height (eg. 40h) ;\same as in [18h,1Ch] ?
 004h 4 GPU Texture width (eg. 10h) ;/
 008h 4 Texture data size (eg. 800h) (width*height*bpp/8) ;\
 00Ch 4 Texture data offset (self-relative) (in IMAG area) ;/
 010h 4 Unknown (0)
 014h 4 Unknown (?) (eg. 10h)
 018h 4 Unknown (0)
 01Ch 4? Unknown (0) |
| --- |

Texture format:

```

| 00h = RGBA8
 01h = RGB8
 02h = RGBA5551
 03h = RGB565
 04h = RGBA4
 05h = LA8
 06h = HILO8
 07h = L8
 08h = A8
 09h = LA4
 0Ah = L4
 0Bh = A4
 0Ch = ETC1
 0Dh = ETC1A4 |
| --- |

3DS GPU Texture Formats