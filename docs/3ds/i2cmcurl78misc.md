# I2Cmcurl78Misc

> Source: https://problemkaputt.de/gbatek.htm
> Section: I2Cmcurl78Misc

3DS I2C MCU - RL78 Misc 
| |
| --- |

 **Overlapping**
 
```

| ADDW AX,saddrp can also be used for first some SFR's FFF00..FFF1F
 CLRB saddr can also be used for first some SFR's FFF00..FFF1F
 addr16 can be used for further SFRs FFF20..FFFFF
 SFR_SP/PSW/ES can be used as alias for SP/PSW/ES
 SFR_AX/BC/DE/HL can be used as alias for AX/BC/DE/HL - but banked!
 SFR_CS ... can be manually changed BEFORE JMP/CALL AX to make FAR-JMP... ? |
| --- |

**Nocash Syntax**

```

| Native Nocash
 CLRB/CLRW/CMP0 op MOV/MOVW/CMP op,0
 ONEB/ONEW op MOV/MOVW op,1
 ADDC/SUBC/ROLC/RORC/ROLWC ADC/SBC/RCL/RCR/RCRW/RCLW
 BR/BF/BT/Bcond/BTCLR JMP/JZ/JNZ/Jcond/JNZCLR
 CALLT CALL
 MULU X MULU AX,A,X ;optional alias |
| --- |

```

| @@def Vbyte #byte ;nn ;imm8
 @@def Vword #word ;nnnn ;imm16
 @@def sfr sfr SFR name aka FFF00h..FFFFFh
 @@def sfrp sfrp 16-bit-manipulatable SFR name (even address)
 @@def saddr saddr 8bit data addr at FFE20h to FFF1Fh
 @@def saddrp saddrp same as saddr, but even-address only
 @@def $addr20 $addr20 probably 8bit reljump
 @@def $Xaddr20 $!addr20 probably 16bit reljump
 @@def XXaddr20_code !!addr20 20bit jumpdest (3 bytes) (CALL/BR) to CS=x
 @@def Xaddr16_code !addr16 16bit jumpdest (2 bytes) (CALL/BR) to CS=0!
 @@def Xaddr16 !addr16 16bit data addr at ES=F
 @@def IhlI [HL] ;[HL] ;\
 @@def IdeI [DE] ;[DE] ; maybe can also use ES:?
 @@def Ihl_bI [HL+B] ;[HL+B] ;
 @@def Ihl_cI [HL+C] ;[HL+C] ;/
 @@def wordIbcI word[BC] ;[BC+nnnn] ;\
 @@def wordIbI word[B] ;[B+nnnn] ;
 @@def wordIcI word[C] ;[C+nnnn] ; can use ES:
 @@def Ihl_byteI [HL+byte] ;[HL+nn] ;
 @@def Ide_byteI [DE+byte] ;[DE+nn] ;/
 @@def Isp_byteI [SP+byte] ;[SP+nn] ;\shouldn't/can't use ES:?
 @@def I00nnI [00nn] ;[000nn] ;/ |
| --- |

Caution: 16bit MOVW/ADDW/etc with memory operand must use even addresses (this
is documented only for [sfrp] and [saddrp], but does also apply for [nnnn],
which does actually read/write memory at [nnnn and FFFEh]).

Identifier Description

```

| ? [HL + byte], [DE + byte], [SP + byte] (only the space from F0000H
 to FFFFFH is specifiable)
 ? word[B], word[C] (only the space from F0000H to FFFFFH is specifiable)
 ? word[BC] (only the space from F0000H to FFFFFH is specifiable)
 ? ES:[HL + byte], ES:[DE + byte] (higher 4-bit addresses are specified by
 the ES register)
 ? ES:word[B], ES:word[C] (higher 4-bit addresses are specified by
 the ES register)
 ? ES:word[BC] (higher 4-bit addresses are specified by the ES register) |
| --- |

MCU-FIRM

```

| 00001F02 MCU v0, v1026, v2048, v3072, v4102, v5122, v6145, v7168, v8192
 20001F02 New_3DS MCU v8192, v9216(New2DSXL)
 00001F03 SAFE_MODE MCU v0
 20001F03 New_3DS SAFE_MODE MCU v9217 |
| --- |

**RL78/G13 - Memory Map (R5F100xA, R5F101xA (x = 6 to 8, A to C, E to G))**

```

| 00000h Boot Cluster 0
 01000h Boot Cluster 1 ;<-- hence the gap |
| --- |