# Filessounddspbinarydsp1Akacdc

> Source: https://problemkaputt.de/gbatek.htm
> Section: Filessounddspbinarydsp1Akacdc

3DS Files - Sound DSP Binary (DSP1 aka .cdc) 
| |
| --- |

 Teak DSP binaries are usually having extension .cdc (supposedly unrelated to the "cdc" codec services). DSP binaries are found in some titles (eg. 3DS eShop RomFS:\sound\dspaudio.cdc, 3DS Browser Applet RomFS:\snd\dspaudio.cdc, and (LZ11-compressed) 3DS Nintendo Zone RomFS:\dspaudio.cdc.nwcx).
 Unknown if there is some kind of default DSP driver for use by other titles.
 
 **Header (300h bytes)**
 
```

| 000h 100h RSA Signature across header bytes [100h-2FFh]
 100h 4 ID ("DSP1")
 104h 4 Total Filesize
 108h 1 Code Memory (bit0-7, for 8 32Kbyte blocks) (usually FFh)
 109h 1 Data Memory (bit0-7, for 8 32Kbyte blocks) (usually FFh)
 10Ah 3 Zero (00h,00h,00h)
 10Dh 1 Special Segment DSP Area (00h..02h) (usually 02h=Data)
 10Eh 1 Number of segments (must be 01h..0Ah)
 10Fh 1 Flags (bit0=RecvData, bit1=WithSpecialSegment)
 110h 4 Special Segment DSP Address in 16bit words
 114h 4 Special Segment Size in bytes (usually 214h)
 118h 8 Zero
 Segment record 0..9:
 120h+N*30h 4 Segment N File Offset in bytes (300h and up)
 124h+N*30h 4 Segment N DSP Address in 16bit words (max 1FFFFh or 0FFFFh)
 128h+N*30h 4 Segment N Size in bytes
 12Ch+N*30h 3 Segment N Zero
 12Fh+N*30h 1 Segment N DSP Area (00h..02h)
 130h+N*30h 20h Segment N SHA256 across Segment Data
 300h .. Segment Data.. |
| --- |

DSP Areas at [10Dh] and [124h+N*30h] can be:

```

| 0 = DSP Code Memory (at ARM:1FF00000h and up) (DSP:00000h..1FFFFh)
 1 = DSP Code Memory (at ARM:1FF00000h and up) (DSP:00000h..0FFFFh only?)
 2 = DSP Data Memory (at ARM:1FF40000h and up) (DSP:00000h..0FFFFh only?) |
| --- |

**Flags.bit0=RecvData**

If set, DSP module calls DSP:RecvData on "all three registers" and expects them
to reply value, uh, that is... probably done as handshake AFTER uploading the
DSP binary?

**Flags.bit1=WithSpecialSegment**

If set, 214h bytes are read from CfgS:GetConfigInfoBlk8 block 70000h, and then
copied to the special segment area/address. If the reading fails, zeroes are
written in its place. The purpose of this segment is currently unknown.