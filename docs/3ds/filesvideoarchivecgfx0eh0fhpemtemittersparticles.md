# Filesvideoarchivecgfx0Eh0Fhpemtemittersparticles

> Source: https://problemkaputt.de/gbatek.htm
> Section: Filesvideoarchivecgfx0Eh0Fhpemtemittersparticles

3DS Files - Video Archive (CGFX_0Eh,0Fh - PEMT Emitters/Particles) 
| |
| --- |

 - 3DS Files - Video Archive (CGFX_0Eh - PEMT Emitters)
 - 3DS Files - Video Archive (CGFX_0Fh - PEMT Particles)
 PEMT emitters is found in some CGFX files, eg. Mii Maker RomFS:\effects\..
 PEMT particles is found in some CGFX files, eg. 3DS Camera RomFS:\effect\P.pack
 
 **PEMT Format (as for CGFX_0Fh Particles) (unknown if CGFX_0Eh is same/similar)**
 
```

| 000h 4 Flags (eg. 40000006h)
 004h 4 ID "PEMT"
 008h 4 Revision (05000000h)
 00Ch 4 Name Offset (self-relative) (same as in parent DICT)
 010h 4+4 Unknown (0,0)
 018h 4+4 Unknown (1,1)
 020h ... Several zeroes and float32's ...?
 0B8h 4 Name Offset (self-relative) (whatever name?)
 0BCh 4 Unknown (24h) (Offset?)
 0C0h 4 Unknown (0)
 0C4h 4 Unknown (3F800000h) (or 40000000h) (float32?)
 0C8h 4 Unknown (0) (or 3DCCCCCDh)
 0CCh 4 Unknown (0Ah) (or 13Dh)
 0D0h 4 Unknown (0) (or 0)
 0D4h 4 Unknown (1) (or 0)
 0D8h 4 Unknown (168h) (or 3)
 0DCh 4 Unknown (0) (or 4) |
| --- |

Then there comes some more stuff at offset 0E0h (which might be addressed via
Offset 24h stored at [0BCh] in above structure?). That extra data can somehow
vary randomly with/without DICT at different locations (it seems to be related
on "Flags" in first which can be 04000000h, 10000000h, 20000000h, 40000000h,
80000000h, although there are two variants for Flags=20000000h).

Unknown how that makes sense... maybe some of the below stuff is totally
unrelated to PEMT (ie. it might be referenced from somewhere else, and just
happens to be located after PEMT).

3DS Camera RomFS:\effect\P.pack\ScoreE.bcptlLZ has three variants with DICT
(all in one file).

**Variant with DICT and Flags=20000000h**

```

| 000h 4 Flags? (20000000h)
 004h 4 Unknown (0)
 008h 4 Unknown (3F000000h)
 ... .. Floats and/or whatever
 02Ch 4 Name Offset (self-relative) (whatever name?)
 030h 4 Unknown (3) (or 2)
 034h 4+4 DICT for ... whatever (NumEntries,Offset)
 03Ch 4+4 Unknown (1,8) (or 4,8) (Num,Offset?) ----.
 044h 4 Unknown (0) (or 1) |
 048h .. Unknown (0) (or 3,7,5,2) <-----' |
| --- |

**Variant with DICT and Flags=40000000h**

```

| 000h 4 Flags? (40000000h)
 004h 4 Unknown (0)
 008h 4 Unknown (3Fxxxxxxh) ;\one extra word here?
 ... .. Floats and/or whatever ;/
 030h 4 Name Offset (self-relative) (whatever name?)
 034h 4 Unknown (3)
 038h 4+4 DICT for ... whatever (NumEntries,Offset)
 040h 4+4 Unknown (1,8) (Num,Offset?) ----.
 048h 4 Unknown (0) |
 04Ch .. Unknown (0) <-----' |
| --- |

**Variant with DICT and Flags=10000000h**

```

| 000h 4 Flags? (10000000h)
 004h 4 Unknown (0) ;\no floats here
 008h 4 Unknown (80000000h) ; (or only a few)
 00Ch 4 Unknown (0) ;/
 010h 4 Name Offset (self-relative) (whatever name?)
 014h 4 Unknown (3)
 018h 4+4 DICT for ... whatever (NumEntries,Offset)
 020h 4+4 Unknown (1,8) (Num,Offset?) ----.
 028h 4 Unknown (0) |
 02Ch .. Unknown (0) <-----' |
| --- |

Then comes the DICT (unless type is 04000000h)... the DICT is containing
whatever... it can have MANY entries, in unknown format(s).

**Variant without DICT and Flags=04000000h**

As found in Mii Maker RomFS:\effect\..\QR00.bcptl

```

| 000h 4 Unknown (04000000h)
 004h 4 Unknown (0) ;has only some floats, no DICT...
 008h 1Ch Floats and/or whatever
 024h 2 Uhm, two zero bytes? (then followed by Symbol table) |
| --- |

**Variant without DICT and Flags=20000000h (despite same Flags can have DICT)**

And Mii Maker also has some file with yet smaller DICT-less block, but with
20000000h (which usuallly DOES have DICT, but not in this case).

```

| 000h 4 Unknown (20000000h)
 004h 4 Unknown (0) ;has only some floats, no DICT...
 008h 18h Floats and/or whatever
 020h 2 Uhm, bytes 00h, 01h? (then followed by Symbol table) |
| --- |

**Variant without DICT and Flags=80000000h**

And, AR Games RomFS:\i_ar\effect\Ef_drawing.lz77

```

| 000h 4 Unknown (80000000h)
 004h 4 Unknown (0) ;has only some floats, no DICT...
 008h 10h Floats and/or whatever
 018h 4 Unknown (0) |
| --- |

Some of the DICT-less variants seem to contain two bytes (rather than a whole
32bit word) in last entry? Usually that's padded to 32bit alignment, but not in
the last entry (which occurs right before the string table, which can start at
a non-aligned location).