# Dsigpioregisters

> Source: https://problemkaputt.de/gbatek.htm
> Section: Dsigpioregisters

DSi GPIO Registers 
| |
| --- |

 **4004C00h DSi7 - GPIO Data In (R) (even in DS mode)**
 
```

| 0 GPIO18[0] ;\maybe 1.8V signals? (1=normal)
 1 GPIO18[1] ; (maybe these are the three "NC" pins on CPU,
 2 GPIO18[2] ;/near to the other GPIO pins)
 3 Unused (0)
 4 GPIO33[0] Testpoint on mainboard (GPIO330) (0=GNDed, 1=Normal)
 5 GPIO33[1] Headphone connect (HP#SP) (0=None, 1=Connected)
 6 GPIO33[2] Powerbutton interrupt (IRQ_O) (0=Short Pulse, 1=Normal)
 7 GPIO33[3] Sound Enable Output (ie. not a useful input) |
| --- |

One of the above is probably the "IRQ_O" signal on mainboard (possibly the
"power button" bit; if so, then that bit might be some general interrupt from
the "BPTWL" chip, rather than being solely related to the power button).

Bit0-2 might be unused "NC" pins. Bit4 is the GPIO330 testpoint, which seems to
have no other connection. Bit7 is the via GPIO330, and goes to Headphone Mute
circuit and TSC chip (weirdly, the cooking coach cart sets interrupt edge
select bit7; the interrupt isn't actually enabled though).

Some bits seem to be floating high-z (when switching from output/low to input
they won't <instantly> get high).

**4004C00h DSi7 - GPIO Data Out (W)**

```

| 0-2 GPIO18[0-2] Data Output (0=Low, 1=High)
 3 Unused (0)
 4-7 GPIO33[0-3] Data Output (0=Low, 1=High) |
| --- |

Used only when below is set to direction=out. Should be 80h (sound enable).

**4004C01h DSi7 - GPIO Data Direction (R/W)**

```

| 0-2 GPIO18[0-2] Data Direction (0=Normal/Input, 1=Output)
 3 Unused (0)
 4-7 GPIO33[0-3] Data Direction (0=Normal/Input, 1=Output) |
| --- |

Should be usually set to 80h (bit0-6=Input, bit7=Output). When using output
direction, the "Data In" register will return the "Data Out" value ANDed with
external inputs.

Observe that HP#SP could be used as Output (output/low will probably cause the
DSi to believe that there is no headphone connected, thus forcing the internal
speakers to be enabled; possibly with/without disabling the headphones?).

**4004C02h DSi7 - GPIO Interrupt Edge Select (R/W)**

```

| 0-2 GPIO18[0-2] Interrupt Edge Select (0=Falling, 1=Rising)
 3 Unused (0)
 4-7 GPIO33[0-3] Interrupt Edge Select (0=Falling, 1=Rising) |
| --- |

**4004C03h DSi7 - GPIO Interrupt Enable (R/W)**

```

| 0-2 GPIO18[0-2] Interrupt Enable (0=Disable, 1=Enable)
 3 Unused (0)
 4-7 GPIO33[0-3] Interrupt Enable (0=Disable, 1=Enable) |
| --- |

**4004C04h - DSi7 - GPIO_WIFI (R/W)**

```

| 0 Unknown (firmware keeps this bit unchanged when writing)
 1-7 Zero
 8 Wifi Mode (0=New Atheros/DSi-Wifi mode, 1=Old NDS-Wifi mode)
 9-15 Zero |
| --- |

Bit8 must be properly configured for DWM-W024 wifi boards (must be 1 for
NDS-Wifi mode; else W_US_COUNT hangs) (and must be 0 for DSi-Wifi; else SDIO
Function 1 commands won't work).

DWM-W015 boards seem to work fine regardless of that bit (unknown if the bit
does have any (minor) effects on that boards at all).