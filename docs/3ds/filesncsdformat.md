# Filesncsdformat

> Source: https://problemkaputt.de/gbatek.htm
> Section: Filesncsdformat

3DS Files - NCSD Format 
| |
| --- |

 NCSD format is used for internal eMMC storage & for 3DS ROM cartridges. The NCSD can contain up to 8 partitions.
 
```

| For eMMC, the partitions are:
 Part Type Content
 0 MBR DSi MBR (with two FAT filesystems, FAT16+FAT12)
 1 GBA-SAVE GBA savegame (temporary storage, used on power-off in GBA mode)
 2 FIRM Firmware partition (firm0)
 3 FIRM Firmware partition (firm1) (backup copy, same as above)
 4 MBR 3DS MBR (with one FAT filesystem, FAT16)
 5-7 - Unused
 For 3DS ROM Cartridges (aka CCI images), the partitions are:
 Part Type Content
 0 NCCH Executable Content (CXI)
 1 NCCH E-Manual (CFA)
 2 NCCH Download Play Child container (CFA) (if any)
 3-5 - Unused
 6 NCCH New3DS System Update Data (CFA) (if any) ;\aka firmware updates
 7 NCCH Old3DS System Update Data (CFA) ;/ |
| --- |

**Blurb**

There are two known specialisations of the NCSD container format:

```

| - The CTR Cart Image (CCI) format (CCI is the format of game ROM images)
 - The 3DS' raw NAND format |
| --- |

CTR System Update (CSU) is a variant of CCI, where the only difference is in
the file extension (uh, that is the file extension of a ROM-image that needs to
be copied to a dev flashcard or so?). This is used with developer System
Updates and associated Tools.

The format of partitions can be determined from the partition FS flags
(normally these are zero for CCI/CSU NCSD Images) (uh, what/where is "FS
flags"?).

**NCSD header**

```

| 000h 100h RSA-2048 SHA-256 signature of the NCSD header (including MBR!)
 100h 4 ID "NCSD"
 104h 4 Size of the NCSD image (in media units)
 108h 8 ? Media ID
 110h 8*1 Partitions Type (0=NCCH or Unused, 1=MBR, 3=FIRM, 4=GBA-SAVE)
 118h 8*1 Partitions Crypt (0=NCCH or Unused, 1=DSi, 2=3DS, 3=New3DS)
 120h 8*(4+4) Partitions Offset & Size (in media units)
 For eMMC:
 160h 25h Zerofilled
 185h 1 Unknown (04h)
 186h 6 Zerofilled
 18Ch 1 Unknown (01h)
 18Dh 1 Zero
 18Eh 1 Media Unit Size (200h SHL N) (usually 00h, aka 200h-bytes)
 18Fh 2Fh Zerofilled (unencrypted, despite of below)
 1BEh 42h Encrypted MBR partition-table, for the two DSi partitions
 (key-data used for this keyslot is console-unique).
 For ROM carts:
 160h 20h Exheader SHA-256 hash (uh, really?) (usually 0)
 180h 4 Additional header size (uh, does that mean "Exheader"?) (usually 0)
 184h 4 Sector zero offset (whatever, in whatever units, usually 0)
 188h .. Partition Flags (uh, why called "flags" and why "partition"?)
 188h 1 Backup Write Wait Time (The time to wait to write save to backup
 after the card is recognized (0-255 seconds)). NATIVE_FIRM loads
 this flag from the gamecard NCSD header starting with 6.0.0-11.
 189h 1 unknown, maybe related to below "Save Crypto"?
 18Ah 1 unknown, ?
 18Bh 1 Media Card Device (1=NOR Flash, 2=None, 3=BT) (SDK 3.X+)
 18Ch 1 Media Platform Index (1=CTR)
 18Dh 1 Media Type Index (0=InnerDevice, 1=Card1, 2=Card2, 3=ExtendedDevice)
 18Eh 1 Media Unit Size (200h SHL N)
 18Fh 1 Media Card Device (1=NOR Flash, 2=None, 3=BT) (Only SDK 2.X)
 190h 40h Partition ID table (8x8 bytes)
 1D0h 20h Reserved
 1F0h 0Eh Reserved?
 1FEh 1 Support for this was implemented with 9.6.0-X FIRM.
 Bit0=1 enables using bits 1-2, it's unknown what these two
 bits are actually used for (the value of these two bits get
 compared with some other value during NCSD
 verification/loading).
 This appears to enable a new, likely hardware-based,
 antipiracy check on cartridges.
 1FFh 1 Support for this was implemented with 9.6.0-X FIRM, see below
 regarding save crypto. |
| --- |

**NCSD Signature**

The RSA pubk used for gamecard NCSD is stored in ITCM (what/wherefrom?).

The RSA pubk used for NAND NCSD is stored in "Process9 .(ro)data instead of
ITCM". Uh, shouldn't that key (also?) be in bootrom?

**Partition Flags (In Terms of Save Crypto Determination) (uh, Save what?)**

Note: Byte 01h,03h,07h do probably refer to NCSD[189h,18Bh,18Fh].

Note: flag[1,3,7] do probably ALSO refer to NCSD[189h,18Bh,18Fh].

Note: partitionflag[1,3,7] do MAYBE ALSO refer to NCSD[189h,18Bh,18Fh]?

```

| Byte Description
 01h Starting with 6.0.0-11 NATIVE_FIRM will use this flag to determine
 the gamecard savegame keyY method, when flag[3] is set.
 00h = 2.0.0-2 hashed keyY,
 01h = new keyY method implemented with 6.0.0-11.
 0Ah = implemented with 9.3.0-X. On Old3DS this is identical to
 the 2.2.0-4 crypto. On New3DS this is identical to the 2.2.0-4
 crypto, except with New3DS-only gamecard savedata keyslots. |
| --- |

Starting with 9.6.0-X FIRM, Process9 now sets <savecrypto_stateval> to
partitionflag[1] + <the u8 value from NCSD+0x1FF>, instead of just
setting it to partitionflag[1].

```

| 03h Support for this flag was implemented in NATIVE_FIRM with 2.0.0-2.
 When this flag is set the hashed gamecard savegame keyY method is
 used, this likely still uses the repeating-CTR however.
 With 6.0.0-11 the system will determine the gamecard savegame keyY
 method via flag[1], instead of just using the hashed keyY via this
 flag.
 07h This flag enables using the hashed gamecard savegame keyY method,
 support for this flag was implemented in NATIVE_FIRM with 2.2.0-4.
 All games with the NCSD image finalized since 2.2.0-4 (and contains
 2.2.0-4+ in the system update partition) have this flag set, this
 flag also enables using new CTR method as well. |
| --- |

Starting with 9.6.0-X FIRM, Process9 will just write val0 to a state field then
return 0, instead of returning an error when the save crypto type isn't
recognized. This was the *only* actual functionality change in the Old3DS
Process9 function for gamecard savedata crypto init.

**Card Info Header - uh, maybe this is the "Exheader" aka "Additional header"?**

```

| Offset Size Description
 200h 4 CARD2: Writable Address In Media Units (For 'On-Chip' Savedata)
 CARD1: Always FFFFFFFFh.
 204h 4 Card Info Bitmask
 208h 108h? Reserved1
 300h 4 Unknown (1DD7AA00h)
 ... ..
 310h 2 Title version
 312h 2 Card revision
 ... ..
 320h 8 Title ID for CVer USA (000400db00017202h) (even so in JPN cart)
 328h ? Unknown (1022h)
 208h?? CEEh? Reserved2
 1000h 10h Card seed AES-KeyY (first u64 is Media ID (same as first NCCH
 partitionId))
 1010h 10h Encrypted card seed (AES-CCM, keyslot 3Bh for retail cards,
 see CTRCARD_SECSEED)
 1020h 10h Card seed AES-MAC
 1030h 0Ch Card seed AES-IV
 103Ch C4h Reserved3
 1100h 100h Copy of first NCCH header (excluding RSA signature)
 Development Card Info Header Extension:
 Unknown how to read from 1200h..3FFFh. When trying to do so on retail card:
 Encrypted read (cmd BFh) does merely return random garbage at 1200h..3FFFh.
 Unencrypted read (cmd 82h) can read from 1000h only (though MAYBE it can
 read MULTIPLE blocks from 1000h upwards?)
 Below says something about NTR cmd 90h or CTR cmd 90h or A2h, but that
 refers to getting the 32bit Chip ID, not to header data at 1200h and up.
 1200h 200h CardDeviceReserved1
 1400h 10h TitleKey (whatever that is... something alike as in tickets?)
 1410h F0h CardDeviceReserved2 |
| --- |

Note that a particular(=which?) flashcard vendor puts what many(=who?) refer to
as "private headers"(=what?) here in place of actual development card
information. This(=which?) header is constituted(=whut?) by a cartridge-unique
Id obtained from pxi:ps9::GetRomId(=what?) and the title-unique cart ID
(identical for all carts of the same title; can be retrieved using the NTR
gamecard protocol command 90h or through the CTR protocol commands 90h or A2h)
(uh, actually, that is the Chip ID, not a "title-unique cart ID" thing).

**Tools**

ctrtool - (CMD) (Windows/Linux) Parsing NCSD files

3DSExplorer - (GUI) (Windows Only) Parsing NCSD files