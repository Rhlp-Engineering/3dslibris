# Dsiregions

> Source: https://problemkaputt.de/gbatek.htm
> Section: Dsiregions

DSi Regions 
| |
| --- |

 There are six DSi regions.
 
 **JP Region - 1 country, 1 language**
 Languages:
 
```

| Japanese (only japanese, there is no language option at all) |
| --- |

Countries:

```

| 01h Japan (only japan, there is no country option at all) |
| --- |

**Chinese Region (iQue)**

Languages: Unknown (supposedly Chinese/Mandarin?, and maybe English or so)

Countries: Unknown (supposedly China only)

```

| A0h China? |
| --- |

China appears to refer to chinese mainland only (hongkong/taiwan are reportedly
selling japanese consoles, and chinese mainland users may have imported
consoles before iQue DSi launch - meaning that many chinese speaking users will
be hardly able to play chinese DSi games; unless the games were released as
"region-free" titles).

**Korean Region - 1 country, 1 language**

Languages:

```

| Koerean (only korean, there is no language option at all) |
| --- |

Countries:

```

| 88h Korea (only korea, there is no country option at all) |
| --- |

Korea appears to refer to South Korea only (ie. there are no separate
country/parental settings for North Korea).

**Australia Region - 2 countries, 1 language (as of DSi Firmware Ver 1.4.5A)**

Languages:

```

| English (only english, there is no language option at all) |
| --- |

Countries:

```

| 41h Australia
 5Fh New Zealand |
| --- |

**US Region - 47 countries, 3 languages (as of DSi Firmware Ver 1.3U)**

Languages:

```

| English
 Francais (=French)
 Espanol (=Spanish) |
| --- |

Countries:

```

| 08h Anguilla
 09h Antigua and Barbuda
 0Ah Argentina
 0Bh Aruba
 0Dh Barbados
 0Eh Belize
 0Fh Bolivia
 10h Brazil
 11h British Virgin Islands
 12h Canada
 13h Cayman Islands
 14h Chile
 15h Colombia
 16h Costa Rica
 17h Dominica
 18h Dominican Republic
 19h Ecuador
 1Ah El Salvador
 1Bh French Guiana
 1Ch Grenada
 1Dh Guadeloupe
 1Eh Guatemala
 1Fh Guyana
 20h Haiti
 21h Honduras
 22h Jamaica
 23h Martinique
 24h Mexico
 25h Montserrat
 26h Netherlands Antilles
 27h Nicaragua
 28h Panama
 29h Paraguay
 2Ah Peru
 2Bh Saint Kitts and Nevis
 2Ch Saint Lucia
 2Dh Saint Vincent and the Grenadines
 99h Singapore
 2Eh Suriname
 0Ch The Bahamas
 2Fh Trinidad and Tobago
 30h Turks and Caicos Islands
 A8h United Arab Emirates
 31h United States
 32h Uruguay
 33h US Virgin Islands
 34h Venezuela |
| --- |

**Europe Region - 47 countries, 5 languages (as of DSi Firmware Ver 1.4E)**

Languages:

```

| English (=English)
 Francais (=French)
 Deutsch (=German)
 Espanol (=Spanish)
 Italiano (=Italian) |
| --- |

Additional EUR region languages on 3DS (including 3DS in DSi mode?):

```

| Nederlands (=Dutch)
 Portugues (=Portuguese)
 Pyccknn (=Russian) |
| --- |

Countries:

```

| 40h Albania
 42h Austria
 43h Belgium
 44h Bosnia and Herzegovnia
 45h Botswana
 46h Bulgaria
 47h Croatia
 48h Cyprus
 49h Czech Republic
 4Ah Denmark
 4Bh Estonia
 4Ch Finland
 4Dh France
 4Eh Germany
 4Fh Greece
 50h Hungary
 51h Iceland
 52h Ireland
 53h Italy
 54h Latvia
 55h Lesotho
 56h Liechtenstein
 57h Lithuania
 58h Luxembourg
 59h Macedonia
 5Ah Malta
 5Bh Montenegro
 5Ch Mozambique
 5Dh Namibia
 5Eh Netherlands
 60h Norway
 61h Poland
 62h Portugal
 63h Romania
 64h Russia
 65h Serbia (renamed to Serbia and Kosovo on 3DS)
 66h Slovakia
 67h Slovenia
 68h South Africa
 69h Spain
 6Ah Swaziland
 6Bh Sweden
 6Ch Switzerland
 6Dh Turkey
 6Eh United Kindgom
 6Fh Zambia
 70h Zimbabwe |
| --- |

Additional EUR region countries on 3DS (including 3DS in DSi mode):

```

| 41h Australia (was AUS on DSi, merged with EUR on 3DS)
 5Fh New Zealand (was AUS on DSi, merged with EUR on 3DS)
 71h Azerbaijan (3DS/Wii)
 72h Mauritania (3DS/Wii)
 73h Mali (3DS/Wii)
 74h Niger (3DS/Wii)
 75h Chad (3DS/Wii)
 76h Sudan (3DS/Wii)
 77h Eritrea (3DS/Wii)
 78h Djibouti (3DS/Wii)
 79h Somalia (3DS/Wii)
 7Ah Andorra (3DS)
 7Bh Gibraltar (3DS)
 7Ch Guernsey (3DS)
 7Dh Isle of Man (3DS)
 7Eh Jersey (3DS)
 7Fh Monaco (3DS)
 A9h India (3DS/Wii)
 B8h San Marino (3DS)
 B9h Vatican City (3DS) |
| --- |

Note that Nintendo might expand those regions in newer european firmware
versions.

The Country option implies localized Age Ratings in Parental Controls (eg. "USK
ab 18" for germany, or "18'TM" for france/finland). Changing the Country will
require to re-agree EULA crap.

The "Language" option affects the Firmware GUI, and the game title (from
Icon/Title structure). Some games are also (trying to) adopt the Firmware's
language setting for choosing the in-game language; but that can fail badly if
the game doesn't support the selected language.

**Additional Wii Regions (unsupported on DSi)**

Below don't exist on DSi. And don't exist on 3DS-EUR. But might exist on
3DS-USA or 3DS-TWN or whatever...?

```

| Southeast Asia:
 80h Taiwan
 90h Hong Kong
 91h Macao
 98h Indonesia
 9Ah Thailand
 9Bh Philippines
 9Ch Malaysia
 Middle East:
 AAh Egypt
 ABh Oman
 ACh Qatar
 ADh Kuwait
 AEh Saudi Arabia
 AFh Syria
 B0h Bahrain
 B1h Jordan |
| --- |

[http://wiibrew.org/wiki/Country_Codes](http://wiibrew.org/wiki/Country_Codes)

| Parental Controls Reset |
| --- |

**Parental Controls Reset**

Click "I Forgot" when getting prompted to enter the PIN and Secret Question,
the console will then display an 8-digit Enquiry Number (or 10-digit with
region/version RV/RVV in upper digits), the customer service hotline uses that
number (and the current month/day, MMDD) to compute a 5-digit Master Key for
resetting Parental Controls.

**DSi/Wii version**

```

| This uses a standard CRC32 with polynomial EDB88320h.
 Enquiry = "XXXXNNNN" (8-digit, based on MAC but only last 4-digits are used)
 MasterKey = ((StandardCRC32("MMDDNNNN") XOR 0000AAAAh)+14C1h mod 100000
 The XOR 0000AAAAh is replacing the standard crc32's final XOR FFFFFFFFh? |
| --- |

**3DS firmware version 1.0.0-X through 6.3.0-X**

```

| Same as DSi/Wii, but using a nonstandard CRC32 (with polynomial EDBA6320h
 instead of EDB88320h, and adding 1657h instead of 14C1h to the final result.
 Enquiry = "XXXXNNNN" (8-digit, based on MAC but only last 4-digits are used)
 MasterKey = ((NonStandardCRC32("MMDDNNNN") XOR 0000AAAAh)+1657h mod 100000
 "This had a minor refactor in 6.0.0-X but is functionally identical." |
| --- |

**3DS firmware version7.0.0-X through 7.1.0-X (with version V=0)**

```

| Similar as above, using a 10-digit Enquiry, and SHA256-HMAC instead CRC32.
 Enquiry = "RVNNNNNNNN" (10-digit, based on MAC?, upper digits=region/ver?)
 HmacKey = from mset .rodata, differs between regions.
 MasterKey = (FirstWordOf(SHA256HMAC("MMDDR0NNNNNNNN",HmacKey)) mod 100000 |
| --- |

**3DS firmware versio n7.2.0-X and up (with version VV=10 and up)**

```

| As above, but with HmacKey stored in a separate encrypted file.
 AesIv,RVV = from masterkey.bin (found in System Data Archives 2, CVer)
 AesKey = from mset .rodata, using RVV as decrypt key number
 Enquiry = "RVVNNNNNNN" (10-digit, based on MAC?, upper digits=region/ver)
 HmacKey = from masterkey.bin (decrypt via AES-CTR with AesIv and AesKey)
 MasterKey = (FirstWordOf(SHA256HMAC("MMDDR0NNNNNNNN",HmacKey)) mod 100000 |
| --- |

Newer(?) firmwares are using R=9 for japan (instead of R=0, maybe because
people did tend to omit leading zeroes when specifying the Enquiry Number).

Alternately, an easier way to reset parental controls is to dump the PIN from
"config" (3DS) or TWLCFG (DSi).