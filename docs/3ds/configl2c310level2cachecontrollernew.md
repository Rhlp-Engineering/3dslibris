# Configl2C310Level2Cachecontrollernew

> Source: https://problemkaputt.de/gbatek.htm
> Section: Configl2C310Level2Cachecontrollernew

3DS Config - L2C-310 Level 2 Cache Controller (New3DS) 
| |
| --- |

 **L2C-310 r3p3, Level 2 Cache Controller (New3DS only)**
 
```

| 17E10000h L2C_CACHE_ID R 410000C9h ;\Cache ID and Cache Type
 17E10004h L2C_CACHE_TYPE R 9E440440h ;/
 17E10100h L2C_CONTROL RW 00000000h ;\
 17E10104h L2C_AUX_CONTROL RW 02090000h ; Control
 17E10108h L2C_TAG_RAM_CONTROL RW 00000111h ;
 17E1010Ch L2C_DATA_RAM_CONTROL RW 00000221h ;/
 17E10200h L2C_EV_COUNTER_CTRL RW 00000000h ;\
 17E10204h L2C_EV_COUNTER1_CFG RW 00000000h ; Interrupt and
 17E10208h L2C_EV_COUNTER0_CFG RW 00000000h ; Counter Control
 17E1020Ch L2C_EV_COUNTER1 RW 00000000h ;
 17E10210h L2C_EV_COUNTER0 RW 00000000h ;
 17E10214h L2C_INT_MASK RW 00000000h ; ;\
 17E10218h L2C_INT_STATUS_MASKED R 00000000h ; ; Interrupts
 17E1021Ch L2C_INT_STATUS_RAW R 00000000h ; ;
 17E10220h L2C_INT_CLEAR W 00000000h ;/ ;/
 17E10730h L2C_CACHE_SYNC RW 00000000h ;\
 17E10770h L2C_INV_PA RW 00000000h ;
 17E1077Ch L2C_INV_WAY RW 00000000h ; Cache Maintenance
 17E107B0h L2C_CLEAN_PA RW 00000000h ; Operations
 17E107B8h L2C_CLEAN_INDEX RW 00000000h ;
 17E107BCh L2C_CLEAN_WAY RW 00000000h ;
 17E107F0h L2C_CLEAN_INV_PA RW 00000000h ;
 17E107F8h L2C_CLEAN_INV_INDEX RW 00000000h ;
 17E107FCh L2C_CLEAN_INV_WAY RW 00000000h ;/
 17E10900h+N*8 L2C_D_LOCKDOWN_0..7 RW 00000000h ;\
 17E10904h+N*8 L2C_I_LOCKDOWN_0..7 RW 00000000h ; Cache Lockdown
 17E10950h L2C_LOCK_LINE_EN RW 00000000h ;
 17E10954h L2C_UNLOCK_WAY RW 00000000h ;/
 17E10C00h L2C_ADDR_FILTERING_START RW 00000000h ;\Address Filtering
 17E10C04h L2C_ADDR_FILTERING_END RW 00000000h ;/
 17E10F40h L2C_DEBUG_CTRL RW 00000004h ;\
 17E10F60h L2C_PREFETCH_CTRL RW 04000000h ; Debug, Prefetch, Power
 17E10F80h L2C_POWER_CTRL RW 00000000h ;/ |
| --- |

Caution: L2C registers can be read via LDR only (LDRB/LDRH cause data abort).

Note: Some L2C registers are write-able only with Secure access (S), not with
Non-secure access (NS), but that seems to apply only for TrustZone extension
(which doesn't seem to exist in 3DS).

Official specs: DDI0246H_l2c310_r3p3_trm.pdf

```

| _____________________ Cache ID and Cache Type (NS and S) _____________________ |
| --- |

**17E10000h - L2C_CACHE_ID - Cache ID Register (410000C9h) (R)**

```

| 0-5 RTL release (9=r3p3)
 6-9 Part number (3=L2C-310)
 10-15 CACHEID pins (reads as 0 on New3DS)
 16-23 Reserved (0)
 24-31 Implementer (41h=ARM) |
| --- |

**17E10004h - L2C_CACHE_TYPE - Cache Type Register (9E440440h) (R)**

```

| 0-1 L2 cache line length (0=32 bytes) ;\
 2-5 Reserved (0) ;
 6 L2 associativity (from L2C_AUX_CONTROL.bit16) ; instruction
 7 Reserved (0) ;
 8-10 Isize L2 cache way size (from L2C_AUX_CONTROL.bit19-17) ;
 11 Reserved (0) ;/
 12-13 L2 cache line length (0=32 bytes) ;\
 14-17 Reserved (0) ;
 18 L2 associativity (from L2C_AUX_CONTROL.bit16) ; data
 19 Reserved (0) ;
 20-22 Dsize L2 cache way size (from L2C_AUX_CONTROL.bit19-17) ;
 23 Reserved (0) ;/
 24 Harvard (0=Unified, 1=Harvard) ;-harvard
 25 Lockdown by Line option (0=Off, 1=On) ;\
 26 Lockdown by Master option (0=Off, 1=On) ; ctype
 27-28 Fixed (always 3) ;/
 29-30 Reserved (0)
 31 Data banking (0=Not implemented, 1=Implemented) |
| --- |

```

| ______________________ Control (Write S, Read NS and S) ______________________ |
| --- |

**17E10100h - L2C_CONTROL - Control Register (R/W)**

```

| 0 L2 Cache enable (0=Disable, 1=Enable)
 1-31 Reserved (0) |
| --- |

Caution: The cache seems to contain garbage on power-up. Set L2C_INV_WAY=FFFFh,
then wait for L2C_INV_WAY=0 before enabling L2C_CONTROL.

**17E10104h - L2C_AUX_CONTROL - Auxiliary Control Register (02090000h) (R/W)**

```

| 0 Full Line of Write Zero Enable (0=Disable, 1=Enable)
 1-7 Reserved (0) (actually R/W, but unknown/undocumented)
 8 Reserved (0) (always 0)
 9 Reserved (0) (actually R/W, but unknown/undocumented)
 10 Priority for Strongly Ordered and Device Reads Enable (0=Low, 1=High)
 11 Limit (0=Device writes can use all slots, 1=Ensure one Memory slot)
 12 Exclusive cache configuration (0=Disable, 1=Enable)
 13 Shared Attribute Invalidate Enable (0=Disable, 1=Enable if no override)
 14-15 Reserved (0) (actually R/W, but unknown/undocumented)
 16 Associativity (0=8-way, 1=16-way)
 17-19 Way-size (1=16K, 2=32K, 3=64K, 4=128K, 5=256K, 6=512K, 0/7=Same as 1/6)
 20 Event monitor bus enable (0=Disable, 1=Enable)
 21 Parity enable (0=Disable, 1=Enable)
 22 Shared attribute override (0=No, 1=Ignore Shared Attrubute)
 23-24 Force write allocate (0=Use WA, 1=ForceWA=0, 2=ForceWA=1, 3=Same as 0?)
 25 Cache Replacement Policy (0=Pseudo-random/LFSR, 1=Round-robin)
 26 Lockdown Register Writes (0=Secure only, 1=Allow non-secure)
 27 Interrupt MASK/CLEAR Access (0=Secure only, 1=Allow non-secure)
 28 Data Prefetch Enable (0=Disable, 1=Enable)
 29 Instruction Prefetch Enable (0=Disable, 1=Enable)
 30 Early BRESP Enable (0=Disable, 1=Enable, Early write response)
 31 Reserved (0) (actually R/W, but unknown/undocumented) |
| --- |

Cache size is reportedly "2MB" on New3DS, which is probably meant to be 2Mbyte,
ie. 16 ways of 128Kbyte each, and that shared for both code and data caching?

**17E10108h - L2C_TAG_RAM_CONTROL - Tag RAM Latency Control (00000111h) (R/W)**

**17E1010Ch - L2C_DATA_RAM_CONTROL - Data RAM Latency Control (00000221h) (R/W)**

```

| 0-2 RAM setup latency (0-7 = 1..8 cycles of latency)
 3 Reserved (0)
 4-6 RAM read access latency (0-7 = 1..8 cycles of latency)
 7 Reserved (0)
 8-10 RAM write access latency (0-7 = 1..8 cycles of latency)
 11-31 Reserved (0) |
| --- |

Uh, is that the latency of the Cache Memory (ie. not the external "cached"
memory)?

```

| __________________ Interrupt and Counter Control (NS and S) __________________ |
| --- |

**17E10200h - L2C_EV_COUNTER_CTRL - Event Counter Control (R/W)**

```

| 0 Event Counting Enable (0=Disable, 1=Enable) (R/W)
 1 Event Counter 0 Reset (0=No change, 1=Reset) (W)
 2 Event Counter 1 Reset (0=No change, 1=Reset) (W)
 3-31 Reserved (0) |
| --- |

**17E10204h - L2C_EV_COUNTER1_CFG - Event Counter 1 Configuration (R/W)**

**17E10208h - L2C_EV_COUNTER0_CFG - Event Counter 0 Configuration (R/W)**

```

| 0-1 Event counter interrupt generation (00h-03h, see below)
 2-5 Counter event source (00h-0Fh, see below)
 6-31 Reserved (0) |
| --- |

Event counter interrupt generation:

```

| 00h Disabled ;count, without irq
 01h Enabled: Increment condition ;count, with irq on any increment
 02h Enabled: Overflow condition ;count, with irq on overflow
 03h Interrupt generation is disabled ;count, without irq (same as 0?) |
| --- |

Counter event source:

```

| 00h - Counter Disabled
 01h CO Eviction, CastOUT, of a line from the L2 cache
 02h DRHIT Data read hit in the L2 cache
 03h DRREQ Data read lookup to the L2 cache
 04h DWHIT Data write hit in the L2 cache
 05h DWREQ Data write lookup to the L2 cache
 06h DWTREQ Data write lookup to the L2 cache with Write-Through attribute
 07h IRHIT Instruction read hit in the L2 cache
 08h IRREQ Instruction read lookup to the L2 cache
 09h WA Allocation into the L2 cache caused by a write, with
 Write-Allocate attribute, miss
 0Ah IPFALLOC Allocation of a prefetch generated by L2C-310 into the L2 cache
 0Bh EPFHIT Prefetch hint hits in the L2 cache
 0Ch EPFALLOC Prefetch hint allocated into the L2 cache
 0Dh SRRCVD Speculative read received by slave port(s)
 0Eh SRCONF Speculative read confirmed in slave port(s)
 0Fh EPFRCVD Prefetch hint received by slave port(s)
 Note: All REQ lookups will subsequently result in a hit or miss. |
| --- |

**17E1020Ch - L2C_EV_COUNTER1 - Event counter 1 value (R/W)**

**17E10210h - L2C_EV_COUNTER0 - Event counter 0 value (R/W)**

```

| 0-31 Counter value, incremented on selected event |
| --- |

If a counter reaches its maximum value, it saturates at that value until it is
reset.

**17E10214h - L2C_INT_MASK - Interrupt Mask (0=Disable, 1=Enable) (R/W)**

**17E10218h - L2C_INT_STATUS_MASKED - Masked Interrupt Status Register (R)**

**17E1021Ch - L2C_INT_STATUS_RAW - Raw Interrupt Status (1=IRQ) (R)**

**17E10220h - L2C_INT_CLEAR - Interrupt Clear (0=No change, 1=Clear) (W)**

```

| 0 ECNTR: Event Counter 0 and 1 Overflow/Increment
 1 PARRT: Parity Error on L2 tag RAM, Read
 2 PARRD: Parity Error on L2 data RAM, Read
 3 ERRWT: Error on L2 tag RAM, Write
 4 ERRWD: Error on L2 data RAM, Write
 5 ERRRT: Error on L2 tag RAM, Read
 6 ERRRD: Error on L2 data RAM, Read
 7 SLVERR: SLVERR from L3
 8 DECERR: DECERR from L3
 9-31 Reserved (0) |
| --- |

Note: STATUS_MASKED is same as STATUS_RAW, but ANDed with MASK.

The IRQ triggers interrupt 76h.

```

| ________________________ Cache Maintenance Operations ________________________ |
| --- |

**17E10730h - L2C_CACHE_SYNC - Cache Maintenance Operations (R and W)**

```

| 0 C, When writing: Must be 0 (trigger cache sync...?)
 0 C, When reading: Background/Way operation is in progress (0=No, 1=Yes)
 1-31 Reserved (0) |
| --- |

**17E10770h - L2C_INV_PA - Invalidate by Physical Address (R/W)**

**17E107B0h - L2C_CLEAN_PA - Clean by Physical Address (R/W)**

**17E107F0h - L2C_CLEAN_INV_PA - Clean+Invalidate by Physical Address (R/W)**

```

| 0 C (uh, is that same meaning as in L2C_CACHE_SYNC.bit0?) (R?)
 1-4 Reserved (0)
 5-xx Index (W?)
 xx-31 Tag (W?) |
| --- |

**17E1077Ch - L2C_INV_WAY - Invalidate by Way (R/W)**

**17E107BCh - L2C_CLEAN_WAY - Clean by Way (R/W)**

**17E107FCh - L2C_CLEAN_INV_WAY - Clean+Invalidate by Way (R/W)**

```

| 0-15 Way bits (for way 0..15) (1=Trigger/Busy?) (R/W)
 16-31 Reserved (0) |
| --- |

**17E107B8h - L2C_CLEAN_INDEX - Clean by Index (R/W)**

**17E107F8h - L2C_CLEAN_INV_INDEX - Clean+Invalidate by Index (R/W)**

```

| 0 C (uh, is that same meaning as in L2C_CACHE_SYNC.bit0?) (R?)
 1-4 Reserved (0)
 5-xx Index (W?)
 xx-xx Reserved (0)
 28-31 Way number (0..15) (W?) |
| --- |

```

| _______________________________ Cache Lockdown _______________________________ |
| --- |

**17E10900h+N*8 - L2C_D_LOCKDOWN_0..7 - Data Cache lockdown 0-7 (R/W)**

**17E10904h+N*8 - L2C_I_LOCKDOWN_0..7 - Instruction Cache lockdown 0-7 (R/W)**

```

| 0-15 DATALOCK/INSTRLOCK 000..111 (use when AyUSERSx[7:5]=000b..111b) ;way?
 16-31 Reserved (0) |
| --- |

**17E10950h - L2C_LOCK_LINE_EN - Lockdown by Line Enable (R/W)**

```

| 0 Lockdown by Line Enable (0=Disable, 1=Enable)
 1-31 Reserved (0) |
| --- |

**17E10954h - L2C_UNLOCK_WAY - Unlock all Lines by Way (R/W)**

```

| 0-15 Unlock all Lines by Way operation (0=No/off, 1=Unlock/Busy) ;way0-15
 16-31 Reserved (0) |
| --- |

Caution: When busy, all other L2C registers are becoming readonly (so after
writing, one MUST wait until above busy flags go off).

```

| _________________ Address Filtering (Write S, Read NS and S) _________________ |
| --- |

**17E10C00h - L2C_ADDR_FILTERING_START - Address filtering Start (R/W)**

**17E10C04h - L2C_ADDR_FILTERING_END - Address filtering End (R/W)**

Not implemented, always zero in New3DS. Allows to redirect a whole address
range to master 1 (when two masters are implemented).

```

| 0 Address Filtering Enable (0=Disable, 1=Enable) ;<-- in Start register
 0 Reserved (0) ;<-- in End register
 1-19 Reserved (0)
 20-31 Address Filtering Start/End Address bit31-20 |
| --- |

```

| _____________ Debug, Prefetch and Power (Write S, Read NS and S) _____________ |
| --- |

**17E10F40h - L2C_DEBUG_CTRL - Debug Register (00000004h) (R/W)**

```

| 0 Disable cache linefill (0=Enable, 1=Disable cache linefills) (R/W)
 1 Disable write-back (0=Write-back, 1=Force Write-through) (R/W)
 2 Secure Privileged Non-Invasive Debug Enable SPNIDEN option (1=on?) (R)
 3-31 Reserved (0) |
| --- |

**17E10F60h - L2C_PREFETCH_CTRL - Prefetch Control Register (04000000h) (R/W)**

```

| 0-4 Prefetch Offset (must be 0-7, 15, 23, or 31) (other=Unsupported)
 5-20 Reserved (0) (always 0)
 21 Not same AXI ID on exclusive sequence enable (0=Same ID, 1=Not same)
 22 Reserved (0) (actually R/W, but unknown/undocumented)
 23 Incr Double Linefill enable (allow 8x64bit) (0=Disable, 1=Allow)
 24 Prefetch drop, Discard prefetch reads to L3 (0=Disable, 1=Enable)
 25 Reserved (0) (actually R/W, but unknown/undocumented)
 26 Speculative Read Synthesis Option (read-only) (0=On, 1=Off) (R)
 27 Double linefill on WRAP read disable (0=Enable, 1=Disable)
 28 Data prefetch enable (0=Disable, 1=Enable)
 29 Instruction prefetch enable (0=Disable, 1=Enable)
 30 Double Linefill, Read bursts to L3 on L2 miss (0=4x64bit, 1=8x64bit)
 31 Reserved (0) (actually R/W, but unknown/undocumented) |
| --- |

**17E10F80h - L2C_POWER_CTRL - Power Control Register (R/W)**

```

| 0 Standby mode enable (0=Disable, 1=Enable)
 1 Dynamic clock gating enable (0=Disable, 1=Enable)
 2 Reserved (0) (actually R/W, but unknown/undocumented)
 3-31 Reserved (0) (always 0) |
| --- |