# Videolcdregisters

> Source: https://problemkaputt.de/gbatek.htm
> Section: Videolcdregisters

3DS Video LCD Registers 
| |
| --- |

 This seems to be an interface between GPU video output and actual LCD screens. The registers allow to control backlight brightness and to set a forced blank color. Some other registers seem to be also affect backlight brightness (or pixel brightness?); and maybe some do affect internal voltages or internal backlight PWM timings?
 
 The bootrom error screen initializes only a few of these registers (and leaves the others at their power-up defaults). Many of the uninitialized registers seem to have no effect on the visible picture, even when setting/clearing all bits; some of them affect the brightness of the pixels or backlight (but only if CtrlFlags.bit0=1).
 
 **General LCD Control Registers (ARM11 only)**
 
```

| 10202000h 4 LCD_PARALLAX_CNT Parallax Barrier Enable (R/W)
 10202004h 4 LCD_PARALLAX_PWM Parallax Barrier PWM (R/W)
 10202008h 4 LCD_STATUS ... Whatever (bit0=top screen mode?) (R)
 1020200Ch 4 LCD_CLOCK_DIS Clock Disable (bit0/16=top/bottom disable)
 10202010h 4 ... ... Whatever, usually 00000900h
 10202014h 4 LCD_RESET Reset both Displays (bit0: 0=reset)
 10202018h 1E8h - Unused (0)
 10202200h 600h LCD0_xxx Configuration for Top Screen (see below)
 10202800h 200h - Unused (0)
 10202A00h 600h LCD1_xxx Configuration for Bottom Screen (see below) |
| --- |

**LCD0/LCD1 Configuration for Top/Bottom Screen**

```

| 10202200h/10202A00h 4 LCDx_FLAGS CtrlFlags
 10202204h/10202A04h 4 LCDx_BLANK_COLOR Forced Blank Color
 10202208h/10202A08h 8 - Unused (0)
 10202210h/10202A10h 4 000003FFh ? (default=0) ;\
 10202214h/10202A14h 4 000003FFh ? (default=3FFh) ; darker when all zero
 10202218h/10202A18h 4 000003FFh ? (default=0) ; (if CtrlFlags.bit0=1)
 1020221Ch/10202A1Ch 4 000003FFh ? (default=3FFh) ;/
 10202220h/10202A20h 4 000001FFh ? (default=100h) ;-
 10202224h/10202A24h 4 000000FFh ? (default=0) ;\
 10202228h/10202A28h 4 000000FFh ? (default=0) ;
 1020222Ch/10202A2Ch 4 000000FFh ? (default=0) ;/
 10202230h/10202A30h 4 001F000Fh ? (default=4) ;-
 10202234h/10202A34h 4 - Unused (0)
 10202238h/10202A38h 4 000000FFh LastSameEntry in 10202400h (FFh=AllSame)
 1020223Ch/10202A3Ch 4 000000FFh ? (default=FFh)
 10202240h/10202A40h 4 LCDx_BACKLIGHT_DUTY Backlight Level
 10202244h/10202A44h 4 LCDx_BACKLIGHT_CNT Backlight Interval (and flags?)
 10202248h/10202A48h 8 - Unused (0)
 10202250h/10202A50h 4 00FFFFFFh New3DS only: ? ;\brighter if 2nd=FFFFFFh
 10202254h/10202A54h 4 00FFFFFFh New3DS only: ? ;/(if CtrlFlags.bit0=1)
 10202258h/10202A58h 8 - Unused (0)
 10202260h/10202A60h 8 LCDx_DITHER0 Dither ;\
 10202268h/10202A68h 8 LCDx_DITHER1 Dither ; DITHER alike Y2R
 10202270h/10202A70h 8 LCDx_DITHER2 Dither ;
 10202278h/10202A78h 8 LCDx_DITHER3 Dither ;/
 10202280h/10202A80h 24h 000000FFh ? (default=0) ;-9 words (8th=brighter)
 102022A4h/10202AA4h 4Ch - Unused (0)
 102022F0h/10202AF0h 4 (-R-) ? (R) ;\initially 00h, but can
 102022F4h/10202AF4h 4 (-R-) ? (R) ; be FFh when screens
 102022F8h/10202AF8h 4 (-R-) ? (R) ;/are on?
 102022FCh/10202AFCh 4 - Unused (0)
 10202300h/10202B00h 100h 00FFFFFFh New3DS only: LCD calibration array?
 10202400h/10202C00h 400h 000003FFh Backlight related array, used by bootrom |
| --- |

```

| ______________________________ General Control ______________________________ |
| --- |

**10202000h - LCD_PARALLAX_CNT - Parallax Barrier Enable (R/W)**

```

| 0-1 TP27 Parallax PWM Output (0=Off, 1=PWM, 2/3=Off)
 2 TP27 Parallax PWM Output Invert (0=Normal, 1=Swap On/Off)
 3-15 Unused (0)
 16-17 TP29 Parallax Enable Output (0=Off, 1=On, 2/3=Off)
 18 TP29 Parallax Enable Output Invert (0=Normal, 1=Swap On/Off)
 19-31 Unused (0) |
| --- |

Unknown what setting is best for Parallax enable... probably 10001h? To disable
parallax: set the register to zero.

Backlight level should be increased when parallax is on (as only half of the
pixels are visible per eye). And the 3D LED should be enabled via MCU[2Ch] to
notify the user to watch for the 3D effect.

No visible effect on bottom screen.

On top screen: Settings all bits causes screen to appear darker: After a while,
it becomes FULLY BLACK when viewed from front (but still visible when viewed
from above, including changes to VRAM being visible).

**10202004h - LCD_PARALLAX_PWM - Parallax Barrier PWM (R/W)**

```

| 0-15 Parallax PWM Duty Off ;(N+1)*0.9us ;\affects TP27
 16-31 Parallax PWM Duty On ;(N+1)*0.9us ;/(if enabled) |
| --- |

Usually set to 0A390A39h (off=2.5ms, on=2.5ms, aka 5ms interval with 50% duty).
At least, that's the default value is used by bootrom - unknown if better
calibration is needed?

**10202008h - LCD_STATUS - Whatever, readonly (R)**

```

| 0 Indicates current horizontal resolution of top screen, uh?
 1-7 Unknown or unused
 8 Unknown (bootrom contains unused code for reading this)
 9 Num layers per screen? (bootrom uses that) (0=One, 1=Two/Stereo)
 10-15 Unknown or unused
 16-31 Unknown or unused (maybe same as above, for bottom screen?) |
| --- |

Unknown where those values come from... maybe from Port 10400470h/10400570h?

**1020200Ch - LCD_CLOCK_DIS - LCD Clock Disable (R/W)**

```

| 0 Top Screen Clock (TP163/TP164) (0=Normal/On, 1=Screen fades out)
 1-15 Unused (0)
 16 Bottom Screen Clock (TP189) (0=Normal/On, 1=Screen fades out)
 17-31 Unused (0) |
| --- |

**10202010h - Whatever, usually 00000900h (R/W)**

```

| 0-3 Unknown (0..0Fh)
 4-7 Unused (0)
 8-12 Unknown (0..1Fh)
 13-31 Unused (0) |
| --- |

No visible effect. Power-up value is 00000900h. Bootrom doesn't change that
setting.

**10202014h - LCD_RESET - LCD Reset both Displays (R/W)**

```

| 0 Both Screens Display Enable (TP244) (0=Reset/Black, 1=On/Picture)
 1-31 Unused (0) |
| --- |

Resets the LCD I2C Registers (device 1:2Ch and 2:2Eh). The screens are black
during reset (although clock, data transfers, and backlight are kept on). For
whatever reason, the top screen isn't fully black: the upper/lower/left/right
screen borders are becoming dark gray.

```

| _____________________________ Top/Bottom Screen _____________________________ |
| --- |

**10202200h/10202A00h - LCDx_FLAGS - CtrlFlags (R/W)**

```

| 0 Enables custom settings when set?
 1-7 Unused (0)
 8-9 Unknown (0..3)
 10-31 Unused (0) |
| --- |

Bit0 should be copied to 10202244h/10202A44h.bit17?

Bit0 seems to enable custom settings in 1020221xh/10202A1xh,
10202254h/10202A54h, and in the eighth word at 10202280h/10202A80h.

**10202204h/10202A04h - LCDx_BLANK_COLOR - Forced Blank Color (R/W)**

```

| 0-7 Blank Color Red (00h..FFh)
 8-15 Blank Color Green (00h..FFh)
 16-23 Blank Color Blue (00h..FFh)
 24 Blank Color Enable (0=Normal/Picture, 1=Force Blank Color)
 25-31 Unused (0) |
| --- |

The blank color overrides the whole picture (including the Screen Border area).

**10202210h/10202A10h 4 000003FFh ? (default=0) ;\**

**10202214h/10202A14h 4 000003FFh ? (default=3FFh) ;**

**10202218h/10202A18h 4 000003FFh ? (default=0) ;**

**1020221Ch/10202A1Ch 4 000003FFh ? (default=3FFh) ;/**

```

| 0-9 Unknown (0..3FFh)
 10-31 Unused (0) |
| --- |

Screen gets darker when set to all zeroes (but only if CtrlFlags.bit0=1).

**10202220h/10202A20h 4 000001FFh ? (default=100h) ;-**

```

| 0-8 Unknown (0..1FFh)
 9-31 Unused (0) |
| --- |

**10202224h/10202A24h 4 000000FFh ? (default=0) ;\**

**10202228h/10202A28h 4 000000FFh ? (default=0) ;**

**1020222Ch/10202A2Ch 4 000000FFh ? (default=0) ;/**

```

| 0-7 Unknown (0..FFh)
 8-31 Unused (0) |
| --- |

**10202230h/10202A30h 4 001F000Fh ? (default=4) ;-**

```

| 0-3 Unknown (0..0Fh)
 4-15 Unused (0)
 16-20 Old3DS: Unused (0)
 16-20 New3DS: Unknown (0..1Fh)
 21-31 Unused (0) |
| --- |

**10202238h/10202A38h 4 000000FFh Last same entry in 10202400h/10202C00h**

```

| 0-7 Index of last SAME entry in 10202400h/10202C00h (0..FFh) (FFh=AllSame)
 8-31 Unused (0) |
| --- |

**1020223Ch/10202A3Ch 4 000000FFh ? (default=FFh)**

```

| 0-7 Unknown (0..FFh)
 8-31 Unused (0) |
| --- |

**10202240h/10202A40h - LCDx_BACKLIGHT_DUTY - Backlight Level (R/W)**

```

| 0-9 Backlight PWM Duty On (0..3FFh, in 67MHz units) (0=off)
 10-31 Unused (0) |
| --- |

Allows to change backlight brightness. The settings are different for Upper and
Lower screen (and also depend on Parallax enable).

User settings and calibration are somehow stored in HWCAL and "config" files.

To enable/disable the backlight: Use MCU[22h] (or Powerman when in NDS mode).

Caution: Setting the PWM Interval (Duty On+Off time, in the register shown
below) to too small values (less than 016h) causes Power Off (if Duty is
nonzero).

**10202244h/10202A44h - LCDx_BACKLIGHT_CNT - Backlight Interval (+flags?) (R/W)**

```

| 0-9 Backlight PWM Interval, minus 1 (000h..3FFh, in 67MHz units)
 10-11 Unused (0)
 12-15 Unknown (should be 0 or 6, depending on MCU?)
 16 Unknown
 17 Unknown
 18 Unknown
 19-31 Old3DS: Unused (0)
 19-31 New3DS: More unknown bits/values... (R/W) |
| --- |

**10202250h/10202A50h 4 00FFFFFFh ? (default=0) ;New3DS only**

**10202254h/10202A54h 4 00FFFFFFh ? (default=0) ;New3DS only**

```

| 0-23 New3DS: Unknown (0..FFFFFFh)
 24-31 Unused (0) |
| --- |

Two 24bit values. Screen gets brighter when setting the SECOND value to FFFFFFh
(but only if CtrlFlags.bit0=1).

**10202260h/10202A60h - LCDx_DITHER0 - Dither (default=0C84h)**

**10202268h/10202A68h - LCDx_DITHER1 - Dither (default=840Ch)**

**10202270h/10202A70h - LCDx_DITHER2 - Dither (default=48C0h)**

**10202278h/10202A78h - LCDx_DITHER3 - Dither (default=C048h)**

```

| 0-31 Dither alike Y2R, R/W-mask 0000CCCCh
 32-63 Unused (0) (ie. there are 32bit gaps between the four registers) |
| --- |

**10202280h/10202A80h 24h 000000FFh ? (default=0) ;-(9 words)**

```

| 0-7 Unknown (0..FFh)
 8-31 Unused (0) |
| --- |

Unknown... 9 words (with 8bit used each) at 10202280h/10202A80h and up.

Screen gets brighter when setting the EIGHTH value to FFh (but only if
CtrlFlags.bit0=1).

**102022F0h/10202AF0h - Readonly (R)**

**102022F4h/10202AF4h - Readonly (R)**

**102022F8h/10202AF8h - Readonly (R)**

```

| 0-xx Unknown (initially 00h, but can be FFh when screens are on?) (R)
 xx-31 Unused (0) |
| --- |

**10202300h/10202B00h 100h 00FFFFFFh New3DS only: LCD calibration? (40 words)**

```

| 0-23 Unknown (0..FFFFFFh) (initially random)
 24-31 Unused (0) |
| --- |

40h words (100h bytes) at 10202300h/10202B00h and up.

Reportedly initialized from nand:/ro/sys/HWCAL0.dat offset 77Ch.

On Old3DS: Unused (0)

On New3DS: R/W (even in Old3DS mode)

Bootrom doesn't init these values (so they are apparently initially disabled
somewhere).

**10202400h/10202C00h 400h 000003FFh Whatever array, used by bootrom!!!**

```

| 0-9 Unknown (0..3FFh) (initially random)
 10-31 Unused (0) |
| --- |

100h words (400h bytes) at 10202400h/10202C00h and up.

The bootrom error screen has some complicated function that initializes this
array with scaled/interpolated values, somehow related to backlight level and
screen type; although, with the used parameters... it seems to be just settings
all 100h table entries to the same value.

Alongsides, the number of leading entries with SAME value should be stored in
register 10202238h/10202A38h (ie. store N-1 if the first N table entries are
all same).