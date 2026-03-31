# Gpuinternalregistersfinalizeinterruptregisters

> Source: https://problemkaputt.de/gbatek.htm
> Section: Gpuinternalregistersfinalizeinterruptregisters

3DS GPU Internal Registers - Finalize Interrupt registers 
| |
| --- |

 **10401000h+i - PICA(N/A) - GPUREG_IRQ_ACK(0..63) (R/W)**
 **10401040h+i - PICA(0010h..001Fh) - GPUREG_IRQ_REQ(0..63) (R/W)**
 **10401080h+i - PICA(N/A) - GPUREG_IRQ_CMP(0..63) (R/W)**
 
```

| 0-7 Request/Compare Byte for IRQ#0 ;\
 8-15 Request/Compare Byte for IRQ#1 ; triggers "P3D" IRQ 2Dh if any
 16-23 Request/Compare Byte for IRQ#2 ; of the request/compare bytes
 .. .. ; are same (and enabled in MASK)
 504-511 Request/Compare Byte for IRQ#63 ;/ |
| --- |

The ACK registers are basically mirrors of the REQ registers (and ARM11 could
write either one, causing IRQ acknowledge or request depending on whether the
written value does match the CMP compare bytes). However, there are at least
two small differences between ACK and REQ registers:

```

| GPU command list's can only write to GPUREG_IRQ_REQ
 ARM11 reads from GPUREG_IRQ_REQ cause ARM11 to wait if GPU is busy |
| --- |

Normally, CMP contains constant values, and REQ/ACK are changed to
same/different values when requesting/releasing interrupt requests.

**104010C0h - PICA(N/A) - GPUREG_IRQ_MASK_LOW (R/W)**

**104010C4h - PICA(N/A) - GPUREG_IRQ_MASK_HIGH (R/W)**

```

| 0-63 Interrupt Disable flags for IRQ#0..63 (0=Enable, 1=Disable) |
| --- |

**104010C8h - PICA(N/A) - GPUREG_IRQ_STAT_LOW (R)**

**104010CCh - PICA(N/A) - GPUREG_IRQ_STAT_HIGH (R)**

```

| 0-63 Interrupt Compare flags for IRQ#0..63 (0=Mismatch, 1=Match/IRQ) |
| --- |

**104010D0h - PICA(N/A) - GPUREG_IRQ_AUTOSTOP (R/W)**

```

| 0 Autostop upon REQ=CMP match (0=IRQ only, 1=IRQ and Stop cmdlist)
 1-31 Unused (0) |
| --- |

**104010D4h - PICA(N/A) - GPUREG_FIXED_00010002h (R)**

```

| 0-31 Unknown, readonly, always 00010002h (some status, or chip id or so?) |
| --- |

Maybe related: Port 10400000h contains the same 00010002h readonly value.

**Notes**

Nintendo uses the IRQ registers to trigger a "FINALIZE" interrupt at end of
command list, done as so:

```

| GPUREG_IRQ_MASK_LOW=FFFFFFF0h set upon GPU init ;\enable first four IRQs
 GPUREG_IRQ_MASK_HIGH=FFFFFFFFh set upon GPU init ;/
 GPUREG_IRQ_AUTOSTOP=1 ;-autostop cmdlist upon IRQ
 GPUREG_IRQ_CMP(0)=12345678h set upon GPU init ;-first four compare values
 GPUREG_IRQ_ACK(0)=00000000h set upon GPU init and before cmdlist
 GPUREG_IRQ_REQ(0)=12345678h set via PICA(0010h) FINALIZE at end of cmdlist |
| --- |

Due to the 32bit writes, that will actually trigger four byte-matches at once.

Caution:

The GPU command list can write to GPUREG_IRQ_REQ(n), but all other registers in
range PICA(0000h..003Fh) are writeable by CPU only, not via command lists.