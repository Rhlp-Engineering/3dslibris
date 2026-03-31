# Dsicameras

> Source: https://problemkaputt.de/gbatek.htm
> Section: Dsicameras

DSi Cameras 
| |
| --- |

 **Camera registers**
 Cameras are controlled and initialized via I2C bus (on ARM7 side).
 DSi I2C Bus
 The actual camera data transfers are done with below registers (on ARM9 side).
 
 **4004200h - DSi9 - CAM_MCNT - Camera Module Control**
 
```

| 0 Unknown (R or R/W)
 1 Unknown (1=Enable?) (R or R/W)
 2-4 Unknown (R or R/W)
 5 Unknown (1=Enable?) (0=CamI2C fails?) (R or R/W)
 6 Unknown (R or R/W)
 7 Unknown (gets set automatically?) (R?)
 8-15 Unknown/Unused (00h) (0?) |
| --- |

Written values are 0000h and 0022h.

Camera I2C access works only when bit5=1 (otherwise camera i2c reads just
return FFh; maybe bit5=0 issues a reset to the camera devices or so?)

"Used for resetting cameras. Once cameras are reset by poking this register,
all three 0x0400420X camera registers are set to zero." Uh, is that really
true, and which "three" registers are that?

**4004202h - DSi9 - CAM_CNT - Camera Control**

```

| 0-3 Number of DMA scanlines minus 1 (usually 3=Four Scanlines) (R or R/W)
 4 Data overrun/underrun error (R)
 5 Clear bit4, and flush CAM_DAT till next Camera Vblank? (W)
 6-7 Unknown/Unused (0) (0?)
 8-9 ? Set to 2 during init, 0 on cameras shutdown (R/W)
 10 ? Set to 1 during init, 0 on cameras shutdown (R/W)
 11 IRQ Enable (0=Disable, 1=Enable) (R/W)
 12 Unknown/Unused (0) (0?)
 13 Color Format (0=Direct/YUV422, 1=Convert YUV-to-RGB555) (R or R/W)
 14 Trimming Enable (0=Normal/FullPicture, 1=Crop via SOFS/EOFS) (R or R/W)
 15 Transfer Enable (0=Disable/AllowConfig, 1=Enable/Transfer) (R/W) |
| --- |

**4004204h - DSi9 - CAM_DAT - Camera Data (R)**

Transfers two camera pixels at once (from left-to-right, starting with upper
scanline).

Pixel Format (in "YUV422" mode):

```

| 0-7 First Pixel Luminance (Y) (unsigned, 00h..FFh, FFh=white)
 8-15 Both Pixels Blue (Cb aka U) (unsigned, 00h..FFh, 80h=gray)
 16-23 Second Pixel Luminance (Y) (unsigned, 00h..FFh, FFh=white)
 24-31 Both Pixels Red (Cr aka V) (unsigned, 00h..FFh, 80h=gray) |
| --- |

Pixel Format (in YUV-to-RGB555 mode) (matches 2D Engine Bitmap format):

```

| 0-4 First Pixel Red Intensity (0..31)
 5-9 First Pixel Green Intensity (0..31)
 10-14 First Pixel Blue Intensity (0..31)
 15 First Pixel Alpha (always 1=NonTransparent)
 16-20 Second Pixel Red Intensity (0..31)
 21-25 Second Pixel Green Intensity (0..31)
 26-30 Second Pixel Blue Intensity (0..31)
 31 Second Pixel Alpha (always 1=NonTransparent) |
| --- |

The Aptina camera's MODE_OUTPUT_FORMAT registers and MIPI_CONTROL register can
be configured to output stuff like YUV, RGB555, RGB444, BGR565, RAW8, etc.
However, DSi games seem to be always using YUV mode at camera side (and the
above RGB555 data is produced by activating YUV-to-RGB conversion in
CAM_CNT.bit13 at console side).

YUV mode gives better quality with 8bit resolution (whilst RGB555 mode is
having only 5bit, and, as it's converted from YUV, it's certainly having color
information being shared for each two-pixel groups, too).

CAM_DAT should be usually read via NDMA (see below). Manually reading CAM_DAT
for one block (eg. 256x4 pixels) does work, but it's unknown how to retrieve
further blocks via manual reading (except, one further block arrives after
around 40000h clock cycles, but that's much too slow, and it's only one extra
block).

**Formulas for converting YUV to RGB**

```

| R = Y+(Cr-80h)*1.402
 G = Y-(Cr-80h)*0.714)-(Cb-80h)*0.344
 B = Y+(Cb-80h)*1.772 |
| --- |

Clip results to MinMax(00h,FFh), and apply final divide by 8 for RGB555.

**4004210h - DSi9 - CAM_SOFS Camera Trimming Starting Position Setting (32bit)**

**4004214h - DSi9 - CAM_EOFS Camera Trimming Ending Position Setting (32bit)**

```

| 0 Unused (0) (0)
 1-9 X-Offset (0..1FFh) in words (ie. 2-pixel units)? (R or R/W)
 10-15 Unused (0) (0)
 16-24 Y-Offset (0..1FFh) in scanlines? (R or R/W)
 25-31 Unused (0) (0) |
| --- |

Crops the incoming camera picture before passing it to CAM_DAT, used only if
enabled in CAM_CNT.14.

**Write-Protected Camera Bits (R or R/W)**

The "(R or R/W)" bits are getting Read-Only when camera transmission is
enabled, ie. they can be changed only when CAM_CNT.Bit15=0.

**Internal Camera Reflections from LCD Backlights**

The LCD backlights can cause nasty reflections on the internal camera
(particulary when wearing glasses). There isn't much that could be done during
preview, but when taking photos, it might be recommended to output a black/dark
picture on the LCDs during the capture.

**Internal Camera Mirroring**

The Internal Camera is conventionally having x-flip enabled (in Aptina
MODE_SENSOR_READ_MODE registers), so the internal camera will behave as a
mirror (which may appear more familar to most users in preview mode). The
firmware's "Nintendo DSi Camera" utility is even saving jpg's in mirrored form
instead of saving true authentic photos.

**Camera Detection**

There are four possible I2C camera devices, although usually only two of them
should be installed. The firmware detects the cameras by reading their Chip ID
registers (but without actually insisting on any specific ID values, instead,
it's merely checking the ACK error flag in the I2C register - if all four
devices are returning ACK=okay, then it's actually initializing all four
cameras; though unknown if the GUI is actually supporting that many cameras).

**Camera Init**

```

| [4004004h]=[4004004h] OR 0004h ;SCFG_CLK, CamInterfaceClock = ON
 [4004200h]=0000h, delay(1Eh) ;CAM_MCNT, Camera Module Control
 [4004004h]=[4004004h] OR 0100h, delay(1Eh) ;SCFG_CLK, CamExternal Clock = ON
 [4004200h]=0022h, delay(2008h) ;CAM_MCNT, Camera Module Control
 [4004004h]=[4004004h] AND NOT 0100h ;SCFG_CLK, CamExternal Clock = OFF
 [4004202h]=[4004202h] AND NOT 8000h ;CAM_CNT, allow changing params
 [4004202h]=[4004202h] OR 0020h ;CAM_CNT, flush data fifo
 [4004202h]=([4004202h] AND NOT 0300h) OR 0200h
 [4004202h]=[4004202h] OR 0400h
 [4004202h]=[4004202h] OR 0800h ;CAM_CNT, irq enable
 [4004004h]=[4004004h] OR 0100h, delay(14h) ;SCFG_CLK, CamExternal Clock = ON
 issue "aptina_code_list_init" via I2C bus on ARM7 side
 [4004004h]=[4004004h] AND NOT 0100h ;SCFG_CLK, CamExternal Clock = OFF
 [4004004h]=[4004004h] OR 0100h, delay(14h) ;SCFG_CLK, CamExternal Clock = ON
 issue "aptina_code_list_activate" via I2C bus on ARM7 side
 [4004202h]=[4004202h] OR 2000h ;CAM_CNT, enable YUV-to-RGB555
 [4004202h]=([4004202h] AND NOT 000Fh) OR 0003h
 [4004202h]=[4004202h] OR 0020h ;CAM_CNT, flush data fifo
 [4004202h]=[4004202h] OR 8000h ;CAM_CNT, start transfer
 [4004120h]=04004204h ;NDMA1SAD, source CAM_DTA
 [4004124h]=0xxxxxxxh ;NDMA1DAD, dest RAM/VRAM
 [4004128h]=00006000h ;NDMA1TCNT, len for 256x192 total
 [400412Ch]=00000200h ;NDMA1WCNT, len for 256x4 blocks
 [4004130h]=00000002h ;NDMA1BCNT, timing interval or so
 [4004138h]=8B044000h ;NDMA1CNT, start camera DMA |
| --- |

**Specifications**

The Nintendo DSi contains two cameras. The cameras can be used in the Nintendo
DSi Camera application or DSi games that are compatible.

```

| 640*480 VGA (0.3 Megapixel)
 No zoom and no flash. |
| --- |

Photos saved in JPG format (saved in DCIM/ folder on the SD/SDHC or in the
internal memory).

**Camera Applications**

```

| Nintendo DSi Camera
 System Menu (can take photos, and can display JPG's with "Star" sticker)
 Flipnote (doesn't directly support camera hardware, but can import JPG's) |
| --- |

**Camera Games**

```

| Asphalt 4 : Elite Racing (DSiWare)
 Brain Challenge (DSiWare)
 Classic Word Games
 Cooking Coach
 Pop SuperStar : Road To Celebrity (DSiWare)
 Real Football 2009 (DSiWare)
 WarioWare : Snapped! (DSiWare)
 iCarly
 Pokemon Black,White (2010,JP)
 Castle of Magic (DSiWare)
 Photo Dojo (DSiWare)
 System Flaw (mis-uses camera as gyro sensor) |
| --- |