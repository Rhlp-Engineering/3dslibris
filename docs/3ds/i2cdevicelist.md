# I2Cdevicelist

> Source: https://problemkaputt.de/gbatek.htm
> Section: I2Cdevicelist

3DS I2C Device List 
| |
| --- |

 **I2C Devices**
 
```

| id bus:dev service Device description
 0 0:4Ah "i2c::MCU" BPTWL (aka MCU registers for DSi mode) ;\same
 1 0:7Ah "i2c::CAM" Camera0 (internal camera, self-facing) ; as DSi
 2 0:78h "i2c::CAM" Camera1 (external camera, right eye) ;/
 3 1:4Ah "i2c::MCU" MCU (aka MCU registers for 3DS mode)
 4 1:78h "i2c::CAM" Camera2 (external camera, left eye)
 5 1:2Ch "i2c::LCD" Upper LCD screen (lcd0)
 6 1:2Eh "i2c::LCD" Lower LCD screen (lcd1)
 7 1:40h "i2c::DEB" Reserved for Debug?
 8 1:44h "i2c::DEB" Reserved for Debug?
 9 2:A6h/D6h"i2c::HID" Debug?? addr changed from A6h to D6h in 8.0.0-18
 10 2:D0h "i2c::HID" Gyroscope (old version) <-- read = FFh's ?
 11 2:D2h "i2c::HID" Gyroscope (new version) <-- EXISTS in New3DS
 12 2:A4h "i2c::HID" Reserved for DebugPad
 13 2:9Ah "i2c::IR" Infrared Transmitter/Receiver (IrDA)
 14 2:A0h "i2c::EEP" Reserved for DebugEeprom?
 15 1:EEh "i2c::NFC" New3DS-only NFC (Near-field communication)
 16 0:40h "i2c::QTM" New3DS-only QTM (head tracking?) IO Expander!
 17 2:54h "i2c::IR" New3DS-only C-stick and ZL/ZR.. 44pin chip??
 - none(?) - New3DS-only Hasn't co-packaged EEPROM in NFC chip?
 - 0:A0h - Reserved for Unknown DSi camera0 (Ext)
 - 0:E0h - Reserved for Unknown DSi camera1 (Self)
 - 0:40h - Reserved for Unknown DSi Debug stuff? and QTM
 - 0:90h - Reserved for Unknown DSi Debug stuff?
 - 0:00h-0Eh Mirrors to BPTWL for whatever reason
 - 0:F0h-FEh Mirrors to BPTWL for whatever reason
 - 0:5Ah Internal dummy addr used by BPTWL when busy?
 - 2:00h Unknown, something responds here with ACK and FFh's |
| --- |

Notice: These device addresses are used for writing to the respective device,
for reading bit0 must be set (see I2C protocol).

**3DS Secondary I2C Devices**

There are also some internal/secondary I2C busses (not connected to the ARM
CPUs).

```

| MCU:30h Accelerometer ;\
 MCU:6Ch Fuel Gauge MAX17040 (or newer chip in New3DS) ; connected to
 MCU:84h video related? ;Power Managment Device? ; MCU chip
 MCU:A4h batt.flg/volume? ;Touchscreen/Sound chip? ;/
 Wifi:A0h I2C bus EEPROM ;connected to Atheros Wifi chip
 NFC:Axh NFC chip has on-chip CAT24C64 EEPROM (?), but not wired to ARM (?) |
| --- |