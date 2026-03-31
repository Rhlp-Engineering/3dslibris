# Filesvideoarchivecgfx09H0Ah0Dhcanmanimations

> Source: https://problemkaputt.de/gbatek.htm
> Section: Filesvideoarchivecgfx09H0Ah0Dhcanmanimations

3DS Files - Video Archive (CGFX_09h,0Ah,0Dh - CANM Animations) 
| |
| --- |

 - 3DS Files - Video Archive (CGFX_09h - CANM Skeleton animations)
 - 3DS Files - Video Archive (CGFX_0Ah - CANM Texture animations)
 - 3DS Files - Video Archive (CGFX_0Dh - CANM Light animations)
 CANM skeleton is found in some CGFX files, 3DS System Settings ExeFS:\banner
 CANM texture is found in some CGFX files, eg. 3DS Download Play ExeFS:\banner
 CANM light is found in some CGFX files, eg. 3DS Camera RomFS:\res\P_Finder_U
 
 All of those types do begin with ID "CANM", but unknown if they are actually using the same data structure(s).
 
 **CANM (for Skeleton Animations, and Texture Animations)**
 
```

| N/A - There is no leading Flags entry,
 000h 4 ID "CANM"
 004h 4 Revision (05000000h)
 008h 4 Name Offset (self-relative) eg. COMMON (same as in parent DICT)
 00Ch 4 Name Offset (self-relative) eg. SkeletalAnimation
 010h 4 Unknown (1)
 014h 4 Unknown (44160000h) (float32 maybe?)
 018h 4+4 DICT for ... whatever (NumEntries,Offset) (Offset=0Ch)
 020h 4+4 Unknown (0,0) |
| --- |

Note: In the above example, the name "COMMON" is used for both CFGX_00h and
CFGX_09h and CFGX_0Ah root entries.

The DICT in the CANM block(s) can contain items with various names, the
format(s) of that items are unknown.