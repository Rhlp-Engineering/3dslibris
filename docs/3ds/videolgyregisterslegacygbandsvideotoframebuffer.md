# Videolgyregisterslegacygbandsvideotoframebuffer

> Source: https://problemkaputt.de/gbatek.htm
> Section: Videolgyregisterslegacygbandsvideotoframebuffer

3DS Video LGY Registers (Legacy GBA/NDS Video to Framebuffer) 
| |
| --- |

 The LGYFB units are for forwarding GBA/NDS/DSi video to 3DS screens with optional scaling. The input comes directly from the GBA/NDS video controller, the output must be DMAed to memory.
 That is, ARM11 must handle that memory transfers in background while running GBA/NDS/DSi software on ARM7/ARM9 side.
 
 **10110000h - LGYFB_0 (Legacy Framebuffer 0) (NDS bottom screen) (and GBA)**
 **10111000h - LGYFB_1 (Legacy Framebuffer 1) (NDS top screen) (and GBA)**
 
```

| 10110000h/10111000h 4 LGYFB_CNT R/W mask:00019f37h ;\
 10110004h/10111004h 4 LGYFB_SIZE R/W mask:01ff01ffh ; Control
 10110008h/10111008h 4 LGYFB_IRQ_STAT R/ack mask:01f80007h ; Status
 1011000Ch/1011100Ch 4 LGYFB_IRQ_ENABLE R/W mask:0007h ;/
 10110020h/10111020h 4 LGYFB_ALPHA R/W mask:000000ffh ;-Alpha
 101100F0h/101110F0h 4 LGYFB_PREFETCH R/W mask:0000000fh ;-Prefetch?
 10110100h/10111100h 4 LGYFB_DITHER0 R/W mask:0000cccch ;\
 10110108h/10111108h 4 LGYFB_DITHER1 R/W mask:0000cccch ; Dither
 10110110h/10111110h 4 LGYFB_DITHER2 R/W mask:0000cccch ;
 10110118h/10111118h 4 LGYFB_DITHER3 R/W mask:0000cccch ;/
 10110200h/10111200h 4 LGYFB_V_LEN R/W mask:00000007h ;\Vertical
 10110204h/10111204h 4 LGYFB_V_PATTERN R/W mask:000000ffh ; Scaling
 10110240h/10111240h 4x30h LGYFB_V_ARRAY R/W mask:0000fff0h ;/
 10110300h/10111300h 4 LGYFB_H_LEN R/W mask:00000007h ;\Horizontal
 10110304h/10111304h 4 LGYFB_H_PATTERN R/W mask:000000ffh ; Scaling
 10110340h/10111340h 4x30h LGYFB_H_ARRAY R/W mask:0000fff0h ;/
 10310000h/10311000h 1000h LGYFB_FIFO R CDMA only ;-Output |
| --- |

**10110000h/10111000h - LGYFB_CNT (R/W)**

```

| 0 Start/Enable (0=Stop, 1=Start) (R/W)
 1 Enable Vertical Scaling (0=Disable, 1=Enable; via LGYFB_V_xxx) (R/W)
 2 Enable Horizontal Scaling (0=Disable, 1=Enable; via LGYFB_H_xxx) (R/W)
 3 Unused (0)
 4 Brightness Dither Enable (0=No, 1=Use LGYFB_DITHER0-3) (R/W)
 5 Brightness Dither, too? (as above, no Y2R-style Pulsation) (R/W)
 6-7 Unused (0)
 8-9 Output Format RGBA (0=8888, 1=8880, 2=5551, 3=5650) (R/W)
 10-11 Output Clockwise Rotate (0=None, 1=90', 2=180', 3=270') (R/W)
 12 Output Swizzle (0=LinearFramebuf, 1=MortonSwizzleTexture)(R/W)
 13-14 Unused (0)
 15 Enable DMA (0=Off, 1=Enable CDMA 0Dh/0Eh) (R/W)
 16 Unknown... seems to have no visible effect for GBA/NDS (0=?, 1=?) (R/W)
 17-31 Unused (0) |
| --- |

Caution: Starting the LGYFB transfers needs to be synced to the
CFG11_TWLMODE_BOOT mode switch (eg. after the mode switch, wait some thousands
of cycles, then start LGYFB, otherwise some scanline counter isn't synced
properly, causing garbage to be displayed in upper scanlines of all frames).

Once when started, the transfer does auto-repeat each frame (although, that may
hang with some/wrong settings; in that case it can help to toggle CNT.bit0
after DMAing the last block of each frame).

**10110004h/10111004h - LGYFB_SIZE (R/W)**

```

| 0-8 Output Width (after scaling), minus 1 (0..1FFh = 1..512 pixels) (R/W)
 9-15 Unused (0)
 16-24 Output Height (after scaling), minus 1 (0..1FFh = 1..512 pixels) (R/W)
 25-31 Unused (0) |
| --- |

Caution: Must be written via 32bit STR opcodes (trying to use 16bit STRH will
set BOTH halfwords to the same value).

Design Mistake: The 512pix limit prevents using the upper screen in 800x240
mode (ie. sharp horizontal scaling can be done only via GPU or software
scaling, not via LGYFB hardware scaling).

**10110008h/10111008h - LGYFB_IRQ_STAT (R/ack)**

```

| 0 First 8-Line Output Block (0=No, 1=Yes/IRQ) (write 1 to clear) (R/ack)
 1 Next 8-Line Output Block (0=No, 1=Yes/IRQ) (write 1 to clear) (R/ack)
 2 Last Input? Line (0=No, 1=Yes/IRQ) (write 1 to clear) (R/ack)
 3-15 Unused (0)
 16-24 Output Block Line Number for IRQ bit0/1 (step 8) (R)
 25-31 Unused (0) |
| --- |

The Output Block Line Number can be used to compute the destination address for
IRQ bit0/1. The initial line number upon reset is random/garbage (but gets
valid after setting LGYFB_CNT.bit0).

Overrun can occur when not reading the output FIFO fast enough. After overrun,
bit1 triggers only on each 2nd block, and bit2 won't trigger at all.

**1011000Ch/1011100Ch - LGYFB_IRQ_ENABLE - ? (R/W)**

```

| 0 First 8-Line Output Block (0=Off, 1=Enable IRQ 4Ch/4Dh) (R/W)
 1 8-Line Output Blocks (0=Off, 1=Enable IRQ 4Ch/4Dh) (R/W)
 2 Last Input? Line (0=Off, 1=Enable IRQ 4Ch/4Dh?) (R/W)
 3-31 Unused (0) |
| --- |

IRQ enable does also require LGYFB_CNT.bit0=1 and CFG11_TWLMODE_BOOT.bit15=1.

The end of frame irq occurs only if the blocks were actually transferred (via
CDMA).

**10110020h/10111020h - LGYFB_ALPHA - (R/W)**

```

| 0-7 Alpha value for all pixels (00h..FFh = Transparent..Solid)
 8-31 Unused (0) |
| --- |

Used as alpha for output format 8888 and 5551 (the latter uses only bit7 of the
8bit value).

**101100F0h/101110F0h - LGYFB_PREFETCH (R/W)**

```

| 0-3 Number of pixel prefetches before scaling? (initially 0Fh on reset)
 4-31 Unused (0) |
| --- |

IRQ and DMA requests won't occur when using too small values. Without scaling
values 01h..0Fh are working, with 2x vertical scaling only values 06h..0Fh are
working.

**10110100h/10111100h - LGYFB_DITHER0 (R/W)**

**10110108h/10111108h - LGYFB_DITHER1 (R/W)**

**10110110h/10111110h - LGYFB_DITHER2 (R/W)**

**10110118h/10111118h - LGYFB_DITHER3 (R/W)**

```

| 0-31 Dither alike Y2R, R/W-mask 0000CCCCh |
| --- |

**10310000h/10311000h - LGYFB_FIFO (R)**

Caution: This FIFO works via CDMA only (unlike most or all other FIFOs, it does
trigger data abort when trying to read via CPU LDR opcodes; even if there is
data in the FIFO).

```

| 0-31 Output FIFO (contains 8 output lines per DMA request) |
| --- |

Use DMAWFP opcode (Wait for Peripheral) before reading an 8-line block. Or,
wait for LGYFB_IRQ_STAT bit0/1, and then manually start the DMA for one 8-line
block (the latter can be useful for transfers with clockwise rotate; where one
may need to patch the destination address for each block).

For the optional swizzling feature, see

3DS Video Texture Swizzling

```

| ________________________________ Scaling Unit ________________________________ |
| --- |

**10110200h/10111200h - LGYFB_V_LEN - Vertical scaling (R/W)**

**10110300h/10111300h - LGYFB_H_LEN - Horizontal scaling (R/W)**

```

| 0-2 Batch size-1 (0..7 = 1..8 dst pixels) (using (1..8)*6 array entries)
 3-31 Unused (0) |
| --- |

Selects the number of pattern bits and array entries to be used (before
repeating the scaling pattern).

**10110204h/10111204h - LGYFB_V_PATTERN - Vertical scaling (R/W)**

**10110304h/10111304h - LGYFB_H_PATTERN - Horizontal scaling (R/W)**

```

| 0-7 Read a new src pixel before computing 1st..8th dst pixel (0=No, 1=Yes)
 8-31 Unused (0) |
| --- |

"The amount of set bits determine how many pixels are read each batch."

"Any bit indexes past LGYFB_x_LEN are ignored."

"This value is 8 bits, but it has to be written with a 32bit write."

Example values:

```

| Len Pattern Effect
 1 xxxxxxx1b No scaling (1 input pixels --> 1 output pixels)
 8 11111111b No scaling (8 input pixels --> 8 output pixels)
 5 xxx01111b Scale by 1.25 (4 input pixels --> 5 output pixels) ;NDS/DSi
 4 xxxx0111b Scale by 1.33 (3 input pixels --> 4 output pixels)
 3 xxxxx011b Scale by 1.5 (2 input pixels --> 3 output pixels)
 6 xx011011b Scale by 1.5 (4 input pixels --> 6 output pixels) ;GBA
 3 xxxxx001b Scale by 3 (1 input pixels --> 3 output pixels)
 2 xxxxxx01b Scale by 2 (1 input pixels --> 2 output pixels)
 8 01010101b Scale by 2 (4 input pixels --> 8 output pixels)
 GBA (240x160) scale by 1.5 = 3DS top screen (360x240)
 GBA (240x160) scale by 1.33 = 3DS bottom screen (320x213)
 NDS (256x192) scale by 1.25 = 3DS either screen (320x240) |
| --- |

**10110240h/10111240h - LGYFB_V_ARRAY - Vertical scaling, 6x8 words (R/W)**

**10110340h/10111340h - LGYFB_H_ARRAY - Horizontal scaling, 6x8 words (R/W)**

This array contains 6x8 words, used to compute up to 8 output pixels, with
brightness multipliers for 6 input pixels each.

```

| 0-3 Unused (0) (Nintendo writes 16bit to bit0-15, but bit0-3 are ignored)
 4-15 Brightness per source pixel (signed, -800h..+7FFh; 400h=full/max)
 16-31 Unused (0) |
| --- |

Caution: The first/last array entries are supporting only smaller signed
numbers (the hardware triggers Data Abort writing larger values and forcefully
copies bit15 to all sign bits, eg. [10110340h]=8230h becomes FE30h).

```

| range=FC00h-03FFh for 1st input pixel ;-first 6 words
 range=F000h-0FFFh for 2nd input pixel
 range=8000h-7FFFh for 3rd input pixel ;\middle 12 words, with full range
 range=8000h-7FFFh for 4th input pixel ;/
 range=F000h-0FFFh for 5th input pixel
 range=FC00h-03FFh for 6th input pixel ;-last 6 words |
| --- |

The sum of six input values should be 400h (the hardware does automatically
clip results to min/max brightness; clipping can happen when mixing positive
and negative values; with some of them getting multiplied with dark input
pixels).

Note: Multipliers bigger than 400h are glitchy (value 7FFh somehow converts
white pixels to dark gray).

**Default Array for GBA screen (scale by 1.5) (240x160 to 360x240)**

This is using Pattern=011011b and Length=6 (minus 1). The array entries are
straight ahead, using pixels with full brightness, and merged pixels with half
brightness:

```

| 0000h,0000h,0000h,0000h,0000h,0000h, N/A , N/A <-- for 1st input pixel
 0000h,0000h,0000h,0000h,0000h,0000h, N/A , N/A <-- for 2nd input pixel
 0000h,2000h,4000h,0000h,2000h,4000h, N/A , N/A <-- for 3rd input pixel
 4000h,2000h,0000h,4000h,2000h,0000h, N/A , N/A <-- for 4th input pixel
 0000h,0000h,0000h,0000h,0000h,0000h, N/A , N/A <-- for 5th input pixel
 0000h,0000h,0000h,0000h,0000h,0000h, N/A , N/A <-- for 6th input pixel
 | | | | | | | |
 | | | | | | '-----'-----> unused array entries
 | | | | | '-----------------> to 6th output pixel
 | | | | '-----------------------> to 5th output pixel
 | | | '-----------------------------> to 4th output pixel
 | | '-----------------------------------> to 3rd output pixel
 | '-----------------------------------------> to 2nd output pixel
 '-----------------------------------------------> to 1st output pixel |
| --- |

For whatever reason, this is scaling by 6:4 with 6 output pixels (instead of
3:2 with 3 output pixels). Unknown if it's faster that way, or if there's some
other advantage.

**Default Array for NDS/DSi screens (scale by 1.25) (256x192 to 320x240)**

This is using Pattern=01111b and Length=5 (minus 1). The array entries contain
positive and negative values, which might raise contrast between bright/dark
pixels:

```

| 0000h,004Eh,011Dh,01E3h,01C1h, N/A , N/A , N/A <-- for 1st input pixel
 0000h,FCA5h,F8D0h,F69Dh,F873h, N/A , N/A , N/A <-- for 2nd input pixel
 0000h,0D47h,1E35h,2F08h,3B6Fh, N/A , N/A , N/A <-- for 3rd input pixel
 4000h,3B6Fh,2F08h,1E35h,0D47h, N/A , N/A , N/A <-- for 4th input pixel
 0000h,F873h,F69Dh,F8D0h,FCA5h, N/A , N/A , N/A <-- for 5th input pixel
 0000h,01C1h,01E3h,011Dh,004Eh, N/A , N/A , N/A <-- for 6th input pixel
 | | | | | | | |
 | | | | | '-----'-----'-----> unused array entries
 | | | | '-----------------------> to 5th output pixel
 | | | '-----------------------------> to 4th output pixel
 | | '-----------------------------------> to 3rd output pixel
 | '-----------------------------------------> to 2nd output pixel
 '-----------------------------------------------> to 1st output pixel |
| --- |

Weirdly, the values for 2nd-5th output pixel values sum up to 3FDDh/3FAAh
(actually less, because the lower 4bit are ignored), making them a bit darker
than 1st output pixel.

```

| ____________________________ Touchscreen Scaling _____________________________ |
| --- |

**Touchscreen Scaling for NDS/DSi mode**

The 3DS does have the 320x240 pixel touchscren calibration stored in the
BlkID=00040000h entry in "config" file (and additionally has the original
factory calibration in HWCAL files).

3DS Files - Config Savegame

The NDS (and DSi) have touch screen calibration in Wifi FLASH user settings
(and additionally in a RAM copy at 2FFFC80h).

DS Firmware User Settings

The DSi additionally has the same touch screen calibration values in
TWLCFGn.dat files (and additionally in a RAM copy at 2000400h; whereof, the
TWLCFGn.dat files don't exist on 3DS, but the RAM copy at 2000400h should be
present in DSi mode).

DSi SD/MMC Firmware System Settings Data Files

Normally, NDS/DSi titles are zoomed from 256x192 to 320x240 pixels (with 1.25
scaling). For that, Nintendo has simply adjusted to pixel coordinates (without
changing the ADC values:

```

| pixNds = pix3ds * 4 / 5 ;(20h,18h),(120h,D8h) --> (19h,13h),(E6h,ACh)
 note: adjusting the ADC values would give slightly better precision |
| --- |

Optionally, NDS/DSi titles can be displayed at 256x192 without scaling (when
holding START or SELECT while starting the title, or while starting DS Download
Play). The image is then horizontally centered (with 32pix borders on left and
right sides), and moved as close to the hinge as possible (0pix upper border,
and 48pix lower border). For that, Nintendo has also used the same "scaled"
pixel coordinates (for whatever reason), and then adjusted to ADC values
somewhat as so:

```

| adcNds = (pixNds+32-pix3ds1)*(adc3ds2-adc3ds1)/(pix3ds2-pix3ds1)+adc3ds1 ;x
 adcNds = (pixNds+0-pix3ds1) *(adc3ds2-adc3ds1)/(pix3ds2-pix3ds1)+adc3ds1 ;y
 note: the formula used by nintendo has some additional rounding errors |
| --- |

Below are some sample values from a 3DS config file, with corresponding scaled
& unscaled NDS/DSi-mode values. Plus values from a real NDS console.

```

| 3DS/config NDS/scaled NDS/unscaled RealNDS
 021Ch 021Ch 0342h 02A4h ;adc.x1
 023Dh 023Dh 01EDh 0354h ;adc.y1
 0020h 19h 19h 20h ;pix.x1
 0018h 13h 13h 20h ;pix.y1
 0DF2h 0DF2h 0CBDh 0D38h ;adc.x2
 0E0Ah 0E0Ah 0B55h 0D28h ;adc.y2
 0120h E6h E6h E0h ;pix.x2
 00D8h ACh ACh A0h ;pix.y2 |
| --- |

Note: Real NDS/DSi did use pixel points (20h,20h),(E0h,A0h). And, for the
unscaled mode, the ADC values may exceed the boundaries of the used 256x192
pixel screen area. Anyways, most or all NDS/DSi titles should work with
whatever points being stored in the calibration data, and should
ignore/saturate any out of bounds ADC values.

Depending on the cart header, the NDS/DSi mode can emulate old NDS or new DSi
touchscreen conrollers:

DS Touch Screen Controller (TSC)

DSi Touchscreen/Sound Controller

The NDS/DSi calibration is same for both modes. Whereas, NDS seems to have
assigned upper-left as pixel (1,1) instead of (0,0), unknown if that applies
for DSi and 3DS, too.