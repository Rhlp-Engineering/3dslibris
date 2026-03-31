# Dswirelesscommunications

> Source: https://problemkaputt.de/gbatek.htm
> Section: Dswirelesscommunications

DS Wireless Communications 
| |
| --- |

 DS Wifi I/O Map
 DS Wifi Control
 DS Wifi Interrupts
 DS Wifi Power-Down Registers
 DS Wifi Receive Control
 DS Wifi Receive Buffer
 DS Wifi Receive Statistics
 DS Wifi Transmit Control
 DS Wifi Transmit Buffers
 DS Wifi Transmit Errors
 DS Wifi Status
 DS Wifi Timers
 DS Wifi Multiplay Master
 DS Wifi Multiplay Slave
 DS Wifi Configuration Ports
 DS Wifi Baseband Chip (BB)
 DS Wifi RF Chip
 DS Wifi RF9008 Registers
 DS Wifi Unknown Registers
 DS Wifi Unused Registers
 DS Wifi Initialization
 DS Wifi Flowcharts
 DS Wifi Hardware Headers
 DS Wifi Nintendo Beacons
 DS Wifi Nintendo DS Download Play
 DS Wifi IEEE802.11 Frames
 DS Wifi IEEE802.11 Managment Frames (Type=0)
 DS Wifi IEEE802.11 Control and Data Frames (Type=1 and 2)
 DS Wifi WPA/WPA2 Handshake Messages (EAPOL)
 DS Wifi WPA/WPA2 Keys and MICs
 DS Wifi WPA/WPA2 Encryption
 DS Wifi FFC ID
 DS Wifi Dslink/Wifiboot Protocol
 DS Firmware Wifi Calibration Data
 DS Firmware Wifi Internet Access Points
 
 2.4GHz band, Wireless LAN (WLAN) IEEE802.11b protocol
 
 **Credits**
 A very large part of the DS Wifi chapters is based on Stephen Stair's great DS Wifi document, thanks there.
 
 
 
| DS Wifi I/O Map |
| --- |

 **Notice**
 Wifi Registers & RAM cannot be written to by STRB opcodes (ignored).
 
 **Registers - NDS7 - 4808000h..4808FFFh**
 
```

| Address Dir Name r/w [Init] Description
 4808000h R W_ID ---- [1440] Chip ID (1440h=DS, C340h=DS-Lite)
 4808004h R/W W_MODE_RST 9fff [0000] Mode/Reset
 4808006h R/W W_MODE_WEP --7f [0000] Mode/Wep modes
 4808008h R/W W_TXSTATCNT ffff [0000] Beacon Status Request
 480800Ah R/W W_X_00Ah ffff [0000] [bit7 - ingore rx duplicates]
 4808010h R/W W_IF ackk [0000] Wifi Interrupt Request Flags
 4808012h R/W W_IE ffff [0000] Wifi Interrupt Enable
 4808018h R/W W_MACADDR_0 ffff [0000] Hardware MAC Address, 1st 2 bytes
 480801Ah R/W W_MACADDR_1 ffff [0000] Hardware MAC Address, next 2 bytes
 480801Ch R/W W_MACADDR_2 ffff [0000] Hardware MAC Address, last 2 bytes
 4808020h R/W W_BSSID_0 ffff [0000] BSSID (first 2 bytes)
 4808022h R/W W_BSSID_1 ffff [0000] BSSID (next 2 bytes)
 4808024h R/W W_BSSID_2 ffff [0000] BSSID (last 2 bytes)
 4808028h R/W W_AID_LOW ---f [0000] usually as lower 4bit of AID value
 480802Ah R/W W_AID_FULL -7ff [0000] AID value assigned by a BSS.
 480802Ch R/W W_TX_RETRYLIMIT ffff [0707] Tx Retry Limit (set from 00h-FFh)
 480802Eh R/W W_INTERNAL ---1 [0000]
 4808030h R/W W_RXCNT ff0e [0000] Receive control
 4808032h R/W W_WEP_CNT ffff [0000] WEP engine enable
 4808034h R? W_INTERNAL 0000 [0000] bit0,1 (see ports 004h,040h,1A0h) |
| --- |

**Power-Down Registers (and Random Generator)**

```

| 4808036h R/W W_POWER_US ---3 [0001]
 4808038h R/W W_POWER_TX ---7 [0003]
 480803Ch R/W W_POWERSTATE -r-2 [0200]
 4808040h R/W W_POWERFORCE 8--1 [0000]
 4808044h R W_RANDOM 0xxx [0xxx]
 4808048h R/W W_POWER_? ---3 [0000] |
| --- |

**Receive Control/Memory**

```

| 4808050h R/W W_RXBUF_BEGIN ffff [4000]
 4808052h R/W W_RXBUF_END ffff [4800]
 4808054h R W_RXBUF_WRCSR 0rrr [0000]
 4808056h R/W W_RXBUF_WR_ADDR -fff [0000]
 4808058h R/W W_RXBUF_RD_ADDR 1ffe [0000]
 480805Ah R/W W_RXBUF_READCSR -fff [0000]
 480805Ch R/W W_RXBUF_COUNT -fff [0000]
 4808060h R W_RXBUF_RD_DATA rrrr [xxxx]
 4808062h R/W W_RXBUF_GAP 1ffe [0000]
 4808064h R/W W_RXBUF_GAPDISP -fff [0000] |
| --- |

**Transmit Control/Memory**

```

| 4808068h R/W W_TXBUF_WR_ADDR 1ffe [0000]
 480806Ch R/W W_TXBUF_COUNT -fff [0000]
 4808070h W W_TXBUF_WR_DATA xxxx [xxxx]
 4808074h R/W W_TXBUF_GAP 1ffe [0000]
 4808076h R/W W_TXBUF_GAPDISP 0fff [0000]
 4808078h W W_INTERNAL mirr [mirr] Read: Mirror of 068h
 4808080h R/W W_TXBUF_BEACON ffff [0000] Beacon Transmit Location
 4808084h R/W W_TXBUF_TIM --ff [0000] Beacon TIM Index in Frame Body
 4808088h R/W W_LISTENCOUNT --ff [0000] Listen Count
 480808Ch R/W W_BEACONINT -3ff [0064] Beacon Interval
 480808Eh R/W W_LISTENINT --ff [0000] Listen Interval
 4808090h R/W W_TXBUF_CMD ffff [0000] Multiplay Command
 4808094h R/W W_TXBUF_REPLY1 ffff [0000] Multiplay Next Reply
 4808098h R W_TXBUF_REPLY2 0000 [0000] Multiplay Current Reply
 480809Ch R/W W_INTERNAL ffff [0050] value 4x00h --> preamble+x*12h us?
 48080A0h R/W W_TXBUF_LOC1 ffff [0000]
 48080A4h R/W W_TXBUF_LOC2 ffff [0000]
 48080A8h R/W W_TXBUF_LOC3 ffff [0000]
 48080ACh W W_TXREQ_RESET fixx [0050]
 48080AEh W W_TXREQ_SET fixx [0050]
 48080B0h R W_TXREQ_READ --1f [0010]
 48080B4h W W_TXBUF_RESET 0000 [0000] (used by firmware part4)
 48080B6h R W_TXBUSY 0000 [0000] (used by firmware part4)
 48080B8h R W_TXSTAT 0000 [0000]
 48080BAh ? W_INTERNAL 0000 [0000]
 48080BCh R/W W_PREAMBLE ---3 [0001]
 48080C0h R/W x W_CMD_TOTALTIME ffff [0000] (used by firmware part4)
 48080C4h R/W x W_CMD_REPLYTIME ffff [0000] (used by firmware part4) |
| --- |

**xxx**

```

| 48080C8h ? W_INTERNAL 0000 [0000]
 48080D0h R/W W_RXFILTER 1fff [0401]
 48080D4h R/W W_CONFIG_0D4h ---3 [0001]
 48080D8h R/W W_CONFIG_0D8h -fff [0004]
 48080DAh R/W W_RX_LEN_CROP ffff [0602]
 48080E0h R/W W_RXFILTER2 ---f [0008] |
| --- |

**Wifi Timers**

```

| 48080E8h R/W W_US_COUNTCNT ---1 [0000] Microsecond counter enable
 48080EAh R/W W_US_COMPARECNT ---1 [0000] Microsecond compare enable
 48080ECh R/W W_CONFIG_0ECh 3f1f [3F03]
 48080EEh R/W W_CMD_COUNTCNT ---1 [0001]
 48080F0h R/W W_US_COMPARE0 fc-- [FC00] Microsecond compare, bits 0-15
 48080F2h R/W W_US_COMPARE1 ffff [FFFF] Microsecond compare, bits 16-31
 48080F4h R/W W_US_COMPARE2 ffff [FFFF] Microsecond compare, bits 32-47
 48080F6h R/W W_US_COMPARE3 ffff [FFFF] Microsecond compare, bits 48-63
 48080F8h R/W W_US_COUNT0 ffff [0000] Microsecond counter, bits 0-15
 48080FAh R/W W_US_COUNT1 ffff [0000] Microsecond counter, bits 16-31
 48080FCh R/W W_US_COUNT2 ffff [0000] Microsecond counter, bits 32-47
 48080FEh R/W W_US_COUNT3 ffff [0000] Microsecond counter, bits 48-63
 4808100h ? W_INTERNAL 0000 [0000]
 4808102h ? W_INTERNAL 0000 [0000]
 4808104h ? W_INTERNAL 0000 [0000]
 4808106h ? W_INTERNAL 0000 [0000]
 480810Ch R/W W_CONTENTFREE ffff [0000] ...
 4808110h R/W W_PRE_BEACON ffff [0000]
 4808118h R/W W_CMD_COUNT ffff [0000]
 480811Ch R/W W_BEACON_COUNT ffff [0000] reloaded with W_BEACONINT |
| --- |

**Configuration Ports (and some other Registers)**

```

| 4808120h R/W W_CONFIG_120h 81ff [0048] init from firmware[04Ch]
 4808122h R/W W_CONFIG_122h ffff [4840] init from firmware[04Eh]
 4808124h R/W W_CONFIG_124h ffff [0000] init from firmware[05Eh], or 00C8h
 4808126h ? W_INTERNAL fixx [ 0080]
 4808128h R/W W_CONFIG_128h ffff [0000] init from firmware[060h], or 07D0h
 480812Ah ? W_INTERNAL fixx [1000] lower 12bit same as W_CONFIG_128h
 4808130h R/W W_CONFIG_130h -fff [0142] init from firmware[054h]
 4808132h R/W W_CONFIG_132h 8fff [8064] init from firmware[056h]
 4808134h R/W W_POST_BEACON ffff [FFFF] ...
 4808140h R/W W_CONFIG_140h ffff [0000] init from firmware[058h], or xx
 4808142h R/W W_CONFIG_142h ffff [2443] init from firmware[05Ah]
 4808144h R/W W_CONFIG_144h --ff [0042] init from firmware[052h]
 4808146h R/W W_CONFIG_146h --ff [0016] init from firmware[044h]
 4808148h R/W W_CONFIG_148h --ff [0016] init from firmware[046h]
 480814Ah R/W W_CONFIG_14Ah --ff [0016] init from firmware[048h]
 480814Ch R/W W_CONFIG_14Ch ffff [162C] init from firmware[04Ah]
 4808150h R/W W_CONFIG_150h ff3f [0204] init from firmware[062h], or 202h
 4808154h R/W W_CONFIG_154h 7a7f [0058] init from firmware[050h] |
| --- |

**Baseband Chip Ports**

```

| 4808158h W W_BB_CNT mirr [00B5] BB Access Start/Direction/Index
 480815Ah W W_BB_WRITE ???? [0000] BB Access data byte to write
 480815Ch R W_BB_READ 00rr [00B5] BB Access data byte read
 480815Eh R W_BB_BUSY 000r [0000] BB Access Busy flag
 4808160h R/W W_BB_MODE 41-- [0100] BB Access Mode
 4808168h R/W W_BB_POWER 8--f [800D] BB Access Powerdown |
| --- |

**Internal Stuff**

```

| 480816Ah ? W_INTERNAL 0000 [0001] (or 0000h?)
 4808170h ? W_INTERNAL 0000 [0000]
 4808172h ? W_INTERNAL 0000 [0000]
 4808174h ? W_INTERNAL 0000 [0000]
 4808176h ? W_INTERNAL 0000 [0000]
 4808178h W W_INTERNAL fixx [0800] Read: mirror of 17Ch |
| --- |

**RF Chip Ports**

```

| 480817Ch R/W W_RF_DATA2 ffff [0800]
 480817Eh R/W W_RF_DATA1 ffff [C008]
 4808180h R W_RF_BUSY 000r [0000]
 4808184h R/W W_RF_CNT 413f [0018] |
| --- |

**xxx**

```

| 4808190h R/W W_INTERNAL ffff [0000]
 4808194h R/W W_TX_HDR_CNT ---7 [0000] used by firmware part4 (0 or 6)
 4808198h R/W W_INTERNAL ---f [0000]
 480819Ch R W_RF_PINS fixx [0004]
 48081A0h R/W W_X_1A0h -933 [0000] used by firmware part4 (0 or 823h)
 48081A2h R/W W_X_1A2h ---3 [0001] used by firmware part4
 48081A4h R/W W_X_1A4h ffff [0000] "Rate used when signal test..." |
| --- |

**Wifi Statistics**

```

| 48081A8h R W_RXSTAT_INC_IF rrrr [0000] Stats Increment Flags
 48081AAh R/W W_RXSTAT_INC_IE ffff [0000] Stats Increment IRQ Enable
 48081ACh R W_RXSTAT_OVF_IF rrrr [0000] Stats Half-Overflow Flags
 48081AEh R/W W_RXSTAT_OVF_IE ffff [0000] Stats Half-Overflow IRQ Enable
 48081B0h R/W W_RXSTAT --ff [0000]
 48081B2h R/W W_RXSTAT ffff [0000] RX_LengthRateErrorCount
 48081B4h R/W W_RXSTAT rrff [0000] ... firmware uses also MSB ... ?
 48081B6h R/W W_RXSTAT ffff [0000]
 48081B8h R/W W_RXSTAT --ff [0000]
 48081BAh R/W W_RXSTAT --ff [0000]
 48081BCh R/W W_RXSTAT ffff [0000]
 48081BEh R/W W_RXSTAT ffff [0000]
 48081C0h R/W W_TX_ERR_COUNT --ff [0000] TransmitErrorCount
 48081C4h R W_RX_COUNT fixx [0000] |
| --- |

[1D0 - 1DE are 15 entries related to multiplayer response errors]

```

| 48081D0h R/W W_CMD_STAT ff-- [0000]
 48081D2h R/W W_CMD_STAT ffff [0000]
 48081D4h R/W W_CMD_STAT ffff [0000]
 48081D6h R/W W_CMD_STAT ffff [0000]
 48081D8h R/W W_CMD_STAT ffff [0000]
 48081DAh R/W W_CMD_STAT ffff [0000]
 48081DCh R/W W_CMD_STAT ffff [0000]
 48081DEh R/W W_CMD_STAT ffff [0000] |
| --- |

**Internal Diagnostics Registers (usually not used for anything)**

```

| 48081F0h R/W W_INTERNAL ---3 [0000]
 4808204h ? W_INTERNAL fixx [0000]
 4808208h ? W_INTERNAL fixx [0000]
 480820Ch W W_INTERNAL fixx [0050]
 4808210h R W_TX_SEQNO fixx [0000]
 4808214h R W_RF_STATUS XXXX [0009] (used by firmware part4)
 480821Ch W W_IF_SET fbff [0000] Force Interrupt (set bits in W_IF)
 4808220h R/W W_RAM_DISABLE ffff [0000] WifiRAM control
 4808224h R/W W_INTERNAL ---3 [0003]
 4808228h W W_X_228h fixx [0000] (used by firmware part4) (bit3)
 4808230h R/W W_INTERNAL --ff [0047]
 4808234h R/W W_INTERNAL -eff [0EFF]
 4808238h R/W W_INTERNAL ffff [0000] ;rx_seq_no-60h+/-x ;why that?
 ;other day: fixed value, not seq_no related?
 480823Ch ? W_INTERNAL fixx [0000] like W_TXSTAT... ONLY for beacons?
 4808244h R/W W_X_244h ffff [0000] (used by firmware part4)
 4808248h R/W W_INTERNAL ffff [0000]
 480824Ch R W_INTERNAL fixx [0000] ;rx_mac_addr_0 ;\OverTheHedge
 480824Eh R W_INTERNAL fixx [0000] ;rx_mac_addr_1 ;/writes FFFFh?
 4808250h R W_INTERNAL fixx [0000] ;rx_mac_addr_2
 4808254h ? W_CONFIG_254h fixx [0000] (read: FFFFh=DS, EEEEh=DS-Lite)
 4808258h ? W_INTERNAL fixx [0000]
 480825Ch ? W_INTERNAL fixx [0000]
 4808260h ? W_INTERNAL fixx [ 0FEF]
 4808264h R W_INTERNAL fixx [0000] ;rx_addr_1 (usually "rxtx_addr-x")
 4808268h R W_RXTX_ADDR fixx [0005] ;rxtx_addr
 4808270h R W_INTERNAL fixx [0000] ;rx_addr_2 (usually "rx_addr_1-1")
 4808274h ? W_INTERNAL fixx [ 0001]
 4808278h R/W W_INTERNAL ffff [000F]
 480827Ch ? W_INTERNAL fixx [ 000A]
 4808290h (R/W) W_X_290h fixx [FFFF] bit 0 = ? (used by firmware part4)
 4808298h W W_INTERNAL fixx [0000]
 48082A0h R/W W_INTERNAL ffff [0000]
 48082A2h R W_INTERNAL XXXX [7FFF] 15bit shift reg (used during tx?)
 48082A4h R W_INTERNAL fixx [0000] ;rx_rate_1 not ALWAYS same as 2C4h
 48082A8h W W_INTERNAL fixx [0000]
 48082ACh ? W_INTERNAL fixx [ 0038]
 48082B0h W W_INTERNAL fixx [0000]
 48082B4h R/W W_INTERNAL -1-3 [0000]
 48082B8h ? W_INTERNAL fixx [0000] ;dsi launcher checks if zero
 48082C0h R/W W_INTERNAL ---1 [0000]
 48082C4h R W_INTERNAL fixx [000A] ;rx_rate_2 (0Ah,14h = 1,2 Mbit/s)
 48082C8h R W_INTERNAL fixx [0000] ;rx_duration/length/rate (or so?)
 48082CCh R W_INTERNAL fixx [0000] ;rx_framecontrol; from ieee header
 48082D0h DIS W_INTERNAL ;"W_POWERACK" (internal garbage)
 ;normally DISABLED (unless FORCE)
 48082F0h R/W W_INTERNAL ffff [0000]
 48082F2h R/W W_INTERNAL ffff [0000]
 48082F4h R/W W_INTERNAL ffff [0000]
 48082F6h R/W W_INTERNAL ffff [0000] |
| --- |

All other ports in range 4808000h..4808FFFh are unused.

All registers marked as "W_INTERNAL" aren't used by Firmware part4, and are
probably unimportant, except for whatever special diagnostics purposes.

Reading from write-only ports (W) often mirrors to data from other ports.

Additionally, there are 69h Baseband Chip Registers (BB), and 0Fh RF Chip
Registers (see BB and RF chapters).

For Wifi Power Managment (POWCNT2), for Wifi Waitstates (WIFIWAITCNT), see:

DS Power Control

For the Power LED Blink Feature (conventionally used to indicate Wifi activity)
see:

DS Power Management Device

For Wifi Configuration and Calibration data in Firmware Header, see:

DS Cartridges, Encryption, Firmware

**Wifi RAM - NDS7 - Memory (4804000h..4805FFFh)**

```

| 4804000h W_MACMEM RX/TX Buffers (2000h bytes) (excluding below specials)
 4805F60h Used for something, not included in the rx circular buffer.
 4805F80h W_WEPKEY_0 (32 bytes)
 4805FA0h W_WEPKEY_1 (32 bytes)
 4805FC0h W_WEPKEY_2 (32 bytes)
 4805FE0h W_WEPKEY_3 (32 bytes) |
| --- |

Unlike all other NDS memory, Wifi RAM is left uninitialized after boot.

**4805F60h - Reserved 20h-byte RAM area**

Nintendo's software does init/change several values:

```

| [480xxxxh]=5A5Ah/A5A5h ;-initial dummy WifiRAM memfill values
 [4805F70h]=FFFFh ;\
 [4805F72h]=FFFFh ; set to FFFFh by software
 [4805F76h]=FFFFh ;
 [4805F7Eh]=FFFFh ;/ |
| --- |

The hardware does update several values (unless disabled in
W_RAM_DISABLE.bit5):

```

| [4805F6Eh]=0F00h (nothing received), or 0F01h (received something) |
| --- |

And, if received packet was accepted (eg. when W_RXFILTER.bit0=1=AcceptAll):

```

| [4805F70h]=Received MAC Address (6 bytes, looks same as port 480824Ch)
 [4805F76h]=xxx0h (increasing value, Sequence Control from packet header) |
| --- |

And, in multiplay slave mode (when receiving CMD packets?):

```

| [4805F7Eh]=xxx0h (next higher sequence number? ie. [4805F76h]+10h) |
| --- |

**4805F80h - W_WEPKEY_0 thru W_WEPKEY_3 - Wifi WEP keys (R/W)**

These WEP key slots store the WEP keys that are used for encryption for 802.11
keys IDs 0-3.

| DS Wifi Control |
| --- |

**4808000h - W_ID - Wifi Chip ID (R)**

```

| 0-15 Chip ID (1440h on NDS, C340h on NDS-lite) |
| --- |

The NDS-lite is more or less backwards compatible with the original NDS (the
W_RXBUF_GAPDISP and W_TXBUF_GAPDISP are different, and most of the garbage
effects on unused/mirrored ports are different, too).

**4808004h - W_MODE_RST - Wifi Hardware mode / reset (R/W)**

```

| 0 Adjust some ports (0/1=see lists below) (R/W)
 TX Master Enable for LOC1..3 and Beacon (0=Disable, 1=Enable)
 1-12 Unknown (R/W)
 13 Reset some ports (0=No change, 1=Reset/see list below) (Write-Only)
 14 Reset some ports (0=No change, 1=Reset/see list below) (Write-Only)
 15 Unknown (R/W) |
| --- |

**4808006h - W_MODE_WEP - Wifi Software mode / Wep mode (R/W)**

```

| 0-2 Unknown, specify a software mode for wifi operation
 (may be related to hardware but a correlation has not yet been found)
 3-5 WEP Encryption Key Size:
 0=Reserved (acts same as 1)
 1=64bit WEP (IV=24bit + KEY=40bit) (aka 3+5 bytes) ;standard/us
 2=128bit WEP (IV=24bit + KEY=104bit) (aka 3+13 bytes) ;standard/world
 3=152bit WEP (IV=24bit + KEY=128bit) (aka 3+16 bytes) ;uncommon
 4=Unknown, mabye 256bit WEP (IV=24bit + KEY=232bit) (aka 3+29 bytes)?
 5=Reserved (acts same as 1)
 6=Reserved (acts same as 1)
 7=Reserved (acts same as 1)
 6 Unknown
 7-15 Always zero |
| --- |

**4808018h - W_MACADDR_0 - MAC Address (R/W)**

**480801Ah - W_MACADDR_1 - MAC Address (R/W)**

**480801Ch - W_MACADDR_2 - MAC Address (R/W)**

48bit MAC Address of the console. Should be initialized from firmware[036h].
The hardware receives only packets that are sent to this address (or to group
addresses, like FF:FF:FF:FF:FF:FF).

**4808020h - W_BSSID_0 - BSSID (R/W)**

**4808022h - W_BSSID_1 - BSSID (R/W)**

**4808024h - W_BSSID_2 - BSSID (R/W)**

48bit BSSID stored here. Ie. the MAC address of the host, obtained from Beacon
frames (on the host itself, that should be just same as W_MACADDR). See
W_RXFILTER.

**4808028h - W_AID_LOW (R/W)**

```

| Bit0-3 Multiplay Slave number (1..15, or 0)
 Bit4-15 Not used |
| --- |

Usually set to zero, or equal to the lower 4bit of the W_AID_FULL value.

**480802Ah - W_AID_FULL - Association ID (R/W)**

```

| Bit0-10 Association ID (AID) (1..2007, or zero)
 Bit11-15 Not used |
| --- |

**4808032h - W_WEP_CNT - WEP Engine Enable (R/W)**

```

| 0-14 Unknown (usually zero)
 15 WEP Engine Enable (0=Disable, 1=Enable) |
| --- |

Normally, bit15 should be always set (but it will only affect WEP-encrypted
packets, ie. packets with Frame Control bit14=1 in 802.11 header). When
disabled, WEP packets aren't received at all (neither in encrypted nor
decrypted form), and sending WEP packets might be also ignored(?).

The firmware contains some code that does toggle the bit off for a moment
(apparently to reset something after transfer errors).

**4808044h - W_RANDOM - Random Generator (R)**

```

| 0-10 Random
 11-15 Not used (zero) |
| --- |

The random generator is updated at 33.51MHz rate, as such:

```

| X = (X AND 1) XOR (X ROL 1) ;(rotation within 11bit range) |
| --- |

That random sequence goes through 5FDh different values before it restarts.

When reading from the random register, the old latched value is returned to the
CPU, and the new current random value is then latched, so reads always return
the older value, timed from the previous read.

Occassionally, about once every some thousand reads, the latching appears to
occur 4 cycles earlier than normally, so the value on the next read will be 4
cycles older than expected.

The random register has ACTIVE mirrors.

**48080BCh - W_PREAMBLE - Preamble Control (R/W)**

```

| Bit Dir Expl.
 0 R/W Unknown (this does NOT affect TX)
 1 R/W Preamble (0=Long, 1=Short) (this does NOT affect TX)
 2 W Preamble (0=Long, 1=Short) (this does affect TX) (only at 2Mbit/s)
 3-15 - Always zero |
| --- |

Short preamble works only with 2Mbit/s transfer rate (ie. when set as so in TX
hardware header). 1Mbit/s rate always uses long preamble.

```

| Type Carrier Signal SFD Value PLCP Header Data
 Long 128bit, 1Mbit 16bit, 1Mbit 48bit, 1Mbit N bits, 1Mbit or 2Mbit
 Short 56bit, 1Mbit 16bit, 1Mbit 48bit, 2Mbit N bits, 2Mbit |
| --- |

Length of the Carrier+SFD+PLCP part is thus 192us (long) or 96us (short).

Note: The Carrier+SFD+PLCP part is sent between IRQ14 and IRQ07 (not between
IRQ07 and IRQ01).

Firmware also changes W_CONFIG_140h alongsides with the preamble setting.

**Writing "0-then-1" to W_MODE_RST.Bit0 does reset following ports:**

```

| [4808034h]=0002h ;W_INTERNAL
 [480819Ch]=0046h ;W_RF_PINS
 [4808214h]=0009h ;W_RF_STATUS
 [480827Ch]=0005h ;W_INTERNAL
 [48082A2h]=? ;...unstable? |
| --- |

**Writing "1-then-0" to W_MODE_RST.Bit0 does reset following ports:**

```

| [480827Ch]=000Ah ;W_INTERNAL |
| --- |

**Writing "1" to W_MODE_RST.Bit13 does reset following ports:**

```

| [4808056h]=0000h ;W_RXBUF_WR_ADDR
 [48080C0h]=0000h ;W_CMD_TOTALTIME
 [48080C4h]=0000h ;W_CMD_REPLYTIME
 [48081A4h]=0000h ;W_X_1A4h
 [4808278h]=000Fh ;W_INTERNAL
 ...Also, following may be affected (results are unstable though)...
 [48080AEh]=? ;or rather the actual port (which it is an mirror of)
 [48080BAh]=? ;W_INTERNAL (occassionally unstable)
 [4808204h]=? ;W_INTERNAL
 [480825Ch]=? ;W_INTERNAL
 [4808268h]=? ;W_RXTX_ADDR
 [4808274h]=? ;W_INTERNAL |
| --- |

**Writing "1" to W_MODE_RST.Bit14 does reset following ports:**

```

| [4808006h]=0000h ;W_MODE_WEP
 [4808008h]=0000h ;W_TXSTATCNT
 [480800Ah]=0000h ;W_X_00Ah
 [4808018h]=0000h ;W_MACADDR_0
 [480801Ah]=0000h ;W_MACADDR_1
 [480801Ch]=0000h ;W_MACADDR_2
 [4808020h]=0000h ;W_BSSID_0
 [4808022h]=0000h ;W_BSSID_1
 [4808024h]=0000h ;W_BSSID_2
 [4808028h]=0000h ;W_AID_LOW
 [480802Ah]=0000h ;W_AID_FULL
 [480802Ch]=0707h ;W_TX_RETRYLIMIT
 [480802Eh]=0000h ;W_INTERNAL
 [4808050h]=4000h ;W_RXBUF_BEGIN
 [4808052h]=4800h ;W_RXBUF_END
 [4808084h]=0000h ;W_TXBUF_TIM
 [48080BCh]=0001h ;W_PREAMBLE
 [48080D0h]=0401h ;W_RXFILTER
 [48080D4h]=0001h ;W_CONFIG_0D4h
 [48080E0h]=0008h ;W_RXFILTER2
 [48080ECh]=3F03h ;W_CONFIG_0ECh
 [4808194h]=0000h ;W_TX_HDR_CNT
 [4808198h]=0000h ;W_INTERNAL
 [48081A2h]=0001h ;W_X_1A2h
 [4808224h]=0003h ;W_INTERNAL
 [4808230h]=0047h ;W_INTERNAL |
| --- |

| DS Wifi Interrupts |
| --- |

**4808010h - W_IF - Wifi Interrupt Request Flags (R/W)**

```

| 0 Receive Complete (packet received and stored in the RX fifo)
 1 Transmit Complete (packet is done being transmitted) (no matter if error)
 2 Receive Event Increment (IRQ02, see W_RXSTAT_INC_IE)
 3 Transmit Error Increment (IRQ03, see W_TX_ERR_COUNT)
 4 Receive Event Half-Overflow (IRQ04, see W_RXSTAT_OVF_IE)
 5 Transmit Error Half-Overflow (IRQ05, see W_TX_ERR_COUNT.Bit7)
 6 Start Receive (IRQ06, a packet has just started to be received)
 7 Start Transmit (IRQ07, a packet has just started to be transmitted)
 8 Txbuf Count Expired (IRQ08, see W_TXBUF_COUNT)
 9 Rxbuf Count Expired (IRQ09, see W_RXBUF_COUNT)
 10 Not used (always zero, even when trying to set it with W_IF_SET)
 11 RF Wakeup (IRQ11, see W_POWERSTATE)
 12 Multiplay CMD done (or failed) (IRQ12, see W_CMD_COUNT)
 13 Post-Beacon Timeslot (IRQ13, see W_POST_BEACON)
 14 Beacon Timeslot (IRQ14, see W_BEACON_COUNT/W_US_COMPARE)
 15 Pre-Beacon Timeslot (IRQ15, see W_BEACON_COUNT/W_PRE_BEACON) |
| --- |

Write a '1' to a bit to clear it. For the Half-Overflow flags that works ONLY
if the counter MSBs are zero (ie. one must first read the counters (to reset
them), and THEN acknowledge the corresponding W_IF bit).

The Transmit Start/Complete bits (Bit7,1) are set for EACH packet (including
beacons, and including retries).

**4808012h - W_IE - Wifi Interrupt Enable Flags (R/W)**

```

| 0-15 Enable Flags, same bits as W_IF (0=Disable, 1=Enable) |
| --- |

In W_IE, Bit10 is R/W, but seems to have no function since IRQ10 doesn't exist.

**480821Ch - W_IF_SET (W_INTERNAL) - Force Wifi Interrupt Flags (W)**

```

| 0-15 Set corresponding bits in W_IF (0=No change, 1=Set Bit) |
| --- |

Notes: Bit10 cannot be set since no IRQ10 exists. This register does only set
IRQ flags, but without performing special actions (such like W_BEACON_COUNT and
W_POST_BEACON reloads that occur on real IRQ14's).

**Wifi Primary IRQ Flag (IF.Bit24, Port 4000214h)**

IF.Bit24 gets set <only> when (W_IF AND W_IE) changes from 0000h to
non-zero.

IF.Bit24 can be reset (ack) <even> when (W_IF AND W_IE) is still
non-zero.

```
****
| Caution Caution Caution Caution Caution
 That means, when acknowledging IF.Bit24, then NO FURTHER wifi IRQs
 will be executed whilst and as long as (W_IF AND W_IE) is non-zero. |
| --- |

One work-around is to process/acknowledge ALL wifi IRQs in a loop, including
further IRQs that may occur inside of that loop, until (W_IF AND W_IE) becomes
0000h.

Another work-around (for single IRQs) would be to acknowledge IF and W_IF, and
then to set W_IE temporarily to 0000h, and then back to the old W_IE setting.

| DS Wifi Power-Down Registers |
| --- |

**4808036h - W_POWER_US (R/W)**

```

| 0 Disable W_US_COUNT and W_BB_ports (0=Enable, 1=Disable)
 1 Unknown (usually 0)
 2-15 Always zero |
| --- |

Bit0=0 enables RFU by setting RFU.Pin11=HIGH, which activates the 22.000MHz
oscillator on the RFU board, the 22MHz clock is then output to RFU.Pin26.

**4808038h - W_POWER_TX (R/W)**

transmit-related power save or sth

init from firmware[05Ch]

```

| 0 Auto Wakeup (1=Leave Idle Mode a while after Pre-Beacon IRQ15)
 1 Auto Sleep (0=Enter Idle Mode on Post-Beacon IRQ13)
 2 Unknown
 3 Unknown (Write-only) (used by firmware)
 4-15 Always zero |
| --- |

**480803Ch - W_POWERSTATE (R/W)/(R)**

```

| 0 Unknown (usually 0) (R/W)
 1 Request Power Enable (0=No, 1=Yes/queued) (R/W, but not always)
 2-7 Always zero
 8 Indicates that Bit9 is about the be cleared (Read only)
 9 Current power state (0=Enabled, 1=Disabled) (Read only)
 10-15 Always zero |
| --- |

[value =1: queue disable power state] ;<-- seems to be incorrect

[value =2: queue enable power state] ;<-- seems to be correct

Enabling causes wakeup interrupt (IRQ11).

Note: That queue stuff seems to work only if W_POWER_US=0 and W_MODE_RST=1.

**4808040h - W_POWERFORCE - Force Power State (R/W)**

```

| 0 New value for W_POWERSTATE.Bit9 (0=Clear/Delayed, 1=Set/Immediately)
 1-14 Always zero
 15 Apply Bit0 to W_POWERSTATE.Bit9 (0=No, 1=Yes) |
| --- |

Setting W_POWERFORCE=8001h whilst W_POWERSTATE.Bit9=0 acts immediately:

```

| (Doing this is okay. Switches to power down mode. Similar to IRQ13.)
 [4808034h]=0002h ;W_INTERNAL
 [480803Ch]=02xxh ;W_POWERSTATE
 [48080B0h]=0000h ;W_TXREQ_READ
 [480819Ch]=0046h ;W_RF_PINS
 [4808214h]=0009h ;W_RF_STATUS (idle) |
| --- |

Setting W_POWERFORCE=8000h whilst W_POWERSTATE.Bit9=1 acts delayed:

```

| (Don't do this. After that sequence, the hardware seems to be messed up)
 W_POWERSTATE.Bit8 gets set to indicate the pending operation,
 while pending, changes to W_POWERFORCE aren't applied to W_POWERSTATE,
 while pending, W_POWERACK becomes Read/Write-able,
 writing 0000h to W_POWERACK does clear W_POWERSTATE.Bit8,
 and does apply POWERFORCE.Bit0 to W_POWERSTATE.Bit9
 and does deactivate Port W_POWERACK again. |
| --- |

**4808048h - W_POWER_? (R/W)**

```

| 0 Unknown
 1 Unknown
 2-15 Always zero |
| --- |

At whatever time (during transmit or so) it gets set to 0003h by hardware.

See also: POWCNT2, W_BB_POWER.

| DS Wifi Receive Control |
| --- |

**4808030h - W_RXCNT - Wifi Receive Control (parts R/W and W)**

```

| 0 Copy W_RXBUF_WR_ADDR to W_RXBUF_WRCSR (aka force RX buf empty) (W)
 1-3 Unknown (R/W)
 4-6 Always zero
 7 Copy W_TXBUF_REPLY1 to W_TXBUF_REPLY2, set W_TXBUF_REPLY1 to 0000h (W)
 8-14 Unknown (R/W)
 15 Enable Queuing received data to RX FIFO (R/W) |
| --- |

**48080D0h - W_RXFILTER - (R/W)**

```

| 0 For Broadcasts? (0=Insist on W_BSSID, 1=Accept no matter of W_BSSID)
 1 Unknown (usually zero)
 2 Unknown (usually zero)
 3 Unknown (usually zero)
 4 Unknown (usually zero)
 5 Unknown (usually zero)
 6 Unknown (usually zero)
 7 Unknown (0 or 1)
 8 Empty Packets (0=Ignore, 1=Accept; with RXHDR[0]=801Fh)
 9 Unknown (0 or 1)
 10 Unknown (0 or 1) (when set, receives beacons, and maybe others)
 11 Unknown (usually zero) ;reportedly "allow toDS" ?
 12 Update W_RXBUF_WRCSR after IEEE header (instead after full packets?)
 (setting bit12 causes a mess, where new "packets" in RX buf could
 either contain RXHDR+IEEE header, or Data corresponding to that
 headers, which could be useful only if there's a way to distinguish
 between headers and data, and knowing the size of the data blocks).
 13-15 Not used (always zero) |
| --- |

Specifies what packets to allow. Some random notes...

```

| 0000h = Disable receive.
 FFFFh = Enable receive.
 0400h = Receives managment frames (and possibly others, too) |
| --- |

The exact meaning of the bits is unknown. The most import part is the address
filtering based on the DA and BSSID values in IEEE header:

```

| DA=W_MACADDR is always received
 DA=Broadcast, and BSSID=W_BSSID is always received
 DA=Broadcast, and BSSID=other is received only if RXFILTER.bit0=1 |
| --- |

Broadcast addresses (aka group addresses) have DA.firstbyte.bit0=1.
RXFILTER.bit0 should be set when searching beacons, and cleared after
connecting to an access point.

**48080E0h - W_RXFILTER2 - (R/W)**

```

| 0 Unknown (0=Receive Data Frames, 1=Ignore Data Frames) (?)
 1 Unknown
 2 Unknown
 3 Unknown (usually set)
 4-15 Not used (always zero) |
| --- |

Firmware writes values 08h, 0Bh, 0Dh (aka 1000b, 1011b, 1101b).

Firmware usually has bit0 set, even when receiving data frames, so, in some
situations data frames seem to pass-through even when bit0 is set...? Possibly
that situation is when W_BSSID matches...?

Control/PS-Poll frames seem to be passed always (even if W_RXFILTER2=0Fh).

| DS Wifi Receive Buffer |
| --- |

The dimensions of the circular Buffer are set with BEGIN/END values, hardware
automatically wraps to BEGIN when an incremented pointer hits END address.

**Write Area**

Memory between WRCSR and READCSR is free for receiving data, the hardware
writes incoming packets to this region (to WRCSR and up) (but without exceeding
READCSR), once when it has successfully received a complete packet, the
hardware moves WRCSR after the packet (aligned to a 4-byte boundary).

**Read Area**

Memory between READCSR and WRCSR contains received data, which can be read by
the CPU via RD_ADDR and RD_DATA registers (or directly from memory). Once when
having processed that data, the CPU must set READCSR to the end of it.

**4808050h - W_RXBUF_BEGIN - Wifi RX Fifo start location (R/W)**

**4808052h - W_RXBUF_END - Wifi RX Fifo end location (R/W)**

```

| 0-15 Byte-offset in Wifi Memory (usually 4000h..5FFEh) |
| --- |

Although the full 16bit are R/W, only the 12bit halfword offset in Bit1-12 is
actually used, the other bits seem to have no effect.

Some or all (?) of the below incrementing registers are automatically matched
to begin/end, that is, after incrementing, IF adr=end THEN adr=begin.

**4808054h - W_RXBUF_WRCSR - Wifi RX Fifo Write or "end" cursor (R)**

```

| 0-11 Halfword Address in RAM
 12-15 Always zero |
| --- |

Hardware does set this register pointing to end of the most recently received
packet (plus 32bit alignment padding). Hardware will write the next packet to
that address. And more importantly, software can read from RX fifo up to that
address.

**4808056h - W_RXBUF_WR_ADDR - Wifi RX Fifo Write Cursor Latch value (R/W)**

```

| 0-11 Halfword Address in RAM
 12-15 Always zero |
| --- |

This is a value that is latched into W_RXBUF_WRCSR, when the W_RXCNT latch bit
(W_RXCNT.Bit0) is written.

**4808058h - W_RXBUF_RD_ADDR - Wifi CircBuf Read Address (R/W)**

```

| 0 Always zero
 1-12 Halfword Address in RAM for reading via W_RXBUF_RD_DATA
 13-15 Always zero |
| --- |

The circular buffer limits are the same as the range specified for the receive
FIFO, however the address can be set outside of that range and will only be
affected by the FIFO boundary if it crosses the FIFO end location by reading
from the circular buffer.

**480805Ah - W_RXBUF_READCSR - Wifi RX Fifo Read or "start" cursor (R/W)**

```

| 0-11 Halfword Address in RAM
 12-15 Always zero |
| --- |

This value is specified the same as W_RXBUF_WRCSR - it's purely software
controlled so it's up to the programmer to move the start cursor after loading
a packet. If W_RXBUF_READCSR != W_RXBUF_WRCSR, then one or more packets exist
in the FIFO that need to be processed (see the section on HW RX Headers, for
information on calculating packet lengths). Once a packet has been processed,
the software should advance the read cursor to the beginning of the next
packet.

**4808060h - W_RXBUF_RD_DATA - Wifi CircBuf Read Data (R)**

```

| 0-15 Data |
| --- |

returns the 16bit value at the address specified by W_RXBUF_RD_ADDR, and
increments W_RXBUF_RD_ADDR by 2. If the increment causes W_RXBUF_RD_ADDR to
equal the address specified in W_RXBUF_END, W_RXBUF_RD_ADDR will be reset to
the address specified in W_RXBUF_BEGIN.

Ports 1060h, 6060h, 7060h are PASSIVE mirrors of 0060h, reading from these
mirrors returns the old latched value from previous read from 0060h, but
without reading a new value from RAM, and without incrementing the address.

**4808062h - W_RXBUF_GAP - Wifi RX Gap Address (R/W)**

```

| 0 Always zero
 1-12 Halfword Address in RAM
 13-15 Always zero |
| --- |

Seems to be intended to define a "gap" in the circular buffer, done like so:

```

| Addr=Addr+2 and 1FFEh ;address increment (by W_RXBUF_RD_DATA read)
 if Addr=RXBUF_END then ;normal begin/end wrapping (done before gap wraps)
 Addr=RXBUF_BEGIN
 if Addr=RXBUF_GAP then ;now gap-wrap (may include further begin/end wrap)
 Addr=RXBUF_GAP+RXBUF_GAPDISP*2
 if Addr>=RXBUF_END then Addr=Addr+RXBUF_BEGIN-RXBUF_END ;wrap more |
| --- |

To disable the gap stuff, set both W_RXBUF_GAP and W_RXBUF_GAPDISP to zero.

**4808064h - W_RXBUF_GAPDISP - Wifi RX Gap Displacement Offset (R/W)**

```

| 0-11 Halfword Offset, used with W_RXBUF_GAP (see there)
 12-15 Always zero |
| --- |

Caution: On the DS-Lite, after adding it to W_RXBUF_RD_ADDR, the
W_RXBUF_GAPDISP setting is destroyed (reset to 0000h) by hardware. The original
DS leaves W_RXBUF_GAPDISP intact.

**480805Ch - W_RXBUF_COUNT (R/W)**

```

| 0-11 Decremented on reads from W_RXBUF_RD_DATA
 12-15 Always zero |
| --- |

Triggers IRQ09 when it reaches zero, and does then stay at zero (without
further decrementing, and without generating further IRQs).

Note: Also decremented on (accidental) writes to read-only W_RXBUF_RD_DATA.

| DS Wifi Receive Statistics |
| --- |

**48081A8h - W_RXSTAT_INC_IF - Statistics Increment Flags (R)**

```

| 0-12 Increment Flags (see Port 48081B0h..1BFh)
 13-15 Always zero |
| --- |

Bitmask for which statistics have been increased at least once.

Unknown how to reset/acknowledge these bits (neither reading/writing 48081A8h,
nor reading 48081B0h..1BFh, nor writing 48081ACh seem to work).

**48081AAh - W_RXSTAT_INC_IE - Statistics Increment Interrupt Enable (R/W)**

```

| 0-12 Counter Increment Interrupt Enable (see 48081B0h..1BFh) (1=Enable)
 13-15 Unknown (usually zero) |
| --- |

Statistic Interrupt Enable Control register for Count Up.

Note: ------> seems to trigger IRQ02 ...?

**48081ACh - W_RXSTAT_OVF_IF - Statistics Half-Overflow Flags (R)**

```

| 0-12 Half-Overflow Flags (see Port 48081B0h..1BFh)
 13-15 Always zero |
| --- |

The W_RXSTAT_OVF_IF bits are simply containing the current bit7-value of the
corresponding counters, setting or clearing that counter bits is directly
reflected to W_RXSTAT_OVF_IF.

The recommended way to acknowledge W_RXSTAT_OVF_IF is to read the corresponding
counters (which are reset to 00h after reading). For some reason, the firmware
is additionally writing FFFFh to W_RXSTAT_OVF_IF (that is possibly a bug, or it
does acknowledge something internally?).

**48081AEh - W_RXSTAT_OVF_IE - Statistics Half-Overflow Interrupt Enable (R/W)**

```

| 0-12 Half-Overflow Interrupt Enable (see Port 48081B0h..1BFh) (1=Enable)
 13-15 Unknown (usually zero) |
| --- |

Statistic Interrupt Enable for Overflow, bits same as in W_RXSTAT_INC_IE

Note: ------> seems to trigger IRQ04 ...?

**48081B0h..1BFh - W_RXSTAT - Receive Statistics (R/W, except 1B5h: Read-only)**

W_RXSTAT is a collection of 8bit counters, which are incremented upon certain
events. These entries are automatically reset to 0000h after reading. Should be
accessed with LDRH opcodes (using LDRB to read only 8bits does work, but the
read is internally expanded to 16bit, and so, the whole 16bit value will be
reset to 0000h).

```

| Port Dir Bit Expl.
 48081B0h R/W 0 W_RXSTAT ?
 48081B1h - - Always 0 -
 48081B2h R/W 1 W_RXSTAT ? "RX_RateErrorCount"
 48081B3h R/W 2 W_RXSTAT Length>2348 error
 48081B4h R/W 3 W_RXSTAT RXBUF Full error
 48081B5h R 4? W_RXSTAT ? (R) (but seems to exist; used by firmware)
 48081B6h R/W 5 W_RXSTAT Length=0 or Wrong FCS Error
 48081B7h R/W 6 W_RXSTAT Packet Received Okay
 (also increments on W_MACADDR mis-match)
 (also increments on internal ACK packets)
 (also increments on invalid IEEE type=3)
 (also increments TOGETHER with 1BCh and 1BEh)
 (not incremented on RXBUF_FULL error)
 48081B8h R/W 7 W_RXSTAT ?
 48081B9h - - Always 0 -
 48081BAh R/W 8 W_RXSTAT ?
 48081BBh - - Always 0 -
 48081BCh R/W 9 W_RXSTAT WEP Error (when FC.Bit14 is set)
 48081BDh R/W 10 W_RXSTAT ?
 48081BEh R/W 11 W_RXSTAT (duplicated sequence control)
 48081BFh R/W 12 W_RXSTAT ? |
| --- |

**48081C4h - W_RX_COUNT (W_INTERNAL) (R)**

```

| 0-? Receive Okay Count (increments together with ports 48081B4h, 48081B7h)
 8-? Receive Error Count (increments together with ports 48081B3h, 48081B6h) |
| --- |

Increments when receiving a packet. Automatically reset to zero after reading.

**48081D0h..1DFh - W_CMD_STAT - Multiplay Response Error Counters (R/W)**

The multiplay error counters are only used when sending a multiplay command
(via W_TXBUF_CMD) to any connected slaves (which must be indicated by flags
located in the second halfword of the multiplay command's frame body).

```

| 48081D0h Not used (always zero)
 48081D1h..1DFh Client 1..15 Response Error (increments on missing replies) |
| --- |

If one or more of those slaves fail to respond, then the corresponding error
counters get incremented (at the master side). Automatically reset to zero
after reading.

Unknown if these counters do also increment at the slave side?

| DS Wifi Transmit Control |
| --- |

**48080ACh - W_TXREQ_RESET - Reset Transfer Request Bits (W)**

```

| 0-3 Reset corresponding bits in W_TXREQ_READ (0=No change, 1=Reset)
 4-15 Unknown (if any) |
| --- |

Firmware writes values 01h,02h,08h,0Dh, and FFFFh.

**48080AEh - W_TXREQ_SET - Set Transfer Request Bits (W)**

```

| 0-3 Set corresponding bits in W_TXREQ_READ (0=No change, 1=Set)
 4-15 Unknown (if any) |
| --- |

Firmware writes values 01h,02h,05h,08h,0Dh.

**48080B0h - W_TXREQ_READ - Get Transfer Request Bits (R)**

```

| 0 Send W_TXBUF_LOC1 (1=Transfer, if enabled in W_TXBUF_LOC1.Bit15)
 1 Send W_TXBUF_CMD (1=Transfer, if enabled in W_TXBUF_CMD.Bit15)
 2 Send W_TXBUF_LOC2 (1=Transfer, if enabled in W_TXBUF_LOC2.Bit15)
 3 Send W_TXBUF_LOC3 (1=Transfer, if enabled in W_TXBUF_LOC3.Bit15)
 4 Unknown (Beacon?) (always 1, except when cleared via W_POWERFORCE)
 5-15 Unknown/Not used |
| --- |

Bit0-3 can be set/reset via W_TXREQ_SET/W_TXREQ_RESET. The setting in
W_TXREQ_READ remains intact even after the transfer(s) have completed.

If more than one of the LOC1,2,3 bits is set, then LOC3 is transferred first,
LOC1 last.

Beacons are transferred in every Beacon Timeslot (if enabled in
W_TXBUF_BEACON.Bit15).

Bit0,2,3 are automatically reset upon IRQ14 (by hardware).

**48080B6h - W_TXBUSY (R)**

```

| 0 W_TXBUF_LOC1 (1=Requested Transfer busy, or not yet started at all)
 1 W_TXBUF_CMD (1=Requested Transfer busy, or not yet started at all)
 2 W_TXBUF_LOC2 (1=Requested Transfer busy, or not yet started at all)
 3 W_TXBUF_LOC3 (1=Requested Transfer busy, or not yet started at all)
 4 W_TXBUF_BEACON (1=Beacon Transfer busy)
 5-15 Unknown (if any) |
| --- |

Busy bits. If all three W_TXBUF_LOC's are sent, then it goes through values
0Dh,05h,01h,00h; ie. LOC3 is transferred first, LOC1 last. The register is
updated upon IRQ01 (by hardware).

Bit4 is set only in Beacon Timeslots.

**48080B8h - W_TXSTAT - RESULT - Status of transmitted frame (R)**

For LOC1-3, this register is updated at the end of a transfer (upon the IRQ01
request), if retries occur then it is updated only after the final retry.

For BEACON, this register is updated only if enabled in W_TXSTATCNT.Bit15, and
only after successful transfers (since beacon errors result in infinite
retries).

For CMD, this register is updated only if enabled in W_TXSTATCNT.Bit13,14).

Bit0/1 act similar to W_IF Bit1/3, however, the W_IF Bits are set after each
transmit (including retries).

```

| 0 One (or more) Packet has Completed (1=Yes)
 (No matter if successful, for that info see Bit1)
 (No matter if ALL packets are done, for that info see Bit12-13)
 1 Packet Failed (1=Error)
 2-7 Unknown/Not used
 8-11 Usually 0, ...but firmware is checking for values 03h,08h,0Bh
 (gets set to 07h when transferred W_TXBUF_LOC1/2/3 did have Bit12=set)
 (gets set to 00h otherwise)
 (gets set to 03h after beacons ;if enabled in W_TXSTATCNT.Bit15)
 (gets set to 08h after cmd's ;if enabled in W_TXSTATCNT.Bit14)
 (gets set to 0Bh after cmd ack's ;if enabled in W_TXSTATCNT.Bit13)
 (gets set to 04h after reply's ;if enabled in W_TXSTATCNT.Bit12)
 12-13 Packet that updated W_TXSTAT (0=LOC1/BEACON/CMD/REPLY, 1=LOC2, 2=LOC3)
 14-15 Unknown/Not used |
| --- |

No idea how to reset bit0/1 once when they are set?

**4808008h - W_TXSTATCNT (R/W)**

```

| 0-11 Unknown (usually zero) (otherwise disables RXing multiplay REPLY's?)
 12 Update W_TXSTAT=0401h and trigger IRQ01 after REPLY transmits (1=Yes)
 13 Update W_TXSTAT=0B01h and trigger IRQ01 after CMD ACK transmits (1=Yes)
 14 Update W_TXSTAT=0800h and trigger IRQ01 after CMD DATA transmits(1=Yes)
 15 Update W_TXSTAT=0301h and trigger IRQ01 after BEACON transmits (1=Yes) |
| --- |

Note: LOC1..3 transmits are always updating W_TXSTAT and triggering IRQ01.

**4808194h - W_TX_HDR_CNT - Disable Transmit Header Adjustments (R/W)**

```

| 0 IEEE FC.Bit12 and Duration (0=Auto/whatever, 1=Manual/Wifi RAM)
 1 IEEE Frame Check Sequence (0=Auto/FCS/CRC32, 1=Manual/Wifi RAM)
 2 IEEE Sequence Control (0=Auto/W_TX_SEQNO, 1=Manual/Wifi RAM)
 3-15 Always zero |
| --- |

Allows to disable automatic adjustments of the IEEE header and checksum.

Note: W_TX_SEQNO can be also disabled by W_TXBUF_LOCn.Bit13 and by TXHDR[04h].

**4808210h - W_TX_SEQNO - Transmit Sequence Number (R)**

```

| 0-11 Increments on IRQ07 (Transmit Start Interrupt)
 12-15 Always zero |
| --- |

Also incremented shortly after IRQ12.

When enabled in W_TXBUF_LOCn.Bit13, this value replaces the upper 12bit of the
IEEE Frame Header's Sequence Control value (otherwise, when disabled, the
original value in Wifi RAM is used, and, in that case, W_TX_SEQNO is NOT
incremented).

Aside from W_TXBUF_LOCn.Bit13, other ways to disable W_TX_SEQNO are: Transmit
Hardware Header entry TXHDR[04h], and W_TX_HDR_CNT.Bit2.

| DS Wifi Transmit Buffers |
| --- |

**4808068h - W_TXBUF_WR_ADDR - Wifi CircBuf Write Address (R/W)**

```

| 0 Always zero
 1-12 Halfword Address in RAM for Writes via W_TXBUF_WR_DATA
 13-15 Always zero |
| --- |

**4808070h - W_TXBUF_WR_DATA - Wifi CircBuf Write Data (W)**

```

| 0-15 Data to be written to address specified in W_TXBUF_WR_ADDR |
| --- |

After writing to this register, W_TXBUF_WR_ADDR is automatically incremented by
2, and, if it gets equal to W_TXBUF_GAP, then it gets additonally incremented
by W_TXBUF_GAPDISP*2.

**4808074h - W_TXBUF_GAP - Wifi CircBuf Write Top (R/W)**

```

| 0 Always zero
 1-12 Halfword Address
 13-15 Always zero |
| --- |

**4808076h - W_TXBUF_GAPDISP - CircBuf Write Offset from Top to Bottom (R/W)**

```

| 0-11 Halfword Offset (added to; if equal to W_TXBUF_GAP)
 12-15 Always zero |
| --- |

Should be "0-write_buffer_size" (wrap from end to begin), or zero (no
wrapping).

Caution: On the DS-Lite, after adding it to W_TXBUF_WR_ADDR, the
W_TXBUF_GAPDISP setting is destroyed (reset to 0000h) by hardware. The original
DS leaves W_TXBUF_GAPDISP intact.

Note: W_TXBUF_GAP and W_TXBUF_GAPDISP may be (not TOO probably) also used by
transmits via W_TXBUF_LOCn and W_TXBUF_BEACON (not tested).

**4808080h - W_TXBUF_BEACON - Beacon Transmit Location (R/W)**

**4808090h - W_TXBUF_CMD - Multiplay Command Transmit Location (R/W)**

**48080A0h - W_TXBUF_LOC1 - Transmit location 1 (R/W)**

**48080A4h - W_TXBUF_LOC2 - Transmit location 2 (R/W)**

**48080A8h - W_TXBUF_LOC3 - Transmit location 3 (R/W)**

```

| 0-11 Halfword Address of TX Frame Header in RAM
 12 For LOC1-3: When set, W_TXSTAT.bit8-10 are set to 07h after transfer
 And, when set, the transferred frame-body gets messed up?
 For BEACON: Unknown, no effect on W_TXSTAT
 For CMD: Unknown, no effect on W_TXSTAT
 13 IEEE Sequence Control (0=From W_TX_SEQNO, 1=Value in Wifi RAM)
 For BEACON: Unknown (always uses W_TX_SEQNO) (no matter of bit13)
 14 Unknown
 15 Transfer Request (1=Request/Pending) |
| --- |

For LOC1..3 and CMD, Bit15 is automatically cleared after (or rather: during?)
transfer (no matter if the transfer was successful). For Beacons, bit15 is kept
unchanged since beacons are intended to be transferred repeatedly.

W_TXBUF_CMD.Bit15 can be set ONLY while W_CMD_COUNT is non-zero.

**48080B4h - W_TXBUF_RESET (W)**

```

| 0 Disable LOC1 (0=No change, 1=Reset W_TXBUF_LOC1.Bit15)
 1 Disable CMD (0=No change, 1=Reset W_TXBUF_CMD.Bit15)
 2 Disable LOC2 (0=No change, 1=Reset W_TXBUF_LOC2.Bit15)
 3 Disable LOC3 (0=No change, 1=Reset W_TXBUF_LOC3.Bit15)
 4-5 Unknown/Not used
 6 Disable REPLY2 (0=No change, 1=Reset W_TXBUF_REPLY2.Bit15)
 7 Disable REPLY1 (0=No change, 1=Reset W_TXBUF_REPLY1.Bit15)
 8-15 Unknown/Not used |
| --- |

Firmware writes values FFFFh, 40h, 02h, xxxx, 09h, 01h, 02h, C0h.

**4808084h - W_TXBUF_TIM - Beacon TIM Index in Frame Body (R/W)**

```

| 0-7 Location of TIM parameters within Beacon Frame Body
 8-15 Not used/zero |
| --- |

Usually set to 15h, that assuming that preceding Frame Body content is:
Timestamp(8), BeaconInterval(2), Capability(2), SuppRatesTagLenParams(4),
ChannelTagLenParam(3), TimTagLen(2); so the value points to TimParams (ie.
after TimTagLen).

**480806Ch - W_TXBUF_COUNT (R/W)**

```

| 0-11 Decremented on writes to W_TXBUF_WR_DATA
 12-15 Always zero |
| --- |

Triggers IRQ08 when it reaches zero, and does then stay at zero (without
further decrementing, and without generating further IRQs).

Note: Not affected by (accidental) reads from write-only W_TXBUF_WR_DATA.

| DS Wifi Transmit Errors |
| --- |

**Automatic ACKs**

Transmit errors occur on missing ACKs. The NDS hardware is automatically
responding with an ACK when receiving a packet (if it has been addressed to the
receipients W_MACADDR setting). And, when sending a packet, the NDS hardware is
automatically checking for ACK responses.

The only exception are packets that are sent to group addresses (ie. Bit0 of
the 48bit MAC address being set to "1", eg. Beacons sent to FF:FF:FF:FF:FF:FF),
the receipient(s) don't need to respond to such packets, and the sender always
passes okay without checking for ACKs.

**480802Ch - W_TX_RETRYLIMIT (R/W)**

Specifies the maximum number of retries on Transmit Errors (eg. 07h means one
initial transmit, plus up to 7 retries, ie. max 8 transmits in total).

```

| 0-7 Retry Count (usually 07h)
 8-15 Unknown (usually 07h) |
| --- |

The Retry Count value is decremented on each Error (unless it is already 00h).
There's no automatic reload, so W_TX_RETRYLIMIT should be reinitialized by
software prior to each transmit (or, actually, there IS a reload?).

When sending multiple packets (by setting more than one bit with W_TXREQ_SET),
then the first packet may eat-up all retries, leaving only a single try to the
other packet(s).

**48081C0h - W_TX_ERR_COUNT - TransmitErrorCount (R/W)**

```

| 0-7 TransmitErrorCount
 8-15 Always zero |
| --- |

Increments on Transmit Errors. Automatically reset to zero after reading.

IRQ03 triggered when W_TX_ERR_COUNT is incremented (for NON-beacons ONLY).

IRQ05 triggered when W_TX_ERR_COUNT > 7Fh (happens INCLUDING for beacons).

**Error Notification**

Transmit Errors can be sensed via W_TX_ERR_COUNT, IRQ03, IRQ05, TX Hardware
Header entry [00h], and W_TXSTAT.Bit1.

**W_TXBUF_BEACON Errors**

As the name says, W_TXBUF_BEACON is intended for sending Beacons to group
addresses (which do not require to respond by ACKs). So, transmit errors would
occur only when mis-using W_TXBUF_BEACON to send packets to individual
addresses, but the W_TXBUF_BEACON error handling isn't fully implemented:

First of, W_TX_RETRYLIMIT isn't used, instead, W_TXBUF_BEACON errors will
result in infinite retries.

Moreover, W_TXBUF_BEACON errors seem to increment W_TX_ERR_COUNT, but without
generating IRQ03, however, IRQ05 is generated when W_TX_ERR_COUNT>7Fh.

**Other Errors**

The NDS transmit hardware seems to do little error checking on the packet
headers. The only known error-checked part is byte [04h] in the TX hardware
header (which must be 00h, 01h, or 02h). Aside from that, when sent to a group
address, it is passing okay even with invalid IEEE type/subtypes, and even with
Length/Rate entries set to zero. However, when sending such data to an
individual address, the receiving NDS won't respond by ACKs.

**Note**

Received ACKs aren't stored in WifiRAM (or, possibly, they ARE stored, but
without advancing W_RXBUF_WRCSR, so that the software won't see them, and so
that they will be overwritten by the next packet).

| DS Wifi Status |
| --- |

**480819Ch - W_RF_PINS - Status of RF-Chip Control Signals (R)**

```

| 0 Reportedly "carrier sense" (maybe 1 during RX.DTA?) (usually 0)
 1 TX.MAIN (RFU.Pin17) Transmit Data Phase (0=No, 1=Active)
 2 Unknown (RFU.Pin3) Seems to be always high (Always 1=high?)
 3-5 Not used (Always zero)
 6 TX.ON (RFU.Pin14) Transmit Preamble+Data Phase (0=No, 1=Active)
 Uhhh, no that seems to be still wrong...
 Bit6 is often set, even when not transmitting anything...
 7 RX.ON (RFU.Pin15) Receive Mode (0=No, 1=Enable)
 8-15 Not used (Always zero) |
| --- |

Physical state of the RFU board's RX/TX pins. Similar to W_RF_STATUS.

XXX better expl. for bit0-1:

```

| 0 RX.BUSY Receiving Preamble or Data (0=Idle or TX Busy, 1=RX Busy)
 1 Data Phase (for both RX/TX mode) (0=Idle or Preamble, 1=Data) |
| --- |

**4808214h - W_RF_STATUS - Current Transmit/Receive State (R)**

```

| 0-3 Current Transmit/Receive State:
 0 = Initial value on Power-up (before raising W_MODE_RST.Bit0)
 1 = RX Mode enabled (waiting for incoming data)
 2 = Switching from RX to TX (takes a few clock cycles)
 3 = TX Mode active (sending preamble and data)
 4 = Switching from TX to RX (takes a few clock cycles)
 5 = Multiplay: CMD was sent, waiting for replies (RF_PINS=0084h) (uh?)
 Or rather: CMD was received, preparing reply? (on slave side!)
 6 = RX (processing incoming data?)
 7 = Switching from RX/REPLY to TX/ACK (between STAT=5 and STAT=8)
 8 = Multiplay: Sending REPLY, or CMD-Ack (RF_PINS=0046h)
 9 = Idle (upon IRQ13, and upon raising W_MODE_RST.Bit0)
 4-15 Always zero? |
| --- |

Numeric Status Code. Similar to W_RF_PINS.

**4808268h - W_RXTX_ADDR - Current Receive/Transmit Address (R)**

```

| 0-11 Halfword address
 12-15 Always zero |
| --- |

Indicates the halfword that is currently transmitted or received. Can be used
by Start Receive IRQ06 handler to determine how many halfwords of the packet
have been already received (allowing to pre-examine portions of the packet
header even when the whole packet isn't fully received). Can be also used in
Transmit Start IRQ07 handler to determine which packet is currently
transmitted.

| DS Wifi Timers |
| --- |

**48080E8h - W_US_COUNTCNT - Microsecond counter enable (R/W)**

```

| 0 Counter Enable (0=Disable, 1=Enable)
 1-15 Always zero |
| --- |

Activates W_US_COUNT, and also W_BEACON_COUNT and W_POST_BEACON (which are
decremented when lower 10bit of W_US_COUNT wrap from 3FFh to 000h). Note:
W_POWER_US must be enabled, too.

**48080F8h - W_US_COUNT0 - Microsecond counter, bits 0-15 (R/W)**

**48080FAh - W_US_COUNT1 - Microsecond counter, bits 16-31 (R/W)**

**48080FCh - W_US_COUNT2 - Microsecond counter, bits 32-47 (R/W)**

**48080FEh - W_US_COUNT3 - Microsecond counter, bits 48-63 (R/W)**

```

| 0-63 Counter Value in microseconds (incrementing) |
| --- |

Clocked by the 22.00MHz oscillator on the RFU board (ie. not by the 33.51MHz
system clock). The 22.00MHz are divided by a 22-step prescaler.

**48080EAh - W_US_COMPARECNT - Microsecond compare enable (R/W)**

```

| 0 Compare Enable (0=Disable, 1=Enable) (IRQ14/IRQ15)
 1 Force IRQ14 (0=No, 1=Force Now) (Write-only)
 2-15 Always zero |
| --- |

Activates IRQ14 on W_US_COMPARE matches, and IRQ14/IRQ15 on W_BEACON_COUNT.

**48080F0h - W_US_COMPARE0 - Microsecond compare, bits 0-15 (R/W)**

**48080F2h - W_US_COMPARE1 - Microsecond compare, bits 16-31 (R/W)**

**48080F4h - W_US_COMPARE2 - Microsecond compare, bits 32-47 (R/W)**

**48080F6h - W_US_COMPARE3 - Microsecond compare, bits 48-63 (R/W)**

```

| 0 Block Beacon IRQ14 until W_US_COUNT=W_US_COMPARE (0=No, 1=Block) (W)
 1-9 Always zero
 10-63 Compare Value in milliseconds (aka microseconds/1024) (R/W) |
| --- |

Triggers IRQ14 (see IRQ14 notes below) when W_US_COMPARE matches W_US_COUNT.

Usually set to FFFFFFFFFFFFFC00h (ie. almost/practically never). Instead, IRQ14
is usually derived via W_BEACON_COUNT.

**480811Ch - W_BEACON_COUNT - Beacon Counter (R/W)**

Triggers IRQ14 and IRQ15 (see IRQ14/IRQ15 notes below) when it reaches 0000h
(if W_PRE_BEACON is non-zero, then IRQ15 occurs that many microseconds in
advance).

```

| 0-15 Decrementing Millisecond Counter (reloaded with W_BEACONINT upon IRQ14) |
| --- |

Set to W_BEACONINT upon IRQ14 events (unlike the other W_US_COMPARE related
actions, this is done always, even if W_US_COMPARECNT is zero).

When reaching 0000h, it is immediately reloaded (as for US_COUNT matches), so
the counting sequence is ..,3,2,1,BEACONINT,.. (not 3,2,1,ZERO,BEACONINT).

**4808134h - W_POST_BEACON - Post-Beacon Counter (R/W)**

```

| 0-15 Decrementing Millisecond Counter (reloaded with FFFFh upon IRQ14) |
| --- |

Triggers IRQ13 when it reaches 0000h (no matter of W_US_COMPARECNT), and does
then stay fixed at 0000h (without any further decrement/wrapping to FFFFh).

Set to FFFFh upon IRQ14 (by hardware), the IRQ14 handler should then adjust the
register (by software) by adding the Tag=DDh Beacon header's Stepping value
(usually 000Ah) to it.

Seems to be used to indicate beacon transmission time (possibly including
additional time being reserved for responses)?

**480808Ch - W_BEACONINT - Beacon Interval (R/W)**

Reload value for W_BEACON_COUNT.

```

| 0-9 Frequency in milliseconds of beacon transmission
 10-15 Always zero |
| --- |

Should be initialized randomly to 0CEh..0DEh or so. The random setting reduces
risk of repeated overlaps with beacons from other hosts.

**4808110h - W_PRE_BEACON - Pre-Beacon Time (R/W)**

```

| 0-15 Pre-Beacon Time in microseconds (static value, ie. NOT decrementing) |
| --- |

Allows to define the distance between IRQ15 and IRQ14. The setting doesn't
affect the IRQ14 timing (which occurs at the W_BEACON_COUNT'th millisecond
boundary), but IRQ15 occurs in advance (at the W_BEACON_COUNT'th millisecond
boundary minus W_PRE_BEACON microseconds). If W_PRE_BEACON is zero, then both
IRQ14 and IRQ15 occur exactly at the same time.

**4808088h - W_LISTENCOUNT - Listen Count (R/W)**

```

| 0-7 Decremented by hardware at IRQ14 events (ie. once every beacon)
 8-15 Always zero |
| --- |

Reload occurs immediately BEFORE decrement, ie. with W_LISTENINT=04h, it will
go through values 03h,02h,01h,00h,03h,02h,01h,00h,etc.

**480808Eh - W_LISTENINT - Listen Interval (R/W)**

```

| 0-7 Listen Interval, counted in beacons (usually 02h)
 8-15 Always zero |
| --- |

Reload value for W_LISTENCOUNT.

**480810Ch - W_CONTENTFREE (R/W)**

```

| 0-15 Decrementing microsecond counter |
| --- |

Operated always (no matter of W_US_COUNTCNT).

Once when it has reached 0000h, it seems to stay fixed at 0000h.

"[Set to the remaining duration of contention-free period when

receiving beacons - only *really* necessary for powersaving mode]"

**IRQ13 Notes (Post-Beacon Interrupt)**

IRQ13 is generated by W_POST_BEACON. It's simply doing:

```

| W_IF.Bit13=1 ;interrupt request |
| --- |

If W_POWER_TX.Bit1=0, then additionally enter sleep mode:

```

| [4808034h]=0002h ;W_INTERNAL ;(similar to W_POWERFORCE=8001h)
 [480803Ch]=02xxh ;W_POWERSTATE ;(W_TXREQ_READ.Bit4 is kept intact though)
 [480819Ch]=0046h ;W_RF_PINS.7=0;disable receive (enter idle mode) (RX.ON=Low)
 [4808214h]=0009h ;W_RF_STATUS=9;indicate idle mode |
| --- |

Unlike for IRQ14/IRQ15, that's done no matter of W_US_COMPARECNT.

**IRQ14 Notes (Beacon Interrupt)**

IRQ14 is generated by W_US_COMPARE, and by W_BEACON_COUNT.

Aside from just setting the IRQ flag in W_IF, the hardware does:

```

| W_BEACON_COUNT=W_BEACONINT ;next IRQ15/IRQ14
 (Above is NOT done when IRQ14 was forced via W_US_COMPARECNT.Bit1) |
| --- |

If W_US_COMPARECNT is 1, then the hardware does additionally:

```

| (Below IS ALSO DONE when IRQ14 was forced via W_US_COMPARECNT.Bit1)
 W_IF.Bit14=1
 W_POST_BEACON=FFFFh ;about 64 secs (ie. almost never) ;next IRQ13 ("never")
 W_TXREQ_READ=W_TXREQ_READ AND FFF2h
 if W_TXBUF_BEACON.15 then W_TXBUSY.Bit4=1
 if W_LISTENCOUNT=00h then W_LISTENCOUNT=W_LISTENINT
 W_LISTENCOUNT=W_LISTENCOUNT-1 |
| --- |

If W_TXBUF_BEACON.Bit15=1, then following is done shortly after IRQ14:

```

| W_RF_PINS.Bit7=0 ;disable receive (RX.ON=Low)
 W_RF_STATUS=2 ;indicate switching from RX to TX mode |
| --- |

If W_TXBUF_BEACON.Bit15=1, then following is done a bit later:

```

| W_RF_PINS.Bit6=1 ;transmit preamble start (TX.ON=High)
 W_RF_STATUS=3 ;indicate TX mode |
| --- |

The IRQ14 handler should then do the following (by software):

```

| W_POST_BEACON = W_POST_BEACON + TagDDhSteppingValue ;next IRQ13 |
| --- |

For using only ONE of the two IRQ14 sources: W_BEACON_COUNT can be disabled by
setting both W_BEACON_COUNT and W_BEACONINT to zero. W_US_COMPARE can be sorts
of "disabled" by setting it to value distant from W_US_COUNT, such like
compare=count-400h.

**IRQ07 Notes (Transmit Start Data; occurs after preamble)**

```

| W_IF.Bit7=1 ;interrupt request
 W_RF_PINS.Bit1=1 ;start data transfer (preamble finished now) (TX.MAIN=High) |
| --- |

Below only if packet was sent through W_TXBUF_BEACON, or if it was sent via
W_TXBUF_LOCn, with W_TXBUF_LOCn.Bit13 being zero:

```

| [TXBUF...] = W_TX_SEQNO*10h ;auto-adjust IEEE Sequence Control
 W_TX_SEQNO=W_TX_SEQNO+1 ;increase sequence number |
| --- |

**IRQ01 Notes (Transmit Done)**

The following happens shortly before IRQ01:

```

| W_RF_PINS.Bit6=0 ;disable TX (TX.ON=Low)
 W_RF_STATUS=4 ;indicate switching from TX to RX mode |
| --- |

Then, upon IRQ01, the following happens:

```

| W_IF.Bit1=1 ;interrupt request
 W_RF_PINS.Bit1=0 ;disable TX (TX.MAIN=Low)
 W_RF_PINS.Bit7=1 ;enable RX (RX.ON=High)
 W_RF_STATUS=1 ;indicate RX mode |
| --- |

**IRQ15 Notes (Pre-Beacon Interrupt)**

IRQ15 is generated via W_BEACON_COUNT and W_PRE_BEACON. It's simply doing:

```

| if W_US_COMPARECNT=1 then W_IF.Bit15=1 |
| --- |

If W_POWER_TX.Bit0=1, then additionally wakeup from sleep mode:

```

| W_RF_PINS.Bit7=1 ;enable RX (RX.ON=High) ;\gets set like so a good while
 W_RF_STATUS=1 ;indicate RX mode ;/after IRQ15 (but not immediately) |
| --- |

**Beacon IRQ Sequence**

```

| IRQ15 Pre-Beacon (beacon will be transferred soon)
 IRQ14 Beacon (beacon will be transferred very soon) (carrier starts)
 IRQ07 Tx Start (beacon transfer starts) (if enabled in W_TXBUF_BEACON.15)
 IRQ01 Tx End (beacon transfer done) (if enabled in W_TXSTATCNT.15)
 IRQ13 Post-Beacon (beacon transferred) (unless next IRQ14 occurs earlier) |
| --- |

That, for tranmitting beacons. (For receiving, IRQ07/IRQ01 would be replaced by
Rx IRQ's, provided that a remote unit is sending beacons).

| DS Wifi Multiplay Master |
| --- |

These registers are used for multiplay host-to-client (aka master to slave)
commands.

**48080EEh - W_CMD_COUNTCNT (R/W)**

```

| 0 Enable W_CMD_COUNT (0=Disable, 1=Enable)
 1-15 Always Zero |
| --- |

**4808118h - W_CMD_COUNT (R/W)**

```

| 0-15 Decremented once every 10 microseconds (Stopped at 0000h) |
| --- |

Defines the time interval during which W_TXBUF_CMD transfers are possible. That
CMD will be automatically transferred again if there are errors (missing
REPLY's), but it will abort if there isn't enough time left in W_CMD_COUNT.

IRQ12 is thrown when cmd/reply were successful, or when CMD_COUNT reached zero
(failed).

Firmware Beacon IRQ14 handler checks for W_CMD_COUNT<=0Ah.

**48080C0h - W_CMD_TOTALTIME - (R/W)**

```

| 0-15 Duration per ALL slave response packet(s) in microseconds |
| --- |

Before sending a MASTER packet, this port should be set to the same value as
the MASTER packet's IEEE header's Duration/ID entry.

**48080C4h - W_CMD_REPLYTIME - (R/W)**

```

| 0-15 Duration per SINGLE slave response packet in microseconds |
| --- |

Before sending a MASTER packet, this port should be set to the expected per
slave response time.

Note: Nintendo's multiboot/pictochat code is also putting this value in the 1st
halfword of the MASTER packet's frame body.

**At 2MBit/s transfer rate, the values should be set up sorts of like so:**

```

| master_time = (master_bytes*4)+(60h) ;60h = 96 decimal = short preamble
 slave_time = (slave_bytes*4)+(0D0h..0D2h)
 all_slave_time = (EAh..F0h)+(slave_time+0Ah)*num_slaves
 txhdr[2] = slave_bits ;hardware header (*)
 ieee[2] = all_slave_time ;ieee header (duration/id)
 body[0] = slave_time ;duration per slave (for multiboot/pictochat)
 body[2] = slave_bits ;frame body -- required (*)
 [48080C0h] = all_slave_time ;W_CMD_TOTALTIME
 [48080C4h] = slave_time ;W_CMD_REPLYTIME duration per slave
 [4808118h] = (388h+(num_slaves*slave_time)+master_time+32h)/10 ;W_CMD_COUNT
 [4808090h] = 8000h+master_packet_address ;start transmit ;W_TXBUF_CMD |
| --- |

With the byte values counting the ieee frame header+body+fcs.

(*) The hardware doesn't actually seem to use the "slave_bits" entry in the
hardware header, instead, it is using the "slave_bits" entry in the frame
body(!)

**Flowchart (at Master side)**

```

| After starting transfer via TXREQ and TXBUF_CMD write:
 TXBUSY=2 (formerly 0) (after TXBUF_CMD write, or sometimes a bit later)
 After about 50-500 microseconds: ;\
 RF_STAT=3 (TXing) (formerly 2) ;
 RXTX_ADDR=0006h..0008h (TXbuf+0Ch..) (formerly in RXBUF) ; CMD
 SEQNO+1 ;
 After TX preamble: ;
 IF=80h (TX Start, for CMD) ;
 RXTX_ADDR=0009h..0xxxh (TXbuf..) ;
 After TX data: ;
 optional: IF=02h (TX Done, for CMD) (if enabled in TXSTATCNT);
 optional: TXSTAT=0800h (CMD done) (if enabled in TXSTATCNT);
 RF_STAT=5 (CMD done, prepare for REPLY) ;/
 US=0017h ;\
 RXTX_ADDR=rxbuf.. ;
 After RX preamble: ;
 IF=40h (RX Start, for REPLY) ; REPLY
 RXTX_ADDR=rxbuf.. ; (if any)
 After RX data: ;
 IF=01h (RX Done, for REPLY) ;
 WRCSR+18h (for REPLY) ;/
 After a dozen microseconds: ;\
 RF_STAT=7 ;Switching from REPLY to ACK ;
 RF_STAT=8 ;TXing ACK (shortly after above STAT=7) ;
 RXTX_ADDR=0FC0h (special dummy addr during TX ACK) ;
 After TX preamble: ; ACK
 IF=80h (TX Start, for ACK) ;
 After TX data: ;
 optional: IF=02h (TX Done, for ACK) (if enabled in TXSTATCNT);
 optional: TXSTAT=0B01h (ACK done) (if enabled in TXSTATCNT);
 TXBUSY=0000h (formerly 0002h) ;
 TXBUF_CMD.bit15=0 ;
 TXHDR_0=0001h (okay) (formerly 0000h) ;
 TXHDR_2=0000h (no error flags) (formerly 0002h) ;
 SEQNO+1 ;
 RF_STAT=1 ;RX awaiting ;
 IF=1000h (CMD timeslot done) (shortly AFTER above IF=02h) ;/ |
| --- |

**Sending a W_TXBUF_CMD frame starts the following process:**

```

| 1. MP host sends the CMD frame, as soon as possible. after preamble,
 IRQ7 is triggered
 2. once the transfer is finished: if bit14 in W_TXSTATCNT is set,
 W_TXSTAT is set to 0x0800, and IRQ1 is triggered
 somewhere here: set W_RF_STATUS=5, RFPINS=0x0084
 3. hardware waits for MP clients' replies, duration is:
 16 + ((10 + W_CMD_REPLYTIME) * count_ones(client_mask_from_frame_body))
 4. MP host sends the CMD ack. after preamble, IRQ7 is triggered
 (this is why you get two IRQ7's from a CMD transfer)
 5. during the ack transfer, W_RF_STATUS is 8, and W_RXTXADDR is 0x0FC0
 6. once the transfer is finished: if bit13 in W_TXSTATCNT is set,
 W_TXSTAT is set to 0x0B01, and IRQ1 is triggered.
 7. the TX header of the CMD frame is adjusted: bits in TXheader[02] are
 cleared to indicate that the corresponding clients responded
 successfully. Nintendo software checks this. |
| --- |

I haven't looked a lot into how retries work. they seem to repeat the entire
process.

the CMD ack is sent automatically. the packet is what is described here: the
03:09:BF:00:00:03 flow. MP clients will receive it like a regular frame.

IRQ12 is used to signal the end of the CMD transfer process. it will be either
when the final ack is done transferring, if everything was successful, or
otherwise at the end of W_CMD_COUNT.

| DS Wifi Multiplay Slave |
| --- |

The slave mode is activated by setting W_AID_LOW to a nonzero value (the slave
number in range 1..15). That settings specifies that (and when) to send replies
to incoming commands.

The replies are queued in W_TXBUF_REPLY1 (next packet) and W_TXBUF_REPLY2
(readonly, current packet).

Software does usually write to the REPLY1 register. Upon incomings CMD's, the
hardware does automatically forward REPLY1 to REPLY2 (and reset REPLY1 to
0000h).

Because of that queuing, one should use two alternating Wifi RAM locations (ie.
don't overwrite the current packet when preparing the next packet).

Alternately, one can manually forward REPLY1 to REPLY2 (via W_RXCNT.bit7, but
that's normally not needed, except maybe for things like manually resetting
REPLY2).

These registers are used for multiplay client-to-host (aka slave to master)
responses.

**4808094h - W_TXBUF_REPLY1 - Multiplay Next Reply Transmit Location (R/W)**

**4808098h - W_TXBUF_REPLY2 - Multiplay Current Reply Transmit Location (R)**

```

| 0-11 Halfword address
 12-14 Unknown (the bits can be set, ie. they DO exist)
 15 Enable |
| --- |

Note: W_TXBUF_REPLY2.Bit15 is NOT cleared after SENDING the reply (instead,
REPLY2 gets overwritten by REPLY1 when RECEIVING the next CMD).

**Flowchart (at Slave side)**

```

| At incoming CMD DATA packet: ;\
 RF_STATUS=6 ;RX processing incoming stuff ;
 After RX preamble: ; CMD
 IRQ6 (RX Start, for CMD DATA) ; DATA
 After RX data: ;
 IRQ0 (RX Done, for CMD DATA) ;
 WRCSR=WRCSR+(size of CMD DATA) ;
 RF_STATUS=5 ;preparing REPLY ;
 if REPLY2.bit15=1 ;
 TXHDR[1]=TXHDR[0] ;<-- or sometimes random? ;\adjust TXHDR[0,1] ;
 TXHDR[0]=01h ;<-- mark done/discarded ;/for <old> REPLY2 ;
 REPLY2=REPLY1, REPLY1=0000h ;-forward new reply ;
 if REPLY2.bit15=1 ;
 TXHDR[4] incremented (unless already max FFh) ;\adjust TXHDR[4,5] ;
 TXHDR[5]=00h ;/for <new> REPLY2 ;
 TX_SEQNO incremented ;<-- done here if REPLY2 exists ;/
 After some moment (at the AID_LOW slot?): ;\
 RF_STATUS=8 ;TX sending REPLY ;
 After TX preamble: ; REPLY
 IRQ7 (TX Start, for REPLY) ;
 After TX data: ;
 RF_STATUS=1 ;RX awaiting next packet ;
 optional: IRQ1 (TX Done) (only if enabled in TXSTATCNT, and REPLY2.bit15=1)
 optional: TXSTAT=0401h (only if enabled in TXSTATCNT) ;
 if REPLY2.bit15=0 ;
 SEQNO increased ;<-- done here when REPLY2 is empty ;/
 After some moment: ;\
 RF_STATUS=6 ;RX processing incoming stuff ;
 After RX preamble: ; CMD
 IRQ6 (RX Start, for CMD ACK) ; ACK
 After RX data: ;
 IRQ0 (RX Done, for CMD ACK) ;
 WRCSR=WRCSR+(size of CMD ACK) ;
 RF_STATUS=1 ;RX awaiting next packet ;/
 Thereafter, Nintendo's software seems to require a delay (at least
 100h microseconds) before receiving the next CMD DATA packet. |
| --- |

**on client side**

The reply transfer is automatically initiated when receiving a MP CMD frame
(this seems to be based on the incoming IEEE header's frame control value
and/or broadcast address, CMD frames typically have FC=0228h).

When receiving the MP CMD frame, the hardware determines its position using its
W_AID_LOW register and the frame's client bitmask, then waits for its turn to
reply.

W_TXBUF_REPLY1 is latched into W_TXBUF_REPLY2, and reset to zero.

The byte at TXHDR[04h] is set incremented. Nintendo software checks for this.

when transferring the MP reply itself:

IRQ7 is triggered. W_RF_STATUS is 8.

at the end: if W_TXSTATCNT.bit12=1, W_TXSTAT is set to 0401h, and IRQ1 is
triggered.

**Reportedly...**

It will always send a reply - if W_TXBUF_REPLY1 isn't configured, it will send
an empty reply frame (with frame control 0x0158). W_RXTXADDR isn't modified
when sending a default empty reply.

| DS Wifi Configuration Ports |
| --- |

**4808120h - W_CONFIG_120h (R/W) ;init from firmware[04Ch] ;81ff 0048->SAME**

**4808122h - W_CONFIG_122h (R/W) ;init from firmware[04Eh] ;ffff 4840->SAME**

**4808124h - W_CONFIG_124h (R/W) ;init from firmware[05Eh] ;ffff 0000->0032**

**4808128h - W_CONFIG_128h (R/W) ;init from firmware[060h] ;ffff 0000->01F4**

**4808130h - W_CONFIG_130h (R/W) ;init from firmware[054h] ;0fff 0142->0140**

**4808132h - W_CONFIG_132h (R/W) ;init from firmware[056h] ;8fff 8064->SAME**

**4808140h - W_CONFIG_140h (R/W) ;init from firmware[058h] ;ffff 0000->E0E0**

**4808142h - W_CONFIG_142h (R/W) ;init from firmware[05Ah] ;ffff 2443->SAME**

**4808144h - W_CONFIG_144h (R/W) ;init from firmware[052h] ;00ff 0042->SAME**

**4808146h - W_CONFIG_146h (R/W) ;init from firmware[044h] ;00ff 0016->0002**

**4808148h - W_CONFIG_148h (R/W) ;init from firmware[046h] ;00ff 0016->0017**

**480814Ah - W_CONFIG_14Ah (R/W) ;init from firmware[048h] ;00ff 0016->0026**

**480814Ch - W_CONFIG_14Ch (R/W) ;init from firmware[04Ah] ;ffff 162C->1818**

**4808150h - W_CONFIG_150h (R/W) ;init from firmware[062h] ;ff3f 0204->0101**

**4808154h - W_CONFIG_154h (R/W) ;init from firmware[050h] ;7a7f 0058->SAME**

These ports are to be initialized from firmware settings.

Above comments show the R/W bits (eg. 81FFh means bit15 and bit8-0 are R/W,
bit14-9 are always zero), followed by the initial value on Reset (eg. 0048h),
followed by new value after initialization from firmware settings (eg. 0032h,
or SAME if the Firmware value is equal to the Reset value; these values seem to
be identical in all currently existing consoles).

Note: Firmware part4 changes W_CONFIG_124h to C8h, and W_CONFIG_128h to 7D0h,
and W_CONFIG_150h to 202h, and W_CONFIG_140h depending on tx rate and preamble:

```

| W_CONFIG_140h = firmware[058h]+0202h ;1Mbit/s
 W_CONFIG_140h = firmware[058h]+0202h-6161h ;2Mbit/s with long preamble
 W_CONFIG_140h = firmware[058h]+0202h-6161h-6060h ;2Mbit/s with short preamble |
| --- |

**48080ECh - W_CONFIG_0ECh (R/W) ;firmware writes 3F03h (same as on power-up)**

**48080D4h - W_CONFIG_0D4h (R/W) ;firmware writes 0003h (affectd by W_MODE_RST)**

**48080D8h - W_CONFIG_0D8h (R/W) ;firmware writes 0004h (same as on power-up)**

**4808254h - W_CONFIG_254h (?) ;firmware writes 0000h (read: EEEEh on DS-Lite)**

Firmware just initializes these ports with fixed values, without further using
them after initialization.

**48080DAh - W_RX_LEN_CROP (R/W) ;firmware writes 0602h (same as on power-up)**

```

| 0-7 Decrease RX Length by N halfwords for Non-WEP packets (usually 2)
 8-15 Decrease RX Length by N halfwords for WEP packets (usually 6) |
| --- |

Used to exclude the FCS (and WEP IV+ICV) from the length entry in the Hardware
RX Header.

| DS Wifi Baseband Chip (BB) |
| --- |

BB-Chip Mitsumi MM3155 (DS), or BB/RF-Chip Mitsumi MM3218 (DS-Lite)

**4808158h - W_BB_CNT - Baseband serial transfer control (W)**

```

| 0-7 Index (00h-68h)
 8-11 Not used (should be zero)
 12-15 Direction (5=Write BB_WRITE to Chip, 6=Read from Chip to BB_READ) |
| --- |

Transfer is started after writing to this register.

**480815Ah - W_BB_WRITE - Baseband serial write data (W)**

```

| 0-7 Data to be sent to chip (by following W_BB_CNT transfer)
 8-15 Not used (should be zero) |
| --- |

**480815Ch - W_BB_READ - Baseband serial read data (R)**

```

| 0-7 Data received from chip (from previous W_BB_CNT transfer)
 8-15 Not used (always zero) |
| --- |

**480815Eh - W_BB_BUSY - Baseband serial busy flag (R)**

```

| 0 Transfer Busy (0=Ready, 1=Busy)
 1-15 Always zero |
| --- |

Used to sense transfer completion after writes to W_BB_CNT.

Not sure if I am doing something wrong... but the busy flag doesn't seem to get
set immediately after W_BB_CNT writes, and works only after waiting a good
number of clock cycles?

**4808160h - W_BB_MODE (R/W)**

```

| 0-7 Always zero
 8 Unknown (usually 1) (no effect no matter what setting?)
 9-13 Always zero
 14 Unknown (usually 0) (W_BB_READ gets unstable when set)
 15 Always zero |
| --- |

This register is initialized by firmware bootcode - don't change.

**4808168h - W_BB_POWER (R/W)**

```

| 0-3 Disable whatever (usually 0Dh=disable)
 4-14 Always zero
 15 Disable W_BB_ports (usually 1=Disable) |
| --- |

Must be set to 0000h before accessing BB registers.

**Read-Write-Ability of the BB-Chip Mitsumi MM3155 registers (DS)**

```

| Index Num Dir Expl.
 00h 1 R always 6Dh (R) (Chip ID)
 01h..0Ch 12 R/W 8bit R/W
 0Dh..12h 6 - always 00h
 13h..15h 3 R/W 8bit R/W
 16h..1Ah 5 - always 00h
 1Bh..26h 12 R/W 8bit R/W
 27h 1 - always 00h
 28h..4Ch R/W 8bit R/W
 4Dh 1 R always 00h or BFh (depending on other regs)
 4Eh..5Ch R/W 8bit R/W
 5Dh 1 R always 01h (R)
 5Eh..61h - always 00h
 62h..63h 2 R/W 8bit R/W
 64h 1 R always FFh or 3Fh (depending on other regs)
 65h 1 R/W 8bit R/W
 66h 1 - always 00h
 67h..68h 2 R/W 8bit R/W
 69h..FFh - always 00h |
| --- |

**Read-Write-Ability of the BB/RF-Chip Mitsumi MM3218 (DS-Lite)**

Same as above. Except that reading always seems to return [5Dh]=00h. And, for
whatever reason, Nintendo initializes DS-Lite registers by writing [00h]=03h
and [66h]=12h. Nethertheless, the registers always read as [00h]=6Dh and
[66h]=00h, ie. same as on original DS.

**Important BB Registers**

Registers 0..68h are initialized by firmware bootcode, and (most) of these
settings do not need to be changed by other programs, except for:

```

| Addr Initial Meaning
 01h 0x9E [unsetting/resetting bit 7 initializes/resets the system?]
 02h unknown (firmware is messing with this register)
 06h unknown (firmware is messing with this register, too)
 13h 0x00 CCA operation - criteria for receiving
 0=only use Carrier Sense (CS)
 1=only use Energy Detection (ED)
 2=receive if CS OR ED
 3=receive only if CS AND ED
 1Eh 0xBB see change channels flowchart (Ext. Gain when RF[09h].bit16=0)
 35h 0x1F Energy Detection (ED) criteria
 value 0..61 (representing energy levels of -60dBm to -80dBm) |
| --- |

| DS Wifi RF Chip |
| --- |

RF-Chip RF9008 (compatible to RF2958 from RF Micro Devices, Inc.) (Original DS)

BB/RF-Chip Mitsumi MM3218 (DS-Lite)

**480817Ch - W_RF_DATA2 - RF chip serial data/transfer enable (R/W)**

For Type2 (ie. firmware[040h]<>3):

```

| 0-1 Upper 2bit of 18bit data
 2-6 Index (00h..1Fh) (firmware uses only 00h..0Bh)
 7 Command (0=Write data, 1=Read data)
 8-15 Should be zero (not used with 24bit transfer) |
| --- |

For Type3 (ie. firmware[040h]=3):

```

| 0-3 Command (5=Write data, 6=Read data)
 4-15 Should be zero (not used with 20bit transfer) |
| --- |

Writing to this register starts the transfer.

**480817Eh - W_RF_DATA1 - RF chip serial data (R/W)**

For Type2 (ie. firmware[040h]<>3):

```

| 0-15 Lower 16bit of 18bit data |
| --- |

For Type3 (ie. firmware[040h]=3):

```

| 0-7 Data (to be written to chip) (or being received from chip)
 8-15 Index (usually 00h..28h) (index 40h..FFh are mirrors of 00h..3Fh) |
| --- |

This value should be set before setting W_RF_DATA2.

**4808180h - W_RF_BUSY - RF chip serial busy flag (R)**

```

| 0 Transfer Busy (0=Ready, 1=Busy)
 1-15 Always zero |
| --- |

Used to sense transfer completion after writes to W_RF_DATA2.

**4808184h - W_RF_CNT - RF chip serial control (R/W)**

```

| 0-5 Transfer length (init from firmware[041h].Bit0-5)
 6-7 Always zero
 8 Unknown (init from firmware[041h].Bit7)
 9-13 Always zero
 14 Unknown (usually 0)
 15 Always zero |
| --- |

This register is initialized by firmware bootcode - don't change.

Usually, Type2 has length=24bit and flag=0. Type3 uses length=20bit and flag=1.

**Caution For Type2 (ie. firmware[040h]<>3)**

Before accessing Type2 RF Registers, first BB[01h] must have been properly
initialized (ie. BB[01h].Bit7 must have been toggled from 0-to-1).

| DS Wifi RF9008 Registers |
| --- |

**RF9008 (RF2958 compatible)**

2.4GHz Spread-Spectrum Transceiver - RF Micro Devices, Inc.

**RF chip data (Type2) (initial NDS settings from firmware, example)**

```

| Firmware Index Data
 (24bit) (4bit) (18bit)
 00C007h = 00h + 0C007h ;-also set to 0C008h for power-down
 129C03h = 04h + 29C03h
 141728h = 05h + 01728h ;\these are also written when changing channels
 1AE8BAh = 06h + 2E8BAh ;/
 1D456Fh = 07h + 1456Fh
 23FFFAh = 08h + 3FFFAh
 241D30h = 09h + 01D30h ;-bit10..14 should be also changed per channel?
 """"50h = """ + """50h ;firmware v5 and up uses narrower tx filter
 280001h = 0Ah + 00001h
 2C0000h = 0Bh + 00000h
 069C03h = 01h + 29C03h
 080022h = 02h + 00022h
 0DFF6Fh = 03h + 1FF6Fh |
| --- |

**RF[00h] - Configuration Register 1 (CFG1) (Power on: 00007h)**

```

| 17-16 Reserved, program to zero (0)
 15-14 Reference Divider Value (0=Div2, 1=Div3, 2=Div44, 3=Div1)
 3 Sleep Mode Current (0=Normal, 1=Very Low)
 2 RF VCO Regulator Enable (0=Disable, 1=Enable)
 1 IF VCO Regulator Enable (0=Disable, 1=Enable)
 0 IF VGA Regulator Enable (0=Disable, 1=Enable) |
| --- |

**RF[01h] - IF PLL Register 1 (IFPLL1) (Power on: 09003h)**

```

| 17 IF PLL Enable (0=Disable, 1=Enable)
 16 IF PLL KV Calibration Enable (0=Disable, 1=Enable)
 15 IF PLL Coarse Tuning Enable (0=Disable, 1=Enable)
 14 IF PLL Loop Filter Select (0=Internal, 1=External)
 13 IF PLL Charge Pump Leakage Current (0=Minimum value, 1=2*Minimum value)
 12 IF PLL Phase Detector Polarity (0=Positive, 1=Negative)
 11 IF PLL Auto Calibration Enable (0=Disable, 1=Enable)
 10 IF PLL Lock Detect Enable (0=Disable, 1=Enable)
 9 IF PLL Prescaler Modulus (0=4/5 Mode, 1=8/9 Mode)
 8-4 Reserved, program to zero (0)
 3-0 IF VCO Coarse Tuning Voltage (N=Voltage*16/VDD) |
| --- |

**RF[02h] - IF PLL Register 2 (IFPLL2) (Power on: 00022h)**

```

| 17-16 Reserved, program to zero (0)
 15-0 IF PLL divide-by-N value |
| --- |

**RF[03h] - IF PLL Register 3 (IFPLL3) (Power on: 1FF78h)**

```

| 17 Reserved, program to zero (0)
 16-8 IF VCO KV Calibration, delta N value (signed) ;DeltaF=(DN/Fr)
 7-4 IF VCO Coarse Tuning Default Value
 3-0 IF VCO KV Calibration Default Value |
| --- |

**RF[04h] - RF PLL Register 1 (RFPLL1) (Power on: 09003h)**

```

| 17-10 Same as for RF[01h] (but for RF, not for IF)
 9 RF PLL Prescaler Modulus (0=8/9 Mode, 1=8/10 Mode)
 8-0 Same as for RF[01h] (but for RF, not for IF) |
| --- |

**RF[05h] - RF PLL Register 2 (RFPLL2) (Power on: 01780h)**

```

| 17-6 RF PLL Divide By N Value
 5-0 RF PLL Numerator Value (Bits 23-18) |
| --- |

**RF[06h] - RF PLL Register 3 (RFPLL3) (Power on: 00000h)**

```

| 17-0 RF PLL Numerator Value (Bits 17-0) |
| --- |

**RF[07h] - RF PLL Register 4 (RFPLL4) (Power on: 14578h)**

```

| 17-10 Same as for RF[03h] (but for RF, not for IF) ;and, DN=(deltaF/Fr)*256 |
| --- |

**RF[08h] - Calibration Register 1 (CAL1) (Power on: 1E742h)**

```

| 17-13 VCO1 Warm-up Time ;TVCO1=(approximate warm-up time)*(Fr/32)
 12-8 VCO1 Tuning Gain Calibration ;TLOCK1=(approximate lock time)*(Fr/128)
 7-3 VCO1 Coarse Tune Calibration Reference ;VALUE=(average time)*(Fr/32)
 2-0 Lock Detect Resolution (0..7) |
| --- |

**RF[09h] - TXRX Register 1 (TXRX1) (Power on: 00120h)**

```

| 17 Receiver DC Removal Loop (0=Enable DC Removal Loop, 1=Disable)
 16 Internal Variable Gain for VGA (0=Disable/External, 1=Enable/Internal)
 15 Internal Variable Gain Source (0=From TXVGC Bits, 1=From Power Control)
 14-10 Transmit Variable Gain Select (TXVGC) (0..1Fh = High..low gain)
 9-7 Receive Baseband Low Pass Filter (0=Wide Bandwidth, 7=Narrow)
 6-4 Transmit Baseband Low Pass Filter (0=Wide Bandwidth, 7=Narrow)
 3 Mode Switch (0=Single-ended mode, 1=Differential mode)
 2 Input Buffer Enable TX (0=Input Buffer Controlled by TXEN, 1=By BBEN)
 1 Internal Bias Enable (0=Disable/External, 1=Enable/Internal)
 0 TX Baseband Filters Bypass (0=Not Bypassed, 1=Bypassed) |
| --- |

**RF[0Ah] - Power Control Register 1 (PCNT1) (Power on: 00000h)**

```

| 17-15 Select MID_BIAS Level (1.6V through 2.6V)
 14-9 Desired output power at antenna (N*0.5dBm)
 8-3 Power Control loop-variation-adjustment Offset (signed, N*0.5dB)
 2-0 Desired delay for using a single TX_PE line (N*0.5us) |
| --- |

**RF[0Bh] - Power Control Register 2 (PCNT2) (Power on: 00000h)**

```

| 17-12 Desired MAX output power when PABIAS=MAX=2.6V (N*0.5dBm)
 11-6 Desired MAX output power when PABIAS=MID_BIAS (N*0.5dBm)
 5-0 Desired MAX output power when PABIAS=MIN=1.6V (N*0.5dBm) |
| --- |

**RF[0Ch] - VCOT Register 1 (VCOT1) (Power on: 00000h)**

```

| 17 IF VCO Band Current Compensation (0=Disable, 1=Enable)
 16 RF VCO Band Current Compensation (0=Disable, 1=Enable)
 15-0 Reserved, program to zero (0) |
| --- |

**RF[0Dh..1Ah] - N/A (Power on: 00000h)**

```

| Not used. |
| --- |

**RF[1Bh] - Test Register 1 (TEST) (Power on: 0000Fh)**

```

| 17-0 This is a test register for internal use only. |
| --- |

**RF[1Ch..1Eh] - N/A (Power on: 00000h)**

```

| Not used. |
| --- |

**RF[1Fh] - Reset Register (Power on: 00001h)**

```

| 17-0 Don't care (writing any value resets the chip) |
| --- |

| DS Wifi Unknown Registers |
| --- |

**480800Ah - W_X_00Ah (R/W)**

```

| 0-15 Unknown (usually zero) |
| --- |

"[bit7 - ingore rx duplicates]" <--- that is NOT correct (no effect).

Firmware writes 0000h to it. That, done many times. So, eventually some bits in
this register are automatically set by hardware in whatever situations,
otherwise repeatedly writing 0000h to it would be kinda useless...?

---

Below Ports W_X_244h and W_X_228h might be related to deciding when to send
multiplay replies...?

**4808244h - W_X_244h (R/W) x ffff [0000] (used by firmware part4)**

Unknown. Seems to be W_IF/W_IE related. Firmware sets Port 4808244h bits 6,7,12
to 1-then-0 upon IRQ06,IRQ07,IRQ12 respectively.

**4808228h - W_X_228h (W) fixx [0000] (used by firmware part4) (bit3)**

Unknown. Firmware writes 8-then-0 (done in IRQ06 handler, after Port 4808244h
access).

---

Below Ports 48081A0h, 48081A2h, 48081A4h are somehow related to BB[02h]...

**48081A0h - W_X_1A0h - (R/W) -933 [0000]**

```

| 0-1 Unknown
 2-3 Always zero
 4-5 Unknown
 6-7 Always zero
 8 Unknown
 9-10 Always zero
 11 Unknown
 12-15 Always zero |
| --- |

Firmware writes values 000h, 823h. Seems to be power-related. The following
experimental code toggles RXTX.ON (RFU.Pin4): "x=0 / @@lop: / [48081A0h]=x /
[4808036h]=0 / x=x XOR 3 / wait_by_loop(1000h) / b @@lop".

Also, writing to port 48081A0h affects ports 4808034h, 480819Ch, 480821Ch, and
48082A2h.

**48081A2h - W_X_1A2h - (R/W) ---3 [0001] (used by firmware part4)**

```

| 0-1 Unknown. Firmware writes values 03h, 01h, and VAR.
 2-15 Always zero |
| --- |

Used in combination with Port 48081A0h, so it's probably power-related, too.

**48081A4h - W_X_1A4h - (R/W) ffff [0000]**

"Rate used when signal test is enabled (0x0A or 0x14 for 1 or 2 mbit)"

(Not too sure if that's correct, there is no visible relation to any rate.)

(This register seems to be R/W only on certain Port 48081A0h settings.)

Unknown. Firmware writes whatever.

---

**4808220h - W_RAM_DISABLE - RAM Control (R/W)**

```

| 0-1 Disable WifiRAM (0=Normal, other=locks memory at 4804000h-5FFFh)
 2-4 Unknown (0=Normal, other=prevents/affects RX to ram?)
 5 Disable Special Log? (0=Normal, 1=Prevent 4805F6Eh..5F77h updates)
 6-15 Unknown (0=Normal, other=?) |
| --- |

**4808290h - W_X_290h - (R/W or Disabled)**

Reportedly, this is the "antenna" register, which should exist on official
devkits, allowing to switch between wired Ethernet, and wireless Wifi mode.

```

| 0 Unknown (R/W) (if present)
 1-15 Not used |
| --- |

On normal NDS release versions, this register seems to be disabled (if it is
implemented at all), and trying to read from it acts as for unused registers,
ie. reads return FFFFh (or probably 0000h on NDS-lite). The NDS firmware
contains code for accessing this port, even in release versions.

**W_INTERNAL**

All registers marked as "W_INTERNAL" aren't used by Firmware part4, and are
probably unimportant, except for whatever special diagnostics purposes.

**Wifi DMA**

Wifi RAM can be accessed with normal "Start Immediately" DMA transfers
(typically by reading through W_RXBUF_RD_DATA, so the DMA automatically wraps
from END to BEGIN).

Additionally, DMA0 and DMA2 can be reportedly synchronized to "Wireless
Interrupt" (rather than using "Start Immediately" timing), no idea if/how
that's working though... and if it gets started on any Wifi IRQ, or only on
specific IRQs...?

Possibly some of the above unknown registers, or some unknown bits in other
registers, are DMA related...?

Reportedly, early firmwares did use "Wireless Interrupt" DMAs (that'd be
firmware v1/v2... or, only earlier unreleased prototype versions?).

| DS Wifi Unused Registers |
| --- |

**Wifi WS0 and WS1 Regions in NDS7 I/O Space**

Wifi hardware occupies two 32K slots, but most of it is filled with unused or
duplicated regions. The timings (waitstates) for WS0 and WS1 are initialized in
WIFIWAITCNT (by firmware).

```

| 4800000h-4807FFFh Wifi WS0 Region (32K) ;used for RAM at 4804000h
 4808000h-4808000h Wifi WS1 Region (32K) ;used for registers at 4808000h
 4810000h-4FFFFFFh Not used (00h-filled) |
| --- |

Structure of the 32K Wifi Regions (WS0 and WS1)

```

| Wifi-WS0-Region Wifi-WS1-Region Content
 4800000h-4800FFFh 4808000h-4808FFFh Registers
 4801000h-4801FFFh 4809000h-4809FFFh Registers (mirror)
 4802000h-4803FFFh 480A000h-480BFFFh Unused
 4804000h-4805FFFh 480C000h-480DFFFh Wifi RAM (8K)
 4806000h-4806FFFh 480E000h-480EFFFh Registers (mirror)
 4807000h-4807FFFh 480F000h-480FFFFh Registers (mirror) |
| --- |

Wifi Registers (recommended 4808000h-4808FFFh) appear more stable in WS1?

Wifi RAM (recommended 4804000h-4805FFFh) appears more stable in WS0?

**Unused Ports (Original NDS)**

Aside from those ports listed in the Wifi I/O Map, all other ports in range
4808000h..4808FFFh are unused. On the original DS, reading from these ports
returns FFFFh.

**Unused Ports (NDS-Lite)**

Reading from unused I/O ports acts as PASSIVE mirror of W_RXBUF_RD_DATA.
Exceptions are: Ports 4808188h, and 48082D8h..48082E6h; which always return
0000h.

**Unused Memory (Original NDS)**

Unused Wifi Memory is at 2000h..3FFFh. On the original DS, reading from that
region returns FFFFh.

**Unused Memory (NDS-Lite)**

Reading from unused memory acts as PASSIVE mirror of WifiRAM (ie. reading from
it returns the value being most recently read from 4000h..5FFFh) (that not
affected by indirect WifiRAM reads via W_RXBUF_RD_DATA) (and, that not affected
by writes to wifi memory, including writes that do overwrite the most recent
read value) (and, that only if WifiRAM is properly enabled, ie. Port
4808220h.Bits0-1 should be 0).

Moreover, certain addresses are additionally ORed with mirrored I/O Ports. That
addresses are:

```

| 2030h, 2044h, 2056h, 2080h, 2090h, 2094h, 2098h, 209Ch, 20A0h, 20A4h,
 20A8h, 20AAh, 20B0h, 20B6h, 20BAh, 21C0h, 2208h, 2210h, 2244h, 31D0h,
 31D2h, 31D4h, 31D6h, 31D8h, 31DAh, 31DCh, 31DEh. |
| --- |

For example, 2044h is a PASSIVE mirror of WifiRAM, ORed with an ACTIVE mirror
of W_RANDOM (Port 044h). Note that some mirrors are at 2000h-2FFFh, and some at
3000h-3FFFh. The W_CMD_STAT mirrors are PASSIVE (that, in unused memory region
only) (in normal port-mirror regions like 1000h-1FFF, W_CMD_STAT mirrors are
ACTIVE).

**Known (W) Mirrors (when reading from Write-only ports)**

```

| Read from (W) Mirrors to (NDS) Or to (NDS-Lite)
 070h W_TXBUF_WR_DATA 060h W_RXBUF_RD_DATA 074h W_TXBUF_GAP
 078h W_INTERNAL 068h W_TXBUF_WR_ADDR 074h W_TXBUF_GAP
 0ACh W_TXREQ_RESET 09Ch W_INTERNAL ? (zero)
 0AEh W_TXREQ_SET 09Ch W_INTERNAL ? (zero)
 0B4h W_TXBUF_RESET 0B6h W_TXBUSY ? (zero)
 158h W_BB_CNT 15Ch W_BB_READ ? (zero)
 15Ah W_BB_WRITE ? (zero) ? (zero)
 178h W_INTERNAL 17Ch W_RF_DATA2 ? (zero)
 20Ch W_INTERNAL 09Ch W_INTERNAL ? (zero)
 21Ch W_IF_SET 010h W_IF 010h-OR-05Ch-OR-more?
 228h W_X_228h ? (zero) ? (zero)
 298h W_INTERNAL 084h W_TXBUF_TIM 084h W_TXBUF_TIM
 2A8h W_INTERNAL 238h W_INTERNAL 238h W_INTERNAL
 2B0h W_INTERNAL 084h W_TXBUF_TIM 084h W_TXBUF_TIM |
| --- |

Notes: The mirror to W_RXBUF_RD_DATA is a passive mirror.

The DS-Lite mirror at 21Ch consists of several ports ORed with each other
(known components are Ports 010h and 05Ch, but there seem to be even more
values ORed with it).

**Port Mirror Regions**

The Wifi Port region at 000h..FFFh is mirrored to 1000h..1FFFh, 6000h..6FFFh,
and 7000h..7FFFh. Many of that mirrored ports are PASSIVE mirrors. Eg. reading
from 1060h (mirror of Port 060h, W_RXBUF_RD_DATA) returns the old
W_RXBUF_RD_DATA value (but without loading a new value from Wifi RAM, and
without incrementing W_RXBUF_RD_ADDR). However, other registers, like W_RANDOM
do have ACTIVE mirrors.

| DS Wifi Initialization |
| --- |

**Initialization sequence**

These events must be done somewhat in sequence. There is some flexibility as to
how they can be ordered but it's best to follow this order:

```

| [4000304h].Bit1 = 1 ;POWCNT2 ;-Enable power to the wifi system
 W_MACADDR = firmware[036h] ;-Set 48bit Mac address
 reg[012h] = 0000h ;W_IE ;-Disable interrupts |
| --- |

Wake Up the wireless system:

```

| reg[036h] = 0000h ;W_POWER_US ;\clear all powerdown bits
 delay 8 ms ; (works without that killer-delay ?)
 reg[168h] = 0000h ;W_BB_POWER ;/
 IF firmware[040h]=02h ;\
 temp=BB[01h] ; for wifitype=02h only:
 BB[01h]=temp AND 7Fh ; reset BB[01h].Bit7, then restore old BB[01h]
 BB[01h]=temp ; (that BB setting enables the RF9008 chip)
 ENDIF ;/
 delay 30 ms ;-(more killer-delay now getting REALLY slow)
 call init_sub_functions ;- same as "Init 16 registers by firmware[..]"
 ; and "Init RF registers", below.
 ; this or the other one probably not necessary |
| --- |

Init the Mac system:

```

| reg[004h] = 0000h - W_MODE_RST ;set hardware mode
 reg[008h] = 0000h - W_TXSTATCNT ;
 reg[00Ah] = 0000h - ? W_X_00Ah ;(related to rx filter)
 reg[012h] = 0000h - W_IE ;disable interrupts (again)
 reg[010h] = FFFFh - W_IF ;acknowledge/clear any interrupts
 reg[254h] = 0000h - W_CONFIG_254h ;
 reg[0B4h] = FFFFh - W_TXBUF_RESET ;--reset all TXBUF_LOC's
 reg[080h] = 0000h - W_TXBUF_BEACON ;disable automatic beacon transmission
 reg[02Ah] = 0000h - W_AID_FULL ;\clear AID
 reg[028h] = 0000h - W_AID_LOW ;/
 reg[0E8h] = 0000h - W_US_COUNTCNT ;disable microsecond counter
 reg[0EAh] = 0000h - W_US_COMPARECNT ;disable microsecond compare
 reg[0EEh] = 0001h - W_CMD_COUNTCNT ;(is 0001h on reset anyways)
 reg[0ECh] = 3F03h - W_CONFIG_0ECh ;
 reg[1A2h] = 0001h - ? ;
 reg[1A0h] = 0000h - ? ;
 reg[110h] = 0800h - W_PRE_BEACON ;
 reg[0BCh] = 0001h - W_PREAMBLE ;disable short preamble
 reg[0D4h] = 0003h - W_CONFIG_0D4h ;
 reg[0D8h] = 0004h - W_CONFIG_0D8h ;
 reg[0DAh] = 0602h - W_RX_LEN_CROP ;
 reg[076h] = 0000h - W_TXBUF_GAPDISP ;disable gap/skip (offset=zero) |
| --- |

Init 16 registers by firmware[044h..063h]

```

| reg[146h] = firmware[044h] ;W_CONFIG_146h
 reg[148h] = firmware[046h] ;W_CONFIG_148h
 reg[14Ah] = firmware[048h] ;W_CONFIG_14Ah
 reg[14Ch] = firmware[04Ah] ;W_CONFIG_14Ch
 reg[120h] = firmware[04Ch] ;W_CONFIG_120h
 reg[122h] = firmware[04Eh] ;W_CONFIG_122h
 reg[154h] = firmware[050h] ;W_CONFIG_154h
 reg[144h] = firmware[052h] ;W_CONFIG_144h
 reg[130h] = firmware[054h] ;W_CONFIG_130h
 reg[132h] = firmware[056h] ;W_CONFIG_132h
 reg[140h] = firmware[058h] ;W_CONFIG_140h
 reg[142h] = firmware[05Ah] ;W_CONFIG_142h
 reg[038h] = firmware[05Ch] ;W_POWER_TX
 reg[124h] = firmware[05Eh] ;W_CONFIG_124h
 reg[128h] = firmware[060h] ;W_CONFIG_128h
 reg[150h] = firmware[062h] ;W_CONFIG_150h |
| --- |

Init RF registers

```

| numbits = BYTE firmware[041h] ;usually 18h
 numbytes = (numbits+7)/8 ;usually 3
 reg[0x184] = (numbits+80h) AND 017Fh -- W_RF_CNT
 for i=0 to BYTE firmware[042h]-1 ;number of entries (usually 0Ch) (0..0Bh)
 if BYTE firmware[040h]=3
 RF[i]=firmware[0CEh+i]
 else
 RF_Write(numbytes at firmware[0CEh+i*numbytes])
 endif |
| --- |

Init the BaseBand System

```

| (this should be not required, already set by firmware bootcode)
 reg[160h] = 0100h ;W_BB_MODE
 BB[0..68h] = firmware[64h+(0..68h)] |
| --- |

Set Mac address

```

| copy 6 bytes from firmware[036h] to mac address at 0x04800018 (why again ?) |
| --- |

Now just set some default variables

```

| reg[02Ch]=0007h ;W_TX_RETRYLIMIT - XXX needs to be set for every transmit?
 Set channel (see section on changing channels)
 Set Mode 2 -- sets bottom 3 bits of W_MODE_WEP to 2
 Set Wep Mode / key -- Wep mode is bits 3..5 of W_MODE_WEP
 BB[13h] = 00h ;CCA operation (use only carrier sense, without ED)
 BB[35h] = 1Fh ;Energy Detection Threshold (ED) |
| --- |

-- To further init wifi to the point that you can properly send

-- and receive data, there are some more variables that need to be set.

```

| reg[032h] = 8000h -- W_WEP_CNT ;Enable WEP processing
 reg[134h] = FFFFh -- W_POST_BEACON ;reset post-beacon counter to LONG time
 reg[028h] = 0000h -- W_AID_LOW ;\clear W_AID value, again?!
 reg[02Ah] = 0000h -- W_AID_FULL ;/
 reg[0E8h] = 0001h -- W_US_COUNTCNT ;enable microsecond counter
 reg[038h] = 0000h -- W_POWER_TX ;disable transmit power save
 reg[020h] = 0000h -- W_BSSID_0 ;\
 reg[022h] = 0000h -- W_BSSID_1 ; clear BSSID
 reg[024h] = 0000h -- W_BSSID_2 ;/ |
| --- |

-- TX prepare

```

| reg[0AEh] = 000Dh -- W_TXREQ_SET ;flush all pending transmits (uh?) |
| --- |

-- RX prepare

```

| reg[030h] = 8000h W_RXCNT ;enable RX system (done again below)
 reg[050h] = 4C00h W_RXBUF_BEGIN ;(example values)
 reg[052h] = 5F60h W_RXBUF_END ;(length = 4960 bytes)
 reg[056h] = 0C00h/2 W_RXBUF_WR_ADDR ;fifo begin latch address
 reg[05Ah] = 0C00h/2 W_RXBUF_READCSR ;fifo end, same as begin at start.
 reg[062h] = 5F60h-2 W_RXBUF_GAP ;(set gap<end) (zero should work, too)
 reg[030h] = 8001h W_RXCNT ;enable, and latch new fifo values to hardware |
| --- |

--

```

| reg[030h] = 8000h W_RXCNT enable receive (again?)
 reg[010h] = FFFFh W_IF clear interrupt flags
 reg[012h] = whatever W_IE set enabled interrupts
 reg[1AEh] = 1FFFh W_RXSTAT_OVF_IE desired STAT Overflow interrupts
 reg[1AAh] = 0000h W_RXSTAT_INC_IE desired STAT Increase interrupts
 reg[0D0h] = 0181h W_RXFILTER set to 0x581 when you successfully connect
 to an access point and fill W_BSSID with a mac
 address for it. (W_RXFILTER) [not sure on the values
 for this yet]
 reg[0E0h] = 000Bh -- W_RXFILTER2 ;
 reg[008h] = 0000h -- ? W_TXSTATCNT ;(again?)
 reg[00Ah] = 0000h -- ? W_X_00Ah ;(related to rx filter) (again?)
 reg[004h] = 0001h -- W_MODE_RST ;hardware mode
 reg[0E8h] = 0001h -- W_US_COUNTCNT ;enable microsecond counter (again?)
 reg[0EAh] = 0001h -- W_US_COMPARECNT ;enable microsecond compare
 reg[048h] = 0000h -- W_POWER_? ;[disabling a power saving technique]
 reg[038h].Bit1 = 0 -- W_POWER_TX ;[this too]
 reg[048h] = 0000h -- W_POWER_? ;[umm, it's done again. necessary?]
 reg[0AEh] = 0002h -- W_TXREQ_SET ;
 reg[03Ch].Bit1 = 1 -- W_POWERSTATE ;queue enable power (RX power, we believe)
 reg[0ACh] = FFFFh -- W_TXREQ_RESET;reset LOC1..3 |
| --- |

That's it, the DS should be now happy to send and receive packets.

It's very possible that there are some unnecessary registers set in here.

| DS Wifi Flowcharts |
| --- |

**Wifi Transmit Procedure**

To transmit data via wifi (Assuming you've already initialized wifi and changed
channels to the channel you want):

```

| (1) Copy the TX Header followed by the 802.11 packet to send anywhere it
 will fit in MAC memory (halfword-aligned)
 (2) Take the offset from start of MAC memory that you put the packet,
 divide it by 2, and or with 0x8000 - store this in one of the
 W_TXBUF_LOC registers
 (3) Set W_TX_RETRYLIMIT, to allow your packet to be retried until an ack is
 received (set it to 7, or something similar)
 (4) Store the bit associated with the W_TXBUF_LOC register you used
 into W_TXREQ_SET - this will send the packet.
 (5) You can then read the result data in W_TXSTAT when the TX is over
 (you can tell either by polling or interrupt) to find out how many
 retries were used, and if the packet was ACK'd |
| --- |

Of course, this is just the simplest approach, you can be a lot more clever
about it.

**Wifi Receive Procedure**

To receive data via wifi, you either need to handle the wifi received data
interrupt, or you need to poll W_RXBUF_WRCSR - whenever it is !=
W_RXBUF_READCSR, there is a new packet. When there is a new packet, take the
following approach:

```

| (1) Calculate the length of the new packet (read "received frame length"
 which is +8 bytes from the start of the packet) - total frame length
 is (12 + received frame length) padded to a multiple of 4 bytes.
 (2) Read the data out of the RX FIFO area (keep in mind it's a circular
 buffer and you may have to wrap around the end of the buffer)
 (3) Set the value of W_RXBUF_READCSR to the location of the next packet
 (add the length of the packet, and wrap around if necessary) |
| --- |

Keep in mind, W_RXBUF_READCSR and W_RXBUF_WRCSR must be multiplied by 2 to get
a byte offset from the start of MAC memory.

**Wifi Change Channels Procedure (ch=1..14)**

For Type2 or Type5 (ie. firmware[040h]<>3): ;(Type2, used in
Original-DS)

```

| RF[firmware[F2h+(ch-1)*6]/40000h] = firmware[F2h+(ch-1)*6] AND 3FFFFh
 RF[firmware[F5h+(ch-1)*6]/40000h] = firmware[F5h+(ch-1)*6] AND 3FFFFh
 delay a few milliseconds ;huh?
 IF RF[09h].bit16=0 ;External Gain (default)
 BB[1Eh]=firmware[146h+(ch-1)] ;set BB.Gain register
 ELSEIF RF[09h].bit15=0 ;Internal Gain from TXVGC Bits
 RF[09h].Bit10..14 = (firmware[154h+(ch-1)] AND 1Fh) ;set RF.TXVGC Bits
 ENDIF |
| --- |

For Type3 (ie. firmware[040h]=3): ;(Type3, used in DS-Lite)

```

| num_initial_regs = firmware[042h]
 addr=0CEh+num_initial_regs
 num_bb_writes = firmware[addr]
 num_rf_writes = firmware[43h]
 addr=addr+1
 for i=1 to num_bb_writes
 BB[firmware[addr]] = firmware[addr+ch]
 addr=addr+15
 next i
 for i=1 to num_rf_writes
 RF[firmware[addr]] = firmware[addr+ch]
 addr=addr+15
 next i |
| --- |

Congrats, you are now ready to transmit/receive on whatever channel you picked.

**Channels**

The IEEE802.11b standard (and the NDS hardware) support 14 channels (1..14).

Channels 1..13 use frequencies 2412MHz..2472MHz (in 5MHz steps). Channel 14
uses frequency 2484MHz. Which channels are allowed to be used varies from
country to country, as indicated by Bit1..14 of firmware[03Ch]. Channel 14 is
rarely used (dated back to an older japanese standard).

Caution: Nearby channels do overlap, you'll get transmission errors on packets
that are transferred simultaneously with packets on nearby channels. But, you
won't successfully receive packets from nearby channels (so you won't even
"see" that they are there, which is bad, as it doesn't allow you to share the
channel synchronized with other hosts; ie. it'd be better if two hosts are
using the SAME channel, rather than to use nearby channels).

To avoid that problem, conventionally only channels 1,6,11 are used - however
Nintendo uses channels 1,7,13 - which is causing conflicts between channel 6,7,
and maybe also between 11,13.

| DS Wifi Hardware Headers |
| --- |

**Hardware TX Header (12 bytes) (TXHDR)**

The TX header immediately precedes the data to be sent, and should be put at
the location that will be given to the register activating a transmission.

```

| Addr Siz Expl.
 00h 2 Status - In: Don't care - Out: Status (0000h=Failed, 0001h=Okay)
 0000h=Retrying? (TXBUF_LOCn)
 0001h=Okay (TXBUF_LOCn,TXBUF_BEACON,TXBUF_CMD)
 xx01h=Okay (TXBUF_REPLY, with increasing "xx")
 0003h=Failed (TXBUF_LOCn)
 0005h=Failed (TXBUF_CMD, with errorflags in TXHDR[2])
 02h 2 Unknown - In: Don't care
 Bit0: Usually zero.
 Bit1..15 --------> flags for multiboot slaves number 1..15
 (Should be usually zero, except when sending multiplay commands
 via W_TXBUF_CMD. In that case, the slave flags should be ALSO
 stored in the second halfword of the FRAME BODY. Actually, the
 hardware seems to use only that entry (in the BODY), rather than
 using this entry (in the hardware header)).
 04h 1 Unknown - In: Must be 00h..02h (should be 00h)
 00h = use W_TX_SEQNO (if enabled in TXBUF_LOCn)
 01h = force NOT to use W_TX_SEQNO (even if it is enabled in LOCn)
 02h = seems to behave same as 01h
 03h..FFh = results in error: W_TXSTAT.Bit1 gets set (though
 header entry[00h] is kept set to 0001h=Okay)
 other theory: maybe an 8bit retry count with 00h=first try?
 05h 1 Unknown - In: Don't care - Out: Set to 00h
 06h 2 Unknown - In: Don't care
 08h 1 Transfer Rate (0Ah=1Mbit/s, 14h=2Mbit/s) (other values=1MBit/s, too)
 09h 1 Unknown - In: Don't care
 0Ah 2 Length of IEEE Frame Header+Body+checksum(s) in bytes
 (14bits, upper 2bits are unused/don't care) |
| --- |

The eight "Don't care" bytes should be usually set to zero (although setting
them to FFh seems to be working as well). Entries [00h] and [05h] are modified
by hardware, all other entries are kept unchanged.

Important note! TX length includes the length of a 4-byte "FCS" (checksum) for
the packet. The hardware generates the FCS for you, but you still must include
it in the packet length. Also note that if the 802.11 WEP enabled bit is set in
the header, the packet will be automatically encrypted via the wep algorithm -
however, the software is responsible for providing the 4-byte IV block with the
WEP key ID and the 24bit IV value. - ALSO, you must include the length of the
*encrypted* FCS used in packets that have wep enabled (increase the tx length
by another 4 bytes) - this value is calculated automaticly for you, but you are
responsible for including it in the length of your packet (if you have data
there, it'll be replaced by the FCS.)

**Hardware RX Header (12 bytes) (RXHDR)**

The RX header is an informational structure that provides needed information
about a received packet. It is written right before the received packet data in
the rx circular buffer.

```

| Addr Siz Expl.
 00h 2 Flags
 Bit0-3: Frame type/subtype:
 00h: managment/any frame (except beacon and invalid subtypes)
 01h: managment/beacon frame
 05h: control/ps-poll frame
 08h: data/any frame (subtype0..7) (ie. except invalid subtypes)
 Values 0Ch..0Fh are for Multiplay cmd/reply packets:
 0Ch: CMD frame ;FC=0228h=Data, FromDS, Data+CF_Poll
 0Dh: CMD ack frame ;FC=0218h=Data, FromDS, Data+CF-Ack
 0Eh: REPLY frame (data) ;FC=0118h=Data, ToDS, Data+CF-Ack
 0Fh: REPLY frame (empty) ;FC=0158h=Data, ToDS, CF_Ack
 0Fh: Also ALL empty packets (raw IEEE header, with 0-byte body)
 Bit4: Seems to be always set
 Bit5-7: Seems to be always zero
 Bit8: Set when FC.Bit10 is set (more fragments)
 Bit9: Set when the lower-4bit of Sequence Control are nonzero,
 it is also set when FC.Bit10 is set (more fragments)
 So, probably, it is set on fragment-mismatch-errors
 Bit10-14: Seems to be always zero
 Bit15: Set when Frame Header's BSSID value equals W_BSSID register
 02h 2 Unknown (0040h=Normal, 0440h=WEP?, or Unchanged for multiplay CMDs?)
 04h 2 Unchanged (not updated by hardware, contains old Wifi RAM content)
 06h 2 Transfer Rate (N*100kbit/s) (ie. 14h for 2Mbit/s)
 08h 2 Length of IEEE Frame Header+Body in bytes (excluding FCS checksum)
 0Ah 1 MAX RSSI (bit0=always 1?) ;\Received Signal Strength Indicator
 0Bh 1 MIN RSSI ;/ |
| --- |

Important Note: Received frame lengths are always multiples of 4 bytes. While
the actual header length + received frame length may be less, when incrementing
the read cursor you must pad the length to a multiple of 4 bytes.

**IEEE Header**

The above Hardware headers should (must) be followed by valid IEEE headers.
Although that headers are to be generated by software, the hardware does do
some interaction with the IEEE headers, such like comparing address fields with
W_MACADDR and W_BSSID. And, it does modify some entries of it:

1) The sequence control value is replaced by W_TX_SEQNO*10h (when enabled in
W_TXBUF_LOCn.Bit13), this replacement does also overwrite the local TXBUF
value.

2) The frame control value is modified, namely, the hardware tends to set Bit12
of it. This replacement does NOT modify the local TXBUF, but the remote RXBUF
will receive the modified value. Also, Bit0-1 (protocol version) are forcefully
set to 0.

3) Transmits via W_TXBUF_BEACON do additionally modify the 64bit timestamp (so
W_TXBUF_BEACON should be used ONLY for packets WITH timestamp, ie. Beacons or
Probe-Responses). The local TXBUF seems to be left unchanged, but the remote
RXBUF will contain the (sender's) W_US_COUNT value.

C) For Control Frames, the hardware headers Length value is transferred as
normally (ie. excluding the FCS length, remote RXBUF will contain TXBUF length
minus 4), but - no matter of that length value - only 10 or 16 bytes (depending
on the subtype) of the IEEE frame are actually transferred and/or stored in
RXBUF.

X) For Control Frames with Subtype 0Ah, the AID entry is set to C000h, that,
probably ORed with original value in WifiRAM, or with the W_AID_FULL register?

XX) No idea if it's possible to send Control Frames with subtype 0Bh..0Fh, as
for now, it seems that either they aren't sent, or the receipient is ignoring
them (or processing them internally, but without storing them in RXBUF).

**RSSI (Received Signal Strength Indicator)**

The MAX RSSI is decoded as so:

```

| arm7_xlat_rssi_r0: ;bit0=unused, bit1=extraflag, bit2-7=unsigned value
 tst r0,2h ;bit1
 mov r0,r0,asr 2h ;div4 (sign bits in bit31-8 are always zero-expanded)
 addeq r0,r0,19h ;add extra constant... when bit1=0 !!!!!!
 and r0,r0,0FFh ;blah (max is FFh/4+19h)
 bx r14 |
| --- |

The MIN RSSI value is unknown and not used by existing software (it's usually
smaller than the 8bit MAX RSSI value, though that value is actually 6bit+1bit,
so it's hard to say what is bigger/smaller), actually MIN RSSI might be
something different like SNR (Signal to Noise Ratio).

| DS Wifi Nintendo Beacons |
| --- |

Beacons are most commonly used to announce Wifi Internet Access Points. The
special Nintendo Beacons are used to announce Local Network games (randomly
broadcast on channel 1, 7, or 13).

**All beacon frames transmitted by a DS host have the following format**

```

| TXHDR using 2MBit/s rate
 802.11 Management frame header
 802.11 Beacon header (Timestamp, BeaconInterval=00xxh, Capability=0021h)
 Supported rates (Tag=01h, Len=02h, 82h,84h) ;1Mbit/s and 2Mbit/s
 Distribution Channel (Tag=03h, Len=01h, 0xh) ;channel 1, 7, or 13
 TIM vector (Tag=05h, Len=05h, 00h,02h,0,0,0) ;adjusted by hardware
 Custom extension (Tag=DDh, Len=18h+N, see below) |
| --- |

**Nintendo specific beacon info (information element Tag=DDh)**

```

| 00h 4 Nintendo Beacon OUI (00h,09h,BFh,00h)
 04h 2 Stepping Offset for 4808134h/W_POST_BEACON (always 000Ah)
 06h 2 LCD Video Sync in 15.625Hz units (VCOUNT*7Fh-W_US_COUNT*2)/80h
 08h 4 Fixed ID (00400001h)
 0Ch 4 Game ID (0040xxxxh) (or 00000857h=Nintendo Zone)
 10h 2 Randomly generated Stream code (0000h..FFFFh?)
 12h 1 Number of bytes from entry 18h and up (70h for multiboot) (0 if Empty)
 13h 1 Beacon Type (1=Multicart/Pictochat, 9=Empty, 0Bh=Multiboot, ?=Zone)
 14h 2 CMD data size (01FEh = FFh halfwords) (or 0100h)
 16h 2 REPLY data size (0008h = 4 halfwords) |
| --- |

For Empty (length zero, may occur at very begin of multiboot)

```

| 18h - Nothing, no data |
| --- |

For Multicart (variable length)

```

| 18h .. Custom data, usually containing the host name, either in 8bit ascii,
 or 16bit unicode format. Sometimes taken from Firmware User Settings,
 and sometimes from Cartridge Backup Memory. |
| --- |

For Pictochat (length 8)

```

| 18h 2 Fixed (always 2348h)
 1Ah 2 Unknown xxxx
 1Ch 1 Chatroom number (00h..03h for Chatroom A..D)
 1Dh 1 Number of users already connected (1..16, including master)
 1Eh 2 Fixed (always 0004h) |
| --- |

For Multiboot (aka DS Download Play) (always 70h bytes)

```

| 18h 4 Game ID (0040xxxxh) (same as [0Ch]) (varies from game to game)
 1Ch 1 Last Snippet flag (00h=Snippet #0..8, 02h=Snippet #9)
 1Dh 1 Session Number (00h=First) (increments when restarting a new upload)
 1Eh 1 Number of slaves already connected (0..15, excluding master)
 Uh, actually [1Eh] is always 01h, even for 2..3 slaves?
 1Fh 1 Snippet number (0..9=Snippet #0..9)
 20h 2 Checksum (on entries [22h..87h])
 chksum=0, for i=22h to 86h step 2, chksum=chksum+halfword[i], next i,
 chksum=FFFFh AND NOT (chksum+chksum/10000h)
 22h 1 In Snippet #0..8: Snippet number (0..8, same as [1Fh])
 In Snippet #9: Number or players connected (0..16, may include master)
 23h 1 Highest Snippet number (09h=Snippet #9)
 24h 2 In Snippet #0..8: Snippet Size in bytes (62h for #0..7, 48h for #8)
 In Snippet #9: Player Mask (eg. 000Fh=4-Players, including Master)
 26h 62h Snippet Data (always 62h bytes, zeropadded if size<62h) |
| --- |

For Nintendo Zone (with Game ID=00000857h) (always 70h bytes):

```

| 18h 70h Encrypted Nintendo Zone Beacon Info (see below) |
| --- |

**Multiboot Advertisement Snippets**

Snippet #0..8 are merged to form a larger datablock with following info:

```

| 000h 32 Icon Palette (same as for ROM Cartridge Icon)
 020h 512 Icon Bitmap (same as for ROM Cartridge Icon)
 220h 1 Favorite color (00h..0Fh) ;\
 221h 1 Username Length (0..10) ; from firmware user settings
 222h 20 Username (max 10 chars UCS-2) ;/
 236h 1 Max number of players (1..16, may include master)
 237h 1 Unknown (00h)
 238h 96 Game name (48 UCS-2) (same as 1st line of ROM Cartridge Title)
 298h 192 Description (96 UCS-2) (same as further lines of ROM Cart Title)
 358h 26 Unused (padding 48h-byte Snippet #8 to 62h-byte size) |
| --- |

Snippet #9 contains slave names:

```

| 000h 2 Slave Mask (eg. 000Eh for three slaves) (initially 0000h, see note)
 002h 1 Slave 1 Number*10h+Color (10h+color) ;\
 003h 1 Slave 1 Username Length (0..10) ; Slave 1 (if any)
 004h 14h Slave 1 Username (max 10 chars UCS-2) ;/
 018h 1 Slave 2 Number*10h+Color (20h+color) ;\
 019h 1 Slave 2 Username Length (0..10) ; Slave 2 (if any)
 01Ah 14h Slave 2 Username (max 10 chars UCS-2) ;/
 01Eh 1 Slave 3 Number*10h+Color (30h+color) ;\
 01Fh 1 Slave 3 Username Length (0..10) ; Slave 3 (if any)
 020h 14h Slave 3 Username (max 10 chars UCS-2) ;/
 ... |
| --- |

Note: The slave Mask is initially 0000h, and isn't updated until AFTER having
transferred the first some Data packets.

Unknown if there can be more 62h-byte snippets appended (if any games support 5
or more slaves).

**Authentication/Association process**

Connecting to a Host console is done via standard 802.11 managment frames. The
Auth/Assoc Requests must be sent during the listen interval (ie. shortly after
having received beacons).

```

| Host sends beacon(s)
 Client sends an Authentication Request (AuthSeq=1) ;\Auth
 Host sends an Authentication Response (AuthSeq=2) ;/
 Host sends beacon(s)
 Client sends an Association Request (with special SSID) ;\Assoc
 Host sends an Association Response ;/
 Host sends beacon(s) and begins sending CMD/Data packets |
| --- |

The Nintendo Beacons are transferred without public SSID (without Tag=00h).
However, the Association Request must contain a SSID, which is derived from the
custom Tag=DDh area.

```

| SSID for multiboot (20h-byte binary, non-ASCII):
 00h 4 Game ID (0040xxxxh) (from Beacon Tag=DDh, entry 0Ch)
 04h 2 Stream code (from Beacon Tag=DDh, entry 10h)
 06h 1Ah Zerofilled |
| --- |

Multicart games or Pictochat may use whatever SSID's they want (unknown if
there is standard for that).

**Encrypted Nintendo Zone Beacon Info (with Game ID=00000857h)**

Nintendo Zone beacons are broadcast by DS consoles with special "Beacon
Cartridges" (that console is merely sending beacons, it doesn't act as an
Access Point itself. Instead, the beacon contains the SSID/password for the
actual Access Point in the Shop/Restaurant, or for your own home network).

The SSID/Password in the Zone Beacons can be also used by DSi games and DSi
browser (depending on the Flags at [66h]).

```

| 00h 32 Access Point SSID (ASCII, zeropadded if shorter than 32 chars)
 20h 10 Server ApNum (ASCII, ten digits/chars...?)
 2Ah 2 Unknown (0001h)
 2Ch 24 Server? Retailer ID string? (eg. "ShopName Country") (purpose=?)
 44h 32 Access Point WEP key (0/5/13/16 bytes) or WPA/WPA2 password
 64h 1 Unknown (00h for DSi, other values for 3DS)
 65h 1 Access Point WEP mode (0=Open, 1/2/3=5/13/16 bytes)
 (only on 3DS?: 4=WPA-TKIP, 5=WPA2-TKIP, 6=WPA-AES, 7=WPA2-AES)
 66h 2 Flags (0003h)
 Bit0: Enable Nintendo Zone content on DS(i)
 Bit1: Enable Online Gaming and Friend list
 Bit4: Enable Nintendo Zone Viewer on 3DS
 Bit7: Block Nintendo eShop
 Bit8: Block Internet Browser
 68h 4 Unknown (uh, maybe zero?)
 6Ch 2 Unknown (0428h)
 6Eh 2 CRC16 with initial value 0 across [00h..6Dh] (or optionally 0=NoCrc) |
| --- |

The above 70h-bytes are RC4 encrypted (the WEP encryption algorithm) with an
8-byte key:

```

| Key[0..3] = "!SDW" (aka "WDS!" backwards)
 Key[4..7] = Last 4 bytes of the 6-byte BSSID from Beacon header |
| --- |

When receiving the Nintendo Zone beacon, DSi Launcher will add some animation
to the Nintendo Zone icon. The Nintendo Zone tool will occassionally show a
green signal strength symbol in upper right (if the SSID and WEP password match
for your Access Point). Moreover, the tool should then connect to the server
(supposedly using the ApNum entry), but that step seems to fail, probably
because Nintendo has shut down the Zone servers someday in 2016-2018.

ApNum:

"Authentication parameter, required for connecting to the server. server uses
this to determine which third-party content to link to on the index page. First
ASCII number char in this param is region, this is also used to determine which
server to connect to. Regions: 0=JP, 1=USA, 2/3=EUR, 4=KOR, 5=China, ?=TWN."
Uh, unknown if those "digits" are letters or digits? There is a screenshot
showing "1BBUSMN000" for Bestbuy. The 3DS has a hotspot.conf file with numeric
ApNums, eg. "2013300000". Unknown if the region digit is required to match the
console region.

| DS Wifi Nintendo DS Download Play |
| --- |

DS Download Play allows to download small games from other NDS/DSi consoles.
This is also known as Wireless Multiboot (WMB), or Single Gamecart Multiplay.

Before downloading: Receive beacons, display Icon/Title, prompt user whether to
download that title, and, if so, connect to host (via authentication and
association requests). See previous chapter for details.

**Download process (after authentication/association)**

```

| Host sends NameRequests, client(s) send UsernameReply
 Host sends RSA frame, client(s) send RsaReply
 Host sends Data (Header/ARM9/ARM7 binaries), client(s) send DataReply
 Host sends Data, if it's the last packet, client(s) send GotAllReply
 Host sends Final message, client(s) send FinalReply
 Client(s) send Deauthentication with Reason=3 and jump to entrypoints |
| --- |

The whole transfer is done at 2MBit/s, without any WEP/WPA encryption, and
without ARM9 Secure Area encryption.

**CMD DATA, Host-to-client packets (with DA = 03:09:BF:00:00:00)**

Sent via 4808090h/W_TXBUF_CMD (multiplayer master)

```

| 00h 2 Value for W_CMD_REPLYTIME (0106h)
 02h 2 Slave Flags, bit1..15 for slave 1..15 (1=connected) (eg. 0002h)
 04h 1 Size in halfwords of Command+Data (ie. [06h..end, excluding footer])
 05h 1 Flags (11h=Normal, 01h=Footerless/Can be ignored, 00h=Deauth'ed?)
 06h 1 Command (01h=NameRequest, 03h=RSA, 04h=DataPacket, 05h=Done)
 For Command 00h (Dummy, NameDone/RsaDone/DataDone): ;\
 07h 5 Unused (zerofilled) ;
 For Command 01h (NameRequest): ;
 07h 5 Unused (zerofilled) ;
 For Command 02h (Error: Wrong Game Serial): ;
 07h 5 Unused (zerofilled) ; Data
 For Command 03h (RSA): ;
 07h E4h RSA Signature Frame (see below) ;
 EBh 5 Unused (zerofilled) ;
 For Command 04h (Data Packet): ;
 07h 2 Unknown (zero) ;
 09h 2 Packet Number (0=Header, 1..N=ARM9, N+1..Last=ARM7) ;
 0Bh .. Data (1F8h bytes, or less for Header, or end of ARM7/ARM9) ;
 xxh 1 Unused (zero) ;
 For Command 05h (Final): ;
 07h 5 Unused (garbage, same as last data command) ;
 For Command 06h (Unknown): ;normally not used, but ds download play ;
 07h .. Unknown ;supports cmd 02h,03h,06h after username ;/
 Extra Footer (usually present, unless Flags [05h].bit4=0): ;\
 xxh 2 Slave Flags, again? (0002h) ;or 0000h ? ; Footer
 Note: The footer is usually present in most or all packets ;
 (Super Mario 64 DS has some dummy packets without footer) ;/ |
| --- |

The packet number (and info from RSA frame) allow to compute the packet
destination address. That max packet size is 1F8h bytes (at least so when using
default Tag=DDh [14h]=1FEh).

The transfer is usually done with increasing packet numbers. However, there may
be lost or duplicated packets. And the uploader tends to do weird things like
starting with packet 0001h instead of 0000h, and once or then inserting packet
0000h (or other unrequested packets) during transfer.

Essentially, the downloader should maintain a flag array to know which packets
were already received, and which ones still need to be requested in Data
Replys. The uploader should ignore slightly outdated replies (keep sending the
next packet if the wanted packet number is same as most recently transferred
one).

**REPLY, Client to Host Replies (with DA = 03:09:BF:00:00:10)**

Sent via 4808094h/W_TXBUF_REPLY1 (multiplayer slave).

```

| 00h 1 Size in halfwords (of [02h..end])? (00h=Short, or 04h=Normal)
 01h 1 Flags? (00h, 01h, 81h, or 80h)
 Short Dummy Reply:
 02h - Nothing (can occur once or then during username or data transfer)
 Normal Dummy Reply:
 02h 1 Reply Type (00h=Dummy) (can occur before username transfer)
 03h 7 Unused (zerofilled)
 Username Reply:
 02h 1 Reply Type (07h=Username)
 03h 1 Username snippet number (00h..04h=Snippet #0..4)
 04h 6 Snippet 0: Game ID (0040xxxxh), Favorite Color, UsernameLength(0-10)
 04h 6 Snippet 1: Username Char[0,1,2]
 04h 6 Snippet 2: Username Char[3,4,5]
 04h 6 Snippet 3: Username Char[6,7,8]
 04h 6 Snippet 4: Username Char[9], 0001h/0002h, C500h/0000h
 (There can be more (smaller) snippets if Tag=DDh [16h] is smaller than 8)
 Rsa Reply:
 02h 1 Reply Type (08h=RsaReply)
 03h 7 Unused (garbage, usually same as Username Snippet #2)
 Data Reply:
 02h 1 Reply Type (09h=DataReply)
 03h 2 Next wanted packet number (smallest missing packet number)
 05h 2 Number of different packets received (0001h..Total-1)
 07h 3 Unused (zerofilled)
 Got All Reply:
 02h 1 Reply Type (0Ah=GotAllReply, no further packets needed)
 03h 7 Garbage (old values from last Data Reply)
 Final Reply:
 02h 1 Reply Type (0Bh=FinalReply, confirms the FinalCommand)
 03h 7 Garbage (old values from last Data Reply) |
| --- |

Ideally, software should update the reply immediately after receiving the
command (within a few hundred clock cycles) (when failing to write
W_TXBUF_REPLY1 in time, the hardware does reportedly generate a completely
Empty Dummy Reply with 0-byte data, and FC=0158h).

**CMD ACK, Host to client acknowledgements (with DA = 03:09:BF:00:00:03)**

Generated automatically by hardware (after sending W_TXBUF_CMD and after
receiving Replies) (DA seems to be fully hardcoded in hardware, the other IEEE
header fields might be derived from preceeding RX/TX packets and/or from
hardware registers).

```

| 00h 2 Unknown/random? (eg. 0046h or 001Bh)
 02h 2 Error Flags (bit1-15=No reply from Slave 1-15, eg. when AID_LOW=0) |
| --- |

Note: The SEQNO value in the IEEE header of CMD ACK must be reportedly one
higher than in the corresponding CMD packet (the NDS hardware is doing that
automatically, but one may need to take care of it on non-NDS hardware).

**RSA signature frame payload (Command 03h)**

"Notably: the RSA frame itself is not included as part of the data being
signed, bringing up various security issues and making Nintendo's firmware
engineers look amateurish at best."

```

| 00h 4 ARM9 Entrypoint (usually 20008xxh)
 04h 4 ARM7 Entrypoint (usually 2380000h) (or WRAM?)
 08h 4 Zerofilled
 0Ch 4 Header Destination (temp) (usually 27FFE00h)
 10h 4 Header Destination (actual) (usually 27FFE00h)
 14h 4 Header Size (160h)
 18h 4 Zerofilled
 1Ch 4 ARM9 Destination (temp) (usually 2000000h)
 20h 4 ARM9 Destination (actual) (usually 2000000h)
 24h 4 ARM9 Size
 28h 4 Zerofilled
 2Ch 4 ARM7 Destination (temp) (usually 22C0000h in Main RAM)
 30h 4 ARM7 Destination (actual) (usually 2380000h) (or WRAM?)
 34h 4 ARM7 Size
 38h 4 Unknown (00000001h)
 3Ch 4 Signature ID (61h,63h,01h,00h) (aka "ac", or backwards "ca") ;\
 40h 80h Signature RSA (RSA signature in OpenPGP SHA1 format) ;
 C0h 4 Signature Seed (same as value used to create RSA signature) ;/
 (Nanostray:22AA9FC2h, Hedge:24272349h)
 (Eragon:2512EE7Ah, TableHockey:02704DF6h)
 C4h 20h Zerofilled (stored together with above in E4h-byte array) |
| --- |

The temp/actual destination addresses are usually identical, except for ARM7
which is loaded to a separate temp address in Main RAM (needed there for
computing its SHA1 on ARM9 side).

The RSA signature is as so: First compute SHA1's on Header and ARM9 and ARM7
areas, and store them in a 40h-byte array:

```

| 00h 14h SHA1 on Header
 14h 14h SHA1 on ARM9 bootcode
 28h 14h SHA1 on ARM7 bootcode
 3Ch 4 Signature Seed (same as the four bytes from [C0h]) |
| --- |

Then compute a SHA1 on that 40h-byte array, that SHA1 value is then used in the
RSA signature (in OpenPGP SHA1 format, ie. same as SWI 23h in DSi BIOS). The
RSA private key is unknown, the RSA public key (9Eh,C1h,CCh,C0h,...) can be
found in the NDS Firmware and in the DSi's DS Download Play utility.

Unknown if the 4-byte Seed value is having any special meaning (eg. it might be
some checksum, or it might be just some random, timestamp, or dummy
expiredate?).

The 4+80h+4 signature bytes at [3Ch..C3h] are conventionally stored at the
"end" of the binary, ie. at the offset specified in cartheader[080h].

**160h-byte Header**

Some Download Play titles have faulty header entries (eg. Table Hockey Tech
Demo, with nonsense Icon offset etc). In such cases, dumps are conventionally
having a manually repaired header & an extra copy of the original header:

```

| 0000h 160h Repaired Header (address/size/entrypoint for ARM9,ARM7,Icon,RSA)
 0160h A0h Zerofilled
 0200h 10h ID "DS DOWNLOAD PLAY" (8bit characters)
 0210h 10h ID "----------------" (8bit characters)
 0220h 160h Original Header (to be uploaded for RSA check)
 0380h 10h ID "----------------" (8bit characters)
 0390h 10h ID "Nintendo" (16bit characters)
 03A0h 10h ID Zerofilled
 03B0h 10h ID "----------------" (8bit characters)
 03C0h .. Zerofilled |
| --- |

The 160h-byte Header itself is about same as in ROM-images. The Icon/Title
entry contains the information that is broadcast in Beacons. The Total Size
entry points to the 88h-byte RSA Signature. The File/Overlay entries contain
unused/dummy values.

**Beacons**

Beacons should be kept sent during transfer, however, the DS Download tool
throws a Deauth message when receiving more than 15 beacons with data traffic
(ie. one must pause uploading at least each 16th beacon).

Also, the download should try to sync itself to the LCD Video Sync value in the
Beacon (supposedly interpreted relative to the IEEE Beacon Timestamp) (with
small uploads, there's probably not enough time to complete the
synchronization, so the uploaded executable may still have to finish the
synchronization by itself).

**Entrypoint/Memory**

The Entrypoints are supposedly taken from the RSA frame (not from the 160h-byte
header)? There is some error checking on the RSA frame addresses (eg. ARM7 area
must exist and must apparently point to RAM or WRAM). Additionally, there is
some special info in RAM:

```

| 27FFC40h 2 Boot Indicator (2=Booted from DS Download Play via Wifi)
 27FFC42h 2 Beacon Size (0060h, based on Tag=DDh size: "(88h-08h+41h)/2")
 27FFC44h 2 Looks like RSSI Signal Strength AND FEh (eg. 12h=Low, 82h=High)
 27FFC46h 6 BSSID (master's MAC address)
 27FFC4Ch 2 SSID Size? (0020h)
 27FFC4Eh 4 SSID Game Serial ID (0040xxxxh)
 27FFC52h 2 SSID Random Stream ID (xxxxh)
 27FFC54h 1Ah SSID Padding? (zerofilled)
 27FFC6Eh 2 Beacon Capabilities (0021h)
 27FFC70h 2 Beacon Rates (bit0/1 = 1/2Mbit/s) (0003h)
 27FFC72h 2 Beacon Rates (same as above) (0003h)
 27FFC74h 2 Beacon Interval (00Cxh..00Dxh)
 27FFC76h 2 Beacon TIM ListenInt (0002h)
 27FFC78h 2 Beacon Channel (1, 7, or 13)
 27FFC7Ah 04h Unknown (zerofilled)
 27FFC7Eh 2 Unused? (zero) (not part of 3Ch-bytes at 27FFC42h) |
| --- |

Multiplayer games can use that info for communicating with each other after the
upload. The player number (needed for more than 2 players) doesn't seem to be
included in that structure, it might be still in W_AID_LOW though?

**List of DS Download Play Games**

Games with DS Download Play Support (single-cart multiplayer):

```

| Game ID Players Title
 00400136h 2 Over the Hedge (download contains a 2D minigame)
 00400052h 2 Nanostray
 00400011h 1-4 Super Mario 64 DS
 etc. (unknown if/which games support more than 4 players) |
| --- |

Demos available from DS Download Stations (or was that pre-Download Station?):

```

| Eragon
 Lara Croft Tomb Raider Legend
 Magnetica
 Metroid Prime Hunters Demo
 Submarine Tech Demo
 (and many trailers with non-playable movie clips) |
| --- |

Download Station (in shops):

```

| Allows to download some bootmenu, offering downloading demos/trailers? |
| --- |

**Exploits (not sure what they do)**

```

| WifiMe - downloader patch for NDS firmware without RSA check?
 Unlaunch.dsi v2.1 - can patch RSA check in DSi's DS Download Play
 HaxxStation - uploader patch for Download Station to upload homebrew/hacks? |
| --- |

Unknown if unchecked entries in RSA frame can be also used to crash/boot?

Unknown if public RSA key can be found in gigaleak or so?

**NDS/DSi Games with DS Download Play support**

```

| 100 Classic Books
 Absolute Chess (Nintendo DSiWare)
 Absolute Reversi (Nintendo DSiWare)
 Advance Wars: Dual Strike
 Age Of Empires: Mythologies
 America's Test Kitchen: Let's Get Cooking
 Animaniacs: Lights, Camera, Action!
 Arkanoid DS
 Art Academy
 Atari Greatest Hits Volume 1
 Atari Greatest Hits Volume 2
 Avatar, The Last Airbender: Into The Inferno
 Bakugan: Defenders Of The Core
 Battle Of Giants: Dinosaurs
 Battle Of Giants: Dragons
 Battle Of Giants: Mutant Insects
 Beyblade: Metal Fusion
 Big Bang Mini
 Big Brain Academy
 Bleach: Blade Of Fate, The
 Bleach: Dark Souls
 Boing! Docomodake DS
 Bomberman
 Bomberman Land Touch!
 Bomberman Land Touch! 2
 Boogie
 Bookworm (retail version)
 Brain Age: Train Your Brain In Minutes A Day!
 Brain Age 2: More Training In Minutes A Day!
 Brain Assist
 Brain Voyage
 Bratz: Forever Diamondz
 Break 'Em All
 Burnout Legends
 Bust-A-Move DS
 Call Of Duty 4: Modern Warfare
 Candace Kane's Candy Factory
 Carnival Games
 Cars 2
 Cars Mater-National
 Cars Race-O-Rama
 Cartoon Network Racing
 Chameleon
 Cheetah Girls, The: Passport To Stardom
 Chessmaster: The Art Of Learning
 Classic Word Games
 Club Penguin: Elite Penguin Force
 Club Penguin: Elite Penguin Force - Herbert's Revenge
 Clubhouse Games
 Clubhouse Games Express: Card Classics (Nintendo DSiWare)
 Clubhouse Games Express: Family Favorites (Nintendo DSiWare)
 Clubhouse Games Express: Strategy Pack (Nintendo DSiWare)
 Cookie & Cream
 Cooking Mama
 Cooking Mama 2: Dinner With Friends
 Cooking Mama 3: Shop & Chop
 C.O.R.E.
 Corvette Evolution GT
 Cosmo Fighters (Nintendo DSiWare)
 Crafting Mama
 Crash Boom Bang!
 Custom Robo Arena
 Dairojo! Samurai Defenders (Nintendo DSiWare)
 Deca Sports DS
 Diary Girl
 Diddy Kong Racing DS
 Dino Master: Dig, Discover, Duel
 DiRT 2
 Disney Fairies: Tinker Bell
 DK Jungle Climber
 Dokapon Journey
 Draglade
 Dragon Ball Z: Harukanaru Densetsu
 Dragon Booster
 Dragon Quest Heroes: Rocket Slime
 Dreamer: Pop Star
 Dropcast
 Eco-Creatures: Save The Forrest
 Elite Beat Agents
 Everyday Soccer (Nintendo DSiWare)
 Ferrari Challenge: Trofeo Pirelli
 FIFA Soccer 06
 FIFA Soccer 08
 FIFA Soccer 09
 FIFA Soccer 10
 FIFA Street 3
 FIFA World Cup 06
 Final Fantasy Fables: Chocobo Tales
 Flash Focus: Vision Training In Minutes A Day
 Foto Frenzy
 Freedom Wings
 Fritz Chess
 Frogger: Helmet Chaos
 Gardening Mama
 Gauntlet
 Geometry Wars: Galaxies
 Godzilla: Unleashed Double Smash
 Golden Compass, The
 GoldenEye: Rogue Agent
 Grease
 GRID
 Gunpey DS
 Harry Potter And The Deathly Hallows, Part 1
 Harry Potter And The Deathly Hallows, Part 2
 Harry Potter And The Order Of The Phoenix
 Harvest Moon: Frantic Farming
 Hasbro Family Game Night
 Hearts Spades Euchre (Nintendo DSiWare)
 Heavy Armor Brigade
 High School Musical 2: Work This Out!
 Homie Rollerz
 Igor: The Game
 Imagine Babysitters
 Imagine Ballet Star
 Imagine Ice Champions
 Imagine Rock Star
 Imagine Teacher
 Intellivision Lives!
 Ivy The Kiwi? (retail version)
 Jeopardy
 Jewel Quest Expeditions
 Jumble Madness
 Kirby: Squeak Squad
 Kirby Super Star Ultra
 Konami Classics Series: Arcade Hits
 Labyrinth
 Learn Science
 Left Brain Right Brain
 Left Brain Right Brain 2
 Legend Of Zelda, The: Phantom Hourglass
 Legend Of Zelda, The: Spirit Tracks
 Legendary Starfy, The
 LEGO Star Wars II: The Original Trilogy
 Little League World Series Baseball 2009
 L.O.L.
 Lost In Blue 2
 Lost In Blue 3
 Lunar Knights
 Madagascar
 Madagascar: Escape 2 Africa
 Madden NFL 08
 Madden NFL 09
 Magnetica
 Mario & Sonic At The Olympic Games
 Mario & Sonic At The Olympic Winter Games
 Mario Hoops 3-On-3
 Mario Kart DS
 Mario Party DS
 Mario Vs. Donkey Kong 2: March Of The Minis
 Marvel Super Hero Squad
 Math Play
 Mega Man ZX Advent
 Meteos
 Meteos: Disney Magic
 Metroid Prime Hunters
 Metroid Prime Pinball
 Might & Magic: Clash Of Heroes
 Mini Ninjas
 MLB Power Pros 2008
 Monopoly/Boggle/Yahtzee/Battleship
 Monster Band
 Monster Bomber
 Monster Racers
 Monsters Vs Aliens
 MX Vs ATV Reflex
 My DoItAll
 My Frogger Toy Trials
 My Word Coach
 MySims
 MySims Agents
 MySims Kingdom
 MySims Party
 MySims Racing
 MySims SkyHeroes
 Nacho Libre
 Namco Museum DS
 Nanostray
 Nanostray 2
 Need For Speed Carbon: Own The City
 Need For Speed: Nitro
 Need For Speed: ProStreet
 Need For Speed: Undercover
 Nervous Brickdown
 NEVES
 New Carnival Games
 New International Track & Field
 New Super Mario Bros.
 New York Times Crosswords, The
 Nicktoons: Attack Of The ToyBots
 Nicktoons: Battle For Volcano Island
 Ninjatown
 Over The Hedge
 PDC World Championship Darts
 Peggle Dual Shot
 Personal Trainer: Math
 Petz Dogz 2
 Petz Dogz Fashion
 Phineas And Ferb Ride Again
 Phineas And Ferb: Across The 2nd Dimension
 Picross 3D
 Picross DS
 PICTOIMAGE
 Ping Pals
 Pirates: Duels On The High Seas
 Pirates Of The Caribbean: At World's End
 Planet 51: The Game
 Planet Puzzle League
 Plants Vs. Zombies
 Playmobil Knights
 Playmobil Pirates
 Pogo Island
 Point Blank DS
 Pokemon Mystery Dungeon: Explorers Of Sky
 Pokemon Trozei!
 Polar Bowler
 Polarium
 Pony Friends
 Pony Friends: Mini-Breeds Edition
 Pony Friends 2
 Pop Island (Nintendo DSiWare)
 Pop Island - Paperfield (Nintendo DSiWare)
 Power Play Pool
 Prey The Stars
 Princess And The Frog, The
 Princess In Love
 Prism: Light The Way
 Pro Evolution Soccer 2008
 Professional Fisherman's Tour: Northern Hemisphere
 Puchi Puchi Virus
 Puyo POP FEVER
 Puzzle De Harvest Moon
 Quest Trio, The
 QuickSpot
 Rabbids Go Home
 Race Driver: Create & Race
 Rafa Nadal Tennis
 Ratatouille
 Rayman Raving Rabbids 2
 Rayman Raving Rabbids TV Party
 Ridge Racer DS
 Ringling Bros. and Barnum & Bailey Circus
 Rio
 Rock Revolution
 Rollin' Rascals
 RooGoo Attack!
 Rub Rabbits
 Rubik's World
 SBK: Snowboard Kids
 Scrabble
 Sega Casino
 Sega Superstars Tennis
 Shaun White Snowboarding
 Shining Stars: Super Starcade
 Shrek The Third
 Simpsons Game, The
 Skate It
 Solitaire Overload
 Sonic Colors
 Sonic Rush
 Sonic Rush Adventure
 Space Bust-A-Move
 Space Invaders Extreme
 Space Invaders Extreme 2
 SpongeBob's Boating Bash
 Squishy Tank
 Star Fox Command
 Stitch Jam
 Style Lab: Makeover
 Super Black Bass Fishing
 Super Dodgeball Brawlers
 Super Mario 64 DS
 Super Monkey Ball: Touch & Roll
 Super Speed Machines
 Superman Returns: The Videogame
 Suzuki Super-Bikes II: Riding Challenge
 Tamagotchi Connection: Corner Shop
 Tamagotchi Connection: Corner Shop 2
 Tamagotchi Connection: Corner Shop 3
 Tank Beat
 Tetris DS
 Tetris Party Deluxe
 ThinkSmart Kids 8+
 Thrillville: Off The Rails
 Tiger Woods PGA Tour 08
 Tinker Bell And The Great Fairy Rescue
 Toon-Doku
 Top Gun
 Totally Spies! 2: Undercover
 Touch Darts
 Touchmaster 2
 Touchmaster 3
 Touchmaster: Connect
 TrackMania DS
 TrackMania Turbo
 Trioncube
 Tropix DS
 True Swing Golf
 Ultimate Mortal Kombat
 Ultimate Puzzle Games: Sudoku Edition
 USA Today Puzzle Craze
 Warhammer 40,000: Squad Command
 Whac-A-Mole
 Wild West, The
 Windy X Windham
 Winning Eleven: Pro Evolution Soccer 2007
 WireWay
 Wizards Of Waverly Place
 Wordfish
 WordJong
 World Championship Games
 World Championship Poker: Deluxe Series
 World Cup Of Pool
 World Of Zoo
 Worms: Open Warfare
 Worms: Open Warfare 2
 Xiaolin Showdown
 Yoshi Touch & Go
 Zendoku
 Zoo Keeper |
| --- |

| DS Wifi IEEE802.11 Frames |
| --- |

**MAC Frame Format**

```

| 10..30 bytes MAC Header
 0..2312 bytes Frame Body (in practice, network MTU is circa 1500 bytes max)
 4 bytes Frame Check Sequence (FCS) (aka CRC32 on Header+Body) |
| --- |

**MAC Header (10..30 bytes)**

```

| Size Content
 2 Frame Control Field (FC)
 2 Duration/ID
 6 Address 1
 (6) Address 2 (if any)
 (6) Address 3 (if any)
 (2) Sequence Control (if any)
 (6) Address 4 (if any) |
| --- |

**Frame Control Field (FC)**

```

| Bit Expl.
 0-1 Protocol Version (0=Current, 1..3=Reserved)
 2-3 Type (0=Managment, 1=Control, 2=Data, 3=Reserved)
 4-7 Subtype (see next chapters) (meaning depends on above Type)
 8 To Distribution System (ToDS)
 9 From Distribution System (FromDS)
 10 More Fragments
 11 Retry
 12 Power Managment (0=Active, 1=STA will enter Power-Safe mode after..)
 13 More Data
 14 Wired Equivalent Privacy (WEP) Encryption (0=No, 1=Yes)
 15 Order |
| --- |

Bit 8-11 and Bit 13-15 are always 0 in Control Frames.

**Duration/ID Field (16bit)**

```

| 0000h..7FFFh Duration (0-32767)
 8000h Fixed value within frames transmitted during the CFP
 (CFP=Contention Free Period)
 8001h..BFFFh Reserved
 C000h Reserved
 C001h..C7D7h Association ID (AID) (1..2007) in PS-Poll frames
 C7D8h..FFFFh Reserved |
| --- |

**48bit MAC Addresses**

MAC Addresses are 48bit (6 bytes) (Bit0 is the LSB of the 1st byte),

```

| 0 Group Flag (0=Individual Address, 1=Group Address)
 1 Local Flag (0=Universally Administered Address, 1=Locally Administered)
 2-23 22bit Manufacturer ID (assigned by IEEE)
 24-47 24bit Device ID (assigned by the Manufacturer) |
| --- |

Special NDS related Addresses:

```

| 00 09 BF xx xx xx NDS-Consoles (Original NDS with firmware v1-v5)
 00 16 56 xx xx xx NDS-Consoles (Newer NDS-Lite with firmware v6 and up)
 00 23 CC xx xx xx DSi-Consoles (Original DSi with early mainboard; nocash)
 00 24 1E xx xx xx DSi-Consoles (Another DSi; scanlime)
 40 F4 07 xx xx xx DSi-Consoles (with DWM-W024; nocash)
 E0 E7 51 xx xx xx DSi-Consoles (with DWM-W024; nocash/desoldered)
 CC 9E 00 xx xx xx DSi-Consoles (with J27H020; nocash)
 03 09 BF 00 00 00 NDS-Multiboot: host to client (main data flow)
 03 09 BF 00 00 10 NDS-Multiboot: client to host (replies)
 03 09 BF 00 00 03 NDS-Multiboot: host to client (acknowledges replies)
 FF FF FF FF FF FF Broadcast to all stations (eg. Beacons) |
| --- |

**Sequence Control Field**

```

| Bit Expl.
 0-3 Fragment Number (0=First (or only) fragment)
 4-15 Sequence Number |
| --- |

(increment by 1, except on retransmissions, ie. retries)

**WEP Frame Body**

```

| 3 bytes Initialization Vector (WEP IV)
 1 byte Pad (6bit, all zero), Key ID (2bit)
 1..? bytes Data (encrypted data)
 4 bytes ICV (encrypted CRC32 across Data) |
| --- |

| DS Wifi IEEE802.11 Managment Frames (Type=0) |
| --- |

All Managment Frames have 24-byte Frame Header, with following values:

```

| FC(2), Duration(2), DA(6), SA(6), BSSID(6), Sequence Control(2) |
| --- |

The content of the Frame Body depends on the FC's Subtype:

```

| Subtype Frame Body
 0 Association request Capability, ListenInterval, SSID, SuppRates
 1 Association response Capability, Status, AID, SuppRates
 2 Reassociation request Capability, ListenInterval, CurrAP, SSID, SuppRates
 3 Reassociation response Capability, Status, AID, SuppRates
 4 Probe request SSID, SuppRates
 5 Probe response Same as for Beacon (but without TIM)
 8 Beacon Timestamp,BeaconInterval,Capability,SSID,SuppRates,
 FH Parameter Set (when using Frequency Hopping),
 DS Parameter Set (when using Direct Sequence),
 CF Parameter Set (when supporting PCF),
 IBSS Parameter Set (when in an IBSS),
 TIM (when generated by AP)
 9 Announcement traffic indication message (ATIM) Body is "null" (=none?)
 A Disassociation ReasonCode
 B Authentication AuthAlgorithm, AuthSequence, Status, ChallengeText
 C Deauthentication ReasonCode |
| --- |

Subtypes 6..7, and D..F are Reserved.

**The separate components of the Frame Body are...**

64bit Parameters (8 bytes)

```

| Timestamp: value of the TSFTIMER (see 11.1) of a frame's source. Uh? |
| --- |

48bit Parameters (6 bytes)

```

| Current AP (Access Point): MAC Address of AP with which station is associated |
| --- |

16bit Parameters (2 bytes)

```

| Capability Information (see list below)
 Status code (see list below) (0000h=Successful, other=Error code)
 Reason code (see list below) (Error code)
 Association ID (AID) (C000h+1..2007)
 Authentication Algorithm (0=Open System, 1=Shared Key, 2..FFFFh=Reserved)
 Authentication Transaction Sequence Number (Open System:1-2, Shared Key:1-4)
 Beacon Interval (Time between beacons, N*1024 us)
 Listen Interval (see note below) |
| --- |

Information elements (1byte ID, 1byte LEN, followed by LEN byte(s) data)

```

| ID LEN Expl.
 00h 00h-20h SSID Service Set Identity (LEN=0 for broadcast SSID) (ASCII)
 01h 01h-08h Supported rates; each (nn AND 7Fh)*500kbit/s, bit7=flag
 02h 05h FH (Frequency Hopping) Parameter Set
 DwellTime(16bit), HopSet, HopPattern, HopIndex
 03h 01h DS (Distribution System) Parameter Set; Channel (01h..0Eh)
 04h 06h CF Parameter Set; Count, Period, MaxDuration, RemainDuration
 05h 04h..FEh TIM; Count,Period,Control, 1-251 bytes PartialVirtualBitmap
 06h 02h IBSS Parameter Set; ATIM Window length (16bit)
 07h-0Fh - Reserved
 (07h) .. 802.11d Country
 (08h) .. 802.11d Hopping Pattern Params
 (09h) .. 802.11d Hopping Pattern Table
 (0Ah) .. 802.11d Request
 10h 02h..FEh Challenge text; 1-253 bytes Authentication data
 (Used only for Shared Key sequence no 2,3)
 (none such for Open System)
 (none such for Shared key sequence no 1,4)
 11h-1Fh - Reserved for challenge text extension
 20h-FFh - Reserved
 (20h) .. 802.11h Power Constraint
 (21h) .. 802.11h Power Capability
 (22h) .. 802.11h TPC Request (Transmit Power Control)
 (23h) .. 802.11h TPC Report
 (24h) .. 802.11h Supported Channels
 (25h) .. 802.11h Channel Switch Announcement
 (26h) .. 802.11h Measurement Request
 (27h) .. 802.11h Measurement Report
 (28h) .. 802.11h Quiet
 (29h) .. 802.11h IBSS DFS
 2Ah .. 802.11g ERP Information (spotted in newer beacons)
 30h var 802.11i Reserved but used for WPA2 RSNIE <-- officially
 32h .. 802.11g Extended Supported Rates (spotted in newer beacons)
 DDh var Reserved but used for WPA RSNIE <-- vendor specific
 DDh var Reserved but used by Nintendo for NDS-Multiboot beacons
 2Dh .. Unknown (spotted in newer beacons)
 2Fh .. Unknown (spotted in newer beacons)
 3Dh .. Unknown (spotted in newer beacons)
 7Fh .. Unknown (spotted in newer beacons) |
| --- |

IDs 20h-FFh are commonly used; I've received values 2xh..3xh and DDh (from
non-nintendo network routers in the neighborhood); no idea if these "Reserved"
IDs are somewhere officially documented?

**Capability Information**

```

| Bit0 ESS
 Bit1 IBSS
 Bit2 CF-Pollable
 Bit3 CF-Poll Request
 Bit4 Privacy
 Bit5 Short Preamble (IEEE802.11b only)
 Bit6 PBCC (IEEE802.11b only)
 Bit7 Channel Agility (IEEE802.11b only)
 Bit5-7 Reserved (0) (original IEEE802.11 specs)
 Bit8-15 Reserved (0) |
| --- |

**Listen Interval**

```

| ... used to indicate to the AP how often an STA wakes to listen to Beacon
 management frames. The value of this parameter is the STA's Listen Interval
 parameter of the MLME-Associate. request primitive and is expressed in
 units of Beacon Interval. |
| --- |

**Reason codes**

```

| 00h Reserved
 01h Unspecified reason
 02h Previous authentication no longer valid
 03h Deauthenticated because sending station is leaving (or has left) IBSS
 or ESS
 04h Disassociated due to inactivity
 05h Disassociated because AP is unable to handle all currently associated
 stations
 06h Class 2 frame received from nonauthenticated station
 07h Class 3 frame received from nonassociated station
 08h Disassociated because sending station is leaving (or has left) BSS
 09h Station requesting (re)association is not authenticated with responding
 station
 0Ah..FFFFh Reserved |
| --- |

**Status codes**

```

| 00h Successful
 01h Unspecified failure
 02h..09h Reserved
 0Ah Cannot support all requested cap's in the Capability Information field
 0Bh Reassociation denied due to inability to confirm that association exists
 0Ch Association denied due to reason outside the scope of this standard
 0Dh Responding station doesn't support the specified authentication algorithm
 0Eh Received an Authentication frame with authentication transaction sequence
 number out of expected sequence
 0Fh Authentication rejected because of challenge failure
 10h Authentication rejected due to timeout waiting for next frame in sequence
 11h Association denied because AP is unable to handle additional associated
 stations
 12h Association denied due to requesting station not supporting all of the
 data rates in the BSSBasicRateSet parameter
 13h Association denied due to requesting station not supporting
 the Short Preamble option (IEEE802.11b only)
 14h Association denied due to requesting station not supporting
 the PBCC Modulation option (IEEE802.11b only)
 15h Association denied due to requesting station not supporting
 the Channel Agility option (IEEE802.11b only)
 13h-15h Reserved (original IEEE802.11 specs)
 16h..FFFFh Reserved |
| --- |

| DS Wifi IEEE802.11 Control and Data Frames (Type=1 and 2) |
| --- |

**Control Frames (Type=1)**

All Control Frames have 10-byte or 16-byte headers, depending on the Subtype:

```

| Subtype Frame Header
 0-9 Reserved - - - -
 A Power Save (PS)-Poll FC AID BSSID TA
 B Request To Send (RTS) FC Duration RA TA
 C Clear To Send (CTS) FC Duration RA -
 D Acknowledgment (ACK) FC Duration RA -
 E Contention-Free (CF)-End FC Duration RA BSSID
 F CF-End + CF-Ack FC Duration RA BSSID |
| --- |

Control Frames do not have a Frame Body, so the Header is directly followed by
the FCS.

**Data Frames (Type=2)**

All Data Frames consist of the following components:

```

| FC, Duration/ID, Address 1, Address 2, Address 3, Sequence Control,
 Address 4 (only on From DS to DS), Frame Body, FCS. |
| --- |

The meaning of the 3 or 4 addresses depends on Frame Control FromDS/ToDS bits:

```

| Frame Control Address 1 Address 2 Address 3 Address 4
 From STA to STA DA SA BSSID -
 From DS to STA DA BSSID SA -
 From STA to DS BSSID SA DA -
 From DS to DS RA TA DA SA |
| --- |

Frame Control Subtypes for Data Frames (Type=2) are:

```

| 0 Data
 1 Data + CF-Ack
 2 Data + CF-Poll
 3 Data + CF-Ack + CF-Poll
 4 Null function (no data)
 5 CF-Ack (no data)
 6 CF-Poll (no data)
 7 CF-Ack + CF-Poll (no data)
 8-F Reserved |
| --- |

Note: The Frame Header is 24 bytes (or 30 bytes if with Address 4). The Data in
Frame Body is usually starting with LLC stuff, ie. AAh,AAh,03h,00h...

| DS Wifi WPA/WPA2 Handshake Messages (EAPOL) |
| --- |

**4-Way Handshake and Group Key Handshake messages**

For WPA, the Group Key Handshake occurs immediately after the 4-way Handshake
(effecitively making it a 6-way handshake). For WPA2, the Group Key is
contained in the 4-way Handshake, so the Group Key Handshake isn't needed.

For both WPA and WPA2, the access point may change the Group Key every once and
then, and invoke a new Group Key Handshake for the new key.

**EAPOL Key Frame**

This is preceeded by a LLC/SNAP header (AAh,AAh,03h,00h,00h,00h,88h,8Eh), the
888Eh in last two bytes indicates that following bytes to contain an EAPOL Key
Frame:

```

| 00h 2 Version/Type (or Type/Version?) (01 03)
 02h 2 Length of [04h..end] (005Fh+LEN) ;BIG-ENDIAN
 04h 1 Descriptor Type (FEh=WPA, 02h=WPA2)
 05h 2 Key Information (flags, see below) ;BIG-ENDIAN
 07h 2 Key Length (0=None, 20h=TKIP, 10h=CCMP, 05h/0Dh=WEP) ;BIG-ENDIAN
 09h 8 Key Replay Counter (usually 0 or 1 in first message) ;BIG-ENDIAN
 11h 32 Key Nonce (ANonce/SNonce)
 31h 16 Key Data IV (RC4 uses IV+KEK) (not used for AES-Key-Wrap)
 41h 8 Key RSC (TSC/PN) (whatever, for GTK) ;LITTLE-ENDIAN
 49h 8 Reserved (zerofilled)
 51h 16 Key MIC on [00h..end] (with MIC initially zerofilled) ;HMAC
 61h 2 Key Data Length (LEN) (00 nn) ;BIG-ENDIAN
 63h LEN Key Data (can be encrypted in certain messages) |
| --- |

**Key Information flags**

```

| 0-2 Key Descriptor Version (1=WPA/MD5/RC4, 2=WPA2/SHA1/AESkeywrap)
 3 Key Type (0=Group, 1=Pairwise)
 4-5 Reserved (0) or WPA Group Key Index (1 or 2) (zero for WPA2)
 6 Install (0=No, 1=Yes, configure temporal key)
 7 Key Ack (0=No, 1=Yes, AP wants a reply; with same Key Replay Counter)
 8 Key MIC (0=No, 1=Yes, key frame contains MIC)
 9 Secure (0=No, 1=Yes, initial key-exchange complete)
 10 Error (0=No, 1=Yes, MIC failure and Request=1)
 11 Request (0=No, 1=Yes, request AP to invoke a new handshake)
 12 Encrypted(0=No, 1=Yes, Key Data is encrypted; via RC4 or AESkeywrap)
 13-15 Reserved (0) |
| --- |

**Key Data**

Key Data does usually consist of element(s) in following form:

```

| 00h 1 Element ID (for WPA: DDh=RSNIE - for WPA2: 30h=RSNIE, DDh=KDE)
 01h 1 Element Length of [02h..end]
 02h .. Element Data (OUI's etc.) |
| --- |

WPA and WPA2 assign different meanings to Element ID value DDh. The 3-byte
maker prefix in OUI values does also differ for WPA/WPA2. And, the meaning of
the OUI's varies depending on WHERE they are used (eg. 00-0F-AC-02 could be a
Cipher Suite entry, or an Authentication entry):

```
********************************
| EAPOL Descriptor Type values
 WPA WPA2 Meaning
 FEh 02h Indicates if ElementIDs and OUIs are WPA or WPA2
 EAPOL Key Information flags/values
 0089h 008Ah Handshake #1 ;\
 0109h 010Ah Handshake #2 ; 4-way Handshake
 01C9h 13CAh Handshake #3 ;
 0109h(again) 030Ah Handshake #4 ;/
 0391h/03A1h 1382h Handshake #5 ;\Group Key Handshake
 0311h/0321h 0302h Handshake #6 ;/
 EAPOL Key Data Element IDs
 DDh 30h Element ID for RSNIE (Robust Network Security info)
 - DDh Element ID for KDE (Key Data Encapsulation)
 - DDh Element ID for padding (followed by 00h-bytes)
 RSNIE Prefix OUI's (WPA only):
 00-50-F2-01 - Element Vendor OUI for RSNIE
 RSNIE Group Cipher suite selector OUI's (aka Multicast):
 00-50-F2-01 00-0F-AC-01 RSNIE Group Cipher WEP-40 (default for US/NSA)
 00-50-F2-02 00-0F-AC-02 RSNIE Group Cipher TKIP (default for WPA)
 00-50-F2-04 00-0F-AC-04 RSNIE Group Cipher CCMP (default for WPA2)
 00-50-F2-05 00-0F-AC-05 RSNIE Group Cipher WEP-104 (default for WEP)
 RSNIE Pairwise Cipher suite selector OUI's (aka Unicast):
 00-50-F2-00 00-0F-AC-00 RSNIE Pairwise Cipher None (WEP, Group Cipher only)
 00-50-F2-02 00-0F-AC-02 RSNIE Pairwise Cipher TKIP (default for WPA)
 00-50-F2-04 00-0F-AC-04 RSNIE Pairwise Cipher CCMP (default for WPA2)
 RSNIE Authentication AKM suite selector OUI's :
 00-50-F2-01 00-0F-AC-01 RSNIE Authentication over IEEE 802.1X (radius?)
 00-50-F2-02 00-0F-AC-02 RSNIE Authentication over PSK (default/home use)
 KDE Key Data Encapsulation OUI's (WPA2 only):
 - 00-0F-AC-01 KDE GTK (followed by 2+N bytes)
 - 00-0F-AC-02 KDE STAKey (followed by 2+6+N bytes)
 - 00-0F-AC-03 KDE MAC address (followed by 6 bytes)
 - 00-0F-AC-04 KDE PMKID (followed by 16 bytes) |
| --- |

**RSNIE (Robust Network Security Information Element)**

RSNIE allows to exchange information about the desired/supported encryption
methods. RSNIE is found in handshake message 2/3 (and also in Beacons). In most
cases one could just ignore the RSNIE stuff (eg. if Wifi FLASH is already
configured to use a specific cipher type). A special case might be networks
that use a weaker Group Cipher (for older devices) combined with offering
stronger Pairwise Ciphers (for devices that support them).

```
************
| WPA2 RSNIE (Robust Network Security Information Element):
 00h 1 Element ID (30h=RSNIE for WPA2)
 01h 1 Element Len of [02h..end] (usually 14h)
 02h 2 RSNIE Version 1 (01 00) ;WHATEVER-ENDIAN?
 04h 4 RSNIE Group Cipher Suite OUI (CCMP) (00 0F AC 04)
 08h 2 RSNIE Pairwise Cipher Suite Count (1) (01 00) ;LITTLE-ENDIAN
 0Ah 4 RSNIE Pairwise Cipher Suite OUI (CCMP) (00 0F AC 04)
 0Eh 2 RSNIE Authentication Count (1) (01 00) ;LITTLE-ENDIAN
 10h 4 RSNIE Authentication OUI (PSK) (00 0F AC 02)
 14h 2 RSNIE Capabilities (00 00) ;LITTLE-ENDIAN?
 16h (2) RSNIE Optional PMKID Count ;\usually none such ;LITTLE-ENDIAN
 18h (16)RSNIE Optional PMKID's ;/
 WPA RSNIE (Robust Network Security Information Element):
 00h 1 Element ID (DDh=Vendor/RSNIE for WPA)
 01h 1 Element Len of [02h..end] (usually 16h or 18h)
 02h 4 Element Vendor OUI for RSNIE (00 50 F2 01) ;<-- WPA only
 06h 2 RSNIE Version value? (1) (01 00) ;WHATEVER-ENDIAN?
 08h 4 RSNIE Mcast OUI (TKIP) (00 50 F2 02)
 0Ch 2 RSNIE Ucast Count (1) (01 00) ;LITTLE-ENDIAN
 0Eh 4 RSNIE Ucast OUI (TKIP) (00 50 F2 02)
 12h 2 RSNIE Auth AKM Count (1) (01 00) ;LITTLE-ENDIAN
 14h 4 RSNIE Auth AKM OUI (PSK) (00 50 F2 02)
 18h (2) RSNIE Capabilities maybe? (00 00) ;LITTLE-ENDIAN?
 RSN Capabilities flags (usually 0000h) (also spotted: 0C 00):
 0 RSN Pre-Auth capabilities
 1 RSN No Pairwise capabilities
 2-3 RSN PTKSA Replay Counters (0..3 = 1,2,4,16 replay counters)
 4-5 RSN GTKSA Replay Counters (0..3 = 1,2,4,16 replay counters)
 6 Managment Frame Protection Required
 7 Managment Frame Protection Capable
 8 Joint Multi-band RSNA
 9 PeerKey Enabled
 10 SPP A-MSDU Capable
 11 SPP A-MSDU Required
 12 PBAC
 13 Ext Key ID for Unicast
 14-15 Reserved (0) |
| --- |

**KDE Key Data Encapsulation (or raw data for WPA)**

KDE (or WPA's raw data) is mainly useful for transferring the GTK group key.
Note that Key Data in WPA2 message 3 contains both RSNIE and KDE GTK (and KDE
padding). Whilst the raw data in WPA cannot be mixed RSNIE.

```
****************
| WPA2 KDE GTK (Key Data Encapsulation for Group Key, in encrypted Key Data):
 00h 1 Element ID (DDh=KDE for WPA2)
 01h 1 Element Len (16h)
 02h 4 KDE OUI GTK (00-0F-AC-01) (occurs in message 3/5)
 06h 1 KDE GTK Key ID (01h or 02h) ;bit2: Tx ?
 07h 1 KDE GTK Reserved (00h)
 08h 16 KDE GTK Key GTK (for Key ID from above byte [06h])
 WPA2 KDE PKMID (Key Data Encapsulation for PKMID) (optional, not needed):
 00h 1 Element ID (DDh=KDE for WPA2)
 01h 1 Element Len (14h)
 02h 4 KDE OUI PMKID (00-0F-AC-04) (optionally occurs in message 1)
 06h 16 KDE PMKID (useless checksum on PMK, sometimes exposed in message 1)
 WPA2 KDE Padding (for padding Key Data to Nx8 bytes for AES-Key-wrap):
 00h 1 Element ID (DDh=KDE for WPA2)
 01h 0-6 Padding (00h) (aka Element Len=00h)
 WPA GTK (raw Group Key; without Element ID or KDE-style encapsulation):
 00h 16 Key GTK (for Key ID from Key Information bit4-5) (in message 5) |
| --- |

| DS Wifi WPA/WPA2 Keys and MICs |
| --- |

**Summary of WPA/WPA2 Keys**

```

| PSK Preshared Key (based on password and SSID)
 PMK Pairwise Master Key (same as PSK)
 PTK Pairwise Transient Key (based on PMK, AA, SPA, ANonce, SNonce)
 KCK EAPOL Key Confirmation Key (PTK.bit0..127) ;for handshake MIC's
 KEK EAPOL Key Encryption Key (PTK.bit128..255) ;for handshake Key Data
 TK Temporal Key (TKIP:PTK.bit256..511, CCMP:PTK.bit256..383)
 GMK Group Master Key (don't care, used only internally by the access point)
 GTK Group Transient Key (for multicast/broadcast) (based on GMK, AA, GNonce) |
| --- |

**Summary of WPA/WPA2 Seeds**

```

| password ASCII password for the Wifi network
 SSID ASCII name of access point
 AA MAC address of access point (BSSID)
 SPA MAC address of DSi console
 Anonce Random number from access point (handshake message #1 and #3)
 Snonce Random number from console (handshake message #2)
 Gnonce Random number internally used by access point (don't care) |
| --- |

**Summary of WPA/WPA2 Checksums**

```

| MIC Message Integrity Code, checksum on EAPOL messages
 PMKID PMK ID, checksum on PMK and AA, SPA (optional, don't care) |
| --- |

**PRF(key,keylen, src,srclen, dst,dstlen, numrounds) ;Pseudo-Random Function**

```

| for i=0 to (dstlen-1)/14
 call SHA1HMAC(src,srclen, key,keylen, tmpdst)
 tmpsum[0..13] = tmpdst[0..13]
 for j=1 to numrounds-1 ;only if numrounds>1
 tmpsrc[0..13] = tmpdst[0..13], tmpsrclen=14
 call SHA1HMAC(tmpsrc,tmpsrclen, key,keylen, tmpdst)
 tmpsum[0..13] = tmpsum[0..13] XOR tmpdst[0..13]
 next j
 src[srclen-1] = src[srclen-1] + 01h ;increase last byte of src
 len=min(14,(dstlen-i*14))
 dst[i*14+(0..(len-1))] = tmpsum[0..(len-1)]
 next i
 src[srclen-1] = src[srclen-1] - (dstlen+13)/14 ;undo increments, if desired |
| --- |

Note: This is using SHA1HMAC for both WPA and WPA2 (unlike the MIC, which is
computed via MD5HMAC for WPA, and SHA1HMAC for WPA2).

**PSK (Preshared Key) aka PMK (Pairwise Master Key)**

```

| key = password, keylen = len(password) ;ASCII string
 src = ssid + bytes(00h,00h,00h,01h), srclen = len(ssid)+4 ;ASCII string
 dst = PSK, dstlen = 32, numrounds=4096
 call PRF(key,keylen, src,srclen, dst,dstlen, numrounds)
 PMK=PSK |
| --- |

Computing the PSK/PMK requires 2x4096 SHA1HMAC calculations, which can be quite
slow. The speed could be doubled by pre-computing the SHA1 states for "key XOR
36h's" and "key XOR 5Ch's". And, the PSK/PMK needs to be calculated only when
changing the password/ssid (the DSi stores the PSK in Wifi-FLASH).

**PTK (Pairwise Transient Key) and KCK/KEK/TK**

```

| src[0..21] = "Pairwise key expansion"
 src[22] = byte(00h)
 src[23..28] = min(AA,SPA) ;\MAC addresses (AA=BSSID, SPA=console)
 src[29..34] = max(AA,SPA) ;/
 src[35..66] = min(ANonce,SNonce) ;\nonces from 4-way handshake message 1+2
 src[67..98] = max(ANonce,SNonce) ;/
 src[99] = byte(00h)
 srclen = 22+1+6+6+32+32+1 = 100
 key=PSK, keylen=32, numrounds=1
 dst=PTK, dstlen=64 ;WPA needs dstlen=64 (WPA2 would also work with len=48)
 call PRF(key,keylen, src,srclen, dst,dstlen, numrounds)
 KCK = PTK[00h..0Fh] ;-for EAPOL handshake MIC checksums
 KEK = PTK[10h..1Fh] ;-for EAPOL handshake Key Data decryption
 TK.key = PTK[20h..2Fh] ;-for data packets
 TX.tx = PTK[30h..37h] ;\needed for WPA/TKIP only (not WPA2/AES)
 TX.rx = PTK[38h..3Fh] ;/
 TK.keyindex = 0 |
| --- |

**GTK (Group Transient Key)**

```

| GTK.key = GTK[00h..0Fh] ;-for data packets
 GTX.tx = GTK[10h..17h] ;\needed for WPA/TKIP only (not WPA2/AES)
 GTX.rx = GTK[18h..1Fh] ;/
 GTK.keyindex = 1 or 2 ;WPA: from EAPOL Key Information bit4-5
 GTK.keyindex = 1 or 2 ;WPA2: from EAPOL Key Data KDE entry |
| --- |

**MIC (Message Integrity Code)**

```

| oldmic = EAPOL[51h..60h]
 EAPOL[51h..60h] = zerofill
 src=EAPOL, srclen=EAPOL[02h]*100h+EAPOL[03h]
 key=KCK, keylen=16
 if (EAPOL[06h] AND 07h)=1 then call MD5HMAC(src,srclen, key,keylen, dst)
 if (EAPOL[06h] AND 07h)=2 then call SHA1HMAC(src,srclen, key,keylen, dst)
 newmic = dst[0..0Fh] ;16-byte MD5 result, or first 16byte of SHA1 result
 EAPOL[51h..60h] = newmic
 if newmic <> oldmic then error ;when verifying MIC |
| --- |

**PMKID (whatever ID, optional/useless, don't care)**

```

| key=PMK, keylen=32
 src[0..7] = "PMK Name"
 src[8..13] = AA ;aka MAC address of access point (BSSID)
 src[14..19] = SPA ;aka MAC address of console
 srclen = 8+6+6 = 20
 call SHA1HMAC(src,srclen, key,keylen, dst)
 PMKID = dst[0..0Fh] ;first 16byte of SHA1 result |
| --- |

Note: SHA1HMAC is used for WPA2. Unknown if WPA uses SHA1HMAC, or MD5HMAC, or
if WPA does have PMKIDs at all.

**Deriving GTK (done internally by access point, don't care)**

```

| src[0..18] = "Group key expansion"
 src[19] = byte(00h)
 src[20..25] = AA ;MAC address (AA=BSSID)
 src[26..57] = GNonce ;whaever random/timer/index
 src[58] = byte(00h)
 srclen = 19+1+6+32+1 = 59
 key=GMK, keylen=32, numrounds=1 ;whatever random key
 dst=GTK, dstlen=32
 call PRF(key,keylen, src,srclen, dst,dstlen, numrounds) |
| --- |

This is the recommended way for creating the GTK, anyways, this is done
internally by the access point, and everybody else doesn't need to worry about
how the GTK was generated (ie. one just uses the GTK received in EAPOL
messages).

| DS Wifi WPA/WPA2 Encryption |
| --- |

**Summary of Encryption/Decryption functions**

```

| Encrypt/Decrypt WPA/WEP packets --> RC4 (Rivest Cipher 4 aka ARC4)
 Encrypt/Decrypt WPA EAPOL key data --> RC4 (Rivest Cipher 4 aka ARC4)
 Encrypt/Decrypt WPA2 EAPOL key data --> AES-Key-Wrap/Unwrap
 Encrypt/Decrypt WPA2 packets --> AES-CCMP (AES-CTR-with-CBC-MAC) |
| --- |

**RC4 (Rivest Cipher 4 aka ARC4) (WEP/WPA)**

Both NDS-Wifi and DSi-Wifi have RC4 hardware support for encrypting and
decrypting WEP/WPA packets, however, that's working only for full packets (not
for the Key Data field in WPA EAPOL packets, so both NDS and DSi will require a
RC4 software implementation for the EAPOL part).

```
****************
| RC4(src,dst,len,preskip,key,keylen):
 for i=0 to FFh, sbox[i]=i, next i ;-clear sbox
 j=0 ;\
 for i=0 to FFh ;
 j=(j+sbox[i]+key[i mod keylen]) and FFh ; apply key
 swap(sbox[i],sbox[j] ;
 next i ;/
 i=0, j=0
 for k=1 to preskip+len
 i=(i+1) and FFh, j=(j+sbox[i]) and FFh, swap(sbox[i],sbox[j])
 if preskip>0 then preskip=preskip-1
 else [dst]=[src] xor sbox[(sbox[i]+sbox[j]) and FFh], dst=dst+1, src=src+1
 next k
 parameters for WEP/WPA packets (done by hardware):
 key=iv(3)+password(5/13), keylen=3+5/13 ;WEP Key=WEP.IV+Password
 key=iv(3)+from PTK???, keylen=3+??? ;WPA Key=WEP.IV+???
 src=data(n)+icv(4), srclen=n+4 ;src, for WEP
 src=data(n)+mic(8)+icv(4), srclen=n+8+4 ;src, for WPA
 preskip=0
 parameters for WPA EAPOL key data (requires software implementation):
 key=EAPOL[31h..40h]+KEK[00h..0Fh], keylen=10h+10h ;Key = EAPOL Key IV + KEK
 src=EAPOL+63h, srclen=bigendian(EAPOL[61h]) ;src, for WPA
 preskip=100h
 parameters for 70h-byte block in Nintendo Zone beacons (by software):
 key="!SDW"+LastFourBytesOf(BSSID), keylen=8
 src=BeaconTagDDh[18h..87h], srclen=70h
 preskip=0 |
| --- |

**AES (Advanced Encryption Standard) (WPA2, DSi only)**

NDS-Wifi doesn't have any AES hardware support at all, and a software
implementation doesn't work because NDS-Wifi automatically applies WEP
encryption (and adds/removes the WEP.IV field) when seeing the encryption flag
in the frame header (and trying to disable the WEP hardware seems to cause
encrypted packets not to be received at all).

DSi-Wifi has hardware support for AES-CCMP encrypted packets, however,
AES-Key-Unwrap for Key Data in WPA2 EAPOL packets isn't supported by hardware,
and does thus require a software implementation (the ARM7 AES hardware doesn't
help because it supports AES-CTR and AES-CCM only).

```
********
| AES-Key-Wrap/Unwrap(src,dst,len,key,keylen,mode) (for WPA2 EAPOL Key Data)
 if (len and 7)<>0 then error ;must be multiple of 8 ;-verify len
 aes_setkey(mode,key,keylen) ;-init key
 if mode=ENCRYPT and [src+00h..07h]<>A6A6A6A6A6A6A6A6h then error ;-verify IV
 if mode=ENCRYPT then org=dst+8, count=1 ;-for wrap
 if mode=DECRYPT then org=dst+len-8, count=((len-8)/8)*6 ;-for unwrap
 [dst+0..len-1] = [src+0..len-1] ;copy IV+DATA to dst
 [tmp+00h..07h] = [dst+00h..07h] ;read IV from dst+0
 for i=1 to 6
 ptr=org
 for j=1 to (len-8)/8
 [tmp+08h..0Fh] = [ptr+00h..07h] ;read DATA from dst+index
 if mode=ENCRYPT then aes_crypt_block(ENCRYPT,tmp,tmp) ;encrypt tmp
 [tmp+07h]=[tmp+07h] xor count ;adjust byte[7]
 if mode=DECRYPT then aes_crypt_block(DECRYPT,tmp,tmp) ;decrypt tmp
 [ptr+00h..07h] = [tmp+08h..0Fh] ;writeback DATA to dst+index
 if mode=ENCRYPT then ptr=ptr+8, count=count+1
 if mode=DECRYPT then ptr=ptr-8, count=count-1
 next j
 next i
 [dst+00h..07h] = [tmp+00h..07h] ;writeback IV to dst+0
 if mode=DECRYPT and [dst+00h..07h]<>A6A6A6A6A6A6A6A6h then error ;-verify IV
 Parameters for Wrap/Unwrap:
 mode=ENCRYPT ;<-- for Wrap (encrypt, used by access points)
 mode=DECRYPT ;<-- for Unwrap (decrypt, used by clients)
 key=KEK, keylen=10h bytes (128bit)
 src=EAPOL+63h, srclen=bigendian(EAPOL[61h]) |
| --- |

The "aes_setkey" and "aes_crypt_block" functions are same as the BIG-ENDIAN
implementations described in DSi AES chapter:

DSi Advanced Encryption Standard (AES)

**Access Points / Keys**

Keys for different access points are stored in Wifi-FLASH. Access Point 1-3 can
be configured as Open/WEP only, and Access Point 4-6 can be Open/WEP/WPA/WPA2
(these extra entries exist on DSi only, the standard NDS firmware doesn't have
free memory for storing access point 4-6).

DS Firmware Wifi Internet Access Points

**WPA/TKIP Encrypted Packets**

```

| .. MAC Header ;-Normal Header
 1 TSC1 ;\ WEPSeed[1]=(TSC1 OR 20h) AND 7Fh
 1 WEPSeed[1] ; WEP IV and Flags
 1 TSC0 (LSB) ; (Flags: bit0-4=Rsvd, bit5=ExtIV, bit6-7=KeyID)
 1 Flags ;/ (bit5: 0=No/WEP, 1=Yes/TKIP)
 1 TSC2 ;\
 1 TSC3 ; WPA Extended IV
 1 TSC4 ;
 1 TSC5 (MSB) ;/
 .. Data ;-Normal Data ;\
 8 MIC ;-WPA MIC "Michael" ; encrypted area
 4 ICV ;-WEP ICV ;/
 4 FCS ;-Normal FCS |
| --- |

**WPA2/AES-CCMP Encrypted Packets**

```

| .. MAC Header ;-Normal Header
 1 PN0 (LSB) ;\
 1 PN1 ; CCMP Header (IV and Flags)
 1 Rsvd ; (Flags: bit0-4=Rsvd, bit5=ExtIV, bit6-7=KeyID)
 1 Flags ; (bit5: 0=No/WEP, 1=Yes/TKIP)
 1 PN2 ;
 1 PN3 ;
 1 PN4 ;
 1 PN5 (MSB) ;/
 .. Data ;-Normal Data ;\encrypted area
 8 MIC ;-CCMP MIC "AES MAC?" ;/
 4 FCS ;-Normal FCS |
| --- |

CCMP related: A2 (MPDU address 2), AAD, PN, Priority field of MPDU.

**TKIP MIC**

```

| 6 DA
 6 SA
 1 Priority (0) (reserved for future)
 3 Zero (0) (also reserved for future)
 .. Data
 8 MIC (M0..M7) (aka L0..L3, R0..R3) |
| --- |

TKIP mixing

```

| TTAK = Phase1 (TK, TA, TSC)
 WEP seed = Phase2 (TTAK, TK, TSC) |
| --- |

| DS Wifi FFC ID |
| --- |

**Wifi FCC ID**

The wifi hardware has been registered by Mitsumi, Hon Hai, and Nintendo
themselves:

```
[https://fccid.io/BKE](https://fccid.io/BKE)[https://fccid.io/EW4](https://fccid.io/EW4)[https://fccid.io/MCL](https://fccid.io/MCL)
| - Nintendo
 - Mitsumi
 - Hon Hai (Foxconn) |
| --- |

The above webpages include some unrelated stuff (like bluetooth), and some
duplicate entries. The wifi/console related entries are:

```
[https://fccid.io/EW4-AGBWA](https://fccid.io/EW4-AGBWA)[https://fccid.io/EW4-OXYWA](https://fccid.io/EW4-OXYWA)[https://fccid.io/BKENTR001](https://fccid.io/BKENTR001)[https://fccid.io/BKEUSG-001](https://fccid.io/BKEUSG-001)[https://fccid.io/EW4DWMW006](https://fccid.io/EW4DWMW006)[https://fccid.io/BKERVL036](https://fccid.io/BKERVL036)[https://fccid.io/EW4DWMW004](https://fccid.io/EW4DWMW004)[https://fccid.io/MCLJ27H010](https://fccid.io/MCLJ27H010)[https://fccid.io/EW4DWMW015](https://fccid.io/EW4DWMW015)[https://fccid.io/EW4DWMW024](https://fccid.io/EW4DWMW024)[https://fccid.io/MCLJ27H020](https://fccid.io/MCLJ27H020)[https://fccid.io/EW4DWMW028](https://fccid.io/EW4DWMW028)[https://fccid.io/MCLJ27H023](https://fccid.io/MCLJ27H023)[https://fccid.io/MCLJ27H02301](https://fccid.io/MCLJ27H02301)[https://fccid.io/BKERED001](https://fccid.io/BKERED001)
| GBA ;\GBA wireless adaptor
 GBA-Micro ;/(not wifi/wlan compatible)
 NDS (non-remove-able board)
 NDS-Lite (old remove-able board, with MM3155)
 NDS-Lite (new remove-able board, with MM3218)
 Wii
 Wii (mitsumi) (also W016, and maybe W014 ?)
 Wii (foxconn) (also H003 ?)
 DSi (old wifi board, mitsumi)
 DSi (new wifi board, mitsumi)
 DSi (new wifi board, foxconn)
 3DS (mitsumi)
 3DS (foxconn)
 2DS (foxconn) (also in 3DS XL)
 New3DS (on mainboard) |
| --- |

The pages include test reports, photos (including some prototypes), and most
interestingly, the Mitsumi "Label location" pages are including pinout &
signal names for most wifi boards.

**DSi Wifi board pinout**

[https://fccid.io/EW4DWMW024/Label/Label-format-and-location-1137926.pdf](https://fccid.io/EW4DWMW024/Label/Label-format-and-location-1137926.pdf)

[https://fccid.io/EW4DWMW015/Label/Label-Location-1031953.pdf](https://fccid.io/EW4DWMW015/Label/Label-Location-1031953.pdf)

```

| 1 MX_SD_CLK 2 GND
 3 GND 4 VDD_18
 5 SDIO_DATA0 6 VDD_18
 7 SDIO_DATA3 8 GND
 9 SDIO_DATA1 10 VDD_33
 11 SDIO_CMD 12 VDD_33
 13 SDIO_DATA2 14 GND
 15 JTAG_TDO 16 ATH_TX_H
 17 JTAG_TMS 18 SYS_RST_L
 19 GND 20 JTAG_TDI
 21 CLK32k 22 JTAG_TCK
 23 GND 24 JTAG_TRST_L
 25 NC(VDD28_TP) 26 SEL_ATH_L
 27 SPI_CS2 28 W_B /FLASH_WP
 29 BBP_SLEEP 30 SPI_CLK
 31 RF_SLEEP 32 SPI_DO MISO
 33 RF_SCS 34 SPI_DI MOSI
 35 BBP_SCS 36 CCA
 37 BB_RF_SDO 38 RXPE
 39 BB_RF_SDI 40 TRDATA
 41 BB_RF_SCLK 42 GND
 43 NC(VDD18_TP) 44 TRCLK
 45 GND 46 TRRDY
 47 MCLK 48 TXPE
 49 GND 50 RESET |
| --- |

**DS-Lite Wifi board DWM-W006 pinout**

[https://fccid.io/EW4DWMW006/Label/ID-label-format-and-location-706511.pdf](https://fccid.io/EW4DWMW006/Label/ID-label-format-and-location-706511.pdf)

```

| 1 GND 16 +3.3V
 2 TXPE 17 GND
 3 RXPE 18 RF_SCS
 4 CCA 19 BBP_SLEEP
 5 TRRDY 20 BBP_SCS
 6 GND 21 RF_SLEEP
 7 TRCLK 22 RESET
 8 TRDATA 23 GND
 9 GND 24 SPI_CLK
 10 BB_RF_SDO 25 SPI_DI MOSI
 11 BB_RF_SDI 26 SPI_DO MISO
 12 BB_RF_SCLK 27 W_B /FLASH_WP
 13 GND 28 SPI_CS2
 14 MCLK 29 LD ?
 15 GND 30 GND |
| --- |

**Wii Wifi board**

[https://fccid.io/EW4DWMW004/Label/ID-Label-670426.pdf](https://fccid.io/EW4DWMW004/Label/ID-Label-670426.pdf)

```

| 1 GND 12 VDD3.3
 2 SDIO_DATA_2 13 GND
 3 SDIO_DATA_1 14 GPIO_0
 4 GND 15 GND
 5 SDIO_CLK 16 SDIO_DATA_3
 6 GND 17 SDIO_DATA_0
 7 GPIO_1 18 GND
 8 GND 19 SDIO_CMD
 9 N.C.(VDD1.8) 20 GND
 10 N.C.(VDD1.8) 21 ANT_A (MAIN)
 11 VDD3.3 22 ANT_B (AUX) |
| --- |

**3DS Wifi Board DWM-W028**

[https://fccid.io/EW4DWMW028/Label/ID-label-location-1272988.pdf](https://fccid.io/EW4DWMW028/Label/ID-label-location-1272988.pdf)

```

| 1 MCLK 2 RF_CSRF
 3 GND 4 BB_CSBB
 5 RXPE 6 BB_RF_SDIN
 7 TXPE 8 BB_RF_SDOUT
 9 CCA 10 BB_RF_SCK
 11 TRDATA 12 GND
 13 TRCLK 14 BBP_SLEEP_L
 15 TRRDY 16 RF_SLEEP_L
 17 TRST_L 18 SEL_ATH_L
 19 GND 20 GND
 21 SDIO_DATA_0 22 JTAG_TDO
 23 SDIO_DATA_1 24 JTAG_TMS
 25 SDIO_DATA_2 26 JTAG_TDI
 27 SDIO_DATA_3 28 JTAG_TCK
 29 GND 30 SPI_CS2
 31 SDIO_CLK 32 W_B /FLASH_WP
 33 GND 34 SPI_CLK
 35 SDIO_CMD 36 SPI_DO MISO
 37 UART_TXD 38 SPI_DI MOSI
 39 UART_RXD 40 SYS_RST_L
 41 GND 42 ATH_TX_H
 43 CLK32k 44 RESET
 45 GND 46 GND
 47 VDD_18 48 VDD_33
 49 VDD_18 50 VDD_33 |
| --- |

Functionally same as DSi, but not pin-compatible, and with two UART pins
(instead of the NC pins).

GBA-Micro: device has two channels only: 2426.248MHz and 2456.576MHz.

| DS Wifi Dslink/Wifiboot Protocol |
| --- |

Below is the transfer protocol for nocash "wifiboot" and devkitpro "dslink".

All UDP/TCP message are using Port 17491 (4453h).

**Wifiboot UDP announce/reply and listen/connect/accept**

```

| PC sends UDP announce message (repeatedly, as broadcast) ;\
 console does TCP listen (if it isn't already listening) ; UDP and
 console sends UDP reply message (repeatedly, to PC) ; listen/accept
 PC does TCP connect (upon UDP reply) ;
 console does TCP accept (upon TCP connect) ;/ |
| --- |

The announce/reply strings in UDP messages depend on the file type:

```

| type NDS/DSi 3DS.firm 3DS.3dsx 3DS.gba
 announce "dsboot" "3dsfirmboot" "3dsboot" "gbaboot"
 reply "bootds" "bootfirm3ds" "boot3ds" "bootgba" |
| --- |

After passing through the announce/reply phase, the actual data is transferred
via TCP:

**Wifiboot TCP blocks for NDS/DSi**

```

| PC sends NDS header (170h bytes) ;SMALLER ;\
 PC sends Info Block (90h bytes) ;NEW ;
 console sends 32bit response word (4 bytes) ;
 PC sends Icon/Title (optional, if response.bit17) ;NEW ;
 PC sends DSi header (1000h bytes, if response.bit16) ; TCP transfer
 PC sends ARM7 bootcode ;
 PC sends ARM9 bootcode ;
 PC sends ARM7i bootcode (optional, if response.bit16) ;
 PC sends ARM9i bootcode (optional, if response.bit16) ;
 PC sends commandline 32bit length (00000000h=none) ;
 PC sends commandline string (if any) ;/ |
| --- |

**Wifiboot TCP blocks for 3DS.firm**

```

| PC sends FIRM header (200h bytes) ;\
 PC sends Info Block (90h bytes) ;
 console sends 32bit response word (4 bytes) ;
 PC sends Icon/Title (optional, if response.bit17) ;
 PC sends Logo (optional, if response.bit18) ; TCP transfer
 PC sends Banner (optional, if response.bit19) ;
 PC sends FIRM section 0 ;
 PC sends FIRM section 1 ;
 PC sends FIRM section 2 ;
 PC sends FIRM section 3 ;
 PC sends commandline 32bit length (0=none) ;
 PC sends commandline string (if any) ;/ |
| --- |

**Wifiboot TCP blocks for 3DS.gba**

```

| PC sends GBA Header (C0h bytes) ;\
 PC sends Info Block (90h bytes) ;
 PC sends GBA Footer (360h bytes) ;
 console sends 32bit response word (4 bytes) ; TCP transfer
 ;PC sends Icon/Title (optional, if response.bit17) ;
 ;PC sends Logo (optional, if response.bit18) ;
 ;PC sends Banner (optional, if response.bit19) ;
 PC sends GBA ROM-Image ;
 PC sends commandline 32bit length (0=none) ;
 PC sends commandline string (if any) ;/ |
| --- |

**Wifiboot TCP blocks for 3DS.3dsx**

Not supported by wifiboot, but supported by devkitpro. The exact protocol is
unknown, it seems to be similar to the above protocols, but with deflate
compression (see "3dslink" source code for details).

**Response word (32bit)**

This did originally contain 32bit error flags, and was later changed to contain
request flags in bit16 and up (nonetheless, uploaders should keep treating any
unexpected or unsupported request flags as errors).

```

| 0-x Error flags
 16 Request DSi header and ARM7i/ARM9i blocks (DSi)
 17 Request Icon/Title (NDS/DSi/3DS)
 18 Request Logo (3DS)
 19 Request Banner (3DS)
 20-31 Reserved (0) |
| --- |

The downloader should set request flags only if it wants to receive extra data,
and only if the header or info block indicates that data to be available.

**Info Block (90h bytes)**

This did formerly contain NDS file bytes 170h..1FFh (ie. as if the cartheader
were 200h bytes in size).

```

| 00h 8 Overall ID "BootINFO" (if other: ignore all entries below)
 08h 24 Uploader name/version, zeropadded (eg. "nocash wifiboot v2.6")
 20h 1 Time Seconds (BCD, 00h..59h) ;\
 21h 1 Time Minutes (BCD, 00h..59h) ; current time (local timezone)
 22h 1 Time Hours (BCD, 00h..23h) ; (for updating lost RTC time)
 23h 1 Time DayOfWeek (0..6, 0=Monday) ; (or all zeroes = none)
 24h 1 Time Day (BCD, 01h..31h) ;
 25h 1 Time Month (BCD, 01h..12h) ;
 26h 1 Time Year (BCD, 00h..99h) ;
 27h 1 Time Century (BCD, 00h..99h) ;/
 28h 4 Icon/Title Size (0=None, 840h/940h/A40h/23C0h=NDS/DSi, 36C0h=3DS)
 2Ch 4 Logo Size (0=None, Other=3DS only)
 30h 4 Banner Size (0=None, Other=3DS only)
 34h 5Ch Reserved (0) |
| --- |