# Dmacorelinkdmatransferstartakadebugregisters

> Source: https://problemkaputt.de/gbatek.htm
> Section: Dmacorelinkdmatransferstartakadebugregisters

3DS DMA - Corelink DMA - Transfer Start (aka "Debug" Registers) 
| |
| --- |

 
```

| ____________________________ DMAC Debug Registers ____________________________
 (debugging is meant to be a synonym for "start execution") |
| --- |

**10xxxD00h - DBGSTATUS - Debug Status Register (R)**

```

| 0 Debug Status (0=Idle, 1=Busy) ;aka INST0/INST1 busy?
 1-31 Unused (undef) |
| --- |

**10xxxD04h - DBGCMD - Debug Command Register (W) (start INST code)**

```

| 0-1 Execution (0=Execute DBGINST, 1..3=Reserved)
 2-31 Unused (0) |
| --- |

**10xxxD08h - DBGINST0 - Debug Instruction-0 (W) (code[2]+ch+thread)**

**10xxxD0Ch - DBGINST1 - Debug Instruction-1 (W) (byte[4])**

```

| 0 Debug thread (0=DMA Manager thread, 1=DMA channel 0..7) ;\
 1-7 Reserved (0) ; usually
 8-10 DMA channel (0..7 for channel 0..7) (when above bit0=1) ; zero
 11-15 Reserved (0) ;/
 16-23 Instruction Byte 0 ;-usually DMA_GO opcode
 24-31 Instruction Byte 1 ;-usually target_channel for DMA_GO
 32-39 Instruction Byte 2 ;\
 40-47 Instruction Byte 3 ; usually target_address for DMA_GO
 48-55 Instruction Byte 4 ;
 56-63 Instruction Byte 5 ;/ |
| --- |

Examples:

```

| db 00h,00h,DMA_GO,<chn>,<addr32bit> ;<-- start channel code
 db 01h,<chn>,DMA_KILL,0,0,0,0,0 ;<-- kill channel (eg. after fault/hang) |
| --- |