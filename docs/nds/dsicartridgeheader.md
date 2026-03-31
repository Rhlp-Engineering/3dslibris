# Dsicartridgeheader

> Source: https://problemkaputt.de/gbatek.htm
> Section: Dsicartridgeheader

DSi Cartridge Header 
| |
| --- |

 **Old NDS Header Entries**
 The first 180h bytes of the DSi Header are essentially same as on NDS:
 DS Cartridge Header
 New/changed entries in DSi carts are:
 
```

| 012h 1 Unitcode (00h=NDS, 02h=NDS+DSi, 03h=DSi) (bit1=DSi)
 01Ch 1 NDS:Reserved, DSi:Flags (03h=Normal, 0Bh=Sys, 0Fh=Debug/Sys)
 bit0 Has TWL-Exclusive Region ;MUST be 1 for DSi titles?
 bit1 Modcrypted (0=No, 1=Yes, see [220h..22Fh])
 bit2 Modcrypt key select (0=Retail, 1=Debug)
 bit3 Disable Debug ?
 01Dh 1 NDS:Region, DSi:Permit jump (00h=Normal, 01h=System Settings)
 bit0 jump (always include title at [2FFD800h..])
 bit1 tmpjump (?)
 068h 4 Icon/Title offset (same as NDS, but with new extra entries)
 080h 4 Total Used ROM size, EXCLUDING DSi area
 088h 4 NDS:Unknown, DSi:ARM9 Parameters Table Offset ??? ;base=[028h]
 08Ch 4 NDS:Reserved, DSi:ARM7 Parameters Table Offset ??? ;base=[038h]
 090h 2 NDS:Reserved, DSi:NTR ROM Region End/80000h ;\usually both same
 092h 2 NDS:Reserved, DSi:TWL ROM Region Start/80000h ;/(zero for DSiware) |
| --- |

NDS carts (that don't use DSi features, but are manufactured after DSi release)
contain these extra entries:

```

| (012h)1 Unitcode (must be 00h for non-DSi carts)
 (020h)16 Changed ARM9/ARM7 areas (DSi-in-NDS-mode more restricted than NDS)
 088h 4 Unknown (B8h,4Bh,00h,00h) (similar as in DSi carts)
 1BFh 1 Flags (40h=RSA+TwoHMACs, 60h=RSA+ThreeHMACs)
 33Ch 14 HMAC for Icon/Title (only if [1BFh]=60h) ;as Whitelist Phase 3
 378h 14 HMAC for 160h-byte header and ARM9+ARM7 areas ;as Whitelist Phase 1
 38Ch 14 HMAC for OverlayARM9+NitroFAT (zero if no overlay) ;as Phase 2
 F80h 128 RSA signature |
| --- |

All other entries at 160h..FFFh are zerofilled in non-DSi carts.

**Changed ARM9/ARM7 areas (and new ARM9i/ARM7i areas)**

NDS allowed ARM9 or ARM7 to occupy about 3.8MB of main RAM. On DSi this is
restriced to 2.5MB for ARM9 and 0.25MB for ARM7, ie. 2.75MB in total, this
restriction applies even in NDS-mode, thus making DSi not fully backwards
compatible with NDS games (officially licensed NDS titles hopefully don't
conflict with that new restriction). In DSi mode, one can additionally load
2.5MB for ARM9i and 1MB for ARM7i, ie. 6.25MB in total for all four areas.

The DSi loading is a bit complicated, the areas are first loaded to DEDICATED
areas:

```

| ARM9 2004000h..227FFFFh (siz=27C000h) (for NDS mode: 2000000h and up)
 ARM7 2380000h..23BFFFFh (siz=40000h)
 ARM9i 2400000h..267FFFFh (siz=280000h)
 ARM7i 2E80000h..2F87FFFh (siz=108000h) |
| --- |

If the cart header does match with those DEDICATED areas then the data is left
in place, otherwise it gets relocated to GENERAL areas (shortly before jumping
to the entrypoint). The GENERAL areas are allowed to be:

```

| Main 2000000h..2FFC000h (excluding bootstrap at 23FEE00h..23FF000h)
 WRAM 3000000h..380F000h (excluding bootstrap at 3FFF600h..3FFF800h) |
| --- |

The GENERAL areas may not overlap with another DEDICATED area (eg. ARM7 cannot
be relocated to the ARM9+ARM9i+ARM7i areas). Concerning the 16K at
2000000h..2003FFFh: ARM7+ARM9i+ARM7i aren't allow to use that, but ARM9 seems
to be relocatable to that address (usually that shouldn't be done as it would
destroy some DSi system variables).

ARM9i+ARM7i areas can have "Size" and "ROM offset" set to zero (but still need
to have a valid nonzero "RAM Load address").

ARM9/ARM7 entrypoints MUST be within ARM9/ARM7 area respectively (NDS allowed
entrypoints being anywhere).

**New DSi Header Entries**

```

| 180h 20 Global MBK1..MBK5 Settings, WRAM Slots
 194h 12 Local MBK6..MBK8 Settings, WRAM Areas for ARM9
 1A0h 12 Local MBK6..MBK8 Settings, WRAM Areas for ARM7
 1ACh 3 Global MBK9 Setting, WRAM Slot Write Protect
 1AFh 1 Global WRAMCNT Setting (usually 03h) (FCh/00h in SysMenu/Settings)
 1B0h 4 Region flags (bit0=JPN, bit1=USA, bit2=EUR, bit3=AUS, bit4=CHN,
 bit5=KOR, bit6-31=Reserved) (FFFFFFFFh=Region Free)
 1B4h 4 Access control (AES Key Select)
 bit0 Common Client Key ;want 380F000h=3FFC600h+00h "common key"
 bit1 AES Slot B ;380F010h=3FFC400h+180h and KEY1=unchanged
 bit2 AES Slot C ;380F020h=3FFC400h+190h and KEY2.Y=3FFC400h+1A0h
 bit3 SD Card ;want Device I
 bit4 NAND Access ;want Device A-H and KEY3=intact
 bit5 Game Card Power On ;tested with bit8
 bit6 Shared2 File ;used... but WHAT for?
 bit7 Sign JPEG For Launcher (AES Slot B);select 1 of 2 jpeg keys?
 bit8 Game Card NTR Mode ;tested with bit5
 bit9 SSL Client Cert (AES Slot A) ;KEY0=3FFC600h+30h (twl-*.der)
 bit10 Sign JPEG For User (AES Slot B) ;\
 bit11 Photo Read Access ; seems to be unused
 bit12 Photo Write Access ; (and, usually ZERO,
 bit13 SD Card Read Access ; even if the stuff is
 bit14 SD Card Write Access ; accessed)
 bit15 Game Card Save Read Access ; (bit11 set in flipnote)
 bit16 Game Card Save Write Access ;/
 bit31 Debugger Common Client Key ;want 380F000h=3FFC600h+10h
 1B8h 4 ARM7 SCFG_EXT7 setting (bit0,1,2,10,18,31)
 1BCh 3 Reserved/flags? (zerofilled)
 1BFh 1 Flags (usually 01h) (DSiware Browser: 0Bh)
 bit0: TSC Touchscreen/Sound Controller Mode (0=NDS, 1=DSi)
 bit1: Require EULA Agreement (see hdr[20Eh] for version)
 bit2: Custom Icon (0=No/Normal, 1=Use banner.sav)
 bit3: Show Nintendo Wi-Fi Connection icon in Launcher
 bit4: Show DS Wireless icon in Launcher
 bit5: NDS cart with icon SHA1 (DSi firmware v1.4 and up)
 bit6: NDS cart with header RSA (DSi firmware v1.0 and up)
 bit7: Developer App
 1C0h 4 ARM9i ROM Offset (usually XX03000h, XX=1MB-boundary after NDS area)
 1C4h 4 Reserved (zero)
 1C8h 4 ARM9i RAM Load address
 1CCh 4 ARM9i Size
 1D0h 4 ARM7i ROM Offset
 1D4h 4 SD/MMC Device List ARM7 RAM Addr; 400h-byte initialized by firmware
 1D8h 4 ARM7i RAM Load address
 1DCh 4 ARM7i Size
 1E0h 4 Digest NTR region offset (usually same as ARM9 rom offs, 0004000h)
 1E4h 4 Digest NTR region length
 1E8h 4 Digest TWL region offset (usually same as ARM9i rom offs, XX03000h)
 1ECh 4 Digest TWL region length
 1F0h 4 Digest Sector Hashtable offset ;\SHA1-HMAC's on all sectors
 1F4h 4 Digest Sector Hashtable length ;/in above NTR+TWL regions
 1F8h 4 Digest Block Hashtable offset ;\SHA1-HMAC's on each N entries
 1FCh 4 Digest Block Hashtable length ;/in above Sector Hashtable
 200h 4 Digest Sector size (eg. 400h bytes per sector)
 204h 4 Digest Block sectorcount (eg. 20h sectors per block)
 208h 4 Icon/Title size (usually 23C0h for DSi) (older 840h-byte works too)
 20Ch 1 SD/MMC size of "shared2\0000" file in 32Kbyte units? (dsi sound)
 20Dh 1 SD/MMC size of "shared2\0001" file in 32Kbyte units?
 ;or are shared2 sizes rather counted in 16Kbyte cluster units?
 20Eh 1 EULA Version (01h) ? ;used when hdr[1BFh].bit1=1 !
 20Fh 1 Use Ratings (00h) ? !
 210h 4 Total Used ROM size, INCLUDING DSi area (optional, can be 0)
 214h 1 SD/MMC size of "shared2\0002" file in 32Kbyte units?
 215h 1 SD/MMC size of "shared2\0003" file in 32Kbyte units?
 216h 1 SD/MMC size of "shared2\0004" file in 32Kbyte units?
 217h 1 SD/MMC size of "shared2\0005" file in 32Kbyte units?
 218h 4 ARM9i Parameters Table Offset (84 D0 04 00) ??? ;base=[028h]
 21Ch 4 ARM7i Parameters Table Offset (2C 05 00 00) ??? ;base=[038h]
 220h 4 Modcrypt area 1 offset ;usually same as ARM9i rom offs (XX03000h)
 224h 4 Modcrypt area 1 size ;usually min(4000h,ARM9iSize+Fh AND not Fh)
 228h 4 Modcrypt area 2 offset (0=None)
 22Ch 4 Modcrypt area 2 size (0=None)
 230h 4 Title ID, Emagcode (aka Gamecode spelled backwards)
 234h 1 Title ID, Filetype (00h=Cartridge, 04h=DSiware, 05h=System Fun
 Tools, [0Fh=Non-executable datafile without cart header],
 15h=System Base Tools, 17h=System Menu)
 235h 1 Title ID, Zero (00h=Normal)
 236h 1 Title ID, Three (03h=DSi) (as opposed to Wii or 3DS)
 237h 1 Title ID, Zero (00h=Normal)
 238h 4 SD/MMC (DSiware) "public.sav" filesize in bytes (0=none)
 23Ch 4 SD/MMC (DSiware) "private.sav" filesize in bytes (0=none)
 240h 176 Reserved (zero-filled) |
| --- |

**Parental Control Age Ratings (set all entries to 80h to allow any age)**

```

| 2F0h 10h Parental Control Age Ratings (for different countries/areas)
 Bit7: Rating exists for local country/area
 Bit6: Game is prohibited in local country/area?
 Bit5: Unused
 Bit4-0: Age rating for local country/area (years)
 2F0h 1 CERO (Japan) (0=None/A, 12=B, 15=C, 17=D, 18=Z)
 2F1h 1 ESRB (US/Canada) (0=None, 3=EC, 6=E, 10=E10+, 13=T, 17=M)
 2F2h 1 Reserved (0=None)
 2F3h 1 USK (Germany) (0=None, 6=6+, 12=12+, 16=16+, 18=18+)
 2F4h 1 PEGI (Pan-Europe) (0=None, 3=3+, 7=7+, 12=12+, 16=16+, 18=18+)
 2F5h 1 Reserved (0=None)
 2F6h 1 PEGI (Portugal) (0=None, 4=4+, 6=6+, 12=12+, 16=16+, 18=18+)
 2F7h 1 PEGI and BBFC (UK) (0=None, 3, 4=4+/U, 7, 8=8+/PG, 12, 15, 16, 18)
 2F8h 1 AGCB (Australia) (0=None/G, 7=PG, 14=M, 15=MA15+, plus 18=R18+?)
 2F9h 1 GRB (South Korea) (0=None, 12=12+, 15=15+, 18=18+) (aka "BG"?)
 2FAh 6 Reserved (6x) (0=None)
 ? ? OFLC (NZ) (unknown, exists in DSi System Settings rom:layout\cmn)
 N/A? - DEJUS (Brazil) (L, 10, 12, 14, 16, 18)
 N/A? - GSRMR (Taiwan) (formerly CSRR) (0,6,12,18) (and GSRMR: 15)
 N/A? - PEGI (Finland) (discontinued 2007, shortly before DSi launch)
 bit0-4 Rating (0..18)
 bit6 Pending
 bit7 Enabled |
| --- |

**SHA1-HMAC's and RSA-SHA1**

```

| 300h 20 SHA1-HMAC hash ARM9 (with encrypted secure area) ;[020h,02Ch]
 314h 20 SHA1-HMAC hash ARM7 ;[030h,03Ch]
 328h 20 SHA1-HMAC hash Digest master ;[1F8h,1FCh]
 33Ch 20 SHA1-HMAC hash Icon/Title (also in newer NDS titles) ;[068h,208h]
 350h 20 SHA1-HMAC hash ARM9i (decrypted) ;[1C0h,1CCh]
 364h 20 SHA1-HMAC hash ARM7i (decrypted) ;[1D0h,1DCh]
 378h 20 Reserved (zero-filled) (but used for non-whitelisted NDS titles)
 38Ch 20 Reserved (zero-filled) (but used for non-whitelisted NDS titles)
 3A0h 20 SHA1-HMAC hash ARM9 (without 16Kbyte secure area) ;[020h,02Ch]
 3B4h 2636 Reserved (zero-filled)
 E00h 180h Reserved and unchecked region, always zero. Used for passing
 arguments in debug environment.
 F80h 80h RSA-SHA1 signature across header entries [000h..DFFh] |
| --- |

**Reserved Area**

```

| 1000h..3FFFh Non-Load area in ROMs... but contains sth in DSiWare files!?! |
| --- |

**DSiware/System Utilities**

Files saved on SD card or internal eMMC memory are having the same header as
ROM carts, with some differences:

```

| No need for NDS backwards compatibility (since DSiware is DSi only)
 Entry 3A0h can be zero-filled (in LAUNCHER) |
| --- |

DSiware files are usually marked as [012h]=03h=DSi (exceptions are the DS
Download Play and PictoChat utilities, which are marked [012h]=00h=NDS, since
they are actually running in NDS mode (yet having at least two DSi-specific
features: 1st is allowing to reboot into DSi menu via Powerman SPI, 2nd is
requiring enabled wifi channels in 2FFFCFAh)).

**SHA1-HMAC**

The SHA1-HMAC's in cart header and Digest tables are SHA1 checksums with a
40h-byte HMAC key (values 21h, 06h, C0h, DEh, BAh, ..., 24h), the key is
contained in the Launcher, and it's also stored in most DSi cartridges
(probably used for verifying Digest values when loading additional data after
booting). The key can be used for verifying checksums, but (due to the RSA
signature) not for changing them. See BIOS chapter for SHA1/HMAC pseudo code.

**RSA-SHA1**

The RSA-SHA1 value is a normal SHA1 (not SHA1-HMAC) across header entries
[000h..DFFh], the 20-byte value is padded to 127-byte size (01h, 105xFFh, 00h,
followed by the 20 SHA1 bytes). It can be decrypted (via SWI 22h) using the
80h-byte RSA public keys located in ARM9BIOS (note that there are at least four
different RSA keys, one is used for games, and others for system files), and
can be then verfied against the SHA1 checksum (computed via SWI 27h).

BIOS RSA Functions (DSi only)

The private key needed for encryption is unknown, which is unfortunately
preventing to boot unlicensed (homebrew) software.

**Modcrypt (AES-CTR) (optional, carthdr[220h..22Fh] can be all zero)**

Modcrypt is a new additional way of encrypting parts of the NDS ROM executable
binary modules using AES CTR. It is mostly being used to encrypt the ARM9i and
ARM7i binaries. DSi cartridges are usually having only the ARM9i binary
encrypted (as area 1), while NAND based applications have both the ARM9i and
ARM7i binaries encrypted (as area 1 and 2).

The initial AES Counter value (IV) is:

```

| Modcrypt Area 1 IV[0..F]: First 16 bytes of the ARM9 SHA1-HMAC [300h..30Fh]
 Modcrypt Area 2 IV[0..F]: First 16 bytes of the ARM7 SHA1-HMAC [314h..323h] |
| --- |

The AES key depends of flags in the cartridge header:

```

| IF header[01Ch].Bit1=0
 None (modcrypt disabled)
 ELSEIF header[01Ch].Bit2 OR header[1BFh].Bit7 THEN (probably for prototypes)
 Debug KEY[0..F]: First 16 bytes of the header [000h..00Fh]
 ELSE (commonly used for retail software)
 Retail KEY_X[0..7]: Fixed 8-byte ASCII string ("Nintendo")
 Retail KEY_X[8..B]: The 4-byte gamecode, forwards [00Ch..00Fh]
 Retail KEY_X[C..F]: The 4-byte gamecode, backwards [00Fh..00Ch]
 Retail KEY_Y[0..F]: First 16 bytes of the ARM9i SHA1-HMAC [350h..35Fh] |
| --- |

Above does describe how modcrypted areas should look like. However, there are
several circumstances where the firmware can't actually decrypt that areas...

Theoretically, the modcrypt areas can span over any of the ARM9i/ARM7i and
ARM9/ARM7 areas (in practice, cartridges should never use modcrypt for the
ARM9/ARM7 areas because NDS consoles would leave them undecrypted; that
restriction doesn't apply to DSiware though).

Theoretically, a large modcrypt area could contain several data areas, but the
launcher does decrypt only the first matching data area (areas are processed in
order ARM9, ARM7, ARM9i, ARM7i). Moreover, matching data areas must be INSIDE
of the modcrypt area (ie. the data area must be same size, or smaller than the
modrypt area) (whereas, the launcher is weirdly rounding-up the data size to a
multiple of 20h-bytes when checking that matches) (as a side-effect, each data
area can be decrypted starting with "IV+0", rather than needing
"IV+(offset_within_modcrypt_area/10h").

Theoretically, AES decryption could be done byte-wise, however, the AES
hardware is doing it in 10h-byte chunks, unknown if the launcher does require
10h-byte alignments; however, in fact, the launcher seems to be rounding the
modcrypt END address to 20h-byte boundary - so it's safest to stick with
20h-byte aligned start+size values for modcrypt areas, as well as for
corresponding data areas.

Note: The size of the modcrypt areas can exceed the AES hardware's size limit
of max FFFF0h bytes (eg. in DSi Sound utility), in such cases decryption must
be split to smaller AES chunks; with manually increased IV.

Modcrypt area 1 and 2 can overlap each other (whereas, it doesn't matter which
of them is processed first, since the encryption/decryption is done by XORing).

Note: The ARM9 code for modcrypt/data areas is at 26A6BB8h in Launcher v1.4E.

**Digests (optional, carthdr[1E0h..207h] can be all zero)**

The NDS format has been extended with a hash tree to verify the entire contents
of an NDS ROM. The NDS ROM is divided into sectors, and each sector will be
hashed and have its hash stored in the digest sector hashtable. The size of a
sector is defined in the header aswell. Furthermore, the sector hashtable is
partitioned and hashed again to form block hashes. This block hashtable is
hashed again into a single hash called the digest master hash. These hashtables
can be used to verify that the sectors of a NDS ROM have not been tampered
with, since the integrity of a sector hash can be verified by a block hash,
which in turn can be verified by the master hash. And this hash is part of the
header, which is signed with RSA.

The sector hashtable reaches over the NTR and TWL regions, respectively.

**Cartridge Protocol**

The DSi cartridge protocol is same as on NDS; with one new command (3Dh) for
unlocking DSi specific memory regions. For details,

DS Cartridge Protocol