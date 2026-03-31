# Gpumemoryandiomap

> Source: https://problemkaputt.de/gbatek.htm
> Section: Gpumemoryandiomap

3DS GPU Memory and I/O Map 
| |
| --- |

 **GPU Overall Memory and I/O Map**
 All registers at 10202xxxh, 10400xxxh, 10401xxxh can be read/written by ARM11 opcodes, the 10401xxxh registers can be also written via command numbers 0000h..03FFh in PICA command lists.
 
```

| 10202000h 18h LCD Config/flags ;\
 10202018h 1E8h Unused (0) ? ;
 10202200h 600h LCD Configuration for Top Screen ; LCD
 10202800h 200h Unused (0) ? ;
 10202A00h 600h LCD Configuration for Bottom Screen ;/
 10400000h 10h GPU Memory Control/Status Registers (part 1) ;\
 10400010h 10h DMA Memory Fill 0 DMA "PSC0" ;
 10400020h 10h DMA Memory Fill 1 DMA "PSC1" ;
 10400030h D0h GPU Memory Control/Status Registers (part 2) ; External
 10400100h 300h Unused (mirrors of above 100h bytes) ;
 10400400h 100h LCD Framebuffer Setup "PDC0" (top screen) ;
 10400500h 100h LCD Framebuffer Setup "PDC1" (bottom screen) ;
 10400600h 200h Unknown/DANGER (hangs when reading?) ;
 10400800h 400h Unknown/Unused (data abort) ;
 10400C00h 100h DMA Memory Copy DMA Transfer Engine "DMA" ;
 10400D00h 300h Unused (mirrors of above 100h bytes) ;/
 10401000h 1000h Internal Command Registers PICA(0000h..03FFh) ;-Internal
 10402000h 2000h Unused (data abort)
 10404000h 1C000h Write-only mirrors of above 4000h bytes (read=data abort)
 18000000h 300000h VRAM area, usually VRAM_A (3Mbyte)
 18300000h 300000h VRAM area, usually VRAM_B (3Mbyte)
 18600000h 600000h VRAM area, usually empty/zerofilled (6Mbyte) |
| --- |

Apart from VRAM, the GPU can also access other memory like FCRAM, AXI, QTM (but
that's most probably slower; and may be problematic when using virtual memory
allocation on ARM11 side).