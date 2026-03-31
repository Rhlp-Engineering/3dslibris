# I2Cmcuregistersummary

> Source: https://problemkaputt.de/gbatek.htm
> Section: I2Cmcuregistersummary

3DS I2C MCU Register Summary 
| |
| --- |

 **Bus/Device 1:4Ah - MCU**
 The McuIndex is usually automatically incremented after each data byte (eg. allowing to read the two Version bytes in one pass).
 However, there are a few arrays (at McuIndex=29h,2Dh,4Fh,61h,7Fh), which will increment the ArrayIndex instead of the McuIndex. This happens only if the index value (in the I2C command) was pointing to the array (eg. writing 5 bytes to index 29h will update the Power LED Array. But, writing 5 bytes to index 28h will update McuIndex 28h,29h,2Ah,2Bh,2Ch; without getting stuck at 29h, and thus updating only the 1st byte of the Power LED Array).
 The extra-special case is using index 60h in the I2C command (it will increment once, then stay at index 61h).
 
 **MCU Registers**
 
```

| 00h R Version high (bit0-3) and hardware.type? (bit4-7) ;\Firm
 01h R Version low (8bit) ;/
 02h R/W Reset Event flags ;-Stat
 03h R/W LCD Top Screen VCOM "flicker" ;\
 04h R/W LCD Bottom Screen VCOM "flicker" ;/
 05h-07h R/W MCU Firmware update unlock sequence ;ARRAY[4003h] ;-Firm
 08h R Raw 3D slider position ;\
 09h R Volume slider state (00h..3Fh) (as MCUHWC:GetSoundVolume) ;
 0Ah R Battery PCB Temperature Celsius (signed) ; Stat
 0Bh R Fuel Gauge Battery Percentage, msb (percent, 0..64h) ;
 0Ch R Fuel Gauge Battery Percentage, lsb (percent/256, 0..FFh) ;
 0Dh R Fuel Gauge Battery Voltage (in 20mV units) ;
 0Eh R Sub-Device Access Flags? (bit0,1,2-3,4) ;
 0Fh R Power Status Flags ;/
 10h-13h R MCU Interrupt Flags, bit0-31 (1=IRQ, cleared after read) ;\
 14h R Unused (cleared after reading, like above IRQ flags) ;
 15h-17h R/W Unused ; IRQs
 18h-1Bh R/W MCU Interrupt Mask, bit0-31 (0=Enable, 1=Disable) ;
 1Ch-1Fh R/W Unused ;/
 20h W System Power Control (bits are 0=No change, 1=Trigger) ;\
 21h W DSi Power Button Status register BPTWL[10h] change ;
 22h W LCD Power Control (bits are 0=No change, 1=Trigger) ; PWR
 23h - Unused (has a dummy write handler, but does nothing) ;
 24h R/W Forced Power Off delay (0=Never, 1=Fastest, 5Dh=Insane) ;/
 25h R/W Unused (0)
 26h R/W Unused (initiallly 0, but, ARM sets MCU[26h]=10h!)
 27h R/W Volume slider raw ADC value ;-Stat
 28h R/W Brightness of Wifi/Power/3D LEDs ;\
 29h * R/W Power LED state + Power LED blink pattern ;ARRAY[5] ;
 2Ah R/W Wifi LED state, 4 bits wide ; LEDs
 2Bh R/W Camera LED state, 4bits wide ;
 2Ch R/W 3D LED state, 4 bits wide ;
 2Dh * W Notification LED Array (4+3x20h bytes) ;ARRAY[64h] ;
 2Eh R Notification LED Status when read (1=new cycle started) ;/
 2Fh - Unused (has a dummy write handler, but does nothing)
 30h R/W RTC Time second (7bit) (BCD, 00h..59h) ;\
 31h R/W RTC Time minute (7bit) (BCD, 00h..59h) ;
 32h R/W RTC Time hour (6bit) (BCD, 00h..23h) ;
 33h R/W RTC Time day of week? (3bit) (?..?, ?=Monday) ;
 (renesas calls this "Week 0=Sunday") ; RTC
 (unknown what Nintendo is using here) ;
 34h R/W RTC Time day (6bit) (BCD, 01h..31h) ;
 35h R/W RTC Time month (5bit) (BCD, 01h..12h) ;
 36h R/W RTC Time year (8bit) (BCD, 00h..99h) ;
 37h R/W RTC Watch Error Correction (SUBCUD) ;NOT leap year ;
 38h R/W RTC Alarm minute (7bit) (BCD, 00h..59h) ;
 39h R/W RTC Alarm hour (6bit) (BCD, 00h..23h) ;
 3Ah R/W RTC Alarm day (6bit) (BCD, 01h..31h) ;\maybe 0=off? ;
 3Bh R/W RTC Alarm month (5bit) (BCD, 01h..12h) ;/ ;
 3Ch R/W RTC Alarm year (8bit) (BCD, 00h..99h) ;
 3Dh R RTC RSUBC.lsb (in 32768Hz units) ;\range 0..7FFFh ;
 3Eh R RTC RSUBC.msb (latched when reading lsb) ;/(or 0..80xxh) ;
 3Fh W RTC Flags (bit0=ScreenBlack?, bit1=DisableRtc32KHzOutput) ;/
 40h R/W Accelerometer Mode (bit0=AccelerometerOn, bit1=PedometerOn) ;\
 41h R/W Accelerometer Index for Manual I2C Read via MCU[44h] ;
 42h R/W Unused (0) ;
 43h R/W Accelerometer Index for Manual I2C Write via MCU[44h] ;
 44h R/W Accelerometer Data from/to Read/Write via MCU[41h/43h] ;
 45h,46h R Accelerometer Output X (lsb,msb) ;resting=+/-00xxh ;
 47h,48h R Accelerometer Output Y (lsb,msb) ;resting=+/-00xxh ;
 49h,4Ah R Accelerometer Output Z (lsb,msb) ;resting=-41xxh (gravity) ;
 4Bh R/W Pedometer Step Count, bit0-7 ;\(for the current day) ;
 4Ch R/W Pedometer Step Count, bit8-15 ; (uh, how/which day?) ;
 4Dh R/W Pedometer Step Count, bit16-23 ;/(rather total count?) ;
 4Eh R/W Pedometer Flags (Wr.bit0=ClearArray?, Rd.bit4=ArrayFull?) ;
 4Fh * R Pedometer Timestamp[6] and StepCounts[2*A8h] ;ARRAY[6+2*A8h] ;
 50h,51h R/W Pedometer Minute,Second compare values (?) ;/
 52h-57h R/W Unused, except some bytes are set to fixed values once and then
 58h R/W Volume slider calibration point for 0% (default=36h)
 59h R/W Volume slider calibration point for 100% (default=C9h)
 5Ah R/RW Invalid, do not use!
 on newer MCU_FIRM versions this is set to FFh once and then
 on older MCU_FIRM versions this is a read-only counter
 5Bh-5Fh - N/A (write=ignored, read=FFh)
 60h * R/W Battery-backed RAM Index ;\
 61h * R/W Battery-backed RAM Data (200 bytes) ;ARRAY[C8h] ; Misc
 62h-7Eh - N/A (write=ignored, read=FFh) ;
 7Fh * R Various system state information ;ARRAY[09h/13h] ;/
 80h-FFh - N/A (write=ignored, read=FFh) |
| --- |

**Blurb**

On old versions of MCU_FIRM none of the invalid registers are masked away by
the read handler function, but are still read-only. Newer MCU_FIRM versions
return the hardcoded value FF instead.

Reportedly register 3Bh (RTC Alarm month) "could be used on very old MCU_FIRM
versions to upload MCU firmware if some conditions (?) are met", uh?.