# Filesvideo3Dphotosmpo

> Source: https://problemkaputt.de/gbatek.htm
> Section: Filesvideo3Dphotosmpo

3DS Files - Video 3D Photos (MPO) 
| |
| --- |

 The Multi-Picture Object (MPO) format developed by CIPA allows to store multiple images in one file, the 3DS (and other stereoscopic cameras) are using it to store 3D photos (ie. two images, from two different perspectives; that is, basically two JPEGs badged together, with Exif or JFIF headers in each JPEG, and with an additional MPO header in the first JPEG). The official MPO specs can be found here:
 
```
[http://www.cipa.jp/std/documents/e/DC-X007_E.pdf](http://www.cipa.jp/std/documents/e/DC-X007_E.pdf)
| |
| --- |

The 3DS Camera files are using MPOs with Exif headers, including some Nintendo
specific information in the Exif header.

There are also some other MPOs found in 3DS titles (eg. 3DS Nintendo Zone has a
MPO with JFIF header, and without Nintendo specific Exif data in
RomFS:\www\included_html\boss_page\BOSS_JP\shared_images\top_screen.mpo or 3DS
Camera has non-Nintendo Exif header in RomFS:\depug_mpo_\resizable.mpo).

**Nintendo's EXIF Data Structure**

```

| ID Description Value
 010Fh Manufacturer Nintendo
 0110h Camera Nintendo 3DS
 011Ah XResolution 72:1
 011Bh YResolution 72:1
 0128h ResolutionUnit 2
 0131h Software Depends on the software:
 00204 for the Camera (EU)
 00227 for the Mii Editor (EU)
 00955 for KH3D (EU)
 0132h DateTime YYYY:MM:DD HH:mm:SS
 0213h YCbCrPositioning 2
 9000h ExifVersion 0220
 9003h DateTimeOriginal YYYY:MM:DD HH:mm:SS
 9004h DateTimeDigitized YYYY:MM:DD HH:mm:SS
 9101h ComponentsConfiguration [0x01,0x02,0x03,0x00]
 927Ch MakerNote [DATA - 174 bytes] (yet to be understood)
 A000h FlashPixVersion 0100
 A001h ColorSpace 1
 A002h PixelXDimension 640
 A003h PixelYDimension 480
 5041h InteroperabilityIndex R98
 5042h ExifInteroperabilityVersion 0100
 1000h InteroperabilityVersion JPEG Exif Ver 2.2
 501Bh ThumbnailData [DATA - 3000+ bytes]
 5023h ThumbnailCompression 6
 502Dh ThumbnailXResolution 72:1
 502Eh ThumbnailYResolution 72:1
 5030h ThumbnailTransferFunction 2
 0201h JPEGInterchangeFormat 682
 0202h JPEGInterchangeFormatLength 3000+
 5091h ChrominanceTable [DATA - 128 bytes]
 5090h LuminanceTable [DATA - 128 bytes]
 Nintendo's IFDs (within IFD=927Ch)
 1100h Nintendo Software-specific data
 1101h Nintendo 64-byte Note |
| --- |

**IFD=927Ch - Nintendo's EXIF MakerNote Tag**

"This still need to be figured out: The MakerNote is made with Tiff Exif
information (Big Endian) first 2 bytes are the IFD Count (IFD is a 12 byte
tag), Regulary there are 2 IFDs."

**IFD=1100h - Nintendo's Software-specific data**

Unknown, reportedly "7-Undefined Count Count=1 ? Value : Offset" uh?

**IFD=1101h - Nintendo's 64-Byte Note (little endian, unlike the JPEG headers)**

```

| 000h 4 ID ("3DS1")
 004h 4 Unknown (can be 1, 2, 4, or 5)
 008h 4 Timestamp (number of seconds since 1st Jan 2000)
 00Ch 4 Padding (0)
 010h 4 Title ID Low ("but without the last two letters" uh?)
 014h 4 Flags
 018h 4 Console ID (unique identifier, unknown where from...?)
 01Ch 12 Padding (0)
 028h 4 3D Parallax "[-52,54]" ;2x float16? or 1x float32 in range -52..+54?
 02Ch 4 Padding (0)
 030h 2 Gender Category??? (1000h=mii, 2000h=man, 4000h=woman)
 032h 2 Filter (unknown meaning)
 034h 12 Padding (0) |
| --- |