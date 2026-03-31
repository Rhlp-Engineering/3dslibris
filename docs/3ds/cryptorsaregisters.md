# Cryptorsaregisters

> Source: https://problemkaputt.de/gbatek.htm
> Section: Cryptorsaregisters

3DS Crypto - RSA Registers 
| |
| --- |

 **RSA Registers (ARM9)**
 
```

| 1000B000h 4 R/W RSA_CNT Control/status and keyslot select
 1000B0F0h 4 ? RSA_UNKNOWN Unknown
 1000B1x0h 4 R/W RSA_SLOTCNT_x Keyslot 0..3 control/status (x=0..3)
 1000B1x4h 4 R RSA_SLOTSIZE_x Keyslot 0..3 size/status (x=0..3)
 1000B200h 4 W RSA_EXPFIFO Exponent (10001h, or private key) ;\for
 1000B204h FCh W RSA_EXPFIFO Mirrors of above ; current
 1000B400h 100h R/W RSA_MOD Modulus (public key) ;/keyslot
 1000B800h 100h R/W RSA_DATA Incoming Data and Result |
| --- |

**1000B000h - ARM9 - RSA_CNT (R/W)**

```

| 0 Start/Busy (0=Idle/Ready, 1=Enable/Busy)
 1 IRQ Enable (0=Disable, 1=Enable, set ARM9 IF.bit22 when Ready)
 2-3 Unused (0)
 4-5 Keyslot (0..3=Key 0-3) ;for Start/Busy and RSA_MOD,EXPFIFO
 6-7 Unused (0)
 8 Byte order (0=Little endian, 1=Big Endian) ;for RSA_MOD,DATA,EXPFIFO
 9 Word order (0=Little endian, 1=Big Endian) ;for RSA_MOD,DATA
 10-31 Unused (0) |
| --- |

**1000B0F0h - ARM9 - RSA_UNKNOWN (R and/or W)**

```

| 0-28 Unknown/unused? (always zero)
 29 Unknown/readonly? (always set)
 30-31 Unknown/unused? (always zero) |
| --- |

The bootroms writes zero in RSA IRQ init, so some bits might be write-only?
Writing seems to have no effect on anything though.

**1000B100h+(0..3)*10h - ARM9 - RSA_SLOTCNT_0..3 (R/W)**

```

| 0 Read: Exponent Status (0=Bad=LessThan4orOdd, 1=Good=4orMoreAndEven) (R)
 Write: Clear RSA_SLOTSIZE/EXPFIFO (0=Clear, 1=No Change) (W)
 1 Disable RSA_EXPFIFO Writes (0=Normal, 1=DataAbort) (R/W)
 2 Disable RSA_MOD Reads (0=Normal, 1=DataAbort) (R/W)
 3-30 Unused (0)
 31 Disable RSA_SLOTCNT_x Writes (0=Normal, 1=Disable/permanent) (R/w) |
| --- |

Bit1,2 can be changed only if Status Bit0 was set (Good=4orMoreAndEven).

**1000B104h+(0..3)*10h - ARM9 - RSA_SLOTSIZE_0..3 (R)**

```

| 0-31 Number of words written to EXPFIFO (range 0..40h) |
| --- |

Indicates the EXP size (and implied: the MOD and DATA sizes). Whilst that
number is important, programmers should usually know how many words they had
used, without needing to use the SLOTSIZE register.

**1000B200h - ARM9 - RSA_EXPFIFO - Exponent; usually 10001h, or Private Key (W)**

**1000B204h..1000B2FFh - ARM9 - RSA_EXPFIFO mirrors (W)**

```

| 0-31 FIFO, in current byte-order, to be written MSW first (max 40h words) |
| --- |

The number of words written to EXPFIFO does imply the size of the MOD and DATA
values (that makes sense for private exponents, but the public exponent 10001h
must be padded with leading zeroes, and then followed by 10001h in last word
(aka 01000100h for big-endian).

The RSA hardware wants the number of words to be an even number in range of
4..40h, aka a multiple of 8 bytes in range 10h..100h (and if so, sets
SLOTCNT.bit0).

**1000B400h..1000B4FFh - ARM9 - RSA_MOD - Modulus; Public Key (R/W)**

```

| 100h-byte area, in currently selected byte/word-order, for current keyslot |
| --- |

The upper bits are unused/don't care when SLOTSIZE<40h.

**1000B800h..1000B8FFh - ARM9 - RSA_DATA (100h bytes) (R/W)**

```

| 100h-byte area, in currently selected byte/word-order |
| --- |

Contains the data that is to be encrypted/decrypted, and contains the result
after completion.

The upper bits are unused/don't care when SLOTSIZE<40h.

**RSA Timing Examples (versus 67MHz timer)**

```

| 0.9ms (E475h clks) Public key, 80h-bytes (DSi signatures)
 3.3ms (3571Dh clks) Public key, 80h-bytes zeropadded to 100h-bytes size
 3.3ms (3574Ah clks) Public key, 100h-bytes (3DS signatures)
 200ms (CE59A5h clks) Private key, 100h-bytes |
| --- |

**Invalid Operations with/without Data Abort**

```

| Reading MOD or DATA when busy (and maybe also on writing?) --> Data Abort
 Reading MOD when disabled in SLOTCNT --> Data Abort
 Writing EXPFIFO when disabled in SLOTCNT --> Data Abort
 Writing more than 40h words to EXPFIFO --> Data Abort
 Reading EXPFIFO or reading unused registers like 1000B5xxh --> Returns Zero |
| --- |

**Keyslot 0..3 usage**

During boot, the bootrom uses the following PUBLIC keys (100h byte modulus,
with exponent 10001h):

```

| Slot 0 uninitialized (unused)
 Slot 1 retail=FFFFB1E0h, debug=FFFFC4E0h (for FIRM from eMMC)
 Slot 2 retail=FFFFB2E0h, debug=FFFFC5E0h (for FIRM from Wifi-Flash/NDS-Cart)
 Slot 3 retail=FFFFB0E0h, debug=FFFFC3E0h (for NCSD from eMMC) |
| --- |

After boot, the bootrom does replace the above keys by four PRIVATE keys (with
100h byte modulus, and 100h byte exponent):

```

| Slot 0 retail=FFFFB3E0h, debug=FFFFC6E0h ;\Hardware slots (modulus+exponent)
 Slot 1 retail=FFFFB5E0h, debug=FFFFC8E0h ; (the modulus are also stored in
 Slot 2 retail=FFFFB7E0h, debug=FFFFCAE0h ; RAM at ITCM+3D00h+(0..3)*100h)
 Slot 3 retail=FFFFB9E0h, debug=FFFFCCE0h ;/
 Slot 4 retail=FFFFBBE0h, debug=FFFFCEE0h ;\
 Slot 5 retail=FFFFBDE0h, debug=FFFFD0E0h ; RAM slots (modulus+exponent are
 Slot 6 retail=FFFFBFE0h, debug=FFFFD2E0h ; stored at ITCM+4100h+(0..3)*200h)
 Slot 7 retail=FFFFC1E0h, debug=FFFFD4E0h ;/ |
| --- |

Later on, the firmware does replace some slots by other keys:

```

| Slot 0 Arbitrary (uh?)
 Slot 1 CXI access desc (following the exheader, uh?)
 Slot 2 Unused (contains the private key from bootrom)
 Slot 3 Unused (contains the private key from bootrom) |
| --- |

**RSA Basics and Differences to DSi**

The 3DS RSA hardware is more or less same as the DSi RSA BIOS functions, see
there for general info about RSA maths and RSA padding:

BIOS RSA Functions (DSi only)

Apart from being hardware-implemented, the 3DS is usually using 100h-bytes
(instead 80h), with SHA256 signatures (instead SHA1), and with OpenPGP headers
(instead raw padding).

**Blurb**

Writing to RSA_MOD does not change the exponent written with RSA_EXPFIFO. An
attack based on the Pohlig-Hellman algorithm exists to "read" the contents of
RSA_EXPFIFO as a result (see 3DS System Flaws).

**RSA Overview**

The RSA module is essentially a hardware-accelerated modular exponentiation
engine. It is specially optimized for RSA applications, so its behavior can be
incoherent when RSA's invariants are broken.

The PKCS (Public-Key Cryptography Standards) message padding must be manually
checked by software, as hardware will only do raw RSA operations.

**Observed edge cases**

```

| "if 2 divides mod, output == 0" |
| --- |

uh, how to "divide" a "mod" ?

uh, for mod2, remainder SHOULD be 0 or 1 (or is it ALWAYS 0 here?)

uh, also, MOD may be required to be bigger than DATA?

**3DS RSA Usage**

```

| NCSD Header for gamecard (key in ITCM)
 NCSD Header for booting FIRM from eMMC (key in "Process9" (and bootrom?))
 NCSD Header for booting FIRM from WifiFlash or NtrCard
 FIRM Header
 NCCH Header CFAs with fixed key
 NCCH Header CXIs with key in Exheader
 NCCH Exheader with key from bootrom
 DSP1 Header
 LocalFriendCodeSeed_B Header
 movable.sed Header
 SecureInfo_A Header
 TMD has RSA certs
 Ticket has RSA certs
 certs.db has RSA certs
 CIA has RSA certs
 CRR0 has RSA
 Savedata FLASH uses a "keyY is generated via the RSA engine"
 .pem files contain RSA keys
 bootrom contains RSA keys |
| --- |

**ECC/ECDSA Keys (also uses private/public keys, similar to RSA)**

```

| 3DS Tickets can have a ECC Public Key
 DSiware Exports are using ECDSA
 OTP contains a "CTCert" with ECDSA
 Apart from OTP (?) there is reportedly also a "CTCert" file, and .ctx files? |
| --- |