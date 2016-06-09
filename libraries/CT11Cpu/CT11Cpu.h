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
            AddressRemapCallback  addressRemapCallback,
            void                 *addressRemapCallbackContext
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
        // The T11 is a 16-bit CPU.
        //
        // T11 Address Space:
        // 0x00000000 -> 8-bit access, 0x*0 = Lo & 0x*1 == Hi.
        // 0x01000000 -> 16-bit access flag
        // 0x02000000 -> VSYNC access flag
        //
        // Atari System 2 address remap callback further defines
        // 0x00300000 -> Paged ROM Chip Select
        // 0x000F0000 -> Extended address range (128Kb ROM addresses)
        //

        virtual PERROR memoryRead(
            UINT32 address,
            UINT16  *data
        );

        virtual PERROR memoryWrite(
            UINT32 address,
            UINT16  data
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
        // CT11Cpu Interface
        //

    private:

        PERROR
        memoryReadWrite(
            UINT32 address,
            UINT16 *data,
            UINT8  *ai,
            bool   read
        );

    private:

        AddressRemapCallback  m_addressRemapCallback;
        void                 *m_addressRemapCallbackContext;

        CFast8BitBus  m_busDALHi;
        CFast8BitBus  m_busDALLo;
        CFast8BitBus  m_busAI;

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
