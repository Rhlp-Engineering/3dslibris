# Gpushaderinstructionsetopcodesummary

> Source: https://problemkaputt.de/gbatek.htm
> Section: Gpushaderinstructionsetopcodesummary

3DS GPU Shader Instruction Set - Opcode Summary 
| |
| --- |

 **Shader Instruction Set Summary**
 
```

| Opcode Fmt Name Description
 00h 1 ADD Add two vectors component by component
 01h 1 DP3 Dot product on two 3-component vectors
 02h 1 DP4 Dot product on two 4-component vectors
 03h 1 DPH Dot product on a 3-component and a 4-component vector
 04h 1 DST Equivalent to Microsoft's "dst" instruction
 05h 1u EX2 Exponent with base 2 on 1st component of SRC1
 06h 1u LG2 Logarithm with base 2 on 1st component of SRC1
 07h 1u LITP Related to Microsoft's "lit" instruction
 08h 1 MUL Multiply two vectors component by component
 09h 1 SGE Set output if SRC1>=SRC2
 0Ah 1 SLT Set output if SRC1<SRC2
 0Bh 1u FLR Computes SRC1's floor component by component
 0Ch 1 MAX Max of two vectors, component by component
 0Dh 1 MIN Min of two vectors, component by component
 0Eh 1u RCP Reciprocal of vector's 1st component
 0Fh 1u RSQ Reciprocal of square root of vector's 1st component
 10h-11h ? ? ?
 12h 1u MOVA Move to address register (float to integer) (to "a0" ?)
 13h 1u MOV Move to register
 14h-17h ? ? ?
 18h 1i DPHI Dot product on a 3-component and a 4-component vector
 19h 1i DSTI DST with sources swapped
 1Ah 1i SGEI Set output if SRC1>=SRC2
 1Bh 1i SLTI Set output if SRC1<SRC2
 1Ch-1Fh ? ? ?
 20h 0 BREAK Break out of LOOP block
 21h 0 NOP No operation
 22h 0 END Done (stop execution and set ready flag?)
 23h 2 BREAKC Break (if condition is true)
 24h 2 CALL Call (jump to DST, and return after NUM instructions)
 25h 2 CALLC Call (as above, if condition is true)
 26h 3 CALLU Call (as above, if BOOL is true)
 27h 3 IFU Jump if/else BOOL is true
 28h 2 IFC Jump if/else condition is true
 29h 3 LOOP Loop start (repeat following opcodes)
 2Ah 0 EMIT Geometry shader only: Emit vertex (or vertex+primitive)
 2Bh 4 SETEMIT Geometry shader only: Configure type/flags for EMIT
 2Ch 2 JMPC Jump if condition is true
 2Dh 3 JMPU Jump if BOOL is true
 2Eh-2Fh 1c CMP Compare X and Y components and Set booleans cmp.x/y
 30h-37h 5i MADI Multiply and Add vectors, component by component
 38h-3Fh 5 MAD Multiply and Add vectors, component by component |
| --- |

**Shader Opcode/Parameter Encoding Formats**

```

| |..3 ..................2 ..................1 ..................0|
 |1_0_9_8_7_6_5_4_3_2_1_0_9_8_7_6_5_4_3_2_1_0_9_8_7_6_5_4_3_2_1_0| Fmt
 |_Opcode____|__0________________________________________________| 0 ;NoParam
 |_Opcode____|__Dst____|i1_|__Src1_______|__Src2___|______Desc___| 1 ;\
 |_Opcode____|__Dst____|i2_|__Src1___|______Src2___|______Desc___| 1i ; misc
 |_Opcode____|__Dst____|i1_|__Src1_______|__?______|______Desc___| 1u ;/
 |_Opcode__|CmpX_|CmpY_|i1_|__Src1_______|__Src2___|______Desc___| 1c CMP
 |_Opcode____|X|Y|Cnd|________DstOffs________|_?_|__NumInstr_____| 2 ;\JMP
 |_Opcode____|uID____|________DstOffs________|_?_|__NumInstr?____| 3 ;/etc.
 |_Opcode____|vID|P|W|__?________________________________________| 4 ;SETEMIT
 |_Opc_|__Dst____|i2_|__Src1___|__Src2_______|__Src3___|__Desc___| 5 ;MAD
 |_Opc_|__Dst____|i3_|__Src1___|__Src2___|______Src3___|__Desc___| 5i ;MADI |
| --- |

**Shader Operand descriptors (OPDESC's)**

```

| |..3 ..................2 ..................1 ..................0|
 |1_0_9_8_7_6_5_4_3_2_1_0_9_8_7_6_5_4_3_2_1_0_9_8_7_6_5_4_3_2_1_0|
 | |s0 |s1 |s2 |s3 |n|s0 |s1 |s2 |s3 |n|s0 |s1 |s2 |s3 |n|X_Y_Z_W|
 <---Source3-----> <---Source2-----> <---Source1-----> <-Dst-> |
| --- |