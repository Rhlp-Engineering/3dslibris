# Dmandmaregisters

> Source: https://problemkaputt.de/gbatek.htm
> Section: Dmandmaregisters

3DS DMA - NDMA Registers 
| |
| --- |

 **NDMA (ARM9 only)**
 10002000h ARM9
 Same as DSi, except, there seem to be 8 channels (instead of 4).
 DSi New DMA (NDMA)
 And with new bit in NDMAGCNT register:
 
```

| 0 SAD/DAD/TCNT/WCNT register read (0=Written value, 1=Current value) |
| --- |

And with new bits in NDMAxCNT registers:

```

| 0-4 New extra startup mode bits (used if startup=0Fh)
 5-9 Unknown (unused on DSi, seem to be always 0 on 3DS, too) |
| --- |

And with changed startup modes:

```

| 00h TIMER0 (Port 10003000h)
 01h TIMER1 (Port 10003004h)
 02h TIMER2 (Port 10003008h)
 03h TIMER3 (Port 1000300Ch)
 04h CTRCARD0 (maybe 10004000h)
 05h CTRCARD1 (maybe 10005000h)
 06h SDMMC controller (eMMC and SD/MMC slot) (Port 10006000h)
 07h Alternate SD/MMC Slot controller (maybe 10007000h)
 08h AES IN (WRFIFO) (Port 10009008h)
 09h AES OUT (RDFIFO) (Port 1000900Ch)
 0Ah SHA IN (WRFIFO) (Port 1000A000h)
 0Bh SHA OUT (RDFIFO) (optional readback) (Port 1000A000h)
 0Ch NTRCARD (used so by bootrom) (maybe 10164000h)
 0Dh ?
 0Eh ?
 0Fh See below (subclassed)
 10h-1Fh Start immediately (without repeat) |
| --- |

Mode 0Fh is subclassed by new bits from NDMAxCNT.bit0-4:

```

| 0Fh.00h exists, related to SPI_CARD and AES IN ;guess:
 0Fh.01h exists, related to SPI_CARD and AES IN ;maybe these also
 0Fh.02h exists, related to SPI_CARD and AES OUT ;work with CTRCARD0/1
 0Fh.03h exists, related to SPI_CARD and AES OUT
 0Fh.04h exists, related to SPI_CARD and SHA IN
 0Fh.05h exists, related to SPI_CARD and SHA IN
 0Fh.06h exists, related to SPI_CARD and SHA OUT
 0Fh.07h exists, related to SPI_CARD and SHA OUT
 0Fh.08h SDMMC DATA32 to AES IN ? ;used by bootrom
 0Fh.09h ? <-- maybe as above for 2nd SDxx controller?
 0Fh.0Ah exists related to SDMMC and AES OUT
 0Fh.0Bh ?
 0Fh.0Ch exists related to SDMMC and SHA IN ;(SHA at time of SDMMC!)
 0Fh.0Dh ?
 0Fh.0Eh exists related to SDMMC and SHA OUT
 0Fh.0Fh ?
 0Fh.10h AES OUT to SHA IN ? ;used by bootrom
 0Fh.11h exists related AES IN and SHA OUT
 0Fh.12h-1Fh ? |
| --- |