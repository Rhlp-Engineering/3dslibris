# Filessavedatadisaanddiffdatapairs

> Source: https://problemkaputt.de/gbatek.htm
> Section: Filessavedatadisaanddiffdatapairs

3DS Files - Savedata DISA and DIFF - Data Pairs 
| |
| --- |

 
```

| DPFS might be short for
 DataPair...
 DuPlicates
 PersistentData structure (DP backwards) |
| --- |

**Loading active blocks**

To read data from src_offset (eg. begin of SAVE filesystem at src_offset=0):

```

| load the active difi/ivfc/dpfs block
 if external_flag_from_DIFI[38h] then
 readbytes(partition_base+external_level7_base+src_offset, len)
 else
 level3_byte_addr = level7_base+src_offset
 snippet_loop:
 snippet_len = min(len, distance to level3_blocksize boundary)
 level2_bit_index = (level3_byte_addr/level3_blocksize) xor 1Fh ;\
 level2_byte_addr = (level2_bit_index/8) ;
 level1_bit_index = (level2_byte_addr/level2_blocksize) xor 1Fh ;
 level1_byte_addr = (level1_bit_index/8) ;/
 base1 = level1_base+level1_size*(active_flag_from_DIFI[039h])
 flag1 = readbyte(level1_byte_addr+base1, size=1)
 base2 = level2_base+level2_size*((flag1 shr (level1_bit_index AND 7) AND 1)
 flag2 = readbyte(level2_byte_addr+base2, size=1)
 base3 = level3_base+level3_size*((flag2 shr (level2_bit_index AND 7) AND 1)
 data = readbytes(level3_byte_addr+base3, snippet_len)
 raise src/dst, decrease len by snippet_len, if len>0 then goto snippet_loop |
| --- |

**Partition**

A partition can have two types of layout. This is determined by the field
DIFI+38h (Enable external IVFC level 7).

The layout type 0 (external IVFC level 7 disabled) contains

```

| DPFS Level 1 (contains active flags for DPFS level 2)
 DPFS Level 2 (contains active flags for DPFS level 3)
 DPFS Level 3 (contains IVFC levels and Data?)
 IVFC Level 4 (contains SHA256's for IVFC level 5)
 IVFC Level 5 (contains SHA256's for IVFC level 6)
 IVFC Level 6 (contains SHA256's for IVFC level 7)
 IVFC Level 7 (contains the actual content data) |
| --- |

The layout type 1 (external IVFC level 4 enabled) contains

```

| DPFS Level 1 ;Flags for DPFS Level 2
 DPFS Level 2 ;Flags for DPFS Level 3
 DPFS Level 3, and inside
 IVFC Level 4 ;SHA256's on IVFC Level 5
 IVFC Level 5 ;SHA256's on IVFC Level 6
 IVFC Level 6 ;SHA256's on IVFC Level 7
 IVFC Level 7 ;the actual content data, note that this is outside DPFS level 3 |
| --- |

**Summary diagram**

external link: [https://i.imgur.com/BjwShJZ.png](https://i.imgur.com/BjwShJZ.png)

Please move this into 3dbrew when file uploading is fixed.