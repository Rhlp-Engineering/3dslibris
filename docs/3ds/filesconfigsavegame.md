# Filesconfigsavegame

> Source: https://problemkaputt.de/gbatek.htm
> Section: Filesconfigsavegame

3DS Files - Config Savegame 
| |
| --- |

 This page describes the format of the "Cfg NAND savegame" (whut?). These blocks can be accessed with the "Cfg service commands".
 
 The data is stored in "3ds:\data\<ID0>\sysdata\00010017\00000000" (which is a DISA file with SAVE filesystem, containing only one file, ie. the "config" file described below).
 
 **Structure of save-file "/config" (8000h bytes)**
 
```

| Offset Size Description
 0000h 2 Total entries (N) (usually 32h..41h)
 0002h 2 End of unused space, begin of Data (455Ch..41E4h)
 0004h N*0Ch Config Block entries (see below)
 ... .. Unused space
 ... .. Data for the entries (used from top, starting at 7FFFh) |
| --- |

Config Block entry format:

```

| 00h 4 BlkID (00xx000xh)
 04h 4 Data (or, when Size>4, Offset to Data from begin of config file)
 08h 2 Size in bytes
 0Ah 2 Flags (usually 000Ch or 000Eh, unknown meaning) |
| --- |

In current firmware: The data is usually at 4-byte aligned locations, except
Gyroscope and Accelerometer are only 2-byte aligned (these consist of 16bit
entries, so that's no problem), unknown if other firmware versions have more
unaligned entries.

**Configuration blocks**

```

| BlkID Flags Size Description
 00000000h 0Ch 2 Config Version? (0015h or 0039h)
 00010000h 0Ch 1 RTC Compensation value (usually 80h) (WHAT is that???)
 00020000h 0Ch 134h Codec (CDC aka TSC)
 00030000h 0Ch 1 RTC Correction value (read by PTM)
 00030001h 0Eh 08h RTC Time offset (read by CECD): displayed_time-rtc_time
 00030002h 0Ch 08h RTC Settings Time offset: newly_set_time-rtc_time
 00040000h 0Ch 10h Input Touchscreen
 00040001h 0Ch 1Ch Input Circle Pad (left Analog Stick)
 00040002h 0Ch 12h Input Gyroscope
 00040003h 0Ch 0Ch Input Accelerometer
 00040004h 0Ch 1Ch Input CStick (Right analog stick)
 00050000h 0Ch 2 LCD Screen flicker (VCOM)
 00050001h 0Ch 2 LCD Backlight controls (read by GSP)
 00050002h 0Ch 38h LCD Backlight PWM (read by GSP)
 00050003h 0Ch 20h LCD Power saving mode (ABL) calibration (read by GSP)
 00050004h 0Ch 20h LCD Power saving mode (ABL) calibration (read by LGY)
 00050005h 0Eh 20h LCD Stereoscopic Display Settings
 00050006h 0Ch 2 LCD 2D/3D Mode Switching Delay
 00050007h 0Ch 4 LCD or Programmable Infrared Transmitter? (PIT) ;New3DS
 00050008h 0Ch 10Ch LCD Power saving mode (ABL) extra config ;New3DS
 00050009h 0Ch 08h LCD new3DS only(?) backlight control ;New3DS
 00060000h 0Ch 96h Outer Cameras
 00070000h 0Eh 214h Sound Surround 3D filters
 00070001h 0Eh 1 Sound Output Mode (mono=0, stereo=1, surround=2)
 00070002h 0Eh 08h Sound Microphone echo cancellation params
 00080000h 0Ch C00h Wifi Internet Access Point 1
 00080001h 0Ch C00h Wifi Internet Access Point 2
 00080002h 0Ch C00h Wifi Internet Access Point 3
 00090000h 0Eh 08h User ID 1 (for NWMUDS:InitializeWithVersion)
 00090001h 0Eh 08h User ID 2 (for GenHashConsoleUnique)
 00090002h 0Eh 4 User ID Random
 000A0000h 0Eh 1Ch User Name in UTF16 (and NGWord version)
 000A0001h 0Eh 2 User Birthday (u8 month, u8 day)
 000A0002h 0Eh 1 User Language
 000B0000h 0Eh 4 Local Country/State Codes
 000B0001h 0Eh 800h Local Country name in UTF-16 (in various languages)
 000B0002h 0Eh 800h Local State name in UTF-16 (in various languages)
 000B0003h 0Eh 4 Local Country/State Coordinates
 000C0000h 0Eh C0h Parental Restrictions
 000C0001h 0Eh 14h Parental Child Online Privacy Protection (COPPACS)
 000C0002h 0Eh 200h Parental Email and Custom Secret Question
 000D0000h 0Eh 4 EULA Version which was agreed to
 000E0000h 0Eh 1 Wifi Internet Spotpass Flags
 000F0000h 0Ch 10h System Debug configuration
 000F0001h 0Ch 8 System ?
 ;000F0002h - - System N/A (or did this exist in some/older firmwares?)
 000F0003h 0Ch 1 System ?
 000F0004h 0Ch 4 System Model value byte, followed by 3 unknown bytes
 000F0005h 0Ch 4 System Network Update Enable
 000F0006h 0Ch 28h System NPNS string for "X-Device-Token" http header field
 00100000h 0Ch 2 DSi EULA
 00100001h 0Ch 94h DSi Parental Restrictions
 00100002h 0Ch 1 DSi Country
 00100003h 0Ch 10h DSi User ID
 00110000h 0Ch 4 Boot System Setup Request flag
 00110001h 0Ch 8 Boot TitleID of Home Menu
 00120000h 0Ch 8 Slider Calibration (Read by HID and PTM)
 00130000h 0Eh 4 Debug mode enable
 ;00140000h - - Reserved BlkID for SecureInfo Region ;\not used in
 ;00140001h - - Reserved BlkID for SecureInfo Serial/Barcode ;/config file
 00150000h 0Ch 4 ?
 00150001h 0Ch 8 ?
 00150002h 0Eh 4 NPNS: Character for making the URL for NPNS
 00160000h 0Eh 4 Unknown, 1st byte is used by config service-cmd 00070040h
 00170000h 0Eh 4 Miiverse (OLV) access key
 00180000h 0Ch 4 QTM Infrared LED related, can be 0 or 1 ... maybe "PIT"?
 00180001h 0Ch 18h QTM calibration data
 00190000h 0Ch 1 NFC module (checks for value1/non-value1) |
| --- |

The Debug/New3DS specific entries do exist even in Retail/Old3DS config files
(except, old firmware versions don't have New3DS entries, of course).

**BlkID=00000000h - Config Version?**

```

| 000h 2 Config Version (0015h or 0039h) |
| --- |

**BlkID=00010000h - RTC Compensation value (WHAT is that???)**

```

| 000h 1 Whatever (usually 80h) |
| --- |

**BlkID=00020000h - Codec (CDC aka TSC)**

```

| 000h 134h XXX see HWCAL |
| --- |

Caution: The various 16bit Filter values in the HWCAL and config files are
stored in LITTLE-ENDIAN, but the TSC chip wants them in BIG-ENDIAN, so one must
swap each 2 bytes.

**BlkID=00030000h - RTC Correction value (read by PTM)**

```

| 000h 1 RTC Correction (spotted values are 86h, 90h or FEh) ;for MCU[37h] |
| --- |

**BlkID=00030001h - RTC Time offset (read by CECD)**

```

| 000h 8 RTC displayed_time-rtc_time ;in bizzare NANOSECONDS !!! |
| --- |

Offset in nanoseconds between MCU[30h..36h] and actual time/date (if the MCU is
set to the actual time/date, then this offset should be zero) (but normally,
the MCU is just starting at 01 Jan 2001 00:00:00 when the battery was inserted,
and this offset is used to convert the MCU time/date to actual time/date, see
MCU chapter for details).

**BlkID=00030002h - RTC Settings Time offset**

```

| 000h 8 RTC newly_set_time-rtc_time ;uh, really??? |
| --- |

Unknown. This is often 0000000000000000h, even after changing the time in
system settings. But, it can change to something like FFFFFA400B6F8C00h
after... playing a game? or connecting to internet? or battery loss? or so?

**BlkID=00040000h - Input Touchscreen**

```

| 000h 2 s16 RawX0 (analog value, usually 02xxh) ;\
 002h 2 s16 RawY0 (analog value, usually 02xxh) ; upper/left
 004h 2 s16 PointX0 (pixel coordinate, 0+32) ; calibration point
 006h 2 s16 PointY0 (pixel coordinate, 0+24) ;/
 008h 2 s16 RawX1 (analog value, usually 0Exxh) ;\
 00Ah 2 s16 RawY1 (analog value, usually 0Exxh) ; lower/right
 00Ch 2 s16 PointX1 (pixel coordinate, 320-32) ; calibration point
 00Eh 2 s16 PointY1 (pixel coordinate, 240-24) ;/ |
| --- |

**BlkID=00040001h - Input Circle Pad (left Analog Stick)**

```

| 000h 4 float ScaleX (near +0.83) ;\unknown HWCAL[6E0h] ;\
 004h 4 float ScaleY (near +0.82) ;/purpose HWCAL[6E4h] ;/
 008h 2 s16 CenterX (usually 08xxh) HWCAL[214h] ;\
 00Ah 2 s16 CenterY (usually 08xxh) HWCAL[216h] ;
 - - Unknown (zerofilled) HWCAL[218h] ;/
 00Ch 2 s16 MaxX (usually 0Exxh) HWCAL[6E8h] ;\
 00Eh 2 s16 MinX (usually 02xxh) HWCAL[6EAh] ;
 010h 2 s16 MaxY (usually 0Exxh) HWCAL[6ECh] ;
 012h 2 s16 MinY (usually 02xxh) HWCAL[6EEh] ;
 014h 2 s16 type (whatever, usually 0) HWCAL[6F0h] ;
 016h 6 Unknown (zerofilled) HWCAL[6F2h] ;/ |
| --- |

This is composed of two HWCAL entries (at 214h and 6E0h), unknown if older
consoles did have only one of those HWCAL entries (and hence might have had a
smaller config entry).

Some consoles have slightly different values in HWCAL versus config.

**BlkID=00040002h - Input Gyroscope**

```

| 000h 2 s16 ZeroX (usually +/-00xxh) ;\
 002h 2 s16 PlusX (usually +19xxh..+1Axxh) ; X
 004h 2 s16 MinusX (usually -19xxh..-1Axxh) ;/
 006h 2 s16 ZeroY (usually +/-00xxh) ;\
 008h 2 s16 PlusY (usually +19xxh..+1Axxh) ; Y
 00Ah 2 s16 MinusY (usually -19xxh..-1Axxh) ;/
 00Ch 2 s16 ZeroZ (usually +/-00xxh) ;\
 00Eh 2 s16 PlusZ (usually +19xxh..+1Axxh) ; Z
 010h 2 s16 MinusZ (usually -19xxh..-1Axxh) ;/ |
| --- |

**BlkID=00040003h - Input Accelerometer (read by HID)**

```

| 000h 2 s16 OffsetX (usually +/-00xxh) ;\X
 002h 2 s16 ScaleX (usually +03Fxh..+040xh) ;/
 004h 2 s16 OffsetY (usually +/-00xxh) ;\Y
 006h 2 s16 ScaleY (usually +03Fxh..+040xh) ;/
 008h 2 s16 OffsetZ (usually +/-00xxh) ;\Z
 00Ah 2 s16 ScaleZ (usually +03Fxh..+040xh) ;/ |
| --- |

**BlkID=00040004h - Input CStick (Right analog stick)**

```

| 000h 1 u8 ThinningCountX (whut?) (whatever, usually 25h) ;\
 001h 1 u8 ThinningCountY (whut?) (whatever, usually 25h) ; HWCAL[880h]
 002h 6 Reserved (zerofilled) ;/
 008h 14h Reserved (zerofilled) ;-EXTRA ZEROES |
| --- |

This is same as in HWCAL (except, config has more extra zeroes appended).

Old3DS can use the Circle Pad Pro hardware add-on as Right analog stick,
unknown if calibration data for that add-on is stored anywhere in HWCAL and/or
config.

**BlkID=00050000h - LCD Screen flicker (VCOM)**

```

| 000h 1 u8 LCD Top Screen VCOM "flicker" ;for MCU[03h]
 001h 1 u8 LCD Bottom Screen VCOM "flicker" ;for MCU[04h] |
| --- |

**BlkID=00050001h - LCD Backlight controls (read by GSP)**

```

| 000h 1 u8 ABL_powersave_enable (00h=Off, 01h=On)
 001h 1 u8 brightness_level (01h..05h) |
| --- |

Backlight brightness and power save can be changed via small icon in upper-left
of System Menu (rather than via System Settings).

**BlkID=00050002h - LCD Backlight PWM (read by GSP)**

```

| 000h 4 float32 unknown (+0.0011 or +0.0003) ;\
 004h 4 float32 scale? (+1.4141 or +1.4250) ; lower screen?
 008h 4 float32 unknown (+0.0717 or -0.0616) ;/
 00Ch 4 float32 unknown (+0.0004 or +0.0002) ;\upper screen 2d mode?
 010h 4 float32 scale? (+0.6656 or +0.9951) ; (when parallax off) ?
 014h 4 float32 unknown (+0.0609 or +1.4314) ;/
 018h 4 float32 unknown (+0.0020 or +0.0008) ;\upper screen 3d mode?
 01Ch 4 float32 scale? (+1.5563 or +1.8836) ; (when parallax on) ?
 020h 4 float32 unknown (+0.0385 or +1.5579) ;/
 024h 1 u8 NumLevels (05h)
 025h 1 u8 Unknown (00h on Old3DS, but 80h on New3DSXL)
 026h 0Eh u16 brightnesses[7] (increasing values, in range 0010h..00ACh)
 034h 2 u16 BaseDivisor (0200h) ;aka PWM interval maybe?
 036h 2 u16 MinimumBrightnessHw (000Ah or 000Dh) |
| --- |

**BlkID=00050003h - LCD Power saving mode (ABL) (read by GSP)**

**BlkID=00050004h - LCD Power saving mode (ABL) (read by LGY)**

```

| 000h 4 u32 DitherPattern (C66C9339h)
 004h 2 s16 StartX (0000h)
 006h 2 s16 StartY (0000h)
 008h 2 u16 SizeX (0400h)
 00Ah 2 u16 SizeY (0400h)
 00Ch 2 s16 GTH_Ratio (0080h)
 00Eh 1 u8 DitherMode (00h)
 00Fh 1 u8 MinRS (72h)
 010h 1 u8 MaxRS (FFh)
 011h 1 u8 MinGTH (69h)
 012h 1 u8 MinMax (???) (FFh)
 013h 1 u8 ExMax (???) (20h)
 014h 1 u8 inertia (02h or 06h) (always 02h for BlkID=00050004h)
 015h 9 u8 LutListRS[9] (14h,2Ch,46h,5Ch,72h,82h,8Eh,98h,A0h)
 01Eh 2 u8 reserved[2] (00h,00h) |
| --- |

**BlkID=00050005h - LCD Stereoscopic Display Settings (from HWCAL)**

```

| 000h 4 float32 Pupillary Distance in mm? (62.0)
 004h 4 float32 Distance between eyes and upper screen in mm? (289.0)
 float32 Screen Width in mm (76.80) ;\size of Old3DS upper screen
 008h 4 float32 Screen Height in mm (46.08) ;/(New3DSXL uses same values)
 00Ch 4 float32 Unknown (10.0)
 010h 4 float32 Unknown (5.0)
 014h 4 float32 Unknown (55.58)
 018h 4 float32 Unknown (21.57)
 01Ch 4 21.57f |
| --- |

"All values are hard-coded in cfg module." Uh, shouldn't they come from HWCAL
file?

**BlkID=00050006h - LCD 2D/3D Mode Switching Delay**

```

| 000h 1 u8 To2D, delay for switching to 2D mode (in frames??) (0Dh or F1h)
 001h 1 u8 To3D, delay for switching to 3D mode (in frames??) (00h or F1h) |
| --- |

**BlkID=00050007h - LCD or Programmable Infrared Transmitter? (PIT) ;New3DS**

```

| 000h 2 u16 VisibleFactor (Old3DS:4000h, or New3DSXL:4C2Bh)
 002h 2 u16 IRFactor (Old3DS:4000h, or New3DSXL:4C2Bh) |
| --- |

Unknown what that is, maybe the New3DS IR LED for internal camera?

**BlkID=00050008h - LCD Power saving mode (ABL) extra config**

-- New3DS only (zerofilled on Old3DS)

```

| 000h 1 u8 MaxInertia (12h) ;guess: delay? pwm_interval?
 001h 1 u8 pad (00h)
 002h 2 u16 PWM_CNT_EX (0001h) ;guess: maybe Port 102022xxh/10202Axxh
 004h 4 u32 Histogram1 (00000000h) ;guess: maybe Port 10202250h/10202A50h
 008h 4 u32 Histogram2 (001E8E51h) ;guess: maybe Port 10202254h/10202A54h
 00Ch 100h u32 adjust[0x40] ;reportedly for Port 10202300h/10202B00h |
| --- |

**BlkID=00050009h - LCD new3DS only(?) backlight control**

```

| 000h 4 Unknown (maybe... float32 value 1.0 ?) (3F800000h)
 004h 1 Reportedly "5th byte: auto-brightness enable" (01h)
 005h 1 Unknown (01h)
 006h 1 Unknown (00h)
 007h 1 Unknown (00h) |
| --- |

**BlkID=00060000h - Outer Cameras**

```

| 000h 4 u32 flags (usually 0) ;\
 004h 4 float scale (near +1.0) ;
 008h 4 float RotationZ (near 0.0) ;
 00Ch 4 float TranslationX (-86.0..-102.0) ;
 010h 4 float TranslationY (-2.0..-4.4) ;VAR ; HWCAL[230h]
 014h 4 float RotationX (usually 0.0) ;
 018h 4 float RotationY (usually 0.0) ;
 01Ch 4 float ViewAngleRight (near +64.0) ;
 020h 4 float ViewAngleLeft (near +64.0) ;
 024h 4 float ChartDistance(?)(usually +250.0) ;
 028h 4 float CameraDistance (usually +35.0) ;
 02Ch 2 s16 ImageWidth (280h aka 640 decimal) ;
 02Eh 2 s16 ImageHeight (1E0h aka 480 decimal) ;
 030h 10h Reserved (usually 0) ;
 040h 40h Unknown (usually 0) ;
 080h 2 s16 aeBaseTarget(???) (usually 0) ;
 082h 2 s16 kRL (usually 0) ;
 084h 2 s16 kGL (usually 0) ;
 086h 2 s16 kBL (usually 0) ;
 088h 2 s16 ccmPosition (usually 0) ;/
 08Ah 2 Unknown (can be 001Dh or 0016h) ;\
 08Ch 2 Unknown (can be 001Eh or 0016h) ;
 08Eh 2 Unknown (can be 0047h or 0055h) ; HWCAL[700h]
 090h 2 Unknown (can be 0043h or 0055h) ;
 092h 2 Unknown (can be 0000h) ;
 094h 2 Unknown (can be 0000h) ;/ |
| --- |

The TranslationY entry [010h] can be calibrated in System Settings (to align
the vertical position/direction? of Left+Right cameras with each other).

**BlkID=00070000h - Sound Surround 3D filters**

```

| 000h 200h u16 SpecialFilter[0x100] <-- Old3DS: mostly zero, unlike New3DS
 200h 14h u32 IIRSurroundFilter[5] <-- both Old3DS and New3DS |
| --- |

**BlkID=00070001h - Sound Output Mode (mono=0, stereo=1, surround=2)**

```

| 000h 1 Sound output mode (mono=0, stereo=1, surround=2) |
| --- |

**BlkID=00070002h - Sound Microphone echo cancellation params**

```

| 760h 1 s8 Unknown (0Ch or 0Ah)
 761h 1 s8 Unknown (06h or 02h)
 762h 1 s8 Unknown (0Ch or 06h)
 763h 1 s8 Unknown (01h or 00h)
 764h 1 s8 Unknown (0Ch)
 765h 1 s8 Unknown (A3h)
 766h 1 s8 Unknown (00h)
 767h 1 s8 Unknown (08h) |
| --- |

**BlkID=00080000h - Wifi Internet Access Point 1**

**BlkID=00080001h - Wifi Internet Access Point 2**

**BlkID=00080002h - Wifi Internet Access Point 3**

```

| 000h 2 Entry in Use (0000h=Unused/Deleted, 0001h=Used)
 002h 2 CRC16 across 410h bytes at [004h..413h] (with initial value 0000h)
 004h 88h 1st network structure. Only set if the network was set
 "normally", or was the last to be set using WPS during the
 session
 08Ch 20h Padding
 0ACh 88h 2nd network structure (if network was set using WPS, otherwise 0's)
 134h 20Ch Padding
 340h 1 Obtain IP, Gateway, Subnet (0=Manual, 1=Auto/DHCP)
 341h 1 Obtain DNS Server address (0=Manual, 1=Auto/DHCP)
 342h 2 Padding ?
 344h 4 IP address ;\
 348h 4 Gateway ; used only if [340h]=00h
 34Ch 4 Subnet Mask ;/
 350h 4 Primary DNS Server ;\used only if [341h]=00h
 354h 4 Secondary DNS Server ;/
 358h 1 Recent Connect (0=No, 1=Yes) ;\
 359h 1 Recent Type (Open,WEP,WPA,etc.) ;
 36Ah 2 Padding ? ; Recent Connection status
 35Ch 4 Recent Local IP address (of 3DS) ; (can be zerofilled if none such)
 360h 6 Recent Remote MAC address (or AP);
 366h 1 Recent Channel (01h..0xh) ;
 367h 1 Padding ? ;/
 368h 1 Proxy Enable (00h=None, 01h=Yes)
 369h 1 Proxy Authentication (00h=None, 01h=Yes)
 36Ah 2 Proxy Port (16bit)
 36Ch 30h Proxy Name (ASCII string, max 47 chars, padded with 00's)
 39Ch 34h Padding
 3D0h 20h Proxy Authentication Username (ASCII string, padded with 00's)
 3F0h 20h Proxy Authentication Password (ASCII string, padded with 00's)
 410h 2 Padding ?
 412h 2 MTU Value (Max transmission unit) (576..1500, usually 1400)
 414h 7ECh Padding (to C00h byte size) |
| --- |

Network structure (used twice in the above Wifi slot structure):

```

| 00h 1 Whether the network was set or not?
 01h 1 Whether to use this network structure to connect?
 02h 1 Whether this structure is the first (0) or the second (1) in the
 larger Wifi slot structure?
 03h 1 Padding ?
 04h 20h SSID (ASCII string, padded with 00's) (see below for length)
 24h 1 SSID Length in characters (01h..20h, or 00h=unused)
 25h 1 AP crypto key type (maybe related to WEP/WPA bytes on NDS?)
 26h 2 Padding ?
 28h 40h WPA/WPA2 password (ASCII string, padded with 00's)
 68h 20h Precomputed PSK (based on WPA/WPA2 password and SSID) |
| --- |

The above data is (slightly reordered) also stored in Wifi FLASH:

DS Firmware Wifi Internet Access Points

The DSi did have support for six access points (3xWEP and 3xWEP/WPA), unknown
if the old 3xWEP ones are still supported on 3DS, and if so, if they are also
stored in the Config Samegame file.

Note: The Wifi Disable flag is stored in MCU memory at MCU[61h:00h].bit0, the
ARM code should toggle that bit upon Wifi button presses (for whatever reason,
the flag is stored in battery backed MCU memory instead of config file).

**BlkID=00090000h - User ID 1 (for NWMUDS:InitializeWithVersion)**

**BlkID=00090001h - User ID 2 (for GenHashConsoleUnique)**

```

| 000h 6 LocalFriendCodeSeed, aka Decrypted OTP[08h..0Dh] ;mask 34bit?
 006h 2 Random 16bit (same value as in BlkID=00090002h) |
| --- |

**BlkID=00090002h - User ID Random (same value as in BlkID=00090000h/00090001h)**

```

| 000h 2 Random 16bit (generated by GenerateRandomBytes)
 002h 2 Zero |
| --- |

**BlkID=000A0000h - User Name in UTF16 (and NGWord version)**

```

| 000h 14h UTF-16 username, with no NULL-terminator (uh, what if shorter?)
 014h 4 Usually zero?
 018h 4 u32 Last checked NGWord version for username (eg. 0Eh or 12h) |
| --- |

If NGWord version is less than the u32 stored in the NGWord CFA
"romfs:/version.dat", the system then checks the username string with the
bad-word list CFA again, then updates this field with the value from the CFA.

Note: NGWord means "N-o-t-g-o-o-d" words, also known as "N**good shit".

**BlkID=000A0001h - User Birthday**

```

| 000h 1 Month (01h..0Ch)
 001h 1 Day (01h..1Fh) |
| --- |

**BlkID=000A0002h - User Language**

```

| 000h 1 Language (00h..0Bh) |
| --- |

Languages:

```

| ID Description (two letter code... and guessed meaning)
 0 = JP Japanese
 1 = EN English
 2 = FR French
 3 = DE German
 4 = IT Italian
 5 = ES Spanish
 6 = ZH ...maybe a widespread chinese dialect...?
 7 = KO Korean
 8 = NL Dutch
 9 = PT Portuguese
 10 = RU Russian
 11 = TW ...maybe a taiwanese chinese dialect...? |
| --- |

**BlkID=000B0000h - Local Country/State Codes**

```

| 000h 1 Unknown (0)
 001h 1 Unknown (0)
 002h 1 State/Province code (optional, 0=Not set, 8=Hamburg)
 003h 1 Country code, same as for DSi/Wii (eg. 4Eh=Germany, FFh=Invalid) |
| --- |

DSi Regions

States can be selected only in some countries (eg. Germany, India).

**BlkID=000B0001h - Local Country name in UTF-16 (in various languages)**

**BlkID=000B0002h - Local State name in UTF-16 (in various languages)**

```

| 000h 800h Names in UTF-16 |
| --- |

Every 80h-bytes is an entry for each language, in the order of the Language
table above (not all entries are set).

**BlkID=000B0003h - Local Country/State Coordinates**

```

| 000h 2 s16 Latitude in 180/32768 degrees (eg. 2614h=Germany/Hamburg, 53'N)
 002h 2 s16 Longitude in 180/32768 degrees (eg. 071Ah=Germany/Hamburg, 9'E) |
| --- |

**BlkID=000C0000h - Parental Restrictions**

```

| 000h 4 Parental Control Flags (see below, eg. 00000FFFh=Restrict all)
 004h 4 Unknown (0)
 008h 1 Parental Controls Region (0=Off, 3=German/USK, 4=French?)
 009h 1 Parental Controls Years of Age Rating (00h..14h) ;cart[2F0h]
 00Ah 1 Parental Controls Secret Question (00h..05h or 06h=Custom)
 00Bh 1 Unknown (0)
 00Ch 4+1 Parental Controls PIN (ASCII digits) 4 digits+EOL
 011h 3 Unknown (0)
 014h 42h Parental Controls Secret Answer (UCS-2), max 32 chars+EOL ?
 056h 6Ah Unknown (0) ;note: answer is max 32 chars (unlike DSi) |
| --- |

Parental Control Flags:

```

| 0 Global Parental Controls enable (1=Enable Restrictions)
 1 Internet Browser (1=Restrict)
 2 Display of 3D Images (parallax barrier) (1=Restrict)
 3 Sharing Images/Audio/Video/Long Text Data (1=Restrict)
 4 Online Interaction (1=Restrict)
 5 StreetPass (1=Restrict)
 6 Friend Registration (1=Restrict)
 7 DS Download Play (1=Restrict)
 8 Nintendo 3DS Shopping Services (eShop) (1=Restrict)
 9 Viewing Distributed Videos (1=Restrict)
 10 Miiverse (view) (1=Restrict)
 11 Miiverse (post) (1=Restrict)
 12-30 Unknown (zero)
 31 Child Online Privacy Protection (see CFG:IsCoppacsSupported) (?) |
| --- |

**BlkID=000C0001h - Parental Child Online Privacy Protection (COPPACS)**

```

| 000h 1 Unknown (can be 01h)
 001h 13h Unknown (0) |
| --- |

**BlkID=000C0002h - Parental Email and Custom Secret Question**

```

| 000h 1 Parent's Email was set (00h=No, 01h=Yes) (optional)
 001h 101h Parent's Email String (in ASCII, max 256 chars?, plus EOL)
 102h 68h Custom Secret Question (in UCS-2, max 51 chars, plus EOL)
 16Ah 96h Unknown (0) |
| --- |

Note: The email can be actually VERY long (although one must enter it twice,
which is almost impossible for long strings).

**BlkID=000D0000h - EULA Version which was agreed to**

```

| 000h 2 u16 EULA Version which was agreed to (eg. 0100h or 0101h)
 002h 2 Unknown (0) |
| --- |

This gets reset to 0000h=None when changing the Country setting, requiring to
reagree.

**BlkID=000E0000h - Wifi Internet Spotpass Flags**

```

| 000h 1 Enable Flags (bit0=SendUsageInfo, bit1:AutomaticSoftwareDownload) |
| --- |

**BlkID=000F0000h - System Debug configuration**

```

| 000h 8 Unknown (0)
 008h 4 Unknown (0 or 6)
 00Ch 4 Unknown (3) |
| --- |

Read by NS on dev-units: On startup, NS does svcKernelSetState(6, 1,
(u64)debug_flags & 1); svcKernelSetState(6, 2, (u64)debug_flags & 2);
(see here) where debug_flags is the u32 located at offset 0xC in this struct.
Then it compares the u32 from +8 in this config-block with the APPMEMTYPE. When
those don't match NS starts a FIRM-launch (with the same FIRM titleID as the
currently running one) to boot into a FIRM with the APPMEMTYPE value from this
config-block.

The byte at offset 0 is related to the memtype as well.

**BlkID=000F0001h - System ?**

```

| 000h 8 Unknown (0) |
| --- |

**BlkID=000F0003h - System ?**

```

| 000h 1 Unknown (0) |
| --- |

**BlkID=000F0004h - System Model value byte, followed by 3 unknown bytes**

```

| 000h 1 System Model value byte (0=Old3DS, 4=New3DSXL) ;as MCU[7Fh:09h]
 001h 3 Unknown (0) |
| --- |

**BlkID=000F0005h - System Network Update Enable**

```

| 000h 1 Network updates enabled (WHAT=Yes, WHAT=No?) (usually 1)
 ("however, NIM only checks this flag with developer ENVINFO")
 001h 3 Unknown (0) |
| --- |

**BlkID=000F0006h - System NPNS string for "X-Device-Token" http header field**

```

| 000h 28h Whatever string in ASCII, UTF or whatever? (usually zerofilled) |
| --- |

In NIM, taken as a (hopefully null terminated) string used for the
"X-Device-Token" http header field for NPNS url.

**BlkID=00100000h - DSi EULA (flag/version?)**

```

| 000h 1 Unknown (FFh)
 001h 1 EULA Agreed (00h=No/CountryChanged, 01h=Yes/Agreed) |
| --- |

Same as BlkID=000D0000h, but with fewer bytes.

**BlkID=00100001h - DSi Parental Restrictions**

```

| 000h 1 Parental Controls Flags (bit0=Parental, bit1-6=Pictochat,etc)
 001h 6 Zero
 007h 1 Parental Controls Region (0=Off, 3=German/USK, 4=French?)
 008h 1 Parental Controls Years of Age Rating (00h..14h) ;cart[2F0h]
 009h 1 Parental Controls Secret Question (00h..05h or 06h=Custom)
 00Ah 1 Parental Controls Unknown (can be 00h, 06h, or 07h)
 00Bh 2 Zero
 00Dh 4+1 Parental Controls PIN (ASCII digits) 4 digits+EOL
 012h 82h Parental Controls Secret Answer (UCS-2), max 64 chars+EOL |
| --- |

Same as BlkID=000C0000h, but slightly reformatted to match DSi TWLCFGn format.

**BlkID=00100002h - DSi Country**

```

| 000h 1 Country (eg. 4Eh=Germany) |
| --- |

Same as BlkID=000B0000h, including non-DSi countries like 71h=Azerbaijan, but
containing only the Country byte, without State/Province.

**BlkID=00100003h - DSi User ID**

Reportedly: TWL "movable" UID, used for DSiWare exports

Maybe equivalent to DSi's HWINFO_N values at 2000600h, or 2000600h+4?

```

| 000h 10h User ID (DD,E8,51,xx,xx,xx,xx,xx,xx,xx,xx,xx,xx,xx,xx,xx) |
| --- |

This is created as follows:

```

| ID[00h..07h] = Random (from ps:ps GenerateRandomBytes)
 ID[08h..0Fh] = Last 8 bytes of SHA256(BlkID=90001h, plus 4-byte zeropadding)
 ID[00h..02h] = 5F,5F,5F (overwrite first 3 random bytes by fixed value)
 ID = ID XOR 82,B7,0E,F3,AD,0A,35,85,AA,84,AE,14,E7,06,D5,00
 (that is, after XORing, the fixed 5F,5F,5F bytes will be DD,E8,51) |
| --- |

Note that the 16bit random value in BlkID=90001h doesn't neccessarily match up
(maybe System Format can change BlkID=90001h, but without changing
BlkID=100003h?).

First 3 bytes seem to be fixed, the other bytes are console unique ID or Key.

**BlkID=00110000h - Boot System Setup Request flag (usually 00010001h)**

Indicates whether the system setup is required, such as when the system is
booted for the first time or after doing a System Format.

```

| 000h 2 System Setup Request flag (0=Setup required, non-zero=Not required)
 002h 2 Unknown |
| --- |

**BlkID=00110001h - Boot TitleID of Home Menu**

```

| 000h 8 TitleID (eg. EUR=0004004000009802h or JPN=0004004000008202h) |
| --- |

This ID can be edited on dev units (allowing to launch other menues/titles).

**BlkID=00120000h - Slider Calibration (Read by HID and PTM)**

```

| 000h 2 s16 SVR2_Min (???) (usually 0007h..001Bh) ;\maybe this is
 002h 2 s16 SVR2_Max (???) (usually 00F2h..00FDh) ;/3D Slider?
 004h 2 s16 VolumeSliderMin (usually 0004h..003Ch) ;use Min+8 for MCU[58h]
 006h 2 s16 VolumeSliderMax (usually 00F5h..00FFh) ;use Max-8 for MCU[59h] |
| --- |

The sliders are factory calibrated and cannot be changed in System Settings
(the "3D calibration" in System Settings seems to be merely a tutorial for
moving the slider, not for changing the calibration values).

**BlkID=00130000h - Debug mode enable**

```

| 000h 4 Unknown (0) |
| --- |

"If response is 0x100 then debug mode is enabled." Umph, which response, where
from? Maybe wants to say that [000h]=00000100h is debug enable??

**BlkID=00140000h - Reserved BlkID for SecureInfo Region**

**BlkID=00140001h - Reserved BlkID for SecureInfo Serial/Barcode**

These BlkIDs don't exist in the config file (the LGY FIRM does reportedly use
that BlkIDs for entries that are to be read from the SecureInfo file, instead
of from config file).

**BlkID=00150000h - ?**

```

| 000h 4 Unknown (24F0h or 9D0h) (aka 9456 and 2512) |
| --- |

**BlkID=00150001h - ?**

```

| 000h 8 Unknown (0) |
| --- |

**BlkID=00150002h - NPNS: Character for making the URL for NPNS**

```

| 000h 4 Unknown (0000314Ch) |
| --- |

"In NIM, taken as an u32 using the low u16, casted to a char, and turned lower
case for the making of the url for NPNS (unknown what that means)." Uh, how
does 314Ch cast to a char?

**BlkID=00160000h - Unknown, 1st byte is used by config service-cmd 00070040h**

```

| 000h 1 Unknown (0) ;Whatever, used by config service-cmd 0x00070040
 001h 3 Unknown (0) |
| --- |

**BlkID=00170000h - Miiverse (OLV) access key**

```

| 000h 4 Unknown (0) |
| --- |

**BlkID=00180000h - QTM Infrared LED related, can be 0 or 1**

```

| 000h 4 Unknown (0) |
| --- |

**BlkID=00180001h - QTM calibration data**

```

| 000h 4 float DivisorAtZero (???) (eg. +5.787) (or def=+6.500)
 004h 4 float TranslationX (eg. -0.014) (or def=+0.000)
 008h 4 float TranslationY (eg. +0.001) (or def=+0.000)
 00Ch 4 float RotationZ (eg. -0.218) (or def=+0.000)
 010h 4 float HorizontalAngle (eg. +68.52) (or def=+66.40)
 014h 4 float OptimalDistance (eg. +319.2) (or def=+301.0) |
| --- |

**BlkID=00190000h - NFC module (checks for value1/non-value1)**

```

| 000h 1 Unknown (0) (01h=What, Other=Whut) |
| --- |

**NDS/DSi Mode**

The Favorite color and User message for NDS/DSi titles can be changed in 3DS
System Settings, however, those two settings are only stored in NDS User
Settings (in Wifi FLASH), not in the config file.