# Dmaregisters

> Source: https://problemkaputt.de/gbatek.htm
> Section: Dmaregisters

3DS DMA Registers 
| |
| --- |

 **ARM9 NDMA**
 3DS DMA - NDMA Registers
 
 **ARM9 XDMA and ARM11 CDMA**
 3DS DMA - Corelink DMA Peripheral IDs
 3DS DMA - Corelink DMA Register Summary
 3DS DMA - Corelink DMA - Interrupt and Fault Status Registers
 3DS DMA - Corelink DMA - Internal State Status Registers
 3DS DMA - Corelink DMA - Transfer Start (aka "Debug" Registers)
 3DS DMA - Corelink DMA - Fixed Configuration and ID Registers
 3DS DMA - Corelink DMA Opcode Summary
 The ARM Corelink DMA hardware is documented in
 
```

| DDI0424D_dma330_r1p2_trm - DMA controller (also covers r0p0 and r1p1)
 IHI0022D amba axi_specification - Memory (ARCACHE, ARPROT etc.) |
| --- |

**ARM11 GPU DMA**

3DS GPU External Registers - Memfill and Memcopy

The GPU does probably also have DMA(s) for reading things like command lists,
and texture data, and for rendering to framebuffer.