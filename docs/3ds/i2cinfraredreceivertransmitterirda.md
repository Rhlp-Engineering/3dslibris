# I2Cinfraredreceivertransmitterirda

> Source: https://problemkaputt.de/gbatek.htm
> Section: I2Cinfraredreceivertransmitterirda

3DS I2C Infrared Receiver/Transmitter (IrDA) 
| |
| --- |

 NXP SC16IS750
 Single UART with I2C-bus/SPI interface, 64 bytes of transmit and receive FIFOs, IrDA SIR built-in support - Rev. 06 - 13 May 2008 Product data sheet
 I2C Device 2:9Ah
 
 **Register address byte (I2C)**
 
```

| 7 Unused (0)
 6-3 UART's internal register select
 2-1 Channel select (0) ;other values are reserved and should not be used.
 0 Unused (0) |
| --- |

**Register set**

```

| Normal register set (default mapping)
 00h.DLAB=0.R RHR Receive Holding Register
 00h.DLAB=0.W THR Transmit Holding Register
 08h.DLAB=0.R/W IER Interrupt Enable Register
 10h.W FCR FIFO Control Register
 10h.R IIR Interrupt Identification Register
 18h.R/W LCR Line Control Register
 20h.R/W MCR Modem Control Register
 28h.R LSR Line Status Register
 30h.normal.R MSR Modem Status Register
 38h.normal.R/W SPR Scratchpad Register
 40h.R TXLVL Transmitter FIFO Level
 48h.R RXLVL Receiver FIFO Level
 50h.R/W IODir Programmable I/O pins Direction ;\IO only on
 58h.R/W IOState Programmable I/O pins State ; SC16IS750
 60h.R/W IOIntEna I/O Interrupt Enable Register ; and
 68h reserved Reserved (00h) ; SC16IS760
 70h.R/W IOControl I/O Control register ;/
 78h.R/W EFCR Extra Features Control Register
 Special/Enhanced registers (mapped depending on LCR/MCR/EFR bits):
 00h.DLAB=1.R/W DLL Baudrate Divisor Latch Low ;\when LCR.bit7=1
 08h.DLAB=1.R/W DLH Baudrate Divisor Latch High ;/and LCR<>BFh
 10h.LCR=BFh.R/W EFR Enhanced Features Register ;\
 20h.LCR=BFh.R/W XON1 Xon1 word ; when
 28h.LCR=BFh.R/W XON2 Xon2 word ; LCR=BFh
 30h.LCR=BFh.R/W XOFF1 Xoff1 word (whatever, maybe OUTGOING char?) ;
 38h.LCR=BFh.R/W XOFF2 Xoff2 word (compare for INCOMING chars) ;/
 30h.special.R/W TCR Transmission Control Register ;\when MCR.bit2=1
 38h.special.R/W TLR Trigger Level Register ;/and EFR.bit4=1 |
| --- |

[2] These bits in can only be modified if register bit EFR[4] is enabled.

[5] After Receive FIFO or Transmit FIFO reset (through FCR[1:0]), the user must
wait at least 2' ??? Tclk of XTAL1 before reading or writing data to RHR and
THR, respectively.

[8] IrDA mode slow/fast for SC16IS760, slow only for SC16IS750.

**IR[00h.DLAB=0.R] - RHR - Receive Holding Register (R)**

The receiver section consists of the Receiver Holding Register (RHR) and the
Receiver Shift Register (RSR). The RHR is actually a 64-byte FIFO. The RSR
receives serial data from the RX pin. The data is converted to parallel data
and moved to the RHR. The receiver section is controlled by the Line Control
Register. If the FIFO is disabled, location zero of the FIFO is used to store
the characters.

**IR[00h.DLAB=0.W] - THR - Transmit Holding Register (W)**

The transmitter section consists of the Transmit Holding Register (THR) and the
Transmit Shift Register (TSR). The THR is actually a 64-byte FIFO. The THR
receives data and shifts it into the TSR, where it is converted to serial data
and moved out on the TX pin. If the FIFO is disabled, the FIFO is still used to
store the byte. Characters are lost if overflow occurs.

**IR[10h.W] - FCR - FIFO Control Register (W)**

```

| 7-6 Trigger level for the RX FIFO (0..3 = 8,16,56,60 chars)
 5-4 Trigger level for the TX FIFO (0..3 = 8,16,32,56 spaces)
 3 Reserved
 2 TX FIFO Reset (0=No change, 1=Clear TX FIFO)
 1 RX FIFO Reset (0=No change, 1=Clear RX FIFO)
 0 RX/TX FIFO enable (0=Disable, 1=Enable) |
| --- |

Bit1,2: FIFO reset requires at least two XTAL1 clocks, therefore, they cannot
be reset without the presence of the XTAL1 clock.

Bit4-5: Can only be modified and enabled when EFR[4] is set. This is because
the transmit trigger level is regarded as an enhanced function.

**IR[18h] - LCR - Line Control Register (R/W)**

```

| 7 Divisor latch access enable (DLAB) (0=Normal, 1=Access DLL/DLH)
 6 Break control bit (0=Off/Normal, 1=Break/Force TX to logic 0)
 5-4 Parity Type (0=Odd, 1=Even, 2=Forced1, 3=Forced0)
 3 Parity Enable (0=Disable, 1=Enable)
 2 Number of stop bits (0=1bit, 1=2bit, or 1.5bit in 5bit mode)
 1-0 Word length (0=5bit, 1=6bit, 2=7bit, 3=8bit) |
| --- |

**IR[28h.R] - LSR - Line Status Register (R)**

```

| 7 FIFO data error (0=None, 1=At least one Error/Break anywhere in RX FIFO)
 6 THR and TSR empty (0=Not empty, 1=Empty) (TX FIFO and TX shift reg)
 5 THR empty (0=Not empty, 1=Empty) (TX FIFO)
 4 Break interrupt (0=No, 1=Break, next RX FIFO char reads as 00h)
 3 Framing Error (0=No, 1=Error; bad stop bit in data from RX FIFO)
 2 Parity Error (0=No, 1=Error; in data being read from RX FIFO)
 1 Overrun Error (0=No, 1=Error; overrun has occurred)
 0 Data in Receiver (0=No, 1=At least one character in the RX FIFO) |
| --- |

Bit2-4 reflect the error bits (BI, FE, PE) of the character at the top of the
RX FIFO (next character to be read). Therefore, errors in a character are
identified by reading the LSR and then reading the RHR.

**IR[20h] - MCR - Modem Control Register (R/W)**

```

| 7 Clock Divisor (0=Div1 clock input, 1=Div4 clock input)
 6 IrDA mode enable (0=Normal UART mode, 1=IrDA mode) (aka pulse RX/TX?)
 5 Xon Any function (0=Disable, 1=Enable)
 4 Enable Loopback (0=Normal, 1=local Loopback mode, internal)
 In this mode the MCR[1:0] signals are looped back into MSR[4:5] and
 the TX output is looped back to the RX input internally.
 3 Reserved
 2 TCR/TLR register access enable (0=Disable, 1=Enable)
 1 RTS output (if Auto RTS disabled) (0=High/Inactive, 1=Low/Active)
 In Loopback mode, controls MSR[4].
 0 DTR output (GPIO5) (if IOControl.bit1=1) (0=High/Inactive, 1=Low/Active)
 Writing to IOState bit 5 will not have any effect on this pin. |
| --- |

Bit2,5-7 can only be modified when EFR[4] is set, ie. EFR[4] is a write enable.

Bit0: Only available on SC16IS750/SC16IS760.

**IR[30h.normal.R] - MSR - Modem Status Register (R)**

```

| 7 CD input state (inverted, 1=Not HIGH, or so) ;\only if modem mode
 6 RI input state (inverted, 1=Not HIGH, or so) ; (IOControl.bit1=1)
 5 DSR input state (inverted, 1=Not HIGH, or so) ;/
 4 CTS input state (inverted, 1=Not HIGH, or so)
 3 CD input changed state (0=No, 1=Yes) ;\
 2 RI input changed state from LOW to HIGH (0=No, 1=Yes) ; cleared
 1 DSR input changed state (0=No, 1=Yes) ; on read
 0 CTS input changed state (0=No, 1=Yes) ;/ |
| --- |

Bit1-3,5-7: Only available on SC16IS750/SC16IS760.

Remark: The primary inputs RI, CD, CTS, DSR are all active LOW.

Reading IOState bit 6,6,4 does not reflect the true state of CD,RI,DSR pin, uh?

**IR[08h] - IER - Interrupt Enable Register (R/W)**

```

| 7 CTS change interrupt enable (0=Disable, 1=Enable)
 6 RTS change interrupt enable (0=Disable, 1=Enable)
 5 Xoff received interrupt (0=Disable, 1=Enable)
 4 Sleep mode (0=Disable, 1=Sleep Mode)
 3 Modem Status Interrupt (0=Disable, 1=Enable)
 2 Receive Line Status error interrupt (0=Disable, 1=Enable)
 1 Transmit Holding Register interrupt (0=Disable, 1=Enable)
 0 Receive Holding Register interrupt (0=Disable, 1=Enable) |
| --- |

Bit4-7 can only be modified if EFR[4] is set, that is, EFR[4] is a write
enable. Re-enabling IER[1] will not cause a new interrupt if the THR is below
the threshold.

Bit3: Only available on the SC16IS750/SC16IS760.

Bit3: See IOControl.bit1 for the description of how to program the pins as
modem pins.

**IR[10h.R] - IIR - Interrupt Identification Register (R)**

```

| 7 Mirror of FCR.bit0: RX/TX FIFO enable (0=Disable, 1=Enable) ;\both same
 6 Mirror of FCR.bit0: RX/TX FIFO enable (0=Disable, 1=Enable) ;/
 5-1 Interrupt source (5bit encoded, see below) (valid when bit0=0)
 0 Interrupt status (0=IRQ pending, 1=No IRQ pending) |
| --- |

Interrupt sources:

```

| Src Prio Expl.
 03h 1 Receiver Line Status error
 06h 2 Receiver time-out interrupt
 02h 2 RHR interrupt
 01h 3 THR interrupt
 00h 4 MSR Modem interrupt (SC16IS750/SC16IS760 only)
 18h 5 IOState GPIO input pin change (SC16IS750/SC16IS760 only)
 08h 6 received Xoff signal/special character
 10h 7 CTS, RTS change state from active(LOW) to inactive(HIGH) |
| --- |

Burst reads on the serial interface should not be performed on the IIR register
(ie. don't read multiple elements on the I2C-bus without a STOP or repeated
START condition, and don't read multiple elements on the SPI bus without
de-asserting the CS pin).

**IR[10h.LCR=BFh] - EFR - Enhanced Features Register (R/W)**

```

| 7 CTS flow control enable (0=Disable, 1=Enable; stop TX upon CTS=HIGH)
 6 RTS flow control enable (0=Disable, 1=Enable; change RTS upon TCR)
 RTS pin goes HIGH when RX FIFO halt trigger level TCR[3:0] is reached
 RTS pin goes LOW when RX FIFO resume trigger level TCR[7:4] is reached
 5 Special character detect (0=Disable, 1=Enable, compare with Xoff2)
 If a Xoff2 match occurs, the received data is transferred to FIFO and
 IIR.bit4 is set to 1 to indicate a special character has been detected
 4 Enhanced functions enable bit (0=Disable, 1=Enable)
 Enables writing to IER[7:4], FCR[5:4], MCR[7:5]
 3-0 Combinations of software flow control can be selected by programming
 these bits. See Table 3 "Software flow control options (EFR[3:0])". |
| --- |

**IR[00h.DLAB=1] - DLL - Baudrate Divisor Latch Register Low, bit0-7 (R/W)**

**IR[08h.DLAB=1] - DLH - Baudrate Divisor Latch Register High, bit8-15 (R/W)**

```

| 15-0 16bit divisor for generation of the clock in the baudrate generator |
| --- |

Remark: DLL and DLH can only be written to before Sleep mode is enabled, that
is, before IER[4] is set.

**IR[30h.MCR[2]=1 and EFR[4]=1] - TCR - Transmission Control Register (R/W)**

```

| 7-4 RX FIFO flow control resume trigger level (N*4) (0..14 = 0..56 chars)
 3-0 RX FIFO flow control halt trigger level (N*4) (1..15 = 4..60 chars) |
| --- |

RX FIFO threshold levels to stop/start transmission during hardware/software
flow control (ie. for RTS pin, or Xon/Xoff chars).

Remark: TCR can only be written to when EFR[4]=1 and MCR[2]=1.

The programmer must program the TCR such that Halt>Resume, aka
TCR[3:0]>TCR[7:4]. There is no built-in hardware check to make sure this
condition is met.

Also, the TCR must be programmed with this condition before auto RTS or
software flow control is enabled to avoid spurious operation of the device.

**IR[38h.MCR[2]=1 and EFR[4]=1] - TLR - Trigger Level Register (R/W)**

```

| 7-4 RX FIFO interrupt trigger level (N*4) (1..15 = 4..60 chars available)
 3-0 TX FIFO interrupt trigger level (N*4) (1..15 = 4..60 spaces available) |
| --- |

Remark: TLR can only be written to when EFR[4]=1 and MCR[2]=1. If TLR[3:0] or
TLR[7:4] are logical 0, the selectable trigger levels via the FIFO Control
Register (FCR) are used for the transmit and receive FIFO trigger levels.
Trigger levels from 4 characters to 60 characters are available with a
granularity of four. The TLR should be programmed for NÏ4, where N is the
desired trigger level.

When the trigger level setting in TLR is zero, the SC16IS740/750/760 uses the
trigger level setting defined in FCR. If TLR has non-zero trigger level value,
the trigger level defined in FCR is discarded. This applies to both transmit
FIFO and receive FIFO trigger level setting.

When TLR is used for RX trigger level control, FCR[7:6] should be left at the
default state, that is, '00'.

**IR[40h.R] - TXLVL - Transmitter FIFO Level register (R)**

```

| 7 Unused (0)
 6-0 Number of spaces available in TX FIFO (00h..40h; 00h=Full, 40h=Empty) |
| --- |

**IR[48h.R] - RXLVL - Receiver FIFO Level register (R)**

```

| 7 Unused (0)
 6-0 Number of characters stored in RX FIFO (00h..40h; 00h=Empty, 40h=Full) |
| --- |

**IR[50h] - IODir - Programmable I/O pins Direction register (R/W)**

This register is only available on the SC16IS750 and SC16IS760.

```

| 7-0 IODir set GPIO7-0 pins to input or output (0=Input, 1=Output) |
| --- |

Remark: If there is a pending input (GPIO) interrupt and IODir is written, this
pending interrupt will be cleared, that is, the interrupt signal will be
negated.

**IR[58h] - IOState - Programmable I/O pins State Register (R/W)**

```

| 7-0 IOState Write this register: set the logic level on the output pins
 0=set output pin to zero
 1=set output pin to one
 Read this register: return states of all pins |
| --- |

This register is only available on the SC16IS750 and SC16IS760.

When read, this register returns the actual state of all I/O pins.

When write, each register bit will be transferred to the corresponding IO pin
programmed as output.

**IR[60h] - IOIntEna - I/O Interrupt Enable Register (R/W)**

```

| 7-0 Input change interrupt enable (0=Disable, 1=Enable) |
| --- |

This register is only available on the SC16IS750 and SC16IS760.

This register enables the interrupt due to a change in the I/O configured as
inputs. If GPIO4-7 are programmed as modem pins, their interrupt generation
must be enabled via IER register bit3. In this case bit4-7 of IOIntEna will
have no effect on GPIO4-7.

**IR[70h] - IOControl - I/O Control register (R/W)**

```

| 7-4 Reserved
 3 Software reset (0=No change, 1=Reset) (W)
 2 Reserved
 1 Use GPIO4-7 as GPIO or as modem pins (0=GPIO, 1=RI,CD,DTR,DSR)
 0 IOLATCH enable/disable inputs latching
 0 = input values are not latched. A change in any input generates an
 interrupt. A read of the input register clears the interrupt. If the
 input goes back to its initial logic state before the input register
 is read, then the interrupt is cleared.
 1 = input values are latched. A change in the input generates an
 interrupt and the input logic value is loaded in the bit of the
 corresponding input state register (IOState). A read of the IOState
 register clears the interrupt. If the input pin goes back to its
 initial logic state before the interrupt register is read, then the
 interrupt is not cleared and the corresponding bit of the IOState
 register keeps the logic value that initiates the interrupt. |
| --- |

This register is only available on the SC16IS750 and SC16IS760.

Remark: As I/O pins, the direction, state, and interrupt of GPIO4-7 are
controlled by the following registers: IODir, IOState, IOIntEna, and IOControl.
The state of CD, RI, DSR pins will not be reflected in MSR[7:5] or MSR[3:1],
and any change of state on these three pins will not trigger a modem status
interrupt (even if enabled via IER[3]), and the state of the DTR pin cannot be
controlled by MCR[0].

As modem CD, RI, DSR pins, the status at the input of these three pins can be
read from MSR[7:5] and MSR[3:1], and the state of DTR pin can be controlled by
MCR[0]. Also, if modem status interrupt bit is enabled, IER[3], a change of
state of RI, CD, DSR pins will trigger a modem interrupt. Bit[7:4] of the
IODir, IOState, and IOIntEna registers will not have any effect on these three
pins.

**IR[78h] - EFCR - Extra Features Control Register (R/W)**

```

| 7 IrDA pulse mode (0=Max115kbit/s, ratio 3/16, 1=Max1152kbit/s, ratio 1/4)
 6 Reserved
 5 Invert RTS signal in RS-485 mode (0=RTS=0 during TX, 1=RTS=1 during TX)
 4 Enable the transmitter to control the RTS pin (0=Disable, 1=Enable)
 3 Reserved
 2 Disable transmitter (0=Normal, 1=Disable; stop forwarding THR to TSR)
 UART does not send serial data out on the transmit pin, but the
 transmit FIFO will continue to receive data from host until full.
 Any data in the TSR will be sent out before the transmitter goes into
 disable state.
 1 Disable receiver (0=Normal, 1=Disable)
 UART will stop receiving data immediately once this bit set to 1, and
 any data in the TSR?? will be sent to the receive FIFO.
 User is advised not to set this bit during receiving.
 0 Enable 9bit or Multidrop mode (0=Normal RS-232, 1=Enable RS-485) |
| --- |

Bit7 is SC16IS760 only.

```

| ____________________________ Infrared IrDA Notes ____________________________ |
| --- |

**Infrared Usage in 3DS**

The 3DS doesn't have any expansion connector, and it's instead using the IrDA
port for expansion hardware (with external batteries):

```

| - Circle Pad Pro (a second Circle Pad, attached right of the A/B/X/Y buttons)
 - NFC Near-field communication (for Amiibo figures) |
| --- |

For the NFC thing, see here:

3DS NFC Adapter

That add-on hardware exists for Old3DS only. The New3DS does have equivalent
hardware built-in, and thus doesn't need that add-ons.

The IrDA port could theoretically communicate with old NDS pedometers (Activity
Meter and P-Walker), there isn't any known 3DS software supporting that though
(and the 3DS has its own pedometer built-in anyways, although it's a bit clumsy
to carry the whole console).

Additionally, IrDA could be reportedly used for things like file transfers,
unknown if there are 3DS tools supporting such things?

```

| XXX see 3DS Connector and Chipset pinout chapters:
 - Receive PWDOWN is controlled via GPIO0 IR[58h]
 - ARM GPIO support manual transmit/receive (without using I2C transfer) |
| --- |

**Infrared Signals**

The SC16IS750 UART/IrDA SIR datasheet does mainly describe the UART part, and
barely mentions infrared at all (apart from saying that the chip is IrDA SIR
compatible, and that IrDA can be enabled in MCR.bit6).

Going by other sources, IrDA SIR works like "UART over infrared". In lack of
further details, here are some guesses on how it might work (and how far it is
(or isn't) compatible with regular cable-based UARTs):

```

| - RX/TX are probably using pulsed signals instead of constant LOW/HIGH levels?
 - RTS/CTS/DTR/DSR are probably not implemented at all?
 - Simultaneous RX+TX might work in opposite light-direction, if no reflection?
 - Infrared interruptions and noise may require additional error checking?
 - Infrared at less than 10mm (as in Circle Pad Pro) may work 100% error-free? |
| --- |

**Infrared Protocol**

Specifications for the IrDA standard aren't available for free, and there's
little known about the protocol:

```

| - IrDA protocol supports file transfers from one device to another
 - IrDA protocol must include some sort of packet headers and checksums
 - And probably much stuff |
| --- |

However, it's unknown if Nintendo did bother to follow the IrDA standard in
their Circle Pad Pro and NFC add-ons. It's also possible that they have just
used a custom UART-like transfer protocol for that hardware.

**Infrared Range**

IrDA is supposed for short-range transfers (about 20cm - 100cm) in binary
format. As such, it isn't intended to use the hardware for things like classic
TV remote controls (although... maybe it could be tweaked to output TV remote
signals on TX pin... possibly in UART mode with IrDA disabled?).