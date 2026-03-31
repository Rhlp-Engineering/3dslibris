# Testpoints

> Source: https://problemkaputt.de/gbatek.htm
> Section: Testpoints

3DS Testpoints 
| |
| --- |

 **TP Testpoints for Old 3DS (Mainboard "C/CTR-CPU-01")**
 
```

| TP1 External Supply Ground (GND) (P2.pin2+3+4+6) (near charger)
 TP2 Supply +1.2V (for cpu) (CL5) (near charger)
 TP3 Supply +3.3V (for cpu,wifi,irda) (CL9+CP10) (near powerman)
 TP4 External Supply 4.6V (P2.pin1+5) (0V when disconnected) (near charger)
 TP5 Supply +1.8V (for cpu,wifi,etc,etc) (CL6) (near mcu)
 TP6 Supply +2.8V (for cameras) (CL11) (near powerman)
 TP7 Charger IC U7.pin7 /PGOOD (external supply power good) (near charger)
 TP8 Charger IC U7.pin10+11 OUT Supply (to system) (near charger)
 TP9 4.03V with charger, 3.5V without (always on) (near start button)
 TP10 Charger IC U7.pin9 /CHG charging (near charger)
 TP11 Charger IC U7.pin4 /CE charge enable (near charger)
 TP12 4.18V with charger, 3.5V without (wired to U13.pin1+6)(near powerman)
 TP13 SD/MMC Slot Supply (P14.pin17+19) (CL12) (near sd slot)
 TP14 3.30V ? (near AIC)
 TP15 N/A ? (?)
 TP16 NDS Cart Slot Supply (P1.pin8) (CL13) (near powerman)
 TP17 ...? (near button y)
 TP18 ...? (near powerman)
 TP19 0.00V ? (near powerman)
 TP20 N/A ? (?)
 TP21 Standby Supply +1.8V (for rtc) (same as TP79) (CL7) (near home butt)
 TP22 Battery Supply Plus (P4.plus) (via F2 to TP259) (near powerman)
 TP23 Battery type/detect/alert? (P4.center) (typ=0.01V) (near mcu)
 TP24 Upper Screen VCOM MCU[03h] (0.95V .. 3.15V) P6.pin3+21 (near gyro)
 TP25 ... GNDed? Battery or so? (CL19?) (near powerman)
 TP26 Lower Screen VCOM MCU[04h] (0.95V .. 3.15V) P8.pin32 (near eMMC)
 TP27 Parallax Logic PWM (3.3V PWM) (near AIC)
 TP28 Supply 5.1V for CAM+3D LEDs? (P7.pin12+19) (CL14) (near powerman)
 TP29 Parallax Logic Enable (0V=off, 3.3V=on) (near AIC)
 TP30 Parallax Power /PWM (P7.pin6) (5.0V PWM inverted) (near button y)
 TP31 Parallax Power PWM (P7.pin5) (5.0V PWM) (near eMMC)
 TP32 Supply +5.0V (for both screens) (CL15) (near button b)
 TP33 I2C Bus MCU SDA ;\for FuelGauge, Accel, Powerman, Tsc? (near powerman)
 TP34 I2C Bus MCU SCL ;/ (1.8V) (near powerman)
 TP35 Upper screen AVDD 3.43V (P6.pin22) (near sd slot)
 TP36 Supply +10.0V (for both screens) (near AIC)
 TP37 Lower screen AVDD 3.70V (P8.pin33) (near AIC)
 TP38 Fuel Gauge: Battery Voltage Input (CELL) (4.07V) (near powerman)
 TP39 Fuel Gauge: Power Supply (VDD) (4.07V) (near powerman)
 TP40 Supply -5.0V (for both screens) (near button b)
 TP41 Lower Backlight + (P12.Pin3+4) (CL8) (lower left edge)
 TP42 Lower Backlight - (P12.Pin1+2) (lower left edge)
 TP43 Upper Backlight? P7.pin3 (?V) (via C35 to CL17) (lower right edge)
 TP44 Upper Backlight? P7.pin2 (0.44V) (CL16) (lower right edge)
 TP45 Upper Backlight? P7.pin4 (0.44V) (CL17) (near start)
 TP46 Microphone P10.pin3 ;\what is that? plus/minus? or left/right?
 TP47 Microphone P10.pin2 ;/ (reportedly TP46 = enable or 3.2V supply??)
 TP48 Headphone P3.pin1 Audio Ground? (GNDed when plug inserted)
 TP49 Headphone P3.pin4 Insert Detect (GNDed when plug inserted) (Q3 to CL20)
 TP50 Headphone P3.pin2 Audio Left
 TP51 Headphone P3.pin3 Audio Right
 TP52 Button Power (SW1) (near headphone)
 TP53 Accelerometer INT1 U9.pin11 (near select)
 TP54 Accelerometer INT2 U9.pin9 (near select)
 TP55 Button Home (SW2)
 TP56 DL5 Notify RGB LED green
 TP57 GNDed via 0 ohm EM7 (near headphone)
 TP58 Button Wifi (SW3)
 TP59 DL1 Charge LED orange (front right)
 TP60 3D LED green (P7.pin20) (near MCU)
 TP61 Speaker R- (/PWM) (P7.pin15+16) (near AIC)
 TP62 Speaker R+ (PWM) (P7.pin13+14) (near AIC)
 TP63 ...? high 1.8v (near MCU)
 TP64 Speaker L- (/PWM) (P7.pin7+8) (near AIC)
 TP65 Speaker L+ (PWM) (P7.pin9+10) (near AIC)
 TP66 Camera LED pink (P7.pin11) (near MCU)
 TP67 DL2 Power LED blue (near power button)
 TP68 Circle Pad Y (P11.pin4) (0.2V=Down, 1.5V=Up)
 TP69 Circle Pad X (P11.pin2) (0.2V=Left, 1.5V=Right)
 TP70 DL3 Power LED low/red (near power button)
 TP71 DL4 Wifi button LED yellow (near wifi button)
 TP72 3D Slider (P7.pin18) (0V=min, 1.8V=max) (CL47) (near MCU)
 TP73 Volume Slider (P17.pin2) (0V=max, 1.8V=min) (CL48) (near MCU)
 TP74 MCU Backdoor /RESET ;\
 TP75 MCU Backdoor FLMD0 ; PCB Side B
 TP76 MCU Backdoor TOOL1 ; (under green
 TP77 MCU Backdoor TOOL0 ; solder stop)
 TP78 MCU Backdoor GND ;
 TP79 MCU Backdoor PVDD18 (same as TP21) (CL7) ;/
 TP80 Button Start (SW4)
 TP81 Button Select (SW5)
 TP82 Button R (P15.pin4)
 TP83 Button L (P16.pin3)
 TP84 ...? high 1.8v (right of (Nintendo) logo)
 TP85 Button Up (SW6)
 TP86 Button X (SW7)
 TP87 Button Left (SW8)
 TP88 Button Y (SW10)
 TP89 Button A (SW11)
 TP90 Button Right (SW9)
 TP91 Button Down (SW12)
 TP92 Button B (SW13)
 TP93 SD/MMC Slot /Unlock (P14.pin8) (CL50)
 TP94 SD/MMC Slot /Insert (P14.pin10) (CL49)
 TP95 NDS Slot CLK (P1.pin2) (near AIC)
 TP96 SD/MMC Slot CMD (P14.pin14, via 57 ohm)
 TP97 SD/MMC Slot Data3 (P14.pin18)
 TP98 SD/MMC Slot Data2 (P14.pin20)
 TP99 NDS Slot /SaveCS (P1.pin6)
 TP100 SD/MMC Slot Data1 (P14.pin2)
 TP101 SD/MMC Slot Data0 (P14.pin4)
 TP102 NDS Slot IRQ (P1.pin7)
 TP103 SD/MMC Slot CLK (P14.pin16)
 TP104 NDS Slot D3 (P1.pin12)
 TP105 NDS Slot D4 (P1.pin13)
 TP106 NDS Slot D7 (P1.pin16)
 TP107 NDS Slot /RomCS (P1.pin4)
 TP108 NDS Slot /Reset (P1.pin5)
 TP109 NDS Slot /Insert (P1.pin18)
 TP110 NDS Slot D0 (P1.pin9)
 TP111 NDS Slot D1 (P1.pin10)
 TP112 NDS Slot D2 (P1.pin11)
 TP113 NDS Slot D5 (P1.pin14)
 TP114 NDS Slot D6 (P1.pin15)
 TP115 SPI Bus0 CLK ;\ (CL53,CL23) (near aic)
 TP116 SPI Bus0 MISO ; (CL24) (near aic)
 TP117 SPI Bus0 MOSI ; SPI 0 (CL25) (near aic)
 TP118 SPI Bus0 Device0 (powerman) ; (CL21) (near aic)
 TP119 SPI Bus0 Device1 (wifi flash) ; (near wifi)
 TP120 SPI Bus0 Device2 (dsi tsc) ;/ (CL22) (near aic)
 TP121 SPI Bus1 CLK ;\ (CL54,CL27) (near aic)
 TP122 SPI Bus1 MISO ; SPI 1 (CL28) (near aic)
 TP123 SPI Bus1 MOSI ; (CL29) (near aic)
 TP124 SPI Bus1 Device0 (3ds tsc) ;/ (CL26) (near aic)
 TP125 GPIO 0:1 Pendown (1.8V) (0=low) (near aic)
 TP126 GPIO 0:2 Hinge (magnet hall sensor) (1=low=closed) (CL2,CL3) (near mcu)
 TP127 I2C Bus0 SDA ;also P9.pin23 (near aic)
 TP128 I2C Bus0 SCL ;also P9.pin24 (1.8V) (near aic)
 TP129 I2C Bus1 SDA ;also P8.pin38 and P9.pin13 (near mcu)
 TP130 I2C Bus1 SCL ;also P8.pin37 and P9.pin12 (1.8V) (near mcu)
 TP131 Upper Screen PWM (1166.7kHz, duty 2.7%-25%) (near main cpu)
 TP132 Lower Screen PWM (1166.7kHz, duty 5.0%-54%) (near main cpu)
 TP133 ...? low (near mcu)
 TP134 ...? low (near mcu)
 TP135 ...? high 1.8v (forced low=triggers reset?) (near mcu)
 TP136 ...? high 1.8v (forced low=triggers reset?) (near AIC)
 TP137 ...? high 1.8v (CL37) (near AIC)
 TP138 ...? low (CL31) (near AIC)
 TP139 ...? low (CL30) (near AIC)
 TP140 ...? low (CL32) (near AIC/wifi)
 TP141 GPIO 3:4 IrDA TX-RC (manual TX remote ctrl) 3.3V (near IrDA)
 TP142 N/A ? (?)
 TP143 ...? high 3.3v (near main cpu)
 TP144 I2C Bus2 SCL (390kHz) (3.3V) (near main cpu)
 TP145 N/A ? (?)
 TP146 I2C Bus2 SDA (IrDA and Gyro) (near main cpu)
 TP147 Audio I2S Sound Data Out (CL34) (near AIC)
 TP148 N/A ? (maybe I2S microphone in, but has no TP?) (?)
 TP149 Audio I2S Word Clock (off, 32kHz, or 48kHz) (CL33) (near AIC)
 TP150 Audio I2S Bit Clock (off, 1MHz, or 1.5MHz) (CL35) (near AIC)
 TP151 N/A ? (?)
 TP152 Audio Clock 8MHz/16MHz (for speaker PWM?) (CL36,CL55) (near AIC)
 TP153 GPIO 3:0 ... (would be C-Stick if New3DS) 3.3V (near main cpu)
 TP154 GPIO 3:3 ... (unknown) 3.3V (near main cpu)
 TP155 GPIO 3:7 ... (would be NFC if New3DS) 1.8V (near main cpu)
 TP156 GPIO 3:8 TSC IRQ (eg. TSC[67h:2Bh] headphone) 1.8V (near AIC)
 TP157 GPIO 3:9 MCU IRQ 1.8V (near mcu)
 TP158 GPIO 3:2 Gyroscope IRQ (ITG-3270.pin12) ?.?V (near circle pad)
 TP159 Upper Screen Video Data bit? (200mV) (P6.pin12) ;\
 TP160 Upper Screen Video Data bit? (200mV) (P6.pin11) ;
 TP161 Upper Screen Video Data bit? (200mV) (P6.pin14) ; Upper
 TP162 Upper Screen Video Data bit? (200mV) (P6.pin15) ; Screen
 TP163 Upper Screen Video Clock (15ns, 100mV) (P6.pin8) ;
 TP164 Upper Screen Video Clock' (15ns, 100mV) (P6.pin9) ;/
 TP165 Lower Screen R7 (red) ;\
 TP166 Lower Screen R3 (red) ;
 TP167 Lower Screen G7 (green) ; Lower
 TP168 Lower Screen G3 (green) ; Screen
 TP169 Lower Screen B7 (blue) ;
 TP170 Lower Screen B3 (blue) ;
 TP171 Lower Screen R6 (red) ;
 TP172 Lower Screen R2 (red) ;
 TP173 Lower Screen G6 (green) ;
 TP174 Lower Screen G2 (green) ;
 TP175 Lower Screen B6 (blue) ;
 TP176 Lower Screen B2 (blue) ;
 TP177 Lower Screen R5 (red) ;
 TP178 Lower Screen R1 (red) ;
 TP179 Lower Screen G5 (green) ;
 TP180 Lower Screen G1 (green) ;
 TP181 Lower Screen B5 (blue) ;
 TP182 Lower Screen B1 (blue) ;
 TP183 Lower Screen R4 (red) ;
 TP184 Lower Screen R0 (red) ;
 TP185 Lower Screen G4 (green) ;
 TP186 Lower Screen G0 (green) ;
 TP187 Lower Screen B4 (blue) ;
 TP188 Lower Screen B0 (blue) ;
 TP189 Lower Screen Dotclk (11MHz, 1.8V) ;
 TP190 Lower Screen VSYNC (24.77kHz) ;
 TP191 Lower Screen HSYNC (59.8Hz) ;/
 TP192 Old Camera Data Bit? (P9.pin34 via RA7 to CPU/TP220);\
 TP193 Old Camera Data Bit? (P9.pin33 via RA7 to CPU) ;
 TP194 Old Camera Data Bit? (P9.pin32 via RA7 to CPU) ; camera side of
 TP195 Old Camera Data Bit? (P9.pin31 via RA7 to CPU) ; camera data bus
 TP196 Old Camera Data Bit? (P9.pin30 via RA8 to CPU) ; (old DSi cams)
 TP197 Old Camera Data Bit? (P9.pin29 via RA8 to CPU) ;
 TP198 Old Camera Data Bit? (P9.pin27 via RA8 to CPU/TP227);
 TP199 Old Camera Data Bit? (P9.pin28 via RA8 to CPU) ;/
 TP200 Old Camera HSYNC (P9.pin26) (200us)
 TP201 Old Camera Data CLK (P9.pin37 via Rxx to CPU/TP228) (pcb upper left)
 TP202 Old Camera VSYNC (P9.pin25) (70ms)
 TP203 Both Camera 16MHz (I2C)(P9.pin17) (XXXohm to TP229?) ;\both cameras
 TP204 Both Camera Reset? (P9.pin20) (always high?) ;/
 TP205 Extra Camera Data Bit? (P9.pin4 via ?? to CPU/TP230) ;\
 TP206 Extra Camera Data Bit? (P9.pin5 via ?? to CPU) ;
 TP207 Extra Camera Data Bit? (P9.pin6 via ?? to CPU) ; camera side of
 TP208 Extra Camera Data Bit? (P9.pin7 via ?? to CPU) ; camera data bus
 TP209 Extra Camera Data Bit? (P9.pin8 via ?? to CPU) ; (extra 3DS cam)
 TP210 Extra Camera Data Bit? (P9.pin9 via ?? to CPU) ;
 TP211 Extra Camera Data Bit? (P9.pin10 via ?? to CPU) ;
 TP212 Extra Camera Data Bit? (P9.pin11 via ?? to CPU/TP237) ;/
 TP213 Touchpad ... ;\ (P13.Pin1)
 TP214 Touchpad ... ; X/Y +/- (P13.Pin2)
 TP215 Touchpad ... ; or so? (P13.Pin3)
 TP216 Touchpad ... ;/ (P13.Pin4)
 TP217 Extra Camera HSYNC (P9.pin14) (200us) (near main cpu)
 TP218 Extra Camera VSYNC (P9.pin15) (70ms) (near main cpu)
 TP219 Extra Camera Data CLK (P9.pin1) (Rxx to CPU/TP238) (pcb upper left)
 TP220 Old Camera Data Bit? (RA7 270ohm to TP192) ;\
 TP221 N/A ;\ ;
 TP222 N/A ; probably 6 more data bits ; CPU side of
 TP223 N/A ; (also wired via 270ohm RA7/RA8) ; DSi Camera bus
 TP224 N/A ; (but have no TPs on PCB) ;
 TP225 N/A ; ; `
 TP226 N/A ;/ ;
 TP227 Old Camera Data Bit? (RA8 270ohm to TP198 ;
 TP228 Old Camera Data CLK 16MHz (100ohm to TP201) ;/ (near main cpu, top)
 TP229 Both Camera 16MHz (I2C) (XXXohm to TP203?) (near main cpu, top)
 TP230 Extra Camera Data Bit? (270ohm to TP205) ;\ (near main cpu, top)
 TP231 N/A ;\ ;
 TP232 N/A ; probably 6 more data bits ; CPU side of
 TP233 N/A ; (but have no TPs on PCB) ; Extra Camera bus
 TP234 N/A ; ;
 TP235 N/A ; ;
 TP236 N/A ;/ ;
 TP237 Extra Camera Data Bit? (270ohm to TP212) ; (near main cpu, top)
 TP238 Extra Camera Data CLK 16MHz (100ohm to TP219) ;/ (near main cpu, top)
 TP239 GPIO 3:1 IrDA /IRQ (P24.pin6) 3.3V (near main cpu, low/right)
 TP240 Accelerometer VDD_IO (U9.pin1) (1.8V) (near home button)
 TP241 TH1 (battery temperature sensor) (off/normal 9Kohm to GND) (8K=warmer)
 TP242 Wifi CLK32k (P5.pin43) (near mcu)
 TP243 Wifi RESET (P5.pin44) ;maybe also for others...? (near wifi)
 TP244 Both Screens /RESET (P6.pin26 and P8.pin39) (near P8)
 TP245 ..pwr on/off for U12 power control? (PCB Side B, underneath NDS slot)
 TP246 ..pwr on/off for U12 power control? low 0V (near above U12)(near wifi)
 TP247 Wifi CCA (P5.pin9 ) ;\this have some slight noise (near wifi)
 TP248 Wifi TRRDY (P5.pin15) ;/from GPIO 4:0 (no real signal?) (near wifi)
 TP249 eMMC DAT0 ;\
 TP250 eMMC DAT1 ; PCB Side B
 TP251 eMMC DAT2 ; (eMMC CLK has no known TP)
 TP252 eMMC DAT3 ;
 TP253 eMMC CMD ;/
 TP254 Gyroscope ITG-3270.pin8 ("VLOGIC") 3.3V (CL56) (near circle pad)
 TP255 GPIO 3:5 IrDA RXD (for manual polling) 3.3V (near main cpu)
 TP256 DL5 Notify RGB LED blue (pcb top-right)
 TP257 DL5 Notify RGB LED red (pcb top-right)
 TP258 Speaker/Audio Supply 3.5V (U13.pin3) (CL45) (near select button)
 TP259 Charger IC U7.pin2+3 BAT (via F2 to TP22) (pcb top-right) |
| --- |

Yet unknown TPs details are...

```

| There should be an IRQ signal from TSC to MCU (but unknown which pin).
 There should be a logical SD Slot Power Enable (for requesting TP13 = 3.3V)
 Details on what is Camera D0..D7, etc.
 Details on plus/minus for Touchpad, UpperBacklight
 Debug button, Debug jtag, FIQ, SCFG_OP, etc. (if any)
 Most wifi pins seem to have no TPs (but some might have still missing ones)
 FCRAM seems to have no TPs (unless a few control signals do have some) |
| --- |

**Testpoints for 3DS XL and 2DS**

3DS XL and 2DS have some extra test points at TP260 and up (unknown if
TP1-TP259 are same as on Old3DS, at least some seem to be same or similar).

```

| TP260 eMMC CLK ;exists on both 2DS and 3DS XL (has no TP on 3DS)
 TP261 ?
 TP262 ? ;spotted on 2DS (near power button)
 TP263 ? ;spotted on 2DS (near power button)
 TP264 ? ;spotted on 2DS (near power button)
 ...
 TP307 ? ;spotted on 2DS |
| --- |

The extra TPs might be for changed signals, or for things that didn't have TPs
on Old3DS (like Wifi).

**Testpoints for New 3DS, New 3DS XL, and New 2DS**

New 3DS and New 3DS XL (and probably New 2DS) do also have testpoints, but only
half as many, and without text layer (so they could be used/described only via
photos or drawings).

**CL Cut-Links for Old 3DS (Mainboard "C/CTR-CPU-01")**

```

| CL1 GNDed (to accel.device.id) (near AIC, low/right)
 CL2 TP126 Hinge (to mcu) (near MCU, up/right/big)
 CL3 TP126 Hinge (to cpu) (near cpu, up, underneath nds-slot)
 CL4 N/A ? - (?)
 CL5 TP2 Supply 1.2V (near powerman, right)
 CL6 TP5 Supply 1.8V (near powerman, right)
 CL7 TP21+TP79 Standby Supply +1.8V MCU unbrick (near MCU, low/right)
 CL8 TP41 Lower Backlight + (near powerman, up/right)
 CL9 TP3 Supply 3.3V (near powerman, left)
 CL10 TP3 Supply 3.3V (near powerman, left)
 CL11 TP6 Supply 2.8V (near powerman, left)
 CL12 TP13 SD/MMC Slot Supply (near powerman, up/left)
 CL13 TP16 NDS Cart Slot supply (near powerman, up/left)
 CL14 TP28 Supply +5.1V for CAM+3D LEDs? (near powerman, up/left)
 CL15 TP32 Supply +5.0V for both screens? (near powerman, up/left)
 CL16 TP44 Upper Backlight? (near powerman, low/right)
 CL17 TP45 Upper Backlight? and via C35 to TP43 (near powerman, low/right)
 CL18 GNDed (near U8, fuel gauge)
 CL19 GNDed (TP25?) (near powerman, low)
 CL20 GPIO 1:0 Headphone? (via Q3 from TP49) (near AIC, low/tiny)
 CL21 TP118 SPI Bus0 Device0 (powerman) (near AIC, up)
 CL22 TP120 SPI Bus0 Device2 (dsi tsc) (near AIC, up)
 CL23 TP115 SPI Bus0 CLK (also CL53) (near AIC, up)
 CL24 TP116 SPI Bus0 MISO (near AIC, up)
 CL25 TP117 SPI Bus0 MOSI (near AIC, up)
 CL26 TP124 SPI Bus1 Device0 (3ds tsc) (near AIC, up)
 CL27 TP121 SPI Bus1 CLK (also CL54) (near AIC, up)
 CL28 TP122 SPI Bus1 MISO (near AIC, up)
 CL29 TP123 SPI Bus1 MOSI (near AIC, up)
 CL30 TP139 (near nds-slot)
 CL31 TP138 (near nds-slot)
 CL32 TP140 (near nds-slot)
 CL33 TP149 Audio I2S Word Clock (near AIC, up/left)
 CL34 TP147 Audio I2S Sound Data Out (near nds-slot)
 CL35 TP150 Audio I2S Bit Clock (near nds-slot)
 CL36 TP152 Audio Clock (also CL55) (near nds-slot)
 CL37 TP137 (near nds-slot)
 CL38 TP5 Supply 1.8V (near AIC, up/right)
 CL39 TP5 Supply 1.8V (near AIC, low/right)
 CL40 TP3 Supply 3.3V (near AIC, left)
 CL41 TP5 Supply 1.8V (near AIC, low/big)
 CL42 TP5 Supply 1.8V (near nds-slot)
 CL43 TP3 Supply 3.3V (near AIC, up)
 CL44 TP5 Supply 1.8V (near AIC, low/right)
 CL45 TP258 Supply 3.5V (Speakers) (near AIC, low/right)
 CL46 ? ? (near MCU, up/right/tiny)
 CL47 TP72 3D Slider (near MCU, left/tiny)
 CL48 TP73 Volume Slider (near MCU, left/tiny)
 CL49 TP94 SD/MMC Card /INSERT (near cpu, right, underneath nds-slot)
 CL50 TP93 SD/MMC Card /UNLOCK (near cpu, low/right, underneath nds-slot)
 CL51 .. Wifi BB_RF_SCK (P5.10) (near U12, underneath nds-slot)
 CL52 .. Wifi SDIO CLK (P5.31) (near cpu, right, underneath nds-slot)
 CL53 TP115 SPI Bus0 CLK (also CL23)(near cpu, low/right, underneath nds-slot)
 CL54 TP121 SPI Bus1 CLK (also CL27)(near cpu, low/right, underneath nds-slot)
 CL55 TP152 Audio Clock (also CL36)(near cpu, up, underneath nds-slot)
 CL56 TP254 Gyroscope pin8 (near gyro) |
| --- |

**Data Bus Resistors and Resistor Arrays**

```

| RA1 1x4 180ohm 4bit SD Slot Databus (pass-thru)
 RA2 1x4 100ohm 4bit eMMC Databus (pass-thru)
 RA3 1x4 56Kohm? 4bit SD Slot Databus (pull-up to TP13)
 RA5 1x4 57Kohm? 4bit eMMC Databus (pull-up to TP3)
 RA7,RA8 2x4 270ohm 8bit DSi Camera Databus (pass-thru)
 Rxx-Rxx 8x1 270ohm 8bit Extra Camera Databus (pass-thru)
 Rxx 1x1 100ohm 1bit DSi Camera Clock (pass-thru)
 Rxx 1x1 100ohm 1bit Extra Camera Clock (pass-thru)
 Rxx 1x1 XXXohm 1bit Both Camera Clock (I2C) (pass-thru?)
 Rxx 1x1 120ohm 1bit eMMC CLK (pass-thru)
 RA11 1x4 56Kohm? 4x Whatever ??? (pull-down to GND)
 EM23 1x4 0ohm? (near P13 touchpad)
 EM24 1x4 0ohm? (near cpu, underneath nds-slot) |
| --- |

**See also**

```
[http://www.3dbrew.org/wiki/Hardware](http://www.3dbrew.org/wiki/Hardware)[http://www.3dbrew.org/wiki/TP_Nets](http://www.3dbrew.org/wiki/TP_Nets)[http://www.3dbrew.org/wiki/Pinouts](http://www.3dbrew.org/wiki/Pinouts)[http://www.3dbrew.org/wiki/Video_Capture](http://www.3dbrew.org/wiki/Video_Capture)
| |
| --- |