# Filesvideolayoutanimationclanflan

> Source: https://problemkaputt.de/gbatek.htm
> Section: Filesvideolayoutanimationclanflan

3DS Files - Video Layout Animation (CLAN/FLAN) 
| |
| --- |

 
```

| This article is a stub. You can help 3DBrew by expanding it. |
| --- |

CLAN (.bclan) is the layout animation format used on the 3DS. It stands for
(Binary) CTR Layout Animation, and is similar to the RLAN format used on the
Wii.

FLAN is something similar, used in 3DS eShop applet (RomFS:\layout\) and 3DS
Browser applet (RomFS:\lyt\).

**Header**

```

| 00h 4 String ID ("CLAN") ;or "FLAN"
 04h 2 UInt16 Byte Order (FEFFh)
 06h 2 UInt16 Header Size (14h)
 08h 4 UInt32 Revision
 0Ch 4 UInt32 File Size
 10h 4 UInt32 Nr Sections |
| --- |

**pat1 (Pattern 1)**

```

| 00h 4 String Signature (pat1)
 04h 4 UInt32 Section Size
 08h 4 UInt32 Flags?
 0Ch 4 UInt32 ?
 10h 4 Int32 ?
 14h 4 UInt32 ?
 18h 10h String Pattern name |
| --- |

**pai1 (Pattern Instruction? 1)**

```

| 00h 4 String Signature (pai1)
 04h 4 UInt32 Section Size
 08h 4 UInt32 ?
 0Ch 4 UInt32 Flags?
 10h 4 UInt32 Nr entries?
 14h 4 UInt32 Entries offset (relative to pai1) |
| --- |

**Instruction? entry(ies)?**

```

| 00h 14h String Target panel name
 14h 4 UInt32 Flags?
 18h 4 UInt32 Animation entry offset (relative to entry)
 1Ch 4 UInt32 ? (if flag bit 1 is set) |
| --- |

**Animation entry**

```

| 00h 4 String Animation type
 Name Description
 CLPA CTR Layout Pane SRT (Scale/Rotate/Translate) animation.
 CLTS CTR Layout Texture SRT animation.
 CLVI CTR Layout Visibility animation.
 CLVC CTR Layout Vertex Color animation.
 CLMC CTR Layout Material Color animation.
 CLTP CTR Layout Texture Pattern animation.
 04h 1 UInt8 Nr of animations
 05h 3 UInt8[3] Padding
 ? ? ? ? |
| --- |

**See also**

```
[http://wiki.tockdom.com/wiki/BRLAN](http://wiki.tockdom.com/wiki/BRLAN)[http://wiibrew.org/wiki/Wii_Animations#Animations_.28.2A.brlan.29](http://wiibrew.org/wiki/Wii_Animations#Animations_.28.2A.brlan.29)
| |
| --- |