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
#include "C8080DedicatedCpu.h"
#include "PinMap8080.h"

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
static const CONNECTION s_RESET_i   = { 12, "RESET"    };
static const CONNECTION s_INT_i     = { 14, "INT"      };
static const CONNECTION s_INTE_o    = { 16, "INTE"     };
static const CONNECTION s_Vcc_i     = { 20, "Vcc"      };

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


C8080DedicatedCpu::C8080DedicatedCpu(
)
{
};

//
// The idle function sets up the pins into the correct direction (input/output)
// and idle state ready for the next bus cycle.
//
PERROR
C8080DedicatedCpu::idle(
)
{
    // Traditional Inputs
    pinMode(g_pinMap8080[s_RESET_i.pin],         INPUT);
    pinMode(g_pinMap8080[s_INT_i.pin],           INPUT);
    pinMode(g_pinMap8080[s_Vcc_i.pin],           INPUT);
    pinMode(g_pinMap8080[s_GND_i.pin],           INPUT_PULLUP);

    // Traditional outputs - INTE Not supported - interrupts always enabled
    digitalWrite(g_pinMap8080[s_INTE_o.pin],     HIGH);
    pinMode(g_pinMap8080[s_INTE_o.pin],          OUTPUT);

    //
    // Control inputs are always input with no pullup except CLK1 & CLK2
    // to give the rising edge a minor boost.
    //
    *g_portOutControlIn  = (s_BIT_IN_CLK1 | s_BIT_IN_CLK2);
    *g_dirControlIn      = s_DIR_BYTE_INPUT;

    // Set address to input with pullup for bus check.
    *g_portOutAddressHi  = s_PORT_BYTE_PULLUP;
    *g_dirAddressHi      = s_DIR_BYTE_INPUT;
    *g_portOutAddressLo  = s_PORT_BYTE_PULLUP;
    *g_dirAddressLo      = s_DIR_BYTE_INPUT;

    // Set data to input with pullup for bus check.
    *g_portOutData       = s_PORT_BYTE_PULLUP;
    *g_dirData           = s_DIR_BYTE_INPUT;

    // Control outputs are always outputs - !WR is inactive and thus high.
    *g_portOutControlOut = s_BIT_OUT_WR;
    *g_dirControlOut     = s_DIR_BYTE_OUTPUT;

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
C8080DedicatedCpu::check(
)
{
    PERROR error = errorSuccess;

    // The ground pin (with pullup) should be connected to GND (LOW)
    CHECK_VALUE_EXIT(error, g_pinMap8080, s_GND_i, LOW);

    // The Vcc pin should be high (power is on).
    CHECK_VALUE_EXIT(error, g_pinMap8080, s_Vcc_i, HIGH);

    // The reset pin should be no reset.
    CHECK_VALUE_EXIT(error, g_pinMap8080, s_RESET_i, LOW);

    {
        // The tester doesn't support bus sharing.
        UINT8 controlIn = *g_portInControlIn;
        CHECK_BOOL_VALUE_EXIT(error, "HOLD 13", (controlIn & s_BIT_IN_HOLD), false);
    }

    {
        UINT16 address = ((UINT16) *g_portInAddressHi << 8) | (*g_portInAddressLo);
        CHECK_UINT16_VALUE_EXIT(error, "A", address, 0xFFFF);
    }

    {
        UINT8 data = *g_portInData;
        CHECK_UINT8_VALUE_EXIT(error, "D", data, 0xFF);
    }

    // Loop to detect a clock by sampling and detecting both high and lows.
    {
        UINT16 clk1CountHi = 0;
        UINT16 clk1CountLo = 0;
        UINT16 clk2CountHi = 0;
        UINT16 clk2CountLo = 0;

        for (int i = 0 ; i < 1000 ; i++)
        {
            UINT8 controlIn = *g_portInControlIn;

            if (controlIn & s_BIT_IN_CLK1)
            {
                clk1CountHi++;
            }
            else
            {
                clk1CountLo++;
            }

            if (controlIn & s_BIT_IN_CLK2)
            {
                clk2CountHi++;
            }
            else
            {
                clk2CountLo++;
            }
        }

        CHECK_BOOL_VALUE_EXIT(error, "CLK1 22", (clk1CountHi != 0), true);
        CHECK_BOOL_VALUE_EXIT(error, "CLK1 22", (clk1CountLo == 0), false);

        CHECK_BOOL_VALUE_EXIT(error, "CLK2 15", (clk2CountHi != 0), true);
        CHECK_BOOL_VALUE_EXIT(error, "CLK2 15", (clk2CountLo == 0), false);
    }

Exit:
    return error;
}


UINT8
C8080DedicatedCpu::dataBusWidth(
    UINT32 address
)
{
    return 1;
}


UINT8
C8080DedicatedCpu::dataAccessWidth(
    UINT32 address
)
{
    return 1;
}


void
C8080DedicatedCpu::outputAddressAndStatus(
    UINT32 address,
    bool   read
)
{
    UINT8 procState = 0x00;

    bool io = (address & 0x010000) ? true : false;

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
    *g_portOutAddressHi = ((address >> 8) & 0xFF);
    *g_portOutAddressLo = ((address >> 0) & 0xFF);

    *g_dirAddressHi = s_DIR_BYTE_OUTPUT;
    *g_dirAddressLo = s_DIR_BYTE_OUTPUT;

    // Output the processor status on the data bus.
    *g_portOutData = procState;
    *g_dirData     = s_DIR_BYTE_OUTPUT;

}


PERROR
C8080DedicatedCpu::memoryRead(
    UINT32 address,
    UINT16 *data
)
{
    PERROR error = errorSuccess;

    outputAddressAndStatus(address, true);

    //
    // These are all ignored by the compiler because the port address are all
    // less than 0xF and thus are rolled into the IO instruction itself.
    // i.e. ports A through D are faster to work with. That was lucky.
    //
    register volatile UINT8 * const portOutControlOut = g_portOutControlOut;
    register volatile UINT8 * const portInControlIn   = g_portInControlIn;
    register volatile UINT8 * const portInData        = g_portInData;
    register volatile UINT8 * const dirData           = g_dirData;

    register const UINT8 bitInCLK2      = (s_BIT_IN_CLK2);
    register const UINT8 bitInREADY     = (s_BIT_IN_READY);

    register const UINT8 byteOutWR      = (s_BIT_OUT_WR);
    register const UINT8 byteOutWR_SYNC = (s_BIT_OUT_WR |  s_BIT_OUT_SYNC);
    register const UINT8 byteOutWR_DBIN = (s_BIT_OUT_WR |  s_BIT_OUT_DBIN);

    //
    // Using separate counts saves 4 instructions because the compiler
    // optimizes out the pre-loop test (it knows the 1st time x > 0).
    //
    register UINT8 x = 255;
    register UINT8 y = 255;

    register UINT8 r1;
    register UINT8 r2;

    // Critical timing section
    noInterrupts();

    //
    // Wait for CLK2 rising edge to be detected.
    // This loop is 8 instructions total, 500ns.
    //
    for ( ; x > 0 ; x--)
    {
        r1 = *portInControlIn;
        r2 = *portInControlIn;

        if (!(r1 & bitInCLK2) &&
             (r2 & bitInCLK2))
        {
            break;
        }
    }

    //
    // Assert SYNC and use dummy writes to hold it.
    //
    // WARNING: This is manually tuned with a scope!
    //
    // According to the 8080 datasheet, SYNC is asserted
    // during CLK2-Hi, held, and de-asserted during the next
    // CLK2-Hi. Space Invaders latches the CPU status using CLK1
    // that pules high in between them, thus:-
    //
    // CLK2  ----    ----    ----    ----    ----    ----
    //       |  |    |  |    |  |    |  |    |  |    |  |
    //      -    ----    ----    ----    ----    ----    -
    //
    // CLK1       --      --      --      --      --
    //            ||      ||      ||      ||      ||
    //      ------  ------  ------  ------  ------  ------
    //
    // SYNC    --------
    //         |      |
    //      ---        ------------------------------------
    //
    // The timing below manually extends SYNC to fall in the CLK2-Hi period.
    //
    *portOutControlOut = byteOutWR_SYNC; //  62.5ns
    *portOutControlOut = byteOutWR_SYNC; // 125.0ns
    *portOutControlOut = byteOutWR_SYNC; // 187.5ns
    *portOutControlOut = byteOutWR_SYNC; // 250.0ns

    // Deassert SYNC.
    *portOutControlOut = byteOutWR;

    // Switch the data bus to input.
    *dirData = s_DIR_BYTE_INPUT;
    *portOutControlOut = byteOutWR_DBIN;

    //
    // Wait for READY high.
    // This loop is 4 instructions total, 250ns.
    //
    for ( ; y > 0 ; y--)
    {
        r1 = *portInControlIn;

        if (r1 & bitInREADY)
        {
            break;
        }
    }

    // Read in the data
    r1 = *portInData;

    // Deassert DBIN and set data out.
    *portOutControlOut = byteOutWR;
    *dirData = s_DIR_BYTE_OUTPUT;

    if ((x == 0) ||
        (y == 0))
    {
        error = errorTimeout;
        goto Exit;
    }

    *data = r1;

Exit:

    interrupts();

    return error;
}


PERROR
C8080DedicatedCpu::memoryWrite(
    UINT32 address,
    UINT16 data
)
{
    PERROR error = errorSuccess;

    outputAddressAndStatus(address, false);

    //
    // These are all ignored by the compiler because the port address are all
    // less than 0xF and thus are rolled into the IO instruction itself.
    // i.e. ports A through D are faster to work with. That was lucky.
    //
    register volatile UINT8 * const portOutControlOut = g_portOutControlOut;
    register volatile UINT8 * const portInControlIn   = g_portInControlIn;
    register volatile UINT8 * const portOutData       = g_portOutData;

    register const UINT8 bitInCLK2      = (s_BIT_IN_CLK2);
    register const UINT8 bitInREADY     = (s_BIT_IN_READY);

    register const UINT8 byteOutWR      = (s_BIT_OUT_WR);
    register const UINT8 byteOutWR_SYNC = (s_BIT_OUT_WR |  s_BIT_OUT_SYNC);
    register const UINT8 byteOut0       = 0;

    //
    // Using separate counts saves 4 instructions because the compiler
    // optimizes out the pre-loop test (it knows the 1st time x > 0).
    //
    register UINT8 x = 255;
    register UINT8 y = 255;

    register UINT8 r1;
    register UINT8 r2;

    // Critical timing section
    noInterrupts();

    //
    // Wait for CLK2 rising edge to be detected.
    // This loop is 8 instructions total, 500ns.
    //
    for ( ; x > 0 ; x--)
    {
        r1 = *portInControlIn;
        r2 = *portInControlIn;

        if (!(r1 & bitInCLK2) &&
             (r2 & bitInCLK2))
        {
            break;
        }
    }

    //
    // Assert SYNC and use dummy writes to hold it.
    // See the note in memoryRead above.
    //
    *portOutControlOut = byteOutWR_SYNC; //  62.5ns
    *portOutControlOut = byteOutWR_SYNC; // 125.0ns
    *portOutControlOut = byteOutWR_SYNC; // 187.5ns
    *portOutControlOut = byteOutWR_SYNC; // 250.0ns

    // Deassert SYNC.
    *portOutControlOut = byteOutWR;

    // Output the data and assert WR.
    *portOutData = (UINT8) data;
    *portOutControlOut = byteOut0;

    //
    // Wait for READY high.
    // This loop is 4 instructions total, 250ns.
    //
    for ( ; y > 0 ; y--)
    {
        r1 = *portInControlIn;

        if (r1 & bitInREADY)
        {
            break;
        }
    }

    // Deassert WR.
    *portOutControlOut = byteOutWR;

    if ((x == 0) ||
        (y == 0))
    {
        error = errorTimeout;
        goto Exit;
    }

Exit:

    interrupts();

    return error;
}


PERROR
C8080DedicatedCpu::waitForInterrupt(
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

    UINT8 intPin = g_pinMap8080[s_INT_i.pin];

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
C8080DedicatedCpu::acknowledgeInterrupt(
    UINT16 *response
)
{
    PERROR error = errorSuccess;

    *response = 0;

    return error;
}

