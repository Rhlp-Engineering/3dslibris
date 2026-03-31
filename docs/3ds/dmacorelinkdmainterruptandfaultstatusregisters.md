# Dmacorelinkdmainterruptandfaultstatusregisters

> Source: https://problemkaputt.de/gbatek.htm
> Section: Dmacorelinkdmainterruptandfaultstatusregisters

3DS DMA - Corelink DMA - Interrupt and Fault Status Registers 
| |
| --- |

 
```

| ___________________________ DMAC Control Registers ___________________________ |
| --- |

**10xxx000h - DSR - DMA Manager Status Register (R)**

```

| 0-3 DMA Manager State (see below)
 4-8 DMA Manager DMA_WFE opcode (wait for event) (0..31 = Event 0..31)
 9 DMA Manager Non-Secure state (0=Secure, 1=Non-secure)
 10-31 Unused (undef) |
| --- |

DMA Manager States:

```

| 00h = Stopped
 01h = Executing
 02h = Cache Miss
 03h = Updating PC
 04h = Wait for event (WFE, see bit4-8 for event number)
 05h-0Eh = Reserved
 0Fh = Faulting |
| --- |

**10xxx004h - DPC - DMA Manager Program Counter (R)**

```

| 0-31 DMA Manager Program Counter |
| --- |

**10xxx020h - INTEN - Interrupt Enable Register (R/W)**

```

| 0-31 Mode for DMA_SEV opcode event 0-31 "event_irq_select for event 0..31" |
| --- |

Bit [N] = 0 If the DMAC executes DMASEV for the event-interrupt resource N then
the DMAC signals event N to all of the threads. Set bit [N] to 0 if your system
design does not use irq[N] to signal an interrupt request.

Bit [N] = 1 If the DMAC executes DMASEV for the event-interrupt resource N then
the DMAC sets irq[N] HIGH. Set bit [N] to 1 if your system design requires
irq[N] to signal an interrupt request.

Note: See DMASEV on page 4-14 for information about selecting an event number.

**10xxx024h - INT_EVENT_RIS - Event-Interrupt Raw Status Register (R)**

**10xxx028h - INTMIS - Interrupt Status Register (R)**

```

| 0-31 Event/Interrupt 0-31 Active (0=Inactive/Low, 1=Active/High) |
| --- |

**10xxx02Ch - INTCLR - Interrupt Clear Register (W)**

```

| 0-31 Interrupt 0-31 (0=No change, 1=Clear Interrupt) |
| --- |

**10xxx030h - FSRD - DMA Manager Fault Status Register (R)**

```

| 0 DMA Manager Thread in Faulting state (0=No, 1=Fault)
 1-31 Unused (undef) |
| --- |

**10xxx034h - FSRC - DMA Channel(s) Fault Status Register (R)**

```

| 0-7 Fault Status for channel 0-7 (0=No, 1=Fault)
 8-31 Unused (undef) |
| --- |

**10xxx038h - FTRD - DMA Manager Fault Type Register (R)**

```

| 0 Undefined Instruction (0=No, 1=Fault)
 1 Invalid Instruction Operand (0=No, 1=Fault)
 2-3 Unused (undef)
 4 DMA_GO with inappropriate security permissions (0=No, 1=Fault)
 5 DMA_WFE or DMA_SEC with inappropriate security (0=No, 1=Fault)
 6-15 Unused (undef)
 16 AXI Response on RRESP bus (0=OKAY, 1=EXOKAY/SLVERR/DECERR)
 17-29 Unused (undef)
 30 DMA Abort program-counter location (0=System memory, 1=Debug interface)
 31 Unused (undef) |
| --- |

**10xxx040h+n*4 - FTRn - DMA channel n Fault type (R)**

```

| 0 Undefined Instruction (0=No, 1=Fault)
 1 Invalid Instruction Operand (0=No, 1=Fault)
 2-4 Unused (undef)
 5 DMA_WFE or DMA_SEC with inappropriate security (0=No, 1=Fault)
 6 Non-secure tried DMAWFP,DMALDP,DMASTP,DMAFLUSHP (0=No, 1=Fault)
 7 Non-secure channel tried to change CCR (0=No, 1=Fault)
 8-11 Unused (undef)
 12 MFIFO too small for DMALD/DMAST (0=No, 1=Fault)
 13 MFIFO lacks data for DMAST (0=No, 1=Fault)
 14-15 Unused (undef)
 16 AXI Response on RRESP bus, opcode (0=OKAY, 1=EXOKAY/SLVERR/DECERR)
 17 AXI Response on BRESP bus, data.write (0=OKAY, 1=EXOKAY/SLVERR/DECERR)
 18 AXI Response on RRESP bus, data.read (0=OKAY, 1=EXOKAY/SLVERR/DECERR)
 19-29 Unused (undef)
 30 DMA Abort program-counter location (0=System memory, 1=Debug interface)
 31 DMA channel locked-up because of insufficient resources (0=No, 1=Fault) |
| --- |