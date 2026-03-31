# I2Cotherunuseddebugdevices

> Source: https://problemkaputt.de/gbatek.htm
> Section: I2Cotherunuseddebugdevices

3DS I2C Other/Unused/Debug Devices 
| |
| --- |

 **Device 7 (Bus/device 1:40h) "i2c::DEB" Debug?**
 **Device 8 (Bus/device 1:44h) "i2c::DEB" Debug?**
 Unknown.
 
 **Device 9 (Bus/device 2:A6h/D6h) "i2c::HID" Debug??**
 Unknown.
 Device Address changed from A6h to D6h in 8.0.0-18.
 
 **Device 12 (Bus/device 2:A4h) "DebugPad"**
 
```

| Register Width Description
 00h 21 DebugPad state. |
| --- |

The DebugPad is reportedly a slightly modified "Wii Classic Controller Pro"
(with type F0h instead of FDh).

**Device 14 (Bus/device 2:A0h) "eeprom?"**

Used by Cfg-sysmodule via the i2c::EEP service. This is presumably EEPROM going
by the service name.

The Cfg-module code which loads the CCAL (nandro:/sys/{HWCAL0.dat/HWCAL1.dat})
file from NAND will load it from I2C instead, if a certain state flag is
non-zero. Likewise for the function which writes CCAL to NAND. HMAC/hash
verification after loading is skipped when the CCAL was loaded from I2C.

**Bus/device 0:00h-0Eh - Mirrors to BPTWL for whatever reason**

**Bus/device 0:F0h-FEh - Mirrors to BPTWL for whatever reason**

These are special broadcast/reserved device ids (defined in I2C protocol). The
MCU chip apparently includes a hardware feature for decoding these IDs; and
accidentally mirrors the BPTWL registers to them.

**Bus/device 2:00h - Unknown, something responds here with ACK and FFh's**

This is a special broadcast address (defined in I2C protocol). One (or more) of
the devices on I2C Bus 2 do apparently respond to it... and perhaps even
support some broadcast commands?