# Cryptoaeskeygenerator

> Source: https://problemkaputt.de/gbatek.htm
> Section: Cryptoaeskeygenerator

3DS Crypto - AES Key Generator 
| |
| --- |

 **Decrypt OTP and get otp_seed (and rom_seed for retail/debug)**
 
```

| if [10010010h]=00h then otp_keyiv=FFFFD6E0h else otp_keyiv=FFFFD700h
 if [10010010h]=00h then rom_seed=FFFFD860h else rom_seed=FFFFDC60h
 aes_setkey_normal(key=otp_keyiv+00h)
 aes_CBC_decrypt(iv=otp_keyiv+10h, src=10012000h, dst=decrypted_otp, len=100h)
 sha256(src=decrypted_otp, dst=temp, len=E0h)
 if [temp+0..1Fh]=[decrypted_otp+E0h..FFh] then otp_okay=1 else otp_okay=0
 if otp_okay=1 then otp_seed=decrypted_otp+90h else otp_seed=10012000h
 if otp_okay=1 then copy [decrypted_otp+0..8Fh] to [ITCM+3800h..388Fh] |
| --- |

**Generate bootrom keys (using above otp_seed and rom_seed)**

```

| sha256(src=otp_seed(00h..1Bh)+rom_seed(00h..23h), dst=temp, len=40h=1Ch+24h)
 aes_setkey_xy(key_x=temp+00h, key_y=temp+10h)
 aes_CBC_encrypt(iv=rom_seed+24h, src=rom_seed+34h, dst=temp, len=40h)
 key_x.00h-03h = uninitialized
 key_x.04h-07h = temp+00h
 key_x.08h-0Bh = temp+10h
 key_x.0Ch-0Fh = temp+20h
 key_x.10h = temp+30h
 key_x.11h-13h = uninitialized
 ;sha256(src=otp_seed(1Ch..37h)+rom_seed(74h..97h), dst=temp, len=40h=1Ch+24h)
 ;DO NOT: aes_setkey_xy(key_x=temp+00h, key_y=temp+10h)
 aes_CBC_encrypt(iv=rom_seed+98h, src=rom_seed+A8h, dst=temp, len=40h)
 key_x.14h = temp+00h
 key_x.15h = temp+10h
 key_x.16h = temp+20h
 key_x.17h = temp+30h
 ;sha256(src=otp_seed(38h..53h)+rom_seed(B8h..DBh), dst=temp, len=40h=1Ch+24h)
 ;DO NOT: aes_setkey_xy(key_x=temp+00h, key_y=temp+10h)
 aes_CBC_encrypt(iv=rom_seed+DCh, src=rom_seed+ECh, dst=temp, len=40h)
 key_x.18h-1Bh = temp+00h
 key_x.1Ch-1Fh = temp+10h
 key_x.20h-23h = temp+20h
 key_x.24h = temp+30h
 key_x.25h-27h = uninitialized
 ;sha256(src=otp_seed(54h..6Fh)+rom_seed(12Ch..14Fh),dst=temp, len=40h=1Ch+24h)
 ;DO NOT: aes_setkey_xy(key_x=temp+00h, key_y=temp+10h)
 aes_CBC_encrypt(iv=rom_seed+150h, src=rom_seed+160h, dst=temp, len=40h)
 key_x.28h = temp+00h
 key_x.29h = temp+10h
 key_x.2Ah = temp+20h
 key_x.2Bh = temp+30h
 key_x.2Ch-2Fh = rom_seed+170h ;\
 key_x.30h-33h = rom_seed+180h ;
 key_x.34h-37h = rom_seed+190h ;
 key_x.38h-3Bh = rom_seed+1A0h ; fixed key X values
 key_x.3Ch = rom_seed+1B0h ;
 key_x.3Dh = rom_seed+1C0h ;
 key_x.3Eh = rom_seed+1D0h ;
 key_x.3Fh = rom_seed+1E0h ;/
 key_y.00h-03h = uninitialized
 key_y.04h = rom_seed+1F0h ;\
 key_y.05h = rom_seed+200h ;
 key_y.06h = rom_seed+210h ;
 key_y.07h = rom_seed+220h ; fixed key Y values
 key_y.08h = rom_seed+230h ;
 key_y.09h = rom_seed+240h ;
 key_y.0Ah = rom_seed+250h ;
 key_y.0Bh = rom_seed+260h ;/
 key_y.0Ch-3Eh = uninitialized
 key_y.3Fh = as set for above CBC encrypt
 key.00h-03h = uninitialized
 key.04h-0Bh = set via above key X/Y combos
 key.0Ch-0Fh = rom_seed+270h ;\
 key.10h = rom_seed+280h ;
 key.11h-13h = uninitialized ;
 key.14h = rom_seed+290h ;
 key.15h = rom_seed+2A0h ;
 key.16h = rom_seed+2B0h ;
 key.17h = rom_seed+2C0h ;
 key.18h-1Bh = rom_seed+2D0h ; fixed normal key values
 key.1Ch-1Fh = rom_seed+2E0h ;
 key.20h-23h = rom_seed+2F0h ;
 key.24h = rom_seed+300h ;
 key.25h-27h = uninitialized ;
 key.28h = rom_seed+300h, too ;
 key.29h = rom_seed+310h ;
 key.2Ah = rom_seed+320h ;
 key.2Bh = rom_seed+330h ;
 key.2Ch-2Fh = rom_seed+340h ;
 key.30h-33h = rom_seed+350h ;
 key.34h-37h = rom_seed+360h ;
 key.38h = rom_seed+370h ;
 key.39h-3Bh = uninitialized ;
 key.3Ch = rom_seed+370h, too ;
 key.3Dh = rom_seed+380h ;
 key.3Eh = rom_seed+390h ;
 key.3Fh = rom_seed+3A0h ;/
 [FFF00618h] = rom_seed+3A0h, too ;-copy of normal key 3Fh
 [temp] = fillword(rom_seed+3B0h) ;-dummy/fill temp buffer on stack |
| --- |

```

| ___________ Further important keys (changed after init by bootrom) ___________ |
| --- |

Misc keys:

```

| key_y.05h = 4D,80,4F,4E,99,90,19,46,13,A2,04,AC,58,44,60,BE ;by New3DS FIRM
 key_y.30h = movable.sed[110h..11Fh]
 key_y.34h = movable.sed[110h..11Fh] ;SD Key
 key_y.3Ah = movable.sed[110h..11Fh] ;DSiware Exports |
| --- |

NCCH retail keys:

```

| key_x.25h = CE,E7,D8,AB,30,C0,0D,AE,85,0E,F5,E3,82,AC,5A,F3 ;NCCH firm 7.0
 key_x.18h = 82,E9,C9,BE,BF,B8,BD,B8,75,EC,C0,A0,7D,47,43,74 ;NCCH firm 9.3
 key_x.1Bh = 45,ad,04,95,39,92,c7,c8,93,72,4a,9a,7b,ce,61,82 ;NCCH firm 9.6 |
| --- |

NCCH debug keys:

```

| fix_sys = 52,7C,E6,30,A9,CA,30,5F,36,96,F3,CD,E9,54,19,4B ;fixed system key
 fix_zero = 00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00 ;fixed zero key
 key_x.25h = 81,90,7A,4B,6F,1B,47,32,3A,67,79,74,CE,4A,D7,1B ;NCCH firm 7.0
 key_x.18h = 30,4B,F1,46,83,72,EE,64,11,5E,BD,40,93,D8,42,76 ;NCCH firm 9.3
 key_x.1Bh = 6C,8B,29,44,A0,72,60,35,F9,41,DF,C0,18,52,4F,B6 ;NCCH firm 9.6 |
| --- |

DSi keys (in little endian):

```

| OTP_TWLMODE = decrypted_otp+08h..0Fh xor 08C267B7B358A6AFh or 80000000h
 key_x.03h[0..3]: OTP_TWLMODE[0..3]
 key_x.03h[4..B]: "NINTENDO" ;<-- this unlike as on real DSi
 key_x.03h[C..F]: OTP_TWLMODE[4..7]
 key_y.03h[0..F]: 0AB9DC76h,BD4DC4D3h,202DDD1Dh,E1A00005h
 Note: DSi-keys can be set by 3DS/ARM9, but the 3DS-to-DSi mode switch does
 reset all AES KEY/IV registers to zero, so one must (re-)initialize them
 via DSi/ARM7 after switching to DSi-mode. |
| --- |

**Ticket Keys (aka "Common" keys)**

The ticket keys in FIRM are as so:

```

| 55,A3,F8,72,BD,C8,0C,55,5A,65,43,81,13,9E,15,3B ;key_3Dh.normal.0 (debug)
 D0,7B,33,7F,9C,A4,38,59,32,A2,E2,57,23,23,2E,B9 ;key_3Dh.y.0 (retail)
 0C,76,72,30,F0,99,8F,1C,46,82,82,02,FA,AC,BE,4C ;key_3Dh.y.1 (retail+debug)
 C4,75,CB,3A,B8,C7,88,BB,57,5E,12,A1,09,07,B8,A4 ;key_3Dh.y.2 (retail+debug)
 E4,86,EE,E3,D0,C0,9C,90,2F,66,86,D4,C0,6F,64,9F ;key_3Dh.y.3 (retail+debug)
 ED,31,BA,9C,04,B0,67,50,6C,44,97,A3,5B,78,04,FC ;key_3Dh.y.4 (retail+debug)
 5E,66,99,8A,B4,E8,93,16,06,85,0F,D7,A1,6D,D7,55 ;key_3Dh.y.5 (retail+debug) |
| --- |

Retail Normal Keys (after mangling above with bootrom's retail Key_3Dh.X):

```

| 64,C5,FD,55,DD,3A,D9,88,32,5B,AA,EC,52,43,DB,98 ;key_3Dh.0.retail (app)
 4A,AA,3D,0E,27,D4,D7,28,D0,B1,B4,33,F0,F9,CB,C8 ;key_3Dh.1.retail (sys)
 FB,B0,EF,8C,DB,B0,D8,E4,53,CD,99,34,43,71,69,7F ;key_3Dh.2.retail
 25,95,9B,7A,D0,40,9F,72,68,41,98,BA,2E,CD,7D,C6 ;key_3Dh.3.retail
 7A,DA,22,CA,FF,C4,76,CC,82,97,A0,C7,CE,EE,EE,BE ;key_3Dh.4.retail
 A5,05,1C,A1,B3,7D,CF,3A,FB,CF,8C,C1,ED,D9,CE,02 ;key_3Dh.5.retail |
| --- |

Debug Normal Keys (after mangling above with bootrom's debug Key_3Dh.X):

```

| 55,A3,F8,72,BD,C8,0C,55,5A,65,43,81,13,9E,15,3B ;key_3Dh.0.debug
 44,34,ED,14,82,0C,A1,EB,AB,82,C1,6E,7B,EF,0C,25 ;key_3Dh.1.debug
 F6,2E,3F,95,8E,28,A2,1F,28,9E,EC,71,A8,66,29,DC ;key_3Dh.2.debug
 2B,49,CB,6F,99,98,D9,AD,94,F2,ED,E7,B5,DA,3E,27 ;key_3Dh.3.debug
 75,05,52,BF,AA,1C,04,07,55,C8,D5,9A,55,F9,AD,1F ;key_3Dh.4.debug
 AA,DA,4C,A8,F6,E5,A9,77,E0,A0,F9,E4,76,CF,0D,63 ;key_3Dh.5.debug |
| --- |