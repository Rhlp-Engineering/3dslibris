# I2Cmcurl78Cpuopcodelist

> Source: https://problemkaputt.de/gbatek.htm
> Section: I2Cmcurl78Cpuopcodelist

3DS I2C MCU - RL78 CPU Opcode List 
| |
| --- |

 **Move Byte**
 
```

| 09/29/49 MOV A, [nnnn+B]/[nnnn+C]/[nnnn+BC]
 88/89/8A/8B/8C MOV A, [nn+SP]/[DE]/[nn+DE]/[HL]/[nn+HL]
 8D/8E/8F MOV A, [saddr]/[sfr]/[nnnn]
 61C9/61E9 MOV A, [HL+B]/[HL+C]
 60/--/62/63/64/65/66/67 MOV A, X/-/C/B/E/D/L/H
 70/--/72/73/74/75/76/77 MOV X/-/C/B/E/D/L/H, A
 50/51/52/53/54/55/56/57 MOV X/A/C/B/E/D/L/H, nn
 D8/--/F8/E8 MOV X/-/C/B, [saddr]
 D9/--/F9/E9 MOV X/-/C/B, [nnnn]
 E0/E1/E2/E3/E4/E5 MOV X/A/C/B/[saddr]/[nnnn], 1 ;native: ONEB
 F0/F1/F2/F3/F4/F5 MOV X/A/C/B/[saddr]/[nnnn], 0 ;native: CLRB
 41/61B8 MOV ES, nn/[saddr]
 18/28/48 MOV [nnnn+B]/[nnnn+C]/[nnnn+BC], A
 98/99/9A/9B/9C MOV [nn+SP]/[DE]/[nn+DE]/[HL]/[nn+HL], A
 9D/9E/9F MOV [saddr]/[sfr]/[nnnn], A
 61D9/61F9 MOV [HL+B]/[HL+C], A
 19/38/39 MOV [nnnn+B]/[nnnn+C]/[nnnn+BC], nn
 C8/CA/CC MOV [nn+SP]/[nn+DE]/[nn+HL], nn
 CD/CE/CF MOV [saddr]/[sfr]/[nnnn], nn
 08/--/618A/618B/618C/618D/618E/618F XCH A, X/-/C/B/E/D/L/H
 61AC/61AD/61B9/61A9 XCH A, [HL]/[HL+byte]/[HL+B]/[HL+C]
 61AE/61AF XCH A, [DE]/[DE+byte]
 61AB/61A8/61AA XCH A, [sfr]/[saddr]/[nnnn] |
| --- |

**Move Word**

```

| 13/15/17/30 MOVW AX, BC/DE/HL/nnnn
 59/69/79 MOVW AX, [nnnn+B]/[nnnn+C]/[nnnn+BC]
 A8/A9/AA/AB/AC MOVW AX, [nn+SP]/[DE]/[nn+DE]/[HL]/[nn+HL]
 AD/AE/AF MOVW AX, [saddrp]/[sfrp]/[nnnn]
 E6/E7 MOVW AX/BC,1 ;native: ONEW AX/BC
 F6/F7 MOVW AX/BC,0 ;native: CLRW AX/BC
 12/32/DA/DB MOVW BC, AX/nnnn/[saddrp]/[nnnn]
 14/34/EA/EB MOVW DE, AX/nnnn/[saddrp]/[nnnn]
 16/36/FA/FB MOVW HL, AX/nnnn/[saddrp]/[nnnn]
 58/68/78 MOVW [nnnn+B]/[nnnn+C]/[nnnn+BC], AX
 B8/B9/BA/BB/BC MOVW [nn+SP]/[DE]/[nn+DE]/[HL]/[nn+HL], AX
 BD/BE/BF MOVW [saddrp]/[sfrp]/[nnnn], AX
 C9/CB MOVW [saddrp]/[sfrp], nnnn
 C0/C2/C4/C6/61CD POP AX/BC/DE/HL/PSW
 C1/C3/C5/C7/61DD PUSH AX/BC/DE/HL/PSW
 33/35/37 XCHW AX, BC/DE/HL |
| --- |

**ALU Byte**

```

| 6100/6101/6102/6103/6104/6105/6106/6107 ADD X/A/C/B/E/D/L/H, A
 6110/6111/6112/6113/6114/6115/6116/6117 ADDC X/A/C/B/E/D/L/H, A
 6120/6121/6122/6123/6124/6125/6126/6127 SUB X/A/C/B/E/D/L/H, A
 6130/6131/6132/6133/6134/6135/6136/6137 SUBC X/A/C/B/E/D/L/H, A
 6140/6141/6142/6143/6144/6145/6146/6147 CMP X/A/C/B/E/D/L/H, A
 6150/6151/6152/6153/6154/6155/6156/6157 AND X/A/C/B/E/D/L/H, A
 6160/6161/6162/6163/6164/6165/6166/6167 OR X/A/C/B/E/D/L/H, A
 6170/6171/6172/6173/6174/6175/6176/6177 XOR X/A/C/B/E/D/L/H, A
 6108/----/610A/610B/610C/610D/610E/610F ADD A, X/-/C/B/E/D/L/H
 6118/----/611A/611B/611C/611D/611E/611F ADDC A, X/-/C/B/E/D/L/H
 6128/----/612A/612B/612C/612D/612E/612F SUB A, X/-/C/B/E/D/L/H
 6138/----/613A/613B/613C/613D/613E/613F SUBC A, X/-/C/B/E/D/L/H
 6148/----/614A/614B/614C/614D/614E/614F CMP A, X/-/C/B/E/D/L/H
 6158/----/615A/615B/615C/615D/615E/615F AND A, X/-/C/B/E/D/L/H
 6168/----/616A/616B/616C/616D/616E/616F OR A, X/-/C/B/E/D/L/H
 6178/----/617A/617B/617C/617D/617E/617F XOR A, X/-/C/B/E/D/L/H
 0B/0C/0D/0E/0F ADD A,[saddr]/nn/[HL]/[HL+byte]/[nnnn]
 1B/1C/1D/1E/1F ADDC A,[saddr]/nn/[HL]/[HL+byte]/[nnnn]
 2B/2C/2D/2E/2F SUB A,[saddr]/nn/[HL]/[HL+byte]/[nnnn]
 3B/3C/3D/3E/3F SUBC A,[saddr]/nn/[HL]/[HL+byte]/[nnnn]
 4B/4C/4D/4E/4F CMP A,[saddr]/nn/[HL]/[HL+byte]/[nnnn]
 5B/5C/5D/5E/5F AND A,[saddr]/nn/[HL]/[HL+byte]/[nnnn]
 6B/6C/6D/6E/6F OR A,[saddr]/nn/[HL]/[HL+byte]/[nnnn]
 7B/7C/7D/7E/7F XOR A,[saddr]/nn/[HL]/[HL+byte]/[nnnn]
 0A ADD [saddr],nn
 1A ADDC [saddr],nn
 2A SUB [saddr],nn
 3A SUBC [saddr],nn
 4A/40 CMP [saddr]/[nnnn],nn
 5A AND [saddr],nn
 6A OR [saddr],nn
 7A XOR [saddr],nn
 D0/D1/D2/D3/D4/D5 CMP X/A/C/B/[saddr]/[nnnn],0 ;native: CMP0
 80/81/82/83/84/85/86/87/A0/A4/6159 INC X/A/C/B/E/D/L/H/[nnnn]/[saddr]/[HL+nn]
 90/91/92/93/94/95/96/97/B0/B4/6169 DEC X/A/C/B/E/D/L/H/[nnnn]/[saddr]/[HL+nn] |
| --- |

**ALU Word**

```

| 01/03/05/07 ADDW AX, AX/BC/DE/HL
 --/23/25/27 SUBW AX, --/BC/DE/HL
 --/43/45/47 CMPW AX, --/BC/DE/HL
 04/02/06/6109/10 ADDW AX, nnnn/[nnnn]/[saddrp]/[HL+nn] / ADDW SP,00nn
 24/22/26/6129/20 SUBW AX, nnnn/[nnnn]/[saddrp]/[HL+nn] / SUBW SP,00nn
 44/42/46/6149/-- CMPW AX, nnnn/[nnnn]/[saddrp]/[HL+nn] / CMPW --
 A1/A3/A5/A7/A2/A6/6179 INCW AX/BC/DE/HL/[nnnn]/[saddrp]/[HL+nn]
 B1/B3/B5/B7/B2/B6/6189 DECW AX/BC/DE/HL/[nnnn]/[saddrp]/[HL+nn] |
| --- |

**Rotate/Shift**

```

| 61DB ROR A, 1
 61EB ROL A, 1
 61FB RCR A, 1 ;native: RORC A,1
 61DC RCL A, 1 ;native: ROLC A,1
 3109/3108/3107+n*10 SHL A/B/C, 1..7
 310A+n*10 SHR A, 1..7
 310B+n*10 SAR A, 1..7
 61EE/61FE RCLW AX/BC, 1 ;native: ROLWC AX/BC,1
 310D/310C+n*10 SHLW AX/BC, 1..15
 310E+n*10 SHRW AX, 1..15
 310F+n*10 SARW AX, 1..15 |
| --- |

**Jump/Call**

```

| 61CB JMP AX ;native: BR AX ;CS:
 ECnnnn0n JMP absolute far addr ;native: BR !!addr20
 EDnnnn JMP absolute addr ;native: BR !addr16
 EEnnnn JMP relative addr ;native: BR $!addr20
 EFnn JMP relative short addr ;native: BR $addr20
 61CA/61DA/61EA/61FA CALL AX/BC/DE/HL ;native: CALL AX/BC/DE/HL ;CS:
 FCnnnn0n CALL absolute far addr ;native: CALL !!addr20
 FDnnnn CALL absolute addr ;native: CALL !addr16
 FEnnnn CALL relative addr ;native: CALL $!addr20
 61nn CALL [000xx] ;80h..BEh ;native: CALLT [xx] |
| --- |

**Conditional Relative Jumps**

```

| DCnn JC aka JB addr ;carry/below ;native: BC
 DDnn JZ aka JE addr ;zero/equal ;native: BZ
 61C3nn JH aka JA addr ;higher/above ;native: BH
 DEnn JNC aka JAE addr ;not carry/below ;native: BNC
 DFnn JNZ aka JNE addr ;not zero/equal ;native: BNZ
 61D3nn JNH aka JBE addr ;not higher/above ;native: BNH |
| --- |

**Test (and Clear) and Conditional Relative Jump**

```

| 3100/3180/3181/3101+n*10h BTCLR|JNZCLR [saddr]/[sfr]/[HL]/A.n, addr
 3102/3182/3183/3103+n*10h BT|JNZ [saddr]/[sfr]/[HL]/A.n, addr
 3104/3184/3185/3105+n*10h BF|JZ [saddr]/[sfr]/[HL]/A.n, addr |
| --- |

**Bit Manipulation**

```

| 7101/7109/7181/7189+n*10 MOV1 [saddr]/[sfr]/[HL]/A.n, CY
 7104/710C/7184/718C+n*10 MOV1 CY, [saddr]/[sfr]/[HL]/A.n
 7105/710D/7185/718D+n*10 AND1 CY, [saddr]/[sfr]/[HL]/A.n
 7106/710E/7186/718E+n*10 OR1 CY, [saddr]/[sfr]/[HL]/A.n
 7107/710F/7187/718F+n*10 XOR1 CY, [saddr]/[sfr]/[HL]/A.n
 7180/7100/7102/710A/7182/718A+n*10 SET1 CY/[nnnn]/[saddr]/[sfr]/[HL]/A.n
 7188/7108/7103/710B/7183/718B+n*10 CLR1 CY/[nnnn]/[saddr]/[sfr]/[HL]/A.n
 71C0 NOT1 CY |
| --- |

**Misc**

```

| 00 NOP
 61CC/61ED/61FD BRK/HALT/STOP
 D7/61EC/61FC RET/RETB/RETI
 FF TRAP ;pseudo invalid.reset
 717AFA/717BFA EI/DI ;pseudo set/clr.sfr.bit
 61C8/61D8/61E8/61F8/61E3/61F3 SKC/SKNC/SKZ/SKNZ/SKH/SKNH ;conditional skip
 61CE MOVS [HL+byte],X
 61DE CMPS X,[HL+byte]
 61CF/61DF/61EF/61FF SEL RB0/1/2/3
 D6 MULU AX,A,X ;unsigned AX=A*X ;native: MULU X
 11 prefix for [ES:addr] ;native: ES:addr[reg] |
| --- |