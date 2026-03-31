# I2Cregisters

> Source: https://problemkaputt.de/gbatek.htm
> Section: I2Cregisters

3DS I2C Registers 
| |
| --- |

 **I2C Registers**
 
```

| Address Width Old3DS Name
 10161000h 1 Yes I2C_BUS0_DATA ;\
 10161001h 1 Yes I2C_BUS0_CNT ; BUS 0 (old DSi devices) (1.8V)
 10161002h 2 Yes I2C_BUS0_CNTEX ;
 10161004h 2 Yes I2C_BUS0_SCL ;/
 10144000h 1 Yes I2C_BUS1_DATA ;\
 10144001h 1 Yes I2C_BUS1_CNT ; BUS 1 (extra 3DS devices) (1.8V)
 10144002h 2 Yes I2C_BUS1_CNTEX ;
 10144004h 2 Yes I2C_BUS1_SCL ;/
 10148000h 1 Yes I2C_BUS2_DATA ;\
 10148001h 1 Yes I2C_BUS2_CNT ; BUS 2 (extra 3DS gimmicks) (3.3V)
 10148002h 2 Yes I2C_BUS2_CNTEX ;
 10148004h 2 Yes I2C_BUS2_SCL ;/ |
| --- |

**10161000h - I2C_BUS0_DATA**

**10144000h - I2C_BUS1_DATA**

**10148000h - I2C_BUS2_DATA**

Unknown. Supposedly data.

**10161001h - I2C_BUS0_CNT**

**10144001h - I2C_BUS1_CNT**

**10148001h - I2C_BUS2_CNT**

```

| 0 Stop (0=No, 1=Stop/last byte)
 1 Start (0=No, 1=Start/first byte)
 2 Pause (0=Transfer Data, 1=Pause after Error, used with/after Stop)
 3 unknown/unspecified
 4 Ack Flag (0=Error, 1=Okay) (For DataRead: W, for DataWrite: R)
 5 Data Direction (0=Write, 1=Read)
 6 Interrupt Enable (0=Disable, 1=Enable)
 7 Start/busy (0=Ready, 1=Start/busy) |
| --- |

**10161002h - I2C_BUS0_CNTEX**

**10144002h - I2C_BUS1_CNTEX**

**10148002h - I2C_BUS2_CNTEX**

```

| 0 Current SCL pin state (0=Low, 1=High/idle) (R)
 1 Wait if SCL held low (0=No/fixed delay, 1=Yes, wait if SCL=low) (R/W)
 3-14 Unused (0)
 15 Unknown (BUS0: can be set, alongsides bit1 becomes read-only?) (?) |
| --- |

Bit1=0 appends a short fixed delay after each byte. Bit1=1 will automatically
wait if the SCL clock pin is held low: If the peripheral doesn't do that then
it can be even slightly faster than the fixed delay, if the peripheral does
hold SCL low then the 3DS will wait as long as needed (eg. required for
3DS/New3DS MCU and New3DS C-Stick).

**10161004h - I2C_BUS0_SCL**

**10144004h - I2C_BUS1_SCL**

**10148004h - I2C_BUS2_SCL**

```

| 0-5 Duration for SCL=Low (0..3Fh, 0=Fastest) ;default=0
 6-7 Unused (0)
 8-12 Duration for SCL=High (0..1Fh, 0=Fastest) ;default=5
 13-15 Unused (0) |
| --- |

The fastest setting (0000h) is about 380kHz (about 41Kbyte/s).

The slowest setting (1F3Fh) is about 84kHz (about 9Kbyte/s)

Hmmm, or maybe the raw bitrate is faster, but with short pause between bytes?