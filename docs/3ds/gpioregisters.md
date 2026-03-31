# Gpioregisters

> Source: https://problemkaputt.de/gbatek.htm
> Section: Gpioregisters

3DS GPIO Registers 
| |
| --- |

 
```

| _______________________________ GPIO Registers _______________________________ |
| --- |

**GPIO Registers**

```

| 10147000h 2 GPIO_DATA0 IN ;GPIO services bitmask 7h
 10147010h 4x1 GPIO_DATA1 IN/OUT ;GPIO services bitmask 18h
 10147014h 2 GPIO_DATA2 OUT ;GPIO services bitmask 20h
 10147020h 4x2 GPIO_DATA3 IN/OUT ;GPIO services bitmask 3FFC0h
 10147028h 2 GPIO_DATA4_WIFI OUT ;GPIO services bitmask 40000h
 10147100h .. Legacy RTC (for RTC emulation in NDS/DSi mode) |
| --- |

**10147000h - GPIO_DATA0_DATA_IN (R?)**

```

| 0 Unknown (0=?, 1=Normal) HID-sysmodule, HID PAD state ;DEBUG Button?
 1 TP125 Touchscreen (0=Pen Down, 1=No) (IRQ:63h)
 2 TP126 Hinge (0=Shell Open, 1=Shell Closed) (IRQ:60h/62h)
 3 Unused (0)
 4 Unused (0) ;"Only used by Boot11" (uh, but bootrom tests only bit2)
 5-15 Unused (0) |
| --- |

**10147010h - GPIO_DATA1_DATA_IN (0=Low, 1=High) (R)**

**10147010h - GPIO_DATA1_DATA_OUT (0=Low, 1=High) (W)**

**10147011h - GPIO_DATA1_DIRECTION (0=Input, 1=Output) (R/W)**

**10147012h - GPIO_DATA1_IRQ_EDGE (0=Falling, 1=Rising) (R/W)**

**10147013h - GPIO_DATA1_IRQ_ENABLE (0=Disable, 1=Enable) (R/W)**

```

| 0 TP49-Q3-CL20 Headphone (0=None, 1=Connected) gpio:CDC (IRQ:64h)
 1 Unknown (0=?, 1=Normal) ;TWL Depop ? (IRQ:66h)
 2-7 Unused (0) |
| --- |

**10147014h - GPIO_DATA2_DATA_OUT (R/W)**

```

| 0 gpio:MCU, gpio:NWM: Wifi?
 1-15 Unused (0) |
| --- |

Reportedly "WiFi mode/freq. select (0 = CTR, 1 = MP (DS WiFi)" or so?

**10147020h - GPIO_DATA3_DATA_IN (0=Low, 1=High) (R)**

**10147020h - GPIO_DATA3_DATA_OUT (0=Low, 1=High) (W)**

**10147022h - GPIO_DATA3_DIRECTION (0=Normal/Input, 1=Output) (R/W)**

**10147024h - GPIO_DATA3_IRQ_EDGE (0=Falling, 1=Rising) (R/W) ?**

**10147026h - GPIO_DATA3_IRQ_ENABLE (0=Disable, 1=Enable) (R/W)**

Most of these might be IRQ signals from I2C peripherals?

Stuff like QTM, NFC, ir:rst is New3DS only, so Old3DS is probably different
here?

```

| 0 TP153 C-stick gpio:CDC, gpio:IR <-- 1 after ir:rst read(IRQ:68h?)
 1 TP239 IrDA IRQ gpio:IR (0=IRQ) Boot11(uh, really?) (IRQ:69h)
 2 TP158 Gyro IRQ gpio:HID (1=IRQ, in default cfg) (IRQ:6Ah?)
 3 TP154 ? gpio:HID, gpio:IR used with ir:rst. (IRQ:6Bh)
 4 TP141 IrDA TX-RC gpio:IR send? 1=IR LED enable, 0=disable (IRQ:6Ch)
 5 TP255 IrDA RXD gpio:IR receive? (IRQ:6Dh)
 6 - ? gpio:NFC (IRQ:6Eh)
 7 TP155 ? gpio:NFC (IRQ:6Fh)
 8 TP156 TSC? gpio:HID HID-sysmodule, HID PAD state (IRQ:70h?)
 9 TP157 MCU IRQ gpio:MCU MCU[10h-1Fh] (IRQ:71h)
 10 - NFC? gpio:NFC <-- cleared after NFC read (IRQ:72h)
 11 - ? gpio:QTM (Twlbg? and/or New3DS?) (IRQ:73h?)
 12-15 Unused (0) |
| --- |

Note: Bootrom uses bit7 and bit10 to configure some LCD/backlight hardware for
pre-Old3DS prototypes (when Port 10140FFCh CFG11_SOCINFO bit0=0).

**10147028h - GPIO_DATA4_DATA_OUT_WIFI (R/W)**

```

| 0 Wifi Enable (0=Reset, need re-upload wifi firmware, 1=On)
 1-15 Unused (0) |
| --- |

Probably wired to RESET or SYS_RST_L pin on wifi board connector?

**Default GPIO values**

After bootrom initialization, these are the values of the registers:

```

| Address Value
 10147000h 0003h
 10147010h 00000002h
 10147014h 0000h
 10147020h 00000DFBh
 10147024h 00000000h
 10147028h 0000h |
| --- |

**10147100h - Legacy RTC (for RTC emulation in NDS/DSi mode)**

Normally only 10147100h.bit15 is R/W, and the other bits/registers at 101471xxh
are all zero. But reportedly they can contain this...

```
****
| 10147100h 2 RTC_CNT Control register (see below)
 Caution: Below are all bytewise bitswapped (swap bit7<-->bit0, etc)
 10147110h 1 RTC_REG_STAT1 Rtc status register 1 ;-STAT1 (0)
 10147111h 1 RTC_REG_STAT2 Rtc status register 2 ;-STAT2 (1)
 10147112h 1 RTC_REG_CLKADJ Rtc clock adjustment register ;-CLKADJ(6)
 10147113h 1 RTC_REG_FREE Free general purpose register ;-FREE (7)
 10147114h 0Ch Unused
 10147120h 1 RTC_REG_TIME_SECOND Second ;\
 10147121h 1 RTC_REG_TIME_MINUTE Minute ;
 10147122h 1 RTC_REG_TIME_HOUR Hour ;
 10147123h 1 RTC_REG_TIME_DOW Day of week ; TIME
 10147124h 1 RTC_REG_TIME_DAY Day ;
 10147125h 1 RTC_REG_TIME_MONTH Month ;
 10147126h 1 RTC_REG_TIME_YEAR Year ;/
 10147127h 09h Unused
 10147130h 1 RTC_REG_ALARM1_MINUTE Alarm 1 Minute ;\
 10147131h 1 RTC_REG_ALARM1_HOUR Alarm 1 Hour ; ALRMTIM1 (4)
 10147132h 1 RTC_REG_ALARM1_DOW Alarm 1 Day of week ;/
 10147133h 01h Unused
 10147134h 1 RTC_REG_ALARM2_MINUTE Alarm 2 Minute ;\
 10147135h 1 RTC_REG_ALARM2_HOUR Alarm 2 Hour ; ALRMTIM2 (5)
 10147136h 1 RTC_REG_ALARM2_DOW Alarm 2 Day of week ;/
 10147137h 09h Unused
 Below are for DSi, unknown if they are used/enabled for NDS, too...
 10147140h 1 RTC_REG_COUNT_LSB Rtc DSi Counter LSB? ;\
 10147141h 1 RTC_REG_COUNT_MID Rtc DSi Counter MID ; COUNT (ex0)
 10147142h 1 RTC_REG_COUNT_MSB Rtc DSi Counter MSB? ;/
 10147143h 0Dh Unused
 10147150h 1 RTC_REG_FOUT1 Rtc DSi Fout register 1 ;-FOUT1 (ex1)
 10147151h 1 RTC_REG_FOUT2 Rtc DSi Fout register 2 ;-FOUT2 (ex2)
 10147152h 0Eh Unused
 10147160h 1 RTC_REG_ALARM1_DAY Rtc DSi Alarm 1 Day ;\
 10147161h 1 RTC_REG_ALARM1_MONTH Rtc DSi Alarm 1 Month ; ALRMDAT1 (ex4)
 10147162h 1 RTC_REG_ALARM1_YEAR Rtc DSi Alarm 1 Year ;/
 10147163h 01h Unused
 10147164h 1 RTC_REG_ALARM2_DAY Rtc DSi Alarm 2 Day ;\
 10147165h 1 RTC_REG_ALARM2_MONTH Rtc DSi Alarm 2 Month ; ALRMDAT2 (ex5)
 10147166h 1 RTC_REG_ALARM2_YEAR Rtc DSi Alarm 2 Year ;/
 10147167h .. Unused |
| --- |

These registers are for initializing the NDS/DSi RTC (Port 4000138h).

DS Real-Time Clock (RTC)

The registers can be used to set the time/date... but unknown if/how the
hardware can increment the time (it doesn't do so automatically, unless there's
a separate CLK enable flag somewhere, or one might be to supposed to increment
it manually once per second from ARM11 side??) (the GBA mode RTC does increment
seconds without problems).

For GBA Cart RTC see "ARM7_RTC_xxx" registers at 100181xxh.

**10147100h - RTC_CNT**

```

| 0 Latch STAT1 (0=No change, 1=Apply) (W)
 1 Latch STAT2 (0=No change, 1=Apply) (W)
 2 Latch CLKADJ (0=No change, 1=Apply) (W)
 3 Latch FREE (0=No change, 1=Apply) (W)
 4 Latch TIME (0=No change, 1=Apply) (W)
 5 Latch ALRMTIM1 (0=No change, 1=Apply) (W)
 6 Latch ALRMTIM2 (0=No change, 1=Apply) (W)
 7 Latch COUNT (0=No change, 1=Apply) (W)
 8 Latch FOUT1 (0=No change, 1=Apply) (W)
 9 Latch FOUT2 (0=No change, 1=Apply) (W)
 10 Latch ALRMDAT1 (0=No change, 1=Apply) (W)
 11 Latch ALRMDAT2 (0=No change, 1=Apply) (W)
 12 ARM7 Busy? This may be chipselect (R)
 13 ARM7 write command received? (writing 1 clears it seems) (R/ack)
 14 ARM7 read command received? (writing 1 clears it seems) (R/ack)
 15 DS SIO SI pin (rtc irq pin) (R/W) |
| --- |

Bit0-11 are used to forward the settings from 10147110h-10147166h to the actual
NDS RTC registers.

Unknown why RTC IRQ pin is write-able, maybe ARM11 must manually trigger it?
But if the ALARM registers are write-only then ARM11 couldn't know their
current value (except, by assuming that the written value wasn't changed from
NDS side).