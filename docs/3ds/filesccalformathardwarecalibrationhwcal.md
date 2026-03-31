# Filesccalformathardwarecalibrationhwcal

> Source: https://problemkaputt.de/gbatek.htm
> Section: Filesccalformathardwarecalibrationhwcal

3DS Files - CCAL Format (Hardware calibration, HWCAL) 
| |
| --- |

 **3ds:\ro\sys\HWCAL0.dat (9D0h bytes)**
 **3ds:\ro\sys\HWCAL1.dat (9D0h bytes)**
 The two HWCAL files are factory provided (ie. the content and version are kept unchanged when installing firmware updates).
 The factory calibration seems to consist of several steps, with one of the HWCAL0/HWCAL1 files being updated after each step (with incrementing values in Header[00Dh], the newer file has that value one bigger than older file).
 
 System Settings allows to recalibrate some things (Touchscreen, Circle Pad, Outer Cameras, and maybe others), the changes are saved in the "config" file, without changing any HWCAL files (which can be used to restore the factory defaults).
 3DS Files - Config Savegame
 
 **Summary**
 Below shows the offset in file (and in raw data area), the size (excluding checksum and Value 0001h), the BlkID corresponding "config" file entries, and the Entry number (entry 00h..0Fh have flags in "Aging Flags" header[00Eh] bit0..15 accordingly, entry 10h and up have flags stored between data and checksum).
 
```

| File Data Size BlkID Entry Description
 000h - 200h - - Header
 200h 000h 10h 00040000h 03h Input Touchscreen
 214h 014h 08h 00040001h.1 04h Input Circle Pad (part 1)
 220h 020h 2 00050000h 01h LCD Screen flicker (VCOM)
 224h 024h 1 00010000h 00h RTC Compensation value (WHAT is that???)
 228h 028h 1 00030000h 07h RTC Correction value
 230h 030h 8Ah 00060000h.1 02h Outer Cameras (part 1)
 2BCh 0BCh 12h 00040002h 06h Input Gyroscope
 2D0h 0D0h 0Ch 00040003h 08h Input Accelerometer
 2E0h 0E0h 134h 00020000h 05h Codec (CDC aka TSC)
 418h 218h 04h 00050007h 16h LCD or Programmable Infrared (PIT);New3DS
 420h 220h 214h 00070000h 09h Sound Surround 3D filters
 640h 440h 20h 00050003h 0Ah LCD Power saving mode (ABL) (read by GSP)
 670h 470h 20h 00050005h 0Bh LCD Stereoscopic Display Settings
 6A0h 4A0h 38h 00050002h 0Ch LCD Backlight PWM (BLPWM)
 6E0h 4E0h 18h 00040001h.2 0Dh Input Circle Pad (part 2)
 700h 500h 0Ch 00060000h.2 0Eh Outer Cameras (part 2)
 710h 510h 20h 00050004h 0Fh LCD Power saving mode (ABL) (read by LGY)
 740h 540h 08h 00120000h 10h Slider Calibration
 750h 550h 02h 00050006h 11h LCD 2D/3D Mode Switching Delay
 760h 560h 08h 00070002h 12h Sound Microphone echo cancellation
 770h 570h 10Ch 00050008h 15h LCD Power saving mode (ABL) extra ;New3DS
 880h 680h 08h 00040004h 13h Input CStick (Right analog stick) ;New3DS
 890h 690h 18h 00180001h 17h QTM (Quad Tracking Module) ;New3DS
 N/A N/A - - 14h Entry 14h doesn't exist (or was it used?) |
| --- |

**Header**

```

| 000h 4 File ID "CCAL"
 004h 4 Version (eg. Old3DS:0Ch or 0Dh, New3DS:12h)
 008h 4 Data size (always 07D0h, for entry 200h..9CFh)
 00Ch 1 Model version (?) (usually 00h)
 00Dh 1 Factory HWCAL0/HWCAL1 revision counter (eg. 0Bh/0Ch or 0Fh/10h)
 00Eh 2 Aging Flags for Entry 00h..0Fh (Old3DS:71FEh, New3DS:77FEh)
 010h 20h SHA256-HMAC (retail) or SHA256 (dev) on 7D0h-byte data section
 030h 1D0h Zerofilled
 200h 7D0h Data |
| --- |

**Aging flags (for Header entry [00Eh])**

Unknown if these are actually test results, maybe they are just entry-present
flags? With later entries instead indicated by "Value 0001h" in the newly added
fields?

**BlkID=00040000h - Input Touchscreen**

```

| 200h 2 s16 RawX0 (analog value, usually 02xxh) ;\
 202h 2 s16 RawY0 (analog value, usually 02xxh) ; upper/left
 204h 2 s16 PointX0 (pixel coordinate, 0+32) ; calibration point
 206h 2 s16 PointY0 (pixel coordinate, 0+24) ;/
 208h 2 s16 RawX1 (analog value, usually 0Exxh) ;\
 20Ah 2 s16 RawY1 (analog value, usually 0Exxh) ; lower/right
 20Ch 2 s16 PointX1 (pixel coordinate, 320-32) ; calibration point
 20Eh 2 s16 PointY1 (pixel coordinate, 240-24) ;/
 210h 2 Checksum
 212h 2 Zero |
| --- |

**BlkID=00040001h.part1 - Input Circle Pad (left Analog Stick) (part 1)**

```

| 214h 2 s16 CenterX (usually 08xxh)
 216h 2 s16 CenterY (usually 08xxh)
 218h 4 Zero
 21Ch 2 Checksum
 21Eh 2 Zero |
| --- |

**BlkID=00050000h - LCD Screen flicker (VCOM)**

```

| 220h 1 u8 LCD Top Screen VCOM "flicker" ;for MCU[03h]
 221h 1 u8 LCD Bottom Screen VCOM "flicker" ;for MCU[04h]
 222h 2 Checksum ([220h] XOR FFFFh) |
| --- |

**BlkID=00010000h - RTC Compensation value (WHAT is that???)**

```

| 224h 1 u8 Compensation Value (???) (usually 80h)
 225h 1 Checksum ([224h] XOR FFh)
 226h 2 Zero |
| --- |

Unknown what this is... and if it is RTC related at all (maybe somebody just
confused it with the RTC Correction value below) (or if it's really RTC
related, it might be old prototype stuff, or related to emulated RTCs for
GBA/NDS?). Also, 3dbrew does randomly refer to it either as "FCRAM" or as "RTC
Compensation value".

**BlkID=00030000h - RTC Correction value**

```

| 228h 1 RTC Correction (spotted values are 86h, 90h or FEh) ;for MCU[37h]
 229h 1 Checksum ([228h] XOR FFh)
 22Ah 6 Zero |
| --- |

**BlkID=00060000h.part1 - Outer Cameras (part 1)**

```

| 230h 4 u32 flags (usually 0)
 234h 4 float scale (near +1.0)
 238h 4 float RotationZ (near 0.0)
 23Ch 4 float TranslationX (-86.0..-102.0)
 240h 4 float TranslationY (-2.0..-4.4) ;VAR
 244h 4 float RotationX (usually 0.0)
 248h 4 float RotationY (usually 0.0)
 24Ch 4 float ViewAngleRight (near +64.0)
 250h 4 float ViewAngleLeft (near +64.0)
 254h 4 float ChartDistance(?)(usually +250.0)
 258h 4 float CameraDistance (usually +35.0)
 25Ch 2 s16 ImageWidth (280h aka 640 decimal)
 25Eh 2 s16 ImageHeight (1E0h aka 480 decimal)
 260h 10h reserved (usually 0)
 270h 40h unknown (usually 0)
 2B0h 2 s16 aeBaseTarget(???) (usually 0)
 2B2h 2 s16 kRL (usually 0)
 2B4h 2 s16 kGL (usually 0)
 2B6h 2 s16 kBL (usually 0)
 2B8h 2 s16 ccmPosition (usually 0)
 2BAh 2 Checksum |
| --- |

**BlkID=00040002h - Input Gyroscope**

```

| 2BCh 2 s16 ZeroX (usually +/-00xxh) ;\
 2BEh 2 s16 PlusX (usually +19xxh..+1Axxh) ; X
 2C0h 2 s16 MinusX (usually -19xxh..-1Axxh) ;/
 2C2h 2 s16 ZeroY (usually +/-00xxh) ;\
 2C4h 2 s16 PlusY (usually +19xxh..+1Axxh) ; Y
 2C6h 2 s16 MinusY (usually -19xxh..-1Axxh) ;/
 2C8h 2 s16 ZeroZ (usually +/-00xxh) ;\
 2CAh 2 s16 PlusZ (usually +19xxh..+1Axxh) ; Z
 2CCh 2 s16 MinusZ (usually -19xxh..-1Axxh) ;/
 2CEh 2 Checksum |
| --- |

**BlkID=00040003h - Input Accelerometer**

```

| 2D0h 2 s16 OffsetX (usually +/-00xxh) ;\X
 2D2h 2 s16 ScaleX (usually +03Fxh..+040xh) ;/
 2D4h 2 s16 OffsetY (usually +/-00xxh) ;\Y
 2D6h 2 s16 ScaleY (usually +03Fxh..+040xh) ;/
 2D8h 2 s16 OffsetZ (usually +/-00xxh) ;\Z
 2DAh 2 s16 ScaleZ (usually +03Fxh..+040xh) ;/
 2DCh 2 Checksum
 2DEh 2 Zero |
| --- |

**BlkID=00020000h - Codec (CDC aka TSC)**

```

| 2E0h 1 u8 DriverGainHP ;TSC[65h:0Ch].bit3-7
 2E1h 1 u8 DriverGainSP ;TSC[65h:12h].bit2-7 and TSC[65h:13h].bit2-7
 2E2h 1 u8 AnalogVolumeHP ;TSC[65h:16h] and TSC[65h:17h]
 2E3h 1 u8 AnalogVolumeSP ;TSC[65h:1Bh] and TSC[65h:1Ch]
 2E4h 1 s8 ShutterVolume0 ;TSC[00h:41h] and TSC[00h:42h] ;\maybe camera
 2E5h 1 s8 ShutterVolume1 ;TSC[64h:7Bh] ;/sound fx?
 2E6h 1 u8 MicrophoneBias ;TSC[65h:33h]
 2E7h 1 u8 QuickCharge (???) ;TSC[65h:42h].bit0-1
 2E8h 1 u8 PGA_GAIN (mic) ;TSC[65h:41h].bit0-5
 2E9h 3 u8 reserved[3]
 2ECh 1Eh s16 FilterHP32[3*5] ;TSC[0Bh:02h..1Fh] and TSC[0Bh:42h..5Fh]
 30Ah 1Eh s16 FilterHP47[3*5] ;TSC[0Bh:20h..3Dh] and TSC[0Bh:60h..7Dh]
 328h 1Eh s16 FilterSP32[3*5] ;TSC[0Ch:02h..1Fh] and TSC[0Ch:42h..5Fh]
 346h 1Eh s16 FilterSP47[3*5] ;TSC[0Ch:20h..3Dh] and TSC[0Ch:60h..7Dh]
 364h 38h s16 FilterMic32[3+5*5];TSC[05h:08h..3Fh]
 39Ch 38h s16 FilterMic47[3+5*5];TSC[05h:48h..7Fh]
 3D4h 38h s16 FilterFree [3+5*5];TSC[08h:xxh, 09h;xxh, 0Ah:xxh]
 40Ch 1 u8 AnalogInterval ;TSC[67h:27h].lsbs ;\
 40Dh 1 u8 AnalogStabilize ;TSC[67h:19h].lsbs ;
 40Eh 1 u8 AnalogPrecharge ;TSC[67h:17h].msbs ; Touchscreen/CirclePad
 40Fh 1 u8 AnalogSense ;TSC[67h:17h].lsbs ;
 410h 1 u8 AnalogDebounce ;TSC[67h:1Bh].lsbs ;
 411h 1 u8 Analog_XP_Pullup ;TSC[67h:19h].msbs ;
 412h 1 u8 YM_Driver ;TSC[67h:1Bh].msb ;/
 413h 1 u8 reserved
 414h 2 Checksum
 416h 2 Zero |
| --- |

Caution: The various 16bit Filter values in the HWCAL and config files are
stored in LITTLE-ENDIAN, but the TSC chip wants them in BIG-ENDIAN, so one must
swap each 2 bytes.

**BlkID=00050007h - LCD or Programmable Infrared Transmitter? (PIT) ;New3DS**

-- New3DS only (zerofilled on Old3DS)

```

| 418h 2 u16 VisibleFactor (usually 4C2Bh)
 41Ah 2 u16 IRFactor (usually 4C2Bh)
 41Ch 2 Aging Flag 17h (0000h, hmmm, uncommonly... not set to 0001h ?)
 41Eh 2 Checksum |
| --- |

Unknown what that is, maybe the New3DS IR LED for internal camera?

**BlkID=00070000h - Sound Surround 3D filters**

```

| 420h 200h u16 SpecialFilter[0x100] <-- Old3DS: mostly zero, unlike New3DS
 620h 14h u32 IIRSurroundFilter[5] <-- both Old3DS and New3DS
 634h 2 Checksum
 636h 0Ah Zero |
| --- |

**BlkID=00050003h - LCD Power saving mode (ABL) (read by GSP)**

```

| 640h 4 u32 DitherPattern (C66C9339h)
 644h 2 s16 StartX (0000h)
 646h 2 s16 StartY (0000h)
 648h 2 u16 SizeX (0400h)
 64Ah 2 u16 SizeY (0400h)
 64Ch 2 s16 GTH_Ratio (0080h)
 64Eh 1 u8 DitherMode (00h)
 64Fh 1 u8 MinRS (72h)
 650h 1 u8 MaxRS (FFh)
 651h 1 u8 MinGTH (69h)
 652h 1 u8 MinMax (???) (FFh)
 653h 1 u8 ExMax (???) (20h)
 654h 1 u8 inertia (02h or 06h) (unlike BlkID=00050004h)
 655h 9 u8 LutListRS[9] (14h,2Ch,46h,5Ch,72h,82h,8Eh,98h,A0h)
 65Eh 2 u8 reserved[2] (00h,00h)
 660h 2 Checksum
 662h 0Eh Zero |
| --- |

Unknown what inertia means (timing delay, or accelerometer related, or
whatever). Unknown what GTH and RS means. ABL is said to relate to backlight.

**BlkID=00050005h - LCD Stereoscopic Display Settings**

```

| 670h 4 float32 Pupillary Distance in mm? (62.0)
 674h 4 float32 Distance between eyes and upper screen in mm? (289.0)
 678h 4 float32 Screen Width in mm (76.80) ;\size of Old3DS upper screen
 67Ch 4 float32 Screen Height in mm (46.08) ;/(New3DSXL uses same values)
 680h 4 float32 Unknown (10.0)
 684h 4 float32 Unknown (5.0)
 688h 4 float32 Unknown (55.58)
 68Ch 4 float32 Unknown (21.57)
 690h 2 Checksum
 692h 0Eh Zero |
| --- |

**BlkID=00050002h - LCD Backlight PWM (BLPWM)**

```

| 6A0h 4 float32 unknown (+0.0011 or +0.0003) ;\
 6A4h 4 float32 scale? (+1.4141 or +1.4250) ; lower screen?
 6A8h 4 float32 unknown (+0.0717 or -0.0616) ;/
 6ACh 4 float32 unknown (+0.0004 or +0.0002) ;\upper screen 2d mode?
 6B0h 4 float32 scale? (+0.6656 or +0.9951) ; (when parallax off) ?
 6B4h 4 float32 unknown (+0.0609 or +1.4314) ;/
 6B8h 4 float32 unknown (+0.0020 or +0.0008) ;\upper screen 3d mode?
 6BCh 4 float32 scale? (+1.5563 or +1.8836) ; (when parallax on) ?
 6C0h 4 float32 unknown (+0.0385 or +1.5579) ;/
 6C4h 1 u8 NumLevels (05h)
 6C5h 1 u8 Unknown (00h on Old3DS, but 80h on New3DSXL)
 6C6h 0Eh u16 brightnesses[7] (increasing values, in range 0010h..00ACh)
 6D4h 2 u16 BaseDivisor (0200h) ;aka PWM interval maybe?
 6D6h 2 u16 MinimumBrightnessHw (000Ah or 000Dh)
 6D8h 2 Checksum
 6DAh 06h Zero |
| --- |

**BlkID=00040001h.part2 - Input Circle Pad (left Analog Stick) (part 2)**

```

| 6E0h 4 float ScaleX (near +0.83) ;\unknown
 6E4h 4 float ScaleY (near +0.82) ;/purpose
 6E8h 2 s16 MaxX (usually 0Exxh)
 6EAh 2 s16 MinX (usually 02xxh)
 6ECh 2 s16 MaxY (usually 0Exxh)
 6EEh 2 s16 MinY (usually 02xxh)
 6F0h 2 s16 type (whatever, usually 0)
 6F2h 6 Unknown (zerofilled)
 6F8h 2 Checksum
 6FAh 06h Zero |
| --- |

**BlkID=00060000h.part2 - Outer Cameras (part 2)**

```

| 700h 2 Unknown (can be 001Dh or 0016h)
 700h 2 Unknown (can be 001Eh or 0016h)
 700h 2 Unknown (can be 0047h or 0055h)
 700h 2 Unknown (can be 0043h or 0055h)
 700h 2 Unknown (can be 0000h)
 700h 2 Unknown (can be 0000h)
 70Ch 2 Checksum
 70Eh 2 Zero |
| --- |

**BlkID=00050004h - LCD Power saving mode (ABL) (read by LGY)**

```

| 710h 4 u32 DitherPattern (C66C9339h)
 714h 2 s16 StartX (0000h)
 716h 2 s16 StartY (0000h)
 718h 2 u16 SizeX (0400h)
 71Ah 2 u16 SizeY (0400h)
 71Ch 2 s16 GTH_Ratio (0080h)
 71Eh 1 u8 DitherMode (00h)
 71Fh 1 u8 MinRS (72h)
 720h 1 u8 MaxRS (FFh)
 721h 1 u8 MinGTH (69h)
 722h 1 u8 MinMax (???) (FFh)
 723h 1 u8 ExMax (???) (20h)
 724h 1 u8 inertia (always 02h) (unlike BlkID=00050003h)
 725h 9 u8 LutListRS[9] (14h,2Ch,46h,5Ch,72h,82h,8Eh,98h,A0h)
 72Eh 2 u8 reserved[2] (00h,00h)
 730h 2 Checksum
 732h 0Eh Zero |
| --- |

**BlkID=00120000h - Slider Calibration**

```

| 740h 2 s16 SVR2_Min (???) (usually 0007h..001Bh) ;\maybe this is
 742h 2 s16 SVR2_Max (???) (usually 00F2h..00FDh) ;/3D Slider?
 744h 2 s16 VolumeSliderMin (usually 0004h..003Ch) ;use Min+8 for MCU[58h]
 746h 2 s16 VolumeSliderMax (usually 00F5h..00FFh) ;use Max-8 for MCU[59h]
 748h 2 Aging Flag 10h (0001h)
 74Ah 2 Checksum
 74Ch 4 Zero |
| --- |

**BlkID=00050006h - LCD 2D/3D Mode Switching Delay**

```

| 750h 1 u8 To2D, delay for switching to 2D mode (in frames??) (0Dh or F1h)
 751h 1 u8 To3D, delay for switching to 3D mode (in frames??) (00h or F1h)
 752h 2 Aging Flag 11h (0001h)
 754h 2 Checksum
 756h 0Ah Zero |
| --- |

Delays for upper LCD screen mode switching. The upper screen has three modes:
2D lores, 2D hires, 3D stereoscopic (the latter with parallax barrier and
higher backlight intensity), unknown how exactly those two delays relate to
those three modes (or to parallax/backlight changes).

**BlkID=00070002h - Sound Microphone echo cancellation**

```

| 760h 1 s8 Unknown (0Ch or 0Ah)
 761h 1 s8 Unknown (06h or 02h)
 762h 1 s8 Unknown (0Ch or 06h)
 763h 1 s8 Unknown (01h or 00h)
 764h 1 s8 Unknown (0Ch)
 765h 1 s8 Unknown (A3h)
 766h 1 s8 Unknown (00h)
 767h 1 s8 Unknown (08h)
 768h 2 Aging Flag 12h (0001h)
 76Ah 2 Checksum
 76Ch 4 Zero |
| --- |

This is all zerofilled in HWCAL files on my Old3DS (but "config" has nonzero
values).

Unknown if this is going to TSC registers or whatever. Also unknown what "echo"
means, maybe classic sound reflection on walls, or feedback(-loops) from
speaker to microphone?

**BlkID=00050008h - LCD Power saving mode (ABL) extra**

-- New3DS only (zerofilled on Old3DS)

```

| 770h 1 u8 MaxInertia (12h) ;guess: delay? pwm_interval?
 771h 1 u8 pad (00h)
 772h 2 u16 PWM_CNT_EX (0001h) ;guess: maybe Port 102022xxh/10202Axxh
 774h 4 u32 Histogram1 (00000000h) ;guess: maybe Port 10202250h/10202A50h
 778h 4 u32 Histogram2 (001E8E51h) ;guess: maybe Port 10202254h/10202A54h
 77Ch 100h u32 adjust[0x40] ;reportedly for Port 10202300h/10202B00h
 87Ch 2 Aging Flag 15h (0001h)
 87Eh 2 Checksum |
| --- |

Maybe something for maintaining the correct color hue at different backlight
levels? The 64 adjust values seem to be RGB multipliers (usually near
00808080h, or slightly off, like 00827D7Ch).

Unknown if that same settings are used for BOTH screens (and how that could
work, except by assuming that both screens are from same manufacturer with
similar characteristics).

**BlkID=00040004h.part - Input CStick (Right analog stick)**

-- New3DS only (zerofilled on Old3DS)

```

| 880h 1 u8 ThinningCountX (whut?) (whatever, usually 25h)
 881h 1 u8 ThinningCountY (whut?) (whatever, usually 25h)
 882h 6 Reserved (zerofilled)
 888h 2 Aging Flag 13h (0001h)
 88Ah 2 Checksum
 88Ch 4 Zero |
| --- |

This is same as in config (except, config has more extra zeroes appended).

**BlkID=00180001h - QTM (Quad Tracking Module)**

-- New3DS only (zerofilled on Old3DS)

```

| 890h 4 float DivisorAtZero (???) (eg. +5.787) (or def=+6.500)
 894h 4 float TranslationX (eg. -0.014) (or def=+0.000)
 898h 4 float TranslationY (eg. +0.001) (or def=+0.000)
 89Ch 4 float RotationZ (eg. -0.218) (or def=+0.000)
 8A0h 4 float HorizontalAngle (eg. +68.52) (or def=+66.40)
 8A4h 4 float OptimalDistance (eg. +319.2) (or def=+301.0)
 8A8h 2 Aging Flag 17h (eg. 0001h) (or def=0000h)
 8AAh 2 Checksum
 8ACh 124h Zero |
| --- |

The QTM values are written in the last calibration step, resulting in different
values in HWCAL0 vs HWCAL1 (the older HWCAL file does merely contain some
default values, the newer HWCAL file and config file contain the actual
calibration).

**HWCAL Checksums**

The SHA256-HMAC in the header is using the following 20h-byte HMAC key:

```

| 0D,36,F9,C8,EA,ED,15,A7,B9,22,E3,C7,70,E7,53,48
 DA,2D,57,35,C2,59,9F,93,5D,E1,81,4D,E6,65,40,8B |
| --- |

Checksums for data blocks depend on the length of the data part (including the
"Aging Flag" entry between data and checksum, if any):

```

| len=1 --> chksum.byte = data XOR FFh
 len=2 --> chksum.halfword = data XOR FFFFh
 len>=4 --> chksum.halfword = CRC16(data, initial=55AAh) |
| --- |

The chksum.byte can be located at unaligned locations, chksum.halfword is
always used with even lengths and stored at 2-byte aligned locations.

**EEPROM Reading (on dev consoles or so?)**

"If 0x1FF81006 (uh, what, where from?) is 3 or 4 or 7 or 8 or 9 then the
calibration data is read from the EEPROM using the i2c:EEP service command
0x001000C0, using offset 0x000 for HWCAL0, and offset 0x800 for HWCAL1.
Otherwise attempt is made to read CTRNAND:/ro/sys/HWCAL(0|1).dat instead."

**EEPROM Unknown things**

Unknown how to read the EEPROM data (maybe similar as for Wifi EEPROM?).

Unknown what size EEPRM has (apparently at least 1000h bytes).

Unknown if EEPROM has any header (maybe only 30h bytes instead 200h bytes?) (it
should/must have header entry[00Dh]).

Note: HWCAL and config are used by 3ds:\title\00040130\00001702 (cfg system
module).