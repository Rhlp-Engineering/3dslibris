# I2Cmcu60H7Fhmiscstatus

> Source: https://problemkaputt.de/gbatek.htm
> Section: I2Cmcu60H7Fhmiscstatus

3DS I2C MCU[60h-7Fh] - Misc Status 
| |
| --- |

 **MCU[60h] - Battery-backed RAM Index (R/W)**
 **MCU[61h] - Battery-backed RAM Data (200 bytes) ;ARRAY[C8h] (R/W)**
 This is a battery-backed 200-byte general purpose RAM area. The MCU itself doesn't care about the RAM content. Instead, the ARM CPU can use it to store powerdown/error flags (and then check those flags on next boot).
 
```

| The 1st byte at array[00h] is used to store flags for managing FIRM/NS state:
 bit0 = "WirelessDisabled" (0=Enable, 1=Disable) (see Wifi button)
 bit1 = "SoftwareClosed"
 bit2 = "PowerOffInitiated"
 bit4 = "LegacyJumpProhibited"
 The following 199 bytes are:
 Unknown, usually zerofilled (maybe contain error info/warmboot params?) |
| --- |

When exceeding the C8h-byte array size: Writing to index=C8h..FFh is ignored
(and the index is not incremented). Reading from index=C8h..FFh returns
data=00h (and the index is incremented, and can wrap from index=FFh to
index=00h).

**MCU[7Fh] - Various system state information ;ARRAY[09h/13h] (R)**

```

| [7Fh:00h] Value 00h..06h (console model? critical_hw_state?) (usually 00h ?)
 [7Fh:01h] Powerman Version (00h=normal) (from POW[00h])
 [7Fh:02h] battery scheme? (0..7, or FFh) (maybe from middle-batt-pin?)
 [7Fh:03h] Fuel Gauge Version.msb (00h) ;FUEL[02h]
 [7Fh:04h] Fuel Gauge Version.lsb (12h) ;FUEL[03h]
 [7Fh:05h] Fuel Gauge Config.msb RCOMP (5Eh) ;FUEL[0Ch]
 [7Fh:06h] Battery PCB Temperature raw ADC(8) value; see MCU[0Ah] for celsius
 [7Fh:07h] Battery PCB Temperature flags (bit0=0=Bad, bit1=0=VeryBad?)
 [7Fh:08h] Fixed 01h on New3DS-XL (older 3DS can be 00h or 01h = what?)
 On MCU_FIRM major version 0:
 [7Fh:09h and up] unknown
 On MCU_FIRM major version 1:
 [7Fh:09h and up] unused (AAh)
 On MCU_FIRM major version 2 and up (or so):
 [7Fh:09h] Sys Model (0=3ds, 1=3dsXL, 2=New3ds, 3=2ds, 4=New3dsXL, 5=New2dsXL)
 [7Fh:0Ah] Power LED color (0=Blue/off, 1=Red)
 [7Fh:0Bh] Power LED intensity (00h..FFh)
 [7Fh:0Ch] 3D LED intensity (00h..FFh)
 [7Fh:0Dh] RGB LED red intensity (00h..FFh)
 [7Fh:0Eh] RGB LED green intensity (00h..FFh)
 [7Fh:0Fh] RGB LED blue intensity (00h..FFh)
 [7Fh:10h] Camera LED state (0=Off, 1=On)
 [7Fh:11h] Wifi LED intensity (00h..FFh)
 [7Fh:12h] Raw button states
 bit0: Power button (0=Pressed)
 bit1: Home button (0=Pressed)
 bit2: Wifi button (0=Pressed) (installed in Old3DS only)
 bit5: Charger LED (0=LED On, 1=LED Off) ;\or vice-versa?
 bit6: Charger connected AND busy (0=Busy, 1=No) ;/or one just "connected?"
 this byte is reset to 0 before an svcBreak takes effect, uh?
 [7Fh:13h and up] unused (FFh) |
| --- |

**Wifi button**

The Wifi button does trigger an MCU interrupt, but doesn't automatically
enable/disable the Wifi LED, Wifi Disable bit, or other Wifi Hardware. Instead,
the ARM software should process the Wifi button IRQ as so:

```

| - Toggle the Wifi Disabled flag in battery backed MCU[61h:00h].bit0
 - Enable/disable the Wifi LED in MCU[2Ah] accordingly
 - Enable/disable the overall Wifi hardware accordingly |
| --- |

The Wifi button exists only in Old3DS (but New3DS does reportedly have a
similar option... in System Settings or so?).