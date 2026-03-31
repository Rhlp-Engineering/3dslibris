# Dmacorelinkdmainternalstatestatusregisters

> Source: https://problemkaputt.de/gbatek.htm
> Section: Dmacorelinkdmainternalstatestatusregisters

3DS DMA - Corelink DMA - Internal State Status Registers 
| |
| --- |

 
```

| ____________________ DMAC Channel Thread Status Registers ____________________ |
| --- |

**10xxx100h+n*8 - CSRn - DMA channel n Channel status (R)**

```

| 0-3 Channel Status (see below)
 4-8 Wakeup number (0-31, channel is waiting for event/peripheral 0-31)
 9-13 Unused (undef)
 14 DMAWFP executed with single/burst operand set (0=Single, 1=Burst)
 15 DMAWFP executed with/without periph operand set (0=Without, 1=With)
 16-20 Unused (undef)
 21 Channel operates in Non-Secure state (0=Secure, 1=Non-secure)
 22-31 Unused (undef) |
| --- |

Channel Status:

```

| 00h = Stopped
 01h = Executing
 02h = Cache miss
 03h = Updating PC
 04h = Waiting for event
 05h = At barrier
 06h = reserved
 07h = Waiting for peripheral
 08h = Killing
 09h = Completing
 0Ah-0Dh = reserved
 0Eh = Faulting completing
 0Fh = Faulting |
| --- |

**10xxx104h+n*8 - CPCn - DMA channel n Channel PC (R)**

```

| 0-31 DMA Channel's Program Counter |
| --- |

```

| _________________ DMAC AXI Status and Loop Counter Registers _________________ |
| --- |

**10xxx400h+n*20h - SARn - DMA channel n Source address (R)**

**10xxx404h+n*20h - DARn - DMA channel n Destination address (R)**

```

| 0-31 DMA Channel's Source/Destination memory address |
| --- |

**10xxx408h+n*20h - CCRn - DMA channel n Channel control (R)**

```

| 0 Source Burst type (0=Fixed address, 1=Incrementing address)
 1-3 Source Burst size (0-4 = 1,2,4,8,16 bytes per beat) (5-7=Reserved)
 4-7 Source Burst len (0-15 = 1..16 data transfers)
 8 Source ARPROT.bit0 Privileged (0=Unprivileged, 1=Privileged) ;\access
 9 Source ARPROT.bit1 Non-secure (0=Secure, 1=Non-secure) ; type
 10 Source ARPROT.bit2 Instruction (0=Data, 1=Instruction) ;/
 11-13 Source ARCACHE.bit0-2 ;ARCACHE.bit3 is always low
 14 Dest Burst type (0=Fixed address, 1=Incrementing address)
 15-17 Dest Burst size (0-4 = 1,2,4,8,16 bytes per beat) (5-7=Reserved)
 18-21 Dest Burst len (0-15 = 1..16 data transfers)
 22 Dest AWPROT.bit0 Privileged (0=Unprivileged, 1=Privileged) ;\access
 23 Dest AWPROT.bit1 Non-Secure (0=Secure, 1=Non-secure) ; type
 24 Dest AWPROT.bit2 Instruction (0=Data, 1=Instruction) ;/
 25-27 Dest AWCACHE.bit0,1,3 ;AWCACHE.bit2 is always low
 28-30 Endian swap bytes (0=Off) (1-4=Within 16,32,64,128 bit) (5-7=Reserved)
 31 Unused (undef) |
| --- |

Bit9,23 should/must be both set, else faults (depending on secure mode, or so).

**10xxx40Ch+n*20h - LC0_n - DMA channel n Loop counter 0 (R)**

**10xxx410h+n*20h - LC1_n - DMA channel n Loop counter 1 (R)**

```

| 0-7 Remaining loop count, minus 1 (00h..FFh = Loop 1..256 times)
 8-31 Unused (undef) |
| --- |