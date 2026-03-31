# = Coprocessor number (0Ah=Single-, 0Bh=Double-Precision)

> Source: https://problemkaputt.de/gbatek.htm
> Section: = Coprocessor number (0Ah=Single-, 0Bh=Double-Precision)

3DS ARM11 Interrupts 
| |
| --- |

 **Private Software Interrupts (can be different for each CPU)**
 
```

| IRQ Listener Description
 00h MPCore software IRQ, not configured
 01h MPCore software IRQ, used by BOOT11 to kickstart Core1
 02h-03h MPCore software IRQ, seem to be unused
 04h Kernel MPCore software IRQ, used to manage performance counter
 05h Kernel MPCore software IRQ, does apparently nothing
 06h Kernel MPCore software IRQ, extensively used by
 KernelSetState (and contains most of the actual
 code of the latter)
 07h Kernel MPCore software IRQ, see KCacheMaintenanceInterruptEvent
 08h Kernel MPCore software IRQ, used for scheduling
 09h Kernel MPCore software IRQ, used when handling
 exceptions that require termination of a thread or
 a process, and in some cases by
 svcSetDebugThreadContext, to store VFP registers
 in the thread's register storage.
 0Ah Kernel TLB operations IRQ, see KTLBOperationsInterruptEvent
 0Bh-0Eh MPCore software IRQ, not configured
 0Fh dmnt/debugger MPCore software IRQ, used to abstract FIQ (debug),
 this interrupt is never sent to New3DS core2/core3 |
| --- |

**Private Timer Interrupts (has separate timers for each CPU)**

```

| 10h-1Ch Hmmmm, these do NOT EXIST (?) in interrupt controller?
 1Dh Kernel MPCore Timer0 (Port 17E00600h)
 1Eh Kernel MPCore Timer1 (Port 17E00620h) (3DS used on "core 1")
 1Fh MPCore Legacy "nIRQ" pin (is that used in 3DS?) |
| --- |

**Hardware Interrupts (can be used for all CPUs that are selected as Target)**

```

| 20h - Unused? ;\maybe
 21h - Unused? But [17E01D00h].bit33 is set ; related to
 22h - Unused? ; first two
 23h - Unused? But [17E01D00h].bit35 is set ;/CPU cores?
 24h spi? SPI_BUS2 (unused) (Port 10143000h)
 25h-27h - Unused?
 28h gsp, TwlBg PSC0 (GPU_MEMFILL 0) (Port 10400010h)
 29h gsp, TwlBg PSC1 (GPU_MEMFILL 1) (Port 10400020h)
 2Ah gsp, TwlBg PDC0 (GPU H/V-IRQ for top screen) (Port 10400400h)
 2Bh gsp, TwlBg PDC1 (GPU H/V-IRQ for bottom screen) (Port 10400500h)
 2Ch gsp, TwlBg PPF (GPU_MEMCOPY) (Port 10400C00h)
 2Dh gsp, TwlBg P3D (GPUREG_IRQ_CMP/REQ) (Port 10401040h)
 2Eh-2Fh - Unused?
 30h-38h Kernel Old CDMA Event 0..8 (nine events with separate IRQs)
 39h Kernel Old CDMA Faulting (eg. CCR=0, or event>15)
 3Ah Kernel New CDMA Event 0..31 (32 events sharing one IRQ) ;\New3DS
 3Bh Kernel New CDMA Faulting (eg. CCR=0) ;/
 3Ch - Unused? ;\maybe alike
 3Dh - Unused? But New3DS [17E01D00h].bit61 is set ; IRQ 20h-23h
 3Eh - Unused? ; for extra
 3Fh - Unused? But New3DS [17E01D00h].bit63 is set ;/CPU cores?
 40h nwm WIFI SDIO Controller (Port 10122000h)
 41h nwm WIFI SDIO IRQ Pin
 42h nwm_dev? Alternate SD/MMC Slot controller (Port 10100000h?)
 43h - Unused? Or maybe Card IRQ Pin for above?
 44h - NTRCARD (Port 10164000h)
 45h mvd L2B_0 (First RGB-to-RGBA Converter) (10130000h) ;\New3DS
 46h mvd L2B_1 (Second RGB-to-RGBA Converter) (10131000h) ;/
 47h - Unused?
 48h camera Camera Bus 0 (DSi cameras) (Port 10120000h)
 49h camera Camera Bus 1 (left-eye) (Port 10121000h)
 4Ah dsp ...probably Teak DSP... ? (maybe 10203000h)
 4Bh camera Y2R_0 (First YUV-to-RGBA Converter) (10102000h)
 4Ch TwlBg LGYFB_0 Legacy GBA/NDS Video (Port 10110000h)
 4Dh TwlBg LGYFB_1 Legacy GBA/NDS Video (Port 10111000h)
 4Eh mvd Y2R_1 (Second YUV-to-RGBA Converter) (10132000h) ;\New3DS
 4Fh mvd MVD Registers (Port 10207000h) ;/
 50h pxi, TwlBg PXI_SYNC.bit29 from ARM9 (commonly used)
 51h pxi, TwlBg PXI_SYNC.bit30 from ARM9 (rarely used)
 52h pxi, TwlBg PXI Send Fifo Empty
 53h pxi, TwlBg PXI Receive Fifo Not Empty
 54h i2c, TwlBg I2C_BUS0 (DSi devices) (Port 10161000h)
 55h i2c, TwlBg I2C_BUS1 (3DS devices) (Port 10144000h)
 56h spi, TwlBg SPI_BUS0 (Pwrman,WifiFlash,Tsc) (Port 10160000h)
 57h spi, TwlBg SPI_BUS1 (Tsc) (Port 10142000h)
 58h Kernel CFG11_MPCORE_CLKCNT (clk change)(Port 10141300h) ;-New3DS
 59h TwlBg CFG11_TWLMODE_SLEEP (Port 10141104h)
 5Ah mic Microphone maybe? (maybe 10162000h)
 5Bh - HID PAD Controller Buttons (Port 10146000h)
 5Ch i2c, TwlBg I2C_BUS2 (3DS extra gimmicks) (Port 10148000h)
 5Dh-5Eh - Unused?
 5Fh - NDS-Wifi Registers (aka MP) (Port 10170000h)
 60h gpio, TwlBg GPIO_DATA0.bit2=0 Shell opened (GPIO falling edge)
 61h - Unused?
 62h gpio, TwlBg GPIO_DATA0.bit2=1 Shell closed (GPIO rising edge)
 63h gpio, TwlBg GPIO_DATA0.bit1 Touchscreen Pen Down (if enabled)
 64h gpio, TwlBg GPIO_DATA1.bit0 Headphone jack plugged in/out
 65h - Unused?
 66h gpio, TwlBg GPIO_DATA1.bit1 ?
 67h - Unused?
 68h gpio, TwlBg GPIO_DATA3.bit0 C-stick Interrupt (New3DS)
 69h gpio, TwlBg GPIO_DATA3.bit1 IrDA Interrupt
 6Ah gpio, TwlBg GPIO_DATA3.bit2 Gyro Interrupt
 6Bh gpio, TwlBg GPIO_DATA3.bit3 ?
 6Ch gpio, TwlBg GPIO_DATA3.bit4 IrDA TX-RC (manual out)
 6Dh gpio, TwlBg GPIO_DATA3.bit5 IrDA RXD (manual in)
 6Eh gpio, TwlBg GPIO_DATA3.bit6 ?
 6Fh gpio, TwlBg GPIO_DATA3.bit7 ?
 70h gpio, TwlBg GPIO_DATA3.bit8 TSC[67h:2Bh] (Headphone connect)
 71h gpio, TwlBg GPIO_DATA3.bit9 MCU Interrupt (MCU[10h-1Fh])
 72h gpio, TwlBg GPIO_DATA3.bit10 NFC Interrupt (New3DS)
 73h TwlBg GPIO_DATA3.bit11 ??
 74h ? CGC Gamecard power off (CFG9_CARD_PWROFF_DELAY)
 75h ? CGC Gamecard insert switch (CFG9_CARD_INSERT_DELAY)
 76h - L2C Level 2 Cache Controller (Port 17E10000h) New3DS
 77h - Unused?
 78h Kernel CPU0 cp15 Performance monitor count (any) overflow
 79h Kernel CPU1 cp15 Performance monitor count (any) overflow
 7Ah Kernel CPU2 cp15 Performance monitor count (any) overflow New3DS
 7Bh Kernel CPU3 cp15 Performance monitor count (any) overflow New3DS
 7Ch-7Fh - Unused?
 80h-3FEh Don't exist (3DS/New3DS has only 80h IRQ sources)
 3FFh None, no interrupt (or spurious interrupt) |
| --- |

The spurious interrupt (3FFh) might appear in an interrupt handler if the
handler got triggered, but something (eg. another CPU) has cleared the IRQ flag
before the handler got a chance to process it.

| ARM11 MPCore Private Memory Region Register Summary |
| --- |

**MPCore private memory region**

The MPCore Private Memory Region contains additional ARM control registers
(additionally to the CP15 coprocessor registers). The 3DS does have this region
mapped at address 17E00000h and up.

```

| 17E00000h 100h MPCore SCU (Snoop Control Unit)
 17E00100h 100h MPCore CPU interrupt interface for LOCAL CPU
 17E00200h 100h MPCore CPU0 interrupt interface (aliased for debug purposes)
 17E00300h 100h MPCore CPU1 interrupt interface (aliased for debug purposes)
 17E00400h 100h MPCore CPU2 interrupt interface (aliased for debug purposes)
 17E00500h 100h MPCore CPU3 interrupt interface (aliased for debug purposes)
 17E00600h 100h MPCore CPU timer and watchdog for LOCAL CPU
 17E00700h 100h MPCore CPU0 timer and watchdog
 17E00800h 100h MPCore CPU1 timer and watchdog
 17E00900h 100h MPCore CPU2 timer and watchdog ;\if enabled in
 17E00A00h 100h MPCore CPU3 timer and watchdog ;/10141312h/13h
 17E00B00h 500h MPCore Reserved (access causes a DECERR abort exception)
 17E01000h 1000h MPCore Global Interrupt distributor |
| --- |

**Snoop Control Unit (SCU)**

```

| Offset Type Reset Name
 17E00000h 4 R/W 00001FFEh SCU Control Register
 17E00004h 4 R (var) SCU Configuration Register
 17E00008h 4 R/W - SCU CPU Status
 17E0000Ch 4 W - SCU Invalidate All
 17E00010h 4 R/W 00000000h SCU Performance Monitor Control Register
 17E00014h 4 R/W 00000000h SCU Monitor Counter Events 0
 17E00018h 4 R/W 00000000h SCU Monitor Counter Events 1
 17E0001Ch 4 R/W 00000000h SCU Monitor Counter 0
 17E00020h 4 R/W 00000000h SCU Monitor Counter 1
 17E00024h 4 R/W 00000000h SCU Monitor Counter 2
 17E00028h 4 R/W 00000000h SCU Monitor Counter 3
 17E0002Ch 4 R/W 00000000h SCU Monitor Counter 4
 17E00030h 4 R/W 00000000h SCU Monitor Counter 5
 17E00034h 4 R/W 00000000h SCU Monitor Counter 6
 17E00038h 4 R/W 00000000h SCU Monitor Counter 7
 17E0003Ch C4h - - SCU Reserved (0) |
| --- |

All SCU registers are byte accessible.

**MP11 CPU Interrupt Interface Registers (for Interrupt Handling)**

```

| Address Type Reset value Function
 17E00100h 4 R/W 00000000h CPU IRQ Control Register
 17E00104h 4 R/W 000000F0h CPU IRQ Priority Mask Register
 17E00108h 4 R/W 00000003h CPU IRQ Binary Point Register
 17E0010Ch 4 R 000003FFh CPU IRQ Interrupt Acknowledge?? Register
 17E00110h 4 W - CPU IRQ End of Interrupt Register
 17E00114h 4 R 000000F0h CPU IRQ Running Priority Register
 17E00118h 4 R 000003FFh CPU IRQ Highest Pending Interrupt Register
 17E0011Ch E4h - - CPU IRQ Reserved |
| --- |

Above registers are for the LOCAL CPU, below are aliases for EACH CPU core.

```

| 17E00200h 100h MPCore CPU0 interrupt interface (aliased for debug purposes)
 17E00300h 100h MPCore CPU1 interrupt interface (aliased for debug purposes)
 17E00400h 100h MPCore CPU2 interrupt interface (aliased for debug purposes)
 17E00500h 100h MPCore CPU3 interrupt interface (aliased for debug purposes) |
| --- |

All registers of the MP11 CPU interrupt interfaces must be accessed by 32bit
transactions only.

**Timer and Watchdog registers**

```

| Offset Type Reset Name
 17E00600h 4 R/W 00000000h MPCore Timer0 Reload Value
 17E00604h 4 R/W 00000000h MPCore Timer0 Counter Value (decrementing)
 17E00608h 4 R/W 00000000h MPCore Timer0 Control Register
 17E0060Ch 4 R/W 00000000h MPCore Timer0 Interrupt Status
 17E00610h 10h - Reserved
 17E00620h 4 R/W 00000000h MPCore Timer1/Watchdog Reload Value
 17E00624h 4 R/W 00000000h MPCore Timer1/Watchdog Counter Value (decrem.)
 17E00628h 4 R/W 00000000h MPCore Timer1/Watchdog Control Register
 17E0062Ch 4 R/W 00000000h MPCore Timer1/Watchdog Interrupt Status
 17E00630h 4 R/W 00000000h MPCore Timer1/Watchdog Reset Sent Register
 17E00634h 4 W - MPCore Timer1/Watchdog Disable Register
 17E00638h C8h - Reserved |
| --- |

Above registers are for the LOCAL CPU, below are aliases for EACH CPU core.

```

| 17E00700h 100h MPCore CPU0 timer and watchdog
 17E00800h 100h MPCore CPU1 timer and watchdog
 17E00900h 100h MPCore CPU2 timer and watchdog ;\if enabled in
 17E00A00h 100h MPCore CPU3 timer and watchdog ;/10141312h/13h |
| --- |

All timer and watchdog registers are word accessible only.

Note: There's also another ARM11 timer/cycle counter in CP15 registers.

**Distributed Interrupt controller registers (for Interrupt Configuration)**

```

| Address Size Type Reset Function
 17E01000h 4 R/W 00000000h Interrupt Distributor Control Register
 17E01004h 4 R - Interrupt Controller Type Register
 17E01008h F8h - .... Reserved
 17E01100h 20h R/W 0000FFFFh Interrupt Enable set Registers ID0-ID31
 17E01104h () 00000000h Interrupt Enable set Registers ID32 and up
 17E01120h 60h - .... Reserved
 17E01180h 20h R/W 0000FFFFh Interrupt Enable clear Registers ID0-ID31
 17E01184h () 00000000h Interrupt Enable clear Registers ID32 and up
 17E011A0h 60h - .... Reserved
 17E01200h 20h R/W 00000000h Interrupt Pending set Registers
 17E01220h 60h - .... Reserved
 17E01280h 20h R/W 00000000h Interrupt Pending clear Registers
 17E012A0h 60h - .... Reserved
 17E01300h 20h R 00000000h Interrupt Active Bit Registers
 17E01320h E0h - .... Reserved
 17E01400h 100h R/W 00000000h Interrupt Priority Registers
 17E01500h 300h - .... Reserved
 17E01800h 100h R/W 00000000h Interrupt CPU targets Registers (a.)
 17E01900h 300h - .... Reserved
 17E01C00h 40h R/W AAAAAAAAh Interrupt Configuration Registers ID0-ID15
 17E01C04h () 28000000h Interrupt Configuration Registers ID29-ID31
 17E01C08h () 00000000h Interrupt Configuration Registers ID32 and up
 17E01C40h C0h - .... Reserved
 17E01D00h 20h R 00000000h Interrupt Line Level Registers ID0-ID31
 17E01D04h () - Interrupt Line Level Registers ID32 and up
 17E01D20h E0h - .... Oddly: mirrors of above 20h bytes
 17E01E00h 100h - .... Reserved
 17E01F00h 4 W - Software Interrupt Register
 17E01F0xh DCh? - .... Reserved
 17E01FE0h 4 R 90h Peripheral Identification Register 0
 17E01FE4h 4 R 13h Peripheral Identification Register 1
 17E01FE8h 4 R 04h Peripheral Identification Register 2
 17E01FECh 4 R 00h Peripheral Identification Register 3
 17E01FF0h 4 R 0Dh PrimeCell Identification Register 0
 17E01FF4h 4 R F0h PrimeCell Identification Register 1
 17E01FF8h 4 R 05h PrimeCell Identification Register 2
 17E01FFCh 4 R B1h PrimeCell Identification Register 3
 a. Except for address 0x81C. See Interrupt CPU Targets Registers. |
| --- |

All Interrupt Distributor Registers are byte accessible.

Official specs: DDI0360F_arm11_mpcore_r2p0_trm.pdf

| ARM11 MPCore - Snoop Control Unit (SCU) |
| --- |

**Snoop Control Unit (SCU)**

Below registers exists only once (not per CPU). However, the performance
counters are somewhat supposed to be used as two counters per CPU.

**17E00000h - SCU Control Register (R/W)**

The SCU Control Register enables the SCU and controls its behavior. It must be
accessed using a read-modify-write sequence.

```

| 0 SCU Enable (0=Disable, 1=Enable)
 (enable: maintain coherency between MP11 CPUs Level 1 data side caches)
 (in single CPU configuration, this bit has no effect and is always 0)
 1-4 Allow CPU0..3 to access SCU at 17E00000h..17E000FFh (0=No, 1=Yes)
 5-8 Allow CPU0..3 to access TMR at 17E00200h..17E005FFh (0=No, 1=Yes)
 9-12 Allow CPU0..3 to access IRQ at 17E00700h..17E00AFFh (0=No, 1=Yes)
 13 Report RAM Parity errors via parity error signals (0=Disable, 1=Enable)
 14-31 Reserved SBZ |
| --- |

Bit1-4: There is a mechanism that prevents all bits being cleared at the same
time.

Bit13: Before enabling SCU parity checking, all SCU tag must be invalidated.

This register doesn't seem to allow to disable access to the Global Interrupt
Distributor at 17E01000h-17E01FFFh (however, the MMU can disable the whole
4Kbyte page).

**17E00004h - SCU Configuration Register (R)**

```

| 0-1 Number of ARM11 CPU cores (0..3 = 1,2,3,4 CPU's)
 2-3 Reserved SBZ
 4-7 CPU0..3 Symmetric/Asymmetric Multi-processing (0=SMP/coherent, 1=AMP)
 8-9 CPU0 Tag RAM cache size (0=16KB, 1=32KB, 2=64KB, 3=Reserved)
 10-11 CPU1 Tag RAM cache size (0=16KB, 1=32KB, 2=64KB, 3=Reserved)
 12-13 CPU2 Tag RAM cache size (0=16KB, 1=32KB, 2=64KB, 3=Reserved)
 14-15 CPU3 Tag RAM cache size (0=16KB, 1=32KB, 2=64KB, 3=Reserved)
 16-31 Reserved SBZ |
| --- |

Cache size 16KB/32KB/64KB implies 64/128/256 indexes per tag RAM accordingly.

On Old3DS this is 00000011h. On New3DS this is 00005013h (even in Old3DS mode).
But changes to 00005003h when using fastboot...?

**17E00008h - SCU CPU Status Register (R/W)**

```

| 0-1 CPU0 status (0=Normal, 1=Reserved, 2=Dormant, 3=Powered-off)
 2-3 CPU1 status (0=Normal, 1=Reserved, 2=Dormant, 3=Powered-off)
 4-5 CPU2 status (0=Normal, 1=Reserved, 2=Dormant, 3=Powered-off)
 6-7 CPU3 status (0=Normal, 1=Reserved, 2=Dormant, 3=Powered-off)
 8-31 Reserved SBZ |
| --- |

Dormant mode and powered-off mode are controlled by an external power
controller.

SCU CPU Status Register bits indicate to the external power controller which
power domains can be powered down.

Before entering any other power mode than Normal, the MP11 CPU must set its
status field to signal to the SCU which mode it is about to enter (so that the
SCU can determine if it still can send coherency requests to the CPU). The MP11
CPU then executes a WFI entry instruction. When in WFI state, the PWRCTLOn bus
is enabled and signals to the power controller what it must do with power
domains.

The SCU CPU Status Register bits are used in conjunction with internal WFI
entry signals to generate PWRCTLOn output pins.

The SCU CPU Status Register bits can also be read by a CPU exiting low-power
mode to determine its state before executing its reset setup.

MP11 CPUs status fields take PWRCTLIn values at reset, except for nonpresent
CPUs.

For nonpresent CPUs writing to this field has no effect.

**17E0000Ch - SCU Invalidate All Register (W)**

```

| 0-3 Invalidate CPU0 ways (bit0-3 = Way 0,1,2,3) (0=No, 1=Invalidate)
 4-9 Invalidate CPU1 ways (bit4-9 = Way 0,1,2,3) (0=No, 1=Invalidate)
 8-11 Invalidate CPU2 ways (bit8-11 = Way 0,1,2,3) (0=No, 1=Invalidate)
 12-15 Invalidate CPU3 ways (bit12-15 = Way 0,1,2,3) (0=No, 1=Invalidate)
 16-31 Reserved SBZ |
| --- |

Allows to invalidate the tag RAMs on a per CPU and per way basis. This
operation is atomic, that is, a write transfer to this address only terminates
when all the lines have been invalidated. This register reads as 0.

Uh, what is a "way"?

**17E00010h - Performance Monitor Control Register (R/W)**

```

| 0 Enable bit for all counters (0=Disable, 1=Enable)
 1 Reset all count registers (0=No, 1=Reset)
 2-7 Reserved SBZ
 8-15 Counter MN0..7 Interrupt Enable (0=Disable, 1=Enable)
 16-23 Counter MN0..7 Interrupt Flag (0=No, 1=Overflow/IRQ) ;write 1 to clear
 24-31 Reserved SBZ/RAZ |
| --- |

**17E00014h - SCU Monitor Counter Events 0, bit0-31 (R/W)**

**17E00018h - SCU Monitor Counter Events 1, bit32-63 (R/W)**

```

| 0-7 EvCount0 Identifies the event for counter MN0
 8-15 EvCount1 Identifies the event for counter MN1
 16-23 EvCount2 Identifies the event for counter MN2 (if any)
 24-31 EvCount3 Identifies the event for counter MN3 (if any)
 48-55 EvCount6 Identifies the event for counter MN6 (if any)
 56-63 EvCount7 Identifies the event for counter MN7 (if any)
 32-39 EvCount4 Identifies the event for counter MN4 (if any)
 40-47 EvCount5 Identifies the event for counter MN5 (if any) |
| --- |

Event source number definitions:

```

| 00h Counter disabled
 01h CPU0 Miss ;\
 02h CPU1 Miss ; CPUn requested a coherent linefill that misses in all
 03h CPU2 Miss ; other CPUs. The request is sent to external memory
 04h CPU3 Miss ;/
 05h CPU0 Hit ;\
 06h CPU1 Hit ; CPUn requested a coherent linefill that hits in another
 07h CPU2 Hit ; CPU. The linefill is fetched from the relevant CPU cache
 08h CPU3 Hit ;/
 09h CPU0 Error ;\
 0Ah CPU1 Error ; CPUn was expected to have a coherent
 0Bh CPU2 Error ; line in its cache but answers nonpresent.
 0Ch CPU3 Error ;/
 0Dh Line migration ;-A line is directly transferred from one
 ; CPU to another on a linefill request instead of
 ; switching to SHARED.
 0Eh Master0 Read port busy
 0Fh Master1 Read port busy
 10h Master0 Write port busy
 11h Master1 Write port busy
 12h A Read transfer is sent to the external memory
 13h A Write transfer is sent to the external memory
 14h-1Eh N/A
 1Fh CycleCount ;-The counter increments on each CPU clock cycle
 20h-FFh N/A |
| --- |

**17E0001Ch - SCU Monitor Counter MN0 (R/W) ;exists always**

**17E00020h - SCU Monitor Counter MN1 (R/W) ;exists always**

**17E00024h - SCU Monitor Counter MN2 (R/W) ;exists only for two or more CPUs**

**17E00028h - SCU Monitor Counter MN3 (R/W) ;exists only for two or more CPUs**

**17E0002Ch - SCU Monitor Counter MN4 (R/W) ;exists only for three or more CPUs**

**17E00030h - SCU Monitor Counter MN5 (R/W) ;exists only for three or more CPUs**

**17E00034h - SCU Monitor Counter MN6 (R/W) ;exists only for four CPUs**

**17E00038h - SCU Monitor Counter MN7 (R/W) ;exists only for four CPUs**

```

| 0-31 Counter (incrementing upon selected event) |
| --- |

Old3DS has four counters, MN0-MN3. New3DS has eight counters, MN0-MN7 (even in
Old3DS mode).

| ARM11 MPCore - Timer and Watchdog |
| --- |

**Timer and Watchdog**

Below registers are for the LOCAL CPU core (the other CPU cores have their own
registers, mapped at the same address).

**17E00600h - MPCore Timer0 Reload Value (R/W)**

**17E00620h - MPCore Timer1/Watchdog Reload Value (R/W)**

```

| 0-31 Reload Value |
| --- |

The Reload value is copied to the Counter in two situations:

1) When the Counter decrements to zero (with Auto-reload enabled).

2) When writing to the Reload register (in watchdog mode this is the only way
to refresh the watchdog, and to prevent it from resetting the CPU).

**17E00604h - MPCore Timer0 Counter Value (R/W)**

**17E00624h - MPCore Timer1/Watchdog Counter Value (R/W, R in watchdog mode)**

```

| 0-31 Counter Value (decrementing) |
| --- |

If the MP11 CPU belonging to the timer is in debug state, the counter does not
decrement until the MP11 CPU returns to non debug state.

**17E00608h - MPCore Timer0 Control Register (R/W)**

**17E00628h - MPCore Timer1/Watchdog Control Register (R/W, R in watchdog mode)**

```

| 0 Timer Enable (0=Stop, 1=Enable/Decrement)
 1 Auto-reload (0=One-shot, stop at zero, 1=Auto-reload at zero)
 2 Interrupt Enable (0=Disable, 1=Trigger Interrupt ID 1Dh/1Eh at zero)
 3 Timer0: Reserved (0=Timer, fixed, always 0)
 3 Timer1: Watchdog mode (0=Timer, 1=Watchdog)
 Note: Bit3 can be cleared via 17E00634h only
 4-7 Reserved (0)
 8-15 Prescaler (0..255 = CPU_CLK/2 divided by 1..256)
 16-31 Reserved (0) |
| --- |

**17E0060Ch - MPCore Timer0 Interrupt Status (R/ack)**

**17E0062Ch - MPCore Timer1/Watchdog Interrupt Status (R/ack)**

```

| 0 Event flag (counter reached zero) (0=No, 1=Event) ;write 1 to clear
 1-31 Reserved |
| --- |

If the timer interrupt is enabled, Interrupt ID 1Dh/1Eh is set as Pending in
the Interrupt Distributor after the event flag is set.

**17E00630h - MPCore Timer1/Watchdog Reset Sent Register (R/ack)**

```

| 0 Reset flag (0=Normal, 1=Reset caused by Watchdog) ;write 1 to clear
 1-31 Reserved |
| --- |

**17E00634h - W - MPCore Timer1/Watchdog Disable Register**

```

| 0-31 Key (write 12345678h, then write 87654321h to disable watchdog) |
| --- |

Switches the watchdog back to Timer mode (ie. clears bit3 in Control Register,
and thereby re-enables writing to 17E00624h/17E00628h).

**Calculating timer intervals**

```

| The timer interval is calculated using the following equation:
 (PRESCALER_value+1)*(Reload_value+1)*2 / CPU_CLK_frequency
 This equation can be used to calculate the period between two events out of
 the timers and the watchdog time-out time. |
| --- |

Uh, doesn't that mean that reload occurs on UNDERFLOW (not on ZERO)...?

| ARM11 MPCore - Interrupt Configuration |
| --- |

**Distributed Interrupt Controller (for Interrupt Configuration)**

These registers are mostly used for interrupt configuration (and for internally
maintaining an list of pending/active interrupts).

```

| -- Most of these registers exists only once (not per CPU) -- |
| --- |

Except, the Priority for Software Interrupts 00h-0Fh can be configured per
LOCAL CPU. And, the Pending and Active flags do <internally> exist for
each CPU (for Software Interrupts 00h-0Fh, they do even exists for "From each
CPU to each CPU"). However, reading the Pending/Active/Priority flags doesn't
fully represent all of the internal per-CPU states.

**17E01000h - Interrupt Distributor Control Register (R/W)**

```

| 0 Global Interrupt Controller Enable (0=Disable, 1=Enable)
 1-31 Reserved |
| --- |

If bit0=0, no interrupts at all are sent to the CPU interrupt interfaces.

**17E01004h - Interrupt Controller Type Register (R)**

```

| 0-4 Number of Interrupt IDs (0-7 = 20h,40h,60h,...,100h) (8-31=Reserved)
 5-7 Number of ARM11 CPU cores (0-3 = 1,2,3,4 CPU's) (4-7=Reserved)
 8-31 Reserved |
| --- |

Note: Interrupt ID 00h-1Fh are internal IRQs, ID 20h-FFh are external IRQs.

On Old3DS this 00000023h, 2 CPUs and 80h IRQs.

On New3DS this 00000063h, 4 CPUs and 80h IRQs (even in Old3DS mode).

**17E01100h..17E0111Fh - Interrupt Enable Set Registers (256 x 1bit) (R/W)**

**17E01180h..17E0119Fh - Interrupt Enable Clear Registers (256 x 1bit) (R/W)**

```

| 0-15 Interrupt 00h-0Fh (Read: Always 1=Enabled) (Write=No effect)
 16-255 Interrupt 10h-FFh (Read: 0=Disabled, 1=Enabled) (Write: 1=Set/Clear) |
| --- |

Enable means that pending IRQs will be transmitted to the targeted CPUs.

The enable bit, when set to 0, does not prevent an edge-triggered interrupt
from becoming Pending.

The enable bit, when set to 0, does prevent a level sensitive interrupt from
becoming Pending only if asserted by the hardware pin, INT.

Note: If an interrupt is Pending or Active when its enable bit is set to 0, it
remains in its current state.

**17E01200h..17E0121Fh - Interrupt Pending set Registers (256 x 1bit) (R/W)**

**17E01280h..17E0129Fh - Interrupt Pending clear Registers (256 x 1bit) (R/W)**

```

| 0-15 Interrupt 00h-0Fh pending on local CPU (0=No, 1=Pending) (W=??)
 16-31 Interrupt 10h-1Fh pending on local CPU (0=No, 1=Pending) (W=?)
 32-255 Interrupt 20h-FFh pending on CPU(s)?? (0=No, 1=Pending) (W=Set/Clr)
 Bit0-31: Reading returns 1 if pending on local CPU core
 Bit32-255: Reading returns 1 if pending on one or more ANY CPU cores
 (if it's pending on the LOCAL CPU can be seen only once when
 reading the Acknowledge register?)
 (the Enable and Target bits do also somewhat imply whether
 the interrupt could have become pending on local CPU)
 Bit0-15: Write/Set is ignored (use 17E01F00h Software Interrupt instead)
 Bit16-31: Write/Set is...?
 Bit32-255: Write/Set allows to force the state=1 for ALL TARGET CPU cores
 Bit0-15: Write/Clear is...?
 Bit16-31: Write/Clear is...?
 Bit32-255: Write/Clear allows to force the state=0 for ALL CPU cores |
| --- |

**17E01300h..17E0131Fh - Active Bit Registers (256 x 1bit) (R)**

```

| 0-31 Interrupt 00h-1Fh is active on local CPU (0=No, 1=Active)
 32-255 Interrupt 20h-FFh is active on one or more CPUs (0=No, 1=Active) |
| --- |

Active means that the interrupt is being processed on at least one MP11 CPU.

**17E01400h..17E014FFh - Interrupt Priority Registers (256 x 8bit) (R/W)**

The priority determines which interrupt will show up next in the interrupt
interface registers. And, the interface registers can be used to manually
disable lower priority interrupts during 'critical sections'. And, while
processing an interrupt, the hardware will automatically disable interrupts
with same or lower priority (optionally, the "Binary Point" feature allows to
ignore the LSBs of the priorioty, eg. ignoring the lowest bit with allow to
also disable interrupts with slightly higher priority).

```

| 0-127 Interrupt 00h-0Fh Priority for local CPU (bit0-3=SBZ, bit4-7=Prio)
 128-231 Interrupt 10h-1Ch Priority for nothing? (bit0-7=Unused/zero)
 232-255 Interrupt 1Dh-1Fh Priority for local CPU (bit0-3=SBZ, bit4-7=Prio)
 256-2047 Interrupt 20h-FFh Priority for all CPUs (bit0-3=SBZ, bit4-7=Prio) |
| --- |

Priority values are 00h=Highest... E0h=Lowest, or F0h=None (same as interrupt
disabled).

When multiple Pending interrupts have the same priority, the selected interrupt
is the one with lowest ID. If there are multiple Pending software interrupts
with the same ID, the lowest MP11 CPU source is selected.

**17E01800h..17E018FFh - Interrupt CPU Targets Registers (256 x 8bit) (R/W)**

These registers store the list of MP11 CPUs for which an interrupt can be
Pending. Interrupt target registers are ignored in cases of software triggered
interrupts.

```

| 0-231 Interrupt 00h-1Ch Target (fixed 00h) (see 17E01F00h instead) (-)
 232-255 Interrupt 1Dh-1Fh Target (fixed 01h/02h/04h/08h for CPU0..3) (R)
 256-2047 Interrupt 20h-FFh Target (bit0-3=CPU0..3, bit4-7=SBZ) (R/W) |
| --- |

Modifying a CPU target list has no influence on a Pending or Active interrupt,
but takes effect on a subsequent assertion of the interrupt.

**17E01C00h..17E01C3Fh - Interrupt Configuration Registers (256 x 2bit) (R/W)**

Interrupt Configuration Registers define the assertion condition and the
software model of each interrupt.

```

| 0-31 Interrupt 00h-0Fh Condition (can be 2,3) (always rising-edge)
 32-57 Interrupt 10h-1Ch Condition (always 0)
 58-61 Interrupt 1Dh-1Eh Condition (always 2) (always N-N, rising-edge)
 62-63 Interrupt 1Fh Condition (always 0) (ignored, always low active)
 64-511 Interrupt 20h-FFh Condition (can be 0,1,2,3) |
| --- |

Interrupt line encodings for bits 1 and 0:

```

| 00h = N-N software model, level high active
 01h = 1-N software model, level high active
 02h = N-N software model, rising edge sensitive
 03h = 1-N software model, rising edge sensitive |
| --- |

The N-N and 1-N models are relevant only if more than one CPU is configured as
Target:

```

| 1-N model: An interrupt that is taken on any CPU clears the Pending status
 on all CPUs.
 N-N model: All CPUs receive the interrupt independently. The Pending status
 is cleared only for the CPU that takes it, not for the other CPUs. The N-N
 model has been deprecated in the latest interrupt controller architecture. |
| --- |

Unknown how 1-N and N-N differ for Interrupt 00h-0Fh (it's said that each CPU
has it's own pending flags for 00h-0Fh, so the model should be always "1-1").

```

| Obscure Notes:
 - With the 1-N software model, the nIRQ input is asserted on all CPUs
 configured in the CPU Targets Register. Uh, but Interrupt 1Fh is fixed?
 - If more than one of these CPUs reads the Interrupt Acknowledge Register
 at the same time, they can all acknowledge the same interrupt. The
 interrupt service routine must ensure that only one of them tries to
 process the interrupt, with the others returning after writing the ID to
 the End of Interrupt Register. Uh, does that apply to nIRQ only? Or to
 all IRQs? |
| --- |

**17E01D00h..17E01D1Fh - Interrupt Line Level Registers (256 x 1bit) (R)**

```

| 0-31 Dummy line level bits for Internal IRQ 00h..1Fh (always 0)
 32-255 Interrupt line level for External IRQ 20h..FFh (0=Low, 1=High) |
| --- |

**17E01F00h - Software Interrupt Register (W)**

```

| 0-9 Interrupt ID (0..FFh? or 0..0Fh/1Fh?) (100h..1FFh=Reserved/ignored)
 10-15 SBZ
 16-19 Send the interrupt to CPU0..3 (0=No, 1=Yes) ;used only if Bit24-25=0
 20-23 SBZ
 24-25 Target list mode (0=Bit16-19, 1=Other CPUs, 2=Local CPU, 3=Reserved)
 26-31 SBZ |
| --- |

This can be used to trigger an interrupt (identified with its ID) to a list of
MP11 CPUs.

| ARM11 MPCore - Interrupt Handling |
| --- |

**CPU Interrupt Interface Registers (for Interrupt Handling)**

Below registers are for the LOCAL CPU core (the other CPU cores have their own
registers, mapped at the same address).

**17E00100h - CPU Interface Control Register (R/W)**

```

| 0 Interrupt Enable for local CPU (0=Disable, 1=Enable)
 1-31 Reserved |
| --- |

When disabled: External nIRQ input is still working (and, in that case, nIRQ
should be handled directly; without using the disabled ACK/EOI registers).

**17E00104h - Priority Mask Register (R/W)**

The priority mask is used to prevent interrupts from being sent to the MP11
CPU. The CPU Interface asserts an interrupt request to an MP11 CPU if the
priority of the highest Pending interrupt sent by the Interrupt Distributor is
strictly higher than the mask set in the Priority Mask Register.

```

| 0-3 Unused SBZ
 4-7 Priority Mask value
 NOTE: This is a 4bit COMPARE value (not an AND-mask)
 0xF Interrupts with priority 0x0-0xE are not masked.
 0x0 All interrupts are masked.
 8-31 Unused SBZ |
| --- |

**17E00108h - Binary Point Register (R/W)**

The Binary Point Register is used to determime whether a new interrupt
pre-empts a currently Active one, using only part, or none of the priority
level.

```

| 0-2 Binary Point (see below)
 3-31 Reserved |
| --- |

Binary point bit values assignment:

```

| 03h = All bits4,5,6,7 of priority are compared for pre-emption
 04h = Only bit5,6,7 of priority are compared for pre-emption
 05h = Only bit6,7 of priority are compared for pre-emption
 06h = Only bit7 of priority is compared for pre-emption
 07h = No bits compared, no pre-emption is performed (no nested IRQs)
 00h,01h,02h = Same as 03h |
| --- |

This is related to the "Running Priority Register" (see there for details).

**17E0010Ch - Interrupt Acknowledge (ACK) Register (R)**

```

| 0-9 Interrupt ID (00h..FFh, or 3FFh=None)
 10-12 Source for ID=00h-0Fh (0..3=CPU0..3, or always 0 for ID=10h-3FFh)
 13-31 Unused (0) |
| --- |

Reading returns the ID of the next pending interrupt with highest priority, and
automatically switches the interrupt from pending state to active state
(indicating that it is being processed). The CPU should then process the
interrupt, and, thereafer, use End of Interrupt register (to indicate that
processing is done).

**17E00110h - End of Interrupt (EOI) Register (W)**

```

| 0-31 Same format as Interrupt Acknowledge Register (see there) |
| --- |

Writing clears the Active flag for the corresponding interrupt, indicating that
interrupt processing is done. The written value should be same as the
(memorized) value from the Interrupt Acknowledge Register.

**17E00114h - Running Priority Register (R)**

Indicates the priority of the currently processed interrupt (the last
acknowledged and not yet completed interrupt on the local CPU core).

```

| 0-3 Reserved SBZ
 4-7 Priority (0=Highest .. 14=lowest, 15=None/No interrupt being processed)
 8-31 Reserved |
| --- |

All interrupts with same or lower priority are automatically disabled until
finishing the current interrupt (by writing to End of Interrupt register).

Optionally, the "Binary Point Register" allows to ignore the priority LSB(s)
when comparing the new Interrupt's priority with the currently "Running
Priority" (for example, ignoring the lower two bits would cause an interrupt
with priority=5 to also disable slighly higher priority interrupts with
priority=6..7).

**17E00118h - Highest Pending Interrupt Register (R)**

```

| 0-31 Same format as Interrupt Acknowledge Register (see there) |
| --- |

Same as Interrupt Acknowledge Register, except that reading doesn't make the
corresponding interrupt Active.

Uh, and supposedly doesn't clear its Pending state... or does it?

| ARM11 MPCore Distributed Interrupt Controller (Blurb) |
| --- |

**10.1.1 Distributed Interrupt Controller clock frequency**

The Distributed Interrupt Controller logic is clocked at half the frequency of
the MPCore CPUs because of power and area considerations. Reducing clock speed
reduces dynamic power consumption. The lower clock speed requires less
pipelining in the design. This means that the overall impact of the reduced
clock speed on the Distributed Interrupt Controller is kept to a minimum.

Note

As a consequence, the minimum pulse width of signals driving external interrupt
lines is two CPU clock cycles.

**10.2 Terminology - From point of view of an MP11 CPU, an interrupt can be:**

```

| Inactive: An Inactive interrupt is one that is nonasserted, or which in a
 multi-processing environment has been completely processed by that MP11
 CPU but can still be either Pending or Active in some of the MP11 CPUs to
 which it is targeted, and so might not have been cleared at the interrupt
 source.
 Pending: A Pending interrupt is one that has been asserted, and for which
 processing has not started on that MP11 CPU.
 Active: An Active interrupt is one that has been started on that MP11 CPU,
 but processing is not complete. |
| --- |

An interrupt can be Pending and Active at the same time. This can happen in the
case of edge triggered interrupts, when the interrupt is asserted while the
MP11 CPU has not finished handling the first occurrence. For level-sensitive
interrupts it can only happen if software triggers it. See Interrupt
Configuration Registers, 0xC00-0xC3C on page 10-17.

```

| Pre-emption: An Active interrupt can be pre-empted when a new interrupt of
 higher priority interrupts MP11 CPU interrupt processing. For the purpose
 of this document, an Active interrupt can be running if it is actually
 being processed, or pre-empted. |
| --- |

The Distributed Interrupt Controller consists of:

Interrupt Distributor:

The Interrupt Distributor handles interrupt detection and interrupt
prioritization.

CPU interrupt interfaces:

There is one CPU interrupt interface per MP11 CPU. The MP11 CPU interrupt
interfaces handle interrupt acknowledgement, interrupt masking, and interrupt
completion acknowledgement.

**10.3 Interrupt Distributor**

The Interrupt Distributor centralizes all interrupt sources for the ARM11
MPCore processor before dispatching the highest priority ones to each
individual MP11 CPU.

All interrupt sources are identified by a unique ID. All interrupt sources have
their own configurable priority and list of targeted CPUs, that is, a list of
CPUs to which the interrupt is sent when triggered by the Interrupt
Distributor.

Note

nFIQ interrupts are not handled by the Distributed Interrupt Controller so that
nFIQ interrupt input pins are directly routed to their respective CPU.

Interrupt sources are of the following types:

**Interprocessor interrupts (IPI)**

Each MP11 CPU has private interrupts, ID0-ID15, that can only be triggered by
software. These interrupts are aliased so that there is no requirement for a
requesting MP11 CPU to determine its own ID when it deals with IPIs. The
priority of an IPI depends on the receiving CPU, not the sending CPU.

**Private timer and/or watchdog interrupts.**

Each MP11 CPU has its own private timer and watchdog that can generate
interrupts, using ID29 and ID30.

**A legacy nIRQ pin**

In legacy IRQ mode the legacy nIRQ pin, on a per CPU basis, bypasses the
Interrupt Distributor logic and directly drives interrupt requests into the
MP11 CPU. In legacy IRQ mode, if bit [0] of the CPU Interface Control Register
is 0, then no interrupts are raised based on input from the Interrupt
Distributor. If bit [0] is 1, then all interrupts are received from the
Interrupt Distributor.

When an MP11 CPU uses the Distributed Interrupt Controller (rather than the
legacy pin in the legacy mode) by enabling its own CPU interface, the legacy
nIRQ pin is treated like other interrupt lines and uses ID31.

**Hardware interrupts**

Hardware interrupts are triggered by programmable events on associated
interrupt input lines. MP11 CPUs can support up to 224 interrupt input lines.
The interrupt input lines can be configured to be edge sensitive (posedge) or
level sensitive (high level). Hardware interrupts start at ID32.

**10.3.1 Interrupt Distributor overview**

The Interrupt Distributor holds the list of Pending interrupts for each CPU,
and then selects the highest priority interrupt before issuing it to the CPU
interface. Interrupts of equal priority are resolved by selecting the lowest
ID.

The Interrupt Distributor consists of a register-based list of interrupts,
their priorities and activation requirements (CPU targets). In addition the
state of each interrupt on each CPU is held in the associated state storage.

The prioritization logic is physically duplicated for each CPU to enable the
selection of the highest priority for each CPU.

The Interrupt Distributor holds the central list of interrupts, processors and
activation information, and is responsible for triggering software interrupts
to processors.

The CPU Interface acknowledges interrupts and changes interrupt priority masks.

The Interrupt Distributor transmits to the CPU interrupt interfaces their
highest Pending interrupt. It receives back the information that the interrupt
has been acknowledged, and can then change the status of the corresponding
interrupt. The CPU Interface also transmits End of Interrupt Information (EOI),
which enables the Interrupt Distributor to update the status of this interrupt
from Active to Inactive.

```

| .-----------. .------------- Core Acknowledge, and
 .---------->| Decoder | |------------- End Of Interrupt (EOI)
 | '-----------' |------------- from CPU Interface
 | ^ ^ |-------------
 | | | |
 V V V V Top priority interrupts
 .-----------.----------.--------.---------.
 -->| | Priority | Status | | .--------.------.
 -->| | Priority | Status | |-->| Irq.no | Prio |--> CPU0
 -->| | Priority | Status | Priori- | '--------'------'
 -->| Interrupt | Priority | Status | zation | .--------.------.
 -->| Interface | Priority | Status | and |-->| Irq.no | Prio |--> CPU1
 -->| | Priority | Status | Select- | '--------'------'
 -->| | Priority | Status | ion | .--------.------.
 -->| | Priority | Status | |-->| Irq.no | Prio |--> CPU2
 -->| | Priority | Status | | '--------'------'
 -->| | Priority | Status | | .--------.------.
 -->| | Priority | Status | |-->| Irq.no | Prio |--> CPU3
 -->| | Priority | Status | | '--------'------'
 '-----------'----------'--------'---------' IRQ's to
 Interrupt List each CPU |
| --- |

**10.3.2 Behavior of the Interrupt Distributor**

When the Interrupt Distributor detects an interrupt assertion, it sets the
status of the interrupt for the targeted MP11 CPUs to Pending. Level-triggered
interrupts cannot be marked as Pending if they are already Active for at least
one MP11 CPU.

For each MP11 CPU the prioritization and selection block searches for the
Pending interrupt with the highest priority. This interrupt is then sent with
its priority to the CPU Interface.

The CPU Interface returns information to the Distributor when the CPU
acknowledges (Pending to Active transition) or clears an interrupt (Active to
Inactive transition). With the given interrupt ID, the Interrupt Distributor
updates the status of this interrupt according to the information sent by the
CPU Interface.

When an interrupt is triggered by the Software Interrupt Register or the
Set-pending Register, the status of that interrupt for the targeted CPU or CPUs
is set to Pending. This interrupt then has the same behavior as a hardware
interrupt. The distributor does not differentiate between software and hardware
triggered interrupts.

| ARM Vector Floating-point Unit (VFP) |
| --- |

The VFP unit exists on 3DS ARM11.

ARM VFP Floating Point Registers

ARM VFP Floating Point Control/Status Registers

ARM VFP Floating Point Opcode Encoding

ARM VFP Floating Point Maths Opcodes

ARM VFP Floating Point Load/Store Opcodes

**Floating Point**

The floating point hardware is called VFPv2 (Vector Floating-point).

```

| ARM DDI 0100I ARM Architecture Reference Manual (for ARMv6 wirh VFPv2)
 ARM DDI 0360F ARM11 MPCore r2p0, contains more (mostly useless) VFPv2 info |
| --- |

The Fxxxx floating point opcodes are aliases for CP10/CP11 copressor numbers;
CP10 used for single, and CP11 for double precision instructions.

| ARM VFP Floating Point Registers |
| --- |

**Floating point Registers**

Registers S0-S31 can contain Single-precision float values, or 32bit Integers
(for conversion to/from float format), or a pair of two Single-precision
registers can contain one Double-precision float value.

```

| Scalar Bank Vector bank 1 Vector bank 2 Vector bank 3
 S1:S0 D0 S9:S8 D4 S17:S16 D8 S25:S24 D12
 S2:S3 D1 S11:S10 D5 S19:S18 D9 S27:S26 D13
 S5:S4 D2 S13:S12 D6 S21:S20 D10 S29:S28 D14
 S7:S6 D3 S15:S14 D7 S23:S22 D11 S31:S30 D15 |
| --- |

The VFP supports "Scalar" and "Vector" modes (and a mixed "Vector/Scalar"
mode).

The "Vector" mode can perform simultaneous operations on up to 8 singles, or up
to 4 doubles (via Vector LEN and STRIDE selected in FPSCR register).

The registers are organized in "banks", and vectors cannot cross banks (eg.
using operand S23 with LEN=3, STRIDE=2 would use registers S23,S17,S19).

**Scalar Mode, Fd=Fm <op> Fn**

The "Scalar" mode performs operations on 1 single or double. This done in any
of the following situations:

```

| - When FPSCR register is set to Vector LEN=1 (and STRIDE=1), or
 - When Destination is S0..S7 or D0..D3 (scalar bank), or
 - When using FCMP comparision opcodes, or
 - When using FCVT or FxxTOxx conversion opcodes, or
 - When using FMxxRxx register transfer opcodes, or
 - When using FLDxx/FSTxx load/store (whereof, FLDM/FSTM can transfer multiple
 registers in vector-like fashion; regardless of LEN/STRIDE settings) |
| --- |

**Vector Mode, Fd[LEN]=Fm[LEN] <op> Fn[LEN]**

The vector mode does merely perform the selected operation on all array
elements, this is correct for cases like Vector+Vector addition, but incorrect
for Vector*Vector multiplication (to get the final result one must manually
compute the sum of the results).

```

| - When FPSCR register is set to Vector LEN=2..8 (and STRIDE=1..2), and
 - When Source and Destination are S8..S31 or D4..D15 (vector banks), and
 - When using FADD, FSUB, FDIV, FCPY, FABS, FNEG, FSQRT, or FxMxx multiply |
| --- |

**Mixed Mode, Fd[LEN]=Fm <op> Fn[LEN]**

This allows to add/multiply/etc. all elements of a vector by a scalar value.
This is done when combining vectors operands with the following:

```

| - When Source operand Fm is S0..S7 or D0..D3 (scalar bank), and
 - othersise same conditions as for Vector mode |
| --- |

**Integer Format (S0..S31 aka I0..I31)**

```

| 31-0 Integer (signed or unsigned, depending on FxxTOxx opcode) |
| --- |

The VFP can't do integer maths, however, one can load/store integer values in
S0..S31, and then use the FxxTOxx opcodes to convert integers to/from float
format. The integers are always 32bit (no matter if converting Single/Double
precision float values).

**Single Precision Registers (float1.8.23) (S0..S31)**

```

| 31 1bit Sign (0=Positive, 1=Negative)
 30-23 8bit Exponent (01h..FEh=for 2^(N-7Fh), or 00h/FFh=Special)
 22-0 23bit Fraction (0..7FFFFFh) |
| --- |

**Double Precision Registers (float1.11.52) (D0..D15)**

```

| 63 1bit Sign (0=Positive, 1=Negative)
 62-52 11bit Exponent (001h..7FEh=for 2^(N-3FFh), or 000h/7FFh=Special)
 51-0 52bit Fraction (0..FFFFFFFFFFFFFh) |
| --- |

**Exponent 01h..FEh (Single) or 001h..7FEh (Double):**

```

| Sign * 2^(exponent-7Fh) * (1.fraction) ;Single
 Sign * 2^(exponent-3FFh) * (1.fraction) ;Double |
| --- |

**Exponent 00h (Single) or 000h (Double), aka Small Numbers and Zero:**

```

| Sign * 2^(-7Eh) * (0.fraction) ;Single
 Sign * 2^(-3FEh) * (0.fraction) ;Double
 The above includes 0 being encoded as fraction=0, the sign bit is ignored
 for cases like "compare +/-0", but the sign is used for "divide by +/-0".
 Small numbers in 0.fraction format may require extra clock cycles for
 counting leading zeroes; unknown if that problem does actually exist on
 ARM hardware, however, the "flush to zero" feature (see FPSCR.bit24) can
 be used to avoid that issue; 0.fraction will be then replaced by 0.000. |
| --- |

**Exponent FFh (Single) or 7FFh (Double), aka NaN's and Infinite:**

```

| fraction=000000h or 0000000000000h +/-Infinite
 fraction=000001h..3FFFFFh or 0000000000001h..7FFFFFFFFFFFFh +/-Signaling NaNs
 fraction=400000h or 8000000000000h +/-Default NaN
 fraction=400000h..7FFFFFh or 8000000000000h..FFFFFFFFFFFFFh +/-Quite NaNs
 NaNs (Not a Number) can be used for abstract non-numeric expressions; this
 isn't useful for normal maths, but may be useful if a database contains
 entries like "Weight=UNKNOWN". If so, one may handle the NaN before passing
 it to the floating point unit, or otherwise the hardware will either trigger
 an exception (Signaling NaNs) or leave the NaN unchanged (Quite NaNs),
 eg. "UNKNOWN*2+3 = UNKNOWN", or replace it by Default NaN (if FPSCR.bit25=1).
 Different NaNs can be compared using integer comparisons, float comparisions
 of NaNs have "unordered" results (even when comparing a NaN with itself). |
| --- |

**Multiply Note**

When multiplying vector*vector, the hardware does merely multiply the
components (without computing the sum of the multiply results). To get sum, one
could use FMUL and several FADD's with different strides. Or better, for
multiple vector*vector multiplications, use FMUL and several FMAC's with source
data rearranged as so:

```

| FMUL (X,X,X,X,X,X,X,X)*(X,X,X,X,X,X,X,X)
 FMAC (Y,Y,Y,Y,Y,Y,Y,Y)*(Y,Y,Y,Y,Y,Y,Y,Y)
 FMAC (Z,Z,Z,Z,Z,Z,Z,Z)*(Z,Z,Z,Z,Z,Z,Z,Z)
 FMAC (W,W,W,W,W,W,W,W)*(W,W,W,W,W,W,W,W) |
| --- |

Ie. in that case, the 1st "vector" contains the X components from up to eight
(X,Y,Z,W) vectors.

| ARM VFP Floating Point Control/Status Registers |
| --- |

**FPSID Register (Floating Point System ID) (R)**

```

| 31-24 Implementor code (41h=ARM)
 23 Hardware/software implementation (0=Hardware, 1=Software)
 22-21 FSTMX/FLDMX format (0=Format 1, Other=Reserved)
 20 Supported Precision (0=Single and Double, 1=Single only)
 19-16 Architecture version number (0=VFPv1, 1=VFPv2, 2-15=Reserved)
 15-8 Primary part number of VFP implementation (20h=VFP11) ;\Implementation
 7-4 Variant number (0Bh=MPCore); defined
 3-0 Revision number of the part (04h=Fourth);/ |
| --- |

New3DS: 410120b4h = VFPv2 D variant (with single AND double precision).

**FPSCR Register (Floating Point Status/Control Register for user-level) (R/W)**

```

| 31 N Flag (1=Comparision result is Less Than)
 30 Z Flag (1=Comparision result is Equal)
 29 C Flag (1=Comparision result is Equal, Greater Than, or Unordered)
 28 V Flag (1=Comparision result is Unordered)
 Note: Use FMSTAT opcode to transfer above flags to ARM CPSR flags
 27-26 Unused (0)
 25 Default Nan mode (XXX see page C2-16) (0=Disable, 1=Enable)
 24 Flush-to-zero mode (XXX see page C2-14) (0=Disable, 1=Enable)
 23-22 Rounding mode (0=To Nearest, 1=Up, 2=Down, 3=Towards Zero)
 21-20 Vector Stride (0/3 = 1/2 Singles; or 0/3 = 1/2 Doubles) (1/2=Reserved)
 19 Unused (0)
 18-16 Vector Len (0..7 = 1..8 Singles; or 0..3 = 1..4 Doubles)
 15 Trap Enable Input Denormal (aka Subnormal) ;\
 14-13 Unused (0) ;
 12 Trap Enable Inexact ; Trap Enable aka
 11 Trap Enable Underflow ; Exception Enable
 10 Trap Enable Overflow ;
 9 Trap Enable Division by Zero ;
 8 Trap Enable Invalid Operation ;/
 7 Cumulative Exception Input Denormal ;\
 6-5 RES ;
 4 Cumulative Exception Inexact ; Cumulative what...?
 3 Cumulative Exception Underflow ;
 2 Cumulative Exception Overflow ;
 1 Cumulative Exception Division by Zero ;
 0 Cumulative Exception Invalid Operation ;/ |
| --- |

**FPEXC Register (Floating Point Exception Register for system-level) (R/W)**

```

| 31 Exception Flag ... long blurb replated to process swap code
 30 Enable Floating Point Instructions (0=Disable, 1=Enable)
 29-0 Sub-architecture defined (see below for mpcore)
 Extra mpcore bits:
 29 Unused (0)
 28 FPINST2 instruction valid flag
 27-11 Unused (0)
 10-8 VECITR Number of remaining iterations after exception (0..6=1..7, 7=0)
 7 INV Input exception flag
 6-4 Unused (0)
 3 UFC Potential Underflow Flag
 2 OFC Potential Overflow Flag
 1 Unused (0)
 0 IOC Potential invalid operation flag |
| --- |

The exception handler must clear bit31 and bit28.

**FPINST - Floating-Point Instruction Register, Privileged 0xEE000A00 (R/W)**

Contains the opcode that has triggered the exception. The Cond field in
bit28-31 is changed to 0Eh (Always), and the Fd:D, Fn:N, Fm:M are changed to
indicated the fault-location within a vector (with FPEXC.bit8-10 indicating the
remaining unprocessed elements of the vector).

**FPINST2 - Floating-Point Instruction Register 2, Privileged UNP (R/W)**

If FPEXC.bit28=1, then this register contains another float opcode (that was
prefetched, but not yet executed). The Cond field in bit28-31 is changed to 0Eh
(Always). The exception handler should handle the failed FPINST opcode, then
try to execute prefetched FPINST2 opcode, and then return from exception.

**MVFR0, Media and VFP Feature Register 0, Any 0x11111111 (R)**

```

| 31-28 VFP hardware support level when user traps are disabled
 (01h=In MPCore processors when Flush-to-Zero and Default_NaN and
 Round-to-Nearest are all selected in FPSCR, the coprocessor does not
 require support code. Otherwise floating-point support code is
 required)
 27-24 Support for short vectors (01h=Yes)
 23-20 Support for hardware square root (01h=Yes)
 19-16 Support for hardware divide (01h=Yes)
 15-12 Support for software/user traps (01h=Yes/support code is required)
 11-8 Support for double precision VFP (01h=Yes, v2)
 7-4 Support for single precision VFP (01h=Yes, v2)
 3-0 Support for the media register bank (01h=Yes/support 16, 64bit regs) |
| --- |

**MVFR1 - Media and VFP Feature Register 1, Any 0x00000000 (R)**

```

| 31-28 Reserved
 11-8 Support for media extension, single precision floating-point (00h=No)
 7-4 Support for media extension, integer instructions (00h=No)
 3-0 Support for media extension, load/store instructions (00h=No) |
| --- |

| ARM VFP Floating Point Opcode Encoding |
| --- |

**Comparision of normal ARM copro opcodes and VFP opcodes**

```

| |..3 ..................2 ..................1 ..................0|
 |1_0_9_8_7_6_5_4_3_2_1_0_9_8_7_6_5_4_3_2_1_0_9_8_7_6_5_4_3_2_1_0|
 |_Cond__|1_1_0_0_0_1_0|L|__Rn___|__Rd___|__CP#__|_CPopc_|__CRm__| 2reg normal
 |_Cond__|1_1_0_0_0_1_0|L|__Rn___|__Rd___|__CP#__|0|0|M|1|__Fm___| 2reg on VFP
 |_Cond__|1_1_0|P|U|N|W|L|__Rn___|__CRd__|__CP#__|____Offset_____| Mem normal
 |_Cond__|1_1_0|P|U|D|W|L|__Rn___|__Fd___|__CP#__|____Offset_____| Mem on VFP
 |_Cond__|1_1_1_0|_CPopc_|__CRn__|__CRd__|__CP#__|_CP__|0|__CRm__| CDP normal
 |_Cond__|1_1_1_0|p|D|q|r|__Fn___|__Fd___|__CP#__|N|s|M|0|__Fm___| CDP on VFP
 |_Cond__|1_1_1_0|CPopc|L|__CRn__|__Rd___|__CP#__|_CP__|1|__CRm__| 1reg normal
 |_Cond__|1_1_1_0|CPopc|L|__Fn___|__Rd___|__CP#__|N|0_0|1|0_0_0_0| 1reg on VFP |
| --- |

```

| Cond = Condition
 L = Load/Store direction for memory/register transfers
 Fm:M, Fn:N, Fd:D = Float Registers S0..S31 (or D0..D15, with LSB=0)
 Rd, Rn = ARM Registers
 PUW, pqrs, CPopc = Opcode bits
 CP# = Coprocessor number (0Ah=Single-, 0Bh=Double-Precision)
 Offset = Address step, implies number of registers for FLDM/FSTM |
| --- |

| ARM VFP Floating Point Maths Opcodes |
| --- |

**VFP data-processing primary opcodes**

```

| pqrs cp10/cp11 Instruction functionality
 0000 FMAC{S|D}{cond} Fd,Fn,Fm Fd = +(Fn*Fm)+Fd ;Multiply, Add
 0001 FNMAC{S|D}{cond} Fd,Fn,Fm Fd = -(Fn*Fm)+Fd ;Multiply, Negate, Add
 0010 FMSC{S|D}{cond} Fd,Fn,Fm Fd = +(Fn*Fm)-Fd ;Multiply, Subtract
 0011 FNMSC{S|D}{cond} Fd,Fn,Fm Fd = -(Fn*Fm)-Fd ;Multiply, Negate, Sub
 0100 FMUL{S|D}{cond} Fd,Fn,Fm Fd = +(Fn*Fm) ;Multiply
 0101 FNMUL{S|D}{cond} Fd,Fn,Fm Fd = -(Fn*Fm) ;Multiply, Negate
 0110 FADD{S|D}{cond} Fd,Fn,Fm Fd = Fn+Fm ;Add
 0111 FSUB{S|D}{cond} Fd,Fn,Fm Fd = Fn-Fm ;Sub
 1000 FDIV{S|D}{cond} Fd,Fn,Fm Fd = Fn/Fm ;Divide
 1001 -Undefined-
 1010 -Undefined-
 1011 -Undefined-
 1100 -Undefined-
 1101 -Undefined-
 1110 -Undefined-
 1111 -Extension instructions- |
| --- |

**VFP data-processing extension opcodes**

```

| Fn N cp10/cp11 Instruction functionality
 0000 0 FCPY{S|D}{cond} Fd,Fm Fd = Fm ;Copy
 0000 1 FABS{S|D}{cond} Fd,Fm Fd = abs(Fm) ;Absolute
 0001 0 FNEG{S|D}{cond} Fd,Fm Fd = -Fm ;Negate
 0001 1 FSQRT{S|D}{cond} Fd,Fm Fd = sqrt(Fm) ;Square root
 001x x -Undefined-
 0100 0 FCMP{S|D}{cond} Fd,Fm Fd-Fm ;Compare
 0100 1 FCMPE{S|D}{cond} Fd,Fm Fd-Fm ;Compare, exception on quiet NaNs
 0101 0 FCMPZ{S|D}{cond} Fd Fd-0 ;Compare
 0101 1 FCMPEZ{S|D}{cond} Fd Fd-0 ;Compare, exception on quiet NaNs
 0110 x -Undefined-
 0111 0 -Undefined-
 0111 1 FCVT{DS|SD}{cond} Fd,Fm Single <--> Double-precision conversion
 1000 0 FUITO{S|D}{cond} Fd,Im Unsigned integer --> float
 1000 1 FSITO{S|D}{cond} Fd,Im Signed integer --> float
 1001 x -Undefined-
 101x x -Undefined-
 1100 0 FTOUI{S|D}{cond} Id,Fm Float --> unsigned integer
 1100 1 FTOUIZ{S|D}{cond} Id,Fm Float --> unsigned integer, round to zero
 1101 0 FTOSI{S|D}{cond} Id,Fm Float --> signed integer
 1101 1 FTOSIZ{S|D}{cond} Id,Fm Float --> signed integer, round to zero
 111x x -Undefined- |
| --- |

cp10: FCVTDS Dd,Sm ;Double <-- Single

cp11: FCVTSD Sd,Dm ;Single <-- Double

**Nocash syntax**

The useless {S|D} and {DS|SD} suffixes are ommitted. FCVT is renamed to FMOV.
F{UI|SI}TO{UI|SI}{Z} is renamed to FMOV{UI|SI}{Z}, with operand I0..I31 for the
integer register.

| ARM VFP Floating Point Load/Store Opcodes |
| --- |

**VFP single register transfer instructions**

```

| cp opcode L Instruction name Instruction functionality
 cp10 000 0 FMSR{cond} Sn,Rd Sn = Rd ;\Single-Precision or Integer
 cp10 000 1 FMRS{cond} Rd,Sn Rd = Sn ;/
 cp10 111 0 FMXR{cond} sys,Rd Reg(Fn,N) = Rd ;\SystemReg (FPSID, etc.)
 cp10 111 1 FMRX{cond} Rd,sys Rd = Reg(Fn,N) ;/ ;<-- or FMSTAT{cond}
 cp11 000 0 FMDLR{cond} Dn,Rd Dn.31-0 = Rd ;\LSW of Double-Precision
 cp11 000 1 FMRDL{cond} Rd,Dn Rd = Dn.31-0 ;/
 cp11 001 0 FMDHR{cond} Dn,Rd Dn.63-32 = Rd ;\MSW of Double-Precision
 cp11 001 1 FMRDH{cond} Rd,Dn Rd = Dn.63-32 ;/
 other's -Undefined- |
| --- |

System Register encodings:

```

| Fn N System register
 0000 0 FPSID (New3DS: 410120b4h = VFPv2 with single AND double precision)
 0001 0 FPSCR ;(FMSTAT opcode encodes as FMRX R15,FPSCR)
 0110 0? MVFR1 ;\mpcore only
 0111 0? MVFR0 ;/
 1000 0 FPEXC
 1001 0? FPINST ;\mpcore only
 1010 0? FPINST2 ;/ |
| --- |

**VFP two register transfer instructions (VFPv2 and above)**

```

| cp L Instruction name Instruction functionality
 cp10 0 FMSRR{cond} {Sm,Sm+1},Rd,Rn Fm = Rn, (Fm+1) = Rd ;XXX swapped?
 cp10 1 FMRRS{cond} Rd,Rn,{Sm,Sm+1} Rn = Fm, Rd = (Fm+1) ;XXX swapped?
 cp11 0 FMDRR{cond} Dm,Rd,Rn Fm[31:0] = Rd, Fm[63:32] = Rn
 cp11 1 FMRRD{cond} Rd,Rn,Dm Rd = Fm[31:0], Rn = Fm[63:32] |
| --- |

**VFP load and store instructions**

```

| PUW L=0/1,cp10/cp11 Registers transferred
 000 -Two-register transfer instructions- -
 001 -Undefined- -
 010 FSTM|FLDMIA{S|D|X}{<cond>} Rn,{Fd,Fd+1,..} Multiple Registers
 011 FSTM|FLDMIA{S|D|X}{<cond>} Rn!,{Fd,Fd+1,..} Multiple Registers Increment
 100 FST|FLD{S|D}{<cond>} Fd, [Rn{,-offs*4}] One register, -offs
 101 FSTM|FLDMDB{S|D|X}{<cond>} Rn!,{Fd,Fd+1,..} Multiple Registers Decrement
 110 FST|FLD{S|D}{<cond>} Fd, [Rn{,+offs*4}] One register, +offs
 111 -Undefined- - |
| --- |

FSTM/FLDM do transfer multiple words (with offs containing the number of words
to be transferred, 1..32 for {S}, or an even number 2..32 for {D}.

VFP load/store multiple addressing modes

```

| Non-stacking mnemonic Stacking mnemonic
 FLDMIA{S|D|X} FLDMFD{S|D|X} FPOP{S|D|X}
 FLDMDB{S|D|X} FLDMEA{S|D|X}
 FSTMIA{S|D|X} FSTMEA{S|D|X}
 FSTMDB{S|D|X} FSTMFD{S|D|X} FPUSH{S|D|X} |
| --- |

**Nocash syntax**

The useless {S|D} suffixes are ommitted, the weird {X} suffix is kept used to
preserve weirdness.

Fancy {} brackets are omitted, LDM/STM must use square [Rn] brackets, the
register list in LDM/STM is specified as Fx-Fz (rather than Fx,Fy,Fz).

All FMxxxx opcodes are renamed to FMOV{LSW|MSW}.

**Weird STM/LDM{X} - for registers with unknown precision**

The weird {X} mode is same as {D}, but with offset.bit0=1 (ie. with offs=3..33
instead of 2..32; and thereby actually transferring an unused dummy word; that
extra word might be intended to contain datatype flags).

The weird {X} mode is/was intended for registers with unknown content (eg. when
pushing/popping registers without knowing if they contain integer/single/double
precision values; which might be a problem with internal accumulators in the
VFP unit).

The weird {X} mode was declared as "deprecated in ARMv6" in DDI 0100I, but
later re-declared as required for "compatibility with future VFP
implementations" in DDI 360F. However, unknown if there are/were/will be any
such implementations that do require it.

For now, it should be best to use {D} mode instead of weird {X}. Probably even
{S} should also work the same (if the endianess-based word-order doesn't
matter).