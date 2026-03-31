# Consoleids

> Source: https://problemkaputt.de/gbatek.htm
> Section: Consoleids

3DS Console IDs 
| |
| --- |

 **Hardware IDs**
 The main Console ID is stored in OTP memory:
 3DS Crypto - PRNG and OTP Registers
 The eMMC CID is used for generating console specific encryption keys. The SD card CID is also used for generating a SD card specific <ID1> folder name.
 
 **3ds:\rw\sys\LocalFriendCodeSeed_B (or Seed_A, if it exists) (110h bytes)**
 
```

| 000h 100h RSA-2048 signature across following 10h-bytes ;\
 100h 1 Zero ; same values are
 101h 1 Devkit (00h=Retail, 01h=Devkit) ; also stored in
 102h 6 Zero ; movable.sed
 108h 8 Decrypted OTP[08h..0Fh] ;/ |
| --- |

This can be also retrieved via "PSPXI:GetLocalFriendCodeSeed".

**3ds:\private\movable.sed (120h or 140h bytes)**

```

| 000h 4 ID "SEED"
 004h 1 Zero
 005h 1 Zero (or 01h if extra bytes at [120h..13Fh] are appended)
 006h 2 Zero
 008h 100h RSA-2048 signature across following 10h-bytes ;\same values are
 108h 1 Zero ; also stored in
 109h 1 Devkit (00h=Retail, 01h=Devkit) ; LocalFriendCode
 10Ah 6 Zero ; Seed_B (or _A)
 110h 8 Decrypted OTP[08h..0Fh] ;\used as AES ;/
 118h 4 Decrypted OTP[10h..13h]+Offset ; KeyY for 3
 11Ch 4 Decrypted OTP[14h..17h] ;/keyslots
 The original movable.sed from the factory is only 120h-bytes.
 Below extra data is written to the file when doing a System Format.
 120h 4 Offset (added to above Decrypted OTP[10h..13h] entry)
 124h 0Ch Zero
 130h 10h AES-MAC (NAND dbs keyslot) across SHA256 across bytes [000h..12Fh] |
| --- |

Bytes [110h..11Fh] are used as KeyY for AES keyslots 30h, 34h, and 3Ah:

```

| Keyslot 30h AES-CMACs for non-DSiWare in sd:\Nintendo 3DS\ and 3ds:\data\
 Keyslot 34h AES encryption in sd:\Nintendo 3DS\
 Keyslot 3Ah AES-CMACs for DSiWare in sd:\Nintendo 3DS\ |
| --- |

Bytes [110h..11Fh] are also used to derive the <ID0> folder name (see
below).

Movable.sed is transferred to the destination 3DS during a System Transfer. The
movable.sed keyY high u64 is updated on the source 3DS during a System
Transfer, and when doing a system format with System Settings.

**3ds:\data\<ID0>\ (<ID0> folder on 3DS partition)**

**sd:\Nintendo 3DS\<ID0>\<ID1>\ (<ID0>\<ID1> folder on SD card)**

The <ID0> folder name is derived by computing the SHA256 across
moveable.sed bytes [110h..11Fh], and then converting the first 4 words of the
SHA256 into a lowercase ASCII string (with the byte order reversed in each of
the four words, and with the last 4 words left unused).

The <ID1> folder name is derived by swapping around the byte order of the
SD card CID: The CID is rotated 8-bits to the left (uh, that means the ending
00h padding byte is moved to begin?). Then, the word-order is reversed. And
then, the halfwords are converted into ascii string (with byte-order for each
byte-pair reversed), or so?

**3ds:\rw\sys\SecureInfo_A (111h bytes) (Region and Serial/Barcode)**

```

| 000h 100h RSA-2048 signature across following 11h-bytes
 100h 1 Region (0=JPN, 1=USA, 2=EUR, 3=Reserved, 4=CHN, 5=KOR, 6=TWN)
 101h 1 Normally zero
 102h 0Fh Serial/Barcode, without ending check digit (ASCII, zeropadded) |
| --- |

The serial/barcode consists of 2-3 letters, followed by 8 digits, followed by a
checksum digit.

The first letter indicates the console model:

```

| 3DS C (or E for devunits)
 3DS XL/LL S (or R for devunits)
 2DS A (or P for devunits)
 New 3DS Y (or Yxx00 for devunits)
 New 3DS XL/LL Q (or Qxx00 for devunits)
 New 2DS XL/LL N (or Nxx01 for devunits) |
| --- |

The next 1-2 letter(s) indicate the region:

```

| JPN Japan JF,JH,JM
 USA North America W
 USA Middle East, Southeast Asia S
 EUR Europe EF,EH,EM
 EUR Australia AG,AH
 CHN China (iQue) CF,CH,CM
 KOR South Korea KF,KH,KM
 TWN Taiwan ...unknown? |
| --- |

The checksum digit is found on the consoles barcode sticker, but isn't included
in the SecureInfo_A file. However, the checksum can be calculated across
1st-8th digit (and ignoring the leading letters):

```

| 9th = (250 - (1st+3rd+5th+7th) - 3*(2nd+4th+6th+8th)) mod 10 |
| --- |

The RSA private key for retail is unknown. The private key for devkits is
included in the devkit system updater package.