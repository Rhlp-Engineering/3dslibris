# Filessoundwavedatacwavformat

> Source: https://problemkaputt.de/gbatek.htm
> Section: Filessoundwavedatacwavformat

3DS Files - Sound Wave Data (CWAV Format) 
| |
| --- |

 CWAV files contain audio samples for use as instruments or sound effects, somewhat resembling .wav files.
 The CWAV is usually stored in a CWAR Wave Archive (with the CWAR being stored in a CSAR Sound Archive). Additionally, a CWAV block can be found inside of Banner files. And, there are also standalone CWAV files (eg. in 3DS Camera "RomFS:\sound\*.bcwav").
 
 **CWAV Header**
 
```

| 000h 4 ID "CWAV"
 004h 2 Byte Order (FEFFh)
 006h 2 Header Size (usually 0040h)
 008h 4 Version (02010000h)
 00Ch 4 Total File Size
 010h 2 Number of Blocks (usually 2, INFO+DATA)
 012h 2 Reserved
 014h 4+4+4 INFO Block (RefID=7000h, Offset from CWAV+0, Size)
 020h 4+4+4 DATA Block (RefID=7001h, Offset from CWAV+0, Size)
 02Ch 14h Padding to 20h-byte boundary (0) |
| --- |

**Info Block**

```

| 000h 4 Chunk ID "INFO"
 004h 4 Chunk Size
 008h 1 Encoding (0=PCM8, 1=PCM16, 2=DSP-ADPCM, 3=IMA-ADPCM)
 009h 1 Loop (0=Don't loop, 1=Loop)
 00Ah 2 Padding (0)
 00Ch 4 Sample Rate (eg. 3E80h = 16000Hz?)
 010h 4 Loop Start Frame (0=Begin of sample?, or 0=None when not looping)
 014h 4 Loop End Frame (eg. 409h ?) (also End Frame when not looping)
 018h 4 Unknown (0)
 01Ch 4 Unknown (1)
 020h 4+4 Sample Info (RefID=7100h, Offset from INFO+8?) ;Offs=0Ch
 028h 4+4 Sample Data (RefID=1F00h, Offset from DATA+8) ;Offs=18h
 .. .. Sample Info
 .. .. ADPCM Info (if any)
 .. .. Padding to 20h-byte boundary (0) |
| --- |

Sample Info (RefID=7100h):

```

| 000h 4+4 DSP-ADPCM Info (RefID=0300h, Offset from INFO+?) ;Offs=14h ;\
 IMA-ADPCM Info (RefID=0301h, Offset from INFO+?) ;Offs=14h ;
 No ADPCM Info (RefID=0000h, Offset=FFFFFFFFh) ;None ;/
 008h 4 Unknown (0) |
| --- |

ADPCM Info (RefID=0300h) (when Encoding is DSP-ADPCM):

```

| 000h 32 16bit Coefficients ;-Whatever Coefficients
 020h 1 4bit Predictor + 4bit Scale ;\
 021h 1 Reserved (0) ; Initial context
 022h 2 Previous Sample ;
 024h 2 Second Previous Sample ;/
 026h 1 4bit Predictor + 4bit Scale ;\
 027h 1 Reserved (0) ; Loop context
 028h 2 Previous Sample ;
 02Ah 2 Second Previous Sample ;/
 02Ch 2 ? Padding ?? ;-uh? |
| --- |

ADPCM Info (RefID=0301h) (when Encoding is IMA-ADPCM):

```

| 000h 2 Data ;\Initial context
 002h 1 Table Index ; ...for port 10103418h+(N*20h)?
 003h 1 Padding ;/
 004h 2 Data ;\Loop context
 006h 1 Table Index ; ...for port 1010341Ch+(N*20h)?
 007h 1 Padding ;/ |
| --- |

**Data Block**

```

| 000h 4 Chunk ID "DATA"
 004h 4 Chunk Size
 008h 18h Padding to 20h-byte boundary (0)
 020h .. Sample Data (eg. PCM16) ;at Offsets specified in INFO block |
| --- |