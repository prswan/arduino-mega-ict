//
// Copyright (c) 2016, Paul R. Swan
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
#include "C6502ClockMasterCpu.h"
#include "PinMap.h"
#include "6502PinDescription.h"

//
// External master clock on J14 AUX pin 8 (next to the 2-pin GND pin).
//
static const CONNECTION s_Clock_o    = {  8, "Clock"    };


C6502ClockMasterCpu::C6502ClockMasterCpu(
) : m_busA(g_pinMap40DIL, s_A_ot,  ARRAYSIZE(s_A_ot)),
    m_busD(g_pinMap40DIL, s_D_iot, ARRAYSIZE(s_D_iot)),
    m_pinCLK0i(g_pinMap40DIL, &s_CLK0i_i),
    m_pinCLK1o(g_pinMap40DIL, &s_CLK1o_o),
    m_pinCLK2o(g_pinMap40DIL, &s_CLK2o_o),
    m_pinRDY(g_pinMap40DIL, &s_RDY_i),
    m_pinClock(g_pinMap8Aux, &s_Clock_o),
    m_valueCLK1o(-1), // Force initial state matching
    m_valueCLK2o(-1)  // Force initial state matching
{
};

//
// The idle function sets up the pins into the correct direction (input/output)
// and idle state ready for the next bus cycle.
//
PERROR
C6502ClockMasterCpu::idle(
)
{
    pinMode(g_pinMap40DIL[s_GND1_i.pin],           INPUT_PULLUP);

    pinMode(g_pinMap40DIL[s__IRQ_i.pin],           INPUT);
    pinMode(g_pinMap40DIL[s__NMI_i.pin],           INPUT);

    digitalWrite(g_pinMap40DIL[s_SYNC_o.pin],      HIGH);
    pinMode(g_pinMap40DIL[s_SYNC_o.pin],           OUTPUT);

    pinMode(g_pinMap40DIL[s_Vcc_i.pin],            INPUT);
    pinMode(g_pinMap40DIL[s_GND2_i.pin],           INPUT_PULLUP);

    digitalWrite(g_pinMap40DIL[s_R_W_o.pin],       HIGH);
    pinMode(g_pinMap40DIL[s_R_W_o.pin],            OUTPUT);

    pinMode(g_pinMap40DIL[s_SO_i.pin],             INPUT);

    pinMode(g_pinMap40DIL[s_RES_i.pin],            INPUT);

    // Use the pullup input as the float to detect shorts to ground.
    m_busA.pinMode(INPUT_PULLUP);
    m_busD.pinMode(INPUT_PULLUP);

    // Set the fast input pins to input
    m_pinRDY.pinMode(INPUT);
    m_pinCLK0i.pinMode(INPUT);

    // Set the clock pins to output and perform a single pulse to start.
    m_pinClock.pinMode(OUTPUT);
    m_pinCLK1o.pinMode(OUTPUT);
    m_pinCLK2o.pinMode(OUTPUT);

    clockPulse();

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
C6502ClockMasterCpu::check(
)
{
    PERROR error = errorSuccess;

    // The ground pin (with pullup) should be connected to GND (LOW)
    CHECK_VALUE_EXIT(error, s_GND1_i, LOW);
    CHECK_VALUE_EXIT(error, s_GND2_i, LOW);

    // The Vcc pin should be high (power is on).
    CHECK_VALUE_EXIT(error, s_Vcc_i, HIGH);

    // Nothing should be driving wait states.
    CHECK_VALUE_EXIT(error, s_RDY_i, HIGH);

    // The address bus should be uncontended and pulled high.
    CHECK_BUS_VALUE_UINT16_EXIT(error, m_busA, s_A_ot, 0xFFFF);

    // The data bus should be uncontended and pulled high.
    CHECK_BUS_VALUE_UINT8_EXIT(error, m_busD, s_D_iot, 0xFF);

    // Loop to detect that reset clears
    // On exit the reset pin should be high (no reset).
    //
    {
        for (UINT32 i = 0 ; i < 0x40000 ; i++)
        {
            int value = ::digitalRead(g_pinMap40DIL[s_RES_i.pin]);

            if (value == HIGH)
            {
                break;
            }

            clockPulse();
        }
    }
    CHECK_VALUE_EXIT(error, s_RES_i, HIGH);

    // Loop to detect a clock by sampling and detecting both high and lows.
    {
        UINT16 hiCount = 0;
        UINT16 loCount = 0;

        for (int i = 0 ; i < 0x400 ; i++)
        {
            int value = m_pinCLK0i.digitalRead();

            if (value == LOW)
            {
                loCount++;
            }
            else
            {
                hiCount++;
            }

            clockPulse();
        }

        if (loCount == 0)
        {
            CHECK_PIN_VALUE_EXIT(error, m_pinCLK0i, s_CLK0i_i, LOW);
        }
        else if (hiCount == 0)
        {
            CHECK_PIN_VALUE_EXIT(error, m_pinCLK0i, s_CLK0i_i, HIGH);
        }
    }

Exit:
    return error;
}


PERROR
C6502ClockMasterCpu::memoryRead(
    UINT32 address,
    UINT8  *data
)
{
    PERROR error = errorSuccess;
    bool interruptsDisabled = false;
    UINT16 data16 = 0;

    // Set a read cycle.
    digitalWrite(g_pinMap40DIL[s_R_W_o.pin], HIGH);

    // Enable the address bus and set the value (the lower 16 bits only)
    m_busA.pinMode(OUTPUT);
    m_busA.digitalWrite((UINT16) (address & 0xFFFF));

    // Set the databus to input.
    m_busD.pinMode(INPUT);

    // Critical timing section
    noInterrupts();
    interruptsDisabled = true;

    // Assert the clocks
    m_pinCLK1o.digitalWriteLOW();
    m_pinCLK2o.digitalWriteHIGH();

    // Poll RDY for cycle completion
    {
        int rdyValue;

        for (int i = 0 ; i < 64 ; i++)
        {
            rdyValue = m_pinRDY.digitalRead();

            if (rdyValue == HIGH)
            {
                // Read the data presented on the bus as soon as we see no RDY set then clear _RD
                m_busD.digitalReadThenDigitalWriteLOW(&data16, m_pinCLK2o);
                m_pinCLK1o.digitalWriteHIGH();

                break;
            }
        }

        CHECK_LITERAL_VALUE_EXIT(error, s_RDY_i, rdyValue, HIGH);
    }

Exit:

    if (interruptsDisabled)
    {
        interrupts();
    }

    *data = (UINT8) data16;

    return error;
}


PERROR
C6502ClockMasterCpu::memoryWrite(
    UINT32 address,
    UINT8  data
)
{
    PERROR error = errorSuccess;
    bool interruptsDisabled = false;

    // Set a write cycle.
    digitalWrite(g_pinMap40DIL[s_R_W_o.pin], LOW);

    // Enable the address bus and set the value.
    m_busA.pinMode(OUTPUT);
    m_busA.digitalWrite((UINT16) (address & 0xFFFF));

    // Set the databus to output and set a value.
    m_busD.pinMode(OUTPUT);
    m_busD.digitalWrite((UINT16) data);

    // Critical timing section
    noInterrupts();
    interruptsDisabled = true;

    // Assert the clocks
    m_pinCLK1o.digitalWriteLOW();
    m_pinCLK2o.digitalWriteHIGH();

    // Poll WAIT for cycle completion
    {
        int rdyValue;

        for (int i = 0 ; i < 64 ; i++)
        {
            rdyValue = m_pinRDY.digitalRead();

            if (rdyValue == HIGH)
            {
                m_pinCLK2o.digitalWriteLOW();
                m_pinCLK1o.digitalWriteHIGH();

                break;
            }
        }
        CHECK_LITERAL_VALUE_EXIT(error, s_RDY_i, rdyValue, HIGH);
    }

    // Set a read cycle.
    digitalWrite(g_pinMap40DIL[s_R_W_o.pin], HIGH);

Exit:

    if (interruptsDisabled)
    {
        interrupts();
    }

    return error;
}



PERROR
C6502ClockMasterCpu::waitForInterrupt(
    Interrupt interrupt,
    UINT16    timeoutInMs
)
{
    PERROR error = errorSuccess;
    unsigned long startTime = millis();
    unsigned long endTime = startTime + timeoutInMs;
    int value = 0;

    UINT8 intPin = ((interrupt == NMI) ? (g_pinMap40DIL[s__NMI_i.pin]) :
                                         (g_pinMap40DIL[s__IRQ_i.pin]));

    do
    {
        value = ::digitalRead(intPin);

        if (value == LOW)
        {
            break;
        }
    }
    while (millis() < endTime);

    if (value != LOW)
    {
        error = errorTimeout;
    }

Exit:

    return error;
}



//
//
//
PERROR
C6502ClockMasterCpu::acknowledgeInterrupt(
    UINT8 *response
)
{
/*
    digitalWrite(g_pinMap40DIL[s_M_IO_o.pin], LOW);

    digitalWrite(g_pinMap40DIL[s_INTACK_o.pin], HIGH);

    return read(0, response);
*/
}


//
// Pulse the clock pin high.
// The 6502 outputs CLK1 & CLK2 based on the transition on the CLK0 input.
// Therefore, every time a clock pulse occurs we must also check to see if
// CLK0 transitions and if so drive proper output state on CLK1 & CLK2.
// The "internal" state of CLK1 & CLK2 are used in place of the pin state
// for properly generating the bus cyle.
//
void
C6502ClockMasterCpu::clockPulse(
)
{
    m_pinClock.digitalWriteHIGH();
    m_pinClock.digitalWriteLOW();

    if (m_pinCLK0i.digitalRead() == HIGH)
    {
        // From the Synertek datasheet timing diagram on CLK0 Hi transitions CLK1 leads.
        if (m_valueCLK1o != LOW)
        {
            m_valueCLK1o = LOW;
            m_pinCLK1o.digitalWrite(m_valueCLK1o);
        }
        if (m_valueCLK2o != HIGH)
        {
            m_valueCLK2o = HIGH;
            m_pinCLK2o.digitalWrite(m_valueCLK2o);
        }
    }
    else
    {
        // From the Synertek datasheet timing diagram on CLK0 Lo transitions CLK2 leads.
        if (m_valueCLK2o != LOW)
        {
            m_valueCLK2o = LOW;
            m_pinCLK2o.digitalWrite(m_valueCLK2o);
        }
        if (m_valueCLK1o != HIGH)
        {
            m_valueCLK1o = HIGH;
            m_pinCLK1o.digitalWrite(m_valueCLK1o);
        }
    }
}

