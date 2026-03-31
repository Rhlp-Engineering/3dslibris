# I2Cgyroscopeoldversion

> Source: https://problemkaputt.de/gbatek.htm
> Section: I2Cgyroscopeoldversion

3DS I2C Gyroscope (old version) 
| |
| --- |

 I2C Device 2:D0h
 
 **GYRO Register Map - InvenSense ITG-3270 gyroscope (used in Old 3DS)**
 
```

| Hex Register Name R/W
 00h WHO_AM_I R/W Device ID (aka Who Am I)
 15h SMPLRT_DIV R/W Sample Rate Divider (reset=00h)
 16h DLPF_FS R/W Low Pass Filter and Full Scale Config (reset=00h)
 17h INT_CFG R/W Interrupt Configuration (reset=00h)
 1Ah INT_STATUS R Interrupt Status (reset=00h)
 1Bh/1Ch TEMP_OUT_H/L R signed 16bit Temperature data (R)
 1Dh/1Eh GYRO_XOUT_H/L R signed 16bit Gyro X output data (R)
 1Fh/20h GYRO_YOUT_H/L R signed 16bit Gyro Y output data (R)
 21h/22h GYRO_ZOUT_H/L R signed 16bit Gyro Y output data (R)
 3Eh PWR_MGM R/W Power Management (reset=00h)
 xxh-FFh Undocumented (unknown) |
| --- |

Note: The 16bit H/L values are in BIG-ENDIAN format.

**Register Description**

The register space allows single-byte reads and writes, as well as burst reads
and writes. When performing burst reads or writes, the memory pointer will
increment until reading or writing is terminated by the master, or until the
memory pointer reaches "certain reserved registers between 21h and 3Ch" (uh,
that would include GYRO_ZOUT at 21h and/or 22h?).

Note that any bit that is not defined should be set to zero.

**GYRO[00h] - WHO_AM_I - Device ID (R/W)**

```

| 0 Reserved ("may be 0 or 1") (maybe ID.bit1, maybe R/W, or what?) (?)
 1-6 I2C Device ID bit2-7 (initially 34h on power-up, aka D0h/4) (R/W)
 7 Reserved ("should be 0") (?) |
| --- |

Contains the upper 6bit of the 8bit I2C Device ID, can be used for detection,
the value is also write-able and can be changed after power-on.

**GYRO[15h] - SMPLRT_DIV - Sample Rate Divider (R/W)**

```

| 0-7 Sample Rate Divider (00h..FFh=Divide by 1..100h) (initially 00h) |
| --- |

The gyros outputs are sampled internally at either 1kHz or 8kHz, determined by
the DLPF_CFG setting (see register 22). This sampling is then filtered
digitally and delivered into the sensor registers after the number of cycles
determined by this register. The sample rate is given by the following formula:

```

| Fsample = Finternal / (divider+1) ;where Finternal is either 1kHz or 8kHz |
| --- |

As an example, if the internal sampling is at 1kHz, then setting this register
to 7 would give the following:

```

| Fsample = 1kHz / (7 + 1) = 125Hz, or 8ms per sample |
| --- |

**GYRO[16h] - DLPF_FS - Low Pass Filter and Full Scale Config (R/W)**

```

| 0-2 DLPF_CFG Digital low pass filter bandwidth & internal sample rate
 3-4 FS_SEL Full scale selection for gyro sensor data
 5-7 Reserved (should be 0) |
| --- |

FS_SEL Gyro Full-Scale Range:

```

| 0 Reserved (despite of being power-up default)
 1 Reserved
 2 Reserved
 3 +/-2000ø/sec (this should be used) |
| --- |

DLPF_CFG, Low Pass Filter Bandwidth, Internal Sample Rate:

```

| 0 Internal Sample Rate=8kHz, Low Pass Filter Bandwidth=256Hz
 1 Internal Sample Rate=1kHz, Low Pass Filter Bandwidth=188Hz
 2 Internal Sample Rate=1kHz, Low Pass Filter Bandwidth=98Hz
 3 Internal Sample Rate=1kHz, Low Pass Filter Bandwidth=42Hz
 4 Internal Sample Rate=1kHz, Low Pass Filter Bandwidth=20Hz
 5 Internal Sample Rate=1kHz, Low Pass Filter Bandwidth=10Hz
 6 Internal Sample Rate=1kHz, Low Pass Filter Bandwidth=5Hz
 7 Reserved |
| --- |

**GYRO[17h] - Interrupt Enable and INT Pin Configuration (R/W)**

```

| 0 Enable interrupt when new sample data is available (1=Enable)
 1 -
 2 Enable interrupt when PLL ready ;after changing clock source (1=Enable)
 3 -
 4 Latch clear method (0=Status register read only, 1=Any register read);\
 5 Latch mode (0=50us pulse, 1=latch until interrupt is cleared); INT
 6 Drive type for INT output pin (0=push-pull, 1=open drain) ; pin
 7 Logic level for INT output pin (0=active high, 1=active low) ;/ |
| --- |

Unknown what the PLL ready IRQ is good for, maybe the whole I2C bus becomes
unstable/inactive and shouldn't be used until receiving the IRQ?

**GYRO[1Ah] - Interrupt Status (R)**

```

| 0 New sample data is ready (0=No, 1=Yes/IRQ)
 1 -
 2 PLL clock source change ready (0=No, 1=Yes/IRQ)
 3-7 - |
| --- |

If the interrupt is not enabled, the associated status bit will NOT get set!

Interrupt Status bits are cleared after reading GYRO[1Ah] (or optionally, if
GYRO[17h].bit4=1, after reading any GYRO[xxh] register).

**GYRO[1Bh/1Ch] - TEMP_OUT_H/L signed 16bit Temperature data (R)**

**GYRO[1Dh/1Eh] - GYRO_XOUT_H/L signed 16bit Gyro X output data (R)**

**GYRO[1Fh/20h] - GYRO_YOUT_H/L signed 16bit Gyro Y output data (R)**

**GYRO[21h/22h] - GYRO_ZOUT_H/L signed 16bit Gyro Y output data (R)**

Sensor Registers. These values can be read at any time; however it is best to
use the interrupt function to determine when new data is available.

**GYRO[3Eh] - Power Management (R/W)**

```

| 0-2 CLK_SEL Select device clock source (see below)
 3 STBY_ZG Gyro Z standby mode (0=Normal, 1=Standby) ;\eg. disable
 4 STBY_YG Gyro Y standby mode (0=Normal, 1=Standby) ; unused axis
 5 STBY_XG Gyro X standby mode (0=Normal, 1=Standby) ;/to save power
 6 SLEEP Enable low power sleep mode (0=Normal, 1=Very low power sleep)
 7 H_RESET Reset device and internal registers to power-up-default settings |
| --- |

The CLK_SEL setting determines the device clock source as follows:

```

| 0 Internal oscillator (default on power-up, but unreliable, PLLs are better)
 1 PLL with X Gyro reference
 2 PLL with Y Gyro reference
 3 PLL with Z Gyro reference
 4 PLL with external 32.768kHz reference ;\unknown if available in 3DS,
 5 PLL with external 19.2MHz reference ;/maybe from 32KHz RTC output?
 6 Reserved
 7 Reserved |
| --- |

**Gyro Datasheets (for ITG-3200 which is hopefully same as ITG-3270)**

```

| Hardware: ITG-3200-Datasheet.pdf ITG-3200 Product Specification (v1.7)
 Software: ITG-3200-Register-Map.pdf ITG-3200 Register Map (v1.0) |
| --- |

Older versions did include the Register Map in the Product Specification.