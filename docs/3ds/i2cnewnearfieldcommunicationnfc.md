# I2Cnewnearfieldcommunicationnfc

> Source: https://problemkaputt.de/gbatek.htm
> Section: I2Cnewnearfieldcommunicationnfc

3DS I2C New3DS Near-Field Communication (NFC) 
| |
| --- |

 **I2C Bus/Device 1:EEh - Near-Field Communication (NFC) (New3DS-only)**
 This device is transferring command/response packets on the I2C bus (so one must only transfer Device+Data bytes; without any register Index values).
 
 **Datasheets and Specs**
 The NFC controller appears to be a 32pin Broadcom BCM20791 chip, there is no datasheet for that chip, however, a datasheet for 34pin BCM20793S chip does exist, but unfortunately without any specs on the transfer protocol.
 Specifications for the lower-level transfer protocol (and maybe even the I2C bus protocol?) are made by the "NFC Forum" (a non-profit organization that charges lots of money for a copy of those specs).
 There is some android open source code for "bcm2079x" available.
 
 **New3DS Transfer Protocol...**
 Command request / response structure:
 
```

| 00h 1 Normally 10h?
 01h 1 Command source / destination
 02h 1 CmdID
 03h 1 Payload size (LEN)
 04h LEN Data/parameters |
| --- |

The command response payload is usually at least 1-byte, where that byte
appears to be normally 00h. For command requests the payload data is the
command parameters.

For command requests sent to the NFC tag itself, Cmd[1]=0x0 and CmdID=0x0. The
command request payload data here is the actual command request data for the
NFC tag, starting with the CmdID u8 at payload+0.

During NFC module startup, a certain command (?) is sent to the controller
which eventually (after various cmd-reply headers etc) returns the following
after the first byte in the payload:

```

| 000000: 44 65 63 20 32 32 20 32 30 31 32 31 34 3a 35 33 Dec 22 201214:53
 000010: 3a 35 30 01 05 0d 46 05 1b 79 20 07 32 30 37 39 :50...F..y .2079
 000020: 31 42 35 1B5 |
| --- |

Or that is: "Dec 22 201214:53:50<binary>20791B5". Therefore, this appears
to return the part-number of the NFC controller (other command request(s) /
response(s) use this part-number value too).

Actually, 20791B5 seems to be NFC certification number for an unspecified
Broadcom product (or possibly 20791B5 is the product name itself... yes,
apparently it's Broadcom BCM20791).

Protocol = NFC Forum NFC Controller Interface (NCI) for host interface

**NFC controller commands**

```

| CmdRequest[1, uh?] CmdID Payload data for parameters
 2Eh 2Fh Firmware image for this chunk, size varies. |
| --- |

This is used during NFC module startup to upload the firmware image to the NFC
controller. This is used repeatedly to upload multiple chunks of the image.

**Power-up**

After power-up, the NFC chip automatically sends these bytes upon I2C reads:

```

| 10 60 00 02 00 01 ;followed by endless FF bytes |
| --- |

That seems to be a 4-byte header, with 2-byte data, as indicated in hdr[3]=02h.

**NFC Interrupt**

Triggers when there is data available for reading. Unknown if there is also a
way for determining if the thing is ready for writing a new command... maybe
one should simply wait for a response packet after sending any command packets?

**Old3DS Version**

The Old3DS did have an external NFC Adapter add-on, accessed via IrDA.

3DS NFC Adapter

The 3DS OS is supposedly having backwards/forwards compatible NFC functions,
but the underlying transfer protocol seems to be completely different (relying
on IrDA to communicate with the adpater firmare, instead of directly accessing
the BCM20791 chip).