# Gpuinternalregistersunknownunusedundocumentedregisters

> Source: https://problemkaputt.de/gbatek.htm
> Section: Gpuinternalregistersunknownunusedundocumentedregisters

3DS GPU Internal Registers - Unknown/Unused/Undocumented Registers 
| |
| --- |

 **Unused Registers that Mirror to GPUREG_RESTART_PRIMITIVE (R/W)**
 
```

| 104011C0h 40h ;PICA(0070h..007Fh) mirrors to GPUREG_RESTART_PRIMITIVE
 104013F8h 08h ;PICA(00FEh..00FFh) mirrors to GPUREG_RESTART_PRIMITIVE
 1040149Ch 24h ;PICA(0127h..012Fh) mirrors to GPUREG_RESTART_PRIMITIVE
 10401780h 80h ;PICA(01E0h..01FFh) mirrors to GPUREG_RESTART_PRIMITIVE
 10401900h 08h ;PICA(0240h..0241h) mirrors to GPUREG_RESTART_PRIMITIVE
 10401918h 10h ;PICA(0246h..0249h) mirrors to GPUREG_RESTART_PRIMITIVE
 1040192Ch 14h ;PICA(024Bh..024Fh) mirrors to GPUREG_RESTART_PRIMITIVE
 10401960h 18h ;PICA(0258h..025Dh) mirrors to GPUREG_RESTART_PRIMITIVE
 10401980h 80h ;PICA(0260h..027Fh) mirrors to GPUREG_RESTART_PRIMITIVE
 10401D00h 300h ;PICA(0340h..03FFh) mirrors to GPUREG_RESTART_PRIMITIVE |
| --- |

The mirrors (and GPUREG_RESTART_PRIMITIVE itself) are R/W, however, writing
doesn't always work for some reason, or the write is applied only after issuing
another three writes or so? That may be due to some cache or write buffer in
the CPU or GPU.

**Unused or Write-Only Registers that return Garbage/Mirrors when reading**

104018BCh..104018DCh and 104018F0h..104018FCh do return mirrors of multiple
other registers ORed together.

```

| Note: below ORing occurred on New3DS XL, but Old3DS behaves differently
 104018BCh = 1040183Ch,1040189Ch,104018B4h ;ATTR_BUF_DRAWELEMENTS
 104018C0h = 10401840h,104018E0h,10401880h ;unused (or write-only?)
 104018C4h = 10401844h,104018E4h,10401884h ;VTX_FUNC
 104018C8h = 10401848h,104018E8h,10401888h ;FIXEDATTRIB_INDEX
 104018CCh = 1040184Ch,104018ECh,1040188Ch,1040189Ch ;FIXEDATTRIB_DATA0
 104018D0h = 10401850h,104018E0h,10401890h ;FIXEDATTRIB_DATA1
 104018D4h = 10401854h,104018E4h,10401894h ;FIXEDATTRIB_DATA2
 104018D8h = 10401858h,104018E8h,10401898h ;unused (or write-only?)
 104018DCh = 1040185Ch,104018ECh ;unused (or write-only?)
 104018F0h = 10401870h,104018E0h ;GPUREG_CMDBUF_JUMP0
 104018F4h = 10401874h,104018E4h,104018B4h ;GPUREG_CMDBUF_JUMP1
 104018F8h = 10401878h,104018E8h ;unused (or write-only?)
 104018FCh = 1040187Ch,104018ECh,104018B4h,1040189Ch ;unused (or write-only?) |
| --- |

10401940h, 10401958h, 1040195Ch seem to mirror to single registers without
ORing multiple values.

```

| 10401940h = 10401944h (GPUREG_VSH_OUTMAP_TOTAL2) ;unused (or write-only?)
 10401958h = 10401950h (GPUREG_GSH_MISC1) ;unused (or write-only?)
 1040195Ch = 1040194Ch (GPUREG_GEOSTAGE_CONFIG2) ;unused (or write-only?) |
| --- |

**Unused or Write-Only Registers that return Zero when reading**

```

| 1040118Ch = zero ;EARLYDEPTH_CLEAR
 10401440h = zero ;RENDERBUFFER_INVALIDATE
 10401444h = zero ;RENDERBUFFER_FLUSH
 1040148Ch = zero ;GAS_LUT_INDEX
 104018B8h = zero ;ATTR_BUF_DRAWARRAYS
 10401A14h/10401AD4h/10401B94h/10401C54h = zero ;unused (or write-only?)
 10401A18h/10401AD8h/10401B98h/10401C58h = zero ;unused (or write-only?)
 10401A1Ch/10401ADCh/10401B9Ch/10401C5Ch = zero ;unused (or write-only?)
 10401A20h/10401AE0h/10401BA0h/10401C60h = zero ;unused (or write-only?)
 10401A38h/10401AF8h/10401BB8h/10401C78h = zero ;unused (or write-only?)
 10401A3Ch/10401AFCh/10401BBCh/10401C7Ch = zero ;CODETRANSFER_END
 10401A40h/10401B00h/10401BC0h/10401C80h = zero ;FLOATUNIFORM_INDEX
 10401A44h/10401B04h/10401BC4h/10401C84h+i*4 = zero ;FLOATUNIFORM_DATA0-7
 10401A64h/10401B24h/10401BE4h/10401CA4h = zero ;unused (or write-only?)
 10401A68h/10401B28h/10401BE8h/10401CA8h = zero ;unused (or write-only?)
 10401A6Ch/10401B2Ch/10401BECh/10401CACh = zero ;CODETRANSFER_INDEX
 10401A70h/10401B30h/10401BF0h/10401CB0h+i*4 = zero ;CODETRANSFER_DATA0-7
 10401A90h/10401B50h/10401B10h/10401CD0h = zero ;unused (or write-only?)
 10401A94h/10401B54h/10401C14h/10401CD4h = zero ;OPDESCS_INDEX
 10401A98h/10401B58h/10401C18h/10401CD8h+i*4 = zero ;OPDESCS_DATA0..7
 10401AB8h/10401B78h/10401C38h/10401CF8h = zero ;unused (or write-only?)
 10401ABCh/10401B7Ch/10401C3Ch/10401CFCh = zero ;unused (or write-only?) |
| --- |

**Unused Registers that return Zero when reading**

These registers aren't R/W, and they seem to be always zero (though some might
be undocumented read-only or write-only registers).

```

| 104010D8h 28h ;PICA(N/A) unused (0) ;-Finalize
 1040115Ch 4 ;PICA(0057h) unused (0)
 10401174h 0Ch ;PICA(005Dh..005Fh) unused (0)
 10401240h 4 ;PICA(0090h) unused (0) ;XXX used, write-only?
 1040125Ch 4 ;PICA(0097h) unused (0) ;XXX used, write-only?
 10401260h 4 ;PICA(0098h) unused (0) ;XXX used, write-only?
 1040127Ch 24h ;PICA(009Fh..00A7h) unused (0)
 104012E0h 20h ;PICA(00B8h..00BFh) unused (0)
 10401314h 08h ;PICA(00C5h..00C6h) unused (0) ;-TexEnv0
 10401334h 0Ch ;PICA(00CDh..00CFh) unused (0) ;-TexEnv1
 10401354h 0Ch ;PICA(00D5h..00D7h) unused (0) ;-TexEnv2
 10401374h 0Ch ;PICA(00DDh..00DFh) unused (0) ;-TexEnv3
 1040139Ch 4 ;PICA(00E7h) unused (0)
 104013D4h 0Ch ;PICA(00F5h..00F7h) unused (0) ;-TexEnv4
 10401420h 14h ;PICA(0108h..010Ch) unused (0)
 104014C4h 38h ;PICA(0131h..013Eh) unused (0)
 10401520h 4 ;PICA(0148h) unused (0) ;\LIGHT0
 10401530h 10h ;PICA(014Ch..014Fh) unused (0) ;/
 10401560h 4 ;PICA(0158h) unused (0) ;\LIGHT1
 10401570h 10h ;PICA(015Ch..015Fh) unused (0) ;/
 104015A0h 4 ;PICA(0168h) unused (0) ;\LIGHT2
 104015B0h 10h ;PICA(016Ch..016Fh) unused (0) ;/
 104015E0h 4 ;PICA(0178h) unused (0) ;\LIGHT3
 104015F0h 10h ;PICA(017Ch..017Fh) unused (0) ;/
 10401620h 4 ;PICA(0188h) unused (0) ;\LIGHT4
 10401630h 10h ;PICA(018Ch..018Fh) unused (0) ;/
 10401660h 4 ;PICA(0198h) unused (0) ;\LIGHT5
 10401670h 10h ;PICA(019Ch..019Fh) unused (0) ;/
 104016A0h 4 ;PICA(01A8h) unused (0) ;\LIGHT6
 104016B0h 10h ;PICA(01ACh..01AFh) unused (0) ;/
 104016E0h 4 ;PICA(01B8h) unused (0) ;\LIGHT7
 104016F0h 10h ;PICA(01BCh..01BFh) unused (0) ;/
 10401704h 4 ;PICA(01C1h) unused (0) ;\
 1040171Ch 4 ;PICA(01C7h) unused (0) ; LIGHTING
 10401768h 18h ;PICA(01DAh..01DFh) unused (0) ;/
 104018ACh 08h ;PICA(022Bh..022Ch) unused (0) ;-VERTEX |
| --- |

**Unused command numbers PICA(0000h..000Fh, 0020h..003Fh)**

Registers at 10401000h..1040103Fh and 10401080h..104010FFh are writeable by
ARM11 only, not via PICA command numbers 0000h..000Fh, 0020h..003Fh.

**Unused command numbers PICA(0400h..FFFFh)**

The PICA command numbers in CMDBUF are said to be 16bit wide, however, values
0400h..FFFFh seem to be unused (and there are no I/O addresses for them;
accessing 10402000h and up triggers data abort).