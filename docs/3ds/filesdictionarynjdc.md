# Filesdictionarynjdc

> Source: https://problemkaputt.de/gbatek.htm
> Section: Filesdictionarynjdc

3DS Files - Dictionary (NJDC) 
| |
| --- |

 NJDC files contain dictionaries (maybe for spellchecking?), NJDC files are found in:
 
```

| Shared Data Archives: Dictionary (RomFS:\..\*.a) ;for console/region languages
 System Applets: 3DS Browser applet (RomFS:\iwnn\..\*.a) ;for all languages |
| --- |

**Dictionary Format**

The files contain a header and several large tables. The header size (and the
location/presence of the header entries) varies from file to file; possibly
depending on some header flags?

All values, offsets and UTF-16 chars are BIG-ENDIAN.

```

| 000h 4 Header ID ("NJDC")
 004h 2 Unknown (1 or 2)
 006h 2 Unknown (0 or 1)
 008h 2 Unknown (4 or 5 or 0Fh)
 00Ah 2 Unknown (0)
 00Ch 4 Total Filesize minus a few bytes?
 010h 4 Unknown (00xxh)
 014h 4+4 Unknown (00xxh,00xxh) (usually both the same value)
 01Ch 4 Unknown (00xxh)
 ... .. ???
 ... 4 Offset to 10-byte Table
 ... 4 Offset to UTF-16 Strings
 ... 4 Offset to 2-byte Table
 .. .. Some zeroes and offsets?
 ... 4 Offset to 2 bytes? (08h,06h)
 ... 4 Values 00h,01h,01h,01h
 ... 4 Values 00h,01h,01h,01h
 ... .. ???
 ... 4 Number of words (N) ?
 ... .. ???
 ... .. 4-byte Table (maybe some lookup table?)
 ... 2 2 bytes? (08h,06h)
 ... N*10 10-byte Table (see below)
 ... .. UTF-16 Strings (raw strings, without trailing zeroes)
 ... N*2 2-byte Table (values 0101h*(40h..7Dh)?) (flags/lookup?) (if any)
 ... 4 Footer ID ("NJDC") (can be unaligned) |
| --- |

10-byte Table:

```

| 000h 1 String Length in bytes (always even, since chars are UTF-16)
 001h 3 String Offset (relative to String Table base)
 004h 6 Zerofilled |
| --- |

**Bad Word List**

Roughly related, there is Bad Word list in

```

| System Data Archives 2, NGWord (RomFS:*.txt) |
| --- |

The .txt files are UTF-16 (starting with FFh,FEh byte-order-marker).