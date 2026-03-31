# Dstechnicaldata

> Source: https://problemkaputt.de/gbatek.htm
> Section: Dstechnicaldata

DS Technical Data 
| |
| --- |

 **Processors**
 
```

| 1x ARM946E-S 32bit RISC CPU, 66MHz (NDS9 video) (not used in GBA mode)
 1x ARM7TDMI 32bit RISC CPU, 33MHz (NDS7 sound) (16MHz in GBA mode) |
| --- |

**Internal Memory**

```

| 4096KB Main RAM (8192KB in debug version)
 96KB WRAM (64K mapped to NDS7, plus 32K mappable to NDS7 or NDS9)
 60KB TCM/Cache (TCM: 16K Data, 32K Code) (Cache: 4K Data, 8K Code)
 656KB VRAM (allocateable as BG/OBJ/2D/3D/Palette/Texture/WRAM memory)
 4KB OAM/PAL (2K OBJ Attribute Memory, 2K Standard Palette RAM)
 248KB Internal 3D Memory (104K Polygon RAM, 144K Vertex RAM)
 ?KB Matrix Stack, 48 scanline cache
 8KB Wifi RAM
 256KB Firmware FLASH (512KB in iQue variant, with chinese charset)
 36KB BIOS ROM (4K NDS9, 16K NDS7, 16K GBA) |
| --- |

**Video**

```

| 2x LCD screens (each 256x192 pixel, 3 inch, 18bit color depth, backlight)
 2x 2D video engines (extended variants of the GBA's video controller)
 1x 3D video engine (can be assigned to upper or lower screen)
 1x video capture (for effects, or for forwarding 3D to the 2nd 2D engine) |
| --- |

**Sound**

```

| 16 sound channels (16x PCM8/PCM16/IMA-ADPCM, 6x PSG-Wave, 2x PSG-Noise)
 2 sound capture units (for echo effects, etc.)
 Output: Two built-in stereo speakers, and headphones socket
 Input: One built-in microphone, and microphone socket |
| --- |

**Controls**

```

| Gamepad 4 Direction Keys, 8 Buttons
 Touchscreen (on lower LCD screen) |
| --- |

**Communication Ports**

```

| Wifi IEEE802.11b |
| --- |

**Specials**

```

| Built-in Real Time Clock
 Power Managment Device
 Hardware divide and square root functions
 CP15 System Control Coprocessor (cache, tcm, pu, bist, etc.) |
| --- |

**External Memory**

```

| NDS Slot (for NDS games) (encrypted 8bit data bus, and serial 1bit bus)
 GBA Slot (for NDS expansions, or for GBA games) (but not for DMG/CGB games) |
| --- |

**Manufactured Cartridges**

```

| ROM: 16MB, 32MB, or 64MB
 EEPROM/FLASH/FRAM: 0.5KB, 8KB, 64KB, 256KB, or 512KB |
| --- |

**Can be booted from**

```

| NDS Cartridge (NDS mode)
 Firmware FLASH (NDS mode) (eg. by patching firmware via ds-xboo cable)
 Wifi (NDS mode)
 GBA Cartridge (GBA mode) (without DMG/CGB support) (without SIO support) |
| --- |

**Power Supply**

```

| Built-in rechargeable Lithium ion battery, 3.7V 1000mAh (DS-Lite)
 External Supply: 5.2V DC |
| --- |

**NDS-Lite**

Slightly smaller than the original NDS, coming in a more decently elegant case.
The LCDs are much more colorful (and thus not backwards compatible with any
older NDS or GBA games), and the LCDs support wider viewing angles. Slightly
different power managment device (with selectable backlight brightness, new
external power source flag, lost audio amplifier mute flag). Slightly different
Wifi controller (different chip ID, different dirt effects when accessing
invalid wifi ports and unused wifi memory regions, different behaviour on
GAPDISP registers, RF/BB chips replaced by a single chip). Slightly different
touch screen controller (with new unused input, and slightly different
powerdown bits).

**Notice**

NDS9 means the ARM9 processor and its memory and I/O ports in NDS mode

NDS7 means the ARM7 processor and its memory and I/O ports in NDS mode

GBA means the ARM7 processor and its memory and I/O ports in GBA mode

**The two Processors**

Most game code is usually executed on the ARM9 processor (in fact, Nintendo
reportedly doesn't allow developers use the ARM7 processor, except by
predefined API functions, anyways, even with the most likely inefficient API
code, most of the ARM7's 33MHz horsepower is left unused).

The ARM9's 66MHz "horsepower" is a different tale - it seems Nintendo thought
that a 33MHz processor would be too "slow" for 3D games, and so they (tried to)
badge an additional CPU to the original GBA hardware.

However, the real 66MHz can be used only with cache and tcm, all other memory
and I/O accesses are delayed to the 33MHz bus clock, that'd be still quite
fast, but, there seems to be a hardware glitch that adds 3 waitcycles to all
nonsequential accesses at the NDS9 side, which effectively drops its bus clock
to about 8MHz, making it ways slower than the 33MHz NDS7 processor, it's even
slower than the original 16MHz GBA processor.

Altogether, with the bugged 66MHz, and the unused 33MHz, Nintendo could have
reached almost the same power when staying with the GBA's 16MHz processor :-)

Although, when properly using cache/tcm, then the 66MHz processor <can>
be very fast, still, the NDS should have worked as well with a single
processor, though using only an ARM9 might cause a lot of compatibility
problems with GBA games, so there's at least one reason for keeping the ARM7
included.