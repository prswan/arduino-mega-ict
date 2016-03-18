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
#ifndef CT11Cpu_h
#define CT11Cpu_h

#include "Arduino.h"
#include "ICpu.h"
#include "CBus.h"
#include "CFast8BitBus.h"
#include "CFastPin.h"


class CT11Cpu : public ICpu
{
    public:

        //
        // Constructor
        //

        CT11Cpu(
        );

        // ICpu Interface
        //

        virtual PERROR idle(
        );

        virtual PERROR check(
        );

        // The T11 is a 16-bit CPU.
        // As a result A0 is not used and the address
        // is shifted down by 1 bit as a result (we can't
        // leave A0 toggling because callers expect 8-bit
        // address access through this interface).
        //
        // T11 Address Space:
        // 0x00000 -> 0x0FFFF - not used
        // 0x10000 -> 0x17FFF - Memory Mapped Data, Lo Byte Access (D0-D7)
        // 0x20000 -> 0x27FFF - Memory Mapped Data, Hi Byte Access (D8-D15)
        // 0x30000 -> 0x37FFF - Memory Mapped Data, 16-Bit Word Access
        //

        virtual PERROR memoryRead(
            UINT32 address,
            UINT8  *data
        );

        virtual PERROR memoryWrite(
            UINT32 address,
            UINT8  data
        );

        virtual
        PERROR
        waitForInterrupt(
            Interrupt interrupt,
            UINT16    timeoutInMs
        );

        virtual
        PERROR
        acknowledgeInterrupt(
            UINT8 *response
        );

        //
        // CT11Cpu Interface
        //

    private:

        PERROR
        memoryReadWrite(
            UINT32 address,
            UINT16 *data,
            bool   read
        );

    private:

        CFast8BitBus  m_busDALHi;
        CFast8BitBus  m_busDALLo;

        CFastPin      m_pinPI;
        CFastPin      m_pinSEL0;
        CFastPin      m_pinSEL1;

        CFastPin      m_pinR_WHB;
        CFastPin      m_pinR_WLB;
        CFastPin      m_pin_RAS;
        CFastPin      m_pin_CAS;
        CFastPin      m_pinCOUT;

        CFastPin      m_pinXTL1;

};

#endif
