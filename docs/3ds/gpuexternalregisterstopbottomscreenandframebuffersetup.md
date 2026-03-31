# Gpuexternalregisterstopbottomscreenandframebuffersetup

> Source: https://problemkaputt.de/gbatek.htm
> Section: Gpuexternalregisterstopbottomscreenandframebuffersetup

3DS GPU External Registers - Top/Bottom Screen and Framebuffer Setup 
| |
| --- |

 104004xxh = Framebuffer Setup "PDC0" (top screen)
 104005xxh = Framebuffer Setup "PDC1" (bottom screen)
 PDC = "PICA Display Control" or so?
 
 **Top/Bottom Screen Setup Register Summary**
 
```

| 10400400h/10400500h 4 PDCx_V_TOTAL ;1C2h ;\
 10400404h/10400504h 4 PDCx_V_LO_BORDER_END ;0D1h ;
 10400408h/10400508h 4 PDCx_V_UP_BORDER_MID ;1C1h ;
 1040040Ch/1040050Ch 4 PDCx_V_UP_BORDER_END ;1C1h ; V
 10400410h/10400510h 4 PDCx_V_SYNC_START ;000h/0CDh ;
 10400414h/10400514h 4 PDCx_V_SYNC_END ;0CFh ;
 10400418h/10400518h 4 PDCx_V_LO_BORDER_START ;0D1h ;
 1040041Ch/1040051Ch 2 PDCx_V_IRQ_START ;1C1h ;
 1040041Eh/1040051Eh 2 PDCx_V_IRQ_END ;1C5h ;/
 10400420h/10400520h 2 PDCx_V_FETCH_START ;000h ;\V
 10400422h/10400522h 2 PDCx_V_UNKNOWN_22H ;001h ;/
 10400424h/10400524h 4 PDCx_H_TOTAL ;19Dh ;\
 10400428h/10400528h 4 PDCx_H_LT_BORDER_END ;002h/052h ;
 1040042Ch/1040052Ch 4 PDCx_H_RT_BORDER_MID ;192h ;
 10400430h/10400530h 4 PDCx_H_RT_BORDER_END ;192h ; H
 10400434h/10400534h 4 PDCx_H_SYNC_START ;192h/04Fh ;
 10400438h/10400538h 4 PDCx_H_SYNC_END ;001h/050h ;
 1040043Ch/1040053Ch 4 PDCx_H_LT_BORDER_START ;002h/052h ;
 10400440h/10400540h 2 PDCx_H_IRQ_START ;192h ;
 10400442h/10400542h 2 PDCx_H_IRQ_END ;193h ;/
 10400444h/10400544h 4 PDCx_V_INCREMENT_H ;000h ;-V
 10400448h/10400548h 4 PDCx_SYNC_POLARITY
 1040044Ch/1040054Ch 4 PDCx_BORDER_COLOR
 10400450h/10400550h 4 PDCx_V_CURR_POS (R)
 10400454h/10400554h 4 PDCx_H_CURR_POS (R)
 10400458h/10400558h 04h Unused (0)
 1040045Ch/1040055Ch 2 PDCx_V_PICT_SIZE ;0F0h ;-V
 1040045Eh/1040055Eh 2 PDCx_H_PICT_SIZE ;190h/140h ;-H
 10400460h/10400560h 2 PDCx_V_LO_BORDER_MID ;0D1h ;\V
 10400462h/10400562h 2 PDCx_V_UP_BORDER_START ;1C1h ;/
 10400464h/10400564h 2 PDCx_H_LT_BORDER_MID ;002h/052h ;\H
 10400466h/10400566h 2 PDCx_H_RT_BORDER_START ;192h ;/
 10400468h/10400568h 4 PDCx_FRAMEBUF_0_LT ;\buffer address for
 1040046Ch/1040056Ch 4 PDCx_FRAMEBUF_1_LT ;/Left eye (or both eyes)
 10400470h/10400570h 4 PDCx_FRAMEBUF_FORMAT
 10400474h/10400574h 4 PDCx_INTERRUPT_TYPE
 10400478h/10400578h 4 PDCx_FRAMEBUF_SEL_ACK
 1040047Ch/1040057Ch 4 PDCx_STATUS (R)
 10400480h/10400580h 4 PDCx_COLOR_RAMP_INDEX ;\color lookup table
 10400484h/10400584h 4 PDCx_COLOR_RAMP_DATA ;/
 10400488h/10400588h 08h Unused (0)
 10400490h/10400590h 4 PDCx_FRAMEBUF_H_STEP ;-horizontal address step
 10400494h/10400594h 4 PDCx_FRAMEBUF_0_RT ;\buffer address for
 10400498h/10400598h 4 PDCx_FRAMEBUF_1_RT ;/Right eye (or unused)
 1040049Ch/1040059Ch 2 PDCx_V_LATCHING_POINT ;000h ;-V
 1040049Eh/1040059Eh 2 PDCx_H_LATCHING_POINT ;192h ;-H
 104004A0h/104005A0h 60h Unused (0) |
| --- |

Most of the registers do have internal copies (the H/V registers, and Border
color, and maybe some others), changing these registers doesn't have any effect
on the visible picture & timings - until reaching the H/V-Latching Point
(which does forward the registers to their internal copies).

```

| __________________________ Control/Status Registers __________________________ |
| --- |

**10400474h/10400574h - PDCx_INTERRUPT_TYPE - Interrupt Type (R/W)**

```

| 0 Display Enable (should be 1) (0=Off/screen fades out, 1=On/normal)
 1-7 Unused (0)
 8-10 Interrupt Mode (5=OncePerFrame, 7=None, 0..4,6=Special) (see below)
 8 reportedly disable v-irq
 9 reportedly disable h-irq
 10 reportedly disable err-irq
 7-11 Unused (0)
 16 Unknown...? (0=???, 1=Normal) (reportedly "output enable"?)
 17-31 Unused (0) |
| --- |

The default is Interrupt Mode 5 (which acts as normal framerate interrupt).

Interrupt Mode 0,1,2,3 do require manually acknowledging/requesting the next
IRQ via Request register bit18 (and bit17 in some cases).

```

| Interrupt Mode 0:
 - Triggers here or there depending on H/V settings
 Interrupt Mode 1:
 - Triggers at V=[10400544h], H=[10400540h] ;irq point
 - Triggers at V=[10400504h], H=[10400528h]..[10400566h] ;picture area
 Interrupt Mode 2:
 - Triggers here or there depending on H/V settings
 Interrupt Mode 3:
 - Triggers at V=[10400504h], H=[10400528h]..[10400566h] ;picture area
 Interrupt Mode 4 (in selected H's)
 - Triggers at V=[1040051Ch], and H=[10400542h]..[10400540h]
 Interrupt Mode 5 (framerate interrupt, usually at H=Hblank)
 - Triggers at V=[10400544h] and H=[10400540h]
 Interrupt Mode 6 (once on every H)
 - Triggers at V=[1040051Ch], and H=any
 Interrupt Mode 7 (interrupts disabled)
 - Doesn't trigger any IRQs. |
| --- |

For all IRQ H/V timings, mind that the display is drawn from left to right
(unlike normal top-to-bottom displays).

Note: Most of the H positions do actually trigger at H+1 (and perhaps similar
for V positions, though those are more difficult to measure).

**10400478h/10400578h - PDCx_FRAMEBUF_SEL_ACK - Buf Select, Req/Ack (R/W)**

```

| 0 Display Framebuffer (0=Buffer 0, 1=Buffer 1) "NEXT"
 1-3 Unused (0)
 4 Request STAT.bit4 to get triggered (0=No, 1=Request/Busy) "CUR"
 5-7 Unused (0)
 8 Unknown...? (0=Normal, 1=???) "RST_FIFO"
 9-15 Unused (0)
 16 Unknown...? (0=Normal, 1=???)
 reportedly v-irq (read=flag, write=ack?)
 17 Unknown...? (0=Normal, 1=???, set in bootrom IRQ handler, but why?)
 reportedly h-irq
 18 Acknowledge STAT.bit15? resurrect after ONESHOT irq (unless,
 bit18/ack works ONLY if OUTSIDE of display area?)
 reportedly err-irq
 19-31 Unused (0) |
| --- |

Apart from bit0, all bits in the register are kind of "Request/Busy" flags (and
get cleared once when the request is handled).

```

| bit4: cleared at H=[10400540h]..[10400542h]
 bit8,16,17,18: cleared after a handful of clock cycles |
| --- |

Request.bit4 does somehow relate to the framebuffer selection in bit0 (see
STAT.bit4 for details)... Guess: maybe bit4 prevents bit0 from being internally
applied until hblank?

**1040047Ch/1040057Ch - PDCx_STATUS - Screen Status flags (R)**

```

| 0 H-IRQ-area, when H=[10400540h]..[10400542h]
 1 V-IRQ-area, when H=[1040051Ch]..[1040051Eh]
 2-3 Unused (0)
 4 Can get set if [10400578h].bit4=1, and depending on [10400578h].bit0:
 if [10400578h].bit4=0: at H=[10400540h]..[10400528h] ;ONCE
 if [10400578h].bit4=1: at H=[10400528h]..FOREVER ;FOREVER
 that, happening at V=[10400544h] (in both of the above two cases)
 note: ONCE means that the bit goes ON-and-OFF once (and stays off)
 note: FOREVER means that the bit goes ON (and stays on)
 5-7 Unused (0)
 8 H-Sync, H=[10400534h]..[10400538h]
 9 H-Blank, H=[10400530h]..[1040053Ch] (between borders)
 10 H-Picture, H=[10400528h]..[10400566h] (between borders)
 11 Unused (0)
 12 V-Sync, H=[10400510h]..[10400514h]
 13 V-Blank, H=[1040050Ch]..[10400518h] (between borders)
 14 V-Picture, H=[10400504h]..[10400562h] (between borders)
 15 Normally 0, but can be set, if interrupts enabled IRQ request?
 bit15: set at H/V-picture start, sticky bit?
 but, WHY is bit15 is never set with bootrom default settings?
 related to 1040059Ch,1040059Eh !!!
 16-31 Unused (0) |
| --- |

**10400448h/10400548h - PDCx_SYNC_POLARITY - Screen Sync Polarity (R/W)**

```

| 0 V-Sync Polarity (0=Active Low, 1=Active High) ;Bottom Screen TP190
 1-7 Unused (0)
 8 H-Sync Polarity (0=Active Low, 1=Active High) ;Bottom Screen TP191
 9-31 Unused (0) |
| --- |

Must be 00h for top screen, and 11h for bottom screen.

```

| ___________________________ Framebuffer Registers ___________________________ |
| --- |

**10400468h/10400568h - PDCx_FRAMEBUF_0_LT - Buffer 0 for Left/both eyes**

**1040046Ch/1040056Ch - PDCx_FRAMEBUF_1_LT - Buffer 1 for Left/both eyes**

**10400494h/10400594h - PDCx_FRAMEBUF_0_RT - Buffer 0 for Right/unused eye**

**10400498h/10400598h - PDCx_FRAMEBUF_1_RT - Buffer 1 for Right/unused eye**

```

| 0-3 Unused (0)
 4-31 Address, in 16-byte units |
| --- |

Framebuffer address for Left eye (or both eyes), and Right eye (or unused).

**10400490h/10400590h - PDCx_FRAMEBUF_H_STEP - Horizontal Address Step**

```

| 0-3 Unused (0)
 4-31 Address step, in 16-byte units (usually 3Ch, aka 240pix*4byte/16) |
| --- |

**10400470h/10400570h - PDCx_FRAMEBUF_FORMAT - Framebuffer format (R/W)**

```

| 0-2 Framebuffer RGBA Format (0=8888, 1=8880, 2=5650, 3=5551, 4..?=4444)
 Note: The alpha in RGBA types is just skipped/ignored.
 3 Unused (0)
 4-5 Horizontal Zoom (0=Normal/HiRes, 1=Double/LowRes, 2/3=LeftRightBuffer)
 6 Vertical Zoom (0=Normal/HiRes, 1=Double/LowRes)
 7 Unused (0)
 8-9 Unknown (0..3) (should be 0)
 00h = Normal (should be normally used)
 01h = Unknown "get rid of rainbow strip on top of screen" uh? mess!
 02h = Unknown (actually distorted display?)
 03h = Unknown "black screen" (actually normal display?)
 10-15 Unused (0)
 16-31 Unknown (0..FFFFh) (should be 8) -- Video DMA timings?
 (FFFFh=slow horizontal drawing?)
 maybe... dotclk divider (and perhaps multiplier in other bits)
 hmmm, but, very large values cause vram-misaddressing? |
| --- |

The upper 16bit(?) seem to be Video DMA timings, there are reportedly also
Video DMA burst length bits somewhere.

Obscure Color Format Notes:

Color components are laid out in reverse byte order, with the most significant
bits used first (ie. non-24bit pixels are stored as a little-endian values).
For instance, a raw data stream of two GL_RGB565_OES pixels looks like GGGBBBBB
RRRRRGGG GGGBBBBB RRRRRGGG. uh, that are TWO pixels, but which one is shown
first/bottom? uh, and that's for non-24bit, so, how does 24bit look like?

```

| ______________________________ Color Registers ______________________________ |
| --- |

**10400480h/10400580h - PDCx_COLOR_RAMP_INDEX - Color Table Index (R/W)**

```

| 0-7 Index ;reportedly write-only (uh, but actually, it is R/W)
 8-31 Unused (0) |
| --- |

**10400484h/10400584h - PDCx_COLOR_RAMP_DATA - Color Table data (R/W)**

```

| 0-7 Red[index] ;\
 8-15 Green[index]; is that always RGB, or could it be swapped to BGR or so?
 16-23 Blue[index] ;/
 24-31 Unused (0) |
| --- |

Contains the value of the color lookup table indexed by the above register.
Accessing this register will increase the index register by one.

For RGBA 8888/8880, the table should be usually filled with linear increasing
values: 000000h, 010101h, 020202h, 030303h, .., FFFFFFh. Or alternately with
non-linear values if the displays require 'gamma' adjustments.

Unknown which indices are used in RGBA 5650, 5551, and 4444 modes.

**1040044Ch/1040054Ch - PDCx_BORDER_COLOR - Screen Border color (R/W)**

```

| 0-7 Red (00h..FFh)
 8-15 Green (00h..FFh)
 16-23 Blue (00h..FFh)
 24-31 Unused (0) |
| --- |

The screen border isn't used for fullscreen pictures, but it could be used if
the picture is smaller than the actual screen. To disable the border, set
border start/end registers to the same value.

There are also "border middle" registers (with unknown purpose and without
visible effect, except, the middle values for vertical border MUST be within
border start/end range, otherwise the screen dies with fading out).

```

| _______________________________ H/V-Registers _______________________________ |
| --- |

**Vertical Position Registers**

```

| 10400444h/10400544h 4 PDCx_V_INCREMENT_H ;000h ;\
 1040049Ch/1040059Ch 2 PDCx_V_LATCHING_POINT ;000h ; Misc
 10400420h/10400520h 2 PDCx_V_FETCH_START ;000h ;
 10400422h/10400522h 2 PDCx_V_UNKNOWN_22H ;001h ;/
 10400410h/10400510h 4 PDCx_V_SYNC_START ;000h/0CDh ;\Sync
 10400414h/10400514h 4 PDCx_V_SYNC_END ;0CFh ;/
 10400460h/10400560h 2 PDCx_V_LO_BORDER_MID ;0D1h ;\
 10400418h/10400518h 4 PDCx_V_LO_BORDER_START ;0D1h ; Lower border
 10400404h/10400504h 4 PDCx_V_LO_BORDER_END ;0D1h ;/
 1040045Ch/1040055Ch 2 PDCx_V_PICT_SIZE ;0F0h ;-Picture size
 10400462h/10400562h 2 PDCx_V_UP_BORDER_START ;1C1h ;\
 10400408h/10400508h 4 PDCx_V_UP_BORDER_MID ;1C1h ; Upper border
 1040040Ch/1040050Ch 4 PDCx_V_UP_BORDER_END ;1C1h ;/
 1040041Ch/1040051Ch 2 PDCx_V_IRQ_START ;1C1h ;\Irq
 1040041Eh/1040051Eh 2 PDCx_V_IRQ_END ;1C5h ;/
 10400400h/10400500h 4 PDCx_V_TOTAL ;1C2h ;-Total-1
 10400450h/10400550h 4 PDCx_V_CURR_POS (R) ;000h..1C2h ;-Curr Pos |
| --- |

These are usually using same values for top/bottom screen (with 240pix height
each), except that Nintendo has used different Sync length for some reason.

**Horizontal Position Registers**

```

| 10400434h/10400534h 4 PDCx_H_SYNC_START ;192h/04Fh ;\Sync
 10400438h/10400538h 4 PDCx_H_SYNC_END ;001h/050h ;/
 1040043Ch/1040053Ch 4 PDCx_H_LT_BORDER_START ;002h/052h ;\
 10400464h/10400564h 2 PDCx_H_LT_BORDER_MID ;002h/052h ; Left border
 10400428h/10400528h 4 PDCx_H_LT_BORDER_END ;002h/052h ;/
 1040045Eh/1040055Eh 2 PDCx_H_PICT_SIZE ;190h/140h ;-Picture size
 10400466h/10400566h 2 PDCx_H_RT_BORDER_START ;192h ;\
 1040042Ch/1040052Ch 4 PDCx_H_RT_BORDER_MID ;192h ; Right border
 10400430h/10400530h 4 PDCx_H_RT_BORDER_END ;192h ;/
 1040049Eh/1040059Eh 2 PDCx_H_LATCHING_POINT ;192h ;-Misc
 10400440h/10400540h 2 PDCx_H_IRQ_START ;192h ;\Irq
 10400442h/10400542h 2 PDCx_H_IRQ_END ;193h ;/
 10400424h/10400524h 4 PDCx_H_TOTAL ;19Dh ;-Total-1
 10400454h/10400554h 4 PDCx_H_CURR_POS (R) ;000h..19Dh ;-Curr Pos |
| --- |

These are using the same Htotal for top/bottom screen, but different
picture/blanking widths (for top/bottom screens with 400pix/320pix width). The
top screen does also support a 800pix hires mode (which should use double
Htotal, and adjust other registers accordingly).

Note: The values in the H/V registers are 12bit wide, 0..FFFh (the upper
4bit/20bit of the 16bit/32bit registers are unused, always zero).

Wrong H/V values may cause various effects - which may differ depending on the
display type. New3DS bottom screen tends to output a stable image (with fading
upper/right areas if it is too small), or no picture at all (with whole screen
fading). However, other screens are said to lose sync or misalign lines and
such stuff.

Setting V_FETCH_START bigger than V_LO_BORDER_END causes the bottom-most
picture lines to display the most recent pixel color instead of fetching new
pixels from VRAM. A Similar effect occurs in upper picture area when setting
V_PICT_SIZE too small (whilst H_PICT_SIZE seems to have no effect at all).

```

| _________________________________ Misc Notes _________________________________ |
| --- |

**Framebuffers**

These LCD framebuffers normally contain the last rendered frames from the GPU.
The framebuffers are drawn from left-to-right, instead of top-to-bottom. Thus
the beginning of the framebuffer is drawn starting at the left side of the
screen.

Both of the 3D screen left/right framebuffers are displayed regardless of the
3D slider's state, however when the 3D slider is set to "off" the 3D effect is
disabled. Normally when the 3D slider's state is set to "off" the left/right
framebuffer addresses are set to the same physical address. When the 3D effect
is disabled and the left/right framebuffers are set to separate addresses, the
LCD seems to alternate between displaying the left/right framebuffer each
frame.

Uh, it can alternate left/right buffers each frame? I have NEVER seen that
effect when testing register settings (and it would have caused massive flicker
because I didn't had initialized the right buffer). What it can do is alternate
left/right buffer after each pixel horizontally (including on bottom screen).

**Further Registers**

Apart from above GPU registers, the screens do also need initialization of the
"LCD" registers,

3DS Video LCD Registers

plus, initialization of I2C registers for each screen. Plus some MCU/I2C
registers for backlight or so.