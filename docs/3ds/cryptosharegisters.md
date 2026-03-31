# Cryptosharegisters

> Source: https://problemkaputt.de/gbatek.htm
> Section: Cryptosharegisters

3DS Crypto - SHA Registers 
| |
| --- |

 **SHA Registers**
 
```

| 1000A000h 4 ARM9 SHA_CNT ;\
 1000A004h 4 ARM9 SHA_BLKCNT ; for ARM9
 1000A040h 20h ARM9 SHA_HASH ;
 1000A080h 40h ARM9 SHA_FIFO ;/
 10101000h 4 ARM11/ARM9 SHA_CNT ;\for ARM11 (some registers
 10101004h 4 ARM11/ARM9 SHA_BLKCNT ; can be also accessed by ARM9,
 10101040h 20h ARM11/ARM9 SHA_HASH ; but FIFO and DMA DRQs are
 10301000h 40h ARM11 SHA_FIFO ;/working for ARM11 only) |
| --- |

**1000A000h/10101000h - SHA_CNT - SHA Control (R/W)**

```

| 0 Read: IN_FIFO full (0=No/ready, 1=Full/Busy) (10h words) (R)
 Write: First round (0=No change, 1=Reset BLKCNT and HASH) (W)
 1 Final round (0=No/ready, 1=Enable/Busy) (R/W)
 2 IN_FIFO DMA Enable (0=Disable, 1=Enable CDMA DRQ 0Bh) (R/W)
 3 Byte order of Result (0=Little endian, 1=Big endian/Standard) (R/W)
 4-5 Mode (0=SHA256, 1=SHA224, 2=3=SHA1) (R/W)
 6-7 Unused (0) ;reportedly "?" (but actually: always 0) (?)
 8 OUT_FIFO Enable (0=No, 1=Readback Mode) ;\optional (R/W)
 9 OUT_FIFO Status (0=Empty, 1=Non-empty) ; readback (R)
 10 OUT_FIFO DMA Enable (0=Disable, 1=Enable CDMA DRQ 0Ch);/ (R/W)
 11-15 Unused (0) (-)
 16-17 Unused (0) ;reportedly "?" (but actually: always 0) (?)
 18-31 Unused (0) (-) |
| --- |

The optional readback mode allows to readback each 40h-byte block from the
FIFO, this can reduce memory reads (and temporary memory writes), for example:

```

| EMMC --> SHA --> AES --> Memory ;saves 1xMemWrite and 2xMemRead
 EMMC --> AES --> SHA --> Memory ;saves 1xMemRead |
| --- |

The SHA_IN writing speed is about 62Mbyte/s for SHA256 and SHA224, and
50Mbyte/s for SHA1. Reading source data, and optional forwarding to SHA_OUT may
cause extra slowdown.

**1000A004h/10101004h - SHA_BLKCNT - SHA Input Length (R/W)**

```

| 0-31 Length in bytes (0..FFFFFFFFh) |
| --- |

The length is automatically updated by hardware:

```

| Length is reset to zero when setting SHA_CNT.bit0 (start).
 Length increments by 40h after each 40h-byte FIFO block.
 Length increments by remaining FIFO size after setting SHA_CNT.bit1 (final). |
| --- |

The hardware does automatically append the length value (and some padding bits)
to the data stream before computing the final result.

**1000A040h/10101040h - SHA_HASH - State/Result (20h bytes) (R/W)**

Contains the SHA state/result. The word order is fixed, the byte order (per
32bit word) depends on SHA_CNT.bit3.

Before reading the result, wait for SHA_CNT bit0 and bit1 to be BOTH zero (when
testing only bit1, the result tends to contain "XX234567...", where "XX" is
correct first byte of result, and "234567..." are still containing the initial
values).

Setting SHA_CNT.bit0 does automatically apply the following initial values:

```

| SHA256 6A09E667,BB67AE85,3C6EF372,A54FF53A,510E527F,9B05688C,1F83D9AB,5BE0CD19
 SHA224 C1059ED8,367CD507,3070DD17,F70E5939,FFC00B31,68581511,64F98FA7,BEFA4FA4
 SHA1 67452301,EFCDAB89,98BADCFE,10325476,C3D2E1F0,0 ,0 ,0 |
| --- |

The values are updated after each 40h-byte FIFO block, and updated once more
after final round.

SHA1 leaves the last 3 words unused (set to zero). SHA224 and SHA256 do
internally use all 8 words (but the last word is usually omitted when reading
the SHA224 result).

**1000A080h/10301000h - SHA_FIFO (40h bytes) - SHA_IN (W) and SHA_OUT (R)**

```

| 0-7 1st byte ;\
 8-15 2nd byte ; data to be checksummed
 16-23 3rd byte ;
 24-31 4th byte ;/ |
| --- |

The FIFO is mapped to a 40h-byte area at FIFO+0..3Fh. The word address is don't
care (one can write all words to FIFO+0, or to FIFO+0,4,8,..,3Ch).

However, 8bit/16bit writes do REQUIRE the lower two address bits to match up
with the number of previously written bytes (eg. byte writes must go to
FIFO+0,1,2,3,4,5,..,3Fh or FIFO+0,1,2,3,0,1,.,3). Writing 8bit/16bit is
normally needed only for the last block before setting final flag (and only if
the length isn't a multiple of 40h). Writing 8bit/16bit may be also needed if
the data comes from an odd source address (but that would slowdown everything).

**Invalid Operations with/without Data Abort**

```

| Reading FIFO when CNT.bit8=0 returns ZERO (readback disabled)
 Reading FIFO when CNT.bit8=1 and FIFO empty causes Data Abort (enabled+empty) |
| --- |

Untested...

```

| Writing FIFO when FIFO full... is ignored? or data abort?
 Writing 32bit to FIFO content is odd (not N*4 bytes)... causes what?
 Writing 32bit to FIFO already contains 3Dh..3Fh bytes... causes what? |
| --- |

**SHA256-HMAC and SHA1-HMAC keys**

Apart from raw SHA checksums, the 3DS does use some SHA-HMACs (that are
scrambled with 'secret' keys):

```

| - The HWCAL header contains a SHA256-HMAC (see HWCAL chapter)
 - FIRM Launch Parameters contains a "SHA1-HMAC across banner for TWL/NTR
 titles" (unknown what HMAC key is used there, and what kind of banner (or
 icon?) that refers to).
 - The NCCH "logo" is a DARC file with appended SHA256-HMAC (unknown what
 HMAC key is used there)
 - The "CVer" Archive contains a "masterkey.bin" file that contains an
 encrypted HMAC for resetting Parental Controls (unknown how to decrypt it). |
| --- |