# rootca.pem

> Source: https://problemkaputt.de/gbatek.htm
> Section: rootca.pem

3DS Files - BASE64 Encoding 
| |
| --- |

 BASE64 is used to encode binary data into 7bit ASCII characters (with 6 bits per char, aka 3 bytes per 4 chars), eg. for binary attachments in email messages. The 3DS uses BASE64 in .CSDR shader files, and in 3DS Browser applet .PEM files.
 
 **BASE64 Character --> Binary**
 
```

| "A..Z" --> value 00..25 (00h..19h)
 "a..z" --> value 26..51 (1Ah..33h)
 "0..9" --> value 52..61 (34h..3Dh)
 "+" --> value 62 (3Eh)
 "/" --> value 63 (3Fh)
 "=" --> padding to 4-character boundary (if needed, at end of file)
 CR,LF --> ignored |
| --- |

**3DS .CSDR Shader files**

Found in 3DS Nintendo Zone, 3DS AR Games, 3DS Face Raiders
(RomFS:\shaders\*.csdr).

```

| <NintendoWareIntermediateFile>
 <GraphicsContentCtr Namespace="" Version="0.7.0">
 <EditData>
 ...
 </EditData>
 <Shaders>
 <ShaderCtr Name="DefaultShader">
 <BinaryData>RFZMQgMAAAAUDgAAZBQ...XplAA==</BinaryData>
 <ShaderKinds>
 ...
 </ShaderKinds>
 <Descriptions>
 ...
 </Descriptions>
 </ShaderCtr>
 </Shaders>
 </GraphicsContentCtr>
 </NintendoWareIntermediateFile> |
| --- |

The .CSDR files uses 0Dh,0Ah for linebreaks, and 09h TAB's to indent lines. The
binary data is stored in a single long line without linebreaks, terminated by
"<". The decoded BASE64 data contains a standard "DVLB" 3DS shader file.

3DS Files - Video Shaders (DVLB and DVOB Format)

Unknown if the <Descriptions> or other fields are containing any useful
additional data. Unknown if a .CSDR file can contain more than one "DVLB"
binary.

**3DS Browser Applet, RomFS:\browser\rootca.pem**

The file starts wirh a small header/comment:

```

| #
 # rootca.pem
 # |
| --- |

Then followed by several certificates, each in following format:

```

| # Issuer:
 # ...
 # O=Entrust.net
 # ...
 # Expire date : May 25 16:39:40 2019 GMT
 # SHA1 Fingerprint: 99 a6 9b e6 1a fe 88 6b 4d 2b 82 ... 7e 15 39
 -----BEGIN CERTIFICATE-----
 MIIE2DCCBEGgAwIBAgIEN0rSQzANBgkqhkiG9w0BAQUFADCBwzELMAkGA1UEBhMC
 VVMxFDASBgNVBAoTC0VudHJ1c3QubmV0MTswOQYDVQQLEzJ3d3cuZW50cnVzdC5u
 ...
 95K+8cPV1ZVqBLssziY2ZcgxxufuP+NXdYR6Ee9GTxj005i7qIcyunL2POI9n9cd
 2cNgQ4xYDiKWL2KjLB+6rQXvqzJ4h6BUcxm1XAX5Uj5tLUUL9wqT6u0G+bI=
 -----END CERTIFICATE----- |
| --- |

The Old3DS Browser .pem file uses 0Dh,0Ah for linebreaks, the New3DS Browser
.pem file uses 0Ah for linebreaks. Both browser versions have 64 chars per
binary line, terminated by "-" in the END CERTIFICATE string. The decoded
BASE64 blocks contain standard .DER files.

DSi SD/MMC Firmware Certificate/Keys (DER)

The comments contain a summary of the DER entries (eg. O=OrganizationName).

**3DS System Data Archives 1, offline mode, RomFS:\browser\cave.pem**

**3DS Nintendo Zone, RomFS:\certs\dev.pem**

**3DS Nintendo Zone, RomFS:\certs\client.key**

Contains only a single DER certificate, with 0Dh,0Ah linebreaks (cave.pem), or
0Ah linebreaks (dev.pem), without any "#" comments:

```

| -----BEGIN CERTIFICATE-----
 MIIEIDCCAwigAwIBAgIBATANBgkqhkiG9w0BAQsFADBtMQswCQYDVQQGEwJVUzET
 MBEGA1UECBMKV2FzaGluZ3RvbjEhMB8GA1UEChMYTmludGVuZG8gb2YgQW1lcmlj
 ...
 MeIurPFOFV/7+3JkZY7cGUBMwqjiou9/9QBHGPvyNhGU34aUoB9vJI164WsMfDXp
 3K7XdX8WCVY0eks4g9LhLrzDmCrdQByABT0TkmhpYykJOQaWVfWzRLJ0qCirZrDQ
 Qfma0w==
 -----END CERTIFICATE----- |
| --- |

The client.key file similar, but marked BEGIN RSA PRIVATE KEY instead of BEGIN
CERTIFICATE.