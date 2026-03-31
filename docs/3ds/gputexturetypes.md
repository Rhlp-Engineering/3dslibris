# Gputexturetypes

> Source: https://problemkaputt.de/gbatek.htm
> Section: Gputexturetypes

3DS GPU Texture Types 
| |
| --- |

 **Mipmap**
 Mipmaps have the original texture bundled with smaller copies (with halved width/height, used for rendering more distant polygons). This may improve memory cache hits, and also improve display quality (eg. when resampling black+white pixels to gray). Mipmaps don't waste too much memory (orginal=100%, total=133%) (or in other terms, original=75%, total=100%).
 The Mipmap could be stored as separate rectangles (as done on 3DS), or within a single rectangle:
 
```

| _____________ _______________ _______ ___ _
 _| | | | | | | |_|
 |___| | | | | |___|
 | | | | | |
 |_______| | | |_______|
 | | | |
 | | | |
 | | | |
 |_______________| |_______________| |
| --- |

**Cubemap (panorama)**

A cubemap consists of six textures arranged as a cube, allowing to render 360'
panoramas (with the camera being located INSIDE of the cube).

```

| ______
 |Top |
 | |
 ______|______|______ ______
 |Left |Front |Right |Back |
 | | | | |
 |______|______|______|______|
 |Bottom|
 | |
 |______| |
| --- |