# Tscregistersummary

> Source: https://problemkaputt.de/gbatek.htm
> Section: Tscregistersummary

3DS TSC, Register Summary 
| |
| --- |

 **Page Selection**
 
```

| TSC[xxh:00h]=page ;Page (each TSC SPI bus probably has own page+index?) |
| --- |

**Page 00h-01h (DSi Registers)**

```

| TSC[00h:02h]=read ;DSi Undocumented status (reserved bits)
 TSC[00h:03h]=read ;DSi Overtemperature OT Flag (reserved bits)
 TSC[00h:0Bh]=87h ;DSi DAC NDAC Value
 TSC[00h:39h]=66h ;DSi ADC DC Measurement 1 (reset=00h, ORed with 66h)
 TSC[00h:3Fh]=D4h ;DSi DAC Data-Path Setup (reset=D4h, ORed with C0h)
 TSC[00h:40h]=00h ;DSi DAC Volume Control
 TSC[00h:41h]=FDh ;DSi DAC Left Volume Control ;\aka 3DS ;HWCAL[2E4h]
 TSC[00h:42h]=FDh ;DSi DAC Right Volume Control ;/ShutterVol0 ;HWCAL[2E4h]
 TSC[00h:51h]=00h/80h ;DSi Microphone Mute/Unmute ;ADC Digital Mic
 TSC[00h:52h]=80h/00h ;DSi Microphone Mute/Unmute ;ADC Digital Volume
 TSC[01h:2Eh]=03h ;DSi Microphone MIC BIAS
 TSC[01h:2Fh]=2Bh ;DSi Microphone MIC PGA
 TSC[01h:30h]=40h ;DSi P-Terminal ADC Channel Fine-Gain Input (reset=40h)
 TSC[01h:31h]=40h ;DSi M-Terminal ADC Input Selection (reset=40h) |
| --- |

The 3DS does usually access only the registers mentioned above (but there are
many more DSi-style registers in page 00h,01h,03h; see DSi chapter for
details).

**Page 04h-0Ch (DSi Coefficient RAM)**

```

| TSC[04h:08h-0Dh]=... ;DSi Mic Autogain ;IIR, as DSi (7Fh,E1h,80h,1Fh,7Fh,C1h)
 TSC[05h:08h-3Fh]=... ;3DS FilterMic32 ;IIR+Biquad A,B,C,D,E;HWCAL[364h-39Bh]
 TSC[05h:48h-7Fh]=... ;3DS FilterMic47 ;IIR+Biquad A,B,C,D,E;HWCAL[39Ch-3D3h]
 TSC[08h:0Ch-3Dh]=... ;3DS FilterFreeB ;Biquad ;\initialized;HWCAL[3DAh-40Bh]
 TSC[08h:4Ch-7Dh]=... ;3DS FilterFreeB' ;Biquad ; for ;HWCAL[3DAh-40Bh]
 TSC[09h:02h-07h]=... ;3DS FilterFreeA ;IIR.L ; non-GBA ;HWCAL[3D4h-3D9h]
 TSC[09h:08h-0Dh]=... ;3DS FilterFreeA' ;IIR.R ;/only ;HWCAL[3D4h-3D9h]
 TSC[0Ah:02h-07h]=... ;3DS FilterFreeA'';IIR? HP47 ;\ ;HWCAL[3D4h-3D9h]
 TSC[0Ah:0Ch-3Dh]=... ;3DS FilterFreeB'';Biquad? HP47 ;/ ;HWCAL[3DAh-40Bh]
 TSC[0Bh:02h-1Fh]=... ;3DS FilterHP32 ;Biquad.L A,B,C ;\ ;HWCAL[2ECh-309h]
 TSC[0Bh:20h-3Dh]=... ;3DS FilterHP47.L ;Biquad.L D,E,F ;/ ;HWCAL[30Ah-327h]
 TSC[0Bh:42h-5Fh]=... ;3DS FilterHP32' ;Biquad.R A,B,C ;\ ;HWCAL[2ECh-309h]
 TSC[0Bh:60h-7Dh]=... ;3DS FilterHP47.R ;Biquad.R D,E,F ;/ ;HWCAL[30Ah-327h]
 TSC[0Ch:02h-1Fh]=... ;3DS FilterSP32 ;Biquad.L A,B,C ;\ ;HWCAL[328h-345h]
 TSC[0Ch:20h-3Dh]=... ;3DS FilterSP47 ;Biquad.L D,E,F ;/ ;HWCAL[346h-363h]
 TSC[0Ch:42h-5Fh]=... ;3DS FilterSP32' ;Biquad.R A,B,C ;\ ;HWCAL[328h-345h]
 TSC[0Ch:60h-7Dh]=... ;3DS FilterSP47' ;Biquad.R D,E,F ;/ ;HWCAL[346h-363h] |
| --- |

The above coefficient RAM pages exists on DSi, too. However, the DSi is usually
initializing only those in page 04h.

Caution: The various 16bit Filter values in the HWCAL and config files are
stored in LITTLE-ENDIAN, but the TSC chip wants them in BIG-ENDIAN, so one must
swap each 2 bytes.

Unknown how the 3DS is using the extra coefficients... does it use miniDSP
instructions for that?

Unknown what the duplicated entries are for... maybe left/right channels?

**Page 64h (100 decimal) (3DS Sound/Microphone Config)**

```

| TSC[64h:01h]=01h ;3DS Software Reset (?)
 TSC[64h:22h]=18h ;3DS ? (reset=00h, ORed with 18h, later bit2=cleared)
 TSC[64h:25h]=read ;3DS status, wait for bit3,7
 TSC[64h:26h]=read ;3DS status, wait for bit3,7
 TSC[64h:2Ch] ;unused, but nonzero ;bit0,1=headphone connect status
 TSC[64h:30h] ;unused, but nonzero
 TSC[64h:31h]=00h/44h ;3DS ? (reset=00h) (GBA:00h, Other:44h)
 TSC[64h:43h]=11h/91h ;3DS set to 11h, later toggles bit=0 then bit7=1
 TSC[64h:44h] ;unused, but nonzero
 TSC[64h:45h]=20h/30h ;3DS Speaker off (reset=00h, later=20h, 30h=speakerOff)
 TSC[64h:75h] ;unused, but nonzero
 TSC[64h:76h]=14h/D4h ;3DS Lock Coefficient RAM? (reset=14h, ORed with C0h)
 TSC[64h:77h]=0Ch/00h ;3DS ? (reset=0Ch, later clear bit2,3 after coeff init)
 TSC[64h:78h]=00h ;3DS ?
 TSC[64h:7Ah]=00h ;3DS ?
 TSC[64h:7Bh]=ECh ;3DS ShutterVolume1 ;HWCAL[2E5h]
 TSC[64h:7Ch]=0Ah ;3DS ? (reset=0Ah, later clears bit0) |
| --- |

**Page 65h (101 decimal) (3DS Sound/Microphone Gains)**

```

| TSC[65h:0Ah]=0Ah ;3DS ?
 TSC[65h:0Bh]=1Ch/3Ch ;3DS ? ... depends on TSC[00h:02h..03h] ;HP
 TSC[65h:0Ch]=04h ;3DS DriverGainHP ;HWCAL[2E0h]*8+4
 TSC[65h:11h]=10h/D0h ;3DS ? (reset=00h, ORed with 10h, later ORed with C0h)
 TSC[65h:12h]=06h ;3DS DriverGainSP ;\maybe left? ;HWCAL[2E1h]*4+2
 TSC[65h:13h]=06h ;3DS DriverGainSP' ;/ right? ;HWCAL[2E1h]*4+2
 TSC[65h:16h]=00h ;3DS AnalogVolumeHP Left (0..7Eh?) ;HWCAL[2E2h]
 TSC[65h:17h]=00h ;3DS AnalogVolumeHP Right (0..7Eh?) ;HWCAL[2E2h]
 TSC[65h:1Bh]=07h ;3DS AnalogVolumeSP ;\maybe left? ;HWCAL[2E3h]
 TSC[65h:1Ch]=07h ;3DS AnalogVolumeSP' ;/ right? ;HWCAL[2E3h]
 TSC[65h:33h]=03h ;3DS MicrophoneBias ;HWCAL[2E6h]
 TSC[65h:41h]=00h+wait;3DS PGA_GAIN (mic) (bit0-5) ;HWCAL[2E8h]
 TSC[65h:42h]=02h+wait;3DS QuickCharge (what?) (bit0-1) ;HWCAL[2E7h]
 TSC[65h:47h,4Bh,4Ch,4Dh,4Eh,52h,53h] ;unused, but nonzero
 TSC[65h:77h]=94h/95h ;3DS ? (reset=94h, ORed with 01h)
 TSC[65h:78h] ;unused, but nonzero
 TSC[65h:7Ah]=01h ;3DS ? |
| --- |

Some here seem to be alike TSC[1:24h]..TSC[1:2Bh]

**Page 67h (103 decimal) (3DS Touchscreen/Circle Pad, Control)**

```

| TSC[67h:17h]=43h ;3DS AnalogSense & Precharge ;HWCAL[40Fh]+[40Eh]*10h
 TSC[67h:19h]=69h ;3DS AnalogStabilize & XP_Pullup ;HWCAL[40Dh]+[411h]*10h
 TSC[67h:1Bh]=80h ;3DS AnalogDebounce & YM_Driver ;HWCAL[410h]+[412h]*80h
 TSC[67h:24h]=98h/18h ;3DS bit7=0=touchscreen.on ;bit2=1=has new touchdata?
 TSC[67h:25h]=43h/53h ;3DS bit5-2=0100b=touchscreen.on, bit6=0=DSi mode
 TSC[67h:26h]=00h/ECh ;3DS bit7=1=touchscreen.on ;bit1=1=had old touchdata?
 TSC[67h:27h]=11h ;3DS AnalogInterval ;HWCAL[40Ch]+10h
 TSC[67h:2Bh]=read ;3DS Ack Headphone connect IRQ (always returns 00h) |
| --- |

**Page FBh (251 decimal) (3DS Touchscreen/Circle Pad, Data)**

```

| TSC[FBh:01h]=read ;3DS fifo 26x16bit; 5xTSC.x, 5xTSC.y, 8xCPAD.y, 8xCPAD.x |
| --- |

**Notes**

```

| L/R Left/Right
 HP/SP Headphone/Speaker
 32/47 I2S Clock (32.73 kHz or 47.61 kHz) |
| --- |