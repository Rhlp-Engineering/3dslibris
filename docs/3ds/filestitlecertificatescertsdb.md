# Filestitlecertificatescertsdb

> Source: https://problemkaputt.de/gbatek.htm
> Section: Filestitlecertificatescertsdb

3DS Files - Title Certificates (certs.db) 
| |
| --- |

 **3ds:\dbs\certs.db (6000h-byte DIFF container with 2800h bytes of raw data)**
 
```

| 000h 4 ID "CERT" ;\
 004h 4 Zero ; header
 008h 4 Size of used area at 010h..A0Fh ? (A00h) ;
 00Ch 4 Random garbage? (eg. 080FFFE0h or 080496E1h) ;/
 010h 4 Signature Type (00,01,00,03) (200h-byte RSA-SHA256) ;\
 014h 200h Signature RSA across 250h..40Fh ? ;
 214h 3Ch Signature padding/alignment (zerofilled) ;
 250h .. Signature Name ("Root", zeropadded) ;
 290h .. Key Type (00,00,00,01) (100h-byte RSA) ; CA00000003
 294h .. Key Name ("CA00000003", zeropadded) ; (for below)
 2D4h 4 Key Random/time/type/flags/chksum? 7BE8EF6Ch ;
 2D8h 100h Key Public RSA Key (B2,79,C9,E2,...) ;
 3D8h 4 Key Public RSA Exponent? (00,01,00,01) ;
 3DCh 34h Key padding/alignment (zerofilled) ;/
 410h 4 Signature Type (00,01,00,04) (100h-byte RSA-SHA256) ;\
 414h 100h Signature RSA across 550h..70Fh ? ;
 514h 3Ch Signature padding/alignment (zerofilled) ;
 550h .. Signature Name ("Root-CA00000003", zeropadded) ;
 590h .. Key Type (00,00,00,01) (100h-byte RSA) ; CP0000000b
 594h .. Key Name ("CP0000000b", zeropadded) ; (for TMDs)
 5D4h 4 Key Random/time/type/flags/chksum? 137A080Bh ;
 5D8h 100h Key Public RSA Key (A6,89,C5,90,...) ;
 6D8h 4 Key Public RSA Exponent? (00,01,00,01) ;
 6DCh 34h Key padding/alignment (zerofilled) ;/
 710h 4 Signature Type (00,01,00,04) (100h-byte RSA-SHA256) ;\
 714h 100h Signature RSA across 850h..A0Fh ? ;
 814h 3Ch Signature padding/alignment (zerofilled) ;
 850h .. Signature Name ("Root-CA00000003", zeropadded) ;
 890h .. Key Type (00,00,00,01) (100h-byte RSA) ; XS0000000c
 894h .. Key Name ("XS0000000c", zeropadded) ; (for
 8D4h 4 Key Random/time/type/flags/chksum? 137A0894h ; Tickets)
 8D8h 100h Key Public RSA Key (AD,50,5B,B6,...) ;
 9D8h 4 Key Public RSA Exponent? (00,01,00,01) ;
 9DCh 34h Key padding/alignment (zerofilled) ;/
 A10h 1F0h Padding (0) ;\unused
 C00h 1C00h Random garbage? ;/ |
| --- |

The 200h-byte Public Key for the 'Root' signature is reportedly somewhere in
NATIVE_FIRM (unknown where though, knowing the first some bytes would help)
(also unknown if it is the same 'Root' key as on DSi).

The DSi has a similar file called cert.sys:

DSi SD/MMC Firmware dev.kp and cert.sys Certificate Files

The DIFF container uses the standard DISA/DIFF format

3DS Files - Savedata DISA and DIFF

However, the DIFF in certs.db does contain raw data (instead of the
SAVE/BDRI/VSXE filesystems that are usually found in DISA/DIFF containers).