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
#include "CBus.h"


CBus::CBus(
    const UINT8       pinMap[],
    const CONNECTION  connection[],
    UINT16            numOfConnections
) : m_pinMap(pinMap),
    m_connection(connection),
    m_numOfConnections(numOfConnections),
    m_pinModeSet(false),
    m_currentPinMode(INPUT)
{
}

void
CBus::pinMode(
    int    mode
)
{
    if ( !m_pinModeSet ||
         (mode != m_currentPinMode))
    {
        m_pinModeSet     = true;
        m_currentPinMode = mode;

        for (UINT16 uiIndex = 0 ; uiIndex < m_numOfConnections ; uiIndex++)
        {
            ::pinMode(m_pinMap[m_connection[uiIndex].pin], mode);
        }
    }
}

void
CBus::digitalWrite(
    UINT16 value
)
{
    for (UINT16 uiIndex = 0 ; uiIndex < m_numOfConnections ; uiIndex++)
    {
        int hiLo = ((value >> uiIndex) & 1) ? HIGH : LOW;

        ::digitalWrite(m_pinMap[m_connection[uiIndex].pin], hiLo);
    }
}

void
CBus::digitalRead(
    UINT16 *value
)
{
    UINT16 localValue = 0;

    for (UINT16 uiIndex = 0 ; uiIndex < m_numOfConnections ; uiIndex++)
    {

        int hiLo = ::digitalRead(m_pinMap[m_connection[uiIndex].pin]);

        if (hiLo == HIGH)
        {
            localValue |= (1 << uiIndex);
        }
    }

    *value = localValue;
}


