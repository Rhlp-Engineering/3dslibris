# Filesvideoarchivecgfx08Hcenvenvironments

> Source: https://problemkaputt.de/gbatek.htm
> Section: Filesvideoarchivecgfx08Hcenvenvironments

3DS Files - Video Archive (CGFX_08h - CENV Environments) 
| |
| --- |

 CENV is found in some CGFX files, eg. 3DS System Settings ExeFS:\banner (with Unknown2) or Mii Maker ExeFS:\banner (with Unknown1, Unknown2, Unknown3)
 
 **CENV Format**
 
```

| 000h 4 Flags (eg. 00800000h)
 004h 4 ID "CENV"
 008h 4 Revision (01000000h)
 00Ch 4 Name Offset (self-relative) (same as in parent DICT)
 010h 4+4 Unknown0 (0,0)
 018h 4+4 Unknown1 (0,0) (or 1,14h) ;(Num,Offset ?)
 020h 4+4 Unknown2 (1,0Ch) (or 2,10h) ;(Num,Offset ?)
 028h 4+4 Unknown3 (0,0) (or 1,10h) ;(Num,Offset ?)
 ... 4*n1 Offset List for Unknown1 ?
 ... 4*n2 Offset List for Unknown2 ?
 ... 4*n3 Offset List for Unknown3 ? |
| --- |

Entries for Unknown1 (if any):

```

| 000h 4 Unknown (0)
 004h 4 Corresponding name? (eg. Offset to "FragmentOperation")
 008h 4 Unknown (0)
 00Ch 4 Unknown (0) |
| --- |

Entries for Unknown2:

```

| 000h 4 Unknown (1)
 004h 4 Unknown (4)
 008h 4 Unknown (4)
 00Ch 4 Unknown (0)
 010h 4 Corresponding Light name? (eg. Offset to Name "Light1")
 014h 4 Unknown (0) |
| --- |

Entries for Unknown3 (if any):

```

| ... .. ? |
| --- |