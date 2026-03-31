# Video

> Source: https://problemkaputt.de/gbatek.htm
> Section: Video

3DS Video 
| |
| --- |

 **LCD Registers**
 These registers are used to configure the LCD screens.
 3DS Video LCD Registers
 Moreover, each screen is having an I2C-bus controller for further configuration.
 3DS I2C LCD Screen Controllers
 And, the MCU has enable flags for LCD Power Supplies and LCD Backlights.
 
 **GPU Memory and I/O Map**
 3DS GPU Memory and I/O Map
 3DS GPU External Register List Summary
 3DS GPU Internal Register List Summary
 
 **GPU External Registers (for memory transfers, and framebuf to LCD output)**
 3DS GPU External Registers - Memory Control/Status Registers
 3DS GPU External Registers - Top/Bottom Screen and Framebuffer Setup
 3DS GPU External Registers - Memfill and Memcopy
 
 **GPU Internal Registers (PICA200) (for drawing polygons to colorbuf/depthbuf)**
 3DS GPU Internal Register Overview
 3DS GPU Internal Registers - Command Lists
 3DS GPU Internal Registers - Finalize Interrupt registers
 3DS GPU Internal Registers - Geometry Pipeline registers
 3DS GPU Internal Registers - Shader registers
 3DS GPU Internal Registers - Rasterizer registers
 3DS GPU Internal Registers - Framebuffer registers
 3DS GPU Internal Registers - Texturing registers (Generic Textures)
 3DS GPU Internal Registers - Texturing registers (Procedural Texture)
 3DS GPU Internal Registers - Texturing registers (Environment)
 3DS GPU Internal Registers - Fragment Lighting registers
 3DS GPU Internal Registers - Unknown/Unused/Undocumented Registers
 3DS GPU Shader Instruction Set - Opcode Summary
 3DS GPU Shader Instruction Set - Blurp
 Misc notes...
 3DS GPU Geometry Pipeline
 3DS GPU Fragment Lighting
 3DS GPU Pitfalls
 3DS GPU Primitive Engine and Shaders
 3DS GPU Texture Formats
 3DS GPU Texture Types
 There is also some 3DS GPU open source driver called citro3d, but it's mostly higher level stuff and would require a lot of reverse engineering to extract the lower level functionality from it.
 
 **Sample Code**
 3DS GPU Triangle Drawing Sample Code
 
 **Camera and Video Data Conversion**
 3DS Video CAM Registers (Camera Input)
 3DS Video Y2R Registers (YUV-to-RGBA Converter)
 3DS Video L2B Registers (RGB-to-RGBA Converter) (New3DS)
 3DS Video MVD Registers (Movie Decoder or so?) (New3DS)
 3DS Video LGY Registers (Legacy GBA/NDS Video to Framebuffer)