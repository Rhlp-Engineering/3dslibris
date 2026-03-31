# Gputriangledrawingsamplecode

> Source: https://problemkaputt.de/gbatek.htm
> Section: Gputriangledrawingsamplecode

3DS GPU Triangle Drawing Sample Code 
| |
| --- |

 
```

| _________________________ Triangle Drawing Examples __________________________ |
| --- |

**gpu_draw_triangle_direct: ;simple, but requires a messy 4x24bit data format**

```

| gpu_clear_colorbuf, gpu_init_framebuf, gpu_init_vertex_shader
 ;[GPUREG_GEOSTAGE_CONFIG]=((1 shl 8)+00h) ;not needed
 [GPUREG_GEOSTAGE_CONFIG2]=(0 shl 8)+01h ;needed
 ;[GPUREG_START_DRAW_FUNC0]=00h ;not needed, 0=draw, 1=config?
 [GPUREG_FIXEDATTRIB_INDEX]=0Fh ;mode for directly writing vertex data...
 for i=0 to (4*2)-1
 [GPUREG_FIXEDATTRIB_DATA+0]=[@@array+i*0Ch+8] ;\write 4x24bit as 3x32bit
 [GPUREG_FIXEDATTRIB_DATA+0]=[@@array+i*0Ch+4] ; upper 32bit written first
 [GPUREG_FIXEDATTRIB_DATA+0]=[@@array+i*0Ch+0] ;/
 next i
 gpu_copy_colorbuf_to_lcd_framebuf
 ret
 @@array: ;x______y______z______w______r______g______b______a_____
 .float24 -1.0 ,-1.0 ,0.0 ,1.0 ,1.0 ,0.5 ,0.5 ,1.0 ;\1st triangle
 .float24 1.0 ,0.4 ,0.0 ,1.0 ,0.5 ,1.0 ,0.5 ,0.5 ; ;\2nd
 .float24 0.2 ,1.0 ,0.0 ,1.0 ,0.5 ,0.5 ,1.0 ,0.5 ;/ ; triangle
 .float24 1.0 ,-0.8 ,-1.0 ,1.0 ,0.5 ,0.5 ,1.0 ,0.5 ;/(of strip)
 ;note: coord.W must be 1.0 (to avoid perspective division) |
| --- |

**gpu_draw_triangle_via_buffer: ;example with 1-2 arrays, optional index_list**

```

| @@numattr equ 2 ;configure for two attributes (coordinate and color)
 gpu_clear_colorbuf, gpu_init_framebuf, gpu_init_vertex_shader
 [GPUREG_ATTR_BUF_BASE]=(((@@base)/10h)*2)
 [GPUREG_ATTR_BUF_FORMAT_LOW]=9Ch ;Signed8bit(x,y,z,w)+Unsigned8bit(r,g,b)
 [GPUREG_ATTR_BUF_FORMAT_HIGH]=((@@numattr-1) shl 28)+(000h shl 16)
 if @@num_arrays=1 ;both coordinate and color in one array...
 [GPUREG_ATTR_BUFi_OFFSET+0*0Ch]=(@@array-@@base)/1
 [GPUREG_ATTR_BUFi_CONFIG_LOW+0*0Ch]=76543210h
 [GPUREG_ATTR_BUFi_CONFIG_HIGH+0*0Ch]=(@@numattr shl 28)+((4+3) shl 16)+BA98h
 elseif @@num_arrays=2 ;coordinate and color in separate arrays...
 [GPUREG_ATTR_BUFi_OFFSET+0*0Ch]=(@@array0-@@base)/1
 [GPUREG_ATTR_BUFi_OFFSET+1*0Ch]=(@@array1-@@base)/1
 [GPUREG_ATTR_BUFi_CONFIG_LOW+0*0Ch]=00000000h
 [GPUREG_ATTR_BUFi_CONFIG_LOW+1*0Ch]=00000001h
 [GPUREG_ATTR_BUFi_CONFIG_HIGH+0*0Ch]=(01h shl 28)+(04h shl 16)+0000h
 [GPUREG_ATTR_BUFi_CONFIG_HIGH+1*0Ch]=(01h shl 28)+(03h shl 16)+0000h
 endif
 ;[GPUREG_GEOSTAGE_CONFIG] =((1 shl 8)+00h) ;not needed
 [GPUREG_GEOSTAGE_CONFIG2]=(1 shl 8)+01h ;needed
 ;[GPUREG_START_DRAW_FUNC0]=00h ;not needed, 0=draw, 1=config?
 [GPUREG_ATTR_BUF_NUMVERTICES]=4*(1+@@use_index_list) ;needed, array size
 if @@use_index_list ;with index list...
 [GPUREG_ATTR_BUF_INDEX_LIST]=(0 shl 31)+(@@index_list-@@base)/1
 [GPUREG_ATTR_BUF_DRAWELEMENTS]=0 ;start drawing via above index list
 else ;without index_list...
 [GPUREG_ATTR_BUF_FIRST_INDEX]=0 ;first index
 [GPUREG_ATTR_BUF_DRAWARRAYS]=0 ;start drawing at above index number
 endif
 gpu_copy_colorbuf_to_lcd_framebuf
 ret
 align 10h
 @@base:
 if @@use_index_list
 @@index_list: db 0,1,2,3 ;index to 1st,2nd,3rd,4th entries in arrays
 endif
 if @@num_arrays=1
 @@array: ;x______y______z______w______r______g______b______a_____
 db -7fh ,-7fh ,0 ,+7fh ,0ffh ,0 ,0 ;\1st triangle
 db +7fh ,+40h ,0 ,+7fh ,0 ,0ffh ,0 ; ;\2nd
 db +20h ,+7fh ,0 ,+7fh ,0 ,0 ,0ffh ;/ ; triangle
 db +7fh ,-60h ,0 ,+7fh ,0 ,0ffh ,0ffh ;/of strip
 elseif @@num_arrays=2
 @@array0: ;x______y______z______w____
 db -7fh ,-7fh ,0 -9 ,+7fh ;\1st triangle
 db +7fh ,+40h ,0 -9 ,+7fh ; ;\2nd
 db +20h ,+7fh ,0 -9 ,+7fh ;/ ; triangle
 db +7fh ,-60h ,-7fh ,+7fh ;/of strip
 @@array1: ;r______g______b______a_____
 db 0ffh ,0 ,0 ;\1st triangle
 db 0 ,0ffh ,0 ; ;\2nd
 db 0 ,0 ,0ffh ;/ ; triangle
 db 0 ,0ffh ,0ffh ;/of strip
 endif |
| --- |

```

| _____________________ Init and Memory Transfer Functions _____________________ |
| --- |

**gpu_clear_colorbuf:**

```

| [GPUREG_RENDERBUFFER_INVALIDATE]=1 ;forget cache
 [GPU_MEMFILL_DST_ADDR0]=(MEMORG_COLORBUF+0)/10h*2
 [GPU_MEMFILL_DST_END0]=(MEMORG_COLORBUF+320*240*4)/10h*2
 [GPU_MEMFILL_DATA0]=11441100h
 [GPU_MEMFILL_CNT0]=00000201h
 wait until [GPU_MEMFILL_CNT0].bit0=0
 ret |
| --- |

**gpu_copy_colorbuf_to_lcd_framebuf:**

```

| DummyRead=[GPUREG_STAT_NUM_TRIANGLES_DISPLAYED] ;waits until rendering done
 [GPUREG_RENDERBUFFER_FLUSH]=1 ;cache writeback
 [GPU_MEMCOPY_SRC_ADDR]=MEMORG_COLORBUF/10h*2
 [GPU_MEMCOPY_DST_ADDR]=MEMORG_SCREEN1/10h*2
 [GPU_MEMCOPY_DISPLAY_SIZE]=(240 shl 0)+(320 shl 16)
 [GPU_MEMCOPY_REMAIN_IRQ]=3FFFh ;want IRQ flag when done
 [GPU_MEMCOPY_FLAGS]=0
 [GPU_MEMCOPY_CNT]=1
 ;caution: polling GPU_MEMCOPY_CNT can HANG the CPU at transfer end!
 ;instead: poll GPU_STAT_IRQ_FLAGS...
 wait until: [GPU_STAT_IRQ_FLAGS].bit30=1
 ret |
| --- |

**gpu_init_framebuf:**

```

| ;[GPUREG_RENDERBUFFER_INVALIDATE]=1 ;forget cache
 [GPUREG_FACECULLING_CONFIG]=0 ;show front+back
 [GPUREG_RENDERBUFFER_DIM_0]=(1 shl 24)+(320-1)*1000h+240
 ;[GPUREG_RENDERBUFFER_DIM_1]=(1 shl 24)+(320-1)*1000h+240
 [GPUREG_DEPTHBUFFER_LOC]=MEMORG_DEPTHBUF/40h*8
 [GPUREG_COLORBUFFER_LOC]=MEMORG_COLORBUF/40h*8
 [GPUREG_VIEWPORT_V_SCALE]=0045E000h ;240/2 ;.float24 120.0 // db 00h
 [GPUREG_VIEWPORT_H_SCALE]=00464000h ;320/2 ;.float24 160.0 // db 00h
 [GPUREG_VIEWPORT_V_STEP]=38111100h ;2/240 ;db 00h // .float24 0.008333333
 [GPUREG_VIEWPORT_H_STEP]=37999900h ;2/320 ;db 00h // .float24 0.006250000
 [GPUREG_VIEWPORT_XY]=(0 shl 16)+0
 [GPUREG_SCISSORTEST_MODE]=0
 ;[GPUREG_SCISSORTEST_POS1]=((0+20) shl 16)+(0+20)
 ;[GPUREG_SCISSORTEST_POS2]=((320-1-20) shl 16)+(240-1-20)
 [GPUREG_COLOR_OPERATION]=00e40100h
 [GPUREG_BLEND_FUNC]=06020000h ;raw drawing
 ;[GPUREG_BLEND_FUNC]=76760000h ;alpha blending
 ;[GPUREG_LOGIC_OP]=00h
 [GPUREG_FRAGOP_ALPHA_TEST]=00h
 [GPUREG_STENCIL_TEST]=00h
 [GPUREG_DEPTH_COLOR_MASK]=1F00h
 [GPUREG_COLORBUFFER_READING]=0Fh
 [GPUREG_COLORBUFFER_WRITING]=0Fh
 [GPUREG_DEPTHBUFFER_READING]=03h
 [GPUREG_DEPTHBUFFER_WRITING]=03h
 [GPUREG_DEPTHBUFFER_FORMAT]=03h
 [GPUREG_COLORBUFFER_FORMAT]=02h
 [GPUREG_RENDERBUFFER_BLOCK32]=0
 ;[GPUREG_EARLYDEPTH_TEST1]=0
 ;[GPUREG_EARLYDEPTH_TEST2]=0
 [GPUREG_DEPTHMAP_ENABLE]=1
 [GPUREG_DEPTHMAP_SCALE]=00bf0000h ;far z (-1.0)
 [GPUREG_DEPTHMAP_OFFSET]=00000000h ;near z (0.0)
 ret |
| --- |

**gpu_init_vertex_shader:**

```

| @@numattr equ 2 ;configure for two attributes (coordinate and color)
 [GPUREG_SH_OUTMAP_TOTAL]=@@numattr ;vertex+color
 [GPUREG_SH_OUTMAP_O+0*4]=03020100h ;vertex.xyzw
 [GPUREG_SH_OUTMAP_O+1*4]=0b0a0908h ;color.rgba
 [GPUREG_SH_OUTATTR_CLOCK]=3 ;bit0=blah?, bit1=needed for colors
 [GPUREG_VSH_COM_MODE]=00h ;GSH/VSH?
 [GPUREG_VSH_NUM_ATTR]=@@numattr-1 ;needed
 [GPUREG_VSH_OUTMAP_TOTAL1]=@@numattr-1 ;needed
 ;[GPUREG_VSH_OUTMAP_TOTAL2]=@@numattr-1 ;not needed
 [GPUREG_START_DRAW_FUNC0]=1 ;needed
 [GPUREG_VSH_INPUTBUFFER_CONFIG]=(0a0h shl 24)+@@numattr-1
 [GPUREG_PRIMITIVE_CONFIG]=(1 shl 8)+@@numattr-1
 [GPUREG_RESTART_PRIMITIVE]=0 ;required for strips (even BEFORE first strip)
 [GPUREG_VSH_ATTR_PERMUTATION_LOW]=076543210h ;\input
 [GPUREG_VSH_ATTR_PERMUTATION_HIGH]=0fedcba98h ;/
 [GPUREG_VSH_OUTMAP_MASK]=(1 shl @@numattr)-1 ;-output enable's
 [GPUREG_VSH_CODETRANSFER_INDEX]=000h/4
 [GPUREG_VSH_CODETRANSFER_DATA+0]=4C000000h ;MOV o0,v0 ;used for coord xyzw
 [GPUREG_VSH_CODETRANSFER_DATA+0]=4C201000h ;MOV o1,v1 ;used for color rgba
 [GPUREG_VSH_CODETRANSFER_DATA+0]=88000000h ;END
 [GPUREG_VSH_CODETRANSFER_END]=0
 [GPUREG_VSH_OPDESCS_INDEX]=000h
 [GPUREG_VSH_OPDESCS_DATA+0]=0000036Fh ;dst=xyzw, src1=xyzw (or rgba)
 [GPUREG_VSH_ENTRYPOINT]=7fff0000h+(000h/4)
 ret |
| --- |