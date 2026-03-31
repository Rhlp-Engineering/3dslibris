# Filescrr0Andcro0Files

> Source: https://problemkaputt.de/gbatek.htm
> Section: Filescrr0Andcro0Files

3DS Files - CRR0 and CRO0 Files 
| |
| --- |

 CRO0 and CRR0 files are reportedly somehow related, but unknown how exactly.
 
```

| System Tools: Nintendo Network ID Settings and System Applets: Miiverse have
 "RomFS:\.crr\static.crr" ;-CRR0 (whatever)
 "RomFS:\cro\static.crs" ;-CRO0 (without actual code/data in there)
 System Data Arvives 1: web-lot/OSS CROs has
 "RomFS:\cro2\oss.cro" ;\both are CRO0's (with code/data each)
 "RomFS:\cro2\webkit.cro" ;/(but without any CRR0 file)
 System Applets 3DS Browser has
 "RomFS:\.crr\static.crr" ;-CRR0 (whatever)
 "RomFS:\*.cro.lex" ;-several LZ11 compressed .cro's
 "RomFS:\static.crs" ;-CRO0 (without actual code/data in there) |
| --- |

Unknown if/where any more such files exist.

```

| _________________________________ CRR0 Files _________________________________ |
| --- |

**CRR0 Files (must be stored under "RomFS:\.crr\")**

```

| 000h 4 ID "CRR0"
 004h 4 Reserved (0)
 008h 4 Next loaded CRR ;\must be zero in file,
 00Ch 4 Previous loaded CRR ;/set by RO during loading
 010h 4 Debug Info Offset (usually 0=None)
 014h 4 Debug Info Size (usually 0=None)
 018h 8 Reserved (0)
 020h 4 UniqueID Mask (eg. F00000FFh)
 024h 4 UniqueID Pattern (eg. zero)
 028h 18h Reserved (0)
 040h 100h RSA-2048 modulo (for signature at 240h..33Fh)
 140h 100h RSA-2048 signature over [020h+(0..11Fh)] signed by Nintendo
 240h 100h RSA-2048 signature over [340h+(0..357h?)] (uh, why that size?)
 340h 4 Process UniqueID<<8, from the ProgramID
 344h 4 Total Filesize
 348h 8 Reserved (0)
 350h 4 Hash-table Offset (usually 360h)
 354h 4 Hash-table Number of Entries (uh, 2 when having only 1 .crs file?)
 358h 4 Plain region Offset containing text tags, from process NCCH
 35Ch 4 Plain region Size
 360h 20h*N Hash-table (SHA256's on first 80h-bytes of CRO0's) (or not so??)
 ... .. Plain region (some SDK strings) (does that have any purpose?)
 ... .. Padding to 1000h-byte boundary (CCh-filled) |
| --- |

Each hash stored in the hash-table is over the (which?) corresponding
executable CRO0 hash-table.

```

| _________________________________ CRO0 Files _________________________________ |
| --- |

CRO with extension .cro is used for "DLLs" (unknown what that means... maybe
the functions are obtained via function name strings, as opposed to function
numbers used in normal System Modules?).

CRS with extension .crs is in the same format of CRO but storing the symbol
information of the static module (the main application) (uh, aka the
"ExeFS:\.code" file?) (uh, and, importantly, .crs files seem to contain only
symbols and stuff (and empty code/data sections)).

**CRO0 Files (.cro and .crs)**

```

| 000h 20h SHA256 on CRO0 header bytes 080h..17Fh ;\
 020h 20h SHA256 on code area (offset/size at [0B0h]) ; hash-table,
 040h 20h SHA256 on ? ; verified by CRR
 060h 20h SHA256 on .data area (offset/size at [0B8h]) ;/
 080h 4 ID "CRO0"
 084h 4 Name offset (same as [0C0h])
 088h 4 Zero (changed to Next loaded CRO pointer by RO during loading)
 08Ch 4 Zero (changed to Prev loaded CRO pointer by RO during loading)
 08Ch 4 Previous loaded CRO pointer, set by RO during loading
 090h 4 Total Filesize
 094h 4 .bss size
 098h 4 Unknown
 09Ch 4 Unknown
 Below four words are "Segment offset" (FFFFFFFFh if not exists)
 0A0h 4 Export symbol "nnroControlObject_" offset (FFFFFFFFh in .crs)
 0A4h 4 Function "OnLoad" (called on init)
 0A8h 4 Function "OnExit" (called on exit)
 0ACh 4 Function "OnUnresolved" (called on unresolved function calls)
 0B0h 4+4 Code Section offset, size
 0B8h 4+4 .data Section offset, size
 0C0h 4+4 Module Name offset, size
 0C8h 4+4 Segment Table offset, num (size=num*12)
 0D0h 4+4 Named Export Table offset, num (size=num*8)
 0D8h 4+4 Indexed Export Table offset, num (size=num*4)
 0E0h 4+4 Export Strings offset, size
 0E8h 4+4 Export Tree (fast lookup) offset, num (size=num*8) (trie-like)
 0F0h 4+4 Import Module Table offset, num (size=num*20)
 0F8h 4+4 Import Patches offset, num (size=num*12)
 100h 4+4 Named Import Table offset, num (size=num*8)
 108h 4+4 Indexed Import Table offset, num (size=num*8)
 110h 4+4 Anonymous Import Table offset, num (size=num*8)
 118h 4+4 Import Strings offset, size
 120h 4+4 unk8 offset, num
 128h 4+4 Relocation Patches offset, num (size=num*12)
 130h 4+4 unk9 offset, num
 138h 48h Zerofilled
 ... .. Code Section, etc. (in whatever order)
 ... .. Padding to 1000h-byte boundary (CCh-filled) |
| --- |

Segment offset (4 bytes) (used at [0A0h..0AFh], and also in many Table's)

```

| Bit0-3 Segment index for table
 Bit4-31 Offset into segment |
| --- |

Segment Table entry (12 bytes)

```

| 000h 4 Segment offset
 004h 4 Segment size
 008h 4 Segment id (0=.text, 1=.rodata, 2=.data, 3=.bss) |
| --- |

Named Export Table entry (8 bytes)

```

| 000h 4 Name offset
 004h 4 "Segment offset" for export |
| --- |

Indexed Export Table entry (4 bytes)

```

| 000h 4 "Segment offset" for export |
| --- |

Named Import Table entry (8 bytes)

```

| 000h 4 Name offset
 004h 4 Offset of the head of a linear list that contains the
 patches for this import |
| --- |

Indexed Import Table entry (8 bytes)

```

| 000h 4 Index of the export symbol
 004h 4 Offset of the head of a linear list that contains the
 patches for this import |
| --- |

Anonymous Import Table entry (8 bytes)

```

| 000h 4 "Segment offset" of the export symbol
 004h 4 Offset of the head of a linear list that contains the
 patches for this import |
| --- |

Import Module Table entry (20 bytes)

```

| 000h 4 Module name offset
 004h 4 Indexed import num
 008h 4 Offset of the head of a sub list in Indexed Import Table
 00Ch 4 Anonymous import num
 010h 4 Offset of the head of a sub list in Anonymous Import Table |
| --- |

Patch entry (12 bytes)

```

| 000h 4 "Segment offset" for output.
 004h 1 Patch type (apparently a subset of relocation type for ARM ELF):
 0=nothing/ignore
 2=38=write u32 absolute (base+addend)
 3=write u32 relative (base+addend-in_ptr)
 10=THUMB branch
 28=ARM32 branch
 29=modify ARM32 branch offset
 42=write u32 relative (((signed int)base*2)/2+addend-in_ptr)
 other=err
 005h 1 For import patches, non-zero if last entry; for relocation
 patches, this is the referred segment index
 006h 1 For import patches, 1 is written to first entry if all symbols
 loaded successfully; unknown (padding?) for relocation patches
 007h 1 Unknown (padding?)
 008h 4 addend |
| --- |

ARM32 branch instruction is constructed as follows:

```

| If addend > 02000000h or addend < FE000000h, then skip.
 If (addend&1) == 1 then write "b +4" (nop).
 Else write as normal. |
| --- |

Unknown what "addend" means... addition offset? end of addition?

When the RO module loads the entire CRO into process memory (mapped in the
00100000h-04000000h region), it modifies the mapped CRO data. The magic field
is also changed to "FIXD" if fix level is not 0.

Upon loading, the RO module will look for export symbol "nnroAeabiAtexit_" to
patch it to its import symbol "__aeabi_atexit".