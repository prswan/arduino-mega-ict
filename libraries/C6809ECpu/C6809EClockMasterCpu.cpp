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
#include "C6809EPinOut.h"
#include "PinMap.h"


//
// External master clock on J14 AUX pin 8 (next to the 2-pin GND pin).
//
static const CONNECTION s_Clock_o    = {  8, "Clock"    };


C6809EClockMasterCpu::C6809EClockMasterCpu(const C6809EPinOut *pinOut
) : m_pinOut(pinOut),
    m_busA(g_pinMap40DIL, pinOut->m_A_ot,  ARRAYSIZE(pinOut->m_A_ot)),
    m_busD(g_pinMap40DIL, pinOut->m_D_iot, ARRAYSIZE(pinOut->m_D_iot)),
    m_pinBA(g_pinMap40DIL, &pinOut->m_BA_o),
    m_pinRW(g_pinMap40DIL, &pinOut->m_RW_o),
    m_pinE(g_pinMap40DIL, &pinOut->m_E_i),
    m_pinQ(g_pinMap40DIL, &pinOut->m_Q_i),
    m_pinClock(g_pinMap8Aux, &s_Clock_o)
{
};

//
// The idle function sets up the pins into the correct direction (input/output)
// and idle state ready for the next bus cycle.
// Some pins are not defined for some packages.
//
PERROR
C6809EClockMasterCpu::idle(
)
{
    if (m_pinOut->m_GND_i.pin != 0)
    {
        pinMode(g_pinMap40DIL[m_pinOut->m_GND_i.pin], INPUT_PULLUP);
    }

    pinMode(g_pinMap40DIL[m_pinOut->m__NMI_i.pin],           INPUT);
    pinMode(g_pinMap40DIL[m_pinOut->m__IRQ_i.pin],           INPUT);
    pinMode(g_pinMap40DIL[m_pinOut->m__FIRQ_i.pin],          INPUT);

    if (m_pinOut->m_BS_o.pin != 0)
    {
        digitalWrite(g_pinMap40DIL[m_pinOut->m_BS_o.pin],      LOW);
        pinMode(g_pinMap40DIL[m_pinOut->m_BS_o.pin],        OUTPUT);
    }

    pinMode(g_pinMap40DIL[m_pinOut->m_VCC_i.pin],            INPUT);

    if (m_pinOut->m_AVMA_o.pin != 0)
    {
        digitalWrite(g_pinMap40DIL[m_pinOut->m_AVMA_o.pin],   HIGH);
        pinMode(g_pinMap40DIL[m_pinOut->m_AVMA_o.pin],      OUTPUT);
    }

    pinMode(g_pinMap40DIL[m_pinOut->m__RESET_i.pin],         INPUT);

    if (m_pinOut->m_LIC_o.pin != 0)
    {
        digitalWrite(g_pinMap40DIL[m_pinOut->m_LIC_o.pin],    HIGH);
        pinMode(g_pinMap40DIL[m_pinOut->m_LIC_o.pin],       OUTPUT);
    }

    pinMode(g_pinMap40DIL[m_pinOut->m_TSC_i.pin],     INPUT_PULLUP);

    if (m_pinOut->m__HALT_i.pin != 0)
    {
        pinMode(g_pinMap40DIL[m_pinOut->m__HALT_i.pin],      INPUT);
    }

    // Start with pulled high to verify the bus is floating.
    m_busA.pinMode(INPUT_PULLUP);

    // Start with pulled high to verify the bus is floating.
    m_busD.pinMode(INPUT_PULLUP);

    // Set the BA pin to output high for idle state
    m_pinBA.digitalWriteHIGH();
    m_pinBA.pinMode(OUTPUT);

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
    if (m_pinOut->m_GND_i.pin != 0)
    {
        CHECK_VALUE_EXIT(error, g_pinMap40DIL, m_pinOut->m_GND_i, LOW);
    }

    // The Vcc pin should be high (power is on).
    CHECK_VALUE_EXIT(error, g_pinMap40DIL, m_pinOut->m_VCC_i, HIGH);

    // The halt pin should be high (running).
    if (m_pinOut->m__HALT_i.pin != 0)
    {
        CHECK_VALUE_EXIT(error, g_pinMap40DIL, m_pinOut->m__HALT_i, HIGH);
    }

    // In everything we'll be testing, TSC is pulled low.
    CHECK_VALUE_EXIT(error, g_pinMap40DIL, m_pinOut->m_TSC_i, LOW);

    // The address bus should be uncontended and pulled high.
    CHECK_BUS_VALUE_UINT16_EXIT(error, m_busA, m_pinOut->m_A_ot, 0xFFFF);

    // The data bus should be uncontended and pulled high.
    // We can't do this because on Star Wars the idle bus is a read of 0xFFFF == 0x61.
    //
    // CHECK_BUS_VALUE_UINT8_EXIT(error, m_busD, s_D_iot, 0xFF);

    // Loop to detect that reset clears
    // On Star Wars this ~0x40000 (262,144) clocks.
    // On exit the reset pin should be high (no reset).
    //
    {
        for (UINT32 i = 0 ; i < 0x41000 ; i++)
        {
            int value = ::digitalRead(g_pinMap40DIL[m_pinOut->m__RESET_i.pin]);

            if (value == HIGH)
            {
                break;
            }

            m_pinClock.digitalWriteHIGH();
            m_pinClock.digitalWriteLOW();
        }
    }
    CHECK_VALUE_EXIT(error, g_pinMap40DIL, m_pinOut->m__RESET_i, HIGH);

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
            CHECK_PIN_VALUE_EXIT(error, m_pinE, m_pinOut->m_E_i, LOW);
        }

        if (hiECount == 0)
        {
            CHECK_PIN_VALUE_EXIT(error, m_pinE, m_pinOut->m_E_i, HIGH);
        }

        if (loQCount == 0)
        {
            CHECK_PIN_VALUE_EXIT(error, m_pinQ, m_pinOut->m_Q_i, LOW);
        }

        if (hiQCount == 0)
        {
            CHECK_PIN_VALUE_EXIT(error, m_pinQ, m_pinOut->m_Q_i, HIGH);
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
    CHECK_LITERAL_VALUE_EXIT(error, m_pinOut->m_E_i, valueE, LOW);
    CHECK_LITERAL_VALUE_EXIT(error, m_pinOut->m_Q_i, valueQ, LOW);

    //
    // Phase 0 Actions
    // - Drive RW, A, BA, BS onto the bus.
    //
    m_busA.pinMode(OUTPUT);
    m_busA.digitalWrite(address);

    m_pinBA.digitalWriteLOW();

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
    CHECK_PIN_VALUE_EXIT(error, m_pinE, m_pinOut->m_E_i, LOW);
    CHECK_LITERAL_VALUE_EXIT(error, m_pinOut->m_Q_i, valueQ, HIGH);

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
    CHECK_LITERAL_VALUE_EXIT(error, m_pinOut->m_E_i, valueE, HIGH);
    CHECK_PIN_VALUE_EXIT(error, m_pinQ, m_pinOut->m_Q_i, HIGH);

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
    CHECK_PIN_VALUE_EXIT(error, m_pinE, m_pinOut->m_E_i, HIGH);
    CHECK_LITERAL_VALUE_EXIT(error, m_pinOut->m_Q_i, valueQ, LOW);

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
    CHECK_LITERAL_VALUE_EXIT(error, m_pinOut->m_E_i, valueE, LOW);
    CHECK_PIN_VALUE_EXIT(error, m_pinQ, m_pinOut->m_Q_i, LOW);

    //
    // Phase 0 Actions
    // - Set BA & address to idle 0xFFFF
    // - On writes, tri-state D
    //

    m_pinBA.digitalWriteHIGH();
    m_busA.digitalWrite(0xFFFF);

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



