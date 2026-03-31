# Filesmodulenwmwifidriver

> Source: https://problemkaputt.de/gbatek.htm
> Section: Filesmodulenwmwifidriver

3DS Files - Module NWM (Wifi Driver) 
| |
| --- |

 **NWM File/versions**
 The wifi driver can be found in following locations (each console should have one normal mode file, and one safe mode file):
 
```

| 3ds:\title\00040130\00002d02\content\000000vv.app ;NWM Normal mode (all 3DS)
 3ds:\title\00040130\00002d03\content\0000000v.app ;NWM Safe mode (Old3DS)
 3ds:\title\00040130\20002d03\content\0000000v.app ;NWM Safe mode (New3DS) |
| --- |

The .app file contains a compressed .code file (inside of the NCCH ExeFS). That
.code file contains ARM11 code and six datablocks with the actual Xtensa wifi
firmware (similar as in the DSi wifi firmware):

```

| Stub.data 038h bytes ;\Database and EEPROM reading stub
 Stub.code 316h bytes ; (always same size/content)
 Database 1E8h bytes ;/
 Main.type1 0FD3h, 10F7h, or 1B1Bh bytes ;\Main firmware (compressed)
 Main.type4 A053h, A482h, or A5EBh bytes ; (size/content varies per version)
 Main.type5 78F6h, or 7A2Eh bytes ;/ |
| --- |

There are a few safe mode versions, and dozen(s) of normal mode versions. The
versions may contain different ARM11 code, and/or different Xtensa code (as
shown above, there are at least three revisions of the Xtensa "Main" code
blocks (spotted in four different NWM files, so there may be much more
variations)).

**NWM Main Type1/4/5**

There are three variants of the Main firmware, called Type1/4/5 (because the
ARM11 code uses "CMP r0,1/4/5" opcodes to determine which one to use).

```
************
| Type1 = Standard functions for normal internet access
 Type1 supports the same commands/events as on DSi
 Type1 from newer NWM versions seems to hang (unknown if that can be fixed
 with extra initialization steps; otherwise better stick with an older Type1
 copy, or use Type4, which doesn't have that issue).
 Type4 = Special AP Mode and whatever:
 WMIcmd(004Ah..0052h) seem to be AP mode (alike Atheros cmd F00Bh..F013h)
 WMIcmd(0053h,0055h..005Fh) unknown
 WMIevent(101Dh,101Fh,1020h,1022h,1024h,1025h) unknown
 Type4 can be used for normal internet access (when ignoring event 1025h)
 Type4 requires heartbeat handling (or to disable it via heartbeat timeout=0)
 Type5 = Special MacFilter, GameID, and built-in SHA1 function
 Contains ascii strings "MACFilter" and "GameID", and a built-in SHA1 function
 WMIcmd(0060h..0073h) unknown
 WMIevent(1021h,1023h,1026h,1027h) unknown
 Type5 doesn't seem to support normal internet access (it can't even find APs) |
| --- |

Type4/5 are probably for Nintendo-specific special local network multiplayer
features, possibly related to "Streetpass" (in sleep mode?) and "DownloadPlay"
(for single gamepak), and/or other purposes.

**Addresses in ARM11 Pool**

The ARM11 code does reference the Xtensa datablocks with "LDR Rd,=address"
opcodes, hence having the datablock addresses stored in the literal pool. The
pool isn't exactly intended to be used as "table of contents", but it can be
misused for that purpose:

Search for constant 00524C00h, check that constant 000003EDh is located at the
expected offset, then extract the datablock start/end addresses from the
corresponding pool locations, subtract 10000h to convert addresses to .code
file offsets.

The NWM file does contain two pools: One pool with the Main type1/4/5
addresses, followed by by larger pool with the same addresses, plus addresses
for the Database and EEPROM reading stub code/data.

Most files (3 of 4 known files) are using this pool structure:

```
********
| Newer Small Pool:
 00h main.dst (and stub.data.dst) ;CONST (00524C00h)
 04h whatever value ;CONST (000003EDh)
 08h main.src.type1.end ;\Type1 (basic internet)
 0Ch main.src.type1.start ;/
 10h main.src.type4.end ;\Type4 (ApMode and whatever)
 14h main.src.type4.start ;/
 18h main.src.type5.end ;\Type5 (MacFilter,GameId,Sha1)
 1Ch main.src.type5.start ;/
 Newer Large Pool:
 00h whatever.addr.plus00h ;\
 04h whatever.addr.plus04h ; INCREASING addresses
 08h whatever.addr.plus08h ;
 0Ch whatever.addr.plus0Ch ;/
 10h whatever.other.addr
 14h main.dst (and stub.data.dst) ;CONST (00524C00h)
 18h whatever value ;CONST (000003EDh)
 1Ch main.src.type1.end ;\Type1 (basic internet)
 20h main.src.type1.start ;/
 24h main.src.type4.end ;\Type4 (ApMode and whatever)
 28h main.src.type4.start ;/
 2Ch main.src.type5.end ;\Type5 (MacFilter,GameId,Sha1)
 30h main.src.type5.start ;/
 34h database.src.end ;\Datab ;SAME as stub.code.src.start
 38h database.src.start ;/
 3Ch database.dst ;CONST (0053FE18h)
 40h stub.code.src.end ;\Stubc
 44h stub.code.src.start ;/ ;SAME as database.src.end
 48h stub.code.dst ;CONST (00527000h)
 4Ch stub.data.src.end ;\Stubd
 50h stub.data.src.start ;/
 54h whatever.thumb.code.addr1
 58h whatever.thumb.code.addr2
 5Ch whatever.thumb.code.addr3 |
| --- |

There is at least one older file using this pool structure:

```
********
| Older Small Pool:
 00h main.dst (and stub.data.dst) ;CONST (00524C00h)
 04h maintype5.src.end
 08h maintype5.src.start
 0Ch maintype1.src.end
 10h maintype1.src.start
 14h maintype4.src.end
 18h maintype4.src.start
 1Ch whatever value ;-CONST (000003EDh)
 Older Large Pool:
 00h whatever.addr.plus00h ;\
 04h whatever.addr.plus04h ;
 08h whatever.addr.plus08h ; INCREASING addresses
 0Ch whatever.addr.plus0Ch ;
 10h whatever.addr.plus10h ;/
 14h whatever.other.addr
 18h main.dst (and stub.data.dst) ;CONST (00524C00h)
 1Ch maintype5.src.end
 20h maintype5.src.start
 24h database.dst ;CONST (53FE18h)
 28h database.src.end ;SAME as stub.code.src.start
 2Ch database.src.start
 30h stub.code.dst ;CONST (527000h)
 34h stub.code.src.end
 38h stub.code.src.start ;SAME as database.src.end
 3Ch stub.data.src.end
 40h stub.data.src.start
 44h whatever.thumb.code.addr1
 48h whatever.thumb.code.addr2
 4Ch whatever.thumb.code.addr3
 50h maintype1.src.end
 54h maintype1.src.start
 58h maintype4.src.end
 5Ch maintype4.src.start
 60h whatever value ;CONST (000003EDh) |
| --- |

Moreover, there are dozens of unknown files, which may (or may not) use yet
different pool structures.