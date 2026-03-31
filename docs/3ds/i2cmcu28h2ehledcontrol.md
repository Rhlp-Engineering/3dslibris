# I2Cmcu28H2Ehledcontrol

> Source: https://problemkaputt.de/gbatek.htm
> Section: I2Cmcu28H2Ehledcontrol

3DS I2C MCU[28h-2Eh] - LED Control 
| |
| --- |

 **MCU[28h] - Brightness of Wifi/Power/3D LEDs (R/W)**
 Brightness for Wifi/Power/3D LEDs (when the LEDs are on). Range 00h..FFh (default is FFh=max).
 
 **MCU[29h] - Power LED state + Power LED blink pattern ;ARRAY[5] (R/W)**
 1st byte (Power LED mode):
 
```

| 00h = Fade to brightness MCU[28h] with battery check
 01h = Fade to brightness MCU[28h]
 02h = Pulsating fade on/off with battery check
 03h = Fade to brightness 00h
 04h = Instantly set brightness 00h
 05h = Instantly set brightness FFh
 06h = Blinking RED (affects Power+Notification LEDs)
 Other = Invalid (same as 00h) |
| --- |

2nd..5th bytes (optional, if any written):

```

| Power LED Blink pattern (default is 55h,55h,55h,55h) |
| --- |

**MCU[2Ah] - Wifi LED state, 4 bits wide (R/W)**

```

| 00h = Wifi LED always off
 01h..0Fh = Wifi LED on (and blink upon traffic?) |
| --- |

**MCU[2Bh] - Camera LED state, 4bits wide (R/W)**

```

| 00h = Camera LED always off
 01h = Camera LED slowly blinking
 02h = Camera LED always on
 03h = Camera LED set via BPTWL[31h] (DSi mode)
 04h = Camera LED flash once (and then switch to 00h=off)
 05h = Camera LED off once (and then switch to 02h=on)
 06h..0Fh = Invalid (same as 00h) |
| --- |

Camera LED exists only in older 3DS, not in New3DS. The camera LED uses a
digital 1bit signal (without variable brightness).

**MCU[2Ch] - 3D LED state, 4 bits wide (R/W)**

```

| 00h = 3D LED Off (or fade-out to zero)
 01h = 3D LED On (or fade-in/out to MCU[28h] setting)
 02h..0Fh = Same as 01h (On) |
| --- |

3D LED exists only in older 3DS, not in New3DS.

**MCU[2Dh] - Notification LED Array (4+3x20h bytes) ;ARRAY[64h] (W)**

```

| [2Dh.00h] ;Delay (0..FFh = Delay 1..100h)
 [2Dh.01h] ;Brightness/divider or so?
 [2Dh.02h] ;Speed, some timer compare value? (FFh=none?)
 [2Dh.03h] ;unused
 [2Dh.04h..23h] ;data Red[0..1Fh]
 [2Dh.24h..43h] ;data Green[0..1Fh]
 [2Dh.44h..63h] ;data Blue[0..1Fh] |
| --- |

"It's possible to write data here with size less than 64h, and only that
portion of the pattern data will get overwritten. Writing past the size of this
register seems to do nothing. Reading from this register only returns zeroes."

**MCU[2Eh] - Notification LED Status when read (1=new cycle started) (R)**

```

| 0 Flag
 1-7 Not used |
| --- |

**Initial LED States**

The LED states are uninitialzed at warmboot (eg. after closing System Settings,
firm0 may boot with 3D LED kept on). All LEDs are automatically fading to zero
upon power off (for faster power-off, use MCU[29h]=4 before MCU[20h]=1). At
coldboot, firm0 is started with only Power LED switched on.

Regardless of the above, the Charge LED can be on/off at any time (it is on
when battery isn't fully charged & charger is connected).