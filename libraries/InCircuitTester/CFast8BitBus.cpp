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
#include "CFast8BitBus.h"


CFast8BitBus::CFast8BitBus(
    const UINT8        pinMap[],
    const CONNECTION   connection[],
    UINT16             numOfConnections
) : m_pinModeSet(false),
    m_currentPinMode(INPUT)
{
    for (UINT8 i = 0 ; i < s_dataBusSize ; i++)
    {
        UINT8 pin = pinMap[connection[i].pin];

        m_decodedPinMap[i] = pin;

        m_physicalPinMask[i]         = digitalPinToBitMask(pin);
        m_physicalPortRegisterIn[i]  = portInputRegister(digitalPinToPort(pin));
        m_physicalPortRegisterOut[i] = portOutputRegister(digitalPinToPort(pin));
    }
};


void
CFast8BitBus::pinMode(
    int     mode
)
{
    if ( !m_pinModeSet ||
         (mode != m_currentPinMode))
    {
        m_pinModeSet     = true;
        m_currentPinMode = mode;

        for (UINT8 i = 0 ; i < s_dataBusSize ; i++)
        {
            ::pinMode(m_decodedPinMap[i], mode);
        }
    }
};


void
CFast8BitBus::digitalWrite(
    UINT16  value
)
{
    for (UINT8 i = 0 ; i < s_dataBusSize; i++)
    {
        UINT8 rawBit = *(m_physicalPortRegisterOut[i]);

        if ((value >> i) & 1)
        {
            // Set the bit with an OR mask.
            rawBit |=  (m_physicalPinMask[i]);
        }
        else
        {
            // Clear with bit with an AND invert  mask.
            rawBit &= ~(m_physicalPinMask[i]);
        }

        *(m_physicalPortRegisterOut[i]) = rawBit;
    }
};


void
CFast8BitBus::digitalRead(
    UINT16  *value
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

