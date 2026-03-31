# Filesvideobannercbmd

> Source: https://problemkaputt.de/gbatek.htm
> Section: Filesvideobannercbmd

3DS Files - Video Banner (CBMD) 
| |
| --- |

 CBMD - CTR Banner Model Data (archive for CGFX/CWAV files)
 These are stored in CXI ExeFS:/banner and optionally in extdata exbanner.
 This is used for banners of titles you see in the home menu.
 BNR used for the app banners in the CXI/exbanner is the same as CBMD with CWAV at the end (uh, who is "BNR", and is that CWAV always supported in all cases, or only for that "BNR" thing?).
 
 **CBMD Header**
 
```

| 000h 4 ID "CBMD"
 004h 4 Zero
 008h 4 CGFX Offset for Default (88h) ;-main CGFX file (for all regions)
 00Ch 4 CGFX Offset for EUR-English ;\
 010h 4 CGFX Offset for EUR-French ; region-specific CGFX files
 014h 4 CGFX Offset for EUR-German ; (or 0=None)
 018h 4 CGFX Offset for EUR-Italian ; (these are smaller CGFX files,
 01Ch 4 CGFX Offset for EUR-Spanish ; usually with one or two custom
 020h 4 CGFX Offset for EUR-Dutch ; Textures) (3DS Activity Log has
 024h 4 CGFX Offset for EUR-Portuguese ; more custom entries though)
 028h 4 CGFX Offset for EUR-Russian ; (Nintendo Network ID Settings
 02Ch 4 CGFX Offset for JPN-Japanese ; doesn't have any custom CGFX's)
 030h 4 CGFX Offset for USA-English ;
 034h 4 CGFX Offset for USA-French ; (unknown if CHN/KOR/TWN are
 038h 4 CGFX Offset for USA-Spanish ; also supported?)
 03Ch 4 CGFX Offset for USA-Portuguese ;/
 040h 44h Padding (0) ;<-- XXX probably CHN/KOR/TWN here?
 084h 4 CWAV Offset for Sound wave data
 088h .. CFGX/CWAV data area... |
| --- |

"The common CGFX is used if the CGFX offset for the system region/language is
zero. Those optional offsets can be used in extdata exbanner, but separate CBMD
banner files for each region/language can be used as well."

**Entry Size**

The header contains only offsets. To compute the size, search the next bigger
offset (or use the filesize as max offset), and then subtract the current
offset from that value. Note: The list does usually contain increasing offsets
(but it may be a bad idea to rely on that, eg. EUR-Spanish and USA-Spanish
might use the same offsets).

**CGFX Graphics**

3DS Files - Video Archive (CGFX)

Banner CGFX files are LZ11-compressed, the decompressed CGFX size shouldn't
exceed 80000h bytes for banners.

**CWAV Sound**

3DS Files - Sound Wave Data (CWAV Format)

Banner CWAV total channels must be 2, and the length of the audio must be 3
seconds or less (uh, is that regardless of sample rate/sample size?), otherwise
the sound will play incorrectly (beeping/clicking) or the model may fail to
load.

| 3DS Files - Video Banner (Extended Banner) |
| --- |

**Overview**

The Extended Banner is used to add text (and optionally an extra texture) to a
given banner. It can also (optionally) be used to set an expiration date for a
banner.

See "\user\ExBanner\" for how the extended-banners are loaded from extdata. The
Home Menu extended-banner loading function will immediately return without
loading anything if the programID is for System Settings.

**System titles using extended banners**

```

| Mii Maker
 Face Raiders |
| --- |

**Format - Extension: .BIN (...stored somewhere as extdata on SD card?)**

```

| 000h 2 Texture width (if texture is used)
 002h 2 Texture height (if texture is used)
 004h 2 Texture colour format (if texture is used)
 008h 4 Expire Date, Year ;\
 00Ch 2 Expire Date, Month ; what is that, ASCII? or 32bit year???
 010h 2 Expire Date, Day ;/
 014h 15h Name of texture (if texture is used) ;uh, 15h may mean 16 decimal?
 024h 200h Plain text comment (255 character max) to be displayed in the
 banner on the HOME Menu (UTF-16 or so?)
 224h 80h unknown/unspecified
 2A4h .. Texture data (if any) |
| --- |

The date used for never-expiring exbanners varies, on retail like with Mii
Maker this is 31 Dec 2099 (year=833h, month=0Ch, day=1Fh). The
expiration-timestamp is only used for SpotPass exbanners, not
extdata-exbanners.

**Texture Colour Formats**

```

| 00h = RGBA8
 01h = RGB8
 02h = RGBA5551
 03h = RGBA565
 04h = RGBA4
 05h = LA8
 06h = HILO8
 07h = L8
 08h = A8
 09h = LA4
 0Ah = L4
 0Bh = A4
 0Ch = ETC1
 0Dh = ETC1A4 |
| --- |

3DS GPU Texture Formats

**SpotPass**

When Home Menu loads extended-banners, it also attempts to load a "CBMD" banner
via SpotPass service commands. Normally this CBMD banner doesn't exist in
extended-banner extdata. This is broken with New3DS titles since Home Menu uses
these BOSS commands with the New3DS bitmask in the programID set. The common
and language-specific (when offset is non-zero) "CGFX" specified by the CBMD
are decompressed and processed.

The "CGFX" sections in this CBMD are actually the exact same exbanners loaded
from normal extdata. The exbanner data from SpotPass is stored to the same
state as the extdata-exbanners. No CWAV is loaded from SpotPass data.

The exbanners from SpotPass must have a timestamp less than current_datetime,
otherwise they won't be parsed. The timestamp for the banner is calculated
with: nintimestamp_mktime(out, exbanner->year, exbanner->month,
exbanner->day, <hour=23>, <minute = 59>, <second = 59>,
<millisecond = 999>);