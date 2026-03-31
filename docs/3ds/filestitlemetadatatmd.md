# Filestitlemetadatatmd

> Source: https://problemkaputt.de/gbatek.htm
> Section: Filestitlemetadatatmd

3DS Files - Title Metadata (TMD) 
| |
| --- |

 Title metadata is a format used to store information about a title (installed title, DLC, etc.) and all its installed contents, including which contents they consist of and their SHA256 hashes.
 
 **Signature Type**
 
```

| Value Signature Method Signature Size Padding Size
 0x010000 RSA_4096 SHA1 (Unused for 3DS) 0x200 0x3C
 0x010001 RSA_2048 SHA1 (Unused for 3DS) 0x100 0x3C
 0x010002 Elliptic Curve with SHA1 (Unused for 3DS) 0x3C 0x40
 0x010003 RSA_4096 SHA256 0x200 0x3C
 0x010004 RSA_2048 SHA256 0x100 0x3C
 0x010005 ECDSA with SHA256 0x3C 0x40 |
| --- |

The hash for the signature, is calculated over the header of the TMD.

**TMD Format**

```

| 000h 4 Signature Type (00h,01h,00h,04h) (100h-byte RSA-SHA256)
 004h 100h Signature RSA-SHA256 across 140h..203h
 104h 3Ch Signature padding/alignment (zerofilled)
 140h 40h Signature Name "Root-CA00000003-CP0000000b", 00h-padded
 180h 1 TMD Version (always 1 on 3DS, unlike DSi)
 181h 1 ca_crl_version (0)
 182h 1 signer_crl_version (0)
 183h 1 Zero (padding/align 4h)
 184h 8 System Version (0)
 18Ch 8 Title ID
 194h 4 Title Type (40h, big-endian)
 198h 2 Group ID (0)
 19Ah 4 SD/MMC "public.sav" filesize in bytes (3DS: savedata size)
 19Eh 4 SD/MMC "private.sav" filesize in bytes (3DS: 0=none)
 1A2h 4 Zero
 1A6h 1 SRL Flag (00h=3DS, but also 0=DSiware, uh?)
 1A7h 3 Zero
 1AAh 10h Zerofilled (DSi: Parental Control Age Ratings)
 1BAh 1Eh Zerofilled
 1D8h 4 Access Rights (0)
 1DCh 2 Title Version
 1DEh 2 Content Count (number of entries at B04h)
 1E0h 2 Boot Content
 1E2h 2 Padding
 1E4h 20h SHA256 across Content Info Records at [204h..B03h]
 204h 900h Content Info Records (40h entries, with 24h-bytes each)
 B04h 30h*N Content Chunk Records (N entries, with 30h-bytes each) |
| --- |

**Content Info Records (useless, can be ignored, except for error checking)**

There are 64 of these records, usually only the first is used.

```

| 000h 2 Content index offset (I)
 002h 2 Content command count (N)
 004h 20h SHA256 across TMD[B04h+I*30h+(0..N*30h-1) ;(?) |
| --- |

**Content chunk records**

There is one of these for each content contained in this title (determined by
"Content Count" in the TMD Header).

```

| 000h 4 Content id (aka name of .app file maybe?) (homebrew/dev=random?)
 004h 2 Content index (see below)
 006h 2 Content type (see below)
 008h 8 Content size (of app file)
 010h 20h SHA-256 hash (on what? the app file?) (before/after decrypt?) |
| --- |

Content Index:

```

| 0000h = Main Content (3DS NCCH.exec, or 3DS NCCH.data, or DSiware/DSidata)
 0001h = Home Menu Manual (.CFA)
 0002h = DLP Child Container (.CFA)
 This does not apply to DLC. |
| --- |

Content Type flags:

```

| 0 Encrypted (CIA blocks) (0=Unencrypted/homebrew, 1=Encrypted)
 1 Disc (uh?) (0=Normal, 1=whut?)
 2 CFM (abbreviation for?) (0=Normal, 1=whut?)
 3-13 Zero (0=Normal)
 14 Optional (uh?) (0=Normal, 1=whut?)
 15 Shared (uh?) (0=Normal, 1=whut?) |
| --- |

**Certificate Chain**

If the TMD file is obtained from Nintendo's CDN, then it will have two
certificates appended at the end of the file:

```

| CERTIFICATE SIG.TYPE RETAIL CERT DEBUG CERT DESCRIPTION
 TMD RSA-2048 CP0000000b CP0000000a to verify the TMD signature
 CA RSA-4096 CA00000003 CA00000004 to verify the TMD Certificate |
| --- |

The CA certificate is issued by 'Root', the public key for which is stored in
NATIVE_FIRM.