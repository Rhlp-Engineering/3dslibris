# Dmacorelinkdmaperipheralids

> Source: https://problemkaputt.de/gbatek.htm
> Section: Dmacorelinkdmaperipheralids

3DS DMA - Corelink DMA Peripheral IDs 
| |
| --- |

 **CDMA Peripheral IDs (ARM11)**
 
```

| 00h Microphone (requires 1014010Ch.bit0=1)
 01h NTRCARD (requires 1014010Ch.bit1=1)
 02h camera (cam) Camera Bus 0 (DSi cameras) (Port 10120000h)
 03h camera (cam) Camera Bus 1 (left-eye camera) (Port 10121000h)
 04h nwm maybe wifi controller? ;\probably requires
 05h nwm maybe debug wifi? ;/1014010Ch.bit/bit5
 06h camera (y2r) Y2R_0 INPUT_Y ;\ (Port 10302000h)
 07h camera (y2r) Y2R_0 INPUT_U ; (Port 10302080h)
 08h camera (y2r) Y2R_0 INPUT_V ; Y2R (Port 10302100h)
 09h camera (y2r) Y2R_0 INPUT_YUV ; (Port 10302180h)
 0Ah camera (y2r) Y2R_0 OUTPUT_RGBA ;/ (Port 10302200h)
 0Bh fs HASH (SHA FIFO IN) ;\SHA (Port 10101000h)
 0Ch HASH (SHA FIFO OUT);/ (Port 10101000h)
 0Dh TwlBg LGYFB_0 ;\GBA (Port 10110000h)
 0Eh TwlBg LGYFB_1 ;/NDS (Port 10111000h)
 0Fh SPI_BUS0 (FIFO) ;\ (Port 10160000h)
 10h SPI_BUS1 (FIFO) ; SPIs (Port 10142000h)
 11h SPI_BUS2 (FIFO) ;/ (Port 10143000h)
 12h mvd (y2r2) Y2R_1 INPUT_Y ;\ ;\
 13h mvd (y2r2) Y2R_1 INPUT_U ; ;
 14h mvd (y2r2) Y2R_1 INPUT_V ; Y2R ; new CDMA controller
 15h mvd (y2r2) Y2R_1 INPUT_YUV ; ; in New3DS only
 16h mvd (y2r2) Y2R_1 OUTPUT_RGBA ;/ ;
 17h mvd L2B_0 INPUT ;\L2B ;
 18h mvd L2B_0 OUTPUT ;/ ;
 19h mvd L2B_1 INPUT ;\L2B ;
 1Ah mvd L2B_1 OUTPUT ;/ ;
 1Bh ;
 1Ch ;
 1Dh ;
 1Eh ;
 1Fh ;/ |
| --- |

New3DS can use both Old CDMA and NewDMA, however, the peripheral data requests
are sent only to either one (as selected in Port 10140410h).

Apart from peripherals, concerning ROM/RAM memory:

- CDMA can probably access whatever ARM11 memory (maybe whatever or so?)

- CDMA might maybe use Virtual memory addresses instead of physical addresses?

**XDMA Peripheral IDs (ARM9)**

```

| 00h Process9 "CTRCARD"(CARD0?) or SPI_CARD (FIFO) (Port 1000D000h)
 01h ? (maybe CARD1?) or SPI_CARD (FIFO) (Port 1000D000h)
 02h ?
 03h ?
 04h AES IN (WRFIFO) (Port 10009008h)
 05h AES OUT (RDFIFO) (Port 1000900Ch)
 06h unused??? SHA FIFO IN (Port 1000A000h)
 07h Process9 SHA FIFO OUT (optional readback) (Port 1000A000h) |
| --- |

Apart from peripherals, concerning ROM/RAM memory:

- XDMA can access ARM9 memory (probably Main RAM, and AXI, and maybe others?)

- XDMA reportedly cannot access the ARM9 bootrom at all