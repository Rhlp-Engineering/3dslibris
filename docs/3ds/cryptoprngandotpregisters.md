# Cryptoprngandotpregisters

> Source: https://problemkaputt.de/gbatek.htm
> Section: Cryptoprngandotpregisters

3DS Crypto - PRNG and OTP Registers 
| |
| --- |

 
```

| _______________________________ PRNG Registers _______________________________ |
| --- |

**PRNG Registers ... Pseudo Random Generator? Boot9, Process9**

```

| 10011000h 4 Random (changes on each read)
 10011010h 4 Random (changes on each read)
 10011020h 4 Random (constant) |
| --- |

Used as entropy-source for seeding random number generators.

```

| _______________________________ OTP Registers ________________________________ |
| --- |

**OTP Registers**

```

| 10012000h 100h OTP_ENCRYPTED - Encrypted 3DS OTP (R)
 10012100h 8 OTP_TWLMODE - DSi OTP Console ID (R/W) |
| --- |

Access to this region is disabled once the ARM9 writes 02h to CFG9_SYSPROT9.

**10012000h - 100h bytes - OTP_ENCRYPTED - Encrypted 3DS OTP (R)**

Console-unique data encrypted with AES-CBC. The normalkey and IV are stored in
Boot9 (retail/devunit have seperate normalkey+IV for this).

**10012100h - 8 bytes - OTP_TWLMODE - DSi OTP Console ID (R/W)**

Before writing CFG9_SYSPROT9 bit1, the ARM9 copies the 8-byte TWL Console ID
here. This sets the registers at 4004D00h for ARM7.

```

| [10012100h] = DecryptedOTP[008h] xor B358A6AFh or 80000000h
 [10012104h] = DecryptedOTP[00Ch] xor 08C267B7h |
| --- |

**Decrypted OTP**

```

| 000h 90h Copied into ITCM. The encrypted version of this is what
 New3DS-arm9loader hashes for key-generation.
 000h 4 Always DEADB00Fh
 004h 4 DeviceId (aka Console ID, used in tickets)
 008h 10h Fall-back keyY used for movable.sed keyY when
 movable.sed doesn't exist in NAND (the last(???) two words
 here are used on retail for generating console-unique TWL
 keydata/etc).
 This is also used for "LocalFriendCodeSeed", etc.
 018h 1 CTCert byte order? (usually 05h)
 019h 1 CTCert issuer type:
 zero = retail ("Nintendo CA - G3_NintendoCTR2prod")
 non-zero = dev ("Nintendo CA - G3_NintendoCTR2dev")
 01Ah 6 Manufacturing date (year-1900, month, day, hour, minute, second)
 020h 4 CTCert ECDSA exponent (byte-swapped when OTP[018h]>=5)
 024h 2 Zero?
 026h 1Eh CTCert ECDSA private key
 044h 3Ch CTCert ECDSA signature
 080h 10h Zerofilled
 090h 1Ch Seed 1 for AES keys ;<-- used for console-unique AES keys
 0ACh 1Ch Seed 2 for AES keys ;\
 0C8h 1Ch Seed 3 for AES keys ; not actually used ;\overlaps SHA256
 0E4h 1Ch Seed 4 for AES keys ;/ ;/(see below)
 0E0h 20h SHA256 hash across [000h..0DFh] |
| --- |

**OTP Blurp**

After decryption, the first 90h-bytes of plaintext are copied to ITCM+3800h at
01FFB800h aka 07FFB800h if hash verification passes.

On FIRM versions prior to 3.0.0-X, this region was left unprotected. On
versions since 3.0.0-X, this has been fixed, and the region disable is now done
by Kernel9 after doing console-unique TWL keyinit, by setting bit1 of
CFG9_SYSPROT9. However, with the New3DS FIRM ARM9 binary this is now done in
the FIRM ARM9 binary loader, which also uses the 10012000h region for New 3DS
key generation.

On development units (UNITINFO != 0) ARM9 uses the first 8-bytes from 10012000h
for the TWL Console ID. This region doesn't seem to be used by NATIVE_FIRM on
retail at all, besides New3DS key-generation in the ARM9-loader.