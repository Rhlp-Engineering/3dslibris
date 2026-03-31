# I2Cmcu30H3Fhrealtimeclockrtc

> Source: https://problemkaputt.de/gbatek.htm
> Section: I2Cmcu30H3Fhrealtimeclockrtc

3DS I2C MCU[30h-3Fh] - Real-Time Clock (RTC) 
| |
| --- |

 **MCU[30h] - RTC Time second (7bit) (BCD, 00h..59h) (R/W)**
 **MCU[31h] - RTC Time minute (7bit) (BCD, 00h..59h) (R/W)**
 **MCU[32h] - RTC Time hour (6bit) (BCD, 00h..23h) (R/W)**
 **MCU[33h] - RTC Time day of week (3bit) (00h..06h=Blah) (R/W)**
 **MCU[34h] - RTC Time day (6bit) (BCD, 01h..31h) (R/W)**
 **MCU[35h] - RTC Time month (5bit) (BCD, 01h..12h) (R/W)**
 **MCU[36h] - RTC Time year (8bit) (BCD, 00h..99h) (R/W)**
 Caution: The 3DS is mis-using the MCU RTC registers for counting "time since power-up" (with the DD-MM-YY HH:MM:SS values converted into seconds). The actual time/date is computed by adding a nanosecond offset from the BlkID=00030001h entry in "config" file to the MCU RTC values.
 
```
************
| Power loss
 When regaining power after fully discharging or removing the battery,
 the MCU sets RTC time lost flag (MCU[02h].bit0=1), and initializes RTC
 registers as time=00:00:00, day=01h, month=01h, year=01h, dayofweek=1.
 First boot-up after power loss
 When the ARM firmware sees the RTC time lost flag, it does reset that
 flag (by writing MCU[02h].bit0=0), and does adjust the offset in the
 "config" file so that the current time becomes 1st Jan 2011, 00:00:00.
 The MCU RTC registers are NOT changed (they do still contain "time since
 power-up").
 Changing time/date in System Menu
 Time/date can be manually changed in system settings. Again, this does
 merely change the BlkID=00030001h entry in the "config" file, without
 changing the MCU RTC registers. |
| --- |

Summary: The MCU RTC values have no direct relation the actual time and date.
For example, if the battery was removed 13 months ago in June, it's perfectly
normal to see MCU[35h]=Month=02h in July.

Note: For Day of Week, the Renesas datasheet recommends to use 0=Sunday, but
the 3DS doesn't really use that register (apart from setting it to 1=Day when
powered up).

Unknown if the RTC treats Year=00h as leap year (centuries are a special case,
maybe that's why the MCU starts in Year=01h).

**MCU[37h] - RTC Watch Error Correction (SUBCUD) (R/W) ;NOT leap year**

```

| 0-6 Signed offset (-40h..+3Fh, see below for details)
 7 Add offset when... (0=Every 20 seconds, 1=Every 60 seconds) |
| --- |

The offsets are a bit weird:

```

| +00h Range 0..(7FFFh) ;error correction disabled
 +01h..3Fh Range 0..(7FFFh+(0..124)) ;add (N-1)*2
 -01h..3Fh Range 0..(7FFFh-(0..124)) ;add (N+1)*2 or so?
 -40h Range 0..(7FFFh) ;error correction disabled |
| --- |

After battery loss, the register is 00h, the 3DS firmware changes it to the RTC
Correction value from "config" or HWCAL files.

**MCU[38h] - RTC Alarm minute (7bit) (BCD, 00h..59h) (R/W)**

**MCU[39h] - RTC Alarm hour (6bit) (BCD, 00h..23h) (R/W)**

**MCU[3Ah] - RTC Alarm day (6bit) (BCD, 01h..31h) (R/W) ;\maybe 0=off?**

**MCU[3Bh] - RTC Alarm month (5bit) (BCD, 01h..12h) (R/W) ;/**

**MCU[3Ch] - RTC Alarm year (8bit) (BCD, 00h..99h) (R/W)**

Alarm. Unknown how to enable/disable that feature (other than using a far-away
date, or even using a non-BCD date). Also unknown if alarm is working while
powered off, or while playing a game.

**MCU[3Dh] - RTC RSUBC.lsb (in 32768Hz units) (R)**

**MCU[3Eh] - RTC RSUBC.msb (latched when reading lsb) (R)**

```

| 0-15 Range 0..7FFFh (or "0..(7FFFh+error_correction_offset)") |
| --- |

**MCU[3Fh] - RTC Flags (bit0=ScreenBlack?, bit1=DisableRtc32KHzOutput) (W)**

```

| 0 Disable Power-On and force Screen Black (0=Normal, 1=Disable)
 1 Disable RTC-32KHz-Output to Atheros Wifi hardware (0=Normal, 1=Disable) |
| --- |

This register seens to enter some low power sleep mode. It should be normally
always zero, nonzero settings can cause problems even after reset or power off:

Bit0=1 prevents to power the console back on (the only way to fix that is to do
a full power off by opening the battery lid and removing the battery).

Bit1=1 disables the wifi clock, and the clock remains off even after reboot via
MCU[20h] (to avoid that, best set MCU[3Fh]=00h before and after reboot).

**Legacy RTC Emulation**

The 3DS can emulate GBA/NDS/DSi RTC's. That is, the 3DS software must read the
battery-backed time from MCU RTC registers, and then copy it to the emulated
RTC registers...

NDS/DSi Console RTC:

3DS GPIO Registers

GBA Cartridge RTC:

3DS Config - ARM7 Registers (GBA/NDS/DSi Mode)