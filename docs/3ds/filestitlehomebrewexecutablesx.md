# Filestitlehomebrewexecutablesx

> Source: https://problemkaputt.de/gbatek.htm
> Section: Filestitlehomebrewexecutablesx

3DS Files - Title Homebrew Executables (3DSX) 
| |
| --- |

 The 3DSX format is a homebrew executable file format. The main feature is having relocateable code, which may have been required for some older exploits that didn't provide full control of the virtual memory mapping.
 As such, the .3dsx format is no longer useful (at least not once when having installed a bootloader), and one could instead use regular .code, .firm, .ncch, or .cia files for homebrew.
 
 **3DSX File Format**
 Header:
 
```

| 00h 4 ID "3DSX"
 04h 2 Header size (20h or 2Ch)
 06h 2 Relocation Header size (unspecified purpose) (always 8)
 08h 4 Format version (unspecified purpose) (always 0)
 0Ch 4 Flags (unspecified purpose) (always 0)
 10h 4 Code Segment Size
 14h 4 Rodata Segment Size
 18h 4 Data Segment Size (including bss)
 1Ch 4 Bss Segment Size |
| --- |

Extended Header (when [04h]>20h):

```

| (20h) 4 Icon/Title Offset (or ?=None)
 (24h) 4 Icon/Title Size (36C0h) (or ?=None)
 (28h) 4 RomFS Offset (or 0=None) |
| --- |

Relocation Header(s)

```

| .. 4 Number of absolute Code relocations (=maybe num skip/patch pairs?)
 .. 4 Number of relative Code relocations (unknown purpose, always 0)
 .. 4 Number of absolute Rodata relocations (=maybe num skip/patch pairs?)
 .. 4 Number of relative Rodata relocations (unknown purpose, always 0)
 .. 4 Number of absolute Data relocations (=maybe num skip/patch pairs?)
 .. 4 Number of relative Data relocations (unknown purpose, always 0) |
| --- |

Segments

```

| .. .. Code Segment
 .. .. Rodata Segment
 .. .. Data Segment (whatever... maybe excluding bss?) |
| --- |

Relocation Tables

```

| .. .. Code Relocation Table
 .. .. Rodata Relocation Table
 .. .. Data Relocation Table |
| --- |

Optional (when [04h]>20h):

```

| .. .. Icon/Title (SMDH Format)
 .. .. RomFS (RomFS Directory/File area, without IVFC header+SHA's) |
| --- |

Unknown how the executable could access the RomFS (it might be loaded to some
RAM address, which would be a waste of memory) (or otherwise the executable
would need to know the path\filename of the 3dsx file).

**Relocation Table (entries?):**

```

| 00h 2 Number of words to skip
 02h 2 Number of words to patch |
| --- |

Unknown what this does. The file is probably meant to be originated at a fixed
base address (maybe zero?), and one would add the actual load address to any
such "absolute" addresses? And do whatever on "relative" addresses?

**Note**

Some 3DSX files can be found at:

[http://www.3dbrew.org/wiki/Homebrew_Applications](http://www.3dbrew.org/wiki/Homebrew_Applications)