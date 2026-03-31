# Dsitouchscreensoundcontroller

> Source: https://problemkaputt.de/gbatek.htm
> Section: Dsitouchscreensoundcontroller

DSi Touchscreen/Sound Controller 
| |
| --- |

 DSi Touchscreen Access
 DSi Touchscreen/Sound Init Flowcharts
 
 **AIC3000D Registers**
 DSi TSC, Register Summary
 DSi TSC[0:00h..1Ah], Basic PLL and Timing Control
 DSi TSC[0:1Bh..23h], Codec Control
 DSi TSC[0:24h..32h], Status and Interrupt Flags
 DSi TSC[0:33h..3Bh], Pin Control
 DSi TSC[0:3Ch..55h], DAC/ADC and Beep
 DSi TSC[0:56h..7Fh], AGC and ADC
 DSi TSC[1:xxh], DAC and ADC Routing, PGA, Power-Controls and MISC Logic
 DSi TSC[3:xxh], Touchscreen/SAR Control and TSC[FCh:xxh], Buffer
 DSi TSC[04h..05h:xxh], ADC Digital Filter Coefficient RAM
 DSi TSC[08h..0Fh:xxh], DAC Digital Filter Coefficient RAM
 DSi TSC[20h..2Bh:xxh], TSC[40h..5Fh:xxh] ADC/DAC Instruction RAM
 
 
 
| DSi Touchscreen Access |
| --- |

 The Touch Screen Controller (for lower LCD screen) is accessed via SPI bus,
 DS Serial Peripheral Interface Bus (SPI)
 so far, it's same as on NDS, but the SPI touchscreen commands are having an entirely different format in DSi mode:
 The DSi touchscreen registers are selected via a combination of a PAGE byte and an INDEX byte. The PAGE byte is located at INDEX=00h, and it does somewhat 'bankswitch' the contents of INDEX=01h..7Fh. And INDEX can be incremented manually, or automatically (but, confusingly, the manual increment doesn't work for reading Y coordinates).
 SPI clock should be set to 4MHz for DSi Mode touchscreen access (unlike NDS, which used 2MHz). The PENIRQ bit in port 4000136h is always zero in DSi mode.
 When reading data: Write dummy 00h-bytes in output direction.
 
 AIC3000D
 
 **DSi Touchscreen PAGE:INDEX values**
 The INDEX/Direction byte is written as first byte after SPI chip select:
 
```

| 0 Direction for following data bytes (0=Write, 1=Read)
 1-7 INDEX (00h..7Fh) for following data bytes (auto-increasing) |
| --- |

However, the chip has more than 128 registers, stored in multiple pages.

```

| TSC[00h]=PAGE ;<-- change page (at INDEX=0)
 TSC[PAGE:INDEX] ;<-- access registers in select page |
| --- |

**Pen Down Testing**

```

| if (TSC[3:09h] AND 40h)<>0 then return(not_pressed) ;ADC Ready Flag
 if (TSC[3:0Eh] AND 03h)<>0 then return(not_pressed) ;Undocumented Flags?
 return(pressed) |
| --- |

Note: On NDS, this would be done by reading port 4000136h.bit6, which isn't
supported in DSi mode.

**X/Y Coordinate Reading**

```

| touchdata[0..19] = TSC[FCh:01h..14h] ;read page FCh, index(1..20)
 rawx=0, rawy=0
 for i=0 to 8 step 2
 x = touchdata[i+0]*100h+touchdata[i+1]
 y = touchdata[i+10]*100h+touchdata[i+11]
 if (x or y) and F000h then return(not_pressed)
 rawx=rawx+x, rawy=rawy+y
 return(rawx/5, rawy/5) |
| --- |

The resulting 12bit coordinates are same as on NDS (ie. they need to be further
processed using the Calibration Points from User Settings).

Reading TSC[FCh:01h..14h] must be done in a single SPI transfer (reading them
separatedly as TSC[FCh:01h], TSC[FCh:02h], etc. doesn't work).

**Touchscreen X/Y Coordinates**

```

| 0-11 Coordinate (0..FFFh) (usually 000h when not pressed)
 12-14 State (0=Pressed, 7=Released) (or sometimes also 1 or 3=Released)
 15 State Changed (0=No, 1=Newly pressed/released; cleared after read) |
| --- |

Bit12-14 are usually set to 7 when releasing the screen (though sometimes they
become 1 or 3 when releasing the screen, and do stay so until newly pressing
it).

Bit15 is cleared after reading (so it will be usually seen only in the first
MSB, ie. at INDEX=01h) (though maybe it can also occur elsewhere if it becomes
newly set during the SPI transfer).

**Microphone**

The microphone input was part of the TSC on NDS. In DSi mode it is reportedly
somehow changed, using a new "CODEC" (aka the DSi's newer TSC touchscreen/sound
controller).

**NDS Backwards Compatibility Mode**

The DSi hardware can emulate the NDS-style touchscreen protocol (with X/Y/MIC
channels and with additional PENIRQ flag; but without Pressure or Temperature
channels).

DS Touch Screen Controller (TSC)

DSi enhanced games should implement both modes, and use the new mode on DSi
consoles, and the old mode on NDS consoles (alternately, CartHdr[1BFh].bit0=0
will activate the old mode for both NDS+DSi).

If the backwards compatibility mode isn't enabled, then trying to read the
touchscreen in NDS fashion will return nothing but zeroes for all TSC channels
(and also zero for the PENIRQ bit).

| DSi Touchscreen/Sound Init Flowcharts |
| --- |

**touch_enable_tsc_init_list:**

```

| TSC[3:0Eh]=00h ;Undoc (RMW: bit7=0)
 TSC[3:02h]=18h ;SAR ADC clk divider, Div8 (12bit mode) (RMW bit34=3, bit7=?)
 TSC[3:0Fh]=A0h ;Scan Mode Timer
 TSC[3:0Eh].28h ;Undoc (RNW: bit345=5)
 TSC[3:0Eh].28h ;Undoc (RMW: bit6=0)
 TSC[3:03h]=87h ;SAR ADC Control 2 (SelfByPenDown, ScanXY, /PENIRQ)
 TSC[3:05h].04h ;Stabilization time = 30us (RMW bit012=4)
 TSC[3:04h].02h ;Sense time = 3us (RMW bit012=2)
 TSC[3:04h].22h ;Precharge time = 3us (RMW bit456=2)
 TSC[3:12h].00h ;Debounce Time = 0us (RMW bit012=0)
 TSC[3:0Eh].A8h ;Undoc (RMW bit7=1) |
| --- |

**mic_enable_tsc_init_list:**

This is needed to unmute the microphone (otherwise microphone will return only
0000h or FFFFh.

```

| TSC[1:2Eh]=03h ;MICBIAS=AVDD
 TSC[0:51h]=80h ;ADC Digital Mic, on
 TSC[0:52h]=00h ;ADC Digital Volume Control Fine Adjust, unmute
 TSC[1:2Fh]=37h ;MIC PGA=27.5dB (or use other value, if desired) |
| --- |

**mic_disable_tsc_init_list:**

```

| TSC[0:52h]=80h ;ADC Digital Volume Control Fine Adjust, mute
 TSC[0:51h]=00h ;ADC Digital Mic, off
 TSC[1:2Eh]=00h ;MICBIAS=Off |
| --- |

**basic_tsc_init_list:**

This is done by DSi Launcher/Unlaunch (and other titles usually won't need to
do this).

```

| TSC[0:01h]=01h ;Software Reset
 TSC[0:39h]=66h ;ADC DC Measurement 1
 TSC[1:20h]=16h ;Class-D Speaker Amplifier (RMW:bit4=1)
 TSC[0:04h]=00h ;Clock-Gen Muxing
 TSC[0:12h]=81h ;ADC NADC Value
 TSC[0:13h]=82h ;ADC MADC Value
 TSC[0:51h]=82h ;ADC Digital Mic
 TSC[0:51h]=00h ;ADC Digital Mic again
 TSC[0:04h]=03h ;Clock-Gen Muxing
 TSC[0:05h]=A1h ;PLL P and R-Values
 TSC[0:06h]=15h ;PLL J-Value
 TSC[0:0Bh]=87h ;DAC NDAC Value
 TSC[0:0Ch]=83h ;DAC MDAC Value
 TSC[0:12h]=87h ;ADC NADC Value
 TSC[0:13h]=83h ;ADC MADC Value
 TSC[3:10h]=88h ;Scan Mode Timer Clock (RMW:bit0-6)
 TSC[4:08h..0Dh]=7Fh,E1h,80h,1Fh,7Fh,C1h ;some coeff's
 TSC[0:41h]=08h ;DAC Left Volume Control
 TSC[0:42h]=08h ;DAC Right Volume Control
 TSC[0:3Ah]=00h ;GPI3 Pin Control
 TSC[4:08h..0Dh]=7Fh,E1h,80h,1Fh,7Fh,C1h ;some coeff's ;again?
 TSC[1:2Fh]=2Bh ;MIC PGA
 TSC[1:30h]=40h ;P-Terminal Delta-Sigma Mono ADC Channel Fine-Gain Input
 TSC[1:31h]=40h ;M-Terminal ADC Input Selection
 TSC[1:32h]=60h ;Input CM Settings
 TSC[0:74h]=82h ;VOL/MICDET-Pin SAR ADC - Volume Control (RMW)
 TSC[0:74h]=92h ;VOL/MICDET-Pin SAR ADC - Volume Control (RMW)
 TSC[0:74h]=D2h ;VOL/MICDET-Pin SAR ADC - Volume Control (RMW)
 TSC[1:21h]=20h ;HP Output Drivers POP Removal Settings
 TSC[1:22h]=F0h ;Output Driver PGA Ramp-Down Period Control
 TSC[0:3Fh]=D4h ;DAC Data-Path Setup (RMW)
 TSC[1:23h]=44h ;DAC_L and DAC_R Output Mixer Routing
 TSC[1:1Fh]=D4h ;Headphone Drivers
 TSC[1:28h]=4Eh ;HPL Driver (Left Headphone)
 TSC[1:29h]=4Eh ;HPR Driver (Right Headphone)
 TSC[1:24h]=9Eh ;Analog Volume to HPL (Left Headphone)
 TSC[1:25h]=9Eh ;Analog Volume to HPR (Right Headphone)
 TSC[1:20h]=D4h ;Class-D Speaker Amplifier
 TSC[1:2Ah]=14h ;SPL Driver (Left Speaker)
 TSC[1:2Bh]=14h ;SPR Driver (Right Speaker)
 TSC[1:26h]=A7h ;Analog Volume to SPL (Left Speaker)
 TSC[1:27h]=A7h ;Analog Volume to SPR (Right Speaker)
 TSC[0:40h]=00h ;DAC Volume Control
 (should set DSi.GPIO.data.bit7 here, but can be also done elsewhere)
 TSC[0:3Ah]=60h ;GPI3 Pin Control |
| --- |

**nds_mode_tsc_init_list:**

Switches to NDS Touchscreen/sound mode. Usually done when starting NDS titles
(or DSi titles that request NDS-TSC mode in carthdr[1BFh]).

```

| TSC[1:26h]=ACh ;\special setting (when found special gamecode)
 TSC[1:27h]=ACh ;/
 TSC[1:26h]=A7h ;\normal setting (for any other gamecodes)
 TSC[1:27h]=A7h ;/
 TSC[1:2Eh]=03h ;MICBIAS=AVDD
 TSC[3:03h]=00h ;SAR ADC Control 2
 TSC[1:21h]=20h ;HP Output Drivers POP Removal Settings
 TSC[1:22h]=F0h ;Output Driver PGA Ramp-Down Period Control (70h OR 80h)
 TSC[1:22h]=70h ;Output Driver PGA Ramp-Down Period Control (bit7=0)
 TSC[0:52h]=80h ;ADC Digital Volume Control Fine Adjust
 TSC[0:51h]=00h ;ADC Digital Mic
 READ[3:02h] (returns 00h)
 TSC[3:02h].Bit7=1 ;SAR ADC Control 1 (set to 80h) (or 98h?) (RMW)
 TSC[FFh:05h]=00h ;TSC final enter NDS mode |
| --- |

And, set powerman[0].bit0=1 (bit0=sound amplifier on) (this is actually part of
TSC chip, accessed via "POWERMAN" SPI chipselect signal).

| DSi TSC, Register Summary |
| --- |

The DSi's Touchscreen/Sound controller (AIC3000D) is essentially a Texas
Instruments TSC2117 chip (possibly with some customizations for NDS backwards
compatibility mode).

TSC[page:index] registers are accessed via SPI bus with 15bit address space:

```

| 7bit index: selected via the first SPI byte, with direction flag in bit0
 8bit page: selected by writing to index 00h, ie. to TSC[xxh:00h] |
| --- |

**TSC page select (for "TSC[page:index]" addressing)**

```

| TSC[xxh:00h] - Page Select Register (00h) |
| --- |

**TSC Basic PLL and Timing Control**

```

| TSC[0:01h] - Software Reset (00h)
 TSC[0:02h] - Reserved (xxh) (R)
 TSC[0:03h] - Overtemperature OT Flag (02h..FFh) (R)
 TSC[0:04h] - Clock-Gen Muxing (00h)
 TSC[0:05h] - PLL P and R-Values (11h)
 TSC[0:06h] - PLL J-Value (04h)
 TSC[0:07h,08h] - PLL D-Value MSB,LSB (0000h)
 TSC[0:09h,0Ah] - Reserved (xxh)
 TSC[0:0Bh] - DAC NDAC Value (01h)
 TSC[0:0Ch] - DAC MDAC Value (01h)
 TSC[0:0Dh,0Eh] - DAC DOSR Value MSB,LSB (0080h)
 TSC[0:0Fh] - DAC IDAC Value (80h)
 TSC[0:10h] - DAC miniDSP Engine Interpolation (08h)
 TSC[0:11h] - Reserved (xxh)
 TSC[0:12h] - ADC NADC Value (01h)
 TSC[0:13h] - ADC MADC Value (01h)
 TSC[0:14h] - ADC AOSR Value (80h)
 TSC[0:15h] - ADC IADC Value (80h)
 TSC[0:16h] - ADC miniDSP Engine Decimation (04h)
 TSC[0:17h,18h] - Reserved (xxh)
 TSC[0:19h] - CLKOUT MUX (00h)
 TSC[0:1Ah] - CLKOUT Divider M Value (01h) |
| --- |

**TSC Codec Control**

```

| TSC[0:1Bh] - Codec Interface Control 1 (00h) (R/W)
 TSC[0:1Ch] - Data-Slot Offset Programmability (00h)
 TSC[0:1Dh] - Codec Interface Control 2 (00h)
 TSC[0:1Eh] - BCLK Divider N Value (01h)
 TSC[0:1Fh] - Codec Secondary Interface Control 1 (00h)
 TSC[0:20h] - Codec Secondary Interface Control 2 (00h)
 TSC[0:21h] - Codec Secondary Interface Control 3 (00h)
 TSC[0:22h] - I2C Bus Condition (00h)
 TSC[0:23h] - Reserved (xxh) |
| --- |

**TSC Status and Interrupt Flags**

```

| TSC[0:24h] - ADC Flag Register (0xh) (R)
 TSC[0:25h] - DAC Flag Register (00h) (R)
 TSC[0:26h] - DAC Flag Register (00h) (R)
 TSC[0:27h] - Overflow Flags (00h) (R)
 TSC[0:28h..2Bh] - Reserved (xxh)
 TSC[0:2Ch] - Interrupt Flags DAC, sticky (00h..30h) (R)
 TSC[0:2Dh] - Interrupt Flags ADC, sticky (00h..18h) (R)
 TSC[0:2Eh] - Interrupt Flags DAC, non-sticky? (00h..30h) (R)
 TSC[0:2Fh] - Interrupt Flags ADC, non-sticky? (00h..18h) (R)
 TSC[0:30h] - INT1 Control Register (Select INT1 Sources) (00h)
 TSC[0:31h] - INT2 Control Register (Select INT2 Sources) (00h)
 TSC[0:32h] - INT1 and INT2 Control Register (00h) |
| --- |

**TSC Pin Control**

```

| TSC[0:33h] - GPIO1 In/Out Pin Control (00h..C2h)
 TSC[0:34h] - GPIO2 In/Out Pin Control (00h..C2h)
 TSC[0:35h] - SDOUT (OUT Pin) Control (12h)
 TSC[0:36h] - SDIN (IN Pin) Control (02h or 03h)
 TSC[0:37h] - MISO (OUT Pin) Control (02h)
 TSC[0:38h] - SCLK (IN Pin) Control (02h..03h)
 TSC[0:39h] - GPI1 and GPI2 Pin Control (00h..11h)
 TSC[0:3Ah] - GPI3 Pin Control (00h..10h)
 TSC[0:3Bh] - Reserved (xxh) |
| --- |

**TSC DAC/ADC and Beep**

```

| TSC[0:3Ch] - DAC Instruction Set (01h)
 TSC[0:3Dh] - ADC Instruction Set (04h)
 TSC[0:3Eh] - Programmable Instruction Mode-Control Bits (00h)
 TSC[0:3Fh] - DAC Data-Path Setup (14h)
 TSC[0:40h] - DAC Volume Control (0Ch)
 TSC[0:41h] - DAC Left Volume Control (00h)
 TSC[0:42h] - DAC Right Volume Control (00h)
 TSC[0:43h] - Headset Detection (00h..60h)
 TSC[0:44h] - DRC Control 1 (0Fh)
 TSC[0:45h] - DRC Control 2 (38h)
 TSC[0:46h] - DRC Control 3 (00h)
 TSC[0:47h] - Beep Generator and Left Beep Volume (00h)
 TSC[0:48h] - Beep Generator and Right Beep Volume (00h)
 TSC[0:49h,4Ah,4Bh] - Beep Length MSB,MID,LSB (0000EEh)
 TSC[0:4Ch,4Dh] - Beep Frequency Sin(x) MSB,LSB (10D8h)
 TSC[0:4Eh,4Fh] - Beep Frequency Cos(x) MSB,LSB (7EE3h)
 TSC[0:50h] - Reserved (xxh)
 TSC[0:51h] - ADC Digital Mic (00h)
 TSC[0:52h] - ADC Digital Volume Control Fine Adjust (80h)
 TSC[0:53h] - ADC Digital Volume Control Coarse Adjust (00h)
 TSC[0:54h,55h] - Reserved (xxh) |
| --- |

**TSC AGC and ADC (Auto-Gain-Control & Analog-to-Digital Converter)**

```

| TSC[0:56h] - AGC Control 1 (00h)
 TSC[0:57h] - AGC Control 2 (00h)
 TSC[0:58h] - AGC Maximum Gain (7Fh, uh that's 7Fh=Reserved?)
 TSC[0:59h] - AGC Attack Time (00h)
 TSC[0:5Ah] - AGC Decay Time (00h)
 TSC[0:5Bh] - AGC Noise Debounce (00h)
 TSC[0:5Ch] - AGC Signal Debounce (00h)
 TSC[0:5Dh] - AGC Gain-Applied Reading (xxh) (R)
 TSC[0:5Eh...65h] - Reserved (xxh)
 TSC[0:66h] - ADC DC Measurement 1 (00h)
 TSC[0:67h] - ADC DC Measurement 2 (00h)
 TSC[0:68h,69h,6Ah] - ADC DC Measurement Output MSB,MID,LSB (R) (000000h)
 TSC[0:6Bh...73h] - Reserved (xxh)
 TSC[0:74h] - VOL/MICDET-Pin SAR ADC - Volume Control (00h)
 TSC[0:75h] - VOL/MICDET-Pin Gain (xxh) (R)
 TSC[0:76h...7Fh] - Reserved (xxh) |
| --- |

**TSC TSC, DAC and ADC Routing, PGA, Power-Controls and MISC Logic**

```

| TSC[1:01h..1Dh] - Reserved (xxh)
 TSC[1:1Eh] - Headphone and Speaker Amplifier Error Control (00h)
 TSC[1:1Fh] - Headphone Drivers (04h)
 TSC[1:20h] - Class-D Speaker Amplifier (06h)
 TSC[1:21h] - HP Output Drivers POP Removal Settings (3Eh)
 TSC[1:22h] - Output Driver PGA Ramp-Down Period Control (00h)
 TSC[1:23h] - DAC_L and DAC_R Output Mixer Routing (00h)
 TSC[1:24h] - Analog Volume to HPL (Left Headphone) (7Fh)
 TSC[1:25h] - Analog Volume to HPR (Right Headphone) (7Fh)
 TSC[1:26h] - Analog Volume to SPL (Left Speaker) (7Fh)
 TSC[1:27h] - Analog Volume to SPR (Right Speaker) (7Fh)
 TSC[1:28h] - HPL Driver (Left Headphone) (02h)
 TSC[1:29h] - HPR Driver (Right Headphone) (02h)
 TSC[1:2Ah] - SPL Driver (Left Speaker) (00h)
 TSC[1:2Bh] - SPR Driver (Right Speaker) (00h)
 TSC[1:2Ch] - HP Driver Control (00h)
 TSC[1:2Dh] - Reserved (xxh)
 TSC[1:2Eh] - MICBIAS (00h)
 TSC[1:2Fh] - MIC PGA (80h)
 TSC[1:30h] - P-Terminal Delta-Sigma Mono ADC Channel Fine-Gain Input (00h)
 TSC[1:31h] - M-Terminal ADC Input Selection (00h)
 TSC[1:32h] - Input CM Settings (00h)
 TSC[1:33h..FFh] - Reserved (xxh) |
| --- |

**Reserved Page**

```

| TSC[2:01h..FFh] - Reserved (00h) |
| --- |

**TSC Touchscreen/SAR Control**

```

| TSC[3:01h] - Reserved (xxh)
 TSC[3:02h] - SAR ADC Control 1 (00h)
 TSC[3:03h] - SAR ADC Control 2 (00h)
 TSC[3:04h] - Precharge and Sense (00h)
 TSC[3:05h] - Panel Voltage Stabilization (00h)
 TSC[3:06h] - Voltage Reference (20h)
 TSC[3:07h,08h] - Reserved (xxh)
 TSC[3:09h] - Status Bits 1 (40h) (R)
 TSC[3:0Ah] - Status Bits 2 (00h) (R)
 TSC[3:0Bh,0Ch] - Reserved (xxh)
 TSC[3:0Dh] - Buffer Mode (03h) ;DSi: Unused,seems to use TSC[3:0Eh] instead
 TSC[3:0Eh] - Reserved / Undocumented (read by DSi for Pen Down Test) (0Fh)
 TSC[3:0Fh] - Scan Mode Timer (40h)
 TSC[3:10h] - Scan Mode Timer Clock (81h)
 TSC[3:11h] - SAR ADC Clock (81h)
 TSC[3:12h] - Debounce Time for Pen-Up Detection (00h)
 TSC[3:13h] - Auto AUX Measurement Selection (00h)
 TSC[3:14h] - Touch-Screen Pen Down (00h)
 TSC[3:15h] - Threshold Check Flags Register (00h) (R)
 TSC[3:16h,17h] - AUX1 Maximum Value Check MSB,LSB (0000h)
 TSC[3:18h,19h] - AUX1 Minimum Value Check MSB,LSB (0000h)
 TSC[3:1Ah,1Bh] - AUX2 Maximum Value Check MSB,LSB (0000h)
 TSC[3:1Ch,1Dh] - AUX2 Minimum Value Check MSB,LSB (0000h)
 TSC[3:1Eh,1Fh] - Temperature(TEMP1/TEMP2) Maximum Value Check MSB,LSB (0000h)
 TSC[3:20h,21h] - Temperature(TEMP1/TEMP2) Minimum Value Check MSB,LSB (0000h)
 TSC[3:22h...29h] - Reserved (xxh)
 TSC[3:2Ah,2Bh] - Touchscreen X-Coordinate Data MSB,LSB (0000h) (R)
 TSC[3:2Ch,2Dh] - Touchscreen Y-Coordinate Data MSB,LSB (0000h) (R)
 TSC[3:2Eh,2Fh] - Touchscreen Z1-Pressure Register MSB,LSB (0000h) (R)
 TSC[3:30h,31h] - Touchscreen Z2-Pressure Register MSB,LSB (0000h) (R)
 TSC[3:32h...35h] - Reserved (xxh)
 TSC[3:36h,37h] - AUX1 Data MSB,LSB (0000h) (R)
 TSC[3:38h,39h] - AUX2 Data MSB,LSB (0000h) (R)
 TSC[3:3Ah,3Bh] - VBAT Data MSB,LSB (0000h) (R)
 TSC[3:3Ch...41h] - Reserved (xxh)
 TSC[3:42h,43h] - Temperature TEMP1 Data Register MSB,LSB (0000h) (R)
 TSC[3:44h,45h] - Temperature TEMP2 Data Register MSB,LSB (0000h) (R)
 TSC[3:46h...7Fh] - Reserved (xxh) |
| --- |

**TSC Coefficient RAM and Instruction RAM for ADC/DAC**

```

| TSC[04h..05h:xxh] - ADC Coefficient RAM (126 x 16bit)
 TSC[06h..07h:xxh] - Reserved (00h)
 TSC[08h:01h] - DAC Coefficient RAM Control (00h)
 TSC[08h..0Bh:xxh] - DAC Coefficient RAM, DAC Buffer A (252 x 16bit)
 TSC[0Ch..0Fh:xxh] - DAC Coefficient RAM, DAC Buffer B (252 x 16bit)
 TSC[10h..1Fh:xxh] - Reserved (00h)
 TSC[20h..2Bh:xxh] - ADC DSP Engine Instruction RAM (384 x 24bit)
 TSC[2Ch..3Fh:xxh] - Reserved (00h)
 TSC[40h..5Fh:xxh] - DAC DSP Engine Instruction RAM (1024 x 24bit)
 TSC[60h..FBh:xxh] - Reserved (00h) |
| --- |

**TSC Additional 3DS Registers (3DS only, and only on the 2nd SPI bus)**

```

| TSC[64h:01h..xxh] - 3DS Config Registers for Sound (and Microphone?)
 TSC[65h:01h..xxh] - 3DS Config Registers for Sound (and Microphone?)
 TSC[67h:01h..xxh] - 3DS Config Registers for Touchscreen and Circle Pad
 TSC[FBh:01h..xxh] - 3DS Buffer Mode Data for Touchscreen and Circle Pad |
| --- |

**TSC Touchscreen/SAR Buffer**

```

| TSC[FCh:01h..xxh] - Buffer Mode Data MSB,LSB (xxxxh) (R)
 TSC[FCh:xxh..7Fh] - Reserved (xxh) |
| --- |

**TSC Undocumented Registers**

```

| TSC[FDh:xxh] - Contains some undocumented non-zero values (DSi specific?)
 TSC[FEh:xxh] - Reserved (00h)
 TSC[FFh:xxh] - Accessing this page changes operation (DSi specific?) |
| --- |

| DSi TSC[0:00h..1Ah], Basic PLL and Timing Control |
| --- |

**TSC[xxh:00h] - Page Select Register (00h)**

```

| 7-0 Page Select (00h..FEh) (FFh=Undocumented, enter special mode?) |
| --- |

Selects the "page" for the TSC[page:index] addresses.

**TSC[0:01h] - Software Reset (00h)**

```

| 7-1 Reserved. Write only zeros to these bits.
 0 Software Reset (0=No change, 1=Reset) |
| --- |

**TSC[0:02h] - Reserved (xxh) (R)**

```

| 7-0 Reserved. Do not write to this register. |
| --- |

**TSC[0:03h] - Overtemperature OT Flag (02h..FFh) (R)**

```

| 7-2 Reserved. Do not write to these bits. (R)
 1 Overtemperature protection flag (0=Alert, 1=Normal) (R)
 0 Reserved. Do not write to these bits. (R/W?) |
| --- |

Bit1 is valid only if speaker amplifier is powered up. Initial value varies per
console:

```

| Old3DS = 16h
 New3DS XL = 36h |
| --- |

Maybe the upper bits contain a Chip ID.

**TSC[0:04h] - Clock-Gen Muxing (00h)**

```

| 7-4 Reserved. Write only zeros to these bits.
 3-2 Select PLL_CLKIN (0=MCLK, 1=BCLK, 2=GPIO1, 3=SDIN)
 1-0 Select CODEC_CLKIN (0=MCLK, 1=BCLK, 2=GPIO1, 3=PLL_CLK) |
| --- |

See Section 5.8 for more details on clock generation mutiplexing and dividers.

**TSC[0:05h] - PLL P and R-Values (11h)**

```

| 7 PLL Enable (0=Power down, 1=Power up)
 6-4 PLL Divider P (1..7=Div1..7, or 0=Div8)
 3-0 PLL Multiplier R (1..15=Mul1..15, or 0=Mul16) |
| --- |

**TSC[0:06h] - PLL J-Value (04h)**

```

| 7-6 Reserved. Write only zeros to these bits.
 5-0 PLL Multiplier J (1..63=Mul1..63, or 0=Reserved) |
| --- |

**TSC[0:07h,08h] - PLL D-Value MSB,LSB (0000h)**

```

| 15-14 Reserved. Write only zeros to these bits.
 13-0 PLL fractional multiplier D-Val (14bit) |
| --- |

Note that LSB register must be written to immediately after writing to MSB.

**TSC[0:09h,0Ah] - Reserved (xxh)**

```

| 7-0 Reserved. Write only zeros to these bits. |
| --- |

**TSC[0:0Bh] - DAC NDAC Value (01h)**

```

| 7 DAC NDAC Divider Enable (0=Power down, 1=Power up)
 6-0 DAC NDAC Divider (1..127=Div1..127, or 0=Div128) |
| --- |

**TSC[0:0Ch] - DAC MDAC Value (01h)**

```

| 7 DAC MDAC Divider Enable (0=Power down, 1=Power up)
 6-0 DAC MDAC Divider (1..127=Div1..127, or 0=Div128) |
| --- |

**TSC[0:0Dh,0Eh] - DAC DOSR Value MSB,LSB (0080h)**

```

| 15-10 Reserved
 9-0 DAC OSR value "DOSR" (1..1023, or 0=1024) |
| --- |

DOSR should be an integral multiple of the interpolation ratio in TSC[0:10h].

Note that LSB register must be written to immediately after writing to MSB.

**TSC[0:0Fh] - DAC IDAC Value (80h)**

```

| 7-0 Number of instructions for DAC miniDSP engine (IDAC=N*4)
 (1..255 = 4..1020 (N*4), or 0=1024) |
| --- |

IDAC should be an integral multiple of the interpolation ratio in TSC[0:10h].

**TSC[0:10h] - DAC miniDSP Engine Interpolation (08h)**

```

| 7-4 Reserved. Do not write to these registers.
 3-0 Interpolation ratio in DAC miniDSP engine (1..15, or 0=16) |
| --- |

**TSC[0:11h] - Reserved (xxh)**

```

| 7-0 Reserved. Do not write to this register. |
| --- |

**TSC[0:12h] - ADC NADC Value (01h)**

```

| 7 ADC NADC divider is powered
 0: ADC NADC divider is powered down and ADC_DSP_CLK = DAC_DSP_CLK.
 1: ADC NADC divider is powered up.
 6-0 ADC NADC divider (1..127, or 0=128) |
| --- |

**TSC[0:13h] - ADC MADC Value (01h)**

```

| 7 ADC MADC divider is powered
 0: ADC MADC divider is powered down and ADC_MOD_CLK = DAC_MOD_CLK.
 1: ADC MADC divider is powered up.
 6-0 ADC MADC divider (1..127, or 0=128) |
| --- |

**TSC[0:14h] - ADC AOSR Value (80h)**

```

| 7-0 ADC OSR "AOSR" divider (1..255, or 0=256) |
| --- |

AOSR should be an integral multiple of the decimation ratio in TSC[0:16h].

**TSC[0:15h] - ADC IADC Value (80h)**

```

| 7-0 Number of instruction for ADC miniDSP engine (IADC=N*2)
 (1..192 = 2..384 (N*2), or 0,193..255=Reserved) |
| --- |

IADC should be an integral multiple of the decimation ratio in TSC[0:16h].

**TSC[0:16h] - ADC miniDSP Engine Decimation (04h)**

```

| 7-4 Reserved
 3-0 Decimation ratio in ADC miniDSP engine (1..15, or 0=16) |
| --- |

**TSC[0:17h,18h] - Reserved (xxh)**

```

| 7-0 Reserved. Do not write to these registers. |
| --- |

**TSC[0:19h] - CLKOUT MUX (00h)**

```

| 7-3 Reserved
 2-0 CDIV_CLKIN (0=MCLK, 1=BCLK, 2=SDIN, 3=PLL_CLK, 4=DAC_CLK(DSP),
 5=DAC_MOD_CLK, 6=ADC_CLK(DSP), 7=ADC_MOD_CLK) |
| --- |

**TSC[0:1Ah] - CLKOUT Divider M Value (01h)**

```

| 7 CLKOUT divider M Enable (0=Powered down, 1=Powered up)
 6-0 CLKOUT divider M (1..127, or 0=128) |
| --- |

| DSi TSC[0:1Bh..23h], Codec Control |
| --- |

**TSC[0:1Bh] - Codec Interface Control 1 (00h) (R/W)**

```

| 7-6 Codec interface type (0=I2S, 1=DSP, 2=RJF, 3=LJF)
 5-4 Codec interface word length (0..3=16,20,24,32 bits)
 3 BCLK Direction (0=Input, 1=Output)
 2 WCLK Direction (0=Input, 1=Output)
 1 Reserved
 0 Driving SDOUT to High-Impedance for the Extra BCLK cycle when
 data is not being transferred (0=Disabled, 1=Enabled) |
| --- |

**TSC[0:1Ch] - Data-Slot Offset Programmability (00h)**

```

| 7-0 Offset (0..255 = 0..255 BCLKs) |
| --- |

Note: Measured with respect to WCLK Rising Edge in DSP Mode.

**TSC[0:1Dh] - Codec Interface Control 2 (00h)**

```

| 7-6 Reserved
 5 SDIN-to-SDOUT loopback (0=Disable, 1=Enable)
 4 ADC-to-DAC loopback (0=Disable, 1=Enable)
 3 BCLK Invert (0=No, 1=Invert)
 2 BCLK and WCLK active even with Codec powered down (0=No, 1=Yes)
 1-0 BDIV_CLKIN (0=DAC_CLK, 1=DAC_MOD_CLK, 2=ADC_CLK, 3=ADC_MOD_CLK) |
| --- |

The BCLK settings in Bit2,3 do apply to both Primary and Secondary BCLK.

**TSC[0:1Eh] - BCLK Divider N Value (01h)**

```

| 7 BCLK divider N Enable (0=Powered down, 1=Powered up)
 6-0 BCLK divider N (1..127, or 0=128) |
| --- |

**TSC[0:1Fh] - Codec Secondary Interface Control 1 (00h)**

```

| 7-5 Secondary BCLK is obtained from ;\(0=GPIO1, 1=SCLK, 2=MISO, 3=SDOUT,
 4-2 Secondary WCLK is obtained from ;/ 4=GPIO2, 5=GPI1, 6=GPI2, 7=GPI3)
 1-0 Secondary SDIN is obtained from (0=GPIO1, 1=SCLK, 2=GPIO2, 3=GPI1) |
| --- |

**TSC[0:20h] - Codec Secondary Interface Control 2 (00h)**

```

| 7-5 ADC_WCLK is obtained from (0=GPIO1, 1=SCLK, 2=MISO, 3=Reserved,
 4 Reserved 4=GPIO2, 5=GPI1, 6=GPI2, 7=GPI3)
 3 Codec/ClockGen BCLK source (0=Primary BCLK, 1=Secondary BCLK)
 2 Codec WCLK source (0=Primary WCLK, 1=Secondary WCLK)
 1 Codec ADC_WCLK source (0=DAC_WCLK, 1=ADC_WCLK)
 0 Codec SDIN source (0=Primary SDIN, 1=Secondary SDIN) |
| --- |

**TSC[0:21h] - Codec Secondary Interface Control 3 (00h)**

```

| 7 Primary BCLK output (0=Internally generated BCLK, 1=Secondary BCLK)
 6 Secondary BCLK output (0=Primary BCLK, 1=Internally generated BCLK)
 5-4 Primary WCLK output (0=DAC_fS, 1=ADC_fS, 2=Secondary WCLK, 3=Reserved)
 3-2 Secondary WCLK output (0=Primary WCLK, 1=DAC_fS, 2=ADC_fS, 3=Reserved)
 1 Primary SDOUT (0=SDOUT from codec, 1=Secondary SDIN)
 0 Secondary SDOUT (0=Primary SDIN, 1=SDOUT from codec) |
| --- |

**TSC[0:22h] - I2C Bus Condition (00h)**

```

| 7-6 Reserved. Write only the reset value to these bits.
 5 Accept I2C general-call address (0=No/Ignore, 1=Yes/Accept)
 4-0 Reserved. Write only zeros to these bits. |
| --- |

**TSC[0:23h] - Reserved (xxh)**

```

| 7-0 Reserved. Write only zeros to these bits. |
| --- |

| DSi TSC[0:24h..32h], Status and Interrupt Flags |
| --- |

**TSC[0:24h] - ADC Flag Register (0xh) (R)**

```

| 7 ADC PGA applied gain = programmed gain (0=Differs, 1=Equal) (R)
 6 ADC powered (0=Powered down, 1=Powered up) (R)
 5 AGC saturated (0=No/inrange, 1=Yes/saturated to max) (R)
 4-0 Reserved. Write only zeros to these bits. |
| --- |

Note on D5(?): Sticky flag bIts. These are read-only bits. They are
automatically cleared once they are read and are set only if the source trigger
occurs again.

**TSC[0:25h] - DAC Flag Register (00h) (R)**

```

| 7 Left-channel DAC powered (0=Powered down, 1=Powered up) (R)
 6 Reserved. Write only zero to this bit.
 5 Left Headphone HPL driver powered (0=Powered down, 1=Powered up) (R)
 4 Left-channel class-D driver powered (0=Powered down, 1=Powered up) (R)
 3 Right-channel DAC powered (0=Powered down, 1=Powered up) (R)
 2 Reserved. Write only zero to this bit.
 1 Right Headphone HPR driver powered (0=Powered down, 1=Powered up) (R)
 0 Right-channel class-D driver powered (0=Powered down, 1=Powered up) (R) |
| --- |

**TSC[0:26h] - DAC Flag Register (00h) (R)**

```

| 7-5 Reserved. Do not write to these bits.
 4 Left-channel DAC PGA applied gain=programmed gain (0=Differs, 1=Equal)
 3-1 Reserved. Write only zeros to these bits.
 0 Right-channel DAC PGA applied gain=programmed gain (0=Differs, 1=Equal) |
| --- |

**TSC[0:27h] - Overflow Flags (00h) (R)**

```

| 7 Left-Channel DAC Overflow Flag (0=None, 1=Overflow) (R)
 6 Right-Channel DAC Overflow Flag (0=None, 1=Overflow) (R)
 5 DAC Barrel Shifter Output Overflow Flag (0=None, 1=Overflow) (R)
 4 Reserved. Write only zeros to these bits.
 3 Delta-Sigma Mono ADC Overflow Flag (0=None, 1=Overflow) (R)
 2 Reserved. Write only zero to this bit.
 1 ADC Barrel Shifter Output Overflow Flag (0=None, 1=Overflow) (R)
 0 Reserved. Write only zero to this bit. |
| --- |

Sticky flag bIts. These are read-only bits. They are automatically cleared once
they are read and are set only if the source trigger occurs again.

**TSC[0:28h..2Bh] - Reserved (xxh)**

```

| 7-0 Reserved. Write only the reset value to these bits. |
| --- |

**TSC[0:2Ch] - Interrupt Flags DAC, sticky (00h..30h) (R)**

```

| 7 Short-circuit detected at HPL/left class-D driver (0=No, 1=Yes)
 6 Short-circuit detected at HPR/right class-D driver (0=No, 1=Yes)
 5 Headset button pressed (0=No, 1=Yes)
 4 Headset insertion/removal is detected (0=No, 1=Yes)
 3 Left DAC signal power vs signal threshold of DRC (0=Less/Equal,1=Above)
 2 Right DAC signal power vs signal threshold of DRC(0=Less/Equal,1=Above)
 1 DAC miniDSP Engine Standard Interrupt-Port Output (0=Read 0, 1=Read 1)
 0 DAC miniDSP Engine Auxiliary Interrupt-Port Output (0=Read 0, 1=Read 1) |
| --- |

Sticky flag bIts. These are read-only bits. They are automatically cleared once
they are read and are set only if the source trigger occurs again.

**TSC[0:2Dh] - Interrupt Flags ADC, sticky (00h..18h) (R)**

```

| 7 Reserved. Write only zero to this bit.
 6 ADC signal power vs noise threshold for AGC (0=Greater, 1=Less)
 5 Reserved. Write only zeros to these bits.
 4 ADC miniDSP Engine Standard Interrupt Port Output (0=Read 0, 1=Read 1)
 3 ADC miniDSP Engine Auxiliary Interrupt Port Output (0=Read 0, 1=Read 1)
 2 DC measurement using Delta Sigma Audio ADC
 (0=Not available, 1=Not available, too, uh?)
 1-0 Reserved. Write only zeros to these bits. |
| --- |

Sticky flag bIts. These are read-only bits. They are automatically cleared once
they are read and are set only if the source trigger occurs again.

**TSC[0:2Eh] - Interrupt Flags DAC, non-sticky? (00h..30h) (R)**

```

| 7 Short circuit detected at HPL/left class-D driver (0=No, 1=Yes)
 6 Short circuit detected at HPR/right class-D driver (0=No, 1=Yes)
 5 Headset button pressed (0=No, 1=Yes)
 4 Headset removal/insertion detected (0=Removal, 1=Insertion)
 3 Left DAC signal power vs signal threshold of DRC (0=Below, 1=Above)
 2 Right DAC signal power vs signal threshold of DRC (0=Below, 1=Above)
 1 DAC miniDSP Engine Standard Interrupt Port Output (0=Read 0, 1=Read 1)
 0 DAC miniDSP Engine Auxiliary Interrupt Port Output (0=Read 0, 1=Read 1) |
| --- |

This is (almost?) same as TSC[0:2Ch]. Maybe this is current state (non-sticky)?

**TSC[0:2Fh] - Interrupt Flags ADC, non-sticky? (00h..18h) (R)**

```

| 7 Reserved
 6 Delta-sigma mono ADC signal power vs noise threshold for left AGC
 5 Reserved (0=Greater, 1=Less)
 4 ADC miniDSP Engine Standard Interrupt Port Output (0=Read 0, 1=Read 1)
 3 ADC miniDSP Engine Auxiliary Interrupt Port Output (0=Read 0, 1=Read 1)
 2 DC measurement using Delta Sigma Audio ADC
 (0=Not available, 1=Not available, too, uh?)
 1-0 Reserved. Write only zeros to these bits. |
| --- |

This is (almost?) same as TSC[0:2Dh]. Maybe this is current state (non-sticky)?

**TSC[0:30h] - INT1 Control Register (Select INT1 Sources) (00h)**

**TSC[0:31h] - INT2 Control Register (Select INT2 Sources) (00h)**

```

| 7 Headset-insertion detect (0=Off, 1=On)
 6 Button-press detect (0=Off, 1=On)
 5 DAC DRC signal-power (0=Off, 1=On)
 4 ADC AGC noise (0=Off, 1=On)
 3 Short-circuit (0=Off, 1=On)
 2 Engine-generated (0=Off, 1=On)
 1 DC measurement using Delta Sigma Audio ADC data-available (0=Off, 1=On)
 0 INT duration (0=Pulse Once, 1=Pulse Repeatedly until Acknowledge) |
| --- |

Bit1-7 select which sources shall trigger INT1/INT2, a few more sources can be
selected in TSC[0:32h].Bit5-7.

Bit0 selects how the INT1/INT2 signal shall be pulsed (once with 2ms length, or
repeating every 4ms with 50% duty, until it gets acknowledged by reading
TSC[0:2Ch,2Dh,32h]).

**TSC[0:32h] - INT1 and INT2 Control Register (00h)**

```

| 7 INT1 upon SAR measurement data-out-of-threshold range (0=Off, 1=Off?)
 6 INT1 upon Pen touch/SAR data-available (0=Off, 1=On)
 5 INT2 upon SAR measurement data-out-of-threshold range (0=Off, 1=Off?)
 4 Reserved
 3 Pen touch detected (0=No, 1=Touch) (R)
 2 Data available for read (0=No, 1=Available) (R)
 1 SAR data out of programmed threshold range (0=No, 1=Out) (R)
 0 Reserved. Write only the default value to this bit. (R) |
| --- |

| DSi TSC[0:33h..3Bh], Pin Control |
| --- |

**TSC[0:33h] - GPIO1 In/Out Pin Control (00h..C2h)**

**TSC[0:34h] - GPIO2 In/Out Pin Control (00h..C2h)**

```

| 7-6 Reserved. Do not write any value other than reset value.
 5-2 GPIOx Mode (R/W)
 0 = GPIOx disabled (input and output buffers powered down)
 1 = GPIOx input mode (as secondary BCLK/WCLK/SDIN input, or
 as ADC_WCLK input, Dig_Mic_In or in ClockGen block)
 2 = GPIOx input mode (as GPI general-purpose input)
 3 = GPIOx output = general-purpose output
 4 = GPIOx output = CLKOUT output
 5 = GPIOx output = INT1 output
 6 = GPIOx output = INT2 output
 7 = GPIOx output = ADC_WCLK output for codec interface
 8 = GPIOx output = secondary BCLK output for codec interface
 9 = GPIOx output = secondary WCLK output for codec interface
 10 = GPIOx output = ADC_MOD_CLK output for the digital microphone
 11 = GPIOx output = secondary SDOUT for codec interface
 12 = GPIOx output = TouchScreen/SAR ADC interrupt (active-low),
 13-15 = Reserved as PINTDAV signal
 1 GPIOx input buffer value (0 or 1) (R)
 0 GPIOx general-purpose output value (0 or 1) (R/W) |
| --- |

**TSC[0:35h] - SDOUT (OUT Pin) Control (12h)**

```

| 7-5 Reserved
 4 SDOUT bus keeper (0=Enabled, 1=Disabled)
 3-1 SDOUT Mode
 0 = SDOUT disabled (output buffer powered down)
 1 = SDOUT = primary SDOUT output for codec interface
 2 = SDOUT = general-purpose output
 3 = SDOUT = CLKOUT output
 4 = SDOUT = INT1 output
 5 = SDOUT = INT2 output
 6 = SDOUT = secondary BCLK output for codec interface
 7 = SDOUT = secondary WCLK output for codec interface
 0 SDOUT general-purpose output value (0 or 1) |
| --- |

**TSC[0:36h] - SDIN (IN Pin) Control (02h or 03h)**

```

| 7-3 Reserved
 2-1 SDIN Mode
 0 = SDIN disabled (input buffer powered down)
 1 = SDIN enabled (as codec SDIN, Dig_Mic_In, or in ClockGen block)
 2 = SDIN enabled (as GPI general-purpose input)
 3 = Reserved
 0 SDIN input-buffer value (0 or 1) (R) |
| --- |

**TSC[0:37h] - MISO (OUT Pin) Control (02h)**

```

| 7-5 Reserved
 4-1 MISO Mode
 0 = MISO disabled (output buffer powered down)
 1 = MISO = MISO output for SPI interface (or disabled for I2C)
 2 = General-purpose output
 3 = MISO = CLKOUT output
 4 = MISO = INT1 output
 5 = MISO = INT2 output
 6 = MISO = ADC_WCLK output for codec interface
 7 = MISO = ADC_MOD_CLK output for the digital microphone
 8 = MISO = secondary SDOUT for codec interface
 9 = MISO = secondary BCLK output for codec interface
 10 = MISO = secondary WCLK output for codec interface
 11-15 = Reserved
 0 MISO general-purpose output value (0 or 1) |
| --- |

**TSC[0:38h] - SCLK (IN Pin) Control (02h..03h)**

```

| 7-3 Reserved
 2-1 SCLK Mode
 0 = SCLK disabled (input buffer powered down)
 1 = SCLK enabled (for the SPI interface)
 2 = SCLK enabled (as a GPI general-purpose input)
 3 = SCLK enabled (as secondary SDIN/BCLK/WCLK input,
 or as ADC_WCLK input, or Dig_Mic_In)
 0 SCLK input buffer value (0 or 1) (R) |
| --- |

**TSC[0:39h] - GPI1 and GPI2 Pin Control (00h..11h)**

```

| 7 Reserved. Write only zero to this bit.
 6-5 GPI1 Mode
 0 = GPI1 disabled (input buffer powered down)
 1 = GPI1 enabled (as secondary SDIN/BCLK/WCLK input, or ADC_WCLK inp)
 2 = GPI1 enabled (as a GPI general-purpose input)
 3 = Reserved (unlike below GPI2)
 4 GPI1 pin value (0 or 1) (R)
 3 Reserved. Write only zero to this bit.
 2-1 GPI2 Mode
 0 = GPI2 disabled (input buffer powered down)
 1 = GPI2 enabled (as secondary BCLK/WCLK input, or ADC_WCLK input)
 2 = GPI2 enabled (as a GPI general-purpose input)
 3 = GPI2 enabled (as an HP_SP input)
 0 GPI2 pin value (0 or 1) (R) |
| --- |

The DSi has GPI1 wired to VCNT5.

The DSi has GPI2 wired to HP#SP (headphone/speaker switch).

**TSC[0:3Ah] - GPI3 Pin Control (00h..10h)**

```

| 7 Reserved. Write only zero to this bit.
 6-5 GPI3 Mode
 0 = GPI3 disabled (input buffer powered down)
 1 = GPI3 enabled (as secondary BCLK/WCLK input, or ADC_WCLK input)
 2 = GPI3 enabled (as a GPI general purpose input)
 3 = Reserved (Undocumented - used by DSi?)
 4 GPI3 pin value (0 or 1) (R)
 3-0 Reserved. Write only zeros to these bits. |
| --- |

The DSi has the GPI3 pin wired to Headphone Mute logic.

**TSC[0:3Bh] - Reserved (xxh)**

```

| 7-0 Reserved. Write only zeros to these bits. |
| --- |

| DSi TSC[0:3Ch..55h], DAC/ADC and Beep |
| --- |

**TSC[0:3Ch] - DAC Instruction Set (01h)**

```

| 7-5 Reserved. Write only default value.
 4-0 DAC Signal Processing Block
 0 = DAC miniDSP is used for signal processing
 1..25 = DAC Signal Processing Block PRB_P1 .. PRB_P25
 26..31 = Reserved. Do not use. |
| --- |

**TSC[0:3Dh] - ADC Instruction Set (04h)**

```

| 7-5 Reserved. Write only default values.
 4-0 ADC Signal Processing Block
 0 = ADC miniDSP is used for signal processing
 1..3 = Reserved
 4..6 = ADC Signal Processing Block PRB_R4 .. PRB_R6
 7..9 = Reserved
 10..12 = ADC Signal Processing Block PRB_R10 .. PRB_R12
 13..15 = Reserved
 16..18 = ADC Signal Processing Block PRB_R16 .. PRB_R18
 19..31 = Reserved. Do not write these sequences to these bits. |
| --- |

**TSC[0:3Eh] - Programmable Instruction Mode-Control Bits (00h)**

```

| 7 Reserved
 6 ADC miniDSP Engine Auxiliary Control bit A (0 or 1)
 5 ADC miniDSP Engine Auxiliary Control bit B (0 or 1)
 4 Reset ADC miniDSP instruction counter at start of new frame (0=Yes)
 3 Reserved
 2 DAC miniDSP Engine Auxiliary Control bit A (0 or 1)
 1 DAC miniDSP Engine Auxiliary Control bit B (0 or 1)
 0 Reset DAC miniDSP instruction counter at start of new frame (0=Yes) |
| --- |

Above DAC/ADC bit A and B can be used for conditional instructions like JMP.

**TSC[0:3Fh] - DAC Data-Path Setup (14h)**

```

| 7 Left-channel DAC (0=Powered down, 1=Powered up)
 6 Right-channel DAC (0=Powered down, 1=Powered up)
 5-4 Left-channel DAC data path (0=Off, 1=Left Data, 2=Right Data, 3=Both)
 3-2 Right-channel DAC data path (0=Off, 1=Right Data, 2=Left Data, 3=Both)
 1-0 DAC channel volume control soft-stepping (0=One step per sample,
 1=One step per 2 samples, 2=Disabled, 3=Reserved) |
| --- |

Wheras, Both=((L+R)/2).

**TSC[0:40h] - DAC Volume Control (0Ch)**

```

| 7-4 Reserved. Write only zeros to these bits.
 3 Left-channel DAC (0=Not muted, 1=Muted)
 2 Right-channel DAC (0=Not muted, 1=Muted)
 1-0 DAC Mono/Stereo Volume
 0: Use Left/Right volume control for Left/Right channels ("stereo")
 1: Use Right volume control for Both channels ("mono")
 2: Use Left volume control for Both channels ("mono")
 3: Same as 0 ("stereo") |
| --- |

**TSC[0:41h] - DAC Left Volume Control (00h)**

**TSC[0:42h] - DAC Right Volume Control (00h)**

```

| 7-0 Digital gain in 0.5dB units (-127..+48 = -63.5dB..+24dB, Other=Reserved) |
| --- |

**TSC[0:43h] - Headset Detection (00h..60h)**

```

| 7 Headset detection Enable (0=Disabled, 1=Enabled)
 6-5 Headset detection (0=None, 1=Headset, 2=Reserved, 3=Headset+Mic) (R)
 4-2 Debounce for Glitch Rejection During Headset Detection
 (0..5 = 16ms, 32ms, 64ms, 128ms, 256ms, 512ms, 6..7=Reserved)
 (when TSC[3:10h] set to 1MHz)
 1-0 Debounce for Glitch Rejection During Headset Button-Press Detection
 (0..3 = 0ms, 8ms, 16ms, 32ms) (when TSC[3:10h] set to 1MHz) |
| --- |

Sampling is 8x faster than above timings (eg. time=32ms uses 4ms sampling).

**TSC[0:44h] - DRC Control 1 (0Fh)**

```

| 7 Reserved. Write only the reset value to these bits.
 6 DRC for left channel (0=Disabled, 1=Enabled)
 5 DRC for right channel (0=Disabled, 1=Enabled)
 4-2 DRC threshold (0..7 = -3dB,-6dB,-9dB,-12dB,-15dB,-18dB,-21dB,-24dB)
 1-0 DRC hysteresis (0..3 = +0dB,+1dB,+2dB,+3dB) |
| --- |

**TSC[0:45h] - DRC Control 2 (38h)**

```

| 7 Reserved. Write only the reset value to these bits.
 6-3 DRC Hold Time
 0 = DRC Hold Disabled ;-disable
 1 = 32 DAC Word Clocks ;\
 2 = 64 DAC Word Clocks ;
 3 = 128 DAC Word Clocks ;
 4 = 256 DAC Word Clocks ; powers of 2
 5 = 512 DAC Word Clocks ;
 6 = 1024 DAC Word Clocks ;
 7 = 2048 DAC Word Clocks ;
 8 = 4096 DAC Word Clocks ;
 9 = 8192 DAC Word Clocks ;
 10 = 16384 DAC Word Clocks ;/
 11 = 1*32768 DAC Word Clocks ;\
 12 = 2*32768 DAC Word Clocks ;
 13 = 3*32768 DAC Word Clocks ; multiples of 32768
 14 = 4*32768 DAC Word Clocks ;
 15 = 5*32768 DAC Word Clocks ;/
 2-0 Reserved. Write only the reset value to these bits. |
| --- |

**TSC[0:46h] - DRC Control 3 (00h)**

```

| 7-4 DRC attack rate, "(4 SHR N) dB per DAC Word Clock"
 (0=4dB, 1=2dB, 2=1dB, ..., 15=0.000122dB per DAC Word Clock)
 3-0 DRC decay rate, "(1 SHR (N+6)) dB per DAC Word Clock"
 (0=0.0156dB, 1=0.00781dB, ..., 15=0.000000476dB per DAC Word Clock) |
| --- |

**TSC[0:47h] - Beep Generator and Left Beep Volume (00h)**

```

| 7 Beep Generator Enable (0=Disabled/Duration ended, 1=Enabled/Busy)
 (self-clearing based on beep duration)
 6 Auto beep generator on pen touch (0=Disabled, 1=Enabled)
 (CODEC_CLKIN should be available for this and is
 used whenever touch is detected).
 5-0 Left-channel beep volume control "(2-N)dB" (0..63 = +2dB .. -61dB) |
| --- |

The beep generator is only available in PRB_P25 DAC processing mode.

**TSC[0:48h] - Beep Generator and Right Beep Volume (00h)**

```

| 7-6 Beep Mono/Stereo Volume
 0: Use Left/Right volume control for Left/Right channels ("stereo")
 1: Use Right volume control for Both channels ("mono")
 2: Use Left volume control for Both channels ("mono")
 3: Same as 0 ("stereo")
 5-0 Right-channel beep volume control "(2-N)dB" (0..63 = +2dB .. -61dB) |
| --- |

The beep generator is only available in PRB_P25 DAC processing mode.

**TSC[0:49h,4Ah,4Bh] - Beep Length MSB,MID,LSB (0000EEh)**

```

| 23-0 Number of samples for which beep need to be generated (24bit) |
| --- |

**TSC[0:4Ch,4Dh] - Beep Frequency Sin(x) MSB,LSB (10D8h)**

**TSC[0:4Eh,4Fh] - Beep Frequency Cos(x) MSB,LSB (7EE3h)**

```

| 15-0 Beep Frequency sin/cos values (16bit, each) |
| --- |

These registers should be set to sin(2pi*fin/fS) and cos(2pi*fin/fS)
accordingly; where fin is the beep frequency and fS is the DAC sample rate.

**TSC[0:50h] - Reserved (xxh)**

```

| 7-0 Reserved. Write only the reset value to these bits. |
| --- |

**TSC[0:51h] - ADC Digital Mic (00h)**

```

| 7 ADC channel (0=Powered Down, 1=Powered Up)
 6 Reserved
 5-4 Digital microphone input (0=GPIO1, 1=SCLK, 2=SDIN, 3=GPIO2)
 3 Digital microphone for delta-sigma mono ADC channel (0=Off, 1=On)
 2 Reserved
 1-0 ADC channel volume control soft-stepping (0=One step per sample,
 1=One step per 2 samples, 2=Disabled, 3=Reserved) |
| --- |

**TSC[0:52h] - ADC Digital Volume Control Fine Adjust (80h)**

```

| 7 ADC channel (0=Not muted, 1=Muted)
 6-4 Delta-Sigma Mono ADC Channel Volume Control Fine Gain
 (0=0dB, 1=-0.1dB, 2=-0.2dB, 3=-0.3dB, 4=-0.4dB, 5..7=Reserved)
 3-0 Reserved. Write only zeros to these bits. |
| --- |

**TSC[0:53h] - ADC Digital Volume Control Coarse Adjust (00h)**

```

| 7 Reserved
 6-0 Delta-Sigma Mono ADC Channel Volume Control Coarse Gain
 00h..27h = 0..39 = Reserved
 28h..xxh = 40 = -12 dB
 29h = 39 = -11.5 dB
 ... ...
 67h = 103 = +19.5 dB
 68h = 104 = +20 dB
 69h..7Fh = 105..127 = Reserved |
| --- |

**TSC[0:54h,55h] - Reserved (xxh)**

```

| 7-0 Reserved. Write only the reset value to these bits. |
| --- |

| DSi TSC[0:56h..7Fh], AGC and ADC |
| --- |

Auto-Gain-Control & Analog-to-Digital Converter

**TSC[0:56h] - AGC Control 1 (00h)**

```

| 7 AGC (0=Disabled, 1=Enabled)
 6-4 AGC target level (0=-5.5dB, 1=-8dB, 2=-10dB, 3=-12dB,
 4=-14dB, 5=-17dB, 6=-20dB, 7=-24dB)
 3-0 Reserved. Write only zeros to these bits. |
| --- |

**TSC[0:57h] - AGC Control 2 (00h)**

```

| 7-6 AGC hysterysis setting (0=1dB, 1=2dB, 2=4dB, 3=Disable AGC hysterysis)
 5-1 AGC noise threshold (and silence detection)
 0 = AGC noise/silence detection is disabled.
 1 = AGC noise threshold = -30dB
 2 = AGC noise threshold = -32dB
 3 = AGC noise threshold = -34dB
 ...
 29 = AGC noise threshold = -86dB
 30 = AGC noise threshold = -88dB
 31 = AGC noise threshold = -90dB
 0 Reserved. Write only zero to this bit. |
| --- |

**TSC[0:58h] - AGC Maximum Gain (7Fh, uh that's 7Fh=Reserved?)**

```

| 7 Reserved. Write only zero to this bit.
 6-0 AGC maximum gain in 0.5dB units (0..119=0..+59.5dB, 120..127=Reserved) |
| --- |

**TSC[0:59h] - AGC Attack Time (00h)**

**TSC[0:5Ah] - AGC Decay Time (00h)**

```

| 7-3 AGC attack/decay time, (N*2+1)*32/fS (0..31 = 1*32/fS .. 63*32/fS)
 2-0 AGC attack/decay time Multiply factor, 1 SHL N (0..7 = 1..128) |
| --- |

Whereas, fS is the ADC sample rate.

**TSC[0:5Bh] - AGC Noise Debounce (00h)**

```

| 7-5 Reserved. Write only zeros to these bits.
 4-0 AGC noise debounce
 0..5 = 0/fS, 4/fS, 8/fS, 16/fS, 32/fS, 64/fS ;\powers of 2
 6..10 = 128/fS, 256/fS, 512/fS, 1024/fS, 2048/fS ;/
 11..14 = 1*4096/fS, 2*4096/fS, 3*4096/fS ;\multiples
 14..31 = 4*4096/fS, .., 20*4096/fS, 21*4096/fS ;/of 4096 |
| --- |

**TSC[0:5Ch] - AGC Signal Debounce (00h)**

```

| 7-4 Reserved. Write only zeros to these bits.
 3-0 AGC signal debounce
 0..5 = 0/fS, 4/fS, 8/fS, 16/fS, 32/fS, 64/fS ;\powers of 2
 6..9 = 128/fS, 256/fS, 512/fS, 1024/fS ;/
 10..13 = 1*2048/fS, 2*2048/fS, 3*2048/fS ;\multiples
 13..15 = 4*2048/fS, 5*2048/fS, 6*2048/fS ;/of 2048 |
| --- |

**TSC[0:5Dh] - AGC Gain-Applied Reading (xxh) (R)**

```

| 7-0 Gain applied by AGC in 0.5dB units (-24..+119 = -12dB..+59.5dB) (R) |
| --- |

**TSC[0:5Eh...65h] - Reserved (xxh)**

```

| 7-0 Reserved. Do not write to these registers. |
| --- |

**TSC[0:66h] - ADC DC Measurement 1 (00h)**

```

| 7 DC measurement for mono ADC channel (0=Disabled, 1=Enabled)
 6 Reserved. Write only reset value.
 5 DC measurement is done based on
 0: 1st order sinc filter with averaging of 2^D.
 1: 1st order low-pass IIR filter whose coefficients
 are calculated based on D value.
 4-0 DC Meaurement D setting (1..20 = D=1 .. D=20) (0 or 21..31=Reserved) |
| --- |

**TSC[0:67h] - ADC DC Measurement 2 (00h)**

```

| 7 Reserved. Write only reset value.
 6 DC measurement data update (0=Enabled, 1=Disabled/allow stable reading)
 (Disabled: user can read the last updated data without corruption)
 5 For IIR based DC measurement, the measurment value is
 0: the instantaneous output of the IIR filter
 1: update before periodic clearing of the IIR filter
 4-0 IIR based DC measurment, average time setting:
 0 Infinite average is used
 1 Averaging time is 2^1 ADC modulator clock periods
 2 Averaging time is 2^2 ADC modulator clock periods
 ...
 19 Averaging time is 2^19 ADC modulator clock periods
 20 Averaging time is 2^20 ADC modulator clock periods
 21..31 Reserved. Don't use. |
| --- |

**TSC[0:68h,69h,6Ah] - ADC DC Measurement Output MSB,MID,LSB (R) (000000h)**

```

| 23-0 ADC DC Measurement Output (24bit) |
| --- |

**TSC[0:6Bh...73h] - Reserved (xxh)**

```

| 7-0 Reserved. Do not write to these registers. |
| --- |

**TSC[0:74h] - VOL/MICDET-Pin SAR ADC - Volume Control (00h)**

```

| 7 DAC volume control is controlled by,
 0: controlled by control register (7-bit Vol ADC is powered down)
 1: controlled by pin (analog volume input)
 6 Clock for the 7-bit Vol ADC for pin volume control,
 0: Internal on-chip RC oscillator
 1: External MCLK
 5-4 Hysteresis
 0: No hysteresis for volume control ADC output
 1: Hysteresis of +/-1 bit
 2: Hysteresis of +/-2 bits
 3: Reserved. Do not write this sequence to these bits.
 3 Reserved. Write only reset value.
 2-0 Throughput of the 7-bit Vol ADC for pin volume control,
 When Bit6=1 and external MCLK is 12MHz:
 (0..7=15.625Hz, 31.25Hz, 62.5Hz, 125Hz, 250Hz, 500Hz, 1000Hz, 2000Hz)
 When Bit6=0 (use Internal oscillator):
 (0..7=10.68Hz, 21.35Hz, 42.71Hz, 85Hz?, 170Hz, 340Hz, 680Hz, 1370Hz) |
| --- |

**TSC[0:75h] - VOL/MICDET-Pin Gain (xxh) (R)**

```

| 7 Reserved. Write only zero to this bit.
 6-0 Gain applied by pin volume control
 0 = +18 dB
 1 = +17.5 dB
 2 = +17 dB
 ...
 35 = +0.5 dB
 36 = 0 dB
 37 = -0.5 dB
 ...
 89 = -26.5 dB
 90 = -27 dB ;below in 1dB steps instead of 0.5dB steps !
 91 = -28 dB
 ...
 125 = -62 dB
 126 = -63 dB
 127 = Reserved |
| --- |

**TSC[0:76h...7Fh] - Reserved (xxh)**

```

| 7-0 Reserved. Do not write to these registers. |
| --- |

| DSi TSC[1:xxh], DAC and ADC Routing, PGA, Power-Controls and MISC Logic |
| --- |

**TSC[1:01h..1Dh] - Reserved (xxh)**

```

| 7-0 Reserved. Do not write to these registers. |
| --- |

**TSC[1:1Eh] - Headphone and Speaker Amplifier Error Control (00h)**

```

| 7-2 Reserved
 1 Reset HPL/HPR power-up bits upon short-circuit detect (0=Yes, 1=No)
 0 Reset SPL/SPR power-up bits upon short-circuit detect (0=Yes, 1=No) |
| --- |

The HPL/HPR auto-reset occurs only if TSC[1:1Fh].Bit1=1 (action=power down).

**TSC[1:1Fh] - Headphone Drivers (04h)**

```

| 7 HPL output driver (0=Powered down, 1=Powered up)
 6 HPR output driver (0=Powered down, 1=Powered up)
 5 Reserved. Write only zero to this bit.
 4-3 Output common-mode voltage (0=1.35V, 1=1.5V, 2=1.65V, 3=1.8V)
 2 Reserved. Write only 1 to this bit. (!!!)
 1 Action when short-circuit protection is enabled/detected,
 0=Limit the maximum current to the load.
 1=Power down the output driver.
 0 Short-circuit detected on the headphone driver (0=No, 1=Yes) (R) |
| --- |

**TSC[1:20h] - Class-D Speaker Amplifier (06h)**

```

| 7 Left-channel class-D output driver (0=Powered down, 1=Powered up)
 6 Right-channel class-D output driver (0=Powered down, 1=Powered up)
 5-1 Reserved. Write only the reset value (00011b) to these bits (!!!)
 0 Short-circuit is detected on the class-D driver (0=No, 1=Yes) (R) |
| --- |

Bit0 is Valid only if class-D amplifier is powered up. For short-circuit flag
sticky bit, see TSC[0:2Ch].

**TSC[1:21h] - HP Output Drivers POP Removal Settings (3Eh)**

```

| 7 If power down sequence is activated by device software power down
 using TSC[1:2Eh].Bit7 then power down DAC,
 0: simultaneously with the HP and SP amplifiers.
 1: after HP and SP amplifiers are completely powered down.
 (the latter setting is to optimize power-down POP).
 6-3 Driver power-on time (at 8.2MHz) (1=15.3us, 2=153us, 3=1.53ms,
 4=15.3ms,5=76.2ms, 6=153ms, 7=304ms, 8=610ms, 9=1.22s, 10=3.04s,
 11=6.1s, 12..15=Reserved)
 2-1 Driver ramp-up step time (8.2MHz) (0=0ms, 1=0.98ms, 2=1.95ms, 3=3.9ms)
 0 Weakly driven output common-mode voltage is generated from,
 0=resistor divider of the AVDD supply.
 1=band-gap reference. |
| --- |

**TSC[1:22h] - Output Driver PGA Ramp-Down Period Control (00h)**

```

| 7 Reserved. Write only the reset value to this bit. (USED on DSi!)
 6-4 Speaker Power-Up Wait Time (at 8.2MHz) (0=0 ms, 1=3.04 ms, 2=7.62 ms,
 3=12.2 ms, 4=15.3 ms, 5=19.8 ms, 6=24.4 ms, 7=30.5 ms)
 3-0 Reserved. Write only the reset value to these bits. |
| --- |

**TSC[1:23h] - DAC_L and DAC_R Output Mixer Routing (00h)**

```

| 7-6 DAC_L route (0=Nowhere, 1=To L-Mixer, 2=Direct to HPL, 3=Reserved)
 5 MIC input routed to the left-channel mixer amplifier (0=No, 1=Yes)
 4 AUX1 input routed to the left-channel mixer amplifier (0=No, 1=Yes)
 3-2 DAC_R route (0=Nowhere, 1=To R-Mixer, 2=Direct to HPR, 3=Reserved)
 1 AUX1 input routed to the right-channel mixer amplifier (0=No, 1=Yes)
 0 HPL driver output routed to HPR driver (for differential) (0=No, 1=Yes) |
| --- |

**TSC[1:24h] - Analog Volume to HPL (Left Headphone) (7Fh)**

**TSC[1:25h] - Analog Volume to HPR (Right Headphone) (7Fh)**

**TSC[1:26h] - Analog Volume to SPL (Left Speaker) (7Fh)**

**TSC[1:27h] - Analog Volume to SPR (Right Speaker) (7Fh)**

```

| 7 Analog volume control routed to HPx/SPx output driver (0=No, 1=Yes)
 6-0 Analog volume control gain (non-linear) (0 dB to -78 dB) |
| --- |

See Table 5-37 and Table 5-38, uh?

**TSC[1:28h] - HPL Driver (Left Headphone) (02h)**

**TSC[1:29h] - HPR Driver (Right Headphone) (02h)**

```

| 7 Reserved. Write only zero to this bit.
 6-3 HPx driver PGA (0..9 = 0dB..9dB, 10..15=Reserved)
 2 HPx driver (0=Muted, 1=Not muted)
 1 HPx driver during power down (0=Weakly driven to a common mode,
 1=High-impedance)
 0 All programmed gains to HPx have been applied (0=Not yet, 1=Yes/all) (R) |
| --- |

**TSC[1:2Ah] - SPL Driver (Left Speaker) (00h)**

**TSC[1:2Bh] - SPR Driver (Right Speaker) (00h)**

```

| 7-5 Reserved. Write only zeros to these bits.
 4-3 SPx class-D driver output stage gain (0=6dB, 1=12dB, 2=18dB, 3=24dB)
 2 SPx class-D driver (0=Muted, 1=Not muted)
 1 Reserved. Write only zero to this bit.
 0 All programmed gains to SPx have been applied (0=Not yet, 1=Yes/all) (R) |
| --- |

**TSC[1:2Ch] - HP Driver Control (00h)**

```

| 7-5 Debounce time for the headset short-circuit detection
 (0..7 = 0us, 8us, 16us, 32us, 64us, 128us, 256us)
 (when TSC[3:10h] set to 1MHz)
 4-3 DAC Performance (0=Normal, 1=Increased, 2=Reserved, 3=Further Increased)
 (increased: by increased current, further: by increased current gain)
 2 HPL output driver type (0=Headphone, 1=Lineout)
 1 HPR output driver type (0=Headphone, 1=Lineout)
 0 Reserved. Write only zero to this bit. |
| --- |

The clock used for the debounce has a clock period = debounce duration/8.

**TSC[1:2Dh] - Reserved (xxh)**

```

| 7-0 Reserved. Do not write to these registers. |
| --- |

**TSC[1:2Eh] - MICBIAS (00h)**

```

| 7 Device software power-down (0=Disabled, 1=PowerDown?-Enabled)
 6-4 Reserved. Write only zeros to these bits.
 3 Power up programmed MICBIAS (0=Only if Headset inserted, 1=Always)
 2 Reserved. Write only zero to this bit.
 1-0 MICBIAS output (0=Off, 1=2V, 2=2.5V, 3=AVDD) |
| --- |

**TSC[1:2Fh] - MIC PGA (80h)**

```

| 7 MIC PGA (0=Controlled by bits6-0, 1=Force 0dB)
 6-0 PGA in 0.5dB units (0..119 = 0..59.5dB, 120..127=Reserved) |
| --- |

**TSC[1:30h] - P-Terminal Delta-Sigma Mono ADC Channel Fine-Gain Input (00h)**

```

| 7-6 MIC to MIC PGA feed-forward (0=Off, 1=10kOhm, 2=20kOhm, 3=40kOhm)
 5-4 AUX1 to MIC PGA feed-forward (0=Off, 1=10kOhm, 2=20kOhm, 3=40kOhm)
 3-2 AUX2 to MIC PGA feed-forward (0=Off, 1=10kOhm, 2=20kOhm, 3=40kOhm)
 1-0 Reserved. Write only zeros to these bits. |
| --- |

Program Bit7-6 of registers TSC[1:30h] and TSC[1:31h] with same value. Input
impedance selection affects the microphone PGA gain. See the Analog Front End
section for details.

**TSC[1:31h] - M-Terminal ADC Input Selection (00h)**

```

| 7-6 CM to MIC PGA feed-forward (0=Off, 1=10kOhm, 2=20kOhm, 3=40kOhm)
 5-4 AUX2 to MIC PGA feed-forward (0=Off, 1=10kOhm, 2=20kOhm, 3=40kOhm)
 3-0 Reserved. Write only zeros to these bits. |
| --- |

Program Bit7-6 of registers TSC[1:30h] and TSC[1:31h] with same value. Input
impedance selection affects the microphone PGA gain. See the Analog Front End
section for details.

**TSC[1:32h] - Input CM Settings (00h)**

```

| 7 MIC input (0=Floating, 1=Connected to CM internally)
 (when not used for MIC PGA and analog bypass)
 6 AUX1 input (0=Floating, 1=Connected to CM internally)
 (when not used for MIC PGA and analog bypass)
 5 AUX2 input (0=Floating, 1=Connected to CM internally)
 (when not used for MIC PGA)
 4-1 Reserved. Write only zeros to these bits.
 0 All programmed gains to ADC have been applied (0=Not yet, 1=Yes/all) (R) |
| --- |

**TSC[1:33h..FFh] - Reserved (xxh)**

```

| 7-0 Reserved. Write only the reset value to these bits. |
| --- |

| DSi TSC[3:xxh], Touchscreen/SAR Control and TSC[FCh:xxh], Buffer |
| --- |

**TSC[3:01h] - Reserved (xxh)**

```

| 7-0 Reserved. Write only the reset value to these bits. |
| --- |

**TSC[3:02h] - SAR ADC Control 1 (00h)**

```

| 7 Stop (0=Normal mode, 1=Stop conversion and power down SAR ADC)
 6-5 SAR ADC resolution (0=12bit, 1=8bit, 2=10bit, 3=12bit)
 4-3 SAR ADC clock divider
 0 = Div1 (use for 8bit resolution mode only) (This divider is only
 for the conversion clock generation, not for other logic)
 1 = Div2 (use for 8bit/10bit resolution mode only)
 2 = Div4 (recommended for better performance in 8bit/10bit mode)
 3 = Div8 (recommended for better performance in 12bit mode)
 (See Figure 5-40, uh?)
 2 Filter used for on-chip data averaging (0=Mean, 1=Median) (if enabled)
 1-0 On-chip data averaging for mean/median filter
 0 = On-chip data averaging disabled
 1 = 4-data averaging (mean), or 5-data averaging (median)
 2 = 8-data averaging (mean), or 9-data averaging (median)
 3 = 16-data averaging (mean), or 15-data averaging (median) |
| --- |

**TSC[3:03h] - SAR ADC Control 2 (00h)**

```

| 7 Conversions controlled,
 0 = Host-controlled conversions
 1 = Self-controlled conversions for touch screen based on pen touch
 6 Reserved. Write only zero to this bit.
 5-2 Conversion Mode
 0 = No scan
 1 = Scan X/Y ;\Even in host-controlled mode ;\until either
 2 = Scan X/Y/Z1/Z2 ;/ ; pen is lifted,
 3 = Scan X ;\ ; or a stop bit
 4 = Scan Y ; Only in self-controlled mode ; TSC[3:02h].Bit7
 5 = Scan Z1/Z2 ;/ ;/is sent
 6 = VBAT measurement
 7 = AUX2 measurement
 8 = AUX1 measurement
 9 = Auto scan. Sequence used is AUX1, AUX2, VBAT.
 Each of these inputs can be enabled or disabled independently
 using TSC[3:13h], and with that sequence is modified accordingly.
 Scan continues until stop bit TSC[3:02h].Bit7 is sent,
 or Bit5-2 of this register are changed.
 10 = TEMP1 measurement
 11 = Port scan: AUX1, AUX2, VBAT
 12 = TEMP2 measurement
 13-15 = Reserved. Do not write these sequences to these bits.
 1-0 Interrupt pin (GPIO1 or GPIO2 pin)
 0 = PEN-interrupt /PENIRQ (active low)
 1 = Data-available /DATA_AVA (active low)
 2 = PEN-interrupt PENIRQ and Data-available DATA_AVA (active high)
 3 = Reserved |
| --- |

**TSC[3:04h] - Precharge and Sense (00h)**

```

| 7 Pen touch detection (0=Enabled, 1=Disabled)
 6-4 Precharge time before touch detection
 (0..7 = 0.25us, 1us, 3us, 10us, 30us, 100us, 300us, 1000us)
 (when TSC[3:11h] set to 8MHz)
 3 Reserved. Write only zero to this bit.
 2-0 Sense time during touch detection
 (0..7 = 1us, 2us, 3us, 10us, 30us, 100us, 300us, 1000us)
 (when TSC[3:11h] set to 8MHz) |
| --- |

**TSC[3:05h] - Panel Voltage Stabilization (00h)**

```

| 7-6 SAR comparator bias current (0=Normal, 1..3=Increase by 25%, 50%, 100%)
 (use Increase to support higher conversion clock)
 5 Sample duration (0=Default, 1=Doubled; for higher impedance)
 4-3 Reserved. Write only zeroes to these bits.
 2-0 Panel voltage stabilization time before conversion
 (0..7 = 0.25us, 1us, 3us, 10us, 30us, 100us, 300us, 1000us)
 (when TSC[3:11h] set to 8MHz) |
| --- |

**TSC[3:06h] - Voltage Reference (20h)**

```

| 7 Reference for Non-touch-screen Measurement (0=External, 1=Internal)
 6 Internal reference voltage (0=1.25V, 1=2.5V)
 5 Internal reference powered (0=Always, 1=Only during conversion)
 4 Reserved
 3-2 Reference Stabilization Time before Conversion
 (0=0us, 1=100us, 2=500us, 3=1ms) (when TSC[3:11h] set to 8MHz)
 1 Reserved
 0 Battery measurement input (0=VBAT<=VREF, 1=VBAT=BAT) |
| --- |

**TSC[3:07h,08h] - Reserved (xxh)**

```

| 7-0 Reserved. Write only the reset value to these bits. |
| --- |

**TSC[3:09h] - Status Bits 1 (40h) (R)**

```

| 7 Pen Touch detected (0=Not detected, 1=Detected) (R)
 6 ADC Ready (0=Busy, 1=Ready) (R)
 5 New data is available (0=None, 1=Yes) (R)
 4 Reserved. Write only the reset value to this bit.
 3 New X data is available (0=None, 1=Yes) (R)
 2 New Y data is available (0=None, 1=Yes) (R)
 1 New Z1 data is available (0=None, 1=Yes) (R)
 0 New Z2 data is available (0=None, 1=Yes) (R) |
| --- |

Bit0-3 and Bit5 are not valid for the buffer mode.

Bit0-3 are cleared after reading the corresponding data.

Bit5 is cleared after completely reading ALL data.

**TSC[3:0Ah] - Status Bits 2 (00h) (R)**

```

| 7 New AUX1 data is available (0=None, 1=Yes) (R)
 6 New AUX2 data is available (0=None, 1=Yes) (R)
 5 New VBAT data is available (0=None, 1=Yes) (R)
 4-2 Reserved. Write only zeros to these bits.
 1 New TEMP1 data is available (0=None, 1=Yes) (R)
 0 New TEMP2 data is available (0=None, 1=Yes) (R) |
| --- |

Bit0-1 and Bit5-7 are not valid for the buffer mode.

Bit0-1 and Bit5-7 are cleared after reading the corresponding data.

**TSC[3:0Bh,0Ch] - Reserved (xxh)**

```

| 7-0 Reserved. Write only the reset value to these bits. |
| --- |

**TSC[3:0Dh] - Buffer Mode (03h)**

```

| 7 Buffer Mode Enable (0=Disabled, 1=Enabled)
 (when disabled: RDPTR/WRPTR/TGPTR are set to their default values)
 6 Buffer Mode Type (0=Countinuos-conversion, 1=Single-shot)
 5-3 Trigger level for conversion "(N+1)*8*number of converted data"
 0..7 = (8..64)*number of converted data
 uh, does "X*number of converted data" mean "after X conversions"?
 2 Reserved
 1 Buffer Full (0=No, 1=Full; contains 64 unread converted data) (R)
 0 Buffer Empty (0=No, 1=Empty; contains 0 unread converted data) (R) |
| --- |

However, in DSi this register is unused, and DSi does instead use TSC[3:0Eh].

**TSC[3:0Eh] - Reserved / Undocumented (read by DSi for Pen Down Test) (0Fh)**

```

| 7-0 Reserved. Write only the reset value to these bits. |
| --- |

However, in DSi this is used as so... (seems to resemble TSC[3:0Dh])...

```

| 7 Undoc Enable (0=Disabled, 1=Enabled) (R/W)
 6 Undoc Whatever (0=Normal) (R/W)
 5-3 Undoc Whatever (5=Normal) (R/W)
 2 Undoc Unused (?)
 1 Undoc Pendown/DataAvailable? (R?)
 0 Undoc Unused (R?) |
| --- |

**TSC[3:0Fh] - Scan Mode Timer (40h)**

```

| 7 Programmable delay for Touch-screen measurement (0=Disable, 1=Enable)
 6-4 Programmable interval timer delay
 (0..7 = 8ms, 1ms, 2ms, 3ms, 4ms, 5ms, 6ms, 7ms)
 (when TSC[3:10h] set to 1MHz)
 3 Programmable delay for Non-touch-screen auto measurement (1=Enable)
 2-0 Programmable interval timer delay (0..7 = 1.12min, 3.36min,
 5.59min, 7.83min, 10.01min, 12.30min, 14.54min, 16.78min)
 (uh, what is that? minutes? minimum? or what?)
 (when TSC[3:10h] set to 1MHz) |
| --- |

These delays are from the end of one data set of conversion to the start of
another new data set of conversion.

Bit7: This interval timer mode is for all self-controlled modes. For
host-controlled mode, it is valid only for (X/Y) or (X/Y/Z1/Z2) conversions.

**TSC[3:10h] - Scan Mode Timer Clock (81h)**

```

| 7 Clock used for Programmable Delay Timer (0=Internal Osc/8, 1=Ext. MCLK)
 6-0 MCLK Divider to Generate 1-MHz Clock for the Programmable Delay Timer
 (1..127=Div1..127, or 0=Div128) |
| --- |

The timings marked "(when TSC[3:10h] set to 1MHz)" are assuming the MCLK
division result to be 1MHz (1us). Other divider settings will cause those
timings to change. Using Internal Osc/8 (Bit7=0) results in 1.025MHz (0.97us),
which is almost same as the "1MHz" timings (internal osc isn't too accurate
though).

Bit7: External clock is used only to control the delay programmed between the
conversions and not used for doing the actual conversion. This is supported to
get an accurate delay, because the internal oscillator frequency varies from
device to device.

**TSC[3:11h] - SAR ADC Clock (81h)**

```

| 7 Clock used for SAR ADC and TSC FSM (0=Internal Osc/1, 1=External MCLK)
 6-0 MCLK Divider for the SAR (min 40ns) (1..127=Div1..127, or 0=Div128) |
| --- |

The timings marked "(when TSC[3:11h] set to 8MHz)" are assuming the MCLK
division result to be 8MHz (125ns). Other divider settings will cause those
timings to change. For the SAR unit, the division result should be max 25MHz
(min 40ns). Using Internal Osc/1 (Bit7=0) results in 8.2MHz (122ns), which is
almost same as the "8MHz" timings (internal osc isn't too accurate though).

**TSC[3:12h] - Debounce Time for Pen-Up Detection (00h)**

```

| 7 Interface used for the buffer data reading (0=SPI, 1=I2C)
 6 SAR/buffer data update is,
 0: held automatically (to avoid simultaneous buffer read and write
 operations) based on internal detection logic.
 1: held using software control and TSC[3:12h].Bit5.
 5 SAR/buffer data update is (only if above Bit6=1),
 0: enabled all the time
 1: stopped so that user can read the last updated data
 without any data corruption.
 4-3 Reserved. Write only zeros to these bits.
 2-0 Pen-touch removal detection with debounce
 (0..7 = 0us, 8us, 16us, 32us, 64us, 128us, 256us, 512us)
 (when TSC[3:10h] set to 1MHz) |
| --- |

The clock used for the debounce has a clock period = debounce duration/8.

**TSC[3:13h] - Auto AUX Measurement Selection (00h)**

```

| 7 Auto AUX1 measurement during auto non-touch screen scan (0=Off, 1=On)
 6 Auto AUX2 measurement during auto non-touch screen scan (0=Off, 1=On)
 5 Auto VBAT measurement during auto non-touch screen scan (0=Off, 1=On)
 4 Auto TEMP measurement during auto non-touch screen scan (0=Off, 1=On)
 3 TEMP Measurement (0=Use TEMP1, 1=Use TEMP2)
 2 AUX1 Usage (0=Voltage measurement, 1=Resistance measurement)
 1 AUX2 Usage (0=Voltage measurement, 1=Resistance measurement)
 0 Resistance measurement bias (0=Internal bias, 1=External bias) |
| --- |

**TSC[3:14h] - Touch-Screen Pen Down (00h)**

```

| 7-3 Reserved
 2-0 Debounce Time for Pen-Down Detection
 (0..7 = 0us, 64us, 128us, 256us, 512us, 1024us, 2048us, 4096us)
 (when TSC[3:10h] set to 1MHz) |
| --- |

The clock used for the debounce has a clock period = debounce duration/8.

**TSC[3:15h] - Threshold Check Flags Register (00h) (R)**

```

| 7-6 Reserved. Write only zeros to these bits.
 5 AUX1 Maximum (0=Inrange, 1=Exceeds Limit; Equal/Above MAX)
 4 AUX1 Minimum (0=Inrange, 1=Exceeds Limit; Equal/Below MIN)
 3 AUX2 Maximum (0=Inrange, 1=Exceeds Limit; Equal/Above MAX)
 2 AUX2 Minimum (0=Inrange, 1=Exceeds Limit; Equal/Below MIN)
 1 TEMP Maximum (0=Inrange, 1=Exceeds Limit; Equal/Above MAX)
 0 TEMP Minimum (0=Inrange, 1=Exceeds Limit; Equal/Below MIN) |
| --- |

Sticky flag bIts. These are read-only bits. They are automatically cleared once
they are read and are set only if the source trigger occurs again.

**TSC[3:16h,17h] - AUX1 Maximum Value Check MSB,LSB (0000h)**

**TSC[3:18h,19h] - AUX1 Minimum Value Check MSB,LSB (0000h)**

**TSC[3:1Ah,1Bh] - AUX2 Maximum Value Check MSB,LSB (0000h)**

**TSC[3:1Ch,1Dh] - AUX2 Minimum Value Check MSB,LSB (0000h)**

**TSC[3:1Eh,1Fh] - Temperature(TEMP1/TEMP2) Maximum Value Check MSB,LSB (0000h)**

**TSC[3:20h,21h] - Temperature(TEMP1/TEMP2) Minimum Value Check MSB,LSB (0000h)**

```

| 15-13 Reserved
 12 Threshold check (0=Disabled, 1=Enabled)
 (valid for auto/non-auto scan measurement).
 11-0 Threshold code (12bit) |
| --- |

**TSC[3:2Ah,2Bh] - Touchscreen X-Coordinate Data MSB,LSB (0000h) (R)**

**TSC[3:2Ch,2Dh] - Touchscreen Y-Coordinate Data MSB,LSB (0000h) (R)**

**TSC[3:2Eh,2Fh] - Touchscreen Z1-Pressure Register MSB,LSB (0000h) (R)**

**TSC[3:30h,31h] - Touchscreen Z2-Pressure Register MSB,LSB (0000h) (R)**

**TSC[3:36h,37h] - AUX1 Data MSB,LSB (0000h) (R)**

**TSC[3:38h,39h] - AUX2 Data MSB,LSB (0000h) (R)**

**TSC[3:3Ah,3Bh] - VBAT Data MSB,LSB (0000h) (R)**

**TSC[3:42h,43h] - Temperature TEMP1 Data Register MSB,LSB (0000h) (R)**

**TSC[3:44h,45h] - Temperature TEMP2 Data Register MSB,LSB (0000h) (R)**

```

| 15-0 Data... but, seems to be always zero on DSi? |
| --- |

Going by the datasheet, these registers should contain current sample values,
but they seem to be always zero on DSi.

Instead, the samples for the currently selected Conversion Mode can be read
from the buffer at TSC[FCh:01h,02h], using the buffer should also ensure that
MSB/LSB-pairs won't change within 16bit reads.

**TSC[3:22h...29h] - Reserved (xxh)**

**TSC[3:32h...35h] - Reserved (xxh)**

**TSC[3:3Ch...41h] - Reserved (xxh)**

**TSC[3:46h...7Fh] - Reserved (xxh)**

```

| 7-0 Reserved. Write only the reset value to these bits. |
| --- |

**TSC[FCh:01h,02h] - Buffer Mode Data MSB,LSB (xxxxh) (R)**

```

| 15 Ring-buffer Full (1=All 64 entries are unread)
 14 Ring-buffer Empty (1=All 64 entries are read)
 13 Reserved (uh?)
 12 Data ID (0=X/Z1/BAT/AUX2, 1=Y/Z2/AUX1/TEMP)
 11-0 Converted data (12bit), read from "RDPTR" ring-buffer location |
| --- |

Note: One can/must read multiple words from index 01h (the index will somewhat
automatically toggle between 01h and 02h in that case (or perhaps it does
increment to 03h and up, but those indices mirror to 01h and 02h)).

**TSC[FCh:03h..7Fh] - Reserved (xxh) (actually mirrors of TSC[FCh:01h,02h])**

```

| 7-0 Reserved. Write only the reset value to these bits. |
| --- |

These are somewhat mirrors of the buffer at TSC[FCh:01h,02h]. Trying to select
index=03h starts reading with the 1st buffer byte (ie. it won't start at the
3rd byte).

| DSi TSC[04h..05h:xxh], ADC Digital Filter Coefficient RAM |
| --- |

Default values shown for this page only become valid 100us following a hardware
or software reset.

**TSC[04h-05h:xxh] - ADC Coefficient RAM (126 x 16bit)**

Coefficients are signed 16bit (-32,768..+32,767), each occupying 2 bytes
(MSB,LSB).

The MSB should always be written first, immediately followed by the LSB (even
if only the MSB or LSB portion of the coefficient changes, both registers
should be written in this sequence).

```

| ADC miniDSP ADC FIR Filter Special
 Coefficients Coefficients Coefficients
 TSC[4:00h] Page Select - -
 TSC[4:01h] Reserved - -
 TSC[4:02h..07h] C1..C3 - N0,N1,D1 for AGC LPF
 (first-order IIR, used
 as averager to detect level)
 TSC[4:08h..0Dh] C4..C6 - N0,N1,D1 for ADC-programmable
 first-order IIR
 TSC[4:0Eh..17h] C7..C11 FIR0..FIR4 N0,N1,N2,D1,D2 for ADC Biquad A
 TSC[4:18h..21h] C12..C16 FIR5..FIR9 N0,N1,N2,D1,D2 for ADC Biquad B
 TSC[4:22h..2Bh] C17..C21 FIR10..FIR14 N0,N1,N2,D1,D2 for ADC Biquad C
 TSC[4:2Ch..35h] C22..C26 FIR15..FIR19 N0,N1,N2,D1,D2 for ADC Biquad D
 TSC[4:36h..3Fh] C27..C31 FIR20..FIR24 N0,N1,N2,D1,D2 for ADC Biquad E
 TSC[4:40h..7Fh] C32..C63 - -
 TSC[5:00h] Page Select - -
 TSC[5:01h] Reserved - -
 TSC[5:02h..7Fh] C65..C127 - - |
| --- |

**Multi-order FIR Filter (Finite Impulse Response)**

```

| input[n] ---o--MUL(F0)-->ADD-----------------> output[n]
 | ^
 input[n-1] | ;\
 | | ; first order FIR
 o--MUL(F1)-->ADD ;/
 | ^
 input[n-2] | ;\
 | | ; second order FIR
 o--MUL(F2)-->ADD ;/
 : ^
 : : ;-further order's |
| --- |

**First-order IIR Filter (Infinite Impulse Response)**

```

| input[n] ---o--MUL(N0)-->ADD-------------o---> output[n]
 | ^ |
 input[n-1] | output[n-1] ;\
 | | | ; first order IIR
 o--MUL(N1)-->ADD<--MUL(-D1)--o ;/ |
| --- |

**Biquad Filter (IIR with 1st and 2nd order)**

```

| input[n] ---o--MUL(N0)-->ADD-------------o---> output[n]
 | ^ |
 input[n-1] | output[n-1] ;\
 | | | ; first order IIR
 o--MUL(N1)-->ADD<--MUL(-D1)--o ;/
 | ^ |
 input[n-2] | output[n-2] ;\
 | | | ; second order IIR
 o--MUL(N2)-->ADD<--MUL(-D2)--o ;/ |
| --- |

| DSi TSC[08h..0Fh:xxh], DAC Digital Filter Coefficient RAM |
| --- |

Default values shown for this page only become valid 100us following a hardware
or software reset.

**TSC[08h:01h] - DAC Coefficient RAM Control (00h)**

```

| 7-4 Reserved. Write only the reset value.
 3 DAC miniDSP generated flag for toggling MSB of coefficient RAM address
 (only used in non-adaptive mode) (R)
 2 DAC Adaptive Filtering in DAC miniDSP (0=Disabled, 1=Enabled) (R/W)
 1 DAC Adaptive Filter Buffer Control Flag (R)
 aka DAC Coefficient Buffers in adaptive filter mode
 0: miniDSP accesses Buffer A,
 external control interface (=the user?) accesses Buffer B
 1: miniDSP accesses Buffer B,
 external control interface (=the user?) accesses Buffer A
 0 DAC Adaptive Filter Buffer Switch Control (R/W)
 0: DAC coefficient buffers will not be switched at next frame boundary
 1: DAC coefficient buffers will be switched at next frame boundary
 (only if adaptive filtering mode is enabled)
 This bit will self-clear on switching. |
| --- |

**TSC[08h..0Bh:xxh] - DAC Coefficient RAM, DAC Buffer A (252 x 16bit)**

Coefficients are signed 16bit (-32,768..+32,767), each occupying 2 bytes
(MSB,LSB).

The MSB should always be written first, immediately followed by the LSB (even
if only the MSB or LSB portion of the coefficient changes, both registers
should be written in this sequence).

```

| DAC miniDSP Special
 (DAC Buffer A) DAC-programmable
 Coefficient Coefficient
 TSC[8:00h] Page Select -
 TSC[8:01h] Control - (see above)
 TSC[8:02h..0Bh] C1..C5 N0,N1,N2,D1,D2 for Left Biquad A ;N0=7FFFh
 TSC[8:0Ch..15h] C6..C10 N0,N1,N2,D1,D2 for Left Biquad B ;N1,N2,D1,
 TSC[8:16h..1Fh] C11..C15 N0,N1,N2,D1,D2 for Left Biquad C ; D2=0
 TSC[8:20h..29h] C16..C20 N0,N1,N2,D1,D2 for Left Biquad D
 TSC[8:2Ah..33h] C21..C25 N0,N1,N2,D1,D2 for Left Biquad E
 TSC[8:34h..3Dh] C26..C30 N0,N1,N2,D1,D2 for Left Biquad F
 TSC[8:3Eh..3Fh] C31 -
 TSC[8:40h..41h] C32 for 3D PGA for PRB_P23, PRB_P24 and PRB_P25
 TSC[8:42h..4Bh] C33..C37 N0,N1,N2,D1,D2 for Right Biquad A
 TSC[8:4Ch..55h] C38..C42 N0,N1,N2,D1,D2 for Right Biquad B
 TSC[8:56h..5Fh] C43..C47 N0,N1,N2,D1,D2 for Right Biquad C
 TSC[8:60h..69h] C48..C52 N0,N1,N2,D1,D2 for Right Biquad D
 TSC[8:6Ah..73h] C53..C57 N0,N1,N2,D1,D2 for Right Biquad E
 TSC[8:74h..7Dh] C58..C62 N0,N1,N2,D1,D2 for Right Biquad F
 TSC[8:7Eh..7Fh] C63 -
 TSC[9:00h] Page Select -
 TSC[9:01h] Reserved - (do not write to this register)
 TSC[9:02h..07h] C65..C67 N0,N1,D1 for Left first-order IIR
 TSC[9:08h..0Dh] C68..C70 N0,N1,D1 for Right first-order IIR
 TSC[9:0Eh..13h] C71..C73 N0,N1,D1 for DRC first-order high-pass filter
 TSC[9:14h..19h] C74..C76 N0,N1,D1 for DRC first-order low-pass filter
 TSC[9:1Ah..7Fh] C77..C127 -
 TSC[A:00h] Page Select -
 TSC[A:01h] Reserved - (do not write to this register)
 TSC[A:02h..7Fh] C129..C191 -
 TSC[B:00h] Page Select -
 TSC[B:01h] Reserved - (do not write to this register)
 TSC[B:02h..7Fh] C193..C255 - |
| --- |

**TSC[0Ch..0Fh:xxh] - DAC Coefficient RAM, DAC Buffer B (252 x 16bit)**

This is essentially same as above Buffer A. But it's unclear if Buffer B is
having the same special Biquad/3DPGA/IIR/DRC functions (the official datasheet
doesn't mention them, but it does specify the initial reset values same as for
Buffer A, ie. with value 7FFFh for the locations that correspond to "N0"
coefficients, which is suggesting that those special functions are present in
Buffer B, too).

```

| DAC miniDSP Special
 (DAC Buffer A) DAC-programmable
 Coefficient Coefficient
 TSC[C:02h..0Bh] C1..C5 Unknown ;\
 TSC[C:0Ch..15h] C6..C10 Unknown ;
 TSC[C:16h..1Fh] C11..C15 Unknown ; maybe Left Biquad A..F
 TSC[C:20h..29h] C16..C20 Unknown ; as for Buffer A
 TSC[C:2Ah..33h] C21..C25 Unknown ;
 TSC[C:34h..3Dh] C26..C30 Unknown ;/
 TSC[C:3Eh..3Fh] C31 -
 TSC[C:40h..41h] C32 Unknown maybe 3D PGA as for Buffer A
 TSC[C:42h..4Bh] C33..C37 Unknown ;\
 TSC[C:4Ch..55h] C38..C42 Unknown ;
 TSC[C:56h..5Fh] C43..C47 Unknown ; maybe Right Biquad A..F
 TSC[C:60h..69h] C48..C52 Unknown ; as for Buffer A
 TSC[C:6Ah..73h] C53..C57 Unknown ;
 TSC[C:74h..7Dh] C58..C62 Unknown ;/
 TSC[C:7Eh..7Fh] C63 -
 TSC[D:00h] Page Select -
 TSC[D:01h] Reserved - (do not write to this register)
 TSC[D:02h..07h] C65..C67 Unknown ;\
 TSC[D:08h..0Dh] C68..C70 Unknown ; maybe IIR and DRC
 TSC[D:0Eh..13h] C71..C73 Unknown ; as for Buffer A
 TSC[D:14h..19h] C74..C76 Unknown ;/
 TSC[D:1Ah..7Fh] C77..C127 -
 TSC[E:00h] Page Select -
 TSC[E:01h] Reserved - (do not write to this register)
 TSC[E:02h..7Fh] C129..C191 -
 TSC[F:00h] Page Select -
 TSC[F:01h] Reserved - (do not write to this register)
 TSC[F:02h..7Fh] C193..C255 - |
| --- |

| DSi TSC[20h..2Bh:xxh], TSC[40h..5Fh:xxh] ADC/DAC Instruction RAM |
| --- |

**TSC[20h..2Bh:xxh] - ADC DSP Engine Instruction RAM (384 x 24bit)**

ADC miniDSP Instructions are 20bit, each occupying 3 bytes (MSB,MID,LSB) (with
dummy padding in upper 4bit of MSB).

```

| TSC[20h..2Bh:00h] Page Select
 TSC[20h..2Bh:01h] Reserved
 TSC[20h:02h...61h] ADC Instructions 0...31
 TSC[21h:02h...61h] ADC Instructions 32...63
 TSC[22h:02h...61h] ADC Instructions 64...95
 TSC[23h:02h...61h] ADC Instructions 96...127
 TSC[24h:02h...61h] ADC Instructions 128...159
 TSC[25h:02h...61h] ADC Instructions 160...191
 TSC[26h:02h...61h] ADC Instructions 192...223
 TSC[27h:02h...61h] ADC Instructions 224...255
 TSC[28h:02h...61h] ADC Instructions 256...287
 TSC[29h:02h...61h] ADC Instructions 288...319
 TSC[2Ah:02h...61h] ADC Instructions 320...351
 TSC[2Bh:02h...61h] ADC Instructions 352...383
 TSC[20h..2Bh:62h..7Fh] Reserved |
| --- |

**TSC[40h..5Fh:xxh] - DAC DSP Engine Instruction RAM (1024 x 24bit)**

DAC miniDSP Instructions are 24bit (uh, unlike 20bit ADC ones?), each occupying
3 bytes (MSB,MID,LSB).

```

| TSC[40h..5Fh:00h] Page Select
 TSC[40h..5Fh:01h] Reserved
 TSC[40h:02h...61h] DAC Instructions 0...31
 TSC[41h:02h...61h] DAC Instructions 32...63
 TSC[42h:02h...61h] DAC Instructions 64...95
 TSC[43h:02h...61h] DAC Instructions 96...127
 TSC[44h:02h...61h] DAC Instructions 128...159
 TSC[45h:02h...61h] DAC Instructions 160...191
 TSC[46h:02h...61h] DAC Instructions 192...223
 TSC[47h:02h...61h] DAC Instructions 224...255
 TSC[48h:02h...61h] DAC Instructions 256...287
 TSC[49h:02h...61h] DAC Instructions 288...319
 TSC[4Ah:02h...61h] DAC Instructions 320...351
 TSC[4Bh:02h...61h] DAC Instructions 352...383
 TSC[4Ch:02h...61h] DAC Instructions 384...415
 TSC[4Dh:02h...61h] DAC Instructions 416...447
 TSC[4Eh:02h...61h] DAC Instructions 448...479
 TSC[4Fh:02h...61h] DAC Instructions 480...511
 TSC[50h:02h...61h] DAC Instructions 512...543
 TSC[51h:02h...61h] DAC Instructions 544...575
 TSC[52h:02h...61h] DAC Instructions 576...607
 TSC[53h:02h...61h] DAC Instructions 608...639
 TSC[54h:02h...61h] DAC Instructions 640...671
 TSC[55h:02h...61h] DAC Instructions 672...703
 TSC[56h:02h...61h] DAC Instructions 704...735
 TSC[57h:02h...61h] DAC Instructions 736...767
 TSC[58h:02h...61h] DAC Instructions 768...799
 TSC[59h:02h...61h] DAC Instructions 800...831
 TSC[5Ah:02h...61h] DAC Instructions 832...863
 TSC[5Bh:02h...61h] DAC Instructions 864...895
 TSC[5Ch:02h...61h] DAC Instructions 896...927
 TSC[5Dh:02h...61h] DAC Instructions 928...959
 TSC[5Eh:02h...61h] DAC Instructions 960...991
 TSC[5Fh:02h...61h] DAC Instructions 992...1023
 TSC[40h..5Fh:62h..7Fh] Reserved |
| --- |

Unknown if the Instruction RAM is implemented, the datasheets says that the RAM
is R/W, but it seems to be always zero on DSi/3DS.

The miniDSP instruction set isn't officially documented anywhere. Texas
Instruments has released a tool called AIC PurePath Studio, the tool supports
graphical drag-and-drop prgrammaning, and it's said to be also supporting
assembly code (perhaps also including opcode specs... it isn't quite clear how
to download that tool though).