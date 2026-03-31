# Dsvideo

> Source: https://problemkaputt.de/gbatek.htm
> Section: Dsvideo

DS Video 
| |
| --- |

 The NDS has two 2D Video Engines, each basically the same as in GBA, see
 GBA LCD Video Controller
 
 **NDS Specific 2D Video Features**
 DS Video Stuff
 DS Video BG Modes / Control
 DS Video OBJs
 DS Video Extended Palettes
 DS Video Capture and Main Memory Display Mode
 DS Video Display System Block Diagram
 
 **NDS/DSi File Formats for 2D video**
 DS Files - 2D Video
 
 For Display Power Control (and Display Swap), and VRAM Allocation, see
 DS Power Control
 DS Power Management Device
 DS Memory Control - VRAM
 
 
 
| DS Video Stuff |
| --- |

 **DS Display Dimensions / Timings**
 Dot clock = 5.585664 MHz (=33.513982 MHz / 6)
 H-Timing: 256 dots visible, 99 dots blanking, 355 dots total (15.7343KHz)
 V-Timing: 192 lines visible, 71 lines blanking, 263 lines total (59.8261 Hz)
 The V-Blank cycle for the 3D Engine consists of the 23 lines, 191..213.
 Screen size 62.5mm x 47.0mm (each) (256x192 pixels)
 Vertical space between screens 22mm (equivalent to 90 pixels)
 
 **400006Ch - NDS9 - MASTER_BRIGHT - 16bit - Master Brightness Up/Down**
 
```

| 0-4 Factor used for 6bit R,G,B Intensities (0-16, values >16 same as 16)
 Brightness up: New = Old + (63-Old) * Factor/16
 Brightness down: New = Old - Old * Factor/16
 5-13 Not used
 14-15 Mode (0=Disable, 1=Up, 2=Down, 3=Reserved)
 16-31 Not used |
| --- |

**DISPSTAT/VCOUNT**

The LY and LYC values are in range 0..262, so LY/LYC values have been expanded
to 9bit values: LY = VCOUNT Bit 0..8, and LYC=DISPSTAT Bit8..15,7.

VCOUNT register is write-able, allowing to synchronize linked DS consoles.

For proper synchronization:

```

| write new LY values only in range of 202..212
 write only while old LY values are in range of 202..212 |
| --- |

DISPSTAT/VCOUNT supported by NDS9 (Engine A Ports, without separate Engine B
Ports), and by NDS7 (allowing to synchronize NDS7 with display timings).

Similar as on GBA, the VBlank flag isn't set in the last line (ie. only in
lines 192..261, but not in line 262).

Although the drawing time is only 1536 cycles (256*6), the NDS9 H-Blank flag is
"0" for a total of 1606 cycles (and, for whatever reason, a bit longer, 1613
cycles in total, on NDS7).

**VRAM Waitstates**

The display controller performs VRAM-reads once every 6 clock cycles, a 1 cycle
waitstate is generated if the CPU simultaneously accesses VRAM. With capture
enabled, additionally VRAM-writes take place once every 6 cycles, so the total
VRAM-read/write access rate is then once every 3 cycles.

**DS Window Glitches**

The DS counts scanlines in range 0..262 (0..106h), of which only the lower 8bit
are compared with the WIN0V/WIN1V register settings. Respectively, Y1
coordinates 00h..06h will be triggered in scanlines 100h-106h by mistake. That
means, the window gets activated within VBlank period, and will be active in
scanline 0 and up (that is no problem with Y1=0, but Y1=1..6 will appear as if
if Y1 would be 0). Workaround would be to disable the Window during VBlank, or
to change Y1 during VBlank (to a value that does not occur during VBlank
period, ie. 7..191).

Also, there's a problem to fit the 256 pixel horizontal screen resolution into
8bit values: X1=00h is treated as 0 (left-most), X2=00h is treated as 100h
(right-most). However, the window is not displayed if X1=X2=00h; the window
width can be max 255 pixels.

**2D Engines**

Includes two 2D Engines, called A and B. Both engines are accessed by the ARM9
processor, each using different memory and register addresses:

```

| Region______Engine A______________Engine B___________
 I/O Ports 4000000h 4001000h
 Palette 5000000h (1K) 5000400h (1K)
 BG VRAM 6000000h (max 512K) 6200000h (max 128K)
 OBJ VRAM 6400000h (max 256K) 6600000h (max 128K)
 OAM 7000000h (1K) 7000400h (1K) |
| --- |

Engine A additionally supports 3D and large-screen 256-color Bitmaps, plus
main-memory-display and vram-display modes, plus capture unit.

**Viewing Angles**

The LCD screens are best viewed at viewing angles of 90 degrees. Colors may
appear distorted, and may even become invisible at other viewing angles.

When the console is handheld, both screens can be turned into preferred
direction. When the console is settled on a table, only the upper screen can be
turned, but the lower screen is stuck into horizontal position - which results
in rather bad visibility (unless the user moves his/her head directly above of
it).

**4000070h - NDS9 - TVOUTCNT - Unknown (W)**

```

| Bit0-3 "COMMAND" (?)
 Bit4-7 "COMMAND2" (?)
 Bit8-11 "COMMAND3" (?) |
| --- |

This register has been mentioned in an early I/O map from Nintendo, as far as I
know, the register isn't used by any games/firmware/bios, not sure if it does
really exist on release-version, or if it's been prototype stuff...?

**DS-Lite Screens**

The screens in the DS-Lite seem to allow a wider range of vertical angles.

The bad news is that the colors of the DS-Lite are (no surprise) not backwards
compatible with older NDS and GBA displays. The good news is that Nintendo has
finally reached near-CRT-quality (without blurred colors), so one could hope
that they won't show up with more displays with other colors in future.

Don't know if there's an official/recommended way to detect DS-Lite displays
(?) possible methods would be whatever values in Firmware header, or by
functionality of Power Managment device, or (not too LCD-related) by Wifi Chip
ID.

| DS Video BG Modes / Control |
| --- |

**4000000h - NDS9 - DISPCNT**

```

| Bit Engine Expl.
 0-2 A+B BG Mode
 3 A BG0 2D/3D Selection (instead CGB Mode) (0=2D, 1=3D)
 4 A+B Tile OBJ Mapping (0=2D; max 32KB, 1=1D; max 32KB..256KB)
 5 A+B Bitmap OBJ 2D-Dimension (0=128x512 dots, 1=256x256 dots)
 6 A+B Bitmap OBJ Mapping (0=2D; max 128KB, 1=1D; max 128KB..256KB)
 7-15 A+B Same as GBA
 16-17 A+B Display Mode (Engine A: 0..3, Engine B: 0..1, GBA: Green Swap)
 18-19 A VRAM block (0..3=VRAM A..D) (For Capture & above Display Mode=2)
 20-21 A+B Tile OBJ 1D-Boundary (see Bit4)
 22 A Bitmap OBJ 1D-Boundary (see Bit5-6)
 23 A+B OBJ Processing during H-Blank (was located in Bit5 on GBA)
 24-26 A Character Base (in 64K steps) (merged with 16K step in BGxCNT)
 27-29 A Screen Base (in 64K steps) (merged with 2K step in BGxCNT)
 30 A+B BG Extended Palettes (0=Disable, 1=Enable)
 31 A+B OBJ Extended Palettes (0=Disable, 1=Enable) |
| --- |

**BG Mode**

Engine A BG Mode (DISPCNT LSBs) (0-6, 7=Reserved)

```

| Mode BG0 BG1 BG2 BG3
 0 Text/3D Text Text Text
 1 Text/3D Text Text Affine
 2 Text/3D Text Affine Affine
 3 Text/3D Text Text Extended
 4 Text/3D Text Affine Extended
 5 Text/3D Text Extended Extended
 6 3D - Large - |
| --- |

Of which, the "Extended" modes are sub-selected by BGxCNT bits:

```

| BGxCNT.Bit7 BGxCNT.Bit2 Extended Affine Mode Selection
 0 CharBaseLsb rot/scal with 16bit bgmap entries (Text+Affine mixup)
 1 0 rot/scal 256 color bitmap
 1 1 rot/scal direct color bitmap |
| --- |

Engine B: Same as above, except that: Mode 6 is reserved (no Large screen
bitmap), and BG0 is always Text (no 3D support).

Affine = formerly Rot/Scal mode (with 8bit BG Map entries)

Large Screen Bitmap = rot/scal 256 color bitmap (using all 512K of 2D VRAM)

**Display Mode (DISPCNT.16-17):**

```

| 0 Display off (screen becomes white)
 1 Graphics Display (normal BG and OBJ layers)
 2 Engine A only: VRAM Display (Bitmap from block selected in DISPCNT.18-19)
 3 Engine A only: Main Memory Display (Bitmap DMA transfer from Main RAM) |
| --- |

Mode 2-3 display a raw direct color bitmap (15bit RGB values, the upper bit in
each halfword is unused), without any further BG,OBJ,3D layers, these modes are
completely bypassing the 2D/3D engines as well as any 2D effects, however the
Master Brightness effect can be applied to these modes. Mode 2 is particulary
useful to display captured 2D/3D images (in that case it can indirectly use the
2D/3D engine).

**BGxCNT**

character base extended from bit2-3 to bit2-5 (bit4-5 formerly unused)

```

| engine A screen base: BGxCNT.bits*2K + DISPCNT.bits*64K
 engine B screen base: BGxCNT.bits*2K + 0
 engine A char base: BGxCNT.bits*16K + DISPCNT.bits*64K
 engine B char base: BGxCNT.bits*16K + 0 |
| --- |

char base is used only in tile/map modes (not bitmap modes)

screen base is used in tile/map modes,

screen base used in bitmap modes as BGxCNT.bits*16K, without DISPCNT.bits*64K

screen base however NOT used at all for Large screen bitmap mode

```

| bgcnt size text rotscal bitmap large bmp
 0 256x256 128x128 128x128 512x1024
 1 512x256 256x256 256x256 1024x512
 2 256x512 512x512 512x256 -
 3 512x512 1024x1024 512x512 - |
| --- |

bitmaps that require more than 128K VRAM are supported on engine A only.

For BGxCNT.Bit7 and BGxCNT.Bit2 in Extended Affine modes, see above BG Mode
description (extended affine doesn't include 16-color modes, so color depth bit
can be used for mode selection. Also, bitmap modes do not use charbase, so
charbase.0 can be used for mode selection as well).

```

| for BG0CNT, BG1CNT only: bit13 selects extended palette slot
 (BG0: 0=Slot0, 1=Slot2, BG1: 0=Slot1, 1=Slot3) |
| --- |

Direct Color Bitmap BG, and Direct Color Bitmap OBJ

BG/OBJ Supports 32K colors (15bit RGB value) - so far same as GBAs BG.

However, the upper bit (Bit15) is used as Alpha flag. That is,
Alpha=0=Transparent, Alpha=1=Normal (ie. on the NDS, Direct Color values
0..7FFFh are NOT displayed).

Unlike GBA bitmap modes, NDS bitmap modes are supporting the Area Overflow bit
(BG2CNT and BG3CNT, Bit 13).

| DS Video OBJs |
| --- |

**DS OBJ Priority**

The GBA has been assigning OBJ priority in respect to the 7bit OAM entry
number, regardless of the OBJs 2bit BG-priority attribute (which allowed to
specify invalid priority orders). That problem has been fixed in DS mode by
combining the above two values into a 9bit priority value.

**OBJ Tile Mapping (DISPCNT.4,20-21):**

```

| Bit4 Bit20-21 Dimension Boundary Total ;Notes
 0 x 2D 32 32K ;Same as GBA 2D Mapping
 1 0 1D 32 32K ;Same as GBA 1D Mapping
 1 1 1D 64 64K
 1 2 1D 128 128K
 1 3 1D 256 256K ;Engine B: 128K max |
| --- |

TileVramAddress = TileNumber * BoundaryValue

Even if the boundary gets changed, OBJs are kept composed of 8x8 tiles.

**Bitmap OBJ Mapping (DISPCNT.6,5,22):**

Bitmap OBJs are 15bit Direct Color data, plus 1bit Alpha flag (in bit15).

```

| Bit6 Bit5 Bit22 Dimension Boundary Total ;Notes
 0 0 x 2D/128 dots 8x8 dots 128K ;Source Bitmap width 128 dots
 0 1 x 2D/256 dots 8x8 dots 128K ;Source Bitmap width 256 dots
 1 0 0 1D 128 bytes 128K ;Source Width = Target Width
 1 0 1 1D 256 bytes 256K ;Engine A only
 1 1 x Reserved |
| --- |

In 1D mapping mode, the Tile Number is simply multiplied by the boundary value.

```

| 1D_BitmapVramAddress = TileNumber(0..3FFh) * BoundaryValue(128..256)
 2D_BitmapVramAddress = (TileNo AND MaskX)*10h + (TileNo AND NOT MaskX)*80h |
| --- |

In 2D mode, the Tile Number is split into X and Y indices, the X index is
located in the LSBs (ie. MaskX=0Fh, or MaskX=1Fh, depending on DISPCNT.5).

**OBJ Attribute 0 and 2**

Setting the OBJ Mode bits (Attr 0, Bit10-11) to a value of 3 has been
prohibited in GBA, however, in NDS it selects the new Bitmap OBJ mode; in that
mode, the Color depth bit (Attr 0, Bit13) should be set to zero; also in that
mode, the color bits (Attr 2, Bit 12-15) are used as Alpha-OAM value (instead
of as palette setting).

**OBJ Vertical Wrap**

On the GBA, a large OBJ (with 64pix height, scaled into double-size region of
128pix height) located near the bottom of the screen has been wrapped to the
top of the screen (and was NOT displayed at the bottom of the screen).

This problem has been "corrected" in the NDS (except in GBA mode), that is, on
the NDS, the OBJ appears BOTH at the top and bottom of the screen. That isn't
necessarily better - the advantage is that one can manually enable/disable the
OBJ in the desired screen-half on IRQ level; that'd be required only if the
wrapped portion is non-transparent.

| DS Video Extended Palettes |
| --- |

**Extended Palettes**

When allocating extended palettes, the allocated memory is not mapped to the
CPU bus, so the CPU can access extended palette only when temporarily
de-allocating it.

Color 0 of all standard/extended palettes is transparent, color 0 of BG
standard palette 0 is used as backdrop. extended palette memory must be
allocated to VRAM.

BG Extended Palette enabled in DISPCNT Bit 30, when enabled,

```

| standard palette --> 16-color tiles (with 16bit bgmap entries) (text)
 256-color tiles (with 8bit bgmap entries) (rot/scal)
 256-color bitmaps
 backdrop-color (color 0)
 extended palette --> 256-color tiles (with 16bit bgmap entries)(text,rot/scal) |
| --- |

Allocated VRAM is split into 4 slots of 8K each (32K used in total), normally
BG0..3 are using Slot 0..3, however BG0 and BG1 can be optionally changed to
BG0=Slot2, and BG1=Slot3 via BG0CNT and BG1CNT.

OBJ Extended Palette enabled in DISPCNT Bit 31, when enabled,

```

| 16 colors x 16 palettes --> standard palette memory (=256 colors)
 256 colors x 16 palettes --> extended palette memory (=4096 colors) |
| --- |

Extended OBJ palette memory must be allocated to VRAM F, G, or I (which are
16K) of which only the first 8K are used for extended palettes (=1000h 16bit
entries).

| DS Video Capture and Main Memory Display Mode |
| --- |

**4000064h - NDS9 - DISPCAPCNT - 32bit - Display Capture Control Register (R/W)**

Capture is supported for Display Engine A only.

```

| 0-4 EVA (0..16 = Blending Factor for Source A)
 5-7 Not used
 8-12 EVB (0..16 = Blending Factor for Source B)
 13-15 Not used
 16-17 VRAM Write Block (0..3 = VRAM A..D) (VRAM must be allocated to LCDC)
 18-19 VRAM Write Offset (0=00000h, 0=08000h, 0=10000h, 0=18000h)
 20-21 Capture Size (0=128x128, 1=256x64, 2=256x128, 3=256x192 dots)
 22-23 Not used
 24 Source A (0=Graphics Screen BG+3D+OBJ, 1=3D Screen)
 25 Source B (0=VRAM, 1=Main Memory Display FIFO)
 26-27 VRAM Read Offset (0=00000h, 0=08000h, 0=10000h, 0=18000h)
 28 Not used
 29-30 Capture Source (0=Source A, 1=Source B, 2/3=Sources A+B blended)
 31 Capture Enable (0=Disable/Ready, 1=Enable/Busy) |
| --- |

Notes:

VRAM Read Block (VRAM A..D) is selected in DISPCNT Bits 18-19.

VRAM Read Block can be (or must be ?) allocated to LCDC (MST=0).

VRAM Read Offset is ignored (zero) in VRAM Display Mode (DISPCNT.16-17).

VRAM Read/Write Offsets wrap to 00000h when exceeding 1FFFFh (max 128K).

Capture Sizes less than 256x192 capture the upper-left portion of the screen.

Blending factors EVA and EVB are used only if "Source A+B blended" selected.

After setting the Capture Enable bit, capture starts at next line 0, and the
capture enable/busy bit is then automatically cleared (in line 192, regardless
of the capture size).

Capture data is 15bit color depth (even when capturing 18bit 3D-images).

Capture A: Dest_Intensity = SrcA_Intensitity ; Dest_Alpha=SrcA_Alpha.

Capture B: Dest_Intensity = SrcB_Intensitity ; Dest_Alpha=SrcB_Alpha.

Capture A+B (blending):

```

| Dest_Intensity = ( (SrcA_Intensitity * SrcA_Alpha * EVA)
 + (SrcB_Intensitity * SrcB_Alpha * EVB) ) / 16
 Dest_Alpha = (SrcA_Alpha AND (EVA>0)) OR (SrcB_Alpha AND EVB>0)) |
| --- |

Capture provides a couple of interesting effects.

For example, 3D Engine output can be captured via source A (to LCDC-allocated
VRAM), in the next frame, either Graphics Engine A or B can display the
captured 3D image in VRAM image as BG2, BG3, or OBJ (from BG/OBJ-allocated
VRAM); this method requires to switch between LCDC- and BG/OBJ-allocation.

Another example would be to capture Engine A output, the captured image can be
displayed (via VRAM Display mode) in the following frames, simultaneously the
new Engine A output can be captured, blended with the old captured image; in
that mode moved objects will leave traces on the screen; this method works with
a single LCDC-allocated VRAM block.

DS Video Display System Block Diagram

**4000068h - NDS9 - DISP_MMEM_FIFO - 32bit - Main Memory Display FIFO (R?/W)**

Intended to send 256x192 pixel 32K color bitmaps by DMA directly

```

| - to Screen A (set DISPCNT to Main Memory Display mode), or
 - to Display Capture unit (set DISPCAPCNT to Main Memory Source). |
| --- |

The FIFO can receive 4 words (8 pixels) at a time, each pixel is a 15bit RGB
value (the upper bit, bit15, is unused).

Set DMA to Main Memory mode, 32bit transfer width, word count set to 4,
destination address to DISP_MMEM_FIFO, source address must be in Main Memory.

Transfer starts at next frame.

Main Memory Display/Capture is supported for Display Engine A only.

| DS Video Display System Block Diagram |
| --- |

```

| _____________ __________
 VRAM A -->| 2D Graphics |--------OBJ->| |
 VRAM B -->| Engine A |--------BG3->| Layering |
 VRAM C -->| |--------BG2->| and |
 VRAM D -->| |--------BG1->| Special |
 VRAM E -->| | ___ | Effects |
 VRAM F -->| |->|SEL| | | ______
 VRAM G -->| - - - - - - | |BG0|-BG0->| |----o--->| |
 | 3D Graphics |->|___| |__________| | |Select|
 | Engine | | |Video |
 |_____________|--------3D----------------. | |Input |
 _______ _______ ___ | | | |
 | | | |<-----------|SEL|<-' | |and |-->
 | | | | _____ |A | | | |
 VRAM A <--|Select | |Select | | |<-|___|<----' |Master|
 VRAM B <--|Capture|<---|Capture|<--|Blend| ___ |Bright|
 VRAM C <--|Dest. | |Source | |_____|<-|SEL|<----. |A |
 VRAM D <--| | | | |B | | | |
 |_______| |_______|<-----------|___|<-. | | |
 _______ | | | |
 VRAM A -->|Select | | | | |
 VRAM B -->|Display|--------------------------------o------>| |
 VRAM C -->|VRAM | | | |
 VRAM D -->|_______| _____________ | | |
 |Main Memory | | | |
 Main ------DMA---->|Display FIFO |------------------o--->|______|
 Memory |_____________|
 _____________ __________ ______
 VRAM C -->| 2D Graphics |--------OBJ->| Layering | | |
 VRAM D -->| Engine B |--------BG3->| and | |Master|
 VRAM H -->| |--------BG2->| Special |-------->|Bright|-->
 VRAM I -->| |--------BG1->| Effects | |B |
 |_____________|--------BG0->|__________| |______| |
| --- |

| DS Files - 2D Video |
| --- |

eg. used in DSi Launcher "rom:\layout\cmn\launcher_d.szs\.."

[http://www.romhacking.net/documents/[469]nds_formats.htm](http://www.romhacking.net/documents/[469]nds_formats.htm)

```

| ____________________________ Nitro Color Palette _____________________________ |
| --- |

**Header**

```

| 000h 4 Chunk ID "RLCN" (aka NCLR backwards, Nitro Color Resource)
 004h 2 Byte Order (FEFFh)
 006h 2 Version (0100h)
 008h 4 Total Filesize
 00Ch 2 Offset to "TTLP" Chunk, aka Size of "RLCN" Chunk (0010h)
 00Eh 2 Total number of following Chunks (1=TTLP) (or 2=TTLP+PMCP ?) |
| --- |

**TTLP Chunk**

```

| 000h 4 Chunk ID "TTLP" (aka PLTT backwards, Palette data)
 004h 4 Chunk Size (eg. 0218h)
 008h 4 Reportedly Color Depth (ie. "tile usage info") (3=4bpp, 4=8bpp)
 00Ch 4 Zero
 010h 4 Palette Data Size in bytes (eg. 200h) (or 200h-N? no, blah!)
 014h 4 Offset from TTLP+8 to Palette Data? (always 10h)
 018h N*2 Palete Data (16bit colors, 0000h..7FFFh) |
| --- |

Most DSi titles use full 200h-byte palettes (Paper Plane has a smaller one in
Graphics.NARC\Seq\pause.zcl). There seem to be no DSi titles with PMCP chunks.

**PMCP Chunk (if any) (reportedly exists, but not in DSi Launcher...?)**

```

| 000h 4 Chunk ID "PMCP" (aka PCMP backwards, Palette CMP?)
 004h 4 Chunk Size (reportedly always 12h ???)
 008h 2 Number of palettes in file (uh?)
 00Ah 2 Unused (BEEFh=Bullshit)
 00Ch 4 Offset from PMCP+8 to Palette IDs? (always 08h)
 DATA N*2 "Palette ID numbers for each palette (starting from 0)" |
| --- |

```

| ___________________________ Nitro Character Tiles ____________________________ |
| --- |

```

| eg. DSi Launcher "rom:\debug\DebugFont.NCGR" -- with SOPC chunk
 eg. DSi Launcher "rom:\layout\cmn\launcher_d.szs\.." -- without SOPC chunk |
| --- |

**Header**

```

| 000h 4 Chunk ID "RGCN" (aka NCGR backwards, Nitro Char Graphics Resource)
 004h 2 Byte Order (FEFFh)
 006h 2 Version (0101h) (unknown if 0100h does also exist?)
 008h 4 Total Filesize
 00Ch 2 Offset to "RAHC" Chunk, aka Size of "RGCN" Chunk (0010h)
 00Eh 2 Total number of following Chunks (1=RAHC, or 2=RAHC+SOPC) |
| --- |

**RAHC Chunk**

```

| 000h 4 Chunk ID "RAHC" (aka CHAR backwards)
 004h 4 Chunk Size (eg. 1420h)
 008h 2 Tile Data Size in Kilobytes ;\or both set to FFFFh
 00Ah 2 Unknown (always 20h) ;/(when size<>N*1024)
 00Ch 4 Color Depth (3=4bpp, 4=8bpp)
 010h 2 Zero ;or 10h (when SOPC not exists? kbyte size rounded up?)
 012h 2 Zero ;or 20h (when SOPC not exists?)
 014h 4 Zero
 018h 4 Tile Data Size in Bytes (eg. 1400h)
 01Ch 4 Offset from RAHC+8 to Tile Data? ;=always 18h
 020h ... Tile Data (eg. 20h-byte zerofilled for 4bpp SPC char?) |
| --- |

Nonzero [10h,12h] spotted in Paper Plane "rom:\Graphics.NARC\Plane\plane.zcg".

**SOPC Chunk (only present if Tile Data size is N*1024 bytes)**

```

| 000h 4 Chunk ID "SOPC" (aka CPOS backwards)
 004h 4 Chunk Size (10h)
 008h 4 Zero
 00Ch 2 Same as [00Ah] in RAHC chunk? (always 20h)
 00Eh 2 Same as [008h] in RAHC chunk? (size in kilobytes) |
| --- |

```

| __________________________ Unknown Character Tiles ___________________________ |
| --- |

Apart from above NCGR, there is reportedly another tile format:

```

| NCGR (Nitro Character Graphic Resource) - Graphical Tiles --> see above
 NBGR (Nitro Basic Graphic Resource) - Graphical Tiles --> what ??? |
| --- |

If it does really exist for real... the header ID might be "RGBN" (aka NBGR
backwards), and file extension might be NBGR? But even if so, it's unknown
if/when/where/why that NBGR format is used. If the "B" is for "Basic" then
might have less features than NCGR, or maybe it might be "B" for non-tiled
Bitmaps, or whatever?

```

| ___________________________ Nitro BG Maps Screens ____________________________ |
| --- |

**Header**

```

| 000h 4 Chunk ID "RCSN" (aka NSCR backwards, Nitro Screen Resource)
 004h 2 Byte Order (FEFFh)
 006h 2 Version (0100h)
 008h 4 Total Filesize
 00Ch 2 Offset to "NRCS" Chunk, aka Size of "RCSN" Chunk (0010h)
 00Eh 2 Total number of following Chunks (1=NRCS) |
| --- |

**NRCS Chunk**

```

| 000h 4 Chunk ID "NRCS" (aka SCRN backwards, Screen)
 004h 4 Chunk Size
 008h 4 Screen Width in pixels
 00Ah 2 Screen Height in pixels
 00Ch 4 Zero
 010h 4 Screen Data Size (width/8)*(height/8)*2
 014h N*2 Screen Data (16bit BG Map entries, palette+xyflip+tileno) |
| --- |

```

| ____________________________ Nitro OBJ Animations ____________________________ |
| --- |

**Header**

```

| 000h 4 Chunk ID "RNAN" (aka NANR backwards, Nitro Animation Resource)
 004h 2 Byte Order (FEFFh)
 006h 2 Version (0100h)
 008h 4 Total Filesize
 00Ch 2 Offset to "KNBA" Chunk, aka Size of "RNAN" Chunk (0010h)
 00Eh 2 Total number of following Chunks (1=KNBA, or 3=KNBA+LBAL+TXEU) |
| --- |

One chunk exists in DSi Launcher

Three chunks exist in DSi Flipnote "rom:ManualData.Eu\md2res_narc.blz\data\obj

**KNBA Chunk**

```

| 000h 4 Chunk ID "KNBA" (aka ABNK backwards, Animation Bank)
 004h 4 Chunk Size (always padded to 4-byte boundary if LABL chunk follows)
 008h 2 Number of 16-byte Animation Blocks ;implies NumLabels in LABL chunk
 00Ah 2 Number of 8-byte Frame Blocks
 00Ch 4 Offset from KNBA+8 to Animation Blocks ;=18h
 010h 4 Offset from KNBA+8 to Frame Blocks ;=[0Ch]+[08h]*10h
 014h 4 Offset from KNBA+8 to Frame Data ;=[10h]+[0Ah]*8
 018h 8 Zero
 DATA .. Animation Blocks (16-byte entries)
 00h 4 Number of Frames
 04h 2 Unknown (0)
 06h 2 Unknown Always (1) ;reportedly "always unknown"
 08h 4 Unknown (1..2)
 0Ch 4 Offset from FrameBlock+0 to First Frame
 DATA .. Frame Blocks (8-byte entries)
 00h 4 Offset from FrameData+0 to whatever? (always 4-byte aligned?)
 04h 2 Frame Width ;3Ch or 01..06h ;Time in 60Hz units? num meta's?
 06h 2 Unused (usually 0000h, or BEEFh=Bullshit)
 DATA .. Frame Data (2-byte entries)
 00h 2 Unknown 16bit values? (maybe CELL index or whatever??) (CCCCh=?) |
| --- |

**LBAL Chunk**

```

| 000h 4 Chunk ID "LBAL" (aka LABL backwards, Labels)
 004h 4 Chunk Size (not padded to 4-byte size, following TXEU is unaligned)
 008h 4*N Offsets from LabelArea+0 to Labels (for each Animation Block)
 ... .. Label Area (ASCII Strings, terminated by 00h) |
| --- |

The LabelArea starts at LBAL+8+NumLabels*4 (whereas, NumLabels is found in KNBA
chunk).

**TXEU Chunk**

Caution: Not 4-byte aligned (the preceeding LBAL chunk can have odd size).

```

| 000h 4 Chunk ID "TXEU" (aka UEXT backwards, Whatever Extension or so?)
 004h 4 Chunk Size (0Ch)
 008h 4 Unknown (usually 0) (reportedly 0 or 1) |
| --- |

```

| __________________________ Nitro OBJ Metatile Cells __________________________ |
| --- |

**Header**

```

| 000h 4 Chunk ID "RECN" (aka NCER backwards, Nitro Cell Resource)
 004h 2 Byte Order (FEFFh)
 006h 2 Version (0100h)
 008h 4 Total Filesize
 00Ch 2 Offset to "KBEC" Chunk, aka Size of "RECN" Chunk (0010h)
 00Eh 2 Total number of following Chunks (1=KBEC, or 3=KBEC+LBAL+TXEU) |
| --- |

**KBEC Chunk**

```

| 000h 4 Chunk ID "KBEC" (aka CEBK backwards, Cell Bank)
 004h 4 Chunk Size (always padded to 4-byte boundary if LABL chunk follows)
 008h 2 Number of Metatiles
 00Ah 2 Metatiles Entry Size (0=Normal 8 bytes, 1=Extended 16 bytes)
 (DSi Launcher ..layout\cmn\launcher_u\.. uses 16-byte size)
 00Ch 4 Offset from KBEC+8 to Metatile Table? (18h)
 010h 4 Boundary Size (?) (but is ZERO in layout\cmn\launcher_u\)
 "Specifies the area in which the image can be drawn,
 multiplied by 64, ie. 2 means that the area is 128x128 pixels."
 014h 0Ch Zero
 020h .. Metatile Table (8 bytes each) (or 16 bytes)
 ... .. OBJ Attribute Table (6-bytes each) |
| --- |

Metatile Table entries are (8-byte or 16-byte):

```

| 000h 2 Number of OBJs
 002h 2 Unknown
 004h 4 OBJ Data Offset (from begin of OBJ Attr Table)
 (008h 2) Unknown (can be 02h,10h,48h,74h)
 (00Ah 2) Unknown (can be 08h)
 (00Ch 2) Unknown (can be FFA0h..FFF0h) ;\maybe extra coordinate offsets?
 (00Eh 2) Unknown (can be FFF0h..FFF9h) ;/ |
| --- |

OBJ Attribute Table...

```

| starts at Number of Cells * 8 | each cell is made up of 6 bytes) |
| --- |

The 6-byte OBJ Attributes seem to be in normal OAM format, containing the
coordinates, tile number, tile size, and other flags (however, the coordinates
contain signed values; ie. one needs to add the current OBJ position to those
values). For details on the OBJ Attributes, see:

LCD OBJ - OAM Attributes

**LBAL and TEXU Chunks (if any)**

Same as in Animation files (see there). In fact, the content seems to be SAME
as the corresponding Animation file (for pairs of filename.NANR and
filename.NCER), and the number of labels must be obtained from the NANR file's
KNBA chunk (as such, it's rather useless to have LBAL in NCER files, except
perhaps for error checking that the correct file pair was loaded).

Note: DSi Launcher layout\cmn\logodemo.szs has KBEC Chunk Size 2B6h (although
the filesize is padded as if it were 2B8h bytes) (the file has no LBAL chunk,
so it's unclear if/how it were aligned if present).

```

| ____________________________ Nitro Unknown Files ____________________________ |
| --- |

DSi Deep Psyche has two extra file types:

```

| .NMAR file (with "RAMN" header ID, and "KNBA"+"LBAL" chunks)
 .NMCR file (with "RCMN" header ID, and "KBCM" chunk) |
| --- |

The purpose is unknown, but they are probably also animating something...

```

| OBJ with 16bit x/y (instead 9bit/8bit)?
 OBJ with fractional x/y-stepping (moving/motion)?
 OBJ rotation/scaling?
 BG scroll offsets?
 BG tile replacement? |
| --- |

Going by the filename (a01_obj01.NMAR) they seem to be OBJ related. Labels
include things like "a01_upNN" and "a01_windowNN".

The chunks seem to resemble those in RNAN/RECN files (Animation+Cells). Except,
the KBCM has 8-byte entries (unlike the 6-byte ones in KBEC):

```

| 000h 2 Unknown (000xh..007Ah, maybe time or so?)
 002h 2 Unknown (signed 16bit?)
 004h 2 Unknown (signed 16bit?)
 006h 2 Unknown (0x21h, with x=0..8) |
| --- |

```

| _________________________ Nitro More Unknown Files __________________________ |
| --- |

Some 2D folders contain more unknown files (eg. DSi Camera
"rom:layout\cmn\fusion_camera.szs"):

**JNBL (whatever, with .bnbl extension)**

```

| 000h 4 ID "JNBL"
 004h 2 Zero
 006h 2 Number of 6-byte entries (01h or more)
 008h N*6 Unknown |
| --- |

**JNCL (whatever, with .bncl extension)**

```

| 000h 4 ID "JNCL"
 004h 2 Zero (0000h)
 006h 2 Number of 8-byte entries (01h or more)
 008h N*8 Unknown (eg. 80h,10h,C0h,20h,00h,00h,00h,00h) |
| --- |

**JNLL (whatever, with .bnll extension)**

```

| 000h 4 ID "JNLL"
 004h 2 Zero (0000h)
 006h 2 Number of 16-byte entries (01h or more)
 008h N*16 Unknown (eg. 80h,50h,60h,10h,7Ch,29h,FFh,FDh,0Dh,19h,0,0,0,0,0,0) |
| --- |

**BNGL (whatever, with .bngl extension)**

```

| 000h 4 ID "BNGL" ;this same as file extension (not JNGL)
 004h 2 Zero (0000h)
 006h 2 Number of ?-byte entries (01h or more)
 008h 2 Unknown (can be 02h,04h,06h,0Ah)
 00Ah 2 Number of ?-byte other entries maybe (01h or more)
 ...
 ... Entries?
 ... Other Entries?
 ... Maybe More Other Entries? |
| --- |

Filesize can range from 18h bytes to 24Ah bytes (or maybe yet smaller/biggger).

```

| ______________________________ .ntft and .ntfp _______________________________ |
| --- |

**.ntft file**

Texture data (maybe for use as extra 2D layer), size is usually/always a power
of 2 (ranging from 80h bytes to at least 64Kbytes (or even 512Kbytes?). Color
depth can be 16bit or 8bit (and maybe less). Files with less than 16bit are
bundled with a .ntfp palette file.

**.ntfp file**

Texture palette, with 16bit color numbers. The files can be quite small (eg.
only 6 or 8 bytes).

```

| ______________________________ .wmif and .wmpf _______________________________ |
| --- |

DSi Sudoku rom:\Textures\ has "Wild Magic" .wmif and .wmpf (image+palette)
files.

**.wmif file**

```

| 000h 1Bh ID "Wild Magic Image File 3.00",00h
 01Bh 4 Palette Filename Length (eg. 0Dh)
 01Fh LEN Palette Filename (eg. "BG_Board.wmpf")
 ... 4 Texture Format (5=2bpp, 6=4bpp, 7=8bpp) (non-standard numbering)
 ... 4 Texture Width in pixels
 ... 4 Texture Height in pixels
 ... .. Texture data (vertically mirrored, starting with lower line) |
| --- |

**.wmpf file**

```

| 000h 1Dh ID "Wild Magic Palette File 1.00",00h
 01Dh 4 Zero?
 021h 4 Number of Colors
 025h .. Colors, 16bit (0000h..7FFFh) |
| --- |

```

| _______________________________ .ntf and .xtf ________________________________ |
| --- |

**Walk with Me - Simple Texture (*.ntf)**

```

| 000h 4 Unknown, ID? (always 0Ch)
 004h 4 Unknown, ID? (always 01h) (maybe num textures, aa in .xtf ?)
 008h 2 Width in pixels (usually 8 SHL W)
 00Ah 2 Height in pixels (usually 8 SHL H)
 00Ch 2 Width shift (W)
 00Eh 2 Height shift (H)
 010h 4? Texture Format (1..7)
 014h 4 Bitmap Size (Width*Height*bpp/8)
 018h 4 Palette Size (NumColors*2)
 01Ch 4 Unknown (0)
 020h .. Bitmap Data
 ... .. Palette Data |
| --- |

**Walk with Me - Extended Texture Archive (*.xtf)**

```

| 000h 4 Offset to Texture part
 004h 4 Number of Whatever (N) ;\
 008h 4 Unknown (0Ch) ; Attributes?
 00Ch N*8 Whatever List (CCCC00xxh,Offset) ;
 ... N*var Whatever Entries ... ;/
 ... 4 Unknown (08h) ;\
 ... 4 Number of Texture Blocks (1 or more) ; Texture part
 ... .. Texture Block(s) ;/
 Texture Block:
 000h 2 Width (usually 8 SHL W) ;\
 002h 2 Height (usually 8 SHL H) ;
 004h 2 Width shift (W) ;
 006h 2 Height shift (H) ; one or more such blocks
 008h 4? Texture Format (1..7) ;
 00Ch 4 Bitmap Size (Width*Height*bpp/8) ; (about same as in .ntf files)
 010h 4 Palette Size (NumColors*2) ;
 014h 4 Unknown (0) ;
 018h .. Bitmap Data ;
 ... .. Palette Data ;/ |
| --- |

```

| ____________________________________ TEX. ____________________________________ |
| --- |

**Nanostray TEX.**

```

| 000h 4 ID ("TEX.")
 004h 4 Texture Format (1..7)
 008h 4 Width shift (W)
 00Ch 4 Height shift (H)
 010h 4 Usually zero (or, 1 in menu\planet_a\waterplanet.tex) (color0 ?)
 014h 4 Width in pixels (usually 8 SHL W)
 018h 4 Height in pixels (usually 8 SHL H, or sometimes less than 8 SHL H)
 01Ch 4 Compressed Palette Offset (34h)
 020h 4 Compressed Palette Size in bytes
 024h 4 Unknown Offset? (usually/always same as [02Ch])
 028h 4 Unknown Size? (usually/always 0=None)
 02Ch 4 Compressed Bitmap Offset
 030h 4 Compressed Bitmap Size in bytes
 034h .. Compressed Palette Data
 ... .. Compressed Bitmap Data
 The Compressed Data blocks look as so:
 000h 1 Compression Method (00h=Stored/uncompressed, 10h=LZSS/compressed)
 001h 3 Uncompressed Size
 004h .. Compressed Data
 ... .. Padding to 4-byte boundary
 The bitmap is stored in some files, and compressed in other files.
 The palette is usually/always stored. |
| --- |

```

| __________________________________ PMB+TTLP __________________________________ |
| --- |

**Cooking Coach PMB (rom:\2d\bitmap\*)**

```

| 000h 4 ID (" PMB") (aka BMP backwards)
 004h 4 Compressed Bitmap Size in bytes (can be odd, footer is unaligned)
 008h .. Compressed Bitmap (LZSS) (10h,size(24bit),compressed data)
 .. 2 Bitmap Width (100h)
 .. 2 Bitmap Height (C0h)
 .. 4 Palette Filename length
 ... .. Palette Filename ("2d/palette/name", without any trailing zero) |
| --- |

**Cooking Coach TTLP (rom:\2d\palette\*)**

```

| 000h 4 ID ("TTLP") (aka PLTT backwards)
 004h 4 Palette Size in bytes
 008h .. Palette (uncompressed, 16bit entries) |
| --- |

```

| ____________________________________ NTFA ____________________________________ |
| --- |

**NTFA**

Reportedly used by Nintendo Spot series for the whole menu (=for what?) (=DS
Station downloads for DSi Nintendo Zone and/or DS Download Play?).

The whole file is LZSS compressed (sometimes compression isn't used, but
usually only very small NTFA files are not compressed).

```

| 000h 4 Maybe Header Size (maybe 1Ch or so?)
 004h 8 ID "NTFA0000"
 00Ch 2 Color 0 Transparency (0=Solid, Nonzero=Transparent)
 00Eh 2 Color Depth (3=4bpp, 4=8bpp)
 010h 2 Bitmap Width
 012h 2 Bitmap Height
 014h 4 Bitmap Offset (maybe 1Ch+NumColors*2 ?)
 018h 4 Palette Offset (maybe 1Ch or so?)
 ... N*2 Palette Data (RGB555)
 ... .. Bitmap Data |
| --- |

The number of palette entries can be determined by subtracting image offset by
palette offset, and dividing the result by 2.

The palette is RGB15. The image is non-tiled.

| DS Files - 3D Video |
| --- |

Some NDS games are using the following set of files for 3D Models:

DS Files - 3D Video BMD0 (.NSBMD Model Data)

DS Files - 3D Video BTX0 (.NSBTX Texture Data)

DS Files - 3D Video BCA0 (.NSBCA Character Skeletal Animation)

DS Files - 3D Video BTA0 (.NSBTA Texture Coordinate Animation)

DS Files - 3D Video BTP0 (.NSBTP Texture Pattern Animation)

DS Files - 3D Video BMA0 (.NSBMA Material Animation)

DS Files - 3D Video NVA0 (.NSBVA Unknown Vis Animation)

The format was used in Nintendo's own titles somewhere between 2005 and 2009
(older launch titles did use an older .bin or .bxx format instead of .nsbxx
format, and later mini-games often don't support 3D video at all, and
third-party games tend to use their own formats). Some known titles that do use
.nsbxx are:

```

| type BMD0/BCA0/BTA0/BTX0/BTP0/BMA0 is used by Mariokart DS (2005)
 type BMD0/BCA0/BTA0 is used by Walk with Me (2008-2009)
 type BVA0 is used by what... if it was ever used? |
| --- |

Note: The format resembles the later CGFX format used on 3DS.

**Dict - Directory with Name/Data (used in various places in 3D video files)**

```

| 000h 1 Dummy (00h)
 001h 1 Number of entries (N)
 002h 2 Size of whole Dict (Hdr(8)+Tree(4+X*4)+Data(4+N*siz)+Name(N*10h))
 004h 2 Offset to Tree Section (always 08h)
 006h 2 Offset to Data Section (0Ch+X*4)
 008h 4 Tree entry 0 (Patricia Tree Root entry) ;\Tree
 00Ch X*4 Tree entry 1..X (Patricia Tree and actual Names/Data) ;/
 ... 2 Size of each Data entry (siz) (usually 4 or 8) ;\
 ... 2 Size of this Data Info Section (4+N*siz) ; Data
 ... N*siz Data (siz bytes, for each entry) ;/
 ... N*10h Name Strings (10h-byte ASCII, zeropadded, for each entry) ;-Names |
| --- |

Patricia Tree Root entry:

```

| 000h 1 Patricia Tree First Bit-number (always 7Fh=Last=char[0Fh].bit7)
 001h 1 Patricia Tree First entry (1..X) (usually points to rightmost bit)
 002h 2 Patricia Tree Unused (zerofilled) |
| --- |

Patricia Tree entry 1..X format:

```

| 000h 1 Patricia Tree Bit-number to be tested (0=Bit0 of 1st char)
 001h 1 Patricia Tree Next entry when test=0 ;\upon match: Next=Curr
 002h 1 Patricia Tree Next entry when test=1 ;/upon error: Next=0
 003h 1 Entry number in Data and Name tables (used when Next=Curr entry) |
| --- |

The Patricia Tree is intended for searching Symbols by testing a few bits
(without comparing all characters).

The Data content depends on what the Dict is used for. Each name can have a
small amount of data stored inside of the Dict (and, that Data may contain
offsets to larger data stored elsewhere).

**References**

[https://github.com/scurest/nsbmd_docs/blob/master/nsbmd_docs.txt](https://github.com/scurest/nsbmd_docs/blob/master/nsbmd_docs.txt)

Errata:

```

| 3D Video Container/Headers have version=1 (except: BMD0 has version=2)
 BoundingBox seems to have "origin/size" values (not "min/max" values)
 SHININESS is a 128-byte array, that's definietly not encoded in 4-byte entry
 Material seems to contain OR-mask and AND-mask values for POLYGON_ATTR ?
 Material can have further parameters, in the TODO part?
 MaterialIdxList offsets are relative to Model[008h], not to MaterialIdxList
 Pivot should use C=B and D=A (not C=A and D=B) for sine/cosine rotations?
 BasisMatrix description could be simplified, and are the unknown 2bit used?
 Animations have IDs "J.AC","M.AT", "M.PT", "M.AM" (with "."=00h)
 Dict's (aka NameList's) contain Patricia Trees for fast lookup (alike 3DS)
 SRT0 does "Texture Coordinate Animation" (not "Material Animation")
 SRT0 probably supports texture Scale/Rotate/Translate (not just Translate)
 TEX0[04h]=Size of TEX0
 TEX0[18h]=Padding32bit (is MISSING in .txt, insert this after "block1_off")
 TEX0[1Eh]=CompressedTextureDictOffs (alias for TEX[0Eh]=TextureDictOffs)
 Texture[004h] is 11bit width, 11bit height (from TEXIMAGE bits), and bit31=1
 NSBMA files do exists (eg. in Mariokart DS), and could/should be described
 NSBVA files... would be nice to know if they are used by any games? |
| --- |

| DS Files - 3D Video BMD0 (.NSBMD Model Data) |
| --- |

**BMD0 Header (in .NSBMD files)**

```

| 000h 4 ID "BMD0" (Basic Model Data)
 004h 2 Byte Order (FEFFh)
 006h 2 Version (2) (unknown if version 1 did also exist)
 008h 4 Total Filesize
 00Ch 2 Header size, excluding the Chunk offsets (always 10h)
 00Eh 2 Number of chunks (1=MDL0 or 2=MDL0+TEX0)
 010h 4 Offset from BMD0 to MDL0 Chunk
 014h 4 Offset from BMD0 to TEX0 Chunk (if any) |
| --- |

**TEX0 Chunk (if any)**

Same as in BTX0 files, see there for details.

DS Files - 3D Video BTX0 (.NSBTX Texture Data)

**MDL0 Chunk**

```

| 000h 4 Chunk ID "MDL0" (Model Block)
 004h 4 Chunk Size
 008h .. Model Dict (with 32bit offsets from MDL0 to Models)
 ... .. Models |
| --- |

**Model**

A Model is 3D model. The process of drawing a Model consists of executing a
list of RenderCommands, which calculate skinning matrices, set material
properties, and draw the individual pieces of the Model (the Meshes).

```

| 000h 4 Size of Model in bytes
 004h 4 Offset from Model to RenderCommandList
 008h 4 Offset from Model to Material info
 00Ch 4 Offset from Model to Dict for Meshes
 010h 4 Offset from Model to InvBindMatrices
 014h 1 Unknown (00h)
 015h 1 Unknown (00h or 01h)
 016h 1 Unknown (00h)
 017h 1 Number of BoneMatrices
 018h 1 Number of Materials
 019h 1 Number of Meshes
 01Ah 2 Unknown (can be ZERO, or same as Number of BoneMatrices?)
 01Ch 4 Scaling Factor for Up Scale command (fixed point, 1.19.12)
 020h 4 Scaling Factor for Down Scale command (fixed point, 1.19.12)
 024h 2 Number of Vertices
 026h 2 Number of Polygons (Triangles+Quads)
 028h 2 Number of Triangles
 02Ah 2 Number of Quads
 02Ch 2 Bounding Box X-Coordinate (fixed point, 1.3.12) ;\
 02Eh 2 Bounding Box Y-Coordinate (fixed point, 1.3.12) ;
 030h 2 Bounding Box Z-Coordinate (fixed point, 1.3.12) ; for BOX_TEST
 032h 2 Bounding Box X-Size, Width (fixed point, 1.3.12) ; command
 034h 2 Bounding Box Y-Size, Height (fixed point, 1.3.12) ;
 036h 2 Bounding Box T-Size, Depth (fixed point, 1.3.12) ;/
 038h 4 Unknown (1000h) ;\maybe fixed point value 1.0 ?
 03Ch 4 Unknown (1000h) ;/
 040h .. Dict for BoneMatrices (with 32bit offsets from 040h to BoneMatrices)
 [04h] .. RenderCommandList
 [08h]+0 2 Offset from [08h] to Dict for Material-to-Texture Pairings ;\
 [08h]+2 2 Offset from [08h] to Dict for Material-to-Palette Pairings ;
 [08h]+4 .. Dict for Materials (with 32bit offsets from [08h] to Materials) ;
 ... .. Dict for Material-to-Texture Pairings (with 4-byte entries) ;
 ... .. Dict for Material-to-Palette Pairings (with 4-byte entries) ;/
 [0Ch] .. Dict for Meshes (with 32bit offsets from [0Ch] to VertexMeshes)
 [10h] .. InvBindMatrices (54h-byte each) |
| --- |

```

| _____________________________ RenderCommandList ______________________________ |
| --- |

**RenderCommandList**

The RenderCommandList is a software script for drawing the Model. The list
consists of command bytes and parameter bytes, the list is terminated by
command 01h.

The commands must be processed by software (they aren't GPU commands). Apart
from the commands bytes, the upper 3bit of the command number do contain some
parameter bits that affect the behaviour of the command.

```

| Cmd Params Description
 00h 0 Nop (no operation?)
 40h 0 Nop (same as above?)
 80h 0 Nop (same as above?)
 01h 0 End of RenderCommandList
 02h 2 Unknown ;reportedly, params: Node ID, Visibility
 03h 1 Load Matrix from Stack (param=StackIndex)
 04h 1 Bind Material for subsequent Draw command (param=MaterialIndex)
 24h 1 Bind Material (same as above?)
 44h 1 Bind Material (same as above?)
 05h 1 Draw VertexMesh (param=VertexMeshIndex)
 06h 3 Multiply Matrix by BoneMatrix (see below)
 26h 4 Multiply Matrix by BoneMatrix and Store Matrix to Stack
 46h 4 Multiply Matrix by BoneMatrix and Load Matrix from Stack
 66h 5 Multiply Matrix by BoneMatrix and Load/Store Matrix from/to Stack
 07h 1 Unknown
 47h 2? Unknown (as above? but with 2 params?)
 08h 1 Unknown
 09h var Calculate Skinning Equation (see below)
 0Ah - Unused?
 0Bh 0 Scale Up (using the scale factor in Model[01Ch]) ;or "BEGIN"?
 2Bh 0 Scale Down (using the scale factor in Model[020h]( ;or "END"?
 0Ch 2 Unknown
 0Dh 2 Unknown
 0Eh - Unused?
 0Fh - Unused?
 1xh - Unused? |
| --- |

**Multiply Current Matrix with Bone Matrix (cmd 06h, 26h, 46h, 66h)**

Multiplies the current matrix by a BoneMatrix. This is used to build the
local-to-world matrices out of the BoneMatrices.

```

| bone_idx = next_param ;index of BoneMatrix ;1st param
 parent_idx = next_param ;Parent of the Bone ;2nd param
 unknown = next_param ;? ;3rd param
 if cmd.bit6 then CurrMatrix = MatrixStack[next_param] ;4th param (if any)
 CurrMatrix *= BoneMatrices[bone_idx]
 if cmd.bit5 then MatrixStack[next_param] = CurrMatrix ;Last param (if any) |
| --- |

Note: If bone_idx is the index of bone B, then parent_idx will be the index of
the parent of B. "I don't think it's used at all at runtime."

**Calculate Skinning Equation (cmd 09h, with 2+N*3 parameters)**

This (the "Equation"?) is the matrix applied to a vertex influenced by multiple
bones. The inverse bind matrices bring the vertex into the local space of each
bone and the local-to-world transforms send it to its world space position.

```

| CurrMatrix = 0
 store_index = next_param ;1st param
 num_terms = next_param ;2nd param
 loop num_terms times:
 term = MatrixStack[next_param] ;local-to-world matrix ;3rd,6th,..
 term *= InvBindMatrices[next_param] ;4th,7th,..
 term *= next_param / 256 ;weight ;5th,8th,..
 CurrMatrix += term
 MatrixStack[store_index] = CurrMatrix |
| --- |

This command is only used when there are vertices influenced by multiple bones
(if a vertex is only influenced by a single bone, then its position will just
be stored (in the Mesh) in the space of that bone, so there's no need for an
InvBindMatrix to bring it into the correct space).

```

| _______________________________ InvBindMatrix ________________________________ |
| --- |

**InvBindMatrix entries**

InvBindMatrices are used for computing the skinning matrix via render command
09h.

```

| 000h 30h Position Matrix (3x4, fixed point 1.19.12) (rotate,scale,translate)
 030h 24h Vector Matrix (3x3, fixed point 1.19.12) (rotate only, for light) |
| --- |

```

| _________________________________ VertexMesh _________________________________ |
| --- |

**VertexMesh**

A Mesh contains actual vertex data in the form of a blob of GPU
commands/parameters. To draw a Mesh, you just submit the blob of commands to
the GPU's Geometry Command FIFO (GXFIFO).

```

| 000h 2 Dummy
 002h 2 Unknown (0010h, possibly the size of VertexMesh?)
 004h 4 Unknown
 008h 4 Offset from VertexMesh to GXFIFO Command List
 00Ch 4 Size of GXFIFO Command List in bytes |
| --- |

Only certain GPU commands appear in a Mesh. Here is a list:

```

| 00h=NOP, 14h=MTX_RESTORE, 1Bh=MTX_SCALE, 40h=BEGIN_VTXS, 41h=END_VTXS
 20h=COLOR, 21h=NORMAL, 22h=TEXCOORD
 23h=VTX_16, 24h=VTX_10,25h=VTX_XY, 26h=VTX_XZ, 27h=VTX_YZ, 28h=VTX_DIFF |
| --- |

```

| _________________________________ Materials __________________________________ |
| --- |

**Material**

A Material is a bunch of GPU state (eg. colors, whether backface culling is
enabled, etc.) to be set when the Material is bound. It also determines the
texture/palette to use, though that isn't stored in this Material object
itself.

```

| 000h 2 Dummy (0)
 002h 2 Size of this Material in bytes (002Ch)
 004h 4 Value for DIF_AMB register
 008h 4 Value for SPE_EMI register
 00Ch 4 Value for POLYGON_ATTR register ;<-- OR value?
 010h 4 Mask for POLYGON_ATTR register (1F3FF8FFh) ? ;<-- AND value?
 014h 4 Value for TEXIMAGE_PARAMS register (bit16-19 and 30-31, see below)
 018h 4 Unknown (FFFFFFFFh)
 01Ch 4 Unknown (1FCE0000h)
 020h 2 Texture_width
 022h 2 Texture_height
 024h 4 Unknown (00001000h)
 028h 4 Unknown (00001000h)
 XXX above is 2Ch-bytes (other source says "Usually 48 bytes" aka 30h-byte)
 XXX unknown if above can have optionaly extra entries
 XXX reportedly above can optionally contain a texcoord transform matrix?
 XXX unknown if above contains 16bit COLOR (eg. for texture-less polygons) |
| --- |

Only some TEXIMAGE_PARAMS bits are stored here; the others are zeroed out. They
are stored in the teximage_params in the Texture object. These two
teximage_param u32s are or-ed together to give the final argument to
TEXIMAGE_PARAMS. The fields stored here are:

```

| 0-15 Zero in Model/Material (instead, derived from TEX0)
 16 Repeat in S Direction (0=Clamp Texture, 1=Repeat Texture)
 17 Repeat in T Direction (0=Clamp Texture, 1=Repeat Texture)
 18 Flip in S Direction (0=No, 1=Flip each 2nd Texture; requires Repeat)
 19 Flip in T Direction (0=No, 1=Flip each 2nd Texture; requires Repeat)
 20-29 Zero in Model (instead, derived from TEX0)
 30-31 Texture Coordinates Transformation Mode (0..3) |
| --- |

**4-byte entries in Dict for Material-to-Texture/Palette Pairings**

```

| 000h 2 Offset from Model[008h] (?) to List of 8bit Materials Indices
 002h 1 Number of entries in the List of 8bit Materials Indices
 003h 1 Dummy (0) |
| --- |

The Texture/Palette names for different Materials are stored in Dict's with the
above 4-byte entries. The assignement is somewhat reversed (normally one would
define "Material 5 uses Texture Name 8", but the Dict's are instead defining
"Texture Name 8 is used by Material Name 2, 5 and 6").

```

| ________________________________ BoneMatrices ________________________________ |
| --- |

**BoneMatrix**

A BoneMatrix stores the local-to-parent transform of some bone.

When a Model is animated by an Skeletal Animation, the only thing that changes
are its BoneMatrices.

Note: A Model doesn't contain any actual bones or skeleton information (but see
the parent_idx parameter to render command 06h); that has all been compiled
down to an imperative list of rendering commands that build up all the
necessary skinning matrices directly. The BoneMatrices just store the data from
the bones that are needed by these rendering commands.

```

| 00h 2 Flags
 0 Disable Translation (0=Enable, 1=Disable)
 1 Disable Rotation (0=Enable, 1=Disable)
 2 Disable Scale (0=Enable, 1=Disable)
 3 Matrix Type for Rotation (0=3x3 Matrix, 1=Pivot Matrix)
 4-7 Rotation Pivot Form ;\
 8 Rotation Pivot NegI ; for Pivot Matrix Rotation
 9 Rotation Pivot NegC ;
 10 Rotation Pivot NegD ;/
 11-15 Unused (0)
 02h 2 Rotation Matrix m0 (fixed point, 1.3.12) ;-here for alignment reasons
 ... 4 Translation X (fixed point, 1.19.12) ;\
 ... 4 Translation Y (fixed point, 1.19.12) ; only if flags.bit0=0
 ... 4 Translation Z (fixed point, 1.19.12) ;/
 ... 2 Rotation Matrix m1 (fixed point, 1.3.12) ;\
 ... 2 Rotation Matrix m2 (fixed point, 1.3.12) ; only if Flags.bit3=0=3x3
 ... 2 Rotation Matrix m3 (fixed point, 1.3.12) ; and Flags.bit1=0
 ... 2 Rotation Matrix m4 (fixed point, 1.3.12) ; [ m0 m3 m6 ]
 ... 2 Rotation Matrix m5 (fixed point, 1.3.12) ; [ m1 m4 m7 ]
 ... 2 Rotation Matrix m6 (fixed point, 1.3.12) ; [ m2 m5 m8 ]
 ... 2 Rotation Matrix m7 (fixed point, 1.3.12) ;
 ... 2 Rotation Matrix m8 (fixed point, 1.3.12) ;/
 ... 2 Rotation Pivot A (fixed point, 1.3.12) ;\only if Flags.bit3=1=Pivot
 ... 2 Rotation Pivot B (fixed point, 1.3.12) ;/ (and Flags.bit1=0?)
 ... 4 Scale X (fixed point, 1.19.12) ;\
 ... 4 Scale Y (fixed point, 1.19.12) ; only if Flags.bit2=0
 ... 4 Scale Z (fixed point, 1.19.12) ;/ |
| --- |

The Pivot Matrix can be used for simple rotation around X, Y, or Z axis; see
the Skeletal Animation chapter for info on how to convert the Pivot parameters
(A, B, Form, NegI, NegC, NegD) into a 3x3 matrix.

| DS Files - 3D Video BTX0 (.NSBTX Texture Data) |
| --- |

Contains Texture bitmaps and Palettes. The TEX0 Chunk can exist in .NSBTX
Texture files, or in .NSBMD Model files.

**BTX0 Header (in .NSBTX files)**

```

| 000h 4 ID "BTX0" (Basic Texture)
 004h 2 Byte Order (FEFFh)
 006h 2 Version (1)
 008h 4 Total Filesize
 00Ch 2 Header size, excluding the Chunk offsets (always 10h)
 00Eh 2 Number of chunks (1=TEX0)
 010h 4 Offset from BTX0 to TEX0 Chunk |
| --- |

**TEX0 Chunk (can occur in .NSBTX and .NSBMD files)**

```

| 000h 4 Chunk ID "TEX0" (Texture Block)
 004h 4 Chunk Size
 008h 4 Padding (0)
 00Ch 2 Texture Data Size/8 ;\Texture
 00Eh 2 Texture Dict Offset (03Ch) ; (Format 1..4
 010h 4 Padding (0) ; and 6..7)
 014h 4 Texture Data Offset ;/
 018h 4 Padding (0)
 01Ch 2 Compressed Texture Data Size/12 (often 0=none) ;\
 01Eh 2 Compressed Texture Dict Offset (03Ch, too) ; Compressed
 020h 4 Padding (0) ; Texture
 024h 4 Compressed Texture Offset for 4x4-Texel Data ; (Format 5)
 028h 4 Compressed Texture Offset for 4x4-Texel Attr ;/
 02Ch 4 Padding (0)
 030h 4 Palette Data Size/8 ;\
 034h 4 Palette Dict Offset ; Palette
 038h 4 Palette Data Offset ;/
 [0Eh] .. Texture Dict (with 8-byte entries, see below) ;\Dict's
 [34h] .. Palette Dict (with 4-byte entries, see below) ;/
 [14h] [0Ch]*8 Texture Data ;\
 [24h] [1Ch]*8 Compressed Texture 4x4-Texel Data (2bpp) ; VRAM Data
 [28h] [1Ch]*4 Compressed Texture 4x4-Texel Attr (1bpp) ;
 [38h] [30h]*8 Palette Data Section ;/ |
| --- |

**8-byte entries in Texture Dict:**

This Dict is shared for Textures and Compressed Textures (ie. TEX0 entries
[00Eh] and [01Eh] do both point to the same Dict).

```

| 000h 4 Value for TEXIMAGE_PARAM register
 0-15 Texture Data (Offset/8, in File Data / VRAM Data)
 16-19 Zero in TEX0 (instead, derived from Model's Material)
 20-22 Texture S-Size (W) (for W=0..7: Width=(8 SHL W)
 23-25 Texture T-Size (H) (for H=0..7: Height=(8 SHL H)
 26-28 Texture Format (0..7)
 29 Palette Color 0 (0=Displayed, 1=Made Transparent)
 30-31 Zero in TEX0 (instead, derived from Model's Material)
 004h 4 Width/Height (contains the above 3bit W/H values decoded to 11bit)
 0-10 Width in pixels (8 shl W) (8..1024)
 11-21 Height in pixels (8 shl H) (8..1024)
 22-30 Unknown (0)
 31 Unknown (1) |
| --- |

**4-byte entries in Palette Dict:**

```

| 000h 2 Value for PLTT_BASE register
 0-12 Palette Base (Offset/8, in File Data / VRAM Data)
 13-15 Unused (0)
 002h 2 Unknown (usually 0, sometimes 1) ;unrelated to number of colors |
| --- |

**Note**

The offsets in LSBs of TEXIMAGE_PARAM and PLTT_BASE are relative the begin of
the corresponding Data block (and need to be adjusted depending on where the
data is stored in VRAM).

**Texture/Palette Names**

TEX0 has Textures and Palettes are stored in separate Dict's. One could usually
guess which Texture belongs to which Palette, but there is no clear naming
scheme.

For example, MarioKart DS "rom:\data\Course\mini_stage1Tex.carc\*.nsbtx":

```

| Texture Name: "MS_ttl_1_2" ;\both SAME name
 Palette Name: "MS_ttl_1_2" ;/
 Texture Name: "MS_ttl_3" ;\palette with extra "_pl" suffix (or "_p")
 Palette Name: "MS_ttl_3_pl" ;/
 Texture Name: "MS1_11_2" ;\palette without suffix
 Palette Name: "MS1_11" ;/
 Texture Name: "nr_space3_2" ;\palette without prefix
 Palette Name: "space3_2" ;/ |
| --- |

| DS Files - 3D Video BCA0 (.NSBCA Character Skeletal Animation) |
| --- |

An Animation is a skeletal animation of a Model. It changes the values of the
BoneMatrices in a Model over time. All animations are frame-based, so "time"
always means "frame number".

An Animation contains a collection of Tracks. A Track targets one of the
BoneMatrices in a Model and tells you what its value should be at each time.

**BCA0 Header (in .NSBCA files)**

```

| 000h 4 ID "BCA0" (Basic Character Animation)
 004h 2 Byte Order (FEFFh)
 006h 2 Version (1)
 008h 4 Total Filesize
 00Ch 2 Header size, excluding the Chunk offsets (always 10h)
 00Eh 2 Number of chunks (1=JNT0)
 010h 4 Offset from BCA0 to JNT0 Chunk |
| --- |

**JNT0 Chunk**

```

| 000h 4 Chunk ID "JNT0" (Joint Block)
 004h 4 Chunk Size
 008h .. Joint Dict (with 32bit offsets from JNT0 to J.AC) |
| --- |

**Joint Animations**

```

| 000h 4 ID "J",00h,"AC" (nicknamed "J.AC") (Joint Animation Content ?)
 004h 2 Number of Frames
 006h 2 Number of Tracks (T)
 008h 4 Unknown (3)
 00Ch 4 Offset from J.AC to PivotMatrices
 010h 4 Offset from J.AC to BasisMatrices
 014h 2*T Offsets from J.AC to AnimationTrack(s)
 ... .. Padding to 4-byte boundary (if needed)
 ... .. AnimationTrack(s)
 ... .. PivotMatrices (06h-bytes each) ;\used for Rotations
 ... .. BasisMatrices (0Ah-bytes each) ;/(see "Curve" entries) |
| --- |

**AnimationTrack**

```

| 000h 2 Flags (eg. 3028h)
 0 No Channel
 1-2 No Translation channels ;why 2bit?
 3 Translation X is constant
 4 Translation Y is constant
 5 Translation Z is constant
 6-7 No Rotation channel ;why 2bit?
 8 Rotation is constant
 9-10 No Scale channels ;why 2bit?
 11 Scale X is constant
 12 Scale Y is constant
 13 Scale Z is constant
 14-15 Unknown (0) (unused?)
 002h 1 Dummy (00h)
 003h 1 target_index (the index of the BoneMatrix this track targets?)
 ... .. Curve, Translation X ;\
 ... .. Curve, Translation Y ; only if Flags.bit0=0 and Flags.bit1-2=0
 ... .. Curve, Translation Z ;/
 ... .. Curve, Rotation ;-only if Flags.bit0=0 and Flags.bit6-7=0
 ... .. Curve, Scale X ;\
 ... .. Curve, Scale Y ; only if Flags.bit0=0 and Flags.bit9-10=0
 ... .. Curve, Scale Z ;/ |
| --- |

```

| ___________________________________ Curves ___________________________________ |
| --- |

**Curve**

The format of the Curve entries varies for Translation, Rotation, Scaling, and
depends on whether the above Flags entry has Constant=1 (ie. using the same
value for all frames).

```

| When Constant=1 and Translation X/Y/Z:
 000h 4 Translation value (fixed point, 1.19.12)
 When Constant=1 and Rotation:
 000h 2 Matrix Index/Type (bit0-14=Index, bit15=Type: 0=Basis, 1=Pivot)
 002h 2 Unused (padding for alignment, probably)
 When Constant=1 and Scaling X/Y/Z:
 000h 4 Scaling Value (fixed point, 1.19.12)
 004h 4 Unknown (fixed point, 1.19.12)
 When Constant=0:
 000h 4 Frame Info
 bit0-15 Start_frame (start on this frame, step=samplerate)
 bit16-27 End_frame (up to excluding this frame)
 bit28-29 Width for Scale/Translate (0=32bit, 1=16bit)
 bit30-31 Samplerate (0,1,2,3 = Each 1,2,4,8 frames)
 004h 4 Offset from J.AC to SampledCurve |
| --- |

**SampledCurve (when Constant=0)**

A sampled curve is always sampled at a fixed rate between two endpoints: one
sample is stored at each of the frames

```

| num_samples=(end_frame-start_frame)/samplerate
 When Translation X/Y/Z, and Width=0:
 000h 4 Sample (fixed point, 1.19.12)
 When Translation X/Y/Z, and Width=1:
 000h 2 Sample (fixed point, 1.3.12)
 When Rotation:
 000h 2 Matrix Index/Type (bit0-14=Index, bit15=Type: 0=Basis, 1=Pivot)
 Whan Scaling X/Y/Z, and Width=0:
 000h 4 Sample (fixed point, 1.19.12)
 004h 4 Unknown (fixed point, 1.19.12)
 Whan Scaling X/Y/Z, and Width=1:
 000h 2 Sample (fixed point, 1.3.12)
 002h 2 Unknown (fixed point, 1.3.12) |
| --- |

The rotation Matrix Index/Type points to a rotation matrix stored in the
PivotMatrices or BasisMatrices arrays for this Animation. The highest bit tells
you which array it's in, and the low bits give the index into that array.

```

| ________________________________ PivotMatrix _________________________________ |
| --- |

A PivotMatrix encodes a rotation matrix for X, Y, or Z-axis in 6 bytes.

```

| 000h 2 Flags (bit0-3=Form, bit4=NegI, bit5=NegC, bit6=NegD, bit7-15=Unused)
 002h 2 Value A (fixed point, 1.3.12) ;\usually cos/sin values
 003h 2 Value B (fixed point, 1.3.12) ;/ |
| --- |

To get the whole matrix, set C=A, D=B, I=1.0, negate them if NegC, NegD, NegI
are set, then arrange A,B,C,D,I depending on the Form bits:

```

| [ i 0 0 ] [ 0 a c ] [ 0 a c ]
 Form0 = [ 0 a c ] Form1 = [ i 0 0 ] Form2 = [ 0 b d ]
 (X) [ 0 b d ] [ 0 b d ] [ i 0 0 ] |
| --- |

```

| [ 0 i 0 ] [ a 0 c ] [ a 0 c ]
 Form3 = [ a 0 c ] Form4 = [ 0 i 0 ] Form5 = [ b 0 d ]
 [ b 0 d ] (Y) [ b 0 d ] [ 0 i 0 ] |
| --- |

```

| [ 0 0 i ] [ a c 0 ] [ a c 0 ]
 Form6 = [ a c 0 ] Form7 = [ 0 0 i ] Form8 = [ b d 0 ]
 [ b d 0 ] [ b d 0 ] (Z) [ 0 0 i ] |
| --- |

XXX shouldn't that use C=B and D=A for usual sine/cosine rotations?

XXX or does that have rows/columns swapped, to match gbatek notation?

XXX what is the practical use of nine formats? three are for x/y/z axis...

XXX what is the default for NegC, NegD, NegI

```

| ________________________________ BasisMatrix _________________________________ |
| --- |

A BasisMatrix encodes an arbitrary rotation matrix as six 13bit fixed point
1.0.12 values in 10 bytes.

```

| 000h 2 bit3-15=a0.bit0-12, and bit0-2=b2.bit9-11
 002h 2 bit3-15=a1.bit0-12, and bit0-2=b2.bit6-8
 004h 2 bit3-15=a2.bit0-12, and bit0-2=b2.bit3-5
 006h 2 bit3-15=b0.bit0-12, and bit0-2=b2.bit0-2
 008h 2 bit3-15=b1.bit0-12, and bit0=b2.bit12, and bit1-2=unknown? |
| --- |

The remaining three matrix elements are computed as:

```

| (c0,c1,c2) = (a0,a1,a2) x (b0,b1,b2) ;cross product |
| --- |

And the rotation matrix is then:

```

| [ a0 b0 c0 ]
 [ a1 b1 c1 ]
 [ a2 b2 c2 ] |
| --- |

XXX or does that have rows/columns swapped, to match gbatek notation?

| DS Files - 3D Video BTA0 (.NSBTA Texture Coordinate Animation) |
| --- |

Allows Texture scrolling... and maybe also texture scale/rotate?

**BTA0 Header (in .NSBTA files)**

```

| 000h 4 ID "BTA0" (Texture Animation)
 004h 2 Byte Order (FEFFh)
 006h 2 Version (1)
 008h 4 Total Filesize
 00Ch 2 Header size, excluding the Chunk offsets (always 10h)
 00Eh 2 Number of chunks (1=SRT0)
 010h 4 Offset from BTA0 to SRT0 Chunk |
| --- |

**SRT0 Chunk**

```

| 000h 4 Chunk ID "SRT0" (maybe short for Scale/Rotate/Translate?)
 004h 4 Chunk Size
 008h .. Dict (with 32bit offsets from SRT0 to Animations)
 ... .. Animation(s) |
| --- |

**Animation**

```

| 000h 4 ID "M",00h,"AT" (nicknamed "M.AT")
 004h 2 Unknown (can be 3Bh) ;is that Number of Frames?
 006h 2 Unknown (can be 0)
 008h .. Dict (with 28h-byte Track entries)
 ... .. Key Frames |
| --- |

**Track Entries (in Dict)**

```

| 000h 8 Channel 0 - unknown, maybe Scale U ;\
 008h 8 Channel 1 - unknown, maybe Scale V ; maybe scale/rotate?
 010h 8 Channel 2 - unknown, maybe Rotate Matrix ;/
 018h 8 Channel 3 - Translation U ;\texture scrolling
 020h 8 Channel 4 - Translation V ;/ |
| --- |

**Channel (within Tracks in Dict)**

```

| 000h 2 Number of Keyframes (can be 3Bh)
 002h 2 Flags (1000h, 2000h or 3000h) (maybe loop flags and/or rate?)
 004h 4 When Flags=1000h: Offset from "M.AT" to Keyframe(s)
 When Flags=2000h: Unknown (10000000h) ;\maybe disable, or fixed
 When Flags=3000h: Unknown (1000h or 0) ;/setting for all frames? |
| --- |

**Keyframe Entries**

```

| 000h 2 Value (for Translation: fixed point 1.10.5) |
| --- |

| DS Files - 3D Video BTP0 (.NSBTP Texture Pattern Animation) |
| --- |

Allows to use different textures/palettes in each frame (ie. to use a short
"movie clip" instead of a static "bitmap" as texture).

**BTP0 Header (in .NSBTP files)**

```

| 000h 4 ID "BTP0" (Texture Pattern Animation)
 004h 2 Byte Order (FEFFh)
 006h 2 Version (1)
 008h 4 Total Filesize
 00Ch 2 Header size, excluding the Chunk offsets (always 10h)
 00Eh 2 Number of chunks (1=PAT0)
 010h 4 Offset from BTP0 to PAT0 Chunk |
| --- |

**PAT0 Chunk**

```

| 000h 4 Chunk ID "PAT0" (Pattern Block)
 004h 4 Chunk Size
 008h .. Dict (with 32bit offsets from PAT0 to Pattern Animations)
 ... .. Pattern Animation(s) |
| --- |

**Pattern Animation**

```

| 000h 4 ID "M",00h,"PT" (nicknamed "M.PT")
 004h 2 Unknown (can be 08h or 32h) ;rather NOT Number of Frames?
 006h 1 Number of Texture Names (Y)
 007h 1 Number of Palette Names (Z)
 008h 2 Offset from "M.PT" to Texture Names
 00Ah 2 Offset from "M.PT" to Palette Names
 00Ch .. Dict (with 8-byte Track entries; includes offsets to Key Frames)
 ... X*4 Key Frames
 ... Y*10h Texture Names (each 10h-byte ASCII, zeropadded)
 ... Z*10h Palette Names (each 10h-byte ASCII, zeropadded) |
| --- |

**Track Entries (in Dict)**

```

| 000h 4 Number of Keyframes
 004h 2 Unknown (1000h or 00F5h) (maybe loop flags and/or framerate etc.)
 006h 2 Offset from "M.PT" to Keyframe(s) |
| --- |

**Keyframe Entries**

```

| 000h 2 Frame Number (increasing 0000h..NumKeyFrames-1)
 002h 1 Index into Texture Names
 003h 1 Index into Palette Names |
| --- |

| DS Files - 3D Video BMA0 (.NSBMA Material Animation) |
| --- |

Allows to animate "Material" whatever that means... maybe colors/lights?

**BMA0 Header (in .NSBMA files)**

```

| 000h 4 ID "BMA0" (Material Animation)
 004h 2 Byte Order (FEFFh)
 006h 2 Version (1)
 008h 4 Total Filesize
 00Ch 2 Header size, excluding the Chunk offsets (always 10h)
 00Eh 2 Number of chunks (1=MAT0)
 010h 4 Offset from BMA0 to MAT0 Chunk |
| --- |

**MAT0 Chunk**

```

| 000h 4 Chunk ID "MAT0" (Material Block)
 004h 4 Chunk Size
 008h .. Dict (with 32bit offsets from PAT0 to Pattern Animations)
 ... .. Animation(s) |
| --- |

**Animations**

```

| 000h 4 ID "M",00h,"AM" (nicknamed "M.AM")
 004h 2 Unknown (can be 5Ah) ;is that Number of Frames?
 006h 2 Unknown (can be 3)
 008h .. Dict (with 14h-byte Track entries)
 ... .. Key Frames |
| --- |

**Track Entries (in Dict)**

```

| 000h 2 Unknown (40h) Offset from "M.AM" to Keyframes ;\
 002h 1 Unknown (5Ah) Number of Keyframes (5Ah*2=B4h bytes) ; ch0
 003h 1 Unknown (0) Flags ;/
 004h 2 Unknown (F4h) Offset from "M.AM" to Keyframes ;\
 006h 1 Unknown (5Ah) Number of Keyframes (5Ah*2=B4h bytes) ; ch1
 007h 1 Unknown (0) Flags ;/
 008h 2 Unknown (0) Maybe fixed value for all frames? ;\
 00Ah 1 Unknown (5Ah) Number of Keyframes ; ch2
 00Bh 1 Unknown (20h) Flags (20h=Fixed/Disabled?) ;/
 00Ch 2 Unknown (0) Maybe fixed value for all frames? ;\
 00Eh 1 Unknown (5Ah) Number of Keyframes ; ch3
 00Fh 1 Unknown (20h) Flags (20h=Fixed/Disabled?) ;/
 010h 2 Unknown (1Fh) Maybe fixed value for all frames? ;\
 012h 1 Unknown (5Ah) Number of Keyframes ; ch4
 013h 1 Unknown (20h) Flags (20h=Fixed/Disabled?) ;/ |
| --- |

**Keyframe Entries**

```

| 000h 2 Unknown |
| --- |

| DS Files - 3D Video NVA0 (.NSBVA Unknown Vis Animation) |
| --- |

Allows to do dunno what... maybe animate alpha/transparency?

**BVA0 Header (in .NSBVA files)**

```

| 000h 4 ID "BVA0" (whatever Vis... Animation?)
 ... .. XXX |
| --- |

**VIS0 Chunk**

```

| 000h 4 Chunk ID "VIS0" (Visibility...?)
 004h 4 Chunk Size
 ... .. Unknown |
| --- |