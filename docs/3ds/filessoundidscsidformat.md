# Filessoundidscsidformat

> Source: https://problemkaputt.de/gbatek.htm
> Section: Filessoundidscsidformat

3DS Files - Sound IDs (CSID Format) 
| |
| --- |

 Some titles have the CSAR file bundled with a separate CSID file with .csid extension (eg. 3DS Health Safety RomFS:\sound\Main.csid and 3DS Instruction Manual applet RomFS:\sound\Main.csid), which appears to be a relict from devkit, used before/after generating the CSAR file.
 The file contains Symbols and 32bit Resource IDs in ASCII, oddly, with an UTF-8 Byte Order Mark (EFh,BBh,BFh) in the file header.