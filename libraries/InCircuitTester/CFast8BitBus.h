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
#ifndef CFast8BitBus_h
#define CFast8BitBus_h

#include "Arduino.h"
#include "Types.h"

#include "CFastPin.h"

//
// There are some devices do not hold the data bus on a read beyond a specific time
// (e.g. the 2636 only holds data on the bus for 2uS).
// Therefore, we need an optimized way to read a data bus in as few clocks as possible.
//
// Optimizations:
// 1) Not using a interface so it remains a static function call.
// 2) Performing the pin decoding in advance.
// 3) Unrolling the digitalRead loop to so all the reads as fast as possible up front.
// 4) Flattening out the library digitalRead into a collection of single register reads.

class CFast8BitBus
{
    public:

        CFast8BitBus(
            const UINT8        pinMap[],
            const CONNECTION   connection[],
            UINT16             numOfConnections
        );

        void
        pinMode(
            int     mode
        );

        void
        digitalWrite(
            UINT16  value
        );

        void
        digitalRead(
            UINT16  *value
        );

        inline
        void
        digitalReadThenDigitalWriteLOW(
            UINT16   *value,
            CFastPin &fastPin
        )
        {
            UINT8 rawBit0 = *(m_physicalPortRegisterIn[0]);
            UINT8 rawBit1 = *(m_physicalPortRegisterIn[1]);
            UINT8 rawBit2 = *(m_physicalPortRegisterIn[2]);
            UINT8 rawBit3 = *(m_physicalPortRegisterIn[3]);
            UINT8 rawBit4 = *(m_physicalPortRegisterIn[4]);
            UINT8 rawBit5 = *(m_physicalPortRegisterIn[5]);
            UINT8 rawBit6 = *(m_physicalPortRegisterIn[6]);
            UINT8 rawBit7 = *(m_physicalPortRegisterIn[7]);

            fastPin.digitalWriteLOW();

            UINT16 localValue = 0;

            localValue |= ((rawBit0 & m_physicalPinMask[0]) ? (1 << 0) : 0);
            localValue |= ((rawBit1 & m_physicalPinMask[1]) ? (1 << 1) : 0);
            localValue |= ((rawBit2 & m_physicalPinMask[2]) ? (1 << 2) : 0);
            localValue |= ((rawBit3 & m_physicalPinMask[3]) ? (1 << 3) : 0);
            localValue |= ((rawBit4 & m_physicalPinMask[4]) ? (1 << 4) : 0);
            localValue |= ((rawBit5 & m_physicalPinMask[5]) ? (1 << 5) : 0);
            localValue |= ((rawBit6 & m_physicalPinMask[6]) ? (1 << 6) : 0);
            localValue |= ((rawBit7 & m_physicalPinMask[7]) ? (1 << 7) : 0);

            *value = localValue;
        }

        inline
        void
        digitalReadThenDigitalWriteHIGH(
            UINT16   *value,
            CFastPin &fastPin
        )
        {
            UINT8 rawBit0 = *(m_physicalPortRegisterIn[0]);
            UINT8 rawBit1 = *(m_physicalPortRegisterIn[1]);
            UINT8 rawBit2 = *(m_physicalPortRegisterIn[2]);
            UINT8 rawBit3 = *(m_physicalPortRegisterIn[3]);
            UINT8 rawBit4 = *(m_physicalPortRegisterIn[4]);
            UINT8 rawBit5 = *(m_physicalPortRegisterIn[5]);
            UINT8 rawBit6 = *(m_physicalPortRegisterIn[6]);
            UINT8 rawBit7 = *(m_physicalPortRegisterIn[7]);

            fastPin.digitalWriteHIGH();

            UINT16 localValue = 0;

            localValue |= ((rawBit0 & m_physicalPinMask[0]) ? (1 << 0) : 0);
            localValue |= ((rawBit1 & m_physicalPinMask[1]) ? (1 << 1) : 0);
            localValue |= ((rawBit2 & m_physicalPinMask[2]) ? (1 << 2) : 0);
            localValue |= ((rawBit3 & m_physicalPinMask[3]) ? (1 << 3) : 0);
            localValue |= ((rawBit4 & m_physicalPinMask[4]) ? (1 << 4) : 0);
            localValue |= ((rawBit5 & m_physicalPinMask[5]) ? (1 << 5) : 0);
            localValue |= ((rawBit6 & m_physicalPinMask[6]) ? (1 << 6) : 0);
            localValue |= ((rawBit7 & m_physicalPinMask[7]) ? (1 << 7) : 0);

            *value = localValue;
        }

    private:
        static const int  s_dataBusSize = 8;

        UINT8             m_decodedPinMap[s_dataBusSize];
        UINT8             m_physicalPinMask[s_dataBusSize];
        volatile UINT8*   m_physicalPortRegisterIn[s_dataBusSize];
        volatile UINT8*   m_physicalPortRegisterOut[s_dataBusSize];

        //
        // Simple speed optimization to skip setting a pin mode when
        // it's already been set.
        //

        bool              m_pinModeSet;
        int               m_currentPinMode;

};

#endif
