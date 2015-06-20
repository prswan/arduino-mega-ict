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
#ifndef CFastPin_h
#define CFastPin_h

#include "Arduino.h"
#include "Types.h"

//
// This is an optimized version of the digitalRead and digitalWrite functions.
// It's used for certain pins that are timing sensitive, such as clocks etc.
//
// Optimizations:
// 1) Not using a interface so it remains a static function call.
// 2) Performing the pin decoding in advance.
// 3) Flattening out the library digitalRead into a collection of single register reads.
//

class CFastPin
{
    public:

        CFastPin(
            const UINT8        pinMap[],
            const CONNECTION  *connection
        );

        void
        pinMode(
            int     mode
        );

        inline
        void
        digitalWrite(
            int value
        )
        {
            UINT8 rawBit = *(m_physicalPortRegisterOut);

            if (value == HIGH)
            {
                // Set the bit with an OR mask.
                rawBit |=  (m_physicalPinMask);
            }
            else
            {
                // Clear with bit with an AND invert  mask.
                rawBit &= ~(m_physicalPinMask);
            }

            *(m_physicalPortRegisterOut) = rawBit;
        };

        inline
        void
        digitalWriteLOW(
        )
        {
            UINT8 rawBit = *(m_physicalPortRegisterOut);

            // Clear with bit with an AND invert mask.
            rawBit &= m_physicalPinMaskInverted;

            *(m_physicalPortRegisterOut) = rawBit;
        };

        inline
        void
        digitalWriteHIGH(
        )
        {
            UINT8 rawBit = *(m_physicalPortRegisterOut);

            // Set the bit with an OR mask.
            rawBit |= (m_physicalPinMask);

            *(m_physicalPortRegisterOut) = rawBit;
        };

        inline
        int
        digitalRead(
        )
        {
            UINT8 rawBit = *(m_physicalPortRegisterIn);

            return ((rawBit & m_physicalPinMask) ? HIGH : LOW);
        };

    private:
        UINT8             m_decodedPinMap;
        UINT8             m_physicalPinMask;
        UINT8             m_physicalPinMaskInverted;
        volatile UINT8*   m_physicalPortRegisterIn;
        volatile UINT8*   m_physicalPortRegisterOut;

};

#endif
