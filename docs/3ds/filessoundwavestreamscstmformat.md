# Filessoundwavestreamscstmformat

> Source: https://problemkaputt.de/gbatek.htm
> Section: Filessoundwavestreamscstmformat

3DS Files - Sound Wave Streams (CSTM Format) 
| |
| --- |

 CSTM files are very similar to CWAV files, with slightly differently arranged data, and with an additional SEEK block.
 Unknown if/which games or system titles are using CSTM files. They can be reportedly found in following locations:
 
```

| - RomFS:\sound\stream\*.bcstm (eg. in Zelda Ocarina of Time)
 - Inside of a CSAR (unknown where exactly, maybe inside of CWAR blocks?)
 - CSAR's can contain RefID=220Dh entries for external CSTM file (eg. Zelda) |
| --- |

CSTM seems to be intended for streaming medium length recordings (longer than
normal CWAV instruments/effects, and shorter than fully fledged AAC/OGG/MP3
recordings).

**CSTM Header**

```

| 000h 4 ID "CSTM"
 004h 2 Byte Order (FEFFh)
 006h 2 Header Size (usually 40h)
 008h 4 Version (02000000h)
 00Ch 4 File Size
 010h 2 Number of Blocks (usually 3, INFO+SEEK+DATA)
 012h 2 Reserved
 014h 4+4+4 INFO Block Reference (RefID=4000h, Offset from CSTM+0, Size)
 020h 4+4+4 SEEK Block Reference (RefID=4001h, Offset from CSTM+0, Size)
 02Ch 4+4+4 DATA Block Reference (RefID=4002h, Offset from CSTM+0, Size)
 038h 8 Padding to 20h-byte boundary |
| --- |

```

| ________________________________ INFO Block _________________________________ |
| --- |

**Info Block**

```

| 000h 4 Chunk ID "INFO"
 004h 4 Chunk Size
 008h 4+4 Stream Info Entry (RefID=4100h, Offset relative to INFO+8)
 010h 4+4 Track Info List (RefID=4101h, Offset relative to INFO+8)
 018h 4+4 Channel Info List (RefID=4102h, Offset relative to INFO+8)
 020h 38h Stream Info Entry ;\
 058h .. Track Info List ; from above Header ;<-- 4+(N*8) ?
 .. .. Channel Info List ;/ ;<-- 4+(N*8) ?
 .. .. Track Info Entries ;\from above Lists
 .. .. Channel Info Entries ;/ |
| --- |

If encoding is DSP-ADPCM:

```

| .. .. DSP-ADPCM Info Entries |
| --- |

If encoding is IMA-ADPCM:

```

| .. .. IMA-ADPCM Info Entries |
| --- |

The info block is aligned to 20h bytes. uh, that is, the "Stream Info" block?

**Stream Info Entry**

```

| 000h 1 Encoding (0=PCM8, 1=PCM16, 2=DSP-ADPCM, 3=IMA-ADPCM)
 001h 1 Loop (0=Don't loop, 1=Loop)
 002h 1 Channel Count
 003h 1 Padding
 004h 4 Sample Rate
 008h 4 Loop Start Frame
 00Ch 4 Loop End Frame
 010h 4 Sample Block Count
 014h 4 Sample Block Size
 018h 4 Sample Block Sample Count
 01Ch 4 Last Sample Block Size
 020h 4 Last Sample Block Sample Count
 024h 4 Last Sample Block Padded Size
 028h 4 Seek Data Size
 02Ch 4 Seek Interval Sample Count
 030h 4+4 Sample Data (RefID=1F00h, Offset from "Data Block Data field") |
| --- |

**Track Info Entry**

```

| 000h 1 Volume
 001h 1 Pan
 002h 2 Padding
 004h 4+4 Channel Index Byte Table (RefID=0100h, Offset from Volume field)
 00Ch .. Channel Index Byte Table (at above Offset)
 .. .. Padding to 4-byte boundary (0) |
| --- |

Byte Table:

```

| 000h 4 Count
 004h Count Elements |
| --- |

**Channel Info Entry (aka Sample Info)**

```

| 000h 4+4 DSP-ADPCM Info (RefID=0300h, Offset from here?) ;\depending
 IMA-ADPCM Info (RefID=0301h, Offset from here?) ; on Encoding
 No ADPCM Info? (RefID=0000h, Offset=FFFFFFFFh) ;/ |
| --- |

DSP-ADPCM Info:

```

| 000h 32 Param (16bit Coefficients) ;-Whatever Coefficients
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

IMA-ADPCM Info:

```

| 000h 2 Data ;\Initial context
 002h 1 Table Index ; ...for port 10103418h+(N*20h)?
 003h 1 Padding ;/
 004h 2 Data ;\Loop context
 006h 1 Table Index ; ...for port 1010341Ch+(N*20h)?
 007h 1 Padding ;/ |
| --- |

```

| ________________________________ SEEK Block _________________________________ |
| --- |

**Seek Block**

```

| 000h 4 Chunk ID "SEEK"
 004h 4 Chunk Size
 008h .. Data ... whatever ? |
| --- |

The seek block is aligned to 20h bytes.

```

| ________________________________ DATA Block _________________________________ |
| --- |

**Data Block**

```

| 000h 4 Chunk ID "DATA"
 004h 4 Chunk Size
 008h .. Data ... probably sample data ? |
| --- |

The data block is aligned to 20h bytes, as well as the data field's actual
sample data.

```

| ________________________________ Notes..... _________________________________ |
| --- |

**Whatever Reference Type**

```

| RefID=0101h = Reference Table ? |
| --- |

**Tools**

The following tools can play BCSTMs and convert them to other formats:

```

| Isabelle Sound Editor
 vgmstream
 Every File Explorer |
| --- |