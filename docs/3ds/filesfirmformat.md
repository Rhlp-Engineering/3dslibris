# Filesfirmformat

> Source: https://problemkaputt.de/gbatek.htm
> Section: Filesfirmformat

3DS Files - FIRM Format 
| |
| --- |

 File format for the 3DS' Firmware, it contains up to four 'sections' of data comprising the ARM9 and ARM11 kernels, and some fundamental processes.
 The firmware sections are not encrypted.
 
 The ARM9 section contains the ARM9 kernel (and loader) and the Process9 NCCH (which is the only process run in user mode on the ARM9). The ARM11 sections contain the ARM11 kernel (and loader), and various ARM11 process NCCHs. For NATIVE_FIRM/SAFE_MODE_FIRM these ARM11 processes are sm, fs, pm, loader, and pxi. Normally the 4th section is not used. The code loaded from FIRM is constantly running on the system until another FIRM is launched. The ARM11 kernel is hard-coded to always decompress the ExeFS .code of embedded ARM11 NCCHs without checking the exheader compression bit.
 
 FIRM Format can exist in two locations:
 
```

| As raw eMMC sectors (defined in the NCSD Header of the eMMC storage)
 As .firm files (in NCCH .app files, in the "\title\00040138" firmware folder) |
| --- |

And, in Wifi-Flash and NTRCARDs (that's both only for repair/unbricking).

**FIRM Header**

```

| 000h 4 ID "FIRM"
 004h 4 Boot priority (0=Normal/Lowest)
 008h 4 ARM11 Entrypoint (usually 1FFxxxxxh, aka AXI WRAM.. or DSP?)
 00Ch 4 ARM9 Entrypoint (usually 080xxxxxh, aka ARM9-only RAM)
 010h 30h Reserved (0)
 040h 30h Firmware Section Header 1 (usually several NCCH's with ExeFS)
 070h 30h Firmware Section Header 2 (usually ARM11 code)
 0A0h 30h Firmware Section Header 3 (usually ARM9 code & Process9 NCCH)
 0D0h 30h Firmware Section Header 4 (often unused, zerofilled)
 100h 100h RSA-2048 SHA-256 signature of the FIRM header |
| --- |

The RSA signature is checked when bootrom/Process9 are doing FIRM-launch (with
the public key being hardcoded in each). The signature is not checked when
installing FIRM to the NAND firm0/firm1 partitions.

**Firmware Section Headers**

```

| 000h 4 Byte offset (relative to begin of FIRM header)
 004h 4 Physical address where the section is loaded to
 008h 4 Byte-size (or 0=None)
 00Ch 4 Copy-method (0=NDMA, 1=XDMA, 2=CPU mem-copy)
 (0=ARM9/WRAM, 1=ARM11/AXI/VRAM, 2=?)
 010h 20h SHA-256 Hash of Firmware Section |
| --- |

Note on Copy-method: Process9 ignores this field. Boot9 doesn't immediately
throw an error when this isn't 0..2. In that case it will jump over
section-data-loading which then results in the hash verification with the below
hash being done with the hash already stored in the SHA hardware.

**FIRM Load Areas**

Common Load areas are:

```

| 08006000h 0.9M ARM9-WRAM (common for ARM9 code) (not first/last some bytes)
 18000000h 6.0M VRAM (eg. used by GBA/DSi firmwares)
 1FF00000h 0.5M DSP Memory (eg. used by FIRM0)
 1FF80000h 0.5M AXI RAM (common for ARM11 code) |
| --- |

With that areas, a FIRM file can be max 7.9Mbyte (if it is loaded as file, the
default size of the firm0/firm1 partitions is only 4Mbyte each).

There is a blacklist in bootrom preventing some memory regions:

```

| 07FFB800h..07FFFBFFh ARM9 ITCM part (otp, mbr, keys...)
 FFF00000h..FFF02FFFh ARM9 DTCM (first 3000h) (arm9 data)
 FFF03000h..FFF03FFFh ARM9 DTCM (last 1000h) (arm9 stack)
 080F8000h..080FFFFFh ARM9 WRAM (last 8000h) (rom card related?)
 08000000h..0800003Fh ARM9 WRAM (first 40h) (exception vectors, etc)
 20000000h..27FFFFFFh FCRAM (whole 128MB)
 FFF00000h..1FFFFFFFh Bugged (size of that area is negative/nonsense) |
| --- |

Moreover, the bootrom doesn't have Main RAM and New3DS memory enabled. And, the
bootrom PU has also disabled some memory areas (though those might pass when
using DMA for loading?).