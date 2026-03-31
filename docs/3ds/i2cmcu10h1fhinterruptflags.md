# I2Cmcu10H1Fhinterruptflags

> Source: https://problemkaputt.de/gbatek.htm
> Section: I2Cmcu10H1Fhinterruptflags

3DS I2C MCU[10h-1Fh] - Interrupt Flags 
| |
| --- |

 **MCU[10h-13h] - MCU Interrupt Flags, bit0-31 (1=IRQ, cleared after read) (R)**
 **MCU[18h-1Bh] - MCU Interrupt Mask, bit0-31 (0=Enable, 1=Disable) (R/W)**
 
```

| 0 Power button press (for 27 "ticks") aka 0.2s
 1 Power button held (for 375 "ticks" aka 3s, turns off after another 12s)
 2 HOME button press (for 5 "ticks") aka 0.04s
 3 HOME button release
 4 Wifi button press (for 5 "ticks") aka 0.04s
 5 Shell close
 6 Shell open
 7 Fatal hardware condition? (sent when MCU gets reset by Watchdog timer)
 8 Charger removed
 9 Charger plugged in
 10 RTC alarm
 11 Accelerometer I2C manual read/write done
 12 Accelerometer new XYZ sample update
 13 Battery dropped below 11%, 6% or 1% (warns at those 3 points)
 14 Battery charging stop (independent of charger state)
 15 Battery charging start
 16 TSC[10h].bit0=1=Whatever, or BPTWL[11h]=01h=Reset
 17 TSC[10h].bit6=1=Whateverelse
 18 TSC[10h].bit2 changed to zero
 19 TSC[10h].bit2 changed to one
 20 TSC[10h].bit3 changed to zero
 21 TSC[10h].bit3 changed to one
 22 Volume slider position change
 23 Read from BPTWL[00h] version register has occurred
 24 Video LCD "push" Power Supplies off
 25 Video LCD "push" Power Supplies on
 26 Video Bottom screen backlight off
 27 Video Bottom screen backlight on
 28 Video Top screen backlight off
 29 Video Top screen backlight on
 30 set by mcu sysmodule ;\uh, flag bits aren't set by MCU (and can't be
 31 set by mcu sysmodule ;/set by ARM), but maybe ARM sets mask bits..? |
| --- |