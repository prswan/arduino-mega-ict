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
#ifndef C2650Cpu_h
#define C2650Cpu_h

#include "Arduino.h"
#include "ICpu.h"
#include "CBus.h"
#include "CFast8BitBus.h"
#include "CFastPin.h"


class C2650Cpu : public ICpu
{
    public:

        //
        // Constructor
        //

        C2650Cpu(
        );

        // ICpu Interface
        //

        virtual
        PERROR
        idle(
        );

        virtual
        PERROR
        check(
        );

        virtual
        UINT8
        dataBusWidth(
            UINT32 address
        );

        virtual
        UINT8
        dataAccessWidth(
            UINT32 address
        );

        // Special Addresses:
        //
        // |---------------------------------------|
        // | (A16) | (A15) |  A14  |  A13  |  A12  |
        // |---------------------------------------|-----------------------
        // |  (na) |  (na) |  C/D  |  E/NE |  A12  | Address | Description
        // |---------------------------------------|---------|-------------
        // |  1    |  0    |  0    |  1    |   0   | 0x120xx | EXT
        // |  1    |  0    |  0    |  0    |   0   | 0x10000 | CTRL
        // |  1    |  0    |  1    |  0    |   0   | 0x14000 | DATA
        // |---------------------------------------------------------------

        virtual
        PERROR
        memoryRead(
            UINT32 address,
            UINT16 *data
        );

        virtual
        PERROR
        memoryWrite(
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
        // C2650Cpu Interface
        //

        void
        flagWrite(
            UINT8 value
        );

        UINT8
        senseRead(
        );

    private:

        PERROR read(
            UINT32 address,
            UINT16 *data
        );

        PERROR write(
            UINT32 address,
            UINT16 data
        );

    private:

        CBus          m_busADR;
        CFast8BitBus  m_busDBUS;

        CFastPin      m_pinCLOCK;
        CFastPin      m_pinOPREQ;
        CFastPin      m_pinWRP;
        CFastPin      m_pin_OPACK;

};

#endif
