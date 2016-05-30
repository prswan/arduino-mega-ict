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
#ifndef ICpu_h
#define ICpu_h

#include "Arduino.h"
#include "Error.h"

class ICpu
{
    public:

        //
        // Interrupt definitions. Most CPU's have an non-maskable & normal interrupt.
        // Additional less common interrupt sources are CPU specific with a general
        // convention that the numerical designations match the CPU, for example on
        // 8085 RST5==IRQ5 and on T11 CP1==IRQ1.
        //
        typedef enum {
            RESET,
            HALT,
            NMI,
            IRQ0,
            IRQ1,
            IRQ2,
            IRQ3,
            IRQ4,
            IRQ5,
            IRQ6,
            IRQ7
        } Interrupt;

        //
        // Set the CPU pins into default idle/inactive state.
        //
        virtual
        PERROR
        idle(
        ) = 0;

        //
        // Perform a basic pin check on the CPU pins, for example:
        // Vcc is high, GND is low, the address and data bus pullup high.
        //
        virtual
        PERROR
        check(
        ) = 0;

        //
        // Returns the data bus width (in bytes) for the specified address:
        // 1 = 1 Byte  =  8-Bit CPU
        // 2 = 2 Bytes = 16-Bit CPU
        //
        // This parameter is used to indicate indicate that the
        // address map of the CPU looks like this in the 2-byte case:
        //
        //   CPU address 0000 == Lower 8 physical address 0000
        //   CPU address 0001 == Upper 8 physical address 0000
        //   CPU address 0002 == Lower 8 physical address 0001
        //   CPU address 0003 == Upper 8 physical address 0001
        //
        // It provides a means for the caller to know how to access a specific
        // 8-bit component on a 16-bit bus (e.g. loops require address += 2).
        //
        virtual
        UINT8
        dataBusWidth(
            UINT32 address
        ) = 0;

        //
        // Returns the data access bus width (in bytes) for the specified address:
        // 1 = 1 Byte  (upper & lower always returned in the lower 8 bits)
        // 2 = 2 Bytes
        //
        // Therefore, for 16-bit access "dataBusWidth" == 2 and "dataAccessWidth" == 2.
        //
        virtual
        UINT8
        dataAccessWidth(
            UINT32 address
        ) = 0;

        //
        // Read one "data" byte from a memory "address" and return it in "byte".
        // 8-bit access is always in the lower 8 bits.
        //
        virtual
        PERROR
        memoryRead(
            UINT32 address,
            UINT16  *data
        ) = 0;

        //
        // Write one "data" byte to a memory "address".
        // 8-bit access is always in the lower 8 bits.
        //
        virtual
        PERROR
        memoryWrite(
            UINT32 address,
            UINT16 data
        ) = 0;

        //
        // Wait for a CPU interrupt to be asserted (active) or de-asserted (inactive).
        // If a timeout occurs then a timeout error is returned.
        //  - For standard asynchronous CPU's the timeout is measured in milliseconds.
        //  - For clock master CPU's the timeout is measured in clock pulses.
        // In most cases this will need to be called, acked, and then waited for the next interrupt.
        //
        virtual
        PERROR
        waitForInterrupt(
            Interrupt interrupt,
            bool      active,
            UINT32    timeoutInMsOrClockPulses
        ) = 0;

        //
        // Acknowledge the interrupt and perform a int ack cycle to acquire the reponse. If the CPU
        // does not support external response (levels or vectors) then the response returned is 0.
        //
        virtual
        PERROR
        acknowledgeInterrupt(
            UINT16 *response
        ) = 0;


};

#endif
