# Videomvdregistersmoviedecoderorsonew

> Source: https://problemkaputt.de/gbatek.htm
> Section: Videomvdregistersmoviedecoderorsonew

3DS Video MVD Registers (Movie Decoder or so?) (New3DS) 
| |
| --- |

 Unknown. New3DS only. MVD might be short for Movie Decoder or so?
 Unknown if there are any FIFOs and CDMA peripheral IDs... there are none known yet... maybe the thing does have it's own "DMA" hardware for direct memory access without needing CDMA?
 
 **10207000h - New3DS: Movie Decoder or so? Rockchip...? (ARM11 only)**
 
```

| 10207000h 4 R 67312398h MVD Registers Chip ID?
 10207004h C4h R/W ffffffffh MVD Registers
 102070C8h 4 R 07b4af80h MVD Registers
 102070CCh 4 R/W ffffffffh MVD Registers
 102070D0h 4 - 00000000h MVD Registers
 102070D4h 4 - 00000000h MVD Registers
 102070D8h 4 R c09a0000h MVD Registers
 102070DCh 4 R/W ffffffffh MVD Registers
 102070E0h 4 - 00000000h MVD Registers
 102070E4h 4 R 8516ffffh MVD Registers
 102070E8h 4 - 00000000h MVD Registers
 102070ECh 40h R/W ffffffffh MVD Registers
 1020712Ch 4 - 00000000h MVD Registers
 10207130h 4 - 00000000h MVD Registers
 10207134h 4 - 00000000h MVD Registers
 10207138h 4 - 00000000h MVD Registers
 1020713Ch 44h R/W ffffffffh MVD Registers
 10207180h 4 - 00000000h MVD Registers
 10207184h 4 - 00000000h MVD Registers
 10207188h 4 - 00000000h MVD Registers
 1020718Ch 4 R ffffffffh MVD Registers
 10207190h 4 R ff874780h MVD Registers
 10207194h 6Ch - 00000000h Zerofilled
 10207200h E00h Mirrors of above 200h byte area |
| --- |

The ID value 67312398h appears to be known as "HW_ID" for linux/android "VPU
SERVICES". Searching for that two strings gives this source code,

```
[http://git.jp.linux-rockchip.org/cgit/rk3288_r-box_android4.4.2_sdk/tree](http://git.jp.linux-rockchip.org/cgit/rk3288_r-box_android4.4.2_sdk/tree)
| /kernel/arch/arm/mach-rockchip/vcodec_service.c |
| --- |

which defines VPU_DEC_ID_9190=6731h (the upper 16bit of the 67312398h value),
so the "MVD" might be identical to that Rockchip hardware (whatever that is).