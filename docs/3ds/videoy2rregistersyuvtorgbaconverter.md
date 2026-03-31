# Videoy2Rregistersyuvtorgbaconverter

> Source: https://problemkaputt.de/gbatek.htm
> Section: Videoy2Rregistersyuvtorgbaconverter

3DS Video Y2R Registers (YUV-to-RGBA Converter) 
| |
| --- |

 **Y2R Registers (aka YUV-to-RGB aka YCbCr-to-RGB)**
 
```

| 10102000h ARM11/ARM9 Y2R_0 Registers ;\original Y2R unit
 10302000h ARM11 Y2R_0 FIFOs ;/(intended for camera)
 10132000h ARM11/ARM9 Y2R_1 Registers ;\New3DS ;\extra New3DS Y2R unit
 10332000h ARM11 Y2R_1 FIFOs ;/ ;/(intended for MVD) |
| --- |

The Y2R unit(s) are general purpose YUV-to-RGB converters for photo/video
decoding. The first Y2R unit was invented for the camera, the second Y2R unit
for MVD on New3DS. The two units appear to be identical.

```

| 10102000h/10132000h 4 Y2R_CNT R/W: E8C31F07h ;\
 10102004h/10132004h 2 Y2R_WIDTH ;width (pix) R/W: 03F8h ;
 10102006h/10132006h 2 Y2R_HEIGHT ;height (pix) R/W: 03FFh ;
 10102008h/10132008h ?? Y2R_STROBE ;ack fifo? dummy r/w? ;
 10102010h/10132010h 2 Y2R_MULTIPLIER_Y_TO_RGB R/W: 03FFh ; Control
 10102012h/10132012h 2 Y2R_MULTIPLIER_V_TO_R R/W: 03FFh ; Regs
 10102014h/10132014h 2 Y2R_MULTIPLIER_V_TO_G R/W: 03FFh ;
 10102016h/10132016h 2 Y2R_MULTIPLIER_U_TO_G R/W: 03FFh ;
 10102018h/10132018h 2 Y2R_MULTIPLIER_U_TO_B R/W: 03FFh ;
 1010201Ah/1013201Ah 2 Y2R_OFFSET_R ;\ R/W: FFFFh ;
 1010201Ch/1013201Ch 2 Y2R_OFFSET_G ; signed R/W: FFFFh ;
 1010201Eh/1013201Eh 2 Y2R_OFFSET_B ;/ R/W: FFFFh ;
 10102020h/10132020h 2 Y2R_ALPHA ;bit7 for 5551 R/W: 000000FFh ;
 10102100h/10132100h 4 Y2R_DITHER0 R/W: 0000CCCCh ;
 10102108h/10132108h 4 Y2R_DITHER1 R/W: 0000CCCCh ;
 10102110h/10132110h 4 Y2R_DITHER2 R/W: 0000CCCCh ;
 10102118h/10132118h 4 Y2R_DITHER3 R/W: 0000CCCCh ;/
 10302000h/10332000h 80h Y2R_INPUT_Y ;aka Luma W: ;\
 10302080h/10332080h 80h Y2R_INPUT_U ;aka Cb W: ; FIFO's
 10302100h/10332100h 80h Y2R_INPUT_V ;aka Cr W: ; (ARM11)
 10302180h/10332180h 80h Y2R_INPUT_YUYV ;Y1,U,Y2,V W: (camera) ;
 10302200h/10332200h 80h Y2R_OUTPUT_RGBA R: (RGBA) ;/ |
| --- |

**10102000h/10132000h - Y2R_CNT (R/W)**

```

| 0-2 Input Format YUV (0-4 = 422'8, 420'8, 422'16, 420'16, 422'BATCH) (R/W)
 3-7 Unused (0)
 8-9 Output Format RGBA (0=8888, 1=8880, 2=5551, 3=5650) (R/W)
 10-11 Output Clockwise Rotate (0=None, 1=90', 2=180', 3=270') (R/W)
 12 Output Swizzle (0=LinearFramebuf, 1=MortonSwizzleTexture)(R/W)
 13-14 Unused (0)
 15 Unknown, reportedly used, but always 0 (maybe write-only?) (?)
 16 Brightness Dither Enable (0=No, 1=Use Y2R_DITHER0-3) (R/W)
 17 Brightness Ugly Pulsation? (0=No, 1=Add 0,1,2,3 in frame 0,1,2,3) (R/W)
 18-20 Unused (0)
 21 Unknown, reportedly used, but always 0 (maybe write-only?) (?)
 22 Input DMA Enable (0=Disable, 1=Enable CDMA 09h/15h) (R/W)
 23 Output DMA Enable (0=Disable, 1=Enable CDMA 0Ah/16h) (R/W)
 24 Input DRQ Y? (0=No, 1=DRQ) (write 1 to ack?) (R/ack?)
 25 Input DRQ U? (0=No, 1=DRQ) (write 1 to ack?) (R/ack?)
 26 Input DRQ V? (0=No, 1=DRQ) (write 1 to ack?) (R/ack?)
 27 Input DRQ YUYV (batch) (0=No, 1=DRQ) (write 1 to ack?) (R/ack?)
 28 Output DRQ RGB, 9th input line (0=No, 1=DRQ) (write 1 to ack?) (R/ack?)
 29 Interrupt upon DRQ(s)? (0=Disable, 1=Enable IRQ 4Bh/4Eh) (R/W)
 30 Interrupt upon Transfer done (0=Disable, 1=Enable IRQ 4Bh/4Eh) (R/W)
 31 Start/Busy (0=Idle/Ready, 1=Start/Busy) (R/W) |
| --- |

Input formats...

```

| INPUT_YUV422_INDIV_8 0 is that 8bit? or 8x8pix? or divide by 8?
 INPUT_YUV420_INDIV_8 1
 INPUT_YUV422_INDIV_16 2 is that 16bit? or 16x16pix? or divide by 16?
 INPUT_YUV420_INDIV_16 3
 INPUT_YUV422_BATCH 4 aka camera YUYV |
| --- |

Conversion is done in units of 8 lines. However, the first Input block must be
9 lines (unless the total height is smaller), and the final blocks can be
smaller (depending on amount of remaining lines). For example:

```

| Send 9,8,8,8,8,3 scanlines ;\for 44 scanlines
 Recv 8,8,8,8,8,4 scanlines ;/ |
| --- |

The odd amount of sending 9 lines in 1st block might allow to keep converting
data while receiving responses.

```

| XXX does that 9-lines-input thing also apply to Input Format 0-3 ? |
| --- |

**10102004h/10132004h - Y2R_WIDTH**

```

| 0-2 Unused (0)
 3-9 Width, in 8-pixel units (01h..7Fh=8..1016 pix, or 00h=?)
 10-15 Unused (0) |
| --- |

**10102006h/10132006h - Y2R_HEIGHT**

```

| 3-9 Height in 1-pixel units (001h..3FFh=1..1023 pix, or 000h=?)
 10-15 Unused (0) |
| --- |

**10102008h/10132008h - Y2R_STROBE ;ack fifo? dummy r/w?**

Unknown, there is no read/write-able data here, and the register is normally
left unused, however, reading or writing-any-value seems to
strobe/trigger/ack/reset some internal stuff?

**Standard YUV to RGB Formula (aka YCbCr to RGB)**

The standard formula uses the constants shown below. However, one can also use
different values in the Y2R_MULTIPLIER_xxx and Y2R_OFFSET_xxx registers (that
would allow to support other YUV variants, or to adjust more pale or more
colorful output, or even to change RGB to BGR order; if the input FIFOs are
also swapped accordingly).

```

| R = Y*1.00 + (Cr-80h)*1.402
 G = Y*1.00 - (Cr-80h)*0.714)-(Cb-80h)*0.344
 B = Y*1.00 + (Cb-80h)*1.772
 Clip results to MinMax(00h,FFh), and apply final divide by 8 for RGB555. |
| --- |

Note: In the standard YUV-to-RGB formula one would usually subtract 80h from
the U/V values before multiplication. However, the 3DS does instead add
variable offsets after multiplication (so those offsets need to be matched to
the multipliers).

**10102010h/10132010h - Y2R_MULTIPLIER_Y_TO_RGB (R/W) ;usually (1.000)*100h**

**10102012h/10132012h - Y2R_MULTIPLIER_V_TO_R (R/W) ;usually (1.402)*100h**

**10102014h/10132014h - Y2R_MULTIPLIER_V_TO_G (R/W) ;usually (0.714)*100h**

**10102016h/10132016h - Y2R_MULTIPLIER_U_TO_G (R/W) ;usually (0.344)*100h**

**10102018h/10132018h - Y2R_MULTIPLIER_U_TO_B (R/W) ;usually (1.772)*100h**

```

| 0-9 Multiplier for the YUV-to-RGB formula (unsigned, 0..3FFh)
 10-15 Unused (0) |
| --- |

**1010201Ah/1013201Ah - Y2R_OFFSET_R (R/W) ;usually (-1.402)*1000h**

**1010201Ch/1013201Ch - Y2R_OFFSET_G (R/W) ;usually (+0.714+0.344)*1000h**

**1010201Eh/1013201Eh - Y2R_OFFSET_B (R/W) ;usually (-1.772)*1000h**

```

| 0-15 Offset for the YUV-to-RGB formula (signed, -8000h..+7FFFh) |
| --- |

**10102020h/10132020h - Y2R_ALPHA (R/W)**

```

| 0-7 Alpha for RGBA output format 8888 and 5551 (the latter uses only bit7)
 8-31 Unused (0) |
| --- |

**10102100h/10132100h - Y2R_DITHER0 (R/W)**

**10102108h/10132108h - Y2R_DITHER1 (R/W)**

**10102110h/10132110h - Y2R_DITHER2 (R/W)**

**10102118h/10132118h - Y2R_DITHER3 (R/W)**

```

| 0-31 Dither, R/W-mask 0000CCCCh |
| --- |

**10302000h/10332000h - 80h byte window - Y2R_INPUT_Y (aka Luma) (W)**

**10302080h/10332080h - 80h byte window - Y2R_INPUT_U (aka Cb) (W)**

**10302100h/10332100h - 80h byte window - Y2R_INPUT_V (aka Cr) (W)**

**10302180h/10332180h - 80h byte window - Y2R_INPUT_YUYV (camera Y1,U,Y2,V) (W)**

**10302200h/10332200h - 80h byte window - Y2R_OUTPUT_RGBA (R)**

```

| 0-31 FIFO data |
| --- |

The FIFOs are mirrored to 80h-byte windows, however, the transfer blocks with 8
or 9 scanlines are much longer than that (so better it's better to use a fixed
FIFO address, instead of increasing addresses in the 80h-byte windows)

For the optional swizzling feature, see

3DS Video Texture Swizzling