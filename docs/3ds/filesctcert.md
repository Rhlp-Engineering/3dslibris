# Filesctcert

> Source: https://problemkaputt.de/gbatek.htm
> Section: Filesctcert

3DS Files - CTCert 
| |
| --- |

 Below seems to describe "CTCert".
 Uh, unknown if/where this "CTCert" is stored?
 Maybe it exists only after having connected to the shop at least once (similar to DSi's dev.kp)?
 Or maybe it's a slightly reformatted copy of the Decrypted OTP, maybe only temporarily stored in RAM?
 Or maybe below does describe .ctx file format rather than CTCert format?
 
 The console-unique CTCert is used for signing CTX files (uh, really signing them? with a public key??), the DeviceCert used by eShop, and for DSiWare Exports. This ECDSA signature, the ECDSA private key for this cert, and the IssuerID is loaded from memory initialized by bootrom instead of NAND. This is the 3DS equivalent of the DSi TWCert (aka the "TW cert" in DSi "Tad" files?).
 
 The CTCert is signed with ECDSA by Nintendo. CTCerts can be verified via a DER stored in NATIVE_FIRM, separate DERs are used for retail and dev/debug.
 
 **Structure**
 
```

| 000h 4 Signature Type (00010005h)
 004h 3Ch Signature ECDSA
 040h 40h Signature padding/alignment (zerofilled)
 080h 40h Signature Name (retail="Nintendo CA - G3_NintendoCTR2prod")
 (dev="Nintendo CA - G3_NintendoCTR2dev")
 0C0h 4 Key Type
 0C4h 40h Key Name "CT<DeviceId>-<ConsoleType>" aka "CTxxxxxxxx-xx"?
 DeviceId is the hex DeviceId (aka Decrypted OTP[04h..07h]?)
 ConsoleType is "00"=retail, "01"=dev
 104h 04 Key Expiration UNIX Time, big endian (seconds since 1 Jan 1970 ?)
 108h 3Ch Key Public ECDSA Key (point X,Y)
 "This is two consecutive u8 arrays (each one of length 1Eh),
 where the first one corresponds to the ECDSA R coordinate,
 and the second to ECDSA S coordinate. These are in big endian."
 144h 3Ch Key padding/alignment (zerofilled) |
| --- |

The ECDSA public key for this cert is converted from the ECDSA private key
initialized by bootrom (uh, that's probably meant to be from Decrypted OTP).
The curve used for ECDSA is sect233r1.