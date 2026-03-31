# Filesvideoiconssmdh

> Source: https://problemkaputt.de/gbatek.htm
> Section: Filesvideoiconssmdh

3DS Files - Video Icons (SMDH) 
| |
| --- |

 This page describes the format of the icon stored at CXI ExeFS:/icon and CIA icons. The CXI icon is displayed by Home Menu and System Settings (3DS Software Management), while CIA icons are dummies and not yet utilised by Dev 3DS' (as of rev 47586). The title strings are used in home menu (but are only displayed when selecting the largest icon size via buttons in upper-right).
 
 **Icon File Format (36C0h bytes)**
 
```

| 0000h 4 ID "SMDH"
 0004h 2 Version (0) (unknown/unspecified) (value 0000h exists)
 0006h 2 Reserved (0)
 0008h 200h Title Japanese ;\
 0208h 200h Title English ;
 0408h 200h Title French ; Each 200h-byte entry consists of:
 0608h 200h Title German ; 80h-byte Short Description
 0808h 200h Title Italian ; 100h-byte Long Description
 0A08h 200h Title Spanish ; 80h-byte Publisher
 0C08h 200h Title Simplified Chinese ; All encoded in UTF-16
 0E08h 200h Title Korean ;
 1008h 200h Title Dutch ;
 1208h 200h Title Portuguese ;
 1408h 200h Title Russian ;
 1608h 200h Title Traditional Chinese ;
 1808h 200h Title Reserved ;
 1A08h 200h Title Reserved ;
 1C08h 200h Title Reserved ;
 1E08h 200h Title Reserved ;/
 2008h 1 Age Rating CERO (Japan)
 2009h 1 Age Rating ESRB (USA)
 200Ah 1 Age Rating Reserved
 200Bh 1 Age Rating USK (German)
 200Ch 1 Age Rating PEGI GEN (Europe)
 200Dh 1 Age Rating Reserved
 200Eh 1 Age Rating PEGI PRT (Portugal)
 200Fh 1 Age Rating PEGI BBFC (England)
 2010h 1 Age Rating COB (Australia)
 2011h 1 Age Rating GRB (South Korea)
 2012h 1 Age Rating CGSRR (Taiwan)
 2013h 1 Age Rating Reserved
 2014h 1 Age Rating Reserved
 2015h 1 Age Rating Reserved
 2016h 1 Age Rating Reserved
 2017h 1 Age Rating Reserved
 2018h 4 Region Lockout (bit0=JPN, bit1=USA, bit2=EUR, bit3=AUS, bit4=CHN,
 bit5=KOR, bit6=TWN, bit7-31=Reserved) (7FFFFFFFh=Region Free)
 201Ch 4 Match Maker ID ;\Match Maker IDs (Online Play)
 2020h 8 Match Maker BIT ID ;/
 2028h 4 Flags
 202Ch 1 EULA Version Minor ;\
 202Dh 1 EULA Version Major ;/
 202Eh 2 Reserved (0)
 2030h 4 'Optimal Animation Default Frame' (for BNR) (uh, maybe preview?)
 2034h 4 CEC (StreetPass) ID (So the Home Menu knows which application
 icon to show the 'Green' CEC notification for)
 2038h 8 Reserved (0)
 2040h 480h Small Icon (24x24pix, used on top screen when pausing the app)
 24C0h 1200h Large Icon (48x48pix, used on lower screen when selecting titles) |
| --- |

**Flags [2028h]**

```

| 0 Visibility Flag (Required for visibility on the Home Menu) (1=show?)
 1 Auto-boot this gamecard title
 2 Allow use of 3D? (For use with parental Controls. An application
 can use the 3D affect, even when this flag isn't set)
 3 Require accepting CTR EULA before being launched by Home
 4 Autosave on exit? (see below)
 5 Uses an Extended Banner?
 6 Region game rating required
 7 Uses save data? (see below)
 8 Application usage is to be recorded. If this is zero, it causes
 the application's usage to be omitted from the Home Menu's icon
 cache, as well as in other places.
 9 unknown/unspecified (0)
 10 Disables SD Savedata Backups for this title. This is in addition to
 the blacklist.
 11 unknown/unspecified (0)
 12 New 3DS exclusive title. Shows an error if used on Old 3DS.
 13-31 unknown/unspecified (0) |
| --- |

**Icon Bitmaps**

The 24x24pix and 48x48pix icons consist of 8x8pix tiles with 16bpp (RGB565),
the pixels (within the 8x8pix tiles) have Z-order swizzling:

3DS Video Texture Swizzling

However, the tiles are arranged in linear order without swizzling (eg. for
48x48pix: Tile 00h..05h for upper two, Tile 06h..0Bh for next row, etc).

There is no power-of-2 padding for the width/height values (unlike CLIM/FLIM
files, which would pad 48x48pix to 64x64pix).

```
****
| *** Unknown if (or how) Transparency is supported *** |
| --- |

Most or all 3DS icons seem to have square shapes without transparent pixels.
The RGB565 format doesn't include alpha bits.

Unknown if there is a way to assign a transpaent color (eg. Magenta). Nintendo
is reportedly having a tool that can create SMDH files in formats other than
RGB565 (unfortunately without saying if such files contain some kind of Texture
Format entries).

**Misc**

Age Rating: Active ratings have a bitmask of 80h, and inactive ratings have no
bitmask at all. Ratings without the 80h bitmask are ignored. 40h bitmask
indicates Rating Pending. 20h bitmask indicates No Age Restriction. Age limits
are set by adding the minimal age to 80h (for example, limiting to 12 years and
up would give a bitmask of 8Ch)

Region Lockout: Regions are 'included' in region lock by setting their bitmask
value. Nintendo defines region free as 7FFFFFFFh. Early in the 3DS'
development, Nintendo grouped the Australian and Europe markets together.
Nintendo defines market Europe as having the combined bitmasks of Europe and
Australia. No 3DS' which check the Australia bitmask have been seen (Australia
uses the European 3DS model).

The EULA version is checked when the Accept EULA flag is set, the version is
compared to one stored in the 3DS. If the SMDH version is greater, then the
user will be prompted to accept the EULA.

**Effect of SaveData and AutoSave**

These options have no effect on the performance of the application itself:
they're used to select an appropriate warning when closing an application from
Home.

Both off: "Closing software" (no warning if quitting directly with X)

SaveData: "Do you want to close [...]? (Unsaved data will be lost.)"

AutoSave: ?

Both on: "Saving data and closing software..." (no warning if quitting directly
with X)

**'Optimal Animation Default Frame' (for BNR)**

This is a float, indicating the preferred (or 'most representative') frame for
the banner animation.

**CEC (StreetPass) ID**

This u32 represents the application CEC ID. This is likely loaded by
applications for use with the CEC services as well.

**Tools**

```

| CiTRUS - (GUI)(Windows Only) Generating ICN files (aka icon files)
 3DSExplorer - (GUI)(Windows Only) Parsing ICN files (aka icon files) |
| --- |