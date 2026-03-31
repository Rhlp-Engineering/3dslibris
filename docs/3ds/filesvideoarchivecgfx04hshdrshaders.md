# Filesvideoarchivecgfx04Hshdrshaders

> Source: https://problemkaputt.de/gbatek.htm
> Section: Filesvideoarchivecgfx04Hshdrshaders

3DS Files - Video Archive (CGFX_04h - SHDR Shaders) 
| |
| --- |

 SHDR is found some CGFX files, for example,
 
```

| 3DS eShop RomFS:\shader contains one CGFX (and several raw DVLBs files)
 3DS Sound RomDS:\res\GfxShader.LZ contains two SHDR's in a single GGFX |
| --- |

**SHDR - Shader (contains a regular DVLB file, with some extra parameters)**

```

| 000h 4 Flags (eg. 80000002h)
 004h 4 ID "SHDR"
 008h 4 Revision (05000000h)
 00Ch 4 Name Offset (self-relative) (same as in parent DICT)
 010h 4+4 Unknown (0,0)
 018h 4 Shader Size (eg. 26A4h) ;\Shader in DVLB file format
 01Ch 4 Shader Offset (self-relative) (28h) ;/
 020h 4 Unknown (04h) ;number of below words (eg. 4) ;\
 024h 4 Unknown (eg. 26C4h) ;ptr to 4 words (8B31h,8B31h,8B31h,8B31h) ;/
 028h 4 Unknown (04h) ;number of below words (eg. 4) ;\
 02Ch 4 Unknown (eg. 26CCh) ;ptr to 4 words (20h,A4h,128h,1ACh) ;Offsets;/
 030h 4 Unknown (04h) ;number of below words (eg. 4) ;\
 034h 4 Unknown (eg. 26D4h) ;ptr to 4 words (0,0,0,0) ;/
 038h 4 Unknown (0)
 03Ch 4 Unknown (0)
 040h 4 Unknown (0) |
| --- |

Unknown what the three lists at [20h..37h] are doing, and unknown what is at
the offset of the second list.

For the DVLB file format, see

3DS Files - Video Shaders (DVLB and DVOB Format)