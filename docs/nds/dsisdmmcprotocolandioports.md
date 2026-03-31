# Dsisdmmcprotocolandioports

> Source: https://problemkaputt.de/gbatek.htm
> Section: Dsisdmmcprotocolandioports

DSi SD/MMC Protocol and I/O Ports 
| |
| --- |

 **I/O Ports**
 DSi SD/MMC I/O Ports: Command/Param/Response/Data
 DSi SD/MMC I/O Ports: Interrupt/Status
 DSi SD/MMC I/O Ports: Control Registers
 DSi SD/MMC I/O Ports: Unknown/Unused Registers
 DSi SD/MMC I/O Ports: Misc
 
 **SD/MMC Protocol**
 DSi SD/MMC Protocol: Command/Response/Register Summary
 DSi SD/MMC Protocol: General Commands
 DSi SD/MMC Protocol: Block Read/Write Commands
 DSi SD/MMC Protocol: Special Extra Commands
 DSi SD/MMC Protocol: CSR Register (32bit Card Status Register)
 DSi SD/MMC Protocol: SSR Register (512bit SD Status Register)
 DSi SD/MMC Protocol: OCR Register (32bit Operation Conditions Register)
 DSi SD/MMC Protocol: CID Register (128bit Card Identification)
 DSi SD/MMC Protocol: CSD Register (128bit Card-Specific Data)
 DSi SD/MMC Protocol: EXT_CSD Register (4096bit Extended CSD Register) (MMC)
 DSi SD/MMC Protocol: RCA Register (16bit Relative Card Address)
 DSi SD/MMC Protocol: DSR Register (16bit Driver Stage Register) (Optional)
 DSi SD/MMC Protocol: SCR Register (64bit SD Card Configuration Register)
 DSi SD/MMC Protocol: PWD Register (128bit Password plus 8bit Password len)
 DSi SD/MMC Protocol: State
 DSi SD/MMC Protocol: Signals
 
 **SDIO Protocol**
 DSi SDIO Special SDIO Commands
 DSi SDIO Memory and I/O Maps
 DSi SDIO Common Control Registers (CCCR)
 DSi SDIO Function Basic Registers (FBR)
 DSi SDIO Card Information Structures (CIS)
 The DSi is using SDIO for the new DSi Wifi interface,
 DSi Atheros Wifi SDIO Interface
 DSi Atheros Wifi Internal Hardware
 
 **Pinouts**
 AUX DSi SD/MMC Pin-Outs
 
 
 
| DSi SD/MMC I/O Ports: Command/Param/Response/Data |
| --- |

 **4004800h/4004A00h - SD_CMD - Command and Response/Data Type (R/W)**
 
```

| 15 undoc Unknown/undoc (read/write-able)
 14 undoc Security Cmd? (0=Normal, 1=Whatever/Security?) (sdio?)
 13 undoc Data Length (0=Single Block, 1=Multiple Blocks)
 12 undoc Data Direction (0=Write, 1=Read)
 11 NTDT Data Transfer (0=No data, 1=With data)
 10-8 REP2-0 Response Type (0=Auto, 1..2=Unknown/Reserved, 3=None, 4=48bit,
 5=48bit+Busy, 6=136bit, 7=48bitOcrWithoutCRC7)
 7-6 CMD1-0 Command Type (0=CMD, 1=ACMD, 2..3=unknown, maybe GEN WR/RD?)
 5-0 CIX Command Index (0..3Fh, command index) |
| --- |

Setting Command Type to "ACMD" does NOT automatically send the APP_CMD prefix
prior to the command number.

For Multiple Blocks, the hardware supports automatically sending
STOP_TRANSMISSION after the last block.

DSi software is usually setting Response Type to "Auto", which is causing the
hardware to use the correct response/data type for standard SD/MMC commands
(bit11-13 are ignored/should be zero when using "Auto"; and maybe same for
bit14-15?).

One exception is that the DSi firmware isn't using "Auto" for SDIO commands
(maybe the hardware isn't aware of them; or it's unable to distinguish between
read/write direction of CMD53, which would require examining the command's
PARAM bits).

There are some differences between some SD and MMC commands, unknown if/how
"Auto" is working in that cases; unknown if there's a SD-or-MMC mode select bit
for that purpose in some configuration register (note: The DSi firmware uses
manual config instead of Auto for CMD8, which differs for SD vs MMC).

Invalid values can cause ILA error (particulary on setting NTDT for CMD12, or
for CMD's Response=None). ILA error will also occur if an old CMD is still
busy.

**4004804h/4004A04h - SD_CMD_PARAM0-1 - Argument (32bit, 2 halfwords) (R/W)**

```

| 31-0 Parameter value for CMD |
| --- |

The parameter value should be written <before> sending the command via
SD_CMD/SDIO_CMD.

**400480Ch/4004A0Ch - SD_RESPONSE0-7 - Response (128bit, 8 halfwords) (R)**

After sending a command, wait for the CMDRESPEND bit (IRQ_STATUS.bit0) to get
set, then read the RESPONSE (if the command does have any response).

For normal 32bit responses:

```

| 31-0 Response
 127-32 Older Responses |
| --- |

For CID/CSD responses:

```

| 119-0 120bit Response
 127-120 Zero (always?) |
| --- |

The above stuff is left-shifted when receiving new response bits (hence moving
older responses to MSBs).

**DATA16 vs DATA32**

Data can be transferred in 16bit or 32bit units (as selected in DATA_CTL.bit1
and DATA32_IRQ.bit1). There are separate data, block len, and block count
registers for 16bit and 32bit mode. 16bit mode uses two FIFOs (each with
200h-byte capactity). 32bit adds another FIFO (also with 200h-byte capacity):

```

| .----------. CPU
 o--| FIFO16_A |--o o---------------- 4004830h
 serial '----------' \ 16bit
 SD/MMC ---o o---------o
 bus \ .----------. \ .--------. CPU/NDMA
 o--| FIFO16_B |--o o---| FIFO32 |--- 400490Ch
 '----------' '--------' 32bit |
| --- |

The 32bit mode is some odd patchwork, apparently Nintendo/Toshiba considered it
easier to add an extra 32bit FIFO (rather than to figure out how to add native
32bit access to Toshiba's original 16bit chip design).

The DSi firmware does use both 32bit and 16bit mode once and then; 32bit mode
can be faster, and it's required for NDMA transfers (which don't support
16bit).

**40048D8h/4004AD8h - SD_DATA_CTL**

```

| 15-13 Always zero
 12 Unknown (usually 1) (R?)
 11-6 Always zero
 5 Unknown (read/write-able) (usually 0) (R/W)
 4 Unknown (usually 1) (R?)
 3-2 Always zero
 1 Select 16bit/32bit Data Mode (0=DATA16, 1=DATA32, see 4004900h) (R/W)
 0 Always zero |
| --- |

DATA32 mode requires setting both 40048D8h.bit1 and 4004900h.bit1. For DATA16
mode, both bits should be zero (though DATA16 seems to be also working the same
way when only either of the bits is zero).

**400480Ah/4004A0Ah - SD_DATA16_BLK_COUNT - NumBlocks for 16/32 bit Modes (R/W)**

**4004908h/4004B08h - SD_DATA32_BLK_COUNT - NumBlocks for 32 bit Mode (R/W)**

```

| 15-0 Number of Data Blocks for multiple read/write commands (0..FFFFh) |
| --- |

SD_DATA16_BLK_COUNT needs to be initialized in both 16bit and 32bit mode, the
written value is copied to a internal register, which gets decremented after
each block, and (when enabled in STOP_INTERNAL_ACTION.bit8) the hardware will
automatically send STOP_TRANSMISSION (CMD12) after the last block (otherwise
the hardware would keep transferring blocks infinitely).

For Data32 mode, DATA32_BLK_COUNT should be set to the same value (it doesn't
really affect the transfer though, the register is intended only for watching
the transfer progress: DATA32_BLK_COUNT is decremented after each block (when
FIFO32 gets empty); except when it would become zero, in that case it stays
stuck at 0001h).

**4004826h/4004A26h - SD_DATA16_BLK_LEN - FIFO16 Size for 16/32 bit Modes (R/W)**

**4004904h/4004B04h - SD_DATA32_BLK_LEN - FIFO32 Size for 32 bit Mode (R/W)**

```

| 15-10 Always zero
 9-0 Data Block Length in bytes (for DATA16: clipped to max 0200h by hw) |
| --- |

SD_DATA16_BLK_LEN needs to be initialized in both 16bit and 32bit mode. For
32bit mode, SD_DATA32_BLK_LEN should be also set to the same value (otherwise
odd effects might occur when forwarding FIFO16 to/from FIFO32).

The block length should be usually 0200h (for 512-byte SD/MMC memory blocks).
Other values may be needed for SDIO functions, or when accessing SSR/SCR/PWD
registers via data transfers.

DATA32_BLK_LEN can be set to max=3FFh (unlike DATA16_BLK_LEN which is clipped
to max=200h by hardware), though settings bigger than 200h won't work in
practice (since the FIFOs are only 200h bytes in size).

**4004830h/4004A30h - SD_DATA16_FIFO - Data FIFO for 16bit Mode (R/W)**

**400490Ch/4004B0Ch - SD_DATA32_FIFO - Data FIFO for 32bit Mode (R/W)**

For Data16:

```

| 15-0 Data (Read/Write one block (usually 100h halfwords) upon RXRDY/TXRQ) |
| --- |

For Data32:

```

| 31-0 Data (Read/Write one block (usually 80h words) upon RX32RDY/TX32RQ) |
| --- |

See the RXRDY/TXRQ and RX32RDY/TX32RQ interrupt flags for details.

The first Data32 block may be written even before sending the command (the DSi
firmware is actually doing that, although, performance-wise, it would make
sense only when writing multiple clusters, ie. sending the first block of the
next cluster while the previous write is still in progress).

Observe that RX32RDY/TX32RQ are actually FIFO full/empty flags (getting
triggered any time when FIFO is full/empty, so you must know for yourself if
you want to transfer data in that situations; eg. FIFO empty will trigger even
after having written all data blocks, or when not intending to write any data
at all).

| DSi SD/MMC I/O Ports: Interrupt/Status |
| --- |

All SD/MMC IRQs are triggering IF2.8, CardIRQ does ADDITIONALLY trigger IF2.9

All SDIO IRQs are triggering IF2.10, CardIRQ does ADDITIONALLY trigger IF2.11?

**400481Ch/4004A1Ch - SD_IRQ_STATUS0-1 - Interrupt Status (R/ack)**

**4004820h/4004A20h - SD_IRQ_MASK0-1 - Interrupt Mask (R/W)**

The IRQ_STATUS registers contain acknowledge-able IRQ Flags (those bits that
that are maskable in IRQ_MASK register), as well as static read-only status
bits without IRQ function (eg. WRPROTECT).

IRQ Flags/Write (0=Acknowledge, 1=No change)

IRQ Flags/Read (0=No IRQ, 1=IRQ)

IRQ Mask (0=Enable, 1=Disable) (8B7F031Dh when all IRQs disabled)

```

| Bit Stat Mask Function
 0 SREP MREP CMDRESPEND (response end) (or R1b: busy end)
 1 0 0 Unknown/unused (always 0)
 2 SRWA MRWA DATAEND (set after (last) data block end)
 3 SCOT MCOT CARD_REMOVE (0=No event, 1=Is/was newly ejected) ;\
 4 SCIN MCIN CARD_INSERT (0=No event, 1=Is/was newly inserted) ; SD
 5 undoc 0 SIGSTATE (0=Ejected, 1=Inserted) (SDIO: always 1) ; Slot
 6 0 0 Unknown/unused (always 0) ; Sw's
 7 undoc 0 WRPROTECT (0=Locked/Ejected, 1=Unlocked/HalfEjected);/
 8 undoc undoc CARD_REMOVE_A (0=No event, 1=High-to-Low occurred) ;\SD
 9 undoc undoc CARD_INSERT_A (0=No event, 1=Low-to-High occurred) ; Slot
 10 undoc 0 SIGSTATE_A (usually 1=High) ;also as so for SDIO ;/Data3
 11 0 0 Unknown/unused (always 0)
 12 0 0 Unknown/unused (always 0)
 13 0 0 Unknown/unused (always 0)
 14 0 0 Unknown/unused (always 0)
 15 0 0 Unknown/unused (always 0)
 16 SCIX MCIX CMD_IDX_ERR Bad CMD-index in response (RCMDE,SCMDE)
 17 SCRC MCRC CRCFAIL CRC response error (WCRCE,RCRCE,SCRCE,CCRCE)
 18 SEND MEND STOPBIT_ERR End bit error (WEBER,REBER,SEBER,CEBER)
 19 SDTO MDTO DATATIMEOUT Data Timeout (NRCS,NWCS,KBSY)
 20 SFOF MFOF RXOVERFLOW HOST tried write full
 21 SFUF MFUF TXUNDERRUN HOST tried read empty
 22 SCTO MCTO CMDTIMEOUT Response start-bit timeout (NRS,NSR)
 23 undoc 0 DATA0_PIN (0=Low, 1=High) (of selected port: eMMC or SD Slot)
 24 SBRE MBRE RXRDY (fifo not empty) (request data read)
 25 SBWE MBWE TXRQ (datafifoempty?) (request data write)
 26 0 0 Unknown/unused (always 0)
 27 undoc undoc Unknown/undoc (bit27 is mask-able in IRQ_MASK)
 28 0 0 Unknown/unused (always 0)
 29 undoc 0 CMD_READY? (inverse of BUSY?) (unlike toshiba ILFSL/IFSMSK)
 30 undoc 0 CMD_BUSY (CMD_BUSY=0 shortly before CMD_READY=1?)
 31 ILA IMSK Illegal Command Access (old CMD still busy, or wrong NTDT) |
| --- |

Normally, IRQs should be acknowledged by writing "FLAGS=NOT X", whilst the
firmware is using an unstable "FLAGS=FLAGS AND NOT X" read-modify-write
function (accidentally acknowledging any IRQs that have newly occurred during
that operation).

**4004836h/4004A36h - SD_CARD_IRQ_STAT (R/ack)**

**4004838h/4004A38h - SD_CARD_IRQ_MASK (R/W)**

IRQ Flags/Write (0=Acknowledge, 1=No change)

IRQ Flags/Read (0=No IRQ, 1=IRQ)

IRQ Mask (0=Enable, 1=Disable) (C007h when all IRQs disabled)

```

| Bit Stat Mask Function
 15 undoc undoc SomeIRQ (triggered SOMETIMES on forced CMDTIMEOUT?)
 14 undoc undoc SomeIRQ (triggered near DATAEND?)
 13-3 0 0 Always zero
 2 undoc undoc SomeIRQ (triggered on forced TXUNDERRUN?)
 1 undoc undoc SomeIRQ (triggered about once per datablock?)
 0 CINT0 CIMSK0 CardIRQ (triggered by /IRQ aka Data1 pin; for SDIO devices) |
| --- |

All stat bits (except bit1) are triggered only if enabled in SD_CARD_IRQ_CTL.

Bit0 is actually used (for SDIO hardware), the other bits aren't used by
existing software (they don't seem to be useful; purpose might be error
testing, or forcing commands to abort).

**4004834h/4004A34h - SD_CARD_IRQ_CTL (R/W)**

CardIRQ Enable works only when also writing SD_CARD_PORT_SELECT.bit10=0 and
only with valid SD_CARD_CLK_CTL setting.

```

| 15-10 Always zero
 9 Enable setting SD_CARD_IRQ_STAT.bit14 and cause nothing special? (R/W)
 8 Enable setting SD_CARD_IRQ_STAT.bit15 and cause CMDTIMEOUT? (R/W)
 7-3 Always zero
 2 Enable setting SD_CARD_IRQ_STAT.bit2 and cause TXUNDERRUN? (R/W)
 1 Always zero
 0 Enable setting SD_CARD_IRQ_STAT.bit0 (CardIRQ upon Data1=LOW) (R/W) |
| --- |

Bit9 is autocleared at time when bit9 causes setting SD_CARD_IRQ_STAT.bit14.

Bit2 is autocleared shortly before bit8 causes CMDTIMEOUT.

SD_CARD_IRQ_STAT.bit15 is set only when setting bit8 AND bit2 DURING cmd/xfer.

**40048F8h/40048FAh - SD_EXT_IRQ_STAT0-1 - Interrupt Status (R/ack)**

**40048FCh/40048FEh - SD_EXT_IRQ_MASK0-1 - Interrupt Mask (R/W)**

The 3DS bootroom is using bit0-2 for eMMC insert/eject detection. DSi software
isn't doing such stuff (though the registers seem to exist in DSi hardware,
too). Either way, eMMC cannot be ejected on neither DSi nor 3DS, so the feature
is kinda useless.

```

| Bit Stat Mask Function
 0 SCOT MCOT CARD_REMOVE (0=No event, 1=Is/was newly ejected) ;\eMMC
 1 SCIN MCIN CARD_INSERT (0=No event, 1=Is/was newly inserted) ; Slot
 2 undoc 1 SIGSTATE (MMC: Always 1=Inserted) (SDIO: always 0);/Sw
 3 0 RW Unknown (stat always 0, but mask is R/W) ;\maybe another
 4 0 RW Unknown (stat always 0, but mask is R/W) ; unimplemented
 5 0 1 Unknown (stat always 0, and mask always 1) ;/device?
 6 0 RW Unknown (stat always 0, but mask is R/W) ;\maybe yet one
 7 0 RW Unknown (stat always 0, but mask is R/W) ; more?
 8-15 0 0 Unknown/unused (always 0) ;/
 16 ?? RW Unknown (stat can be 0/1) (R/W? or rather R?)
 17 ?? RW Unknown (stat can be 0/1) (R/W? or rather R?)
 18 ?? 1 Unknown (1=normal, 0=data/read from card to fifo busy?) (R)
 19 0 RW Unknown (stat always 0, but mask is R/W)
 20 0 RW Unknown (stat always 0, but mask is R/W)
 21 0 1 Unknown (stat always 0, and mask always 1)
 22 0 RW Unknown (stat always 0, but mask is R/W)
 23 0 RW Unknown (stat always 0, but mask is R/W)
 24-31 0 0 Unknown/unused (always 0) |
| --- |

Some of the unknown bits might be CD/Data3 or IRQ/Data1 pins for Device 1, in
similar way as for Device 0 (maybe that is in bit16-18, which are observed to
become nonzero in certain situations).

EXT_IRQ_STAT can be 00000000h..00070004h for MMC, or always 00000000h for SDIO
(as there's only one device on SDIO bus). EXT_IRQ_MASK is R/W (can be
00240024h..00FF00FFh) for both SD/MMC and SDIO.

**4004900h/4004B00h - SD_DATA32_IRQ**

```

| 15-13 Always zero
 12 TX32RQ IRQ Enable (0=Disable, 1=Enable) (R/W)
 11 RX32RDY IRQ Enable (0=Disable, 1=Enable) (R/W)
 10 Clear FIFO32 (0=No change, 1=Force FIFO32 Empty) (W)
 9 TX32RQ IRQ Flag (0=IRQ, 1=No) (0=FIFO32 Empty) (R)
 8 RX32RDY IRQ Flag (0=No, 1=IRQ) (1=FIFO32 Full) (R)
 7-2 Always zero
 1 Select 16bit/32bit Data Mode (0=DATA16, 1=DATA32, see 40048D8h) (R/W)
 0 Always zero |
| --- |

Bit8,9 are extra IRQ flags, the flags get set ONLY in DATA32 mode (not in
DATA16 mode).

Bit8,9 are somewhat edge-triggered (setting the IF2 bit only on NoIRQ-to-IRQ
transitions; whilst Disable-to-Enable transitions don't trigger IF2).

Bit8,9 don't need to be acknowledged, they are automatically switched to "No
IRQ" by hardware (when reading/writing DATA32_FIFO, ie. when the FIFO is no
longer empty/full).

**400482Ch/4004A2Ch - SD_ERROR_DETAIL_STATUS0-1 - Error Detail Status (R)**

This register contains extra info about the error bits in SD_IRQ_STATUS. The
error bits (except bit13/always set) are automatically cleared when sending a
new command by writing to SD_CMD.

```

| 31-23 0 Always zero
 22 KBSY Timeout for CRC status busy ;\STAT.19
 21 NWCS Timeout for CRC status (can occur for Data Write) ; (SDTO)
 20 NRCS Timeout for Data start-bit, or for Post Data Busy ;/
 19-18 0 Always zero
 17 NRS Response Timeout for auto-issued CMD12 ;\STAT.22
 16 NCR Response Timeout for non-auto-issued CMD's ;/(SCTO)
 15-14 0 Always zero
 13 undoc Unknown/undoc (always 1) ;-Always 1
 12 0 Always zero
 11 WCRCE CRC error for Write CRC status for a write command ;\
 10 RCRCE CRC error for Read Data ; STAT.17
 9 SCRCE CRC error for a Response for auto-issued CMD12 ; (SCRC)
 8 CCRCE CRC error for a Response for non-auto-issued CMD's ;/
 5 WEBER End bit error for Write CRC status ;\
 4 REBER End bit error for Read Data ; STAT.18
 3 SEBER End bit error for Response for auto-issued CMD12 ; (SEND)
 2 CEBER End bit error for Response for non-auto-issued CMD's ;/
 1? SCMDE Bad CMD-index in Response of auto-issued CMD12 ;\STAT.16
 0 RCMDE Bad CMD-index in Response of non-auto-issued CMD's ;/(SCIX) |
| --- |

Note: CMD12 is STOP_TRANSMISSION (automatically sent after BLK_COUNT blocks).

The four "auto-issued CMD12" bits exist for SD registers only (not for SDIO,
going by old toshiba datasheets; which may be wrong).

SCMDE is probably in bit1 (though, official specs say bit0, which would be same
as RCMDE).

Some error bits can be intentionally provoked: Bit8=1 when programming the
controller to expect GET_STATUS to return a 136bit response. Bit16=1 when
sending GET_CID in "tran" state. Bit20=1 when sending GET_STATUS configured to
expect a data/read reply. Bit21=1 when sending GET_STATUS configured to expect
a data/write block (and with actually sending a data block to it).

**40048F6h/4004AF6h - SD_WRPROTECT_2 (RESERVED4) (R)**

```

| 15-1 Always zero
 0 WRPROTECT_2 for onboard eMMC (usually/always 0=Unlocked) (R) |
| --- |

Bit0 is write-protect flag for onboard eMMC (equivalent to the SD/MMC slot's
write-protect switch in 400481Ch.bit7, but in inverted form: 0=Unlocked for
eMMC, instead of 0=Locked for SD/MMC). The firmware does check bit0 (and, if
set, hangs shortly before starting games), unknown if the TWL CPU and DSi
mainboard do actually have any solder pads for it.

**IRQ Edge-Triggering**

One nasty "feature" for both IRQ_STATUS and DATA32_IRQ is that the interrupts
are edge-triggered (IF2.bit8 gets set only on No-IRQ-to-IRQ transitions) (IF2
can be acknowledged even if IRQ(s) are still requested, which would mean that
those IRQ(s) would get lost). Workaround would be:

```

| - first acknowledge IF2.bit8 (must be done before next step)
 - then check for pending IRQs in IRQ_STATUS and DATA32_IRQ, and process
 all of them |
| --- |

Ie. if you would process only a single IRQ, then any other IRQs would get lost.

For IRQ_STATUS, one could also force unprocessed IRQs to re-trigger IF2 by
temporarily disabling IRQ_MASK bits (disable-to-enable for pending IRQs is also
edge-triggering IF2). That trick works for IRQ_STATUS only, not for DATA32_IRQ.

| DSi SD/MMC I/O Ports: Control Registers |
| --- |

**4004802h/4004A02h - SD_CARD_PORT_SELECT**

```

| 15-11 Always zero
 10 Unknown (should be set on write) (reads as zero) (1=CardIRQ off!) (W)
 9-8 Unknown (Always 2 for SD/4004802h, always 1 for SDIO/4004A02h) (R)
 7-4 Always zero
 3-1 Unknown (read/write-able) (R/W)
 0 Port Select (0=SD Card Slot, 1=Onboard eMMC) (for SDIO: Unknown) (R/W) |
| --- |

Known written values are 0400h and 0401h for SD/MMC. The register is left
uninitialized for SDIO.

**4004828h/4004A28h - SD_CARD_OPTION - Card Option Setup**

```

| 15 undoc Bus Width (0=4bit, 1=1bit) (R/W)
 14 undoc Unknown (usually set) (R?)
 13-9 0 Always zero
 8 undoc Unknown (firmware tries to toggle this after CLK change?) (W?)
 7-4 RTO Data start/busy timeout (2000h SHL 0..14, or 15=100h SDCLK's)(R/W)
 0-3 TO? SIGSTATE detect delay (400h SHL 0..14, or 15=100h HCLK's) (R/W) |
| --- |

See Timeout Notes below for details.

**4004824h/4004A24h - SD_CARD_CLK_CTL Card Clock Control**

```

| 15-11 Always zero ;unlike Toshiba: no HCLK divider-disable in bit15)
 10 Unknown (0=Normal, 1=Unknown, doesn't affect SDCLK output?) (R/W)
 9 SDCLK Auto pause (0=Normal, 1=Freeze SDCLK output when inactive) (R/W)
 8 SDCLK Pin Enable (0=Force SDCLK=LOW, 1=Output SDCLK=HCLK/n) (R/W)
 7-0 HCLK Div (0,1,2,4,8,16,32,64,128 = Div2,4,8,16,32,64,128,256,512) (R/W) |
| --- |

The DSi uses HCLK=33.513982 MHz, the SDCLK pin can range from HCLK/512=65kHz to
HCLK/2=16.757MHz, max transfer rate would be thus 8MByte/s in 4bit mode.

**Max CLK speed:**

Observe that card detection/initialization should be done at lower CLK rate
than during normal operation.

For SD/MMC initialization: The DSi firmware starts with HCLK/128=262kHz (max
allowed would be 400kHz for MMC). This is actually required: The DSi's onboard
Samsung KMAPF0000M-S998 eMMC chip won't respond to ALL_GET_CID when trying to
use 16MHz CLK). Higher CLK can be used once when detecting max speed (see
TRAN_SPEED in CSD register; when extracting bits from CSD: mind the different
120bit-without-CRC vs 128bit-with-CRC notations).

For SDIO/Wifi initialization: The DSi firmware starts with HCLK/256=131kHz, and
switches to HCLK/2=16.757MHz after reading SDIO Bus Speed register
(Function0:00013h).

After init, one can use the detected speeds (see above), it should be also safe
to assume that HCLK/2=16.757MHz is always supported after initialization (all
SD devices should support at least 25MHz, and all(?) MMC devices at least
26MHz, and all DSi SDIO/Wifi boards should be fast enough either).

**Notes:**

The SDCLK pins are permanently pulsed, even for devices deselected via
SD_CARD_PORT_SELECT.0, and even if no CMD or DATA is being transferred.
However, the DSi firmware is usually stopping SDCLK via Bit8=0 when not
accessing SD/MMC (doing so may reduce noise and power consumption).

Setting bit9, or to trying to set more than one bit in bit7-0 will freeze the
SDCLK output (in this case SDCLK may get stuck HIGH or LOW, unlike Bit8=0 which
forces LOW).

Odd Effect: Setting bit10-8 to ALL ones, combined with an invalid HDIV (eg.
writing 0703h) does disable CardIRQ on Data1 pin.

**4004808h/4004A08h - SD_STOP_INTERNAL_ACTION**

```

| 15-9 Always zero
 8 Auto-Stop (1=Automatically send CMD12 after BLK_COUNT blocks) (R/W)
 7-1 Always zero
 0 Unknown (firmware often clears this bit, but never sets it?) (R/W) |
| --- |

Existing code does set bit8 (prior to changing SD_DATA16_BLK_COUNT).

Existing code does clear bit0 (alongsides with IRQ enable/acknowledge or so).

Note: Bit8 MUST be also set for SDIO with multiple blocks (although SDIO
doesn't literally have CMD12).

**40048E0h/4004AE0h - SD_SOFT_RESET - Software Reset**

```

| 15-3 Always zero
 2 Unknown (always 1) (R?)
 1 Unknown (always 1) (though firmware tries to toggle this bit) (R?)
 0 SRST Soft Reset (0=Reset, 1=Release) (R/W) |
| --- |

Software should apply reset after sensing card insertion/removal, and
(thereafter) release reset in case of card insertion. Software reset does
acknowledge all IRQs (except that from SDIO /IRQ pin?), and does probably also
reinitialize some other registers.

Clearing bit0 does force following settings (while and as long as Bit0=0):

```

| SD_STOP_INTERNAL_ACTION = 0000h
 SD_RESPONSE0-7 = zerofilled
 SD_IRQ_STATUS0-1 = all IRQ flags acknowledged
 SD_ERROR_DETAIL_STATUS0-1 = all bits cleared (except bit13/always set)
 SD_CARD_CLK_CTL = bit 8 and 10 cleared
 SD_CARD_OPTION = 40EEh
 SD_CARD_IRQ_STAT = 0000h
 Internal FIFO16 address is reset to first halfword of FIFO_A
 Reading FIFO16 returns 0000h (but old content reappears when releasing reset) |
| --- |

All other registers seem to be left unaffected (including the extra IRQ flags
in 4004900h); though there may be some further hidden effects (like aborting
transfers or resetting internal registers).

Note: The DSi firmware does issue reset by toggling both bit0 and bit1,
although bit1 does seem to be read-only (always 1), and trying to clear that
bit doesn't seem to have any effect at all.

**Timeout Notes**

Timeouts are counted in SDCLK units (the CLK-Pin rate selected in
SD_CARD_CLK_CTL register). For Response-Timeouts, the timeout is fixed: Around
290h SDCLK's (preceeded by 30h SDCLK's for sending the command). For
Data-Timeouts, the timeout can be selected in SD_CARD_OPTION.bit4-7, which is
apparently what toshiba tried to describe as "RTO" bits. Values 0..14 select
timeout "2000h SHL 0..14 SDCLK's" and value 15 selects "100h SDCLK's" (that,
oddly, resulting in Data-timeout getting triggered before Response-timeout,
which is rather nonsense since it's opposite of the actual transfer order).

For data/read, the timeout starts counting after transferring Command+Response.
For data/write it starts after transferring Command+Response+DataBlock. The
maximum duration for data timeouts (with RTO=14) would be around 8 seconds (at
SDCLK=HCLK/2), or up to about 30 minutes (at HCLK/512).

One odd effect is that Response-Timeouts can occur (after 290h SDCLKs, and
recursing the selected SDCLK=HCLK/n rate) even if SDCLK is stopped via
SD_CARD_CLK_CTL.Bit8 (ie. the selected clock is kept running internally, and
only the CLK-Pin output is forced LOW when Bit8=0).

**CFG9_SDMMC_CTL and SD_CARD_OPTION**

On DSi, SD/MMC Slot power supply is always on (it takes about 3ms to reach the
3.3V after power on; if the console booted up and loaded data from eMMC, one
can be sure that the SD/MMC Slot is properly powered up).

On 3DS, SD/MMC Slot power supply can be enabled/disabled, the bootrom does
leave it disabled, so, before accessing SD/MMC Slot or before testing the card
insert flag, FIRM software must enable it (set CFG9_SDMMC_CTL bit0=0 and
usually bit9=1, it takes about 4.2ms to reach 3.3V, SD cards may take 1ms to
boot up, for those 4.2ms+1ms delays, best set the SIGSTATE detect delay in
SD_CARD_OPTION.bit0-3 to 09h, ie. circa 8ms).

| DSi SD/MMC I/O Ports: Unknown/Unused Registers |
| --- |

Below registers don't seem to be used by existing software...

**40048F2h/4004AF2h - Can be 0003h**

```

| 15-2 Always zero
 1-0 Unknown (0..3) (R/W) |
| --- |

**40048F4h/4004AF4h - Can be 0770h**

```

| 15-11 Always zero
 10-8 Unknown (0..7) (R/W)
 7 Always zero
 6-4 Unknown (0..7) (R/W)
 3-0 Always zero |
| --- |

**Unused Registers with Fixed value (all bits read-only, or write-only)**

```

| 400482Ah/4004A2Ah 2 Fixed always zero?
 4004832h/4004A32h 2 Fixed always zero? ;(TC6371AF:BUF1 Data MSBs?)
 400483Ah/4004A3Ah 2 Fixed always zero? ;(SDCTL_SDIO_HOST_INFORMATION)
 400483Ch/4004A3Ch 2 Fixed always zero? ;(SDCTL_ERROR_CONTROL)
 400483Eh/4004A3Eh 2 Fixed always zero? ;(TC6387XB: LED_CONTROL)
 4004840h/4004A40h 2 Fixed always 003Fh?
 4004842h/4004A42h 2 Fixed always 002Ah?
 4004844h/4004A44h 6Eh Fixed always zerofilled?
 40048B2h/4004AB2h 2 Fixed always FFFFh?
 40048B4h/4004AB4h 6 Fixed always zerofilled?
 40048BAh/4004ABAh 2 Fixed always 0200h?
 40048BCh/4004ABCh 1Ch Fixed always zerofilled?
 40048DAh/4004ADAh 6 Fixed always zerofilled?
 40048E2h/4004AE2h 2 Fixed always 0009h? ;(RESERVED2/9, TC6371AF:CORE_REV)
 40048E4h/4004AE4h 2 Fixed always zero?
 40048E6h/4004AE6h 2 Fixed always zero? ;(RESERVED3, TC6371AF:BUF_ADR)
 40048E8h/4004AE8h 2 Fixed always zero? ;(TC6371AF:Resp_Header)
 40048EAh/4004AEAh 6 Fixed always zerofilled?
 40048F0h/4004AF0h 2 Fixed always zero? ;(RESERVED10)
 4004902h/4004B02h 2 Fixed always zero?
 4004906h/4004B06h 2 Fixed always zero?
 400490Ah/4004B0Ah 2 Fixed always zero?
 4004910h/4004B10h F0h Fixed always zerofilled? |
| --- |

| DSi SD/MMC I/O Ports: Misc |
| --- |

**Toshiba Chips**

The DSi SDIO/MMC port addresses and status bits appear to be identical to those
on Toshiba SD/MMC/SDIO controller chips.

One small difference is that the DSi can set SD_IRQ_MASK.Bit27 (which wasn't
used on (older) Toshiba chips). The Toshiba chips seem to include additional
"CNF" configuration registers (which seem to be missing on DSi).

```

| Chip Year Pages Features
 Toshiba TC6371AF 2000-2002 58 SD/MMC/Smart/PCI (old/basic specs, no SDIO)
 Toshiba TC6380AF 2001-2002 90 SD/MMC/SDIO/SmartMedia
 Toshiba TC6387XB 2001-2002 62 SD/MMC/SDIO/SDLED
 Toshiba TC6391XB 2002 202 SD/MMC/SDIO/SmartMedia/USB/LCD/etc.
 Toshiba TC6393XB ? ;\unknown features, no datasheet exists (the chips
 Toshiba T7L66XB ? ;/are mentioned in tmio_mmc.h and tmio_mmc.c source) |
| --- |

Related source code:

```
[https://github.com/torvalds/linux/tree/master/drivers/mmc/host/](https://github.com/torvalds/linux/tree/master/drivers/mmc/host/)
| (see files toshsd.* and tmio_mmc.*)
 (that just as historic references, meanwhile there's better source code
 around, written specifically for DSi and 3DS hardware) |
| --- |

The TC6380AF/TC6387XB/TC6391XB datasheets are more or less identical on the
SD/MMC/SDIO section, TC6387XB is probably the best reference because it doesn't
contain offtopic extras like SmartMedia, USB, LCD, etc. The datasheets contain
I/O Maps with port addresses, but no description tables for the bits in those
ports (though some bits are mentioned here and there in the text, scattered
across many different pages, and other bits are left completely undocumented).

| DSi SD/MMC Protocol: Command/Response/Register Summary |
| --- |

**Basic Commands (class 0)**

```

| CMD0 sd/mmc spi GO_IDLE_STATE (CMD0 with arg=stuff) (type=bc)
 CMD0 mmc GO_PRE_IDLE_STATE (CMD0 with arg=F0F0F0F0h) (type=bc)
 CMD0 mmc BOOT_INITIATION (CMD0 with arg=FFFFFFFAh, type=N/A)
 CMD1 sd/mmc spi SEND_OP_COND (On SD Cards: SPI only)
 CMD2 sd/mmc ALL_GET_CID (type=bcr)
 CMD3 sd GET_RELATIVE_ADDR (type=bcr)
 CMD3 mmc SET_RELATIVE_ADDR (type=ac)
 CMD4 sd/mmc SET_DSR (type=bc)
 CMD5 sd spi Reserved for I/O cards (see "SDIO Card Specification")
 CMD5 mmc ? SLEEP_AWAKE (type=ac) (MMC only, IO_SEND_OP_COND on SDIO)
 CMD7 sd/mmc SELECT_DESELECT_CARD (type=ac) ;actually: (type=bcr)
 CMD8 sd spi SET_IF_COND (type=bcr)
 CMD8 mmc spi GET_EXT_CSD (type=adtc)
 CMD9 sd/mmc spi GET_CSD (type=ac) (SPI: type=adtc)
 CMD10 sd/mmc spi GET_CID (type=ac) (SPI: type=adtc)
 CMD11 sd VOLTAGE_SWITCH (type=ac)
 CMD12 sd/mmc spi STOP_TRANSMISSION (type=ac)
 CMD13 sd/mmc spi GET_STATUS (type=ac) (sends 16bit status in SPI Mode)
 CMD14 mmc BUSTEST_R (type=adtc) (MMC only, Reserved on SD)
 CMD19 mmc BUSTEST_W (type=adtc) (MMC only, SET_TUNING_BLOCK on SD)
 CMD15 sd/mmc GO_INACTIVE_STATE (type=ac) |
| --- |

**Block-Oriented Read Commands (class 2)**

```

| CMD16 sd/mmc spi SET_BLOCKLEN (type=ac)
 CMD17 sd/mmc spi READ_SINGLE_BLOCK (type=adtc)
 CMD18 sd/mmc spi READ_MULTIPLE_BLOCK (type=adtc)
 CMD19 sd SET_TUNING_BLOCK (type=adtc)
 CMD20 sd SPEED_CLASS_CONTROL (type=ac)
 CMD22 sd Reserved
 CMD23 sd/mmc-spi SET_BLOCK_COUNT (type=ac) (SPI supported ONLY on MMC?) |
| --- |

**Block-Oriented Write Commands (class 4)**

```

| CMD16 sd/mmc spi SET_BLOCKLEN (type=ac)
 CMD20 sd SPEED_CLASS_CONTROL (type=ac)
 CMD23 sd/mmc-spi SET_BLOCK_COUNT (type=ac) (SPI supported ONLY on MMC?)
 CMD24 sd/mmc spi WRITE_BLOCK (type=adtc)
 CMD25 sd/mmc spi WRITE_MULTIPLE_BLOCK (type=adtc)
 CMD26 sd/mmc Reserved For Manufacturer (MMC: PROGRAM_CID)
 CMD27 sd/mmc spi PROGRAM_CSD (type=adtc) |
| --- |

**Block-Oriented Write-Protection Commands (class 6)**

```

| CMD28 sd/mmc spi SET_WRITE_PROT (type=ac)
 CMD29 sd/mmc spi CLR_WRITE_PROT (type=ac)
 CMD30 sd/mmc spi GET_WRITE_PROT (type=adtc)
 CMD31 - SD: Reserved
 CMD31 mmc MMC: SEND_WRITE_PROT_TYPE (type=adtc) |
| --- |

**Erase Commands (class 5)**

```

| CMD32 sd spi ERASE_WR_BLK_START (type=ac)
 CMD33 sd spi ERASE_WR_BLK_END (type=ac)
 CMD32-34 mmc spi Reserved for compatibility with older MMC cards (uh?)
 CMD35 mmc spi ERASE_GROUP_START (type=ac)
 CMD36 mmc spi ERASE_GROUP_END (type=ac)
 CMD37 mmc spi Reserved for compatibility with older MMC cards (uh?)
 CMD38 sd/mmc spi ERASE (type=ac)
 CMD39 - Reserved
 CMD41 - Reserved |
| --- |

**Lock Card (class 7)**

```

| CMD16 sd/mmc spi SET_BLOCKLEN (type=ac)
 CMD40 sd Defined by DPS Spec (Data Protection System) (type=adtc)
 CMD42 sd/mmc spi LOCK_UNLOCK (type=adtc)
 CMD43-47 - Reserved
 CMD51 - Reserved |
| --- |

**Application-Specific Commands (class 8)**

```

| CMD39-40 mmc MMCA Optional Command, currently not supported
 CMD55-56 mmc MMCA Optional Command, currently not supported
 CMD55 sd spi APP_CMD (type=ac) ;\also defined for MMC,
 CMD56 sd spi GEN_CMD (type=adtc) ;/but ONLY in SPI mode !!??
 CMD60-63 sd/mmc spi Reserved for manufacturer |
| --- |

**I/O Mode Commands (class 9) (Refer to "SDIO Card Specification")**

```

| CMD5 sdio spi SDIO: IO_SEND_OP_COND
 CMD52 sdio spi SDIO: IO_RW_DIRECT
 CMD53 sdio spi SDIO: IO_RW_EXTENDED
 CMD54 - SDIO: Reserved
 CMD39 mmcio MMCIO: FAST_IO (type=ac)
 CMD40 mmcio MMCIO: GO_IRQ_STATE (type=bcr) |
| --- |

**Switch Function Commands (class 10) (version 1.10+)**

```

| CMD6 mmc spi SWITCH (type=ac) ;related to EXT_CSD register
 CMD6 sd spi SWITCH_FUNC (type=adtc)
 CMD34-37 sd+spi Reserved for Command Systems from CMD6 ;\SPI
 CMD50,57 sd+spi Reserved for Command Systems from CMD6 ;/
 CMD34-35 sd Reserved ;\
 CMD36-37 sd Undoc (description field is held blank) ; Non-SPI
 CMD50,57 sd Undoc (description field is held blank) ;/ |
| --- |

**Function Extension Commands (class 11)**

```

| CMD21 sd Reserved for DPS Specification (Data Protection System)
 CMD48 sd READ_EXTR_SINGLE (type=adtc)
 CMD49 sd WRITE_EXTR_SINGLE (type=adtc)
 CMD58 sd READ_EXTR_MULTI (type=adtc) ;SPI: READ_OCR
 CMD59 sd WRITE_EXTR_MULTI (type=adtc) ;SPI: CRC_ON_OFF |
| --- |

**MMC Data Streaming Commands (class 1/class 3)**

```

| CMD11 mmc READ_DAT_UNTIL_STOP (class 1) (type=adtc)
 CMD20 mmc WRITE_DAT_UNTIL_STOP (class 3) (type=adtc) |
| --- |

**Below CMD58-59 SPI-only (in Non-SPI mode: MMC=Reserved, SD=EXTR_MULTI)**

```

| CMD58 sd/mmc+spi READ_OCR ;SPI-only ;SD Mode: READ_EXTR_MULTI
 CMD59 sd/mmc+spi CRC_ON_OFF ;SPI-only ;SD Mode: WRITE_EXTR_MULTI |
| --- |

Above two commands are supported in SPI mode only, and are supported for both
SD and MMC (though newer MMC docs are no longer mentioning them since JEDEC
dropped SPI support).

**Application Specific Commands (prefixed by CMD55 aka APP_CMD)**

```

| ACMD6 sd SET_BUS_WIDTH (type=ac)
 ACMD13 sd spi SD_STATUS (type=adtc) (get 512bit SSR)
 ACMD22 sd spi GET_NUM_WR_BLOCKS (type=adtc)
 ACMD23 sd spi SET_WR_BLK_ERASE_COUNT (type=ac)
 ACMD41 sd spi SD_SEND_OP_COND (type=bcr) ;SPI: reduced functionality
 ACMD42 sd spi SET_CLR_CARD_DETECT (type=ac)
 ACMD51 sd spi GET_SCR (type=adtc)
 ACMD1-5 - Reserved
 ACMD7-12 - Reserved
 ACMD14-16 sd Reserved for DPS Specification (Data Protection System)
 ACMD17 - Reserved
 ACMD18 sd spi Reserved for SD security applications
 ACMD19-21 - Reserved
 ACMD24 - Reserved
 ACMD25 sd spi Reserved for SD security applications
 ACMD26 sd spi Reserved for SD security applications
 ACMD27 - Shall not use this command
 ACMD28 sd Reserved for DPS Specification (Data Protection System)
 ACMD29 - Reserved
 ACMD30-35 sd Reserved for Security Specification
 ACMD36-37 - Reserved
 ACMD38 sd spi Reserved for SD security applications
 ACMD39-40 - Reserved
 ACMD43-49 sd spi Reserved for SD security applications
 ACMD52-54 sd Reserved for Security Specification
 ACMD55 - Not exist (equivalent to CMD55)
 ACMD56-59 sd Reserved for Security Specification
 ACMD0 - Unknown/Unused/Undocumented
 ACMD50 - Unknown/Unused/Undocumented
 ACMD60-63 - Unknown/Unused/Undocumented |
| --- |

**Card Registers**

```

| CSR 32bit sd/mmc spi Card Status: command error & state information
 OCR 32bit sd/mmc spi Operation Conditions Register
 CID 128bit sd/mmc spi Card Identification
 CSD 128bit sd/mmc spi Card-Specific Data (CSD Version 1.0 and 2.0)
 RCA 16bit sd/mmc Relative Card Address (not used in SPI mode)
 DSR 16bit sd/mmc spi Driver Stage Register (optional)
 SSR 512bit sd spi SD Card Status Register: Extended status field
 SCR 64bit sd spi SD Card Configuration Register
 EXT_CSD 4096bit mmc spi MMC Extended CSD Register (status & config)
 PWD 128bit sd/mmc spi Password (Card Lock) (max 16 bytes)
 PWD_LEN 8bit sd/mmc spi Password Length (0..16 max) (0=no password) |
| --- |

**SD Mode Response Types**

```

| N/A 0bit CMD0, CMD4, CMD15 No response
 R1 48bit Normal CMDs/ACMDs 32bit CSR Card Status
 R1b 48bit Busy CMDs/ACMDs 32bit CSR Card Status (and DATA=busy)
 R2 136bit CMD9 120bit CSD Card-Specific Data
 R2 136bit CMD2, CMD10 120bit CID Card Identification
 R3 48bit ACMD41, MMC:CMD1 32bit OCR Register (without crc7)
 R4 - - Reserved for SDIO
 R5 - - Reserved for SDIO
 R6 48bit CMD3 16bit RCA and cut-down 16bit CSR
 R7 48bit CMD8 32bit Card interface condition |
| --- |

**SPI Mode Response Types**

```

| R1 8bit Normal CMDs/ACMDs 8bit CSR Card Status
 R1b 8bit Busy CMDs/ACMDs 8bit CSR Card Status (and DATA=busy)
 R2 16bit CMD13, ACMD13 16bit CSR Card Status
 R3 40bit CMD58 8bit CSR and 32bit OCR
 R4 - - Reserved for SDIO
 R5 - - Reserved for SDIO
 R6 - - Reserved
 R7 40bit CMD8 8bit CSR and 32bit Card interface condition
 ERROR 8bit Only first 8bit sent upon Illegal Command or Command CRC Error |
| --- |

**Commands with Data Transfers (additionally to command/response) (type=adtc)**

```

| CMD17,18 R sd/mmc spi READ_SINGLE_BLOCK, READ_MULTIPLE_BLOCK
 CMD24,25 W sd/mmc spi WRITE_BLOCK, WRITE_MULTIPLE_BLOCK
 CMD8 R mmc spi GET_EXT_CSD (4096bit)
 CMD9 R sd/mmc spi GET_CSD (128bit) ;\in SPI Mode only (Non-SPI mode
 CMD10 R sd/mmc spi GET_CID (128bit) ;/sends that info as CMD response)
 ACMD13 R sd spi SD_STATUS (512bit SSR register)
 ACMD22 R sd spi GET_NUM_WR_BLOCKS (32bit counter)
 ACMD51 R sd spi GET_SCR (64bit SCR register)
 CMD14,19 R/W mmc BUSTEST_R, BUSTEST_W
 CMD19 W? sd SET_TUNING_BLOCK (512bit tuning pattern)
 CMD27 W sd/mmc spi PROGRAM_CSD (128bit CSD register)
 CMD30 R sd/mmc spi GET_WRITE_PROT (32bit write-protect flags)
 CMD31 R mmc GET_WRITE_PROT_TYPE (32x2bit write-protect types)
 CMD42 W sd/mmc spi LOCK_UNLOCK (password header/data)
 CMD6 ?? sd spi SWITCH_FUNC
 CMD40 ? sd Defined by DPS Spec (Data Protection System)
 CMD48,49 R/W sd READ_EXTR_SINGLE, WRITE_EXTR_SINGLE
 CMD58,59 R/W sd READ_EXTR_MULTI, WRITE_EXTR_MULTI
 CMD56 R/W sd spi GEN_CMD
 CMD11 R mmc READ_DAT_UNTIL_STOP (class 1) (type=adtc)
 CMD20 W mmc WRITE_DAT_UNTIL_STOP (class 3) (type=adtc)
 xR1b R sd/mmc spi Busy signal for commands with "R1b" response |
| --- |

**Misnamed Commands**

Official command names include various SEND_xxx commands, which are misleading
because they don't indicate if they "send" information <to> or
<from> the card (or both). Better naming would be GET_xxx, SET_xxx, or
GET_SET_xxx.

```

| Official Name Renamed
 ALL_SEND_CID ALL_GET_CID
 SEND_CID GET_CID
 SEND_CSD GET_CSD
 SEND_STATUS GET_STATUS
 SEND_RELATIVE_ADDR GET_RELATIVE_ADDR
 SEND_SCR GET_SCR
 SEND_EXT_CSD GET_EXT_CSD
 SEND_WRITE_PROT GET_WRITE_PROT
 SEND_WRITE_PROT_TYPE GET_WRITE_PROT_TYPE
 SEND_NUM_WR_BLOCKS GET_NUM_WR_BLOCKS
 SEND_IF_COND SET_IF_COND ;-to card
 SEND_TUNING_BLOCK SET_TUNING_BLOCK ;-to card
 SEND_OP_COND ...
 SD_SEND_OP_COND ... |
| --- |

Other misnamed commands include SET_BLOCKLEN occassionally spelled
SET_BLOCK_LEN in SD specs. SELECT_DESELECT_CARD is officially spelled
SELECT/DESELECT_CARD.

**Difference of SD Commands Definition in UHS-II**

SD-TRAN driver of host should manage the difference of SD commands functions.
Not supported commands should not issue to UHS-II card. CMD13 shall not be
issued during data transfer. Normally, data transfer should be stopped by
setting TLEN instead of using CMD12.

CMD23 and CMD55 functions are included in UHS-II packet functions.

```

| CMD0 Terminate SD transaction and reset SD-TRAN state.
 CMD3 Returns Device ID in the response instead of RCA
 CMD4 Illegal
 CMD6 Function Group 1 and 3 are not used.
 CMD7 Device ID is set to the argument instead of RCA
 CMD13 Device operation is up to implementation during data transfer (eg. CTS)
 CMD11 Illegal
 CMD12 Normally, TLEN (data length) in UHS-II packet is used to stop data
 transfer.
 CMD12 Should be used to abort an operation when illegal situation occurs.
 CMD15 Illegal
 CMD19 Illegal
 CMD23 Not Affected. TLEN in UHS-II packet is used to specify data length.
 CMD55 Not Affected. ACMD is set by APP field in UHS-II packet.
 ACMD6 Illegal
 ACMD42 Illegal |
| --- |

Not Affected means that the command is not executed in any card state, and
response is returned (response type is up to implementation).

Illegal means that card returns response with NACK=1.

As SDHC/SDXC Cards do not support CMD28, 29 and 30, these commands are also
illegal in UHS-II mode.

**Note**

All future reserved commands shall have a codeword length of 48 bits, as well
as their responses (if there are any).

| DSi SD/MMC Protocol: General Commands |
| --- |

**CMD0 - SD/MMC - SPI - GO_IDLE_STATE (type=bc)**

Parameter bits:

```

| 31-0 stuff bits |
| --- |

SD Mode Response: N/A

SPI Mode Response: R1

Resets all cards to idle state, it's usually sent to (re-)invoke card detection
and initialization. The command does also seem to reset many further registers
(for example, TRAN_SPEED is said to be reset to 25MHz, and, although not
officially specified, the DSi's eMMC chip appears to get forced back to 1bit
data bus mode).

Observe that card detection/initialization should be done at lower CLK rate
than usually (MMC specifies max 400kHz - this is actually required - the DSi's
onboard Samsung KMAPF0000M-S998 eMMC chip won't respond to ALL_GET_CID when
trying to use 16MHz CLK), higher CLK can be used once when detecting max speed
(TRAN_SPEED in CSD register).

The command is also used to enter SPI mode (in SPI mode, the /CS pin is held
low, while in 1bit/4bit mode that pin would be DAT3=floating/high), SPI
commands can be sent without CRCs, however, at time when entering SPI mode,
memory cards may still insist on checksums, CMD0 should be thus always sent
with CRC7.

**CMD8 - SD (SD v2.00 and up) - SPI - SET_IF_COND (type=bcr)**

Parameter bits:

```

| 31-12 reserved bits
 11-8 supply voltage (VHS)
 7-0 check pattern |
| --- |

SD Mode Response: R7:

```

| 47 Start Bit (0) ;\
 46 Transmission To Host (0) ; 1st byte
 45-40 Command (the 6bit CMD being responded to) ;/
 39-20 Reserved (zero filled) (20bit) ;\2nd..4th byte
 19-16 Voltage accepted (see below) (4bit) ;/
 23-8 Echo-back of check pattern (8bit) ;-5th byte
 7-1 CRC7 ;\6th byte
 0 End Bit (1) ;/ |
| --- |

SPI Mode Response: R7:

```

| 39-32 R1 (8bit Card Status, same as in normal SPI command responses)
 31-28 Command version (???) (4bit)
 27-12 Reserved (0) (16bit)
 11-8 Voltage Accepted (see below) (4bit)
 7-0 Echo-back of check pattern (8bit) |
| --- |

Sends SD Memory Card interface condition, which includes host supply voltage
information and asks the card whether card supports voltage.

Voltage Accepted values:

```

| 0001b = 2.7-3.6V
 0010b = Reserved for Low Voltage Range
 0100b = Reserved
 1000b = Reserved
 Others = Not Defined |
| --- |

The card supported voltage information of 3.3V range power pin is sent by the
response of CMD8. Bits 19-16 indicate the voltage range that the card supports.
The card that accepted the supplied voltage returns R7 response. In the
response, the card echoes back both the voltage range and check pattern set in
the argument.

**CMD11 - SD - VOLTAGE_SWITCH (type=ac)**

Parameter bits:

```

| 31-0 reserved bits (0) |
| --- |

Response: R1

Switch to 1.8V bus signaling level.

**CMD12 - SD/MMC - SPI - STOP_TRANSMISSION (type=ac)**

Parameter bits:

```

| 31-0 stuff bits |
| --- |

Response: R1b

Additional Data Transfer (from card): Busy signal for "R1b" response

Forces the card to stop transmission (SPI: in Multiple Block Read Operation).

Note: Toshiba SD/MMC controllers are sending STOP_TRANSMISSION automatically.

**CMD15 - SD/MMC - GO_INACTIVE_STATE (type=ac)**

Parameter bits:

```

| 31-16 RCA
 15-0 reserved bits (0) |
| --- |

Response: N/A

Sends an addressed card into the Inactive State. This command is used when the
host explicitly wants to deactivate a card once and forever (and won't even
react to GO_IDLE_STATE) until next power-up (aka until ejecting/reinserting the
card).

**CMD59 - SD/MMC - SPI-ONLY (not Non-SPI Mode) - CRC_ON_OFF**

Supported in SPI Mode only (in Non-SPI mode, CMD59 would be: MMC=Reserved,
SD=WRITE_EXTR_MULTI)!

Parameter bits:

```

| 31-1 stuff bits
 0 CRC option (0=off, 1=on) |
| --- |

SPI Mode Response: R1

Default on power up is unknown. Also unknown if this does completely prevent
transmission of both CRC7 and CRC-CCITT values (especially in case of CID/CSD
registers that have the CRC7 "inside" of the "128bit" register). Also unknown
if CID/CSD are having "double" checksums (CRC7 plus CRC-CCITT) when
transferring them as DATA packet (instead of as normal command/response).

**ACMD6 - SD - SET_BUS_WIDTH (type=ac)**

```

| 31-2 stuff bits
 1-0 Bus width for Data transfers (0=1bit, 2=4bit, 1/3=reserved). |
| --- |

Response: R1

The supported widths can be found in SCR register. The current width is stored
in SSR register. Default width is 1bit on power up.

Note: MMC uses a different mechanism to change the bus-width (via EXT_CSD).

**ACMD42 - SD - SPI - SET_CLR_CARD_DETECT (type=ac)**

```

| 31-1 stuff bits
 0 set_cd (0=Disconnect, 1=Connect) |
| --- |

Response: R1

Connect/Disconnect the 50 KOhm pull-up resistor on CD/DAT3 pin of the card.

The pull-up might be intended for card detection (other than by using the
slot's card detect switch), and/or for sensing SPI mode (which would drag that
pin to LOW level when asserting /CS chip select).

During operation, disabling the pull-up might improve 4bit mode data transfers
(unless for card controllers which do rely on the card pull-up to be present).
The TC6387XB datasheet recommends external 100K pull-ups on DAT0-2, and only
47K on DAT3 (not quite sure why, unless Toshiba believed the parallel 50K+47K
pull-ups to sum up to 100K, rather than to 25K).

**CMD55 - SD/MMC (MMC: only in SPI-mode?) - SPI - APP_CMD (type=ac)**

```

| 31-16 RCA (SPI Mode: stuff bits)
 15-0 stuff bits |
| --- |

Response: R1

Used as prefix for application specific commands, ie. the next command will be
treated as "ACMDnn" rather than as normal "CMDnn".

As the name says, this was originally intended for "application specific"
extensions, however, in the SD Card protocol, it's also used for some ACMD's
that are part of the SD protocol.

**CMD56 - SD/MMC (MMC: only in SPI-mode?) - SPI - GEN_CMD (type=adtc)**

```

| 31-1 stuff bits
 0 RD/WR Direction (0=Write to Card, 1=Read from Card) |
| --- |

Response: R1

Additional Data Transfer (to/from card, depending on above R/W bit):

```

| General purpose data
 For SDSC, block length is set via SET_BLOCKLEN command.
 For SDHC/SDXC, block length is fixed to 512 bytes. |
| --- |

Used to transfer a data block to/from the card for general purpose/application
specific commands.

**CMD14 - MMC - BUSTEST_R (type=adtc) (MMC only, Reserved on SD)**

**CMD19 - MMC - BUSTEST_W (type=adtc) (MMC only, SET_TUNING_BLOCK on SD)**

```

| 31-0 stuff bits |
| --- |

Response: R1

Additional Data Transfer (to/from card):

```

| test pattern (2bit per DATA line? eg. 8bit pattern in 4bit-mode?) |
| --- |

MMC only. And, that, in Non-SPI mode only.

BUSTEST_W: Host sends the "bus TEST Data pattern" to card.

BUSTEST_R: Host reads the "REVERSED bus TESTING data pattern" from card.

The reversing is said to change a 2bit value of "01" into "10", unknown if that
means that the bit-order is reversed, or that the bits are inverted.

| DSi SD/MMC Protocol: Block Read/Write Commands |
| --- |

**CMD16 - SD/MMC - SPI - SET_BLOCKLEN (type=ac)**

```

| 31-0 Block length (for Block Read, Block Write, Lock, and GEN_CMD) |
| --- |

Response: R1

In the case of SDSC Card, this command sets the block length (in bytes) for all
following block commands (read, write, lock). Default block length is fixed to
512 Bytes. Set length is valid for memory access commands only if partial block
read operation are allowed in CSD.

In the case of SDHC/SDXC Cards, block length set by CMD16 command does not
affect memory read and write commands. Always 512 Bytes fixed block length is
used. This command is effective for LOCK_UNLOCK command.

In both cases, if block length is set larger than 512 Bytes, the card sets the
BLOCK_LEN_ERROR bit.

In DDR50 mode, block length must be even (because data is sampled on both clock
edges).

**CMD20 - SD (optional, see SCR.Bit32) - SPEED_CLASS_CONTROL (type=ac)**

```

| 31-28 Speed Class Control (for Block Read, and Block Write commands)
 27-0 Reserved (0) |
| --- |

Response: R1b

Additional Data Transfer (from card): Busy signal for "R1b" response

Speed Class control command. Refer to Section 4.13.2.8.

**CMD23 - SD/MMC - SPI (but only on MMC) - SET_BLOCK_COUNT (type=ac)**

Supported by SD and MMC Cards. However, in SPI-mode it's supported only for
MMC? And, for SD it's optional (see SCR.Bit33).

```

| 31-0 Block Count (MMC: only lower 16bit used, upper 16bit=reserved) |
| --- |

Response: R1

Specify block count for CMD18 and CMD25.

```

| ________________________ Block-Oriented READ Commands ________________________ |
| --- |

**CMD17 - SD/MMC - SPI - READ_SINGLE_BLOCK (type=adtc)**

```

| 31-0 data address (SDSC: in 1-byte units, SDHC/SDXC: in 512-byte units) |
| --- |

Response: R1

Additional Data Transfer (from card):

```

| data |
| --- |

In the case of SDSC Card, this command reads a block of the size selected by
the SET_BLOCKLEN command. The data transferred shall not cross a physical block
boundary unless READ_BLK_MISALIGN is set in the CSD.

In case of SDHC and SDXC Cards, block length is fixed 512 Bytes regardless of
the SET_BLOCKLEN command.

**CMD18 - SD/MMC - SPI - READ_MULTIPLE_BLOCK (type=adtc)**

```

| 31-0 data address (SDSC: in 1-byte units, SDHC/SDXC: in 512-byte units) |
| --- |

Response: R1

Additional Data Transfer (from card):

```

| data |
| --- |

Continuously transfers data blocks from card to host until interrupted by a
STOP_TRANSMISSION command.

Block length is specified the same as READ_SINGLE_BLOCK command.

**CMD19 - SD - SET_TUNING_BLOCK (type=adtc) (SD only, BUSTEST_W on MMC)**

```

| 31-0 reserved bits (0) |
| --- |

Response: R1

Additional Data Transfer (to card):

```

| 64 bytes (512bit) tuning pattern is sent for SDR50 and SDR104. |
| --- |

```

| _______________________ Block-Oriented WRITE Commands _______________________ |
| --- |

**CMD24 - SD/MMC - SPI - WRITE_BLOCK (type=adtc)**

```

| 31-0 data address (SDSC: in 1-byte units, SDHC/SDXC: in 512-byte units) |
| --- |

Response: R1

Additional Data Transfer (to card):

```

| data |
| --- |

In case of SDSC Card, block length is set by the SET_BLOCKLEN command. The data
transferred shall not cross a physical block boundary unless WRITE_BLK_MISALIGN
is set in the CSD. In the case that write partial blocks is not supported, then
the block length=default block length (given in CSD).

In case of SDHC and SDXC Cards, block length is fixed 512 Bytes regardless of
the SET_BLOCKLEN command.

**CMD25 - SD/MMC - SPI - WRITE_MULTIPLE_BLOCK (type=adtc)**

```

| 31-0 data address (SDSC: in 1-byte units, SDHC/SDXC: in 512-byte units) |
| --- |

Response: R1

Additional Data Transfer (to card):

```

| data |
| --- |

Continuously writes blocks of data until a STOP_TRANSMISSION follows.

Block length is specified the same as WRITE_BLOCK command.

**ACMD22 - SD - SPI - GET_NUM_WR_BLOCKS (type=adtc)**

```

| 31-0 stuff bits |
| --- |

Response: R1

Additional Data Transfer (from card):

```

| 31-0 Number of the written (without errors) write blocks (32bit) |
| --- |

Responds with 32bit+CRC data block.

If WRITE_BL_PARTIAL='0', the unit of ACMD22 is always 512 byte.

If WRITE_BL_PARTIAL='1', the unit of ACMD22 is a block length which was used
when the write command was executed.

**ACMD23 - SD - SPI - SET_WR_BLK_ERASE_COUNT (type=ac)**

```

| 31-23 stuff bits
 22-0 Number of blocks |
| --- |

Response: R1

Set the number of write blocks to be pre-erased before writing (to be used for
faster Multiple Block WR command). "1"=default (one wr block).

Command STOP_TRAN (CMD12) shall be used to stop the transmission in Write
Multiple Block whether or not the pre-erase (ACMD23) feature is used.

```

| _____________________ Byte-Streaming READ/WRITE Commands _____________________ |
| --- |

**CMD11 - MMC - READ_DAT_UNTIL_STOP (class 1) (type=adtc)**

**CMD20 - MMC - WRITE_DAT_UNTIL_STOP (class 3) (type=adtc)**

```

| 31-0 data address (SDSC: in 1-byte units, SDHC/SDXC: in 512-byte units) |
| --- |

Response: R1

Additional Data Transfer (to/from card):

```

| data |
| --- |

Similar to read/write multiple blocks, but transferring the data as an endless
byte stream (instead of splitting it into separate blocks). Transfer is
terminated by sending STOP_TRANSMISSION.

| DSi SD/MMC Protocol: Special Extra Commands |
| --- |

```

| _________________________ Write PROTECTION Commands _________________________ |
| --- |

**CMD28 - SDSC/MMC (not SDHC/SDXC) - SPI - SET_WRITE_PROT (type=ac)**

**CMD29 - SDSC/MMC (not SDHC/SDXC) - SPI - CLR_WRITE_PROT (type=ac)**

```

| 31-0 data address (SDSC: in 1-byte units, SDHC/SDXC: Unsupported) |
| --- |

Response: R1b

Additional Data Transfer (from card): Busy signal for "R1b" response

Write protection support is indicated in CSD(WP_GRP_ENABLE), and additionally
"class 6" should be flagged in CSD(CCC). The group size is indicated in
CSD(WP_GRP_SIZE), observe that that field is 5bit/7bit wide for SD/MMC
accordingly.

**CMD30 - SDSC/MMC (not SDHC/SDXC) - SPI - GET_WRITE_PROT (type=adtc)**

```

| 31-0 data address (SDSC: in 1-byte units, SDHC/SDXC: Unsupported) |
| --- |

Response: R1

Additional Data Transfer (from card):

```

| 31-0 Flags (1=write-protected) (bit0=addressed group, bit1..31=next groups) |
| --- |

If the card provides write protection features, this command asks the card to
send the status of the write protection bits: 32 write protection bits
(representing 32 write protect groups starting at the specified address)
followed by 16 CRC bits are transferred in a payload format via the DATA line.
The last (least significant) bit of the protection bits corresponds to the
first addressed group. If the addresses of the last groups are outside the
valid range, then the corresponding write protection bits shall be set to 0.

**CMD31 - MMC - GET_WRITE_PROT_TYPE (type=adtc)**

```

| 31-0 data address (SDSC: in 1-byte units, SDHC/SDXC: Unsupported) |
| --- |

Response: R1

Additional Data Transfer (from card):

```

| 63-0 Flags (1=write-protected) (bit0-1=addressed group, bit2..63=next) |
| --- |

Returns thirty-two 2bit values (0=not protected, 1=temporary write protection,
2=power-on write protection, 3=permanent write protection).

**Further Write-Protection Mechanisms**

The whole card can be write-protected via PERM_WRITE_PROTECT and
TMP_WRITE_PROTECT bits in CSD register (supported for MMC and SDSC/SDHC/SDXC).

SD Cards (and SD Card adaptors for miniSD and microSD cards) are additionally
having a mechanical "LOCK" write protection tab (MMC cards don't have that
feature).

The PWD feature provides Read/Write-protection (when not knowing the password).

```

| _______________________________ Erase Commands _______________________________ |
| --- |

**CMD32 - SD - SPI - ERASE_WR_BLK_START (type=ac)**

**CMD33 - SD - SPI - ERASE_WR_BLK_END (type=ac)**

```

| 31-0 data address (SDSC: in 1-byte units, SDHC/SDXC: in 512-byte units) |
| --- |

Response: R1

Sets the address of the first/last write block of the continuous range to be
erased.

**CMD35 - MMC - SPI - ERASE_GROUP_START (type=ac)**

**CMD36 - MMC - SPI - ERASE_GROUP_END (type=ac)**

```

| 31-0 data address (MMC: in WHAT units?) |
| --- |

Response: R1

MMC only. Unknown, maybe similar to above SD commands?

**CMD32-34,37 - SPI - MMC - Reserved for compatibility with older MMC cards**

MMC only. Unknown, maybe also Erase related?

**CMD38 - SD/MMC - SPI - ERASE (type=ac)**

```

| 31-0 stuff bits |
| --- |

Response: R1b

Additional Data Transfer (from card): Busy signal for "R1b" response

Erases all previously selected write blocks.

**Further Erase Commands**

Sectors are automatically erased on-the-fly when writing data blocks, so
manually using above erase commands isn't really necessary; it may be useful
for shreddering private data though, and it might also speed up subsequent
writes since the writes can omit the on-the-fly erasing step.

The SET_WR_BLK_ERASE_COUNT (ACMD23) can be used to notify the card that it may
pre-erase multiple sectors upon write commands (eg. to speed-up cluster writes
that are spanning across multiple sectors).

The password lock feature includes a Forced Erase function, which will reset
the password, and ERASE THE WHOLE CARD, this may be useful if the user has
forgot the password, but will destroy data (possibly including the
pre-formatted filesystem headers; which would be bad, because that headers
should contain cluster sizes somewhat matched to the physical sector sizes).

```

| ________________________________ I/O Commands ________________________________ |
| --- |

**CMD5 - SD - SPI - Reserved for I/O cards**

**CMD52-54 - SD - SPI - Commands for SDIO**

```

| CMD5 SDIO: IO_SEND_OP_COND
 CMD52 SDIO: IO_RW_DIRECT
 CMD53 SDIO: IO_RW_EXTENDED |
| --- |

Refer to the "SDIO Card Specification". SDIO is an extension to the SD protocol
that allows to access non-memory-card hardware (such like cameras or network
adaptors).

```

| __________________________ Switch Function Commands __________________________ |
| --- |

**CMD6 - SD (SD v1.10 and up) - SPI - SWITCH_FUNC (type=adtc)**

```

| 31 Mode (0=Check function, 1=Switch function)
 30-24 reserved (All '0')
 23-20 function group 6: Reserved (0h or Fh)
 19-16 function group 5: Reserved (0h or Fh)
 15-12 function group 4: Power Limit ;SPI Mode: Reserved (0h or Fh)
 11-8 function group 3: Drive Strength ;SPI Mode: Reserved (0h or Fh)
 7-4 function group 2: Command System
 3-0 function group 1: Access Mode |
| --- |

Response: R1

Additional Data Transfer (to/from whatever):

```

| unknown |
| --- |

Checks switch-able function (mode 0) and switch card function (mode 1). See
Chapter 4.3.10.

**CMD34-35 - SD - Reserved**

Reserved for each command system set by switch function command (CMD6).

Detailed definition is referred to each command system specification.

Maybe related to above "function group 5..6"?

**CMD36,37 - SD - Undoc (description field is held blank)**

**CMD50,57 - SD - Undoc (description field is held blank)**

Undoc. Maybe related to above "function group 1..4"?

**SPI: CMD34-37 - SD - SPI - Reserved for Command Systems from CMD6**

**SPI: CMD50,57 - SD - SPI - Reserved for Command Systems from CMD6**

Described as so for SPI Mode. Maybe related to above "function group 1..6"?

```

| ________________________ Function Extension Commands ________________________ |
| --- |

**CMD21 - SD - Reserved for DPS Specification (Data Protection System)**

Reserved.

**CMD48 - SD (optional, see SCR.Bit34) - READ_EXTR_SINGLE (type=adtc)**

```

| 31 MIO (0=Memory, 1=I/O)
 30-27 FNO
 26 Reserved (0)
 25-9 ADDR
 8-0 LEN |
| --- |

Response: R1

Additional Data Transfer (from card):

```

| whatever |
| --- |

Single block read type. Refer to Section 5.7.2.1.

**CMD49 - SD (optional, see SCR.Bit34) - WRITE_EXTR_SINGLE (type=adtc)**

```

| 31 MIO (0=Memory, 1=I/O)
 30-27 FNO
 26 MW
 25-9 ADDR
 8-0 LEN/MASK |
| --- |

Response: R1

Additional Data Transfer (to card):

```

| whatever |
| --- |

Single block write type. Refer to Section 5.7.2.2.

**CMD58 - SD (optional, see SCR.Bit35) - READ_EXTR_MULTI (type=adtc)**

```

| 31 MIO (0=Memory, 1=I/O)
 30-27 FNO
 26 BUS (0=512B, 1=32KB)
 25-9 ADDR
 8-0 BUC |
| --- |

Response: R1

Additional Data Transfer (from card):

```

| whatever |
| --- |

Multi-block read type. Refer to Section 5.7.2.4.

**CMD59 - SD (optional, see SCR.Bit35) - WRITE_EXTR_MULTI (type=adtc)**

```

| 31 MIO (0=Memory, 1=I/O)
 30-27 FNO
 26 BUS (0=512B, 1=32KB)
 25-9 ADDR
 8-0 BUC |
| --- |

Response: R1

Additional Data Transfer (to card):

```

| whatever |
| --- |

Multi-block write type. Refer to Section 5.7.2.5.

Note: CCC bit 11 is set to 1 when any command of class 11 is supported.
Supporting of these commands is indicated in SCR register.

| DSi SD/MMC Protocol: CSR Register (32bit Card Status Register) |
| --- |

**CMD13 - SD/MMC - SPI - GET_STATUS (type=ac)**

Parameter bits:

```

| 31-16 RCA (SPI Mode: stuff bits)
 15-0 stuff bits |
| --- |

SD Mode Response: R1 (32bit Card Status):

```

| 47 Start Bit (0) ;\
 46 Transmission To Host (0) ; 1st byte
 45-40 Command (the 6bit CMD being responded to) ;/
 39-8 CSR Card Status Register (32bit) (see below) ;-2nd..5th byte
 7-1 CRC7 ;\6th byte
 0 End Bit (1) ;/ |
| --- |

SPI Mode Response: R2 (16bit Card Status):

```

| 15-0 CSR Card Status Register (16bit) (see below) ;-1st..2nd byte |
| --- |

Addressed card sends its status register.

**CMDxx/ACMDxx - Other Commands**

Most other commands are also returning the Card Status in their responses:

SD Mode Response: R1 (32bit Card Status)

SPI Mode Response: R1 (8bit Card Status; most SPI commands return only 8bit)

SPI Mode Response: R2 (16bit Card Status; SPI commands CMD13/ACMD return 16bit)

**CMDxx/ACMDxx - Other Commands with R1b Response**

R1b is identical to R1, with an optional busy signal transmitted on the DATA
line (R1b occurs for CMD7, CMD12, CMD20, CMD28, CMD29, CMD38) (and for MMC:
also for CMD5, CMD6). The card may become busy after receiving these commands
based on its state prior to the command reception. The Host shall check for
busy at the response.

In SD Mode, the busy signal is sent on DAT0 line (DAT1-3 aren't used, even if
the card is in 4bit mode). The busy signal does consist of BITs? (not bytes?),
and has a "start bit"?, followed by what-value-when-busy? and
what-final-value-when-ready?

In SPI Mode, the busy signal is sent as BYTEs (00h=Busy, xxh=Nonzero=Ready).

**CSR Card Status Register (full 32bit, as returned in SD Mode Response: R1)**

```

| Bit Typ Clr Identifier Meaning
 31 ERX C OUT_OF_RANGE (1=Command's argument was out of range)
 30 ERX C ADDRESS_ERROR (1=Misaligned address/block len mismatch)
 29 ERX C BLOCK_LEN_ERROR (1=Wrong block length, bytelen mismatch)
 28 ER C ERASE_SEQ_ERROR (1=Error in erase command sequence)
 27 ERX C ERASE_PARAM (1=Wrong erease selection of write-blocks)
 26 ERX C WP_VIOLATION (1=Write failed due to write-protection)
 25 SX A CARD_IS_LOCKED (1=Card is locked by the host)
 24 ERX C LOCK_UNLOCK_FAILED (1=Lock/unlock sequence or password error)
 23 ER B COM_CRC_ERROR (1=CRC check of previous command failed)
 22 ER B ILLEGAL_COMMAND (1=Command not legal for the card state)
 21 ERX C CARD_ECC_FAILED (1=Internal error correction failed)
 20 ERX C CC_ERROR (1=Internal card controller error)
 19 ERX C ERROR (1=General error, or Unknown error)
 18 - - Reserved (eMMC: UNDERRUN)
 17 - - Reserved (eMMC: OVERRUN) (eSD: DEFERRED_RESPONSE)
 16 ERX C CSD_OVERWRITE (1=read-only CSD section doesn't match card
 content, or attempted to reverse the
 Copy/WP bits)
 15 ERX C WP_ERASE_SKIP (1=partial erase error due to write-protect)
 14 SX A CARD_ECC_DISABLED (1=Internal error correction wasn't used)
 13 SR C ERASE_RESET (1=Erase sequence was aborted)
 12-9 SX B CURRENT_STATE (00h..0Fh=state, see below)
 8 SX A READY_FOR_DATA (1=Ready/buffer is empty)
 7 EX C SWITCH_ERROR (1=SWITCH command refused, MMC only)
 6 - - Reserved/Unspecified (description is left blank)
 5 SR C APP_CMD (1=Card will expect ACMD)
 4 - - Reserved for SD I/O Card
 3 ER C AKE_SEQ_ERROR (1=Authentication Sequence Error)
 2 - - Reserved for application specific commands
 1-0 - - Reserved for manufacturer test mode |
| --- |

Values for CURRENT_STATE (bit12-9):

```

| These bits indicate the OLD state of card when receiving the command,
 (ie. if the command does change the state, then the NEW state won't be
 seen until the NEXT command returns the new updated status bits)
 00h = idle
 01h = ready
 02h = ident
 03h = stby
 04h = tran ;<-- normal state (when waiting for read/write commands)
 05h = data ;data read (CMD8,CMD11,CMD17,CMD18,CMD30,CMD56/R)
 06h = rcv ;data write (CMD20?,CMD24,CMD25,CMD26,CMD27,CMD42,CMD56/W)
 07h = prg ;erase/wprot (CMD6,CMD28,CMD29,CMD38)
 08h = dis
 09h = btst ;bus test write (CMD19, MMC only)
 0Ah = slp ;sleep (CMD5, MMC only)
 0Bh-0Eh = reserved
 0Fh = reserved for I/O mode (SDIO-only devices, without SD-memory)
 N/A = ina ;inactive (CMD15) (card is killed, and can't send status)
 N/A = irq ;interrupt mode (CMD40, MMC only)
 N/A = pre ;pre-idle (MMC only) |
| --- |

Type aka Typ column (in above table):

```

| E: Error bit.
 S: Status bit.
 R: Flag may get set within response of current command.
 X: Flag may get set within response of NEXT command (with R1 response) |
| --- |

Clear Condition aka Clr column (in above table):

```

| A: According to the card current state.
 B: Always related to the previous command. Reception of a valid command
 will clear it (with a delay of one command).
 C: Clear by read. |
| --- |

**SPI Responses (8bit "R1" Responses, and 16bit "R2" Responses)**

```

| FIRST BYTE of all SPI Responses:
 7 always 0 ;\
 6 parameter error ; These 8bit are returned in ALL normal
 5 address error ; SPI commands (with 8bit "R1" response)
 4 erase sequence error ; and,
 3 com crc error ; the same 8bits are also returned
 2 illegal command ; as FIRST BYTE in SPI commands with
 1 erase reset ; longer responses
 0 in idle state ;/
 SECOND BYTE of SPI "R2" Response:
 7 out of range, or csd overwrite ;\
 6 erase param ;
 5 wp violation ; These extra 8bits are returned
 4 card ecc failed ; as SECOND BYTE in SPI commands
 3 CC error ; with 16bit "R2" status response
 2 error ; (ie. in CMD13 and ACMD13)
 1 wp erase skip, or lock/unlock cmd failed ;
 0 Card is locked ;/ |
| --- |

**Card Status Field/Command - Cross Reference**

For each command responded by R1 response, following table defines the affected
bits in the status field. An 'x' means the error/status bit may be set in the
response to the respective command.

```

| Bits 31 30 29 28 27 26 25 24 23 22 21 20 19 18 17 16 15 14 13 12-9 8 5
 CMD3 x x x x
 CMD6 x x x x x x x x
 CMD7 x x x x x x x x x x x x x x x
 CMD11 x x x x x
 CMD12 x x x x x x x x x x x
 CMD13 x x x x x x x x x x x x x x x x
 CMD16 x x x x x x x x x x x x x x x
 CMD17 x x x x x x x x x x x x x x x x
 CMD18 x x x x x x x x x x x x x x x x
 CMD19 x x x x x x x x x x x x x x x x
 CMD20 x x x x x x x x x x x x x x x x x x
 CMD23 x x x x x x x x x x x x x x x x x
 CMD24 x x x x x x x x x x x x x x x x x x
 CMD25 x x x x x x x x x x x x x x x x x x
 CMD26 x x x x x x x x x x x x x x
 CMD27 x x x x x x x x x x x x x x
 CMD28 x x x x x x x x x x x x x x x
 CMD29 x x x x x x x x x x x x x x x
 CMD30 x x x x x x x x x x x x x x x
 CMD32 x x x x x x x x x x x x x x x x
 CMD33 x x x x x x x x x x x x x x x x
 CMD38 x x x x x x x x x x x x x x x
 CMD42 x x x x x x x x x x x x x x
 CMD48 x x x x x x x x x x x x x x x x x x
 CMD49 x x x x x x x x x x x x x x x x x x
 CMD55 x x x x x x x x x x x x x x x
 CMD56 x x x x x x x x x x x x x x x x
 CMD58 x x x x x x x x x x x x x x x x x x
 CMD59 x x x x x x x x x x x x x x x x x x
 ACMD6 x x x x x x x x x x x x x x x x
 ACMD13 x x x x x x x x x x x x x x x
 ACMD22 x x x x x x x x x x x x x x x
 ACMD23 x x x x x x x x x x x x x x x
 ACMD42 x x x x x x x x x x x x x x x
 ACMD51 x x x x x x x x x x x x x x x |
| --- |

Note: The response to CMD3 is R6 that includes only bits 23, 22, 19 and 12:9
out of the Card Status.

| DSi SD/MMC Protocol: SSR Register (512bit SD Status Register) |
| --- |

**ACMD13 - SD - SPI - SD_STATUS (type=adtc)**

```

| 31-0 stuff bits |
| --- |

SD Mode Response: R1 (32bit Card Status)

SPI Mode Response: R2 (16bit Card Status) (same as for CMD13, see there)

Additional Data Transfer (from card):

```

| 511-0 SSR Register (512bit) |
| --- |

Send the SD Status. The status fields are given in Table 4-43.

**SD Status (transferred on DATA line after ACMD13)**

The size of the SD Status is one data block of 512 bit. The content of this
register is transmitted to the Host over the DAT bus along with a 16-bit CRC.

ACMD13 can be sent to a card only in 'tran_state' (card is selected).

```

| Bits Type Clr Identifier
 511-510 SR A DAT_BUS_WIDTH (0..3, see below)
 509 SR A SECURED_MODE (0=Normal, 1=Secured) (Part 3 Security Specs)
 508-502 - - Reserved for Security Functions (Part 3 Security Specs)
 501-496 - - Reserved
 495-480 SR A SD_CARD_TYPE (0..FFFFh, see below)
 479-448 SR A SIZE_OF_PROTECTED_AREA Size of protected area (see below)
 447-440 SR A SPEED_CLASS Speed Class of the card (see below)
 439-432 SR A PERFORMANCE_MOVE Performance of move indicated by 1 MB/s step
 431-428 SR A AU_SIZE Size of AU (see below)
 427-424 - - Reserved
 423-408 SR A ERASE_SIZE Number of AUs to be erased at a time
 407-402 SR A ERASE_TIMEOUT Timeout value for erasing areas
 specified by UNIT_OF_ERASE_AU (see below)
 401-400 SR A ERASE_OFFSET Fixed offset value added to erase time
 399-396 SR A UHS_SPEED_GRADE Speed Grade for UHS mode (see below)
 395-392 SR A UHS_AU_SIZE Size of AU for UHS mode (see below)
 391-312 - - Reserved
 311-0 - - Reserved for manufacturer |
| --- |

**Values for DAT_BUS_WIDTH (as set via SET_BUS_WIDTH command):**

```

| 00h = 1 bit width (default)
 01h = reserved
 02h = 4 bit width
 03h = reserved |
| --- |

**Values for SD_CARD_TYPE**

```

| 0000h = Regular SD RD/WR Card
 0001h = SD ROM Card
 0002h = OTP
 0004h,0008h,0010h,0020h,0040h,0080h = Reserved for future variations
 01xxh..FFxxh = Reserved for Cards that don't comply to Physical Layer Specs |
| --- |

**Values for SIZE_OF_PROTECTED_AREA**

Setting this field differs between SDSC and SDHC/SDXC.

In case of SDSC Card, the capacity of protected area is calculated as follows:

```

| Protected Area = SIZE_OF_PROTECTED_AREA_* MULT * BLOCK_LEN.
 SIZE_OF_PROTECTED_AREA is specified by the unit in MULT*BLOCK_LEN. |
| --- |

In case of SDHC and SDXC Cards, the capacity of protected area is calculated as
follows:

```

| Protected Area = SIZE_OF_PROTECTED_AREA
 SIZE_OF_PROTECTED_AREA is specified by the unit in byte. |
| --- |

**Values for SPEED_CLASS**

This 8-bit field indicates the Speed Class. Classes lower than indicated by
this field are also effective.

```

| 00h Speed Class 0
 01h Speed Class 2
 02h Speed Class 4
 03h Speed Class 6
 04h Speed Class 10
 05h-FFh Reserved for future/faster classes |
| --- |

Application Note:

If a Class value indicated in SD Status (including reserved value) is larger
than that of host supported, the host should read as any Class can be used with
the card.

For example, Class 10 is indicated, host should consider Class 2 to 6 is also
effective.

**Values for PERFORMANCE_MOVE**

This 8-bit field indicates Pm and the value can be set by 1 [MB/sec] step.

If the card does not move used RUs, Pm should be considered as infinity.

Setting to FFh means infinity.

Pm is defined for Class 2 to 6 in Default Speed Mode. When host uses Class 10,
Pm indicated in SD Status shall be ignored and treated as 0.

```

| 00h Sequential Write
 01h 1 [MB/sec]
 02h 2 [MB/sec]
 ... ...
 FEh 254 [MB/sec]
 FFh Infinity |
| --- |

**Values for AU_SIZE**

This 4-bit field indicates AU Size and the value can be selected from 16 KB.

```

| 00h Not Defined
 01h 16 KB
 02h 32 KB
 03h 64 KB
 04h 128 KB
 05h 256 KB
 06h 512 KB
 07h 1 MB
 08h 2 MB
 09h 4 MB
 0Ah 8 MB
 0Bh 12 MB (!)
 0Ch 16 MB
 0Dh 24 MB (!)
 0Eh 32 MB
 0Fh 64 MB |
| --- |

Maximum AU size (depending on card capacity):

```

| Card Capacity up to 64MB up to 256MB up to 512MB up to 32GB up to 2TB
 Maximum AU Size 512 KB 1 MB 2 MB 4 MB1 64MB |
| --- |

The card can set any AU size (up to above maximum AU size values).

The card should set smaller AU size as much as possible.

Application Notes:

The host should determine host buffer size based on total busy time of 4MB and
the card supported class. The host can treat multiple AUs combined as one unit.

**Values for ERASE_SIZE**

This 16-bit field indicates NERASE. When NERASE numbers of AUs are erased, the
timeout value is specified by ERASE_TIMEOUT (Refer to ERASE_TIMEOUT).

The host should determine proper number of AUs to be erased in one operation so
that the host can indicate progress of erase operation.

```

| 0000h Erase Time-out Calculation is not supported.
 0001h 1 AU
 0002h 2 AU
 0003h 3 AU
 ... ...
 FFFFh 65535 AU |
| --- |

**Values for ERASE_TIMEOUT**

This 6-bit field indicates the TERASE and the value indicates erase timeout
from offset when multiple AUs are erased as specified by ERASE_SIZE. The range
of ERASE_TIMEOUT can be defined as up to 63 seconds and the card manufacturer
can choose any combination of ERASE_SIZE and ERASE_TIMEOUT depending on the
implementation. Once ERASE_TIMEOUT is determined, it determines the ERASE_SIZE.
The host can determine timeout for any number of AU erase by the Equation (6).
Refer to 4.14 for the concept of calculating erase timeout. If ERASE_SIZE field
is set to 0, this field shall be set to 0.

```

| 00h Erase Time-out Calculation is not supported.
 01h 1 [sec]
 02h 2 [sec]
 03h 3 [sec]
 ... ...
 3Fh 63 [sec] |
| --- |

**Values for ERASE_OFFSET**

This 2-bit field indicates the TOFFSET and one of four values can be selected.
The erase offset adjusts the line by moving in parallel on the upper side.
Refer to Figure 4-57 and Equation (6) in 4.14. This field is meaningless if
ERASE_SIZE and ERASE_TIMEOUT fields are set to 0.

```

| 00h 0 [sec]
 01h 1 [sec]
 02h 2 [sec]
 03h 3 [sec] |
| --- |

**Values for UHS_SPEED_GRADE**

This 4-bit field indicates the UHS mode Speed Grade. Reserved values are for
future speed grades larger than the highest defined value. Host shall treat
reserved values (undefined) as highest grade defined.

```

| 00h Less than 10MB/sec
 01h 10MB/sec and above
 02h-0Fh Reserved |
| --- |

**Values for UHS_AU_SIZE**

This 4-bit field indicates AU Size for UHS-I and UHS-II cards. Card should set
smaller value as much as possible. Host shall refer to UHS_AU_SIZE instead of
AU_SIZE when the card is operating in UHS-I or UHS-II bus speed modes.

```

| 00h Not Defined
 01h-06h Not Used
 07h 1 MB
 08h 2 MB
 09h 4 MB
 0Ah 8 MB
 0Bh 12 MB (!)
 0Ch 16 MB
 0Dh 24 MB (!)
 0Eh 32 MB
 0Fh 64 MB |
| --- |

| DSi SD/MMC Protocol: OCR Register (32bit Operation Conditions Register) |
| --- |

CMD1 (MMC) and ACMD41 (SD) are intended to exchange OCR information. That is,
the OCR parameter bits should indicate the host conditions (eg. for DSi:
40100000h, ie. bit20=3.3V supply, and bit30=HCS support for High Capacity carts
with more than 2GBytes). The OCR response may then return something like
007f8000h when busy, and 807f8000h when ready (bit20 indicating the voltage
being actually supported, bit30 indicating if it's High Capacity card, and
bit31 indicating if the card is ready & switched from "idle" to "ready"
state).

Cards do usually send at least one response with bit31=0 (busy), one should
repeat sending CMD1/ACMD51 until bit31=1 (ready).

Note: All card(s) on the bus will respond to CMD1/ACMD51: with the response
bits ANDed together (thus returning nonsense in bit30=HCS when actually sharing
the same bus for multiple cards).

Note: The card switches to "ina" state if the voltage in param bits isn't
supported.

**CMD1 - SD/MMC (For SD Cards: SPI-only) - SPI - SEND_OP_COND**

Parameter For MMC Cards (supported in SPI and Non-SPI mode):

```

| 31-0 OCR without busy (ie. without the power-up busy flag in bit31) |
| --- |

Parameter For SD Cards (supported in SPI mode only, not in Non-SPI mode):

```

| 31 Reserved (0) ;\special case (applies
 30 HCS (Host Capacity Support information) ; to SD-cards in SPI-mode
 29-0 Reserved (0) ;/only) |
| --- |

SD Response: R1

MMC Response: R3 (same/similar as SD Mode's ACMD41 response, see below)

Sends host capacity support information and activates the card's initialization
process. HCS is effective when card receives SET_IF_COND command.

**ACMD41 - SD - SPI - SD_SEND_OP_COND (type=bcr)**

```

| 31 reserved bit
 30 HCS(OCR[30]) (Host Capacity Support information)
 29 reserved for eSD ;\
 28 XPC Max Power Consumption (watts); SPI Mode: Reserved
 27-25 reserved bits ; (ie. only bit30 is used for SPI)
 24 S18R ; (ie. ACMD41 is SAME as SPI CMD1 ?)
 23-0 VDD Voltage Window(OCR[23-0]) ;/ |
| --- |

SD Mode Response: R3:

```

| 47 Start Bit (0) ;\
 46 Transmission To Host (0) ; 1st byte
 45-40 Reserved (111111) (instead of Command value) ;/
 39-8 OCR (32bit) ;-2nd..5th byte
 7-1 Reserved (111111) (instead of CRC7) ;\6th byte
 0 End Bit (1) ;/ |
| --- |

SPI Mode Response: R1 (without extra Data transfer? use READ_OCR instead?)

Sends host capacity support information (HCS) and asks the accessed card to
send its operating condition register (OCR) content in the response on the CMD
line. HCS is effective when card receives SET_IF_COND command (uh, but IF_COND
should be set BEFORE setting OP_COND?).

Sends request to switch to 1.8V signaling (S18R).

Reserved bit shall be set to '0'. CCS bit is assigned to OCR[30].

XPC controls the maximum power in the default speed mode of SDXC card.

```

| XPC=0: 0.36W (100mA at 3.6V on VDD1) (max) but speed class is not supported.
 XPC=1: 0.54W (150mA at 3.6V on VDD1) (max) and speed class is supported. |
| --- |

**CMD58 - SD/MMC - SPI-ONLY (not Non-SPI Mode) - READ_OCR**

Supported on SD Cards in SPI Mode only (in Non-SPI mode, CMD58 would be:
MMC=Reserved, SD=READ_EXTR_MULTI)!

Parameter bits:

```

| 31-0 stuff bits |
| --- |

SPI Mode Response: R3:

```

| 39-32 R1 (8bit Card Status, same as in normal SPI command responses)
 31-0 OCR (32bit) |
| --- |

**OCR register**

The 32-bit operation conditions register stores the VDD voltage profile of the
non UHS-II card and VDD1 voltage profile of the UHS-II card. Additionally, this
register includes status information bits. One status bit is set if the card
power up procedure has been finished. This register includes another status bit
indicating the card capacity status after set power up status bit. The OCR
register shall be implemented by the cards.

The 32-bit operation conditions register stores the VDD voltage profile of the
card.

Bit 7 of OCR is newly defined for Dual Voltage Card and set to 0 in default. If
a Dual Voltage Card does not receive CMD8, OCR bit 7 in the response indicates
0, and the Dual Voltage Card which received CMD8, sets this bit to 1.

```

| 31 Card power up status bit (0=Busy, 1=Ready)
 30 Card Capacity Status (CCS) (valid only if above Bit31 indicates Ready)
 CCS=0 SDSC Card (addressed in 1-byte units) ;MMC max 2GB
 CCS=1 SDHC/SDXC card (addressed in 512-byte units) ;MMC > 2GB
 29 UHS-II Card Status
 28-25 Reserved
 24 Switching to 1.8V Accepted (S18A) (Only UHS-I card supports this bit)
 23 3.5-3.6 ;\
 22 3.4-3.5 ;
 21 3.3-3.4 ;
 20 3.2-3.3 ;<-- this used by DSi ;
 19 3.1-3.2 ; VDD Voltage Window
 18 3.0-3.1 ;
 17 2.9-3.0 ;
 16 2.8-2.9 ;
 15 2.7-2.8 ;
 14-8 Reserved (MMC: 2.0V .. 2.6V) ; ;<-- uh, probably in opposite order?
 7 Reserved for Low Voltage Range ;
 6-4 Reserved ;
 3-0 Reserved ;/ |
| --- |

The supported voltage range is coded as shown in Table 5-1. A voltage range is
not supported if the corresponding bit value is set to LOW. As long as the card
is busy, the corresponding bit (31) is set to LOW.

VDD Voltage Window of OCR indicates VDD1 voltage range in case of UHS-II Card.

UHS-II Card Status bit is added in Bit 29 to indicate whether the card supports
UHS-II Interface. Non UHS-II Card sets Bit 29 to 0 and UHS-II Card sets Bit 29
to 1. This bit is not affected by whether VDD2 is supplied or not.

| DSi SD/MMC Protocol: CID Register (128bit Card Identification) |
| --- |

**CMD2 - SD/MMC - ALL_GET_CID (type=bcr)**

CMD2 is/was intended for multiple MMC cards on the same SD/MMC bus, the
connected card(s) should compare the CMD2 response bits seen on the bus, and
the card with the smallest CID number is switched to "ident" state (and any
other cards stay in "ready" state until sending further CMD2's).

CMD2 is still required for both SD and MMC during initialization, although
actually sharing the same bus for multiple cards is rather uncommon/depracted
(and might envolve various problems: Like conflicting OCR responses,
conflicting pull-ups on DAT3 pin, signal noise/spikes on insertion/removal of a
second card while accessing another card, problems with (shared) Write Protect
and Card Detect switches, and so on).

Parameter bits:

```

| 31-0 stuff bits |
| --- |

SD Mode Response: R2 (same 136bit response as for CMD10, see there)

Asks any card to send the CID numbers on the CMD line (any card that is
connected to the host will respond - until it sees a "0" bit from another card
while itself outputting a "1" bit).

Observe that CMD2 (and other card detection/initialization commands) should be
done at lower CLK rate than usually (MMC specifies max 400kHz - this is
actually required - the DSi's onboard Samsung KMAPF0000M-S998 eMMC chip won't
respond to ALL_GET_CID when trying to use 16MHz CLK), higher CLK can be used
once when detecting max speed (TRAN_SPEED in CSD register).

**CMD10 - SD/MMC - SPI - GET_CID (type=ac)**

This command should be used for actually READING the CID (as opposed to
ALL_GET_CID which is primarily intended for the connected card(s) to COMPARE
their CIDs with each other).

Parameter bits:

```

| 31-16 RCA (SPI Mode: stuff bits)
 15-0 stuff bits |
| --- |

SD Mode Response: R2:

```

| 135 Start Bit (0) ;\
 134 Transmission To Host (0) ; 1st byte
 133-128 Reserved (111111) (instead of Command value) ;/
 127-8 CID (120bit) (15 bytes) ;\aka 128bit ;-2nd..16th byte
 7-1 CRC7 ; when including ;\17th byte
 0 End Bit (1) ;/CRC7+EndBit ;/ |
| --- |

SPI Mode Response: R1, plus DATA line,

SPI Mode Additional Data Transfer (from card):

```

| 127-0 CID (128bit) ... or 120bit ? |
| --- |

Addressed card sends its card identification (CID).

**CID register**

The Card IDentification (CID) register is 128 bits wide. It contains the card
identification information used during the card identification phase. Every
individual Read/Write (RW) card shall have a unique identification number.

For SD Cards (short product name, but bigger date field, 2000..2255?):

```

| Bit Siz Field Name
 127-120 8 MID Manufacturer ID (binary) ;\assigned by SD-3C, LLC
 119-104 16 OID OEM/Application ID (ASCII) ;/
 103-64 40 PNM Product name (ASCII)
 63-56 8 PRV Product revision (BCD, 00h-99h) (eg 62h = rev 6.2)
 55-24 32 PSN Product serial number (32bit)
 23-20 4 - Reserved (zero)
 19-8 12 MDT Manufacturing date (yymh) (m=1..12, yy=0..255?; +2000)
 7-1 7 CRC CRC7 checksum
 0 1 1 Stop bit (always 1) |
| --- |

For MMC Cards (smaller date field, range 1997..2012 only):

```

| Bit Siz Field Name
 127-120 8 MID Manufacturer ID (binary) ;\assigned by MMCA
 119-104 16 OID OEM/Application ID (binary) ;/ ... or ...
 127-120 8 MID Manufacturer ID (binary) ;\assigned by MMCA/JEDEC
 119-114 6 - Reserved (0) ;
 113-112 2 CBX Device (0=Card, 1=BGA, 2=POP) ;
 119-104 8 OID OEM/Application ID (binary) ;/
 103-56 48 PNM Product name (ASCII)
 55-48 8 PRV Product revision (BCD, 00h-99h) (eg 62h = rev 6.2)
 47-16 32 PSN Product serial number (32bit)
 15-8 8 MDT Manufacturing date (myh) (m=1..12, y=0..15; +1997)
 7-1 7 CRC CRC7 checksum
 0 1 1 Stop bit (always 1) |
| --- |

**Known CID's for DSi eMMC chips (excluding CRC in LSB, padded 00 in MSB)**

```

| MY ss ss ss ss 03 4D 30 30 46 50 41 00 00 15 00 ;DSi Samsung KMAPF0000M-S998
 MY ss ss ss ss 32 57 37 31 36 35 4D 00 01 15 00 ;DSi Samsung KLM5617EFW-B301
 MY ss ss ss ss 30 36 35 32 43 4D 4D 4E 01 FE 00 ;DSi ST NAND02GAH0LZC5 rev30
 MY ss ss ss ss 31 36 35 32 43 4D 4D 4E 01 FE 00 ;DSi ST NAND02GAH0LZC5 rev31
 MY ss ss ss ss 03 47 31 30 43 4D 4D 00 01 11 00 ;3DS whatever chiptype?
 MY ss ss ss ss 07 43 59 31 47 34 4D 00 01 15 00 ;3DS Samsung KLM4G1YE0C-B301 |
| --- |

See also:

DSi Console IDs

| DSi SD/MMC Protocol: CSD Register (128bit Card-Specific Data) |
| --- |

**CMD9 - SD/MMC - SPI - GET_CSD (type=ac)**

Parameter bits:

```

| 31-16 RCA (SPI Mode: stuff bits)
 15-0 stuff bits |
| --- |

SD Mode Response: R2:

```

| 135 Start Bit (0) ;\
 134 Transmission To Host (0) ; 1st byte
 133-128 Reserved (111111) (instead of Command value) ;/
 127-8 CSD (120bit) (15 bytes) ;\aka 128bit ;-2nd..16th byte
 7-1 CRC7 ; when including ;\17th byte
 0 End Bit (1) ;/CRC7+EndBit ;/ |
| --- |

SPI Mode Response: R1, plus DATA line,

SPI Mode Additional Data Transfer (from card):

```

| 127-0 CID (128bit) ... or 120bit ? |
| --- |

Addressed card sends its card-specific data (CSD).

**CMD27 - SD/MMC - SPI - PROGRAM_CSD (type=adtc)**

```

| 31-0 stuff bits |
| --- |

Response: R1

Additional Data Transfer (to card):

```

| 128-0 CSD register (whole 128bit) (read-only bits must be unchanged) |
| --- |

Programming of the programmable bits of the CSD, ie. the "R/W" bits, the "R"
bits must be kept unchanged (read via CMD9, and write-back same values via
CMD27).

The writable once "R/W(1)" bits can be changed only from 0-to-1, or vice-versa,
they can be really written only ONCE, by the manufacturer?

Most of the "R/W(1)" bits are probably set by the manufacturer at time when
pre-formatting the card, so they aren't actually user-writeable.

**CSD Register**

The types of the entries in the table below are coded as follows: R=readable,
W(1)=writable once, W=multiple writable.

```

| Bit Siz Type Name Field Value
 127-126 2 R CSD structure version CSD_STRUCTURE 00b
 125-122 4 R MMC: System spec version SPEC_VERS ..
 125-122 4 R SD: reserved - 0000b
 121-120 2 R reserved - 00b
 119-112 8 R data read access-time-1 TAAC xxh
 111-104 8 R data read access-time-2 NSAC xxh
 103-96 8 R max data transfer rate TRAN_SPEED 32h or 5Ah
 95-84 12 R card command classes CCC 01x110110101b
 83-80 4 R max read data block len READ_BL_LEN xh
 79 1 R partial blocks for read allowed READ_BL_PARTIAL 1b
 78 1 R write block misalignment WRITE_BLK_MISALIGN xb
 77 1 R read block misalignment READ_BLK_MISALIGN xb
 76 1 R DSR implemented DSR_IMP xb
 75-74 2 R reserved - 00b
 73-70 4 R SDHC/SDXC: reserved - 0000b
 69-48 22 R SDHC/SDXC: device size C_SIZE ...
 47 1 R SDHC/SDXC: reserved - 0
 73-62 12 R MMC/SDSC: device size C_SIZE xxxh
 61-59 3 R MMC/SDSC: max read current @VDD min VDD_R_CURR_MIN xxxb
 58-56 3 R MMC/SDSC: max read current @VDD max VDD_R_CURR_MAX xxxb
 55-53 3 R MMC/SDSC: max write current @VDD min VDD_W_CURR_MIN xxxb
 52-50 3 R MMC/SDSC: max write current @VDD max VDD_W_CURR_MAX xxxb
 49-47 3 R MMC/SDSC: device size multiplier C_SIZE_MULT xxxb
 46-42 5 R MMC: Erase Group Size ERASE_GRP_SIZE ..
 41-37 5 R MMC: Erase Group Multiplier ERASE_GRP_MULT ..
 36-32 5 R MMC: Write Protect Grp Size WP_GRP_SIZE ..
 46 1 R SD: erase single block enable ERASE_BLK_EN xb
 45-39 7 R SD: erase sector size SECTOR_SIZE xxxxxxxb
 38-32 7 R SD: write protect group size WP_GRP_SIZE xxxxxxxb
 31 1 R write protect group enable WP_GRP_ENABLE xb
 30-29 2 R MMC: Manufacturer default ECC DEFAULT_ECC ..
 30-29 2 R SD: reserved (do not use) - 00b
 28-26 3 R write speed factor R2W_FACTOR xxxb
 25-22 4 R max write data block len WRITE_BL_LEN xxxxb
 21 1 R partial blocks for write allowed WRITE_BL_PARTIAL xb
 20-17 4 R reserved - 0000b
 16 1 R SD: reserved - 0
 16 1 R MMC: Content Protection Applicat. CONTENT_PROP_APP ..
 15 1 R/W(1) File format group FILE_FORMAT_GRP xb
 15 1 R SDHC/SDXC: reserved (FILE_FORMAT_GRP)0
 14 1 R/W(1) copy flag COPY xb
 13 1 R/W(1) permanent write protection PERM_WRITE_PROTECT xb
 12 1 R/W temporary write protection TMP_WRITE_PROTECT xb
 11-10 2 R/W(1) File format FILE_FORMAT xxb
 11-10 2 R SDHC/SDXC: reserved (FILE_FORMAT) 00b
 9-8 2 R/W MMC: ECC Code ECC ..
 9-8 2 R/W SDSC: reserved, R/W - 00b
 9-8 2 R SDHC/SDXC: reserved, R - 00b
 7-1 7 R/W CRC CRC xxxxxxxb
 0 1 - not used, always '1' - 1b |
| --- |

**Known CSD's for DSi eMMC chips (excluding CRC in LSB, padded 00 in MSB)**

```

| 8 16 24 32 40 48 56 64 72 80 88 96 104112120pad ;<--bit numbers
 40 40 96 E9 7F DB F6 DF 01 59 0F 2A 01 26 90 00 ;DSi Samsung KMAPF0000M-S998
 40 40 8E FF 03 DB F6 DF 01 59 0F 32 01 27 90 00 ;DSi Samsung KLM5617EFW-B301
 00 40 8A E0 BF FF 7F F5 80 59 0F 32 01 2F 90 00 ;DSi ST NAND02GAH0LZC5 rev30
 00 40 8A E0 BF FF 7F F5 80 59 0F 32 01 2F 90 00 ;DSi ST NAND02GAH0LZC5 rev31
 ? 00 ;3DS whatever chiptype?
 40 40 8A E7 FF DB F6 6B 02 5A 0F 32 01 27 D0 00 ;3DS Samsung KLM4G1YE0C-B301 |
| --- |

That is, differences are:

```

| bit name KMAPF0000M KLM5617EFW NAND02GAH0LZC5 KLM4G1YE0C
 126-127 CSD_STRUCTURE 2=v1.2 2=v1.2 2=v1.2 3=SeeEXT_CSD
 112-119 TAAC 26h=1.5ms 27h=15ms 2Fh=20ms 27h=15ms
 96-103 TRAN_SPEED 2Ah=20MHz 32h=25MHz 32h=25MHz 32h=25MHz
 80-83 READ_BL_LEN 9=512 9=512 9=512 0Ah=1024
 79 READ_BL_PARTIAL 0=No(?) 0=No(?) 1=Yes 0=No(?)
 62-73 C_SIZE 77Fh=240MB 77Fh=240MB 3D5h=245.5MB 9AFh=1240MB
 59-61 VDD_R_CURR_MIN 6=60mA 6=60mA 7=100mA 6=60mA
 56-58 VDD_R_CURR_MAX 6=80mA 6=80mA 7=200mA 6=80mA
 53-55 VDD_W_CURR_MIN 6=60mA 6=60mA 7=100mA 6=60mA
 50-52 VDD_W_CURR_MAX 6=80mA 6=80mA 7=200mA 6=80mA
 47-49 C_SIZE_MULT 6=256 6=256 7=512 7=512
 42-46 ERASE_GRP_SIZE 1Fh=32x32 00h=1x32 1Fh=32x32 1Fh=32x32
 32-36 WP_GRP_SIZE 09h=10 1Fh=32 00h=1 07h=8
 26-28 R2W_FACTOR 05h=32x 03h=8x 02h=4x 02h=4x
 14 COPY 1=Copy 1=Copy 0=Original 1=Copy |
| --- |

Not sure if that values are really correct, or if the manufacturer has screwed
up some bits. TAAC being 10x slower in newer chips looks weird, 20MHz would be
for 1bit MCC (whilst 4bit MMCplus/MMCmobile should support 26MHz), erase group
32x32x512 bytes would somewhat require 512Kbyte clusters, write protect group
size 10 decimal looks a bit odd (though it could be true), and, well, faster
writing in newer chips looks plausible.

**CSD_STRUCTURE (upper 2bit of CSD register)**

Field structures of the CSD register are different depend on the Physical Layer
Specification Version and Card Capacity.

The CSD_STRUCTURE field in the CSD register indicates its structure version.

For MMC:

```

| 00h CSD version No. 1.0 MMC Version 1.0 - 1.2
 01h CSD version No. 1.1 MMC Version 1.4 - 2.2
 02h CSD version No. 1.2 MMC Version 3.1 - 3.2 - 3.31 - 4.0 - 4.1- 4.2
 03h Version is coded in the CSD_STRUCTURE byte in the EXT_CSD register |
| --- |

For SD:

```

| 00h CSD Version 1.0 SDSC (Standard Capacity)
 01h CSD Version 2.0 SDHC/SDXC (High Capacity and Extended Capacity)
 02h-03h Reserved |
| --- |

SDHC/SDXC applies major changes to CSD register (C_SIZE is expanded, and many
other fields are removed or set to dummy values), for details see:

DSi SD/MMC Protocol: CSD Register (128bit Card-Specific Data) Version 2.0

**SPEC_VERS (MMC only)**

```

| 00h MMC System Specification Version 1.0 - 1.2
 01h MMC System Specification Version 1.4
 02h MMC System Specification Version 2.0 - 2.2
 03h MMC System Specification Version 3.1 - 3.2 - 3.31
 04h MMC System Specification Version 4.0 - 4.1 - 4.2
 05h-0Fh Reserved |
| --- |

**TAAC**

Defines the asynchronous part of the data access time.

```

| 7 Reserved
 6-3 Time value
 0=reserved, 1=1.0, 2=1.2, 3=1.3, 4=1.5, 5=2.0, 6=2.5, 7=3.0,
 8=3.5, 9=4.0, A=4.5, B=5.0, C=5.5, D=6.0, E=7.0, F=8.0
 2-0 Time unit
 0=1ns, 1=10ns, 2=100ns, 3=1us, 4=10us, 5=100us, 6=1ms, 7=10ms |
| --- |

**NSAC**

Defines the worst case for the clock-dependent factor of the data access time.
The unit for NSAC is 100 clock cycles. Therefore, the maximal value for the
clock-dependent part of the data access time is 25500 clock cycles.

The total access time NAC is the sum of TAAC and NSAC. It should be computed by
the host for the actual clock rate. The read access time should be interpreted
as a typical delay for the first data bit of a data block or stream.

**TRAN_SPEED**

The following table defines the maximum data transfer rate PER ONE data line:

```

| 7 Reserved
 6-3 Time value
 0=reserved, 1=1.0, 2=1.2, 3=1.3, 4=1.5, 5=2.0, 6=2.5, 7=3.0,
 8=3.5, 9=4.0, A=4.5, B=5.0, C=5.5, D=6.0, E=7.0, F=8.0
 2-0 Transfer rate unit
 0=100kbit/s, 1=1Mbit/s, 2=10Mbit/s, 3=100Mbit/s, 4..7=reserved
 MMC: same as above, but specified in <Hz> instead of <bits/s> |
| --- |

Note that for current SD Memory Cards, this field shall be always 32h which is
equal to 25 MHz - the mandatory maximum operating frequency of SD Memory Card.

In High-Speed mode, this field shall be always 5Ah which is equal to 50 MHz,
and when the timing mode returns to the default by CMD6 or CMD0 command, its
value will be 32h.

**CCC (Card Command Class)**

The SD Memory Card command set is divided into subsets (command classes). A
value of 1 in a CCC bit means that the corresponding command class is
supported. For command class definitions, refer to Table 4-21.

```

| 11 Supports Command Class 11 - Function Extension Commands (SD)
 10 Supports Command Class 10 - Switch Function Commands (SD)
 9 Supports Command Class 9 - I/O Mode Commands (SDIO/MMCIO)
 8 Supports Command Class 8 - Application-Specific Commands
 7 Supports Command Class 7 - Password Lock Commands
 6 Supports Command Class 6 - Block-Oriented Write Protection Commands
 5 Supports Command Class 5 - Erase Commands
 4 Supports Command Class 4 - Block-Oriented Write Commands
 3 Supports Command Class 3 - WRITE_DAT_UNTIL_STOP (MMC)
 2 Supports Command Class 2 - Block-Oriented Read Commands
 1 Supports Command Class 1 - READ_DAT_UNTIL_STOP (MMC)
 0 Supports Command Class 0 - Basic Commands |
| --- |

Same for MMC (though on MMC the classes may have different meaning?)

**READ_BL_LEN**

The maximum read data block length is computed as 2^READ_BL_LEN. The maximum
block length might therefore be in the range 512...2048 bytes (Refer to 4.3.3
for details). Note that in an SD Memory Card the WRITE_BL_LEN is always equal
to READ_BL_LEN.

```

| 3-0 Setting |
| --- |

Values:

```

| 00h..08h Reserved
 09h Block length 512 Bytes (2^9)
 0Ah Block length 1024 Bytes (2^10)
 0Bh Block length 2048 Bytes (2^11)
 0Ch..0Fh Reserved |
| --- |

MMC allows any values from 2^0 to 2^14, and uses 0Fh for Extension (see TBD
field in EXT_CSD) (uh, but "TBD" isn't yet defined in KMCEN0000M datasheet,
maybe TBD means to-be-defined?).

**READ_BL_PARTIAL (always = 1 in SDSC Memory Card)**

Partial Block Read is always allowed in an SDSC Memory Card. It means that
smaller blocks can be used as well. The minimum block size will be one byte.

**WRITE_BLK_MISALIGN**

Defines if the data block to be written by one command can be spread over more
than one physical block of the memory device. The size of the memory block is
defined in WRITE_BL_LEN.

```

| WRITE_BLK_MISALIGN=0 crossing physical block boundaries is invalid
 WRITE_BLK_MISALIGN=1 crossing physical block boundaries is allowed |
| --- |

**READ_BLK_MISALIGN**

Defines if the data block to be read by one command can be spread over more
than one physical block of the memory device. The size of the memory block is
defined in READ_BL_LEN.

```

| READ_BLK_MISALIGN=0 crossing physical block boundaries is invalid
 READ_BLK_MISALIGN=1 crossing physical block boundaries is allowed |
| --- |

**DSR_IMP**

Defines if the configurable driver stage is integrated on the card. If set, a
driver stage register (DSR) shall be implemented (also see Chapter 5.5).

```

| DSR_IMP=0 no DSR implemented
 DSR_IMP=1 DSR implemented |
| --- |

**C_SIZE (for max 2GB)**

This parameter is used to compute the user's data card capacity (not include
the security protected area). The memory capacity of the card is computed from
the entries C_SIZE, C_SIZE_MULT and READ_BL_LEN as follows:

```

| memory capacity = BLOCKNR * BLOCK_LEN |
| --- |

Whereas,

```

| BLOCKNR = (C_SIZE+1) * MULT
 MULT = 2^(C_SIZE_MULT+2) ;(C_SIZE_MULT < 8)
 BLOCK_LEN = 2^READ_BL_LEN ;(READ_BL_LEN < 12) |
| --- |

To indicate 2 GByte card, BLOCK_LEN shall be 1024 bytes.

Therefore, the maximal capacity that can be coded is 4096*512*1024 = 2 G bytes.

Example: A 32 Mbyte card with BLOCK_LEN = 512 can be coded by C_SIZE_MULT = 3
and C_SIZE = 2000.

The Maximum Data Area size of SDSC Standard Capacity Card is 4,153,344 sectors
(2028MB).

**C_SIZE (for MMC above 2GB)**

The 12bit C_SIZE field should be set 0FFFh. Use "SEC_COUNT" in EXT_CSD to
specify actual size.

**VDD_R_CURR_MIN, VDD_W_CURR_MIN**

Maximum values for read and write currents at the MINIMAL power supply VDD:

```

| 2-0 0=0.5mA, 1=1mA, 2=5mA, 3=10mA, 4=25mA, 5=35mA, 6=60mA, 7=100mA |
| --- |

**VDD_R_CURR_MAX, VDD_W_CURR_MAX**

Maximum values for read and write currents at the MAXIMAL power supply VDD:

```

| 2-0 0=1mA, 1=5mA, 2=10mA, 3=25mA, 4=35mA, 5=45mA, 6=80mA, 7=200mA |
| --- |

**C_SIZE_MULT**

This parameter is used for coding a factor MULT for computing the total device
size (see 'C_SIZE'). Defined as "MULT = 2^(C_SIZE_MULT+2)".

```

| 2-0 Device Size Factor (0..7 = Factor 4,8,16,32,64,128,256,512) |
| --- |

**SD: ERASE_BLK_EN**

The ERASE_BLK_EN defines the granularity of the unit size of the data to be
erased. The erase operation can erase either one or multiple units of 512 bytes
or one or multiple units (or sectors) of SECTOR_SIZE (see definition below).

If ERASE_BLK_EN=0, the host can erase one or multiple units of SECTOR_SIZE. The
erase will start from the beginning of the sector that contains the start
address to the end of the sector that contains the end address. For example, if
SECTOR_SIZE=31 and the host sets the Erase Start Address to 5 and the Erase End
Address to 40, the physical blocks from 0 to 63 will be erased as shown in
Figure 5-1.

```

| Figure 5-1: ERASE_BLK_EN = 0 Example
 Physical Block (per CSD)
 0 1 2 3 4 5 6
 0123456789 0123456789 0123456789 0123456789 0123456789 0123456789 0123456789
 <----- Host Erase Address Range ------->
 <---------- Erase Area ---------------------------------------------->
 <---------- Erase Unit Size ------><------- Erase Unit Size ---------> |
| --- |

If ERASE_BLK_EN=1 the host can erase one or multiple units of 512 bytes. All
blocks that contain data from start address to end address are erased. For
example, if the host sets the Erase Start Address to 5 and the Erase End
Address to 40, the physical blocks from 5 to 40 will be erased as shown in
Figure 5-2.

```

| Figure 5-2: ERASE_BLK_EN = 1 Example
 Physical Block (per CSD)
 0 1 2 3 4 5 6
 0123456789 0123456789 0123456789 0123456789 0123456789 0123456789 0123456789
 <----- Host Erase Address Range ------->
 <----- Erase Area ---------------------> |
| --- |

**SD: SECTOR_SIZE**

The size of an erasable sector. The content of this register is a 7-bit binary
coded value, defining the number of write blocks (see WRITE_BL_LEN). The actual
size is computed by increasing this number by one. A value of zero means one
write block, 127 means 128 write blocks.

**MMC: ERASE_GRP_SIZE**

The contents of this register is a 5 bit binary coded value, used to calculate
the size of the erasable unit of the moviNAND. The size of the erase unit (also
referred to as erase group) is determined by the ERASE_GRP_SIZE and the
ERASE_GRP_MULT entries of the CSD, using the following equation:

```

| size of erasable unit = (ERASE_GRP_SIZE + 1) * (ERASE_GRP_MULT + 1) |
| --- |

This size is given as minimum number of write blocks that can be erased in a
single erase command.

**MMC: ERASE_GRP_MULT**

A 5 bit binary coded value used for calculating the size of the erasable unit
of the moviNAND. See ERASE_GRP_SIZE section for detailed description.

**MMC: DEFAULT_ECC**

Set by the moviNAND manufacturer. It defines the ECC code which is recommended
for use. The field definition is the same as for the ECC field described later.

**MMC: CONTENT_PROT_APP**

This field in the CSD indicates whether the content protection application is
supported. MultiMediaCards which implement the content protection application
will have this bit set to '1'.

**MMC: ECC**

Defines the ECC code that was used for storing data on the moviNAND. This field
is used by the host (or application) to decode the user data. The following
table defines the field format:

```

| ECC ECC type Maximum number of correctable bits per block
 00h None (default) Mone
 01h BCH (542,512) 3
 02h-03h Reserved - |
| --- |

**MMC: WP_GRP_SIZE (5bit)**

The size of a write protected group. The contents of this register is a 5 bit
binary coded value, defining the number of erase groups which can be write
protected.

The actual size is computed by increasing this number by one. A value of zero
means 1 erase group, 31 means 32 erase groups. (Refer to the chapter 4.11.1 on
page 48)

**SD: WP_GRP_SIZE (7bit)**

The size of a write protected group. The content of this register is a 7-bit
binary coded value, defining the number of erase sectors (see SECTOR_SIZE). The
actual size is computed by increasing this number by one. A value of zero means
one erase sector, 127 means 128 erase sectors.

**WP_GRP_ENABLE**

A value of 0 means no group write protection possible.

**R2W_FACTOR**

Defines the typical block program time as a multiple of the read access time.

```

| 2-0 Multiples of read access time (0..5=Mul 1,2,4,8,16,32, 6..7=Reserved) |
| --- |

For example, value 5 means that writing is 32 times slower than reading.

**WRITE_BL_LEN**

The maximum write data block length is computed as 2^WRITE_BL_LEN. The maximum
block length might therefore be in the range from 512 to 2048 bytes. Write
Block Length of 512 bytes is always supported.

Note that in the SD Memory Card, the WRITE_BL_LEN is always equal to
READ_BL_LEN.

```

| 3-0 Block Length |
| --- |

Values:

```

| 00h..08h Reserved
 09h 512 bytes (2^9)
 0Ah 1024 Bytes (2^10)
 0Bh 2048 Bytes (2^11)
 0Ch..0Fh Reserved |
| --- |

MMC: See READ_BL_LEN

**WRITE_BL_PARTIAL**

Defines whether partial block sizes can be used in block write commands.

WRITE_BL_PARTIAL=0 means that only the WRITE_BL_LEN block size and its partial
derivatives, in resolution of units of 512 bytes, can be used for block
oriented data write.

WRITE_BL_PARTIAL=1 means that smaller blocks can be used as well. The minimum
block size is one byte.

**COPY**

Defines whether the contents is original (=0) or has been copied (=1). Setting
this bit to 1 indicates that the card content is a copy. The COPY bit is a one
time programmable bit except ROM card.

**PERM_WRITE_PROTECT**

**TMP_WRITE_PROTECT**

Permanently/temporarily write-protects the entire card (by disabling all write
and erase commands). The default values are 0, ie. not write protected.

**FILE_FORMAT**

**FILE_FORMAT_GRP**

Indicates the file format on the card. These fields are read-only for ROM. The
following formats are defined:

```

| FILE_FORMAT_GRP FILE_FORMAT Type
 0 0 Hard disk-like file system with partition table
 0 1 DOS FAT (floppy-like) with boot sector only
 (no partition table)
 0 2 Universal File Format
 0 3 Others/Unknown
 1 0, 1, 2, 3 Reserved |
| --- |

A more detailed description is given in the Filesystem Specification.

| DSi SD/MMC Protocol: CSD Register (128bit Card-Specific Data) Version 2.0 |
| --- |

**CSD Register (CSD Version 2.0) (SDHC/SDXC)**

The field name in parenthesis is set to fixed value and indicates that the host
is not necessary to refer these fields. The fixed values enables host, which
refers to these fields, to keep compatibility to CSD Version 1.0.

The Cell Type field is coded as follows: R=readable, W(1)=writable once,
W=multiple writable.

```

| Bit Siz Type Name Field Value
 127-126 2 R CSD structure CSD_STRUCTURE 01b
 125-120 6 R reserved - 000000b
 119-112 8 R data read access-time-1 (TAAC) 0Eh
 111-104 8 R data read access-time-2 (NSAC) 00h
 103-96 8 R max data transfer rate (TRAN_SPEED) 32h,5Ah,0Bh,2Bh
 95-84 12 R card command classes CCC x1x110110101b
 83-80 4 R max read data block length (READ_BL_LEN) 9
 79 1 R partial blocks for read allowed (READ_BL_PARTIAL) 0
 78 1 R write block misalignment (WRITE_BLK_MISALIGN) 0
 77 1 R read block misalignment (READ_BLK_MISALIGN) 0
 76 1 R DSR implemented DSR_IMP x
 75-70 6 R reserved - 000000b
 69-48 22 R device size C_SIZE xxxxxxh
 47 1 R reserved - 0
 46 1 R erase single block enable (ERASE_BLK_EN) 1
 45-39 7 R erase sector size (SECTOR_SIZE) 7Fh
 38-32 7 R write protect group size (WP_GRP_SIZE) 00h
 31 1 R write protect group enable (WP_GRP_ENABLE) 0
 30-29 2 R reserved - 00b
 28-26 3 R write speed factor (R2W_FACTOR) 010b
 25-22 4 R max write data block length (WRITE_BL_LEN) 9
 21 1 R partial blocks for write allowed (WRITE_BL_PARTIAL) 0
 20-16 5 R reserved - 00000b
 15 1 R File format group (FILE_FORMAT_GRP) 0
 14 1 R/W(1) copy flag COPY x
 13 1 R/W(1) permanent write protection PERM_WRITE_PROTECT x
 12 1 R/W temporary write protection TMP_WRITE_PROTECT x
 11-10 2 R File format (FILE_FORMAT) 00b
 9-8 2 R reserved - 00b
 7-1 7 R/W CRC CRC xxh
 0 1 - not used, always '1' - 1 |
| --- |

**C_SIZE**

This field is expanded to 22 bits and can indicate up to 2 TBytes (that is the
same as the maximum memory space specified by a 32-bit block address.)

This parameter is used to calculate the user data area capacity in the SD
memory card (not include the protected area). The user data area capacity is
calculated from C_SIZE as follows:

```

| memory capacity = (C_SIZE+1) * 512KByte |
| --- |

The Minimum user area size of SDHC Card is 4,211,712 sectors (2GB + 8.5MB).

The Minimum value of C_SIZE for SDHC in CSD Version 2.0 is 001010h (4112).

The Maximum user area size of SDHC Card is (32GB - 80MB).

The Maximum value of C_SIZE for SDHC in CSD Version 2.0 is 00FF5Fh (65375).

The Minimum user area size of SDXC Card is 67,108,864 sectors (32GB).

The Minimum value of C_SIZE for SDXC in CSD Version 2.0 is 00FFFFh (65535).

**TRAN_SPEED**

TRAN_SPEED is variable depends on bus speed mode of SD Interface.

When CMD0 is received, this field is reset to 32h.

On SDSC (but not SDHC/SDXC), CMD6 does the same reset stuff?

```

| 32h SDSC/SDHC/SDXC in Default Speed mode (25MHz)
 5Ah SDSC/SDHC/SDXC in High Speed mode (50MHz)
 0Bh SDHC/SDXC in SDR50 or DDR50 mode (100Mbit/sec)
 2Bh SDHC/SDXC in SDR104 mode (200Mbit/sec) |
| --- |

UHS-II mode is not related to this field.

**CCC, DSR_IMP, COPY, PERM_WRITE_PROTECT, TMP_WRITE_PROTECT**

Definition of these fields is same as in CSD Version 1.0.

**TAAC, NSAC, R2W_FACTOR**

In SDHC/SDXC, these fields should be fixed to TAAC=0Eh (1 ms), NSAC=00h, and
R2W_FACTOR=02h (mul4).

The host should not use TAAC, NSAC, and R2W_FACTOR to calculate timeout and
should uses fixed timeout values for read and write operations (See 4.6.2).

**READ_BL_LEN, WRITE_BL_LEN**

These two fields are fixed to 9h (which indicates 512 Bytes).

**READ_BL_PARTIAL, WRITE_BL_PARTIAL, READ_BLK_MISALIGN, WRITE_BLK_MISALIGN**

These four fields are fixed to 0 (partial block read and physical page crossing
prohibited for block read/write).

**SECTOR_SIZE**

This field is fixed to 7Fh, which indicates 64 KBytes. This value is not
related to erase operation. SDHC and SDXC Cards indicate memory boundary by AU
size and this field should not be used.

**ERASE_BLK_EN**

This field is fixed to 1, which means the host can erase one or multiple units
of 512 bytes.

**WP_GRP_SIZE, WP_GRP_ENABLE**

These field are fixed to WP_GRP_SIZE=00h, and WP_GRP_ENABLE=0.

SDHC and SDXC Cards do not support write protected groups.

**FILE_FORMAT_GRP**

**FILE_FORMAT**

These fields are set to 0. Host should not use these fields.

| DSi SD/MMC Protocol: EXT_CSD Register (4096bit Extended CSD Register) (MMC) |
| --- |

**CMD8 - MMC - SPI - GET_EXT_CSD (type=adtc)**

```

| 31-0 stuff bits |
| --- |

Response: R1

Additional Data Transfer (from card):

```

| 4095-0 EXT_CSD Register (4096bit) |
| --- |

MMC only.

**CMD6 - MMC - SPI - SWITCH (type=ac)**

```

| 31-26 6bit Reserved (0)
 25-24 2bit Access
 00h Change Command Set (EXT_CSD[191] = parameter bit2-0)
 01h Set bits (EXT_CSD[index] = EXT_CSD[index] OR value)
 02h Clr bits (EXT_CSD[index] = EXT_CSD[index] AND NOT value)
 03h Write (EXT_CSD[index] = value)
 23-16 8bit Index (0..191) ;\used only if "Access=1..3"
 15-8 8bit Value (0..255) ;/
 7-3 5bit Reserved (0)
 2-0 3bit Cmd Set (0..7) ;-used only if "Access=0" |
| --- |

Response: R1b

Additional Data Transfer (from card): Busy signal for "R1b" response

MMC only.

**Extended CSD Register (MMC only)**

The Extended CSD register defines the card properties and selected modes. It is
512 bytes (4096 bits) long.

The most significant 320 bytes are the Properties segment, which defines the
card capabilities and cannot be modified by the host. The lower 192 bytes are
the Modes segment, which defines the configuration the card is working in.
These modes can be changed by the host by means of the SWITCH command.

```
********
| Properties Segment
 Byte Siz Type Name Field
 511-505 7 - Reserved(1) -
 504 1 R Supported Command Sets S_CMD_SET
 503-216 288 - Reserved(1) -
 215-212 4 R moviNAND only: Sector Count SEC_COUNT
 211 1 - Reserved -
 210 1 R Min Write Performance for 8bit @52MHz MIN_PERF_W_8_52
 209 1 R Min Read Performance for 8bit @52MHz MIN_PERF_R_8_52
 208 1 R Min Write Perf. for 8/4bit @26/52MHz MIN_PERF_W_8_26_4_52
 207 1 R Min Read Perf. for 8/4bit @26/52MHz MIN_PERF_R_8_26_4_52
 206 1 R Min Write Performance for 4bit @26MHz MIN_PERF_W_4_26
 205 1 R Min Read Performance for 4bit @26MHz MIN_PERF_R_4_26
 204 1 - Reserved(1) -
 203 1 R Power Class for 26MHz @ 3.6V PWR_CL_26_360
 202 1 R Power Class for 52MHz @ 3.6V PWR_CL_52_360
 201 1 R Power Class for 26MHz @ 1.95V PWR_CL_26_195
 200 1 R Power Class for 52MHz @ 1.95V PWR_CL_52_195
 199-197 3 - Reserved(1) -
 196 1 R Card Type CARD_TYPE
 195 1 - Reserved(1) -
 194 1 R CSD Structure Version CSD_STRUCTURE
 193 1 - Reserved(1) -
 192 C0h 1 R Extended CSD Revision EXT_CSD_REV
 Modes Segment
 191 BFh 1 R/W Command Set CMD_SET
 190 BEh 1 - Reserved(1) -
 189 BDh 1 RO Command Set Revision CMD_SET_REV
 188 BCh 1 - Reserved(1) -
 187 BBh 1 R/W Power Class POWER_CLASS
 186 BAh 1 - Reserved(1) -
 185 B9h 1 R/W High Speed Interface Timing HS_TIMING
 184 B8h 1 - Reserved(1) -
 183 B7h 1 WO Bus Width Mode BUS_WIDTH
 182 B6h ? ?
 181 B5h 1 - Reserved -
 180 B4h 1 RO moviNAND only: Erased Memory Content ERASED_MEM_CONT
 180-0 181 - Reserved(a) - |
| --- |

(a) Reserved(a) bits should read as '0'.

(1) Reserved(1) bits should be probably ZERO, too.

The above table is transferred "most significant first", which does probably
mean that it starts with BYTE 0, not with byte 511. ALTHOUGH, the 4-byte
SEC_COUNT appears to be LITTLE-ENDIAN?

Note: JEDEC Standard No. 84-A44 contains MANY additional fields in EXT_CSD.

**S_CMD_SET**

This field defines which command sets are supported by the card.

```

| Bit Command Set
 7-5 Reserved
 4 moviNAND only: ATA on MMC
 3 moviNAND only: SecureMCC 2.0
 2 Content Protection SecureMMC
 1 SecureMMC
 0 Standard MMC |
| --- |

**SEC_COUNT (moviNAND and newer JEDEC specs only)**

The device density is calculated from the register by multiplying the value of
the register (sector count) by 512B/sector. The maximum density possible to be
indicated is thus 2 Tera bytes (minus 512 bytes) (4,294,967,295 x 512B). The
least significant byte (LSB) of the sector count value is the byte [212].

**MIN_PERF_a_b_ff**

These fields defines the overall minimum performance value for the read and
write access with different bus width and max clock frequency modes. The value
in the register is coded as follows. Other than defined values are illegal.

```

| Value Performance
 0x00 For Cards not reaching the 2.4MB/s minimum value
 0x08 Class A: 2.4MB/s and is the lowest allowed value for MMCplus and
 MMCmobile(16x150kB/s)
 0x0A Class B: 3.0MB/s and is the next allowed value (20x150kB/s)
 0x0F Class C: 4.5MB/s and is the next allowed value (30x150kB/s)
 0x14 Class D: 6.0MB/s and is the next allowed value (40x150kB/s)
 0x1E Class E: 9.0MB/s and is the next allowed value (60x150kB/s)
 This is also the highest class which any MMCplus or MMCmobile card
 is needed to support in low bus category operation mode (26MHz with
 4bit data bus).
 A MMCplus or MMCmobile card supporting any higher class than this
 have to support this class also (in low category bus operation mode).
 0x28 Class F: Equals 12.0MB/s and is the next allowed value (80x150kB/s)
 0x32 Class G: Equals 15.0MB/s and is the next allowed value (100x150kB/s)
 0x3C Class H: Equals 18.0MB/s and is the next allowed value (120x150kB/s)
 0x46 Class J: Equals 21.0MB/s and is the next allowed value (140x150kB/s)
 This is also the highest class which any MMCplus or MMCmobile card
 is needed to support in mid bus category operation mode (26MHz with
 8bit data bus or 52MHz with 4bit data bus).
 A MMCplus or MMCmobile card supporting any higher class than this
 have to support this Class (in mid category bus operation mode) and
 Class E also (in low category bus operation mode).
 0x50 Class K: Equals 24.0MB/s and is the next allowed value (160x150kB/s)
 0x64 Class M: Equals 30.0MB/s and is the next allowed value (200x150kB/s)
 0x78 Class O: Equals 36.0MB/s and is the next allowed value (240x150kB/s)
 0x8C Class R: Equals 42.0MB/s and is the next allowed value (280x150kB/s)
 0xA0 Class T: Equals 48.0MB/s and is the last defined value (320x150kB/s) |
| --- |

**PWR_CL_ff_vvv**

These fields define the supported power classes by the card. By default, the
card has to operate at maximum frequency using 1 bit bus configuration, within
the default max current consumption, as stated in the table below. If 4 bit/8
bits bus configurations, require increased current consumption, it has to be
stated in these registers.

By reading these registers the host can determine the power consumption of the
card in different bus modes. Bits [7:4] code the current consumption for the 8
bit bus configuration. Bits [3:0] code the current consumption for the 4 bit
bus configuration.

The PWR_52_vvv registers are not defined for 26MHz MultiMediaCards.

```

| Voltage Value Max RMS Current Max Peak Current Remarks
 3.6V 0 100 mA 200 mA Default current
 1 120 mA 220 mA consumption for
 2 150 mA 250 mA high voltage cards
 3 180 mA 280 mA
 4 200 mA 300 mA
 5 220 mA 320 mA
 6 250 mA 350 mA
 7 300 mA 400 mA
 8 350 mA 450 mA
 9 400 mA 500 mA
 10 450 mA 550 mA
 11-15 Reserved for future use
 1.95V 0 65 mA 130 mA Default current
 1 70 mA 140 mA consumption for
 2 80 mA 160 mA Dual voltage cards
 3 90 mA 180 mA (if any, not moviNAND)
 4 100 mA 200 mA
 5 120 mA 220 mA
 6 140 mA 240 mA
 7 160 mA 260 mA
 8 180 mA 280 mA
 9 200 mA 300 mA
 10 250 mA 350 mA
 6-15 Reserved for future use |
| --- |

The measurement for max RMS current is done as average RMS current consumption
over a period of 100ms.

Max peak current is defined as absolute max value not to be exceeded at all.

The conditions under which the power classes are defined are:

```

| - Maximum bus frequency
 - Maximum operating voltage
 - Worst case functional operation
 - Worst case environmental parameters (temperature,...) |
| --- |

These registers define the maximum power consumption for any protocol operation
in data transfer mode, Ready state and Identification state.

**CARD_TYPE**

This field defines the type of the card. The only currently valid values for
this field are 0x01 and 0x03.

```

| Bit Card Type
 7-2 Reserved
 1 High Speed MultiMediaCard @ 52MHz
 0 High Speed MultiMediaCard @ 26MHz |
| --- |

**CSD_STRUCTURE**

This field is a continuation of the CSD_STRUCTURE field in the CSD register.

```

| CSD_STRUCTURE CSD structure version Valid for System Specification Version
 0 CSD version No. 1.0 Version 1.0 - 1.2
 1 CSD version No. 1.1 Version 1.4 - 2.2
 2 CSD version No. 1.2 Version 3.1-3.2-3.31-4.0-4.1-4.2
 3 Reserved for future use
 4-255 Reserved for future use |
| --- |

**EXT_CSD_REV**

Defines the fixed parameters. related to the EXT_CSD, according to its
revision.

```

| EXT_CSD_REV Extended CSD Revision
 0 Revision 1.0
 1 Revision 1.1
 2 Revision 1.2 (moviNAND)
 3-255 Reserved |
| --- |

**CMD_SET**

Contains the binary code of the command set that is currently active in the
card. It is set to '0' (Standard MMC) after power up and can be changed by a
SWITCH command.

**CMD_SET_REV**

Contains a binary number reflecting the revision of the currently active
command set. For Standard MMC command set it is:

```

| Code MMC Revisions
 0 v4.0
 1-255 Reserved |
| --- |

This field, though in the Modes segment of the EXT_CSD, is read only.

**POWER_CLASS**

This field contains the 4 bit value of the selected power class for the card.
The power classes are defined in Table. The host should be responsible of
properly writing this field with the maximum power class it allows the card to
use. The card uses this information to, internally, manage the power budget and
deliver an optimized performance.

This field is 0 after power-on or software reset.

```

| Bits Description
 7-4 Reserved
 3-0 Card power class code (See Table 5-29) |
| --- |

**HS_TIMING**

This field is 0 after power-on, or software reset, thus selecting the backwards
compatibility interface timing for the card. If the host writes 1 to this
field, the card changes its timing to high speed interface timing (refer to
Chapter 5.4.8).

**BUS_WIDTH**

It is set to '0' (1 bit data bus) after power up and can be changed by a SWITCH
command.

```

| Value Bus Mode
 0 1 bit data bus (MMC, with old 7pin connector)
 1 4 bit data bus (MMCplus, with SD-card-compatible 9pin connector)
 2 8 bit data bus (MMCplus, with special 13pin connector)
 3-255 Reserved |
| --- |

For detecting cards with 4bit/8bit data bus support: Switch the SD/MMC
controller to 4bit/8bit modes, and use BUSTEST_W and BUSTEST_R to test if the
card sends a proper response, see [https://www.mikrocontroller.net/attachment/101561/AN_MMCA050419.pdf](https://www.mikrocontroller.net/attachment/101561/AN_MMCA050419.pdf)

Note: The SD/MMC controller in the DSi supports 1bit/4bit modes only (no 8bit
mode). For the DSi's onboard eMMC it's safe to assume 4bit being supported,
however, external MMC cards do require detecting 4bit support.

**ERASED_MEM_CONT (moviNAND) (but, RESERVED in newer JEDEC specs!)**

This Field defines the content of an explicitly erased memory range.

```

| Value Erased Memory content
 00h Erased memory range shall be '0'
 01h Erased memory range shall be '1'
 02h-FFh Reserved |
| --- |

Looks like a mis-definition, since value 00h should have been kept reserved for
cards that do not specify whether they set erased bits to 0 or 1.

| DSi SD/MMC Protocol: RCA Register (16bit Relative Card Address) |
| --- |

The RCA was intended for connecting multiple cards to the same host, possibly
even sharing the same signal wires for multiple cards. The multi-card feature
isn't used to often though.

Most hosts are having only a single card slot. And, hosts that <do>
support multiple cards may use separate busses and even separate controllers
for each card (eg. Nintendo DSi is doing so for onboard NAND and external SD
slot).

However, even single-card systems will need to obtain a "dummy" RCA, and use
that RCA value for selecting the card.

The only exception is SPI mode: SPI isn't using RCA, and doesn't support RCA
commands at all - instead, in SPI mode, the cards are selected via /CS signal
(which may include multiple /CS signals for multiple cards).

**CMD3 - SD - GET_RELATIVE_ADDR (type=bcr)**

Parameter bits:

```

| 31-0 stuff bits |
| --- |

Response: R6:

```

| 47 Start Bit (0) ;\
 46 Transmission To Host (0) ; 1st byte
 45-40 Command (the 6bit CMD being responded to) ;/
 39-24 New published RCA of the card ;-16bit ;-2nd..3th byte
 23-22 CSR Card Status, bit 23-22 ;\ ;\
 21 CSR Card Status, bit 19 ; 16bit ; 4nd..5th byte
 20-8 CSR Card Status, bit 12-0 ;/ ;/
 7-1 CRC7 ;\6th byte
 0 End Bit (1) ;/ |
| --- |

Ask the card to publish a new relative address (RCA).

Dunno how this is intended to work with multiple cards. The goal should be to
assign <different> RCAs to each card. The command should be probably
repeatedly used until all cards respond with different RCAs. This would require
the cards to contain some sort of analog random generator - or maybe to use the
CID register as random seed (the CID seems to contain unique serial numbers per
card)?

**CMD3 - MMC - SET_RELATIVE_ADDR (type=ac)**

Parameter bits:

```

| 31-16 RCA
 15-0 stuff bits |
| --- |

Response: R1

Assigns an RCA value TO the card (ie. the opposite of CMD3 on SD Cards).

Dunno how this is intended to work with multiple cards. The goal should be to
assign <different> RCAs to each card. But actually, the command appears
to assign the <same> RCA to all cards?

**CMD7 - SD/MMC - SELECT_DESELECT_CARD (type=ac) ;actually: (type=bcr)**

Parameter bits:

```

| 31-16 RCA
 15-0 stuff bits |
| --- |

Response: R1b (only from the selected card)

Additional Data Transfer (from card): Busy signal for "R1b" response

Command toggles a card between the stand-by and transfer states or between the
programming and disconnect states. In both cases, the card is selected by its
own relative address and gets deselected by any other address; address 0
deselects all.

In the case that the RCA equals 0, then the host may do one of the following:

- Use other RCA number to perform card de-selection.

- Re-send CMD3 to change its RCA number to other than 0 and then use CMD7 with
RCA=0 for card deselection.

**CMD5 - MMC - SLEEP_AWAKE (type=ac)**

Parameter bits:

```

| 31-16 RCA
 15 Sleep/Awake flag (0=Awake/stby, 1=Sleep/slp)
 14-0 stuff bits |
| --- |

Response: R1b

**RCA register**

The writable 16-bit relative card address register carries the card address
that is published by the card during the card identification. This address is
used for the addressed host-card communication after the card identification
procedure. The default value of the RCA register is 0000h. The value 0000h is
reserved to set all cards into the Stand-by State with CMD7.

In UHS-II mode, Node ID is used as RCA. Refer to SD-TRAN Section of UHS-II
Addendum for more details.

**Note**

Commands GET_CSD, GET_CID, APP_CMD, GO_INACTIVE_STATE, and GET_STATUS
allow/require to specify RCA in parameter field.

Other commands are either processed by all cards (broadcast commands), or
processed only be cards that have been previously selected via CMD7 (most
normal commands).

**Broadcast Commands**

```

| CMD0 sd/mmc spi GO_IDLE_STATE (type=bc)
 CMD2 sd/mmc ALL_GET_CID (type=bcr)
 CMD3 sd GET_RELATIVE_ADDR (type=bcr)
 CMD4 sd/mmc SET_DSR (type=bc)
 CMD7 sd/mmc SELECT_DESELECT_CARD (type=ac) ;actually: (type=bcr)
 CMD8 sd spi SET_IF_COND (type=bcr)
 ACMD41 sd spi SD_SEND_OP_COND (type=bcr) ;SPI: reduced functionality |
| --- |

Some broadcast commands are sending responses.

SD specs are suggesting to use separate CMD lines for each card (so the host
would broadcast the same command on all CMD lines, and would receive separate
responses in parallel from each CMD line).

MMC cards are said to support open-collector CMD lines (so responses from
separate cards would be logically ORed, though, dunno what that would be good
for).

| DSi SD/MMC Protocol: DSR Register (16bit Driver Stage Register) (Optional) |
| --- |

**CMD4 - SD/MMC - SET_DSR (type=bc)**

Parameter bits:

```

| 31-16 DSR
 15-0 stuff bits |
| --- |

Response: N/A

Programs the DSR of all cards.

**DSR register (Optional)**

The 16-bit driver stage register is described in detail in Chapter 0 (uh,
where?). It can be optionally used to improve the bus performance for extended
operating conditions (depending on parameters like bus length, transfer rate or
number of cards). The CSD register carries the information about the DSR
register usage. The default value of the DSR register is 0404h.

| DSi SD/MMC Protocol: SCR Register (64bit SD Card Configuration Register) |
| --- |

**ACMD51 - SD - SPI - GET_SCR (type=adtc)**

```

| 31-0 stuff bits |
| --- |

Response: R1

Additional Data Transfer (from card):

```

| 63-0 SCR Register (8bytes, aka 64bit) |
| --- |

**SD Configuration Register (SCR)**

In addition to the CSD register, there is another configuration register named
SD CARD Configuration Register (SCR). SCR provides information on the SD Memory
Card's special features that were configured into the given card. The size of
SCR register is 64 bits. This register shall be set in the factory by the SD
Memory Card manufacturer.

```

| Bit Siz Typ Description Field ;common
 63-60 4 R SCR Structure SCR_STRUCTURE ;\00h or
 59-56 4 R SD Memory Card - Spec. Version SD_SPEC ;/01h
 55 1 R data_status_after erases DATA_STAT_AFTER_ERASE ;\
 54-52 3 R CPRM Security Support SD_SECURITY ; A5h
 51-48 4 R DAT Bus widths supported SD_BUS_WIDTHS ;/
 47 1 R Spec. Version 3.00 or higher SD_SPEC3 ;\
 46-43 4 R Extended Security Support EX_SECURITY ; 0000h
 42 1 R Spec. Version 4.00 or higher SD_SPEC4 ;
 41-36 6 R Reserved - ;
 35-32 4 R Command Support bits CMD_SUPPORT ;/
 31-0 32 R reserved for manufacturer usage - ;-var |
| --- |

**SCR Register Structure Version**

```

| SCR_STRUCTURE SCR Structure Version SD Physical Layer Specification Version
 00h SCR version 1.0 Version 1.01-4.00
 01h..0Fh reserved |
| --- |

Note: SD_SPEC is used to indicate SCR Structure Version instead of this field.

**SD_SPEC, SD_SPEC3, SD_SPEC4**

The Physical Layer Specification Version is indicated in combination with
SD_SPEC, SD_SPEC3 and SD_SPEC4 as described Table 5-19.

```

| SD_SPEC SD_SPEC3 SD_SPEC4 Physical Layer Specification Version Number
 0 0 0 Version 1.0 and 1.01
 1 0 0 Version 1.10
 2 0 0 Version 2.00
 2 1 0 Version 3.0X
 2 1 1 Version 4.XX
 Others Reserved |
| --- |

(1) Version 2.00 hosts do not recognize SD_SPEC3 and SD_SPEC4.

(2) Version 3.00 hosts do not recognize SD_SPEC4.

Hosts recognize Physical Layer Specification Version shall also recognize
including future version. Next version will be defined in SD_SPEC field.

The card manufacturer determines SD_SPEC value by conditions indicated below.
All conditions shall be satisfied for each version. The other combination of
conditions is not allowed.

Essential conditions to indicate Version 1.01 Card (SD_SPEC=0, SD_SPEC3=0 and
SD_SPEC4=0):

```

| (1) The card does not support CMD6
 (2) The card does not support CMD8
 (3) User area capacity shall be up to 2GB |
| --- |

Essential conditions to indicate Version 1.10 Card (SD_SPEC=1, SD_SPEC3=0 and
SD_SPEC4=0):

```

| (1) The card shall support CMD6
 (2) The card does not support CMD8
 (3) User area capacity shall be up to 2GB |
| --- |

Essential conditions to indicate Version 2.00 Card (SD_SPEC=2, SD_SPEC3=0 and
SD_SPEC4=0):

```

| (1) The card shall support CMD6
 (2) The card shall support CMD8
 (3) The card shall support CMD42
 (4) User area capacity shall be up to 2GB (SDSC) or 32GB (SDHC)
 (5) Speed Class shall be supported (SDHC) |
| --- |

Essential conditions to indicate Version 3.00 Card (SD_SPEC=2, SD_SPEC3=1 and
SD_SPEC4=0):

```

| (1) The card shall support CMD6
 (2) The card shall support CMD8
 (3) The card shall support CMD42
 (4) User area capacity shall be up to 2GB (SDSC) or 32GB (SDHC)
 User area capacity shall be more than or equal to 32GB and up to 2TB (SDXC)
 (5) Speed Class shall be supported (SDHC or SDXC) |
| --- |

Optional conditions to indicate Version 3.00 Card:

```

| A card supports any of following functions shall satisfy essential
 conditions of Version 3.00 Card
 (1) Speed Class supported under the conditions defined in Version 3.00
 (2) UHS-I supported card
 (3) CMD23 supported card |
| --- |

Essential conditions to indicate Version 4.XX Card (SD_SPEC=2, SD_SPEC3=1 and
SD_SPEC4=1):

```

| (1) Same as the essential conditions of Version 3.00 device
 (2) Support any of additional functions defined by Version 4.XX:
 Followings functions (a) to (c) are defined by Version 4.00.
 (a) Support of CMD48 and CMD49
 (b) Support of UHS-II mode
 (c) Support of DPS (Data Protection System)
 Followings functions (d) to (f) are defined by Version 4.10.
 (d) Support of CMD58 and CMD59
 (e) Support of Power Management Functions
 (f) Support of Speed Grade 1 for UHS-II mode |
| --- |

The requirements of supporting commands mentioned above are for the optional
commands, the support of which depends on versions (SD_SPEC, SD_SPEC3 and
SD_SPEC4). Refer to Table 4-21 (and Notes below the table) about the mandatory
and optional commands in the card.

**DATA_STAT_AFTER_ERASE**

Defines the data status after erase, whether it is 0 or 1 (the status is card
vendor dependent).

**SD_SECURITY**

This field indicates CPRM Security Specification Version for each capacity
card. The definition of Protected Area is different in each capacity card.

```

| 00h No Security
 01h Not Used
 02h SDSC Card (CPRM Security Version 1.01)
 03h SDHC Card (CPRM Security Version 2.00)
 04h SDXC Card (CPRM Security Version 3.xx)
 05h-07h Reserved |
| --- |

The basic rule of setting this field:

```

| SDSC Card sets this field to 2 (Version 1.01).
 SDHC Card sets this field to 3 (Version 2.00).
 SDXC Card sets this field to 4 (Version 3.xx). |
| --- |

Note that it is mandatory for a regular writable SD Memory Card to support
Security Protocol. For ROM (Read Only) and OTP (One Time Programmable) types of
the SD Memory Card, the security feature is optional.

**SD_BUS_WIDTHS**

Describes all the DAT bus widths that are supported by this card.

```

| Bit 3 Reserved
 Bit 2 4 bit (DAT0-3)
 Bit 1 Reserved
 Bit 0 1 bit (DAT0) |
| --- |

Since the SD Memory Card shall support at least the two bus modes 1-bit or
4-bit width, then any SD Card shall set at least bits 0 and 2
(SD_BUS_WIDTH="0101").

**EX_SECURITY**

This field indicates Extended Security which is defined by the Part A4 Data
Protection System Specification Version 1.00 or will be defined by a later
version of the Part 3 Security Specification Version 3.00.

```

| 00h Extended Security is not supported.
 01h..0Fh Extended Security is supported. SCR[44-43] is defined by the
 Part A4 Data Protection System Specification. SCR[46-45] is
 reserved for future extension. |
| --- |

**CMD_SUPPORT**

Support bit of new commands are defined to Bit 33-32 (uh, 35-32?) of SCR.

```

| Bit Supported Command Command CCC Remark
 35 Extension Register Multi-Block CMD58/59 11 Optional.
 34 Extension Register Single Block CMD48/49 11 Optional.
 33 Set Block Count CMD23 2,4 Mandatory for UHS104 card
 32 Speed Class Control CMD20 2,4 Mandatory for SDXC card |
| --- |

If CMD58/59 is supported, then CMD48/49 shall be also supported.

| DSi SD/MMC Protocol: PWD Register (128bit Password plus 8bit Password len) |
| --- |

**CMD40 - SD - Defined by DPS Spec (Data Protection System) (type=adtc)**

```

| Defined by DPS Spec. |
| --- |

Response: R1

Additional Data Transfer (to/from whatever):

```

| unknown |
| --- |

Single block read type. Intended to read whatever "public" data, which is
available even if the card is locked.

**CMD42 - SD/MMC (SD v2.00 and up) - SPI - LOCK_UNLOCK (type=adtc)**

```

| 31-0 Reserved bits (0) |
| --- |

Response: R1 (MMC: R1b?)

MMC?: Additional Data Transfer (from card): Busy signal for "R1b" response

Additional Data Transfer (to card):

```

| Note: Before using this command, the size of the following data block (ie.
 "1st..Nth/Extra" byte) must be set via SET_BLOCKLEN command (CMD16).
 1st byte: Flags
 Bit7-4 Reserved (0)
 Bit3 ERASE Force Erase (1=Erase WHOLE CARD and clear password)
 Bit1 LOCK_UNLOCK Lock card (0=Unlock, 1=Lock) (default on power up: Lock)
 Bit1 CLR_PWD Clears password (0=no, 1=yes)
 Bit0 SET_PWD Set new password (0=no, 1=yes)
 2nd byte: PWDS_LEN Length of the Password(s) in bytes ("3rd..Nth" byte)
 3rd..Nth byte: Password (old password, if SET_PWD: followed by new password)
 Extra byte: Alignment padding (only in DDR50 mode, if above is odd num bytes) |
| --- |

Used to set/clear the password (=to change the password), or to lock/unlock the
card (=to log out/log in). If the password exists, then the default state on
power-up is Locked (user is logged out).

In locked state, the card will accept only "Basic Commands" (class 0), plus
CMD16,40,42, plus ACMD41,42. Observe that this will prevent some initialization
commands (for example, switching to 4bit bus via SET_BUS_WIDTH (ACMD6) isn't
possible on locked cards).

The password is stored in a 128bit PWD register, so password can be max 16
bytes long. The PWDS_LEN value can be max 32 bytes (when sending old+new
password). The length of the current/old password is stored in an 8bit PWD_LEN
register (though due to the above limit, that "8bit" value can be in range
0..16 only; PWD_LEN=0 means that there is no password, which is somewhat
equivalent to what happens when setting the CLR_PWD bit?).

PWD Notes:

Related CSR status bits are: CARD_IS_LOCKED and LOCK_UNLOCK_FAILED. Presence of
the locking feature is indicated by the CCC "class 7" bit in CSD register.

Many SD cards are internally containing separate controller and memory chips,
so it might be quite easy to bypass the locking by desoldering one of those
chips.

| DSi SD/MMC Protocol: State |
| --- |

**SD/MMC State**

The "state" is an important SD/MMC feature to deal with - most commands can be
send only in certain states:

For normal operation, the card should be in "tran" state (and it may then
temporarily switch to "data/rcv/prg" states during read/write/erase commands).

For initialization, the card should be first forced to "idle" state, and the
init commands should then go through "ready/ident/stby" states, until finally
reaching "tran" state.

Less important states are "dis/ina", and, on MMC only, "btst/slp/irq" and
"pre-idle".

Both SD and MMC specs are leaving state undocumented for SPI mode (meaning that
SPI specific commands like CMD58/CMD59 are supported only in "unknown" state).

**SD Card State Transition Table**

```

| Command old state --> idle readyidentstby tran data rcv prg dis ina
 DONE Operation Complete ---- ---- ---- ---- ---- tran ---- tran stby ----
 class 0
 CMD0 GO_IDLE_STATE ok idle idle idle idle idle idle idle idle ----
 CMD2 ALL_SEND_CID ---- ident---- ---- ---- ---- ---- ---- ---- ----
 CMD3 SEND_RELATIVE_ADDR ---- ---- stby ok ---- ---- ---- ---- ---- ----
 CMD4 SET_DSR ---- ---- ---- ok ---- ---- ---- ---- ---- ----
 CMD7 SELECT_DESELECT_CARD
 card is addressed ---- ---- ---- tran ---- ---- ---- ---- prg ----
 card is not addr. ---- ---- ---- ok stby stby ---- dis ---- ----
 CMD8 SEND_IF_COND ok ---- ---- ---- ---- ---- ---- ---- ---- ----
 CMD9 SEND_CSD ---- ---- ---- ok ---- ---- ---- ---- ---- ----
 CMD10 SEND_CID ---- ---- ---- ok ---- ---- ---- ---- ---- ----
 CMD11 VOLTAGE_SWITCH ---- ok ---- ---- ---- ---- ---- ---- ---- ----
 CMD12 STOP_TRANSMISSION ---- ---- ---- ---- ---- tran prg ---- ---- ----
 CMD13 SEND_STATUS ---- ---- ---- ok ok ok ok ok ok ----
 CMD15 GO_INACTIVE_STATE ---- ---- ---- ina ina ina ina ina ina ----
 class 2
 CMD16 SET_BLOCKLEN ---- ---- ---- ---- ok ---- ---- ---- ---- ----
 CMD17 READ_SINGLE_BLOCK ---- ---- ---- ---- data ---- ---- ---- ---- ----
 CMD18 READ_MULTIPLE_BLOCK ---- ---- ---- ---- data ---- ---- ---- ---- ----
 CMD19 SEND_TUNING_BLOCK ---- ---- ---- ---- data ---- ---- ---- ---- ----
 CMD20 SPEED_CLASS_CONTROL ---- ---- ---- ---- prg ---- ---- ---- ---- ----
 CMD23 ---- ---- ---- ---- ok ---- ---- ---- ---- ----
 class 4
 CMD16 SET_BLOCKLEN (2)---- ---- ---- ---- ok ---- ---- ---- ---- ----
 CMD20 SPEED_CLASS_CONTROL(2)---- ---- ---- ---- prg ---- ---- ---- ---- ----
 CMD23 SET_BLOCK_COUNT ---- ---- ---- ---- ok ---- ---- ---- ---- ----
 CMD24 WRITE_BLOCK ---- ---- ---- ---- rcv ---- ---- ---- ---- ----
 CMD25 WRITE_MULTIPLE_BLOCK ---- ---- ---- ---- rcv ---- ---- ---- ---- ----
 CMD27 PROGRAM_CSD ---- ---- ---- ---- rcv ---- ---- ---- ---- ----
 class 6
 CMD28 SET_WRITE_PROT ---- ---- ---- ---- prg ---- ---- ---- ---- ----
 CMD29 CLR_WRITE_PROT ---- ---- ---- ---- prg ---- ---- ---- ---- ----
 CMD30 SEND_WRITE_PROT ---- ---- ---- ---- data ---- ---- ---- ---- ----
 class 5
 CMD32 ERASE_WR_BLK_START ---- ---- ---- ---- ok ---- ---- ---- ---- ----
 CMD33 ERASE_WR_BLK_END ---- ---- ---- ---- ok ---- ---- ---- ---- ----
 CMD38 ERASE ---- ---- ---- ---- prg ---- ---- ---- ---- ----
 class 7
 CMD40 Read Block (DPS Spec) ---- ---- ---- ---- data ---- ---- ---- ---- ----
 CMD42 LOCK_UNLOCK ---- ---- ---- ---- rcv ---- ---- ---- ---- ----
 class 8
 CMD55 APP_CMD ok ---- ---- ok ok ok ok ok ok ----
 CMD56 GEN_CMD, RD/WR=0 ---- ---- ---- ---- rcv ---- ---- ---- ---- ----
 GEN_CMD, RD/WR=1 ---- ---- ---- ---- data ---- ---- ---- ---- ----
 ACMD6 SET_BUS_WIDTH ---- ---- ---- ---- ok ---- ---- ---- ---- ----
 ACMD13 SD_STATUS ---- ---- ---- ---- data ---- ---- ---- ---- ----
 ACMD22 SEND_NUM_WR_BLOCKS ---- ---- ---- ---- data ---- ---- ---- ---- ----
 ACMD23 SET_WR_BLK_ERASE_CO. ---- ---- ---- ---- ok ---- ---- ---- ---- ----
 ACMD41 SD_SEND_OP_COND
 OCR check is OK
 and card is not busy ready---- ---- ---- ---- ---- ---- ---- ---- ----
 OCR check is OK
 and card is busy(2) ok ---- ---- ---- ---- ---- ---- ---- ---- ----
 OCR check fails
 query mode ina ---- ---- ---- ---- ---- ---- ---- ---- ----
 ACMD42 SET_CLR_CARD_DETECT ---- ---- ---- ---- ok ---- ---- ---- ---- ----
 ACMD51 SEND_SCR ---- ---- ---- ---- data ---- ---- ---- ---- ----
 class 9
 class 10 (1)
 CMD6 SWITCH_FUNC ---- ---- ---- ---- data ---- ---- ---- ---- ----
 class 11
 CMD48 READ_EXTR_SINGLE ---- ---- ---- ---- data ---- ---- ---- ---- ----
 CMD49 WRITE_EXTR_SINGLE ---- ---- ---- ---- rcv ---- ---- ---- ---- ----
 CMD58 READ_EXTR_MULTI ---- ---- ---- ---- data ---- ---- ---- ---- ----
 CMD59 WRITE_EXTR_MULTI ---- ---- ---- ---- rcv ---- ---- ---- ---- ----
 ACMD14-16 Refer to DPS Specification (class 8)
 ACMD28 Refer to DPS Specification (class 8)
 ACMD18,25,26,38,
 43,44,45,46,47,48,49 Refer to the "Part3 Security Specification" for
 information about the SD Security Features (class 8)
 CMD52-CMD54 Refer to the "SDIO Card Specification" (class 9)
 CMD21 Refer to DPS Specification (class 11)
 CMD34-37,50,57 Refer to each command system specification (class 10)
 CMD41,CMD43-47 reserved (class 11)
 CMD60...CMD63 reserved for manufacturer (class 11)
 SPI Mode
 CMD1 SEND_OP_COND SPI-only
 CMD58 READ_OCR SPI-only
 CMD59 CRC_ON_OFF SPI-only |
| --- |

Note (1): Class 10 commands were defined in Version 1.10.

Note (2): Card returns busy in case of following:

```

| - Card executes internal initialization process
 - When HCS in the argument is set to 0 to SDHC or SDXC Card. |
| --- |

The state transitions of the SD Memory Card application-specific commands are
given under Class 8, above.

```

| ---- command is treated as illegal command
 ok command is accepted, and card stays in SAME state
 xxx command is accepted, and card switches to "xxx" state |
| --- |

**MMC Card State Transition Table (JEDEC)**

```

| Command old state --> idl rdy idt stb trn dta tst rcv prg dis ina slp irq
 Class Independent
 ERR CRC error --- --- --- --- --- --- --- --- --- --- --- --- stb
 ERR command not supported--- --- --- --- --- --- --- --- --- --- --- --- stb
 Class 0
 CMD0 (arg=00000000h) ok idl idl idl idl idl idl idl idl idl --- idl stb
 GO_IDLE_STATE
 CMD0 (arg=F0F0F0F0h) pre pre pre pre pre pre pre pre pre pre --- pre stb
 GO_PRE_IDLE_STATE
 CMD0 (arg=FFFFFFFAh) initiate alternative boot operation
 BOOT_INITIATION
 CMD1 SEND_OP_COND
 card VDD range ok rdy --- --- --- --- --- --- --- --- --- --- --- stb
 card is busy ok --- --- --- --- --- --- --- --- --- --- --- stb
 card VDD range bad ina --- --- --- --- --- --- --- --- --- --- --- stb
 CMD2 ALL_SEND_CID
 card wins bus --- idt --- --- --- --- --- --- --- --- --- --- stb
 card loses bus --- ok --- --- --- --- --- --- --- --- --- --- stb
 CMD3 SET_RELATIVE_ADDR --- --- stb --- --- --- --- --- --- --- --- --- stb
 CMD4 SET_DSR --- --- --- ok --- --- --- --- --- --- --- --- stb
 CMD5 SLEEP_AWAKE --- --- --- slp -?- -?- -?- -?- -?- -?- -?- stb stb
 CMD6 SWITCH --- --- --- --- prg --- --- --- --- --- --- --- stb
 CMD7 SELECT_DESELECT_CARD
 card is addressed --- --- --- trn --- --- --- --- --- prg --- --- stb
 card is not addr. --- --- --- --- stb stb --- --- dis --- --- --- stb
 CMD8 SEND_EXT_CSD --- --- --- --- dta --- --- --- --- --- --- --- stb
 CMD9 SEND_CSD --- --- --- ok --- --- --- --- --- --- --- --- stb
 CMD10 SEND_CID --- --- --- ok --- --- --- --- --- --- --- --- stb
 CMD12 STOP_TRANSMISSION --- --- --- --- --- trn --- prg --- --- --- --- stb
 CMD13 SEND_STATUS --- --- --- ok ok ok ok ok ok ok --- --- stb
 CMD14 BUSTEST_R --- --- --- --- --- --- trn --- --- --- --- --- stb
 CMD15 GO_INACTIVE_STATE --- --- --- ina ina ina ina ina ina ina --- --- stb
 CMD19 BUSTEST_W --- --- --- --- tst --- --- --- --- --- --- --- stb
 Class 1
 CMD11 READ_DAT_UNTIL_STOP --- --- --- --- dta --- --- --- --- --- --- --- stb
 Class 2
 CMD16 SET_BLOCKLEN --- --- --- --- ok --- --- --- --- --- --- --- stb
 CMD17 READ_SINGLE_BLOCK --- --- --- --- dta --- --- --- --- --- --- --- stb
 CMD18 READ_MULTIPLE_BLOCK --- --- --- --- dta --- --- --- --- --- --- --- stb
 CMD23 SET_BLOCK_COUNT --- --- --- --- ok --- --- --- --- --- --- --- stb
 Class 3
 CMD20 WRITE_DAT_UNTIL_STOP--- --- --- --- rcv --- --- --- --- --- --- --- stb
 Class 4
 CMD16 SET_BLOCKLEN see class 2
 CMD23 SET_BLOCK_COUNT see class 2
 CMD24 WRITE_BLOCK --- --- --- --- rcv --- --- --- rcv1--- --- --- stb
 CMD25 WRITE_MULTIPLE_BL. --- --- --- --- rcv --- --- --- rcv2--- --- --- stb
 CMD26 PROGRAM_CID --- --- --- --- rcv --- --- --- --- --- --- --- stb
 CMD27 PROGRAM_CSD --- --- --- --- rcv --- --- --- --- --- --- --- stb
 Class 6
 CMD28 SET_WRITE_PROT --- --- --- --- prg --- --- --- --- --- --- --- stb
 CMD29 CLR_WRITE_PROT --- --- --- --- prg --- --- --- --- --- --- --- stb
 CMD30 SEND_WRITE_PROT --- --- --- --- dta --- --- --- --- --- --- --- stb
 CMD31 SEND_WRITE_PROT_TYPE--- --- --- --- dta --- --- --- --- --- --- --- stb
 Class 5
 CMD35 ERASE_GROUP_START --- --- --- --- ok --- --- --- --- --- --- --- stb
 CMD36 ERASE_GROUP_END --- --- --- --- ok --- --- --- --- --- --- --- stb
 CMD38 ERASE --- --- --- --- prg --- --- --- --- --- --- --- stb
 Class 7
 CMD16 SET_BLOCKLEN see class 2
 CMD42 LOCK_UNLOCK --- --- --- --- rcv --- --- --- --- --- --- --- stb
 Class 8
 CMD55 APP_CMD --- --- --- ok ok ok ok ok ok ok --- --- ok
 CMD56 GEN_CMD, RD/WR=0 --- --- --- --- rcv --- --- --- --- --- --- --- stb
 GEN_CMD, RD/WR=1 --- --- --- --- dta --- --- --- --- --- --- --- stb
 Class 9
 CMD39 FAST_IO --- --- --- ok --- --- --- --- --- --- --- --- stb
 CMD40 GO_IRQ_STATE --- --- --- irq --- --- --- --- --- --- --- --- stb
 Class 10-11
 CMD41, CMD43..CMD54 Reserved
 CMD57..CMD59 Reserved
 CMD60..CMD63 Reserved for Manufacturer
 SPI Mode
 CMD58 READ_OCR SPI-only
 CMD59 CRC_ON_OFF SPI-only |
| --- |

NOTE 1. Due to legacy considerations, a card may treat CMD24/25 during a prg
state_while busy is active_as a legal or an illegal command. A card that treats
CMD24/25 during a prg-state_while busy is active_as an illegal command will not
change its state to the rcv state. A host should not send CMD24/25 while the
card is in prg state and busy is active.

NOTE 2. Due to legacy considerations, a card may treat CMD24/25 during a prg
state_while busy is active_as a legal or an illegal command. A card that treats
CMD24/25 during a prg state_while busy is active_as an illegal command will not
change its state to the rcv state. A host should not send CMD24/25 while the
card is in prg state and busy is active.

NOTE 3. As there is no way to obtain state information in boot mode, boot-mode
states are not shown in this table.

```

| pre Pre-idle
 idl idle
 rdy ready
 idt ident
 stb stby
 trn tran
 dta data
 tst btst |
| --- |

| DSi SD/MMC Protocol: Signals |
| --- |

XXX...

**SD Mode 1-bit data transfer mode**

```

| __ start bit __ checksum bits (CRC-CCITT)
 | |
 | <------------data bits-------------> | __ stop bit
 | | |
 DAT0 0 1st 2nd 3rd 4th 5th 6th 7th ... last crc 1 |
| --- |

**SD Mode 4-bit data transfer mode**

```

| __ start bit __ checksum bits (CRC-CCITT)
 | |
 | <--data bits--> | __ stop bit
 | | |
 DAT3 0 1st 5th ... ... crc 1
 DAT2 0 2nd 6th ... ... crc 1
 DAT1 0 3rd 7th ... ... crc 1
 DAT0 0 4th 8th ... last crc 1 |
| --- |

| DSi SDIO Special SDIO Commands |
| --- |

**CMD52 - SDIO: IO_RW_DIRECT**

Read/write single byte. Mostly used for detection/configuration via SDIO
Function 0 commands.

```

| 31 R/W Flag (0=Read, 1=Write)
 30-28 Function Number (3bit)
 27 Read-after-write (RAW) Flag (if Bit31=1=Write, and Bit27=1)
 26 Stuff (unspecified, should be probably 0, but is 1 on DSi)
 25-9 Register Address (17bit)
 8 Stuff (unspecified, should be probably 0, but is 1 on DSi)
 7-0 Write Data (8bit), or Stuff bits (for read) |
| --- |

SD Mode Response: R5:

```

| 47 Start Bit (0) ;\
 46 Transmission To Host (0) ; 1st byte
 45-40 Command (the 6bit CMD being responded to) ;/
 39-24 Stuff Bits ;-2nd..3rd byte
 23-16 Response Flags ;-4th byte
 7 COM_CRC_ERROR
 6 ILLEGAL_COMMAND
 5-4 IO_CURRENT STATE (0=dis, 1=cmd, 2=trn(cmd53), 3=rfu)
 3 ERROR
 2 RFU (reserved for future use)
 1 INVALID_FUNCTION_NUMBER
 0 OUT_OF_RANGE
 15-8 Read or Write Data (8bit) ;-5th byte
 7-1 CRC7 ;\6th byte
 0 End Bit (1) ;/ |
| --- |

SPI Mode Response: R5:

```

| 8bit modified R1 response
 7 start bit (0)
 6 parameter error (0=okay, 1=error)
 5 RFU (0)
 4 function number error (0=okay, 1=error)
 3 COM CRC error (0=okay, 1=error)
 2 illegal command (0=okay, 1=error)
 1 RFU (0)
 0 in idle state (0=no, 1=idle)
 8bit Read or Write Data |
| --- |

**CMD53 - SDIO: IO_RW_EXTENDED**

Mostly used for actual command/data transfers via SDIO Function 1 commands.

```

| 31 R/W Flag (0=Read, 1=Write)
 30-28 Function Number (3bit) (0=CIA)
 27 Block Mode (0=Bytes, 1=Blocks/optional)
 26 OP Code (0=Fixed Address, 1=Incrementing Address)
 25-9 Register Address (17bit)
 8-0 Byte/Block Count (9bit) (1..511) (0=512 Bytes, or 0=Infinite Blocks) |
| --- |

Response: R5: Same as for CMD52 (with 8bit data = 00h)

Data Transfer:

```

| For Byte Mode: Similar to CMD17/CMD24 (single block)
 For Block Mode: Similar to CMD18/CMD25 (multiple block)
 For Block Mode: CMD52:STOP_TRANSMISSION only needed if using "InfiniteBlocks" |
| --- |

**CMD5 - SPI - SDIO: IO_SEND_OP_COND**

Similar to SD Memory Card's ACMD41.

```

| 31-25 stuff bits (0)
 24 Switching to 1.8V Request (S18R)
 23 I/O OCR VDD Voltage Window 3.5V-3.6V
 22 I/O OCR VDD Voltage Window 3.4V-3.5V
 21 I/O OCR VDD Voltage Window 3.3V-3.4V
 20 I/O OCR VDD Voltage Window 3.2V-3.3V
 19 I/O OCR VDD Voltage Window 3.1V-3.2V
 18 I/O OCR VDD Voltage Window 3.0V-3.1V
 17 I/O OCR VDD Voltage Window 2.9V-3.0V
 16 I/O OCR VDD Voltage Window 2.8V-2.9V
 15 I/O OCR VDD Voltage Window 2.7V-2.8V
 14 I/O OCR VDD Voltage Window 2.6V-2.7V
 13 I/O OCR VDD Voltage Window 2.5V-2.6V
 12 I/O OCR VDD Voltage Window 2.4V-2.5V
 11 I/O OCR VDD Voltage Window 2.3V-2.4V
 10 I/O OCR VDD Voltage Window 2.2V-2.3V
 9 I/O OCR VDD Voltage Window 2.1V-2.2V
 8 I/O OCR VDD Voltage Window 2.0V-2.1V
 7-4 I/O OCR VDD Voltage Window Reserved
 3-0 I/O OCR VDD Voltage Window Reserved |
| --- |

SD Mode Response: R4:

```

| 47 Start Bit (0) ;\
 46 Transmission To Host (0) ; 1st byte
 45-40 Reserved (111111) (instead of Command value) ;/
 39 Card is ready to operate after init ;\
 38-36 Number of I/O Functions ;
 35 Memory Present ; 2nd byte
 34-33 Stuff bits (0) ;
 32 Switching to 1.8V Accepted (S18R) (not SPI) ;/
 31-8 I/O OCR (24bit) ;-3rd..5th byte
 7-1 Reserved (111111) (instead of CRC7) ;\6th byte
 0 End Bit (1) ;/ |
| --- |

SPI Mode Response: R4:

```

| 8bit modified R1 Response
 7 start bit (0)
 6 parameter error (0=okay, 1=error)
 5 RFU (0)
 4 function number error (0=okay, 1=error)
 3 COM CRC error (0=okay, 1=error)
 2 illegal command (0=okay, 1=error)
 1 RFU (0)
 0 in idle state (0=no, 1=idle)
 32bit same as SD Response bit39-8 (but without S18R bit) |
| --- |

**DSi SDIO Wifi Init**

Related required registers/bits are:

```

| - SCFG_EXT7.bit19 needed for SDIO controller (else 4004Axxh-4004Bxxh disabled)
 - SCFG_CLK7 seems to be NOT needed for SDIO clock enable (unlike SDMMC)
 - SCFG_WL.bit0 seems to be wifi-related (but effect is unknown)
 - GPIO_WIFI.bit8 needed for AR6013G chips (else SDIO Function 1 fails)
 - BPTWL[30h] needed for LED and SDIO (else SDIO fails badly)
 - RTC.FOUT pin as configured by firmware (else WMI commands/events fail) |
| --- |

DSi init sequence is trying to send one CMD52 command first; if that fails,
then the DSi is sending several CMD5's, followed by CMD3+CMD7.

**SDIO State**

```

| Command ini stb cmd trn ina
 CMD3 SET_RELATIVE_ADDR stb ok --- --- ---
 CMD5 IO_SEND_OP_COND ok --- --- --- ---
 ocr bad ina --- --- --- ---
 CMD7 SELECT_CARD --- cmd ok --- ---
 DESELECT_CARD --- ok stb --- ---
 CMD15 GO_INACTIVE_STATE ina ina ina --- ---
 CMD52 IO_RW_DIRECT --- --- ok (cmd)---
 CMD53 IO_RW_EXTENDED --- --- trn --- --- |
| --- |

Note: In CMD52, state "dis" can mean state "ini","stb","ina" (though,
theoretically CMD52 cannot be used in that states, so one should never see the
"dis" state at all).

**More SD commands that are (occassionally) used for SDIO**

```

| CMD0 GO_IDLE_STATE for entering SPI mode only (but does NOT reset SDIO)
 CMD8 SEND_IF_COND optional for SDHC/SDXC
 CMD11 VOLTAGE_SWITCH optional for UHS-I
 CMD19 SEND_TUNING_BLOCK optional for UHS-I
 CMD59 CRC_ON_OFF spi-only |
| --- |

Moreover a combo card (a SDIO device with built-in SD memory card) may
implement various SD commands; these commands will affect only the SD memory
card side, not the SDIO device).

SDIO doesn't have CID or CSD registers, nor commands for STOP_TRANSMISSION,
SET_BUSWIDTH, or SET_BLOCKLEN (but CMD52 can do equivalent stuff via SDIO
Function 0 registers).

```

| ____________________________ I/O Commands for MMC____________________________ |
| --- |

**CMD39 - MMCIO: FAST_IO (type=ac)**

```

| 31-16 RCA
 15 Register Write Flag
 14-8 Register Address
 7-0 Register Data |
| --- |

MMC Response: R4:

```

| 47 Start Bit (0) ;\
 46 Transmission To Host (0) ; 1st byte
 45-40 Command (the 6bit CMD being responded to) ;/
 39-24 RCA ;-2nd..3rd byte
 23 Status (0=Bad, 1=Successful) ;\4th byte
 22-16 Register Address ;/
 15-8 Read Register Contents ;-5th byte
 7-1 CRC7 ;\6th byte
 0 End Bit (1) ;/ |
| --- |

**CMD40 - MMCIO: GO_IRQ_STATE (type=bcr)**

```

| 31-0 Stuff Bits |
| --- |

MMC Response: R5:

```

| 47 Start Bit (0) ;\
 46 Transmission To Host (0) ; 1st byte
 45-40 Command (the 6bit CMD being responded to) ;/
 39-24 RCA ;-2nd..3rd byte
 23-8 Not defined (may be used for IRQ data) ;-4th..5th byte
 7-1 CRC7 ;\6th byte
 0 End Bit (1) ;/ |
| --- |

| DSi SDIO Memory and I/O Maps |
| --- |

**Function 0 - Common I/O Area (CIA)**

```

| 0:00000h..000FFh Card Common Control Registers (CCCR)
 0:00100h..001FFh Function Basic Registers (FBR) for Function 1
 0:00200h..002FFh Function Basic Registers (FBR) for Function 2
 0:00300h..003FFh Function Basic Registers (FBR) for Function 3
 0:00400h..004FFh Function Basic Registers (FBR) for Function 4
 0:00500h..005FFh Function Basic Registers (FBR) for Function 5
 0:00600h..006FFh Function Basic Registers (FBR) for Function 6
 0:00700h..007FFh Function Basic Registers (FBR) for Function 7
 0:00800h..00FFFh Reserved for Future
 0:01000h..17FFFh Card Information Structures (Common CIS and Func 1-7 CIS)
 0:18000h..1FFFFh Reserved for Future |
| --- |

**Function 1..7 - Function specific Register Space**

```

| n:00000h..1FFFFh Registers (seven 128K spaces, one for each function) |
| --- |

**Code Storage Area (CSA) (optional, R or R/W)**

```

| CSA:00000h..FFFFFh 16Mbyte FAT12/FAT16 (accessed indirectly via "Window") |
| --- |

**Card Common Control Registers (CCCR)**

```

| 0:00000h 2 CCCR: Revision (R)
 0:00002h 2 CCCR: I/O Function Enable/Ready (R/W)
 0:00004h 2 CCCR: Interrupt Enable/Pending (R/W)
 0:00006h 1 CCCR: I/O Abort (W)
 0:00007h 1 CCCR: Bus Interface Control (R/W)
 0:00008h 1 CCCR: Card Capability
 0:00009h 3 CCCR: Common CIS Pointer, Lo/Mid/Hi
 0:0000Ch 1 CCCR: Bus Suspend
 0:0000Dh 1 CCCR: Function Select (R/W)
 0:0000Eh 2 CCCR: Exec/Ready Flags (R)
 0:00010h 2 CCCR: CMD53 Block Size for Function 0, Lo/Hi (R/W)
 0:00012h 1 CCCR: Power Control
 0:00013h 2 CCCR: Bus Speed Select
 0:00015h 1 CCCR: Driver Strength
 0:00016h 1 CCCR: Interrupt Extension
 0:00017h D9h CCCR: Reserved for Future
 0:000F0h 10h CCCR: Reserved for Vendors |
| --- |

**Function Basic Registers (FBR) for Function n (n=1..7)**

```

| 0:00n00h 1 FBR(n): Misc
 0:00n01h 1 FBR(n): Extended standard SDIO Function interface code
 0:00n02h 1 FBR(n): Misc
 0:00n02h 7 FBR(n): Reserved for Future
 0:00n09h 3 FBR(n): Pointer to Card Information Structure (CIS), Lo/Mid/Hi
 0:00n0Ch 3 FBR(n): Code Storage Area (CSA) Address, Lo/Mid/Hi
 0:00n0Fh 1 FBR(n): Code Storage Area (CSA) Data "Window"
 0:00n10h 2 FBR(n): CMD53 Block Size for Function n, Lo/Hi
 0:00n12h EEh FBR(n): Reserved for Future |
| --- |

| DSi SDIO Common Control Registers (CCCR) |
| --- |

**0:00000h - CCCR: Revision (R)**

```

| 0-3 CCCR/FBR Format Version (0=v1.00, 1=v1.10, 2=v2.00, 3=v3.00) (R)
 4-7 SDIO Spec Version (0=v1.00, 1=v1.10, 2=v1.20, 3=v2.00, 4=v3.00) (R)
 8-11 SD Physical Layer Spec (0=v1.01, 1=v1.10, 2=v2.00, 3=v3.0x) (R)
 12-15 Reserved for Future (-) |
| --- |

**0:00002h - CCCR: I/O Function Enable/Ready (R/W)**

```

| 0 Reserved for Future (-)
 1-7 SDIO Function 1..7 Enable Flags (0=Disable, 1=Enable) (R/W)
 8 Reserved for Future (-)
 9-15 SDIO Function 1..7 Ready Flags (0=Disabled/Busy, 1=Ready) (R) |
| --- |

**0:00004h - CCCR: Interrupt Enable/Pending (R/W)**

```

| 0 SDIO Interrupt Master Enable (0=Disable, 1=Enable) (R/W)
 1-7 SDIO Function 1..7 Interrupt Enable (0=Disable, 1=Enable) (R/W)
 8 Reserved for Future (-)
 9-15 SDIO Function 1..7 Interrupt Pending (0=No, 1=IRQ) (R) |
| --- |

**0:00006h - CCCR: I/O Abort (W)**

```

| 0-2 SDIO Function Number to be Aborted (0=None?, 1..7=Function 1..7) (W)
 XXXsee pg 35
 3 Reset SDIO Card (0=Normal, 1=Reset) (W)
 4-7 Reserved for Future (-) |
| --- |

**0:00007h - CCCR: Bus Interface Control (R/W)**

```

| 0-1 Bus Width (0=1bit, 1=Reserved, 2=4bit, 3=EmbeddedSDIO/8bit) (R/W)
 2 Support 8bit Bus Flag (0=No, 1=Yes/EmbeddedSDIO only) (R)
 3-4 Reserved for Future (-)
 5 Enable Continous SPI Interrupt (0=Disable, 1=Enable) (R/W)
 6 Support Continous SPI Interrupt (0=No, 1=Yes) (R)
 7 Card Detect Disable (0=Enable Pull-up on DAT3 pin, 1=Disable) (R/W) |
| --- |

**0:00008h - CCCR: Card Capability**

```

| 0 Support Direct Command (CMD52) during Data Transfer (0=No, 1=Yes) (R)
 1 Support Multi-Block transfer (CMD53.block mode) (0=No, 1=Yes) (R)
 2 Support Read Wait Control (RWC via DAT2 pin) (0=No, 1=Yes) (R)
 3 Support Bus Control Suspend/Resume (0=No, 1=Yes) (R)
 4 Support Block Gap Interrupt during Multi-Block (0=No, 1=Yes) (R)
 5 Enable Block Gap Interrupt during Multi-Block (0=No, 1=Enable) (R/W)
 6 Low Speed Card (0=Full-Speed, 1=Low-Speed) (R)
 7 Support 4bit Mode for Low-Speed Card (0=No, 1=Yes) (R) |
| --- |

**0:00009h - CCCR: Common CIS Pointer, Lo**

**0:0000Ah - CCCR: Common CIS Pointer, Mid**

**0:0000Bh - CCCR: Common CIS Pointer, Hi**

```

| 0-16 Pointer to Card Common Card Information Structure (Common CIS) (R)
 17-23 Unspecified (probably reserved) (-) |
| --- |

**0:0000Ch - CCCR: Bus Suspend**

```

| 0 Bus Status XXX see pg 37 (R)
 1 Bus Release Request XXX see pg 38 (R)
 2-7 Reserved for Future (-) |
| --- |

**0:0000Dh - CCCR: Function Select (R/W)**

```

| 0-3 Select Function (0=CIA, 1..7=Function 1..7, 8=Memory Card) (R/W)
 4-6 Reserved for Future (-)
 7 Data Flag (more data after resuming) (0=No, 1=Yes) (R) |
| --- |

**0:0000Eh - CCCR: Exec/Ready Flags (R)**

```

| 0 Command Execution Flag for Memory (=SD/Combo? or CSA?) (R)
 1-7 Command Execution Flags for Function 1..7 (0=Busy, 1=Ready) (R)
 8 Read/Write Ready Flag for Memory (=SD/Combo? or CSA?) (R)
 9-15 Read/Write Ready Flags for Function 1..7 (0=Busy, 1=Ready) (R) |
| --- |

**0:00010h - CCCR: CMD53 Block Size for Function 0, Lo (R/W)**

**0:00011h - CCCR: CMD53 Block Size for Function 0, Hi (R/W)**

```

| 0-15 CMD53 Block size for Function(0) (0001h..0800h) (0=None) (R/W) |
| --- |

**0:00012h - CCCR: Power Control**

```

| 0 Support Master Power Control (0=No, 1=Yes) (R)
 1 Enable Master Power Control (0=No/max 720mW, 1=Yes/allow more) (R/W)
 2-7 Reserved for Future (-) |
| --- |

**0:00013h,00014h - CCCR: Bus Speed Select**

```

| 0 Support High-Speed Mode (SDR25 or higher) (0=No, 1=Yes) (R)
 1-3 Bus Speed Select (0=SDR12, 1=SDR25, 2=SDR50, 3=SDR104, 4=DDR50) (R/W)
 4-7 Reserved for Future (-)
 8 Support UHS-I SDR50 (usable in 1.8V mode only) (0=No, 1=Yes) (R)
 9 Support UHS-I SDR104 (usable in 1.8V mode only) (0=No, 1=Yes) (R)
 10 Support UHS-I DDR50 (usable in 1.8V mode only) (0=No, 1=Yes) (R)
 11-15 Reserved for Future |
| --- |

**0:00015h - CCCR: Driver Strength**

```

| 0 Support Driver Type A ;\see Physical Layer Specs (0=No, 1=Yes) (R)
 1 Support Driver Type C ; version 3.0x for details (0=No, 1=Yes) (R)
 2 Support Driver Type D ;/ (0=No, 1=Yes) (R)
 3 Reserved for Future (-)
 5-4 Driver Type Select (0=Default/B, 1=Type A, 2=Type C, 3=Type D) (R/W)
 7-6 Reserved for Future (-) |
| --- |

**0:00016h - CCCR: Interrupt Extension**

```

| 0 Support Asynchronous Interrupt in 4bit mode (0=No, 1=Yes) (R)
 1 Enable Asynchronous Interrupt in 4bit mode (0=No, 1=Enable) (R/W)
 7-2 Reserved for Future (-) |
| --- |

| DSi SDIO Function Basic Registers (FBR) |
| --- |

**0:00n00h - FBR(n): Interface Type**

```

| 0-3 Standard SDIO Function Interface Code (R)
 4-5 Reserved for Future (-)
 6 Code Storage Area (CSA) Supported (0=No, 1=Yes) (R)
 7 Code Storage Area (CSA) Enable (0=Block reads/writes, 1=Enable) (R/W)
 8-15 Extended standard SDIO Function interface code (when bit0-3=0Fh) (R) |
| --- |

The interface type is merged of bit0-3 and bit8-15:

```

| 0h:00h = No SDIO standard interface (eg. Atheros Wifi in DSi)
 1h:00h = SDIO Standard UART
 2h:00h = SDIO Bluetooth Type-A standard interface
 3h:00h = SDIO Bluetooth Type-B standard interface
 4h:00h = SDIO GPS standard interface
 5h:00h = SDIO Camera standard interface
 6h:00h = SDIO PHS standard interface
 7h:00h = SDIO WLAN interface
 8h:00h = Embedded SDIO-ATA standard interface
 9h:00h = SDIO Bluetooth Type-A Alternate MAC PHY (AMP) standard interface
 Ah:00h = Reserved for Future
 Bh:00h = Reserved for Future
 Ch:00h = Reserved for Future
 Dh:00h = Reserved for Future
 Eh:00h = Reserved for Future
 Fh:00h..FFh = Reserved for Future |
| --- |

**0:00n02h - FBR(n): Power**

```

| 0 Support Power Selection (0=No, 1=Yes) (R)
 1 Enable Power Selection (0=Normal Current, 1=Lower Current) (R/W)
 2-3 Reserved for Future (-)
 4-7 Power State (R/W) |
| --- |

**0:00n09h-00n0Bh - FBR(n): Pointer to Card Information Structure (CIS)**

```

| 0-16 Pointer to Function(n)'s Card Information Structure (Function CIS)(R)
 17-23 Unspecified (probably reserved) (-) |
| --- |

Should point to "End-of-Chain Tuple" for unsupported functions?

**0:00n0Ch-00n0Eh - FBR(n): Code Storage Area (CSA) Address (R/W)**

```

| 0-23 Pointer to CSA memory (incremented after CSA data read/write) (R/W) |
| --- |

**0:00n0Fh - FBR(n): Code Storage Area (CSA) Data "Window" (R or R/W)**

```

| 0-7 Data (to/from auto-incrementing CSA Address) (R for ROM, R/W otherwise) |
| --- |

**0:00n10h-00n11h - FBR(n): CMD53 Block Size (R/W)**

```

| 0-15 CMD53 Block size for Function(n) (0001h..0800h) (0=None) (R/W) |
| --- |

| DSi SDIO Card Information Structures (CIS) |
| --- |

The CIS used by SDIO is based directly upon the metaformat specification used
by PCMCIA and Compact Flash. For details on the metaformat, refer to:

```

| PC Card Standard, Volume 4, Metaformat Specification |
| --- |

Published by: PCMCIA (Personal Computer Memory Card International Association)

**Basic Format**

```

| 00h CISTPL_code
 01h Offset to next tuple (n) (aka size of body)
 02h+(0..n-1) Body (n bytes) |
| --- |

**Summary**

```

| 00h = CISTPL_NULL Null Tuple
 10h = CISTPL_CHECKSUM Checksum Control
 15h = CISTPL_VERS_1 Level 1 Version/Product Information
 16h = CISTPL_ALTSTR Alternate Language String
 20h = CISTPL_MANFID Manufacturer ID
 21h = CISTPL_FUNCID Function ID
 22h = CISTPL_FUNCE Function Extensions
 80h-8Fh = Vendor specific Vendor specific
 91h = CISTPL_SDIO_STD Info for Standard SDIO Functions
 92h = CISTPL_SDIO_EXT Reserved for future SDIO stuff
 FFh = CISTPL_END End-of-chain |
| --- |

**Tuple 00h - Null Tuple (meaningless, unknown purpose)**

```

| 00h Tuple ID (00h)
 01h Tuple Size (00h) |
| --- |

Unspecified in SDIO reference... but maybe defined in PCMIA specs?)

**Tuple 10h - Checksum Control**

```

| 00h Tuple ID (10h)
 01h Tuple Size (?)
 ... Unknown |
| --- |

Unspecified in SDIO reference... but maybe defined in PCMIA specs?)

**Tuple 15h - Level 1 Version/Product Information**

```

| 00h Tuple ID (15h)
 01h Tuple Size (?)
 ... Unknown |
| --- |

Unspecified in SDIO reference... but maybe defined in PCMIA specs?)

**Tuple 20h - Manufacturer ID**

```

| 00h Tuple ID (20h)
 01h Tuple Size (at least 4)
 02h-03h Manufacturer ID (assigned by JEIDA or PCMCIA)
 04h-05h Part Number/Revision (manufacturer specific) |
| --- |

**Tuple 21h - Function ID**

```

| 00h Tuple ID (21h)
 01h Tuple Size (2)
 02h Card Function Code (0Ch for SDIO)
 03h System initialization bit mask (Not used, 00h) |
| --- |

**Tuple 22h - Function Extension**

```

| 00h Tuple ID (22h)
 01h Tuple Size (..)
 02h Type of extended data
 03h..xxh Function information |
| --- |

**Tuple 22h - Function Extension, Type 00h, for Function 0**

```

| 00h Tuple ID (22h)
 01h Tuple Size (04h+2*N)
 02h Type of extended data (00h=Type 00h)
 03h-04h Max Block Size for Function 0 (0001h or higher)
 05h Max Transfer Speed for Function 0-7 (specified as Value*Unit bits/s)
 bit0-2: Unit (0=0.1M, 1=1M, 2=10M, 3=100M, 4..7=Reserved)
 bit3-6: Value (0=Reserved, 1=1, 2=1.2, 3=1.3, 4=1.5, 5=2, 6=2.5,
 7=3, 8=3.5, 9=4, 10=4.5, 11=5, 12=5.5, 13=6, 14=7, 15=8)
 bit7: Reserved
 06h... N two-byte pairs (TC,CP) for 1..N ;(N=([01h]-4)/2) |
| --- |

**Tuple 22h - Function Extension, Type 01h, for Function 1-7**

```

| 00h Tuple ID (22h)
 01h Tuple Size (2Ah)
 02h Type of extended data (01h=Type 01h)
 03h Function Info (bit0=WakeUpSupport, bit1..7=Reserved)
 04h Standard SDIO Function version (2x4bit maj.min, or 00h=Nonstandard)
 05h-08h Card Product Serial Number PSN (32bit) (unique value, or 0=None)
 09h-0Ch CSA Size in bytes available for this Function (32bit)
 0Dh CSA Property (bit0=WriteProtected/ReadOnly, bit1=NoReformatting)
 0Eh-0Fh Max Block Size for this Function (0001h or higher)
 10h-13h Operation Condition OCR (same as in ACMD41 for SD Memory devices)
 14h-16h 3x8bit Operation Power (Min/Average/Max) (0..254mA, or 255=more)
 17h-19h 3x8bit Standby Power (Min/Average/Max) (0..254mA, or 255=more)
 1Ah-1Dh 2x16bit Bandwidth (Min/Optimal) (1..65535 KB/sec, or 0=None)
 1Eh-1Fh Timeout for Enable-till-Ready in 10ms units (max 655.35 seconds)
 20h-23h 2x16bit Operation 3.3V (Average/Max) (1..65535mA, or 0=?)
 24h-25h 2x16bit High-Current-Mode 3.3V (Average/Max) (1..65535mA, or 0=?)
 28h-2Bh 2x16bit Low-Current-Mode 3.3V (Average/Max) (1..65535mA, or 0=?) |
| --- |

**Tuple 22h - Function Extension, Type 02h, for Function 1-7 (Power State)**

```

| 00h Tuple ID (22h)
 01h Tuple Size (02h+N*2) (N=1..15, for up to 15 power states)
 02h Type of extended data (02h=Type 02h)
 03h Fixed value (00h)
 04h..xxh Nx16bit Max consumption in Power State 1..N (0..65535mW) |
| --- |

**Tuple 91h - SDIO_STD - Info about Standard SDIO Functions**

```

| 00h Tuple ID (91h)
 01h Tuple Size (02h..FFh)
 02h SDIO STD ID (the 4+8bit Interface Type in FBR, squeezed into 8bits?)
 03h SDIO STD Type ;\depends on Interface Type
 04h... SDIO STD Data (if any) ;/ |
| --- |

**Tuple 92h - SDIO_EXT**

```

| 00h Tuple ID (92h)
 01h Tuple Size (?)
 02h... Reserved (if any) |
| --- |

**Tuple FFh - END**

```

| 00h Tuple ID (FFh) |
| --- |

Indicates the end of tuple's list. Unlike all other tuple's, this is only a
single FFh-byte (without "Tuple Size" entry).

01 CISTPL_DEVICE

14 CISTPL_NO_LINK

1A CISTPL_CONFIG

1B CISTPL_CFTABLE_ENTRY