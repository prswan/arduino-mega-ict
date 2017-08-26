//
// Copyright (c) 2015, Paul R. Swan
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without modification,
// are permitted provided that the following conditions are met:
//
// 1. Redistributions of source code must retain the above copyright notice,
//    this list of conditions and the following disclaimer.
//
// 2. Redistributions in binary form must reproduce the above copyright notice,
//    this list of conditions and the following disclaimer in the documentation
//    and/or other materials provided with the distribution.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS
// OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
// MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
// COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
// EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
// HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR
// TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
// EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
#include "Arduino.h"
#include "Error.h"
#include "C2650Cpu.h"
#include "PinMap.h"


//
// Pin prefixes
//
// _ - active low
//
// Pin suffixes
//
// i - input
// o - output
// t - tri-state
//
// NOTE: This implementation was tested using a shield without protection resistors.
//       It has not been tested with a "standard" shield with protection resistors.
//


/*
WARNING: Ports E, G & H are used by the LCD/Keypad shield and thus cannot be directly used!

14,   // PJ1 -  1
16,   // PH1 -  2
18,   // PD3 -  3
20,   // PD1 -  4
22,   // PA0 -  5
24,   // PA2 -  6
26,   // PA4 -  7
28,   // PA6 -  8
30,   // PC7 -  9
32,   // PC5 - 10
34,   // PC3 - 11
36,   // PC1 - 12
38,   // PD7 - 13
40,   // PG1 - 14
42,   // PL7 - 15
44,   // PL5 - 16
46,   // PL3 - 17
48,   // PL1 - 18
50,   // PB3 - 19
52,   // PB1 - 20
53,   // PB0 - 21
51,   // PB2 - 22 - WRP
49,   // PL0 - 23
47,   // PL2 - 24 - OPREQ
45,   // PL4 - 25
43,   // PL6 - 26 - D7
41,   // PG0 - 27 - D6
39,   // PG2 - 28 - D5
37,   // PC0 - 29 - D4
35,   // PC2 - 30 - D3
33,   // PC4 - 31 - D2
31,   // PC6 - 32 - D1
29,   // PA7 - 33 - D0
27,   // PA5 - 34
25,   // PA3 - 35
23,   // PA1 - 36 - OPACK
21,   // PD0 - 37
19,   // PD2 - 38 - CLK
17,   // PH0 - 39
15    // PJ0 - 40
*/


//
// Byte-wide port registers
//
static volatile UINT8 * const g_portInA       = &PINA;
static volatile UINT8 * const g_portOutA      = &PORTA;

static volatile UINT8 * const g_portInB       = &PINB;
static volatile UINT8 * const g_portOutB      = &PORTB;

static volatile UINT8 * const g_portInC       = &PINC;
static volatile UINT8 * const g_portOutC      = &PORTC;

static volatile UINT8 * const g_portInD       = &PIND;
static volatile UINT8 * const g_portOutD      = &PORTD;

static volatile UINT8 * const g_portInG       = &PING;
static volatile UINT8 * const g_portOutG      = &PORTG;

static volatile UINT8 * const g_portInL       = &PINL;
static volatile UINT8 * const g_portOutL      = &PORTL;

//
// Input definitions.
//
static const UINT8 s_D2_BIT_IN_CLK     = (1 << 2);
static const UINT8 s_A1_BIT_IN_OPACK   = (1 << 1);

//
// Output definitions.
//
static const UINT8 s_B2_BIT_OUT_WRP    = (1 << 2);
static const UINT8 s_L2_BIT_OUT_OPREQ  = (1 << 2);


//
// Data Bus definition
//
static const UINT8 s_A7_BIT_D0        = (1 << 7);
static const UINT8 s_C6_BIT_D1        = (1 << 6);
static const UINT8 s_C4_BIT_D2        = (1 << 4);
static const UINT8 s_C2_BIT_D3        = (1 << 2);
static const UINT8 s_C0_BIT_D4        = (1 << 0);
static const UINT8 s_G2_BIT_D5        = (1 << 2);
static const UINT8 s_G0_BIT_D6        = (1 << 0);
static const UINT8 s_L6_BIT_D7        = (1 << 6);


//
// Wait for CLK rising edge to be detected.
// This loop is 8 instructions total, 500ns.
// Note that the system will hang here if CLK is stuck.
//
#define WAIT_FOR_CLK_RISING_EDGE(r1,r2)    \
    {                                      \
        while(1)                           \
        {                                  \
            r1 = *g_portInD;               \
            r2 = *g_portInD;               \
                                           \
            if (!(r1 & s_D2_BIT_IN_CLK) && \
                 (r2 & s_D2_BIT_IN_CLK))   \
            {                              \
                break;                     \
            }                              \
        }                                  \
    }                                      \


//
// Wait for CLK low.
// This loop is 2 instructions total, 125ns.
// Note that the system will hang here if CLK is stuck.
//
#define WAIT_FOR_CLK_LO(r1)                \
    {                                      \
        while(1)                           \
        {                                  \
            r1 = *g_portInD;               \
                                           \
            if (!(r1 & s_D2_BIT_IN_CLK))   \
            {                              \
                break;                     \
            }                              \
        }                                  \
    }                                      \

//
// Wait for CLK High.
// This loop is 2 instructions total, 125ns.
// Note that the system will hang here if CLK is stuck.
//
#define WAIT_FOR_CLK_HI(r1)                \
    {                                      \
        while(1)                           \
        {                                  \
            r1 = *g_portInD;               \
                                           \
            if ((r1 & s_D2_BIT_IN_CLK))    \
            {                              \
                break;                     \
            }                              \
        }                                  \
    }                                      \

//
// Wait for OPACK low.
// This loop is 2 instructions total, 125ns.
// Note that the system will hang here if OPACK is stuck.
//
#define WAIT_FOR_OPACK_LO(r1)              \
    {                                      \
        while(1)                           \
        {                                  \
            r1 = *g_portInA;               \
                                           \
            if (!(r1 & s_A1_BIT_IN_OPACK)) \
            {                              \
                break;                     \
            }                              \
        }                                  \
    }                                      \


//
// Control Pins
//
static const CONNECTION s_SENSE_i    = {  1, "SENSE"    };
static const CONNECTION s__ADREN_i   = { 15, "_ADREN"   };
static const CONNECTION s_RESET_i    = { 16, "RESET"    };
static const CONNECTION s__INTREQ_i  = { 17, "_INTREQ"  };
static const CONNECTION s_M_IO_o     = { 20, "M_IO"     };
static const CONNECTION s_GND_i      = { 21, "GND"      };
static const CONNECTION s_WRP_o      = { 22, "WRP"      };
static const CONNECTION s__RW_o      = { 23, "_RW"      };
static const CONNECTION s_OPREQ_o    = { 24, "OPREQ"    };
static const CONNECTION s__DBUSEN_i  = { 25, "_DBUSEN"  };
static const CONNECTION s_INTACK_o   = { 34, "INTACK"   };
static const CONNECTION s_RUN_WAIT_o = { 35, "RUN_WAIT" };
static const CONNECTION s__OPACK_i   = { 36, "_OPACK"   };
static const CONNECTION s__PAUSE_i   = { 37, "_PAUSE"   };
static const CONNECTION s_CLOCK_i    = { 38, "CLOCK"    };
static const CONNECTION s_Vcc_i      = { 39, "Vcc"      };
static const CONNECTION s_FLAG_o     = { 40, "FLAG"     };;

//
// Dual function pins
//
static const CONNECTION s_D_C_ot  = { 18, "D_C"  };
static const CONNECTION s_E_NE_ot = { 19, "N_NE" };

//
// Bus pins
//
static const CONNECTION s_ADR_ot[]   = { {14, "ADR0"  },
                                         {13, "ADR1"  },
                                         {12, "ADR2"  },
                                         {11, "ADR3"  },
                                         {10, "ADR4"  },
                                         { 9, "ADR5"  },
                                         { 8, "ADR6"  },
                                         { 7, "ADR7"  },
                                         { 6, "ADR8"  },
                                         { 5, "ADR9"  },
                                         { 4, "ADR10" },
                                         { 3, "ADR11" },
                                         { 2, "ADR12" },
                                         {19, "ADR13" },
                                         {18, "ADR14" } }; // 15 bits

static const CONNECTION s_DBUS_iot[] = { {33, "DBUS0" },
                                         {32, "DBUS1" },
                                         {31, "DBUS2" },
                                         {30, "DBUS3" },
                                         {29, "DBUS4" },
                                         {28, "DBUS5" },
                                         {27, "DBUS6" },
                                         {26, "DBUS7" } }; // 8 bits.


C2650Cpu::C2650Cpu(
) : m_busADR(g_pinMap40DIL, s_ADR_ot, ARRAYSIZE(s_ADR_ot)),
    m_busDBUS(g_pinMap40DIL, s_DBUS_iot, ARRAYSIZE(s_DBUS_iot)),
    m_pinCLOCK(g_pinMap40DIL, &s_CLOCK_i),
    m_pinOPREQ(g_pinMap40DIL, &s_OPREQ_o),
    m_pinWRP(g_pinMap40DIL, &s_WRP_o),
    m_pin_OPACK(g_pinMap40DIL, &s__OPACK_i)
{
};

//
// The idle function sets up the pins into the correct direction (input/output)
// and idle state ready for the next bus cycle.
//
PERROR
C2650Cpu::idle(
)
{
    pinMode(g_pinMap40DIL[s_SENSE_i.pin],         INPUT);
    pinMode(g_pinMap40DIL[s__ADREN_i.pin],        INPUT);
    pinMode(g_pinMap40DIL[s_RESET_i.pin],         INPUT);
    pinMode(g_pinMap40DIL[s__INTREQ_i.pin],       INPUT);

    digitalWrite(g_pinMap40DIL[s_M_IO_o.pin],     HIGH);
    pinMode(g_pinMap40DIL[s_M_IO_o.pin],          OUTPUT);

    pinMode(g_pinMap40DIL[s_GND_i.pin],           INPUT_PULLUP);

    digitalWrite(g_pinMap40DIL[s__RW_o.pin],      HIGH);
    pinMode(g_pinMap40DIL[s__RW_o.pin],           OUTPUT);

    pinMode(g_pinMap40DIL[s__DBUSEN_i.pin],       INPUT);

    digitalWrite(g_pinMap40DIL[s_INTACK_o.pin],   LOW);
    pinMode(g_pinMap40DIL[s_INTACK_o.pin],        OUTPUT);

    digitalWrite(g_pinMap40DIL[s_RUN_WAIT_o.pin], LOW);
    pinMode(g_pinMap40DIL[s_RUN_WAIT_o.pin],      OUTPUT);

    pinMode(g_pinMap40DIL[s__PAUSE_i.pin],        INPUT);
    pinMode(g_pinMap40DIL[s_Vcc_i.pin],           INPUT);

    digitalWrite(g_pinMap40DIL[s_FLAG_o.pin],     LOW);
    pinMode(g_pinMap40DIL[s_FLAG_o.pin],          OUTPUT);

    // since the address bus is controlled by ADREN we can't set it output.
    m_busADR.pinMode(INPUT_PULLUP);

    // since the data bus is controlled by DBUSEN we can't set it output.
    m_busDBUS.pinMode(INPUT_PULLUP);

    // Set the fast clock & opack pins to input
    m_pinCLOCK.pinMode(INPUT);
    m_pin_OPACK.pinMode(INPUT);

    // Set the fast WRP & OPREQ pin to output and LOW.
    m_pinOPREQ.digitalWrite(LOW);
    m_pinOPREQ.pinMode(OUTPUT);

    m_pinWRP.digitalWrite(HIGH);
    m_pinWRP.pinMode(OUTPUT);

    return errorSuccess;
}

//
// The check function performs a basic pin check that all the outputs can be pulled high
// by the pullup resistor as a way to detect a GND short or pulled output. It also validates
// the default state of the control pins would allow the CPU to execute instructions.
//
// If check fails the pins are left in the failing state. If check succeeds the pins are reset
// to idle state.
//
PERROR
C2650Cpu::check(
)
{
    PERROR error = errorSuccess;

    // The ground pin (with pullup) should be connected to GND (LOW)
    CHECK_VALUE_EXIT(error, g_pinMap40DIL, s_GND_i, LOW);

    // The Vcc pin should be high (power is on).
    CHECK_VALUE_EXIT(error, g_pinMap40DIL, s_Vcc_i, HIGH);

    // The reset pin should be low (no reset).
    CHECK_VALUE_EXIT(error, g_pinMap40DIL, s_RESET_i, LOW);

    // The pause pin should be high (running).
    CHECK_VALUE_EXIT(error, g_pinMap40DIL, s__PAUSE_i, HIGH);

    // In everything we'll be testing, ADREN is tied low.
    CHECK_VALUE_EXIT(error, g_pinMap40DIL, s__ADREN_i, LOW);

    // In everything we'll be testing, DBUSEN is tied low.
    CHECK_VALUE_EXIT(error, g_pinMap40DIL, s__DBUSEN_i, LOW);

    // The address bus should be uncontended and pulled high.
    CHECK_BUS_VALUE_UINT16_EXIT(error, m_busADR,  s_ADR_ot,   0x7FFF);

    // The data bus should be uncontended and pulled high.
    CHECK_BUS_VALUE_UINT8_EXIT(error, m_busDBUS, s_DBUS_iot, 0xFF);

    // Loop to detect a clock by sampling and detecting both high and lows.
    {
        UINT16 hiCount = 0;
        UINT16 loCount = 0;

        for (int i = 0 ; i < 1000 ; i++)
        {
            int value = ::digitalRead(g_pinMap40DIL[s_CLOCK_i.pin]);

            if (value == LOW)
            {
                loCount++;
            }
            else
            {
                hiCount++;
            }
        }

        if (loCount == 0)
        {
            CHECK_VALUE_EXIT(error, g_pinMap40DIL, s_CLOCK_i, LOW);
        }
        else if (hiCount == 0)
        {
            CHECK_VALUE_EXIT(error, g_pinMap40DIL, s_CLOCK_i, HIGH);
        }
    }

Exit:
    return error;
}


UINT8
C2650Cpu::dataBusWidth(
    UINT32 address
)
{
    return 1;
}


UINT8
C2650Cpu::dataAccessWidth(
    UINT32 address
)
{
    return 1;
}


PERROR
C2650Cpu::memoryRead(
    UINT32 address,
    UINT16 *data
)
{
    if (address > (UINT32) 0xFFFF)
    {
        digitalWrite(g_pinMap40DIL[s_M_IO_o.pin], LOW);
    }
    else
    {
        digitalWrite(g_pinMap40DIL[s_M_IO_o.pin], HIGH);
    }

    digitalWrite(g_pinMap40DIL[s_INTACK_o.pin], LOW);

    return read((address & 0xFFFF), data);
}

PERROR
C2650Cpu::memoryWrite(
    UINT32 address,
    UINT16 data
)
{
    if (address > (UINT32) 0xFFFF)
    {
        digitalWrite(g_pinMap40DIL[s_M_IO_o.pin], LOW);
    }
    else
    {
        digitalWrite(g_pinMap40DIL[s_M_IO_o.pin], HIGH);
    }

    digitalWrite(g_pinMap40DIL[s_INTACK_o.pin], LOW);

    return write((address & 0xFFFF), data);
}


//
// This is the internal function that does a read cycle. The caller is expected
// to set the M_IO pin to reflect the actual space being accessed.
//
PERROR
C2650Cpu::read(
    UINT32 address,
    UINT16 *data
)
{
    PERROR error = errorSuccess;
    bool interruptsDisabled = false;

    // Set a read cycle.
    digitalWrite(g_pinMap40DIL[s__RW_o.pin], LOW);

    // By default WRP is high unless pulsed for write.
    m_pinWRP.digitalWrite(HIGH);

    // Enable the address bus and set the value.
    m_busADR.pinMode(OUTPUT);
    m_busADR.digitalWrite((UINT16) address);

    // Set the databus to input.
    m_busDBUS.pinMode(INPUT);

    // Critical timing section
    noInterrupts();
    interruptsDisabled = true;

    // Timing Note
    // -----------
    // The critical timing on Zaccaria boards is the 2636.
    // Assmbly extract for the critical timing section:
    //
    // 6f6e:	f8 94       	cli
    // 6f70:	60 91 0b 01 	lds	r22, 0x010B	; 0x80010b <__data_load_end+0x7f4fe9>
    // 6f74:	85 b1       	in	r24, 0x05	; 5
    // 6f76:	3e 81       	ldd	r19, Y+6	; 0x06
    // 6f78:	2d 81       	ldd	r18, Y+5	; 0x05
    // 6f7a:	49 b1       	in	r20, 0x09	; 9
    // 6f7c:	99 b1       	in	r25, 0x09	; 9
    // 6f7e:	42 fd       	sbrc	r20, 2
    // 6f80:	fc cf       	rjmp	.-8      	; 0x6f7a <_ZN8C2650Cpu4readEmPt+0x88>
    // 6f82:	92 ff       	sbrs	r25, 2
    // 6f84:	fa cf       	rjmp	.-12     	; 0x6f7a <_ZN8C2650Cpu4readEmPt+0x88>
    // 6f86:	96 2f       	mov	r25, r22
    // 6f88:	94 60       	ori	r25, 0x04	; 4
    // 6f8a:	90 93 0b 01 	sts	0x010B, r25	; 0x80010b <__data_load_end+0x7f4fe9>
    // 6f8e:	85 b9       	out	0x05, r24	; 5
    // 6f90:	90 b1       	in	r25, 0x00	; 0
    // 6f92:	90 b1       	in	r25, 0x00	; 0
    // 6f94:	90 b1       	in	r25, 0x00	; 0
    // 6f96:	90 b1       	in	r25, 0x00	; 0
    // 6f98:	90 b1       	in	r25, 0x00	; 0
    // 6f9a:	01 99       	sbic	0x00, 1	; 0
    // 6f9c:	fe cf       	rjmp	.-4      	; 0x6f9a <_ZN8C2650Cpu4readEmPt+0xa8>
    // 6f9e:	85 b9       	out	0x05, r24	; 5
    // 6fa0:	80 b1       	in	r24, 0x00	; 0
    // 6fa2:	80 b1       	in	r24, 0x00	; 0
    // 6fa4:	80 b1       	in	r24, 0x00	; 0
    // 6fa6:	80 b1       	in	r24, 0x00	; 0
    // 6fa8:	80 b1       	in	r24, 0x00	; 0
    // 6faa:	80 b1       	in	r24, 0x00	; 0
    // 6fac:	80 b1       	in	r24, 0x00	; 0
    // 6fae:	80 b1       	in	r24, 0x00	; 0
    // 6fb0:	40 91 09 01 	lds	r20, 0x0109	; 0x800109 <__data_load_end+0x7f4fe7>
    // 6fb4:	82 b3       	in	r24, 0x12	; 18
    // 6fb6:	96 b1       	in	r25, 0x06	; 6
    // 6fb8:	50 b1       	in	r21, 0x00	; 0
    // 6fba:	60 93 0b 01 	sts	0x010B, r22	; 0x80010b <__data_load_end+0x7f4fe9>
    //
    {
        register UINT8 portOutL;
        register UINT8 portOutB;
        register UINT8 r1;
        register UINT8 r2;
        register UINT8 r3;
        register UINT8 r4;

        // Cache the current value of the port.
        portOutL = *g_portOutL;
        portOutB = *g_portOutB;

        // Wait for the clock edge
        WAIT_FOR_CLK_RISING_EDGE(r1,r2);

        // Start the cycle by assert the control lines
        *g_portOutL = portOutL | s_L2_BIT_OUT_OPREQ;
        *g_portOutB = portOutB; // Wait state, ~60ns

        // 600ns *maximum* for peripherals to assert OPACK
        *g_portInA; // Wait state, ~120ns
        *g_portInA; // Wait state, ~180ns
        *g_portInA; // Wait state, ~240ns
        *g_portInA; // Wait state, ~300ns
        *g_portInA; // Wait state, ~360ns

        // Wait for OPACK to be asserted.
        WAIT_FOR_OPACK_LO(r1);

        //
        // The 2636 requires 450ns to assert data on the bus after OPACK
        // is asserted.
        //
        *g_portOutB = portOutB;//  ~ 60ns
        *g_portInA; // Wait state, ~120ns
        *g_portInA; // Wait state, ~180ns
        *g_portInA; // Wait state, ~240ns
        *g_portInA; // Wait state, ~300ns
        *g_portInA; // Wait state, ~360ns
        *g_portInA; // Wait state, ~420ns
        *g_portInA; // Wait state, ~480ns
        *g_portInA; // Wait state, ~540ns

        // Read in reverse order - port L is a slower access.
        r1 = *g_portInL;
        r2 = *g_portInG;
        r3 = *g_portInC;
        r4 = *g_portInA;

        // Terminate the cycle
        *g_portOutL = portOutL;

        // Populate the output data word
        *data = (((r4 & s_A7_BIT_D0) >> 7) << 0) |
                (((r3 & s_C6_BIT_D1) >> 6) << 1) |
                (((r3 & s_C4_BIT_D2) >> 4) << 2) |
                (((r3 & s_C2_BIT_D3) >> 2) << 3) |
                (((r3 & s_C0_BIT_D4) >> 0) << 4) |
                (((r2 & s_G2_BIT_D5) >> 2) << 5) |
                (((r2 & s_G0_BIT_D6) >> 0) << 6) |
                (((r1 & s_L6_BIT_D7) >> 6) << 7);

    }

Exit:

    if (interruptsDisabled)
    {
        interrupts();
    }
    return error;
}

//
// This is the internal function that does a write cycle. The caller is expected
// to set the M_IO pin to reflect the actual space being accessed.
//
PERROR
C2650Cpu::write(
    UINT32 address,
    UINT16 data
)
{
    PERROR error = errorSuccess;
    bool interruptsDisabled = false;

    // Set a write cycle.
    digitalWrite(g_pinMap40DIL[s__RW_o.pin], HIGH);

    // By default WRP is high unless pulsed for write.
    m_pinWRP.digitalWrite(HIGH);

    // Enable the address bus and set the value.
    m_busADR.pinMode(OUTPUT);
    m_busADR.digitalWrite((UINT16) address);

    // Set the databus to output and set a value.
    m_busDBUS.pinMode(OUTPUT);
    m_busDBUS.digitalWrite(data);

    // Critical timing section
    noInterrupts();
    interruptsDisabled = true;

    // Timing Note
    // -----------
    // The critical timing on Zaccaria boards is the 2636.
    // Assmbly extract for the critical timing section:
    //
    // 717c:	f8 94       	cli
    // 717e:	80 91 0b 01 	lds	r24, 0x010B	; 0x80010b <__data_load_end+0x7f4fe9>
    // 7182:	95 b1       	in	r25, 0x05	; 5
    // 7184:	39 b1       	in	r19, 0x09	; 9
    // 7186:	29 b1       	in	r18, 0x09	; 9
    // 7188:	32 fd       	sbrc	r19, 2
    // 718a:	fc cf       	rjmp	.-8      	; 0x7184 <_ZN8C2650Cpu11memoryWriteEmt+0xb6>
    // 718c:	22 ff       	sbrs	r18, 2
    // 718e:	fa cf       	rjmp	.-12     	; 0x7184 <_ZN8C2650Cpu11memoryWriteEmt+0xb6>
    // 7190:	28 2f       	mov	r18, r24
    // 7192:	24 60       	ori	r18, 0x04	; 4
    // 7194:	20 93 0b 01 	sts	0x010B, r18	; 0x80010b <__data_load_end+0x7f4fe9>
    // 7198:	29 2f       	mov	r18, r25
    // 719a:	2b 7f       	andi	r18, 0xFB	; 251
    // 719c:	25 b9       	out	0x05, r18	; 5
    // 719e:	20 b1       	in	r18, 0x00	; 0
    // 71a0:	20 b1       	in	r18, 0x00	; 0
    // 71a2:	20 b1       	in	r18, 0x00	; 0
    // 71a4:	20 b1       	in	r18, 0x00	; 0
    // 71a6:	20 b1       	in	r18, 0x00	; 0
    // 71a8:	01 99       	sbic	0x00, 1	; 0
    // 71aa:	fe cf       	rjmp	.-4      	; 0x71a8 <_ZN8C2650Cpu11memoryWriteEmt+0xda>
    // 71ac:	95 b9       	out	0x05, r25	; 5
    // 71ae:	90 b1       	in	r25, 0x00	; 0
    // 71b0:	90 b1       	in	r25, 0x00	; 0
    // 71b2:	90 b1       	in	r25, 0x00	; 0
    // 71b4:	90 b1       	in	r25, 0x00	; 0
    // 71b6:	90 b1       	in	r25, 0x00	; 0
    // 71b8:	90 b1       	in	r25, 0x00	; 0
    // 71ba:	90 b1       	in	r25, 0x00	; 0
    // 71bc:	90 b1       	in	r25, 0x00	; 0
    // 71be:	90 91 09 01 	lds	r25, 0x0109	; 0x800109 <__data_load_end+0x7f4fe7>
    // 71c2:	92 b3       	in	r25, 0x12	; 18
    // 71c4:	96 b1       	in	r25, 0x06	; 6
    // 71c6:	90 b1       	in	r25, 0x00	; 0
    // 71c8:	80 93 0b 01 	sts	0x010B, r24	; 0x80010b <__data_load_end+0x7f4fe9>
    //
    {
        register UINT8 portOutL;
        register UINT8 portOutB;
        register UINT8 r1;
        register UINT8 r2;
        register UINT8 r3;
        register UINT8 r4;

        // Cache the current value of the port.
        portOutL = *g_portOutL;
        portOutB = *g_portOutB;

        // Wait for the clock edge
        WAIT_FOR_CLK_RISING_EDGE(r1,r2);

        // Start the cycle by assert the control lines
        *g_portOutL = portOutL | s_L2_BIT_OUT_OPREQ;
        *g_portOutB = portOutB & ~s_B2_BIT_OUT_WRP; // ~60ns

        // 600ns *maximum* for peripherals to assert OPACK
        *g_portInA; // Wait state, ~120ns
        *g_portInA; // Wait state, ~180ns
        *g_portInA; // Wait state, ~240ns
        *g_portInA; // Wait state, ~300ns
        *g_portInA; // Wait state, ~360ns

        // Wait for OPACK to be asserted.
        WAIT_FOR_OPACK_LO(r1);

        //
        // The 2636 requires 450ns to assert data on the bus after OPACK
        // is asserted.
        //
        *g_portOutB = portOutB;//  ~ 60ns
        *g_portInA; // Wait state, ~120ns
        *g_portInA; // Wait state, ~180ns
        *g_portInA; // Wait state, ~240ns
        *g_portInA; // Wait state, ~300ns
        *g_portInA; // Wait state, ~360ns
        *g_portInA; // Wait state, ~420ns
        *g_portInA; // Wait state, ~480ns
        *g_portInA; // Wait state, ~540ns

        // This is a write but we keep these here to match the read cycle timing.
        r1 = *g_portInL;
        r2 = *g_portInG;
        r3 = *g_portInC;
        r4 = *g_portInA;

        // Terminate the cycle
        *g_portOutL = portOutL;
    }

Exit:

    if (interruptsDisabled)
    {
        interrupts();
    }

    return error;
}


void
C2650Cpu::flagWrite(
    UINT8 value
)
{
    digitalWrite(g_pinMap40DIL[s_FLAG_o.pin], value);
}


UINT8
C2650Cpu::senseRead(
)
{
    return digitalRead(g_pinMap40DIL[s_SENSE_i.pin]);
}


PERROR
C2650Cpu::waitForInterrupt(
    Interrupt interrupt,
    bool      active,
    UINT32    timeoutInMs
)
{
    PERROR error = errorSuccess;
    unsigned long startTime = millis();
    unsigned long endTime = startTime + timeoutInMs;
    int sense = (active ? LOW : HIGH);
    int value = 0;

    UINT8 intPin = g_pinMap40DIL[s__INTREQ_i.pin];

    do
    {
        value = ::digitalRead(intPin);

        if (value == sense)
        {
            break;
        }
    }
    while (millis() < endTime);

    if (value != sense)
    {
        error = errorTimeout;
    }

Exit:

    return error;
}


//
// The 2650 UM states that only INTACK asserted with OPREQ are all that are needed to get the
// interrupt acknowledged and the address returned. However, both MAME, Galaxia & Quasar show
// INTACK is only used to gate off ~REDC, i.e. the INTACK cycle is an IO read control
// (at least A14 and A15 low).
// FYI, Galaxia is hard coded to respond with 0x03.
//
PERROR
C2650Cpu::acknowledgeInterrupt(
    UINT16 *response
)
{
    digitalWrite(g_pinMap40DIL[s_M_IO_o.pin], LOW);

    digitalWrite(g_pinMap40DIL[s_INTACK_o.pin], HIGH);

    return read(0, response);
}

