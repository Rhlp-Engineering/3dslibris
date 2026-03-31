# Filestitleticketticketdb

> Source: https://problemkaputt.de/gbatek.htm
> Section: Filestitleticketticketdb

3DS Files - Title Ticket (ticket.db) 
| |
| --- |

 **3ds:\dbs\ticket.db (about 35.5Mbytes) (237F600h bytes)**
 ticket.db is a DIFF container containing a short 10h-byte header followed by a BDRI filesystem.
 3DS Files - Savedata DISA and DIFF
 3DS Files - Savedata SAVE/BDRI/VSXE Formats
 The 10h-byte header (between DIFF and BDRI layer) looks as so:
 
```

| 000h 8 ID "TICK",01h,00h,00h,00h
 008h 4 Garbage (eg. random, or 0)
 00Ch 4 Garbage (eg. random-30h, or 1)
 010h ... BDRI filesystem (about 17Mbytes) (10A2000h) (with ticket files) |
| --- |

The BDRI can contain up to 8192 files, usually it does contain about 200
tickets for the system titles, including the DSiware titles, plus any further
tickets for titles purchased from e-Shop (no matter if the titles are stored on
eMCC or on SD Cards).

Tickets are a format used to store an encrypted titlekey (using AES-CBC). With
3DS, the Ticket format was updated (now v1) from Wii/DSi format (v0).

DSi SD/MMC DSiware Tickets and Title metadata

**Ticket format**

```

| Pre 4 One (1) (unknown purpose) ;\little endian ;\in ticket.db
 Pre 4 Size of below ticket (350h) ;/ ;/(not in .cia's)
 000h 4 Signature Type (00h,01h,00h,04h) (100h-byte RSA-SHA256)
 004h 100h Signature RSA-SHA256 across 140h..34Fh ?
 104h 3Ch Signature padding/alignment (zerofilled)
 140h 40h Signature Name "Root-CA00000003-XS0000000c", 00h-padded
 180h 3Ch ECC Public Key (uh, what for?) (can be zero/none)
 1BCh 1 Ticket Version (always 1 on 3DS) (unlike DSi/Wii)
 1BDh 1 CaCrl Version
 1BEh 1 SignerCrl Version
 1BFh 10h TitleKey (normal-key encrypted using one of the common keyYs)
 1CFh 1 Zero
 1D0h 8 Ticket ID (00,04,xx,xx,xx,xx,xx,xx) ? (...or random 64bit)
 1D8h 4 Console ID (Decrypted OTP[04h..07h], in reverse byte-order)
 1DCh 8 Title ID (00,04,xx,xx,xx,xx,xx,xx) (same as ticket filename)
 1E4h 2 Zero
 1E6h 2 Title Version
 1E8h 4 Zero
 1ECh 4 Zero
 1F0h 1 License Type
 1F1h 1 Common keyY number for [1BFh] (0..5) (1 for retail system titles)
 1F2h 2Ah Zero
 21Ch 4 eShop Account ID? (zero in many system files)
 220h 1 Zero
 221h 1 Audit (whatever, usually 01h) (can be 00h, for debug/whatever?)
 222h 20h 00h-filled (Wii: Content access permissions, 1 bit per content)
 242h 20h 00h-filled (Wii: Content access permissions, 1 bit per content)
 262h 2 Zero
 264h 40h Limits
 2A4h ACh Content Index (*) (uh, but WHAT is a Content Index ??)
 350h .. Certificates (included in e-Shop downloads, not stored in files) |
| --- |

* The Content Index of a ticket has its own size defined within itself (usually
ACh), with seemingly a minimal of 20 bytes.

The "Content Index" (whatever that is) at 2A4h looks as so:

```

| 2A4h 4 Unknown (00h,01h,00h,14h)
 2A8h 4 Unknown (00h,00h,00h,ACh) Size of stuff at 2A4h..34Fh ?
 2ACh 4 Unknown (00h,00h,00h,14h)
 2B0h 4 Unknown (00h,01h,00h,14h)
 2B4h 4 Unknown (00h,00h,00h,00h)
 2B8h 4 Unknown (00h,00h,00h,28h)
 2BCh 4 Unknown (00h,00h,00h,01h)
 2C0h 4 Unknown (00h,00h,00h,84h)
 2C4h 4 Unknown (00h,00h,00h,84h)
 2C8h 4 Unknown (00h,03h,00h,00h)
 2CCh 4 Unknown (00h,00h,00h,00h)
 2D0h 20h Unknown (FFh-filled) (or 00h-filled in some tickets)
 2F0h 60h Unknown (00h-filled) |
| --- |

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

The hash for the signature is calculated over the Ticket Data.

The Ticket Title Version is generally the same as the title version stored in
the Title Metadata. Although it doesn't have to match the TMD version to be
valid.

**TitleKey (for decrypting .app's in CIA's or Shop downloads)**

First of, decrypt the ticket's Title Key via AES-CBC:

```

| dev/retail = can be detected from RSA Signature Name at TIK[140h]
 KEY_X[00h..0Fh] = Keyslot 3Dh, as set by bootrom (differs for retail/dev)
 KEY_Y[00h..0Fh] = common key(0..5) from FIRM, index 0..5 from TIK[1F1h]
 KEY[00h..0Fh] = derived from above KeyX/Y (retail key0..5 and dev key1..5)
 KEY[00h..0Fh] = fixed normal key from FIRM (special, for dev key0 only)
 IV[00h..07h] = Title ID (big-endian), from TIK[1DCh]
 IV[08h..0Fh] = Zero
 SRC[00h..0Fh] = Encrypted Title Key, from TIK[1BFh]
 aes-cbc decrypt |
| --- |

Then use the decrypted key to decrypt the .app via AES-CBC:

```

| KEY[00h..0Fh] = Decrypted title key (decrypted from ticket)
 IV[00h..01h] = Content Index (big endian, from TMD[B04h+N*30h+4])
 IV[02h..0Fh] = Zero
 SIZE = Content Size (from TMD[B04h+N*30h+8])
 aes-cbc decrypt (unless TMD[B04h+N*30h+6] has flagged encryption=0) |
| --- |

Note: For AES-CBC random access, one can use the preceeding "encrypted 10h-byte
block" as IV value (except, use the above IV for first block).

Unknown if there is a dev/retail flag in the ticket (if not, then one could

instead check the RSA Signature Name; retail="Root-CA00000003-XS0000000c")

**Limits**

In demos, the first u32 in the "Limits" section is 0x4, then the second u32 is
the max-playcount.

**Certificate Chain**

Tickets retrieved from CDN/SOAP have a certificate chain appended at the end of
the file. There are two certificates in this chain:

```

| CERTIFICATE SIG.TYPE RETAIL CERT DEBUG CERT DESCRIPTION
 Ticket RSA-2048 XS0000000c XS00000009 to verify Ticket signature
 CA RSA-4096 CA00000003 CA00000004 to verify Ticket Certificate |
| --- |

The CA certificate is issued by 'Root', the public key for which is stored in
NATIVE_FIRM.

**Notes**

CommonETicket (for short, cetk) is the name given to tickets for titles which
are not available on the eShop, like system titles. As the name suggests, they
are not unique tickets, the same ticket is common to each 3ds which has
installed that title. This is in contrast to tickets for eShop content, which
are generated prior to initial download, and are unique to 3ds it was generated
for.

CETK can be fetched through HTTP using the link to default update server, using
the title's TMD URL where "cetk" is used instead of "tmd" for the URL. The 3DS
NIM module retrieves system tickets via SOAP request GetSystemCommonETicket
instead of directly downloading the cetks with HTTPS. The cetks are also
accessible via HTTP (even though the 3DS never accesses them like that).

**Other Ticket files/folders**

Apart from ticket.db, there is a 4B000h-byte file called 3ds:\tmp\title.tik
containing the following stuff:

```

| 0000h 350h Ticket (without 8-byte header) ;-350h-byte ticket
 0350h 9Ch Unknown (00h-filled) ;\ ;\
 03ECh 4 Unknown (FFh,FFh,E3h,E7h) ; 128h-byte whatever ; fifty
 03F0h 88h Unknown (00h-filled) ;/ ; 128h
 0478h 9Ch Unknown (00h-filled) ;\ ; byte
 0514h 4 Unknown (FFh,FFh,E3h,E7h) ; 128h-byte as above ; blks
 0518h 88h Unknown (00h-filled) ;/ ;
 ... 48*128h ... ;-128h-byte's as above;/
 3D20h 0B8h Unknown (00h-filled) ;\
 3DD8h 4 Unknown (52h,46h,00h,00h) ; whatever
 3DDCh 4 Unknown (58h,A8h,07h,00h) ;
 3DE0h 4 Unknown (B4h,32h,15h,00h) ;/
 3DE4h 4721Ch Unknown (00h-filled) ;-lots of zeroes |
| --- |

Additionally, there are two empty ticket folders:

```

| 3ds:\ticket\ ;on 3DS partition
 dsi:\ticket\ ;on DSi partition |
| --- |

The latter would have been used on DSi to store .tik files (but the 3DS does
instead store all tickets in ticket.db, including DSiware tickets).