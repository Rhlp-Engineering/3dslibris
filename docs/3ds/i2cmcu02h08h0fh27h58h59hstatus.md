# I2Cmcu02H08H0Fh27H58H59Hstatus

> Source: https://problemkaputt.de/gbatek.htm
> Section: I2Cmcu02H08H0Fh27H58H59Hstatus

3DS I2C MCU[02h, 08h-0Fh, 27h, 58h-59h] - Status 
| |
| --- |

 **MCU[02h] - Reset Event flags (R/W)**
 
```

| 0 RTC time lost (0=Normal, 1=Lost) (write 0 to clear) (R/ack)
 1 Watchdog reset happened (0=Normal, 1=Crash) (write 0 to clear) (R/ack)
 2-4 Unused (0)
 5 Value from BPTWL[12h].bit7 (Unknown) (R)
 6 Value from BPTWL[12h].bit0 (1=IRQ on Pwr Butt tap) (R)
 7 Value from BPTWL[12h].bit1 (Unknown) (R) |
| --- |

**MCU[08h] - Raw 3D slider position (R)**

```

| 0-7 3D slider raw ADC value (00h..FFh) |
| --- |

This the uncalibrated raw ADC value, calibration must be applied on ARM side
(using calibration settings from HWCAL or "config" files).

**MCU[09h] - Volume slider state (00h..3Fh) (as MCUHWC:GetSoundVolume) (R)**

```

| 0-7 Volume slider calibrated value (00h..3Fh, 0=Mute, 3Fh=Max) |
| --- |

See MCU[27h] for the raw ADC value, and MCU[58h,59h] for calibration values.

**MCU[0Ah] - Battery PCB Temperature Celsius (signed) (R)**

```

| 0-7 Degrees Celsius (51F3h-(ADC(8)*70h))/100h ;signed, usually 17h..19h |
| --- |

**MCU[0Bh] - Fuel Gauge Battery Percentage, msb (percent, 0..64h) (R)**

**MCU[0Ch] - Fuel Gauge Battery Percentage, lsb (percent/256, 0..FFh) (R)**

```

| 0-15 Battery percent/256, from Fuel Gauge SOC register (State Of Charge) |
| --- |

**MCU[0Dh] - Fuel Gauge Battery Voltage (in 20mV units) (R)**

```

| 0-7 Battery voltage, from upper 8bit of Fuel Gauge VCELL register |
| --- |

**MCU[0Eh] - Sub-Device Access Flags? (bit0,1,2-3,4) (R)**

```

| 0 Device 6Ch Error (Fuel Gauge) (0=Okay, 1=Error)
 1 Device 30h Error (Accelerometer) (0=Okay, 1=Error)
 2-3 Device A4h related (Volume etc.) ;TSC[10h]
 4 LED brightness related
 5-7 Unused |
| --- |

**MCU[0Fh] - Power Status Flags (R)**

```

| 0 Unused
 1 ShellState (hinge) (1=Shell open) ;\
 2 Unused ; mcu::RTC
 3 AdapterState (Charger connected) ;
 4 BatteryChargeState (1=Charging) ;/
 5 OldMCU: Unused ;\
 6 OldMCU: something (1bit) ;\back- ; mcu::GPU
 5-6 NewMCU: something (2bit) ;/light(s)? ;
 7 whatever ... VCOM? ;-"push"? ;/ |
| --- |

**MCU[27h] - Volume slider raw ADC value (R/W, but better treat as R)**

```

| 0-7 Volume slider raw ADC value (00h..FFh) |
| --- |

**MCU[58h] - Volume slider calibration point for 0% (default=36h) (R/W)**

**MCU[59h] - Volume slider calibration point for 100% (default=C9h) (R/W)**

```

| 0-7 Volume slider calibration value |
| --- |

The MCU defaults to min=36h, max=C9h. Better calibration values can be found in
HWCAL and "config" files. Observe that one should add/sub 8 to the file values
as safety margin (eg. for file values=04h,FFh write 0Ch,F7h to MCU[58h,59h]).
The MCU does need the calibration for forwarding the correct volume to TSC chip
(and for adjusting MCU[09h]).

**MCU[5Ah] - R/RW Invalid, do not use!**

on newer MCU_FIRM versions this is set to FFh once and then

on older MCU_FIRM versions this is a read-only counter