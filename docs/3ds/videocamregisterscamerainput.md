# Videocamregisterscamerainput

> Source: https://problemkaputt.de/gbatek.htm
> Section: Videocamregisterscamerainput

3DS Video CAM Registers (Camera Input) 
| |
| --- |

 **10120000h - CAM0 (external Right-Eye & internal Self-Facing camera) (as DSi)**
 **10121000h - CAM1 (external Left-Eye camera) (extra 3DS camera)**
 
```

| 1012x000h 4 R/W mask: 0000ef1ch ;CAM_CNT alike DSi cameras
 1012x004h 2 R mask: 8500h ;CAM_STAT unlike DSi (lsbs=0..500h)
 1012x006h 2 R/W mask: 003fh ;CAM_???
 1012x010h 4 R/W mask: 01ff03feh ;CAM_SOFS alike DSi cameras
 1012x014h 4 R/W mask: 01ff03feh ;CAM_EOFS alike DSi cameras
 1032x000h 80h R ;CAM_DAT fifo (20h-word window) |
| --- |

The ARM registers are similar (but not identical) as for DSi cameras,

DSi Cameras

The I2C bus camera init does work same as on DSi (for Aptina cameras),

DSi I2C Bus

Cameras must be enabled in CFG11_CAMERA_CNT (Port 10141224h).

**1012x000h - CAM_CNT - mask: 0000ef1ch - Control (R/W)**

```

| 0 Unknown (status, toggles on/off during transfer?) (R)
 1 Unknown (status, gets set during transfer?) (R)
 2 Unknown (MUST be 0, else picture gets blank?) (R/W)
 3 Unknown (0=Normal/YUV422, 1=Some other data format?) (R/W)
 4 Unknown (MUST be 1, else data transfer hangs, maybe reset?) (R/W)
 5-7 Unused (0) (-)
 8 Unknown (no effect?) (R/W)
 9 IRQ Enable (0=Disable, 1=Enable) (at picture end?) (R/W)
 10 Unknown (no effect?) (R/W)
 11 Unknown (no effect?) (R/W)
 12 Unused (0) (-)
 13 Unknown (MUST be 0, else data transfer hangs) (R/W)
 14 DMA Data Request enable (0=Disable, 1=Enable) (R/W)
 15 Transfer Enable (0=Disable/AllowConfig, 1=Enable/Transfer) (R/W)
 16-31 Unused (0) (-) |
| --- |

**1012x004h - CAM_STAT - Status (R)**

```

| 0-10 Number of 8-byte units (?) in FIFO (0..500h) (R)
 11-14 Unused? (0) (?)
 15 Overrun error, deadlock (0=Normal, 1=Overrun) (R) |
| --- |

Caution: There will be no further data received after overrun. To avoid
overrun, be sure to read data fast enough, and disable CAM_CNT.bit15 after
reading the camera bitmap.

**1012x006h - CAM_??? - Unknown (R/W)**

```

| 0-5 Unknown (no effect?) (can be set to 0..3Fh) (R/W)
 6-14 Unused? (0) (?)
 15 Clear overrun (0=No change, 1=Clear; when CAM_CNT.bit15=0) (W) |
| --- |

Unknown, bit0-5 seems to have no direct effect on the camera picture. Maybe
selects timings or amount of words for DMA requests? Or maybe some bits
resemble DSi's CAM_MCNT register?

**1012x010h - CAM_SOFS alike DSi cameras (0..01FF03FEh) (R/W)**

**1012x014h - CAM_EOFS alike DSi cameras (0..01FF03FEh) (R/W)**

```

| 0 Unused (0) (0)
 1-9 X-Offset (0..1FFh) in words (ie. 2-pixel units)? (R or R/W)
 10-15 Unused (0) (0)
 16-24 Y-Offset (0..1FFh) in scanlines? (R or R/W)
 25-31 Unused (0) (0) |
| --- |

Looks same as on DSi, but the settings seem to have no effect on 3DS, ie. there
appears to be no way to enable the trimming feature?

**1032x000h..1032x07Fh - CAM_DAT - Data FIFO (20h words window) (R)**

The FIFO read window is 20h words in size (the actual FIFO is bigger; about
500h or A00h words?). The (default) data format is same as the YUV422 format
for DSi cameras:

```

| 0-7 First Pixel Luminance (Y) (unsigned, 00h..FFh, FFh=white)
 8-15 Both Pixels Blue (Cb aka U) (unsigned, 00h..FFh, 80h=gray)
 16-23 Second Pixel Luminance (Y) (unsigned, 00h..FFh, FFh=white)
 24-31 Both Pixels Red (Cr aka V) (unsigned, 00h..FFh, 80h=gray) |
| --- |

Caution: It is important to read words from INCREASING addresses at
1032x000h..1032x07Fh (or at least toggle address bit2 after each read) (trying
to read words from FIXED address 1032x000h will return the same word endless
repeated).

**Camera Notes**

The "camera in-use" LED for external camera is controlled via MCU (but the LED
exists on DSi/3DS only, New3DS doesn't have that LED installed).

The three 3DS/New3DS cameras have the same Chip IDs (2280h) as found in the two
DSi's Aptina MT9V113 cameras.

However, New3DS is said to have improved picture quality. Unknown what that
means, maybe the I2C registers are simply initialized with better gain settings
at software side, or maybe the camera hardware is actually improved despite of
having the same chip id.

New3DS is also said to support eye-tracking, whatever that means, it is
probably done using the internal camera. And New3DS is said to have an IR-LED
next to the internal camera to assist eye-tracking (yet unknown if/how the
YUV/RGB camera could see IR-light at all).