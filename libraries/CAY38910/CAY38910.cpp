//
// Copyright (c) 2018, Paul R. Swan
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
#include "CAY38910.h"


static const UINT8 AY_R00_CHA_FINE_TONE     = 0x0;
static const UINT8 AY_R01_CHA_COARSE_TONE   = 0x1;
static const UINT8 AY_R02_CHB_FINE_TONE     = 0x2;
static const UINT8 AY_R03_CHB_COARSE_TONE   = 0x3;
static const UINT8 AY_R04_CHC_FINE_TONE     = 0x4;
static const UINT8 AY_R05_CHC_COARSE_TONE   = 0x5;
static const UINT8 AY_R06_NOISE_PERIOD      = 0x6;
static const UINT8 AY_R07_ENABLE            = 0x7;
static const UINT8 AY_R10_CHA_AMPLITUDE     = 0x8;
static const UINT8 AY_R11_CHB_AMPLITUDE     = 0x9;
static const UINT8 AY_R12_CHC_AMPLITUDE     = 0xA;
static const UINT8 AY_R13_FINE_ENV_PERIOD   = 0xB;
static const UINT8 AY_R14_COARSE_ENV_PERIOD = 0xC;
static const UINT8 AY_R15_COARSE_ENV_SHAPE  = 0xD;
static const UINT8 AY_R16_PORT_A_DATA       = 0xE;
static const UINT8 AY_R17_PORT_B_DATA       = 0xF;


CAY38910::CAY38910(
    ICpu   *cpu,
    UINT32 regAddress,
    UINT32 regData
) : m_cpu(cpu),
    m_regAddress(regAddress),
    m_regData(regData)
{
}


CAY38910::~CAY38910(
)
{
}


//
// Clear back to post-reset state.
//
PERROR
CAY38910::idle(
)
{
    PERROR error = errorSuccess;

    for (UINT8 reg = 0 ; reg < 16 ; reg++)
    {
        error = write(reg, 0x00);

        if (FAILED(error))
        {
            break;
        }
    }

    return error;
}


//
// Simple read/write test
//
PERROR
CAY38910::check(
)
{
    PERROR error = errorSuccess;

    for (UINT8 exp = 0 ; exp < 250 ; exp++)
    {
        // Write
        {
            error = write(AY_R00_CHA_FINE_TONE, (exp + 1));
            if (FAILED(error))
            {
                break;
            }

            error = write(AY_R02_CHB_FINE_TONE, (exp + 2));
            if (FAILED(error))
            {
                break;
            }

            error = write(AY_R04_CHC_FINE_TONE, (exp + 3));
            if (FAILED(error))
            {
                break;
            }
        }
        // Read/Verify
        {
            UINT8 rec = 0;

            error = read(AY_R00_CHA_FINE_TONE, &rec);
            if (FAILED(error))
            {
                break;
            }

            CHECK_VALUE_UINT8_BREAK(error, "AY3", AY_R00_CHA_FINE_TONE, (exp + 1), rec);

            error = read(AY_R02_CHB_FINE_TONE, &rec);
            if (FAILED(error))
            {
                break;
            }

            CHECK_VALUE_UINT8_BREAK(error, "AY3", AY_R02_CHB_FINE_TONE, (exp + 2), rec);

            error = read(AY_R04_CHC_FINE_TONE, &rec);
            if (FAILED(error))
            {
                break;
            }

            CHECK_VALUE_UINT8_BREAK(error, "AY3", AY_R04_CHC_FINE_TONE, (exp + 3), rec);
        }
    }

    return error;
}


//
// Generate noise on the specified channel.
// Unlike a tone, the noise output is more suited to additional testing of
// external filter.
//
PERROR
CAY38910::noise(
    Channel channel
)
{
    PERROR error = errorSuccess;

    UINT8 noisePeriod   = 0x08;
    UINT8 amplitudeData = 0x09; // 0x0F, max, is a bit loud
    UINT8 enable        = 0x3F;

    UINT8 amplitudeReg;

    switch (channel)
    {
        case CHA :
        {
            enable ^= 0x08;
            amplitudeReg = AY_R10_CHA_AMPLITUDE;
            break;
        }

        case CHB :
        {
            enable ^= 0x10;
            amplitudeReg = AY_R11_CHB_AMPLITUDE;
            break;
        }

        case CHC :
        {
            enable ^= 0x20;
            amplitudeReg = AY_R12_CHC_AMPLITUDE;
            break;
        }

        default:
        {
            error = errorNotImplemented;
            break;
        }
    }

    error = write(AY_R06_NOISE_PERIOD, noisePeriod);
    if (SUCCESS(error))
    {
        error = write(AY_R07_ENABLE, enable);
    }

    if (SUCCESS(error))
    {
        error = write(amplitudeReg, amplitudeData);
    }

    return error;
}


PERROR
CAY38910::read(
    UINT8 reg,
    UINT8 *data
)
{
    PERROR error = errorSuccess;
    UINT16 data16 = 0;

    error = m_cpu->memoryWrite(m_regAddress, (UINT16) reg);

    if (SUCCESS(error))
    {
        error = m_cpu->memoryRead(m_regData, &data16);
    }

    *data = (UINT8) data16;

    return error;
}


PERROR
CAY38910::write(
    UINT8 reg,
    UINT8 data
)
{
    PERROR error = errorSuccess;

    error = m_cpu->memoryWrite(m_regAddress, (UINT16) reg);

    if (SUCCESS(error))
    {
        error = m_cpu->memoryWrite(m_regData, (UINT16) data);
    }

    return error;
}


