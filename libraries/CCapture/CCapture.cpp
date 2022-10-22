//
// Copyright (c) 2022 Paul R. Swan
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
#include "CCapture.h"
#include "C6809EClockMasterCpu.h"

#include "PinMap.h"

//
// External capture input on J14 AUX pin 1.
//
static const CONNECTION s_AUX1_i = {1, "AUX1"};


CCapture::CCapture(
    ICpu *cpu
) : m_cpu(cpu)
{
}


CCapture::~CCapture(
)
{
}


//
// Prototype Signal Capture
// ------------------------
// Capture the clocked input of aux pin 1 and display the hex result.
// The 4 hex digits covers 32 clocks.
//
PERROR
CCapture::capture32(
)
{
    C6809EClockMasterCpu *cpu = (C6809EClockMasterCpu *) m_cpu;
    PERROR error = errorCustom;
    UINT8 capture[4] = {0,0,0,0};

    ::pinMode(g_pinMap8Aux[s_AUX1_i.pin], INPUT);

    for (int byte = 0 ; byte < 4 ; byte++)
    {
        for (int bit = 0 ; bit < 8 ; bit++)
        {
            cpu->clockPulse();

            int value = ::digitalRead(g_pinMap8Aux[s_AUX1_i.pin]);

            capture[byte] = capture[byte] << 1;
            if (value == HIGH)
            {
                capture[byte] |= 1;
            }
        }
    }

    error->code = ERROR_SUCCESS;
    error->description = "OK: ";

    for (int byte = 0 ; byte < 4 ; byte++)
    {
        STRING_UINT8_HEX(error->description, capture[byte]);
    }

    return error;
}

