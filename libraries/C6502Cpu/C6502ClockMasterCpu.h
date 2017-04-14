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
#ifndef C6502ClockMasterCpu_h
#define C6502ClockMasterCpu_h

#include "Arduino.h"
#include "ICpu.h"
#include "CBus.h"
#include "CFast8BitBus.h"
#include "CFastPin.h"


class C6502ClockMasterCpu : public ICpu
{
    public:

        //
        // Constructor
        //
        // Some games (e.g. Astro Fighter) have strictly synchronous bus
        // cycles clocked from the external master clock.
        // The "CLK2oHiToD" setting allows the cycle time to be specified
        // such that the data read point is performed after
        // "CLK2oHiToD" pulses. Typically this is to the n-1 pulse for
        // an n clock pulse cycle.
        // If "CLK2oHiToD" value is zero then the cycle is treated as being
        // asynchronous.
        //
        //                                             End of cycle
        //                                             ^
        // CLK2o  ----\                /---------------\
        //            |                |               |
        //            \----------------/               \----
        //                             .
        //                             .
        // CLK2oHiToD                  .------------>
        //                                          ^
        //                                          D read

        C6502ClockMasterCpu(
            UINT8 CLK2oHiToDInClockPulses
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

        // Address Space:
        // 0x00000 -> 0x0FFFF - Memory Mapped Data
        //

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
            UINT32    timeoutInClockPulses
        );

        virtual
        PERROR
        acknowledgeInterrupt(
            UINT16 *response
        );

        //
        // C6502ClockMasterCpu Interface
        //

        void
        clockPulse(
        );

    private:

        PERROR
        memoryReadWrite(
            UINT32 address,
            UINT16 *data,
            int    readWrite
        );


    private:

        UINT8         m_CLK2oHiToDInClockPulses;

        CBus          m_busA;
        CFast8BitBus  m_busD;

        CFastPin      m_pinCLK0i;
        CFastPin      m_pinCLK1o;
        CFastPin      m_pinCLK2o;
        CFastPin      m_pinRDY;

        CFastPin      m_pinClock;

        int           m_valueCLK1o;
        int           m_valueCLK2o;

};

#endif
