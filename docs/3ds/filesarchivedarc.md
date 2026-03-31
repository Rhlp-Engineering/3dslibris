# Filesarchivedarc

> Source: https://problemkaputt.de/gbatek.htm
> Section: Filesarchivedarc

3DS Files - Archive DARC 
| |
| --- |

 DARC archives are used in various places, sometimes uncompressed, or often with LZSS or LZ11 compression. For example,
 
```

| Uncompressed DARC Manual.bcma file (e-manual)
 LZSS compressed DARC .arc files (nested within above .bcma file)
 LZ11 compressed DARC NCCH Logo (in NCCH Header, or NCCH ExeFS:\logo)
 LZ11 compressed DARC .arc.LZ files (eg. in 3DS Camera RomFS "lyt" folder) |
| --- |

The DARC format is about same as the Wii's U8 format (with different header,
different byte-order, and UTF-16 instead of ASCII names).

**DARC header**

```

| 000h 4 ID "darc"
 004h 2 Byte Order (FEFFh)
 006h 2 Header Size (001Ch)
 008h 4 Version (01000000h)
 00Ch 4 Total Filesize (excluding trailing 20h-byte SHA256-HMAC, if any)
 010h 4 Offset of File/Folder List (usually 1Ch)
 014h 4 Size of File/Folder List, PLUS File/Folder Name Area
 018h 4 Offset of File Data Area
 .. .. File/Folder List (0Ch bytes per file/folder)
 .. .. File/Folder Name Area (UTF-16)
 .. .. File Data Area (usually each file is 20h-byte aligned)
 (..) (20h) Trailing SHA256-HMAC, if any (this is NOT 20h-byte aligned) |
| --- |

**Root/Dot Entries (first 1-2 entries in File/Folder List)**

```

| Entry0: Type=Folder, Name="", Offset=0, End=TotalNumDirEntries ;Root
 Entry1: Type=Folder, Name=".", Offset=0, End=TotalNumDirEntries ;Dot (if any) |
| --- |

Entry0/Root is important to know the total directory size, and to compute the
begin of the Name Area:

```

| NameArea = FileFolderList + TotalNumDirEntries*0Ch |
| --- |

Entry1/Dot is somewhat useless, but it does exist in most (not all) darc files,
ie. the path strings can appear as so:

```

| darc:\.\dir\subdir\file ;<-- ugly, but used in most darc files
 darc:\dir\subdir\file ;<-- nicer, used in a few darc files |
| --- |

It's best to hide the "." in path strings (ie. detect if Entry1 is a Folder,
with name "." and same Offset/End as Entry0, and if so, skip/ignore it).

**File/Folder List Entries (0Ch bytes per entry)**

```

| 000h 3 Name offset (24bit, relative to begin of Name Area)
 003h 1 Type (00h=File, 01h=Folder)
 When Type=00h=File:
 004h 4 File offset (relative to begin if darc; NOT to begin of Data Area)
 008h 4 File size
 When Type=01h=Folder:
 004h 4 Folder Parent Index (0=Root, 1=Dot, Other=NestedSubdir's)
 008h 4 Folder End Index (entries up to excluding End Index are childs) |
| --- |

**Special Cases**

3DS Face Raiders RomFS:\hal\effect\kbb\kbb.dat doesn't have a "." dot folder.

3DS Mii Maker RomFS:\shaders\shader.arc has nested folders & empty folders.

3DS Activity Log RomFS:\P_UlcdTest_LZ.bin contains an empty darc.

**Example**

```

| 64 61 72 63 ;id "darc"
 FF FE 1C 00
 00 00 00 01
 28 65 00 00
 1C 00 00 00
 1C 04 00 00
 40 04 00 00
 00 00 00 01 00 00 00 00 15 00 00 00 ;00h folder ""
 02 00 00 01 00 00 00 00 15 00 00 00 ;01h folder "."
 06 00 00 01 01 00 00 00 09 00 00 00 ;02h folder "anim";\
 10 00 00 00 40 04 00 00 F8 00 00 00 ;03h ;
 54 00 00 00 40 05 00 00 EC 00 00 00 ;04h ;
 98 00 00 00 40 06 00 00 F8 00 00 00 ;05h ;
 DC 00 00 00 40 07 00 00 CC 0C 00 00 ;06h ;
 20 01 00 00 20 14 00 00 54 0C 00 00 ;07h ;
 64 01 00 00 80 20 00 00 4C 02 00 00 ;08h ;/
 A8 01 00 01 01 00 00 00 0C 00 00 00 ;09h folder "..." ;\
 B2 01 00 00 E0 22 00 00 A8 02 00 00 ;0Ah ;
 E2 01 00 00 A0 25 00 00 3C 10 00 00 ;0Bh ;/
 12 02 00 01 01 00 00 00 15 00 00 00 ;0Ch folder "..." ;\
 1C 02 00 00 00 36 00 00 28 04 00 00 ;0Dh ;
 3E 02 00 00 80 3A 00 00 28 01 00 00 ;0Eh ;
 60 02 00 00 00 3C 00 00 28 04 00 00 ;0Fh ;
 82 02 00 00 80 40 00 00 28 08 00 00 ;10h ;
 A4 02 00 00 00 49 00 00 A8 00 00 00 ;11h ;
 C4 02 00 00 00 4A 00 00 28 08 00 00 ;12h ;
 DC 02 00 00 80 52 00 00 28 02 00 00 ;13h ;
 F4 02 00 00 00 55 00 00 28 10 00 00 ;14h ;/
 00 00 ;name "",0000h
 2E 00 00 00 ;name ".",0000h
 61 00 6E 00 69 00 6D 00 00 00 ;name "anim",0000h
 ... |
| --- |