# Filestitleinstallationarchivecia

> Source: https://problemkaputt.de/gbatek.htm
> Section: Filestitleinstallationarchivecia

3DS Files - Title Installation Archive (CIA) 
| |
| --- |

 **CIA Format (CTR Importable Archive)**
 
```

| 0000h 4 Header Size (usually 2020h bytes)
 0004h 2 Type (seems to be always 0)
 0006h 2 Version (seems to be always 0)
 0008h 4 Cert size (0A00h, or 0=None)
 000Ch 4 Ticket Size (0350h, or 0=None)
 0010h 4 TMD Size (0B34h+N*30h, or 200h=Dummy)
 0014h 4 Meta Size (3AC0h, or 200h=Dummy, 8=CVer USA, or 0=None)
 0018h 8 APP Size (can contain multiple contents, see TMD)
 0020h 2000h APP Flags (64Kbit/flags, usually 80h or C0h plus zeropadding)
 2020h 20h Zerofilled (padding to 40h-byte boundary)
 2040h [08h] Certificate chain
 ... .. Zerofilled (padding to 40h-byte boundary)
 ... [0Ch] Ticket file
 ... .. Zerofilled (padding to 40h-byte boundary)
 ... [10h] TMD file
 ... .. Zerofilled (padding to 40h-byte boundary)
 ... [18h] APP file(s) (usually encrypted via ticket)
 ... .. Zerofilled (padding to 40h-byte boundary)
 ... [14h] Meta file (3AC0h, or 08h, or 0=None) (if any)
 ... .. Zerofilled (padding to 40h-byte boundary) |
| --- |

**Certificate Chain (A00h bytes, with three certificates)**

```

| RSA-4096 for below certificates (Retail=CA00000003, or Dev=CA00000004)
 RSA-2048 for Ticket signature (Retail=XS0000000c, or Dev=XS00000009)
 RSA-2048 for TMD signature (Retail=CP0000000b, or Dev=CP0000000a) |
| --- |

The same three certificates are also stored in certs.db,

3DS Files - Title Certificates (certs.db)

**Ticket (350h bytes), with decrypt key for APP file(s)**

3DS Files - Title Ticket (ticket.db)

**TMD (B34h+N*30h bytes), contains info about the separate APP file(s)**

3DS Files - Title Metadata (TMD)

**APP file(s)**

This region can contain one or more contents (eg. separate .app files for
executable and manual, with .app sizes as listed in the TMD file; the .app
files can be 3DS NCCH images, or DSiware images, or DSi datafiles like DS Cart
Whitelist).

The .app contents are usually encrypted using the ticket's titlekey (see the
Ticket chapter for details).

```

| There are two unencrypted variants:
 1) Encryption can be disabled in Content Type flags in the TMD file (that's
 done in most or all homebrew CIA's like ctrQuake.cia)
 2) Pre-retail prototype CIA's from mid 2010 didn't include tickets, and are
 thus unencrypted (the Ticket Size entry in CIA header is probably zero?) |
| --- |

**Meta (3AC0h bytes) (if any)**

```

| 0000h 30h*8 Dependency module list ;from NCCH Exheader ;NCCH[240h]
 0180h 180h Reserved (0)
 0300h 4 Core version ;from NCCH Exheader ;NCCH[408h?]
 0304h FCh Reserved (0)
 0400h 36C0h SMDH Icon/Title ;from NCCH ExeFS ;ExeFS:\icon |
| --- |

The above data block is only present for NCCH executables (DSiware files and
NCCH data files don't have the Exheader and ExeFS).

3DS Files - NCCH Extended Header

3DS Files - Video Icons (SMDH)

**Meta (8 bytes) (CVer USA)**

Seems to contain a copy of the "CVer - RomFS:\version.bin" file.

3DS Files - Version (CVer and NVer)

Confusingly, the japanese system update contains 000400db00017202.cia (that
name would be CVer USA, but the file contains CVer JPN). And confusingly, the
system update doesn't contain any NVer file (unknown if/how it is updating
NVer).

**APP Flags**

This appears to be a 64Kbit array with flags for Content Index 0000h..FFFFh
(the first flag for index 0000h being located in bit7 of first byte).

The number of contents and content indices are found in the TMD file (most
titles have only 1 or 2 contents, so the first flag byte is usually 80h or C0h,
followed by 1FFFh zero bytes).

Unknown if these flags are used for anything... maybe there are cases where the
TMD lists multiple APPs, but only one of them being updated in the CIA?

```

| ___________________________________ Blurp ____________________________________ |
| --- |

**Overview**

CIA format allows the installation of titles to the 3DS. CIA files and titles
on Nintendo's CDN contain identical data. As a consequence, valid CIA files can
be generated from CDN content. This also means CIA files can contain anything
that titles on Nintendo's CDN can contain. Uh, what is a CDN?

Under normal circumstances CIA files are used where downloading a title is
impractical or not possible. Such as distributing a Download Play child, or
installing forced Gamecard updates. Those CIA(s) are stored by the titles in
question, in an auxiliary CFA file.

Development Units, are capable of manually installing CIA files via the Dev
Menu.

**Format**

The CIA format has a similar structure to the Wii's WAD format (aka DSi Tad?).

The file is represented in little-endian.

The data is aligned in 40h-byte blocks (if a content ends at the middle of the
block, the next content will begin from a new block).

```

| _____________________________ System Update CFA ______________________________ |
| --- |

This system update CFA contains the below files in the RomFS. This CFA is
stored in the NCSD system-update partition (partition 7), the data from this
CFA RomFS can also be transferred for Download Play when the client needs
updated.

```

| ROM Cart NCSD Partition 6, NCCH RomFS:\SNAKE\cup_list ;\New3DS System Update
 ROM Cart NCSD Partition 6, NCCH RomFS:\SNAKE\*.cia ;/
 ROM Cart NCSD Partition 7, NCCH RomFS:\cup_list ;\Old3DS System Update
 ROM Cart NCSD Partition 7, NCCH RomFS:\*.cia ;/ |
| --- |

**cup_list**

This uses the same format as the CVer cup_list, this system-update CFA cup_list
contains the titleIDs for all of the titles stored in this RomFS.

3DS Files - Version (CVer and NVer)

**<titleID>.cia**

This is the CIA for each system-update title, for retail gamecards these CIAs
are generated from the titles available on CDN at the time the NCSD image was
finalized. The latest version(at the time of CFA generation) of every system
title from CDN is stored here, excluding titles with the title-versions for
1.0.0-0.

**New3DS**

New3DS system-update partitions are located at partition6 in gamecards. The
RomFS contains a "SNAKE" directory, the contents of that directory have the
same structure as the Old3DS sysupdate CFA RomFS.

```

| _____________ Old CIA Formats (some pre-retail prototype stuff) ______________ |
| --- |

**24/06/2010 - CTR_SDK: 0_10**

```

| 000h 4 Archive Header Size (Usually = 0x2020 bytes)
 004h 2 Type
 006h 2 Version
 008h 4 Unused (aka size=0, or what?) (no Cert)
 00Ch 4 Unused (aka size=0, or what?) (no Tick)
 010h 4 Zero Filled Data (usually 0x200 bytes in length) (no TMD)
 014h 4 Zero Filled Data (usually 0x200 bytes in length) (no Meta)
 018h 8 APP file size
 020h 2000h Content Index |
| --- |

The order of the sections in the CIA file:

```

| * Header
 * Zero Data (aka dummy TMD?)
 * Zero Data (aka dummy Meta?) (but here with Meta in front of APP area!?)
 * APP file data (unencrypted, as there's no ticket with titlekey) |
| --- |

Only one executable supported. Uh, are there MORE than one "executable"
supported in later versions??? With different title IDs??? But that would
somewhat require multiple TMD's and Ticket's?

**06/07/2010 - CTR_SDK: 0_10_2**

```

| 000h 4 Archive Header Size (Usually = 0x2020 bytes)
 004h 2 Type
 006h 2 Version
 008h 4 Unused (aka size=0, or what?) (no Cert)
 00Ch 4 Ticket size
 010h 4 TMD file size
 014h 4 Unused (aka size=0, or what?) (no Meta)
 018h 8 APP file size
 020h 2000h Content Index |
| --- |

The order of the sections in the CIA file:

```

| * Header
 * Ticket
 * TMD
 * APP file data |
| --- |

Very similar to the final CIA version (encryption, TMD, etc), except there is
no embedded Certificate Chain.

**Late 2010**

The final version was invented in late 2010 (and, with the 3DS being released
in 2011, retail CIA's are probably always using the final version).