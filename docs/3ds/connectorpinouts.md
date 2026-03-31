# Connectorpinouts

> Source: https://problemkaputt.de/gbatek.htm
> Section: Connectorpinouts

3DS Connector Pinouts 
| |
| --- |

 Connectors in Old 3DS with Mainboard "C/CTR-CPU-01"...
 
 **P1 - 19pin - NDS/DSi cartridge slot (17pin slot + 2pin switch at right side)**
 
```

| 1 GND NDS Slot GND
 2 TP95 NDS Slot CLK
 3 NC? NDS Slot ?
 4 TP107 NDS Slot /RomCS
 5 TP108 NDS Slot /Reset
 6 TP99 NDS Slot /SaveCS
 7 TP102 NDS Slot IRQ
 8 TP16 NDS Slot 3.3V
 9 TP110 NDS Slot D0
 10 TP111 NDS Slot D1
 11 TP112 NDS Slot D2
 12 TP104 NDS Slot D3
 13 TP105 NDS Slot D4
 14 TP113 NDS Slot D5
 15 TP114 NDS Slot D6
 16 TP106 NDS Slot D7
 17 GND NDS Slot GND
 18 TP109 NDS Slot /Insert Signal
 19 GND NDS Slot /Insert GND |
| --- |

**P2 - 6pin - External Power Supply input (4.6V DC IN) (with sticky glue!)**

```

| 1 TP4 4.6VIN (Power pin +)
 2 TP1 GND (Power pin -)
 3 TP1 GND (Shield)
 4 TP1 GND (Shield)
 5 TP4 4.6VIN (goldplate +) ;\alternate contacts (requires constant
 6 TP1 GND (goldplate -) ;/pressure, maybe for expansion hardware) |
| --- |

Note: This surface mounted connector breaks off easily, but it can be soldered
back in without problems (the 2DS uses a better connector with thru-holes; and
without goldplates).

**P3 - 5pin - Headphone (stereo, plus switch)**

```

| 1 TP48 Headphone Audio Ground? (GNDed when plug inserted)
 2 TP50 Headphone Audio Left (tip)
 3 TP51 Headphone Audio Right (ring)
 4 TP49 Headphone Insert Detect? (GNDed when plug inserted)
 5 GND |
| --- |

Note: Unlike NDS/DSi, the 3DS has no connector for external microphone.

**P4 - 3pin - Battery**

```

| + TP22 Battery + ;(via F2 to TP259)
 x TP23 Battery type/detect/alert?
 - GND Battery - |
| --- |

**P5 - 50pin - Wifi daughterboard (DWM-W028) (and probably same for J27H023.01)**

[https://fccid.io/EW4DWMW028/Label/ID-label-location-1272988.pdf](https://fccid.io/EW4DWMW028/Label/ID-label-location-1272988.pdf)

```

| 1 MCLK 2 RF_CSRF
 3 GND GND 4 BB_CSBB
 5 RXPE 6 BB_RF_SDIN
 7 TXPE 8 BB_RF_SDOUT
 9 TP247 CCA 10 CL51 BB_RF_SCK
 11 TRDATA 12 GND GND
 13 TRCLK 14 BBP_SLEEP_L
 15 TP248 TRRDY 16 RF_SLEEP_L
 17 TRST_L 18 SEL_ATH_L
 19 GND GND 20 GND GND
 21 SDIO_DATA_0 22 JTAG_TDO
 23 SDIO_DATA_1 24 JTAG_TMS
 25 SDIO_DATA_2 26 JTAG_TDI
 27 SDIO_DATA_3 28 JTAG_TCK
 29 GND GND 30 TP119 SPI_CS2 (SPI FLASH /CS)
 31 CL52 SDIO_CLK 32 W_B (SPI FLASH /WP)
 33 GND GND 34 TP115 SPI_CLK (SPI Bus0 CLK)
 35 SDIO_CMD 36 TP116 SPI_DO (SPI Bus0 MISO)
 37 UART_TXD 38 TP117 SPI_DI (SPI Bus0 MOSI)
 39 UART_RXD 40 SYS_RST_L
 41 GND GND 42 ATH_TX_H
 43 TP242 CLK32k 44 TP243 RESET
 45 GND GND 46 GND GND
 47 TP5 VDD_18 48 TP3 VDD_33
 49 TP5 VDD_18 50 TP3 VDD_33 |
| --- |

Functionally same as DSi, but not pin-compatible, and with two UART pins
(instead of the NC pins).

**P6 - 29pin - Upper Screen Video (connector P6 is on PCB side A)**

```

| 1 TP32 Supply 5.0V 2 GND
 3 TP24 VCOM 1.7V 4 GND
 5 TP5 Supply 1.8V 6 TP5 Supply 1.8V
 7 GND 8 TP163 Clock +/-? ;\
 9 TP164 Clock +/-? 10 GND ; Video Data (200mV)
 11 TP160 Data bit+/-? 12 TP159 Data bit+/-? ; and Clock (15ns, 100mV)
 13 GND 14 TP161 Data bit+/-? ; (Serial MIPI DSI format)
 15 TP162 Data bit+/-? 16 GND ;/
 17 TP32 Supply 5.0V 18 TP32 Supply 5.0V
 19 GND 20 TP5 Supply 1.8V
 21 TP24 VCOM 1.7V 22 TP35 AVDD 3.43V ;-calibrate as VCOM=AVDD/2
 23 GND 24 TP130 I2C Bus1 SCL
 25 TP129 I2C Bus1 SDA 26 TP244 I2C /RESET
 27 GND 28 TP36 Supply 10.0V
 29 TP40 Supply -5.0V |
| --- |

**P7 - 21pin - Upper Backlight/Parallax?, Speakers, 3D LED/Slider, Camera LED**

```

| 1 GND 2 TP44 Backlight 0.4V;\upper backlight?
 3 TP43 Backlight..? 4 TP45 Backlight 0.4V;/with THREE pins???
 5 TP31 Parallax PWM 6 TP30 Parallax /PWM ;-parallax
 7 TP64 Speaker L- 8 TP64 Speaker L- ;\left speaker
 9 TP65 Speaker L+ 10 TP65 Speaker L+ ;/
 11 TP66 Camera LED 12 TP28 Supply 5.1V ;-Camera LED (2.8V=off, Less=on)
 13 TP62 Speaker R+ 14 TP62 Speaker R+ ;\right speaker
 15 TP61 Speaker R- 16 TP61 Speaker R- ;/
 17 TP5 Supply 1.8V 18 TP72 3D Slider ;-3D Slider (0V=min, 1.8V=max)
 19 TP28 Supply 5.1V 20 TP60 3D LED green ;-3D LED (2.8V=off, Less=on)
 21 GND |
| --- |

Note: Speakers are 3.5V PWM, duty 50% when off, and about max duty 45%/55% for
negative/positive amplitude (apparently 45%/55% is generating enough watts).
Positive amplitudes have longer high duty on Speaker+ PWM pin, and shorter duty
on Speaker- /PWM pins.

**P8 - 43pin - Lower Screen: Video (Parallel RGB format)**

```

| 1 TP186 G0 (green) 2 TP180 G1 (green) ;\
 3 TP174 G2 (green) 4 TP168 G3 (green) ; green
 5 TP185 G4 (green) 6 TP179 G5 (green) ;
 7 TP173 G6 (green) 8 TP167 G7 (green) ;/
 9 GND 10 TP188 B0 (blue) ;\
 11 TP182 B1 (blue) 12 TP176 B2 (blue) ;
 13 TP170 B3 (blue) 14 TP187 B4 (blue) ; blue
 15 TP181 B5 (blue) 16 TP175 B6 (blue) ;
 17 TP169 B7 (blue) 18 GND ;/
 19 TP165 R7 (red) 20 TP171 R6 (red) ;\
 21 TP177 R5 (red) 22 TP183 R4 (red) ; red
 23 TP166 R3 (red) 24 TP172 R2 (red) ;
 25 TP178 R1 (red) 26 TP184 R0 (red) ;/
 27 GND 28 TP5 Supply 1.8V
 29 TP32 Supply 5.0V 30 GND
 31 TP5 Supply 1.8V 32 TP26 VCOM 1.9V ;\calibrate as VCOM=AVDD/2
 33 TP37 AVDD 3.7V 34 GND ;/
 35 TP189 Dotclk 11MHz 36 GND ;-clock 90ns, 1.8V
 37 TP130 I2C Bus1 SCL 38 TP129 I2C Bus1 SDA ;-I2C bus
 39 TP244 I2C /RESET 40 TP190 VSYNC 24.77kHz
 41 TP191 HSYNC 59.8Hz 42 TP36 Supply +10.0V
 43 TP40 Supply -5.0V |
| --- |

CAUTION: Below is NONSENSE (for example, pin9 is clearly GND)

```
[http://www.3dbrew.org/wiki/Pinouts](http://www.3dbrew.org/wiki/Pinouts)
| - with REVERSED pin numbers & bogus...
 OK BAD
 43 1 Supply -6V ;-supply -5v
 42 2 Supply 12V ;-supply 10v
 41 3 Pixel Clock (uh?) ;-HSYNC
 40 4 /HBL, Horizontal blank (low while blanking) (uh??) ;-VSYNC
 39 5 /VBL, Vertical blank (low while blanking) (uh???) ;-maybe DEN?
 38 6 2v2, Loopback of pin 07? (uh????) ;-I2C
 37 7 2v2, Content latch? screen memory?? (uh?????) ;-I2C
 36 8 GND ;-GND
 35 9 HCL Horizontal clock (uh?) ;-Dotclk
 34 10 GND ;-GND
 33 11 Contrast BIAS (usually 4.5V..4.8V) ;-AVDD 3.7
 32 12 Flicker BIAS (usually voltage same as above) ;-VCOM 1.9
 31 13 ??? Might be a transistor? Shorted if off??? (whut?) ;-supply 1.8
 30 14 Supply +6V (uh????) ;-GND
 29 15 ??? Loopback of pin 14 ? harsh pop sound. (uh?????) ;-supply +5v
 28 16 ??? ??? ;-supply
 27 17 ??? ??? (uh??????) ;-GND
 .. 18-25 RED 0..7 Red pixel bits
 18 26 GND
 .. 27-34 BLUE 7..0 Blue pixel bits
 9 35 GND
 .. 36-43 GREEN 7..0 Green pixel bits |
| --- |

**P9 - 37pin - Cameras (in upper screen unit)**

```

| 1 TP219 Data CLK ? 2 GND ;\
 3 GND 4 TP205 Data...? ;
 5 TP206 Data...? 6 TP207 Data...? ; extra 3DS camera
 7 TP208 Data...? 8 TP209 Data...? ; (13 signals)
 9 TP210 Data...? 10 TP211 Data...? ;
 11 TP212 Data...? 12 TP130 I2C Bus1 SCL ;
 13 TP129 I2C Bus1 SDA 14 TP217 HSYNC ;
 15 TP218 VSYNC 16 GND ;/
 17 TP203 16MHz (I2C) 18 GND ;\both cameras
 19 GND 20 TP204 Reset? ; (2 signals and supply)
 21 TP5 Supply 1.8V 22 TP6 Supply 2.8V ;/
 23 TP127 I2C Bus0 SDA 24 TP128 I2C Bus0 SCL ;\
 25 TP202 VSYNC 26 TP200 HYSNC ;
 27 TP198 Data...? 28 TP199 Data...? ; old DSi cameras
 29 TP197 Data...? 30 TP196 Data...? ; (13 signals)
 31 TP195 Data...? 32 TP194 Data...? ;
 33 TP193 Data...? 34 TP192 Data...? ;
 35 GND 36 GND ;
 37 TP201 Data CLK ? ;/ |
| --- |

**P10 - 4pin - Microphone**

```

| 1 GND
 2 TP47 1.02V ;\what is that? plus/minus? or left/right?
 3 TP46 3.30V ;/
 4 GND |
| --- |

**P11 - 4pin - Circle Pad (analog joystick)**

```

| 1 GND
 2 TP69 Circle Pad X (0.2V=Left, 1.5V=Right)
 3 TP5 Supply 1.8V
 4 TP68 Circle Pad Y (0.2V=Down, 1.5V=Up) |
| --- |

**P12 - 4pin - Lower Screen: Backlight**

```

| 1 TP42 Lower Backlight - ;\Minus (near 0V)
 2 TP42 Lower Backlight - ;/
 3 TP41 Lower Backlight + ;\Plus (pulsed 14V or so?)
 4 TP41 Lower Backlight + ;/ |
| --- |

**P13 - 4pin - Lower Screen: Touchpad**

```

| 1 TP213 Touchpad 1.8V ;\
 2 TP214 Touchpad 0.2V ; voltages measured without touchpad connected
 3 TP215 Touchpad 0.2V ;
 4 TP216 Touchpad 0V ;/ |
| --- |

**P14 - 20pin - SD Card Slot**

```

| 1 GND 2 TP100 Data1
 3 GND 4 TP101 Data0
 5 GND 6 GND
 7 GND 8 TP93 /Unlock (CL50)
 9 GND 10 TP94 /Inserted (CL49)
 11 GND 12 GND
 13 GND 14 TP96 Cmd
 15 GND 16 TP103 Clk
 17 TP13 VDD 18 TP97 Data3
 19 TP13 VDD 20 TP98 Data2 |
| --- |

**P15 - 8pin - Right Shoulder button**

```

| 1 GND 2 GND
 3 GND 4 TP82 Button R
 5 GND 6 GND
 7 GND 8 GND |
| --- |

**P16 - 8pin - Left Shoulder button**

```

| 1 GND 2 GND
 3 TP83 Button L 4 GND
 5 GND 6 GND
 7 GND 8 GND |
| --- |

**P17 - 3pin - Volume Slider (on PCB side A)**

```

| 1 TP5 Supply 1.8V (red wire)
 2 TP73 MCU Volume Slider (white wire) (0V=max, 1.8V=min)
 3 GND (black wire) |
| --- |

**P18/P19 - 1pin (each) - shield for eMMC**

**P20/P20 - 1pin (each) - shield for MCU**

GNDed clips for holding shielding plates.

**P22/P23 - N/A**

None such.

**P24 - 12pin - IR daughterboard (I2C Device 2:9Ah)**

```

| 1 TP146 I2C Bus2 SDA 2 TP144 I2C Bus2 SCL
 3 GND 4 GND
 5 TP141 GPIO 3:4 TX-RC 6 TP239 GPIO 3:1 /IRQ
 7 GND 8 GND
 9 GND 10 TP255 GPIO 3:5 RXD (IR.led.pin4 and IR.chip.pin24)
 11 TP3 Supply 3.3V 12 TP3 Supply 3.3V |
| --- |

TX-RC (Remote Control Transmit) is for sending pulses manually, with higher
signal strength. RXD can be used for manual polling. Anyways, normally bytes
are transferred via I2C and automatically passed from/to transceiver.

**P25 - 1pin - antenna cable holder or so?**

GNDed clip for holding antenna cable or so?

**Goldplate (left of NDS cart slot)**

GNDed contact on PCB, for bottom screen case/shield.