# Filesarchivezar

> Source: https://problemkaputt.de/gbatek.htm
> Section: Filesarchivezar

3DS Files - Archive ZAR 
| |
| --- |

 **ZAR Format (Zelda Archive, used in 3DS Zelda Ocarina of Time)**
 
```

| 000h 4 ID "ZAR",01h ;\
 004h 4 Total Filesize ;
 008h 2 Number of Filetype Entries (T) ;
 00Ah 2 Number of Files (of any type) (F) ; header
 00Ch 4 Offset to Filetypes section (20h) ;
 010h 4 Offset to Filesize/name section ;
 014h 4 Offset to Fileoffset section ;
 018h 8 ID "queen",0,0,0 ;/
 020h T*10h Filetype Entries ;\type list
 ... .. Filetype Names and File List for each Filetype ;/
 ... F*8 Filesize/Filename Entries ;\
 ... .. Filenames (ASCII, terminated by zero) ; file list
 ... F*4 Fileoffset Entries ;/
 ... .. File data... ;-file data |
| --- |

**Filetypes Entries**

```

| 000h 4 Number of Files for the Filetype (0=None)
 004h 4 Offset to File List for the Filetype (FFFFFFFFh=None)
 008h 4 Offset to Filetype name (eg. "unknown",0 or "zsi",0 etc.)
 00Ch 4 Unknown (always FFFFFFFFh) |
| --- |

The File List contains a list of 32bit File indices for the Filetype (0=1st
file, 1=2nd file, etc).

The number of filetypes varies between 2 and 8 in known files. Known filetype
names are:

```

| "anb",00h boy/.../*.anb ;eg. in actor\zelda_link_child_ultra.zar
 "ccb",00h ?
 "cmab",00h misc/.../*.cmab
 "cmb",00h Model/.../*.cmb (or lowercase, model/.../*.cmb)
 "csab",00h Anim/.../*.csab
 "ctxb",00h tex/.../*.ctxb
 "faceb",00h boy/.../*.faceb ;eg. in actor\zelda_link_opening.zar
 "qdb",00h demo/.../*.qdb ;eg. in actor\zelda_horse.zar, scene\*.zar
 "tbd",00h txt/.../*.tbd ;eg. in kankyo\kankyo_common.zar
 "unknown",00h Anim/.../*.cskla ;eg. in actor/zelda_magic_wind.zar
 "zsi",00h collision/.../*.zsi |
| --- |

Type "unknown" does always exist as first type entry (even if it contains no
files; the only archive containing an "unknown" file is
actor/zelda_magic_wind.zar, which contains Anim/new_magic_wind.cskla).

**Filesize/Filename Entries**

```

| 000h 4 File size
 004h 4 Offset to filename (ASCII, terminated by zero) |
| --- |

**Fileoffset Entries**

```

| 000h 4 Offset to File data |
| --- |

**Directories**

The ZAR format doesn't directly support directories. However, most filenames
contain slashes so one could split those into folder/filename.

Note: RomFS:\scene\*.zar does use "\" backslashes instead of "/" slashes, that,
with some names starting with dot or dotdot (".\" or "..\..\").

If desired, one could also treat the ZAR's Filetypes section as top-level
directories, or as filetype suffix (although "Model" and ".cmb" in filenames
like "Model\filename.cmb" do already imply the type anyways).

**Note**

See [https://wiki.cloudmodding.com/oot/Category:File_Formats](https://wiki.cloudmodding.com/oot/Category:File_Formats)
 for more Zelda fileformats (QBF, QCL, QM, etc).