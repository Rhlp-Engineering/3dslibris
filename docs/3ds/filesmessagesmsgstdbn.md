# Filesmessagesmsgstdbn

> Source: https://problemkaputt.de/gbatek.htm
> Section: Filesmessagesmsgstdbn

3DS Files - Messages (MsgStdBn) 
| |
| --- |

 Message files contain text messages, usually there are separate message files for different languages (in separate folders).
 
```

| <name>.msbt (MsgStdBn format) - Message Text
 <name>.msbp (MsgPrnBn format) - Message Project/styles (if present)
 RI.mstl (raw binary) - Unknown purpose |
| --- |

**MsgStdBn Header (<file>.msbt)**

```

| 000h 8 ID "MsgStdBn"
 008h 2 Byte Order (FEFFh)
 00Ah 2 Zero (0)
 00Ch 1 Message encoding (0=UTF-8, 1=UTF-16, 2=UTF-32)
 00Dh 1 Version number (always 3)
 00Eh 2 Number of following Chunks (4=LBL1+ATR1+TXT2+TSY1 or 2=NLI1+TXT2)
 010h 2 Zero (0)
 012h 4 Total Filesize (unaligned 32bit)
 016h 0Ah Zero (0) |
| --- |

**LBL1 Chunk (Labels)**

```

| 000h 4 Chunk ID "LBL1"
 004h 4 Chunk Size (minus 10h, and excluding 16-byte padding)
 008h 8 Padding (0)
 010h 4 Number of Hash Table Entries (always 65h in MsgStdBn files)
 014h N*(4+4) Number of Matches + Offset to 1st match (from LBL1+10h)
 ... .. Label strings (8bitLen+Chars+32bitIndexIntoTXT2)
 .. .. Padding to 16-byte boundary (ABh-filled) |
| --- |

Hash Table Entries

```

| 000h 4 Number of matching labels (0=None)
 004h 4 Offset to 1st matching label (from LBL1+10h) (garbage if None) |
| --- |

Label Strings

```

| 000h 1 Label String length (LEN)
 001h LEN Label String (ASCII) (without any ending 00h byte)
 1+LEN 4 Message Index in TXT2 Chunk (unaligned 32bit) |
| --- |

**ATR1 Chunk (Attributes)**

```

| 000h 4 Chunk ID "ATR1"
 004h 4 Chunk Size (minus 10h, and excluding 16-byte padding)
 008h 8 Zero (0)
 010h 4 Number of Messages (N) (same as in TXT2 chunk)
 014h 4 Bytes per Attribute (siz) (0=None)
 ... N*4 offsets to attributes from ATR1+10h ;\only present if siz<>0
 ... N*siz attributes? (voice/window style?) ;/
 .. .. Padding to 16-byte boundary (ABh-filled) |
| --- |

**TXT2 Chunk (Text Strings)**

```

| 000h 4 Chunk ID "TXT2"
 004h 4 Chunk Size (minus 10h, and excluding 16-byte padding)
 008h 8 Zero (0)
 010h 4 Number of Messages (N)
 014h N*4 Offsets to Strings (from TXT2+10h)
 ... .. Strings (usually in UTF-16, terminated by 0000h)
 .. .. Padding to 16-byte boundary (ABh-filled) |
| --- |

UTF-16 Characters:

```

| 0000h=End of String (except inside Escape sequences)
 000Ah=Linebreak
 000Eh=Escape Code (change text color, etc.)
 00xxh=ASCII Characters 20h..7Eh |
| --- |

Escape Sequences:

```

| 000h 2 Escape Code (000Eh)
 002h 2 Command Type
 004h 2 Command SubType
 006h 2 Number of following parameter bytes (0=None) (usually even)
 008h N Parameters |
| --- |

Command Type.SubType are maybe somewhat like so:

```

| 0000h.0003h Change Color maybe (param16bit) ?
 0000h.000xh Change other stuff maybe, like font size? |
| --- |

There is probably also something for inserting User Name or other strings and
numeric variables, sound effects, and perhaps something for prompting user
input?

**TSY1 Chunk (Style Info)**

```

| 000h 4 Chunk ID "TSY1"
 004h 4 Chunk Size (minus 10h, and excluding 16-byte padding) (NumMsg*4)
 008h 8 Zero (0)
 010h N*4 Style index in MSBP file (aka MsgPrjBn) (FFFFFFFFh=None?)
 .. .. Padding to 16-byte boundary (ABh-filled) |
| --- |

Uh, 3DS System Settings has Style Index values 0, 1, 2, and FFFFFFFFh (but
doesn't has a .msbt file at all).

**NLI1 Chunk (reportedly used in some files, instead of LBL1/ATR1/TSY1?)**

```

| 0x00h 4 Chunk ID "NLI1"
 0x04h 4 Chunk Size (minus 10h, and excluding 16-byte padding)
 0x08h 8 Zero
 0x10h 4 Number of entries
 0x14h N*(4+4) Whatever List (Message ID? and Offset in TXT2) |
| --- |

Hash Tables

Many items (such as messages in msbt files or colors in msbp files) are looked
up by label. The labels are looked up with a hash table and are stored in a
different block than the items themselves. In official files the hash table
always has a fixed number of slots (101 in msbt files, 29 in msbp files), even
if it contains only a few labels.

```

| The following hash algorithm is used:
 def calc_hash(label, num_slots):
 hash = 0
 for char in label:
 hash = hash * 492h + ord(char)
 return (hash AND 0FFFFFFFFh) MOD num_slots |
| --- |

The block with the labels contains the following data:

```

| 000h 4 Number of slots
 004h 8*N Hash table slots
 ... .. Labels |
| --- |

Hash Table Slot

```

| 000h 4 Number of labels
 004h 4 Offset to labels |
| --- |

Label

```

| 000h 1 Length of label string
 001h .. Label string (without null terminator)
 ... 4 Item index |
| --- |

```

| __________________________ MsgPrjBn (aka .msbp file)__________________________ |
| --- |

**MsgPrjBn Header (<file>.msbp) (if any)**

```

| 000h 8 ID "MsgPrjBn"
 008h 2 Byte Order (FEFFh)
 00Ah 2 Zero (0)
 00Ch 1 Message encoding (0=UTF-8, 1=UTF-16, 2=UTF-32) (usually 0 ??)
 00Dh 1 Version number (always 3)
 00Eh 2 Number of following Chunks (12=CLR1+CLB1+ATI2+ALB1+etc.)
 010h 2 Zero (0)
 012h 4 Total Filesize (unaligned 32bit)
 016h 0Ah Zero (0) |
| --- |

This file contains style information for text messages.

```

| CLR1 Color RGBA Values ;\Colors
 CLB1 Color Labels ;/
 ATI2 Attribute Information ;\
 ALB1 Attribute Labels ; Attributes
 ALI2 Attribute Lists ;/
 TGG2 Tag Groups ;\
 TAG2 Tags ; Tags
 TGP2 Tag Parameters ;
 TGL2 Tag Lists ;/
 SYL3 Styles ;\Styles
 SLB1 Style Labels ;/
 CTI1 Project Content Info ;-Contents |
| --- |

The overall chunk format is:

```

| 000h 4 Chunk ID (eg. "CLR1")
 004h 4 Chunk Size (minus 10h, and excluding 16-byte padding)
 008h 8 Padding (0)
 010h .. Data (depending on chunk)
 .. .. Padding to 16-byte boundary (ABh-filled) |
| --- |

```

| ______________________________ MsgPrjBn Colors _______________________________ |
| --- |

**CLR1 Chunk (Color RGBA Values)**

```

| 000h 4+4+8 Chunk ID "CLR1", Chunk Size, Zeroes
 010h 4 Number of Colors
 014h N*4 Colors (RGBA)
 .. .. Padding to 16-byte boundary (ABh-filled) |
| --- |

**CLB1 Chunk (Color Labels)**

```

| 000h 4+4+8 Chunk ID "CLB1", Chunk Size, Zeroes
 010h 4 Number of Hash Table Entries (always 1Dh in MsgPrjBn files)
 014h N*(4+4) Number of Matches + Offset to 1st match (from CLB1+10h)
 ... .. Label strings (8bitLen+Chars+32bitIndexIntoCLR1)
 .. .. Padding to 16-byte boundary (ABh-filled) |
| --- |

Color Labels can be "Red", "Write, "Blue", for example.

```

| ____________________________ MsgPrjBn Attributes _____________________________ |
| --- |

**ATI2 Chunk (Attribute Information)**

```

| 000h 4+4+8 Chunk ID "ATI1", Chunk Size, Zeroes
 010h 4 Number of attributes (0=None)
 014h 8*N Attributes |
| --- |

Attribute Format:

```

| 000h 1 Type
 001h 1 Padding
 002h 2 List index (in ALI2 block, only valid if the type=9)
 004h 4 Offset |
| --- |

**ALB1 Chunk (Attribute Labels)**

```

| 000h 4+4+8 Chunk ID "ALB1", Chunk Size, Zeroes
 This block contains labels for the ATI2 block. |
| --- |

The Hash slots can be all empty without any strings (eg. as so in DSi-to-3DS
Transfer Tool).

**ALI2 Chunk (Attribute Lists)**

```

| 000h 4+4+8 Chunk ID "ALI2", Chunk Size, Zeroes
 010h 4 Number of lists
 014h 4*N Offsets to attribute lists
 ... .. Attribute lists |
| --- |

Attribute List

```

| 000h 4 Number of list items
 004h 4*N Offsets to list item names
 ... .. Null-terminated list item names |
| --- |

```

| _______________________________ MsgPrjBn Tags ________________________________ |
| --- |

**TGG2 Chunk (Tag Groups)**

```

| 000h 4+4+8 Chunk ID "TGG2", Chunk Size, Zeroes
 010h 2 Number of tag groups
 012h 2 Padding
 014h 4*N Offsets to tag groups
 ... .. Tag groups |
| --- |

Tag Group

```

| 000h 2 Number of tags
 002h 2*N Tag indexes (in TAG2 block)
 ... .. Null-terminated tag group name |
| --- |

Tag Groups can be "UserName", "DayTime", "ErrorInfo", for example.

**TAG2 Chunk (Tags)**

```

| 000h 4+4+8 Chunk ID "TAG2", Chunk Size, Zeroes
 010h 2 Number of tags
 012h 2 Padding
 014h 4*N Offsets to tags |
| --- |

Tag

```

| 000h 2 Number of tag parameters
 002h 2*N Tag parameter indexes (in TGP2 block)
 ... .. Null-terminated tag name |
| --- |

Tags can be "MyName", "SenderName" "Hour", "Minute", "ErrorCode", for example.

**TGP2 Chunk (Tag Parameters)**

```

| 000h 4+4+8 Chunk ID "TGP2", Chunk Size, Zeroes
 010h 2 Number of parameters
 012h 2 Padding
 014h 4*N Offsets to parameters |
| --- |

Tag Parameter

```

| 000h 1 Parameter type
 If type is not 9:
 001h .. Null terminated parameter name
 If type is 9:
 001h 1 Padding
 002h 2 Number of tag list items
 004h 2*N List item indexes (in TGL2 block)
 ... .. Null terminated parameter name |
| --- |

Parameter names can be "face", "percent", "r", "g", "b", "a", "name", for
example.

**TGL2 Chunk (Tag Lists)**

```

| 000h 4+4+8 Chunk ID "TGL2", Chunk Size, Zeroes
 010h 2 Number of list items (0=None)
 012h 2 Padding
 014h 4*N Offsets to list item names
 ... .. Null-terminated list item names |
| --- |

```

| ______________________________ MsgPrjBn Styles _______________________________ |
| --- |

**SYL3 Chunk (Styles)**

```

| 000h 4+4+8 Chunk ID "SYL3", Chunk Size, Zeroes
 010h 4 Number of styles
 014h 16*N Styles |
| --- |

Style

```

| 000h 4 Region width (in pixels? max FFh on DSi)
 004h 4 Line num (position? or Height?) (usually 1 or more)
 008h 4 Font index (eg. 0,1,2 for type, or small/med/large maybe?)
 00Ch 4 Base color index (uh, what?) (usually FFFFFFFFh) (or 0) |
| --- |

**SLB1 Chunk (Style Labels)**

```

| 000h 4+4+8 Chunk ID "SLB1", Chunk Size, Zeroes
 This block contains labels for the SYL3 block. |
| --- |

Style names can be "commonBtn_D_1_T_sysBtnL_00" or the like.

```

| ______________________________ MsgPrjBn Content ______________________________ |
| --- |

**CTI1 Chunk (Project Content Info)**

This block contains the names of the source files that the MSBT files were
generated from. The source files have the .mstxt extension.

```

| 000h 4+4+8 Chunk ID "CTI1", Chunk Size, Zeroes
 010h 4 Number of filenames
 014h 4*N Offsets to filename strings (from CTI1+10h)
 ... .. Null-terminated source filename strings (eg. "cardboard.mstxt") |
| --- |

```

| ________________________________ RI.mstl file ________________________________ |
| --- |

**RI.mstl file (unknown purpose)**

```

| 000h 4 Number of whatevers
 004h N*2Ch Whatevers |
| --- |