# x of total num_filters

> Source: https://problemkaputt.de/gbatek.htm
> Section: x of total num_filters

DSi Atheros Wifi SDIO Interface 
| |
| --- |

 **AR6002 SDIO Registers**
 DSi Atheros Wifi SDIO Function 0 Register Summary
 DSi Atheros Wifi SDIO Function 1 Register Summary
 DSi Atheros Wifi - SDIO Function 1 I/O - mbox_wlan_host_reg
 DSi Atheros Wifi Misc
 For general info about SDIO protocol and I/O ports, and SDIO Function 0, see
 DSi SD/MMC Protocol and I/O Ports
 
 **Transfer Protocol (Commands/Events)**
 DSi Atheros Wifi - Command Summary
 DSi Atheros Wifi - Response Summary
 DSi Atheros Wifi - Host Interest Area in RAM
 DSi Atheros Wifi - BMI Bootloader Commands
 DSi Atheros Wifi - MBOX Transfer Headers
 DSi Atheros Wifi - WMI Misc Commands
 DSi Atheros Wifi - WMI Misc Events
 DSi Atheros Wifi - WMI Connect Functions
 DSi Atheros Wifi - WMI Channel and Cipher Functions
 DSi Atheros Wifi - WMI Scan Functions
 DSi Atheros Wifi - WMI Bit Rate Functions
 DSi Atheros Wifi - WMI Threshold Functions
 DSi Atheros Wifi - WMI Error, Retry and Debug Functions
 DSi Atheros Wifi - WMI Priority Stream Functions
 DSi Atheros Wifi - WMI Roam Functions
 DSi Atheros Wifi - WMI Power Functions
 DSi Atheros Wifi - WMI Statistics Function
 DSi Atheros Wifi - WMI Bluetooth Coexistence (older AR6002)
 DSi Atheros Wifi - WMI Wake on Wireless (WOW) Functions
 DSi Atheros Wifi - WMI General Purpose I/O (GPIO) Functions
 Additional WMI Functions (NOT implemented in DSi with AR6002, but maybe exist in DSi/3DS with AR6013/AR6014):
 DSi Atheros Wifi - Unimplemented WMI Misc Functions
 DSi Atheros Wifi - Unimplemented WMI Bluetooth Coexistence (newer AR6002)
 DSi Atheros Wifi - Unimplemented WMI Bluetooth Coexistence (AR6003)
 DSi Atheros Wifi - Unimplemented WMI DataSet Functions
 DSi Atheros Wifi - Unimplemented WMI AP Mode Functions (exists on 3DS)
 DSi Atheros Wifi - Unimplemented WMI DFS Functions
 DSi Atheros Wifi - Unimplemented WMI P2P Functions
 DSi Atheros Wifi - Unimplemented WMI WAC Functions
 DSi Atheros Wifi - Unimplemented WMI RF Kill and Store/Recall Functions
 DSi Atheros Wifi - Unimplemented WMI THIN Functions
 DSi Atheros Wifi - Unimplemented WMI Pyxis Functions
 Aside from WMI Commands/Events, it should be obviously also possible to transfer actual data packets, but unknown to do that... maybe it's done through MBOX0 too, and maybe related to WMI_DATA_HDR, WMI_TX_META_V0..3, WMI_RX_META_V0..2 in "wmi.h" and/or to stuff in "htc.h".
 
 DSi Atheros Wifi I2C EEPROM
 
 
 
| DSi Atheros Wifi SDIO Function 0 Register Summary |
| --- |

 **Atheros SDIO "Function 0" area**
 
```

| 0:00000 2 Revision (0011h = CCCRv1.10, SDIOv1.10, SDv1.01) ;\
 0:00002 2 Function (0202h = Function 1 enabled/ready) ;
 0:00004 2 Interrupt Flags(0000h = None enabled/pending) ;
 0:00006 1 Abort/Reset (00h) ;
 0:00007 1 Bus Interface (82h = 4bit mode, pulldown=off) ;
 0:00008 1 Card Capability(17h) ; CCCR
 0:00009 3 CIS0 Pointer (001000h = CIS0 at 0:01000h) ;
 0:0000C .. ..suspend..? (zero-filled) ;
 0:00010 2 Block Size (0000h = Function 0 Block Size, variable);
 0:00012 1 Power Control (03h = supports/uses more than 720mW) ;
 0:00013 2 Bus Speed (0000h = Supports only SDR12) ;
 0:00015 1 Driver Strength(00h) ;
 0:00016 1 Interrupt Ext (00h = No aysnc IRQ support in 4bit mode);
 0:00017 E9h Reserved (zero-filled) ;/
 0:00100 2 Interface Type (0000h=Not SDIO standard, no CSA) ;\
 0:00102 1 Power (00h=No power selection) ;
 0:00103 6 Reserved (zero-filled) ;
 0:00109 3 CIS1 Pointer (001100h = CIS1 at 0:01100h) ; FBR1
 0:0010C 4 CSA Stuff (zero-filled, CSA isn't supported) ;
 0:00110 2 Block Size (0080h = Function 1 Block Size, variable);
 0:00112 EEh Reserved (zero-filled) ;/
 0:00200 600h FBR2..FBR7 (zero-filled) ;-FBRn
 0:00800 800h Reserved (zero-filled) ;-N/A
 0:01000 01 03 D9 01 FF ;DEVICE (D9h=FUNCSPEC,01h=Siz,FFh=End;\
 0:01005 20 04 71 02 00 02 ;MANFID (0271h=Atheros, 0200h=AR6002);
 0:0100B 21 02 0C 00 ;FUNCID (0Ch,00h=Standard for SDIO) ;
 0:0100F 22 04 00 00 08 32 ;FUNCE (0800h=MaxBlkSiz,32h=25Mbit/s); CIS0
 0:01015 1A 05 01 01 00 02 07 ;\ ;CONFIG ;
 0:0101C 1B 08 C1 41 30 30 FF FF 32 00 ; PROM? ;CFTABLE_ENTRY;
 0:01026 14 00 ; RAM? ;NO_LINK ;
 0:01028..01044 FF-filled (1Dh bytes) ;uh? ;/ ;END ;
 0:01045..010FF 00-filled (BBh bytes) ;unused ;/
 0:01100 20 04 71 02 00 02 ;MANFID (0271h=Atheros, 0200h=AR6002);\
 0:01106 21 02 0C 00 ;FUNCID (0Ch,00h=Standard for SDIO) ;
 0:0110A 22 2A 01 ;FUNCE ;
 0:0110D 01 11 ;FUNCE WakeUpSupport(01h), v1.1(11h) ;
 0:0110F 00 00 00 00 ;FUNCE Serial Number (00000000h=None) ;
 0:01113 00 00 00 00 00 ;FUNCE CSA Stuff (00000000h,00h=None) ;
 0:01118 00 08 ;FUNCE Max Block Size (0800h) ; CIS1
 0:0111A 00 00 FF 80 ;FUNCE OCR (80FF0000h) ;
 0:0111E 00 00 00 ;FUNCE Operate Min/Avg/Max (00,00,00) ;
 0:01121 00 01 0A ;FUNCE Standby Min/Avg/Max (00,01,0A) ;
 0:01124 00 00 00 00 ;FUNCE Bandwidth Min/Opt (0000h,0000h) ;
 0:01128 00 00 ;FUNCE Timeout Enable-till-Rdy (0000h) ;
 0:0112A 00 00 00 00 ;FUNCE Operation Avg/Max (0000h,0000h);
 0:0112E 00 01 00 01 ;FUNCE HighCurrentAvg/Max (0100h,0100h);
 0:01132 00 01 00 01 ;FUNCE LowCurrent Avg/Max (0100h,0100h);
 0:01136 80 01 06 ;VENDOR ;
 0:01139 81 01 07 ;VENDOR ;
 0:0113C 82 01 DF ;VENDOR ;
 0:0113F FF ;END ;
 0:01140 01 ;Garbage? ;
 0:01141..011FF 00-filled (BFh bytes) ;unused ;/
 0:01200..02FFF mirrors of 01000h..011FFh (CIS0 and CIS1) (1E00h bytes);\N/A
 0:03000.. 00-filled (.... bytes) ;unused... reserved ;/ |
| --- |

**Briefly**

```

| 0:00000 11 00 02 02 00 00 00 82 17 00 10 00 00 00 00 00 ;\
 0:00010 00 00 03 00 00 00 00 ;
 0:00017..000FF unused (zerofilled) ;/
 0:00100 00 00 00 00 00 00 00 00 00 00 11 00 00 00 00 00 ;\
 0:00110 80 00 ;
 0:00112..00FFF unused (zerofilled) ;/
 0:01000 01 03 D9 01 FF ;\
 0:01005 20 04 71 02 00 02 ;hif.h: 271h ;
 0:0100B 21 02 0C 00 ;
 0:0100F 22 04 00 00 08 32 ;
 0:01015 1A 05 01 01 00 02 07 ;\ ;
 0:0101C 1B 08 C1 41 30 30 FF FF 32 00 ; PROM? ;
 0:01026 14 00 ; RAM? ;
 0:01028..01044 FF-filled (1Dh bytes) uh? ;/ ;
 0:01045..010FF 00-filled ;/
 0:01100 20 04 71 02 00 02 ;\
 0:01106 21 02 0C 00 ;
 0:0110A 22 2A 01 01 11 00 00 00 00 00 00 00 00 00 00 08 00 00
 0:0111C FF 80 00 00 00 00 01 0A 00 00 00 00 00 00 00 00
 0:0112C 00 00 00 01 00 01 00 01 00 01
 0:01136 80 01 06 ;
 0:01139 81 01 07 ;
 0:0113C 82 01 DF ;
 0:0113F FF 01 00 ;
 0:01142..011FF 00-filled ;/
 0:01200..02FFF mirrors of 01000..011FF (common cis and function 1 cis) ?
 0:03000.. 00-filled |
| --- |

| DSi Atheros Wifi SDIO Function 1 Register Summary |
| --- |

**Atheros SDIO "Function 1" area**

```

| 1:00000..000FF Mbox0 (100h bytes) <--DMA----------> Internal 256MB
 1:00100..001FF Mbox1 (100h bytes) <--DMA----------> Internal 256MB
 1:00200..002FF Mbox2 (100h bytes) <--DMA----------> Internal 256MB
 1:00300..003FF Mbox3 (100h bytes) <--DMA----------> Internal 256MB
 1:00400..005FF Control Registers <--WINDOW_DATA--> Internal 256MB
 1:00600..007FF CIS Window; Window ---huh???-------> Internal 256MB ???
 1:00800..00FFF Mbox0 Alias (bigger 800h bytes alias)
 1:01000..017FF Mbox1 Alias (bigger 800h bytes alias)
 1:01800..01FFF Mbox2 Alias (bigger 800h bytes alias)
 1:02000..027FF Mbox3 Alias (bigger 800h bytes alias)
 1:02800..03FFF Extra Mbox0 Alias "for future usage" (1800h bytes)
 1:04000..1FFFF Unspecified |
| --- |

**mbox_wlan_host_reg.h -- in SDIO Function 1 address space**

```

| 1:00000h 100h Mbox0 (100h bytes) <--DMA----------> Internal 256MB
 1:00100h 100h Mbox1 (100h bytes) <--DMA----------> Internal 256MB
 1:00200h 100h Mbox2 (100h bytes) <--DMA----------> Internal 256MB
 1:00300h 100h Mbox3 (100h bytes) <--DMA----------> Internal 256MB
 1:00400h 1 HOST_INT_STATUS (R)
 1:00401h 1 CPU_INT_STATUS (R/W)
 1:00402h 1 ERROR_INT_STATUS (R/W)
 1:00403h 1 COUNTER_INT_STATUS (R)
 1:00404h 1 MBOX_FRAME (R)
 1:00405h 1 RX_LOOKAHEAD_VALID (R)
 1:00406h 1 HOST_INT_STATUS2 ;\GMBOX related, hw4/hw6 only
 1:00407h 1 GMBOX_RX_AVAIL ;/
 1:00408h 1x4 RX_LOOKAHEAD0[0..3] (R)
 1:0040Ch 1x4 RX_LOOKAHEAD1[0..3] (R)
 1:00410h 1x4 RX_LOOKAHEAD2[0..3] (R)
 1:00414h 1x4 RX_LOOKAHEAD3[0..3] (R)
 1:00418h 1 (HOST_)INT_STATUS_ENABLE (R/W)
 1:00419h 1 CPU_INT_STATUS_ENABLE (R/W)
 1:0041Ah 1 ERROR_(INT_)STATUS_ENABLE (R/W)
 1:0041Bh 1 COUNTER_INT_STATUS_ENABLE (R/W)
 1:0041Ch 1x4 PAD1 (FFh,6Eh,D7h,BFh - maybe some mirror?)
 1:00420h 1x8 COUNT[0..7] (R/W)
 1:00428h 1x24 PAD2
 00428h 4 - (mirror of 1:00468h?)
 0042Ch 4 - (mirror of 1:0041Ch?)
 00430h 4 - (mirror of 1:00410h?)
 00434h 4 - (mirror of 1:00...h?)
 00438h 4 - (mirror of 1:00468h?)
 0043Ch 4 - (mirror of 1:0041Ch?)
 1:00440h 4x8 COUNT_DEC[0..7] (R, or Write=any)
 1:00460h 1x8 SCRATCH[0..7] (R/W)
 1:00468h 1 FIFO_TIMEOUT (R/W)
 1:00469h 1 FIFO_TIMEOUT_ENABLE (R/W)
 1:0046Ah 1 DISABLE_SLEEP (R/W)
 1:0046Bh 1x3 -
 1:0046Eh 1 LOCAL_BUS_ENDIAN (R/W) (AR6001 only, not hw2/hw4/hw6)
 1:0046Fh 1 -
 1:00470h 1 LOCAL_BUS (R and R/W)
 1:00471h 1x1 PAD4
 1:00472h 1 INT_WLAN (R/W)
 1:00473h 1x1 PAD5
 1:00474h 4 WINDOW_DATA (R/W) ;\
 1:00478h 4 WINDOW_WRITE_ADDR (W) ;
 1:0047Ch 4 WINDOW_READ_ADDR (W) ;/
 1:00480h 1 HOST_CTRL_SPI_CONFIG (R/W)
 1:00481h 1 HOST_CTRL_SPI_STATUS (R/W)
 1:00482h 1 NON_ASSOC_SLEEP_EN ;hw2/hw4/hw6 (but didn't exist on AR6001)
 1:00483h 1 CPU_DBG_SEL ;\DBG, hw4/hw6 only
 1:00484h 1x4 CPU_DBG[0..3] ;/
 1:00488h 1 (HOST_)INT_STATUS2_ENABLE (R/W);\
 1:00489h 1x7 PAD6 ; GMBOX related, hw4/hw6 only
 1:00490h 1x8 GMBOX_RX_LOOKAHEAD[0..7] ;
 1:00498h 1 GMBOX_RX_LOOKAHEAD_MUX ;/
 1:00499h 1x359 PAD7
 1:00600h 1x512 CIS_WINDOW[0..511] (R/W?!) ;SDIO 0:01000h..0:011FFh
 1:00800h 800h Mbox0 Alias (bigger 800h bytes alias)
 1:01000h 800h Mbox1 Alias (bigger 800h bytes alias)
 1:01800h 800h Mbox2 Alias (bigger 800h bytes alias)
 1:02000h 800h Mbox3 Alias (bigger 800h bytes alias)
 1:02800h 1800h Extra Mbox0 Alias "for future usage" (1800h bytes)
 1:04000h 1C000h Unspecified |
| --- |

| DSi Atheros Wifi - SDIO Function 1 I/O - mbox_wlan_host_reg |
| --- |

Differences between hw2 versus hw4/hw6 (hw4 and hw6 are exactly same):

```

| added several new "GMBOX" registers (hw4/hw6)
 added new CPU_DBG registers (hw4/hw6)
 added three new "UART_HCI_FRAMER_xxx" error bits (hw4/hw6)
 renamed "DRAGON_INT" (hw2) to "INT" (hw4/hw6)
 renamed "SPI_xxx" (hw2) to "HOST_CTRL_SPI_xxx" (hw4/hw6) |
| --- |

**1:00000h..000FFh - Mbox0 (100h bytes)**

**1:00100h..001FFh - Mbox1 (100h bytes)**

**1:00200h..002FFh - Mbox2 (100h bytes)**

**1:00300h..003FFh - Mbox3 (100h bytes)**

**1:00800h..00FFFh - Mbox0 Alias (bigger 800h bytes alias)**

**1:01000h..017FFh - Mbox1 Alias (bigger 800h bytes alias)**

**1:01800h..01FFFh - Mbox2 Alias (bigger 800h bytes alias)**

**1:02000h..027FFh - Mbox3 Alias (bigger 800h bytes alias)**

**1:02800h..03FFFh - Extra Mbox0 Alias "for future usage" (1800h bytes)**

The MBOXes are some sort of FIFOs for transferring data blocks to/from wifi
controller. Transfer end seems to be indicated by reading/writing the LAST byte
(eg. when sending 4 bytes via Mbox0, data would be usually written to
increasing addresses at 1:000FCh..000FFh, or 1:00FFCh..00FFFh when using the
bigger Mbox alias) (technically, those addresses are just mirrors of each
other, so one could as well write all bytes to the same address, or to random
addresses at 1:00000h..000FFh and/or 1:00800h..00FFFh in no specific order; the
only special case is that the last byte at 1:000FFh/1:00FFFh seems to be
triggering something... like maybe throwing an IRQ at remote side or so).

In total, there appear to be eight MBOXes for SDIO: Four TXFIFOs (from SDIO to
Xtensa side), plus four RXFIFOs (from Xtensa to SDIO side). The capacity of the
FIFOs is unknown; the 800h-byte spaces would suggest 800h bytes, but the
firmware uploader seems to use only max 200h bytes for whatever reason.
Hardware tests suggest only 80h bytes MBOX1-3 TXFIFOs, and 8Ah or 3CD0h bytes
for MBOX0 TXFIFO (8Ah when writing LAST bytes, 3CD0h otherwise); that results
may be disturbed by the firmware trying to process incoming data.

Reading from EMPTY FIFO keeps returning the most recently read value (the last
byte before the FIFO got empty).

Writes to FULL FIFO... results are unknown... maybe ignored, and/or producing
TIMEOUTs?

**1:00400h - HOST_INT_STATUS (R)**

**1:00418h - (HOST_)INT_STATUS_ENABLE (R/W)**

Status.read: 0=No IRQ, 1=IRQ

Enable.read/write: 0=Disable, 1=Enable

```

| 0-3 MBOX_DATA MBOX0..3 Data pending (RX FIFO not empty)
 4 COUNTER Secondary IRQ from COUNTER_INT_STATUS
 5 INT Copy of internal CPU's interrupt line (aka DRAGON_INT)
 6 CPU Secondary IRQ from CPU_INT_STATUS
 7 ERROR Secondary IRQ from ERROR_INT_STATUS |
| --- |

The status register is read-only (to reset the status bits: read/reset the
corresponding MBOXes, or acknowledge the corresponding Secondary IRQ sources).

**1:00401h - CPU_INT_STATUS (R/W)**

**1:00419h - CPU_INT_STATUS_ENABLE (R/W)**

Status.read: 0=No IRQ, 1=IRQ

Status.write: 0=No change, 1=Acknowledge

Enable.read/write: 0=Disable, 1=Enable

```

| 0-7 BIT Interrupt 0..7 from internal CPU |
| --- |

These bits are eight general purpose IRQ signals from the internal CPU (the
meaning of the bits depends on software/firmware) (see WLAN_INT_HOST).

**1:00402h - ERROR_INT_STATUS (R/W)**

**1:0041Ah - ERROR_(INT_)STATUS_ENABLE (R/W)**

Status.read: 0=No IRQ, 1=IRQ

Status.write: 0=No change, 1=Acknowledge (except bit3)

Enable.read/write: 0=Disable, 1=Enable (except bit3)

```

| 0 TX_OVERFLOW (host tried to write to a full MBOX)
 1 RX_UNDERFLOW (host tried to read from an empty MBOX)
 2 WAKEUP (client has entered ON-state)
 3 SPI Error Interrupt ;STATUS only (not STATUS_ENABLE) (R)
 4 hw4/hw6: UART_HCI_FRAMER_UNDERFLOW ;\
 5 hw4/hw6: UART_HCI_FRAMER_OVERFLOW ; hw4/hw6 only
 6 hw4/hw6: UART_HCI_FRAMER_SYNC_ERROR ;/
 7 - |
| --- |

Bit3 can be acknowledged or disabled only via SPI-specific registers from SPI
host (that is, probably referring to the HOST_CTRL_SPI_CONFIG and
HOST_CTRL_SPI_STATUS registers).

**1:00403h - COUNTER_INT_STATUS (R)**

**1:0041Bh - COUNTER_INT_STATUS_ENABLE (R/W)**

Status.read: 0=No IRQ, 1=IRQ

Enable.read/write: 0=Disable, 1=Enable

```

| 0-7 COUNT[0..7] is nonzero |
| --- |

The status register is read-only (to reset the status bits: decrease the COUNT
values via COUNT_DEC[0..7]) (unknown if directly writing to COUNT[0..7] does
also affect the interrupt bits; probably it does, although official specs
suggests that only 00h <--> 01h transitions affect IRQ bits).

**1:00406h - HOST_INT_STATUS2 - hw4/hw6 only**

**1:00488h - (HOST_)INT_STATUS2_ENABLE (R/W) - hw4/hw6 only**

```

| 0 hw4/hw6: GMBOX_DATA ;\
 1 hw4/hw6: GMBOX_TX_OVERFLOW ; hw4/hw6 only
 2 hw4/hw6: GMBOX_RX_UNDERFLOW ;/
 3-7 - |
| --- |

**1:00404h - MBOX_FRAME (R)**

```

| 0-3 MBOX0..3 contains a SOM (start of message) byte in RX FIFO (1=Yes)
 4-7 MBOX0..3 contains a EOM (end of message) byte in RX FIFO (1=Yes) |
| --- |

Note: A SOM byte always follows an EOM byte from the previous message.

**1:00405h - RX_LOOKAHEAD_VALID (R)**

```

| 0-3 MBOX0..3 contains at least 4 bytes in RX FIFO (1=Yes)
 4-7 - |
| --- |

**1:00407h - GMBOX_RX_AVAIL - hw4/hw6 only**

```

| 0-6 hw4/hw6: BYTE ;-hw4/hw6 only
 7 - |
| --- |

uh, a "7bit-byte"? ... or maybe "number of bytes"?

**1:00408h..0040Bh - RX_LOOKAHEAD0[0..3] (R)**

**1:0040Ch..0040Fh - RX_LOOKAHEAD1[0..3] (R)**

**1:00410h..00413h - RX_LOOKAHEAD2[0..3] (R)**

**1:00414h..00417h - RX_LOOKAHEAD3[0..3] (R)**

```

| 0-7 MBOX RX FIFO Head-3 byte ;\what is that?
 8-15 MBOX RX FIFO Head-2 byte ; head "minus" N, or maybe
 16-23 MBOX RX FIFO Head-1 byte ;/head "plus index" N?
 24-31 MBOX RX FIFO Head byte |
| --- |

Allows to preview the first 1..4 byte(s) from MBOX0..3, without removing the
data from the FIFO. The first byte is valid when HOST_INT_STATUS indicates FIFO
not empty. The first four bytes are valid when RX_LOOKAHEAD_VALID indicates at
least 4 bytes in FIFO.

**1:00420h..00427h - COUNT[0..7] (R/W)**

This are eight 8bit counter registers for communicating with internal CPU (see
LOCAL_COUNT and COUNT_INC in internal I/O map).

```

| 0-7 Credit Counter Value |
| --- |

**1:00440h..0045Fh - COUNT_DEC[0..7] (R, or Write=any)**

This are eight 32bit registers. Reading or writing the LSB of the 32bit values
does decrement the correspondig COUNT register by one. The written value is
ignored, reading returns the old COUNT value (before decrement). The decrement
doesn't occur if the COUNT is already zero. The corresponding IRQ bit is
cleared when COUNT becomes zero.

```

| 0-7 Credit Counter Value
 8-31 Zero? (dummy padding for 32bit access) |
| --- |

**1:00460h..00467h - SCRATCH[0..7] (R/W)**

This are eight 8bit general-purpose registers for communicating with internal
CPU (see LOCAL_SCRATCH in internal I/O map).

```

| 0-7 General Purpose Value |
| --- |

**1:00468h - FIFO_TIMEOUT (R/W)**

Timeout (SDIO Wait duration) for cases when reading from empty MBOXes or
writing to full MBOXes. On AR6001, timeout is counted in 1ms units (when
CORE_CLK=40MHz) or 0.5ms units (when CORE_CLK=80MHz). Timings for AR6002 are
probably same/similar?

```

| 0-7 Timeout (01h..FFh) (00h=Reserved/don't use) |
| --- |

**1:00469h - FIFO_TIMEOUT_ENABLE (R/W)**

```

| 0 Enable FIFO Timeouts (0=Disable, 1=Enable)
 1-7 - |
| --- |

**1:0046Ah - DISABLE_SLEEP (R/W)**

```

| 0 Prevent Sleep (0=Allow Sleep, 1=Prevent Sleep)
 1 Prevent Sleep when Host IRQ pending (0=Allow Sleep, 1=Prevent Sleep)
 2-7 - |
| --- |

**1:0046Eh - LOCAL_BUS_ENDIAN (R/W) (AR6001 only, not hw2/hw4/hw6)**

```

| 0 AR6001 only: (0=Little Endian, 1=Big Endian) ;-not hw2/hw4/hw6
 1-7 - |
| --- |

**1:00470h - LOCAL_BUS (R and R/W)**

```

| 0-1 Current Chip State (0=Shutdown, 1=On, 2=Sleep, 3=Wakeup) (R)
 2 AR6001 only: KEEP_AWAKE (R/W) ;\
 3 AR6001 only: IO_ENABLE (R/W) ; not hw2/hw4/hw6
 4 AR6001 only: SOFT_RESET (R/W) ;/
 5-7 - |
| --- |

**1:00472h - INT_WLAN (R/W)**

```

| 0-7 "VECTOR" or interrupt 0..7 ? (0=No change, 1=Set) |
| --- |

Sends IRQs to internal CPU (see WLAN_MBOX_INT_STATUS bit0-7).

**1:00474h..00477h - WINDOW_DATA**

```

| 0-31 DATA |
| --- |

Used to access the internal Xtensa memory space. The actual memory access
occurs when writing the WINDOW_xxx_ADDR registers (see below).

**1:00478h..0047Bh - WINDOW_WRITE_ADDR (W) (read: crashes hardware?)**

**1:0047Ch..0047Fh - WINDOW_READ_ADDR (W) (read: crashes hardware?)**

```

| 0-1 Ignored
 2-27 ADDR (in 4-byte steps)
 28-31 ? |
| --- |

Writing an address to these registers causes 32bit data to be transferred
to/from WINDOW_DATA register.

```

| For a memory write: First write WINDOW_DATA, then write WINDOW_WRITE_ADDR
 For a memory read: First write WINDOW_READ_ADDR, then read WINDOW_DATA |
| --- |

The memory transfers seem to occur on writing address LSB (ie. one must first
write the upper three ADDR bytes, then write the lower ADDR byte; that requires
sending two separate SDIO commands, except, for accesses within the same
100h-byte block, one may get away with a single SDIO command for changing the
LSB only, and leaving the MSBs unchanged).

Caution: Trying to read the ADDR registers seems to be somehow crashing the
SDIO hardware (causing errors when trying to send any further SDIO commands).

**1:00480h - HOST_CTRL_SPI_CONFIG (R/W)**

This register can be accessed only via SPI interface.

```

| 0-1 DATA_SIZE (0=8bit, 1=16bit, 2=32bit, 3=Reserved) (addr = always 16bit)
 2 TEST_MODE (0=Normal, 1=Loopback/Echo)
 3 INTERRUPT_ENABLE (0=Disable, 1=Enable)
 4 SPI_RESET (0=Normal Operation, 1=Reset SPI core)
 5 AR6001 only, not AR6002? - SPI_CLK_OFFSET (R)
 6 -
 7 AR6001 only, not AR6002? - ENDIAN (R/W) |
| --- |

**1:00481h - HOST_CTRL_SPI_STATUS (R/W)**

This register is automatically output on SPI bus after completion of SPI data
transfers.

```

| 0 READY (0=Command Pending, 1=Completed/Ready) (R)
 1 WR_ERR (0=Okay, 1=Write-Error) (write: 0=No change, 1=Ack) (R/ack)
 2 RD_ERR (0=Okay, 1=Read-Error) (write: 0=No change, 1=Ack) (R/ack)
 3 ADDR_ERR (0=Okay, 1=Addr-Error) (write: 0=No change?, 1=Ack?)
 4 AR6001 only, not AR6002? - IFF_ERR
 5 AR6001 only, not AR6002? - DMA_OVER
 6-7 - |
| --- |

**1:00482h - NON_ASSOC_SLEEP_EN ;hw2/hw4/hw6 (but didn't exist on AR6001)**

```

| 0 BIT
 1-7 - |
| --- |

**1:00483h - CPU_DBG_SEL - hw4/hw6 only**

```

| 0-5 BIT ;-hw4/hw6 only
 6-7 - |
| --- |

**1:00484h..00487h - CPU_DBG[0..3] - hw4/hw6 only**

```

| 0-7 DATA ;-hw4/hw6 only |
| --- |

**1:00490h..00497h - GMBOX_RX_LOOKAHEAD[0..7] - hw4/hw6 only**

```

| 0-7 DATA ;-hw4/hw6 only |
| --- |

**1:00498h - GMBOX_RX_LOOKAHEAD_MUX - hw4/hw6 only**

```

| 0 SEL ;-hw4/hw6 only
 1-7 - |
| --- |

**1:00600h..007FFh - CIS_WINDOW[0..511] (R/W?!)**

```

| 0-7 DATA |
| --- |

| DSi Atheros Wifi Misc |
| --- |

**Atheros chip References**

Related Atheros chips:

```

| AR6001 with MIPS CPU, 18x18 pin BGA package ;not used in DSi/3DS
 AR6002 with Xtensa CPU, 13x13 pin BGA package ;used in early DSi
 AR6013 as AR6002, with built-in NDS-mode MM3218 ;used in later DSi
 AR6014 as AR6013, with new 3DS-multiplay mode ;used in 3DS/New3DS |
| --- |

There are some datasheets & source code:

```
[http://www.datasheetspdf.com/PDF/AR6002/705769/6](http://www.datasheetspdf.com/PDF/AR6002/705769/6)[http://www.datasheetspdf.com/PDF/AR6001X/900300/1](http://www.datasheetspdf.com/PDF/AR6001X/900300/1)[http://svn.openmoko.org/developers/nbd/ar6k/](http://svn.openmoko.org/developers/nbd/ar6k/)
| ;AR6002 datasheet 56 pages
 ;AR6001 datasheet 148 pages
 ;AR6K source code |
| --- |

The overall hardware registers appear to be same for all AR60xx chips (no
matter if they contain MIPS or Xtensa CPUs). The AR6002 datasheet doesn't
contain ANY details about hardware registers. The AR6001 datasheet describes
SOME hardware registers. And, the AR6K source code contains details about MORE
undocumented hardware registers (in some cases listing DIFFERENT addresses as
in the datasheet).

The AR6013/AR6014 chips are probably custom designs with some extra MM3218
emulation for NDS games (and possible with some unknown extra hardware
features).

**BPTWL**

Apart from SDIO bus, some wifi functions are also controlled by the BPTWL chip
(accessed via I2C bus). The wifi related BPTWL signals are:

```

| ATH_TX_H ;\maybe some/all of these do just indicate traffic
 WL_RXPE ; (for blinking the wifi LED, if it is enabled)
 WL_TXPE ;/
 /WIFI_RST ;-Reset or so |
| --- |

That signals seem to be used with BPTWL register 30h, which is allowing to
disable the Wifi LED, and might also allow to do things like restting the Wifi
hardware, and/or switching between NDS and DSi wifi-modes.

| DSi Atheros Wifi - Command Summary |
| --- |

**BMI Command Summary (Bootloader Messaging Interface)**

```

| 00h BMI_NO_COMMAND Invalid (ignored)
 01h BMI_DONE Launch Firmware
 02h BMI_READ_MEMORY Read Memory
 03h BMI_WRITE_MEMORY (normal) Write Memory
 03h BMI_WRITE_MEMORY (with dest=00001234h) Segmented Write (not in DSi)
 04h BMI_EXECUTE Execute
 05h BMI_SET_APP_START Set App Start
 06h BMI_READ_SOC_REGISTER Read Register
 07h BMI_WRITE_SOC_REGISTER Write Register
 08h BMI_GET_TARGET_ID aka BMI_GET_TARGET_INFO Get Version
 09h BMI_ROMPATCH_INSTALL TCAM/BCAM_xxxxx
 0Ah BMI_ROMPATCH_UNINSTALL TCAM/BCAM_Clr_index_and_xxx
 0Bh BMI_ROMPATCH_ACTIVATE TCAM/BCAM_Set_indices ;\
 0Ch BMI_ROMPATCH_DEACTIVATE TCAM/BCAM_Clr_indices ;/
 0Dh BMI_LZ_STREAM_START LZ Uncompress Stream Start
 0Eh BMI_LZ_DATA LZ Data Input
 0Fh BMI_NVRAM_PROCESS ;not implemented in DSi Invalid (ignored)
 10h..FFFFFFFFh Unused Invalid (ignored) |
| --- |

**HTC Services/Handshake**

After BMI, and before WMI, there is a "HTC_SERVICES" handshake with following
values:

```

| RSVD_SERVICE_GROUP = 0
 WMI_SERVICE_GROUP = 1
 HTC_TEST_GROUP = 254
 HTC_SERVICE_GROUP_LAST = 255 |
| --- |

MAKE_SERVICE_ID(group,index) = group*100h + index

NOTE: service ID of 0000h is reserved and should never be used

```

| HTC_CTRL_RSVD_SVC = MAKE_SERVICE_ID(RSVD_SERVICE_GROUP,1)
 WMI_CONTROL_SVC = MAKE_SERVICE_ID(WMI_SERVICE_GROUP,0) ;control
 WMI_DATA_BE_SVC = MAKE_SERVICE_ID(WMI_SERVICE_GROUP,1) ;best effort
 WMI_DATA_BK_SVC = MAKE_SERVICE_ID(WMI_SERVICE_GROUP,2) ;background
 WMI_DATA_VI_SVC = MAKE_SERVICE_ID(WMI_SERVICE_GROUP,3) ;video
 WMI_DATA_VO_SVC = MAKE_SERVICE_ID(WMI_SERVICE_GROUP,4) ;voice
 WMI_MAX_SERVICES = 5 |
| --- |

raw stream service (i.e. flash, tcmd, calibration apps):

```

| HTC_RAW_STREAMS_SVC = MAKE_SERVICE_ID(HTC_TEST_GROUP,0) |
| --- |

Obscure note: The above services are also known as ACs or ENDPOINTs (whatever
that means).

**WMI Command Summary (Wireless Module Interface)**

```

| 0001h WMI_CONNECT_CMD
 0002h WMI_RECONNECT_CMD
 0003h WMI_DISCONNECT_CMD
 0004h WMI_SYNCHRONIZE_CMD
 0005h WMI_CREATE_PSTREAM_CMD ;aka WMI_CRE_PRIORITY_STREAM
 0006h WMI_DELETE_PSTREAM_CMD ;aka WMI_DEL_PRIORITY_STREAM
 0007h WMI_START_SCAN_CMD
 0008h WMI_SET_SCAN_PARAMS_CMD
 0009h WMI_SET_BSS_FILTER_CMD ;aka WMI_BSS_FILTER_CMD
 000Ah WMI_SET_PROBED_SSID_CMD
 000Bh WMI_SET_LISTEN_INT_CMD
 000Ch WMI_SET_BMISS_TIME_CMD
 000Dh WMI_SET_DISC_TIMEOUT_CMD ;aka WMI_SET_DISCONNECT_TIMEOUT
 000Eh WMI_GET_CHANNEL_LIST_CMD ;reply 000Eh ;aka WMI_CHANNEL_LIST
 000Fh WMI_SET_BEACON_INT_CMD
 0010h WMI_GET_STATISTICS_CMD ;reply WMI_REPORT_STATISTICS
 0011h WMI_SET_CHANNEL_PARAMS_CMD ;aka WMI_CHANNEL_PARAMS_CMD
 0012h WMI_SET_POWER_MODE_CMD ;aka WMI_POWER_MODE_CMD
 0013h WMI_SET_IBSS_PM_CAPS_CMD ;aka WMI_IBSS_PM_CAPS_CMD
 0014h WMI_SET_POWER_PARAMS_CMD ;aka WMI_POWER_PARAMS_CMD
 0015h WMI_SET_POWERSAVE_TIMERS_POLICY_CMD ;aka WMI_POWERSAVE...
 0016h WMI_ADD_CIPHER_KEY_CMD
 0017h WMI_DELETE_CIPHER_KEY_CMD ;\
 0018h WMI_ADD_KRK_CMD ; ignored dummy commands on DSi
 0019h WMI_DELETE_KRK_CMD ;/
 001Ah WMI_SET_PMKID_CMD
 001Bh WMI_SET_TX_PWR_CMD
 001Ch WMI_GET_TX_PWR_CMD ;aka WMI_TX_PWR ;reply 001Ch
 001Dh WMI_SET_ASSOC_INFO_CMD
 001Eh WMI_ADD_BAD_AP_CMD
 001Fh WMI_DELETE_BAD_AP_CMD
 0020h WMI_SET_TKIP_COUNTERMEASURES_CMD
 0021h WMI_RSSI_THRESHOLD_PARAMS_CMD
 0022h WMI_TARGET_ERROR_REPORT_BITMASK_CMD
 0023h WMI_SET_ACCESS_PARAMS_CMD
 0024h WMI_SET_RETRY_LIMITS_CMD
 0025h WMI_SET_OPT_MODE_CMD
 0026h WMI_OPT_TX_FRAME_CMD
 0027h WMI_SET_VOICE_PKT_SIZE_CMD
 0028h WMI_SET_MAX_SP_LEN_CMD
 0029h WMI_SET_ROAM_CTRL_CMD
 002Ah WMI_GET_ROAM_TBL_CMD ;aka REPORT_ROAM_TBL,TARGET_ROAM_TBL ;reply 100Fh
 002Bh WMI_GET_ROAM_DATA_CMD ;reply 1015h ? ;\
 002Ch WMI_ENABLE_RM_CMD ; not implemented in DSi
 002Dh WMI_SET_MAX_OFFHOME_DURATION_CMD ;/
 002Eh WMI_EXTENSION_CMD ;prefix for WMIX "Non-wireless extensions"...
 002Eh:2001h WMIX_DSETOPEN_REPLY_CMD ;reply to 3001h ;\not implemented in DSi
 002Eh:2002h WMIX_DSETDATA_REPLY_CMD ;reply to 3003h ;/
 002Eh:2003h WMIX_GPIO_OUTPUT_SET_CMD ;reply=3006h ;\
 002Eh:2004h WMIX_GPIO_INPUT_GET_CMD ;reply=3005h ;
 002Eh:2005h WMIX_GPIO_REGISTER_SET_CMD ;reply=3006h, too ; GPIO
 002Eh:2006h WMIX_GPIO_REGISTER_GET_CMD ;reply=3005h, too ;
 002Eh:2007h WMIX_GPIO_INTR_ACK_CMD ;reply to 3004h ;/
 002Eh:2008h WMIX_HB_CHALLENGE_RESP_CMD ;reply=3007h ;-HB=heartbeat
 002Eh:2009h WMIX_DBGLOG_CFG_MODULE_CMD
 002Eh:200Ah WMIX_PROF_CFG_CMD ;\
 002Eh:200Bh WMIX_PROF_ADDR_SET_CMD ;
 002Eh:200Ch WMIX_PROF_START_CMD ; not implemented in DSi
 002Eh:200Dh WMIX_PROF_STOP_CMD ;
 002Eh:200Eh WMIX_PROF_COUNT_GET_CMD ;reply 3009h ;/
 002Fh WMI_SNR_THRESHOLD_PARAMS_CMD
 0030h WMI_LQ_THRESHOLD_PARAMS_CMD
 0031h WMI_SET_LPREAMBLE_CMD
 0032h WMI_SET_RTS_CMD
 0033h WMI_CLR_RSSI_SNR_CMD
 0034h WMI_SET_FIXRATES_CMD ;aka WMI_FIX_RATES_CMD
 0035h WMI_GET_FIXRATES_CMD ;reply 0035h
 0036h WMI_SET_AUTH_MODE_CMD ;aka WMI_SET_RECONNECT_AUTH_MODE_CMD |
| --- |

;below not in AR6001

```

| 0037h WMI_SET_REASSOC_MODE_CMD
 0038h WMI_SET_WMM_CMD
 0039h WMI_SET_WMM_TXOP_CMD
 ;NOT! WMI_SET_QOS_SUPP_CMD ;<-- this NOT here!
 003Ah WMI_TEST_CMD ;-not implemented in DSi
 003Bh WMI_SET_BT_STATUS_CMD ;\AR6002 Bluetooth Coexistence only?
 003Ch WMI_SET_BT_PARAMS_CMD ;/
 003Dh WMI_SET_KEEPALIVE_CMD
 003Eh WMI_GET_KEEPALIVE_CMD ;reply 003Eh
 003Fh WMI_SET_APPIE_CMD ;aka SET_APP_IE
 0040h WMI_GET_APPIE_CMD ;aka GET_APP_IE ;reply=? ;-not implemented in DSi
 0041h WMI_SET_WSC_STATUS_CMD ;aka WSC_REG
 0042h WMI_SET_HOST_SLEEP_MODE_CMD ;\
 0043h WMI_SET_WOW_MODE_CMD ;
 0044h WMI_GET_WOW_LIST_CMD ;reply=1018h ; Wake on Wireless (WOW)
 0045h WMI_ADD_WOW_PATTERN_CMD ;
 0046h WMI_DEL_WOW_PATTERN_CMD ;/
 ;below four as of "AR6kSDK.build_sw.18/include/wmi.h" (from 2006)
 ;0047h WMI_SET_MAC_ADDRESS_CMD (later moved to F003h)
 ;0048h WMI_SET_AKMP_PARAMS_CMD (later moved to F004h)
 ;0049h WMI_SET_PMKID_LIST_CMD (later moved to F005h)
 ;004Ah WMI_GET_PMKID_LIST_CMD (later moved to F006h)
 ;below two are almost always supported (but DSi is somewhat different)
 ;0047h WMI_SET_FRAMERATES_CMD ;aka WMI_FRAME_RATES_CMD ;-cmd 48h on DSi!
 ;0048h WMI_SET_AP_PS_CMD ;aka WMI_AP_PS_CMD ;-not on DSi |
| --- |

Special DSi commands:

```

| 0047h WMI_SET_HB_CHALLENGE_RESP_PARAMS_CMD
 0048h WMI_SET_FRAMERATES_CMD ;aka WMI_FRAME_RATES_CMD ;-cmd 48h on DSi!
 0049h WMI_HOST_EXIT_NOTIFY_CMD ;special DSi/3DS command |
| --- |

Special 3DS commands (for Type4 firmware):

```

| 004Ah WMI_AP_HIDDEN_SSID_CMD ;01h bytes (1)
 004Bh WMI_AP_SET_NUM_STA_CMD aka WMI_AP_NUM_STA_CMD ;01h bytes (1)
 004Ch WMI_AP_ACL_POLICY_CMD ;01h bytes (1)
 004Dh WMI_AP_ACL_MAC_LIST_CMD aka WMI_AP_ACL_MAC_CMD ;09h bytes (1+1+6+1)
 004Eh WMI_AP_CONFIG_COMMIT_CMD ;34h bytes (..)
 004Fh WMI_AP_SET_MLME_CMD ;09h bytes (6+2+1)
 0050h WMI_AP_SET_PVB_CMD ;ignored on 3DS ;06h bytes (ignored)
 0051h WMI_AP_CONN_INACT_CMD ;04h bytes (4)
 0052h WMI_AP_PROT_SCAN_TIME_CMD ;08h bytes (4+4)
 0053h WMI_? ;01h bytes (1)
 0054h N/A
 0055h WMI_? ;04h bytes (2+2)
 0056h WMI_ignored_? ;01h bytes (ignored)
 0057h WMI_? ;with EVENT_1022h ;00h bytes
 0058h WMI_? <-- with optional params? ;var (2+2+1+1+N)
 0059h WMI_? ;only lower 8bit used ;04h bytes (4; only 1 used)
 005Ah WMI_? ;01h bytes (1)
 005Bh WMI_? ;04h bytes (2+1+1)
 005Ch WMI_? ;04h bytes (1+3)
 005Dh WMI_? ;0Ah bytes (1+1+2+1+1+2+2)
 005Eh WMI_? ;01h bytes (1)
 005Fh WMI_? ;04h bytes (4) |
| --- |

Special 3DS commands (for Type5 firmware):

```

| 0060h WMI_? ;3Ah bytes
 0061h WMI_? ;28h bytes
 0062h WMI_? ;12h bytes
 0063h WMI_? ;02h bytes
 0064h WMI_? ;03h bytes
 0065h WMI_? ;07h bytes
 0066h WMI_? ;07h bytes
 0067h WMI_? ;03h bytes
 0068h WMI_? ;can be with EVENT_1023h ;01h bytes
 0069h WMI_? ;00h bytes
 006Ah WMI_? ;00h bytes
 006Bh WMI_? ;01h bytes
 006Ch WMI_? ;06h bytes
 006Dh WMI_? ;with EVENT_1026h ;00h bytes
 006Eh WMI_? ;01h bytes
 006Fh WMI_? ;06h bytes
 0070h WMI_? ;01h bytes
 0071h WMI_? ;08h bytes
 0072h WMI_? ;02h bytes
 0073h WMI_? ;with EVENT_1027h ;01h bytes |
| --- |

Newer commands

```

| 0049h WMI_SET_QOS_SUPP_CMD ;<-- this shall be HERE ;\
 004Ah WMI_SET_IE_CMD ;new cmd from 2012 ; not implemented in DSi
 08xxh WILOCITY types ;\wil6210 stuff ;
 09xxh Performance monitoring ;/ ;/
 8000h WMI_THIN_RESERVED_START ;\
 8000h WMI_THIN_CONFIG_CMD ;
 8001h WMI_THIN_SET_MIB_CMD ; not implemented in DSi
 8002h WMI_THIN_GET_MIB_CMD ;reply=8001h ; (thin commands
 8003h WMI_THIN_JOIN_CMD ;\newer ; from wmi_thin.h)
 8004h WMI_THIN_CONNECT_CMD ; versions ;
 8005h WMI_THIN_RESET_CMD ;/only ;
 8FFFh WMI_THIN_RESERVED_END ;/ |
| --- |

Developer commands

```

| F000h WMI_SET_BITRATE_CMD ;aka WMI_BIT_RATE_CMD
 F001h WMI_GET_BITRATE_CMD ;reply=F001h
 F002h WMI_SET_WHALPARAM_CMD ;aka WHAL_PARAMCMD
 F003h WMI_SET_MAC_ADDRESS_CMD ;formerly 0047h ;-not implemented in DSi
 F004h WMI_SET_AKMP_PARAMS_CMD ;formerly 0048h
 F005h WMI_SET_PMKID_LIST_CMD ;formerly 0049h
 F006h WMI_GET_PMKID_LIST_CMD ;formerly 004Ah ;reply 1019h |
| --- |

Below stuff (F007h..F05Eh) is not implemented in DSi...

```

| F007h WMI_ABORT_SCAN_CMD
 F008h WMI_SET_TARGET_EVENT_REPORT_CMD
 F009h WMI_UNUSED1 or WMI_PYXIS_CONFIG_CMD ;\Unused (or Pyxis specific
 F00Ah WMI_UNUSED2 or WMI_PYXIS_OPERATION_CMD ;/commands)
 F00Bh WMI_AP_HIDDEN_SSID_CMD ;\
 F00Ch WMI_AP_SET_NUM_STA_CMD aka WMI_AP_NUM_STA_CMD ; AP mode commands
 F00Dh WMI_AP_ACL_POLICY_CMD ;
 F00Eh WMI_AP_ACL_MAC_LIST_CMD aka WMI_AP_ACL_MAC_CMD ; (not on DSi, but
 F00Fh WMI_AP_CONFIG_COMMIT_CMD ; 3DS type4 supports
 F010h WMI_AP_SET_MLME_CMD ; F00Ah..F013h,
 F011h WMI_AP_SET_PVB_CMD ; renumbered to
 F012h WMI_AP_CONN_INACT_CMD ; 004Ah..0052h)
 F013h WMI_AP_PROT_SCAN_TIME_CMD ;
 F014h WMI_AP_SET_COUNTRY_CMD ;aka WMI_SET_COUNTRY_CMD ;
 F015h WMI_AP_SET_DTIM_CMD ;
 F016h WMI_AP_MODE_STAT_CMD ;formerly N/A ;/
 F017h WMI_SET_IP_CMD ;formerly F016h ;\
 F018h WMI_SET_PARAMS_CMD ;formerly F017h ;reply=101Fh ;
 F019h WMI_SET_MCAST_FILTER_CMD ;formerly F018h ;
 F01Ah WMI_DEL_MCAST_FILTER_CMD ;formerly F019h ;/
 F01Bh WMI_ALLOW_AGGR_CMD ;\
 F01Ch WMI_ADDBA_REQ_CMD ;
 F01Dh WMI_DELBA_REQ_CMD ;
 F01Eh WMI_SET_HT_CAP_CMD ;
 F01Fh WMI_SET_HT_OP_CMD ;
 F020h WMI_SET_TX_SELECT_RATES_CMD ;
 F021h WMI_SET_TX_SGI_PARAM_CMD ;
 F022h WMI_SET_RATE_POLICY_CMD ;/
 F023h WMI_HCI_CMD_CMD aka WMI_HCI_CMD ;\
 F024h WMI_RX_FRAME_FORMAT_CMD ;
 F025h WMI_SET_THIN_MODE_CMD ;
 F026h WMI_SET_BT_WLAN_CONN_PRECEDENCE_CMD ;/
 F027h WMI_AP_SET_11BG_RATESET_CMD ;\
 F028h WMI_SET_PMK_CMD ;
 F029h WMI_MCAST_FILTER_CMD ;/
 F02Ah WMI_SET_BTCOEX_FE_ANT_CMD ;\
 F02Bh WMI_SET_BTCOEX_COLOCATED_BT_DEV_CMD ;
 F02Ch WMI_SET_BTCOEX_SCO_CONFIG_CMD ; AR6003
 F02Dh WMI_SET_BTCOEX_A2DP_CONFIG_CMD ; Bluetooth Coexistence
 F02Eh WMI_SET_BTCOEX_ACLCOEX_CONFIG_CMD ;
 F02Fh WMI_SET_BTCOEX_BTINQUIRY_PAGE_CONFIG_CMD ;
 F030h WMI_SET_BTCOEX_DEBUG_CMD ;
 F031h WMI_SET_BTCOEX_BT_OPERATING_STATUS_CMD ;
 F032h WMI_GET_BTCOEX_STATS_CMD ;reply=1026h..1028h ;
 F033h WMI_GET_BTCOEX_CONFIG_CMD ;reply=1027h..1029h ;/
 F034h WMI_SET_DFS_ENABLE_CMD ;aka WMI_SET_DFS_CMD maybe ? ;\
 F035h WMI_SET_DFS_MINRSSITHRESH_CMD ;aka WMI_SET_DFS_CMD too ?? ; DFS
 F036h WMI_SET_DFS_MAXPULSEDUR_CMD ;aka WMI_SET_DFS_CMD too ?? ;
 F037h WMI_DFS_RADAR_DETECTED_CMD ;aka WMI_RADAR_DETECTED_CMD ;/
 F038h WMI_P2P_SET_CONFIG_CMD ;\ ;<-- confirmed to be F038h
 F039h WMI_WPS_SET_CONFIG_CMD ;P2P related ;
 F03Ah WMI_SET_REQ_DEV_ATTR_CMD ;P2P related ; P2P CMDS
 F03Bh WMI_P2P_FIND_CMD ;
 F03Ch WMI_P2P_STOP_FIND_CMD ;
 F03Dh WMI_P2P_GO_NEG_START_CMD ;
 F03Eh WMI_P2P_LISTEN_CMD ;/
 F03Fh WMI_CONFIG_TX_MAC_RULES_CMD ;\ ;<-- claimed to be F040h ?
 F040h WMI_SET_PROMISCUOUS_MODE_CMD ;
 F041h WMI_RX_FRAME_FILTER_CMD ;
 F042h WMI_SET_CHANNEL_CMD ;/
 F043h WMI_WAC_ENABLE_CMD aka WMI_ENABLE_WAC_CMD ;\
 F044h WMI_WAC_SCAN_REPLY_CMD ; WAC commands
 F045h WMI_WAC_CTRL_REQ_CMD ;/
 F046h WMI_SET_DIV_PARAMS_CMD aka WMI_DIV_PARAMS_CMD
 F047h WMI_GET_PMK_CMD ;reply? ;\
 F048h WMI_SET_PASSPHRASE_CMD ;/
 F049h WMI_SEND_ASSOC_RES_CMD ;aka WMI_SEND_ASSOCRES_CMD ;\ASSOC
 F04Ah WMI_SET_ASSOC_REQ_RELAY_CMD ;aka WMI_SET_ASSOCREQ_RELAY ;/ |
| --- |

Below uses entirely different numbering in code from 2010 vs 2012...

```

| F04Bh or F04Dh WMI_ACS_CTRL_CMD ;aka WMI_ACS_CTRL_MSG ;-ACS sub-commands
 F04Ch or F052h WMI_SET_EXCESS_TX_RETRY_THRES_CMD
 F04Dh or N/A WMI_SET_TBD_TIME_CMD ;-added for wmiconfig command for TBD
 F04Eh or N/A WMI_PKTLOG_ENABLE_CMD ;\Pktlog cmds
 F04Fh or N/A WMI_PKTLOG_DISABLE_CMD ;/(code from 2012 only)
 F050h or F053h WMI_P2P_GO_NEG_REQ_RSP_CMD ;\
 F051h or F054h WMI_P2P_GRP_INIT_CMD ;
 F052h or F055h WMI_P2P_GRP_FORMATION_DONE_CMD ;
 F053h or F056h WMI_P2P_INVITE_CMD ; More P2P commands
 F054h or F057h WMI_P2P_INVITE_REQ_RSP_CMD ;
 F055h or F058h WMI_P2P_PROV_DISC_REQ_CMD ;
 F056h or F059h WMI_P2P_SET_CMD ;/
 F057h or F04Bh WMI_GET_RFKILL_MODE_CMD ;\RFKILL
 F058h or F04Ch WMI_SET_RFKILL_MODE_CMD ;aka WMI_RFKILL_MODE_CMD ;/
 F059h or F05Ah WMI_AP_SET_APSD_CMD ;\More AP commands
 F05Ah or F05Bh WMI_AP_APSD_BUFFERED_TRAFFIC_CMD ;/
 F05Bh or F05Ch WMI_P2P_SDPD_TX_CMD ;\
 F05Ch or F05Dh WMI_P2P_STOP_SDPD_CMD ; More P2P commands
 F05Dh or F05Eh WMI_P2P_CANCEL_CMD ;/
 F05Eh or F04Eh WMI_STORERECALL_CONFIGURE_CMD ;\Ultra low power
 F05Fh or F04Fh WMI_STORERECALL_RECALL_CMD ; store/recall commands
 F060h or F050h WMI_STORERECALL_HOST_READY_CMD ;/
 F061h or F051h WMI_FORCE_TARGET_ASSERT_CMD ;-
 F062h or N/A WMI_SET_PROBED_SSID_EX_CMD ;\
 F063h or N/A WMI_SET_NETWORK_LIST_OFFLOAD_CMD ;
 F064h or N/A WMI_SET_ARP_NS_OFFLOAD_CMD ;
 F065h or N/A WMI_ADD_WOW_EXT_PATTERN_CMD ; NEW stuff
 F066h or N/A WMI_GTK_OFFLOAD_OP_CMD ; (code from 2012 only)
 F067h or N/A WMI_REMAIN_ON_CHNL_CMD ;
 F068h or N/A WMI_CANCEL_REMAIN_ON_CHNL_CMD ;
 F069h or N/A WMI_SEND_ACTION_CMD ;
 F06Ah or N/A WMI_PROBE_REQ_REPORT_CMD ;
 F06Bh or N/A WMI_DISABLE_11B_RATES_CMD ;
 F06Ch or N/A WMI_SEND_PROBE_RESPONSE_CMD ;
 F06Dh or N/A WMI_GET_P2P_INFO_CMD ;
 F06Eh or N/A WMI_AP_JOIN_BSS_CMD ;/
 ? WMI_SET_ADHOC_BSSID_CMD ;-old, not implemented?
 F0AFh WMI_AP_PSBUFF_OFFLOAD ;-QCA4002 chipset on GT202 WiFi module |
| --- |

| DSi Atheros Wifi - Response Summary |
| --- |

Events can be actual hardware events, or responses to certain commands. See
MBOX Transfer Headers chapter for details on the transfer format.

**WMI Events/Responses**

Events/Responses with same ID as corresponding command

```

| 000Eh WMI_GET_CHANNEL_LIST_CMD
 001Ch WMI_GET_TX_PWR_CMD
 0035h WMI_GET_FIXRATES_CMD
 003Eh WMI_GET_KEEPALIVE_CMD
 F001h WMI_GET_BITRATE_CMD |
| --- |

Events/Responses that could/should exist (but aren't documented)

```

| ? WMI_GET_APPIE_CMD ;aka GET_APP_IE ;\not implemented in DSi
 ? WMI_AP_MODE_STAT_CMD ;has reply? ;/ |
| --- |

Events/Responses that have REPLY structs defined (but CMDs don't send reply?)

```

| ? WMI_CRE_PRIORITY_STREAM_REPLY ;\
 ? WMI_DEL_PRIORITY_STREAM_REPLY ; not implemented in DSi
 ? WMI_FRAME_RATES_REPLY ;/ |
| --- |

Events/Responses with special IDs

```

| 1001h WMI_READY_EVENT
 1002h WMI_CONNECT_EVENT
 1003h WMI_DISCONNECT_EVENT
 1004h WMI_BSSINFO_EVENT ;aka WMI_BSS_INFO
 1005h WMI_CMDERROR_EVENT ;aka WMI_CMD_ERROR_EVENT ;for CMD 01h,11h,16h,26h
 1006h WMI_REGDOMAIN_EVENT ;aka WMI_REG_DOMAIN_EVENT
 1007h WMI_PSTREAM_TIMEOUT_EVENT
 1008h WMI_NEIGHBOR_REPORT_EVENT
 1009h WMI_TKIP_MICERR_EVENT
 100Ah WMI_SCAN_COMPLETE_EVENT
 100Bh WMI_REPORT_STATISTICS_EVENT ;related to CMD 0010h ?
 100Ch WMI_RSSI_THRESHOLD_EVENT
 100Dh WMI_ERROR_REPORT_EVENT ;aka WMI_TARGET_ERROR_REPORT_EVENT
 100Eh WMI_OPT_RX_FRAME_EVENT ;aka WMI_OPT_RX_INFO
 100Fh WMI_REPORT_ROAM_TBL_EVENT ;related to CMD 002Ah ?
 1010h WMI_EXTENSION_EVENT ;prefix for WMIX events...
 1010h:3001h WMIX_DSETOPENREQ_EVENT ;request 2001h ;\
 1010h:3002h WMIX_DSETCLOSE_EVENT ;request close ; not implemented in DSi
 1010h:3003h WMIX_DSETDATAREQ_EVENT ;request 2002h ;/
 1010h:3004h WMIX_GPIO_INTR_EVENT ;used (interrupt)
 1010h:3005h WMIX_GPIO_DATA_EVENT ;used (reply to 2004h and 2006h)
 1010h:3006h WMIX_GPIO_ACK_EVENT ;used (reply to 2003h and 2005h)
 1010h:3007h WMIX_HB_CHALLENGE_RESP_EVENT ;used (reply to 2008h)
 1010h:3008h WMIX_DBGLOG_EVENT ;used (probably related to 2009h)
 1010h:3009h WMIX_PROF_COUNT_EVENT ;-not implemented in DSi
 1011h WMI_CAC_EVENT
 1012h WMI_SNR_THRESHOLD_EVENT
 1013h WMI_LQ_THRESHOLD_EVENT
 1014h WMI_TX_RETRY_ERR_EVENT
 1015h WMI_REPORT_ROAM_DATA_EVENT ;related to 002Bh? ;\not implemented in DSi
 1016h WMI_TEST_EVENT ;/
 1017h WMI_APLIST_EVENT
 1018h WMI_GET_WOW_LIST_EVENT ;reply to CMD 0044h
 1019h WMI_GET_PMKID_LIST_EVENT ;reply to CMD F006h |
| --- |

Below not in AR6kSDK.build_sw.18, however, "101Ah" is USED on DSi...

```

| 101Ah WMI_CHANNEL_CHANGE_EVENT ;<-- used on DSi |
| --- |

Below some are spotted on 3DS (possibly inofficial stuff?)

(type4=3ds/internet?, macfilter=3ds/localmultiplay?)

```

| 101Dh type4, len=0
 101Fh type4, len=N*4
 1020h type4, len=8
 1021h macfilter, len=10h+? ;"succes"
 1022h type4, len=0 ;reply to cmd 0057h ?
 1023h macfilter, len=4+? ;reply to cmd 0068h
 1024h type4, len=0Eh
 1025h type4, len=1 ;thrown by 3DS type4 variant (works when ignored)
 1026h macfilter, len=0Ch ;reply to cmd 006Dh
 1027h macfilter, len=2+? ;reply to cmd 0073h |
| --- |

Below (101Bh..9004h) not implemented in DSi...

```

| 101Bh WMI_PEER_NODE_EVENT
 101Ch WMI_PSPOLL_EVENT ;aka WMI_PS_POLL_EVENT ;AP mode related?
 101Dh WMI_DTIMEXPIRY_EVENT
 101Eh WMI_WLAN_VERSION_EVENT
 101Fh WMI_SET_PARAMS_REPLY_EVENT ;reply to CMD F018h (reply to "SET" cmd!)
 1020h WMI_ADDBA_REQ_EVENT
 1021h WMI_ADDBA_RESP_EVENT
 1022h WMI_DELBA_REQ_EVENT aka WMI_DELBA_EVENT
 1023h WMI_TX_COMPLETE_EVENT
 1024h WMI_HCI_EVENT_EVENT aka WMI_HCI_EVENT
 1025h WMI_ACL_DATA_EVENT
 1026h WMI_REPORT_SLEEP_STATE_EVENT ;formerly N/A
 1027h WMI_WAPI_REKEY_EVENT ;formerly N/A, or 1026h if WAPI_ENABLE
 1028h WMI_REPORT_BTCOEX_STATS_EVENT ;formerly 1026h/1027h ;reply to F032h
 1029h WMI_REPORT_BTCOEX_CONFIG_EVENT ;formerly 1027h/1028h ;reply to F033h
 102Ah WMI_GET_PMK_EVENT aka WMI_GET_PMK_REPLY
 102Bh WMI_DFS_HOST_ATTACH_EVENT ;\
 102Ch WMI_DFS_HOST_INIT_EVENT ;
 102Dh WMI_DFS_RESET_DELAYLINES_EVENT ;
 102Eh WMI_DFS_RESET_RADARQ_EVENT ;
 102Fh WMI_DFS_RESET_AR_EVENT ; DFS Events
 1030h WMI_DFS_RESET_ARQ_EVENT ;
 1031h WMI_DFS_SET_DUR_MULTIPLIER_EVENT ;
 1032h WMI_DFS_SET_BANGRADAR_EVENT ;
 1033h WMI_DFS_SET_DEBUGLEVEL_EVENT ;
 1034h WMI_DFS_PHYERR_EVENT ;/
 1035h WMI_CCX_RM_STATUS_EVENT ;-CCX Evants ;uh, EvAntS?
 1036h WMI_P2P_GO_NEG_RESULT_EVENT ;-P2P Events ;uh, EventS?
 1037h WMI_WAC_SCAN_DONE_EVENT ;\
 1038h WMI_WAC_REPORT_BSS_EVENT ; WAC
 1039h WMI_WAC_START_WPS_EVENT ;
 103Ah WMI_WAC_CTRL_REQ_REPLY_EVENT ;/
 103Bh WMI_RFKILL_STATE_CHANGE_EVENT ;\RFKILL Events
 103Ch WMI_RFKILL_GET_MODE_CMD_EVENT ;/
 103Dh WMI_P2P_GO_NEG_REQ_EVENT ;\
 103Eh WMI_P2P_INVITE_REQ_EVENT ;
 103Fh WMI_P2P_INVITE_RCVD_RESULT_EVENT ;
 1040h WMI_P2P_INVITE_SENT_RESULT_EVENT ; More P2P Events
 1041h WMI_P2P_PROV_DISC_RESP_EVENT ;
 1042h WMI_P2P_PROV_DISC_REQ_EVENT ;
 1043h WMI_P2P_START_SDPD_EVENT ;
 1044h WMI_P2P_SDPD_RX_EVENT ;/
 1045h WMI_SET_HOST_SLEEP_MODE_CMD_PROCESSED_EVENT ;-avoid AR6003 crash
 8000h WMI_THIN_EVENTID_RESERVED_START ;\
 8001h WMI_THIN_GET_MIB_EVENT ; THIN events (wmi_thin.h)
 8002h WMI_THIN_JOIN_EVENT ;
 8FFFh WMI_THIN_EVENTID_RESERVED_END ;/
 9000h WMI_SET_CHANNEL_EVENT ;\
 9001h WMI_ASSOC_REQ_EVENT aka WMI_ASSOCREQ_EVENT ; More events,
 9002h WMI_ACS_EVENT ;generic ACS event ; somehow located
 9003h WMI_REPORT_WMM_PARAMS_EVENT ; after THIN area
 9004h WMI_STORERECALL_STORE_EVENT ;/ |
| --- |

code from 2012 has WMI_WAPI_REKEY_EVENT re-removed again, the two RFKILL Events
moved around (placed into the P2P event area), and WMI_REPORT_WMM_PARAMS_EVENT
moved to 10xxh, a new WMI_WAC_REJECT_WPS_EVENT at 10xxh, and
WMI_SET_HOST_SLEEP_MODE_CMD_PROCESSED_EVENT removed, and
WMI_STORERECALL_STORE_EVENT moved to 9003h, and adds more NEW stuff at
9004h-900Dh:

```

| 10xxh WMI_REPORT_WMM_PARAMS_EVENT ;-moved to 10xxh or so
 10xxh WMI_WAC_REJECT_WPS_EVENT ;-NEW
 9003h WMI_STORERECALL_STORE_EVENT ;-move to HERE
 9004h WMI_WOW_EXT_WAKE_EVENT ;\
 9005h WMI_GTK_OFFLOAD_STATUS_EVENT ;
 9006h WMI_NETWORK_LIST_OFFLOAD_EVENT ;
 9007h WMI_REMAIN_ON_CHNL_EVENT ; NEW
 9008h WMI_CANCEL_REMAIN_ON_CHNL_EVENT ;
 9009h WMI_TX_STATUS_EVENT ;
 900Ah WMI_RX_PROBE_REQ_EVENT ;
 900Bh WMI_P2P_CAPABILITIES_EVENT ;
 900Ch WMI_RX_ACTION_EVENT ;
 900Dh WMI_P2P_INFO_EVENT ;/ |
| --- |

| DSi Atheros Wifi - Host Interest Area in RAM |
| --- |

**Host Interest Area (aka Target Addresses)**

Apart from sending BMI and WMI commands via MBOX0, some communication is also
done by directly accessing the 100h-byte "Host Interest" area in RAM, this can
be done via WINDOW_DATA register (or via BMI commands).

The format of this memory area (defined in "targaddrs.h") is held to be same on
all AR60xx chips, however, its BASE ADDRESS may vary on different chips:

```

| AR6002_HOST_INTEREST_ADDRESS = 00500400h ;older DSi
 AR6013_HOST_INTEREST_ADDRESS = 00520000h ;newer DSi
 AR6014_HOST_INTEREST_ADDRESS = 00520000h ;3DS and New3DS
 AR6003_HOST_INTEREST_ADDRESS = 00540600h
 MCKINLEY_HOST_INTEREST_ADDRESS = 00400600h |
| --- |

That base address can be also found in the DSi's "Wifi Firmware" fileheader.

Aside from the 100h bytes in RAM, there's also another host interest byte in
the LOCAL_SCRATCH[0] register; the CHIP_ID register could be also considered as
interesting to the host.

**Host Interest Entries in RAM ("host_interest_s" structure, 64 words)**

```

| 00h hi_app_host_interest ;-Pointer to application-defined area, if any.
 ; (set by Target application during startup)
 04h hi_failure_state ;-Pointer to register dump area after Target crash
 08h hi_dbglog_hdr ;-Pointer to debug logging header
 0Ch hi_flash_is_present ;Indicates whether or not flash is present on Target
 ;NB: flash_is_present indicator is here not just because it might be
 ;of interest to the Host; but also because it's set early on by
 ;Target's startup asm code and we need it to have a special RAM
 ;address so that it doesn't get reinitialized with the rest of data.
 10h hi_option_flag ;-Various flags (see below)
 14h hi_serial_enable ;-Boolean whether to output (additional) TTY messages
 18h hi_dset_list_head ;-Start address of DataSet index, if any
 1Ch hi_app_start ;-Override BMI_DONE Target application start address
 20h hi_skip_clock_init ;\
 24h hi_core_clock_setting ;
 28h hi_cpu_clock_setting ; Clock and voltage tuning
 2Ch hi_system_sleep_setting ;
 30h hi_xtal_control_setting ;
 34h hi_pll_ctrl_setting_24ghz ;
 38h hi_pll_ctrl_setting_5ghz ;
 3Ch hi_ref_voltage_trim_setting ;
 40h hi_clock_info ;/
 44h hi_bank0_addr_value ;\Flash configuration overrides, used only
 48h hi_bank0_read_value ; when firmware is not executing from flash
 4Ch hi_bank0_write_value ; (when using flash, modify the global
 50h hi_bank0_config_value ;/variables with equivalent names)
 54h hi_board_data ;\Pointer to Board Data (eg. from I2C
 58h hi_board_data_initialized ;/EEPROM) and data present/init flag
 5Ch hi_dset_RAM_index_table ;-
 60h hi_desired_baud_rate ;\ ;<-- for TTY/UART (default=9600 decimal)
 64h hi_dbglog_config ;
 68h hi_end_RAM_reserve_sz ;
 6Ch hi_mbox_io_block_sz ;/
 70h hi_num_bpatch_streams ;-Unused (supposedly was used before 2010)
 74h hi_mbox_isr_yield_limit ;-
 78h hi_refclk_hz ;-OSC ;NN,000,000 decimal (26MHz/40MHz)
 Below seems to be newer stuff... not implemented in DSi... (?)
 7Ch hi_ext_clk_detected ;\
 80h hi_dbg_uart_txpin ;
 84h hi_dbg_uart_rxpin ;
 88h hi_hci_uart_baud ; <-- used in 3DS WMI_READY_EVENT ?
 8Ch hi_hci_uart_pin_assignments ;/ ;<-- byte[0]=tx, [1]=rx, [2]=rts, [3]=cts
 90h hi_hci_uart_baud_scale_val ;\
 94h hi_hci_uart_baud_step_val ;/
 98h hi_allocram_start ;\
 9Ch hi_allocram_sz ;/
 A0h hi_hci_bridge_flags ;\
 A4h hi_hci_uart_support_pins ;/
 ;NOTE: byte[0]=RESET pin (bit7 is polarity), byte[1..3]=for future use
 A8h hi_hci_uart_pwr_mgmt_params ;-
 ;Bit[1]: 0=UART FC active low, 1=UART FC active high
 ;Bit[16-31]: wakeup timeout in ms
 ACh hi_board_ext_data ;\Pointer to extended board Data, and
 B0h hi_board_ext_data_config ;/Config/flags (bit0=valid, bit16-31=size)
 B4h hi_reset_flag ;\warmboot flags, valid when [B8h]=12345678h
 B8h hi_reset_flag_valid ;/
 BCh hi_hci_uart_pwr_mgmt_params_ext ;-bit[0-31]: idle timeout in ms
 C0h hi_acs_flags ;-ACS flags
 C4h hi_console_flags ;
 C8h hi_nvram_state ;
 CCh hi_option_flag2 ;
 D0h hi_sw_version_override ;\If non-zero, override values sent to Host
 D4h hi_abi_version_override ;/in WMI_READY event
 D8h hi_test_apps_related ;-test applications flags
 DCh hi_ota_testscript ;-location of test script
 E0h hi_cal_data ;-location of CAL data
 E4h..FFh ;reserved
 FCh reserved, but 3DS sets this to 00000003h along hi_board_data_initialized |
| --- |

**LOCAL_SCRATCH[0] - AR6K option bits, to enable/disable various features**

By default, all option bits are 0.

```

| AR6K_OPTION_BMI_DISABLE = 01h ;bit0 Disable BMI comm with Host
 AR6K_OPTION_SERIAL_ENABLE = 02h ;bit1 Enable UART serial port TTY messages
 AR6K_OPTION_WDT_DISABLE = 04h ;bit2 WatchDog Timer override
 AR6K_OPTION_SLEEP_DISABLE = 08h ;bit3 Disable system sleep
 AR6K_OPTION_STOP_BOOT = 10h ;bit4 Stop boot processes (for ATE)
 AR6K_OPTION_ENABLE_NOANI = 20h ;bit5 Operate without ANI
 AR6K_OPTION_DSET_DISABLE = 40h ;bit6 Ignore DataSets
 AR6K_OPTION_IGNORE_FLASH = 80h ;bit7 Ignore flash during bootup |
| --- |

**targaddr[10h] - hi_option_flag**

```

| 0 HI_OPTION_TIMER_WAR ;Enable timer workaround
 1 HI_OPTION_BMI_CRED_LIMIT ;Limit BMI command credits
 2 HI_OPTION_RELAY_DOT11_HDR ;Relay Dot11 hdr to/from host
 3 HI_OPTION_MAC_ADDR_METHOD ;MAC addr method 0=locally administred
 ; 1=globally unique addrs
 4 HI_OPTION_ENABLE_RFKILL ;RF Kill Enable Feature
 5 HI_OPTION_ENABLE_PROFILE ;Enable CPU profiling
 6 HI_OPTION_DISABLE_DBGLOG ;Disable debug logging
 7 HI_OPTION_SKIP_ERA_TRACKING ;Skip Era Tracking
 8 HI_OPTION_PAPRD_DISABLE ;Disable PAPRD (debug)
 9-11 HI_OPTION_NUM_DEV ;num dev (3bit)
 12-27 HI_OPTION_DEV_MODE ;dev mode (16bit) (aka 4xMODE, 4xSUBMODE?)
 28 HI_OPTION_NO_LFT_STBL ;Disable LowFreq LF Timer Stabilization
 29 HI_OPTION_SKIP_REG_SCAN ;Skip regulatory scan
 30 HI_OPTION_INIT_REG_SCAN ;Do regulatory scan during init before
 ; sending WMI ready event to host
 31 HI_OPTION_FW_BRIDGE ;Firmware bridging |
| --- |

Two bits of hi_option_flag are used to represent 3 (three?) modes

```

| HI_OPTION_FW_MODE_IBSS = 00h ;IBSS Mode
 HI_OPTION_FW_MODE_BSS_STA = 01h ;STA Mode
 HI_OPTION_FW_MODE_AP = 02h ;AP Mode
 HI_OPTION_FW_MODE_BT30AMP = 03h ;BT30 AMP Mode |
| --- |

Two bits of hi_option flag are used to represent 4 submodes (aka DEV_MODE?)

```

| HI_OPTION_FW_SUBMODE_NONE = 00h ;Normal mode
 HI_OPTION_FW_SUBMODE_P2PDEV = 01h ;p2p device mode
 HI_OPTION_FW_SUBMODE_P2PCLIENT = 02h ;p2p client mode
 HI_OPTION_FW_SUBMODE_P2PGO = 03h ;p2p go mode |
| --- |

Fw Mode/SubMode Mask

```

| .--------.-------.-------.-------.-------.-------.-------.-------.
 | SUB | SUB | SUB | SUB | | | | |
 |MODE[3] |MODE[2]|MODE[1]|MODE[0]|MODE[3]|MODE[2]|MODE[1]|MODE[0]|
 | (2) | (2) | (2) | (2) | (2) | (2) | (2) | (2) |
 '--------'-------'-------'-------'-------'-------'-------'-------'
 HI_OPTION_FW_MODE_BITS 0x2 ;\
 HI_OPTION_FW_MODE_MASK 0x3 ; MODE
 HI_OPTION_FW_MODE_SHIFT 0xC ;bit12-13 (2bit) per device? ;
 HI_OPTION_ALL_FW_MODE_MASK 0xFF ;bit12-19 (8bit) per 4 devices? ;/
 HI_OPTION_FW_SUBMODE_BITS 0x2 ;\
 HI_OPTION_FW_SUBMODE_MASK 0x3 ; SUB-
 HI_OPTION_FW_SUBMODE_SHIFT 0x14 ;bit20-21 (2bit) per device? ; MODE
 HI_OPTION_ALL_FW_SUBMODE_MASK 0xFF00 ;bit20-27 (8bit) per 4 devices? ;
 HI_OPTION_ALL_FW_SUBMODE_SHIFT 0x8 ;/ |
| --- |

**targaddr[CCh] - hi_option_flag2**

```

| 0 HI_OPTION_OFFLOAD_AMSDU ;aka OFFLAOD
 1 HI_OPTION_DFS_SUPPORT ;-only in newer code from 2011 or so
 2 HI_OPTION_ENABLE_RFKILL ;\
 3 HI_OPTION_RADIO_RETENTION_DISABLE ;
 4 HI_OPTION_EARLY_CFG_DONE ; only in newer code from 2015 or so
 5 HI_OPTION_DISABLE_CDC_MAX_PERF_WAR ;
 6 HI_OPTION_USE_EXT_LDO ;
 7 HI_OPTION_DBUART_SUPPORT ;
 8 Reserved? ;
 9 HT_OPTION_GPIO_WAKEUP_SUPPORT ;HT? ;/
 10-31 Reserved |
| --- |

**targaddr[B4h] - hi_reset_flag (warmboot flags, valid when [B8h]=12345678h)**

hi_reset_flag is used to do some stuff when target reset. such as restore
app_start after warm reset or preserve host Interest area, or preserve ROM
data, literals, etc.

```

| 0 HI_RESET_FLAG_PRESERVE_APP_START ;preserve App Start address
 1 HI_RESET_FLAG_PRESERVE_HOST_INTEREST ;preserve Host Interest
 2 HI_RESET_FLAG_PRESERVE_ROMDATA ;preserve ROM data
 3 HI_RESET_FLAG_PRESERVE_NVRAM_STATE
 4 HI_RESET_FLAG_PRESERVE_BOOT_INFO ;\only in newer code from 2015 or so
 5 HI_RESET_FLAG_WARM_RESET ;/
 6-31 Reserved |
| --- |

**targaddr[C0h] - hi_acs_flags**

```

| 0 HI_ACS_FLAGS_ENABLED ;ACS is enabled
 1 HI_ACS_FLAGS_USE_WWAN ;Use physical WWAN device
 2 HI_ACS_FLAGS_TEST_VAP ;Use test VAP
 3-31 Reserved |
| --- |

**targaddr[C4h] - hi_console_flags**

```

| 0-2 HI_CONSOLE_FLAGS_UART ;UART ID (0=Default)
 3 HI_CONSOLE_FLAGS_BAUD_SELECT ;Baud Select (0=9600, 1=115200)
 4-30 Reserved
 31 HI_CONSOLE_FLAGS_ENABLE ;Enable Console |
| --- |

**targaddr[D8h] - Bitmap for hi_test_apps_related**

```

| 0 HI_TEST_APPS_TESTSCRIPT_LOADED
 1 HI_TEST_APPS_CAL_DATA_AVAIL
 2-31 Reserved |
| --- |

Convert a Target virtual address into a Target physical address

```

| AR6002_VTOP(vaddr) = ((vaddr) & 0x001fffff) ;\uh, 2Mbyte space?
 AR6003_VTOP(vaddr) = ((vaddr) & 0x001fffff) ;/(shouldn't that be 4Mbyte?)
 MCKINLEY_VTOP(vaddr) = ((vaddr)) ;whatever, maybe uses a different CPU/HW |
| --- |

Override REV2 ROM's app start address (whatever crap, doesn't apply to DSi)

```

| AR6002_REV2_APP_START_OVERRIDE 0x911A00 ;\
 AR6002_REV2_DATASET_PATCH_ADDRESS 0x52D8B0 ; AR6002
 AR6002_REV2_APP_LOAD_ADDRESS 0x502070 ;/
 AR6003_REV2_APP_START_OVERRIDE 0x944C00 ;\
 AR6003_REV2_APP_LOAD_ADDRESS 0x543180 ;
 AR6003_REV2_BOARD_EXT_DATA_ADDRESS 0x57E500 ; AR6003 REV2
 AR6003_REV2_DATASET_PATCH_ADDRESS 0x57E884 ;
 AR6003_REV2_RAM_RESERVE_SIZE 6912 ;/
 AR6003_REV3_APP_START_OVERRIDE 0x945D20 ;\
 AR6003_REV3_APP_LOAD_ADDRESS 0x545000 ;
 AR6003_REV3_BOARD_EXT_DATA_ADDRESS 0x542330 ; AR6003 REV3
 AR6003_REV3_DATASET_PATCH_ADDRESS 0x57FEC8 ;
 AR6003_REV3_RAM_RESERVE_SIZE 512 ;
 AR6003_REV3_RAM_RESERVE_SIZE_TCMD 4352 ;/ |
| --- |

| DSi Atheros Wifi - BMI Bootloader Commands |
| --- |

The BMI commands are used to upload the Wifi Firmware to RAM. Doing that is
required because the ROM functions themselves aren't fully functional, and the
chip can process only BMI commands or WMI commands (not both at once):

```

| BMI Commands --> After RESET
 WMI Commands --> After uploading and sending BMI_DONE |
| --- |

The AR6002 ROM contains about 40-50% of the program code needed to operate the
chip (and most of that ROM code is left unused, until it is getting initialized
by the firmware; so the firmware isn't just an "update", it's absolutely
required to get the chip working).

**DSi Wifi Firmware**

On the DSi, the Wifi Firmware is stored in a eMMC file, and it's automatically
uploaded by the DSi System Menu (Launcher), so DSi games should be always
started with Firmware already installed, and don't need to deal with BMI
commands.

DSi SD/MMC Firmware Wifi Firmware

If desired, one could force the chip back to BMI state by issuing a reset, eg.
via RESET_CONTROL.Bit8; there might be further ways to issue resets, like SDIO
CCCR maybe).

Note: The BMI uploader in DSi is reffering to BMI as "PRE-AUTH" phase, the
system menu will be shown even in case of BMI failure, but any such failure
will later cause a "WLFIRM 2" error (in sysmenu.log) at time when trying to
start a title from within system menu.

**BMI Transfer**

Before each BMI command, wait for LOCAL_COUNT[4] to become nonzero. Then, write
the command and parameters to MBOX0. Then read the response (if any) from MBOX0
(ideally with checking MBOX empty flag before reading response bytes; DSi code
doesn't seem to do that though - maybe the SDIO controller is automatically
waiting while MBOX empty?).

```

| ____________________________ Execute Functions ____________________________ |
| --- |

**BMI_CMD(01h) - BMI_DONE - Launch Firmware**

```

| Send 32bit Command (00000001h) |
| --- |

Starts the firmware by calling its entrypoint. The default entry is 915000h for
AR6002G (or 927000h for AR6013G and AR6014G), alternately a custom entrypoint
can be set via BMI_CMD(05h).

**BMI_CMD(04h) - BMI_EXECUTE - Execute**

```

| Send 32bit Command (00000004h)
 Send 32bit Entrypoint
 Send 32bit Argument
 Receive 32bit Return Value |
| --- |

Calls a function on Xtensa CPU. On DSi, this is used to execute the boot stub
(for reading the I2C EEPROM data). The main firmware should be started via
BMI_DONE, not via BMI_EXECUTE.

**BMI_CMD(05h) - BMI_SET_APP_START - Set App Start**

```

| Send 32bit Command (00000005h)
 Send 32bit Entrypoint |
| --- |

Changes the default entrypoint for BMI_DONE. The DSi doesn't use this feature
(and uses the default entrypoint).

```

| ___________________________ Read/Write Functions ___________________________ |
| --- |

**BMI_CMD(02h) - BMI_READ_MEMORY - Read Memory**

```

| Send 32bit Command (00000002h)
 Send 32bit Address
 Send 32bit Length (should be max 80h or 200h or so (?) due to MBOX size)
 Receive LEN bytes, read from [address and up] |
| --- |

Allows to read Xtensa RAM or ROM in byte-units (I/O ports should be read in
32bit units, via BMI_READ_SOC_REGISTER).

**BMI_CMD(03h) - BMI_WRITE_MEMORY - Write Memory**

```

| Send 32bit Command (00000003h)
 Send 32bit Address (or special value for "Segmented Write", see below)
 Send 32bit Length (should be max 1F4h due to MBOX size)
 Send LEN bytes, written to [address and up] |
| --- |

Allows to write Xtensa RAM. Used to upload the stub and data parts of the
firmware (the main part of the firmware is uploaded via BMI_LZ_xxx functions).

**BMI_CMD(06h) - BMI_READ_SOC_REGISTER - Read Register**

```

| Send 32bit Command (00000006h)
 Send 32bit Address
 Receive 32bit Word from [address] |
| --- |

Allows to read Xtensa I/O Ports (or RAM or ROM) in 32bit-units. The same effect
can be gained via WINDOW_DATA register (which is also working when the Xtensa
CPU isn't in BMI bootloader state).

**BMI_CMD(07h) - BMI_WRITE_SOC_REGISTER - Write Register**

```

| Send 32bit Command (00000007h)
 Send 32bit Address
 Send 32bit Word to [address] |
| --- |

Allows to write Xtensa I/O Ports (or RAM) in 32bit-units.

**BMI_CMD(0Dh) - BMI_LZ_STREAM_START - LZ Uncompress Stream Start**

```

| Send 32bit Command (0000000Dh)
 Send 32bit Destination Start Address for BMI_CMD(0Eh) |
| --- |

Sets the destination start address for following BMI_LZ_DATA command(s). Also
resets the decompressor to expect the "tag" value in first byte.

**BMI_CMD(0Eh) - BMI_LZ_DATA - LZ Data Input**

```

| Send 32bit Command (0000000Eh)
 Send 32bit Length (should be max 1F8h due to MBOX size)
 Send LEN compressed bytes, decompressed to incrementing destination address |
| --- |

```

| ___________________________ ROM Patch Functions ___________________________ |
| --- |

ROM Patches using the hardware's TCAM/BCAM registers (implemented in hardware,
but not actually used by DSi firmware).

**BMI_CMD(09h) - BMI_ROMPATCH_INSTALL - TCAM/BCAM_xxxxx**

```

| Send 32bit Command (00000009h)
 Send 32bit Target ROM Address
 Send 32bit Target RAM Address or Value (depending on Target Type)
 Send 32bit Size (in bytes)
 Send 32bit Activate (0=Install without activate, 1=Install and activate)
 Receive 32bit PatchID |
| --- |

The DSi/3DS do contain TCAM hardware. The TCAM hardware's eight size settings
are probably 20h,40h,80h,100h,200h,400h,800h,1000h. For the BCAM hardware, the
size must be probably always set to 04h.

**BMI_CMD(0Ah) - BMI_ROMPATCH_UNINSTALL - TCAM/BCAM_Clr_index_and_xxx**

Uninstall (and deactivate) a previously-installed ROM Patch.

```

| Send 32bit Command (0000000Ah)
 Send 32bit PatchID (to be uninstalled & deactivated) |
| --- |

**BMI_CMD(0Bh) - BMI_ROMPATCH_ACTIVATE - TCAM/BCAM_Set_indices**

**BMI_CMD(0Ch) - BMI_ROMPATCH_DEACTIVATE - TCAM/BCAM_Clr_indices**

Activate/Deactivate a list of previously-installed ROM Patches.

```

| Send 32bit Command (0000000Bh/0000000Ch)
 Send 32bit Number of patches (N)
 Send Nx32bit List of PatchID's (to be activated/deactivated) |
| --- |

```

| ______________________________ Misc Functions ______________________________ |
| --- |

**BMI_CMD(08h) - BMI_GET_TARGET_ID aka BMI_GET_TARGET_INFO - Get Version**

```

| Send 32bit Command (00000008h)
 Receive 32bit Value (FFFFFFFFh) ;ROM version (or FFFFFFFFh)
 If above value is FFFFFFFFh then following extra data is appended:
 Receive 32bit Value (0000000Ch) ;total size of extra data
 Receive 32bit Value (20000188h) ;ROM version
 Receive 32bit Value (00000002h) ;TARGET_TYPE (2=AR6002) |
| --- |

**BMI_CMD(0Fh) - BMI_NVRAM_PROCESS ;not implemented in DSi, Invalid (ignored)**

Unknown purpose, said to "process or execute" something in "NVRAM" with a name
in "LE format":

"Cause Target to search NVRAM (if any) for a segment with the specified name
and process it according to NVRAM metadata."

```

| Send 32bit Command (0000000Fh)
 Send 16x8bit Name (16 characters, in "LE format", uh?)
 Receive 32bit Value returned from last executed NVRAM segment (or 0=None) |
| --- |

**BMI_CMD(00h) - BMI_NO_COMMAND - Invalid (ignored)**

**BMI_CMD(0Fh..FFFFFFFFh) - N/A - Invalid (ignored)**

```

| Send 32bit Command (00000000h, or 0000000Fh..FFFFFFFFh) |
| --- |

**Segmented Write**

The Segmented Write feature is implemented only in newer ROMs (not in DSi with
AR6002). It is invoked via BMI_CMD(03h) with destination address set to special
value 00001234h.

```

| Send 32bit Command (00000003h) <-- same as Write Memory command
 Send 32bit Address (00001234h) <-- special value for Segmented Write
 Send 32bit Length (should be max 1F4h due to MBOX size)
 Send LEN bytes, as described below... |
| --- |

The transferred data should contain a file header:

```

| 00h 4 File ID (544D4753h) ("SGMT")
 04h 4 File Flags (0=Raw, 1=BMI_SGMTFILE_FLAG_COMPRESS) |
| --- |

Followed by one or more segments:

```

| 00h 4 Destination Address (the actual address, no special value here)
 04h 4 Segment Length (N) (or special value FFFFFFFxh)
 08h N Data (N bytes) (no data when N=FFFFFFFxh) |
| --- |

Special values for "Segment Length" (all with bit31=1):

```

| FFFFFFFFh ;End of segmented data file (should occur as last segment)
 FFFFFFFEh ;Board Data (write "hi_board_data+Address", instead raw "Address")
 FFFFFFFDh ;Set App Start=Addresss; like BMI_CMD(05h)
 FFFFFFFCh ;Call Address; like BMI_CMD(04h), but without param/return value |
| --- |

Compressed Segmented data is said to work in two ways (?):

```

| 1) Use BMI_LZ_STREAM_START with Addr=00001234h, followed by BMI_LZ_DATA, or
 2) Use BMI_WRITE_MEMORY with Addr=00001234h and file header Flags=1. |
| --- |

Atheros has invented that weird feature for "backwards compatibility & darn
convenience", which is both sad and funny because Atheros is definitely unable
to <maintain> backwards compatibility (such as keeping the same WMI
command/event numbering in firmware revisions), but as it seems, Atheros is
believing that "backwards compatibility" is something that can be
<invented> by adding obscure features.

| DSi Atheros Wifi - MBOX Transfer Headers |
| --- |

MBOX transfers include a small header for distinguishing between Data Packets
and Commands/Events, and for indicating the actual length (as opposed to the
MBOX transfer length, which is usually garbage-padded to 80h-byte boundary).

The exception are BMI Bootloader commands/responses (which don't have such
headers or padding).

**SDIO Functions used after BMI init**

```

| Func[0:00005h] R 1 Interrupt Flags (bit1=Function 1 IRQ)
 Func[1:00400h] R 10h Interrupt Status, etc. and Lookahead
 Func[1:00800h..00FFF] R/W .. MBOX0 |
| --- |

**MBOX Send Header/Body**

```

| 00h 1 Type (0=BootInfo, 1=WMI Command, 2/3/4/5=Data Packet BE/BK/VI/VO)
 01h 1 Request Ack (00h=No, 01h=Yes)
 02h 2 Length of entries [06h..end] (LEN)
 04h 2 ?
 06h LEN Body (BootInfo/Event/Data)
 .. .. Padding to 80h-byte boundary (for SDIO block size 80h) |
| --- |

Send Body for BootInfo:

```

| 06h .. related to BE/BK/VI/VO stuff (best effort, background, video, voice) |
| --- |

Send Body for WMI Commands:

```

| 06h 2 WMI Command Number
 08h .. Parameters |
| --- |

Send Body for Data Packets:

```

| 06h 2 <Unknown6>?? 0000h or 1C00h ;maybe whatever ?
 08h 6 Destination (MAC Addr of Router) (or FF:FF:FF:FF:FF:FF=Broadcast)
 0Eh 6 Source (MAC Addr of DSi console)
 14h 2 Length Data at [16h...] ;(LEN1-10h) ;<-- BIG-ENDIAN !!!
 16h .. Data (usually starting with LLC stuff, ie. AAh,AAh,03h,00h...) |
| --- |

Send Body for empty Data Packets (used when changing WPA/WPA2 CIPHER keys):

```

| 06h 1 Garbage (usually LSB of a previously used WMI command number)
 07h 1 Unknown (02h) |
| --- |

**MBOX Receive Header/Body**

```

| 00h 1 Type (0=Ack only, 1=WMI Event, 2/3/4/5=Data Packet BE/BK/VI/VO)
 01h 1 Ack Present (00h=No, 02h=Yes)
 02h 2 Length of entries [06h..end] (LEN1+LEN2)
 04h 1 Length of Ack at [06h+LEN1..end] (LEN2) ;garbage when [01h]=00h
 05h 1 ? (00h,20h?,7Fh,F4h,FFh)
 06h LEN1 Body (Event/Data)
 06h+LEN1 LEN2 Ack List
 .. .. Padding to 80h-byte boundary (for SDIO block size 80h) |
| --- |

Receive Body for Ack only:

```

| 06h - N/A (or boot request info; occurs only shortly after BMI) |
| --- |

Receive Body for Events:

```

| 06h 2 WMI Event Number
 08h .. Parameters |
| --- |

Receive Body for Data Packets:

```

| 06h 1 RSSI (Received Signal Strength Indicator) (00h..3Ch) (aka 0..60)
 07h 1 <Unknown7> 00h
 08h 6 Destination (MAC Addr of DSi console) (or FF:FF:FF:FF:FF:FF=Broadcast)
 0Eh 6 Source (MAC Addr of Router)
 14h 2 Length of Data entries at [16h..(end-xtracrap)] ;<-- BIG-ENDIAN !!!
 16h .. Data (usually LLC stuff, ie. AAh,AAh,03h.. or, once F0h,F0h,03h..??) |
| --- |

Ack List (aka Trailer) (can contain multiple List items):

```

| X+00h List Item Type (01h=Ack, 02h=Lookahead)
 X+01h List Item Len (02h*N for Ack's, 06h for Lookahead)
 X+02h List Item data |
| --- |

Ack Items (can contain multiple Ack items):

```

| Y+00h Ack Item Type (01h=Command Ack, 02h=Data Ack, 05h=Data Ack, too?)
 Y+01h Ack Item Count? (usually 1) (or 2 for double-ack?) |
| --- |

Lookahead Item (if valid: allows to omit reading Func[1:00408h]):

```

| Z+00h Lookahead valid ID1 (00h=No, 55h=Valid)
 Z+01h Lookahead MBOX0 (or garbage if ID1/ID2 not valid)
 Z+05h Lookahead valid ID2 (00h=No, AAh=Valid) |
| --- |

**Types**

There are four data packet types (BE/BK/VI/VO), most packets are using type=2
(Best Effort), however, some weird access points are sending other types (eg.
Asus RT-AX56U has type=4 (video) in WPA2/EAPOL handshakes). Basically, one can
ignore that, and simply all types in range 2,3,4,5 as data packets.

**Notes**

Data Packet Type is usually 02h=BE (best effort),

The send/receive DATA headers are apparently containing incomplete wifi "Frame
Headers":

```

| without 3rd address field
 without WEP entries (those are apparently automatically inserted)
 without Frame Control, Duration/ID, Sequence Control
 there seem to be only Data Frames (no Managment/Control Frames) |
| --- |

That, unless some of the missing info would be encoded in the <Unknown>
header entries.

| DSi Atheros Wifi - WMI Misc Commands |
| --- |

**WMIcmd(0004h) - WMI_SYNCHRONIZE_CMD**

Parameters (01h bytes):

```

| 00h A_UINT8 1 dataSyncMap ;00h, 01h, or 09h used? |
| --- |

**WMIcmd(0009h) - WMI_SET_BSS_FILTER_CMD ;aka WMI_BSS_FILTER_CMD**

Parameters (08h bytes):

```

| 00h A_UINT8 1 bssFilter; /* see WMI_BSS_FILTER
 01h A_UINT8 1 reserved1; /* For alignment
 02h A_UINT16 2 reserved2; /* For alignment
 04h A_UINT32 4 ieMask; |
| --- |

WMI_BSS_FILTER values:

```

| NONE_BSS_FILTER = 00h ;no beacons forwarded
 ALL_BSS_FILTER = 01h ;all beacons forwarded
 PROFILE_FILTER = 02h ;only beacons matching profile
 ALL_BUT_PROFILE_FILTER = 03h ;all but beacons matching profile
 CURRENT_BSS_FILTER = 04h ;only beacons matching current BSS
 ALL_BUT_BSS_FILTER = 05h ;all but beacons matching BSS
 PROBED_SSID_FILTER = 06h ;beacons matching probed ssid
 LAST_BSS_FILTER = 07h ;marker only |
| --- |

**WMIcmd(000Ah) - WMI_SET_PROBED_SSID_CMD**

Parameters (23h bytes):

```

| 00h A_UINT8 1 entryIndex; /* 0 to MAX_PROBED_SSID_INDEX
 01h A_UINT8 1 flag; /* WMI_SSID_FLG
 02h A_UINT8 1 ssidLength;
 03h A_UINT8 32 ssid[32]; |
| --- |

#define MAX_PROBED_SSID_INDEX = 15

WMI_SSID_FLAG values:

```

| DISABLE_SSID_FLAG = 00h /* disables entry
 SPECIFIC_SSID_FLAG = 01h /* probes specified ssid
 ANY_SSID_FLAG = 02h /* probes for any ssid |
| --- |

**WMIcmd(000Bh) - WMI_SET_LISTEN_INT_CMD**

Parameters (04h bytes):

```

| 00h A_UINT16 2 listenInterval;
 02h A_UINT16 2 numBeacons; |
| --- |

The Listen interval is between 15 and 3000 TUs

```

| MIN_LISTEN_INTERVAL = 15 ;min = 15
 MAX_LISTEN_INTERVAL = 5000 ;max = 5000 or 3000, uh?
 MIN_LISTEN_BEACONS = 1
 MAX_LISTEN_BEACONS = 500 |
| --- |

**WMIcmd(000Ch) - WMI_SET_BMISS_TIME_CMD**

Parameters (04h bytes):

```

| 00h A_UINT16 2 bmissTime;
 02h A_UINT16 2 numBeacons; |
| --- |

Valid values are between 1000 and 5000 TUs

```

| MIN_BMISS_TIME = 1000
 MAX_BMISS_TIME = 5000
 MIN_BMISS_BEACONS = 1
 MAX_BMISS_BEACONS = 50 |
| --- |

**WMIcmd(000Fh) - WMI_SET_BEACON_INT_CMD**

Parameters (02h bytes):

```

| 00h A_UINT16 2 beaconInterval; |
| --- |

**WMIcmd(001Ah) - WMI_SET_PMKID_CMD**

Parameters:

```

| 00h A_UINT8 6 bssid[ATH_MAC_LEN];
 06h A_UINT8 1 enable; /* PMKID_ENABLE_FLG
 07h A_UINT8 16 pmkid[WMI_PMKID_LEN]; |
| --- |

#define WMI_PMKID_LEN = 16

PMKID_ENABLE_FLG values:

```

| PMKID_DISABLE = 0
 PMKID_ENABLE = 1 |
| --- |

**WMIcmd(001Dh) - WMI_SET_ASSOC_INFO_CMD**

Parameters:

```

| 00h A_UINT8 1 ieType
 01h A_UINT8 1 bufferSize
 02h A_UINT8 N*1 assocInfo[1] ;up to WMI_MAX_ASSOC_INFO_LEN |
| --- |

A maximum of 2 private IEs can be sent in the [Re]Assoc request.

A 3rd one, the CCX version IE can also be set from the host.

```

| WMI_MAX_ASSOC_INFO_TYPE = 2
 WMI_CCX_VER_IE = 2 /* ieType to set CCX Version IE
 WMI_MAX_ASSOC_INFO_LEN = 240 |
| --- |

**WMIcmd(001Eh) - WMI_ADD_BAD_AP_CMD**

Parameters (07h bytes):

```

| 00h A_UINT8 1 badApIndex ;0 to WMI_MAX_BAD_AP_INDEX
 01h A_UINT8 6 bssid[ATH_MAC_LEN]
 WMI_MAX_BAD_AP_INDEX = 1 |
| --- |

**WMIcmd(001Fh) - WMI_DELETE_BAD_AP_CMD**

Parameters (01h bytes):

```

| 00h A_UINT8 1 badApIndex ;0 to WMI_MAX_BAD_AP_INDEX |
| --- |

**WMIcmd(0023h) - WMI_SET_ACCESS_PARAMS_CMD**

Parameters:

```

| 00h A_UINT16 2 txop ;in units of 32 usec
 02h A_UINT8 1 eCWmin
 03h A_UINT8 1 eCWmax
 04h A_UINT8 1 aifsn
 05h A_UINT8 1 ac
 WMI_DEFAULT_TXOP_ACPARAM = 0 /* implies one MSDU
 WMI_DEFAULT_ECWMIN_ACPARAM = 4 /* corresponds to CWmin of 15
 WMI_DEFAULT_ECWMAX_ACPARAM = 10 /* corresponds to CWmax of 1023
 WMI_MAX_CW_ACPARAM = 15 /* maximum eCWmin or eCWmax
 WMI_DEFAULT_AIFSN_ACPARAM = 2
 WMI_MAX_AIFSN_ACPARAM = 15 |
| --- |

**WMIcmd(0025h) - WMI_SET_OPT_MODE_CMD**

Parameters (01h bytes on DSi?, but parameter structure is undocumented):

```

| 00h A_UINT8 optMode (documented in code from 2008 only) |
| --- |

OPT_MODE_TYPE values:

```

| SPECIAL_OFF = unknown (maybe 0 or 1 or so) ;\
 SPECIAL_ON = SPECIAL_OFF+1 ; code from 2008 only
 PYXIS_ADHOC_ON = SPECIAL_OFF+2 ; (removed/undoc in 2010)
 PYXIS_ADHOC_OFF = SPECIAL_OFF+3 ;/ |
| --- |

**WMIcmd(0026h) - WMI_OPT_TX_FRAME_CMD**

Unknown/undocumented purpose. Unknown if/how this is implemented in DSi. The
DSi does redirect cmd 0026h to a function handler, but that handler does just
trigger a misalign exception. However, the DSi does also have a function for
handling the command (with "opt_tx_frame_cmd" string in AR6013/AR6014), but
that function doesn't seem to be called upon command number 0026h. Maybe the
CMD is triggered upon special flags in the SDIO transfer block header, rather
than by the command number?

Parameters (11h bytes on DSi)

```

| Unknown (11h bytes, but parameter structure is undocumented) |
| --- |

See also: WMI_OPT_RX_FRAME_EVENT (counterpart in opposite direction), and
WMI_SET_OPT_MODE_CMD.

**WMIcmd(0027h) - WMI_SET_VOICE_PKT_SIZE_CMD**

Parameters (02h bytes):

```

| 00h A_UINT16 2 voicePktSize |
| --- |

**WMIcmd(0028h) - WMI_SET_MAX_SP_LEN_CMD**

Parameters (01h bytes):

```

| 00h A_UINT8 1 maxSPLen |
| --- |

APSD_SP_LEN_TYPE values:

```

| DELIVER_ALL_PKT = 00h
 DELIVER_2_PKT = 01h
 DELIVER_4_PKT = 02h
 DELIVER_6_PKT = 03h |
| --- |

**WMIcmd(002Eh) - WMI_EXTENSION_CMD ;prefix for WMIX "Non-wireless extensions"**

Parameters:

```

| 00h UNIT32 4 WMIX Command (values 2001h and up) ;WMIX_CMD_HDR
 04h ... .. WMIX Parameter(s) |
| --- |

Prefix for WMIX commands.

**WMIcmd(0031h) - WMI_SET_LPREAMBLE_CMD**

Parameters (01h bytes on DSi?, but other sources claim 02h bytes):

```

| 01h 02h <---- total size (on DSi it's 01h, ie. left column)
 00h 00h A_UINT8 1 status
 -- 01h A_UINT8 1 preamblePolicy |
| --- |

WMI_LPREAMBLE_STATUS values:

```

| WMI_LPREAMBLE_DISABLED = 0
 WMI_LPREAMBLE_ENABLED = 1 |
| --- |

WMI_PREAMBLE_POLICY values:

```

| WMI_IGNORE_BARKER_IN_ERP = 0
 WMI_DONOT_IGNORE_BARKER_IN_ERP = 1 |
| --- |

**WMIcmd(0032h) - WMI_SET_RTS_CMD**

Parameters (02h bytes):

```

| 00h A_UINT16 2 threshold |
| --- |

**WMIcmd(0036h) - WMI_SET_AUTH_MODE_CMD ;aka WMI_SET_RECONNECT_AUTH_MODE_CMD**

Parameters (01h bytes):

```

| 00h A_UINT8 1 mode |
| --- |

WMI_AUTH_MODE values:

```

| RECONN_DO_AUTH = 00h
 RECONN_NOT_AUTH = 01h |
| --- |

Set authentication mode

**WMIcmd(0037h) - WMI_SET_REASSOC_MODE_CMD**

Parameters (01h bytes):

```

| 00h A_UINT8 1 mode |
| --- |

WMI_REASSOC_MODE values:

```

| REASSOC_DO_DISASSOC = 00h
 REASSOC_DONOT_DISASSOC = 01h |
| --- |

Set authentication(?) mode (uh, so SET_REASSOC is same as SET_AUTH?)

**WMIcmd(0038h) - WMI_SET_WMM_CMD**

Parameters (01h bytes):

```

| 00h A_UINT8 1 status |
| --- |

WMI_WMM_STATUS values:

```

| WMI_WMM_DISABLED = 0
 WMI_WMM_ENABLED = 1 |
| --- |

WMM stands for Wi-Fi Multimedia (WMM) aka Wireless Multimedia Extensions (WME)
aka some QOS related stuff?

"WMM prioritizes traffic according to four Access Categories (AC): voice
(AC_VO), video (AC_VI), best effort (AC_BE), and background (AC_BK)."

**WMIcmd(0039h) - WMI_SET_WMM_TXOP_CMD**

Parameters (01h bytes):

```

| 00h A_UINT8 1 txopEnable |
| --- |

WMI_TXOP_CFG values:

```

| WMI_TXOP_DISABLED = 0
 WMI_TXOP_ENABLED = 1 |
| --- |

**WMIcmd(003Dh) - WMI_SET_KEEPALIVE_CMD**

Parameters (01h bytes):

```

| 00h A_UINT8 1 keepaliveInterval ;MAYBE in seconds? usually 00h=what? |
| --- |

**WMIcmd(003Eh) - WMI_GET_KEEPALIVE_CMD ;reply 003Eh**

Parameters:

```

| None |
| --- |

Reply: WMI_GET_KEEPALIVE_REPLY

```

| 00h A_BOOL 4 configured (what?)
 04h A_UINT8 1 keepaliveInterval |
| --- |

**WMIcmd(003Fh) - WMI_SET_APPIE_CMD ;aka WMI_SET_APP_IE_CMD**

Can be used to add custom Information Elements (IE) to beacons?

Parameters (old=2+N bytes, or new=4+N bytes):

```

| Older version (used on DSi, and on older 3DS firmwares):
 00h A_UINT8 1 mgmtFrmType ;one of WMI_MGMT_FRAME_TYPE
 01h A_UINT8 1 ieLen ;Length of the IE to be added to the MGMT frame
 02h A_UINT8 N*1 ieInfo[1] ;
 Newer version (used on newer 3DS firmwares):
 00h A_UINT8 1 mgmtFrmType ;one of WMI_MGMT_FRAME_TYPE
 01h A_UINT8 1 reserved (zero, actually USED as MSB of above type value)
 02h A_UINT16 2 ieLen ;Length of the IE to be added to the MGMT frame
 04h A_UINT8 N*1 ieInfo[1] |
| --- |

WMI_MGMT_FRAME_TYPE values:

```

| WMI_FRAME_BEACON = 0 ;\old version (with 8bit ieLen):
 WMI_FRAME_PROBE_REQ = 1 ; supports type 0,1,2,3,4 (or 5..FFh=same as 4)
 WMI_FRAME_PROBE_RESP = 2 ; new version (with 16bit ieLen):
 WMI_FRAME_ASSOC_REQ = 3 ; supports type 1,3 only (ignores all other)
 WMI_FRAME_ASSOC_RESP = 4 ;/ |
| --- |

Add Application specified IE to a management frame

```

| WMI_MAX_IE_LEN = 78 ;older versions (with 8bit ieLen) ;<-- actual limit
 WMI_MAX_IE_LEN = 255 ;older versions (with 8bit ieLen) ;<-- incorrect
 WMI_MAX_IE_LEN = 1024 ;newer versions (with 16bit ieLen) (from 2014) |
| --- |

Uh, if that newer 3DS firmware is also used in DSi mode (?) then the changed
parameter format would be incompatible with DSi software (the APPIE command is
rarely (perhaps never) used though).

**WMIcmd(0041h) - WMI_SET_WSC_STATUS_CMD ;aka WSC_REG**

Parameters:

```

| Unknown (none? or maybe UINT8 or so, maybe with values listed below)
 DSi uses 1 byte parameter.
 WSC_REG_ACTIVE = 1
 WSC_REG_INACTIVE = 0 |
| --- |

Notify the WSC registration status to the target.

WSC means Wifi Simple Config?

**WMIcmd(0049h) - WMI_HOST_EXIT_NOTIFY_CMD ;special DSi/3DS command**

Parameters:

```

| None |
| --- |

This is apparently a special nintendo command, supported on DSi and 3DS
(supported on AR6002/AR6013/AR6014, whereof, AR6013/AR6014 have it bundled with
a "wmi_host_exit_notify_cmd" message, so, this is apparently not the official
WMI_SET_QOS_SUPP_CMD from atheros).

**WMIcmd(0049h) - WMI_SET_QOS_SUPP_CMD ;not implemented in DSi/3DS...?**

Parameters:

```

| 00h A_UINT8 1 status; |
| --- |

QOS or QoS stands for "Quality of service"?

Note: Some older source code from 2008 did have this command accidently
inserted between WMIcmd(0039h and 003Ah), thereby smashing the numbering for
WMIcmd(003Ah..004xh), that issue is fixed in newer source code from 2010.

...Developer commands...

**WMIcmd(F002h) - WMI_SET_WHALPARAM_CMD ;aka WHAL_PARAMCMD**

Parameters:

```

| 00h A_UINT8 1 whalCmdId; ;see WHAL_CMDID enumeration
 01h A_UINT8 .. data[1]; ;aka SETCABTO structure ? |
| --- |

Generic Hal Interface for setting hal parameters.

Add new Set HAL Param cmdIds here for newer params.

WHAL_CMDID values (only one defined):

```

| WHAL_SETCABTO_CMDID = 1 |
| --- |

WHAL_SETCABTO_PARAM structure:

```

| A_UINT8 cabTimeOut; |
| --- |

**WMIcmd(F004h, or formerly 0048h) - WMI_SET_AKMP_PARAMS_CMD**

Parameters (04h bytes):

```

| 00h A_UINT32 4 akmpInfo; |
| --- |

WMI_AKMP_MULTI_PMKID_EN = 000001h

**WMIcmd(F005h, or formerly 0049h) - WMI_SET_PMKID_LIST_CMD**

Parameters (01h bytes on DSi?, but below would be 4+N*? bytes?):

```

| 00h A_UINT32 4 numPMKID;
 04h WMI_PMKID N*.. pmkidList[WMI_MAX_PMKID_CACHE]; |
| --- |

#define WMI_MAX_PMKID_CACHE = 8

WMI_PMKID structure:

```

| A_UINT8 pmkid[WMI_PMKID_LEN]; |
| --- |

**WMIcmd(F006h, or formerly 004Ah) - WMI_GET_PMKID_LIST_CMD ;reply 1019h**

Parameters:

```

| Unknown (none?) |
| --- |

Reply: WMI_GET_PMKID_LIST_EVENT ;aka WMI_PMKID_LIST_REPLY

```

| 00h A_UINT32 4 numPMKID;
 04h A_UINT8 N*6 bssidList[ATH_MAC_LEN][1];
 .. WMI_PMKID N*1 pmkidList[1]; |
| --- |

"Following the Number of PMKIDs is the list of PMKIDs" uh, what exactly?

WMI_PMKID structure:

```

| A_UINT8 pmkid[WMI_PMKID_LEN]; |
| --- |

| DSi Atheros Wifi - WMI Misc Events |
| --- |

**WMIevent(000Eh) - WMI_GET_CHANNEL_LIST_CMD**

**WMIevent(001Ch) - WMI_GET_TX_PWR_CMD**

**WMIevent(0035h) - WMI_GET_FIXRATES_CMD**

**WMIevent(003Eh) - WMI_GET_KEEPALIVE_CMD**

**WMIevent(F001h) - WMI_GET_BITRATE_CMD**

Events/Responses with same ID as corresponding command.

See corresponding commands for description of reply data.

**WMIevent(1001h) - WMI_READY_EVENT**

This event exists with numerous (incorrect) definitions, claiming the event
data to be 07h, 0Bh, 0Ch, 0Fh, or 10h bytes in length:

Event Data (0Ch bytes on DSi, as returned by DSi firmware):

```

| 00h A_UINT8 6 macaddr[ATH_MAC_LEN] ;MAC addr of DSi console
 06h A_UINT8 1 phyCapability (=02h aka "11G") ;WMI_PHY_CAPABILITY
 07h A_UINT8 1 unused/padding
 08h A_UINT32 4 version (=2100007Bh/2300006Ch) (firmware version) |
| --- |

Event Data (07h bytes, exists in AR6002 ROM, but overridden by DSi firmware):

```

| 00h A_UINT8 6 macaddr[ATH_MAC_LEN]
 06h A_UINT8 1 phyCapability ;WMI_PHY_CAPABILITY |
| --- |

Event Data (0Bh bytes, accidently claimed to be so in source code from 2008):

```

| 00h A_UINT32 4 version
 04h A_UINT8 6 macaddr[ATH_MAC_LEN]
 0Ah A_UINT8 1 phyCapability ;WMI_PHY_CAPABILITY |
| --- |

Event Data (0Fh bytes, accidently claimed to be so in source code from 2010):

```

| 00h A_UINT32 4 sw_version
 04h A_UINT32 4 abi_version
 08h A_UINT8 6 macaddr[ATH_MAC_LEN]
 0Eh A_UINT8 1 phyCapability ;WMI_PHY_CAPABILITY |
| --- |

Event Data (10h bytes, as done by 3DS function in wifi firmware RAM):

```

| 00h A_UINT8 6 macaddr[ATH_MAC_LEN] ;MAC addr of DSi console
 06h A_UINT8 1 phyCapability (=02h aka "11G") ;WMI_PHY_CAPABILITY
 07h A_UINT8 1 unused/padding
 08h A_UINT32 4 version (230000B3h) (firmware version)
 0Ch A_UINT16 2 whus ;=[004134h] ;some I/O register ???
 0Eh A_UINT16 2 what ;=[520088h] ;hi_hci_uart_baud ??? |
| --- |

WMI_PHY_CAPABILITY values (maybe the "11" is related to "IEEE 802.11"?):

```

| WMI_11A_CAPABILITY = 1
 WMI_11G_CAPABILITY = 2
 WMI_11AG_CAPABILITY = 3
 WMI_11NA_CAPABILITY = 4
 WMI_11NG_CAPABILITY = 5
 WMI_11NAG_CAPABILITY = 6 |
| --- |

**WMIevent(1004h) - WMI_BSSINFO_EVENT aka WMI_BSS_INFO**

Event Data (0Ch+N bytes or 10h+N bytes on DSi, depending on version setting):

```

| When version<2:
 00h A_UINT16 2 channel ;in MHz ;\
 02h A_UINT8 1 frameType ;see WMI_BI_FTYPE ;
 03h A_UINT8 1 snr ;eg. 33h ; WMI_BSS_INFO_HDR
 04h A_INT16 2 rssi ;eg. FFD4h aka "33h-95" ; version 1 (10h bytes)
 06h A_UINT8 6 bssid[ATH_MAC_LEN] ;
 0Ch A_UINT32 4 ieMask ;/
 10h BODY ... beacon or probe-response frame body ;-Body (timestamp etc)
 When version>=2:
 00h A_UINT16 2 channel ;in MHz ;\
 02h A_UINT8 1 frameType ;see WMI_BI_FTYPE ; WMI_BSS_INFO_HDR2
 03h A_UINT8 1 snr (implies "rssi=snr-95" in v2) ; version 2 (0Ch bytes)
 04h A_UINT8 6 bssid[ATH_MAC_LEN] ;
 0Ah A_UINT16 2 ieMask (only 2 bytes in v2) ;/
 0Ch BODY ... beacon or probe-response frame body ;-Body (timestamp etc) |
| --- |

BUG: The "snr" value can be negative, but isn't sign-expanded to 16bit, and the
and the rssi value at [04h] does then contain nonsense (eg. spotted bytes
at[03h..05h] are "FC 9D 00").

The BODY starts with 64bit Timestamp, 16bit BeaconInterval, etc. For details,
see:

DS Wifi IEEE802.11 Managment Frames (Type=0)

The size of the BODY varies per access point (from around 64 bytes to exceeding
256 bytes).

The DSi firmware contains code for both of the above HDR versions (unknown
if/when/how it does use which version; the DSi Browser receives the older type,
with 10h+N bytes).

BSS Info Event.

Mechanism used to inform host of the presence and characteristic of wireless
networks present. Consists of bss info header followed by the beacon or
probe-response frame body. The 802.11 header is not included.

BSS INFO HDR version 2.0:

With 6 bytes HTC header and 6 bytes of WMI header WMI_BSS_INFO_HDR cannot be
accomodated in the removed 802.11 management header space.

```

| - Reduce the ieMask to 2 bytes as only two bit flags are used
 - Remove rssi and compute it on the host. rssi = snr - 95 |
| --- |

WMI_BI_FTYPE values:

```

| BEACON_FTYPE = 01h
 PROBERESP_FTYPE = 02h
 ACTION_MGMT_FTYPE = 03h
 PROBEREQ_FTYPE = 04h |
| --- |

BSS_ELEMID values (unclear WHAT that is, maybe the "ieMask" stuff, which said
to use "two bit flags"):

```

| BSS_ELEMID_CHANSWITCH = 01h ;value for bit0? (or bit-number for bit1?)
 BSS_ELEMID_ATHEROS = 02h ;value for bit1? (or bit-number for bit2?) |
| --- |

**WMIevent(1006h) - WMI_REGDOMAIN_EVENT ;aka WMI_REG_DOMAIN_EVENT**

Event Data (04h bytes):

```

| 00h A_UINT32 4 regDomain ;80000188h on DSi (after firmware upload) |
| --- |

New Regulatory Domain Event.

Value 80000188h on DSi/AR6002 and 80000348h on DSi/AR6013 (these are based on
the wifi firmware's "RGBD" DataSet, with 0188h="JP" and 0348h="US").

The country code seems to be derived from I2C EEPROM[008h], and the database is
used to translate the country code to enabled channels.

**WMIevent(1008h) - WMI_NEIGHBOR_REPORT_EVENT**

Event Data:

typedef PREPACK struct {

```

| 00h A_INT8 1 numberOfAps;
 01h WMI_NEIGHBOR_INFO N*7 neighbor[1]; |
| --- |

WMI_NEIGHBOR_INFO structure:

```

| A_UINT8 6 bssid[ATH_MAC_LEN];
 A_UINT8 1 bssFlags; /* see WMI_BSS_FLAGS |
| --- |

WMI_BSS_FLAGS values:

```

| WMI_DEFAULT_BSS_FLAGS = 00h
 WMI_PREAUTH_CAPABLE_BSS = 01h
 WMI_PMKID_VALID_BSS = 02h |
| --- |

The WMI_NEIGHBOR_REPORT Event is generated by the target to inform the host of
BSS's it has found that matches the current profile.

It can be used by the host to cache PMKs and/to initiate pre-authentication if
the BSS supports it. The first bssid is always the current associated BSS.

The bssid and bssFlags information repeats according to the number or APs
reported.

**WMIevent(100Eh) - WMI_OPT_RX_FRAME_EVENT aka WMI_OPT_RX_INFO**

Event Data (10h+N bytes):

```

| 00h A_UINT16 2 channel ;\
 02h A_UINT8 1 frameType ;see WMI_OPT_FTYPE ; special frame info header
 03h A_INT8 1 snr ;
 04h A_UINT8 6 srcAddr[ATH_MAC_LEN] ;
 0Ah A_UINT8 6 bssid[ATH_MAC_LEN] ;/
 10h ... .. body (having WHAT length?) ;-special frame body |
| --- |

Special frame receive Event.

Mechanism used to inform host of the receiption of the special frames.

The 802.11 header is not included.

See also: WMI_OPT_TX_FRAME_CMD

**WMIevent(1010h) - WMI_EXTENSION_EVENT ;prefix for WMIX events...**

Unknown, probably similar prefix as "WMIX_CMD_HDR" with 32bit WMIX value.

**WMIevent(1011h) - WMI_CAC_EVENT**

Supposedly related to Call Admission Control (CAC), eg. for VoIP.

Event Data (42h bytes):

```

| 00h A_UINT8 1 ac;
 01h A_UINT8 1 cac_indication;
 02h A_UINT8 1 statusCode;
 03h A_UINT8 3Fh tspecSuggestion[WMM_TSPEC_IE_LEN]; |
| --- |

CAC_INDICATION values:

```

| CAC_INDICATION_ADMISSION = 00h
 CAC_INDICATION_ADMISSION_RESP = 01h
 CAC_INDICATION_DELETE = 02h
 CAC_INDICATION_NO_RESP = 03h |
| --- |

#define WMM_TSPEC_IE_LEN = 63

**WMIevent(1017h) - WMI_APLIST_EVENT**

Event Data:

```

| 00h A_UINT8 1 apListVer;
 01h A_UINT8 1 numAP;
 02h WMI_AP_INFO N*8 apList[1]; |
| --- |

APLIST_VER values (only one defined):

```

| APLIST_VER1 = 1, |
| --- |

WMI_AP_INFO structure:

```

| typedef PREPACK union {
 WMI_AP_INFO_V1 8 apInfoV1;
 } POSTPACK WMI_AP_INFO; |
| --- |

WMI_AP_INFO_V1 structure:

```

| A_UINT8 6 bssid[ATH_MAC_LEN];
 A_UINT16 2 channel; |
| --- |

**WMIevent(1019h) - WMI_GET_PMKID_LIST_EVENT ;reply to CMD F006h**

See WMI_GET_PMKID_LIST_CMD for response details.

| DSi Atheros Wifi - WMI Connect Functions |
| --- |

**WMIcmd(0001h) - WMI_CONNECT_CMD**

Parameters (34h bytes):

```

| 00h A_UINT8 1 networkType ;somewhat NETWORK_TYPE related ? (1)
 01h A_UINT8 1 dot11AuthMode ;aka DOT11_AUTH_MODE ? (1=Open, 2=WEP)
 02h A_UINT8 1 authMode ;aka AUTH_MODE ? (1)
 03h A_UINT8 1 pairwiseCryptoType ;aka CRYPTO_TYPE (1=Open, 2=WEP)
 04h A_UINT8 1 pairwiseCryptoLen (0)
 05h A_UINT8 1 groupCryptoType ;aka CRYPTO_TYPE (1=Open, 2=WEP)
 06h A_UINT8 1 groupCryptoLen (0)
 07h A_UINT8 1 ssidLength
 08h A_UCHAR 32 ssid[WMI_MAX_SSID_LEN]
 28h A_UINT16 2 channel ;in MHz
 2Ah A_UINT8 6 bssid[ATH_MAC_LEN]
 30h A_UINT32 4 ctrl_flags (0) |
| --- |

Returns WMI_CONNECT_EVENT (or WMI_DISCONNECT_EVENT, eg. upon bad WEP password).

NETWORK_TYPE values:

```

| INFRA_NETWORK = 01h ;DSi uses 01h for Open/WEP/WPA/WPA2
 ADHOC_NETWORK = 02h
 ADHOC_CREATOR = 04h
 AP_NETWORK = 10h |
| --- |

NETWORK_SUBTYPE values (unknown purpose, and unknown if they start at 0 or 1 or
so):

```

| SUBTYPE_NONE = unknown (maybe 0 or 1 or so?)
 SUBTYPE_BT = SUBTYPE_NONE+1
 SUBTYPE_P2PDEV = SUBTYPE_NONE+2
 SUBTYPE_P2PCLIENT = SUBTYPE_NONE+3
 SUBTYPE_P2PGO = SUBTYPE_NONE+4 |
| --- |

DOT11_AUTH_MODE values:

```

| OPEN_AUTH = 01h ;DSi uses 01h for Open/WPA/WPA2
 SHARED_AUTH = 02h ;DSi uses 02h for WEP
 LEAP_AUTH = 04h /* different from IEEE_AUTH_MODE definitions |
| --- |

AUTH_MODE values:

```

| WMI_NONE_AUTH = 01h ;DSi uses 01h for Open/WEP
 WMI_WPA_AUTH = 02h ;\whatever maybe for RADIUS?
 WMI_WPA2_AUTH = 04h ;/
 WMI_WPA_PSK_AUTH = 08h ;DSi uses 03h (not 08h) for WPA-PSK
 WMI_WPA2_PSK_AUTH = 10h ;DSi uses 05h (not 10h) for WPA2-PSK
 WMI_WPA_AUTH_CCKM = 20h ;\whatever for "Cisco Centralized Key Management"?
 WMI_WPA2_AUTH_CCKM = 40h ;/ |
| --- |

CRYPTO_TYPE values:

```

| NONE_CRYPT = 01h ;DSi uses 01h for Open
 WEP_CRYPT = 02h ;DSi uses 02h for WEP
 TKIP_CRYPT = 04h ;DSi uses 03h (not 04h) for WPA
 AES_CRYPT = 08h ;DSi uses 04h (not 08h) for WPA2
 WAPI_CRYPT = 10h ;only if WAPI_ENABLE |
| --- |

connect "ctrl_flags":

```

| 0 CONNECT_ASSOC_POLICY_USER = 0001h
 1 CONNECT_SEND_REASSOC = 0002h
 2 CONNECT_IGNORE_WPAx_GROUP_CIPHER = 0004h
 3 CONNECT_PROFILE_MATCH_DONE = 0008h
 4 CONNECT_IGNORE_AAC_BEACON = 0010h
 5 CONNECT_CSA_FOLLOW_BSS = 0020h
 6 CONNECT_PYXIS_REMOTE = 0040h ;-old code from 2008
 6 CONNECT_DO_WPA_OFFLOAD = 0040h ;\
 7 CONNECT_DO_NOT_DEAUTH = 0080h ; new code from 2010
 8 CONNECT_WPS_FLAG = 0100h ;
 9 CONNECT_IGNORE_BSSID_HINT = 0200h ;
 16 AP_NO_DISASSOC_UPON_DEAUTH = 10000h ;/ <--AP configuration flags |
| --- |

DEFAULT_CONNECT_CTRL_FLAGS = (CONNECT_CSA_FOLLOW_BSS)

**WMIcmd(0002h) - WMI_RECONNECT_CMD**

Parameters (optional...?... 08h bytes):

```

| 00h A_UINT16 2 channel ;hint
 02h A_UINT8 6 bssid[ATH_MAC_LEN] ;mandatory if set |
| --- |

**WMIcmd(0003h) - WMI_DISCONNECT_CMD**

Parameters:

```

| Unknown (none?) |
| --- |

Returns WMI_DISCONNECT_EVENT. Uhm, or seems to crash on AR6013...?

**WMIcmd(000Dh) - WMI_SET_DISC_TIMEOUT_CMD ;aka WMI_SET_DISCONNECT_TIMEOUT**

Parameters (01h bytes):

```

| 00h A_UINT8 1 disconnectTimeout ;seconds |
| --- |

**WMIevent(1002h) - WMI_CONNECT_EVENT**

First 10h bytes of response can be "infra_ibss_bss" or "ap_sta" or "ap_bss"
(unclear which selects/indicates which it is, maybe the parameters from
preceeding WMI_CONNECT_CMD, or from WMI_BSSINFO_EVENT's frame body?)

Upon error, DISCONNECT_EVENT is sent instead of WMI_CONNECT_EVENT (eg. when
using WMI_CONNECT with wrong WEP password).

Event Data:

```

| When "infra_ibss_bss": ;<-- occurs for my WPA2 connect
 00h A_UINT16 2 channel ;in MHz
 02h A_UINT8 6 bssid[ATH_MAC_LEN] ;
 08h A_UINT16 2 listenInterval ;0064h
 0Ah A_UINT16 2 beaconInterval ;0064h
 0Ch A_UINT32 4 networkType ;00000001h
 When "ap_sta":
 00h A_UINT8 1 phymode
 01h A_UINT8 1 aid
 02h A_UINT8 6 mac_addr[ATH_MAC_LEN]
 08h A_UINT8 1 auth
 09h A_UINT8 1 keymgmt
 0Ah A_UINT16 2 cipher
 0Ch A_UINT8 1 apsd_info
 0Dh A_UINT8 3 unused[3]
 When "ap_bss":
 00h A_UINT16 2 channel
 02h A_UINT8 6 bssid[ATH_MAC_LEN]
 08h A_UINT8 8 unused[8]
 And, in all three cases:
 10h A_UINT8 1 beaconIeLen ;16h
 11h A_UINT8 1 assocReqLen ;2Fh
 12h A_UINT8 1 assocRespLen ;16h
 13h A_UINT8 .. assocInfo[1] ;whatever 100 bytes? |
| --- |

In STA mode networkType comes along with connected phy mode.

To get networkType, WMI_NETWORK_TYPE (networkType).

To get connected phymode, WMI_CONNECTED_PHYMODE(networkType) will give the
phymode value.

**WMIevent(1003h) - WMI_DISCONNECT_EVENT**

This event can occur when using WMI_CONNECT with wrong WEP password (instead of
WMI_CONNECT_EVENT), or when failing to respond to WPA/WPA2 handshake messages,
or when using WMI_DISCONNECT.

Event Data (0Ah+N bytes):

```

| 00h A_UINT16 2 protocolReasonStatus ;reason code, see 802.11 spec.
 02h A_UINT8 6 bssid[ATH_MAC_LEN] ;set if known
 08h A_UINT8 1 disconnectReason ;see WMI_DISCONNECT_REASON
 09h A_UINT8 1 assocRespLen (00h=none)
 0Ah A_UINT8 N*1 assocInfo[1] |
| --- |

WMI_DISCONNECT_REASON values:

```

| NO_NETWORK_AVAIL = 01h ;-occurs on wrong WEP key
 LOST_LINK = 02h ;-bmiss
 DISCONNECT_CMD = 03h ;-requested via disconnect command
 BSS_DISCONNECTED = 04h ;-occurs after some minutes of inactivity?
 AUTH_FAILED = 05h ;-reportedly occurs if AP was already connected?
 ASSOC_FAILED = 06h
 NO_RESOURCES_AVAIL = 07h
 CSERV_DISCONNECT = 08h
 INVALID_PROFILE = 0Ah
 DOT11H_CHANNEL_SWITCH = 0Bh
 PROFILE_MISMATCH = 0Ch
 PYXIS_VIRT_ADHOC_DISC = 0Dh ;-old code from 2008
 CONNECTION_EVICTED = 0Dh ;\
 IBSS_MERGE = 0Eh ; new code from 2010
 EXCESS_TX_RETRY = 0Fh ;/ <--TX frames failed after excessive retries |
| --- |

| DSi Atheros Wifi - WMI Channel and Cipher Functions |
| --- |

**WMIcmd(000Eh) - WMI_GET_CHANNEL_LIST_CMD ;reply 000Eh ;aka WMI_CHANNEL_LIST**

Parameters:

```

| None |
| --- |

WMI_GET_CHANNEL_LIST_CMD reply ;aka WMI_CHANNEL_LIST_REPLY

```

| 00h A_UINT8 1 reserved1 ;whatever (zero)
 01h A_UINT8 1 numChannels ;number of channels in reply (N)
 02h A_UINT16 N*2 channelList[1] ;channels in MHz
 02h+N*2 2 Zero (0000h) ;undocumented (end of list or so) |
| --- |

Reply size is 1Ah bytes on DSi (2+(11*2)+2 bytes, for 11 channels, plus ending
0000h; that ending halfward isn't officially mentioned to exist).

**WMIcmd(0011h) - WMI_SET_CHANNEL_PARAMS_CMD ;aka WMI_CHANNEL_PARAMS_CMD**

Parameters (04h+N*2 bytes):

```

| 00h A_UINT8 1 reserved1 ;whatever (?)
 01h A_UINT8 1 scanParam ;set if enable scan
 02h A_UINT8 1 phyMode ;see WMI_PHY_MODE
 03h A_UINT8 1 numChannels ;how many channels follow
 04h A_UINT16 N*2 channelList[1] ;channels in MHz |
| --- |

WMI_PHY_MODE values:

```

| WMI_11A_MODE = 01h
 WMI_11G_MODE = 02h
 WMI_11AG_MODE = 03h
 WMI_11B_MODE = 04h
 WMI_11GONLY_MODE = 05h |
| --- |

#define WMI_MAX_CHANNELS = 32

Some (or all?) DSi's have only channel 1-11 enabled, and throw
WMI_CMDERROR_EVENT when trying to set channel 12,13,14. Unknown what defines
which channels are enabled (probably something in EEPROM).

The enabled channels can be obtained via WMI_GET_CHANNEL_LIST_CMD (which is
probably related to the country reported in WMI_REGDOMAIN_EVENT).

**WMIevent(101Ah) - WMI_CHANNEL_CHANGE_EVENT ;<-- used on DSi ?**

Event Data (06h bytes):

```

| 00h A_UINT16 2 oldChannel; ;\uh, old is 16bit, new is misaligned 32bit?
 02h A_UINT32 4 newChannel; ;/(DSi does really send 6 bytes) |
| --- |

This event is not defined in "AR6kSDK.build_sw.18", however, "101Ah" is USED on
DSi.

**WMIcmd(0016h) - WMI_ADD_CIPHER_KEY_CMD**

Parameters (2Dh bytes on DSi?, but other sources claim 2Ch or 33h bytes):

```

| 2Ch 2Dh 33h <---- total size (on DSi it's 2Dh, ie. middle column)
 00h 00h 00h A_UINT8 1 keyIndex ;aka WMI_MAX_KEY_INDEX ?
 01h 01h 01h A_UINT8 1 keyType ;aka CRYPTO_TYPE
 02h 02h 02h A_UINT8 1 keyUsage ;KEY_USAGE
 03h 03h 03h A_UINT8 1 keyLength
 04h 04h 04h A_UINT8 8 keyRSC[8] ;key replay sequence counter
 0Ch 0Ch 0Ch A_UINT8 32 key[WMI_MAX_KEY_LEN] ;aka password
 -- 2Ch 2Ch A_UINT8 1 key_op_ctrl ;Additional Key Control information
 -- -- 2Dh A_UINT8 6 key_macaddr[ATH_MAC_LEN] |
| --- |

KEY_USAGE values:

```

| PAIRWISE_USAGE = 00h ;<-- DSi browser uses THIS for WPA/WPA2 key 0
 GROUP_USAGE = 01h ;<-- DSi browser uses THIS for WEP/WPA/WPA2 key 1..3
 TX_USAGE = 02h ;<-- reportedly "default Tx Key - Static WEP only"
 Undoc (or 01h+02h) = 03h ;<-- DSi browser uses THIS for WEP key 0 |
| --- |

Bit Flag. (aka key_op_ctrl values?):

```

| Bit 0 - Initialise TSC - default is Initialize
 KEY_OP_INIT_TSC = 01h
 KEY_OP_INIT_RSC = 02h
 KEY_OP_INIT_WAPIPN = 10h (only if "WAPI_ENABLE")
 KEY_OP_INIT_VAL = 03h ;<-- Default Initialise the TSC & RSC ;used by DSi
 KEY_OP_VALID_MASK = 03h |
| --- |

More constants:

```

| WMI_MIN_KEY_INDEX = 0
 WMI_MAX_KEY_INDEX = 3 ;<-- when not "WAPI_ENABLE"
 WMI_MAX_KEY_INDEX = 7 ;<-- when "WAPI_ENABLE" (wapi grpKey 0-3, prwKey 4-7)
 WMI_MAX_KEY_LEN = 32 |
| --- |

**WEP Cipher Keys**

All four keys (with KeyIndex=0..3) must be set before WMI_CONNECT_CMD. The
NDS/DSi user interface allows to define only one WEP key 0, but one must add
dummy keys for key 1..3, else connect will fail.

**WPA/WPA2 Cipher Keys**

For WPA/WPA2, WMI_CONNECT_CMD isn't working fully automated: After
WMI_CONNECT_EVENT, one must manually receive & reply EAPOL handshake
messages, and then add the cipher keys based on the message contents. The used
keys are key 0 (for pairwise cipher), and key 1 or 2 (for the current group
cipher key).

DS Wifi WPA/WPA2 Handshake Messages (EAPOL)

DS Wifi WPA/WPA2 Keys and MICs

DS Wifi WPA/WPA2 Encryption

DS Firmware Wifi Calibration Data

DS Firmware Wifi Internet Access Points

Moreover, the access point may throw further EAPOL messages for assigning new
group keys every once and then, and one must also manually apply that new key
via ADD_CIPHER (the group key index switches between 1 and 2 on each update).

```

| KeyIndex=0, key=PTK[20h..2Fh]+PTK[38h..3Fh]+PTK[30h..37h], RSC=0
 KeyIndex=1/2, key=GTK[00h..0Fh]+PTK[18h..1Fh]+PTK[10h..17h], RSC=EAPOL RSC |
| --- |

**WAPI Cipher Keys**

Some atheros firmwares have "WAPI_ENABLE" with eight key indices (grpKey=0..3,
prwKey=4..7). The DSi firmware probably doesn't have that WAPI stuff.

**WMIcmd(0017h) - WMI_DELETE_CIPHER_KEY_CMD ;ignored dummy command on DSi**

Parameters (01h bytes):

```

| 00h A_UINT8 1 keyIndex |
| --- |

**WMIcmd(0018h) - WMI_ADD_KRK_CMD ;ignored dummy command on DSi**

Parameters (10h bytes):

```

| 00h A_UINT8 16 krk[WMI_KRK_LEN] |
| --- |

#define WMI_KRK_LEN = 16

KRK maybe means "Key Registration with Knowledge"?

**WMIcmd(0019h) - WMI_DELETE_KRK_CMD ;ignored dummy command on DSi**

Parameters:

```

| Unknown (none?) (or maybe same as for ADD_KRK ?) (seems to be NONE on DSi) |
| --- |

**WMIcmd(0020h) - WMI_SET_TKIP_COUNTERMEASURES_CMD**

Parameters (01h bytes):

```

| 00h A_UINT8 1 cm_en ;WMI_TKIP_CM_CONTROL |
| --- |

WMI_TKIP_CM_CONTROL values:

```

| WMI_TKIP_CM_DISABLE = 00h
 WMI_TKIP_CM_ENABLE = 01h |
| --- |

Note: There are also "CM_CONNECT_TYPE" values in "cnxmgmt.h", is THAT related?

**WMIevent(1009h) - WMI_TKIP_MICERR_EVENT**

Event Data (02h bytes):

```

| 00h A_UINT8 1 keyid
 01h A_UINT8 1 ismcast |
| --- |

TKIP MIC Error Event

| DSi Atheros Wifi - WMI Scan Functions |
| --- |

**WMIcmd(0007h) - WMI_START_SCAN_CMD**

Parameters (theoretically 12h+N*2 bytes, but 14h bytes when N=0 ?):

```

| 00h A_BOOL 4 forceFgScan
 04h A_BOOL 4 isLegacy For Legacy Cisco AP compatibility
 08h A_UINT32 4 homeDwellTime Max duration in the home channel (msec)
 0Ch A_UINT32 4 forceScanInterval Time interval between scans (msec)
 10h A_UINT8 1 scanType WMI_SCAN_TYPE
 11h A_UINT8 1 numChannels how many channels follow
 12h A_UINT16 N*2 channelList[1] channels in MHz |
| --- |

WMI_SCAN_TYPE values:

```

| WMI_LONG_SCAN = 0
 WMI_SHORT_SCAN = 1 |
| --- |

Old code from 2008 did (additionally) use value 0 and 1 as so:

```

| WMI_PYXIS_PAS_DSCVR = 0
 WMI_PYXIS_ACT_DSCVR = 1 |
| --- |

The DSi Browser receives one or more WMI_BSSINFO_EVENT's (if there are any
beacons on the current channel), and, once when the DwellTime (mul5?) has
ellapsed, finally receives WMI_SCAN_COMPLETE_EVENT.

**WMIcmd(0008h) - WMI_SET_SCAN_PARAMS_CMD**

Parameters (14h bytes):

```

| 00h A_UINT16 2 fg_start_period ;seconds
 02h A_UINT16 2 fg_end_period ;seconds
 04h A_UINT16 2 bg_period ;seconds
 06h A_UINT16 2 maxact_chdwell_time ;msec
 08h A_UINT16 2 pas_chdwell_time ;msec
 0Ah A_UINT8 1 shortScanRatio ;how many shorts scan for one long
 0Bh A_UINT8 1 scanCtrlFlags
 0Ch A_UINT16 2 minact_chdwell_time ;msec
 0Eh A_UINT16 2 maxact_scan_per_ssid ;max active scans per ssid
 10h A_UINT32 4 max_dfsch_act_time ;msec |
| --- |

#define WMI_SHORTSCANRATIO_DEFAULT = 3

Warning: ScanCtrlFlag value of FFh is used to disable all flags in
WMI_SCAN_PARAMS_CMD

Do not add any more flags to WMI_SCAN_CTRL_FLAG_BITS

WMI_SCAN_CTRL_FLAGS_BITS:

```

| CONNECT_SCAN_CTRL_FLAGS = 01h ;set if can scan in the Connect cmd
 SCAN_CONNECTED_CTRL_FLAGS = 02h ;set if scan for the SSID it is
 ; already connected to
 ACTIVE_SCAN_CTRL_FLAGS = 04h ;set if enable active scan
 ROAM_SCAN_CTRL_FLAGS = 08h ;set if enable roam scan when bmiss
 ; and lowrssi
 REPORT_BSSINFO_CTRL_FLAGS = 10h ;set if follows customer BSSINFO
 ; reporting rule
 ENABLE_AUTO_CTRL_FLAGS = 20h ;if disabled, target doesn't
 ; scan after a disconnect event
 ENABLE_SCAN_ABORT_EVENT = 40h ;Scan complete event with canceled status
 ; will be generated when a scan is
 ; prempted before it gets completed
 #define CAN_SCAN_IN_CONNECT(flags) (flags & CONNECT_SCAN_CTRL_FLAGS)
 #define CAN_SCAN_CONNECTED(flags) (flags & SCAN_CONNECTED_CTRL_FLAGS)
 #define ENABLE_ACTIVE_SCAN(flags) (flags & ACTIVE_SCAN_CTRL_FLAGS)
 #define ENABLE_ROAM_SCAN(flags) (flags & ROAM_SCAN_CTRL_FLAGS)
 #define CONFIG_REPORT_BSSINFO(flags) (flags & REPORT_BSSINFO_CTRL_FLAGS)
 #define IS_AUTO_SCAN_ENABLED(flags) (flags & ENABLE_AUTO_CTRL_FLAGS)
 #define SCAN_ABORT_EVENT_ENABLED(flags) (flags & ENABLE_SCAN_ABORT_EVENT) |
| --- |

#define DEFAULT_SCAN_CTRL_FLAGS = (CONNECT_SCAN_CTRL_FLAGS |
SCAN_CONNECTED_CTRL_FLAGS | ACTIVE_SCAN_CTRL_FLAGS | ROAM_SCAN_CTRL_FLAGS |
ENABLE_AUTO_CTRL_FLAGS)

**WMIevent(100Ah) - WMI_SCAN_COMPLETE_EVENT**

Event Data (old: None, or new: status, 04h bytes):

```

| 00h A_INT32 4 status; ;aka "staus" |
| --- |

(whatever, usually/always zero)

(can be 10h... when already connected maybe?)

Note: There are several "SCAN_xxx" and "xxx_SCPRI" values defined in
"discovery.h" - purpose is unknown (maybe that stuff is used only internally).

| DSi Atheros Wifi - WMI Bit Rate Functions |
| --- |

**WMIcmd(0034h) - WMI_SET_FIXRATES_CMD ;aka WMI_FIX_RATES_CMD**

Parameters (02h bytes on DSi?, but other sources claim 08h bytes):

```

| 02h 08h <---- total size (on DSi it's 02h, ie. left column)
 00h -- A_UINT16 2 fixRateMask ;0..0FFFh ;see WMI_BIT_RATE
 -- 00h A_UINT32 4 fixRateMask(0) ;0..0FFFFFFFh ;see WMI_BIT_RATE
 -- 04h A_UINT32 4 fixRateMask(1) ;0..0FFFFFFFh ;see WMI_BIT_RATE |
| --- |

**WMIcmd(0035h) - WMI_GET_FIXRATES_CMD ;reply 0035h**

Parameters:

```

| Unknown (none?) |
| --- |

Reply: WMI_GET_FIXRATES aka WMI_FIX_RATES_REPLY

Event Data (02h bytes on DSi?, but other sources claim 08h bytes):

```

| 02h 08h <---- total size (on DSi it's 02h, ie. left column)
 00h -- A_UINT16 2 fixRateMask ;0..0FFFh ;see WMI_BIT_RATE
 -- 00h A_UINT32 4 fixRateMask(0) ;0..0FFFFFFFh ;see WMI_BIT_RATE
 -- 04h A_UINT32 4 fixRateMask(1) ;0..0FFFFFFFh ;see WMI_BIT_RATE |
| --- |

See "FIX_RATE_xxx" values.

**WMIcmd(0048h or 0047h) - WMI_SET_FRAMERATES_CMD ;aka WMI_FRAME_RATES_CMD**

DSi uses command number 0048h for this (whilst most or all official Atheros
code is using number 0047h).

Parameters (04h bytes, or 0Ch bytes on hardware with more rates):

```

| 04h 0Ch <---- total size (on DSi it's 04h, ie. left column)
 00h 00h A_UINT8 1 bEnableMask (used: 01h) ;whatever?
 01h 01h A_UINT8 1 frameType (D4h=special?) (used: A4h) ;type and subtype
 02h -- A_UINT16 2 frameRateMask ;0..0FFFh (used: FFF7h) ;see WMI_BIT_RATE
 -- 02h A_UINT8 2 reserved[2] ;for alignment
 -- 04h A_UINT32 4 frameRateMask(0) ;0..0FFFFFFFh ;see WMI_BIT_RATE
 -- 08h A_UINT32 4 frameRateMask(1) ;0..0FFFFFFFh ;see WMI_BIT_RATE |
| --- |

Note: FRAMERATE is apparently meant to be "bitrate" for certain "packet types"
(frames). The type and subtype values are maybe meant to resemble those in
Frame Control (FC) field, or the FTYPE_xxx constants in some atheros source
code?

Reportedly there is a "WMI_FRAME_RATES_REPLY" with same Reply structure as
above Parameter structure, the DSi doesn't support that feature (and it's
rather unknown if/when/how that Reply would be sent if the feature were
implemented... maybe as event, or as reply to SET_FRAMERATES, or as reply to
whatever other cmd).

**WMIcmd(F000h) - WMI_SET_BITRATE_CMD ;aka WMI_BIT_RATE_CMD**

Parameters (03h bytes):

```

| 00h A_INT8 1 rateIndex (FFh=Auto) ;see WMI_BIT_RATE
 01h A_INT8 1 mgmtRateIndex (00h=1Mbit/s)
 02h A_INT8 1 ctlRateIndex (00h=1Mbit/s) |
| --- |

Note: This doesn't really work. Using rateIndex=FFh (or 0Bh) does currently
reach only max 600Kbyte/s upload speed (which MIGHT be a bug in my software).
However, using rateIndex=00h is slower, but still reaches more than 200Kbyte/s
(which is outright wrong and impossible at 1Mbit/s).

**WMIcmd(F001h) - WMI_GET_BITRATE_CMD ;reply F001h**

Parameters:

```

| Unknown (none?) |
| --- |

Reply:

```

| 00h A_INT8 1 rateIndex ;see WMI_BIT_RATE |
| --- |

**WMI_BIT_RATE values**

```

| Mbit/s = Index Mbit/s = Mask
 RATE_AUTO = -1 - = -
 RATE_1Mb = 0 FIX_RATE_1Mb = 1h
 RATE_2Mb = 1 FIX_RATE_2Mb = 2h
 RATE_5_5Mb = 2 FIX_RATE_5_5Mb = 4h
 RATE_11Mb = 3 FIX_RATE_11Mb = 8h
 RATE_6Mb = 4 FIX_RATE_6Mb = 10h
 RATE_9Mb = 5 FIX_RATE_9Mb = 20h
 RATE_12Mb = 6 FIX_RATE_12Mb = 40h
 RATE_18Mb = 7 FIX_RATE_18Mb = 80h
 RATE_24Mb = 8 FIX_RATE_24Mb = 100h
 RATE_36Mb = 9 FIX_RATE_36Mb = 200h
 RATE_48Mb = 10 FIX_RATE_48Mb = 400h
 RATE_54Mb = 11 FIX_RATE_54Mb = 800h |
| --- |

Below only for newer (non-DSi) revisions (with 32bit RATE_MASK values):

```

| RATE_MCS_0_20 = 12 FIX_RATE_MCS_0_20 = 1000h
 RATE_MCS_1_20 = 13 FIX_RATE_MCS_1_20 = 2000h
 RATE_MCS_2_20 = 14 FIX_RATE_MCS_2_20 = 4000h
 RATE_MCS_3_20 = 15 FIX_RATE_MCS_3_20 = 8000h
 RATE_MCS_4_20 = 16 FIX_RATE_MCS_4_20 = 10000h
 RATE_MCS_5_20 = 17 FIX_RATE_MCS_5_20 = 20000h
 RATE_MCS_6_20 = 18 FIX_RATE_MCS_6_20 = 40000h
 RATE_MCS_7_20 = 19 FIX_RATE_MCS_7_20 = 80000h
 RATE_MCS_0_40 = 20 FIX_RATE_MCS_0_40 = 100000h
 RATE_MCS_1_40 = 21 FIX_RATE_MCS_1_40 = 200000h
 RATE_MCS_2_40 = 22 FIX_RATE_MCS_2_40 = 400000h
 RATE_MCS_3_40 = 23 FIX_RATE_MCS_3_40 = 800000h
 RATE_MCS_4_40 = 24 FIX_RATE_MCS_4_40 = 1000000h
 RATE_MCS_5_40 = 25 FIX_RATE_MCS_5_40 = 2000000h
 RATE_MCS_6_40 = 26 FIX_RATE_MCS_6_40 = 4000000h
 RATE_MCS_7_40 = 27 FIX_RATE_MCS_7_40 = 8000000h |
| --- |

Get bit rate cmd uses same definition as set bit rate cmd

| DSi Atheros Wifi - WMI Threshold Functions |
| --- |

Below commands allow to define thresholds for RSSI, SNR, LQ.

```

| RSSI Received Signal Strength Indicator
 SNR Signal to Noise Ratio
 LQ Link Quality |
| --- |

**WMIcmd(0021h) - WMI_RSSI_THRESHOLD_PARAMS_CMD**

Parameters (20h bytes):

```

| 00h A_UINT32 4 pollTime ;Polling time as a factor of LI
 04h A_INT16 2 thresholdAbove1_Val ;lowest of upper
 06h A_INT16 2 thresholdAbove2_Val
 08h A_INT16 2 thresholdAbove3_Val
 0Ah A_INT16 2 thresholdAbove4_Val
 0Ch A_INT16 2 thresholdAbove5_Val
 0Eh A_INT16 2 thresholdAbove6_Val ;highest of upper
 10h A_INT16 2 thresholdBelow1_Val ;lowest of bellow
 12h A_INT16 2 thresholdBelow2_Val
 14h A_INT16 2 thresholdBelow3_Val
 16h A_INT16 2 thresholdBelow4_Val
 18h A_INT16 2 thresholdBelow5_Val
 1Ah A_INT16 2 thresholdBelow6_Val ;highest of bellow
 1Ch A_UINT8 1 weight ;"alpha"
 1Dh A_UINT8 3 reserved[3]
 Setting the polltime to 0 would disable polling.
 Threshold values are in the ascending order, and should agree to:
 (lowThreshold_lowerVal < lowThreshold_upperVal < highThreshold_lowerVal
 < highThreshold_upperVal) |
| --- |

See also: WMI_RSSI_THRESHOLD_EVENT

**WMIcmd(002Fh) - WMI_SNR_THRESHOLD_PARAMS_CMD**

Parameters (10h bytes):

```

| 00h A_UINT32 4 pollTime ;Polling time as a factor of LI
 04h A_UINT8 1 weight ;"alpha"
 05h A_UINT8 1 thresholdAbove1_Val ;lowest of uppper ;uh, ppper?
 06h A_UINT8 1 thresholdAbove2_Val
 07h A_UINT8 1 thresholdAbove3_Val
 08h A_UINT8 1 thresholdAbove4_Val ;highest of upper
 09h A_UINT8 1 thresholdBelow1_Val ;lowest of bellow ;uh bell?
 0Ah A_UINT8 1 thresholdBelow2_Val
 0Bh A_UINT8 1 thresholdBelow3_Val
 0Ch A_UINT8 1 thresholdBelow4_Val ;highest of bellow ;uh bell?
 0Dh A_UINT8 3 reserved[3] |
| --- |

Setting the polltime to 0 would disable polling.

See also: WMI_SNR_THRESHOLD_EVENT

**WMIcmd(0030h) - WMI_LQ_THRESHOLD_PARAMS_CMD**

Parameters (0Ch bytes):

```

| 00h A_UINT8 1 enable ;<-- enable (unlike SNR command)
 01h A_UINT8 1 thresholdAbove1_Val ;\these parameters seem to be same as
 02h A_UINT8 1 thresholdAbove2_Val ; for WMI_SNR_THRESHOLD_PARAMS_CMD
 03h A_UINT8 1 thresholdAbove3_Val ;
 04h A_UINT8 1 thresholdAbove4_Val ;
 05h A_UINT8 1 thresholdBelow1_Val ;
 06h A_UINT8 1 thresholdBelow2_Val ;
 07h A_UINT8 1 thresholdBelow3_Val ;
 08h A_UINT8 1 thresholdBelow4_Val ;
 09h A_UINT8 3 reserved[3] ;/ |
| --- |

See also: WMI_LQ_THRESHOLD_EVENT

**WMIcmd(0033h) - WMI_CLR_RSSI_SNR_CMD**

Parameters:

```

| Unknown (none?) |
| --- |

Probably somehow related to RSSI_THRESHOLD and SNR_THRESHOLD.

**WMIevent(100Ch) - WMI_RSSI_THRESHOLD_EVENT**

Event Data (03h bytes):

```

| 00h A_INT16 2 rssi
 02h A_UINT8 1 range |
| --- |

WMI_RSSI_THRESHOLD_VAL values (which are probably meant to occurr in one of
above fields, unclear which one though):

```

| WMI_RSSI_THRESHOLD1_ABOVE = 0
 WMI_RSSI_THRESHOLD2_ABOVE = 1
 WMI_RSSI_THRESHOLD3_ABOVE = 2
 WMI_RSSI_THRESHOLD4_ABOVE = 3
 WMI_RSSI_THRESHOLD5_ABOVE = 4
 WMI_RSSI_THRESHOLD6_ABOVE = 5
 WMI_RSSI_THRESHOLD1_BELOW = 6
 WMI_RSSI_THRESHOLD2_BELOW = 7
 WMI_RSSI_THRESHOLD3_BELOW = 8
 WMI_RSSI_THRESHOLD4_BELOW = 9
 WMI_RSSI_THRESHOLD5_BELOW = 10
 WMI_RSSI_THRESHOLD6_BELOW = 11 |
| --- |

Indicate the RSSI events to host. Events are indicated when we breach a
thresold value. (uh, how old do we breach?)

**WMIevent(1012h) - WMI_SNR_THRESHOLD_EVENT**

Event Data (02h bytes):

```

| 00h A_UINT8 1 range ;WMI_SNR_THRESHOLD_VAL
 01h A_UINT8 1 snr |
| --- |

WMI_SNR_THRESHOLD_VAL values:

```

| WMI_SNR_THRESHOLD1_ABOVE = 1
 WMI_SNR_THRESHOLD1_BELOW = 2
 WMI_SNR_THRESHOLD2_ABOVE = 3
 WMI_SNR_THRESHOLD2_BELOW = 4
 WMI_SNR_THRESHOLD3_ABOVE = 5
 WMI_SNR_THRESHOLD3_BELOW = 6
 WMI_SNR_THRESHOLD4_ABOVE = 7
 WMI_SNR_THRESHOLD4_BELOW = 8 |
| --- |

**WMIevent(1013h) - WMI_LQ_THRESHOLD_EVENT**

Event Data (05h bytes):

```

| 00h A_INT32 4 lq
 04h A_UINT8 1 range ;WMI_LQ_THRESHOLD_VAL |
| --- |

WMI_LQ_THRESHOLD_VAL values:

```

| WMI_LQ_THRESHOLD1_ABOVE = 1
 WMI_LQ_THRESHOLD1_BELOW = 2
 WMI_LQ_THRESHOLD2_ABOVE = 3
 WMI_LQ_THRESHOLD2_BELOW = 4
 WMI_LQ_THRESHOLD3_ABOVE = 5
 WMI_LQ_THRESHOLD3_BELOW = 6
 WMI_LQ_THRESHOLD4_ABOVE = 7
 WMI_LQ_THRESHOLD4_BELOW = 8 |
| --- |

| DSi Atheros Wifi - WMI Error, Retry and Debug Functions |
| --- |

**WMIcmd(0024h) - WMI_SET_RETRY_LIMITS_CMD**

Parameters:

```

| 00h A_UINT8 1 frameType ;WMI_FRAMETYPE
 01h A_UINT8 1 trafficClass ;applies only to DATA_FRAMETYPE
 02h A_UINT8 1 maxRetries
 03h A_UINT8 1 enableNotify |
| --- |

This command is used to customize the number of retries the wlan device will
perform on a given frame.

```

| WMI_MIN_RETRIES = 2
 WMI_MAX_RETRIES = 13 |
| --- |

WMI_FRAMETYPE values:

```

| MGMT_FRAMETYPE = 0
 CONTROL_FRAMETYPE = 1
 DATA_FRAMETYPE = 2 |
| --- |

**WMIcmd(0022h) - WMI_TARGET_ERROR_REPORT_BITMASK_CMD**

Parameters (04h bytes):

```

| 00h A_UINT32 4 bitmask ;... probably "WMI_TARGET_ERROR_VAL" ? |
| --- |

Sets the error reporting event bitmask in target. Target clears it upon an
error. Subsequent errors are counted, but not reported via event, unless the
bitmask is set again.

See also: WMI_TARGET_ERROR_REPORT_EVENT

**WMIevent(1005h) - WMI_CMDERROR_EVENT ;aka WMI_CMD_ERROR_EVENT**

Event Data (03h bytes):

```

| 00h A_UINT16 2 commandId ;on DSi, this can be: 0001h,0011h,0016h,0026h
 02h A_UINT8 1 errorCode ;on DSi, this can be: 01h,02h |
| --- |

WMI_ERROR_CODE values:

```

| INVALID_PARAM = 01h
 ILLEGAL_STATE = 02h
 INTERNAL_ERROR = 03h
 DFS_CHANNEL = 04h |
| --- |

Command Error Event

**WMIevent(100Dh) - WMI_ERROR_REPORT_EVENT ;aka WMI_TARGET_ERROR_REPORT_EVENT**

Event Data (04h bytes):

```

| 00h A_UINT32 4 errorVal |
| --- |

WMI_TARGET_ERROR_VAL values:

```

| WMI_TARGET_PM_ERR_FAIL = 00000001h
 WMI_TARGET_KEY_NOT_FOUND = 00000002h
 WMI_TARGET_DECRYPTION_ERR = 00000004h
 WMI_TARGET_BMISS = 00000008h
 WMI_PSDISABLE_NODE_JOIN = 00000010h
 WMI_TARGET_COM_ERR = 00000020h
 WMI_TARGET_FATAL_ERR = 00000040h
 WMI_TARGET_BCN_FOUND = 00000080h |
| --- |

**WMIevent(1014h) - WMI_TX_RETRY_ERR_EVENT**

Event Data (01h bytes):

```

| 00h A_UINT8 1 retrys |
| --- |

**WMIcmd(002Eh:2009h) - WMIX_DBGLOG_CFG_MODULE_CMD**

Parameters (08h bytes):

```

| 00h A_UINT32 4 cfgvalid ;change mask (0=no change, 1=change)
 04h A_UINT32 4 dbglog_config ;new state (0=clear, 1=set) |
| --- |

The cfgvalid mask and dbglog_config bits are:

```

| Bit0-15 DBGLOG_MODULE_LOG_ENABLE ;logging enable flags for module 0-15
 Bit16 DBGLOG_REPORTING_ENABLED ;reporting enable flag (1=enable)
 Bit17-19 DBGLOG_TIMESTAMP_RESOLUTION ;timestamp resolution (default=1 ms)
 Bit20-29 DBGLOG_REPORT_SIZE ;report size in number of messages
 Bit30-31 Reserved ;reserved |
| --- |

Using parameters FFFFFFFFh,00000000h or 0001FFFFh,00000000h does disable
WMIX_DBGLOG_EVENT.

**WMIevent(1010h:3008h) - WMIX_DBGLOG_EVENT ;used (probably related to 2009h)**

Event Data:

```

| Unknown (?) (related/enabled by WMIX_DBGLOG_CFG_MODULE_CMD) |
| --- |

DSi Browser does receive this - with LOTs of data (almost 1.5Kbyte)

Reportedly there is a dbglog message value (with numeric "message" IDs, perhaps
related to dbglog EVENT response headers):

```

| Bit0-15 DBGLOG_TIMESTAMP ;contains bit8-23 of the LF0 timer (0..FFFFh)
 Bit16-25 DBGLOG_DBGID ;minor ID (defined in "dbglog_id.h")
 Bit26-29 DBGLOG_MODULEID ;major ID (defined in "dbglog.h")
 Bit30-31 DBGLOG_NUM_ARGS ;whatever "num args" |
| --- |

| DSi Atheros Wifi - WMI Priority Stream Functions |
| --- |

**WMIcmd(0005h) - WMI_CREATE_PSTREAM_CMD ;aka WMI_CRE_PRIORITY_STREAM**

Parameters (3Fh bytes in DSi, but other variants exist, too):

```

| old 3Fh 40h <---- total size (on DSi it's 3Fh, ie. middle column)
 0Bh 00h 00h A_UINT32 4 minServiceInt ;in msec (14h=20)
 0Fh 04h 04h A_UINT32 4 maxServiceInt ;in msec (14h=20)
 13h 08h 08h A_UINT32 4 inactivityInt ;in msec (98967Fh=9999999)
 17h 0Ch 0Ch A_UINT32 4 suspensionInt ;in msec (-1)
 1Bh 10h 10h A_UINT32 4 serviceStartTime (0)
 1Fh 14h 14h A_UINT32 4 minDataRate ;in bps (14500h=83200)
 23h 18h 18h A_UINT32 4 meanDataRate ;in bps (14500h=83200)
 27h 1Ch 1Ch A_UINT32 4 peakDataRate ;in bps (14500h=83200)
 2Bh 20h 20h A_UINT32 4 maxBurstSize (0)
 2Fh 24h 24h A_UINT32 4 delayBound (0)
 33h 28h 28h A_UINT32 4 minPhyRate ;in bps (5B8D80h=6000000)
 37h 2Ch 2Ch A_UINT32 4 sba (2000h=8192)
 3Bh 30h 30h A_UINT32 4 mediumTime (0)
 07h 34h 34h A_UINT16 2 nominalMSDU ;in octeCts (80D0h=?)
 09h 36h 36h A_UINT16 2 maxMSDU ;in octeCts (00D0h=?)
 00h 38h 38h A_UINT8 1 trafficClass (00h)
 01h 39h 39h A_UINT8 1 trafficDirection ;DIR_TYPE (02h=Bidir)
 02h 3Ah 3Ah A_UINT8 1 rxQueueNum (FFh)
 03h 3Bh 3Bh A_UINT8 1 trafficType ;TRAFFIC_TYPE (01h=Periodic)
 04h 3Ch 3Ch A_UINT8 1 voicePSCapability ;VOICEPS_CAP_TYPE (00h)
 05h 3Dh 3Dh A_UINT8 1 tsid (05h)
 06h 3Eh 3Eh A_UINT8 1 userPriority ;802.1D user priority (00h)
 - - 3Fh A_UINT8 1 nominalPHY ;nominal phy rate |
| --- |

Old wmi.h source code from 2006 used 8bit/16bit/32bit parameter ordering, later
source code reversed that to 32bit/16bit/8bit and added an extra "nominalPHY"
entry. DSi uses the new ordering, but without the extra entry.

DIR_TYPE values:

```

| UPLINK_TRAFFIC = 0
 DNLINK_TRAFFIC = 1
 BIDIR_TRAFFIC = 2 |
| --- |

TRAFFIC_TYPE values:

```

| TRAFFIC_TYPE_APERIODIC = 0
 TRAFFIC_TYPE_PERIODIC = 1 |
| --- |

VOICEPS_CAP_TYPE values:

```

| DISABLE_FOR_THIS_AC = 0
 ENABLE_FOR_THIS_AC = 1
 ENABLE_FOR_ALL_AC = 2 |
| --- |

XXX see WMI_CRE_PRIORITY_STREAM_REPLY ????

**WMIcmd(0006h) - WMI_DELETE_PSTREAM_CMD ;aka WMI_DEL_PRIORITY_STREAM**

Parameters (05h bytes):

```

| 00h A_UINT8 1 txQueueNumber
 01h A_UINT8 1 rxQueueNumber
 02h A_UINT8 1 trafficDirection
 03h A_UINT8 1 trafficClass
 04h A_UINT8 1 tsid |
| --- |

XXX see WMI_DEL_PRIORITY_STREAM_REPLY ????

**WMIevent(1007h) - WMI_PSTREAM_TIMEOUT_EVENT**

Event Data (04h bytes):

```

| 00h A_UINT8 1 txQueueNumber
 01h A_UINT8 1 rxQueueNumber
 02h A_UINT8 1 trafficDirection
 03h A_UINT8 1 trafficClass |
| --- |

| DSi Atheros Wifi - WMI Roam Functions |
| --- |

**WMIcmd(0029h) - WMI_SET_ROAM_CTRL_CMD**

Parameters (09h bytes on DSi?, but would be greater if "numBss>1"):

```

| PREPACK union {
 .. A_UINT8 bssid[ATH_MAC_LEN] ;WMI_FORCE_ROAM
 .. A_UINT8 roamMode ;WMI_SET_ROAM_MODE
 .. WMI_BSS_BIAS_INFO bssBiasInfo ;WMI_SET_HOST_BIAS
 .. WMI_LOWRSSI_SCAN_PARAMS lrScanParams
 } POSTPACK info
 .. A_UINT8 roamCtrlType |
| --- |

This command is used to influence the Roaming behaviour.

Set the host biases of the BSSs before setting the roam mode as bias based.

WMI_ROAM_CTRL_TYPE, Different types of Roam Control:

```

| WMI_FORCE_ROAM = 1 ;Roam to the specified BSSID
 WMI_SET_ROAM_MODE = 2 ;default, progd bias, no roam
 WMI_SET_HOST_BIAS = 3 ;Set the Host Bias
 WMI_SET_LOWRSSI_SCAN_PARAMS = 4 ;Set lowrssi Scan parameters |
| --- |

WMI_ROAM_MODE, aka ROAM MODES:

```

| WMI_DEFAULT_ROAM_MODE = 1 ;RSSI based ROAM
 WMI_HOST_BIAS_ROAM_MODE = 2 ;HOST BIAS based ROAM
 WMI_LOCK_BSS_MODE = 3 ;Lock to the Current BSS - no Roam |
| --- |

BSS HOST BIAS INFO structures:

```

| WMI_BSS_BIAS typedef PREPACK struct:
 6 A_UINT8 bssid[ATH_MAC_LEN]
 1 A_INT8 bias
 WMI_BSS_BIAS_INFO typedef PREPACK struct:
 00h 1 A_UINT8 numBss
 01h 7*N WMI_BSS_BIAS bssBias[1]
 WMI_LOWRSSI_SCAN_PARAMS typedef PREPACK struct WMI_LOWRSSI_SCAN_PARAMS:
 00h 2 A_UINT16 lowrssi_scan_period
 02h 2 A_INT16 lowrssi_scan_threshold
 04h 2 A_INT16 lowrssi_roam_threshold
 06h 1 A_UINT8 roam_rssi_floor
 07h 1 A_UINT8 reserved[1] ;for alignment |
| --- |

**WMIcmd(002Ah) - WMI_GET_ROAM_TBL_CMD ;aka WMI_REPORT_ROAM_TBL ;reply 100Fh**

Parameters:

```

| Unknown (none?) |
| --- |

Reply: See WMI_REPORT_ROAM_TBL_EVENT

**WMIevent(100Fh) - WMI_REPORT_ROAM_TBL_EVENT ;related to CMD 002Ah ?**

Event Data (14h bytes on DSi, might be more on other systems if
numEntries>1):

```

| 00h A_UINT16 2 roamMode
 02h A_UINT16 2 numEntries
 04h WMI_BSS_ROAM_INFO N*10h bssRoamInfo[1] |
| --- |

WMI_BSS_ROAM_INFO structure:

```

| A_INT32 4 roam_util
 A_UINT8 6 bssid[ATH_MAC_LEN]
 A_INT8 1 rssi
 A_INT8 1 rssidt
 A_INT8 1 last_rssi
 A_INT8 1 util
 A_INT8 1 bias
 A_UINT8 1 reserved ;for alignment |
| --- |

MAX_ROAM_TBL_CAND = 5

Names: WMI_GET_ROAM_TBL aka WMI_REPORT_ROAM_TBL aka WMI_TARGET_ROAM_TBL

| DSi Atheros Wifi - WMI Power Functions |
| --- |

**WMIcmd(0012h) - WMI_SET_POWER_MODE_CMD ;aka WMI_POWER_MODE_CMD**

Parameters (01h bytes):

```

| 00h A_UINT8 1 powerMode ;WMI_POWER_MODE |
| --- |

WMI_POWER_MODE values:

```

| REC_POWER = 01h ;recommended,receive,record,rectal?
 MAX_PERF_POWER = 02h ;maximum performance,perforation? |
| --- |

**WMIcmd(0013h) - WMI_SET_IBSS_PM_CAPS_CMD ;aka WMI_IBSS_PM_CAPS_CMD**

Parameters (06h bytes):

```

| 00h A_UINT8 1 power_saving
 01h A_UINT8 1 ttl ;number of beacon periods
 02h A_UINT16 2 atim_windows ;msec
 04h A_UINT16 2 timeout_value ;msec |
| --- |

Adhoc power save types aka WMI_ADHOC_PS_TYPE:

```

| ADHOC_PS_DISABLE = 1
 ADHOC_PS_ATH = 2
 ADHOC_PS_IEEE = 3
 ADHOC_PS_OTHER = 4 |
| --- |

**WMIcmd(0014h) - WMI_SET_POWER_PARAMS_CMD ;aka WMI_POWER_PARAMS_CMD**

Parameters (06h bytes on DSi?, but other sources claim 0Ch bytes):

```

| 06h 0Ch <---- total size (on DSi it's 06h, ie. left column)
 00h 00h A_UINT16 2 idle_period ;msec
 02h 02h A_UINT16 2 pspoll_number
 04h 04h A_UINT16 2 dtim_policy
 -- 06h A_UINT16 2 tx_wakeup_policy
 -- 08h A_UINT16 2 num_tx_to_wakeup
 -- 0Ah A_UINT16 2 ps_fail_event_policy |
| --- |

WMI_DTIM_POLICY values:

```

| IGNORE_DTIM = 01h
 NORMAL_DTIM = 02h
 STICK_DTIM = 03h
 AUTO_DTIM = 04h |
| --- |

WMI_TX_WAKEUP_POLICY_UPON_SLEEP values (Policy to determnine (Nine?) whether TX
should wakeup WLAN if sleeping):

```

| TX_WAKEUP_UPON_SLEEP = 1
 TX_DONT_WAKEUP_UPON_SLEEP = 2 |
| --- |

POWER_SAVE_FAIL_EVENT_POLICY values (Policy to determnine (Nine?) whether power
save failure event should be sent to host during scanning):

```

| SEND_POWER_SAVE_FAIL_EVENT_ALWAYS = 1
 IGNORE_POWER_SAVE_FAIL_EVENT_DURING_SCAN = 2 |
| --- |

**WMIcmd(0015h) - WMI_SET_POWERSAVE_TIMERS_POLICY_CMD ;aka WMI_POWERSAVE...**

Parameters (0Ch bytes):

```

| 00h A_UINT16 2 psPollTimeout (msec)
 02h A_UINT16 2 triggerTimeout (msec)
 04h A_UINT32 4 apsdTimPolicy (TIM behavior with ques (=?) APSD enabled.
 Default is IGNORE_TIM_ALL_QUEUES_APSD)
 08h A_UINT32 4 simulatedAPSDTimPolicy (TIM behavior with simulated APSD
 enabled. Default is PROCESS_TIM_SIMULATED_APSD) |
| --- |

APSD_TIM_POLICY values:

```

| IGNORE_TIM_ALL_QUEUES_APSD = 0
 PROCESS_TIM_ALL_QUEUES_APSD = 1
 IGNORE_TIM_SIMULATED_APSD = 2
 PROCESS_TIM_SIMULATED_APSD = 3 |
| --- |

**WMIcmd(001Bh) - WMI_SET_TX_PWR_CMD**

Parameters:

```

| 00h A_UINT8 1 dbM ;in dbM units |
| --- |

**WMIcmd(001Ch) - WMI_GET_TX_PWR_CMD ;aka WMI_TX_PWR ;reply 001Ch**

Parameters:

```

| None |
| --- |

Reply:

```

| 00h A_UINT8 1 dbM ;in dbM units |
| --- |

**WMIcmd(0048h) - WMI_SET_AP_PS_CMD ;aka WMI_AP_PS_CMD ;not implemented on DSi**

This supported in most or all atheros code versions, but DSi doesn't support
it.

Parameters (0Ah bytes):

```

| - 00h A_UINT32 4 idle_time ;in msec
 - 04h A_UINT32 4 ps_period ;in usec
 - 08h A_UINT8 1 sleep_period ;in ps_period's (=above "ps_period"?)
 - 09h A_UINT8 1 psType ;AP power save type |
| --- |

WMI_AP_PS_TYPE, AP power save types:

```

| AP_PS_DISABLE = 1
 AP_PS_ATH = 2 |
| --- |

```

| _________________________________ Heartbeat __________________________________ |
| --- |

Heartbeat Challenge Response is said to be an "Error Detection support"
feature, but it isn't exactly clear what it is supposed to do. DSi software
does usually set the heartbeat timeout to 2, and does then throw heartbeat
commands once per second or so (which will respond with a heartbeat event and
restart the heartbeat timeout).

If a timeout occurs because of not sending heartbeat commands in time: Some
firmwares do simply ignoring the timeout (eg. DSi/AR6013, or 3DS/AR6014 with
basic Type1 firmware). However, other firmwares do hang up (eg. 3DS/AR6014 with
AP-mode Type4 firmware). There seems to be no error message/event on timeout,
and no known way to recover from timeout.

**WMIcmd(002Eh:2008h) - WMIX_HB_CHALLENGE_RESP_CMD ;reply=3007h ;HB=heartbeat**

```

| 00h A_UINT32 4 cookie ;usually increasing 1,2,3,4,5,etc.
 04h A_UINT32 4 source ;usually 0 |
| --- |

This should be sent once per second or so, the command does reload the
heartbeat timeout and (immediately) trigger a WMIX_HB_CHALLENGE_RESP_EVENT.

The cookie/source parameters are passed on to the reponse event (and seem to
have no other function).

**WMIevent(1010h:3007h) - WMIX_HB_CHALLENGE_RESP_EVENT ;used (reply to 2008h)**

Event Data:

```

| 00h A_UINT32 4 cookie ;\same reply-format as command parameters
 04h A_UINT32 4 source ;/ |
| --- |

**WMIcmd(0047h) - WMI_SET_HB_CHALLENGE_RESP_PARAMS_CMD**

This seems to be a nintendo-specific command with 4-byte parameter (official
atheros source code contains a 5-byte WMI_SET_HB_CHALLENGE_RESP_PARAMS_CMD
structure, but doesn't have any CMD number assigned to it).

Parameters (04h or 05h bytes):

```

| 04h 05h <---- total size (on DSi it's 04h, ie. left column)
 00h - A_UINT32 4 timeout ;in seconds, used: 00000002h ;(or 0=Disable)
 - 00h A_UINT32 4 frequency ;\whatever, not really used because there's
 - 04h A_UINT8 1 threshold ;/no WMIcmd(xxxxh) value assigned |
| --- |

BUG: The function uses a bizarre timeout*1000/1000 mechanism. Values in range
timeout=21h..83h (aka timeout*1000=8000h..1FFFFh) won't work on AR6002 (which
tries to add a 17bit fractional part to such values; this is bugfixed in
AR6013/AR6014 by using only 15bit fractions). And, on all chips, there will be
an overflow on very large values (when timeout*1000/1000 plus 15bit/17bit
fraction exceeds 32bit).

| DSi Atheros Wifi - WMI Statistics Function |
| --- |

**WMIcmd(0010h) - WMI_GET_STATISTICS_CMD ;reply WMI_REPORT_STATISTICS**

Parameters:

```

| Unknown (none?) |
| --- |

**WMIevent(100Bh) - WMI_REPORT_STATISTICS_EVENT ;related to CMD 0010h**

Event Data (D5h bytes on DSi, although other sources claim A9h or EDh bytes):

```

| A9h D5h EDh <---- total size (on DSi it's D5h, ie. middle column)
 -- 00h 00h A_UINT32 4 lqVal ;- <-- newer version only
 -- 04h 04h A_INT32 4 noise_floor_calibation ;- <-- newer version only
 -- 08h 08h A_UINT32 4 power_save_failure_cnt ;\pm_stats_t (new)
 -- -- 0Ch A_UINT16 2 stop_tx_failure_cnt ; <-- NEWEST version only
 -- -- 0Eh A_UINT16 2 atim_tx_failure_cnt ; <-- NEWEST version only
 -- -- 10h A_UINT16 2 atim_rx_failure_cnt ; <-- NEWEST version only
 -- -- 12h A_UINT16 2 bcn_rx_failure_cnt ;/ <-- NEWEST version only
 00h 0Ch 14h A_UINT32 4 tx_packets ;\
 04h 10h 18h A_UINT32 4 tx_bytes ;
 08h 14h 1Ch A_UINT32 4 tx_unicast_pkts ;
 0Ch 18h 20h A_UINT32 4 tx_unicast_bytes ; tx_stats_t
 10h 1Ch 24h A_UINT32 4 tx_multicast_pkts ;
 14h 20h 28h A_UINT32 4 tx_multicast_bytes ;
 18h 24h 2Ch A_UINT32 4 tx_broadcast_pkts ;
 1Ch 28h 30h A_UINT32 4 tx_broadcast_bytes ;
 20h 2Ch 34h A_UINT32 4 tx_rts_success_cnt ;
 24h 30h 38h A_UINT32 16 tx_packet_per_ac[4] ;
 -- 40h 48h A_UINT32 16 tx_errors_per_ac[4] ; <-- newer version only
 34h 50h 58h A_UINT32 4 tx_errors ;
 38h 54h 5Ch A_UINT32 4 tx_failed_cnt ;
 3Ch 58h 60h A_UINT32 4 tx_retry_cnt ;
 -- -- 64h A_UINT32 4 tx_mult_retry_cnt ; <-- NEWEST version only
 40h 5Ch 68h A_UINT32 4 tx_rts_fail_cnt ;
 -- 60h 6Ch A_INT32 4 tx_unicast_rate ;/ <-- newer version only
 44h 64h 70h A_UINT32 4 rx_packets ;\
 48h 68h 74h A_UINT32 4 rx_bytes ;
 4Ch 6Ch 78h A_UINT32 4 rx_unicast_pkts ;
 50h 70h 7Ch A_UINT32 4 rx_unicast_bytes ;
 54h 74h 80h A_UINT32 4 rx_multicast_pkts ; rx_stats_t
 58h 78h 84h A_UINT32 4 rx_multicast_bytes ;
 5Ch 7Ch 88h A_UINT32 4 rx_broadcast_pkts ;
 60h 80h 8Ch A_UINT32 4 rx_broadcast_bytes ;
 64h 84h 90h A_UINT32 4 rx_fragment_pkt ;
 68h 88h 94h A_UINT32 4 rx_errors ;
 6Ch 8Ch 98h A_UINT32 4 rx_crcerr ;
 70h 90h 9Ch A_UINT32 4 rx_key_cache_miss ;
 74h 94h A0h A_UINT32 4 rx_decrypt_err ;
 78h 98h A4h A_UINT32 4 rx_duplicate_frames ;
 -- 9Ch A8h A_INT32 4 rx_unicast_rate ;/ <-- newer version only
 7Ch A0h ACh A_UINT32 4 tkip_local_mic_failure ;\
 80h A4h B0h A_UINT32 4 tkip_counter_measures_invoked ;
 84h A8h B4h A_UINT32 4 tkip_replays ; tkip_ccmp_stats_t
 88h ACh B8h A_UINT32 4 tkip_format_errors ;
 8Ch B0h BCh A_UINT32 4 ccmp_format_errors ;
 90h B4h C0h A_UINT32 4 ccmp_replays ;/
 -- B8h C4h A_UINT32 4 wow_num_pkts_dropped ;\
 -- BCh C8h A_UINT16 2 wow_num_events_discarded ; wlan_wow_stats_t
 -- BEh CAh A_UINT8 1 wow_num_host_pkt_wakeups ;
 -- BFh CBh A_UINT8 1 wow_num_host_event_wakeups ;/
 -- -- CCh A_UINT32 4 arp_received ;\
 -- -- D0h A_UINT32 4 arp_matched ; arp_stats_t
 -- -- D4h A_UINT32 4 arp_replied ;/
 94h C0h D8h A_UINT32 4 cs_bmiss_cnt ;\
 98h C4h DCh A_UINT32 4 cs_lowRssi_cnt ;
 9Ch C8h E0h A_UINT16 2 cs_connect_cnt ; cserv_stats_t
 9Eh CAh E2h A_UINT16 2 cs_disconnect_cnt ;
 A0h CCh E4h A_INT16 2 cs_aveBeacon_rssi ;
 -- CEh E6h A_UINT16 2 cs_roam_count ; <-- newer version only
 -- D0h E8h A_INT16 2 cs_rssi ; <-- newer version only
 -- D2h EAh A_UINT8 1 cs_snr ; <-- newer version only
 -- D3h EBh A_UINT8 1 cs_aveBeacon_snr ; <-- newer version only
 A2h D4h ECh A_UINT8 1 cs_lastRoam_msec ;/
 A3h -- -- A_UINT32 4 power_save_failure_cnt ;-pm_stats_t (old)
 A7h -- -- A_INT16 2 noise_floor_calibation ;-old version only |
| --- |

The D5h-byte structure was found in AR6kSDK.build_sw.18 from 2006.

The A9h-byte structure was found in older (AR6001?) source from 2006.

The EDh-byte structure was found in newer source from 2008 and up.

Names: WMI_GET_STATISTICS aka WMI_REPORT_STATISTICS aka WMI_TARGET_STATS

| DSi Atheros Wifi - WMI Bluetooth Coexistence (older AR6002) |
| --- |

Bluetooth Coexistence support has underwent significant changes:

```

| Older AR6002 from 2008 ;-original Bluetooth COEX version
 Newer AR6002 from 2008 ;\same commands as above, but with entirely different
 Newer AR6002 from 2010 ;/parameters (and minor changes for 2008 vs 2010)
 AR6003 from 2010 ;-completely different commands for Bluetooth COEX |
| --- |

Below are Bluetooth COEX functions for OLDER AR6002 - as used on DSi - and as
defined in "AR6kSDK.build_sw.18".

On DSi, AR6002 does handle command 003Ch and 003Ch, but AR6013/AR6014 do merely
redirect them to dummy handlers; despite of also having (unused) actual
handlers for those commands.

**WMIcmd(003Bh) - WMI_SET_BT_STATUS_CMD ;AR6002 Bluetooth Coexistence only?**

Parameters (02h bytes):

```

| 00h A_UINT8 1 streamType ;aka BT_STREAM_TYPE ?
 01h A_UINT8 1 status ;aka BT_STREAM_STATUS ? |
| --- |

BT_STREAM_TYPE values:

```

| BT_STREAM_UNDEF = 0 ;\
 BT_STREAM_SCO = 1 ;SCO stream ; only this three types in Older AR6002
 BT_STREAM_A2DP = 2 ;A2DP stream ;/ |
| --- |

BT_STREAM_STATUS values:

```

| BT_STATUS_UNDEF = 0 ;\
 BT_STATUS_START = 1 ; this five states in Older AR6002
 BT_STATUS_STOP = 2 ; (Newer A6002 has added/removed/renamed states)
 BT_STATUS_RESUME = 3 ;
 BT_STATUS_SUSPEND = 4 ;/ |
| --- |

AR6002 only? (although other comment says "COMMON to AR6002 and AR6003"?)

**WMIcmd(003Ch) - WMI_SET_BT_PARAMS_CMD ;AR6002 Bluetooth Coexistence only?**

Parameters (16h bytes on DSi?, whatever that might match up with below?):

```

| when paramType=1=BT_PARAM_SCO: ;SCO stream parameters (BT_PARAMS_SCO)
 00h A_UINT8 1 noSCOPkts
 01h A_UINT8 1 pspollTimeout
 02h A_UINT8 1 stompbt
 03h PAD 12h undefined/padding
 when paramType=2=BT_PARAM_A2DP: ;whatever (BT_PARAMS_A2DP)
 00h A_UINT32 4 period
 04h A_UINT32 4 dutycycle
 08h A_UINT8 1 stompbt
 09h PAD 0Ch undefined/padding
 when paramType=3=BT_PARAM_MISC and paramSubType=1=WLAN_PROTECT_POLICY:
 00h A_UINT32 4 period
 04h A_UINT32 4 dutycycle
 08h A_UINT8 1 stompbt
 09h A_UINT8 1 policy
 0Ah A_UINT8 1 paramSubType (=1 in this case)
 0Bh PAD 0Ah undefined/padding
 when paramType=3=BT_PARAM_MISC and paramSubType=2=WLAN_COEX_CTRL_FLAGS:
 00h A_UINT16 2 wlanCtrlFlags
 02h PAD 8 undefined/padding
 0Ah A_UINT8 1 paramSubType (=2 in this case)
 0Bh PAD 0Ah undefined/padding
 when paramType=4=BT_PARAM_REGS: ;co-existence register params (BT_COEX_REGS)
 00h A_UINT32 4 mode
 04h A_UINT32 4 scoWghts
 08h A_UINT32 4 a2dpWghts
 0Ch A_UINT32 4 genWghts
 10h A_UINT32 4 mode2
 14h A_UINT8 1 setVal
 and, in all cases:
 15h A_UINT8 1 paramType ;<-- selects which of the above to use |
| --- |

Below might be "policy" for WLAN_PROTECT_POLICY(?):

```

| WLAN_PROTECT_PER_STREAM = 01h /* default
 WLAN_PROTECT_ANY_TX = 02h |
| --- |

Below might be "wlanCtrlFlags" for WLAN_COEX_CTRL_FLAGS(?):

```

| WLAN_DISABLE_COEX_IN_DISCONNECT = 0001h ;default
 WLAN_KEEP_COEX_IN_DISCONNECT = 0002h
 WLAN_STOMPBT_IN_DISCONNECT = 0004h
 WLAN_DISABLE_COEX_IN_ROAM = 0010h ;default
 WLAN_KEEP_COEX_IN_ROAM = 0020h
 WLAN_STOMPBT_IN_ROAM = 0040h
 WLAN_DISABLE_COEX_IN_SCAN = 0100h ;default
 WLAN_KEEP_COEX_IN_SCAN = 0200h
 WLAN_STOMPBT_IN_SCAN = 0400h
 WLAN_DISABLE_COEX_BT_OFF = 1000h ;default
 WLAN_KEEP_COEX_BT_OFF = 2000h
 WLAN_STOMPBT_BT_OFF = 4000h |
| --- |

| DSi Atheros Wifi - WMI Wake on Wireless (WOW) Functions |
| --- |

**WMIcmd(0042h) - WMI_SET_HOST_SLEEP_MODE_CMD**

Parameters (08h bytes on DSi?, so, a BOOL must be 4 bytes?):

```

| 00h A_BOOL 4 awake;
 04h A_BOOL 4 asleep; |
| --- |

See also: WMI_SET_HOST_SLEEP_MODE_CMD_PROCESSED_EVENT

**WMIcmd(0043h) - WMI_SET_WOW_MODE_CMD**

Event Data (04h bytes on DSi?, but other sources claim MORE bytes?):

```

| 04h ??h <---- total size (on DSi it's 04h, ie. left column)
 00h 00h A_BOOL 4 enable_wow
 -- 04h WMI_WOW_FILTER .. filter ;UINTx or so? with "WMI_WOW_FILTER" value?
 -- .. A_UINT16 2 hostReqDelay |
| --- |

WMI_WOW_FILTER values (only one defined):

```

| WOW_FILTER_SSID = 01h |
| --- |

**WMIcmd(0044h) - WMI_GET_WOW_LIST_CMD ;reply 1018h (!)**

Parameters:

```

| 00h A_UINT8 1 filter_list_id; |
| --- |

Reply (88h bytes on DSi): WMI_GET_WOW_LIST_EVENT:

```

| 00h A_UINT8 1 num_filters /* number of patterns in reply
 01h A_UINT8 1 this_filter_num /* filter # x of total num_filters
 02h A_UINT8 1 wow_mode
 03h A_UINT8 1 host_mode
 04h WOW_FILTER N*84h wow_filters[1] |
| --- |

WOW_FILTER structure:

```

| A_UINT8 1 wow_valid_filter;
 A_UINT8 1 wow_filter_id;
 A_UINT8 1 wow_filter_size;
 A_UINT8 1 wow_filter_offset;
 A_UINT8 40h wow_filter_mask[WOW_MASK_SIZE];
 A_UINT8 40h wow_filter_pattern[WOW_PATTERN_SIZE]; |
| --- |

There's also a "WOW_FILTER_LIST" structure (unknown purpose):

```

| A_UINT8 1 wow_valid_list;
 A_UINT8 1 wow_list_id;
 A_UINT8 1 wow_num_filters;
 A_UINT8 1 wow_total_list_size;
 WOW_FILTER 4*84h list[WOW_MAX_FILTERS_PER_LIST];
 #define WOW_MAX_FILTER_LISTS = 1 /* 4 */
 #define WOW_MAX_FILTERS_PER_LIST = 4
 #define WOW_PATTERN_SIZE = 64
 #define WOW_MASK_SIZE = 64 |
| --- |

**WMIcmd(0045h) - WMI_ADD_WOW_PATTERN_CMD**

Parameters:

```

| 00h A_UINT8 1 filter_list_id;
 01h A_UINT8 1 filter_size;
 02h A_UINT8 1 filter_offset;
 03h A_UINT8 .. filter[1]; |
| --- |

**WMIcmd(0046h) - WMI_DEL_WOW_PATTERN_CMD**

Parameters (04h bytes):

```

| 00h A_UINT16 2 filter_list_id;
 02h A_UINT16 2 filter_id; |
| --- |

**WMIevent(1018h) - WMI_GET_WOW_LIST_EVENT ;reply to CMD 0044h**

See WMI_GET_WOW_LIST_CMD for response details.

| DSi Atheros Wifi - WMI General Purpose I/O (GPIO) Functions |
| --- |

**WMIcmd(002Eh:2003h) - WMIX_GPIO_OUTPUT_SET_CMD ;reply=3006h**

Parameters:

```

| 00h A_UINT32 4 set_mask; /* pins to set
 04h A_UINT32 4 clear_mask; /* pins to clear
 08h A_UINT32 4 enable_mask; /* pins to enable for output
 0Ch A_UINT32 4 disable_mask; /* pins to disable/tristate |
| --- |

Set GPIO pin output state.

In order for output to be driven, a pin must be enabled for output.

This can be done during initialization through the GPIO Configuration DataSet,
or during operation with the enable_mask.

If a request is made to simultaneously set/clear or set/disable or
clear/disable or disable/enable, results are undefined.

NB: Some of the WMIX APIs use a 32-bit mask. On Targets that support more than
32 GPIO pins, those APIs only support the first 32 GPIO pins.

**WMIcmd(002Eh:2004h) - WMIX_GPIO_INPUT_GET_CMD ;reply=3005h**

Parameters:

```

| Unknown (none?) |
| --- |

**WMIcmd(002Eh:2005h) - WMIX_GPIO_REGISTER_SET_CMD ;reply=3006h, too**

Parameters:

```

| 00h A_UINT32 4 gpioreg_id; /* GPIO register ID
 04h A_UINT32 4 value; /* value to write |
| --- |

Set a GPIO register. For debug/exceptional cases.

Values for gpioreg_id are GPIO_ID_*, defined in a platform-dependent header,
gpio.h.

**WMIcmd(002Eh:2006h) - WMIX_GPIO_REGISTER_GET_CMD ;reply=3005h, too**

Parameters:

```

| 00h A_UINT32 4 gpioreg_id; /* GPIO register to read |
| --- |

Get a GPIO register. For debug/exceptional cases.

**WMIcmd(002Eh:2007h) - WMIX_GPIO_INTR_ACK_CMD**

Parameters:

```

| A_UINT32 ack_mask; /* interrupts to acknowledge |
| --- |

Host acknowledges and re-arms GPIO interrupts. A single message should be used
to acknowledge all interrupts that were delivered in an earlier
WMIX_GPIO_INTR_EVENT message.

**WMIevent(1010h:3004h) - WMIX_GPIO_INTR_EVENT ;used (interrupt)**

Event Data:

```

| 00h A_UINT32 4 intr_mask; /* pending GPIO interrupts
 04h A_UINT32 4 input_values; /* recent GPIO input values |
| --- |

Target informs Host of GPIO interrupts that have ocurred since the last
WMIX_GIPO_INTR_ACK_CMD was received. Additional information -- the current GPIO
input values is provided -- in order to support use of a GPIO interrupt as a
Data Valid signal for other GPIO pins.

**WMIevent(1010h:3005h) - WMIX_GPIO_DATA_EVENT ;used (reply to 2004h and 2006h)**

Event Data:

```

| 00h A_UINT32 4 value;
 04h A_UINT32 4 reg_id; |
| --- |

Target responds to Host's earlier WMIX_GPIO_INPUT_GET_CMD request using a
GPIO_DATA_EVENT with value set to the mask of GPIO pin inputs and reg_id set to
GPIO_ID_NONE.

Target responds to Hosts's earlier WMIX_GPIO_REGISTER_GET_CMD request using a
GPIO_DATA_EVENT with value set to the value of the requested register and
reg_id identifying the register (reflects the original request).

NB: reg_id supports the future possibility of unsolicited WMIX_GPIO_DATA_EVENTs
(for polling GPIO input), and it may simplify Host GPIO support.

**WMIevent(1010h:3006h) - WMIX_GPIO_ACK_EVENT ;used (reply to 2003h and 2005h)**

Event Data:

```

| Unknown (none?) (confirms GPIO_xxx_SET commands) |
| --- |

**GPIO Constants**

```

| AR6001_GPIO_PIN_COUNT = 18
 AR6002_GPIO_PIN_COUNT = 18 ;aka hw2.0
 AR6003_GPIO_PIN_COUNT = 28 ;aka hw4.0 ;XXX shouldn't that be 26 ?
 MCKINLEY_GPIO_PIN_COUNT = 57 ;aka hw6.0 |
| --- |

Values of gpioreg_id in the WMIX_GPIO_REGISTER_SET_CMDID and
WMIX_GPIO_REGISTER_GET_CMDID commands come in two flavors. If the upper bit of
gpioreg_id is CLEAR, then the remainder is interpreted as one of these values.
This provides platform-independent access to GPIO registers. If the upper bit
(GPIO_ID_OFFSET_FLAG) of gpioreg_id is SET, then the remainder is interpreted
as a platform-specific GPIO register offset.

```

| GPIO_ID_OUT = 00000000h
 GPIO_ID_OUT_W1TS = 00000001h
 GPIO_ID_OUT_W1TC = 00000002h
 GPIO_ID_ENABLE = 00000003h
 GPIO_ID_ENABLE_W1TS = 00000004h
 GPIO_ID_ENABLE_W1TC = 00000005h
 GPIO_ID_IN = 00000006h
 GPIO_ID_STATUS = 00000007h
 GPIO_ID_STATUS_W1TS = 00000008h
 GPIO_ID_STATUS_W1TC = 00000009h
 GPIO_ID_PIN0 = 0000000Ah
 GPIO_ID_PIN(n) = (GPIO_ID_PIN0+(n)) ;=0000000Ah and up
 GPIO_ID_NONE = FFFFFFFFh
 GPIO_ID_OFFSET_FLAG = 80000000h
 GPIO_ID_REG_MASK = 7fffffffh
 GPIO_ID_IS_OFFSET(reg_id) = (((reg_id) & GPIO_ID_OFFSET_FLAG) != 0) |
| --- |

| DSi Atheros Wifi - Unimplemented WMI Misc Functions |
| --- |

Not implemented in DSi.

**WMIcmd(002Bh) - WMI_GET_ROAM_DATA_CMD ;reply 1015h ? ;not implemented in DSi**

Parameters:

```

| Unknown (none?) |
| --- |

Reply: See WMI_REPORT_ROAM_DATA_EVENT

**WMIevent(1015h) - WMI_REPORT_ROAM_DATA_EVENT**

```

| ;-not implemented in DSi ;related to 002Bh? |
| --- |

Event Data:

```

| PREPACK union {
 00h WMI_TARGET_ROAM_TIME roamTime;
 } POSTPACK u;
 14h A_UINT8 roamDataType ; |
| --- |

ROAM_DATA_TYPE values (only one defined)

```

| ROAM_DATA_TIME = 1 /* Get The Roam Time Data |
| --- |

WMI_TARGET_ROAM_TIME structure:

```

| 00h A_UINT32 4 disassoc_time
 04h A_UINT32 4 no_txrx_time
 08h A_UINT32 4 assoc_time
 0Ch A_UINT32 4 allow_txrx_time
 10h A_UINT8 6 disassoc_bssid[ATH_MAC_LEN]
 16h A_INT8 1 disassoc_bss_rssi
 17h A_UINT8 6 assoc_bssid[ATH_MAC_LEN] ;UNALIGNED!!!
 1Dh A_INT8 1 assoc_bss_rssi |
| --- |

Names: WMI_GET_ROAM_DATA aka WMI_REPORT_ROAM_DATA aka WMI_TARGET_ROAM_DATA

**WMIcmd(002Ch) - WMI_ENABLE_RM_CMD ;not implemented in DSi**

Parameters:

```

| 00h A_BOOL 4 enable_radio_measurements; |
| --- |

**WMIcmd(002Dh) - WMI_SET_MAX_OFFHOME_DURATION_CMD ;not implemented in DSi**

Parameters:

```

| 00h A_UINT8 1 max_offhome_duration; |
| --- |

**WMIcmd(002Eh:200Ah) - WMIX_PROF_CFG_CMD**

**WMIcmd(002Eh:200Bh) - WMIX_PROF_ADDR_SET_CMD**

**WMIcmd(002Eh:200Ch) - WMIX_PROF_START_CMD**

**WMIcmd(002Eh:200Dh) - WMIX_PROF_STOP_CMD**

**WMIcmd(002Eh:200Eh) - WMIX_PROF_COUNT_GET_CMD ;reply 3009h**

Not implemented in DSi. Said to be "Target Profiling support".

Parameter structures are defined only for WMIX_PROF_CFG_CMD and
WMIX_PROF_ADDR_SET_CMD.

Parameters for WMIX_PROF_CFG_CMD:

```

| 00h A_UINT32 4 period; /* Time (in 30.5us ticks) between samples
 04h A_UINT32 4 nbins; |
| --- |

Parameters for WMIX_PROF_ADDR_SET_CMD:

```

| 00h A_UINT32 4 addr; |
| --- |

Maybe the other three WMIX_PROF_xxx_CMD's don't have any parameters.

See also: WMIX_PROF_COUNT_EVENT

**WMIevent(1010h:3009h) - WMIX_PROF_COUNT_EVENT ;-not implemented in DSi**

Not implemented in DSi. Response to WMIX_PROF_COUNT_GET_CMD.

Event Data:

```

| 00h A_UINT32 4 addr;
 04h A_UINT32 4 count; |
| --- |

Target responds to Hosts's earlier WMIX_PROF_COUNT_GET_CMD request using a
WMIX_PROF_COUNT_EVENT with addr set to the next address count set to the
corresponding count.

**WMIcmd(003Ah) - WMI_TEST_CMD ;not implemented in DSi**

Parameters:

```

| Unknown (maybe related to file "testcmd.h"?) |
| --- |

**WMIevent(1016h) - WMI_TEST_EVENT ;not implemented in DSi**

Event Data:

```

| Unknown (maybe related to file "testcmd.h"?) (or general purpose?) |
| --- |

**WMIcmd(0040h) - WMI_GET_APPIE_CMD ;aka GET_APP_IE ;not implemented in DSi**

Parameters:

```

| Unknown (none?) |
| --- |

Reply: Unknown:

```

| EVENTID is unknown (maybe 0040h, ie. same as GET_APPIE_CMD)
 Reply structure is unknown (maybe same parameter structure for SET_APPIE_CMD) |
| --- |

**WMIcmd(004Ah) - WMI_SET_IE_CMD ;not implemented in DSi (newer 2012 stuff)**

Parameters:

```

| 00h u8 1 ie_id;
 01h u8 1 ie_field; /* enum wmi_ie_field_type
 02h u8 1 ie_len;
 03h u8 1 reserved;
 04h u8 .. ie_info[0]; |
| --- |

wmi_ie_field_type:

```

| WMI_RSN_IE_CAPB = 01h
 WMI_IE_FULL = FFh /* indicats full IE ;uh, kittykats? |
| --- |

See also: WMI_SET_APP_IE_CMD (similar older command)

**WMIcmd(08xxh) - wil6210: WILOCITY types ;not implemented in DSi**

**WMIcmd(09xxh) - wil6210: Performance monitoring ;not implemented in DSi**

```

| not implemented in DSi |
| --- |

**WMIcmd(F003h, or formerly 0047h) - WMI_SET_MAC_ADDRESS_CMD**

Parameters:

```

| 00h A_UINT8 6 macaddr[ATH_MAC_LEN]; |
| --- |

**WMIcmd(F007h) - WMI_ABORT_SCAN_CMD ;not implemented in DSi**

Parameters:

```

| Unknown (none?) |
| --- |

Reply: Unknown, if any (see (optional?) SCAN_ABORT_EVENT)

**WMIcmd(F008h) - WMI_SET_TARGET_EVENT_REPORT_CMD ;not implemented in DSi**

Parameters:

```

| 00h A_UINT32 1 evtConfig; |
| --- |

TARGET_EVENT_REPORT_CONFIG values:

```

| DISCONN_EVT_IN_RECONN = 0 ;default
 NO_DISCONN_EVT_IN_RECONN = 1 |
| --- |

Apparently related to cases where to throw WMI_DISCONNECT_EVENT.

**WMIcmd(F017h or formerly F016h) - WMI_SET_IP_CMD**

Parameters:

```

| 00h A_UINT32 4*2 ips[MAX_IP_ADDRS] ;IP in Network Byte Order |
| --- |

#define MAX_IP_ADDRS 2

**WMIcmd(F018h or formerly F017h) - WMI_SET_PARAMS_CMD ;reply=101Fh**

Parameters:

```

| 00h A_UINT32 4 opcode;
 04h A_UINT32 4 length;
 08h A_CHAR ... buffer[1]; /* WMI_SET_PARAMS |
| --- |

Reply: See WMI_SET_PARAMS_REPLY_EVENT

**WMIevent(101Fh) - WMI_SET_PARAMS_REPLY_EVENT ;reply to "SET" CMD F018h**

Event Data:

```

| 00h A_INT8 1 status; /* WMI_SET_PARAMS_REPLY |
| --- |

Reply to WMI_SET_PARAMS_CMD (?) aka WMI_SET_PARAMS_REPLY aka
WMI_SET_PARAMS_REPLY_EVENT.

**WMIcmd(F019h or formerly F018h) - WMI_SET_MCAST_FILTER_CMD**

Parameters:

```

| 00h A_UINT8 6 multicast_mac[ATH_MAC_LEN]; /* WMI_SET_MCAST_FILTER |
| --- |

**WMIcmd(F01Ah or formerly F019h) - WMI_DEL_MCAST_FILTER_CMD**

Parameters:

```

| Unknown (None?) (or maybe same as for WMI_SET_MCAST_FILTER_CMD ?) |
| --- |

**WMIcmd(F029h) - WMI_MCAST_FILTER_CMD ;related to SET/DEL "MCAST" commands?**

Parameters:

```

| 00h A_UINT8 1 enable; /* WMI_MCAST_FILTER |
| --- |

Related to SET/DEL "MCAST" commands?

**WMIcmd(F01Bh) - WMI_ALLOW_AGGR_CMD**

Parameters:

```

| 00h A_UINT16 2 tx_allow_aggr (16bit mask to allow tx/uplink ADDBA
 negotiation - bit position indicates tid)
 02h A_UINT16 2 rx_allow_aggr (16bit mask to allow rx/donwlink ADDBA
 negotiation - bit position indicates tid) |
| --- |

Configures tid's to allow ADDBA negotiations on each tid, in each direction.

uh, donwlink?

**WMIcmd(F01Ch) - WMI_ADDBA_REQ_CMD**

Parameters:

```

| 00h A_UINT8 1 tid |
| --- |

"f/w starts performing ADDBA negotiations with peer on the given tid"

"f/w" means FirmWare? ForWard? Fail/Wrong? or What?

**WMIcmd(F01Dh) - WMI_DELBA_REQ_CMD**

Parameters:

```

| 00h A_UINT8 1 tid
 01h A_UINT8 1 is_sender_initiator |
| --- |

"f/w would teardown BA with peer." - uh, "f/w"?

"is_send_initiator indicates if it's or tx or rx side" - uh, "it's or"?

**WMIevent(1020h) - WMI_ADDBA_REQ_EVENT**

Event Data:

```

| 00h A_UINT8 1 tid
 01h A_UINT8 1 win_sz
 02h A_UINT16 2 st_seq_no
 04h A_UINT8 1 status "f/w response for ADDBA Req; OK(0) or failure(!=0)" |
| --- |

**WMIevent(1021h) - WMI_ADDBA_RESP_EVENT**

Event Data:

```

| 00h A_UINT8 1 tid
 01h A_UINT8 1 status /* OK(0), failure (!=0)
 02h A_UINT16 2 amsdu_sz /* Three values: Not supported(0), 3839, 8k |
| --- |

Uhm, does "8k" mean 8192 or 8000 or so?

**WMIevent(1022h) - WMI_DELBA_REQ_EVENT aka WMI_DELBA_EVENT**

Event Data:

```

| 00h A_UINT8 1 tid;
 01h A_UINT8 1 is_peer_initiator;
 02h A_UINT16 2 reason_code; |
| --- |

"f/w received a DELBA for peer and processed it. Host is notified of this."

**WMIcmd(F01Eh) - WMI_SET_HT_CAP_CMD**

Parameters:

```

| 00h A_UINT8 1 band (specifies which band to apply these values)
 01h A_UINT8 1 enable (allows 11n to be disabled on a per band basis)
 02h A_UINT8 1 chan_width_40M_supported
 03h A_UINT8 1 short_GI_20MHz
 04h A_UINT8 1 short_GI_40MHz
 05h A_UINT8 1 intolerance_40MHz
 06h A_UINT8 1 max_ampdu_len_exp |
| --- |

**WMIcmd(F01Fh) - WMI_SET_HT_OP_CMD**

Parameters:

```

| 00h A_UINT8 1 sta_chan_width; |
| --- |

**WMIcmd(F020h) - WMI_SET_TX_SELECT_RATES_CMD**

Parameters:

```

| 00h A_UINT32 4*8*2 rateMasks[WMI_MODE_MAX * WMI_MAX_RATE_MASK]; |
| --- |

**WMIcmd(F021h) - WMI_SET_TX_SGI_PARAM_CMD**

Parameters:

```

| 00h A_UINT32 4*2 sgiMask[WMI_MAX_RATE_MASK];
 08h A_UINT8 1 sgiPERThreshold; |
| --- |

DEFAULT_SGI_MASK_L32 = 08080000h

DEFAULT_SGI_MASK_U32 = 00000000h

DEFAULT_SGI_PER = 10

**WMIcmd(F022h) - WMI_SET_RATE_POLICY_CMD**

Parameters:

```

| 00h A_UINT32 4*2 rateField[WMI_MAX_RATE_MASK]
 (rateField: "1 bit per rate corresponding to index")
 08h A_UINT8 1 id ;range 1..5 (aka 1..WMI_RATE_POLICY_ID_MAX)
 09h A_UINT8 1 shortTrys
 0Ah A_UINT8 1 longTrys
 0Bh A_UINT8 1 reserved ;padding |
| --- |

WMI_RATE_POLICY_ID_MAX = 5

**WMIcmd(F023h) - WMI_HCI_CMD_CMD aka WMI_HCI_CMD**

Parameters:

```

| 00h A_UINT16 2 cmd_buf_sz ;HCI cmd buffer size
 02h A_UINT8 .. buf[1] ;Absolute HCI cmd (see file "hci.h") |
| --- |

**WMIevent(1024h) - WMI_HCI_EVENT_EVENT aka WMI_HCI_EVENT**

Event Data:

```

| 00h A_UINT16 2 evt_buf_sz ;HCI event buffer size
 02h A_UINT8 .. buf[1] ;HCI event (see file "hci.h") |
| --- |

**WMIcmd(F024h) - WMI_RX_FRAME_FORMAT_CMD**

Parameters:

```

| 00h A_UINT8 1 metaVersion ;version of meta data for rx packets
 ;(0-7=valid, 0=default)
 01h A_UINT8 1 dot11Hdr ;1=leave .11 header intact,
 ;0=default/replace .11 header with .3
 02h A_UINT8 1 defragOnHost ;1=defragmentation is performed by host,
 ;0=performed by target <default>
 03h A_UINT8 1 reserved[1] ;alignment |
| --- |

**WMIcmd(F025h) - WMI_SET_THIN_MODE_CMD**

Parameters:

```

| 00h A_UINT8 1 enable ;0=default/normal mode, 1=operate in thin mode
 01h A_UINT8 3 reserved[3] |
| --- |

**WMIcmd(F026h) - WMI_SET_BT_WLAN_CONN_PRECEDENCE_CMD**

Parameters:

```

| 00h A_UINT8 1 precedence; |
| --- |

BT_WLAN_CONN_PRECEDENCE values:

```

| BT_WLAN_CONN_PRECDENCE_WLAN = 0 ;default
 BT_WLAN_CONN_PRECDENCE_PAL = 1 |
| --- |

Unknown purpose. Maybe related to BT=Bluetooth? CONN=Connect? PAL=What?

**WMIcmd(F03Fh) - WMI_CONFIG_TX_MAC_RULES_CMD**

Parameters:

```

| 00h A_UINT32 4 rules ;combination of WMI_WRT_xxx values (see "wmi_thin.h") |
| --- |

**WMIcmd(F040h) - WMI_SET_PROMISCUOUS_MODE_CMD**

Parameters:

```

| 00h A_UINT8 1 enable (0=default/normal mode, 1=promiscuous mode) |
| --- |

**WMIcmd(F041h) - WMI_RX_FRAME_FILTER_CMD**

Parameters:

```

| 00h A_UINT16 2 filtermask(0) ;WMI_FILTERMASK_MGMT
 02h A_UINT16 2 filtermask(1) ;WMI_FILTERMASK_CTRL
 04h A_UINT16 2 filtermask(2) ;WMI_FILTERMASK_DATA
 06h A_UINT16 2 reserved ;alignment |
| --- |

**WMIcmd(F042h) - WMI_SET_CHANNEL_CMD**

Parameters:

```

| 00h A_UINT16 2 channel ;frequency in MHz
 -- //A_UINT8 - mode ;outcommented (HT20 or HT40 flag?)
 -- //A_UINT8 - secondary ;outcommented (HT40 2nd channel above/below flag?) |
| --- |

See also: WMI_SET_CHANNEL_EVENT

**WMIevent(9000h) - WMI_SET_CHANNEL_EVENT**

Event Data:

```

| 00h A_UINT8 1 result ;WMI_SET_CHANNEL_RES (or WMI_THIN_JOIN_RESULT??)
 01h A_UINT8 3 reserved[3] ;alignment |
| --- |

WMI_SET_CHANNEL_RES values:

```

| WMI_SET_CHANNEL_RES_SUCCESS = 0
 WMI_SET_CHANNEL_RES_FAIL = 1 |
| --- |

This is probably the reply to WMI_SET_CHANNEL_CMD (although official comments
claim it to be WMI_THIN_JOIN related; probably because of copying/pasting the
WMI_THIN_JOIN parameter structure without adjusting the comments).

**WMIcmd(F046h) - WMI_SET_DIV_PARAMS_CMD aka WMI_DIV_PARAMS_CMD**

Parameters:

```

| 00h A_UINT32 4 divIdleTime;
 04h A_UINT8 1 antRssiThresh;
 05h A_UINT8 1 divEnable;
 06h A_UINT16 2 active_treshold_rate; |
| --- |

**WMIcmd(F028h) - WMI_SET_PMK_CMD**

Parameters:

```

| 00h A_UINT8 20h pmk[WMI_PMK_LEN]; |
| --- |

WMI_PMK_LEN = 32

**WMIcmd(F047h) - WMI_GET_PMK_CMD ;reply?**

Parameters:

```

| Unknown (none?) |
| --- |

Reply: See WMI_GET_PMK_EVENT aka WMI_GET_PMK_REPLY

**WMIevent(102Ah) - WMI_GET_PMK_EVENT aka WMI_GET_PMK_REPLY**

Event Data:

```

| 00h A_UINT8 20h pmk[WMI_PMK_LEN]; |
| --- |

**WMIcmd(F048h) - WMI_SET_PASSPHRASE_CMD**

Parameters:

```

| 00h A_UCHAR 20h ssid[WMI_MAX_SSID_LEN];
 20h A_UINT8 40h passphrase[WMI_PASSPHRASE_LEN];
 60h A_UINT8 1 ssid_len;
 61h A_UINT8 1 passphrase_len; |
| --- |

WMI_PASSPHRASE_LEN = 64

**WMIcmd(F049h) - WMI_SEND_ASSOC_RES_CMD ;aka WMI_SEND_ASSOCRES_CMD**

Parameters:

```

| 00h A_UINT8 1 host_accept;
 01h A_UINT8 1 host_reasonCode;
 02h A_UINT8 1 target_status;
 03h A_UINT8 6 sta_mac_addr[ATH_MAC_LEN];
 09h A_UINT8 1 rspType; |
| --- |

**WMIcmd(F04Ah) - WMI_SET_ASSOC_REQ_RELAY_CMD ;aka WMI_SET_ASSOCREQ_RELAY**

Parameters:

```

| 00h A_UINT8 1 enable; |
| --- |

**WMIevent(9001h) - WMI_ASSOC_REQ_EVENT aka WMI_ASSOCREQ_EVENT**

Event Data:

```

| 00h A_UINT8 1 status;
 01h A_UINT8 1 rspType; |
| --- |

**WMIcmd(F04Bh or F04Dh) - WMI_ACS_CTRL_CMD ;aka WMI_ACS_CTRL_MSG**

Parameters:

```

| 00h A_UINT8 1 ctrl_id; /* control identifier (aka sub-command?)
 01h A_UINT8 1 length; /* number of bytes of data to follow
 02h A_UINT8 .. data[1]; /* start of control data |
| --- |

WMI_ACS_CTRL_HDR_LEN = (sizeof(WMI_ACS_CTRL_MSG) - sizeof(A_UINT8))

**WMIevent(9002h) - WMI_ACS_EVENT ;generic ACS event**

Event Data:

```

| 00h A_UINT8 1 event_id; /* event identifier
 01h A_UINT8 1 length; /* number of bytes of data that follows
 02h A_UINT8 .. data[1]; /* start of event data |
| --- |

WMI_ACS_EVENT_HDR_LEN = (sizeof(WMI_ACS_EVENT_MSG) - sizeof(A_UINT8))

**WMIcmd(F04Ch or F052h) - WMI_SET_EXCESS_TX_RETRY_THRES_CMD**

Parameters:

```

| 00h A_UINT32 4 threshold; |
| --- |

**WMIcmd(F061h or F051h) - WMI_FORCE_TARGET_ASSERT_CMD**

Parameters:

```

| Unknown (None?) |
| --- |

**WMIcmd(F04Dh or N/A) - WMI_SET_TBD_TIME_CMD ;added for wmiconfig cmd for TBD**

**WMIcmd(F04Eh or N/A) - WMI_PKTLOG_ENABLE_CMD**

**WMIcmd(F04Fh or N/A) - WMI_PKTLOG_DISABLE_CMD**

**WMIcmd(F062h or N/A) - WMI_SET_PROBED_SSID_EX_CMD**

**WMIcmd(F063h or N/A) - WMI_SET_NETWORK_LIST_OFFLOAD_CMD**

**WMIcmd(F064h or N/A) - WMI_SET_ARP_NS_OFFLOAD_CMD**

**WMIcmd(F065h or N/A) - WMI_ADD_WOW_EXT_PATTERN_CMD**

**WMIcmd(F066h or N/A) - WMI_GTK_OFFLOAD_OP_CMD**

**WMIcmd(F067h or N/A) - WMI_REMAIN_ON_CHNL_CMD**

**WMIcmd(F068h or N/A) - WMI_CANCEL_REMAIN_ON_CHNL_CMD**

**WMIcmd(F069h or N/A) - WMI_SEND_ACTION_CMD**

**WMIcmd(F06Ah or N/A) - WMI_PROBE_REQ_REPORT_CMD**

**WMIcmd(F06Bh or N/A) - WMI_DISABLE_11B_RATES_CMD**

**WMIcmd(F06Ch or N/A) - WMI_SEND_PROBE_RESPONSE_CMD**

Unknown/undocumented (invented 2012 or so).

**WMIevent(?) - WMI_GET_APPIE_CMD ;aka GET_APP_IE ;-not implemented in DSi**

The "GET_APP" command name suggests that there should be some reply, but:

```

| EVENTID is unknown (maybe 0040h, ie. same as GET_APPIE_CMD)
 Reply structure is unknown (maybe same parameter structure for SET_APPIE_CMD) |
| --- |

**WMIevent(?) - WMI_CRE_PRIORITY_STREAM_REPLY ;-not implemented in DSi**

Unknown crap. The DSi doesn't send a reply to WMI_CREATE_PSTREAM_CMD,
nonetheless, existing source code does have a WMI_CRE_PRIORITY_STREAM_REPLY
structure for whatever reason, maybe it's send only in certain firmware
version(s), with whatever/unknown WMIevent(xxxx) number. The reply structure
is:

```

| 00h A_UINT8 1 status; /* PSTREAM_REPLY_STATUS
 01h A_UINT8 1 txQueueNumber;
 02h A_UINT8 1 rxQueueNumber;
 03h A_UINT8 1 trafficClass;
 04h A_UINT8 1 trafficDirection; /* DIR_TYPE |
| --- |

PSTREAM_REPLY_STATUS values:

```

| A_SUCCEEDED = A_OK = 0
 A_FAILED_DELETE_STREAM_DOESNOT_EXIST = 250
 A_SUCCEEDED_MODIFY_STREAM = 251
 A_FAILED_INVALID_STREAM = 252
 A_FAILED_MAX_THINSTREAMS = 253
 A_FAILED_CREATE_REMOVE_PSTREAM_FIRST = 254 |
| --- |

**WMIevent(?) - WMI_DEL_PRIORITY_STREAM_REPLY ;-not implemented in DSi**

Unknown crap. See above for details. The WMI_DEL_PRIORITY_STREAM_REPLY
structure is:

```

| 00h A_UINT8 1 status; ;\
 01h A_UINT8 1 txQueueNumber; ; same as WMI_CRE_PRIORITY_STREAM_REPLY
 02h A_UINT8 1 rxQueueNumber; ;/
 03h A_UINT8 1 trafficDirection; ;\unlike WMI_CRE_PRIORITY_STREAM_REPLY
 04h A_UINT8 1 trafficClass; ;/(entries are swapped) |
| --- |

**WMIevent(?) - WMI_FRAME_RATES_REPLY ;-not implemented in DSi**

Unknown crap. WMI_FRAME_RATES_REPLY is said to have same structure as
WMI_SET_FRAMERATES_CMD parameter structure. But WMIevent(?) number is unknown,
and DSi doesn't seem to send any such REPLY.

**WMIevent(101Bh) - WMI_PEER_NODE_EVENT**

Event Data:

```

| 00h A_UINT8 1 eventCode;
 01h A_UINT8 6 peerMacAddr[ATH_MAC_LEN]; |
| --- |

Below PEER values are probably meant to be the "eventCode" values(?):

```

| PEER_NODE_JOIN_EVENT = 00h
 PEER_NODE_LEAVE_EVENT = 01h
 PEER_FIRST_NODE_JOIN_EVENT = 10h
 PEER_LAST_NODE_LEAVE_EVENT = 11h |
| --- |

**WMIevent(101Dh) - WMI_DTIMEXPIRY_EVENT**

Event Data:

```

| Unknown (if any) |
| --- |

**WMIevent(101Eh) - WMI_WLAN_VERSION_EVENT**

Event Data:

```

| 00h A_UINT32 4 version; |
| --- |

Whatever event with whatever version?

**WMIevent(1023h) - WMI_TX_COMPLETE_EVENT**

Event Data:

```

| 00h A_UINT8 1 numMessages ;number of tx comp msgs following
 01h A_UINT8 1 msgLen ;length in bytes for each individual msg following
 02h A_UINT8 1 msgType ;version of tx complete msg data following
 03h A_UINT8 1 reserved
 When msgType=01h=WMI_TXCOMPLETE_VERSION_1
 04h ... .. individual message(s) (see TX_COMPLETE_MSG_V1 structure)
 When msgType=Other
 04h ... .. reserved for other MSG types (none such defined yet) |
| --- |

TX_COMPLETE_MSG_V1 structure:

```

| 00h A_UINT8 1 status /* one of TX_COMPLETE_STATUS_xxx values
 01h A_UINT8 1 pktID /* packet ID to identify parent packet
 02h A_UINT8 1 rateIdx /* rate index on successful transmission
 03h A_UINT8 1 ackFailures /* number of ACK failures in tx attempt
 #if 0 ;optional "host delivery time" params currently ommitted...
 -- A_UINT32 queueDelay /* usec delay measured Tx Start time
 -- A_UINT32 mediaDelay /* usec delay measured ACK rx time
 #endif |
| --- |

TX_COMPLETE_STATUS_xxx values:

```

| TX_COMPLETE_STATUS_SUCCESS = 0
 TX_COMPLETE_STATUS_RETRIES = 1
 TX_COMPLETE_STATUS_NOLINK = 2
 TX_COMPLETE_STATUS_TIMEOUT = 3
 TX_COMPLETE_STATUS_OTHER = 4 |
| --- |

Transmit complete event.

**WMIevent(1025h) - WMI_ACL_DATA_EVENT**

Event Data:

```

| Unknown (what?) |
| --- |

ACL is what? Is that somehow related to "ACLCOEX"?

**WMIevent(1026h, or formerly N/A, or N/A) - WMI_REPORT_SLEEP_STATE_EVENT**

Event Data:

```

| 00h A_UINT32 4 sleepState; |
| --- |

Values for "sleepState":

```

| WMI_REPORT_SLEEP_STATUS_IS_DEEP_SLEEP = 0
 WMI_REPORT_SLEEP_STATUS_IS_AWAKE = 1 |
| --- |

Names: WMI_REPORT_SLEEP_STATE_EVENT aka WMI_REPORT_SLEEP_STATUS

**WMIevent(1027h, or formerly 1026h, or N/A) - WMI_WAPI_REKEY_EVENT**

This event is added/removed randomly in different source code versions.

Event Data:

```

| 00h A_UINT8 1 type;
 01h A_UINT8 6 macAddr[ATH_MAC_LEN]; |
| --- |

Values (probably for above "type" field?):

```

| WAPI_REKEY_UCAST = 1
 WAPI_REKEY_MCAST = 2 |
| --- |

The numbering for WMIevent(1026h..1029h) does vary in older source versions
because WMI_REPORT_SLEEP_STATE_EVENT originally didn't exist, and
WMI_WAPI_REKEY_EVENT originally did exist only if "WAPI_ENABLE". Later source
code did always include WMI_WAPI_REKEY_EVENT, and even later code did re-remove
it completely.

Names: WMI_WAPI_REKEY_EVENT aka WMI_WAPIREKEY_EVENT

**WMIevent(1035h) - WMI_CCX_RM_STATUS_EVENT ;CCX Evants, uh, EvAntS?**

Event Data:

```

| 00h A_INT32 4 rm_type ;\one of these MIGHT be "WMI_CCX_RM_STATUS_TYPE" ?
 04h A_INT32 4 status ;/ |
| --- |

WMI_CCX_RM_STATUS_TYPE values (probably for the "rm_type" field?):

```

| WMI_CCX_RM_STATUS_UNKNOWN = 0
 WMI_CCX_RM_REPORT_SENT = 1
 WMI_CCX_RM_REFUSE_REPORT_SENT = 2 |
| --- |

Uh, "CCX" means... What?

Uh, "RM" means... maybe "radio_measurements" or What?

Maybe this is somehow related to "WMI_ENABLE_RM_CMD"?

**WMIevent(1045h) - WMI_SET_HOST_SLEEP_MODE_CMD_PROCESSED_EVENT**

Event Data:

```

| Unknown (if any?) |
| --- |

This event exists ONLY in source code from 2010, not in older code, and it's
removed in newer code from 2012.

Special event used to notify host that AR6003 has processed sleep command (aka
WMI_SET_HOST_SLEEP_MODE_CMD?) (needed to prevent a late incoming credit report
from crashing the system).

**WMIevent(9003h) - WMI_REPORT_WMM_PARAMS_EVENT**

Event Data:

```

| 00h wmm_params 6*4 wmm_params[4]; |
| --- |

"wmm_params" structure:

```

| 00h A_UINT8 1 acm; /* ACM parameter
 01h A_UINT8 1 aifsn; /* AIFSN parameters
 02h A_UINT8 1 logcwmin; /* cwmin in exponential form
 03h A_UINT8 1 logcwmax; /* cwmax in exponential form
 04h A_UINT16 2 txopLimit; /* txopLimit |
| --- |

**WMIcmd(?) - WMI_SET_ADHOC_BSSID_CMD**

Parameters:

```

| 00h A_UINT8 6 bssid[ATH_MAC_LEN]; |
| --- |

The above parameter structure is defined in "wmi.h", but there's no
WMIcmd(xxxxh) command ID for it. Maybe the command did exist only in older
versions (from dates before 2006)?

| DSi Atheros Wifi - Unimplemented WMI Bluetooth Coexistence (newer AR6002) |
| --- |

/*----------------------COMMON to AR6002 and AR6003 -------------------------*/

BT_PARAMS_SCO_PSPOLL_LATENCY values:

```

| BT_PARAM_SCO_PSPOLL_LATENCY_ONE_FOURTH = 1 ;aka 25%
 BT_PARAM_SCO_PSPOLL_LATENCY_HALF = 2 ;aka 50%
 BT_PARAM_SCO_PSPOLL_LATENCY_THREE_FOURTH = 3 ;aka 75% |
| --- |

BT_PARAMS_SCO_STOMP_RULES values:

```

| BT_PARAMS_SCO_STOMP_SCO_NEVER = 1
 BT_PARAMS_SCO_STOMP_SCO_ALWAYS = 2
 BT_PARAMS_SCO_STOMP_SCO_IN_LOWRSSI = 3 |
| --- |

BT_ANT_FRONTEND_CONFIG values:

```

| BT_ANT_TYPE_UNDEF = 0 ;aka "Disabled (default)"
 BT_ANT_TYPE_DUAL = 1
 BT_ANT_TYPE_SPLITTER = 2
 BT_ANT_TYPE_SWITCH = 3
 BT_ANT_TYPE_HIGH_ISO_DUAL = 4 ;<-- not in "code aurora" |
| --- |

BT_COLOCATED_DEV_TYPE values:

```

| BT_COLOCATED_DEV_BTS4020 = 0
 BT_COLCATED_DEV_CSR = 1
 BT_COLOCATED_DEV_VALKYRIE = 2 ;aka BT_COLOCATED_DEV_VALKYRIe |
| --- |

/*********************** Applicable to AR6002 ONLY ***************************/

**WMIcmd(003Bh) - WMI_SET_BT_STATUS_CMD ;AR6002 Bluetooth Coexistence only?**

Parameters (02h bytes):

```

| 00h A_UINT8 1 streamType; ;aka BT_STREAM_TYPE ?
 01h A_UINT8 1 status; ;aka BT_STREAM_STATUS ? |
| --- |

BT_STREAM_TYPE values:

```

| BT_STREAM_UNDEF = 0
 BT_STREAM_SCO = 1 ;SCO stream
 BT_STREAM_A2DP = 2 ;A2DP stream
 BT_STREAM_SCAN = 3 ;BT Discovery or Page ;\"Newer AR6002 from 2008-2010"
 BT_STREAM_ESCO = 4 ;Whatever ;/
 BT_STREAM_ALL = 5 ;Whatever ;-"Newer AR6002 from 2008 only" |
| --- |

BT_STREAM_STATUS values:

```

| BT_STATUS_UNDEF = 0
 BT_STATUS_START = 1 ;-renamed to BT_STATUS_ON in code from 2010
 BT_STATUS_STOP = 2 ;-renamed to BT_STATUS_OFF in code from 2010
 BT_STATUS_RESUME = 3 ;\defined in "Older/Newer AR6002 from 2008"
 BT_STATUS_SUSPEND = 4 ;/(not in "Newer AR6002 for 2010")
 BT_STATUS_SUSPEND_A2DP = 5 ;\defined in "Newer AR6002 from 2008")
 BT_STATUS_SUSPEND_SCO = 6 ; (not in "Older AR6002 for 2008")
 BT_STATUS_SUSPEND_ACL = 7 ; (not in "Newer AR6002 for 2010")
 BT_STATUS_SUSPEND_SCAN = 8 ;/ |
| --- |

AR6002 only? (although other comment says "COMMON to AR6002 and AR6003"?)

**WMIcmd(003Ch) - WMI_SET_BT_PARAMS_CMD ;AR6002 Bluetooth Coexistence only?**

Parameters (1Fh or 19h bytes, for "Newer AR6002" code from 2008 or 2010):

```

| 1Fh 19h <--- total size (1Fh for code from 2008, 19h for code from 2010)
 when paramType=1=BT_PARAM_SCO: ;SCO stream parameters (BT_PARAMS_SCO)
 00h 00h A_UINT32 4 numScoCyclesForceTrigger (Number SCO cycles after which
 force a pspoll, default=10)
 04h 04h A_UINT32 4 dataResponseTimeout (Timeout Waiting for Downlink pkt in
 response for ps-poll, default=10 ms)
 08h 08h A_UINT32 4 stompScoRules ;aka BT_PARAMS_SCO_STOMP_RULES ?
 0Ch 0Ch A_UINT32 4 scoOptFlags (SCO Options Flags)
 10h -- A_UINT32 4 p2lrpOptModeBound ;\PacketToLowRatePacketRatio's
 14h -- A_UINT32 4 p2lrpNonOptModeBound ;/
 18h 10h A_UINT8 1 stompDutyCyleVal (SCO cycles to limit ps-poll queuing
 if stomped)
 19h 11h A_UINT8 1 stompDutyCyleMaxVal (firmware increases stomp duty cycle
 gradually uptill this value on need basis)
 1Ah 12h A_UINT8 1 psPollLatencyFraction (Fraction of idle period, within
 which additional ps-polls can be queued)
 1Bh 13h A_UINT8 1 noSCOSlots (Number of SCO Tx/Rx slots. HVx,EV3,2EV3=2)
 1Ch 14h A_UINT8 1 noIdleSlots (Number of Bluetooth idle slots between
 consecutive SCO Tx/Rx slots. HVx,EV3=4, 2EV3=10)
 1Dh -- A_UINT8 1 reserved8 (maintain word algnment) (uh, really?)
 -- 15h A_UINT8 1 scoOptOffRssi (RSSI value below which we go to ps poll)
 -- 16h A_UINT8 1 scoOptOnRssi (RSSI value above which we reenter opt mode)
 -- 17h A_UINT8 1 scoOptRtsCount
 when paramType=2=BT_PARAM_A2DP: ;whatever (BT_PARAMS_A2DP)
 00h 00h A_UINT32 4 a2dpWlanUsageLimit (MAX time firmware uses the medium for
 wlan, after it identifies the idle time, default=30 ms)
 04h 04h A_UINT32 4 a2dpBurstCntMin (Minimum number of bluetooth data frames
 to replenish Wlan Usage limit, default 3)
 08h 08h A_UINT32 4 a2dpDataRespTimeout
 0Ch 0Ch A_UINT32 4 a2dpOptFlags (A2DP Option flags)
 10h -- A_UINT32 4 p2lrpOptModeBound ;\PacketToLowRatePacketRatio's
 14h -- A_UINT32 4 p2lrpNonOptModeBound ;/
 18h -- A_UINT16 2 reserved16 (maintain word alignment)
 1Ah 10h A_UINT8 1 isCoLocatedBtRoleMaster
 1Bh -- A_UINT8 1 reserved8 (maintain word alignment)
 1Ch -- PAD 2 undefined/padding
 -- 11h A_UINT8 1 a2dpOptOffRssi (RSSI value below which we go to ps poll)
 -- 12h A_UINT8 1 a2dpOptOnRssi(RSSI value above which we reenter opt mode)
 -- 13h A_UINT8 1 a2dpOptRtsCount
 -- 14h PAD 4 undefined/padding
 when paramType=3=BT_PARAM_ANTENNA_CONFIG:
 00h 00h A_UINT8 1 antType aka BT_ANT_FRONTEND_CONFIG
 01h -- PAD 1Dh undefined/padding
 -- 01h PAD 17h undefined/padding
 when paramType=4=BT_PARAM_COLOCATED_BT_DEVICE:
 00h 00h A_UINT8 1 coLocatedBtDev aka BT_COLOCATED_DEV_TYPE
 01h -- PAD 1Dh undefined/padding
 -- 01h PAD 17h undefined/padding
 when paramType=5=BT_PARAM_ACLCOEX: ;whatever (BT_PARAMS_ACLCOEX)
 ;During BT ftp/ BT OPP or any another data based acl profile on bluetooth
 ;(non a2dp).
 00h 00h A_UINT32 4 aclWlanMediumUsageTime (Wlan usage time during
 Acl (non-a2dp) coexistence, default=30 ms)
 04h 04h A_UINT32 4 aclBtMediumUsageTime (Bt usage time during
 acl coexistence, default=30 ms)
 08h 08h A_UINT32 4 aclDataRespTimeout
 0Ch 0Ch A_UINT32 4 aclDetectTimeout (ACL coexistence enabled if we get
 10 Pkts in X ms, default=100 ms)
 10h 10h A_UINT32 4 aclmaxPktCnt (No of ACL pkts to receive before
 enabling ACL coex)
 14h -- PAD 0Ah undefined/padding
 -- 14h PAD 4 undefined/padding
 when paramType=6=BT_PARAM_11A_SEPARATE_ANT:
 00h 00h UNKNOWN ? unknown (maybe same as antType ?)
 xxh -- PAD .. undefined/padding
 -- xxh PAD .. undefined/padding
 and, in all cases:
 1Eh 18h A_UINT8 1 paramType |
| --- |

Values for "scoOptFlags" and "a2dpOptFlags":

```

| Bit0 Allow Close Range Optimization ;\all versions
 Bit1 Force awake during close range ;/
 Bit2 If set use (host supplied) threshold ;\Newer AR6002
 Bit3..23 Unused ;/from 2008
 Bit2 If set use host supplied RSSI for OPT ;\
 Bit3 If set use host supplied RTS COUNT for OPT ; Newer AR6002
 Bit4..7 Unused ; from 2010
 Bit8..15 Low Data Rate Min Cnt ;
 Bit16..23 Low Data Rate Max Cnt ;/
 Bit24..31 Undocumented (unused?) ;-all versions |
| --- |

PacketToLowRatePacketRatio's (p2lrp) entries (in code from 2008 only):

```

| p2lrpOptModeBound: Minimum ratio required to STAY IN opt mode
 p2lrpNonOptModeBound: Minimum ratio required to SWITCH TO opt mode |
| --- |

| DSi Atheros Wifi - Unimplemented WMI Bluetooth Coexistence (AR6003) |
| --- |

**WMIcmd(003Bh) - WMI_SET_BT_STATUS_CMD ;AR6002 Bluetooth Coexistence only?**

This command does maybe exist for AR6003 too (conflicting comments claim that
it is for AR6002 only, or for both AR6002 and AR6003). See "Newer AR6002"
description for details.

Also possible that it's replaced by "WMI_SET_BTCOEX_BT_OPERATING_STATUS_CMD".

**WMIcmd(003Ch) - WMI_SET_BT_PARAMS_CMD ;AR6002 Bluetooth Coexistence only?**

This command is used for AR6002 only. On AR6003, it's replaced by the new
commands described below:

**WMIcmd(F02Ah) - WMI_SET_BTCOEX_FE_ANT_CMD**

Parameters:

```

| 00h A_UINT8 1 btcoexFeAntType
 1 - WMI_BTCOEX_FE_ANT_SINGLE for single antenna front end
 2 - WMI_BTCOEX_FE_ANT_DUAL for dual antenna front end
 (for isolations less 35dB, for higher isolation there
 is not need to pass this command).
 (not implemented) |
| --- |

WMI_BTCOEX_FE_ANT_TYPE values:

```

| WMI_BTCOEX_NOT_ENABLED = 0
 WMI_BTCOEX_FE_ANT_SINGLE = 1
 WMI_BTCOEX_FE_ANT_DUAL = 2
 WMI_BTCOEX_FE_ANT_DUAL_HIGH_ISO = 3
 WMI_BTCOEX_FE_ANT_BYPASS_MODE = 4
 WMI_BTCOEX_FE_ANT_COMBINE_MODE = 5 |
| --- |

Indicates front end antenna configuration. This command needs to be issued
right after initialization and after WMI_SET_BTCOEX_COLOCATED_BT_DEV_CMD.
AR6003 enables coexistence and antenna switching based on the configuration.

**WMIcmd(F02Bh) - WMI_SET_BTCOEX_COLOCATED_BT_DEV_CMD**

Parameters:

```

| 00h A_UINT8 1 btcoexCoLocatedBTdev; 1 - Qcom BT (3 -wire PTA)
 2 - CSR BT (3 wire PTA)
 3 - Atheros 3001 BT (3 wire PTA)
 4 - STE bluetooth (4-wire ePTA)
 5 - Atheros 3002 BT (4-wire MCI)
 default=3 (Atheros 3001 BT ) |
| --- |

Indicate the bluetooth chip to the firmware. Firmware can have different
algorithm based bluetooth chip type. Based on bluetooth device, different
coexistence protocol would be used.

**WMIcmd(F02Ch) - WMI_SET_BTCOEX_SCO_CONFIG_CMD**

Parameters:

```

| --------------- BTCOEX_SCO_CONFIG scoConfig;
 00h A_UINT32 4 scoSlots (Number of SCO Tx/Rx slots: HVx,EV3,2EV3 = 2)
 04h A_UINT32 4 scoIdleSlots (Number of Bluetooth idle slots between
 consecutive SCO Tx/Rx slots: HVx,EV3 = 4, 2EV3 = 10)
 08h A_UINT32 4 scoFlags; SCO Options Flags:
 Bit0 Allow Close Range Optimization
 Bit1 Is EDR capable or Not
 Bit2 IS Co-located Bt role Master
 Bit3 Firmware determines the periodicity of SCO
 0Ch A_UINT32 4 linkId (applicable to STE-BT - not used)
 --------------- BTCOEX_PSPOLLMODE_SCO_CONFIG scoPspollConfig;
 10h A_UINT32 4 scoCyclesForceTrigger (Number SCO cycles after which
 force a pspoll, default=10)
 14h A_UINT32 4 scoDataResponseTimeout (Timeout Waiting for Downlink pkt
 in response for ps-poll, default=20 ms)
 18h A_UINT32 4 scoStompDutyCyleVal (not implemented)
 1Ch A_UINT32 4 scoStompDutyCyleMaxVal (not implemented)
 20h A_UINT32 4 scoPsPollLatencyFraction (Fraction of idle period, within
 which additional ps-polls can be queued
 1 - 1/4 of idle duration
 2 - 1/2 of idle duration
 3 - 3/4 of idle duration
 default=2 (1/2)
 --------------- BTCOEX_OPTMODE_SCO_CONFIG scoOptModeConfig;
 24h A_UINT32 4 scoStompCntIn100ms (max number of SCO stomp in 100ms
 allowed in opt mode. If exceeds the configured value,
 switch to ps-poll mode, default=3)
 28h A_UINT32 4 scoContStompMax (max number of continous stomp allowed in
 opt mode. if excedded switch to pspoll mode, default=3)
 2Ch A_UINT32 4 scoMinlowRateMbps (Low rate threshold) |
| --- |

```

| 30h A_UINT32 4 scoLowRateCnt (number of low rate pkts (< scoMinlowRateMbps)
 allowed in 100 ms. If exceeded switch/stay to ps-poll mode,
 lower stay in opt mode, default=36)
 34h A_UINT32 4 scoHighPktRatio "(Total Rx pkts in 100 ms + 1)/((Total tx
 pkts in 100 ms - No of high rate pkts in 100 ms) + 1) in
 100 ms"
 if exceeded switch/stay in opt mode and if lower
 switch/stay in pspoll mode.
 default=5 (80% of high rates)
 38h A_UINT32 4 scoMaxAggrSize (Max number of Rx subframes allowed in this
 mode. (Firmware re-negogiates max number of aggregates if
 it was negogiated to higher value, default=1,
 Recommended value Basic rate headsets = 1, EDR (2-EV3) =4.
 --------------- BTCOEX_WLANSCAN_SCO_CONFIG scoWlanScanConfig;
 3Ch A_UINT32 4 scanInterval;
 40h A_UINT32 4 maxScanStompCnt; |
| --- |

Configure SCO parameters. These parameters would be used whenever firmware is
indicated of (e)SCO profile on bluetooth (via
WMI_SET_BTCOEX_BT_OPERATING_STATUS_CMD).

Configration of BTCOEX_SCO_CONFIG data structure are common configuration and
applies ps-poll mode and opt mode.

Ps-poll Mode - Station is in power-save and retrieves downlink data between sco
gaps.

Opt Mode - station is in awake state and access point can send data to station
any time.

BTCOEX_PSPOLLMODE_SCO_CONFIG - Configuration applied only during ps-poll mode.

BTCOEX_OPTMODE_SCO_CONFIG - Configuration applied only during opt mode.

```

| Aliases for "scoFlags":
 #define WMI_SCO_CONFIG_FLAG_ALLOW_OPTIMIZATION (1 << 0)
 #define WMI_SCO_CONFIG_FLAG_IS_EDR_CAPABLE (1 << 1)
 #define WMI_SCO_CONFIG_FLAG_IS_BT_MASTER (1 << 2)
 #define WMI_SCO_CONFIG_FLAG_FW_DETECT_OF_PER (1 << 3) |
| --- |

**WMIcmd(F02Dh) - WMI_SET_BTCOEX_A2DP_CONFIG_CMD**

Parameters:

```

| --------------- BTCOEX_A2DP_CONFIG a2dpConfig;
 00h A_UINT32 4 a2dpFlags; 2DP Option flags:
 Bit0 Allow Close Range Optimization
 Bit1 IS EDR capable
 Bit2 IS Co-located Bt role Master
 Bit3 a2dp traffic is high priority
 Bit4 Fw detect the role of bluetooth.
 04h A_UINT32 4 linkId (Applicable only to STE-BT - not used)
 --------------- BTCOEX_PSPOLLMODE_A2DP_CONFIG a2dppspollConfig;
 08h A_UINT32 4 a2dpWlanMaxDur (MAX time firmware uses the medium for
 wlan, after it identifies the idle time, default=30 ms)
 0Ch A_UINT32 4 a2dpMinBurstCnt (Minimum number of bluetooth data frames
 to replenish Wlan Usage limit, default=3)
 10h A_UINT32 4 a2dpDataRespTimeout (Max duration firmware waits for
 downlink by stomping on bluetooth after ps-poll is
 acknowledged, default=20 ms)
 --------------- BTCOEX_OPTMODE_A2DP_CONFIG a2dpOptConfig;
 14h A_UINT32 4 a2dpMinlowRateMbps (Low rate threshold)
 18h A_UINT32 4 a2dpLowRateCnt (number of low rate pkts
 (<a2dpMinlowRateMbps) allowed in 100 ms.
 If exceeded switch/stay to ps-poll mode, lower stay in
 opt mode, default=36)
 1Ch A_UINT32 4 a2dpHighPktRatio "(Total Rx pkts in 100 ms + 1)/
 ((Total tx pkts in 100 ms - No of high rate pkts in 100 ms)
 + 1) in 100 ms", if exceeded switch/stay in opt mode and
 if lower switch/stay in pspoll mode.
 default=5 (80% of high rates)
 20h A_UINT32 4 a2dpMaxAggrSize (Max number of Rx subframes allowed in this
 mode. (Firmware re-negogiates max number of aggregates if
 it was negogiated to higher value, default=1.
 Recommended value Basic rate headsets = 1, EDR (2-EV3) =8)
 24h A_UINT32 4 a2dpPktStompCnt (number of a2dp pkts that can be stomped
 per burst, default=6) |
| --- |

Configure A2DP profile parameters. These parameters would be used whenver
firmware is indicated of A2DP profile on bluetooth (via
WMI_SET_BTCOEX_BT_OPERATING_STATUS_CMD).

Configuration of BTCOEX_A2DP_CONFIG data structure are common configuration and
applies to ps-poll mode and opt mode.

Ps-poll Mode - Station is in power-save and retrieves downlink data between
a2dp data bursts.

Opt Mode - station is in power save during a2dp bursts and awake in the gaps.

BTCOEX_PSPOLLMODE_A2DP_CONFIG - Configuration applied only during ps-poll mode.

BTCOEX_OPTMODE_A2DP_CONFIG - Configuration applied only during opt mode.

```

| Aliases for "a2dpFlags":
 #define WMI_A2DP_CONFIG_FLAG_ALLOW_OPTIMIZATION (1 << 0)
 #define WMI_A2DP_CONFIG_FLAG_IS_EDR_CAPABLE (1 << 1)
 #define WMI_A2DP_CONFIG_FLAG_IS_BT_ROLE_MASTER (1 << 2)
 #define WMI_A2DP_CONFIG_FLAG_IS_A2DP_HIGH_PRI (1 << 3)
 #define WMI_A2DP_CONFIG_FLAG_FIND_BT_ROLE (1 << 4) |
| --- |

**WMIcmd(F02Eh) - WMI_SET_BTCOEX_ACLCOEX_CONFIG_CMD**

Parameters:

```

| --------------- BTCOEX_ACLCOEX_CONFIG aclCoexConfig;
 00h A_UINT32 4 aclWlanMediumDur (Wlan usage time during Acl (non-a2dp)
 coexistence, default=30 ms)
 04h A_UINT32 4 aclBtMediumDur (Bt usage time during acl coexistence,
 default=30 ms)
 08h A_UINT32 4 aclDetectTimeout (BT activity observation time limit.
 In this time duration, number of bt pkts are counted.
 If the Cnt reaches "aclPktCntLowerLimit" value for
 "aclIterToEnableCoex" iteration continuously, firmware gets
 into ACL coexistence mode. Similarly, if bt traffic count
 during ACL coexistence has not reached "aclPktCntLowerLimit"
 continuously for "aclIterToEnableCoex", then ACL coexistence
 is disabled, default=100 ms)
 0Ch A_UINT32 4 aclPktCntLowerLimit (Acl Pkt Cnt to be received in duration
 of "aclDetectTimeout" for "aclIterForEnDis" times to
 enabling ACL coex. Similar logic is used to disable acl
 coexistence. (If "aclPktCntLowerLimit" cnt of acl pkts
 are not seen by the for "aclIterForEnDis" then acl
 coexistence is disabled), default=10)
 10h A_UINT32 4 aclIterForEnDis (number of Iteration of
 "aclPktCntLowerLimit" for Enabling and Disabling Acl
 Coexistence, default=3)
 14h A_UINT32 4 aclPktCntUpperLimit (This is upperBound limit, if there is
 more than "aclPktCntUpperLimit" seen in "aclDetectTimeout",
 ACL coexistence is enabled right away, default=15)
 18h A_UINT32 4 aclCoexFlags A2DP Option flags:
 Bit0 Allow Close Range Optimization
 Bit1 disable Firmware detection
 (Currently supported configuration is aclCoexFlags=0)
 1Ch A_UINT32 4 linkId; ;Applicable only for STE-BT - not used
 --------------- BTCOEX_PSPOLLMODE_ACLCOEX_CONFIG aclCoexPspollConfig;
 20h A_UINT32 4 aclDataRespTimeout (Max duration firmware waits for downlink
 by stomping on bluetooth after ps-poll is acknowledged,
 default=20 ms)
 --------------- BTCOEX_OPTMODE_ACLCOEX_CONFIG aclCoexOptConfig;
 24h A_UINT32 4 aclCoexMinlowRateMbps ;\
 28h A_UINT32 4 aclCoexLowRateCnt ;
 2Ch A_UINT32 4 aclCoexHighPktRatio ; Not implemented yet
 30h A_UINT32 4 aclCoexMaxAggrSize ;
 34h A_UINT32 4 aclPktStompCnt ;/ |
| --- |

Configure non-A2dp ACL profile parameters.The starts of ACL profile can either
be indicated via WMI_SET_BTCOEX_BT_OPERATING_STATUS_CMD or enabled via firmware
detection which is configured via "aclCoexFlags".

Configration of BTCOEX_ACLCOEX_CONFIG data structure are common configuration
and applies ps-poll mode and opt mode.

Ps-poll Mode - Station is in power-save and retrieves downlink data during wlan
medium.

Opt Mode - station is in power save during bluetooth medium time and awake
during wlan duration.

```

| (Not implemented yet) (uh, what?) |
| --- |

BTCOEX_PSPOLLMODE_ACLCOEX_CONFIG - Configuration applied only during ps-poll
mode.

BTCOEX_OPTMODE_ACLCOEX_CONFIG - Configuration applied only during opt mode.

```

| Aliases for "aclCoexFlags":
 #define WMI_ACLCOEX_FLAGS_ALLOW_OPTIMIZATION (1 << 0)
 #define WMI_ACLCOEX_FLAGS_DISABLE_FW_DETECTION (1 << 1) |
| --- |

**WMIcmd(F02Fh) - WMI_SET_BTCOEX_BTINQUIRY_PAGE_CONFIG_CMD**

Parameters:

```

| 00h A_UINT32 4 btInquiryDataFetchFrequency (The frequency of querying the
 AP for data (via pspoll) is configured by this parameter,
 default=10 ms)
 04h A_UINT32 4 protectBmissDurPostBtInquiry (The firmware will continue to
 be in inquiry state for configured duration, after inquiry
 completion. This is to ensure other bluetooth transactions
 (RDP, SDP profiles, link key exchange, etc.) goes through
 smoothly without wifi stomping, default=10 secs)
 08h A_UINT32 4 maxpageStomp (Applicable only for STE-BT interface.
 Currently not used)
 0Ch A_UINT32 4 btInquiryPageFlag (Not used) |
| --- |

Configuration parameters during bluetooth inquiry and page. Page configuration
is applicable only on interfaces which can distinguish page (applicable only
for ePTA - STE bluetooth).

Bluetooth inquiry start and end is indicated via
WMI_SET_BTCOEX_BT_OPERATING_STATUS_CMD. During this the station will be
power-save mode.

**WMIcmd(F030h) - WMI_SET_BTCOEX_DEBUG_CMD**

Parameters:

```

| 00h A_UINT32 4 btcoexDbgParam1 ;\
 04h A_UINT32 4 btcoexDbgParam2 ; Used for firmware development
 08h A_UINT32 4 btcoexDbgParam3 ; and debugging
 0Ch A_UINT32 4 btcoexDbgParam4 ;
 10h A_UINT32 4 btcoexDbgParam5 ;/ |
| --- |

**WMIcmd(F031h) - WMI_SET_BTCOEX_BT_OPERATING_STATUS_CMD**

Parameters:

```

| 00h A_UINT32 4 btProfileType (1=SCO, 2=A2DP, 3=INQUIRY_PAGE, 4=ACLCOEX)
 04h A_UINT32 4 btOperatingStatus ;aka BT_STREAM_STATUS on AR6002 ?
 08h A_UINT32 4 btLinkId |
| --- |

WMI_BTCOEX_BT_PROFILE values:

```

| WMI_BTCOEX_BT_PROFILE_SCO = 1
 WMI_BTCOEX_BT_PROFILE_A2DP = 2
 WMI_BTCOEX_BT_PROFILE_INQUIRY_PAGE = 3
 WMI_BTCOEX_BT_PROFILE_ACLCOEX = 4 |
| --- |

This command is probably equivalent to WMI_SET_BT_STATUS_CMD on AR6002.

**WMIcmd(F032h) - WMI_GET_BTCOEX_STATS_CMD ;reply WMI_REPORT_BTCOEX_STATS_EVENT**

Parameters:

```

| Unknown (none?) |
| --- |

**WMIcmd(F033h) - WMI_GET_BTCOEX_CONFIG_CMD ;reply WMI_REPORT_BTCOEX_CONFIG_EV.**

Parameters:

```

| 00h A_UINT32 4 btProfileType (1=SCO, 2=A2DP, 3=INQUIRY_PAGE, 4=ACLCOEX)
 04h A_UINT32 4 linkId (not used) (reserved/dummy?) |
| --- |

Command to firmware to get configuration parameters of the bt profile reported
via WMI_BTCOEX_CONFIG_EVENTID.

**WMIevent(1029h, or formerly 1028h, or 1027h) - WMI_REPORT_BTCOEX_CONFIG_EVENT**

Event Data:

```

| 00h A_UINT32 4 btProfileType (1=SCO, 2=A2DP, 3=INQUIRY_PAGE, 4=ACLCOEX)
 04h A_UINT32 4 linkId (not used)
 PREPACK union -- below are same as parameters from corresponding CMD's:
 08h .. WMI_SET_BTCOEX_SCO_CONFIG_CMD scoConfigCmd;
 08h .. WMI_SET_BTCOEX_A2DP_CONFIG_CMD a2dpConfigCmd;
 08h .. WMI_SET_BTCOEX_ACLCOEX_CONFIG_CMD aclcoexConfig;
 08h .. WMI_SET_BTCOEX_BTINQUIRY_PAGE_CONFIG_CMD btinquiryPageConfigCmd; |
| --- |

Event from firmware to host, sent in response to WMI_GET_BTCOEX_CONFIG_CMD.

**WMIevent(1028h, or formerly 1027h, or 1026h) - WMI_REPORT_BTCOEX_STATS_EVENT**

Event Data:

```

| --------------- BTCOEX_GENERAL_STATS coexStats;
 00h A_UINT32 4 highRatePktCnt;
 04h A_UINT32 4 firstBmissCnt;
 08h A_UINT32 4 psPollFailureCnt;
 0Ch A_UINT32 4 nullFrameFailureCnt;
 10h A_UINT32 4 optModeTransitionCnt;
 --------------- BTCOEX_SCO_STATS scoStats;
 14h A_UINT32 4 scoStompCntAvg;
 18h A_UINT32 4 scoStompIn100ms;
 1Ch A_UINT32 4 scoMaxContStomp;
 20h A_UINT32 4 scoAvgNoRetries;
 24h A_UINT32 4 scoMaxNoRetriesIn100ms;
 --------------- BTCOEX_A2DP_STATS a2dpStats;
 28h A_UINT32 4 a2dpBurstCnt;
 2Ch A_UINT32 4 a2dpMaxBurstCnt;
 30h A_UINT32 4 a2dpAvgIdletimeIn100ms;
 34h A_UINT32 4 a2dpAvgStompCnt;
 --------------- BTCOEX_ACLCOEX_STATS aclCoexStats;
 38h A_UINT32 4 aclPktCntInBtTime;
 3Ch A_UINT32 4 aclStompCntInWlanTime;
 40h A_UINT32 4 aclPktCntIn100ms; |
| --- |

Used for firmware development and debugging.

Names: WMI_GET_BTCOEX_STATS aka WMI_REPORT_BTCOEX_STATS_EVENT aka
WMI_REPORT_BTCOEX_BTCOEX_STATS_EVENT

| DSi Atheros Wifi - Unimplemented WMI DataSet Functions |
| --- |

Not implemented in DSi.

The "DataSet" feature allows the firmware to read external data from host
memory for whatever purpose (maybe intended for cases where the wifi board
doesn't contain an EEPROM, or for cases where Xtensa RAM/ROM is too small to
hold the whole firmware, or just to make it easier to modify data for
testing/debugging).

For the "DataSet" stuff, the normal Command/Response flow is reversed: the
firmware does send an EVENT to request data, and the host must respond by a
REPLY_CMD (except for the CLOSE_EVENT, which requires no reply).

**WMIevent(1010h:3001h) - WMIX_DSETOPENREQ_EVENT**

```

| 00h A_UINT32 4 dset_id ;-ID of requested DataSet (see "dsetid.h")
 04h A_UINT32 4 targ_dset_handle ;\to be echo'ed in REPLY_CMD
 08h A_UINT32 4 targ_reply_fn ; (host doesn't need to deal with this)
 0Ch A_UINT32 4 targ_reply_arg ;/ |
| --- |

DataSet Open Request Event. The host should open the DataSet and send a
WMIX_DSETOPEN_REPLY_CMD.

**WMIevent(1010h:3003h) - WMIX_DSETDATAREQ_EVENT**

```

| 00h A_UINT32 4 access_cookie ;-some kind of "filehandle" on host side
 04h A_UINT32 4 offset ;\source offset & length of requested data
 08h A_UINT32 4 length ;/
 0Ch A_UINT32 4 targ_buf ;\to be echo'ed in REPLY_CMD
 10h A_UINT32 4 targ_reply_fn ; (host doesn't need to deal with this)
 14h A_UINT32 4 targ_reply_arg ;/ |
| --- |

DataSet Data Request Event. The host should send the requested data via
WMIX_DSETDATA_REPLY_CMD.

**WMIevent(1010h:3002h) - WMIX_DSETCLOSE_EVENT**

```

| 00h A_UINT32 4 access_cookie ;-some kind of "filehandle" on host side |
| --- |

DataSet Close Event. The host should close the DataSet (and doesn't need to
send any REPLY_CMD).

**WMIcmd(002Eh:2001h) - WMIX_DSETOPEN_REPLY_CMD**

```

| 00h A_UINT32 4 status ;-what status ?
 04h A_UINT32 4 targ_dset_handle ;\
 08h A_UINT32 4 targ_reply_fn ; to be echo'ed from open EVENT
 0Ch A_UINT32 4 targ_reply_arg ;/
 10h A_UINT32 4 access_cookie ;-some kind of "filehandle" on host side
 14h A_UINT32 4 size ;-what size ?
 18h A_UINT32 4 version ;-what version ? |
| --- |

This REPLY_CMD should be send in response to WMIX_DSETOPENREQ_EVENTs.

**WMIcmd(002Eh:2002h) - WMIX_DSETDATA_REPLY_CMD**

```

| 00h A_UINT32 4 status ;-what status ?
 04h A_UINT32 4 targ_buf ;\
 08h A_UINT32 4 targ_reply_fn ; to be echo'ed from data EVENT
 0Ch A_UINT32 4 targ_reply_arg ;/
 10h A_UINT32 4 length ;\requested data
 14h A_UINT8 LEN buf[length] ;/ |
| --- |

This REPLY_CMD should be send in response to WMIX_DSETDATAREQ_EVENTs.

| DSi Atheros Wifi - Unimplemented WMI AP Mode Functions (exists on 3DS) |
| --- |

**AP Mode**

Whatever that is... probably something where the AR600x acts as Access Point
(AP) for other Stations (STA).

The DSi doesn't support AP Mode stuff, however, 3DS firmware type4 does support
most of them (but with F00Ah..F013h renumbered to 004Ah..0052h).

**AP Mode definitions**

Changing the following values needs compilation of both driver and firmware.

```

| AP_MAX_NUM_STA = 4 ;for old AR6002_REV2 version
 AP_MAX_NUM_STA = 10 ;for newer versions
 NUM_DEV = 3 ;Maximum no. of virtual interface supported
 NUM_CONN = (AP_MAX_NUM_STA + NUM_DEV)
 AP_ACL_SIZE = 10
 IEEE80211_MAX_IE = 256
 MCAST_AID = 0FFh ;Spl. AID used to set DTIM flag in the beacons
 DEF_AP_COUNTRY_CODE = "US "
 DEF_AP_WMODE_G = WMI_11G_MODE
 DEF_AP_WMODE_AG = WMI_11AG_MODE
 DEF_AP_DTIM = 5
 DEF_BEACON_INTERVAL = 100
 AP_DISCONNECT_STA_LEFT = 101 ;\
 AP_DISCONNECT_FROM_HOST = 102 ;
 AP_DISCONNECT_COMM_TIMEOUT = 103 ; AP mode disconnect reasons
 AP_DISCONNECT_MAX_STA = 104 ; (101..107 decimal):
 AP_DISCONNECT_ACL = 105 ;
 AP_DISCONNECT_STA_ROAM = 106 ;
 AP_DISCONNECT_DFS_CHANNEL = 107 ;/ |
| --- |

**WMIcmd(F00Bh or 004Ah) - WMI_AP_HIDDEN_SSID_CMD**

Parameters:

```

| 00h A_UINT8 1 hidden_ssid; |
| --- |

#define HIDDEN_SSID_FALSE = 0

#define HIDDEN_SSID_TRUE = 1

**WMIcmd(F00Ch or 004Bh) - WMI_AP_SET_NUM_STA_CMD aka WMI_AP_NUM_STA_CMD**

Parameters:

```

| 00h A_UINT8 1 num_sta; |
| --- |

**WMIcmd(F00Dh or 004Ch) - WMI_AP_ACL_POLICY_CMD**

Parameters:

```

| 00h A_UINT8 1 policy;
 #define AP_ACL_DISABLE = 00h
 #define AP_ACL_ALLOW_MAC = 01h
 #define AP_ACL_DENY_MAC = 02h
 #define AP_ACL_RETAIN_LIST_MASK = 80h |
| --- |

**WMIcmd(F00Eh or 004Dh) - WMI_AP_ACL_MAC_LIST_CMD aka WMI_AP_ACL_MAC_CMD**

Parameters:

```

| 00h A_UINT8 1 action;
 01h A_UINT8 1 index;
 02h A_UINT8 6 mac[ATH_MAC_LEN];
 08h A_UINT8 1 wildcard; |
| --- |

ADD_MAC_ADDR = 1

DEL_MAC_ADDR = 2

There is also a "WMI_AP_ACL" structure with unknown purpose (maybe internally
used in Xtensa memory or whatever):

```

| A_UINT16 2 index;
 A_UINT8 ... acl_mac[AP_ACL_SIZE][ATH_MAC_LEN];
 A_UINT8 .. wildcard[AP_ACL_SIZE];
 A_UINT8 1 policy; |
| --- |

**WMIcmd(F00Fh or 004Eh) - WMI_AP_CONFIG_COMMIT_CMD**

Unknown/undocumented? Except, see this:

sp.chip1stop.com/sp/wp-content/uploads/2015/05/gt202wifimoduleapiguidev13.pdf

Parameters: 34h bytes on 3DS.

Command Parameters: WMI_AP_CONFIG_COMMIT_CMD

```

| Type Name Comment
 00h A_UINT8 networktype NETWORK_TYPE
 0x01 INFRA_NETWORK
 0x02 ADHOC_NETWORK
 0x04 ADHOC_CREATOR
 01h A_UINT8 dot11authmode DOT11_AUTH_MODE
 0x01 OPEN_AUTH
 0x02 SHARED_AUTH
 02h A_UINT8 authmode AUTH_MODE
 0x01 NONE_AUTH
 0x02 WPA_AUTH
 0x04 WPA2_AUTH
 0x08 WPA_PSK_AUTH
 0x10 WPA2_PSK_AUTH
 0x20 WPA_AUTH_CCKM
 0x40 WPA2_AUTH_CCKM
 03h A_UINT8 pairwiseCryptoType CRYPTO_TYPE
 0x01 NONE_CRYPT
 0x02 WEP_CRYPT
 0x04 TKIP_CRYPT
 0x08 AES_CRYPT
 04h A_UINT8 pairwiseCryptoLen Length in bytes. Valid when the type is
 WEP_CRYPT, otherwise this should be 0
 05h A_UINT8 groupCryptoType CRYPTO_TYPE
 06h A_UINT8 groupCryptoLen Length in bytes. Valid when the type is
 WEP_CRYPT, otherwise this should be 0
 07h A_UINT8 ssidLength SSID length for the AP mode
 08h A_UCHAR ssid[32] SSID value for the SoftAP mode
 28h A_UINT16 channel Channel in which the AP mode has to be started
 2Ah A_UINT8 bssid[6]
 30h A_UINT8 ctrl_flags WMI_CONNECT_CTRL_FLAGS_BITS
 0x0001 CONNECT_ASSOC_POLICY_USER Associative frames are sent using
 the policy specified by the CONNECT_SEND-_REASSOC flag
 0x0004 CONNECT_IGNORE_WPAx-_GROUP_CIPHER Ignore the WPAx group cipher
 for WPA/WPA2
 0x0040 CONNECT_DO_WPA_OFFLOAD Use the authenticator in the QCA4002(=chip)
 0x0100 CONNECT_WPS_FLAG Set to indicate that the AP will add WPS IE to
 its beacon
 0xFFFF Reset all control flags |
| --- |

Uh, the flags are said to be 8bit... but can be as large as FFFFh?

Also, 3DS seems to use 34h bytes total... so flags would be 32bit maybe?

Whereof, 3DS seems to be simply ignoring param [2Ah..33h].

**WMIcmd(F010h or 004Fh) - WMI_AP_SET_MLME_CMD**

Parameters:

```

| 00h A_UINT8 6 mac[ATH_MAC_LEN];
 06h A_UINT16 2 reason; /* 802.11 reason code
 08h A_UINT8 1 cmd; /* operation to perform |
| --- |

MLME Commands (aka above "cmd"):

```

| WMI_AP_MLME_ASSOC 1 /* associate station
 WMI_AP_DISASSOC 2 /* disassociate station
 WMI_AP_DEAUTH 3 /* deauthenticate station
 WMI_AP_MLME_AUTHORIZE 4 /* authorize station
 WMI_AP_MLME_UNAUTHORIZE 5 /* unauthorize station |
| --- |

**WMIcmd(F011h or 0050h) - WMI_AP_SET_PVB_CMD**

Parameters (06h bytes on 3DS, but other variant exists, too):

```

| 06h 08h <---- total size (on 3DS it's 06h)
 00h 00h A_BOOL 4 flag;
 -- 04h A_UINT16 2 rsvd;
 04h 06h A_UINT16 2 aid; |
| --- |

Note: The 3DS command list does support the 6-byte variant, but it seem to be
simply ignoring the command & parameters.

**WMIcmd(F012h or 0051h) - WMI_AP_CONN_INACT_CMD**

Parameters:

```

| 00h A_UINT32 4 period; |
| --- |

**WMIcmd(F013h or 0052h) - WMI_AP_PROT_SCAN_TIME_CMD**

Parameters:

```

| 00h A_UINT32 4 period_min;
 04h A_UINT32 4 dwell_ms; |
| --- |

```

| _____________________ Below doesn't seem to exist on 3DS _____________________ |
| --- |

**WMIcmd(F014h) - WMI_AP_SET_COUNTRY_CMD ;aka formerly WMI_SET_COUNTRY_CMD**

Parameters:

```

| 00h A_UCHAR 3 countryCode[3] ;two letter ASCII... plus ending 00h? |
| --- |

WMI_DISABLE_REGULATORY_CODE = "FF" ;uh, are that THREE chars?

**WMIcmd(F015h) - WMI_AP_SET_DTIM_CMD**

Parameters:

```

| 00h A_UINT8 1 dtim; ;wake up each N beacon interval units |
| --- |

DTIM interval used to check for multicast packets.

**WMIcmd(F016h or N/A) - WMI_AP_MODE_STAT_CMD**

Parameters:

```

| Unknown (if any) |
| --- |

Reply:

```

| Unknown (if any) |
| --- |

There is an "WMI_AP_MODE_STAT" structure (said to be an event, although there
is no WMIevent(xxxxh) number defined for it anywhere) (and, the "action" entry
sounds more like an command/parameter than like an event/response?):

WMI_AP_MODE_STAT structure:

```

| 00h A_UINT32 action;
 04h WMI_PER_STA_STAT sta[AP_MAX_NUM_STA]; |
| --- |

Mysterious values (probably for the "action" entry:

```

| AP_GET_STATS = 0
 AP_CLEAR_STATS = 1 |
| --- |

WMI_PER_STA_STAT structure (for the "sta" entries):

```

| 00h A_UINT32 tx_bytes;
 04h A_UINT32 tx_pkts;
 08h A_UINT32 tx_error;
 0Ch A_UINT32 tx_discard;
 10h A_UINT32 rx_bytes;
 14h A_UINT32 rx_pkts;
 18h A_UINT32 rx_error;
 1Ch A_UINT32 rx_discard;
 20h A_UINT32 aid; |
| --- |

**WMIcmd(F027h or N/A) - WMI_AP_SET_11BG_RATESET_CMD**

Parameters:

```

| 00h A_UINT8 1 rateset;
 AP_11BG_RATESET1 = 1
 AP_11BG_RATESET2 = 2
 DEF_AP_11BG_RATESET = AP_11BG_RATESET1 |
| --- |

**WMIcmd(F05Ah or F059h or N/A) - WMI_AP_SET_APSD_CMD**

Parameters:

```

| 00h A_UINT8 1 enable; |
| --- |

WMI_AP_APSD_DISABLED = 0

WMI_AP_APSD_ENABLED = 1

**WMIcmd(F05Bh or F05Ah or N/A) - WMI_AP_APSD_BUFFERED_TRAFFIC_CMD**

Parameters:

```

| 00h A_UINT16 2 aid;
 02h A_UINT16 2 bitmap;
 04h A_UINT32 4 flags; |
| --- |

WMI_AP_APSD_BUFFERED_TRAFFIC_FLAGS values (only one defined)

```

| WMI_AP_APSD_NO_DELIVERY_FRAMES_FOR_THIS_TRIGGER = 01h |
| --- |

**WMIcmd(F06Eh or N/A) - WMI_AP_JOIN_BSS_CMD**

Unknown/undocumented (some new command, invented in 2012 or so).

**WMIevent(?) - WMI_AP_MODE_STAT_EVENT ;reply to WMI_AP_MODE_STAT_CMD ?**

Maybe there is an event/reply for the "WMI_AP_MODE_STAT_CMD" command (see
above).

**WMIevent(101Ch) - WMI_PSPOLL_EVENT ;aka WMI_PS_POLL_EVENT ;AP mode related?**

Event Data:

```

| 00h A_UINT16 2 aid; |
| --- |

Whatever. Said to be an "AP mode event". Maybe WMI_SET_AP_PS_CMD related?

**WMIcmd(F0AFh) - WMI_AP_PSBUFF_OFFLOAD ;QCA4002 chipset on GT202 WiFi module**

```

| 00h A_UINT8 Enable
 0 Enable buffering mechanism in firmware to handle power save clients
 1 Disable buffering mechanism
 01h A_UINT8 psBufCount Range: 1-3,
 Specifies number of buffers allowed to buffer power save packets. |
| --- |

| DSi Atheros Wifi - Unimplemented WMI DFS Functions |
| --- |

**WMIcmd(F034h) - WMI_SET_DFS_ENABLE_CMD ;aka WMI_SET_DFS_CMD maybe ?**

**WMIcmd(F035h) - WMI_SET_DFS_MINRSSITHRESH_CMD ;aka WMI_SET_DFS_CMD too ??**

**WMIcmd(F036h) - WMI_SET_DFS_MAXPULSEDUR_CMD ;aka WMI_SET_DFS_CMD too ??**

Parameters:

```

| Unknown (maybe WMI_SET_DFS_CMD structure?) |
| --- |

There is a "WMI_SET_DFS_CMD" structure defined:

```

| 00h A_UINT8 1 enable; |
| --- |

Maybe that structure is meant to be used with one of the WMI_SET_DFS_xxx_CMD
commands, or maybe it's even meant to be used with ALL of that THREE commands.

```

| XXX see file "dfs_common.h" |
| --- |

**WMIcmd(F037h) - WMI_DFS_RADAR_DETECTED_CMD ;aka WMI_RADAR_DETECTED_CMD**

Parameters:

```

| 00h A_UINT16 2 chan_index;
 02h A_INT8 1 bang_radar; |
| --- |

**WMIevent(102Bh) - WMI_DFS_HOST_ATTACH_EVENT**

Event Data:

```

| 00h A_UINT64 8 ext_chan_busy_ts;
 08h A_UINT8 1 enable_ar;
 09h A_UINT8 1 enable_radar; |
| --- |

**WMIevent(102Ch) - WMI_DFS_HOST_INIT_EVENT**

Event Data:

```

| 00h A_UINT32 4 dfs_domain; |
| --- |

**WMIevent(102Dh) - WMI_DFS_RESET_DELAYLINES_EVENT**

**WMIevent(102Eh) - WMI_DFS_RESET_RADARQ_EVENT**

**WMIevent(102Fh) - WMI_DFS_RESET_AR_EVENT**

**WMIevent(1030h) - WMI_DFS_RESET_ARQ_EVENT**

**WMIevent(1031h) - WMI_DFS_SET_DUR_MULTIPLIER_EVENT**

**WMIevent(1032h) - WMI_DFS_SET_BANGRADAR_EVENT**

**WMIevent(1033h) - WMI_DFS_SET_DEBUGLEVEL_EVENT**

Event Data:

```

| Unknown (if any) (not defined in file "dfs_common.h") |
| --- |

**WMIevent(1034h) - WMI_DFS_PHYERR_EVENT**

Event Data:

```

| 00h A_UINT8 1 num_events;
 01h dfs_event_info .. ev_info[WMI_DFS_EVENT_MAX_BUFFER_SIZE]; |
| --- |

WMI_DFS_EVENT_MAX_BUFFER_SIZE = ((255)/sizeof(struct dfs_event_info))

Format of the above "dfs_event_info" structure:

```

| 00h A_UINT64 8 full_ts; /* 64-bit full timestamp from interrupt time
 08h A_UINT32 4 ts; /* Original 15 bit recv timestamp
 0Ch A_UINT32 4 ext_chan_busy; /* Ext chan busy %
 10h A_UINT8 1 rssi; /* rssi of radar event
 11h A_UINT8 1 dur; /* duration of radar pulse
 12h A_UINT8 1 chanindex; /* Channel of event
 13h A_UINT8 1 flags; |
| --- |

Values for "flags":

```

| PRIMARY_CH = 0 ;\flags.bit0
 EXT_CH = 1 ;/
 AR_EVENT = 0 ;\flags.bit1
 DFS_EVENT = 2 ;/ |
| --- |

Some more DFS related constants (unknown purpose):

```

| DFS_UNINIT_DOMAIN = 0 ;Uninitialized dfs domain
 DFS_FCC_DOMAIN = 1 ;FCC3 dfs domain
 DFS_ETSI_DOMAIN = 2 ;ETSI dfs domain
 DFS_MKK4_DOMAIN = 3 ;Japan dfs domain
 MAX_BIN5_DUR = 131 ;rounded from 131.25=(105*1.25) ;DFS related
 TRAFFIC_DETECTED = 1 ;whatever ;DFS related
 ATH_DEBUG_DFS = 00000100h ;Minimal DFS debug ;\
 ATH_DEBUG_DFS1 = 00000200h ;Normal DFS debug ; should match the
 ATH_DEBUG_DFS2 = 00000400h ;Maximal DFS debug ; table from if_ath.c
 ATH_DEBUG_DFS3 = 00000800h ;matched filterID display ;/ |
| --- |

| DSi Atheros Wifi - Unimplemented WMI P2P Functions |
| --- |

P2P module definitions

P2P_SSID structure:

```

| 00h A_UINT8 1 ssidLength;
 01h A_UINT8 20h ssid[WMI_MAX_SSID_LEN]; |
| --- |

**WMIcmd(F038h) - WMI_P2P_SET_CONFIG_CMD**

Parameters:

```

| 00h A_UINT8 1 go_intent;
 01h A_UINT8 3 country[3];
 04h A_UINT8 1 reg_class;
 05h A_UINT8 1 listen_channel;
 06h A_UINT8 1 op_reg_class;
 07h A_UINT8 1 op_channel;
 09h A_UINT16 2 config_methods; |
| --- |

**WMIcmd(F039h) - WMI_WPS_SET_CONFIG_CMD ;P2P related**

Parameters:

```

| 00h device_type_tuple 4 pri_dev_type;
 -- outcommented? 0 //A_UINT8 pri_device_type[8];
 04h device_type_tuple 4*5 sec_dev_type[MAX_P2P_SEC_DEVICE_TYPES];
 18h A_UINT8 10h uuid[WPS_UUID_LEN];
 28h A_UINT8 20h device_name[WPS_MAX_DEVNAME_LEN];
 48h A_UINT8 1 dev_name_len; |
| --- |

"device_type_tuple" structure (4 bytes):

```

| 00h A_UINT16 2 categ;
 02h A_UINT16 2 sub_categ; |
| --- |

MAX_P2P_SEC_DEVICE_TYPES = 5

WPS_UUID_LEN = 16

WPS_MAX_DEVNAME_LEN = 32

**WMIcmd(F03Ah) - WMI_SET_REQ_DEV_ATTR_CMD ;P2P related**

Parameters:

```

| 00h device_type_tuple 4 pri_dev_type;
 04h device_type_tuple 4*5 sec_dev_type[MAX_P2P_SEC_DEVICE_TYPES];
 18h A_UINT8 6 device_addr[ATH_MAC_LEN]; |
| --- |

**WMIcmd(F03Bh) - WMI_P2P_FIND_CMD**

Parameters:

typedef PREPACK struct {

```

| 00h A_UINT32 4 timeout;
 04h A_ENUM .. type; ;A_UINTx or so? ;aka WMI_P2P_DISC_TYPE |
| --- |

WMI_P2P_DISC_TYPE values:

```

| WMI_P2P_FIND_START_WITH_FULL = Unknown (0 or 1 or so)
 WMI_P2P_FIND_ONLY_SOCIAL = WMI_P2P_FIND_START_WITH_FULL+1
 WMI_P2P_FIND_PROGRESSIVE = WMI_P2P_FIND_START_WITH_FULL+2 |
| --- |

**WMIcmd(F03Ch) - WMI_P2P_STOP_FIND_CMD**

Parameters:

```

| Unknown (none?) |
| --- |

**WMIcmd(F03Dh) - WMI_P2P_GO_NEG_START_CMD**

Parameters:

```

| 00h A_UINT16 2 listen_freq;
 02h A_UINT16 2 force_freq;
 04h A_UINT16 2 go_oper_freq;
 06h A_UINT8 1 dialog_token;
 07h A_UINT8 6 peer_addr[ATH_MAC_LEN];
 0Dh A_UINT8 6 own_interface_addr[ATH_MAC_LEN];
 13h A_UINT8 6 member_in_go_dev[ATH_MAC_LEN];
 19h A_UINT8 1 go_dev_dialog_token;
 1Ah P2P_SSID 21h peer_go_ssid;
 3Bh A_UINT8 1 wps_method;
 3Ch A_UINT8 1 dev_capab;
 3Dh A_INT8 1 go_intent;
 3Eh A_UINT8 1 persistent_grp; |
| --- |

**WMIcmd(F03Eh) - WMI_P2P_LISTEN_CMD**

Parameters:

```

| 00h A_UINT32 4 timeout; |
| --- |

**WMIcmd(F050h or F053h) - WMI_P2P_GO_NEG_REQ_RSP_CMD**

Parameters:

```

| 000h A_UINT16 2 listen_freq;
 002h A_UINT16 2 force_freq;
 004h A_UINT8 1 status;
 005h A_INT8 1 go_intent;
 006h A_UINT8 200h wps_buf[512];
 206h A_UINT16 2 wps_buflen;
 208h A_UINT8 200h p2p_buf[512];
 408h A_UINT16 2 p2p_buflen;
 40Ah A_UINT8 1 dialog_token;
 40Bh A_UINT8 1 wps_method;
 40Ch A_UINT8 1 persistent_grp;
 40Dh A_UINT8 6 sa[ATH_MAC_LEN]; |
| --- |

**WMIcmd(F051h or F054h) - WMI_P2P_GRP_INIT_CMD**

Parameters:

```

| 00h A_UINT8 1 persistent_group;
 01h A_UINT8 1 group_formation; |
| --- |

**WMIcmd(F052h or F055h) - WMI_P2P_GRP_FORMATION_DONE_CMD**

Parameters:

```

| 00h A_UINT8 6 peer_addr[ATH_MAC_LEN];
 06h A_UINT8 1 grp_formation_status; |
| --- |

**WMIcmd(F053h or F056h) - WMI_P2P_INVITE_CMD**

Parameters:

```

| 00h A_ENUM .. role; ;A_UINTx or so? ;WMI_P2P_INVITE_ROLE
 .. A_UINT16 2 listen_freq;
 .. A_UINT16 2 force_freq;
 .. A_UINT8 1 dialog_token;
 .. A_UINT8 6 peer_addr[ATH_MAC_LEN];
 .. A_UINT8 6 bssid[ATH_MAC_LEN];
 .. A_UINT8 6 go_dev_addr[ATH_MAC_LEN];
 .. P2P_SSID 21h ssid;
 .. A_UINT8 1 is_persistent;
 .. A_UINT8 1 wps_method; |
| --- |

WMI_P2P_INVITE_ROLE values:

```

| WMI_P2P_INVITE_ROLE_GO = Unknown (0 or 1 or so)
 WMI_P2P_INVITE_ROLE_ACTIVE_GO = WMI_P2P_INVITE_ROLE_GO+1
 WMI_P2P_INVITE_ROLE_CLIENT = WMI_P2P_INVITE_ROLE_GO+2 |
| --- |

**WMIcmd(F054h or F057h) - WMI_P2P_INVITE_REQ_RSP_CMD**

Parameters:

```

| 000h A_UINT16 2 force_freq;
 002h A_UINT8 1 status;
 003h A_UINT8 1 dialog_token;
 004h A_UINT8 200h p2p_buf[512];
 204h A_UINT16 2 p2p_buflen;
 206h A_UINT8 1 is_go;
 207h A_UINT8 6 group_bssid[ATH_MAC_LEN]; |
| --- |

**WMIcmd(F055h or F058h) - WMI_P2P_PROV_DISC_REQ_CMD**

Parameters:

```

| 00h A_UINT16 2 wps_method;
 02h A_UINT16 2 listen_freq;
 04h A_UINT8 1 dialog_token;
 05h A_UINT8 6 peer[ATH_MAC_LEN];
 0Bh A_UINT8 6 go_dev_addr[ATH_MAC_LEN];
 11h P2P_SSID 21h go_oper_ssid; |
| --- |

**WMIcmd(F056h or F059h) - WMI_P2P_SET_CMD**

Parameters:

```

| 00h A_UINT8 1 config_id; ;set to one of WMI_P2P_CONF_ID
 When config_id=1=WMI_P2P_CONFID_LISTEN_CHANNEL ;WMI_P2P_LISTEN_CHANNEL
 01h A_UINT8 1 reg_class;
 02h A_UINT8 1 listen_channel;
 When config_id=2=WMI_P2P_CONFID_CROSS_CONNECT ;WMI_P2P_SET_CROSS_CONNECT
 01h A_UINT8 1 flag;
 When config_id=3=WMI_P2P_CONFID_SSID_POSTFIX ;WMI_P2P_SET_SSID_POSTFIX
 01h A_UINT8 17h ssid_postfix[WMI_MAX_SSID_LEN-9];
 18h A_UINT8 1 ssid_postfix_len;
 When config_id=4=WMI_P2P_CONFID_INTRA_BSS ;WMI_P2P_SET_INTRA_BSS
 01h A_UINT8 1 flag;
 When config_id=5=WMI_P2P_CONFID_CONCURRENT_MODE ;WMI_P2P_SET_CONCURRENT_MODE
 01h A_UINT8 1 flag;
 When config_id=6=WMI_P2P_CONFID_GO_INTENT ;WMI_P2P_SET_GO_INTENT
 01h A_UINT8 1 value;
 When config_id=7=WMI_P2P_CONFID_DEV_NAME ;WMI_P2P_SET_DEV_NAME
 01h A_UINT8 20h dev_name[WPS_MAX_DEVNAME_LEN];
 21h A_UINT8 1 dev_name_len; |
| --- |

**WMIcmd(F05Bh or F05Ch) - WMI_P2P_SDPD_TX_CMD**

Parameters:

```

| 000h A_UINT8 1 type;
 001h A_UINT8 1 dialog_token;
 002h A_UINT8 1 frag_id;
 003h A_UINT8 1 reserved1; /* alignment
 004h A_UINT8 6 peer_addr[ATH_MAC_LEN];
 00Ah A_UINT16 2 freq;
 00Ch A_UINT16 2 status_code;
 00Eh A_UINT16 2 comeback_delay;
 010h A_UINT16 2 tlv_length;
 012h A_UINT16 2 update_indic;
 014h A_UINT16 2 total_length;
 016h A_UINT16 2 reserved2; /* future
 018h A_UINT8 400h tlv[WMI_P2P_MAX_TLV_LEN]; |
| --- |

WMI_P2P_SDPD_TYPE values:

```

| WMI_P2P_SD_TYPE_GAS_INITIAL_REQ = 01h
 WMI_P2P_SD_TYPE_GAS_INITIAL_RESP = 02h
 WMI_P2P_SD_TYPE_GAS_COMEBACK_REQ = 03h
 WMI_P2P_SD_TYPE_GAS_COMEBACK_RESP = 04h
 WMI_P2P_PD_TYPE_RESP = 05h
 WMI_P2P_SD_TYPE_STATUS_IND = 06h |
| --- |

WMI_P2P_SDPD_TRANSACTION_STATUS values:

```

| WMI_P2P_SDPD_TRANSACTION_PENDING = 01h
 WMI_P2P_SDPD_TRANSACTION_COMP = 02h |
| --- |

WMI_P2P_MAX_TLV_LEN = 1024

**WMIcmd(F05Ch or F05Dh) - WMI_P2P_STOP_SDPD_CMD**

Parameters:

```

| Unknown (none?) |
| --- |

**WMIcmd(F05Dh or F05Eh) - WMI_P2P_CANCEL_CMD**

Parameters:

```

| Unknown (none?) |
| --- |

**WMIcmd(F06Dh or N/A) - WMI_GET_P2P_INFO_CMD**

Unknown/undocumented (invented around 2012).

...P2P Events...

**WMIevent(1036h) - WMI_P2P_GO_NEG_RESULT_EVENT**

Event Data:

```

| 00h A_UINT16 2 freq;
 02h A_INT8 1 status;
 03h A_UINT8 1 role_go;
 04h A_UINT8 20h ssid[WMI_MAX_SSID_LEN];
 24h A_UINT8 1 ssid_len;
 25h A_CHAR 9 pass_phrase[WMI_MAX_PASSPHRASE_LEN];
 2Eh A_UINT8 6 peer_device_addr[ATH_MAC_LEN];
 34h A_UINT8 6 peer_interface_addr[ATH_MAC_LEN];
 3Ah A_UINT8 1 wps_method;
 3Bh A_UINT8 1 persistent_grp; |
| --- |

WMI_MAX_PASSPHRASE_LEN = 9

**WMIevent(103Dh) - WMI_P2P_GO_NEG_REQ_EVENT**

Event Data:

```

| 000h A_UINT8 6 sa[ATH_MAC_LEN];
 006h A_UINT8 200h wps_buf[512];
 206h A_UINT16 2 wps_buflen;
 208h A_UINT8 200h p2p_buf[512];
 408h A_UINT16 2 p2p_buflen;
 40Ah A_UINT8 1 dialog_token; |
| --- |

**WMIevent(103Eh) - WMI_P2P_INVITE_REQ_EVENT**

Event Data:

```

| 000h A_UINT8 200h p2p_buf[512];
 200h A_UINT16 2 p2p_buflen;
 202h A_UINT8 6 sa[ATH_MAC_LEN];
 208h A_UINT8 6 bssid[ATH_MAC_LEN];
 20Eh A_UINT8 6 go_dev_addr[ATH_MAC_LEN];
 214h P2P_SSID 21h ssid;
 235h A_UINT8 1 is_persistent;
 236h A_UINT8 1 dialog_token; |
| --- |

**WMIevent(103Fh) - WMI_P2P_INVITE_RCVD_RESULT_EVENT**

Event Data:

```

| 00h A_UINT16 2 oper_freq;
 02h A_UINT8 6 sa[ATH_MAC_LEN];
 08h A_UINT8 6 bssid[ATH_MAC_LEN];
 0Eh A_UINT8 1 is_bssid_valid;
 0Fh A_UINT8 6 go_dev_addr[ATH_MAC_LEN];
 15h P2P_SSID 21h ssid;
 36h A_UINT8 1 status; |
| --- |

**WMIevent(1040h) - WMI_P2P_INVITE_SENT_RESULT_EVENT**

Event Data:

```

| 00h A_UINT8 1 status;
 01h A_UINT8 6 bssid[ATH_MAC_LEN];
 07h A_UINT8 1 is_bssid_valid; |
| --- |

**WMIevent(1041h) - WMI_P2P_PROV_DISC_RESP_EVENT**

Event Data:

```

| 00h A_UINT8 6 peer[ATH_MAC_LEN];
 06h A_UINT16 2 config_methods; |
| --- |

**WMIevent(1042h) - WMI_P2P_PROV_DISC_REQ_EVENT**

Event Data:

```

| 00h A_UINT8 6 sa[ATH_MAC_LEN];
 06h A_UINT16 2 wps_config_method;
 08h A_UINT8 6 dev_addr[ATH_MAC_LEN];
 0Eh A_UINT8 8 pri_dev_type[WPS_DEV_TYPE_LEN];
 16h A_UINT8 20h device_name[WPS_MAX_DEVNAME_LEN];
 36h A_UINT8 1 dev_name_len;
 37h A_UINT16 2 dev_config_methods;
 39h A_UINT8 1 device_capab;
 3Ah A_UINT8 1 group_capab; |
| --- |

WPS_DEV_TYPE_LEN = 8

**WMIevent(1043h) - WMI_P2P_START_SDPD_EVENT**

Event Data:

```

| Unknown (none?) |
| --- |

**WMIevent(1044h) - WMI_P2P_SDPD_RX_EVENT**

Event Data:

```

| 00h A_UINT8 1 type;
 01h A_UINT8 1 transaction_status;
 02h A_UINT8 1 dialog_token;
 03h A_UINT8 1 frag_id;
 04h A_UINT8 6 peer_addr[ATH_MAC_LEN];
 0Ah A_UINT16 2 freq;
 0Ch A_UINT16 2 status_code;
 0Eh A_UINT16 2 comeback_delay;
 10h A_UINT16 2 tlv_length;
 12h A_UINT16 2 update_indic;
 14h VAR .. Variable length TLV will be placed after the event |
| --- |

| DSi Atheros Wifi - Unimplemented WMI WAC Functions |
| --- |

**WMIcmd(F043h) - WMI_WAC_ENABLE_CMD aka WMI_ENABLE_WAC_CMD**

Parameters:

```

| 00h A_UINT32 4 period;
 04h A_UINT32 4 threshold;
 08h A_INT32 4 rssi;
 0Ch A_BOOL 4 enable;
 10h A_CHAR 8 wps_pin[8]; ;WPS related? |
| --- |

**WMIcmd(F044h) - WMI_WAC_SCAN_REPLY_CMD**

Parameters:

```

| 00h A_ENUM .. cmdid ;A_UINTx or so? (WAC_SUBCMD) |
| --- |

WAC_SUBCMD values:

```

| WAC_MORE_SCAN = -1
 WAC_SEND_PROBE_IDX = 0 |
| --- |

**WMIcmd(F045h) - WMI_WAC_CTRL_REQ_CMD**

Parameters:

```

| 00h A_UINT8 1 req; ;aka WAC_REQUEST_TYPE
 01h A_UINT8 1 cmd; ;aka WAC_COMMAND
 02h A_UINT8 1 frame; ;aka WAC_FRAME_TYPE
 03h A_UINT8 11h ie[17];
 14h A_INT32 4 status; ;aka WAC_STATUS |
| --- |

WAC related constants (from wac_defs.h):

WAC_REQUEST_TYPE values:

```

| WAC_SET = Unknown (0 or 1 or so)
 WAC_GET = WAC_SET+1 |
| --- |

WAC_COMMAND values:

```

| WAC_ADD = Unknown (0 or 1 or so)
 WAC_DEL = WAC_ADD+1
 WAC_GET_STATUS = WAC_ADD+2
 WAC_GET_IE = WAC_ADD+3 |
| --- |

WAC_FRAME_TYPE values:

```

| PRBREQ = Unknown (0 or 1 or so)
 PRBRSP = PRBREQ+1
 BEACON = PRBREQ+2 |
| --- |

WAC_STATUS values:

```

| WAC_FAILED_NO_WAC_AP = -4
 WAC_FAILED_LOW_RSSI = -3
 WAC_FAILED_INVALID_PARAM = -2
 WAC_FAILED_REJECTED = -1
 WAC_SUCCESS = 0
 WAC_DISABLED = 1
 WAC_PROCEED_FIRST_PHASE = 2
 WAC_PROCEED_SECOND_PHASE = 3 |
| --- |

**WMIevent(1037h) - WMI_WAC_SCAN_DONE_EVENT**

**WMIevent(1038h) - WMI_WAC_REPORT_BSS_EVENT**

**WMIevent(1039h) - WMI_WAC_START_WPS_EVENT**

**WMIevent(103Ah) - WMI_WAC_CTRL_REQ_REPLY_EVENT**

Event Data:

```

| Unknown (if any?) |
| --- |

WAC Events. Event data format is unknown, maybe related to below two structs:

WMI_GET_WAC_INFO structure (UNION):

```

| When some case:
 00h A_UINT8 11h ie[17];
 When some other case:
 00h A_INT32 4 wac_status; |
| --- |

WMI_WPS_PIN_INFO structure: ;"WPS" might be WAC_START_WPS related?

```

| 00h A_UINT8 6 bssid[ATH_MAC_LEN];
 06h A_UINT8 8 pin[8]; ;aka "wps_pin[8]" presumably? |
| --- |

| DSi Atheros Wifi - Unimplemented WMI RF Kill and Store/Recall Functions |
| --- |

**WMIcmd(F057h or F04Bh) - WMI_GET_RFKILL_MODE_CMD**

Parameters:

```

| Unknown (none?) |
| --- |

Reply: See WMI_RFKILL_GET_MODE_CMD_EVENT

**WMIcmd(F058h or F04Ch) - WMI_SET_RFKILL_MODE_CMD ;aka WMI_RFKILL_MODE_CMD**

Parameters:

```

| 00h A_UINT8 1 GPIOPinNumber ;GPIO related
 01h A_UINT8 1 IntrType ;?
 02h A_UINT8 1 RadioState ;RFKILL_RADIO_STATE |
| --- |

RFKILL_RADIO_STATE values:

```

| RADIO_STATE_OFF = 01h
 RADIO_STATE_ON = 02h
 RADIO_STATE_INVALID = FFh |
| --- |

**WMIevent(103Bh) - WMI_RFKILL_STATE_CHANGE_EVENT**

Event Data:

```

| Unknown (if any?) |
| --- |

**WMIevent(103Ch) - WMI_RFKILL_GET_MODE_CMD_EVENT**

Event Data:

```

| Unknown (maybe some format as in "SET_RFKILL" command parameters?) |
| --- |

**WMIcmd(F05Eh or F04Eh) - WMI_STORERECALL_CONFIGURE_CMD**

Parameters:

```

| 00h A_UINT8 1 enable (probably some flag)
 01h A_UINT8 1 recipient (only one value defined: STRRCL_RECIPIENT_HOST = 1) |
| --- |

Ultra low power store/recall feature. Seems to be intended to memorize data in
HOST memory... allowing the AR600x chip to power-down its on memory, or so?

**WMIcmd(F05Fh or F04Fh) - WMI_STORERECALL_RECALL_CMD**

Parameters:

```

| 00h A_UINT32 4 length; ;number of bytes of data to follow
 04h A_UINT8 .. data[1]; ;start of "RECALL" data |
| --- |

Ultra low power store/recall feature. Maybe RECALL is meant to restore data
that was formerly memorized from a WMI_STORERECALL_STORE_EVENT.

**WMIcmd(F060h or F050h) - WMI_STORERECALL_HOST_READY_CMD**

Parameters:

```

| 00h A_UINT32 4 sleep_msec;
 04h A_UINT8 1 store_after_tx_empty;
 05h A_UINT8 1 store_after_fresh_beacon_rx; |
| --- |

Ultra low power store/recall feature. Whatever parameters.

**WMIevent(9004h) - WMI_STORERECALL_STORE_EVENT**

Event Data:

```

| 00h A_UINT32 4 msec_sleep; ;time between power off/on
 04h A_UINT32 4 length; ;length of following data
 08h A_UINT8 .. data[1]; ;start of "STORE" data |
| --- |

Ultra low power store/recall feature. Maybe this requests to memorize the
"STORE" data in host memory?

| DSi Atheros Wifi - Unimplemented WMI THIN Functions |
| --- |

**WMIcmd(8000h) - WMI_THIN_RESERVED_START**

**WMIcmd(8FFFh) - WMI_THIN_RESERVED_END**

Area for Thin commands. These command IDs can be found in "wmi_thin.h".

**WMIcmd(8000h) - WMI_THIN_CONFIG_CMD**

Parameters:

```

| 00h A_UINT32 4 cfgField ;combination of WMI_THIN_CFG_...
 04h A_UINT16 2 length ;length in bytes of appended sub-command(s)
 06h A_UINT8 2 reserved[2] ;align padding
 08h ... .. structure(s) selected in "cfgField"... |
| --- |

When cfgField.Bit0 set: append WMI_THIN_CONFIG_TXCOMPLETE struct:

Used to configure the params and content for TX Complete messages the will come
from the Target. these messages are disabled by default but can be enabled
using this structure and the WMI_THIN_CONFIG_CMDID.

```

| +00h A_UINT8 1 version (the versioned type of messages to use, 0=disable)
 +01h A_UINT8 1 countThreshold (msg count threshold triggering a tx
 complete message)
 +02h A_UINT16 2 timeThreshold (timeout interval in MSEC triggering a
 tx complete message) |
| --- |

When cfgField.Bit1 set: append WMI_THIN_CONFIG_DECRYPT_ERR struct:

Used to configure behavior for received frames that have decryption errors. The
default behavior is to discard the frame without notification. Alternately, the
MAC Header is forwarded to the host with the failed status.

```

| +00h A_UINT8 1 enable (1=send decrypt errors to the host, 0=don't)
 +01h A_UINT8 3 reserved[3] (align padding) |
| --- |

When cfgField.Bit2 set: Unused --- NEW VERSION

```

| Unused. |
| --- |

When cfgField.Bit2 set: append WMI_THIN_CONFIG_TX_MAC_RULES --- OLD VERSION

Used to configure behavior for transmitted frames that require partial MAC
header construction. These rules are used by the target to indicate which
fields need to be written.

```

| +00h A_UINT32 4 rules (combination of WMI_WRT_... values) |
| --- |

When cfgField.Bit3 set: append WMI_THIN_CONFIG_RX_FILTER_RULES struct:

Used to configure behavior for received frames as to which frames should get
forwarded to the host and which should get processed internally.

```

| +00h A_UINT32 4 rules (combination of WMI_FILT_... values) |
| --- |

When cfgField.Bit4 set: append WMI_THIN_CONFIG_CIPHER_ENCAP struct:

Used to configure behavior for both TX and RX frames regarding security cipher
encapsulation. The host may specify whether or not the firmware is responsible
for cipher encapsulation. If the firmware is responsible it will add the
security header and trailer for TX frames and remove the header and trailer for
Rx frames. Also, the firmware will examine the resource counter if any and
behave appropriately when a bad value is detected. If the host indicates
responsibility for encapsulation then it is responsible for all aspects of
encapsulation, however the device will still perform the encryption and
decryption of the frame payload if a key has been provided.

```

| +00h A_UINT8 1 enable (enables/disables firmware cipher encapsulation)
 +01h A_UINT8 3 reserved[3] (align padding) |
| --- |

Summary of values for "cfgField":

```

| WMI_THIN_CFG_TXCOMP = 00000001h
 WMI_THIN_CFG_DECRYPT = 00000002h
 WMI_THIN_CFG_MAC_RULES = 00000004h ;old version (or planned for future?)
 WMI_THIN_UNUSED1 = 00000004h ;current version
 WMI_THIN_CFG_FILTER_RULES = 00000008h
 WMI_THIN_CFG_CIPHER_ENCAP = 00000010h |
| --- |

**WMIcmd(8001h) - WMI_THIN_SET_MIB_CMD**

Parameters:

```

| 00h A_UINT16 2 length; /* the length in bytes of the appended MIB data
 02h A_UINT8 1 mibID; /* the ID of the MIB element being set
 03h A_UINT8 1 reserved; /* align padding |
| --- |

**WMIcmd(8002h) - WMI_THIN_GET_MIB_CMD ;reply?**

Parameters:

```

| 00h A_UINT8 1 mibID; /* the ID of the MIB element being set
 01h A_UINT8 3 reserved[3]; /* align padding |
| --- |

Reply: See WMI_THIN_GET_MIB_EVENT

**WMIcmd(8003h) - WMI_THIN_JOIN_CMD ;newer ver only**

Parameters:

```

| 00h A_UINT32 4 basicRateMask; /* bit mask of basic rates
 04h A_UINT32 4 beaconIntval; /* TUs
 08h A_UINT16 2 atimWindow; /* TUs
 0Ah A_UINT16 2 channel; /* frequency in MHz
 0Ch A_UINT8 1 networkType; /* INFRA_NETWORK | ADHOC_NETWORK
 0Dh A_UINT8 1 ssidLength; /* 0 - 32
 0Eh A_UINT8 1 probe; /* != 0 : issue probe req at start
 0Fh A_UINT8 1 reserved; /* alignment
 10h A_UCHAR 20h ssid[WMI_MAX_SSID_LEN];
 30h A_UINT8 6 bssid[ATH_MAC_LEN]; |
| --- |

Reply: See WMI_THIN_JOIN_EVENT

**WMIcmd(8004h) - WMI_THIN_CONNECT_CMD ;newer ver only**

Parameters:

```

| 00h A_UINT16 2 dtim; /* dtim interval in num beacons
 02h A_UINT16 2 aid; /* 80211 association ID from Assoc resp |
| --- |

**WMIcmd(8005h) - WMI_THIN_RESET_CMD ;newer ver only**

Parameters:

```

| 00h A_UINT8 4 reserved[4]; |
| --- |

**WMIevent(8000h) - WMI_THIN_EVENTID_RESERVED_START**

**WMIevent(8FFFh) - WMI_THIN_EVENTID_RESERVED_END**

Events/Responses with special IDs for THIN stuff (wmi_thin.h)

**WMIevent(8001h) - WMI_THIN_GET_MIB_EVENT**

Event Data:

```

| Unknown (maybe same/similar format as for "SET_MIB" command parameters?) |
| --- |

**WMIevent(8002h) - WMI_THIN_JOIN_EVENT**

Event Data:

```

| 00h A_UINT8 1 result (the result of the join command)
 01h A_UINT8 3 reserved[3]; /* alignment |
| --- |

WMI_THIN_JOIN_RESULT values (aka above "result" value):

```

| WMI_THIN_JOIN_RES_SUCCESS = 0 ;device has joined the network
 WMI_THIN_JOIN_RES_FAIL = 1 ;failed for unspecified reason
 WMI_THIN_JOIN_RES_TIMEOUT = 2 ;failed due to no beacon rx in time limit
 WMI_THIN_JOIN_RES_BAD_PARAM = 3 ;failed due to bad cmd param
 WMI_THIN_JOIN_RES_IBSS_START = 4 ;device started new IBSS network |
| --- |

**MIB Access Identifiers tailored for Symbian - mibID**

Below are "mibID" values for SET_MIB and GET_MIB commands, with corresponding
data structures. GET/SET works only if the corresponding structure is
read/write-able, as indicated by (R), (W), and (R/W). Some structures aren't
actually implemented (-), ie. not read/write-able.

```

| When mibID=01h=MIB_ID_STA_MAC; WMI_THIN_MIB_STA_MAC struct: (R)
 00h A_UINT8 6 addr[ATH_MAC_LEN];
 When mibID=02h=MIB_ID_RX_LIFE_TIME; WMI_THIN_MIB_RX_LIFE_TIME struct: (-)
 00h A_UINT32 4 time (units = msec)
 When mibID=03h=MIB_ID_SLOT_TIME; WMI_THIN_MIB_SLOT_TIME struct: (R/W)
 00h A_UINT32 4 time (units = usec)
 When mibID=04h=MIB_ID_RTS_THRESHOLD; WMI_THIN_MIB_RTS_THRESHOLD struct: (R/W)
 00h A_UINT16 2 length (units = bytes)
 When mibID=05h=MIB_ID_CTS_TO_SELF; WMI_THIN_MIB_CTS_TO_SELF struct: (R/W)
 00h A_UINT8 1 enable (1=on, 0=off)
 When mibID=06h=MIB_ID_TEMPLATE_FRAME; WMI_THIN_MIB_TEMPLATE_FRAME struct: (W)
 00h A_UINT8 1 type (type of frame, 0..5, see below "FRM" values)
 01h A_UINT8 1 rate (tx rate to be used, one of WMI_BIT_RATE)
 02h A_UINT16 2 length (num bytes following this structure as template data)
 04h .. .. template data
 Frame "type" values: frame max length:
 TEMPLATE_FRM_PROBE_REQ = 0 FRM_LEN_PROBE_REQ = 256 ;\Symbian dictates a
 TEMPLATE_FRM_BEACON = 1 FRM_LEN_BEACON = 256 ; minimum of 256 for
 TEMPLATE_FRM_PROBE_RESP= 2 FRM_LEN_PROBE_RESP = 256 ;/these 3 frame types
 TEMPLATE_FRM_NULL = 3 FRM_LEN_NULL = 32
 TEMPLATE_FRM_QOS_NULL = 4 FRM_LEN_QOS_NULL = 32
 TEMPLATE_FRM_PSPOLL = 5 FRM_LEN_PSPOLL = 32
 Total sum of above lengths: TEMPLATE_FRM_LEN_SUM = 256+256+256+32+32+32
 When mibID=07h=MIB_ID_RXFRAME_FILTER; WMI_THIN_MIB_RXFRAME_FILTER struct:(R/W)
 00h A_UINT32 4 filterMask;
 FRAME_FILTER_PROMISCUOUS = 00000001h
 FRAME_FILTER_BSSID = 00000002h
 When mibID=08h=MIB_ID_BEACON_FILTER_TABLE; Several structure(s)...? (W)
 There are three related sturctures; the actual "TABLE", and additional
 "TABLE_OUI" and "TABLE_HEADER"; unknown which of those structure(s) are
 meant to be used here...
 WMI_THIN_MIB_BEACON_FILTER_TABLE structure:
 00h A_UINT8 1 ie;
 01h A_UINT8 1 treatment;
 IE_FILTER_TREATMENT_CHANGE = 1
 IE_FILTER_TREATMENT_APPEAR = 2
 WMI_THIN_MIB_BEACON_FILTER_TABLE_OUI structure:
 00h A_UINT8 1 ie;
 01h A_UINT8 1 treatment;
 02h A_UINT8 3 oui[3];
 05h A_UINT8 1 type;
 06h A_UINT16 2 version;
 WMI_THIN_MIB_BEACON_FILTER_TABLE_HEADER structure:
 00h A_UINT16 2 numElements
 02h A_UINT8 1 entrySize (sizeof(WMI_THIN_MIB_BEACON_FILTER_TABLE) on
 03h A_UINT8 1 reserved host cpu may be 2 may be 4)
 When mibID=09h=MIB_ID_BEACON_FILTER; WMI_THIN_MIB_BEACON_FILTER struct: (R/W)
 00h A_UINT32 4 count (num beacons between deliveries)
 04h A_UINT8 1 enable;
 05h A_UINT8 3 reserved[3];
 When mibID=0Ah=MIB_ID_BEACON_LOST_COUNT; WMI_THIN_MIB_BEACON_LOST_COUNT: (W)
 00h A_UINT32 4 count (num consec lost beacons after which send event)
 When mibID=0Bh=MIB_ID_RSSI_THRESHOLD; WMI_THIN_MIB_RSSI_THRESHOLD struct: (W)
 00h A_UINT8 1 rssi (the low threshold which can trigger an event warning)
 01h A_UINT8 1 tolerance (the range above and below the threshold to
 prevent event flooding to the host)
 02h A_UINT8 1 count (the sample count of consecutive frames necessary to
 trigger an event)
 03h A_UINT8 1 reserved[1] (padding)
 When mibID=0Ch=MIB_ID_HT_CAP; WMI_THIN_MIB_HT_CAP struct: (-)
 00h A_UINT32 4 cap;
 04h A_UINT32 4 rxRateField;
 08h A_UINT32 4 beamForming;
 0Ch A_UINT8 6 addr[ATH_MAC_LEN];
 12h A_UINT8 1 enable;
 13h A_UINT8 1 stbc;
 14h A_UINT8 1 maxAMPDU;
 15h A_UINT8 1 msduSpacing;
 16h A_UINT8 1 mcsFeedback;
 17h A_UINT8 1 antennaSelCap;
 When mibID=0Dh=MIB_ID_HT_OP; WMI_THIN_MIB_HT_OP struct: (-)
 00h A_UINT32 4 infoField;
 04h A_UINT32 4 basicRateField;
 08h A_UINT8 1 protection;
 09h A_UINT8 1 secondChanneloffset;
 0Ah A_UINT8 1 channelWidth;
 0Bh A_UINT8 1 reserved;
 When mibID=0Eh=MIB_ID_HT_2ND_BEACON; WMI_THIN_MIB_HT_2ND_BEACON struct: (-)
 00h A_UINT8 1 cfg (see below SECOND_BEACON_xxx values)
 01h A_UINT8 3 reserved[3] (padding)
 SECOND_BEACON_PRIMARY = 1
 SECOND_BEACON_EITHER = 2
 SECOND_BEACON_SECONDARY = 3
 When mibID=0Fh=MIB_ID_HT_BLOCK_ACK; WMI_THIN_MIB_HT_BLOCK_ACK struct: (-)
 00h A_UINT8 1 txTIDField
 01h A_UINT8 1 rxTIDField
 02h A_UINT8 2 reserved[2] (padding)
 When mibID=10h=MIB_ID_PREAMBLE; WMI_THIN_MIB_PREAMBLE struct: (R/W)
 00h A_UINT8 1 enableLong (1=long preamble, 0=short preamble)
 01h A_UINT8 3 reserved[3]
 When mibID=N/A=MIB_ID_GROUP_ADDR_TABLE ;[NOT IMPLEMENTED] (-)
 When mibID=N/A=MIB_ID_WEP_DEFAULT_KEY_ID ;satisfied by wmi_addKey_cmd() (-)
 When mibID=N/A=MIB_ID_TX_POWER ;[NOT IMPLEMENTED] (-)
 When mibID=N/A=MIB_ID_ARP_IP_TABLE ;[NOT IMPLEMENTED] (-)
 When mibID=N/A=MIB_ID_SLEEP_MODE ;[NOT IMPLEMENTED] (-)
 When mibID=N/A=MIB_ID_WAKE_INTERVAL ;[NOT IMPLEMENTED] (-)
 When mibID=N/A=MIB_ID_STAT_TABLE ;[NOT IMPLEMENTED] (-)
 When mibID=N/A=MIB_ID_IBSS_PWR_SAVE ;[NOT IMPLEMENTED] (-)
 When mibID=N/A=MIB_ID_COUNTERS_TABLE ;[NOT IMPLEMENTED] (-)
 When mibID=N/A=MIB_ID_ETHERTYPE_FILTER ;[NOT IMPLEMENTED] (-)
 When mibID=N/A=MIB_ID_BC_UDP_FILTER ;[NOT IMPLEMENTED] (-)
 N/A |
| --- |

**THIN related constants...**

MAC Header Build Rules

These values allow the host to configure the target code that is responsible
for constructing the MAC header. In cases where the MAC header is provided by
the host framework, the target has a diminished responsibility over what fields
it must write. This will vary from framework to framework.

Symbian requires different behavior from MAC80211 which requires different
behavior from MS Native Wifi.

```

| WMI_WRT_VER_TYPE = 00000001h
 WMI_WRT_DURATION = 00000002h
 WMI_WRT_DIRECTION = 00000004h
 WMI_WRT_POWER = 00000008h
 WMI_WRT_WEP = 00000010h
 WMI_WRT_MORE = 00000020h
 WMI_WRT_BSSID = 00000040h
 WMI_WRT_QOS = 00000080h
 WMI_WRT_SEQNO = 00000100h
 WMI_GUARD_TX = 00000200h ;prevent TX ops that are not allowed for a
 ; current state
 WMI_WRT_DEFAULT_CONFIG = 3FFh ;<-- default all bits set |
| --- |

See also: WMI_SET_THIN_MODE_CMD

| DSi Atheros Wifi - Unimplemented WMI Pyxis Functions |
| --- |

**WMIcmd(F009h) - WMI_UNUSED1 or WMI_PYXIS_CONFIG_CMD**

Parameters:

```

| Config Header:
 00h A_UINT16 2 pyxisConfigType ;One of WMI_PYXIS_CONFIG_TYPE
 02h A_UINT16 2 pyxisConfigLen ;Length in Bytes of Information that follows
 When pyxisConfigType=0=WMI_PYXIS_GEN_PARAMS
 04h A_UINT32 2 dataWindowSizeMin
 08h A_UINT32 2 dataWindowSizeMax
 0Ch A_UINT8 1 maxJoiners
 When pyxisConfigType=1=WMI_PYXIS_DSCVR_PARAMS
 04h A_UINT32 4 dscvrWindow
 08h A_UINT32 4 dscvrInterval
 0Ch A_UINT32 4 dscvrLife
 10h A_UINT32 4 probeInterval
 14h A_UINT32 4 probePeriod
 18h A_UINT16 2 dscvrChannel
 When pyxisConfigType=2=WMI_PYXIS_SET_TX_MODE
 04h A_BOOL 4 mode |
| --- |

Whatever "Pyxis specific".

**WMIcmd(F00Ah) - WMI_UNUSED2 or WMI_PYXIS_OPERATION_CMD**

Parameters:

```

| Command Header:
 00h A_UINT16 2 pyxisCmd
 02h A_UINT16 2 pyxisCmdLen ;Length following this header
 When pyxisCmd=0=WMI_PYXIS_DISC_PEER
 04h A_UINT8 6 peerMacAddr[ATH_MAC_LEN]
 When pyxisCmd=1=WMI_PYXIS_JOIN_PEER
 04h A_UINT32 4 ctrl_flags (One of the Bits determines if it
 is Virt Adhoc/the device is to join a BSS)
 08h A_UINT16 2 channel ;Data Channel
 0Ah A_UINT8 1 networkType ;network type
 0Bh A_UINT8 1 dot11AuthMode ;OPEN_AUTH
 0Ch A_UINT8 1 authMode ;NONE_AUTH
 0Dh A_UINT8 1 pairwiseCryptoType ;One of NONE_CRYPT, AES_CRYPT
 0Eh A_UINT8 1 pairwiseCryptoLen ;0 since ADD_KEY passes the length
 0Fh A_UINT8 1 groupCryptoType ;One of NONE_CRYPT, AES_CRYPT
 10h A_UINT8 1 groupCryptoLen ;0 since ADD_KEY passes the length
 11h A_UINT8 6 peerMacAddr[ATH_MAC_LEN] ;BSSID of peer network
 17h A_UINT8 6 nwBSSID[ATH_MAC_LEN] ;BSSID of the Pyxis Adhoc Network
 When pyxisCmd=?=WHAT? below is also "incompletely-defined" as pyxisCmd:
 04h A_BOOL 4 mode (what is this here? dupe of WMI_PYXIS_CONFIG_CMD?) |
| --- |

Whatever "Pyxis specific".

| DSi Atheros Wifi I2C EEPROM |
| --- |

The I2C EEPROM is read via the wifi firmware's bootstub (prior to executing the
main firmware) via the SI_xxx registers. The DSi wifi boards are usually
containing a HN58X2408F chip (1Kx8, with 8bit index). However, the bootstub
contains code for also supporting HN58X24xx chips with bigger capacity
(including such with 16bit index) (although actually using only 300h bytes
regardless of the capacity). The I2C device number is A0h, or, in case of
EEPROMs with 8bit index, the device number is misused to contain the upper
address bits as so:

```

| device = A0h + direction_flag + (addr/100h)*2 ;for devices with 8bit index
 device = A0h + direction_flag ;for devices with 16bit index |
| --- |

**I2C EEPROM Content for AR6002G**

```

| 000h 4 Maybe Size, ID, or Version? (00000300h)
 004h 2 Checksum (all halfwords at [0..2FFh] XORed shall give FFFFh)
 006h 2 Unknown
 008h 2 Country+8000h ;eg. 8000h+188h=JP, 8000h+348h=US (REG_DOMAIN)
 00Ah 6 MAC Address (must be same as in SPI FLASH)
 010h 4 Type/version? (MSB must be 60h, verified by ARM7)
 014h 4 Zerofilled
 018h 5 Unknown
 01Dh 1Fh Zerofilled
 03Ch 70h FFh-filled
 0ACh 8 Zerofilled
 0B4h 12 Unknown
 0C0h 20 Unknown
 0D4h 18h Zerofilled
 0ECh 4 Unknown
 0F0h 4 Unknown, overwritten by [0ECh] after loading
 0F4h 12 Unknown, similar to data at 0B4h ?
 100h 20 Unknown, similar to data at 0C0h ?
 114h 2Ch Zerofilled
 140h 8 FFh-filled
 148h 4 Unknown
 14Ch 88h Zerofilled
 1D4h 3x18 Unknown
 212h 18 Zerofilled
 224h 4x4 Unknown ;\
 234h 2x4 Unknown ;
 23Ch 3x4 Unknown ; together 15x4 maybe ?
 248h 12 Unknown ;
 254h 3x4 Unknown ;/
 260h 60h Unknown
 2C0h 40h Zerofilled
 300h 100h Not used (not loaded to RAM) |
| --- |

The presence of the I2C EEPROM appears to be some mis-conception. It might make
sense to store the wifi calibration data on the daughterboard (rather than in
eMMC storage on mainboard), however, it could be as well stored in the SPI
FLASH chip, but Nintendo apparently didn't knew how to do that.