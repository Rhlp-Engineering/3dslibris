# I2Cmcurl78Sfrregistersspecialfunctionregistersioports

> Source: https://problemkaputt.de/gbatek.htm
> Section: I2Cmcurl78Sfrregistersspecialfunctionregistersioports

3DS I2C MCU - RL78 SFR Registers (Special Function Registers) (I/O ports) 
| |
| --- |

 **Datasheets**
 SFRs are documented in RL78 Hardware Manuals. However, the SFRs aren't 100% same for all RL78 chips. The "78K0R/KC3-L, 78K0R/KE3-L" datasheet appears to be pretty close to the 3DS hardware (aside from the F0500h-F07FFh area).
 
 **SFR List**
 
```

| Addr Access Reset Symbol Special Function Register (SFR)
 FFF00h R/W 1B- 00h P0 Port data 0
 FFF01h R/W 1B- 00h P1 Port data 1
 FFF02h R/W 1B- 00h P2 Port data 2
 FFF03h R/W 1B- 00h P3 Port data 3
 FFF04h R/W 1B- 00h P4 Port data 4
 FFF05h R/W 1B- 00h P5 Port data 5
 FFF06h R/W 1B- 00h P6 Port data 6
 FFF07h R/W 1B- 00h P7 Port data 7
 FFF08h R/W 1B- 00h P8 Port data 8 (N/A)
 FFF09h R/W 1B- 00h P9 Port data 9 (N/A)
 FFF0Ah R/W 1B- 00h P10 Port data 10 (N/A)
 FFF0Bh R/W 1B- 00h P11 Port data 11 (78K0R/K E3-L)
 FFF0Ch R/W 1B- Undef P12 Port data 12
 FFF0Dh R/W 1B- 00h P13 Port data 13 (78K0R/K E3-L)
 FFF0Eh R/W 1B- 00h P14 Port data 14
 FFF0Fh R/W 1B- 00h P15 Port data 15 (N/A)
 FFF10h R/W -BW 0000h SDR00 Serial data 00 (TXD0/SIO00)
 FFF12h R/W -BW 0000h SDR01 Serial data 01 (RXD0/SIO01)
 FFF14h R/W -BW 0000h SDR12 Serial data 12 (TXD3/SIO30)
 FFF16h R/W -BW 0000h SDR13 Serial data 13 (RXD3/SIO31)
 FFF18h R/W --W 0000h TDR00 Timer data 00
 FFF1Ah R/W -BW 0000h TDR01(L/H) Timer data 01 (NOT L/H)
 FFF1Eh R --W 0000h ADCR 10-bit A/D conversion result
 FFF1Fh R -B- 00h ADCRH 8-bit A/D conversion result
 FFF20h R/W 1B- FFh PM0 Port mode 0
 FFF21h R/W 1B- FFh PM1 Port mode 1
 FFF22h R/W 1B- FFh PM2 Port mode 2
 FFF23h R/W 1B- FFh PM3 Port mode 3
 FFF24h R/W 1B- FFh PM4 Port mode 4
 FFF25h R/W 1B- FFh PM5 Port mode 5
 FFF26h R/W 1B- FFh PM6 Port mode 6
 FFF27h R/W 1B- FFh PM7 Port mode 7
 FFF28h R/W 1B- FFh PM8 Port mode 8 (N/A)
 FFF29h R/W 1B- FFh PM9 Port mode 9 (N/A)
 FFF2Ah R/W 1B- FFh PM10 Port mode 10 (N/A)
 FFF2Bh R/W 1B- FFh PM11 Port mode 11 (78K0R/K E3-L)
 FFF2Ch R/W 1B- FFh PM12 Port mode 12
 FFF2Dh - --- - PM13 Port mode 13 (N/A)
 FFF2Eh R/W 1B- FFh PM14 Port mode 14
 FFF2Fh R/W 1B- FFh PM15 Port mode 15 (N/A) (3DS)
 FFF30h R/W 1B- 00h ADM0 A/D converter mode 0
 FFF31h R/W 1B- 00h ADS Analog input channel specification
 FFF32h R/W 1B- 00h ADM1 A/D converter mode 1 (N/A)
 FFF37h R/W 1B- 00h KRM Key return mode
 FFF38h R/W 1B- 00h EGP0 External interrupt rising edge enable 0
 FFF39h R/W 1B- 00h EGN0 External interrupt falling edge enable 0
 FFF3Ah R/W 1B- 00h EGP1 External int rising edge enable 1 (E3-L)
 FFF3Bh R/W 1B- 00h EGN1 External int falling edge enable 1 (E3-L)
 FFF3Ch R/W 1B- 00h ISC Input switch control register (!)
 FFF3Eh R/W 1B- 00h TIS0 Timer input select register 0 (!)
 FFF44h R/W -BW 0000h SDR02 Serial data 02 (TXD1/SIO10)
 FFF46h R/W -BW 0000h SDR03 Serial data 03 (RXD1/SIO11)
 FFF48h R/W -BW 0000h SDR10 Serial data 10 (TXD2/SIO20) (78K0R/K E3-L)
 FFF4Ah R/W -BW 0000h SDR11 Serial data 11 (RXD2/SIO21) (78K0R/K E3-L)
 FFF50h R/W -B- 00h IICA0 I2C IICA shift 0
 FFF51h R 1B- 00h IICS0 I2C IICA status 0
 FFF52h R/W 1B- 00h IICF0 I2C IICA flag 0
 ;FFF54h R/W -B- 00h IICA1 I2C IICA shift 1 (N/A)
 ;FFF55h R 1B- 00h IICS1 I2C IICA status 1 (N/A)
 ;FFF56h R/W 1B- 00h IICF1 I2C IICA flag 1 (N/A)
 FFF64h R/W --W 0000h TDR02 Timer data 02
 FFF66h R/W -BW 0000h TDR03(L/H) Timer data 03 (NOT L/H)
 FFF68h R/W --W 0000h TDR04 Timer data 04
 FFF6Ah R/W --W 0000h TDR05 Timer data 05
 FFF6Ch R/W --W 0000h TDR06 Timer data 06
 FFF6Eh R/W --W 0000h TDR07 Timer data 07
 FFF70h R/W --W 0000h TDR10 Timer data 10 (N/A)
 FFF72h R/W -BW 0000h TDR11(L/H) Timer data 11 (N/A)
 FFF74h R/W --W 0000h TDR12 Timer data 12 (N/A)
 FFF76h R/W -BW 0000h TDR13(L/H) Timer data 13 (N/A)
 FFF78h R/W --W 0000h TDR14 Timer data 14 (N/A)
 FFF7Ah R/W --W 0000h TDR15 Timer data 15 (N/A)
 FFF7Ch R/W --W 0000h TDR16 Timer data 16 (N/A)
 FFF7Eh R/W --W 0000h TDR17 Timer data 17 (N/A) |
| --- |

```

| FFF80h Undoc:
 FFF81h Undoc:
 FFF82h Undoc:
 FFF83h Undoc:
 FFF84h Undoc:
 FFF85h Undoc:
 FFF86h Undoc:
 FFF87h Undoc:
 FFF88h Undoc:
 FFF89h Undoc: something?
 FFF8Ah Undoc: something?
 FFF8Bh Undoc: Serial TOOL0 stat/ack
 FFF8Ch Undoc: Serial TOOL0 control
 FFF8Dh Undoc: Serial TOOL0 bauds.lsb
 FFF8Eh Undoc: Serial TOOL0 bauds.msb
 FFF8Fh Undoc: Serial TOOL0 data |
| --- |

```

| FFF90h R/W --W 0000h RSUBC Sub-count register (!) (3DS) ;\either
 (FFF90h)R/W --W 0FFFh ITMC Interval timer control (N/A) ;/or
 FFF92h R/W -B- 00h SEC Second count
 FFF93h R/W -B- 00h MIN Minute count
 FFF94h R/W -B- 12h HOUR Hour count (12 AM upon reset)
 FFF95h R/W -B- 00h WEEK Week count (uh, week??????)
 FFF96h R/W -B- 01h DAY Day count
 FFF97h R/W -B- 01h MONTH Month count
 FFF98h R/W -B- 00h YEAR Year count
 FFF99h R/W -B- 00h SUBCUD Watch error correction
 FFF9Ah R/W -B- 00h ALARMWM Alarm minute
 FFF9Bh R/W -B- 12h ALARMWH Alarm hour
 FFF9Ch R/W -B- 00h ALARMWW Alarm week
 FFF9Dh R/W 1B- 00h RTCC0 Real-time clock control 0
 FFF9Eh R/W 1B- 00h RTCC1 Real-time clock control 1
 FFF9Fh R/W 1B- 00h RTCC2 Real-time clock control 2 (!)
 FFFA0h R/W -B- 00h CMC Clock operation mode control
 FFFA1h R/W 1B- C0h CSC Clock operation status control
 FFFA2h R 1B- 00h OSTC Osc stabilization time counter status
 FFFA3h R/W -B- 07h OSTS Osc stabilization time select
 FFFA4h R/W 1B- 00h CKC System clock control
 FFFA5h R/W 1B- 00h CKS0 Clock output select 0
 FFFA6h R/W 1B- 00h CKS1 Clock output select 1 (N/A) (Old3DS)
 FFFA8h R -B- Undef RESF Reset control flag
 FFFA9h R/W 1B- 00h LVIM Low-Voltage detection
 FFFAAh R/W 1B- var LVIS Low-Voltage detection level (reset=xxh)
 FFFABh R/W -B- var WDTE Watchdog timer enable (reset=1Ah/9Ah)
 FFFACh R/W -B- 00h CRCIN CRC input (N/A)
 FFFB0h R/W -B- 00h DSA0 DMA SFR address 0
 FFFB1h R/W -B- 00h DSA1 DMA SFR address 1
 FFFB2h R/W -BW 0000h DRA0(L/H) DMA RAM address 0
 FFFB4h R/W -BW 0000h DRA1(L/H) DMA RAM address 1
 FFFB6h R/W -BW 0000h DBC0(L/H) DMA byte count 0
 FFFB8h R/W -BW 0000h DBC1(L/H) DMA byte count 1
 FFFBAh R/W 1B- 00h DMC0 DMA mode control 0
 FFFBBh R/W 1B- 00h DMC1 DMA mode control 1
 FFFBCh R/W 1B- 00h DRC0 DMA operation control 0
 FFFBDh R/W 1B- 00h DRC1 DMA operation control 1
 FFFBEh R/W 1B- 00h BECTL Background event control register (!)
 FFFBFh ? 1 ? - Undoc: Flash ... FFFBFh ... bit0,7
 FFFC0h W? ?B? ? PFCMD Undoc: Flash Protect Command (write A5h)
 FFFC1h R/W 1B- ? - Undoc: Flash ... FFC1h ... bits/pulses FSSQ ?
 FFFC2h R ? PFS Undoc: Flash Protect Status
 FFFC4h R/W 1B? ? FLPMC Undoc: Flash Programming Mode Ctl ;triple write
 FFFC5h W 1?- ? - Undoc: Flash ... 0FFC5h ... bits/mode ?
 FFFC6h W 1B- ? FLAPH Undoc: Flash addr ptr upper 8bit (bit1=flag)
 FFFC7h R/W 1?- ? - Undoc: Flash ... 0FFC7h ... flag bits
 FFFC8h W 1BW ? FLAPL Undoc: Flash addr ptr lower 16bit (bit0,1=flag)
 FFFCAh R/W ?B? ? - Undoc: Flash ... 0FFCAh ... ;triple write
 FFFCBh W ?B- ? FLWE Undoc: Flash write data ECC error correction?
 FFFCCh W --W ? FLWH Undoc: Flash write data upper 16bit ;\4-byte
 FFFCEh W --W ? FLWL Undoc: Flash write data lower 16bit ;/
 FFFD0h R/W 1BW 0000h IF2(L/H) Interrupt request flag 2
 FFFD2h R/W 1BW 00h IF3(L) Interrupt request flag 3L (N/A)
 FFFD4h R/W 1BW FFFFh MK2(L/H) Interrupt mask flag 2
 FFFD6h R/W 1BW FFh MK3(L) Interrupt mask flag 3L (N/A)
 FFFD8h R/W 1BW FFFFh PR02(L/H) Priority specification flag 02
 FFFDAh R/W 1BW FFh PR03(L) Priority specification flag 03L (N/A)
 FFFDCh R/W 1BW FFFFh PR12(L/H) Priority specification flag 12
 FFFDEh R/W 1BW FFh PR13(L) Priority specification flag 13L (N/A)
 FFFE0h R/W 1BW 0000h IF0(L/H) Interrupt request flag 0
 FFFE2h R/W 1BW 0000h IF1(L/H) Interrupt request flag 1
 FFFE4h R/W 1BW FFFFh MK0(L/H) Interrupt mask flag 0
 FFFE6h R/W 1BW FFFFh MK1(L/H) Interrupt mask flag 1
 FFFE8h R/W 1BW FFFFh PR00(L/H) Priority specification flag 00
 FFFEAh R/W 1BW FFFFh PR01(L/H) Priority specification flag 01
 FFFECh R/W 1BW FFFFh PR10(L/H) Priority specification flag 10
 FFFEEh R/W 1BW FFFFh PR11(L/H) Priority specification flag 11
 FFFF0h R/W --W 0000h MDAL Multiplication/division data A (L) (MULA)
 FFFF2h R/W --W 0000h MDAH Multiplication/division data A (H) (MULB)
 FFFF4h R/W --W 0000h MDBH Multiplication/division data B (H) (MULOH)
 FFFF6h R/W --W 0000h MDBL Multiplication/division data B (L) (MULOL)
 FFFF8h ??? ??? ????h SP(L/H) CPU Stack Pointer ;\
 FFFFAh R/W ??? ??h PSW CPU Program Status Word ;
 FFFFBh W -B- -- - CPU Reserve (for MUL/DIV opcodes) ; Fixed
 FFFFCh ??? ??? 00h CS CPU Program Counter Bank ; SFRs
 FFFFDh ??? ??? ??h ES CPU Data Bank ;
 FFFFEh R/W 1B- 00h PMC CPU Processor mode control ;
 FFFFFh ??? ??? ??h MEM CPU ... whatever? ;/ |
| --- |

**Extended SFR (2nd SFR) List**

```

| F0010h R/W 1B- 00h ADM2 A/D converter mode 2 (N/A)
 F0011h R/W -B- FFh ADUL A/D result comparison upper limit setting(N/A)
 F0012h R/W -B- 00h ADLL A/D result comparison lower limit setting(N/A)
 F0013h R/W -B- 00h ADTES A/D test (N/A)
 F0017h R/W -B- 10h ADPC A/D port configuration register (!)
 F0030h R/W 1B- 00h PU0 Pull-up resistor option 0 ;\
 F0031h R/W 1B- 00h PU1 Pull-up resistor option 1 ;
 F0032h - --- - PU2 Pull-up resistor option 2 (N/A) ;
 F0033h R/W 1B- 00h PU3 Pull-up resistor option 3 ;
 F0034h R/W 1B- 01h PU4 Pull-up resistor option 4 ;
 F0035h R/W 1B- 00h PU5 Pull-up resistor option 5 ;
 F0036h R/W 1B- 00h PU6 Pull-up resistor option 6 (N/A) ;
 F0037h R/W 1B- 00h PU7 Pull-up resistor option 7 ;
 F0038h R/W 1B- 00h PU8 Pull-up resistor option 8 (N/A) ;
 F0039h R/W 1B- 00h PU9 Pull-up resistor option 9 (N/A) ;
 F003Ah R/W 1B- 00h PU10 Pull-up resistor option 10 (N/A) ;
 F003Bh R/W 1B- 00h PU11 Pull-up resistor option 11 (78K0R/K E3-L)
 F003Ch R/W 1B- 00h PU12 Pull-up resistor option 12 ;
 F003Dh - --- - PU13 Pull-up resistor option 13 (N/A) ;
 F003Eh R/W 1B- 00h PU14 Pull-up resistor option 14 ;
 F003Fh - --- - PU15 Pull-up resistor option 15 (N/A) ;/
 F0040h R/W 1B- 00h PIM0 Port input mode 0 ;\
 F0041h R/W 1B- 00h PIM1 Port input mode 1 ;
 F0042h - --- - PIM2 Port input mode 2 (N/A) ;
 F0043h - --- - PIM3 Port input mode 3 (N/A) ;
 F0044h R/W 1B- 00h PIM4 Port input mode 4 (N/A) ;
 F0045h R/W 1B- 00h PIM5 Port input mode 5 (N/A) ;
 F0046h - --- - PIM6 Port input mode 6 (N/A) ;
 F0047h - --- - PIM7 Port input mode 7 (N/A) ;
 F0048h R/W 1B- 00h PIM8 Port input mode 8 (N/A) ;
 F0049h - --- - PIM9 Port input mode 9 (N/A) ;
 F004Ah - --- - PIM10 Port input mode 10 (N/A) ;
 F004Bh - --- - PIM11 Port input mode 11 (N/A) ;
 F004Ch - --- - PIM12 Port input mode 12 (N/A) ;
 F004Dh - --- - PIM13 Port input mode 13 (N/A) ;
 F004Eh R/W 1B- 00h PIM14 Port input mode 14 (78K0R/K E3-L);
 F004Fh - --- - PIM15 Port input mode 15 (N/A) ;/
 F0050h R/W 1B- 00h POM0 Port output mode 0 ;\
 F0051h R/W 1B- 00h POM1 Port output mode 1 ;
 F0052h - --- - POM2 Port output mode 2 (N/A) ;
 F0053h - --- - POM3 Port output mode 3 (N/A?) (3DS!);
 F0054h R/W 1B- 00h POM4 Port output mode 4 (N/A) ;
 F0055h R/W 1B- 00h POM5 Port output mode 5 (N/A) ;
 F0056h - --- - POM6 Port output mode 6 (N/A) ;
 F0057h R/W 1B- 00h POM7 Port output mode 7 (N/A) ;
 F0058h R/W 1B- 00h POM8 Port output mode 8 (N/A) ;
 F0059h R/W 1B- 00h POM9 Port output mode 9 (N/A) ;
 F005Ah - --- - POM10 Port output mode 10 (N/A) ;
 F005Bh - --- - POM11 Port output mode 11 (N/A) ;
 F005Ch - --- - POM12 Port output mode 12 (N/A) ;
 F005Dh - --- - POM13 Port output mode 13 (N/A) ;
 F005Eh R/W 1B- 00h POM14 Port output mode 14 (78K0R/K E3-L);
 F005Fh - --- - POM15 Port output mode 15 (N/A) ;/ |
| --- |

```

| F0060h R/W 1B- 00h NFEN0 Noise filter enable 0 (!) ;\(!)
 F0061h R/W 1B- 00h NFEN1 Noise filter enable 1 (!) ;/ |
| --- |

```

| ;F0060h R/W 1B- FFh PMC0 Port mode control 0 ;\ ;\
 ;F0061h - --- - PMC1 Port mode control 1 (N/A) ; ;
 ;F0062h - --- - PMC2 Port mode control 2 (N/A) ; ;
 ;F0063h R/W 1B- FFh PMC3 Port mode control 3 ; ;
 ;F0064h - --- - PMC4 Port mode control 4 (N/A) ; ;
 ;F0065h - --- - PMC5 Port mode control 5 (N/A) ; ;
 ;F0066h - --- - PMC6 Port mode control 6 (N/A) ; ;
 ;F0067h - --- - PMC7 Port mode control 7 (N/A) ; ; N/A
 ;F0068h - --- - PMC8 Port mode control 8 (N/A) ; ; !!!
 ;F0069h - --- - PMC9 Port mode control 9 (N/A) ; ;
 ;F006Ah R/W 1B- FFh PMC10 Port mode control 10 ; ;
 ;F006Bh R/W 1B- FFh PMC11 Port mode control 11 ; ;
 ;F006Ch R/W 1B- FFh PMC12 Port mode control 12 ; ;
 ;F006Dh - --- - PMC13 Port mode control 13 (N/A) ; ;
 ;F006Eh R/W 1B- FFh PMC14 Port mode control 14 ; ;
 ;F006Fh - --- - PMC15 Port mode control 15 (N/A) ;/ ;
 ;F0070h R/W 1B- 00h NFEN0 Noise filter enable 0 ;
 ;F0071h R/W 1B- 00h NFEN1 Noise filter enable 1 ;
 ;F0072h R/W 1B- 00h NFEN2 Noise filter enable 2 ;
 ;F0073h R/W 1B- 00h ISC Input switch control ;
 ;F0074h R/W -B- 00h TIS0 Timer input select 0 ;
 ;F0076h R/W -B- 00h ADPC A/D port configuration ;
 ;F0077h R/W -B- 00h PIOR Peripheral I/O redirection ;
 ;F0078h R/W -B- 00h IAWCTL Invalid memory access detection control ;
 ;F007Dh R/W 1B- 00h GDIDIS Global digital input disable ;
 ;F0090h R/W 1B- 00h DFLCTL Data flash control ;
 ;F00A0h R/W -B- opt. HIOTRM High-speed on-chip oscillator trimming ;
 ;F00A8h R/W -B- opt. HOCODIV High-speed on-chip oscillator frequency ;/ |
| --- |

```

| F00C0h ?? flash? (!)(!)(!) ;-3DS |
| --- |

```

| F00C0h R/W B ? Undoc: Flash ... F00C0h ;triple write
 F00C1h R B FLrE Undoc: Flash read data ECC error correction?
 F00C2h R W FLrH Undoc: Flash read data upper 16bit ;\4-byte
 F00C4h R W FLrL Undoc: Flash read data lower 16bit ;/total
 F00C8h R/W W ? Undoc: Flash ... F00C8h |
| --- |

```

| F00E0h R/W --W 0000h MDCL Multiplication/division data C (L)
 F00E2h R/W --W 0000h MDCH Multiplication/division data C (H)
 F00E8h R/W 1B- 00h MDUC Multiplication/division control
 F00F0h R/W 1B- 00h PER0 Peripheral enable 0
 F00F2h Timer? (!)(!)(!) ;-3DS
 F00F3h R/W -B- 00h OSMC Subsystem clock supply mode control
 F00F4h R/W -B- 00h RMC Regulator mode control register (!)
 F00F5h R/W 1B- 00h RPECTL RAM parity error control (!)
 F00F6h R/W 1B- 00h DSCCTL 20 MHz internal high-speed osc. control (!)
 F00FEh R -B- Undef BCDADJ BCD adjust result
 F0100h R -BW 0000h SSR00(L) Serial status 00
 F0102h R -BW 0000h SSR01(L) Serial status 01
 F0104h R -BW 0000h SSR02(L) Serial status 02
 F0106h R -BW 0000h SSR03(L) Serial status 03
 F0108h R/W -BW 0000h SIR00(L) Serial flag clear trigger 00
 F010Ah R/W -BW 0000h SIR01(L) Serial flag clear trigger 01
 F010Ch R/W -BW 0000h SIR02(L) Serial flag clear trigger 02
 F010Eh R/W -BW 0000h SIR03(L) Serial flag clear trigger 03
 F0110h R/W --W 0020h SMR00 Serial mode 00
 F0112h R/W --W 0020h SMR01 Serial mode 01
 F0114h R/W --W 0020h SMR02 Serial mode 02
 F0116h R/W --W 0020h SMR03 Serial mode 03
 F0118h R/W --W 0087h SCR00 Serial communication operation setting 00
 F011Ah R/W --W 0087h SCR01 Serial communication operation setting 01
 F011Ch R/W --W 0087h SCR02 Serial communication operation setting 02
 F011Eh R/W --W 0087h SCR03 Serial communication operation setting 03
 F0120h R 1BW 0000h SE0(L) Serial channel enable status 0
 F0122h R/W 1BW 0000h SS0(L) Serial channel start 0
 F0124h R/W 1BW 0000h ST0(L) Serial channel stop 0
 F0126h R/W -BW 0000h SPS0(L) Serial clock select 0
 F0128h R/W --W 0F0Fh SO0 Serial output 0
 F012Ah R/W 1BW 0000h SOE0(L) Serial output enable 0
 F0134h R/W -BW 0000h SOL0(L) Serial output level 0
 F0138h R/W -BW 0000h SSC0(L) Serial standby control 0 (N/A)
 F0140h R -BW 0000h SSR10(L) Serial status 10 (78K0R/K E3-L)
 F0142h R -BW 0000h SSR11(L) Serial status 11 (78K0R/K E3-L)
 F0144h R -BW 0000h SSR12(L) Serial status 12
 F0146h R -BW 0000h SSR13(L) Serial status 13
 F0148h R/W -BW 0000h SIR10(L) Serial flag clear trigger 10 (78K0R/K E3-L)
 F014Ah R/W -BW 0000h SIR11(L) Serial flag clear trigger 11 (78K0R/K E3-L)
 F014Ch R/W -BW 0000h SIR12(L) Serial flag clear trigger 12
 F014Eh R/W -BW 0000h SIR13(L) Serial flag clear trigger 13
 F0150h R/W --W 0020h SMR10 Serial mode 10 (78K0R/K E3-L)
 F0152h R/W --W 0020h SMR11 Serial mode 11 (78K0R/K E3-L)
 F0154h R/W --W 0020h SMR12 Serial mode 12
 F0156h R/W --W 0020h SMR13 Serial mode 13
 F0158h R/W --W 0087h SCR10 Serial communication operation setting 10 (E3)
 F015Ah R/W --W 0087h SCR11 Serial communication operation setting 11 (E3)
 F015Ch R/W --W 0087h SCR12 Serial communication operation setting 12
 F015Eh R/W --W 0087h SCR13 Serial communication operation setting 13
 F0160h R 1BW 0000h SE1(L) Serial channel enable status 1
 F0162h R/W 1BW 0000h SS1(L) Serial channel start 1
 F0164h R/W 1BW 0000h ST1(L) Serial channel stop 1
 F0166h R/W -BW 0000h SPS1(L) Serial clock select 1
 F0168h R/W --W 0F0Fh SO1 Serial output 1
 F016Ah R/W 1BW 0000h SOE1(L) Serial output enable 1
 F0174h R/W -BW 0000h SOL1(L) Serial output level 1
 F0178h R/W -BW 0000h SSC1(L) Serial standby control 1 (N/A)
 ... - - - ?
 F0180h R --W FFFFh TCR00 Timer counter 00
 F0182h R --W FFFFh TCR01 Timer counter 01
 F0184h R --W FFFFh TCR02 Timer counter 02
 F0186h R --W FFFFh TCR03 Timer counter 03
 F0188h R --W FFFFh TCR04 Timer counter 04
 F018Ah R --W FFFFh TCR05 Timer counter 05
 F018Ch R --W FFFFh TCR06 Timer counter 06
 F018Eh R --W FFFFh TCR07 Timer counter 07
 F0190h R/W --W 0000h TMR00 Timer mode 00
 F0192h R/W --W 0000h TMR01 Timer mode 01
 F0194h R/W --W 0000h TMR02 Timer mode 02
 F0196h R/W --W 0000h TMR03 Timer mode 03
 F0198h R/W --W 0000h TMR04 Timer mode 04
 F019Ah R/W --W 0000h TMR05 Timer mode 05
 F019Ch R/W --W 0000h TMR06 Timer mode 06
 F019Eh R/W --W 0000h TMR07 Timer mode 07
 F01A0h R -BW 0000h TSR00(L) Timer status 00
 F01A2h R -BW 0000h TSR01(L) Timer status 01
 F01A4h R -BW 0000h TSR02(L) Timer status 02
 F01A6h R -BW 0000h TSR03(L) Timer status 03
 F01A8h R -BW 0000h TSR04(L) Timer status 04
 F01AAh R -BW 0000h TSR05(L) Timer status 05
 F01ACh R -BW 0000h TSR06(L) Timer status 06
 F01AEh R -BW 0000h TSR07(L) Timer status 07
 F01B0h R 1BW 0000h TE0(L) Timer channel enable status 0
 F01B2h R/W 1BW 0000h TS0(L) Timer channel start 0
 F01B4h R/W 1BW 0000h TT0(L) Timer channel stop 0
 F01B6h R/W --W 0000h TPS0 Timer clock select 0
 F01B8h R/W -BW 0000h TO0(L) Timer output 0
 F01BAh R/W 1BW 0000h TOE0(L) Timer output enable 0
 F01BCh R/W -BW 0000h TOL0(L) Timer output level 0
 F01BEh R/W -BW 0000h TOM0(L) Timer output mode 0
 ;F01C0h R --W FFFFh TCR10 Timer counter 10 ;\
 ;F01C2h R --W FFFFh TCR11 Timer counter 11 ;
 ;F01C4h R --W FFFFh TCR12 Timer counter 12 ;
 ;F01C6h R --W FFFFh TCR13 Timer counter 13 ;
 ;F01C8h R --W FFFFh TCR14 Timer counter 14 ;
 ;F01CAh R --W FFFFh TCR15 Timer counter 15 ;
 ;F01CCh R --W FFFFh TCR16 Timer counter 16 ;
 ;F01CEh R --W FFFFh TCR17 Timer counter 17 ;
 ;F01D0h R/W --W 0000h TMR10 Timer mode 10 ;
 ;F01D2h R/W --W 0000h TMR11 Timer mode 11 ;
 ;F01D4h R/W --W 0000h TMR12 Timer mode 12 ; N/A !!!
 ;F01D6h R/W --W 0000h TMR13 Timer mode 13 ;
 ;F01D8h R/W --W 0000h TMR14 Timer mode 14 ;
 ;F01DAh R/W --W 0000h TMR15 Timer mode 15 ;
 ;F01DCh R/W --W 0000h TMR16 Timer mode 16 ;
 ;F01DEh R/W --W 0000h TMR17 Timer mode 17 ;
 ;F01E0h R -BW 0000h TSR10(L) Timer status 10 ;
 ;F01E2h R -BW 0000h TSR11(L) Timer status 11 ;
 ;F01E4h R -BW 0000h TSR12(L) Timer status 12 ;
 ;F01E6h R -BW 0000h TSR13(L) Timer status 13 ;
 ;F01E8h R -BW 0000h TSR14(L) Timer status 14 ;
 ;F01EAh R -BW 0000h TSR15(L) Timer status 15 ;
 ;F01ECh R -BW 0000h TSR16(L) Timer status 16 ;
 ;F01EEh R -BW 0000h TSR17(L) Timer status 17 ;
 ;F01F0h R 1BW 0000h TE1(L) Timer channel enable status 1 ;
 ;F01F2h R/W 1BW 0000h TS1(L) Timer channel start 1 ;
 ;F01F4h R/W 1BW 0000h TT1(L) Timer channel stop 1 ;
 ;F01F6h R/W --W 0000h TPS1 Timer clock select 1 ;
 ;F01F8h R/W -BW 0000h TO1(L) Timer output 1 ;
 ;F01FAh R/W 1BW 0000h TOE1(L) Timer output enable 1 ;
 ;F01FCh R/W -BW 0000h TOL1(L) Timer output level 1 ;
 ;F01FEh R/W -BW 0000h TOM1(L) Timer output mode 1 ;
 ;F0200h R/W -B- 00h DSA2 DMA SFR address 2 ;
 ;F0201h R/W -B- 00h DSA3 DMA SFR address 3 ;
 ;F0202h R/W -BW 0000h DRA2(L/H) DMA RAM address 2 ;
 ;F0204h R/W -BW 0000h DRA3(L/H) DMA RAM address 3 ;
 ;F0206h R/W -BW 0000h DBC2(L/H) DMA byte count 2 ;
 ;F0208h R/W -BW 0000h DBC3(L/H) DMA byte count 3 ;
 ;F020Ah R/W 1B- 00h DMC2 DMA mode control 2 ;
 ;F020Bh R/W 1B- 00h DMC3 DMA mode control 3 ;
 ;F020Ch R/W 1B- 00h DRC2 DMA operation control 2 ;
 ;F020Dh R/W 1B- 00h DRC3 DMA operation control 3 ;/
 F0230h R/W 1B- 00h IICCTL00 I2C IICA control 00
 F0231h R/W 1B- 00h IICCTL01 I2C IICA control 01
 F0232h R/W -B- FFh IICWL0 I2C IICA low-level width setting 0
 F0233h R/W -B- FFh IICWH0 I2C IICA high-level width setting 0
 F0234h R/W -B- 00h SVA0 I2C Slave address 0
 ;F0238h R/W 1B- 00h IICCTL10 I2C IICA control 10 (N/A)
 ;F0239h R/W 1B- 00h IICCTL11 I2C IICA control 11 (N/A)
 ;F023Ah R/W -B- FFh IICWL1 I2C IICA low-level width setting 1 (N/A)
 ;F023Bh R/W -B- FFh IICWH1 I2C IICA high-level width setting 1 (N/A)
 ;F023Ch R/W -B- 00h SVA1 I2C Slave address 1 (N/A)
 ;F02F0h R/W 1B- 00h CRC0CTL Flash memory CRC control (N/A)
 ;F02F2h R/W --W 0000h PGCRCL Flash memory CRC operation result (N/A)
 ;F02FAh R/W --W 0000h CRCD CRC data (N/A)
 For 78K0R...
 F0540h.. .. .. UF0.. UF0 stuff.. etc.. (not present in 3DS)
 Below are 3DS specific...
 F0501h ? ? ?? Peripheral enable 1 for I2C ? ;-3DS
 F0510h ? ? Port data X ? ;\
 F0511h ? ? Port mode X ; 3DS
 F0512h ? ? Pull-up resistor option X ;/
 F0538h ? ? ?? ;-3DS
 F0540h R/W -B- 00h IICA1 I2C IICA shift 1 ;\
 F0541h R 1B- 00h IICS1 I2C IICA status 1 ; 3DS
 F0542h R/W 1B- 00h IICF1 I2C IICA flag 1 ;/
 F0550h R/W 1B- 00h IICCTL10 I2C IICA control 10 ;\
 F0551h R/W 1B- 00h IICCTL11 I2C IICA control 11 ;
 F0552h R/W -B- FFh IICWL1 I2C IICA low-level width setting 1 ; 3DS
 F0553h R/W -B- FFh IICWH1 I2C IICA high-level width setting 1 ;
 F0554h R/W -B- 00h SVA1 I2C Slave address 1 ;/ |
| --- |

```

| F0746h B Undoc: ;-??
 F07E0h W Undoc: Saved AX ;\ ;\
 F07E2h W Undoc: Saved BC ; ;
 F07E4h W Undoc: Saved DE ; debug ; RAM
 F07E6h W Undoc: Saved HL ; stack? ;
 F07E8h W Undoc: Saved pop (maybe PSW?) ; ;
 F07EAh B Undoc: Saved CS ; ;
 F07EBh B Undoc: Saved ES ;/ ;
 F07ECh B Undoc: Security okay flag (55h=yes) ;
 F07EDh B Undoc: Saved A ? ;
 F07EEh B Undoc: Reply value ? ;
 F07F0h B Undoc: RET opcode (or custom vector) ;
 ;
 F0800h R/W B ? FLASH related ;\ ;
 F0801h W B ? FLASH related ; ;
 F0802h W B ? FLASH related ; 3DS;
 F0803h W B ? FLASH related ; ;
 F0804h R/W BW ? FLASH related address? ; ;
 F0806h R/W B ? FLASH related ;/ ;/ |
| --- |

**CPU Register Banks & short-addressable RAM**

```

| FFE20h R/W 1BW ?? RAM[C0h] (can be accessed with "short" address)
 FFEE0h R/W 1BW ?? AX3(X3/A3) AX Bank 3 aka RP0(3) aka R0(3)/R1(3) ;\
 FFEE2h R/W 1BW ?? BC3(C3/B3) BC Bank 3 aka RP1(3) aka R2(3)/R3(3) ; Bank 3
 FFEE4h R/W 1BW ?? DE3(E3/D3) DE Bank 3 aka RP2(3) aka R4(3)/R5(3) ;
 FFEE6h R/W 1BW ?? HL3(L3/H3) HL Bank 3 aka RP3(3) aka R6(3)/R7(3) ;/
 FFEE8h R/W 1BW ?? AX2(X2/A2) AX Bank 2 aka RP0(2) aka R0(2)/R1(2) ;\
 FFEEAh R/W 1BW ?? BC2(C2/B2) BC Bank 2 aka RP1(2) aka R2(2)/R3(2) ; Bank 2
 FFEECh R/W 1BW ?? DE2(E2/D2) DE Bank 2 aka RP2(2) aka R4(2)/R5(2) ;
 FFEEEh R/W 1BW ?? HL2(L2/H2) HL Bank 2 aka RP3(2) aka R6(2)/R7(2) ;/
 FFEF0h R/W 1BW ?? AX1(X1/A1) AX Bank 1 aka RP0(1) aka R0(1)/R1(1) ;\
 FFEF2h R/W 1BW ?? BC1(C1/B1) BC Bank 1 aka RP1(1) aka R2(1)/R3(1) ; Bank 1
 FFEF4h R/W 1BW ?? DE1(E1/D1) DE Bank 1 aka RP2(1) aka R4(1)/R5(1) ;
 FFEF6h R/W 1BW ?? HL1(L1/H1) HL Bank 1 aka RP3(1) aka R6(1)/R7(1) ;/
 FFEF8h R/W 1BW ?? AX0(X0/A0) AX Bank 0 aka RP0(0) aka R0(0)/R1(0) ;\
 FFEFAh R/W 1BW ?? BC0(C0/B0) BC Bank 0 aka RP1(0) aka R2(0)/R3(0) ; Bank 0
 FFEFCh R/W 1BW ?? DE0(E0/D0) DE Bank 0 aka RP2(0) aka R4(0)/R5(0) ;
 FFEFEh R/W 1BW ?? HL0(L0/H0) HL Bank 0 aka RP3(0) aka R6(0)/R7(0) ;/ |
| --- |

The "1BW" column indicates support for Bit/Byte/Word access.

Words with (L) or (L/H) symbol suffix can be alternately accessed as low and/or
high byte.