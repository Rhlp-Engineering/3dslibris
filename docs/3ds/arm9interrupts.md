# Arm9Interrupts

> Source: https://problemkaputt.de/gbatek.htm
> Section: Arm9Interrupts

3DS ARM9 Interrupts 
| |
| --- |

 **ARM9 Interrupts (ARM9 only)**
 Mostly same as GBA/NDS/DSi, but without IME (only CPSR irq enable), and without IE2/IF2, and with changed IRQ bits:
 10001000h - IRQ_IE - ARM9 Interrupt Enable
 10001004h - IRQ_IF - ARM9 Interrupt Flags
 
```

| 0-7 NDMA 0..7 (Port 10002000h)
 8-11 TIMER 0..3 (Port 10003000h)
 12 PXI_SYNC ;aka IPC Sync (Port 10008000h)
 13 PXI_NOT_FULL ;aka IPC Send FIFO Empty ? (Port 10008000h)
 14 PXI_NOT_EMPTY ;aka IPC Recv FIFO Not Empty ? (Port 10008000h)
 15 AES (Port 10009000h)
 16 SDMMC controller (eMMC and SD/MMC slot) (Port 10006000h)
 17 SDMMC sdio.irq pin?
 18 SDxx controller? ;maybe this is SDIO wifi ? or unused 2nd SDMMC?
 19 SDxx sdio.irq pin?
 20 DEBUG_RECV ;uh? (?)
 21 DEBUG_SEND ;uh? (?)
 22 RSA (Port 1000B000h)
 23 CTRCARD0 and/or SPI_CARD ? (Port 10004000h)
 24 CTRCARD1 (Port 10005000h?)
 25 CGC Gamecard power off (CFG9_CARD_PWROFF_DELAY) (Port 10000010h)
 26 CGC_DET Gamecard insert (CFG9_CARD_INSERT_DELAY) (Port 10000010h)
 27 NTRCARD (used by ARM9, although it's ARM11/ARM9) (Port 10164000h)
 28 XDMA Event 0..4 (five events sharing one IRQ) (Port 1000C000h)
 29 XDMA Faulting (eg. CCR=0, or event>11) (Port 1000C000h)
 30-31 Unused (always 0) |
| --- |