# Filesfirmencryption

> Source: https://problemkaputt.de/gbatek.htm
> Section: Filesfirmencryption

3DS Files - FIRM Encryption 
| |
| --- |

 FIRMs are more or less unencrypted, except that they are stored in encrypted memory:
 
```

| - FIRM bootcode is stored in an encrypted partition (with the usual eMMC
 encryption; using a different keyslot than FAT partitions though)
 - FIRMs for GBA/DSi/SafeMode are stored in NCCHs files (with the eMMC
 filesystem encryption; and usually with NCCH[18Fh].bit2=0, ie. with
 further encryption in NCCH ExeFS) |
| --- |

Apart from that, FIRMs are having a RSA signature (which can be broken via
sighax).

**Self-decrypting New3DS FIRMs**

New3DS FIRMs are containing self-decrypting ARM9 code (there is no requirement
to do that in homebrew FIRMs, it's basically a nag feature, making it slightly
more difficult to disassemble or patch the official FIRMs).

The ARM9 block (the FIRM section with the ARM9 entrypoint) looks as so:

```

| 000h 10h Encrypted Key_15h_x (same for all FIRM's)
 010h 10h AES-CTR Key Y ;\for decrypting the code at 800h and up
 020h 10h AES-CTR IV ;/(these are different in each FIRM version)
 030h 8 Size of encrypted binary as decimal ASCII string (eg. "546048",0,0)
 038h 8 Padding (FFh-filled)
 040h 10h Encrypted zeroes (won't boot if decrypted data is nonzero)
 050h 7B0h Padding (FFh-filled) ;-before version 9.5.0
 050h 10h Some kind of unused debug info ;\
 060h 10h Encrypted Key_16h_x ; version 9.5.0 and up
 070h 790h Padding (00h-filled) ;/
 800h .. Encrypted ARM9-binary (size as from [030h], eg. 546048 bytes)
 ... .. Code: Unencrypted ARM9 bootstub
 ... 10h Data: Seed value for key 18h..1Fh (eg. A4h,8Dh,E4h,F1h,..)
 ... 10h Data: Key_15h_y (for encrypted zeroes) (eg. 25h,91h,2Ah,C1h,..)
 ... 4 Data: Entrypoint to decrypted ARM9 code (eg. 08xxxxxxh)
 ... 1+3 Data: Some flag byte (initially zero, and FFh-filled padding)
 ... .. Padding to 200h-byte boundary (zerofilled)
 NOTE: Later FIRMs do reportedly have another Seed value for key 19h-1Fh,
 which is probably inserted somewhere(?) in the above data area. |
| --- |

Alongsides with the keys stored at begin & end of the above ARM9 block, the
decryption code use two more sources:

```

| SHA256 computed across first 90h bytes of encrypted OTP memory
 Data read from eMMC address 12C00h (aka eMMC sector number 96h) |
| --- |

Aside from doing the self-decryption, the bootstub does additionally initialize
keyslots 15h, 16h, and 18h..1Fh (some of those are being used elsewhere).

**New3DS FIRM Self-decryption and Key Init**

```

| sha256(dst=sha_otp, src=10012000h, srclen=90h) ;\load key
 key_11h_x = sha_otp[00h..0Fh] ; sector and
 key_11h_y = sha_otp[10h..1Fh] ; decrypt it
 Read_eMMC(dst=mmc_buf, src=12C00h, len=200h) ; via OTP
 Decrypt_AES_ECB(key=11h, srcdst=mmc_buf, len=200h) ;/
 key_11h_normal = mmc_buf[000h..00Fh]
 seed = const(A4h,8Dh,E4h,F1h,..) ;from firm9+xxx ;\
 AES_ECB_Decrypt(key=11h, dst=temp, src=seed, len=10h) ; key 18h
 key_18h_x = temp[00h..0Fh] ;/
 Decrypt_AES_ECB(key=11h, dst=temp, src=firm9+00h, len=10h) ;\
 key_15h_x = temp[00h..0Fh] ;
 key_16h_x = temp[00h..0Fh] ; key 15h/16h
 if ver>=v9.6 then key_11h_normal = mmc_buf[010h..01Fh] ;
 Decrypt_AES_ECB(key=11h, dst=temp, src=firm9+60h, len=10h) ;
 if ver>=v9.5 then key_16h_x = temp[00h..0Fh] ;/
 BigEndianIncrement(seed) ;\
 if ver>=v9.6 then seed=OtherConst(??) ;XXX unknown ;
 for i=19h to 1Fh ; key 19h-1Fh
 AES_ECB_Decrypt(key=11h, dst=temp, src=seed, len=10h) ;
 key_(i)_x = temp[00h..0Fh] ;
 BigEndianIncrement(seed) ;/
 key_16h_y = firm9+10h ;\
 aes_iv = firm9+20h ; decrypt
 size=asc2bin(firm9+30h) ; firm code
 Decrypt_AES_CTR(key=16h, src=dst=firm9+800h, len=size) ;/
 key_15h_y = const(25h,91h,2Ah,C1h,..) ;from firm9+xxx ;\verify
 Decrypt_AES_ECB(key=15h, dst=temp, src=firm9+40h, len=10h) ; zeroes
 if temp[00h..0Fh]<>0 then error ;/ |
| --- |

As shown above, there are three revisions (v8.1 being the original New3DS
firmware, and small changes made in v9.5 and v9.6). The versions before v9.5
didn't actually touch keyslot 16h (the key_16h_x/y values shown above were
written to keyslot 15h). The versions before v9.6 did produce other keys for
keyslot 19h-1Fh (but they were never used, so one will only need the newer v9.6
values; which require finding a yet unknown seed value though).

**Terror**

Caution: The FIRM key init description on 3dbrew FIRM page is incomplete,
confusing, irrelevant, and simply mind blowing (the description won't make any
sense at all without simultaneously looking at the disassembled firmware code;
or perhaps looking at above pseudo code).