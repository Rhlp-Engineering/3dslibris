# I2Cmcurl78Flashprogrammingviauart

> Source: https://problemkaputt.de/gbatek.htm
> Section: I2Cmcurl78Flashprogrammingviauart

3DS I2C MCU - RL78 Flash Programming via UART 
| |
| --- |

 Both DSi and 3DS firmwares can be updated by software from ARM side. If that goes wrong, one can reprogram the chip via UART cable.
 
 **Setting Flash Programming Mode (78K0R) (3DS)**
 
```

| VDD __/"""""""""""""""""""""""""""""
 /RESET ______/"""""""""""""""""""""""""
 FLMD0 xx__/""""""""""""""""""""""""""" (high=flash mode)
 TOOL0 xx__/"""""""\_/"""""\_/"\_/""""" (merged uart rx/tx on one pin) |
| --- |

The 3DS has the above pins wired to the test points covered by green solder
stop layer.

**Setting Flash Programming Mode (78K0) (DSo)**

The 78K0 suppors three different programming modes:

```

| FLMD0 constant high --> UART mode with EXTERNAL clock on X1,X2 pins
 FLMD0 with three low pulses --> UART mode with EXTERNAL clock on EXCLK pin
 FLMD0 with eight low pulses --> SPI mode with INTERNAL clock |
| --- |

All three modes do somewhat conflict with the wiring of the DSi mainboard
(PMOFF on the external clock pin, and secondary I2C bus chips on the SPI pins).
However, at least one mode is actually working:

```

| DSi 78K0 Signal
 VDD33 VDD __/"""""""""""""""""""""""""""""""" power supply
 /mRST /RESET """"\_/"""""""""""""""""""""""""""" flash mode reset
 mFE FLMD0 __/"""""|"|"|"""""""""""""""""""""" flash mode select pulses
 VOLP P14/RX """"""""""""""""\_/"\_/"XXX"""""""" baudrate detect & cmd's
 VOLN P13/TX """"'---------'""""""""""""""XXX""" reply packets
 PMOFF EXCLK __/!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! shortcut to 8MHz SND_MCLK |
| --- |

Whereas, PMOFF is usually LOW, which causes SND_MCLK to drop from 3.3V to 1V
when shortcutting the two pins, which is far below the official EXCLK voltage
limits, but regardless, it's working just fine!

**Old3DS Mainboard (near headphone socket, all 6 pins under solderstop)**

```

| 1.78V 0.00V 1.76V
 PVDD18 FLMD0 /RESET
 TP79 TP75 TP74 GND'
 _ | _ _ _|_
 (_)' (_)- (_)- | |
 _ _ _ |EM7|
 -(_) (_)- (_)- |_ _|
 TP78 TP76 TP77 |
 GND TOOL1 TOOL0 GND
 0.00V 1.76V 1.76V |
| --- |

**Old3DS XL Mainboard (near wifi daughterboard, all 6 pins under solderstop)**

```

| [X]
 TP78
 GND
 _ _
 (_) (_)
 TP76 TP75
 TOOL1 FLMD0
 _ _ _
 (_) (_) (_)
 TP79 TP77 TP74
 PVDD18 TOOL0 /RES |
| --- |

**Old2DS Mainboard (near front/middle, all 6 pins under solderstop)**

```

| _ _ _ _ _||||_
 (_) (_) (_) (_) | P14 | _
 TP77 TP75 TP74 TP76 | .... | (_) (X)
 TOOL0 FLMD0 /RES TOOL1 |______| TP79 TP78
 PVDD18 GND |
| --- |

**New3DS-XL Mainboard (near headphone socket, right 4 pins under solderstop)**

```

| _ _
 _ _ _ TOOL0(_) (_)FLMD0
 (_) (_) (_) _ x _
 ____ _ _ TOOL1(_) (_)/RESET
 (____| (_) (_)
 VDD18 PVDD18 GND |
| --- |

**New3DS**

**New2DS**

Unknown.

**Wiring Programmer (PC Parallel Port, GPIO, UART, whatever) to 3DS MCU**

```

| TP74 /RESET ----------------|>|-------------- PC.Data1.Reset
 TP77 TOOL0 ------------o---|>|-------------- PC.Data0.TX
 | BAT85 .------- PC.Busy.RX
 TP75 FLMD0 -----. | |/ C
 | '--[33K]---| B
 TP79 PVDD18 -----' |\ E
 BC547 v--.
 TP78 GND ----------------------------o---- PC.Ground |
| --- |

The 3DS MCU programming signals are 1.8V, two BAT85 Schottky diodes can be used
as one-directional level changer (with replies ignored/skipped via hardcoded
delays), an additional 10K-65K resistor and BC547 transistor can be used for
bi-directional transfers.

**Command Summary**

```

| 00h Reset (aka enter flash mode)
 13h Verify/compare
 14h --Undocumented-- verify/compare first 4 bytes of flash, if [000C3h].bit1
 19h --Undocumented-- resembles "internal verify" from write command
 20h Chip Erase
 22h Block Erase
 32h Block Blank check
 40h Write
 90h Set Oscillation Frequency (78K0 only) (DSi)
 9Ah Set Baud Rate (78K0R only) (3DS)
 9Eh Set Custom Flash Timings (78K0 only) (DSi) (undoc)
 A0h Set Security
 A4h Get 1Kbyte Security info? (78K0 only) (DSi) (undoc)
 B0h Get Checksum
 C0h Get Silicon Signature
 C5h Get Version |
| --- |

The relevant commands are Reset, Block Erase, and Write.

**Response Status/Error Codes**

```

| 04h Command Number Error
 05h Parameter Error
 06h Normal Acknowledge (ACK) (okay)
 07h Checksum Error
 0Fh Verify Error
 10h Write Protected Error
 15h Negative Acknowledge (NACK)
 1Ah MRG10 Error (mrg, uh?) ;78K0: Erase verify error
 1Bh MRG11 Error (mrg, uh?) ;78K0: Write internal verify or blank check error
 1Ch Write Error
 20h Security read error (78K0 only, not 78K0R)
 FFh N/A (for 2nd status byte, when 1st status byte was bad) aka BUSY |
| --- |

**Command Frames (Command to chip)**

```

| 00h 1 Command Start (fixed=01h)
 01h 1 Length (LEN) (1..255, or 00h=256)
 02h 1 Command Number
 03h LEN-1 Command Parameters (if any)
 xxh 1 Checksum (01h minus all of the above bytes)
 xxh 1 Command End (fixed=03h) |
| --- |

**Data Frames (Data to chip, and Data/Status from chip)**

```

| 00h 1 Data Start (fixed=02h)
 01h 1 Length (LEN) (1..255, or 00h=256)
 02h LEN Data
 xxh 1 Checksum (02h minus all of the above bytes)
 xxh 1 Data End (03h=Last Data Frame, or 17h=More data frames follow) |
| --- |

**Reset Command (aka Start/Sync)**

Drag FLDM0 high (1.8V in 3DS) to enable programming mode, and drag /RESET low
for a moment. Then exchange the following bytes/packets.

Low Pulse (from chip, should arrive within 10ms after releasing /RESET):

```

| On 78K0R (3DS):
 00h 1 Low pulse (value 00h) ;-transferred at 9600 bps
 On 78K0 (DSi):
 The 78K0 doesn't specifically output a leading low-pulse on TX pin
 (however, it has the TX pin floating for about 35 ms while waiting
 for SPI/UART mode selection via FLMD0 pulses) (accordingly, one
 should wait about 60ms before trying to send below stuff to RX pin). |
| --- |

Low Pulses (to chip):

```

| 00h 1 Low pulse (value 00h) ;\chip autodetects baudrate based on
 01h 1 Low pulse (value 00h) ;/transfer time for these two bytes |
| --- |

Command (to chip):

```

| 00h 3 Start/Len/Command (01h,01h,00h)
 03h 2 Checksum/End (CHK,03h) |
| --- |

Response (from chip):

```

| 00h 2 Start/Len (02h,01h)
 02h 1 Status/Error code
 03h 2 Checksum/End (CHK,03h) |
| --- |

**Set Oscillation Frequency (78K0 only) (DSi)**

Allows to set the baudrate to 115200 bauds (in an absurd BCD-based way based on
the external oscillation clock input), which isn't really needed because the
baudrate is autodetected when sending the Reset pulses.

Command (to chip):

```

| 00h 3 Start/Len/Command (01h,05h,90h)
 03h 1 Oscillation Freq, 1st digit (BCD, 0..9) ;\Example:
 04h 1 Oscillation Freq, 2nd digit (BCD, 0..9) ; 1,2,3,4 is
 05h 1 Oscillation Freq, 3rd digit (BCD, 0..9) ; 1230000 Hz
 06h 1 Oscillation Freq, Num trailing 0-digits (BCD, 0..9) ;/
 07h 2 Checksum/End (CHK,03h) |
| --- |

Response (from chip):

```

| 00h 2 Start/Len (02h,01h)
 02h 1 Status/Error code
 03h 2 Checksum/End (CHK,03h) |
| --- |

**Set Baudrate Command (78K0R only) (3DS)**

Allows to set the baudrate, which isn't really needed because the baudrate is
autodetected when sending the Reset pulses.

Recommended might be 57.6Kbps, higher rates aren't really useful because of the
slow write duration. Tested/working rates are 9.6Kbps through 115.2Kbps,
including odd nonstandard rates.

Bytes should be sent as 8N2 (8 databits, no parity, 2 stopbits). However, the
chip seems to send only 1 stopbit. Nethertheless, it seems to insist on
receiving 2 stopbits (alternately, one could send the 2nd stopbit before
startbit).

Command (to chip):

```

| 00h 3 Start/Len/Command (01h,06h,9Ah)
 03h 1 Sync/Connection Mode (00h=Microcontroller, 1=Programmer)
 04h 2 Baud rate, in whatever units
 06h 1 Noise filter (00h=Off, 01h=On)
 07h 1 Speed/voltage (00h=Fast 2.7V and up, 01h=Slow 1.8V and up)
 08h 2 Checksum/End (CHK,03h) |
| --- |

Response (from chip):

```

| 00h 2 Start/Len (02h,01h)
 02h 1 Status/Error code
 03h 2 Checksum/End (CHK,03h) |
| --- |

**Chip Erase Command**

Erases the whole flash memory & security settings (unless the security
settings did have disabled the Chip Erase command itself).

Command (to chip):

```

| 00h 3 Start/Len/Command (01h,01h,20h)
 03h 2 Checksum/End (CHK,03h) |
| --- |

Response (from chip):

```

| 00h 2 Start/Len (02h,01h)
 02h 1 Status/Error code (chip erase result)
 03h 2 Checksum/End (CHK,03h) |
| --- |

**Block Erase Command**

Blocks seem to be 1Kbyte (400h bytes). Erasing 1000h bytes at once does also
work.

Command (to chip):

```

| 00h 3 Start/Len/Command (01h,07h,22h)
 03h 3 Start address (MSB,MID,LSB) ;lower 10bit=000h ;\400h byte boundary
 06h 3 End address (MSB,MID,LSB) ;lower 10bit=3FFh ;/
 09h 2 Checksum/End (CHK,03h) |
| --- |

Response (from chip):

```

| 00h 2 Start/Len (02h,01h)
 02h 1 Status/Error code (block erase result)
 03h 2 Checksum/End (CHK,03h) |
| --- |

**Block Blank Check Command**

Command (to chip):

```

| 00h 3 Start/Len/Command (01h,08h,32h)
 03h 3 Start address (MSB,MID,LSB) ;lower 10bit=000h ;\400h byte boundary
 06h 3 End address (MSB,MID,LSB) ;lower 10bit=3FFh ;/
 09h 1 Type (00h=Before single block, 01h=Before chip erase)
 0Ah 2 Checksum/End (CHK,03h) |
| --- |

Response (from chip):

```

| 00h 2 Start/Len (02h,01h)
 02h 1 Status/Error code (block blank check result)
 03h 2 Checksum/End (CHK,03h) |
| --- |

**Write Command**

Requires prior Erase command, max length for erase/write commands seems to be
1000h bytes. To quit flash mode & resume normal operation, it seems to be
required to unplug battery supply for a moment after having written all data.

Command (to chip):

```

| 00h 3 Start/Len/Command (01h,07h,40h)
 03h 3 Start address (MSB,MID,LSB) ;lower 8bit=00h ;\100h byte boundary
 06h 3 End address (MSB,MID,LSB) ;lower 8bit=FFh ;/
 09h 2 Checksum/End (CHK,03h) |
| --- |

Response (from chip, after command):

```

| 00h 2 Start/Len (02h,01h)
 02h 1 Status/Error code (command reception result)
 03h 2 Checksum/End (CHK,03h) |
| --- |

Data (to chip):

```

| 00h 2 Start/Len (02h,LEN) ;LEN=1..255 bytes, or 0=256 bytes
 02h LEN Data (usually 256 bytes)
 xxh 2 Checksum/End (CHK,03h/17h) ;03h=Last, 17h=Nonlast |
| --- |

Response (from chip, circa 60ms after each data block):

```

| 00h 2 Start/Len (02h,02h)
 02h 1 Status/Error code (data reception result)
 02h 1 Status/Error code (write result)
 04h 2 Checksum/End (CHK,03h) |
| --- |

Response (from chip, extra response, about 1.5 seconds after LAST data block):

```

| 00h 2 Start/Len (02h,01h)
 02h 1 Status/Error code (internal verify result)
 03h 2 Checksum/End (CHK,03h) |
| --- |

Note: The ugly 1.5 seconds verify delay can be avoided by issuing a chip reset
instead of waiting for the final response.

**Verify/Compare Command**

Command (to chip):

```

| 00h 3 Start/Len/Command (01h,07h,13h)
 03h 3 Start address (MSB,MID,LSB) ;lower 8bit=00h ;\100h byte boundary
 06h 3 End address (MSB,MID,LSB) ;lower 8bit=FFh ;/
 09h 2 Checksum/End (CHK,03h) |
| --- |

Response (from chip, after command):

```

| 00h 2 Start/Len (02h,01h)
 02h 1 Status/Error code (command reception result)
 03h 2 Checksum/End (CHK,03h) |
| --- |

Data (to chip):

```

| 00h 2 Start/Len (02h,LEN) ;LEN must be N*4 bytes (usually (1)00h bytes)
 02h LEN Data
 xxh 2 Checksum/End (CHK,03h/17h) ;03h=Last, 17h=Nonlast |
| --- |

Response (from chip, after each data block):

```

| 00h 2 Start/Len (02h,02h)
 02h 1 Status/Error code (data reception result)
 02h 1 Status/Error code (verify result) (always ACK/okay for Nonlast!)
 04h 2 Checksum/End (CHK,03h) |
| --- |

With data LEN=4, this seems to allow dump the chip content via brute-force (but
requires to send/receive trillions of bits to dump 4 bytes, and gets slower
towards end of 100h-byte snippets).

**Get Checksum Command**

Command (to chip):

```

| 00h 3 Start/Len/Command (01h,07h,B0h)
 03h 3 Start address (MSB,MID,LSB) ;lower 8bit=00h ;\100h byte boundary
 06h 3 End address (MSB,MID,LSB) ;lower 8bit=FFh ;/blah: "from top 1KB"
 09h 2 Checksum/End (CHK,03h) |
| --- |

Response (from chip, after command):

```

| 00h 2 Start/Len (02h,01h)
 02h 1 Status/Error code (command reception result)
 03h 2 Checksum/End (CHK,03h) |
| --- |

Data (from chip, after above response):

```

| 00h 2 Start/Len (02h,02h)
 02h 2 Data checksum, 0000h minus all data bytes (MSB,LSB)
 04h 2 Checksum/End (CHK,03h) |
| --- |

**Set Security Command**

Allows to write protect flash areas. Don't touch (or use with care), some
settings cannot be undone (not even via Chip Erase command).

Command (to chip):

```

| 00h 3 Start/Len/Command (01h,03h,A0h)
 03h 2 Fixed (00h,00h)
 08h 2 Checksum/End (CHK,03h) |
| --- |

Response (from chip):

```

| 00h 2 Start/Len (02h,01h)
 02h 1 Status/Error code (command reception result)
 03h 2 Checksum/End (CHK,03h) |
| --- |

Data (to chip, on 3DS with 78K0R):

```

| 00h 2 Start/Len (02h,06h) ;uh, len=6 (but below should have len=8)?
 02h 1 FLG Security Flags (can disable write/erase etc.)
 03h 1 BOT Boot cluster last block number ("fixed to 03h")
 04h 2 FSWS Flash shield window start ("Higher bits, Lower bits")
 06h 2 FSWE Flash shield window end ("Higher bits, Lower bits")
 08h 2 FFH Fixed (FFh,FFh)
 0Ah 2 Checksum/End (CHK,03h) |
| --- |

Data (to chip, on DSi with 78K0):

```

| 00h 2 Start/Len (02h,02h)
 02h 1 FLG Security Flags (can disable write/erase etc.)
 03h 1 BOT Boot cluster last block number ("fixed to 03h")
 0Ah 2 Checksum/End (CHK,03h) |
| --- |

Response (from chip):

```

| 00h 2 Start/Len (02h,01h)
 02h 1 Status/Error code (security write result)
 03h 2 Checksum/End (CHK,03h) |
| --- |

Response (from chip):

```

| 00h 2 Start/Len (02h,01h)
 02h 1 Status/Error code (internal verify result)
 03h 2 Checksum/End (CHK,03h) |
| --- |

**Get Silicon Signature Command**

Command (to chip):

```

| 00h 3 Start/Len/Command (01h,01h,C0h)
 03h 2 Checksum/End (CHK,03h) |
| --- |

Response (from chip, after command):

```

| 00h 2 Start/Len (02h,01h)
 02h 1 Status/Error code (command reception result)
 03h 2 Checksum/End (CHK,03h) |
| --- |

Data (from chip, after above response, on 3DS with 78K0R):

```

| 00h 2 Start/Len (02h,1Bh)
 02h 1 VEN Vendor (10h=NEC/Renseas) ;bit7=parity
 03h 1 MET Macro extension code (EFh) ;bit7=parity
 04h 1 MSC Macro function code (04h) ;bit7=parity
 05h 3 DET Device extension code 1,2,3 (DCh,FDh,xxh) ;bit7=parity
 08h 3 UAE User flash size-1 (LSB,MID,MSB) (007FFFh, little-endian!)
 0Bh 10 DEV Device name, ASCII ("D79F0104 ")
 15h 1 SCF Security flag information ;\same as
 16h 1 BOT Boot block number ("03h=fixed") ; from
 17h 2 FSWS Flash shield window start("Higher side, Lower side"); security
 19h 2 FSWE Flash shield window end ("Higher side, Lower side"); command?
 1Bh 2 RES Reserved (FFFFh) ;/
 1Dh 2 Checksum/End (CHK,03h) |
| --- |

Data (from chip, after above response, on DSi with 78K0):

```

| 00h 2 Start/Len (02h,13h)
 02h 1 VEN Vendor (10h=NEC/Renseas) ;bit7=parity
 03h 1 MET Macro extension code (7Fh=78K0/Kx2) ;bit7=parity
 04h 1 MSC Macro function code (04h=78K0/Kx2) ;bit7=parity
 05h 1 DEC Device extension code 1 (7Ch=78K0/Kx2) ;bit7=parity
 06h 1 END Flash Memory End, bit0-6 (7Fh) ;\ ;bit7=parity
 07h 1 END Flash Memory End, bit7-13 (7Fh) ; =3FFFh ;bit7=parity
 08h 1 END Flash Memory End, bit14-20 (80h) ;/ ;bit7=parity
 09h 10 DEV Device name, ASCII with parity ("D78F0501A ") ;bit7=parity
 13h 1 SCF Security flag information (7Fh=whatever)
 14h 1 BOT Boot block number (03h=fixed)
 15h 2 Checksum/End (CHK,03h) |
| --- |

**Get Version Command**

Command (to chip):

```

| 00h 3 Start/Len/Command (01h,01h,C5h)
 03h 2 Checksum/End (CHK,03h) |
| --- |

Response (from chip, after command):

```

| 00h 2 Start/Len (02h,01h)
 02h 1 Status/Error code (command reception result)
 03h 2 Checksum/End (CHK,03h) |
| --- |

Data (from chip, after above response):

```

| 00h 2 Start/Len (02h,06h)
 02h 3 Device Version (MSB,MID,LSB) (3DS: 000000h ?) (DSi:000000h)
 05h 3 Firmware Version (MSB,MID,LSB) (3DS: 010000h ?) (DSi:020100h)
 08h 2 Checksum/End (CHK,03h) |
| --- |

XXX test Get Version on 3DS

**Notes**

The above protocol is documented in the "78K0R/Kx3-L Flash Memory Programming"
application note.

Unknown if verify/checksum could be tweaked to dump the current flash content.
There is also an undocumented anti-dumping feature related to bytes at 000C4h
(that's probably related to TOOL1 debug pin, not to TOOL0 serial flash pin).