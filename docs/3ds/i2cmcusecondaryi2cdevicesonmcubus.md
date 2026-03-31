# I2Cmcusecondaryi2Cdevicesonmcubus

> Source: https://problemkaputt.de/gbatek.htm
> Section: I2Cmcusecondaryi2Cdevicesonmcubus

3DS I2C MCU secondary I2C Devices (on MCU bus) 
| |
| --- |

 **Bus/Device MCU:30h - Accelerometer**
 See Accel chapter.
 3DS I2C MCU[40h-51h] - Accelerometer/Pedometer
 
 **Bus/Device MCU:6Ch - Fuel Gauge**
 
```

| index____________dir_;MAX17040___________;MAX17048___;Richtek RT9428___
 FUEL[02h] R ;VCELL, voltage ;VCELL ;VBAT
 FUEL[04h] R ;SOC, StateOfCharge ;SOC ;SOC
 FUEL[06h] W ;MODE ;MODE ;CONTROL
 FUEL[08h] R ;VERSION ;VERSION ;DEVICE ID
 FUEL[0Ah] - ;- ;HIBRT ;Status, dSOC
 FUEL[0Ch] R/W ;RCOMP ;CONFIG ;CONFIG
 FUEL[0Eh] R/W ;??? ;??? ;OCV (but read-only!)
 FUEL[14h] - ;- ;VALRT ;-
 FUEL[16h] - ;- ;CRATE ;-
 FUEL[18h] - ;- ;VRESET/ID ;-
 FUEL[1Ah] - ;- ;STATUS ;-
 FUEL[3Eh] W ;??? ;??? ;???
 FUEL[40h+0..3Eh] W ;??? ;TABLE ;???
 FUEL[80h+0..1Eh] W ;??? ;??? ;???
 FUEL[FEh] - ;COMMAND ;CMD ;MFA |
| --- |

The fuel gauge registers are 16bit, big-endian (eg.
"write(device,index,msb,lsb)").

One should always read/write 2 bytes. Trying to read a single byte from odd
index does instead return the msb from even index. Trying to read more than 2
bytes does weirdly return each 2-byte pair TWICE, and does then advance to the
next 2-byte pair.

Old3DS uses a small 8pin fuel gauge with marking 17040, which is apparently
Maxim MAX17040.

New3DS uses a very tiny 8pin bga fuel gauge with marking 7048, which might be
Maxim MAX17048 (although the MCU firmware uses some undocumented registers that
aren't mentioned in MAX17048 datasheet). The MCU firmware seems to detect
different fuel gauge versions/revisions, unknown which different chip(s) are
used (eg. Richtek RT9428 would be also compatible).

**Bus/Device MCU:84h - Power Managment Device**

```

| POW[00h] Powerman version? (can be read via MCU[7Fh:01h])
 POW[01h] set to 00h,0Fh,1Fh
 POW[02h] flags (bit0..bit4 or so)
 POW[03h] flags (bit0, bit1)
 POW[04h] backlight enable flags (bit0, bit1)
 POW[05h] set to 00h,08h,27h ;00h=Old3DS, 08h=New3DS, 27h=PowerOff??
 POW[06h] from MCU[03h] ;top_screen_flicker (VCOM)
 POW[07h] from MCU[04h] ;bottom_screen_flicker (VCOM)
 POW[08h..FFh] unused (mirrors of POW[00h..07h]) |
| --- |

One should always read/write only 1 byte, trying to read more bytes returns
FFh's for the extra bytes.

**Bus/Device MCU:A4h - Touchscreen/Sound Controller**

```

| TSC[10h] flags?
 TSC[12h] batt.low.flag (bit0)
 TSC[13h] volume (38h..7Fh)
 TSC[20h] set to AAh |
| --- |

One can read 1 or more bytes.