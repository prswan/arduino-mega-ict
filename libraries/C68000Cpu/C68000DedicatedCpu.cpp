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
#include "C68000DedicatedCpu.h"
#include "PinMap68000.h"

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
// Viewing Assembly Output
//
// 1) At a CMD prompt change to the build output location
// e.g. C:\Users\paswan\AppData\Local\Temp\build64b5540e57fda5d1c290b65f1623a702.tmp\*
//      where the user name and magic number reflect the local configuration
//
// 2) "C:\Program Files (x86)\Arduino\hardware\tools\avr\avr\bin\objdump.exe" -S InCircuitTester68000.ino.elf >> asm.txt
//
// ** NOTE **
//
// The 68000 dedicated probe head should be used with a test shield fitted with 0-ohm links instead of
// the current limit resistor. The timing on 68000 is too fast to work properly with these protection resistors
// in place.
//

//
// Control Pins
//
//
static const CONNECTION s__BR_io     = { 13, "_BR"      };
static const CONNECTION s_Vcc_i      = { 14, "Vcc"      };
static const CONNECTION s__HALT_io   = { 17, "_HALT"    };
static const CONNECTION s__RESET_io  = { 18, "_RESET"   };
static const CONNECTION s__IPL2_i    = { 23, "_IPL2"    };
static const CONNECTION s__IPL1_i    = { 24, "_IPL1"    };
static const CONNECTION s__IPL0_i    = { 25, "_IPL0"    };

//
// Definitions for the processor function codes
//
static const UINT8 s_FC_UNDEF_000   = 0x00; // Reserved/undefined
static const UINT8 s_FC_USER_DATA   = 0x01; // User Data
static const UINT8 s_FC_USER_PROG   = 0x02; // User Program
static const UINT8 s_FC_UNDEF_003   = 0x03; // Reserved/undefined
static const UINT8 s_FC_UNDEF_004   = 0x04; // Reserved/undefined
static const UINT8 s_FC_SUPER_DATA  = 0x05; // Supervisor Data
static const UINT8 s_FC_SUPER_PROG  = 0x06; // Supervisor Program
static const UINT8 s_FC_CPU_SPACE   = 0x07; // CPU Space

//
// Address flag for 6800 synchronous bus mode
//
static const UINT32 s_vpaAddress = 0x04000000;


//
// Wait for CLK rising edge to be detected.
// This loop is 8 instructions total, 500ns.
//
#define WAIT_FOR_CLK_EDGE(x,r1,r2)         \
    {                                      \
        for ( ; x > 0 ; x--)               \
        {                                  \
            r1 = *g_portInControlIn;       \
            r2 = *g_portInControlIn;       \
                                           \
            if (!(r1 & s_BIT_IN_CLK) &&    \
                 (r2 & s_BIT_IN_CLK))      \
            {                              \
                break;                     \
            }                              \
        }                                  \
    }                                      \

//
// Wait for DTACK low.
// This loop is 2 instructions total, 125ns.
// Note that the system will hang here if DTACK is stuck.
//
#define WAIT_FOR_DTACK(x,r1,r2)            \
    {                                      \
        while(1)                           \
        {                                  \
            r1 = *g_portInControlIn;       \
                                           \
            if (!(r1 & s_BIT_IN_DTACK))    \
            {                              \
                break;                     \
            }                              \
        }                                  \
    }                                      \


//
// Wait for VPA low.
// This loop is 2 instructions total, 125ns.
// Note that the system will hang here if VPA is stuck.
//
#define WAIT_FOR_VPA(x,r1,r2)              \
    {                                      \
        while(1)                           \
        {                                  \
            r1 = *g_portInControlIn;       \
                                           \
            if (!(r1 & s_BIT_IN_VPA))      \
            {                              \
                break;                     \
            }                              \
        }                                  \
    }                                      \


C68000DedicatedCpu::C68000DedicatedCpu(bool useBusRequest
) : busRequest(useBusRequest)
{
};

//
// The idle function sets up the pins into the correct direction (input/output)
// and idle state ready for the next bus cycle.
//
PERROR
C68000DedicatedCpu::idle(
)
{
    // Using clip-over and Bus Request
    if (busRequest)
    {
        digitalWrite(g_pinMap68000[s__BR_io.pin], LOW);
        pinMode(g_pinMap68000[s__BR_io.pin], OUTPUT);
    }
    else
    {
        pinMode(g_pinMap68000[s__BR_io.pin], INPUT);
    }

    // Traditional Inputs
    pinMode(g_pinMap68000[s_Vcc_i.pin],    INPUT);
    pinMode(g_pinMap68000[s__HALT_io.pin], INPUT);
    pinMode(g_pinMap68000[s__IPL2_i.pin],  INPUT);
    pinMode(g_pinMap68000[s__IPL1_i.pin],  INPUT);
    pinMode(g_pinMap68000[s__IPL0_i.pin],  INPUT);

    // Control inputs are always input with no pullup
    *g_portOutControlIn   = s_PORT_BYTE_OFF;
    *g_dirControlIn       = s_DIR_BYTE_INPUT;

    //
    // Control outputs are always outputs
    // Except "E" is not Hi-Z during bus request so we must not drive it.
    //
    *g_portOutControlOutD = s_BYTE_OUT_IDLE_D;

    if (busRequest)
    {
        *g_dirControlOutD = (s_DIR_BYTE_OUTPUT ^ s_BIT_OUT_E);
    }
    else
    {
        *g_dirControlOutD = s_DIR_BYTE_OUTPUT;
    }

    *g_portOutControlOutL = s_BYTE_OUT_IDLE_L;
    *g_dirControlOutL     = s_DIR_BYTE_OUTPUT;

    // Set data to input with pullup for bus check.
    *g_portOutDataHi      = s_PORT_BYTE_PULLUP;
    *g_dirDataHi          = s_DIR_BYTE_INPUT;

    *g_portOutDataLo      = s_PORT_BYTE_PULLUP;
    *g_dirDataLo          = s_DIR_BYTE_INPUT;

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
C68000DedicatedCpu::check(
)
{
    PERROR error = errorSuccess;

    // The Vcc pin should be high (power is on).
    CHECK_VALUE_EXIT(error, g_pinMap68000, s_Vcc_i, HIGH);

    // The reset pin should be no reset.
    CHECK_VALUE_EXIT(error, g_pinMap68000, s__RESET_io, HIGH);

    // The halt pin should be no halt.
    CHECK_VALUE_EXIT(error, g_pinMap68000, s__HALT_io, HIGH);

    // Not using Bus Request
    if (!busRequest)
    {
        // The bus request pin should be no bus request.
        CHECK_VALUE_EXIT(error, g_pinMap68000, s__BR_io, HIGH);
    }

    {
        UINT16 data = ((UINT16) *g_portInDataHi << 8) | (*g_portInDataHi);
        CHECK_UINT16_VALUE_EXIT(error, "D", data, 0xFFFF);
    }

    // Loop to detect a clock by sampling and detecting both high and lows.
    {
        UINT16 clkCountHi = 0;
        UINT16 clkCountLo = 0;

        for (int i = 0 ; i < 1000 ; i++)
        {
            UINT8 controlIn = *g_portInControlIn;

            if (controlIn & s_BIT_IN_CLK)
            {
                clkCountHi++;
            }
            else
            {
                clkCountLo++;
            }
        }

        CHECK_BOOL_VALUE_EXIT(error, "CLK 15", (clkCountHi != 0), true);
        CHECK_BOOL_VALUE_EXIT(error, "CLK 15", (clkCountLo == 0), false);
    }

Exit:
    return error;
}


UINT8
C68000DedicatedCpu::dataBusWidth(
    UINT32 address
)
{
    // 16-bit data bus only.
    return 2;
}


UINT8
C68000DedicatedCpu::dataAccessWidth(
    UINT32 address
)
{
    // 8-bit only access.
    return 1;
}


PERROR
C68000DedicatedCpu::outputAddress(
    UINT32 address,
    bool   read
)
{
    PERROR error   = errorSuccess;
    UINT8 portOutL = s_BYTE_OUT_IDLE_L;
    UINT8 portOutD;
    UINT8 portInD;

    // We must set the RW line correctly in case there are external buffers
    portOutL = portOutL ^ (s_BIT_OUT_RW);

    // Add in the OE control to enable the address bus
    portOutL = portOutL ^ (s_BIT_OUT_LOE);

    *g_portOutControlOutL = portOutL;

    // Set the lower databus to output
    *g_dirDataLo          = s_DIR_BYTE_OUTPUT;

    // Output byte LE2
    portOutD = (address >> 16) & 0xFF;
    *g_portOutDataLo      = portOutD;
    *g_portOutDataLo      = portOutD; // Wait state
    *g_portOutControlOutL = (portOutL ^ s_BIT_OUT_LE2);
    *g_portOutControlOutL = (portOutL ^ s_BIT_OUT_LE2); // Wait state
    *g_portOutControlOutL = portOutL;

    portInD = *g_portInDataLo;
    CHECK_UINT8_VALUE_EXIT(error, "LE2", portInD, portOutD);

    // Output byte LE1
    portOutD = (address >>  8) & 0xFF;
    *g_portOutDataLo      = portOutD;
    *g_portOutDataLo      = portOutD; // Wait state
    *g_portOutControlOutL = (portOutL ^ s_BIT_OUT_LE1);
    *g_portOutControlOutL = (portOutL ^ s_BIT_OUT_LE1); // Wait state
    *g_portOutControlOutL = (portOutL);

    portInD = *g_portInDataLo;
    CHECK_UINT8_VALUE_EXIT(error, "LE1", portInD, portOutD);

    // Output byte LE0
    portOutD = (address >>  0) & 0xFF;
    *g_portOutDataLo      = portOutD;
    *g_portOutDataLo      = portOutD; // Wait state
    *g_portOutControlOutL = (portOutL ^ s_BIT_OUT_LE0);
    *g_portOutControlOutL = (portOutL ^ s_BIT_OUT_LE0); // Wait state
    *g_portOutControlOutL = (portOutL);

    portInD = *g_portInDataLo;
    CHECK_UINT8_VALUE_EXIT(error, "LE0", portInD, portOutD);

    // Turn off the data bus
    *g_dirDataLo          = s_DIR_BYTE_INPUT;

    // Revert RW to now set it based on the bus cycle
    portOutL = portOutL ^ s_BIT_OUT_RW;

    // Add in the RW line for a write.
    if (!read)
    {
        portOutL = portOutL ^ s_BIT_OUT_RW;
    }

    *g_portOutControlOutL = portOutL;

Exit:
    return error;
}


PERROR
C68000DedicatedCpu::memoryRead(
    UINT32 address,
    UINT16 *data
)
{
    PERROR error = errorSuccess;
    bool   lo    = (address & 1) ? true : false;
    bool   vpa   = (address & s_vpaAddress) ? true : false;

    // Critical timing section
    noInterrupts();

    error = outputAddress(address, true);
    if (FAILED(error))
    {
        goto Exit;
    }

    if (lo)
    {
        *g_dirDataLo = s_DIR_BYTE_INPUT;

        if (vpa)
        {
            error = readWriteLoVPA(data);
        }
        else
        {
            error = readWriteLoDTACK(data);
        }
    }
    else
    {
        *g_dirDataHi = s_DIR_BYTE_INPUT;

        if (vpa)
        {
            error = readWriteHiVPA(data);
        }
        else
        {
            error = readWriteHiDTACK(data);
        }
    }

    *g_portOutControlOutL =  s_BYTE_OUT_IDLE_L;

Exit:

    interrupts();

    return error;
}


PERROR
C68000DedicatedCpu::memoryWrite(
    UINT32 address,
    UINT16 data
)
{
    PERROR error = errorSuccess;
    bool   lo    = (address & 1) ? true : false;
    bool   vpa   = (address & s_vpaAddress) ? true : false;
    UINT16 dummyData;

    // Critical timing section
    noInterrupts();

    error = outputAddress(address, false);
    if (FAILED(error))
    {
        goto Exit;
    }

    if (lo)
    {
        *g_dirDataLo = s_DIR_BYTE_OUTPUT;
        *g_portOutDataLo = data & 0xFF;

        if (vpa)
        {
            error = readWriteLoVPA(&dummyData);
        }
        else
        {
            error = readWriteLoDTACK(&dummyData);
        }

        *g_dirDataLo  = s_DIR_BYTE_INPUT;
    }
    else
    {
        *g_dirDataHi = s_DIR_BYTE_OUTPUT;
        *g_portOutDataHi = data & 0xFF;

        if (vpa)
        {
            error = readWriteHiVPA(&dummyData);
        }
        else
        {
            error = readWriteHiDTACK(&dummyData);
        }

        *g_dirDataHi = s_DIR_BYTE_INPUT;
    }

    *g_portOutControlOutL =  s_BYTE_OUT_IDLE_L;

Exit:

    interrupts();

    return error;
}


PERROR
C68000DedicatedCpu::waitForInterrupt(
    Interrupt interrupt,
    bool      active,
    UINT32    timeoutInMs
)
{
    PERROR error = errorNotImplemented;
    return error;
}


//
// TBD.
//
PERROR
C68000DedicatedCpu::acknowledgeInterrupt(
    UINT16 *response
)
{
    PERROR error = errorSuccess;

    *response = 0;

    return error;
}


PERROR
C68000DedicatedCpu::readWriteLoDTACK(
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

    // Wait for the clock edge
    WAIT_FOR_CLK_EDGE(x,r1,r2);

    // Start the cycle by assert all the control lines
    *g_portOutControlOutD = s_BYTE_OUT_IDLE_D ^ (s_BIT_OUT_AS | s_BIT_OUT_LDS);
    *g_portOutControlOutD = s_BYTE_OUT_IDLE_D ^ (s_BIT_OUT_AS | s_BIT_OUT_LDS); // Wait state

    // Wait for DTACK to be asserted
    WAIT_FOR_DTACK(x,r1,r2);

    // Read in the data
    r1 = *g_portInDataLo;

    // Terminate the cycle
    *g_portOutControlOutD = s_BYTE_OUT_IDLE_D;

    // Check for timeout
    if (x == 0)
    {
        error = errorTimeout;
        goto Exit;
    }

    // Populate the output data word
    *data = r1;

Exit:

    return error;
}


PERROR
C68000DedicatedCpu::readWriteHiDTACK(
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

    // Wait for the clock edge
    WAIT_FOR_CLK_EDGE(x,r1,r2);

    // Start the cycle by assert all the control lines
    *g_portOutControlOutD = s_BYTE_OUT_IDLE_D ^ (s_BIT_OUT_AS | s_BIT_OUT_UDS);
    *g_portOutControlOutD = s_BYTE_OUT_IDLE_D ^ (s_BIT_OUT_AS | s_BIT_OUT_UDS); // Wait state

    // Wait for DTACK to be asserted
    WAIT_FOR_DTACK(x,r1,r2);

    // Read in the data
    r1 = *g_portInDataHi;

    // Terminate the cycle
    *g_portOutControlOutD = s_BYTE_OUT_IDLE_D;

    // Check for timeout
    if (x == 0)
    {
        error = errorTimeout;
        goto Exit;
    }

    // Populate the output data word
    *data = r1;

Exit:

    return error;
}



PERROR
C68000DedicatedCpu::readWriteLoVPA(
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

    // Wait for the clock edge
    WAIT_FOR_CLK_EDGE(x,r1,r2);

    // Start the cycle by assert all the control lines
    *g_portOutControlOutD = s_BYTE_OUT_IDLE_D ^ (s_BIT_OUT_AS | s_BIT_OUT_LDS);
    *g_portOutControlOutD = s_BYTE_OUT_IDLE_D ^ (s_BIT_OUT_AS | s_BIT_OUT_LDS); // Wait state

    // Wait for VPA to be asserted
    WAIT_FOR_VPA(x,r1,r2);

    // Assert VMA
    *g_portOutControlOutD = s_BYTE_OUT_IDLE_D ^ (s_BIT_OUT_AS | s_BIT_OUT_LDS | s_BIT_OUT_VMA);
    *g_portOutControlOutD = s_BYTE_OUT_IDLE_D ^ (s_BIT_OUT_AS | s_BIT_OUT_LDS | s_BIT_OUT_VMA); // Wait state
    *g_portOutControlOutD = s_BYTE_OUT_IDLE_D ^ (s_BIT_OUT_AS | s_BIT_OUT_LDS | s_BIT_OUT_VMA); // Wait state

    // Read in the data
    r1 = *g_portInDataLo;

    // Terminate the cycle
    *g_portOutControlOutD = s_BYTE_OUT_IDLE_D;

    // Check for timeout
    if (x == 0)
    {
        error = errorTimeout;
        goto Exit;
    }

    // Populate the output data word
    *data = r1;

Exit:

    return error;
}


PERROR
C68000DedicatedCpu::readWriteHiVPA(
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

    // Wait for the clock edge
    WAIT_FOR_CLK_EDGE(x,r1,r2);

    // Start the cycle by assert all the control lines
    *g_portOutControlOutD = s_BYTE_OUT_IDLE_D ^ (s_BIT_OUT_AS | s_BIT_OUT_UDS);
    *g_portOutControlOutD = s_BYTE_OUT_IDLE_D ^ (s_BIT_OUT_AS | s_BIT_OUT_UDS); // Wait state

    // Wait for VPA to be asserted
    WAIT_FOR_VPA(x,r1,r2);

    // Assert VMA
    *g_portOutControlOutD = s_BYTE_OUT_IDLE_D ^ (s_BIT_OUT_AS | s_BIT_OUT_UDS | s_BIT_OUT_VMA);
    *g_portOutControlOutD = s_BYTE_OUT_IDLE_D ^ (s_BIT_OUT_AS | s_BIT_OUT_UDS | s_BIT_OUT_VMA); // Wait state
    *g_portOutControlOutD = s_BYTE_OUT_IDLE_D ^ (s_BIT_OUT_AS | s_BIT_OUT_UDS | s_BIT_OUT_VMA); // Wait state

    // Read in the data
    r1 = *g_portInDataHi;

    // Terminate the cycle
    *g_portOutControlOutD = s_BYTE_OUT_IDLE_D;

    // Check for timeout
    if (x == 0)
    {
        error = errorTimeout;
        goto Exit;
    }

    // Populate the output data word
    *data = r1;

Exit:

    return error;
}

