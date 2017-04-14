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
#ifndef C8080DedicatedCpu_h
#define C8080DedicatedCpu_h

#include "Arduino.h"
#include "ICpu.h"


class C8080DedicatedCpu : public ICpu
{
    public:

        //
        // Constructor
        //

        C8080DedicatedCpu(
        );

        // ICpu Interface
        //

        virtual PERROR idle(
        );

        virtual PERROR check(
        );

        virtual UINT8 dataBusWidth(
            UINT32 address
        );

        virtual UINT8 dataAccessWidth(
            UINT32 address
        );

        //
        // 8080 Address Space:
        // 0x00000 -> 0x0FFFF - Memory Mapped Data
        // 0x10000 -> 0x1FFFF - Input/Output Ports
        //
        // READY synchronization is always enabled.
        //
        virtual PERROR memoryRead(
            UINT32 address,
            UINT16 *data
        );

        virtual PERROR memoryWrite(
            UINT32 address,
            UINT16 data
        );

        virtual
        PERROR
        waitForInterrupt(
            Interrupt interrupt,
            bool      active,
            UINT32    timeoutInMs
        );

        virtual
        PERROR
        acknowledgeInterrupt(
            UINT16 *response
        );

        //
        // C8080DedicatedCpu Interface
        //

    private:

        void
        outputAddressAndStatus(
            UINT32 address,
            bool   read
        );

    private:

};

#endif
