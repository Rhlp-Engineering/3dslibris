# = 1..C hex) |

> Source: https://problemkaputt.de/gbatek.htm
> Section: = 1..C hex) |

DSi SD/MMC Filesystem 
| |
| --- |

 **DSi Partition Table and FAT Filesystems**
 DSi SD/MMC Partition Table (aka Master Boot Record aka MBR)
 DSi SD/MMC Filesystem (FAT)
 System Tools and DSiware games are having regular "ROM Cartridge" headers, with NitroROM filesystem defined in that headers (ie. the ".app" files are internally containing a 2nd filesystem inside of the FAT filesystem; eventually there can be also NARC files as 3rd filesystem inside of the NitroROM filesystem).
 DSi Cartridge Header
 DS Cartridge NitroROM and NitroARC File Systems
 Savedata for DSiWare games is usually stored in "public.sav" or "private.sav" files. That .sav files are usually containing a FAT12 with its own VBR, FAT, and Directories (so they use some virtual FAT12 inside of the real FAT16).
 
 **DSi Filesystem Overview**
 DSi SD/MMC Internal NAND Layout
 DSi SD/MMC Bootloader
 DSi SD/MMC Device List
 DSi SD/MMC Complete List of SD/MMC Files/Folders
 DSi SD/MMC Summary of SD/MMC Files/Folders
 DSi SD/MMC Images
 DSi Autoload on Warmboot
 
 **DSi Files**
 DSi SD/MMC DSiware Files on Internal eMMC Storage
 DSi SD/MMC DSiware Files on External SD Card (.bin aka Tad Files)
 DSi SD/MMC DSiware Files from Nintendo's Server
 DSi SD/MMC DSiware Tickets and Title metadata
 DSi SD/MMC Firmware dev.kp and cert.sys Certificate Files
 DSi SD/MMC Firmware Certificate/Keys (DER)
 DSi SD/MMC Firmware Font File
 DSi SD/MMC Firmware Log Files
 DSi SD/MMC Firmware Misc Files
 DSi SD/MMC Firmware Wifi Firmware
 DSi SD/MMC Firmware System Settings Data Files
 DSi SD/MMC Firmware Version Data File
 DSi SD/MMC Firmware Nintendo DS Cart Whitelist File
 DSi SD/MMC Camera Files - Overview
 DSi SD/MMC Camera Files - JPEG's
 DSi SD/MMC Camera Files - pit.bin
 DSi SD/MMC Flipnote Files
 DS File Formats
 
 
 
| DSi SD/MMC Partition Table (aka Master Boot Record aka MBR) |
| --- |

 **DSi eMMC Partition table**
 The decrypted DSi MBR contains (for 240MB chips; 3rd part differs for 245.5MB):
 
```

| 0000 00 00 00 00 00 00 00 00 .. .. .. .. 00 00 ;bootcode (zero)
 01BE 00 03 18 04 06 0F E0 3B 77 08 00 00 89 6F 06 00 ;1st partition (main)
 01CE 00 02 CE 3C 06 0F E0 BE 4D 78 06 00 B3 05 01 00 ;2nd partition (photo)
 01DE 00 02 DE BF 01 0F E0 BF 5D 7E 07 00 A3 01 00 00 ;3rd partition (extra)
 01EE 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 ;4th partition (none)
 01FE 55 AA ;mbr id (55h,AAh) |
| --- |

Above values are following the classical standard MBR format:

```

| 000h 446 bootcode (zerofilled on DSi) ;-bootcode
 1BEh+n*10h 1 status (00h) ;\
 1BFh+n*10h 3 chsFirst ; four
 1C2h+n*10h 1 type (00h=unused, 01h=FAT12, 06h=FAT16B) ; partitions
 1C3h+n*10h 3 chsLast ; (n=0..3)
 1C6h+n*10h 4 lbaFirst ;\logical block addresses/sizes ;
 1CAh+n*10h 4 lbaSize ;/counted in 200h-byte sectors ;/
 1FEh 2 mbrsig (55h,AAh) ;-MBR ID |
| --- |

The CHS and LBA values are essentially containing the same information (CHS
being an older standard, and LBA being invented in 1996). The 24bit CHS values
are encoded as:

```

| 0-7 Head Bit0-7 (00h..FEh) (or less common, 00h..FFh)
 8-13 Sector Bit0-5 (01h..3Fh)
 14-15 Cylinder Bit8-9
 16-23 Cylinder Bit0-7 (000h..3FFh, with above bit8-7) |
| --- |

To convert CHS to LBA, one must know the number of (logical) heads and sectors
per cylinder (that info isn't stored in the MBR). The DSi's eMMC uses 32
sectors, 16 heads, 1024 cylinders:

```

| LBA = (Cylinder*32*16) + (Head*32) + (Sector-1) |
| --- |

Anyways, it's better/easier to use the LBA values directly, and ignore the CHS
values.

**3DS eMMC Partition table**

The 3DS does have a DSi compatible MBR (with main & photo partitions, as
shown above). The additional 3DS partitions are accessed via a separate "NCSD"
header (with 3DS specific encryption keys, which aren't available in DSi mode).

**SD/MMC Card Partition table**

SD/MMC Cards are shipped with pre-formatted filesystem, so they should stick to
some standarized MBR variant, with only a single partition used, possibly with
additional date/id fields(?).

Note that SD/MMC Cards may or may not use partition tables (see CSD Register,
FILE_FORMAT entry).

**Formatting and Reformatting**

SD/MMC cards are usually pre-formatted with empty filesystems. Reformatting
should be avoided, or should be done only with tools that are aware of some
memory card specific requirements:

The cluster size should not be less than the physical sector size (otherwise,
when writing smaller clusters, sectors may get erased multiple times, which
would result in slower writing and reduced lifetime).

Many devices do support only the standard FAT filesystems (for example, cameras
or mp3-players may be unable to access memory cards with NTFS filesystem).

**More info**

For more info on MBR variants (and on the Partition Type value at 1C2h+n*10h),
see:

```
[http://en.wikipedia.org/wiki/Master_Boot_Record](http://en.wikipedia.org/wiki/Master_Boot_Record)[http://en.wikipedia.org/wiki/Partition_type](http://en.wikipedia.org/wiki/Partition_type)
| <-- rather meaningless |
| --- |

| DSi SD/MMC Filesystem (FAT) |
| --- |

**Volume Boot Record (VBR) (FAT12/FAT16) (DOS 4.0 EBPB)**

```

| 000h 3 80x86 jump opcode (DSi: E9h,00h,00h)
 003h 8 ascii disk name (DSi: "TWL ")
 00Bh 2 bytes / sector (DSi: 0200h)
 00Dh 1 sectors / cluster (DSi: 20h)
 00Eh 2 sectors / boot-record (DSi: 0001h)
 010h 1 number of FAT-copys (DSi: 02h)
 011h 2 entrys / root-directory (DSi: 0200h)
 013h 2 sectors / disk (DSi: 0000h)
 015h 1 ID (DSi: F8h=HDD)
 016h 2 sectors / FAT (DSi: A:0034h, B:0009h)
 018h 2 sectors / track (DSi: 0020h)
 01Ah 2 heads / disk (DSi: 0010h)
 01Ch 2 number of reserved sectors (DSi: None such entry!)
 01Ch 4 LBA First "hidden" (DSi: A:00000877h, B:0006784Dh)
 020h 4 LBA Size (total sectors)(DSi: A:00066F89h, B:000105B3h)
 024h 1 Drive Number (DSi: A:00h, B:01h)
 025h 1 Flags (reserved) (DSi: 00h)
 026h 1 EBPB Version (DSi: 29h) (that is, DOS 4.0 EBPB)
 027h 4 Volume Serial Number (DSi: 12345678h)
 02Bh 11 Volume Label (DSi: " ")
 036h 8 Filesystem Type (DSi: 00h-filled)
 03Eh 448 Bootcode (DSi: 00h-filled)
 1FEh 2 Signature (DSi: 55h,AAh) |
| --- |

SDHC carts use FAT32, which differs in entries 011h, 016h and 024h-059h:

```

| 011h 2 Must be 0 (number of root entries, is variable-length FAT chain)
 016h 2 Must be 0 (sectors per fat, instead use 32bit value at 024h)
 024h 4 sectors / FAT (new 32bit value instead of old entry 016h)
 028h 2 ExtFlags (related to "active" FAT copy)
 02Ah 2 Version of FAT32 filesystem (minor, major) (should be 0.0)
 02Ch 4 Cluster number of first Root directory cluster (usually/often 2)
 030h 2 Sector number of FSINFO in reserved area (usually 0001h)
 032h 2 Sector number of VBR backup copy in reserved area (usually 0006h)
 034h 12 Reserved for future ;Should be zerofilled
 040h 1 Drive Number ;\
 041h 1 Flags (reserved) ; as old
 042h 1 EBPB Version ;Must be 29h (that is, DOS 4.0 EBPB) ; entries
 043h 4 Volume Serial Number ; at 024h
 047h 11 Volume Label ;
 052h 8 Filesystem Type ;Must be "FAT32 " ;/ |
| --- |

Moreover, FAT32 has the "FSINFO" at sector number VBR[30h]:

```

| 000h 4 Value 41615252h (aka "RRaA")
 004h 480 Reserved (should be 0)
 1E4h 4 Value 61417272h (aka "rrAa")
 1E8h 4 Hint on number of free clusters (or FFFFFFFFh=unknown)
 1ECh 4 Hint on first free cluster number (or FFFFFFFFh=unknown)
 1F0h 12 Reserved (should be 0)
 1FCh 4 Value AA550000h |
| --- |

For more info see [http://en.wikipedia.org/wiki/BIOS_Parameter_Block](http://en.wikipedia.org/wiki/BIOS_Parameter_Block)
 and fatgen103.pdf (official specs from microsoft).

**File Allocation Table - FAT and FAT copy(s)**

The following sectors are occupied by the File Allocation Table (FAT), which
contains 12bit, 16bit, or 32bit entries (for FAT12/16/32) for each cluster:

```

| (x000)(0)000 unused, free
 (x000)(0)001 ???
 (x000)(0)002... pointer to next cluster in chain (0)002..(F)FEF
 (xFFF)(F)FF0-6 reserved (no part of chain, not free)
 (xFFF)(F)FF7 defect cluster, don't use
 (xFFF)(F)FF8-F last cluster of chain |
| --- |

The "x" in MSB of FAT32 entries is reserved (ie. FAT32 is restricted to 28bit
cluster numbers).

Number and size of FATs can be calculated by the information in the boot
sector.

Note on The first two FAT entries:

"The first cluster of the data area is cluster #2. That leaves the first two
entries of the FAT unused. In the first byte of the first entry a copy of the
media descriptor is stored. The remaining bits of this entry are 1. In the
second entry the end-of-file marker is stored. The high order two bits of the
second entry are sometimes, in the case of FAT16 and FAT32, used for dirty
volume management: high order bit 1: last shutdown was clean; next highest bit
1: during the previous mount no disk I/O errors were detected.

(Historically this description has things backwards: DOS 1.0 did not have a
BIOS Parameter Block, and the distinction between single-sided and double-sided
5.25" 360K floppies was indicated by the first byte in the FAT. DOS 2.0
introduced the BPB with media descriptor byte.)"

**Root directory**

The following sectors are the Root directory, again, size depends on the info
in bootsector (on FAT32 is it's a normal cluster chain with variable size).
Each entry consists of 32 bytes:

```

| 00-07 8 Filename (first byte: 00=free entry, 2E=dir, E5=deleted entry)
 08-0A 3 Filename extension
 0B 1 Fileattribute
 bit0 read only
 bit1 hidden
 bit2 system
 bit3 volume label
 bit4 subdirectory
 bit5 archive-flag
 bit6 reserved
 bit7 reserved
 0C-0D 2 Reserved, or stuff
 0E-0F 2 Reserved, or Creation Timestamp
 10-11 2 Reserved, or Creation Datestamp
 12-13 2 Reserved, or Last Access Datestamp
 14-15 2 Reserved, or MSBs of Cluster (for FAT32)
 16-17 2 Last Modify Timestamp: HHHHHMMM, MMMSSSSS
 18-19 2 Last Modify Datestamp: YYYYYYYM, MMMDDDDD
 1A-1B 2 Pointer to first Cluster of file
 1C-1F 4 Filesize in bytes (always 0 for directories) |
| --- |

The 'cluster' entry points to the first used cluster of the file. The FAT entry
for that cluster points to the next used cluster (if any), the FAT entry for
that cluster points to the next cluster, and so on.

**Long File Names (LFNs)**

Long File Names (LFNs) are occupying multiple continous directory entries,
consisting of a normal short filename entry, preceeded by one or more LFN
prefix entries (with Attribute=0Fh). Each LFN prefix can hold 13 characters,
the total length should not exceed 255 characters. The name should be
terminated by 0000h, and any remaining unused characters should be padded by
FFFFh. The LFN prefix entries are using the following format:

```

| 00h 1 Sequence Number (bit6: last logical, first physical LFN entry,
 bit5: 0, bit4-0: number 01h..14h (1Fh)) (or E5h=deleted entry)
 01h 10 Long Filename characters (five UCS-2 characters)
 0Bh 1 Attributes (always 0Fh for LFN prefix)
 0Ch 1 Type (always 00h)
 0Dh 1 Short Filename Checksum
 sum=00h, for i=0 to 10, sum = (sum ROR 1) + shortname_char[i], next i
 0Eh 12 Long Filename characters (six UCS-2 characters)
 1Ah 2 First cluster (always 0000h)
 1Ch 4 Long Filename characters (two UCS-2 characters) |
| --- |

For example, "File with very long filename.ext" would be formatted as so:

```

| Entry 1: LFN Prefix (43h) "me.ext", 0000h, 6xFFFFh
 Entry 2: LFN Prefix (02h) "y long filena"
 Entry 3: LFN Prefix (01h) "File with ver"
 Entry 4: Normal 8.3 short filename entry "FILEWI~1.EXT" |
| --- |

[http://en.wikipedia.org/wiki/Design_of_the_FAT_file_system#VFAT_long_file_names](http://en.wikipedia.org/wiki/Design_of_the_FAT_file_system#VFAT_long_file_names)

**Reserved Sectors (if any)**

Usually the number of reserved sectors is zero. If it is non-zero, then the
following sector(s) are reserved (and could be used by the boot procedure for
whatever purposes).

**Data Clusters 0002..nnnn**

Finally all following sectors are data clusters. The first cluster is called
cluster number (000)(0)002, followed by number (000)(0)003, (000)(0)004, and so
on.

[http://en.wikipedia.org/wiki/Design_of_the_FAT_file_system](http://en.wikipedia.org/wiki/Design_of_the_FAT_file_system)

| DSi SD/MMC Internal NAND Layout |
| --- |

The DSi uses a 256MB Samsung eMMC moviNAND(?) flash chip, which is a NAND flash
chip with a built-in controller that implements a MMC (SDIO?) interface. In
many ways, it's like an SD card (or actually: MMC card) in BGA packaging, and
some people have successfully read it with modified(why/how?) SD(/MMC?) card
readers (is there any standard software that can be used for doing that?). The
last 16MB is used for wear-leveling purposes (such as replacing bad blocks),
while the first 240MB is used for storing actual data.

Addressing is done in terms of 512-bytes sectors. All wear-levelling and
bad-block-mapping is handled transparently inside the chip by the controller.
Most sectors are encrypted with a per-console key.

**Overall eMMC Layout**

```

| Offset Size Description
 00000000h 200h PC-style MBR, encrypted with a per-console key
 00000200h 200h Stage 2 Boot Info Block 1 (used)
 00000400h 200h Stage 2 Boot Info Block 2 (unused, same as above)
 00000600h 200h Stage 2 Boot Info Block 3 (unused, nonsense NAND offsets)
 00000800h 26600h Stage 2 ARM9 Bootcode (encrypted with universal key)
 00026E00h 27600h Stage 2 ARM7 Bootcode (encrypted with universal key)
 0004E400h 400h Stage 2 Footer -- unknown format, but first 10 bytes
 are (unencrypted) build number of Stage 2 bootloader
 0004E800h B1000h Unused (all 00h)
 000FF800h 200h Unused (all 00h) (or No$gba Footer with CID & Console ID)
 000FFA00h 400h Diagnostic area. (often contains build date of
 device in plaintext) Blank in never-before-booted
 DSi. Might be written to during firmware updates.
 000FFE00h 200h Unused (all FFh)
 00100000h EE00h Unused (all 00h)
 0010EE00h CDF1200h 1st partition (205.9Mbyte) (main, encrypted, FAT16)
 0CF00000h 9A00h Unused (all 00h)
 0CF09A00h 20B6600h 2nd partition (32.7Mbyte) (photo, encrypted, FAT12)
 For 240.0MB chips (Samsung KMAPF0000M-S998 or KLM5617EFW-B301):
 0EFC0000h BA00h Unused (all 00h)
 0EFCBA00h 34600h 3rd partition (0.2Mbyte) (extra, unformatted)
 0F000000h - End of 240MByte Address Space
 For 245.5MB chips (ST NAND02GAH0LZC5, both rev30 and rev31):
 0EFC0000h B600h Unused (all 00h?) (smaller unused area as in 240MB chip)
 0EFCB600h 5B4A00h 3rd partition (5.7Mbyte) (extra, unformatted)
 0F580000h - End of 245.5MByte Address Space |
| --- |

**Stage 2 Boot Info Blocks 1, 2, 3 (unencrypted, aside from the RSA block)**

```

| 000h 20h Zerofilled
 020h 4 ARM9 Bootcode NAND Offset (800h) (Info Block 3: 80400h)
 024h 4 ARM9 Bootcode Size actual (26410h)
 028h 4 ARM9 Bootcode RAM Address / Entry (37B8000h)
 02Ch 4 ARM9 Bootcode Size rounded-up (26600h)
 030h 4 ARM7 Bootcode NAND Offset (26E00h) (Info Block 3: A6A00h)
 034h 4 ARM7 Bootcode Size actual (27588h)
 038h 4 ARM7 Bootcode RAM Address / Entry (37B8000h)
 03Ch 4 ARM7 Bootcode Size rounded-up (27600h)
 040h BFh Zerofilled
 0FFh 1 ARM Loadmode Flags (0Ch)
 100h 80h RSA Block (B3,FF,EC,E5,..) (Boot Info Block 3: 5B,E1,7A,9F,..)
 180h 14h Global MBK1..MBK5 Slot Settings
 194h 0Ch Local MBK6..MBK8 Settings, WRAM Areas for ARM9
 1A0h 0Ch Local MBK6..MBK8 Settings, WRAM Areas for ARM7
 1ACh 4 Global MBK9 Setting, WRAM Slot Write Protect (FF000000h)
 1B0h 50h Zerofilled |
| --- |

The ARM Loadmode Flags are (usually 0Ch):

```

| 0 ARM9 Loadmode (0=Normal to memory, 1=Special via bit3)
 1 ARM7 Loadmode (0=Normal to memory, 1=Special via bit3)
 2 Unused, set (usually 1)
 3 Special Loadmode (0=LZSS to memory, 1=Transfer via IPC FIFO)
 4-7 Unused, cleared (usually 0) |
| --- |

BUG: If bit3 is set, then parts of bootrom behaves as if both bit0 & bit1
are also set. Hmmm, but bit3 is actually set, so it's default behaviour...?

The above RSA Block contains 74h bytes of information (plus 0Bh bytes padding):

```

| Pre 0Bh Leading RSA Padding (01,FF,FF,FF,FF,FF,FF,FF,FF,FF,00)
 00h 10h AES_Engine Key Y for ARM9/ARM7 Bootcode (EC,07,00,00,...)
 10h 14h SHA1 on WifiFlash[00h..27h] and eMMCBootInfo[00h..FFh,180h..1FFh]
 3DS: reportedly NAND/MBR[00h..27h] instead of WifiFlash[00h..27h]??
 24h 14h SHA1 on decrypted ARM9 Bootcode, with the actual binary size
 38h 14h SHA1 on decrypted ARM7 Bootcode, with the actual binary size
 4Ch 14h Zerofilled
 60h 14h SHA1 on above 60h-byte area at [00h..5Fh] (63,D2,FC,6E,...) |
| --- |

**eMMC Encryption for Boot Sectors (AES-CTR, with fixed key; from RSA block)**

The ARM9/ARM7 bootcode is encrypted via AES-CTR:

```

| RSA_KEY = F1,F5,1A,FF,... ;-from 3DS TWL_FIRM (for RSA Block)
 IV[0..3] = +size ;\
 IV[4..7] = -size ; size rounded up to 200h boundary, ie.
 IV[8..B] = -size-1 ; from Boot Info Block entries [02Ch,03Ch]
 IV[C..F] = 00000000h ;/
 KEY_X[0..F] = "Nintendo DS",... ;-same as Key X for "Tad Files"
 KEY_Y[0..F] = EC,07,00,00,... ;-from RSA Block (see above) |
| --- |

The RSA_KEY key is stored in some non-dumpable area of the DSi BIOS, making it
impossible to obtain that key without chip decapping. However, Nintendo has
included the same RSA_KEY in the "TWL_FIRM" firmware update for 3DS.

**eMMC Encryption for MBR/Partitions (AES-CTR, with console-specific key)**

The MBR and both partitions are encrypted via AES-CTR:

```

| IV[0..F]: SHA1(CID)+Address/10h ;-eMMC Chip ID
 KEY_X[0..3]: [4004D00h] ;\
 KEY_X[4..7]: [4004D00h] XOR 24EE6906h ; CPU/Console ID, for
 KEY_X[8..B]: [4004D04h] XOR E65B601Dh ; DSi partitions on DSi
 KEY_X[C..F]: [4004D04h] ;/
 KEY_X[0..3]: [4004D00h] ;\CPU/Console ID, for
 KEY_X[4..B]: "NINTENDO" ; DSi partitions on 3DS
 KEY_X[C..F]: [4004D04h] ;/
 KEY_Y[0..F]: 0AB9DC76h,BD4DC4D3h,202DDD1Dh,E1A00005h ;-Constant |
| --- |

The CID value (eMMC Chip ID) should be in same format as stored in RAM at
2FFD7BCh: little-endian 120bit (without crc7), padded to 128bit (with MSB=00h),
ie. it should look like this (dd/ss being date/serial numbers):

```

| CID = [2FFD7BCh] = dd,ss,ss,ss,ss,03,4D,30,30,46,50,41,00,00,15,00
 SHA1(CID) = SWI_27h(SHA1value,2FFD7BCh,10h) |
| --- |

The resulting SHA1value is 14h-bytes, the first 10h-bytes are used as IV value,
whereas the DSi doesn't adjust the endianness (it does just use the SWI's
"big-endian" SHA1value as "little-endian" AES/IV value).

The CTR gets incremented after each 10h bytes (ie. to access a random address:
"IV=SHA1value+(address/10h)".

For more info on obtaining the CID and Port [4004D00h] values, see:

DSi Console IDs

See also:

DSi SD/MMC Images

**Related Decryption Tools**

```

| "NUS Downloader" allows to download and decrypt system updates
 "DSi SRL Extract" allows to decrypt DSiware files (when copied to SD card)
 "TWLTool" decrypt/encrypt eMMC images (firmware downgrading, dsiware-hax)
 "TWLbf" and "bfCL" bruteforce Console ID or CID (or both) from eMMC images |
| --- |

| DSi SD/MMC Bootloader |
| --- |

**Stages**

```

| Stage 1: Load Stage 2 from NAND Boot Sectors (via code in BIOS ROM)
 Stage 2: Load Stage 3 from NAND Filesystem
 Stage 3: Contains GUI and allows to boot Cartridges or NAND files |
| --- |

**Stage 1**

The first stage of the DSi's bootloader lives in ROM, presumably on the CPU
die. It loads further encrypted (and probably signed) stages from NAND flash,
starting with a (partially unencrypted) offset table in the sector at 0x200.

Not much is known about this bootloader yet, but it presumably knows how to:

```

| Initialize the encryption hardware
 Read the contents of NVRAM
 Initialize both LCDs
 Read blocks (but not files) from the NAND flash
 Perform some variety of integrity check on all data it reads(signature,CRC,?)
 Display basic hexadecimal error codes
 Possibly factory-programming the NAND flash?
 Might also do basic power-on self test of peripherals |
| --- |

When the Stage 1 bootloader (in ROM) fails, it displays a 32-bit hexadecimal
number on the top screen, known Stage 1 error codes are:

```

| Error Code Description
 0000FE00 Error communicating NAND chip (It's missing, CLK shorted, etc.)
 0000FEFC Integrity error in first block of Stage 2 (address at 220h)
 0000FEFD Integrity error in second block of Stage 2 (address at 230h)
 0000FEFE Boot sector integrity error (Sector 200h not valid), or error
 in NVRAM contents. |
| --- |

**Stage 2**

Unlike the stage1 bootloader, which must be small enough to fit in ROM
(probably several kilobytes), the stage2 bootloader has about a megabyte of
NAND flash reserved for it. The stage2 bootloader understands partitions and
filesystems, and it is capable of loading the DSi menu. It also must understand
the encryption used on filesystem blocks in the NAND, and it must understand
how to load and validate title metadata.

The Stage 2 loader was not modified by the System Menu 1.4 update. This is
still earlier in the boot process than the "Health and Safety" warning.

The first stage bootloader reads sector 0x200 in order to find a table of
offsets to the Stage 2 bootloader:

```

| 00000220 00 08 00 00 10 64 02 00 00 80 7b 03 00 66 02 00 |.....d....{..f..|
 00000230 00 6e 02 00 88 75 02 00 00 80 7b 03 00 76 02 00 |.n...u....{..v..|
 00000240 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 |................| |
| --- |

This appears to be describing two chunks of the stage2 loader, one 0x26410
bytes in length at address 0x800, and one 0x27588 bytes at address 0x26e00.

Note that this sector (and two similar ones at 0x400 and 0x600) appear to be
the only unencrypted blocks on the NAND flash.

It is unclear why there are two pieces which are nearly but not quite the same
size. Passive traces of the boot sequence confirm that the 0x26e00 chunk is
slightly larger, and it's loaded first. The 0x800 chunk is read immediately
after the 0x26e00 chunk.

Whereas the filesystem data in NAND is encrypted using a unique key for every
DSi, the stage2 bootloader is identical on every DSi tested so far. This
probably means that it is encrypted using a fixed key included in stage1.

**After Stage 2 is loaded:**

```

| 1. The NAND flash is partially re-initialized
 2. Sector 0 is read from the NAND. Appears to be (encrypted) DOS-style MBR.
 3. The MBR signature and the type of the first partition are verified.
 4. Filesystem metadata is read from sectors starting around 0x100000. The
 metadata appears to be in FAT format with long filenames.
 5. Multiple files are loaded from the filesystem. The exact read addresses
 will vary depending on your DSi's firmware version and the state of
 its filesystem when you performed the last firmware update. On a brand
 new DSi, it appears that the DSi Menu itself is loaded from 0xb20000
 after two small metadata files are read from 0xb1c000 and 0x7a0000. |
| --- |

All Stage 2 errors show before the health and safety screen. It appears that
Stage 2 errors from a cold power-on always cause the DSi to hang at a black
screen, whereas Stage 2 errors after reset (pressing but not holding the power
button) will give an error message screen. Known Stage 2 errors:

```

| Text Description
 "Error: 1-2435-8325" Invalid signature or partition type in MBR,
 invalid starting LBA.
 "Error: 2-2435-8325" Error reading fat/sectors from eMMC
 "Error: 3-2435-8325" DSi Menu integrity checks failed |
| --- |

**Boot Sectors in DSi Debug version**

Debug version bootsectors are almost same as in retail version: There's an
extra RSA key inserted at ARM9:37CEADCh, and lots of B/BL/BLX/LDR addresses
have changed due to the inserted bytes, but, apart from that changes, only 5
opcodes are really different, ARM9:37C07A0h, ARM9:37C0864h, ARM7:37B9110h,
ARM7:37B9200h do trigger errors when SCFG_DEBUG bit0-1 are zero (opposite of
retail version), and ARM9:37B8BA4h uses the extra RSA key at 37CEADCh for Debug
Launcher (instead of the retail key at 1FFC400h).

| DSi SD/MMC Device List |
| --- |

The Device List is automatically copied to ARM7 RAM address defined in
cartheader[1D4h] by firmware. The 400h-byte list is mainly used for
DSiware/firmware titles (DSi ROM titles will usually just receive an empty
zerofilled 400h-byte list; unless maybe if they are flagged to use SD/MMC
hardware in cartheader?). There appears to be no range check for the
cartheader[1D4h] entry (setting it to zero causes the list to be 'written' to
ARM7 ROM area).

**Device List, 400h-bytes, loaded to ARM7/RAM address from cartheader[1D4h]**

```

| 000h 54h*11 Device List (max 11 entries)
 39Ch 24h Zerofilled
 3C0h 40h Name 'nand:/title/000300tt/4ggggggg/content/000000vv.app' + 00h's |
| --- |

Format of the 54h-byte device list entries:

```

| 00h 1 Drive Letter ("A".."I")
 01h 1 Flags (see below)
 02h 1 Access Rights (bit1=Write, bit2=Read)
 03h 1 Zero
 04h 10h Device Name (eg. "nand" or "dataPub") (zeropadded)
 14h 40h Path (eg. "/" or "nand:/shared1") (zeropadded) |
| --- |

Bits in Flags byte:

```

| 0 Physical Drive (0=External SD/MMC Slot, 1=Internal eMMC)
 1-2 Zero (maybe MSBs of Drive)
 3-4 Device Type (0=Physical, 1=Virtual/File, 2=Virtual/Folder, 3=Reserved)
 5 Partition (0=1st, 1=2nd)
 6 Zero (maybe MSB of Partition)
 7 Encrypt? (set for eMMC physical devices; not for virtual, not for SD) |
| --- |

The DSi has 9 default devices ("A"-"I"):

```

| Letter/Flags Name Path ;Notes
 'A',81h,06h,00h 'nand' '/' ;eMMC Cart Partition 1
 'B',A1h,06h,00h 'nand2' '/' ;eMMC Cart Partition 2
 'C',11h,04h,00h 'content' 'nand:/title/000300tt/4ggggggg/content'
 'D',11h,04h,00h 'shared1' 'nand:/shared1' ;TWLCFGn.dat
 'E',11h,06h,00h 'shared2' 'nand:/shared2' ;Sound and wrap.bin
 'F',31h,06h,00h 'photo' 'nand2:/photo' ;Camera photos/frames
 'G',09h,06h,00h 'dataPrv' 'nand:/title/000300tt/4ggggggg/data/private.sav'
 'H',09h,06h,00h 'dataPub' 'nand:/title/000300tt/4ggggggg/data/public.sav'
 'I',00h,06h,00h 'sdmc' '/' ;SD Cart Partition 1 |
| --- |

Depending on the cartheader, usually only 5-8 of that 9 devices are copied to
the device list in RAM, and Access Rights for some devices can be crippled in
the RAM list entries.

Caution: The list may not contain forward references (eg. one can redirect
"dataPub" to "sdmc:/flipnote.pub", but that works only if "sdmc" was already
defined in one of the previous entries).

A and B: These physical eMMC partitions are always included in the list
(because they are needed as parent entries for Virtual devices), however, their
Access Rights are usually set to 00h (unless Title ID indicates a system file;
with cartheader[234h].bit4=1).

C and E: These virtual devices are passed ONLY to System Menu (Launcher)...
hmmm, or actually, the Launcher doesn't receive ANY device list at all?

G and H: Present only if Title ID indicates DSiware/firmware; with
cartheader[234h].bit2=1 (ie. not ROM carts), and only if public/private.sav
sizes are nonzero in carthdr[238h/23Ch].

A-H: removed from list if carthdr[1B4h].4=0 (no eMMC access).

I: removed from list if carthdr[1B4h].3=0 (no SD card access).

Some games may adjust the Device List AFTER booting. For example, Flipnote
changes the "photo" (F) Access Rights to 04h, and appends an extra device at
the end of the list, using a spare drive letter (C):

```

| 'C',09h,06h,00h 'share' 'nand:/shared2/0000' ;Sound file |
| --- |

Other device names that have been spotted here or there:

```

| 'verdata' for Version Data NARC file
 'rom' for executable's NitroROM filesystem
 'otherPub'
 'otherPrv' |
| --- |

Apart from Virtual Devices, there are also some Virtual Filename Placeholders:

```

| 'nand:/<tmpjump>' --> 'nand:/tmp/jump.app'
 'nand:/<sharedFont>' --> 'nand:/sys/TWLFontTable.dat'
 'nand:/<verdata>' --> 'nand:/title/0003000f/484e4c%02x/content/%08x.app'
 'nand:/<banner>' --> ..... '/data/banner.sav'
 ':<srl>' --> ..... |
| --- |

**public & private savedata**

If the file is in a folder named "CONTENT", then savedata is stored in a
separata "DATA" folder (this is as how Nintendo is doing it officially):

```

| "nand:/../CONTENT/title.tmd"
 "nand:/../CONTENT/00000002.app"
 "nand:/../DATA/public.sav"
 "nand:/../DATA/private.sav" |
| --- |

If the file is anywhere else (not in a CONTENT folder), then savedata should be
stored in the same folder & same name, with extension changed to .pub or
.prv (this homebrew convention allows to use more descriptive non-numeric
filenames, and doesn't require "title.tmd"):

```

| "sdmc:/../My Folder Name/Filpnote Studio (EUR.AUS).dsi"
 "sdmc:/../My Folder Name/Filpnote Studio (EUR.AUS).pub"
 "sdmc:/../My Folder Name/Filpnote Studio (EUR.AUS).prv" |
| --- |

As seen above, names can exceed the 8.3 character shortfilename limit (and can
use 16bit unicode), however, using such long filenames can quickly exceed the
64 character limit of the DSi's Device List (or it's 8bit character space), as
a workaround, unlaunch is converting the above names to 8.3 character format
before storing them in the Device List:

```

| "sdmc:/../MYFOLD~1/FLIPN~12.DSI"
 "sdmc:/../MYFOLD~1/FLIPN~10.PUB"
 "sdmc:/../MYFOLD~1/FLIPNO~2.PRV" |
| --- |

With those short 8-character folder names, one can have up to five folders
nested (or only three folders with extension alike "MYFOLD~1.EXT", or more than
five folders if folder/filename are shorter than 8 characters).

As shown in the above examples, the longname ("Filpnote Studio (EUR.AUS)") is
same for all three files, but the ~NN suffix may vary for shortnames.

| DSi SD/MMC Complete List of SD/MMC Files/Folders |
| --- |

**DSi eMMC Partition 1 (FAT16)**

```

| SYS <DIR> sys
 LOG <DIR> log
 PRODUCT LOG 0000023D product.log
 SYSMENU LOG 00004000 sysmenu.log
 SHOP LOG 00000020 shop.log
 HWINFO_S DAT 00004000 HWINFO_S.dat
 HWINFO_N DAT 00004000 HWINFO_N.dat
 CERT SYS 00000F40 cert.sys
 HWID SGN 00000100 HWID.sgn
 TWLFON~1 DAT 000D2C40 TWLFontTable.dat
 DEV KP 000001BE dev.kp
 TITLE <DIR> title
 00030017 <DIR> 00030017 (aka System Menu)
 484E4150 <DIR> 484e4150 (aka Launcher)
 DATA <DIR> data
 PRIVATE SAV 00004000 private.sav
 CONTENT <DIR> content
 TITLE TMD 00000208 title.tmd
 00000002 APP 0019E400 00000002.app
 00030015 <DIR> 00030015 (aka System Base Tools)
 484E4250 <DIR> 484e4250 (aka System Settings)
 DATA <DIR> data
 CONTENT <DIR> content
 TITLE TMD 00000208 title.tmd
 00000002 APP 00285C00 00000002.app
 484E4650 <DIR> 484e4650 (aka Nintendo DSi Shop)
 DATA <DIR> data
 PRIVATE SAV 00004000 private.sav
 EC CFG 00000134 ec.cfg
 CONTENT <DIR> content
 00000004 APP 00526400 00000004.app
 TITLE TMD 00000208 title.tmd
 0003000F <DIR> 0003000f (aka System Data)
 484E4341 <DIR> 484e4341 (aka Wifi Firmware)
 DATA <DIR> data
 CONTENT <DIR> content
 TITLE TMD 00000208 title.tmd
 00000002 APP 00017E60 00000002.app
 484E4841 <DIR> 484e4841 (aka Nintendo DS Cart Whitelist)
 DATA <DIR> data
 CONTENT <DIR> content
 TITLE TMD 00000208 title.tmd
 00000001 APP 0004B1D0 00000001.app
 484E4C50 <DIR> 484e4c50 (aka Version Data)
 DATA <DIR> data
 CONTENT <DIR> content
 00000004 APP 00001B50 00000004.app
 TITLE TMD 00000208 title.tmd
 00030005 <DIR> 00030005 (aka System Fun Tools)
 484E4441 <DIR> 484e4441 (aka DS Download Play)
 DATA <DIR> data
 CONTENT <DIR> content
 TITLE TMD 00000208 title.tmd
 00000001 APP 00069BC0 00000001.app
 484E4541 <DIR> 484e4541 (aka Pictochat)
 DATA <DIR> data
 CONTENT <DIR> content
 00000000 APP 00074FC0 00000000.app
 TITLE TMD 00000208 title.tmd
 484E4950 <DIR> 484e4950 (aka Nintendo DSi Camera)
 DATA <DIR> data
 PRIVATE SAV 00080000 private.sav
 CONTENT <DIR> content
 TITLE TMD 00000208 title.tmd
 00000002 APP 00443C00 00000002.app
 484E4A50 <DIR> 484e4a50 (aka Nintendo Zone)
 DATA <DIR> data
 PRIVATE SAV 00100000 private.sav
 CONTENT <DIR> content
 00000003 APP 0014D000 00000003.app
 TITLE TMD 00000208 title.tmd
 484E4B50 <DIR> 484e4b50 (aka Nintendo DSi Sound)
 DATA <DIR> data
 PRIVATE SAV 00080000 private.sav
 CONTENT <DIR> content
 00000002 APP 00451000 00000002.app
 TITLE TMD 00000208 title.tmd
 00030004 <DIR> 00030004 (aka DSiware)
 484E4750 <DIR> 484e4750 (aka Nintendo DSi Browser)
 DATA <DIR> data
 PRIVATE SAV 00200000 private.sav
 CONTENT <DIR> content
 00000001 APP 008F1C00 00000001.app
 TITLE TMD 00000208 title.tmd
 4B475556 <DIR> 4b475556 (aka Flipnote Studio)
 DATA <DIR> data
 PUBLIC SAV 007F0000 public.sav
 CONTENT <DIR> content
 00000000 APP 00348400 00000000.app
 TITLE TMD 00000208 title.tmd
 TICKET <DIR> ticket
 00030017 <DIR> 00030017 (aka System Menu)
 484E4150 TIK 000002C4 484e4150.tik (aka Launcher)
 00030015 <DIR> 00030015 (aka System Base Tools)
 484E4250 TIK 000002C4 484e4250.tik (aka System Settings)
 484E4650 TIK 000002C4 484e4650.tik (aka Nintendo DSi Shop)
 0003000F <DIR> 0003000f (aka System Data)
 484E4341 TIK 000002C4 484e4341.tik (aka Wifi Firmware)
 484E4841 TIK 000002C4 484e4841.tik (aka Nintendo DS Cart Whitelist)
 484E4C50 TIK 000002C4 484e4c50.tik (aka Version Data)
 00030005 <DIR> 00030005 (aka System Fun Tools)
 484E4441 TIK 000002C4 484e4441.tik (aka DS Download Play)
 484E4541 TIK 000002C4 484e4541.tik (aka Pictochat)
 484E4950 TIK 000002C4 484e4950.tik (aka Nintendo DSi Camera)
 484E4A50 TIK 000002C4 484e4a50.tik (aka Nintendo Zone)
 484E4B50 TIK 000002C4 484e4b50.tik (aka Nintendo DSi Sound)
 00030004 <DIR> 00030004 (aka DSiware)
 484E4750 TIK 000002C4 484e4750.tik (aka Nintendo DSi Browser)
 4B414D56 TIK 000002C4 4b414d56.tik (aka Paper Plane)
 4B443956 TIK 000002C4 4b443956.tik (aka Dr. Mario)
 4B475556 TIK 000002C4 4b475556.tik (aka Flipnote Studio)
 4B4D3958 TIK 000002C4 4b4d3958.tik (aka Magic Made Fun: Deep Psyche)
 SHARED1 <DIR> shared1
 TWLCFG0 DAT 00004000 TWLCFG0.dat
 TWLCFG1 DAT 00004000 TWLCFG1.dat
 SHARED2 <DIR> shared2
 LAUNCHER <DIR> launcher
 WRAP BIN 00004000 wrap.bin
 0000 00200000 0000
 IMPORT <DIR> import
 TMP <DIR> tmp
 ES <DIR> es
 WRITE <DIR> write
 PROGRESS <DIR> progress |
| --- |

**DSi eMMC Partition 2 (FAT12)**

```

| PHOTO <DIR> photo
 PRIVATE <DIR> private
 DS <DIR> ds
 APP <DIR> app
 484E494A <DIR> 484E494A (aka Nintendo DSi Camera Stuff)
 PIT BIN 00001F60 pit.bin
 DCIM <DIR> DCIM
 100NIN02 <DIR> 100NIN02
 HNI_0008 JPG 0000AB51 HNI_0008.JPG
 HNI_0009 JPG 00009A96 HNI_0009.JPG
 HNI_0010 JPG 0000932B HNI_0010.JPG
 HNI_0011 JPG 00009CB8 HNI_0011.JPG
 HNI_0012 JPG 00009CA9 HNI_0012.JPG
 HNI_0013 JPG 00009A3B HNI_0013.JPG |
| --- |

**DSi eMMC Partition 3 (unformatted)**

There's a small 3rd partition in MBR, but it's left unformatted (the VBR and
FAT and everything is left zero-filled). Unknown if there are any cases where
this partition is used, and if so: if it's meant to be encrypted or
unencrypted. In case it's meant to be encrypted: Observe that the unformatted
partition contains UNENCRYPTED zeroes (so trying to "decrypt" those zeroes
would produce random garbage data).

**SD Card**

DSiware (including Browser and Flipnote) can be exported to SD Card (via System
Menu, Data Managment).

Camera Photos and Frames can be exported (via Nintendo DSi Camera, Options,
Copy) (Frames are some sort of gaudi-masks that can be used (and created) via
one of the Camera special effect features; the mask uses YELLOW as transparent
color). As on eMMC storage, all photos from internal camera are mirrored
horizontally.

```

| PRIVATE <DIR> private
 DS <DIR> ds
 TITLE <DIR> title ;\
 484E4750 BIN 9.180K 484E4750.bin (aka Nintendo DSi Browser) ; dsiware
 4B475556 BIN 11.510K 4B475556.bin (aka Flipnote Studio) ; games
 HNB_ LST 2K HNB_.lst (content: "VUGKPGNH") ;/
 APP <DIR> app
 484E494A <DIR> 484E494A (aka Nintendo DSi Camera Stuff) ;\
 PIT BIN 47K pit.bin ; camera
 DCIM <DIR> DCIM ; frames
 100NIN02 <DIR> 100NIN02 ;
 HNI_0001 JPG 45K HNI_0001.JPG ;-frame/mask ;/
 4B475556 <DIR> 4B475556 (aka Flipnote Studio Stuff) ;\
 RECENT10 PLS 4K recent10.pls ;
 MARK0 PLS 8K mark0.pls ; flipnote
 MARK1 PLS 8K mark1.pls ; stuff
 MARK2 PLS 8K mark2.pls ;
 MARK3 PLS 8K mark3.pls ;
 001 <DIR> 001 ;
 DIRMEMO2 LST 157K dirmemo2.lst ;
 F08243~1 PPM 467K F08243_0E5E2296197E5_000.ppm ;/
 DCIM <DIR> DCIM
 101NIN02 <DIR> 101NIN02 ;<-- can be 100NIN02 thru 999NIN02
 HNI_0001 JPG 43K HNI_0001.JPG ;\dsi camera photos
 HNI_0002 JPG 17K HNI_0002.JPG ; (names are numbered differently
 HNI_0003 JPG 39K HNI_0003.JPG ;/as on eMMC where they came from) |
| --- |

Reportedly, the "Nintendo DSi Sound" application can also access audio files in
AAC format, saved in any folders on SD Card.

Aside from DSi-related files, the SD Card can also contain whatever files from
other computers.

**Blocks**

The System Menu, Data Management feature is referring to filesizes & free
space in "Blocks" (aka 128Kbytes units, aka 1Mbit units).

| DSi SD/MMC Summary of SD/MMC Files/Folders |
| --- |

**File/folder names**

The DSi is using weird numeric strings as file/folder names:

```

| 000000vv Title Version (lowercase hex32bit) from tmd[1E4h] as carthdr[1Eh]
 4ggggggg Title ID Gamecode (hex) as carthdr[230h..233h]
 000300tt Title ID Filetype (hex) as carthdr[234h..237h]
 HNI_nnnn Camera photo/frame files (nnnn = 0001..0100 decimal)
 nnnNIN02 Camera photo/frame folders (nnn = 100..999 decimal) |
| --- |

The "000300tt" can be:

```

| 00030000 ROM Cartridges (as so for ROMs, doesn't appear in SD/MMC files)
 00030004 DSiware (browser, flipnote, and games) (if any installed)
 00030005 System Fun Tools (camera, sound, zone, pictochat, ds download play)
 0003000f System Data (non-executable, without carthdr)
 00030015 System Base Tools (system settings, dsi shop, 3ds transfer tool)
 00030017 System Menu (launcher) |
| --- |

The "4ggggggg" can be (last two digits are region(s), or "41" for all regions):

```

| 484e41gg System Menu (Launcher)
 484e42gg System Settings
 484e4341 Wifi Firmware (non-executable datafile) (all regions)
 484e4441 DS Download Play (all regions)
 484e4541 Pictochat (all regions) (no update available)
 484e46gg Nintendo DSi Shop
 484e47gg Nintendo DSi Browser
 484e4841 Nintendo DS Cart Whitelist (non-executable datafile) (all regions)
 484e49gg Nintendo DSi Camera
 484e4agg Nintendo Zone (doesn't exist in Korea)
 484e4bgg Nintendo DSi Sound
 484e4cgg Version Data (non-executable datafile)
 484e4fgg Nintendo 3DS Transfer Tool
 484E494A Nintendo DSi Camera Data (uppercase) ("japan") (aka all regions)
 4b44474a Dokodemo Wii no Ma (japan only)
 4b4755gg Flipnote Studio (doesn't exist in Korea/China)
 42383841 DS Internet settings (a new DSi tool on 3DS consoles)
 4bgggggg DSiware games... (whatever games you have purchased, if any) |
| --- |

These files can be stored in Internal eMMC, or on external SD card, and can be
downloaded from Nintendo's server (when buying games, or updating system
files).

**DSi Internal eMMC**

Internal eMMC can contain System files and any purchased DSiware games:

```

| FAT16:\ticket\000300tt\4ggggggg.tik (encrypted) ;ticket (708 bytes)
 FAT16:\title\000300tt\4ggggggg\content\title.tmd ;tmd (520 bytes)
 FAT16:\title\000300tt\4ggggggg\content\000000vv.app ;executable (decrypted)
 FAT16:\title\000300tt\4ggggggg\data\public.sav ;size as carthdr[238h]
 FAT16:\title\000300tt\4ggggggg\data\private.sav ;size as carthdr[23Ch]
 FAT16:\title\000300tt\4ggggggg\data\ec.cfg ;dsi shop only
 FAT16:\title\000300tt\4ggggggg\data\banner.sav ;if carthdr[1BFh].bit2=1 |
| --- |

Note that some of the above files are containing their own virtual filesystem
inside of the eMMC's FAT16 filesystem (NitroROM filesystems in "000000vv.app"
files, and FAT12 filesystems in "public.sav" and "private.sav" files).

The System tools (menu, settings, and shop) are also storing further data on
FAT16 (outside of the ticket and title folders):

```

| FAT16:\shared1\TWLCFG0.dat ;16K
 FAT16:\shared1\TWLCFG1.dat ;16K
 FAT16:\shared2\launcher\wrap.bin ;16K
 FAT16:\shared2\0000 ;2048K (sound recorder)
 FAT16:\sys\log\product.log ;573 bytes
 FAT16:\sys\log\sysmenu.log ;16K
 FAT16:\sys\log\shop.log ;32 bytes
 FAT16:\sys\HWINFO_S.dat ;16K
 FAT16:\sys\HWINFO_N.dat ;16K
 FAT16:\sys\cert.sys ;3904 bytes (or 2560 bytes)
 FAT16:\sys\HWID.sgn ;256 bytes (unknown purpose/content)
 FAT16:\sys\TWLFontTable.dat ;843.1K (D2C40h bytes) (compressed)
 FAT16:\sys\dev.kp ;446 bytes (encrypted)
 FAT16:\import\ ;empty folder
 FAT16:\progress\ ;empty folder
 FAT16:\tmp\es\write\ ;empty folder |
| --- |

The Camera is storing further data on the eMMC FAT12 partition:

```

| FAT12:\photo\DCIM\100NIN02\HNI_nnnn.JPG ;camera photos
 FAT12:\photo\private\ds\app\484E494A\pit.bin ;camera info
 FAT12:\photo\private\ds\app\484E494A\DCIM\100NIN02\HNI_nnnn.JPG;camera frames |
| --- |

And, there's a small 3rd eMMC partition in MBR, but it's left unformatted (the
VBR and FAT and everything is left zero-filled).

**DSi External SD Card**

DSiware games (and browser and flipnote) can be copied to SD card (via System
Menu, Data Managment) (however, the DSi doesn't seem to allow to execute files
on SD card, so they can be used only if they are copied back to the DSi):

```

| SD:\private\ds\title\4GGGGGGG.bin ;executable/data in one file (encrypted)
 SD:\private\ds\title\HNB_.lst ;list of gamecodes |
| --- |

Camera data can be copied to SD card (via Nintendo DSi Camera, Options, Copy):

```

| SD:\DCIM\nnnNIN02\HNI_nnnn.JPG ;camera photos
 SD:\private\ds\app\484E494A\pit.bin ;camera info
 SD:\private\ds\app\484E494A\DCIM\nnnNIN02\HNI_nnnn.JPG ;camera frames |
| --- |

Flipnote "movies" can be also saved on SD card:

```

| SD:\private\ds\app\4B4755GG\recent10.pls ;Recently saved path/filenames
 SD:\private\ds\app\4B4755GG\mark0.pls ;Heart sticker path/filenames
 SD:\private\ds\app\4B4755GG\mark1.pls ;Crown sticker path/filenames
 SD:\private\ds\app\4B4755GG\mark2.pls ;Music sticker path/filenames
 SD:\private\ds\app\4B4755GG\mark3.pls ;Skull sticker path/filenames
 SD:\private\ds\app\4B4755GG\001\dirmemo2.lst ;List of all files in folder
 SD:\private\ds\app\4B4755GG\001\XNNNNN_NNNNNNNNNNNNN_NNN.ppm ;normal
 SD:\private\ds\app\4B4755GG\YYYYMMDD\NNN\XNNNNN_NNNNNNNNNNNNN_NNN.ppm ;backup
 SD:\private\ds\app\4B4755GG\gif\XNNNNN_NNNNNNNNNNNNN_NNN.gif ;gif |
| --- |

The Nintendo DSi Sound utility can read .AAC (and .M4A) files from SD card
(though it doesn't seem to allow to save your own recordings to SD card?).
There appears to be no special folder location, ie. the AAC/M4A files can be
anywhere:

```

| SD:\...\*.aac
 SD:\...\*.m4a |
| --- |

**DSi Shop and System Update Download URLs**

```
[http://nus.cdn.t.shop.nintendowifi.net/ccs/download/000300tt4ggggggg/tmd](http://nus.cdn.t.shop.nintendowifi.net/ccs/download/000300tt4ggggggg/tmd)[http://nus.cdn.t.shop.nintendowifi.net/ccs/download/000300tt4ggggggg/cetk](http://nus.cdn.t.shop.nintendowifi.net/ccs/download/000300tt4ggggggg/cetk)[http://nus.cdn.t.shop.nintendowifi.net/ccs/download/000300tt4ggggggg/000000vv](http://nus.cdn.t.shop.nintendowifi.net/ccs/download/000300tt4ggggggg/000000vv)
| |
| --- |

The "cetk" file contains the encrypted ticket. The "cetk" file is available
only for freeware downloads (eg. system updates). Commercial DSi Shop titles
can be downloaded the same way, except that the ticket must be somehow
purchased/downloaded separetely.

Nintendo does keep hosting older "00000vv" versions (except, the original
version, "00000000" isn't available in all cases; namely if the title was
pre-installed on all DSi's then it would be pointless to provide it as update).

**NUS Downloader Notation**

The homebrew NUS Downloader utility for PCs comes up with its own renaming
scheme:

```

| d:\...\TITLES\000300tt4ggggggg\ddd\000000vv ;executable (encrypted)
 d:\...\TITLES\000300tt4ggggggg\ddd\000000vv.APP ;executable (decrypted)
 d:\...\TITLES\000300tt4ggggggg\ddd\CETK ;cetk (2468 bytes)
 d:\...\TITLES\000300tt4ggggggg\ddd\TMD ;tmd (520 bytes) |
| --- |

Whereas, "ddd" is same as "000000vv" multiplied by 256 decimal (which is
nonsense and redundant). The decrypted ".APP" file is generated only if the
"common key" is found in 16-byte file "dsikey.bin".

| DSi SD/MMC Images |
| --- |

**Filesystem Viewer**

no$gba debug version is allowing to view the filesystem tree from encrypted
eMMC images (via menubar: Window, Filesystem), the filesystem viewer is also
able to export single files from eMMC and SD images (by double-clicking
separate files).

**Emulation**

No$gba emulates read/write-accesses to eMMC images.

Another idea for future would be using files & folders on the PC filesystem
instead of a single image file (that might be easier to deal with in some
cases, but for the reverse-engineering stage it's better to stick with original
authentic images).

**Encrypted eMMC Images**

no$gba can emulate up to 12 consoles simultaneously, and expects the eMMC
images having following filename(s), in no$gba folder:

```

| DSi-#.mmc ;eMMC for machine 1..12 (# = 1..C hex) |
| --- |

the eMMC images are encrypted with per-console keys, so decryption won't work
without knowing the console ID values. no$gba expects that info to be stored in
a 40h-byte Footer at the end of the file:

```

| 00000000h .. Encrypted eMMC image (usually 240Mbyte for DSi)
 0F000000h 16 Footer ID ("DSi eMMC CID/CPU")
 0F000010h 16 eMMC CID (dd ss ss ss ss 03 4D 30 30 46 50 41 00 00 15 00)
 0F000020h 8 CPU/Console ID (nn n1 nn nn nn nn xn 08)
 0F000028h 24 Reserved (zerofilled) |
| --- |

Alternately, the "footer" can be stored in the zerofilled area at eMMC offset
000FF800h..000FF83Fh (using that area, the data can be kept in place even when
using other tools; that were getting confused by the data appended at end of
file).

**SD Card Images**

no$gba supports SD Card images in similar fashion as above eMMC images, but
without needing any footer since there's encryption on SD cards. The image
should contain a pre-formatted MBR and FAT (as real SD cards do).

```

| DSi-#.sd ;SD Card for machine 1..12 (# = 1..C hex) |
| --- |

note: no$gba does currently support only 128MB SDSC images (the CID, CSD, OCR,
SCR, SSR registers are hardcoded for images with 125698048 byte size), there is
a .zip file with an empty pre-formatted SD image in the no$gba package (if you
want to use that image: unzip it to the no$gba folder).

**Dumping eMMC Images**

DSiware exploits like sudokuhax are allowing to access SD/MMC hardware (so one
could simply copy all eMMC sectors to a file on SD Card).

Unfortunately, most/other exploits don't have SD/MMC access, so dumping would
work only when the eMMC chip to a SD/MMC card reader. For details, see:

AUX DSi SD/MMC Pin-Outs

For obtaining the Console IDs, see:

DSi Console IDs

| DSi SD/MMC DSiware Files on Internal eMMC Storage |
| --- |

DSiware games (downloaded from DSi Shop), and pre-installed System Tools are
consisting of following files:

**FAT16:\title\000300tt\4ggggggg\content\000000vv.app ;executable (decrypted)**

Contains the executable, with same header as used in Cartridge ROM images, and
usually with a NitroROM File System (ie. a second virtual filesystem inside of
the FAT16 filesystem).

DSi Cartridge Header

DS Cartridge NitroROM and NitroARC File Systems

Note: There are also three non-executable ".app" files without cartridge
headers (Wifi Firmware, Version Data, and DS Cart Whitelist).

**FAT16:\title\000300tt\4ggggggg\data\public.sav ;size as carthdr[238h]**

**FAT16:\title\000300tt\4ggggggg\data\private.sav ;size as carthdr[23Ch]**

These files can contain whatever save data. The .sav files are usually
containing a FAT12 with its own VBR, FAT, and Directories (so they use some
virtual FAT12 inside of the real FAT16).

When exporting a game to SD Card (via System Settings, Data Managment), then
public.sav (eg. used by Flipnote) will be included in the exported image,
whilst private.sav (eg. used by DSi Browser) won't be included.

**FAT16:\title\000300tt\4ggggggg\data\ec.cfg ;dsi shop only**

Whatever extra file, encrypted, 134h bytes, used by DSi Shop only.

**FAT16:\title\000300tt\4ggggggg\data\banner.sav ;if carthdr[1BFh].bit2=1**

Custom icon, used by some games to indicate the game progress. Format is
similar as Icon/Title, but containing only animated icon data (without title).
For details, see:

DS Cartridge Icon/Title

**FAT16:\title\000300tt\4ggggggg\content\title.tmd ;tmd (520 bytes)**

**FAT16:\ticket\000300tt\4ggggggg.tik (encrypted) ;ticket (708 bytes)**

**FAT16:\ticket\000300tt\00000000.tik (encrypted) ;multi-tik? (N*708 bytes?)**

These files do contain title metadata (.tmd) and tickets (.tik).

DSi SD/MMC DSiware Tickets and Title metadata

The .tik files are encrypted with ES Block Encryption (using same key X/Y as
for dev.kp):

```

| KEY_X[00h..03h] = 4E00004Ah ;\
 KEY_X[04h..07h] = 4A00004Eh ; same as for Tad
 KEY_X[08h..0Bh] = Port[4004D00h+4] xor C80C4B72h ;
 KEY_X[0Ch..0Fh] = Port[4004D00h+0] ;/
 KEY_Y[00h..0Fh] = Constant (E5,CC,5A,8B,...) ;from ARM7BIOS |
| --- |

DSi ES Block Encryption

Caution: There are some ways to modify .tmd files, but that can cause the whole
title to be deleted when starting one of the following three tools:

```

| Data Management (in System Settings), DSi Shop, and 3DS transfer tool |
| --- |

These tools will delete the titles "content" folder (with .app and .tmd files)
and the "data" folder (with .sav files). As a workaroung: Set read-only
attribute for .tmd and .app files (the deletion aborts once when hitting a
read-only file; with the files being processed as ordered in the directory).

Note: Tickets are kept stored in eMMC even after deleting titles (that's
allowing to redownload the titles for free; at least that's been the case when
the DSi shop was still online).

Below "wrap.bin" and "menusave.dat" files are containing lists of installed
titles, however, it isn't neccessary to edit those files when manually
installing .tmd/.app/.tik files.

**FAT16:\shared2\launcher\wrap.bin (16Kbytes)**

Contains a list of installed DSiware Title IDs (in no specific order).

```

| 0000h 14h SHA1 on entries [014h..03Fh]
 0014h 14h SHA1 on entries [040h..177h]
 0028h 4 ID ("APWR") (aka 'WRAP' with mis-ordered letters)
 002Ch 4 Size of entries at [040h..177h] (00000138h, aka 39*8)
 0030h 10h Zerofilled
 0040h 138h Space for 39 Title IDs (as at cart[230h]) (8x00h=unused entry)
 0178h 3E88h Unknown (looks like random/garbage, or encrypted junk) |
| --- |

**FAT16:\title\00030017\484e41gg\data\private.sav:\menusave.dat (System Menu)**

This private.sav file contains a 4000h-byte FAT12 image. The FAT12 contains
only one file: menusave.dat (154h bytes), containing a list of Title IDs (and
their sort-order how they are arranged in System Menu; users can drag the icons
to rearrange their ordering):

```

| 0000h 4 ID ("TSSV")
 0004h 4 Zerofilled (used somehow, can be nonzero?)
 0008h 2 CRC16 on [000h..0153h], initial value 5356h, assume [008h]=0000h
 000Ah 6 Zerofilled
 0010h 39x8 Title IDs (gg,gg,gg,gg,tt,00,03,00) (0=NDS CartSlot or Unused)
 0148h 8 Zerofilled
 0150h 4 Index of NDS CartSlot Entry (0..39) |
| --- |

The current selection isn't stored in this file (instead, the Title ID of the
most recently selected title is stored in the TWLCFGn.dat files).

Note that the "Nintendo Zone" utility isn't included in this list (even though
it's present in title & ticket folders, and listed in wrap.bin).

The System Menu works even if data\private.sav doesn't exist (however, the
sort-order is stored only if data\private.sav does exist).

| DSi SD/MMC DSiware Files on External SD Card (.bin aka Tad Files) |
| --- |

The DSi can export applications from NAND to SD (via System Settings -->
Data Managment). The ".bin" files created on SD are using the "Tad file
structure", and alongsides, there's a "HNB_.lst" file containing a list of game
codes.

**SD:\private\ds\title\HNB_.lst (list of gamecodes) (1208 bytes)**

```

| 000h 1200 List of 300 gamecodes, spelled backwards (or zero = unused entry)
 4B0h 1 Language (0=Jap, 1=Eng, 2=Fre, 3=Ger, 4=Ita, 5=Spa, 6=Chi, 7=Kor?)
 4B1h 3 Zero
 4B4h 2 CRC16 on entries [000h..4B3h] (with initial value FFFFh)
 4B6h 2 Zero |
| --- |

For example, "VUGK" in HNB_.lst would indicate gamecode KGUV aka "Flipnote
Studio" for EUR/AUS regions. And the corresponding SD Card file would be
"SD:\private\ds\title\4B475556.bin" (with the 4-letter gamecode encoded as
8-digit uppercase HEX number). The full 16-digit on eMMC storage would be
"00030004\4b475556" (in lowercase, and with the "00030004" implied for the
DSiWare folder; files from system folders cannot be exported to SD Card).

The Language byte reflects the System Settings's language selection at time
when the HNB_.lst was created or modified (unknown why that info is stored in
the file).

**SD:\private\ds\title\4GGGGGGG.bin (encrypted executable/data in one file)**

```

| Offset Size Key Description
 000000h 4000h+20h FIX Icon/Title
 004020h B4h+20h FIX Header
 0040F4h 440h+20h FIX Cert (certificates/hashes)
 004554h 208h+20h VAR title.tmd (usually 208h bytes; but could be bigger)
 00477Ch size+N*20h VAR 000000vv.app
 ... 0 ? seven N/A parts (unknown if/when they are used)
 ... size+N*20h FIX public.sav (if any)
 ... ? ? banner.sav (if any) |
| --- |

ES Block Encryption is used to encrypt the header block, cert block, and the 11
content parts. Each are their own seperate ES blocks.

DSi ES Block Encryption

Some of the above blocks use fixed keys (FIX):

```

| KEY_X[00h..0Fh] = Constant ("Nintendo DS",...)
 KEY_Y[00h..0Fh] = Constant (66,82,32,04,...) ;from ARM7BIOS
 since above X/Y are constant, that gives a fixed normal key:
 KEY[00h..0Fh] = Constant (3D,A3,EA,33,...) ;as used in "dsi srl extract" |
| --- |

Other blocks use variable per-console keys (VAR):

```

| KEY_X[00h..03h] = 4E00004Ah ;\
 KEY_X[04h..07h] = 4A00004Eh ; same as for dev.kp
 KEY_X[08h..0Bh] = Port[4004D00h+4] xor C80C4B72h ;
 KEY_X[0Ch..0Fh] = Port[4004D00h+0] ;/
 KEY_Y[00h..0Fh] = Constant (CC,FC,A7,03,...) ;from ARM7BIOS |
| --- |

Without knowing the console-specifc Port[4004D00h] value, the data could be
decrypted only by the DSi console that has originally exported the file to SD
card.

However, Nintendo has somehow (maybe accidently) managed to store the
Port[4004D00h] value as 16-digit ASCII string in the "TW cert"; which can be
decrypted right from the SD card file (as done by the homebrew "dsi srl
extract" utility).

**Decrypted Icon/Title (at 0000h, size 4000h)**

```

| 0000h 23C0h Icon/Title (usually 23C0h bytes) ;see carthdr[068h,208h]
 23C0h 1C40h Zerofilled (padding to get 4000h byte size) |
| --- |

**Decrypted Header block (at 4020h, size B4h)**

```

| 000h 4 Fixed ID "4ANT" (aka TNA4, spelled backwards)
 004h 2 Maker Code, spelled backwards ("10"=Nintendo) ;carthdr[010h]
 006h 1 Zero
 007h 1 Title version (vv) ;carthdr[01Eh]
 008h 6 DSi MAC Address, spelled backwards ;wifi_flash[036h]
 00Eh 2 Zero
 010h 16 Some console ID from HWINFO_N.dat ;datfile[8Ch..9Bh]
 020h 8 Title ID (gg gg gg gg 04 00 03 00) ;carthdr[230h]
 028h 4 Size of title.tmd (usually 208h+20h)
 02Ch 4 Size of 000000vv.app (size+N*20h) ;carthdr[210h]
 030h 4*7 Size of seven N/A parts (0)
 04Ch 4 Size of public.sav (size+N*20h) ;carthdr[238h]
 050h 4 Size of banner.sav? (usually 0) ;carthdr[1BFh].bit2=1
 054h 4*8 List of eight Content IDs in same order as title.tmd
 074h 3Eh Reserved section per tmds, uh? (mostly zero, plus garbage?)
 0B2h 2 Unknown (zero) |
| --- |

**Decrypted Cert block (at 40F4h, size 460h)**

```

| 000h 20 SHA1 of Icon/Title
 014h 20 SHA1 of TNA4
 028h 20 SHA1 of title.tmd
 03Ch 20 SHA1 of 000000vv.app
 040h 20*7 SHA1 of seven N/A parts (unused, can be whatever garbage)
 0DCh 20 SHA1 of public.sav
 0F0h 20 SHA1 of banner.sav
 104h 3Ch ECC signature of [000h..103h] with AP cert
 140h 180h AP cert, signed by TW cert
 2C0h 180h TW cert, specific to a console (see dev.kp) |
| --- |

More in depth, the above two cert's look as so:

```

| 140h 4 Signature Type (00,01,00,02) (ECC, sect233r1, non-RSA) ;\
 144h 3Ch Signature Hex numbers... across... below? ; AP cert
 180h 40h Signature padding/alignment (zerofilled) ; 180h-byte
 1C0h 40h Signature Name "Root-CA..-MS..-TW..-08..", 00h-padded ;
 "Root-CA00000001-MS00000008-TWxxxxxxxx-08nnnnnnnnnnn1nn";
 200h 4 Key Type (00,00,00,02) (ECC, sect233r1, non-RSA) ;
 204h 40h Key Name "AP00030015484e42gg", 00h-padded ;sys.settings ;
 244h 4 Key Random/time/type/flags/chksum? ;<-- ZERO here ;
 248h 3Ch Key Public ECC Key (point X,Y) (random/per game?) ;
 284h 3Ch Key padding/alignment (zerofilled) ;/
 2C0h 4 Signature Type (00,01,00,02) (ECC, sect233r1, non-RSA) ;\
 2C4h 3Ch Signature Hex numbers... across... below? ; TW cert
 300h 40h Signature padding/alignment (zerofilled) ; 180h-byte
 340h 40h Signature Name "Root-CA00000001-MS00000008", 00h-padded ; (same as
 380h 4 Key Type (00,00,00,02) (ECC, sect233r1, non-RSA) ; dev.kp,
 384h 40h Key Name "TWxxxxxxxx-08nnnnnnnnnnn1nn", 00h-padded ; excluding
 3C4h 4 Key Random/time/type/flags/chksum? ; private
 3C8h 3Ch Key Public ECC Key (point X,Y) ; key)
 404h 3Ch Key padding/alignment (zerofilled) ;/ |
| --- |

Much like the Wii, the DSi carries with it a private ECC key that it can use to
sign things, and a certificate signed by Nintendo that attests to the fact that
the public ECC key belongs to a genuine DSi.

| DSi SD/MMC DSiware Files from Nintendo's Server |
| --- |

**[http://nus.cdn.t.shop.nintendowifi.net/ccs/download/000300tt4ggggggg/000000vv](http://nus.cdn.t.shop.nintendowifi.net/ccs/download/000300tt4ggggggg/000000vv)
**

The "000000vv" file contains the ".app" file (in encrypted form).

```

| Server: "000000vv" (AES-CBC encrypted, raw)
 eMMC: "000000vv.app" (decrypted, raw)
 SD Card: "GGGGGGGG.bin" (ES-block encrypted, with extra data) |
| --- |

First, the encrypted Title Key must be decrypted (via AES-CBC):

```

| KEY[00h..0Fh] = Common Key (AF,1B,F5,16,...) ;from ARM7BIOS
 IV[00h..07h] = Title ID (00,03,00,tt,gg,gg,gg,gg) ;from tik/cetk[1DCh]
 IV[08h..0Fh] = Zerofilled ;padding
 Input: Encrypted Title Key ;from tik/cetk[1BFh]
 Output: Decrypted Title Key ;for use in next step |
| --- |

Then, the actual executable/file can be decrypted (also via AES-CBC):

```

| KEY[00h..0Fh] = Decrypted Title Key ;from above step
 IV[00h..01h] = Usually Zero (or "Index" from tmd?) ;from tmd[1E8h+N*24h] ?
 IV[02h..0Fh] = Zerofilled ;padding
 Input: Encrypted file "000000vv" ;from http download
 Output: Decrypted file "000000vv.app" ;saved on eMMC |
| --- |

The above decryption steps do require a big-endian AES-CBC software
implementation (the DSi hardware supports only little-endian, and it supports
only AES-CTR and AES-CCM, and, especially, it supports only the "encrypt" key
schedule, whilst AES-CBC would require a different "decrypt" key schedule).

**[http://nus.cdn.t.shop.nintendowifi.net/ccs/download/000300tt4ggggggg/cetk](http://nus.cdn.t.shop.nintendowifi.net/ccs/download/000300tt4ggggggg/cetk)
**

**[http://nus.cdn.t.shop.nintendowifi.net/ccs/download/000300tt4ggggggg/tmd](http://nus.cdn.t.shop.nintendowifi.net/ccs/download/000300tt4ggggggg/tmd)
**

**[http://nus.cdn.t.shop.nintendowifi.net/ccs/download/000300tt4ggggggg/tmd.nn](http://nus.cdn.t.shop.nintendowifi.net/ccs/download/000300tt4ggggggg/tmd.nn)
**

These files do contain tickets ("cetk"), and title metadata ("tmd" for newest
version, plus "tmd.nn" for older versions; with nn=0,1,256,257,512 and the
like).

DSi SD/MMC DSiware Tickets and Title metadata

The "cetk" file is available only for free system updates (not for titles sold
commercially in DSi Shop).

**Downloading**

The files can be downloaded with normal web browsers. The homebrew "NUS
Downloader" utility is also allowing to download those files (and to decrypt
them, provided that the "cetk" is available).

For free system updates, tickets can be downloaded as "cetk" files. For titles
sold commercially in DSi ship, tickets must be purchased somehow differently.

For example, the updates for DSi System Settings (EUR) can be downloaded from:

```
[http://nus.cdn.t.shop.nintendowifi.net/ccs/download/00030015484e4250/tmd](http://nus.cdn.t.shop.nintendowifi.net/ccs/download/00030015484e4250/tmd)[http://nus.cdn.t.shop.nintendowifi.net/ccs/download/00030015484e4250/cetk](http://nus.cdn.t.shop.nintendowifi.net/ccs/download/00030015484e4250/cetk)[http://nus.cdn.t.shop.nintendowifi.net/ccs/download/00030015484e4250/00000002](http://nus.cdn.t.shop.nintendowifi.net/ccs/download/00030015484e4250/00000002)[http://nus.cdn.t.shop.nintendowifi.net/ccs/download/00030015484e4250/00000003](http://nus.cdn.t.shop.nintendowifi.net/ccs/download/00030015484e4250/00000003)
| |
| --- |

The tmd and cetk files are unencrypted. The 00000002 and 00000003 files are
encrypted executables (containing System Settings v2 and v3).

**Older Versions**

Nintendo is usually keeping older "000000vv" files on their server, so one
could still download those older versions.

The oldest/original version would be usually "00000000", however, in case of
system files that are pre-installed on all consoles, only later versions are
available as updates (ie. starting with "00000001", or for some reason, with
"00000002", in case of system settings).

The tmd/cetk files are available only for the newest version (meaning that some
cosmetic values like title size & title version are adjusted for the newest
version; the cetk's AES-CBC key usually doesn't change for updates, so older
versions can be still decrypted with newer cetk's).

Older versions are usually deleted on internal eMMC storage, so only the
"000000vv.app" file exists.

| DSi SD/MMC DSiware Tickets and Title metadata |
| --- |

Below describes the "raw" ticket+tmd formats. For more info on the data being
stored/encrypted in various locations, see these chapters:

DSi SD/MMC DSiware Files on Internal eMMC Storage

DSi SD/MMC DSiware Files on External SD Card (.bin aka Tad Files)

DSi SD/MMC DSiware Files from Nintendo's Server

**Ticket (cetk aka .tik)**

Tickets exist as "cetk" files (as found on Nintendo's server), and as ".tik"
files (as found in nand/ticket folder):

```

| Server: "cetk" unencrypted, 2468 bytes (2A4h+700h), tik+certificate
 eMMC: "gggggggg.tik" encrypted, 708 bytes (2A4h+20h), tik+es_block
 SD Card: N/A N/A, tickets aren't exported to SD card |
| --- |

Tickets are used for decrypting downloads from DSi shop. They are essentially
containing a 16-byte AES-CBC decryption key, plus signatures and some other
stuff.

```

| 000h 4 Signature Type (00h,01h,00h,01h) (100h-byte RSA)
 004h 100h Signature RSA-OpenPGP-SHA1 across 140h..2A3h
 104h 3Ch Signature padding/alignment (zerofilled)
 140h 40h Signature Name "Root-CA00000001-XS00000006", 00h-padded
 180h 3Ch ECDH data for one-time installation keys? ;zero for free tik's
 1BCh 1 Zero (3DS: Ticket Version=1)
 1BDh 1 Zero (3DS: CaCrl Version=0)
 1BEh 1 Zero (3DS: SignerCrl Version=0)
 1BFh 10h Encrypted AES-CBC Title Key
 1CFh 1 Zero
 1D0h 8 Ticket ID (00,03,xx,xx,xx,xx,xx,xx) ?
 1D8h 4 Console ID (see dev.kp "TWxxxxxxxx", zero for free system updates)
 1DCh 8 Title ID (00,03,00,17,"HNAP") ;cart[230h]
 1E4h 2 Zero (Wii: mostly FFFFh)
 1E6h 2 Title Version (vv,00) (LITTLE-ENDIAN!?) ;NEWEST ;cart[01Eh]
 1E8h 4 Zero (Wii: Permitted Titles Mask)
 1ECh 4 Zero (Wii: Permit mask)
 1F0h 1 Zero (Wii: Allow Title Export using PRNG key, 0=No, 1=Yes)
 1F1h 1 Zero (Wii: Common Key Index, 0=Normal, 1=Korea) (DSi: Always 0)
 1F2h 2Ah Zero
 21Ch 4 Zero (3DS: eShop Account ID?)
 220h 1 Zero
 221h 1 Unknown (01h) (Wii: Unknown, 00h=Non-VC, 01h=VC=VirtualConsole?)
 222h 20h FFh-filled (Wii: Content access permissions, 1 bit per content)
 242h 20h 00h-filled (Wii: Content access permissions, 1 bit per content)
 262h 2 Zero
 264h 4 Zero ;Wii: Time Limit Enable (0=Disable, 1=Enable)
 268h 4 Zero ;Wii: Time Limit Seconds (uh, seconds since/till when?)
 26Ch 38h Zero ;Wii: Seven more Time Limits (Enable, Seconds)
 2A4h 700h Certificates (see below) (only in "cetk", not in ".tik) |
| --- |

The Launcher checks some of the permission entries, but it doesn't check RSA
for tickets, so one can create own/dummy tickets. The Console ID and Title ID
might be also unchecked, so one could possibly simply copy/rename existing
tickets (from the same console), or decrypt/copy/re-encrypt tickets (from other
consoles).

Note: It seems to be possible to store multiple tickets in one .tik file (in
that case each ticket is separately encrypted in 2A4h+20h bytes).

**Title metadata (tmd aka .tmd)**

Title metadata exists as "tmd" file (as found on Nintendo's server), and as
".tmd" file (as found in eMMC title folders), and it's also included in ".bin"
files (in files exported to SD cards):

```

| Server: "tmd" unencrypted, 2312 bytes (208h+700h), tmd+certificate
 Server: "tmd.nn" as above, OLDER tmd versions (nn=0,1,256,257,512,etc)
 eMMC: "title.tmd" unencrypted, 520 bytes (208h+0), tmd
 SD Card: "GGGGGGGG.bin" encrypted, huge file, contains .app+tmd+sav files |
| --- |

Title metadata contains signatures and other useless stuff. One possibly useful
feature is that it allows to define more than one "content" per title, however,
that feature appears to be only used on Wii. DSi titles are usually having only
one content (the ".app" file).

```

| 000h 4 Signature Type (00h,01h,00h,01h) (100h-byte RSA)
 004h 100h Signature RSA-OpenPGP-SHA1 across 140h..207h
 104h 3Ch Signature padding/alignment (zerofilled)
 140h 40h Signature Name "Root-CA00000001-CP00000007", 00h-padded
 180h 1 TMD Version (00h) (unlike 3DS)
 181h 1 ca_crl_version (00h)
 182h 1 signer_crl_version (00h)
 183h 1 Zero (padding/align 4h)
 184h 8 System Version (0)
 18Ch 8 Title ID (00,03,00,17,"HNAP") ;cart[230h]
 194h 4 Title Type (0)
 198h 2 Group ID (eg. "01"=Nintendo) ;cart[010h]
 19Ah 4 SD/MMC "public.sav" filesize in bytes (0=none) ;cart[238h]
 19Eh 4 SD/MMC "private.sav" filesize in bytes (0=none) ;cart[23Ch]
 1A2h 4 Zero
 1A6h 1 Zero (3DS: SRL Flag)
 1A7h 3 Zero
 1AAh 10h Parental Control Age Ratings ;cart[2F0h]
 1BAh 1Eh Zerofilled
 1D8h 4 Access rights (0)
 1DCh 2 Title Version (vv,00) (LITTLE-ENDIAN!?) ;NEWEST ;cart[01Eh]
 1DEh 2 Number of contents (at 1E4h and up) (usually 00h,01h)
 1E0h 2 boot content index (0)
 1E2h 2 Zerofilled (padding/align 4h)
 1E4h+N*24h 4 Content ID (00,00,00,vv) ;lowercase/hex ;"0000000vv.app"
 1E8h+N*24h 2 Content Index (00,00)
 1EAh+N*24h 2 Content Type (00,01) ;aka DSi .app
 1ECh+N*24h 8 Content Size (00,00,00,00,00,19,E4,00) ;NEWEST ;cart[210h]
 1F4h+N*24h 14h Content SHA1 (on decrypted ".app" file);NEWEST
 208h+.. 700h Certificates (see below) (only in "tmd", not in ".tmd) |
| --- |

The Launcher does verify the .tmd's RSA signature, and uses the Title/Content
IDs to create the path/filename for the .app file. The Version, Size, SHA1
entries are not verified, so one could use any .tmd version with any .app
version (when renaming the .app to match the .tmd's Content ID, but that'd be a
messy solution, and it's better to use the correct .tmd per .app).

Note: title.tmd is usually/always 208h bytes (one content), max permitted size
is 49E4h (200h contents), a larger filesize can crash the firmware (used by
Unlaunch.dsi exploit).

**Certificates (at end of "cetk" and "tmd) (not in ".tik" or ".tmd)**

```

| cert cetk tmd siz content
 000h 2A4h 208h 4 Signature Type (00h,01h,00h,01h) ;\
 004h 2A8h 20Ch 100h Signature ;
 104h 3A8h 30Ch 3Ch Signature padding/alignment (zerofilled) ;
 140h 3E4h 348h 40h Signature Name "Root-CA00000001", 00h-padded ; 300h bytes
 180h 424h 388h 4 Key Type (00,00,00,01) (100h-byte RSA) ;
 184h 428h 38Ch 40h Key Name "XS00000006", 00h-padded ;
 1C4h 468h 3CCh 4 Key Random/time/type/flags/chksum? ;
 1C8h 46Ch 3D0h 100h Key Public RSA Key ;
 2C8h 56Ch 4D0h 4 Key Public RSA Exponent? (00,01,00,01) ;
 2CCh 570h 4D4h 34h Key padding/alignment (zerofilled) ;/
 300h 5A4h 508h 4 Signature Type (00h,01h,00h,00h) ;\
 304h 5A8h 50Ch 200h Signature ;
 504h 7A8h 70Ch 3Ch Signature padding/alignment (zerofilled) ;
 540h 7E4h 748h 40h Signature Name "Root" (padded with 00h) ; 400h bytes
 580h 824h 788h 4 Key Type (00,00,00,01) (100h-byte RSA) ;
 584h 828h 78Ch 40h Key Name "CA00000001", 00h-padded ;
 5C4h 868h 7CCh 4 Key Random/time/type/flags/chksum? ;
 5C8h 86Ch 7D0h 100h Key Public RSA Key ;
 6C8h 86Ch 8D0h 4 Key Public RSA Exponent? (00,01,00,01) ;
 6CCh 970h 8D4h 34h Key padding/alignment (zerofilled) ;/ |
| --- |

Note: Above certificates should be just a 1:1 copy of the entries in cert.sys.

Note: "XS00000006" or "XS00000003" is found in cetk/tik's, in tmd's it's called
"CP00000007".

**Notes**

The Title Version and Content Size/SHA1 entries are reflecting the NEWEST
".app" version (but the AES-CBC key should usually/always also work for older
versions).

The homebrew NUS Downloader utility is saving TMD (520 bytes; with REMOVED
certificate) and CETK (2468 bytes; with included certificate).

| DSi SD/MMC Firmware dev.kp and cert.sys Certificate Files |
| --- |

**FAT16:\sys\cert.sys ;3904 bytes (or 2560 bytes before DSi-Shop connection)**

Data in this file is same on all retail DSi consoles (even for different
regions like US and EUR and KOR).

```

| 000h 300h Public RSA Key "XS00000006" signed by "Root-CA00000001"
 300h 400h Public RSA Key "CA00000001" signed by "Root"
 700h 300h Public RSA Key "CP00000007" signed by "Root-CA00000001"
 Below NOT in Korea? Or NOT when notyet connected to DSi Shop?
 A00h 240h Public ECC Key "MS00000008" signed by "Root-CA00000001"
 C40h 300h Public RSA Key "XS00000003" signed by "Root-CA00000001" |
| --- |

The cert.sys for DSi debug version is different:

```

| 000h 300h Public RSA Key "CP00000005" signed by "Root-CA00000002"
 300h 300h Public RSA Key "XS00000006" signed by "Root-CA00000002"
 600h 400h Public RSA Key "CA00000002" signed by "Root"
 A00h 300h Public RSA Key "CP00000007" signed by "Root-CA00000002" |
| --- |

More detailed, the retail version of cert.sys looks as so:

```

| 000h 4 Signature Type (00,01,00,01) (100h-byte RSA) ;\
 004h 100h Signature RSA-OpenPGP-SHA1 across 140h..2FFh ;
 104h 3Ch Signature padding/alignment (zerofilled) ;
 140h 40h Signature Name "Root-CA00000001", 00h-padded ;
 180h 4 Key Type (00,00,00,01) (100h-byte RSA) ;
 184h 40h Key Name "XS00000006", 00h-padded ;
 1C4h 4 Key Random/time/type/flags/chksum? ;
 1C8h 100h Key Public RSA Key (92,FF,96,40..) ;
 2C8h 4 Key Public RSA Exponent? (00,01,00,01) ;
 2CCh 34h Key padding/alignment (zerofilled) ;/
 300h 4 Signature Type (00,01,00,00) (200h-byte RSA) (!) ;\
 304h 200h Signature RSA-OpenPGP-SHA1 across 540h..6FFh ;
 504h 3Ch Signature padding/alignment (zerofilled) ;
 540h 40h Signature Name "Root", 00h-padded ;
 580h 4 Key Type (00,00,00,01) (100h-byte RSA) ;
 584h 40h Key Name "CA00000001", 00h-padded ;
 5C4h 4 Key Random/time/type/flags/chksum? ;
 5C8h 100h Key Public RSA Key (B2,79,C9,E2..) ;
 6C8h 4 Key Public RSA Exponent? (00,01,00,01) ;
 6CCh 34h Key padding/alignment (zerofilled) ;/
 700h 4 Signature Type (00,01,00,00) (100h-byte RSA) ;\
 704h 100h Signature RSA-OpenPGP-SHA1 across 840h..9FFh ;
 804h 3Ch Signature padding/alignment (zerofilled) ;
 840h 40h Signature Name "Root-CA00000001", 00h-padded ;
 880h 4 Key Type (00,00,00,01) (100h-byte RSA) ;
 884h 40h Key Name "CP00000007", 00h-padded ;
 8C4h 4 Key Random/time/type/flags/chksum? ;
 8C8h 100h Key Public RSA Key (93,BC,0D,1F..) ;
 9C8h 4 Key Public RSA Exponent? (00,01,00,01) ;
 9CCh 34h Key padding/alignment (zerofilled) ;/
 Below NOT when notyet connected to DSi Shop:
 A00h 4 Signature Type (00,01,00,01) (100h-byte RSA) ;\
 A04h 100h Signature RSA-OpenPGP-SHA1 across B40h..C3Fh ;
 B04h 3Ch Signature padding/alignment (zerofilled) ;
 B40h 40h Signature Name "Root-CA00000001", 00h-padded ;
 B80h 4 Key Type (00,00,00,02) (ECC, sect233r1, non-RSA) ;
 B84h 40h Key Name "MS00000008", 00h-padded ;
 BC4h 4 Key Random/time/type/flags/chksum? ;
 BC8h 3Ch Key Public ECC Key (point X,Y) (01,93,6D,08..) ;
 C04h 3Ch Key padding/alignment (zerofilled) ;/
 C40h 4 Signature Type (00,01,00,01) (100h-byte RSA) ;\
 C44h 100h Signature RSA-OpenPGP-SHA1 across D80h..F3Fh ;
 D44h 3Ch Signature padding/alignment (zerofilled) ;
 D80h 40h Signature Name "Root-CA00000001", 00h-padded ;
 DC0h 4 Key Type (00,00,00,01) (100h-byte RSA) ;
 DC4h 40h Key Name "XS00000003", 00h-padded ;
 E04h 4 Key Random/time/type/flags/chksum? ;
 E08h 100h Key Public RSA Key (AD,07,A9,37..) ;
 F08h 4 Key Public RSA Exponent? (00,01,00,01) ;
 F0Ch 34h Key padding/alignment (zerofilled) ;/ |
| --- |

Cert chain for the DSi. Contains certificates with signed keys:

```

| Root-CA00000001: used for signing the four certificates below
 Root-CA00000001-CP00000007: used for signing TMDs ("Content Protection"?)
 Root-CA00000001-MS00000008: used for signing per-console ECC keys ("Master"?)
 Root-CA00000001-XS00000003: used for signing tickets from the DSiWare Shop
 Root-CA00000001-XS00000006: used for signing (common) tickets ("access"?) |
| --- |

A similar file with the same name exists on the Wii.

**FAT16:\sys\dev.kp ;446 bytes (encrypted), 414 bytes (when decrypted)**

The dev.kp file is encrypted with ES Block Encryption (using same key X/Y as
for .tik files):

```

| KEY_X[00h..03h] = 4E00004Ah ;\
 KEY_X[04h..07h] = 4A00004Eh ; same as for Tad
 KEY_X[08h..0Bh] = Port[4004D00h+4] xor C80C4B72h ;
 KEY_X[0Ch..0Fh] = Port[4004D00h+0] ;/
 KEY_Y[00h..0Fh] = Constant (E5,CC,5A,8B,...) ;from ARM7BIOS |
| --- |

DSi ES Block Encryption

The decrypted dev.kp contains following entries:

```

| 000h 4 Signature Type (00,01,00,02) (ECC, sect233r1, non-RSA) ;\
 004h 3Ch Signature Hex numbers... across... below? ;
 040h 40h Signature padding/alignment (zerofilled) ;
 080h 40h Signature Name "Root-CA00000001-MS00000008", 00h-padded ;
 0C0h 4 Key Type (00,00,00,02) (ECC, sect233r1, non-RSA) ;
 0C4h 40h Key Name "TWxxxxxxxx-08nnnnnnnnnnn1nn", 00h-padded ;
 104h 4 Key Random/time/type/flags/chksum? ;
 108h 3Ch Key Public ECC Key (point X,Y) ;<-- public key ;
 144h 3Ch Key padding/alignment (zerofilled) ;
 180h 1Eh Key Private ECC Key ;<-- private key ;/ |
| --- |

The "TWxxxxxxxx-08nnnnnnnnnnn1nn" string may vary:

```

| "TW" might be for DSi only (ie. it might be different on DSi XL or 3DS?)
 "xxxxxxxx" is 8-digit lower-case hex number (unknown where from; for .tik)
 "08nnnnnnnnnnn1nn" is 16-digit lower-case hex number (from Port 4004D00h) |
| --- |

Example:

```

| Signature across rest of block -- type = 0x00010002, ECC
 0000000: 00 01 00 02 00 db da 21 3b e1 f1 bf bb 4d dc 1d
 0000010: 60 29 da 19 42 1e 66 4f a8 e5 27 a1 d4 ea 46 7d
 0000020: 9b b4 00 95 c5 0d e8 fa ef a7 8d e9 bc 54 da c1
 0000030: 24 94 0b 7c ad a8 61 d5 05 97 c2 64 38 ad 18 f9 |
| --- |

```

| 0000040: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
 0000050: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
 0000060: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
 0000070: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 |
| --- |

```

| Key used to sign this cert (Root-CA00000001-MS00000008)
 0000080: 52 6f 6f 74 2d 43 41 30 30 30 30 30 30 30 31 2d Root-CA00000001-
 0000090: 4d 53 30 30 30 30 30 30 30 38 00 00 00 00 00 00 MS00000008
 00000a0: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
 00000b0: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 |
| --- |

```

| Console ID string
 00000c0: 00 00 00 02 54 57 63 37 39 64 63 65 63 39 2d 30 ....TWc79dcec9-0
 00000d0: 38 61 32 30 32 38 37 30 31 30 38 34 31 31 38 00 8a2028701084118.
 00000e0: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
 00000f0: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 |
| --- |

```

| Public ECC key (30 bytes, starting at 0x108)
 0000100: 00 00 00 00 6f dd de 42 01 e0 34 a3 19 bc a9 af
 0000110: 50 fe 8a ac 75 08 07 a9 3a 2c 21 51 93 ae 4a 90
 0000120: 6e 62 41 f1 a2 fe 00 00 3d 0a 13 97 da 53 17 98
 0000130: 69 38 65 67 ca f4 9c 87 ec 44 b7 eb d0 ec b8 3d
 0000140: 23 cf 7a 35 00 00 00 00 00 00 00 00 00 00 00 00
 0000150: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
 0000160: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
 0000170: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 |
| --- |

```

| Private per-console ECC key, used for signing files on SD
 0000180: 01 12 9d e0 77 82 44 d3 ee 99 ad ce e5 fa fa ed
 0000190: c9 ab 8e a1 f9 b5 c8 14 3c 74 74 f8 19 3a |
| --- |

Note that the console id itself is burned in an OTP area of the TWL CPU, and
changing the contents of this file will not actually change the console id.

This file contains the unique per-console ECC private-public key pair, along
with a certificate issued by Nintendo.

This file is created by the DSi Shop, with data from a SOAP reply. The SOAP
request data includes the hw console id, and the 0x100-byte RSA signature
stored in NAND file "HWID.sgn". Trying to send that request would require a
NAND dump, but when you have a NAND dump already sending that request is
pointless since you can grab dev.kp from NAND.

Sending that request is pointless anyway since the dev.kp data from the server
is random. The returned dev.kp data from the server for the EC private/public
keys are random, the ticket consoleID immediately following TW before - in the
twcert keyid is random as well. DSi Shop and System Settings don't contain any
code for deleting dev.kp. If you try to delete/rename dev.kp manually from NAND
a new dev.kp will be generated by the shop, but then the server will return an
error since the server account public dev.kp cert won't match.

Data management can't be accessed when dev.kp doesn't exist since you'd have no
twcert to sign/verify tads with, like when you never connected the DSi Shop
server.

| DSi SD/MMC Firmware Certificate/Keys (DER) |
| --- |

**.der files (ASN.1)**

```

| DSi Version Data, narc:\NintendoCA-G2.der (unencrypted)
 DSi Version Data, narc:\.twl-*.der (ES Block encrypted)
 DSi Browser, rom:\ca\*.ca (unencrypted)
 DSi is-twl-update, *.der (dev unit public keys) (unencrypted, stringless)
 DSi Dokodemi, rom:key\pubkey.der (only 162 bytes) (unencrypted, stringless)
 3DS browser applet, RomFS:\browser\rootca.pem (ASCII BASE64 .der's)
 3DS System Data Archives 1, offline... cave.pem (ASCII BASE64 .der)
 3DS Shared Data Archives, Non-Nintendo TLS, *.der (unencrypted)
 3DS System Data Archives 1, ClCertA, RomFS:*.bin (AES-CBC encrypted)
 3DS Nintendo Zone, RomFS:\certs\*.der (unencrypted)
 3DS Nintendo Zone, RomFS:\certs\dev.pem (ASCII BASE64 .der)
 3DS Nintendo Zone, RomFS:\certs\client.key (ASCII BASE64 .der)
 3DS Miiverse olv applet, RomFS:\browser\cave.pem (ASCII BASE64 .der)
 3DS Miiverse olv applet, RomFS:\browser\*.p12 (nested der-inside-der?) |
| --- |

For the BASE64 .pem variant, see:

3DS Files - BASE64 Encoding

There are two types of .der files:

Public keys with Certificates (these do usually contain lots of entries with
many OIDs and name strings for certificate, company, country, email, etc).

Private keys (these do usually contain only a few entries, often without any
OIDs at all, apart from the private key, they do usually also contain the
public key & original prime numbers).

```

| _________________________ Tag,Length,Value Encoding __________________________ |
| --- |

The files consist of several Tag-Length-Value (TLV) triplets. All data is in
BIG-ENDIAN.

**Tag**

```

| bit6-7 class (0=Universal, 1=Application, 2=Context-defined, 3=Private)
 bit5 form (0=Primitive, 1=Constructed)
 bit4-0 number (0..1Fh) |
| --- |

Common are:

```

| BOOLEAN UNIVERSAL Primitive 00000001 (01h)
 INTEGER UNIVERSAL Primitive 00000010 (02h)
 BIT STRING UNIVERSAL Primitive 00000011 (03h)
 OCTET STRING UNIVERSAL Primitive 00000100 (04h)
 NULL UNIVERSAL Primitive 00000101 (05h)
 OBJECT IDENTIFIER UNIVERSAL Primitive 00000110 (06h)
 UTF8String UNIVERSAL Primitive 00001100 (0Ch)
 PrintableString UNIVERSAL Primitive 00010011 (13h)
 TeletexString UNIVERSAL Primitive 00010100 (14h)
 IA5String UNIVERSAL Primitive 00010110 (16h)
 Time String (17h) in dsi version data
 BMPString UNIVERSAL Primitive 00011110 (1Eh)
 SEQUENCE UNIVERSAL Constructed 00110000 (30h)
 SEQUENCE OF UNIVERSAL Constructed 00110000 (30h)
 SET UNIVERSAL Constructed 00110001 (31h)
 SET OF UNIVERSAL Constructed 00110001 (31h)
 -unknown- CONTEXT Primitive (80h) ;3ds miiverse .p12
 -unknown- CONTEXT Constructed (A0h) ;dsi version (small)
 -unknown- CONTEXT Constructed (A3h) ;dsi browser (big) |
| --- |

**Length**

The length encoding depends on bit7 of the first byte:

```

| when bit7=0 --> bit6-0 contain length (0..127 bytes)
 when bit7=1 --> bit6-0 contain number of following length bytes |
| --- |

The length does rarely exceed 16bits, the most common lengths are:

```

| len = Length 0..127 bytes (with len<80h)
 81h,len = Length 128..255 bytes (with len>7Fh)
 82h,msb,lsb = Length 256..65535 bytes (with msb:len>00FFh) |
| --- |

```

| ___________________________________ Tag's ____________________________________ |
| --- |

**01h,Len,Value - BOOLEAN**

Allows to define a boolean flag. Len is always 1, and value must be:

```

| value[0] = flag (00h=False, or FFh=True) |
| --- |

**02h,Len,Value - INTEGER**

Allows to define integer, including "large" values like RSA keys. Bit7 of the
first byte is treated as sign bit, when storing unsigned values one must insert
a leading 00h byte if the high bit is set.

```

| value[0..(len-1)] = data ;(len)*8 bits (with sign bit in MSB of 1st byte) |
| --- |

**03h,Len,Value - BIT STRING**

Allows to define data that isn't a multiple of 8 bits. Or multiples of 8 bits
are also working (when first byte is 00h), this sometimes used to store RSA
keys as unsigned integers, or, more weirdly, it can also contain further
Tag,Len,Value items (eg. SEQUENCE tags with separate RSA key and RSA exponent
enclosed in the BIT string) (... somewhat as if the BIT string contains a child
.der file?).

```

| value[0] = number of unused bits in LSB of last byte (0..7)
 value[1..(len-1)] = data ;(len-1)*8-unused bits |
| --- |

**04h,Len,Value - OCTET STRING**

Allows to define data that is a multiple of 8 bits. This can contain general
purpose stuff, including mixup's of flags, integers, arrays, text strings.

```

| value[1..(len-1)] = data ;(len-1)*8 bits |
| --- |

**05h,Len - NULL**

Allows to define empty/zero data, Len must be 00h, and accordingly, value is
0-byte tall.

**06h,Len,Value - OBJECT IDENTIFIER (OID)**

OIDs can be used as prefix for other tags (eg. to indicate that the following
tag(s) contain a username or email address). The OIDs are usually expressed as
a series of decimal numbers separated by dots. In binary form, these are stored
as follows:

```

| value[0] = 2nd+(1st*40) ;\
 value[1] = 3rd ; basic encoding for 7bit values
 value[2] = 4th ;
 etc. ;/ |
| --- |

There are some special cases: The 1st number must be 0..2. The 2nd number must
be 0..39 when 1st=0..1 (there is no such limit when 1st=2).

Values bigger than 7bit are split into two or more 7bit fragments (with bit7=1
in the leading fragments, and bit7=0 in the last fragment), that
variable-length encoding is used whenever "2nd+(1st*40)" or "3rd,4th,etc." are
exceeding 7bits. For example,

```

| 1.2.3.4.255 --> 2Ah, 03h, 04h, 81h,7Fh
 2.999.1234.1.2.3.4 --> 88h,37h, 89h,52h, 01h, 02h, 03h, 04h |
| --- |

There are millions of OID values (assigned by thousands of organizations), the
OID's can be deciphered with online databases, for example,

```
[http://oidref.com/](http://oidref.com/)
| |
| --- |

However, DER certificates are usually containing only a few standard OID
values. See the "OID Values" list below for the OID's found in DSi/3DS
certificates.

**0Ch,Len,Value - UTF8 String (Unicode byte & multibyte chars)**

**13h,Len,Value - Printable String (some ASCII A-Z,a-z,0-9,space,'()+,-./:=?)**

**14h,Len,Value - T61 Teletext String (ITU T.61) (extended 8bit ASCII)**

**16h,Len,Value - IA5 String (aka ISO 646, full 7bit ASCII with @&{}[] etc.)**

**17h,Len,Value - Time String (numeric chars "0-9" and timezone suffix)**

**1Eh,Len,Value - BMP String (aka UTF-16 Unicode) (two-byte chars)**

Allows to define things like company/address/country strings, using various
extended or restriceted variants of the ASCII character set. The most common
type is Printable String (Tag=13h) for basic english, or IA5 String(Tag=16h)
for email addresses with "@" characters.

```

| value[0..(len-1)] = text string |
| --- |

The Time String (Tag=17h) can contain UTC (GMT) time in different variants
(without seconds, with seconds, or with seconds+century; and optional seconds
fraction; terminated by a "Z" character, or by +/-HHMM timezone info):

```

| "YYMMDDHHMMZ" ;\
 "YYMMDDHHMM+HHMM" ; UTC Time without seconds, and optional timezone
 "YYMMDDHHMM-HHMM" ;/
 "YYMMDDHHMMSSZ" ;\
 "YYMMDDHHMMSS+HHMM" ; UTC Time with seconds, and optional timezone
 "YYMMDDHHMMSS-HHMM" ;/
 "YYYYMMDDHHMMSSZ" ;\Generalized Time with seconds and century, and
 "YYYYMMDDHHMMSS.SSSSZ" ;/optional seconds fraction (no trailing zeroes) |
| --- |

**30h,Len,Value - Sequence (or Sequence of)**

**31h,Len,Value - Set (or Set of)**

**A0h,Len,Value - Undoc_A0h**

**A3h,Len,Value - Undoc_A3h**

```

| value[0..(len-1)] = contains further Tag,Length,Data values |
| --- |

XXX

```

| _________________________________ OID Values _________________________________ |
| --- |

```
****************************************************
| 0.9.2342.19200300.100.1.* /itu-t/data/pss/ucl/pilot/pilotAttributeType/..
 0.9.2342.19200300.100.1.1 ../userid ;unused
 0.9.2342.19200300.100.1.25 ../domainComponent ;unused
 1.2.840.113533.7.65.* /iso/member-body/us/nortelnetworks/entrust/nsn-ce/..
 1.2.840.113533.7.65.0 ../0 ;dsi browser equifax_s_ca.ca
 1.2.840.113549.1.1.* /iso/member-body/us/rsadsi/pkcs/pkcs-1/..
 1.2.840.113549.1.1.1 ../rsaEncryption
 1.2.840.113549.1.1.2 ../rsaEncryptionWithMD2 ;dsi browser rsa.ca
 1.2.840.113549.1.1.3 ../rsaEncryptionWithMD4 ;unused
 1.2.840.113549.1.1.4 ../rsaEncryptionWithMD5 ;dsi browser thawte.ca
 1.2.840.113549.1.1.5 ../rsaEncryptionWithSHA1
 1.2.840.113549.1.1.11 ../rsaEncryptionWithSHA256
 1.2.840.113549.1.1.12 ../rsaEncryptionWithSHA384 ;unused
 1.2.840.113549.1.1.13 ../rsaEncryptionWithSHA512 ;unused
 1.2.840.113549.1.1.14 ../rsaEncryptionWithSHA224 ;unused
 1.2.840.113549.1.7.* /iso/member-body/us/rsadsi/pkcs/pkcs-7/..
 1.2.840.113549.1.7.1 ../data ;for nested .der? ;3ds miiverse olv applet
 1.2.840.113549.1.7.6 ../encryptedData ;"inside" pkcs-7
 1.2.840.113549.1.9.* /iso/member-body/us/rsadsi/pkcs/pkcs-9/..
 1.2.840.113549.1.9.1 ../emailAddress
 1.2.840.113549.1.9.21 ../localKeyID ;"inside" pkcs-7
 1.2.840.113549.1.12.1.* /iso/member-body/us/rsadsi/pkcs/pkcs-12/PbeIds/..
 1.2.840.113549.1.12.1.3 ../pbeWithSHAAnd3-KeyTripleDES-CBC ;\inside pkcs-7
 1.2.840.113549.1.12.1.6 ../pbeWithSHAAnd40BitRC2-CBC ;/
 1.2.840.113549.1.12.10.* /iso/member-body/us/rsadsi/pkcs/pkcs-12/Version1/.
 1.2.840.113549.1.12.10.1.2 ../BagIds/ShroudedKeyBag ;-inside pkcs-7
 1.3.6.1.4.1.311.* /iso/org/dod/internet/prv/enterprise/microsoft/..
 1.3.6.1.4.1.311.20.2 ../20/2/? ;incomplete blurp? ;3ds browser securetrust
 1.3.6.1.4.1.311.21.1 ../21.1 ;unknown blurp? ;3ds browser securetrust
 1.3.6.1.5.5.7.1.* /iso/org/dod/internet/security/mechanisms/pkix/pe/..
 1.3.6.1.5.5.7.1.12 ../id-pe-logotype ;3ds shared data TLS, CA_8.der
 1.3.14.3.2.* /iso/org/oiw/secsig/algorithms/..
 1.3.14.3.2.26 ../hashAlgorithmIdentifier ;3ds miiverse olv applet
 2.5.4.* /joint/ds/attributeType/..
 2.5.4.3 ../commonName
 2.5.4.4 ../surname
 2.5.4.5 ../serialNumber
 2.5.4.6 ../countryName
 2.5.4.7 ../localityName
 2.5.4.8 ../stateOrProvinceName
 2.5.4.9 ../streetAddress
 2.5.4.10 ../organizationName
 2.5.4.11 ../organizationalUnit
 2.5.4.12 ../title
 2.5.4.42 ../givenName
 2.5.29.* /joint/ds/certificateExtension/..
 2.5.29.14 ../subjectKeyIdentifier
 2.5.29.15 ../keyUsage
 2.5.29.16 ../privateKeyUsagePeriod ;dsi browser equifax_s_ca.ca
 2.5.29.17 ../subjectAltName ;3ds browser japnese gov
 2.5.29.18 ../issuerAltName ;unused
 2.5.29.19 ../basicConstraints
 2.5.29.20 ../cRLNumber ;unused
 2.5.29.21 ../reasonCode ;unused
 2.5.29.22 ../expirationDate ;unused
 2.5.29.23 ../instructionCode ;unused
 2.5.29.24 ../invalidityDate ;unused
 2.5.29.25 ../cRLDistributionPoints ;unused
 2.5.29.26 ../issuingDistributionPoint ;unused
 2.5.29.27 ../deltaCRLIndicator ;unused
 2.5.29.28 ../issuingDistributionPoint ;unused
 2.5.29.29 ../certificateIssuer ;unused
 2.5.29.30 ../nameConstraints ;unused
 2.5.29.31 ../cRLDistributionPoints
 2.5.29.32 ../certificatePolicies ;dsi browser swiss_g2.ca
 2.5.29.33 ../policyMappings ;unused
 2.5.29.34 ../policyConstraints ;unused
 2.5.29.35 ../authorityKeyIdentifier
 2.5.29.36 ../policyConstraints ;unused
 2.5.29.37 ../extKeyUsage ;3ds browser usertrust
 2.16.840.1.113730.1.* /joint/country/us/organization/netscape/cert-ext/..
 2.16.840.1.113730.1.1 ../cert-type ;dsi browser tc_c2_ca.ca
 2.16.840.1.113730.1.8 ../ca-policy-url ;dsi browser tc_c2_ca.ca
 2.16.840.1.113730.1.13 ../comment |
| --- |

| DSi SD/MMC Firmware Font File |
| --- |

**FAT16:\sys\TWLFontTable.dat ;843.1K (D2C40h bytes) (compressed) (Normal)**

**FAT16:\sys\TWLFontTable.dat ;568.1K (8E020h bytes) (compressed) (China)**

**FAT16:\sys\TWLFontTable.dat ;158.9K (27B80h bytes) (compressed) (Korea)**

This file contains LZrev-compressed fonts in the NFTR (Nitro font) format.

This is the only real long filename that exceeds the 8.3 limit on the DSi
(alternate short name is TWLFON~1.DAT). DSi software is often using a virtual
filename "nand:/<sharedFont>" on ARM9 side, which is then replaced by
"nand:/sys/TWLFontTable.dat" on ARM7 side.

The font can be used by DSiware titles (DSi ROM cartridges usually don't have
eMMC access, and thus cannot use the font).

```

| 0000h 80h RSA-SHA1 on entries [0080h..009Fh] (23h,8Bh,F9h,08h,...)
 0080h 4 Date? (00h,31h,07h,08h=Normal, 00h,27h,05h,09h=China/Korea)
 0084h 1 Number of NFTR resources (NUM) (3=Normal, 9=China/Korea)
 0085h 1 Zerofilled
 0086h 1 Unknown (0=Normal, 4=China, 5=Korea)
 0087h 5 Zerofilled
 008Ch 14h SHA1 on below resource headers at [00A0h+(0..NUM*40h-1)]
 00A0h+N*40h 20h Resource Name in ASCII, padded with 00h
 00C0h+N*40h 4 Compressed Resource Size in .dat file ;\compressed
 00C4h+N*40h 4 Compressed Resource Start in .dat file ;/
 00C8h+N*40h 4 Decompressed Resource Size ;-decompressed
 00CCh+N*40h 14h SHA1 on Compressed Resource at [Start+0..Size-1]
 ... .. Compressed Font Resources (with 16-byte alignment padding) |
| --- |

The resources are containing the same font thrice (at three different sizes),
normal consoles have only three resources (0,1,2), chinese/korean consoles have
nine resources (six zerofilled 40h-byte entries, and three actually used
entries: 3,4,5 for china, or 6,7,8 for korea). The name strings of the
resources are:

```

| "TBF1_l.NFTR" ;0 Large 16x21 pixels ;\Normal (blurry: 4 colors used)
 "TBF1_m.NFTR" ;1 Medium 12x16 pixels ; 2bpp, 7365 characters, Unicode
 "TBF1_s.NFTR" ;2 Small 10x12 pixels ;/
 "TBF1-cn_l.NFTR" ;3 Large 16x21 pixels ;\China (crisp-clear: 2 colors used)
 "TBF1-cn_m.NFTR" ;4 Medium 12x16 pixels ; 2bpp, 7848 characters, Unicode
 "TBF1-cn_s.NFTR" ;5 Small 12x13 pixels ;/
 "TBF1-kr_l.NFTR" ;6 Large 16x21 pixels ;\Korea (crisp-clear: 2 colors used)
 "TBF1-kr_m.NFTR" ;7 Medium 12x16 pixels ; 2bpp, 3679 characters, Unicode
 "TBF1-kr_s.NFTR" ;8 Small 12x12 pixels ;/ |
| --- |

All characters have proportional width (as defined in the Character Width
chunk), eg. the width of the 16x21 font can be max 16 pixels (plus spacing),
but most of the characters are less than 16 pixels wide.

The character numbers in the Char Map chunks are 16bit Unicode, supporting
ASCII, plus extra punctuation marks, european letters with accent marks, greek,
cyrillic, math symbols, and thousands of japanese letters. There are also some
custom nintendo-specific symbols (like buttons and Wii symbols).

**Notes**

The blurry japanese letters aren't actually legible (especially for the smaller
font sizes). Also note that the size of small font varies for the three regions
(ie. 10x12, 12x13, or 12x12).

**LZrev Compressed Font Resource Format**

```

| .. uncompressed area (usually 15h bytes)
 ... compressed area (decompressed backwards)
 .. footer: padding (to 4-byte boundary)
 3 footer: size of footer+compressed area (offset to compressed.bottom)
 1 footer: size of footer (offset to compressed.top)
 4 footer: extra DEST size (offset to decompressed.top)
 .. zeropadding to 10h-byte boundary |
| --- |

LZ Decompression Functions

The original decompression function can be found in Flipnote (EUR) at address
20BF8E4h (which is mainly doing error checking, and then calling the actual
decompression function at 20BF938h) (Flipnote does also contain several custom
fonts, the TWLFontTable.dat file is used only for Flipnote's "Help" function).

**Nitro Font Resources**

The format of the decompressed data is Nintendo's standard Nitro Font format:

DS Cartridge Nitro Font Resource Format

| DS Cartridge Nitro Font Resource Format |
| --- |

**Nitro Font Resource File formats (compressed & uncompressed)**

Nitro Fonts are often found as .NFTR or .ZFTR files (within NitroROM
filesystems). The DSi firmware does additionally contain Nitro Fonts in a .dat
file (in the eMMC FAT16 filesystem).

```

| .NFTR Raw uncompressed Nitro Font Resource
 .ZFTR LZ11-compressed Nitro Font Resource
 .dat Archive with three LZrev-compressed Nitro Font Resources (used on DSi) |
| --- |

The .ZFTR files are containing a 4-byte compression header, followed by the
compressed data (starting with the first compression flag byte, following by
the first chunk header). For details, see:

LZ Decompression Functions

The DSi's "\sys\TWLFontTable.dat" contains three fonts, using a special LZ
compression variant (with the data decompressed backwards, starting at highest
memory address). For details, see:

DSi SD/MMC Firmware Font File

Either way, the decompressed fonts are looking as follows:

**Nitro Font Resource Header Chunk**

```

| 00h 4 Chunk ID "RTFN" (aka NFTR backwards, Nitro Font Resource)
 04h 2 Byte Order (FEFFh) (indicates that above is to be read backwards)
 06h 2 Version (0100h..0102h) (usually 0101h or 0102h)
 08h 4 Decompressed Resource Size (000A3278h) (including the NFTR header)
 0Ch 2 Offset to "FNIF" Chunk, aka Size of "RTFN" Chunk (0010h)
 0Eh 2 Total number of following Chunks (0003h+NumCharMaps) (0018h) |
| --- |

**Font Info Chunk**

```

| 00h 4 Chunk ID "FNIF" (aka FINF backwards, Font Info)
 04h 4 Chunk Size (1Ch or 20h)
 08h 1 Unknown/unused (zero)
 09h xxx 1 Height ;or Height+/-1
 0Ah xxx 1 Unknown (usually 00h, or sometimes 1Fh maybe for chr(3Fh)="?")
 0Bh 2 Unknown/unused (zero)
 0Dh xxx 1 Width ;\or Width+1
 0Eh xxx 1 Width_bis (?) ;/
 0Fh 1 Encoding (0=UTF8, 1=Unicode, 2=SJIS, 3=CP1252) (usually 1)
 10h 4 Offset to Character Glyph chunk, plus 8
 14h 4 Offset to Character Width chunk, plus 8
 18h 4 Offset to first Character Map chunk, plus 8
 1Ch (1) Tile Height ;\present only
 1Dh xxx (1) Max Width or so +/-? ; when above
 1Eh (1) Underline location ; Chunk Size = 20h
 1Fh (1) Unknown/unused (zero) ;/(version 0102h) |
| --- |

**Character Glyph Chunk (Tile Bitmaps)**

```

| 00h 4 Chunk ID "PLGC" (aka CGLP backwards, Character Glyph)
 04h 4 Chunk Size (10h+NumTiles*siz+padding)
 08h 1 Tile Width in pixels
 09h 1 Tile Height in pixels
 0Ah 2 Tile Size in bytes (siz=width*height*bpp+7)/8)
 0Ch 1 Underline location
 0Dh 1 Max proportional Width including left/right spacing
 0Eh 1 Tile Depth (bits per pixel) (usually 1 or 2, sometimes 3)
 0Fh 1 Tile Rotation (0=None/normal, other=see below)
 10h ... Tile Bitmaps
 ... ... Padding to 4-byte boundary (zerofilled) |
| --- |

All tiles are starting on a byte boundary. However, the separate scanlines
aren't necessarily byte-aligned (for example, at 10pix width, a byte may
contain rightmost pixels of one line, followed by leftmost pixels of next
line).

Bit7 of the first byte of a bitmap is the MSB of the upper-left pixel, bit6..0
are then containing the LSB(s) of the pixel (if bpp>1), followed by the next
pixels of the scanline, followed by further scanlines; the data is arranged as
straight Width*Height bitmap (without splitting into 8x8 sub-tiles).

Colors are ranging from Zero (transparent/background color) to all bit(s) set
(solid/text color).

The meaning of the Tile Rotation entry is unclear (one source claims 0=0',
1=90', 2=270', 3=180', and another source claims 0=0', 2=90', 4=180', 6=270',
and for both sources, it's unclear if the rotation is meant to be clockwise or
anti-clockwise).

**Character Width Chunk**

```

| 00h 4 Chunk ID "HDWC" (aka CWDH backwards, Character Width)
 04h 4 Chunk Size (10h+NumTiles*3+padding)
 08h 2 First Tile Number (should be 0000h)
 0Ah 2 Last Tile Number (should be NumTiles-1)
 0Ch 4 Unknown/unused (zero)
 10h+N*3 1 Left Spacing (to be inserted left of character bitmap)
 11h+N*3 1 Width of Character Bitmap (excluding left/right spacing)
 12h+N*3 1 Total Width of Character (including left/right spacing)
 ... ... Padding to 4-byte boundary (zerofilled) |
| --- |

Defines the proportional character width for each tile. Entry [11h+N*3] defines
the width of the non-transparent character area (which left-aligned in the Tile
Bitmap; any further pixels in the Bitmap are unused/zero). The other two
entries define the left/right spacing that is needed to be added to the
character.

**Character Map(s) - Translation Tables for ASCII/JIS/etc to Tile Numbers?**

```

| 00h 4 Chunk ID "PAMC" (aka CMAP backwards, Character Map)
 04h 4 Chunk Size (14h+...+padding)
 08h 2 First Character (eg. 0020h=First ASCII Char)
 0Ah 2 Last Character (eg. 007Eh=Last ASCII Char)
 0Ch 4 Map Type (0..2, for entry 14h and up, see there)
 10h 4 Offset to next Character Map, plus 8 (0=None, no further) |
| --- |

For Map Type0, Increasing TileNo's assigned to increasing CharNo's:

```

| 14h 2 TileNo for First Char (and increasing for further chars)
 16h 2 Padding to 4-byte boundary (zerofilled) |
| --- |

For Map Type1, Custom TileNo's assigned to increasing CharNo's:

```

| 14h+N*2 2 TileNo's for First..Last Char (FFFFh=None; no tile assigned)
 ... 0/2 Padding to 4-byte boundary (zerofilled) |
| --- |

For Map Type2, Custom TileNo's assigned to custom CharNo's:

```

| 14h 2 Number of following Char=Tile groups...
 16h+N*4 2 Character Number
 18h+N*4 2 Tile Number
 ... 2 Padding to 4-byte boundary (zerofilled) |
| --- |

These chunks are containing tables for translating character numbers (eg.
Unicode numbers, or whatever format is selected in the Font Info chunk) to
actual Tile Numbers (ie. the way how tiles are ordered in the Glyph and Width
chunks).

Font files can contain several Character Map chunks (eg. some Type0 chunks for
Char 0020h..007Eh and Char 00A0h..00FFh, plus some Type1 chunks for areas like
Char 037Eh..0451h, plus one large Type2 chunk for everything that wasn't
defined in the other chunks; the First/Last Character entries are don't care
for Type2, they are usually set to First=0000h and Last=FFFFh in that case).
Characters that are NOT included in any of the tables should be treated as
undefined (as so for any characters that are assigned as Tile=FFFFh in Type1
chunks).

Unicode character numbers are stored as 16bit values. Unknown how other
character numbers like UTF8 or SJIS are stored.

| LZ Decompression Functions |
| --- |

**LZSS, Yaz0, LZ11 - Decompression for BIOS/SWI and ZFTR Font Files**

LZSS/Yaz0/LZ11 are very similar formats, with small differences in the header
and compression codes, which could be summarized as so:

```

| 10,ss,ss,Ss ;ID(10h), Size(Ssssss) ;-LZSS header
 11,ss,ss,Ss ;ID(11h), Size(Ssssss) ;-LZ11 header
 "Yaz0",Ss,ss,ss,ss,0,0,0,0,0,0,0,0 ;-Yaz0 header
 Ff ;Flags(Ff), each 8 entries ;-Flags (bit7=first)
 1N,nn,nP,pp ;Disp(Ppp)+1, Len(Nnnn)+111h ;\
 0N,nP,pp ;Disp(Ppp)+1, Len(Nn)+11h ; LZ11, when flag=1
 NP,pp ;Disp(Ppp)+1, Len(N)+1 ;/
 0P,pp,Nn ;Disp(Ppp)+1, Len(Nn)+12h ;\Yaz0, when flag=0
 NP,pp ;Disp(Ppp)+1, Len(N)+2 ;/
 NP,pp ;Disp(Ppp)+1, Len(N)+3 ;-LZSS, when flag=1
 Dd ;Databyte(Dd) ;-When flag other than above |
| --- |

Below function can decompress LZSS data (as used by BIOS SWIs), and the LZ11
variant (with 11h in the header; as used by ZFTR font files, but not being
compatible with the BIOS SWI functions on neither GBA nor NDS nor DSi).

```

| if src has "LZ77" or "CMPR" prefix then src=src+4 ;skip prefix (if any)
 typ=byte[src]
 if typ=59h ;YAZ0
 errif (bytes[src+0..3]<>"Yaz0") OR (bytes[src+8..15]<>0)
 fin=dst+BigEndian(word[src+04h]), src=src+16
 else ;LZSS/LZ11
 fin=dst+(word[src]/100h), src=src+4
 endif
 @@collect_more:
 flagbits=[src], src=src+1, numflags=8
 if typ=59h then flagbits=flagbits XOR 0FFh ;-invert for YAZ0
 @@decompress_lop:
 if dst>=fin then goto @@decompress_done
 if numflags=0 then goto @@collect_more
 numflags=numflags-1, flagbits=flagbits*2
 if (flagbits AND 100h)=0 then
 [dst]=[src], dst=dst+1, src=src+1
 else
 if typ=10h ;LZ10 aka LZSS (BIOS SWI compatible)
 len=3
 len=len+[src]/10h, disp=001h+([src] AND 0Fh)*100h+[src+1], src=src+2
 elseif typ=11h ;LZ11 (special extended format)
 if [src]/10h>1 then len=001h
 if [src]/10h<1 then len=011h+([src] AND 0Fh)*10h, src=src+1
 if [src]/10h=1 then len=111h+([src] AND 0Fh)*1000h+[src+1]*10h, src=src+2
 len=len+[src]/10h, disp=001h+([src] AND 0Fh)*100h+[src+1], src=src+2
 elseif typ=59h ;YAZ0 (special extended format)
 len=[src]/10h, disp=001h+([src] AND 0Fh)*100h+[src+1], src=src+2
 if len=0 then len=[src]+12h, src=src+1, else len=len+02h
 endif
 for i=1 to len, [dst]=[dst-disp], dst=dst+1, next i
 endif
 goto @@decompress_lop
 @@decompress_done:
 ret |
| --- |

The LZSS variant is implemented in GBA/NDS/DSi BIOS SWI functions:

BIOS Decompression Functions

LZSS and LS11 are used with many NDS/DSi/3DS files. LZSS/LZ11 can be detected
by checking for 10h/11h in first byte, and additionally checking for:

```

| Extension is ".lz", ".lz77", ".LZ", or extension is preceeded by "_LZ."
 Less common extensions are ".l", ".lex", ".cmp" (and various others)
 Bytes[5..10] (after header+flags) contain IDs alike "NARC",FFFEh etc.
 File starts with "LZ77",10h or "CMPR",10h (both rather uncommon) |
| --- |

Prefix strings exist in a few LZSS files ("LZ77" in Super Mario 64 DS, or
"CMPR" in Nanostray), then followed by normal LZSS data starting with type=10h.

Yaz0 files can be detected by checking for "Yaz0" in first 4 bytes. Yaz0 is
used by a few titles only (eg. used by DSi Sound, rom:\texture.szs). Yaz0 is
widely believed to use RLE compression (don't ask why).

**LZrev - Reverse Decompression**

```

| Used for DSi Font (within TWLFontTable.dat)
 Used for 3DS .code files (within NCCH ExeFS filesystems)
 Used for files with .blz extension (eg. in DSi Flipnote, DSi Paper Plane)
 Used for ALZ1+LZrev double compression with .blz extension (eg. DSi Dr Mario) |
| --- |

This function resembles LZSS, but with src/dst processed in reversed order
(starting at highest memory location). Further differences are that the header
is replaced by a footer, and, weirdly, data is copied from "disp+3" instead of
"disp+1". The decompression is intended to use a single buffer for src/dst (so
the source data will be overwritten during decompression) (in order to avoid
overwriting still unprocessed data, some bytes near start of file are usually
left uncompressed, eg. for the font files, decompression usually ends somewhere
at fin=buf+15h rather than at fin=buf+0).

```

| dest_size=src_size+[src+src_size-4] ;when dest_size is unknown (for 3DS)
 allocate buf(dest_size), copy "src_size" bytes from file to buf
 src=buf+src_size ;origin = pointing after footer
 dst=src+(word[src-4])-1 ;dst = src plus extra len
 fin=src-(word[src-8] AND 00FFFFFFh) ;fin = src minus compressed_len
 src=src-(byte[src-5])-1 ;src = src minus footer_len
 @@collect_more:
 flagbits=[src], src=src-1, numflags=8
 @@decompress_lop:
 if src<=fin then goto @@decompress_done
 if numflags=0 then goto @@collect_more
 numflags=numflags-1, flagbits=flagbits*2
 if (flagbits AND 100h)=0 then
 [dst]=[src], dst=dst-1, src=src-1
 else
 len=([src]/10h)+3, disp=([src] AND 0Fh)*100h+([src-1])+3, src=src-2
 for i=1 to len, [dst]=[dst+disp], dst=dst-1, next i
 endif
 goto @@decompress_lop
 @@decompress_done:
 ret |
| --- |

Note: The footer_len at [src-5] is usually 08h+0..3 (with 0..3 padding bytes to
make the footer entries & file size word aligned).

**LZ/Huffman**

Apart from the above chubby functions, there are at least two compression
schemes trying to implement real serious compression:

GBA Cart e-Reader VPK Decompression

LZ Decompression Functions ASH0

Some titles contain regular .zip files (with Stored and Deflated files):

```

| DSi Browser (rom:\skin\std_skin.zip) -- corrupted .zip (with 0D,0A appended)
 DSi Shop (rom:\skin\std_skin.zip) -- intact .zip (unlike above)
 3DS Browser applet (RomFS:\browser\page\..\*.zip) |
| --- |

ZIP Decompression

**Others**

Arika has brewed up a obscure compression+archive+encryption format:

DS Encrypted Arika Archives with ALZ1 compression

DS Files - Vicarious Vision GFC/GOB Archives with ZLIB compression

| LZ Decompression Functions ASH0 |
| --- |

ASH0 is used by several games on different consoles (eg. DSi Flipnote,
rom:\SampleData.Eu\*.ash).

**ash_decompress(source,dest)**

```

| collected[0]=80000000h ;stream 0 initially empty (endflag in bit31)
 collected[1]=80000000h ;stream 1 initially empty (endflag in bit31)
 src[0]=source+0 ;stream 0 start
 for i=0 to 3, id[i]=read_bits(0,8), next i, if id[0..3]<>'ASH0' then error
 dest_end=dest+read_bits(0,32) ;size of decompressed data
 src[1]=source+read_bits(0,32) ;stream 1 start
 load_huffman_tree(0,9) ;load stream 0 tree (9bit len/data values)
 load_huffman_tree(1,11) ;load stream 1 tree (11bit disp values)
 decompress_loop:
 temp=0, while temp<8000h do temp=tree[0][temp+read_bits(0,1)]
 if temp<8100h then
 [dest]=temp AND FFh, dest=dest+1 ;store one uncompressed data byte
 else
 disp=0, while disp<8000h do disp=tree[1][disp+read_bits(1,1)]
 len=temp+3-8100h, disp=disp+1-8000h
 for j=1 to len, [dest]=[dest-disp], dest=dest+1, next j
 if dest<dest_end then decompress_loop
 ret |
| --- |

**read_bits(s,num)**

```

| data=0
 for i=1 to num
 shl collected[s],1 ;move next bit to carry, or set zeroflag if empty
 if zeroflag
 collected[s]=[src[s]]*1000000h+[src[s]+1]*10000h+[src[s]+2]*100h+[src[s]+3]
 src[s]=src[s]+4 ;read data in 32bit units, in reversed byte-order
 carryflag=1 ;endbit
 rcl collected[s],1 ;move bit31 to carry (and endbit to bit0)
 rcl data,1 ;move carry to data
 next i
 ret(data) |
| --- |

**load_huffman_tree(s,width)**

```

| stacktop=sp
 tree_index=0
 load_lop:
 if getbits(0,1)=1
 push tree_index+1 ;right (bit0=right) ;\
 push tree_index ;left ; memorize left/right indices
 tree_index=tree_index+2 ; and load more
 goto load_lop ;/
 dta=getbits(0,width)+8000h ;get data (bit15=data)
 resolve_lop:
 pop idx ;parent index
 tree[s][idx]=dta ;store data (or child_index)
 if (idx AND 1)=0 then goto load_lop ;load more (if it was left node)
 dta=idx-1 ;child_index (to be stored at next parent_index)
 if stacktop<>sp then goto resolve_lop
 ret |
| --- |

Note: The official max tree sizes appear to be:

```

| tree[0] = about 200h*6 halfwords
 tree[1] = about 800h*6 halfwords |
| --- |

| ZIP Decompression |
| --- |

**Overall ZIP structure**

```

| Local file headers, with file data
 Central directory, with file headers, and optional signature
 End of central directory record, with optional comment |
| --- |

**ZIP Local file header**

```

| 000h 4 local file header signature (04034B50h) ("PK",3,4)
 004h 2 version needed to extract (os*256+major*10+minor) (common=14h=v2.0)
 006h 2 general purpose bit flag (common=0)
 008h 2 compression method (0=stored, 8=deflated, other=rare?)
 00Ah 2 last mod file time
 00Ch 2 last mod file date
 00Eh 4 crc32 of uncompressed file data (zero when size=0)
 012h 4 compressed size
 016h 4 uncompressed size
 01Ah 2 file name length
 01Ch 2 extra field length
 01Eh .. file name (or "folder/file" name)
 ... .. extra field
 ... .. compressed file data |
| --- |

**ZIP Central directory structure**

Central directory entries:

```

| 000h 4 central file header signature (02014B50h) ("PK",1,2)
 004h 2 version made by
 006h 2 version needed to extract
 008h 2 general purpose bit flag
 00Ah 2 compression method (0=stored, 8=deflated, other=rare?)
 00Ch 2 last mod file time
 00Eh 2 last mod file date
 010h 4 crc32 of uncompressed file data
 014h 4 compressed size
 018h 4 uncompressed size
 01Ch 2 file name length
 01Eh 2 extra field length
 020h 2 file comment length
 022h 2 disk number start
 024h 2 internal file attributes
 026h 4 external file attributes
 02Ah 4 offset of local file header (from begin of disk number [022h])
 02Eh .. file name (or "folder/file" name)
 ... .. extra field
 ... .. file comment (usually ASCII text, if any) |
| --- |

Central directory signature (optional, after last directory entry)

```

| 000h 4 header signature (05054B50h) ("PK",5,5)
 004h 2 size of data
 006h .. signature data (variable size, whatever crap) |
| --- |

**ZIP End of central directory record**

```

| 000h 4 end of central dir signature (06054B50h) ("PK",5,6)
 004h 2 number of this disk (usually 0000h)
 006h 2 number of disk with the start of central directory
 008h 2 total number of entries in the central directory on this disk
 00Ah 2 total number of entries in the central directory
 00Ch 4 size of the central directory
 010h 4 offset of start of central directory, on disk number [006h]
 014h 2 .ZIP file comment length (can be zero even if comment/junk follows)
 016h .. .ZIP file comment (usually ASCII text, if any) |
| --- |

**Official specs**

The official ZIP specs can be reportedly found on pkware.com (but it's unknown
which browsers are capable of viewing that webpage):

```
[http://support.pkware.com/home/pkzip/developer-tools/appnote](http://support.pkware.com/home/pkzip/developer-tools/appnote)
| |
| --- |

Alternately, the currently latest specs can be found here:

```
[http://pkware.cachefly.net/webdocs/APPNOTE/APPNOTE-6.3.9.TXT](http://pkware.cachefly.net/webdocs/APPNOTE/APPNOTE-6.3.9.TXT)
| ;July 2020 |
| --- |

That specs include info on rarely used things (including other compression
methods, encryption, compressed directory entries, and support for 64bit
offset/size values).

**Inflate/Deflate**

Inflate/Deflate is a common (de-)compression algorithm, used in ZIP files since
1991. And also used by ZLIB, GZIP, PNG. The DSi Browsers GET command includes
the string "Accept-Encoding: deflate, gzip, x-gzip, identity", so it's
apparently also used for internet transfers. In the PSX world, it's also used
by the .CDZ cdrom-image format.

Inflate - Core Functions

Inflate - Initialization & Tree Creation

Inflate - Headers and Checksums

Note: Very old ZIP versions from 1989/1990 did use Shrink and Implode instead
of Deflate.

| Inflate - Core Functions |
| --- |

**tinf_uncompress(dst,src)**

```

| tinf_init() ;init constants (needed to be done only once)
 tinf_align_src_to_byte_boundary()
 repeat
 bfinal=tinf_getbit() ;read final block flag (1 bit)
 btype=tinf_read_bits(2) ;read block type (2 bits)
 if btype=0 then tinf_inflate_uncompressed_block()
 if btype=1 then tinf_build_fixed_trees(), tinf_inflate_compressed_block()
 if btype=2 then tinf_decode_dynamic_trees(), tinf_inflate_compressed_block()
 if btype=3 then ERROR ;reserved
 until bfinal=1
 tinf_align_src_to_byte_boundary()
 ret |
| --- |

**tinf_inflate_uncompressed_block()**

```

| tinf_align_src_to_byte_boundary()
 len=LittleEndian16bit[src+0] ;get len
 if LittleEndian16bit[src+2]<>(len XOR FFFFh) then ERROR ;verify inverse len
 src=src+4 ;skip len values
 for i=0 to len-1, [dst]=[src], dst=dst+1, src=src+1, next i ;copy block
 ret |
| --- |

**tinf_inflate_compressed_block()**

```

| repeat
 sym1=tinf_decode_symbol(tinf_len_tree)
 if sym1<256
 [dst]=sym1, dst=dst+1
 if sym1>256
 len = tinf_read_bits(length_bits[sym1-257])+length_base[sym1-257]
 sym2 = tinf_decode_symbol(tinf_dist_tree)
 dist = tinf_read_bits(dist_bits[sym2])+dist_base[sym2]
 for i=0 to len-1, [dst]=[dst-dist], dst=dst+1, next i
 until sym1=256
 ret |
| --- |

**tinf_decode_symbol(tree)**

```

| sum=0, cur=0, len=0
 repeat ;get more bits while code value is above sum
 cur=cur*2 + tinf_getbit()
 len=len+1
 sum=sum+tree.table[len]
 cur=cur-tree.table[len]
 until cur<0
 return tree.trans[sum+cur] |
| --- |

**tinf_read_bits(num) ;get N bits from source stream**

```

| val=0
 for i=0 to num-1, val=val+(tinf_getbit() shl i), next i
 return val |
| --- |

**tinf_getbit() ;get one bit from source stream**

```

| bit=tag AND 01h, tag=tag/2
 if tag=00h then tag=[src], src=src+1, bit=tag AND 01h, tag=tag/2+80h
 return bit |
| --- |

**tinf_align_src_to_byte_boundary()**

```

| tag=01h ;empty/end-bit (discard any bits, align src to byte-boundary)
 ret |
| --- |

| Inflate - Initialization & Tree Creation |
| --- |

**tinf_init()**

```

| tinf_build_bits_base(length_bits, length_base, 4, 3)
 length_bits[28]=0, length_base[28]=258
 tinf_build_bits_base(dist_bits, dist_base, 2, 1)
 ret |
| --- |

**tinf_build_bits_base(bits,base,delta,base_val)**

```

| for i=0 to 29
 bits[i]=min(0,i-delta)/delta
 base[i]=base_val
 base_val=base_val+(1 shl bits[i])
 ret |
| --- |

**tinf_build_fixed_trees()**

```

| for i=0 to 6, tinf_len_tree.table[i]=0, next i ;[0..6]=0 ;len tree...
 tinf_len_tree.table[7,8,9]=24,152,112 ;[7..9]=24,152,112
 for i=0 to 23, tinf_len_tree.trans[i+0] =i+256, next i ;[0..23] =256..279
 for i=0 to 143, tinf_len_tree.trans[i+24] =i+0, next i ;[24..167] =0..143
 for i=0 to 7, tinf_len_tree.trans[i+168]=i+280, next i ;[168..175]=280..287
 for i=0 to 111, tinf_len_tree.trans[i+176]=i+144, next i ;[176..287]=144..255
 for i=0 to 4, tinf_dist_tree.table[i]=0, next i ;[0..4]=0,0,0,0,0 ;\dist
 tinf_dist_tree.table[5]=32 ;[5]=32 ; tree
 for i=0 to 31, tinf_dist_tree.trans[i]=i, next i ;[0..31]=0..31 ;/
 ret |
| --- |

**tinf_decode_dynamic_trees()**

```

| hlit = tinf_read_bits(5)+257 ;get 5 bits HLIT (257-286)
 hdist = tinf_read_bits(5)+1 ;get 5 bits HDIST (1-32)
 hclen = tinf_read_bits(4)+4 ;get 4 bits HCLEN (4-19)
 for i=0 to 18, lengths[i]=0, next i
 for i=0 to hclen-1 ;read lengths for code length alphabet
 lengths[clcidx[i]]=tinf_read_bits(3) ;get 3 bits code length (0-7)
 tinf_build_tree(code_tree, lengths, 19) ;build code length tree
 for num=0 to hlit+hdist-1 ;decode code lengths for dynamic trees
 sym = tinf_decode_symbol(code_tree)
 len=1, val=sym ;default (for sym=0..15)
 if sym=16 then len=tinf_read_bits(2)+3, val=lengths[num-1] ;3..6 previous
 if sym=17 then len=tinf_read_bits(3)+3, val=0 ;3..10 zeroes
 if sym=18 then len=tinf_read_bits(7)+11, val=0 ;11..138 zeroes
 for i=1 to len, lengths[num]=val, num=num+1, next i
 tinf_build_tree(tinf_len_tree, 0, hlit) ;\build trees
 tinf_build_tree(tinf_dist_tree, 0+hlit, hdist) ;/
 ret |
| --- |

**tinf_build_tree(tree, first, num)**

```

| for i=0 to 15, tree.table[i]=0, next i ;clear code length count table
 ;scan symbol lengths, and sum code length counts...
 for i=0 to num-1, x=lengths[i+first], tree.table[x]=tree.table[x]+1, next i
 tree.table[0]=0
 sum=0 ;compute offset table for distribution sort
 for i=0 to 15, offs[i]=sum, sum=sum+tree.table[i], next i
 for i=0 to num-1 ;create code to symbol xlat table (symbols sorted by code)
 x=lengths[i+first], if x<>0 then tree.trans[offs[x]]=i, offs[x]=offs[x]+1
 next i
 ret |
| --- |

**tinf_data**

```

| clcidx[0..18] = 16,17,18,0,8,7,9,6,10,5,11,4,12,3,13,2,14,1,15 ;constants |
| --- |

```

| typedef struct TINF_TREE:
 unsigned short table[16] ;table of code length counts
 unsigned short trans[288] ;code to symbol translation table |
| --- |

```

| TINF_TREE tinf_len_tree ;length/symbol tree
 TINF_TREE tinf_dist_tree ;distance tree
 TINF_TREE code_tree ;temporary tree (for generating the dynamic trees)
 unsigned char lengths[288+32] ;temporary 288+32 x 8bit ;\for dynamic tree
 unsigned short offs[16] ;temporary 16 x 16bit ;/creation |
| --- |

```

| unsigned char length_bits[30]
 unsigned short length_base[30]
 unsigned char dist_bits[30]
 unsigned short dist_base[30] |
| --- |

| Inflate - Headers and Checksums |
| --- |

**tinf_gzip_uncompress(dst, destLen, src, sourceLen)**

```

| src_start=src, dst_start=dst ;memorize start addresses
 if (src[0]<>1fh or src[1]<>8Bh) then ERROR ;check id bytes
 if (src[2]<>08h) then ERROR ;check method is deflate
 flg=src[3] ;get flag byte
 if (flg AND 0E0h) then ERROR ;verify reserved bits
 src=src+10 ;skip base header
 if (flg AND 04h) then src=src+2+LittleEndian16bit[src] ;skip extra data
 if (flg AND 08h) then repeat, src=src+1, until [src-1]=00h ;skip file name
 if (flg AND 10h) then repeat, src=src+1, until [src-1]=00h ;skip file comment
 hcrc=(tinf_crc32(src_start, src-src_start) & 0000ffffh)) ;calc header crc
 if (flg AND 02h) then x=LittleEndian16bit[src], src=src+2 ;get header crc
 if (flg AND 02h) then if x<>hcrc then ERROR ;verify header
 tinf_uncompress(dst, destLen, src, src_start+sourceLen-src-8) ;----> inflate
 crc32=LittleEndian32bit[src], src=src+4 ;get crc32 of decompressed data
 dlen=LittleEndian32bit[src], src=src+4 ;get decompressed length
 if (dlen<>destLen) then ERROR ;verify dest len
 if (crc32<>tinf_crc32(dst_start,dlen)) then ERROR ;verify crc32
 ret |
| --- |

**tinf_quasijarus_uncompress(dst, destLen, src, sourceLen)**

```

| src_start=src, dst_start=dst ;memorize start addresses
 if (src[0]<>1fh or src[1]<>A1h) then ERROR ;check id bytes
 src=src+2 ;skip header
 tinf_uncompress(dst, destLen, src, sourceLen-0Ah) ;----> inflate
 crc32=LittleEndian32bit[src], src=src+4 ;get crc32 of decompressed data
 dlen=LittleEndian32bit[src], src=src+4 ;get decompressed length
 if (dlen<>destLen) then ERROR ;verify dest len
 if (crc32<>tinf_crc32(dst_start,dlen)) then ERROR ;verify crc32
 ret |
| --- |

**tinf_zlib_uncompress(dst, destLen, src, sourceLen)**

```

| src_start=src, dst_start=dst ;memorize start addresses
 hdr=BigEndian16bit[src], src=src+2 ;get header
 if (hdr MOD 31)<>0 then ERROR ;check header checksum (modulo)
 if (hdr AND 20h)>0 then ERROR ;check there is no preset dictionary
 if (hdr AND 0F00h)<>0800h then ERROR ;check method is deflate
 if (had AND 0F000h)>7000h then ERROR ;check window size is valid
 tinf_uncompress(dst, destLen, src, sourceLen-6) ;------> inflate
 chk=BigEndian32bit[src], src=src+4 ;get data checksum
 if src-src_start<>sourceLen then ERROR ;verify src len
 if dst-dst_start<>destLen then ERROR ;verify dst len
 if a32<>tinf_adler32(dst_start,destLen)) then ERROR ;verify data checksum
 ret |
| --- |

**tinf_adler32(src, length)**

```

| s1=1, s2=0
 while (length>0)
 k=max(length,5552) ;max length for avoiding 32bit overflow before mod
 for i=0 to k-1, s1=s1+[src], s2=s2+s1, src=src+1, next i
 s1=s1 mod 65521, s2=s2 mod 65521, length=length-k
 return (s2*10000h+s1) |
| --- |

| PNG Bitmaps |
| --- |

Used by DSi Browser (rom:\*.png)

**File Header**

```

| 000h 8 ID (89h,'PNG',0Dh,0Ah,1Ah,0Ah)
 008h .. Chunks |
| --- |

**Chunk Format**

```

| 000h 4 Chunk Size (LEN) ;big-endian
 004h 4 Chunk ID (4-letter ASCII) ("A..Z" and "a..z")
 008h LEN Chunk Data
 ... 4 Chunk CRC32 on [004h+(0..LEN+3)] ;big-endian |
| --- |

The Chunk ID is 4-letter ASCII, with upper/lowercase having special meanings:

```

| 1st char Uppercase=Critical (Basic) Lowercase=Ancillary (Optional extras)
 2nd char Uppercase=Public (Official) Lowercase=Private (Custom extensions)
 3rd char Uppercase=Normal Lowercase=Reserved
 4th char Uppercase=Unsafe to copy Lowercase=Safe to copy |
| --- |

PNG Chunks

```

| IHDR v1.0 Image header (first chunk) ;\
 PLTE v1.0 Palette (if any) ; Critical chunks
 IDAT v1.0 Image data (can be multiple IDAT chunks!) ; (aka the bitmap data)
 IEND v1.0 Image trailer (last chunk) ;/
 tRNS v1.0 Transparency ;-Transparency
 cHRM v1.0 Primary chromaticities and white point ;\
 gAMA v1.0 Image gamma ;
 iCCP v1.1 Embedded ICC profile ; Colour space info
 sBIT v1.0 Significant bits ;
 sRGB v1.1 Standard RGB colour space ;
 cICP v3rd Coding-independent code points ;/
 bKGD v1.0 Background colour ;\
 hIST v1.0 Image histogram ;
 pHYs v1.0 Physical pixel dimensions ; Miscellaneous info
 sPLT v1.1 Suggested palette ;
 eXIf v3rd EXIF chunk (starts with "MM"=byteorder) ;/
 tEXt v1.0 Textual data ;\Textual information
 zTXt v1.0 Compressed textual data ; (aka comments)
 iTXt v1.2 International textual data ;/
 tIME v1.0 Image last-modification time ;-Time stamp
 acTL v3rd Animation Control Chunk ;\
 fcTL v3rd Frame Control Chunk ; Animation (APNG)
 fdAT v3rd Frame Data Chunk ;/ |
| --- |

Note: There are some rules for the chunk ordering, see PNG specs for details.

**IHDR Chunk (first chunk)**

```

| 000h 4 Chunk Size (always 0Dh) ;big-endian
 004h 4 Chunk ID ("IHDR")
 008h 4 Width in pixels (0=invalid) ;big-endian
 00Ch 4 Height in pixels (0=invalid) ;big-endian
 010h 1 Bits per sample (1,2,4,8,16) (allowed values depend on Color Type)
 011h 1 Color Type (0,2,3,4,6)
 012h 1 Compression method (always 0=Zlib-Deflate)
 013h 1 Filter method (always 0=Adaptive; with subtype per line)
 014h 1 Interlace method (0=None, 1=Adam7)
 015h 4 Chunk CRC32 on [004h..] ;big-endian |
| --- |

The valid Color types and bits per sample are:

```

| Color type (Samples) Bits/sample Bits/pixel
 0=Grayscale (I) 1,2,4,8,16 --> 1,2,4,8,16
 2=Truecolor (RGB) 8,16 --> 24,48
 3=Palette indices (I) 1,2,4,8 --> 1,2,4,8
 4=Grayscale with alpha (IA) 8,16 --> 16,32
 6=Truecolor with alpha (RGBA) 8,16 --> 32,64 |
| --- |

**PLTE Chunk (required for Color type 3)**

```

| 000h 4 Chunk Size (LEN=NumColors*3) (3..300h) ;big-endian
 004h 4 Chunk ID ("PLTE")
 008h LEN Palette Data (three bytes per color, ordered R,G,B)
 ... 4 Chunk CRC32 on [004h+(0..LEN+3)] ;big-endian |
| --- |

Additional Alpha values for each palette entry can be defined in tRNS chunk.

**IDAT Chunk(s)**

```

| 000h 4 Chunk Size (LEN) ;big-endian
 004h 4 Chunk ID ("IDAT")
 008h LEN Zlib-Deflate compressed image data
 ... 4 Chunk CRC32 on [004h+(0..LEN+3)] ;big-endian |
| --- |

There may be multiple IDAT chunks; if so, they shall appear consecutively with
no other "intervening" chunks. The compressed datastream is then the
"concatenation" of the contents of the data fields of all the IDAT chunks.

**IEND Chunk (last chunk)**

```

| 000h 4 Chunk Size (0) ;big-endian
 004h 4 Chunk ID ("IEND")
 008h 4 Chunk CRC32 on [004h..007h] (always AE426082h) ;big-endian |
| --- |

```

| _________________ Decompression, Unfiltering, Deinterlacing __________________ |
| --- |

**Overall image decoding flowchart**

- merge SDAT chunk(s) into a continous bitstream (remove Chunk Headers/CRCs)

- decompress via Zlib-Inflate

- unfilter

- convert to desired color depth and RGBA bit-order

- deinterlace

**Decompression**

The merged IDAT chunks contain:

```

| 000h 1 zlib compression method/flags code (usually 78h)
 001h 1 Additional flags/check bits (usually 9Ch) (or DAh)
 002h .. Compressed data blocks (deflate)
 ... 4 Adler32 checksum on decompressed data ;big-endian |
| --- |

ZIP Decompression

"tinf_zlib_uncompress"

All scanlines are padded to 1-byte boundary, and all scanlines include a
leading filter bytes. That is, the decompressed size is:

```

| decompressed_size = ((width*bpp+7)/8+1)*height |
| --- |

For interlaced images, compute the width & height for each pass, then
compute the sum of the decompressed_size's.

**Unfiltering**

Pixels can have 1..64 bpp, but unfiltering is always done in BYTE units.

```

| for pass=1 to num_passes ;for 7-pass interlacing
 calc width/height for current pass
 for x=1 to (bpp+7)/8 + (width*bpp+7)/8
 byte[dst]=00h, dst=dst+1 ;-pad topmost for unfiltering
 for y=1 to height
 filter = byte[src], src=src+1
 for x=1 to (bpp+7)/8
 byte[dst]=00h, dst=dst+1 ;-pad leftmost for unfiltering
 for x=1 to (width*bpp+7)/8
 left = byte[dst-(bpp+7)/8]
 upper = byte[dst-bytes_per_scanline]
 upperleft = byte[dst-bytes_per_scanline-(bpp+7)/8]
 if filter=0 then byte[dst]=byte[src]
 if filter=1 then byte[dst]=byte[src]+left
 if filter=2 then byte[dst]=byte[src]+upper
 if filter=3 then byte[dst]=byte[src]+(left+upper)/2
 if filter=4 then
 pa=abs(upper-upperleft)
 pb=abs(left-upperleft)
 pc=abs(left+upper-(upperleft*2))
 if pa<=pb AND pa<=pc then byte[dst]=byte[src]+left
 elseif pb<=pc then byte[dst]=byte[src]+upper
 else byte[dst]=byte[src]+upperleft
 if filter>4 then error
 src=src+1, dst=dst+1
 next x
 next y
 next pass |
| --- |

Note: The dst buffer may be as small as two scanlines (when directly forwarding
the unfiltered data to the color conversion & deinterlacing steps).

**Color Depth conversion and RGBA bit-order**

The MSB of first pixel is in bit7 of first byte (ie. 16bit samples are
big-endian, and 2bpp would have 1st pixel in bit7-6). Pixels with multiple
samples are ordered R,G,B or R,G,B,A or Luminance,A.

The data should be converted to a format supported by the OS/hardware. Also,
deinterlacing will be easier when converting to at least 8bpp (or simply
convert everything to 32bpp).

**Deinterlacing (if enabled in IHDR chunk)**

Interlace allows to display a low-resolution preview during slow network
downloads. The image is divided into 8x8 pixel blocks.

```

| Pass Numbers per 8x8: Width/Height per pass:
 1 6 4 6 2 6 4 6 Pass 1 (width+7)/8, (height+7)/8 ;\
 7 7 7 7 7 7 7 7 Pass 2 (width+3)/8, (height+7)/8 ;
 5 6 5 6 5 6 5 6 Pass 3 (width+3)/4, (height+3)/8 ; seven-pass 7 7 7 7 7 7 7 7 Pass 4 (width+1)/4, (height+3)/4 ; (Adam7
 3 6 4 6 3 6 4 6 Pass 5 (width+1)/2, (height+1)/4 ; interlace)
 7 7 7 7 7 7 7 7 Pass 6 (width+0)/2, (height+1)/2 ;
 5 6 5 6 5 6 5 6 Pass 7 (width+0)/1, (height+0)/2 ;/
 7 7 7 7 7 7 7 7 Single (width+0)/1, (height+0)/1 ;-single-pass |
| --- |

For example, a 12x2 pixel image would be interlaced as so:

```

| 1 6 4 6 2 6 4 6 1 6 4 6 ;\Pass1=2x1, Pass2=1x1, Pass3=Empty, Pass4=4x1,
 7 7 7 7 7 7 7 7 7 7 7 7 ;/Pass5=Empty, Pass6=6x1, Pass7=12x1. |
| --- |

All seven passes are stored in a single Zlib-bitstream (for low-res preview,
one must pause the Zlib decompression every once and then to display the yet
downloaded pixels).

```

| ____________________________________ Misc ____________________________________ |
| --- |

**tRNS Chuunk**

Colour type 0 Grayscale

```

| Grey sample value 2 bytes (big-endian) (0..SampleSize-1) |
| --- |

Colour type 2 RGB

```

| Red sample value 2 bytes (big-endian) (0..SampleSize-1)
 Blue sample value 2 bytes (big-endian)
 Green sample value 2 bytes (big-endian) |
| --- |

Colour type 3 Color-indexed

```

| Alpha for palette index 0 1 byte (alpha value, 00h..FFh; FFh=Solid)
 Alpha for palette index 1 1 byte
 ...etc... 1 byte |
| --- |

**References**

Official PNG specs are filled with overhelming details (without actually
getting to the point).

```
[http://www.libpng.org/pub/png/spec/1.0/](http://www.libpng.org/pub/png/spec/1.0/)[http://www.libpng.org/pub/png/spec/1.1/](http://www.libpng.org/pub/png/spec/1.1/)[http://www.libpng.org/pub/png/spec/1.2/](http://www.libpng.org/pub/png/spec/1.2/)[http://www.w3.org/TR/2003/REC-PNG-20031110/#minitoc](http://www.w3.org/TR/2003/REC-PNG-20031110/#minitoc)[http://w3c.github.io/PNG-spec/](http://w3c.github.io/PNG-spec/)
| (gzip) PNG v1.0
 (html/txt) PNG v1.1
 (html/txt) PNG v1.2
 PNG "second edition"
 (corrupted html) PNG "third edition" |
| --- |

To figure out "how to decode the damn bitmap", there is this valuable resource:

```
[http://pyokagan.name/blog/2019-10-14-png/](http://pyokagan.name/blog/2019-10-14-png/)
| Writing a (simple) PNG decoder |
| --- |

For testing different color depths and interlacing:

```
[http://www.schaik.com/pngsuite/](http://www.schaik.com/pngsuite/)
| PNG test images (color types, interlace, etc) |
| --- |

| DS Encrypted Arika Archives with ALZ1 compression |
| --- |

Used in Arika's DSi Dr. Mario and reportedly also in other Arika titles.

```

| rom:\INFO.DAT - contains encrypted directory
 rom:\GAME.DAT - contains ALZ1 compressed files, and some uncompressed files |
| --- |

**Arika INFO.DAT header**

```

| 000h 10h Title (eg. "*Dr.Mario-DSi!!!" used as decryption key)
 010h 14h Zerofilled ;\
 024h 4 Sector size (100h) (if zero: default to 800h) ;
 028h 4 Unknown, maybe version (1) ; encrypted
 02Ch 4 Number of used Directory entries (eg. 288h) ;
 030h N*30h Directory entries (see below) ;/
 ... .. Ununsed Directory entries (40h-filled) ;-unencrypted |
| --- |

Arika Directory Entry format:

```

| 000h 20h Filename (ASCII, max 31 chars, plus zeropadding)
 020h 4 File Size in bytes
 024h 4 File Offset in sector units (in GAME.DAT file)
 028h 4 File Size in sector units
 02Ch 4 Decompressed File Size in bytes (same as [20h] when uncompressed) |
| --- |

The file names must contain lowercase "a..z" in first character (the software
is generating a not-so-fast "quick lookup table" based on the 1st char, and
produces memory corruption if it's outside of the "a..z" range).

Arika archives contain only files, not folders. However, the filenames do
usually contain slashes (eg. "com/chr/e_eff_vy01.dat"), so software could split
those names into folder+filename, if desired.

**Arika INFO.DAT decryption**

```

| if buf[0]=00h then exit ;unencrypted
 for i=10h to filesize-1, buf[i]=((buf[i] ror 4) xor FFh)-buf[i AND 0Fh] |
| --- |

The strength of the 10h-byte key pattern is that people will have a hard time
to realize that the file is encrypted at all; otherwise they could easily
decrypt it by just gazing at it in a hex editor.

**Arika ALZ1 compression**

ALZ1 is almost identical to LZss, with some useless changes:

The 4-byte fileheader contains "ALZ1", without any info about the decompressed
size (which must be taken from INFO.DAT instead).

The compression flags are inverted (0=compressed), and processed backwards (LSB
first).

The dictionary is stored in separate 1000h-byte ringbuffer (which is initially
zerofilled, so the files can directly start with compressed 00h bytes).

The 16bit compression codes are byte-swapped (1st byte is dict.lsb, 2nd byte is
dict.msb*10h+(len-3)), with dict being the dictionary's read index.

The dictionary's initial write index is FEEh, all compressed/uncompressed data
written to dest is also written to the dictionary.

```

| for i=0 to FFFh, ringbuf[i]=00h, next i ;required for DSiware titles
 p=0FEEh ;initial ringbuf write pointer
 @@collect_more:
 flagbits=[src]+100h, src=src+1 ;8bit flags
 @@decompress_lop:
 if dst>=dst.end then goto @@decompress_done
 flagbits=flagbits SHR 1
 if zero then goto @@collect_more
 if carry=1 then
 dta=[src], [dst]=dta, ringbuf[p AND FFFh]=dta, dst=dst+1, p=p+1, src=src+1
 else
 q=[src]+((src+1]/10h)*100h), len=([src+1] AND 0Fh)+3, src=src+2
 for i=1 to len ;read from ringbuf[q] (instead of from relative [dst-q])
 dta=ringbuf[q AND FFFh], [dst]=dta, ringbuf[p AND FFFh]=dta
 dst=dst+1, p=p+1, q=q+1
 next i
 endif
 goto @@decompress_lop
 @@decompress_done:
 ret |
| --- |

ALZ1 compression is also used in PSX Power Spike (in BIZ archives inside of the
main IDX/HUG archive). There are two small differences: The DSi version does
have the compressed data prefixed by ID "ALZ1" (the PSX version doesn't have
that prefix). The DSi version requires the ringbuffer to be initially
zerofilled (the PSX version uses an uncompressed 00h byte before using
compressed zeroes).

**Arika ALZ1+LZrev double compression**

The file "com/m2dres_narc.blz" is double compressed (LZrev inside of ALZ1,
which can actually reduce the size because LZrev or ALZ1 alone are having
rather poor compression ratios) (whereas, LZrev can be only detected via the
".blz" extension, so make sure not to forget the extension after handling the
first compression layer).

Other files like "lng/eur/fr/man/man_narc.blz" are only LZrev compressed.

**Note**

ALZ1 seems to be identical to LArc's lz5 compression method (except that ALZ1
does initially zerofill the ringbuf; whilst lz5 is using different initial byte
patterns).

| DS Files - Vicarious Vision GFC/GOB Archives with ZLIB compression |
| --- |

NDS Over the Hedge, rom:\vvobj\generated\gob\main.gfc and main.gob

NDS Tony Hawks Skateland, rom:\vvobj\generated\gob\main.gfc and main.gob

NDS Tony Hawks Downhill Jam, rom:\vvobj\generated\gob\main.gfc and main.gob

NDS Ultimate Spiderman, rom:\vvobj2\generated\gob\main.gfc and main.gob

```

| .GFC - directory
 .GOB - zlib compressed data cluster |
| --- |

**GFC File (BIG ENDIAN)**

```

| 000h 4 File ID (always 8008h)
 004h 4 Size of the .GOB data file (eg. 01735224h)
 008h 4 Number of Clusters (C) (eg. 2468h)
 00Ch 4 Number of Files (F) (eg. 1AF4h, 6900 decimal)
 010h C*10h Cluster Table
 24690h C*4 Cluster Checksums (ADLER32 checksums on compressed clusters)
 2D830h F*0Ch Basic File Table
 41BA0h (F*68h) Extended File Table ;-NOT included in retail version! |
| --- |

**Cluster Table Entries (FAT-like allocation table)**

```

| 000h 4 Compressed size in bytes
 004h 4 Offset in .GOB data file (usually PrevOffs+PrevSize+4byteAlign)
 008h 4 Next Cluster (0..NumClusters-1, usually Curr+1, or 7FFFh=None)
 00Ch (1) Compression (30h="0"=None, 7Ah="z"=zlib) ;\exists in NDS games, 00Dh (3) Zerofilled ;/NOT in other games? |
| --- |

Larger files can consist of several compressed and/or uncompressed clusters,
that allows random-access to compressed files (without needing to decompress
the preceeding clusters; provided that the decompressor knows maximum size of
the the uncompressed cluster; that is 2000h on NDS?).

Small files (and last cluster of larger files) may be left uncompressed (if
they are considered to be small enough without compression).

**Cluster Checksums**

The Cluster Checksums (in .GFC) contain ADLER32 checksums on the compressed
data. The zlib compressed data (.in GOB) additionally includes a ADLER32 on the
uncompressed data.

Caution: ADLER32 is usally computed with InitialValue=00000001h (aka 2x16bit
0001h,0000h). However, the .GFC and .GOB are using InitialValue=00000000h, this
is incompatible with unmodified zlib functions.

**Basic File Table (sorted by filename CRC32s)**

Files are accessed via filename CRCs (that can be faster than string compare,
but fails if two filenames have the same CRCs; the dev tool has error checking
for that, and the developer must then rename one of the files).

```

| 000h 4 CRC32 on lowercase filename, excluding ending zero
 004h 4 Filesize (of uncompressed file)
 008h 4 First Cluster Number in Cluster Table (0..NumClusters-1) |
| --- |

Examples names found in the NDS Over the Hedge executable:

```

| ".\strm\LA_BG.hwas" --> CRC32=522AFF5Dh (with name converted to lowercase)
 ".\strm\boy_tv2.hwas" --> CRC32=F047AA4Eh (with name already being lowercase) |
| --- |

**Extended File Table (if any, not included in NDS retail titles)**

This contains a list of all filenames, but does apparently exist in dev version
only. The retail version does have filenames in the executable (but they may be
scattered around at various locations, or even generated on the fly, eg.
path+filename+index+ext).

```

| 000h 60h Filename in lowercase ASCII (eg. ".\path\name.ext",00h)
 060h 4 CRC32 on uncompressed file data
 064h 4 Timestamp |
| --- |

**Compressed .GOB file**

Compression=NONE stores uncompressed clusters, except: if the remaining file
size (in last cluster of the file) is smaller than ClusterSize then it stores
only that many bytes in the .GOB file.

Compression=ZLIB stores 2-byte ZLIB header, deflated data, and 4-byte checksum,
except: the initial checksum value is different as in standard zlib functions
(see above notes on ADLER32).

**Variants**

The GFC/GOP format is used in different games on different consoles. Some
hardcoded parameters may differ (and, unfortunately, the file header doesn't
contain that info):

```

| - The Extended File Table (with filenames) doesn't exist in all GFC files.
 - Use "if Next>=NumClusters then End" (instead hardcoded "if Next=7FFFh").
 - Offsets in GOP may be matched to DMA alignment or CDROM sector (sector
 align might help on 1st cluster of file, without hurting continous loading).
 - ClusterSize is important only for random access (if needed, one could detect
 the biggest (uncompressed) ClusterSize and use that as filepos divider).
 - Source code seems to use 0Ch-byte Cluster Entries (without compression flag,
 it seems to have compression "codec" type byte at begin compressed data?).
 - FilenameSize=60h might vary (if the Extended File Table is present at all). |
| --- |

**References**

[https://github.com/jedis/jediacademy/tree/master/code/goblib](https://github.com/jedis/jediacademy/tree/master/code/goblib)
 - source code

| DSi SD/MMC Firmware Log Files |
| --- |

**FAT16:\sys\log\shop.log (32 bytes)**

```

| 0000h 20h Zerofilled |
| --- |

Unknown if this file can contain anything else.

**FAT16:\sys\log\product.log (573 bytes)**

Contains some ASCII text with version, date (YY/MM/DD), and time (HH:SS) info
(using 0Ah as CRLF):

```

| 0,BOARD,START,1.5,09/01/14,14:52,000055, ,
 0,BOARD,OK,1.5,09/01/14,14:53,000055, ,
 0,TP_CAL,OK,2.0, , , ,(647 811)-(3478 3245),
 0,AGING,OK,1.0, , , ,Time=60:20(m:s)
 Count=32,
 0,FINAL,START,1.5,09/01/15,09:52,000084,TWL Ver.2.0,
 0,FINAL,OK,1.5,09/01/15,09:52,000084,TWL Ver.2.0,
 0,MIC,OK,2.1, , , ,All Test Passed,
 0,CAMERA,OK,2.1, , , , ,
 0,WRFU,START,0.60,09/01/15,10:03,000143,P000063 G000143 717cfde74f5ef6763473,
 0,WRFU,OK,0.60,09/01/15,10:04,000143,PCVer:1.7f
 R-53 -55 E0.00 0.00,
 0,IMPORT,START,1.0, , , , ,
 0,IMPORT,OK,1.0, , , ,Region=EUR,
 0,NCHECK,OK,1.0, , , , , |
| --- |

**FAT16:\sys\log\sysmenu.log (16Kbytes)**

Contains several groups of three text lines. Each "#FFT" group begins with two
0Ah characters, and is followed by space padding for 256-byte alignment of the
next group).

Below is some example (with blank space removed, original 40-digit hash strings
abbreviated to "xxxx..xx", and some lines are replaced by "...").

```

| #FFT 13-08-18[SUN] 12:37:10
 title: HNAP
 DHT_PAHSE1_FAILED (sub info): hash1 - 8dfc..59
 #FFT 13-08-18[SUN] 12:37:10
 title: HNAP
 DHT_PAHSE1_FAILED (sub info): calc_hash - 7eca..f5
 #FFT 13-08-18[SUN] 12:37:11
 title: HNAP
 menuRedIplManager.cpp [l.514] RED FATAL 0000000010000000 (0000000041575445)
 #FFT 13-08-18[SUN] 12:37:11
 title: HNAP
 menuResetCallback.cpp [l.50] type 0
 #FFT 13-08-18[SUN] 13:44:16
 title: HNAP
 DHT_PAHSE1_FAILED (sub info): hash1Addr-02799e38
 #FFT 13-08-18[SUN] 13:44:16
 title: HNAP
 DHT_PAHSE1_FAILED (sub info): hash1 - 8dfc..59
 ...
 ...
 #FFT 13-09-10[TUE] 22:07:39
 title: HNAP
 menuResetCallback.cpp [l.50] type 0
 #FFT 13-09-14[SAT] 14:59:16
 title: HNAP
 SYSMi_LoadTitleThreadFunc: some error has occurred.
 #FFT 13-09-14[SAT] 14:59:16
 title: HNAP
 SYSMi_AuthenticateTitleThreadFunc: loaded 1 times.
 #FFT 13-09-14[SAT] 14:59:17
 title: HNAP
 menuRedIplManager.cpp [l.514] RED FATAL 0000800000002100 (0003000049524544)
 #FFT 13-09-14[SAT] 14:59:17
 title: HNAP
 menuResetCallback.cpp [l.50] type 0
 #FFT 00-01-03[MON] 20:50:18
 title: HNAP
 WHITELIST_NOTFOUND (sub info): no entry for phase 1/2.
 #FFT 00-01-03[MON] 20:50:18
 title: HNAP
 WHITELIST_NOTFOUND (sub info): no entry for phase 3.
 #FFT 00-01-03[MON] 20:50:18
 title: HNAP
 SYSMi_LoadTitleThreadFunc: some error has occurred.
 #FFT 00-01-03[MON] 20:50:18
 title: HNAP
 SYSMi_AuthenticateTitleThreadFunc: loaded 1 times.
 #FFT 00-01-03[MON] 20:50:19
 title: HNAP
 menuRedIplManager.cpp [l.514] RED FATAL 0000800008000100 (000000004143454b)
 #FFT 00-01-03[MON] 20:50:19
 title: HNAP
 menuResetCallback.cpp [l.50] type 0
 #FFT 00-01-05[WED] 01:03:16
 title: HNAP
 WHITELIST_NOTFOUND (sub info): no entry for phase 1/2.
 ...
 ...
 #FFT 00-01-01[SAT] 00:02:37
 title: HNAP
 SYSMi_AuthenticateTitleThreadFunc: loaded 1 times.
 #FFT 00-01-01[SAT] 00:02:38
 title: HNAP
 menuRedIplManager.cpp [l.514] RED FATAL 0002004000000100 (00000000414e5045)
 #FFT 00-01-01[SAT] 00:02:38
 title: HNAP
 menuResetCallback.cpp [l.50] type 0 |
| --- |

| DSi SD/MMC Firmware Misc Files |
| --- |

**FAT16:\sys\HWINFO_S.dat (aka Serial/Barcode) (16Kbytes)**

```

| 0000h 80h RSA-SHA1-HMAC across entries [0088h..00A3h]
 (with RSA key from Bootsectors, and also from Launcher)
 (with SHA1-HMAC key = SHA1([4004D00h..4004D07h], aka Console ID)
 0080h 4 Header, Version or so (00000001h)
 0084h 4 Header, Size of entries at [0088h..00A3h] (0000001Ch)
 0088h 4 Bitmask for Supported Languages (3Eh for Europe) (as wifi_flash)
 008Ch 4 Unknown (00,00,00,00) (bit0=flag for 4004020h.bit0=wifi ?)
 0090h 1 Console Region (0=JPN, 1=USA, 2=EUR, 3=AUS, 4=CHN, 5=KOR)
 0091h 12 Serial/Barcode (ASCII, 11-12 characters; see console sticker)
 009Dh 3 Unknown (00,00,3C) ;"<"
 00A0h 4 Title ID LSBs for Launcher ("PANH", aka HNAP spelled backwards)
 00A4h 3F5Ch Unused (FFh-filled) |
| --- |

Entries [0088h..009Fh] are copied to [2FFFD68h..2FFFD7Fh]. Entry [00A0h] is
used to construct the region-specific filename of the Launcher (System Menu).

The RSA with Console ID means that one cannot change the region/language stuff.

The 3DS has similar data stored in file SecureInfo_A on 3DS partition.

**FAT16:\sys\HWINFO_N.dat (16Kbytes)**

```

| 0000h 14h SHA1 on entries [088h..09Bh]
 0014h 6Ch Zerofilled
 0080h 4 Header, Version or so (00000001h)
 0084h 4 Header, Size of entries at [0088h..009Bh] (00000014h)
 0088h 4 Some per-console ID (used what for?)
 008Ch 10h Some per-console ID (used in "Tad Files")
 009Ch 3F64h Unused (FFh-filled) |
| --- |

Entries [0088h..009Bh] are copied to [2000600h..2000613h].

**FAT16:\sys\HWID.sgn (256 bytes)**

```

| 0000h 100h RSA-OpenPGP-SHA1 across... whatever? |
| --- |

Seems to be used only by DSi Shop. The RSA keys are unknown for retail version.
Also unknown WHAT the SHA1 is computed on (probably some console and/or region
IDs).

The System Updater tool (for debug version) contains both public and private
RSA keys for the file; the keys don't work for retail version though.

The OpenPGP bytes are same as for SWI 23h (but with more FFh padding bytes due
to the 100h-byte RSA size).

**FAT16:\shared2\0000 (2048K) (sound recorder)**

Huge 2Mbyte file with several used areas (and many zerofilled areas).

Contains a FAT12 filesystem with several voice .dat files for the Sound
Recorder of the Nintendo DSi Sound utility. Unused clusters seem to contain
garbage (maybe un-encrypted eMMC sectors).

```

| voice18111008215651000010001.dat ;14402h bytes
 voice20131018211242000010001.dat ;14402h bytes
 voice19111008215708000010001.dat ;14402h bytes
 voice00131018211411003110001.dat ;14402h bytes
 voice01150418144405002110001.dat ;14402h bytes
 voiceNNYYMMDDHHMMSS00NN10001.dat ;14402h bytes |
| --- |

Note: The DSi Sound utility is additionally having a 512Kbyte private.sav file
(also containing a FAT12 filesystem; although it seems to contain only a MBR,
FATs, and an empty Root directory - plus garbage in unused clusters).

**FAT16:\import\**

**FAT16:\progress\**

**FAT16:\tmp\es\write\**

Empty folders.

| DSi SD/MMC Firmware Wifi Firmware |
| --- |

**FAT16:\title\0003000f\484e4341\content\000000vv.app (aka Wifi Firmware)**

WLFIRM aka WLANFIRM (compressed, non-executable datafile, for all regions,
v0:13B80h bytes, v1:13BA0h bytes, v2:17E60h bytes). This file contains Wifi
Firmwares for the Xtensa CPU in the Atheros AR60xxG chips on the DWM-W0xx Wifi
Daughterboards.

```

| DSi Firmware 1.0 --> Wifi Firmware v0 (supports AR6002)
 DSi Firmware 1.1 thru 1.2 --> Unknown (presumably v0 or v1)
 DSi Firmware 1.3 --> Wifi Firmware v1 (supports AR6002)
 DSi Firmware 1.4 thru 1.4.5 --> Wifi Firmware v2 (supports AR6002+AR6013)
 Note: The AR6002 part is exact same in v1 and v2 (with same SHA1 in Part 1)
 However, part 1.c was slightly smaller in v0, apparently some small bugfix. |
| --- |

The BIOS ROM in the AR60xxG chips can be extended/upgraded by uploading the
firmware into RAM (the RAM isn't nonvolatile, so this must be done each time
after power-up, that is done by the System Menu (launcher), so DSi games won't
need to upload the firmware themselves.

```

| 00000h 80h RSA-SHA1 (on [00080h..0009Fh]) (via RSA key from BIOS) ;\
 00080h 14h Header SHA1 (on [000A0h..000FFh]) ; SHA
 00094h 4 Header Size (00000060h, for entries 000A0h..000FFh) ;
 00098h 8 Zerofilled ;/
 000A0h 2 Version (0000h,0001h,0002h for v0,v1,v2) ;\
 000A2h 1 Number of parts (01h..02h) ;(02h in v2 only) ; Header
 000A3h 1 Unknown/zero? (00h) ;
 000A4h 4 Part 1 Start (00000100h) ;(in v1: E0h) ;\Part 1 ; with IDs
 000A8h 4 Part 1 Size (00013AC0h) ; DWM-W015; as in wifi
 000ACh 4 Part 1 ID (00000001h) (=DWM-W015) ; AR6002G ; flash[1FDh]
 000B0h 14h Part 1 SHA1 (on [00100h..13BBFh]) ;/ ;
 000C4h 4 Part 2 Start (00013BC0h) ;\Part 2 ; ;\
 000C8h 4 Part 2 Size (000042A0h) ; DWM-W024; ; not in
 000CCh 4 Part 2 ID (00000002h) (=DWM-W024) ; AR6013G ; ; version 1
 000D0h 14h Part 2 SHA1 (on [13BC0h..17E5Fh]) ;/ ; ;/
 000E4h 1Ch Zerofilled (padding to 20h-byte boundary) ;/
 00100h 1 Part 1 num subheader's (04h) (a/b/c/d) ;\
 00101h 1 Part 1 num ChipID's (02h) ;
 00102h 2 Part 1 offset to ChipID's (0044h) ;
 00104h 10h Part 1.a firm/main (00000080h,00013458h,80000001h,00502400h) ;
 00114h 10h Part 1.b database (000134E0h,000002BCh,00000002h,0052D944h) ;
 00124h 10h Part 1.c stub/code (000137A0h,000002DEh,00000004h,00515000h) ;
 00134h 10h Part 1.d stub/data (00013A80h,00000030h,00000005h,00502400h) ;
 00144h 8 Part 1 ChipID 1 ;alternate IDs ? (02010001h,20000188h) ;
 0014Ch 8 Part 1 ChipID 2 ;CHIP_ID, ROM_VERSION (02000001h,20000188h) ;
 00154h 4 Part 1 Firmware Version: 2.1.0.123 (2100007Bh) ;
 00158h 0Ch Part 1 RAM vars/base/size (00500400h,00500000h,0002E000h) ;
 00164h 1Ch Zerofilled ;
 00180h 13460h Part 1.a data (13458h compressed bytes, +8 bytes zeropadding);
 135E0h 2C0h Part 1.b data (2BCh bytes, +04h bytes zeropadding) ;database ;
 138A0h 2E0h Part 1.c data (2DEh bytes, +02h bytes zeropadding) ;stubcode ;
 13B80h 40h Part 1.d data (30h bytes, +10h bytes zeropadding) ;stubdata ;/
 13BC0h 1 Part 2 num subheader's (04h) ;\
 13BC1h 1 Part 2 num ChipID's (02h) ;
 13BC2h 2 Part 2 offset to ChipID's (0044h) ;
 13BC4h 10h Part 2.a firm/main (00000080h,00002EECh,80000001h,00524C00h) ;
 13BD4h 10h Part 2.b database (00002F80h,00000FC0h,00000002h,0053F040h) ;
 13BE4h 10h Part 2.c stub/code (00003F40h,00000312h,00000004h,00527000h) ;
 13BF4h 10h Part 2.d stub/data (00004260h,00000038h,00000005h,00524C00h) ;
 13C04h 8 Part 2 ChipID 1 ;CHIP_ID, ROM_VERSION (0D000000h,23000024h) ;
 13C0Ch 8 Part 2 ChipID 2 ;alternate IDs? (0D000001h,23000024h) ;
 13C14h 4 Part 2 Firmware Version: 2.3.0.108 (2300006Ch) ;
 13C18h 0Ch Part 2 RAM vars/base/size (00520000h,00520000h,00020000h) ;
 13C24h 1Ch Zerofilled ;
 13C40h 2F00h Part 2.a data (2EECh compressed bytes,+14h bytes zeropadding);
 16B40h FC0h Part 2.b data (FC0h bytes, +00h bytes zeropadding) ;
 17B00h 320h Part 2.c data (312h bytes, +0Eh bytes zeropadding) ;
 17E20h 40h Part 2.d data (38h bytes, +08h bytes zeropadding) ;/ |
| --- |

The a/b/c/d subheaders consist of File Source Offset (relative to Start of Part
1/2 accordingly), Length, ID/Flags, and RAM Destination Address.

The stub/code and stub/data parts are loaded and executed first (the stub is
reading calibration data from I2C bus EEPROM; this is decoupled from the main
firmware because hardware implementations could use different calibration
sources like I2C chips or SPI chips or mass-storage devices).

Thereafter, firm/main and database are loaded and executed. The "LZ" compressed
firm/main part is automatically decompressed on the Xtensa side, the "LZ" stuff
is some kind of "tag,len,disp" format:

```

| Part 1.a data: 9F,FF,FF,FF,FF,FF,FF,00,00,00,00,9F,04,04,..
 Part 2.a data: 5E,00,00,00,00,5E,04,04,5E,08,08,41,5F,49,.. |
| --- |

The first byte identifies the "tag" value (this should be the value used least
often in the uncompressed data). The following bytes are plain uncompressed
data, mixed with "tag,len,disp" values (which will copy "len" bytes from
"dest-disp" to "dest"). A special case is "tag,00h", which will store the "tag"
value at dest. The len and disp values can consist of one or more byte(s) each
(the LSB aka last byte is indicated by bit7=0; for example, "84h,86h,0Fh" would
mean 01030Fh). For some odd reason, the values are always "len<=disp" (even
for zerofilled regions where "len>disp" would be useful).

| DSi SD/MMC Firmware System Settings Data Files |
| --- |

The DSi stores System Settings (and Title ID of most recent System Menu index)
on eMMC in two identical files: TWLCFG0.dat and TWLCFG1.dat.

If both files are intact then the newer file is taken (as indicated by the
update counter; for some weird reason, the DSi System Menu is always updating
BOTH files, so they are usually both "newer").

For NDS compatibility, some of the data is additionally stored on Wifi FLASH:

DS Firmware User Settings

The TWL data and the NDS-style data are also copied to RAM:

```

| 2000400h 128h TWLCFGn.dat bytes [088h..1AFh]
 2FFFC80h 70h Wifi FLASH User Settings (fmw[newest_user_settings])
 2FFFDFCh 4 Pointer to 2000400h |
| --- |

For some reason, most DSi games are containing some initialization code for
repairing or initializing the above pointer, as so:

```

| if [2FFFDFCh]=0 then [2FFFDFCh]=2000400h |
| --- |

The RAM data at 2000400h isn't actually used by too many games though (one
program that is using it is Flipnote).

Some games are attempting to adopt the System Setting's language selection as
game language, games should do that only if they do support that language (a
bad example is the german version of Magic Made Fun: Deep Psyche, which
defaults to French when using English as system language).

**FAT16:\shared1\TWLCFG0.dat (16Kbytes) (System Settings Data)**

**FAT16:\shared1\TWLCFG1.dat (16Kbytes) (System Settings Data)**

```

| File RAM Siz Description
 000h - 14h SHA1 on entries [088h..1AFh]
 014h - 6Ch Zerofilled
 080h - 1 Version or so (01h)
 081h - 1 Update Counter (0..7Fh, wraps after 7bit) ;fmw_user[070h]
 082h - 2 Zero (0000h)
 084h - 4 Size of below RAM area (00000128h)
 088h 000h 4 Config Flags (usually 0000000Fh)
 bit0 Unknown (usually 1)
 bit1 Unknown (usually 1)
 bit2 LanguageSet? (usually 1)
 bit3 Wifi Enable (0=Disable, 1=Enable)
 bit4 Unknown (usually 0)
 bit5-23 Zero
 bit24 EULA Flag (0=No/CountryChanged, 1=Agreed)
 bit25-31 Zero
 08Ch 004h 1 Zero
 08Dh 005h 1 Country code, same as Wii country codes (eg. 46h=Bulgaria)
 08Eh 006h 1 Selected Language (eg. 1=English) ;fmw_user[064h,075h]
 08Fh 007h 1 RTC Year (last date change) (max 63h=2099) ;fmw_user[066h]
 090h 008h 4 RTC Offset (difference in seconds on change) ;fmw_user[068h]
 094h 00Ch 4 Zerofilled (or FFh-filled) (=MSBs of above?) 06Ch?
 098h 010h 1 EULA Version (0=None/CountryChanged, 1=v1) ;cart[20Eh]
 099h 011h 9 Zerofilled
 0A2h 01Ah 1 Alarm Hour (0..17h) ;fmw_user[052h]
 0A3h 01Bh 1 Alarm Minute (0..3Bh) ;fmw_user[053h]
 0A4h 01Ch 2 Zerofilled
 0A6h 01Eh 1 Alarm Enable (0=Off, 1=On) ;fmw_user[056h]
 0A7h 01Fh 1 Zerofilled
 0A8h 020h 1 Zerofilled ;\
 0A9h 021h 1 System Menu number of used Title Slots (0..39) ; zerofilled
 0AAh 022h 1 System Menu number of free Title Slots (39..0) ; when copied
 0ABh 023h 1 Unknown (0) ; to RAM
 0ACh 024h 1 Unknown (02h or 03h) ;
 0ADh 025h 3 Zerofilled ;
 0B0h 028h 8 System Menu most recently selected Title ID ;/ ;cart[230h]
 0B8h 030h 2x2 TSC calib (adc.x1,y1) 12bit ADC-position ;fmw_user[058h]
 0BCh 034h 2x1 TSC calib (scr.x1,y1) 8bit pixel-position ;fmw_user[05Ch]
 0BEh 036h 2x2 TSC calib (adc.x2,y2) 12bit ADC-position ;fmw_user[05Eh]
 0C2h 03Ah 2x1 TSC calib (scr.x2,y2) 8bit pixel-position ;fmw_user[062h]
 0C4h 03Ch 4 Unknown (9C 20 01 02)
 0C8h 040h 4 Zerofilled
 0CCh 044h 1 Favorite color (also Sysmenu Cursor Color) ;fmw_user[002h]
 0CDh 045h 1 Zero
 0CEh 046h 2 Birthday (month, day) ;fmw_user[003h..004h]
 0D0h 048h 14h+2 Nickname (UCS-2), max 10 chars+EOL ;fmw_user[006h..019h]
 0E6h 05Eh 34h+2 Message (UCS-2), max 26 chars+EOL ;fmw_user[01Ch..04Fh]
 11Ch 094h 1 Parental Controls Flags (bit0=Parental, bit1-6=Pictochat,etc)
 11Dh 095h 6 Zero
 123h 09Bh 1 Parental Controls Region (0=Off, 3=German/USK, 4=French?)
 124h 09Ch 1 Parental Controls Years of Age Rating (00h..14h) ;cart[2F0h]
 125h 09Dh 1 Parental Controls Secret Question (00h..05h)
 126h 09Eh 1 Parental Controls Unknown (can be 00h, 06h, or 07h)
 127h 09Fh 2 Zero
 129h 0A1h 4+1 Parental Controls PIN (ASCII digits) 4 digits+EOL
 12Eh 0A6h 80h+2 Parental Controls Secret Answer (UCS-2), max 64 chars+EOL
 1B0h - 3E50h Unused (FFh-filled) |
| --- |

Additionally, there's some stuff in RAM (maybe current Wifi Firmware version):

```

| - 128h B8h Unused
 - 1E0h 1 WlFirm Type (1=DWM-W015, 2=W024, 3=W028) ;wifi_flash[1FDh]
 - 1E1h 1 WlFirm Unknown (zero)
 - 1E2h 2 WlFirm CRC16 with initial value FFFFh on [1E4h..1EFh]
 - 1E4h 4 WlFirm RAM vars (500400h) ;\
 - 1E8h 4 WlFirm RAM base (500000h) ; as from "Wifi Firmware" file
 - 1ECh 4 WlFirm RAM size (02E000h) ;/
 - 1F0h 10h WlFirm Unknown (zero)
 - 200h 14h Hexvalues from HWINFO_N.dat
 - 214h 0Ch Unused/padding? (zero) |
| --- |

Parental controls fields are all zero when not in use.

The Wifi Enable flag is... a surreal fake Wireless-Disable option; which does
only disable the Wifi LED, the actual Wifi transmission does still work.

| DSi SD/MMC Firmware Version Data File |
| --- |

verdata (00030005-HNLx) is a bundle of data which corresponds to a release of
the "System Menu" -- every time Nintendo announces a new version of the system
menu, they will update one or more other titles and then update this title. The
verdata filesize is constant for all versions/regions (1B50h bytes). Existing
verdata downloads on Nintendo's server are:

```

| 00000001..00000009 jpn (9 versions)
 00000003..00000009 usa/eur/aus (7 versions)
 00000001..00000006 chn (6 versions)
 00000002..00000006 kor (5 versions) |
| --- |

Apart from those downloads/updates, each region did probably originally have an
older version pre-installed. The japanese 1.0J firmware did oddly have
00000000.app files for BOTH japan and usa?/aus? installed (whereof the latter
contained 0.1A instead of 1.0A as version string).

**FAT16:\title\0003000f\484e4cgg\content\000000vv.app (aka Version Data)**

```

| 0000h 80h RSA-SHA1 on entries [0080h..end of file]
 0080h ... NARC (Nitro Archive) ... |
| --- |

The NARC is a nintendo-specific virtual filesystem. For details, see:

DS Cartridge NitroROM and NitroARC File Systems

The NARC archive contains the following files:

```
[https://cfh.t.app.nintendowifi.net/eula/](https://cfh.t.app.nintendowifi.net/eula/)
| .twl-nup-cert.der - server cert for software update server
 .twl-nup-prvkey.der - client-side private key for software update server
 .twl-shop-cert.der - server cert for Shopping Channel server
 .twl-shop-prvkey.der - client-side private key for Shopping Channel server
 NintendoCA-G2.der - Certificate Authority cert, used to sign other certs
 eula_url.bin - URL to the EULA text for this system update,
 generally 
 nup_host.bin - server to query for the next system update,
 generally nus.t.shop.nintendowifi.net:443
 time_stamp.bin - build date for this version, eg. 00281108 (28 Nov 2008)
 user_area_size.bin - eg. 08000000h (signed) (=128Mbyte? aka 1024 "blocks"?)
 version.bin - firmware version (and version/region string) |
| --- |

The NARC is usually mounted as "verdata:" device (eg. "verdata:/version.bin").

The four ".twl-*.der" files are encrypted with ES Block Encryption (using a
fixed key):

```

| KEY[00h..0Fh] = Constant (08,2F,61,38,...) ;from ARM7BIOS |
| --- |

DSi ES Block Encryption

For the decrypted .der files (and unecrypted NintendoCA-G2.der file), see

DSi SD/MMC Firmware Certificate/Keys (DER)

The version.bin file contains the firmare version:

```

| 000h 2 Major Version (0001h) (eg. 1)
 002h 2 Minor Version (0000h..0004h) (eg. 4)
 004h 1Ch Version/Region string in UTF-16, zeropadded (eg. "1.4.5E") |
| --- |

The Version/Region string is displayed in System Settings. The region suffix is
"J"=JAP, "U"=USA, "E"=EUR, "A"=AUS, "C"=CHN, "K"=KOR, or "ALL"=3DS/New3DS.

**DSi Firmware Versions**

```

| 0.1 31 Jul 2008 Pre-release v0.1A (accidentally included in v1.0J)
 1.0 09 Sep 2008 Pre-installed v1.0J version (the actual file in v1.0J)
 1.0? 22 Oct 2008? First Update(??) to Japanese Region DSi System Menu
 1.1? ? dsibrew:NoneSuch?, wikipedia:Preinstalled1stJpnVersion??
 1.2 18 Dec 2008 Second Update to Japanese Region DSi System Menu
 1.3 03 Apr 2009 Launch Day (USA, EUR, AUS), new "start DSi Camera" button
 1.4 29 Jul 2009 Blocks NDS flashcarts, Facebook support to share photos
 1.4.1 07 Sep 2010 Blocks more NDS flashcarts
 1.4.2 10 May 2011 Blocks DSiWare exploits on SD card (sudokuhax etc.)
 1.4.3 29 Jun 2011 Blocks more NDS flashcarts (only whitelist was updated)
 1.4.4 21 Mar 2012 Blocks DSi cart exploits (CookingCoach/ClassicWordGames)
 1.4.5 11 Dec 2012 Blocks more NDS flashcards |
| --- |

JAP region launched first (unknown if there was any pre-installed version prior
to the v1.0 update (or if v1.0 was really released as "update" at all), unknown
if v1.1 did also exist).

USA/EUR/AUS regions launched on 03 Apr 2009 (so only v1.3 and up exist as
update?; but they had an older version pre-installed: v1.2U is known to exist).

CHN region launched on 11 Sep 2010 with firmware v1.4.2C (or more probably with
v1.4.1C pre-installed?).

KOR region launched at unknown date (probably near chinese launch date) (korean
v1.4.1K is known to exist).

CHN and KOR do have version numbers one step higher than normal regions (ie.
v1.4 through v1.4.5 are called v1.4.1 through v1.4.6 in china/korea).

3DS and New3DS identify themselves as DSi version v1.4.2ALL.

| DSi SD/MMC Firmware Nintendo DS Cart Whitelist File |
| --- |

**FAT16:\title\0003000f\484e4841\content\00000001.app (aka NDS Cart Whitelist)**

The NDS Cart Whitelist contains checksums of all officially released licensed
NDS cartridges (newer NDS Carts that aren't included in the list must contain
extended NDS Cart Headers with RSA signatures).

That means, unlike the original NDS, the DSi refuses to boot any
unlicensed/homebrew NDS software (though some "DSi compatible" FLASH carts are
bypassing that restriction via exploits in licensed NDS games).

Below Whitelist example is from firmware v1.4E:

```

| Part 1 ("NDHT") is same in v1.0 through v1.4.5:
 00000h 4 ID "NDHT" ;\
 00004h 80h RSA-SHA1 on [00084h..286A7h] ;
 00084h 4 Number of titles (00000D76h) (=3446) ;
 00088h D76h*30h Titles (30h bytes each, with two SHA1s) ;/
 Part 2 ("NDHX") is same in v1.4 through v1.4.5 (doesn't exist in v1.3):
 286A8h 4 ID "NDHX" ;\
 286ACh 80h RSA-SHA1 on [2872Ch..4AFBFh] ;
 2872Ch 4 Number of titles (000013BCh) (=5052) ;
 28730h 13BCh*1Ch Titles (1Ch bytes each, only one SHA1) ;/
 Part 3 ("NDHI") differs in v1.4 versus v1.4.5 (doesn't exist in v1.3):
 4AFC0h 4 ID "NDHI" ;\
 4AFC4h 80h RSA-SHA1 on [4B044h..4B1B7h] ;
 4B044h 4 Number of titles (04h in v1.4E) ;60h in v1.4.5E ;
 4B048h 4*5Ch Specials for A3TE,A6WE,YF7E,YOUF ;210h on New3DS ;/
 Footer:
 4B1B8h 13 Version String ("2832",0Dh,0Ah,"10619",0Dh,0Ah in v1.4E) ;\
 4B1C5h 11 Random garbage (padding to 10h-byte boundary) ;/ |
| --- |

The Version String at the end can be:

```

| 00000000.app v1.0J (at 286A8h) "2435",0Ah,"8325",0Ah ;with LF's
 00000000.app v1.3U (at 286A8h) "2435",0Ah,"8325",0Ah ;with LF's
 00000001.app v1.4E (at 4B1B8h) "2832",0Dh,0Ah,"10619",0Dh,0Ah ;with CRLF's
 0000000x.app v... (?)
 00000006.app v1.4.5E (at 4D2C8h) "3067",0Ah,"11437",0Ah ;with LF's
 0000000x.app v... (?)
 0000000e.app New3DS (at 56E08h) "3106",0Ah,"11437",0Ah ;with LF's |
| --- |

BUG: The Whitelist's RSA signatures are NOT checked in firmware v1.4E (whilst
other older/newer firmwares like 1.3U, 1.4.2E, and 1.4.5E are checking those
signatures).

NDHT Title Structure (30h bytes each):

```

| This contains all NDS titles released prior to DSi firmware v1.0.
 Start Length Description
 000h 4 Title ID (Gamecode)
 004h 4 Title version
 008h 20 Phase 1 SHA1-HMAC on 160h-byte cartheader and ARM9+ARM7 areas (?)
 01Ch 20 Phase 2 SHA1-HMAC on ARM9 Overlay and NitroFAT (zero if no overlay) |
| --- |

NDHX Title Structure (1Ch bytes each):

```

| This contains all NDS titles released prior to DSi firmware v1.4.
 000h 4 Title ID (Gamecode)
 004h 4 Title version
 008h 20 Phase 3 SHA1-HMAC on Icon/Title |
| --- |

NDHI Title Structure (5Ch bytes each):

```

| This contains extra checks for detecting hacked/exploited NDS titles.
 000h 4 Title ID (Gamecode)
 004h 4 Title version
 008h 8*8 Offset+Length for up to 8 regions (or 0,0=None)
 048h 20 Phase 4 SHA1-HMAC on above region(s) |
| --- |

The 40h-byte SHA1-HMAC keys are contained in Launcher (61h,BDh,DDh,72h,... for
Phase 1+2, and 85h,29h,48h,F3h,... for Phase 3+4). The RSA key is also
contained in Launcher (C7h,F4h,1Dh,27h,... for all Phases; though the RSA key
is missing in firmwares where Nintendo forgot to implement the RSA check, eg.
in v1.4E).

Example values for Metroid Demo ("AMFE"):

```

| 41 4D 46 45 00 00 00 00 ;\
 95 9A B3 09 B7 4E AF 29 2E 97 61 B9 DC E9 5F FE 86 5C 91 4E ; NDHT
 D3 94 43 02 64 3A AF C5 D1 E1 3B C0 47 4A A2 98 AB 5D 71 8F ;/
 41 4D 46 45 00 00 00 00 ;\NDHX
 51 24 FE EF D4 3C 22 42 CC 17 13 0A 72 F8 FA 3B 4D 83 2A B1 ;/ |
| --- |

Specials related to games:

```

| NTR-A3TE-USA = Tak: The Great Juju Challenge
 NTR-A6WE-USA = FIFA World Cup 2006
 NTR-YF7E-USA = Fish Tycoon
 NTR-YOUF-FRA = Samantha Oups! |
| --- |

**Newer NDS Carts with RSA**

NDS games released after DSi firmware v1.0 have RSA headers without Icon SHA1

NDS games released after DSi firmware v1.4 have RSA headers with Icon SHA1

Accordingly, NDS games released between DSi firmware v1.0 and v1.4 do have
whitelist NDHX entries (for the icon), but don't need NDHT entries (since
that's already covered by the RSA header).

Related carthdr flags are:

```

| cart[1BFh].bit6 = Cart Header RSA Signature exists
 cart[1BFh].bit5 = Cart Header has Icon SHA1 at [33Ch]
 cart[378h] = SHA1 (same as whitelist Phase 1)
 cart[38Ch] = SHA1 (same as whitelist Phase 2)
 cart[33Ch] = SHA1 (same as whitelist Phase 3) (if above bit5=1) |
| --- |

| DSi SD/MMC Camera Files - Overview |
| --- |

**Photos/Frames**

Photos can be taken via Nintendo DSi Camera utitilty (or alternately, directly
via hotkeys in System Menu; which will be automatically flagging the photos
with "Star" stickers, which will cause them to be shown as System Menu
background image).

Frames are masks (with transparent pixels) that can be put onto photos. The
Frames can be created via Nintendo DSi Camera utility (Camera, select Frame
(upper-right Lens option), accept that Lens, then click Create Frame; the
procedure then is to take a photo, and to rub-out pixels on touchscreen to make
them transparent).

**Internal/External Storage**

The Camera is storing further data on the eMMC FAT12 partition:

```

| FAT12:\photo\DCIM\100NIN02\HNI_nnnn.JPG ;camera photos
 FAT12:\photo\private\ds\app\484E494A\pit.bin ;camera info
 FAT12:\photo\private\ds\app\484E494A\DCIM\100NIN02\HNI_nnnn.JPG;camera frames |
| --- |

Camera data can be copied to SD card (via Nintendo DSi Camera, Options, Copy):

```

| SD:\DCIM\nnnNIN02\HNI_nnnn.JPG ;camera photos
 SD:\private\ds\app\484E494A\pit.bin ;camera info
 SD:\private\ds\app\484E494A\DCIM\nnnNIN02\HNI_nnnn.JPG ;camera frames |
| --- |

And, in internal eMMC only (not on SD), the DSi is somewhere storing Calendar
entries (some sort of bitmaps with optional handwritten comments drawn via
touchscreen).

**File/Folder Numbers**

The "nnnNIN02" folders are numbered "100NIN02" through "999NIN02". The first
folder is usually 100NIN02, unless another "100xxxxx" folder did already exist
(eg. if the SD card contains a "100CANON" folder, then DSi would start at
101NIN02 or higher).

The trailing "02" of the "nnnNIN02" folders appears to be fixed for DSi photos
(folder name "nnnNIN01" is reserved for Wii screenshots).

The "HNI_nnnn.JPG" filenames are numbered "HNI_0001.JPG" through
"HNI_0100.JPG", thereafter, the DSi will switch to next higher folder number,
and wrap to using "HNI_0001.JPG" as first filename in that folder.

The weird "484E494A" folder name is based on the japanese Nintendo DSi Camera's
gamecode (HNIJ) converted to an 8-digit uppercase HEX string (this appears to
be always the japanese gamecode, even on european DSi consoles).

| DSi SD/MMC Camera Files - JPEG's |
| --- |

**Overall JPEG Format (big-endian)**

```

| Offs ID Len Data
 0000h FFD8h ;(start of image) ;SOI
 0002h FFE1h,10C4h,"Exif",00h,00h,<Exif Body> ;(extra "Exif" data) ;APP1
 10C8h FFC0h,0011h,08h,01E0h,0280h,03h,012100h,021101h,031101h ;SOF0
 10DBh FFDBh,0084h, 00 06 04 05 06 05 04 06 06 05 06 07 07 .. 28 28 28 ;DQT
 1161h FFC4h,01A2h, 00 00 01 05 01 01 01 .. .. F8 F9 FA ;DHT
 1305h FFDAh,000Ch,03h,010002h,110311h,003F00h ;(start of scan) ;SOS
 1313h E6 76 F4 DD 4F 0A 3B 60 0F 4C D7 9E 9A 93 3D 4B EE 98 B8 .. ..
 AB4Fh FFD9h ;(end of image) ;EOI |
| --- |

**Exif Body for Nintendo DSi Photos**

The Exif data consists of several headers/footers and data blocks, mixed with
several "IFD" tables.

```

| Offs Siz ExID Type Length Offset ;<-- Format for "IFD" Tables
 0000h 4 "MM",002Ah ;Big-Endian (aka Motorola)
 0004h 4 00000008h ;first IFD offset (IFD0) |
| --- |

IFD0 (Main Image):

```

| 0008h 2 0009h ;number of IFD0 entries
 000Ah 12 010Fh,0002h,00000009h,0000007Ah ;Maker ("Nintendo",0)
 0016h 12 0110h,0002h,0000000Bh,00000084h ;Model ("NintendoDS",0)
 0022h 12 011Ah,0005h,00000001h,00000090h ;Resolution X (72 dpi)
 002Eh 12 011Bh,0005h,00000001h,00000098h ;Resolution Y (72 dpi)
 003Ah 12 0128h,0003h,00000001h,00020000h ;Resolution Unit (2=Inches)
 0046h 12 0131h,0002h,00000005h,000000A0h ;Firmware (Gamecode backwards)
 0052h 12 0132h,0002h,00000014h,000000A6h ;Date/Time Modified
 005Eh 12 0213h,0003h,00000001h,00020000h ;Subsampling (2=datum point)
 006Ah 12 8769h,0004h,00000001h,000000BAh ;Exif SubIFD offset
 0076h 4 000001DEh ;next IFD offset (IFD1)
 007Ah 9+1 "Nintendo",00h,00h ;Maker ("Nintendo",0,0)
 0084h 11+1 "NintendoDS",00h,00h ;Model ("NintendoDS",0,0)
 0090h 00000048h,00000001h ;Resolution X (72 dpi)
 0098h 00000048h,00000001h ;Resolution Y (72 dpi)
 00A0h 5+1 "PINH",00h,00h ;aka HNIP ;Firmware (Gamecode backwards)
 00A6h 20 "YYYY:MM:DD HH:MM:SS",00h ;Date/Time Modified |
| --- |

Sub IFD:

```

| 00BAh 2 000Ah ;number of Sub IFD entries
 00BCh 12 9000h,0007h,00000004h,30323230h ;Exif Version ("0220")
 00C8h 12 9003h,0002h,00000014h,00000138h ;Date/Time Original
 00D4h 12 9004h,0002h,00000014h,0000014Ch ;Date/Time Digitized
 00E0h 12 9101h,0007h,00000004h,01020300h ;Components (Y,Cb,Cr)
 00ECh 12 927Ch,0007h,00000042h,00000160h ;Maker dependent internal data
 00F8h 12 A000h,0007h,00000004h,30313030h ;Flashpix Version ("0100")
 0104h 12 A001h,0003h,00000001h,00010000h ;Color Space (1=Normal=sRGB)
 0110h 12 A002h,0004h,00000001h,00000280h ;Pixel Dimension X (640)
 011Ch 12 A003h,0004h,00000001h,000001E0h ;Pixel Dimension Y (480)
 0128h 12 A005h,0004h,00000001h,000001A2h ;Interoperability IFD (R98)
 0134h 4 00000000h ;next IFD offset (none)
 0138h 20 "YYYY:MM:DD HH:MM:SS",00h ;Date/Time Original
 014Ch 20 "YYYY:MM:DD HH:MM:SS",00h ;Date/Time Digitized |
| --- |

Maker dependent IFD (DSi specific):

```

| 0160h 2 0002h ;number of IFD entries
 0162h 12 1000h,0007h,0000001Ch,0000017Eh ;DSi Signature (IV+MAC)
 016Eh 12 1001h,0007h,00000008h,0000019Ah ;DSi Whatever Zero (Frame info?)
 017Ah 4 00000000h ;next IFD offset (none)
 017Eh 12 2E AB A5 D1 FD A8 .. .. ;DSi Signature (IV) ;\
 018Ah 16 xx xx xx xx xx xx .. .. ;DSi Signature (MAC) ;/
 019Ah 8 0000000000000000h ;<-- different for Frames |
| --- |

Interoperability IFD (R98) (some common/useless stuff for JPEGs):

```

| 01A2h 2 0003h ;number of IFD entries
 01A4h 12 0001h,0002h,00000004h,52393800h ;Stipulated File ("R98",0)
 01B0h 12 0002h,0007h,00000004h,30313030h ;Whatever ("0100")
 01BCh 12 1000h,0002h,00000012h,000001CCh ;Whatever (JPEG Exif Ver 2.2",0)
 01C8h 4 00000000h ;next IFD offset (none)
 01CCh 18 "JPEG Exif Ver 2.2",00h ;Whatever (JPEG Exif Ver 2.2",0) |
| --- |

IFD1 (Thumbnail Image):

```

| 01DEh 2 0006h ;number of IFD1 entries
 01E0h 12 0103h,0003h,00000001h,00060000h ;Compression (1=JPEG)
 01ECh 12 011Ah,0005h,00000001h,0000022Ch ;Resolution X (72 dpi)
 01F8h 12 011Bh,0005h,00000001h,00000234h ;Resolution Y (72 dpi)
 0204h 12 0128h,0003h,00000001h,00020000h ;Resolution Unit (2=Inches)
 0210h 12 0201h,0004h,00000001h,0000023Ch ;Jpeg Offset
 021Ch 12 0202h,0004h,00000001h,00000xxxh ;Jpeg Size (eg. E80h)
 0228h 4 00000000h ;next IFD offset (none)
 022Ch 8 00000048h,00000001h ;Resolution X (72 dpi)
 0234h 8 00000048h,00000001h ;Resolution Y (72 dpi) |
| --- |

Thumbnail Data (160x120pix, in JPEG Format):

```

| 023Ch 2 FFD8h ;(start of thumbnail/image) ;SOI
 023Eh 13h FFC0h,0011h,08h,0078h,00A0h,03h,012100h,021101h,031101h ;SOF0
 0251h 86h FFDBh,0084h, 00 0A 07 07 08 07 .. .. ;DQT
 02D7h 1A4h FFC4h,01A2h, 00 00 01 05 01 .. .. F8 F9 FA ;DHT
 047Bh 0Eh FFDAh,000Ch,03h,010002h,110311h,003F00h ;(start of scan) ;SOS
 0489h ... CC 55 14 F0 3D 2B 8B 4B 9D C2 E3 BD 18 A5 B0 09 B6 .. ..
 10xxh 2 FFD9h ;(end of thumbnail/image) ;EOI |
| --- |

The above european gamecode entry differs for other regions. Above offsets are
usually as so for DSi jpeg's, but they might change if Nintendo adds/removes
some entries, or changes size of some entries (for example, the Model string is
said to be different for 3DS, and "Frames" are including a bigger entry, as
described below).

**DSi Signature (IV+MAC)**

The 1Ch-byte Signature is split into a 0Ch-byte IV value (this might be just a
random number?), and a 10h-byte MAC value. The MAC is computed via AES-CCM:

```

| IV[00h..0Bh] = First 0Ch-bytes of signature
 KEY[00h..0Fh] = Constant (70,88,52,06,...) ;from BIOS ROM
 Zerofill the 1Ch-byte signature area in the JPEG file
 Probably zeropad(?) the JPEG file (if filesize isn't a multiple of 16 bytes)
 Pass the whole JPEG as "extra associated data" to the AES-CCM hardware
 Copy the IV value and computed MAC value back to the JPEG's signature area |
| --- |

Unknown if the IV value is just random, and unknown if there are further
requirements (such like using same Maker/Model strings or same resolution as in
original DSi files).

Locating "ldr rx,=927Ch" opcodes at various locations in Nintendo DSi Camera is
easy; but the stuff is handled via numerous sub-functions, including IPC stuff
with both ARM7 and ARM9 envolved; which isn't too easy to disassemble.

**Exif Body for Nintendo DSi Frames**

Frames are using same format above, but with the 8-byte zero entry at [019Ah]
replaced by a bigger 8FCh-byte entry (accordingly, the size in IFD entry
[016Eh] is also adjusted, and offsets for entries [01A2h..10xxh] are moved up).

The actual change is that 0000000000000000h is changed from 8 byte size to
8FCh-byte size, containing 0000000000000001h, followed by 8F4h extra bytes
(with unknown content; maybe the frame mask for transparent pixels; the data
doesn't really look like a mask though, unless it's compressed, but then the
fixed size would be strange).

**IFD Type Values (and Length/Offset)**

```

| 0001h = 8bit Unsigned
 0002h = 7bit ASCII
 0003h = 16bit Unsigned
 0004h = 32bit Unsigned
 0005h = 64bit Unsigned Rational (32bit numerator, plus 32bit denominator)
 0006h = Reserved
 0007h = 8bit General Purpose
 0009h = 32bit Signed
 000Ah = 64bit Signed Rational (32bit numerator, plus 32bit denominator)
 000Bh..FFFFh = Reserved |
| --- |

The "Length" value indicates the number of type units, eg. type=16bit, length=3
would mean 6 bytes. If the information fits into 4 bytes then it's stored
directly in the 4-byte "Offset" field, otherwise "Offset" is a pointer to the
actual information.

| DSi SD/MMC Camera Files - pit.bin |
| --- |

**FAT12:\photo\private\ds\app\484E494A\pit.bin (8K) (camera info)**

**SD:\private\ds\app\484E494A\pit.bin (47K) (camera info)**

```

| 0000h 8 ID ("0TIP00_1") (maybe meant to read as PIT01_00 or so)
 0008h 2 Number of pit.bin entries (3000 for SD Card) (500 for eMMC)
 000Ah 2 Unknown (0001h)
 000Ch 2 Next Photo Folder-Number minus 100 (xxxNIN02)
 000Eh 2 Next Photo File-Number minus 1 (HNB_0xxx.JPG)
 0010h 2 Next Frame Folder-Number minus 100 (xxxNIN02)
 0012h 2 Next Frame File-Number minus 1 (HNB_0xxx.JPG)
 0014h 2 CRC16 of whole file (with initial value 0000h, and with
 entry [0014h] being treated as 0000h for calculation)
 0016h 2 Size of Header (0018h)
 0018h+N*10h 4 Entry N, Time/Date (seconds since 01 Jan 2000)
 001Ch+N*10h 8 Entry N, Unknown (zerofilled)
 0024h+N*10h 4 Entry N, Flags (see below)
 0 Used Entry Flag (0=Unused/Deleted, 1=Used)
 1-10 Folder-Number minus 100 (xxxNIN02)
 11-17 File-Number minus 1 (0..99 = HNB_0001..0100.JPG)
 18-19 Sticker (0=None, 1=Star, 2=Clover, 3=Heart)
 20-21 Type (0,3=Photo, 1=Frame, 2=?)
 22-23 Unknown (0,2=Normal?, 1=?, 3=Error)
 24-31 Unused (zero)
 xxx8h 8 Padding for 16-byte filesize alignment (zerofilled) |
| --- |

The "Next Photo/Frame" entries contain File/Folder Numbers where the next
images will be saved; that file numbers increase after saving, and do
eventually wrap to next higher folder number.

The Nintendo DSi Camera utility shows only photos listed in "pit.bin", when
manually copying jpg's to SD Card one could:

```

| - Delete "pit.bin" (it'll be recreated with ALL jpgs, sticker flags are lost)
 - Replace an existing 'listed' file by a new file with same filename
 - Manually edit "pit.bin" and adjust its CRC16 checksum |
| --- |

Photos are region free, can be viewed from any other DSi's (as long as they are
listed in pit.bin). However, they do require some signature in Exif header, so
in general, the DSi accepts only images that come from DSi consoles; not images
from other sources.

**Stickers**

Photos with "Star" sticker are shown as background picture in System Menu; this
works only for images stored in internal eMMC memory (images on SD Card can
have stickers, too, but they are ignored by System Menu).

| DSi SD/MMC Flipnote Files |
| --- |

**FAT16:\title\00030004\4b4755gg\data\public.sav**

This .sav file contains a FAT12 filesystem with following files:

```

| Flipnote(public.sav):\eula.txt ;128 Kbytes, 20000h - zerofilled
 Flipnote(public.sav):\option.bin ;256 bytes, 100h - options
 Flipnote(public.sav):\mark0.pls ;8000 bytes, 1F40h - Heart sticker
 Flipnote(public.sav):\mark1.pls ;8000 bytes, 1F40h - Crown sticker
 Flipnote(public.sav):\mark2.pls ;8000 bytes, 1F40h - Music sticker
 Flipnote(public.sav):\mark3.pls ;8000 bytes, 1F40h - Skull sticker
 Flipnote(public.sav):\recent10.pls ;4000 bytes, FA0h - Recently saved
 Flipnote(public.sav):\friend.pls ;28800 bytes, 7080h - F7,A0,CD,zeroes..
 Flipnote(public.sav):\remind.pls ;10240 bytes, 2800h - F7,A0,CD,zeroes..
 Flipnote(public.sav):\latest1.pls ;256 bytes, 100h - xxxxxxxx,zeroes..
 Flipnote(public.sav):\nand.pls ;160000 bytes, 27100h - All files
 Flipnote(public.sav):\ugo\0NN\XNNNNN_NNNNNNNNNNNNN_NNN.ppm - flipnotes |
| --- |

**Flipnote(public.sav):\option.bin (256 bytes, 100h)**

```

| 0000h 1 Unknown (02h) (?)
 0001h 1 Stylus (00h=Right Hand, 01h=Left Hand)
 0002h 1 Sound Effects (00h=On, 01h=Off)
 0003h 1 Unknown (01h) (?)
 0004h 1 Unknown (00h) (?)
 0005h 1 Unknown (01h) (?)
 0006h 1 Unknown (01h) (?)
 0007h 1 Unknown (03h) (?)
 0008h 1 Unknown (02h) (?)
 0009h 1 Unknown (01h) (?)
 000Ah 1 Unknown (00h) (?)
 000Bh 1 Unknown (01h) (?)
 000Ch 1 Advanced Tools (00h=Off, 01h=On)
 000Dh 1 Pages to Trace (01h..04h=1..4)
 000Eh 1 Frog Display (01h=Off, 01h=On)
 000Fh 1 Start on Calendar (00h=Off, 01h=On)
 0010h 8 Flipnote Studio ID (64bit User ID) (fixed)
 0018h 2 Checksum (see below)
 001Ah 2 Date of Birth, Year (076Ch..0840h=1900..2112)
 001Bh 1 Date of Birth, Month (01h..0Ch=1..12)
 001Ch 1 Date of Birth, Day (01h..1Fh=1..31)
 001Eh E2h Unknown/unused (zerofilled) |
| --- |

Checksum is computed as "chk=0000h, halfword[18h]=0000h, for i=0 to FFh,
chk=chk+(byte[i] xor i), next i, halfword[18h]=chk".

**SD:\private\ds\app\4B4755GG\mark0.pls (Heart sticker), 8000 bytes (1F40h)**

**SD:\private\ds\app\4B4755GG\mark1.pls (Crown sticker), 8000 bytes (1F40h)**

**SD:\private\ds\app\4B4755GG\mark2.pls (Music sticker), 8000 bytes (1F40h)**

**SD:\private\ds\app\4B4755GG\mark3.pls (Skull sticker), 8000 bytes (1F40h)**

**SD:\private\ds\app\4B4755GG\recent10.pls (Recently saved), 4000 bytes (FA0h)**

Recently saved files list, and four files with flipnotes that have "stickers"
assigned to them (which can be done when clicking "Details" in the flipnote
file menu).

```

| 0000h N*3Fh List of filenames (if any) ;\encrypted
 N*3Fh+0 1 End of filename list (00h) ;/
 N*3Fh+1 2 Crippled MD5 checksum bytes [6,8] ;\unencrypted
 N*3Fh+3 SIZ-N*3Fh-3 Padding to end of file (zerofilled) ;/ |
| --- |

The filenames are formatted as so (consisting of device, path, and name):

```

| "sdmc:/private/ds/app/4B4755GG/001/XNNNNN_NNNNNNNNNNNNN_NNN.ppm",0Ah |
| --- |

The files are encrypted using a fixed XOR pattern (repeated every 40h bytes):

```

| F7h,4Ch,6Ah,3Ah,FBh,82h,A6h,37h,6Eh,11h,38h,CFh,A0h,DDh,85h,C0h
 C7h,9Bh,C4h,D8h,DDh,28h,8Ah,87h,53h,20h,EEh,E0h,0Bh,EBh,43h,A0h
 DBh,55h,0Fh,75h,36h,37h,EBh,35h,6Ah,34h,7Fh,B5h,0Fh,99h,F7h,EFh
 43h,25h,CEh,A0h,29h,46h,D9h,D4h,4Dh,BBh,04h,66h,68h,08h,F1h,F8h |
| --- |

The checksum is derived by computing the MD5 checksum across the data bytes (in
unencrypted form), and then crippling the 16-byte MD5 value to 2 bytes.

For example, an empty file contains only three bytes: F7h,A0h,CDh (plus zero
padding).

**SD:\private\ds\app\4B4755GG\001\dirmemo2.lst (files), 160000 bytes (27100h)**

```

| 0000h N*1Dh List of filenames (if any) ;\encrypted
 N*1Dh+0 1 End of filename list (00h) ;/
 N*1Dh+1 2 MD5 checksum bytes [6,8] ;\unencrypted
 N*1Dh+3 SIZ-N*1Dh-3 Padding to end of file (zerofilled) ;/ |
| --- |

The filenames are formatted as so (raw names, without path):

```

| "XNNNNN_NNNNNNNNNNNNN_NNN.ppm",0Ah |
| --- |

Checksum and encryption is same as for the ".pls" files, however, the ".lst"
file contains only raw filenames (without path's).

**SD:\private\ds\app\4B4755GG\gif\XNNNNN_NNNNNNNNNNNNN_NNN.gif**

Flipnotes exported to GIF format. Supported are animated GIFs, and separate
GIFs for each frame. There appears to be no way to view GIFs, or to convert
them back to PPM format.

**SD:\private\ds\app\4B4755GG\001\XNNNNN_NNNNNNNNNNNNN_NNN.ppm ;normal**

**SD:\private\ds\app\4B4755GG\YYYYMMDD\NNN\XNNNNN_NNNNNNNNNNNNN_NNN.ppm ;backup**

Flipnotes animation files.

```

| 0000h 4 File ID ("PARA")
 0004h 4 Size of Animation Data (vid)
 0008h 4 Size of Audio Data (aud) (0=none)
 000Ch 2 Number of Frames minus 1 (NF-1)
 000Eh 2 Unknown (always 24h,00h)
 0010h 2 Lock Flag (0=Open, 1=Locked, prevent editing)
 0012h 2 Preview frame number
 0014h 22 Nickname of Original Author (UCS-2) ;\max 10 characters
 002Ah 22 Nickname of Last Editor (UCS-2) ; (plus ending zero)
 0040h 22 Nickname of User (?) (UCS-2) ;/
 0056h 8 User ID of Original Author (Flipnote Studio ID)
 005Eh 8 User ID of Last Editor (Flipnote Studio ID)
 0066h 18 Filename of Original File (3xHEX, 13xASCII, 2xVER)
 0078h 18 Filename of Current File (3xHEX, 13xASCII, 2xVER)
 008Ah 8 User ID of Previous Editor (Flipnote Studio ID)
 0092h 8 Filename Fragment (3xHEX, 5xHEX)
 009Ah 4 Time/Date (seconds since 1st Jan 2000)
 009Eh 2 Zerofilled
 00A0h 600h Preview Bitmap (8x6 tiles, aka 64x48 pixels, 4bpp)
 06A0h 2 Size of Animation Table (4*NF)
 06A2h 4 Zerofilled
 06A6h 2 Flags (bit0=Can be set?, bit1=Loop/Repeat, bit6=Set?)
 06A8h 4*NF Animation Table (offsets in Animation Data for each frame)
 ... (vid) Animation Data Frame(s)
 ... 1*NF Audio Flags for each Frame (bit0-2: Effect 1-3, bit3-7: Zero)
 ... .. Padding (0..3 bytes zerofilled, for alignment of next entry)
 ... 4 Size of Background music in bytes (0=not used/empty)
 ... 4 Size of Sound effect #1 in bytes (0=not used/empty, max 2000h)
 ... 4 Size of Sound effect #2 in bytes (0=not used/empty, max 2000h)
 ... 4 Size of Sound effect #3 in bytes (0=not used/empty, max 2000h)
 ... 1 Framespeed for playback (1..8) aka "8 minus N"
 ... 1 Framespeed when BGM was recorded (1..8) aka "8-decimal"
 ... 14 Zerofilled
 ... (aud) Audio Data (BGM, followed by Effects 1, 2, 3) (if any)
 ... 80h RSA-OpenPGP-SHA1 across all preceeding bytes
 ... 10h Zerofilled |
| --- |

The RSA signature is in OpenPGP SHA1 format (as used by SWI 23h, however
Flipnote is using it's own RSA functions instead of the BIOS SWIs). The RSA
public/private keys are contained in the Flipnote executable (in the modcrypted
area).

**Animation Data Frame(s)**

```

| Start Length Description
 0000h 1 Pen and Paper information
 0001h 48 Layer 1 Line Encoding (48 bytes = 2bit per 192 lines)
 0031h 48 Layer 2 Line Encoding (48 bytes = 2bit per 192 lines)
 0061h ... Frame Data for Layer 1
 ... ... Frame Data for Layer 2 |
| --- |

The pen and paper byte at the start is encoded as follows:

```

| 0 Paper (0=Black, 1=White)
 1-2 Layer 1 (0=None, 1=Inverse of Paper, 2=Red, 3=Blue)
 3-4 Layer 2 (0=None, 1=Inverse of Paper, 2=Red, 3=Blue)
 5-6 Unknown
 7 New Frame (0=Change between last frame, 1=Totally new frame) |
| --- |

The Line Encoding contains 2bit values for all 192 scanlines (starting with
bit0-1 of the first byte; for the top-most(?) scanline). The meaning of the
2bit values is:

```

| 0 = Skip Line (0 bytes) (0 pixels)
 1 = Packed Line (4+N bytes) (32bit flags, plus Nx8 pixels)
 2 = Inverse Line (4+N bytes) (32bit flags, plus Nx8 inverted pixels)
 3 = Raw Line (32 bytes) (256 pixels) |
| --- |

The packed lines contain a 32bit header (with flags for each 8-pixel fragment
of the line, bit31 being the left-most fragment), followed by data bytes for
each flagged fragment (with 8 pixels per fragment, bit0 being the left-most
pixel). The Inverse lines have the same 32bit flags, but for whatever reason,
the following data byte(s) are to be XORed with FFh.

**Audio Data**

```

| First comes the BGM (if used)
 Then comes sound effect #1 (if used)
 Then comes sound effect #2 (if used)
 Then comes sound effect #3 (if used) |
| --- |

The sound data seems to be a variant of VOX ADPCM at around 8KHz.

**IDs and Filenames**

Half of the Flipnote Studio ID is same as the last 4 bytes of the wifi MAC
address. The filenames are also containing the last 3 bytes of the MAC address.
The "XNNNNN_NNNNNNNNNNNNN_NNN.ppm" filename is also encoded in three header
entries:

```

| Header = Filename = Meaning
 3xHEX = XNNNNN = Based on MAC address (the "X" in "XNNNNN" is what?)
 13xASCII= NNNNNNNNNNNNN = Some 13-digit random number or so as ASCII string
 5xHEX = NNNNNNNNNN = First 10-digits of above 13-digit string
 2xVER = NNN.ppm = Trailing version(?) number (hex, decimal?) |
| --- |

For the file name "G35B20_0909841CDBEB1_002.ppm":

```

| <-hex--> <-----ascii-----------------> <-asc--> <-n->
 Filename (ori) D3 5B 20 30 39 30 39 38 34 31 43 44 42 45 42 31 00 00
 Filename (curr) D3 5B 20 30 39 30 39 38 34 31 43 44 42 45 42 31 02 00
 <-hex--> <--hex------->
 Filename (frag) D3 5B 20 09 09 84 1C DB |
| --- |

**Preview Palette (fixed)**

Fullscreen flipnotes can have only four colors: Black, White, Red, Blue).
Additional shades like gray, magenta, or dark/light colors exist only in
resampled preview images. The Preview List uses pale colors, with higher
contrast for the Selected icon.

```

| Color Purpose (appearance) Preview Selected
 00h N/A (transparent) 1F-1F-1F 1F-1F-1F
 01h Black (dark grey) 13-13-13 0A-0A-0A
 02h White (white) 1F-1F-1F 1F-1F-1F
 03h White+Black (grey) 19-19-19 13-13-13
 04h Red (red) 1F-12-12 1F-09-09
 05h Red+Black (dark red) 1B-13-13 18-0A-0A
 06h Red+White (pink) 1F-19-19 1F-15-15
 07h N/A (green) 0E-1F-0E 02-1F-02
 08h Blue (blue) 12-12-1F 09-09-1F
 09h Blue+Black (dark blue) 13-13-1A 0A-0A-16
 0Ah Blue+White (light blue) 19-19-1F 15-15-1F
 0Bh N/A (green) 0E-1F-0E 02-1F-02
 0Ch Red+Blue (magenta) 1A-13-1A 16-0B-16
 0Dh N/A (green) 0E-1F-0E 02-1F-02
 0Eh N/A (green) 0E-1F-0E 02-1F-02
 0Fh N/A (green) 0E-1F-0E 02-1F-02 |
| --- |

**FAT12:\photo\DCIM\100NIN02\HNI_nnnn.JPG ;camera photos**

Flipnote doesn't contain an own camera function. However, the drawing utility
does allow to import JPGs from the camera partition (ie. images that have been
previously taken via the DSi Camera utility).