# Dsibasicdifferencestonds

> Source: https://problemkaputt.de/gbatek.htm
> Section: Dsibasicdifferencestonds

DSi Basic Differences to NDS 
| |
| --- |

 **Interrupts**
 There are several new interrupt sources in IE/IF registers, plus further ones in new IE2/IF2 registers.
 DS Interrupts
 
 **Video**
 Essentially same as for NDS. Some details can be changed in SCFG_EXT9. For the 2D Engine, DISPSTAT.Bit6 contains a new "LCD Initialization Ready" flag on both ARM7 and ARM9 side (the bit is checked by DSi System Menu) (the bit is supposedly used at power-up, maybe also for wake-up from certain sleep modes).
 
 **BIOS SWI Functions**
 Some SWI Functions are changed (bugged in some cases), new SHA1 and RSA functions are added, and the initial RAM contents are moved from 27FFxxxh to 2FFFxxxh (with some extra fields, eg. a copy of extended DSi cart header).
 BIOS Functions
 
 **Revised Hardware Functions**
 Some hardware features have been slightly revised (for example, the division by 0 flag was fixed). The revised functions can be enabled/disabled via SCFG registers.
 DSi Control Registers (SCFG)
 
 **NDS Slot / Cartridges**
 DSi carts are using an extended cart header (1000h bytes), with RSA signature (making it problematic to run unlicensed/homebrew code), the icon/title format has been also extended, and the cartridge protocol contains a new command (command 3Dh, for unlocking extra DSi regions on the cartridge, and for reading new DSi secure area blocks).
 The NDS Slot's Reset signal can be controlled by software (required because otherwise one could use only command 3Ch or 3Dh, but not both). The Power supply pin can be also controlled by software (yet not 100% confirmed how?). Moreover, there's new cartridge inserted sensor. And, DSi prototypes did have two NDS slots; DSi retail consoles do have only one slot, but they do still contain prototype relicts internally (like extra registers and extra irq sources for second slot) (there appear to be also unused extra pins on the CPU, but they couldn't be used without desoldering the whole chip).
 
 **Enable Bits**
 One new DSi invention is that setting Enable Bits (eg. for NDMA or CAM registers) is write-protecting the corresponding registers (ie. those registers can be initialized only while the Enable Bits are off).
 
 **SPI Bus**
 The SPI bus works same as on NDS, except, one small change is added support for 8MHz SPI clock (in SPICNT.bit2).
 
 **SPI Touchscreen Controller**
 This chip is working entirely different in DSi mode. It's still accessed via SPI bus, but with some new PAGE/INDEX values.
 DSi Touchscreen/Sound Controller
 The touchscreen hardware can be switched to NDS compatibility mode.
 
 **SPI Power Managment Device**
 The Power Managment Device contains some changed register, and some new extra registers. Internally, it is actually split to two devices: The power managment chipselect signal connects to U3 and U4 chips. Ie. some SPI registers are processed by U3 (power down, and backlight enable), and others by R4 (audio out and microphone).
 Further functions like LED control and backlight brightness are moved to the BPTWL chip, accessed via I2C bus instead of SPI bus - the power LED blink feature (which was used on Wifi access) seems to be no longer working, however, the Wifi LED seems to blink automatically on Wifi access; the changed backlight brightness mechanism shouldn't cause compatibility issuses since that feature is somewhat reserved for being controlled by the firmware.
 
 **SPI FLASH Memory (Wifi Calibration, User Settings, Firmware)**
 This memory does still exist, but it's only 128Kbytes in DSi (instead 256K), and most of it is empty (since the DSi Firmware is stored in the eMMC chip). Later DSi models do even have the SPI FLASH capacity reduced to 4Kbytes.
 
 **RTC**
 Should be compatible with NDS. But seems to contain extra registers?
 One of the RTC outputs does also seem to supply some (32kHz?) clock to some other mainboard components?
 [XXX see Seiko S-35199A01 datasheet].
 
 **Wifi**
 Supports new WPA and WPA2 encryption and supports higher transfer rates (via a new SDIO wifi unit). For the old NDS-wifi mode, there are some new control registers:
 
```

| 4004020h - SCFG_WL
 4004C04h - GPIO_WIFI
 BPTWL[30h] - Wifi LED related (also needed to enable Atheros Wifi SDIO) |
| --- |

SPI FLASH contains three new access point settings (for WPA/WPA2/proxy
support):

DS Firmware Wifi Internet Access Points

The access point configuration can be done via Firmware (unlike as on NDS,
where it needed to be done by the games).

**GBA Slot**

The GBA Slot has been removed. The memory regions and IRQ bits do still exist
internally, but the DSi does basically behave as if there is no GBA cartridge
inserted. Reading GBA ROM areas does return FFFFh halfwords instead of the
usual open bus values though.

**NDS Mode**

In NDS mode, the DSi is basically working same as NDS: The new extra hardware
is disabled, original NDS BIOS ROMs are mapped, and the hardware is simulating
the old touchscreen controller.

Nonetheless, there are still a some small differences to real NDS consoles:

- Unlicensed NDS carts don't work (requires RSA, or whitelist for older games)

- GBA Slot is removed (more or less behaves as if no cart inserted)

- DSi ports 4004700h and 4004C0xh can be read (and written?) even in DS mode

- SPI Power Managment has some added/removed/changed registers

- SPI Touchscreen controller doesn't support pressure & temperature

- SPI FLASH exists, but it's smaller, and has extra access point info, etc.

- ARM7 BIOS has only first 20h bytes locked (instead first 1204h bytes)

- Power Button issues a Reset (goes to boot menu) (instead of plain power off)

- Power LED blink feature (for wifi) is replaced by a separate wifi LED

- RTC extra registers (if they do really exist) should exist in DS mode (?)

Unknown: does hot-swapping auto-power-off the nds-cart-slot in nds mode?