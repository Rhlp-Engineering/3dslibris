# Filesncchencryption

> Source: https://problemkaputt.de/gbatek.htm
> Section: Filesncchencryption

3DS Files - NCCH Encryption 
| |
| --- |

 The extended header, the ExeFS, and the RomFS are encrypted using AES-CTR.
 
 **AES-CTR Encryption FirstKeyX and FirstKeyY:**
 The FirstKey is used for NCCH Exheader, and ExeFS Header, and for two ExeFS files ("icon" and "banner").
 
```

| FirstKeyX = KeyX from keyslot 2Ch (as set by bootrom)
 FirstKeyY = NCCH[000h..00Fh] ;first 10h bytes of RSA signature |
| --- |

**AES-CTR Encryption SecondKeyX and SecondKeyY:**

The SecondKey is used for the whole RomFS (header, all files, and SHA256's).
And, for all ExeFS files (EXCEPT "icon" and "banner"), ie. it's used for
".code", ".firm", "logo", and any further ExeFS files.

```

| if NCCH[18Bh]=00h then SecondKeyX = KeyX from keyslot 2Ch (original firmware)
 if NCCH[18Bh]=01h then SecondKeyX = KeyX from keyslot 25h (firm7.0.0)
 if NCCH[18Bh]=0Ah then SecondKeyX = KeyX from keyslot 18h (firm9.3.0) ;\New
 if NCCH[18Bh]=0Bh then SecondKeyX = KeyX from keyslot 1Bh (firm9.6.0) ;/3DS
 if NCCH[18Fh].bit5=0 then SecondKeyY = NCCH[000h..00Fh] (original firmware)
 if NCCH[18Fh].bit5=1 then SecondKeyY = see SEEDDB below (firm9.6.0) |
| --- |

With the original firmware, the SecondKey is just same as FirstKey (ie. there's
only one key in that case). Newer firmwares have added the extra keys shown
above, but they aren't used for ALL titles (eg. a new 3DS may use keyslot 25h
for eShop, and keyslot 2Ch for System Settings).

**SecondKeyY from SEEDDB (firm9.6.0) (New3DS)**

```

| ;load SEEDDB (nand:/data/<ID0>/sysdata/0001000f/00000000),
 ;that file contains 32byte entries (with 8-byte Title ID, 16-byte seed,
 ;plus 8-byte reserved; whereof titleid might refer to ProgramID or so?),
 ;find the matching entry for current "titleid", then do:
 temp[00h..0Fh] = seed for current title (from SEEDDB file) ;\
 temp[10h..17h] = NCCH[118h..11Fh] ;Program ID ; test
 sha256(dst=temp, src=temp, srclen=18h) ;
 if temp[00h..03h] <> NCCH[114h..117h] then error ;/
 temp[00h..0Fh] = NCCH[000h..00Fh] ;first 10h bytes of RSA signature ;\
 temp[10h..1Fh] = seed for current title (from SEEDDB file) ; key
 sha256(dst=temp, src=temp, srclen=20h) ;
 SecondKeyY=temp[00h..0Fh] ;use first 10h bytes of SHA256 as KeyY ;/ |
| --- |

**Special Cases**

```

| if NCCH[18Fh].bit2=1 ;\NoCrypto flag (eg. in NCCH's
 Exit ;not encrypted ;/located inside of FIRM files)
 if NCCH[118h..11Fh]=NCCH[400h..407h] ;\
 SkipDecrypt (or NeedEncrypt) ; Program ID in Header vs Exheader
 ;above can be checked only if Exheader ; currently/already decrypted
 ;does exist, ie. if NCCH[180h]<>0 ;/
 if NCCH[18Fh].bit0=1 ;FixedCryptoKey flag ;\
 if (ProgramID.MSW AND FFFFC010h)=00040010h ; debug only, not retail:
 NormalKey=FixedSystemKey (52h,7Ch,E6h,30h,..) ; instead of First/Second
 else ; KeyX+KeyY
 NormalKey=Zerofilled (00h's) ;/ |
| --- |

**Encryption IV**

```

| if NCCH[112h]=01h ;NCCH version (weird, is/was that version really used?)
 Force MediaUnitSize=1 (or 200h?) ;fixed? (instead of 200h SHL N bytes?)
 IV[00h..07h] = NCCH[108h..10Fh] ;NCCH Partition ID (forwards!)
 IV[08h..0Bh] = Zero
 IV[0Ch..0Fh] = BigEndianAddr (200h=Exheader, [1A0h]=ExeFS, [1B0h]=RomFS)
 if NCCH[112h]=02h (or =00h, too?) ;NCCH version
 IV[00h..07h] = NCCH[10Fh..108h] ;NCCH Partition ID (backwards!)
 IV[08h] = Type (01h=Exheader, 2=ExeFS, 3=RomFS)
 IV[09h..0Fh] = Zero (plus offset/10h within Exheader/ExeFS/RomFS area) |
| --- |

When starting elsewhere than BEGIN of Exheader/ExeFS/RomFS, add offset/10h to
the IV value (whereas, mind that IV is big-endian in the above description; it
may be easier to set the IV in little-endian mode though).