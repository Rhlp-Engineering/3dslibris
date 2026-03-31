# Filessoundgroupcgrpformat

> Source: https://problemkaputt.de/gbatek.htm
> Section: Filessoundgroupcgrpformat

3DS Files - Sound Group (CGRP Format) 
| |
| --- |

 CGRP files are included in some CSAR files, but not always (for example, a CGRP can be found in the "AR Games" system title).
 The purpose of CGRP is unknown. The INFO block seems to be mainly good for re-ordering whatever indices. And the FILE block is somewhat empty.
 
 **CGRP Header**
 
```

| 000h 4 ID "CGRP"
 004h 2 Byte Order (FEFFh)
 006h 2 Header Size (usually 40h)
 008h 4 Version? (can be 01000000h)
 00Ch 4 Total Filesize
 010h 4 Number of Blocks (usually 2 = INFO+FILE)
 014h 4+4+4 INFO Block (RefID=7800h, Offset from CGRP+0, Size)
 020h 4+4+4 FILE Block (RefID=7801h, Offset from CGRP+0, Size)
 02Ch 14h Padding to 20h-byte boundary (0) |
| --- |

**INFO Block**

```

| 000h 4 ID "INFO"
 004h 4 INFO Block Size (same as in CGRP Header)
 008h 4 Number of following List entries (eg. 40h)
 00Ch (4+4)*N List Entries (RefID=7900h, Offset from INFO+8)
 ... .. Padding to 20h-byte boundary (0) |
| --- |

List Entry Format:

```

| 000h 4 Unknown (0..NumListEntries, in no specific order)
 (and apparently one value skipped, since using values
 in range 0..Num would be Num+1 values in total)
 004h 4 Unknown (FFFFFFFFh)
 008h 4 Unknown (FFFFFFFFh)
 00Ch 4 Unknown (0) |
| --- |

**FILE Block**

```

| 000h 4 ID "FILE"
 004h 4 FILE Block Size (same as in CGRP Header) (=20h)
 008h 18h Padding to 20h-byte boundary (0) |
| --- |

Apart from the above header, there is no actual data in the FILE block, unless
it's meant to be a prefix for the following CWAR blocks in the CSAR file?