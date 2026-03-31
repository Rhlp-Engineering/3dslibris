# Filessavedatasavebdrivsxeformats

> Source: https://problemkaputt.de/gbatek.htm
> Section: Filessavedatasavebdrivsxeformats

3DS Files - Savedata SAVE/BDRI/VSXE Formats 
| |
| --- |

 **SAVE/BDRI/VSXE Header**
 
```

| 000h 4 ID "SAVE",00h,00h,04h,00h ;for savegame (file names + data clusters)
 ID "BDRI",00h,00h,03h,00h ;for database (file IDs + data clusters)
 ID "VSXE",00h,00h,03h,00h ;for extdata (file names + data IDs)
 008h 8 File System Information offset (SAVE/BDRI=20h or VSXE=138h)
 010h 8 Image size in blocks (eg. 1Eh)
 018h 4 Image block size (usually 1000h) (or 80h or 200h, in databases)
 01Ch 4 Padding
 Extra entries, for VSXE (extdata) only:
 020h 8 Unknown (can be 1) (1)
 028h 4 Most recently mounted Extdata image 'Action' ;whut? (0)
 02Ch 4 Unknown (0)
 030h 4 Most recently mounted Extdata image FAT file (eg. 3="00000003")
 034h 4 Unknown (0)
 038h 100h Most recently mounted Extdata image VSXE name (eg. "/user/file",0)
 Note: The Quota.dat file does also seem to log some "most recent" stuff. |
| --- |

**File System Information (usually at offset 20h or 138h)**

```

| 000h 4 Unknown (0)
 004h 4 Cluster size (usually 1000h) (or 80h or 200h, in databases)
 008h 8 Directory hash table offset (eg. 88h, ie. after SAVE header)
 010h 4 Directory hash table bucket count (eg. 0Bh)
 014h 4 Padding (0) (or 04h in BDRI?)
 018h 8 File hash table offset (eg. B4h, ie. after above table)
 020h 4 File hash table bucket count (eg. 0Bh)
 024h 4 Padding (0) (or 04h in BDRI?)
 028h 8 File allocation table offset (eg. E0h)
 030h 4 File allocation table entry count (eg. 1Dh = Cluster 0..1Dh)
 034h 4 Padding (0) (or 04h in BDRI?)
 Single Partition (normal case):
 038h 8 Cluster 1 offset (usually 1000h) ;<--
 040h 4 Num clusters (eg. 1Dh = Cluster 1..1Dh)
 044h 4 Padding (0)
 048h 4 Directory table first cluster-1 (usually 0, ie. Cluster 1) ;<--
 04Ch 4 Directory table num clusters (usually 1) ;<--
 050h 4 Max number of subdirectories (0..N) (eg. 0Ah=Dummy+Root+10 Sub's)
 054h 4 Padding (0)
 058h 4 File table first cluster-1 (usually 1, ie. Cluster 2) ;<--
 05Ch 4 File table num clusters (usually 01h) ;<--
 060h 4 Max number of files (eg. 0Ah=Dummy+10 Files?)
 064h 4 Padding (0)
 Dual Partition (more tightly packed):
 038h 8 Cluster 1 start (always 0, start in partition B) ;<--
 040h 4 Num clusters (eg. 1Dh = Cluster 1..1Dh)
 044h 4 Padding (0)
 048h 8 Directory table offset ;<--
 050h 4 Max number of subdirectories (0..N) (eg. 0Ah=Dummy+Root+10 Sub's)
 054h 4 Padding (0)
 058h 8 File table offset ;<--
 060h 4 Max number of files (eg. 0Ah=Dummy+10 Files?)
 064h 4 Padding (0) |
| --- |

When partition B doesn't exist, directory & file entry tables are allocated
in the data region, and will be marked as allocated in file allocation table as
if they are two normal files (only continuous allocation has been observed for
these tables, so directly reading "block_count * block_size" bytes from
"data_region + starting_block_index * block_size" should be safe).

**Directory Table**

```

| Entry #0 Dummy Entry (aka entrypoint for unused (deleted?) entries)
 Entry #1 Root Directory (aka entrypoint for used entries)
 Entry #2..X Sub-directories (used and deleted entries?)
 Entry #Y..Z Sub-directories (still unused entries?) (zerofilled) |
| --- |

Directory Entries:

```

| SAVE VSXE BDRI Siz Content
 000h 000h 000h 4 Directory Index of Parent directory (0=none; in root)
 004h 004h - 16 Directory Name in ASCII, zeropadded (all zero for root)
 014h 014h 004h 4 Directory Index of next sibling directory (0=none)
 018h 018h 008h 4 Directory Index of first subdirectory (0=none)
 01Ch 01Ch 00Ch 4 File index of first file in this directory (0=none)
 020h 020h 010h .. Padding (0) / zero?
 024h 024h 01Ch 4 Directory Index of next same hash table bucket (0=none) |
| --- |

Dummy Directory entries:

```

| SAVE VSXE BDRI Siz Content
 000h 000h 000h 4 Current Total entry count (eg. 2 = Dummy+Root)
 004h 004h 004h 4 Maximum entry count = maximum directory count + 2
 008h 008h 008h .. Padding / All zero
 024h 024h 01Ch 4 Directory Index of next dummy (=deleted?) entry (0=none) |
| --- |

Dummy entries are left there when deleting directories, and reserved for future
use.

**File Table**

```

| Entry #0 Dummy Entry
 Entry #1..X File Entries (used and deleted entries?)
 Entry #Y..Z File Entries (still unused entries?) (zerofilled) |
| --- |

The file entry table is an array of the entry type shown below. It contains
information for each file.

```

| SAVE VSXE BDRI Siz Content
 000h 000h 000h 4 Directory index of Parent Directory
 004h 004h - 16 File Name in ASCII, zeropadded
 - - 004h 8 Title ID
 014h 014h 00Ch 4 File index of next sibling file (0=none)
 018h 018h 010h 4 Padding
 01Ch - 014h 4 First cluster-1 (0..N-1 = Cluster 1..N, or 80000000h=None)
 020h - 018h 8 File Size
 - - 020h 4 Padding?
 - 01Ch - 4 Always 80000000h (extdata has no clusters)
 - 020h - 8 Unique identifier. See Extdata
 028h 028h 024h 4 Padding?
 02Ch 02Ch 028h 4 File Index of next file in same hash table bucket (0=none) |
| --- |

Dummy File entries:

```

| SAVE VSXE BDRI Siz Content
 000h 000h 000h 4 Current total entry count (eg. 2=Dummy+OneFile)
 004h 004h 004h 4 Maximum File entry count = maximum file count + 1
 008h 008h 008h .. Padding (0)
 02Ch 02Ch 028h 4 File Index of the next dummy (=deleted?) entry (0=last) |
| --- |

Dummy entries are left there when deleting files, and reserved for future use.

**File Allocation Table (FAT)**

The FAT uses 64bit entries, containing the next & previous cluster number
(unlike microsoft FATs, which contain next cluster only).

```

| 0-30 Previous Cluster (1..max, or 0=None)
 31 Blah flag (0=No, 1=First cluster, or Second/Last entry of large block)
 32-62 Next Cluster (1..max, or 0=None)
 63 Multiple flag (0=No, 1=First entry of Large block) |
| --- |

The Multiple flag indicates a large block that consists of more than one
cluster. The first entry in the large block has Multiple=1, the second and last
entry of the large block contain the first/last cluster numbers of the large
block.

```

| For example, a file using cluster 20, 100-120, 50 (in that order):
 File List: Cluster=19 (aka 20-1) ;-first cluster of file
 FAT[20]: Prev=0, Next=100, Blah=1 ;-cluster 20
 FAT[100]: Prev=20, Next=50, Multiple=1 ;\
 FAT[101]: Prev=100, Next=120, Blah=1 ; cluster 100..120
 FAT[102..119]: Zero (or garbage?) ;
 FAT[120]: Prev=100, Next=120, Blah=1 ;/
 FAT[50]: Prev=100, Next=0 ;-cluster 50 |
| --- |

All free blocks that are not allocated to any files also form a node chain in
the allocation table, with starting in FAT[0]:

```

| FAT[0]: Prev=0, Next=FirstFree ;-points to begin of free chain |
| --- |

**Directory Hash Table & File Hash Table**

```

| 000h 4 File/Directory Index of 1st name with hash=0
 004h 4 File/Directory Index of 1st name with hash=1
 008h 4 File/Directory Index of 1st name with hash=2
 etc. |
| --- |

The hash depends on parent_directory_index and file/directory name as key:

```

| hash = parent_dir_index XOR 091A2B3Ch ;-parent mangled with digits 0123+9ABC
 hash = (hash ROR 1) XOR name[00h] ;\
 hash = (hash ROR 1) XOR name[04h] ; 16-byte file/directory name
 hash = (hash ROR 1) XOR name[08h] ; (including trailing 00h's)
 hash = (hash ROR 1) XOR name[0Ch] ;/
 hash = (hash MOD bucket_count) ;-final value |
| --- |

Uh, but, that cannot be true for BDRI...

```

| BDRI doesn't have any file names (instead has 8-byte Title IDs)
 BDRI doesn't have any directory names |
| --- |

**Layout for duplicate data = true**

The DISA container only has one partition which is always configured as
external IVFC level 7 disabled (see DISA format for details). All components
are stored in this partition as

```

| Partition A
 SAVE/BDRI/VSXE header
 Directory hash table
 File hash table
 File allocation table (FAT)
 data region
 directory entry table is allocated inside data region ;Cluster 1
 file entry table as well ;Cluster 1+x
 all file data is also allocated here ;Cluster 1+x+y
 Partition B
 none such |
| --- |

In this layout, all data is duplicated by DISA's DPFS tree, which is what the
parameter duplicate data implies.

**Layout for duplicate data = false**

The DISA container has two partitions. Partition A is always configured as
external IVFC level 7 disabled, and partition B is configured as it enabled.
Components are stored among the two partitions as

```

| Partition A
 SAVE header at the beginning
 Directory hash table
 File hash table
 File allocation table (FAT)
 directory entry table
 file entry table
 Partition B
 used as data region entirely, and only has file data allocated. ;Cluster 1 |
| --- |

In this layout, all file system metadata is duplicated by partition A DPFS
tree, but file data is not as partition B has external IVFC level 7.

**Initialization**

When a save FLASH contains all xFFFF blocks it's assumed uninitialized by the
game cartridges and it initializes default data in place, without prompting the
user. The FFFFFFFFh blocks are uninitialized data. When creating a non-gamecard
savegame and other images/files, it's initially all FFFFFFFFh until it's
formatted where some of the blocks are overwritten with encrypted data.

**Tools**

3dsfuse supports reading and modifying savegames. In the mounted FUSE
filesystem, the /output.sav is the raw FLASH save-image. When the save was
modified, a separate tool to update the CMAC must be used with /clean.sav,
prior to writing output.sav to a gamecard.

3DSExplorer supports reading of savegames, it doesn't support reading the new
encrypted savegames and maybe in the future it will support modifying (some of
the modyfing code is already implemented).

wwylele's 3ds-save-tool supports extracting files from savegames and extdata.
It properly reconstructs data from the DPFS tree and extracts files in
directories hierarchy.

3dsfuse-ex similar to 3dsfuse, but supports savegame inner FS, proper DPFS
handling, and automatic CMAC update. Still WIP.