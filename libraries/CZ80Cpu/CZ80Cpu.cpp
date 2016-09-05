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
#include "CZ80Cpu.h"
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


CZ80Cpu::CZ80Cpu(
) : m_busA(g_pinMap40DIL, s_A_ot,  ARRAYSIZE(s_A_ot)),
    m_busD(g_pinMap40DIL, s_D_iot, ARRAYSIZE(s_D_iot)),
    m_pin_RD(g_pinMap40DIL, &s__RD_ot),
    m_pin_WR(g_pinMap40DIL, &s__WR_ot),
    m_pin_WAIT(g_pinMap40DIL, &s__WAIT_i),
    m_pin_IORQ(g_pinMap40DIL, &s__IORQ_ot),
    m_pin_MREQ(g_pinMap40DIL, &s__MREQ_ot)
{
};

//
// The idle function sets up the pins into the correct direction (input/output)
// and idle state ready for the next bus cycle.
//
PERROR
CZ80Cpu::idle(
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
CZ80Cpu::check(
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

Exit:
    return error;
}


UINT8
CZ80Cpu::dataBusWidth(
    UINT32 address
)
{
    return 1;
}


UINT8
CZ80Cpu::dataAccessWidth(
    UINT32 address
)
{
    return 1;
}


//
// Convert the supplied virtual address into a physical address.
//
void
CZ80Cpu::selectAddressSpace(
    UINT32 address
)
{
    if ( (address >= (UINT32) 0x10000) &&
         (address <= (UINT32) 0x1FFFF) )
    {
        m_pin_IORQ.digitalWriteLOW();
        m_pin_MREQ.digitalWriteHIGH();
    }
    else // Everything else is memory access
    {
        m_pin_IORQ.digitalWriteHIGH();
        m_pin_MREQ.digitalWriteLOW();
    }
}

PERROR
CZ80Cpu::memoryRead(
    UINT32 address,
    UINT16 *data
)
{
    PERROR error = errorSuccess;
    bool interruptsDisabled = false;

    // Enable the address bus and set the value (the lower 16 bits only)
    m_busA.pinMode(OUTPUT);
    m_busA.digitalWrite((UINT16) (address & 0xFFFF));

    // Set the databus to input.
    m_busD.pinMode(INPUT);

    // Select the address space based on the supplied address
    selectAddressSpace(address);

    // Critical timing section
    noInterrupts();
    interruptsDisabled = true;

    // Start the cycle by asserting _RD.
    m_pin_RD.digitalWriteLOW();

    // Poll WAIT for cycle completion
    {
        int waitValue;

        // Check for the Money Money V-RAM access that requires synchronization
        // with HBLANK using the WAIT input.

        if (address & 0x100000)
        {
            // Wait for wait to become active (leaving HBLANK)
            for (int i = 0 ; i < 4096 ; i++)
            {
                waitValue = m_pin_WAIT.digitalRead();

                if (waitValue == LOW)
                {
                    break;
                }
            }
            CHECK_LITERAL_VALUE_EXIT(error, s__WAIT_i, waitValue, LOW);

            // Wait for wait to become inactive (start of HBLANK)
            for (int i = 0 ; i < 4096 ; i++)
            {
                waitValue = m_pin_WAIT.digitalRead();

                if (waitValue == HIGH)
                {
                    break;
                }
            }
            CHECK_LITERAL_VALUE_EXIT(error, s__WAIT_i, waitValue, HIGH);
        }

        // Perform a usual cycle.
        for (int i = 0 ; i < 64 ; i++)
        {
            waitValue = m_pin_WAIT.digitalRead();

            if (waitValue == HIGH)
            {
                // Read the data presented on the bus as soon as we see no WAIT set then clear _RD
                m_busD.digitalReadThenDigitalWriteHIGH(data, m_pin_RD);

                break;
            }
        }
        CHECK_LITERAL_VALUE_EXIT(error, s__WAIT_i, waitValue, HIGH);
    }

    // Money Money V-RAM access that prefers the address to be
    // cleared in order to let the video switch back to display.

    m_pin_MREQ.digitalWriteHIGH();
    m_pin_IORQ.digitalWriteHIGH();

Exit:

    if (interruptsDisabled)
    {
        interrupts();
    }
    return error;
}


PERROR
CZ80Cpu::memoryWrite(
    UINT32 address,
    UINT16 data
)
{
    PERROR error = errorSuccess;
    bool interruptsDisabled = false;

    // Enable the address bus and set the value.
    m_busA.pinMode(OUTPUT);
    m_busA.digitalWrite((UINT16) (address & 0xFFFF));

    // Set the databus to output and set a value.
    m_busD.pinMode(OUTPUT);
    m_busD.digitalWrite(data);

    // Select the address space based on the supplied address
    selectAddressSpace(address);

    // Critical timing section
    noInterrupts();
    interruptsDisabled = true;

    // Start the cycle by asserting write.
    m_pin_WR.digitalWriteLOW();

    // Poll WAIT for cycle completion
    {
        int waitValue;

        // Check for the Money Money V-RAM access that requires synchronization
        // with HBLANK using the WAIT input.

        if (address & 0x100000)
        {
            // Wait for wait to become active (leaving HBLANK)
            for (int i = 0 ; i < 4096 ; i++)
            {
                waitValue = m_pin_WAIT.digitalRead();

                if (waitValue == LOW)
                {
                    break;
                }
            }
            CHECK_LITERAL_VALUE_EXIT(error, s__WAIT_i, waitValue, LOW);

            // Wait for wait to become inactive (start of HBLANK)
            for (int i = 0 ; i < 4096 ; i++)
            {
                waitValue = m_pin_WAIT.digitalRead();

                if (waitValue == HIGH)
                {
                    break;
                }
            }
            CHECK_LITERAL_VALUE_EXIT(error, s__WAIT_i, waitValue, HIGH);
        }

        for (int i = 0 ; i < 64 ; i++)
        {
            waitValue = m_pin_WAIT.digitalRead();

            if (waitValue == HIGH)
            {
                // End the cycle by clearing write.
                m_pin_WR.digitalWriteHIGH();

                break;
            }
        }
        CHECK_LITERAL_VALUE_EXIT(error, s__WAIT_i, waitValue, HIGH);
    }

    // Money Money V-RAM access that prefers the address to be
    // cleared in order to let the video switch back to display.

    m_pin_MREQ.digitalWriteHIGH();
    m_pin_IORQ.digitalWriteHIGH();

Exit:

    if (interruptsDisabled)
    {
        interrupts();
    }

    return error;
}


PERROR
CZ80Cpu::waitForInterrupt(
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
CZ80Cpu::acknowledgeInterrupt(
    UINT16 *response
)
{
    PERROR error = errorSuccess;

    *response = 0;

    return error;
}

