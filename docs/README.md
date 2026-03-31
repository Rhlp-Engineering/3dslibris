# Hardware Reference Documentation

Technical reference documentation extracted from [GBATek](https://problemkaputt.de/gbatek.htm) — the definitive hardware reference for Nintendo handhelds.

## Structure

```
docs/
├── README.md          ← You are here
├── nds/               ← Nintendo DS / DSi hardware reference
│   ├── technical-data.md
│   ├── memory-maps.md
│   ├── io-maps.md
│   ├── video.md
│   ├── 3d-video.md
│   ├── sound.md
│   ├── system-peripherals.md
│   ├── wireless-communications.md
│   ├── cartridges-encryption-firmware.md
│   ├── bios-functions.md
│   ├── external-connectors.md
│   ├── dsi-*.md       ← DSi-specific sections
│   └── ...
└── 3ds/               ← Nintendo 3DS hardware reference
    ├── reference.md
    ├── memory-and-iomap.md
    ├── video.md
    ├── gpu-*.md       ← PICA200 GPU registers and pipeline
    ├── config-*.md    ← System configuration registers
    ├── crypto-*.md    ← Cryptography hardware (AES, SHA, RSA)
    ├── dma-*.md       ← DMA controllers
    ├── i2c-*.md       ← I2C bus devices (MCU, gyroscope, IR, etc.)
    ├── spi-*.md       ← SPI bus devices
    ├── files-*.md     ← File formats (NCCH, NCSD, CIA, CTR, etc.)
    ├── sound-and-microphone.md
    ├── cartridge-registers.md
    ├── interrupts-and-timers.md
    ├── arm9-interrupts.md
    ├── arm11-interrupts.md
    └── ...
```

## Quick Reference for 3dslibris Optimization

### Most Relevant Sections for an Ebook Reader

| Area | NDS Reference | 3DS Reference | What It Covers |
|------|--------------|---------------|----------------|
| **Memory** | `memory-maps.md`, `memory-control.md` | `memory-and-iomap.md`, `config-config11-registers.md` | RAM layout, cache control, memory timing |
| **Display** | `video.md` | `video.md`, `video-lcd-registers.md`, `gpu-external-registers-top-bottom-screen-and-framebuffer-setup.md` | Framebuffers, screen controllers, VRAM |
| **Touch Screen** | `system-peripherals.md` | `tsc-registersummary.md` | Touch input handling |
| **SD Card / Storage** | `sd-mmc-protocol.md`, `sd-mmc-filesystem.md` | `files-emmc-partitions.md` | File I/O, FAT access patterns |
| **CPU / Cache** | `memory-control.md` | `config-l2c310-level2-cache-controller-new.md` | L2 cache on New 3DS, ARM11/ARM9 |
| **DMA** | `memory-control.md` (DMA section) | `dma-registers.md`, `dma-ndma-registers.md` | Hardware-accelerated memory transfers |
| **Interrupts** | `io-maps.md` | `interrupts-and-timers.md`, `arm11-interrupts.md` | IRQ handling, timer-based operations |
| **Sound** | `sound.md` | `sound-and-microphone.md` | Audio output (not critical for reader) |

### Key Optimization Targets

1. **Framebuffer management** — 3DS dual-screen rendering, GPU external registers for top/bottom screen setup
2. **L2 Cache** — New 3DS has L2 cache controller that can significantly improve memory throughput
3. **DMA transfers** — Hardware DMA for bulk memory operations (loading book data, rendering pages)
4. **SD card access** — Understanding eMMC/SD protocol for faster file reads
5. **Touch screen latency** — TSC registers for responsive touch input
6. **Memory timing** — Config registers that affect RAM access speed

## Source Attribution

All documentation in this directory is extracted from:
- **GBATek**: https://problemkaputt.de/gbatek.htm (by Martin Korth / nocash)
- Extracted from no$gba version 3.06

This documentation is provided for educational and development purposes. GBATek is a community-maintained reference and should be consulted alongside official Nintendo documentation when available.
