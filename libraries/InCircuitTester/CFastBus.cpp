//
// Copyright (c) 2019, Paul R. Swan
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
#include "CFastBus.h"


CFastBus::CFastBus(
    const UINT8        pinMap[],
    const CONNECTION   connection[],
    UINT8              numOfConnections
) : m_dataBusSize(numOfConnections),
    m_pinModeSet(false),
    m_currentPinMode(INPUT)
{
    m_decodedPinMap            = (UINT8*)malloc(m_dataBusSize * sizeof(*m_decodedPinMap));
    m_physicalPinMask          = (UINT8*)malloc(m_dataBusSize * sizeof(*m_physicalPinMask));
    m_physicalPortRegisterIn   = (volatile UINT8**)malloc(m_dataBusSize * sizeof(*m_physicalPortRegisterIn));
    m_physicalPortRegisterOut  = (volatile UINT8**)malloc(m_dataBusSize * sizeof(*m_physicalPortRegisterOut));
    m_physicalPortRegisterMode = (volatile UINT8**)malloc(m_dataBusSize * sizeof(*m_physicalPortRegisterMode));

    for (UINT8 i = 0 ; i < m_dataBusSize ; i++)
    {
        UINT8 pin = pinMap[connection[i].pin];

        m_decodedPinMap[i] = pin;

        m_physicalPinMask[i]          = digitalPinToBitMask(pin);
        m_physicalPortRegisterIn[i]   = portInputRegister(digitalPinToPort(pin));
        m_physicalPortRegisterOut[i]  = portOutputRegister(digitalPinToPort(pin));
        m_physicalPortRegisterMode[i] = portModeRegister(digitalPinToPort(pin));
    }
};


CFastBus::~CFastBus(
)
{
    free(m_decodedPinMap);
    free(m_physicalPinMask);
    free(m_physicalPortRegisterIn);
    free(m_physicalPortRegisterOut);
    free(m_physicalPortRegisterMode);
};


void
CFastBus::pinMode(
    int     mode
)
{
    if ( !m_pinModeSet ||
         (mode != m_currentPinMode))
    {
        m_pinModeSet     = true;
        m_currentPinMode = mode;

        for (UINT8 i = 0 ; i < m_dataBusSize; i++)
        {
            UINT8 rawMode = *(m_physicalPortRegisterMode[i]);

            if (mode == OUTPUT)
            {
                // Set the bit with an OR mask.
                rawMode |=  (m_physicalPinMask[i]);

                *(m_physicalPortRegisterMode[i]) = rawMode;
            }
            else
            {
                // Clear with bit with an AND invert mask.
                rawMode &= ~(m_physicalPinMask[i]);

                *(m_physicalPortRegisterMode[i]) = rawMode;

                // Also set the output pullup control
                {
                    UINT8 rawOut = *(m_physicalPortRegisterOut[i]);

                    if (mode == INPUT_PULLUP)
                    {
                        // Set the bit with an OR mask.
                        rawOut |=  (m_physicalPinMask[i]);
                    }
                    else
                    {
                        // Clear with bit with an AND invert mask.
                        rawOut &= ~(m_physicalPinMask[i]);
                    }

                    *(m_physicalPortRegisterOut[i])  = rawOut;
                }
            }
        }
    }
};


void
CFastBus::digitalWrite(
    UINT16  value
)
{
    for (UINT8 i = 0 ; i < m_dataBusSize; i++)
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
CFastBus::digitalRead(
    UINT16  *value
)
{
    UINT16 localValue = 0;

    for (UINT8 i = 0 ; i < m_dataBusSize; i++)
    {
        UINT8 rawIn = *(m_physicalPortRegisterIn[i]);

        if (rawIn & m_physicalPinMask[i])
        {
            // Set the bit with an OR mask.
            localValue |=  (1 << i);
        }
        else
        {
            //
            // Clear with bit with an AND invert mask.
            // This is redundant but left in code to keep
            // the execution time more stable on the scope.
            //
            localValue &= ~(1 << i);
        }
    }

    *value = localValue;
}

