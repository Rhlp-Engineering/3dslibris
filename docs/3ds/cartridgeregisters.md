# #decrypted from NCSD[1000h..103Bh] header

> Source: https://problemkaputt.de/gbatek.htm
> Section: #decrypted from NCSD[1000h..103Bh] header

3DS Cartridge Registers 
| |
| --- |

 **Gamecard related CONFIG9 Registers**
 
```

| 1000000Ch 2 CFG9_CARD_CTL NTRCARD/CTRCARD/SPI controller select
 10000010h 1 CFG9_CARD_POWER Power On/Off/Reset and Eject detect
 10000012h 2 CFG9_CARD_INSERT_DELAY Cart insert detection delay
 10000014h 2 CFG9_CARD_PWROFF_DELAY Cart power off delay |
| --- |

3DS Config - CONFIG9 Registers

```

| ___________________________ 3DS Cartridge Protocol ___________________________ |
| --- |

Cartridge detection is done using unencrypted 8-byte NTRCARD commands, 3DS
cartridges have ChipID.bit28=1, if so, command 3Eh is used to switch to 16-byte
CTRCARD mode.

Of these 16-byte commands, the first command/reply (82h) is unencrypted, used
to receive the header with an AES-encrypted encryption constant. The next
command/reply (83h) is encrypted using that constant, used to send a random
value. The following commands are encrypted using a combination of the original
constant and the random value.

```

| Command Data Expl.
 9F00000000000000 2000h Dummy
 71C93FE9BB0A3B18 0 Unknown/dummy (hardcoded constant)
 9000000000000000 4 Get Chip ID, response=9000FEC2
 9000000000000000 4 Get Chip ID, response=9000FEC2
 A000000000000000 4 Get Chip Type, response=00000000
 3E00000000000000 0 Enter 16-byte command mode
 82000000000000000000000000000000 200h Get header/seed (NCSD[1000h..11FFh])
 8300000000000000rrrrrrrrrrrrrrrr 0 Change Seed (to random)
 A200000000000000rrrrrrrrrrrrrrrr 4 Get Chip ID, response=9000FEC2
 A300000000000000rrrrrrrrrrrrrrrr 4 Get Chip Type, response=00000000
 C500000000000000rrrrrrrrrrrrrrrr 0 Unknown Watchdog?
 A200000000000000rrrrrrrrrrrrrrrr 4 Get Chip ID, response=9000FEC2
 A200000000000000rrrrrrrrrrrrrrrr 4 Get Chip ID, response=9000FEC2
 A200000000000000rrrrrrrrrrrrrrrr 4 Get Chip ID, response=9000FEC2
 A200000000000000rrrrrrrrrrrrrrrr 4 Get Chip ID, response=9000FEC2
 A200000000000000rrrrrrrrrrrrrrrr 4 Get Chip ID, response=9000FEC2
 BF000000000000000000000000000000 200h Read address 0 (NCSD[0..1FFh])
 BF000000000040000000000000000000 200h Read address 4000h (maybe partition?)
 C6000000000000000000000000000000 40h Get Unique ID (on-chip PROM)
 BFaaaaaaaaaaaaaa0000000000000000 200h Read address aaa...
 C5000000000000000000000000000000 0 Unknown Watchdog (each 10,000 reads)
 BFaaaaaaaaaaaaaa0000000000000000 200h Read address aaa... |
| --- |

On a lower level, the NTRCARD commands are working about as on NDS/DSi (with
hardcoded gaps between command and data). The CTRCARD commands are sending a
status byte instead of gaps (00h bytes when not ready, 01h when ready for data
transfer; the 3DS hardware does automatically wait for that status byte before
transferring data blocks). The data block is reportedly followed by a standard
CRC32 value across unencrypted data. However, cartridges do only seem to output
correct CRC's for command 82h/BFh. And, even then, the CRC doesn't actually
look like a standard CRC32, or maybe the CRC gets garbled by encryption or
status bytes?

The PROM is used for online games, it contains a 16-byte hex number (followed
by 30h-byte FFh-filled area).

Some cards do also have writeable NAND instead of ROM. The NAND is also
accessed via CTRCARD0 registers (not CTRCARD1), the NAND commands are still
unknown.

```

| __________________________ 3DS Cartridge Encryption __________________________ |
| --- |

The encryption seed is initialized as follows:

```

| CardType = (ReplyFromCommandA0h AND 00000003h)
 send CTRCARD command 82h ;-receive NCSD[1000h..11FFh]
 AES.KEYX = as set by bootrom, keyslot 3Bh ;\
 AES.KEYY = NCSD[1000h..100Fh] ; decrypt seed via AES-CCM
 AES.MAC = NCSD[1020h..102Fh] ; (use big-endian input,
 AES.IV = NCSD[1030h..103Bh] ; and little-endian output)
 if CardType=3 then AES.KEY = zerofilled ; <--dev card (keyslot 11h)
 AES.DATA.IN = NCSD[1010h..101Fh] ;
 ctrcard_seed[00h..0Fh] = AES.DATA.OUT ;/
 CTRCARD_SECSEED = ctrcard_seed ;\
 CTRCARD_SECCNT = CardType*100h+8004h ; apply fixed seed
 wait until CTRCARD_SECCNT.bit14=1 ;/
 random64bit = whatever, can be zero, or same as fixed seed, or random
 cmdRand1 = REG_PRNG[0];
 cmdRand2 = REG_PRNG[4];
 send CTRCARD command 83h + random64bit ;-send random64bit to card
 ctrcard_seed[00h..07h] = random64bit ;-change LSBs of fixed seed
 CTRCARD_SECSEED = ctrcard_seed ;\
 CTRCARD_SECCNT = CardType*100h+8004h ; apply random seed
 wait until CTRCARD_SECCNT.bit14=1 ;/
 send further CTRCARD commands... ;-get chip id, read data, etc. |
| --- |

The crypto hardware does reportedly use SNOW 2.0 and RC4 encryption. RC4 is the
standard WEP algorithm, SNOW 2.0 is a less known algorithm:

```
[http://web.archive.org/web/20060202010143/http://www.it.lth.se/cryptology/snow](http://web.archive.org/web/20060202010143/http://www.it.lth.se/cryptology/snow)
| |
| --- |

The overall encryption does reportedly work as shown below (but cardrc4keyhalf
and cardsnowiv are unknown, so it's not possible to confirm if it's really
working; and there may be few more unknown keys for other CTRCARD_SECCNT
settings).

```
[http://www.3dbrew.org/wiki/Gamecards](http://www.3dbrew.org/wiki/Gamecards)
| ##################################.py
 from snow20 import SNOW20
 from rc4 import RC4
 cardrc4keyhalf = 0x4##############################7
 cardsnowiv = 0xD##############################4
 #cardrc4keyhalf = 0x3##############################9
 #cardsnowiv = 0xD##############################4
 #cardrc4keyhalf = 0x7##############################C
 #cardsnowiv = 0x0##############################A
 def tolist(val, nbytes):
 return [val >> i & 0xff for i in reversed(range(0, nbytes*8, 8))]
 def initsnowstream(snowkey):
 snow20stream = SNOW20(snowkey, cardsnowiv)
 for _ in xrange(32): #discard 1024 bits of output, each output is 32 bits
 snow20stream.next()
 return snow20stream
 def initrc4stream(snowstream):
 tmp = snowstream.next() << 96 | snowstream.next() << 64 | \
 snowstream.next() << 32 | snowstream.next()
 key = cardrc4keyhalf << 128 | tmp
 rc4stream = RC4(tolist(key, 32))
 for _ in xrange(256): #discard the first 256 bytes of output
 rc4stream.next()
 return rc4stream
 #sample data from ...
 titlekey1 = 0x################ #decrypted from NCSD[1000h..103Bh] header
 titlekey2 = 0x################
 enccmds = [[0xF32C92D85C9D44DED3E0E41DBE7C90D9, 0x00],
 [0x696B9D8582FB55D31B68CAFE70C74A95, 0x04],
 [0xBAA4812CA0AC9C5D19399530E3ACCCAB, 0x04],
 [0x178E427C22D87ADB86387249A97D321A, 0x00],
 [0xE06019B1BD5C9130ED6A4D9F4A9E7193, 0x04],
 [0x4E0D224862523BBFE2E6255F80E15F37, 0x04],
 [0x4CDF93D319FB62D0DB632A45E3E8D84C, 0x04],
 [0x9AA5D80551002F955546D296A57F0FEF, 0x04],
 [0xC12BA81AEF30DDDBD93FAD5D544C6334, 0x04],
 [0x62EC5FB7F420AE1DC6253AE18AFA5BB3, 0x200+4], #+crc
 [0xE3FA23AA016BE0C93430D1F42FF41324, 0x200+4]] #+crc
 snow20stream = initsnowstream(titlekey1 << 64 | titlekey2)
 rc4stream = initrc4stream(snow20stream)
 for cmd, datalen in enccmds:
 print 'encrypted command: %032X' % (cmd)
 dec = reduce(lambda x, y: x<<8 | y, [rc4stream.next() ^ x \
 for x in tolist(cmd, 16)])
 print 'decrypted command: %032X' % (dec)
 if dec >> (15*8) == 0x83: #"set seed" command
 randseed = dec & 0xFFFFFFFFFFFFFFFF
 print 'changing "seed" to %016X' % (randseed)
 snow20stream = initsnowstream(titlekey1 << 64 | randseed)
 rc4stream = initrc4stream(snow20stream)
 print 'skipping %d bytes' % (datalen)
 for _ in xrange(datalen):
 rc4stream.next() #these RC4 bytes would be used to decrypt the data |
| --- |

Even without the unknown keys, cart dumping is possible using the 3DS console
hardware, there are also "sky3DS" flashcards that can run pirate copies on 3DS
consoles without needing exploits, but that's kinda useless and works only for
RSA signed official titles with region lock (the game selection is done via
buttons on the cartridge instead of file menu).

```

| _____________________________ NTRCARD Registers ______________________________ |
| --- |

**NTRCARD Registers (ARM9/ARM11)**

These registers are same as the old NDS Cartridge registers, see

DS Cartridge I/O Ports

```

| 10164000h 2 REG_NTRCARD_MCNT ;40001A0h 2 Gamecard ROM and SPI Control
 10164002h 2 REG_NTRCARD_MDATA ;40001A2h 2 Gamecard SPI Bus Data/Strobe
 10164004h 4 REG_NTRCARD_ROMCNT ;40001A4h 4 Gamecard bus timing/control
 10164008h 8 REG_NTRCARD_CMD ;40001A8h 8 Gamecard bus 8-byte command out
 10164010h 4 REG_NTRCARD_SEEDX_L ;40001B0h 4 Gamecard Encryption Seed 0 Low
 10164014h 4 REG_NTRCARD_SEEDY_L ;40001B4h 4 Gamecard Encryption Seed 1 Low
 10164018h 1 REG_NTRCARD_SEEDX_H ;40001B8h 2 Gamecard Encryption Seed 0 High
 1016401Ah 1 REG_NTRCARD_SEEDY_H ;40001BAh 2 Gamecard Encryption Seed 1 High
 1016401Ch 4 REG_NTRCARD_DATA ;4100010h 4 Gamecard bus 4-byte Data |
| --- |

These registers are usually used on ARM9 side. However, they are mapped to both
ARM11/ARM9, and do support IRQ/DMA on ARM11/ARM9 side (unlike most other
registers in that area, which support IRQ/DMA on ARM11 side only).

Note: ARM11-side CDMA 01h requires Port 1014010Ch, CFG11_CDMA_CNT.bit1=1.

```

| _____________________________ CTRCARD Registers ______________________________ |
| --- |

**CTRCARD Registers (ARM9 only)**

```

| 10004000h - CTRCARD0 - normal cartridge access
 10005000h - CTRCARD1 - unknown purpose, not used for ROM cards, nor NAND cards
 10004000h/10005000h 4 CTRCARD_CNT
 10004004h/10005004h 4 CTRCARD_BLKCNT
 10004008h/10005008h 4 CTRCARD_SECCNT ;varies for CARD0/CARD1
 1000400Ch/1000500Ch 4 CTRCARD_LOCK
 10004010h/10005010h 4 CTRCARD_SECSEED FIFO!!!!!
 10004020h/10005020h 16 CTRCARD_CMD
 10004030h/10005030h 4 CTRCARD_FIFO |
| --- |

**10004000h/10005000h - CTRCARD_CNT**

```

| 0-4 Timeout (0-16=1ms,2ms,4ms,8ms,..,64s; 17-31=64s, too; def=12=4s) (R/W)
 5 Timeout Error (0=Okay, 1=Error) (write 0 to ack) (R/ack)
 6 Timeout Enable (0=Disable, 1=Enable) (R/W)
 7 Unused (0)
 8 CRC Error (0=Okay, 1=Error) (write 0 to ack) (R/ack)
 9 CRC Enable (0=Disable, 1=Enable) (works for cmd 82h/BFh) (R/W)
 10-14 Unused (0)
 15 DMA Enable (0=Disable, 1=Enable DMA DRQs, each 8 words) (R/W)
 16-19 Data Block size (0-8=0,4,16,64,512,1K,2K,4K,8K; 9-15=8K, too) (R/W)
 20-23 Unused (0)
 24-26 Transfer Clock (0-5=67MHz div 4,5,6,8,10,16; 6-7=div16, too) (R/W)
 27 Data-Word status (0=Busy, 1=Ready/DRQ) (R)
 28 Reset Pin (0=Low/Reset, 1=High/Release) (SET-ONCE) (R/W)
 29 Transfer Direction (0=Read, 1=Write) (R/W)
 30 Interrupt Enable (0=Disable, 1=Enable) (ARM9 IF.bit23/24) (R/W)
 31 Start (0=Idle, 1=Start/Busy) (R/W) |
| --- |

With the above Transfer Clock settings, the clock (per byte) can range from
16.7MHz downto 4.2MHz.

Once reset pin is set high, it cannot be changed until controller is reset
(which can be done CFG9_CARD_POWER). Setting bit28 does work only when writing
bit31=0.

**10004004h/10005004h - CTRCARD_BLKCNT (R or R/W)**

```

| 0-14 Number of data blocks to read, minus 1 (0..7FFFh=1..8000h) (R/W)
 15 Unused (0)
 16-28 Number of data blocks to write, minus 1 (0..1FFFh=1..2000h) (R/W)
 29-31 Unused (0) |
| --- |

**10004008h/10005008h - CTRCARD_SECCNT (R or R/W)**

For CARD0 (10004008h): (R/W mask 0307h, with readonly mask 4000h)

```

| 0-1 Crypto Mode (0=Normal, 1=Unknown, 2=Ignore SEED, 3=same as 0) (R/W)
 2 Crypto Enable (0=Disable, 1=Enable) (R/W)
 3-7 Unused (0)
 8-9 Crypto Key index (0..3, from A0h command) (3=debug) (R/W)
 10-14 Unused (0)
 14 Crypto Apply Ready (0=Busy, 1=Ready) (R)
 15 Crypto Apply Seed/Key (0=No, 1=Update Seed, works only if bit2=1) (W)
 16-31 Unused (0) |
| --- |

For CARD1 (10005008h): (R/W mask 9300h, without readonly mask)

```

| 0-7 Unused (0)
 8-9 Key index (?) (R/W)
 10-11 Unused (0)
 12 unknown... ? (R/W)
 13-14 Unused (0)
 15 unknown... ? (R/W)
 16-31 Unused (0) |
| --- |

**1000400Ch/1000500Ch - CTRCARD_LOCK (R or R/W)**

```

| 0 Write-protect CNT.bit28, SECCNT.bit0-2 (0=No, 1=Lock) (SET-ONCE) (R/W)
 1-31 Unused (0) |
| --- |

**10004010h/10005010h - CTRCARD_SECSEED (W)**

```

| 0-31 Encryption Seed, 4-word FIFO (16 bytes) |
| --- |

To apply the four most recent words written to SECSEED, write SECCNT.bit15=1,
then wait for SECCNT.bit14=1.

The seed is usually updated twice: Once after command 82h (using four seed
words from the cart header), and once after command 83h (with two seed words
changed to random values).

**10004020h/10005020h - CTRCARD_CMD (W)**

```

| 0-127 Command (128bit, aka 16 bytes, little endian, transferred MSB first) |
| --- |

**10004030h/10005030h - CTRCARD_FIFO (R) (maybe also for write-direction?)**

```

| 0-31 Data (from 8 word FIFO) |
| --- |

```

| _____________________________ SPI CARD Registers _____________________________ |
| --- |

SPI FLASH cartridge savedata is accessed via SPI_CARD registers, see:

3DS SPI Registers