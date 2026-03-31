# Componentlists

> Source: https://problemkaputt.de/gbatek.htm
> Section: Componentlists

3DS Component Lists 
| |
| --- |

 **3DS Component List**
 
```

| PCB "C/CTR-CPU-01"
 U1 bga "1048 0H, CPU CTR, (M)(C)2010, Nintendo, JAPAN ARM" (main cpu)
 U2 bga "F JAPAN, MB82M8080-07L, 1040 M90, E1" (main ram)
 U3 bga "Texas Instruments, 93045A4, 0AAH86W GI" (PCB back, below YXAB)
 U4 bga "Texas Instruments, PAIC301DB, 0AA37DW, GI" (TSC)
 U5 bga "TOSHIBA THGBM2G3P1FBAI8, VX2306, TAIWAN, 10459AE" (eMMC)
 U6 bga "UC CTR, 041KM73, KG10" (MCU)
 U7 16pin "CKP, TI 09W, ZF1T" (PCB back, above YXAB) (battery charger)
 U8 8pin "17040, 08A45" MAX17040 (PCB back, above PWR button) (fuel gauge)
 U9 bga "2048, 33DH, X1MAQ" or so (small, below/right of PAIC) (accelero)
 U10 3pin "H0X" or "H1X" (PCB back, near A/B buttons) (magnet hall sensor)
 U11 24pin "INVENSENSE, ITG-3270, D1T134-C1, EL 1140 J" (gyro near circlepad)
 U12 6pin "EPB" (between main cpu and wifi socket) (TP3, TP5 power control?)
 U13 6pin "/\\ N5, 120" (right of PAIC) (TP258 audio voltage regulator)
 Q1 6pin "JG" (PCB back, right of POWER button)
 Q4 6pin "?" (between PAIC and UC CTR)
 X1 4pin "16.756" osc (near main CPU)
 X2 4pin "CA038" osc (near UC CTR)
 Other Stuff
 P1 19pin NDS/DSi cartridge slot (17pin slot + 2pin switch at right side)
 P2 6pin External Power Supply input (4.6V DC IN) (with sticky glue!)
 P3 5pin Headphone (stereo, plus switch)
 P4 3pin Battery
 P5 50pin Wifi daughterboard
 P6 29pin Upper screen: Video (connector P6 on PCB side A)
 P7 21pin Upper screen: Backlight, Parallax, Speakers, Cam/3D LEDs, Slider
 P8 43pin Lower Screen: Video
 P9 37pin Upper screen: Cameras
 P10 4pin Microphone
 P11 4pin Circle Pad (analog joystick)
 P12 4pin Lower Screen: Backlight
 P13 4pin Lower Screen: Touchpad
 P14 20pin SD/MMC Card Slot
 P15 8pin Right Shoulder button
 P16 8pin Left Shoulder button
 P17 3pin Volume Slider (on PCB side A)
 P18 1pin GNDed clip ;\for eMMC shielding plate
 P19 1pin GNDed clip ;/
 P20 1pin GNDed clip ;\for MCU shielding plate
 P21 1pin GNDed clip ;/
 P22 - N/A
 P23 - N/A
 P24 12pin IR daughterboard
 P25 1pin GNDed clip ;-antenna cable holder or so?
 SW1-SW5 POWER, HOME, WIFI, START, SELECT
 SW6-SW13 UP, X, LEFT, RIGHT, Y, A, DOWN, B
 DL1 2pin LED Charge (orange) (front-right)
 DL2 2pin LED Power good (blue) (front)
 DL3 2pin LED Power low (red) (front)
 DL4 2pin LED Wifi (yellow) (side-right, near wifi button)
 DL5 4pin LED Notify (RGB) (rear-right)
 F1 2pin Fuse External Power connector (marking "K")
 F2 2pin Fuse Internal Battery connector (marking "K")
 Wifi board "DWM-W028" (and probably same/similar for J27H023.01)
 Un 80pin Wifi "Atheros, AR6014G.AL1C, N2T689,00B, 1036, TAIWAN" ;76+4pin
 Un 8pin Wifi SPI FLASH "32A, 0VX, 46"
 Un 8pin Wifi I2C EEPROM "08B, H1, 0DQ"
 Un 4pin voltage regulator? "M12, KA"
 Qn 4pin Crystal "40.000"
 Pn 50pin Connector to mainboard
 Pn 2pin Antenna connector
 IrDA board "12-25, CTR-IR-01"
 U1 7pin IrDA transceiver "841 1A67" (RPM841-H16 or similar)
 U2 24pin IrDA chip "NXP, S750, 0803, TSD031C"
 X1 3pin crystal "/L'e." (unknown frequency)
 R1 2pin 3.3 ohm (between VCC and LEDA pin)
 C1-C3 capacitors
 SD/MMC Slot Board "SD 02 DM 2-2"
 P1 20pin Connector to P14 on mainboard
 P2 13pin SD/MMC slot (13 solder pads, plus shield)
 Note: The SD/MMC board is firmly glued to eMMC shielding plate
 (desolder the eMMC plate before trying to remove the SD/MMC board)
 L/R Button Boards
 L Button & R button, with 8pin connector each to P15/P16 on mainboard
 Volume Slider Board "L-28 R"
 O1 6pin volume slider (shielded potentiometer)
 R,W,B red/white/black wires to P17 on mainboard
 Circle Pad (analog joystick unit) "A145"
 Pn 4pin Cable to P11 on mainboard
 Microphone unit "HF 01 KF" (front side, between Start and Power buttons)
 Pn 4pin Cable to P10 on mainboard
 SD/MMC slot board
 Pn Xpin Connector to mainboard
 Pn Xpin Connector to full-size SD/MMC card
 Lower Screen Unit:
 Pn 43pin "LS030Q7DW48P 1Y17L D8045572QTFD" (lower display, 320x240)
 Pn 4pin Backlight (2-wire, with 4pin connector)
 Pn 4pin Touchpad "340, NB-F8D", "....NB-FBE AA5 117"
 Upper Screen Unit:
 Upper screen, parallax, backlight, speakers, 3d led, 3d slider, antenna |
| --- |

**3DS XL**

**2DS**

**New3DS**

**New2DS XL**

Unknown. These consoles seem to be quite rare, and no PCB photos exist...?

Old 2DS does have a Sleep button additionally to Power, maybe Sleep is
equivalent to Hinge on folded consoles?

**New3DS LL Component List (aka japanese version of New3DS XL)**

Mainboard (PCB "RED-CPU-01 SIDE B 11-1, [05: 14 071], RED-CPU-01 SIDE A")

```

| Chips on Side B (top side, access requires removing mainboard):
 Un bga CPU "1485 16, CPU LGR A, (C) 2014, Nintendo, JAPAN ARM" ;\
 Un bga RAM "F JAPAN, 82MK9A9A, 7L FCRAM, 1429969, E1" ;
 Un 6pin MAX8570EUT+T for upper backlight "ABTJ" ; Side B
 Un sqr Wifi "ATHEROS, AR6014G-AL1C, NKY197.00B, 1422, TAIWAN" ;
 Un bga eMMC "dotcode, SAMSUNG 410, KLM4G1YE0C-B301, CDA519GLN" ;
 Un 8pin Wifi-EEPROM I2C "408F, B347" (HN58X2408F; 1Kx8) ;
 Un bga MCU "UC KTR, 423KM01, 'TK14" ;
 Un bga Powerman? "TexasInstr, 93045A4, 3CAK08W L, GI" ;/
 Chips on Side A (bottom side, easily accessible):
 Un 16pin BQ24072 battery charger "CKP, TI 3A1, CBYS" (near ext.supply input)
 Un 6pin power/charge? "9D" (or "06"?) (near ext.supply input)
 Un 6pin power "635Q" (under powerman) (...maybe lower backlight?)
 Un 24pin IrDA "U8997, 3522" (nocash), or "NXP, S750, 1603, TSD438C" (ifixit)
 Un 7pin Infrared transceiver "841 3B60"
 Un 16pin Gyroscope? less-near infra red "IT3B,315BA1,LT425A" (like ITG-1010?)
 Un 32pin NFC Broadcom BCM20791 "20791UZ, KMLG 25, TD1430, 3976901W"
 Un 8pin Wifi-FLASH SPI(?) "32B, 3XH, .01" (4Kx8) (near AIC)
 Un bga AIC "TexasInstr, AIC3010D, 3BC473W, GI" (mic/sound/touchscr)
 Un 16pin Accelerometer? near AIC and headphone "KXTKK, 40860, .3413" Kionix?
 Un bga TCA6416A IO Expander "PH416A, TI 46T" ;near top-screen connectors
 Other stuff:
 Xn 4pin "CA405" or so (near UC KTR, probably for RTC)
 Xn 4pin "40000, K42BY" (under atheros, probably for Wifi)
 Xn 4pin "13000, K413Y" (under cpu, probably whatever) (to near-field chip?)
 Xn 4pin "D164G" or so (under cpu, probably whatever) (for CPU?)
 (or maybe one of above crystals is for IrDA, as on Old3DS?)
 Fn Fuse (for external charger + pin, near charger connector)
 Pn 2pin External Power supply
 Pn 3pin External Headphone
 Pn 2pin Connector to antenna
 Pn ??pin Connector to Button board (many pins)
 Pn ??pin Connector to MicroSD slot (medium pins)
 Pn 4pin Connector to Front panel Reset button? and MIC
 Pn 4pin Connector to Front panel Home button? (4pins, only 2 wires attached)
 Pn Connector to bottom-shell (left shoulder, 8pin+shield?, only 4 wires used)
 Pn Connector to bottom-shell (right shoulder, 8pin+shld?, only 4 wires used)
 Pn 4pin Connector to NFC (rectangle under bottom screen) (only 2 wires used)
 Pn 4pin Connector to analog stick (above dpad)
 Pn 4pin Connector to bottom screen, touchscreen
 Pn 4pin Connector to bottom screen, backlight (4pin, only 2 wires used)
 Pn ??pin Connector to bottom screen, video (many pins)
 Pn ??pin Connector to top screen? ;\presumably video,backlight,cameras,sound
 Pn ??pin Connector to top screen? ; (many pins on each connector)
 Pn ??pin Connector to top screen? ;/
 Pn 1pin Goldplate as additonal connection to cartridge board GND
 Transistors/resistors/capacitors and the like
 Barcode/sticker "22946012 Q01011K N" |
| --- |

Button board (PCB "F-7, PWB, /ABXY, /AU-C, /RED-01, CS, 01, 3814")

```

| Un 44pin ?? Chip "428A2, HF374, 7NU9" ;for C-stick and ZL/ZR-buttons?
 Un 3pin "(M)UN" (large thing, right of start button) (magnet sensor?)
 Un 8pin "7048, xxxxxx?" (tiny bga chip, right of select button) (fuel gauge)
 Pn Connector to mainboard
 Pn Connector to battery (3pin)
 Pn Connector to rubber nibble (C-Stick) (4pin)
 Pn Connector to shoulder (8pin)
 Sw Six buttons (A,B,X,Y,Sel,Start)
 Sw Front button (Power)
 Fn Fuse (for battery + pin, above wifi led)
 Led Four Front LEDs (two single ones, and one LED-pair)
 Led Notification LED (small 4pin RGB LED? under shoulder-connector)
 Transistors/resistors and the like
 Barcode/sticker "30946012 Z0101E9 X" |
| --- |

DPAD board (PCB "F-KEY 01, R, ZD, HF, 4354.")

```

| Pn Connector to cartridge slot board (5pin)
 Sw Four switches (Up,Down,Left,Right) |
| --- |

Cartridge Slot board (PCB "(DC)17P-01, C-3, 1 '14 12")

```

| Pn Cartridge Slot ("4081911")
 Pn Connector to mainboard
 Pn Connector to DPAD board (5pin)
 Pn Goldplate as additonal connection to mainboard GND |
| --- |

Battery:

```

| Name "Nintendo SPR-003, Rechargeable Battery, 3.7V 1750mAh 6.5Wh"
 Dotcode with text "MKH905D10"
 Back side "Li-ion 00, SPR-A-BPAA-C1" |
| --- |

Bottom Screen:

```

| Name "1912TSS140725, 21P4808L, B0014747QTFD" (4.18") |
| --- |

Top-screen unit

```

| black ribbon cable: to camera unit ;outer/rear cable
 orange ribbon cable: to slider boards ;middle cable
 orange ribbon cable: to lcd screen ;inner/front cable
 red cable: to wifi antenna
 1x LCD with Dotcode/sticker "LAM049M003A1, 0020S4911848"
 2x 2pin speaker's "G4830V23A" or "G4B30V23A" or so
 1x 2pin wifi-antenna (nameless pcb)
 1x camera unit (with two pcbs, 3 cameras, and shielding, see below)
 2x slider boards (see below) |
| --- |

Left slider/speaker board:

```

| 1x volume slider (9.8Kohm)
 1x wires to left speaker
 1x 13pin connector to lcd screen (maybe parallax or whatever)
 1x 4pin connector to lcd screen (maybe backlight and/or whatever) |
| --- |

Right slider/speaker board:

```

| 1x 3d slider (8.4Kohm)
 1x wires to right speaker |
| --- |

Front facing camera board:

```

| two cameras, capacitors (C1..C12), FB1, FB2 |
| --- |

Self facing camera board:

```

| one camera, capacitors (C1..C5), FB1, R1, Q1, D1 (ir-led?) |
| --- |

Case/bottom

```

| "(new), Nintendo, o3DS LL, (C)2014 Nintendo Made in China RED-001"
 "[MIC/KS], EC-14016, RED-RB-JPN-C0"
 "[R]007-AC0104, [T]D14-0144001, V(Ci Li-ion, (Nintendo)"
 Barcode/sticker "QJF10203741 6" |
| --- |