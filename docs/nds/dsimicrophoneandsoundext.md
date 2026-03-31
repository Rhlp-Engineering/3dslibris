# Dsimicrophoneandsoundext

> Source: https://problemkaputt.de/gbatek.htm
> Section: Dsimicrophoneandsoundext

DSi Microphone and SoundExt 
| |
| --- |

 **4004600h - DSi7 - MIC_CNT - Microphone Control (can be 0000E10Fh)**
 
```

| 0-1 Data Format (0=MakeStereo, 1=SameAsNormal?, 2=Normal, 3=None) (R/W)
 2-3 Sampling Rate (0..3=F/1, F/2, F/3, F/4) (R/W)
 4-7 Unused (0) (-)
 8 FIFO Empty (0=No, 1=Empty) ;0 words (R)
 9 FIFO Half-Full (0=No, 1=Half-Full) ;8 or more words (R)
 10 FIFO Full (0=No, 1=Full) ;16 words (R)
 11 FIFO Overrun (0=No, 1=Overrun/Stopped) ;more than 16 words (R)
 12 Clear FIFO (0=No change, 1=Clear) ;works only if bit15 was 0 (W)
 13-14 IRQ Enable (0=Off, 1=Same as 3, 2=When Full, 3=When Half-Full)(R/W)
 15 Enable (0=Disable, 1=Enable) (R/W) |
| --- |

The Sampling Rate depends on the I2S frequency in SNDEXCNT.Bit13,

```

| I2S=32.73kHz --> F/1=32.73kHz, F/2=16.36kHz, F/3=10.91kHz, F/4=8.18kHz
 I2S=47.61kHz --> F/1=47.61kHz, F/2=23.81kHz, F/3=15.87kHz, F/4=11.90kHz |
| --- |

The Sampling Rate becomes zero (no data arriving) when SNDEXCNT.Bit15=0, or
when MIC_CNT.bit0-1=3, or when MIC_CNT.bit15=0, or when Overrun has occurred.

**4004604h - DSi7 - MIC_DATA - Microphone Data (R)**

The internal (and external) microphones are mono only. However, the
"MakeStereo" data format can convert "mono to stereo" (ie. each sample is
output twice, and 1st/2nd sample contain the same value; accordingly, the FIFO
is getting full twice as fast).

MIC_DATA can be read by software, or via NDMA (with startup mode 0Ch and
Blocksize 8 words).

```

| 0-15 Signed 16bit Data, 1st sample ;\16 words FIFO, aka 32 halfwords
 16-31 Signed 16bit Data, 2nd sample ;/ |
| --- |

Overrun Caution: If data isn't read fast enough then the Overrun flag will get
set, and FIFO updates will be stopped. At that point one can still read the
remaining 16 words from the FIFO, but no further words will be added to the
FIFO (until clearing the FIFO and Overrun flag via MIC_CNT.bit12; which
requires bit15=0 before setting bit12).

**Microphone Touchscreen/sound control**

DSi Touchscreen/Sound Init Flowcharts

The Sample Data tends to be 0000h or FFFFh if the microphone related
TSC-registers aren't unmuted.

**4004700h - DSi7 - SNDEXCNT (R/W)**

```

| 0-3 NITRO/DSP ratio (valid range is 0 to 8) (R/W)
 4-12 Unknown/Unused (0) (0?)
 13 Sound/Microphone I2S frequency (0=32.73 kHz, 1=47.61 kHz) (R or R/W)
 14 Mute status (does NOT affect mic) (?=Mute WHAT?) (R/W)
 15 Enable Microphone (and Sound Output!) (1=Enable) (R/W) |
| --- |

Bit13 is write-able only if bit15 was zero before the write.

NITRO/DSP ratio

The DSP can generate sound output aswell, alongside the old NITRO sound mixer.
The following settings configure the ratio between DSP and NITRO mixer output:

```

| 00h DSP sound 8/8, NITRO sound 0/8 (=DSP sound only)
 01h DSP sound 7/8, NITRO sound 1/8
 02h DSP sound 6/8, NITRO sound 2/8
 03h DSP sound 5/8, NITRO sound 3/8
 04h DSP sound 4/8, NITRO sound 4/8 (=half volume for DSP and NITRO each)
 05h DSP sound 3/8, NITRO sound 5/8
 06h DSP sound 2/8, NITRO sound 6/8
 07h DSP sound 1/8, NITRO sound 7/8
 08h DSP sound 0/8, NITRO sound 8/8 (=NITRO sound only)
 09h..0Fh Reserved |
| --- |

Uh, what is that? Hopefully, a volume-ratio? Preferably, no time-ratio!