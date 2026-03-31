# Filestitledatabasefilestitledbimportdbetc

> Source: https://problemkaputt.de/gbatek.htm
> Section: Filestitledatabasefilestitledbimportdbetc

3DS Files - Title Database Files (title.db, import.db, etc.) 
| |
| --- |

 These files contain data relating to install/usage/management of installed 3DS titles. The database files are located at:
 
```

| 3ds:\dbs\*.db
 sd:\Nintendo 3DS\<ID0>\<ID1>\dbs\*.db |
| --- |

ID0 is the first 0x10-bytes from a SHA256 hash. The installation of SD Card
titles was introduced in the 2.0.0-2 update and the SD dbs files are encrypted
by the general SD filesystem encryption rule. These files are DIFF containers.
These DIFF files do not use external IVFC level 4, so all database data is
duplicated in the container. In this page only the inner content of the
container is described.

These files are only created on SD (via AM) if they don't exist when the eShop
application is starting up, during network init etc (prior to showing the
"system update required" dialog).

**3ds:\dbs\title.db (can contain max 1379 files)**

**sd:\Nintendo 3DS\<ID0>\<ID1>\dbs\title.db**

Title database, contains entries for all installed titles (TWL & CTR) on
the 3DS (each database is responsible for titles installed on its medium).

**3ds:\dbs\import.db**

**sd:\Nintendo 3DS\<ID0>\<ID1>\dbs\import.db**

Import Databases, containing entries for titles (or versions of titles) not yet
installed, ready for transferring to the title.db. ("Automatic Update uses
this, so completing the update takes seconds." uh, why does that take so much
time?)

**3ds:\dbs\tmp_t.db**

Temporary Title database containing one entry for the currently installed
Download Play Child.

**3ds:\dbs\tmp_i.db**

Similar to import.db, except it's used in conjunction with tmp_t.db, for
installing Download Play Children.

The inner content of the container consists of a pre-header with size of 0x80
identifying the Database Type, followed by a BDRI container. The offsets in the
BDRI header are usually relative to the offset to the start of the BDRI header
(0x80 in the file)

Pre Header

```

| 000h 8 ID (eg. "NANDTDB",00h) (see below)
 008h 78h Reserved
 080h ... BDRI filesystem (with 80h-byte files containing Title Info) |
| --- |

The 8-byte ID can be:

```

| ID="NANDIDB",00h --> 3ds:\dbs\import.db
 ID="NANDTDB",00h --> 3ds:\dbs\title.db
 ID="TEMPIDB",00h --> 3ds:\dbs\tmp_i.db, tmp_t.db
 ID="TEMPTDB",00h --> sd:\Nintendo 3DS\<ID0>\<ID1>\dbs\import.db, title.db |
| --- |

**Title Info Entry -- is that... maybe "title.db" files...?**

An entry contains information taken from both the application NCCH file(s) and
TMD.

```

| 000h 8 Title Size
 008h 4 Title Type (usually 40h)
 00Ch 4 Title Version
 010h 1 Flags_0.0 (Electronic Manual: 0=None?, 1=Yes?)
 011h 1 Flags_0.1 (0)
 012h 1 Flags_0.2 (0)
 013h 1 Flags_0.3 (0)
 014h 4 TMD Content ID (aka filenumber for "000000xx.tmd" ?)
 018h 4 CMD Content ID (aka filenumber for "cmd\000000xx.cmd" ?)
 01Ch 1 Flags_1.0 (SD Save Data: 0=None?, 1=Yes?)
 01Dh 1 Flags_1.1
 01Eh 1 Flags_1.2
 01Fh 1 Flags_1.3
 020h 4 Extdata ID low (zero if title doesn't use Extdata)
 024h 4 Reserved (0)
 028h 1 Flags_2.0 DSiWare Related (Visibility on Home Menu/Export Flag?)
 029h 1 Flags_2.1 ?
 02Ah 1 Flags_2.2 ?
 02Bh 1 Flags_2.3 ?
 02Ch 1 Flags_2.4 in DSiWare and in titles with an 'Application' Title ID
 02Dh 1 Flags_2.5 DSiWare Related (Visibility on Home Menu/Export Flag?)
 02Eh 1 Flags_2.6 ?
 02Fh 1 Flags_2.7 ?
 030h 10h Product Code (eg. "CTR-N-HASJ", "", or "DS INTERNET", zeropadded)
 040h 10h Reserved (0)
 050h 4 Unknown (32bit random/checksum/id/whatever)
 054h 2Ch Reserved (0) |
| --- |

**Notes**

"It is important to note the database doesn't contain a hash of the .cmd. So if
a user has more than one valid set of application data for a given .cmd Content
ID they can be manually interchanged without issue. Though renaming a .cmd file
to match the Content ID which the title.db is expecting will result in an
error, as the CTR for the per-console encryption layer changes depending on the
file path, and the MAC of the .cmd is probably generated with the .cmd Content
ID in mind."

"These NAND/SD /dbs images seem to be loaded by the ARM9 while NATIVE_FIRM is
booting."

"Removing ticket.db from a New3DS with signature checks disabled will not
result in an unbootable system, however all icons except Slot-1 will disappear
from Home. Applets can however still be used. Recovery can be accomplished via
hardmod or arm9loaderhax plus a known good backup of the file (or the whole
partition or disk); Gamecard exploits were not tested, and Browserhax did not
work."