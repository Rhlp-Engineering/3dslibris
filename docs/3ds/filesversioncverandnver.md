# Filesversioncverandnver

> Source: https://problemkaputt.de/gbatek.htm
> Section: Filesversioncverandnver

3DS Files - Version (CVer and NVer) 
| |
| --- |

 **3ds:\title\000400db System Data Archives 2 - NVer and CVer**
 
```

| NVer - RomFS:\version.bin ;exists always (revision number) ;-NVer
 CVer - RomFS:\version.bin ;exists always (major.minor version) ;\
 CVer - RomFS:\cup_list ;exists only before 8.0.0-18 ; CVer
 CVer - RomFS:\masterkey.bin ;exists in 7.2.0-17 and up ;
 CVer - RomFS:\titleversion.txt ;exists in 9.6.0-X and up ;/ |
| --- |

See also:

3DS Files - FIRM Versions

[http://www.3dbrew.org/wiki/Home_Menu](http://www.3dbrew.org/wiki/Home_Menu)
 <-- more versions!

**NVer - RomFS:\version.bin (8 bytes)**

This 8-byte file mainly contains the system revision number, for the version
number displayed by System Settings. The rest of the data used for the system
version is contained in CVer.

```

| 000h 2 Reserved (0)
 002h 1 Revision version number (0..45) (the version's -N suffix)
 003h 1 Reserved (0)
 004h 1 ASCII character for the system version region (eg. "J"=Japan)
 005h 3 Reserved (0) |
| --- |

Note: The NVer "revision" number does usually increase alongsides with the
"major.minor" number in CVer (but some updates have only either one of those
increased) (unknown why, maybe one file refers to FIRM version, and the other
to Kernel version or whatever).

**CVer - RomFS:\version.bin (8 bytes)**

This 8-byte file contains most of the data used for displaying the system
version in System Settings. Format of the system version:
"major.minor.build-revision region".

```

| 000h 1 Build version number (0)
 001h 1 Minor version number (0..13)
 002h 1 Major version number (1..11)
 003h 1 Reserved (0)
 004h 1 ASCII character for the system version region (eg. "J"=Japan)
 005h 3 Reserved (zeroes, or same as [000h..002h]) |
| --- |

**CVer - RomFS:\cup_list (800h bytes) (exists only before 8.0.0-18)**

```

| 000h N*8 TitleID list of system titles (8 bytes each) ;eg. 127 titles
 ... .. Zerofilled (padding to 800h-byte filesize) |
| --- |

The list contains most (not all) system titles.

This file was removed from the CDN CVer RomFS, starting with 8.0.0-18.

A similar "cup_list" file is bundled with System Update CIA's:

3DS Files - Title Installation Archive (CIA)

**CVer - RomFS:\masterkey.bin (40h bytes, exists in 7.2.0-17 and up)**

```

| 000h 1 Region (00h=Japan, 01h=USA, 02h=EUR, etc.) ;\aka RVV in parental
 001h 1 Version (0Ah=v7.2, 0Bh=v8.0, etc.) ;/Enquiry Number
 002h 0Eh Zerofilled
 010h 10h AES IV value for decrypting below key
 020h 20h Encrypted HMAC key for resetting Parental Controls |
| --- |

This file is used by System Settings, for resetting Parental Controls. The HMAC
key is decrypted using AES-CTR using the above AES IV value and an AES key from
"mset .rodata" (uh, what/where?) (there are different AES keys depending on the
RVV region/version digits).

Parental Controls Reset

**CVer - RomFS:\titleversion.txt (ASCII 5-7 bytes) (exists in 9.6.0-X and up)**

With 9.6.0-X, this is a 5-byte file containing the following: "9.6.7". This is
the parsed version of the CVer TMD title-version.

After USA 9.6.0-X for <=v11.0, this matches the version from CVer
version.bin.