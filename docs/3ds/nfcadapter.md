# Nfcadapter

> Source: https://problemkaputt.de/gbatek.htm
> Section: Nfcadapter

3DS NFC Adapter 
| |
| --- |

 The Nintendo NFC adapter, formally Nintendo NFC Reader/Writer and codenamed Fangate, is an external device which adds NFC capabilities for amiibos to old Nintendo 3DS and Nintendo 2DS consoles, using the infrared port on the back of the console.
 
 It launched simultaneously with Animal Crossing Happy Home Designer, with which it's optionally bundled; it can also be bought standalone at a nominal(?) price of 21 EUR.
 
 **Technical details**
 Based on analysis of the fangate_updater.bin file, which is part of the old Nintendo 3DS operating system since 9.3.0-21 and contains the firmware running on the external adapter; and analysis of the NFC Services running on old 3DS.
 
```

| SOC inside the adapter: Broadcom BCM20791B1
 or ST proprietary "MCU-FGT/rev.A/GH24S VQ"
 uh, either or? or rather both?
 is that guessed from fangate_updater.bin,
 or seen on photos of the actual pcb/hardware?
 CPU: ARM Cortex M0 |
| --- |

Memory map:

```

| Address Size Description
 08008000h 256KB? Firmware (fangate_updater.bin)
 20000000h 128KB? RAM
 40023C00h 1Ch FLASH ROM control
 E000ED00h 104h ARM Cortex system control block |
| --- |

**Layer 1 - IR communications framing format**

Packets are sent using IrDA-SIR (using ir:USER) at 115200 bps 8N1 (eight data
bits, no parity, one stop bit).

```

| IR framing format - short frame (max 3Fh data bytes):
 00h 1 Synchronization byte (A5h)
 01h 1 Reserved for future use (00h)
 02h 1 bit7:RFU (0), bit6:Short frame (0), bit0-5:Payload size
 03h N Payload
 03h+N 1 CRC-8-CCITT for whole packet [00h..N+02h] ;uh, before/after XORing?
 IR framing format - long frame (max 3FFFh data bytes):
 00h 1 Synchronization byte (A5h)
 01h 1 Reserved for future use (???)
 02h 1 bit7:RFU (0), bit6:Long frame (1), bit0-5: Payload size upper bits
 03h 1 Payload size (lower 8 bits)
 04h N Payload (XOR-encrypted)
 04h+N 1 CRC-8-CCITT for whole packet [00h..N+03h] ;uh, before/after XORing? |
| --- |

The payload is encrypted using a XOR-based encryption:

```

| halfCount = size/2 ;Divide by 2 rounding towards zero
 xorval = htobe16(0xE963) ;that is, BIG-ENDIAN ?
 for (i = 0; i < halfCount; i++)
 xorval = xorval XOR = src[i]
 dst[i] = xorval |
| --- |

**Layer 2 - "ircom"**

ircom is a simple stateful point-to-point master-slave communication protocol
built on top of IR layer 1.

```

| 00h 4 Random (makes the whole packet look random after XOR encryption)
 04h 1 bit4-7:RFU?, bit0-3:Protocol version (01h)
 05h 1 Connection ID of master (3DS), value determined by master
 06h 1 Connection ID of slave (NFC adapter), value determined by slave
 07h 1 bit4-7:???, bit0-3: Operation code
 08h N-8 Payload (if any) |
| --- |

Operation codes:

```

| Code Name Has payload Direction
 00h = Layer 3 command Yes Master to slave
 01h = ???
 02h = ???
 03h = ???
 04h = ???
 05h = ???
 06h = ???
 07h = ???
 08h = ???
 09h = ???
 0Ah = Disconnect request No Master to slave
 0Bh = Disconnection acknowledgment No Slave to master
 0Ch = Handshake No Master to slave
 0Dh = Handshake acknowledgement No Slave to master
 0Eh = ???
 0Fh = ??? |
| --- |

NFC adapter will ignore packets whose protocol version is not 1. It will not
even reply.

Connection identifier is a random byte the 3DS assigns to identify the
connection should there be several connections in range at once. Slave devices
must save this value from the initial handshake packet and use it for replies.
The 3DS will also save the connection identifier byte of the slave which is
also random. The 3DS must also ignore packets whose connection ID does not
match.

**Layer 3**

Layer 3 is the payload of layer 2. A lot is unknown of this layer and thus a
lot of assumptions were made in the following tables.

Layer 3 contains the following data:

```

| 00h bit4-7:Request identifier nibble, bit0-3: Always 01h
 01h Slave/master identifier byte
 02h Request type code upper byte
 03h Request type code lower byte
 04h+ Payload (if any) |
| --- |

Request identifier nibble is incremented by 0x1 at every new request by the
master, the same value for this byte is also sent by the slave in response to
the request of the master

Slave/master identifier byte is 0x1 for a message from the master and 0x10 for
a message from the slave

**Request type codes**

```

| Code Request description Direction Has payload
 0000h = ACK Slave to master Yes
 0003h = Get firmware version and battery level Master to slave No
 0004h = Firmware version and battery level Slave to master Yes
 0100h = Unknown, slave always responds with ACK Master to slave Yes
 0202h = Request to stop communication with NFC tag Master to slave No
 0204h = Get dumped data from NFC tag Master to slave No
 0205h = Data from NFC tag Slave to master Yes
 0206h = Request to start communication with NFC tag Master to slave Yes
 0207h = Request to write data to NFC tag Master to slave Yes |
| --- |

**0000h**

Acknowledgement message always send by slave. Payload always contains
000000AAh.

**0004h**

Payload contains firmware version and battery level of NFC adapter. Payload has
a size of 6 bytes.

```

| 00h Upper or lower byte of version number, newest firmware is 01h
 01h Upper or lower byte of version number, newest firmware is 06h
 02h Padding byte? Always 00h
 03h Padding byte? Always 00h
 04h Battery level (03h=Full, 00h=Empty)
 05h Padding byte? Always 00h |
| --- |

NFC reader LED already turns red when the battery level byte is 02h, this will
also trigger a low battery level warning on the 3DS.

**0100h**

The purpose of this request by the master is unknown. Slave always responds
with ACK. Payload of this request is always 0003E8AAh.

**0205h**

Payload contains data regarding NFC communication. The first byte of the
payload means the following:

```

| Code Description
 00h = NFC communication is stopped as result of a 0202h request from master
 01h = No NFC tag on top of the reader
 02h = Busy dumping NFC tag
 03h = NFC tag dump after write by master
 04h = NFC tag fully dumped
 05h = NFC tag dump after write by master
 07h = NFC tag not a NTAG215 or contains no Amiibo compatible data
 08h = NFC tag removed from reader |
| --- |

After the tag is written by the master and dumped again, the first few dumps
start with 03h, this changes to 05h after a few dumps. The reason for this is
unknown.

**0206h**

Request from master to start NFC communication. Payload always contains 19 00h
padding bytes, followed by C80300393A737486000001h and another 26 00h padding
bytes.

**0207h**

Request from master to write to NFC tag. The request packet already contains
the desired data to be written to the tag. Payload start with two 00h padding
bytes followed by the 7 ID bytes of the tag. These ID bytes are used by the NFC
adapter to check if same Amiibo is placed on the NFC adapter again.

**Samples**

NFC handshake beacons:

```

| Layer 1 packet Layer 2 packet Layer 3 packet
 A5 00 08 73 FE A5 C4 A4 2C A4 20 F5 9A 9D D6 3A 01 E8 00 0C ?
 A5 00 08 D1 3E B7 7B B6 91 B6 9D 87 38 5D 66 45 01 EA 00 0C ?
 A5 00 08 09 58 23 36 22 DA 22 D6 AE E0 3B 2A 6E 01 EC 00 0C ?
 A5 00 08 5E DD A4 A0 A5 4E A5 42 A8 B7 BE FA 7D 01 EE 00 0C ?
 A5 00 08 BC 19 C6 37 C7 C7 C7 CB 8B 55 7A 7A 2E 01 F0 00 0C ?
 A5 00 08 C9 15 F6 63 F7 91 F7 9D B2 20 76 3F 76 01 F2 00 0C ?
 A5 00 08 6E 48 47 1A 46 EE 46 E2 C7 87 2B 29 52 01 F4 00 0C ?
 A5 00 08 A2 8C E5 C3 E4 35 E4 39 74 4B EF 47 4F 01 F6 00 0C ?
 A5 00 08 26 1C 07 10 06 E8 06 E4 64 CF 7F 21 0C 01 F8 00 0C ?
 A5 00 08 7E 73 A2 3F A3 C5 A3 C9 FD 97 10 DC 4C 01 FA 00 0C ?
 A5 00 08 75 00 F3 B8 F2 44 F2 48 63 9C 63 86 B8 01 FC 00 0C ?
 A5 00 08 8D AC 0F D5 0E 2B 0E 27 72 64 CF 82 79 01 FE 00 0C ?
 A5 00 08 A3 55 7C 53 7D 52 7D 5E B2 4A 36 DF 06 01 01 00 0C ?
 A5 00 08 15 06 43 C0 42 C3 42 CF 85 FC 65 56 C6 01 03 00 0C ?
 A5 00 08 66 E0 9A 17 9B 12 9B 1E A0 8F 83 FC F7 01 05 00 0C ?
 A5 00 08 A4 35 09 97 08 90 08 9C 25 4D 56 AD A2 01 07 00 0C ?
 A5 00 08 73 E2 BD AF BC A6 BC AA 60 9A 81 CE 4D 01 09 00 0C ?
 A5 00 08 02 57 D7 B0 D6 BB D6 B7 28 EB 34 D5 E7 01 0B 00 0C ?
 A5 00 08 0D 79 01 AA 00 A7 00 AB 22 E4 1A 0C D3 01 0D 00 0C ?
 A5 00 08 14 91 04 B9 05 B6 05 BA B2 FD F2 10 28 01 0F 00 0C ?
 A5 00 08 2C 86 B1 49 B0 58 B0 54 C0 C5 E5 9D CF 01 11 00 0C ?
 A5 00 08 D5 1D DE DB DF C8 DF C4 F9 3C 7E 0B C6 01 13 00 0C ?
 A5 00 08 AF 75 DE 5C DF 49 DF 45 9C 46 16 71 29 01 15 00 0C ?
 A5 00 08 C8 E2 5B C6 5A D1 5A DD B5 21 81 93 24 01 17 00 0C ?
 A5 00 08 9B 51 68 2D 69 34 69 38 41 72 32 F3 7C 01 19 00 0C ?
 A5 00 08 13 7B 9F EF 9E F4 9E F8 32 FA 18 8C 94 01 1B 00 0C ?
 A5 00 08 A7 62 02 9C 03 81 03 8D BD 4E 01 A5 FE 01 1D 00 0C ?
 A5 00 08 39 06 94 36 95 29 95 25 09 D0 65 AD 30 01 1F 00 0C ?
 A5 00 08 32 4C D7 C0 D6 E1 D6 ED 92 DB 2F E5 8C 01 21 00 0C ?
 A5 00 08 83 BE F2 8F F3 AC F3 A0 B1 6A DD 71 31 01 23 00 0C ?
 A5 00 08 83 5E A0 57 A1 72 A1 7E F0 6A 3D 23 09 01 25 00 0C ?
 A5 00 08 6E C8 AD 69 AC 4E AC 42 D1 87 AB C3 A1 01 27 00 0C ?
 A5 00 08 C7 33 A1 2C A0 05 A0 09 FC 2E 50 66 1F 01 29 00 0C ? |
| --- |

**External links**

BCM2079x brief on Broadcom's website

Python scripts to sniff and spoof IR communication between the NFC reader and
3DS using an IrDA adapter