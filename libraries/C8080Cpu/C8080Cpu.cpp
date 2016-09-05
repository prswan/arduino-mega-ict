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
#include "C8080Cpu.h"
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
static const CONNECTION s_GND_i     = {  2, "GND"      };
static const CONNECTION s_Vbb_i     = { 11, "Vbb"      }; // -5V - Do Not Connect!
static const CONNECTION s_RESET_i   = { 12, "RESET"    };
static const CONNECTION s_HOLD_i    = { 13, "HOLD"     };
static const CONNECTION s_INT_i     = { 14, "INT"      };
static const CONNECTION s_CLK2_i    = { 15, "CLK2"     }; // +12V - Do Not Connect!
static const CONNECTION s_INTE_o    = { 16, "INTE"     };
static const CONNECTION s_DBIN_o    = { 17, "DBIN"     };
static const CONNECTION s__WR_o     = { 18, "_WR"      };
static const CONNECTION s_SYNC_o    = { 19, "SYNC"     };
static const CONNECTION s_Vcc_i     = { 20, "Vcc"      };
static const CONNECTION s_HLDA_o    = { 21, "HLDA"     };
static const CONNECTION s_CLK1_i    = { 22, "CLK1"     }; // +12V - Do Not Connect
static const CONNECTION s_READY_i   = { 23, "READY"    };
static const CONNECTION s_WAIT_o    = { 24, "WAIT"     };
static const CONNECTION s_Vdd_i     = { 28, "Vdd"      }; // +12V - Do Not Connect

//
// Bus pins
//
static const CONNECTION s_A_ot[]   = { {25, "A0"  },
                                       {26, "A1"  },
                                       {27, "A2"  },
                                       {29, "A3"  },
                                       {30, "A4"  },
                                       {31, "A5"  },
                                       {32, "A6"  },
                                       {33, "A7"  },
                                       {34, "A8"  },
                                       {35, "A9"  },
                                       { 1, "A10" },
                                       {40, "A11" },
                                       {37, "A12" },
                                       {38, "A13" },
                                       {39, "A14" },
                                       {36, "A15" } }; // 16 bits

static const CONNECTION s_D_iot[] = { {10, "D0" },
                                      { 9, "D1" },
                                      { 8, "D2" },
                                      { 7, "D3" },
                                      { 3, "D4" },
                                      { 4, "D5" },
                                      { 5, "D6" },
                                      { 6, "D7" } }; // 8 bits.
//
// Definitions for the processor state byte output during the
// SYNC phase.
//
static const UINT8 s_PS_INTA   = 0x01; // D0 - Interrupt Acknowledge
static const UINT8 s_PS_RD     = 0x02; // D1 - Read Cycle
static const UINT8 s_PS_STACK  = 0x04; // D2 - Stack Access
static const UINT8 s_PS_HLT    = 0x08; // D3 - Processor Halt
static const UINT8 s_PS_OUTP   = 0x10; // D4 - Output Port Write
static const UINT8 s_PS_M1     = 0x20; // D5 - Instruction Fetch Byte #1
static const UINT8 s_PS_INP    = 0x40; // D6 - Input Port Read
static const UINT8 s_PS_MEMRD  = 0x80; // D7 - Memory Read


C8080Cpu::C8080Cpu(
) : m_busA(g_pinMap40DIL, s_A_ot,  ARRAYSIZE(s_A_ot)),
    m_busD(g_pinMap40DIL, s_D_iot, ARRAYSIZE(s_D_iot)),
    m_pinDBIN(g_pinMap40DIL, &s_DBIN_o),
    m_pinSYNC(g_pinMap40DIL, &s_SYNC_o),
    m_pinREADY(g_pinMap40DIL, &s_READY_i),
    m_pin_WR(g_pinMap40DIL, &s__WR_o)
{
};

//
// The idle function sets up the pins into the correct direction (input/output)
// and idle state ready for the next bus cycle.
//
PERROR
C8080Cpu::idle(
)
{
    // None-TTL pins set to input pullup as not connected.
    pinMode(g_pinMap40DIL[s_CLK1_i.pin],          INPUT_PULLUP);
    pinMode(g_pinMap40DIL[s_CLK2_i.pin],          INPUT_PULLUP);
    pinMode(g_pinMap40DIL[s_Vdd_i.pin],           INPUT_PULLUP);
    pinMode(g_pinMap40DIL[s_Vbb_i.pin],           INPUT_PULLUP);

    // HLDA Not supported
    digitalWrite(g_pinMap40DIL[s_HLDA_o.pin],     LOW);
    pinMode(g_pinMap40DIL[s_HLDA_o.pin],          OUTPUT);

    // WAIT Not supported
    digitalWrite(g_pinMap40DIL[s_WAIT_o.pin],     LOW);
    pinMode(g_pinMap40DIL[s_WAIT_o.pin],          OUTPUT);

    // INTE Not supported - interrupts always enabled
    digitalWrite(g_pinMap40DIL[s_INTE_o.pin],     HIGH);
    pinMode(g_pinMap40DIL[s_INTE_o.pin],          OUTPUT);

    // Inputs Not supported
    pinMode(g_pinMap40DIL[s_HOLD_i.pin],          INPUT);

    // Inputs
    pinMode(g_pinMap40DIL[s_RESET_i.pin],         INPUT);
    pinMode(g_pinMap40DIL[s_INT_i.pin],           INPUT);
    pinMode(g_pinMap40DIL[s_Vcc_i.pin],           INPUT);
    pinMode(g_pinMap40DIL[s_GND_i.pin],           INPUT_PULLUP);

    m_pinDBIN.digitalWrite(LOW);
    m_pinDBIN.pinMode(OUTPUT);

    m_pinSYNC.digitalWrite(LOW);
    m_pinSYNC.pinMode(OUTPUT);

    m_pin_WR.digitalWrite(HIGH);
    m_pin_WR.pinMode(OUTPUT);

    m_pinREADY.pinMode(INPUT);

    // Use the pullup input as the float to detect shorts to ground.
    m_busA.pinMode(INPUT_PULLUP);
    m_busD.pinMode(INPUT_PULLUP);

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
C8080Cpu::check(
)
{
    PERROR error = errorSuccess;

    // The ground pin (with pullup) should be connected to GND (LOW)
    CHECK_VALUE_EXIT(error, g_pinMap40DIL, s_GND_i, LOW);

    // The Vcc pin should be high (power is on).
    CHECK_VALUE_EXIT(error, g_pinMap40DIL, s_Vcc_i, HIGH);

    // These should be unconnected and thus pulled high.
    CHECK_VALUE_EXIT(error, g_pinMap40DIL, s_CLK1_i, HIGH);
    CHECK_VALUE_EXIT(error, g_pinMap40DIL, s_CLK2_i, HIGH);
    CHECK_VALUE_EXIT(error, g_pinMap40DIL, s_Vdd_i, HIGH);
    CHECK_VALUE_EXIT(error, g_pinMap40DIL, s_Vbb_i, HIGH);

    // The reset pin should be no reset.
    CHECK_VALUE_EXIT(error, g_pinMap40DIL, s_RESET_i, LOW);

    // The tester doesn't support bus sharing.
    CHECK_VALUE_EXIT(error, g_pinMap40DIL, s_HOLD_i, LOW);

    // The address bus should be uncontended and pulled high.
    CHECK_BUS_VALUE_UINT16_EXIT(error, m_busA, s_A_ot, 0xFFFF);

    // The address/data bus should be uncontended and pulled high.
    CHECK_BUS_VALUE_UINT8_EXIT(error, m_busD, s_D_iot, 0xFF);

    // The clocks are none-TTL so we can't use the tester to verify the CPU clocks.
    // Use a scope instead.

Exit:
    return error;
}


UINT8
C8080Cpu::dataBusWidth(
    UINT32 address
)
{
    return 1;
}


UINT8
C8080Cpu::dataAccessWidth(
    UINT32 address
)
{
    return 1;
}


PERROR
C8080Cpu::memoryReadWrite(
    UINT32 address,
    UINT16 *data,
    bool   read
)
{
    PERROR error = errorSuccess;
    bool interruptsDisabled = false;
    UINT8 procState = 0x00;

    bool io        = (address & 0x010000) ? true : false;
    bool readySync = (address & 0x100000) ? true : false;

    //
    // IO addressing is 256 bytes that mirrors the low byte onto the high
    // byte of the address bus. Space Invaders uses this effect for address
    // decoding so we'll simulate that also.
    //
    if (io)
    {
        address = (address & ~0xFF00) | ((address & 0xFF) << 8);
    }

    // Determine the processor state byte
    if (read)
    {
        procState |= s_PS_RD;

        if (io)
        {
            procState |= s_PS_INP;
        }
        else
        {
            procState |= s_PS_MEMRD;
        }
    }
    else
    {
        if (io)
        {
            procState |= s_PS_OUTP;
        }
    }

    // Assert the address
    m_busA.pinMode(OUTPUT);
    m_busA.digitalWrite((UINT16) (address & 0xFFFF));

    // Output the proessor status on the data bus.
    m_busD.pinMode(OUTPUT);
    m_busD.digitalWrite(procState);

    // Critical timing section
    noInterrupts();
    interruptsDisabled = true;

    //
    // TODO: Implement support for synchronous READY timed cycles.
    // There is no way to access the video RAM at the present time without
    // this support :(
    //
    if (readySync)
    {
        error = errorNotImplemented;
        goto Exit;
    }

    // Pulse SYNC
    m_pinSYNC.digitalWriteHIGH();
    m_pinSYNC.digitalWriteLOW();

    // Perform the data access
    if (read)
    {
        m_busD.pinMode(INPUT);

        m_pinDBIN.digitalWriteHIGH();
        m_busD.digitalReadThenDigitalWriteLOW(data, m_pinDBIN);
    }
    else
    {
        m_busD.digitalWrite(*data & 0xFF);

        m_pin_WR.digitalWriteLOW();
        m_pin_WR.digitalWriteHIGH();
    }

Exit:

    if (interruptsDisabled)
    {
        interrupts();
    }

    return error;
}


PERROR
C8080Cpu::memoryRead(
    UINT32 address,
    UINT16 *data
)
{
    return memoryReadWrite(address, data, true);
}


PERROR
C8080Cpu::memoryWrite(
    UINT32 address,
    UINT16 data
)
{
    return memoryReadWrite(address, &data, false);
}


PERROR
C8080Cpu::waitForInterrupt(
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

    UINT8 intPin = g_pinMap40DIL[s_INT_i.pin];

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
C8080Cpu::acknowledgeInterrupt(
    UINT16 *response
)
{
    PERROR error = errorSuccess;

    *response = 0;

    return error;
}

