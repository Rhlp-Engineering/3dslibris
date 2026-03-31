# I2Cmcurl78Cpuregistersandflags

> Source: https://problemkaputt.de/gbatek.htm
> Section: I2Cmcurl78Cpuregistersandflags

3DS I2C MCU - RL78 CPU Registers and Flags 
| |
| --- |

 **Registers**
 
```

| PC 20bit Program Counter
 PSW 8bit Program Status Word (flags)
 SP 16bit Stack Pointer (bit0=always 0, bit1-15=used)
 AX 16bit aka A:X ;\these can be used as 16bit (eg. AX) or two 8bit
 BC 16bit aka B:C ; registers (eg. A and X).
 DE 16bit aka D:E ; all of these registers exist in 4 banks (the
 HL 16bit aka H:L ;/active bank is selected via "SEL RBn" opcode)
 ES 4bit Data Bank ;-used only for PREFIX'ed opcode, otherwise bank=0Fh
 CS 4bit Code Bank ;-MSBs of destination for JMP/CALL AX/BC/DE/HL |
| --- |

Note: All registers (except PC) are mapped to the SFR register area, and can be
also used by accessing that memory. In case of the banked AX/BC/DE/HL
registers, observe that direct memory access won't recurse the current "bank"
selection (that weird feature/problem is same as for 8051 CPUs).

**RL78 Memory Map**

```

| 00000h 80h Code Vector Table ;\
 00080h 40h Code Callt Table ;
 000C0h 4 Code Option byte area (4 bytes) ; Code
 000C4h 0Ah Code On-chip debug security ID setting area ; FLASH
 000CEh F32h Code ; memory
 01000h 1000h Code (can be hardware-swapped with 00000h..00FFFh?) ;
 02000h 3000h Code ;
 05000h 3000h Code (3DS uses this area as firmware backup copy) ;/
 08000h 8000h Unused, FFh-filled (does NOT seem to be flash)
 10000h DD800h Unused, 00h-filled
 ED800h 2800h ROM area (or A4h,FBh,5Ah,FAh-filled when disabled) ;\
 EFFD0h 2 ROM data, FLASH (size-1)/100h ;007Fh=32Kbyte ; ROM
 EFFF8h 4 ROM code, jump vector for flash functions (c=func) ;/
 F0000h 7E0h Special-function registers (2nd SFRs)
 F07E0h 620h Hidden RAM (for use by ROM) (or 00h-filled when disabled) ;-RAM
 F0E00h 200h Mirror of RAM at FF900h (or 00h-filled when disabled)
 F1000h 7000h Mirror of FLASH ROM code area at 01000h (for [Fxxxxh] data)
 F8000h 78F0h Mirror of unused FFh-filled area at 08000h
 FF8F0h 10h Hidden RAM (for use by ROM) (or FFh-filled when disabled) ;-?
 FF900h 520h RAM
 FFE20h C0h RAM (C0h bytes, short-addressable) ;short addr 20h..DFh
 FFEE0h 20h Registers AX,BC,DE,HL (in four banks) ;short addr E0h..FFh
 FFF00h 20h SFRs (Port 0-15, etc.) ;SFR addr 00h..1Fh ;short addr 00h..1Fh
 FFF20h D8h SFRs ;SFR addr 20h..F7h
 FFFF8h 8 Registers SP,PSW,etc. ;SFR addr F8h..FFh |
| --- |

**RL78 Program Status Word (PSW aka Flags)**

```

| S1 S2/S3
 0 CY CY Carry flag (0=No, 1=Carry/borrow)
 1 ISP0 ISP0 In-service Priority bit0
 2 ISP1 ISP1 In-service Priority bit1
 3 0 RBS0 Register Bank Select bit0 (RL78-S2/S3 only) (used on 3DS)
 4 AC AC Auxilliary Carry flag (aka carry on 4bit nibble)
 5 0 RBS1 Register Bank Select bit1 (RL78-S2/S3 only) (used on 3DS)
 6 Z Z Zero flag (0=No, 1=Zero/Equal)
 7 IE IE Interrupt Enable flag (0=Disable, 1=Enable) |
| --- |

**RL78 Opcode flags**

```

| Opcode ZHC
 ADD/ADDC/SUB/SUBC/CMP/CMPS XXX
 ADDW/SUBW/CMPW ;with dst<>SP XXX
 ADDW/SUBW ;with dst=SP ---
 CMP0 X00
 AND/OR/XOR X--
 INC/DEC XX-
 INCW/DECW ---
 MOVS X-X
 SHR/SHRW/SHL/SHLW/SAR/SARW --X
 ROR/RORC/ROL/ROLC/ROLWC --X
 AND1/OR1/XOR1/NOT1 --X
 MOV1 ---
 MOV/XCH/ONEB/CLRB ---
 MOVW/XCHW/ONEW/CLRW ---
 MULU ---
 SET1/CLR1 ---
 CALL/CALLT/BRK/RET/PUSH/POP ---
 BR/Bcond/SKcond/BTCLR ---
 SEL/NOP/DI/EI/HALT/STOP ---
 RETI/RETB old |
| --- |

Note: Opcodes that use PSW or PSW.n or CY as destination operand do of course
also affect the corresponding flags.

**RL78 Conditional Opcodes**

The CPU supports conditional branch/skip opcodes. Skip does skip the next
opcode if the condition is true (unknown if that is any faster, especially in
case of large 5-byte opcodes). Combining conditional SKx+BT/BF is possible.

```

| Native Mocash
 BC SKC JC/JB SKC/SKB CY=1 (carry, unsigned below)
 BNC SKNC JNC/JAE SKNC/SKAE CY=0 (no carry, unsigned above or equal)
 BZ SKZ JZ/JE SKZ/SKE Z=1 (zero/equal)
 BNZ SKNZ JNZ/JNE SKNZ/SKNE Z=0 (nonzero/not equal)
 BH SKH JA SKA CY=0 and Z=0 (unsigned above/higher)
 BNH SKNH JBE SKBE CY=1 or Z=1 (unsigned below or equal, not higher)
 BF JZ bit=0 (bit false/zero)
 BT JNZ bit=1 (bit true/nonzero)
 BTCLR JNZCLR bit=1 (bit true/nonzero, and auto-clear bit) |
| --- |

There are no conditions for signed overflow/less/greater. Signed
positive/negative can be tested via BF/BT on result.bit7/bit15.

The skip feature could be used as prefix for making conditional opcodes, eg.
"ADD.Z" could be made of "SKNZ+ADD" (to add if zero).

**Stack**

```

| PUSH/POP rp/PSW --> 2 bytes
 CALL/CALLT/RET --> 4 bytes
 Interrupt/RETI --> 4 bytes
 BRK/RETB --> 4 bytes |
| --- |