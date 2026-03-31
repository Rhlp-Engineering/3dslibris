# Soundandmicrophone

> Source: https://problemkaputt.de/gbatek.htm
> Section: Soundandmicrophone

3DS Sound and Microphone 
| |
| --- |

 **3DS Sound File Formats**
 3DS Files - Sound Archive (CSAR Format)
 3DS Files - Sound SEQ (CSEQ Format)
 3DS Files - Sound WSD (CWSD Format)
 3DS Files - Sound Bank (CBNK Format)
 3DS Files - Sound Wave Archive (CWAR Format)
 3DS Files - Sound Wave Data (CWAV Format)
 3DS Files - Sound Wave Streams (CSTM Format)
 3DS Files - Sound Group (CGRP Format)
 3DS Files - Sound IDs (CSID Format)
 Note: The sound files resemble the older SDAT format used on NDS/DSi (many of the unknown entries are probably similar to those older files).
 Additionally, .aac files are probably AAC audio. And .cdc files contain DSP program code.
 3DS Files - Sound DSP Binary (DSP1 aka .cdc)
 
 
```

| ________________________________ DSP Registers _______________________________ |
| --- |

**10203000h Teak DSP Registers (ARM11 only)**

These registers are disabled by default (always zero), use CFG11_DSP_CNT (Port
10141230h) to enable them.

```

| 10203000h 2 DSP_PDATA
 10203004h 2 DSP_PADR
 10203008h 2 DSP_PCFG
 1020300Ch 2 DSP_PSTS
 10203010h 2 DSP_PSEM
 10203014h 2 DSP_PMASK
 10203018h 2 DSP_PCLEAR
 1020301Ch 2 DSP_SEM
 10203020h 2 DSP_CMD0
 10203024h 2 DSP_REP0
 10203028h 2 DSP_CMD1
 1020302Ch 2 DSP_REP1
 10203030h 2 DSP_CMD2
 10203034h 2 DSP_REP2
 10203040h FC0h Mirrors of above 40h-byte area
 10203200h 40h Reportedly "LCD", but actually it's just one of above mirrors |
| --- |

These are same as on DSi.

DSi XpertTeak (DSP)

3DS software is commonly using the DSP to play music recordings in AAC format,
and reportedly there is something called DSP-ADPCM (maybe something similar to
IMA-ADPCM)?

```

| _______________________________ CSND Registers _______________________________ |
| --- |

The sound hardware works very similar to NDS sound, see there for details about
PCM, IMA-ADPCM and PSG formats.

DS Sound

Differences are twice as many sound channels, some moved control bits, and
simplified volume control (without sound/panning/master volumes).

Unknown if/where BIAS exists on 3DS?

**10103000h - Sound Control (R/W)**

```

| 0-15 CSND Master Volume (0..8000h) (8001h..FFFFh=replaced by 8000h) (R/W)
 16 Disable sound output (0=Enable, 1=Mute) (R/W)
 17-29 Unused? (0)
 30 Dissonant sound?? (0=Dissonant, 1=Normal) (R/W)
 31 Makes some register/bits R/W (0=No, 1=Yes) (R/W) |
| --- |

When bit31=1:

```

| 10103400h+(N*20h).bit15 becomes R/W
 1010340Ch+(N*20h).bit31-2 becomes R/W
 10103800h+(N*10h).bit15 becomes R/W
 1010380Ch+(N*10h).bit31-0 becomes R/W |
| --- |

```

| ___________________________ CSND Channel Registers ___________________________ |
| --- |

**10103400h+(N*20h) - Sound Channel 0-31: Control (parts W and R/W)**

```

| 0-2 Wave Duty (0..7) ;HIGH=(N+1)*12.5%, LOW=(7-N)*12.5% (PSG only) (R/W)
 3-5 Unused (0)
 6 Linear interpolation on each two samples (0=Disable, 1=Enable) (R/W)
 7 Hold (0=Normal, 1=Hold last sample after one-shot sound) (R/W)
 8-9 Unused (0)
 10-11 Repeat (0=Manual, 1=Loop Infinite, 2=One-shot, 3=Same as 1?) (R/W)
 12-13 Format (0=PCM8, 1=PCM16, 2=IMA-ADPCM, 3=PSG/Noise) (R/W)
 14 Pause DMA or so? (0=Pause?, 1=Normal) ;no effect on PSG?! (R/W)
 15 Start/Status (0=Stop, 1=Start/Busy) ;need 10103000h.bit31 (R/W)
 16-31 Sample Rate (0..FFBEh; 0=Slowest, FFBEh=Fastest) (FFBFh..=Hangs) (W) |
| --- |

PSG/Noise: Rectangular waves are supported on channel 8-13, noise on channel
14-15. Rectangular wave Duty is same as for NDS (but 32 samples long instead of
8 samples).

Sample rate is reportedly 4x higher than NDS:

```

| value = 67.027964MHz / samplerate
 uh, that value is NOT negative??? apparently wrong.
 XXX but what is NOISE frequency? |
| --- |

**10103404h+(N*20h) - Sound Channel 0-31: Output Volume (R/W)**

```

| 0-15 Volume Right (0..8000h) ;\writing values 8001h..FFFFh is
 16-31 Volume Left (0..8000h) ;/automatically changed to 8000h |
| --- |

**10103408h+(N*20h) - Sound Channel 0-31: Capture Volume (R/W)**

```

| 0-15 Capture 0 volume (right?) (0..8000h) ;\writing 8001h..FFFFh gets
 16-31 Capture 1 volume (left?) (0..8000h) ;/automatically changed to 8000h |
| --- |

**1010340Ch+(N*20h) - Sound Channel 0-31: Start Address (R/W) (Bit0-1: W)**

**10103414h+(N*20h) - Sound Channel 0-31: Loop Restart Address (W)**

```

| 0-31 Physical Memory Address (unused for PSG/noise) |
| --- |

The R/W-ability is a mess: Start.bit2-31 are R/W (if enabled in
10103000h.bit31). Start.bit0-1 do also exist, but are write-only.
Restart.bit0-31 are write-only.

**10103410h+(N*20h) - Sound Channel 0-31: Total Size (W)**

```

| 0-26 Size in bytes (0..7FFFFFFh) (unused in manual mode)
 27-31 Unknown/Unused (0) |
| --- |

The size value is for the total size from Start to End (or Loop End if looped).
The total size (and size of looped part) semm to be required to be at least
10h. Hardware behaves quirky if the size of the looped part
(total-(restart-start)) is odd and/or(?) less than 10h. And, the quirkyness can
differ depending on whether CNT.bit10-11 is 1 or 3 (which is otherwise same).

**10103418h+(N*20h) - Sound Channel 0-31: Start IMA-ADPCM state (W)**

```

| 0-15 Initial PCM16 Value (Pcm16bit = -7FFFh..+7FFF) (not -8000h)
 16-22 Initial Table Index Value (Index = 0..88)
 23 Uh, reportedly MSB of above 7bit value ???
 .. Unknown/Unused (0) |
| --- |

Equivalent to the IMA-ADPCM header from NDS samples (but, unlike as NDS, the
header must be manually written here; instead being located in the first four
sample bytes).

**1010341Ch+(N*20h) - Sound Channel 0-31: Loop Restart IMA-ADPCM state (W)**

```

| 0-15 Restart PCM16 Value (Pcm16bit = -7FFFh..+7FFF) (not -8000h)
 16-22 Restart Table Index Value (Index = 0..88)
 23 Uh, reportedly MSB of above 7bit value ???
 .. Unknown/Unused (0)
 31 Reportedly ADPCM state reload at Loop Restart (1=Enable) |
| --- |

Allows to force the ADPCM state to the correct (pre-calculated) value upon
looping (or, with bit31=0, samples could drift if the last sample doesn't end
with exact same state as the first loop sample)...?

Uh, that's both weird... on NDS, the hardware did simply latch the correct
value automatically... is that really left unsupported on 3DS?

```

| ___________________________ CSND Capture Registers ___________________________ |
| --- |

These are probably for left/right channel...

Maybe Capture 0 Left, and Capture 1 right?

**10103800h+(N*10h) - Sound Capture 0-1: Control (R/W)**

```

| 0 Capture Repeat (0=Loop, 1=One-shot)
 1 Capture Format (0=PCM16, 1=PCM8)
 2 Unknown ...? maybe something similar as on NDS
 2-14 Unused (0)
 15 Capture Start/Status (0=Stop, 1=Start/Busy) ;need 10103000h.bit31
 16-31 Unused (0) |
| --- |

There seems to be some extra delay at begin or end of capture: For one-shot,
the bit15=1 duration should depend on rate and length (but takes a bit longer
than it should).

**10103804h+(N*10h) - Sound Capture 0-1: Sample Rate (W)**

```

| 0-15 Sample Rate (0..FFBEh; 0=Slowest, FFBEh=Fastest) (FFBFh..FFFFh=Hangs)
 16-31 Unknown/unused (0) |
| --- |

**1010380Ch+(N*10h) - Sound Capture 0-1: Length (W)**

```

| 0-23 Length (in bytes) (1..FFFFFFh) (and oddly: 0=Same as 4)
 24-31 Unknown/unused (0) |
| --- |

**1010380Ch+(N*10h) - Sound Capture 0-1: Address (R/W)**

```

| 0-31 Address (in bytes) <--- This is R/W only if enabled in 10103000h.bit31 |
| --- |

Capture works even if address and size are unaligned (even so for PCM16).

```

| _______________________________ CODEC Registers ______________________________ |
| --- |

**10145000h - CODEC_SNDEXCNT (R/W)**

```

| 0-5 DSP sound volume (00h..3Fh) (20h=max?)
 6-11 GBA sound volume (00h..3Fh) (20h=max?)
 12 Enable Microphone timing
 13 Sound/Microphone I2S Clock Frequency (0=32.73 kHz, 1=47.61 kHz)
 14 Unknown...? (maybe Mute, alike DSi's SNDEXCNT)
 15 Enable Microphone and Teak BTDMP timing and GBA sound
 16-28 Unused (0)
 29 Unknown...? (R/W)
 30 Audio Clock TP152 (Speaker PWM?) (0=3DS/8MHz, 1=GBA/16MHz)
 31 Sound/Microphone I2S Clock Enable (TP149/TP150) |
| --- |

Bit12-14 and bit28-30 are write-able only if bit31 was zero before the write.

Bit30 with wrong setting causes dissonant sound (it should be 0=3DS, 1=GBA).

Setting this register while sound was playing can produce a high-pitch fieeep.

Microphone requires bit31 set, and also one or both of bit15/bit12 set.

Teak BTDMP timing requires bit15 set (and is also affected by bit13).

GBA sound requires bit15 set, and nonzero volume in bit6-11.

Maybe some further bits here resemble DSi's SNDEXCNT...?

See also: [https://www.3dbrew.org/wiki/I2S_Registers](https://www.3dbrew.org/wiki/I2S_Registers)

```

| ________________________________ MIC Registers _______________________________ |
| --- |

**MIC Registers**

```

| 10162000h 2 REG_MIC_CNT Microphone Control
 10162004h 4 REG_MIC_DATA Microphone Data |
| --- |

These are about same as for DSi.

DSi Microphone and SoundExt

The microphone must be unmuted in touchscreen TSC registers.

The microphone must be enabled in Port 10145000h.

Microphone triggers CDMA 00h (requires Port 1014010Ch, CFG11_CDMA_CNT.bit0=1).