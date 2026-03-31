# I2Cmcu00H01H05H07Hfirmware

> Source: https://problemkaputt.de/gbatek.htm
> Section: I2Cmcu00H01H05H07Hfirmware

3DS I2C MCU[00h-01h,05h-07h] - Firmware 
| |
| --- |

 **MCU[00h] - Version high (bit0-3) and hardware.type? (bit4-7) (R)**
 **MCU[01h] - Version low (8bit) (R)**
 
```

| OldMCU: Version 0.008 or lower ;\differs on LCD bits:
 NewMCU: Version 0.009 or higher ;/backlight and power |
| --- |

**MCU[05h-07h] - MCU Firmware update unlock sequence ;ARRAY[4003h] (R/W)**

Updating MCU Firmware FLASH is done by writing 4003h bytes to MCU[05h and up].
The I2C transfer timings (and flash write timings) are automatically handled
via SCL-hold. The upload takes about 2 seconds, and the console does then
reboot the MCU... the backlights stay on, but I2C MCU access seems to be
non-functional... for a while?

The first three bytes must be 6Ah,68h,6Ch (aka "jhl"), if the 3rd write does
match (and 1st-2nd write were also correct), then the MCU switches to receiving
a 4000h-byte firmware image. The image consists of two blocks:

```

| 1000h bytes written to flash address 0000h-0FFFh (4K)
 3000h bytes written to flash address 2000h-4FFFh (12K) |
| --- |

For error checking, the image must contain three identical 9-byte
"HH:MM:SS",00h timestamps:

```

| 0FF6h..0FFEh - 1st timestamp
 2000h..2008h - 2nd timestamp
 4FF6h..4FFEh - 3rd timestamp |
| --- |

The first byte of the timestamp must be other than FFh, and the third byte must
be ":".

BUG: The 1st/3rd timestamp are checked before applying the firmware update, but
2nd timestamp isn't checked until AFTER applying it (then causing blinking red
power led, and requiring the reflash via UART pin).

3DS I2C MCU - RL78 Flash Programming via UART

Uploading bugged code may also require reflashing via UART (the UART stuff is
handled in ROM, and works safely even if the whole flash is erased).

**Firmware Inner Workings**

The MCU contains a RL78 processor with reflashable firmware.

3DS I2C MCU - RL78 CPU Opcode List

3DS I2C MCU - RL78 CPU Opcode Map

3DS I2C MCU - RL78 CPU Registers and Flags

3DS I2C MCU - RL78 SFR Registers (Special Function Registers) (I/O ports)

3DS I2C MCU - RL78 Misc

**Firmware Nocash MCU Patches**

Code Execution in Battery RAM:

```

| MCU[05h-07h] = "exc" --> Battery RAM code gets executed immediately
 MCU[05h-07h] = "exq" --> Battery RAM code gets enqueued as callback
 Before execution, use MCU[61h] to upload code to Battery RAM, the RAM
 can be also used to store parameters and return values, you may want to
 restore the original RAM content after execution for not confusing the OS.
 The ROM/RAM memory map varies for different MCU firmware versions, however,
 the patched firmware provides useful ROM/RAM addresses in below tables,
 code should either use relative jumps, or addresses from those tables. |
| --- |

Call Table Vectors for ROM functions:

```

| 00080h Process Standard Callbacks ;\
 00082h Add Callback AX ; Nintendo's own stuff (not useful)
 00084h Process Enqueued Callbacks ;/
 000B4h I2C_Receive_Block ;\Slave I2C recv/send (can be used
 000B6h I2C_Send_Block ;/from within callbacks only)
 000B8h FLASH Init and kill IRQs ;\
 000BAh FLASH Erase 400h bytes ; FLASH init/erase/write/finish
 000BCh FLASH Write max 100h bytes ; (eg. for custom code in backup area)
 000BEh FLASH Finish 400h bytes ;/ |
| --- |

Pointers to RAM arrays:

```

| F4FE8h MCU[00h..5Ah] Registers (5Bh bytes) ;\
 F4FEAh MCU[61h] Battery RAM (C8h bytes) ; Pointers to RAM arrays
 F4FECh MCU[2Dh] Notify LED RAM (64h bytes) ;
 F4FEEh MCU[4Fh] Pedometer_array (150h bytes) ;/ |
| --- |

Other General Patches:

```

| Faster power button tap duration (0s instead laggy delay)
 Faster power button hold duration (1s instead 3s)
 Changed Power LED color upon holding power button (purple)
 Faster shutdown after holding power button (can be 0s instead 12s)
 Faster shutdown after normal power down (omitting LED fade-out delay) |
| --- |

**Firmware Image/Dumping**

A copy of the firmware-image can be found in the System Modules folder,

```

| 3ds:\title\00040130\00001f02\content\000000vv.app ;MCU
 3ds:\title\00040130\20001f02\content\000000vv.app ;MCU New3DS
 3ds:\title\00040130\00001f03\content\000000vv.app ;MCU Safe mode
 3ds:\title\00040130\20001f03\content\000000vv.app ;MCU Safe mode New3DS |
| --- |

The .app file contains a .code file with ARM11 code (and the MCU firmware
somewhere inside of that .code file, usually at file offset A078h or A07Ch;
starting with the 3-byte unlock code ("jhl"), followed by the 4000h-byte
firmware image).

Dumping the firmware directly from FLASH memory isn't supported. However, one
could patch the firmware, and then dump the backup of the old version (that
will destroy the backup of the yet older version though).