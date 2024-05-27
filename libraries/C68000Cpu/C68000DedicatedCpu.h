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
#ifndef C68000DedicatedCpu_h
#define C68000DedicatedCpu_h

#include "Arduino.h"
#include "ICpu.h"


class C68000DedicatedCpu : public ICpu
{
    public:

        //
        // Constructor
        //

        C68000DedicatedCpu(bool useBusRequest = true
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
        // The 68000 is a Big Endian 16-bit CPU.
        //
        // 68000 Address Space:
        // 0x00000000 -> 8-bit DTACK access, 0x*1 = Lo & 0x*0 == Hi.
        // 0x04000000 -> VPA access flag.
        //
        // 16-bit access is not currently supported.
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
        // C68000DedicatedCpu Interface
        //

    private:

        PERROR
        outputAddress(
            UINT32 address,
            bool   read
        );

        PERROR
        readWriteLoDTACK(
            UINT16 *data
        );

        PERROR
        readWriteHiDTACK(
            UINT16 *data
        );

        PERROR
        readWriteLoVPA(
            UINT16 *data
        );

        PERROR
        readWriteHiVPA(
            UINT16 *data
        );

    private:

        bool busRequest;

};

#endif
