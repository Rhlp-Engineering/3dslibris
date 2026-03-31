# Filessavedatadisaanddiff

> Source: https://problemkaputt.de/gbatek.htm
> Section: Filessavedatadisaanddiff

3DS Files - Savedata DISA and DIFF 
| |
| --- |

 DISA and DIFF are two container formats (usually containing SAVE/BDRI/VSXE filesystems, or raw data in some cases).
 The DISA/DIFF containers are storing everything in data pairs that consist of active/inactive halves (somewhat maintaining a backup copy in the active half, whilst allowing to write new data to the inactive half). Additionally, there are SHA256's for all data blocks (for error checking, and for preventing hacks/patches when not having the AES-CMAC key for the master checksum).
 
 **Duplicated Data Pairs**
 One can safely overwrite the inactive halves (whilst other tasks could still read the old intact data, and without risking to destroy the active data if the console crashes during writing), and finally apply the changes by toggling the active half flags.
 The disadvantages are that the duplicated data requires twice as much memory, crawling through the 7 levels does hurt read/write access time, writing the 7 levels does reduce the chip lifetime, the DIFI's and DPFS level 1-2 are using the same 200h-byte sector for active/inactive halves (so write errors on that sectors would also destroy the old active data).
 The whole concept is either blatant nonsense, or incredibly clever (in case Nintendo should have somehow estimated that the few benefits are outweighting the many disadvanges).
 
 **Variants with Duplicated or Non-duplicated Data**
 1) Single Partition with Internal Level 7 (duplicated data)
 2) Single Partition with External Level 7 (non-duplicated data)
 3) Dual Partition (one Internal and one External for partially-duplicated data)
 4) DIFF:VSXE directory file, with data stored in separate DIFF:data files
 
 
```

| XXX extdata\..\Quota.dat has external IVFC
 XXX extdata\..\00000000\00000002 has external IVFC |
| --- |

**Format**

A DISA / DIFF file consists of the following components:

```

| 0000h 200h Header (DISA or DIFF)
 0200h 130h 1st DIFI+IVFC+DPFS+SHA256 for Partition A
 (and somewhere DIFI+IVFC+DPFS+SHA256 for Partition B, if any?)
 0330h 130h 2nd DIFI+IVFC+DPFS+SHA256 for Partition A (as above)
 1000h ... Partition A
 xxxxh ... Partition B (optional, can only exist for DISA, not for DIFF). |
| --- |

**Encryption**

DISA and DIFF formats don't have their own encryption specification. They
follow the encryption method defined by their media:

```

| - Gamecard savegames have special wear leveling + encryption layers.
 - Files on SD follow the general SD filesystem encryption rule.
 - Files on NAND are in cleartext, after decrypting the NAND partition. |
| --- |

**DISA header (used for Savegame and System savedata)**

**DIFF header (for Extdata and Title Database)**

All offsets in the header are relative to the beginning of the DISA/DIFF file,
except for partition descriptor offsets, which are relative to the beginning of
the (active) partition table.

```

| DISA DIFF Siz Content
 000h 000h 10h AES-CMAC on SHA256 on special ID-strings-and-Header[100h..1FFh]
 010h 010h F0h Padding (0)
 100h 100h 8 ID ("DISA",00h,00h,04h,00h or "DIFF",00h,00h,03h,00h)
 108h - 4 Partition count, 1 or 2 (aka "1=A" or "2=A+B") (1)
 10Ch - 4 Padding
 110h 108h 8 1st DIFI offset ;\the active one is selected (200h)
 118h 110h 8 2nd DIFI offset ;/via below Active flag) (330h)
 120h 118h 8 DIFI+IVFC+DPFS+SHA256 table size (12Ch) (or 12Ch*2 when two?)
 128h - 8 Partition A DIFI+IVFC+DPFS+SHA256 offset in active DIFI (0)
 130h - 8 Partition A DIFI+IVFC+DPFS+SHA256 size (12Ch)
 138h - 8 Partition B DIFI+IVFC+DPFS+SHA256 offset in active DIFI (0) ;?
 140h - 8 Partition B DIFI+IVFC+DPFS+SHA256 size (0) ;12Ch?
 148h 120h 8 Partition A offset (DPFS flags, IVFC SHA's, ... (1000h)
 150h 128h 8 Partition A size (3F000h)
 158h - 8 Partition B offset (0)
 160h - 8 Partition B size (0)
 168h 130h 4 Active DIFI+IVFC+DPFS+SHA256 flag (0=2nd DIFI, 1=First DIFI)
 16Ch 134h 20h Active DIFI+IVFC+DPFS+SHA256 table SHA256
 - 154h 8 Unique identifier for Extdata (zero for title database)
 18Ch 15Ch .. Unused (up to 1FFh), might contain leftover data |
| --- |

When the partition count is 1, there is no partition B and all of its related
fields are zero.

**DIFI table**

All offsets are relative to the beginning of the DIFI partition descriptor,
except for External IVFC level 7 offset, which is relative to the beginning of
the partition.

```

| 000h 8 ID "DIFI",00h,00h,01h,00h
 008h 8 IVFC descriptor offset (44h) ;\checksum tree SHA256's ;"IVFC"
 010h 8 IVFC descriptor size (78h) ;/
 018h 8 DPFS descriptor offset (BCh) ;\active/inactive data pair flags
 020h 8 DPFS descriptor size (50h) ;/
 028h 8 SHA256 Partition hash offset (10Ch) ;\aka "Partition master hash"?
 030h 8 SHA256 Partition hash size (20h*N) ;/
 038h 1 IVFC level 7 location (0=Internal, Nonzero=External) (usually 0)
 039h 1 Active flag for DPFS Level 1 (0=1st half, 1=2nd half)
 03Ah 2 Padding (0)
 03Ch 8 External IVFC level 7 offset (zero if Internal IVFC level 7)
 044h 78h IVFC descriptor (see below)
 0BCh 50h DPFS descriptor (see below)
 10Ch 20h*N SHA256 Partition master hash list (SHA's for Level 4)
 (then followed by another DIFI+IVFC+DPFS+SHA256 for partition B, if any?) |
| --- |

Note: There are two DIFI tables (the active one being selected in
DISA[168h]/DIFF[130h]). Both DIFI's are usually containing the exact same
offsets and sizes (and do differ only by Flag in DIFI[039h] and the SHA's at
DIFI[10Ch]) (however, the inactive DIFI may initially contain unformatted
garbage).

**DPFS descriptor**

This header defines each level of DPFS tree. All offsets are relative to the
beginning of the partition.

```

| 000h 8 ID "DPFS",00h,00h,01h,00h
 008h 8 Level 1 offset (usually 0) ;\Flags for
 010h 8 Level 1 size per half (usually 4, or N*4 if bigger) ; Level 2
 018h 8 Level 1 block size (unused, usually 0) ;/
 020h 8 Level 2 offset (usually N*4*2, at level 1 end) ;\Flags for
 028h 8 Level 2 size per half (usually 80h*N) ; Level 3
 030h 8 Level 2 block size (1 SHL N, usually 7=80h bytes) ;/
 038h 8 Level 3 offset (usually 1000h-aligned) ;\IVFC's
 040h 8 Level 3 size per half (usually 1000h*N) ; (SHA's+Data)
 048h 8 Level 3 block size (1 SHL N, usually 12=1000h bytes) ;/ |
| --- |

The flags are stored in 32bit snippets, weirdly with bit31 being the 1st block
(that can be resolved as "BitNumber=BlockNumber XOR 1Fh"). The flags are
selecting the active half of the next level (0=1st Half, or 1=2nd Half).

The above DPFS "size" values refer to "one half" (the 1st half is at "offset",
the 2nd half is at "offset+size").

3DS Files - Savedata DISA and DIFF - Data Pairs

**IVFC descriptor**

This header defines each level of IVFC tree. All offsets are relative to

the beginning of the active half of DPFS level 3.

```

| 000h 8 ID "IVFC",00h,00h,02h,00h
 008h 8 Master hash size = partition master hash size in DIFI header (N*20h)
 010h 8 Level 4 offset (usually 0) ;\SHA's for
 018h 8 Level 4 size (usually 20h, or N*20h if bigger) ; Level 5
 020h 8 Level 4 block size (1 SHL N, usually 9=200h bytes) ;/
 028h 8 Level 5 offset (usually 20h, aka level 1 end) ;\SHA's for
 030h 8 Level 5 size (usually 20h, or N*20h if bigger) ; Level 6
 038h 8 Level 5 block size (1 SHL N, usually 9=200h bytes) ;/
 040h 8 Level 6 offset (usually 40h, aka level 2 end) ;\SHA's for
 048h 8 Level 6 size (usually 20h*N) ; Data
 050h 8 Level 6 block size (1 SHL N, usually 12=1000h bytes) ;/
 058h 8 Level 7 offset (unused for External IVFC Level 7) ;\Data (eg.
 060h 8 Level 7 size (usually 1000h*N) ; SAVE filesys)
 068h 8 Level 7 block size (1 SHL N, usually 12=1000h bytes) ;/
 070h 8 IVFC descriptor size? (usually 78h) |
| --- |

The above IVFC "size" values refer to "one half" (the 1st half is at "offset",
the 2nd half is at "offset+level3size").

3DS Files - Savedata DISA and DIFF - Checksums