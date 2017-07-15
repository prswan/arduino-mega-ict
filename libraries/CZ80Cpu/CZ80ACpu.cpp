//
// Copyright (c) 2017, Paul R. Swan
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
#include "CZ80ACpu.h"
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
24,   // PA2 -  6 - CLK
26,   // PA4 -  7 - D4
28,   // PA6 -  8 - D3
30,   // PC7 -  9 - D5
32,   // PC5 - 10 - D6
34,   // PC3 - 11
36,   // PC1 - 12 - D2
38,   // PD7 - 13 - D7
40,   // PG1 - 14 - D0
42,   // PL7 - 15 - D1
44,   // PL5 - 16
46,   // PL3 - 17
48,   // PL1 - 18
50,   // PB3 - 19 - MREQ
52,   // PB1 - 20 - IORQ
53,   // PB0 - 21 - RD
51,   // PB2 - 22 - WR
49,   // PL0 - 23
47,   // PL2 - 24 - WAIT
45,   // PL4 - 25
43,   // PL6 - 26
41,   // PG0 - 27
39,   // PG2 - 28
37,   // PC0 - 29
35,   // PC2 - 30
33,   // PC4 - 31
31,   // PC6 - 32
29,   // PA7 - 33
27,   // PA5 - 34
25,   // PA3 - 35
23,   // PA1 - 36
21,   // PD0 - 37
19,   // PD2 - 38
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
static const UINT8 s_A2_BIT_IN_CLK     = 0x04;
static const UINT8 s_L2_BIT_IN_WAIT    = 0x04;

//
// Output definitions.
//
static const UINT8 s_B0_BIT_OUT_RD     = 0x01;
static const UINT8 s_B1_BIT_OUT_IORQ   = 0x02;
static const UINT8 s_B2_BIT_OUT_WR     = 0x04;
static const UINT8 s_B3_BIT_OUT_MREQ   = 0x08;


//
// Data Bus definition
//
static const UINT8 s_G1_BIT_D0        = 0x02;
static const UINT8 s_L7_BIT_D1        = 0x80;
static const UINT8 s_C1_BIT_D2        = 0x02;
static const UINT8 s_A6_BIT_D3        = 0x40;
static const UINT8 s_A4_BIT_D4        = 0x10;
static const UINT8 s_C7_BIT_D5        = 0x80;
static const UINT8 s_C5_BIT_D6        = 0x20;
static const UINT8 s_D7_BIT_D7        = 0x80;


//
// Wait for CLK rising edge to be detected.
// This loop is 8 instructions total, 500ns.
//
#define WAIT_FOR_CLK_RISING_EDGE(x,r1,r2)  \
    {                                      \
        for ( ; x > 0 ; x--)               \
        {                                  \
            r1 = *g_portInA;               \
            r2 = *g_portInA;               \
                                           \
            if (!(r1 & s_A2_BIT_IN_CLK) && \
                 (r2 & s_A2_BIT_IN_CLK))   \
            {                              \
                break;                     \
            }                              \
        }                                  \
    }                                      \

//
// Wait for CLK falling edge to be detected.
// This loop is 8 instructions total, 500ns.
//
#define WAIT_FOR_CLK_FALLING_EDGE(x,r1,r2) \
    {                                      \
        for ( ; x > 0 ; x--)               \
        {                                  \
            r1 = *g_portInA;               \
            r2 = *g_portInA;               \
                                           \
            if ( (r1 & s_A2_BIT_IN_CLK) && \
                !(r2 & s_A2_BIT_IN_CLK))   \
            {                              \
                break;                     \
            }                              \
        }                                  \
    }                                      \


//
// Wait for WAIT high.
// This loop is 2 instructions total, 125ns.
// Note that the system will hang here if WAIT is stuck.
//
#define WAIT_FOR_WAIT_HI(x,r1,r2)          \
    {                                      \
        while(1)                           \
        {                                  \
            r1 = *g_portInL;               \
                                           \
            if ((r1 & s_L2_BIT_IN_WAIT))   \
            {                              \
                break;                     \
            }                              \
        }                                  \
    }                                      \


//
// Wait for WAIT low.
// This loop is 2 instructions total, 125ns.
// Note that the system will hang here if WAIT is stuck.
//
#define WAIT_FOR_WAIT_LO(x,r1,r2)          \
    {                                      \
        while(1)                           \
        {                                  \
            r1 = *g_portInL;               \
                                           \
            if (!(r1 & s_L2_BIT_IN_WAIT))  \
            {                              \
                break;                     \
            }                              \
        }                                  \
    }                                      \

#define IS_IO_SPACE(ad)   ((ad & (UINT32) 0x010000) != 0)
#define IS_WAIT_SPACE(ad) ((ad & (UINT32) 0x100000) != 0)


//
// Control Pins
//
static const CONNECTION s_CLK_i      = {  6, "CLK"      };
static const CONNECTION s_Vcc_i      = { 11, "Vcc"      };
static const CONNECTION s__INT_i     = { 16, "_INT"     };
static const CONNECTION s__NMI_i     = { 17, "_NMI"     };
static const CONNECTION s__HALT_o    = { 18, "_HALT"    };
static const CONNECTION s__MREQ_ot   = { 19, "_MREQ"    };
static const CONNECTION s__IORQ_ot   = { 20, "_IORQ"    };
static const CONNECTION s__RD_ot     = { 21, "_RD"      };
static const CONNECTION s__WR_ot     = { 22, "_WR"      };
static const CONNECTION s__BUSACK_o  = { 23, "_BUSACK"  };
static const CONNECTION s__WAIT_i    = { 24, "_WAIT"    };
static const CONNECTION s__BUSREQ_i  = { 25, "_BUSREQ"  };
static const CONNECTION s__RESET_i   = { 26, "_RESET"   };
static const CONNECTION s__M1_o      = { 27, "_M1"      };
static const CONNECTION s__RFSH_ot   = { 28, "_RFSH"    };
static const CONNECTION s_GND_i      = { 29, "GND"      };


//
// Bus pins
//
static const CONNECTION s_A_ot[]   = { {30, "A0"  },
                                       {31, "A1"  },
                                       {32, "A2"  },
                                       {33, "A3"  },
                                       {34, "A4"  },
                                       {35, "A5"  },
                                       {36, "A6"  },
                                       {37, "A7"  },
                                       {38, "A8"  },
                                       {39, "A9"  },
                                       {40, "A10" },
                                       { 1, "A11" },
                                       { 2, "A12" },
                                       { 3, "A13" },
                                       { 4, "A14" },
                                       { 5, "A15" } }; // 16 bits

static const CONNECTION s_D_iot[] = { {14, "D0" },
                                      {15, "D1" },
                                      {12, "D2" },
                                      { 8, "D3" },
                                      { 7, "D4" },
                                      { 9, "D5" },
                                      {10, "D6" },
                                      {13, "D7" } }; // 8 bits.


CZ80ACpu::CZ80ACpu(
    UINT32 vramAddress
) : m_busA(g_pinMap40DIL, s_A_ot,  ARRAYSIZE(s_A_ot)),
    m_busD(g_pinMap40DIL, s_D_iot, ARRAYSIZE(s_D_iot)),
    m_pin_RD(g_pinMap40DIL, &s__RD_ot),
    m_pin_WR(g_pinMap40DIL, &s__WR_ot),
    m_pin_WAIT(g_pinMap40DIL, &s__WAIT_i),
    m_pin_IORQ(g_pinMap40DIL, &s__IORQ_ot),
    m_pin_MREQ(g_pinMap40DIL, &s__MREQ_ot),
    m_vramAddress(vramAddress)
{
};

//
// The idle function sets up the pins into the correct direction (input/output)
// and idle state ready for the next bus cycle.
//
PERROR
CZ80ACpu::idle(
)
{
    pinMode(g_pinMap40DIL[s_CLK_i.pin],           INPUT);
    pinMode(g_pinMap40DIL[s_Vcc_i.pin],           INPUT);
    pinMode(g_pinMap40DIL[s__INT_i.pin],          INPUT);
    pinMode(g_pinMap40DIL[s__NMI_i.pin],          INPUT);

    digitalWrite(g_pinMap40DIL[s__HALT_o.pin],    HIGH);
    pinMode(g_pinMap40DIL[s__HALT_o.pin],         OUTPUT);

    digitalWrite(g_pinMap40DIL[s__BUSACK_o.pin],  HIGH);
    pinMode(g_pinMap40DIL[s__BUSACK_o.pin],       OUTPUT);

    pinMode(g_pinMap40DIL[s__WAIT_i.pin],         INPUT);
    pinMode(g_pinMap40DIL[s__BUSREQ_i.pin],       INPUT);
    pinMode(g_pinMap40DIL[s__RESET_i.pin],        INPUT);

    digitalWrite(g_pinMap40DIL[s__M1_o.pin],      HIGH);
    pinMode(g_pinMap40DIL[s__M1_o.pin],           OUTPUT);
    digitalWrite(g_pinMap40DIL[s__RFSH_ot.pin],   HIGH);
    pinMode(g_pinMap40DIL[s__RFSH_ot.pin],        OUTPUT);

    pinMode(g_pinMap40DIL[s_GND_i.pin],           INPUT_PULLUP);

    // Use the pullup input as the float to detect shorts to ground.
    m_busA.pinMode(INPUT_PULLUP);
    m_busD.pinMode(INPUT_PULLUP);

    // Set the fast output pins to output an inactive.
    m_pin_RD.digitalWrite(HIGH);
    m_pin_RD.pinMode(OUTPUT);

    m_pin_WR.digitalWrite(HIGH);
    m_pin_WR.pinMode(OUTPUT);

    m_pin_IORQ.digitalWrite(HIGH);
    m_pin_IORQ.pinMode(OUTPUT);

    m_pin_MREQ.digitalWrite(HIGH);
    m_pin_MREQ.pinMode(OUTPUT);

    // Set the fast input pins to input
    m_pin_WAIT.pinMode(INPUT);

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
CZ80ACpu::check(
)
{
    PERROR error = errorSuccess;

    // The ground pin (with pullup) should be connected to GND (LOW)
    CHECK_VALUE_EXIT(error, g_pinMap40DIL, s_GND_i, LOW);

    // The Vcc pin should be high (power is on).
    CHECK_VALUE_EXIT(error, g_pinMap40DIL, s_Vcc_i, HIGH);

    // The reset pin should be high (no reset).
    CHECK_VALUE_EXIT(error, g_pinMap40DIL, s__RESET_i, HIGH);

    // Nothing should be driving wait states.
    CHECK_VALUE_EXIT(error, g_pinMap40DIL, s__WAIT_i, HIGH);

    // The tester doesn't support bus sharing.
    CHECK_VALUE_EXIT(error, g_pinMap40DIL, s__BUSREQ_i, HIGH);

    // The address bus should be uncontended and pulled high.
    CHECK_BUS_VALUE_UINT16_EXIT(error, m_busA, s_A_ot, 0xFFFF);

    // The data bus should be uncontended and pulled high.
    CHECK_BUS_VALUE_UINT8_EXIT(error, m_busD, s_D_iot, 0xFF);

    // Loop to detect a clock by sampling and detecting both high and lows.
    {
        UINT16 hiCount = 0;
        UINT16 loCount = 0;

        for (int i = 0 ; i < 1000 ; i++)
        {
            int value = ::digitalRead(g_pinMap40DIL[s_CLK_i.pin]);

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
            CHECK_VALUE_EXIT(error, g_pinMap40DIL, s_CLK_i, LOW);
        }
        else if (hiCount == 0)
        {
            CHECK_VALUE_EXIT(error, g_pinMap40DIL, s_CLK_i, HIGH);
        }
    }

    //
    // Check if a none-zero VRAM address was supplied, indicating that the
    // WAIT toggling should be active for that access. This is checked here
    // because there is no timeout in the bus cycles thus they hang if the
    // VRAM WAIT hold off isn't working.
    //

    if (m_vramAddress != 0)
    {
        UINT16 hiCount = 0;
        UINT16 loCount = 0;

        m_busA.pinMode(OUTPUT);
        m_busA.digitalWrite(m_vramAddress);
        m_pin_MREQ.digitalWriteLOW();

        for (int i = 0 ; i < 1000 ; i++)
        {
            int value = m_pin_WAIT.digitalRead();

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
            CHECK_PIN_VALUE_EXIT(error, m_pin_WAIT, s__WAIT_i, LOW);
        }
        else if (hiCount == 0)
        {
            CHECK_PIN_VALUE_EXIT(error, m_pin_WAIT, s__WAIT_i, HIGH);
        }

        m_pin_MREQ.digitalWriteHIGH();
        m_busA.digitalWrite(~0);
        m_busA.pinMode(INPUT);
    }

Exit:
    return error;
}


UINT8
CZ80ACpu::dataBusWidth(
    UINT32 address
)
{
    return 1;
}


UINT8
CZ80ACpu::dataAccessWidth(
    UINT32 address
)
{
    return 1;
}


PERROR
CZ80ACpu::memoryRead(
    UINT32 address,
    UINT16 *data
)
{
    PERROR error = errorSuccess;

    // Enable the address bus and set the value (the lower 16 bits only)
    m_busA.pinMode(OUTPUT);
    m_busA.digitalWrite((UINT16) (address & 0xFFFF));

    // Set the databus to input.
    m_busD.pinMode(INPUT);

    // Critical timing section
    noInterrupts();

    // Check for video RAM access that requires synchronization
    // with HBLANK using the WAIT input for pre-synchronization.

    if (IS_WAIT_SPACE(address))
    {
        register UINT8 x = 255;

        register UINT8 r1;
        register UINT8 r2;

        *g_portOutB = ~(s_B3_BIT_OUT_MREQ);

        // Wait for wait to become active (leaving HBLANK)
        WAIT_FOR_WAIT_LO(x,r1,r2);

        // Wait for wait to become inactive (start of HBLANK)
        WAIT_FOR_WAIT_HI(x,r1,r2);

        *g_portOutB = ~(0);
    }

    // Select the address space based on the supplied address
    if (IS_IO_SPACE(address))
    {
        error = IORQread(data);
    }
    else
    {
        error = MREQread(data);
    }

    interrupts();

    return error;
}


PERROR
CZ80ACpu::memoryWrite(
    UINT32 address,
    UINT16 data
)
{
    PERROR error = errorSuccess;

    // Enable the address bus and set the value.
    m_busA.pinMode(OUTPUT);
    m_busA.digitalWrite((UINT16) (address & 0xFFFF));

    // Set the databus to output and set a value.
    m_busD.pinMode(OUTPUT);
    m_busD.digitalWrite(data);

    // Critical timing section
    noInterrupts();

    // Check for video RAM access that requires synchronization
    // with HBLANK using the WAIT input for pre-synchronization.

    if (IS_WAIT_SPACE(address))
    {
        register UINT8 x = 255;

        register UINT8 r1;
        register UINT8 r2;

        *g_portOutB = ~(s_B3_BIT_OUT_MREQ);

        // Wait for wait to become active (leaving HBLANK)
        WAIT_FOR_WAIT_LO(x,r1,r2);

        // Wait for wait to become inactive (start of HBLANK)
        WAIT_FOR_WAIT_HI(x,r1,r2);

        *g_portOutB = ~(0);
    }

    // Select the address space based on the supplied address
    if (IS_IO_SPACE(address))
    {
        error = IORQwrite(data);
    }
    else
    {
        error = MREQwrite(data);
    }

    interrupts();

    return error;
}


PERROR
CZ80ACpu::waitForInterrupt(
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

    UINT8 intPin = ((interrupt == NMI) ? (g_pinMap40DIL[s__NMI_i.pin]) :
                                         (g_pinMap40DIL[s__INT_i.pin]));

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
// TBD.
//
PERROR
CZ80ACpu::acknowledgeInterrupt(
    UINT16 *response
)
{
    PERROR error = errorSuccess;

    *response = 0;

    return error;
}


PERROR
CZ80ACpu::MREQread(
    UINT16 *data
)
{
    PERROR error = errorSuccess;

    //
    // Using separate counts saves 4 instructions because the compiler
    // optimizes out the pre-loop test (it knows the 1st time x > 0).
    //
    register UINT8 x = 255;

    register UINT8 r1;
    register UINT8 r2;
    register UINT8 r3;
    register UINT8 r4;
    register UINT8 r5;

    // Wait for the clock edge
    WAIT_FOR_CLK_RISING_EDGE(x,r1,r2);

    // Start the cycle by assert the control lines
    *g_portOutB = ~(s_B3_BIT_OUT_MREQ | s_B0_BIT_OUT_RD);
    *g_portOutB = ~(s_B3_BIT_OUT_MREQ | s_B0_BIT_OUT_RD); // Wait state.

    // Wait for WAIT to be deasserted.
    // Port L requires an "lds" to access so no wait state needed from above.
    WAIT_FOR_WAIT_HI(x,r1,r2);

    *g_portOutB = ~(s_B3_BIT_OUT_MREQ | s_B0_BIT_OUT_RD); // Wait state

    // Read in reverse order - port L is a slower access.
    r1 = *g_portInL;
    r2 = *g_portInG;
    r3 = *g_portInD;
    r4 = *g_portInC;
    r5 = *g_portInA;

    // Terminate the cycle
    *g_portOutB = ~(0);

    // Check for timeout
    if (x == 0)
    {
        error = errorTimeout;
        goto Exit;
    }

    // Populate the output data word
    *data = (((r2 & s_G1_BIT_D0) >> 1) << 0) |
            (((r1 & s_L7_BIT_D1) >> 7) << 1) |
            (((r4 & s_C1_BIT_D2) >> 1) << 2) |
            (((r5 & s_A6_BIT_D3) >> 6) << 3) |
            (((r5 & s_A4_BIT_D4) >> 4) << 4) |
            (((r4 & s_C7_BIT_D5) >> 7) << 5) |
            (((r4 & s_C5_BIT_D6) >> 5) << 6) |
            (((r3 & s_D7_BIT_D7) >> 7) << 7);

Exit:

    return error;
}


PERROR
CZ80ACpu::MREQwrite(
    UINT16 *data
)
{
    PERROR error = errorSuccess;

    //
    // Using separate counts saves 4 instructions because the compiler
    // optimizes out the pre-loop test (it knows the 1st time x > 0).
    //
    register UINT8 x = 255;

    register UINT8 r1;
    register UINT8 r2;
    register UINT8 r3;
    register UINT8 r4;
    register UINT8 r5;

    // Wait for the clock edge
    WAIT_FOR_CLK_RISING_EDGE(x,r1,r2);

    // Start the cycle by assert the control lines
    *g_portOutB = ~(s_B3_BIT_OUT_MREQ);
    *g_portOutB = ~(s_B3_BIT_OUT_MREQ); // wait state

    // Wait for WAIT to be deasserted.
    // Port L requires an "lds" to access so no wait state needed from above.
    WAIT_FOR_WAIT_HI(x,r1,r2);

    *g_portOutB = ~(s_B3_BIT_OUT_MREQ | s_B2_BIT_OUT_WR);

    // This is a write but we keep these here to match the read cycle timing.
    r1 = *g_portInL;
    r2 = *g_portInG;
    r3 = *g_portInD;
    r4 = *g_portInC;
    r5 = *g_portInA;

    // Terminate the cycle
    *g_portOutB = ~(0);

    // Check for timeout
    if (x == 0)
    {
        error = errorTimeout;
        goto Exit;
    }

Exit:

    return error;
}


PERROR
CZ80ACpu::IORQread(
    UINT16 *data
)
{
    // IO cycles are slightly different than memory cycles.
    return errorNotImplemented;
}


PERROR
CZ80ACpu::IORQwrite(
    UINT16 *data
)
{
    // IO cycles are slightly different than memory cycles.
    return errorNotImplemented;
}

