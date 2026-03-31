# I2Cmcu03H04H20H24Hpowercontrol

> Source: https://problemkaputt.de/gbatek.htm
> Section: I2Cmcu03H04H20H24Hpowercontrol

3DS I2C MCU[03h-04h, 20h-24h] - Power Control 
| |
| --- |

 **MCU[03h] - LCD Top Screen VCOM "flicker" (R/W)**
 **MCU[04h] - LCD Bottom Screen VCOM "flicker" (R/W)**
 The VCOM voltages should be half of the corresponding AVDD voltages (so that positive and negative voltages will have same intensity), otherwise the picture may appear flimmering, have colors with wrong brightness, and could suffer permanent burn-in, or temporarily stuck pixels.
 
```

| 0-7 VCOM Voltage (00h..FFh = 0.94V .. 3.15V) |
| --- |

The MCU firmware defaults to MCU[03h]=5Ch=1.74V and MCU[04h]=5Fh=1.77V, but
better calibrations can be found in HWCAL and "config" files.

To avoid burn-in, the 3DS is toggling between drawing pixels with
positive/negative voltages each 2nd scanline (and vice-versa in each 2nd
frame). This interlacing can cause visible dirt effects on animated, scrolled,
or blinking pixels: For example, if a "red" pixel exists only in each 2nd
frame, then it might end up to be always drawn with positive voltages (with
possible burn-in even if VCOM is okay, and with wrong brightness if VCOM isn't
properly centered, whereas, the exact centering may change when the console
does warm up).

**MCU[20h] - System Power Control (bits here are 0=No change, 1=Trigger) (W)**

```

| 0 Power off
 1 Reboot (unused?)
 2 Reboot (used by mcu sysmodule and LgyBg)
 3 Used by LgyBg to power off, causes hangs in 3DS-mode
 4 NewMCU: looks like power-off type (real power off, or sleep state...?)
 "an mcu::RTC command uses this, seems to do something with the
 watchdog: Bit 4 sets a bit at a RAM address which seems to control
 the watchdog timer state, then this bit is immediately unmasked.
 This field has a bitmask of 0x0F."
 4 OldMCU: Both backlights off ;\this is probably very
 5 OldMCU: Both backlights on ; old prototype stuff?
 6 OldMCU: LCD "push" Power Supplies off ; (normally use MCU[22h])
 7 OldMCU: LCD "push" Power Supplies on ;/ |
| --- |

Unknown how to fully power-off. Writing 01h does somewhat power-off, but the
battery still runs empty after 10-20 hours. Some people execute a
Wait-for-Interrupt opcode after the I2C write; not tested if that does help
(and if that opcode is needed on all ARM11 CPU core(s) and ARM9 side).

**MCU[21h] - DSi Power Button Status register BPTWL[10h] change (W)**

00h=No Change, other=Shuffle bits and set BPTWL[10h]

```

| 0 Copied to BPTWL[10h].bit3
 1 Copied to BPTWL[10h].bit0
 2 Copied to BPTWL[10h].bit1
 3 Copied to BPTWL[10h].bit5
 4 Copied to BPTWL[10h].bit4
 5 Copied to BPTWL[10h].bit6
 6-7 Not used (except, affect the "00h=No change" thing when nonzero) |
| --- |

**MCU[22h] - LCD Power Control (bits here are 0=No change, 1=Trigger) (W)**

```

| 0 NewMCU: LCD "push" Power Supplies and Backlights off
 1 NewMCU: LCD "push" Power Supplies on
 2 NewMCU: Bottom Screen Backlight off ;\For 2DS: Both screens
 3 NewMCU: Bottom Screen Backlight on ;/
 4 NewMCU: Top Screen Backlight off ;\For 2DS: No effect
 5 NewMCU: Top Screen Backlight on ;/
 6-7 Not used |
| --- |

Setting bit0 does disable most or all LCD related power supplies: VCOM, AVDD,
10V (probably also +5V and -5V), and backlights. Setting bit1 enables those
supplies (except backlights, which require bit3/bit5). The 1.8V supply for the
LCD I2C controllers is always on. The backlight PWM level is controlled via ARM
LCD registers.

**MCU[24h] - Forced Power Off delay (0=Never,1=Fastest,5Dh=InsaneDefault) (R/W)**

```

| 0-7 Delay in 8Hz units, this value seems to be battery backed
 (the value seems to also affect the bootrom error screen) |
| --- |