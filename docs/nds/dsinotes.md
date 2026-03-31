# Dsinotes

> Source: https://problemkaputt.de/gbatek.htm
> Section: Dsinotes

DSi Notes 
| |
| --- |

 **DSi Detection**
 Cartridges are using the same executable for NDS and DSi mode, the executable must thus detect whether it is running on a NDS console or DSi console. At ARM9 this is usually done as follows:
 
```

| if ([4004000h] AND 03h)=01h then DSi_mode else NDS_mode |
| --- |

On ARM7 side, the executables are attempting to do the same thing, but they are
(maybe accidently) skipping the detection depending on a 2nd I/O port:

```

| ;Caution: Below detection won't work with DSi exploits (because they are
 ; usually having the ARM7 SCFG registers disabled - it would be thus better
 ; to do the dection only on ARM9 side as described above, and then forward
 ; the result to ARM7 side).
 if ([4004008h] AND 80000000h)=0 then skip_detection_and_assume_NDS_mode
 else if ([4004000h] AND 03h)=01h then DSi_mode else NDS_mode |
| --- |

In DSi_mode, the game can use additional hardware features, and it must be also
aware of some changed BIOS SWI functions.

In NDS_mode, the game can use only old hardware features, in case of
DSi-exclusive games: The cartridge must contain a small NDS function that
displays a message saying that the game will work only on DSi consoles.

**DSi Executables**

The boot executables & entrypoints are always defined in the ARM9/ARM7
entries in cartridge header, regardless of whether the game is running on a NDS
console or DSi console. The ARM9/ARM7 executables are thus restricted to max
2MByte size (for not violating the NDS memory limit). The ARM9i/ARM7i entries
are allowing to load additional code or data, presumably to addresses (almost)
anywhere within the DSi's 16MByte memory space. Of course, if you want to use
separate executables for NDS and DSi mode, then you can put some small
bootstrap loader into the ARM9/ARM7 area, which will then load the actual main
executables.

**DSi Official Games**

There are only a few DSi-Exclusive games, and quite a lot of DSi-Enhanced
games. The package of that games doesn't seem bear any DSi-logos, so it's hard
to tell if a game contains DSi features (except via inofficial databases).

**DSi Homebrew Games**

These are extremly rare, hard to find, and practically non-existant yet. Most
webpages are mis-offering NDS games as "homebrew DSi games" (which, well, they
may work on DSi, but only in NDS mode).

There are a few "real" DSi homebrew games/emus/demos: Atari 5200 EMU, Atari
7800 EMU, CQuake, DSx86, GBA Emulator, Project Legends DS, Sandbox Engine,
StellaDS, Zoomx3 - most or all of them seem to require weird DSi exploits
(probably CycloDS iEvolution flashcarts), the games appear to be completely
lacking DSi cartridge headers, without even setting the DSi flag in
cartheader[012h].bit1 (and instead they do contain pre-historic NDS-passme
headers for booting from GBA slot, which is definetely incompatible with real
DSi consoles).