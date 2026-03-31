# I2Cnew16Bitioexpanderakaqtm

> Source: https://problemkaputt.de/gbatek.htm
> Section: I2Cnew16Bitioexpanderakaqtm

3DS I2C New3DS 16bit IO Expander (aka QTM) 
| |
| --- |

 **TCA6416A 16bit IO Expander I2C Registers (I2C Bus/Device: 0:40h)**
 
```

| 00h Input Port 0 (R) (reset: var, reads as 05h on New3DS)
 01h Input Port 1 (R) (reset: var, reads as 00h on New3DS)
 02h Output Port 0 (R/W) (reset: FFh)
 03h Output Port 1 (R/W) (reset: FFh)
 04h Input Polarity Inversion 0 (R/W) (reset: 00h=normal)
 05h Input Polarity Inversion 1 (R/W) (reset: 00h=normal)
 06h Configuration (aka Direction) 0 (R/W) (reset: FFh=input)
 07h Configuration (aka Direction) 1 (R/W) (reset: FFh=input)
 08h..FFh Reserved (-) (FFh's) |
| --- |

**Port Usage in New3DS**

Unknown. The IO Expander chip is close to the top-screen connectors, as such it
does probably connect to components in the top-screen unit (which might include
upper camera; or reportedly-existing head-tracking related stuff).

Unknown how a 16bit IO could do head-tracking, maybe it does opposite: receive
the tracking RESULT from ARM side, and then OUTPUT something to parallax
barrier?

Unknown if this can be reverse-engineered by people with broken top-screen
connectors. Note: Below "usually set/cleared" assumes that the connector for
upper backlight is broken, unknown if an intact console behaves the same.

```

| P0.0 Unknown (usually set) ;DANGER: power-off when set to Output/Low
 P0.1 Unknown (usually 0)
 P0.2 Unknown (usually set) ;DANGER: power-off when set to Output/Low
 P0.3 Unknown (usually 0)
 P0.4 Unknown (usually 0)
 P0.5 Unknown (usually 0)
 P0.6 Unknown (usually 0)
 P0.7 Unknown (usually 0)
 P1.0 Unknown (usually 0)
 P1.1 Unknown (usually 0)
 P1.2 Unknown (usually 0)
 P1.3 Unknown (usually 0)
 P1.4 Unknown (usually 0)
 P1.5 Unknown (usually 0)
 P1.6 Unknown (usually 0)
 P1.7 Unknown (usually 0) |
| --- |

**00h - Input Port 0 (reset: var, reads as 05h on New3DS) (R)**

**01h - Input Port 1 (reset: var, reads as 00h on New3DS) (R)**

```

| 0-7 Input level (0=Low, 1=High; or vice-versa when using Polarity Inversion) |
| --- |

**02h - Output Port 0 (reset: FFh) (R/W)**

**03h - Output Port 1 (reset: FFh) (R/W)**

```

| 0-7 Output level (0=Low, 1=High) (no effect if direction is Input) |
| --- |

**04h - Polarity Inversion 0 (reset: 00h) (R/W)**

**05h - Polarity Inversion 1 (reset: 00h) (R/W)**

```

| 0-7 Polarity of Inputs (0=Normal, 1=Invert) |
| --- |

**06h - Configuration 0 (reset: FFh) (R/W)**

**07h - Configuration 1 (reset: FFh) (R/W)**

```

| 0-7 Direction (0=Output, 1=Input) |
| --- |

**Interrupt Pin**

The IO Expander can trigger interrupts on any changes on any input pins.
Unknown if the Interrupt signal is connected in New3DS.