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
#include "C8085Cpu.h"
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
static const CONNECTION s_X1_i      = {  1, "X1"       };
static const CONNECTION s_X2_i      = {  2, "X2"       };
static const CONNECTION s_RESOUT_o  = {  3, "RESOUT"   };
static const CONNECTION s_SOD_o     = {  4, "SOD"      };
static const CONNECTION s_SID_i     = {  5, "SID"      };
static const CONNECTION s_TRAP_i    = {  6, "TRAP"     };
static const CONNECTION s_RST75_i   = {  7, "RST75"    };
static const CONNECTION s_RST65_i   = {  8, "RST65"    };
static const CONNECTION s_RST55_i   = {  9, "RST55"    };
static const CONNECTION s_INTR_i    = { 10, "INTR"     };
static const CONNECTION s__INTA_o   = { 11, "_INTA"    };
static const CONNECTION s_GND_i     = { 20, "GND"      };
static const CONNECTION s_S0_o      = { 29, "S0"       };
static const CONNECTION s_ALE_o     = { 30, "ALE"      };
static const CONNECTION s__WR_ot    = { 31, "_WR"      };
static const CONNECTION s__RD_ot    = { 32, "_RD"      };
static const CONNECTION s_S1_o      = { 33, "S1"       };
static const CONNECTION s_IO_M_ot   = { 34, "IO_M"     };
static const CONNECTION s_READY_i   = { 35, "READY"    };
static const CONNECTION s__RESIN_i  = { 36, "_RESIN"   };
static const CONNECTION s_CLKOUT_o  = { 37, "CLKOUT"   };
static const CONNECTION s_HLDA_o    = { 38, "HLDA"     };
static const CONNECTION s_HOLD_i    = { 39, "HOLD"     };
static const CONNECTION s_Vcc_i     = { 40, "Vcc"      };

//
// Bus pins
//
static const CONNECTION s_A_ot[]   = { {21, "A8"  },
                                       {22, "A9"  },
                                       {23, "A10" },
                                       {24, "A11" },
                                       {25, "A12" },
                                       {26, "A13" },
                                       {27, "A14" },
                                       {28, "A15" } }; // Upper 8 of 16 bits

static const CONNECTION s_AD_iot[] = { {12, "AD0" },
                                       {13, "AD1" },
                                       {14, "AD2" },
                                       {15, "AD3" },
                                       {16, "AD4" },
                                       {17, "AD5" },
                                       {18, "AD6" },
                                       {19, "AD7" } }; // 8 bits.


C8085Cpu::C8085Cpu(
) : m_busA(g_pinMap40DIL, s_A_ot,  ARRAYSIZE(s_A_ot)),
    m_busAD(g_pinMap40DIL, s_AD_iot, ARRAYSIZE(s_AD_iot)),
    m_pinIO_M(g_pinMap40DIL, &s_IO_M_ot),
    m_pinS0(g_pinMap40DIL, &s_S0_o),
    m_pinS1(g_pinMap40DIL, &s_S1_o),
    m_pinALE(g_pinMap40DIL, &s_ALE_o),
    m_pin_RD(g_pinMap40DIL, &s__RD_ot),
    m_pin_WR(g_pinMap40DIL, &s__WR_ot),
    m_pinREADY(g_pinMap40DIL, &s_READY_i)
{
};

//
// The idle function sets up the pins into the correct direction (input/output)
// and idle state ready for the next bus cycle.
//
PERROR
C8085Cpu::idle(
)
{
    pinMode(g_pinMap40DIL[s_X1_i.pin],            INPUT);
    pinMode(g_pinMap40DIL[s_X2_i.pin],            INPUT);

    // RESOUT Not supported
    digitalWrite(g_pinMap40DIL[s_RESOUT_o.pin],   LOW);
    pinMode(g_pinMap40DIL[s_RESOUT_o.pin],        OUTPUT);

    // SOD Not supported
    digitalWrite(g_pinMap40DIL[s_SOD_o.pin],      LOW);
    pinMode(g_pinMap40DIL[s_SOD_o.pin],           OUTPUT);

    // SID Not supported
    pinMode(g_pinMap40DIL[s_SID_i.pin],           INPUT);
    pinMode(g_pinMap40DIL[s_TRAP_i.pin],          INPUT);
    pinMode(g_pinMap40DIL[s_RST75_i.pin],         INPUT);
    pinMode(g_pinMap40DIL[s_RST65_i.pin],         INPUT);
    pinMode(g_pinMap40DIL[s_RST55_i.pin],         INPUT);
    pinMode(g_pinMap40DIL[s_INTR_i.pin],          INPUT);

    digitalWrite(g_pinMap40DIL[s__INTA_o.pin],    HIGH);
    pinMode(g_pinMap40DIL[s__INTA_o.pin],         OUTPUT);

    pinMode(g_pinMap40DIL[s_GND_i.pin],           INPUT);

    m_pinS0.digitalWrite(LOW);
    m_pinS0.pinMode(OUTPUT);

    m_pinALE.digitalWrite(LOW);
    m_pinALE.pinMode(OUTPUT);

    m_pin_WR.digitalWrite(HIGH);
    m_pin_WR.pinMode(OUTPUT);

    m_pin_RD.digitalWrite(HIGH);
    m_pin_RD.pinMode(OUTPUT);

    m_pinS1.digitalWrite(LOW);
    m_pinS1.pinMode(OUTPUT);

    m_pinIO_M.digitalWrite(LOW);
    m_pinIO_M.pinMode(OUTPUT);

    m_pinREADY.pinMode(INPUT);

    pinMode(g_pinMap40DIL[s__RESIN_i.pin],        INPUT);

    // CLKOUT Not supported
    digitalWrite(g_pinMap40DIL[s_CLKOUT_o.pin],   LOW);
    pinMode(g_pinMap40DIL[s_CLKOUT_o.pin],        OUTPUT);

    // HLDA Not supported
    digitalWrite(g_pinMap40DIL[s_HLDA_o.pin],     LOW);
    pinMode(g_pinMap40DIL[s_HLDA_o.pin],          OUTPUT);

    // HOLD Not supported
    pinMode(g_pinMap40DIL[s_HOLD_i.pin],          INPUT);
    pinMode(g_pinMap40DIL[s_Vcc_i.pin],           INPUT);

    // Use the pullup input as the float to detect shorts to ground.
    m_busA.pinMode(INPUT_PULLUP);
    m_busAD.pinMode(INPUT_PULLUP);

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
C8085Cpu::check(
)
{
    PERROR error = errorSuccess;

    // The ground pin (with pullup) should be connected to GND (LOW)
    CHECK_VALUE_EXIT(error, g_pinMap40DIL, s_GND_i, LOW);

    // The Vcc pin should be high (power is on).
    CHECK_VALUE_EXIT(error, g_pinMap40DIL, s_Vcc_i, HIGH);

    // The reset pin should be high (no reset).
    CHECK_VALUE_EXIT(error, g_pinMap40DIL, s__RESIN_i, HIGH);

    // The tester doesn't support bus sharing.
    CHECK_VALUE_EXIT(error, g_pinMap40DIL, s_HOLD_i, LOW);

    // The address bus should be uncontended and pulled high.
    CHECK_BUS_VALUE_UINT8_EXIT(error, m_busA, s_A_ot, 0xFF);

    // The address/data bus should be uncontended and pulled high.
    CHECK_BUS_VALUE_UINT8_EXIT(error, m_busAD, s_AD_iot, 0xFF);

    // Loop to detect a clock by sampling and detecting both high and lows.
    {
        UINT16 hiCount = 0;
        UINT16 loCount = 0;

        for (int i = 0 ; i < 1000 ; i++)
        {
            int value = ::digitalRead(g_pinMap40DIL[s_X1_i.pin]);

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
            CHECK_VALUE_EXIT(error, g_pinMap40DIL, s_X1_i, LOW);
        }
        else if (hiCount == 0)
        {
            CHECK_VALUE_EXIT(error, g_pinMap40DIL, s_X1_i, HIGH);
        }
    }

Exit:
    return error;
}


UINT8
C8085Cpu::dataBusWidth(
    UINT32 address
)
{
    return 1;
}


UINT8
C8085Cpu::dataAccessWidth(
    UINT32 address
)
{
    return 1;
}


PERROR
C8085Cpu::memoryReadWrite(
    UINT32 address,
    UINT16 *data,
    bool   read
)
{
    PERROR error = errorSuccess;
    bool interruptsDisabled = false;

    bool io        = (address & 0x010000) ? true : false;
    bool readySync = (address & 0x100000) ? true : false;

    // Assert the bus state
    if (io)
    {
        m_pinIO_M.digitalWriteHIGH();
    }

    if (read)
    {
        m_pinS1.digitalWriteHIGH();
    }
    else
    {
        m_pinS0.digitalWriteHIGH();
    }

    // Assert the upper address
    m_busA.pinMode(OUTPUT);
    m_busA.digitalWrite((UINT16) ((address >> 8) & 0xFF));

    // Assert the lower address
    m_busAD.pinMode(OUTPUT);
    m_busAD.digitalWrite((UINT16) (address & 0xFF));

    // Pulse ALE to latch the lower address
    m_pinALE.digitalWriteHIGH();
    m_pinALE.digitalWriteLOW();

    // Setup the data bus ready for access
    if (read)
    {
        m_busAD.pinMode(INPUT);
    }
    else
    {
        m_busAD.digitalWrite(*data & 0xFF);
    }

    // Critical timing section
    noInterrupts();
    interruptsDisabled = true;

    // Perform a ready sync if needed
    if (readySync)
    {
        int value;

        // Wait for active
        for (int i = 0 ; i < 8192 ; i++)
        {
            value = m_pinREADY.digitalRead();

            if (value == LOW)
            {
                break;
            }
        }
        CHECK_LITERAL_VALUE_EXIT(error, s_READY_i, value, LOW);

        // Wait for inactive
        for (int i = 0 ; i < 8192 ; i++)
        {
            value = m_pinREADY.digitalRead();

            if (value == HIGH)
            {
                break;
            }
        }
        CHECK_LITERAL_VALUE_EXIT(error, s_READY_i, value, HIGH);
    }

    // Perform the data access
    if (read)
    {
        m_pin_RD.digitalWriteLOW();
        m_busAD.digitalReadThenDigitalWriteHIGH(data, m_pin_RD);

        m_pinS1.digitalWriteLOW();
    }
    else
    {
        m_pin_WR.digitalWriteLOW();
        m_pin_WR.digitalWriteHIGH();

        m_busAD.pinMode(INPUT);

        m_pinS0.digitalWriteLOW();
    }

    if (io)
    {
        m_pinIO_M.digitalWriteLOW();
    }

Exit:

    if (interruptsDisabled)
    {
        interrupts();
    }

    return error;
}


PERROR
C8085Cpu::memoryRead(
    UINT32 address,
    UINT16 *data
)
{
    return memoryReadWrite(address, data, true);
}


PERROR
C8085Cpu::memoryWrite(
    UINT32 address,
    UINT16 data
)
{
    return memoryReadWrite(address, &data, false);
}


PERROR
C8085Cpu::waitForInterrupt(
    Interrupt interrupt,
    bool      active,
    UINT32    timeoutInMs
)
{
    PERROR error = errorSuccess;
    unsigned long startTime = millis();
    unsigned long endTime = startTime + timeoutInMs;
    int sense = (active ? HIGH : LOW);
    int value = 0;

    UINT8 intPin = ((interrupt == NMI) ? (g_pinMap40DIL[s_TRAP_i.pin]) :
                                         (g_pinMap40DIL[s_INTR_i.pin]));

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
C8085Cpu::acknowledgeInterrupt(
    UINT16 *response
)
{
    PERROR error = errorSuccess;

    *response = 0;

    return error;
}

