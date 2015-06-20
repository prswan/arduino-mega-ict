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
        // Interrupt definitions. Most CPU's have an Non Maskable & normal interrupt
        //
        typedef enum {
            NMI,
            INT
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
        // Read one "data" byte from a memory "address" and return it in "byte".
        //
        // 16-Bit CPU:
        //   0x00000 -> 0x0FFFF is memory space
        //   0x10000 -> 0x1FFFF is IO space
        //
        virtual
        PERROR
        memoryRead(
            UINT32 address,
            UINT8  *data
        ) = 0;

        //
        // Write one "data" byte to a memory "address".
        // See "memoryRead" for address space information.
        //
        virtual
        PERROR
        memoryWrite(
            UINT32 address,
            UINT8  data
        ) = 0;

        //
        // Wait for the CPU interrupt to be asserted. If a timeout occurs then a timeout error is returned.
        // In most cases this will need to be called, acked, and then waited for the next interrupt.
        //
        virtual
        PERROR
        waitForInterrupt(
            Interrupt interrupt,
            UINT16    timeoutInMs
        ) = 0;

        //
        // Acknowledge the interrupt and perform a int ack cycle to acquire the reponse. If the CPU
        // does not support external response (levels or vectors) then the response returned is 0.
        //
        virtual
        PERROR
        acknowledgeInterrupt(
            UINT8 *response
        ) = 0;


};

#endif
