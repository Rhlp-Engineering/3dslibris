# Dsii2Cbus

> Source: https://problemkaputt.de/gbatek.htm
> Section: Dsii2Cbus

DSi I2C Bus 
| |
| --- |

 **I2C Bus**
 DSi I2C I/O Ports
 DSi I2C Signals
 
 **Device 4Ah (BPTWL chip) (LED/Volume/Powerbutton/Reset)**
 DSi I2C Device 4Ah (BPTWL chip)
 
 **Device 78h/7Ah (Aptina MT9V113 Cameras)**
 DSi Aptina Camera Initialization
 Directly addressed I2C Registers (16bit index, 16bit data):
 DSi Aptina Camera Registers: SYSCTL (0000h-0051h)
 DSi Aptina Camera Registers: RX_SS, FUSE, XDMA (0100h-099Fh)
 DSi Aptina Camera Registers: CORE (3000h-31FFh, 38xxh)
 DSi Aptina Camera Registers: SOC1 (3210h-33FDh)
 DSi Aptina Camera Registers: SOC2 (3400h-3729h)
 Indirectly addressed MCU Variables (via above "XDMA" commands):
 DSi Aptina Camera Variables: RAM/SFR/MON (GPIO/Monitor) (MCU:0000h-20xxh)
 DSi Aptina Camera Variables: SEQ (Sequencer) (MCU:21xxh)
 DSi Aptina Camera Variables: AE (Auto Exposure) (MCU:22xxh)
 DSi Aptina Camera Variables: AWB (Auto White Balance) (MCU:23xxh)
 DSi Aptina Camera Variables: FD (Anti-Flicker) (MCU:24xxh)
 DSi Aptina Camera Variables: MODE (Mode/Context) (MCU:27xxh)
 DSi Aptina Camera Variables: HG (Histogram) (MCU:2Bxxh)
 I2C Bus Caution: The Camera I2C access requires the "16.76MHz Camera External Clock" enabled in Port 4004004h.Bit8 on ARM9 Side. For accessing registers other than SYSCTL/CORE, one must also clear the Standby flag in SYSCTL[0018h].
 
 **Device A0h/E0h (Unknown, maybe cameras from other manufacturer)**
 DSi Alternate Cameras from Unknown Manufacturer
 
 **Camera Data Transfers**
 Camera configuration is done on ARM7 side via serial I2C bus. However, the actual Camera Data transfers are done on ARM9 side through 8bit parallel bus:
 DSi Cameras
 
 **Broken Cameras (defunct device 78h/7Ah/A0h/E0h)**
 Consoles should contain two Aptina cameras, or two Unknown cameras. The Unlaunch installer is throwing a warning when detecting Unknown cameras, so far nobody has reported that case, so Unknown cameras seem to be very rare (if they were ever used at all). However, two people reported that warning showing FFh's in the ID bytes for most or all cameras (caused by a broken camera with only one working camera, or broken camera connector with no working cameras at all).
 Bottom line is that broken cameras are more common than unknown cameras, and games with optional/extra camera features should support that situation: ie. disable the feature in case of broken camera(s) instead of becoming unplayable.
 
 **Device 90h (Whatever)**
 Trying to read I2C for this device just returns FFh? Maybe exists in debug version only. The firmware contains a few functions for accessing this register.
 
```

| Register Width Description
 02h 1 Used for DSi IRQ6 IF flags
 uh, IF.Bit6 would be Timer3overflow ?
 or, IF2.Bit6 would be PowerButton ?
 04h 1 Unknown (bit0 toggled) |
| --- |

Unknown purpose.

**Device 40h (Whatever) (and New3DS QTM)**

Trying to read I2C for this device just returns FFh? Maybe exists in debug
version only. The firmware doesn't use this register (it does only contain the
device number in the device table).

Caution: New3DS does use device number 40h for the QTM IO Expander, this device
will be visible in DSi mode (and shouldn't be mistaken for whatever DSi device
40h was originally used for).

**DSi I2C Devices**

```

| Device Delay Description
 7Ah 0 0 Camera0(internal) ;Aptina MT9V113 (SelfPortrait)
 78h 0 1 Camera1(external) ;Aptina MT9V113 (External)
 A0h 0 2 Camera0 config (Ext) ;\maybe for other manufacturer?
 E0h 0 3 Camera1 config (Self);/
 4Ah 180h 4 BPTWL Chip (LED/Volume/Powerbutton/Reset)
 40h 0 5 Debug?
 90h 0 6 Debug? |
| --- |

Delay: required swiWaitByLoop delay

**DSi Secondary I2C Devices**

There are also some internal/secondary I2C busses (not connected to the ARM
CPUs).

```

| 82h Power Managment Device (connected to BPTWL chip)
 50h I2C bus potentiometer (volume D/A converter) (connected to BPTWL chip)
 30h I2C bus unknown write-only, DSiXL omly ? (connected to BPUTL chip)
 A0h I2C bus EEPROM (connected to Atheros wifi chip)
 - I2C voltage translator (between ARM CPU and BPTWL chip) |
| --- |

| DSi I2C I/O Ports |
| --- |

**4004500h - DSi7 - I2C_DATA (R/W)**

```

| 0-7 Data (or Device, or Register) |
| --- |

When sending data, I2C_DATA should be written <before> setting
I2C_CNT.bit7.

When reading data, I2C_DATA should be read <after> I2C_CNT.bit7 goes off.

Alongsides with the 8bit data, an additional 1bit "Ack" flag is transferred as
response to the data (ie. in opposite direction of data direction), the Ack is
located in I2C_CNT.Bit4, and it's usually indicating errors (or in some cases
it appears to be also used to indicate that no further data is to be
transferred).

**4004501h - DSi7 - I2C_CNT (R/W)**

```

| 0 Stop (0=No, 1=Stop signal, after last byte)
 1 Start (0=No, 1=Start signal, before first byte)
 2 Error (0=No, 1=Pause/Flush? after Error, used with/after Stop)
 (1=Transfer only selected Start/Stop signal(s), without data?)
 (Nintendo seems to use bit2+bit0 to (try to) overcome problems
 with the BPTWL chip's quirky Stop handling)
 3 Unknown/unused (0)
 4 Ack (0=High=Error/LastRead, 1=Low=Okay) ;For DataRead:W, for DataWrite:R
 5 Data Direction (0=Write, 1=Read) (note: Ack uses opposite direction)
 6 Interrupt Enable (0=Disable, 1=Enable)
 7 Start/busy (0=Ready, 1=Start/busy) |
| --- |

**I2C Transfer Flowchart**

The first byte (with the "Start" condition) contains the device number, which
consists of a 7bit chip ID and a direction flag in bit0 (0=Write or 1=Read).
The direction flag applies to all following bytes (until last byte with "Stop"
condition), that rule means that "Write Index & Write Data" can be done in
a single step, whilst "Write Index & Read Data" must be split into two
separate steps (each with own "Start/Stop" conditions):

```

| For Writing:
 Write Device+0 (with Start condition) ;\
 Write Index byte(s) ; write index + data
 Write Data byte(s) (last byte with Stop condition) ;/
 For Reading:
 Write Device+0 (with Start condition) ;\1st step: write index
 Write Index byte(s) ;/
 Write Device+1 (with Start condition) ;\2nd step: read data
 Read Data byte(s) (last byte with ACK=0 and Stop) ;/ |
| --- |

The index is usually a single byte (except for Aptina cameras, which do use
16bit indices transferred in two bytes; ordered MSB, LSB).

Per-byte transfer completion is indicated by the Start/busy flag, which should
also indicate if the I2C chip is ready for next byte (I2C devices can hold the
clock line low if they aren't ready), however, the BPTWL chip somehow doesn't
support that, and it should be accessed with an extra delay:

```

| Invoke byte-transfer
 Do WaitByLoop (needed for the BPTWL device only)
 Wait for start/busy flag to get zero |
| --- |

Note: The DSi firmware is doing eight retries per I2C command (in case of
receiving wrong ACK bytes), unknown if that's really required, a stable system
should never need to do retries.

| DSi I2C Signals |
| --- |

Below is some pseudo code for the I2C signal transmission. The DSi hardware is
doing most of that stuff automatically. The pseudo code may be useful for
understanding the purpose of the start/stop/ack flags in the control register.

```

| START D7 D6 D5 D4 D3 D2 D1 D0 ACK D7 D6/ .. /D1 D0 ACK STOP
 __ ___ ___ ___ ___ ___ ___ ___ ___ ___ __/ /___ ___ ___
 SDA |__|___|___|___|___|___|___|___|___|___|___|_/ .. /|___|___|______|
 ____ _ _ _ _ _ _ _ _ _ _ / / _ _ _ _____
 SCL |_| |_| |_| |_| |_| |_| |_| |_| |_| |_| |/ .. / |_| |_| |_| |_|
 / /
 <--><------------------------------><--><--------------------><--><-->
 Start Device/Direction Byte Ack Index/Data Byte(s) Ack Stop |
| --- |

SDA should be changed at/after falling SCL edge (except for Start/Stop
conditions, which are output during SCL=High).

The Device/Direction Byte is sent by master (the byte contains a 7bit device
address in bit7-1, and a direction flag in bit0). The direction of the follwing
index/data byte(s) depends on that direction flag (0=Write, 1=Read). The ACK
bit responses are sent in opposite direction as the preceeding byte.

The SCL line is driven by master, however, when the master changes SCL from Low
to HighZ, then the slave may keep SCL held Low to signalize that it isn't yet
ready for the next bit.

**i2c_write_byte(send_start,send_stop,databyte):**

```

| if (send_start) then i2c_start_cond() ;-start (if so)
 for i=7 downto 0, i2c_write_bit(databyte.bit(i)), next i ;-write 8bit
 nack = i2c_read_bit() ;-read nack
 if (send_stop) then i2c_stop_cond() ;-stop (if so)
 return nack ;return 0 if ack by the slave. ;-return nack |
| --- |

**i2c_read_byte(send_stop):**

```

| for i=7 downto 0, databyte.bit(i)=i2c_read_bit(), next i ;-read 8bit
 i2c_write_bit(nack) ;-write nack
 if (send_stop) then ;\
 i2c_write_bit(1) ;NACK (ack=high=Last byte) ; nack (finish)
 i2c_stop_cond() ;STOP ;/
 else ;\ack (want more)
 i2c_write_bit(0) ;ACK (ack=low=More bytes) ;/
 return databyte ;-return databyte |
| --- |

**i2c_write_bit(bit):**

```

| if (bit) then SDA=HighZ else SDA=Low ;-
 I2C_delay() ;-
 SCL=HighZ
 wait until SCL=High (or timeout) ;-wait (for clock stretching)
 if (bit=1 and SDA=Low) then arbitration_lost();-errif other HW pulls SDA=low
 I2C_delay() ;-
 SCL=Low ;- |
| --- |

**i2c_read_bit():**

```

| SDA=HighZ ;-let the slave drive data
 I2C_delay() ;-delay (one half clk)
 SCL=HighZ
 wait until SCL=High (or timeout) ;-wait (for clock stretching)
 bit = SDA ;-
 I2C_delay() ;-delay (one half clk)
 SCL=Low ;-
 return bit ;- |
| --- |

**i2c_start_cond():**

```

| if (started) then ;if started, do a restart cond
 SDA=HighZ ;-set SDA to 1
 I2C_delay()
 SCL=HighZ
 wait until SCL=High (or timeout) ;-wait (for clock stretching)
 I2C_delay() ;Repeated start setup time, minimum 4.7us
 if (SDA=Low) then arbitration_lost()
 SDA=Low ;-
 I2C_delay()
 SCL=Low
 started = true |
| --- |

**i2c_stop_cond():**

```

| SDA=Low ;-
 I2C_delay() ;-
 SCL=HighZ ;-
 wait until SCL=High (or timeout) ;-wait (for clock stretching)
 I2C_delay() ;Stop bit setup time, minimum 4us
 if (SDA=Low) then arbitration_lost()
 I2C_delay()
 started = false |
| --- |

**I2C ACK/NACK Notes**

There are five conditions that lead to the generation of a NACK:

```

| 1. No receiver is present on the bus with the transmitted address so there
 is no device to respond with an acknowledge.
 2. The receiver is unable to receive or transmit because it is performing
 some real-time function and is not ready to start communication with the
 master.
 3. During the transfer, the receiver gets data or commands that it does not
 understand.
 4. During the transfer, the receiver cannot receive any more data bytes.
 5. A master-receiver must signal the end of the transfer to the slave
 transmitter. |
| --- |

Sending the I2C Stop condition is done by sending a "corrupted" databit
(instead of sending the first databit of the next byte) on the SDA output pin.

However, in receive mode, SDA would be in input direction (for receiving the
first databit of next byte). So the master simply cannot output the Stop
condition in that state (unless when using the ACK bit to notify the slave that
the transfer will be stopped now).

| DSi I2C Device 4Ah (BPTWL chip) |
| --- |

I2C Bus Caution: The BPTWL chip requires swiWaitByLoop(180h) after each I2C
byte transfer (if the Version/Speed byte at BPTWL[00h] indicates "Fast", then
the delay can be reduced to 90h instead of 180h).

And, SPI bus Powerman chip does somehow interact with I2C BPTWL chip; there
must be a similar delay between Powerman writes and BPTWL writes.

**BPTWL Chip (LED/Volume/Powerbutton/Reset) (Device 4Ah)**

```

| 00h R Version/Speed (usually 33h) (00h..20h=Slow, 21h..FFh=Fast)
 01h R ? Unknown, Powerman Status 1 (00h) ;some hw/sw type/version?
 02h R ? Unknown, Powerman Status 2 (50h) ;30h=AlternateCamLedHardware?
 03h-0Fh - Reserved (5Ah-filled)
 10h R Power Button Status (bit0=WasWhat?, bit1=IsDown, bit3=WasDown?)
 (bit0/3 are cleared after reading)
 11h R/W System Reset (00h=No, 01h=Force Reset, 02h=Prevent Reset?)
 12h R/W Power Button Tapping (00h=Auto-Reset, 01h=IRQ) (bit1=WL_RXPE??)
 bit0: Enable IRQs (for Power/Vol+/- button, battery low)
 bit1: battery boundaries (and 0=disable battery low IRQs)
 bit5: initially set (while bptwl chip is booting up?)
 bit7: Linear volume button mode
 13h-1Fh - Reserved (5Ah-filled)
 20h R Battery State (bit0..3=Battery Level, bit7=Charge)
 W On 3DS in DSi mode: Write 8=Shutdown, 4=Return to 3DS mode ?
 21h R/W Powerman Sleep?, NDS Cartridge related? (07h=normal)
 22h-2Fh - Reserved (5Ah-filled)
 30h R/W Wifi LED (0/2=Off, 1=On, 3=BlinkOnTraffic) and bit4=SDIO enable
 31h R/W Camera LED (00h=Off, 01h=On, 02h=Blink)
 32h-3Fh - Reserved (5Ah-filled)
 40h R/W Volume Level (00h..1Fh) ;\nonvolatile!
 41h R/W Backlight Level (00h..04h) ;/
 42h-4Fh - Reserved (5Ah-filled)
 50h R/W Unknown Value (DSi-XL only) (output to I2C device 30h)
 51h R/W Unknown Trigger (DSi-XL only) (cleared when processed)
 50h-5Fh - Reserved (5Ah-filled) (except 50h-51h on DSi XL)
 60h R/W Battery Calibration Mode (bit0=1=Calibrate, Stop I2C, Purple LED)
 61h R Battery Calibration Flag (01h=Calibrated, E0h/E8h=NotCalibrated)
 62h R Battery Calibration Data (xxh=BatteryBias, 50h=NotCalibrated)
 63h R/W Force Power LED (0=Automatic, 1=Red, 2=Blue, 3=Purple)
 64h-6Fh - Reserved (5Ah-filled)
 70h R/W Bootflag (00h=Coldboot, 01h=Warmboot/SkipHealthSafety)
 71h R/W ? Unknown (00h) ;\more general-purpose bootflags?
 72h-76h R/W ? Unknown (00h-filled) ;/
 77h ?/W Firmware Updater (write 4Ah, plus HEX file with delays) (danger!)
 78h-7Fh - Reserved (5Ah-filled)
 80h R/W Power Button Tap delay (10h) ;\can affect/disable
 81h R/W Power Button Hold delay (64h) ;/Power Button Tapping
 82h-FFh - Reserved (5Ah-filled) |
| --- |

The R/W values can be set to 00h..FFh (except Index 40h/41h are quickly cropped
to max 1Fh/04h, and Index 70h/71h are (after some time) cropped to 01h/02h).

Above should probably also include:

```

| Forced volume (for alerts) (ie. alternately to current "user volume") |
| --- |

**BPTWL Emulation on 3DS**

The 3DS MCU emulates the BPTWL chip for DSi mode (and the emulated registers
can be also accessed in 3DS mode). The emulation supports only a few of the
original BPTWL registers & bits though:

```

| 00h R Version/Speed (35h on New3DS)
 10h R Power Button Status (only 2 bits?: bit3=WasDown?, bit6=?)
 11h R/W Reset (01h=Reset, other=ignored) (read: always 00h)
 12h R/W Power Button Tapping (bit0,1,7=?)
 20h R Battery State
 31h R/W Camera LED (bit0,1)
 40h R/W Volume Level
 70h R/W Bootflag
 Other - Unused (FFh) |
| --- |

**DSi Power Button**

Logically, the Power Button has two functions:

```

| Short tap --> reset (warmboot, go to DSi menu, without health and safety)
 Hold 1 second --> power-off |
| --- |

Technically, the button can have three functions:

```

| Auto-Reset (used for NDS games)
 IRQ (supposed to be used with Manual-Reset) (used for DSi games)
 Forced Power-off (for games which fail to handle the IRQ within 5 seconds) |
| --- |

DSi games should handle the IRQ as follows: First, do some clean up (like
finishing writes to SD/MMC storage; to avoid FAT corruption). Then, issue a
Reset manually (via I2C/BPTWL registers [70h]=01h/Warmboot, [11h]=01h/Reset).

Power-Off can be implemented via SPI/Power Managment Device, however, games
only need to implement Manual-Reset or Auto-Reset, but don't need to implement
Power-Off (the firmware will do that automatically if the button is held down
for 1 second after issuing the Reset).

Ideally, emulators should also reproduce the power button (when resetting or
closing the emulator): Signalize power-button and keep the emulation running
until the game responds by Reset (or until five second timeout). That will
allow the game to finish writes to emulated SD/MMC storage.

**DSi Autostart on Warmboot (20h-byte area) (also requires BPTWL[70h]=01h)**

The DSi can be commanded to load a different title (eg. System Settings),
instead of showing the Boot Menu after warmboot.

DSi Autoload on Warmboot

**Older blurb...**

```

| 0x10 1 Power flags. When bit0 is set, arm7 does a system reset.
 When bit1 or bit3 are set, arm7 does a shutdown. Bits 0-2
 are used for DSi IRQ6 IF flags (uh, rather IF2 maybe?).
 0x20 1 Battery flags. When zero the battery is at critical level,
 arm7 does a shutdown. Bit7 is set when the battery is
 charging. Battery levels in the low 4-bits: battery icon
 bars full 0xF, 3 bars 0xB, 2 bars 0x7, one solid red bar
 0x3, and one blinking red bar 0x1. When plugging in or
 removing recharge cord, this value increases/decreases
 between the real battery level and 0xF, thus the battery
 level while bit7 is set is useless. |
| --- |

**BPTWL/BPUTL Chip Names**

```

| DSi: Renesas Electronics "BPTWL, KG07K" ;reg[00h]=33h
 DSiXL: Renesas Electronics "BP UTL-1, KG08" ;reg[00h]=BBh or B7h
 3DS: Renesas Electronics "UC CTR, 041KM73, KG10" ;reg[00h]=?
 New3DSXL: Renesas Electronics "UC KTR, 423KM01, 'TK14" ;reg[00h]=35h |
| --- |

**Inner workings**

The DSi's BPTWL chip contains a 78K/0 microprocessor. The BPTWL firmware has
aparently leaked in a "TWL_MCU_33-MP_95811supp.hex" file. There is also a
"UTL_MCU_BA.hex" file (for DSi XL?).

78K/0 Opcode List

78K/0 Opcode Map

78K/0 Memory Map and SFRs

Programming via external serial cable is about same as on 3DS:

3DS I2C MCU - RL78 Flash Programming via UART

The 3DS' MCU emulates the BPTWL functions by using a 78K0R/ microprocessor
(with similar asm syntax, but completely different opcode numbering, with more
16bit ALU opcodes, new conditional-skip opcodes, and 20bit addressing).

| 78K/0 Opcode List |
| --- |

**Move Byte**

```

| 85/87/AB/AA/AE MOV A, [DE]/[HL]/[HL+B]/[HL+C]/[HL+nn]
 F0/F4/8E MOV A, [FExx]/[FFxx]/[nnnn]
 60/--/62/63/64/65/66/67 MOV A, X/-/C/B/E/D/L/H
 70/--/72/73/74/75/76/77 MOV X/-/C/B/E/D/L/H, A
 A0/A1/A2/A3/A4/A5/A6/A7 MOV X/A/C/B/E/D/L/H, nn
 95/97/BB/BA/BE MOV [DE]/[HL]/[HL+B]/[HL+C]/[HL+nn], A
 F2/F6/9E MOV [FExx]/[FFxx]/[nnnn], A
 11/13 MOV [FExx]/[FFxx], nn
 30/--/32/33/34/35/36/37 XCH A, X/-/C/B/E/D/L/H
 05/07/318B/318A/DE XCH A, [DE]/[HL]/[HL+B]/[HL+C]/[HL+nn]
 83/93/CE XCH A, [FExx]/[FFxx]/[nnnn] |
| --- |

**Move Word**

```

| C2/C4/C6/10 MOVW AX, BC/DE/HL/nnnn
 89/A9/02 MOVW AX, [FExx]/[FFxx]/[nnnn]
 D2/D4/D6 MOVW BC/DE/HL, AX
 12/14/16 MOVW BC/DE/HL, nnnn
 99/B9/03 MOVW [FExx]/[FFxx]/[nnnn], AX
 EE/FE MOVW [FExx]/[FFxx], nnnn
 E2/E4/E6 XCHW AX,BC/DE/HL
 B1/B3/B5/B7/22 PUSH AX/BC/DE/HL/PSW ;PSW=[FF1Eh] (8bit)
 B0/B2/B4/B6/23 POP AX/BC/DE/HL/PSW ;PSW=[FF1Eh] (8bit) |
| --- |

**ALU Byte**

```

| 6100/6101/6102/6103/6104/6105/6106/6107 ADD X/A/C/B/E/D/L/H, A
 6120/6121/6122/6123/6124/6125/6126/6127 ADDC X/A/C/B/E/D/L/H, A
 6110/6111/6112/6113/6114/6115/6116/6117 SUB X/A/C/B/E/D/L/H, A
 6130/6131/6132/6133/6134/6135/6136/6137 SUBC X/A/C/B/E/D/L/H, A
 6140/6141/6142/6143/6144/6145/6146/6147 CMP X/A/C/B/E/D/L/H, A
 6150/6151/6152/6153/6154/6155/6156/6157 AND X/A/C/B/E/D/L/H, A
 6160/6161/6162/6163/6164/6165/6166/6167 OR X/A/C/B/E/D/L/H, A
 6170/6171/6172/6173/6174/6175/6176/6177 XOR X/A/C/B/E/D/L/H, A
 6108/----/610A/610B/610C/610D/610E/610F ADD A, X/-/C/B/E/D/L/H
 6128/----/612A/612B/612C/612D/612E/612F ADDC A, X/-/C/B/E/D/L/H
 6118/----/611A/611B/611C/611D/611E/611F SUB A, X/-/C/B/E/D/L/H
 6138/----/613A/613B/613C/613D/613E/613F SUBC A, X/-/C/B/E/D/L/H
 6148/----/614A/614B/614C/614D/614E/614F CMP A, X/-/C/B/E/D/L/H
 6158/----/615A/615B/615C/615D/615E/615F AND A, X/-/C/B/E/D/L/H
 6168/----/616A/616B/616C/616D/616E/616F OR A, X/-/C/B/E/D/L/H
 6178/----/617A/617B/617C/617D/617E/617F XOR A, X/-/C/B/E/D/L/H
 0D/0F/310B/310A/09/0E/08 ADD A,nn/[HL]/[HL+B]/[HL+C]/[HL+nn]/[FExx]/[nnnn]
 2D/2F/312B/312A/29/2E/28 ADDC A,nn/[HL]/[HL+B]/[HL+C]/[HL+nn]/[FExx]/[nnnn]
 1D/1F/311B/311A/19/1E/18 SUB A,nn/[HL]/[HL+B]/[HL+C]/[HL+nn]/[FExx]/[nnnn]
 3D/3F/313B/313A/39/3E/38 SUBC A,nn/[HL]/[HL+B]/[HL+C]/[HL+nn]/[FExx]/[nnnn]
 4D/4F/314B/314A/49/4E/48 CMP A,nn/[HL]/[HL+B]/[HL+C]/[HL+nn]/[FExx]/[nnnn]
 5D/5F/315B/315A/59/5E/58 AND A,nn/[HL]/[HL+B]/[HL+C]/[HL+nn]/[FExx]/[nnnn]
 6D/6F/316B/316A/69/6E/68 OR A,nn/[HL]/[HL+B]/[HL+C]/[HL+nn]/[FExx]/[nnnn]
 7D/7F/317B/317A/79/7E/78 XOR A,nn/[HL]/[HL+B]/[HL+C]/[HL+nn]/[FExx]/[nnnn]
 88 ADD [FExx], nn
 A8 ADDC [FExx], nn
 98 SUB [FExx], nn
 B8 SUBC [FExx], nn
 C8 CMP [FExx], nn
 D8 AND [FExx], nn
 E8 OR [FExx], nn
 F8 XOR [FExx], nn
 40/41/42/43/44/45/46/47/81 INC X/A/C/B/E/D/L/H/[FExx]
 50/51/52/53/54/55/56/57/91 DEC X/A/C/B/E/D/L/H/[FExx] |
| --- |

**ALU Word**

```

| CA/DA/EA ADDW/SUBW/CMPW AX,nnnn
 80/82/84/86 INCW AX/BC/DE/HL
 90/92/94/96 DECW AX/BC/DE/HL |
| --- |

**Rotate/Shift**

```

| 26 ROL A,1
 24 ROR A,1
 27 ROLC A,1
 25 RORC A,1
 3180 ROL4 A,[HL] ;12bit A.lsb:[HL],4 ;native: ROL4 [HL]
 3190 ROR4 A,[HL] ;12bit A.lsb:[HL],4 ;native: ROR4 [HL] |
| --- |

**Jump/Call**

```

| 3198 JMP AX ;native: BR AX
 9B JMP absolute addr ;native: BR
 FA JMP relative short addr ;native: BR
 9A CALL absolute addr ;native: CALL
 C1+(nn AND 3Eh) CALL [40h..7Eh] ;native: CALLT
 0C/1C/2C/3C/4C/5C/6C/7C CALLF 08nn/09nn/0Ann/0Bnn/0Cnn/0Dnn/0Enn/0Fnn |
| --- |

**Conditional Relative Jumps**

```

| 8D JC aka JB rel ;carry/below ;native: BC
 AD JZ aka JE rel ;zero/equal ;native: BZ
 9D JNC aka JAE rel ;not carry/below ;native: BNC
 BD JNZ aka JNE rel ;not zero/equal ;native: BNZ
 04/8B/8A DJNZ [FExx]/B/C, rel ;decrement+jnz ;native: DBNZ |
| --- |

**Test (and Clear) and Conditional Relative Jump**

```

| 3101/3105/3185/310D+n*10 BTCLR [FExx]/[FFxx]/[HL]/A.n, rel
 8C/3106/3186/310E+n*10 BT [FExx]/[FFxx]/[HL]/A.n, rel
 3103/3107/3187/310F+n*10 BF [FExx]/[FFxx]/[HL]/A.n, rel |
| --- |

**Bit Manipulation**

```

| 7101/7109/7181/6189+n*10 MOV1 [FExx]/[FFxx]/[HL]/A.n, CY
 7104/710C/7184/618C+n*10 MOV1 CY, [FExx]/[FFxx]/[HL]/A.n
 7105/710D/7185/618D+n*10 AND1 CY, [FExx]/[FFxx]/[HL]/A.n
 7106/710E/7186/618E+n*10 OR1 CY, [FExx]/[FFxx]/[HL]/A.n
 7107/710F/7187/618F+n*10 XOR1 CY, [FExx]/[FFxx]/[HL]/A.n
 20/0A/710A/7182/618A+n*10 SET1 CY/[FExx]/[FFxx]/[HL]/A.n
 21/0B/710B/7183/618B+n*10 CLR1 CY/[FExx]/[FFxx]/[HL]/A.n
 01 NOT1 CY |
| --- |

**Misc**

```

| 00 NOP
 BF/7110/7100 BRK/HALT/STOP
 AF/9F/8F RET/RETB/RETI
 61D0/61D8/61F0/61F8 SEL RB0/1/2/3
 3188 MULU AX,A,X ;unsigned AX=A*X ;native: MULU X
 3182 DIVUW AX,C ;unsigned AX=AX/C, C=remainder ;X
 6180/6190 ADJBA/ADJBS ;decimal BCD adjust A after Add/Sub
 7A1E/7B1E EI/DI ;pseudo SET1/CLR1 PSW.7
 FA JR addr ;pseudo jmp short relative addr |
| --- |

**Note**

Operand [FExx] can he in range FE20h..FF1Fh, of which FE20h..FEFFh is encoded
as 20..FF, and FF00h..FF1Fh is encoded as 00..1F (the latter overlaps with
operand [FFxx]) (for the overlapping addresses, [FExx] is preferred because
BT/SET1/CLR1 have smaller encoding for [FExx] than [FFxx], and DJNZ/ALU support
only [FExx]) (all opcodes that support [FFxx] do also support [FExx], so [FFxx]
should be NEVER used for [FF00..FF1F]).

| 78K/0 Opcode Map |
| --- |

**78K/0 Primary Opcode Map**

```

| 00 10 20 30
 00 NOP MOVW AX,nnnn SET1 CY XCH A,X
 01 NOT1 CY MOV [FExx],nn CLR1 CY <prefix>
 02 MOVW AX,[nnnn] MOVW BC,nnnn PUSH PSW ;[FF1Eh] XCH A,C
 03 MOVW [nnnn],AX MOV [FFxx],nn POP PSW ;[FF1Eh] XCH A,B
 04 DBNZ [FExx],rel MOVW DE,nnnn ROR A,1 XCH A,E
 05 XCH A,[DE] <undef> RORC A,1 XCH A,D
 06 <undef> MOVW HL,nnnn ROL A,1 XCH A,L
 07 XCH A,[HL] <undef> ROLC A,1 XCH A,H
 08 ADD A,[nnnn] SUB A,[nnnn] ADDC A,[nnnn] SUBC A,[nnnn]
 09 ADD A,[HL+nn] SUB A,[HL+nn] ADDC A,[HL+nn] SUBC A,[HL+nn]
 0A SET1 [FExx].0 SET1 [FExx].1 SET1 [FExx].2 SET1 [FExx].3
 0B CLR1 [FExx].0 CLR1 [FExx].1 CLR1 [FExx].2 CLR1 [FExx].3
 0C CALLF 08nn CALLF 09nn CALLF 0Ann CALLF 0Bnn
 0D ADD A,nn SUB A,nn ADDC A,nn SUBC A,nn
 0E ADD A,[FExx] SUB A,[FExx] ADDC A,[FExx] SUBC A,[FExx]
 0F ADD A,[HL] SUB A,[HL] ADDC A,[HL] SUBC A,[HL] |
| --- |

```

| 40 50 60 70
 40 INC X DEC X MOV A,X MOV X,A
 41 INC A DEC A <prefix> <prefix>
 42 INC C DEC C MOV A,C MOV C,A
 43 INC B DEC B MOV A,B MOV B,A
 44 INC E DEC E MOV A,E MOV E,A
 45 INC D DEC D MOV A,D MOV D,A
 46 INC L DEC L MOV A,L MOV L,A
 47 INC H DEC H MOV A,H MOV H,A
 48 CMP A,[nnnn] AND A,[nnnn] OR A,[nnnn] XOR A,[nnnn]
 49 CMP A,[HL+nn] AND A,[HL+nn] OR A,[HL+nn] XOR A,[HL+nn]
 4A SET1 [FExx].4 SET1 [FExx].5 SET1 [FExx].6 SET1 [FExx].7
 4B CLR1 [FExx].4 CLR1 [FExx].5 CLR1 [FExx].6 CLR1 [FExx].7
 4C CALLF 0Cnn CALLF 0Dnn CALLF 0Enn CALLF 0Fnn
 4D CMP A,nn AND A,nn OR A,nn XOR A,nn
 4E CMP A,[FExx] AND A,[FExx] OR A,[FExx] XOR A,[FExx]
 4F CMP A,[HL] AND A,[HL] OR A,[HL] XOR A,[HL] |
| --- |

```

| 80 90 A0 B0
 80 INCW AX DECW AX MOV X,nn POP AX
 81 INC [FExx] DEC [FExx] MOV A,nn PUSH AX
 82 INCW BC DECW BC MOV C,nn POP BC
 83 XCH A,[FExx] XCH A,[FFxx] MOV B,nn PUSH BC
 84 INCW DE DECW DE MOV E,nn POP DE
 85 MOV A,[DE] MOV [DE],A MOV D,nn PUSH DE
 86 INCW HL DECW HL MOV L,nn POP HL
 87 MOV A,[HL] MOV [HL],A MOV H,nn PUSH HL
 88 ADD [FExx],nn SUB [FExx],nn ADDC [FExx],nn SUBC [FExx],nn
 89 MOVW AX,[FExx] MOVW [FExx],AX MOVW AX,[FFxx] MOVW [FFxx],AX
 8A DBNZ C,rel CALL nnnn MOV A,[HL+C] MOV [HL+C],A
 8B DBNZ B,rel BR nnnn MOV A,[HL+B] MOV [HL+B],A
 8C BT [FExx].0,rel BT [FExx].1,rel BT [FExx].2,rel BT [FExx].3,rel
 8D BC rel BNC rel BZ rel BNZ rel
 8E MOV A,[nnnn] MOV [nnnn],A MOV A,[HL+nn] MOV [HL+nn],A
 8F RETI RETB RET BRK |
| --- |

```

| C0 D0 E0 F0
 C0 <undef> <undef> <undef> MOV A,[FExx]
 C1 CALLT [40] CALLT [50] CALLT [60] CALLT [70]
 C2 MOVW AX,BC MOVW BC,AX XCHW AX,BC MOV [FExx],A
 C3 CALLT [42] CALLT [52] CALLT [62] CALLT [72]
 C4 MOVW AX,DE MOVW DE,AX XCHW AX,DE MOV A,[FFxx]
 C5 CALLT [44] CALLT [54] CALLT [64] CALLT [74]
 C6 MOVW AX,HL MOVW HL,AX XCHW AX,HL MOV [FFxx],A
 C7 CALLT [46] CALLT [56] CALLT [66] CALLT [76]
 C8 CMP [FExx],nn AND [FExx],nn OR [FExx],nn XOR [FExx],nn
 C9 CALLT [48] CALLT [58] CALLT [68] CALLT [78]
 CA ADDW AX,nnnn SUBW AX,nnnn CMPW AX,nnnn BR rel
 CB CALLT [4A] CALLT [5A] CALLT [6A] CALLT [7A]
 CC BT [FExx].4,rel BT [FExx].5,rel BT [FExx].6,rel BT [FExx].7,rel
 CD CALLT [4C] CALLT [5C] CALLT [6C] CALLT [7C]
 CE XCH A,[nnnn] XCH A,[HL+nn] MOVW [FExx],nnnn MOVW [FFxx],nnnn
 CF CALLT [4E] CALLT [5E] CALLT [6E] CALLT [7E] |
| --- |

**78K/0 Opcodes with Prefix 31**

```

| 3100+xxx*10 -
 3180+xxx*10 - (unless below ROL/ROR)
 3101+bbb*10 BTCLR [FExx].n,rel ;n=0..7
 3181+xxx*10 -
 3102+xxx*10 -
 3182+xxx*10 - (unless below DIV)
 3103+bbb*10 BF [FExx].n,rel ;n=0..7
 3183+xxx*10 -
 3104+xxx*10 -
 3184+xxx*10 -
 3105+bbb*10 BTCLR [FFxx].n,rel ;n=0..7
 3185+bbb*10 BTCLR [HL].n,rel ;n=0..7
 3106+bbb*10 BT [FFxx].n,rel ;n=0..7
 3186+bbb*10 BT [HL].n,rel ;n=0..7
 3107+bbb*10 BF [FFxx].n,rel ;n=0..7
 3187+bbb*10 BF [HL].n,rel ;n=0..7
 3108+xxx*10 -
 3188+xxx*10 - (unless below MUL/BR)
 3109+xxx*10 -
 3189+xxx*10 -
 310A+xxx*10 <see below ADD/SUB/ADDC/SUBC/CMP/AND/OR/XOR)
 318A+xxx*10 - (unless below XCH)
 310B+xxx*10 <see below ADD/SUB/ADDC/SUBC/CMP/AND/OR/XOR)
 318B+xxx*10 - (unless below XCH)
 310C+xxx*10 -
 318C+xxx*10 -
 310D+bbb*10 BTCLR A.n,rel ;n=0..7
 318D+xxx*10 -
 310E+bbb*10 BT A.n,rel ;n=0..7
 318E+xxx*10 -
 310F+bbb*10 BF A.n,rel ;n=0..7
 318F+xxx*10 -
 310A ADD A,[HL+C]
 310B ADD A,[HL+B]
 311A SUB A,[HL+C]
 311B SUB A,[HL+B]
 312A ADDC A,[HL+C]
 312B ADDC A,[HL+B]
 313A SUBC A,[HL+C]
 313B SUBC A,[HL+B]
 314A CMP A,[HL+C]
 314B CMP A,[HL+B]
 315A AND A,[HL+C]
 315B AND A,[HL+B]
 316A OR A,[HL+C]
 316B OR A,[HL+B]
 317A XOR A,[HL+C]
 317B XOR A,[HL+B]
 318A XCH A,[HL+C]
 318B XCH A,[HL+B]
 3180 ROL4 A,[HL] ;rol 12bit A.lsb:[HL],4 ;native: ROL4 [HL]
 3190 ROR4 A,[HL] ;ror 12bit A.lsb:[HL],4 ;native: ROR4 [HL]
 3182 DIVUW AX,C ;unsigned AX=AX/C, C=remainder ;native: DIVWU C
 3188 MULU AX,A,X ;unsigned AX=A*X ;native: MULU X
 3198 BR AX |
| --- |

**78K/0 Opcodes with Prefix 61**

```

| 6100+rrr ADD r,A ;r=X,A,C,B,E,D,L,H
 6108+rrr ADD A,r ;r=X,-,C,B,E,D,L,H
 6110+rrr SUB r,A ;r=X,A,C,B,E,D,L,H
 6118+rrr SUB A,r ;r=X,-,C,B,E,D,L,H
 6120+rrr ADDC r,A ;r=X,A,C,B,E,D,L,H
 6128+rrr ADDC A,r ;r=X,-,C,B,E,D,L,H
 6130+rrr SUBC r,A ;r=X,A,C,B,E,D,L,H
 6138+rrr SUBC A,r ;r=X,-,C,B,E,D,L,H
 6140+rrr CMP r,A ;r=X,A,C,B,E,D,L,H
 6148+rrr CMP A,r ;r=X,-,C,B,E,D,L,H
 6150+rrr AND r,A ;r=X,A,C,B,E,D,L,H
 6158+rrr AND A,r ;r=X,-,C,B,E,D,L,H
 6160+rrr OR r,A ;r=X,A,C,B,E,D,L,H
 6168+rrr OR A,r ;r=X,-,C,B,E,D,L,H
 6170+rrr XOR r,A ;r=X,A,C,B,E,D,L,H
 6178+rrr XOR A,r ;r=X,-,C,B,E,D,L,H
 6180+xxx*10 - (unless below ADJ/SEL)
 6181+xxx*10 -
 6182+xxx*10 -
 6183+xxx*10 -
 6184+xxx*10 -
 6185+xxx*10 -
 6186+xxx*10 -
 6187+xxx*10 -
 6188+xxx*10 - (unless below ADJ/SEL)
 6189+bbb*10 MOV1 A.n,CY ;n=0..7
 618A+bbb*10 SET1 A.n ;n=0..7
 618B+bbb*10 CLR1 A.n ;n=0..7
 618C+bbb*10 MOV1 CY,A.n ;n=0..7
 618D+bbb*10 AND1 CY,A.n ;n=0..7
 618E+bbb*10 OR1 CY,A.n ;n=0..7
 618F+bbb*10 XOR1 CY,A.n ;n=0..7
 6180 ADJBA ;Decimal BCD adjust A after Addition
 6190 ADJBS ;Decimal BCD adjust A after Subtraction
 61D0 SEL RB0
 61D8 SEL RB1
 61F0 SEL RB2
 61F8 SEL RB3 |
| --- |

**78K/0 Opcodes with Prefix 71**

```

| 7100+xxx*10 - (unless below STOP/HALT)
 7180+xxx*10 -
 7101+bbb*10 MOV1 [FExx].n,CY ;n=0..7
 7181+bbb*10 MOV1 [HL].n,CY ;n=0..7
 7102+xxx*10 -
 7182+bbb*10 SET1 [HL].n ;n=0..7
 7103+xxx*10 -
 7183+bbb*10 CLR1 [HL].n ;n=0..7
 7104+bbb*10 MOV1 CY,[FExx].n ;n=0..7
 7184+bbb*10 MOV1 CY,[HL].n ;n=0..7
 7105+bbb*10 AND1 CY,[FExx].n ;n=0..7
 7185+bbb*10 AND1 CY,[HL].n ;n=0..7
 7106+bbb*10 OR1 CY,[FExx].n ;n=0..7
 7186+bbb*10 OR1 CY,[HL].n ;n=0..7
 7107+bbb*10 XOR1 CY,[FExx].n ;n=0..7
 7187+bbb*10 XOR1 CY,[HL].n ;n=0..7
 7108+xxx*10 -
 7188+xxx*10 -
 7109+bbb*10 MOV1 [FFxx].n,CY ;n=0..7
 7189+xxx*10 -
 710A+bbb*10 SET1 [FFxx].n ;n=0..7
 718A+xxx*10 -
 710B+bbb*10 CLR1 [FFxx].n ;n=0..7
 718B+xxx*10 -
 710C+bbb*10 MOV1 CY,[FFxx].n ;n=0..7
 718C+xxx*10 -
 710D+bbb*10 AND1 CY,[FFxx].n ;n=0..7
 718D+xxx*10 -
 710E+bbb*10 OR1 CY,[FFxx].n ;n=0..7
 718E+xxx*10 -
 710F+bbb*10 XOR1 CY,[FFxx].n ;n=0..7
 718F+xxx*10 -
 7100 STOP
 7110 HALT |
| --- |

| 78K/0 Memory Map and SFRs |
| --- |

**78K0 References**

```

| 78K0/Kx2 User's Manual: Hardware ;info on I/O Ports
 78K0/Kx2 Flash Memory Self Programming ;info on 8100h function
 78K0/Kx2 Flash Memory Programming ;info on UART/SPI cable
 78K/0 Series Instructions ;info on opcodes
 78K0/KB2 One-sheet Manual ;quick reference poster |
| --- |

**78K/0 Memory Map**

```

| 0000h..003Fh FLASH ROM Exception Vectors (16bit each)
 0040h..007Fh FLASH ROM CALLT Vectors (16bit each)
 0080h..0084h FLASH ROM Option Byte Area (5 bytes)
 0085h..008Eh FLASH ROM Debug Security Area (10 bytes, if any)
 008Fh..07FFh FLASH ROM
 0800h..0FFFh FLASH ROM (accessible via CALLF opcodes)
 1000h..1FFFh FLASH ROM (can be swapped with 0000h..0FFFh)
 2000h..xFFFh FLASH ROM (if any, in larger chips)
 8000h..BFFFh FLASH ROM (if any, in larger chips) (bank-switched via BANK)
 8000h..9FFFh System ROM area (when mapped, with function handler at 8100h)
 A000h..BFFFh System ROM mirror (same as 8000h..9FFFh)
 C000h.. System ROM empty (mostly FFh-filled, some FEh-bytes)
 E000h..F7FFh Extra RAM (max 6K, if any) (mostly FFh-filled, some F0h-bytes)
 F800h..F8FFh Undoc SFR's
 F900h..FAFFh Zerofilled
 FB00h..FBFFh Unknown (filled with random values) (more RAM?) (unused in DSi)
 (FB00h is ONLY mapped when ROM enabled, otherwise FFh-filled)
 FC00h..FF1Fh RAM (officially/used area)
 FE20h..FEDFh RAM (accessible via short "saddr" opcodes)
 FEE0h..FEE7h Register Bank 3 (X,A,C,B,E,D,L,H aka AX,BC,DE,HL)
 FEE8h..FEEFh Register Bank 2 (X,A,C,B,E,D,L,H aka AX,BC,DE,HL)
 FEF0h..FEF7h Register Bank 1 (X,A,C,B,E,D,L,H aka AX,BC,DE,HL)
 FEF8h..FEFFh Register Bank 0 (X,A,C,B,E,D,L,H aka AX,BC,DE,HL)
 FF00h..FFFFh Special Function Registers (SFR's) |
| --- |

FLASH banks are 1Kbyte tall (aka erase block size?).

**78K/0 Vectors**

```

| 0000h RESET input, POC, LVI, WDT
 0002h -
 0004h INTLVI
 0006h INTP0
 0008h INTP1
 000Ah INTP2
 000Ch INTP3
 000Eh INTP4
 0010h INTP5
 0012h INTSRE6
 0014h INTSR6
 0016h INTST6
 0018h INTCSI10/INTST0
 001Ah INTTMH1
 001Ch INTTMH0
 001Eh INTTM50
 0020h INTTM000
 0022h INTTM010
 0024h INTAD
 0026h INTSR0
 0028h INTWTI
 002Ah INTTM51
 002Ch INTKR
 002Eh INTWT
 0030h INTP6
 0032h INTP7
 0034h INTIIC0/NTDMU
 0036h INTCSI11
 0038h INTTM001
 003Ah INTTM011
 003Ch INTACSI
 003Eh BRK |
| --- |

**78K/0 Option Bytes / Debug Security**

```

| 0080h db 07Eh ;Option Byte: Watchdog Mode
 0081h db 000h ;Option Byte: POC Mode (0=1.59V, 1=2.7V)
 0082h db 000h ;Option Byte: Reserved (00h)
 0083h db 000h ;Option Byte: Reserved (00h)
 0084h db 000h ;Option Byte: On-chip Debug Control (0=Off)
 ;---
 0085h db 0CCh ;On-chip debug security ID (0Ah bytes) ...
 0086h db 023h
 0087h db 0BAh
 0088h db 0C3h
 0089h db 07Dh
 008Ah db 094h
 008Bh db 0FEh
 008Ch db 073h
 008Dh db 0A5h
 008Eh db 0E6h |
| --- |

**78K/0 Program Status Word (PSW) aka [FF1Eh]**

```

| 7 IE ;Interrupt Enable
 6 Z ;Zero Flag
 5 RBS1 ;Register Bank Select bit1
 4 AC ;Aux Carry Flag
 3 RBS0 ;Register Bank Select bit0
 2 0
 1 ISP ;In-Service Priority Flag
 0 CY ;Carry Flag |
| --- |

**78K/0 Special Function Registers (SFR's)**

```

| Addr Symbol R/W 1BW Reset BCDEF Special Function Register (SFR)
 FF00h P0 R/W 1B- 00h BCDEF Port 0
 FF01h P1 R/W 1B- 00h BCDEF Port 1
 FF02h P2 R/W 1B- 00h BCDEF Port 2
 FF03h P3 R/W 1B- 00h BCDEF Port 3
 FF04h P4 R/W 1B- 00h -CDEF Port 4
 FF05h P5 R/W 1B- 00h ---EF Port 5
 FF06h P6 R/W 1B- 00h BCDEF Port 6
 FF07h P7 R/W 1B- 00h -CDEF Port 7
 FF08h ADCR R --W 0000h BCDEF 10-bit A/D conversion result
 FF09h ADCRH R -B- 00h BCDEF 8-bit A/D conversion result
 FF0Ah RXB6 R -B- FFh BCDEF Receive buffer 6
 FF0Bh TXB6 R/W -B- FFh BCDEF Transmit buffer 6
 FF0Ch P12 R/W 1B- 00h BCDEF Port 12
 FF0Dh P13 R/W 1B- 00h -cDEF Port 13 (KC2: 48pin only)
 FF0Eh P14 R/W 1B- 00h -cDEF Port 14 (KC2: 48pin only)
 FF0Fh SIO10 R -B- 00h BCDEF Serial I/O shift 10
 FF10h TM00 R --W 0000h BCDEF 16-bit timer counter 00
 FF12h CR000 R/W --W 0000h BCDEF 16-bit timer capture/compare 000
 FF14h CR010 R/W --W 0000h BCDEF 16-bit timer capture/compare 010
 FF16h TM50 R -B- 00h BCDEF 8-bit timer counter 50
 FF17h CR50 R/W -B- 00h BCDEF 8-bit timer compare 50
 FF18h CMP00 R/W -B- 00h BCDEF 8-bit timer H compare 00
 FF19h CMP10 R/W -B- 00h BCDEF 8-bit timer H compare 10
 FF1Ah CMP01 R/W -B- 00h BCDEF 8-bit timer H compare 01
 FF1Bh CMP11 R/W -B- 00h BCDEF 8-bit timer H compare 11
 FF1Ch SP R/W --W xxxxh BCDEF CPU Stack Pointer ;\undocumented, used
 FF1Eh PSW R/W 1B- xxh BCDEF CPU Program Status Word;/in pseudo opcodes
 FF1Fh TM51 R -B- 00h BCDEF 8-bit timer counter 51
 FF20h PM0 R/W 1B- FFh BCDEF Port mode 0
 FF21h PM1 R/W 1B- FFh BCDEF Port mode 1
 FF22h PM2 R/W 1B- FFh BCDEF Port mode 2
 FF23h PM3 R/W 1B- FFh BCDEF Port mode 3
 FF24h PM4 R/W 1B- FFh -CDEF Port mode 4
 FF25h PM5 R/W 1B- FFh ---EF Port mode 5
 FF26h PM6 R/W 1B- FFh BCDEF Port mode 6
 FF27h PM7 R/W 1B- FFh -CDEF Port mode 7
 FF28h ADM R/W 1B- 00h BCDEF A/D converter mode
 FF29h ADS R/W 1B- 00h BCDEF Analog input channel specification
 FF2Ch PM12 R/W 1B- FFh BCDEF Port mode 12
 FF2Eh PM14 R/W 1B- FFh -cDEF Port mode 14 (KC2: 48pin only)
 FF2Fh ADPC R/W 1B- 00h BCDEF A/D port configuration
 FF30h PU0 R/W 1B- 00h BCDEF Pull-up resistor option 0
 FF31h PU1 R/W 1B- 00h BCDEF Pull-up resistor option 1
 FF33h PU3 R/W 1B- 00h BCDEF Pull-up resistor option 3
 FF34h PU4 R/W 1B- 00h -CDEF Pull-up resistor option 4
 FF35h PU5 R/W 1B- 00h ---EF Pull-up resistor option 5
 FF36h PU6 R/W 1B- 00h ----F Pull-up resistor option 6
 FF37h PU7 R/W 1B- 00h -CDEF Pull-up resistor option 7
 FF3Ch PU12 R/W 1B- 00h BCDEF Pull-up resistor option 12
 FF3Eh PU14 R/W 1B- 00h -cDEF Pull-up resistor option 14 (KC2: 48pin only)
 FF40h CKS R/W 1B- 00h -cDEF Clock output selection (KC2: 48pin only)
 FF41h CR51 R/W -B- 00h BCDEF 8-bit timer compare 51
 FF43h TMC51 R/W 1B- 00h BCDEF 8-bit timer mode control 51
 FF48h EGP R/W 1B- 00h BCDEF External interrupt rising edge enable
 FF49h EGN R/W 1B- 00h BCDEF External interrupt falling edge enable
 FF4Ah SIO11 R -B- 00h ---eF Serial I/O shift 11 (KE2: 48KB only)
 FF4Ch SOTB11 R/W -B- 00h ---eF Transmit buffer 11 (KE2: 48KB only)
 FF4Fh ISC R/W 1B- 00h BCDEF Input switch control
 FF50h ASIM6 R/W 1B- 01h BCDEF Async serial interface operation mode 6
 FF53h ASIS6 R -B- 00h BCDEF Async serial interface reception err/stat 6
 FF55h ASIF6 R -B- 00h BCDEF Async serial interface transmission status 6
 FF56h CKSR6 R/W -B- 00h BCDEF Clock selection 6
 FF57h BRGC6 R/W -B- FFh BCDEF Baud rate generator control 6
 FF58h ASICL6 R/W 1B- 16h BCDEF Async serial interface control 6
 FF60h SDR0 R -BW 0000h -cdeF Remainder data 0 bit0.. ;\
 FF61h SDR0H R -B- 00h -cdeF Remainder data 0 bit8.. ;
 FF62h MDA0L R/W -BW 0000h -cdeF Multiply/divide data A0 bit0.. ; KC2-KE2:
 FF63h MDA0LH R/W -B- 00h -cdeF Multiply/divide data A0 bit8.. ; 48KB only
 FF64h MDA0H R/W -BW 0000h -cdeF Multiply/divide data A0 bit16.. ;
 FF65h MDA0HH R/W -B- 00h -cdeF Multiply/divide data A0 bit24.. ;
 FF66h MDB0 R/W -BW 0000h -cdeF Multiply/divide data B0 bit0.. ;
 FF67h MDB0H R/W -B- 00h -cdeF Multiply/divide data B0 bit8.. ;
 FF68h DMUC0 R/W 1B- 00h -cdeF Multiply/divide control 0 ;/
 FF69h TMHMD0 R/W 1B- 00h BCDEF 8-bit timer H mode 0
 FF6Ah TCL50 R/W 1B- 00h BCDEF Timer clock selection 50
 FF6Bh TMC50 R/W 1B- 00h BCDEF 8-bit timer mode control 50
 FF6Ch TMHMD1 R/W 1B- 00h BCDEF 8-bit timer H mode 1
 FF6Dh TMCYC1 R/W 1B- 00h BCDEF 8-bit timer H carrier control 1
 FF6Eh KRM R/W 1B- 00h -CDEF Key return mode
 FF6Fh WTM R/W 1B- 00h -CDEF Watch timer operation mode
 FF70h ASIM0 R/W 1B- 01h BCDEF Async serial interface operation mode 0
 FF71h BRGC0 R/W -B- 1Fh BCDEF Baud rate generator control 0
 FF72h RXB0 R -B- FFh BCDEF Receive buffer 0
 FF73h ASIS0 R -B- 00h BCDEF Async serial interface reception err/stat 0
 FF74h TXS0 W -B- FFh BCDEF Transmit shift 0
 FF80h CSIM10 R/W 1B- 00h BCDEF Serial operation mode 10
 FF81h CSIC10 R/W 1B- 00h BCDEF Serial clock selection 10
 FF84h SOTB10 R/W -B- 00h BCDEF Transmit buffer 10
 FF88h CSIM11 R/W 1B- 00h ---eF Serial operation mode 11 (KE2: 48KB only)
 FF89h CSIC11 R/W 1B- 00h ---eF Serial clock selection 11 (KE2: 48KB only)
 FF8Ch TCL51 R/W 1B- 00h BCDEF Timer clock selection 51
 FF90h CSIMA0 R/W 1B- 00h ----F Serial operation mode specification 0
 FF91h CSIS0 R/W 1B- 00h ----F Serial status 0
 FF92h CSIT0 R/W 1B- 00h ----F Serial trigger 0
 FF93h BRGCA0 R/W -B- 03h ----F Division value selection 0
 FF94h ADTP0 R/W -B- 00h ----F Automatic data transfer address point spec 0
 FF95h ADTI0 R/W -B- 00h ----F Automatic data transfer interval spec 0
 FF96h SIOA0 R/W -B- 00h ----F Serial I/O shift 0
 FF97h ADTC0 R -B- 00h ----F Automatic data transfer address count 0
 FF99h WDTE R/W -B- xAh BCDEF Watchdog timer enable (initial=1Ah/9Ah)
 FF9Fh OSCCTL R/W 1B- 00h BCDEF Clock operation mode select
 FFA0h RCM R/W 1B- x0h BCDEF Internal oscillation mode
 FFA1h MCM R/W 1B- 00h BCDEF Main clock mode
 FFA2h MOC R/W 1B- 80h BCDEF Main OSC control
 FFA3h OSTC R 1B- 00h BCDEF Oscillation stabilization time counter stat
 FFA4h OSTS R/W -B- 05h BCDEF Oscillation stabilization time select
 FFA5h IIC0 R/W -B- 00h BCDEF IIC shift 0
 FFA6h IICC0 R/W 1B- 00h BCDEF IIC control 0
 FFA7h SVA0 R/W -B- 00h BCDEF Slave address 0
 FFA8h IICCL0 R/W 1B- 00h BCDEF IIC clock selection 0
 FFA9h IICX0 R/W 1B- 00h BCDEF IIC function expansion 0
 FFAAh IICS0 R 1B- 00h BCDEF IIC status 0
 FFABh IICF0 R/W 1B- 00h BCDEF IIC flag 0
 FFACh RESF R -B- xxh BCDEF Reset control flag
 FFB0h TM01 R --W 0000h ---eF 16-bit timer counter 01 ;\
 FFB2h CR001 R/W --W 0000h ---eF 16-bit timer capture/compare 001 ;
 FFB4h CR011 R/W --W 0000h ---eF 16-bit timer capture/compare 011 ; KE2:
 FFB6h TMC01 R/W 1B- 00h ---eF 16-bit timer mode control 01 ; 48KB
 FFB7h PRM01 R/W 1B- 00h ---eF Prescaler mode 01 ; only)
 FFB8h CRC01 R/W 1B- 00h ---eF Capture/compare control 01 ;
 FFB9h TOC01 R/W 1B- 00h ---eF 16-bit timer output control 01 ;/
 FFBAh TMC00 R/W 1B- 00h BCDEF 16-bit timer mode control 00
 FFBBh PRM00 R/W 1B- 00h BCDEF Prescaler mode 00
 FFBCh CRC00 R/W 1B- 00h BCDEF Capture/compare control 00
 FFBDh TOC00 R/W 1B- 00h BCDEF 16-bit timer output control 00
 FFBEh LVIM R/W 1B- xxh BCDEF Low-voltage detection
 FFBFh LVIS R/W 1B- xxh BCDEF Low-voltage detection level selection
 FFC0h PFCMD W? ?B? ? ? Undoc: Flash Protect Command (write A5h)
 FFC1h - R/W 1B- ? ? Undoc: Flash ... FFC1h ... bits/pulses FSSQ ?
 FFC2h PFS R ? ? Undoc: Flash Protect Status
 FFC4h FLPMC R/W 1B? ? ? Undoc: Flash Programming Mode Ctl ;triple write
 FFC5h - W 1?- ? ? Undoc: Flash ... 0FFC5h ... bits/mode ?
 FFC6h FLAPH W 1B- ? ? Undoc: Flash addr ptr upper 8bit (bit1=flag)
 FFC7h - R/W 1?- ? ? Undoc: Flash ... 0FFC7h ... flag bits
 FFC8h FLAPL W 1BW ? ? Undoc: Flash addr ptr lower 16bit (bit0,1=flag)
 FFCAh - R/W ?B? ? ? Undoc: Flash ... 0FFCAh ... ;triple write
 FFCBh FLWE W ?B- ? ? Undoc: Flash write data ECC error correction?
 FFCCh FLWH W -BW ? ? Undoc: Flash write data upper 16bit ;\4-byte
 FFCEh FLWL W -BW ? ? Undoc: Flash write data lower 16bit ;/
 FFE0h IF0L R/W 1BW 00h BCDEF Interrupt request flag 0L ;\IF0
 FFE1h IF0H R/W 1BW 00h BCDEF Interrupt request flag 0H ;/
 FFE2h IF1L R/W 1BW 00h BCDEF Interrupt request flag 1L ;\IF1
 FFE3h IF1H R/W 1BW 00h BCDEF Interrupt request flag 1H ;/
 FFE4h MK0L R/W 1BW FFh BCDEF Interrupt mask flag 0L ;\MK0
 FFE5h MK0H R/W 1BW FFh BCDEF Interrupt mask flag 0H ;/
 FFE6h MK1L R/W 1BW FFh BCDEF Interrupt mask flag 1L ;\MK1
 FFE7h MK1H R/W 1BW FFh BCDEF Interrupt mask flag 1H ;/
 FFE8h PR0L R/W 1BW FFh BCDEF Priority specification flag 0L ;\PR0
 FFE9h PR0H R/W 1BW FFh BCDEF Priority specification flag 0H ;/
 FFEAh PR1L R/W 1BW FFh BCDEF Priority specification flag 1L ;\PR1
 FFEBh PR1H R/W 1BW FFh BCDEF Priority specification flag 1H ;/
 FFF0h IMS R/W -B- CFh BCDEF Internal memory size switching
 FFF3h BANK R/W -B- 00h --def Memory bank select (KD2-KF2: min 96KB only)
 FFF4h IXS R/W -B- 0Ch bcdeF Internal expansion RAM size switching
 FFF5h - R ?B- ? ? Undoc: chip_id (package type and memory size)
 FFFBh PCC R/W 1B- 01h BCDEF Processor clock control |
| --- |

And, more undoc/secret registers:

```

| F800h..F0h - --- ? ? Undoc: unknown values, unused by software
 F8F1h FLrE R -B- ? ? Undoc: Flash read data ECC error correction?
 F8F2h FLrH R -BW ? ? Undoc: Flash read data upper 16bit ;\4-byte
 F8F4h FLrL R -BW ? ? Undoc: Flash read data lower 16bit ;/total
 F8F6h ??? R/W --W ? ? Undoc: Flash ...0F8F6h_word ;-16bit
 F8F8h - - --- ? ? Undoc: ;(unused by software)
 F8F9h ??? R/W -B- ? ? Undoc: Flash ...0F8F9h ;-
 F8FAh - - --- ? ? Undoc: ;(unused by software)
 F8FBh - - --- ? ? Undoc: ;(unused by software)
 F8FCh - - --- ? ? Undoc: ;(unused by software)
 F8FDh - - --- ? ? Undoc: ;(unused by software)
 F8FEh - - --- ? ? Undoc: ;(unused by software)
 F8FFh - - --- ? ? Undoc: ;(unused by software) |
| --- |

Whereas,

```

| 1BT = supports Bit/Byte/Word access (1bit/8bit/16bit)
 BCDEF = supported on KB2/KC2/KD2/KE2/KF2 chip versions |
| --- |

For IMS/IXS: Regardless of the internal memory capacity, the initial values of
the internal memory size switching register (IMS) and internal expansion RAM
size switching register (IXS) of all products in the 78K0/Kx2 microcontrollers
are fixed (IMS = CFH, IXS = 0CH). Therefore, set the value corresponding to
each product as indicated in Tables 3-1 and 3-2.

For IMS/IXS: The ROM and RAM capacities of the products with the on-chip debug
function can be debugged by setting IMS and IXS, according to the debug target
products. Set IMS and IXS according to the debug target products.

For IXS on KB2-KE2: Set IXS only in products with internal expansion RAM.

The reset values of RES, LVIM and LVIS vary depending on the reset source.

The reset value of WDTE is determined by setting of option byte.

The reset value of RCM is 00h immediately after a reset release but
automatically changes to 80h after oscillation accuracy stabilization of
high-speed internal oscillator has been waited.

| DSi Autoload on Warmboot |
| --- |

**Overview**

Launcher (and unlaunch) can be commanded to autoload a different title.

```

| 2000000h Autoload Parameters for newly loaded title ;<-- optional extra
 2000300h Autoload via numeric Title ID ;<-- official method
 2000800h Autoload via string "device:\path\filename" ;<-- alternate method
 2FFD800h Title List (jump-able titles for use at 2000300h)
 BPTWL[70h].bit0 Warmboot flag ;<-- required flag
 BPTWL[11h].bit0 Trigger reset ;<-- trigger reset |
| --- |

Moreover, autoload can occur on warmboot & coldboot: Launcher autoloads any
ROM cartridge with carthdr[01Fh].Bit2. Unlaunch defaults to autoload any file
named "sdmc:\bootcode.dsi".

**Examples**

DSi Browser settings screen allows to autoload System Settings (via 2000300h),
and automatically enter the Internet options page (via 2000000h).

Nintendo Zone, if started with Wireless Communications disabled, allows to
reboot itself (via specifying it's own Title ID in 2000300h).

Homebrew frontends for unlaunch could start themselves (via bootcode.dsi) and
then command unlaunch to load another title (via 2000800h or 2000300h).

**2000000h - Optional Auto-load parameters (passed on to new title)**

```

| 2000000h 8 AutoParam Old Title ID (former title) ;carthdr[230h]
 2000008h 1 AutoParam Unknown/Unused
 2000009h 1 AutoParam Flags (03h=Stuff is used?)
 200000Ah 2 AutoParam Old Maker code ;carthdr[010h]
 200000Ch 2 AutoParam Unknown (02ECh) ;\counter/length/indices/whatever?
 200000Eh 2 AutoParam Unknown (0000h) ;/
 2000010h 2 AutoParam CRC16 on [000h..2FFh], initial=FFFFh, [010h]=0000h
 2000012h 2 AutoParam Unknown/Unused (000Fh = want Internet Settings?)
 2000014h 2ECh AutoParam Unknown... some buffer... string maybe? |
| --- |

Above is the overall skeleton as intended by Nintendo, the purpose/format of
the 2ECh bytes is unknown (there seems to be some relation to entries [0Ch] and
[0Eh], but theoretically, each title could use that bytes as pleased).

**2000300h - Nintendo Auto-load feature (via numeric Title ID)**

```

| 2000300h 4 AutoLoad ID ("TLNC")
 2000304h 1 AutoLoad Unknown/unused (usually 01h)
 2000305h 1 AutoLoad Length of data at 2000308h (01h..18h,for CRC,18h=norm)
 2000306h 2 AutoLoad CRC16 of data at 2000308h (with initial value FFFFh)
 2000308h 8 AutoLoad Old Title ID (former title) (can be 0=anonymous)
 2000310h 8 AutoLoad New Title ID (new title to be started,0=none/launcher)
 2000318h 4 AutoLoad Flags (bit0, 1-3, 4, 5,6,7) ;usually 16bit, once 32bit
 200031Ch 4 AutoLoad Unused (but part of checksummed area when CRC len=18h)
 2000320h E0h AutoLoad Unused (but zerofilled upon erasing autoload area) |
| --- |

Flags (usually 13h or 17h):

```

| 0 IsValid (somehow enables/disables HealthSafety when TitleID is wrong?)
 1-3 Boottype (01h=Cartridge, 02h=Landing, 03h=DSiware) (see below)
 4 Unknown
 5 Unknown
 6 LoadCompl (causes some error when set) (loading completed flag?)
 7 Unknown
 8-15 Unused
 16-31 Unused (usually not accessed at all, with normal 16bit reads) |
| --- |

Boottypes (in Flags.bit1-3):

```

| 01h = Cartridge (with NewTitleID) (with RSA signed header, or Whitelisted)
 02h = Landing ("nand:/tmp/jump.app") (with RSA signed DownloadPlay footer)
 03h = DSiware (with NewTitleID) (with RSA signed header) |
| --- |

TitleID.LSW should match DSi cart header (or be reverse of NDS gamecode?)

TitleID.MSW should match DSi cart header (or be zero for NDS titles?)

Note: Many titles do create the above structure even when not requesting to
boot a new file: with NewTitleID=0=none & flags=13h=cartridge (in that case
flags should be ignored, and NewTitleID=0=none has priority).

**2000800h - Unlaunch Auto-load feature (via "device:/Path/Filename.ext")**

```

| 2000800h 12 Unlaunch Auto-load ID ("AutoLoadInfo")
 200080Ch 2 Unlaunch Length for CRC16 (fixed, must be 3F0h)
 200080Eh 2 Unlaunch CRC16 (across 2000810h..2000BFFh, initial value FFFFh)
 2000810h 4 Unlaunch Flags
 2000814h 2 Unlaunch Upper screen BG color (0..7FFFh)
 2000816h 2 Unlaunch Lower screen BG color (0..7FFFh)
 2000818h 20h Unlaunch Reserved (zero)
 2000838h 208h Unlaunch Device:/Path/Filename.ext (16bit Unicode,end by 0000h)
 2000A40h 1C0h Unlaunch Reserved (zero) |
| --- |

Unlaunch Flags (usually 01h):

```

| 0 Load the title at 2000838h
 1 Use colors 2000814h (use if loaded title is KNOWN to use such colors)
 2-31 Reserved (zero) |
| --- |

The name can use slashes or backslashes for folders, and it can use both long
or short filenames (LFN or 8.3). The total length should not exceed 260
characters including EOL (alike windows MAX_PATH, although WinNT seems to allow
up to 32K characters, which isn't supported here).

```

| "nand:/path/name.ext",0000h File on 1st partition of internal eMMC
 "sdmc:/path/name.ext",0000h File on 1st partition of external SD/MMC
 "cart:",0000h ROM cartridge (in NDS cartridge slot)
 "menu:",0000h Force starting unlaunch filemenu
 "sett:",0000h Force starting unlaunch options menu
 "wifi:",0000h Force starting unlaunch wifiboot overlay |
| --- |

Case-sensitivity: device, path and file can use upper/lower case A-Z (not
case-sensititive), however currently any other letters are case-sensitive (eg.
umlaut's or accented letters) (that is, they must be uppercase for short names,
or have matching case for long names).

For black colors, you should also disable backlights before issuing reset (else
screen will flash white for a short moment during initial forced blank; before
unlaunch gets started).

**2FFD800h - Title List**

Before autoloading a title, one should make sure that the title is actually
installed (and which region code it is having, ie. one should use wildcards
that ignore the lower 8bit of the Title ID when searching the title).

The offical way is to look up the Title List in RAM, this is faster than
manually crawling the directory tree. However, there are some restrictions: The
Title List contains only titles from the same Maker (as the currently loaded
title), plus some special "jumpable" system titles.

```

| 2FFD800h 1 Titles: Number of titles in below lists (max 76h)
 2FFD801h 0Fh Titles: Zerofilled
 2FFD810h 10h Titles: Pub Flags (1bit each) ;same maker plus public.sav
 2FFD820h 10h Titles: Prv Flags (1bit each) ;same maker plus private.sav
 2FFD830h 10h Titles: Jmp Flags (1bit each) ;jumpable or current-title
 2FFD840h 10h Titles: Mkr Flags (1bit each) ;same maker
 2FFD850h 3B0h Titles: Title IDs (8 bytes each) |
| --- |

Related carthdr entries are:

```

| [010h].bit0-15 Maker (must match current title for Mkr Flags)
 [01Dh].bir0 Jump (must be set for Jmp Flags)
 [230h].bit0-63 Title ID (must be nonzero for being listed)
 [238h].bit0-31 Public.sav size (must be nonzero for Pub Flags)
 [23Ch].bit0-31 Private.sav size (must be nonzero for Prv Flags) |
| --- |

The list can contain the hidden Nintendo Zone utility, and DSi ROM cartridges
(both provided that Maker does match up with current title).

The jumpable titles with [01Dh].bit0 that are always in the list are:

```

| 00030015484E42xxh ;System Settings
 00030005484E4441h ;DS Download Play
 00030005484E4541h ;Pictochat
 00030004484E47xxh ;Nintendo DSi Browser (if installed) |
| --- |

The list does NOT contain the Launcher itself, nor files from System Data
folder (WifiFirmware, Whitelist, VersionData), nor NDS ROM cartridges, nor
anything where Jmp+Mkr flags would be both zero.

If started via 2000300h, then the New Title (from 2000310h) does also receive
the Old Title ID (from 2000308h) with Jmp flag being set; ie. permitting to
return to the Old Title (to know which title was the old title, one should
probably look at 2000000h, or at 2000308h if that's still intact in memory?).

Also, the Jmp flag is always set for the current title; ie. permitting the
title to reboot itself.

| DSi Aptina Camera Initialization |
| --- |

Aptina I2C registers are accessed via 16bit index, and one or more data bytes
at auto-increasing indices (usually, all transfers are done as big-endian
2-byte (16bit) values at even indices). Additional MCU Variables are accessed
indirectly via XMDA registers.

**aptina_get_chip_id:**

Reading a 16bit value from index 0000h returns the CHIP_VERSION_REG (always
2280h=MT9V113 for DSi/3DS/New3DS), the DSi games are actually reading that
register, but they seem to ignore it's value.

If the DSi isn't fitted with Aptina cameras, then reading anything from device
78h/7Ah would most likely return FFh-bytes.

**brightness / low light environments**

Below configurations are okay for daylight (without much sunshine), but the
picture will be almost completely black at night (in rooms with small bulbs).
There are probably numerous good/bad ways to manipulate the brightness. Some
random solutions are:

Leave AE_MIN_INDEX/AE_MAX_INDEX at their power-on defaults (instead of using
below settings) (the power-on defaults will greatly improve the brightness, but
the conversion will be also much slower). Increasing COARSE_INTEGRATION_TIME to
some big value (like 0800h) does also seem to raise the brightness.

**aptina_code_list_init:**

Below is some minimal initialization (though it might still include some
unnecessary stuff). Most important sections are leaving standby mode, matching
PLL to DSi timings, selecting desired resolution(s) and YUV color format.

DSi games are usually initializing further stuff like P0..P4 Coefficients,
Gamma Tables, and Color Correction Matrices - but the cameras are also working
when leaving those settings at their power-on defaults.

It's recommended to initialize both cameras in parallel (eg. issue Wakeup to
<both> cameras, and <then> wait for Wakeup completion; this is
faster than doing it separately for each camera).

```

| AptWr ,0001Ah,00003h ;RESET_AND_MISC_CONTROL (issue reset) ;\reset
 AptWr ,0001Ah,00000h ;RESET_AND_MISC_CONTROL (release reset) ;/
 AptWr ,00018h,04028h ;STANDBY_CONTROL (wakeup) ;\
 AptWr ,0001Eh,00201h ;PAD_SLEW ; wakeup
 AptWr ,00016h,042DFh ;CLOCKS_CONTROL ;
 AptWaitClr,00018h,04000h ;STANDBY_CONTROL (wait for WakeupDone) ;
 AptWaitSet,0301Ah,00004h ;UNDOC_CORE_301A (wait for WakeupDone) ;/
 AptWrMcu ,002F0h,00000h ;UNDOC! RAM?
 AptWrMcu ,002F2h,00210h ;UNDOC! RAM?
 AptWrMcu ,002F4h,0001Ah ;UNDOC! RAM?
 AptWrMcu ,02145h,002F4h ;UNDOC! SEQ?
 AptWrMcu ,0A134h, 001h ;UNDOC! SEQ?
 AptSetMcu ,0A115h, 002h ;SEQ_CAP_MODE (set bit1=video)
 AptWrMcu ,02755h,00002h ;MODE_OUTPUT_FORMAT_A (bit5=0=YUV) ;\select
 AptWrMcu ,02757h,00002h ;MODE_OUTPUT_FORMAT_B ;/YUV mode
 AptWr ,00014h,02145h ;PLL_CONTROL ;\
 AptWr ,00010h,00111h ;PLL_DIVIDERS ; match
 AptWr ,00012h,00000h ;PLL_P_DIVIDERS ; PLL
 AptWr ,00014h,0244Bh ;PLL_CONTROL ; to DSi
 AptWr ,00014h,0304Bh ;PLL_CONTROL ; timings
 AptWaitSet,00014h,08000h ;PLL_CONTROL (wait for PLL Lock okay) ;
 AptClr ,00014h,00001h ;PLL_CONTROL (disable PLL Bypass) ;/
 AptWrMcu ,02703h,00100h ;MODE_OUTPUT_WIDTH_A ;\Size A
 AptWrMcu ,02705h,000C0h ;MODE_OUTPUT_HEIGHT_A ;/ 256x192
 AptWrMcu ,02707h,00280h ;MODE_OUTPUT_WIDTH_B ;\Size B
 AptWrMcu ,02709h,001E0h ;MODE_OUTPUT_HEIGHT_B ;/ 640x480
 AptWrMcu ,02715h,00001h ;MODE_SENSOR_ROW_SPEED_A ;\
 AptWrMcu ,02719h,0001Ah ;MODE_SENSOR_FINE_CORRECTION_A ;
 AptWrMcu ,0271Bh,0006Bh ;MODE_SENSOR_FINE_IT_MIN_A ; Sensor A
 AptWrMcu ,0271Dh,0006Bh ;MODE_SENSOR_FINE_IT_MAX_MARGIN_A ;
 AptWrMcu ,0271Fh,002C0h ;MODE_SENSOR_FRAME_LENGTH_A ;
 AptWrMcu ,02721h,0034Bh ;MODE_SENSOR_LINE_LENGTH_PCK_A ;/
 AptWrMcu ,0A20Bh, 000h ;AE_MIN_INDEX ;\AE min/max
 AptWrMcu ,0A20Ch, 006h ;AE_MAX_INDEX ;/
 AptWrMcu ,0272Bh,00001h ;MODE_SENSOR_ROW_SPEED_B ;\
 AptWrMcu ,0272Fh,0001Ah ;MODE_SENSOR_FINE_CORRECTION_B ;
 AptWrMcu ,02731h,0006Bh ;MODE_SENSOR_FINE_IT_MIN_B ; Sensor B
 AptWrMcu ,02733h,0006Bh ;MODE_SENSOR_FINE_IT_MAX_MARGIN_B ;
 AptWrMcu ,02735h,002C0h ;MODE_SENSOR_FRAME_LENGTH_B ;
 AptWrMcu ,02737h,0034Bh ;MODE_SENSOR_LINE_LENGTH_PCK_B ;/
 AptSet ,03210h,00008h ;COLOR_PIPELINE_CONTROL (PGA pixel shading..)
 AptWrMcu ,0A208h, 000h ;UNDOC! RESERVED_AE_08
 AptWrMcu ,0A24Ch, 020h ;AE_TARGETBUFFERSPEED
 AptWrMcu ,0A24Fh, 070h ;AE_BASETARGET
 If Device=7Ah ;\
 AptWrMcu,02717h,00024h ;MODE_SENSOR_READ_MODE_A ; Read Mode
 AptWrMcu,0272Dh,00024h ;MODE_SENSOR_READ_MODE_B ; with x-flip
 Else (xflip) ; on internal
 AptWrMcu,02717h,00025h ;MODE_SENSOR_READ_MODE_A ; camera
 AptWrMcu,0272Dh,00025h ;MODE_SENSOR_READ_MODE_B ;/
 If Device=7Ah ;\
 AptWrMcu,0A202h, 022h ;AE_WINDOW_POS ;
 AptWrMcu,0A203h, 0BBh ;AE_WINDOW_SIZE ;
 Else (?) ;
 AptWrMcu,0A202h, 000h ;AE_WINDOW_POS ;
 AptWrMcu,0A203h, 0FFh ;AE_WINDOW_SIZE ;/
 AptSet ,00016h,00020h ;CLOCKS_CONTROL (set bit5=1, reserved)
 AptWrMcu ,0A115h, 072h ;SEQ_CAP_MODE (was already manipulated above)
 AptWrMcu ,0A11Fh, 001h ;SEQ_PREVIEW_1_AWB ;\
 If Device=7Ah ;
 AptWr ,0326Ch,00900h ;APERTURE_PARAMETERS ;
 AptWrMcu,0AB22h, 001h ;HG_LL_APCORR1 ;
 Else (?) ;
 AptWr ,0326Ch,01000h ;APERTURE_PARAMETERS ;
 AptWrMcu,0AB22h, 002h ;HG_LL_APCORR1 ;/
 AptWrMcu ,0A103h, 006h ;SEQ_CMD (06h=RefreshMode)
 AptWaitMcuClr,0A103h, 00Fh ;SEQ_CMD (wait above to become ZERO)
 AptWrMcu ,0A103h, 005h ;SEQ_CMD (05h=Refresh)
 AptWaitMcuClr,0A103h, 00Fh ;SEQ_CMD (wait above to become ZERO) |
| --- |

Above does set two Mode/Contexts, 256x192 and 640x480. Yet unknown how to
activate the latter one.

**aptina_code_list_activate:**

```

| AptClr ,00018h,00001h ;STANDBY_CONTROL (bit0=0=wakeup) ;\
 AptWaitClr,00018h,04000h ;STANDBY_CONTROL (wait for WakeupDone) ; Wakeup
 AptWaitSet,0301Ah,00004h ;UNDOC_CORE_301A (wait for WakeupDone) ;/
 AptWr ,03012h,000xxh ;COARSE_INTEGRATION_TIME (Y Time)
 AptSet ,0001Ah,00200h ;RESET_AND_MISC_CONTROL (Parallel On) ;-Data on |
| --- |

Also, don't forget to activate the Camera LED via BPTWL chip (when using the
external camera).

**aptina_code_list_deactivate:**

Before activating another camera: First disable the Parallel output of the old
camera (for avoiding collisions on the camera's parallel databus). When not
using the camera for longer time, also enter standby mode (for reducing power
consumption).

```

| AptClr ,0001Ah,00200h ;RESET_AND_MISC_CONTROL (Parallel Off) ;-Data off
 AptSet ,00018h,00001h ;STANDBY_CONTROL (set bit0=1=Standby) ;\
 AptWaitSet,00018h,04000h ;STANDBY_CONTROL (wait for StandbyDone) ; Standby
 AptWaitClr,0301Ah,00004h ;UNDOC_CORE_301A (wait for StandbyDone) ;/ |
| --- |

**References**

There aren't any MT9V113 specs released, but info for MT9D113 (a higher
resolution variant) does exist: a pdf datasheet, and an xml reference for the
I2C registers.

There are several source code files for MT9V113 cameras (different files from
different people; for use with linux/android/whatever) including samples for
adjusting stuff like contrast and sharpness. However, observe that the source
code may need some adjustments: PLL register matched to DSi timings, and, use
YUV 8bit parallel databus transfer for DSi.

| DSi Aptina Camera Registers: SYSCTL (0000h-0051h) |
| --- |

**SYSCTL (0000h-0051h)**

```

| 0000h 2 CHIP_VERSION_REG Model ID (2280h=MT9V113 on DSi/3DS) (R)
 0006h .. RESERVED_SYSCTL_06 Reserved
 0010h 2 PLL_DIVIDERS PLL Dividers (def=0366h)
 0-7 PLL M-Divider value (uh, actually a Multiplier?!)
 8-13 PLL N-Divider value
 14-15 Unused (0)
 Because the input clock frequency is unknown, the sensor starts
 up with the PLL disabled. The PLL takes time to power up. During
 this time, the behavior of its output clock signal is not
 guaranteed. The PLL output frequency is determined by two
 constants, M and N, and the input clock frequency.
 VCO = Fin * 2 * M / (N+1)
 PLL_output_frequency = VCO / (P1+1)
 The PLL can generate a master clock signal whose frequency is up
 to 85 MHz (input clock from 6 MHz through 54 MHz).
 0012h 2 PLL_P_DIVIDERS PLL P Dividers (def=00F5h)
 0-3 P1 (00h..0Fh)
 4-7 Unspecified
 8-11 P3 (00h..0Fh)
 12-13 Division ratio of word clock/clockn from bit_clock (0..3)
 14 Unused (0)
 15 Unspecified
 0014h 2 PLL_CONTROL PLL Control (def=21F9h)
 0 PLL Bypass
 1 PLL Enable
 2-3 Reserved (0..3)
 4-7 Reserved (0..0Fh)
 8 Reset_cntr
 9 Reserved
 10 Reserved
 11 Reserved
 12 Reserved
 13 Reserved
 14 Unused (0)
 15 PLL Lock (R)
 0016h 2 CLOCKS_CONTROL Clocks Control
 0 Reserved
 1 Reserved
 2 Reserved
 3 Reserved
 4 Reserved
 5 Reserved/UNDOC/USED (manipulated by DSi)
 6 Reserved
 7 Reserved
 8 Reserved
 9 clk_clkin_en
 11-12 Reserved
 13 Reserved
 15 Reserved
 0018h 2 STANDBY_CONTROL Standby Control and Status (def=4029h)
 0 Ship (uh?) (0=Enable various regs, 1=Standby)
 1 Reserved
 2 Stop MCU
 3 en_IRQ
 4 Reserved
 5 Reserved
 6-13 Unused (0)
 14 Standby_done (0=WakeupDone, 1=StandbyDone) (R?)
 (takes MUCH time?)
 15 Reserved (R)
 001Ah 2 RESET_AND_MISC_CONTROL Reset and Control (def=0050h) (0-0333h)
 0 Reset SOC I2C
 1 MIPI_TX_Reset
 2 Unused (0)
 3 MIPI_TX_en (=Serial Data?)
 4 IP_PD_en (=Parallel Data or what?)
 5 Reserved
 6 Sensor_full_res
 7 Unused (0)
 8 OE_N_Enable
 9 Parallel_enable (=Parallel Data?)
 10 Unused (0)
 11 Reserved
 12-15 Unused (0)
 001Ch 2 MCU_BOOT_MODE MCU Boot Mode
 0 Reset MCU
 1 Reserved
 2 Reserved
 3 Reserved
 4-7 Reserved (0..0Fh)
 8-15 Reserved (0..FFh) (R)
 001Eh 2 PAD_SLEW Pad Slew Control (def=0400h)
 0-2 Parallel Data Output Slew Rate Control (0-7)
 3 Unused (0)
 4-6 GPIO Slew Rate Control (0-7)
 7 Unused (0)
 8-10 PCLK aka PXLCLK Slew Rate Control (0-7)
 11-15 Unused (0)
 0020h .. RESERVED_SYSCTL_20 Reserved
 0022h 2 VDD_DIS_COUNTER VDD_DIS_COUNTER (0..FFFFh, def=0438h)
 0024h 2 GPI_STATUS GPI_STATUS (0..000Fh) (R)
 0026h .. RESERVED_SYSCTL_26 Reserved
 0028h 2 EN_VDD_DIS_SOFT EN_VDD_DIS_SOFT (0..0001h, def=0001h)
 0050h .. RESERVED_SYSCTL_50 Reserved |
| --- |

| DSi Aptina Camera Registers: RX_SS, FUSE, XDMA (0100h-099Fh) |
| --- |

**RX_SS (0100h-0117h)**

```

| 0100h .. RESERVED_RX_SS_100 Reserved
 0102h 2 TEST_PXL_RED Test Pixel Red ;\Default value is 1FFh
 0104h 2 TEST_PXL_G1 Test Pixel Green1 ; for Gray Flat Field
 0106h 2 TEST_PXL_G2 Test Pixel Green2 ; (0..03FFh, def=01FFh)
 0108h 2 TEST_PXL_BLUE Test Pixel Blue ;/
 010Ah .. RESERVED_RX_SS_10A-116 Reserved |
| --- |

**FUSE_ROM (0800h-081Fh)**

Reserved, unknown purpose, all zero in DSi.

```

| 0800h .. RESERVED_FUSE_ROM_800-81E Reserved |
| --- |

**XDMA (0982h-099Fh)**

Access to internal LOGICAL "driver" variables.

```

| 0982h .. RESERVED_XDMA_982 Reserved
 098Ch 2 MCU_ADDRESS MCU Address (0000h..FFFFh)
 0-7 driver_variable (0..FFh)
 8-12 driver_id (0..1Fh) (eg. 3=AWB, 7=MODE, etc.)
 13-14 address space (0=Physical/RAM/SFR, 1=Logical/Variables)
 15 access_8_bit (0=16bit, 1=8bit; converted to 16bit)
 0990h 8x2 MCU_DATA_0-7 MCU Data 0..7 (8 x 16bit) |
| --- |

For reading, it's best to use "16bit" mode, no matter if reading an 8bit BYTE,
or a 16bit MSB,LSB value. The "8bit" mode is converting bytes to 16bit values
(MSB=00h, LSB=BYTE), which is a rather contraproductive idiotism; intended for
I2C functions that implement only 16bit data transfers, but no 8bit transfers.

Unknown what exactly is mapped at MCU_DATA_0-7 (probably the 16 bytes at
MCU_ADDRESS+0..15, probably with direct mapping / ie. without latching a copy
of that memory).

MCU_ADDRESS doesn't seem to increment after reading data, however, the i2c
index does increase, so one can probably read up to 16 bytes from MCU_DATA_0-7.

| DSi Aptina Camera Registers: CORE (3000h-31FFh, 38xxh) |
| --- |

**CORE (3000h-31FFh)**

```

| 3000h .. RESERVED_CORE_3000 Reserved (same as CHIP_VERSION_REG)
 3002h 2 Y_ADDR_START Y1 ;\Image Position/Size ;def=0004h
 3004h 2 X_ADDR_START X1 ; (up to including ;def=0004h
 3006h 2 Y_ADDR_END Y2 ; X2,Y2) (0-07FFh) ;def=04BBh
 3008h 2 X_ADDR_END X2 ;/ ;def=064Bh
 300Ah 2 FRAME_LENGTH_LINES Y Total ;\Total X/Y Size with ;def=0512h
 300Ch 2 LINE_LENGTH_PCK X Total ;/blanking (0..FFFFh) ;def=0886h
 3010h .. RESERVED_CORE_3010 Reserved
 3012h 2 COARSE_INTEGRATION_TIME Y Time ;\Integration Time in ;def=0010h
 3014h 2 FINE_INTEGRATION_TIME X Time ;/lines/pix (0..FFFFh);def=00F6h
 3016h 2 ROW_SPEED Row Speed (def=0111h)
 0-2 Pixclk_speed (0..7)
 3 Unused (0)
 4-6 Reserved
 7 Unused (0)
 8-10 Reserved
 11-15 Unused (0)
 3018h .. RESERVED_CORE_3018-3019 Reserved
 301Ah UNDOC_CORE_301A Undocumented Status Reg (mask=D7FFh)
 0-1 Unspecified
 2 Undoc/USED (1=WakeupDone) (opposite of 0018h.bit14)
 3-4 Unspecified
 5 Whatever "demo_system, version_reg_write, value=1"
 6-8 Unspecified
 9 Mask_corrupted_frames (alias of 3022h.bit0)
 10 Unspecified
 11 Unused (0)
 12 Unspecified
 13 Unused (0)
 14 Unspecified
 15 Grouped_parameter_hold (alias of 3022h.bit8)
 301Ch .. RESERVED_CORE_301C-3020 Reserved
 3022h 2 GROUPED_PARAMETER_HOLD_MASK_CORRUPTED_FRAMES
 0 Mask_corrupted_frames (alias of Reg 301Ah.bit9)
 1-7 Unused (0)
 8 Grouped_parameter_hold (alias of Reg 301Ah.bit15)
 9-15 Unused (0)
 3024h 2 PIXEL_ORDER Pixel Order (mask=0300h, 0..0300h) (R)
 3026h .. RESERVED_CORE_3026 Reserved
 3028h 2 ANALOGUE_GAIN_CODE_GLOBAL Analog Global ;\
 302Ah 2 ANALOGUE_GAIN_CODE_GREENR Analog GreenR ; Analogue Gain Codes
 302Ch 2 ANALOGUE_GAIN_CODE_RED Analog Red ; with 3bit fraction
 302Eh 2 ANALOGUE_GAIN_CODE_BLUE Analog Blue ; (0..007Fh, def=000Bh)
 3030h 2 ANALOGUE_GAIN_CODE_GREENB Analog GreenB ;/
 3032h 2 DIGITAL_GAIN_GREENR Digital GreenR ;\Digital Gain with
 3034h 2 DIGITAL_GAIN_RED Digital Red ; 8bit dummy-fraction
 3036h 2 DIGITAL_GAIN_BLUE Digital Blue ; (bit8-10=Gain, 0..7)
 3038h 2 DIGITAL_GAIN_GREENB Digital GreenB ;/(mask=0700h,def=100h)
 303Ah .. RESERVED_CORE_303A-3C Reserved
 3040h 2 READ_MODE Read Mode (0-DEFFh, def=0024h)
 0 horiz_mirror
 1 vert_flip
 2-4 y_odd_inc (0..7)
 5-7 x_odd_inc (0..7)
 8 Unused (0)
 9 low_power
 10 xy_bin_en
 11 x_bin_en
 12 bin_sum (Enable summing mode for binning)
 13 read_mode_y_sumen
 14 Reserved
 15 Reserved
 3044h .. RESERVED_CORE_3044-3048 Reserved
 304Ah 2 OTPM_CONTROL One-time Programmable Memory? Control
 0 auto_wr_start ;\
 1 auto_wr_end (finished) (R) ; automatic write sequence
 2 auto_wr_success (okay) (R) ;/
 3 unspecified
 4 auto_rd_start ;\
 5 auto_rd_end (finished) (R) ; automatic read sequence
 6 auto_rd_success (okay) (R) ;/
 7-15 Unused (0)
 3050h .. RESERVED_CORE_3050-3054 Reserved
 3056h 2 GREEN1_GAIN Gain Green1 ;\
 3058h 2 BLUE_GAIN Gain Blue ; Gain Values
 305Ah 2 RED_GAIN Gain Red ; (0..0FFFh,
 305Ch 2 GREEN2_GAIN Gain Green2 ; def=022Ch)
 0-6 Initial Gain (0..7Fh, with 5bit fraction) ;
 7-8 Analog Gain (0..3) (bit8+1)*(bit7+1)*(initial_gain/32)
 9-11 Digital Gain (1..7) ;
 12-15 Unused (0) ;/
 305Eh .. RESERVED_CORE_305E-31DF Reserved
 31E0h 2 UNDOC_CORE_31E0 (mask=E003h, 0..8001h, def=0001h) USED!
 Used by DSi (set to 0001h) (reportedly "PIX_DEF_ID")
 31E2h .. RESERVED_CORE_31E2-31F9 Reserved
 31FAh 2 UNDOC_CORE_31FA Whatever (mask=FFFFh, def=CDEFh)
 0-4 Unspecified
 5-11 Whatever "demo_system, version_reg_read, value=3"
 12-15 Unspecified
 31FCh .. RESERVED_CORE_305E-31FE Reserved |
| --- |

**More CORE (3800h..3803h)**

```

| 3800h .. RESERVED_CORE_3800-3802 Reserved |
| --- |

| DSi Aptina Camera Registers: SOC1 (3210h-33FDh) |
| --- |

**SOC1 Registers (3210h-33FDh)**

```

| 3210h 2 COLOR_PIPELINE_CONTROL (mask=05B8h, 0..05B0h, def=01B0h)
 3 Enable PGA pixel shading correction
 All coefficients and other configuration settings
 (including other fields in this register) must be set up
 before enabling shading correction.
 4 Enable 2D aperture correction
 5 Enable color correction
 7 Enable gamma correction
 8 Decimator (1=Enable scale)
 10 Reserved
 3216h .. RESERVED_SOC1_3216-321A Reserved
 321Ch 2 OFIFO_CONTROL_STATUS Ofifo control status 1 (def=0003h)
 0-3 txfifo_bypass
 (0=tx_fifo, 1=sensor, 2=sam observe, 3=cpipe format,
 4=test walking ones cpipe frequency,
 5=test walking ones sensor frequency,
 6=RESERVED, 7=test PIXCLK, 8..F=Unspecified)
 4-6 Unused (0)
 7 sensor_bypass (0=cpipe, 1=sensor)
 8 Reserved
 9 Reserved
 10 Reserved
 11 Reserved
 12 Reserved (R)
 13 Reserved (R)
 14 Reserved (R)
 15 Reserved (R)
 321Eh 2 OFIFO_CONTROL_STATUS_2 Ofifo control status 2 (def=0010h)
 0-9 Reserved (0..3FFh)
 10 Disable PV output clock during blank (1=disable)
 11-15 Reserved (0..1Fh)
 3220h .. RESERVED_SOC1_3220 Reserved
 3222h 2 LOWER_X_BOUND_ZOOM_WINDOW Lower X ;def=? ;\Zoom Window
 3224h 2 UPPER_X_BOUND_ZOOM_WINDOW Upper X ;def=063Fh ; Boundaries
 3226h 2 LOWER_Y_BOUND_ZOOM_WINDOW Lower Y ;def=? ; (0..07FFh)
 3228h 2 UPPER_Y_BOUND_ZOOM_WINDOW Upper Y ;def=04AFh ;/
 322Ah 2 UNDOC_SOC1_322A (mask=0016h, 0..0016h) USED by DSi!
 322Ch 2 WEIGHT_HORIZ_DECIMATION Scaling Weight X ;\Scaling Weight X,Y
 322Eh 2 WEIGHT_VERTICAL_DECIMATION Scaling Weight Y ;/(0..0FFFh, def=800h)
 323Eh 2 UNDOC_SOC1_323E (0..FFFFh, def=1A2Dh) (DSi: C22Ch)
 3240h 2 UNDOC_SOC1_3240 (0..FFFFh, def=C814h) (DSi: 6214h)
 3242h .. RESERVED_SOC1_3242 Reserved
 3244h 2 UNDOC_SOC1_3244 (mask=03FFh, range=0..00FFh?, def=0310)
 3254h .. RESERVED_SOC1_3254-326A Reserved
 326Ch 2 APERTURE_PARAMETERS Aperture Params (0..7FFFh, def=0A08h)
 0-7 2D aperture threshold (knee) (00h-FFh)
 8-10 2D aperture gain (0-7)
 11-13 2D aperture gain's exponent (0-7)
 14 Abs (1=force aperture gain be positive)
 15 Unused (0)
 326Eh .. RESERVED_SOC1_326E-3276 Reserved
 327Ah 2 BLACK_LEVEL_1ST_RED Offset Red ;\Offsets subtracted
 327Ch 2 BLACK_LEVEL_1ST_GREEN1 Offset Green1 ; from RGB pixels
 327Eh 2 BLACK_LEVEL_1ST_GREEN2 Offset Green2 ; (0000-01FFh/03FFh,
 3280h 2 BLACK_LEVEL_1ST_BLUE Offset Blue ;/def=002Ah)
 328Eh 2 THRESH_EDGE_DETECT Demosaic Edge Threshold (def=000Ch)
 3290h 2 TEST_PATTERN Test Pattern Enable/Width
 0-4 Unused (0)
 5 en_walk_ones_tp Enable Test Pattern (0=disable, 1=enable)
 6 walk_ones_10 Pattern Width (0=8-bit, 1=10-bit)
 7-15 Unused (0)
 329Eh .. RESERVED_SOC1_329E-32A0 Reserved
 32C0h 2 COLOR_CORR_MATRIX_SCALE_14 Exponents C11..C22 (0-7FFFh, def=3923h)
 32C2h 2 COLOR_CORR_MATRIX_SCALE_11 Exponents C23..C33 (0-0FFFh, def=0724h)
 32C4h 2 COLOR_CORR_MATRIX_1_2 Elements C11=LSB, C12=MSB (def=7DCCh)
 32C6h 2 COLOR_CORR_MATRIX_3_4 Elements C13=LSB, C21=MSB (def=2711h)
 32C8h 2 COLOR_CORR_MATRIX_5_6 Elements C22=LSB, C23=MSB (def=62E5h)
 32CAh 2 COLOR_CORR_MATRIX_7_8 Elements C31=LSB, C32=MSB (def=690Dh)
 32CCh 2 COLOR_CORR_MATRIX_9 Element C33=LSB, Signs=MSB (def=2DCDh)
 32D4h 2 DIGITAL_GAIN_1_RED Gain for Red channel ;\Digital Gain1
 32D6h 2 DIGITAL_GAIN_1_GREEN1 Gain for Green1 channel ; (mul 128,
 32D8h 2 DIGITAL_GAIN_1_GREEN2 Gain for Green2 channel ; 0000h..03FFh,
 32DAh 2 DIGITAL_GAIN_1_BLUE Gain for Blue channel ;/def=0080h)
 32F4h .. RESERVED_SOC1_32F4-332E Reserved
 3330h 2 OUTPUT_FORMAT_TEST OUTPUT_FORMAT_TEST (0..0FFFh)
 0 Disable Cr channel
 1 Disable Y channel
 2 Disable Cb channel
 3-5 Test ramp output
 6 8+2 bypass
 7 Reserved
 8 Enable Lens Correction Bypass
 9 Reserved
 10 Reserved
 11 Reserved
 12-15 Unused (0)
 3332h .. RESERVED_SOC1_3332-334A Reserved
 337Ch 2 YUV_YCBCR_CONTROL YUV_YCBCR_CONTROL (0..000Fh, def=0006h)
 0 Mult_y_uv (normalize Y in 16-235; U and V in 16-240)
 1 Coefficient control
 2 Add 128 to U and V
 3 Clip Y in 16-235; U and V in 16-240
 4-15 Unused (0)
 337Eh 2 Y_RGB_OFFSET Y_RGB Offset
 0-7 Reserved (0..FFh)
 8-15 Y offset (0..FFh)
 33E6h .. RESERVED_SOC1_33E6-33EE Reserved
 33F4h 2 KERNEL_CONFIG Kernel Config (0..01FFh, def=0003h)
 0 Defect correction (DC) enable
 1 Reserved
 2 Reserved
 3 Noise reduction (NR) enable
 4 Reserved
 5 Reserved
 6 Reserved
 7 Reserved
 8 Reserved
 33F6h .. RESERVED_SOC1_33F6-33FC Reserved |
| --- |

| DSi Aptina Camera Registers: SOC2 (3400h-3729h) |
| --- |

**SOC2 (3400h-3729h)**

```

| 3400h 2 MIPI_CONTROL MIPI_Control (def=782Eh)
 0 MIPI restart enable
 1 MIPI standby
 2 Continuous MIPI clock
 3 Frame boundary sync bit (R)
 4 Wait until eof to react to standby
 5 Reserved
 6-8 MIPI channel number
 9 Unused (0) or Reserved (REV3)
 10-15 Data Type (1Eh=YUV422_8bit, 20h=RGB444, 21h=RGB555,
 22h=RGB565, 2Ah=RAW8, 2Bh=RAW10)
 3402h 2 MIPI_STATUS MIPI_Status (def=0011h)
 0 MIPI in standby (R)
 1-3 Unused (0)
 4 MIPI aka MIPICCP idle (R)
 5 MIPI ready to receive data (R)
 6-8 Unused (0)
 9 Reserved (R)
 10 Reserved (R)
 11 Reserved
 12 Reserved
 13-15 Unused (0)
 3404h 2 CUSTOM_SHORT_PKT MIPI_Custom_Short_Packet (0000h-3F00h)
 0-5 Unused (0)
 6 frame_cnt_reset (sent in frame start/end short packets)
 7 frame_cnt_en (Insert frame counter value in WC field)
 8-10 custom_short_packet_data_type
 11 custom_short_packet_request
 12 custom_short_packet_frame_sync
 13 custom_short_packet_reset (R)
 14-15 Unused (0)
 3408h 2 LINE_BYTE_CNT MIPI line byte count (def=0C80h)
 340Ch 2 CUSTOM_SHORT_PKT_WC WC field of a custom short packet
 340Eh .. RESERVED_SOC2_340E-341A Reserved
 3580h 2 AE_ZONE_X AE Window/Zone X (def=1300h)
 0-7: ae_zone_x_start (00h..FFh) (div8) ;for WINDOW
 8-15: ae_zone_x_width (00h..FFh) (div8, minus 1) ;for each ZONE
 3582h 2 AE_ZONE_Y AE Window/Zone Y (def=0E00h)
 0-7: ae_zone_y_start (00h..FFh) (div8) ;for WINDOW
 8-15: ae_zone_y_width (00h..FFh) (div8, minus 1) ;for each ZONE
 3584h 2 AE_WINDOW_SIZE_LO LSBs ;\Size of each AE zone in pixels
 3586h 2 AE_WINDOW_SIZE_HI MSBs ;/(0..0001FFFFh, def=000x4B00h ?)
 3588h .. RESERVED_SOC2_3588-35AE Reserved
 35B0h UNDOC_SOC2_35B0 (mask=FFFFh, 0..FFFFh, def=05FAh) USED!
 35B2h .. RESERVED_SOC2_35B2-3602 Reserved
 3604h 20 R_GAMMA_CURVE_KNEES_0-18 Red Gamma Curve Knees 0..18 (1B00h,..)
 3618h 20 G_GAMMA_CURVE_KNEES_0-18 Green Gamma Curve Knees 0..18 (1B00h,..)
 362Ch 20 B_GAMMA_CURVE_KNEES_0-18 Blue Gamma Curve Knees 0..18 (1B00h,..)
 Above 20-byte knees consist of ten 16bit values (Knee0 in LSB)
 Due to the 16bit-big-endian format, the byte-order is:
 Knee1,Knee0,Knee3,Knee2,...,Knee17,Knee16,UNUSED,Knee18
 3640h .. RESERVED_SOC2_3640 Reserved
 3642h 2 POLY_ORIGIN_R Center Row (max 07FFh, def=025Ch)
 3644h 2 POLY_ORIGIN_C Center Column (max 07FFh, def=0324h)
 3646h .. RESERVED_SOC2_3646-364C Reserved
 364Eh 5x2 P_GR_P0Q0-4 P0Q for Green1 ;\P0 Coefficients
 3658h 5x2 P_RD_P0Q0-4 P0Q for Red ; (5 x float16 each)
 3662h 5x2 P_BL_P0Q0-4 P0Q for Blue ; (0010h,... each)
 366Ch 5x2 P_GB_P0Q0-4 P0Q for Green2 ;/
 3676h 5x2 P_GR_P1Q0-4 P1Q for Green1 ;\
 3680h 5x2 P_RD_P1Q0-4 P1Q for Red ; P1 Coefficients
 368Ah 5x2 P_BL_P1Q0-4 P1Q for Blue ; (5 x float16 each)
 3694h 5x2 P_GB_P1Q0-4 P1Q for Green2 ;/
 369Eh 5x2 P_GR_P2Q0-4 P2Q for Green1 ;\
 36A8h 5x2 P_RD_P2Q0-4 P2Q for Red ; P2 Coefficients
 36B2h 5x2 P_BL_P2Q0-4 P2Q for Blue ; (5 x float16 each)
 36BCh 5x2 P_GB_P2Q0-4 P2Q for Green2 ;/
 36C6h 5x2 P_GR_P3Q0-4 P3Q for Green1 ;\
 36D0h 5x2 P_RD_P3Q0-4 P3Q for Red ; P3 Coefficients
 36DAh 5x2 P_BL_P3Q0-4 P3Q for Blue ; (5 x float16 each)
 36E4h 5x2 P_GB_P3Q0-4 P3Q for Green2 ;/
 36EEh 5x2 P_GR_P4Q0-4 P4Q for Green1 ;\
 36F8h 5x2 P_RD_P4Q0-4 P4Q for Red ; P4 Coefficients
 3702h 5x2 P_BL_P4Q0-4 P4Q for Blue ; (5 x float16 each)
 370Ch 5x2 P_GB_P4Q0-4 P4Q for Green2 ;/
 3716h .. RESERVED_SOC2_3716-3278 Reserved |
| --- |

| DSi Aptina Camera Variables: RAM/SFR/MON (GPIO/Monitor) (MCU:0000h-20xxh) |
| --- |

**Internal RAM (MCU:0000h..0xxxh)**

Internal RAM is reserved for whatever internal purposes (probably including for
storing the 'logical variables' at MCU:2xxxh at some physical memory locations
at MCU:0xxxh). However, some of those undocumented reserved RAM cells are
manipulated by DSi games:

```

| 02F0h 2 UNDOC_RAM_02F0 (set to 0000h by DSi games)
 02F2h 2 UNDOC_RAM_02F2 (set to 0210h by DSi games)
 02F4h 2 UNDOC_RAM_02F4 (set to 001Ah by DSi games) |
| --- |

Exact RAM Size is unknown (around 2Kbyte or so)? Some Aptina chips do also
contain some sort of User RAM (at 0800h or so) for unknown purpose (just
general purpose storage maybe). Unknown if the DSi chips are having any such
User RAM.

**Special Function Registers SFR (MCU:1040h..10FEh)**

```

| 1040h .. RESERVED_SFR_1040-1050 Reserved
 1060h .. RESERVED_SFR_1060-1066 Reserved (REV3)
 1070h 2 GPIO_DATA GPIO Data (0..1E00h)
 0-8 Unused (0)
 9-12 gpio_3_0_data
 13-15 Unused (0)
 1072h 2 RESERVED_SFR_1072 Reserved
 1074h 2 GPIO_OUTPUT_SET GPIO Set (0..0C00h/1E00h?) (W)
 0-8 Unused (0)
 9-12 gpio_3_0_output_toggle (uh, toggle or set?)
 13-15 Unused (0)
 1076h 2 GPIO_OUTPUT_CLEAR GPIO Clear (0..0C00h/1E00h?) (W)
 0-8 Unused (0)
 9-12 gpio_3_0_output_clear
 13-15 Unused (0)
 1078h 2 GPIO_DIR GPIO Direction (0..1E00h, def=1E00h)
 0-8 Unused (0)
 9 gpio_0_dir (0=Output, 1=Input) ;(LSB0 of 10bit Output)
 10 gpio_1_dir (0=Output, 1=Input) ;(LSB1 of 10bit Output)
 11 gpio_2_dir (0=Output, 1=Input) ;(Flash/Shutter Pulse)
 12 gpio_3_dir (0=Output, 1=Input) ;(OE_BAR for Databus)
 13-15 Unused (0)
 107Ah .. RESERVED_SFR_107A-10FD Reserved |
| --- |

**Monitor Variables MON (MCU:2000h..2025h)**

```

| 2000h 5 RESERVED_MON_00-04 Reserved
 2005h 1 MON_CMD Monitor Command (0..FFh)
 2006h 2 MON_ARG1 Monitor First Argument (0..FFFFh)
 2008h .. RESERVED_MON_08-22 Reserved
 2024h 2 MON_PATCH_ID_0 Monitor First Patch (0..FFFFh) (REV1)
 0-7 mon_patch_0_version (00h-0Fh)
 The version number of the first patch (R)
 8-15 mon_patch_0_number (00h-0Fh)
 Identifies which patch the first patch is (R)
 2024h 1 MON_PATCH_ID_0 (mask=FFh) (R) ;\unlike above (REV3)
 2025h 1 MON_PATCH_ID_1 (0..FF) ;/REV1 specs (REV3)
 2026h 1 MON_PATCH_ID_2 (0..FF) (REV3)
 2027h 1 RESERVED_MON_27 Reserved (REV3) |
| --- |

| DSi Aptina Camera Variables: SEQ (Sequencer) (MCU:21xxh) |
| --- |

**Sequencer Variables SEQ (MCU:2100h..215Ah)**

```

| 2100h .. RESERVED_SEQ_00 Reserved
 2102h 1 SEQ_MODE SEQ Mode (enables "drivers") (def=0Fh)
 0 Enable AE (ID=2)
 1 Enable FD (ID=4)
 2 Enable AWB (ID=3)
 3 Enable HG (ID=11)
 4-7 Unspecified
 2103h 1 SEQ_CMD SEQ Cmd (0..FFh, def=01h)
 0-7 Cmd (0=Run, 1=Preview, 2=Capture, 3=Standby,
 4=Lock, 5=Refresh, 6=Refresh Mode)
 2104h 1 SEQ_STATE SEQ State (0..FFh)
 0-7 State (0=Run, 1=ToPreview, 2=Enter, 3=Preview
 4=Leave, 5=ToCapture, 6=Enter, 7=Capture,
 8=Leave, 9=Standby)
 2105h .. RESERVED_SEQ_05 Reserved
 2106h 1 SEQ_FLASHTYPE Type of flash to be used
 0-6 Flash Type (0=None, 1=LED, 2=Xenon, 3=XenonBurst)
 7 Set flash to LOCK mode (0=Normal, 1=LOCK mode)
 2107h .. RESERVED_SEQ_07-08 Reserved
 2109h 1 SEQ_AE_FASTBUFF AE Fast Buff (0..FFh, def=10h)
 210Ah 1 SEQ_AE_FASTSTEP AE Fast Step (0..FFh, def=02h)
 210Bh 1 SEQ_AWB_CONTBUFF AWB Cont Buff (0..FFh, def=08h)
 210Ch 1 SEQ_AWB_CONTSTEP AWB Cont Step (0..FFh, def=02h)
 210Dh .. RESERVED_SEQ_0D-10 Reserved
 2111h 1 SEQ_OPTIONS SEQ Options (0..FFh, def=08h)
 0 Reserved
 1 Reserved
 2 Reserved
 3 seq_crop_win_ae, Use crop window for AE statistics
 4 seq_crop_win_awb, Use crop window for AWB statistics
 7 Reserved
 2112h .. RESERVED_SEQ_12 Reserved
 2113h 2 SEQ_FLASH_TH SEQ Flash TH (0..FFFFh)
 2115h 1 SEQ_CAP_MODE Capture mode (in Capture state only)
 0 Xenon Flash (Still Only)
 1 Video
 2 Turn Flash off before last frame in capture state
 4 Video AE on
 5 Video AWB on
 6 Video HG on
 2116h 1 SEQ_CAP_NUMFRAMES Num still frames captured (0..FFh,def=3)
 2117h 1 SEQ_PREVIEW_0_AE Preview 0 AE (PREVIEW ENTER) ;\
 0-3 AE (0=Off, 1=Fast, 2=Manual, 3=Continuous, 4=MDR) ;
 4-7 Unspecified (0..5) (0..0Fh for PREVIEW_2/3) ; Pre-
 2118h 1 SEQ_PREVIEW_0_FD Preview 0 FD (PREVIEW ENTER) ; view
 0-7 FD (0=Off, 1=Continuous, 2=Manual) ; 0
 2119h 1 SEQ_PREVIEW_0_AWB Preview 0 AWB (PREVIEW ENTER) ;
 0-7 AWB (0=Off, 1=On) ; PRE-
 211Ah 1 SEQ_PREVIEW_0_HG Preview 0 HG (PREVIEW ENTER) ; VIEW
 0-7 HG (0=Off, 1=Fast, 2=Manual, 3=Continuous) ; ENTER
 211Bh 1 SEQ_PREVIEW_0_FLASH Flash Config (0..FFh) ;
 0-6 Flash (0=Off,1=On,2=Locked,3=AutoEvaluate,7=UserDef) ;
 7 Reserved ;
 211Ch 1 SEQ_PREVIEW_0_SKIPFRAME Skipframe State Config (def=40h) ;
 0-3 Unspecified ;
 4 Unspecified (except PREVIEW_2: Reserved) ;
 5 Skip_led_on ;
 6 Skip_state (0=No skip state, 1=Skip state) ;
 7 Turn_off_fen ;/
 211Dh 1 SEQ_PREVIEW_1_AE ;\ def=01h
 211Eh 1 SEQ_PREVIEW_1_FD ; Preview 1 (PREVIEW) def=01h
 211Fh 1 SEQ_PREVIEW_1_AWB ; (same as Preview 0, but def=01h
 2120h 1 SEQ_PREVIEW_1_HG ; without AE=MDR, def=01h
 2121h 1 SEQ_PREVIEW_1_FLASH ; without HG=Manual/Continous)
 2122h 1 SEQ_PREVIEW_1_SKIPFRAME ;/ def=N/A
 2123h 1 SEQ_PREVIEW_2_AE ;\
 2124h 1 SEQ_PREVIEW_2_FD ; Preview 2 (PREVIEW LEAVE)
 2125h 1 SEQ_PREVIEW_2_AWB ; (same as Preview 0, but
 2126h 1 SEQ_PREVIEW_2_HG ; without HG=Manual/Continous)
 2127h 1 SEQ_PREVIEW_2_FLASH ;
 2128h 1 SEQ_PREVIEW_2_SKIPFRAME ;/
 2129h 1 SEQ_PREVIEW_3_AE ;\
 212Ah 1 SEQ_PREVIEW_3_FD ; Preview 3 (CAPTURE ENTER)
 212Bh 1 SEQ_PREVIEW_3_AWB ; (same as Preview 0)
 212Ch 1 SEQ_PREVIEW_3_HG ;
 212Dh 1 SEQ_PREVIEW_3_FLASH ;
 212Eh 1 SEQ_PREVIEW_3_SKIPFRAME ;/
 212Fh .. RESERVED_SEQ_2F-33 Reserved
 2134h 1 UNDOC_SEQ_34 (0..FFh)
 2135h .. RESERVED_SEQ_35-44 Reserved
 2145h 2 UNDOC_SEQ_45 (0..FFFFh)
 2147h .. RESERVED_SEQ_47-59 Reserved |
| --- |

| DSi Aptina Camera Variables: AE (Auto Exposure) (MCU:22xxh) |
| --- |

**Auto Exposure Variables AE (MCU:2200h-2261h)**

```

| 2200h .. RESERVED_AE_00 Reserved
 2202h 1 AE_WINDOW_POS AE Window Position Y0 and X0
 0-3 X0 (in units of 1/16th of frame width) (0..0Fh)
 4-7 Y0 (in units of 1/16th of frame height) (0..0Fh)
 2203h 1 AE_WINDOW_SIZE AE Window Height and Width (def=FFh)
 0-3 Width (units of 1/16th of frame width, minus 1) (0..0Fh)
 4-7 Height (units of 1/16th of frame height, minus 1) (0..0Fh)
 2204h .. RESERVED_AE_04 Reserved
 2206h 1 AE_TARGET AE Target Brightness (0..FFh, def=32h)
 2207h 1 AE_GATE AE Sensitivity (0..FFh, def=04h)
 2208h .. UNDOC_AE_08 (0..FFh, def=02h)
 2209h .. RESERVED_AE_09-0A Reserved
 220Bh 1 AE_MIN_INDEX Min (0-FFh)
 220Ch 1 AE_MAX_INDEX Max allowed zone number (0-FFh,def=18h)
 220Dh 1 AE_MIN_VIRTGAIN Min allowed virtual gain (0-FFh,def=10h)
 220Eh 1 AE_MAX_VIRTGAIN Max allowed virtual gain (0-FFh,def=80h)
 220Fh .. RESERVED_AE_0F-11 Reserved
 2212h 2 AE_MAX_DGAIN_AE1 Max digital gain pre-LC (def=8000h)
 2214h .. RESERVED_AE_14-16 Reserved
 2217h 1 AE_STATUS AE Status
 0 AE_at_limit (1=AE reached limit)
 1 R9_changed (1=Need to skip frame)
 2 Ready (0=AE not ready, 1=AE ready)
 3-7 Unused (0)
 2218h 1 AE_CURRENT_Y Last measured luma (0-FFh,def=4Bh) (R)
 2219h 2 AE_R12 Curr shutter delay (def=0279h) (R)
 221Bh 1 AE_INDEX Curr zone integration time (def=04h) (R)
 221Ch 1 AE_VIRTGAIN Curr virtual gain (0-FFh,def=10h) (R)
 221Dh .. RESERVED_AE_1D-1E Reserved
 221Fh 2 AE_DGAIN_AE1 Current digital gain pre-LC (def=0080h)
 2221h .. RESERVED_AE_21 Reserved
 2222h 2 AE_R9 Current R9:0 value (0-FFFFh, def=0010h)
 2224h .. RESERVED_AE_24-2C Reserved
 222Dh 2 AE_R9_STEP Integration time per zone (def=009Dh)
 222Fh .. RESERVED_AE_2F-49 Reserved
 224Ah 1 AE_TARGETMIN Min value for target (0..FFh, def=32h)
 224Bh 1 AE_TARGETMAX Max value for target (0..FFh, def=96h)
 224Ch 1 AE_TARGETBUFFERSPEED Target Buffer Speed (0..FFh, def=0Ch)
 224Dh .. RESERVED_AE_4D Reserved
 224Fh 1 AE_BASETARGET Target Base (0..FFh, def=36h)
 2250h .. RESERVED_AE_50-61 Reserved
 2262h .. RESERVED_AE_62-64 Reserved (REV3) |
| --- |

| DSi Aptina Camera Variables: AWB (Auto White Balance) (MCU:23xxh) |
| --- |

**Auto White Balance Variables AWB (MCU:2300h..236Eh)**

```

| 2300h .. RESERVED_AWB_00 Reserved
 2302h 1 AWB_WINDOW_POS AWB Window Position Y0 and X0
 0-3 X0 (in units of 1/16th of frame width) (0..0Fh)
 4-7 Y0 (in units of 1/16th of frame height) (0..0Fh)
 2303h 1 AWB_WINDOW_SIZE AWB Window Size (def=EFh)
 0-3 Width (units of 1/16th of frame width, minus 1) (0..0Fh)
 4-7 Height (units of 1/16th of frame height, minus 1) (0..0Fh)
 2304h .. RESERVED_AWB_04 Reserved
 2306h 3x2 AWB_CCM_L_0-2 Left CCM K11,K12,K13 (0180h,FF00h,0080h)
 230Ch 3x2 AWB_CCM_L_3-5 Left CCM K21,K22,K23 (FF66h,0180h,FFEEh)
 2312h 3x2 AWB_CCM_L_6-8 Left CCM K31,K32,K33 (FFCDh,FECDh,019Ah)
 2318h 2 AWB_CCM_L_9 Left CCM Red/Green gain (0020h)
 231Ah 2 AWB_CCM_L_10 Left CCM Blue/Green gain (0033h)
 231Ch 3x2 AWB_CCM_RL_0-2 DeltaCCM D11,D12,D13 (0100h,FF9Ah,xxxxh)
 2322h 3x2 AWB_CCM_RL_3-5 DeltaCCM D21,D22,D23 (004Dh,FFCDh,FFB8h)
 2328h 3x2 AWB_CCM_RL_6-8 DeltaCCM D31,D32,D33 (004Dh,0080h,FF66h)
 232Eh 2 AWB_CCM_RL_9 DeltaCCM Red/Green gain (0008h)
 2330h 2 AWB_CCM_RL_10 DeltaCCM Blue/Green gain (FFF7h)
 2332h 3x2 AWB_CCM_0-2 Curr CCM C11,C12,C13 (01BAh,FF5Bh,FFF1h)
 2338h 3x2 AWB_CCM_3-5 Curr CCM C21,C22,C23 (FFC7h,01B9h,FF87h)
 233Eh 3x2 AWB_CCM_6-8 Curr CCM C31,C32,C33 (FFF9h,FF32h,01DCh)
 2344h 2 AWB_CCM_9 Curr CCM Red/Green gain (003Ch)
 2346h 2 AWB_CCM_10 Curr CCM Blue/Green gain (002Bh)
 2348h 1 AWB_GAIN_BUFFER_SPEED Gain Speed (1-20h, def=08h, 20h=fastest)
 2349h 1 AWB_JUMP_DIVISOR Jump Divisor (1-FFh, def=02h, 1=fastest)
 234Ah 1 AWB_GAIN_MIN Min AWB Red (def=59h) ;\Digital Gain
 234Bh 1 AWB_GAIN_MAX Max allowed Red (def=B6h) ; Min/max
 234Ch 1 AWB_GAINMIN_B Min AWB (def=59h) ; (0..FFh)
 234Dh 1 AWB_GAINMAX_B Max allowed (def=A6h) ;/
 234Eh 1 AWB_GAIN_R Current R digital gain ;\Current Gain
 234Fh 1 AWB_GAIN_G Current G digital gain ; (0..FFh,
 2350h 1 AWB_GAIN_B Current B digital gain ;/def=80h)
 2351h 1 AWB_CCM_POSITION_MIN Min/Left (def=?) ;\(range 0..FFh,
 2352h 1 AWB_CCM_POSITION_MAX Max/Right (def=7Fh) ; 00h=incandescent,
 2353h 1 AWB_CCM_POSITION Position (def=40h) ;/7Fh=daylight)
 2354h 1 AWB_SATURATION Saturation (0..FFh, def=80h, 80h=100%)
 2355h 1 AWB_MODE Misc control for AWB (0..FFh)
 0 Steady (1=AWB is done)
 1 Limits Reached (1=AWB limit is reached)
 2 Reserved
 3 Reserved
 4 Reserved
 5 Force_unit_dgains
 6 NormCCM_off
 2356h 2 AWB_GAINR_BUF Time-buffered R gain (0..FFFFh)
 2358h 2 AWB_GAINB_BUF Time-buffered B gain (0..FFFFh)
 235Ah .. RESERVED_AWB_5A-5C Reserved
 235Dh 1 AWB_STEADY_BGAIN_OUT_MIN (0-FF, def=78h)
 235Eh 1 AWB_STEADY_BGAIN_OUT_MAX (0-FF, def=86h)
 235Fh 1 AWB_STEADY_BGAIN_IN_MIN (0-FF, def=7Eh)
 2360h 1 AWB_STEADY_BGAIN_IN_MAX (0-FF, def=82h)
 2361h 2 UNDOC_AWB_61 (0..FFFFh, def=0040h)
 2363h 1 AWB_TG_MIN0 True Gray minimum (0..FFh, def=D2h)
 2364h 1 AWB_TG_MAX0 True Gray maximum (0..FFh, def=F6h)
 2365h 1 AWB_X0 (0-FFh, def=10h)
 2366h 1 AWB_KR_L (0-FFh, def=80h)
 2367h 1 AWB_KG_L (0-FFh, def=80h)
 2368h 1 AWB_KB_L (0-FFh, def=80h)
 2369h 1 AWB_KR_R (0-FFh, def=80h)
 236Ah 1 AWB_KG_R (0-FFh, def=80h)
 236Bh 1 AWB_KB_R (0-FFh, def=80h)
 236Ch .. RESERVED_AWB_6C-6E Reserved |
| --- |

| DSi Aptina Camera Variables: FD (Anti-Flicker) (MCU:24xxh) |
| --- |

**Anti-Flicker Variables FD (MCU:2400h..247Bh)**

```

| 2400h .. RESERVED_FD_00 Reserved
 2402h 1 FD_WINDOW_POSH Window Pos H (0..FFh, def=1Dh)
 0-3 Width (in units of 1/16th of frame width, minus 1) (0..0Fh)
 4-7 X0 (=position/origin or so?) (0..0Fh)
 2403h 1 FD_WINDOW_HEIGHT FlickerMeasurementWindowHeight (def=04h)
 0-5 Flicker measurement window height in rows (0..3Fh)
 6-7 Unspecified
 2404h 1 FD_MODE Flicked Detection switches/indicators
 0-3 Reserved (0..0Fh) (R)
 4 Debug_mode (0=Disable, 1=Enable single period mode)
 5 Curr Flicker State (0=60Hz, 1=50Hz) (R)
 6 Curr Settings (0=60Hz, 1=50Hz)
 7 Manual Mode (0=Disable, 1=Enable)
 2405h .. RESERVED_FD_05-07 Reserved
 2408h 1 FD_SEARCH_F1_50 Search F1 50Hz (0..FFh, def=33h)
 2409h 1 FD_SEARCH_F2_50 Search F2 50Hz (0..FFh, def=35h)
 240Ah 1 FD_SEARCH_F1_60 Search F1 60Hz (0..FFh, def=29h)
 240Bh 1 FD_SEARCH_F2_60 Search F2 60Hz (0..FFh, def=2Bh)
 240Ch 1 UNDOC_FD_0C (0..FFh)
 240Dh 1 FD_STAT_MIN Stat Min (0..FFh, def=03h)
 240Eh 1 FD_STAT_MAX Stat Max (0..FFh, def=05h)
 240Fh .. RESERVED_FD_0F Reserved
 2410h 1 FD_MIN_AMPLITUDE Ignore Signals below Min (0..FFh, def=5)
 2411h 2 FD_R9_STEP_F60_A 60HzA (def=0D4h) ;\Minimal Shutter Width
 2413h 2 FD_R9_STEP_F50_A 50HzA (def=103h) ; Steps for 60Hz/50H AC
 2415h 2 FD_R9_STEP_F60_B 60HzB (def=09Dh) ; in Context A/B
 2417h 2 FD_R9_STEP_F50_B 50HzB (def=0B8h) ;/(0..FFFFh)
 2419h .. RESERVED_FD_19-7B Reserved |
| --- |

| DSi Aptina Camera Variables: MODE (Mode/Context) (MCU:27xxh) |
| --- |

**Mode Variables MODE (MCU:2700h..2768h)**

```

| 2700h .. RESERVED_MODE_00-02 Reserved
 2703h 2 MODE_OUTPUT_WIDTH_A (CX) (0..FFFFh, def=0320h) ;\Size A
 2705h 2 MODE_OUTPUT_HEIGHT_A (CY) (0..FFFFh, def=0258h) ;/
 2707h 2 MODE_OUTPUT_WIDTH_B (0..FFFFh, def=0640h) ;\Size B
 2709h 2 MODE_OUTPUT_HEIGHT_B (0..FFFFh, def=04B0h) ;/
 270Bh 1 MODE_A_MIPI_VC (0..07h) (REV3) ;-Mipi A
 270Ch 1 MODE_B_MIPI_VC (0..07h) (REV3) ;-Mipi B
 270Dh 2 MODE_SENSOR_ROW_START_A (Y1) (0..FFFFh) ;\
 270Fh 2 MODE_SENSOR_COL_START_A (X1) (0..FFFFh) ;
 2711h 2 MODE_SENSOR_ROW_END_A (Y2) (0..FFFFh, def=040Dh) ;
 2713h 2 MODE_SENSOR_COL_END_A (X2) (0..FFFFh, def=050Dh) ; Sensor
 2715h 2 MODE_SENSOR_ROW_SPEED_A (0..0777h, def=0112h) ; A
 0-2: pixclk_speed (0..7) ;
 1ADC: Pclk = 2 mclks * bits[0:2] ;
 2ADC: bits[0:2] ;
 4-6: Reserved (0..7) ;
 8-10: Reserved (0..7) ;
 2717h 2 MODE_SENSOR_READ_MODE_A (0..FFFFh, def=046Ch) ;
 0: horiz_mirror ;
 1: vert_flip ;
 2-4: y_odd_inc (0..7) ;
 5-7: x_odd_inc (0..7) ;
 9: low_power ;
 10: xy_bin_en ;
 11: x_bin_en ;
 2719h 2 MODE_SENSOR_FINE_CORRECTION_A (0..FFFFh, def=007Bh) ;
 271Bh 2 MODE_SENSOR_FINE_IT_MIN_A (0..FFFFh, def=0408h) ;
 271Dh 2 MODE_SENSOR_FINE_IT_MAX_MARGIN_A (0..FFFFh, def=00ABh) ;
 271Fh 2 MODE_SENSOR_FRAME_LENGTH_A (0..FFFFh, def=0293h) ;
 2721h 2 MODE_SENSOR_LINE_LENGTH_PCK_A (0..FFFFh, def=07D0h) ;/
 2723h 2 MODE_SENSOR_ROW_START_B (0..FFFFh, def=0004h) ;\
 2725h 2 MODE_SENSOR_COL_START_B (0..FFFFh, def=0004h) ; Sensor
 2727h 2 MODE_SENSOR_ROW_END_B (0..FFFFh, def=040Bh) ; B
 2729h 2 MODE_SENSOR_COL_END_B (0..FFFFh, def=050Bh) ;
 272Bh 2 MODE_SENSOR_ROW_SPEED_B (0..0777h, def=0111h) ; (same
 272Dh 2 MODE_SENSOR_READ_MODE_B (0..FFFFh, def=0024h) ; as
 272Fh 2 MODE_SENSOR_FINE_CORRECTION_B (0..FFFFh, def=00A4h) ; Sensor
 2731h 2 MODE_SENSOR_FINE_IT_MIN_B (0..FFFFh, def=0408h) ; A, see
 2733h 2 MODE_SENSOR_FINE_IT_MAX_MARGIN_B (0..FFFFh, def=00A4h) ; there)
 2735h 2 MODE_SENSOR_FRAME_LENGTH_B (0..FFFFh, def=04EDh) ;
 2737h 2 MODE_SENSOR_LINE_LENGTH_PCK_B (0..FFFFh, def=0D06h) ;/
 2739h 2 MODE_CROP_X0_A (0..FFFFh) ;\
 273Bh 2 MODE_CROP_X1_A (0..FFFFh, def=031Fh) ; Crop A
 273Dh 2 MODE_CROP_Y0_A (0..FFFFh) ;
 273Fh 2 MODE_CROP_Y1_A (0..FFFFh, def=0257h) ;/
 2741h .. RESERVED_MODE_41-45 Reserved
 2747h 2 MODE_CROP_X0_B (0..FFFFh) ;\
 2749h 2 MODE_CROP_X1_B (0..FFFFh, def=063Fh) ; Crop B
 274Bh 2 MODE_CROP_Y0_B (0..FFFFh) ;
 274Dh 2 MODE_CROP_Y1_B (0..FFFFh, def=04AFh) ;/
 274Fh .. RESERVED_MODE_4F-53 Reserved
 2755h 2 MODE_OUTPUT_FORMAT_A Format A (0..FFFFh ;\
 2757h 2 MODE_OUTPUT_FORMAT_B Format B (0..FFFFh ;
 0 swap_channels (swap Cb/Cr in YUV and R/B in RGB);
 1 swap_chrominance_luma ; Format
 2 bayer_out (Progressive Bayer) ; A/B
 3 monochrome (0..1) ;
 4 Reserved ;
 5 output_mode (0=YUV, 1=RGB) ;
 6-7 RGB Format (0=565, 1=555, 2=444xh, 3:x444h) ;
 8 Processed Bayer (0..1) ;
 9 Invert out_clk (0..1) (REV3) ;
 10-15 Unspecified ;/
 2759h 2 MODE_SPEC_EFFECTS_A Effects A (def=6440h) ;\
 275Bh 2 MODE_SPEC_EFFECTS_B Effects B (def=6440h) ;
 0-2 Selection (1=Mono, 2=Sepia, 3=Negative, ; Effects
 4=Solarization, 5=Solarization w/ UV) ; A/B
 3-5 Dither_bitwidth ;
 6 Dither_luma ;
 8-15 Solarization Threshold (0..7 for diff effects) ;/
 275Dh 1 MODE_Y_RGB_OFFSET_A Offset A (00h..FFh) ;\Offset
 275Eh 1 MODE_Y_RGB_OFFSET_B Offset B (00h..FFh) ;/A/B
 275Fh 2 MODE_COMMON_MODE_SETTINGS_BRIGHT_COLOR_KILL ;\
 Shadow register for 35A4h in SOC2 ;
 0-2 Color kill saturation point (0..7) ; Kill
 3-5 Bright color kill gain (0..7) ; Bright
 6-8 Bright color kill threshold (0..7) ;
 9 Signal_ctrl (1=use luma as min/max value) ;
 10 en_kl (1=enable bright color kill) ;
 11-15 Unspecified ;/
 2761h 2 MODE_COMMON_MODE_SETTINGS_DARK_COLOR_KILL ;\
 Shadow register for 35A2h in SOC2 ;
 0-2 Dark color kill gain (0..7) ; Kill
 3-5 Dark color kill threshold (0..7) ; Dark
 6 Signal_ctrl (1=use luma as min/max value) ;
 7 en_dark_kl (1=enable dark color kill) ;
 8-15 Unspecified ;/
 2763h 2 MODE_COMMON_MODE_SETTINGS_FX_SEPIA_SETTINGS ;\
 0-7 Sepia constants for Cr (00h..FFh) ; Sepia
 8-15 Sepia constants for Cb (00h..FFh) ;/
 2765h 1 MODE_COMMON_MODE_SETTINGS_FILTER_MODE ;\
 Shadow register for 326Eh in SOC1 ;
 0-2 UV Filter mode (0..7) ; Filter
 3-4 Y Filter mode (0..3) ;
 5 Enable_y_filter (enable y permanently) ;
 6 Threshold_switch, switch for adaptive Y filter threshold
 7 Off_switch, B/W filter enable switch ;/
 2766h 1 MODE_COMMON_MODE_SETTINGS_TEST_MODE Test (00h..FFh)
 0-? Test Pattern (0=None?, 1=Flat, 2=Ramp, 3=ColorBars,
 4=VertStripes, 5=Noise, 6=HoriStripes)
 Output test pattern (instead camera image)
 requires "Refresh Command" sent to Sequencer
 2767h .. RESERVED_MODE_67-68 Reserved |
| --- |

| DSi Aptina Camera Variables: HG (Histogram) (MCU:2Bxxh) |
| --- |

**Histogram Variables HG (MCU:2B00h..2B61h)**

```

| 2B00h .. RESERVED_HG_00-03 Reserved
 2B04h 1 HG_MAX_DLEVEL DarkLevel Limit (0..FFh, def=40h)
 2B05h .. RESERVED_HG_05 Reserved
 2B06h 1 HG_PERCENT Percent? (0..FFh, def=03h)
 2B07h .. RESERVED_HG_07 Reserved
 2B08h 1 HG_DLEVEL DarkLevel (0..FFh, def=10h)
 2B09h .. RESERVED_HG_09-16 Reserved
 2B17h 1 HG_AVERAGELUMA Average Luma (0..FFh)
 2B18h .. RESERVED_HG_18-1A Reserved
 2B1Bh 2 HG_BRIGHTNESSMETRIC Brightness Metric (0..FFFFh)
 2B1Dh .. RESERVED_HG_1D Reserved
 2B1Fh 1 HG_LLMODE Low Light mode controls (def=C4h)
 0-3 Brightness Metric Prescaler (01h..0Fh)
 4-5 Unused (0)
 6 HG_2d_corr_vs_clusterdc
 7 Clusterdc_vs_gains
 2B20h 1 HG_LL_SAT1 LL_SAT1 (0..FFh, def=43h)
 2B21h 1 UNDOC_HG_21 Whatever (0..FFh, def=10h)
 2B22h 1 HG_LL_APCORR1 LL_APCORR1 (0..FFh, def=03h)
 2B23h 1 UNDOC_HG_23 Whatever (0..FFh, def=04h)
 2B24h 1 HG_LL_SAT2 LL_SAT2 (0..FFh, def=0Ch)
 2B25h 1 HG_LL_INTERPTHRESH2 LL_INTERPTHRESH2 (0..FFh, def=23h)
 2B26h 1 HG_LL_APCORR2 LL_APCORR2 (0..FFh)
 2B27h 1 HG_LL_APTHRESH2 LL_APTHRESH2 (0..FFh, def=04h)
 2B28h 2 HG_LL_BRIGHTNESSSTART LL_BRIGHTNESSSTART (0..FFFFh, def=0A8Ch)
 2B2Ah 2 HG_LL_BRIGHTNESSSTOP LL_BRIGHTNESSSTOP (0..FFFFh, def=34BCh)
 2B2Ch 1 HG_NR_START_R NR_START_R (0..FFh, def=06h)
 2B2Dh 1 HG_NR_START_G NR_START_G (0..FFh, def=0Eh)
 2B2Eh 1 HG_NR_START_B NR_START_B (0..FFh, def=06h)
 2B2Fh 1 HG_NR_START_OL NR_START_OL (0..FFh, def=06h)
 2B30h 1 HG_NR_STOP_R NR_STOP_R (0..FFh, def=1Eh)
 2B31h 1 HG_NR_STOP_G NR_STOP_G (0..FFh, def=1Eh)
 2B32h 1 HG_NR_STOP_B NR_STOP_B (0..FFh, def=1Eh)
 2B33h 1 HG_NR_STOP_OL NR_STOP_OL (0..FFh, def=1Eh)
 2B34h 1 HG_NR_GAINSTART NR_GAINSTART (0..FFh, def=08h)
 2B35h 1 HG_NR_GAINSTOP NR_GAINSTOP (0..FFh, def=80h)
 2B36h 1 HG_CLUSTERDC_TH CLUSTERDC_TH (0..FFh, def=1Eh)
 2B37h 1 HG_GAMMA_MORPH_CTRL Gamma Morphing Control (0..FFh, def=3)
 0-1 Enable Gamma Morph (0=Disable, 1=Use Table A, 2=Use Table B,
 3=AutoMorph between Table A and B based on BrightnessMetric)
 2-7 Unspecified
 2B38h 2 HG_GAMMASTARTMORPH Gamma Start Morph (0..FFFFh, def=0A8Ch)
 2B3Ah 2 HG_GAMMASTOPMORPH Gamma Stop Morph (0..FFFFh, def=34BCh)
 2B3Ch 19 HG_GAMMA_TABLE_A_0-18 Gamma Table A for normal light condition
 Default=xx,1B,2E,4C,78,98,B0,E8,CF,D9,E1,E8,EE,F2,F6,F9,FB,FD,FF
 2B4Fh 19 HG_GAMMA_TABLE_B_0-18 Gamma Table B for low light condition
 Default=xx,0F,1A,2E,50,6A,80,91,A1,AF,BB,C6,D0,D9,E2,EA,F1,F9,FF
 Above 2 tables have normal byte-order (Entry0,Entry1,...,Entry18)
 2B62h 2 HG_FTB_START_BM (0..FFFFh, def=7FBCh) (REV3)
 2B64h 2 HG_FTB_STOP_BM (0..FFFFh, def=82DCh) (REV3)
 2B66h 2 HG_CLUSTER_DC_BM (0..FFFFh, def=4A38h) (REV3) |
| --- |

| DSi Alternate Cameras from Unknown Manufacturer |
| --- |

Device A0h/E0h appear to be cameras from an alternate manufacturer. DSi games
are supporting these devices, but as by now, there aren't any DSi consoles
known to be actually fitted with these cameras.

The camera type & manufacturer are still unknown. Below initialization data
is containing some characterstic info that should allow to identify them. For
example, register 03h appears to be bank-switching the other registers.

**unknown_cam_get_chip_id:**

Reading an 8bit value from index 00h (in any bank?) seems to return some Chip
ID, at least the DSi is reading that register before initialization (despite of
reading it, the DSi does appear to ignore that value though).

Note: On a DSi with Aptina cameras, trying to read anything from I2C devices
A0h/E0h does just return FFh-bytes.

**Formatting Note**

Below tables consist of "Index,Length,Data[Length]" entries.

**unknown_cam_type_code_list_init:**

```

| 003h, 1,001h ;<-- bank maybe?
 009h, 3,0E2h,002h,002h
 004h, 1,010h
 004h, 1,0A0h
 004h, 2,090h,04Ch
 00Dh, 1,0FFh
 016h, 1,053h
 018h, 3,002h,001h,00Fh
 020h, 1,000h
 023h, 2,000h,000h
 034h, 8,000h,003h,000h,003h,001h,002h,000h,0C2h
 03Dh, 4,050h,050h,000h,067h
 042h, 1,01Ch
 04Ah, 2,043h,0F8h
 04Eh, 7,028h,0FCh,000h,024h,014h,008h,008h
 056h,13,000h,018h,028h,034h,044h,056h,06Eh,080h,0A4h,0C2h,0D6h,0E8h,0F4h
 065h,12,00Fh,038h,008h,000h,01Fh,01Fh,01Fh,01Fh,01Fh,01Fh,01Fh,01Fh
 07Ah,17,039h,03Bh,03Ah,036h,03Ch,03Ch,03Ah,03Ch,03Ch,03Ch,03Ah,03Ch,038h
 03Ah,031h,03Ah,082h
 08Dh,22,08Ah,090h,096h,09Ch,0A4h,0AAh,0B0h,0B6h,0BCh,0C4h,0CAh,0D0h,0D6h
 0DCh,0E4h,0EAh,0F0h,0F2h,0F4h,0F6h,0F8h,0FAh
 0A9h, 1,02Bh
 0ABh, 3,02Eh,000h,050h
 0AFh, 1,070h
 0B2h, 4,03Ch,068h,049h,070h
 0B7h,21,032h,000h,00Eh,0F8h,00Ch,07Ah,040h,000h,000h,010h,044h,064h,052h
 012h,001h,0D7h,004h,002h,024h,002h,024h
 0D4h, 5,004h,004h,008h,00Ah,010h
 016h, 1,0F7h
 0DEh, 2,002h,024h
 016h, 1,053h
 0E1h, 1,034h
 0FFh, 1,00Fh
 003h, 1,002h ;<-- bank maybe?
 005h, 2,06Dh,004h
 011h, 4,004h,048h,004h,048h
 016h, 2,00Ch,0D8h
 019h, 2,00Ch,0D8h
 01Eh, 6,002h,024h,070h,000h,001h,06Eh
 026h, 7,008h,00Fh,00Fh,006h,0FFh,0FFh,003h
 02Eh,19,07Eh,088h,074h,07Eh,008h,010h,080h,008h,084h,078h,001h,003h,00Ah
 025h,060h,0B0h,006h,000h,000h
 042h, 7,080h,010h,010h,010h,040h,080h,0FFh
 04Ah,30,000h,000h,001h,0E5h,001h,0E0h,000h,070h,002h,0F0h,000h,02Eh,001h
 0F3h,000h,005h,000h,000h,001h,000h,000h,0C0h,000h,026h,000h,01Ch
 000h,0B3h,000h,086h
 069h,36,000h,000h,006h,014h,014h,01Fh,000h,000h,000h,000h,000h,01Fh,000h
 000h,010h,010h,010h,01Fh,000h,000h,004h,004h,004h,01Fh,000h,000h
 000h,000h,000h,01Fh,000h,000h,010h,010h,010h,01Fh
 095h, 1,084h
 097h,18,002h,000h,0FFh,0FFh,000h,0FFh,0FFh,000h,000h,0FFh,0FFh,000h,0FFh
 0FFh,000h,0F8h,014h,010h
 0AAh,13,044h,098h,08Ch,09Ch,048h,08Ch,08Ah,09Ch,046h,02Ah,080h,008h,026h
 0B8h, 8,02Ah,084h,000h,026h,02Ah,080h,008h,020h
 0C1h,10,038h,020h,01Fh,01Dh,034h,020h,01Fh,01Dh,045h,05Dh
 0CCh, 2,020h,020h
 0D0h, 3,080h,000h,0FFh
 003h, 1,000h ;<-- bank maybe?
 013h, 2,000h,04Ch
 01Dh, 2,000h,04Ch
 015h, 2,001h,05Fh
 055h, 2,001h,05Eh
 031h, 6,006h,068h,00Ch,005h,004h,047h
 047h, 2,000h,003h
 04Ah, 3,0A0h,000h,003h
 04Fh, 2,000h,003h
 059h, 2,000h,001h
 05Fh, 2,000h,001h
 066h, 1,09Eh
 06Eh, 2,07Fh,003h
 075h, 1,050h
 07Ah, 2,000h,001h
 07Eh, 1,020h
 082h, 1,038h
 084h,14,003h,040h,003h,040h,000h,000h,040h,003h,0FFh,002h,008h,020h,018h,006h
 093h,11,020h,040h,040h,01Fh,002h,000h,000h,000h,000h,000h,000h
 003h, 1,001h ;<-- bank maybe?
 00Fh, 1,0C9h ;or, for Device E0h: 00Fh, 1,0C8h
 052h, 3,004h,008h,008h ;or, for Device E0h: N/A
 003h, 1,002h ;<-- bank maybe?
 026h, 1,008h ;or, for Device E0h: 026h, 1,000h
 0CCh, 2,0C0h,0C0h ;or, for Device E0h: N/A
 0B4h, 1,000h ;or, for Device E0h: N/A
 0B6h, 1,026h ;or, for Device E0h: N/A
 0B9h, 3,000h,008h,026h ;or, for Device E0h: N/A
 0BDh, 1,000h ;or, for Device E0h: N/A
 026h, 1,008h ;or, for Device E0h: N/A
 003h, 1,001h ;<-- bank maybe?
 02Dh, 1,0FFh
 004h, 1,020h |
| --- |

**unknown_cam_type_code_list_activate:**

```

| 003h, 1,002h ;<-- bank maybe?
 0A7h, 1,014h
 003h, 1,001h ;<-- bank maybe?
 004h, 1,0A0h
 004h, 1,090h
 02Dh, 1,000h
 004h, 1,098h |
| --- |

**Random Note**

This info is probably not really helpful, but the DSi firmware contains code
for setting Register C1h..C9h (within unknown bank) to one of the following
twelve settings.

```

| C1h, 8,038h,030h,01Fh,01Fh,02Ch,030h,01Fh,01Fh
 C1h, 8,038h,030h,01Fh,01Fh,038h,030h,01Fh,01Fh
 C1h, 8,02Ch,030h,01Fh,01Fh,02Ch,030h,01Fh,01Fh
 C1h, 8,02Ch,030h,01Fh,01Fh,02Ch,030h,01Fh,01Fh
 C1h, 8,02Ch,030h,01Fh,01Fh,02Ch,030h,01Fh,01Fh
 C1h, 8,02Ch,030h,01Fh,01Fh,02Ch,030h,01Fh,01Fh
 C1h, 8,030h,028h,018h,018h,034h,028h,008h,018h
 C1h, 8,030h,028h,018h,018h,030h,028h,008h,018h
 C1h, 8,028h,028h,018h,018h,028h,028h,008h,018h
 C1h, 8,028h,028h,018h,018h,028h,028h,008h,018h
 C1h, 8,028h,028h,018h,018h,028h,028h,008h,018h
 C1h, 8,028h,028h,018h,018h,028h,028h,008h,018h |
| --- |