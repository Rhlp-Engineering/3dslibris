# Gpuinternalregisterscommandlists

> Source: https://problemkaputt.de/gbatek.htm
> Section: Gpuinternalregisterscommandlists

3DS GPU Internal Registers - Command Lists 
| |
| --- |

 Commands (aka GPU registers writes) can be done by manually writing parameters to register 10401000h-10401FFFh, or by using command lists.
 With the command lists, the GPU does automatically read parameters and command numbers from memory (and automatically pauses reading when it is busy).
 
 **104018E8h - PICA(023Ah) - GPUREG_CMDBUF_ADDR0 (aka entrypoint) (R/W)**
 **104018ECh - PICA(023Bh) - GPUREG_CMDBUF_ADDR1 (aka entrypoint) (R/W)**
 
```

| 0 Unused (0)
 1-28 Physical address of 1st/2nd command buffer, in 16-byte units |
| --- |

**104018E0h - PICA(0238h) - GPUREG_CMDBUF_SIZE0 (R/W)**

**104018E4h - PICA(0239h) - GPUREG_CMDBUF_SIZE1 (R/W)**

The size value is required to indicate the end of the command list
(alternately, if GPUREG_IRQ_AUTOSTOP is enabled, command list execution will
also terminate when triggering an IRQ via FINALIZE command; which is usually
stored at end of command list).

```

| 0 Unused (0)
 1-20 Size of command buffer, in 16-byte units |
| --- |

Observe that commands are multiples of 8 bytes (so one may need to append a
dummy 8-byte command for the 16-byte size boundary; unknown if there's a NOP
command for that purpose; reportedly FINALIZE command(s) are used for padding,
though that could trigger multiple IRQs when GPUREG_IRQ_AUTOSTOP is disabled).

**104018F0h - PICA(023Ch) - GPUREG_CMDBUF_JUMP0 (jump to ADDR0) (W)**

**104018F4h - PICA(023Dh) - GPUREG_CMDBUF_JUMP1 (jump to ADDR0) (W)**

```

| 0-31 Don't care (writing any value starts CMDBUF execution, at ADDR0/1) |
| --- |

Executes the Command List, starting at CMDBUF_ADDR0/1, until reaching
CMDBUF_SIZE0/1 (or until FINALIZE+AUTOSTOP).

Unknown why there are two command lists, supposedly they cannot execute
simultaneously... maybe the second list is automatically started at the end of
the other list?

**Command List entries**

Each entry in the command list does contain a "Command Header", and one or more
parameter words, the entry size must be a multiple of 8 bytes.

```

| 00h 4 1st Parameter word
 04h 4 Command Header (see below)
 08h 4 2nd Parameter word (if any)
 0Ch 4 3rd Parameter word (if any)
 ... .. Nth Parameter word (if any)
 xxh 4 Padding (zero) (if above wasn't a multiple of 8 bytes) |
| --- |

Command Header:

```

| 0-15 Command ID, PICA(0000h..03FFh)
 16 Parameter mask, update parameter bit0-7 (0=Don't change, 1=Write)
 17 Parameter mask, update parameter bit8-15 (0=Don't change, 1=Write)
 18 Parameter mask, update parameter bit16-23 (0=Don't change, 1=Write)
 19 Parameter mask, update parameter bit24-31 (0=Don't change, 1=Write)
 20-27 Number of parameter words-1 (0..FFh = 1..256 parameter words)
 28-30 Unused
 31 Consecutive writing mode (0=Fixed Command number, 1=Increasing) |
| --- |