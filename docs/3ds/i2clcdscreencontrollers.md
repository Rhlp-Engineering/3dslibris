# I2Clcdscreencontrollers

> Source: https://problemkaputt.de/gbatek.htm
> Section: I2Clcdscreencontrollers

3DS I2C LCD Screen Controllers 
| |
| --- |

 **I2C Bus/Device 1:2Ch - Upper LCD screen (lcd0)**
 **I2C Bus/Device 1:2Eh - Lower LCD screen (lcd1)**
 LCD controllers for top/bottom screen.
 
 **I2C Access**
 I2C writing works as for most other I2C chips:
 
```

| I2cWriteCmd (Device+0, Index, Data) |
| --- |

However, I2C reading works uncommon:

```

| I2cWriteCmd (Device+0, 40h, ReadIndex)
 I2cReadCmd (Device+1, ReadIndex, Data) |
| --- |

Ie. the LCD chips distinguish between WriteIndex and ReadIndex, with the
ReadIndex being set by writing to register 40h. The ReadIndex value in the
response can be ignored (or verified to be same as the written ReadIndex
value).

**I2C Registers**

```

| 00h Zero (-)
 01h Display on/off (10h=Display on, 11h=Display black) ;bit0,4 is R/W
 02h Usually 01h, but bit0 is R/W
 03h Usually 00h, but bit0-7 is R/W
 04h..10h Zero (-)
 11h Whatever (set to 10h during init, maybe bit4=reset?)
 12h..3Fh Zero (-)
 40h Read Index for I2C read (00h..FFh)
 41h..4Fh Zero (-)
 50h Bit0 is R/W (set to 1 for top-screen; left 0 on bottom-screen)
 51h..53h Zero (-)
 54h Usually 00h, but bit0 is R/W
 55h Usually 00h, but bit0-1 is R/W
 56h..5Fh Zero (-)
 60h Whatever (set to 00h during init) ;bit0 is R/W ?
 61h Status (initially B4h=Top Screen, F8h=Bottom Screen) (R?)
 62h Status (01h=Ready) (except, broken top screen reads 00h?) (R?)
 63h Zero (-)
 64h Whatever/Fixed 03h (R)
 65h..AEh Zero (-)
 AFh Usually 00h, but bit0-7 is R/W
 B0h..D3h Zero (-)
 D4h Usually 00h, but lower 4bit are R/W
 D5h Usually 00h, but bit0-7 is R/W
 D6h..FDh Zero (-)
 FEh Usually 00h, but bit0-7 is R/W
 FFh Maybe Chip ID (10h=Top Screen, C7h=Bottom Screen) (R) |
| --- |

Odd effect observed during testing: Setting LCD[AFh]=AAh causes MANY more
registers to become nonzero; this doesn't happen for other values like
LCD[AFh]=0Fh,F0h,FFh.

```

| MIPI DSI |
| --- |

```

| // LCD_I2C_REG_POWER - 01h
 #define LCD_REG_POWER_BLACK (0x11u) // Force blackscreen.
 #define LCD_REG_POWER_ON (0x10u) // Normal operation.
 #define LCD_REG_POWER_OFF (0x00u) // LCD powered off. |
| --- |

```

| // LCD_I2C_REG_UNK11 - 11h
 #define LCD_REG_UNK11_UNK10 (0x10u) // Written on init. |
| --- |

```

| // LCD_I2C_REG_HS_SERIAL - 50h ;Highspeed serial, upper LCD only
 #define LCD_REG_HS_SERIAL_ON (0x01u) // Enable highspeed serial. |
| --- |

```

| // LCD_I2C_REG_UNK54 - 54h ;Checksum on/off?
 // LCD_I2C_REG_UNK55 - 55h ;Checksum status? |
| --- |

```

| // LCD_I2C_REG_STATUS - 60h ;Initially 0x01.
 #define LCD_REG_STATUS_OK (0x00u)
 #define LCD_REG_STATUS_ERR (0x01u) |
| --- |

```

| // LCD_I2C_REG_BL_STATUS - 62h ;Backlight status.
 #define LCD_REG_BL_STATUS_OFF (0x00u)
 #define LCD_REG_BL_STATUS_ON (0x01u) |
| --- |

```

| // LCD_I2C_REG_RST_STATUS - FEh ;Reset status. Initially 0x00.
 #define LCD_REG_RST_STATUS_NONE (0xAAu)
 #define LCD_REG_RST_STATUS_RST (0x00u) |
| --- |

```

| // LCD_I2C_REG_REVISION - FFh ;Revision/vendor infos. |
| --- |

**bootrom error screen init sequence for the I2C registers:**

```

| LCD[11h]=10h ;whatever
 LCD[50h]=01h ;whatever, this one done for TOP screen only
 LCD[60h]=00h ;whatever
 LCD[01h]=10h/11h ;on/off (10h=display on, 11h=display black) |
| --- |

The bootrom additionally checks if status LCD[62h]=01h=Ready.