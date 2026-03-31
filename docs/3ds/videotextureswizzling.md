# Videotextureswizzling

> Source: https://problemkaputt.de/gbatek.htm
> Section: Videotextureswizzling

3DS Video Texture Swizzling 
| |
| --- |

 **Texture Swizzling**
 Morton Swizzling, or Z-order Swizzling is done by reading source pixels from a scanline based bitmap in "Z-shaped" read-direction, and then storing that pixels at continous VRAM addresses.
 In the drawing below, each "Z" represents 2x2 pixels (arranged as a "Z" shape, ie. upper-left, upper-right, lower-left, lower-right). On a larger scale, each 2x2 Z's are also forming a larger Z, and so on.
 
```

| Z/Z /Z/Z
 .-' / .-'
 Z/Z/ Z/Z
 ..--'
 .--'
 Z/Z /Z/Z
 .-' / .-'
 Z/Z/ Z/Z |
| --- |

This can improve cache hits for adjacent pixels. In a large bitmap, pixels in
adjacent scanlines are always located in separate cache entries. With the
swizzling, there is a better chance that they are in the same cache entry.

**Addressing**

The swizzling does interleave the x/y address bits. On the 3DS hardware, data
is processed in units of 8 scanlines, so the interleave occurs on lower three
x/y bits only. For example, for 256x256 pixel data:

```

| Scanline-based bitmap --> YyyyyyyyXxxxxxxx
 Swizzled texture --> YyyyyXxxxxyxyxyx |
| --- |

**Swizzling Hardware**

The 3DS has some hardware for converting scanlines to swizzled textures:

```

| Y2R Registers (YUV-to-RGBA)
 L2B Registers (RGB-to-RGBA) (New3DS only)
 LGYFB Registers (GBA/NDS/DSi-to-3DS-Framebuffer) |
| --- |

**Swizzling Examples**

Examples for scanline pixels before/after swizzling:

```

| Linear scanlines, 16x8 pixels: Linear lines, 8x8 pixels:
 00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F 00 01 02 03 04 05 06 07
 10 11 12 13 14 15 16 17 18 19 1A 1B 1C 1D 1E 1F 08 09 0A 0B 0C 0D 0E 0F
 20 21 22 23 24 25 26 27 28 29 2A 2B 2C 2D 2E 2F 10 11 12 13 14 15 16 17
 30 31 32 33 34 35 36 37 38 39 3A 3B 3C 3D 3E 3F 18 19 1A 1B 1C 1D 1E 1F
 40 41 42 43 44 45 46 47 48 49 4A 4B 4C 4D 4E 4F 20 21 22 23 24 25 26 27
 50 51 52 53 54 55 56 57 58 59 5A 5B 5C 5D 5E 5F 28 29 2A 2B 2C 2D 2E 2F
 60 61 62 63 64 65 66 67 68 69 6A 6B 6C 6D 6E 6F 30 31 32 33 34 35 36 37
 70 71 72 73 74 75 76 77 78 79 7A 7B 7C 7D 7E 7F 38 39 3A 3B 3C 3D 3E 3F
 Swizzled texture, 2 tiles: Swizzled texture, 1 tile:
 00 01 10 11 02 03 12 13 ;\ 00 01 08 09 02 03 0A 0B
 20 21 30 31 22 23 32 33 ; 10 11 18 19 12 13 1A 1B
 04 05 14 15 06 07 16 17 ; 04 05 0C 0D 06 07 0E 0F
 24 25 34 35 26 27 36 37 ; left tile 14 15 1C 1D 16 17 1E 1F
 40 41 50 51 42 43 52 53 ; 20 21 28 29 22 23 2A 2B
 60 61 70 71 62 63 72 73 : 30 31 38 39 32 33 3A 3B
 44 45 54 55 46 47 56 57 ; 24 25 2C 2D 26 27 2E 2F
 64 65 74 75 66 67 76 77 ;/ 34 35 3C 3D 36 37 3E 3F
 08 09 18 19 0A 0B 1A 1B ;\
 28 29 38 39 2A 2B 3A 3B ;
 0C 0D 1C 1D 0E 0F 1E 1F ;
 2C 2D 3C 3D 2E 2F 3E 3F ; right tile
 48 49 58 59 4A 4B 5A 5B ;
 68 69 78 79 6A 6B 7A 7B :
 4C 4D 5C 5D 4E 4F 5E 5F ;
 6C 6D 7C 7D 6E 6F 7E 7F ;/ |
| --- |

Note: Z-order refers to the shape of the letter "Z", not to be confused with
Z-axis, and not to be confused with the zigzag-order that is used in JPEGs.