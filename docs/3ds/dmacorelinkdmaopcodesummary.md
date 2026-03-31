# Dmacorelinkdmaopcodesummary

> Source: https://problemkaputt.de/gbatek.htm
> Section: Dmacorelinkdmaopcodesummary

3DS DMA - Corelink DMA Opcode Summary 
| |
| --- |

 **Corelink DMA Opcode Summary**
 
```

| Opcode CM Syntax
 00h CM DMAEND ;-done (finish transfer)
 01h CM DMAKILL ;-abort (unfinished transfer)
 04h C- DMALD ;\
 05h C- DMALDS ;\cond ; load from SAR (to dmafifo)
 07h C- DMALDB ;/ ;/
 08h C- DMAST ;\
 09h C- DMASTS ;\cond ; store to DAR (from dmafifo)
 0Bh C- DMASTB ;/ ;/
 0Ch C- DMASTZ ;-store zero to DAR
 12h C- DMARMB ;-read memory barrier
 13h C- DMAWMB ;-write memory barrier
 18h CM DMANOP ;-no operation (align padding)
 20h,len-1 C- DMALP lpc0,len ;\loop start with loop count
 22h,len-1 C- DMALP lpc1,len ;/
 25h,periph*8 C- DMALDPS periph ;\ ;\load and notify peripheral
 27h,periph*8 C- DMALDPB periph ; cond ;/
 29h,periph*8 C- DMASTPS periph ; ;\store and notify peripheral
 2Bh,periph*8 C- DMASTPB periph ;/ ;/
 2Ch,rel_addr C- DMALPEND ;-loop end for DMALPFE
 30h,periph*8 C- DMAWFP periph,single ;\
 31h,periph*8 C- DMAWFP periph,periph ; wait for peripheral
 32h,periph*8 C- DMAWFP periph,burst ;/
 34h,event*8 CM DMASEV event ;-send event (or interrupt)
 35h,periph*8 C- DMAFLUSHP periph ;-init peripheral
 36h,event*8+00h CM DMAWFE event ;\wait for event (with optional
 36h,event*8+02h CM DMAWFE event,invalid ;/ code cache invalidate)
 38h,rel_addr C- DMALPEND lpc0 ;\
 39h,rel_addr C- DMALPENDS lpc0 ;\cond ;
 3Bh,rel_addr C- DMALPENDB lpc0 ;/ ; loop end for DMALP lpc0/1
 3Ch,rel_addr C- DMALPEND lpc1 ;
 3Dh,rel_addr C- DMALPENDS lpc1 ;\cond ;
 3Fh,rel_addr C- DMALPENDB lpc1 ;/ ;/
 54h,imm16 C- DMAADDH SAR,imm16 ;\add halfword
 56h,imm16 C- DMAADDH DAR,imm16 ;/ ("positive")
 5Ch,imm16 C- DMAADNH SAR,imm16 ;\add imm16+0FFFF0000h
 5Eh,imm16 C- DMAADNH DAR,imm16 ;/ ("negative", rev1 only)
 A0h,channel,imm32 -M DMAGO channel,imm32 ;\jump/goto entrypoint
 A2h,channel,imm32 -M DMAGO channel,imm32,ns ;/ (ns=non-secure mode)
 BCh,00h,imm32 C- DMAMOV SAR,imm32 ;\
 BCh,01h,imm32 C- DMAMOV CCR,imm32 ; move to register
 BCh,02h,imm32 C- DMAMOV DAR,imm32 ;/
 imm8 -- DCB imm8 ;\manually defined 'code'
 imm32 -- DCD imm32 ;/
 - (no opcode) C- DMALPFE ;loop forever ;-loop start without loop count |
| --- |

All opcodes and parameters (except imm16/imm32) are 8bit wide. The CM column
indicates if the opcode can be used in Channel or Manager threads (or both).

The rel_addr parameters are unsigned offsets (jump target is $-rel_addr).

**Nocash Syntax**

Allows MOV instead LP, and LOOP instead LPEND, and "ADD reg,+/-imm" instead of
"ADDH/ADNH reg,imm".