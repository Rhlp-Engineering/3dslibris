# Cryptoaeskeyslots

> Source: https://problemkaputt.de/gbatek.htm
> Section: Cryptoaeskeyslots

3DS Crypto - AES Keyslots 
| |
| --- |

 **AES Keyslot Usage**
 
```

| 00h DSi ;\
 01h DSi ; DSi keys (little-endian,
 02h DSi ; with DSi key X/Y)
 03h eMMC key for DSi MBR and FAT filesystems ;/
 04h eMMC key for Old3DS MBR and FAT filesystem
 05h eMMC key for New3DS MBR and FAT filesystem (keyY from FIRM)
 06h eMMC key for FIRM partitions
 07h eMMC key for GBA savegame partition
 08h-09h Unused
 0Ah DSiWare export (encrypted 10h-byte zeroes in DSiWare_Exports header)
 0Bh AES-CMAC key (for emmc:\dbs images, and emmc:\private\movable.sed)
 0Ch Unused
 0Dh SSL-certificate key (see ClCertA)
 0Eh-10h Unused
 11h Temporary keyslot (used by FIRM)
 12h-13h Unused
 14h Unknown (initialized by v5.0.0-11 and up)
 15h New3DS arm9 FIRM binary loader
 16h New3DS arm9 FIRM binary loader (v9.5.0 and up)
 17h Unused
 18h New3DS Second NCCH key (when NCCH[18Bh]=0Ah)
 19h New3DS Gamecard savedata AES-CMAC key (when [ROM?-]NCSD=what?)
 1Ah New3DS Gamecard savedata Actual key (when [ROM?-]NCSD=what?)
 1Bh New3DS Second NCCH key (when NCCH[18Bh]=0Bh)
 1Ch-1Fh New3DS Unused keys (initialized, but not used yet)
 20h-23h Unused
 24h GBA savegame AES-CMAC key
 25h Second NCCH key (when NCCH[18Bh]=01h)
 26h-2Bh Unused
 2Ch First NCCH key (also 2nd key, when NCCH[18Bh]=00h)
 2Dh UDS local-WLAN CCMP key
 2Eh Streetpass key
 2Fh v6.0 save key
 30h AES-CMAC key (for emmc:\data\ and sd:\Nintendo 3DS\...)
 31h APT wrap key
 32h Unknown
 33h Gamecard savedata AES-CMAC (New3DS alternately uses keyslot 19h)
 34h SD key (for all data in "sd:\Nintendo 3DS\<ID0>\<ID1>\")
 35h Movable.sed key (movable.sed & AES-CBC MAC with import/export cmds)
 36h Used by friends module
 37h Gamecard savedata Actual key (New3DS alternately uses keyslot 1Ah)
 38h BOSS key
 39h Temporary keys (Download Play & NFC key for generating Amiibo keys)
 3Ah DSiWare export (for calculating AES-CMACs for SD DSiWare_Exports)
 3Bh CTRCARD key (for decrypting seed via AES-CCM)
 3Ch Unused
 3Dh Common key (used to decrypt title keys in Ticket)
 3Eh Unused
 3Fh Temporary keyslot (used by bootrom) |
| --- |

**Random notes**

After writing to a keyslot, the keyslot must be selected again (write
AES_KEYSEL + set AES_CNT bit26), even when writing to the same keyslot. Writing
the last word to a key FIFO immediately after selecting a keyslot will not
affect the keyslot keydata that gets used at that time, the new keydata will
not get used until the keyslot gets selected again.

Writing to the key FIFOs with byte writes results in the AES engine converting
the byte to a word (word=byte*01010101h). The result is the same regardless of
which FIFO register byte was written to.

The key FIFOs can be written simultaneously. For example, executing the
following will result in the keyX and keyY being set to all-zero (unknown for
normalkey): memset(0x10009100, 0, 0x100); uh, does that mean there are mirrors
at 1000910Ch and up?

Each key FIFO has a 10h-byte tmp-buffer for storing the words written to that
FIFO. Once the last word is written to a key FIFO, the filled tmp-buffer is
then written to the key-data for the keyslot selected by AES_KEYCNT at the time
the last word was written.

**FIRM-launch key clearing**

Starting with 9.0.0-20 the Process9 FIRM-launch code now "clears" the following
AES keyslots, with certain keydata by writing the normal-key: 0x15 and
0x18-0x20. These are the keyslots used by the New3DS FIRM arm9bin loader (minus
keyslot 0x11), the New3DS Process9 does this too.