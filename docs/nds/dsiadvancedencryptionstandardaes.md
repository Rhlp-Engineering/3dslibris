# Dsiadvancedencryptionstandardaes

> Source: https://problemkaputt.de/gbatek.htm
> Section: Dsiadvancedencryptionstandardaes

DSi Advanced Encryption Standard (AES) 
| |
| --- |

 **AES I/O Ports**
 DSi AES I/O Ports
 
 **AES Pseudo Code**
 Little Endian Code (as used in DSi hardware):
 DSi AES Little-Endian High Level Functions
 DSi AES Little-Endian Core Function and Key Schedule
 DSi AES Little-Endian Tables and Test Values
 Big Endian Code (as used more commonly, in non-DSi implementations):
 DSi AES Big-Endian High Level Functions
 DSi AES Big-Endian Core Function and Key Schedule
 DSi AES Big-Endian Tables and Test Values
 Most AES values are endian-free byte-strings, so different "endianness" does just mean to reverse the byte order of the 16/24/32-byte KEYs, the 16-byte data chunks, and the 16-byte CTR/CFB/CBC/MAC registers (in some of the latter cases it's also referring to actual endiannes, eg. for CTR increments).
 
 **AES Usage in DSi**
 AES-CCM is used for several SD/MMC files (using a custom Nintendo-specific CCM variant; consisting of 128K-byte data blocks with 32-byte footers):
 DSi ES Block Encryption
 AES-CTR is used for the Modcrypt areas defined in Cartridge Header, and for eMMC Boot Sectors and for eMMC MBR/Partitions.
 
 **AES Usage in DSi-Wifi**
 DSi Wifi is also supporting AES (and TKIP and WEP) encryption, the Wifi unit contains additional AES hardware for data packet encryption (however, Wifi EAPOL handshakes do require AES key unwrap implemented in software).
 
 **AES Usage in DSi-Shop**
 DSi Shop downloads (and system updates) are using big-endian AES-CBC, this does require an AES software implementation because the DSi's AES hardware couldn't decrypt that AES variant.
 
 
 
| DSi AES I/O Ports |
| --- |

 **4004400h - DSi7 - AES_CNT (parts R/W)**
 
```

| 0-4 Write FIFO Count (00h..10h words) (00h=Empty, 10h=Full) (R)
 5-9 Read FIFO Count (00h..10h words) (00h=Empty, 10h=Full) (R)
 10 Write FIFO Flush (0=No change, 1=Flush) (N/A or W)
 11 Read FIFO Flush (0=No change, 1=Flush) (N/A or W)
 12-13 Write FIFO DMA Size (0..3 = 16,12,8,4 words) (2=Normal=8) (R or R/W)
 14-15 Read FIFO DMA Size (0..3 = 4,8,12,16 words) (1=Normal=8) (R or R/W)
 16-18 CCM MAC Size, max(4,(N*2+2)) bytes, usually 7=16 bytes (R or R/W)
 19 CCM Pass Associated Data to RDFIFO (0=No/Normal, 1=Yes) (R or R/W)
 Bit19=1 is a bit glitchy: The data should theoretically arrive in
 RDFIFO immediately after writing 4 words to WRFIFO, but actually,
 Bit19=1 seems to cause 4 words held hidden in neither FIFO, until
 the first Payload block is written (at that point, the hidden
 associated words are suddenly appearing in RDFIFO)
 20 CCM MAC Verify Source (0=From AES_WRFIFO, 1=From AES_MAC) (R or R/W)
 21 CCM MAC Verify Result (0=Invalid/Busy, 1=Verified/Okay) (R)
 22-23 Unknown/Unused (0) (0)
 24 Key Select (0=No change, 1=Apply key selected in Bit26-27) (W)
 25 Key Schedule Busy (uh, always 0=ready?) (rather sth else busy?) (R)
 26-27 Key Slot (0..3=KEY0..KEY3, applied via Bit24) (R or R/W)
 28-29 Mode (0=CCM/decrypt, 1=CCM/encrypt, 2=CTR, 3=Same as 2) (R or R/W)
 30 Interrupt Enable (0=Disable, 1=Enable IRQ on Transfer End) (R or R/W)
 31 Start/Enable (0=Disable/Ready, 1=Enable/Busy) (R/W) |
| --- |

Bit31 gets cleared automatically shortly after all data (as indicated in
AES_BLKCNT) is written to WRFIFO, and the IRQ is generated alongsides; the
transfer isn't fully completed at that point since there may be still data (and
CCM/encrypt MAC result) in RDFIFO.

**4004404h - DSi7 - AES_BLKCNT (W)**

Specifies the transfer length, counted in 16-byte blocks.

```

| 0-15 Number of Extra associated data blocks for AES-CCM (unused for AES-CTR)
 16-31 Number of Payload data blocks (0..FFFFh = 0..FFFF0h bytes) |
| --- |

The length values are copied to internal counter registers on transfer start
(the value in AES_BLKCNT is left unchanged during/after transfer).

**4004408h - DSi7 - AES_WRFIFO (W)**

**400440Ch - DSi7 - AES_RDFIFO (R)**

```

| 0-31 Data |
| --- |

Writing to WRFIFO works even when AES_CNT.bit31=0 (the data does then stay in
WRFIFO though, and doesn't arrive in RDFIFO).

**4004420h - DSi7 - AES_IV (16 bytes) (W)**

This contains the Initialization Vector (aka IV aka Nonce). The hardware does
use that value to automatically initialize the internal CTR/CBC registers when
starting encryption/decryption:

```

| For AES-CTR mode: CTR[00h..0Fh] = AES_IV[00h..0Fh]
 CBC[00h..0Fh] = not used by AES-CTR mode
 For AES-CCM mode: CTR[00h..0Fh] = 00h,00h,00h,AES_IV[00h..0Bh],02h
 CBC[00h..0Fh] = x0h,xxh,0xh,AES_IV[00h..0Bh],flg |
| --- |

The initial CTR/CBC values for AES-CCM mode are following the CCM
specifications, but WITHOUT encoding the "extra associated data size" in upper
bytes of first block (see CCM pseudo code chapter for details).

The CTR/CBC registers are manipulated during transfer, however, the AES_IV
content is kept unchanged during/after transfer.

**4004430h - DSi7 - AES_MAC (16 bytes) (W)**

The MAC (Message Authentication Code) is an encrypted checksum, computed
alongsides with the actual data encryption/decryption, and used only in AES-CCM
mode. There are three ways how the DSi deals with MAC values:

```

| AES-CCM Encryption: MAC is returned in AES_RDFIFO after transfer
 AES-CCM Decryption, AES_CNT.20=0: MAC written to AES_WRFIFO after transfer
 AES-CCM Decryption, AES_CNT.20=1: MAC written to AES_MAC before transfer |
| --- |

The AES_MAC register and the RDFIFO/WRFIFO blocks are always 16-byte wide; when
selecting a smaller MAC size in AES_CNT, then the lower bytes of that 16-byte
value are 00h-padded (eg. a 6-byte MAC would appear as 00000000h, 00000000h,
xxxx0000h, xxxxxxxxh), for ENCRYPT those 00h-bytes are returned in RDFIFO, for
DECRYPT those padding bytes MUST be 00h (else the verification will fail).

The minimum MAC size is 4 bytes (trying to use 2 byte by setting AES_CNT.16-18
to 00h is producing the exact same result as when setting it to 01h, ie.
4-bytes)

**4004440h - DSi7 - AES_KEY0 (48 bytes) (W)**

**4004470h - DSi7 - AES_KEY1 (48 bytes) (W)**

**40044A0h - DSi7 - AES_KEY2 (48 bytes) (W)**

**40044D0h - DSi7 - AES_KEY3 (48 bytes) (W)**

```

| Byte 00h-0Fh Normal 128bit Key ;\use either normal key,
 Byte 10h-1Fh Special 128bit Key_X ; or special key_x/y
 Byte 20h-2Fh Special 128bit Key_Y ;/ |
| --- |

Writing the last word of "Key_Y" (or any of its last four bytes, ie. byte(s)
2Ch..2Fh) causes the Normal Key to be overwritten by following values:

```

| Key = ((Key_X XOR Key_Y) + FFFEFB4E295902582A680F5F1A4F3E79h) ROL 42 |
| --- |

After changing a key, one must (re-)apply it via AES_CNT.Bits 24,26-27.

**DMA**

The AES data would be usually transferred via two NDMA channels, one for
WRFIFO, one for RDFIFO. The NDMAs should be started BEFORE setting AES_CNT.31
(else the DMA will miss the first WRFIFO data request; and DMA won't start).
The DMAs 'Logical Block' sizes should match up with the block sizes selected in
AES_CNT (a bigger logical block size would cause FIFO overruns/underruns, a
smaller logical block size could work theoretically, but it practice it causes
the DMA to hang after the first data request; apparently data requests are
somewhat generated upon "empty-not-empty" transitions, rather than upon "enough
data/space" status).

**Reading Write-Only Values**

The AES_IV register and the AES_KEY registers are fully write-able, including
8bit STRB writes; this allows to 'read' the write-only values via brute-force
without any noticeable delay (ie. encrypt 16 bytes with original values, then
change one byte to values 00h..FFh, and check which of those values gives same
encryption result). AES_BLKCNT can be also dumped by simple counting.

**Cartheader Key Request Byte**

The firmware is usually destroying the AES_KEY registers before starting DSi
programs. However, bits in CartHeader[1B4h] allow to "request" certain keys to
be left intact.

**DSi BIOS & Firmware Keys**

The DSi BIOS contains several AES keys in the non-dumpable upper 32K halves;
most of that keys are relocated to RAM/TCM, so they can be dumped via main
memory hacks (there might be some further keys that cannot be dumped, in case
they exist only in early boot stages).

| DSi AES Little-Endian High Level Functions |
| --- |

**AES-CTR (Counter)**

aes_crypt_ctr(src,dst,len,nc_off,iv)

```

| aes_setkey(ENCRYPT,key,key_size] ;-init key
 [ctr+0..15] = [iv+0..15] ;-init ctr
 n=[nc_off]
 while len>0 ;code is 100% same for ENCRYPT and DECRYPT ;\
 if n=0 ; encrypt
 aes_crypt_block(ENCRYPT,ctr,tmp) ; or decrypt
 littleendian(ctr)=littleendian(ctr)+1 ;increment counter ; message
 [dst] = [src] xor [tmp+n] ;
 src=src+1, dst=dst+1, len=len-1, n=(n+1) and 0Fh ;/
 [nc_off]=n |
| --- |

**AES-CCM (Counter with CBC-MAC)**

aes_ccm_crypt(mode,src,dst,msg_len,iv,iv_len,xtra,xtra_len,mac,mac_len)

```

| if mac_len<4 or mac_len>16 or (mac_len and 1)=1 then error ;\limits
 if iv_len<7 or iv_len>13 then error ;/
 aes_setkey(ENCRYPT,key,key_size] ;-init key
 ctr_len = 15-iv_len ;\
 [ctr+15]=ctr_len-1 ;bit3..7=zero ;1 byte (ctr_len) ; init ctr
 [ctr+(15-iv_len)..14] = [iv+0..(iv_len-1)] ;7..13 bytes (iv) ;
 [ctr+0..(14-iv_len)]=littleendian(0) ;8..2 bytes (counter=0) ;/
 [cbc+0..15]=littleendian(msg_len) ;-[(iv_len+1)..15]=msg_len ;\
 if [cbc+15..15-iv_len]<>0 then error ;msg_len overlaps iv/flags ;
 [cbc+(15-iv_len)..14]=[iv+0..iv_len-1] ;-[1..iv_len]=iv/nonce ;
 [cbc+15].bit7=0 ;reserved/zero ;\ ; init cbc
 [cbc+15].bit6=(xtra_len>0) ; [15]=flags ;
 [cbc+15].bit5..3=(mac_len/2-1) ; ;
 [cbc+15].bit2..0=(ctr_len-1) ;/ ;
 aes_crypt_block(ENCRYPT,cbc,cbc) ;UPDATE_CBC_MAC ;/
 if NintendoDSi then ;\
 a=0 ;the DSi hardware doesn't support xtra_len encoding at all ;
 elseif xtra_len<0FF00h then ;
 [cbc+14..15]=[cbc+14..15] xor littleendian(xtra_len), a=2 ; weird
 elseif xtra_len<100000000h then ; encoding
 [cbc+14..15]=[cbc+14..15] xor littleendian(FFFEh) ; for
 [cbc+10..13]=[cbc+10..13] xor littleendian(xtra_len), a=6 ; xtra_len
 else ;
 [cbc+14..15]=[cbc+14..15] xor littleendian(FFFFh) ;
 [cbc+6..13] =[cbc+6..13] xor littleendian(xtra_len), a=10 ;/
 while xtra_len>0 ;\scatter
 z=min(xtra_len,16-a) ; cbc by
 [cbc+16-a-z..(15-a)]=[cbc+16-a-z..(15-a)] xor [xtra+0..(z-1)] ; xtra
 aes_crypt_block(ENCRYPT,cbc,cbc) ;UPDATE_CBC_MAC ; (if any)
 xtra=xtra+z, xtra_len=xtra_len-z, a=0 ;/
 while msg_len>0 ;\
 littleendian(ctr)=littleendian(ctr)+1 ;increment counter ;
 aes_crypt_block(ENCRYPT,ctr,tmp) ;CTR_CRYPT ;
 z=min(msg_len,16) ; encrypt
 if mode=ENCRYPT ; or decrypt
 [cbc+(16-z)..15] = [cbc+(16-z)..15] xor [src+0..(z-1)] ; message
 [dst+0..(z-1)] = [src+0..(z-1)] xor [tmp+(16-z)..15] ; body
 if mode=DECRYPT ;
 [cbc+(16-z)..15] = [cbc+(16-z)..15] xor [dst+0..(z-1)] ;
 aes_crypt_block(ENCRYPT,cbc,cbc) ;UPDATE_CBC_MAC ;
 src=src+z, dst=dst+z, msg_len=msg_len-z ;/
 [ctr+0..(14-iv_len)]=littleendian(0) ;reset counter=0 ;\
 aes_crypt_block(ENCRYPT,ctr,tmp) ;CTR_CRYPT ; message
 [cbc+0..15] = [cbc+0..15] xor [tmp+0..15] ; auth code
 z=mac_len ; (mac)
 IF mode=ENCRYPT then [mac+0..(z-1)] = [cbc+(16-z)..15] ;
 IF mode=DECRYPT and [mac+0..(z-1)] <> [cbc+(16-z)..15] then error;/ |
| --- |

**AES-Key-Wrap/Unwrap**

This is used for EAPOL Key Data in WPA2 Wifi packets, however, neither DSi-ARM7
nor DSi-Wifi do support that by hardware, so the unwrap (decrypt) must be
implemented by software. For details see:

DS Wifi WPA/WPA2 Encryption

Below are some other AES variants (just for curiosity - those variants aren't
used in DSi):

**AES-CBC (Cipher-block chaining)**

aes_crypt_cbc(mode,src,dst,len,iv)

```

| aes_setkey(mode,key,key_size] ;-init key
 [cbc+0..15] = [iv+0..15] ;-init cbc
 if (len AND 0Fh)>0 then error
 while len>0 ;\
 if mode=ENCRYPT ;
 [dst+0..15] = [src+0..15] xor [cbc+0..15] ;
 aes_crypt_block(mode,dst,dst) ; encrypt
 [cbc+0..15] = [dst+0..15] ; or decrypt
 if mode=DECRYPT ; message
 [tmp+0..15] = [src+0..15] ;
 aes_crypt_block(mode,src,dst) ;
 [dst+0..15] = [dst+0..15] xor [cbc+0..15] ;
 [cbc+0..15] = [tmp+0..15] ;
 src=src+16, dst=dst+16, len=len-16 ;/ |
| --- |

**AES-CFB128 (Cipher feedback on 128bits, aka 16 bytes)**

aes_crypt_cfb128(mode,src,dst,len,iv_off,iv)

```

| aes_setkey(ENCRYPT,key,key_size] ;-init key
 [cfb+0..15] = [iv+0..15] ;-init cfb
 n=[iv_off]
 while len>0 ;\
 if n=0 then aes_crypt_block(ENCRYPT,cfb,cfb) ; encrypt
 if mode=DECRYPT then c=[src], [dst]=c xor [cfb+n], [cfb+n]=c ; or decrypt
 if mode=ENCRYPT then c=[cfb+n] xor [src], [cfb+n]=c, [dst]=c ; message
 src=src+1, dst=dst+1, len=len-1, n=(n+1) and 0Fh ;/
 [iv_off]=n |
| --- |

**AES-CFB8 (Cipher feedback on 8bits, aka 1 byte, very inefficient)**

aes_crypt_cfb8(mode,src,dst,len,iv)

```

| aes_setkey(ENCRYPT,key,key_size] ;-init key
 [cfb+0..15] = [iv+0..15] ;-init cfb
 n=[iv_off]
 while len>0 ;\
 aes_crypt_block(ENCRYPT,cfb,tmp) ;
 [cfb+1..15] = [cfb+0..14] ;shift with 8-bit step ; encrypt
 if mode=DECRYPT then [cfb+0] = [src+(n xor 0Fh)] ; or decrypt
 [dst+(n xor 0Fh)] = [src+(n xor 0Fh)] xor [tmp+15] ;shift-in ; message
 if mode=ENCRYPT then [cfb+0] = [dst+(n xor 0Fh)] ;
 len=len-1, n=n+1 ;/
 [iv_off]=n |
| --- |

**AES-ECB (Electronic codebook, very basic, very insecure)**

aes_crypt_ecb(mode,src,dst,len)

```

| aes_setkey(mode,key,key_size] ;-init key
 if (len AND 0Fh)>0 then error
 while len>0 ;\encrypt
 aes_crypt_block(mode,src,dst) ; or decrypt
 src=src+16, dst=dst+16, len=len-16 ;/message |
| --- |

| DSi AES Little-Endian Core Function and Key Schedule |
| --- |

```
****
| aes_crypt_block(mode,src,dst):
 Y0 = RK[0] xor [src+00h]
 Y1 = RK[1] xor [src+04h]
 Y2 = RK[2] xor [src+08h]
 Y3 = RK[3] xor [src+0Ch]
 ;below code depending on mode: <---ENCRYPT---> -or- <---DECRYPT--->
 for i=1 to nr-1
 X0 = RK[i*4+0] xor scatter32(FT,Y1,Y2,Y3,Y0) -or- (RT,Y3,Y2,Y1,Y0)
 X1 = RK[i*4+1] xor scatter32(FT,Y2,Y3,Y0,Y1) -or- (RT,Y0,Y3,Y2,Y1)
 X2 = RK[i*4+2] xor scatter32(FT,Y3,Y0,Y1,Y2) -or- (RT,Y1,Y0,Y3,Y2)
 X3 = RK[i*4+3] xor scatter32(FT,Y0,Y1,Y2,Y3) -or- (RT,Y2,Y1,Y0,Y3)
 Y0=X0, Y1=X1, Y2=X2, Y3=X3
 [dst+00h] = RK[nr*4+0] xor scatter8(FSb,Y1,Y2,Y3,Y0) -or- (RSb,Y3,Y2,Y1,Y0)
 [dst+04h] = RK[nr*4+1] xor scatter8(FSb,Y2,Y3,Y0,Y1) -or- (RSb,Y0,Y3,Y2,Y1)
 [dst+08h] = RK[nr*4+2] xor scatter8(FSb,Y3,Y0,Y1,Y2) -or- (RSb,Y1,Y0,Y3,Y2)
 [dst+0Ch] = RK[nr*4+3] xor scatter8(FSb,Y0,Y1,Y2,Y3) -or- (RSb,Y2,Y1,Y0,Y3) |
| --- |

```
****
| scatter32(TAB,a,b,c,d): scatter8(TAB,a,b,c,d):
 w= (TAB[a.bit0..7] ror 24) w.bit0..7 = TAB[a.bit0..7]
 w=w xor (TAB[b.bit8..15] ror 16) w.bit8..15 = TAB[b.bit8..15]
 w=w xor (TAB[c.bit16..23] ror 8) w.bit16..23 = TAB[c.bit16..23]
 w=w xor (TAB[d.bit24..31]) w.bit24..31 = TAB[d.bit24..31]
 return w return w |
| --- |

```
****
| aes_setkey(mode,key,keysize): ;out: RK[0..43/51/59], nr=10/12/14
 aes_generate_tables ;<-- unless tables are already initialized
 if keysize<>128 and keysize<>192 and keysize<>256 then error ;size in bits
 rc=01h, j=0, jj=keysize/32, nr=jj+6 ;jj=4,6,8 ;\
 for i=0 to (nr+1)*4-1 ;nr=10,12,14 ; copy 16/24/32-byte key
 if i<jj then w=[key+(jj-1-i)*4+0..3] ; to RK[0..3/5/7]
 else w=w xor RK[(i-jj) xor 3] ; and, make
 RK[i xor 3]=w, j=j+1 ; RK[4/6/8..43/51/59]
 if j=jj then ;
 w=scatter8(FSb,w,w,w,w) ;
 w=(w rol 8) xor (rc shl 24) ;
 j=0, rc=rc*2, if rc>0FFh then rc=rc xor 11Bh ;
 if j=4 and jj=8 then w=scatter8(FSb,w,w,w,w) ;/
 if mode=DECRYPT then
 for i=0 to nr/2-1 ;swap entries (except middle one)
 for j=0 to 3
 w=RK[i*4+j], v=RK[nr*4-i*4+j]
 RK[i*4+j]=v, RK[nr*4-i*4+j]=w
 for i=4 to nr*4-1 ;modify entries (except RK[0..3] and RK[nr*4+0..3])
 w=RK[i], w=scatter8(FSb,w,w,w,w), RK[i]=scatter32(RT,w,w,w,w) |
| --- |

| DSi AES Little-Endian Tables and Test Values |
| --- |

```
****
| aes_generate_tables:
 for i=0 to 0FFh ;compute pow and log tables...
 if i=0 then x=01h, else x=x xor x*2, if x>0FFh then x=x xor 11Bh
 pow[i]=x, log[x]=i
 for i=0 to 0FFh ;generate the forward and reverse S-boxes...
 x=pow[0FFh-log[i]]
 x=x xor (x rol 1) xor (x rol 2) xor (x rol 3) xor (x rol 4) xor 63h
 if i=0 then x=63h
 FSb[i]=x, RSb[x]=i
 for i=0 to 0FFh ;generate the forward and reverse tables...
 x=FSb[i]*2, if x>0FFh then x=x xor 11Bh
 FT[i]=(FSb[i]*00010101h) xor (x*01000001h)
 w=00000000h, x=RSb[i]
 if x<>00h then ;ie. not at i=63h
 w=w+pow[(log[x]+log[0Eh]) mod 00FFh]*1000000h
 w=w+pow[(log[x]+log[09h]) mod 00FFh]*10000h
 w=w+pow[(log[x]+log[0Dh]) mod 00FFh]*100h
 w=w+pow[(log[x]+log[0Bh]) mod 00FFh]*1h
 RT[i]=w |
| --- |

```
****
| aes_generate_tables_results:
 pow[00h..FFh] = 01,03,05,0F,11,..,C7,52,F6,01 ;pow ;\needed temporarily
 log[00h..FFh] = 00,FF,19,01,32,..,C0,F7,70,07 ;log ;/for table creation
 FSb[00h..FFh] = 63,7C,77,7B,F2,..,B0,54,BB,16 ;Forward S-box
 RSb[00h..FFh] = 52,09,6A,D5,30,..,55,21,0C,7D ;Reverse S-box
 FT[00h..FFh] = C66363A5,F87C7C84,..,2C16163A ;Forward Table
 RT[00h..FFh] = 51F4A750,7E416553,..,D0B85742 ;Reverse Table |
| --- |

```
****
| aes_setkey_results:
 key = "AES-Test-Key-Str-1234567-Abcdefg" ;use only 1st bytes for 128/192bit
 128bit ENCRYPT --> RK[0..9..30..43] = 2D534541..2783080F..93AF7DF0..827EE10D
 192bit ENCRYPT --> RK[0..9..30..51] = 79654B2D..9708FA95..2529372B..C66C19FA
 256bit ENCRYPT --> RK[0..9..30..59] = 3332312D..DF5C92A5..74174E2E..3C8ADAE6
 128bit DECRYPT --> RK[0..9..30..43] = AEABCD4D..ECD33F19..8C87B246..7274532D
 192bit DECRYPT --> RK[0..9..30..51] = AFA9796F..72A3EFE5..455646C7..37363534
 256bit DECRYPT --> RK[0..9..30..59] = 0ED52830..4601F929..415A7D65..67666564 |
| --- |

```
****
| aes_crypt_results:
 [key+0..15] = "AES-Test-Key-Str-1234567-Abcdefg"
 [iv+0..15] = "Nonce/InitVector"
 [xtra+0..20] = "Extra-Associated-Data" ;\for CCM
 iv_len=12, mac_len=16, xtra_len=xx ;/
 Unencrypted: [dta+0..113Fh] = "Unencrypted-Data", 190h x "TestPadding"
 AES-ECB: [dta+0..113Fh] = 20,24,73,88,..,44,A8,D6,A8 ;\
 AES-CBC: [dta+0..113Fh] = A4,6F,7A,F2,..,58,C9,02,B4 ;
 AES-CFB128: [dta+0..113Fh] = 20,C6,DB,35,..,9A,83,7F,DB ; keysize=128
 AES-CFB8: [dta+0..113Fh] = 55,C7,75,1C,..,24,6E,A6,D1 ;
 AES-CTR: [dta+0..113Fh] = 20,C6,DB,35,..,AB,09,0C,75 ;
 AES-CCM: [dta+0..113Fh] = C8,37,D7,F1,..,7B,EF,FC,12 ;
 AES-CCM (ori): [mac+0..0Fh] = xx,xx,xx,xx,..,xx,xx,xx,xx ;
 AES-CCM (DSi): [mac+0..0Fh] = xx,xx,xx,xx,..,xx,xx,xx,xx ;/
 AES-ECB: [dta+0..113Fh] = CC,B6,4D,17,..,D3,56,3E,64 ;-keysize=192
 AES-ECB: [dta+0..113Fh] = A9,A9,9B,3E,..,8A,C6,13,A1 ;-keysize=256 |
| --- |

| DSi AES Big-Endian High Level Functions |
| --- |

**AES-CTR (Counter)**

aes_crypt_ctr(src,dst,len,nc_off,iv)

```

| aes_setkey(ENCRYPT,key,key_size] ;-init key
 [ctr+0..15] = [iv+0..15] ;-init ctr
 n=[nc_off]
 while len>0 ;code is 100% same for ENCRYPT and DECRYPT ;\
 if n=0 ; encrypt
 aes_crypt_block(ENCRYPT,ctr,tmp) ; or decrypt
 bigendian(ctr)=bigendian(ctr)+1 ;increment counter ; message
 [dst] = [src] xor [tmp+n] ;
 src=src+1, dst=dst+1, len=len-1, n=(n+1) and 0Fh ;/
 [nc_off]=n |
| --- |

**AES-CCM (Counter with CBC-MAC)**

aes_ccm_crypt(mode,src,dst,msg_len,iv,iv_len,xtra,xtra_len,mac,mac_len)

```

| if mac_len<4 or mac_len>16 or (mac_len and 1)=1 then error ;\limits
 if iv_len<7 or iv_len>13 then error ;/
 aes_setkey(ENCRYPT,key,key_size] ;-init key
 ctr_len = 15-iv_len ;\
 [ctr+0]=ctr_len-1 ;bit3..7=zero ;1 byte (ctr_len) ; init ctr
 [ctr+1..iv_len] = [iv+0..(iv_len-1)] ;7..13 bytes (iv) ;
 [ctr+(iv_len+1)..15]=bigendian(0) ;8..2 bytes (counter=0) ;/
 [cbc+0..15]=bigendian(msg_len) ;-[(iv_len+1)..15]=msg_len ;\
 if [cbc+0..iv_len]<>0 then error ;errif msg_len overlaps iv/flags;
 [cbc+1..iv_len]=[iv+0..iv_len-1] ;-[1..iv_len]=iv (aka nonce) ;
 [cbc+0].bit7=0 ;reserved/zero ;\ ; init cbc
 [cbc+0].bit6=(xtra_len>0) ; [0]=flags ;
 [cbc+0].bit5..3=(mac_len/2-1) ; ;
 [cbc+0].bit2..0=(ctr_len-1) ;/ ;
 aes_crypt_block(ENCRYPT,cbc,cbc) ;UPDATE_CBC_MAC ;/
 if NintendoDSi then ;\
 a=0 ;the DSi hardware doesn't support xtra_len encoding at all ;
 elseif xtra_len<0FF00h then ;
 [cbc+0..1]=[cbc+0..1] xor bigendian(xtra_len), a=2 ; weird
 elseif xtra_len<100000000h then ; encoding
 [cbc+0..1]=[cbc+0..1] xor bigendian(FFFEh) ; for
 [cbc+2..5]=[cbc+2..5] xor bigendian(xtra_len), a=6 ; xtra_len
 else ;
 [cbc+0..1]=[cbc+0..1] xor bigendian(FFFFh) ;
 [cbc+2..9]=[cbc+2..9] xor bigendian(xtra_len), a=10 ;/
 while xtra_len>0 ;\scatter
 z=min(xtra_len,16-a) ; cbc by
 [cbc+a..(a+z-1)]=[cbc+a..(a+z-1)] xor [xtra+0..(z-1)] ; xtra
 aes_crypt_block(ENCRYPT,cbc,cbc) ;UPDATE_CBC_MAC ; (if any)
 xtra=xtra+z, xtra_len=xtra_len-z, a=0 ;/
 while msg_len>0 ;\
 bigendian(ctr)=bigendian(ctr)+1 ;increment counter ;
 aes_crypt_block(ENCRYPT,ctr,tmp) ;CTR_CRYPT ;
 z=min(msg_len,16) ; encrypt
 if mode=ENCRYPT ; or decrypt
 [cbc+0..(z-1)] = [cbc+0..(z-1)] xor [src+0..(z-1)] ; message
 [dst+0..(z-1)] = [src+0..(z-1)] xor [tmp+0..(z-1)] ; body
 if mode=DECRYPT ;
 [cbc+0..(z-1)] = [cbc+0..(z-1)] xor [dst+0..(z-1)] ;
 aes_crypt_block(ENCRYPT,cbc,cbc) ;UPDATE_CBC_MAC ;
 src=src+z, dst=dst+z, msg_len=msg_len-z ;/
 [ctr+(iv_len+1)..15]=bigendian(0) ;reset counter=0 ;\
 aes_crypt_block(ENCRYPT,ctr,tmp) ;CTR_CRYPT ; message
 [cbc+0..15] = [cbc+0..15] xor [tmp+0..15] ; auth code
 z=mac_len ; (mac)
 IF mode=ENCRYPT then [mac+0..(z-1)] = [cbc+0..(z-1)] ;
 IF mode=DECRYPT and [mac+0..(z-1)] <> [cbc+0..(z-1)] then error ;/ |
| --- |

**AES-Key-Wrap/Unwrap**

This is used for EAPOL Key Data in WPA2 Wifi packets, however, neither DSi-ARM7
nor DSi-Wifi do support that by hardware, so the unwrap (decrypt) must be
implemented by software. For details see:

DS Wifi WPA/WPA2 Encryption

Below are some other AES variants (just for curiosity - those variants aren't
used in DSi):

**AES-CBC (Cipher-block chaining)**

aes_crypt_cbc(mode,src,dst,len,iv)

```

| aes_setkey(mode,key,key_size] ;-init key
 [cbc+0..15] = [iv+0..15] ;-init cbc
 if (len AND 0Fh)>0 then error
 while len>0 ;\
 if mode=ENCRYPT ;
 [dst+0..15] = [src+0..15] xor [cbc+0..15] ;
 aes_crypt_block(mode,dst,dst) ; encrypt
 [cbc+0..15] = [dst+0..15] ; or decrypt
 if mode=DECRYPT ; message
 [tmp+0..15] = [src+0..15] ;
 aes_crypt_block(mode,src,dst) ;
 [dst+0..15] = [dst+0..15] xor [cbc+0..15] ;
 [cbc+0..15] = [tmp+0..15] ;
 src=src+16, dst=dst+16, len=len-16 ;/ |
| --- |

**AES-CFB128 (Cipher feedback on 128bits, aka 16 bytes)**

aes_crypt_cfb128(mode,src,dst,len,iv_off,iv)

```

| aes_setkey(ENCRYPT,key,key_size] ;-init key
 [cfb+0..15] = [iv+0..15] ;-init cfb
 n=[iv_off]
 while len>0 ;\
 if n=0 then aes_crypt_block(ENCRYPT,cfb,cfb) ; encrypt
 if mode=DECRYPT then c=[src], [dst]=c xor [cfb+n], [cfb+n]=c ; or decrypt
 if mode=ENCRYPT then c=[cfb+n] xor [src], [cfb+n]=c, [dst]=c ; message
 src=src+1, dst=dst+1, len=len-1, n=(n+1) and 0Fh ;/
 [iv_off]=n |
| --- |

**AES-CFB8 (Cipher feedback on 8bits, aka 1 byte, very inefficient)**

aes_crypt_cfb8(mode,src,dst,len,iv)

```

| aes_setkey(ENCRYPT,key,key_size] ;-init key
 [cfb+0..15] = [iv+0..15] ;-init cfb
 while len>0 ;\
 aes_crypt_block(ENCRYPT,cfb,tmp) ;
 [cfb+0..14] = [cfb+1..15] ;shift with 8-bit step ; encrypt
 if mode=DECRYPT then [cfb+15] = [src] ; or decrypt
 [dst] = [src] xor [tmp+0] ;shift-in new 8-bits ; message
 if mode=ENCRYPT then [cfb+15] = [dst] ;
 src=src+1, dst=dst+1, len=len-1 ;/ |
| --- |

**AES-ECB (Electronic codebook, very basic, very insecure)**

aes_crypt_ecb(mode,src,dst,len)

```

| aes_setkey(mode,key,key_size] ;-init key
 if (len AND 0Fh)>0 then error
 while len>0 ;\encrypt
 aes_crypt_block(mode,src,dst) ; or decrypt
 src=src+16, dst=dst+16, len=len-16 ;/message |
| --- |

| DSi AES Big-Endian Core Function and Key Schedule |
| --- |

```
****
| aes_crypt_block(mode,src,dst):
 Y0 = RK[0] xor [src+00h]
 Y1 = RK[1] xor [src+04h]
 Y2 = RK[2] xor [src+08h]
 Y3 = RK[3] xor [src+0Ch]
 ;below code depending on mode: <---ENCRYPT---> -or- <---DECRYPT--->
 for i=1 to nr-1
 X0 = RK[i*4+0] xor scatter32(FT,Y0,Y1,Y2,Y3) -or- (RT,Y0,Y3,Y2,Y1)
 X1 = RK[i*4+1] xor scatter32(FT,Y1,Y2,Y3,Y0) -or- (RT,Y1,Y0,Y3,Y2)
 X2 = RK[i*4+2] xor scatter32(FT,Y2,Y3,Y0,Y1) -or- (RT,Y2,Y1,Y0,Y3)
 X3 = RK[i*4+3] xor scatter32(FT,Y3,Y0,Y1,Y2) -or- (RT,Y3,Y2,Y1,Y0)
 Y0=X0, Y1=X1, Y2=X2, Y3=X3
 [dst+00h] = RK[nr*4+0] xor scatter8(FSb,Y0,Y1,Y2,Y3) -or- (RSb,Y0,Y3,Y2,Y1)
 [dst+04h] = RK[nr*4+1] xor scatter8(FSb,Y1,Y2,Y3,Y0) -or- (RSb,Y1,Y0,Y3,Y2)
 [dst+08h] = RK[nr*4+2] xor scatter8(FSb,Y2,Y3,Y0,Y1) -or- (RSb,Y2,Y1,Y0,Y3)
 [dst+0Ch] = RK[nr*4+3] xor scatter8(FSb,Y3,Y0,Y1,Y2) -or- (RSb,Y3,Y2,Y1,Y0) |
| --- |

```
****
| scatter32(TAB,a,b,c,d): scatter8(TAB,a,b,c,d):
 w= (TAB[a.bit0..7]) w.bit0..7 = TAB[a.bit0..7]
 w=w xor (TAB[b.bit8..15] rol 8) w.bit8..15 = TAB[b.bit8..15]
 w=w xor (TAB[c.bit16..23] rol 16) w.bit16..23 = TAB[c.bit16..23]
 w=w xor (TAB[d.bit24..31] rol 24) w.bit24..31 = TAB[d.bit24..31]
 return w return w |
| --- |

```
****
| aes_setkey(mode,key,keysize): ;out: RK[0..43/51/59], nr=10/12/14
 aes_generate_tables ;<-- unless tables are already initialized
 if keysize<>128 and keysize<>192 and keysize<>256 then error ;size in bits
 rc=01h, j=0, jj=keysize/32, nr=jj+6 ;jj=4,6,8 ;\
 for i=0 to (nr+1)*4-1 ;nr=10,12,14 ; copy 16/24/32-byte key
 if i<jj then w=[key+i*4+0..3] ; to RK[0..3/5/7]
 else w=w xor RK[i-jj] ; and, make
 RK[i]=w, j=j+1 ; RK[4/6/8..43/51/59]
 if j=jj then ;
 w=scatter8(FSb,w,w,w,w) ;
 w=(w ror 8) xor (rc) ;
 j=0, rc=rc*2, if rc>0FFh then rc=rc xor 11Bh ;
 if j=4 and jj=8 then w=scatter8(FSb,w,w,w,w) ;/
 if mode=DECRYPT then
 for i=0 to nr/2-1 ;swap entries (except middle one)
 for j=0 to 3
 w=RK[i*4+j], v=RK[nr*4-i*4+j]
 RK[i*4+j]=v, RK[nr*4-i*4+j]=w
 for i=4 to nr*4-1 ;modify entries (except RK[0..3] and RK[nr*4+0..3])
 w=RK[i], w=scatter8(FSb,w,w,w,w), RK[i]=scatter32(RT,w,w,w,w) |
| --- |

| DSi AES Big-Endian Tables and Test Values |
| --- |

```
****
| aes_generate_tables:
 for i=0 to 0FFh ;compute pow and log tables...
 if i=0 then x=01h, else x=x xor x*2, if x>0FFh then x=x xor 11Bh
 pow[i]=x, log[x]=i
 for i=0 to 0FFh ;generate the forward and reverse S-boxes...
 x=pow[0FFh-log[i]]
 x=x xor (x rol 1) xor (x rol 2) xor (x rol 3) xor (x rol 4) xor 63h
 if i=0 then x=63h
 FSb[i]=x, RSb[x]=i
 for i=0 to 0FFh ;generate the forward and reverse tables...
 x=FSb[i]*2, if x>0FFh then x=x xor 11Bh
 FT[i]=(FSb[i]*01010100h) xor (x*01000001h)
 w=00000000h, x=RSb[i]
 if x<>00h then ;ie. not at i=63h
 w=w+pow[(log[x]+log[0Eh]) mod 00FFh]*1h
 w=w+pow[(log[x]+log[09h]) mod 00FFh]*100h
 w=w+pow[(log[x]+log[0Dh]) mod 00FFh]*10000h
 w=w+pow[(log[x]+log[0Bh]) mod 00FFh]*1000000h
 RT[i]=w |
| --- |

```
****
| aes_generate_tables_results:
 pow[00h..FFh] = 01,03,05,0F,11,..,C7,52,F6,01 ;pow ;\needed temporarily
 log[00h..FFh] = 00,FF,19,01,32,..,C0,F7,70,07 ;log ;/for table creation
 FSb[00h..FFh] = 63,7C,77,7B,F2,..,B0,54,BB,16 ;Forward S-box
 RSb[00h..FFh] = 52,09,6A,D5,30,..,55,21,0C,7D ;Reverse S-box
 FT[00h..FFh] = A56363C6,847C7CF8,..,3A16162C ;Forward Table
 RT[00h..FFh] = 50A7F451,5365417E,..,4257B8D0 ;Reverse Table |
| --- |

```
****
| aes_setkey_results:
 key = "AES-Test-Key-Str-1234567-Abcdefg" ;use only 1st bytes for 128/192bit
 128bit ENCRYPT --> RK[0..9..30..43] = 2D534541..ED0DC6FA..43DAC81C..0F5026BB
 192bit ENCRYPT --> RK[0..9..30..51] = 2D534541..4AAB3D82..29CA38D2..CA4DFE3B
 256bit ENCRYPT --> RK[0..9..30..59] = 2D534541..1AA51359..CCB886C8..88956C9C
 128bit DECRYPT --> RK[0..9..30..43] = F653079B..47DD8A1C..1C2070A7..7274532D
 192bit DECRYPT --> RK[0..9..30..51] = 3CEC6AFF..C4F96B6F..AE36B4AE..7274532D
 256bit DECRYPT --> RK[0..9..30..59] = DE7ADCD9..8C559ADD..067A387E..7274532D |
| --- |

```
****
| aes_crypt_results:
 [key+0..15] = "AES-Test-Key-Str-1234567-Abcdefg"
 [iv+0..15] = "Nonce/InitVector"
 [xtra+0..20] = "Extra-Associated-Data" ;\for CCM
 iv_len=12, mac_len=16, xtra_len=21 ;/
 Unencrypted: [dta+0..113Fh] = "Unencrypted-Data", 190h x "TestPadding"
 AES-ECB: [dta+0..113Fh] = 5F,BD,04,DB,..,E4,07,F4,B6 ;\
 AES-CBC: [dta+0..113Fh] = 0B,BB,53,FA,..,DD,28,6D,AE ;
 AES-CFB128: [dta+0..113Fh] = F4,75,4F,0E,..,73,B5,D7,E7 ; keysize=128
 AES-CFB8: [dta+0..113Fh] = F4,10,6A,83,..,BF,1B,16,3E ;
 AES-CTR: [dta+0..113Fh] = F4,75,4F,0E,..,04,DF,EB,BA ;
 AES-CCM: [dta+0..113Fh] = FD,1A,6D,98,..,EE,FD,68,F6 ;
 AES-CCM (ori): [mac+0..0Fh] = FD,F9,FE,85,..,4F,50,3C,AF ;
 AES-CCM (DSi): [mac+0..0Fh] = xx,xx,xx,xx,..,xx,xx,xx,xx ;/
 AES-ECB: [dta+0..113Fh] = 0E,69,F5,1A,..,9A,5F,7A,9A ;-keysize=192
 AES-ECB: [dta+0..113Fh] = C6,FB,68,C1,..,14,89,6C,E0 ;-keysize=256 |
| --- |

| DSi ES Block Encryption |
| --- |

ES Block Encryption, for lack of a better name, is a Nintendo DSi specific data
encryption method. It's used for some SD/MMC files:

```

| FAT16:\sys\dev.kp
 FAT16:\ticket\000300tt\4ggggggg.tik (tickets)
 SD Card: .bin files (aka Tad Files) (contains multiple blocks)
 .twl-*.der files (within the "verdata" NARC file) |
| --- |

**Block Layout**

```

| 00000h BLKLEN Data Block (AES-CCM encrypted)
 BLKLEN+00h 10h Data Checksum (AES-CCM MAC value on above Data)
 BLKLEN+10h 1 Fixed 3Ah (AES-CTR encrypted)
 BLKLEN+11h 0Ch Nonce (unencrypted)
 BLKLEN+1Dh 1 BLKLEN.bit16-23 (AES-CTR encrypted)
 BLKLEN+1Eh 1 BLKLEN.bit8-15 (AES-CTR encrypted)
 BLKLEN+1Fh 1 BLKLEN.bit0-7 (AES-CTR encrypted) |
| --- |

BLKLEN can be max 20000h. If the Data is bigger than 128Kbytes, then it's split
into multiple block(s) with BLKLEN=20000h (the last block can have smaller
BLKLEN).

**Data Block Encryption/Decryption (AES-CCM)**

```

| IV[00h..0Bh]=[BLKLEN+11h..1Ch] ;Nonce
 IV[0Ch..0Fh]=Don't care (not used for CCM) |
| --- |

With that IV value, apply AES-CCM on the Data Block:

```

| 00000h BLKLEN Data Block (AES-CCM) |
| --- |

Observe that some DSi files have odd BLKLEN values, so you may need to append
padding bytes to the Data Block (the DSi hardware requires full 16-byte chunks
for encryption/decryption).

Alternately, one could use AES-CTR for decryption (this may be faster/easier,
and allows random access, though without MAC verify), using this IV value:

```

| IV[00h..02h]=offset/10h + 1 ;CTR value for desired 16-byte block
 IV[03h..0Eh]=[BLKLEN+11h..1Ch] ;Nonce for CTR
 IV[0Fh]=02h ;Indicate 3-byte wide CTR (fixed on DSi) |
| --- |

**Data Block Padding (16-byte alignment)**

For encryption, it's simple: Just append 00h byte(s) as padding value.

For decryption, it's more complicated: The padding values should be ENCRYPTED
00h-bytes (required to get the same MAC result as for encryption). If you don't
want to verify the MAC, then you could append whatever dummy bytes. If you want
to verify the MAC, then you could pre-calculate the padding values as so:

```

| IV[00h..02h]=BLKLEN/10h + 1 ;CTR value for last 16-byte block
 IV[03h..0Eh]=[BLKLEN+11h..1Ch] ;Nonce
 IV[0Fh]=02h ;Indicate 3-byte wide CTR (fixed on DSi) |
| --- |

Then, use AES-CTR (not CCM) to encrypt sixteen 00h-bytes, the last byte(s) of
the result can be then used as padding value(s). The padding values should be
pre-calculated BEFORE starting the CCM decryption (the DSi hardware allows only
one AES task at once, so they cannot be calculated via AES-CTR when AES-CCM
decryption is in progress).

**Verifying the Footer values (AES-CTR)**

This step is needed only for verification purposes (encryption tools should
create these values, but decryption tools may or may not verify them).

```

| IV[00h]=00h ;Zero
 IV[01h..0Ch]=[BLKLEN+11h..1Ch] ;Nonce
 IV[0Dh..0Fh]=00h,00h,00h ;Zero |
| --- |

With that IV value (and same Key as for AES-CCM), apply AES-CTR on the last 16
bytes of the block:

```

| BLKLEN+10h 1 Fixed 3Ah (AES-CTR encrypted)
 BLKLEN+11h 0Ch Nonce (unencrypted)
 BLKLEN+1Dh 1 BLKLEN.bit16-23 (AES-CTR encrypted)
 BLKLEN+1Eh 1 BLKLEN.bit8-15 (AES-CTR encrypted)
 BLKLEN+1Fh 1 BLKLEN.bit0-7 (AES-CTR encrypted) |
| --- |

AES-CTR is XORing the data stream (encrypted bytes will turn into unencrypted
bytes, and vice-versa), so the result would look as so:

```

| BLKLEN+10h 1 Fixed 3Ah (unencrypted) (to be verified)
 BLKLEN+11h 0Ch Nonce (AES-CTR encrypted) (useless/garbage)
 BLKLEN+1Dh 1 BLKLEN.bit16-23 (unencrypted) (to be verified)
 BLKLEN+1Eh 1 BLKLEN.bit8-15 (unencrypted) (to be verified)
 BLKLEN+1Fh 1 BLKLEN.bit0-7 (unencrypted) (to be verified) |
| --- |

Mind that BLKLEN can be odd, so data at BLKLEN+00h..1Fh isn't necessarily
located at 4-byte aligned addresses.