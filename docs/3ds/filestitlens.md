# Filestitlens

> Source: https://problemkaputt.de/gbatek.htm
> Section: Filestitlens

3DS Files - Title NS 
| |
| --- |

 System Data Archives NS (titleID=0004001B00010702) contains 2-4 files:
 
```

| RomFS:\key.bin
 RomFS:\value.bin
 RomFS:\ctr_backup_black_list ;v6.0.0 and up
 RomFS:\qtm_black_list ;v8.1.0 and up, New3DS only |
| --- |

Most of these files seem to contain 20bit Title IDs, merged with flags in upper
bits. The 20bit IDs are taken from bit8-27 of the full 64bit Title ID:

```

| 64bit: "0004xxxxxtttttxx" --> 20bit: "ttttt" |
| --- |

A list with 64bit 3DS Title IDs can be found at [http://www.3dsdb.com/](http://www.3dsdb.com/)

**key.bin**

Unknown purpose, used when launching applications, contains several entries
with flags and 20bit title IDs for Ridge Racer and Zelda Ocarina of Time (for
several regions each).

```

| 000h N*4 Entries (00Xttttth) |
| --- |

Original filesize was 12 bytes, changed to 48 bytes in v6.0.0, and changed to
32 bytes in later New3DS versions (and also changed the flag bits in bit20-23).
The separate entries are:

```

| 12-byte 48-byte 32-byte ;Title
 - 00000000h - ;Garbage?
 - 00400000h - ;Garbage?
 - 00800000h - ;Garbage?
 - 00800000h - ;Garbage?
 00200328h 00E00328h 00200328h ;Ridge Racer 3D (JPN)
 - 00000334h 00400334h ;Zelda Ocarina of Time 3D (JPN)
 - 00000335h 00400335h ;Zelda Ocarina of Time 3D (USA)
 - 00000336h 00400336h ;Zelda Ocarina of Time 3D (EUR)
 0020033Bh 00E0033Bh 0020033Bh ;Ridge Racer 3D (EUR)
 00200358h 00E00358h 00200358h ;Ridge Racer 3D (USA)
 - 000008F8h 004008F8h ;Zelda Ocarina of Time 3D (KOR)
 - 000008F9h 004008F9h ;Zelda Ocarina of Time 3D (CHN) |
| --- |

**value.bin**

```

| 000h N*8 Entries (00000000xxxxxxxxh) |
| --- |

Original filesize was 8 bytes, changed to 32 bytes in v6.0.0, and changed to 16
bytes in later New3DS versions.

```

| 8-bytes on Old3DS:
 000h 8 Unknown, 0000000047E00001h
 32-bytes on Old3DS:
 000h 8 Unknown, 0000000000000002h
 008h 8 Unknown, 00000000002001CAh
 010h 8 Unknown, 0000000000000000h
 018h 8 Unknown, 0000000047E00001h
 16-bytes on New3DS:
 000h 8 Unknown, 0000000047E00001h
 008h 8 Unknown, 0000000000000002h |
| --- |

This is used when launching applications.

**ctr_backup_black_list (v6.0.0 and up)**

Blacklist with 20bit Title IDs for SD Savedata Backups.

```

| 000h 4 Number of entries (5Dh in v6.0.0, or 61h in later versions)
 004h 0Ch Zero
 010h N*4 Entries (001ttttth, with ttttt=00321h..0107Ch) |
| --- |

**qtm_black_list (v8.1.0 and up, New3DS only)**

Might be a list of 20bit Title IDs for disabling QTM (New3DS head-tracking)?

```

| 000h 4 Number of entries? (04h)
 004h 0Ch Zero
 010h N*4 Entries (001F8087h, 004F8088h, 006F8089h, 010F8090h) |
| --- |

The 20bit values in range F8000h-FEFFFh are reportedly used for "Evaluation" or
"Factory" titles, rather than retail titles?