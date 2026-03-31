# Videol2Bregistersrgbtorgbaconverternew

> Source: https://problemkaputt.de/gbatek.htm
> Section: Videol2Bregistersrgbtorgbaconverternew

3DS Video L2B Registers (RGB-to-RGBA Converter) (New3DS) 
| |
| --- |

 **10130000h - New3DS - L2B_0 - First RGB-to-RGBA converter (New3DS only)**
 **10131000h - New3DS - L2B_1 - Second RGB-to-RGBA converter (New3DS only)**
 
```

| 10130000h/10131000h 4 L2B_CNT Control R/W e3c00303h
 10130004h/10131004h 2 L2B_WIDTH Width R/W 03f8h
 10130006h/10131006h 2 L2B_HEIGHT Height R/W 03f8h
 10130020h/10131020h 4 L2B_ALPHA Alpha R/W 000000ffh
 10330000h/10331000h 1000h L2B_FIFO (IN and OUT, empty=data_abort) (R+W) |
| --- |

These registers appear to be called L2B, which might be short for
Line-to-Block. They are similar to the YUV-to-RGBA converters, but merely
converting from RGB-to-RGBA (and also allowing to convert between 5bit and 8bit
color depths). And, they are always re-ordering the data from scanline format
to swizzled texture format:

3DS Video Texture Swizzling

**10130000h/10131000h - New3DS - L2B_CNT (R/W)**

```

| 0-1 Input RGBx Format (0=8888, 1=8880, 2=5551, 3=5650) (R/W)
 2-7 Unused (0)
 8-9 Output RGBA Format (0=8888, 1=8880, 2=5551, 3=5650) (R/W)
 10-21 Unused (0)
 22 Input DMA Enable (0=Disable, 1=Enable CDMA 17h/19h) (R/W)
 23 Output DMA Enable (0=Disable, 1=Enable CDMA 18h/1Ah) (R/W)
 24 Input DRQ (0=No, 1=DRQ) (write 1 to ack?) (R/ack?)
 25 Output DRQ, 8th input line (0=No, 1=DRQ) (write 1 to ack?) (R/ack?)
 26-28 Unused (0)
 29 Interrupt upon DRQ(s)? (0=Disable, 1=Enable IRQ 45h/46h) (R/W)
 30 Interrupt upon Transfer done (0=Disable, 1=Enable IRQ 45h/46h) (R/W)
 31 Start/Busy (0=Idle/Ready, 1=Start/Busy) (R/W) |
| --- |

Send/Recv is done in units of 8 lines (unlike Y2R, which requires sending 9
lines in first block).

**10130004h/10131004h - New3DS - L2B_WIDTH (R/W)**

**10130006h/10131006h - New3DS - L2B_HEIGHT (R/W)**

```

| 0-2 Unused (0)
 3-9 Width/Height in 8 pixel units (01h..7Fh=8..1016? pixels, or 00h=?)
 10-15 Unused (0) |
| --- |

**10130020h/10131020h - New3DS - L2B_ALPHA (R/W)**

```

| 0-7 Alpha value for all pixels (00h..FFh = Transparent..Solid)
 8-31 Unused (0) |
| --- |

Used as alpha for output format 8888 and 5551 (the latter uses only bit7 of the
8bit value).

Note: Any alpha values written to the input fifo are ignored, the alpha value
is always taken from the alpha register, not from the incoming pixels.

**10330000h/10331000h - New3DS - L2B_FIFO (R and W) (empty=data_abort)**

```

| 0-31 Pixel data |
| --- |

The FIFO is mirrored across a 1000h-byte window (but eight large scanlines may
be larger than that, so it's better to used a fixed FIFO address instead of
increasing addresses in that window).