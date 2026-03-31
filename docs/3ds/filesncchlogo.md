# Filesncchlogo

> Source: https://problemkaputt.de/gbatek.htm
> Section: Filesncchlogo

3DS Files - NCCH Logo 
| |
| --- |

 The logo was originally stored as file "logo" (in the encrypted ExeFS). Newer files have the logo stored in separate unencrypted data block (see NCCH header entries [130h,198h,19Ch]). Even in newer firmwares, the old location in "logo" file is still used by Safe Mode files (at least so on Old3DS). Unknown if the new logo block can also exist in absence of ExeFS.
 
 "The Logo contains distribution licensing Binary data (types: Nintendo, Licensed, Published, or Distributed). Additionally it could have no license (type None). System applications and applets that interact with the Home Menu are typically set to type 'none' as they don't display that information."
 
```

| Uh, what means "display that information"?
 Is there any way to view any logo for any titles... how/where? |
| --- |

**Logo Compression**

The logo file is a LZ11 compressed DARC. The compressed data is always padded
to 2000h-byte size.

LZ Decompression Functions

The last 20h bytes of the decompressed data is a SHA256-HMAC hash over the
actual DARC (using a yet unknown HMAC key).

**Logo DARC Archive**

3DS Files - Archive DARC

The DARC contains three folders and several files (below is from Safe Mode
System Updater):

```

| anim
 NintendoLogo_D_00_SceneOutA.bclan ;\
 NintendoLogo_D_00_SceneOutB.bclan ; D
 NintendoLogo_D_00_SceneOutC.bclan ;/
 NintendoLogo_U_00_SceneOutA.bclan ;\
 NintendoLogo_U_00_SceneOutB.bclan ; U
 NintendoLogo_U_00_SceneOutC.bclan ;/
 blyt
 NintendoLogo_D_00.bclyt ;-D
 NintendoLogo_U_00.bclyt ;-U
 timg
 3dsLogo_00.bclim
 3dsLogo_01.bclim
 3dsLogo_02.bclim
 3dsLogo_03.bclim
 LT_00.bclim
 LT_01.bclim
 LTMask_00.bclim
 Nintendo_128x64.bclim |
| --- |

Unknown if other logo/darc packages contain other file/folder names.

**Logo DARC Files**

3DS Files - Video Layout (CLYT/FLYT)

3DS Files - Video Layout Animation (CLAN/FLAN)

3DS Files - Video Layout Images (CLIM/FLIM)