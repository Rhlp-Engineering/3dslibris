# Cryptoaesregisters

> Source: https://problemkaputt.de/gbatek.htm
> Section: Cryptoaesregisters

3DS Crypto - AES Registers 
| |
| --- |

 **AES Registers (ARM9)**
 
```

| Address Name Byte order Word order
 10009000h 4 R/W AES_CNT Little - ;-bit22-29 unlike DSi
 10009004h 4 W AES_BLKCNT Little -
 10009008h 4 W AES_WRFIFO WordWrite FifoWrite
 1000900Ch 4 R AES_RDFIFO WordRead FifoRead
 10009010h 1 R/W AES_KEYSEL - - ;\new, unlike DSi
 10009011h 1 R/W AES_KEYCNT - - ;/
 10009020h 16 W AES_IV WordWrite Little
 10009030h 16 W AES_MAC WordWrite Little
 10009040h 48 W AES_KEY0 WordWrite Little
 10009070h 48 W AES_KEY1 WordWrite Little
 100090A0h 48 W AES_KEY2 WordWrite Little
 100090D0h 48 W AES_KEY3 WordWrite Little
 10009100h 4 W AES_KEYFIFO WordWrite FifoWrite ;\
 10009104h 4 W AES_KEYXFIFO WordWrite FifoWrite ; new, unlike DSi
 10009108h 4 W AES_KEYYFIFO WordWrite FifoWrite ;/ |
| --- |

Most of these registers are same as on DSi:

DSi AES I/O Ports

New features are more keyslots, CBC/ECB modes, optional big-endian mode, and a
new Key X/Y scrambler.

**10009000h - ARM9 - AES_CNT (R/W)**

```

| 0-4 Write FIFO Count (00h..10h words) (00h=Empty, 10h=Full) (R)
 5-9 Read FIFO Count (00h..10h words) (00h=Empty, 10h=Full) (R)
 10 Write FIFO Flush (0=No change, 1=Flush) (N/A or W)
 11 Read FIFO Flush (0=No change, 1=Flush) (N/A or W)
 12-13 Write FIFO DMA Size (0..3 = 16,12,8,4 words) (2=Normal=8) (R or R/W)
 14-15 Read FIFO DMA Size (0..3 = 4,8,12,16 words) (1=Normal=8) (R or R/W)
 16-18 CCM MAC Size, max(4,(N*2+2)) bytes, usually 7=16 bytes (R or R/W)
 19 CCM Pass Associated Data to RDFIFO (0=No/Normal, 1=Yes) (R or R/W)
 20 CCM MAC Verify Source (0=From AES_WRFIFO, 1=From AES_MAC) (R or R/W)
 21 CCM MAC Verify Result (0=Invalid/Busy, 1=Verified/Okay) (R)
 Below bits (bit22-29) are other than DSi
 22 Byte order for Word Read (0=Little endian, 1=Big endian) (R? or R/W)
 23 Byte order for Word Write (0=Little endian, 1=Big endian) (R? or R/W)
 24 Word order per Fifo Read (0=Little first, 1=Big first) (R? or R/W)
 25 Word order per Fifo Write (0=Little first, 1=Big first) (R? or R/W)
 26 Key Select (0=No change, 1=Apply key selected in AES_KEYSEL) (W)
 27-29 Mode (0=CCM decrypt, 1=CCM encrypt, 2=CTR, 3=CTR,
 4=CBC decrypt, 5=CBC encrypt, 6=ECB decrypt, 7=ECB encrypt)
 30 Interrupt Enable (0=Disable, 1=Enable IRQ on Transfer End) (R or R/W)
 31 Start/Enable (0=Disable/Ready, 1=Enable/Busy) (R/W) |
| --- |

**10009010h - ARM9 - AES_KEYSEL (R/W)**

```

| 0-5 Keyslot for encrypt/decrypt, apply via AES_CNT.bit26 (00h..3Fh) (R/W)
 6-7 Unused (0) |
| --- |

**10009011h - ARM9 - AES_KEYCNT (R/W)**

```

| 0-5 Keyslot for writes via AES_KEYxyFIFO (04h..3Fh, or 0..3=None) (R/W)
 6 Key X/Y Scrambler for key 4-3Fh (0=3DS, 1=DSi) (R/W)
 7 Flush AES_KEYxyFIFO (0=No change, 1=Flush) (W) |
| --- |

"The hardware key generator is triggered by writing the keyY, which is the only
way to trigger it with the 3DS keyslots. The algorithm for generating the
normal-key from keyX and keyY is as follows":

```

| KeyDSi = (((KeyX) XOR KeyY) + FFFEFB4E295902582A680F5F1A4F3E79h) ROL 42
 Key3DS = (((KeyX ROL 2) XOR KeyY) + 1FF9E9AAC5FE0408024591DC5D52768Ah) ROL 87 |
| --- |

Key 0-3 are writeable only via the old DSi-style AES_KEY0..3 registers (not via
AES_KEYxyFIFO), and key0-3 are always using the old DSi-style Key scrambler.
Key 0-3 can be used in 3DS and DSi mode, but they are reset to zero on
3DS-to-DSi mode switch, and thus need to be (re-)initialized in DSi mode.

**10009100h - ARM9 - AES_KEYFIFO - Normal Key (four words)**

**10009104h - ARM9 - AES_KEYXFIFO - Key X (four words)**

**10009108h - ARM9 - AES_KEYYFIFO - Key Y (four words)**

Whatever, new (unlike DSi).

The Key FIFO's are automatically flushed after each 4 words, and after setting
AES_KEYCNT.bit7, and reportedly also when changing the AES_CNT word order.

```

| __________________________ Notes and DSi Registers __________________________ |
| --- |

**Notes**

When AES_CNT.bit31 is set, then AES_CNT essentially becomes locked and doesn't
change when written to. However if AES_CNT.bit26 is "set", keyslot-selection is
cued to be handled when AES_CNT.bit31 is cleared.

Clearing AES_CNT.bit31 while the AES engine is doing crypto will result in the
AES engine stopping crypto, once it finishes processing the current block.

**10009004h - ARM9 - AES_MACEXTRABLKCNT**

(CCM-MAC extra data length)>>4, ie. the number of block of CCM-MAC extra
data.

**10009006h - ARM9 - AES_BLKCNT**

(Data length)>>4, ie. the number of blocks to process

**10009008h - ARM9 - AES_WRFIFO**

**1000900Ch - ARM9 - AES_RDFIFO**

Reading from AES_RDFIFO when there's no data available in the RDFIFO will
result in reading the last word that was in the RDFIFO.

When triggering either RDFIFO or WRFIFO to be flushed, the AES Engine does not
clear either buffer.

Word order and endianness can be changed between each read/write to these
FIFOs. However changing the word order when writing to WRFIFO can cause the
word to be written outside the current block, leaving uninitialized data in its
place. Attempts to change endianness or word order are not honored when reading
from RDFIFO when no more data is available.

**10009020h - ARM9 - AES_IV (16 bytes)**

This register specifies the counter (CTR mode), nonce (CCM mode) or the
initialization vector (CBC mode) depending on the mode of operation. For CBC
and CTR mode this register takes up the full 16 bytes, but for CCM mode the
nonce is only the first 12 bytes. The AES engine will automatically increment
the counter up to the maximum BLKCNT, after which point it must be manually
incremented and set again.

**10009030h - ARM9 - AES_MAC (16 bytes)**

This register specifies the message authentication code (MAC) for use in CCM
mode.

**10009040h/10009070h/100090A0h/100090D0h - AES_KEY0/1/2/3**

These registers are the same as they were on TWL, and are likely preserved for
compatibility reasons. The keyslot is updated immediately after *any*
data(u8/u32/...) is written here, which was used on DSi to break the
key-generator.

**CCM mode pitfall**

Non-standard AES-CCM behaviour is observed on Wrap/Unwrap function. According
to RFC 3610, the first block B_0 for authentication should be generated from
the message length and some other parameters. Using these function, it seems
that the message length is aligned up to 16 when generating B_0. This makes the
generated MAC not compliant with the standard when (inputsize-noncesize)%16!=0.
It is very likely that this non-standard behaviour happens on the hardware
level, but not confirmed yet.

```

| uh, or does it just OMIT that, as on DSi?
 instead of that "aligned" thing? |
| --- |