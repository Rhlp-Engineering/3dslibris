# Gpuinternalregisteroverview

> Source: https://problemkaputt.de/gbatek.htm
> Section: Gpuinternalregisteroverview

3DS GPU Internal Register Overview 
| |
| --- |

 **Aliases**
 It is possible for multiple register (sequential) IDs to correspond to the same register. This is done to leverage the consecutive writing mode for GPU commands, which makes it possible for a single command to write data to multiple sequential register IDs. For example, register IDs 02C1 through 02C8 all correspond to GPUREG_VSH_FLOATUNIFORM_DATAi so that a consecutively writing command based at 02C0 will write its first parameter to GPUREG_VSH_FLOATUNIFORM_INDEX and ever subsequent ones to GPUREG_VSH_FLOATUNIFORM_DATAi
 
 **Data Types**
 
```

| signed Signed integer
 unsigned Unsigned integer
 floatX.Y.Z Floating-point number with X sign bits, Y exponent bits,
 and Z mantissa bits
 fixedX.Y.Z Fixed-point number with X sign bits, Y integer bits,
 and Z fractional bits |
| --- |

float1.5.10 (16bit)

float1.7.12 (20bit) (uncommon, for light attentuation)

float1.7.16 (24bit)

float1.8.23 (32bit)

float1.7.24 (32bit) (uncommon, for viewport)