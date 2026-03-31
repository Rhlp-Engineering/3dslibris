# Filesvideoarchivecgfx06Hcfltchltcaltlights

> Source: https://problemkaputt.de/gbatek.htm
> Section: Filesvideoarchivecgfx06Hcfltchltcaltlights

3DS Files - Video Archive (CGFX_06h - CFLT/CHLT/CALT Lights) 
| |
| --- |

 CFLT (Directional/Fragment Lights) is found in some CGFX files, eg. 3DS System Settings banner
 CHLT (Hemisphere Lights) is less common, but found in StreetPass Mii Plaza RomFS:\graphics\model\figure_Galaxy2.lz77 and figure_Link.lz77
 CALT (Ambient? Lights) is even less common, found in StreetPass Mii Plaza RomFS:\graphics\model\figure_Pikmin.lz77 and figure_Samus.lz77
 
 **CFLT Format**
 
```

| 000h 4 Flags (eg. 400000A2h)
 004h 4 ID "CFLT"
 008h 4 Revision (06000000h)
 00Ch 4 Name Offset (self-relative) (same as in parent DICT)
 010h 4+4 Unknown (0,0)
 018h 4+4 Unknown (1,1)
 020h 4+4 Unknown (0,0)
 028h 4+4 DICT for Light Animation (NumEntries,Offset)
 030h 0Ch Vector? (3xFloat32) (1.0, 1.0, 1.0)
 03Ch 0Ch Vector? (3xFloat32) (0.0, 0.0, 0.0)
 048h 0Ch Vector? (3xFloat32) (0.0, 0.0, 0.0)
 054h 30h Matrix? (4x3xFloat32) (Identity)
 084h 30h Matrix? (4x3xFloat32) (Identity)
 0B4h 4 Unknown (1)
 0B8h 10h Unknown (zeroes)
 0BCh 40h Float32's (all 1.0)
 0FCh 10h Unknown (FF000000h, FFFFFFFFh, FFFFFFFFh, FFFFFFFFh)
 10Ch 0Ch Vector? (3xFloat32?) (3F13CD3Ah, 3F13CD3Ah, BF13CD3Ah)
 118h 0Ch Vector? (3xFloat32?) (00000000h, 00000000h, 00000000h)
 124h 0Ch Vector? (3xFloat32?) (3F800000h, 0003F000h, 00080000h)
 130h 4 Unknown (1) |
| --- |

Some of the Vectors might be X,Y,Z vectors. Some might be R,G,B colors?

**Light Animation (from DICT in CFLT) (eg. Name=LightAnimation)**

```

| 000h 4 Unknown (80000000h)
 004h 4 Unknown (0)
 008h 4 Name Offset (self-relative) (same as in parent DICT)
 00Ch 4 Unknown (4)
 010h 4+4 DICT for whatever (NumEntries,Offset)
 018h 4 Unknown (5)
 01Ch 4 Unknown (8)
 020h 4 Unknown (0)
 024h 4 Unknown (0)
 028h 4 Unknown (3)
 02Ch 4 Unknown (6)
 030h 4 Unknown (2)
 034h 4 Unknown (0) |
| --- |

The DICT can have several entries (with names Ambient, Diffuse, Direction,
DistanceAttentuationEnd, DistanceAttentuationStart, IsLightEnabled, Specular0,
Specular1, Transform). The formats of those entries are unknown (the entries
seem to use different formats, eg. IsLightEnabled is 8 bytes).

**CHLT Format**

**CALT Format**

Unknown. Seems to be similar as CFLT (but, in case of CALT, with less Light
Animation DICT entries).