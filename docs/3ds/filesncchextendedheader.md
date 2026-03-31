# Filesncchextendedheader

> Source: https://problemkaputt.de/gbatek.htm
> Section: Filesncchextendedheader

3DS Files - NCCH Extended Header 
| |
| --- |

 The NCCH Exheader contains information for the .code file in the ExeFS (the .code file itself has no file header, and contains only raw code/data).
 
 The Exheader exists only if NCCH[180h]>0. That is, it does exist for NCCH's with ExeFS .code file. For some reason ExeFS .firm does also have Exheader (but unknown if it's used for anything). Whilst E-Manuals have only RomFS, without ExeFS, and without Exheader.
 
 **NCCH Extended Header (Exheader, at offset 200h and up)**
 
```

| 200h 200h SCI, System Control Info
 400h 200h ACI, Access Control Info 1
 600h 100h RSA-SHA256 Signature across [900h..BFFh] (using key from bootrom)
 700h 100h RSA Public Key for NCCH Header at [000h..1FFh]
 800h 200h ACI, Access Control Info 2 (for limitation of first ACI) |
| --- |

When loading the exheader, Process9 compares the exheader data with the data in
the AccessDesc (note that not everything is compared here). When these don't
match, an error is returned. The Process9 code handling this validation was
updated with v6.0; the only change in this function seems to be the check for
the "Ideal processor" field.

**System Control Info (SCI)**

```

| 200h 8 Application title (default is "CtrApp") or module name (eg. "mcu")
 208h 5 Reserved
 20Dh 1 Flags (bit0=Compressed .code file, bit1=SD Application)
 20Eh 2 Remaster version
 210h 4 Text code set info Address (usually 00100000h) ;\
 214h 4 Text code set info Size in 1000h-byte page units ; Code
 218h 4 Text code set info Size in bytes (excluding padding) ;/
 21Ch 4 Stack size (in bytes?) (usually 1000h) ;-Stack
 220h 4 Read-only code set Address (should be 1000h-page aligned) ;\
 224h 4 Read-only code set Size in 1000h-byte page units ; Const
 228h 4 Read-only code set Size in bytes (excluding padding) ;/
 22Ch 4 Reserved
 230h 4 Data code set info Address (should be 1000h-page aligned) ;\
 234h 4 Data code set info Size in 1000h-byte page units ; Data
 238h 4 Data code set info Size in bytes (excluding padding) ;/
 23Ch 4 BSS size (in bytes?) (usually XXXXh) ;-BSS
 240h 30h*8 Dependency module list (several 8-byte Title IDs)
 3C0h 8 SystemInfo SaveData Size
 3C8h 8 SystemInfo Jump ID
 3D0h 30h SystemInfo Reserved |
| --- |

Most of these fields are used in LOADER:LoadProcess.

"On retail for SD applications, exheader_systeminfoflags.flag bit1 must be
set."

**Access Control Info (ACI)**

```

| ARM11 Local System Capabilities:
 400h 8 Program ID (same as in NCCH Header)
 408h 4 Core version (Title ID low of required FIRM)
 40Ch 2 Flag1 and Flag2 (both implemented starting from 8.0.0-18).
 40Eh 1 Flag0
 40Fh 1 Priority
 410h 16*2 Resource limit descriptors ;1st byte controls max allowed CpuTime
 430h 8 Storage Info Extdata ID
 438h 8 Storage Info System savedata IDs
 440h 8 Storage Info Storage accessible unique IDs
 448h 8 Storage Info Filesystem Access Info (flags, see below)
 450h 32*8 Service Access Control
 550h 2*8 Extended service access control ;implemented with 9.3.0-X
 560h 15 Reserved
 56Fh 1 Resource limit category (0=APPLICATION, 1=SYS_APPLET,
 2=LIB_APPLET,
 3=OTHER (sysmodules running under the BASE memregion))
 ARM11 Kernel Capabilities:
 570h 70h ARM11 Kernel Capability Descriptors (28x32bit)
 5E0h 10h Reserved
 ARM9 Access Control:
 5F0h 15 ARM9 Access Control Descriptors
 5FFh 1 ARM9 Descriptor Version (must be 02h for original firmware,
 must be 02h or 03h for 9.3.0-X and up) |
| --- |

**Flag0**

```

| Bits Description
 0-1 Ideal processor
 2-3 Affinity mask
 4-7 Old3DS system mode:
 0 = Prod (64MB of usable application memory)
 1 = Undefined (unusable)
 2 = Dev1 (96MB of usable application memory)
 3 = Dev2 (80MB of usable application memory)
 4 = Dev3 (72MB of usable application memory)
 5 = Dev4 (32MB of usable application memory)
 6-7 = Undefined Same as Prod?
 8-15 = unknown/unspecified |
| --- |

In the exheader data, the ideal processor field is a bit-index, while in the
AccessDesc (the 2nd ACI at A00h and up) the ideal processor field is a bitmask.
When the bit specified by the exheader field is not set in the AccessDesc
field, an error is returned.

if((1 << exheaderval) & accessdescval == 0) return error

During a FIRM-launch when a TitleInfo structure was specified, the field at
offset 400h in the FIRM-launch parameters is set to the SystemMode of the
specified title, however in some cases other values are written there. With
8.0.0-18 NS will now check the output of PTMSYSM command 040A0000h, when the
output is non-zero and a certain NS state field is value-zero, the following is
executed otherwise this is skipped. With that check passed on 8.0.0-18, NS will
then check (Flag2 & 0Fh). When that is value2, the output value (used for
the FIRM-launcher parameter field mentioned above) is set to value7. Otherwise,
when that value is non-zero, the output value is set to 6.

**Flag1**

```

| Bits Description
 0 EnableL2Cache (Unknown what this actually does, New3DS-only presumably)
 1 cpuspeed_804MHz (Default "cpuspeed" when not set)
 2-7 Unused |
| --- |

In order for the exheader to have any of the above new bits set, the AccessDesc
must have the corresponding bit set, otherwise the invalid-exheader error is
returned.

Homebrew which runs under a title which has the above cpuspeed flag set, runs
much faster on New3DS. It's unknown how exactly the system handles these flags.

When launching titles / perhaps other things with APT, NS uses
PTMSYSM:ConfigureNew3DSCPU with data which originally came from these flags; NS
does this regardless of what the running 3DS system is. However, due to a
bug(?) in NS the value sent to that command is always either 00h or 03h. When
calculating that value, the code only ever uses the cpuspeed field, not the
cache field: code to actually load and check the value of the cache field
appears to be missing.

**Flag2**

```

| Bit Description
 0-3 New3DS system mode:
 0 = Legacy (use Old3DS system mode)
 1 = Prod (124MB of usable application memory)
 2 = Dev1 (178MB of usable application memory)
 3 = Dev2 (124MB of usable application memory)
 4-7 = Undefined Same as Prod?
 8-15 = unknown/unspecified
 4-7 Unused |
| --- |

When in Legacy mode, the actual memory layout is the same as in New3DS Prod,
except the available application memory as reported to the application is
reduced to the Old3DS size.

The exheader value for the New3DS system mode value must be equal to the
AccessDesc value, otherwise the invalid-exheader error is returned.

**Storage Info Filesystem Access Info**

```

| Bit Description
 0 Category system application
 1 Category hardware check
 2 Category filesystem tool
 3 Debug
 4 TWL card backup
 5 TWL NAND data
 6 BOSS
 7 sdmc:/
 8 Core
 9 nand:/ro/ (Read Only)
 10 nand:/rw/
 11 nand:/ro/ (Write Access)
 12 Category system settings
 13 Cardboard
 14 Export/Import IVS
 15 sdmc:/ (Write-only)
 16 Switch cleanup (Introduced in 3.0.0?)
 17 Savedata move (Introduced in 5.0.0)
 18 Shop (Introduced in 5.0.0)
 19 Shell (Introduced in 5.0.0)
 20 Category home menu (Introduced in 6.0.0)
 21 Seed DB (Introduced in 9.6.0-X FIRM, Home Menu
 has this bit set starting with 9.6.0-X)
 22-55 Reserved
 56 Not use RomFS
 57 Use Extended Savedata Access
 58-63 Reserved |
| --- |

When bit57 is set, the "Extdata ID" and "Storage Accessable Unique IDs" regions
are used to store a total of 6 "Accessible Save IDs". Introduced in 6.0.0.

**Service Access Control**

This is the list of services which the process is allowed to access, this is
registered with the services manager. Each service listed in the exheader must
be listed in the AccessDesc, otherwise the invalid exheader error is returned.
The order of the services for exheader/AccessDesc doesn't matter. The
AccessDesc can list services which are not in the exheader, but normally the
service-access-control data for exheader/AccessDesc are exactly the same.

This list is submitted to SRVPM:RegisterProcess.

**ARM11 Kernel Capability Descriptors**

The kernel capability descriptors are passed to svcCreateProcess.

There are different descriptor types, determined by the number of leading ones
in the binary value representation of bits 20-31. The different types are laid
out as follows:

```

| Bit31-20 Expl.
 1110xxxxxxxx Interrupt info
 11110xxxxxxx System call mask
 Bits 24-26: System call mask table index
 Bits 0-23: mask
 1111110xxxxx Kernel release version
 Bits 8-15: Major version
 Bits 0-7: Minor version
 11111110xxxx Handle table size
 Bits 0-18: size
 111111110xxx Kernel flags
 Bit Description
 0 Allow debug
 1 Force debug
 2 Allow non-alphanum
 3 Shared page writing
 4 Privilege priority
 5 Allow main() args
 6 Shared device memory
 7 Runnable on sleep
 8-11 Memory type (1: application, 2: system, 3: base)
 12 Special memory
 13 Process has access to CPU core 2 (New3DS only)
 11111111100x Map address range
 Describes a memory mapping like the 111111111110
 descriptor, but an entire range rather than a single
 page is mapped. Another 11111111100x descriptor
 must follow this one to denote the (exclusive) end
 of the address range to map
 111111111110 Map memory page
 Bits 0-19: page index to map (virtual address >> 12;
 the physical address is determined per-page according
 to Memory layout); Bit 20: Map read-only (otherwise
 read-write) |
| --- |

**ARM9 Access Control Descriptors**

```

| Bit Description
 0 Mount nand:/
 1 Mount nand:/ro/ (Write Access)
 2 Mount twln:/
 3 Mount wnand:/
 4 Mount card SPI
 5 Use SDIF3
 6 Create seed
 7 Use card SPI
 8 SD application (Not checked)
 9 Mount sdmc:/ (Write Access) |
| --- |