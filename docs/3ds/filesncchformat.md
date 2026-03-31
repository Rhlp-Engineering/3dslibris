# Filesncchformat

> Source: https://problemkaputt.de/gbatek.htm
> Section: Filesncchformat

3DS Files - NCCH Format 
| |
| --- |

 **NCCH (Nintendo Content Container Header) (in .app files)**
 This format is used to store the content of any installed title. There are two NCCH variants:
 
```

| CXI aka CTR Executable Image, with ARM11 code
 CFA aka CTR File Archive, with data only |
| --- |

The CXI/CFA format is structured in the following order:

```

| NCCH header (unencrypted)
 Exheader (CXI only) (encrypted)
 Logo (5.0.0-11 and up) (unencrypted)
 SDK Strings (aka plain binary region) (optional, CXI only) (unencrypted)
 ExeFS filesystem (optional) - ARM11 code (CXI), icon/banner (encrypted)
 RomFS filesystem (optional) - Used for external file storage (encrypted) |
| --- |

User-executable Tools (eg. System Settings) do have two NCCH .app files: One
with the program .code, and one with the data manual. Internal System Modules
(eg. MCU driver) have only one .app file (with program .code, without manual).

**NCCH Header**

```

| 000h 100h RSA-2048 SHA-256 signature of the NCCH header
 100h 4 ID "NCCH"
 104h 4 Content size, in media units (1 media unit = 200h bytes)
 108h 8 Partition ID (usually same as Program ID, or weird stuff...?)
 110h 2 Maker code (3030h)
 112h 2 Version (0000h=Normal/Data?, 0001h=Weird?, 0002h=Normal/Code?)
 114h 4 SEEDDB Checksum (see NCCH Encryption chapter) (firmware 9.6.0)
 118h 8 Program ID (aka Title ID)
 120h 10h Reserved
 130h 20h Logo Region SHA-256 hash (5.0.0-11 and up)
 150h 10h Product code ("CTR-x-xxxx") (for CFA: always "CTR-P-CTAP"?)
 160h 20h Extended header SHA-256 hash (SHA256 of 2x Alignment Size... uh?)
 180h 4 Extended header size minus 400h, in bytes (usually 400h) (0=None)
 184h 4 Reserved
 188h .. "Flags" (see below) (aka "ncchflag[0..7]")
 188h 3 Unknown (zero)
 18Bh 1 Crypto 2nd Keyslot (00h=None, 01h=Key25h, 0Ah=Key18h, 0Bh=Key1Bh)
 18Ch 1 Content Platform (01h=CTR=3DS, 02h=Snake=New3DS)
 18Dh 1 Content Type Bit-masks: Data=0x1, Executable=0x2, SystemUpdate=0x4,
 Manual=0x8, Child=(0x4|0x8), Trial=0x10.
 When 'Data' is set, but not 'Executable', NCCH is a CFA.
 Otherwise when 'Executable' is set, NCCH is a CXI.
 18Eh 1 Content Unit Size (200h SHL N) ;uh, NOT? same as "media units"?
 18Fh 1 Flags Bit0=FixedKey, Bit1=NoMountRomFs, Bit2=NoCrypto, Bit5=NewKeyY
 190h 4 Plain region offset, in media units
 194h 4 Plain region size, in media units
 198h 4 Logo Region offset, in media units ;\For applications built with
 19Ch 4 Logo Region size, in media units ;/SDK 5+ (5.0.0-11 and up)
 1A0h 4 ExeFS offset, in media units
 1A4h 4 ExeFS total size, in media units
 1A8h 4 ExeFS header size, in media units (for SHA256 at [1C0h])
 1ACh 4 Reserved
 1B0h 4 RomFS offset, in media units
 1B4h 4 RomFS total size, in media units
 1B8h 4 RomFS header size, in media units (for SHA256 at [1E0h])
 1BCh 4 Reserved
 1C0h 20h ExeFS superblock SHA-256 hash
 1E0h 20h RomFS superblock SHA-256 hash |
| --- |

All of the hashes stored in this NCCH header are over the cleartext data.

Given offsets are based on the start of the file.

**Extended header**

The extended header contains additional information regarding/restricting
access control.

3DS Files - NCCH Extended Header

**Logo**

3DS Files - NCCH Logo

**Plain binary region (SDK strings) (CXI only)**

This region contains several zero-terminated ASCII strings (usually in form
"[SDK+Blah:Blahblah]",00h).

```

| "The version used for the "FIRMWARE" tag (what?) is the kernel/FIRM
 version, this version can also be stored in the exheader "kernel release
 version" ARM11 kernel descriptor field. As of 2.2.0-X the NATIVE_FIRM
 kernels check the CXI exheader "kernel release version" field, if it is
 stored in the CXI exheader. If the kernel/FIRM version specified by this
 field is higher than the version of the running NATIVE_FIRM, the kernel
 will return error-code D9001413h." |
| --- |

Uh, all that blurb seems to say that... the SDK string is irrelevant - and if
there's anything relevant, then it's the descriptor, but not/never the SDK
string?

**ExeFS and RomFS (Filesystems)**

3DS Files - NCCH ExeFS

3DS Files - NCCH RomFS

Both ExeFS and RomFS are designed as read-only filesystems (in lack of a FAT,
changing filesizes would require to relocate all following files).

**Encryption (for Exheader, ExeFS, and RomFS)**

3DS Files - NCCH Encryption

**Region Locking**

The region-locking info checked by home menu is stored in the "icon" file in
ExeFS.

**RSA**

CFAs NCCH header signature uses a fixed RSA public key.

CXIs NCCH header signature uses the RSA public key stored in the Exheader
(whereof, that part of the Exheader is signed via another RSA key).

**Tools**

ctrtool - (CMD) (Windows/Linux) Parsing and decrypting (debug only) NCCH files