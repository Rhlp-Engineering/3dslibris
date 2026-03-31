# Filesvideoarchivecgfx05Hccamcameras

> Source: https://problemkaputt.de/gbatek.htm
> Section: Filesvideoarchivecgfx05Hccamcameras

3DS Files - Video Archive (CGFX_05h - CCAM Cameras) 
| |
| --- |

 CCAM is found in some CGFX files (eg. 3DS Sound RomFS:\res\S.pack).
 
 **CCAM Format**
 
```

| 000h 4 Flags (eg. 4000000Ah)
 004h 4 ID "CCAM"
 008h 4 Revision (07000000h)
 00Ch 4 Name Offset (self-relative) (same as in parent DICT)
 010h 4+4 Unknown (0,0)
 018h 4+4 Unknown (1,1)
 020h 4+4 Unknown (0,0)
 028h 4+4 DICT for Camera Animation (NumEntries,Offset)
 030h 0Ch Vector? (3xFloat32) (1.0, 1.0, 1.0)
 ... .. More Vectors and/or Matrices ?
 0BCh 4 Unknown (1D4h)
 0C0h 4 Unknown (1E8h)
 0C4h 4 Unknown (0) |
| --- |

**Camera Animation (from DICT in CCAM) (eg. Name=CameraAnimation)**

```

| 000h 4 Unknown (80000000h)
 004h 4 Unknown (0)
 008h 4 Name Offset (self-relative) (same as in parent DICT)
 00Ch 4 Unknown (5)
 010h 4+4 DICT for whatever (NumEntries,Offset)
 018h 4 Unknown (3)
 01Ch 4 Unknown (8)
 020h 4 Unknown (0)
 024h 4 Unknown (8)
 028h 4 Unknown (6)
 02Ch 4 Unknown (2) |
| --- |

The DICT can have several entries (with names ProjectionUpdater.AspectRatio,
ProjectionUpdater.Far, ProjectionUpdater.Fovy, ProjectionUpdater.Near,
Transform, ViewUpdater.TargetPosition, ViewUpdater.Twist). The format(s) of
that entries are unknown.