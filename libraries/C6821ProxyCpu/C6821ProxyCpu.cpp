//
// Copyright (c) 2024, Paul R. Swan
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
#include "C6821ProxyCpu.h"
#include "PinMap.h"

//
// Proxy interface for a 6821 used to drive peripherals, for example as used on Cat'n Mouse and
// Money Money sound PCBs where the AY-3-8910s are pin driven from a 6821.
//
// 6821 Port A - Bi-directional data bus, open collector.
// 6821 Port B - Output address & control bus.
//
// The ICpu interface doesn't interpret specific control lines for address, chip select or
// direction and it's up to the caller to supply the correct active "address" pattern for
// Port B to perform the read or write. The "postIdle" is used as the "inactive" pattern.
//

// Register Addresses
static const UINT8 MC6821_PR_DDR_A = 0x0;
static const UINT8 MC6821_CR_A     = 0x1;
static const UINT8 MC6821_PR_DDR_B = 0x2;
static const UINT8 MC6821_CR_B     = 0x3;

// Control Register Definitions
static const UINT8 MC6821_CR_DDR  = 0x0;
static const UINT8 MC6821_CR_PR   = 0x4;


C6821ProxyCpu::C6821ProxyCpu(
    ICpu  *cpu,
    UINT32 address,
    UINT8  portIdle
) : m_cpu(cpu),
    m_address(address),
    m_portIdle(portIdle)
{
};


//
// The idle function sets up the pins into the correct direction (input/output)
// and idle state ready for the next bus cycle.
//
PERROR
C6821ProxyCpu::idle(
)
{
    PERROR error = errorSuccess;

    // Set Port A input idle
    CHECK_CPU_WRITE_EXIT(error, m_cpu, m_address + MC6821_CR_A,     MC6821_CR_DDR);
    CHECK_CPU_WRITE_EXIT(error, m_cpu, m_address + MC6821_PR_DDR_A, 0x00);
    CHECK_CPU_WRITE_EXIT(error, m_cpu, m_address + MC6821_CR_A,     MC6821_CR_PR);

    // Set Port B output idle
    CHECK_CPU_WRITE_EXIT(error, m_cpu, m_address + MC6821_CR_B,     MC6821_CR_PR);
    CHECK_CPU_WRITE_EXIT(error, m_cpu, m_address + MC6821_PR_DDR_B, m_portIdle);
    CHECK_CPU_WRITE_EXIT(error, m_cpu, m_address + MC6821_CR_B,     MC6821_CR_DDR);
    CHECK_CPU_WRITE_EXIT(error, m_cpu, m_address + MC6821_PR_DDR_B, 0xFF);
    CHECK_CPU_WRITE_EXIT(error, m_cpu, m_address + MC6821_CR_B,     MC6821_CR_PR);

Exit:

    return error;
}


//
// The check function performs a basic pin check that all the outputs can be pulled high
// by the pullup resistor as a way to detect a GND short or pulled output.
//
PERROR
C6821ProxyCpu::check(
)
{
    PERROR error = errorSuccess;
    UINT16 data16 = 0;

    // Port A data set for input has a strong pullup
    CHECK_CPU_READ_EXIT(error, m_cpu, m_address + MC6821_PR_DDR_A, &data16);
    CHECK_UINT8_VALUE_EXIT(error, "PAf", data16, 0xFF);

    // Set Port A output data
    CHECK_CPU_WRITE_EXIT(error, m_cpu, m_address + MC6821_PR_DDR_A, 0xFF);
    CHECK_CPU_WRITE_EXIT(error, m_cpu, m_address + MC6821_CR_A,     MC6821_CR_DDR);
    CHECK_CPU_WRITE_EXIT(error, m_cpu, m_address + MC6821_PR_DDR_A, 0xFF);
    CHECK_CPU_WRITE_EXIT(error, m_cpu, m_address + MC6821_CR_A,     MC6821_CR_PR);

    // Output bus test walk - Port A read of an output pin returns the pin state
    for (UINT16 outData16 = 1 ; outData16 < 0xFF ; outData16 = outData16 * 2)
    {
        CHECK_CPU_WRITE_EXIT(error, m_cpu, m_address + MC6821_PR_DDR_A, outData16);

        CHECK_CPU_READ_EXIT(error, m_cpu, m_address + MC6821_PR_DDR_A, &data16);
        CHECK_UINT8_VALUE_EXIT(error, "PAx", data16, outData16);
    }

    // Set Port A back to default input
    CHECK_CPU_WRITE_EXIT(error, m_cpu, m_address + MC6821_PR_DDR_A, 0xFF);
    CHECK_CPU_WRITE_EXIT(error, m_cpu, m_address + MC6821_CR_A,     MC6821_CR_DDR);
    CHECK_CPU_WRITE_EXIT(error, m_cpu, m_address + MC6821_PR_DDR_A, 0x00);
    CHECK_CPU_WRITE_EXIT(error, m_cpu, m_address + MC6821_CR_A,     MC6821_CR_PR);

Exit:

    return error;
}


UINT8
C6821ProxyCpu::dataBusWidth(
    UINT32 address
)
{
    return 1;
}


UINT8
C6821ProxyCpu::dataAccessWidth(
    UINT32 address
)
{
    return 1;
}


PERROR
C6821ProxyCpu::memoryRead(
    UINT32 address,
    UINT16 *data
)
{
    PERROR error = errorSuccess;
    UINT16 data16 = 0;

    // Assert the address & control port bus
    CHECK_CPU_WRITE_EXIT(error, m_cpu, m_address + MC6821_PR_DDR_B, address);

    // Read the data port
    CHECK_CPU_READ_EXIT(error, m_cpu, m_address + MC6821_PR_DDR_A, &data16);

    // Set address & control port back to idle
    CHECK_CPU_WRITE_EXIT(error, m_cpu, m_address + MC6821_PR_DDR_B, m_portIdle);

Exit:

    *data = data16;

    return error;
}


PERROR
C6821ProxyCpu::memoryWrite(
    UINT32 address,
    UINT16 data
)
{
    PERROR error = errorSuccess;

    // Set Port A output data
    CHECK_CPU_WRITE_EXIT(error, m_cpu, m_address + MC6821_PR_DDR_A, data);
    CHECK_CPU_WRITE_EXIT(error, m_cpu, m_address + MC6821_CR_A,     MC6821_CR_DDR);
    CHECK_CPU_WRITE_EXIT(error, m_cpu, m_address + MC6821_PR_DDR_A, 0xFF);
    CHECK_CPU_WRITE_EXIT(error, m_cpu, m_address + MC6821_CR_A,     MC6821_CR_PR);

    // Assert the address & control port bus
    CHECK_CPU_WRITE_EXIT(error, m_cpu, m_address + MC6821_PR_DDR_B, address);

    // Set address & control port back to idle
    CHECK_CPU_WRITE_EXIT(error, m_cpu, m_address + MC6821_PR_DDR_B, m_portIdle);

    // Set Port A input idle
    CHECK_CPU_WRITE_EXIT(error, m_cpu, m_address + MC6821_CR_A,     MC6821_CR_DDR);
    CHECK_CPU_WRITE_EXIT(error, m_cpu, m_address + MC6821_PR_DDR_A, 0x00);
    CHECK_CPU_WRITE_EXIT(error, m_cpu, m_address + MC6821_CR_A,     MC6821_CR_PR);
    CHECK_CPU_WRITE_EXIT(error, m_cpu, m_address + MC6821_PR_DDR_A, 0x00);

Exit:

    return error;
}


PERROR
C6821ProxyCpu::waitForInterrupt(
    Interrupt interrupt,
    bool      active,
    UINT32    timeoutInMs
)
{
    return errorNotImplemented;
}


PERROR
C6821ProxyCpu::acknowledgeInterrupt(
    UINT16 *response
)
{
    PERROR error = errorSuccess;

    *response = 0;

    return error;
}
