# Gpushaderinstructionsetblurp

> Source: https://problemkaputt.de/gbatek.htm
> Section: Gpushaderinstructionsetblurp

3DS GPU Shader Instruction Set - Blurp 
| |
| --- |

 **Overview**
 A compiled shader binary is comprised of two parts: the main instruction sequence and the operand descriptor table. These are both sent to the GPU around the same time but using separate GPU Commands. Instructions (such as format 1 instruction) may reference operand descriptors. When such is the case, the operand descriptor ID is the offset, in words, of the descriptor within the table. Both instructions and descriptors are coded in little endian. Basic implementations of the following specification can be found at [1] and [2]. The instruction set seems to have been heavily inspired by Microsoft's vs_3_0 [3] and the Direct3D shader code [4]. Please note that this page is being written as the instruction set is reverse engineered; as such it may very well contain mistakes.
 
 Debug information found in the code.bin of "Ironfall: Invasion" suggests that there may not be more than 512 instructions and 128 operand descriptors in a shader.
 
 **Nomenclature**
 
```

| - opcode names with I appended to them are the same as their non-I version,
 except they use the inverted instruction format, giving 7 bits to SRC2
 (and access to uniforms) and 5 bits to SRC1
 - opcode names with U appended to them are the same as their non-U version,
 except they are executed conditionally based on the value of a uniform
 boolean.
 - opcode names with C appended to them are the same as their non-C version,
 except they are executed conditionally based on a logical expression
 specified in the instruction. |
| --- |

**Instruction formats**

```

| Offset Size (bits) Description
 Format 1: (used for register operations)
 0x0 0-6 0x7 Operand descriptor ID (DESC)
 0x7 7-11 0x5 Source 2 register (SRC2)
 0xC 12-18 0x7 Source 1 register (SRC1)
 0x13 19-20 0x2 Address register index for SRC1 (IDX_1)
 0x15 21-25 0x5 Destination register (DST)
 0x1A 26-31 0x6 Opcode
 Format 1i: (used for register operations)
 0x0 0-6 0x7 Operand descriptor ID (DESC)
 0x7 7-13 0x7 Source 2 register (SRC2)
 0xE 14-18 0x5 Source 1 register (SRC1)
 0x13 19-20 0x2 Address register index for SRC2 (IDX_2)
 0x15 21-25 0x5 Destination register (DST)
 0x1A 26-31 0x6 Opcode
 Format 1u: (used for unary register operations)
 0x0 0-6 0x7 Operand descriptor ID (DESC)
 7 7-11 5 ?
 0xC 12-18 0x7 Source 1 register (SRC1)
 0x13 19-20 0x2 Address register index for SRC1 (IDX_1)
 0x15 21-25 0x5 Destination register (DST)
 0x1A 26-31 0x6 Opcode
 Format 1c: (used for comparison operations)
 0x0 0-6 0x7 Operand descriptor ID (DESC)
 0x7 7-11 0x5 Source 2 register (SRC2)
 0xC 12-18 0x7 Source 1 register (SRC1)
 0x13 19-20 0x2 Address register index for SRC1 (IDX_1)
 0x15 21-23 0x3 Comparison operator for Y (CMPY)
 0x18 24-26 0x3 Comparison operator for X (CMPX)
 0x1B 27-31 0x5 Opcode (5bit only)
 Format 2: (used for flow control instructions)
 0x0 0-7 0x8 Number of instructions (NUM)
 8 8-9 ? ?
 0xA 10-21 0xC Destination offset (in words) (DST)
 0x16 22-23 0x2 Condition boolean operator (CONDOP)
 0x18 24 0x1 Y reference bit (REFY)
 0x19 25 0x1 X reference bit (REFX)
 0x1A 26-31 0x6 Opcode
 Format 3: (used for uniform-based conditional flow control instructions)
 0x0 0-7 0x8 Number of instructions ? (NUM)
 8 8-9 ? ?
 0xA 10-21 0xC Destination offset (in words) (DST)
 0x16 22-25 0x4 Uniform ID (BOOL/INT)
 0x1A 26-31 0x6 Opcode
 Format 4: (used for SETEMIT)
 0 0-21 ? ?
 0x16 22 0x1 Winding flag (FLAG_WINDING)
 0x17 23 0x1 Primitive emit flag (FLAG_PRIMEMIT)
 0x18 24-25 0x2 Vertex ID (VTXID)
 0x1A 26-31 0x6 Opcode
 Format 5: (used for MAD)
 0x0 0-4 0x5 Operand descriptor ID (DESC)
 0x5 5-9 0x5 Source 3 register (SRC3)
 0xA 10-16 0x7 Source 2 register (SRC2)
 0x11 17-21 0x5 Source 1 register (SRC1)
 0x16 22-23 0x2 Address register index for SRC2 (IDX_2)
 0x18 24-28 0x5 Destination register (DST)
 0x1D 29-31 0x3 Opcode (3bit only)
 Format 5i: (used for MADI)
 0x0 0-4 0x5 Operand descriptor ID (DESC)
 0x5 5-11 0x7 Source 3 register (SRC3)
 0xC 12-16 0x5 Source 2 register (SRC2)
 0x11 17-21 0x5 Source 1 register (SRC1)
 0x16 22-23 0x2 Address register index for SRC3 (IDX_3)
 0x18 24-28 0x5 Destination register (DST)
 0x1D 29-31 0x3 Opcode (3bit only) |
| --- |

**Instructions**

Unless noted otherwise, SRC1 and SRC2 refer to their respectively indexed
float[4] registers (after swizzling). Similarly, DST refers to its indexed
register modulo destination component masking, i.e. an expression like DST=SRC1
might actually just set DST.y to SRC1.y.

```

| Opcode Format Name Description
 00h 1 ADD Adds two vectors component by component
 DST[i] = SRC1[i]+SRC2[i] for all i
 01h 1 DP3 Computes dot product on 3-component vectors
 DST = SRC1.SRC2
 02h 1 DP4 Computes dot product on 4-component vectors
 DST = SRC1.SRC2
 03h 1 DPH Computes dot product on a 3-component vector
 with 1.0 appended to it and a 4-component vector
 DST = SRC1.SRC2 (with SRC1 homogenous)
 04h 1 DST Equivalent to Microsoft's dst instruction
 DST = {1, SRC1[1]*SRC2[1], SRC1[2], SRC2[3]}
 05h 1u EX2 Computes SRC1's first component exponent with base 2
 DST[i] = EXP2(SRC1[0]) for all i
 06h 1u LG2 Computes SRC1's first component logarithm with base 2
 DST[i] = LOG2(SRC1[0]) for all i
 07h 1u LITP Appears to be related to Microsoft's lit instruction
 DST = clamp(SRC1, min={0, -127.9961, 0, 0},
 max={inf, 127.9961, 0, inf})
 ;note: 127.9961 = 0x7FFF / 0x100
 08h 1 MUL Multiplies two vectors component by component
 DST[i] = SRC1[i].SRC2[i] for all i
 09h 1 SGE Sets output if SRC1 is greater than or equal to SRC2
 DST[i] = (SRC1[i] >= SRC2[i]) ? 1.0 : 0.0
 "for all i" ;uh, who/what is "all i"?
 0Ah 1 SLT Sets output if SRC1 is strictly less than SRC2
 DST[i] = (SRC1[i] < SRC2[i]) ? 1.0 : 0.0
 "for all i"
 0Bh 1u FLR Computes SRC1's floor component by component
 DST[i] = FLOOR(SRC1[i])
 "for all i"
 0Ch 1 MAX Takes the max of two vectors, component by component
 DST[i] = MAX(SRC1[i], SRC2[i])
 "for all i"
 0Dh 1 MIN Takes the min of two vectors, component by component
 DST[i] = MIN(SRC1[i], SRC2[i])
 "for all i"
 0Eh 1u RCP Computes the reciprocal of the vector's 1st component
 DST[i] = 1/SRC1[0]
 "for all i"
 0Fh 1u RSQ Computes the reciprocal of the square root of the
 vector's first component;
 DST[i] = 1/sqrt(SRC1[0])
 "for all i"
 10h-11h ? ??? ?
 12h 1u MOVA Move to address register; Casts the float uniform
 given by SRC1 to an integer (truncating the fractional
 part) and assigns the result to (a0.x, a0.y, _, _),
 respecting the destination component mask.
 13h 1u MOV Moves value from one register to another
 DST = SRC1
 14h-17h ? ??? ?
 18h 1i DPHI Computes dot product on a 3-component vector
 with 1.0 appended to it and a 4-component vector;
 DST = SRC1.SRC2 (with SRC1 homogenous)
 19h 1i DSTI DST with sources swapped
 1Ah 1i SGEI Sets output if SRC1 is greater than or equal to SRC2
 DST[i] = (SRC1[i] >= SRC2[i]) ? 1.0 : 0.0
 "for all i"
 1Bh 1i SLTI Sets output if SRC1 is strictly less than SRC2
 DST[i] = (SRC1[i] < SRC2[i]) ? 1.0 : 0.0
 "for all i"
 1Ch-1Fh ? ??? ?
 20h 0 BREAK Breaks out of LOOP block; do not use while in
 nested IF/CALL block inside LOOP block.
 21h 0 NOP Does literally nothing.
 22h 0 END Signals the shader unit that processing for this
 vertex/primitive is done.
 23h 2 BREAKC If condition (see below for details) is true, then
 breaks out of LOOP block.
 24h 2 CALL Jumps to DST and executes instructions until it
 reaches DST+NUM instructions
 25h 2 CALLC If condition (see below for details) is true, then
 jumps to DST and executes instructions until it
 reaches DST+NUM instructions, else does nothing.
 26h 3 CALLU Jumps to DST and executes instructions until it
 reaches DST+NUM instructions if BOOL is true
 27h 3 IFU If condition BOOL is true, then executes instructions
 until DST, then jumps to DST+NUM; else, jumps to DST.
 28h 2 IFC If condition (see below for details) is true, then
 executes instructions until DST, then jumps
 to DST+NUM; else, jumps to DST
 29h 3 LOOP Loops over the code between itself and DST
 (inclusive), performing INT.x+1 iterations in total.
 First, aL is initialized to INT.y. After each
 iteration, aL is incremented by INT.z.
 2Ah 0 EMIT (geometry shader only) Emits a vertex (and primitive
 if FLAG_PRIMEMIT was set in the corresponding
 SETEMIT). SETEMIT must be called before this.
 2Bh 4 SETEMIT (geometry shader only) Sets VTXID, FLAG_WINDING and
 FLAG_PRIMEMIT for the next EMIT instruction. VTXID
 is the ID of the vertex about to be emitted within
 the primitive, while FLAG_PRIMEMIT is zero if we are
 just emitting a single vertex and non-zero if are
 emitting a vertex and primitive simultaneously.
 FLAG_WINDING controls the output primitive's winding.
 Note that the output vertex buffer (which holds 4
 vertices) is not cleared when the primitive is
 emitted, meaning that vertices from the previous
 primitive can be reused for the current one. (this
 is still a working hypothesis and unconfirmed)
 2Ch 2 JMPC If condition (see below for details) is true, then
 jumps to DST, else does nothing.
 2Dh 3 JMPU If condition BOOL is true, then jumps to DST, else
 does nothing. Having bit 0 of NUM = 1 will invert the
 test, jumping if BOOL is false instead.
 2Eh-2Fh 1c CMP Sets booleans cmp.x and cmp.y based on the operand's
 x and y components and the CMPX and CMPY comparison
 operators respectively. See below for details about
 operators. It's unknown whether CMP respects the
 destination component mask or not.
 30h-37h 5i MADI Multiplies two vectors and adds a third one
 component by component;
 DST[i] = SRC3[i] + SRC2[i].SRC1[i]
 "for all i"
 "this is not an FMA, the intermediate result is
 rounded"
 38h-3Fh 5 MAD Multiplies two vectors and adds a third one
 component by component;
 DST[i] = SRC3[i] + SRC2[i].SRC1[i]
 "for all i"
 "this is not an FMA, the intermediate result
 is rounded" |
| --- |

**Shader Operand descriptors (OPDESC's)**

Below 31bit (or 32bit?) are related to the 5bit/7bit "DESC" id's in opcode
LSBs.

```

| 0 Destination component mask W (0=Don't change, 1=Write)
 1 Destination component mask Z (0=Don't change, 1=Write)
 2 Destination component mask Y (0=Don't change, 1=Write)
 3 Destination component mask X (0=Don't change, 1=Write)
 4 Source 1 Negation bit
 5-6 Source 1 Component 3 value (0..3 = X,Y,Z,W) (usually 3)
 7-8 Source 1 Component 2 value (0..3 = X,Y,Z,W) (usually 2)
 9-10 Source 1 Component 1 value (0..3 = X,Y,Z,W) (usually 1)
 11-12 Source 1 Component 0 value (0..3 = X,Y,Z,W) (usually 0)
 13 Source 2 Negation bit
 14-15 Source 2 Component 3 value (0..3 = X,Y,Z,W)
 16-17 Source 2 Component 2 value (0..3 = X,Y,Z,W)
 18-19 Source 2 Component 1 value (0..3 = X,Y,Z,W)
 20-21 Source 2 Component 0 value (0..3 = X,Y,Z,W)
 22 Source 3 Negation bit
 23-24 Source 3 Component 3 value (0..3 = X,Y,Z,W)
 25-26 Source 3 Component 2 value (0..3 = X,Y,Z,W)
 27-28 Source 3 Component 1 value (0..3 = X,Y,Z,W)
 29-30 Source 3 Component 0 value (0..3 = X,Y,Z,W)
 31 unnown/unspecified/unused (usually 0) |
| --- |

The component selector (the 8bit fragments with 4x2bits) enables swizzling. For
example, component selector 0x1B is equivalent to .xyzw, while 0x55 is
equivalent to .yyyy.

Depending on the current shader opcode, source components are disabled
implicitly by setting (*to ZERO*) the destination component mask. For example,
ADD o0.xy, r0.xyzw, r1.xyzw will not make use of r0's or r1's z/w components,
while DP4 o0.xy, r0.xyzw, r1.xyzw will use all input components regardless of
the used destination component mask.

**Relative addressing**

```

| IDX Address offset
 00h +0 ;-no offset
 01h +a0.x ;\X or Y component of address register (see MOVA)
 02h +a0.y ;/
 03h +lp ;-loop counter (see LOOP) |
| --- |

There are 3 address registers: a0.x, a0.y and aL (loop counter). For format 1
instructions, when IDX != 0, the value of the corresponding address register is
added to SRC1's value. For example, if IDX = 2, a0.y = 3 and SRC1 = c8, then
instead SRC1+a0.y = c11 will be used for the instruction. It is only possible
to use address registers with vector uniform registers, attempting to use them
with input attribute or temporary registers results in the address register
being ignored (i.e. read as zero).

a0.x and a0.y are set manually through the MOVA instruction by rounding a float
value to integer precision. Hence, they may take negative values.

aL can only be set indirectly by the LOOP instruction. It is still accessible
and valid after exiting a LOOP block, though.

**Comparison operator**

```

| CMPX/CMPY raw value Operator name Expression
 00h EQ src1 == src2
 01h NE src1 != src2
 02h LT src1 < src2
 03h LE src1 <= src2
 04h GT src1 > src2
 05h GE src1 >= src2
 06h ?? seems to always return true
 07h ?? seems to always return true |
| --- |

**Conditions**

A number of format 2 instructions are executed conditionally. These conditions
are based on two boolean registers which can be set with CMP: cmp.x and cmp.y.

Conditional instructions include 3 parameters: CONDOP, REFX and REFY. REFX and
REFY are reference values which are tested for equality against cmp.x and
cmp.y, respectively. CONDOP describes how the final truth value is constructed
from the results of the two tests. There are four conditional expression
formats:

```

| CONDOP raw value Expression Description
 00h cmp.x == REFX || cmp.y == REFY OR
 01h cmp.x == REFX && cmp.y == REFY AND
 02h cmp.x == REFX X
 03h cmp.y == REFY Y |
| --- |

**Registers**

Input attribute registers (v0-v7?) store the per-vertex data given by the CPU
and hence are read-only.

Output attribute registers (o0-o6) hold the data to be passed to the later GPU
stages and are write-only. Each of the output attribute register components is
assigned a semantic by setting the corresponding GPU_Internal_Registers.

Uniform registers hold user-specified data which is constant throughout all
processed vertices. There are 96 float[4] uniform registers (c0-c95),
eight(???) boolean registers (b0-b7), and four int[4] registers (i0-i3).

Temporary registers (r0-r15) can be used for intermediate calculations and can
both be read and written.

Many shader instructions which take float arguments have only 5 bits available
for the second argument. They may hence only refer to input attributes or
temporary registers. In particular, it's not possible to pass two float[4]
uniforms to these instructions.

It appears that writing twice to the same output register can cause problems
(eg. GPU hangs).

DST mapping:

```

| DST raw value Register name Description
 00h-06h? o0-o6? Output registers (aka output FIFO) (W)
 10h-1Fh r0-r15 Temporary registers (R/W) |
| --- |

SRC1/SRC2/SRC3 mapping:

```

| SRC raw value Register name Description
 00h-07h? v0-v7? Input vertex/attribute (aka input FIFO) (R)
 10h-1Fh r0-r15 Temporary registers
 20h-7Fh c0-c95 Vector uniform registers (only for 7bit SRC) |
| --- |

**Control Flow**

Control flow is implemented using four independent stacks:

```

| 4-deep CALL stack
 8-deep IF stack
 4-deep LOOP stack |
| --- |

All stacks are initially empty. After every instruction but before JMP takes
effect, the PC is incremented and a copy is sent to each stack. Each stack is
checked against its copy of the PC. If an entry is popped from the stack, the
copied PC is updated and used for the next check of this stack, although the
IF/LOOP stacks can each only pop one entry per instruction, whereas the CALL
stack is checked again until it doesn't match or the stack is empty. The
updated PC copy with the highest priority wins: LOOP (highest), IF, CALL, JMP,
original PC (lowest).