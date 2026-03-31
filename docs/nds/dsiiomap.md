# Dsiiomap

> Source: https://problemkaputt.de/gbatek.htm
> Section: Dsiiomap

DSi I/O Map 
| |
| --- |

 **DSi Memory Map**
 The overall memory map is same as on NDS. New/changed areas are:
 
```

| 0000000h 64Kbyte ARM7 BIOS (unlike NDS which had only 16KB)
 2000000h 16MByte Main RAM (unlike NDS which had only 4MB)
 3000000h 800Kbyte Shared RAM (unlike NDS which had only 32KB)
 4004000h New DSi I/O Ports
 8000000h Fake GBA Slot (32MB+64KB) (FFh-filled; when mapped to current CPU)
 C000000h Mirror of 16Mbyte Main RAM
 D000000h Open Bus? in retail version, Extra 16Mbyte MainRAM in debug version
 FFFF000h 64Kbyte ARM9 BIOS (unlike NDS which had only 4KB) |
| --- |

**DSi I/O Maps**

The overall DSi I/O Maps are same as on NDS,

DS I/O Maps

additional new/changed registers are:

**ARM9 NDS Register that are changed in DSi mode**

```

| 4000004h 2 DISPSTAT (new Bit6, LCD Initialization Ready Flag)
 4000204h 2 EXMEMCNT (removed Bit0-7, ie. the GBA-slot related bits)
 4000210h 4 IE (new interrupt sources, removed GBA-slot IRQ)
 4000214h 4 IF (new interrupt sources, removed GBA-slot IRQ)
 40021A0h 4 Unknown, nonzero, probably same/silimar as on DSi7 side
 40021A4h 4 Unknown, zero, probably same/silimar as on DSi7 side
 40021A8h ..
 40021Bxh ..
 4102010h 4 |
| --- |

**ARM9 DSi System Control**

```

| 4004000h 2 SCFG_A9ROM DSi - NDS9 - ROM Status (R) [0000h]
 4004004h 2 SCFG_CLK DSi - NDS9 - New Block Clock Control (R/W)
 4004006h 2 SCFG_RST DSi - NDS9 - New Block Reset (R/W)
 4004008h 4 SCFG_EXT9 DSi - NDS9 - Extended Features (R/W)
 4004010h 2 SCFG_MC Memory Card Interface Status (16bit) (undocumented) |
| --- |

**ARM9 DSi New Shared WRAM Bank Control**

```

| 4004040h 4 MBK1 WRAM-A Slots for Bank 0,1,2,3 ;\Global ARM7+ARM9
 4004044h 4 MBK2 WRAM-B Slots for Bank 0,1,2,3 ; Slot Mapping
 4004048h 4 MBK3 WRAM-B Slots for Bank 4,5,6,7 ; (R or R/W, depending
 400404Ch 4 MBK4 WRAM-C Slots for Bank 0,1,2,3 ; on MBK9 setting)
 4004050h 4 MBK5 WRAM-C Slots for Bank 4,5,6,7 ;/
 4004054h 4 MBK6 WRAM-A Address Range ;\Local ARM9 Side
 4004058h 4 MBK7 WRAM-B Address Range ; (R/W)
 400405Ch 4 MBK8 WRAM-C Address Range ;/
 4004060h 4 MBK9 WRAM-A/B/C Slot Write Protect (R) |
| --- |

**ARM9 DSi New DMA (NDMA)**

```

| 4004100h 4 NDMAGCNT NewDMA Global Control ;-Control
 4004104h 4 NDMA0SAD NewDMA0 Source Address ;\
 4004108h 4 NDMA0DAD NewDMA0 Destination Address ;
 400410Ch 4 NDMA0TCNT NewDMA0 Total Length for Repeats ; NewDMA0
 4004110h 4 NDMA0WCNT NewDMA0 Logical Block Size ;
 4004114h 4 NDMA0BCNT NewDMA0 Block Transfer Timing/Interval ;
 4004118h 4 NDMA0FDATA NewDMA0 Fill Data ;
 400411Ch 4 NDMA0CNT NewDMA0 Control ;/
 4004120h 4 NDMA1SAD ;\
 4004124h 4 NDMA1DAD ;
 4004128h 4 NDMA1TCNT ; NewDMA1
 400412Ch 4 NDMA1WCNT ;
 4004130h 4 NDMA1BCNT ;
 4004134h 4 NDMA1FDATA ;
 4004138h 4 NDMA1CNT ;/
 400413Ch 4 NDMA2SAD ;\
 4004140h 4 NDMA2DAD ;
 4004144h 4 NDMA2TCNT ; NewDMA2
 4004148h 4 NDMA2WCNT ;
 400414Ch 4 NDMA2BCNT ;
 4004150h 4 NDMA2FDATA ;
 4004154h 4 NDMA2CNT ;/
 4004158h 4 NDMA3SAD ;\
 400415Ch 4 NDMA3DAD ;
 4004160h 4 NDMA3TCNT ; NewDMA3
 4004164h 4 NDMA3WCNT ;
 4004168h 4 NDMA3BCNT ;
 400416Ch 4 NDMA3FDATA ;
 4004170h 4 NDMA3CNT ;/ |
| --- |

**ARM9 DSi Camera Module**

```

| 4004200h 2 CAM_MCNT Camera Module Control (16bit)
 4004202h 2 CAM_CNT Camera Control (16bit)
 4004204h 4 CAM_DAT Camera Data (32bit)
 4004210h 4 CAM_SOFS Camera Trimming Starting Position Setting (32bit)
 4004214h 4 CAM_EOFS Camera Trimming Ending Position Setting (32bit) |
| --- |

**ARM9 DSi DSP - XpertTeak processor**

```

| 4004300h 2 DSP_PDATA DSP Transfer Data
 4004304h 2 DSP_PADR DSP Transfer Address
 4004308h 2 DSP_PCFG DSP Configuration
 400430Ch 2 DSP_PSTS DSP Status
 4004310h 2 DSP_PSEM DSP ARM9-to-DSP Semaphore (R/W)
 4004314h 2 DSP_PMASK DSP DSP-to-ARM9 Semaphore Mask (R/W)
 4004318h 2 DSP_PCLEAR DSP DSP-to-ARM9 Semaphore Clear (W)
 400431Ch 2 DSP_SEM DSP DSP-to-ARM9 Semaphore Data (R)
 4004320h 2 DSP_CMD0 DSP ARM9-to-DSP Command Register 0 (R/W)
 4004324h 2 DSP_REP0 DSP DSP-to-ARM9 Reply Register 0 (R)
 4004328h 2 DSP_CMD1 DSP ARM9-to-DSP Command Register 1 (R/W)
 400432Ch 2 DSP_REP1 DSP DSP-to-ARM9 Reply Register 1 (R)
 4004330h 2 DSP_CMD2 DSP ARM9-to-DSP Command Register 2 (R/W)
 4004334h 2 DSP_REP2 DSP DSP-to-ARM9 Reply Register 2 (R)
 4004340h C0h DSP_mirror Mirrors of above 40h-byte DSP register area |
| --- |

**ARM7 DSi**

```

| 4000004h 2 DISPSTAT (new Bit6, LCD Initialization Ready Flag) (as DSi9?)
 40001C0h 2 SPICNT (new Bit2, for 8MHz transfer clock)
 4000204h 2 EXMEMCNT (removed Bit0-7: GBA-slot related bits) (as DSi9?)
 4000210h 4 IE (new interrupt sources, removed GBA-slot IRQ)
 4000214h 4 IF (new interrupt sources, removed GBA-slot IRQ)
 4000218h IE2 (new register with more new interrupt sources)
 400021Ch IF2 (new register with more new interrupt sources) |
| --- |

**ARM7 DSi Maybe 2nd ROM slot (DSi prototypes did have 2 cartridge slots)**

```

| 40021A0h 4 Unknown, nonzero, probably related to below 40021A4h
 40021A4h 4 Unknown, related to 40001A4h (Gamecard Bus ROMCTRL)
 40021A8h ..
 40021Bxh ..
 4102010h 4 |
| --- |

**ARM7 DSi System Control**

```

| 4004000h 1 SCFG_A9ROM used by BIOS and SystemFlaw (bit0,1)
 4004001h 1 SCFG_A7ROM used by BIOS and SystemFlaw (bit0,1,2)
 4004004h 2 SCFG_CLK7 used by SystemFlaw
 4004006h 2 SCFG_JTAG Debugger Control
 4004008h 4 SCFG_EXT7 used by SystemFlaw
 4004010h 2 SCFG_MC Memory Card Interface Control (R/W) ;\
 4004012h 2 SCFG_CARD_INSERT_DELAY (usually 1988h = 100ms) ; Game Cartridge
 4004014h 2 SCFG_CARD_PWROFF_DELAY (usually 264Ch = 150ms) ;/
 4004020h 2 SCFG_WL Wireless Disable ;bit0 = wifi?
 4004024h 2 SCFG_OP Debugger Type (R) ;bit0-1 = (0=retail, ?=debug) |
| --- |

**ARM7 DSi New Shared WRAM Bank Control**

```

| 4004040h 4 MBK1 WRAM-A Slots for Bank 0,1,2,3 ;\
 4004044h 4 MBK2 WRAM-B Slots for Bank 0,1,2,3 ; Global ARM7+ARM9
 4004048h 4 MBK3 WRAM-B Slots for Bank 4,5,6,7 ; Slot Mapping (R)
 400404Ch 4 MBK4 WRAM-C Slots for Bank 0,1,2,3 ; (set on ARM9 side)
 4004050h 4 MBK5 WRAM-C Slots for Bank 4,5,6,7 ;/
 4004054h 4 MBK6 WRAM-A Address Range ;\Local ARM7 Side
 4004058h 4 MBK7 WRAM-B Address Range ; (R/W)
 400405Ch 4 MBK8 WRAM-C Address Range ;/
 4004060h 4 MBK9 WRAM-A/B/C Slot Write Protect (R/W) |
| --- |

**ARM7 DSi New DMA (NDMA)**

```

| 4004100h 74h NewDMA (new DMA, as on ARM9i, see there) |
| --- |

**ARM7 DSi AES Encryption Unit**

```

| 4004400h 4 AES_CNT (R/W)
 4004404h 4 AES_BLKCNT (W)
 4004408h 4 AES_WRFIFO (W)
 400440Ch 4 AES_RDFIFO (R)
 4004420h 16 AES_IV (W)
 4004430h 16 AES_MAC (W)
 4004440h 48 AES_KEY0 (W) ;used for modcrypt
 4004470h 48 AES_KEY1 (W) ;used for ?
 40044A0h 48 AES_KEY2 (W) ;used for JPEG signatures
 40044D0h 48 AES_KEY3 (W) ;used for eMMC sectors |
| --- |

**ARM7 DSi I2C Bus**

```

| 4004500h 1 I2C_DATA
 4004501h 1 I2C_CNT |
| --- |

**ARM7 DSi Microphone**

```

| 4004600h 2 MIC_CNT Microphone Control
 4004604h 4 MIC_DATA Microphone FIFO |
| --- |

**ARM7 DSi SNDEX**

```

| 4004700h 2 SNDEXCNT <-- can be read even in DS mode! |
| --- |

**ARM7 DSi SD/MMC Registers for Memory Card access (SD Card and onboard eMMC)**

```

| 4004800h 2 SD_CMD Command and Response/Data Type
 4004802h 2 SD_CARD_PORT_SELECT (SD/MMC:020Fh, SDIO:010Fh)
 4004804h 4 SD_CMD_PARAM0-1 Argument (32bit, 2 halfwords)
 4004808h 2 SD_STOP_INTERNAL_ACTION
 400480Ah 2 SD_DATA16_BLK_COUNT "Transfer Block Count"
 400480Ch 16 SD_RESPONSE0-7 (128bit, 8 halfwords)
 400481Ch 4 SD_IRQ_STATUS0-1 ;IRQ Status (0=ack, 1=req)
 4004820h 4 SD_IRQ_MASK0-1 ;IRQ Disable (0=enable, 1=disable)
 4004824h 2 SD_CARD_CLK_CTL Card Clock Control
 4004826h 2 SD_DATA16_BLK_LEN Memory Card Transfer Data Length
 4004828h 2 SD_CARD_OPTION Memory Card Option Setup (can be C0FFh)
 400482Ah 2 Fixed always zero?
 400482Ch 4 SD_ERROR_DETAIL_STATUS0-1 Error Detail Status
 4004830h 2 SD_DATA16_FIFO Data Port (SD_FIFO?)
 4004832h 2 Fixed always zero? ;(TC6371AF:BUF1 Data MSBs?)
 4004834h 2 SD_CARD_IRQ_CTL ;(SD_TRANSACTION_CTL)
 4004836h 2 SD_CARD_IRQ_STAT ;(SD_CARD_INTERRUPT_CONTROL)
 4004838h 2 SD_CARD_IRQ_MASK ;(SDCTL_CLK_AND_WAIT_CTL)
 400483Ah 2 Fixed always zero? ;(SDCTL_SDIO_HOST_INFORMATION)
 400483Ch 2 Fixed always zero? ;(SDCTL_ERROR_CONTROL)
 400483Eh 2 Fixed always zero? ;(TC6387XB: LED_CONTROL)
 4004840h 2 Fixed always 003Fh?
 4004842h 2 Fixed always 002Ah?
 4004844h 6Eh Fixed always zerofilled?
 40048B2h 2 Fixed always FFFFh?
 40048B4h 6 Fixed always zerofilled?
 40048BAh 2 Fixed always 0200h?
 40048BCh 1Ch Fixed always zerofilled?
 40048D8h 2 SD_DATA_CTL
 40048DAh 6 Fixed always zerofilled?
 40048E0h 2 SD_SOFT_RESET Software Reset (bit0=SRST=0=reset)
 40048E2h 2 Fixed always 0009h? ;(RESERVED2/9, TC6371AF:CORE_REV)
 40048E4h 2 Fixed always zero?
 40048E6h 2 Fixed always zero? ;(RESERVED3, TC6371AF:BUF_ADR)
 40048E8h 2 Fixed always zero? ;(TC6371AF:Resp_Header)
 40048EAh 6 Fixed always zerofilled?
 40048F0h 2 Fixed always zero? ;(RESERVED10)
 40048F2h 2 ? Can be 0003h
 40048F4h 2 ? Can be 0770h
 40048F6h 2 SD_WRPROTECT_2 (R) ;Write protect for eMMC (RESERVED4)
 40048F8h 4 SD_EXT_IRQ_STAT0-1 ;Insert/eject for eMMC (RESERVED5-6)
 40048FCh 4 SD_EXT_IRQ_MASK0-1 ;(TC6371AF:Revision) (RESERVED7-8)
 4004900h 2 SD_DATA32_IRQ
 4004902h 2 Fixed always zero?
 4004904h 2 SD_DATA32_BLK_LEN
 4004906h 2 Fixed always zero?
 4004908h 2 SD_DATA32_BLK_COUNT
 400490Ah 2 Fixed always zero?
 400490Ch 4 SD_DATA32_FIFO
 4004910h F0h Fixed always zerofilled? |
| --- |

**ARM7 DSi SD/MMC Registers for SDIO access (for Atheros Wifi)**

```

| 4004A00h 512 SDIO_xxx (same as SD_xxx at 4004800h..40049FFh, see there)
 4004A02h 2 SDIO_CARD_PORT_SELECT (slightly different than 4004802h) |
| --- |

**ARM7 DSi General Purpose I/O (GPIO) (headphone connect, power button)**

```

| 4004C00h 1 GPIO Data In (R) (even in DS mode)
 4004C00h 1 GPIO Data Out (W)
 4004C01h 1 GPIO Data Direction (R/W)
 4004C02h 1 GPIO Interrupt Edge Select (R/W)
 4004C03h 1 GPIO Interrupt Enable (R/W)
 4004C04h 2 GPIO_WIFI (R/W) |
| --- |

**ARM7 DSi CPU/Console ID (used as eMMC key)**

```

| 4004D00h 8 CPU/Console ID Code (64bit) (R)
 4004D08h 2 CPU/Console ID Flag (1bit) (R) |
| --- |

**ARM7 DSi Junk?**

```

| 8030200h 2 GBA area, accessed alongsides with SDIO port [4004A30h] (bug?) |
| --- |

**Main Memory Control**

```

| 2FFFFFEh 2 Main Memory Control (for 16MByte RAM chip)
 DFFFFFEh 2 Main Memory Control (extra 16MByte RAM chip in debug version) |
| --- |