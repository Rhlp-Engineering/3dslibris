# 80+#*10 --> see below (80,C0)

> Source: https://problemkaputt.de/gbatek.htm
> Section: 80+#*10 --> see below (80,C0)

3DS I2C MCU - RL78 CPU Opcode Map 
| |
| --- |

 **Instruction Map (1st Map) (without prefix)**
 
```

| 00 10 20 30
 00 NOP ADDW SP,nn SUBW SP,nn MOVW AX,nnnn
 01 ADDW AX,AX PREFIX (ES:) - 4th MAP !!!
 02 ADDW AX,!addr16 MOVW BC,AX SUBW AX,!addr16 MOVW BC,nnnn
 03 ADDW AX,BC MOVW AX,BC SUBW AX,BC XCHW AX,BC
 04 ADDW AX,nnnn MOVW DE,AX SUBW AX,nnnn MOVW DE,nnnn
 05 ADDW AX,DE MOVW AX,DE SUBW AX,DE XCHW AX,DE
 06 ADDW AX,saddrp MOVW HL,AX SUBW AX,saddrp MOVW HL,nnnn
 07 ADDW AX,HL MOVW AX,HL SUBW AX,HL XCHW AX,HL
 08 XCH A,X MOV [B+nnnn],A MOV [C+nnnn],A MOV [C+nnnn],nn
 09 MOV A,[B+nnnn] MOV [B+nnnn],nn MOV A,[C+nnnn] MOV [BC+nnnn],nn
 0A ADD saddr,nn ADDC saddr,nn SUB saddr,nn SUBC saddr,nn
 0B ADD A,saddr ADDC A,saddr SUB A,saddr SUBC A,saddr
 0C ADD A,nn ADDC A,nn SUB A,nn SUBC A,nn
 0D ADD A,[HL] ADDC A,[HL] SUB A,[HL] SUBC A,[HL]
 0E ADD A,[HL+byte] ADDC A,[HL+byte] SUB A,[HL+byte] SUBC A,[HL+byte]
 0F ADD A,!addr16 ADDC A,!addr16 SUB A,!addr16 SUBC A,!addr16 |
| --- |

```

| 40 50 60 70
 40 CMP !addr16,nn MOV X,nn MOV A,X MOV X,A
 41 MOV ES,nn MOV A,nn 2nd MAP !!! 3rd MAP !!!
 42 CMPW AX,!addr16 MOV C,nn MOV A,C MOV C,A
 43 CMPW AX,BC MOV B,nn MOV A,B MOV B,A
 44 CMPW AX,nnnn MOV E,nn MOV A,E MOV E,A
 45 CMPW AX,DE MOV D,nn MOV A,D MOV D,A
 46 CMPW AX,saddrp MOV L,nn MOV A,L MOV L,A
 47 CMPW AX,HL MOV H,nn MOV A,H MOV H,A
 48 MOV [BC+nnnn],A MOVW [B+nnnn],AX MOVW [C+nnnn],AX MOVW [BC+nnnn],AX
 49 MOV A,[BC+nnnn] MOVW AX,[B+nnnn] MOVW AX,[C+nnnn] MOVW AX,[BC+nnnn]
 4A CMP saddr,nn AND saddr,nn OR saddr,nn XOR saddr,nn
 4B CMP A,saddr AND A,saddr OR A,saddr XOR A,saddr
 4C CMP A,nn AND A,nn OR A,nn XOR A,nn
 4D CMP A,[HL] AND A,[HL] OR A,[HL] XOR A,[HL]
 4E CMP A,[HL+byte] AND A,[HL+byte] OR A,[HL+byte] XOR A,[HL+byte]
 4F CMP A,!addr16 AND A,!addr16 OR A,!addr16 XOR A,!addr16 |
| --- |

```

| 80 90 A0 B0
 80 INC X DEC X INC !addr16 DEC !addr16
 81 INC A DEC A INCW AX DECW AX
 82 INC C DEC C INCW !addr16 DECW !addr16
 83 INC B DEC B INCW BC DECW BC
 84 INC E DEC E INC saddr DEC saddr
 85 INC D DEC D INCW DE DECW DE
 86 INC L DEC L INCW saddrp DECW saddrp
 87 INC H DEC H INCW HL DECW HL
 88 MOV A,[SP+byte] MOV [SP+byte],A MOVW AX,[SP+byte] MOVW [SP+byte],AX
 89 MOV A,[DE] MOV [DE],A MOVW AX,[DE] MOVW [DE],AX
 8A MOV A,[DE+byte] MOV [DE+byte],A MOVW AX,[DE+byte] MOVW [DE+byte],AX
 8B MOV A,[HL] MOV [HL],A MOVW AX,[HL] MOVW [HL],AX
 8C MOV A,[HL+byte] MOV [HL+byte],A MOVW AX,[HL+byte] MOVW [HL+byte],AX
 8D MOV A,saddr MOV saddr,A MOVW AX,saddrp MOVW saddrp,AX
 8E MOV A,sfr MOV sfr,A MOVW AX,sfrp MOVW sfrp,AX
 8F MOV A,!addr16 MOV !addr16,A MOVW AX,!addr16 MOVW !addr16,AX |
| --- |

```

| C0 D0 E0 F0
 C0 POP AX CMP0 X ONEB X CLRB X
 C1 PUSH AX CMP0 A ONEB A CLRB A
 C2 POP BC CMP0 C ONEB C CLRB C
 C3 PUSH BC CMP0 B ONEB B CLRB B
 C4 POP DE CMP0 saddr ONEB saddr CLRB saddr
 C5 PUSH DE CMP0 !addr16 ONEB !addr16 CLRB !addr16
 C6 POP HL MULU X ;(AX=A*X) ONEW AX CLRW AX
 C7 PUSH HL RET ONEW BC CLRW BC
 C8 MOV [SP+byte],nn MOV X,saddr MOV B,saddr MOV C,saddr
 C9 MOVW saddrp,nnnn MOV X,!addr16 MOV B,!addr16 MOV C,!addr16
 CA MOV [DE+byte],nn MOVW BC,saddrp MOVW DE,saddrp MOVW HL,saddrp
 CB MOVW sfrp,nnnn MOVW BC,!addr16 MOVW DE,!addr16 MOVW HL,!addr16
 CC MOV [HL+byte],nn BC $addr20 BR !!addr20 CALL !!addr20
 CD MOV saddr,nn BZ $addr20 BR !addr16 CALL !addr16
 CE MOV sfr,nn BNC $addr20 BR $!addr20 CALL $!addr20
 CF MOV !addr16,nn BNZ $addr20 BR $addr20 - (TRAP) |
| --- |

Opcode FFh = TRAP (illegal opcode, used by 3DS MCU, triggers Reset vector).

Uh, other/prefixed illegal opcodes do not act as TRAP?

**Instruction Map (2nd MAP) (with prefix byte 61h)**

```

| 00 10 20 30 40 50 60 70 Notes
 00 ADD ADDC SUB SUBC CMP AND OR XOR with operands X,A
 01 ADD ADDC SUB SUBC CMP AND OR XOR with operands A,A
 02 ADD ADDC SUB SUBC CMP AND OR XOR with operands C,A
 03 ADD ADDC SUB SUBC CMP AND OR XOR with operands B,A
 04 ADD ADDC SUB SUBC CMP AND OR XOR with operands E,A
 05 ADD ADDC SUB SUBC CMP AND OR XOR with operands D,A
 06 ADD ADDC SUB SUBC CMP AND OR XOR with operands L,A
 07 ADD ADDC SUB SUBC CMP AND OR XOR with operands H,A
 08 ADD ADDC SUB SUBC CMP AND OR XOR with operands A,X
 09 ADDW - SUBW - CMPW INC DEC INCW <see below>
 0A ADD ADDC SUB SUBC CMP AND OR XOR with operands A,C
 0B ADD ADDC SUB SUBC CMP AND OR XOR with operands A,B
 0C ADD ADDC SUB SUBC CMP AND OR XOR with operands A,E
 0D ADD ADDC SUB SUBC CMP AND OR XOR with operands A,D
 0E ADD ADDC SUB SUBC CMP AND OR XOR with operands A,L
 0F ADD ADDC SUB SUBC CMP AND OR XOR with operands A,H
 80 ADD ADDC SUB SUBC CMP AND OR XOR with operands A,[HL+B]
 81 - - - - - - - - -
 82 ADD ADDC SUB SUBC CMP AND OR XOR with operands A,[HL+C]
 83 - - - - BH BNH SKH SKNH <see below>
 84 [80h] [82h] [84h] [86h] [88h] [8Ah] [8Ch] [8Eh] CALLT [0008xh]
 85 [90h] [92h] [94h] [96h] [98h] [9Ah] [9Ch] [9Eh] CALLT [0009xh]
 86 [A0h] [A2h] [A4h] [A6h] [A8h] [AAh] [ACh] [AEh] CALLT [000Axh]
 87 [B0h] [B2h] [B4h] [B6h] [B8h] [BAh] [BCh] [BEh] CALLT [000Bxh]
 88 - - XCH MOV SKC SKNC SKZ SKNZ <see below>
 89 DECW - XCH XCH MOV MOV MOV MOV <see below>
 8A XCH - XCH - CALL CALL CALL CALL <see below>
 8B XCH - XCH - BR ROR ROL RORC <see below>
 8C XCH - XCH - BRK ROLC RETB RETI <see below>
 8D XCH - XCH - POP PUSH HALT STOP <see below>
 8E XCH - XCH - MOVS CMPS ROLWC ROLWC <see below>
 8F XCH - XCH - SEL SEL SEL SEL <see below> |
| --- |

Operands for above opcodes:

```

| Operands for ADDW/SUBW/CMPW AX,[HL+byte]
 Operands for INCW/DECW/INC/DEC [HL+byte]
 Operands for BR AX
 Operands for ROL/ROR/ROLC/RORC A,1
 Operands for PUSH/POP PSW
 Operands for MOVS [HL+byte],X
 Operands for CMPS X,[HL+byte]
 Operands for SKC/SKNC/SKZ/SKNZ/SKH/SKNH None
 Operands for BRK/RETB/RETI/HALT/STOP None
 Operands for BH/BNH $addr20
 Operands for ROLWC (opcode EE/FE) AX,1 / BC,1
 Operands for CALL (opcode CA/DA/EA/FA) AX/BC/DE/HL
 Operands for SEL (opcode CF/DF/EF/FF) RB0/RB1/RB2/RB3
 Operands for MOV (opcode C9/D9) A,[HL+B] / [HL+B],A
 Operands for MOV (opcode E9/F9/B8) A,[HL+C] / [HL+C],A / ES,saddr
 Operands for XCH (opcode 8A..8F) A,C / A,B / A,E / A,D / A,L / A,H
 Operands for XCH (opcode A8,A9,AA) A,saddr / A,[HL+C] / A,!addr16
 Operands for XCH (opcode AB,AC,AD) A,sfr / A,[HL] / A,[HL+byte]
 Operands for XCH (opcode AE,AF,B9) A,[DE] / A,[DE+byte] / A,[HL+B] |
| --- |

Note Not mounted on the RL78-S1 core.

**Instruction Map (3rd MAP) (with prefix byte 71h)**

```

| 00+#*10 SET1 !addr16.# 80+#*10 --> see below (80,C0)
 01+#*10 MOV1 saddr.#,CY 81+#*10 MOV1 [HL].#,CY
 02+#*10 SET1 saddr.# 82+#*10 SET1 [HL].#
 03+#*10 CLR1 saddr.# 83+#*10 CLR1 [HL].#
 04+#*10 MOV1 CY,saddr.# 84+#*10 MOV1 CY,[HL].#
 05+#*10 AND1 CY,saddr.# 85+#*10 AND1 CY,[HL].#
 06+#*10 OR1 CY,saddr.# 86+#*10 OR1 CY,[HL].#
 07+#*10 XOR1 CY,saddr.# 87+#*10 XOR1 CY,[HL].#
 08+#*10 CLR1 !addr16.# 88+#*10 --> see below (88)
 09+#*10 MOV1 sfr.#,CY 89+#*10 MOV1 A.#,CY
 0A+#*10 SET1 sfr.# 8A+#*10 SET1 A.#
 0B+#*10 CLR1 sfr.# 8B+#*10 CLR1 A.#
 0C+#*10 MOV1 CY,sfr.# 8C+#*10 MOV1 CY,A.#
 0D+#*10 AND1 CY,sfr.# 8D+#*10 AND1 CY,A.#
 0E+#*10 OR1 CY,sfr.# 8E+#*10 OR1 CY,A.#
 0F+#*10 XOR1 CY,sfr.# 8F+#*10 XOR1 CY,A.#
 80 SET1 CY 90,A0,B0 reserved
 88 CLR1 CY D0,E0,F0 reserved
 C0 NOT1 CY 98,A8,B8,C8,D8,E8,F8 reserved |
| --- |

**Instruction Map (4th MAP) (with prefix byte 31h)**

```

| 00+#*10 BTCLR saddr.#,$addr20 ;\
 01+#*10 BTCLR A.#, $addr20 ;
 80+#*10 BTCLR sfr.#, $addr20 ;
 81+#*10 BTCLR [HL].#, $addr20 ;
 02+#*10 BT saddr.#,$addr20 ; #=0..7
 03+#*10 BT A.#, $addr20 ;
 82+#*10 BT sfr.#, $addr20 ;
 83+#*10 BT [HL].#, $addr20 ;
 04+#*10 BF saddr.#,$addr20 ;
 05+#*10 BF A.#, $addr20 ;
 84+#*10 BF sfr.#, $addr20 ;
 85+#*10 BF [HL].#, $addr20 ;/
 06+#*10 - ;-#=N/A (0..15=reserved)
 07+#*10 SHL C,# ;\
 08+#*10 SHL B,# ;
 09+#*10 SHL A,# ; #=1..7 (0,8..15=reserved)
 0A+#*10 SHR A,# ;
 0B+#*10 SAR A,# ;/
 0C+#*10 SHLW BC,# ;\
 0D+#*10 SHLW AX,# ; #=1..15 (0=reserved)
 0E+#*10 SHRW AX,# ;
 0F+#*10 SARW AX,# ;/ |
| --- |

**Pseudo Opcodes**

```

| EI 71 7A FA ;SET1 SFR(FAh).7 (PSW.bit7 enable interrupts)
 DI 71 7B FA ;CLR1 SFR(FAh).7 (PSW.bit7 disable interrupts)
 TRAP FF ;invalid opcode FF (triggers Reset vector) |
| --- |

Extra MUL opcodes (unknown if these are supported on 3DS):

```

| MULHU CE FB 01 ;MOV SFR(FBh),#01h Note
 MULH CE FB 02 ;MOV SFR(FBh),#02h Note
 DIVHU CE FB 03 ;MOV SFR(FBh),#03h Note
 DIVWU CE FB 0B ;MOV SFR(FBh),#0Bh Note
 MACHU CE FB 05 ;MOV SFR(FBh),#05h Note
 MACH CE FB 06 ;MOV SFR(FBh),#06h Note |
| --- |

Others:

```

| "saddr.#" and "sfr.#" with #=8..15 could be implemented as "addr+1.#-8".
 "callt [nn]" can be seen as 2-byte alias for "call nnnn" (via the ROM table) |
| --- |