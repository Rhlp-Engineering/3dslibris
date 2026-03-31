# Filesfirmlaunchparameters

> Source: https://problemkaputt.de/gbatek.htm
> Section: Filesfirmlaunchparameters

3DS Files - FIRM Launch Parameters 
| |
| --- |

 **FIRM Launch Parameters**
 The FIRM-launch parameters structure is located at FCRAM+0, size 0x1000-bytes. The ARM11-kernel copies this structure elsewhere, then clears the 0x1000-bytes at FCRAM+0. It will not handle an existing structure at FCRAM+0 if CFG_BOOTENV is zero. The ARM9 kernel writes some values about the boot environment to AXI WRAM during init to enable this.
 Note: it seems NATIVE_FIRM ARM11-kernel didn't parse this during boot until 3.0.0-X?
 
```

| 000h 300h unknown/unspecified (probably as on DSi?)
 300h 100h 'TLNC' block created by TWL applications, handled by NS for
 backwards-compatibility purposes. See here for more info
 400h 4 Flags
 404h 0Ch unknown/unspecified
 410h 0Ch This is used for overriding the FIRM_* fields in
 Configuration_Memory, when the flag listed below is set,
 in the following order (basically just data-copy from here
 to 0x1FF80060): "FIRM_?", FIRM_VERSIONREVISION,
 FIRM_VERSIONMINOR, FIRM_VERSIONMAJOR, FIRM_SYSCOREVER,
 and FIRM_CTRSDKVERSION
 41Ch .. unknown/unspecified
 438h 4 The kernel checks this field for value 0xFFFF, if it matches
 the kernel uses the rest of these parameter fields,
 otherwise FIRM-launch parameters fields are ignored by
 the kernel
 43Ch 4 CRC32 across [400h..53Fh] with [43Ch]=zero
 When invalid the kernel clears the entire buffer used for
 storing the FIRM-params, therefore no actual FIRM-params are
 handled after that
 440h 10h Titleinfo Program Info, used by NS during NS startup, to
 launch the specified title when the below flag is set
 450h 10h Titleinfo Program Info. This might be used for returning
 to the specified title, once the above launched title
 terminates?
 460h 4 Titleinfo structure is set (bit0: 0=Not set, 1=Set)
 464h .. unknown/unspecified
 480h 20h Can be set via buf1 for APT:SendDeliverArg/APT:StartApplication
 4A0h 10h Can be set by NSS:SetWirelessRebootInfo
 4B0h 14h SHA1-HMAC across banner for TWL/NTR titles ;NSS:SetTWLBannerHMAC
 4C4h .. unknown/unspecified
 500h 40h This is used by APT:LoadSysMenuArg and APT:StoreSysMenuArg
 540h .. unknown/unspecified
 D50h 20h WlFirm info (same as DSi 20005E0h) ;nice, but lacks type1/4/5 info
 D70h 290h Config data struct for LGY FIRM |
| --- |

Flags from offset 0x400:

```

| +00h 1 This can be used for overriding the default FCRAM
 memory-regions allocation sizes (APPLICATION, SYSTEM, and
 BASE). The values for this is the same as
 Configmem-APPMEMTYPE. Values 0-1 are handled the same way
 by the kernel. However for NS, 0=titleinfo structure for
 launching a title isn't set, while non-zero=titleinfo
 structure is set.
 +01h 3 Setting bit0 here enables overriding the FIRM_* fields in
 Configuration_Memory. |
| --- |

Config struct for booting LGY FIRMs from offset 0xD70:

```

| 000h 1 Config BlkID=00030000h RTC Correction value
 001h 1 Config BlkID=00070001h Sound Output Mode
 002h 1 Config BlkID=000A0002h User Language
 003h 1 SecureInfo Region (pseudo BlkID=00140000h in LGY FIRM)
 004h 0Fh SecureInfo Serial/Barcode (pseudo BlkID=00140001h in LGY FIRM)
 013h 1 Config BlkID=00100002h DSi Country
 014h 10h Config BlkID=00100003h DSi User ID
 024h 2 Config BlkID=00100000h DSi EULA
 026h 2 Unused (0)
 028h 94h Config BlkID=00100001h DSi Parental Restrictions
 0BCh 2 Config BlkID=00050000h LCD Screen flicker (VCOM)
 0BEh 2 Config BlkID=00050001h LCD Backlight controls
 0C0h 38h Config BlkID=00050002h LCD Backlight PWM (BLPWM)
 0F8h 20h Config BlkID=00050004h LCD Power saving mode (ABL) (read by LGY)
 118h 134h Config BlkID=00020000h Codec (CDC aka TSC)
 24Ch 10h Config BlkID=00040000h Input Touchscreen
 25Ch 1Ch Config BlkID=00040001h Input Circle Pad (left Analog Stick)
 278h 8 Unused (0)
 280h 8 Config BlkID=00030001h RTC Time offset
 288h 2 CRC16 across [000h..287h] with still unknown initial value
 28Ah 2 CRC16 size flag (0=VariableSize=[28Ch], Nonzero=FixedSize=288h)
 28Ch 4 CRC16 size value (288h) |
| --- |