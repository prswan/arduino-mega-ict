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
#include "C6809EClockMasterCpu.h"
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
static const CONNECTION s_GND_i      = {  1, "GND"      };
static const CONNECTION s__NMI_i     = {  2, "_NMI"     };
static const CONNECTION s__IRQ_i     = {  3, "_IRQ"     };
static const CONNECTION s__FIRQ_i    = {  4, "_FIRQ"    };
static const CONNECTION s_BS_o       = {  5, "BS"       };
static const CONNECTION s_BA_o       = {  6, "BA"       };
static const CONNECTION s_VCC_i      = {  7, "Vcc"      };
static const CONNECTION s_RW_o       = { 32, "RW"       };
static const CONNECTION s_BUSY_o     = { 33, "BUSY"     };
static const CONNECTION s_E_i        = { 34, "E"        };
static const CONNECTION s_Q_i        = { 35, "Q"        };
static const CONNECTION s_AVMA_o     = { 36, "AVMA"     };
static const CONNECTION s__RESET_i   = { 37, "_RESET"   };
static const CONNECTION s_LIC_o      = { 38, "LIC"      };
static const CONNECTION s_TSC_i      = { 39, "TSC"      };
static const CONNECTION s__HALT_i    = { 40, "_HALT"    };

//
// External master clock on J14 AUX pin 8 (next to the 2-pin GND pin).
//
static const CONNECTION s_Clock_o    = {  8, "Clock"    };

//
// Bus pins
//
static const CONNECTION s_A_ot[]   = { { 8, "A0"  },
                                       { 9, "A1"  },
                                       {10, "A2"  },
                                       {11, "A3"  },
                                       {12, "A4"  },
                                       {13, "A5"  },
                                       {14, "A6"  },
                                       {15, "A7"  },
                                       {16, "A8"  },
                                       {17, "A9"  },
                                       {18, "A10" },
                                       {19, "A11" },
                                       {20, "A12" },
                                       {21, "A13" },
                                       {22, "A14" },
                                       {23, "A15" } }; // 16 bits

static const CONNECTION s_D_iot[] = { {31, "D0" },
                                      {30, "D1" },
                                      {29, "D2" },
                                      {28, "D3" },
                                      {27, "D4" },
                                      {26, "D5" },
                                      {25, "D6" },
                                      {24, "D7" } }; // 8 bits.

C6809EClockMasterCpu::C6809EClockMasterCpu(
) : m_busA(g_pinMap40DIL, s_A_ot,  ARRAYSIZE(s_A_ot)),
    m_busD(g_pinMap40DIL, s_D_iot, ARRAYSIZE(s_D_iot)),
    m_pinRW(g_pinMap40DIL, &s_RW_o),
    m_pinE(g_pinMap40DIL, &s_E_i),
    m_pinQ(g_pinMap40DIL, &s_Q_i),
    m_pinClock(g_pinMap8Aux, &s_Clock_o)
{
};

//
// The idle function sets up the pins into the correct direction (input/output)
// and idle state ready for the next bus cycle.
//
PERROR
C6809EClockMasterCpu::idle(
)
{
    pinMode(g_pinMap40DIL[s_GND_i.pin],    INPUT_PULLUP);
    pinMode(g_pinMap40DIL[s__NMI_i.pin],          INPUT);
    pinMode(g_pinMap40DIL[s__IRQ_i.pin],          INPUT);
    pinMode(g_pinMap40DIL[s__FIRQ_i.pin],         INPUT);

    digitalWrite(g_pinMap40DIL[s_BS_o.pin],         LOW);
    pinMode(g_pinMap40DIL[s_BS_o.pin],           OUTPUT);
    digitalWrite(g_pinMap40DIL[s_BA_o.pin],         LOW);
    pinMode(g_pinMap40DIL[s_BA_o.pin],           OUTPUT);

    pinMode(g_pinMap40DIL[s_VCC_i.pin],           INPUT);

    digitalWrite(g_pinMap40DIL[s_AVMA_o.pin],      HIGH);
    pinMode(g_pinMap40DIL[s_AVMA_o.pin],         OUTPUT);

    pinMode(g_pinMap40DIL[s__RESET_i.pin],        INPUT);

    digitalWrite(g_pinMap40DIL[s_LIC_o.pin],       HIGH);
    pinMode(g_pinMap40DIL[s_LIC_o.pin],          OUTPUT);

    pinMode(g_pinMap40DIL[s_TSC_i.pin],           INPUT);
    pinMode(g_pinMap40DIL[s__HALT_i.pin],         INPUT);

    // Start with pulled high to verify the bus is floating.
    m_busA.pinMode(INPUT_PULLUP);

    // Start with pulled high to verify the bus is floating.
    m_busD.pinMode(INPUT_PULLUP);

    // Set the RW pin to output high for read
    m_pinRW.digitalWriteLOW();
    m_pinRW.pinMode(OUTPUT);

    // Set the fast E & Q pins to input
    m_pinE.pinMode(INPUT);
    m_pinQ.pinMode(INPUT);

    // Set the clock pin to output low
    m_pinClock.digitalWriteLOW();
    m_pinClock.pinMode(OUTPUT);

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
C6809EClockMasterCpu::check(
)
{
    PERROR error = errorSuccess;

    // The ground pin (with pullup) should be connected to GND (LOW)
    CHECK_VALUE_EXIT(error, g_pinMap40DIL, s_GND_i, LOW);

    // The Vcc pin should be high (power is on).
    CHECK_VALUE_EXIT(error, g_pinMap40DIL, s_VCC_i, HIGH);

    // The halt pin should be high (running).
    CHECK_VALUE_EXIT(error, g_pinMap40DIL, s__HALT_i, HIGH);

    // In everything we'll be testing, TSC is pulled low.
    CHECK_VALUE_EXIT(error, g_pinMap40DIL, s_TSC_i, LOW);

    // The address bus should be uncontended and pulled high.
    CHECK_BUS_VALUE_UINT16_EXIT(error, m_busA, s_A_ot, 0xFFFF);

    // The data bus should be uncontended and pulled high.
    // We can't do this because on Star Wars the idle bus is a read of 0xFFFF == 0x61.
    //
    //CHECK_BUS_VALUE_UINT8_EXIT(error, m_busD, s_D_iot, 0xFF);

    // Loop to detect that reset clears
    // On Star Wars this ~0x40000 (262,144) clocks.
    // On exit the reset pin should be high (no reset).
    //
    {
        for (UINT32 i = 0 ; i < 0x41000 ; i++)
        {
            int value = ::digitalRead(g_pinMap40DIL[s__RESET_i.pin]);

            if (value == HIGH)
            {
                break;
            }

            m_pinClock.digitalWriteHIGH();
            m_pinClock.digitalWriteLOW();
        }
    }
    CHECK_VALUE_EXIT(error, g_pinMap40DIL, s__RESET_i, HIGH);

    // Loop to detect E & Q by sampling and detecting both high and lows.
    {
        int hiECount = 0, loECount = 0;
        int hiQCount = 0, loQCount = 0;

        for (int i = 0 ; i < 1000 ; i++)
        {
            int valueE = m_pinE.digitalRead();
            int valueQ = m_pinQ.digitalRead();

            (valueE == LOW) ? (loECount++) : (hiECount++);
            (valueQ == LOW) ? (loQCount++) : (hiQCount++);

            m_pinClock.digitalWriteHIGH();
            m_pinClock.digitalWriteLOW();
        }

        if (loECount == 0)
        {
            CHECK_PIN_VALUE_EXIT(error, m_pinE, s_E_i, LOW);
        }

        if (hiECount == 0)
        {
            CHECK_PIN_VALUE_EXIT(error, m_pinE, s_E_i, HIGH);
        }

        if (loQCount == 0)
        {
            CHECK_PIN_VALUE_EXIT(error, m_pinQ, s_Q_i, LOW);
        }

        if (hiQCount == 0)
        {
            CHECK_PIN_VALUE_EXIT(error, m_pinQ, s_Q_i, HIGH);
        }
    }

Exit:
    return error;
}


UINT8
C6809EClockMasterCpu::dataBusWidth(
    UINT32 address
)
{
    return 1;
}


UINT8
C6809EClockMasterCpu::dataAccessWidth(
    UINT32 address
)
{
    return 1;
}


PERROR
C6809EClockMasterCpu::memoryReadWrite(
    UINT32 address,
    UINT16 *data,
    int    readWrite
)
{
    PERROR error = errorSuccess;
    bool interruptsDisabled = false;
    int valueE;
    int valueQ;

    //
    // Phase 0 - Initial State
    // - Wait for E-Lo, Q-Lo
    // - E-falling
    //
    for (int x = 0 ; x < 100 ; x++)
    {
        valueE = m_pinE.digitalRead();
        valueQ = m_pinQ.digitalRead();

        if ( (valueE == LOW) &&
             (valueQ == LOW) )
        {
            break;
        }

        m_pinClock.digitalWriteHIGH();
        m_pinClock.digitalWriteLOW();
    }
    CHECK_LITERAL_VALUE_EXIT(error, s_E_i, valueE, LOW);
    CHECK_LITERAL_VALUE_EXIT(error, s_Q_i, valueQ, LOW);

    //
    // Phase 0 Actions
    // - Drive RW, A, BA, BS onto the bus.
    //
    m_busA.pinMode(OUTPUT);
    m_busA.digitalWrite(address);

    //
    // Driving D on write is due in Phase 1 however
    // many systems treat Q high time as the active
    // access time so taking care of D here reduces
    // the effective cycle time.
    //
    if (readWrite == LOW)
    {
        m_pinRW.digitalWriteLOW();
        m_busD.pinMode(OUTPUT);
        m_busD.digitalWrite(*data);
    }

    // Critical timing section
    noInterrupts();
    interruptsDisabled = true;

    //
    // Phase 1
    // - Wait for E-Lo, Q-Hi
    // - Q-rising
    //
    for (int x = 0 ; x < 100 ; x++)
    {
        valueQ = m_pinQ.digitalRead();

        if (valueQ == HIGH)
        {
            break;
        }

        m_pinClock.digitalWriteHIGH();
        m_pinClock.digitalWriteLOW();
    }
    CHECK_PIN_VALUE_EXIT(error, m_pinE, s_E_i, LOW);
    CHECK_LITERAL_VALUE_EXIT(error, s_Q_i, valueQ, HIGH);

    //
    // Phase 1 Actions
    // - On write, drive D (see note above)
    //

    //
    // Phase 2
    // - Wait for E-Hi, Q-Hi
    // - E-rising
    //
    for (int x = 0 ; x < 100 ; x++)
    {
        valueE = m_pinE.digitalRead();

        if (valueE == HIGH)
        {
            break;
        }

        m_pinClock.digitalWriteHIGH();
        m_pinClock.digitalWriteLOW();
    }
    CHECK_LITERAL_VALUE_EXIT(error, s_E_i, valueE, HIGH);
    CHECK_PIN_VALUE_EXIT(error, m_pinQ, s_Q_i, HIGH);

    //
    // Phase 2 Actions
    // - None, currently.
    //

    //
    // Phase 3
    // - Wait for E-Hi, Q-Lo
    // - Q-falling
    //
    for (int x = 0 ; x < 100 ; x++)
    {
        valueQ = m_pinQ.digitalRead();

        if (valueQ == LOW)
        {
            break;
        }

        m_pinClock.digitalWriteHIGH();
        m_pinClock.digitalWriteLOW();
    }
    CHECK_PIN_VALUE_EXIT(error, m_pinE, s_E_i, HIGH);
    CHECK_LITERAL_VALUE_EXIT(error, s_Q_i, valueQ, LOW);

    //
    // Phase 0 (Initial State)
    // - Wait for E-Lo, Q-Lo
    // - E-falling
    //
    for (int x = 0 ; x < 100 ; x++)
    {
        valueE = m_pinE.digitalRead();

        if (valueE == LOW)
        {
            break;
        }

        //
        // Since data-in is latched on the falling edge of E we
        // read in the data before every clock pulse so we have
        // the latest data before E transitions.
        //
        if (readWrite == HIGH)
        {
            m_busD.digitalRead(data);
        }

        m_pinClock.digitalWriteHIGH();
        m_pinClock.digitalWriteLOW();
    }
    CHECK_LITERAL_VALUE_EXIT(error, s_E_i, valueE, LOW);
    CHECK_PIN_VALUE_EXIT(error, m_pinQ, s_Q_i, LOW);

    //
    // Phase 0 Actions
    // - On writes, tri-state D
    //

    if (readWrite == LOW)
    {
        m_busD.pinMode(INPUT);
        m_pinRW.digitalWriteHIGH();
    }

Exit:
    if (interruptsDisabled)
    {
        interrupts();
    }

    return error;
}

PERROR
C6809EClockMasterCpu::memoryRead(
    UINT32 address,
    UINT16 *data
)
{
    return memoryReadWrite(address, data, HIGH);
}

PERROR
C6809EClockMasterCpu::memoryWrite(
    UINT32 address,
    UINT16 data
)
{
    return memoryReadWrite(address, &data, LOW);
}


PERROR
C6809EClockMasterCpu::waitForInterrupt(
    Interrupt interrupt,
    bool      active,
    UINT32    timeoutInClockPulses
)
{
    return errorNotImplemented;
}


PERROR
C6809EClockMasterCpu::acknowledgeInterrupt(
    UINT16 *response
)
{
    return errorNotImplemented;
}

//
// Pulse the clock pin high.
//
void
C6809EClockMasterCpu::clockPulse(
)
{
    m_pinClock.digitalWriteHIGH();
    m_pinClock.digitalWriteLOW();
}



