# I2Cnewcstickandzlzrbuttons

> Source: https://problemkaputt.de/gbatek.htm
> Section: I2Cnewcstickandzlzrbuttons

3DS I2C New3DS C-Stick and ZL/ZR-Buttons 
| |
| --- |

 **I2C Bus/Device 2:54h - C-Stick and ZL/ZR-Buttons**
 This is a New3DS-only device (the Old3DS does have a similar add-on: Circle Pad Pro, connected to the infrared expansion port).
 Oberseve that the bootrom doesn't initialize I2C_BUS2_CNTEX/SCL (especially CNTEX is required to have "Wait if SCL held low" enabled, else the C-stick replies are garbage).
 The chip triggers ARM11 IRQ 68h (aka GPIO_DATA3.bit0=0) when moving the analog input or buttons. Reading the I2C data changes GPIO back to bit0=1.
 Maybe this is the HUGE 44pin HF374 chip on button board, but if it's merely used for ZL/ZR and C-stick, why is that chip so huge?
 
 **I2C Read(Device+1,Byte0,Byte1,Byte2,Byte3,...)**
 Reading returns some kind of array, always starting with the status value in byte0, there is no need to write an index value before reading.
 The array has useful info stored in first 4-7 bytes; and internal/garbage when reading further bytes, up to including something that looks like a CPU stack).
 
```

| The array entries are...
 00h Status byte (80h..83h, or FFh) (power-up default=80h)
 01h Button byte (00h=None, bit2=ZL, ?=ZR)
 02h Analog X (00h=Center, -1xh=Left, +1xh=Right) ;\if enabled
 03h Analog Y (00h=Center, -1xh=Down, +1xh=Up) ;/
 Following bytes aren't needed, except for better 8bit X/Y resolution...
 04h Fixed FFh
 05h Analog X (00h=Center, -7xh=Left, +7xh=Right) ;\hires
 06h Analog Y (00h=Center, -7xh=Down, +7xh=Up) ;/
 Following bytes aren't actually useful...
 07h Fixed 00h
 08h Analog X (7xh=Center, FEh=Left, 00h=Right) ;\unsigned/uncentered
 09h Analog Y (7xh=Center, FEh=Down, 00h=Up) ;/
 0Ah Fixed 00h
 0Bh Analog X (7xh=Center, FEh=Left, 00h=Right) ;\same as [08h,09h]
 0Ch Analog Y (7xh=Center, FEh=Down, 00h=Up) ;/
 0Dh Fixed 00h
 0Eh Center X (7xh) ;\auto-calibrating, with minor changes
 0Fh Center Y (7xh) ;/every some seconds
 10h Fixed 00h
 11h..15h Analog stuff
 16h Flag (00h=Idle, 80h=Analog is/was recently touched)
 17h Analog stuff
 18h Flag (01h=Idle, 02h=Analog is/was recently left or down)
 19h Analog stuff
 Following bytes can crash the chip upon reading (see below for details):
 1Ah Flag (01h=Idle, 00h/02h=Analog is/was recently somehow moved)
 1Bh Fixed 01h
 1Ch Fixed 11h
 1Dh Historic X (7xh) ;\updated every some seconds
 1Eh Historic Y (7xh) ;/(same as center when idle)
 1Fh..27h Fixed 00h-filled
 28h..2Ch Fixed 07h,06h,06h,03h,01h
 2Dh..3Bh Fixed 00h-filled
 3Ch Fixed 01h
 3Dh..46h Fixed 00h-filled
 47h..4Bh Fixed 04h,C0h,00h,00h,03h
 4Ch Whatever, changes
 4Dh Flag (00h=Idle, 01h=Analog is/was recently moved)
 4Eh..54h Fixed 01h,00h,06h,01h,A5h,00h,00h
 55h Initially random xxh, becomes 00h/01h after button/analog
 56h Flag 00h/10h/20h
 57h Analog X (00h=Center, -1xh=Left, +1xh=Right) ;\same as [02h,03h]
 58h Analog Y (00h=Center, -1xh=Down, +1xh=Up) ;/
 59h Analog
 5Ah Analog
 5Bh..65h Fixed 07h,00h,08h,04h,00h,00h,00h,09h,22h,71h,00h
 66h Fast Timer (00h..1xh or so)
 67h Fixed 15h ;maybe update period in ms, maybe limit for above timer?
 68h Slow Timer (00h..08h, increasing)
 69h..6Ch Fixed 09h,03h,00h,00h ;maybe 09h is limit for above timer?
 6Dh Up/down Timer (00h=Idle, increases-then-decreases upon analog move)
 6Eh..6Fh Fixed 00h,00h
 70h Button byte (00h=None, bit4=ZL, ?=ZR)
 71h Usually 00h (sometimes shortly 10h or so)
 72h..74h Fixed 00h-filled
 75h Fixed FFh
 76h..7Bh Fixed 00h-filled
 7Ch..82h Fixed 7Fh,15h,09h,03h,54h,28h,10h
 83h..BDh Fixed 00h-filled
 Following bytes might be CPU stack, first some bytes maybe random/garbage?
 BEh 7Ah,02h,1Dh,D9h,C7h,93h,31h,CCh,7Eh,A9h,BEh,86h,B3h,93h,6Dh,07h
 CEh C7h,82h,E7h,00h,10h,00h,00h,10h,00h,10h,00h,10h,xxh,xxh,xxh,xxh
 DEh xxh,xxh,xxh,xxh,xxh,xxh,xxh,xxh,xxh,xxh,xxh,xxh,xxh,xxh,xxh,xxh
 Following bytes seem to be used...
 EEh..xxxx Fixed FFh-filled (somewhat endless repeating) |
| --- |

Caution: The chip hangs after some seconds when reading more than 1Ah bytes; at
that point all values get frozen (except, the "stack" at DDh..EDh does keep
changing).

**I2C Write(Device,Mode)**

Writing does set a mode value, eg. write(Device,Mode). There is no need to
write anything, the analog input and irq are automatically enabled on power-up.
Effects for different mode values are:

```

| None Default is Status=80h at power-up
 00h..51h Set Status=80h
 52h..F4h Set Status=81h
 F5h Set Status=82h
 F6h..F9h Set Status=83h
 FAh..FBh Set Status=81h
 FCh..FDh Set Status=FFh
 FEh Set Status=80h with long 1 second I2C-clk-hold delay?
 FFh Set Status=80h |
| --- |

Unknown what those mode/status values are meaning exactly. Maybe some can
change polling interval or enable/disable interrupts, or maybe even do
dangerous stuff like reflashing the firmware?

Some of the mode values disable the Analog bytes at index 02h/03h (causing that
bytes to become always 00h, or maybe actually divide them by a large value,
causing them to be always NEAR 00h?)

Trying to write more bytes (eg. write(Device,xx,yy) seems to apply the last
written byte as mode value.

**Operating System**

The 3DS OS does access the C-stick via IR:RST service (probably for backwards
compatibility with Circle Pad Pro, which was accessed via infrared).

The OS can reportedly change the update period in range of 10..21ms (aka
0Ah..15h), maybe via whatever I2C write.

The OS does reportedly return analog values in range -9Ch..+9Ch, maybe that are
the I2C values scaled for Circle Pad Pro compatibility, or maybe a sum of
multiple I2C values?