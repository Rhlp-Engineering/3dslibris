# Filessavedatagamecardflashencryptionandwearleveling

> Source: https://problemkaputt.de/gbatek.htm
> Section: Filessavedatagamecardflashencryptionandwearleveling

3DS Files - Savedata Gamecard FLASH Encryption and Wear Leveling 
| |
| --- |

 Unlike SD and NAND savegames, Gamecard savegames have and additional Encryption and Wear Leveling layer.
 
 
```

| ________________________ Gamecard savegame Encryption ________________________ |
| --- |

**Repeating CTR Fail (newbie guide on what is a repeating 512-byte keystream)**

On the 3DS savegames are stored much like on the DS, that is on a FLASH chip in
the gamecart. On the DS these savegames were stored in plain-text but on the
3DS a layer of encryption was added. This is AES-CTR, as the contents of
several savegames exhibit the odd behavior that xor-ing certain parts of the
savegame together will result in the plain-text appearing.

The reason this works is because the stream cipher used has a period of 512
bytes. That is to say, it will repeat the same keystream after 512 bytes. The
way you encrypt with a stream cipher is you XOR your data with the keystream as
it is produced. Unfortunately, if your streamcipher repeats and you are
encrypting a known plain-text (in our case, zeros) you are basically giving
away your valuable keystream.

**Savegame keyY**

All gamecard and (!!?!!) SD savegames are encrypted with AES-CTR. The base CTR
for gamecard savegames is all-zero. The gamecard savegame keyslots' keyY (these
savegame keyslots use the hardware key-generator) is unique for each region and
for each game. The NCSD partition flags determine the method used to generate
this keyY. When the save NCSD flags checked by the running NATIVE_FIRM are
all-zero, the system will use the repeating CTR, otherwise a proper CTR which
never repeats within the image is used.

The AES-CMAC (which uses a hardware key-generator keyslot, as mentioned above)
at the beginning of the savegame must match the calculated CMAC using the
DISA/DIFF data, otherwise the savegame is considered corrupted (see below).

When all of the flags checked by the running NATIVE_FIRM are clear, the keyY
(original keyY method used with saves where the CTR repeats within the image)
is the following:

```

| 00h 8 First 8-bytes from the plaintext CXI accessdesc signature.
 08h 4 u32 CardID0 from gamecard plaintext-mode command 0x90,
 Process9 reads this with the NTRCARD hw. The actual cmdID used
 by Process9 is different since Process9 reads it with the
 gamecard in encrypted-mode.
 0Ch 4 u32 CardID1 from gamecard plaintext-mode command 0xA0,
 Process9 reads this with the NTRCARD hw. The actual cmdID used
 by Process9 is different since Process9 reads it with the
 gamecard in encrypted-mode. |
| --- |

**2.0.0-2 Hashed keyY and 2.2.0-4 Savegame Encryption**

When certain NCSD partition flags are set, a SHA-256 hash is calculated over
the data from the CXI (same data used with the original plain keyY), and the
0x40-bytes read from a gamecard command (this 0x40-byte data is also read by
GetRomId, which is the gamecard-uniqueID). The first 0x10-bytes from this hash
is used for the keyY. When flag[7] is set, the CTR will never repeat within the
save image, unlike the original CTR-method. All games which had the retail NCSD
image finalized after the 2.2.0-4 update (and contain 2.2.0-4+ in the System
update partition), use this encryption method.

This keyY generation method was implemented with 2.0.0-2 via NCSD partition
flag[3], however the proper CTR wasn't implemented for flag[7] until 2.2.0-4.
The hashed keyY flag[3] implemented with 2.0.0-2 was likely never used with
retail gamecards.

**6.0.0-11 Savegame keyY**

```

| XXX also used/similar for NCCH "Keyslot25h" ??? |
| --- |

6.0.0-11 implemented support for generating the savegame keyY with a new
method, this method is much more complex than previous keyY methods. This is
enabled via new NCSD partition flags, all retail games which have the NCSD
image finalized after the 6.0.0-11 release (and 6.0.0-11+ in the system update
partition) will have these flags set for using this new method.

A SHA-256 hash is calculated over the same data used with the above hashed keyY
method, after hashing the above data the following data is hashed: the CXI
programID, and the ExeFS:/.code hash from the decrypted ExeFS header. An
AES-CMAC (the keyslot used for this uses the hardware key-scrambler) is then
calculated over this hash, the output CMAC is used for the savegame keyY.

The keyY used for calculating this AES-CMAC is initialized while NATIVE_FIRM is
loading, this keyY is generated via the RSA engine. The RSA slot used here is
slot0 (key-data for slot0 is initialized by bootrom), this RSA slot0 key-data
is overwritten during system boot. This RSA slot0 key-data gets overwritten
with the RSA key-data used for verifying RSA signatures, every time Process9
verifies any RSA signatures except for NCCH accessdesc signatures. Starting
with 7.0.0-13 this key-init function used at boot is also used to initialize a
separate keyslot used for the new NCCH encryption method.

This Process9 key-init function first checks if a certain 0x10-byte block in
the 0x01FF8000 region is all-zero. When all-zero it immediately returns,
otherwise it clears that block then continues to do the key generation. This is
likely for supporting launching a v6.0+ NATIVE_FIRM under this FIRM.

```

| ___________________________ Gamecard wear leveling ___________________________ |
| --- |

The 3DS employs a wear leveling scheme on the savegame FLASH chips (only used
for CARD1 gamecards). This is done through the usage of blockmaps and a
journal. The blockmap is located at offset 0 of the flash chip, and is
immediately followed by the journal. The initial state is dictated by the
blockmap, and the journal is then applied to that.

First, there are 8 bytes whose purposes are currently unknown. Then comes the
actual blockmap. The blockmap structure is simple:

```

| struct header_entry {
 uint8_t phys_sec ;when bit7=1: block has chksums (else chksums are all 0)
 uint8_t alloc_cnt
 uint8_t chksums[8]
 } __attribute__((__packed__)); |
| --- |

There's one entry per sector, counting from physical sector 1 (sector 0
contains the blockmap/journal).

The 2 bytes that follow the blockmap are the CRC16 (with initial value FFFFh)
of the first 8 bytes and the blockmap.

Then comes the journal. The journal structure is as follows:

```

| struct sector_entry {
 uint8_t virt_sec ;Mapped to sector
 uint8_t prev_virt_sec ;Physical sector previously mapped to
 uint8_t phys_sec ;Mapped from sector
 uint8_t prev_phys_sec ;Virtual sector previously mapped to
 uint8_t phys_realloc_cnt ;Amount of times physical sector has been remapped
 uint8_t virt_realloc_cnt ;Amount of times virtual sector has been remapped
 uint8_t chksums[8]
 } __attribute__((__packed__)); |
| --- |

```

| struct long_sector_entry{
 struct sector_entry sector
 struct sector_entry dupe
 uint32_t magic ;With magic being a constant 080D6CE0h.
 }__attribute__((__packed__)); |
| --- |

The checksums in the blockmap/journal entries work as follows:

each byte is the checksum of an encrypted 200h bytes large block,

to calculate the checksum, a CRC16 of the block (with initial value FFFFh) is
calculated, and the two bytes of the CRC16 are XORed together to produce the
8bit checksum