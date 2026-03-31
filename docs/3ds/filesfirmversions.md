# Filesfirmversions

> Source: https://problemkaputt.de/gbatek.htm
> Section: Filesfirmversions

3DS Files - FIRM Versions 
| |
| --- |

 **New3DS ARM9 kernel - weird blurb**
 The only actual code-difference for the Old3DS/New3DS ARM9-kernels' crt0, besides TWL AES / 10012000h (=OTP?) related code, is that the New3DS ARM9-kernel writes 1 to REG_EXTMEMCNT9 (=CFG9_EXTMEMCNT9?) in the crt0 (unknown what crt0 means, elsewhere it's said that "0xffff8000 is crt0", although that does actually more look like a ROM address).
 
 **New3DS Process9**
 The following is all of the differences for Old3DS/New3DS Process9 with 9.3.0-X:
 The FIRM-launch code called at the end of the New3DS proc9 main() has different mem-range checks.
 In the New3DS proc9, the v6.0/v7.0 keyinit function at the very beginning (before the original code) had additional code added for setting CTRNAND keyslot 0x5, with keydata from .data. After setting the keyY, the keyY in .data is cleared.
 In New3DS proc9, the functions for getting the gamecard crypto keyslots / NCCH keyslot can return New3DS keyslots when New3DS flags (NCSD/NCCH) are set.
 The code/data for the binary near the end of arm9mem is slightly different, because of memory-region sizes.
 The only difference in .data (besides the above code binary) is that the New3DS proc9 has an additional 0x10-byte block for the keyslot 0x5 keyY, see above.
 
 **Variations**
 There exists different official firmwares for the 3DS: The default one (NATIVE_FIRM) is used to run all 3DS content and boots by default, while backwards compatibility is handled by TWL_FIRM and AGB_FIRM. There furthermore is a rescue mode provided by SAFE_MODE_FIRM.
 
 **NATIVE_FIRM**
 NATIVE_FIRM is the FIRM which is installed to the NAND firm partitions, which is loaded by bootrom.
 Version history:
 
```

| System
 version old 3DS
 title version old 3DS
 / hex title
 / contentID Kernel/FIRM
 / ________/ version (old
 / / ______________ 3DS/new 3DS) FIRM
 / / / ________________________ ARM11-sysmodule
 / / / / Product Code
 Factory v0 00 2.3-0 - ;Factory FIRM, titleID 00040001-00000002
 Pre-1.0 - - 2.23-X - ;Referenced in v1.0 Home Menu NCCH hdr
 1.0.0-0 v432 00 2.27-0 - ? ? 2011?
 1.1.0- v1472 02 2.28-0 - - - 2011?
 2.0.0- v2516 09 2.29-7 - - - 2011?
 2.1.0-3 v3553 0B 2.30-18 0608builder ? ? 2011?
 2.2.0-X ? v4595 0F 2.31-40 0909builder ? ? 2011?
 3.0.0- v5647 18 2.32-15 1128builder - - 2011?
 4.0.0-7 v6677 1D 2.33-4 0406builder ? ? 2012
 4.1.0- v7712 1F 2.34-0 0508builder - - 2012?
 5.0.0-11 v8758 25 2.35-6 0228builder ? ? 2013
 5.1.0-11 v9792 26 2.36-0 0401builder ? ? 2013
 6.0.0-11 v10833 29 2.37-0 0520builder 17 Jun 2013 ;Non-US: -12
 6.1.0-11 v11872 2A 2.38-0 0625builder 27 Jun 2013 ;Non-US: -12
 7.0.0-13 v12916 2E 2.39-4 1125builder 09 Dec 2013
 7.2.0-17 v13956 30 2.40-0 0404builder 12 May 2014
 8.0.0-18 v15047 37 2.44-6 0701builder 07 Jul 2014
 8.1.0-0 ? - - 2.45-5 - 26 Sep 2014 ;New3DS Launch
 9.0.0-20 v17120 38 2.46-0 0828builder 06 Oct 2014
 9.3.0-21 v18182 3F 2.48-3 1125builder 08 Dec 2014
 9.5.0-22 v19216 40 2.49-0 0126builder 02 Feb 2015
 9.6.0-24 v20262 49 2.50-1 0311builder 23 Mar 2015
 10.0.0-27 v21288 4B 2.50-7 0812builder 08 Sep 2015
 10.2.0-28 v22313 4C 2.50-9 1009builder 19 Oct 2015
 10.4.0-29 v23341 50 2.50-11 1224builder 18 Jan 2016
 11.0.0-33 v24368 52 2.51-0 0406builder 09 May 2016
 11.1.0-34 v25396 56 2.51-2 0805builder 13 Sep 2016
 11.2.0-35 v26432 58 2.52-0 1015builder 24 Oct 2016
 11.3.0-36 v27476 5C 2.53-0 0126builder 06 Feb 2017
 11.4.0-37 v28512 5E 2.54-0 0314builder 10 Apr 2017
 11.8.0-41 v29557 64 2.55-0 0710pseg-ciuser 30 Jul 2018
 11.12.0-44 v30593 66 2.56-0 1021pseg-ciuser 04 Nov 2019
 11.13.0-45 ? ? ? ? 02 Dec 2019 |
| --- |

Note: The above "builder" strings seem to be the actual Month/Day built-date.

**SAFE_MODE_FIRM**

SAFE_MODE is used for running the System Updater. SAFE_MODE_FIRM and
NATIVE_FIRM for the initial versions are exactly the same, except for the
system core version fields.

**TWL_FIRM**

TWL_FIRM handles DS(i) backwards compatibility.

The 3DS-mode ARM9 core seems to switch into DSi-mode (for running DSi-mode ARM9
code) by writing to a PDN register (this changes the memory layout to DSi-mode
/ etc, therefore this register poke *must* be executed from ITCM). This is the
final 3DS-mode register poke before the ARM9 switches into DSi-mode. DS(i)-mode
ARM7 code is run on the internal ARM7 core, which is started up during TWL_FIRM
boot. Trying to read from the exception-vector region (address 0x0) under this
DSi-mode ARM7 seems to only return 0x00/0xFF data. Also note that this DSi-mode
ARM7 runs code (stored in TWL_FIRM) which pokes some DSi-mode registers that on
the DSi were used for disabling access to the DSi bootROMs, however these
registers do not affect the 3DS DSi-mode ARM9/ARM7 "bootrom" region
(exceptionvector region + 0x8000) at all.

For shutting down the system, TWL_FIRM writes MCU[20h]=08h. For returning to
3DS-mode, TWL_FIRM writes to MCU[20h]=04h to trigger a hardware system reboot.

The TWL_FIRM ARM11-process includes a TWL bootloader, see here(?) and here(?)
for details.

TWL_FIRM verifies all TWL RSA padding with the following. This is different
from the DSi "BIOS" code.

The first byte must be 0x0.

The second byte must be 0x1 or 0x2.

Executes a while(<value of byte at current pos in RSA message>). When the
second_byte in the message is 0x1, the byte at curpos must be 0xFF (otherwise
the non-zero value of the byte at curpos doesn't matter). This loop must find a
zero byte before offset 0x7F in the message otherwise an error is returned.

Returns an address for msg_curpos+1.

totalhashdatasize = rsasig_bytesize - above position in the message for the
hashdata. The actual "totalhashdatasize" in the RSA message must be <=
<expected hashdata_size>(0x74 for bootloader). Uh, <=<>()??? The
TWL_FIRM code copies the RSA "hashdata" to the output buffer, using the actual
size of the RSA "hashdata".

**AGB_FIRM**

AGB_FIRM handles running GBA VC titles. The ARM9 FIRM section for TWL_FIRM and
AGB_FIRM are exactly the same (for TWL_FIRM and AGB_FIRM versions which were
updated with the same system-update).