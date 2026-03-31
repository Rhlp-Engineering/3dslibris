# output is asserted. The bits in the secondary interrupt status/mask

> Source: https://problemkaputt.de/gbatek.htm
> Section: output is asserted. The bits in the secondary interrupt status/mask

DSi Atheros Wifi Internal Hardware 
| |
| --- |

 Below describes the internal Atheros hardware. The hardware registers can be accessed via WINDOW_DATA, or by uploading custom code to the Xtensa CPU via BMI Bootloader commands.
 Anyways, normally, the Wifi unit should be accessed via WMI commands, so one won't need to deal with internal hardware (except cases like reading the CHIP_ID, or for better understanding of the inner workings of the hardware).
 
 **Internal Xtensa CPU**
 DSi Atheros Wifi - Xtensa CPU Registers
 DSi Atheros Wifi - Xtensa CPU Core Opcodes
 DSi Atheros Wifi - Xtensa CPU Optional General Opcodes
 DSi Atheros Wifi - Xtensa CPU Optional Exception/Cache/MMU Opcodes
 DSi Atheros Wifi - Xtensa CPU Optional Floating-Point Opcodes
 DSi Atheros Wifi - Xtensa CPU Optional MAC16 Opcodes
 DSi Atheros Wifi - Xtensa CPU Opcode Encoding Tables
 
 **Internal Memory and I/O Maps**
 DSi Atheros Wifi - Internal Memory Map
 DSi Atheros Wifi - Internal I/O Map Summary (hw2)
 DSi Atheros Wifi - Internal I/O Map Summary (hw4)
 DSi Atheros Wifi - Internal I/O Map Summary (hw6)
 Note: All DSi/3DS boards are using hw2 (though with minor changes between AR6002 and AR6013/AR6014 on the GPIO I2C pin wiring, and external oscillator). The hw4/hw6 variants are shown here only for curiosity, they aren't used anywhere in DSi/3DS).
 
 **AR60XX Internal Registers**
 DSi Atheros Wifi - Internal I/O - Unknown and Unused Registers (hw2)
 DSi Atheros Wifi - Internal I/O - 004000h - RTC/Clock SOC (hw2/hw4/hw6)
 DSi Atheros Wifi - Internal I/O - 00x000h - RTC/Clock WLAN (hw2/hw4/hw6)
 DSi Atheros Wifi - Internal I/O - 0xx240h - RTC/Clock SYNC (hw6)
 DSi Atheros Wifi - Internal I/O - 006000h - WLAN Coex (MCI) (hw6)
 DSi Atheros Wifi - Internal I/O - 00x000h - Bluetooth Coex (hw4/hw6)
 DSi Atheros Wifi - Internal I/O - 00x000h - Memory Control (hw2/hw4/hw6)
 DSi Atheros Wifi - Internal I/O - 00C000h - Serial UART (hw2/hw4/hw6)
 DSi Atheros Wifi - Internal I/O - 00E000h - UMBOX Registers (hw4/hw6)
 DSi Atheros Wifi - Internal I/O - 010000h - Serial I2C/SPI (hw2/hw4/hw6)
 DSi Atheros Wifi - Internal I/O - 014000h - GPIO 18/26/57 pin (hw2/hw4/hw6)
 DSi Atheros Wifi - Internal I/O - 018000h - MBOX Registers (hw2/hw4/hw6)
 DSi Atheros Wifi - Internal I/O - 01C000h - Analog Intf (hw2)
 DSi Atheros Wifi - Internal I/O - 01C000h - Analog Intf (hw4/hw6)
 DSi Atheros Wifi - Internal I/O - 020000h - WMAC DMA (hw4/hw6)
 DSi Atheros Wifi - Internal I/O - 020080h - WMAC IRQ Interrupt (hw4/hw6)
 DSi Atheros Wifi - Internal I/O - 020800h - WMAC QCU Queue (hw4/hw6)
 DSi Atheros Wifi - Internal I/O - 021000h - WMAC DCU (hw4/hw6)
 DSi Atheros Wifi - Internal I/O - 028000h - WMAC PCU (hw2/hw4/hw6)
 DSi Atheros Wifi - Internal I/O - 029800h - BB Baseband (hw4/hw6)
 DSi Atheros Wifi - Internal I/O - 0xxx00h - RDMA Registers (hw4/hw6)
 DSi Atheros Wifi - Internal I/O - 03x000h - EFUSE Registers (hw4/hw6)
 DSi Atheros Wifi - Internal I/O - 034000h - More Stuff (hw6)
 
 
 
| DSi Atheros Wifi - Xtensa CPU Registers |
| --- |

 **Xtensa Core Registers**
 
```

| - AR Address registers A0..A15 (general purpose registers)
 - PC Program Counter |
| --- |

The AR registers are used as so:

```

| A0 general purpose (and return address for CALL/RET opcodes)
 A1 general purpose (commonly used as stack pointer)
 A2..A15 general purpose |
| --- |

Chips with Windowed Code option are actually having more than sixteen AR
registers, and the register window is rotated by CALL4/CALL8/CALL12/RETW
opcodes (allowing to save/restore registers without needing to push/pop them in
memory):

```

| CALL4 saves A0..A3 and moves A4..A15 to A0..A11 ;\and, probably copies
 CALL8 saves A0..A7 and moves A8..A15 to A0..A7 ; old A1 to new A1 (?),
 CALL12 saves A0..A11 and moves A12..A15 to A0..A3 ;/and new A0=ret_addr
 ENTRY used at begin of sub-functions (allocate local variables on stack)
 RETW windowed return (deallocate locals, and undo the CALL4/8/12 rotation) |
| --- |

The sub-functions will have retaddr/stack in A0/A1, and incoming parameters in
A2 and up, and return value in A2. However, due to the rotation, the caller
will see parameters/return value elsewhere (eg. for CALL8, parameters in A10
and up, and return value in A10).

The ENTRY opcode is used to allocate local variables on stack. For some weird
reason ENTRY is usually also allocating "N*4 bytes" dummy space for the "N"
saved words (as if they were intended to be pushed on stack, rather than being
saved via rotation, maybe some exception handler is actually USING that dummy
space when the register window gets full(?), the AR6002 BIOS contains a few
functions that aren't allocating enough dummy words though, eg. 8EDE44h uses
"entry a1,10h" although being called via CALL8).

**Xtensa Special Registers**

```

| 0 00h LBEG Loop Begin ;\
 1 01h LEND Loop End ; Loop option
 2 02h LCOUNT Loop Count ;/
 3 03h SAR Shift-Amount Register ;-Core
 4 04h BR Boolean Registers (16x1bit) ;-Boolean option
 5 05h LITBASE Literal Base ;-Literal base option
 12 0Ch SCOMPARE1 ;-Multiprocessor... vs S32C1I
 16 10h ACCLO Accumulator low (32bit) ;\
 17 11h ACCHI Accumulator high (8bit) ;
 32 20h MR0 MAC16 register m0 (32bit) ; MAC16 option
 33 21h MR1 MAC16 register m1 (32bit) ;
 34 22h MR2 MAC16 register m2 (32bit) ;
 35 23h MR3 MAC16 register m3 (32bit) ;/
 177 B1h EPC[1] Exception Program Counter ;\
 232 E8h EXCAUSE Cause of last Exception ;
 209 D1h EXCSAVE[1] ;
 230 E6h PS ; Exception option
 230 E6h PS.EXCM ;
 238 EEH EXCVADDR ;
 192 C0h DEPC ;/
 see PS.INTLEVEL ;-Interrupt option
 178..183 B2h.. EPC[2..7] ;\
 194..199 C2h.. EPS[2..7] ; High-Priority Interrupt option
 210..215 D2h.. EXCSAVE[2..7] ;/
 234 EAh CCOUNT ;\Timer Interrupt option
 240-242 F0h CCOMPARE ;/
 - AR[NAREG] ;\
 72 48h WindowBase ; Windowed Register option
 73 49h WindowStart ;
 230 E6h PS.CALLINC ;
 230 E6h PS.OWB ;
 230 E6h PS.WOE ;/
 244-247 F4h.. MISC ;-Misc Special Register option
 236 ECh ICOUNT ;\
 237 EDh ICOUNTLEVEL ;
 128-129 80h.. IBREAKA ;
 96 60h IBREAKENABLE ; Debug option
 144-145 90h.. DBREAKA ;
 160-161 A0h.. DBREAKC ;
 233 E9h DEBUGCAUSE ;
 104 68h DDR ;/
 230 E6h PS.RING ;\
 83 53h PTEVADDR ;
 90 5Ah RASID ; MMU option
 91 5Bh ITLBCFG ;
 92 5Ch DTLBCFG ;
 see ITLB ;
 see DTLB ;/
 98 62h CACHEATTR ;-
 99 63h ATOMCTL ;-
 224 E0h CPENABLE ;-
 226 E2h INTERRUPT (R);\
 226 E2h INTSET (W); Interrupt
 227 E3h INTCLEAR ;
 228 E4h INTENABLE ;/
 106 6Ah MEPC ;\
 107 6Bh MEPS ;
 108 6Ch MESAVE ; Memory ECC/Parity
 109 6Dh MESR ;
 110 6Eh MECR ;
 111 6Fh MEVADDR ;/
 89 59h MMID ;-Trace Port
 231 E7h VECBASE ;-
 235 EBh PRID ;-Processor ID |
| --- |

XSR with >=64 is privileged.

**Xtensa User Registers**

```

| 0-223 0-DFh Available for designer extensions
 192-255 C0h.. Reserved by Tensilica (conflicts with above "available" info?)
 231 E7h THREADPTR ;-Thread Pointer
 232 E8h FCR (float control) ;\
 233 E9h FSR (float status) ; Float
 - FR (f0..f15?) ;/ |
| --- |

| DSi Atheros Wifi - Xtensa CPU Core Opcodes |
| --- |

**Core Opcodes - Move/Load/Store**

```

| Opcode Native Nocash Expl.
 ii0st2h L8UI at,as,imm movb at,[as+imm8] Load 8bit Unsigned
 ii1st2h L16UI at,as,imm*2 movh at,[as+imm8*2] Load 16bit Unsigned
 ii9st2h L16SI at,as,imm*2 movsh at,[as+imm8*2] Load 16bit Signed
 ii2st2h L32I at,as,imm*4 mov at,[as+imm8*4] Load 32bit
 ii4st2h S8I at,as,imm movb [as+imm8],at Store 8bit
 ii5st2h S16I at,as,imm*2 movh [as+imm8*2],at Store 16bit
 ii6st2h S32I at,as,imm*4 mov [as+imm8*4],at Store 32bit
 iiiit1h L32R at,adr movp at,literal Load 32bit literal pool
 iiAit2h MOVI at,imm12 mov at,+/-imm12 Move Immediate(signed)
 83rst0h MOVEQZ ar,as,at movz at,ar,as Move if at=0 ;zero
 93rst0h MOVNEZ ar,as,at movnz at,ar,as Move if at<>0 ;nonzero
 A3rst0h MOVLTZ ar,as,at movs at,ar,as Move if at<0 ;negative
 B3rst0h MOVGEZ ar,as,at movns at,ar,as Move if at>=0 ;positive |
| --- |

**Core Opcodes - ALU**

```

| Opcode Native Nocash Expl.
 iiCst2h ADDI at,as,imm8 add at,as,+/-imm8 Add Immediate (signed)
 iiDst2h ADDMI at,as,imm add at,as,+/-imm8*256 Add Immediate*100h
 80rst0h ADD ar,as,at add ar,as,at Add (as+at)
 90rst0h ADDX2 ar,as,at add ar,at,as*2 Add shift1 (as*2+at)
 A0rst0h ADDX4 ar,as,at add ar,at,as*4 Add shift2 (as*4+at)
 B0rst0h ADDX8 ar,as,at add ar,at,as*8 Add shift3 (as*8+at)
 C0rst0h SUB ar,as,at sub ar,as,at Subtract (as-at)
 D0rst0h SUBX2 ar,as,at sub ar,as*2,at Sub shift1 (as*2-at)
 E0rst0h SUBX4 ar,as,at sub ar,as*4,at Sub shift2 (as*4-at)
 F0rst0h SUBX8 ar,as,at sub ar,as*8,at Sub shift3 (as*8-at)
 60r0t0h NEG ar,at neg ar,at Negate
 60r1t0h ABS ar,at abs ar,at Absolute Value
 10rst0h AND ar,as,at and ar,as,at Bitwise Logical And
 20rst0h OR ar,as,at or ar,as,at ;akaMOV Bitwise Logical Or
 30rst0h XOR ar,as,at xor ar,as,at Bitwise Logical Xor |
| --- |

**Core Opcodes - Shift**

```

| Opcode Native Nocash Expl.
 01rsi0h SLLI ar,as,32-imm5 shl ar,as,32-imm5 Shift Left Logical
 21rit0h SRAI ar,at,imm5 sar ar,at,imm5 Shift Right Arithmetic
 41rit0h SRLI ar,at,imm4 shr ar,at,imm4 Shift Right Logical
 m4rst0h EXTUI ar,at,s,m shrmask ar,at,imm5,mask ExtractUnsignedImm
 81rst0h SRC ar,as,at shr ar,as,at,shiftreg Shift Right Combined
 91r0t0h SRL ar,at shr ar,at,shiftreg Shift Right Logical
 A1rs00h SLL ar,as shl ar,as,shiftreg ?? Shift Left Logical
 B1r0t0h SRA ar,at sar ar,at,shiftreg Shift Right Arithmetic
 400s00h SSR as mov shiftreg,as SetShiftAm for RightSh
 401s00h SSL as sub shiftreg,32,as SetShiftAm for LeftSh
 402s00h SSA8L as mov shiftreg,as*8 SetShiftAmFor LE shift
 403s00h SSA8B as sub shiftreg,32,as*8 SetShiftAmFor BE shift
 404i.0h SSAI imm5 mov shiftreg,imm5sar SetShiftAm Immediate |
| --- |

**Core Opcodes - Jump/Call**

```

| Opcode Native Nocash Expl.
 iiii06h J adr jmp rel18 Unconditional Jump
 000sA0h JX as jmp as Unconditional Jump Reg
 iiii05h CALL0 adr call0 rel18x4 Non-windowed Call
 000sC0h CALLX0 as call0 as Non-windowed Call Reg
 000080h RET ;(jx a0) ret ;(jx a0) Non-Windowed Return |
| --- |

**Core Opcodes - Conditional Jump**

```

| Opcode Native Nocash Branch if...
 iiis16h BEQZ as,adr jz as,rel12 as=0
 iiis56h BNEZ as,adr jnz as,rel12 as<>0
 iiis96h BLTZ as,adr js as,rel12 as<0 (signed)
 iiisD6h BGEZ as,adr jns as,rel12 as>=0 (signed)
 iics26h BEQI as,c,adr je as,const4,rel8 as=Imm4(c)
 iics66h BNEI as,c,adr jne as,const4,rel8 as<>Imm4(c)
 iicsA6h BLTI as,c,adr jl as,const4,rel8 as<Imm4(c) (signed)
 iicsE6h BGEI as,c,adr jge as,const4,rel8 as>=Imm4(c) (signed)
 iicsB6h BLTUI as,c,adr jb as,const4u,rel8 as<UnsiImm4 (unsigned)
 iicsF6h BGEUI as,c,adr jae as,const4u,rel8 as>=UnsiImm4 (unsigned)
 ii1st7h BEQ as,at,adr je as,at,rel8 as=at equal
 ii9st7h BNE as,at,adr jne as,at,rel8 as<>at not equal
 ii2st7h BLT as,at,adr jl as,at,rel8 as<at less (signed)
 iiAst7h BGE as,at,adr jge as,at,rel8 as>=at gt/eq (signed)
 ii3st7h BLTU as,at,adr jb as,at,rel8 as<at less (unsigned)
 iiBst7h BGEU as,at,adr jae as,at,rel8 as>=at gt/eq (unsigned)
 ii0st7h BNONE as,at,adr tstjz as,at,rel8 (as AND at)=0 ;none
 ii8st7h BANY as,at,adr tstjnz as,at,rel8 (as AND at)<>0 ;any set
 ii4st7h BALL as,at,adr tstje as,at,rel8 (as AND at)=at ;all set
 iiCst7h BNALL as,at,adr tstjne as,at,rel8 (as AND at)<>at;not all
 ii5st7h BBC as,at,adr tstjz as,1 shl at,rel8 (as AND (1 shl at))=0
 ii6sb7h BBCI as,b,adr tstjz as,1 shl imm5,r8 (as AND (1 shl imm))=0
 iiDst7h BBS as,at,adr tstjnz as,1 shl at,rel8 (as AND (1 shl at))<>0
 iiEsb7h BBSI as,b,adr tstjnz as,1 shl imm5,r8 (as AND (1 shl imm))<>0 |
| --- |

**Core Opcodes - Misc**

```

| Opcode Native Nocash Expl.
 406st0h RER at,as mov at,ext[as] ReadExternal Register
 407st0h WER at,as mov ext[as],at WriteExternalRegister
 03iit0h RSR at,imm8 mov at,special[imm8] ReadSpecial Register
 13iit0h WSR at,imm8 mov special[imm8],at WriteSpecialRegister
 61iit0h XSR at,imm8 xchg at,special[imm8] ExchangeSpecialRegister
 002000h ISYNC isync Instruction Fetch Sync
 002010h RSYNC rsync Register Read Sync
 002020h ESYNC esync Execute Synchronize
 002030h DSYNC dsync Load/Store Synchronize
 0020C0h MEMW memwait Memory Wait
 0020D0h EXTW extwait External Wait
 0020F0h NOP nop No-Operation |
| --- |

**Pseudo Opcodes**

```

| MOV ar,as Macro (=OR ar,as,as)
 NOP Alias for "OR An,An,An" (alternate, instead of 0020F0h)
 J.L adr,as Macro (J or LiteralLoad+JX)
 BBCI.L as,b,adr Macro Branch Bit Clear Imm5 LE
 BBSI.L as,b,adr Macro Branch Bit Set Imm5 LE
 SRLI ar,at,imm5 Alias for "SRLI ar,at,imm4" or EXTUI (when imm5>=16) |
| --- |

More (inofficial) pseudos...

```

| mov br,bs or br,bs,bs
 mov br,0 and br,bs,not bs
 mov br,1 or br,bs,not bs
 sub at,as,imm add at,as,-imm
 mov sfr_xxx mov special[imm8]
 alu ax,... alu ax,ax,... |
| --- |

| DSi Atheros Wifi - Xtensa CPU Optional General Opcodes |
| --- |

**Boolean Option**

```

| Opcode Native Nocash
 008st0h ANY4 bt,bs or bt,bs..bs+3 Any 4 Booleans True
 009st0h ALL4 bt,bs and bt,bs..bs+3 All 4 Booleans True
 00Ast0h ANY8 bt,bs or bt,bs..bs+7 Any 8 Booleans True
 00Bst0h ALL8 bt,bs and bt,bs..bs+7 All 8 Booleans True
 02rst0h ANDB br,bs,bt and br,bs,bt BooleanAnd
 12rst0h ANDBC br,bs,bt and br,bs,not bt BooleanAndComplement(t)
 22rst0h ORB br,bs,bt or br,bs,bt BooleanOr
 32rst0h ORBC br,bs,bt or br,bs,not bt BooleanOrComplement(t)
 42rst0h XORB br,bs,bt xor br,bs,bt Boolean Xor
 C3rst0h MOVF ar,as,bt movz bt,ar,as Move if False
 D3rst0h MOVT ar,as,bt movnz bt,ar,as Move if True
 ii0s76h BF bs,adr jz bs,rel8 Branch if False
 ii1s76h BT bs,adr jnz bs,rel8 Branch if True |
| --- |

**Misc Option**

```

| 40Est0h NSA at,as nsa at,as Normaliz.ShiftAmount
 40Fst0h NSAU at,as nsau at,as Norma.ShiftAmUnsigned
 23rsi0h SEXT ar,as,imm sext ar,as,imm4+7 Sign Extend 7..22
 33rsi0h CLAMPS ar,as,imm clamps ar,as,imm4+7 Signed Clamp minmax
 43rst0h MIN ar,as,at min ar,as,at Minimum Value Signed
 53rst0h MAX ar,as,at max ar,as,at Maximum Value Signed
 63rst0h MINU ar,as,at minu ar,as,at Minimum Value Unsigned
 73rst0h MAXU ar,as,at maxu ar,as,at Maximum Value Unsigned |
| --- |

**Loop Option**

```

| ii8s76h LOOP as,adr loop as,rel8abs Loop
 ii9s76h LOOPNEZ as,adr loopnz as,rel8abs Loop if NotEqual zero
 iiAs76h LOOPGTZ as,adr loopgtz as,rel8abs Loop if Greater zero |
| --- |

**Windowed Code Option**

```

| iiii15h CALL4 adr call4 rel18x4 Call RotateWinBy4
 iiii25h CALL8 adr call8 rel18x4 Call RotateWinBy8
 iiii35h CALL12 adr call12 rel18x4 Call RotateWinBy12
 000sD0h CALLX4 as call4 as Call RegRotateBy4
 000sE0h CALLX8 as call8 as Call RegRotateBy8
 000sF0h CALLX12 as call12 as Call RegRotateBy12
 iiis36h ENTRY as,imm*8 entry as,imm12*8 Subroutine Entry
 000090h RETW retw Windowed-Return
 003400h RFWO ret_wo RetFromWinOverflow
 003500h RFWU ret_wu RetFromWinUnderflw
 001st0h MOVSP at,as movsp at,as Move to Stack Ptr
 4080i0h ROTW imm4 rotw imm4 Rotate Window -8..+7
 09ist0h L32E at,as,imm mov_e at,[as-imm*4] Load32bitException
 49ist0h S32E at,as,imm mov_e [as-imm*4],at StrWinForExcepts |
| --- |

**Narrow Code Option**

```

| --ist8h L32I.N at,as,imm4*4 mov at,[as+imm4*4] Load 32bit
 --ist9h S32I.N at,as,imm4*4 mov [as+imm4*4],at Store 32bit
 --rstAh ADD.N ar,as,at add ar,as,at Add
 --rsiBh ADDI.N ar,as,imm4 add ar,as,imm4 Add Imm (0=-1 or 1..15)
 --is0Ch MOVI.N as,imm mov as,imm7 Move Imm (-32..95)
 --is8Ch BEQZ.N as,adr jz as,rel6abs Branch if as=0
 --isCCh BNEZ.N as,adr jnz as,rel6abs Branch if as<>0
 --0stDh MOV.N at,as mov at,as Move
 --F00Dh RET.N ;(jx a0) ret ;jx a0 Non-Windowed Return
 --F01Dh RETW.N retw Windowed Return
 --F06Dh ILL.N ill Xcept Illegal Instr.
 --Fi2Dh BREAK.N imm4 break imm4 Debug Breakpoint
 --F03Dh NOP.N nop No-Operation |
| --- |

**Mul16 Option**

```

| C1rst0h MUL16U ar,as,at umul16 ar,as,at Multiply16bitUnsigned
 D1rst0h MUL16S ar,as,at smul16 ar,as,at Multiply16bitSigned |
| --- |

**Mul32 Option**

```

| 82rst0h MULL ar,as,at mul ar,as,at Multiply Low
 A2rst0h MULUH ar,as,at umulhi ar,as,at MultiplyUnsignedHigh
 B2rst0h MULSH ar,as,at smulhi ar,as,at MultiplySignedHigh |
| --- |

**Div32 Option**

```

| C2rst0h QUOU ar,as,at udiv ar,as,at Quotient Unsigned
 D2rst0h QUOS ar,as,at sdiv ar,as,at Quotient Signed
 E2rst0h REMU ar,as,at udivrem ar,as,at Remainder Unsigned
 F2rst0h REMS ar,as,at sdivrem ar,as,at Remainder Signed |
| --- |

| DSi Atheros Wifi - Xtensa CPU Optional Exception/Cache/MMU Opcodes |
| --- |

**Interrupt Option**

```

| 006it0h RSIL at,level xchg at,ps,intlevel i Read/Set IntLevel
 007i00h WAITI level waiti ps,intlevel i Set IntLevel and Wait |
| --- |

**High-Priority Interrupt Option**

```

| 003i10h RFI level ret_i level RetFromHiPrioInt |
| --- |

**Exception Option**

```

| 000000h ILL ill Illegal Instruction
 002080h EXCW xceptwait Exception Wait
 003000h RFE ret_e RetFromException
 003100h RFUE ret_ue RetFromUserModeExcept
 003200h RFDE ret_de RetFromDoubleExcept
 005000h SYSCALL syscall System Call |
| --- |

**Debug Option**

```

| 004xy0h BREAK imm4,imm4 break imm8 Breakpoint
 F1E000h RFDO ret_do RetFromDebugOperat.
 F1Es10h RFDD ;s=??? rer_dd imm1 RetFromDebugDispatch |
| --- |

**MemECC/Parity Option**

```

| 003020h RFME ret_me RetFromMemError |
| --- |

**No Option**

```

| E3rii0h RUR ar,imm8 mov ar,user[imm8] Read User Register
 F3iit0h WUR at,imm8 mov user[imm8],at WriteUserRegister |
| --- |

**Region/MMU Option**

```

| 503st0h RITLB0 at,as mov at,itlb0[as] Read InstTLB Virtual
 507st0h RITLB1 at,as mov at,itlb1[as] Read InstTLB Translat
 50Bst0h RDTLB0 at,as mov at,dtlb0[as] Read DataTLB Virtual
 50Fst0h RDTLB1 at,as mov at,dtlb1[as] Read DataTLB Translat
 504s00h IITLB as inv itlb[as] Invalidate InstTLB
 50Cs00h IDTLB as inv dtlb[as] Invalidate DataTLB
 505st0h PITLB at,as probe at,itlb[as] Probe InstTLB
 50Dst0h PDTLB at,as probe at,dtlb[as] Probe DataTLB
 506st0h WITLB at,as mov itlb[as],at Write InstTLB Entry
 50Est0h WDTLB at,as mov dtlb[as],at Write DataTLB Entry |
| --- |

**Multiprocessor Option**

```

| iiBst2h L32AI at,as,i*4 mov_m at,[as+imm8*4] Load 32bit Acquire
 iiFst2h S32RI at,as,imm*4 mov_m [as+imm8*4],at Store 32bit Release |
| --- |

**Multiprocessor Conditional Store Option**

```

| iiEst2h S32C1I at,as,imm*4 s32c1i at,[as+imm8*4] CompareCond |
| --- |

**Data Cache Option**

```

| i07s82h DPFL as,imm4*16 cach_dpfl [as+imm4*16] PrefetchAndLock *
 i27s82h DHU as,imm4*16 cach_dhu [as+imm4*16] HitUnlock
 i37s82h DIU as,imm4*16 cach_diu [as+imm4*16] Index Unlock
 i47s82h DIWB as,imm4*16 cach_diwb [as+imm4*16] Index Writeback
 i57s82h DIWBI as,imm4*16 cach_diwbi [as+imm4*16] Index WbInvali.
 ii7s02h DPFR as,imm8*4 cach_dpfr [as+imm8*4] PrefetchForRead
 ii7s12h DPFW as,imm8*4 cach_dpfw [as+imm8*4] PrefetchForWrite
 ii7s22h DPFRO as,imm8*4 cach_dpfro [as+imm8*4] PrefetchForRdOnce
 ii7s32h DPFWO as,imm8*4 cach_dpfwo [as+imm8*4] PrefetchForWrOnce
 ii7s42h DHWB as,imm8*4 cach_dhwb [as+imm8*4] HitWriteback
 ii7s52h DHWBI as,imm8*4 cach_dhwbi [as+imm8*4] HitWritebackInv.
 ii7s62h DHI as,imm8*4 cach_dhi [as+imm8*4] HitInvalidate
 ii7s72h DII as,imm8*4 cach_dii [as+imm8*4] Index Invalidate |
| --- |

**Instruction Cache Option**

```

| i07sD2h IPFL as,imm4*16 cach_ipfl [as+imm4*16] PrefetchAndLock *
 i27sD2h IHU as,imm4*16 cach_ihu [as+imm4*16] Hit Unlock
 i37sD2h IIU as,imm4*16 cach_iiu [as+imm4*16] Index Unlock
 ii7sC2h IPF as,imm8*4 cach_ipf [as+imm8*4] Prefetch
 ii7sE2h IHI as,imm8*4 cach_ihi [as+imm8*4] Hit Invalidate
 ii7sF2h III as,imm8*4 cach_iii [as+imm8*4] Index Invalidate |
| --- |

**Data/Instruction Cache Test Options**

```

| F18st0h LDCT at,as cach_mov at,dCachTag[as] LoadDataCacheTag
 F10st0h LICT at,as cach_mov at,iCachTag[as] LoadInstCacheTag
 F12st0h LICW at,as cach_mov at,iCachDta[as] LoadInstCacheWord
 F19st0h SDCT at,as cach_mov dCachTag[as],at StoreDataCacheTag
 F11st0h SICT at,as cach_mov iCachTag[as],at StoreInstCacheTag
 F13st0h SICW at,as cach_mov iCachDta[as],at StoreInstCacheWord |
| --- |

**Unknown**

```

| 71xxx0h ACCER ... accer ... Unknown/Unspecified |
| --- |

**Custom Designer-Defined Opcode Option**

```

| x6xxx0h CUST ... cust ... DesignerDefinedOpcodes |
| --- |

**Simcall Option**

```

| 005100h SIMCALL simcall Non-HW Simulator-Call |
| --- |

| DSi Atheros Wifi - Xtensa CPU Optional Floating-Point Opcodes |
| --- |

**Float Option**

```

| 08rst0h LSX fr,as,at f_mov fr,[as+at] LoadSingleIndexed
 ii0st3h LSI ft,as,imm*4 f_mov ft,[as+imm8*4] LoadSingleImmediate
 48rst0h SSX fr,as,at f_mov [as+at],fr Store Single Indexed
 ii4st3h SSI ft,as,imm*4 f_mov [as+imm8*4],ft Store Single Immedia.
 18rst0h LSXU fr,as,at f_movupd fr,[as+at] LoadSingleIndexed+Upd
 ii8st3h LSIU ft,as,imm*4 f_movupd ft,[as+imm8*4] LoadSingleImm+Update
 58rst0h SSXU fr,as,at f_movupd [as+at],fr Store Single Indx+Upd
 iiCst3h SSIU ft,as,imm*4 f_movupd [as+imm8*4],ft Store Single Imm+Upd.
 0Arst0h ADD.S fr,fs,ft f_add fr,fs,ft Add Single
 1Arst0h SUB.S fr,fs,ft f_sub fr,fs,ft Subtract Single
 2Arst0h MUL.S fr,fs,ft f_mul fr,fs,ft Multipy Single
 4Arst0h MADD.S fr,fs,ft f_muladd fr,fs,ft Multiply+Add Single
 5Arst0h MSUB.S fr,fs,ft f_mulsub fr,fs,ft Multiply+Sub Single
 8Arsi0h ROUND.S ar,fs,imm4 f_round ar,fs,pow4 Round Single to Fixed
 9Arsi0h TRUNC.S ar,fs,imm4 f_trunc ar,fs,pow4 TruncateSingleToFixed
 EArsi0h UTRUNC.S ar,fs,imm4 f_utrunc ar,fs,pow4 UnsignedTruncatetoFix
 AArsi0h FLOOR.S ar,fs,imm4 f_floor ar,fs,pow4 FloorSingleToFixed
 BArsi0h CEIL.S ar,fs,imm4 f_ceil ar,fs,pow4 Ceiling SingleToFixed
 CArsi0h FLOAT.S fr,as,imm4 f_float fr,as,frac4 ConvertFixedToSingle
 DArsi0h UFLOAT.S fr,as,imm4 f_ufloat fr,as,frac4 UnsignedFixedToSingle
 FArs00h MOV.S fr,fs f_mov fr,fs Move Single
 FArs10h ABS.S fr,fs f_abs fr,fs Absolute Value Single
 FArs40h RFR ar,fs f_mov ar,fs Move FR to AR
 FArs50h WFR fr,as f_mov fr,as Move AR to FR
 FArs60h NEG.S fr,fs f_neg fr,fs Negate Single
 1Brst0h UN.S br,fs,ft f_cmp_un br,fs,ft CompareSingle Unord
 2Brst0h OEQ.S br,fs,ft f_cmp_oeq br,fs,ft CompareSingle Equal
 3Brst0h UEQ.S br,fs,ft f_cmp_ueq br,fs,ft CompareSingle UnordEq
 4Brst0h OLT.S br,fs,ft f_cmp_olt br,fs,ft CompareSingle OrdLt
 5Brst0h ULT.S br,fs,ft f_cmp_ult br,fs,ft CompareSingle UnorLt
 6Brst0h OLE.S br,fs,ft f_cmp_ole br,fs,ft CompareSingle OrdLt/Eq
 7Brst0h ULE.S br,fs,ft f_cmp_ule br,fs,ft CompareSingle UnorLtEq
 8Brst0h MOVEQZ.S fr,fs,at f_movz at,fr,fs Move Single if at=0
 9Brst0h MOVNEZ.S fr,fs,at f_movnz at,fr,fs Move Single if at<>0
 ABrst0h MOVLTZ.S fr,fs,at f_movs at,fr,fs Move Single if at<0
 BBrst0h MOVGEZ.S fr,fs,at f_movns at,fr,fs Move Single if at>=0
 CBrst0h MOVF.S fr,fs,bt f_movz bt,fr,fs Move Single if bt=0
 DBrst0h MOVT.S fr,fs,bt f_movnz bt,fr,fs Move Single if bt=1 |
| --- |

pow4: (imm: opcode.bit7..4 = 0..15 aka (1 shl 0..15) aka 1..8000h)

frac4: (imm: opcode.bit7..4 = 0..15 aka (1 shr 0..15) aka 1..1/8000h)

| DSi Atheros Wifi - Xtensa CPU Optional MAC16 Opcodes |
| --- |

**MAC16 Option**

```

| mw = m0..m3
 mx = m0..m1
 my = m2..m3
 as,at = a0..a15
 acc = special register acchi(8bit):acclo(32bit)
 700st4h UMUL.AA.LL as,at umul acc,as_l,at_l ;\
 710st4h UMUL.AA.HL as,at umul acc,as_h,at_l ; Unsigned Mul
 720st4h UMUL.AA.LH as,at umul acc,as_l,at_h ; acc=as*at
 730st4h UMUL.AA.HH as,at umul acc,as_h,at_h ;/
 24x0y4h MUL.DD.LL mx,my smul acc,mx_l,my_l ;\
 25x0y4h MUL.DD.HL mx,my smul acc,mx_h,my_l ; Signed Mul
 26x0y4h MUL.DD.LH mx,my smul acc,mx_l,my_h ; acc=mx*my
 27x0y4h MUL.DD.HH mx,my smul acc,mx_h,my_h ;/
 340sy4h MUL.AD.LL as,my smul acc,as_l,my_l ;\
 350sy4h MUL.AD.HL as,my smul acc,as_h,my_l ; Signed Mul
 360sy4h MUL.AD.LH as,my smul acc,as_l,my_h ; acc=as*my
 370sy4h MUL.AD.HH as,my smul acc,as_h,my_h ;/
 64x0t4h MUL.DA.LL mx,at smul acc,mx_l,at_l ;\
 65x0t4h MUL.DA.HL mx,at smul acc,mx_h,at_l ; Signed Mul
 66x0t4h MUL.DA.LH mx,at smul acc,mx_l,at_h ; acc=mx*at
 67x0t4h MUL.DA.HH mx,at smul acc,mx_h,at_h ;/
 740st4h MUL.AA.LL as,at smul acc,as_l,at_l ;\
 750st4h MUL.AA.HL as,at smul acc,as_h,at_l ; Signed Mul
 760st4h MUL.AA.LH as,at smul acc,as_l,at_h ; acc=as*at
 770st4h MUL.AA.HH as,at smul acc,as_h,at_h ;/
 28x0y4h MULA.DD.LL mx,my smuladd acc,mx_l,my_l ;\
 29x0y4h MULA.DD.HL mx,my smuladd acc,mx_h,my_l ; Signed MulAdd
 2Ax0y4h MULA.DD.LH mx,my smuladd acc,mx_l,my_h ; acc=acc+mx*my
 2Bx0y4h MULA.DD.HH mx,my smuladd acc,mx_h,my_h ;/
 380sy4h MULA.AD.LL as,my smuladd acc,as_l,my_l ;\
 390sy4h MULA.AD.HL as,my smuladd acc,as_h,my_l ; Signed MulAdd
 3A0sy4h MULA.AD.LH as,my smuladd acc,as_l,my_h ; acc=acc+as*my
 3B0sy4h MULA.AD.HH as,my smuladd acc,as_h,my_h ;/
 68x0t4h MULA.DA.LL mx,at smuladd acc,mx_l,at_l ;\
 69x0t4h MULA.DA.HL mx,at smuladd acc,mx_h,at_l ; Signed MulAdd
 6Ax0t4h MULA.DA.LH mx,at smuladd acc,mx_l,at_h ; acc=acc+mx*at
 6Bx0t4h MULA.DA.HH mx,at smuladd acc,mx_h,at_h ;/
 780st4h MULA.AA.LL as,at smuladd acc,as_l,at_l ;\
 790st4h MULA.AA.HL as,at smuladd acc,as_h,at_l ; Signed MulAdd
 7A0st4h MULA.AA.LH as,at smuladd acc,as_l,at_h ; acc=acc+as*at
 7B0st4h MULA.AA.HH as,at smuladd acc,as_h,at_h ;/
 2Cx0y4h MULS.DD.LL mx,my smulsub acc,mx_l,my_l ;\
 2Dx0y4h MULS.DD.HL mx,my smulsub acc,mx_h,my_l ; Signed MulSub
 2Ex0y4h MULS.DD.LH mx,my smulsub acc,mx_l,my_h ; acc=acc-mx*my
 2Fx0y4h MULS.DD.HH mx,my smulsub acc,mx_h,my_h ;/
 3C0sy4h MULS.AD.LL as,my smulsub acc,as_l,my_l ;\
 3D0sy4h MULS.AD.HL as,my smulsub acc,as_h,my_l ; Signed MulSub
 3E0sy4h MULS.AD.LH as,my smulsub acc,as_l,my_h ; acc=acc-as*my
 3F0sy4h MULS.AD.HH as,my smulsub acc,as_h,my_h ;/
 6Cx0t4h MULS.DA.LL mx,at smulsub acc,mx_l,at_l ;\
 6Dx0t4h MULS.DA.HL mx,at smulsub acc,mx_h,at_l ; Signed MulSub
 6Ex0t4h MULS.DA.LH mx,at smulsub acc,mx_l,at_h ; acc=acc-mx*at
 6Fx0t4h MULS.DA.HH mx,at smulsub acc,mx_h,at_h ;/
 7C0st4h MULS.AA.LL as,at smulsub acc,as_l,at_l ;\
 7D0st4h MULS.AA.HL as,at smulsub acc,as_h,at_l ; Signed MulSub
 7E0st4h MULS.AA.LH as,at smulsub acc,as_l,at_h ; acc=acc-as*at
 7F0st4h MULS.AA.HH as,at smulsub acc,as_h,at_h ;/
 80ws04h LDINC mw,as movupd mw,[as+4] ;Load+AutoInc
 90ws04h LDDEC mw,as movupd mw,[as-4] ;Load+AutoDec |
| --- |

Below opcodes are doing two separate things:

1. acc=acc+x*y ;Signed MulAdd

2. as=as+/-4, mw=[as] ;Load mw from memory (for use by NEXT opcode)

```

| 08wsy4h MULA.DD.LL.LDINC mw,as,mx,my smuladd_movupd acc,mx_l,my_l,mw,[as+4]
 09wsy4h MULA.DD.HL.LDINC mw,as,mx,my smuladd_movupd acc,mx_h,my_l,mw,[as+4]
 0Awsy4h MULA.DD.LH.LDINC mw,as,mx,my smuladd_movupd acc,mx_l,my_h,mw,[as+4]
 0Bwsy4h MULA.DD.HH.LDINC mw,as,mx,my smuladd_movupd acc,mx_h,my_h,mw,[as+4]
 18wsy4h MULA.DD.LL.LDDEC mw,as,mx,my smuladd_movupd acc,mx_l,my_l,mw,[as-4]
 19wsy4h MULA.DD.HL.LDDEC mw,as,mx,my smuladd_movupd acc,mx_h,my_l,mw,[as-4]
 1Awsy4h MULA.DD.LH.LDDEC mw,as,mx,my smuladd_movupd acc,mx_l,my_h,mw,[as-4]
 1Bwsy4h MULA.DD.HH.LDDEC mw,as,mx,my smuladd_movupd acc,mx_h,my_h,mw,[as-4]
 48wst4h MULA.DA.LL.LDINC mw,as,mx,at smuladd_movupd acc,mx_l,at_l,mw,[as+4]
 49wst4h MULA.DA.HL.LDINC mw,as,mx,at smuladd_movupd acc,mx_h,at_l,mw,[as+4]
 4Awst4h MULA.DA.LH.LDINC mw,as,mx,at smuladd_movupd acc,mx_l,at_h,mw,[as+4]
 4Bwst4h MULA.DA.HH.LDINC mw,as,mx,at smuladd_movupd acc,mx_h,at_h,mw,[as+4]
 58wst4h MULA.DA.LL.LDDEC mw,as,mx,at smuladd_movupd acc,mx_l,at_l,mw,[as-4]
 59wst4h MULA.DA.HL.LDDEC mw,as,mx,at smuladd_movupd acc,mx_h,at_l,mw,[as-4]
 5Awst4h MULA.DA.LH.LDDEC mw,as,mx,at smuladd_movupd acc,mx_l,at_h,mw,[as-4]
 5Bwst4h MULA.DA.HH.LDDEC mw,as,mx,at smuladd_movupd acc,mx_h,at_h,mw,[as-4] |
| --- |

| DSi Atheros Wifi - Xtensa CPU Opcode Encoding Tables |
| --- |

**Xtensa Opcodes**

Xtensa opcodes are 24bit wide (or 16bit for "narrow" opcodes), the opcodes
consist of several 4bit fields (or 2bit, 8bit, 12bit, 16bit, 18bit fields in
some cases):

```

| 23-20 19-16 15-12 11-8 7-4 3-0 Type
 op2 op1 r s t op0 RRR
 imm4 op1 r s t op0 RRI4
 imm8--------> r s t op0 RRI8
 imm16-------------------> t op0 RRI16
 op2 op1 rs--------> t op0 RSR
 offset----------------------> n op0 CALL
 op2 op1 r s m n op0 CALLX
 imm8--------> r s m n op0 BRI8
 imm12-------------> s m n op0 BRI8
 r s t op0 RRRN
 imm.l s imm.h op0 RI7 (bit7="i")
 imm.l s imm.h op0 RI6 (bit7="i", bit6="z") |
| --- |

**Xtensa Opcode Root- and Subtables (Summary)**

Opcode decoding can be done in 4bit units, starting at the "op0" field, and
then decoding further 4bit field(s) depending on the opcode. Decoding speed
could be improved by grouping two 4bit fields into a 8bit field (eg. op2 and
op1; that won't work out perfectly for all opcodes though).

```

| ROOT\ op0
 ROOT\QRST op0=0, op1
 ROOT\QRST\RST0 op0=0, op1=0, op2
 ROOT\QRST\RST0\ST0 op0=0, op1=0, op2=0, r
 ROOT\QRST\RST0\ST0\SNM0 op0=0, op1=0, op2=0, r=0, mn
 ROOT\QRST\RST0\ST0\SYNC op0=0, op1=0, op2=0, r=2, t
 ROOT\QRST\RST0\ST0\RFEI op0=0, op1=0, op2=0, r=3, t
 ROOT\QRST\RST0\ST0\RFEI\RFET op0=0, op1=0, op2=0, r=3, t=0, s
 ROOT\QRST\RST0\ST1 op0=0, op1=0, op2=4, r
 ROOT\QRST\RST0\TLB op0=0, op1=0, op2=5, r
 ROOT\QRST\RST0\RT0 op0=0, op1=0, op2=6, s
 ROOT\QRST\RST1 op0=0, op1=1, op2
 ROOT\QRST\RST1\IMP op0=0, op1=1, op2=F, r
 ROOT\QRST\RST1\IMP\RFDX op0=0, op1=1, op2=F, r=E, t
 ROOT\QRST\RST2 op0=0, op1=2, op2
 ROOT\QRST\RST3 op0=0, op1=3, op2
 ROOT\QRST\LSCX op0=0, op1=8, op2
 ROOT\QRST\LSC4 op0=0, op1=9, op2
 ROOT\QRST\FP0 op0=0, op1=A, op2
 ROOT\QRST\FP0\FP1OP op0=0, op1=A, op2=F, t
 ROOT\QRST\FP1 op0=0, op1=B, op2
 ROOT\LSAI op0=2, r
 ROOT\LSAI\CACHE op0=2, r=7, t
 ROOT\LSAI\CACHE\DCE op0=2, r=7, t=8, op1
 ROOT\LSAI\CACHE\ICE op0=2, r=7, t=D, op1
 ROOT\LSCI op0=3, r
 ROOT\MAC16 op0=4, op2
 ROOT\MAC16\MACID op0=4, op2=0, op1
 ROOT\MAC16\MACCD op0=4, op2=1, op1
 ROOT\MAC16\MACDD op0=4, op2=2, op1
 ROOT\MAC16\MACAD op0=4, op2=3, op1
 ROOT\MAC16\MACIA op0=4, op2=4, op1
 ROOT\MAC16\MACCA op0=4, op2=5, op1
 ROOT\MAC16\MACDA op0=4, op2=6, op1
 ROOT\MAC16\MACAA op0=4, op2=7, op1
 ROOT\MAC16\MACI op0=4, op2=8, op1
 ROOT\MAC16\MACC op0=4, op2=9, op1
 ROOT\CALLN op0=5, mn
 ROOT\SI op0=6, mn (and \SI\BZ, \SI\BI0, \SI\BI1)
 ROOT\SI\BI1\B1 op0=6, mn=7, r
 ROOT\B op0=7, r
 ROOT\ST2 op0=C, t
 ROOT\ST3 op0=D, r
 ROOT\ST3\S3 op0=D, r=F, t |
| --- |

**Xtensa Opcode Root- and Subtables (Complete)**

Below is showing the whole opcode "tree", starting with the root table (indexed
via op0). Entries with "-->" are referencing to child tables, the other
entries are indicating the actual opcodes (or reserved opcodes). The lower-case
suffices are somewhat indicating optional opcodes.

```

| ROOT\ ROOT\QRST ROOT\QRST\RS ROOT\QRST\RST0\ST0
 op0 op1 op2 r
 0 --> QRST --> RST0 --> ST0 --> SNM0
 1 L32R --> RST1 AND MOVSP
 2 --> LSAI --> RST2 OR --> SYNC
 3 --> LSCIp --> RST3 XOR --> RFEIx
 4 --> MAC16d EXTUI ;\ --> ST1 BREAKx
 5 --> CALLN EXTUI ;/ --> TLB SYSCALLx
 6 --> SI CUST0 ;\ --> RT0 RSILx
 7 --> B CUST1 ;/ reserved WAITIx (t=0)
 8 L32I.Nn ;\ --> LSCXp ADD ANY4p
 9 S32I.Nn ; --> LSC4 ADDX2 ALL4p
 A ADD.Nn ; narrow --> FP0f ADDX4 ANY8p
 B ADDI.Nn ; 16bit --> FP1f ADDX8 ALL8p
 C --> ST2n ; reserved SUB reserved
 D --> ST3n ;/ reserved SUBX2 reserved
 E reserved reserved SUBX4 reserved
 F reserved reserved SUBX8 reserved |
| --- |

```

| ..\RST0\ST0\SNM0 ..\RST0\ST0\SYNC ..\RST0\ST0\RFEI ..\ST0\RFEI\RFET
 mn t t s
 0 ILL ;\ ISYNC --> RFETx RFEx
 1 reserved ; ILL RSYNC RFIx RFUEx
 2 reserved ; ESYNC RFME (s=0) RFDEx
 3 reserved ;/ DSYNC reserved reserved
 4 reserved ;\ reserved reserved RFWOw
 5 reserved ; N/A reserved reserved RFWUw
 6 reserved ; reserved reserved reserved
 7 reserved ;/ reserved reserved reserved
 8 RET ;\ EXCW reserved reserved
 9 RETWw ; JR reserved reserved reserved
 A JX ; reserved reserved reserved
 B reserved ;/ reserved reserved reserved
 C CALLX0 ;\ MEMW reserved reserved
 D CALLX4w ; CALLX EXTW reserved reserved
 E CALLX8w ; reserved reserved reserved
 F CALLX12w ;/ NOP/reserved reserved reserved |
| --- |

```

| ..\RST0\ST1 ..\RST0\TLB ..\RST0\RT0 ROOT\QRST\RST1
 r r s op2
 0 SSR (t=0) reserved NEG SLLI ;\
 1 SSL (t=0) reserved ABS SLLI ;/
 2 SSA8L (t=0) reserved reserved SRAI ;\
 3 SSA8B (t=0) RITLB0 reserved SRAI ;/
 4 SSAI (t=0) IITLB (t=0) reserved SRLI ;-
 5 reserved PITLB reserved reserved
 6 RER WITLB reserved XSR
 7 WER RITLB1 reserved --> ACCER (?)
 8 ROTWw (s=0) reserved reserved SRC
 9 reserved reserved reserved SRL (s=0)
 A reserved reserved reserved SLL (t=0)
 B reserved RDTLB0 reserved SRA (s=0)
 C reserved IDTLB (t=0) reserved MUL16U
 D reserved PDTLB reserved MUL16S
 E NSAu WDTLB reserved reserved
 F NSAUu RDTLB1 reserved --> IMP |
| --- |

```

| ..\RST1\IMP ..\RST1\IMP\RFDX ROOT\QRST\RST2 ROOT\QRST\RST3
 r t op2 op2
 0 LICT RFDO (s=0) ABDBp RSR
 1 SICT RFDD (s=0,1) ANDBCp WSR
 2 LICW reserved ORBp SEXTu
 3 SICW reserved ORBCp CLAMPSu
 4 reserved reserved XORBp MINu
 5 reserved reserved reserved MAXu
 6 reserved reserved reserved MINUu
 7 reserved reserved reserved MAXUu
 8 LDCT reserved MULLi MOVEQZ
 9 SDCT reserved reserved MOVNEZ
 A reserved reserved MULUHi MOVLTZ
 B reserved reserved MULSHi MOVGEZ
 C reserved reserved QUOUi MOVFp
 D reserved reserved QUOSi MOVTp
 E --> RFDX reserved REMUi RUR
 F reserved reserved REMSi WUR |
| --- |

```

| ROOT\QRST\LSCX ROOT\QRST\LSC4 ROOT\QRST\FP0 ROOT\QRST\FP0\FP1OP
 op2 op2 op2 t
 0 LSXf L32E ADD.Sf MOV.Sf
 1 LSXUf reserved SUB.Sf ABS.Sf
 2 reserved reserved MUL.Sf reserved
 3 reserved reserved reserved reserved
 4 SSXf S32E MADD.Sf RFRf
 5 SSXUf reserved MSUB.Sf WFRf
 6 reserved reserved reserved NEG.Sf
 7 reserved reserved reserved reserved
 8 reserved reserved ROUND.Sf reserved
 9 reserved reserved TRUNC.Sf reserved
 A reserved reserved FLOOR.Sf reserved
 B reserved reserved CEIL.Sf reserved
 C reserved reserved FLOAT.Sf reserved
 D reserved reserved UFLOAT.Sf reserved
 E reserved reserved UTRUNC.Sf reserved
 F reserved reserved --> FP1OPf reserved |
| --- |

```

| ROOT\QRST\FP1 ROOT\LSAI ROOT\LSAI\CACHE ..\CACHE\DCE
 op2 r t op1
 0 reserved L8UI DPFRc DPFLl
 1 UN.Sf L16UI DPFWc reserved
 2 OEQ.Sf L32I DPFROc DHUl
 3 UEQ.Sf reserved DPFWOc DIUl
 4 OLT.Sf S8I DHWBc DIWBc
 5 ULT.Sf S16I DHWBIc DIWBIc
 6 OLE.Sf S32I DHIc reserved
 7 ULE.Sf --> CACHEc DIIc reserved
 8 MOVEQZ.Sf reserved --> DCEc reserved
 9 MOVNEZ.Sf L16SI reserved reserved
 A MOVLTZ.Sf MOVI reserved reserved
 B MOVGEZ.Sf L32AIy reserved reserved
 C MOVF.Sf ADDI IPFc reserved
 D MOVT.Sf ADDMI --> ICEc reserved
 E reserved S32C1Iy IHIc reserved
 F reserved S32RIy IIIc reserved |
| --- |

```

| ..\CACHE\ICE ROOT\LSCI ROOT\CALLN ROOT\SI
 op1 r mn mn
 0 IPFLl LSIf CALL0 ;\ J
 1 reserved reserved CALL4 ; BEQZ
 2 IHUl reserved CALL8 ; BEQI
 3 IIUl reserved CALL12 ;/ ENTRYw
 4 reserved SSIf CALL0 ;\ J
 5 reserved reserved CALL4 ; BNEZ
 6 reserved reserved CALL8 ; BNEI
 7 reserved reserved CALL12 ;/ --> B1
 8 reserved LSIUf CALL0 ;\ J
 9 reserved reserved CALL4 ; BLTZ
 A reserved reserved CALL8 ; BLTI
 B reserved reserved CALL12 ;/ BLTUI
 C reserved SSIUf CALL0 ;\ J
 D reserved reserved CALL4 ; BGEZ
 E reserved reserved CALL8 ; BGEI
 F reserved reserved CALL12 ;/ BGEUI |
| --- |

```

| ROOT\SI\BI1\B1 ROOT\B ROOT\ST2 ROOT\ST3
 r r t r
 0 BFp BNONE MOVI.Nn ;\ MOV.Nn
 1 BTp BEQ MOVI.Nn ; reserved
 2 reserved BLT MOVI.Nn ; reserved
 3 reserved BLTU MOVI.Nn ; reserved
 4 reserved BALL MOVI.Nn ; reserved
 5 reserved BBC MOVI.Nn ; reserved
 6 reserved BBCI ;\ MOVI.Nn ; reserved
 7 reserved BBCI ;/ MOVI.Nn ;/ reserved
 8 LOOP BANY BEQZ.Nn ;\ reserved
 9 LOOPNEZ BNE BEQZ.Nn ; reserved
 A LOOPGTZ BGE BEQZ.Nn ; reserved
 B reserved BGEU BEQZ.Nn ;/ reserved
 C reserved BNALL BNEZ.Nn ;\ reserved
 D reserved BBS BNEZ.Nn ; reserved
 E reserved BBSI ;\ BNEZ.Nn ; reserved
 F reserved BBSI ;/ BNEZ.Nn ;/ --> S3 |
| --- |

```

| ROOT\ST3\S3 ROOT\MAC16 ROOT\MAC16\MACI ROOT\MAC16\MACC
 t op2 op1 op1
 0 RET.Nn --> MACID LDINC LDDEC
 1 RETW.Nwn --> MACCD reserved reserved
 2 BREAK.Nn --> MACDD reserved reserved
 3 NOP.Nn --> MACAD reserved reserved
 4 reserved --> MACIA reserved reserved
 5 reserved --> MACCA reserved reserved
 6 ILL.Nn --> MACDA reserved reserved
 7 reserved --> MACAA reserved reserved
 8 reserved --> MACI reserved reserved
 9 reserved --> MACC reserved reserved
 A reserved reserved reserved reserved
 B reserved reserved reserved reserved
 C reserved reserved reserved reserved
 D reserved reserved reserved reserved
 E reserved reserved reserved reserved
 F reserved reserved reserved reserved |
| --- |

```

| ROOT\MAC16\MACID ROOT\MAC16\MACCD ROOT\MAC16\MACIA ROOT\MAC16\MACCA
 op1 op1 op1 op1
 0 reserved reserved reserved reserved
 1 reserved reserved reserved reserved
 2 reserved reserved reserved reserved
 3 reserved reserved reserved reserved
 4 reserved reserved reserved reserved
 5 reserved reserved reserved reserved
 6 reserved reserved reserved reserved
 7 reserved reserved reserved reserved
 8 MULA.DD.LL.LDINC MULA.DD.LL.LDDEC MULA.DA.LL.LDINC MULA.DA.LL.LDDEC
 9 MULA.DD.HL.LDINC MULA.DD.HL.LDDEC MULA.DA.HL.LDINC MULA.DA.HL.LDDEC
 A MULA.DD.LH.LDINC MULA.DD.LH.LDDEC MULA.DA.LH.LDINC MULA.DA.LH.LDDEC
 B MULA.DD.HH.LDINC MULA.DD.HH.LDDEC MULA.DA.HH.LDINC MULA.DA.HH.LDDEC
 C reserved reserved reserved reserved
 D reserved reserved reserved reserved
 E reserved reserved reserved reserved
 F reserved reserved reserved reserved |
| --- |

```

| ROOT\MAC16\MACDD ROOT\MAC16\MACAD ROOT\MAC16\MACDA ROOT\MAC16\MACAA
 op1 op1 op1 op1
 0 reserved reserved reserved UMUL.AA.LL
 1 reserved reserved reserved UMUL.AA.HL
 2 reserved reserved reserved UMUL.AA.LH
 3 reserved reserved reserved UMUL.AA.HH
 4 MUL.DD.LL MUL.AD.LL MUL.DA.LL MUL.AA.LL
 5 MUL.DD.HL MUL.AD.HL MUL.DA.HL MUL.AA.HL
 6 MUL.DD.LH MUL.AD.LH MUL.DA.LH MUL.AA.LH
 7 MUL.DD.HH MUL.AD.HH MUL.DA.HH MUL.AA.HH
 8 MULA.DD.LL MULA.AD.LL MULA.DA.LL MULA.AA.LL
 9 MULA.DD.HL MULA.AD.HL MULA.DA.HL MULA.AA.HL
 A MULA.DD.LH MULA.AD.LH MULA.DA.LH MULA.AA.LH
 B MULA.DD.HH MULA.AD.HH MULA.DA.HH MULA.AA.HH
 C MULS.DD.LL MULS.AD.LL MULS.DA.LL MULS.AA.LL
 D MULS.DD.HL MULS.AD.HL MULS.DA.HL MULS.AA.HL
 E MULS.DD.LH MULS.AD.LH MULS.DA.LH MULS.AA.LH
 F MULS.DD.HH MULS.AD.HH MULS.DA.HH MULS.AA.HH |
| --- |

| DSi Atheros Wifi - Internal Memory Map |
| --- |

**Overall Memory Map (Internal Xtensa memory)**

Internal Memory contains I/O Ports, ROM, and RAM. Arranged as so:

```

| 00000000h I/O Ports
 000E0000h ROM ;\as so on AR6002 (other AR60xx chips may use
 00100000h RAM ;/slightly different addresses). |
| --- |

The whole memory is repeated every 400000h bytes (4MByte), this is just dumb
mirroring (although Atheros is referring to it as "virtual memory"). The first
three mirrors (in first 12Mbytes) are commonly used as so:

```

| 00000000h..003FFFFh used for I/O Port read/write ;1st mirror
 00400000h..007FFFFh used for ROM and RAM data read/write ;2nd mirror
 00800000h..00BFFFFh used for ROM and RAM opcode read ;3rd mirror
 00C00000h..FFFFFFFh normally unused ;4th..1024th |
| --- |

Unknown how far it is really required to use the mirrors that way. The
different areas might be related to different access rights, or caching
(although the AR6002/AR6003/AR6004 chips don't have any cache at all).

The AR6002 BIOS does usually read ROM data from 4E0000h..4F3FFFh, however, in
one case it's accidently reading ROM data from 8EEFF8h..8EF73Bh, so that's
apparently possible & working, too.

Using WINDOW_DATA to read memory works well for address 00000000h..FFFFFFFFh
(apart from some dangerous read-locations in the I/O area; as well as mirrors
of those I/O locations).

**RAM/ROM address/size, IDs, and various stuff for different chips**

```

| AR60xx chip name AR6002 AR6003 AR6004 AR6013 AR6014
 AR6002_rev alias REV2 REV4 REV6 REV2 REV2
 hw name hw2 hw4 hw6 hw2 hw2
 Nintendo DSi/3DS Old DSi N/A N/A New DSi 3DS/New3DS
 Wifi Board DWM-W015 N/A N/A DWM-W024 DWM-W028
 SPI FLASH ID Byte 01h N/A N/A 02h 03h
 SPI FLASH Size 4 or 128K N/A N/A 4K 4K
 I2C EEPROM SizeUsed 300h ? ? 300h 300h
 ROM Size (Kbyte) 80K 256K 512K 256K 256K
 RAM Size (Kbyte) 184K 256K 256-288K? 128K 128K
 IRAM Size (Kbyte) N/A N/A 160K? N/A N/A
 ROM Date 23nov2007 ? ? 18jul2008 25jun2009
 ROM ID version 2.0.0.392 ? ? 2.3.0.36 2.3.0.111
 Firmware version 2.1.0.123 ? ? 2.3.0.108 2.3.0.179+
 ROM Base 0E0000h 0E0000h 100000h 0E0000h 0E0000h
 ROM Reset Entry 8E0000h ? ? 8E0000h 8E0000h
 RAM Base 100000h 140000h? 000000h?? 120000h 120000h
 RAM Host Interest 500400h 540600h 400600h?? 520000h 520000h
 RAM Start of Free 502400h ? ? 524C00h 524C00h
 RAM BMI_DONE Entry 915000h ? ? 927000h 927000h
 CPU Litbase 52F000h+1 ? ? 54C000h+1 54C000h+1
 IRAM Base N/A N/A 998000h N/A N/A
 ROM Size (hex) 14000h 40000h 80000h 40000h 40000h
 RAM Size (hex) 2E000h 40000h 4xxxxh? 20000h 20000h
 ROM ID hex 20000188h ? ? 23000024h 2300006Fh
 Firm ID hex 2100007Bh ? ? 2300006Ch 230000B3h+
 CHIP_ID used 02000001h ? ? 0D000000h 0D000001h
 CHIP_ID alternate? 02010001h ? ? 0D00000xh ?
 BB_D2_CHIP_ID has any? ? ? has any? has any?
 SDIO MANFID 02000271h ? ? 02010271h 02010271h
 OSC 26MHz ? ? 40MHz 40MHz |
| --- |

**Special ROM Addresses**

There are only a few ROM locations with fixed/standarized addresses:

```

| Entrypoint: ROM_Base+0
 Exception Vectors: ROM_Base+xxx ?
 DataSet Address: ROM_Base+ROM_Size-8
 MBIST Cksum: ROM_Base+ROM_Size-4 ;MBIST = memory built-in-self-test ? |
| --- |

The firmware may call ROM functions, but the firmware code must be matched to
specific ROM versions: There is some call table in RAM, allowing to call (or
change) function vectors, but the table's location/indices vary for different
ROM versions. The RAM table doesn't contain entries for all ROM functions
though, so the firmware must use hardcoded addresses (like "8E35A0h") for
functions that aren't in the table.

Note that it's possible to patch ROM via TCAM/BCAM registers.

**Special RAM Addresses**

The first some kilobytes of RAM contain stuff like stack, call table,
variables, and host interest area (but the exact addresses of that regions
depend on the ROM version).

The remaining RAM could be more or less freely used by the firmware (whereas,
one should probably use ROM's memory allocation function for that purpose).

The CPU's "litbase" is dictacted by the ROM, so the firmware must adapt its
literal pool offsets to that value. The default BMI_DONE entrypoint is also
dictated by the ROM (although one could override it if neccessary).

**Hardware I/O Addresses**

The hardware I/O addresses are defined in source code folder
"include\AR6002\hw*". Whereas, the folder is called "AR6002", but it does also
contain defintions for AR6003 and AR6004. In a similar fashion, the source code
does also contain definitions like AR6002_REVn, whereas some of that "AR6002
revisions" are actually referring to AR6003 and AR6004. The naming scheme
appears to be as so:

```

| AR6002 = AR6002_REV2 = include\AR6002\hw2.0
 AR6003 = AR6002_REV4 = include\AR6002\hw4.0
 AR6004 = AR6002_REV6 = include\AR6002\hw6.0 |
| --- |

The above folders are included in all source code versions (newer atheros code
from 2010 is definetly no longer compatible with the DSi's AR6002 firmware,
however, concerning the hardware definitions, the "hw2.0" folder appears to
have stayed intact and should be still compatible with real hardware, even in
newer source code versions).

However, there are at least two different "hw4.0" versions, the newer one
containing some additional registers, and some changed register addresses.
There's also some AR6002_REV42 definition in some files, which might be related
to that "hw4.0" variant. Current theory would be that "hw4.0" means AR6003, and
the newer "hw4.0" should be actually "hw4.2" and means some different chip with
unknown name. Or whatever.

There's also a "include\AR6002\hw" folder, this is just some useless dupe,
containing the exact same files as "include\AR6002\hw2.0".

There's also something called "MCKINLEY", which seems to same (or similar) as
"AR6004".

The actual files in the above folders are containing some very ugly bloated
auto-generated definitions, definetly not suitable for human reading (except by
using some software parser for extracting relevant definitions).

| DSi Atheros Wifi - Internal I/O Map Summary (hw2) |
| --- |

**Overall Summary (hw2.0)**

```

| 004000h 1C4h Clock/RTC Registers (rtc_reg.h) (hw2.0)
 008000h 208h Memory Controller (TCAM) (vmc_reg.h) (hw2.0)
 00C000h 40h Serial UART (uart_reg.h) (hw2.0)
 010000h 18h Serial I2C/SPI Interface (si_reg.h) (hw2.0)
 0140F4h 4 GPIO Registers (gpio_reg.h) (hw2.0)
 018000h 114h MBOX Registers (mbox_reg.h) (hw2.0)
 01A000h 2000h HOST_IF_WINDOW (mbox_reg.h) (hw2.0)
 01C000h 64h Analog Intf Registers (analog_reg.h) (hw2.0)
 01C080h 10h Analog Intf Registers (analog_intf_reg.h) (hw2.0)
 020000h ? MAC DMA maybe, if any ?
 028000h 1800h MAC PCU Registers (mac_pcu.h) (hw2.0)
 029800h ... BB/LC maybe...
 029800h ? Used in hw2, but undocumented... maybe BASEBAND_0 alike hw4?
 02A000h ? Used in hw2, but undocumented... maybe BASEBAND_1 alike hw4?
 030000h ? Used in hw2, but undocumented... maybe RDMA or EFUSE or so? |
| --- |

**Clock/RTC Registers (rtc_reg.h) (hw2.0)**

```

| 004000h 4 (WLAN_)RESET_CONTROL
 004004h 4 (WLAN_)XTAL_CONTROL
 004008h 4 (WLAN_)TCXO_DETECT
 00400Ch 4 (WLAN_)XTAL_TEST
 004010h 4 (WLAN_)QUADRATURE
 004014h 4 (WLAN_)PLL_CONTROL
 004018h 4 (WLAN_)PLL_SETTLE
 00401Ch 4 (WLAN_)XTAL_SETTLE
 004020h 4 (WLAN_)CPU_CLOCK
 004024h 4 (WLAN_)CLOCK_OUT
 004028h 4 (WLAN_)CLOCK_CONTROL
 00402Ch 4 (WLAN_)BIAS_OVERRIDE
 004030h 4 (WLAN_)WDT_CONTROL ;\
 004034h 4 (WLAN_)WDT_STATUS ;
 004038h 4 (WLAN_)WDT ; Watchdog Timer
 00403Ch 4 (WLAN_)WDT_COUNT ;
 004040h 4 (WLAN_)WDT_RESET ;/
 004044h 4 (WLAN_)INT_STATUS ;-Interrupt Status
 004048h 4 (WLAN_)LF_TIMER0 ;\
 00404Ch 4 (WLAN_)LF_TIMER_COUNT0 ; Low-Freq Timer
 004050h 4 (WLAN_)LF_TIMER_CONTROL0 ;
 004054h 4 (WLAN_)LF_TIMER_STATUS0 ;/
 004058h 4 (WLAN_)LF_TIMER1 ;\
 00405Ch 4 (WLAN_)LF_TIMER_COUNT1 ; Low-Freq Timer
 004060h 4 (WLAN_)LF_TIMER_CONTROL1 ;
 004064h 4 (WLAN_)LF_TIMER_STATUS1 ;/
 004068h 4 (WLAN_)LF_TIMER2 ;\
 00406Ch 4 (WLAN_)LF_TIMER_COUNT2 ; Low-Freq Timer
 004070h 4 (WLAN_)LF_TIMER_CONTROL2 ;
 004074h 4 (WLAN_)LF_TIMER_STATUS2 ;/
 004078h 4 (WLAN_)LF_TIMER3 ;\
 00407Ch 4 (WLAN_)LF_TIMER_COUNT3 ; Low-Freq Timer
 004080h 4 (WLAN_)LF_TIMER_CONTROL3 ;
 004084h 4 (WLAN_)LF_TIMER_STATUS3 ;/
 004088h 4 (WLAN_)HF_TIMER ;\
 00408Ch 4 (WLAN_)HF_TIMER_COUNT ; High-Freq Timer
 004090h 4 (WLAN_)HF_LF_COUNT ;<-- ;
 004094h 4 (WLAN_)HF_TIMER_CONTROL ;
 004098h 4 (WLAN_)HF_TIMER_STATUS ;/
 00409Ch 4 (WLAN_)RTC_CONTROL ;\
 0040A0h 4 (WLAN_)RTC_TIME ;
 0040A4h 4 (WLAN_)RTC_DATE ;
 0040A8h 4 (WLAN_)RTC_SET_TIME ; Real-Time Clock
 0040ACh 4 (WLAN_)RTC_SET_DATE ;
 0040B0h 4 (WLAN_)RTC_SET_ALARM ;
 0040B4h 4 (WLAN_)RTC_CONFIG ;
 0040B8h 4 (WLAN_)RTC_ALARM_STATUS ;/
 0040BCh 4 (WLAN_)UART_WAKEUP
 0040C0h 4 (WLAN_)RESET_CAUSE
 0040C4h 4 (WLAN_)SYSTEM_SLEEP
 0040C8h 4 (WLAN_)SDIO_WRAPPER
 0040CCh 4 (WLAN_)MAC_SLEEP_CONTROL
 0040D0h 4 (WLAN_)KEEP_AWAKE
 0040D4h 4 (WLAN_)LPO_CAL_TIME ;\
 0040D8h 4 (WLAN_)LPO_INIT_DIVIDEND_INT ;
 0040DCh 4 (WLAN_)LPO_INIT_DIVIDEND_FRACTION ;
 0040E0h 4 (WLAN_)LPO_CAL ;
 0040E4h 4 (WLAN_)LPO_CAL_TEST_CONTROL ;
 0040E8h 4 (WLAN_)LPO_CAL_TEST_STATUS ;/
 0040ECh 4 (WLAN_)CHIP_ID
 0040F0h 4 (WLAN_)DERIVED_RTC_CLK
 0040F4h 4 MAC_PCU_SLP32_MODE
 0040F8h 4 MAC_PCU_SLP32_WAKE
 0040FCh 4 MAC_PCU_SLP32_INC
 004100h 4 MAC_PCU_SLP_MIB1
 004104h 4 MAC_PCU_SLP_MIB2
 004108h 4 MAC_PCU_SLP_MIB3
 00410Ch 4 MAC_PCU_SLP_BEACON ;<-- hw2.0 only (not hw4.0)
 004110h 4 (WLAN_)POWER_REG ;\located here in hw2.0
 004114h 4 (WLAN_)CORE_CLK_CTRL ;/
 004118h 1x8 PAD0 ;\
 004120h 4x8 SDIO_SETUP_CIRCUIT[8] ;
 004140h 4 SDIO_SETUP_CONFIG ;
 004144h 4 CPU_SETUP_CONFIG ; hw2.0 only (not hw4.0)
 004148h 1x24 PAD1 ;
 004160h 4x8 CPU_SETUP_CIRCUIT[8] ;
 004180h 4 BB_SETUP_CONFIG ;
 004184h 1x28 PAD2 ;
 0041A0h 4x8 BB_SETUP_CIRCUIT[8] ;/
 0041C0h 4 (WLAN_)GPIO_WAKEUP_CONTROL ;-located here in hw2.0 |
| --- |

**Memory Controller (vmc_reg.h) (hw2.0)**

```

| 008000h 4x32 (WLAN_)MC_TCAM_VALID[0..31] ;\
 008080h 4x32 (WLAN_)MC_TCAM_MASK[0..31] ; ROM Patches
 008100h 4x32 (WLAN_)MC_TCAM_COMPARE[0..31] ;
 008180h 4x32 (WLAN_)MC_TCAM_TARGET[0..31] ;/
 008200h 4 (WLAN_)ADDR_ERROR_CONTROL ;\ADDR_ERROR
 008204h 4 (WLAN_)ADDR_ERROR_STATUS ;/ |
| --- |

**Serial UART (uart_reg.h) (hw2.0)**

```

| 00C000h 4 (WLAN_UART_)RBR - RX Data FIFO (R) (when DLAB=0)
 00C000h 4 (WLAN_UART_)THR - TX Data FIFO (W) (when DLAB=0)
 00C000h 4 (WLAN_UART_)DLL - Baudrate Divisor LSB (R/W) (when DLAB=1)
 00C004h 4 (WLAN_UART_)IER - Interrupt Control (R/W) (when DLAB=0)
 00C004h 4 (WLAN_UART_)DLH - Baudrate Divisor MSB (R/W) (when DLAB=1)
 00C008h 4 (WLAN_UART_)IIR - Interrupt Status (R)
 00C008h 4 (WLAN_UART_)FCR - FIFO Control (W)
 00C00Ch 4 (WLAN_UART_)LCR - Character Format Control (R/W)
 00C010h 4 (WLAN_UART_)MCR - Handshaking Control (R/W)
 00C014h 4 (WLAN_UART_)LSR - RX/TX Status (R) (W=don't do)
 00C018h 4 (WLAN_UART_)MSR - Handshaking Status (R) (W=don't do)
 00C01Ch 4 (WLAN_UART_)SCR - Scratch (R/W)
 00C020h 4 (WLAN_UART_)SRBR - (mirror of RBR?) (when DLAB=0?)
 00C024h 1x4 PAD0
 00C028h 4 (WLAN_UART_)SIIR - (mirror or IIR?)
 00C02Ch 4 (WLAN_UART_?)MWR - Whatever "M Write Register?"
 00C030h 1x4 PAD1
 00C034h 4 (WLAN_UART_)SLSR - (mirror or LSR?) <-- used by AR6002 ROM
 00C038h 4 (WLAN_UART_)SMSR - (mirror of MSR?)
 00C03Ch 4 (WLAN_UART_?)MRR - Whatever "M Read Register?" |
| --- |

**Serial I2C/SPI Interface (si_reg.h) (hw2.0)**

```

| 010000h 4 SI_CONFIG
 010004h 4 SI_CS
 010008h 4 SI_TX_DATA0
 01000Ch 4 SI_TX_DATA1
 010010h 4 SI_RX_DATA0
 010014h 4 SI_RX_DATA1 |
| --- |

**GPIO Registers (gpio_reg.h) (hw2.0)**

```

| 014000h 4 (WLAN_)GPIO_OUT ;\GPIO Output Data
 014004h 4 (WLAN_)GPIO_OUT_W1TS ; (direct, and Write-1-To-Set/Clr)
 014008h 4 (WLAN_)GPIO_OUT_W1TC ;/
 01400Ch 4 (WLAN_)GPIO_ENABLE ;\GPIO Output Enable
 014010h 4 (WLAN_)GPIO_ENABLE_W1TS ; (direct, and Write-1-To-Set/Clr)
 014014h 4 (WLAN_)GPIO_ENABLE_W1TC ;/
 014018h 4 (WLAN_)GPIO_IN ;-GPIO Input
 01401Ch 4 (WLAN_)GPIO_STATUS ;\GPIO Interrupt Status
 014020h 4 (WLAN_)GPIO_STATUS_W1TS ; (direct, and Write-1-To-Set/Clr)
 014024h 4 (WLAN_)GPIO_STATUS_W1TC ;/
 014028h 4 (WLAN_)GPIO_PIN0 ;GPIO0 Bluetooth coex BT_PRIORITY
 01402Ch 4 (WLAN_)GPIO_PIN1 ;GPIO1 Bluetooth coex WLAN_ACTIVE
 014030h 4 (WLAN_)GPIO_PIN2 ;GPIO2 Bluetooth coex BT_FREQ ;I2C SCL
 014034h 4 (WLAN_)GPIO_PIN3 ;GPIO3 Bluetooth coex BT_ACTIVE ;I2C SDA
 014038h 4 (WLAN_)GPIO_PIN4 ;GPIO4 SDIO/GSPI interface select
 01403Ch 4 (WLAN_)GPIO_PIN5 ;GPIO5 SDIO/GSPI interface select
 014040h 4 (WLAN_)GPIO_PIN6 ;GPIO6 -
 014044h 4 (WLAN_)GPIO_PIN7 ;GPIO7 TRST for JTAG debug
 014048h 4 (WLAN_)GPIO_PIN8 ;GPIO8 external 32kHz clock in
 01404Ch 4 (WLAN_)GPIO_PIN9 ;GPIO9 I2C SCL or SPI CLK
 014050h 4 (WLAN_)GPIO_PIN10 ;GPIO10 I2C SDA or SPI MISO
 014054h 4 (WLAN_)GPIO_PIN11 ;GPIO11 UART RXD or SPI MOSI
 014058h 4 (WLAN_)GPIO_PIN12 ;GPIO12 UART TXD or SPI /CS
 01405Ch 4 (WLAN_)GPIO_PIN13 ;GPIO13 Reset in for JTAG debug
 014060h 4 (WLAN_)GPIO_PIN14 ;GPIO14 UART CTS
 014064h 4 (WLAN_)GPIO_PIN15 ;GPIO15 UART RTS
 014068h 4 (WLAN_)GPIO_PIN16 ;GPIO16 -
 01406Ch 4 (WLAN_)GPIO_PIN17 ;GPIO17 -
 014070h 4 SDIO_PIN - Config: Pad Pull/Strength
 014074h 4 CLK_REQ_PIN - Config: Pad Pull/Strength/AteOeLow
 014078h 4 (WLAN_)SIGMA_DELTA
 01407Ch 4 (WLAN_)DEBUG_CONTROL
 014080h 4 (WLAN_)DEBUG_INPUT_SEL
 014084h 4 (WLAN_)DEBUG_OUT
 014088h 4 LA_CONTROL
 01408Ch 4 LA_CLOCK
 014090h 4 LA_STATUS
 014094h 4 LA_TRIGGER_SAMPLE
 014098h 4 LA_TRIGGER_POSITION
 01409Ch 4 LA_PRE_TRIGGER
 0140A0h 4 LA_POST_TRIGGER
 0140A4h 4 LA_FILTER_CONTROL
 0140A8h 4 LA_FILTER_DATA
 0140ACh 4 LA_FILTER_WILDCARD
 0140B0h 4 LA_TRIGGERA_DATA
 0140B4h 4 LA_TRIGGERA_WILDCARD
 0140B8h 4 LA_TRIGGERB_DATA
 0140BCh 4 LA_TRIGGERB_WILDCARD
 0140C0h 4 LA_TRIGGER
 0140C4h 4 LA_FIFO
 0140C8h 4x2 LA[0..1]
 0140D0h 4 ANT_PIN - Config: Pad Pull/Strength
 0140D4h 4 ANTD_PIN - Config: Pad Pull
 0140D8h 4 GPIO_PIN - Config: Pad Pull/Strength
 0140DCh 4 GPIO_H_PIN - Config: Pad Pull
 0140E0h 4 BT_PIN - Config: Pad Pull/Strength
 0140E4h 4 BT_WLAN_PIN - Config: Pad Pull
 0140E8h 4 SI_UART_PIN - Config: Pad Pull/Strength
 0140ECh 4 CLK32K_PIN - Config: Pad Pull
 0140F0h 4 (WLAN_)RESET_TUPLE_STATUS |
| --- |

**MBOX Registers (mbox_reg.h) (hw2.0)**

```

| 018000h 4x4 (WLAN_)MBOX_FIFO[0..3]
 018010h 4 (WLAN_)MBOX_FIFO_STATUS
 018014h 4 (WLAN_)MBOX_DMA_POLICY
 018018h 4 (WLAN_)MBOX0_DMA_RX_DESCRIPTOR_BASE ;\
 01801Ch 4 (WLAN_)MBOX0_DMA_RX_CONTROL ; MBOX 0
 018020h 4 (WLAN_)MBOX0_DMA_TX_DESCRIPTOR_BASE ;
 018024h 4 (WLAN_)MBOX0_DMA_TX_CONTROL ;/
 018028h 4 (WLAN_)MBOX1_DMA_RX_DESCRIPTOR_BASE ;\
 01802Ch 4 (WLAN_)MBOX1_DMA_RX_CONTROL ; MBOX 1
 018030h 4 (WLAN_)MBOX1_DMA_TX_DESCRIPTOR_BASE ;
 018034h 4 (WLAN_)MBOX1_DMA_TX_CONTROL ;/
 018038h 4 (WLAN_)MBOX2_DMA_RX_DESCRIPTOR_BASE ;\
 01803Ch 4 (WLAN_)MBOX2_DMA_RX_CONTROL ; MBOX 2
 018040h 4 (WLAN_)MBOX2_DMA_TX_DESCRIPTOR_BASE ;
 018044h 4 (WLAN_)MBOX2_DMA_TX_CONTROL ;/
 018048h 4 (WLAN_)MBOX3_DMA_RX_DESCRIPTOR_BASE ;\
 01804Ch 4 (WLAN_)MBOX3_DMA_RX_CONTROL ; MBOX 3
 018050h 4 (WLAN_)MBOX3_DMA_TX_DESCRIPTOR_BASE ;
 018054h 4 (WLAN_)MBOX3_DMA_TX_CONTROL ;/
 018058h 4 (WLAN_)MBOX_INT_STATUS ;\Interrupt
 01805Ch 4 (WLAN_)MBOX_INT_ENABLE ;/
 018060h 4 (WLAN_)INT_HOST
 018064h 1x28 PAD0
 018080h 4x8 (WLAN_)LOCAL_COUNT[0..7]
 0180A0h 4x8 (WLAN_)COUNT_INC[0..7]
 0180C0h 4x8 (WLAN_)LOCAL_SCRATCH[0..7]
 0180E0h 4 (WLAN_)USE_LOCAL_BUS
 0180E4h 4 (WLAN_)SDIO_CONFIG
 0180E8h 4 (WLAN_)MBOX_DEBUG
 0180ECh 4 (WLAN_)MBOX_FIFO_RESET
 0180F0h 4x4 (WLAN_)MBOX_TXFIFO_POP[0..3]
 018100h 4x4 (WLAN_)MBOX_RXFIFO_POP[0..3]
 018110h 4 (WLAN_)SDIO_DEBUG
 018114h 1x7916 PAD1
 01A000h 4x2048 (WLAN_)HOST_IF_WINDOW[0..2047] |
| --- |

**Analog Intf Registers (analog_reg.h) (hw2.0)**

```

| 01C000h 4 SYNTH_SYNTH1 ;\
 01C004h 4 SYNTH_SYNTH2 ;
 01C008h 4 SYNTH_SYNTH3 ;
 01C00Ch 4 SYNTH_SYNTH4 ; also defined in "synth_reg.h"
 01C010h 4 SYNTH_SYNTH5 ;
 01C014h 4 SYNTH_SYNTH6 ;
 01C018h 4 SYNTH_SYNTH7 ;
 01C01Ch 4 SYNTH_SYNTH8 ;/
 01C020h 4 RF5G_RF5G1 ;\also defined in "rf5G_reg.h"
 01C024h 4 RF5G_RF5G2 ;/
 01C028h 4 RF2G_RF2G1 ;\also defined in "rf2G_reg.h"
 01C02Ch 4 RF2G_RF2G2 ;/
 01C030h 4 TOP_GAIN ;\also defined in "top_reg.h"
 01C034h 4 TOP_TOP ;/
 01C038h 4 BIAS_BIAS_SEL ;\
 01C03Ch 4 BIAS_BIAS1 ; also defined in "bias_reg.h"
 01C040h 4 BIAS_BIAS2 ;
 01C044h 4 BIAS_BIAS3 ;/
 01C048h 4 TXPC_TXPC ;\also defined in "txpc_reg.h"
 01C04Ch 4 TXPC_MISC ;/
 01C050h 4 RXTXBB_RXTXBB1 ;\
 01C054h 4 RXTXBB_RXTXBB2 ; also defined in "rxtxbb_reg.h"
 01C058h 4 RXTXBB_RXTXBB3 ;
 01C05Ch 4 RXTXBB_RXTXBB4 ;/
 01C060h 4 ADDAC_ADDAC1 ;-also defined in "addac.h"
 01C064h 1x1Ch - |
| --- |

**More Analog Intf Registers (analog_intf_reg.h) (hw2.0)**

```

| 01C080h 4 SW_OVERRIDE ;\
 01C084h 4 SIN_VAL ; defined ONLY in "analog_intf_reg.h"
 01C088h 4 SW_SCLK ;
 01C08Ch 4 SW_CNTL ;/ |
| --- |

**MAC PCU Registers (mac_pcu.h) (hw2.0)**

```

| 028000h (00h) - REG_STA_ID0 ;aka MAC_PCU_STA_ADDR_L32
 028004h (01h) - REG_STA_ID1 ;aka MAC_PCU_STA_ADDR_U16
 028008h (02h) - REG_BSS_ID0 ;aka MAC_PCU_BSSID_L32
 02800Ch (03h) - REG_BSS_ID1 ;aka MAC_PCU_BSSID_U16
 028010h (04h) - MAC_PCU_REG_BCNRSSI ;aka MAC_PCU_BCN_RSSI_AVE
 028014h (05h) - REG_TIME_OUT ;aka MAC_PCU_ACK_CTS_TIMEOUT
 028018h (06h) - MAC_PCU_REG_BCNSIG ;aka MAC_PCU_BCN_RSSI_CTL
 02801Ch (07h) - REG_USEC ;aka MAC_PCU_USEC_LATENCY
 028020h (08h) - REG_BEACON
 028024h (09h) - REG_CFP_PERIOD ;aka (MAC_???)PCU_MAX_CFP_DUR (?)
 028028h (0Ah) - REG_TIMER0
 02802Ch (0Bh) - REG_TIMER1
 028030h (0Ch) - REG_TIMER2
 028034h (0Dh) - REG_TIMER3
 028038h (0Eh) - REG_CFP_DUR ;aka (MAC_???)PCU_MAX_CFP_DUR (?)
 02803Ch (0Fh) - REG_RX_FILTER ;aka MAC_PCU_RX_FILTER
 028040h (10h) - REG_MCAST_FIL0 ;aka MAC_PCU_MCAST_FILTER_L32
 028044h (11h) - REG_MCAST_FIL1 ;aka MAC_PCU_MCAST_FILTER_U32
 028048h (12h) - MAC_PCU_REG_DIAGSW ;aka MAC_PCU_DIAG_SW
 02804Ch (13h) - REG_TSF_L32
 028050h (14h) - REG_TSF_U32
 028054h (15h) - REG_TST_ADDAC ;aka MAC_PCU_TST_ADDAC
 028058h (16h) - REG_DEF_ANT ;aka MAC_PCU_DEF_ANTENNA
 02805Ch (17h) - MAC_PCU_REG_MUTE_MASKS0 ;aka MAC_PCU_AES_MUTE_MASK_0
 028060h (18h) - MAC_PCU_REG_MUTE_MASKS1 ;aka MAC_PCU_AES_MUTE_MASK_1
 028064h (19h) - MAC_PCU_REG_GATED_CLKS ;aka MAC_PCU_GATED_CLKS
 028068h (1Ah) - MAC_PCU_REG_OBS2 ;aka MAC_PCU_OBS_BUS_2
 02806Ch (1Bh) - MAC_PCU_REG_OBS1 ;aka MAC_PCU_OBS_BUS_1
 028070h (1Ch..1Fh) - N/A
 028080h (20h) - REG_LAST_TSTP ;aka MAC_PCU_LAST_BEACON_TSF (?)
 028084h (21h) - REG_NAV ;aka MAC_PCU_NAV
 028088h (22h) - REG_RTS_OK ;aka MAC_PCU_RTS_SUCCESS_CNT
 02808Ch (23h) - REG_RTS_FAIL ;aka MAC_PCU_RTS_FAIL_CNT
 028090h (24h) - REG_ACK_FAIL ;aka MAC_PCU_ACK_FAIL_CNT
 028094h (25h) - REG_FCS_FAIL ;aka MAC_PCU_FCS_FAIL_CNT
 028098h (26h) - REG_BEACON_CNT ;aka MAC_PCU_BEACON_CNT
 02809Ch (27h..2Fh) - N/A
 0280C0h (30h) - MAC_PCU_REG_XRMODE ;aka MAC_PCU_XRMODE
 0280C4h (31h) - MAC_PCU_REG_XRDEL ;aka MAC_PCU_XRDEL
 0280C8h (32h) - MAC_PCU_REG_XRTO ;aka MAC_PCU_XRTO
 0280CCh (33h) - MAC_PCU_REG_XRCRP ;aka MAC_PCU_XRCRP
 0280D0h (34h) - MAC_PCU_REG_XRSTMP ;aka MAC_PCU_XRSTMP
 0280D4h (35h) - MAC_PCU_REG_SLP1 ;aka MAC_PCU_SLP1 ;\moved to
 0280D8h (36h) - MAC_PCU_REG_SLP2 ;aka MAC_PCU_SLP2 ; 004xxxh/005xxxh
 0280DCh (37h) - (//MAC_PCU_REG_SLP3) ;aka MAC_PCU_SLP3 ;/in hw4/hw6 (!)
 0280E0h (38h) - MAC_PCU_REG_BSSMSKL ;aka MAC_PCU_ADDR1_MASK_L32
 0280E4h (39h) - MAC_PCU_REG_BSSMSKH ;aka MAC_PCU_ADDR1_MASK_U16
 0280E8h (3Ah) - MAC_PCU_REG_TPC ;aka MAC_PCU_TPC
 0280ECh (3Bh) - MAC_PCU_REG_TFC ;aka MAC_PCU_TX_FRAME_CNT
 0280F0h (3Ch) - MAC_PCU_REG_RFC ;aka MAC_PCU_RX_FRAME_CNT
 0280F4h (3Dh) - MAC_PCU_REG_RCC ;aka MAC_PCU_RX_CLEAR_CNT
 0280F8h (3Eh) - MAC_PCU_REG_CC ;aka MAC_PCU_CYCLE_CNT
 0280FCh (3Fh) - MAC_PCU_REG_QT1 ;aka MAC_PCU_QUIET_TIME_1
 028100h (40h) - MAC_PCU_REG_QT2 ;aka MAC_PCU_QUIET_TIME_2
 028104h (41h) - MAC_PCU_REG_TSF
 028108h (42h) - MAC_PCU_REG_NOACK ;aka MAC_PCU_QOS_NO_ACK
 02810Ch (43h) - MAC_PCU_REG_PHYERR ;aka MAC_PCU_PHY_ERROR_MASK
 028110h (44h) - MAC_PCU_REG_XRLAT ;aka MAC_PCU_XRLAT
 028114h (45h) - MAC_PCU_REG_ACKSIFS_RESERVED
 028118h (46h) - MAC_PCU_REG_MICQOSCTL ;aka MAC_PCU_MIC_QOS_CONTROL
 02811Ch (47h) - MAC_PCU_REG_MICQOSSEL ;aka MAC_PCU_MIC_QOS_SELECT
 028120h (48h) - MAC_PCU_REG_MISCMODE ;aka MAC_PCU_MISC_MODE
 028124h (49h) - MAC_PCU_REG_FILTOFDM ;aka MAC_PCU_FILTER_OFDM_CNT
 028128h (4Ah) - MAC_PCU_REG_FILTCCK ;aka MAC_PCU_FILTER_CCK_CNT
 02812Ch (4Bh) - MAC_PCU_REG_PHYCNT1 ;aka MAC_PCU_PHY_ERR_CNT_1
 028130h (4Ch) - MAC_PCU_REG_PHYCNTMASK1 ;aka MAC_PCU_PHY_ERR_CNT_1_MASK
 028134h (4Dh) - MAC_PCU_REG_PHYCNT2 ;aka MAC_PCU_PHY_ERR_CNT_2
 028138h (4Eh) - MAC_PCU_REG_PHYCNTMASK2 ;aka MAC_PCU_PHY_ERR_CNT_2_MASK
 02813Ch (4Fh) - MAC_PCU_REG_TSFTHRESH ;aka MAC_PCU_TSF_THRESHOLD
 028140h (50h) - outcommented:MAC_PCU_REG_TSFCAL ;Misc
 028144h (51h) - MAC_PCU_REG_PHYERR_EIFS ;aka MAC_PCU_PHY_ERROR_EIFS_MASK
 028148h (52h) - outcommented:MAC_PCU_REG_SYNC1 ;Time
 02814Ch (53h) - outcommented:MAC_PCU_REG_SYNC2 ;Misc
 028150h (54h) - outcommented:MAC_PCU_REG_SYNC3 ;MCAST Addr_L
 028154h (55h) - outcommented:MAC_PCU_REG_SYNC4 ;MCAST Addr_U
 028158h (56h) - outcommented:MAC_PCU_REG_SYNC5 ;RX Time
 02815Ch (57h) - outcommented:MAC_PCU_REG_SYNC6 ;INC
 028160h (58h) - outcommented:MAC_PCU_REG_SYNC7 ;Last Time
 028164h (59h) - outcommented:MAC_PCU_REG_SYNC8 ;Updated Time
 028168h (5Ah) - MAC_PCU_REG_PHYCNT3 ;aka MAC_PCU_PHY_ERR_CNT_3
 02816Ch (5Bh) - MAC_PCU_REG_PHYCNTMASK3 ;aka MAC_PCU_PHY_ERR_CNT_3_MASK
 028170h (5Ch) - MAC_PCU_REG_BTMODE ;aka MAC_PCU_BLUETOOTH_MODE
 028174h (5Dh) - MAC_PCU_REG_BTWEIGHT ;aka MAC_PCU_BLUETOOTH_WEIGHTS
 028178h (5Eh) - MAC_PCU_REG_HCF ;aka MAC_PCU_HCF_TIMEOUT
 02817Ch (5Fh) - MAC_PCU_REG_BTMODE2 ;aka MAC_PCU_BLUETOOTH_MODE2
 028180h (60h..67h) - MAC_PCU_REG_BFCOEF1[0..7]
 0281A0h (68h..6Fh) - N/A
 0281C0h (70h) - MAC_PCU_REG_BFCOEF2
 0281C4h (71h) - MAC_PCU_REG_KCMASK
 0281C8h (72h..73h) - N/A
 0281D0h (74h) - MAC_PCU_REG_TXSIFS ;aka MAC_PCU_TXSIFS
 0281D4h (75h..7Ah) - N/A
 0281ECh (7Bh) - MAC_PCU_REG_TXOP_X ;aka MAC_PCU_TXOP_X
 0281F0h (7Ch) - MAC_PCU_REG_TXOP_0_3 ;aka MAC_PCU_TXOP_0_3
 0281F4h (7Dh) - MAC_PCU_REG_TXOP_4_7 ;aka MAC_PCU_TXOP_4_7
 0281F8h (7Eh) - MAC_PCU_REG_TXOP_8_11 ;aka MAC_PCU_TXOP_8_11
 0281FCh (7Fh) - MAC_PCU_REG_TXOP_12_15 ;aka MAC_PCU_TXOP_12_15
 028200h (80h..87h) - MAC_PCU_REG_GNRCTMR_N[0..7] ;aka GENERIC_TIMERSxxx?
 028220h (88h..8Fh) - MAC_PCU_REG_GNRCTMR_P[0..7] ;aka GENERIC_TIMERSxxx?
 028240h (90h) - MAC_PCU_REG_GNRCTMR_M ;aka MAC_PCU_GENERIC_TIMERS_MODE
 028244h (91h) - MAC_PCU_REG_SLP32_MODE
 028248h (92h) - MAC_PCU_REG_SLP32_WAKE
 02824Ch (93h) - MAC_PCU_REG_SLP32_TSF_INC
 028250h (94h) - MAC_PCU_REG_SLPMIB1
 028254h (95h) - MAC_PCU_REG_SLPMIB2
 028258h (96h) - MAC_PCU_REG_SLPMIB3
 02825Ch (97h) - MAC_PCU_REG_MISCMODE2 ;aka MAC_PCU_MISC_MODE2
 028260h (98h) - MAC_PCU_REG_SLP4
 028264h (99h) - MAC_PCU_REG_SLP5
 028268h (9Ah) - MAC_PCU_REG_MCICTL ;\
 02826Ch (9Bh) - MAC_PCU_REG_MCIISR ;
 028270h (9Ch) - MAC_PCU_REG_MCIIER ;
 028274h (9Dh) - MAC_PCU_REG_MCIWLP ;
 028278h (9Eh) - MAC_PCU_REG_MCIARW ;
 02827Ch (9Fh) - MAC_PCU_REG_MCIARR ; whatever MCI stuff
 028280h (A0h) - MAC_PCU_REG_MCIADW ;
 028284h (A1h) - MAC_PCU_REG_MCIADR ;
 028288h (A2h) - MAC_PCU_REG_MCIFRW ;
 02828Ch (A3h) - MAC_PCU_REG_MCIFRR ;
 028290h (A4h) - MAC_PCU_REG_MCIQRW ;
 028294h (A5h) - MAC_PCU_REG_MCIQRR ;
 028298h (A6h) - MAC_PCU_REG_MCIGRW ;
 02829Ch (A7h) - MAC_PCU_REG_MCIGRR ;
 0282A0h (A8h) - MAC_PCU_REG_MCISTAT ;/
 0282A4h (A9h) - MAC_PCU_REG_BASIC_RATE_SET0 ;aka MAC_PCU_BASIC_RATE_SET0
 0282A8h (AAh) - MAC_PCU_REG_BASIC_RATE_SET1 ;aka MAC_PCU_BASIC_RATE_SET1
 0282ACh (ABh) - MAC_PCU_REG_BASIC_RATE_SET2 ;aka MAC_PCU_BASIC_RATE_SET2
 0282B0h (ACh) - MAC_PCU_REG_SEC_BSSID_L32 ;aka MAC_PCU_BSSID2_L32
 0282B4h (ADh) - MAC_PCU_REG_SEC_BSSID_U16 ;aka MAC_PCU_BSSID2_U16
 0282B8h (AEh..13Fh) - N/A
 028500h (140h..17Fh) - MAC_PCU_REG_FTYPE[0..3Fh]
 028600h (180h..19Fh) - N/A
 028680h (1A0h..1BFh) - MAC_PCU_REG_ACKSIFSMEM_RESERVED[0..1Fh]
 028700h (1C0h..1DFh) - MAC_PCU_REG_DUR[0..1Fh]
 028780h (1E0h..1EFh) - N/A
 0287C0h (1F0h..1F7h) - MAC_PCU_REG_RTD[0..7]
 0287E0h (1F8h..1FFh) - MAC_PCU_REG_DTR[0..7]
 028800h (200h..5FFh) - MAC_PCU_REG_KC[0..3FFh] ;aka KC = KEY_CACHE ? |
| --- |

**Undocumented hw2 registers**

```

| 029800h ? Used in hw2, but undocumented... maybe BASEBAND_0 alike hw4?
 02A000h ? Used in hw2, but undocumented... maybe BASEBAND_1 alike hw4?
 030000h ? Used in hw2, but undocumented... maybe mode switch? |
| --- |

Note: 030000h seems to be used only in AR6013/AR6014 (not AR6002), maybe RDMA
or EFUSE alike hw4, or maybe for atheros/mitsumi mode switch.

| DSi Atheros Wifi - Internal I/O Map Summary (hw4) |
| --- |

**Overall Summary (hw4.0)**

```

| 004000h 2E8h (rtc_wlan_reg.h)
 008000h 630h Memory Controller (BCAM) (vmc_wlan_reg.h)
 00C000h 14h (uart_reg.h)
 00D000h .. DBG_UART_BASE_ADDRESS ;another UART, as above, for debug?
 00E000h 38h (umbox_wlan_reg.h)
 010000h 18h (si_reg.h)
 014000h BCh (gpio_athr_wlan_reg.h)
 018000h 12Ch (mbox_wlan_reg.h)
 01A000h 20000h WLAN_HOST_IF_WINDOW (mbox_wlan_reg.h)
 01C000h 748h (analog_intf_athr_wlan_reg.h)
 020000h DCh WMAC DMA and IRQ (mac_dma_reg.h)
 020800h 244h WMAC QCU Queue (mac_dma_reg.h)
 021000h 274h WMAC DCU (mac_dma_reg.h)
 028000h C00h MAC_PCU (mac_pcu_reg.h)
 029800h 800h MAC_PCU_BASEBAND_0 (bb_lc_reg.h)
 02A000h 1210h MAC_PCU_BASEBAND_1 (bb_lc_reg.h)
 02C000h 1000h MAC_PCU_BASEBAND_2 (mac_pcu_reg.h)
 02D000h 1000h MAC_PCU_BASEBAND_3 (mac_pcu_reg.h)
 02E000h 800h MAC_PCU_BUF (mac_pcu_reg.h)
 030100h 68h (rdma_reg.h)
 031000h 1000h (efuse_reg.h) |
| --- |

**rtc_wlan_reg.h (hw4.0)**

```

| 004000h 4 WLAN_RESET_CONTROL
 004004h 4 WLAN_XTAL_CONTROL
 004008h 4 WLAN_TCXO_DETECT
 00400Ch 4 WLAN_XTAL_TEST
 004010h 4 WLAN_QUADRATURE
 004014h 4 WLAN_PLL_CONTROL
 004018h 4 WLAN_PLL_SETTLE
 00401Ch 4 WLAN_XTAL_SETTLE
 004020h 4 WLAN_CPU_CLOCK
 004024h 4 WLAN_CLOCK_OUT
 004028h 4 WLAN_CLOCK_CONTROL
 00402Ch 4 WLAN_BIAS_OVERRIDE
 004030h 4 WLAN_WDT_CONTROL ;\
 004034h 4 WLAN_WDT_STATUS ;
 004038h 4 WLAN_WDT ; Watchdog Timer
 00403Ch 4 WLAN_WDT_COUNT ;
 004040h 4 WLAN_WDT_RESET ;/
 004044h 4 WLAN_INT_STATUS ;-Interrupt Status
 004048h 4 WLAN_LF_TIMER0 ;\
 00404Ch 4 WLAN_LF_TIMER_COUNT0 ; Low-Freq Timer 0
 004050h 4 WLAN_LF_TIMER_CONTROL0 ;
 004054h 4 WLAN_LF_TIMER_STATUS0 ;/
 004058h 4 WLAN_LF_TIMER1 ;\
 00405Ch 4 WLAN_LF_TIMER_COUNT1 ; Low-Freq Timer 1
 004060h 4 WLAN_LF_TIMER_CONTROL1 ;
 004064h 4 WLAN_LF_TIMER_STATUS1 ;/
 004068h 4 WLAN_LF_TIMER2 ;\
 00406Ch 4 WLAN_LF_TIMER_COUNT2 ; Low-Freq Timer 2
 004070h 4 WLAN_LF_TIMER_CONTROL2 ;
 004074h 4 WLAN_LF_TIMER_STATUS2 ;/
 004078h 4 WLAN_LF_TIMER3 ;\
 00407Ch 4 WLAN_LF_TIMER_COUNT3 ; Low-Freq Timer 3
 004080h 4 WLAN_LF_TIMER_CONTROL3 ;
 004084h 4 WLAN_LF_TIMER_STATUS3 ;/
 004088h 4 WLAN_HF_TIMER ;\
 00408Ch 4 WLAN_HF_TIMER_COUNT ; High-Freq Timer
 004090h 4 WLAN_HF_LF_COUNT ;<-- ;
 004094h 4 WLAN_HF_TIMER_CONTROL ;
 004098h 4 WLAN_HF_TIMER_STATUS ;/
 00409Ch 4 WLAN_RTC_CONTROL ;\
 0040A0h 4 WLAN_RTC_TIME ;
 0040A4h 4 WLAN_RTC_DATE ;
 0040A8h 4 WLAN_RTC_SET_TIME ; Real-Time Clock
 0040ACh 4 WLAN_RTC_SET_DATE ;
 0040B0h 4 WLAN_RTC_SET_ALARM ;
 0040B4h 4 WLAN_RTC_CONFIG ;
 0040B8h 4 WLAN_RTC_ALARM_STATUS ;/
 0040BCh 4 WLAN_UART_WAKEUP
 0040C0h 4 WLAN_RESET_CAUSE
 0040C4h 4 WLAN_SYSTEM_SLEEP
 0040C8h 4 WLAN_SDIO_WRAPPER
 0040CCh 4 WLAN_MAC_SLEEP_CONTROL
 0040D0h 4 WLAN_KEEP_AWAKE
 0040D4h 4 WLAN_LPO_CAL_TIME ;\
 0040D8h 4 WLAN_LPO_INIT_DIVIDEND_INT ;
 0040DCh 4 WLAN_LPO_INIT_DIVIDEND_FRACTION ; LPO
 0040E0h 4 WLAN_LPO_CAL ;
 0040E4h 4 WLAN_LPO_CAL_TEST_CONTROL ;
 0040E8h 4 WLAN_LPO_CAL_TEST_STATUS ;/
 0040ECh 4 WLAN_CHIP_ID ;-Chip ID
 0040F0h 4 WLAN_DERIVED_RTC_CLK
 0040F4h 4 MAC_PCU_SLP32_MODE ;\
 0040F8h 4 MAC_PCU_SLP32_WAKE ;
 0040FCh 4 MAC_PCU_SLP32_INC ;
 004100h 4 MAC_PCU_SLP_MIB1 ;
 004104h 4 MAC_PCU_SLP_MIB2 ;
 004108h 4 MAC_PCU_SLP_MIB3 ;/
 00410Ch 4 WLAN_POWER_REG ;\located here in hw4.0
 004110h 4 WLAN_CORE_CLK_CTRL ; (other address as in hw2.0)
 004114h 4 WLAN_GPIO_WAKEUP_CONTROL ;/
 (below 4118h..42E8h is new in hw4.0, didn't exist in hw2.0)
 004118h 4 (WLAN_)HT
 00411Ch 4 MAC_PCU_TSF_L32
 004120h 4 MAC_PCU_TSF_U32
 004124h 4 MAC_PCU_WBTIMER
 004128h 1x24 PAD0
 004140h 4x16 MAC_PCU_GENERIC_TIMERS[0..15]
 004180h 4 MAC_PCU_GENERIC_TIMERS_MODE
 004184h 1x60 PAD1
 0041C0h 4x16 MAC_PCU_GENERIC_TIMERS2[0..15]
 004200h 4 MAC_PCU_GENERIC_TIMERS_MODE2
 004204h 4 MAC_PCU_SLP1
 004208h 4 MAC_PCU_SLP2
 00420Ch 4 MAC_PCU_RESET_TSF
 004210h 4 MAC_PCU_TSF_ADD_PLL
 004214h 4 SLEEP_RETENTION
 004218h 4 BTCOEXCTRL ;\
 00421Ch 4 WBSYNC_PRIORITY1 ;
 004220h 4 WBSYNC_PRIORITY2 ;
 004224h 4 WBSYNC_PRIORITY3 ;
 004228h 4 BTCOEX0 ;SYNC_DUR ;
 00422Ch 4 BTCOEX1 ;CLK_THRES ;
 004230h 4 BTCOEX2 ;FRAME_THRES ; Bluetooth
 004234h 4 BTCOEX3 ;CLK_CNT ; Coexistance
 004238h 4 BTCOEX4 ;FRAME_CNT ;
 00423Ch 4 BTCOEX5 ;IDLE_CNT ;
 004240h 4 BTCOEX6 ;IDLE_RESET_LVL_BITMAP ;
 004244h 4 LOCK ;
 004248h 4 NOLOCK_PRIORITY ;
 00424Ch 4 WBSYNC ;
 004250h 4 WBSYNC1 ;
 004254h 4 WBSYNC2 ;
 004258h 4 WBSYNC3 ;
 00425Ch 4 WB_TIMER_TARGET ;
 004260h 4 WB_TIMER_SLOP ;
 004264h 4 BTCOEX_INT_EN ;
 004268h 4 BTCOEX_INT_STAT ;
 00426Ch 4 BTPRIORITY_INT_EN ;
 004270h 4 BTPRIORITY_INT_STAT ;
 004274h 4 BTPRIORITY_STOMP_INT_EN ;
 004278h 4 BTPRIORITY_STOMP_INT_STAT ;/
 00427Ch 4 MAC_PCU_BMISS_TIMEOUT
 004280h 4 MAC_PCU_CAB_AWAKE
 004284h 4 LP_PERF_COUNTER
 004288h 4 LP_PERF_LIGHT_SLEEP
 00428Ch 4 LP_PERF_DEEP_SLEEP
 004290h 4 LP_PERF_ON
 004294h 4 ST_64_BIT ;\
 004298h 4 MESSAGE_WR ; also Bluetooth Coex
 00429Ch 4 MESSAGE_WR_P ; related? (sorted as
 0042A0h 4 MESSAGE_RD ; so in hw6 files)
 0042A4h 4 MESSAGE_RD_P ;/
 0042A8h 4 CHIP_MODE
 0042ACh 4 CLK_REQ_FALL_EDGE
 0042B0h 4 OTP
 0042B4h 4 OTP_STATUS
 0042B8h 4 PMU
 0042BCh 1x4 PAD2
 0042C0h 4x2 PMU_CONFIG[0..1]
 0042C8h 4 PMU_BYPASS
 0042CCh 4 MAC_PCU_TSF2_L32
 0042D0h 4 MAC_PCU_TSF2_U32
 0042D4h 4 MAC_PCU_GENERIC_TIMERS_MODE3
 0042D8h 4 MAC_PCU_DIRECT_CONNECT
 0042DCh 4 THERM_CTRL1
 0042E0h 4 THERM_CTRL2
 0042E4h 4 THERM_CTRL3
 0042E8h - unused/unspecified |
| --- |

**vmc_wlan_reg.h (hw4.0)**

```

| 008000h 4x128 WLAN_MC_BCAM_VALID[0..127] ;\
 008200h 4x128 WLAN_MC_BCAM_COMPARE[0..127] ; ROM Patches
 008400h 4x128 WLAN_MC_BCAM_TARGET[0..127] ;/
 008600h 4 WLAN_APB_ADDR_ERROR_CONTROL ;\
 008604h 4 WLAN_APB_ADDR_ERROR_STATUS ; ADDR_ERROR
 008608h 4 WLAN_AHB_ADDR_ERROR_CONTROL ;
 00860Ch 4 WLAN_AHB_ADDR_ERROR_STATUS ;/
 008610h 4 WLAN_BCAM_CONFLICT_ERROR
 008614h 4 WLAN_CPU_PERF_CNT
 008618h 4 WLAN_CPU_INST_FETCH
 00861Ch 4 WLAN_CPU_DATA_FETCH
 008620h 4 WLAN_CPU_RAM1_CONFLICT
 008624h 4 WLAN_CPU_RAM2_CONFLICT
 008628h 4 WLAN_CPU_RAM3_CONFLICT
 00862Ch 4 WLAN_CPU_RAM4_CONFLICT
 008630h - unused/unspecified |
| --- |

**uart_reg.h (hw4.0)**

```

| 00C000h 4 UART_DATA
 00C004h 4 UART_CONTROL
 00C008h 4 UART_CLKDIV
 00C00Ch 4 UART_INT
 00C010h 4 UART_INT_EN
 00C014h - unused/unspecified
 00D000h .. DBG_UART_BASE_ADDRESS ;another UART, as above, for debug?
 00Dxxxh - unused/unspecified |
| --- |

**umbox_wlan_reg.h (hw4.0)**

```

| 00E000h 4x2 UMBOX_FIFO[0..1]
 00E008h 4 UMBOX_FIFO_STATUS
 00E00Ch 4 UMBOX_DMA_POLICY
 00E010h 4 UMBOX0_DMA_RX_DESCRIPTOR_BASE
 00E014h 4 UMBOX0_DMA_RX_CONTROL
 00E018h 4 UMBOX0_DMA_TX_DESCRIPTOR_BASE
 00E01Ch 4 UMBOX0_DMA_TX_CONTROL
 00E020h 4 UMBOX_FIFO_TIMEOUT
 00E024h 4 UMBOX_INT_STATUS
 00E028h 4 UMBOX_INT_ENABLE
 00E02Ch 4 UMBOX_DEBUG
 00E030h 4 UMBOX_FIFO_RESET
 00E034h 4 UMBOX_HCI_FRAMER
 00E038h - unused/unspecified |
| --- |

**si_reg.h (hw4.0)**

```

| 010000h 4 SI_CONFIG
 010004h 4 SI_CS
 010008h 4 SI_TX_DATA0
 01000Ch 4 SI_TX_DATA1
 010010h 4 SI_RX_DATA0
 010014h 4 SI_RX_DATA1
 010018h - unused/unspecified |
| --- |

**gpio_athr_wlan_reg.h (hw4.0)**

```

| 014000h 4 WLAN_GPIO_OUT ;\GPIO Output Data
 014004h 4 WLAN_GPIO_OUT_W1TS ; (direct, and Write-1-To-Set/Clr)
 014008h 4 WLAN_GPIO_OUT_W1TC ;/
 01400Ch 4 WLAN_GPIO_ENABLE ;\GPIO Output Enable
 014010h 4 WLAN_GPIO_ENABLE_W1TS ; (direct, and Write-1-To-Set/Clr)
 014014h 4 WLAN_GPIO_ENABLE_W1TC ;/
 014018h 4 WLAN_GPIO_IN ;-GPIO Input
 01401Ch 4 WLAN_GPIO_STATUS ;\GPIO Interrupt Status
 014020h 4 WLAN_GPIO_STATUS_W1TS ; (direct, and Write-1-To-Set/Clr)
 014024h 4 WLAN_GPIO_STATUS_W1TC ;/
 014028h 4 WLAN_GPIO_PIN0 ;GPIO0 Bluetooth coex BT_FREQUENCY
 01402Ch 4 WLAN_GPIO_PIN1 ;GPIO1 Bluetooth coex WLAN_ACTIVE
 014030h 4 WLAN_GPIO_PIN2 ;GPIO2 Bluetooth coex BT_ACTIVE
 014034h 4 WLAN_GPIO_PIN3 ;GPIO3 Bluetooth coex BT_PRIORITY
 014038h 4 WLAN_GPIO_PIN4 ;GPIO4 -
 01403Ch 4 WLAN_GPIO_PIN5 ;GPIO5 JTAG TMS input
 014040h 4 WLAN_GPIO_PIN6 ;GPIO6 JTAG TCK input
 014044h 4 WLAN_GPIO_PIN7 ;GPIO7 JTAG TDI input
 014048h 4 WLAN_GPIO_PIN8 ;GPIO8 JTAG TDO output
 01404Ch 4 WLAN_GPIO_PIN9 ;GPIO9 SDIO CMD
 014050h 4 WLAN_GPIO_PIN10 ;GPIO10 SDIO D3
 014054h 4 WLAN_GPIO_PIN11 ;GPIO11 SDIO D2
 014058h 4 WLAN_GPIO_PIN12 ;GPIO12 SDIO D1
 01405Ch 4 WLAN_GPIO_PIN13 ;GPIO13 SDIO D0
 014060h 4 WLAN_GPIO_PIN14 ;GPIO14 SDIO CLK
 014064h 4 WLAN_GPIO_PIN15 ;GPIO15 HCI UART TXD
 014068h 4 WLAN_GPIO_PIN16 ;GPIO16 HCI UART RTS
 01406Ch 4 WLAN_GPIO_PIN17 ;GPIO17 HCI UART RXD
 014070h 4 WLAN_GPIO_PIN18 ;GPIO18 HCI UART CTS
 014074h 4 WLAN_GPIO_PIN19 ;GPIO19 SDIO/GSPI interface select
 014078h 4 WLAN_GPIO_PIN20 ;GPIO20 SDIO/GSPI interface select
 01407Ch 4 WLAN_GPIO_PIN21 ;GPIO21 external input sleep clock
 014080h 4 WLAN_GPIO_PIN22 ;GPIO22 wake on wireless input (WOW)
 014084h 4 WLAN_GPIO_PIN23 ;GPIO23 reference clk output to BT chip
 014088h 4 WLAN_GPIO_PIN24 ;GPIO24 request clk from BT chip
 01408Ch 4 WLAN_GPIO_PIN25 ;GPIO25 request reference clk (CLK_REQ)
 014090h 4 SDIO
 014094h 4 FUNC_BUS
 014098h 4 WL_SOC_APB
 01409Ch 4 WLAN_SIGMA_DELTA
 0140A0h 4 WL_BOOTSTRAP
 0140A4h 4 CLOCK_GPIO
 0140A8h 4 WLAN_DEBUG_CONTROL
 0140ACh 4 WLAN_DEBUG_INPUT_SEL
 0140B0h 4 WLAN_DEBUG_OUT
 0140B4h 4 WLAN_RESET_TUPLE_STATUS
 0140B8h 4 ANTENNA_SLEEP_CONTROL
 0140BCh - unused/unspecified |
| --- |

**MBOX Registers (mbox_wlan_reg.h) (hw4.0)**

```

| 018000h 4x4 WLAN_MBOX_FIFO[0..3]
 018010h 4 WLAN_MBOX_FIFO_STATUS
 018014h 4 WLAN_MBOX_DMA_POLICY
 018018h 4 WLAN_MBOX0_DMA_RX_DESCRIPTOR_BASE ;\
 01801Ch 4 WLAN_MBOX0_DMA_RX_CONTROL ; MBOX 0
 018020h 4 WLAN_MBOX0_DMA_TX_DESCRIPTOR_BASE ;
 018024h 4 WLAN_MBOX0_DMA_TX_CONTROL ;/
 018028h 4 WLAN_MBOX1_DMA_RX_DESCRIPTOR_BASE ;\
 01802Ch 4 WLAN_MBOX1_DMA_RX_CONTROL ; MBOX 1
 018030h 4 WLAN_MBOX1_DMA_TX_DESCRIPTOR_BASE ;
 018034h 4 WLAN_MBOX1_DMA_TX_CONTROL ;/
 018038h 4 WLAN_MBOX2_DMA_RX_DESCRIPTOR_BASE ;\
 01803Ch 4 WLAN_MBOX2_DMA_RX_CONTROL ; MBOX 2
 018040h 4 WLAN_MBOX2_DMA_TX_DESCRIPTOR_BASE ;
 018044h 4 WLAN_MBOX2_DMA_TX_CONTROL ;/
 018048h 4 WLAN_MBOX3_DMA_RX_DESCRIPTOR_BASE ;\
 01804Ch 4 WLAN_MBOX3_DMA_RX_CONTROL ; MBOX 3
 018050h 4 WLAN_MBOX3_DMA_TX_DESCRIPTOR_BASE ;
 018054h 4 WLAN_MBOX3_DMA_TX_CONTROL ;/
 018058h 4 WLAN_MBOX_INT_STATUS ;\Interrupt
 01805Ch 4 WLAN_MBOX_INT_ENABLE ;/
 018060h 4 WLAN_INT_HOST ;IRQ to sdio/host
 018064h 1x28 PAD0
 018080h 4x8 WLAN_LOCAL_COUNT[0..7] ;SDIO func1 ?
 0180A0h 4x8 WLAN_COUNT_INC[0..7] ;SDIO func1 ?
 0180C0h 4x8 WLAN_LOCAL_SCRATCH[0..7] ;SDIO func1 ?
 0180E0h 4 WLAN_USE_LOCAL_BUS
 0180E4h 4 WLAN_SDIO_CONFIG ;SDIO func0 ?
 0180E8h 4 WLAN_MBOX_DEBUG
 0180ECh 4 WLAN_MBOX_FIFO_RESET
 0180F0h 4x4 WLAN_MBOX_TXFIFO_POP[0..3]
 018100h 4x4 WLAN_MBOX_RXFIFO_POP[0..3]
 018110h 4 WLAN_SDIO_DEBUG
 018114h 4 WLAN_GMBOX0_DMA_RX_DESCRIPTOR_BASE ;\
 018118h 4 WLAN_GMBOX0_DMA_RX_CONTROL ;
 01811Ch 4 WLAN_GMBOX0_DMA_TX_DESCRIPTOR_BASE ; new (unlike hw2.0)
 018120h 4 WLAN_GMBOX0_DMA_TX_CONTROL ;
 018124h 4 WLAN_GMBOX_INT_STATUS ;
 018128h 4 WLAN_GMBOX_INT_ENABLE ;/
 01812Ch 1x7892 PAD1
 01A000h 4x2048 WLAN_HOST_IF_WINDOW[0..2047] |
| --- |

**analog_intf_athr_wlan_reg.h (hw4.0)**

```

| 01C000h 4 RXRF_BIAS1
 01C004h 4 RXRF_BIAS2
 01C008h 4 RXRF_GAINSTAGES
 01C00Ch 4 RXRF_AGC
 01C010h 1x48 PAD__0
 01C040h 4 TXRF1
 01C044h 4 TXRF2
 01C048h 4 TXRF3
 01C04Ch 4 TXRF4
 01C050h 4 TXRF5
 01C054h 4 TXRF6
 01C058h 4 TXRF7 ;PADRVGNTAB_0..4 ;\
 01C05Ch 4 TXRF8 ;PADRVGNTAB_5..9 ;
 01C060h 4 TXRF9 ;PADRVGNTAB_10..14 ;/
 01C064h 4 TXRF10
 01C068h 4 TXRF11
 01C06Ch 4 TXRF12
 01C070h 1x16 PAD__1
 01C080h 4 SYNTH1
 01C084h 4 SYNTH2
 01C088h 4 SYNTH3
 01C08Ch 4 SYNTH4
 01C090h 4 SYNTH5
 01C094h 4 SYNTH6
 01C098h 4 SYNTH7
 01C09Ch 4 SYNTH8
 01C0A0h 4 SYNTH9
 01C0A4h 4 SYNTH10
 01C0A8h 4 SYNTH11
 01C0ACh 4 SYNTH12
 01C0B0h 4 SYNTH13
 01C0B4h 4 SYNTH14
 01C0B8h 1x8 PAD__2
 01C0C0h 4 BIAS1
 01C0C4h 4 BIAS2
 01C0C8h 4 BIAS3
 01C0CCh 4 BIAS4
 01C0D0h 1x48 PAD__3
 01C100h 4 RXTX1
 01C104h 4 RXTX2
 01C108h 4 RXTX3
 01C10Ch 1x52 PAD__4
 01C140h 4 BB1
 01C144h 4 BB2
 01C148h 4 BB3
 01C14Ch 1x308 PAD__5
 01C280h 4 PLLCLKMODA
 01C284h 4 PLLCLKMODA2
 01C288h 4 TOP
 01C28Ch 4 THERM
 01C290h 4 XTAL
 01C294h 1x236 PAD__6
 01C380h 4 RBIST_CNTRL ;with extra bit in newer revision
 01C384h 4 TX_DC_OFFSET
 01C388h 4 TX_TONEGEN0
 01C38Ch 4 TX_TONEGEN1
 01C390h 4 TX_LFTONEGEN0
 01C394h 4 TX_LINEAR_RAMP_I
 01C398h 4 TX_LINEAR_RAMP_Q
 01C39Ch 4 TX_PRBS_MAG
 01C3A0h 4 TX_PRBS_SEED_I
 01C3A4h 4 TX_PRBS_SEED_Q
 01C3A8h 4 CMAC_DC_CANCEL
 01C3ACh 4 CMAC_DC_OFFSET
 01C3B0h 4 CMAC_CORR
 01C3B4h 4 CMAC_POWER
 01C3B8h 4 CMAC_CROSS_CORR
 01C3BCh 4 CMAC_I2Q2
 01C3C0h 4 CMAC_POWER_HPF
 01C3C4h 4 RXDAC_SET1
 01C3C8h 4 RXDAC_SET2
 01C3CCh 4 RXDAC_LONG_SHIFT
 01C3D0h 4 CMAC_RESULTS_I
 01C3D4h 4 CMAC_RESULTS_Q
 01C3D8h 1x872 PAD__7
 01C740h 4 PMU1
 01C744h 4 PMU2
 01C748h - unused/unspecified |
| --- |

**mac_dma_reg.h (hw4.0)**

```

| 020000h 1x8 -
 020008h 4 MAC_DMA_CR - MAC Control Register
 02000Ch 4 MAC_DMA_RXDP - MAC receive queue descriptor pointer
 020010h 4 -
 020014h 4 MAC_DMA_CFG - MAC configuration and status register
 020018h 4 -
 02001Ch 4 -
 020020h 4 MAC_DMA_MIRT - Maximum rate threshold register
 020024h 4 MAC_DMA_IER - MAC Interrupt enable register
 020028h 4 MAC_DMA_TIMT - Transmit Interrupt Mitigation Threshold
 02002Ch 4 MAC_DMA_RIMT - Receive Interrupt Mitigation Threshold
 020030h 4 MAC_DMA_TXCFG - MAC tx DMA size config register
 020034h 4 MAC_DMA_RXCFG - MAC rx DMA size config register
 020038h 4 -
 02003Ch 4 -
 020040h 4 MAC_DMA_MIBC - MAC MIB control register
 020044h 4 MAC_DMA_TOPS - MAC timeout prescale count
 020048h 4 MAC_DMA_RXNPTO - MAC no frame received timeout
 02004Ch 4 MAC_DMA_TXNPTO - MAC no frame trasmitted timeout
 020050h 4 MAC_DMA_RPGTO - MAC receive frame gap timeout
 020054h 4 MAC_DMA_RPCNT - MAC receive frame count limit
 020058h 4 MAC_DMA_MACMISC - MAC miscellaneous control/status register
 02005Ch .. -
 MAC IRQ...
 020080h 4 MAC_DMA_ISR - Primary Interrupt Status Register ;\
 020084h 4 MAC_DMA_ISR_S0 - Secondary Interrupt 0 Status TX OK/DESC ;
 020088h 4 MAC_DMA_ISR_S1 - Secondary Interrupt 1 Status TX ERR/EOL ;
 02008Ch 4 MAC_DMA_ISR_S2 - Secondary Interrupt 2 Status TX URN/MISC ;
 020090h 4 MAC_DMA_ISR_S3 - Secondary Interrupt 3 Status QCBR OVF/URN ;
 020094h 4 MAC_DMA_ISR_S4 - Secondary Interrupt 4 Status QTRIG ;
 020098h 4 MAC_DMA_ISR_S5 - Secondary Interrupt 5 Status TIMERS ;/
 02009Ch 4 -
 0200A0h 4 MAC_DMA_IMR - Primary Interrupt Mask Register ;\
 0200A4h 4 MAC_DMA_IMR_S0 - Secondary Interrupt 0 Mask TX OK/DESC ;
 0200A8h 4 MAC_DMA_IMR_S1 - Secondary Interrupt 1 Mask TX ERR/EOL ;
 0200ACh 4 MAC_DMA_IMR_S2 - Secondary Interrupt 2 Mask TX URN/MISC ;
 0200B0h 4 MAC_DMA_IMR_S3 - Secondary Interrupt 3 Mask QCBR OVF/URN ;
 0200B4h 4 MAC_DMA_IMR_S4 - Secondary Interrupt 4 Mask QTRIG ;
 0200B8h 4 MAC_DMA_IMR_S5 - Secondary Interrupt 5 Mask TIMERS ;/
 0200BCh 4 -
 0200C0h 4 MAC_DMA_ISR_RAC - Primary Interrupt Read-and-Clear ;\
 0200C4h 4 MAC_DMA_ISR_S0_S - Secondary 0 Read-and-Clear TX OK/DESC ;
 0200C8h 4 MAC_DMA_ISR_S1_S - Secondary 1 Read-and-Clear TX ERR/EOL ;
 0200CCh 4 MAC_DMA_ISR_S2_S - Secondary 2 Read-and-Clear TX URN/MISC ;
 0200D0h 4 MAC_DMA_ISR_S3_S - Secondary 3 Read-and-Clear QCBR OVF/URN ;
 0200D4h 4 MAC_DMA_ISR_S4_S - Secondary 4 Read-and-Clear QTRIG ;
 0200D8h 4 MAC_DMA_ISR_S5_S - Secondary 5 Read-and-Clear TIMERS ;/
 0200DCh .. -
 MAC QCU...
 020800h 4x10 MAC_DMA_Q(0..9)_TXDP ;MAC Transmit Queue descr.ptr
 020828h .. -
 020840h 4 MAC_DMA_Q_TXE ;MAC Transmit Queue enable
 020844h .. -
 020880h 4 MAC_DMA_Q_TXD ;MAC Transmit Queue disable
 020884h .. -
 0208C0h 4x10 MAC_DMA_Q(0..9)_CBRCFG ;MAC CBR configuration
 0208E8h .. -
 020900h 4x10 MAC_DMA_Q(0..9)_RDYTIMECFG ;MAC ReadyTime configuration
 020928h .. -
 020940h 4 MAC_DMA_Q_ONESHOTMAC_DMAM_SC ;MAC OneShotArm set control
 020944h .. -
 020980h 4 MAC_DMA_Q_ONESHOTMAC_DMAM_CC ;MAC OneShotArm clear control
 020984h .. -
 0209C0h 4x10 MAC_DMA_Q(0..9)_MISC ;MAC Misc QCU settings
 0209E8h .. -
 020A00h 4x10 MAC_DMA_Q(0..9)_STS ;MAC Misc QCU status/counter
 020A28h .. -
 020A40h 4 MAC_DMA_Q_RDYTIMESHDN ;MAC ReadyTimeShutdown status
 020A44h .. -
 MAC DCU...
 021000h 4x10 MAC_DMA_D(0..9)_QCUMASK - MAC QCU Mask (DCU-to-QCU or so?)
 021028h 8 -
 021030h 4 MAC_DMA_D_GBL_IFS_SIFS - DCU global SIFS settings
 021034h 12 -
 021040h 4x10 MAC_DMA_D(0..9)_LCL_IFS - MAC DCU-specific IFS settings
 021068h 8 -
 021070h 4 MAC_DMA_D_GBL_IFS_SLOT - DC global slot interval
 021074h 12 -
 021080h 4x10 MAC_DMA_D(0..9)_RETRY_LIMIT - MAC Retry limits
 0210A8h 8 -
 0210B0h 4 MAC_DMA_D_GBL_IFS_EIFS - DCU global EIFS setting
 0210B4h 12 -
 0210C0h 4x10 MAC_DMA_D(0..9)_CHNTIME - MAC ChannelTime settings
 0210E8h 8 -
 0210F0h 4 MAC_DMA_D_GBL_IFS_MISC - DCU global misc. IFS settings
 0210F4h 12 -
 021100h 4x10 MAC_DMA_D(0..9)_MISC - MAC Misc DCU-specific settings
 021128h .. -
 021140h 4 MAC_DMA_D_SEQNUM - MAC Frame sequence number
 021144h .. -
 021180h 4x10 MAC_DMA_D(0..9)_EOL -
 0211A8h .. -
 021230h 4 MAC_DMA_D_FPCTL - DCU frame prefetch settings
 021234h .. -
 021270h 4 MAC_DMA_D_TXPSE - DCU transmit pause control/status
 021274h .. - |
| --- |

**mac_pcu_reg.h (1) (hw4.0)**

```

| 028000h 4 MAC_PCU_STA_ADDR_L32
 028004h 4 MAC_PCU_STA_ADDR_U16
 028008h 4 MAC_PCU_BSSID_L32
 02800Ch 4 MAC_PCU_BSSID_U16
 028010h 4 MAC_PCU_BCN_RSSI_AVE
 028014h 4 MAC_PCU_ACK_CTS_TIMEOUT
 028018h 4 MAC_PCU_BCN_RSSI_CTL
 02801Ch 4 MAC_PCU_USEC_LATENCY
 028020h 4 PCU_MAX_CFP_DUR
 028024h 4 MAC_PCU_RX_FILTER
 028028h 4 MAC_PCU_MCAST_FILTER_L32
 02802Ch 4 MAC_PCU_MCAST_FILTER_U32
 028030h 4 MAC_PCU_DIAG_SW
 028034h 4 MAC_PCU_TST_ADDAC
 028038h 4 MAC_PCU_DEF_ANTENNA
 02803Ch 4 MAC_PCU_AES_MUTE_MASK_0
 028040h 4 MAC_PCU_AES_MUTE_MASK_1
 028044h 4 MAC_PCU_GATED_CLKS
 028048h 4 MAC_PCU_OBS_BUS_2
 02804Ch 4 MAC_PCU_OBS_BUS_1
 028050h 4 MAC_PCU_DYM_MIMO_PWR_SAVE
 028054h 4 MAC_PCU_LAST_BEACON_TSF
 028058h 4 MAC_PCU_NAV
 02805Ch 4 MAC_PCU_RTS_SUCCESS_CNT
 028060h 4 MAC_PCU_RTS_FAIL_CNT
 028064h 4 MAC_PCU_ACK_FAIL_CNT
 028068h 4 MAC_PCU_FCS_FAIL_CNT
 02806Ch 4 MAC_PCU_BEACON_CNT
 028070h 4 MAC_PCU_XRMODE
 028074h 4 MAC_PCU_XRDEL
 028078h 4 MAC_PCU_XRTO
 02807Ch 4 MAC_PCU_XRCRP
 028080h 4 MAC_PCU_XRSTMP
 028084h 4 MAC_PCU_ADDR1_MASK_L32
 028088h 4 MAC_PCU_ADDR1_MASK_U16
 02808Ch 4 MAC_PCU_TPC
 028090h 4 MAC_PCU_TX_FRAME_CNT
 028094h 4 MAC_PCU_RX_FRAME_CNT
 028098h 4 MAC_PCU_RX_CLEAR_CNT
 02809Ch 4 MAC_PCU_CYCLE_CNT
 0280A0h 4 MAC_PCU_QUIET_TIME_1
 0280A4h 4 MAC_PCU_QUIET_TIME_2
 0280A8h 4 MAC_PCU_QOS_NO_ACK
 0280ACh 4 MAC_PCU_PHY_ERROR_MASK
 0280B0h 4 MAC_PCU_XRLAT
 0280B4h 4 MAC_PCU_RXBUF
 0280B8h 4 MAC_PCU_MIC_QOS_CONTROL
 0280BCh 4 MAC_PCU_MIC_QOS_SELECT
 0280C0h 4 MAC_PCU_MISC_MODE
 0280C4h 4 MAC_PCU_FILTER_OFDM_CNT
 0280C8h 4 MAC_PCU_FILTER_CCK_CNT
 0280CCh 4 MAC_PCU_PHY_ERR_CNT_1
 0280D0h 4 MAC_PCU_PHY_ERR_CNT_1_MASK
 0280D4h 4 MAC_PCU_PHY_ERR_CNT_2
 0280D8h 4 MAC_PCU_PHY_ERR_CNT_2_MASK
 0280DCh 4 MAC_PCU_TSF_THRESHOLD
 0280E0h 4 MAC_PCU_PHY_ERROR_EIFS_MASK
 0280E4h 4 MAC_PCU_PHY_ERR_CNT_3
 0280E8h 4 MAC_PCU_PHY_ERR_CNT_3_MASK
 0280ECh 4 MAC_PCU_BLUETOOTH_MODE
 0280F0h 4 MAC_PCU_BLUETOOTH_WEIGHTS
 0280F4h 4 MAC_PCU_BLUETOOTH_MODE2
 0280F8h 4 MAC_PCU_TXSIFS
 0280FCh 4 MAC_PCU_TXOP_X
 028100h 4 MAC_PCU_TXOP_0_3
 028104h 4 MAC_PCU_TXOP_4_7
 028108h 4 MAC_PCU_TXOP_8_11
 02810Ch 4 MAC_PCU_TXOP_12_15
 028110h 4 MAC_PCU_LOGIC_ANALYZER
 028114h 4 MAC_PCU_LOGIC_ANALYZER_32L
 028118h 4 MAC_PCU_LOGIC_ANALYZER_16U
 02811Ch 4 MAC_PCU_PHY_ERR_CNT_MASK_CONT
 028120h 4 MAC_PCU_AZIMUTH_MODE
 028124h 4 MAC_PCU_20_40_MODE
 028128h 4 MAC_PCU_RX_CLEAR_DIFF_CNT
 02812Ch 4 MAC_PCU_SELF_GEN_ANTENNA_MASK
 028130h 4 MAC_PCU_BA_BAR_CONTROL
 028134h 4 MAC_PCU_LEGACY_PLCP_SPOOF
 028138h 4 MAC_PCU_PHY_ERROR_MASK_CONT
 02813Ch 4 MAC_PCU_TX_TIMER
 028140h 4 MAC_PCU_TXBUF_CTRL
 028144h 4 MAC_PCU_MISC_MODE2 ;with extra bit in newer revision
 028148h 4 MAC_PCU_ALT_AES_MUTE_MASK
 02814Ch 4 MAC_PCU_AZIMUTH_TIME_STAMP
 028150h 4 MAC_PCU_MAX_CFP_DUR
 028154h 4 MAC_PCU_HCF_TIMEOUT
 028158h 4 MAC_PCU_BLUETOOTH_WEIGHTS2
 02815Ch 4 MAC_PCU_BLUETOOTH_TSF_BT_ACTIVE
 028160h 4 MAC_PCU_BLUETOOTH_TSF_BT_PRIORITY
 028164h 4 MAC_PCU_BLUETOOTH_MODE3
 028168h 4 MAC_PCU_BLUETOOTH_MODE4
 02816Ch 1x148 PAD0
 028200h 4x64 MAC_PCU_BT_BT[0..63]
 028300h 4 MAC_PCU_BT_BT_ASYNC
 028304h 4 MAC_PCU_BT_WL_1
 028308h 4 MAC_PCU_BT_WL_2
 02830Ch 4 MAC_PCU_BT_WL_3
 028310h 4 MAC_PCU_BT_WL_4
 028314h 4 MAC_PCU_COEX_EPTA
 028318h 4 MAC_PCU_COEX_LNAMAXGAIN1
 02831Ch 4 MAC_PCU_COEX_LNAMAXGAIN2
 028320h 4 MAC_PCU_COEX_LNAMAXGAIN3
 028324h 4 MAC_PCU_COEX_LNAMAXGAIN4
 028328h 4 MAC_PCU_BASIC_RATE_SET0
 02832Ch 4 MAC_PCU_BASIC_RATE_SET1
 028330h 4 MAC_PCU_BASIC_RATE_SET2
 028334h 4 MAC_PCU_BASIC_RATE_SET3
 028338h 4 MAC_PCU_RX_INT_STATUS0
 02833Ch 4 MAC_PCU_RX_INT_STATUS1
 028340h 4 MAC_PCU_RX_INT_STATUS2
 028344h 4 MAC_PCU_RX_INT_STATUS3
 028348h 4 HT_HALF_GI_RATE1
 02834Ch 4 HT_HALF_GI_RATE2
 028350h 4 HT_FULL_GI_RATE1
 028354h 4 HT_FULL_GI_RATE2
 028358h 4 LEGACY_RATE1
 02835Ch 4 LEGACY_RATE2
 028360h 4 LEGACY_RATE3
 028364h 4 RX_INT_FILTER ;with extra bit in newer revision
 028368h 4 RX_INT_OVERFLOW
 02836Ch 4 RX_FILTER_THRESH(0)
 028370h 4 RX_FILTER_THRESH1
 028374h 4 RX_PRIORITY_THRESH0
 028378h 4 RX_PRIORITY_THRESH1
 02837Ch 4 RX_PRIORITY_THRESH2
 028380h 4 RX_PRIORITY_THRESH3
 028384h 4 RX_PRIORITY_OFFSET0
 028388h 4 RX_PRIORITY_OFFSET1
 02838Ch 4 RX_PRIORITY_OFFSET2
 028390h 4 RX_PRIORITY_OFFSET3
 028394h 4 RX_PRIORITY_OFFSET4
 028398h 4 RX_PRIORITY_OFFSET5
 02839Ch 4 MAC_PCU_BSSID2_L32
 0283A0h 4 MAC_PCU_BSSID2_U16
 0283A4h 4 MAC_PCU_TSF1_STATUS_L32
 0283A8h 4 MAC_PCU_TSF1_STATUS_U32
 0283ACh 4 MAC_PCU_TSF2_STATUS_L32
 0283B0h 4 MAC_PCU_TSF2_STATUS_U32
 0283B4h 1x76 PAD1
 028400h 4x64 MAC_PCU_TXBUF_BA[0..63]
 028500h 1x768 PAD2
 028800h 4x256 MAC_PCU_KEY_CACHE_1[0..255]
 028C00h 1x3072 PAD3
 029800h 4x512 MAC_PCU_BASEBAND_0[0..511] ;\aka BB_xxx ports
 02A000h 4x2048 MAC_PCU_BASEBAND_1[0..2047] ;/(see below)
 02C000h 4x1024 MAC_PCU_BASEBAND_2[0..1023] ;\
 02D000h 4x1024 MAC_PCU_BASEBAND_3[0..1023] ; after BB registers
 02E000h 4x512 MAC_PCU_BUF[0..511] ;/
 02E800h - unused/unspecified |
| --- |

**bb_lc_reg.h (hw4.0)**

```

| "BASEBAND_0"
 029800h 4 BB_TEST_CONTROLS
 029804h 4 BB_GEN_CONTROLS
 029808h 4 BB_TEST_CONTROLS_STATUS
 02980Ch 4 BB_TIMING_CONTROLS_1
 029810h 4 BB_TIMING_CONTROLS_2
 029814h 4 BB_TIMING_CONTROLS_3
 029818h 4 BB_D2_CHIP_ID
 02981Ch 4 BB_ACTIVE
 029820h 4 BB_TX_TIMING_1
 029824h 4 BB_TX_TIMING_2
 029828h 4 BB_TX_TIMING_3
 02982Ch 4 BB_ADDAC_PARALLEL_CONTROL
 029830h 1x4 PAD__1
 029834h 4 BB_XPA_TIMING_CONTROL
 029838h 4 BB_MISC_PA_CONTROL
 02983Ch 4 BB_TSTDAC_CONSTANT
 029840h 4 BB_FIND_SIGNAL_LOW
 029844h 4 BB_SETTLING_TIME
 029848h 4 BB_GAIN_FORCE_MAX_GAINS_B0
 02984Ch 4 BB_GAINS_MIN_OFFSETS_B0
 029850h 4 BB_DESIRED_SIGSIZE
 029854h 4 BB_TIMING_CONTROL_3A
 029858h 4 BB_FIND_SIGNAL
 02985Ch 4 BB_AGC
 029860h 4 BB_AGC_CONTROL
 029864h 4 BB_CCA_B0
 029868h 4 BB_SFCORR
 02986Ch 4 BB_SELF_CORR_LOW
 029870h 1x4 PAD__2
 029874h 4 BB_SYNTH_CONTROL
 029878h 4 BB_ADDAC_CLK_SELECT
 02987Ch 4 BB_PLL_CNTL
 029880h 1x128 PAD__3
 029900h 4 BB_VIT_SPUR_MASK_A
 029904h 4 BB_VIT_SPUR_MASK_B
 029908h 4 BB_PILOT_SPUR_MASK
 02990Ch 4 BB_CHAN_SPUR_MASK
 029910h 4 BB_SPECTRAL_SCAN
 029914h 4 BB_ANALOG_POWER_ON_TIME
 029918h 4 BB_SEARCH_START_DELAY
 02991Ch 4 BB_MAX_RX_LENGTH
 029920h 4 BB_TIMING_CONTROL_4
 029924h 4 BB_TIMING_CONTROL_5
 029928h 4 BB_PHYONLY_WARM_RESET
 02992Ch 4 BB_PHYONLY_CONTROL
 029930h 1x4 PAD__4
 029934h 4 BB_POWERTX_RATE1 ;Power TX 0..3
 029938h 4 BB_POWERTX_RATE2 ;Power TX 4..7
 02993Ch 4 BB_POWERTX_MAX ;Power TX Flags
 029940h 4 BB_EXTENSION_RADAR
 029944h 4 BB_FRAME_CONTROL
 029948h 4 BB_TIMING_CONTROL_6
 02994Ch 4 BB_SPUR_MASK_CONTROLS
 029950h 4 BB_RX_IQ_CORR_B0
 029954h 4 BB_RADAR_DETECTION
 029958h 4 BB_RADAR_DETECTION_2
 02995Ch 4 BB_TX_PHASE_RAMP_B0
 029960h 4 BB_SWITCH_TABLE_CHN_B0
 029964h 4 BB_SWITCH_TABLE_COM1
 029968h 4 BB_CCA_CTRL_2_B0
 02996Ch 4 BB_SWITCH_TABLE_COM2
 029970h 4 BB_RESTART
 029974h 1x4 PAD__5
 029978h 4 BB_SCRAMBLER_SEED
 02997Ch 4 BB_RFBUS_REQUEST
 029980h 1x32 PAD__6
 0299A0h 4 BB_TIMING_CONTROL_11
 0299A4h 4 BB_MULTICHAIN_ENABLE
 0299A8h 4 BB_MULTICHAIN_CONTROL
 0299ACh 4 BB_MULTICHAIN_GAIN_CTRL
 0299B0h 1x4 PAD__7
 0299B4h 4 BB_ADC_GAIN_DC_CORR_B0
 0299B8h 4 BB_EXT_CHAN_PWR_THR_1
 0299BCh 4 BB_EXT_CHAN_PWR_THR_2_B0
 0299C0h 4 BB_EXT_CHAN_SCORR_THR
 0299C4h 4 BB_EXT_CHAN_DETECT_WIN
 0299C8h 4 BB_PWR_THR_20_40_DET
 0299CCh 1x4 PAD__8
 0299D0h 4 BB_SHORT_GI_DELTA_SLOPE
 0299D4h 1x8 PAD__9
 0299DCh 4 BB_CHANINFO_CTRL
 0299E0h 4 BB_HEAVY_CLIP_CTRL
 0299E4h 4 BB_HEAVY_CLIP_20
 0299E8h 4 BB_HEAVY_CLIP_40
 0299ECh 4 BB_RIFS_SRCH
 0299F0h 4 BB_IQ_ADC_CAL_MODE
 0299F4h 1x8 PAD__10
 0299FCh 4 BB_PER_CHAIN_CSD
 029A00h 4x128 BB_RX_OCGAIN[0..127]
 029C00h 4 BB_TX_CRC
 029C04h 1x12 PAD__11
 029C10h 4 BB_IQ_ADC_MEAS_0_B0
 029C14h 4 BB_IQ_ADC_MEAS_1_B0
 029C18h 4 BB_IQ_ADC_MEAS_2_B0
 029C1Ch 4 BB_IQ_ADC_MEAS_3_B0
 029C20h 4 BB_RFBUS_GRANT
 029C24h 4 BB_TSTADC
 029C28h 4 BB_TSTDAC
 029C2Ch 1x4 PAD__12
 029C30h 4 BB_ILLEGAL_TX_RATE
 029C34h 4 BB_SPUR_REPORT_B0
 029C38h 4 BB_CHANNEL_STATUS
 029C3Ch 4 BB_RSSI_B0
 029C40h 4 BB_SPUR_EST_CCK_REPORT_B0
 029C44h 1x104 PAD__13 ;(old 1x172)
 029CF0h 4 BB_CHAN_INFO_NOISE_PWR ;\
 029CF4h 4 BB_CHAN_INFO_GAIN_DIFF ; located HERE in
 029CF8h 4 BB_CHAN_INFO_FINE_TIMING ; older revision
 029CFCh 4 BB_CHAN_INFO_GAIN_B0 ; (unlike below)
 029D00h 4x60 BB_CHAN_INFO_CHAN_TAB_B0[0..59] ;/
 029CACh 4 BB_CHAN_INFO_NOISE_PWR ;\
 029CB0h 4 BB_CHAN_INFO_GAIN_DIFF ; located HERE in
 029CB4h 4 BB_CHAN_INFO_FINE_TIMING ; newer revision
 029CB8h 4 BB_CHAN_INFO_GAIN_B0 ; (unlike above)
 029CBCh 4x60 BB_CHAN_INFO_CHAN_TAB_B0[0..59] ;/
 029DACh 1x56 PAD__14 ;(old 1x528 at 9DF0h)
 029DE4h 4 BB_PAPRD_AM2AM_MASK ;\
 029DE8h 4 BB_PAPRD_AM2PM_MASK ;
 029DECh 4 BB_PAPRD_HT40_MASK ;
 029DF0h 4 BB_PAPRD_CTRL0 ; exists ONLY in
 029DF4h 4 BB_PAPRD_CTRL1 ; newer revision
 029DF8h 4 BB_PA_GAIN123 ;
 029DFCh 4 BB_PA_GAIN45 ;
 029E00h 4x8 BB_PAPRD_PRE_POST_SCALE_(0..7) ;
 029E20h 4x120 BB_PAPRD_MEM_TAB[....] ;/
 "BASEBAND_1"
 02A000h 4 BB_PEAK_DET_CTRL_1
 02A004h 4 BB_PEAK_DET_CTRL_2
 02A008h 4 BB_RX_GAIN_BOUNDS_1
 02A00Ch 4 BB_RX_GAIN_BOUNDS_2
 02A010h 4 BB_PEAK_DET_CAL_CTRL
 02A014h 4 BB_AGC_DIG_DC_CTRL
 02A018h 4 BB_AGC_DIG_DC_STATUS_I_B0
 02A01Ch 4 BB_AGC_DIG_DC_STATUS_Q_B0
 02A020h 1x468 PAD__15
 02A1F4h 4 BB_BBB_TXFIR_0
 02A1F8h 4 BB_BBB_TXFIR_1
 02A1FCh 4 BB_BBB_TXFIR_2
 02A200h 4 BB_MODES_SELECT
 02A204h 4 BB_BBB_TX_CTRL
 02A208h 4 BB_BBB_SIG_DETECT
 02A20Ch 4 BB_EXT_ATTEN_SWITCH_CTL_B0
 02A210h 4 BB_BBB_RX_CTRL_1
 02A214h 4 BB_BBB_RX_CTRL_2
 02A218h 4 BB_BBB_RX_CTRL_3
 02A21Ch 4 BB_BBB_RX_CTRL_4
 02A220h 4 BB_BBB_RX_CTRL_5
 02A224h 4 BB_BBB_RX_CTRL_6
 02A228h 4 BB_BBB_DAGC_CTRL
 02A22Ch 4 BB_FORCE_CLKEN_CCK
 02A230h 4 BB_RX_CLEAR_DELAY
 02A234h 4 BB_POWERTX_RATE3 ;Power TX 1L,2L,2S
 02A238h 4 BB_POWERTX_RATE4 ;Power TX 55L,55S,11L,11S
 02A23Ch 1x4 PAD__16
 02A240h 4 BB_CCK_SPUR_MIT
 02A244h 4 BB_PANIC_WATCHDOG_STATUS
 02A248h 4 BB_PANIC_WATCHDOG_CTRL_1
 02A24Ch 4 BB_PANIC_WATCHDOG_CTRL_2
 02A250h 4 BB_IQCORR_CTRL_CCK ;with extra bit in newer revision
 02A254h 4 BB_BLUETOOTH_CNTL
 02A258h 4 BB_TPC_1
 02A25Ch 4 BB_TPC_2
 02A260h 4 BB_TPC_3
 02A264h 4 BB_TPC_4_B0
 02A268h 4 BB_ANALOG_SWAP
 02A26Ch 4 BB_TPC_5_B0
 02A270h 4 BB_TPC_6_B0
 02A274h 4 BB_TPC_7
 02A278h 4 BB_TPC_8
 02A27Ch 4 BB_TPC_9
 02A280h 4x32 BB_PDADC_TAB_B0[0..31]
 02A300h 4x16 BB_CL_TAB_B0[0..15]
 02A340h 4 BB_CL_MAP_0_B0
 02A344h 4 BB_CL_MAP_1_B0
 02A348h 4 BB_CL_MAP_2_B0
 02A34Ch 4 BB_CL_MAP_3_B0
 02A350h 1x8 PAD__17
 02A358h 4 BB_CL_CAL_CTRL
 02A35Ch 4 BB_CL_MAP_PAL_0_B0 ;\
 02A360h 4 BB_CL_MAP_PAL_1_B0 ; exists ONLY in
 02A364h 4 BB_CL_MAP_PAL_2_B0 ; newer revision
 02A368h 4 BB_CL_MAP_PAL_3_B0 ;/
 02A36Ch 1x28 PAD__18 ;(old 1x44 at A35Ch)
 02A388h 4 BB_RIFS
 02A38Ch 4 BB_POWERTX_RATE5 ;Power TX HT20_0..3
 02A390h 4 BB_POWERTX_RATE6 ;Power TX HT20_4..7
 02A394h 4 BB_TPC_10
 02A398h 4 BB_TPC_11_B0
 02A39Ch 4 BB_CAL_CHAIN_MASK
 02A3A0h 1x28 PAD__19
 02A3BCh 4 BB_POWERTX_SUB ;Power TX Sub_for_2chain
 02A3C0h 4 BB_POWERTX_RATE7 ;Power TX HT40_0..3
 02A3C4h 4 BB_POWERTX_RATE8 ;Power TX HT40_4..7
 02A3C8h 4 BB_POWERTX_RATE9 ;Power TX DUP40/EXT20_CCK/OFDM
 02A3CCh 4 BB_POWERTX_RATE10 ;Power TX HT20_8..11
 02A3D0h 4 BB_POWERTX_RATE11 ;Power TX HT20/40_12/13
 02A3D4h 4 BB_POWERTX_RATE12 ;Power TX HT40_8..11
 02A3D8h 4 BB_FORCE_ANALOG
 02A3DCh 4 BB_TPC_12
 02A3E0h 4 BB_TPC_13
 02A3E4h 4 BB_TPC_14
 02A3E8h 4 BB_TPC_15
 02A3ECh 4 BB_TPC_16
 02A3F0h 4 BB_TPC_17
 02A3F4h 4 BB_TPC_18
 02A3F8h 4 BB_TPC_19
 02A3FCh 4 BB_TPC_20
 02A400h 4x32 BB_TX_GAIN_TAB_(1..32)
 02A480h 4x32 BB_TX_GAIN_TAB_PAL_(1..32)
 02A500h 1x24 PAD__20
 02A518h 4x16 BB_CALTX_GAIN_SET_(0,2,4,6,..,28,30)
 02A558h 4x96 BB_TXIQCAL_MEAS_B0[0..95]
 02A6D8h 4 BB_TXIQCAL_START
 02A6DCh 4 BB_TXIQCAL_CONTROL_0
 02A6E0h 4 BB_TXIQCAL_CONTROL_1
 02A6E4h 4 BB_TXIQCAL_CONTROL_2
 02A6E8h 4 BB_TXIQCAL_CONTROL_3
 02A6ECh 4 BB_TXIQ_CORR_COEFF_01_B0
 02A6F0h 4 BB_TXIQ_CORR_COEFF_23_B0
 02A6F4h 4 BB_TXIQ_CORR_COEFF_45_B0
 02A6F8h 4 BB_TXIQ_CORR_COEFF_67_B0
 02A6FCh 4 BB_TXIQ_CORR_COEFF_89_B0
 02A700h 4 BB_TXIQ_CORR_COEFF_AB_B0
 02A704h 4 BB_TXIQ_CORR_COEFF_CD_B0
 02A708h 4 BB_TXIQ_CORR_COEFF_EF_B0
 02A70Ch 4 BB_CAL_RXBB_GAIN_TBL_0
 02A710h 4 BB_CAL_RXBB_GAIN_TBL_4
 02A714h 4 BB_CAL_RXBB_GAIN_TBL_8
 02A718h 4 BB_CAL_RXBB_GAIN_TBL_12
 02A71Ch 4 BB_CAL_RXBB_GAIN_TBL_16
 02A720h 4 BB_CAL_RXBB_GAIN_TBL_20
 02A724h 4 BB_CAL_RXBB_GAIN_TBL_24
 02A728h 4 BB_TXIQCAL_STATUS_B0
 02A72Ch 4 BB_PAPRD_TRAINER_CNTL1 ;\
 02A730h 4 BB_PAPRD_TRAINER_CNTL2 ;
 02A734h 4 BB_PAPRD_TRAINER_CNTL3 ; exists ONLY in
 02A738h 4 BB_PAPRD_TRAINER_CNTL4 ; newer revision
 02A73Ch 4 BB_PAPRD_TRAINER_STAT1 ;
 02A740h 4 BB_PAPRD_TRAINER_STAT2 ;
 02A744h 4 BB_PAPRD_TRAINER_STAT3 ;/
 02A748h 1x144 PAD__21 ;(old 1x172 at A72Ch)
 02A7D8h 4 BB_FCAL_1
 02A7DCh 4 BB_FCAL_2_B0
 02A7E0h 4 BB_RADAR_BW_FILTER
 02A7E4h 4 BB_DFT_TONE_CTRL_B0
 02A7E8h 4 BB_THERM_ADC_1
 02A7ECh 4 BB_THERM_ADC_2
 02A7F0h 4 BB_THERM_ADC_3
 02A7F4h 4 BB_THERM_ADC_4
 02A7F8h 4 BB_TX_FORCED_GAIN
 02A7FCh 4 BB_ECO_CTRL
 02A800h 1x72 PAD__22
 02A848h 4 BB_GAIN_FORCE_MAX_GAINS_B1
 02A84Ch 4 BB_GAINS_MIN_OFFSETS_B1
 02A850h 1x432 PAD__23
 02AA00h 4x128 BB_RX_OCGAIN2[0..127]
 02AC00h 1x1548 PAD__24
 02B20Ch 4 BB_EXT_ATTEN_SWITCH_CTL_B1
 02B210h - unused/unspecified |
| --- |

**mac_pcu_reg.h (2) (hw4.0)**

```

| 02C000h 4x1024 MAC_PCU_BASEBAND_2[0..1023] ;\
 02D000h 4x1024 MAC_PCU_BASEBAND_3[0..1023] ; after BB registers
 02E000h 4x512 MAC_PCU_BUF[0..511] ;/
 02E800h - unused/unspecified |
| --- |

**rdma_reg.h (hw4.0)**

```

| 030100h 4 DMA_CONFIG
 030104h 4 DMA_CONTROL
 030108h 4 DMA_SRC
 03010Ch 4 DMA_DEST
 030110h 4 DMA_LENGTH
 030114h 4 VMC_BASE
 030118h 4 INDIRECT_REG
 03011Ch 4 INDIRECT_RETURN
 030120h 4x16 RDMA_REGION_(0..15)_
 030160h 4 DMA_STATUS
 030164h 4 DMA_INT_EN
 030168h - unused/unspecified |
| --- |

**efuse_reg.h (hw4.0)**

```

| 031000h 4 EFUSE_WR_ENABLE_REG
 031004h 4 EFUSE_INT_ENABLE_REG
 031008h 4 EFUSE_INT_STATUS_REG
 03100Ch 4 BITMASK_WR_REG
 031010h 4 VDDQ_SETTLE_TIME_REG
 031014h 4 RD_STROBE_PW_REG
 031018h 4 PG_STROBE_PW_REG
 03101Ch 1x2020 PAD0
 031800h 4x512 EFUSE_INTF[0..511]
 032000h - unused/unspecified |
| --- |

| DSi Atheros Wifi - Internal I/O Map Summary (hw6) |
| --- |

**Overall Summary (hw6.0)**

```

| 004000h 33Ch (rtc_soc_reg.h)
 xxx240h 1Ch (rtc_sync_reg.h) ;-unknown base address
 005000h 164h (rtc_wlan_reg.h)
 006000h 264h (wlan_coex_reg.h)
 007000h 94h (bt_coex_reg.h)
 008000h .. MIT (what is that...?) (maybe related to MITSUMI mode?)
 00C000h 14h (uart_reg.h)
 00D000h ... DBG_UART (another UART ?)
 00E000h 38h (umbox_wlan_reg.h)
 010000h 18h Serial I2C/SPI (si_reg.h)
 010018h 18h ADDR_ERROR (si_reg.h)
 014000h 170h (gpio_athr_wlan_reg.h)
 018000h 130h (mbox_wlan_reg.h)
 01A000h 2000h WLAN_HOST_IF_WINDOW (mbox_wlan_reg.h)
 01C000h 748h (analog_intf_athr_wlan_reg.h)
 020000h 130h (wmac_dma_reg.h)
 020800h 24Ch (wmac_qcu_reg.h)
 021000h 7FCh (wmac_dcu_reg.h)
 028000h 1000h (wmac_pcu_reg.h)
 029800h 3F8h bb_reg.h (1) - bb_chn_reg_map
 029C00h 24h bb_reg.h (2) - bb_mrc_reg_map
 029D00h 1Ch bb_reg.h (3) - bb_bbb_reg_map
 029E00h 400h bb_reg.h (4) - bb_agc_reg_map
 02A200h 5F8h bb_reg.h (5) - bb_sm_reg_map
 02A800h 3F8h bb_reg.h (6) - bb_chn1_reg_map
 02AE00h 400h bb_reg.h (7) - bb_agc1_reg_map
 02B200h 5F8h bb_reg.h (8) - bb_sm1_reg_map
 02C800h 400h bb_reg.h (9) - bb_chn3_reg_map (DUMMY)
 02CE00h 184h bb_reg.h (10) - bb_agc3_reg_map (mostly DUMMY)
 02D200h 600h bb_reg.h (11) - bb_sm3_reg_map (DUMMY)
 02D800h 20h bb_reg.h (12) - mit_local_reg_map, aka bb_mit_reg_map
 02E000h 4x2048 MAC_PCU_BUF (wmac_pcu_reg.h)
 030000h 1800h EFUSE (efuse_wlan_reg.h)
 034000h 1Ch STEREO 0 (stereo_reg.h)
 035000h 58h (chk_sum_seg_acc_reg.h)
 036000h ? STEREO 1 (maybe same format as STEREO 0 ?)
 038000h 3Ch (mmac_reg.h)
 039000h 0Ch (fpga_reg.h)
 040000h 8 (bridge_intr_reg.h)
 040100h 8 (mii_reg.h)
 040200h 28h (mdio_reg.h)
 040800h 20h (bridge_chain_gmac_0_rx_reg.h)
 040C00h 1Ch (bridge_chain_gmac_0_tx_reg.h)
 050000h .. SVD (what is that...?)
 054000h ... (usb_cast_reg.h) ;<--- located at 54000h (?)
 054100h .. usb RX chain 0..5 at 00054100h+(0..5)*100h (?)
 054700h .. usb TX chain 0..5 at 00054700h+(0..5)*100h (?)
 054C00h ... UART2 (yet another UART ?)
 054D00h A8h (rdma_reg.h)
 054E00h 50h (athrI2cSlaveApbCsr.h)
 055000h 40h I2S (mbox_i2s_reg.h)
 056000h .. I2S_1 (maybe same format as above "mbox_i2s_reg.h"?)
 xxxxxxh A00h (map_rf_reg.h) ;\unknown base address
 xxxxxxh 20h (odin_reg.h) ;/ |
| --- |

**rtc_soc_reg.h (hw6.0)**

```

| 004000h 4 SOC_RESET_CONTROL
 004004h 4 SOC_TCXO_DETECT
 004008h 4 SOC_XTAL_TEST
 00400Ch 1x20 PAD0
 004020h 4 SOC_CPU_CLOCK
 004024h 1x4 PAD1
 004028h 4 SOC_CLOCK_CONTROL
 00402Ch 1x4 PAD2
 004030h 4 SOC_WDT_CONTROL ;\
 004034h 4 SOC_WDT_STATUS ;
 004038h 4 SOC_WDT ; Watchdog Timer
 00403Ch 4 SOC_WDT_COUNT ;
 004040h 4 SOC_WDT_RESET ;/
 004044h 4 SOC_INT_STATUS ;-Interrupt Status
 004048h 4 SOC_LF_TIMER0 ;\
 00404Ch 4 SOC_LF_TIMER_COUNT0 ; Low-Freq Timer
 004050h 4 SOC_LF_TIMER_CONTROL0 ;
 004054h 4 SOC_LF_TIMER_STATUS0 ;/
 004058h 4 SOC_LF_TIMER1 ;\
 00405Ch 4 SOC_LF_TIMER_COUNT1 ; Low-Freq Timer
 004060h 4 SOC_LF_TIMER_CONTROL1 ;
 004064h 4 SOC_LF_TIMER_STATUS1 ;/
 004068h 4 SOC_LF_TIMER2 ;\
 00406Ch 4 SOC_LF_TIMER_COUNT2 ; Low-Freq Timer
 004070h 4 SOC_LF_TIMER_CONTROL2 ;
 004074h 4 SOC_LF_TIMER_STATUS2 ;/
 004078h 4 SOC_LF_TIMER3 ;\
 00407Ch 4 SOC_LF_TIMER_COUNT3 ; Low-Freq Timer
 004080h 4 SOC_LF_TIMER_CONTROL3 ;
 004084h 4 SOC_LF_TIMER_STATUS3 ;/
 004088h 4 SOC_HF_TIMER ;\
 00408Ch 4 SOC_HF_TIMER_COUNT ; High-Freq Timer
 004090h 4 SOC_HF_LF_COUNT ;<-- ;
 004094h 4 SOC_HF_TIMER_CONTROL ;
 004098h 4 SOC_HF_TIMER_STATUS ;/
 00409Ch 4 SOC_RTC_CONTROL ;\
 0040A0h 4 SOC_RTC_TIME ;
 0040A4h 4 SOC_RTC_DATE ;
 0040A8h 4 SOC_RTC_SET_TIME ; Real-Time Clock
 0040ACh 4 SOC_RTC_SET_DATE ;
 0040B0h 4 SOC_RTC_SET_ALARM ;
 0040B4h 4 SOC_RTC_CONFIG ;
 0040B8h 4 SOC_RTC_ALARM_STATUS ;/
 0040BCh 4 SOC_UART_WAKEUP
 0040C0h 4 SOC_RESET_CAUSE
 0040C4h 4 SOC_SYSTEM_SLEEP
 0040C8h 4 SOC_SDIO_WRAPPER
 0040CCh 4 SOC_INT_STATUS1
 0040D0h 1x4 PAD3
 0040D4h 4 SOC_LPO_CAL_TIME ;\
 0040D8h 4 SOC_LPO_INIT_DIVIDEND_INT ;
 0040DCh 4 SOC_LPO_INIT_DIVIDEND_FRACTION ; LPO
 0040E0h 4 SOC_LPO_CAL ;
 0040E4h 4 SOC_LPO_CAL_TEST_CONTROL ;
 0040E8h 4 SOC_LPO_CAL_TEST_STATUS ;/
 0040ECh 4 LEGACY_SOC_CHIP_ID ;\Chip ID
 0040F0h 4 SOC_CHIP_ID ;/
 0040F4h 1x24 PAD4
 00410Ch 4 SOC_POWER_REG
 004110h 4 SOC_CORE_CLK_CTRL
 004114h 4 SOC_GPIO_WAKEUP_CONTROL
 004118h 1x252 PAD5
 004214h 4 SLEEP_RETENTION
 004218h 1x108 PAD6
 004284h 4 LP_PERF_COUNTER ;\
 004288h 4 LP_PERF_LIGHT_SLEEP ; Perf
 00428Ch 4 LP_PERF_DEEP_SLEEP ;
 004290h 4 LP_PERF_ON ;/
 004294h 1x20 PAD7
 0042A8h 4 CHIP_MODE
 0042ACh 4 CLK_REQ_FALL_EDGE
 0042B0h 4 OTP ;\OTP
 0042B4h 4 OTP_STATUS ;/
 0042B8h 4 PMU
 0042BCh 4 PMU_CONFIG
 0042C0h 4 PMU_PAREG
 0042C4h 4 PMU_BYPASS
 0042C8h 1x20 PAD8
 0042DCh 4 THERM_CTRL1 ;\
 0042E0h 4 THERM_CTRL2 ; Therm
 0042E4h 4 THERM_CTRL3 ;/
 0042E8h 4 LISTEN_MODE1
 0042ECh 4 LISTEN_MODE2
 0042F0h 4 AUDIO_PLL_CONFIG
 0042F4h 4 AUDIO_PLL_MODULATION
 0042F8h 4 AUDIO_PLL_MOD_STEP
 0042FCh 4 CURRENT_AUDIO_PLL_MODULATION
 004300h 4 ETH_PLL_CONFIG
 004304h 4 CPU_PLL_CONFIG
 004308h 4 BB_PLL_CONFIG
 00430Ch 4 ETH_XMII
 004310h 4 USB_PHY_CONFIG
 004314h 4 MITSUMI_INT_CONTROL_REG
 004318h 4 MITSUMI_INT_STATUS_REG
 00431Ch 4 CURRENT_WORKING_MODE
 004320h 4 RTC_SLEEP_COUNT
 004324h 4 MIT2_VAP
 004328h 4 SECOND_HOST_INFT
 00432Ch 4 SDIO_HOST
 004330h 4 ENTERPRISE_CONFIG
 004334h 4 RTC_DEBUG_BUS
 004338h 4 RTC_EXT_CLK_BUF |
| --- |

**rtc_sync_reg.h (hw6.0)**

```

| 000000h 1x576 PAD__0
 000240h 4 RTC_SYNC_RESET
 000244h 4 RTC_SYNC_STATUS
 000248h 4 RTC_SYNC_DERIVED
 00024Ch 4 RTC_SYNC_FORCE_WAKE
 000250h 4 RTC_SYNC_INTR_CAUSE
 000254h 4 RTC_SYNC_INTR_ENABLE
 000258h 4 RTC_SYNC_INTR_MASK
 00025Ch .. - |
| --- |

**rtc_wlan_reg.h (hw6.0)**

```

| 005000h 4 WLAN_RESET_CONTROL
 005004h 4 WLAN_XTAL_CONTROL
 005008h 4 WLAN_REG_CONTROL0
 00500Ch 4 WLAN_REG_CONTROL1
 005010h 4 WLAN_QUADRATURE
 005014h 4 WLAN_PLL_CONTROL
 005018h 4 WLAN_PLL_SETTLE
 00501Ch 4 WLAN_XTAL_SETTLE
 005020h 4 WLAN_CLOCK_OUT
 005024h 4 WLAN_BIAS_OVERRIDE
 005028h 4 WLAN_RESET_CAUSE
 00502Ch 4 WLAN_SYSTEM_SLEEP
 005030h 4 WLAN_MAC_SLEEP_CONTROL
 005034h 4 WLAN_KEEP_AWAKE
 005038h 4 WLAN_DERIVED_RTC_CLK
 00503Ch 4 MAC_PCU_SLP32_MODE
 005040h 4 MAC_PCU_SLP32_WAKE
 005044h 4 MAC_PCU_SLP32_INC
 005048h 4 MAC_PCU_SLP_MIB1
 00504Ch 4 MAC_PCU_SLP_MIB2
 005050h 4 MAC_PCU_SLP_MIB3
 005054h 4 MAC_PCU_TSF_L32
 005058h 4 MAC_PCU_TSF_U32
 00505Ch 4 MAC_PCU_WBTIMER_0
 005060h 4 MAC_PCU_WBTIMER_1
 005064h 4x16 MAC_PCU_GENERIC_TIMERS[0..15]
 0050A4h 1x24 PAD__0
 0050BCh 4 MAC_PCU_GENERIC_TIMERS_MODE
 0050C0h 4 MAC_PCU_SLP1
 0050C4h 4 MAC_PCU_SLP2
 0050C8h 4 MAC_PCU_SLP3
 0050CCh 4 MAC_PCU_SLP4
 0050D0h 4 MAC_PCU_RESET_TSF
 0050D4h 4 MAC_PCU_TSF2_L32
 0050D8h 4 MAC_PCU_TSF2_U32
 0050DCh 4x16 MAC_PCU_GENERIC_TIMERS2[0..15]
 00511Ch 1x24 PAD__1
 005134h 4 MAC_PCU_GENERIC_TIMERS_MODE2
 005138h 1x12 PAD__2
 005144h 4 MAC_PCU_TSF_THRESHOLD
 005148h 4 WLAN_HT
 00514Ch 1x4 PAD__3
 005150h 4 MAC_PCU_GENERIC_TIMERS_TSF_SEL
 005154h 4 MAC_PCU_BMISS_TIMEOUT
 005158h 4 MAC_PCU_BMISS2_TIMEOUT
 00515Ch 4 RTC_AXI_AHB_BRIDGE
 005160h 4 UNIFIED_MAC_REVID
 005164h .. - |
| --- |

**wlan_coex_reg.h (hw6.0)**

```

| 006000h 4 MCI_COMMAND0
 006004h 4 MCI_COMMAND1
 006008h 4 MCI_COMMAND2
 00600Ch 4 MCI_RX_CTRL
 006010h 4 MCI_TX_CTRL
 006014h 4 MCI_MSG_ATTRIBUTES_TABLE
 006018h 4 MCI_SCHD_TABLE_0
 00601Ch 4 MCI_SCHD_TABLE_1
 006020h 4 MCI_GPM_0
 006024h 4 MCI_GPM_1
 006028h 4 MCI_INTERRUPT_RAW
 00602Ch 4 MCI_INTERRUPT_EN
 006030h 4 MCI_REMOTE_CPU_INT
 006034h 4 MCI_REMOTE_CPU_INT_EN
 006038h 4 MCI_INTERRUPT_RX_MSG_RAW
 00603Ch 4 MCI_INTERRUPT_RX_MSG_EN
 006040h 4 MCI_CPU_INT
 006044h 4 MCI_RX_STATUS
 006048h 4 MCI_CONT_STATUS
 00604Ch 4 MCI_BT_PRI0
 006050h 4 MCI_BT_PRI1
 006054h 4 MCI_BT_PRI2
 006058h 4 MCI_BT_PRI3
 00605Ch 4 MCI_BT_PRI
 006060h 4 MCI_WL_FREQ0
 006064h 4 MCI_WL_FREQ1
 006068h 4 MCI_WL_FREQ2
 00606Ch 4 MCI_GAIN
 006070h 4 MCI_WBTIMER1
 006074h 4 MCI_WBTIMER2
 006078h 4 MCI_WBTIMER3
 00607Ch 4 MCI_WBTIMER4
 006080h 4 MCI_MAXGAIN
 006084h 1x40 PAD__0
 0060ACh 4 BTCOEX_CTRL
 0060B0h 1x156 PAD__1
 00614Ch 4 BTCOEX_CTRL2
 006150h 1x260 PAD__2
 006254h 4 BTCOEX_DBG
 006258h 4 MCI_LAST_HW_MSG_HDR
 00625Ch 4 MCI_LAST_HW_MSG_BDY
 006260h 4 MCI_MAXGAIN_RST
 006264h .. - |
| --- |

**bt_coex_reg.h (hw6.0)**

```

| 007000h 4 BTCOEXCTRL ;\
 007004h 4 WBSYNC_PRIORITY1 ;
 007008h 4 WBSYNC_PRIORITY2 ;
 00700Ch 4 WBSYNC_PRIORITY3 ;
 007010h 4 BTCOEX0 ;SYNC_DUR ;
 007014h 4 BTCOEX1 ;CLK_THRES ;
 007018h 4 BTCOEX2 ;FRAME_THRES ;
 00701Ch 4 BTCOEX3 ;CLK_CNT ; moved from 004218h (hw4)
 007020h 4 BTCOEX4 ;FRAME_CNT ; to 007000h (hw6)
 007024h 4 BTCOEX5 ;IDLE_CNT ;
 007028h 4 BTCOEX6 ;IDLE_RESET_LVL_BITMAP ;
 00702Ch 4 LOCK ;
 007030h 4 NOLOCK_PRIORITY ;
 007034h 4 WBSYNC ;
 007038h 4 WBSYNC1 ;
 00703Ch 4 WBSYNC2 ;
 007040h 4 WBSYNC3 ;
 007044h 4 WB_TIMER_TARGET ;
 007048h 4 WB_TIMER_SLOP ;
 00704Ch 4 BTCOEX_INT_EN ;
 007050h 4 BTCOEX_INT_STAT ;
 007054h 4 BTPRIORITY_INT_EN ;
 007058h 4 BTPRIORITY_INT_STAT ;
 00705Ch 4 BTPRIORITY_STOMP_INT_EN ;
 007060h 4 BTPRIORITY_STOMP_INT_STAT ;/
 007064h 4 ST_64_BIT ;\
 007068h 4 MESSAGE_WR ; moved from 004294h (hw4)
 00706Ch 4 MESSAGE_WR_P ; to 007064h (hw6)
 007070h 4 MESSAGE_RD ;
 007074h 4 MESSAGE_RD_P ;/
 007078h 4 BTPRIORITY_INT ;\
 00707Ch 4 SCO_PARAMS ;
 007080h 4 SCO_PRIORITY ;
 007084h 4 SCO_SYNC ; new, hw6.0 only
 007088h 4 BTCOEX_RAW_STAT ;
 00708Ch 4 BTPRIORITY_RAW_STAT ;
 007090h 4 BTPRIORITY_STOMP_RAW_STAT ;/ |
| --- |

**uart_reg.h (hw6.0)**

```

| 00C000h 4 UART_DATA
 00C004h 4 UART_CONTROL
 00C008h 4 UART_CLKDIV
 00C00Ch 4 UART_INT
 00C010h 4 UART_INT_EN
 00C014h .. -
 00D000h .. ?? |
| --- |

**umbox_wlan_reg.h (hw6.0)**

```

| 00E000h 4x2 UMBOX_FIFO[0..1]
 00E008h 4 UMBOX_FIFO_STATUS
 00E00Ch 4 UMBOX_DMA_POLICY
 00E010h 4 UMBOX0_DMA_RX_DESCRIPTOR_BASE
 00E014h 4 UMBOX0_DMA_RX_CONTROL
 00E018h 4 UMBOX0_DMA_TX_DESCRIPTOR_BASE
 00E01Ch 4 UMBOX0_DMA_TX_CONTROL
 00E020h 4 UMBOX_FIFO_TIMEOUT
 00E024h 4 UMBOX_INT_STATUS
 00E028h 4 UMBOX_INT_ENABLE
 00E02Ch 4 UMBOX_DEBUG
 00E030h 4 UMBOX_FIFO_RESET
 00E034h 4 UMBOX_HCI_FRAMER |
| --- |

**Serial I2C/SPI (si_reg.h) (hw6.0)**

```

| 010000h 4 SI_CONFIG
 010004h 4 SI_CS
 010008h 4 SI_TX_DATA0
 01000Ch 4 SI_TX_DATA1
 010010h 4 SI_RX_DATA0
 010014h 4 SI_RX_DATA1 |
| --- |

**ADDR_ERROR (si_reg.h) (hw6.0)**

```

| 010018h 4 WLAN_APB_ADDR_ERROR_CONTROL ;\
 01001Ch 4 WLAN_APB_ADDR_ERROR_STATUS ; ADDR_ERROR
 010020h 4 WLAN_AHB_ADDR_ERROR_CONTROL ; (located at 8xxxh in hw4)
 010024h 4 WLAN_AHB_ADDR_ERROR_STATUS ;/
 010028h 4 WLAN_AHB_CONFIG
 01002Ch 4 WLAN_MEMORY_MAP |
| --- |

**gpio_athr_wlan_reg.h (hw6.0)**

```

| 014000h 4 WLAN_GPIO_OUT_LOW ;\
 014004h 4 WLAN_GPIO_OUT_W1TS_LOW ;
 014008h 4 WLAN_GPIO_OUT_W1TC_LOW ; GPIO Output Data
 01400Ch 4 WLAN_GPIO_OUT_HIGH ; (direct, and Write-1-To-Set/Clr)
 014010h 4 WLAN_GPIO_OUT_W1TS_HIGH ;
 014014h 4 WLAN_GPIO_OUT_W1TC_HIGH ;/
 014018h 4 WLAN_GPIO_ENABLE_LOW ;\
 01401Ch 4 WLAN_GPIO_ENABLE_W1TS_LOW ;
 014020h 4 WLAN_GPIO_ENABLE_W1TC_LOW ; GPIO Output Enable
 014024h 4 WLAN_GPIO_ENABLE_HIGH ; (direct, and Set/Clr)
 014028h 4 WLAN_GPIO_ENABLE_W1TS_HIGH ;
 01402Ch 4 WLAN_GPIO_ENABLE_W1TC_HIGH ;/
 014030h 4 WLAN_GPIO_IN_LOW ;\
 014034h 4 WLAN_GPIO_STATUS_LOW ;\ ; GPIO Input
 014038h 4 WLAN_GPIO_IN_HIGH ; ;/
 01403Ch 4 WLAN_GPIO_STATUS_HIGH ;
 014040h 4 WLAN_GPIO_STATUS_W1TS_LOW ; GPIO Interrupt Status
 014044h 4 WLAN_GPIO_STATUS_W1TC_LOW ; (direct, and Set/Clr)
 014048h 4 WLAN_GPIO_STATUS_W1TS_HIGH ;
 01404Ch 4 WLAN_GPIO_STATUS_W1TC_HIGH ;/
 014050h 4 WLAN_GPIO_PIN0 ;GPIO0 or SDIO_CMD
 014054h 4 WLAN_GPIO_PIN1 ;GPIO1 or SDIO_D3
 014058h 4 WLAN_GPIO_PIN2 ;GPIO2 or SDIO_D2
 01405Ch 4 WLAN_GPIO_PIN3 ;GPIO3 or SDIO_D1
 014060h 4 WLAN_GPIO_PIN4 ;GPIO4 or SDIO_D0
 014064h 4 WLAN_GPIO_PIN5 ;GPIO5 or SDIO_CLK
 014068h 4 WLAN_GPIO_PIN6 ;GPIO6
 01406Ch 4 WLAN_GPIO_PIN7 ;GPIO7
 014070h 4 WLAN_GPIO_PIN8 ;...
 014074h 4 WLAN_GPIO_PIN9 ;..
 014078h 4 WLAN_GPIO_PIN10
 01407Ch 4 WLAN_GPIO_PIN11
 014080h 4 WLAN_GPIO_PIN12
 014084h 4 WLAN_GPIO_PIN13
 014088h 4 WLAN_GPIO_PIN14
 01408Ch 4 WLAN_GPIO_PIN15
 014090h 4 WLAN_GPIO_PIN16
 014094h 4 WLAN_GPIO_PIN17
 014098h 4 WLAN_GPIO_PIN18
 01409Ch 4 WLAN_GPIO_PIN19
 0140A0h 4 WLAN_GPIO_PIN20
 0140A4h 4 WLAN_GPIO_PIN21
 0140A8h 4 WLAN_GPIO_PIN22
 0140ACh 4 WLAN_GPIO_PIN23
 0140B0h 4 WLAN_GPIO_PIN24
 0140B4h 4 WLAN_GPIO_PIN25
 0140B8h 4 WLAN_GPIO_PIN26
 0140BCh 4 WLAN_GPIO_PIN27
 0140C0h 4 WLAN_GPIO_PIN28
 0140C4h 4 WLAN_GPIO_PIN29
 0140C8h 4 WLAN_GPIO_PIN30
 0140CCh 4 WLAN_GPIO_PIN31
 0140D0h 4 WLAN_GPIO_PIN32
 0140D4h 4 WLAN_GPIO_PIN33
 0140D8h 4 WLAN_GPIO_PIN34
 0140DCh 4 WLAN_GPIO_PIN35
 0140E0h 4 WLAN_GPIO_PIN36
 0140E4h 4 WLAN_GPIO_PIN37
 0140E8h 4 WLAN_GPIO_PIN38
 0140ECh 4 WLAN_GPIO_PIN39
 0140F0h 4 WLAN_GPIO_PIN40
 0140F4h 4 WLAN_GPIO_PIN41
 0140F8h 4 WLAN_GPIO_PIN42
 0140FCh 4 WLAN_GPIO_PIN43
 014100h 4 WLAN_GPIO_PIN44
 014104h 4 WLAN_GPIO_PIN45
 014108h 4 WLAN_GPIO_PIN46
 01410Ch 4 WLAN_GPIO_PIN47
 014110h 4 WLAN_GPIO_PIN48
 014114h 4 WLAN_GPIO_PIN49
 014118h 4 WLAN_GPIO_PIN50
 01411Ch 4 WLAN_GPIO_PIN51
 014120h 4 WLAN_GPIO_PIN52
 014124h 4 WLAN_GPIO_PIN53
 014128h 4 WLAN_GPIO_PIN54
 01412Ch 4 WLAN_GPIO_PIN55
 014130h 4 WLAN_GPIO_PIN56
 014134h 4 SDIO
 014138h 4 WL_SOC_APB
 01413Ch 4 WLAN_SIGMA_DELTA
 014140h 4 WL_BOOTSTRAP
 014144h 4 CORE_BOOTSTRAP_LOW
 014148h 4 CORE_BOOTSTRAP_HIGH
 01414Ch 4 WLAN_DEBUG_CONTROL
 014150h 4 WLAN_DEBUG_INPUT_SEL
 014154h 4 WLAN_DEBUG_OUT
 014158h 4 WLAN_RESET_TUPLE_STATUS
 01415Ch 4 ANTENNA_CONTROL
 014160h 4 SDIO2
 014164h 4 SDHC
 014168h 4 AMBA_DEBUG_BUS
 01416Ch 4 CPU_MBIST |
| --- |

**MBOX Registers (mbox_wlan_reg.h) (hw6.0)**

```

| 018000h 4x4 WLAN_MBOX_FIFO[0..3]
 018010h 4 WLAN_MBOX_FIFO_STATUS
 018014h 4 WLAN_MBOX_DMA_POLICY
 018018h 4 WLAN_MBOX0_DMA_RX_DESCRIPTOR_BASE ;\
 01801Ch 4 WLAN_MBOX0_DMA_RX_CONTROL ; MBOX 0
 018020h 4 WLAN_MBOX0_DMA_TX_DESCRIPTOR_BASE ;
 018024h 4 WLAN_MBOX0_DMA_TX_CONTROL ;/
 018028h 4 WLAN_MBOX1_DMA_RX_DESCRIPTOR_BASE ;\
 01802Ch 4 WLAN_MBOX1_DMA_RX_CONTROL ; MBOX 1
 018030h 4 WLAN_MBOX1_DMA_TX_DESCRIPTOR_BASE ;
 018034h 4 WLAN_MBOX1_DMA_TX_CONTROL ;/
 018038h 4 WLAN_MBOX2_DMA_RX_DESCRIPTOR_BASE ;\
 01803Ch 4 WLAN_MBOX2_DMA_RX_CONTROL ; MBOX 2
 018040h 4 WLAN_MBOX2_DMA_TX_DESCRIPTOR_BASE ;
 018044h 4 WLAN_MBOX2_DMA_TX_CONTROL ;/
 018048h 4 WLAN_MBOX3_DMA_RX_DESCRIPTOR_BASE ;\
 01804Ch 4 WLAN_MBOX3_DMA_RX_CONTROL ; MBOX 3
 018050h 4 WLAN_MBOX3_DMA_TX_DESCRIPTOR_BASE ;
 018054h 4 WLAN_MBOX3_DMA_TX_CONTROL ;/
 018058h 4 WLAN_MBOX_INT_STATUS ;\Interrupt
 01805Ch 4 WLAN_MBOX_INT_ENABLE ;/
 018060h 4 WLAN_INT_HOST
 018064h 1x28 PAD0
 018080h 4x8 WLAN_LOCAL_COUNT[0..7]
 0180A0h 4x8 WLAN_COUNT_INC[0..7]
 0180C0h 4x8 WLAN_LOCAL_SCRATCH[0..7]
 0180E0h 4 WLAN_USE_LOCAL_BUS
 0180E4h 4 WLAN_SDIO_CONFIG
 0180E8h 4 WLAN_MBOX_DEBUG
 0180ECh 4 WLAN_MBOX_FIFO_RESET
 0180F0h 4x4 WLAN_MBOX_TXFIFO_POP[0..3]
 018100h 4x4 WLAN_MBOX_RXFIFO_POP[0..3]
 018110h 4 WLAN_SDIO_DEBUG
 018114h 4 WLAN_GMBOX0_DMA_RX_DESCRIPTOR_BASE ;\
 018118h 4 WLAN_GMBOX0_DMA_RX_CONTROL ;
 01811Ch 4 WLAN_GMBOX0_DMA_TX_DESCRIPTOR_BASE ; hw4.0 and hw6.0
 018120h 4 WLAN_GMBOX0_DMA_TX_CONTROL ;
 018124h 4 WLAN_GMBOX_INT_STATUS ;
 018128h 4 WLAN_GMBOX_INT_ENABLE ;/
 01812Ch 4 STE_MODE ;<-- hw6.0 only
 018130h 1x7888 PAD1
 01A000h 4x2048 WLAN_HOST_IF_WINDOW[0..2047] |
| --- |

**analog_intf_athr_wlan_reg.h (hw6.0)**

```

| 01C000h 4 RXRF_BIAS1
 01C004h 4 RXRF_BIAS2
 01C008h 4 RXRF_GAINSTAGES
 01C00Ch 4 RXRF_AGC
 01C010h 1x48 PAD__0
 01C040h 4 TXRF1
 01C044h 4 TXRF2
 01C048h 4 TXRF3
 01C04Ch 4 TXRF4
 01C050h 4 TXRF5
 01C054h 4 TXRF6
 01C058h 4 TXRF7
 01C05Ch 4 TXRF8
 01C060h 4 TXRF9
 01C064h 4 TXRF10
 01C068h 4 TXRF11
 01C06Ch 4 TXRF12
 01C070h 1x16 PAD__1
 01C080h 4 SYNTH1
 01C084h 4 SYNTH2
 01C088h 4 SYNTH3
 01C08Ch 4 SYNTH4
 01C090h 4 SYNTH5
 01C094h 4 SYNTH6
 01C098h 4 SYNTH7
 01C09Ch 4 SYNTH8
 01C0A0h 4 SYNTH9
 01C0A4h 4 SYNTH10
 01C0A8h 4 SYNTH11
 01C0ACh 4 SYNTH12
 01C0B0h 4 SYNTH13
 01C0B4h 4 SYNTH14
 01C0B8h 1x8 PAD__2
 01C0C0h 4 BIAS1
 01C0C4h 4 BIAS2
 01C0C8h 4 BIAS3
 01C0CCh 4 BIAS4
 01C0D0h 1x48 PAD__3
 01C100h 4 RXTX1
 01C104h 4 RXTX2
 01C108h 4 RXTX3
 01C10Ch 1x52 PAD__4
 01C140h 4 BB1
 01C144h 4 BB2
 01C148h 4 BB3
 01C14Ch 1x308 PAD__5
 01C280h 4 PLLCLKMODA
 01C284h 4 PLLCLKMODA2
 01C288h 4 TOP
 01C28Ch 4 THERM
 01C290h 4 XTAL
 01C294h 1x236 PAD__6
 01C380h 4 RBIST_CNTRL
 01C384h 4 TX_DC_OFFSET
 01C388h 4 TX_TONEGEN0
 01C38Ch 4 TX_TONEGEN1
 01C390h 4 TX_LFTONEGEN0
 01C394h 4 TX_LINEAR_RAMP_I
 01C398h 4 TX_LINEAR_RAMP_Q
 01C39Ch 4 TX_PRBS_MAG
 01C3A0h 4 TX_PRBS_SEED_I
 01C3A4h 4 TX_PRBS_SEED_Q
 01C3A8h 4 CMAC_DC_CANCEL
 01C3ACh 4 CMAC_DC_OFFSET
 01C3B0h 4 CMAC_CORR
 01C3B4h 4 CMAC_POWER
 01C3B8h 4 CMAC_CROSS_CORR
 01C3BCh 4 CMAC_I2Q2
 01C3C0h 4 CMAC_POWER_HPF
 01C3C4h 4 RXDAC_SET1
 01C3C8h 4 RXDAC_SET2
 01C3CCh 4 RXDAC_LONG_SHIFT
 01C3D0h 4 CMAC_RESULTS_I
 01C3D4h 4 CMAC_RESULTS_Q
 01C3D8h 1x872 PAD__7
 01C740h 4 PMU1
 01C744h 4 PMU2 |
| --- |

**wmac_dma_reg.h (hw6.0)**

```

| 020000h 1x8 PAD__0
 020008h 4 MAC_DMA_CR
 020004h 1x4 PAD__1
 02000Ch 1x4 PAD__1
 020014h 4 MAC_DMA_CFG
 020018h 4 MAC_DMA_RXBUFPTR_THRESH
 02001Ch 4 MAC_DMA_TXDPPTR_THRESH
 020020h 4 MAC_DMA_MIRT
 020024h 4 MAC_DMA_GLOBAL_IER
 020028h 4 MAC_DMA_TIMT_0
 02002Ch 4 MAC_DMA_RIMT
 020030h 4 MAC_DMA_TXCFG
 020034h 4 MAC_DMA_RXCFG
 020038h 4 MAC_DMA_RXJLA
 02003Ch 1x4 PAD__2
 020040h 4 MAC_DMA_MIBC
 020044h 4 MAC_DMA_TOPS
 020048h 4 MAC_DMA_RXNPTO
 02004Ch 4 MAC_DMA_TXNPTO
 020050h 4 MAC_DMA_RPGTO
 020054h 1x4 PAD__3
 020058h 4 MAC_DMA_MACMISC
 02005Ch 4 MAC_DMA_INTER
 020060h 4 MAC_DMA_DATABUF
 020064h 4 MAC_DMA_GTT
 020068h 4 MAC_DMA_GTTM
 02006Ch 4 MAC_DMA_CST
 020070h 4 MAC_DMA_RXDP_SIZE
 020074h 4 MAC_DMA_RX_QUEUE_HP_RXDP
 020078h 4 MAC_DMA_RX_QUEUE_LP_RXDP
 02007Ch 1x4 PAD__4
 020080h 4 MAC_DMA_ISR_P - Primary Interrupt Status Register ;\
 020084h 4 MAC_DMA_ISR_S0 - Secondary Interrupt 0 Status TX OK/DESC ;
 020088h 4 MAC_DMA_ISR_S1 - Secondary Interrupt 1 Status TX ERR/EOL ;
 02008Ch 4 MAC_DMA_ISR_S2 - Secondary Interrupt 2 Status TX URN/MISC ;
 020090h 4 MAC_DMA_ISR_S3 - Secondary Interrupt 3 Status QCBR OVF/URN ;
 020094h 4 MAC_DMA_ISR_S4 - Secondary Interrupt 4 Status QTRIG ;
 020098h 4 MAC_DMA_ISR_S5 - Secondary Interrupt 5 Status TIMERS ;
 02009Ch 4 MAC_DMA_ISR_S6 - Secondary Interrupt 6 Status UNKNOWN? ;/
 0200A0h 4 MAC_DMA_IMR_P - Primary Interrupt Mask Register ;\
 0200A4h 4 MAC_DMA_IMR_S0 - Secondary Interrupt 0 Mask TX OK/DESC ;
 0200A8h 4 MAC_DMA_IMR_S1 - Secondary Interrupt 1 Mask TX ERR/EOL ;
 0200ACh 4 MAC_DMA_IMR_S2 - Secondary Interrupt 2 Mask TX URN/MISC ;
 0200B0h 4 MAC_DMA_IMR_S3 - Secondary Interrupt 3 Mask QCBR OVF/URN ;
 0200B4h 4 MAC_DMA_IMR_S4 - Secondary Interrupt 4 Mask QTRIG ;
 0200B8h 4 MAC_DMA_IMR_S5 - Secondary Interrupt 5 Mask TIMERS ;
 0200BCh 4 MAC_DMA_IMR_S6 - Secondary Interrupt 6 Mask UNKNOWN? ;/
 0200C0h 4 MAC_DMA_ISR_P_RAC - Primary Interrupt Read-and-Clear ;\
 0200C4h 4 MAC_DMA_ISR_S0_S - Secondary 0 Read-and-Clr TX OK/DESC ;
 0200C8h 4 MAC_DMA_ISR_S1_S - Secondary 1 Read-and-Clr TX ERR/EOL ;
 0200CCh 4 MAC_DMA_ISR_S6_S - Secondary 6? Read-and-Clr UNKNOWN? <-- ;
 0200D0h 4 MAC_DMA_ISR_S2_S - Secondary 2? Read-and-Clr TX URN/MISC ;
 0200D4h 4 MAC_DMA_ISR_S3_S - Secondary 3? Read-and-Clr QCBR OVF/URN ;
 0200D8h 4 MAC_DMA_ISR_S4_S - Secondary 4? Read-and-Clr QTRIG ;
 0200DCh 4 MAC_DMA_ISR_S5_S - Secondary 5? Read-and-Clr TIMERS ;/
 0200E0h 4 MAC_DMA_DMADBG_0
 0200E4h 4 MAC_DMA_DMADBG_1
 0200E8h 4 MAC_DMA_DMADBG_2
 0200ECh 4 MAC_DMA_DMADBG_3
 0200F0h 4 MAC_DMA_DMADBG_4
 0200F4h 4 MAC_DMA_DMADBG_5
 0200F8h 4 MAC_DMA_DMADBG_6
 0200FCh 4 MAC_DMA_DMADBG_7
 020100h 4 MAC_DMA_QCU_TXDP_REMAINING_QCU_7_0
 020104h 4 MAC_DMA_QCU_TXDP_REMAINING_QCU_9_8
 020108h 4 MAC_DMA_TIMT_1 ;note: "MAC_DMA_TIMT_0" is at 020028h
 02010Ch 4 MAC_DMA_TIMT_2
 020110h 4 MAC_DMA_TIMT_3
 020114h 4 MAC_DMA_TIMT_4
 020118h 4 MAC_DMA_TIMT_5
 02011Ch 4 MAC_DMA_TIMT_6
 020120h 4 MAC_DMA_TIMT_7
 020124h 4 MAC_DMA_TIMT_8
 020128h 4 MAC_DMA_TIMT_9
 02012Ch 4 MAC_DMA_CHKACC |
| --- |

**wmac_qcu_reg.h (hw6.0)**

```

| 020800h 4x10 MAC_QCU_TXDP[0..9]
 020828h 1x8 PAD__1
 020830h 4 MAC_QCU_STATUS_RING_START
 020834h 4 MAC_QCU_STATUS_RING_END
 020838h 4 MAC_QCU_STATUS_RING_CURRENT
 02083Ch 1x4 PAD__2
 020840h 4 MAC_QCU_TXE
 020844h 1x60 PAD__3
 020880h 4 MAC_QCU_TXD
 020884h 1x60 PAD__4
 0208C0h 4x10 MAC_QCU_CBR[0..9]
 0208E8h 1x24 PAD__5
 020900h 4x10 MAC_QCU_RDYTIME[0..9]
 020928h 1x24 PAD__6
 020940h 4 MAC_QCU_ONESHOT_ARM_SC
 020944h 1x60 PAD__7
 020980h 4 MAC_QCU_ONESHOT_ARM_CC
 020984h 1x60 PAD__8
 0209C0h 4x10 MAC_QCU_MISC[0..9]
 0209E8h 1x24 PAD__9
 020A00h 4x10 MAC_QCU_CNT[0..9]
 020A28h 1x24 PAD__10
 020A40h 4 MAC_QCU_RDYTIME_SHDN
 020A44h 4 MAC_QCU_DESC_CRC_CHK
 020A48h 4 MAC_QCU_EOL |
| --- |

**wmac_dcu_reg.h (hw6.0)**

```

| 021000h 4x10 MAC_DCU_QCUMASK[0..9]
 021028h 1x8 PAD__1
 021030h 4 MAC_DCU_GBL_IFS_SIFS
 021034h 1x4 PAD__2
 021038h 4 MAC_DCU_TXFILTER_DCU0_31_0
 02103Ch 4 MAC_DCU_TXFILTER_DCU8_31_0
 021040h 4x10 MAC_DCU_LCL_IFS[0..9]
 021068h 1x8 PAD__3
 021070h 4 MAC_DCU_GBL_IFS_SLOT
 021074h 1x4 PAD__4
 021078h 4 MAC_DCU_TXFILTER_DCU0_63_32
 02107Ch 4 MAC_DCU_TXFILTER_DCU8_63_32
 021080h 4x10 MAC_DCU_RETRY_LIMIT[0..9]
 0210A8h 1x8 PAD__5
 0210B0h 4 MAC_DCU_GBL_IFS_EIFS
 0210B4h 1x4 PAD__6
 0210B8h 4 MAC_DCU_TXFILTER_DCU0_95_64
 0210BCh 4 MAC_DCU_TXFILTER_DCU8_95_64
 0210C0h 4x10 MAC_DCU_CHANNEL_TIME[0..9]
 0210E8h 1x8 PAD__7
 0210F0h 4 MAC_DCU_GBL_IFS_MISC
 0210F4h 1x4 PAD__8
 0210F8h 4 MAC_DCU_TXFILTER_DCU0_127_96
 0210FCh 4 MAC_DCU_TXFILTER_DCU8_127_96
 021100h 4x10 MAC_DCU_MISC[0..9]
 021128h 1x16 PAD__9
 021138h 4 MAC_DCU_TXFILTER_DCU1_31_0
 02113Ch 4 MAC_DCU_TXFILTER_DCU9_31_0
 021140h 4 MAC_DCU_SEQ
 021144h 1x52 PAD__10
 021178h 4 MAC_DCU_TXFILTER_DCU1_63_32
 02117Ch 4 MAC_DCU_TXFILTER_DCU9_63_32
 021180h 1x56 PAD__11
 0211B8h 4 MAC_DCU_TXFILTER_DCU1_95_64
 0211BCh 4 MAC_DCU_TXFILTER_DCU9_95_64
 0211C0h 1x56 PAD__12
 0211F8h 4 MAC_DCU_TXFILTER_DCU1_127_96
 0211FCh 4 MAC_DCU_TXFILTER_DCU9_127_96
 021200h 1x56 PAD__13
 021238h 4 MAC_DCU_TXFILTER_DCU2_31_0
 02123Ch 1x52 PAD__14
 021270h 4 MAC_DCU_PAUSE
 021274h 1x4 PAD__15
 021278h 4 MAC_DCU_TXFILTER_DCU2_63_32
 02127Ch 1x52 PAD__16
 0212B0h 4 MAC_DCU_WOW_KACFG
 0212B4h 1x4 PAD__17
 0212B8h 4 MAC_DCU_TXFILTER_DCU2_95_64
 0212BCh 1x52 PAD__18
 0212F0h 4 MAC_DCU_TXSLOT
 0212F4h 1x4 PAD__19
 0212F8h 4 MAC_DCU_TXFILTER_DCU2_127_96
 0212FCh 1x60 PAD__20
 021338h 4 MAC_DCU_TXFILTER_DCU3_31_0 ;\
 02133Ch 1x60 PAD__21 ;
 021378h 4 MAC_DCU_TXFILTER_DCU3_63_32 ;
 02137Ch 1x60 PAD__22 ;
 0213B8h 4 MAC_DCU_TXFILTER_DCU3_95_64 ;
 0213BCh 1x60 PAD__23 ;
 0213F8h 4 MAC_DCU_TXFILTER_DCU3_127_96 ;/
 0213FCh 1x60 PAD__24
 021438h 4 MAC_DCU_TXFILTER_DCU4_31_0
 02143Ch 4 MAC_DCU_TXFILTER_CLEAR
 021440h 1x56 PAD__25
 021478h 4 MAC_DCU_TXFILTER_DCU4_63_32
 02147Ch 4 MAC_DCU_TXFILTER_SET
 021480h 1x56 PAD__26
 0214B8h 4 MAC_DCU_TXFILTER_DCU4_95_64
 0214BCh 1x60 PAD__27
 0214F8h 4 MAC_DCU_TXFILTER_DCU4_127_96
 0214FCh 1x60 PAD__28
 021538h 4 MAC_DCU_TXFILTER_DCU5_31_0 ;\
 02153Ch 1x60 PAD__29 ;
 021578h 4 MAC_DCU_TXFILTER_DCU5_63_32 ;
 02157Ch 1x60 PAD__30 ;
 0215B8h 4 MAC_DCU_TXFILTER_DCU5_95_64 ;
 0215BCh 1x60 PAD__31 ;
 0215F8h 4 MAC_DCU_TXFILTER_DCU5_127_96 ;/
 0215FCh 1x60 PAD__32
 021638h 4 MAC_DCU_TXFILTER_DCU6_31_0 ;\
 02163Ch 1x60 PAD__33 ;
 021678h 4 MAC_DCU_TXFILTER_DCU6_63_32 ;
 02167Ch 1x60 PAD__34 ;
 0216B8h 4 MAC_DCU_TXFILTER_DCU6_95_64 ;
 0216BCh 1x60 PAD__35 ;
 0216F8h 4 MAC_DCU_TXFILTER_DCU6_127_96 ;/
 0216FCh 1x60 PAD__36
 021738h 4 MAC_DCU_TXFILTER_DCU7_31_0 ;\
 02173Ch 1x60 PAD__37 ;
 021778h 4 MAC_DCU_TXFILTER_DCU7_63_32 ;
 02177Ch 1x60 PAD__38 ;
 0217B8h 4 MAC_DCU_TXFILTER_DCU7_95_64 ;
 0217BCh 1x60 PAD__39 ;
 0217F8h 4 MAC_DCU_TXFILTER_DCU7_127_96 ;/ |
| --- |

**wmac_pcu_reg.h (1) (hw6.0)**

```

| 028000h 4 MAC_PCU_STA_ADDR_L32
 028004h 4 MAC_PCU_STA_ADDR_U16
 028008h 4 MAC_PCU_BSSID_L32
 02800Ch 4 MAC_PCU_BSSID_U16
 028010h 4 MAC_PCU_BCN_RSSI_AVE
 028014h 4 MAC_PCU_ACK_CTS_TIMEOUT
 028018h 4 MAC_PCU_BCN_RSSI_CTL
 02801Ch 4 MAC_PCU_USEC_LATENCY
 028020h 4 MAC_PCU_BCN_RSSI_CTL2
 028024h 4 MAC_PCU_BT_WL_1
 028028h 4 MAC_PCU_BT_WL_2
 02802Ch 4 MAC_PCU_BT_WL_3
 028030h 4 MAC_PCU_BT_WL_4
 028034h 4 MAC_PCU_COEX_EPTA
 028038h 4 MAC_PCU_MAX_CFP_DUR
 02803Ch 4 MAC_PCU_RX_FILTER
 028040h 4 MAC_PCU_MCAST_FILTER_L32
 028044h 4 MAC_PCU_MCAST_FILTER_U32
 028048h 4 MAC_PCU_DIAG_SW
 02804Ch 1x8 PAD__1
 028054h 4 MAC_PCU_TST_ADDAC
 028058h 4 MAC_PCU_DEF_ANTENNA
 02805Ch 4 MAC_PCU_AES_MUTE_MASK_0
 028060h 4 MAC_PCU_AES_MUTE_MASK_1
 028064h 4 MAC_PCU_GATED_CLKS
 028068h 4 MAC_PCU_OBS_BUS_2
 02806Ch 4 MAC_PCU_OBS_BUS_1
 028070h 4 MAC_PCU_DYM_MIMO_PWR_SAVE
 028074h 1x12 PAD__2
 028080h 4 MAC_PCU_LAST_BEACON_TSF
 028084h 4 MAC_PCU_NAV
 028088h 4 MAC_PCU_RTS_SUCCESS_CNT
 02808Ch 4 MAC_PCU_RTS_FAIL_CNT
 028090h 4 MAC_PCU_ACK_FAIL_CNT
 028094h 4 MAC_PCU_FCS_FAIL_CNT
 028098h 4 MAC_PCU_BEACON_CNT
 02809Ch 4 MAC_PCU_BEACON2_CNT
 0280A0h 4 MAC_PCU_BASIC_SET
 0280A4h 4 MAC_PCU_MGMT_SEQ
 0280A8h 4 MAC_PCU_BF_RPT1
 0280ACh 4 MAC_PCU_BF_RPT2
 0280B0h 4 MAC_PCU_TX_ANT_1
 0280B4h 4 MAC_PCU_TX_ANT_2
 0280B8h 4 MAC_PCU_TX_ANT_3
 0280BCh 4 MAC_PCU_TX_ANT_4
 0280C0h 4 MAC_PCU_XRMODE
 0280C4h 4 MAC_PCU_XRDEL
 0280C8h 4 MAC_PCU_XRTO
 0280CCh 4 MAC_PCU_XRCRP
 0280D0h 4 MAC_PCU_XRSTMP
 0280D4h 1x8 PAD__3
 0280DCh 4 MAC_PCU_SELF_GEN_DEFAULT
 0280E0h 4 MAC_PCU_ADDR1_MASK_L32
 0280E4h 4 MAC_PCU_ADDR1_MASK_U16
 0280E8h 4 MAC_PCU_TPC
 0280ECh 4 MAC_PCU_TX_FRAME_CNT
 0280F0h 4 MAC_PCU_RX_FRAME_CNT
 0280F4h 4 MAC_PCU_RX_CLEAR_CNT
 0280F8h 4 MAC_PCU_CYCLE_CNT
 0280FCh 4 MAC_PCU_QUIET_TIME_1
 028100h 4 MAC_PCU_QUIET_TIME_2
 028104h 1x4 PAD__4
 028108h 4 MAC_PCU_QOS_NO_ACK
 02810Ch 4 MAC_PCU_PHY_ERROR_MASK
 028110h 4 MAC_PCU_XRLAT
 028114h 4 MAC_PCU_RXBUF
 028118h 4 MAC_PCU_MIC_QOS_CONTROL
 02811Ch 4 MAC_PCU_MIC_QOS_SELECT
 028120h 4 MAC_PCU_MISC_MODE
 028124h 4 MAC_PCU_FILTER_OFDM_CNT
 028128h 4 MAC_PCU_FILTER_CCK_CNT
 02812Ch 4 MAC_PCU_PHY_ERR_CNT_1
 028130h 4 MAC_PCU_PHY_ERR_CNT_1_MASK
 028134h 4 MAC_PCU_PHY_ERR_CNT_2
 028138h 4 MAC_PCU_PHY_ERR_CNT_2_MASK
 02813Ch 1x8 PAD__5
 028144h 4 MAC_PCU_PHY_ERROR_EIFS_MASK
 028148h 1x8 PAD__6
 028150h 4 MAC_PCU_COEX_LNAMAXGAIN1
 028154h 4 MAC_PCU_COEX_LNAMAXGAIN2
 028158h 4 MAC_PCU_COEX_LNAMAXGAIN3
 02815Ch 4 MAC_PCU_COEX_LNAMAXGAIN4
 028160h 1x8 PAD__7
 028168h 4 MAC_PCU_PHY_ERR_CNT_3
 02816Ch 4 MAC_PCU_PHY_ERR_CNT_3_MASK
 028170h 4 MAC_PCU_BLUETOOTH_MODE
 028174h 1x4 PAD__8
 028178h 4 MAC_PCU_HCF_TIMEOUT
 02817Ch 4 MAC_PCU_BLUETOOTH_MODE2
 028180h 1x72 PAD__9
 0281C8h 4 MAC_PCU_BLUETOOTH_TSF_BT_ACTIVE
 0281CCh 4 MAC_PCU_BLUETOOTH_TSF_BT_PRIORITY
 0281D0h 4 MAC_PCU_TXSIFS
 0281D4h 4 MAC_PCU_BLUETOOTH_MODE3
 0281D8h 4 MAC_PCU_BLUETOOTH_MODE4
 0281DCh 4 MAC_PCU_BLUETOOTH_MODE5
 0281E0h 4 MAC_PCU_BLUETOOTH_WEIGHTS
 0281E4h 4 MAC_PCU_BT_BT_ASYNC
 0281E8h 1x4 PAD__10
 0281ECh 4 MAC_PCU_TXOP_X
 0281F0h 4 MAC_PCU_TXOP_0_3
 0281F4h 4 MAC_PCU_TXOP_4_7
 0281F8h 4 MAC_PCU_TXOP_8_11
 0281FCh 4 MAC_PCU_TXOP_12_15
 028200h 4 MAC_PCU_TDMA_TXFRAME_START_TIME_TRIGGER_LSB
 028204h 4 MAC_PCU_TDMA_TXFRAME_START_TIME_TRIGGER_MSB
 028208h 4 MAC_PCU_TDMA_SLOT_ALERT_CNTL
 02820Ch 1x80 PAD__11
 02825Ch 4 MAC_PCU_WOW1 ;WOW Misc
 028260h 4 MAC_PCU_WOW2 ;WOW AIFS/SLOT/TRY_CNT
 028264h 4 MAC_PCU_LOGIC_ANALYZER
 028268h 4 MAC_PCU_LOGIC_ANALYZER_32L
 02826Ch 4 MAC_PCU_LOGIC_ANALYZER_16U
 028270h 4 MAC_PCU_WOW3_BEACON_FAIL ;WOW Beacon Fail Enable
 028274h 4 MAC_PCU_WOW3_BEACON ;WOW Beacon Timeout
 028278h 4 MAC_PCU_WOW3_KEEP_ALIVE ;WOW Keep-Alive Timeout
 02827Ch 4 MAC_PCU_WOW_KA ;WOW Auto/Fail/BkoffCs Enable/Disable
 028280h 1x4 PAD__12
 028284h 4 PCU_1US
 028288h 4 PCU_KA
 02828Ch 4 WOW_EXACT ;WOW Exact Length/Offset 1
 028290h 1x4 PAD__13
 028294h 4 PCU_WOW4 ;WOW Offset 0..3
 028298h 4 PCU_WOW5 ;WOW Offset 4..7
 02829Ch 4 MAC_PCU_PHY_ERR_CNT_MASK_CONT
 0282A0h 1x96 PAD__14
 028300h 4 MAC_PCU_AZIMUTH_MODE
 028304h 1x16 PAD__15
 028314h 4 MAC_PCU_AZIMUTH_TIME_STAMP
 028318h 4 MAC_PCU_20_40_MODE
 02831Ch 4 MAC_PCU_H_XFER_TIMEOUT
 028320h 1x8 PAD__16
 028328h 4 MAC_PCU_RX_CLEAR_DIFF_CNT
 02832Ch 4 MAC_PCU_SELF_GEN_ANTENNA_MASK
 028330h 4 MAC_PCU_BA_BAR_CONTROL
 028334h 4 MAC_PCU_LEGACY_PLCP_SPOOF
 028338h 4 MAC_PCU_PHY_ERROR_MASK_CONT
 02833Ch 4 MAC_PCU_TX_TIMER
 028340h 4 MAC_PCU_TXBUF_CTRL
 028344h 4 MAC_PCU_MISC_MODE2
 028348h 4 MAC_PCU_ALT_AES_MUTE_MASK
 02834Ch 4 MAC_PCU_WOW6 ;;WOW RX Buf Start Addr (R)
 028350h 4 ASYNC_FIFO_REG1
 028354h 4 ASYNC_FIFO_REG2
 028358h 4 ASYNC_FIFO_REG3
 02835Ch 4 MAC_PCU_WOW5 ;WOW RX Abort Enable
 028360h 4 MAC_PCU_WOW_LENGTH1 ;WOW Pattern 0..3
 028364h 4 MAC_PCU_WOW_LENGTH2 ;WOW Pattern 4..7
 028368h 4 WOW_PATTERN_MATCH_LESS_THAN_256_BYTES
 02836Ch 1x4 PAD__17
 028370h 4 MAC_PCU_WOW4 ;WOW Pattern Enable/Detect
 028374h 4 WOW2_EXACT ;WOW Exact Length/Offset 2 ;\
 028378h 4 PCU_WOW6 ;WOW Offset 8..11 ;
 02837Ch 4 PCU_WOW7 ;WOW Offset 12..15 ;
 028380h 4 MAC_PCU_WOW_LENGTH3 ;WOW Pattern 8..11 ;
 028384h 4 MAC_PCU_WOW_LENGTH4 ;WOW Pattern 12..15 ;/
 028388h 4 MAC_PCU_LOCATION_MODE_CONTROL
 02838Ch 4 MAC_PCU_LOCATION_MODE_TIMER
 028390h 1x8 PAD__18
 028398h 4 MAC_PCU_BSSID2_L32
 02839Ch 4 MAC_PCU_BSSID2_U16
 0283A0h 4 MAC_PCU_DIRECT_CONNECT
 0283A4h 4 MAC_PCU_TID_TO_AC
 0283A8h 4 MAC_PCU_HP_QUEUE
 0283ACh 1x16 PAD__19
 0283BCh 4 MAC_PCU_AGC_SATURATION_CNT0
 0283C0h 4 MAC_PCU_AGC_SATURATION_CNT1
 0283C4h 4 MAC_PCU_AGC_SATURATION_CNT2
 0283C8h 4 MAC_PCU_HW_BCN_PROC1
 0283CCh 4 MAC_PCU_HW_BCN_PROC2
 0283D0h 4 MAC_PCU_MISC_MODE3
 0283D4h 4 MAC_PCU_MISC_MODE4
 0283D8h 1x4 PAD__20
 0283DCh 4 MAC_PCU_PS_FILTER
 0283E0h 4 MAC_PCU_BASIC_RATE_SET0
 0283E4h 4 MAC_PCU_BASIC_RATE_SET1
 0283E8h 4 MAC_PCU_BASIC_RATE_SET2
 0283ECh 4 MAC_PCU_BASIC_RATE_SET3
 0283F0h 1x16 PAD__21
 028400h 4x64 MAC_PCU_TXBUF_BA[0..63]
 028500h 4x64 MAC_PCU_BT_BT[0..63]
 028600h 4 MAC_PCU_RX_INT_STATUS0
 028604h 4 MAC_PCU_RX_INT_STATUS1
 028608h 4 MAC_PCU_RX_INT_STATUS2
 02860Ch 4 MAC_PCU_RX_INT_STATUS3
 028610h 4 HT_HALF_GI_RATE1
 028614h 4 HT_HALF_GI_RATE2
 028618h 4 HT_FULL_GI_RATE1
 02861Ch 4 HT_FULL_GI_RATE2
 028620h 4 LEGACY_RATE1
 028624h 4 LEGACY_RATE2
 028628h 4 LEGACY_RATE3
 02862Ch 4 RX_INT_FILTER
 028630h 4 RX_INT_OVERFLOW
 028634h 4 RX_FILTER_THRESH0
 028638h 4 RX_FILTER_THRESH1
 02863Ch 4 RX_PRIORITY_THRESH0
 028640h 4 RX_PRIORITY_THRESH1
 028644h 4 RX_PRIORITY_THRESH2
 028648h 4 RX_PRIORITY_THRESH3
 02864Ch 4 RX_PRIORITY_OFFSET0
 028650h 4 RX_PRIORITY_OFFSET1
 028654h 4 RX_PRIORITY_OFFSET2
 028658h 4 RX_PRIORITY_OFFSET3
 02865Ch 4 RX_PRIORITY_OFFSET4
 028660h 4 RX_PRIORITY_OFFSET5
 028664h 4 MAC_PCU_LAST_BEACON2_TSF
 028668h 4 MAC_PCU_PHY_ERROR_AIFS
 02866Ch 4 MAC_PCU_PHY_ERROR_AIFS_MASK
 028670h 4 MAC_PCU_FILTER_RSSI_AVE
 028674h 4 MAC_PCU_TBD_FILTER
 028678h 4 MAC_PCU_BT_ANT_SLEEP_EXTEND
 02867Ch 1x388 PAD__22
 028800h 4x512 MAC_PCU_KEY_CACHE[0..511]
 029000h 1x20480 PAD__23 ;<-- this includes BB regs
 02E000h 4x2048 MAC_PCU_BUF[0..2047] ;<-- this after BB regs |
| --- |

**bb_reg.h (1) - bb_chn_reg_map (hw6.0)**

```

| 029800h 4 BB_TIMING_CONTROLS_1
 029804h 4 BB_TIMING_CONTROLS_2
 029808h 4 BB_TIMING_CONTROLS_3
 02980Ch 4 BB_TIMING_CONTROL_4
 029810h 4 BB_TIMING_CONTROL_5
 029814h 4 BB_TIMING_CONTROL_6
 029818h 4 BB_TIMING_CONTROL_11
 02981Ch 4 BB_SPUR_MASK_CONTROLS
 029820h 4 BB_FIND_SIGNAL_LOW
 029824h 4 BB_SFCORR
 029828h 4 BB_SELF_CORR_LOW
 02982Ch 4 BB_EXT_CHAN_SCORR_THR
 029830h 4 BB_EXT_CHAN_PWR_THR_2_B0
 029834h 4 BB_RADAR_DETECTION
 029838h 4 BB_RADAR_DETECTION_2
 02983Ch 4 BB_EXTENSION_RADAR
 029840h 1x64 PAD__0
 029880h 4 BB_MULTICHAIN_CONTROL
 029884h 4 BB_PER_CHAIN_CSD
 029888h 1x24 PAD__1
 0298A0h 4 BB_TX_CRC
 0298A4h 4 BB_TSTDAC_CONSTANT
 0298A8h 4 BB_SPUR_REPORT_B0
 0298ACh 1x4 PAD__2
 0298B0h 4 BB_TXIQCAL_CONTROL_3
 0298B4h 1x8 PAD__3
 0298BCh 4 BB_GREEN_TX_CONTROL_1
 0298C0h 4 BB_IQ_ADC_MEAS_0_B0
 0298C4h 4 BB_IQ_ADC_MEAS_1_B0
 0298C8h 4 BB_IQ_ADC_MEAS_2_B0
 0298CCh 4 BB_IQ_ADC_MEAS_3_B0
 0298D0h 4 BB_TX_PHASE_RAMP_B0
 0298D4h 4 BB_ADC_GAIN_DC_CORR_B0
 0298D8h 1x4 PAD__4
 0298DCh 4 BB_RX_IQ_CORR_B0
 0298E0h 1x4 PAD__5
 0298E4h 4 BB_PAPRD_AM2AM_MASK
 0298E8h 4 BB_PAPRD_AM2PM_MASK
 0298ECh 4 BB_PAPRD_HT40_MASK
 0298F0h 4 BB_PAPRD_CTRL0_B0
 0298F4h 4 BB_PAPRD_CTRL1_B0
 0298F8h 4 BB_PA_GAIN123_B0
 0298FCh 4 BB_PA_GAIN45_B0
 029900h 4 BB_PAPRD_PRE_POST_SCALE_0_B0
 029904h 4 BB_PAPRD_PRE_POST_SCALE_1_B0
 029908h 4 BB_PAPRD_PRE_POST_SCALE_2_B0
 02990Ch 4 BB_PAPRD_PRE_POST_SCALE_3_B0
 029910h 4 BB_PAPRD_PRE_POST_SCALE_4_B0
 029914h 4 BB_PAPRD_PRE_POST_SCALE_5_B0
 029918h 4 BB_PAPRD_PRE_POST_SCALE_6_B0
 02991Ch 4 BB_PAPRD_PRE_POST_SCALE_7_B0
 029920h 4x120 BB_PAPRD_MEM_TAB_B0[0..119]
 029B00h 4x60 BB_CHAN_INFO_CHAN_TAB_B0[0..59]
 029BF0h 4 BB_CHN_TABLES_INTF_ADDR
 029BF4h 4 BB_CHN_TABLES_INTF_DATA |
| --- |

**bb_reg.h (2) - bb_mrc_reg_map (hw6.0)**

```

| 029C00h 4 BB_TIMING_CONTROL_3A
 029C04h 4 BB_LDPC_CNTL1
 029C08h 4 BB_LDPC_CNTL2
 029C0Ch 4 BB_PILOT_SPUR_MASK
 029C10h 4 BB_CHAN_SPUR_MASK
 029C14h 4 BB_SHORT_GI_DELTA_SLOPE
 029C18h 4 BB_ML_CNTL1
 029C1Ch 4 BB_ML_CNTL2
 029C20h 4 BB_TSTADC |
| --- |

**bb_reg.h (3) - bb_bbb_reg_map (hw6.0)**

```

| 029D00h 4 BB_BBB_RX_CTRL_1
 029D04h 4 BB_BBB_RX_CTRL_2
 029D08h 4 BB_BBB_RX_CTRL_3
 029D0Ch 4 BB_BBB_RX_CTRL_4
 029D10h 4 BB_BBB_RX_CTRL_5
 029D14h 4 BB_BBB_RX_CTRL_6
 029D18h 4 BB_FORCE_CLKEN_CCK |
| --- |

**bb_reg.h (4) - bb_agc_reg_map (hw6.0)**

```

| 029E00h 4 BB_SETTLING_TIME
 029E04h 4 BB_GAIN_FORCE_MAX_GAINS_B0
 029E08h 4 BB_GAINS_MIN_OFFSETS
 029E0Ch 4 BB_DESIRED_SIGSIZE
 029E10h 4 BB_FIND_SIGNAL
 029E14h 4 BB_AGC
 029E18h 4 BB_EXT_ATTEN_SWITCH_CTL_B0
 029E1Ch 4 BB_CCA_B0
 029E20h 4 BB_CCA_CTRL_2_B0
 029E24h 4 BB_RESTART
 029E28h 4 BB_MULTICHAIN_GAIN_CTRL
 029E2Ch 4 BB_EXT_CHAN_PWR_THR_1
 029E30h 4 BB_EXT_CHAN_DETECT_WIN
 029E34h 4 BB_PWR_THR_20_40_DET
 029E38h 4 BB_RIFS_SRCH
 029E3Ch 4 BB_PEAK_DET_CTRL_1
 029E40h 4 BB_PEAK_DET_CTRL_2
 029E44h 4 BB_RX_GAIN_BOUNDS_1
 029E48h 4 BB_RX_GAIN_BOUNDS_2
 029E4Ch 4 BB_PEAK_DET_CAL_CTRL
 029E50h 4 BB_AGC_DIG_DC_CTRL
 029E54h 4 BB_BT_COEX_1
 029E58h 4 BB_BT_COEX_2
 029E5Ch 4 BB_BT_COEX_3
 029E60h 4 BB_BT_COEX_4
 029E64h 4 BB_BT_COEX_5
 029E68h 4 BB_REDPWR_CTRL_1
 029E6Ch 4 BB_REDPWR_CTRL_2
 029E70h 1x272 PAD__0
 029F80h 4 BB_RSSI_B0
 029F84h 4 BB_SPUR_EST_CCK_REPORT_B0
 029F88h 4 BB_AGC_DIG_DC_STATUS_I_B0
 029F8Ch 4 BB_AGC_DIG_DC_STATUS_Q_B0
 029F90h 4 BB_DC_CAL_STATUS_B0
 029F94h 1x44 PAD__1
 029FC0h 4 BB_BBB_SIG_DETECT
 029FC4h 4 BB_BBB_DAGC_CTRL
 029FC8h 4 BB_IQCORR_CTRL_CCK
 029FCCh 4 BB_CCK_SPUR_MIT
 029FD0h 4 BB_MRC_CCK_CTRL
 029FD4h 4 BB_CCK_BLOCKER_DET
 029FD8h 1x40 PAD__2
 02A000h 4x128 BB_RX_OCGAIN[0..127] |
| --- |

**bb_reg.h (5) - bb_sm_reg_map (hw6.0)**

```

| 02A200h 4 BB_D2_CHIP_ID
 02A204h 4 BB_GEN_CONTROLS
 02A208h 4 BB_MODES_SELECT
 02A20Ch 4 BB_ACTIVE
 02A210h 1x16 PAD__0
 02A220h 4 BB_VIT_SPUR_MASK_A
 02A224h 4 BB_VIT_SPUR_MASK_B
 02A228h 4 BB_SPECTRAL_SCAN
 02A22Ch 4 BB_RADAR_BW_FILTER
 02A230h 4 BB_SEARCH_START_DELAY
 02A234h 4 BB_MAX_RX_LENGTH
 02A238h 4 BB_FRAME_CONTROL
 02A23Ch 4 BB_RFBUS_REQUEST
 02A240h 4 BB_RFBUS_GRANT
 02A244h 4 BB_RIFS
 02A248h 4 BB_SPECTRAL_SCAN_2
 02A24Ch 1x4 PAD__1
 02A250h 4 BB_RX_CLEAR_DELAY
 02A254h 4 BB_ANALOG_POWER_ON_TIME
 02A258h 4 BB_TX_TIMING_1
 02A25Ch 4 BB_TX_TIMING_2
 02A260h 4 BB_TX_TIMING_3
 02A264h 4 BB_XPA_TIMING_CONTROL
 02A268h 1x24 PAD__2
 02A280h 4 BB_MISC_PA_CONTROL
 02A284h 4 BB_SWITCH_TABLE_CHN_B0
 02A288h 4 BB_SWITCH_TABLE_COM1
 02A28Ch 4 BB_SWITCH_TABLE_COM2
 02A290h 1x16 PAD__3
 02A2A0h 4 BB_MULTICHAIN_ENABLE
 02A2A4h 1x28 PAD__4
 02A2C0h 4 BB_CAL_CHAIN_MASK
 02A2C4h 4 BB_AGC_CONTROL
 02A2C8h 4 BB_IQ_ADC_CAL_MODE
 02A2CCh 4 BB_FCAL_1
 02A2D0h 4 BB_FCAL_2_B0
 02A2D4h 4 BB_DFT_TONE_CTRL_B0
 02A2D8h 4 BB_CL_CAL_CTRL
 02A2DCh 4 BB_CL_MAP_0_B0
 02A2E0h 4 BB_CL_MAP_1_B0
 02A2E4h 4 BB_CL_MAP_2_B0
 02A2E8h 4 BB_CL_MAP_3_B0
 02A2ECh 4 BB_CL_MAP_PAL_0_B0
 02A2F0h 4 BB_CL_MAP_PAL_1_B0
 02A2F4h 4 BB_CL_MAP_PAL_2_B0
 02A2F8h 4 BB_CL_MAP_PAL_3_B0
 02A2FCh 1x4 PAD__5
 02A300h 4x16 BB_CL_TAB_B0[0..15]
 02A340h 4 BB_SYNTH_CONTROL
 02A344h 4 BB_ADDAC_CLK_SELECT
 02A348h 4 BB_PLL_CNTL
 02A34Ch 4 BB_ANALOG_SWAP
 02A350h 4 BB_ADDAC_PARALLEL_CONTROL
 02A354h 1x4 PAD__6
 02A358h 4 BB_FORCE_ANALOG
 02A35Ch 1x4 PAD__7
 02A360h 4 BB_TEST_CONTROLS
 02A364h 4 BB_TEST_CONTROLS_STATUS
 02A368h 4 BB_TSTDAC
 02A36Ch 4 BB_CHANNEL_STATUS
 02A370h 4 BB_CHANINFO_CTRL
 02A374h 4 BB_CHAN_INFO_NOISE_PWR
 02A378h 4 BB_CHAN_INFO_GAIN_DIFF
 02A37Ch 4 BB_CHAN_INFO_FINE_TIMING
 02A380h 4 BB_CHAN_INFO_GAIN_B0
 02A384h 4 BB_SM_HIST
 02A388h 1x8 PAD__8
 02A390h 4 BB_SCRAMBLER_SEED
 02A394h 4 BB_BBB_TX_CTRL
 02A398h 4 BB_BBB_TXFIR_0
 02A39Ch 4 BB_BBB_TXFIR_1
 02A3A0h 4 BB_BBB_TXFIR_2
 02A3A4h 4 BB_HEAVY_CLIP_CTRL
 02A3A8h 4 BB_HEAVY_CLIP_20
 02A3ACh 4 BB_HEAVY_CLIP_40
 02A3B0h 4 BB_ILLEGAL_TX_RATE
 02A3B4h 1x12 PAD__9
 02A3C0h 4 BB_POWERTX_RATE1 ;Power TX 0..3
 02A3C4h 4 BB_POWERTX_RATE2 ;Power TX 4..7
 02A3C8h 4 BB_POWERTX_RATE3 ;Power TX 1L,2L,2S
 02A3CCh 4 BB_POWERTX_RATE4 ;Power TX 55L,55S,11L,11S
 02A3D0h 4 BB_POWERTX_RATE5 ;Power TX HT20_0..3
 02A3D4h 4 BB_POWERTX_RATE6 ;Power TX HT20_4..7
 02A3D8h 4 BB_POWERTX_RATE7 ;Power TX HT40_0..3
 02A3DCh 4 BB_POWERTX_RATE8 ;Power TX HT40_4..7
 02A3E0h 4 BB_POWERTX_RATE9 ;Power TX DUP40/EXT20_CCK/OFDM
 02A3E4h 4 BB_POWERTX_RATE10 ;Power TX HT20_8..11
 02A3E8h 4 BB_POWERTX_RATE11 ;Power TX HT20/40_12/13
 02A3ECh 4 BB_POWERTX_RATE12 ;Power TX HT40_8..11
 02A3F0h 4 BB_POWERTX_MAX ;Power TX Flags
 02A3F4h 4 BB_POWERTX_SUB ;Power TX Sub_for_2chain
 02A3F8h 4 BB_TPC_1
 02A3FCh 4 BB_TPC_2
 02A400h 4 BB_TPC_3
 02A404h 4 BB_TPC_4_B0
 02A408h 4 BB_TPC_5_B0
 02A40Ch 4 BB_TPC_6_B0
 02A410h 4 BB_TPC_7
 02A414h 4 BB_TPC_8
 02A418h 4 BB_TPC_9
 02A41Ch 4 BB_TPC_10
 02A420h 4 BB_TPC_11_B0
 02A424h 4 BB_TPC_12
 02A428h 4 BB_TPC_13
 02A42Ch 4 BB_TPC_14
 02A430h 4 BB_TPC_15
 02A434h 4 BB_TPC_16
 02A438h 4 BB_TPC_17
 02A43Ch 4 BB_TPC_18
 02A440h 4 BB_TPC_19_B0
 02A444h 4 BB_TPC_20
 02A448h 4 BB_THERM_ADC_1
 02A44Ch 4 BB_THERM_ADC_2
 02A450h 4 BB_THERM_ADC_3
 02A454h 4 BB_THERM_ADC_4
 02A458h 4 BB_TX_FORCED_GAIN
 02A45Ch 1x36 PAD__10
 02A480h 4x32 BB_PDADC_TAB_B0[0..31]
 02A500h 4x32 BB_TX_GAIN_TAB_(1..32)
 02A580h 4 BB_RTT_CTRL
 02A584h 4 BB_RTT_TABLE_SW_INTF_B0
 02A588h 4 BB_RTT_TABLE_SW_INTF_1_B0
 02A58Ch 4 BB_TX_GAIN_TAB_1_16_LSB_EXT
 02A590h 4 BB_TX_GAIN_TAB_17_32_LSB_EXT
 02A594h 1x108 PAD__11
 02A600h 4x16 BB_CALTX_GAIN_SET_(0,2,4,6,..,28,30)
 02A640h 1x4 PAD__12
 02A644h 4 BB_TXIQCAL_CONTROL_0
 02A648h 4 BB_TXIQCAL_CONTROL_1
 02A64Ch 4 BB_TXIQCAL_CONTROL_2
 02A650h 4 BB_TXIQ_CORR_COEFF_01_B0
 02A654h 4 BB_TXIQ_CORR_COEFF_23_B0
 02A658h 4 BB_TXIQ_CORR_COEFF_45_B0
 02A65Ch 4 BB_TXIQ_CORR_COEFF_67_B0
 02A660h 4 BB_TXIQ_CORR_COEFF_89_B0
 02A664h 4 BB_TXIQ_CORR_COEFF_AB_B0
 02A668h 4 BB_TXIQ_CORR_COEFF_CD_B0
 02A66Ch 4 BB_TXIQ_CORR_COEFF_EF_B0
 02A670h 4 BB_CAL_RXBB_GAIN_TBL_0
 02A674h 4 BB_CAL_RXBB_GAIN_TBL_4
 02A678h 4 BB_CAL_RXBB_GAIN_TBL_8
 02A67Ch 4 BB_CAL_RXBB_GAIN_TBL_12
 02A680h 4 BB_CAL_RXBB_GAIN_TBL_16
 02A684h 4 BB_CAL_RXBB_GAIN_TBL_20
 02A688h 4 BB_CAL_RXBB_GAIN_TBL_24
 02A68Ch 4 BB_TXIQCAL_STATUS_B0
 02A690h 4 BB_PAPRD_TRAINER_CNTL1
 02A694h 4 BB_PAPRD_TRAINER_CNTL2
 02A698h 4 BB_PAPRD_TRAINER_CNTL3
 02A69Ch 4 BB_PAPRD_TRAINER_CNTL4
 02A6A0h 4 BB_PAPRD_TRAINER_STAT1
 02A6A4h 4 BB_PAPRD_TRAINER_STAT2
 02A6A8h 4 BB_PAPRD_TRAINER_STAT3
 02A6ACh 1x276 PAD__13
 02A7C0h 4 BB_WATCHDOG_STATUS
 02A7C4h 4 BB_WATCHDOG_CTRL_1
 02A7C8h 4 BB_WATCHDOG_CTRL_2
 02A7CCh 4 BB_BLUETOOTH_CNTL
 02A7D0h 4 BB_PHYONLY_WARM_RESET
 02A7D4h 4 BB_PHYONLY_CONTROL
 02A7D8h 1x4 PAD__14
 02A7DCh 4 BB_ECO_CTRL
 02A7E0h 1x16 PAD__15
 02A7F0h 4 BB_TABLES_INTF_ADDR_B0
 02A7F4h 4 BB_TABLES_INTF_DATA_B0 |
| --- |

**bb_reg.h (6) - bb_chn1_reg_map (hw6.0)**

```

| 02A800h 1x48 PAD__0
 02A830h 4 BB_EXT_CHAN_PWR_THR_2_B1
 02A834h 1x116 PAD__1
 02A8A8h 4 BB_SPUR_REPORT_B1
 02A8ACh 1x20 PAD__2
 02A8C0h 4 BB_IQ_ADC_MEAS_0_B1
 02A8C4h 4 BB_IQ_ADC_MEAS_1_B1
 02A8C8h 4 BB_IQ_ADC_MEAS_2_B1
 02A8CCh 4 BB_IQ_ADC_MEAS_3_B1
 02A8D0h 4 BB_TX_PHASE_RAMP_B1
 02A8D4h 4 BB_ADC_GAIN_DC_CORR_B1
 02A8D8h 1x4 PAD__3
 02A8DCh 4 BB_RX_IQ_CORR_B1
 02A8E0h 1x16 PAD__4
 02A8F0h 4 BB_PAPRD_CTRL0_B1
 02A8F4h 4 BB_PAPRD_CTRL1_B1
 02A8F8h 4 BB_PA_GAIN123_B1
 02A8FCh 4 BB_PA_GAIN45_B1
 02A900h 4 BB_PAPRD_PRE_POST_SCALE_0_B1
 02A904h 4 BB_PAPRD_PRE_POST_SCALE_1_B1
 02A908h 4 BB_PAPRD_PRE_POST_SCALE_2_B1
 02A90Ch 4 BB_PAPRD_PRE_POST_SCALE_3_B1
 02A910h 4 BB_PAPRD_PRE_POST_SCALE_4_B1
 02A914h 4 BB_PAPRD_PRE_POST_SCALE_5_B1
 02A918h 4 BB_PAPRD_PRE_POST_SCALE_6_B1
 02A91Ch 4 BB_PAPRD_PRE_POST_SCALE_7_B1
 02A920h 4x120 BB_PAPRD_MEM_TAB_B1[0..119]
 02AB00h 4x60 BB_CHAN_INFO_CHAN_TAB_B1[0..59]
 02ABF0h 4 BB_CHN1_TABLES_INTF_ADDR
 02ABF4h 4 BB_CHN1_TABLES_INTF_DATA |
| --- |

**bb_reg.h (7) - bb_agc1_reg_map (hw6.0)**

```

| 02AE00h 1x4 PAD__0
 02AE04h 4 BB_GAIN_FORCE_MAX_GAINS_B1
 02AE08h 1x16 PAD__1
 02AE18h 4 BB_EXT_ATTEN_SWITCH_CTL_B1
 02AE1Ch 4 BB_CCA_B1
 02AE20h 4 BB_CCA_CTRL_2_B1
 02AE24h 1x348 PAD__2
 02AF80h 4 BB_RSSI_B1
 02AF84h 4 BB_SPUR_EST_CCK_REPORT_B1
 02AF88h 4 BB_AGC_DIG_DC_STATUS_I_B1
 02AF8Ch 4 BB_AGC_DIG_DC_STATUS_Q_B1
 02AF90h 4 BB_DC_CAL_STATUS_B1
 02AF94h 1x108 PAD__3
 02B000h 4x128 BB_RX_OCGAIN2[0..127] |
| --- |

**bb_reg.h (8) - bb_sm1_reg_map (hw6.0)**

```

| 02B200h 1x132 PAD__0
 02B284h 4 BB_SWITCH_TABLE_CHN_B1
 02B288h 1x72 PAD__1
 02B2D0h 4 BB_FCAL_2_B1
 02B2D4h 4 BB_DFT_TONE_CTRL_B1
 02B2D8h 1x4 PAD__2
 02B2DCh 4 BB_CL_MAP_0_B1
 02B2E0h 4 BB_CL_MAP_1_B1
 02B2E4h 4 BB_CL_MAP_2_B1
 02B2E8h 4 BB_CL_MAP_3_B1
 02B2ECh 4 BB_CL_MAP_PAL_0_B1
 02B2F0h 4 BB_CL_MAP_PAL_1_B1
 02B2F4h 4 BB_CL_MAP_PAL_2_B1
 02B2F8h 4 BB_CL_MAP_PAL_3_B1
 02B2FCh 1x4 PAD__3
 02B300h 4x16 BB_CL_TAB_B1[0..15]
 02B340h 1x64 PAD__4
 02B380h 4 BB_CHAN_INFO_GAIN_B1
 02B384h 1x128 PAD__5
 02B404h 4 BB_TPC_4_B1
 02B408h 4 BB_TPC_5_B1
 02B40Ch 4 BB_TPC_6_B1
 02B410h 1x16 PAD__6
 02B420h 4 BB_TPC_11_B1
 02B424h 1x28 PAD__7
 02B440h 4 BB_TPC_19_B1
 02B444h 1x60 PAD__8
 02B480h 4x32 BB_PDADC_TAB_B1[0..31]
 02B500h 1x132 PAD__9
 02B584h 4 BB_RTT_TABLE_SW_INTF_B1
 02B588h 4 BB_RTT_TABLE_SW_INTF_1_B1
 02B58Ch 1x196 PAD__10
 02B650h 4 BB_TXIQ_CORR_COEFF_01_B1
 02B654h 4 BB_TXIQ_CORR_COEFF_23_B1
 02B658h 4 BB_TXIQ_CORR_COEFF_45_B1
 02B65Ch 4 BB_TXIQ_CORR_COEFF_67_B1
 02B660h 4 BB_TXIQ_CORR_COEFF_89_B1
 02B664h 4 BB_TXIQ_CORR_COEFF_AB_B1
 02B668h 4 BB_TXIQ_CORR_COEFF_CD_B1
 02B66Ch 4 BB_TXIQ_CORR_COEFF_EF_B1
 02B670h 1x28 PAD__11
 02B68Ch 4 BB_TXIQCAL_STATUS_B1
 02B690h 1x352 PAD__12
 02B7F0h 4 BB_TABLES_INTF_ADDR_B1
 02B7F4h 4 BB_TABLES_INTF_DATA_B1 |
| --- |

**bb_reg.h (9) - bb_chn3_reg_map (hw6.0)**

```

| 02C800h 4x256 BB_DUMMY1[0..255] |
| --- |

**bb_reg.h (10) - bb_agc3_reg_map (hw6.0)**

```

| 02CE00h 4 BB_DUMMY
 02CE04h 1x380 PAD__0
 02CF80h 4 BB_RSSI_B3 |
| --- |

**bb_reg.h (11) - bb_sm3_reg_map (hw6.0)**

```

| 02D200h 4x384 BB_DUMMY2[0..383] |
| --- |

**bb_reg.h (12) - mit_local_reg_map, aka bb_mit_reg_map (hw6.0)**

```

| 02D800h 4 BB_MIT_RF_CNTL
 02D804h 4 BB_MIT_CCA_CNTL
 02D808h 4 BB_MIT_RSSI_CNTL_1
 02D80Ch 4 BB_MIT_RSSI_CNTL_2
 02D810h 4 BB_MIT_TX_CNTL
 02D814h 4 BB_MIT_RX_CNTL
 02D818h 4 BB_MIT_OUT_CNTL
 02D81Ch 4 BB_MIT_SPARE_CNTL |
| --- |

**wmac_pcu_reg.h (2) (hw6.0)**

```

| 02E000h 4x2048 MAC_PCU_BUF[0..2047] |
| --- |

**efuse_wlan_reg.h (hw6.0)**

```

| 030000h 4 EFUSE_WR_ENABLE_REG
 030004h 4 EFUSE_INT_ENABLE_REG
 030008h 4 EFUSE_INT_STATUS_REG
 03000Ch 4 BITMASK_WR_REG
 030010h 4 VDDQ_SETTLE_TIME_REG
 030014h 4 VDDQ_HOLD_TIME_REG
 030018h 4 RD_STROBE_PW_REG
 03001Ch 4 PG_STROBE_PW_REG
 030020h 4 PGENB_SETUP_HOLD_TIME_REG
 030024h 4 STROBE_PULSE_INTERVAL_REG
 030028h 4 CSB_ADDR_LOAD_SETUP_HOLD_REG
 03002Ch 1x2004 PAD0
 030800h 4x512 EFUSE_INTF0[0..511]
 031000h 4x512 EFUSE_INTF1[0..511] |
| --- |

**stereo_reg.h (hw6.0)**

```

| 034000h 4 STEREO0_CONFIG ;\Stereo 0
 034004h 4 STEREO0_VOLUME ;/
 034008h 4 STEREO_MASTER_CLOCK ;-Stereo Master
 03400Ch 4 STEREO0_TX_SAMPLE_CNT_LSB ;\
 034010h 4 STEREO0_TX_SAMPLE_CNT_MSB ; Stereo 0
 034014h 4 STEREO0_RX_SAMPLE_CNT_LSB ;
 034018h 4 STEREO0_RX_SAMPLE_CNT_MSB ;/ |
| --- |

**chk_sum_seg_acc_reg.h (hw6.0)**

```

| 035000h 4 CHKSUM_ACC_DMATX_CONTROL0
 035004h 4 CHKSUM_ACC_DMATX_CONTROL1
 035008h 4 CHKSUM_ACC_DMATX_CONTROL2
 03500Ch 4 CHKSUM_ACC_DMATX_CONTROL3
 035010h 4 CHKSUM_ACC_DMATX_DESC0
 035014h 4 CHKSUM_ACC_DMATX_DESC1
 035018h 4 CHKSUM_ACC_DMATX_DESC2
 03501Ch 4 CHKSUM_ACC_DMATX_DESC3
 035020h 4 CHKSUM_ACC_DMATX_DESC_STATUS
 035024h 4 CHKSUM_ACC_DMATX_ARB_CFG
 035028h 4 CHKSUM_ACC_RR_PKTCNT01
 03502Ch 4 CHKSUM_ACC_RR_PKTCNT23
 035030h 4 CHKSUM_ACC_TXST_PKTCNT
 035034h 4 CHKSUM_ACC_DMARX_CONTROL
 035038h 4 CHKSUM_ACC_DMARX_DESC
 03503Ch 4 CHKSUM_ACC_DMARX_DESC_STATUS
 035040h 4 CHKSUM_ACC_INTR
 035044h 4 CHKSUM_ACC_IMASK
 035048h 4 CHKSUM_ACC_ARB_BURST
 03504Ch 1x4 PAD__0
 035050h 4 CHKSUM_ACC_RESET_DMA
 035054h 4 CHKSUM_CONFIG |
| --- |

**mmac_reg.h (hw6.0)**

```

| 038000h 4 RX_FRAME0
 038004h 4 RX_FRAME_0
 038008h 4 RX_FRAME1
 03800Ch 4 RX_FRAME_1
 038010h 4 MMAC_INTERRUPT_RAW
 038014h 4 MMAC_INTERRUPT_EN
 038018h 4 RX_PARAM1
 03801Ch 4 RX_PARAM0
 038020h 4 TX_COMMAND0
 038024h 4 TX_COMMAND
 038028h 4 TX_PARAM
 03802Ch 4 BEACON_PARAM
 038030h 4 BEACON
 038034h 4 TSF_L
 038038h 4 TSF_U |
| --- |

**fpga_reg.h (hw6.0)**

```

| 039000h 4 FPGA_REG1
 039004h 4 FPGA_REG2
 039008h 4 FPGA_REG4 |
| --- |

**bridge_intr_reg.h (hw6.0)**

```

| 040000h 4 INTERRUPT
 040004h 4 INTERRUPT_MASK |
| --- |

**mii_reg.h (hw6.0)**

```

| 040100h 4 MII0_CNTL
 040104h 4 STAT_CNTL |
| --- |

**mdio_reg.h (hw6.0)**

```

| 040200h 4x8 MDIO_REG[0..7]
 040220h 4 MDIO_ISR
 040224h 4 PHY_ADDR |
| --- |

**bridge_chain_gmac_0_rx_reg.h (hw6.0)**

```

| 040800h 4 GMAC_RX_0_DESC_START_ADDRESS
 040804h 4 GMAC_RX_0_DMA_START
 040808h 4 GMAC_RX_0_BURST_SIZE
 04080Ch 4 GMAC_RX_0_PKT_OFFSET
 040810h 4 GMAC_RX_0_CHECKSUM
 040814h 4 GMAC_RX_0_DBG_RX
 040818h 4 GMAC_RX_0_DBG_RX_CUR_ADDR
 04081Ch 4 GMAC_RX_0_DATA_SWAP |
| --- |

**bridge_chain_gmac_0_tx_reg.h (hw6.0)**

```

| 040C00h 4 GMAC_TX_0_DESC_START_ADDRESS
 040C04h 4 GMAC_TX_0_DMA_START
 040C08h 4 GMAC_TX_0_INTERRUPT_LIMIT
 040C0Ch 4 GMAC_TX_0_BURST_SIZE
 040C10h 4 GMAC_TX_0_DBG_TX
 040C14h 4 GMAC_TX_0_DBG_TX_CUR_ADDR
 040C18h 4 GMAC_TX_0_DATA_SWAP |
| --- |

**usb_cast_reg.h (hw6.0)**

```

| 054000h 4 ENDP0
 054004h 1x4 PAD0
 054008h 4 OUT1ENDP
 05400Ch 4 IN1ENDP
 054010h 4 OUT2ENDP
 054014h 4 IN2ENDP
 054018h 4 OUT3ENDP
 05401Ch 4 IN3ENDP
 054020h 4 OUT4ENDP
 054024h 4 IN4ENDP
 054028h 4 OUT5ENDP
 05402Ch 4 IN5ENDP
 054030h 1x92 PAD1
 05408Ch 4 USBMODESTATUS
 054090h 1x248 PAD2
 054188h 4 EPIRQ
 05418Ch 4 USBIRQ
 054190h 1x4 PAD3
 054194h 4 EPIEN
 054198h 4 PIEN
 05419Ch 1x8 PAD4
 0541A4h 4 FNCTRL
 0541A8h 1x20 PAD5
 0541BCh 4 OTGREG
 0541C0h 1x12 PAD6
 0541CCh 4 DMASTART
 0541D0h 4 DMASTOP
 0541D4h 1x556 PAD7
 054400h 4 EP0DMAADDR
 054404h 1x28 PAD8
 054420h 4 EP1DMAADDR
 054424h 1x8 PAD9
 05442Ch 4 OUT1DMACTRL
 054430h 1x16 PAD10
 054440h 4 EP2DMAADDR
 054444h 1x8 PAD11
 05444Ch 4 OUT2DMACTRL
 054450h 1x16 PAD12
 054460h 4 EP3DMAADDR
 054464h 1x8 PAD13
 05446Ch 4 OUT3DMACTRL
 054470h 1x16 PAD14
 054480h 4 EP4DMAADDR
 054484h 1x8 PAD15
 05448Ch 4 OUT4DMACTRL
 054490h 1x16 PAD16
 0544A0h 4 EP5DMAADDR
 0544A4h 1x8 PAD17
 0544ACh 4 OUT5DMACTRL
 0544B0h 1x539472 PAD18 ;pad to BASE + 84000h
 0D8000h 4 USB_IP_BASE |
| --- |

**rdma_reg.h (formerly at 00030100h in hw4.0) (hw6.0)**

```

| 054D00h 4 DMA_CONFIG
 054D04h 4 DMA_CONTROL
 054D08h 4 DMA_SRC
 054D0Ch 4 DMA_DEST
 054D10h 4 DMA_LENGTH
 054D14h 4 VMC_BASE
 054D18h 4 INDIRECT_REG
 054D1Ch 4 INDIRECT_RETURN
 054D20h 4x16 RDMA_REGION_(0..15)_
 054DA0h 4 DMA_STATUS
 054DA4h 4 DMA_INT_EN |
| --- |

**athrI2cSlaveApbCsr.h (hw6.0)**

```

| 054E00h 4 I2CFIFOCONTROL
 054E04h 4 I2CFIFOREADPTR
 054E08h 4 I2CFIFOREADUPDATE
 054E0Ch 4 I2CFIFOREADBASEADDR
 054E10h 4 I2CFIFOWRITEPTR
 054E14h 4 I2CFIFOWRITEUPDATE
 054E18h 4 I2CFIFOWRITEBASEADDR
 054E1Ch 4 I2CMEMCONTROL
 054E20h 4 I2CMEMBASEADDR
 054E24h 4 I2CREGREADDATA
 054E28h 4 I2CREGWRITEDATA
 054E2Ch 4 I2CREGCONTROL
 054E30h 4 I2CCSRREADDATA
 054E34h 4 I2CCSRWRITEDATA
 054E38h 4 I2CCSRCONTROL
 054E3Ch 4 I2CFILTERSIZE
 054E40h 4 I2CADDR
 054E44h 4 I2CINT
 054E48h 4 I2CINTEN
 054E4Ch 4 I2CINTCSR |
| --- |

**mbox_i2s_reg.h (hw6.0)**

```

| 055000h 4x1 MBOX_FIFO ;<-- defined as array of ONE word (?)
 055004h 4 MBOX_FIFO_STATUS
 055008h 4 MBOX_DMA_POLICY
 05500Ch 4 MBOX0_DMA_RX_DESCRIPTOR_BASE
 055010h 4 MBOX0_DMA_RX_CONTROL
 055014h 4 MBOX0_DMA_TX_DESCRIPTOR_BASE
 055018h 4 MBOX0_DMA_TX_CONTROL
 05501Ch 4 MBOX_FRAME
 055020h 4 FIFO_TIMEOUT
 055024h 4 MBOX_INT_STATUS
 055028h 4 MBOX_INT_ENABLE
 05502Ch 4 MBOX_FIFO_RESET
 055030h 4 MBOX_DEBUG_CHAIN0
 055034h 4 MBOX_DEBUG_CHAIN1
 055038h 4 MBOX_DEBUG_CHAIN0_SIGNALS
 05503Ch 4 MBOX_DEBUG_CHAIN1_SIGNALS |
| --- |

**map_rf_reg.h (hw6.0)**

```

| xxx000h 4x256 RAM1[0..255]
 xxx400h 4x12 INT_PENDING[0..11]
 xxx430h 4 BB_WR_MASK_0 ;\
 xxx434h 4 BB_WR_MASK_1 ; BB Write Mask 0..3
 xxx438h 4 BB_WR_MASK_2 ;
 xxx43Ch 4 BB_WR_MASK_3 ;/
 xxx440h 4 RF_WR_MASK_0 ;\RF Write Mask 0..1
 xxx444h 4 RF_WR_MASK_1 ;/
 xxx448h 4 BB_RD_MASK_0 ;\
 xxx44Ch 4 BB_RD_MASK_1 ; BB Read Mask 0..3
 xxx450h 4 BB_RD_MASK_2 ;
 xxx454h 4 BB_RD_MASK_3 ;/
 xxx458h 4 RF_RD_MASK_0 ;\RF Read Mask 0..1
 xxx45Ch 4 RF_RD_MASK_1 ;/
 xxx460h 4 INT_SRC
 xxx464h 1x924 PAD__0
 xxx800h 4x128 RAM2[0..127] |
| --- |

**odin_reg.h (hw6.0)**

```

| xxx000h 4 PHY_CTRL0
 xxx004h 4 PHY_CTRL1
 xxx008h 4 PHY_CTRL2
 xxx00Ch 4 PHY_CTRL3
 xxx010h 4 PHY_CTRL4
 xxx014h 4 PHY_CTRL5
 xxx018h 4 PHY_CTRL6
 xxx01Ch 4 PHY_STATUS |
| --- |

| DSi Atheros Wifi - Internal I/O - Unknown and Unused Registers (hw2) |
| --- |

**hw4.0 would have the following extra registers**

```

| 00D000h - DBG_UART Registers ;\don't exist in hw2.0 ?
 00E000h - UMBOX Registers ;/
 020000h - WMAC DMA and IRQ ;\
 020800h - WMAC QCU Queue ; these MIGHT EXIST in hw2.0, too ?
 021000h - WMAC DCU ; (not defined in hw2.0 source code though)
 029800h - BB/LC Registers ;/
 030100h - RDMA Registers ;\don't exist in hw2.0 ?
 031000h - EFUSE Registers ;/ |
| --- |

**some partial hw2.0 memory dump...**

```

| 000000 Deadc0de
 004000 sth (01 00 00 00, 00 00 00 00) ;"RTC"
 005000 Deadc0de
 008000 sth (00 00 00 00, 00 00 00 00) ;"VMC?"
 009000 Deadc0de ;should contain UART etc. (maybe disabled?)
 014000 sth (00 00 00 00, 00 00 00 00) (--crash-- at 0140cx) ;"GPIO?"
 015000 Deadc0de
 018000 sth (00 01 0E 00, 00 01 0E 00) ;\MBOX
 019000 sth (00 01 0E 00, 00 01 0E 00) ;/
 01A000 sth (00 01 0E 00, 00 01 0E 00) ;\MBOX:HOST_IF?
 01B000 sth (--crash-- at 01B00x) ;/
 01C000 sth (00 00 14 00, D8 48 45 0E) ;-ANALOG?
 01D000 Deadc0de
 020000 sth (00's) ;\DMA?
 021000 sth (01 00 00 00, 02 00 00 00) ;/
 022000 sth (00's) ;\ ;\same as
 023000 sth (01 00 00 00, 02 00 00 00) ; ;/DMA?
 024000 sth (00's) ; ??
 025000 sth (00's) ;
 026000 sth (00's) ;
 027000 sth (00's) ;/
 028000 sth (<--- mac addr ---> 86 38) ;\MAC_PCU? mac_pcu.h ?
 029000 sth (14 E1 38 8A, 80 73 00 00) ;/ ;\
 02A000 sth (00's) ; BB at 29800h?
 02B000 sth (00's) ;/
 02C000 sth (00's) ;-
 02D000 sth (00's) ;-
 02E000 sth (00's) ;-
 02F000 sth (00's) ;-
 030000 Deadc0de ;RDMA?? and (not?) EFUSE??
 040000 Deadbeef ;\
 050000 Deadbeef ;
 060000 Deadbeef ;
 070000 Deadbeef ;/
 080000 004F1B74 ;\
 090000 004F1B74 ;
 0A0000 004F1B74 ; mirror of ROM word at [0F3FF8] (second-last-word)
 0B0000 004F1B74 ;
 0C0000 004F1B74 ;
 0D0000 004F1B74 ;/
 0E0000 sth (06 10 00 00, 21 22 22 22) ;\80K ROM (14000h bytes)
 0F0000 sth (00 00 05 60, FF DF FF FF) ;/
 0F4000 004F1B74 ;-mirror of ROM word at [0F3FF8] (second-last-word)
 100000 sth (48 0F 8E 00, 70 14 50 00) ;\
 110000 sth ; 184K RAM (2E000h bytes)
 120000 sth ;/
 12E000 98A8A2AA ;\
 ... ;
 1FF000 98A8A2AA ;/
 200000 Deadbeef ;\
 300000 Deadbeef ;/
 00400000 looks like mirror of 000000
 0041B000 looks like mirror of 01B000 --crash--
 ... probably more mirrors...
 FFC00000 looks like mirror of 000000
 FFC1B000 looks like mirror of 01B000 --crash--
 ... probably more mirrors till FFFFFFFF |
| --- |

**some partial hw4.0 memory dump...**

```

| 000000 zerofilled
 004000 sth (01 00 00 00, 00 00 00 00) ;"RTC"
 005000 zerofilled
 040000 Deadbeef
 080000 zerofilled
 0E0000 06 10 00 00 21 22 22 22 00 00 00 E0 83 00 8E 00 ... ;ROM?
 109DC0 zerofilled ;ROM?
 10C000 14 19 52 00 ... ;ROM?
 114000 zerofilled ;ROM?
 120000 A0 B2 52 00 ;=52B2A0h (app_defined_area) ;RAM
 140000 zerofilled
 200000 Deadbeef
 400000..FFFFFFFF not checked (probably mirrors of above?) |
| --- |

| DSi Atheros Wifi - Internal I/O - 004000h - RTC/Clock SOC (hw2/hw4/hw6) |
| --- |

**ATH:004000h - WLAN/SOC_RESET_CONTROL ;hw2/hw4/hw6**

```

| 0 SI0_RST
 1 UART_RST
 2 MBOX_RST (requires delay before further MBXO access, else hangs 1-2s)
 3 -
 4 hw2/hw4: MAC_WARM_RST ;-moved to 005000h.bit0 in hw6 ;\hw2/hw4 only
 5 hw2/hw4: MAC_COLD_RST ;-moved to 005000h.bit1 in hw6 ;/
 6 CPU_WARM_RST
 7 hw2/hw4: WARM_RST ;-moved to 005000h.bit2 in hw6 ;-hw2/hw4 only
 8 COLD_RST (0=no change, 1=reset) ;-also in 005000h.bit3 in hw6
 9 RST_OUT
 10 hw2/hw4: VMC_REMAP_RESET ;removed in hw6 ;-hw2/hw4 only
 11 CPU_INIT_RESET
 12 hw4: BB_WARM_RST ;-moved to 005000h.bit4 in hw6 ;\hw4 only
 13 hw4: BB_COLD_RST ;-moved to 005000h.bit5 in hw6 ; (not hw2, and
 14 hw4: DEBUG_UART_RST ;-moved to bit16 in hw6 ;/moved in hw6)
 12 hw6: MIT_ADAPTOR_RST ;\
 13 hw6: MIT_REG_MAPPING_RST ;
 14-15 hw6: - ;
 16 hw6: DEBUG_UART_RST ;<-- moved from old bit14 ;
 17 hw6: UART2_RST ;
 18 hw6: CHECKSUM_ACC_RST ;
 19 hw6: I2S_MBOX_RST ;
 20 hw6: I2S_RST ;
 21 hw6: GE0_RST ;
 22 hw6: MDIO_RST ; hw6
 23 hw6: MMAC_RST ;
 24 hw6: USB_RST ;
 25 hw6: USB_PHY_RST ;
 26 hw6: USB_PHY_ARST ;
 27 hw6: I2C_SLAVE_RST ;
 28 hw6: I2S_1_MBOX_RST ;
 29 hw6: I2S_1_RST ;
 30 hw6: SPI2_RST ;
 31 hw6: SDIO2_RST ;/ |
| --- |

**ATH:004008h - WLAN_TCXO_DETECT ;hw2/hw4 at this address**

**ATH:004004h - SOC_TCXO_DETECT ;hw6 at this address (unlike hw2/hw4)**

```

| 0 PRESENT |
| --- |

**ATH:00400Ch - WLAN_XTAL_TEST ;hw2/hw4 at this address**

**ATH:004008h - SOC_XTAL_TEST ;hw6 at this address (unlike hw2/hw4)**

```

| 0 NOTCXODET |
| --- |

**ATH:004020h - WLAN/SOC_CPU_CLOCK ;hw2/hw4/hw6**

```

| 0-1 STANDARD |
| --- |

**ATH:004028h - WLAN/SOC_CLOCK_CONTROL ;hw2/hw4/hw6**

```

| 0 SI0_CLK
 1 hw2: UART_CLK ;0=enable? ;<-- hw2 only (removed in hw4/hw6)
 2 LF_CLK32 |
| --- |

```

| ______________________________ Watchdog Timer ______________________________ |
| --- |

**ATH:004030h - WLAN/SOC_WDT_CONTROL - Watchdog Timer Control ;hw2/hw4/hw6**

```

| 0-2 ACTION |
| --- |

**ATH:004034h - WLAN/SOC_WDT_STATUS - Watchdog Interrupt Status ;hw2/hw4/hw6**

```

| 0 INTERRUPT |
| --- |

**ATH:004038h - WLAN/SOC_WDT - Watchdog Timer Target ;hw2/hw4/hw6**

```

| 0-21 TARGET |
| --- |

**ATH:00403Ch - WLAN/SOC_WDT_COUNT - Watchdog Timer Count ;hw2/hw4/hw6**

```

| 0-21 VALUE |
| --- |

**ATH:004040h - WLAN/SOC_WDT_RESET - Watchdog Timer Reset ;hw2/hw4/hw6**

```

| 0 VALUE |
| --- |

```

| _____________________________ Interrupt Status _____________________________ |
| --- |

**ATH:004044h - WLAN/SOC_INT_STATUS ;hw2/hw4/hw6**

```

| 0 WDT_INT ;-Watchdog Timer
 1 ERROR
 2 UART ;-Serial UART
 3 GPIO ;-GPIO
 4 SI ;-Serial I2C/SPI
 5 KEYPAD
 6 LF_TIMER0 ;\
 7 LF_TIMER1 ; Low-Freq Timer 0..3
 8 LF_TIMER2 ; and
 9 LF_TIMER3 ; High-Freq Timer
 10 HF_TIMER ;/
 11 RTC_ALARM ;-Real-Time Clock Alarm
 12 MAILBOX
 13 MAC ;-maybe this is "MAC's INTA#" (see WMAC IRQ) ?
 14 RTC_POWER
 15 hw4/hw6: BTCOEX ;Bluetooth Coex ;\
 16 hw4/hw6: RDMA ;
 17 hw4/hw6: GENERIC_MBOX (aka GMBOX) ; hw4/hw6
 18 hw4/hw6: UART_MBOX ;
 19 hw4/hw6: EFUSE_OVERWRITE ;
 20 hw4/hw6: THERM ;
 21 hw4/hw6: HCI_UART ;/
 22 hw6: MODE_SWITCH ;\
 23 hw6: RF_SLEEP_RISING ;
 24 hw6: BBP_SLEEP_RISING ;
 25 hw6: FLIGHT_MODE ;
 26 hw6: MIT_REG_ACCESS ; hw6 only
 27 hw6: MMAC ;
 28 hw6: USBIP ;
 29 hw6: USBDMA ;
 30 hw6: SDIO2_MBOX ;
 31 hw6: STE_MBOX ;/ |
| --- |

**ATH:0040CCh - WLAN/SOC_INT_STATUS1 ;hw6**

```

| 0 MAC_1 ;\
 1 MAC_2 ;
 2 MAC_3 ;
 3 MAC_4 ; hw6 only
 4 CKSUM ; (additional bits, extending
 5 I2C_S ; the bits in port 004044h)
 6 GMAC ;
 7 MDIO ;
 8 I2S ;
 9 I2S_1 ;/ |
| --- |

```

| __________________ Low-Freq Timer 0-3 and High-Freq Timer __________________ |
| --- |

**ATH:004048h - WLAN/SOC_LF_TIMER0 ;hw2/hw4/hw6**

**ATH:004058h - WLAN/SOC_LF_TIMER1 ;hw2/hw4/hw6**

**ATH:004068h - WLAN/SOC_LF_TIMER2 ;hw2/hw4/hw6**

**ATH:004078h - WLAN/SOC_LF_TIMER3 ;hw2/hw4/hw6**

**ATH:004088h - WLAN/SOC_HF_TIMER ;hw2/hw4/hw6**

```

| For LF Timer 0-3:
 0-31 TARGET
 For HF Timer:
 12-31 TARGET ;<-- not bit0-31 for HF timer |
| --- |

**ATH:00404Ch - WLAN/SOC_LF_TIMER_COUNT0 ;hw2/hw4/hw6**

**ATH:00405Ch - WLAN/SOC_LF_TIMER_COUNT1 ;hw2/hw4/hw6**

**ATH:00406Ch - WLAN/SOC_LF_TIMER_COUNT2 ;hw2/hw4/hw6**

**ATH:00407Ch - WLAN/SOC_LF_TIMER_COUNT3 ;hw2/hw4/hw6**

**ATH:00408Ch - WLAN/SOC_HF_TIMER_COUNT ;hw2/hw4/hw6**

```

| For LF Timer 0-3:
 0-31 VALUE
 For HF Timer:
 12-31 VALUE ;<-- not bit0-31 for HF timer |
| --- |

**ATH:004090h - WLAN/SOC_HF_LF_COUNT ;hw2/hw4/hw6**

```

| 0-31 VALUE ;<-- extra for HF timer |
| --- |

**ATH:004050h - WLAN/SOC_LF_TIMER_CONTROL0 ;hw2/hw4/hw6**

**ATH:004060h - WLAN/SOC_LF_TIMER_CONTROL1 ;hw2/hw4/hw6**

**ATH:004070h - WLAN/SOC_LF_TIMER_CONTROL2 ;hw2/hw4/hw6**

**ATH:004080h - WLAN/SOC_LF_TIMER_CONTROL3 ;hw2/hw4/hw6**

**ATH:004094h - WLAN/SOC_HF_TIMER_CONTROL ;hw2/hw4/hw6**

```

| For both LF and HF:
 0 RESET
 1 AUTO_RESTART
 For LF Timer 0-3:
 2 ENABLE
 For HF Timer:
 2 ON ;<-- extra bit for HF timer
 3 ENABLE ;<-- moved to bit3 |
| --- |

**ATH:004054h - WLAN/SOC_LF_TIMER_STATUS0 ;hw2/hw4/hw6**

**ATH:004064h - WLAN/SOC_LF_TIMER_STATUS1 ;hw2/hw4/hw6**

**ATH:004074h - WLAN/SOC_LF_TIMER_STATUS2 ;hw2/hw4/hw6**

**ATH:004084h - WLAN/SOC_LF_TIMER_STATUS3 ;hw2/hw4/hw6**

**ATH:004098h - WLAN/SOC_HF_TIMER_STATUS ;hw2/hw4/hw6**

```

| 0 INTERRUPT |
| --- |

```

| _____________________________ Real-Time Clock _____________________________ |
| --- |

The DSi does have a battery-backed RTC accessed via Port 4000138h on ARM7 side.
Below SOC_RTC would offer a second RTC, but it isn't actually used in DSi (the
SOC_RTC registers aren't battery-backed, and the DSi firmware isn't
initializing them from the ARM7 time either).

**ATH:00409Ch - WLAN/SOC_RTC_CONTROL ;hw2/hw4/hw6**

```

| 0 LOAD_ALARM
 1 LOAD_RTC
 2 ENABLE |
| --- |

**ATH:0040A0h - WLAN/SOC_RTC_TIME ;hw2/hw4/hw6**

**ATH:0040A8h - WLAN/SOC_RTC_SET_TIME ;hw2/hw4/hw6**

```

| 0-6 SECOND
 8-14 MINUTE
 16-21 HOUR
 24-26 WEEK_DAY |
| --- |

**ATH:0040A4h - WLAN/SOC_RTC_DATE ;hw2/hw4/hw6**

**ATH:0040ACh - WLAN/SOC_RTC_SET_DATE ;hw2/hw4/hw6**

```

| 0-5 MONTH_DAY
 8-12 MONTH
 16-23 YEAR |
| --- |

**ATH:0040B0h - WLAN/SOC_RTC_SET_ALARM ;hw2/hw4/hw6**

```

| 0-6 SECOND
 8-14 MINUTE
 16-21 HOUR |
| --- |

**ATH:0040B4h - WLAN/SOC_RTC_CONFIG ;hw2/hw4/hw6**

```

| 0 DSE
 1 TWELVE_HOUR
 2 BCD |
| --- |

**ATH:0040B8h - WLAN/SOC_RTC_ALARM_STATUS ;hw2/hw4/hw6**

```

| 0 INTERRUPT
 1 ENABLE |
| --- |

```

| _________________________________ Chip ID _________________________________ |
| --- |

**ATH:0040ECh - WLAN_CHIP_ID ;hw2/hw4 - single ID in hw2/hw4**

**ATH:0040ECh - LEGACY_SOC_CHIP_ID ;hw6 - first/legacy/old ID in hw6**

**ATH:0040F0h - SOC_CHIP_ID ;hw6 - second/actual/new ID in hw6**

```

| 0-3 VERSION_ID (4bit, usually 0 or 1)
 4-15 CONFIG_ID (12bit, usually 0)
 16-31 DEVICE_ID (16bit, usually xx00h or xx01h for AR60xx, eg. 0D00h=AR6013) |
| --- |

The DSi Wifi Firmware file contains a list of supported ID(s) for each chip:

```

| AR6002: 02010001h or 02000001h ;on actual DSi hardware it is 02000001h
 AR6013: 0D000000h or 0D000001h ;on actual DSi hardware it is 0D000000h |
| --- |

For 3DS:

```

| AR6014: 0D000001h=[0040ECh] ;on actual 3DS hardware it is 0D000001h |
| --- |

Whereas, the DSi is using that value only for verification, not for actually
detecting the installed chip type (that is done by a byte in SPI FLASH).

Note: There's also a "BB_D2_CHIP_ID" register in the Baseband section.

```

| ___________________________________ Misc ___________________________________ |
| --- |

**ATH:0040BCh - WLAN/SOC_UART_WAKEUP ;hw2/hw4/hw6**

```

| 0 ENABLE |
| --- |

**ATH:0040C0h - WLAN/SOC_RESET_CAUSE ;hw2/hw4/hw6**

```

| 0-2 Cause of most recent Reset event (LAST) |
| --- |

Possible values are (according to AR6001 datasheet - might be different in
hw2/hw4/hw6):

```

| 00h = SYS_RST_L pin was asserted
 01h = Host wrote to the SDIO reset register
 02h = Software wrote RTC_CONTROL_COLD_RST register (aka COLD_RST bit?)
 03h = Software wrote RTC_CONTROL_WARM_RST register (aka WARM_RST bit?)
 04h = Software wrote RTC_CONTROL_CPU_RST register (aka CPU_WARM_RST bit?)
 05h = Watchdog Timer has expired
 06h..07h = Reserved |
| --- |

**ATH:0040C4h - WLAN/SOC_SYSTEM_SLEEP ;hw2/hw4/hw6**

```

| 0 DISABLE
 1 LIGHT
 2 MAC_IF
 3 MBOX
 4 HOST_IF
 5 hw6: MCI ;-hw6 only |
| --- |

**ATH:0040C8h - WLAN/SOC_SDIO_WRAPPER ;hw2/hw4/hw6**

```

| 0 ON
 1 SOC_ON
 2 WAKEUP
 3 SLEEP |
| --- |

**ATH:004110h - WLAN/SOC_POWER_REG - located here in hw2**

**ATH:00410Ch - WLAN/SOC_POWER_REG - located here in hw4/hw6**

```

| 0 POWER_EN
 1 WLAN_PWD_EN
 2 hw2: WLAN_SCALE_EN ;\
 2 hw4: WLAN_ISO_EN ; hw2/hw4 (renamed from hw2:SCALE
 3 hw2: SOC_SCALE_EN ; to hw4:ISO, and removed in hw6)
 3 hw4: SOC_ISO_EN ;/
 4 RADIO_PWD_EN
 5 hw2/hw4: WLAN_ISO_CNTL ;\hw2/hw4 (removed? in hw6)
 6 hw2/hw4: WLAN_ISO_DIS ;/
 7 CPU_INT_ENABLE
 8-11 hw2/hw4: VLVL ;-hw2/hw4 (removed? in hw6)
 12 hw4/hw6: WLAN_MAC_PWD_EN ;\
 13 hw4/hw6: WLAN_BB_PWD_EN ; hw4/hw6
 14 hw4/hw6: DEBUG_EN ;/
 15 hw4: SLEEP_MAKE_N_BREAK_EN ;\hw4+hw6, but changed/renamed?
 15 hw6: DEEP_SLEEP_EN ;/
 16 hw6: DISCON_MODE_EN ;\hw6
 17 hw6: SWREG_VS_EN ;/ |
| --- |

**ATH:004114h - WLAN/SOC_CORE_CLK_CTRL - located here in hw2**

**ATH:004110h - WLAN/SOC_CORE_CLK_CTRL - located here in hw4/hw6**

```

| 0-2 DIV |
| --- |

**ATH:0041C0h - WLAN/SOC_GPIO_WAKEUP_CONTROL - located here in hw2**

**ATH:004114h - WLAN/SOC_GPIO_WAKEUP_CONTROL - located here in hw4/hw6**

```

| 0 ENABLE |
| --- |

**ATH:004214h - SLEEP_RETENTION ;hw4/hw6**

```

| 0 ENABLE
 1 MODE
 2-9 TIME |
| --- |

```

| ___________________________________ LPO ___________________________________ |
| --- |

**ATH:0040D4h - WLAN/SOC_LPO_CAL_TIME ;hw2/hw4/hw6**

```

| 0-13 LENGTH |
| --- |

**ATH:0040D8h - WLAN/SOC_LPO_INIT_DIVIDEND_INT ;hw2/hw4/hw6**

```

| 0-23 VALUE |
| --- |

**ATH:0040DCh - WLAN/SOC_LPO_INIT_DIVIDEND_FRACTION ;hw2/hw4/hw6**

```

| 0-10 VALUE |
| --- |

**ATH:0040E0h - WLAN/SOC_LPO_CAL ;hw2/hw4/hw6**

```

| 0-19 COUNT
 20 ENABLE |
| --- |

**ATH:0040E4h - WLAN/SOC_LPO_CAL_TEST_CONTROL ;hw2/hw4/hw6**

```

| 0-4 hw2/hw4: RTC_CYCLES (5bit) ;\hw2/hw4 (5bit)
 5 hw2/hw4: ENABLE ;/
 0-15 hw6: RTC_CYCLES (16bit) ;\hw6 (expanded to 16bit)
 16 hw6: ENABLE ;/(and moved enable flag) |
| --- |

**ATH:0040E8h - WLAN/SOC_LPO_CAL_TEST_STATUS ;hw2/hw4/hw6**

```

| 0-15 COUNT
 16 READY |
| --- |

```

| __________________________ below in hw4/hw6 only __________________________ |
| --- |

**ATH:004284h - LP_PERF_COUNTER ;hw4/hw6**

```

| 0 EN |
| --- |

**ATH:004288h - LP_PERF_LIGHT_SLEEP ;hw4/hw6**

**ATH:00428Ch - LP_PERF_DEEP_SLEEP ;hw4/hw6**

**ATH:004290h - LP_PERF_ON - hw4 only**

```

| 0-31 CNT |
| --- |

```

| ___________________________________ MISC ___________________________________ |
| --- |

**ATH:0042A8h - CHIP_MODE ;hw4/hw6**

```

| 0-1 BIT |
| --- |

**ATH:0042ACh - CLK_REQ_FALL_EDGE ;hw4/hw6**

```

| 0-7 DELAY
 31 EN |
| --- |

```

| ___________________________________ OTP ___________________________________ |
| --- |

**ATH:0042B0h - OTP ;hw4/hw6**

```

| 0 VDD12_EN
 1 LDO25_EN |
| --- |

**ATH:0042B4h - OTP_STATUS ;hw4/hw6**

```

| 0 VDD12_EN_READY
 1 LDO25_EN_READY |
| --- |

```

| ___________________________________ PMU ___________________________________ |
| --- |

**ATH:0042B8h - PMU ;hw4/hw6**

```

| 0-1 REG_WAKEUP_TIME_SEL |
| --- |

**ATH:0042C0h..42C4h - PMU_CONFIG[0..1] ;hw4**

```

| 0-15 VALUE ... whatever... (2x16bit)
 16-31 - |
| --- |

Maybe this arrary entries are equivalent/similar to the PMU_CONFIG and
PMU_PAREG registers in hw6? The hw4 source code is claiming the array entries
to be 16bit wide each, which doesn't match up with the two 5bit/3bit registers
on hw6 though.

**ATH:0042BCh - PMU_CONFIG ;hw6**

```

| 0-4 VALUE |
| --- |

Maybe equivalent/similar to one of the PMU_CONFIG[0..1] entries in hw4?

**ATH:0042C0h - PMU_PAREG ;hw6**

```

| 0-2 LVL_CTR |
| --- |

Maybe equivalent/similar to one of the PMU_CONFIG[0..1] entries in hw4?

**ATH:0042C8h - PMU_BYPASS ;hw4 at this address**

**ATH:0042C4h - PMU_BYPASS ;hw6 at this address (unlike hw4)**

```

| 0 hw4/hw6: PAREG ;-hw4/hw6 ;\
 1 hw4: DREG ;-removed in hw6 ;\hw4 ; hw4/hw6 only
 2 hw4: SWREG ;-moved to bit1 in hw6 ;/ ; (not hw2)
 1 hw6: SWREG ;-formerly in bit2 ;-hw6 ;/ |
| --- |

```

| ________________________________ THERM CTRL ________________________________ |
| --- |

**ATH:0042DCh - THERM_CTRL1 ;hw4/hw6**

```

| 0 INT_STATUS
 1 INT_EN
 2 MEASURE
 3-4 TYPE
 5-11 WIDTH
 12-15 WIDTH_ARBITOR
 16 BYPASS |
| --- |

**ATH:0042E0h - THERM_CTRL2 ;hw4/hw6**

```

| 0-7 LOW
 8-15 HIGH
 16-23 SAMPLE
 24 ADC_ON
 25 ADC_OFF |
| --- |

**ATH:0042E4h - THERM_CTRL3 ;hw4/hw6**

```

| 0-7 ADC_OFFSET
 8-16 ADC_GAIN |
| --- |

```

| ____________________________ below in hw6 only ____________________________ |
| --- |

**ATH:0042E8h - LISTEN_MODE1 ;hw6**

```

| 0 ENABLE
 1 CLOCK_GATE
 2 TIMER_OVERFLOW_WAKE
 3-18 TIMER_THRESH_WAKE
 19 TIMER_CLEAR |
| --- |

**ATH:0042ECh - LISTEN_MODE2 ;hw6**

```

| 0-15 TIMER_TRIGGER_WAKE |
| --- |

**ATH:0042F0h - AUDIO_PLL_CONFIG ;hw6**

```

| 0-3 REFDIV
 4 BYPASS
 5 PLLPWD
 7-9 POSTPLLDIV
 12-14 EXT_DIV
 31 UPDATING |
| --- |

**ATH:0042F4h - AUDIO_PLL_MODULATION ;hw6**

```

| 0 START
 1-6 TGT_DIV_INT
 11-28 TGT_DIV_FRAC |
| --- |

**ATH:0042F8h - AUDIO_PLL_MOD_STEP ;hw6**

```

| 0-3 UPDATE_CNT
 4-13 INT
 14-31 FRAC |
| --- |

**ATH:0042FCh - CURRENT_AUDIO_PLL_MODULATION ;hw6**

```

| 1-6 INT
 10-27 FRAC |
| --- |

**ATH:004300h - ETH_PLL_CONFIG ;hw6**

```

| 0-4 REFDIV
 5 BYPASS
 6 PLLPWD
 7-9 OUTDIV
 12-17 INT
 18-27 FRAC
 28 RANGE
 29 GE0
 30 GE0_MASTER |
| --- |

**ATH:004304h - CPU_PLL_CONFIG ;hw6**

```

| 0-4 REFDIV
 6 PLLPWD
 7-9 OUTDIV
 12-17 INT
 20-25 FRAC
 28 RANGE |
| --- |

**ATH:004308h - BB_PLL_CONFIG ;hw6**

```

| 0-17 FRAC |
| --- |

**ATH:00430Ch - ETH_XMII ;hw6**

```

| 0-7 PHASE0_COUNT
 8-15 PHASE1_COUNT
 16-23 OFFSET_COUNT
 24 OFFSET_PHASE
 25 GIGE
 26-27 TX_DELAY
 28-29 RX_DELAY
 30 GIGE_QUAD
 31 TX_INVERT |
| --- |

**ATH:004310h - USB_PHY_CONFIG ;hw6**

```

| 0 HOSTMODE
 1 PLL_PWD
 2 TESTMODE
 3 REFDIV
 4-7 REFCLK_SEL |
| --- |

**ATH:004314h - MITSUMI_INT_CONTROL_REG ;Mitsumi Interrupt Enable ;hw6**

**ATH:004318h - MITSUMI_INT_STATUS_REG ;Mitsumi Interrupt Status ;hw6**

```

| 0 MODE_SWITCH
 1 RF_SLEEP
 2 BBP_SLEEP
 3 FLIGHT_MODE |
| --- |

Maybe this is related to the Nintendo DSi's backwards compatibilty mode (in
which, the Atheros chip is simulating a Mitsumi BB/RF chip for use with older
NDS games)? Or maybe it's unrelated to that (DSi/3DS don't seem to have hw6 at
all).

**ATH:00431Ch - CURRENT_WORKING_MODE ;hw6**

```

| 0 VALUE
 1 NOT_FIRST_MIT_MODE
 2 MIT_REG_WR_TRIGGER_EN
 5 MIT_FORCE_ACTIVE_ON |
| --- |

**ATH:004320h - RTC_SLEEP_COUNT ;hw6**

```

| 0-5 THRESHOLD |
| --- |

**ATH:004324h - MIT2_VAP ;hw6**

```

| 0 MODE |
| --- |

**ATH:004328h - SECOND_HOST_INFT ;hw6**

```

| 0 SDIO_MODE |
| --- |

**ATH:00432Ch - SDIO_HOST ;hw6**

```

| 0 RESET |
| --- |

**ATH:004330h - ENTERPRISE_CONFIG ;hw6**

```

| 0 LOCATION_DISABLE
 1 LOOPBACK_DISABLE
 2 MIN_PKT_SIZE_DISABLE
 3 TXBF_DISABLE
 4 CH_10MHZ_DISABLE
 5 CH_5MHZ_DISABLE
 6 CHAIN1_DISABLE
 7 DUAL_BAND_DISABLE
 8 GREEN_TX_DISABLE
 9 LDPC_DISABLE
 10 STBC_DISABLE
 11 SWCOM_IDLE_MODE
 12 TPC_LOWER_PERFORMANCE |
| --- |

**ATH:004334h - RTC_DEBUG_BUS ;hw6**

```

| 0 SEL |
| --- |

**ATH:004338h - RTC_EXT_CLK_BUF ;hw6**

```

| 0 EN |
| --- |

| DSi Atheros Wifi - Internal I/O - 00x000h - RTC/Clock WLAN (hw2/hw4/hw6) |
| --- |

**ATH:004004h/004004h/005004h - WLAN_XTAL_CONTROL ;hw2/hw4/hw6**

```

| 0 TCXO |
| --- |

**ATH:004010h/004010h/005010h - WLAN_QUADRATURE ;hw2/hw4/hw6**

```

| 0-1 hw2/hw4: DAC (2bit) ;\expanded DAC from 2bit (hw2/hw4) to 3bit (hw6)
 0-2 hw6: DAC (3bit) ; (and removed SEL bit in hw6)
 2 hw2/hw4: SEL ;/
 4-5 hw2: ADC (2bit) ;\expanded ADC from 2bit (hw2) to 4bit (hw4)
 4-7 hw4/hw6: ADC (4bit) ;/ |
| --- |

**ATH:004014h/004014h/005014h - WLAN_PLL_CONTROL ;hw2/hw4/hw6**

```

| 0-9 DIV
 12-15 hw2/hw4: REFDIV (4bit)
 10-13 hw6: REFDIV (4bit, now here)
 14-15 hw6: CLK_SEL ;<-- maybe replaces removed "SEL" in WLAN_QUADRATURE?
 16 BYPASS
 17 UPDATING (R)
 18 NOPWD
 19 MAC_OVERRIDE
 20 DIG_TEST_CLK |
| --- |

**ATH:004018h/004018h/005018h - WLAN_PLL_SETTLE ;hw2/hw4/hw6**

```

| 0-11 hw2/hw4: TIME (12bit) ;\decreased from 12bit to 11bit in hw6
 0-10 hw6: TIME (11bit) ;/ |
| --- |

**ATH:00401Ch/00401Ch/00501Ch - WLAN_XTAL_SETTLE ;hw2/hw4/hw6**

```

| 0-7 hw2/hw4: TIME (8bit) ;\decreased from 8bit to 7bit in hw6
 0-6 hw6: TIME (7bit) ;/ |
| --- |

**ATH:004020h/004020h/005020h - WLAN_CLOCK_OUT ;hw2/hw4/hw6**

```

| 0-3 hw2/hw4: SELECT (4bit) ;\raised from 4bit to 5bit in hw6,
 0-4 hw6: SELECT (5bit) ; and added new DELAY field in hw6
 5-7 hw6: DELAY (3bit, new) ;/ |
| --- |

**ATH:00402Ch/00402Ch/005024h - WLAN_BIAS_OVERRIDE ;hw2/hw4/hw6**

```

| 0 ON |
| --- |

**ATH:0040CCh/0040CCh/005030h - WLAN_MAC_SLEEP_CONTROL ;hw2/hw4/hw6**

```

| 0-1 hw2/hw4: ENABLE ;\
 0 hw6: ENABLE ; reduced from 2bit to 1bit in hw6
 1 hw6: RESERVED ;/
 2 hw6: HSEL_WMAC_ENABLE ;-new in hw6 |
| --- |

**ATH:0040D0h/0040D0h/005034h - WLAN_KEEP_AWAKE ;hw2/hw4/hw6**

```

| 0-7 COUNT |
| --- |

**ATH:0040F0h/0040F0h/005038h - WLAN_DERIVED_RTC_CLK ;hw2/hw4/hw6**

```

| 1-15 PERIOD
 16-17 hw2/hw4: FORCE ;-hw2/hw4 only (removed in hw6)
 18 EXTERNAL_DETECT (R)
 20 EXTERNAL_DETECT_EN |
| --- |

```

| ________________________ SLP or SLOP or SLEEP or so ________________________ |
| --- |

**ATH:0040F4h/0040F4h/00503Ch - MAC_PCU_SLP32_MODE ;hw2/hw4/hw6**

**ATH:028244h (mirror of 0040F4h) - MAC_PCU_REG_SLP32_MODE (ini:10F424h) ;hw2**

```

| 0-19 HALF_CLK_LATENCY
 20 ENABLE ;<-- see hw2 note ;-hw2/hw4/hw6 (on hw2 in mirror only)
 21 hw2: TSF_WRITE_PENDING ;\changed/renamed in hw2/hw4
 21 hw4/hw6: TSF_WRITE_STATUS (R) ;/
 22 hw4/hw6: DISABLE_32KHZ ;\
 23 hw4/hw6: FORCE_BIAS_BLOCK_ON ; hw4/hw6 only (unspecified in hw2)
 24 hw4/hw6: TSF2_WRITE_STATUS (R) ;/ |
| --- |

In hw2, this register appears to be mirrored to 0040F4h (without ENABLE flag in
bit20) and 028244h (bit ENABLED flag in bit20).

**ATH:0040F8h/0040F8h/005040h - MAC_PCU_SLP32_WAKE ;hw2/hw4/hw6**

**ATH:028248h (mirror of 0040F8h) - MAC_PCU_REG_SLP32_WAKE (ini:07EFh) ;hw2**

```

| 0-15 XTL_TIME |
| --- |

**ATH:0040FCh/0040FCh/005044h - MAC_PCU_SLP32_INC ;hw2/hw4/hw6**

**ATH:02824Ch (mirror of 0040FCh) - MAC_PCU_REG_SLP32_TSF_INC (ini:1E848h) ;hw2**

```

| 0-19 TSF_INC |
| --- |

**ATH:004100h/004100h/005048h - MAC_PCU_SLP_MIB1 ;hw2/hw4/hw6**

**ATH:028250h (mirror of 004100h) - MAC_PCU_REG_SLPMIB1 ;hw2**

```

| 0-31 SLEEP_CNT |
| --- |

**ATH:004104h/004104h/00504Ch - MAC_PCU_SLP_MIB2 ;hw2/hw4/hw6**

**ATH:028254h (mirror of 004104h) - MAC_PCU_REG_SLPMIB2 ;hw2**

```

| 0-31 CYCLE_CNT |
| --- |

**ATH:004108h/004108h/005050h - MAC_PCU_SLP_MIB3 ;hw2/hw4/hw6**

**ATH:028258h (mirror of 004108h) - MAC_PCU_REG_SLPMIB3 ;hw2**

```

| 0 CLR_CNT
 1 PENDING (R) |
| --- |

In hw2, this register appears to be mirrored to 004108h (with bit1 called
"PENDING") and 028258h (with bit1 called "PEND" for whatever reason).

**ATH:0280D4h/004204h/0050C0h - MAC_PCU_SLP1 ;hw2/hw4/hw6**

```

| 0-18 hw2: outcommented: NEXT_DTIM) (hw2: ini:2AAAAh) ;\outcommented
 20 hw2: outcommented: ENH_SLEEP_ENABLE) (hw2: ini:1) ;/
 0-4 hw2/hw6: CAB_TIMEOUT_EXT (hw2: ini:0) ;-hw2/hw6
 0-15 hw4: CAB_TIMEOUT ;-hw4
 19 ASSUME_DTIM (hw2: ini:0) ;-hw2/hw4/hw6
 20 hw6: BUG_59985_FIX_ENABLE ;-hw6
 21-31 hw2/hw6: CAB_TIMEOUT (hw2: ini:5) ;-hw2/hw6 |
| --- |

**ATH:0280D8h/004208h/0050C4h - MAC_PCU_SLP2 ;hw2/hw4/hw6**

```

| 0-18 hw2: outcommented: NEXT_TIM (hw2: ini:55555h) ;-outcommented
 0-15 hw4: BEACON_TIMEOUT ;-hw4
 0-4 hw2/hw6: BEACON_TIMEOUT_EXT (hw2: ini:0) ;\hw2/hw6
 21-31 hw2/hw6: BEACON_TIMEOUT (hw2: ini:2) ;/ |
| --- |

**ATH:0280DCh - (outcommented) ;aka MAC_PCU_REG_SLP3 ;hw2 (but outcommented)**

```

| 0-15 hw2: outcommented: TIM_PERIOD (hw2: ini:2) ;\outcommented
 16-31 hw2: outcommented: DTIM_PERIOD (hw2: ini:3) ;/ |
| --- |

**ATH:028260h/0050C8h - MAC_PCU_SLP3 ;hw2/hw6**

```

| 0-15 hw2/hw6: CAB_AWAKE_DUR (hw2: ini:0005h) ;\hw2/hw6
 16 hw2/hw6: CAB_AWAKE_ENABLE (hw2: ini:0) ;/ |
| --- |

**ATH:0050CCh - MAC_PCU_SLP4 ;hw6**

```

| 0-15 hw6: BEACON2_TIMEOUT
 16-30 hw6: CAB2_TIMEOUT
 31 hw6: ASSUME_DTIM2 |
| --- |

```

| ______________________________ Generic Timers ______________________________ |
| --- |

**ATH:028200h..02821Ch - MAC_PCU_REG_GNRCTMR_N[0..7] ;hw2**

**ATH:028220h..02823Ch - MAC_PCU_REG_GNRCTMR_P[0..7] ;hw2**

**ATH:004140h..417Ch/005064h..50A0h - MAC_PCU_GENERIC_TIMERS[0..15] ;hw4/hw6**

**ATH:0041C0h..41FCh/0050DCh..5118h - MAC_PCU_GENERIC_TIMERS2[0..15] ;hw4/hw6**

```

| 0-31 hw2, for "GNRCTMR_N" entries: GNRCTMR_N (32bit)
 0-27 hw2, for "GNRCTMR_P" entries: GNRCTMR_P (only 28bit here)
 0-31 hw4/hw6: DATA (32bit) |
| --- |

Unknown how that stuff is related...

```

| - hw2 has "8xTMR_N" plus "8xTMR_P"
 - hw4/6 has "16xTIMER" plus "16xTIMER2" |
| --- |

maybe TIMER and TIMER2 are equivalent to TMR_N and TMR_P, or maybe TIMER is
meant to contain "eight N+P pairs" (and TIMER2 another eight pairs), or maybe
the "N" and "P" stuff was completely dropped in hw4/hw6. The ENABLE bits are
also weird:

```

| - hw2 has 1x8 ENABLE bits (for 8+8 timer entries)
 - hw4 has 2x16 ENABLE bits (for 16+16 timer entries)
 - hw6 has 2x8 ENABLE bits (for 16+16 timer entries, too) |
| --- |

Note that hw2 has the generic timers in the WMAC PCU area at 028xxxh.

**ATH:028240h/004180h/0050BCh - MAC_PCU_GENERIC_TIMERS_MODE ;hw2/hw4/hw6**

```

| 0-15 hw4: ENABLE (16bit) ;\hw4 (the other bits are
 16-31 hw4: - ;/moved to "MODE3" in hw4)
 0-7 hw6: ENABLE (8bit) ;\
 8-10 hw6: OVERFLOW_INDEX (3bit) (R) ; hw2/hw6
 11 hw6: - ;
 12-31 hw6: THRESH (20bit) ;/ |
| --- |

**ATH:004200h/005134h - MAC_PCU_GENERIC_TIMERS_MODE2 ;hw4/hw6 (not hw2)**

```

| 0-15 hw4: ENABLE (16bit) ;-hw4
 0-7 hw6: ENABLE (8bit) ;\
 8-11 hw6: OVERFLOW_INDEX (4bit) (R) ; hw6
 12-15 hw6: OVERFLOW_INDEX2 (4bit) (R) ;/ |
| --- |

**ATH:0042D4h - MAC_PCU_GENERIC_TIMERS_MODE3 ;hw4 only**

```

| 0-19 hw4: THRESH ;\hw4 only (in hw2/hw6 this stuff is
 24-27 hw4: OVERFLOW_INDEX ;/located in "MODE" instead of "MODE3") |
| --- |

**ATH:005150h - MAC_PCU_GENERIC_TIMERS_TSF_SEL ;hw6 only**

```

| 0-15 VALUE |
| --- |

```

| __________________________ below in hw4/hw6 only __________________________ |
| --- |

**ATH:004118h/005148h - WLAN_HT (aka HT) ;hw4/hw6**

```

| 0 MODE |
| --- |

**ATH:00411Ch/005054h - MAC_PCU_TSF_L32 ;hw4/hw6 (hw2: see REG_TSF_L32)**

**ATH:004120h/005058h - MAC_PCU_TSF_U32 ;hw4/hw6**

```

| 0-63 VALUE |
| --- |

**ATH:0042CCh/0050D4h - MAC_PCU_TSF2_L32 ;hw4/hw6**

**ATH:0042D0h/0050D8h - MAC_PCU_TSF2_U32 ;hw4/hw6**

```

| 0-63 VALUE |
| --- |

**ATH:00505Ch - MAC_PCU_WBTIMER_0 ;hw6**

```

| 0 ENABLE ;-hw6 only |
| --- |

**ATH:004124h/005060h - MAC_PCU_WBTIMER_1 (aka MAC_PCU_WBTIMER) ;hw4/hw6**

```

| 0-31 VALUE |
| --- |

**ATH:00420Ch/0050D0h - MAC_PCU_RESET_TSF ;hw4/hw6**

```

| 24 ONE_SHOT ;aka "one shot RESET_TSF" ;<-- see "REG_BEACON" in hw2
 25 ONE_SHOT2 |
| --- |

**ATH:00427Ch/005154h - MAC_PCU_BMISS_TIMEOUT ;hw4/hw6**

**ATH:005158h - MAC_PCU_BMISS2_TIMEOUT ;hw6 only**

```

| 0-23 VALUE (aka BMISS_TIMEOUT)
 24 ENABLE (aka BMISS_TIMEOUT_ENABLE) |
| --- |

```

| ____________________________ below in hw2 only ____________________________ |
| --- |

**ATH:00410Ch - MAC_PCU_SLP_BEACON - hw2 only (removed or renamed in hw4)**

**ATH:028264h (mirror of 00410Ch) - MAC_PCU_REG_SLP5 (ini: 0FFFFFFh) ;hw2**

```

| 0-23 hw2: BMISS_TIMEOUT ;\hw2 only
 24 hw2: BMISS_TIMEOUT_ENABLE ;/ |
| --- |

See also: - MAC_PCU_BMISS_TIMEOUT <--- is that equivalent on hw4/hw6!?!

**ATH:004120h..413Ch - SDIO_SETUP_CIRCUIT[8] - hw2 only (removed in hw4/hw6)**

**ATH:004160h..417Ch - CPU_SETUP_CIRCUIT[8] - hw2 only (removed in hw4/hw6)**

**ATH:0041A0h..41BCh - BB_SETUP_CIRCUIT[8] - hw2 only (removed in hw4/hw6)**

```

| 0-7 hw2: VECTOR |
| --- |

**ATH:004140h - SDIO_SETUP_CONFIG - hw2 only (removed in hw4/hw6)**

**ATH:004144h - CPU_SETUP_CONFIG - hw2 only (removed in hw4/hw6)**

**ATH:004180h - BB_SETUP_CONFIG - hw2 only (removed in hw4/hw6)**

```

| 0 hw2: CLEAR
 1 hw2: ENABLE |
| --- |

```

| ____________________________ below in hw4 only ____________________________ |
| --- |

**ATH:004210h - MAC_PCU_TSF_ADD_PLL ;hw4**

```

| 0-7 hw4: VALUE |
| --- |

**ATH:004280h - MAC_PCU_CAB_AWAKE ;hw4**

```

| 0-15 hw4: DURATION
 16 hw4: ENABLE |
| --- |

**ATH:0042D8h - MAC_PCU_DIRECT_CONNECT ;hw4**

```

| 0 hw4: AP_STA_ENABLE
 1 hw4: AP_TSF_1_2_SEL
 2 hw4: STA_TSF_1_2_SEL |
| --- |

**ATH:004218h..004278h - Bluetooth Coex related ;hw4**

**ATH:004294h..0042A4h - Bluetooth Coex related, too? ;hw4**

These hw4 ports have been moved to 007xxxh in hw6. See Bluetooth Coex chapter
for details.

```

| ____________________________ below in hw6 only ____________________________ |
| --- |

**ATH:005000h - WLAN_RESET_CONTROL ;hw6**

```

| 0 MAC_WARM_RST ;-moved from 004000h.bit4 ;\
 1 MAC_COLD_RST ;-moved from 004000h.bit5 ;
 2 WARM_RST ;-moved from 004000h.bit7 ;
 3 COLD_RST ;-also in 004000h.bit8 ;
 4 BB_WARM_RST ;-moved from 004000h.bit12 ;
 5 BB_COLD_RST ;-moved from 004000h.bit13 ;/
 6 RADIO_SRESET ;-new hw6 bit ;\
 7 MCI_RESET ;-new hw6 bit ;/ |
| --- |

**ATH:005008h - WLAN_REG_CONTROL0 ;hw6**

```

| 0-31 SWREG_BITS |
| --- |

**ATH:00500Ch - WLAN_REG_CONTROL1 ;hw6**

```

| 0 SWREG_PROGRAM
 1-2 OTPREG_LVL |
| --- |

**ATH:005028h - WLAN_RESET_CAUSE ;hw6 ;<--- mirror of SOC_RESET_CAUSE?**

```

| 0-2 LAST (R) |
| --- |

**ATH:00502Ch - WLAN_SYSTEM_SLEEP ;hw6 ;<--partial mirror of SOC_SYSTEM_SLEEP?**

```

| 0 DISABLE
 1 LIGHT
 2 MAC_IF (R) |
| --- |

**ATH:00515Ch - RTC_AXI_AHB_BRIDGE ;hw6 only**

```

| 0-1 hw6: MAX_BEATS |
| --- |

**ATH:005160h - UNIFIED_MAC_REVID (R) ;hw6 only**

```

| 0-31 hw6: VALUE (R) |
| --- |

| DSi Atheros Wifi - Internal I/O - 0xx240h - RTC/Clock SYNC (hw6) |
| --- |

**ATH:xxx240h - RTC_SYNC_RESET**

```

| 0 RESET_L |
| --- |

**ATH:xxx244h - RTC_SYNC_STATUS**

```

| 0 SHUTDOWN_STATE (R)
 1 ON_STATE (R)
 2 SLEEP_STATE (R)
 3 WAKEUP_STATE (R)
 4 WRESET (R)
 5 PLL_CHANGING (R) |
| --- |

**ATH:xxx248h - RTC_SYNC_DERIVED**

```

| 0 BYPASS
 1 FORCE
 2 FORCE_SWREG_PWD (W)
 3 FORCE_LPO_PWD (W) |
| --- |

**ATH:xxx24Ch - RTC_SYNC_FORCE_WAKE**

```

| 0 ENABLE (R)
 1 INTR |
| --- |

**ATH:xxx250h - RTC_SYNC_INTR_CAUSE**

```

| 0 SHUTDOWN_STATE
 1 ON_STATE
 2 SLEEP_STATE
 3 WAKEUP_STATE
 4 SLEEP_ACCESS
 5 PLL_CHANGING |
| --- |

**ATH:xxx254h - RTC_SYNC_INTR_ENABLE**

```

| 0 SHUTDOWN_STATE
 1 ON_STATE
 2 SLEEP_STATE
 3 WAKEUP_STATE
 4 SLEEP_ACCESS
 5 PLL_CHANGING |
| --- |

**ATH:xxx258h - RTC_SYNC_INTR_MASK**

```

| 0 SHUTDOWN_STATE
 1 ON_STATE
 2 SLEEP_STATE
 3 WAKEUP_STATE
 4 SLEEP_ACCESS
 5 PLL_CHANGING |
| --- |

| DSi Atheros Wifi - Internal I/O - 006000h - WLAN Coex (MCI) (hw6) |
| --- |

**ATH:006000h - MCI_COMMAND0 ;hw6**

```

| 0-7 HEADER
 8-12 LEN
 13 DISABLE_TIMESTAMP |
| --- |

**ATH:006004h - MCI_COMMAND1 ;hw6**

```

| 0-31 ADDR |
| --- |

**ATH:006008h - MCI_COMMAND2 ;hw6**

```

| 0 RESET_TX
 1 RESET_RX
 2-9 RESET_RX_NUM_CYCLES |
| --- |

**ATH:00600Ch - MCI_RX_CTRL ;hw6**

```

| 0 DISABLE_TIMESTAMP
 1 DISABLE_MAXGAIN_RESET
 2 DISABLE_MAXGAIN_WBTIMER_RESET |
| --- |

**ATH:006010h - MCI_TX_CTRL ;hw6**

```

| 0-1 CLK_DIV
 2 DISABLE_LNA_UPDATES
 3-23 GAIN_UPDATE_FREQ
 24-27 GAIN_UPDATE_NUM |
| --- |

**ATH:006014h - MCI_MSG_ATTRIBUTES_TABLE ;hw6**

```

| 0-15 CHECKSUM_EN
 16-31 INVALID_HDR |
| --- |

**ATH:006018h - MCI_SCHD_TABLE_0 ;hw6**

```

| 0-31 BASE_ADDR |
| --- |

**ATH:00601Ch - MCI_SCHD_TABLE_1 ;hw6**

```

| 0-15 OWN
 16-31 SW_REQ_OWN |
| --- |

**ATH:006020h - MCI_GPM_0 ;hw6**

```

| 0-31 START_ADDR |
| --- |

**ATH:006024h - MCI_GPM_1 ;hw6**

```

| 0-15 LEN
 16-31 WRITE_PTR (R) |
| --- |

**ATH:006028h - MCI_INTERRUPT_RAW ;Interrupt Flags ;hw6**

**ATH:00602Ch - MCI_INTERRUPT_EN ;Interrupt Enable ;hw6**

```

| 0 SW_MSG_DONE
 1 CPU_INT_MSG
 2 RX_CKSUM_FAIL
 3 RX_INVALID_HDR
 4 RX_HW_MSG_FAIL
 5 RX_SW_MSG_FAIL
 7 TX_HW_MSG_FAIL
 8 TX_SW_MSG_FAIL
 9 RX_MSG
 10 REMOTE_SLEEP_UPDATE
 11-26 BT_PRI
 27 BT_PRI_THRESH
 28 BT_FREQ
 29 BT_STOMP |
| --- |

**ATH:006030h - MCI_REMOTE_CPU_INT ;Flags ;hw6**

**ATH:006034h - MCI_REMOTE_CPU_INT_EN ;Enable ;hw6**

```

| 0-31 BODY |
| --- |

**ATH:006038h - MCI_INTERRUPT_RX_MSG_RAW ;Flags ;hw6**

**ATH:00603Ch - MCI_INTERRUPT_RX_MSG_EN ;Enable ;hw6**

```

| 0 REMOTE_RESET
 1 LNA_CTRL
 2 CONT_NACK
 3 CONT_INFO
 4 CONT_RST
 5 SCHD_INFO
 6 CPU_INT
 8 GPM
 9 LNA_INFO
 10 SYS_SLEEPING
 11 SYS_WAKING
 12 REQ_WAKE |
| --- |

**ATH:006040h - MCI_CPU_INT ;hw6**

```

| 0-31 MSG |
| --- |

**ATH:006044h - MCI_RX_STATUS ;hw6**

```

| 8-11 SCHD_MSG_INDEX (R)
 12 REMOTE_SLEEP (R) |
| --- |

**ATH:006048h - MCI_CONT_STATUS ;hw6**

```

| 0-7 RSSI_POWER (R)
 8-15 PRIORITY (R)
 16 TX (R)
 17-20 LINKID (R)
 21-27 CHANNEL (R)
 28-31 OWNER (R) |
| --- |

**ATH:00604Ch - MCI_BT_PRI0 ;hw6**

**ATH:006050h - MCI_BT_PRI1 ;hw6**

**ATH:006054h - MCI_BT_PRI2 ;hw6**

**ATH:006058h - MCI_BT_PRI3 ;hw6**

```

| 0-7 VAL0
 8-15 VAL1
 16-23 VAL2
 24-31 VAL3 |
| --- |

**ATH:00605Ch - MCI_BT_PRI ;hw6**

```

| 0-7 THRESH |
| --- |

**ATH:006060h - MCI_WL_FREQ0 ;hw6**

```

| 0-31 MASK |
| --- |

**ATH:006064h - MCI_WL_FREQ1 ;hw6**

```

| 0-31 MASK |
| --- |

**ATH:006068h - MCI_WL_FREQ2 ;hw6**

```

| 0-15 MASK |
| --- |

**ATH:00606Ch - MCI_GAIN ;hw6**

```

| 0-7 OFFSET1
 8-15 OFFSET2 |
| --- |

**ATH:006070h - MCI_WBTIMER1 ;hw6**

**ATH:006074h - MCI_WBTIMER2 ;hw6**

**ATH:006078h - MCI_WBTIMER3 ;hw6**

**ATH:00607Ch - MCI_WBTIMER4 ;hw6**

```

| 0-31 TARGET |
| --- |

**ATH:006080h - MCI_MAXGAIN ;hw6**

```

| 0-7 GAIN1
 8-15 GAIN2
 16-23 GAIN3
 24-31 GAIN4 |
| --- |

**ATH:0060ACh - BTCOEX_CTRL ;hw6**

```

| 2 MCI_MODE_ENABLE |
| --- |

**ATH:00614Ch - BTCOEX_CTRL2 ;hw6**

```

| 0-7 RESERVED2
 8-10 OBS_SEL |
| --- |

**ATH:006254h - BTCOEX_DBG ;hw6**

```

| 0-31 OBS (R) |
| --- |

**ATH:006258h - MCI_LAST_HW_MSG_HDR ;hw6**

```

| 0-7 HDR (R)
 8-10 LEN (R) |
| --- |

**ATH:00625Ch - MCI_LAST_HW_MSG_BDY ;hw6**

```

| 0-31 BDY (R) |
| --- |

**ATH:006260h - MCI_MAXGAIN_RST ;hw6**

```

| 0-31 TARGET |
| --- |

| DSi Atheros Wifi - Internal I/O - 00x000h - Bluetooth Coex (hw4/hw6) |
| --- |

**ATH:004218h/007000h - BTCOEXCTRL - hw4/hw6**

```

| 0-7 GAP
 8 CLK_CNT_EN
 9 FRAME_CNT_EN
 10 IDLE_CNT_EN
 11 SYNC_DET_EN
 12-17 PRIORITY_TIME
 18-22 FREQ_TIME
 23-24 PTA_MODE
 25 WBSYNC_ON_BEACON
 26 hw4: WBTIMER_ENABLE ;hw4 only
 27 unspecified
 28 hw6: RFGAIN_VALID_SRC ;hw6 only |
| --- |

**ATH:004228h/007010h - BTCOEX0 ;SYNC_DUR - hw4/hw6**

```

| 0-7 SYNC_DUR |
| --- |

**ATH:00422Ch/007014h - BTCOEX1 ;CLK_THRES - hw4/hw6**

```

| 0-20 CLK_THRES |
| --- |

**ATH:004230h/007018h - BTCOEX2 ;FRAME_THRES - hw4/hw6**

```

| 0-7 FRAME_THRES |
| --- |

**ATH:004234h/00701Ch - BTCOEX3 ;CLK_CNT - hw4/hw6**

```

| 0-20 CLK_CNT |
| --- |

**ATH:004238h/007020h - BTCOEX4 ;FRAME_CNT - hw4/hw6**

```

| 0-7 FRAME_CNT |
| --- |

**ATH:00423Ch/007024h - BTCOEX5 ;IDLE_CNT - hw4/hw6**

```

| 0-15 IDLE_CNT |
| --- |

**ATH:004240h/007028h - BTCOEX6 ;IDLE_RESET_LVL_BITMAP - hw4/hw6**

```

| 0-31 IDLE_RESET_LVL_BITMAP |
| --- |

**ATH:004244h/00702Ch - LOCK - hw4/hw6**

```

| 0-7 TUNLOCK_MASTER
 8-15 TLOCK_MASTER
 16-23 TUNLOCK_SLAVE
 24-31 TLOCK_SLAVE |
| --- |

**ATH:00421Ch/007004h - WBSYNC_PRIORITY1 - hw4/hw6**

**ATH:004220h/007008h - WBSYNC_PRIORITY2 - hw4/hw6**

**ATH:004224h/00700Ch - WBSYNC_PRIORITY3 - hw4/hw6**

**ATH:004248h/007030h - NOLOCK_PRIORITY - hw4/hw6**

```

| 0-31 BITMAP |
| --- |

**ATH:00424Ch/007034h - WBSYNC - hw4/hw6**

**ATH:004250h/007038h - WBSYNC1 - hw4/hw6**

**ATH:004254h/00703Ch - WBSYNC2 - hw4/hw6**

**ATH:004258h/007040h - WBSYNC3 - hw4/hw6**

```

| 0-31 BTCLOCK (R) (read-only, according to hw6) |
| --- |

**ATH:00425Ch/007044h - WB_TIMER_TARGET - hw4/hw6**

```

| 0-31 VALUE |
| --- |

**ATH:004260h/007048h - WB_TIMER_SLOP - hw4/hw6**

```

| 0-9 VALUE |
| --- |

**ATH:004264h/00704Ch - BTCOEX_INT_EN - hw4/hw6**

**ATH:004268h/007050h - BTCOEX_INT_STAT - hw4/hw6**

```

| 0 CLK_CNT
 1 FRAME_CNT
 2 END
 3 SYNC
 4 NOSYNC
 5 BTPRIORITY ;<-- for INT_STAT (but, N/A for INT_EN) (R)
 6 BTPRIORITY_STOMP ;<-- for INT_STAT (but, N/A for INT_EN) (R)
 7 WB_TIMER
 8 I2C_MESG_RECV ;<-- for INT_STAT (but, "ST_MESG_RECV" for INT_EN?)
 9 I2C_MESG_SENT
 10 I2C_TX_FAILED
 11 I2C_RECV_OVERFLOW |
| --- |

**ATH:00426Ch/007054h - BTPRIORITY_INT_EN - hw4/hw6**

**ATH:004270h/007058h - BTPRIORITY_INT_STAT - hw4/hw6**

```

| 0-31 BITMAP |
| --- |

**ATH:004274h/00705Ch - BTPRIORITY_STOMP_INT_EN - hw4/hw6**

**ATH:004278h/007060h - BTPRIORITY_STOMP_INT_STAT - hw4/hw6**

```

| 0-31 BITMAP |
| --- |

**ATH:004294h/007064h - ST_64_BIT - hw4/hw6**

```

| 0 MODE
 1-5 SOC_CLK_DIVIDE_RATIO
 6 CLOCK_GATE
 7 DRIVE_MODE
 8 REQ_ACK_NOT_PULLED_DOWN
 9-26 TIMEOUT |
| --- |

**ATH:004298h/007068h - MESSAGE_WR - hw4/hw6**

**ATH:0042A0h/007070h - MESSAGE_RD - hw4/hw6**

```

| 0-31 TYPE |
| --- |

**ATH:00429Ch/00706Ch - MESSAGE_WR_P - hw4/hw6**

**ATH:0042A4h/007074h - MESSAGE_RD_P - hw4/hw6**

```

| 0-31 PARAMETER |
| --- |

```

| _________ below hw6 only _________ |
| --- |

**ATH:007078h - BTPRIORITY_INT - hw6 only**

```

| 0-7 DELAY |
| --- |

**ATH:00707Ch - SCO_PARAMS - hw6 only**

```

| 0-13 PERIOD
 14-23 SLOP |
| --- |

**ATH:007080h - SCO_PRIORITY - hw6 only**

```

| 0-31 BITMAP |
| --- |

**ATH:007084h - SCO_SYNC - hw6 only**

```

| 0-31 BTCLOCK |
| --- |

**ATH:007088h - BTCOEX_RAW_STAT - hw6 only**

```

| 0 CLK_CNT
 1 FRAME_CNT
 2 END
 3 SYNC
 4 NOSYNC
 7 WB_TIMER |
| --- |

**ATH:00708Ch - BTPRIORITY_RAW_STAT - hw6 only**

```

| 0-31 BITMAP |
| --- |

**ATH:007090h - BTPRIORITY_STOMP_RAW_STAT - hw6 only**

```

| 0-31 BITMAP |
| --- |

| DSi Atheros Wifi - Internal I/O - 00x000h - Memory Control (hw2/hw4/hw6) |
| --- |

**TCAM/BCAM (Ternary/Binary Content Addressable Memory) (ROM Patches)**

TCAM/BCAM registers are allowing to patch ROM (in a similar fashion as Game
Genie cheat devices). The ROM patches can be initialized via BMI commands:

DSi Atheros Wifi - BMI Bootloader Commands

Many ROM functions are called via a Table in RAM, which can be patched without
needing the TCAM/BCAM feature (actually, the DSi's AR6002 firmware is patching
only three of that RAM Table entries, and leaves the TCAM feature completely
unused).

```

| ______________________ hw2 ROM Patches (TCAM) ______________________ |
| --- |

**ATH:008000h..807Ch - (WLAN_)MC_TCAM_VALID[0..31] ;hw2**

```

| 0 BIT (?=Patch Enable) |
| --- |

**ATH:008080h..80FCh - (WLAN_)MC_TCAM_MASK[0..31] ;hw2**

```

| 0-2 SIZE (... patch area, selectable 32-bytes or bigger or so?) |
| --- |

The eight size settings are probably 20h,40h,80h,100h,200h,400h,800h,1000h.

**ATH:008100h..817Ch - (WLAN_)MC_TCAM_COMPARE[0..31] ;hw2**

```

| 5-21 KEY (Patch ROM Address in 32-byte steps) (probably 0E0000h and up?) |
| --- |

**ATH:008180h..81FCh - (WLAN_)MC_TCAM_TARGET[0..31] ;hw2**

```

| 5-21 ADDR (Patch RAM Address in 32-byte steps) (probably 100000h and up?) |
| --- |

```

| ______________________ hw4 ROM Patches (BCAM) ______________________ |
| --- |

The hw4 ROM patching is done in 32bit Data units, but 16bit/24bit Xtensa
opcodes aren't neccessarily 32bit aligned, so one may need 1-2 patch slots per
opcode.

**ATH:008000h..0081FCh - WLAN_MC_BCAM_VALID[0..127] ;hw4**

```

| 0 BIT some "bit" (128 x 1bit) (?=Patch Enable)
 1-31 - |
| --- |

**ATH:008200h..0083FCh - WLAN_MC_BCAM_COMPARE[0..127] ;hw4**

```

| 0-1 -
 2-19 KEY some "key" (128 x 18bit) (Patch Address in 4-byte steps)
 20-31 - |
| --- |

**ATH:008400h..0085FCh - WLAN_MC_BCAM_TARGET[0..127] ;hw4**

```

| 0-31 INST some "inst" (128 x 32bit) (Patch Data) |
| --- |

**ATH:008610h - WLAN_BCAM_CONFLICT_ERROR ;hw4**

```

| 0 DPORT_FLAG
 1 IPORT_FLAG
 2-31 - |
| --- |

```

| _______________________ hw6 ROM Patches (?) _______________________ |
| --- |

Unknown if or how ROM Patches are supported on hw6 (the hw6 source code doesn't
define any TCAM/BCAM registers).

```

| ______________________ ADDR_ERROR Registers ______________________ |
| --- |

**ATH:008200h - (WLAN_)ADDR_ERROR_CONTROL ;hw2**

**ATH:008600h - WLAN_APB_ADDR_ERROR_CONTROL ;hw4**

**ATH:010018h - WLAN_APB_ADDR_ERROR_CONTROL ;hw6**

```

| 0 ENABLE
 1 QUAL_ENABLE
 2-31 - |
| --- |

**ATH:008204h - (WLAN_)ADDR_ERROR_STATUS ;hw2**

**ATH:008604h - WLAN_APB_ADDR_ERROR_STATUS ;hw4**

**ATH:01001Ch - WLAN_APB_ADDR_ERROR_STATUS ;hw6**

```

| 0-24 ADDRESS
 25 WRITE
 26-31 - |
| --- |

**ATH:008608h - WLAN_AHB_ADDR_ERROR_CONTROL ;hw4**

**ATH:010020h - WLAN_AHB_ADDR_ERROR_CONTROL ;hw6**

```

| 0 ENABLE
 1-31 - |
| --- |

**ATH:00860Ch - WLAN_AHB_ADDR_ERROR_STATUS ;hw4**

**ATH:010024h - WLAN_AHB_ADDR_ERROR_STATUS ;hw6**

```

| 0-23 ADDRESS
 24-29 -
 30 MBOX
 31 MAC |
| --- |

```

| ______________________ hw4 MISC Registers ______________________ |
| --- |

**ATH:008614h - WLAN_CPU_PERF_CNT ;hw4**

```

| 0 EN
 1-31 - |
| --- |

**ATH:008618h - WLAN_CPU_INST_FETCH ;hw4**

**ATH:00861Ch - WLAN_CPU_DATA_FETCH ;hw4**

```

| 0-31 CNT |
| --- |

**ATH:008620h - WLAN_CPU_RAM1_CONFLICT ;hw4**

**ATH:008624h - WLAN_CPU_RAM2_CONFLICT ;hw4**

**ATH:008628h - WLAN_CPU_RAM3_CONFLICT ;hw4**

**ATH:00862Ch - WLAN_CPU_RAM4_CONFLICT ;hw4**

```

| 0-11 CNT
 12-31 - |
| --- |

```

| ______________________ hw6 MISC Registers ______________________ |
| --- |

**ATH:010028h - WLAN_AHB_CONFIG ;hw6**

```

| 0 MAX_BURST_4
 1 MAX_BURST_8
 2 MAX_BURST_16 |
| --- |

**ATH:01002Ch - WLAN_MEMORY_MAP ;hw6**

```

| 0 ONE_IRAM_BANK
 1 TWO_IRAM_BANKS
 2 THREE_IRAM_BANKS
 3 FOUR_IRAM_BANKS |
| --- |

```

| ______________________ Xtensa CPU ______________________ |
| --- |

**Xtensa Region/MMU (hw2)**

The Xtensa CPU is additionally having a Region/MMU unit with ITLB and DTLB. The
AR6002 ROM is doing some basic initialization via mmu-opcodes:

```

| set ITLB[(0..7)*20000000h] to values (1,2,2,2,2,2,2,2)
 set DTLB[(0..7)*20000000h] to values (1,2,2,2,2,2,2,2) |
| --- |

Alongsides, it's issuing an "isync" opcode after setting the first ITLB entry,
and a "dsync" opcode after setting the last DTLB entry. After that
initialization, the ROM and Firmware aren't using any further mmu-opcodes.

| DSi Atheros Wifi - Internal I/O - 00C000h - Serial UART (hw2/hw4/hw6) |
| --- |

The UART_xxx registers are used to output TTY messages (ASCII strings) when
enabled in the "Host Interest" area:

```

| LOCAL_SCRATCH[0].bit1 AR6K_OPTION_SERIAL_ENABLE --> TTY master enable
 targaddr[14h] hi_serial_enable --> enable additional TTY msg's during BMI
 targaddr[60h] hi_desired_baud_rate --> for TTY/UART (default=9600 decimal)
 targaddr[C4h] hi_console_flags - whatever, UART related, maybe newer firmware |
| --- |

```

| ______________________ hw2 UART Registers ______________________ |
| --- |

**Texas Instruments TL16C550AN - Asynchronous Communications Element (ACE)**

The hw2 UART is based on the TL16C550AN chip (which is also found in the SNES
"Exertainment Bicycle" add-on).

**ATH:00C000h (when DLAB=0) - (WLAN_UART_)RBR - RX Data FIFO (R) ;hw2**

**ATH:00C020h (when DLAB=0) - (WLAN_UART_)SRBR (mirror of RBR?) ;hw2**

```

| 0-7 Data (with 16-byte FIFO) |
| --- |

**ATH:00C000h (when DLAB=0) - (WLAN_UART_)THR - TX Data FIFO (W) ;hw2**

```

| 0-7 Data (with 16-byte FIFO) |
| --- |

**ATH:00C004h (when DLAB=0) - (WLAN_UART_)IER - Interrupt Control (R/W) ;hw2**

```

| 0 ERBFI Received Data Available Interrupt (0=Disable, 1=Enable)
 1 ETBEI Transmitter Holding Register Empty Interrupt (0=Disable, 1=Enable)
 2 ELSI Receiver Line Status Interrupt (0=Disable, 1=Enable)
 3 EDDSI Modem Status Interrupt (0=Disable, 1=Enable)
 4-7 - Not used (always zero) |
| --- |

**ATH:00C000h (when DLAB=1) - (WLAN_UART_)DLL - Baudrate Divisor LSB (R/W) ;hw2**

**ATH:00C004h (when DLAB=1) - (WLAN_UART_)DLH - Baudrate Divisor MSB (R/W) ;hw2**

```

| 0-7 Divisor Latch LSB/MSB, should be set to "divisor = XIN / (baudrate*16)" |
| --- |

**ATH:00C008h - (WLAN_UART_)IIR - Interrupt Status (R) ;hw2**

**ATH:00C028h - (WLAN_UART_)SIIR (mirror or IIR?) ;hw2**

```

| 0 Interrupt Pending Flag (0=Pending, 1=None) ;\IID
 1-3 Interrupt ID, 3bit (0..7=see below) (always 00h when Bit0=1) ;/
 4-5 Not used (always zero)
 6 FIFOs Enabled (always zero in TL16C450 mode) ;\these bits have same
 7 FIFOs Enabled (always zero in TL16C450 mode) ;/value as "FIFO Enable" |
| --- |

The 3bit Interrupt ID can have following values:

```

| ID Prio Expl.
 00h 4 Handshaking inputs CTS,DSR,RI,DCD have changed (Ack: Read MSR)
 01h 3 Transmitter Holding Register Empty (Ack: Write THR or Read IIR)
 02h 2 RX FIFO has reached selected trigger level (Ack: Read RBR)
 03h 1 RX Overrun/Parity/Framing Error, or Break Interrupt (Ack: Read LSR)
 06h 2 RX FIFO non-empty & wasn't processed for longer time(Ack: Read RBRh) |
| --- |

Interrupt ID values 04h,05h,07h are not used.

**ATH:00C008h - (WLAN_UART_)FCR - FIFO Control (W) ;hw2**

```

| 0 FIFO Enable (0=Disable, 1=Enable) (Enables access to FIFO related bits)
 1 Receiver FIFO Reset (0=No Change, 1=Clear RX FIFO) (RCVR_FIFO_RST)
 2 Transmitter FIFO Reset (0=No Change, 1=Clear TX FIFO) (XMIT_FIFO_RST)
 3 DMA Mode Select (Mode for /RXRDY and /TXRDY) (0=Mode 0, 1=Mode 1)
 4-5 Not used (should be zero)
 6-7 Receiver FIFO Trigger (0..3 = 1,4,8,14 bytes) (RCVR_TRIG) |
| --- |

**ATH:00C00Ch - (WLAN_UART_)LCR - Character Format Control (R/W) ;hw2**

```

| 0-1 Character Word Length (0..3 = 5,6,7,8 bits) (CLS)
 2 Number of Stop Bits (0=1bit, 1=2bit; for 5bit chars: only 1.5bit)
 3 Parity Enable (PEN) (0=None, 1=Enable Parity or 9th data bit)
 4 Parity Type/9th Data bit (0=Odd, 1=Even) (EPS)
 5 Unused in hw2? ;for TL16C550AN: Bit4-5 can be 2=Set9thBit, 3=Clear9thBit
 6 Set Break (0=Normal, 1=Break, Force SOUT to Low)
 7 Divisor Latch Access (0=Normal I/O, 1=Divisor Latch I/O) (DLAB) |
| --- |

**ATH:00C010h - (WLAN_UART_)MCR - Handshaking Control (R/W) ;hw2**

```

| 0 DTR Output Level for /DTR pin (Data Terminal Ready) (0=High, 1=Low)
 1 RTS Output Level for /RTS pin (Request to Send) (0=High, 1=Low)
 2 OUT1 Output Level for /OUT1 pin (General Purpose) (0=High, 1=Low)
 3 OUT2 Output Level for /OUT2 pin (General Purpose) (0=High, 1=Low)
 4/5? LOOP Loopback Mode (0=Normal, 1=Testmode, loopback TX to RX)
 5-7 Not used (always zero) |
| --- |

The Loopback bit should be Bit4 (according to TL16C550AN datasheet), but hw2
source code claims it to be in bit5.

**ATH:00C014h - (WLAN_UART_)LSR - RX/TX Status (R) (W=don't do) ;hw2**

**ATH:00C034h - (WLAN_UART_)SLSR (mirror of LSR?) ;hw2**

```

| 0 RX Data Ready (DR) (0=RX FIFO Empty, 1=RX Data Available)
 1 RX Overrun Error (OE) (0=Okay, 1=Error) (RX when RX FIFO Full)
 2 RX Parity Error (PE) (0=Okay, 1=Error) (RX parity bad)
 3 RX Framing Error (FE) (0=Okay, 1=Error) (RX stop bit bad)
 4 RX Break Interrupt (BI) (0=Normal, 1=Break) (RX line LOW for long time)
 5 Transmitter Holding Register (THRE) (1=TX FIFO is empty)
 6 Transmitter Empty (TEMT) (0=No, 1=Yes, TX FIFO and TX Shift both empty)
 7 At least one Overrun/Parity/Framing Error in RX FIFO (0=No, 1=Yes/Error) |
| --- |

Bit7 is always zero in TL16C450 mode. Bit1-3 are automatically cleared after
reading. In FIFO mode, bit2-3 reflect to status of the current (=oldest)
character in the FIFO (unknown/unclear if bit2-3 are also auto-cleared when in
FIFO mode).

Note: The AR6002 BIOS ROM is using "SLSR" (instead of "LSR") for testing
bit0,5,6. And, before each read from SLSR register, the AR6002 BIOS BIOS does
first write 0 to SLSR (for whatever unknown purpose).

Basically, "SLSR" (and other "Sxxx" registers) seems to be some sort of a
mirror of "LSR" (and other "xxx" registers)? Maybe one of them omits automatic
IRQ acknowledge or so?

**ATH:00C018h - (WLAN_UART_)MSR - Handshaking Status (R) (W=don't do) ;hw2**

**ATH:00C038h - (WLAN_UART_)SMSR (mirror or MSR?) ;hw2**

```

| 0 DCTS Change flag for /CTS pin ;ClearToSend ;\change flags (0=none,
 1 DDSR Change flag for /DSR pin ;DataSetReady ; 1=changed since last
 2 TERI Change flag for /RI pin ;RingIndicator ; read) (automatically
 3 DDCD Change flag for /DCD pin ;DataCarrierDetect ;/cleared after read)
 4 CTS Input Level on /CTS pin ;ClearToSend ;\
 5 DSR Input Level on /DSR pin ;DataSetReady ; current levels
 6 RI Input Level on /RI pin ;RingIndicator ; (inverted ?)
 7 DCD Input Level on /DCD pin ;DataCarrierDetect ;/ |
| --- |

**ATH:00C01Ch - (WLAN_UART_)SCR - Scratch (R/W) ;hw2**

```

| 0-7 General Purpose Storage (eg. read/write-able for UART chip detection) |
| --- |

**ATH:00C02Ch - (WLAN_UART_?)MWR ;whatever "M Write Register?" ;hw2**

**ATH:00C03Ch - (WLAN_UART_?)MRR ;whatever "M Read Register?" ;hw2**

```

| 0-31 whatever... 32bit wide (unlike other UART registers) (?) (UART related?) |
| --- |

```

| ______________________ hw4/hw6 UART Registers ______________________ |
| --- |

**Multiple UARTs ;hw4/hw6**

There appear to be multiple hw4/hw6 UARTs: one normal, one for debug, one for
hw6:

```

| WLAN_UART_BASE_ADDRESS = 0000C000h ;hw4/hw6
 WLAN_DBG_UART_BASE_ADDRESS = 0000D000h ;hw4/hw6
 WLAN_UART2_BASE_ADDRESS = 00054C00h ;hw6 |
| --- |

Maybe the UARTs are all using the same register format with differerent base?

**ATH:00C000h - UART_DATA ;hw4/hw6**

```

| 0-7 TXRX_DATA
 8 RX_CSR
 9 TX_CSR |
| --- |

**ATH:00C004h - UART_CONTROL ;hw4/hw6**

```

| 0 PARITY_EVEN
 1 PARITY_ENABLE
 2 IFC_DCE
 3 IFC_ENABLE
 4 FLOW_INVERT
 5 FLOW_ENABLE
 6 DMA_ENABLE
 7 RX_READY_ORIDE
 8 TX_READY_ORIDE
 9 SERIAL_TX_READY
 10 RX_BREAK
 11 TX_BREAK
 12 HOST_INT
 13 HOST_INT_ENABLE
 14 TX_BUSY
 15 RX_BUSY |
| --- |

**ATH:00C008h - UART_CLKDIV ;hw4/hw6**

```

| 0-15 CLK_STEP
 16-23 CLK_SCALE |
| --- |

**ATH:00C00Ch - UART_INT ;hw4/hw6**

**ATH:00C010h - UART_INT_EN ;hw4/hw6**

```

| 0 RX_VALID_INT
 1 TX_READY_INT
 2 RX_FRAMING_ERR_INT
 3 RX_OFLOW_ERR_INT
 4 TX_OFLOW_ERR_INT
 5 RX_PARITY_ERR_INT
 6 RX_BREAK_ON_INT
 7 RX_BREAK_OFF_INT
 8 RX_FULL_INT
 9 TX_EMPTY_INT |
| --- |

| DSi Atheros Wifi - Internal I/O - 00E000h - UMBOX Registers (hw4/hw6) |
| --- |

**ATH:00E000h..00E004h - UMBOX_FIFO[0..1]**

```

| 0-8 DATA ... uh, twice[0..1], with 9bit each ? |
| --- |

**ATH:00E008h - UMBOX_FIFO_STATUS**

```

| 0 RX_FULL
 1 RX_EMPTY
 2 TX_FULL
 3 TX_EMPTY |
| --- |

**ATH:00E00Ch - UMBOX_DMA_POLICY**

```

| 0 RX_ORDER
 1 RX_QUANTUM
 2 TX_ORDER
 3 TX_QUANTUM |
| --- |

**ATH:00E010h - UMBOX0_DMA_RX_DESCRIPTOR_BASE**

**ATH:00E018h - UMBOX0_DMA_TX_DESCRIPTOR_BASE**

```

| 2-27 ADDRESS |
| --- |

**ATH:00E014h - UMBOX0_DMA_RX_CONTROL**

**ATH:00E01Ch - UMBOX0_DMA_TX_CONTROL**

```

| 0 STOP
 1 START
 2 RESUME |
| --- |

**ATH:00E020h - UMBOX_FIFO_TIMEOUT**

```

| 0-7 VALUE
 8 ENABLE_SET |
| --- |

**ATH:00E024h - UMBOX_INT_STATUS**

**ATH:00E028h - UMBOX_INT_ENABLE**

```

| 0 RX_NOT_FULL
 1 TX_NOT_EMPTY
 2 RX_UNDERFLOW
 3 TX_OVERFLOW
 4 HCI_SYNC_ERROR
 5 TX_DMA_COMPLETE
 6 TX_DMA_EOM_COMPLETE
 7 RX_DMA_COMPLETE
 8 HCI_FRAMER_OVERFLOW
 9 HCI_FRAMER_UNDERFLOW |
| --- |

**ATH:00E02Ch - UMBOX_DEBUG**

```

| 0-2 SEL |
| --- |

**ATH:00E030h - UMBOX_FIFO_RESET**

```

| 0 INIT |
| --- |

**ATH:00E034h - UMBOX_HCI_FRAMER**

```

| 0-1 CONFIG_MODE
 2 OVERFLOW
 3 UNDERFLOW
 4 SYNC_ERROR
 5 ENABLE
 6 CRC_OVERRIDE |
| --- |

| DSi Atheros Wifi - Internal I/O - 010000h - Serial I2C/SPI (hw2/hw4/hw6) |
| --- |

These registers are providing a general purpose I2C/SPI serial bus. The SI_xxx
registers are exactly same in hw2/hw4/hw6.

In the DSi, they are used in I2C mode - for reading wifi calibration data:

DSi Atheros Wifi I2C EEPROM

**ATH:010000h - SI_CONFIG**

```

| 0-3 DIVIDER (probably transfer rate, should be 6 on DSi)
 4 INACTIVE_CLK (whatever, should be 1 for I2C)
 5 INACTIVE_DATA (whatever, should be 1 for I2C)
 6 POS_DRIVE (whatever, should be zero for I2C)
 7 POS_SAMPLE (whatever, should be 1 for I2C)
 8-15 -
 16 I2C (0=SPI, 1=I2C)
 17 -
 18 BIDIR_OD_DATA (whatever, should be 1 for I2C)
 19 ERR_INT (whatever, enable or status?) |
| --- |

On DSi, this is register is set to 500B6h.

**ATH:010004h - SI_CS**

```

| 0-3 TX_CNT Number of TX bytes (0..8) (should be 1..8 for I2C device)
 4-7 RX_CNT Number of RX bytes (0..8)
 8 START Write 1 to start transfer
 9 DONE_INT Status (0=Busy, 1=Done/Okay)
 10 DONE_ERR Status (1=Error)
 11-13 BIT_CNT_IN_LAST_BYTE (0=Normal/8bit, 1..7=whatever) |
| --- |

For I2C with TX_CNT and RX_CNT both nonzero: TX data is transferred first.

Unknown when the DONE flags are cleared (possibly when writing 0 to bit9,10, or
when writing 1 to bit8, or maybe automatically after reading).

**ATH:010008h..01000Ch - SI_TX_DATA0/SI_TX_DATA1**

```

| 0-7 DATA0 1st TX byte (device number in case of I2C mode)
 8-15 DATA1 2nd TX byte (if any)
 16-23 DATA2 ...
 24-31 DATA3 ..
 32-39 DATA4
 40-47 DATA5
 48-55 DATA6
 56-63 DATA7 |
| --- |

**ATH:010010h..010014h - SI_RX_DATA0/SI_RX_DATA1**

```

| 0-7 DATA0 1st RX byte (if any)
 8-15 DATA1 2nd RX byte (if any)
 16-23 DATA2 ...
 24-31 DATA3 ..
 32-39 DATA4
 40-47 DATA5
 48-55 DATA6
 56-63 DATA7 |
| --- |

| DSi Atheros Wifi - Internal I/O - 014000h - GPIO 18/26/57 pin (hw2/hw4/hw6) |
| --- |

**ATH:014000h/014000h/014000h - WLAN_GPIO_OUT ;GPIO Data Out ;hw2/hw4/hw6**

**ATH:014004h/014004h/014004h - WLAN_GPIO_OUT_W1TS ;Write-1-to-Set ;hw2/hw4/hw6**

**ATH:014008h/014008h/014008h - WLAN_GPIO_OUT_W1TC ;Write-1-to-Clr ;hw2/hw4/hw6**

**ATH:01400Ch - WLAN_GPIO_OUT_HIGH ;for pin32 and up ;hw6**

**ATH:014010h - WLAN_GPIO_OUT_W1TS_HIGH ;for pin32 and up ;hw6**

**ATH:014014h - WLAN_GPIO_OUT_W1TC_HIGH ;for pin32 and up ;hw6**

```

| 0-17 hw2: DATA (for pin 0..17)
 0-25 hw4: DATA (for pin 0..25)
 0-63 hw6: DATA (for pin 0..56) (and bit57-63=unused or so?) |
| --- |

**ATH:01400Ch/01400Ch/014018h - WLAN_GPIO_ENABLE ;GPIO Out Enable ;hw2/hw4/hw6**

**ATH:014010h/014010h/01401Ch - WLAN_GPIO_ENABLE_W1TS ;Wr-1-to-Set ;hw2/hw4/hw6**

**ATH:014014h/014014h/014020h - WLAN_GPIO_ENABLE_W1TC ;Wr-1-to-Clr ;hw2/hw4/hw6**

**ATH:014024h - WLAN_GPIO_ENABLE_HIGH ;for pin32 and up ;hw6**

**ATH:014028h - WLAN_GPIO_ENABLE_W1TS_HIGH ;for pin32 and up ;hw6**

**ATH:01402Ch - WLAN_GPIO_ENABLE_W1TC_HIGH ;for pin32 and up ;hw6**

```

| 0-17 hw2: DATA (for pin 0..17)
 0-25 hw4: DATA (for pin 0..25)
 0-63 hw6: DATA (for pin 0..56) (and bit57-63=unused or so?) |
| --- |

**ATH:014018h/014018h/014030h - WLAN_GPIO_IN - GPIO Data In ;hw2/hw4/hw6**

**ATH:014038h - WLAN_GPIO_IN_HIGH ;for pin32 and up ;hw6**

```

| 0-17 hw2: DATA (for pin 0..17)
 0-25 hw4: DATA (for pin 0..25)
 0-63 hw6: DATA (for pin 0..56) (and bit57-63=unused or so?) |
| --- |

**ATH:01401Ch/01401Ch/014034h - WLAN_GPIO_STATUS - GPIO Interrupt ;hw2/hw4/hw6**

**ATH:014020h/014020h/014040h - WLAN_GPIO_STATUS_W1TS ;Wr-1-to-Set ;hw2/hw4/hw6**

**ATH:014024h/014024h/014044h - WLAN_GPIO_STATUS_W1TC ;Wr-1-to-Clr ;hw2/hw4/hw6**

**ATH:01403Ch - WLAN_GPIO_STATUS_HIGH ;for pin32 and up ;hw6**

**ATH:014048h - WLAN_GPIO_STATUS_W1TS_HIGH ;for pin32 and up ;hw6**

**ATH:01404Ch - WLAN_GPIO_STATUS_W1TC_HIGH ;for pin32 and up ;hw6**

```

| 0-17 hw2: INTERRUPT (for pin 0..17)
 0-25 hw4: INTERRUPT (for pin 0..25)
 0-63 hw6: INTERRUPT (for pin 0..56) (and bit57-63=unused or so?) |
| --- |

```

| ______________________________ hw2 GPIO ports ______________________________ |
| --- |

**ATH:014028h - GPIO_PIN0 ;GPIO0 Bluetooth coex BT_PRIORITY**

**ATH:01402Ch - GPIO_PIN1 ;GPIO1 Bluetooth coex WLAN_ACTIVE**

**ATH:014030h - GPIO_PIN2 ;GPIO2 Bluetooth coex BT_FREQUENCY ;I2C SCL**

**ATH:014034h - GPIO_PIN3 ;GPIO3 Bluetooth coex BT_ACTIVE ;I2C_SDA**

**ATH:014038h - GPIO_PIN4 ;GPIO4 SDIO/GSPI interface select**

**ATH:01403Ch - GPIO_PIN5 ;GPIO5 SDIO/GSPI interface select**

**ATH:014040h - GPIO_PIN6 ;GPIO6 -**

**ATH:014044h - GPIO_PIN7 ;GPIO7 TRST for JTAG debug**

**ATH:014048h - GPIO_PIN8 ;GPIO8 external 32kHz clock in**

**ATH:01404Ch - GPIO_PIN9 ;GPIO9 I2C SCL or SPI CLK**

**ATH:014050h - GPIO_PIN10 ;GPIO10 I2C SDA or SPI MISO**

**ATH:014054h - GPIO_PIN11 ;GPIO11 UART RXD or SPI MOSI**

**ATH:014058h - GPIO_PIN12 ;GPIO12 UART TXD or SPI /CS**

**ATH:01405Ch - GPIO_PIN13 ;GPIO13 Reset in for JTAG debug**

**ATH:014060h - GPIO_PIN14 ;GPIO14 UART CTS**

**ATH:014064h - GPIO_PIN15 ;GPIO15 UART RTS**

**ATH:014068h - GPIO_PIN16 ;GPIO16 -**

**ATH:01406Ch - GPIO_PIN17 ;GPIO17 -**

```

| 0 SOURCE
 1 -
 2 PAD_DRIVER
 3-6 -
 7-9 INT_TYPE
 10 WAKEUP_ENABLE
 11-12 CONFIG |
| --- |

Note: The I2C EEPROM in DSi/3DS is accessed via Pin9/10 on AR6002, and Pin2/3
on AR6013/AR6014.

```

| ______________________________ hw4 GPIO ports ______________________________ |
| --- |

**ATH:014028h - WLAN_GPIO_PIN0 ;GPIO0 Bluetooth coex BT_FREQUENCY**

**ATH:01402Ch - WLAN_GPIO_PIN1 ;GPIO1 Bluetooth coex WLAN_ACTIVE**

**ATH:014030h - WLAN_GPIO_PIN2 ;GPIO2 Bluetooth coex BT_ACTIVE**

**ATH:014034h - WLAN_GPIO_PIN3 ;GPIO3 Bluetooth coex BT_PRIORITY**

**ATH:014038h - WLAN_GPIO_PIN4 ;GPIO4 -**

**ATH:01403Ch - WLAN_GPIO_PIN5 ;GPIO5 JTAG TMS input**

**ATH:014040h - WLAN_GPIO_PIN6 ;GPIO6 JTAG TCK input**

**ATH:014044h - WLAN_GPIO_PIN7 ;GPIO7 JTAG TDI input**

**ATH:014048h - WLAN_GPIO_PIN8 ;GPIO8 JTAG TDO output**

**ATH:01404Ch - WLAN_GPIO_PIN9 ;GPIO9 SDIO CMD**

**ATH:014050h - WLAN_GPIO_PIN10 ;GPIO10 SDIO D3**

**ATH:014054h - WLAN_GPIO_PIN11 ;GPIO11 SDIO D2**

**ATH:014058h - WLAN_GPIO_PIN12 ;GPIO12 SDIO D1**

**ATH:01405Ch - WLAN_GPIO_PIN13 ;GPIO13 SDIO D0**

**ATH:014060h - WLAN_GPIO_PIN14 ;GPIO14 SDIO CLK**

**ATH:014064h - WLAN_GPIO_PIN15 ;GPIO15 HCI UART TXD**

**ATH:014068h - WLAN_GPIO_PIN16 ;GPIO16 HCI UART RTS**

**ATH:01406Ch - WLAN_GPIO_PIN17 ;GPIO17 HCI UART RXD**

**ATH:014070h - WLAN_GPIO_PIN18 ;GPIO18 HCI UART CTS**

**ATH:014074h - WLAN_GPIO_PIN19 ;GPIO19 SDIO/GSPI interface select**

**ATH:014078h - WLAN_GPIO_PIN20 ;GPIO20 SDIO/GSPI interface select**

**ATH:01407Ch - WLAN_GPIO_PIN21 ;GPIO21 external input sleep clock**

**ATH:014080h - WLAN_GPIO_PIN22 ;GPIO22 wake on wireless input (WOW)**

**ATH:014084h - WLAN_GPIO_PIN23 ;GPIO23 reference clk output to BT chip**

**ATH:014088h - WLAN_GPIO_PIN24 ;GPIO24 request clk from BT chip**

**ATH:01408Ch - WLAN_GPIO_PIN25 ;GPIO25 request reference clk (CLK_REQ)**

```

| 0 SOURCE
 1 -
 2 PAD_DRIVER
 3-4 PAD_STRENGTH ;\pull/strength supported for PIN0..PIN22 only
 5-6 PAD_PULL ;/(bit3-6 are unused in PIN23..PIN25 registers)
 7-9 INT_TYPE
 10 WAKEUP_ENABLE
 11-13 CONFIG |
| --- |

AR6003 datasheet assigns only the above stuff with a single UART, although the
AR6003 should additionally support a DBG_UART (and I2C).

```

| ______________________________ hw6 GPIO ports ______________________________ |
| --- |

**ATH:014050h - WLAN_GPIO_PIN0 ;GPIO0 or SDIO_CMD**

**ATH:014054h - WLAN_GPIO_PIN1 ;GPIO1 or SDIO_D3**

**ATH:014058h - WLAN_GPIO_PIN2 ;GPIO2 or SDIO_D2**

**ATH:01405Ch - WLAN_GPIO_PIN3 ;GPIO3 or SDIO_D1**

**ATH:014060h - WLAN_GPIO_PIN4 ;GPIO4 or SDIO_D0**

**ATH:014064h - WLAN_GPIO_PIN5 ;GPIO5 or SDIO_CLK**

**ATH:014068h - WLAN_GPIO_PIN6**

**ATH:01406Ch - WLAN_GPIO_PIN7**

**ATH:014070h - WLAN_GPIO_PIN8**

**ATH:014074h - WLAN_GPIO_PIN9**

**ATH:014078h - WLAN_GPIO_PIN10**

**ATH:01407Ch - WLAN_GPIO_PIN11**

**ATH:014080h - WLAN_GPIO_PIN12**

**ATH:014084h - WLAN_GPIO_PIN13**

**ATH:014088h - WLAN_GPIO_PIN14**

**ATH:01408Ch - WLAN_GPIO_PIN15**

**ATH:014090h - WLAN_GPIO_PIN16**

**ATH:014094h - WLAN_GPIO_PIN17**

**ATH:014098h - WLAN_GPIO_PIN18**

**ATH:01409Ch - WLAN_GPIO_PIN19**

**ATH:0140A0h - WLAN_GPIO_PIN20**

**ATH:0140A4h - WLAN_GPIO_PIN21**

**ATH:0140A8h - WLAN_GPIO_PIN22**

**ATH:0140ACh - WLAN_GPIO_PIN23**

**ATH:0140B0h - WLAN_GPIO_PIN24**

**ATH:0140B4h - WLAN_GPIO_PIN25**

**ATH:0140B8h - WLAN_GPIO_PIN26**

**ATH:0140BCh - WLAN_GPIO_PIN27**

**ATH:0140C0h - WLAN_GPIO_PIN28**

**ATH:0140C4h - WLAN_GPIO_PIN29**

**ATH:0140C8h - WLAN_GPIO_PIN30**

**ATH:0140CCh - WLAN_GPIO_PIN31**

**ATH:0140D0h - WLAN_GPIO_PIN32**

**ATH:0140D4h - WLAN_GPIO_PIN33**

**ATH:0140D8h - WLAN_GPIO_PIN34**

**ATH:0140DCh - WLAN_GPIO_PIN35**

**ATH:0140E0h - WLAN_GPIO_PIN36**

**ATH:0140E4h - WLAN_GPIO_PIN37**

**ATH:0140E8h - WLAN_GPIO_PIN38**

**ATH:0140ECh - WLAN_GPIO_PIN39**

**ATH:0140F0h - WLAN_GPIO_PIN40**

**ATH:0140F4h - WLAN_GPIO_PIN41**

**ATH:0140F8h - WLAN_GPIO_PIN42**

**ATH:0140FCh - WLAN_GPIO_PIN43**

**ATH:014100h - WLAN_GPIO_PIN44**

**ATH:014104h - WLAN_GPIO_PIN45**

**ATH:014108h - WLAN_GPIO_PIN46**

**ATH:01410Ch - WLAN_GPIO_PIN47**

**ATH:014110h - WLAN_GPIO_PIN48**

**ATH:014114h - WLAN_GPIO_PIN49**

**ATH:014118h - WLAN_GPIO_PIN50**

**ATH:01411Ch - WLAN_GPIO_PIN51**

**ATH:014120h - WLAN_GPIO_PIN52**

**ATH:014124h - WLAN_GPIO_PIN53**

**ATH:014128h - WLAN_GPIO_PIN54**

**ATH:01412Ch - WLAN_GPIO_PIN55**

**ATH:014130h - WLAN_GPIO_PIN56**

```

| 0 SOURCE
 1 -
 2 PAD_DRIVER
 3-4 PAD_STRENGTH
 5-6 PAD_PULL
 7-9 INT_TYPE
 10 WAKEUP_ENABLE
 11-14 CONFIG |
| --- |

AR6004 datasheet assigns only six SDIO signals to GPIO pins. However, signals
similar as on AR6002/AR6003 should exist (I2C/SPI, UART, etc.), plus TWO
additional UARTs).

```

| _____________________________ hw2/hw4/hw6 stuff _____________________________ |
| --- |

**ATH:014078h/01409Ch/01413Ch - WLAN_SIGMA_DELTA ;hw2/hw4/hw6**

```

| 0-7 TARGET
 8-15 PRESCALAR ;uh, scalar?
 16 ENABLE |
| --- |

**ATH:01407Ch/0140A8h/01414Ch - WLAN_DEBUG_CONTROL ;hw2/hw4/hw6**

```

| 0 ENABLE ;-hw2/hw4/hw6
 1 hw2: OBS_OE_L ;-hw2 only (bit1 removed in hw4/hw6) |
| --- |

**ATH:014080h/0140ACh/014150h - WLAN_DEBUG_INPUT_SEL ;hw2/hw4/hw6**

```

| 0-3 SRC ;-hw2/hw4/hw6
 4-5 hw4/hw6: SHIFT ;-hw4/hw6 |
| --- |

**ATH:014084h/0140B0h/014154h - WLAN_DEBUG_OUT ;hw2/hw4/hw6**

```

| 0-17 DATA (whatever) (always 18bit, no matter if GPIO with 18,25,57 pins) |
| --- |

**ATH:0140F0h/0140B4h/014158h - WLAN_RESET_TUPLE_STATUS ;hw2/hw4/hw6**

```

| 0-7 PIN_RESET_TUPLE
 8-11 TEST_RESET_TUPLE |
| --- |

```

| _______________________________ hw4/hw6 stuff _______________________________ |
| --- |

**ATH:014090h/014134h - SDIO ;hw4/hw6**

**ATH:014160h - SDIO2 ;hw6**

**ATH:014164h - SDHC ;hw6**

```

| 0 PINS_EN |
| --- |

**ATH:014098h/014138h - WL_SOC_APB ;hw4/hw6**

```

| 0 TOGGLE |
| --- |

**ATH:0140A0h/014140h - WL_BOOTSTRAP ;hw4/hw6**

```

| 0-22 hw4: STATUS (23bit) ;maybe for pin 0..22 (but not pin 23-25 ?)
 0-11 hw6: STATUS (12bit) ;maybe for pin 0..57 (with below "CORE_BOOTSTRAP")
 12 hw6: CPU_MBIST_EN |
| --- |

**ATH:014144h - CORE_BOOTSTRAP_LOW ;hw6**

```

| 0-31 hw6: STATUS (32bit) (extra bits, expanding STATUS in "WL_BOOTSTRAP"?) |
| --- |

**ATH:014148h - CORE_BOOTSTRAP_HIGH ;hw6**

```

| 0-12 hw6: STATUS (13bit) (extra bits, expanding STATUS in "WL_BOOTSTRAP"?) |
| --- |

**ATH:0140B8h/01415Ch - ANTENNA_SLEEP_CONTROL/ANTENNA_CONTROL ;hw4/hw6**

```

| 0-4 hw4: ENABLE (5bit) ;\
 5-9 hw4: VALUE (5bit) ; hw4 "ANTENNA_SLEEP_CONTROL"
 10-14 hw4: OVERRIDE (5bit) ;/
 0-3 hw6: ENABLE (4bit) ;\
 4-7 hw6: VALUE (4bit) ;
 8-11 hw6: OVERRIDE (4bit) ;
 12-13 hw6: LED_SEL (2bit) ; hw6 "ANTENNA_CONTROL"
 14 hw6: SPI_MODE ;
 15 hw6: SPI_CS ;
 16 hw6: RX_CLEAR ;/ |
| --- |

```

| _________________________________ hw6 stuff _________________________________ |
| --- |

**ATH:014168h - AMBA_DEBUG_BUS ;hw6**

```

| 0-4 SEL |
| --- |

**ATH:01416Ch - CPU_MBIST ;hw6**

```

| 0 DONE
 1 GLOBAL_FAIL
 2-10 BLOCK_FAIL |
| --- |

```

| _________________________________ hw4 stuff _________________________________ |
| --- |

**ATH:014094h - FUNC_BUS ;hw4**

```

| 0-21 OE_L
 22 GPIO_MODE |
| --- |

**ATH:0140A4h - CLOCK_GPIO ;hw4**

```

| 0 hw4: BT_CLK_OUT_EN
 1 hw4: BT_CLK_REQ_EN
 2 hw4: CLK_REQ_OUT_EN |
| --- |

```

| ____________________________ hw2 GPIO PIN config ____________________________ |
| --- |

**ATH:0140D4h - ANTD_PIN - Config: Pad Pull ;hw2**

**ATH:0140DCh - GPIO_H_PIN - Config: Pad Pull ;hw2**

**ATH:0140E4h - BT_WLAN_PIN - Config: Pad Pull ;hw2**

**ATH:0140ECh - CLK32K_PIN - Config: Pad Pull ;hw2**

```

| 0-1 hw2: PAD_PULL |
| --- |

**ATH:014070h - SDIO_PIN - Config: Pad Pull/Strength ;hw2**

**ATH:0140D0h - ANT_PIN - Config: Pad Pull/Strength ;hw2**

**ATH:0140D8h - GPIO_PIN - Config: Pad Pull/Strength ;hw2**

**ATH:0140E0h - BT_PIN - Config: Pad Pull/Strength ;hw2**

**ATH:0140E8h - SI_UART_PIN - Config: Pad Pull/Strength ;hw2**

```

| 0-1 hw2: PAD_STRENGTH
 2-3 hw2: PAD_PULL |
| --- |

**ATH:014074h - CLK_REQ_PIN - Config: Pad Pull/Strength/AteOeLow ;hw2**

```

| 0-1 hw2: PAD_STRENGTH
 2-3 hw2: PAD_PULL
 4 hw2: ATE_OE_L |
| --- |

```

| _______________________________ hw2 LA stuff _______________________________ |
| --- |

**ATH:014088h - LA_CONTROL ;hw2**

```

| 0 hw2: TRIGGERED
 1 hw2: RUN |
| --- |

**ATH:01408Ch - LA_CLOCK ;hw2**

```

| 0-7 hw2: DIV |
| --- |

**ATH:014090h - LA_STATUS ;hw2**

```

| 0 hw2: INTERRUPT |
| --- |

**ATH:014094h - LA_TRIGGER_SAMPLE ;hw2**

```

| 0-15 hw2: COUNT |
| --- |

**ATH:014098h - LA_TRIGGER_POSITION ;hw2**

```

| 0-15 hw2: VALUE |
| --- |

**ATH:01409Ch - LA_PRE_TRIGGER ;hw2**

**ATH:0140A0h - LA_POST_TRIGGER ;hw2**

```

| 0-15 hw2: COUNT |
| --- |

**ATH:0140A4h - LA_FILTER_CONTROL ;hw2**

```

| 0 hw2: DELTA |
| --- |

**ATH:0140A8h - LA_FILTER_DATA ;hw2**

**ATH:0140B0h - LA_TRIGGERA_DATA ;hw2**

**ATH:0140B8h - LA_TRIGGERB_DATA ;hw2**

**ATH:0140ACh - LA_FILTER_WILDCARD ;hw2**

**ATH:0140B4h - LA_TRIGGERA_WILDCARD ;hw2**

**ATH:0140BCh - LA_TRIGGERB_WILDCARD ;hw2**

```

| 0-17 hw2: MATCH ... maybe related to GPIO_PIN0..17 ? |
| --- |

**ATH:0140C0h - LA_TRIGGER ;hw2**

```

| 0-2 hw2: EVENT |
| --- |

**ATH:0140C4h - LA_FIFO ;hw2**

```

| 0 hw2: EMPTY
 1 hw2: FULL |
| --- |

**ATH:0140C8h..0140CCh - LA[0..1] ;hw2**

```

| 0-17 hw2: DATA |
| --- |

| DSi Atheros Wifi - Internal I/O - 018000h - MBOX Registers (hw2/hw4/hw6) |
| --- |

These registers are same in hw2/hw4/hw6, except that:

```

| GMBOX registers exist in hw4/hw6 only
 STE_MODE register exists in hw6 only
 WLAN_MBOX_INT_xxx bit18,19 exist in hw6 only |
| --- |

And, register names didn't have had the "WLAN_" prefix in hw2.

```

| ___________________________ Manual MBOX Transfer ___________________________ |
| --- |

**ATH:018000h..01800Ch - WLAN_MBOX_FIFO[0..3]**

```

| 0-7 DATA: DATABYTE
 8-11 DATA: zero?
 12-15 DATA: zero? maybe copy of MBOX_FIFO_STATUS bit12-15 ? (FULL)
 16-19 DATA: looks like copy of MBOX_FIFO_STATUS bit16-19 ? (EMPTY)
 20-31 - |
| --- |

READ: Allows to read incoming MBOX data; before reading this register, the data
MUST be manually copied to this register via WLAN_MBOX_TXFIFO_POP[n], then read
this register, and check the EMPTY flag; this requires "double indexing" as so:
for MBOX(n), test "WLAN_MBOX_FIFO[n].bit(16+n)", if the bit is zero, then
bit0-7 contains valid data.

WRITE: Allows to send outgoing MBOX data (write the databyte, with zeroes in
bit8-31); before doing so, one SHOULD check if the FIFO is full via
WLAN_MBOX_FIFO_STATUS.

**ATH:0180F0h..0180FCh - WLAN_MBOX_TXFIFO_POP[0..3]**

```

| 0 DATA ... uh 4x1bit ? for MBOX0..3 ?
 1-31 - |
| --- |

Writing 0 to WLAN_MBOX_TXFIFO_POP[n] does remove the oldest "TXFIFO" entry (the
data transmitted from SDIO side to xtensa side via MBOXn), and stores that
value (and a copy of the WLAN_MBOX_FIFO_STATUS bits) in WLAN_MBOX_FIFO[n].

**ATH:018100h..01810Ch - WLAN_MBOX_RXFIFO_POP[0..3]**

```

| 0 DATA ... uh 4x1bit ? for MBOX0..3 ?
 1-31 - |
| --- |

Probably similar as above, but for opposite direction (ie. allowing to read
data that was "sent-to-the-host"; normally such data should be read by the
host, so one would use this feature only if one wants to screw up the normal
transfer flow).

```

| ____________________________ DMA MBOX Transfer ____________________________ |
| --- |

**ATH:018014h - WLAN_MBOX_DMA_POLICY**

```

| 0 RX_ORDER
 1 RX_QUANTUM
 2 TX_ORDER
 3 TX_QUANTUM
 4-31 - |
| --- |

**ATH:018018h - WLAN_MBOX0_DMA_RX_DESCRIPTOR_BASE**

**ATH:018020h - WLAN_MBOX0_DMA_TX_DESCRIPTOR_BASE**

**ATH:018028h - WLAN_MBOX1_DMA_RX_DESCRIPTOR_BASE**

**ATH:018030h - WLAN_MBOX1_DMA_TX_DESCRIPTOR_BASE**

**ATH:018038h - WLAN_MBOX2_DMA_RX_DESCRIPTOR_BASE**

**ATH:018040h - WLAN_MBOX2_DMA_TX_DESCRIPTOR_BASE**

**ATH:018048h - WLAN_MBOX3_DMA_RX_DESCRIPTOR_BASE**

**ATH:018050h - WLAN_MBOX3_DMA_TX_DESCRIPTOR_BASE**

**ATH:018114h - WLAN_GMBOX0_DMA_RX_DESCRIPTOR_BASE - hw4/hw6 only**

**ATH:01811Ch - WLAN_GMBOX0_DMA_TX_DESCRIPTOR_BASE - hw4/hw6 only**

```

| 0-1 -
 2-27 ADDRESS
 28-31 - |
| --- |

**ATH:01801Ch - WLAN_MBOX0_DMA_RX_CONTROL**

**ATH:018024h - WLAN_MBOX0_DMA_TX_CONTROL**

**ATH:01802Ch - WLAN_MBOX1_DMA_RX_CONTROL**

**ATH:018034h - WLAN_MBOX1_DMA_TX_CONTROL**

**ATH:01803Ch - WLAN_MBOX2_DMA_RX_CONTROL**

**ATH:018044h - WLAN_MBOX2_DMA_TX_CONTROL**

**ATH:01804Ch - WLAN_MBOX3_DMA_RX_CONTROL**

**ATH:018054h - WLAN_MBOX3_DMA_TX_CONTROL**

**ATH:018118h - WLAN_GMBOX0_DMA_RX_CONTROL - hw4/hw6 only**

**ATH:018120h - WLAN_GMBOX0_DMA_TX_CONTROL - hw4/hw6 only**

```

| 0 STOP
 1 START
 2 RESUME
 3-31 - |
| --- |

```

| __________________________________ Status __________________________________ |
| --- |

**ATH:018010h - WLAN_MBOX_FIFO_STATUS**

```

| 0-11 -
 12-15 FULL flags for MBOX 0..3
 16-19 EMPTY flags for MBOX 0..3
 20-31 - |
| --- |

**ATH:018058h - WLAN_MBOX_INT_STATUS**

**ATH:01805Ch - WLAN_MBOX_INT_ENABLE**

```

| 0-7 HOST Interrupt 0..7 from Host ;SDIO 1:00472h.bit0..7
 8-11 RX_NOT_FULL MBOX0..3 RX FIFO Not Full
 12-15 TX_NOT_EMPTY MBOX0..3 TX FIFO Not Empty
 16 RX_UNDERFLOW MBOX RX Underflow (tried to read from empty fifo)
 17 TX_OVERFLOW MBOX TX Overflow (tried to write to full fifo)
 18 hw6: FRAME_DONE ;\hw6.0 only
 19 hw6: NO_RX_MBOX_DATA_AVA ;/
 20-23 TX_DMA_COMPLETE MBOX0..3 TX DMA Complete
 24-27 TX_DMA_EOM_COMPLETE MBOX0..3 TX DMA Complete .. End of message?
 28-31 RX_DMA_COMPLETE MBOX0..3 RX DMA Complete |
| --- |

**ATH:018124h - WLAN_GMBOX_INT_STATUS - hw4/hw6 only**

**ATH:018128h - WLAN_GMBOX_INT_ENABLE - hw4/hw6 only**

```

| 0 RX_NOT_FULL
 1 TX_NOT_EMPTY
 2 TX_DMA_COMPLETE
 3 TX_DMA_EOM_COMPLETE
 4 RX_DMA_COMPLETE
 5 RX_UNDERFLOW
 6 TX_OVERFLOW
 7-31 - |
| --- |

```

| ______________________________ SDIO Handshake ______________________________ |
| --- |

**ATH:018060h - WLAN_INT_HOST**

```

| 0-7 VECTOR Interrupt 0..7 to Host ;SDIO 1:00401h.bit0..7
 8-31 - |
| --- |

**ATH:018080h..01809Ch - WLAN_LOCAL_COUNT[0..7]**

**ATH:0180A0h..0180BCh - WLAN_COUNT_INC[0..7]**

```

| 0-7 VALUE (credit counter) ;SDIO 1:00420h..00427h
 8-31 - |
| --- |

See COUNT and COUNT_DEC.

**ATH:0180C0h..0180DCh - WLAN_LOCAL_SCRATCH[0..7]**

```

| 0-7 VALUE (scratch) ;SDIO 1:00460h..00467h
 8-31 - |
| --- |

**ATH:0180E0h - WLAN_USE_LOCAL_BUS**

```

| 0 PIN_INIT ;whatever, maybe PCI bus related (non-SDIO) ?
 1-31 - |
| --- |

**ATH:0180E4h - WLAN_SDIO_CONFIG**

```

| 0 CCCR_IOR1 ;SDIO Func I/O Ready bit1 ? ;SDIO 0:00002h.bit1
 1-31 - |
| --- |

**ATH:01A000h..01BFFCh - WLAN_HOST_IF_WINDOW[0..2047]**

```

| 0-7 DATA ;SDIO 1:00000h..007FFh
 8-31 - |
| --- |

Allows to access the SDIO Host registers via Internal registers, should be done
only for testing purposes.

```

| ___________________________________ Misc ___________________________________ |
| --- |

**ATH:0180E8h - WLAN_MBOX_DEBUG**

```

| 0-2 SEL
 3-31 - |
| --- |

**ATH:0180ECh - WLAN_MBOX_FIFO_RESET**

```

| 0 INIT
 1-31 - |
| --- |

**ATH:018110h - WLAN_SDIO_DEBUG**

```

| 0-3 SEL
 4-31 - |
| --- |

**ATH:01812Ch - STE_MODE - hw6.0 only**

```

| 0 SEL
 1-2 PHA_POL
 3 SEL_16BIT
 4 SWAP
 5 RST
 6 SPI_CTRL_EN |
| --- |

| DSi Atheros Wifi - Internal I/O - 01C000h - Analog Intf (hw2) |
| --- |

**ATH:01C000h - SYNTH_SYNTH1 ;aka - PHY_ANALOG_SYNTH1**

```

| 0 MONITOR_SYNTHLOCKVCOK
 1 MONITOR_VC2LOW
 2 MONITOR_VC2HIGH
 3 MONITOR_FB_DIV2
 4 MONITOR_REF
 5 MONITOR_FB
 6 PWUP_LOBUF5G_PD
 7 PWUP_LOMIX_PD
 8 PWUP_LODIV_PD
 9 PWUP_VCOBUF_PD
 10-12 SEL_VCMONABUS
 13 CON_IVCOBUF
 14 CON_IVCOREG
 15 CON_VDDVCOREG
 16 SPARE_PWD
 17 SLIDINGIF
 18-19 VCOREGBIAS
 20-21 VCOREGLEVEL
 22 VCOREGBYPASS
 23 PWD_LOBUF5G
 24 FORCE_LO_ON
 25 PWD_LOMIX
 26 PWD_LODIV
 27 PWD_PRESC
 28 PWD_VCO
 29 PWD_VCMON
 30 PWD_CP
 31 PWD_BIAS |
| --- |

**ATH:01C004h - SYNTH_SYNTH2 ;aka - PHY_ANALOG_SYNTH2 (one part)**

```

| 0-2 SPARE_BITS
 3-4 LOOP_CS
 5-9 LOOP_RS
 10-14 LOOP_CP
 15-19 LOOP_3RD_ORDER_R
 20-22 VC_LOW_REF
 23-25 VC_MID_REF
 26-28 VC_HI_REF
 29-31 VC_CAL_REF |
| --- |

**ATH:01C008h - SYNTH_SYNTH3 ;aka - PHY_ANALOG_SYNTH3**

```

| 0-5 WAIT_VC_CHECK
 6-11 WAIT_CAL_LIN
 12-17 WAIT_CAL_BIN
 18-23 WAIT_PWRUP
 24-29 WAIT_SHORTR_PWRUP
 30 SEL_CLK_DIV2
 31 DIS_CLK_XTAL |
| --- |

**ATH:01C00Ch - SYNTH_SYNTH4 ;aka - PHY_ANALOG_SYNTH4**

```

| 0 FORCE_SHIFTREG
 1 LONGSHIFTSEL
 2-3 SPARE_MISC
 4 SEL_CLKXTAL_EDGE
 5 PSCOUNT_FBSEL
 6-7 SDM_DITHER
 8 SDM_MODE
 9 SDM_DISABLE
 10 RESET_PRESC
 11-12 PRESCSEL
 13 PFD_DISABLE
 14 PFDDELAY
 15-16 REFDIVSEL
 17 VCOCAPPULLUP
 18-25 VCOCAP_OVR
 26 FORCE_VCOCAP
 27 FORCE_PINVC
 28 SHORTR_UNTIL_LOCKED
 29 ALWAYS_SHORTR
 30 DIS_LOSTVC
 31 DIS_LIN_CAPSEARCH |
| --- |

**ATH:01C010h - SYNTH_SYNTH5 ;aka - PHY_ANALOG_SYNTH2 (other part)**

```

| 0-1 SPARE
 2-3 LOBUF5GTUNE_OVR
 4 FORCE_LOBUF5GTUNE
 5-8 CAPRANGE3
 9-12 CAPRANGE2
 13-16 CAPRANGE1
 17-20 LOOPLEAKCUR
 21 CPLOWLK
 22 CPSTEERING_EN
 23-24 CPBIAS
 25-27 SLOPE_IP
 28-31 LOOP_IP0 |
| --- |

**ATH:01C014h - SYNTH_SYNTH6**

```

| 0-2 SPARE_BIAS
 3-4 VCOBUFBIAS
 5-7 ICVCO
 8-10 ICSPAREB
 11-13 ICSPAREA
 14-16 ICLOMIX
 17-19 ICLODIV
 20-22 ICPRESC
 23-25 IRSPARE
 26-28 IRVCMON
 29-31 IRCP |
| --- |

**ATH:01C018h - SYNTH_SYNTH7 ;aka "PHY_ANALOG_SYNTH6" (six) on later hw**

```

| 0-2 SPARE_READ
 3-4 LOBUF5GTUNE
 5-8 LOOP_IP
 9 VC2LOW
 10 VC2HIGH
 11 RESET_SDM_B
 12 RESET_PSCOUNTERS
 13 RESET_PFD
 14 RESET_RFD
 15 SHORT_R
 16-23 VCO_CAP_ST
 24 PIN_VC
 25 SYNTH_LOCK_VC_OK
 26 CAP_SEARCH
 27-30 SYNTH_SM_STATE
 31 SYNTH_ON |
| --- |

**ATH:01C01Ch - SYNTH_SYNTH8 ;aka "PHY_ANALOG_SYNTH7" (seven) on later hw**

```

| 0 FORCE_FRACLSB
 1-17 CHANFRAC
 18-26 CHANSEL
 27 SPARE
 28-29 AMODEREFSEL
 30 FRACMODE
 31 LOADSYNTHCHANNEL |
| --- |

**ATH:01C020h - RF5G_RF5G1**

```

| 0-1 SPARE
 2 REGLO_BYPASS5
 3 LO5CONTROL
 4-6 LO5_ATB_SEL
 7 PDREGLO5
 8 PDLO5AGC
 9 PDQBUF5
 10 PDLO5MIX
 11 PDLO5DIV
 12-14 TX5_ATB_SEL
 15-17 OB5
 18-20 DB5
 21-23 PWDTXPKD
 24-26 TUNE_PADRV5
 27 PDPAOUT5
 28 PDPADRV5
 29 PDTXBUF5
 30 PDTXMIX5
 31 PDTXLO5 |
| --- |

**ATH:01C024h - RF5G_RF5G2**

```

| 0-1 SPARE
 2-4 TUNE_LO
 5 ENABLE_PCA
 6-7 LNA5_ATTENMODE
 8 REGFE_BYPASS5
 9-11 BVGM5
 12-14 BCSLNA5
 15-17 BRFVGA5
 18-20 TUNE_RFVGA5
 21 PDREGFE5
 22 PDRFVGA5
 23 PDCSLNA5
 24 PDVGM5
 25 PDCMOSLO5
 26-28 RX5_ATB_SEL
 29-31 AGCLO_B |
| --- |

**ATH:01C028h - RF2G_RF2G1**

```

| 0-4 SPARE
 5 SHORTLNA2
 6 LOCONTROL
 7 SELLNA
 8-10 RF_ATB_SEL
 11-13 FE_ATB_SEL
 14-16 OB
 17-19 DB
 20-22 BLNA2
 23-25 BLNA1BUF
 26-28 BLNA1F
 29-31 BLNA1 |
| --- |

**ATH:01C02Ch - RF2G_RF2G2**

```

| 0-16 SPARE
 17 ENABLE_PCB
 18 REGLO_BYPASS
 19 REGLNA_BYPASS
 20 PDTXMIX
 21 PDTXLO
 22 PDRXLO
 23 PDRFGM
 24 PDREGLO
 25 PDREGLNA
 26 PDPAOUT
 27 PDPADRV
 28 PDDIV
 29 PDCSLNA
 30 PDCGLNABUF
 31 PDCGLNA |
| --- |

**ATH:01C030h - TOP_GAIN**

```

| 0 SPARE
 1-2 RX6DBHIQGAIN
 3-5 RX1DBLOQGAIN
 6-7 RX6DBLOQGAIN
 8-10 RFGMGN
 11-12 RFVGA5GAIN
 13-16 LNAGAIN
 17 LNAON
 18-20 PAOUT2GN
 21-23 PADRVGN
 24 PABUF5GN
 25-26 TXV2IGAIN
 27-29 TX1DBLOQGAIN
 30-31 TX6DBLOQGAIN |
| --- |

**ATH:01C034h - TOP_TOP**

```

| 0 FORCE_XPAON
 1 INT2GND
 2 PAD2GND
 3 INTH2PAD
 4 INT2PAD
 5-7 REVID
 8-9 DATAOUTSEL
 10 PDBIAS
 11 SYNTHON_FORCE
 12 SCLKEN_FORCE
 13 OSCON
 14 PWDCLKIN
 15 LOCALXTAL
 16 PWDDAC
 17 PWDADC
 18 PWDPLL
 19 LOCALADDAC
 20 CALTX
 21 PAON
 22 TXON
 23 RXON
 24 SYNTHON
 25 BMODE
 26 CAL_RESIDUE
 27 CALDC
 28 CALFC
 29 LOCALMODE
 30 LOCALRXGAIN
 31 LOCALTXGAIN |
| --- |

**ATH:01C038h - BIAS_BIAS_SEL**

```

| 0 PWD_ICLDO25
 1-3 PWD_ICTXPC25
 4-6 PWD_ICTSENS25
 7-9 PWD_ICXTAL25
 10-12 PWD_ICCOMPBIAS25
 13 PWD_ICCPLL25
 14 PWD_ICREFOPAMPBIAS25
 15 PWD_IRREFMASTERBIAS12P5
 16 PWD_IRDACREGREF12P5
 17-19 PWD_ICREFBUFBIAS12P5
 20 SPARE
 21-24 SEL_SPARE
 25-30 SEL_BIAS
 31 PADON |
| --- |

**ATH:01C03Ch - BIAS_BIAS1**

```

| 0-1 SPARE
 2-4 PWD_IC5GMIXQ25
 5-7 PWD_IC5GQB25
 8-10 PWD_IC5GTXBUF25
 11-13 PWD_IC5GTXPA25
 14 PWD_IC5GRXRF25
 15 PWD_ICDETECTORA25
 16 PWD_ICDETECTORB25
 17-19 PWD_IC2GLNAREG25
 20-22 PWD_IC2GLOREG25
 23-25 PWD_IC2GRFFE25
 26-28 PWD_IC2GVGM25
 29-31 PWD_ICDAC2BB25 |
| --- |

**ATH:01C040h - BIAS_BIAS2**

```

| 0-2 PWD_IR5GRFVREF2525
 3-5 PWD_IR2GLNAREG25
 6-8 PWD_IR2GLOREG25
 9-11 PWD_IR2GTXMIX25
 12 PWD_IRLDO25
 13-15 PWD_IRTXPC25
 16-18 PWD_IRTSENS25
 19-21 PWD_IRXTAL25
 22 PWD_IRPLL25
 23-25 PWD_IC5GLOREG25
 26-28 PWD_IC5GDIV25
 29-31 PWD_IC5GMIXI25 |
| --- |

**ATH:01C044h - BIAS_BIAS3**

```

| 0 SPARE
 1-3 PWD_ICDACREG12P5
 4-6 PWD_IR25SPARE2
 7-9 PWD_IR25SPARE1
 10-12 PWD_IC25SPARE2
 13-15 PWD_IC25SPARE1
 16 PWD_IRBB50
 17 PWD_IRSYNTH50
 18-20 PWD_IC2GDIV50
 21 PWD_ICBB50
 22 PWD_ICSYNTH50
 23-25 PWD_ICDAC50
 26-28 PWD_IR5GAGC25
 29-31 PWD_IR5GTXMIX25 |
| --- |

**ATH:01C048h - TXPC_TXPC**

```

| 0-1 ATBSEL
 2 SELCOUNT
 3-4 SELINIT
 5 ON1STSYNTHON
 6-13 N
 14-15 TSMODE
 16 SELCMOUT
 17 SELMODREF
 18 CLKDELAY
 19 NEGOUT
 20 CURHALF
 21 TESTPWDPC
 22-27 TESTDAC
 28-29 TESTGAIN
 30 TEST
 31 SELINTPD |
| --- |

**ATH:01C04Ch - TXPC_MISC**

```

| 0-5 SPARE
 6-7 XTALDIV
 8-17 DECOUT
 18-20 SPARE_A
 21 SELTSN
 22 SELTSP
 23 LOCALBIAS2X
 24 LOCALBIAS
 25 PWDXINPAD
 26 PWDCLKIND
 27 NOTCXODET
 28 LDO_TEST_MODE
 29-30 LEVEL
 31 FLIPBMODE |
| --- |

**ATH:01C050h - RXTXBB_RXTXBB1**

```

| 0 PDHIQ
 1 PDLOQ
 2 PDOFFSETI2V
 3 PDOFFSETHIQ
 4 PDOFFSETLOQ
 5 PDRXTXBB
 6 PDI2V
 7 PDV2I
 8 PDDACINTERFACE
 6-16 SEL_ATB
 17-18 FNOTCH
 19-31 SPARE |
| --- |

**ATH:01C054h - RXTXBB_RXTXBB2**

```

| 0 PATH_OVERRIDE
 1 PATH1LOQ_EN
 2 PATH2LOQ_EN
 3 PATH3LOQ_EN
 4 PATH1HIQ_EN
 5 PATH2HIQ_EN
 6 FILTERDOUBLEBW
 7 LOCALFILTERTUNING
 8-12 FILTERFC
 13-14 CMSEL
 15 SEL_I2V_TEST
 16 SEL_HIQ_TEST
 17 SEL_LOQ_TEST
 18 SEL_DAC_TEST
 19 SELBUFFER
 20 SHORTBUFFER
 21-22 SPARE
 23-25 IBN_37P5_OSI2V_CTRL
 26-28 IBN_37P5_OSLO_CTRL
 29-31 IBN_37P5_OSHI_CTRL |
| --- |

**ATH:01C058h - RXTXBB_RXTXBB3**

```

| 0-2 IBN_100U_TEST_CTRL
 3-5 IBRN_12P5_CM_CTRL
 6-8 IBN_25U_LO2_CTRL
 9-11 IBN_25U_LO1_CTRL
 12-14 IBN_25U_HI2_CTRL
 15-17 IBN_25U_HI1_CTRL
 18-20 IBN_25U_I2V_CTRL
 21-23 IBN_25U_BKV2I_CTRL
 24-26 IBN_25U_CM_BUFAMP_CTRL
 27-31 SPARE |
| --- |

**ATH:01C05Ch - RXTXBB_RXTXBB4**

```

| 0-4 OFSTCORRI2VQ
 5-9 OFSTCORRI2VI
 10-14 OFSTCORRLOQ
 15-19 OFSTCORRLOI
 20-24 OFSTCORRHIQ
 25-29 OFSTCORRHII
 30 LOCALOFFSET
 31 SPARE |
| --- |

**ATH:01C060h - ADDAC_ADDAC1 ;aka "A/D and D/A Converter"**

```

| 0-5 SPARE
 6 DISABLE_DAC_REG
 7-8 CM_SEL
 9 INV_CLK160_ADC
 10 SELMANPWDS
 11 FORCEMSBLOW
 12 PWDDAC
 13 PWDADC
 14 PWDPLL
 15-22 PLL_FILTER
 23-25 PLL_ICP
 26-27 PLL_ATB
 28-30 PLL_SCLAMP
 31 PLL_SVREG |
| --- |

**ATH:01C080h - SW_OVERRIDE**

```

| 0 ENABLE
 1 SUPDATE_DELAY |
| --- |

**ATH:01C084h - SIN_VAL**

```

| 0 SIN |
| --- |

**ATH:01C088h - SW_SCLK**

```

| 0 SW_SCLK |
| --- |

**ATH:01C08Ch - SW_CNTL**

```

| 0 SW_SOUT
 1 SW_SUPDATE
 2 SW_SCAPTURE |
| --- |

| DSi Atheros Wifi - Internal I/O - 01C000h - Analog Intf (hw4/hw6) |
| --- |

These registers are same in hw4/hw6, except for some small differences:

```

| 0001C050h one new bit in hw6.0
 0001C148h several new bits in hw6.0
 0001C740h added/removed/renumbered bits in hw6.0
 0001C744h two changed/renamed bits in hw6.0 |
| --- |

**ATH:01C000h - PHY_ANALOG_RXRF_BIAS1**

```

| 0 SPARE
 1-3 PWD_IR25SPARE
 4-6 PWD_IR25LO18
 7-9 PWD_IC25LO36
 10-12 PWD_IC25MXR2_5GH
 13-15 PWD_IC25MXR5GH
 16-18 PWD_IC25VGA5G
 19-21 PWD_IC75LNA5G
 22-24 PWD_IR25LO24
 25-27 PWD_IC25MXR2GH
 28-30 PWD_IC75LNA2G
 31 PWD_BIAS |
| --- |

**ATH:01C004h - PHY_ANALOG_RXRF_BIAS2**

```

| 0 SPARE
 1-3 PKEN
 4-6 VCMVALUE
 7 PWD_VCMBUF
 8-10 PWD_IR25SPAREH
 11-13 PWD_IR25SPARE
 14-16 PWD_IC25LNABUF
 17-19 PWD_IR25AGCH
 20-22 PWD_IR25AGC
 23-25 PWD_IC25AGC
 26-28 PWD_IC25VCMBUF
 29-31 PWD_IR25VCM |
| --- |

**ATH:01C008h - PHY_ANALOG_RXRF_GAINSTAGES**

```

| 0 SPARE
 1 LNAON_CALDC
 2-3 VGA5G_CAP
 4-5 LNA5G_CAP
 6 LNA5G_SHORTINP
 7 PWD_LO5G
 8 PWD_VGA5G
 9 PWD_MXR5G
 10 PWD_LNA5G
 11-12 LNA2G_CAP
 13 LNA2G_SHORTINP
 14 LNA2G_LP
 15 PWD_LO2G
 16 PWD_MXR2G
 17 PWD_LNA2G
 18-19 MXR5G_GAIN_OVR
 20-22 VGA5G_GAIN_OVR
 23-25 LNA5G_GAIN_OVR
 26-27 MXR2G_GAIN_OVR
 28-30 LNA2G_GAIN_OVR
 31 RX_OVERRIDE |
| --- |

**ATH:01C00Ch - PHY_ANALOG_RXRF_AGC**

```

| 0 RF5G_ON_DURING_CALPA
 1 RF2G_ON_DURING_CALPA
 2 AGC_OUT (R)
 3 LNABUFGAIN2X
 4 LNABUF_PWD_OVR
 5 PWD_LNABUF
 6-8 AGC_FALL_CTRL
 9-14 AGC5G_CALDAC_OVR
 15-18 AGC5G_DBDAC_OVR
 19-24 AGC2G_CALDAC_OVR
 25-28 AGC2G_DBDAC_OVR
 29 AGC_CAL_OVR
 30 AGC_ON_OVR
 31 AGC_OVERRIDE |
| --- |

**ATH:01C040h - PHY_ANALOG_TXRF1**

```

| 0 PDLOBUF5G
 1 PDLODIV5G
 2 LOBUF5GFORCED
 3 LODIV5GFORCED
 4-7 PADRV2GN5G
 8-11 PADRV3GN5G
 12-15 PADRV4GN5G
 16 LOCALTXGAIN5G
 17 PDOUT2G
 18 PDDR2G
 19 PDMXR2G
 20 PDLOBUF2G
 21 PDLODIV2G
 22 LOBUF2GFORCED
 23 LODIV2GFORCED
 24-30 PADRVGN2G
 31 LOCALTXGAIN2G |
| --- |

**ATH:01C044h - PHY_ANALOG_TXRF2**

```

| 0-2 D3B5G
 3-5 D4B5G
 6-8 OCAS2G
 9-11 DCAS2G
 12-14 OB2G_PALOFF
 15-17 OB2G_QAM
 18-20 OB2G_PSK
 21-23 OB2G_CCK
 24-26 DB2G
 27-30 PDOUT5G
 31 PDMXR5G |
| --- |

**ATH:01C048h - PHY_ANALOG_TXRF3**

```

| 0-1 FILTR2G
 2 PWDFB2_2G
 3 PWDFB1_2G
 4 PDFB2G
 5-6 RDIV5G
 7-9 CAPDIV5G
 10 PDPREDIST5G
 11-12 RDIV2G
 13 PDPREDIST2G
 14-16 OCAS5G
 17-19 D2CAS5G
 20-22 D3CAS5G
 23-25 D4CAS5G
 26-28 OB5G
 29-31 D2B5G |
| --- |

**ATH:01C04Ch - PHY_ANALOG_TXRF4**

```

| 0-1 PK1B2G_CCK
 2-4 MIOB2G_QAM
 5-7 MIOB2G_PSK
 8-10 MIOB2G_CCK
 11-13 COMP2G_QAM
 14-16 COMP2G_PSK
 17-19 COMP2G_CCK
 20-22 AMP2B2G_QAM
 23-25 AMP2B2G_PSK
 26-28 AMP2B2G_CCK
 29-31 AMP2CAS2G |
| --- |

**ATH:01C050h - PHY_ANALOG_TXRF5**

```

| 0 hw4: SPARE5
 0 hw6: TXMODPALONLY ;-hw6.0 only
 1 PAL_LOCKED (R)
 2 FBHI2G (R)
 3 FBLO2G (R)
 4 NOPALGAIN2G
 5 ENPACAL2G
 6-12 OFFSET2G
 13 ENOFFSETCAL2G
 14-16 REFHI2G
 17-19 REFLO2G
 20-21 PALCLAMP2G
 22-23 PK2B2G_QAM
 24-25 PK2B2G_PSK
 26-27 PK2B2G_CCK
 28-29 PK1B2G_QAM
 30-31 PK1B2G_PSK |
| --- |

**ATH:01C054h - PHY_ANALOG_TXRF6**

```

| 0 PALCLKGATE2G
 1-8 PALFLUCTCOUNT2G
 9-10 PALFLUCTGAIN2G
 11 PALNOFLUCT2G
 12-14 GAINSTEP2G
 15 USE_GAIN_DELTA2G
 16-19 CAPDIV_I2G
 20-23 PADRVGN_INDEX_I2G
 24-26 VCMONDELAY2G
 27-30 CAPDIV2G
 31 CAPDIV2GOVR |
| --- |

**ATH:01C058h - PHY_ANALOG_TXRF7 ;PADRVGNTAB_0..4**

```

| 0-1 SPARE7
 2-7 PADRVGNTAB_4
 8-13 PADRVGNTAB_3
 14-19 PADRVGNTAB_2
 20-25 PADRVGNTAB_1
 26-31 PADRVGNTAB_0 |
| --- |

**ATH:01C05Ch - PHY_ANALOG_TXRF8 ;PADRVGNTAB_5..9**

```

| 0-1 SPARE8
 2-7 PADRVGNTAB_9
 8-13 PADRVGNTAB_8
 14-19 PADRVGNTAB_7
 20-25 PADRVGNTAB_6
 26-31 PADRVGNTAB_5 |
| --- |

**ATH:01C060h - PHY_ANALOG_TXRF9 ;PADRVGNTAB_10..14**

```

| 0-1 SPARE9
 2-7 PADRVGNTAB_14
 8-13 PADRVGNTAB_13
 14-19 PADRVGNTAB_12
 20-25 PADRVGNTAB_11
 26-31 PADRVGNTAB_10 |
| --- |

**ATH:01C064h - PHY_ANALOG_TXRF10**

```

| 0-2 SPARE10
 3 PDOUT5G_3CALTX
 4-6 D3B5GCALTX
 7-9 D4B5GCALTX
 10-16 PADRVGN2GCALTX
 17-19 DB2GCALTX
 20 CALTXSHIFT
 21 CALTXSHIFTOVR
 22-27 PADRVGN2G_SMOUT (R)
 28-31 PADRVGN_INDEX2G_SMOUT (R) |
| --- |

**ATH:01C068h - PHY_ANALOG_TXRF11**

```

| 0-1 SPARE11
 2-4 PWD_IR25MIXDIV5G
 5-7 PWD_IR25PA2G
 8-10 PWD_IR25MIXBIAS2G
 11-13 PWD_IR25MIXDIV2G
 14-16 PWD_ICSPARE
 17-19 PWD_IC25TEMPSEN
 20-22 PWD_IC25PA5G2
 23-25 PWD_IC25PA5G1
 26-28 PWD_IC25MIXBUF5G
 29-31 PWD_IC25PA2G |
| --- |

**ATH:01C06Ch - PHY_ANALOG_TXRF12**

```

| 0-7 SPARE12_2 (R)
 8-9 SPARE12_1
 10-13 ATBSEL5G
 14-16 ATBSEL2G
 17-19 PWD_IRSPARE
 20-22 PWD_IR25TEMPSEN
 23-25 PWD_IR25PA5G2
 26-28 PWD_IR25PA5G1
 29-31 PWD_IR25MIXBIAS5G |
| --- |

**ATH:01C080h - PHY_ANALOG_SYNTH1**

```

| 0-2 SEL_VCMONABUS
 3-5 SEL_VCOABUS
 6 MONITOR_SYNTHLOCKVCOK
 7 MONITOR_VC2LOW
 8 MONITOR_VC2HIGH
 9 MONITOR_FB_DIV2
 10 MONITOR_REF
 11 MONITOR_FB
 12 SEVENBITVCOCAP
 13-15 PWUP_PD
 16 PWD_VCOBUF
 17-18 VCOBUFGAIN
 19-20 VCOREGLEVEL
 21 VCOREGBYPASS
 22 PWUP_LOREF
 23 PWD_LOMIX
 24 PWD_LODIV
 25 PWD_LOBUF5G
 26 PWD_LOBUF2G
 27 PWD_PRESC
 28 PWD_VCO
 29 PWD_VCMON
 30 PWD_CP
 31 PWD_BIAS |
| --- |

**ATH:01C084h - PHY_ANALOG_SYNTH2**

```

| 0-3 CAPRANGE3
 4-7 CAPRANGE2
 8-11 CAPRANGE1
 12-15 LOOPLEAKCUR_INTN
 16 CPLOWLK_INTN
 17 CPSTEERING_EN_INTN
 18-19 CPBIAS_INTN
 20-22 VC_LOW_REF
 23-25 VC_MID_REF
 26-28 VC_HI_REF
 29-31 VC_CAL_REF |
| --- |

**ATH:01C088h - PHY_ANALOG_SYNTH3**

```

| 0-5 WAIT_VC_CHECK
 6-11 WAIT_CAL_LIN
 12-17 WAIT_CAL_BIN
 18-23 WAIT_PWRUP
 24-29 WAIT_SHORTR_PWRUP
 30 SEL_CLK_DIV2
 31 DIS_CLK_XTAL |
| --- |

**ATH:01C08Ch - PHY_ANALOG_SYNTH4**

```

| 0 PS_SINGLE_PULSE
 1 LONGSHIFTSEL
 2-3 LOBUF5GTUNE_OVR
 4 FORCE_LOBUF5GTUNE
 5 PSCOUNT_FBSEL
 6-7 SDM_DITHER1
 8 SDM_MODE
 9 SDM_DISABLE
 10 RESET_PRESC
 11-12 PRESCSEL
 13 PFD_DISABLE
 14 PFDDELAY_FRACN
 15 FORCE_LO_ON
 16 CLKXTAL_EDGE_SEL
 17 VCOCAPPULLUP
 18-25 VCOCAP_OVR
 26 FORCE_VCOCAP
 27 FORCE_PINVC
 28 SHORTR_UNTIL_LOCKED
 29 ALWAYS_SHORTR
 30 DIS_LOSTVC
 31 DIS_LIN_CAPSEARCH |
| --- |

**ATH:01C090h - PHY_ANALOG_SYNTH5**

```

| 0-1 VCOBIAS
 2-4 PWDB_ICLOBUF5G50
 5-7 PWDB_ICLOBUF2G50
 8-10 PWDB_ICVCO25
 11-13 PWDB_ICVCOREG25
 14 PWDB_IRVCOREG50
 15-17 PWDB_ICLOMIX
 18-20 PWDB_ICLODIV50
 21-23 PWDB_ICPRESC50
 24-26 PWDB_IRVCMON25
 27-29 PWDB_IRPFDCP
 30-31 SDM_DITHER2 |
| --- |

**ATH:01C094h - PHY_ANALOG_SYNTH6**

```

| 0-1 LOBUF5GTUNE (R)
 2-8 LOOP_IP (R)
 9 VC2LOW (R)
 10 VC2HIGH (R)
 11 RESET_SDM_B (R)
 12 RESET_PSCOUNTERS (R)
 13 RESET_PFD (R)
 14 RESET_RFD (R)
 15 SHORT_R (R)
 16-23 VCO_CAP_ST (R)
 24 PIN_VC (R)
 25 SYNTH_LOCK_VC_OK (R)
 26 CAP_SEARCH (R)
 27-30 SYNTH_SM_STATE (R)
 31 SYNTH_ON (R) |
| --- |

**ATH:01C098h - PHY_ANALOG_SYNTH7**

```

| 0 OVRCHANDECODER
 1 FORCE_FRACLSB
 2-18 CHANFRAC
 19-27 CHANSEL
 28-29 AMODEREFSEL
 30 FRACMODE
 31 LOADSYNTHCHANNEL |
| --- |

**ATH:01C09Ch - PHY_ANALOG_SYNTH8**

```

| 0 CPSTEERING_EN_FRACN
 1-7 LOOP_ICPB
 8-11 LOOP_CSB
 12-16 LOOP_RSB
 17-21 LOOP_CPB
 22-26 LOOP_3RD_ORDER_RB
 27-31 REFDIVB |
| --- |

**ATH:01C0A0h - PHY_ANALOG_SYNTH9**

```

| 0 PFDDELAY_INTN
 1-3 SLOPE_ICPA0
 4-7 LOOP_ICPA0
 8-11 LOOP_CSA0
 12-16 LOOP_RSA0
 17-21 LOOP_CPA0
 22-26 LOOP_3RD_ORDER_RA
 27-31 REFDIVA |
| --- |

**ATH:01C0A4h - PHY_ANALOG_SYNTH10**

```

| 0-1 SPARE10A
 2-4 PWDB_ICLOBIAS50
 5-7 PWDB_IRSPARE25
 8-10 PWDB_ICSPARE25
 11-13 SLOPE_ICPA1
 14-17 LOOP_ICPA1
 18-21 LOOP_CSA1
 22-26 LOOP_RSA1
 27-31 LOOP_CPA1 |
| --- |

**ATH:01C0A8h - PHY_ANALOG_SYNTH11**

```

| 0-4 SPARE11A
 5 FORCE_LOBUF5G_ON
 6-7 LOREFSEL
 8-9 LOBUF2GTUNE
 10 CPSTEERING_MODE
 11-13 SLOPE_ICPA2
 14-17 LOOP_ICPA2
 18-21 LOOP_CSA2
 22-26 LOOP_RSA2
 27-31 LOOP_CPA2 |
| --- |

**ATH:01C0ACh - PHY_ANALOG_SYNTH12**

```

| 0-9 SPARE12A
 10-13 LOOPLEAKCUR_FRACN
 14 CPLOWLK_FRACN
 15-16 CPBIAS_FRACN
 17 SYNTHDIGOUTEN
 18 STRCONT
 19-22 VREFMUL3
 23-26 VREFMUL2
 27-30 VREFMUL1
 31 CLK_DOUBLER_EN |
| --- |

**ATH:01C0B0h - PHY_ANALOG_SYNTH13**

```

| 0 SPARE13A
 1-3 SLOPE_ICPA_FRACN
 4-7 LOOP_ICPA_FRACN
 8-11 LOOP_CSA_FRACN
 12-16 LOOP_RSA_FRACN
 17-21 LOOP_CPA_FRACN
 22-26 LOOP_3RD_ORDER_RA_FRACN
 27-31 REFDIVA_FRACN |
| --- |

**ATH:01C0B4h - PHY_ANALOG_SYNTH14**

```

| 0-1 SPARE14A
 2-3 LOBUF5GTUNE_3
 4-5 LOBUF2GTUNE_3
 6-7 LOBUF5GTUNE_2
 8-9 LOBUF2GTUNE_2
 10 PWD_LOBUF5G_3
 11 PWD_LOBUF2G_3
 12 PWD_LOBUF5G_2
 13 PWD_LOBUF2G_2
 14-16 PWUPLO23_PD
 17-19 PWDB_ICLOBUF5G50_3
 20-22 PWDB_ICLOBUF2G50_3
 23-25 PWDB_ICLOBUF5G50_2
 26-28 PWDB_ICLOBUF2G50_2
 29-31 PWDB_ICLVLSHFT |
| --- |

**ATH:01C0C0h - PHY_ANALOG_BIAS1**

```

| 0-6 SPARE1
 7-9 PWD_IC25V2IQ
 10-12 PWD_IC25V2II
 13-15 PWD_IC25BB
 16-18 PWD_IC25DAC
 19-21 PWD_IC25FIR
 22-24 PWD_IC25ADC
 25-31 BIAS_SEL |
| --- |

**ATH:01C0C4h - PHY_ANALOG_BIAS2**

```

| 0-4 SPARE2
 5-7 PWD_IC25XPA
 8-10 PWD_IC25XTAL
 11-13 PWD_IC25TXRF
 14-16 PWD_IC25RXRF
 17-19 PWD_IC25SYNTH
 20-22 PWD_IC25PLLREG
 23-25 PWD_IC25PLLCP2
 26-28 PWD_IC25PLLCP
 29-31 PWD_IC25PLLGM |
| --- |

**ATH:01C0C8h - PHY_ANALOG_BIAS3**

```

| 0-1 SPARE3
 2-4 PWD_IR25SAR
 5-7 PWD_IR25TXRF
 8-10 PWD_IR25RXRF
 11-13 PWD_IR25SYNTH
 14-16 PWD_IR25PLLREG
 17-19 PWD_IR25BB
 20-22 PWD_IR50DAC
 23-25 PWD_IR25DAC
 26-28 PWD_IR25FIR
 29-31 PWD_IR50ADC |
| --- |

**ATH:01C0CCh - PHY_ANALOG_BIAS4**

```

| 0-10 SPARE4
 11-13 PWD_IR25SPARED
 14-16 PWD_IR25SPAREC
 17-19 PWD_IR25SPAREB
 20-22 PWD_IR25XPA
 23-25 PWD_IC25SPAREC
 26-28 PWD_IC25SPAREB
 29-31 PWD_IC25SPAREA |
| --- |

**ATH:01C100h - PHY_ANALOG_RXTX1**

```

| 0 SCFIR_GAIN
 1 MANRXGAIN
 2-5 AGC_DBDAC
 6 OVR_AGC_DBDAC
 7 ENABLE_PAL
 8 ENABLE_PAL_OVR
 9-11 TX1DB_BIQUAD
 12-13 TX6DB_BIQUAD
 14 PADRVHALFGN2G
 15-18 PADRV2GN
 19-22 PADRV3GN5G
 23-26 PADRV4GN5G
 27-30 TXBB_GC
 31 MANTXGAIN |
| --- |

**ATH:01C104h - PHY_ANALOG_RXTX2**

```

| 0 BMODE
 1 BMODE_OVR
 2 SYNTHON
 3 SYNTHON_OVR
 4-5 BW_ST
 6 BW_ST_OVR
 7 TXON
 8 TXON_OVR
 9 PAON
 10 PAON_OVR
 11 RXON
 12 RXON_OVR
 13 AGCON
 14 AGCON_OVR
 15-17 TXMOD
 18 TXMOD_OVR
 19-21 RX1DB_BIQUAD
 22-23 RX6DB_BIQUAD
 24-25 MXRGAIN
 26-28 VGAGAIN
 29-31 LNAGAIN |
| --- |

**ATH:01C108h - PHY_ANALOG_RXTX3**

```

| 0-2 SPARE3
 3 SPURON
 4 PAL_LOCKEDEN
 5 DACFULLSCALE
 6 ADCSHORT
 7 DACPWD
 8 DACPWD_OVR
 9 ADCPWD
 10 ADCPWD_OVR
 11-16 AGC_CALDAC
 17 AGC_CAL
 18 AGC_CAL_OVR
 19 LOFORCEDON
 20 CALRESIDUE
 21 CALRESIDUE_OVR
 22 CALFC
 23 CALFC_OVR
 24 CALTX
 25 CALTX_OVR
 26 CALTXSHIFT
 27 CALTXSHIFT_OVR
 28 CALPA
 29 CALPA_OVR
 30 TURBOADC
 31 TURBOADC_OVR |
| --- |

**ATH:01C140h - PHY_ANALOG_BB1**

```

| 0 I2V_CURR2X
 1 ENABLE_LOQ
 2 FORCE_LOQ
 3 ENABLE_NOTCH
 4 FORCE_NOTCH
 5 ENABLE_BIQUAD
 6 FORCE_BIQUAD
 7 ENABLE_OSDAC
 8 FORCE_OSDAC
 9 ENABLE_V2I
 10 FORCE_V2I
 11 ENABLE_I2V
 12 FORCE_I2V
 13-15 CMSEL
 16-17 ATBSEL
 18 PD_OSDAC_CALTX_CALPA
 19-23 OFSTCORRI2VQ
 24-28 OFSTCORRI2VI
 29 LOCALOFFSET
 30-31 RANGE_OSDAC |
| --- |

**ATH:01C144h - PHY_ANALOG_BB2**

```

| 0-3 SPARE
 4-7 MXR_HIGHGAINMASK
 8-9 SEL_TEST
 10-14 RCFILTER_CAP
 15 OVERRIDE_RCFILTER_CAP
 16-19 FNOTCH
 20 OVERRIDE_FNOTCH
 21-25 FILTERFC
 26 OVERRIDE_FILTERFC
 27 I2V2RXOUT_EN
 28 BQ2RXOUT_EN
 29 RXIN2I2V_EN
 30 RXIN2BQ_EN
 31 SWITCH_OVERRIDE |
| --- |

**ATH:01C148h - PHY_ANALOG_BB3**

```

| 0-7 SPARE
 8-15 hw4: SPARE
 8-9 hw6: SEL_OFST_READBK ;\
 10 hw6: OVERRIDE_RXONLY_FILTERFC ; hw6.0 only
 11-15 hw6: RXONLY_FILTERFC ;/
 16-20 FILTERFC (R)
 21-25 OFSTCORRI2VQ (R)
 26-30 OFSTCORRI2VI (R)
 31 EN_TXBBCONSTCUR |
| --- |

**ATH:01C280h - PHY_ANALOG_PLLCLKMODA**

```

| 0 PWD_PLLSDM
 1 PWDPLL
 2-16 PLLFRAC
 17-20 REFDIV
 21-30 DIV
 31 LOCAL_PLL |
| --- |

**ATH:01C284h - PHY_ANALOG_PLLCLKMODA2**

```

| 0-3 SPARE
 4 DACPWD
 5 ADCPWD
 6 LOCAL_ADDAC
 7-8 DAC_CLK_SEL
 9-12 ADC_CLK_SEL
 13 LOCAL_CLKMODA
 14 PLLBYPASS
 15 LOCAL_PLLBYPASS
 16-17 PLLATB
 18 PLL_SVREG
 19 HI_FREQ_EN
 20 RST_WARM_INT_L
 21 RST_WARM_OVR
 22-23 PLL_KVCO
 24-26 PLLICP
 27-31 PLLFILTER |
| --- |

**ATH:01C288h - PHY_ANALOG_TOP**

```

| 0-2 SPARE
 3 PWDBIAS
 4 FLIP_XPABIAS
 5 XPAON2
 6 XPAON5
 7 XPASHORT2GND
 8-11 XPABIASLVL
 12 XPABIAS_EN
 13 ATBSELECT
 14 LOCAL_XPA
 15 XPABIAS_BYPASS
 16 TEST_PADQ_EN
 17 TEST_PADI_EN
 18 TESTIQ_RSEL
 19 TESTIQ_BUFEN
 20 PAD2GND
 21 INTH2PAD
 22 INTH2GND
 23 INT2PAD
 24 INT2GND
 25 PWDPALCLK
 26 INV_CLK320_ADC
 27 FLIP_REFCLK40
 28 FLIP_PLLCLK320
 29 FLIP_PLLCLK160
 30-31 CLK_SEL |
| --- |

**ATH:01C28Ch - PHY_ANALOG_THERM**

```

| 0-2 LOREG_LVL
 3-5 RFREG_LVL
 6 SAR_ADC_DONE (R)
 7-14 SAR_ADC_OUT (R)
 15-22 SAR_DACTEST_CODE
 23 SAR_DACTEST_EN
 24 SAR_ADCCAL_EN
 25-26 THERMSEL
 27 SAR_SLOW_EN
 28 THERMSTART
 29 SAR_AUTOPWD_EN
 30 THERMON
 31 LOCAL_THERM |
| --- |

**ATH:01C290h - PHY_ANALOG_XTAL**

```

| 0-5 SPARE
 6 XTAL_NOTCXODET
 7 LOCALBIAS2X
 8 LOCAL_XTAL
 9 XTAL_PWDCLKIN
 10 XTAL_OSCON
 11 XTAL_PWDCLKD
 12 XTAL_LOCALBIAS
 13 XTAL_SHRTXIN
 14-15 XTAL_DRVSTR
 16-22 XTAL_CAPOUTDAC
 23-29 XTAL_CAPINDAC
 30 XTAL_BIAS2X
 31 TCXODET (R) |
| --- |

**ATH:01C380h - PHY_ANALOG_RBIST_CNTRL**

```

| 0 ATE_TONEGEN_DC_ENABLE
 1 ATE_TONEGEN_TONE0_ENABLE
 2 ATE_TONEGEN_TONE1_ENABLE
 3 ATE_TONEGEN_LFTONE0_ENABLE
 4 ATE_TONEGEN_LINRAMP_ENABLE_I
 5 ATE_TONEGEN_LINRAMP_ENABLE_Q
 6 ATE_TONEGEN_PRBS_ENABLE_I
 7 ATE_TONEGEN_PRBS_ENABLE_Q
 8 ATE_CMAC_DC_WRITE_TO_CANCEL
 9 ATE_CMAC_DC_ENABLE
 10 ATE_CMAC_CORR_ENABLE
 11 ATE_CMAC_POWER_ENABLE
 12 ATE_CMAC_IQ_ENABLE
 13 ATE_CMAC_I2Q2_ENABLE
 14 ATE_CMAC_POWER_HPF_ENABLE
 15 ATE_RXDAC_CALIBRATE
 16 ATE_RBIST_ENABLE
 17 ATE_ADC_CLK_INVERT ;-newer revision only |
| --- |

**ATH:01C384h - PHY_ANALOG_TX_DC_OFFSET**

```

| 0-10 ATE_TONEGEN_DC_I
 16-26 ATE_TONEGEN_DC_Q |
| --- |

**ATH:01C388h - PHY_ANALOG_TX_TONEGEN0**

**ATH:01C38Ch - PHY_ANALOG_TX_TONEGEN1**

**ATH:01C390h - PHY_ANALOG_TX_LFTONEGEN0**

```

| 0-6 ATE_TONEGEN_TONE_FREQ
 8-11 ATE_TONEGEN_TONE_A_EXP
 16-23 ATE_TONEGEN_TONE_A_MAN
 24-30 ATE_TONEGEN_TONE_TAU_K |
| --- |

**ATH:01C394h - PHY_ANALOG_TX_LINEAR_RAMP_I**

**ATH:01C398h - PHY_ANALOG_TX_LINEAR_RAMP_Q**

```

| 0-10 ATE_TONEGEN_LINRAMP_INIT
 12-21 ATE_TONEGEN_LINRAMP_DWELL
 24-29 ATE_TONEGEN_LINRAMP_STEP |
| --- |

**ATH:01C39Ch - PHY_ANALOG_TX_PRBS_MAG**

```

| 0-9 ATE_TONEGEN_PRBS_MAGNITUDE_I
 16-25 ATE_TONEGEN_PRBS_MAGNITUDE_Q |
| --- |

**ATH:01C3A0h - PHY_ANALOG_TX_PRBS_SEED_I**

```

| 0-30 ATE_TONEGEN_PRBS_SEED |
| --- |

**ATH:01C3A4h - PHY_ANALOG_TX_PRBS_SEED_Q**

```

| 0-30 ATE_TONEGEN_PRBS_SEED |
| --- |

**ATH:01C3A8h - PHY_ANALOG_CMAC_DC_CANCEL**

```

| 0-9 ATE_CMAC_DC_CANCEL_I
 16-25 ATE_CMAC_DC_CANCEL_Q |
| --- |

**ATH:01C3ACh - PHY_ANALOG_CMAC_DC_OFFSET**

```

| 0-3 ATE_CMAC_DC_CYCLES |
| --- |

**ATH:01C3B0h - PHY_ANALOG_CMAC_CORR**

```

| 0-4 ATE_CMAC_CORR_CYCLES
 8-13 ATE_CMAC_CORR_FREQ |
| --- |

**ATH:01C3B4h - PHY_ANALOG_CMAC_POWER**

```

| 0-3 ATE_CMAC_POWER_CYCLES |
| --- |

**ATH:01C3B8h - PHY_ANALOG_CMAC_CROSS_CORR**

```

| 0-3 ATE_CMAC_IQ_CYCLES |
| --- |

**ATH:01C3BCh - PHY_ANALOG_CMAC_I2Q2**

```

| 0-3 ATE_CMAC_I2Q2_CYCLES |
| --- |

**ATH:01C3C0h - PHY_ANALOG_CMAC_POWER_HPF**

```

| 0-3 ATE_CMAC_POWER_HPF_CYCLES
 4-7 ATE_CMAC_POWER_HPF_WAIT |
| --- |

**ATH:01C3C4h - PHY_ANALOG_RXDAC_SET1**

```

| 0-1 ATE_RXDAC_MUX
 4 ATE_RXDAC_HI_GAIN
 8-13 ATE_RXDAC_CAL_WAIT
 16-19 ATE_RXDAC_CAL_MEASURE_TIME |
| --- |

**ATH:01C3C8h - PHY_ANALOG_RXDAC_SET2**

```

| 0-4 ATE_RXDAC_I_HI
 8-12 ATE_RXDAC_Q_HI
 16-20 ATE_RXDAC_I_LOW
 24-28 ATE_RXDAC_Q_LOW |
| --- |

**ATH:01C3CCh - PHY_ANALOG_RXDAC_LONG_SHIFT**

```

| 0-4 ATE_RXDAC_I_STATIC
 8-12 ATE_RXDAC_Q_STATIC |
| --- |

**ATH:01C3D0h - PHY_ANALOG_CMAC_RESULTS_I**

```

| 0-31 ATE_CMAC_RESULTS |
| --- |

**ATH:01C3D4h - PHY_ANALOG_CMAC_RESULTS_Q**

```

| 0-31 ATE_CMAC_RESULTS |
| --- |

**ATH:01C740h - PHY_ANALOG_PMU1**

This register differs in hw4/hw6 (in hw6, bits are renumbered, new OVERRIDE
bits are added, and the SREG_LVLCTR bit is removed):

```

| hw4 hw6 name
 0-10 0-3 SPARE ;-unused
 11 4 OTP_V25_PWD ;-OTP V25
 12 5 PAREGON_MAN ;\PA REG
 - 6 PAREGON_OVERRIDE_EN ;/
 13 7 OTPREGON_MAN ;\OTP REG
 - 8 OTPREGON_OVERRIDE_EN ;/
 14 9 DREGON_MAN ;\DREG
 - 10 DREGON_OVERRIDE_EN ;/
 15 11 DISCONTMODEEN ;\DISCONT MODE
 - 12 SWREGDISCONT_OVERRIDE_EN ;/
 16 13 SWREGON_MAN ;\
 - 14 SWREGON_OVERRIDE_EN ;
 17-18 15-16 SWREG_FREQCUR ;
 19-21 17-19 SWREG_FREQCAP ; SW REG
 - 20 SWREGFREQ_OVERRIDE_EN ;
 22-23 21-22 SWREG_LVLCTR ;
 - 23 SWREGLVL_OVERRIDE_EN ;/
 24-25 - hw4:SREG_LVLCTR ;-SREG ;<---- removed in hw6 (!)
 26-27 24-25 DREG_LVLCTR ;\DREG
 - 26 DREGLVL_OVERRIDE_EN ;/
 28 27 PAREG_XPNP ;\
 29-31 28-30 PAREG_LVLCTR ; PA REG
 - 31 PAREGLVL_OVERRIDE_EN ;/ |
| --- |

**ATH:01C744h - PHY_ANALOG_PMU2**

```

| 0-7 SPARE
 8 VBATT_1_3TOATB
 9 VBATT_1_2TOATB
 10 VBATT_2_3TOATB
 11 PWD_BANDGAP_MAN
 12 PWD_LFO_MAN
 13 VBATT_LT_3P2
 14 VBATT_LT_2P8
 15 VBATT_GT_4P2
 16 hw4: PMU_MAN_OVERRIDE_EN ;\changed/renamed in hw4/hw6
 16 hw6: PMU_XPNP_OVERRIDE_EN ;/
 17-18 VBATT_GT_LVLCTR
 19 SWREGVSSL2ATB
 20-21 SWREGVSSL_LVLCTR
 22 SWREGVDDH2ATB
 23-24 SWREGVDDH_LVLCTR
 25-27 SWREG2ATB
 28 OTPREG2ATB
 29-30 OTPREG_LVLCTR
 31 hw4: DREG_LVLCTR_MANOVR_EN ;\changed/renamed in hw4/hw6
 31 hw6: OTPREG_LVLCTR_MANOVR_EN ;/ |
| --- |

| DSi Atheros Wifi - Internal I/O - 020000h - WMAC DMA (hw4/hw6) |
| --- |

**ATH:020008h - MAC_DMA_CR - MAC Control Register**

```

| 0-1 -
 2 hw4: Receive enable (RXE) (R) ;\one bit in hw4,
 2 hw6: Receive LP enable (RXE_LP) (R) ; two bits in hw6
 3 hw6: Receive HP enable (RXE_HP) (R) ;/
 4 -
 5 Receive disable (RXD)
 6 One-shot software interrupt (SWI) (R) |
| --- |

**ATH:02000Ch - MAC_DMA_RXDP - MAC receive queue descriptor pointer ;hw4 only**

```

| .. Pointer <------------ HW4 ONLY |
| --- |

**ATH:020014h - MAC_DMA_CFG - MAC configuration and status register**

```

| 0 Byteswap TX descriptor words (BE_MODE_XMIT_DESC)
 1 Byteswap TX data buffer words (BE_MODE_XMIT_DATA)
 2 Byteswap RX descriptor words (BE_MODE_RCV_DESC)
 3 Byteswap RX data buffer words (BE_MODE_RCV_DATA)
 4 Byteswap register access data words (BE_MODE_MMR)
 5 AP/adhoc indication (ADHOC) (0=AP, 1=Adhoc)
 6-7 -
 8 PHY OK status (PHY_OK) (R)
 9 hw6: EEPROM_BUSY (R) ;-hw6 only
 10 Clock gating disable (CLKGATE_DIS)
 11 hw6: HALT_REQ ;\
 12 hw6: HALT_ACK (R) ;
 13-16 - ;
 17-18 hw6: REQ_Q_FULL_THRESHOLD ; hw6 only
 19 hw6: MISSING_TX_INTR_FIX_ENABLE ;
 20 hw6: LEGACY_INT_MIT_MODE_ENABLE ;
 21 hw6: RESET_INT_MIT_CNTRS ;/ |
| --- |

**ATH:020018h - MAC_DMA_RXBUFPTR_THRESH ;hw6 only**

```

| 0-3 hw6: HP_DATA ;\hw6 only
 8-14 hw6: LP_DATA ;/ |
| --- |

**ATH:02001Ch - MAC_DMA_TXDPPTR_THRESH ;hw6 only**

```

| 0-3 DATA ;-hw6 only |
| --- |

**ATH:020020h - MAC_DMA_MIRT - Maximum rate threshold register**

```

| 0-15 Threshold (RATE_THRESH) |
| --- |

**ATH:020024h - MAC_DMA_IER aka MAC_DMA_GLOBAL_IER - MAC Interrupt enable**

```

| 0 Global interrupt enable (0=Disable, 1=Enable) |
| --- |

**ATH:020028h - MAC_DMA_TIMT_0 - Transmit Interrupt Mitigation Threshold**

**ATH:02002Ch - MAC_DMA_RIMT - Receive Interrupt Mitigation Threshold**

```

| 0-15 Last packet threshold (LAST_PKT_THRESH)
 16-31 First packet threshold (FIRST_PKT_THRESH) |
| --- |

**ATH:020030h - MAC_DMA_TXCFG - MAC Transmit DMA size config register**

In hw4, most bits are left undefined, however, hw4 DOES refer to the register
as "MAC tx DMA size config", so one may assume that at least the SIZE value in
bit0-2 does exist in hw4, too.

```

| 0-2 hw6: DMA_SIZE (maybe as in RXCFG below?) ;-hw6 only (???)
 3 -
 4-9 Frame trigger level (TRIGLVL)
 10 hw6: JUMBO_EN ;-hw6 only (??)
 11 ADHOC_BEACON_ATIM_TX_POLICY (hw6name: BCN_PAST_ATIM_DIS)
 12 hw6: ATIM_DEFER_DIS ;\
 13 - ;
 14 hw6: RTCI_DIS ; hw6 only (?)
 15-16 - ;
 17 hw6: DIS_RETRY_UNDERRUN ;
 18 hw6: DIS_CW_INC_QUIET_COLL ;
 19 hw6: RTS_FAIL_EXCESSIVE_RETRIES ;/ |
| --- |

Blurb...

```

| MAC_DMA_FTRIG_IMMED = 0x00000000 ;bytes in PCU TX FIFO before air
 MAC_DMA_FTRIG_64B = 0x00000010 ;default
 MAC_DMA_FTRIG_128B = 0x00000020
 MAC_DMA_FTRIG_192B = 0x00000030
 MAC_DMA_FTRIG_256B = 0x00000040 ;5 bits total |
| --- |

**ATH:020034h - MAC_DMA_RXCFG - MAC rx DMA size config register**

```

| 0-2 DMA Size (0..7 = 4,8,16,32,64,128,256,512 bytes)
 3-4 hw6: ZERO_LEN_DMA_EN ;-hw6: two bits?
 4 hw4: Enable DMA of zero-length frame ;-hw4: one bit?
 5 hw6: JUMBO_EN ;\
 6 hw6: JUMBO_WRAP_EN ; hw6 only (?)
 7 hw6: SLEEP_RX_PEND_EN ;/ |
| --- |

Blurb...

```

| MAC_DMA_RXCFG_DMASIZE_4B = 0x00000000 ;DMA size 4 bytes (TXCFG + RXCFG)
 MAC_DMA_RXCFG_DMASIZE_8B = 0x00000001 ;DMA size 8 bytes
 MAC_DMA_RXCFG_DMASIZE_16B = 0x00000002 ;DMA size 16 bytes
 MAC_DMA_RXCFG_DMASIZE_32B = 0x00000003 ;DMA size 32 bytes
 MAC_DMA_RXCFG_DMASIZE_64B = 0x00000004 ;DMA size 64 bytes
 MAC_DMA_RXCFG_DMASIZE_128B = 0x00000005 ;DMA size 128 bytes
 MAC_DMA_RXCFG_DMASIZE_256B = 0x00000006 ;DMA size 256 bytes
 MAC_DMA_RXCFG_DMASIZE_512B = 0x00000007 ;DMA size 512 bytes |
| --- |

**ATH:020038h - MAC_DMA_RXJLA (R) ;hw6 only**

```

| 31-2 DATA (R) ;-hw6 only |
| --- |

**ATH:020040h - MAC_DMA_MIBC - MAC MIB control register**

```

| 0 counter overflow warning (WARNING) (R)
 1 freeze MIB counters (FREEZE)
 2 clear MIB counters (CLEAR)
 3 MIB counter strobe, increment all (STROBE) (R) |
| --- |

**ATH:020044h - MAC_DMA_TOPS - MAC timeout prescale count**

```

| 0-15 Timeout prescale (TIMEOUT) |
| --- |

**ATH:020048h - MAC_DMA_RXNPTO - MAC no frame received timeout**

```

| 0-9 No frame received timeout (TIMEOUT) |
| --- |

**ATH:02004Ch - MAC_DMA_TXNPTO - MAC no frame trasmitted timeout**

```

| 0-9 No frame transmitted timeout (TIMEOUT)
 10-19 QCU Mask (QCU 0-9) ;QCU's for which frame completions will cause
 a reset of the no frame xmit'd timeout |
| --- |

**ATH:020050h - MAC_DMA_RPGTO - MAC receive frame gap timeout**

```

| 0-9 Receive frame gap timeout (TIMEOUT) |
| --- |

**ATH:020054h - MAC_DMA_RPCNT - MAC receive frame count limit ;hw4 only**

```

| 0-4 Receive frame count limit ;-hw4 only |
| --- |

**ATH:020058h - MAC_DMA_MACMISC - MAC miscellaneous control/status register**

```

| 4 hw6: FORCE_PCI_EXT ;-hw6 only
 5-8 DMA observation bus mux select (DMA_OBS_MUXSEL)
 9-11 MISC observation bus mux select (MISC_OBS_MUXSEL)
 12-14 MAC observation bus mux select (lsb) (MISC_F2_OBS_LOW_MUXSEL)
 15-17 MAC observation bus mux select (msb) (MISC_F2_OBS_HIGH_MUXSEL) |
| --- |

```

| ____________ below in hw6 only ____________ |
| --- |

**ATH:02005Ch - MAC_DMA_INTER ;hw6 only**

```

| 0 REQ
 1-2 MSI_RX_SRC
 3-4 MSI_TX_SRC |
| --- |

**ATH:020060h - MAC_DMA_DATABUF ;hw6 only**

```

| 0-11 LEN |
| --- |

**ATH:020064h - MAC_DMA_GTT ;hw6 only**

**ATH:02006Ch - MAC_DMA_CST ;hw6 only**

```

| 0-15 COUNT
 16-31 LIMIT |
| --- |

**ATH:020068h - MAC_DMA_GTTM ;hw6 only**

```

| 0 USEC_STROBE
 1 IGNORE_CHAN_IDLE
 2 RESET_ON_CHAN_IDLE
 3 CST_USEC_STROBE
 4 DISABLE_QCU_FR_ACTIVE_GTT
 5 DISABLE_QCU_FR_ACTIVE_BT |
| --- |

**ATH:020070h - MAC_DMA_RXDP_SIZE ;hw6 only**

```

| 0-7 LP (R)
 8-12 HP (R) |
| --- |

**ATH:020074h - MAC_DMA_RX_QUEUE_HP_RXDP ;hw6 only**

**ATH:020078h - MAC_DMA_RX_QUEUE_LP_RXDP ;hw6 only**

```

| 0-31 ADDR |
| --- |

**ATH:0200E0h - MAC_DMA_DMADBG_0 (R) - hw6 only**

**ATH:0200E4h - MAC_DMA_DMADBG_1 (R) - hw6 only**

**ATH:0200E8h - MAC_DMA_DMADBG_2 (R) - hw6 only**

**ATH:0200ECh - MAC_DMA_DMADBG_3 (R) - hw6 only**

**ATH:0200F0h - MAC_DMA_DMADBG_4 (R) - hw6 only**

**ATH:0200F4h - MAC_DMA_DMADBG_5 (R) - hw6 only**

**ATH:0200F8h - MAC_DMA_DMADBG_6 (R) - hw6 only**

**ATH:0200FCh - MAC_DMA_DMADBG_7 (R) - hw6 only**

```

| 0-31 DATA (R) ;-hw6 only |
| --- |

**ATH:020100h - MAC_DMA_QCU_TXDP_REMAINING_QCU_7_0 (R) ;hw6 only**

**ATH:020104h - MAC_DMA_QCU_TXDP_REMAINING_QCU_9_8 (R) ;hw6 only**

```

| 0-39 For QCU 0-9 (4bits each) (R) ;\hw6 only
 40-63 - ;/ |
| --- |

**(see above) - MAC_DMA_TIMT_0 - hw4/hw6**

**ATH:020108h - MAC_DMA_TIMT_1 - hw6 only**

**ATH:02010Ch - MAC_DMA_TIMT_2 - hw6 only**

**ATH:020110h - MAC_DMA_TIMT_3 - hw6 only**

**ATH:020114h - MAC_DMA_TIMT_4 - hw6 only**

**ATH:020118h - MAC_DMA_TIMT_5 - hw6 only**

**ATH:02011Ch - MAC_DMA_TIMT_6 - hw6 only**

**ATH:020120h - MAC_DMA_TIMT_7 - hw6 only**

**ATH:020124h - MAC_DMA_TIMT_8 - hw6 only**

**ATH:020128h - MAC_DMA_TIMT_9 - hw6 only**

```

| 0-15 LAST_PKT_THRESH
 16-31 FIRST_PKT_THRESH |
| --- |

**ATH:02012Ch - MAC_DMA_CHKACC - hw6 only**

```

| 0 CHKSUM_SEL |
| --- |

| DSi Atheros Wifi - Internal I/O - 020080h - WMAC IRQ Interrupt (hw4/hw6) |
| --- |

**ATH:020080h - MAC_DMA_ISR(_P) - MAC Primary interrupt status register**

**ATH:0200A0h - MAC_DMA_IMR(_P) - MAC Primary interrupt mask register**

**ATH:0200C0h - MAC_DMA_ISR(_P)_RAC - MAC Primary interrupt read-and-clear**

Interrupt Status Registers

Only the bits in the ISR_P register and the IMR_P registers control whether the
MAC's INTA# output is asserted. The bits in the secondary interrupt status/mask
registers control what bits are set in the primary interrupt status register;
however the IMR_S* registers DO NOT determine whether INTA# is asserted. That
is INTA# is asserted only when the logical AND of ISR_P and IMR_P is non-zero.
The secondary interrupt mask/status registers affect what bits are set in ISR_P
but they do not directly affect whether INTA# is asserted.

Interrupt Mask Registers

Only the bits in the IMR control whether the MAC's INTA# output will be
asserted. The bits in the secondary interrupt mask registers control what bits
get set in the primary interrupt status register; however the IMR_S* registers
DO NOT determine whether INTA# is asserted.

Read-and-Clear Registers

The read-and-clear registers are read-only (R) "shadow copies" of the interrupt
status registers, with read-and-clear access.

```

| 0 At least one frame received sans errors ;\
 1 Receive interrupt request ;
 2 Receive error interrupt ; RX
 3 No frame received within timeout clock ;
 4 Received descriptor empty interrupt ;
 5 Receive FIFO overrun interrupt ;/
 6 Transmit okay interrupt ;\ ;<-- ISR_S0.Bit0..9
 7 Transmit interrupt request ;
 8 Transmit error interrupt ; TX ;<-- ISR_S1.Bit0..9
 9 No frame transmitted interrupt ;
 10 Transmit descriptor empty interrupt ;
 11 Transmit FIFO underrun interrupt ;/ ;<-- ISR_S2.Bit0..9
 12 MIB interrupt - see MIBC
 13 Software interrupt
 14 PHY receive error interrupt
 15 Key-cache miss interrupt
 16 Beacon rssi high threshold interrupt ;aka Beacon rssi hi threshold
 17 Beacon threshold interrupt ;aka Beacon rssi lo threshold
 18 Beacon missed interrupt
 19 Maximum transmit interrupt rate
 20 Beacon not ready interrupt ;aka BNR interrupt
 21 An unexpected bus error has occurred
 22 -
 23 Beacon Misc (TIM, CABEND, DTIMSYNC, BCNTO) ;<-- ISR_S2.Bit24..27
 24 Maximum receive interrupt rate
 25 QCU CBR overflow interrupt ;<-- ISR_S3.Bit0..9
 26 QCU CBR underrun interrupt ;<-- ISR_S3.Bit16..27
 27 QCU scheduling trigger interrupt ;<-- ISR_S4.Bit0..9
 28 GENTMR interrupt (aka GENERIC_TIMERS... and/or ISR_S5?)
 29 HCFTO interrupt
 30 Transmit completion mitigation interrupt
 31 Receive completion mitigation interrupt |
| --- |

**ATH:020084h - MAC_DMA_ISR_S0 - MAC Secondary Interrupt 0 Stat TX OK/DESC**

**ATH:0200A4h - MAC_DMA_IMR_S0 - MAC Secondary Interrupt 0 Mask TX OK/DESC**

**ATH:0200C4h - MAC_DMA_ISR_S0_S - MAC Secondary Interrupt 0 Read-and-Clear**

```

| 0-9 TXOK (QCU 0-9) ;--> Primary_ISR.Bit6
 16-27 TXDESC (QCU 0-9) ;--> Primary_ISR. ?? |
| --- |

**ATH:020088h - MAC_DMA_ISR_S1 - MAC Secondary Interrupt 1 Stat TX ERR/EOL**

**ATH:0200A8h - MAC_DMA_IMR_S1 - MAC Secondary Interrupt 1 Mask TX ERR/EOL**

**ATH:0200C8h - MAC_DMA_ISR_S1_S - MAC Secondary Interrupt 1 Read-and-Clear**

```

| 0-9 TXERR (QCU 0-9) ;--> Primary_ISR.Bit8
 16-27 TXEOL (QCU 0-9) |
| --- |

**ATH:02008Ch - MAC_DMA_ISR_S2 - MAC Secondary Interrupt 2 Stat TX URN/MISC**

**ATH:0200ACh - MAC_DMA_IMR_S2 - MAC Secondary Interrupt 2 Mask TX URN/MISC**

**ATH:0200CCh (hw6:000200D0h?) - MAC_DMA_ISR_S2_S - MAC .. 2 Read-and-Clear**

```

| 0-9 TXURN (QCU 0-9) ;--> Primary_ISR.Bit11
 10 -
 11 RX_INT ;RX
 12 WL_STOMPED
 13 RX_PTR_BAD ;RX
 14 BT_LOW_PRIORITY_RISING
 15 BT_LOW_PRIORITY_FALLING
 16 BB_PANIC_IRQ
 17 BT_STOMPED
 18 BT_ACTIVE_RISING
 19 BT_ACTIVE_FALLING
 20 BT_PRIORITY_RISING
 21 BT_PRIORITY_FALLING
 22 CST
 23 GTT
 24 TIM ;\
 25 CABEND ; Beacon Misc --> Primary_ISR.Bit23
 26 DTIMSYNC ;
 27 BCNTO ;/
 28 CABTO
 29 DTIM
 30 TSFOOR
 31 - |
| --- |

**ATH:020090h - MAC_DMA_ISR_S3 - MAC Secondary Interrupt 3 Stat QCBR OVF/URN**

**ATH:0200B0h - MAC_DMA_IMR_S3 - MAC Secondary Interrupt 3 Mask QCBR OVF/URN**

**ATH:0200D0h (hw6:000200D4h?) - MAC_DMA_ISR_S3_S - MAC .. 3 Read-and-Clear**

```

| 0-9 QCBROVF (QCU 0-9) ;--> Primary_ISR.Bit25
 16-27 QCBRURN (QCU 0-9) ;--> Primary_ISR.Bit26 |
| --- |

**ATH:020094h - MAC_DMA_ISR_S4 - MAC Secondary Interrupt 4 Stat QTRIG**

**ATH:0200B4h - MAC_DMA_IMR_S4 - MAC Secondary Interrupt 4 Mask QTRIG**

**ATH:0200D4h (hw6:000200D8h?) - MAC_DMA_ISR_S4_S - MAC .. 4 Read-and-Clear**

```

| 0-9 QTRIG (QCU 0-9) ;--> Primary_ISR.Bit27 |
| --- |

**ATH:020098h - MAC_DMA_ISR_S5 - MAC Secondary Interrupt 5 Stat TIMERS**

**ATH:0200B8h - MAC_DMA_IMR_S5 - MAC Secondary Interrupt 5 Mask TIMERS**

**ATH:0200D8h (hw6:000200DCh?) - MAC_DMA_ISR_S5_S - MAC .. 5 Read-and-Clear**

```

| 0 TBTT_TIMER_TRIGGER ;-TBTT timer
 1 DBA_TIMER_TRIGGER ;\
 2 SBA_TIMER_TRIGGER ;
 3 HCF_TIMER_TRIGGER ;
 4 TIM_TIMER_TRIGGER ; timer's
 5 DTIM_TIMER_TRIGGER ;
 6 QUIET_TIMER_TRIGGER ;
 7 NDP_TIMER_TRIGGER ;
 8-15 GENERIC_TIMER2_TRIGGER ;/
 16 TIMER_OVERFLOW ;<-- which timer overflow ?
 17 DBA_TIMER_THRESHOLD ;\
 18 SBA_TIMER_THRESHOLD ;
 19 HCF_TIMER_THRESHOLD ;
 20 TIM_TIMER_THRESHOLD ; threshold's
 21 DTIM_TIMER_THRESHOLD ;
 22 QUIET_TIMER_THRESHOLD ;
 23 NDP_TIMER_THRESHOLD ;
 24-31 GENERIC_TIMER2_THRESHOLD ;/ |
| --- |

**ATH:02009Ch - MAC_DMA_ISR_S6 - hw6 only ;Interrupt 6 Stat UNKNOWN(?)**

**ATH:0200BCh - MAC_DMA_IMR_S6 - hw6 only ;Interrupt 6 Mask UNKNOWN(?)**

**ATH:0200CCh (hw6:really?) - MAC_DMA_ISR_S6_S (R) shadow - hw6 only**

```

| 0-31 ?? (probably related to the new "hw6" registers in MAC DMA chapter) |
| --- |

Note: According to hw6.0 source code, "S6_S" has been accidently inserted
between "S1_S" and "S2_S" (thus smashing the old hw4-style port numbering for
"S2_S thru S5_S") (not checked if that's a source code bug, or if it's actually
implemented as so in real hardware).

The hw6.0 source code leaves ALL primary and secondary IRQ bits undocumented,
thw above descriptions are based on hw4 (but hw6 might have some added/changed
bits here and there).

| DSi Atheros Wifi - Internal I/O - 020800h - WMAC QCU Queue (hw4/hw6) |
| --- |

**ATH:020800h..020824h - MAC_QCU_TXDP[0..9] aka MAC_DMA_Q(0..9)_TXDP**

```

| 0-31 DATA ... unspecified ;MAC Transmit Queue descriptor pointer |
| --- |

**ATH:020840h - MAC_QCU_TXE aka MAC_DMA_Q_TXE - MAC Transmit Queue enable (R)**

**ATH:020880h - MAC_QCU_TXD aka MAC_DMA_Q_TXD - MAC Transmit Queue disable**

```

| 0-9 DATA |
| --- |

**ATH:0208C0h..0208E4h - MAC_QCU_CBR[0..9] aka MAC_DMA_Q(0..9)_CBRCFG**

```

| 0-23 CBR interval (us) (INTERVAL) ;\MAC CBR configuration
 24-31 CBR overflow threshold (OVF_THRESH) ;/ |
| --- |

**ATH:020900h..020924h - MAC_QCU_RDYTIME[0..9] aka MAC_DMA_Q(0..9)_RDYTIMECFG**

```

| 0-23 CBR interval (us) (DURATION) ;\MAC ReadyTime configuration
 24 CBR enable (EN) ;/ |
| --- |

**ATH:020940h - MAC_QCU_ONESHOT_ARM_SC aka MAC_DMA_Q_ONESHOTMAC_DMAM_SC - Set**

**ATH:020980h - MAC_QCU_ONESHOT_ARM_CC aka MAC_DMA_Q_ONESHOTMAC_DMAM_CC - Clr**

MAC OneShotArm set/clear control

```

| 0-9 SET/CLEAR |
| --- |

**ATH:0209C0h..0209E4h - MAC_QCU_MISC[0..9] aka MAC_DMA_Q(0..9)_MISC**

MAC Miscellaneous QCU settings

```

| 0-3 Frame Scheduling Policy mask (FSP):
 0=ASAP ;\
 1=CBR ; defined as so for
 2=DMA Beacon Alert gated ; hw4 (maybe same
 3=TIM gated ; for hw6)
 4=Beacon-sent-gated ;/
 4 OneShot enable (ONESHOT_EN)
 5 CBR expired counter disable incr (NOFR, empty q)
 6 CBR expired counter disable incr (NOBCNFR, empty beacon q)
 7 Beacon use indication (IS_BCN)
 8 CBR expired counter limit enable (CBR_EXP_INC_LIMIT)
 9 Enable TXE cleared on ReadyTime expired or VEOL (TXE_CLR_ON_CBR_END)
 10 CBR expired counter reset (MMR_CBR_EXP_CNT_CLR_EN)
 11 FR_ABORT_REQ_EN DCU frame early termination request control |
| --- |

**ATH:020A00h..020A24h - MAC_QCU_CNT[0..9] aka MAC_DMA_Q(0..9)_STS**

```

| 0-1 FR_PEND: Pending Frame Count (R) ;\MAC Misc QCU status/counter
 8-15 CBR_EXP: CBR expired counter (R) ;/ |
| --- |

**ATH:020A40h - MAC_QCU_RDYTIME_SHDN aka MAC_DMA_Q_RDYTIMESHDN**

```

| 0-9 SHUTDOWN: MAC ReadyTimeShutdown status (flags for QCU 0-9 ?) |
| --- |

```

| _____________ below in hw6 only _____________ |
| --- |

**ATH:020830h - MAC_QCU_STATUS_RING_START ;hw6 only**

```

| 0-31 ADDR |
| --- |

**ATH:020834h - MAC_QCU_STATUS_RING_END ;hw6 only**

```

| 0-31 ADDR |
| --- |

**ATH:020838h - MAC_QCU_STATUS_RING_CURRENT (R) ;hw6 only**

```

| 0-31 ADDRESS (R) |
| --- |

**ATH:020A44h - MAC_QCU_DESC_CRC_CHK ;hw6 only**

```

| 0 EN |
| --- |

**ATH:020A48h - MAC_QCU_EOL ;hw6 only**

```

| 0-9 DUR_CAL_EN |
| --- |

| DSi Atheros Wifi - Internal I/O - 021000h - WMAC DCU (hw4/hw6) |
| --- |

**ATH:021000h..021024h - MAC_DCU_QCUMASK[0..9] aka MAC_DMA_D(0..9)_QCUMASK**

The "DCU_QCU" Mask is some two-dimensional array: An array of ten DCU
registers, each containing an array of ten QCU bits.

```

| 0-9 QCU Mask (QCU 0-9) |
| --- |

**ATH:021030h - MAC_DCU_GBL_IFS_SIFS aka MAC_DMA_D_GBL_IFS_SIFS**

```

| 0-15 DURATION ;-DCU global SIFS settings |
| --- |

**ATH:021040h..021064h - MAC_DCU_LCL_IFS[0..9] aka MAC_DMA_D(0..9)_LCL_IFS**

```

| 0-9 CW_MIN ;\
 10-19 CW_MAX ; MAC DCU-specific IFS settings
 20-27 AIFS ;
 28 hw6: LONG_AIFS ;-hw6 only ;
 29-31 - ;/ |
| --- |

Note: Even though this field is 8 bits wide the maximum supported AIFS value is
FCh. Setting the AIFS value to FDh,FEh,FFh will not work correctly and will
cause the DCU to hang (said to be so; at least for hw4).

**ATH:021070h - MAC_DCU_GBL_IFS_SLOT aka MAC_DMA_D_GBL_IFS_SLOT**

```

| 0-15 DURATION ;DC global slot interval |
| --- |

**ATH:021080h..0210A4h - MAC_DCU_RETRY_LIMIT[0..9] aka MAC_DMA_D(0..9)_RETR..**

```

| 0-3 frame RTS failure limit (FRFL) ;\
 4-7 - ; MAC Retry limits
 8-13 station RTS failure limit (SRFL) ;
 14-19 station short retry limit (SDFL) ;
 20-31 - ;/ |
| --- |

**ATH:0210B0h - MAC_DCU_GBL_IFS_EIFS aka MAC_DMA_D_GBL_IFS_EIFS**

```

| 0-15 DURATION ;-DCU global EIFS setting |
| --- |

**ATH:0210C0h..0210E4h - MAC_DCU_CHANNEL_TIME[0..9] aka MAC_DMA_D(0..9)_CH..**

```

| 0-15 ChannelTime duration (us) (DURATION) ;\MAC ChannelTime settings
 16 ChannelTime enable (ENABLE) ;/ |
| --- |

**ATH:0210F0h - MAC_DCU_GBL_IFS_MISC aka MAC_DMA_D_GBL_IFS_MISC**

```

| 0-2 LFSR slice select (LFSR_SLICE_SEL) ;\
 3 Turbo mode indication (TURBO_MODE) ;
 4-9 hw6: SIFS_DUR_USEC ;-hw6 only ;
 10-19 - ;
 20-21 DCU arbiter delay (ARB_DLY) ; DCU global misc.
 22 hw6: SIFS_RST_UNCOND ;\ ; IFS settings
 23 hw6: AIFS_RST_UNCOND ; ;
 24 hw6: LFSR_SLICE_RANDOM_DIS ; hw6 only ;
 25-26 hw6: CHAN_SLOT_WIN_DUR ; ;
 27 hw6: CHAN_SLOT_ALWAYS ;/ ;
 28 IGNORE_BACKOFF ;
 29 hw6: SLOT_COUNT_RST_UNCOND ;-hw6 only ;
 30-31 - ;/ |
| --- |

**ATH:021100h..021124h - MAC_DCU_MISC[0..9] aka MAC_DMA_D(0..9)_MISC**

```

| 0-5 BKOFF_THRESH ;\
 6 SFC_RST_AT_TS_END_EN ;
 7 CW_RST_AT_TS_END_DIS ;
 8 FRAG_BURST_WAIT_QCU_EN ;
 9 FRAG_BURST_BKOFF_EN ; MAC Miscellaneous
 10 - ; DCU-specific settings
 11 HCF_POLL_EN ;
 12 BKOFF_PF ; (specified as so for hw6)
 13 - ; (hw4 bit numbers are undocumented,
 14-15 VIRT_COLL_POLICY ; although... actually the SAME bits
 16 IS_BCN ; ARE documented, but for the "EOL"
 17 ARB_LOCKOUT_IF_EN ; registers instead of for "MISC"...?)
 18 LOCKOUT_GBL_EN ;
 19 LOCKOUT_IGNORE ;
 20 SEQNUM_FREEZE ;
 21 POST_BKOFF_SKIP ;
 22 VIRT_COLL_CW_INC_EN ;
 23 RETRY_ON_BLOWN_IFS_EN ;
 24 SIFS_BURST_CHAN_BUSY_IGNORE ;
 25-31 - ;/ |
| --- |

**ATH:021140h - MAC_DCU_SEQ aka MAC_DMA_D_SEQNUM - MAC Frame sequence number**

```

| 0-31 NUM |
| --- |

BUG: The original hw4 header file used sorted addresses, but it's been having
00021140h placed between 000211A4h and 00021230h... either it's just
mis-sorted, or the address is mis-spelled? Probably just mis-sorted, because
hw6 does confirm the address.

**ATH:021270h - MAC_DCU_PAUSE aka MAC_DMA_D_TXPSE**

```

| 0-9 REQUEST ;\DCU transmit pause control/status
 16 STATUS (R) ;/ |
| --- |

```

| _____________ below in hw4 only _____________ |
| --- |

**ATH:021230h - MAC_DMA_D_FPCTL - DCU frame prefetch settings - hw4 only?**

```

| ... unspecified |
| --- |

**ATH:021180h..021xx4h - MAC_DMA_D(0..9)_EOL - hw4 only (removed in hw6)**

;BUG: entry D(8..9) are officially at 21200h..21204h ;\which is implemented

;BUG: entry D(8..9) are *intended* at 211A0h..211A4h ;/in actual hardware?

;BUG: below bits are probably meant to be "MISC" (not "EOL") ?

```

| 0-5 Backoff threshold
 6 End of transmission series station RTS/data failure count reset policy
 7 End of transmission series CW reset policy
 8 Fragment Starvation Policy
 9 Backoff during a frag burst
 10 -
 11 HFC poll enable
 12 Backoff persistence factor setting
 13
 14-15 Mask for Virtual collision handling policy
 (0=Normal, 1=Ignore, 2..3=Unspecified)
 16 Beacon use indication
 17-18 Mask for DCU arbiter lockout control
 (0=No Lockout, 1=Intra-frame, 2=Global, 3=Unspecified)
 19 DCU arbiter lockout ignore control
 20 Sequence number increment disable
 21 Post-frame backoff disable
 22 Virtual coll. handling policy
 23 Blown IFS handling policy
 24-31 - |
| --- |

BUG: The official source code assigns incorrect "BCD-style" values to
MAC_DMA_D8_EOL and MAC_DMA_D9_EOL, the source code does also have a
MAC_DMA_DEOL_ADDRESS(i) macro function; that function would work okay even with
i=8..9.

```

| _____________ below in hw6 only _____________ |
| --- |

**ATH:0212B0h - MAC_DCU_WOW_KACFG - hw6 only**

```

| 0 TX_EN
 1 TIM_EN
 4-11 BCN_CNT
 12-23 RX_TIMEOUT_CNT |
| --- |

**ATH:0212F0h - MAC_DCU_TXSLOT - hw6 only**

```

| 0-15 MASK |
| --- |

**ATH:02143Ch - MAC_DCU_TXFILTER_CLEAR - hw6 only**

```

| 0-31 DATA |
| --- |

**ATH:02147Ch - MAC_DCU_TXFILTER_SET - hw6 only**

```

| 0-31 DATA |
| --- |

**ATH:021038h - MAC_DCU_TXFILTER_DCU0_31_0 - hw6 only**

**ATH:021078h - MAC_DCU_TXFILTER_DCU0_63_32 - hw6 only**

**ATH:0210B8h - MAC_DCU_TXFILTER_DCU0_95_64 - hw6 only**

**ATH:0210F8h - MAC_DCU_TXFILTER_DCU0_127_96 - hw6 only**

```

| 0-31 DATA |
| --- |

**ATH:02103Ch - MAC_DCU_TXFILTER_DCU8_31_0 - hw6 only (R)**

**ATH:02107Ch - MAC_DCU_TXFILTER_DCU8_63_32 - hw6 only (R)**

**ATH:0210BCh - MAC_DCU_TXFILTER_DCU8_95_64 - hw6 only (R)**

**ATH:0210FCh - MAC_DCU_TXFILTER_DCU8_127_96 - hw6 only (R)**

```

| 0-31 DATA (R) |
| --- |

**ATH:021138h - MAC_DCU_TXFILTER_DCU1_31_0 - hw6 only (R)**

**ATH:021178h - MAC_DCU_TXFILTER_DCU1_63_32 - hw6 only (R)**

**ATH:0211B8h - MAC_DCU_TXFILTER_DCU1_95_64 - hw6 only (R)**

**ATH:0211F8h - MAC_DCU_TXFILTER_DCU1_127_96 - hw6 only (R)**

```

| 0-31 DATA (R) |
| --- |

**ATH:02113Ch - MAC_DCU_TXFILTER_DCU9_31_0 - hw6 only (R)**

**ATH:02117Ch - MAC_DCU_TXFILTER_DCU9_63_32 - hw6 only (R)**

**ATH:0211BCh - MAC_DCU_TXFILTER_DCU9_95_64 - hw6 only (R)**

**ATH:0211FCh - MAC_DCU_TXFILTER_DCU9_127_96 - hw6 only (R)**

```

| 0-31 DATA (R) |
| --- |

**ATH:021238h - MAC_DCU_TXFILTER_DCU2_31_0 - hw6 only (R)**

**ATH:021278h - MAC_DCU_TXFILTER_DCU2_63_32 - hw6 only (R)**

**ATH:0212B8h - MAC_DCU_TXFILTER_DCU2_95_64 - hw6 only (R)**

**ATH:0212F8h - MAC_DCU_TXFILTER_DCU2_127_96 - hw6 only (R)**

```

| 0-31 DATA (R) |
| --- |

**ATH:021338h - MAC_DCU_TXFILTER_DCU3_31_0 - hw6 only (R)**

**ATH:021378h - MAC_DCU_TXFILTER_DCU3_63_32 - hw6 only (R)**

**ATH:0213B8h - MAC_DCU_TXFILTER_DCU3_95_64 - hw6 only (R)**

**ATH:0213F8h - MAC_DCU_TXFILTER_DCU3_127_96 - hw6 only (R)**

```

| 0-31 DATA (R) |
| --- |

**ATH:021438h - MAC_DCU_TXFILTER_DCU4_31_0 - hw6 only (R)**

**ATH:021478h - MAC_DCU_TXFILTER_DCU4_63_32 - hw6 only (R)**

**ATH:0214B8h - MAC_DCU_TXFILTER_DCU4_95_64 - hw6 only (R)**

**ATH:0214F8h - MAC_DCU_TXFILTER_DCU4_127_96 - hw6 only (R)**

```

| 0-31 DATA (R) |
| --- |

**ATH:021538h - MAC_DCU_TXFILTER_DCU5_31_0 - hw6 only (R)**

**ATH:021578h - MAC_DCU_TXFILTER_DCU5_63_32 - hw6 only (R)**

**ATH:0215B8h - MAC_DCU_TXFILTER_DCU5_95_64 - hw6 only (R)**

**ATH:0215F8h - MAC_DCU_TXFILTER_DCU5_127_96 - hw6 only (R)**

```

| 0-31 DATA (R) |
| --- |

**ATH:021638h - MAC_DCU_TXFILTER_DCU6_31_0 - hw6 only (R)**

**ATH:021678h - MAC_DCU_TXFILTER_DCU6_63_32 - hw6 only (R)**

**ATH:0216B8h - MAC_DCU_TXFILTER_DCU6_95_64 - hw6 only (R)**

**ATH:0216F8h - MAC_DCU_TXFILTER_DCU6_127_96 - hw6 only (R)**

```

| 0-31 DATA (R) |
| --- |

**ATH:021738h - MAC_DCU_TXFILTER_DCU7_31_0 - hw6 only (R)**

**ATH:021778h - MAC_DCU_TXFILTER_DCU7_63_32 - hw6 only (R)**

**ATH:0217B8h - MAC_DCU_TXFILTER_DCU7_95_64 - hw6 only (R)**

**ATH:0217F8h - MAC_DCU_TXFILTER_DCU7_127_96 - hw6 only (R)**

```

| 0-31 DATA (R) |
| --- |

| DSi Atheros Wifi - Internal I/O - 028000h - WMAC PCU (hw2/hw4/hw6) |
| --- |

**ATH:028000h - MAC_PCU_STA_ADDR_L32 (aka hw2: REG_STA_ID0) ;hw2/hw4/hw6**

**ATH:028004h - MAC_PCU_STA_ADDR_U16 (aka hw2: REG_STA_ID1) ;hw2/hw4/hw6**

```

| 0-47 ADDR (called STA_ADDR in hw2) (local MAC address)
 48 STA_AP (called AP in hw2)
 49 ADHOC
 50 PW_SAVE (called PWR_SV in hw2)
 51 KEYSRCH_DIS (called NO_KEYSRCH in hw2)
 52 PCF
 53 USE_DEFANT (called USE_DEF_ANT in hw2)
 54 DEFANT_UPDATE (called DEF_ANT_UPDATE in hw2)
 55 RTS_USE_DEF (called RTS_DEF_ANT in hw2)
 56 ACKCTS_6MB
 57 BASE_RATE_11B (called RATE_11B in hw2)
 58 SECTOR_SELF_GEN
 59 CRPT_MIC_ENABLE
 60 KSRCH_MODE
 61 PRESERVE_SEQNUM
 62 CBCIV_ENDIAN
 63 ADHOC_MCAST_SEARCH |
| --- |

**ATH:028008h - MAC_PCU_BSSID_L32 (aka hw2: REG_BSS_ID0) ;hw2/hw4/hw6**

**ATH:02800Ch - MAC_PCU_BSSID_U16 (aka hw2: REG_BSS_ID1) ;hw2/hw4/hw6**

```

| 0-47 ADDR (called BSSID in hw2)
 48-58 AID (11bit, although claimed to be 16bit wide, bit48-63 in hw2) |
| --- |

**ATH:0282B0h/02839Ch/028398h - MAC_PCU_BSSID2_L32 ;hw2/hw4/hw6**

**ATH:0282B4h/0283A0h/02839Ch - MAC_PCU_BSSID2_U16 ;hw2/hw4/hw6**

```

| 0-47 ADDR (hw2: SEC_BSSID, ini:0) ;\hw2/hw4/hw6
 48 ENABLE (hw2: SEC_BSSID_ENABLE, ini:0) ;/
 49-51 -
 52-62 hw6: AID ;-hw 6 only
 63 - |
| --- |

Observe that hw2/hw6 port addresses are different here (unlike as usually).

**ATH:028010h - MAC_PCU_BCN_RSSI_AVE (aka hw2:BCNRSSI) (R) ;hw2/hw4/hw6**

```

| 0-11 AVE_VALUE (aka hw2:BCN_RSSI_AVE ini:800h) (R) ;-hw2/hw4/hw6
 16-27 hw6: AVE_VALUE2 (R) ;-hw6 only |
| --- |

**ATH:028014h - MAC_PCU_ACK_CTS_TIMEOUT (aka hw2:REG_TIME_OUT) ;hw2/hw4/hw6**

```

| 0-13 ACK_TIMEOUT (aka 16bit wide, bit0-15: ACK_TIME_OUT in hw2)
 16-29 CTS_TIMEOUT (aka 16bit wide, bit16-31: CTS_TIME_OUT in hw2) |
| --- |

**ATH:028018h - MAC_PCU_BCN_RSSI_CTL (aka hw2:BCNSIG) ;hw2/hw4/hw6**

```

| 0-7 RSSI_LOW_THRESH (aka hw2: BCN_RSSI_LO_THR, ini:0)
 8-15 MISS_THRESH (aka hw2: BCN_MISS_THR, ini:FFh)
 16-23 RSSI_HIGH_THRESH (aka hw2: BCN_RSSI_HI_THR, ini:7Fh)
 24-28 WEIGHT (aka hw2: BCN_RSSI_WEIGHT, ini:0)
 29 RESET (aka hw2: BCN_RSSI_RESET) |
| --- |

**ATH:02801Ch - MAC_PCU_USEC_LATENCY (aka hw2:REG_USEC) ;hw2/hw4/hw6**

```

| <-- hw2 (REG_USEC) --> <--- hw4/hw6 ---------->
 0-6 USEC (7bit) 0-7 USEC (8bit)
 7-13 USEC32 (7bit) 8-13 - (-)
 14-18 TX_DELAY (5bit) 14-22 TX_LATENCY (9bit)
 19-24 RX_DELAY (6bit) 23-28 RX_LATENCY (6bit) |
| --- |

**ATH:02803Ch/028024h/02803Ch - MAC_PCU_RX_FILTER ;hw2/hw4/hw6**

```

| 0 UNICAST
 1 MULTICAST
 2 BROADCAST
 3 CONTROL
 4 BEACON
 5 PROMISCUOUS
 6 XR_POLL
 7 PROBE_REQ
 8 hw2: outcommented: SYNC ;\hw4 and hw6 (outcommented in hw2)
 8 hw4/hw6: SYNC_FRAME ;/
 9 MY_BEACON
 10 hw4/hw6: COMPRESSED_BAR ;\
 11 hw4/hw6: COMPRESSED_BA ;
 12 hw4/hw6: UNCOMPRESSED_BA_BAR ; hw4 and hw6
 13 hw4/hw6: ASSUME_RADAR ;
 14 hw4/hw6: PS_POLL ;
 15 hw4/hw6: MCAST_BCAST_ALL ;
 16 hw4/hw6: RST_DLMTR_CNT_DISABLE ;/
 17 hw4: FROM_TO_DS ;\
 18-23 hw4: GENERIC_FTYPE ; hw4 only (moved to bit20-28 in hw6)
 24-25 hw4: GENERIC_FILTER ;/
 17 hw6: HW_BCN_PROC_ENABLE ;\
 18 hw6: MGMT_ACTION_MCAST ; hw6 only
 19 hw6: CONTROL_WRAPPER ;
 20 hw6: FROM_TO_DS ; ;\these bits were formerly
 21-26 hw6: GENERIC_FTYPE ; ; in bit17-25 in hw4)
 27-28 hw6: GENERIC_FILTER ; ;/
 29 hw6: MY_BEACON2 ;/ |
| --- |

**ATH:028040h/028028h/028040h - MAC_PCU_MCAST_FILTER_L32 ;hw2/hw4/hw6**

**ATH:028044h/02802Ch/028044h - MAC_PCU_MCAST_FILTER_U32 ;hw2/hw4/hw6**

```

| 0-63 VALUE (aka hw2: unspecified) |
| --- |

**ATH:028048h/028030h/028048h - MAC_PCU_DIAG_SW ;hw2/hw4/hw6**

```

| 0 INVALID_KEY_NO_ACK (aka hw2:DIS_WEP_ACK ;ini:0)
 1 NO_ACK (aka hw2:DIS_ACK ;ini:0)
 2 NO_CTS (aka hw2:DIS_CTS ;ini:0)
 3 NO_ENCRYPT (aka hw2:DIS_ENC ;ini:0)
 4 NO_DECRYPT (aka hw2:DIS_DEC ;ini:0)
 5 HALT_RX (aka hw2:DIS_RX ;ini:0)
 6 LOOP_BACK (aka hw2:LOOP_BACK ;ini:0)
 7 CORRUPT_FCS (aka hw2:CORR_FCS ;ini:0)
 8 DUMP_CHAN_INFO (aka hw2:CHAN_INFO ;ini:0)
 9-16 - (aka hw2: RESERVED) (aka hw2:RESERVED ;ini:0)
 17 ACCEPT_NON_V0 (aka hw2:ACCEPT_NONV0 ;ini:0)
 18-19 OBS_SEL_1_0 (aka hw2:OBS_SEL_0_1 ;ini:0)
 20 RX_CLEAR_HIGH (aka hw2:RXCLR_HIGH ;ini:0)
 21 IGNORE_NAV (aka hw2:IGNORE_NAV ;ini:0)
 22 CHAN_IDLE_HIGH (aka hw2:CHANIDLE_HIGH ;ini:0)
 23 PHYERR_ENABLE_EIFS_CTL (aka hw2:PHYERR_ENABLE_NEW ;ini:0)
 24 DUAL_CHAIN_CHAN_INFO (aka hw2:DUAL_CHAIN_CHAN_INFO ;ini:0)
 25 FORCE_RX_ABORT (aka hw2:FORCE_RX_ABORT ;ini:0)
 26 SATURATE_CYCLE_CNT (aka hw2:SATURATE_CYCLE_CNT ;ini:0)
 27 OBS_SEL_2 (aka hw2:OBS_SEL_2 ;ini:0)
 28 hw4/hw6: RX_CLEAR_CTL_LOW ;\
 29 hw4/hw6: RX_CLEAR_EXT_LOW ; hw4/hw6 only
 30-31 hw4/hw6: DEBUG_MODE ;/ |
| --- |

**ATH:028054h/028034h/028054h - MAC_PCU_TST_ADDAC ;hw2/hw4/hw6**

```

| 0 hw2: TEST_MODE ;\
 1 hw2: TEST_LOOP ; hw2 (moved to bit1-14 in hw4/hw6)
 2-12 hw2: LOOP_LEN ;
 13 hw2: TEST_UPPER_8B ;/
 14 hw2: TEST_MSB ;-hw2 only
 15 hw2: TEST_CAPTURE ;-hw2 (moved to bit19 in hw4/hw6) |
| --- |

Differently arranged in hw4/hw6:

```

| 0 hw4/hw6: CONT_TX ;-hw4/hw6 only
 1 hw4/hw6: TESTMODE ;\
 2 hw4/hw6: LOOP ; hw4/hw6 (formerly bit0-13 in hw2)
 3-13 hw4/hw6: LOOP_LEN ;
 14 hw4/hw6: UPPER_8B ;/
 15 hw6: SAMPLE_SIZE_2K ;-hw6 only
 16 hw4/hw6: TRIG_SEL ;\
 17 hw4/hw6: TRIG_POLARITY ; hw4/hw6 only
 18 hw4/hw6: CONT_TEST (R) ;/
 19 hw4/hw6: TEST_CAPTURE ;-hw4/hw6 (formerly bit15 in hw2)
 20 hw4/hw6: TEST_ARM ;-hw4/hw6 only |
| --- |

**ATH:028058h/028038h/028058h - MAC_PCU_DEF_ANTENNA ;hw2/hw4/hw6**

```

| 0-23 VALUE ;\hw4/hw6 (and maybe hw2, too)
 24 TX_DEF_ANT_SEL ;/
 25 hw6: SLOW_TX_ANT_EN ;\
 26 hw6: TX_CUR_ANT ; hw6 only
 27 hw6: FAST_DEF_ANT ;/
 28 RX_LNA_CONFIG_SEL ;-hw4/hw6 (and maybe hw2, too)
 29 hw6: FAST_TX_ANT_EN ;\
 30 hw6: RX_ANT_EN ; hw6 only
 31 hw6: RX_ANT_DIV_ON ;/ |
| --- |

**ATH:02805Ch/02803Ch/02805Ch - MAC_PCU_AES_MUTE_MASK_0 ;hw2/hw4/hw6**

```

| 0-15 FC (hw2: ini:C7FFh)
 16-31 QOS (hw2: ini:FFFFh) |
| --- |

**ATH:028060h/028040h/028060h - MAC_PCU_AES_MUTE_MASK_1 ;hw2/hw4/hw6**

```

| 0-15 SEQ (hw2: ini:000Fh)
 16-31 FC_MGMT (hw2: ini:E7FFh) |
| --- |

**ATH:028064h/028044h/028064h - MAC_PCU_GATED_CLKS ;hw2/hw4/hw6**

```

| 0 - (aka hw2: outcommented: SYNC, ini:1)
 1 GATED_TX (aka hw2: TX, ini:0)
 2 GATED_RX (aka hw2: RX, ini:0)
 3 GATED_REG (aka hw2: REG, ini:0) |
| --- |

**ATH:028068h/028048h/028068h - MAC_PCU_OBS_BUS_2 (R) ;hw2/hw4/hw6**

```

| 0-17 VALUE (aka hw2: OBS_BUS) (R) ;-hw2/hw4/hw6
 18-21 hw6: WCF_STATE (R) ;\
 22 hw6: WCF0_FULL (R) ;
 23 hw6: WCF1_FULL (R) ; hw6 only
 24-28 hw6: WCF_COUNT (R) ;
 29 hw6: MACBB_ALL_AWAKE (R) ;/ |
| --- |

**ATH:02806Ch/02804Ch/02806Ch - MAC_PCU_OBS_BUS_1 (R) ;hw2/hw4/hw6**

```

| 0 PCU_DIRECTED (R)
 1 PCU_RX_END (R)
 2 RX_WEP (R)
 3 RX_MY_BEACON (R)
 4 FILTER_PASS (R)
 5 TX_HCF (R)
 6 TM_QUIET_TIME (aka hw2: QUIET_TIME) (R)
 7 PCU_CHANNEL_IDLE (aka hw2: CHAN_IDLE) (R)
 8 TX_HOLD (R)
 9 TX_FRAME (R)
 10 RX_FRAME (R)
 11 RX_CLEAR (R)
 12-17 WEP_STATE (R)
 20-23 hw2: RX_STATE (4bit) (R) ;\hw2 (less bits)
 24-28 hw2: TX_STATE (5bit) (R) ;/
 20-24 hw4/hw6: RX_STATE (5bit) (R) ;\hw4/hw6 (one more
 25-30 hw4/hw6: TX_STATE (6bit) (R) ;/bit than hw2 each) |
| --- |

**ATH:028080h/028054h/028080h - MAC_PCU_LAST_BEACON_TSF (R) ;hw2/hw4/hw6**

**ATH:028664h - MAC_PCU_LAST_BEACON2_TSF ;extra beacon (R) ;hw6 only**

```

| 0-31 VALUE (hw2: unspecified/LAST_TSTP) (R) |
| --- |

**ATH:028084h/028058h/028084h - MAC_PCU_NAV ;hw2/hw4/hw6**

```

| 0-25 VALUE (hw2: unspecified/NAV) |
| --- |

**ATH:028088h/02805Ch/028088h - MAC_PCU_RTS_SUCCESS_CNT (R) ;hw2/hw4/hw6**

**ATH:02808Ch/028060h/02808Ch - MAC_PCU_RTS_FAIL_CNT (R) ;hw2/hw4/hw6**

**ATH:028090h/028064h/028090h - MAC_PCU_ACK_FAIL_CNT (R) ;hw2/hw4/hw6**

**ATH:028094h/028068h/028094h - MAC_PCU_FCS_FAIL_CNT (R) ;hw2/hw4/hw6**

**ATH:028098h/02806Ch/028098h - MAC_PCU_BEACON_CNT (R) ;hw2/hw4/hw6**

**ATH:02809Ch - MAC_PCU_BEACON2_CNT (R) ;hw6 only**

```

| 0-15 VALUE (COUNT or so?) (hw2: unspecified) (R) |
| --- |

**ATH:0280C0h/028070h/0280C0h - MAC_PCU_XRMODE ;hw2/hw4/hw6**

```

| 0-5 POLL_TYPE (hw2: ini:1Ah)
 7 WAIT_FOR_POLL (hw2: ini:0)
 20-31 FRAME_HOLD (hw2: ini:680 decimal) |
| --- |

**ATH:0280C4h/028074h/0280C4h - MAC_PCU_XRDEL ;hw2/hw4/hw6**

```

| 0-15 SLOT_DELAY (hw2: ini:360 (decimal)
 16-31 CHIRP_DATA_DELAY (hw2: ini:1680 decimal) |
| --- |

**ATH:0280C8h/028078h/0280C8h - MAC_PCU_XRTO ;hw2/hw4/hw6**

```

| 0-15 CHIRP_TIMEOUT (hw2: ini:7200 decimal)
 16-31 POLL_TIMEOUT (hw2: ini:5000 decimal) |
| --- |

**ATH:0280CCh/02807Ch/0280CCh - MAC_PCU_XRCRP ;hw2/hw4/hw6**

```

| 0 SEND_CHIRP (hw2: ini:0)
 16-31 CHIRP_GAP (hw2: ini:500 decimal) |
| --- |

**ATH:0280D0h/028080h/0280D0h - MAC_PCU_XRSTMP ;hw2/hw4/hw6**

```

| 0 RX_ABORT_RSSI (hw2: ini:0)
 1 RX_ABORT_BSSID (hw2: ini:0)
 2 TX_STOMP_RSSI (hw2: ini:0)
 3 TX_STOMP_BSSID (hw2: ini:0)
 4 TX_STOMP_DATA (hw2: ini:0)
 5 RX_ABORT_DATA (hw2: ini:0)
 8-15 TX_STOMP_RSSI_THRESH (hw2: ini:25h)
 16-23 RX_ABORT_RSSI_THRESH (hw2: ini:25h) |
| --- |

**ATH:0280E0h/028084h/0280E0h - MAC_PCU_ADDR1_MASK_L32 ;hw2/hw4/hw6**

**ATH:0280E4h/028088h/0280E4h - MAC_PCU_ADDR1_MASK_U16 ;hw2/hw4/hw6**

```

| 0-47 VALUE (aka hw2:BSSID_MASK, ini:FFFFFFFFFFFFh) |
| --- |

This should be 48bit (as so in hw2/hw6, though hw4 claims 64bit, bit0-63).

**ATH:0280E8h/02808Ch/0280E8h - MAC_PCU_TPC ;hw2/hw4/hw6**

```

| 0-5 ACK_PWR (hw2: ini:3Fh)
 8-13 CTS_PWR (hw2: ini:3Fh)
 16-21 CHIRP_PWR (hw2: ini:3Fh)
 24-29 hw6: RPT_PWR ;-hw6 only |
| --- |

**ATH:0280ECh/028090h/0280ECh - MAC_PCU_TX_FRAME_CNT ;hw2/hw4/hw6**

**ATH:0280F0h/028094h/0280F0h - MAC_PCU_RX_FRAME_CNT ;hw2/hw4/hw6**

**ATH:0280F4h/028098h/0280F4h - MAC_PCU_RX_CLEAR_CNT ;hw2/hw4/hw6**

**ATH:0280F8h/02809Ch/0280F8h - MAC_PCU_CYCLE_CNT ;hw2/hw4/hw6**

```

| 0-31 VALUE (aka COUNT or so?) (aka hw2: CNT, ini:0) |
| --- |

**ATH:0280FCh/0280A0h/0280FCh - MAC_PCU_QUIET_TIME_1 ;hw2/hw4/hw6**

```

| 0-15 hw2: NEXT_QUIET (hw2: ini:0) ;\hw2 only (not hw4/hw6)
 16 hw2: QUIET_ENABLE (hw2: ini:0) ;/
 17 ACK_CTS_ENABLE (hw2: ini:1) ;-hw2/hw4/hw6 |
| --- |

**ATH:028100h/0280A4h/028100h - MAC_PCU_QUIET_TIME_2 ;hw2/hw4/hw6**

```

| 0-15 hw2: QUIET_PERIOD (hw2: ini:0002h) ;\differs in
 0-15 hw4: - ; hw2, hw4, hw6
 0-15 hw6: DURATION2 ;-hw6 only ;/
 16-31 DURATION (aka hw2: QUIET_DURATION, ini:0001h) ;-hw2/hw/hw6 |
| --- |

**ATH:028108h/0280A8h/028108h - MAC_PCU_QOS_NO_ACK ;hw2/hw4/hw6**

```

| 0-3 TWO_BIT_VALUES (hw2: NOACK_2_BIT_VALUES, ini:2)
 4-6 BIT_OFFSET (hw2: NOACK_BIT_OFFSET, ini:5)
 7-8 BYTE_OFFSET (hw2: NOACK_BYTE_OFFSET, ini:0) |
| --- |

**ATH:02810Ch/0280ACh/02810Ch - MAC_PCU_PHY_ERROR_MASK ;hw2/hw4/hw6**

```

| 0-31 VALUE (hw2: PHYERR_MASK, ini:0) |
| --- |

**ATH:028110h/0280B0h/028110h - MAC_PCU_XRLAT ;hw2/hw4/hw6**

```

| 0-11 VALUE (hw2: XR_TX_DELAY, ini:168h) |
| --- |

**ATH:0280B4h/028114h - MAC_PCU_RXBUF ;hw4/hw6 (not hw2)**

```

| 0-10 HIGH_PRIORITY_THRSHD ;\hw4/hw6 only (not hw2)
 11 REG_RD_ENABLE ;/ |
| --- |

Note: hw2 does have a "MAC_PCU_REG_TSF" register in this place, which seems to
be something different than the hw4/hw6 stuff (unless the hw4/hw6 "RXBUF" is
related to the hw2 "ACKSIFSMEM" array?).

**ATH:028118h/0280B8h/028118h - MAC_PCU_MIC_QOS_CONTROL ;hw2/hw4/hw6**

```

| 0-15 VALUE_0..7 (2bit each) (aka hw2: MICQOSCTL, ini:00AAh)
 16 ENABLE (aka hw2: MICQOSCTL_ENABLE, ini:1) |
| --- |

**ATH:02811Ch/0280BCh/02811Ch - MAC_PCU_MIC_QOS_SELECT ;hw2/hw4/hw6**

```

| 0-31 VALUE_0..7 (4bit each) (aka hw2: MICQOSSEL, ini:00003210h) |
| --- |

**ATH:028124h/0280C4h/028124h - MAC_PCU_FILTER_OFDM_CNT ;hw2/hw4/hw6**

**ATH:028128h/0280C8h/028128h - MAC_PCU_FILTER_CCK_CNT ;hw2/hw4/hw6**

```

| 0-23 VALUE (count or so?) (hw2: CNT, ini:0) |
| --- |

**ATH:02812Ch/0280CCh/02812Ch - MAC_PCU_PHY_ERR_CNT_1 ;hw2/hw4/hw6**

**ATH:028134h/0280D4h/028134h - MAC_PCU_PHY_ERR_CNT_2 ;hw2/hw4/hw6**

**ATH:028168h/0280E4h/028168h - MAC_PCU_PHY_ERR_CNT_3 ;hw2/hw4/hw6**

```

| 0-23 VALUE (count or so?) (hw2: PHYCNT, ini:0) |
| --- |

**ATH:028130h/0280D0h/028130h - MAC_PCU_PHY_ERR_CNT_1_MASK ;hw2/hw4/hw6**

**ATH:028138h/0280D8h/028138h - MAC_PCU_PHY_ERR_CNT_2_MASK ;hw2/hw4/hw6**

**ATH:02816Ch/0280E8h/02816Ch - MAC_PCU_PHY_ERR_CNT_3_MASK ;hw2/hw4/hw6**

```

| 0-31 VALUE (mask or so?) (hw2: PHYCNTMASK, ini:0) |
| --- |

**ATH:02813Ch/0280DCh/005144h - MAC_PCU_TSF_THRESHOLD ;hw2/hw4/hw6**

```

| 0-15 VALUE (hw2: TSFTHRESH, ini:FFFFh) |
| --- |

Observe that hw2/hw6 port addresses are different here (unlike as usually).

**ATH:028144h/0280E0h/028144h - MAC_PCU_PHY_ERROR_EIFS_MASK ;hw2/hw4/hw6**

```

| 0-31 VALUE (hw2: MASK, ini:0) |
| --- |

```

| ________________________________ Misc Mode ________________________________ |
| --- |

**ATH:028120h/0280C0h/028120h - MAC_PCU_MISC_MODE ;hw2/hw4/hw6**

```

| 0 BSSID_MATCH_FORCE (hw2: ini:0)
 1 hw2: ACKSIFS_MEMORY_RESERVED (hw2: ini:0)
 1 hw4/hw6: DEBUG_MODE_AD
 2 MIC_NEW_LOCATION(_ENABLE) (hw2: ini:0)
 3 TX_ADD_TSF (hw2: ini:0)
 4 CCK_SIFS_MODE (hw2: ini:0)
 5 hw2: BFCOEF_MODE_RESERVED (hw2: ini:0)
 6 hw2: BFCOEF_ENABLE (hw2: ini:0)
 7 hw2: BFCOEF_UPDATE_SELF_GEN (hw2: ini:1)
 8 hw2: BFCOEF_MCAST (hw2: ini:1)
 9 hw2: DUAL_CHAIN_ANT_MODE (hw2: ini:0)
 10 hw2: FALCON_DESC_MODE (hw2: ini:0)
 5-8 hw4: -
 5 hw6: RXSM2SVD_PRE_RST
 6 hw6: RCV_DELAY_SOUNDING_IM_TXBF
 7-8 hw6: -
 9 hw4/hw6: DEBUG_MODE_BA_BITMAP
 10 hw4/hw6: DEBUG_MODE_SIFS
 11 KC_RX_ANT_UPDATE (hw2: ini:1)
 12 TXOP_TBTT_LIMIT(_ENABLE) (hw2: ini:0)
 13 hw2: FALCON_BB_INTERFACE (hw2: ini:0)
 14 MISS_BEACON_IN_SLEEP (hw2: ini:1)
 15-16 -
 17 hw2: BUG_12306_FIX_ENABLE (hw2: ini:1)
 18 FORCE_QUIET_COLLISION (hw2: ini:0)
 19 hw2: BUG_12549_FORCE_TXBF (hw2: ini:0)
 20 BT_ANT_PREVENTS_RX (hw2: ini:1)
 21 TBTT_PROTECT (hw2: ini:1)
 22 HCF_POLL_CANCELS_NAV (hw2: ini:1)
 23 RX_HCF_POLL_ENABLE (hw2: ini:1)
 24 CLEAR_VMF (hw2: ini:0)
 25 CLEAR_FIRST_HCF (hw2: ini:0)
 26 hw2: ADHOC_MCAST_KEYID_ENABLE (hw2: ini:0)
 27 hw2: ALLOW_RAC (hw2: ini:0)
 28-31 hw2: -
 26 hw4/hw6: CLEAR_BA_VALID
 27 hw4/hw6: SEL_EVM
 28 hw4/hw6: ALWAYS_PERFORM_KEY_SEARCH
 29 hw4/hw6: USE_EOP_PTR_FOR_DMA_WR
 30-31 hw4/hw6: DEBUG_MODE |
| --- |

**ATH:02825Ch/028144h/028344h - MAC_PCU_MISC_MODE2 ;hw2/hw4/hw6**

```

| 0 hw2: MGMT_CRYPTO_ENABLE (ini:0) ;moved to bit1 in hw4 ;\
 1 hw2: NO_CRYPTO_FOR_NON_DATA_PKT(ini:0) ;moved to bit2 in hw4 ; hw2
 2-7 hw2: RESERVED ;/
 0 hw4/hw6: BUG_21532_FIX_ENABLE ;\
 1 hw4/hw6: MGMT_CRYPTO_ENABLE ; hw4/hw6
 2 hw4/hw6: NO_CRYPTO_FOR_NON_DATA_PKT ;/
 3 hw4: RESERVED
 3 hw6: BUG_58603_FIX_ENABLE ;-hw6
 4 hw6 and hw4.2: BUG_58057_FIX_ENABLE ;-hw4.2 and up (not hw2 and hw4.0)
 5 hw4/hw6: RESERVED ;\
 6 hw4/hw6: ADHOC_MCAST_KEYID_ENABLE ; hw4/hw6
 7 hw4/hw6: CFP_IGNORE ;/
 8-15 MGMT_QOS ;-all hw
 16 hw2: BC_MC_WAPI_MODE (ini:0) ;moved to bit18 in hw4 ;\
 17 hw2: IGNORE_TXOP_FOR_1ST_PKT (ini:0) ;moved to bit22 in hw4 ; hw2
 18 hw2: IGNORE_TXOP_IF_ZERO (ini:0) ;moved to bit23 in hw4 ;
 19-31 hw2_ RESERVED ;/
 16 hw4/hw6: ENABLE_LOAD_NAV_BEACON_DURATION ;\
 17 hw4/hw6: AGG_WEP ;
 18 hw4/hw6: BC_MC_WAPI_MODE ;
 19 hw4/hw6: DUR_ACCOUNT_BY_BA ; hw4/hw6
 20 hw4/hw6: BUG_28676 ;
 21 hw4/hw6: CLEAR_MORE_FRAG ;
 22 hw4/hw6: IGNORE_TXOP_1ST_PKT ;/
 23 hw4: IGNORE_TXOP_IF_ZERO ;moved to MISC_MODE3.bit22 in hw6 ;\
 24 hw4: PM_FIELD_FOR_DAT ;moved to MISC_MODE3.bit24 in hw6 ;
 25 hw4: PM_FIELD_FOR_MGMT ;moved to MISC_MODE3.bit25 in hw6 ; hw4
 26 hw4: BEACON_FROM_TO_DS ;moved to MISC_MODE3.bit23 in hw6? ;
 27 hw4: RCV_TIMESTAMP_FIX ;moved to bit25 in hw6 ;
 28-31 hw4: RESERVED ;/
 23 hw6: MPDU_DENSITY_STS_FIX ;\
 24 hw6: MPDU_DENSITY_WAIT_WEP ;
 25 hw6: RCV_TIMESTAMP_FIX ;moved from bit27 in hw4 ;
 27 hw6: DECOUPLE_DECRYPTION ; hw6
 28 hw6: H_TO_SW_DEBUG_MODE ;
 29 hw6: TXBF_ACT_RPT_DONE_PASS ;
 30 hw6: PCU_LOOP_TXBF ;
 31 hw6: CLEAR_WEP_TXBUSY_ON_TXURN ;/ |
| --- |

Observe that hw2/hw6 port addresses are different here (unlike as usually).

**ATH:0283D0h - MAC_PCU_MISC_MODE3 ;hw6**

```

| 0 BUG_55702_FIX_ENABLE
 1 AES_3STREAM
 2 REGULAR_SOUNDING
 3 BUG_58011_FIX_ENABLE
 4 BUG_56991_FIX_ENABLE
 5 WOW_ADDR1_MASK_ENABLE
 6 BUG_61936_FIX_ENABLE
 7 CHECK_LENGTH_FOR_BA
 8-15 BA_FRAME_LENGTH
 16 MATCH_TID_FOR_BA
 17 WAPI_ORDER_MASK
 18 BB_LDPC_EN
 19 SELF_GEN_SMOOTHING
 20 SMOOTHING_FORCE
 21 ALLOW_RAC
 22 IGNORE_TXOP_IF_ZER0 ;uh, ZerNull or Zero? ;moved from MODE2.bit23
 23 BEACON_FROM_TO_DS_CHECK ;moved from MODE2.bit26?
 24 PM_FIELD_FOR_DAT ;moved from MODE2.bit24
 25 PM_FIELD_FOR_MGMT ;moved from MODE2.bit25
 26 PM_FIELD2_FOR_CTL
 27 PM_FIELD2_FOR_DAT
 28 PM_FIELD2_FOR_MGT
 29 KEY_MISS_FIX
 30 PER_STA_WEP_ENTRY_ENABLE
 31 TIME_BASED_DISCARD_EN |
| --- |

**ATH:0283D4h - MAC_PCU_MISC_MODE4 ;hw6**

```

| 0 BC_MC_WAPI_MODE2_EN
 1 BC_MC_WAPI_MODE2
 2 SYNC_TSF_ON_BEACON
 3 SYNC_TSF_ON_BCAST_PROBE_RESP
 4 SYNC_TSF_ON_MCAST_PROBE_RESP
 5 SYNC_TSF_ON_UCAST_MOON_PROBE_RESP
 6 SYNC_TSF_ON_UCAST_PROBE_RESP |
| --- |

```

| ______________________________ Basic Rate Set ______________________________ |
| --- |

**ATH:0282A4h - MAC_PCU_BASIC_RATE_SET0 ;hw2 (other as in hw4/hw6)**

**ATH:0282A8h - MAC_PCU_BASIC_RATE_SET1 ;hw2 (other as in hw4/hw6)**

**ATH:0282ACh - MAC_PCU_BASIC_RATE_SET2 ;hw2 (other as in hw4/hw6)**

```

| Bitfields for hw2 RATE_SET0 register:
 0-4 BRATE_1MB_L (hw2: ini:#CCK_RATE_1Mb_L)
 5-9 BRATE_2MB_L (hw2: ini:#CCK_RATE_2Mb_L)
 10-14 BRATE_2MB_S (hw2: ini:#CCK_RATE_2Mb_S)
 15-19 BRATE_5_5MB_L (hw2: ini:#CCK_RATE_5_5Mb_L)
 20-24 BRATE_5_5MB_S (hw2: ini:#CCK_RATE_5_5Mb_S)
 25-29 BRATE_11MB_L (hw2: ini:#CCK_RATE_11Mb_L)
 Bitfields for hw2 RATE_SET1 register:
 0-4 BRATE_11MB_S (hw2: ini:#CCK_RATE_11Mb_S)
 5-9 BRATE_6MB (hw2: ini:#OFDM_RATE_6Mb)
 10-14 BRATE_9MB (hw2: ini:#OFDM_RATE_6Mb, too?)
 15-19 BRATE_12MB (hw2: ini:#OFDM_RATE_12Mb)
 20-24 BRATE_18MB (hw2: ini:#OFDM_RATE_12Mb, too?)
 25-29 BRATE_24MB (hw2: ini:#OFDM_RATE_24Mb)
 Bitfields for hw2 RATE_SET2 register:
 0-4 BRATE_36MB (hw2: ini:#OFDM_RATE_24Mb, too?)
 5-9 BRATE_48MB (hw2: ini:#OFDM_RATE_24Mb, too?)
 10-14 BRATE_54MB (hw2: ini:#OFDM_RATE_24Mb, too?) |
| --- |

Alongsides, hw2 source code defines following "Rate" values:

```

| OFDM_RATE_6Mb = 0Bh CCK_RATE_1Mb_L = 1Bh XR_RATE_0_25Mb = 03h
 OFDM_RATE_9Mb = 0Fh CCK_RATE_2Mb_L = 1Ah XR_RATE_0_5Mb = 07h
 OFDM_RATE_12Mb = 0Ah CCK_RATE_2Mb_S = 1Eh XR_RATE_1Mb = 02h
 OFDM_RATE_18Mb = 0Eh CCK_RATE_5_5Mb_L = 19h XR_RATE_2Mb = 06h
 OFDM_RATE_24Mb = 09h CCK_RATE_5_5Mb_S = 1Dh XR_RATE_3Mb = 01h
 OFDM_RATE_36Mb = 0Dh CCK_RATE_11Mb_L = 18h (the XR_stuff might be
 OFDM_RATE_48Mb = 08h CCK_RATE_11Mb_S = 1Ch unrelated to RATE_SET)
 OFDM_RATE_54Mb = 0Ch |
| --- |

Note: The hw2 RATE_SET registers contain 75bit (30+30+15bit), hw4/hw6 has
similar RATE_SET registers with 100bit (4x25bit), that registers may have
similar functions - but their content is obviously differently arranged.

**ATH:028328h/0283E0h - MAC_PCU_BASIC_RATE_SET0 ;hw4/hw6 (other as in hw2)**

**ATH:02832Ch/0283E4h - MAC_PCU_BASIC_RATE_SET1 ;hw4/hw6 (other as in hw2)**

**ATH:028330h/0283E8h - MAC_PCU_BASIC_RATE_SET2 ;hw4/hw6 (other as in hw2)**

**ATH:028334h/0283ECh - MAC_PCU_BASIC_RATE_SET3 ;hw4/hw6 (new in hw4/hw6)**

```

| 0-24 VALUE (maybe this 25bit value is meant to contain 5 rates of 5bit ?) |
| --- |

Note: The hw4/hw6 RATE_SET registers contain 100bit (4x25bit), hw2 has similar
RATE_SET registers with 75bit (30+30+15bit), that registers may have similar
functions - but their content is obviously differently arranged.

```

| ______________________________ Bluetooth Mode ______________________________ |
| --- |

**ATH:028170h/0280ECh/028170h - MAC_PCU_BLUETOOTH_MODE ;hw2/hw4/hw6**

```

| 0-7 TIME_EXTEND (hw2: ini:20h)
 8 TX_STATE_EXTEND (hw2: ini:1)
 9 TX_FRAME_EXTEND (hw2: ini:1)
 10-11 MODE (hw2: ini:3)
 12 QUIET (hw2: ini:1)
 13-16 QCU_THRESH (hw2: ini:1)
 17 RX_CLEAR_POLARITY (hw2: ini:0)
 18-23 PRIORITY_TIME (hw2: ini:05h)
 24-31 FIRST_SLOT_TIME (hw2: ini:9Bh) |
| --- |

**ATH:02817Ch/0280F4h/02817Ch - MAC_PCU_BLUETOOTH_MODE2 ;hw2/hw4/hw6**

```

| 0-7 BCN_MISS_THRESH (hw2: ini:0)
 8-15 BCN_MISS_CNT (R)
 16 HOLD_RX_CLEAR (hw2: ini:0)
 17 SLEEP_ALLOW_BT_ACCESS (hw2: WL_CONTROL_ANT, ini:0)
 18 hw2: RESPOND_TO_BT_ACTIVE (hw2: ini:0) ;-hw2 only
 19 PROTECT_BT_AFTER_WAKEUP (hw2: ini:0)
 20 DISABLE_BT_ANT (hw2: ini:0)
 21 hw4/hw6: QUIET_2_WIRE ;\
 22-23 hw4/hw6: WL_ACTIVE_MODE ;
 24 hw4/hw6: WL_TXRX_SEPARATE ;
 25 hw4/hw6: RS_DISCARD_EXTEND ; hw4/hw6 only
 26-27 hw4/hw6: TSF_BT_ACTIVE_CTRL ;
 28-29 hw4/hw6: TSF_BT_PRIORITY_CTRL ;
 30 hw4/hw6: INTERRUPT_ENABLE ;
 31 hw4/hw6: PHY_ERR_BT_COLL_ENABLE ;/ |
| --- |

**ATH:028164h/0281D4h - MAC_PCU_BLUETOOTH_MODE3 ;hw4/hw6 (not hw2)**

```

| 0-7 WL_ACTIVE_TIME ;\
 8-15 WL_QC_TIME ;
 16-19 ALLOW_CONCURRENT_ACCESS ;
 20 hw4: SHARED_RX ;<-- hw4 ;
 20 hw6: AGC_SATURATION_CNT_ENABLE ;<-- hw6 ; hw4/hw6 only (not hw2)
 21 WL_PRIORITY_OFFSET_EN ;
 22 RFGAIN_LOCK_SRC ;
 23 DYNAMIC_PRI_EN ;
 24 DYNAMIC_TOGGLE_WLA_EN ;
 25-26 SLOT_SLOP ;
 27 BT_TX_ON_EN ;
 28-31 BT_PRIORITY_EXTEND_THRES ;/ |
| --- |

**ATH:028168h/0281D8h - MAC_PCU_BLUETOOTH_MODE4 ;hw4/hw6 (not hw2)**

```

| 0-15 BT_ACTIVE_EXTEND ;\hw4/hw6 only (not hw2)
 16-31 BT_PRIORITY_EXTEND ;/ |
| --- |

**ATH:0281DCh - MAC_PCU_BLUETOOTH_MODE5 ;hw6 (not hw2/hw4)**

```

| 0-2 MCI_WL_LEVEL_MULT ;\
 3 TX_ON_SRC ;
 4-19 TIMER_TARGET ; hw6 only (not hw2/hw4)
 20 SHARED_RX ;
 21 USE_BTP_EXT ;/ |
| --- |

**ATH:028174h/0280F0h/0281E0h - MAC_PCU_BLUETOOTH_WEIGHTS ;hw2/hw4/hw6**

```

| 0-15 BT_WEIGHT (hw2: ini:FA50h)
 16-31 WL_WEIGHT (hw2: ini:FAA4h) |
| --- |

Observe that hw2/hw6 port addresses are different here (unlike as usually).

For hw4 only: There's also a "WL_WEIGHT_CONTD" in a "WEIGHTS" register.

**ATH:028158h - MAC_PCU_BLUETOOTH_WEIGHTS2 ;hw4 only (not hw2/hw6)**

```

| 16-31 WL_WEIGHT_CONTD (extends "WL_WEIGHT" or so) ;-hw4 only (not hw2/hw6) |
| --- |

```

| _______________________________ hw2/hw6 only _______________________________ |
| --- |

**ATH:028178h/028154h/028178h - MAC_PCU_HCF_TIMEOUT ;hw2/hw6 (not hw4)**

```

| 0-15 VALUE (hw2: TIMEOUT, ini:100h) ;-hw2/hw6 only (not hw4) |
| --- |

**ATH:0281D0h/0280F8h/0281D0h - MAC_PCU_TXSIFS ;hw2/hw6 (not hw4)**

```

| 0-7 SIFS_TIME (hw2: ini: 16 decimal)
 8-11 TX_LATENCY (hw2: ini:2)
 12-14 ACK_SHIFT (hw2: ini:3) |
| --- |

**ATH:0281ECh/0280FCh/0281ECh - MAC_PCU_TXOP_X ;hw2/hw6 (not hw4)**

```

| 0-7 VALUE (hw2: TXOP_X, ini:0) |
| --- |

**ATH:0281F0h/028100h/0281F0h - MAC_PCU_TXOP_0_3 ;hw2/hw6 (not hw4)**

**ATH:0281F4h/028104h/0281F4h - MAC_PCU_TXOP_4_7 ;hw2/hw6 (not hw4)**

**ATH:0281F8h/028108h/0281F8h - MAC_PCU_TXOP_8_11 ;hw2/hw6 (not hw4)**

**ATH:0281FCh/02810Ch/0281FCh - MAC_PCU_TXOP_12_15 ;hw2/hw6 (not hw4)**

```

| 0-7 TXOP_0 / TXOP_4 / TXOP_8 / TXOP_12 (hw2: ini:0)
 8-15 TXOP_1 / TXOP_5 / TXOP_9 / TXOP_13 (hw2: ini:0)
 16-23 TXOP_2 / TXOP_6 / TXOP_10 / TXOP_14 (hw2: ini:0)
 24-31 TXOP_3 / TXOP_7 / TXOP_11 / TXOP_15 (hw2: ini:0) |
| --- |

```

| _______________________________ hw4/hw6 only _______________________________ |
| --- |

**ATH:028304h/028024h - MAC_PCU_BT_WL_1 ;hw4/hw6**

**ATH:028308h/028028h - MAC_PCU_BT_WL_2 ;hw4/hw6**

**ATH:02830Ch/02802Ch - MAC_PCU_BT_WL_3 ;hw4/hw6**

**ATH:028310h/028030h - MAC_PCU_BT_WL_4 ;hw4/hw6**

```

| 0-31 WEIGHT |
| --- |

**ATH:028314h/028034h - MAC_PCU_COEX_EPTA ;hw4/hw6 only**

```

| 0-5 LINKID
 6-12 WT_IDX |
| --- |

**ATH:028300h/0281E4h - MAC_PCU_BT_BT_ASYNC ;hw4/hw6 (not hw2)**

```

| 0-3 TXHP_WEIGHT ;\
 4-7 TXLP_WEIGHT ; hw4/hw6 only (not hw2)
 8-11 RXHP_WEIGHT ;
 12-15 RXLP_WEIGHT ;/ |
| --- |

**ATH:028110h/028264h - MAC_PCU_LOGIC_ANALYZER ;hw4/hw6 (not hw2)**

```

| 0 HOLD ;\
 1 CLEAR ;
 2 STATE (R) ; hw4/hw6 only (not hw2)
 3 ENABLE ;
 4-7 QCU_SEL ;
 8-17 INT_ADDR (R) ;
 18-31 DIAG_MODE ;/ |
| --- |

**ATH:028114h/028268h - MAC_PCU_LOGIC_ANALYZER_32L ;hw4/hw6 (not hw2)**

**ATH:028118h/02826Ch - MAC_PCU_LOGIC_ANALYZER_16U ;hw4/hw6 (not hw2)**

```

| 0-47 MASK ;\hw4/hw6 only (not hw2)
 48-31 - ;/ |
| --- |

**ATH:02815Ch/0281C8h - MAC_PCU_BLUETOOTH_TSF_BT_ACTIVE ;hw4/hw6 (not hw2)**

```

| 0-31 VALUE (R) ;-hw4/hw6 only (not hw2) |
| --- |

**ATH:028160h/0281CCh - MAC_PCU_BLUETOOTH_TSF_BT_PRIORITY ;hw4/hw6 (not hw2)**

```

| 0-31 VALUE (R) ;-hw4/hw6 only (not hw2) |
| --- |

**ATH:028318h/028150h - MAC_PCU_COEX_LNAMAXGAIN1 ;hw4/hw6 (not hw2)**

**ATH:02831Ch/028154h - MAC_PCU_COEX_LNAMAXGAIN2 ;hw4/hw6 (not hw2)**

**ATH:028320h/028158h - MAC_PCU_COEX_LNAMAXGAIN3 ;hw4/hw6 (not hw2)**

**ATH:028324h/02815Ch - MAC_PCU_COEX_LNAMAXGAIN4 ;hw4/hw6 (not hw2)**

```

| 0-7 MAXGAIN1 ;\that are 4 gain value ;\
 8-15 MAXGAIN2 ; for each of the 4 registers ; hw4/hw6 only (not hw2)
 16-23 MAXGAIN3 ; (ie. 16 values in total) ;
 24-31 MAXGAIN4 ;/ ;/ |
| --- |

**ATH:028050h/028070h - MAC_PCU_DYM_MIMO_PWR_SAVE ;hw4/hw6 only**

```

| 0 hw4/hw6: USE_MAC_CTRL ;\
 1 hw4/hw6: HW_CTRL_EN ;
 2 hw4/hw6: SW_CHAIN_MASK_SEL ; hw4/hw6 only
 4-6 hw4/hw6: LOW_PWR_CHAIN_MASK ;
 8-10 hw4/hw6: HI_PWR_CHAIN_MASK ;/ |
| --- |

**ATH:02811Ch/02829Ch - MAC_PCU_PHY_ERR_CNT_MASK_CONT ;hw4/hw6 (not hw2)**

```

| 0-7 MASK1
 8-15 MASK2
 16-23 MASK3 |
| --- |

**ATH:028120h/028300h - MAC_PCU_AZIMUTH_MODE ;hw4/hw6 (not hw2)**

```

| 0 DISABLE_TSF_UPDATE
 1 KEY_SEARCH_AD1
 2 TX_TSF_STATUS_SEL
 3 RX_TSF_STATUS_SEL
 4 CLK_EN
 5 TX_DESC_EN
 6 ACK_CTS_MATCH_TX_AD2
 7 BA_USES_AD1
 8 hw6: WMAC_CLK_SEL
 9 hw6: FILTER_PASS_HOLD |
| --- |

**ATH:02814Ch/028314h - MAC_PCU_AZIMUTH_TIME_STAMP ;hw4/hw6 (not hw2)**

```

| 0-31 VALUE |
| --- |

**ATH:028124h/028318h - MAC_PCU_20_40_MODE ;hw4/hw6 (not hw2)**

```

| 0 JOINED_RX_CLEAR
 1 EXT_PIFS_ENABLE
 2 TX_HT20_ON_EXT_BUSY
 3 SWAMPED_FORCES_RX_CLEAR_CTL_IDLE
 4-15 PIFS_CYCLES |
| --- |

**ATH:028128h/028328h - MAC_PCU_RX_CLEAR_DIFF_CNT ;hw4/hw6 (not hw2)**

```

| 0-31 VALUE |
| --- |

**ATH:02812Ch/02832Ch - MAC_PCU_SELF_GEN_ANTENNA_MASK ;hw4/hw6 (not hw2)**

```

| 0-2 VALUE
 3 hw6: ONE_RESP_EN
 4 hw6: FORCE_CHAIN_0 |
| --- |

**ATH:028130h/028330h - MAC_PCU_BA_BAR_CONTROL ;hw4/hw6 (not hw2)**

```

| 0-3 COMPRESSED_OFFSET
 4-7 ACK_POLICY_OFFSET
 8 COMPRESSED_VALUE
 9 ACK_POLICY_VALUE
 10 FORCE_NO_MATCH
 11 TX_BA_CLEAR_BA_VALID
 12 UPDATE_BA_BITMAP_QOS_NULL |
| --- |

**ATH:028134h/028334h - MAC_PCU_LEGACY_PLCP_SPOOF ;hw4/hw6 (not hw2)**

```

| 0-7 EIFS_MINUS_DIFS
 8-12 MIN_LENGTH |
| --- |

**ATH:028138h/028338h - MAC_PCU_PHY_ERROR_MASK_CONT ;hw4/hw6 (not hw2)**

```

| 0-7 MASK_VALUE
 16-23 EIFS_VALUE
 24-31 hw6: AIFS_VALUE |
| --- |

**ATH:02813Ch/02833Ch - MAC_PCU_TX_TIMER ;hw4/hw6 (not hw2)**

```

| 0-14 TX_TIMER
 15 TX_TIMER_ENABLE
 16-19 RIFS_TIMER
 20-24 QUIET_TIMER
 25 QUIET_TIMER_ENABLE |
| --- |

**ATH:028140h/028340h - MAC_PCU_TXBUF_CTRL ;hw4/hw6 (not hw2)**

```

| 0-11 USABLE_ENTRIES
 16 TX_FIFO_WRAP_ENABLE |
| --- |

**ATH:028148h/028348h - MAC_PCU_ALT_AES_MUTE_MASK ;hw4/hw6 (not hw2)**

```

| 16-31 QOS |
| --- |

**ATH:028338h/028600h - MAC_PCU_RX_INT_STATUS0 ;hw4/hw6**

```

| 0-7 FRAME_CONTROL_L (R)
 8-15 FRAME_CONTROL_H (R)
 16-23 DURATION_L (R)
 24-31 DURATION_H (R) |
| --- |

**ATH:02833Ch/028604h - MAC_PCU_RX_INT_STATUS1 ;hw4/hw6**

```

| 0-17 VALUE (R) |
| --- |

**ATH:028340h/028608h - MAC_PCU_RX_INT_STATUS2 ;hw4/hw6**

```

| 0-26 VALUE (R) |
| --- |

**ATH:028344h/02860Ch - MAC_PCU_RX_INT_STATUS3 ;hw4/hw6**

```

| 0-23 VALUE (R) |
| --- |

**ATH:028348h/028610h - HT_HALF_GI_RATE1 ;MCS0..3 ;hw4/hw6**

**ATH:02834Ch/028614h - HT_HALF_GI_RATE2 ;MCS4..7 ;hw4/hw6**

```

| 0-7 MCS0 / MCS4
 8-15 MCS1 / MCS5
 16-23 MCS2 / MCS6
 24-31 MCS3 / MCS7 |
| --- |

**ATH:028350h/028618h - HT_FULL_GI_RATE1 ;MCS0..3 ;hw4/hw6**

**ATH:028354h/02861Ch - HT_FULL_GI_RATE2 ;MCS4..7 ;hw4/hw6**

```

| 0-7 MCS0 / MCS4
 8-15 MCS1 / MCS5
 16-23 MCS2 / MCS6
 24-31 MCS3 / MCS7 |
| --- |

**ATH:028358h/028620h - LEGACY_RATE1 ;RATE 8..12 ;hw4/hw6**

**ATH:02835Ch/028624h - LEGACY_RATE2 ;RATE 13..15 and RATE 24..25 ;hw4/hw6**

**ATH:028360h/028628h - LEGACY_RATE3 ;RATE 26..30 ;hw4/hw6**

```

| 0-5 RATE8 / RATE13 / RATE26
 6-11 RATE9 / RATE14 / RATE27
 12-17 RATE10 / RATE15 / RATE28
 18-23 RATE11 / RATE24 / RATE29
 24-29 RATE12 / RATE25 / RATE30 |
| --- |

**ATH:028364h/02862Ch - RX_INT_FILTER ;hw4/hw6**

```

| 0 ENABLE
 1 DIRECTED
 2 BCAST
 3 MCAST
 4 RTS
 5 ACK
 6 CTS
 7 RETRY
 8 MORE_DATA
 9 MORE_FRAG
 10 RATE_HIGH
 11 RATE_LOW
 12 RSSI
 13 LENGTH_HIGH
 14 LENGTH_LOW
 15 EOSP
 16 AMPDU
 17 hw4.2: BEACON ;-hw6 and newer "hw4.2" revision only
 18 hw6: RSSI_HIGH ;-hw6 only |
| --- |

**ATH:028368h/028630h - RX_INT_OVERFLOW ;hw4/hw6**

```

| 0 STATUS |
| --- |

**ATH:02836Ch/028634h - RX_FILTER_THRESH0 ;hw4/hw6**

```

| 0-7 RATE_HIGH
 8-15 RATE_LOW
 16-23 RSSI_LOW
 24-31 hw6: RSSI_HIGH |
| --- |

**ATH:028370h/028638h - RX_FILTER_THRESH1 ;hw4/hw6**

```

| 0-11 LENGTH_HIGH
 12-23 LENGTH_LOW |
| --- |

**ATH:028374h/02863Ch - RX_PRIORITY_THRESH0 ;hw4/hw6**

```

| 0-7 RATE_HIGH
 8-15 RATE_LOW
 16-23 RSSI_HIGH
 24-31 RSSI_LOW |
| --- |

**ATH:028378h/028640h - RX_PRIORITY_THRESH1 ;hw4/hw6**

```

| 0-11 LENGTH_HIGH
 12-23 LENGTH_LOW
 24-31 XCAST_RSSI_HIGH |
| --- |

**ATH:02837Ch/028644h - RX_PRIORITY_THRESH2 ;hw4/hw6**

```

| 0-7 PRESP_RSSI_HIGH
 8-15 MGMT_RSSI_HIGH
 16-23 BEACON_RSSI_HIGH
 24-31 NULL_RSSI_HIGH |
| --- |

**ATH:028380h/028648h - RX_PRIORITY_THRESH3 ;hw4/hw6**

```

| 0-7 PREQ_RSSI_HIGH
 8-15 PS_POLL_RSSI_HIGH |
| --- |

**ATH:028384h/02864Ch - RX_PRIORITY_OFFSET0 ;hw4/hw6**

```

| 0-5 PHY_RATE_HIGH
 6-11 PHY_RATE_LOW
 12-17 RSSI_HIGH
 18-23 RSSI_LOW
 24-29 XCAST_RSSI_HIGH |
| --- |

**ATH:028388h/028650h - RX_PRIORITY_OFFSET1 ;hw4/hw6**

```

| 0-5 LENGTH_HIGH
 6-11 LENGTH_LOW
 12-17 PRESP_RSSI_HIGH
 18-23 RETX
 24-29 RTS |
| --- |

**ATH:02838Ch/028654h - RX_PRIORITY_OFFSET2 ;hw4/hw6**

```

| 0-5 XCAST
 6-11 PRESP
 12-17 ATIM
 18-23 MGMT
 24-29 BEACON |
| --- |

**ATH:028390h/028658h - RX_PRIORITY_OFFSET3 ;hw4/hw6**

```

| 0-5 MORE
 6-11 EOSP
 12-17 AMPDU
 18-23 AMSDU
 24-29 PS_POLL |
| --- |

**ATH:028394h/02865Ch - RX_PRIORITY_OFFSET4 ;hw4/hw6**

```

| 0-5 PREQ
 6-11 NULL
 12-17 BEACON_SSID
 18-23 MGMT_RSSI_HIGH
 24-29 BEACON_RSSI_HIGH |
| --- |

**ATH:028398h/028660h - RX_PRIORITY_OFFSET5 ;hw4/hw6**

```

| 0-5 NULL_RSSI_HIGH
 6-11 PREQ_RSSI_HIGH
 12-17 PS_POLL_RSSI_HIGH |
| --- |

**ATH:028200h..0282FCh - MAC_PCU_BT_BT[0..63] ;hw4 (at 028200h)**

**ATH:028500h..0285FCh - MAC_PCU_BT_BT[0..63] ;hw6 (at 028500h)**

```

| 0-31 WEIGHT |
| --- |

**ATH:028400h..0284FCh - MAC_PCU_TXBUF_BA[0..63] ;hw4/hw6**

```

| 0-31 DATA |
| --- |

**ATH:028800h..028BFCh - MAC_PCU_KEY_CACHE_1[0..255] ;hw4 (256 words)**

**ATH:028800h..028FFCh - MAC_PCU_KEY_CACHE[0..511] ;hw6 (512 words)**

```

| 0-31 DATA |
| --- |

**ATH:02E000h..02E7FCh - MAC_PCU_BUF[0..511] ;hw4 (512 words)**

**ATH:02E000h..02FFFCh - MAC_PCU_BUF[0..2047] ;hw6 (2048 words)**

```

| 0-31 DATA |
| --- |

```

| _________________________________ hw6 only _________________________________ |
| --- |

**ATH:0280A0h - MAC_PCU_BASIC_SET ;hw6 only**

```

| 0-31 MCS ;-hw6 only |
| --- |

**ATH:0280A4h - MAC_PCU_MGMT_SEQ ;hw6 only**

```

| 0-11 MIN ;\hw6 only
 16-27 MAX ;/ |
| --- |

**ATH:0280A8h - MAC_PCU_BF_RPT1 ;hw6 only**

```

| 0-7 V_ACTION_VALUE ;\
 8-15 CV_ACTION_VALUE ; hw6 only
 16-23 CATEGORY_VALUE ;
 24-27 FRAME_SUBTYPE_VALUE ;
 28-29 FRAME_TYPE_VALUE ;/ |
| --- |

**ATH:0280ACh - MAC_PCU_BF_RPT2 ;hw6 only**

```

| 0-3 FRAME_SUBTYPE_VALUE ;-hw6 only |
| --- |

**ATH:0280B0h - MAC_PCU_TX_ANT_1 ;hw6 only**

**ATH:0280B4h - MAC_PCU_TX_ANT_2 ;hw6 only**

**ATH:0280B8h - MAC_PCU_TX_ANT_3 ;hw6 only**

**ATH:0280BCh - MAC_PCU_TX_ANT_4 ;hw6 only**

```

| 0-31 VALUE ;-hw6 only |
| --- |

**ATH:028038h - MAC_PCU_MAX_CFP_DUR ;hw6 only (merged two hw4 registers here)**

```

| 0-15 VALUE ;-formerly bit0-15 of "PCU_MAX_CFP_DUR"
 16-19 USEC_FRAC_NUMERATOR ;-formerly bit0-3 of "MAC_PCU_MAX_CFP_DUR"
 24-27 USEC_FRAC_DENOMINATOR ;-formerly bit4-7 of "MAC_PCU_MAX_CFP_DUR" |
| --- |

**ATH:028020h - MAC_PCU_BCN_RSSI_CTL2 ;hw6 only**

```

| 0-7 RSSI2_LOW_THRESH
 16-23 RSSI2_HIGH_THRESH
 29 RESET2 |
| --- |

**ATH:0280DCh - MAC_PCU_SELF_GEN_DEFAULT ;hw6 only**

```

| 0-2 MMSS ;\
 3-4 CEC ; hw6 only
 5 STAGGER_SOUNDING ;/ |
| --- |

**ATH:02831Ch - MAC_PCU_H_XFER_TIMEOUT ;hw6 only**

```

| 0-4 VALUE ;\
 5 DISABLE ;
 6 EXTXBF_IMMEDIATE_RESP ; hw6 only
 7 DELAY_EXTXBF_ONLY_UPLOAD_H ;
 8 EXTXBF_NOACK_NORPT ;/ |
| --- |

**ATH:028200h - MAC_PCU_TDMA_TXFRAME_START_TIME_TRIGGER_LSB ;hw6**

```

| 0-31 VALUE |
| --- |

**ATH:028204h - MAC_PCU_TDMA_TXFRAME_START_TIME_TRIGGER_MSB ;hw6**

```

| 0-31 VALUE |
| --- |

**ATH:028208h - MAC_PCU_TDMA_SLOT_ALERT_CNTL ;hw6**

```

| 0-15 VALUE |
| --- |

**ATH:028284h - PCU_1US ;hw6**

```

| 0-6 SCALER |
| --- |

**ATH:028288h - PCU_KA ;hw6**

```

| 0-11 DEL |
| --- |

**ATH:028350h - ASYNC_FIFO_REG1 ;hw6**

```

| 0-29 DBG |
| --- |

**ATH:028354h - ASYNC_FIFO_REG2 ;hw6**

```

| 0-27 DBG |
| --- |

**ATH:028358h - ASYNC_FIFO_REG3 ;hw6**

```

| 0-9 DBG
 10 DATAPATH_SEL
 31 SFT_RST_N |
| --- |

**ATH:028388h - MAC_PCU_LOCATION_MODE_CONTROL ;hw6**

```

| 0 ENABLE
 1 UPLOAD_H_DISABLE |
| --- |

**ATH:02838Ch - MAC_PCU_LOCATION_MODE_TIMER ;hw6**

```

| 0-31 VALUE |
| --- |

**ATH:0283A0h - MAC_PCU_DIRECT_CONNECT ;hw6**

```

| 0 TSF2_ENABLE
 1 TS_TSF_SEL
 2 TSF1_UPDATE
 3 TSF2_UPDATE
 4 MY_BEACON_OVERRIDE
 5 MY_BEACON2_OVERRIDE
 6 BMISS_CNT_TSF_SEL
 7 BMISS_CNT_OVERRIDE
 8-31 RESERVED |
| --- |

**ATH:0283A4h - MAC_PCU_TID_TO_AC ;hw6**

```

| 0-31 DATA |
| --- |

**ATH:0283A8h - MAC_PCU_HP_QUEUE ;hw6**

```

| 0 ENABLE
 1 AC_MASK_BE
 2 AC_MASK_BK
 3 AC_MASK_VI
 4 AC_MASK_VO
 5 HPQON_UAPSD
 6 FRAME_FILTER_ENABLE0
 7 FRAME_BSSID_MATCH0
 8-9 FRAME_TYPE0
 10-11 FRAME_TYPE_MASK0
 12-15 FRAME_SUBTYPE0
 16-19 FRAME_SUBTYPE_MASK0
 20 UAPSD_EN
 21 PM_CHANGE
 22 NON_UAPSD_EN
 23 UAPSD_AC_MUST_MATCH
 24 UAPSD_ONLY_QOS |
| --- |

**ATH:0283BCh - MAC_PCU_AGC_SATURATION_CNT0 ;hw6**

**ATH:0283C0h - MAC_PCU_AGC_SATURATION_CNT1 ;hw6**

**ATH:0283C4h - MAC_PCU_AGC_SATURATION_CNT2 ;hw6**

```

| 0-31 VALUE |
| --- |

**ATH:0283C8h - MAC_PCU_HW_BCN_PROC1 ;hw6**

```

| 0 CRC_ENABLE
 1 RESET_CRC
 2 EXCLUDE_BCN_INTVL
 3 EXCLUDE_CAP_INFO
 4 EXCLUDE_TIM_ELM
 5 EXCLUDE_ELM0
 6 EXCLUDE_ELM1
 7 EXCLUDE_ELM2
 8-15 ELM0_ID
 16-23 ELM1_ID
 24-31 ELM2_ID |
| --- |

**ATH:0283CCh - MAC_PCU_HW_BCN_PROC2 ;hw6**

```

| 0 FILTER_INTERVAL_ENABLE
 1 RESET_INTERVAL
 2 EXCLUDE_ELM3
 8-15 FILTER_INTERVAL
 16-23 ELM3_ID |
| --- |

**ATH:0283DCh - MAC_PCU_PS_FILTER ;hw6**

```

| 0 ENABLE
 1 PS_SAVE_ENABLE |
| --- |

**ATH:028668h - MAC_PCU_PHY_ERROR_AIFS ;hw6**

```

| 0 MASK_ENABLE |
| --- |

**ATH:02866Ch - MAC_PCU_PHY_ERROR_AIFS_MASK ;hw6**

```

| 0-31 VALUE |
| --- |

**ATH:028670h - MAC_PCU_FILTER_RSSI_AVE ;hw6**

```

| 0-7 AVE_VALUE
 8-10 NUM_FRAMES_EXPONENT
 11 ENABLE
 12 RESET |
| --- |

**ATH:028674h - MAC_PCU_TBD_FILTER ;hw6**

```

| 0 USE_WBTIMER_TX_TS
 1 USE_WBTIMER_RX_TS |
| --- |

**ATH:028678h - MAC_PCU_BT_ANT_SLEEP_EXTEND ;hw6**

```

| 0-15 CNT |
| --- |

```

| _____________________ Wake on Wireless (WOW) hw6 only _____________________ |
| --- |

**ATH:02825Ch - MAC_PCU_WOW1 ;WOW Misc ;hw6**

```

| 0-7 PATTERN_ENABLE
 8-15 PATTERN_DETECT (R)
 16 MAGIC_ENABLE
 17 MAGIC_DETECT (R)
 18 INTR_ENABLE
 19 INTR_DETECT (R)
 20 KEEP_ALIVE_FAIL (R)
 21 BEACON_FAIL (R)
 28-31 CW_BITS |
| --- |

**ATH:028260h - MAC_PCU_WOW2 ;WOW AIFS/SLOT/TRY_CNT ;hw6**

```

| 0-7 AIFS
 8-15 SLOT
 16-23 TRY_CNT |
| --- |

**ATH:028270h - MAC_PCU_WOW3_BEACON_FAIL ;WOW Beacon Fail Enable ;hw6**

```

| 0 ENABLE |
| --- |

**ATH:028274h - MAC_PCU_WOW3_BEACON ;WOW Beacon Timeout ;hw6**

**ATH:028278h - MAC_PCU_WOW3_KEEP_ALIVE ;WOW Keep-Alive Timeout ;hw6**

```

| 0-31 TIMEOUT |
| --- |

**ATH:028370h - MAC_PCU_WOW4 ;WOW Pattern Enable/Detect ;hw6**

```

| 0-7 PATTERN_ENABLE
 8-15 PATTERN_DETECT (R) |
| --- |

**ATH:02835Ch - MAC_PCU_WOW5 ;WOW RX Abort Enable ;hw6**

```

| 0-15 RX_ABORT_ENABLE |
| --- |

**ATH:02834Ch - MAC_PCU_WOW6 (R) ;WOW RX Buf Start Addr ;hw6**

```

| 0-15 RXBUF_START_ADDR (R) |
| --- |

**ATH:02827Ch - MAC_PCU_WOW_KA ;WOW Auto/Fail/BkoffCs Enable/Disable ;hw6**

```

| 0 AUTO_DISABLE
 1 FAIL_DISABLE
 2 BKOFF_CS_ENABLE |
| --- |

**ATH:028294h - PCU_WOW4 ;WOW Offset 0..3 ;hw6**

**ATH:028298h - PCU_WOW5 ;WOW Offset 4..7 ;hw6**

**ATH:028378h - PCU_WOW6 ;WOW Offset 8..11 ;hw6**

**ATH:02837Ch - PCU_WOW7 ;WOW Offset 12..15 ;hw6**

```

| 0-7 OFFSET0 / OFFSET4 / OFFSET8 / OFFSET12 ;<-- 1st offset in LSBs
 8-15 OFFSET1 / OFFSET5 / OFFSET9 / OFFSET13
 16-23 OFFSET2 / OFFSET6 / OFFSET10 / OFFSET14
 24-31 OFFSET3 / OFFSET7 / OFFSET11 / OFFSET15 |
| --- |

**ATH:028360h - MAC_PCU_WOW_LENGTH1 ;WOW Pattern 0..3 ;hw6**

**ATH:028364h - MAC_PCU_WOW_LENGTH2 ;WOW Pattern 4..7 ;hw6**

**ATH:028380h - MAC_PCU_WOW_LENGTH3 ;WOW Pattern 8..11 ;hw6**

**ATH:028384h - MAC_PCU_WOW_LENGTH4 ;WOW Pattern 12..15 ;hw6**

```

| 0-7 PATTERN_3 / PATTERN_7 / PATTERN_11 / PATTERN_15
 8-15 PATTERN_2 / PATTERN_6 / PATTERN_10 / PATTERN_14
 16-23 PATTERN_1 / PATTERN_5 / PATTERN_9 / PATTERN_13
 24-31 PATTERN_0 / PATTERN_4 / PATTERN_8 / PATTERN_12 ;1st pattern in MSBs |
| --- |

**ATH:02828Ch - WOW_EXACT ;WOW Exact Length/Offset 1 ;hw6**

**ATH:028374h - WOW2_EXACT ;WOW Exact Length/Offset 2 ;hw6**

```

| 0-7 LENGTH
 8-15 OFFSET |
| --- |

**ATH:028368h - WOW_PATTERN_MATCH_LESS_THAN_256_BYTES ;hw6**

```

| 0-15 EN |
| --- |

```

| _________________________________ hw4 only _________________________________ |
| --- |

**ATH:028020h - PCU_MAX_CFP_DUR ;hw4 only**

```

| 0-15 VALUE |
| --- |

Note: This register does have (almost) the same name as the register below, but
without the "MAC_" prefix. In hw6, these two registers have been merged into a
single register (called MAC_PCU_MAX_CFP_DUR). In hw2, registers REG_CFP_PERIOD
and REG_CFP_DUR might be equivalent?

**ATH:028150h - MAC_PCU_MAX_CFP_DUR ;hw4 only**

```

| 0-3 USEC_FRAC_NUMERATOR
 4-7 USEC_FRAC_DENOMINATOR |
| --- |

Note: See "PCU_MAX_CFP_DUR" (other register with similar name, but without
"MAC_" prefix).

**ATH:0283A4h - MAC_PCU_TSF1_STATUS_L32 ;hw4 only**

**ATH:0283A8h - MAC_PCU_TSF1_STATUS_U32 ;hw4 only**

```

| 0-63 VALUE |
| --- |

**ATH:0283ACh - MAC_PCU_TSF2_STATUS_L32 ;hw4 only**

**ATH:0283B0h - MAC_PCU_TSF2_STATUS_U32 ;hw4 only**

```

| 0-63 VALUE |
| --- |

**ATH:029800h..029FFCh - MAC_PCU_BASEBAND_0[0..511] ;hw4**

**ATH:02A000h..02BFFCh - MAC_PCU_BASEBAND_1[0..2047] ;hw4**

```

| 0-31 DATA |
| --- |

These two "MAC_PCU" areas are just placeholders for the Baseband Registers at
029800h and up (see BB chapter for details).

**ATH:02C000h..02CFFCh - MAC_PCU_BASEBAND_2[0..1023] ;hw4**

**ATH:02D000h..02DFFCh - MAC_PCU_BASEBAND_3[0..1023] ;hw4**

```

| 0-31 DATA |
| --- |

Unknown what these two "MAC_PCU" areas are intended for.

```

| _________________________________ hw2 only _________________________________ |
| --- |

**ATH:028500h.. (140h..17Fh) - MAC_PCU_REG_FTYPE[0..3Fh] ;hw2**

```

| 0 BFCOEF_RX_UPDATE_NORMAL
 1 BFCOEF_RX_UPDATE_SELF_GEN
 2 BFCOEF_TX_ENABLE_NORMAL
 3 BFCOEF_TX_ENABLE_SELF_GEN
 4 BFCOEF_TX_ENABLE_GEN
 5 BFCOEF_TX_ENABLE_MCAST
 6 FILTER_PASS_IF_ALL
 7 FILTER_PASS_IF_DIRECTED
 8 FILTER_PASS_IF_MCAST
 9 FILTER_PASS_IF_BCAST
 10 FILTER_PASS_MC_BC_BSSID |
| --- |

**ATH:028020h - REG_BEACON ;hw2 only**

```

| 0-15 BEACON_PERIOD
 16-22 TIM_OFFSET
 23 unspecified
 24 RESET_TSF <--- related to hw4/hw6: see MAC_PCU_RESET_TSF ? |
| --- |

**ATH:028024h - REG_CFP_PERIOD ;hw2 only**

**ATH:028038h - REG_CFP_DUR ;hw2 only**

```

| unspecified |
| --- |

These two hw2 registers have unspecified content and purpose. Going by the
names, they might be similar or equivalent to "MAC_PCU_MAX_CFP_DUR" and
"PCU_MAX_CFP_DUR" on hw4 (although if so, unknown which one would be which).

**ATH:028028h - REG_TIMER0 ;hw2 only**

**ATH:02802Ch - REG_TIMER1 ;hw2 only**

**ATH:028030h - REG_TIMER2 ;hw2 only**

**ATH:028034h - REG_TIMER3 ;hw2 only**

```

| unspecified ;MAYBE related to MAC_PCU_BT_WL_1..4 or so in hw4/hw6 (?) |
| --- |

**ATH:02804Ch - REG_TSF_L32 ;hw2 only ... aka MAC_PCU_TSF_L32 ?**

**ATH:028050h - REG_TSF_U32 ;hw2 only ... aka MAC_PCU_TSF_U32 ?**

```

| unspecified |
| --- |

**ATH:028104h - MAC_PCU_REG_TSF ;hw2 only ;aka MAC_PCU_TSF_ADD_PLL on hw4?**

```

| 0-7 TSF_INCREMENT (hw2: ini:1) ;-hw2 only |
| --- |

**ATH:028114h - MAC_PCU_REG_ACKSIFS_RESERVED ;hw2 only**

```

| 0-7 ACKSIFS_INCREMENT_RESERVED (hw2: ini:0) ;-hw2 only |
| --- |

Related to the "MAC_PCU_REG_ACKSIFSMEM_RESERVED[0..1Fh]" array?

**ATH:028680h.. (1A0h..1BFh) - MAC_PCU_REG_ACKSIFSMEM_RESERVED[0..1Fh] ;hw2**

```

| 0-9 NORMAL_RESERVED
 10-19 TURBO_RESERVED |
| --- |

**ATH:028700h.. (1C0h..1DFh) - MAC_PCU_REG_DUR[0..1Fh] ;hw2**

```

| 0-15 DUR_RATE_TO_DURATION |
| --- |

**ATH:0287C0h.. (1F0h..1F7h) - MAC_PCU_REG_RTD[0..7] ;hw2**

```

| 0-4 RTD_RATE_TO_DB_0
 8-12 RTD_RATE_TO_DB_1
 16-20 RTD_RATE_TO_DB_2
 24-28 RTD_RATE_TO_DB_3 |
| --- |

**ATH:0287E0h.. (1F8h..1FFh) - MAC_PCU_REG_DTR[0..7] ;hw2**

```

| 0-4 DTR_DB_TO_RATE_0
 8-12 DTR_DB_TO_RATE_1
 16-20 DTR_DB_TO_RATE_2
 24-28 DTR_DB_TO_RATE_3 |
| --- |

**ATH:028800h.. (200h..5FFh) - MAC_PCU_REG_KC[0..3FFh] ;hw2**

Below bitfields are supposedly somehow stored in multiple words...?

```

| 0-31 KC_KEY_31_0 ;aka byte 00h..03h ?
 0-15 KC_KEY_47_32 ;aka byte 04h..05h (and 06h..07h unused?) ?
 0-31 KC_KEY_79_48 ;aka byte 08h..0Bh ?
 0-15 KC_KEY_95_80 ;aka byte 0Ch..0Dh (and 0Eh..0Fh unused?) ?
 0-31 KC_KEY_127_96 ;aka byte 10h..13h ?
 0-2 KC_KEY_TYPE ;\
 3 KC_LAST_ANTENNA ;
 4-8 KC_ASYNC_ACK_OFFSET ;
 9 KC_UPDATE_BEAM_FORMING ;aka byte 14h..15h (and 16h..17h unused?) ?
 10 KC_RX_CHAIN_0_ACK_ANT ;
 11 KC_RX_CHAIN_1_ACK_ANT ;
 12 KC_TX_CHAIN_0_ANT_SEL ;
 13 KC_TX_CHAIN_1_ANT_SEL ;
 14 KC_TX_CHAIN_SEL ;/
 0-31 KC_ADDR_32_1 ;aka byte 18h..1Bh ? (no bit 0 ?)
 0-14 KC_ADDR_47_33 ;aka byte 1Ch..1Dh (and 1Eh..1Fh unused?) ?
 1 KC_VALID ;aka byte 20h (and 21h..xxh unused?) ? |
| --- |

**ATH:028180h..02819Ch - MAC_PCU_REG_BFCOEF1[0..7] ;hw2**

```

| 0-23 TSF
 24-30 KEYIDX
 31 KEY_VALID (hw2: ini:0) |
| --- |

**ATH:0281C0h - MAC_PCU_REG_BFCOEF2 ;hw2**

```

| 0-22 THRESH (hw2: ini:0)
 23 unspecified
 24-31 LOCK (hw2: ini:0) |
| --- |

**ATH:0281C4h - MAC_PCU_REG_KCMASK ;hw2**

```

| 0-15 KCMASK_47_32 (hw2: ini:0000h)
 16 KCMASK_31_0 (hw2: ini:0) |
| --- |

```

| ___________________________ hw2 "MCI" registers ___________________________ |
| --- |

Below "MCIxxx" registers exist on hw2 only. Purpose is unknown. There seems to
be nothing equivalent in hw4. However, hw6 is having several "MCI_xxx"
registers (see WLAN Coex section; unknown if that's related to hw2 MCI stuff).

**ATH:028268h - MAC_PCU_REG_MCICTL ;Control ;hw2**

```

| 0 MCI_ENABLE (hw2: ini:0)
 1 OLA_ENABLE (hw2: ini:1)
 2 PREEMPT_ENABLE (hw2: ini:1)
 3 CHANNEL_BUSY_ENABLE (hw2: ini:1)
 4-9 EARLY_NOTIFY_DELAY (hw2: ini:5)
 10 BMISS_FORCE_WL (hw2: ini:0)
 11 SLEEP_FORCE_BT (hw2: ini:1)
 12 HP_QCU_STOMP_BT (hw2: ini:0)
 31 MCI_BUSY |
| --- |

**ATH:02826Ch - MAC_PCU_REG_MCIISR ;Interrupt Status ;hw2**

**ATH:028270h - MAC_PCU_REG_MCIIER ;Interrupt Enable ;hw2**

```

| 0 ACT_RPT_RCV_INT (hw2: stat and enable: ini:0)
 1 ACT_DEN_RCV_INT (hw2: stat and enable: ini:0)
 2 FRQ_RPT_RCV_INT (hw2: stat and enable: ini:0)
 3 QOS_RPT_RCV_INT (hw2: stat and enable: ini:0)
 4 GEN_RPT_RCV_INT (hw2: stat and enable: ini:0) |
| --- |

**ATH:028274h - MAC_PCU_REG_MCIWLP ;WLP ?? ;hw2 (hw2: ini:0)**

```

| unspecified |
| --- |

**ATH:028278h - MAC_PCU_REG_MCIARW ;AR Write? ;hw2 (hw2: ini:0)**

**ATH:02827Ch - MAC_PCU_REG_MCIARR ;AR Read? ;hw2**

**ATH:028280h - MAC_PCU_REG_MCIADW ;AD Write? ;hw2 (hw2: ini:0)**

**ATH:028284h - MAC_PCU_REG_MCIADR ;AD Read? ;hw2**

**ATH:028288h - MAC_PCU_REG_MCIFRW ;FR Write? ;hw2 (hw2: ini:0)**

**ATH:02828Ch - MAC_PCU_REG_MCIFRR ;FR Read? ;hw2**

**ATH:028290h - MAC_PCU_REG_MCIQRW ;QR Write? ;hw2 (hw2: ini:0)**

**ATH:028294h - MAC_PCU_REG_MCIQRR ;QR Read? ;hw2**

**ATH:028298h - MAC_PCU_REG_MCIGRW ;GR Write? ;hw2 (hw2: ini:0)**

**ATH:02829Ch - MAC_PCU_REG_MCIGRR ;GR Read? ;hw2**

```

| unspecified |
| --- |

**ATH:0282A0h - MAC_PCU_REG_MCISTAT ;Status (counters?) ;hw2**

```

| 0-7 ACT_RPT_RCV_CNT (hw2: ini:0)
 8-15 QC_CNT (hw2: ini:0)
 16-23 OLA_CNT (hw2: ini:0)
 24-31 PREEMPT_CNT (hw2: ini:0) |
| --- |

```

| ___________ hw2 MAC_PCU registers (moved to RTC WLAN in hw4/hw6) ___________ |
| --- |

**ATH:028200h..02821Ch - MAC_PCU_REG_GNRCTMR_N[0..7] ;hw2**

**ATH:028220h..02823Ch - MAC_PCU_REG_GNRCTMR_P[0..7] ;hw2**

**ATH:028240h - MAC_PCU_GENERIC_TIMERS_MODE ;aka MAC_PCU_REG_GNRCTMR_M ;hw2**

**ATH:0280D4h - MAC_PCU_SLP1 ;aka MAC_PCU_REG_SLP1 ;hw2**

**ATH:0280D8h - MAC_PCU_SLP2 ;aka MAC_PCU_REG_SLP2 ;hw2**

**ATH:0280DCh - (outcommented) ;aka MAC_PCU_REG_SLP3 ;hw2 (but outcommented)**

**ATH:028260h - MAC_PCU_SLP3 ;aka old name: MAC_PCU_REG_SLP4 (four) ;hw2**

This stuff is located in "WMAC PCU" at 028xxxh in hw2 only. Later versions have
it moved to the "RTC WLAN" area (at 004xxxh or 005xxxh), see there for details.

**ATH:028244h (mirror of 0040F4h) - MAC_PCU_REG_SLP32_MODE (ini:10F424h) ;hw2**

**ATH:028248h (mirror of 0040F8h) - MAC_PCU_REG_SLP32_WAKE (ini:07EFh) ;hw2**

**ATH:02824Ch (mirror of 0040FCh) - MAC_PCU_REG_SLP32_TSF_INC (ini:1E848h) ;hw2**

**ATH:028250h (mirror of 004100h) - MAC_PCU_REG_SLPMIB1 ;hw2**

**ATH:028254h (mirror of 004104h) - MAC_PCU_REG_SLPMIB2 ;hw2**

**ATH:028258h (mirror of 004108h) - MAC_PCU_REG_SLPMIB3 ;hw2**

**ATH:028264h (mirror of 00410Ch) - MAC_PCU_REG_SLP5 (ini: 0FFFFFFh) ;hw2**

These hw2 registers seem to be just mirrors of other hw2 registers in the RTC
area at 004xxxh (see RTC WLAN chapter for details).

In hw4/hw6, the mirrors at 028xxxh are removed (and only the registers at
004xxxh are kept; whereas, in hw6 that part moved to 005xxxh).

```

| __________________________ outcommented hw2 stuff __________________________ |
| --- |

Below outcommented stuff is found in hw2 source code, maybe it was used in
older hw2 revisions (in case there multiple hw2 revisions), or maybe it was
used in even older pre-hw2 chips, or maybe it's just some experimental stuff
that was never implemented in hardware.

**ATH:028140h - outcommented:MAC_PCU_REG_TSFCAL ;Misc ;hw2**

```

| 0-3 outcommented:COUNT (hw2: ini:8) ;\
 4-7 outcommented:INTERVAL (hw2: ini:0Ah) ; hw2 only
 8 outcommented:ENABLE (hw2: ini:1) ; (although it's
 9 outcommented:AUTO_CAL (hw2: ini:1) ; outcommented even
 10 outcommented:VALUE_WE (hw2: ini:0) ; in hw2 source code)
 16-31 outcommented:VALUE (hw2: ini:8000h) ;/ |
| --- |

**ATH:02814Ch - outcommented:MAC_PCU_REG_SYNC2 ;Misc ;hw2**

```

| 0-7 outcommented:TIME_OFFSET (hw2: ini:0)
 8 outcommented:MASTER (hw2: ini:0)
 9 outcommented:REPLACE (hw2: ini:0)
 10 outcommented:TUNE (hw2: ini:0)
 11 outcommented:CLEAR (hw2: ini:0)
 16-31 outcommented:INTR_THRESH (hw2: ini:FFFFh) |
| --- |

**ATH:028148h - outcommented:MAC_PCU_REG_SYNC1 ;Time (ini:0) ;hw2**

**ATH:028158h - outcommented:MAC_PCU_REG_SYNC5 ;RX Time ;hw2**

**ATH:028160h - outcommented:MAC_PCU_REG_SYNC7 ;Last Time ;hw2**

**ATH:028164h - outcommented:MAC_PCU_REG_SYNC8 ;Updated Time ;hw2**

```

| 0-30 outcommented:TIME
 31 - |
| --- |

**ATH:028150h - outcommented:MAC_PCU_REG_SYNC3 ;MCAST Addr_L ;hw2**

**ATH:028154h - outcommented:MAC_PCU_REG_SYNC4 ;MCAST Addr_U ;hw2**

```

| 0-47 outcommented:MCAST_ADDR (hw2: ini:0)
 48-63 - |
| --- |

**ATH:02815Ch - outcommented:MAC_PCU_REG_SYNC6 ;INC ;hw2**

```

| 0-31 outcommented:INC |
| --- |

| DSi Atheros Wifi - Internal I/O - 029800h - BB Baseband (hw4/hw6) |
| --- |

**ATH:029800h/02A360h - BB_TEST_CONTROLS ;hw4/hw6**

```

| 0-3 CF_TSTTRIG_SEL
 4 CF_TSTTRIG
 5-6 CF_RFSHIFT_SEL
 8-9 CARDBUS_MODE
 10 CLKOUT_IS_CLK32
 13 ENABLE_RFSILENT_BB
 15 ENABLE_MINI_OBS
 17 SLOW_CLK160
 18 AGC_OBS_SEL_3
 19-22 CF_BBB_OBS_SEL
 23 RX_OBS_SEL_5TH_BIT
 24 AGC_OBS_SEL_4
 28 FORCE_AGC_CLEAR
 30-31 TSTDAC_OUT_SEL |
| --- |

**ATH:029804h/02A204h - BB_GEN_CONTROLS ;hw4/hw6**

```

| 0 TURBO
 1 CF_SHORT20
 2 DYN_20_40
 3 DYN_20_40_PRI_ONLY
 4 DYN_20_40_PRI_CHN
 5 DYN_20_40_EXT_CHN
 6 HT_ENABLE
 7 ALLOW_SHORT_GI
 8 CF_2_CHAINS_USE_WALSH
 9 hw4: CF_SINGLE_HT_LTF1 ;-hw4
 9 hw6: CF_3_CHAINS_USE_WALSH ;-hw6
 10 GF_ENABLE
 11 hw4: BYPASS_DAC_FIFO_N ;-hw4
 11 hw6: ENABLE_DAC_ASYNC_FIFO ;\
 14 hw6: BOND_OPT_CHAIN_SEL ;
 15 hw6: STATIC20_MODE_HT40_PACKET_HANDLING ;
 16 hw6: STATIC20_MODE_HT40_PACKET_ERROR_RPT ; hw6
 17 hw6: ENABLE_CSD_PHASE_DITHERING ;
 18-24 hw6: UNSUPP_HT_RATE_THRESHOLD ;
 25 hw6: EN_ERR_TX_CHAIN_MASK_ZERO ;
 26 hw6: IS_MCKINLEY_TPC ;/ |
| --- |

**ATH:029808h/02A364h - BB_TEST_CONTROLS_STATUS ;hw4/hw6**

```

| 0 CF_TSTDAC_EN
 1 CF_TX_SRC_IS_TSTDAC
 2-4 CF_TX_OBS_SEL
 5-6 CF_TX_OBS_MUX_SEL
 7 CF_TX_SRC_ALTERNATE
 8 CF_TSTADC_EN
 9 CF_RX_SRC_IS_TSTADC
 10-13 RX_OBS_SEL
 14 DISABLE_A2_WARM_RESET
 15 RESET_A2
 16-18 AGC_OBS_SEL
 19 CF_ENABLE_FFT_DUMP
 23 CF_DEBUGPORT_IN
 27 DISABLE_AGC_TO_A2
 28 CF_DEBUGPORT_EN
 29-30 CF_DEBUGPORT_SEL |
| --- |

**ATH:02980Ch/029800h - BB_TIMING_CONTROLS_1 ;hw4/hw6**

```

| 0-6 STE_THR
 7-12 STE_TO_LONG1
 13-16 TIMING_BACKOFF
 17 ENABLE_HT_FINE_PPM
 18-19 HT_FINE_PPM_STREAM
 20-21 HT_FINE_PPM_QAM
 22 ENABLE_LONG_CHANFIL
 23 ENABLE_RX_STBC
 24 ENABLE_CHANNEL_FILTER
 25-26 FALSE_ALARM
 27 ENABLE_LONG_RESCALE
 28 TIMING_LEAK_ENABLE
 29-30 COARSE_PPM_SELECT
 31 FFT_SCALING |
| --- |

**ATH:029810h/029804h - BB_TIMING_CONTROLS_2 ;hw4/hw6**

```

| 0-11 FORCED_DELTA_PHI_SYMBOL
 12 FORCE_DELTA_PHI_SYMBOL
 13 ENABLE_MAGNITUDE_TRACK
 14 ENABLE_SLOPE_FILTER
 15 ENABLE_OFFSET_FILTER
 16-22 DC_OFF_DELTAF_THRES
 24-26 DC_OFF_TIM_CONST
 27 ENABLE_DC_OFFSET
 28 ENABLE_DC_OFFSET_TRACK
 29 ENABLE_WEIGHTING
 30 TRACEBACK128
 31 ENABLE_HT_FINE_TIMING |
| --- |

**ATH:029814h/029808h - BB_TIMING_CONTROLS_3 ;hw4/hw6**

```

| 0-7 PPM_RESCUE_INTERVAL
 8 ENABLE_PPM_RESCUE
 9 ENABLE_FINE_PPM
 10 ENABLE_FINE_INTERP
 11 CONTINUOUS_PPM_RESCUE
 12 ENABLE_DF_CHANEST
 13-16 DELTA_SLOPE_COEF_EXP
 17-31 DELTA_SLOPE_COEF_MAN |
| --- |

**ATH:029818h/02A200h - BB_D2_CHIP_ID (R) ;hw4/hw6**

```

| 0-7 OLD_ID (R)
 8-31 ID (R) |
| --- |

**ATH:02981Ch/02A20Ch - BB_ACTIVE ;hw4/hw6**

```

| 0 CF_ACTIVE |
| --- |

**ATH:029820h/02A258h - BB_TX_TIMING_1 ;hw4/hw6**

```

| 0-7 TX_FRAME_TO_ADC_OFF
 8-15 TX_FRAME_TO_A2_RX_OFF
 16-23 TX_FRAME_TO_DAC_ON
 24-31 TX_FRAME_TO_A2_TX_ON |
| --- |

**ATH:029824h/02A25Ch - BB_TX_TIMING_2 ;hw4/hw6**

```

| 0-7 TX_FRAME_TO_TX_D_START
 8-15 TX_FRAME_TO_PA_ON
 16-23 TX_END_TO_PA_OFF
 24-31 TX_END_TO_A2_TX_OFF |
| --- |

**ATH:029828h/02A260h - BB_TX_TIMING_3 ;hw4/hw6**

```

| 0-7 TX_END_TO_DAC_OFF
 8-15 TX_FRAME_TO_THERM_CHAIN_ON
 16-23 TX_END_TO_A2_RX_ON
 24-31 TX_END_TO_ADC_ON |
| --- |

**ATH:02982Ch/02A350h - BB_ADDAC_PARALLEL_CONTROL ;hw4/hw6**

```

| 12 OFF_DACLPMODE
 13 OFF_PWDDAC
 15 OFF_PWDADC
 28 ON_DACLPMODE
 29 ON_PWDDAC
 31 ON_PWDADC |
| --- |

**ATH:029834h/02A264h - BB_XPA_TIMING_CONTROL ;hw4/hw6**

```

| 0-7 TX_FRAME_TO_XPAA_ON
 8-15 TX_FRAME_TO_XPAB_ON
 16-23 TX_END_TO_XPAA_OFF
 24-31 TX_END_TO_XPAB_OFF |
| --- |

**ATH:029838h/02A280h - BB_MISC_PA_CONTROL ;hw4/hw6**

```

| 0 XPAA_ACTIVE_HIGH
 1 XPAB_ACTIVE_HIGH
 2 ENABLE_XPAA
 3 ENABLE_XPAB |
| --- |

**ATH:02983Ch/0298A4h - BB_TSTDAC_CONSTANT ;hw4/hw6**

```

| 0-10 CF_TSTDAC_CONSTANT_I
 11-21 CF_TSTDAC_CONSTANT_Q |
| --- |

**ATH:029840h/029820h - BB_FIND_SIGNAL_LOW ;hw4/hw6**

```

| 0-5 RELSTEP_LOW
 6-11 FIRSTEP_LOW
 12-19 FIRPWR_LOW
 20-23 YCOK_MAX_LOW
 24-30 LONG_SC_THRESH |
| --- |

**ATH:029844h/029E00h - BB_SETTLING_TIME ;hw4/hw6**

```

| 0-6 AGC_SETTLING
 7-13 SWITCH_SETTLING
 14-19 ADCSAT_THRL
 20-25 ADCSAT_THRH
 26-29 LBRESET_ADVANCE |
| --- |

**ATH:029848h/029E04h - BB_GAIN_FORCE_MAX_GAINS_B0 ;hw4/hw6**

**ATH:02A848h/02AE04h - BB_GAIN_FORCE_MAX_GAINS_B1 ;hw4/hw6**

```

| 7-13 hw4: XATTEN1_HYST_MARGIN_0/1 ;\ ;\separate settings in B0/B1
 14-20 hw4: XATTEN2_HYST_MARGIN_0/1 ; hw4 ;/
 21 hw4: GAIN_FORCE ; ;\global setting (not in B1)
 31 hw4: ENABLE_SHARED_RX ;/ ;/
 0-7 hw6: RF_GAIN_F_0/1 ;\ ;\
 8-15 hw6: MB_GAIN_F_0/1 ; ;
 16 hw6: XATTEN1_SW_F_0/1 ; hw6 ; separate settings in B0/B1
 17 hw6: XATTEN2_SW_F_0/1 ; ;
 18-24 hw6: XATTEN1_HYST_MARGIN_0/1 ; ;
 25-31 hw6: XATTEN2_HYST_MARGIN_0/1 ;/ ;/ |
| --- |

**ATH:02984Ch - BB_GAINS_MIN_OFFSETS_B0 ;hw4**

**ATH:02A84Ch - BB_GAINS_MIN_OFFSETS_B1 ;hw4**

**ATH:029E08h - BB_GAINS_MIN_OFFSETS ;hw6 (only global setting for B0 and B1)**

```

| 0-6 OFFSETC1 ;\ ;\global setting
 7-11 OFFSETC2 ; hw4/hw6 ; (not in B1 register)
 12-16 OFFSETC3 ;/ ;/
 17-24 hw4: RF_GAIN_F_0/1 ;\ ;\separate settings
 25 hw4: XATTEN1_SW_F_0/1 ; hw4 ; in B0/B1 registers
 26 hw4: XATTEN2_SW_F_0/1 ;/ ;/
 17 hw6: GAIN_FORCE ;\ ;\
 18 hw6: CF_AGC_HIST_ENABLE ; ; global setting
 19 hw6: CF_AGC_HIST_GC ; hw6 ; (hw6 doesn't have
 20 hw6: CF_AGC_HIST_VOTING ; ; a B1 register at all)
 21 hw6: CF_AGC_HIST_PHY_ERR ;/ ;/ |
| --- |

**ATH:029850h/029E0Ch - BB_DESIRED_SIGSIZE ;hw4/hw6**

```

| 0-7 ADC_DESIRED_SIZE
 20-27 TOTAL_DESIRED
 28-29 INIT_GC_COUNT_MAX
 30 REDUCE_INIT_GC_COUNT
 31 ENA_INIT_GAIN |
| --- |

**ATH:029854h/029C00h - BB_TIMING_CONTROL_3A ;hw4/hw6**

```

| 0-6 STE_THR_HI_RSSI ;-hw4/hw6
 7 hw6: USE_HTSIG1_20_40_BW_VALUE ;-hw6 |
| --- |

**ATH:029858h/029E10h - BB_FIND_SIGNAL ;hw4/hw6**

```

| 0-5 RELSTEP
 6-11 RELPWR
 12-17 FIRSTEP
 18-25 FIRPWR
 26-31 M1COUNT_MAX |
| --- |

**ATH:02985Ch/029E14h - BB_AGC ;hw4/hw6**

```

| 0-6 COARSEPWR_CONST
 7-14 COARSE_LOW
 15-21 COARSE_HIGH
 22-29 QUICK_DROP
 30-31 RSSI_OUT_SELECT |
| --- |

**ATH:029860h/02A2C4h - BB_AGC_CONTROL ;hw4/hw6**

```

| 0 DO_CALIBRATE
 1 DO_NOISEFLOOR
 3-5 MIN_NUM_GAIN_CHANGE
 6-9 YCOK_MAX
 10 LEAKY_BUCKET_ENABLE
 11 CAL_ENABLE
 12 USE_TABLE_SEED
 13 AGC_UPDATE_TABLE_SEED
 15 ENABLE_NOISEFLOOR
 16 ENABLE_FLTR_CAL
 17 NO_UPDATE_NOISEFLOOR
 18 EXTEND_NF_PWR_MEAS
 19 CLC_SUCCESS (R)
 20 ENABLE_PKDET_CAL |
| --- |

**ATH:029864h/029E1Ch - BB_CCA_B0 ;hw4/hw6**

**ATH:02AE1Ch - BB_CCA_B1 ;hw6**

```

| 0-8 CF_MAXCCAPWR_0 ;-separate settings (on hw6)
 9-11 CF_CCA_COUNT_MAXC ;\global setting (not in B1)
 12-19 CF_THRESH62 ;/
 20-28 MINCCAPWR_0 (R) ;-separate settings (on hw6) |
| --- |

**ATH:029868h/029824h - BB_SFCORR ;hw4/hw6**

```

| 0-4 M2COUNT_THR
 5-10 ADCSAT_THRESH
 11-16 ADCSAT_ICOUNT
 17-23 M1_THRES
 24-30 M2_THRES |
| --- |

**ATH:02986Ch/029828h - BB_SELF_CORR_LOW ;hw4/hw6**

```

| 0 USE_SELF_CORR_LOW
 1-7 M1COUNT_MAX_LOW
 8-13 M2COUNT_THR_LOW
 14-20 M1_THRESH_LOW
 21-27 M2_THRESH_LOW |
| --- |

**ATH:029874h/02A340h - BB_SYNTH_CONTROL ;hw4/hw6**

```

| 0-16 RFCHANFRAC
 17-25 RFCHANNEL
 26-27 RFAMODEREFSEL
 28 RFFRACMODE
 29 RFBMODE
 30 RFSYNTH_CTRL_SSHIFT |
| --- |

**ATH:029878h/02A344h - BB_ADDAC_CLK_SELECT ;hw4/hw6**

```

| 2-3 BB_DAC_CLK_SELECT
 4-5 BB_ADC_CLK_SELECT |
| --- |

**ATH:02987Ch/02A348h - BB_PLL_CNTL ;hw4/hw6**

```

| 0-9 BB_PLL_DIV
 10-13 BB_PLL_REFDIV
 14-15 BB_PLL_CLK_SEL
 16 BB_PLLBYPASS
 17-27 BB_PLL_SETTLE_TIME |
| --- |

**ATH:029900h/02A220h - BB_VIT_SPUR_MASK_A ;CF_PUNC_MASK_A ;hw4/hw6**

**ATH:029904h/02A224h - BB_VIT_SPUR_MASK_B ;CF_PUNC_MASK_B ;hw4/hw6**

```

| 0-9 CF_PUNC_MASK_A / CF_PUNC_MASK_B
 10-16 CF_PUNC_MASK_IDX_A / CF_PUNC_MASK_IDX_B |
| --- |

**ATH:029908h/029C0Ch - BB_PILOT_SPUR_MASK ;CF_PILOT_MASK_A/B ;hw4/hw6**

**ATH:02990Ch/029C10h - BB_CHAN_SPUR_MASK ;CF_CHAN_MASK_A/B ;hw4/hw6**

```

| 0-4 CF_PILOT_MASK_A / CF_CHAN_MASK_A
 5-11 CF_PILOT_MASK_IDX_A / CF_CHAN_MASK_IDX_A
 12-16 CF_PILOT_MASK_B / CF_CHAN_MASK_B
 17-23 CF_PILOT_MASK_IDX_B / CF_CHAN_MASK_IDX_B |
| --- |

**ATH:029910h/02A228h - BB_SPECTRAL_SCAN ;hw4/hw6**

```

| 0 SPECTRAL_SCAN_ENA
 1 SPECTRAL_SCAN_ACTIVE
 2 DISABLE_RADAR_TCTL_RST
 3 DISABLE_PULSE_COARSE_LOW
 4-7 SPECTRAL_SCAN_FFT_PERIOD
 8-15 SPECTRAL_SCAN_PERIOD
 16-27 SPECTRAL_SCAN_COUNT
 28 SPECTRAL_SCAN_SHORT_RPT
 29 SPECTRAL_SCAN_PRIORITY
 30 SPECTRAL_SCAN_USE_ERR5
 31 hw6: SPECTRAL_SCAN_COMPRESSED_RPT ;-hw6 |
| --- |

**ATH:02A248h - BB_SPECTRAL_SCAN_2 ;hw6**

```

| 0 hw6: SPECTRAL_SCAN_RPT_MODE ;\hw6
 1-8 hw6: SPECTRAL_SCAN_NOISE_FLOOR_REF ;/ |
| --- |

**ATH:029914h/02A254h - BB_ANALOG_POWER_ON_TIME ;hw4/hw6**

```

| 0-13 ACTIVE_TO_RECEIVE |
| --- |

**ATH:029918h/02A230h - BB_SEARCH_START_DELAY ;hw4/hw6**

```

| 0-11 SEARCH_START_DELAY
 12 ENABLE_FLT_SVD
 13 ENABLE_SEND_CHAN
 14 hw6: RX_SOUNDING_ENABLE
 15 hw6: RM_HCSD4SVD |
| --- |

**ATH:02991Ch/02A234h - BB_MAX_RX_LENGTH ;hw4/hw6**

```

| 0-11 MAX_RX_LENGTH
 12-29 MAX_HT_LENGTH |
| --- |

**ATH:029920h/02980Ch - BB_TIMING_CONTROL_4 ;hw4/hw6**

```

| 12-15 CAL_LG_COUNT_MAX
 16 DO_GAIN_DC_IQ_CAL
 17-20 USE_PILOT_TRACK_DF
 21-27 EARLY_TRIGGER_THR
 28 ENABLE_PILOT_MASK
 29 ENABLE_CHAN_MASK
 30 ENABLE_SPUR_FILTER
 31 ENABLE_SPUR_RSSI |
| --- |

**ATH:029924h/029810h - BB_TIMING_CONTROL_5 ;hw4/hw6**

```

| 0 ENABLE_CYCPWR_THR1
 1-7 CYCPWR_THR1
 15 ENABLE_RSSI_THR1A
 16-22 RSSI_THR1A
 23-29 LONG_SC_THRESH_HI_RSSI
 30 FORCED_AGC_STR_PRI
 31 FORCED_AGC_STR_PRI_EN |
| --- |

**ATH:029928h/02A7D0h - BB_PHYONLY_WARM_RESET ;hw4/hw6**

```

| 0 PHYONLY_RST_WARM_L |
| --- |

**ATH:02992Ch/02A7D4h - BB_PHYONLY_CONTROL ;hw4/hw6**

```

| 0 RX_DRAIN_RATE
 1 LATE_TX_SIGNAL_SYMBOL
 2 GENERATE_SCRAMBLER
 3 TX_ANTENNA_SELECT
 4 STATIC_TX_ANTENNA
 5 RX_ANTENNA_SELECT
 6 STATIC_RX_ANTENNA
 7 EN_LOW_FREQ_SLEEP |
| --- |

**ATH:02993Ch/02A3F0h - BB_POWERTX_MAX ;hw4/hw6**

```

| 6 USE_PER_PACKET_POWERTX_MAX
 7 hw6: USE_PER_PACKET_OLPC_GAIN_DELTA_ADJ |
| --- |

**ATH:029940h/02983Ch - BB_EXTENSION_RADAR ;hw4/hw6**

```

| 8-13 BLOCKER40_MAX_RADAR
 14 ENABLE_EXT_RADAR
 15-22 RADAR_DC_PWR_THRESH
 23-30 RADAR_LB_DC_CAP
 31 DISABLE_ADCSAT_HOLD |
| --- |

**ATH:029944h/02A238h - BB_FRAME_CONTROL ;hw4/hw6**

```

| 0-1 CF_OVERLAP_WINDOW
 2 CF_SCALE_SHORT
 3-5 CF_TX_CLIP
 6-7 CF_TX_DOUBLESAMP_DAC
 8-15 TX_END_ADJUST
 16 PREPEND_CHAN_INFO
 17 SHORT_HIGH_PAR_NORM
 18 EN_ERR_GREEN_FIELD
 19 hw4: EN_ERR_XR_POWER_RATIO
 19 hw6: EN_ERR_STATIC20_MODE_HT40_PACKET
 20 EN_ERR_OFDM_XCORR
 21 EN_ERR_LONG_SC_THR
 22 EN_ERR_TIM_LONG1
 23 EN_ERR_TIM_EARLY_TRIG
 24 EN_ERR_TIM_TIMEOUT
 25 EN_ERR_SIGNAL_PARITY
 26 EN_ERR_RATE_ILLEGAL
 27 EN_ERR_LENGTH_ILLEGAL
 28 hw4: EN_ERR_HT_SERVICE
 28 hw6: NO_6MBPS_SERVICE_ERR
 29 EN_ERR_SERVICE
 30 EN_ERR_TX_UNDERRUN
 31 EN_ERR_RX_ABORT |
| --- |

**ATH:029948h/029814h - BB_TIMING_CONTROL_6 ;hw4/hw6**

```

| 0-7 HI_RSSI_THRESH
 8-14 EARLY_TRIGGER_THR_HI_RSSI
 15-20 OFDM_XCORR_THRESH
 21-27 OFDM_XCORR_THRESH_HI_RSSI
 28-31 LONG_MEDIUM_RATIO_THR |
| --- |

**ATH:02994Ch/02981Ch - BB_SPUR_MASK_CONTROLS ;hw4/hw6**

```

| 0-7 SPUR_RSSI_THRESH
 8 EN_VIT_SPUR_RSSI
 17 ENABLE_MASK_PPM
 18-25 MASK_RATE_CNTL
 26 hw6: ENABLE_NF_RSSI_SPUR_MIT |
| --- |

**ATH:029950h/0298DCh - BB_RX_IQ_CORR_B0 ;hw4/hw6**

**ATH:02A8DCh - BB_RX_IQ_CORR_B1 ;hw6**

```

| 0-6 RX_IQCORR_Q_Q_COFF_0/1 ;\separate settings (on hw6)
 7-13 RX_IQCORR_Q_I_COFF_0/1 ;/
 14 RX_IQCORR_ENABLE ;-global setting (not in B1)
 15-21 LOOPBACK_IQCORR_Q_Q_COFF_0/1 ;\separate settings (on hw6)
 22-28 LOOPBACK_IQCORR_Q_I_COFF_0/1 ;/
 29 LOOPBACK_IQCORR_ENABLE ;-global setting (not in B1) |
| --- |

**ATH:029954h/029834h - BB_RADAR_DETECTION ;hw4/hw6**

```

| 0 PULSE_DETECT_ENABLE
 1-5 PULSE_IN_BAND_THRESH
 6-11 PULSE_RSSI_THRESH
 12-17 PULSE_HEIGHT_THRESH
 18-23 RADAR_RSSI_THRESH
 24-30 RADAR_FIRPWR_THRESH
 31 ENABLE_RADAR_FFT |
| --- |

**ATH:029958h/029838h - BB_RADAR_DETECTION_2 ;hw4/hw6**

```

| 0-7 RADAR_LENGTH_MAX
 8-12 PULSE_RELSTEP_THRESH
 13 ENABLE_PULSE_RELSTEP_CHECK
 14 ENABLE_MAX_RADAR_RSSI
 15 ENABLE_BLOCK_RADAR_CHECK
 16-21 RADAR_RELPWR_THRESH
 22 RADAR_USE_FIRPWR_128
 23 ENABLE_RADAR_RELPWR_CHECK
 24-26 CF_RADAR_BIN_THRESH_SEL
 27 ENABLE_PULSE_GC_COUNT_CHECK |
| --- |

**ATH:02995Ch/0298D0h - BB_TX_PHASE_RAMP_B0 ;hw4/hw6**

**ATH:02A8D0h - BB_TX_PHASE_RAMP_B1 ;hw6**

```

| 0 CF_PHASE_RAMP_ENABLE
 1-6 CF_PHASE_RAMP_BIAS
 7-16 CF_PHASE_RAMP_INIT
 17-24 CF_PHASE_RAMP_ALPHA |
| --- |

**ATH:029960h/02A284h - BB_SWITCH_TABLE_CHN_B0 ;hw4/hw6**

**ATH:02B284h - BB_SWITCH_TABLE_CHN_B1 ;hw6**

```

| 0-1 SWITCH_TABLE_IDLE
 2-3 SWITCH_TABLE_T
 4-5 SWITCH_TABLE_R
 6-7 SWITCH_TABLE_RX1
 8-9 SWITCH_TABLE_RX12
 10-11 SWITCH_TABLE_B |
| --- |

**ATH:029964h/02A288h - BB_SWITCH_TABLE_COM1 ;hw4/hw6**

```

| 0-3 SWITCH_TABLE_COM_IDLE
 4-7 SWITCH_TABLE_COM_T1
 8-11 SWITCH_TABLE_COM_T2
 12-15 SWITCH_TABLE_COM_B
 16-19 hw6: SWITCH_TABLE_COM_IDLE_ALT ;\hw6
 20-23 hw6: SWITCH_TABLE_COM_SPDT ;/ |
| --- |

**ATH:029968h/029E20h - BB_CCA_CTRL_2_B0 ;hw4/hw6**

**ATH:02AE20h - BB_CCA_CTRL_2_B1 ;hw6**

```

| 0-8 MINCCAPWR_THR_0/1 ;-separate settings (on hw6)
 9 ENABLE_MINCCAPWR_THR ;-global setting (not in B1)
 10-17 NF_GAIN_COMP_0/1 ;-separate settings (on hw6)
 18 THRESH62_MODE ;-global setting (not in B1) |
| --- |

**ATH:02996Ch/02A28Ch - BB_SWITCH_TABLE_COM2 ;hw4/hw6**

```

| 0-3 hw4: SWITCH_TABLE_COM_RA1NXAL1 ;\
 4-7 hw4: SWITCH_TABLE_COM_RA2NXAL1 ;
 8-11 hw4: SWITCH_TABLE_COM_RA1XAL1 ;
 12-15 hw4: SWITCH_TABLE_COM_RA2XAL1 ; hw4
 16-19 hw4: SWITCH_TABLE_COM_RA1NXAL2 ;
 20-23 hw4: SWITCH_TABLE_COM_RA2NXAL2 ;
 24-27 hw4: SWITCH_TABLE_COM_RA1XAL2 ;
 28-31 hw4: SWITCH_TABLE_COM_RA2XAL2 ;/
 0-3 hw6: SWITCH_TABLE_COM_RA1L1 ;\
 4-7 hw6: SWITCH_TABLE_COM_RA2L1 ;
 8-11 hw6: SWITCH_TABLE_COM_RA1L2 ; hw6
 12-15 hw6: SWITCH_TABLE_COM_RA2L2 ;
 16-19 hw6: SWITCH_TABLE_COM_RA12 ;/ |
| --- |

**ATH:029970h/029E24h - BB_RESTART ;hw4/hw6**

```

| 0 ENABLE_RESTART
 1-5 RESTART_LGFIRPWR_DELTA
 6 ENABLE_PWR_DROP_ERR
 7-11 PWRDROP_LGFIRPWR_DELTA
 12-17 OFDM_CCK_RSSI_BIAS
 18-20 ANT_FAST_DIV_GC_LIMIT
 21 ENABLE_ANT_FAST_DIV_M2FLAG
 22-28 WEAK_RSSI_VOTE_THR
 29 ENABLE_PWR_DROP_ERR_CCK
 30 DISABLE_DC_RESTART
 31 RESTART_MODE_BW40 |
| --- |

**ATH:029978h/02A390h - BB_SCRAMBLER_SEED ;hw4/hw6**

```

| 0-6 FIXED_SCRAMBLER_SEED |
| --- |

**ATH:02997Ch/02A23Ch - BB_RFBUS_REQUEST ;hw4/hw6**

```

| 0 RFBUS_REQUEST |
| --- |

**ATH:0299A0h/029818h - BB_TIMING_CONTROL_11 ;hw4/hw6**

```

| 0-19 SPUR_DELTA_PHASE
 20-29 SPUR_FREQ_SD
 30 USE_SPUR_FILTER_IN_AGC
 31 USE_SPUR_FILTER_IN_SELFCOR |
| --- |

**ATH:0299A4h/02A2A0h - BB_MULTICHAIN_ENABLE ;hw4/hw6**

```

| 0-2 RX_CHAIN_MASK |
| --- |

**ATH:0299A8h/029880h - BB_MULTICHAIN_CONTROL ;hw4/hw6**

```

| 0 FORCE_ANALOG_GAIN_DIFF
 1-7 FORCED_GAIN_DIFF_01
 8 SYNC_SYNTHON
 9 USE_POSEDGE_REFCLK
 10-20 CF_SHORT_SAT
 22-28 FORCED_GAIN_DIFF_02
 29 FORCE_SIGMA_ZERO |
| --- |

**ATH:0299ACh/029E28h - BB_MULTICHAIN_GAIN_CTRL ;hw4/hw6**

```

| 0-7 QUICKDROP_LOW
 8 ENABLE_CHECK_STRONG_ANT
 9-14 ANT_FAST_DIV_BIAS
 15-20 CAP_GAIN_RATIO_SNR
 21 CAP_GAIN_RATIO_ENA
 22 CAP_GAIN_RATIO_MODE
 23 ENABLE_ANT_SW_RX_PROT
 24 ENABLE_ANT_DIV_LNADIV
 25-26 ANT_DIV_ALT_LNACONF
 27-28 ANT_DIV_MAIN_LNACONF
 29 ANT_DIV_ALT_GAINTB
 30 ANT_DIV_MAIN_GAINTB |
| --- |

**ATH:0299B4h/0298D4h - BB_ADC_GAIN_DC_CORR_B0 ;hw4/hw6**

**ATH:02A8D4h - BB_ADC_GAIN_DC_CORR_B1 ;hw6**

```

| 0-5 ADC_GAIN_CORR_Q_COEFF_0/1 ;\
 6-11 ADC_GAIN_CORR_I_COEFF_0/1 ; separate settings (on hw6)
 12-20 ADC_DC_CORR_Q_COEFF_0/1 ;
 21-29 ADC_DC_CORR_I_COEFF_0/1 ;/
 30 ADC_GAIN_CORR_ENABLE ;\global setting (not in B1)
 31 ADC_DC_CORR_ENABLE ;/ |
| --- |

**ATH:0299B8h/029E2Ch - BB_EXT_CHAN_PWR_THR_1 ;hw4/hw6**

```

| 0-7 THRESH62_EXT
 8-15 ANT_DIV_ALT_ANT_MINGAINIDX
 16-20 ANT_DIV_ALT_ANT_DELTAGAINIDX
 21-26 ANT_DIV_ALT_ANT_DELTANF |
| --- |

**ATH:0299BCh/029830h - BB_EXT_CHAN_PWR_THR_2_B0 ;hw4/hw6**

**ATH:02A830h - BB_EXT_CHAN_PWR_THR_2_B1 ;hw6**

```

| 0-8 CF_MAXCCAPWR_EXT_0/1 ;-separate settings (on hw6)
 9-15 CYCPWR_THR1_EXT ;-global setting (not in B1)
 16-24 MINCCAPWR_EXT_0/1 (R) ;-separate settings (on hw6) |
| --- |

**ATH:0299C0h/02982Ch - BB_EXT_CHAN_SCORR_THR ;hw4/hw6**

```

| 0-6 M1_THRES_EXT
 7-13 M2_THRES_EXT
 14-20 M1_THRES_LOW_EXT
 21-27 M2_THRES_LOW_EXT
 28 SPUR_SUBCHANNEL_SD |
| --- |

**ATH:0299C4h/029E30h - BB_EXT_CHAN_DETECT_WIN ;hw4/hw6**

```

| 0-3 DET_DIFF_WIN_WEAK
 4-7 DET_DIFF_WIN_WEAK_LOW
 8-12 DET_DIFF_WIN_WEAK_CCK
 13-15 DET_20H_COUNT
 16-18 DET_EXT_BLK_COUNT
 19-24 WEAK_SIG_THR_CCK_EXT
 25-28 DET_DIFF_WIN_THRESH |
| --- |

**ATH:0299C8h/029E34h - BB_PWR_THR_20_40_DET ;hw4/hw6**

```

| 0-4 PWRDIFF40_THRSTR
 5-10 BLOCKER40_MAX
 11-15 DET40_PWRSTEP_MAX
 16-23 DET40_THR_SNR
 24-28 DET40_PRI_BIAS
 29 PWRSTEP40_ENA
 30 LOWSNR40_ENA |
| --- |

**ATH:0299D0h/029C14h - BB_SHORT_GI_DELTA_SLOPE ;hw4/hw6**

```

| 0-3 DELTA_SLOPE_COEF_EXP_SHORT_GI
 4-18 DELTA_SLOPE_COEF_MAN_SHORT_GI |
| --- |

**ATH:0299DCh/02A370h - BB_CHANINFO_CTRL ;hw4/hw6**

```

| 0 CAPTURE_CHAN_INFO
 1 DISABLE_CHANINFOMEM
 2 hw6: CAPTURE_SOUNDING_PACKET
 3 hw6: CHANINFOMEM_S2_READ |
| --- |

**ATH:0299E0h/02A3A4h - BB_HEAVY_CLIP_CTRL ;hw4/hw6**

```

| 0-8 CF_HEAVY_CLIP_ENABLE
 9 PRE_EMP_HT40_ENABLE
 10-17 hw6: HEAVY_CLIP_FACTOR_XR ;-hw6 (moved from hw4's BB_RIFS_SRCH) |
| --- |

**ATH:0299E4h/02A3A8h - BB_HEAVY_CLIP_20 ;FACTOR_0..3 ;hw4/hw6**

**ATH:0299E8h/02A3ACh - BB_HEAVY_CLIP_40 ;FACTOR_4..7 ;hw4/hw6**

```

| 0-7 HEAVY_CLIP FACTOR_0 / FACTOR_4
 8-15 HEAVY_CLIP FACTOR_1 / FACTOR_5
 16-23 HEAVY_CLIP FACTOR_2 / FACTOR_6
 24-31 HEAVY_CLIP FACTOR_3 / FACTOR_7 |
| --- |

**ATH:0299ECh/029E38h - BB_RIFS_SRCH ;hw4/hw6**

```

| 0-7 hw4: HEAVY_CLIP_FACTOR_XR ;-hw4 (moved to BB_HEAVY_CLIP_CTRL in hw6)
 8-15 INIT_GAIN_DB_OFFSET
 16-25 RIFS_INIT_DELAY
 26 RIFS_DISABLE_PWRLOW_GC
 27 RIFS_DISABLE_CCK_DET |
| --- |

**ATH:0299F0h/02A2C8h - BB_IQ_ADC_CAL_MODE ;hw4/hw6**

```

| 0-1 GAIN_DC_IQ_CAL_MODE
 2 TEST_CALADCOFF |
| --- |

**ATH:0299FCh/029884h - BB_PER_CHAIN_CSD ;hw4/hw6**

```

| 0-4 CSD_CHN1_2CHAINS
 5-9 CSD_CHN1_3CHAINS
 10-14 CSD_CHN2_3CHAINS |
| --- |

**ATH:029A00h..029BFCh - BB_RX_OCGAIN[0..127] (W) ;hw4**

**ATH:02A000h..02A1FCh - BB_RX_OCGAIN[0..127] (R/W?) ;hw6**

**ATH:02AA00h..02ABFCh - BB_RX_OCGAIN2[0..127] (W) ;hw4**

**ATH:02B000h..02B1FCh - BB_RX_OCGAIN2[0..127] (R/W?) ;hw6**

```

| 0-31 GAIN_ENTRY |
| --- |

**ATH:029C00h/0298A0h - BB_TX_CRC (R) ;hw4/hw6**

```

| 0-15 TX_CRC (R) |
| --- |

**ATH:029C10h/0298C0h - BB_IQ_ADC_MEAS_0_B0 (R) ;hw4/hw6**

**ATH:029C14h/0298C4h - BB_IQ_ADC_MEAS_1_B0 (R) ;hw4/hw6**

**ATH:029C18h/0298C8h - BB_IQ_ADC_MEAS_2_B0 (R) ;hw4/hw6**

**ATH:029C1Ch/0298CCh - BB_IQ_ADC_MEAS_3_B0 (R) ;hw4/hw6**

**ATH:02A8C0h - BB_IQ_ADC_MEAS_0_B1 (R) ;hw6**

**ATH:02A8C4h - BB_IQ_ADC_MEAS_1_B1 (R) ;hw6**

**ATH:02A8C8h - BB_IQ_ADC_MEAS_2_B1 (R) ;hw6**

**ATH:02A8CCh - BB_IQ_ADC_MEAS_3_B1 (R) ;hw6**

```

| 0-31 GAIN_DC_IQ_CAL_MEAS (R) |
| --- |

**ATH:029C20h/02A240h - BB_RFBUS_GRANT (R) ;hw4/hw6**

```

| 0 RFBUS_GRANT (R)
 1 BT_ANT (R) |
| --- |

**ATH:029C24h/029C20h - BB_TSTADC (R) ;hw4/hw6**

```

| 0-9 TSTADC_OUT_Q (R)
 10-19 TSTADC_OUT_I (R) |
| --- |

**ATH:029C28h/02A368h - BB_TSTDAC (R) ;hw4/hw6**

```

| 0-9 TSTDAC_OUT_Q (R)
 10-19 TSTDAC_OUT_I (R) |
| --- |

**ATH:029C30h/02A3B0h - BB_ILLEGAL_TX_RATE (R) ;hw4/hw6**

```

| 0 ILLEGAL_TX_RATE (R) |
| --- |

**ATH:029C34h/0298A8h - BB_SPUR_REPORT_B0 (R) ;hw4/hw6**

**ATH:02A8A8h - BB_SPUR_REPORT_B1 (R) ;hw6**

```

| 0-7 SPUR_EST_I (R)
 8-15 SPUR_EST_Q (R)
 16-31 POWER_WITH_SPUR_REMOVED (R) |
| --- |

**ATH:029C38h/02A36Ch - BB_CHANNEL_STATUS (R) ;hw4/hw6**

```

| 0 BT_ACTIVE (R)
 1 RX_CLEAR_RAW (R)
 2 RX_CLEAR_MAC (R)
 3 RX_CLEAR_PAD (R)
 4-5 BB_SW_OUT_0 (R)
 6-7 BB_SW_OUT_1 (R)
 8-9 BB_SW_OUT_2 (R)
 10-13 BB_SW_COM_OUT (R)
 14-16 ANT_DIV_CFG_USED (R) |
| --- |

**ATH:029C3Ch/029F80h - BB_RSSI_B0 (R) ;hw4/hw6**

**ATH:02AF80h - BB_RSSI_B1 (R) ;hw6**

**ATH:02CF80h - BB_RSSI_B3 (R) ;hw6**

```

| 0-7 RSSI (R)
 8-15 RSSI_EXT (R) |
| --- |

**ATH:029C40h/029F84h - BB_SPUR_EST_CCK_REPORT_B0 (R) ;hw4/hw6**

**ATH:02AF84h - BB_SPUR_EST_CCK_REPORT_B1 (R) ;hw6**

```

| 0-7 SPUR_EST_SD_I_CCK (R)
 8-15 SPUR_EST_SD_Q_CCK (R)
 16-23 SPUR_EST_I_CCK (R)
 24-31 SPUR_EST_Q_CCK (R) |
| --- |

**ATH:029CF0h/029CACh/02A374h - BB_CHAN_INFO_NOISE_PWR ;hw4/hw4.2/hw6**

```

| 0-11 NOISE_POWER (R) |
| --- |

**ATH:029CF4h/029CB0h/02A378h - BB_CHAN_INFO_GAIN_DIFF ;hw4/hw4.2/hw6**

```

| 0-11 FINE_PPM (R)
 12-18 hw6: ANALOG_GAIN_DIFF_01 (R) ;-hw6 |
| --- |

**ATH:029CF8h/029CB4h/02A37Ch - BB_CHAN_INFO_FINE_TIMING ;hw4/hw4.2/hw6**

```

| 0-11 COARSE_PPM (R)
 12-21 FINE_TIMING (R) |
| --- |

**ATH:029CFCh/029CB8h/02A380h - BB_CHAN_INFO_GAIN_B0 ;hw4/hw4.2/hw6**

**ATH:02B380h - BB_CHAN_INFO_GAIN_B1 ;hw6**

```

| 0-7 CHAN_INFO_RSSI (R)
 8-15 CHAN_INFO_RF_GAIN (R)
 16 hw4: CHAN_INFO_XATTEN1_SW (R) ;\hw4 (and hw4.2)
 17 hw4: CHAN_INFO_XATTEN2_SW (R) ;/
 16-22 hw6: CHAN_INFO_MB_GAIN (R) ;\
 23 hw6: CHAN_INFO_XATTEN1_SW (R) ; hw6
 24 hw6: CHAN_INFO_XATTEN2_SW (R) ;/ |
| --- |

**ATH:029D00h.. - BB_CHAN_INFO_CHAN_TAB_B0[0..59] ;hw4**

**ATH:029CBCh.. - BB_CHAN_INFO_CHAN_TAB_B0[0..59] ;hw4.2**

**ATH:029B00h.. - BB_CHAN_INFO_CHAN_TAB_B0[0..59] ;hw6**

**ATH:02AB00h.. - BB_CHAN_INFO_CHAN_TAB_B1[0..59] ;hw6**

```

| 0-5 hw4: MAN_Q_0 ;\ (R) ;\
 6-11 hw4: MAN_I_0 ; aka B0 ? (R) ;
 12-15 hw4: EXP_0 ;/ (R) ; hw4 (and hw4.2)
 16-21 hw4: MAN_Q_1 ;\ (R) ;
 22-27 hw4: MAN_I_1 ; aka B1 ? (R) ;
 28-31 hw4: EXP_1 ;/ (R) ;/
 0-31 hw6: CHANINFO_WORD (R) ;-hw6 |
| --- |

**ATH:02A000h/029E3Ch - BB_PEAK_DET_CTRL_1 ;hw4/hw6**

```

| 0 USE_OC_GAIN_TABLE
 1 USE_PEAK_DET
 2-7 PEAK_DET_WIN_LEN
 8-12 PEAK_DET_TALLY_THR_LOW(_0)
 13-17 PEAK_DET_TALLY_THR_MED(_0)
 18-22 PEAK_DET_TALLY_THR_HIGH(_0)
 23-29 PEAK_DET_SETTLING
 30 PWD_PKDET_DURING_CAL
 31 PWD_PKDET_DURING_RX |
| --- |

**ATH:02A004h/029E40h - BB_PEAK_DET_CTRL_2 ;hw4/hw6**

```

| 0-9 RFSAT_2_ADD_RFGAIN_DEL
 10-14 RF_GAIN_DROP_DB_LOW(_0)
 15-19 RF_GAIN_DROP_DB_MED(_0)
 20-24 RF_GAIN_DROP_DB_HIGH(_0)
 25-29 RF_GAIN_DROP_DB_NON(_0)
 30 hw6: ENABLE_RFSAT_RESTART |
| --- |

**ATH:02A008h/029E44h - BB_RX_GAIN_BOUNDS_1 ;hw4/hw6**

```

| 0-7 RX_MAX_MB_GAIN
 8-15 RX_MAX_RF_GAIN_REF
 16-23 RX_MAX_RF_GAIN
 24 RX_OCGAIN_SEL_2G
 25 RX_OCGAIN_SEL_5G |
| --- |

**ATH:02A00Ch/029E48h - BB_RX_GAIN_BOUNDS_2 ;hw4/hw6**

```

| 0-7 GC_RSSI_LOW_DB
 8-15 RF_GAIN_REF_BASE_ADDR
 16-23 RF_GAIN_BASE_ADDR
 24-31 RF_GAIN_DIV_BASE_ADDR |
| --- |

**ATH:02A010h/029E4Ch - BB_PEAK_DET_CAL_CTRL ;hw4/hw6**

```

| 0-5 PKDET_CAL_WIN_THR
 6-11 PKDET_CAL_BIAS
 12-13 PKDET_CAL_MEAS_TIME_SEL |
| --- |

**ATH:02A014h/029E50h - BB_AGC_DIG_DC_CTRL ;hw4/hw6**

```

| 0 USE_DIG_DC
 1-3 DIG_DC_SCALE_BIAS
 4-9 DIG_DC_CORRECT_CAP
 10 hw6: DIG_DC_SWITCH_CCK ;-hw6
 16-31 DIG_DC_MIXER_SEL_MASK |
| --- |

**ATH:02A018h/029F88h - BB_AGC_DIG_DC_STATUS_I_B0 (R) ;DIG_DC RES_I_0 ;hw4/hw6**

**ATH:02A01Ch/029F8Ch - BB_AGC_DIG_DC_STATUS_Q_B0 (R) ;DIG_DC RES_Q_0 ;hw4/hw6**

**ATH:02AF88h - BB_AGC_DIG_DC_STATUS_I_B1 (R) ;hw6**

**ATH:02AF8Ch - BB_AGC_DIG_DC_STATUS_Q_B1 (R) ;hw6**

```

| 0-8 DIG_DC_C1 RES_I_0 / RES_Q_0 / RES_I_1 / RES_Q_1 (R)
 9-17 DIG_DC_C2 RES_I_0 / RES_Q_0 / RES_I_1 / RES_Q_1 (R)
 18-26 DIG_DC_C3 RES_I_0 / RES_Q_0 / RES_I_1 / RES_Q_1 (R) |
| --- |

**ATH:02A1F4h/02A398h - BB_BBB_TXFIR_0 ;TXFIR_COEFF_H0..H3 ;hw4/hw6**

**ATH:02A1F8h/02A39Ch - BB_BBB_TXFIR_1 ;TXFIR_COEFF_H4..H7 ;hw4/hw6**

**ATH:02A1FCh/02A3A0h - BB_BBB_TXFIR_2 ;TXFIR_COEFF_H8..H11 ;hw4/hw6**

```

| 0-7 TXFIR COEFF_H0 (4bit) / COEFF_H4 (6bit) / COEFF_H8 (8bit)
 8-15 TXFIR COEFF_H1 (4bit) / COEFF_H5 (6bit) / COEFF_H9 (8bit)
 16-23 TXFIR COEFF_H2 (5bit) / COEFF_H6 (7bit) / COEFF_H10 (8bit)
 24-31 TXFIR COEFF_H3 (5bit) / COEFF_H7 (7bit) / COEFF_H11 (8bit) |
| --- |

Note: The entries are aligned to 8bit boundaries, but not all entries are 8bit
wide (eg. COEFF H0 and H1 are located in bit0-3 and bit8-11, with bit4-7 left
unused).

**ATH:02A200h/02A208h - BB_MODES_SELECT ;hw4/hw6**

```

| 0 CCK_MODE
 2 DYN_OFDM_CCK_MODE
 5 HALF_RATE_MODE
 6 QUARTER_RATE_MODE
 7 MAC_CLK_MODE
 8 DISABLE_DYN_CCK_DET
 9 hw6: SVD_HALF_RATE_MODE ;\hw6
 10 hw6: DISABLE_DYN_FAST_ADC ;/ |
| --- |

**ATH:02A204h/02A394h - BB_BBB_TX_CTRL ;hw4/hw6**

```

| 0 DISABLE_SCRAMBLER
 1 USE_SCRAMBLER_SEED
 2-3 TX_DAC_SCALE_CCK
 4 TXFIR_JAPAN_CCK
 5 ALLOW_1MBPS_SHORT
 6-8 TX_CCK_DELAY_1
 9-11 TX_CCK_DELAY_2 |
| --- |

**ATH:02A208h/029FC0h - BB_BBB_SIG_DETECT ;hw4/hw6**

```

| 0-5 WEAK_SIG_THR_CCK
 6-12 ANT_SWITCH_TIME
 13 ENABLE_ANT_FAST_DIV
 14 LB_ALPHA_128_CCK
 15 LB_RX_ENABLE_CCK
 16 CYC32_COARSE_DC_EST_CCK
 17 CYC64_COARSE_DC_EST_CCK
 18 ENABLE_COARSE_DC_CCK
 19 CYC256_FINE_DC_EST_CCK
 20 ENABLE_FINE_DC_CCK
 21 DELAY_START_SYNC_CCK
 22 USE_DC_EST_DURING_SRCH
 23 hw6: BBB_MRC_OFF_NO_SWAP ;\hw6
 24 hw6: SWAP_DEFAULT_CHAIN_CCK ;/
 31 ENABLE_BARKER_TWO_PHASE |
| --- |

**ATH:02A20Ch/029E18h - BB_EXT_ATTEN_SWITCH_CTL_B0 ;hw4/hw6**

**ATH:02B20Ch/02AE18h - BB_EXT_ATTEN_SWITCH_CTL_B1 ;hw4/hw6**

```

| 0-5 XATTEN1_DB
 6-11 XATTEN2_DB
 12-16 XATTEN1_MARGIN
 17-21 XATTEN2_MARGIN
 22-26 hw6: XLNA_GAIN_DB ;-hw6 |
| --- |

**ATH:02A210h/029D00h - BB_BBB_RX_CTRL_1 ;hw4/hw6**

```

| 0-2 COARSE_TIM_THRESHOLD_2
 3-7 COARSE_TIM_THRESHOLD
 8-10 COARSE_TIM_N_SYNC
 11-15 MAX_BAL_LONG
 16-20 MAX_BAL_SHORT
 21-23 RECON_LMS_STEP
 24-30 SB_CHECK_WIN
 31 EN_RX_ABORT_CCK |
| --- |

**ATH:02A214h/029D04h - BB_BBB_RX_CTRL_2 ;hw4/hw6**

```

| 0-5 FREQ_EST_N_AVG_LONG
 6-11 CHAN_AVG_LONG
 12-16 COARSE_TIM_THRESHOLD_3
 17-21 FREQ_TRACK_UPDATE_PERIOD
 22-25 FREQ_EST_SCALING_PERIOD
 26-31 LOOP_COEF_DPSK_C2_DATA |
| --- |

**ATH:02A218h/029D08h - BB_BBB_RX_CTRL_3 ;hw4/hw6**

```

| 0-7 TIM_ADJUST_FREQ_DPSK
 8-15 TIM_ADJUST_FREQ_CCK
 16-23 TIMER_N_SFD |
| --- |

**ATH:02A21Ch/029D0Ch - BB_BBB_RX_CTRL_4 ;hw4/hw6**

```

| 0-3 TIMER_N_SYNC
 4-15 TIM_ADJUST_TIMER_EXP
 16 FORCE_UNLOCKED_CLOCKS
 17 DYNAMIC_PREAM_SEL
 18 SHORT_PREAMBLE
 19-24 FREQ_EST_N_AVG_SHORT
 25-30 CHAN_AVG_SHORT
 31 hw6: USE_MRC_WEIGHT ;-hw6 |
| --- |

**ATH:02A220h/029D10h - BB_BBB_RX_CTRL_5 ;hw4/hw6**

```

| 0-4 LOOP_COEF_DPSK_C1_DATA
 5-9 LOOP_COEF_DPSK_C1_HEAD
 10-15 LOOP_COEF_DPSK_C2_HEAD
 16-20 LOOP_COEF_CCK_C1
 21-26 LOOP_COEF_CCK_C2 |
| --- |

**ATH:02A224h/029D14h - BB_BBB_RX_CTRL_6 ;hw4/hw6**

```

| 0-9 SYNC_START_DELAY
 10 MAP_1S_TO_2S
 11-20 START_IIR_DELAY
 21 hw6: USE_MCORR_WEIGHT ;\
 22 hw6: USE_BKPWR_FOR_CENTER_INDEX ;
 23 hw6: CCK_SEL_CHAIN_BY_EO ; hw6
 24 hw6: FORCE_CCK_SEL_CHAIN ;
 25 hw6: FORCE_CENTER_INDEX ;/ |
| --- |

**ATH:02A228h/029FC4h - BB_BBB_DAGC_CTRL ;hw4/hw6**

```

| 0 ENABLE_DAGC_CCK
 1-8 DAGC_TARGET_PWR_CCK
 9 ENABLE_BARKER_RSSI_THR
 10-16 BARKER_RSSI_THR
 17 ENABLE_FIRSTEP_SEL
 18-23 FIRSTEP_2
 24-27 FIRSTEP_COUNT_LGMAX
 28-29 hw6: FORCE_RX_CHAIN_CCK_0 ;\hw6
 30-31 hw6: FORCE_RX_CHAIN_CCK_1 ;/ |
| --- |

**ATH:02A22Ch/029D18h - BB_FORCE_CLKEN_CCK ;hw4/hw6**

```

| 0 FORCE_RX_ENABLE0
 1 FORCE_RX_ENABLE1
 2 FORCE_RX_ENABLE2
 3 FORCE_RX_ENABLE3
 4 FORCE_RX_ALWAYS
 5 FORCE_TXSM_CLKEN |
| --- |

**ATH:02A230h/02A250h - BB_RX_CLEAR_DELAY ;hw4/hw6**

```

| 0-9 OFDM_XR_RX_CLEAR_DELAY |
| --- |

**ATH:02A240h/029FCCh - BB_CCK_SPUR_MIT ;hw4/hw6**

```

| 0 USE_CCK_SPUR_MIT
 1-8 SPUR_RSSI_THR
 9-28 CCK_SPUR_FREQ
 29-30 SPUR_FILTER_TYPE |
| --- |

**ATH:02A244h/02A7C0h - BB_PANIC_WATCHDOG_STATUS/BB_WATCHDOG_STATUS ;hw4/hw6**

```

| 0-2 (PANIC_)WATCHDOG_STATUS_1
 3 hw4: (PANIC_)WATCHDOG_DET_HANG ;-hw4
 3 hw6: (PANIC_)WATCHDOG_TIMEOUT ;-hw6
 4-7 (PANIC_)WATCHDOG_STATUS_2
 8-11 (PANIC_)WATCHDOG_STATUS_3
 12-15 (PANIC_)WATCHDOG_STATUS_4
 16-19 (PANIC_)WATCHDOG_STATUS_5
 20-23 (PANIC_)WATCHDOG_STATUS_6
 24-27 (PANIC_)WATCHDOG_STATUS_7
 28-31 (PANIC_)WATCHDOG_STATUS_8 |
| --- |

**ATH:02A248h/02A7C4h - BB_PANIC_WATCHDOG_CTRL_1/BB_WATCHDOG_CTRL_1 ;hw4/hw6**

```

| 0 ENABLE_(PANIC_)WATCHDOG_(TIMEOUT_RESET_)NON_IDLE
 1 ENABLE_(PANIC_)WATCHDOG_(TIMEOUT_RESET_)IDLE
 2-15 (PANIC_)WATCHDOG_(TIMEOUT_RESET_)NON_IDLE_LIMIT
 16-31 (PANIC_)WATCHDOG_(TIMEOUT_RESET_)IDLE_LIMIT |
| --- |

**ATH:02A24Ch/02A7C8h - BB_PANIC_WATCHDOG_CTRL_2/BB_WATCHDOG_CTRL_2 ;hw4/hw6**

```

| 0 FORCE_FAST_ADC_CLK
 1 (PANIC_)WATCHDOG_(TIMEOUT_)RESET_ENA
 2 (PANIC_)WATCHDOG_IRQ_ENA |
| --- |

**ATH:02A250h/029FC8h - BB_IQCORR_CTRL_CCK ;hw4/hw6**

```

| 0-4 IQCORR_Q_Q_COFF_CCK
 5-10 IQCORR_Q_I_COFF_CCK
 11 ENABLE_IQCORR_CCK
 12-13 RXCAL_MEAS_TIME_SEL
 14-15 CLCAL_MEAS_TIME_SEL
 16-20 CF_CLC_INIT_RFGAIN
 21 hw4.2: CF_CLC_PAL_MODE ;-hw4.2 only (removed again in hw6) |
| --- |

**ATH:02A254h/02A7CCh - BB_BLUETOOTH_CNTL ;hw4/hw6**

```

| 0 BT_BREAK_CCK_EN
 1 BT_ANT_HALT_WLAN
 2 hw6:ENABLE_RFBUS_GRANT_WAKEUP ;-hw6 |
| --- |

**ATH:02A258h/02A3F8h - BB_TPC_1 ;hw4/hw6**

```

| 0 FORCE_DAC_GAIN
 1-5 FORCED_DAC_GAIN
 6-13 PD_DC_OFFSET_TARGET
 14-15 NUM_PD_GAIN
 16-17 PD_GAIN_SETTING1
 18-19 PD_GAIN_SETTING2
 20-21 PD_GAIN_SETTING3
 22 ENABLE_PD_CALIBRATE
 23-28 PD_CALIBRATE_WAIT
 29 FORCE_PDADC_GAIN
 30-31 FORCED_PDADC_GAIN |
| --- |

**ATH:02A25Ch/02A3FCh - BB_TPC_2 ;hw4/hw6**

```

| 0-7 TX_FRAME_TO_PDADC_ON
 8-15 TX_FRAME_TO_PD_ACC_OFDM
 16-23 TX_FRAME_TO_PD_ACC_CCK |
| --- |

**ATH:02A260h/02A400h - BB_TPC_3 ;hw4/hw6**

```

| 0-7 TX_END_TO_PDADC_ON
 8-15 TX_END_TO_PD_ACC_ON
 16-18 PD_ACC_WINDOW_DC_OFF
 19-21 PD_ACC_WINDOW_CAL
 22-24 PD_ACC_WINDOW_OFDM
 25-27 PD_ACC_WINDOW_CCK
 31 TPC_CLK_GATE_ENABLE |
| --- |

**ATH:02A264h/02A404h - BB_TPC_4_B0 ;hw4/hw6**

**ATH:02B404h - BB_TPC_4_B1 ;hw6**

```

| 0 PD_AVG_VALID_0/1 (R) ;\
 1-8 PD_AVG_OUT_0/1 (R) ;
 9-13 DAC_GAIN_0/1 (R) ; separate settings (on hw6)
 14-19 TX_GAIN_SETTING_0/1 (R) ;
 20-24 RATE_SENT_0/1 (R) ;/
 25-30 ERROR_EST_UPDATE_POWER_THRESH ;-global setting (not in B1) |
| --- |

**ATH:02A268h/02A34Ch - BB_ANALOG_SWAP ;hw4/hw6**

```

| 0-2 ANALOG_RX_SWAP_CNTL
 3-5 ANALOG_TX_SWAP_CNTL
 6 SWAP_ALT_CHN
 7 ANALOG_DC_DAC_POLARITY
 8 ANALOG_PKDET_DAC_POLARITY |
| --- |

**ATH:02A26Ch/02A408h - BB_TPC_5_B0 ;hw4/hw6**

**ATH:02B408h - BB_TPC_5_B1 ;hw6**

```

| 0-3 PD_GAIN_OVERLAP ;-global setting (not in B1)
 4-9 PD_GAIN_BOUNDARY_1_0/1 ;\
 10-15 PD_GAIN_BOUNDARY_2_0/1 ; separate settings (on hw6)
 16-21 PD_GAIN_BOUNDARY_3_0/1 ;
 22-27 PD_GAIN_BOUNDARY_4_0/1 ;/ |
| --- |

**ATH:02A270h/02A40Ch - BB_TPC_6_B0 ;hw4/hw6**

**ATH:02B40Ch - BB_TPC_6_B1 ;hw6**

```

| 0-5 PD_DAC_SETTING_1
 6-11 PD_DAC_SETTING_2
 12-17 PD_DAC_SETTING_3
 18-23 PD_DAC_SETTING_4
 24-25 ERROR_EST_MODE
 26-28 ERROR_EST_FILTER_COEFF |
| --- |

**ATH:02A274h/02A410h - BB_TPC_7 ;hw4/hw6**

```

| 0-5 TX_GAIN_TABLE_MAX
 6-11 INIT_TX_GAIN_SETTING
 12 EN_CL_GAIN_MOD
 13 USE_TX_PD_IN_XPA
 14 EXTEND_TX_FRAME_FOR_TPC
 15 USE_INIT_TX_GAIN_SETTING_AFTER_WARM_RESET |
| --- |

**ATH:02A280h..02A2FCh - BB_PDADC_TAB_B0[0..31] (W) ;hw4**

**ATH:02A480h..02A4FCh - BB_PDADC_TAB_B0[0..31] (W) ;hw6**

**ATH:02B480h..02B4FCh - BB_PDADC_TAB_B1[0..31] (W) ;hw6**

```

| 0-31 TAB_ENTRY (W) |
| --- |

**ATH:02A300h..02A33Ch - BB_CL_TAB_B0[0..15] ;hw4**

**ATH:02A300h..02A33Ch - BB_CL_TAB_B0[0..15] ;hw6**

**ATH:02B300h..02B33Ch - BB_CL_TAB_B1[0..15] ;hw6 only (B1)**

```

| 0-4 CL_GAIN_MOD
 5-15 CARR_LK_DC_ADD_Q
 16-26 CARR_LK_DC_ADD_I
 27-30 BB_GAIN |
| --- |

**ATH:02A340h/02A2DCh - BB_CL_MAP_0_B0 ;hw4/hw6**

**ATH:02A344h/02A2E0h - BB_CL_MAP_1_B0 ;hw4/hw6**

**ATH:02A348h/02A2E4h - BB_CL_MAP_2_B0 ;hw4/hw6**

**ATH:02A34Ch/02A2E8h - BB_CL_MAP_3_B0 ;hw4/hw6**

**ATH:02B2DCh - BB_CL_MAP_0_B1 ;hw6**

**ATH:02B2E0h - BB_CL_MAP_1_B1 ;hw6**

**ATH:02B2E4h - BB_CL_MAP_2_B1 ;hw6**

**ATH:02B2E8h - BB_CL_MAP_3_B1 ;hw6**

```

| 0-31 CL_MAP |
| --- |

**ATH:02A358h/02A2D8h - BB_CL_CAL_CTRL ;hw4/hw6**

```

| 0 ENABLE_PARALLEL_CAL
 1 ENABLE_CL_CALIBRATE
 2-3 CF_CLC_TEST_POINT
 4-7 CF_CLC_FORCED_PAGAIN
 8-15 CARR_LEAK_MAX_OFFSET
 16-21 CF_CLC_INIT_BBGAIN
 22-29 CF_ADC_BOUND
 30 USE_DAC_CL_CORRECTION
 31 CL_MAP_HW_GEN |
| --- |

**ATH:02A388h/02A244h - BB_RIFS ;hw4/hw6**

```

| 25 DISABLE_FCC_FIX
 26 ENABLE_RESET_TDOMAIN
 27 DISABLE_FCC_FIX2
 28 DISABLE_RIFS_CCK_FIX
 29 DISABLE_ERROR_RESET_FIX
 30 RADAR_USE_FDOMAIN_RESET |
| --- |

**ATH:029934h/02A3C0h - BB_POWERTX_RATE1 ;Power TX_0..3 ;hw4/hw6**

**ATH:029938h/02A3C4h - BB_POWERTX_RATE2 ;Power TX_4..7 ;hw4/hw6**

**ATH:02A234h/02A3C8h - BB_POWERTX_RATE3 ;Power TX_1L,2L,2S ;hw4/hw6**

**ATH:02A238h/02A3CCh - BB_POWERTX_RATE4 ;Power TX_55L,55S,11L,11S ;hw4/hw6**

```

| 0-5 POWERTX_0 / POWERTX_4 / POWERTX_1L / POWERTX_55L
 8-13 POWERTX_1 / POWERTX_5 / - / POWERTX_55S
 16-21 POWERTX_2 / POWERTX_6 / POWERTX_2L / POWERTX_11L
 24-29 POWERTX_3 / POWERTX_7 / POWERTX_2S / POWERTX_11S |
| --- |

**ATH:02A38Ch/02A3D0h - BB_POWERTX_RATE5 ;Power TX HT20_0..3 ;hw4/hw6**

**ATH:02A390h/02A3D4h - BB_POWERTX_RATE6 ;Power TX HT20_4..7 ;hw4/hw6**

**ATH:02A3CCh/02A3E4h - BB_POWERTX_RATE10 ;Power TX HT20_8..11 ;hw4/hw6**

**ATH:02A3D0h/02A3E8h - BB_POWERTX_RATE11 ;Power TX HT20/40_12/13 ;hw4/hw6**

```

| 0-5 POWERTX HT20_0 / HT20_4 / HT20_8 / HT20_12
 8-13 POWERTX HT20_1 / HT20_5 / HT20_9 / HT20_13
 16-21 POWERTX HT20_2 / HT20_6 / HT20_10 / HT40_12
 24-29 POWERTX HT20_3 / HT20_7 / HT20_11 / HT40_13 |
| --- |

**ATH:02A3C0h/02A3D8h - BB_POWERTX_RATE7 ;Power TX HT40_0..3 ;hw4/hw6**

**ATH:02A3C4h/02A3DCh - BB_POWERTX_RATE8 ;Power TX HT40_4..7 ;hw4/hw6**

**ATH:02A3D4h/02A3ECh - BB_POWERTX_RATE12 ;Power TX HT40_8..11 ;hw4/hw6**

**ATH:02A3C8h/02A3E0h - BB_POWERTX_RATE9 ;PowerTX DUP40/EXT20_CCK/OFDM ;hw4/hw6**

```

| 0-5 POWERTX HT40_0 / HT40_4 / HT40_8 / DUP40_CCK
 8-13 POWERTX HT40_1 / HT40_5 / HT40_9 / DUP40_OFDM
 16-21 POWERTX HT40_2 / HT40_6 / HT40_10 / EXT20_CCK
 24-29 POWERTX HT40_3 / HT40_7 / HT40_11 / EXT20_OFDM |
| --- |

**ATH:02A3BCh/02A3F4h - BB_POWERTX_SUB ;Power TX Sub_for_2chain ;hw4/hw6**

```

| 0-5 POWERTX SUB_FOR_2CHAIN |
| --- |

**ATH:02A278h/02A414h - BB_TPC_8 ;DESIRED_SCALE_0..5 ;hw4/hw6**

```

| 0-4 DESIRED_SCALE_0
 5-9 DESIRED_SCALE_1
 10-14 DESIRED_SCALE_2
 15-19 DESIRED_SCALE_3
 20-24 DESIRED_SCALE_4
 25-29 DESIRED_SCALE_5 |
| --- |

**ATH:02A27Ch/02A418h - BB_TPC_9 ;DESIRED_SCALE_6,7,CCK and MISC ;hw4/hw6**

```

| 0-4 DESIRED_SCALE_6
 5-9 DESIRED_SCALE_7
 10-14 DESIRED_SCALE_CCK
 20 EN_PD_DC_OFFSET_THR
 21-26 PD_DC_OFFSET_THR
 27-30 WAIT_CALTX_SETTLE
 31 DISABLE_PDADC_RESIDUAL_DC_REMOVAL |
| --- |

**ATH:02A394h/02A41Ch - BB_TPC_10 ;DESIRED_SCALE HT20_0..5 ;hw4/hw6**

**ATH:02A3E4h/02A42Ch - BB_TPC_14 ;DESIRED_SCALE HT20_8..13 ;hw4/hw6**

**ATH:02A3DCh/02A424h - BB_TPC_12 ;DESIRED_SCALE HT40_0..5 ;hw4/hw6**

**ATH:02A3E0h/02A428h - BB_TPC_13 ;DESIRED_SCALE HT40_6..7 ;hw4/hw6**

**ATH:02A3E8h/02A430h - BB_TPC_15 ;DESIRED_SCALE HT40_8..13 ;hw4/hw6**

```

| 0-4 DESIRED_SCALE HT20_0 / HT20_8 / HT40_0 / HT40_6 / HT40_8
 5-9 DESIRED_SCALE HT20_1 / HT20_9 / HT40_1 / HT40_7 / HT40_9
 10-14 DESIRED_SCALE HT20_2 / HT20_10 / HT40_2 / - / HT40_10
 15-19 DESIRED_SCALE HT20_3 / HT20_11 / HT40_3 / - / HT40_11
 20-24 DESIRED_SCALE HT20_4 / HT20_12 / HT40_4 / - / HT40_12
 25-29 DESIRED_SCALE HT20_5 / HT20_13 / HT40_5 / - / HT40_13 |
| --- |

**ATH:02A398h/02A420h - BB_TPC_11_B0 ;DESIRED_SCALE HT20_6..7 and OLPC ;hw4/hw6**

**ATH:02B420h - BB_TPC_11_B1 ;hw6**

```

| 0-4 DESIRED_SCALE HT20_6 ;\global setting (not in B1)
 5-9 DESIRED_SCALE HT20_7 ;/
 16-23 OLPC_GAIN_DELTA_0/1 ;\
 24-31 hw4: OLPC_GAIN_DELTA_0/1_PAL_ON ;-hw4 ; separate settings (on hw6)
 24-25 hw6: OLPC_GAIN_DELTA_0/1_LSB_EXT ;-hw6 ;/ |
| --- |

**ATH:02A39Ch/02A2C0h - BB_CAL_CHAIN_MASK ;hw4/hw6**

```

| 0-2 CAL_CHAIN_MASK |
| --- |

**ATH:02A3D8h/02A358h - BB_FORCE_ANALOG ;hw4/hw6**

```

| 0 FORCE_XPAON
 1-3 FORCED_XPAON
 4 FORCE_PDADC_PWD
 5-7 FORCED_PDADC_PWD |
| --- |

**ATH:02A3ECh/02A434h - BB_TPC_16 ;hw4/hw6**

```

| 8-13 PDADC_PAR_CORR_CCK
 16-21 PDADC_PAR_CORR_OFDM
 24-29 PDADC_PAR_CORR_HT40 |
| --- |

**ATH:02A3F0h/02A438h - BB_TPC_17 ;hw4/hw6**

```

| 0 ENABLE_PAL
 1 ENABLE_PAL_CCK
 2 ENABLE_PAL_OFDM_20
 3 ENABLE_PAL_OFDM_40
 4-9 PAL_POWER_THRESHOLD
 10 FORCE_PAL_LOCKED
 11-16 INIT_TX_GAIN_SETTING_PAL_ON |
| --- |

**ATH:02A3F4h/02A43Ch - BB_TPC_18 ;hw4/hw6**

```

| 0-7 THERM_CAL_VALUE
 8-15 VOLT_CAL_VALUE
 16 USE_LEGACY_TPC
 17-22 hw6: MIN_POWER_THERM_VOLT_GAIN_CORR ;-hw6 |
| --- |

**ATH:02A3F8h/02A440h - BB_TPC_19/BB_TPC_19_B0 ;hw4/hw6**

**ATH:02B440h - BB_TPC_19_B1 ;hw6**

```

| 0-7 ALPHA_THERM
 8-15 ALPHA_THERM_PAL_ON
 16-20 ALPHA_VOLT
 21-25 ALPHA_VOLT_PAL_ON |
| --- |

**ATH:02A3FCh/02A444h - BB_TPC_20 ;hw4/hw6**

```

| 0-23 ENABLE_PAL_MCS_0..23 |
| --- |

**ATH:02A518h..02A554h - BB_CALTX_GAIN_SET_(0,2,4,6,..,28,30) ;hw4**

**ATH:02A600h..02A63Ch - BB_CALTX_GAIN_SET_(0,2,4,6,..,28,30) ;hw6**

Contains 32 table entries (numbered 0..31), with two 14bit entries per word.

```

| 0-13 CALTX_GAIN_SET_nn table entry 0,2,4,6,...,28,30 accordingly
 14-27 CALTX_GAIN_SET_nn table entry 1,3,5,7,...,29,31 accordingly
 28-31 - |
| --- |

**ATH:02A400h..02A47Ch - BB_TX_GAIN_TAB_(1..32) ;hw4**

**ATH:02A500h..02A57Ch - BB_TX_GAIN_TAB_(1..32) ;hw6**

Contains 32 table entries (numbered 1..32), with one 32bit entry per word.

```

| 0-31 TG_TABLE entry entry 1..32 accordingly |
| --- |

On hw6, the 32bit entries are expanded to 34bit size (with extra 2bits in the
BB_TX_GAIN_TAB_xxx_LSB_EXT registers).

**ATH:02A58Ch - BB_TX_GAIN_TAB_1_16_LSB_EXT ;hw6**

**ATH:02A590h - BB_TX_GAIN_TAB_17_32_LSB_EXT ;hw6**

Contains 32 table entries (numbered 1..32), with sixteen 2bit entries per word.

```

| 0-31 TG_TABLE_LSB_EXT (sixteen 2bit entries per word) |
| --- |

These 2bit values are used to expand the 32bit entries in
BB_TX_GAIN_TAB_(1..32) to 34bit size.

**ATH:02A6DCh/02A644h - BB_TXIQCAL_CONTROL_0 ;hw4/hw6**

```

| 0 IQC_TX_TABLE_SEL
 1-6 BASE_TX_TONE_DB
 7-12 MAX_TX_TONE_GAIN
 13-18 MIN_TX_TONE_GAIN
 19-22 CALTXSHIFT_DELAY
 23-29 LOOPBACK_DELAY
 30 hw6: ENABLE_COMBINED_CARR_IQ_CAL ;\hw6
 31 hw6: ENABLE_TXIQ_CALIBRATE ;/ |
| --- |

**ATH:02A6E0h/02A648h - BB_TXIQCAL_CONTROL_1 ;hw4/hw6**

```

| 0-5 RX_INIT_GAIN_DB
 6-11 MAX_RX_GAIN_DB
 12-17 MIN_RX_GAIN_DB
 18-24 IQCORR_I_Q_COFF_DELPT |
| --- |

**ATH:02A6E4h/02A64Ch - BB_TXIQCAL_CONTROL_2 ;hw4/hw6**

```

| 0-3 IQC_FORCED_PAGAIN
 4-8 IQCAL_MIN_TX_GAIN
 9-13 IQCAL_MAX_TX_GAIN |
| --- |

**ATH:02A6E8h/0298B0h - BB_TXIQCAL_CONTROL_3 ;hw4/hw6**

```

| 0-5 PWR_HIGH_DB
 6-11 PWR_LOW_DB
 12-21 IQCAL_TONE_PHS_STEP
 22-23 DC_EST_LEN
 24 ADC_SAT_LEN
 25-26 ADC_SAT_SEL
 27-28 IQCAL_MEAS_LEN
 29-30 DESIRED_SIZE_DB
 31 TX_IQCORR_EN |
| --- |

**ATH:02A6ECh/02A650h - BB_TXIQ_CORR_COEFF_01_B0 ;hw4/hw6**

**ATH:02A6F0h/02A654h - BB_TXIQ_CORR_COEFF_23_B0 ;hw4/hw6**

**ATH:02A6F4h/02A658h - BB_TXIQ_CORR_COEFF_45_B0 ;hw4/hw6**

**ATH:02A6F8h/02A65Ch - BB_TXIQ_CORR_COEFF_67_B0 ;hw4/hw6**

**ATH:02A6FCh/02A660h - BB_TXIQ_CORR_COEFF_89_B0 ;hw4/hw6**

**ATH:02A700h/02A664h - BB_TXIQ_CORR_COEFF_AB_B0 ;hw4/hw6**

**ATH:02A704h/02A668h - BB_TXIQ_CORR_COEFF_CD_B0 ;hw4/hw6**

**ATH:02A708h/02A66Ch - BB_TXIQ_CORR_COEFF_EF_B0 ;hw4/hw6**

**ATH:02B650h - BB_TXIQ_CORR_COEFF_01_B1 ;hw6**

**ATH:02B654h - BB_TXIQ_CORR_COEFF_23_B1 ;hw6**

**ATH:02B658h - BB_TXIQ_CORR_COEFF_45_B1 ;hw6**

**ATH:02B65Ch - BB_TXIQ_CORR_COEFF_67_B1 ;hw6**

**ATH:02B660h - BB_TXIQ_CORR_COEFF_89_B1 ;hw6**

**ATH:02B664h - BB_TXIQ_CORR_COEFF_AB_B1 ;hw6**

**ATH:02B668h - BB_TXIQ_CORR_COEFF_CD_B1 ;hw6**

**ATH:02B66Ch - BB_TXIQ_CORR_COEFF_EF_B1 ;hw6**

The B0 (and B1) table each contain 16 entries (numbered 0..F), with two 14bit
entries per word.

```

| 0-13 IQC_COEFF_TABLE_n ;table entry (n=0,2,4,6,8,A,C,E) accordingly
 14-27 IQC_COEFF_TABLE_n ;table entry (n=1,3,5,7,9,B,D,F) accordingly
 28-31 - |
| --- |

**ATH:02A70Ch/02A670h - BB_CAL_RXBB_GAIN_TBL_0 ;hw4/hw6**

**ATH:02A710h/02A674h - BB_CAL_RXBB_GAIN_TBL_4 ;hw4/hw6**

**ATH:02A714h/02A678h - BB_CAL_RXBB_GAIN_TBL_8 ;hw4/hw6**

**ATH:02A718h/02A67Ch - BB_CAL_RXBB_GAIN_TBL_12 ;hw4/hw6**

**ATH:02A71Ch/02A680h - BB_CAL_RXBB_GAIN_TBL_16 ;hw4/hw6**

**ATH:02A720h/02A684h - BB_CAL_RXBB_GAIN_TBL_20 ;hw4/hw6**

**ATH:02A724h/02A688h - BB_CAL_RXBB_GAIN_TBL_24 ;hw4/hw6**

Contains 25 table entries (numbered 0..24), with four 6bit entries per word
(except, only one entry in the last word).

```

| 0-5 TXCAL_RX_BB_GAIN_TABLE_n ;table entry (n=0,4, 8,12,16,20,24)
 6-11 TXCAL_RX_BB_GAIN_TABLE_n ;table entry (n=1,5, 9,13,17,21)
 12-17 TXCAL_RX_BB_GAIN_TABLE_n ;table entry (n=2,6,10,14,18,22)
 18-23 TXCAL_RX_BB_GAIN_TABLE_n ;table entry (n=3,7,11,15,19,23)
 24-31 - |
| --- |

**ATH:02A728h/02A68Ch - BB_TXIQCAL_STATUS_B0 (R) ;hw4/hw6**

**ATH:02B68Ch - BB_TXIQCAL_STATUS_B1 (R) ;hw6**

```

| 0 TXIQCAL_FAILED (R)
 1-5 CALIBRATED_GAINS (R)
 6-11 TONE_GAIN_USED (R)
 12-17 RX_GAIN_USED (R)
 18-24 hw4: LAST_MEAS_ADDR (7bit) (R) ;-hw4
 18-23 hw6: LAST_MEAS_ADDR (6bit) (R) ;-hw6 |
| --- |

**ATH:02A7D8h/02A2CCh - BB_FCAL_1 ;hw4/hw6**

```

| 0-9 FLC_PB_FSTEP
 10-19 FLC_SB_FSTEP
 20-24 FLC_PB_ATTEN
 25-29 FLC_SB_ATTEN |
| --- |

**ATH:02A7DCh/02A2D0h - BB_FCAL_2_B0 ;hw4/hw6**

**ATH:02B2D0h - BB_FCAL_2_B1 ;hw6**

```

| 0-2 FLC_PWR_THRESH ;-global setting (not in B1)
 3-7 FLC_SW_CAP_VAL_0/1 ;-separate settings (on hw6)
 8-9 FLC_BBMISCGAIN ;\
 10-12 FLC_BB1DBGAIN ;
 13-14 FLC_BB6DBGAIN ; global setting (not in B1)
 15 FLC_SW_CAP_SET ;
 16-18 FLC_MEAS_WIN ;/
 20-24 FLC_CAP_VAL_STATUS_0/1 (R) ;-separate settings (on hw6) |
| --- |

**ATH:02A7E0h/02A22Ch - BB_RADAR_BW_FILTER ;hw4/hw6**

```

| 0 RADAR_AVG_BW_CHECK
 1 RADAR_DC_SRC_SEL
 2-3 RADAR_FIRPWR_SEL
 4-5 RADAR_PULSE_WIDTH_SEL
 8-14 RADAR_DC_FIRPWR_THRESH
 15-20 RADAR_DC_PWR_BIAS
 21-26 RADAR_BIN_MAX_BW |
| --- |

**ATH:02A7E4h/02A2D4h - BB_DFT_TONE_CTRL_B0 ;hw4/hw6**

**ATH:02B2D4h - BB_DFT_TONE_CTRL_B1 ;hw6**

```

| 0 DFT_TONE_EN
 2-3 DFT_TONE_AMP_SEL
 4-12 DFT_TONE_FREQ_ANG |
| --- |

**ATH:02A7E8h/02A448h - BB_THERM_ADC_1 ;hw4/hw6**

```

| 0-7 INIT_THERM_SETTING
 8-15 INIT_VOLT_SETTING
 16-23 INIT_ATB_SETTING
 24-25 SAMPLES_CNT_CODING
 26 USE_INIT_THERM_VOLT_ATB_AFTER_WARM_RESET
 27 FORCE_THERM_VOLT_ATB_TO_INIT_SETTINGS
 28 hw6: CHECK_DONE_FOR_1ST_ADC_MEAS_OF_EACH_FRAME ;\hw6
 29 hw6: THERM_MEASURE_RESET ;/ |
| --- |

**ATH:02A7ECh/02A44Ch - BB_THERM_ADC_2 ;hw4/hw6**

```

| 0-11 MEASURE_THERM_FREQ
 12-21 MEASURE_VOLT_FREQ
 22-31 MEASURE_ATB_FREQ |
| --- |

**ATH:02A7F0h/02A450h - BB_THERM_ADC_3 ;hw4/hw6**

```

| 0-7 THERM_ADC_OFFSET
 8-16 THERM_ADC_SCALED_GAIN
 17-29 ADC_INTERVAL |
| --- |

**ATH:02A7F4h/02A454h - BB_THERM_ADC_4 ;hw4/hw6**

```

| 0-7 LATEST_THERM_VALUE (R)
 8-15 LATEST_VOLT_VALUE (R)
 16-23 LATEST_ATB_VALUE (R)
 24 hw6: FORCE_THERM_CHAIN ;\hw6
 25-27 hw6: PREFERRED_THERM_CHAIN ;/ |
| --- |

**ATH:02A7F8h/02A458h - BB_TX_FORCED_GAIN ;hw4/hw6**

```

| 0 FORCE_TX_GAIN
 1-3 FORCED_TXBB1DBGAIN
 4-5 FORCED_TXBB6DBGAIN
 6-9 FORCED_TXMXRGAIN
 10-13 FORCED_PADRVGNA
 14-17 FORCED_PADRVGNB
 18-21 FORCED_PADRVGNC
 22-23 FORCED_PADRVGND
 24 FORCED_ENABLE_PAL
 25-27 hw6: FORCED_OB ;\
 28-30 hw6: FORCED_DB ; hw6
 31 hw6: FORCED_GREEN_PAPRD_ENABLE ;/ |
| --- |

**ATH:02A7FCh/02A7DCh - BB_ECO_CTRL ;hw4/hw6**

```

| 0-7 ECO_CTRL |
| --- |

```

| ______________ below in hw4.2 and hw6 only (not original hw4) ______________ |
| --- |

**ATH:029DE4h/0298E4h - BB_PAPRD_AM2AM_MASK ;hw4.2/hw6 (not original hw4)**

```

| 0-24 PAPRD_AM2AM_MASK ;-newer revision only |
| --- |

**ATH:029DE8h/0298E8h - BB_PAPRD_AM2PM_MASK ;hw4.2/hw6 (not original hw4)**

```

| 0-24 PAPRD_AM2PM_MASK ;-newer revision only |
| --- |

**ATH:029DECh/0298ECh - BB_PAPRD_HT40_MASK ;hw4.2/hw6 (not original hw4)**

```

| 0-24 PAPRD_HT40_MASK ;-newer revision only |
| --- |

**ATH:029DF0h - BB_PAPRD_CTRL0 ;hw4.2 (not original hw4)**

**ATH:0298F0h - BB_PAPRD_CTRL0_B0 ;hw6**

**ATH:02A8F0h - BB_PAPRD_CTRL0_B1 ;hw6**

```

| 0 PAPRD_ENABLE ;\
 1 PAPRD_ADAPTIVE_USE_SINGLE_TABLE ; newer revision only
 2-26 PAPRD_VALID_GAIN ;
 27-31 PAPRD_MAG_THRSH ;/ |
| --- |

**ATH:029DF4h - BB_PAPRD_CTRL1 ;hw4.2 (not original hw4)**

**ATH:0298F4h - BB_PAPRD_CTRL1_B0 ;hw6**

**ATH:02A8F4h - BB_PAPRD_CTRL1_B1 ;hw6**

```

| 0 PAPRD_ADAPTIVE_SCALING_ENABLE ;\
 1 PAPRD_ADAPTIVE_AM2AM_ENABLE ;
 2 PAPRD_ADAPTIVE_AM2PM_ENABLE ; newer revision only
 3-8 PAPRD_POWER_AT_AM2AM_CAL ;
 9-16 PA_GAIN_SCALE_FACTOR ;
 17-26 PAPRD_MAG_SCALE_FACTOR ;
 27 PAPRD_TRAINER_IANDQ_SEL ;/ |
| --- |

**ATH:029DF8h - BB_PA_GAIN123 ;hw4.2 (not original hw4)**

**ATH:0298F8h - BB_PA_GAIN123_B0 ;hw6**

**ATH:02A8F8h - BB_PA_GAIN123_B1 ;hw6**

```

| 0-9 PA_GAIN1 ;\
 10-19 PA_GAIN2 ; newer revision only
 20-29 PA_GAIN3 ;/ |
| --- |

**ATH:029DFCh - BB_PA_GAIN45 ;hw4.2 (not original hw4)**

**ATH:0298FCh - BB_PA_GAIN45_B0 ;hw6**

**ATH:02A8FCh - BB_PA_GAIN45_B1 ;hw6**

```

| 0-9 PA_GAIN4 ;\
 10-19 PA_GAIN5 ; newer revision only
 20-24 PAPRD_ADAPTIVE_TABLE_VALID ;/ |
| --- |

**ATH:029E00h..029E1Ch - BB_PAPRD_PRE_POST_SCALE_(0..7) ;hw4.2 (not hw4)**

**ATH:029900h..02991Ch - BB_PAPRD_PRE_POST_SCALE_(0..7)_B0 ;hw6**

**ATH:02A900h..02A91Ch - BB_PAPRD_PRE_POST_SCALE_(0..7)_B1 ;hw6**

```

| 0-17 PAPRD_PRE_POST_SCALING ;-newer revision only |
| --- |

**ATH:029E20h.. - BB_PAPRD_MEM_TAB[......] ;hw4.2 (not original hw4)**

**ATH:029920h.. - BB_PAPRD_MEM_TAB_B0[0..119] ;hw6**

**ATH:02A920h.. - BB_PAPRD_MEM_TAB_B1[0..119] ;hw6**

```

| 0-21 PAPRD_MEM ;-newer revision only |
| --- |

**ATH:02A35Ch/02A2ECh - BB_CL_MAP_PAL_0_B0 ;hw4.2/hw6 (not original hw4)**

**ATH:02A360h/02A2F0h - BB_CL_MAP_PAL_1_B0 ;hw4.2/hw6 (not original hw4)**

**ATH:02A364h/02A2F4h - BB_CL_MAP_PAL_2_B0 ;hw4.2/hw6 (not original hw4)**

**ATH:02A368h/02A2F8h - BB_CL_MAP_PAL_3_B0 ;hw4.2/hw6 (not original hw4)**

**ATH:02B2ECh - BB_CL_MAP_PAL_0_B1 ;hw6**

**ATH:02B2F0h - BB_CL_MAP_PAL_1_B1 ;hw6**

**ATH:02B2F4h - BB_CL_MAP_PAL_2_B1 ;hw6**

**ATH:02B2F8h - BB_CL_MAP_PAL_3_B1 ;hw6**

```

| 0-31 CL_MAP_PAL |
| --- |

**ATH:02A72Ch/02A690h - BB_PAPRD_TRAINER_CNTL1 ;hw4.2/hw6 (not original hw4)**

```

| 0 CF_PAPRD_TRAIN_ENABLE ;\
 1-7 CF_PAPRD_AGC2_SETTLING ;
 8 CF_PAPRD_IQCORR_ENABLE ; newer revision only
 9 CF_PAPRD_RX_BB_GAIN_FORCE ;
 10 CF_PAPRD_TX_GAIN_FORCE ;
 11 CF_PAPRD_LB_ENABLE ;
 12-18 CF_PAPRD_LB_SKIP ;/ |
| --- |

**ATH:02A730h/02A694h - BB_PAPRD_TRAINER_CNTL2 ;hw4.2/hw6 (not original hw4)**

```

| 0-31 CF_PAPRD_INIT_RX_BB_GAIN ;-newer revision only |
| --- |

**ATH:02A734h/02A698h - BB_PAPRD_TRAINER_CNTL3 ;hw4.2/hw6 (not original hw4)**

```

| 0-5 CF_PAPRD_ADC_DESIRED_SIZE ;\
 6-11 CF_PAPRD_QUICK_DROP ;
 12-16 CF_PAPRD_MIN_LOOPBACK_DEL ;
 17-19 CF_PAPRD_NUM_CORR_STAGES ; newer revision only
 20-23 CF_PAPRD_COARSE_CORR_LEN ;
 24-27 CF_PAPRD_FINE_CORR_LEN ;/
 28 hw4.2: CF_PAPRD_BBTXMIX_DISABLE ;-hw4.2
 28 hw6: CF_PAPRD_REUSE_CORR ;\hw6
 29 hw6: CF_PAPRD_BBTXMIX_DISABLE ;/ |
| --- |

**ATH:02A738h/02A69Ch - BB_PAPRD_TRAINER_CNTL4 ;hw4.2/hw6 (not original hw4)**

```

| 0-11 CF_PAPRD_MIN_CORR ;\
 12-15 CF_PAPRD_SAFETY_DELTA ; newer revision only
 16-25 CF_PAPRD_NUM_TRAIN_SAMPLES ;/ |
| --- |

**ATH:02A73Ch/02A6A0h - BB_PAPRD_TRAINER_STAT1 ;hw4.2/hw6 (not original hw4)**

```

| 0 PAPRD_TRAIN_DONE ;\
 1 PAPRD_TRAIN_INCOMPLETE (R) ;
 2 PAPRD_CORR_ERR (R) ; newer revision only
 3 PAPRD_TRAIN_ACTIVE (R) ;
 4-8 PAPRD_RX_GAIN_IDX (R) ;
 9-16 PAPRD_AGC2_PWR (R) ;/ |
| --- |

**ATH:02A740h/02A6A4h - BB_PAPRD_TRAINER_STAT2 ;hw4.2/hw6 (not original hw4)**

```

| 0-15 PAPRD_FINE_VAL (R) ;\
 16-20 PAPRD_COARSE_IDX (R) ; newer revision only
 21-22 PAPRD_FINE_IDX (R) ;/ |
| --- |

**ATH:02A744h/02A6A8h - BB_PAPRD_TRAINER_STAT3 ;hw4.2/hw6 (not original hw4)**

```

| 0-19 PAPRD_TRAIN_SAMPLES_CNT (R) ;-newer revision only |
| --- |

```

| ____________________________ below on hw4 only ____________________________ |
| --- |

**ATH:02A480h..02A4FCh - BB_TX_GAIN_TAB_PAL_(1..32) ;hw4 only (not hw6)**

Contains 32 table entries (numbered 1..32), with one 32bit entry per word.

```

| 0-31 TG_TABLE_PAL_ON entry 1..32 accordingly |
| --- |

Seems to be some extra table, alternately to "BB_TX_GAIN_TAB_(1..32)". In hw6,
this has been replaced by the "LSB_EXT" feature (see BB_TPC_11).

**ATH:02A558h..02A6D4h - BB_TXIQCAL_MEAS_B0[0..95] (R) ;hw4 only (not hw6)**

```

| 0-11 TXIQC_MEAS_DATA0_0 (R) ;entry 0,2,4,...,190 (?)
 12-23 TXIQC_MEAS_DATA1_0 (R) ;entry 1,3,5,...,191 (?)
 24-31 - |
| --- |

**ATH:02A6D8h - BB_TXIQCAL_START ;hw4 only (not hw6)**

```

| 0 DO_TX_IQCAL |
| --- |

```

| ____________________________ below on hw6 only ____________________________ |
| --- |

**ATH:029C04h - BB_LDPC_CNTL1 ;hw6**

```

| 0-31 LDPC_LLR_SCALING0 |
| --- |

**ATH:029C08h - BB_LDPC_CNTL2 ;hw6**

```

| 0-15 LDPC_LLR_SCALING1
 16-26 LDPC_LATENCY |
| --- |

**ATH:029C18h - BB_ML_CNTL1 ;hw6**

```

| 0-23 CF_ML_2S_WEIGHT_TABLE
 24-25 CF_IS_FLAT_CH_THR_ML
 26-27 CF_IS_FLAT_CH_THR_ZF |
| --- |

**ATH:029C1Ch - BB_ML_CNTL2 ;hw6**

```

| 0-23 CF_ML_3S_WEIGHT_TABLE |
| --- |

**ATH:029E54h - BB_BT_COEX_1 ;hw6**

```

| 0-4 PEAK_DET_TALLY_THR_LOW_1
 5-9 PEAK_DET_TALLY_THR_MED_1
 10-14 PEAK_DET_TALLY_THR_HIGH_1
 15-19 RF_GAIN_DROP_DB_LOW_1
 20-24 RF_GAIN_DROP_DB_MED_1
 25-29 RF_GAIN_DROP_DB_HIGH_1
 30 BT_TX_DISABLE_NF_CAL |
| --- |

**ATH:029E58h - BB_BT_COEX_2 ;hw6**

```

| 0-4 PEAK_DET_TALLY_THR_LOW_2
 5-9 PEAK_DET_TALLY_THR_MED_2
 10-14 PEAK_DET_TALLY_THR_HIGH_2
 15-19 RF_GAIN_DROP_DB_LOW_2
 20-24 RF_GAIN_DROP_DB_MED_2
 25-29 RF_GAIN_DROP_DB_HIGH_2
 30-31 RFSAT_RX_RX |
| --- |

**ATH:029E5Ch - BB_BT_COEX_3 ;hw6**

```

| 0-1 RFSAT_BT_SRCH_SRCH
 2-3 RFSAT_BT_RX_SRCH
 4-5 RFSAT_BT_SRCH_RX
 6-7 RFSAT_WLAN_SRCH_SRCH
 8-9 RFSAT_WLAN_RX_SRCH
 10-11 RFSAT_WLAN_SRCH_RX
 12-13 RFSAT_EQ_SRCH_SRCH
 14-15 RFSAT_EQ_RX_SRCH
 16-17 RFSAT_EQ_SRCH_RX
 18-22 RF_GAIN_DROP_DB_NON_1
 23-27 RF_GAIN_DROP_DB_NON_2
 28-31 BT_RX_FIRPWR_INCR |
| --- |

**ATH:029E60h - BB_BT_COEX_4 ;RFGAIN_EQV_LNA_0..3 ;hw6**

**ATH:029E64h - BB_BT_COEX_5 ;RFGAIN_EQV_LNA_4..7 ;hw6**

```

| 0-7 RFGAIN_EQV_LNA_0 / RFGAIN_EQV_LNA_4
 8-15 RFGAIN_EQV_LNA_1 / RFGAIN_EQV_LNA_5
 16-23 RFGAIN_EQV_LNA_2 / RFGAIN_EQV_LNA_6
 24-31 RFGAIN_EQV_LNA_3 / RFGAIN_EQV_LNA_7 |
| --- |

**ATH:029E68h - BB_REDPWR_CTRL_1 ;hw6**

```

| 0-1 REDPWR_MODE
 2 REDPWR_MODE_CLR
 3 REDPWR_MODE_SET
 4-8 GAIN_CORR_DB2
 9-12 SCFIR_ADJ_GAIN
 13-17 QUICKDROP_RF
 18 BYPASS_FIR_F
 19 ADC_HALF_REF_F |
| --- |

**ATH:029E6Ch - BB_REDPWR_CTRL_2 ;hw6**

```

| 0-6 SC01_SW_INDEX
 7-13 SC10_SW_INDEX
 14-20 LAST_SC0_INDEX |
| --- |

**ATH:029FD0h - BB_MRC_CCK_CTRL ;hw6**

```

| 0 BBB_MRC_EN
 1 AGCDP_CCK_MRC_MUX_REG
 2-4 AGCDP_CCK_PD_ACCU_THR_HI
 5-7 AGCDP_CCK_PD_ACCU_THR_LOW
 8-11 AGCDP_CCK_BARKER_RSSI_THR
 12-16 AGCDP_CCK_MRC_BK_THR_HI
 17-21 AGCDP_CCK_MRC_BK_THR_LOW
 22-27 AGCDP_CCK_MIN_VALUE |
| --- |

**ATH:029FD4h - BB_CCK_BLOCKER_DET ;hw6**

```

| 0 CCK_FREQ_SHIFT_BLOCKER_DETECTION
 1 CCK_BLOCKER_DET_RESTART_WEAK_SIG
 2-5 CCK_BLOCKER_DET_BKSUM_NUM
 6-8 BK_VALID_DELAY
 9-13 CCK_BLOCKER_DET_THR
 14-19 CCK_BLOCKER_DET_DELAY_THR
 20-25 CCK_BLOCKER_MONITOR_TIME
 26 SKIP_RAMP_ENABLE
 27-31 CCK_DET_RAMP_THR |
| --- |

**ATH:02A384h - BB_SM_HIST ;hw6**

```

| 0 SM_REC_EN
 1 SM_REC_MODE
 2-3 SM_REC_TIME_RES
 4-11 SM_REC_PART_EN
 12-14 SM_REC_CHN_EN
 15-18 SM_REC_DATA_NUM
 19 SM_REC_AGC_SEL
 20-22 SM_REC_MAC_TRIG
 24-29 SM_REC_LAST_ADDR (R) |
| --- |

**ATH:02A580h - BB_RTT_CTRL ;hw6**

```

| 0 ENA_RADIO_RETENTION
 1-6 RESTORE_MASK
 7 FORCE_RADIO_RESTORE |
| --- |

**ATH:0298BCh - BB_GREEN_TX_CONTROL_1 ;hw6**

```

| 0 GREEN_TX_ENABLE
 1 GREEN_CASES |
| --- |

**ATH:02D800h - BB_MIT_RF_CNTL ;hw6**

```

| 0 MIT_FORCE_SYNTH_ON
 1 MIT_FORCE_SYNTH_ON_EN
 2 MIT_FORCE_ACTIVE_ON |
| --- |

**ATH:02D804h - BB_MIT_CCA_CNTL ;hw6**

```

| 0-2 MIT_CCA_MODE_SEL
 3-20 MIT_CCA_COUNT |
| --- |

**ATH:02D808h - BB_MIT_RSSI_CNTL_1 ;hw6**

```

| 0-5 MIT_RSSI_TH
 6-11 MIT_RX_RF_ATT_TH_H
 12-17 MIT_RX_RF_ATT_TH_L
 18-23 MIT_RX_RF_ATT_OFFSET
 24-29 MIT_AGC_LIMIT |
| --- |

**ATH:02D80Ch - BB_MIT_RSSI_CNTL_2 ;hw6**

```

| 0 MIT_AGC_SEL
 1-11 MIT_RSSI_BASE |
| --- |

**ATH:02D810h - BB_MIT_TX_CNTL ;hw6**

```

| 0-7 MIT_TX_STA_CNT
 8-21 MIT_TX_END_DLY_CNT
 22 MIT_TX_THROUGH_ENA
 23-25 MIT_TXHDR_CHAIN_MASK_CCK
 26-28 MIT_TXHDR_PAPRD_TRAIN_MASK_CCK
 29-30 MIT_TXHDR_CHAN_MODE_CCK |
| --- |

**ATH:02D814h - BB_MIT_RX_CNTL ;hw6**

```

| 0-7 MIT_RX_END_DLY_CNT
 8 MIT_RX_THROUGH_ENA |
| --- |

**ATH:02D818h - BB_MIT_OUT_CNTL ;hw6**

```

| 0-1 MIT_CLK_TUNE_MOD
 2 MIT_NO_DATA_TO_ATH |
| --- |

**ATH:02D81Ch - BB_MIT_SPARE_CNTL ;hw6**

```

| 0-30 MIT_SPARE_IN
 31 MIT_SPARE_OUT (R) |
| --- |

**ATH:029F90h - BB_DC_CAL_STATUS_B0 (R) ;hw6**

**ATH:02AF90h - BB_DC_CAL_STATUS_B1 (R) ;hw6**

```

| 0-4 OFFSETC1I (R)
 5-9 OFFSETC1Q (R)
 10-14 OFFSETC2I (R)
 15-19 OFFSETC2Q (R)
 20-24 OFFSETC3I (R)
 25-29 OFFSETC3Q (R) |
| --- |

**ATH:02A584h - BB_RTT_TABLE_SW_INTF_B0 ;hw6**

**ATH:02B584h - BB_RTT_TABLE_SW_INTF_B1 ;hw6**

```

| 0 SW_RTT_TABLE_ACCESS
 1 SW_RTT_TABLE_WRITE
 2-4 SW_RTT_TABLE_ADDR |
| --- |

**ATH:02A588h - BB_RTT_TABLE_SW_INTF_1_B0 ;hw6**

**ATH:02B588h - BB_RTT_TABLE_SW_INTF_1_B1 ;hw6**

```

| 4-31 SW_RTT_TABLE_DATA |
| --- |

**ATH:02A7F0h - BB_TABLES_INTF_ADDR_B0 ;hw6**

**ATH:02B7F0h - BB_TABLES_INTF_ADDR_B1 ;hw6**

**ATH:029BF0h - BB_CHN_TABLES_INTF_ADDR ;hw6**

**ATH:02ABF0h - BB_CHN1_TABLES_INTF_ADDR ;hw6**

```

| 2-17 TABLES_ADDR
 31 ADDR_AUTO_INCR |
| --- |

**ATH:02A7F4h - BB_TABLES_INTF_DATA_B0 ;hw6**

**ATH:02B7F4h - BB_TABLES_INTF_DATA_B1 ;hw6**

**ATH:029BF4h - BB_CHN_TABLES_INTF_DATA ;hw6**

**ATH:02ABF4h - BB_CHN1_TABLES_INTF_DATA ;hw6**

```

| 0-31 TABLES_DATA |
| --- |

**ATH:02CE00h - BB_DUMMY (R) ;hw6**

**ATH:02C800h - BB_DUMMY1[0..255] (R) ;hw6**

**ATH:02D200h - BB_DUMMY2[0..383] (R) ;hw6**

```

| 0 DUMMY (R) |
| --- |

Whatever "dummy" registers, maybe intended only for padding purposes; to get
the three BB_RSSI_B0/B1/B3 registers mapped to 029F80h+(0,1,3)*1000h on hw6.

```

| _______________________ missing B1 registers in hw4 _______________________ |
| --- |

hw4 does have only a few "B1" registers defined, although it does have a lot of
"B0" registers - which would suggest the presence of corresponding "B1"
registers, but those seem to exist only on hw6.

Maybe the hw4 designed was already aimed at adding "B1" registers in future.

Or maybe the "B1" registers DO EXIST even on hw4, being accessd by using
something like "address of B0 register PLUS some offset", or by using the
so-called "BASEBAND_2" and/or "BASEBAND_3" regions - and without mentioning
that anywhere in hw4 source code?

| DSi Atheros Wifi - Internal I/O - 0xxx00h - RDMA Registers (hw4/hw6) |
| --- |

These registers are same in hw4/hw6, except that:

```

| - base address changed from 30100h (hw4) to 54D00h (hw6)
 - number of regions has increased from 16 (hw4) to 32 (hw6)
 - accordingly, index for STATUS and INT_EN has has changed |
| --- |

**ATH:030100h/054D00h - DMA_CONFIG ;hw4/hw6**

```

| 0 DMA_TYPE
 1 RTC_PRIORITY
 2 ENABLE_RETENTION
 3 WLMAC_PWD_EN
 4 WLBB_PWD_EN
 5-31 - |
| --- |

**ATH:030104h/054D04h - DMA_CONTROL ;hw4/hw6**

```

| 0 STOP
 1 START
 2-31 - |
| --- |

**ATH:030108h/054D08h - DMA_SRC ;hw4/hw6**

**ATH:03010Ch/054D0Ch - DMA_DEST ;hw4/hw6**

```

| 0-1 -
 2-31 ADDR |
| --- |

**ATH:030110h/054D10h - DMA_LENGTH ;hw4/hw6**

```

| 0-11 WORDS
 12-31 - |
| --- |

**ATH:030114h/054D14h - VMC_BASE ;hw4/hw6**

```

| 0-1 -
 2-31 ADDR |
| --- |

**ATH:030118h/054D18h - INDIRECT_REG ;hw4/hw6**

```

| 0-1 -
 2-31 ID |
| --- |

**ATH:03011Ch/054D1Ch - INDIRECT_RETURN ;hw4/hw6**

```

| 0-1 -
 2-31 ADDR |
| --- |

**ATH:030120h..3015Ch - RDMA_REGION_(0..15)_ - located here in hw4, 16 regions**

**ATH:054D20h..54D9Ch - RDMA_REGION_(0..31)_ - located here in hw6, 32 regions**

```

| 0 NEXT
 1 INDI
 2-12 LENGTH
 13-31 ADDR |
| --- |

**ATH:030160h/054DA0h - DMA_STATUS ;hw4/hw6**

**ATH:030164h/054DA4h - DMA_INT_EN ;hw4/hw6**

```

| 0 RUNNING ;STATUS only (not INT_EN)
 1 STOPPED
 2 DONE
 3 ERROR
 4-14 ERROR_CODE ;STATUS only (not INT_EN)
 15-31 - |
| --- |

| DSi Atheros Wifi - Internal I/O - 03x000h - EFUSE Registers (hw4/hw6) |
| --- |

EFUSE exists in hw4/hw6, with some differences

```

| - base address changed from 31000h (hw4) to 30000h (hw6)
 - the single INTF region (hw4) replaced by two INTF regions (hw6)
 - four new registers added in hw6
 - indices for the two STROBE registers have changed |
| --- |

Unknown what EFUSE is... maybe it's meant to be a PROM for storing MAC addr and
calibration data (rather than something for surving electrical shortcuts).

**ATH:031000h/030000h - EFUSE_WR_ENABLE_REG ;hw4/hw6**

```

| 0 V
 1-31 - |
| --- |

**ATH:031004h/030004h - EFUSE_INT_ENABLE_REG ;hw4/hw6**

**ATH:031008h/030008h - EFUSE_INT_STATUS_REG ;hw4/hw6**

```

| 0 V
 1-31 - |
| --- |

**ATH:03100Ch/03000Ch - BITMASK_WR_REG ;hw4/hw6**

```

| 0-31 V |
| --- |

**ATH:031010h/030010h - VDDQ_SETTLE_TIME_REG ;hw4/hw6**

```

| 0-31 V |
| --- |

**ATH:031014h/030018h - RD_STROBE_PW_REG ;hw4/hw6**

```

| 0-31 V |
| --- |

**ATH:031018h/03001Ch - PG_STROBE_PW_REG ;hw4/hw6**

```

| 0-31 V |
| --- |

**ATH:031800h..031FFCh - EFUSE_INTF[0..511] - only one single area in hw4**

**ATH:030800h..030FFCh - EFUSE_INTF0[0..511] - first area in hw6 here**

**ATH:031000h..0317FCh - EFUSE_INTF1[0..511] - second area in hw6 here**

```

| 0-31 R |
| --- |

**ATH:030014h - VDDQ_HOLD_TIME_REG ;hw6 only**

```

| 0-31 V |
| --- |

**ATH:030020h - PGENB_SETUP_HOLD_TIME_REG ;hw6 only**

```

| 0-31 V |
| --- |

**ATH:030024h - STROBE_PULSE_INTERVAL_REG ;hw6 only**

```

| 0-31 V |
| --- |

**ATH:030028h - CSB_ADDR_LOAD_SETUP_HOLD_REG ;hw6 only**

```

| 0-31 V |
| --- |

| DSi Atheros Wifi - Internal I/O - 034000h - More Stuff (hw6) |
| --- |

```

| __________________________________ STEREO __________________________________ |
| --- |

**ATH:034000h - STEREO0_CONFIG**

```

| 0-7 POSEDGE
 8 MASTER
 9 SAMPLE_CNT_CLEAR_TYPE
 10 MCK_SEL
 11 I2S_WORD_SIZE
 12-13 DATA_WORD_SIZE
 14-15 STEREO_MONO
 16 MIC_WORD_SIZE
 17 PCM_SWAP
 18 I2S_DELAY
 19 RESET
 20 MIC_RESET
 21 ENABLE
 22 REFCLK_SEL
 23 SPDIF_ENABLE |
| --- |

**ATH:034004h - STEREO0_VOLUME**

```

| 0-4 CHANNEL0
 8-12 CHANNEL1 |
| --- |

**ATH:034008h - STEREO_MASTER_CLOCK**

```

| 0 MCK_SEL |
| --- |

**ATH:03400Ch - STEREO0_TX_SAMPLE_CNT_LSB**

**ATH:034010h - STEREO0_TX_SAMPLE_CNT_MSB**

**ATH:034014h - STEREO0_RX_SAMPLE_CNT_LSB**

**ATH:034018h - STEREO0_RX_SAMPLE_CNT_MSB**

```

| 0-15 CH0
 16-31 CH1 |
| --- |

```

| ________________________________ CHKSUM SEG ________________________________ |
| --- |

**ATH:035000h - CHKSUM_ACC_DMATX_CONTROL0**

**ATH:035004h - CHKSUM_ACC_DMATX_CONTROL1**

**ATH:035008h - CHKSUM_ACC_DMATX_CONTROL2**

**ATH:03500Ch - CHKSUM_ACC_DMATX_CONTROL3**

```

| 0 TXEN
 1 LITTLEENDIAN |
| --- |

**ATH:035010h - CHKSUM_ACC_DMATX_DESC0**

**ATH:035014h - CHKSUM_ACC_DMATX_DESC1**

**ATH:035018h - CHKSUM_ACC_DMATX_DESC2**

**ATH:03501Ch - CHKSUM_ACC_DMATX_DESC3**

```

| 0-31 ADDR |
| --- |

**ATH:035020h - CHKSUM_ACC_DMATX_DESC_STATUS**

```

| 0 UNDERRUN0
 1 UNDERRUN1
 2 UNDERRUN2
 3 UNDERRUN3
 4 BUSERROR
 5-8 DESC_INTR
 16-23 PKTCNT0
 24-25 CHAIN_NUM (R) |
| --- |

**ATH:035024h - CHKSUM_ACC_DMATX_ARB_CFG**

```

| 0 RRMODE
 8-13 WGT0
 14-19 WGT1
 20-25 WGT2
 26-31 WGT3 |
| --- |

**ATH:035028h - CHKSUM_ACC_RR_PKTCNT01 ;PKT CNT0..1**

**ATH:03502Ch - CHKSUM_ACC_RR_PKTCNT23 ;PKT CNT2..3**

```

| 0-8 PKTCNT0 / PKTCNT2 ;9bit each
 16-24 PKTCNT1 / PKTCNT3 |
| --- |

**ATH:035030h - CHKSUM_ACC_TXST_PKTCNT**

```

| 0-7 N/A ?
 8-15 PKTCNT1 ;8bit each
 16-23 PKTCNT2
 24-31 PKTCNT3 |
| --- |

**ATH:035034h - CHKSUM_ACC_DMARX_CONTROL**

```

| 0 RXEN
 1 LITTLEENDIAN |
| --- |

**ATH:035038h - CHKSUM_ACC_DMARX_DESC**

```

| 0-31 ADDR |
| --- |

**ATH:03503Ch - CHKSUM_ACC_DMARX_DESC_STATUS**

```

| 0 OVERFLOW
 1 BUSERROR
 2 DESC_INTR
 16-23 PKTCNT |
| --- |

**ATH:035040h - CHKSUM_ACC_INTR (R) ;ACC Interrupt (readonly)**

**ATH:035044h - CHKSUM_ACC_IMASK ;ACC Interrupt Mask**

```

| 0-3 RX_VAL
 4-16 TX_VAL |
| --- |

**ATH:035048h - CHKSUM_ACC_ARB_BURST**

```

| 0-9 MAX_TX
 10 INCR16_EN
 11 INCR8_EN
 16-25 MAX_RX |
| --- |

**ATH:035050h - CHKSUM_ACC_RESET_DMA**

```

| 0 TX
 1 RX |
| --- |

**ATH:035054h - CHKSUM_CONFIG**

```

| 0 CHKSUM_SWAP
 4-9 TXFIFO_MAX_TH
 16-21 TXFIFO_MIN_TH
 22-31 SPARE |
| --- |

```

| ___________________________________ MMAC ___________________________________ |
| --- |

**ATH:038000h - RX_FRAME0**

**ATH:038008h - RX_FRAME1**

```

| 0 OWN
 1-12 LEN (R)
 13-14 SEQ_NUM (R) |
| --- |

**ATH:038004h - RX_FRAME_0**

**ATH:03800Ch - RX_FRAME_1**

```

| 0-31 ADDR |
| --- |

**ATH:038010h - MMAC_INTERRUPT_RAW**

**ATH:038014h - MMAC_INTERRUPT_EN**

```

| 0 RX_DONE0
 1 RX_CRC_FAIL0
 2 ACK_RESP_FAIL0
 3 RX_DONE1
 4 RX_CRC_FAIL1
 5 ACK_RESP_FAIL1
 6 RX_ERR_OVERFLOW
 7 TX_DONE
 8 TX_DONE_ACK_MISSING
 9 TX_DONE_ACK_RECEIVED
 10 TX_ERROR |
| --- |

**ATH:038018h - RX_PARAM1**

```

| 0-31 VAP_ADDR_L |
| --- |

**ATH:03801Ch - RX_PARAM0**

```

| 0-15 VAP_ADDR_U
 16-21 SIFS
 22-23 CAPTURE_MODE
 24-26 TYPE_FILTER
 27 LIVE_MODE |
| --- |

**ATH:038020h - TX_COMMAND0**

```

| 0-11 LEN
 12 CRC
 13 EXP_ACK |
| --- |

**ATH:038024h - TX_COMMAND**

```

| 0-31 ADDR |
| --- |

**ATH:038028h - TX_PARAM**

```

| 0 ACK_MODE_EN
 1-6 ACK_TIMEOUT
 7-14 BACKOFF
 15 FORCE_ACKF_RSSI
 16-23 ACKF_RSSI |
| --- |

**ATH:03802Ch - BEACON_PARAM**

```

| 0-15 INTERVAL
 16-27 LEN
 28 EN
 29 CRC
 30 RESET_TS |
| --- |

**ATH:038030h - BEACON**

```

| 0-31 ADDR |
| --- |

**ATH:038034h - TSF_L**

**ATH:038038h - TSF_U**

```

| 0-31 COUNT |
| --- |

```

| ___________________________________ FPGA ___________________________________ |
| --- |

**ATH:039000h - FPGA_REG1**

```

| 2 DCM_RELEASE
 4-7 EMUL_RADIO_CLOCK_RATIO
 8-9 LONG_SHIFT_CHAIN_OVERRIDE_INDEX
 10 ENABLE_LONG_SHIFT_CHAIN_OVERRIDE_INDEX
 11-15 LONG_SHIFT_DRIVE_PHASE
 16-20 LONG_SHIFT_SAMPLE_PHASE
 21-30 SPARE_FPGA_REG1
 31 FPGA_SRIF_DELAY |
| --- |

**ATH:039004h - FPGA_REG2**

```

| 0-3 FPGA_PLATFORM_TYPE
 4-7 FPGA_IP_RELEASE_VERSION
 8-11 FPGA_IP_REVISION
 12 FPGA_OWL_PLL_ENABLED
 13 FPGA_LOOPBACK_I2C
 14-31 FPGA_SPARE |
| --- |

**ATH:039008h - FPGA_REG4**

```

| 0 RADIO_0_TCK
 1 RADIO_0_TDI
 2 RADIO_0_TMS
 3 RADIO_0_TDO |
| --- |

```

| _______________________________ BRIDGE INTR _______________________________ |
| --- |

**ATH:040000h - INTERRUPT**

**ATH:040004h - INTERRUPT_MASK**

```

| 0-7 RX_(0..7)_COMPLETE
 8-15 RX_(0..7)_END
 16-23 TX_(0..7)_COMPLETE
 24-31 TX_(0..7)_END |
| --- |

```

| ___________________________________ MII ___________________________________ |
| --- |

**ATH:040100h - MII0_CNTL**

```

| 0-1 SELECT
 2 MASTER
 4-5 SPEED
 8-9 RGMII_DELAY |
| --- |

**ATH:040104h - STAT_CNTL (whatever, MII related?)**

```

| 0 AUTOZ
 1 CLRCNT
 2 STEN
 3 GIG |
| --- |

```

| ___________________________________ MDIO ___________________________________ |
| --- |

**ATH:040200h - MDIO_REG[0..7]**

```

| 0-15 VALUE |
| --- |

**ATH:040220h - MDIO_ISR**

```

| 0-7 REGS
 8-15 MASK |
| --- |

**ATH:040224h - PHY_ADDR (whatever, MDIO related?)**

```

| 0-2 VAL |
| --- |

```

| _______________________________ BRIDGE RX/TX _______________________________ |
| --- |

**ATH:040800h - GMAC_RX_0_DESC_START_ADDRESS**

**ATH:040C00h - GMAC_TX_0_DESC_START_ADDRESS**

```

| 0-31 ADDRESS |
| --- |

**ATH:040804h - GMAC_RX_0_DMA_START**

**ATH:040C04h - GMAC_TX_0_DMA_START**

```

| 0 START
 4 RESTART |
| --- |

**ATH:040C08h - GMAC_TX_0_INTERRUPT_LIMIT**

```

| 0-3 COUNT
 4-15 TIMEOUT |
| --- |

**ATH:040808h - GMAC_RX_0_BURST_SIZE**

**ATH:040C0Ch - GMAC_TX_0_BURST_SIZE**

```

| 0-1 BURST |
| --- |

**ATH:04080Ch - GMAC_RX_0_PKT_OFFSET**

```

| 0-7 OFFSET |
| --- |

**ATH:040810h - GMAC_RX_0_CHECKSUM**

```

| 0 TCP
 1 UDP |
| --- |

**ATH:040814h - GMAC_RX_0_DBG_RX**

```

| 0-3 STATE |
| --- |

**ATH:040C10h - GMAC_TX_0_DBG_TX**

```

| 16-31 FIFO_TOTAL_LEN
 0-2 STATE |
| --- |

**ATH:040818h - GMAC_RX_0_DBG_RX_CUR_ADDR**

**ATH:040C14h - GMAC_TX_0_DBG_TX_CUR_ADDR**

```

| 0-31 ADDR |
| --- |

**ATH:04081Ch - GMAC_RX_0_DATA_SWAP**

**ATH:040C18h - GMAC_TX_0_DATA_SWAP**

```

| 0 SWAP
 1 SWAPD |
| --- |

```

| _________________________________ USB CAST _________________________________ |
| --- |

**ATH:054000h - ENDP0**

```

| 0-7 MAXP
 16 STALL
 17 HSNAK
 20 DSTALL
 23 CHGSETUP |
| --- |

**ATH:054008h - OUT1ENDP**

**ATH:054010h - OUT2ENDP**

**ATH:054018h - OUT3ENDP**

**ATH:054020h - OUT4ENDP**

**ATH:054028h - OUT5ENDP**

**ATH:05400Ch - IN1ENDP**

**ATH:054014h - IN2ENDP**

**ATH:05401Ch - IN3ENDP**

**ATH:054024h - IN4ENDP**

**ATH:05402Ch - IN5ENDP**

```

| 0-10 MAXP
 18-19 TYPE
 20-21 ISOD
 22 STALL
 23 VAL
 24 ISOERR
 28 HCSET ;<-- for INxENDP registers only (not OUTxENDP) |
| --- |

**ATH:05408Ch - USBMODESTATUS**

```

| 0 LS
 1 FS
 2 HS
 4 HOST
 5 DEVICE |
| --- |

**ATH:054188h - EPIRQ ;Endpoint Interrupt Request**

**ATH:054194h - EPIEN ;Endpoint Interrupt Enable**

```

| 0-15 IN 0..15
 16-31 OUT 0..15 |
| --- |

**ATH:05418Ch - USBIRQ ;USB Interrupt Request**

**ATH:054198h - PIEN ;P Interrupt Enable**

```

| 0 SUDAV IR
 1 SOF IR
 2 SUTOK IR
 3 SUSP IR
 4 URES IR
 5 HSPEED IR
 6 OVERFLOW IR
 7 LPM IR
 16-31 OUTP NGIRQ ? |
| --- |

**ATH:0541A4h - FNCTRL**

```

| 0-2 MFR
 3-7 FRMNR0
 8-13 FRMNR1
 16-22 FNADDR
 24-31 CLKGATE |
| --- |

**ATH:0541BCh - OTGREG**

```

| 0 OTGIRQ_IDLEIRQ
 1 OTGIRQ_SRPDETIRQ
 2 OTGIRQ_LOCSOFIRQ
 3 OTGIRQ_VBUSERRIRQ
 4 OTGIRQ_PERIPHIRQ
 8-11 OTGSTATE
 16 OTGCTRL_BUSREQ
 17 OTGCTRL_ABUSDROP
 18 OTGCTRL_ASETBHNPEN
 19 OTGCTRL_BHNPEN
 20 OTGCTRL_SRPVBUSDETEN
 21 OTGCTRL_SRPDATDETEN
 23 OTGCTRL_FORCEBCONN
 24 OTGSTATUS_BSE0SRP
 25 OTGSTATUS_CONN
 27 OTGSTATUS_ASESSVAL
 28 OTGSTATUS_BSESSEND
 29 OTGSTATUS_AVBUSVAL
 30 OTGSTATUS_ID |
| --- |

**ATH:0541CCh - DMASTART**

**ATH:0541D0h - DMASTOP**

```

| 0-15 IN 0..15
 16..31 OUT 0..15 |
| --- |

**ATH:054400h - EP0DMAADDR ;what is Endpoint 0, normal are 1..5, not 0?**

**ATH:054420h - EP1DMAADDR**

**ATH:054440h - EP2DMAADDR**

**ATH:054460h - EP3DMAADDR**

**ATH:054480h - EP4DMAADDR**

**ATH:0544A0h - EP5DMAADDR**

```

| 2-31 ADDR |
| --- |

**ATH:05442Ch - OUT1DMACTRL**

**ATH:05444Ch - OUT2DMACTRL**

**ATH:05446Ch - OUT3DMACTRL**

**ATH:05448Ch - OUT4DMACTRL**

**ATH:0544ACh - OUT5DMACTRL**

```

| 2-15 RINGSIZ
 16 ENDIAN
 17 DMASTOP
 18 DMASTART
 20 DMATUNLIM
 21 DMANINCR
 22 DMARING
 25 HLOCK
 26-27 HSIZE
 28-31 HRPROT |
| --- |

**ATH:0D8000h - USB_IP_BASE**

```

| ? |
| --- |

```

| ________________________________ I2C SLAVE ________________________________ |
| --- |

**ATH:054E00h - I2CFIFOCONTROL ;FIFO Control**

```

| 0 FIFO RESET
 1 FIFO PREFETCH
 2-4 FIFO READ LENGTH
 5-14 FIFO READ THRESHOLD
 15 FIFO READ STALL
 16-18 FIFO WRITE LENGTH
 19-28 FIFO WRITE THRESHOLD
 29 FIFO WRITE STALL |
| --- |

**ATH:054E04h - I2CFIFOREADPTR ;FIFO Read WR+RD Pointers**

**ATH:054E10h - I2CFIFOWRITEPTR ;FIFO Write WR+RD Pointers**

```

| 0-9 WR PTR (R)
 16-25 RD PTR (R) |
| --- |

**ATH:054E08h - I2CFIFOREADUPDATE ;FIFO Read Update**

**ATH:054E14h - I2CFIFOWRITEUPDATE ;FIFO Write Update**

```

| 0-10 UPDATE |
| --- |

**ATH:054E0Ch - I2CFIFOREADBASEADDR ;FIFO Read Base Address**

**ATH:054E18h - I2CFIFOWRITEBASEADDR ;FIFO Write Base Address**

```

| 0-31 BASE |
| --- |

**ATH:054E1Ch - I2CMEMCONTROL ;Mem Control**

```

| 0 RESET
 1 FLUSH |
| --- |

**ATH:054E20h - I2CMEMBASEADDR ;Mem Base Address**

```

| 0-31 BASE |
| --- |

**ATH:054E24h - I2CREGREADDATA ;Reg Read Data**

**ATH:054E28h - I2CREGWRITEDATA ;Reg Write Data (R) ;uh, write is read-only?**

```

| 0-31 DATA |
| --- |

**ATH:054E2Ch - I2CREGCONTROL ;Reg Control**

```

| 0 RESET
 1 READ STALL
 2 WRITE STALL
 3-5 READ COUNT (R)
 6-8 WRITE COUNT (R)
 9 READ EMPTY (R)
 10 WRITE FULL (R) |
| --- |

**ATH:054E30h - I2CCSRREADDATA ;Csr Read Data**

**ATH:054E34h - I2CCSRWRITEDATA ;Csr Write Data (R) ;uh, write is read-only?**

```

| 0-5 DATA (6bit, what is that?) |
| --- |

**ATH:054E38h - I2CCSRCONTROL ;Csr Control**

```

| 0-7 READDELAY
 8 CLOCKREQUESTENABLE
 9-11 FILTERCLOCKSELECT
 12-14 FILTERCLOCKSCALE
 15-17 FILTERSDARXSELECT
 18-20 FILTERSCLRXSELECT |
| --- |

**ATH:054E3Ch - I2CFILTERSIZE ;Filter Size**

```

| 0-7 SDA RX SIZE
 8-15 SCL RX SIZE |
| --- |

**ATH:054E40h - I2CADDR ;Address**

```

| 0-6 FIFO ADDR
 8-14 MEM ADDR
 16-22 REG ADDR
 24-30 CSR ADDR |
| --- |

**ATH:054E44h - I2CINT ;Interrupt Status**

**ATH:054E48h - I2CINTEN ;Interrupt Enable**

```

| 0 FIFO READ START INT ;\
 1 FIFO READ FINISH INT ;
 2 FIFO WRITE START INT ;
 3 FIFO WRITE FINISH INT ; R/W
 4 REG READ START INT ;
 5 REG READ FINISH INT ;
 6 REG WRITE START INT ;
 7 REG WRITE FINISH INT ;/
 8 FIFO READ EMPTY INT ;\
 9 FIFO WRITE FULL INT ; For Status: R
 10 FIFO READ THRESHOLD INT ; For Enable: R/W
 11 FIFO WRITE THRESHOLD INT ;/
 12 CSR INT ;-R/W |
| --- |

**ATH:054E4Ch - I2CINTCSR ;Int Csr**

```

| 0 INT ;Status (R)
 1 INTEN ;Enable |
| --- |

```

| _________________________________ MAP I2S _________________________________ |
| --- |

**ATH:055000h - MBOX_FIFO**

```

| 0-19 DATA |
| --- |

**ATH:055004h - MBOX_FIFO_STATUS**

```

| 0 FULL
 2 EMPTY |
| --- |

**ATH:055008h - MBOX_DMA_POLICY**

```

| 0 RX_ORDER
 1 RX_QUANTUM
 2 TX_ORDER
 3 TX_QUANTUM
 4-7 TX_FIFO_THRESH0 |
| --- |

**ATH:05500Ch - MBOX0_DMA_RX_DESCRIPTOR_BASE**

**ATH:055014h - MBOX0_DMA_TX_DESCRIPTOR_BASE**

```

| 2-27 ADDRESS |
| --- |

**ATH:055010h - MBOX0_DMA_RX_CONTROL**

**ATH:055018h - MBOX0_DMA_TX_CONTROL**

```

| 0 STOP
 1 START
 2 RESUME |
| --- |

**ATH:05501Ch - MBOX_FRAME**

```

| 0 RX_SOM
 2 RX_EOM |
| --- |

**ATH:055020h - FIFO_TIMEOUT**

```

| 0-7 VALUE
 8 ENABLE |
| --- |

**ATH:055024h - MBOX_INT_STATUS**

**ATH:055028h - MBOX_INT_ENABLE**

```

| 0 RX_NOT_FULL
 2 TX_NOT_EMPTY
 4 RX_UNDERFLOW
 5 TX_OVERFLOW
 6 TX_DMA_COMPLETE
 8 TX_DMA_EOM_COMPLETE
 10 RX_DMA_COMPLETE |
| --- |

**ATH:05502Ch - MBOX_FIFO_RESET**

```

| 0 TX_INIT
 2 RX_INIT |
| --- |

**ATH:055030h - MBOX_DEBUG_CHAIN0**

**ATH:055034h - MBOX_DEBUG_CHAIN1**

```

| 0-31 ADDRESS |
| --- |

**ATH:055038h - MBOX_DEBUG_CHAIN0_SIGNALS**

**ATH:05503Ch - MBOX_DEBUG_CHAIN1_SIGNALS**

```

| 0-31 COLLECTION |
| --- |

```

| __________________________________ MAP RF __________________________________ |
| --- |

**ATH:xxx400h - INT_PENDING[0..11]**

```

| 0-31 REG (32bit x 12 entries) |
| --- |

**ATH:xxx460h - INT_SRC (R)**

```

| 0-11 REG (12bit) (R) |
| --- |

**ATH:xxx430h - BB_WR_MASK_0**

**ATH:xxx434h - BB_WR_MASK_1**

**ATH:xxx438h - BB_WR_MASK_2**

**ATH:xxx43Ch - BB_WR_MASK_3**

**ATH:xxx448h - BB_RD_MASK_0**

**ATH:xxx44Ch - BB_RD_MASK_1**

**ATH:xxx450h - BB_RD_MASK_2**

**ATH:xxx454h - BB_RD_MASK_3**

```

| 0-10 REG (11bit) |
| --- |

**ATH:xxx440h - RF_WR_MASK_0**

**ATH:xxx444h - RF_WR_MASK_1**

**ATH:xxx458h - RF_RD_MASK_0**

**ATH:xxx45Ch - RF_RD_MASK_1**

```

| 0-8 REG (9bit) |
| --- |

**ATH:xxx000h - RAM1[0..255]**

```

| 0-7 DATA (8bit)
 8-31 - |
| --- |

**ATH:xxx800h - RAM2[0..127]**

```

| 0-6 DATA (7bit)
 7-31 - |
| --- |

```

| ___________________________________ ODIN ___________________________________ |
| --- |

**ATH:xxx000h - PHY_CTRL0**

```

| 0-2 PLL_ICP
 3-5 PLL_RS
 6-14 PLL_DIV
 15-17 PLL_MOD
 18 PLL_OVERIDE
 19 TEST_SPEED_SELECT
 20 RX_PATTERN_EN
 21 TX_PATTERN_EN
 22 ANA_LOOPBACK_EN
 23 DIG_LOOPBACK_EN
 24-31 LOOPBACK_ERR_CNT (R) |
| --- |

**ATH:xxx004h - PHY_CTRL1**

```

| 0-1 RX_FILBW_SEL
 2 RX_FORCERXON
 3 RX_BYPASSEQ
 4 RX_LOWR_PDET
 5-6 RX_SELIR_100M
 7 RX_SELVREF0P6
 8 RX_SELVREF0P25
 9-11 RX_RSVD
 12 NO_PLL_PWD
 13 FORCE_SUSPEND
 18-19 TX_PATTERN_SEL
 20 USE_PLL_LOCKDETECT
 21-22 USE_PLL_LOCK_DLY_SEL
 23-25 CLKOBS_SEL
 26 ENABLE_REFCLK_GATE
 27 DISABLE_CLK_GATING
 31 PLL_OBS_MODE_N |
| --- |

**ATH:xxx008h - PHY_CTRL2**

```

| 0 HSTXBIAS_PS_EN
 1 HSRXPHASE_PS_EN
 2-7 PWD_IPLL
 8-13 PWD_ISP
 20 TX_CAL_EN
 21 TX_CAL_SEL
 22-25 TX_MAN_CAL
 26 TX_LCKDET_OVR
 27-30 TX_RSVD
 31 PWD_EXTBIAS |
| --- |

**ATH:xxx00Ch - PHY_CTRL3**

```

| 0-18 PWD_ITX
 21 TX_DISABLE_SHORT_DET
 22-24 TX_SELTEST
 25 TX_STARTCAL |
| --- |

**ATH:xxx010h - PHY_CTRL4**

```

| 0-11 PWD_IRX |
| --- |

**ATH:xxx014h - PHY_CTRL5**

```

| 0-6 TX_BIAS_DELAY
 7-12 EB_WATERMARK
 13 FORCE_IDDQ
 14 FORCE_TEST_J
 15 FORCE_TEST_K
 16 FORCE_TEST_SE0_NAK
 17 TEST_JK_OVERRIDE
 18-19 XCVR_SEL
 20 TERM_SEL
 21 SUSPEND_N
 22 DP_PULLDOWN
 23 DM_PULLDOWN
 24 HOST_DISCON_FIX_ON
 25 HOST_DISCON_DETECT_ON
 26-28 HOST_DISCON_SAMPLE_WIDTH |
| --- |

**ATH:xxx018h - PHY_CTRL6**

```

| 0 AVALID
 1 BVALID
 2 VBUSVALID
 3 SESSEND
 4 IDDIG |
| --- |

**ATH:xxx01Ch - PHY_STATUS (R)**

```

| 0-3 TX_CAL (R) |
| --- |