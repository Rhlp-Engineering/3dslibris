# I2Cmcu40H51Haccelerometerpedometer

> Source: https://problemkaputt.de/gbatek.htm
> Section: I2Cmcu40H51Haccelerometerpedometer

3DS I2C MCU[40h-51h] - Accelerometer/Pedometer 
| |
| --- |

 **MCU Registers**
 
```

| 40h R/W Accelerometer Mode (bit0=AccelerometerOn, bit1=PedometerOn) ;\
 41h R/W Accelerometer Index for Manual I2C Read via MCU[44h] ;
 42h R/W Unused ;
 43h R/W Accelerometer Index for Manual I2C Write via MCU[44h] ;
 44h R/W Accelerometer Data from/to Read/Write via MCU[41h/43h] ;
 45h,46h R Accelerometer Output X (lsb,msb) ;resting=+/-00xxh ;
 47h,48h R Accelerometer Output Y (lsb,msb) ;resting=+/-00xxh ;
 49h,4Ah R Accelerometer Output Z (lsb,msb) ;resting=-41xxh (gravity) ;
 4Bh R/W Pedometer Step Count, bit0-7 ;\(for the current day) ;
 4Ch R/W Pedometer Step Count, bit8-15 ; (uh, how/which day?) ;
 4Dh R/W Pedometer Step Count, bit16-23 ;/(rather total count?) ;
 An up/down movement (on whichever axis that corresponds ;
 to gravity direction) is treated as step; however, the ;
 first step isn't counted, so 5 continous steps would ;
 increment the counter by 4. ;
 4Eh R/W Pedometer Flags (Wr.bit0=ClearArray?, Rd.bit4=ArrayFull?) ;
 4Fh * R Pedometer Timestamp[6] and StepCounts[2*A8h] ;ARRAY[6+2*A8h] ;
 50h,51h R/W Pedometer Minute,Second compare values (?) ;/ |
| --- |

Reading the Output X/Y/Z registers would be the normal way to get the
accelerometer data (if enabled in the mode register).

**Pedometer (Step counter)**

The Pedometer is some software feature in the MCU that converts the
accelerometer data to step counters.

Power-off switches the accelerometer power supply off. However, it is kept
powered & can do step counting in sleep mode. Step counting is used for
"Play Coins" and the pre-installed statistic tool "Omoide Kirokuchou" (probably
has other name in english?).

**Manual access to Accelerometer I2C registers**

The manual read/write feature allows to view/modify I2C registers directly; for
debugging/hacking purposes or so. The manual read/write seem to be done in a
callback function (=not immediately after writing MCU[41h] or MCU[44h]), and,
before touching further accelerometer registers, one must wait until the
callback has executed (which is indicated by MCU.interrupt.bit11).

**Accelerometer Chip**

On Old3DS it is marked "2048, 33DH, X1MAQ", which is said to be ST LIS331DLH.

On New3DSXL it is marked "KXTKK, 40860, .3413", which might be a Kionix chip,
resembling KXTIK, but with LIS331DLH-style register map & device address.

Both Old3DS and New3DSXL have the chip on the mainboard, somewhere underneath
of the bottom-screen. That (and the exact location) may be important when
interpreting the accelerometer data (ie. the chip isn't parallel with the
top-screen unless the case is fully unfolded).

The Accelerometer chip is wired to the MCU, so it's I2C registers can be
accessed only indirectly, through the MCU.

```

| ___________________ Internal Registers (for manual access) ___________________ |
| --- |

**Internal LIS331DLH Accelerometer I2C Register map (for manual access)**

```

| Addr Type Default Name
 00h-0Eh - - - Reserved (do not modify)
 0Fh R 00110010 WHO_AM_I Device identification register (32h)
 10h-1Fh - - - Reserved (do not modify)
 20h RW 00000111 CTRL_REG1 Power Control
 21h RW 00000000 CTRL_REG2 Filter Control
 22h RW 00000000 CTRL_REG3 Interrupt Control
 23h RW 00000000 CTRL_REG4 Misc Control
 24h RW 00000000 CTRL_REG5 Sleep to Wake Control
 25h R - HP_FILTER_RESET High-Pass Filter Clear (Read=clear)
 26h RW 00000000 REFERENCE High-pass Filter Reference value
 27h R 00000000 STATUS_REG Output Status
 28h R output OUT_X_L Out.X.L ;\usually near +/-00xxh
 29h R output OUT_X_H OUT.X.H ;/
 2Ah R output OUT_Y_L OUT.Y.L ;\usually near +/-00xxh
 2Bh R output OUT_Y_H OUT.Y.H ;/
 2Ch R output OUT_Z_L OUT.Z.L ;\usually near -41xxh (gravity)
 2Dh R output OUT_Z_H OUT.Z.H ;/
 2Eh-2Fh - - - Reserved (do not modify)
 30h RW 00000000 INT1_CFG Interrupt 1 Config ;\
 31h R 00000000 INT1_SOURCE Interrupt 1 Status ; INT 1
 32h RW 00000000 INT1_THS Interrupt 1 Threshold ;
 33h RW 00000000 INT1_DURATION Interrupt 1 Duration ;/
 34h RW 00000000 INT2_CFG Interrupt 2 Config ;\
 35h R 00000000 INT2_SOURCE Interrupt 2 Status ; INT 2
 36h RW 00000000 INT2_THS Interrupt 2 Threshold ;
 37h RW 00000000 INT2_DURATION Interrupt 2 Duration ;/
 38h-3Fh - - - Reserved (do not modify)
 40h-7Fh - - - Undocumented (zero)
 80h-FFh Same as 00h-7Fh, with auto-incrementing index |
| --- |

**ACCL[0Fh] - WHO_AM_I - Device identification register (R)**

```

| 0-7 Fixed (32h for LIS331DLH) |
| --- |

**ACCL[20h] - CTRL_REG1 - Power Control (R/W)**

```

| 0 Xen X axis enable (0=Disable, 1=Enable) (default=1)
 1 Yen Y axis enable (0=Disable, 1=Enable) (default=1)
 2 Zen Z axis enable (0=Disable, 1=Enable) (default=1)
 3-4 DR Data rate selection (0=50Hz, Others: see Table 20) (default=0)
 5-7 PM Power mode selection (0=Power-down, Other: see Table 19) (default=0) |
| --- |

PM bits allow to select between power-down and two operating active modes. The
device is in power-down mode when PD bits are set to "000" (default value after
boot). Table 19 shows all the possible power mode configurations and respective
output data rates. Output data in the low-power modes are computed with
low-pass filter cut-off frequency defined by DR1, DR0 bits.

DR bits, in the normal-mode operation, select the data rate at which
acceleration samples are produced. In low-power mode they define the output
data resolution. Table 20 shows all the possible configuration for DR1 and DR0
bits.

Table 19. Power mode and low-power output data rate configurations:

```

| PM Power mode selection Output data rate ODR'LP
 00h Power-down --
 01h Normal mode <ODR>
 02h Low-power 0.5Hz
 03h Low-power 1Hz
 04h Low-power 2Hz
 05h Low-power 5Hz
 06h Low-power 10Hz |
| --- |

Table 20. Normal-mode output data rate configurations and low-pass cut-off

frequencies:

```

| DR Output Data Rate ODR Low-pass filter cut-off frequency
 00h 50Hz 37Hz
 01h 100Hz 74Hz
 02h 400Hz 292Hz
 03h 1000Hz 780Hz |
| --- |

**ACCL[21h] - CTRL_REG2 - Filter Control (R/W)**

```

| 0-1 HPCF High pass filter cut-off frequency (HPc) (0..3 = 8,16,32,64)
 2 HPen1 High pass filter for interrupt 1 src (0=Bypassed, 1=Filter enabled)
 3 HPen2 High pass filter for interrupt 2 src (0=Bypassed, 1=Filter enabled)
 4 FDS Filtered data selection (0=Internal filter bypassed, 1=Data from
 internal filter sent to output register)
 5-6 HPM High pass filter mode (0=Normal mode, Other=see Table 23)
 7 BOOT Reboot memory content (0=Normal mode, 1=Reboot memory content) |
| --- |

BOOT bit is used to refresh the content of internal registers stored in the
flash memory block. At the device power up the content of the flash memory
block is transferred to the internal registers related to trimming functions to
permit a good behavior of the device itself.

If for any reason the content of trimming registers was changed it is
sufficient to use this bit to restore correct values. When BOOT=1 the content
of internal flash is copied inside corresponding internal registers and it is
used to calibrate the device. These values are factory trimmed and they are
different for every accelerometer. They permit a good behavior of the device
and normally they have not to be changed. At the end of the boot process the
BOOT bit is set again to 0.

```

| Table 23. High-pass filter mode configuration
 HPM High-pass filter mode
 00h Normal mode (reset reading HP_RESET_FILTER)
 01h Reference signal for filtering
 02h Normal mode (reset reading HP_RESET_FILTER) |
| --- |

HPCF[1:0]. These bits are used to configure high-pass filter cut-off frequency
ft which is given by:

```

| ... XXX ... |
| --- |

The equation can be simplified to the following approximated equation:

```

| ... XXX ... |
| --- |

Table 24. High-pass filter cut-off frequency configuration

```

| HPcoeff2,1 ft [Hz] ft [Hz] ft [Hz] ft [Hz]
 Data rate=50Hz Data rate=100Hz Data rate=400Hz Data rate=1000Hz
 00 1 2 8 20
 01 0.5 1 4 10
 10 0.25 0.5 2 5
 11 0.125 0.25 1 2.5 |
| --- |

**ACCL[22h] - CTRL_REG3 - Interrupt Control (R/W)**

```

| 0-1 I1_CFG Data signal on INT 1 pad control bits (see table below)
 2 LIR1 Latch IRQ on INT1_SRC register, with INT1_SRC cleared by reading
 INT1_SRC itself (0=IRQ not latched, 1=IRQ latched)
 3-4 I2_CFG Data signal on INT 2 pad control bits (see table below)
 5 LIR2 Latch IRQ on INT2_SRC register, with INT2_SRC cleared by reading
 INT2_SRC itself (0=IRQ not latched, 1=IRQ latched)
 6 PP_OD Interrupt Push-pull/Open drain (0=Push-pull, 1=Open drain)
 7 IHL Interrupt active high/low (0=Active high, 1=Active low) |
| --- |

Table 27. Data signal on INT 1 and INT 2 pad

```

| I#_CFG INT1/INT2 Pad
 00h Interrupt 1/2 source
 01h Interrupt 1 source OR interrupt 2 source
 02h Data ready
 03h Boot running |
| --- |

**ACCL[23h] - CTRL_REG4 - Misc Control (R/W)**

```

| 0 SIM SPI serial interface mode (0=4-wire, 1=3-wire)
 1 ST Self-test enable (0=No, 1=Self-test)
 2 - Reserved (0)
 3 STsign Self-test sign (0=Self-test Plus; 1=Self-test Minus)
 4-5 FS Full-scale selection (0..3 = +/-2g, +/-4g, Reserved, +/-8g)
 6 BLE Big/little endian data selection (0=Litte-endian, 1=Big-endian)
 7 BDU Block data update (0=Continuos update, 1=Output registers
 not updated between MSB and LSB reading) |
| --- |

BDU bit is used to inhibit output registers update between the reading of upper
and lower register parts. In default mode (BDU=0) the lower and upper register
parts are updated continuously. If it is not sure to read faster than output
data rate, it is recommended to set BDU=1. In this way, after the reading of
the lower (upper) register part, the content of that output registers is not
updated until the upper (lower) part is read too.

This feature avoids reading LSB and MSB related to different samples.

**ACCL[24h] - CTRL_REG5 - Sleep to Wake Control (R/W)**

```

| 0-1 Turn-on mode selection for sleep to wake function (0 or 3)
 2-7 Reserved (0) |
| --- |

TurnOn bits are used for turning on the sleep to wake function.

```

| Table 32. Sleep to wake configuration
 TurnOn Sleep to wake status
 00h Sleep to wake function is disabled
 03h Turned on: The device is in low power mode (ODR=Defined in CTRL_REG1) |
| --- |

Setting TurnOn=3 the "sleep to wake" function is enabled. When an interrupt
event occurs the device is turned to normal mode increasing the ODR to the
value defined in CTRL_REG1. Although the device is in normal mode, CTRL_REG1
content is not automatically changed to "normal mode" configuration.

**ACCL[25h] - HP_FILTER_RESET - High-Pass Filter Clear content (R=clear)**

Dummy register. Reading at this address zeroes instantaneously the content of
the internal high pass-filter. If the high pass filter is enabled all three
axes are instantaneously set to 0g.

This allows to overcome the settling time of the high pass filter.

**ACCL[26h] - REFERENCE - High-pass Filter Reference value (R/W)**

```

| 0-7 Reference value for high-pass filter. Default=00h |
| --- |

This register sets the acceleration value taken as a reference for the
high-pass filter output.

When filter is turned on (at least one of FDS, HPen2, or HPen1 bit is equal to
1) and HPM bits are set to "01", filter out is generated taking this value as a
reference.

**ACCL[27h] - STATUS_REG - Output Status (R)**

```

| 0 XDA New X axis new data available (0=No, 1=Available)
 1 YDA New Y axis new data available (0=No, 1=Available)
 2 ZDA New Z axis new data available (0=No, 1=Available)
 3 ZYXDA New set of X,Y,Z axis available (0=No, 1=Available)
 4 XOR Overrun has overwritten X axis data (0=No, 1=Overrun)
 5 YOR Overrun has overwritten Y axis data (0=No, 1=Overrun)
 6 ZOR Overrun has overwritten Z axis data (0=No, 1=Overrun)
 7 ZYXOR Overrun has overwritten X,Y,Z axis data (0=No, 1=Overrun) |
| --- |

Overrun means that previous data got overwritten before it was read.

Unknown if the Status bits get cleared after reading the Status register, or
after reading the Output registers, or elsewhere?

**ACCL[28h,29h] - OUT_X_L, OUT_X_H - X-axis acceleration data (R)**

**ACCL[2Ah,2Bh] - OUT_Y_L, OUT_Y_H - Y-axis acceleration data (R)**

**ACCL[2Ch,2Dh] - OUT_Z_L, OUT_Z_H - Z-axis acceleration data (R)**

```

| 0-15 The value is expressed as two's complement. |
| --- |

**ACCL[30h] - INT1_CFG - Interrupt 1 source Configuration (R/W)**

**ACCL[34h] - INT2_CFG - Interrupt 2 source Configuration (R/W)**

```

| 0 XLIE Enable interrupt generation on X low event (0=Disable, 1=Enable)
 1 XHIE Enable interrupt generation on X high event (0=Disable, 1=Enable)
 2 YLIE Enable interrupt generation on Y low event (0=Disable, 1=Enable)
 3 YHIE Enable interrupt generation on Y high event (0=Disable, 1=Enable)
 4 ZLIE Enable interrupt generation on Z low event (0=Disable, 1=Enable)
 5 ZHIE Enable interrupt generation on Z high event (0=Disable, 1=Enable)
 6 6D 6 direction detection function enable. (See table below)
 7 AOI AND/OR combination of interrupt events. (See table below) |
| --- |

The six X/Y/Z high/low events trigger when measured accel. value is
higher/lower than preset threshold.

Interrupt mode configuration

```

| AOI 6D Interrupt mode
 0 0 OR combination of interrupt events
 0 1 6 direction movement recognition
 1 0 AND combination of interrupt events
 1 1 6 direction position recognition |
| --- |

**ACCL[31h] - INT1_SRC - Interrupt 1 source Status (R)**

**ACCL[35h] - INT2_SRC - Interrupt 2 source Status (R)**

```

| 0 XL X low event has occurred (0=No, 1=Yes)
 1 XH X high event has occurred (0=No, 1=Yes)
 2 YL Y low event has occurred (0=No, 1=Yes)
 3 YH Y high event has occurred (0=No, 1=Yes)
 4 ZL Z low event has occurred (0=No, 1=Yes)
 5 ZH Z high event has occurred (0=No, 1=Yes)
 6 IA Interrupt active (0=No, 1=One or more interrupts have been generated)
 7 - Reserved (0) |
| --- |

Reading at this address clears INTn_SRC.bit6 (and the interrupt signal on INTn
pin) and allows the refreshment of data in the INTn_SRC register if the latched
option was chosen.

**ACCL[32h] - INT1_THS - Interrupt 1 threshold (R/W)**

**ACCL[36h] - INT2_THS - Interrupt 1? threshold (R/W)**

```

| 0-6 THS Interrupt threshold. Default value: 000 0000
 7 - Reserved (0) |
| --- |

**ACCL[33h] - INT1_DURATION - Minimum duration of Interrupt 2? event (R/W)**

**ACCL[37h] - INT2_DURATION - Minimum duration of Interrupt 2 event (R/W)**

```

| 0-6 D Duration value. Default value: 000 0000
 7 - Reserved (0) |
| --- |

These bits set the minimum duration of the Interrupt 2(?) event to be
recognized. Duration time steps and maximum values depend on the ODR chosen.